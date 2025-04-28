#include <iostream>
#include <list>
#include <algorithm>
#include "ppu.hpp"
#include <bitset>

const int LCDC_MAP_CHOICE_MASK = 0x08;
const int LCDC_ADDRESSING_MODE_MASK = 0x10;
const int TILE_MAP_1 = 0x9800;
const int TILE_MAP_2 = 0x9C00;
const int TILE_DATA_1 = 0x8000;
const int TILE_DATA_2 = 0x9000;
const int TILE_DATA_SIZE = 16;
const int TILE_WIDTH = 8;
const int TILE_HEIGHT = 8;
const int MAP_WIDTH = 32;
const int MAP_HEIGHT = 32;

const uint16_t OAM_START = 0xFE00;
const int16_t SPRITE_Y_OFFSET = 16;
const int16_t SPRITE_X_OFFSET = 8;

PPU::PPU() : mmu(nullptr)
{
	mode = 2;
	clock = 0;
	for (int i = 0; i < SCREEN_HEIGHT; i++)
	{
		for (int j = 0; j < SCREEN_WIDTH; j++)
		{
			pixelsToRender[i][j] = 0xFFFFFFFF; // white
			pixelData[i][j] = WHITE_OR_TRANSPARENT;
			backgroundData[i][j] = WHITE_OR_TRANSPARENT;
			windowData[i][j] = WINDOW_TRANSPARENT;
			spriteData[i][j] = WHITE_OR_TRANSPARENT;
		}
	}
}

PPU::~PPU()
{
}

void PPU::connect_mmu(MMU *mmu_ptr)
{
	this->mmu = mmu_ptr;
}

void PPU::connect_ram(RAM *ram_ptr)
{
	this->ram = ram_ptr;
}

void PPU::connect_interrupt_handler(InterruptHandler *IH)
{
	this->IH = IH;
}

bool PPU::tick(uint64_t outsideClock)
{
	outsideClock *= 4;
	updateRegs();
	bool render_on_return = false;

	// check for LY=LYC and STAT interrupt
	uint8_t stat = read_mem(0xFF41);
	uint8_t LY = read_mem(0xFF44);
	uint8_t LYC = read_mem(0xFF45);
	if (LY == LYC)
	{
		stat |= 0b00000100; // set the LYC=LY flag
		if ((stat & 0b01000000))
		{
			IH->enable_STAT_interrupt();
		}
	}

	// perform different actions depending on the mode
	switch (mode)
	{
	case 2: // OAM
		if (dma_transfer)
		{
			uint8_t dma_reg = ram->read_mem(0xFF46);
			uint16_t transfer_address = (static_cast<uint16_t>(dma_reg) << 8);
			for (int i = 0; i < 160; i++)
			{
				ram->write_mem(OAM_START + i, ram->read_mem(transfer_address + i));
			}
			dma_transfer = false;
		}
		scanOAM(scanLine);
		if (outsideClock - clock >= 80)
		{
			// switch to VRAM mode
			mode = 3;
			clock = outsideClock;
		}
		break;
	case 3: // VRAM
		if (outsideClock - clock >= 172)
		{
			updateBackground(scanLine);
			updateWindow(scanLine);
			updateSprites(scanLine);
			updatePixelData(scanLine);

			// switch to HBLANK mode
			mode = 0;
			// check for STAT interrupt
			uint8_t stat = read_mem(0xFF41);
			if (stat & 0b00001000)
			{
				IH->enable_STAT_interrupt();
			}
			clock = outsideClock;
		}
		break;
	case 0: // HBLANK
		// check for end of HBLANK
		if (outsideClock - clock >= 204)
		{
			// check for VBLANK switch
			if (scanLine == 144)
			{
				mode = 1;
				// check for STAT interrupt
				uint8_t stat = read_mem(0xFF41);
				if (stat & 0b00010000)
				{
					IH->enable_STAT_interrupt();
				}
				IH->enable_VBLANK_interrupt();
				render_on_return = true;
			}
			else
			{
				// switch back to OAM
				mode = 2;
				// check for STAT interrupt
				uint8_t stat = read_mem(0xFF41);
				if (stat & 0b00100000)
				{
					IH->enable_STAT_interrupt();
				}
				scanLine++;
			}
			clock = outsideClock;
		}
		break;
	case 1: // VBLANK
		if (scanLine > 154)
		{
			// switch back to rendering/OAM
			mode = 2;
			// check for STAT interrupt
			uint8_t stat = read_mem(0xFF41);
			if (stat & 0b00100000)
			{
				IH->enable_STAT_interrupt();
			}
			clock = outsideClock;
			scanLine = 0;
		}
		else
		{
			if (outsideClock - clock >= 456)
			{
				// increment scanline
				scanLine++;
				clock = outsideClock;
			}
			else
			{
				// std::cout << "CHILLIN IN VBLANK\n";
			}
		}
		break;
	default:
		std::cerr << "PPU Error: Unrecognized mode.\n";
	}

	update_LY();
	update_LCDSTAT();

	return render_on_return;
}

void PPU::update_LY()
{
	ram->write_mem(0xFF44, scanLine);
}

void PPU::update_LCDSTAT()
{
	uint8_t stat = read_mem(0xFF41);
	stat &= 0b11111100; // clear the mode bits
	stat |= mode;		// set the new mode
	ram->write_mem(0xFF41, stat);
}

void PPU::updatePixelData(uint8_t row)
{
	// mix the layers
	for (int j = 0; j < SCREEN_WIDTH; j++)
	{
		if ((LCDC_reg & 0b10) && spriteData[row][j] != WHITE_OR_TRANSPARENT)
		{ // sprites enabled
			pixelData[row][j] = spriteData[row][j];
		}
		else if ((LCDC_reg & 0b100000) && (LCDC_reg & 1) && windowData[row][j] != WINDOW_TRANSPARENT)
		{ // window enabled
			pixelData[row][j] = windowData[row][j];
		}
		else if (LCDC_reg & 1)
		{ // display background
			pixelData[row][j] = backgroundData[row][j];
		}
		else
		{
			pixelData[row][j] = WHITE_OR_TRANSPARENT; // bg/window not enabled, set to white
		}
	}

	// now, convert COLOR array into hex array
	for (int j = 0; j < SCREEN_WIDTH; j++)
	{
		// All pixels are in ARGB format (1 byte per info)
		switch (pixelData[row][j])
		{
		case WHITE_OR_TRANSPARENT:
			pixelsToRender[row][j] = 0xFFFFFFFF;
			break;
		case LIGHT_GRAY:
			pixelsToRender[row][j] = 0xFFAAAAAA;
			break;
		case DARK_GRAY:
			pixelsToRender[row][j] = 0xFF555555;
			break;
		case BLACK:
			pixelsToRender[row][j] = 0xFF000000;
			break;
		default:
			pixelsToRender[row][j] = 0xFFFFFFFF; // default to white for any other case
		}
	}
}

void PPU::updateRegs()
{
	// update the registers for graphics
	LCDC_reg = read_mem(0xFF40);
	// std::cout << "LCDC reg: " << std::bitset<8>(LCDC_reg) << '\n';
	SCY_reg = read_mem(0xFF42);
	SCX_reg = read_mem(0xFF43);
	WY_reg = read_mem(0xFF4A);
	WX_reg = read_mem(0xFF4B);
	BGP_reg = read_mem(0xFF47);
	OBP0_reg = read_mem(0xFF48);
	OBP1_reg = read_mem(0xFF49);
}

void PPU::updateBackground(uint8_t row)
{
	// get bg palette from register
	COLOR bg_palette[4];
	bg_palette[0] = static_cast<COLOR>(BGP_reg & 0b11);
	bg_palette[1] = static_cast<COLOR>((BGP_reg >> 2) & 0b11);
	bg_palette[2] = static_cast<COLOR>((BGP_reg >> 4) & 0b11);
	bg_palette[3] = static_cast<COLOR>((BGP_reg >> 6) & 0b11);

	// which tile map to use?
	uint16_t map_addr = (LCDC_reg & LCDC_MAP_CHOICE_MASK) ? TILE_MAP_2 : TILE_MAP_1;
	// start at 0x8000 w/ unsigned offsets OR at 0x9000 w/ signed offsets
	bool simple_addressing_mode = (LCDC_reg & LCDC_ADDRESSING_MODE_MASK) == 0;
	uint16_t tiles_addr = simple_addressing_mode ? TILE_DATA_1 : TILE_DATA_2;

	uint16_t map_row = map_addr + ((SCY_reg + row) / TILE_HEIGHT * MAP_WIDTH) % (MAP_WIDTH * MAP_HEIGHT);
	uint16_t tile_row = (SCY_reg + row) % TILE_HEIGHT;
	for (int i = 0; i < SCREEN_WIDTH;)
	{
		uint16_t map_col = map_row + ((SCX_reg + i) / TILE_WIDTH) % MAP_WIDTH;
		uint16_t tile_col = (SCX_reg + i) % TILE_WIDTH;
		uint16_t tile_addr;
		if (simple_addressing_mode)
		{
			uint16_t tile_offset = read_mem(map_col);
			tile_addr = tiles_addr + tile_offset * TILE_DATA_SIZE;
		}
		else
		{
			int16_t tile_offset = static_cast<int8_t>(read_mem(map_col));
			tile_addr = tiles_addr + tile_offset * TILE_DATA_SIZE;
		}
		uint8_t lsbs = read_mem(tile_addr + tile_row * 2);
		uint8_t msbs = read_mem(tile_addr + tile_row * 2 + 1);
		for (int j = tile_col; j < TILE_WIDTH; j++)
		{
			uint8_t color = ((lsbs >> (7 - j)) & 1) | (((msbs >> (7 - j)) & 1) << 1);
			backgroundData[row][i] = bg_palette[color];
			i++;
		}
	}

	// keeping this just in case i'm bad at coding
	// COLOR full_map[MAP_HEIGHT * TILE_HEIGHT][MAP_WIDTH * TILE_WIDTH];
	// uint16_t tiles_addr = simple_addressing_mode ? TILE_DATA_1 : TILE_DATA_2;
	// for (int i = 0; i < MAP_HEIGHT * TILE_HEIGHT; i += TILE_HEIGHT)
	// {
	// 	for (int j = 0; j < MAP_WIDTH * TILE_WIDTH; j += TILE_WIDTH)
	// 	{
	// 		// get tiles in this row
	// 		uint8_t tile_addr;
	// 		// transform tile data into pixel data
	// 		for (int k = 0; k < TILE_HEIGHT; k++)
	// 		{
	// 			uint8_t lsbs = read_mem(tile_addr + k * 2);
	// 			uint8_t msbs = read_mem(tile_addr + k * 2 + 1);
	// 			for (int l = 0; l < TILE_WIDTH; l++)
	// 			{
	// 				// get color from each bit pair and store in map
	// 				uint8_t color = ((lsbs >> (7 - l)) & 1) | (((msbs >> (7 - l)) & 1) << 1);
	// 				full_map[i + k][j + l] = bg_palette[color];
	// 			}
	// 		}
	// 	}
	// }

	// // now, apply scroll
	// for (int i = 0; i < SCREEN_HEIGHT; i++)
	// {
	// 	for (int j = 0; j < SCREEN_WIDTH; j++)
	// 	{
	// 		// use SCX, SCY regs as offsets w/ wrap-around
	// 		int y = (i + SCX_reg) % (MAP_HEIGHT * TILE_HEIGHT);
	// 		int x = (j + SCY_reg) % (MAP_WIDTH * TILE_WIDTH);
	// 		backgroundData[i][j] = full_map[y][x];
	// 	}
	// }
}

void PPU::updateWindow(uint8_t row)
{
	// get bg palette from register
	COLOR bg_palette[4];
	bg_palette[0] = static_cast<COLOR>(BGP_reg & 0b11);
	bg_palette[1] = static_cast<COLOR>((BGP_reg >> 2) & 0b11);
	bg_palette[2] = static_cast<COLOR>((BGP_reg >> 4) & 0b11);
	bg_palette[3] = static_cast<COLOR>((BGP_reg >> 6) & 0b11);

	// initializing to transparent
	for (int i = 0; i < 160; i++)
	{
		windowData[row][i] = WINDOW_TRANSPARENT;
	}

	if (!(LCDC_reg & 0b00100000))
	{
		// window disabled
		return;
	}

	uint8_t x = WX_reg - 7;
	uint8_t y = WY_reg;
	if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
	{
		return;
	}

	bool unsigned_addressing = (LCDC_reg >> 4) & 0b1;
	int basePointer = unsigned_addressing ? TILE_MAP_1 : TILE_MAP_2;

	uint8_t tileMapPointer = (LCDC_reg >> 6) & 0b1 ? TILE_MAP_2 : TILE_MAP_1;
	// iterating through the tile map, skipping tiles and pixels not displayed
	for (int r = 0; r < MAP_HEIGHT; r++)
	{
		// this row of tiles is not visible
		if (y + r * TILE_HEIGHT >= SCREEN_HEIGHT)
		{
			continue;
		}
		for (int c = 0; c < MAP_WIDTH; c++)
		{
			// this column of tiles is not visible
			if (x + c * TILE_WIDTH >= SCREEN_WIDTH)
			{
				continue;
			}

			// finding address in memory of start of current tile
			uint16_t tileAddr;
			if (unsigned_addressing)
			{
				uint8_t tileAddrOffset = read_mem(tileMapPointer);
				tileAddr = basePointer + tileAddrOffset * TILE_DATA_SIZE;
			}
			else
			{
				int8_t tileAddrOffset = static_cast<int8_t>(read_mem(tileMapPointer));
				tileAddr = basePointer + static_cast<int16_t>(tileAddrOffset) * TILE_DATA_SIZE;
			}

			// iterating through bytes of tile data
			for (int i = 0; i < 16; i += 2)
			{
				// this tile row of pixels is not visible
				if (y + r * TILE_HEIGHT + i / 2 != row)
				{
					continue;
				}

				uint8_t lsbs = read_mem(tileAddr + i);
				uint8_t msbs = read_mem(tileAddr + i + 1);

				// displaying this tile row of pixels
				for (int j = 0; j < 8; j++)
				{
					// this tile column of pixels is not visible
					if (x + r * TILE_WIDTH + j >= SCREEN_WIDTH)
					{
						continue;
					}
					uint8_t color = ((lsbs >> (7 - j)) & 1) | (((msbs >> (7 - j)) & 1) << 1);
					windowData[y + r * TILE_HEIGHT + i / 2][x + r * TILE_WIDTH + j] = bg_palette[color];
				}
			}

			// finding next tile address
			tileMapPointer++;
		}
	}
}

void PPU::updateSprites(uint8_t row)
{
	if (!spriteBuffer.empty())
		spriteBuffer.sort([](const Sprite &sp1, const Sprite &sp2)
						  { return sp1.x < sp2.x; });

	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		spriteData[row][i] = WHITE_OR_TRANSPARENT;
	}

	COLOR obp0_palette[4];
	obp0_palette[0] = static_cast<COLOR>(OBP0_reg & 0b11);
	obp0_palette[1] = static_cast<COLOR>((OBP0_reg >> 2) & 0b11);
	obp0_palette[2] = static_cast<COLOR>((OBP0_reg >> 4) & 0b11);
	obp0_palette[3] = static_cast<COLOR>((OBP0_reg >> 6) & 0b11);
	COLOR obp1_palette[4];
	obp1_palette[0] = static_cast<COLOR>(OBP1_reg & 0b11);
	obp1_palette[1] = static_cast<COLOR>((OBP1_reg >> 2) & 0b11);
	obp1_palette[2] = static_cast<COLOR>((OBP1_reg >> 4) & 0b11);
	obp1_palette[3] = static_cast<COLOR>((OBP1_reg >> 6) & 0b11);

	bool tall_sprites = LCDC_reg & 0b100;
	int sprite_height = tall_sprites ? TILE_HEIGHT * 2 : TILE_HEIGHT;

	while (!spriteBuffer.empty())
	{
		Sprite sprite = spriteBuffer.front();
		spriteBuffer.pop_front();

		int16_t y = sprite.y - SPRITE_Y_OFFSET;
		int16_t x = sprite.x - SPRITE_X_OFFSET;
		uint8_t tileIndex = sprite.tileIndex;
		uint8_t flags = sprite.flags;

		uint16_t tileAddr = TILE_DATA_1 + tileIndex * TILE_DATA_SIZE;
		if (tall_sprites)
		{
			tileAddr &= 0xFE;
		}

		bool background_priority = (flags >> 7) & 1;
		bool flip_y = (flags >> 6) & 1;
		bool flip_x = (flags >> 5) & 1;

		int sprite_row = flip_y ? sprite_height - (static_cast<uint16_t>(row) - y) - 1 : static_cast<uint16_t>(row) - y;

		COLOR palette[4];
		for (int i = 0; i < 4; i++)
		{
			palette[i] = ((flags >> 4) & 1) ? obp1_palette[i] : obp0_palette[i];
		}

		uint8_t lsbs = read_mem(tileAddr + sprite_row * 2);
		uint8_t msbs = read_mem(tileAddr + sprite_row * 2 + 1);

		for (int i = 0; i < TILE_WIDTH; i++)
		{
			int index_x = flip_x ? x + TILE_WIDTH - i - 1 : x + i;
			if (index_x >= SCREEN_WIDTH || index_x < 0)
			{
				continue;
			}
			if (spriteData[row][index_x] != WHITE_OR_TRANSPARENT)
			{
				continue;
			}
			// this tile column of pixels is not visible
			if (index_x >= SCREEN_WIDTH || index_x < 0)
			{
				continue;
			}
			// sprite does not have priority
			if (background_priority && pixelData[row][index_x] != WHITE_OR_TRANSPARENT)
			{
				continue;
			}
			uint8_t color_index = ((lsbs >> (7 - index_x)) & 1) | (((msbs >> (7 - index_x)) & 1) << 1);
			COLOR color = palette[color_index];
			spriteData[row][index_x] = color;
		}
	}
}

void PPU::scanOAM(uint8_t row)
{
	bool tall_sprites = LCDC_reg & 0b100;
	int sprite_height = tall_sprites ? TILE_HEIGHT * 2 : TILE_HEIGHT;
	for (int i = 0; i < 40; i++)
	{
		if (spriteBuffer.size() >= 10)
		{
			return; // too many sprites, do nothing
		}

		uint8_t y = read_mem(OAM_START + i * 4);
		if (row + 16 < y || row + 16 >= y + sprite_height)
		{
			return; // sprite is not visible
		}
		uint8_t x = read_mem(OAM_START + i * 4 + 1);
		if (x <= 0)
		{
			return; // sprite is not visible
		}
		uint8_t tileIndex = read_mem(OAM_START + i * 4 + 2);
		uint8_t flags = read_mem(OAM_START + i * 4 + 3);
		spriteBuffer.push_back(Sprite(y, x, tileIndex, flags));
	}
}

uint8_t PPU::read_mem(uint16_t addr)
{
	if (!mmu)
	{
		throw std::runtime_error("PPU Error: MMU not connected!");
	}

	// If accessing VRAM or OAM, bypass MMU checks and go straight to MMAP
	if ((addr >= 0x8000 && addr <= 0x9FFF) || (addr >= 0xFE00 && addr <= 0xFE9F))
	{
		MMAP *mmap_ptr = mmu->get_mmap(); // Get direct MMAP pointer
		if (!mmap_ptr)
		{
			throw std::runtime_error("PPU Error: MMU did not provide MMAP pointer!");
		}
		// PPU has direct, unrestricted access to VRAM/OAM via MMAP
		return mmap_ptr->read_mem(addr);
	}
	else
	{
		// For other addresses (e.g., I/O registers like LCDC), go through MMU
		return ram->read_mem(addr);
	}
}

void PPU::write_mem(uint16_t addr, uint8_t data)
{
	if (!mmu)
	{
		throw std::runtime_error("PPU Error: MMU not connected!");
	}

	// If accessing VRAM or OAM, bypass MMU checks and go straight to MMAP
	if ((addr >= 0x8000 && addr <= 0x9FFF) || (addr >= 0xFE00 && addr <= 0xFE9F))
	{
		MMAP *mmap_ptr = mmu->get_mmap(); // Get direct MMAP pointer
		if (!mmap_ptr)
		{
			throw std::runtime_error("PPU Error: MMU did not provide MMAP pointer!");
		}
		// PPU has direct, unrestricted access to VRAM/OAM via MMAP
		mmap_ptr->write_mem(addr, data);
	}
	else
	{
		// For other addresses (e.g., I/O registers like LCDC), go through MMU
		ram->write_mem(addr, data);
	}
}