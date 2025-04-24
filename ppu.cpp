#include "ppu.hpp"

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

PPU::PPU() : mmu(nullptr) { // Initialize mmu pointer
}

PPU::~PPU() {
}

void PPU::connect_mmu(MMU* mmu_ptr) {
    this->mmu = mmu_ptr;
}

uint64_t** PPU::writePixels() {
	updateRegs();
	updateBackground();
	updateWindow();
	updateSprites();

	// now, mix the background and the window
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		for (int j = 0; j < SCREEN_WIDTH; j++) {
			pixelData[i][j] = backgroundData[i][j];
		}
	}
	// now overlay the window
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		for (int j = 0; j < SCREEN_WIDTH; j++) {
			// account for WX, WY
			int screenX = j + WX_reg - 7;
			int screenY = i + WY_reg;
			if (screenX >= 0 && screenX < SCREEN_WIDTH && screenY >= 0 && screenY < SCREEN_HEIGHT) {
				pixelData[screenY][screenX] = windowData[i][j];
			}
		}
	}
	// finally, overlay the sprites
}

void PPU::updateRegs() {
	// update the registers for graphics
	LCDC_reg = ram->read_mem(0xFF40);
	SCY_reg = ram->read_mem(0xFF42);
	SCX_reg = ram->read_mem(0xFF43);
	WY_reg = ram->read_mem(0xFF4A);
	WX_reg = ram->read_mem(0xFF4B);
}

void PPU::updateBackground() {
	// update background via tile map and scroll regs
	
	// which tile map to use?
	uint8_t map_addr = (LCDC_reg & LCDC_MAP_CHOICE_MASK) ? TILE_MAP_2 : TILE_MAP_1;
	// start at 0x8000 w/ unsigned offsets OR at 0x9000 w/ signed offsets
	bool simple_addressing_mode = (LCDC_reg & LCDC_ADDRESSING_MODE_MASK) == 0;
	COLOR full_map[MAP_HEIGHT * TILE_HEIGHT][MAP_WIDTH * TILE_WIDTH];
	uint16_t tiles_addr = simple_addressing_mode ? TILE_DATA_1 : TILE_DATA_2;
	for (int i = 0; i < MAP_HEIGHT * TILE_HEIGHT; i += TILE_HEIGHT) {
		for (int j = 0; j < MAP_WIDTH * TILE_WIDTH; j += TILE_WIDTH) {
			// get tiles in this row
			uint8_t tile_addr;
			if (simple_addressing_mode) {
				uint8_t tile_offset = ram->read_mem(map_addr + (i / TILE_HEIGHT) * MAP_WIDTH + (j / TILE_WIDTH));
				tile_addr = tiles_addr + tile_offset * TILE_DATA_SIZE;
			}
			else {
				int8_t tile_offset = static_cast<int8_t>(ram->read_mem(map_addr + (i / TILE_HEIGHT) * MAP_WIDTH + (j / TILE_WIDTH)));
				tile_addr = tiles_addr + tile_offset * TILE_DATA_SIZE;
			}
			// transform tile data into pixel data
			for (int k = 0; k < TILE_HEIGHT; k++) {
				uint8_t lsbs = ram->read_mem(tile_addr + k*2);
				uint8_t msbs = ram->read_mem(tile_addr + k*2 + 1);
				for (int l = 0; l < TILE_WIDTH; l++) {
					// get color from each bit pair and store in map
					uint8_t color = ((lsbs >> (7 - l)) & 1) | (((msbs >> (7 - l)) & 1) << 1);
					full_map[i + k][j + l] = static_cast<COLOR>(color);
				}
			}
		}
	}

	// now, apply scroll
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		for (int j = 0; j < SCREEN_WIDTH; j++) {
			// use SCX, SCY regs as offsets w/ wrap-around
			int y = (i + SCX_reg) % (MAP_HEIGHT * TILE_HEIGHT);
			int x = (j + SCY_reg) % (MAP_WIDTH * TILE_WIDTH);
			backgroundData[i][j] = full_map[y][x];
		}
	}
}
  
uint8_t PPU::read_memory(uint16_t addr) {
    if (!mmu) {
        throw std::runtime_error("PPU Error: MMU not connected!");
    }

    // If accessing VRAM or OAM, bypass MMU checks and go straight to MMAP
    if ((addr >= 0x8000 && addr <= 0x9FFF) || (addr >= 0xFE00 && addr <= 0xFE9F)) {
        MMAP* mmap_ptr = mmu->get_mmap(); // Get direct MMAP pointer
        if (!mmap_ptr) {
             throw std::runtime_error("PPU Error: MMU did not provide MMAP pointer!");
        }
        // PPU has direct, unrestricted access to VRAM/OAM via MMAP
        return mmap_ptr->read_mem(addr);
    } else {
        // For other addresses (e.g., I/O registers like LCDC), go through MMU
        return mmu->read_mem(addr);
    }
}

void PPU::write_memory(uint16_t addr, uint8_t data) {
    if (!mmu) {
        throw std::runtime_error("PPU Error: MMU not connected!");
    }

    // If accessing VRAM or OAM, bypass MMU checks and go straight to MMAP
    if ((addr >= 0x8000 && addr <= 0x9FFF) || (addr >= 0xFE00 && addr <= 0xFE9F)) {
        MMAP* mmap_ptr = mmu->get_mmap(); // Get direct MMAP pointer
         if (!mmap_ptr) {
             throw std::runtime_error("PPU Error: MMU did not provide MMAP pointer!");
        }
        // PPU has direct, unrestricted access to VRAM/OAM via MMAP
        mmap_ptr->write_mem(addr, data);
    } else {
        // For other addresses (e.g., I/O registers like LCDC), go through MMU
        mmu->write_mem(addr, data);
    }
}