#include <iostream>
#include <list>
#include <algorithm>
#include <bitset>
#include "../include/ppu.hpp"

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
		if (outsideClock - clock >= 80)
		{
			scanOAM(scanLine);
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

    // which tile map to use? (0x9800 or 0x9C00) - LCDC Bit 3
    uint16_t map_base_addr = (LCDC_reg & LCDC_MAP_CHOICE_MASK) ? TILE_MAP_2 : TILE_MAP_1;

    // get Tile Data Addressing Mode based on LCDC Bit 4
    // Bit 4 = 1 -> Use $8000 base with unsigned tile index
    // Bit 4 = 0 -> Use $9000 base with signed tile index
    bool use_unsigned_8000_mode = (LCDC_reg & LCDC_ADDRESSING_MODE_MASK); // True if Bit 4 is 1

    uint8_t map_pixel_y = SCY_reg + row;
    uint8_t tile_row_pixel = map_pixel_y % TILE_HEIGHT; // Row within the tile (0-7)

    for (int screen_x = 0; screen_x < SCREEN_WIDTH; ++screen_x)
    {
        uint8_t map_pixel_x = SCX_reg + screen_x;

        // get tile map coordinates
        uint16_t map_tile_y = map_pixel_y / TILE_HEIGHT;
        uint16_t map_tile_x = map_pixel_x / TILE_WIDTH;

        uint16_t map_offset = (map_tile_y * MAP_WIDTH) + map_tile_x;
        uint16_t tile_index_addr = map_base_addr + map_offset;
        uint8_t tile_index = read_mem(tile_index_addr);

        // Calculate the address of the tile data based on the mode
        uint16_t tile_data_addr;
        if (use_unsigned_8000_mode)
        {
            tile_data_addr = TILE_DATA_1 + tile_index * TILE_DATA_SIZE;
        }
        else
        {
            int8_t signed_index = static_cast<int8_t>(tile_index);
            tile_data_addr = TILE_DATA_2 + static_cast<int16_t>(signed_index) * TILE_DATA_SIZE;
        }

        uint16_t tile_row_data_addr = tile_data_addr + tile_row_pixel * 2;
        uint8_t lsbs = read_mem(tile_row_data_addr);
        uint8_t msbs = read_mem(tile_row_data_addr + 1);

        // Calculate the specific pixel's column within the tile (0-7)
        uint8_t tile_col_pixel = map_pixel_x % TILE_WIDTH;

        uint8_t shift = 7 - tile_col_pixel;
        uint8_t color_index = ((lsbs >> shift) & 1) | (((msbs >> shift) & 1) << 1);

        backgroundData[row][screen_x] = bg_palette[color_index];
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
    // Clear the sprite pixel data for the current row
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        spriteData[row][i] = WHITE_OR_TRANSPARENT;
    }

    // Check if sprites are enabled globally
    if (!(LCDC_reg & 0b10)) {
        return; // Sprites disabled
    }

    // Sort sprites: lower X first, then lower OAM index first
    std::sort(spriteBuffer.begin(), spriteBuffer.end()); // <<< Sort the vector

    // Palettes (Color 0 is always transparent for sprites)
    COLOR obp0_palette[4];
    obp0_palette[0] = WHITE_OR_TRANSPARENT; // <<< Explicitly transparent
    obp0_palette[1] = static_cast<COLOR>((OBP0_reg >> 2) & 0b11);
    obp0_palette[2] = static_cast<COLOR>((OBP0_reg >> 4) & 0b11);
    obp0_palette[3] = static_cast<COLOR>((OBP0_reg >> 6) & 0b11);
    COLOR obp1_palette[4];
    obp1_palette[0] = WHITE_OR_TRANSPARENT; // <<< Explicitly transparent
    obp1_palette[1] = static_cast<COLOR>((OBP1_reg >> 2) & 0b11);
    obp1_palette[2] = static_cast<COLOR>((OBP1_reg >> 4) & 0b11);
    obp1_palette[3] = static_cast<COLOR>((OBP1_reg >> 6) & 0b11);

    bool tall_sprites = LCDC_reg & 0b100;
    int sprite_height = tall_sprites ? TILE_HEIGHT * 2 : TILE_HEIGHT;

    // Iterate through sorted sprites (lower X first = higher priority)
    for (const auto& sprite : spriteBuffer) // <<< Iterate through sorted vector
    {
        int16_t screen_y = sprite.y - SPRITE_Y_OFFSET; // Top Y coordinate on screen
        int16_t screen_x = sprite.x - SPRITE_X_OFFSET; // Left X coordinate on screen
        uint8_t tileIndex = sprite.tileIndex;
        uint8_t flags = sprite.flags;

        // Adjust tile index for 8x16 sprites
        uint16_t tileAddrBase = TILE_DATA_1; // Base address for tile data
        if (tall_sprites) {
            tileIndex &= 0xFE; // Ignore lowest bit
        }
        uint16_t tileAddr = tileAddrBase + tileIndex * TILE_DATA_SIZE;


        bool background_priority = (flags >> 7) & 1; // Bit 7: BG/Win over OBJ
        bool flip_y = (flags >> 6) & 1;
        bool flip_x = (flags >> 5) & 1;
        bool use_obp1 = (flags >> 4) & 1;

        // Row within the sprite (0-7 or 0-15)
        int sprite_row = row - screen_y;
        if (flip_y) {
            sprite_row = (sprite_height - 1) - sprite_row;
        }

        // Calculate address of the specific row data within the tile(s)
        // For 8x16 sprites, the correct tile (top or bottom) is implicitly handled
        // by adding sprite_row * 2 to the base address of the first tile (tileIndex & 0xFE)
        uint16_t tile_row_data_addr = tileAddr + (sprite_row * 2);

        uint8_t lsbs = read_mem(tile_row_data_addr);
        uint8_t msbs = read_mem(tile_row_data_addr + 1);

        // Select the correct palette
        COLOR* palette = use_obp1 ? obp1_palette : obp0_palette;

        // Draw the 8 pixels of the sprite row
        for (int tile_col = 0; tile_col < TILE_WIDTH; ++tile_col) // tile_col is 0-7
        {
            // Calculate the actual screen X coordinate for this pixel
            int current_screen_x = screen_x + tile_col;

            // Skip if pixel is off-screen horizontally
            if (current_screen_x < 0 || current_screen_x >= SCREEN_WIDTH) {
                continue;
            }

            // Determine the bit position within the tile data bytes based on X flip
            int bit_pos = flip_x ? tile_col : (7 - tile_col); // <<< Correct bit position (0-7)

            // Get the 2-bit color index for the pixel
            uint8_t color_index = ((lsbs >> bit_pos) & 1) | (((msbs >> bit_pos) & 1) << 1); // <<< Use bit_pos

            // --- Priority Checks ---

            // 1. Check if sprite pixel is transparent (color index 0)
            if (color_index == 0) { // <<< Explicit transparency check
                continue;
            }

            // 2. Check if a higher-priority sprite pixel has already been drawn here
            // Since sprites are sorted, we only draw if the current spot is empty (transparent).
            if (spriteData[row][current_screen_x] != WHITE_OR_TRANSPARENT) {
                continue; // Higher priority sprite (lower X or OAM index) already drew here
            }

            // 3. Check Background/Window priority (if sprite's bit 7 is set)
            // Sprite pixel is hidden if bg_priority is set AND Background/Window pixel is not color 0 (WHITE/Transparent)
            // Check the already rendered BG/Window data for this pixel on the *current* row.
            bool bg_win_pixel_is_visible = (backgroundData[row][current_screen_x] != WHITE_OR_TRANSPARENT) || // Check BG data
                                           (windowData[row][current_screen_x] != WINDOW_TRANSPARENT); // Check Window data

            if (background_priority && bg_win_pixel_is_visible) { // <<< Check against BG/Window data
                 continue; // Background/Window has priority over this sprite pixel
            }

            // --- Draw Pixel ---
            // All checks passed, draw the sprite pixel using its palette color
            spriteData[row][current_screen_x] = palette[color_index];
        }
    }
}

void PPU::scanOAM(uint8_t row)
{
    spriteBuffer.clear(); // <<< Clear the vector

    bool tall_sprites = LCDC_reg & 0b100;
    int sprite_height = tall_sprites ? TILE_HEIGHT * 2 : TILE_HEIGHT;

    for (int i = 0; i < 40; i++) // OAM index 'i'
    {
        if (spriteBuffer.size() >= 10)
        {
            break; // Max 10 sprites per scanline
        }

        uint16_t oam_addr = OAM_START + i * 4;
        uint8_t y = read_mem(oam_addr);     // Sprite Y position + 16
        uint8_t x = read_mem(oam_addr + 1); // Sprite X position + 8

        // Check vertical intersection first (more efficient)
        int16_t screen_y = y - SPRITE_Y_OFFSET; // Top Y coordinate on screen
        if (row >= screen_y && row < (screen_y + sprite_height))
        {
            // Check horizontal visibility (X=0 hides sprite)
            if (x > 0) {
                uint8_t tileIndex = read_mem(oam_addr + 2);
                uint8_t flags = read_mem(oam_addr + 3);
                // <<< Use emplace_back with OAM index 'i'
                spriteBuffer.emplace_back(y, x, tileIndex, flags, i);
            }
        }
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