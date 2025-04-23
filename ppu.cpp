#include "ppu.hpp"

const int TILE_MAP_1 = 0x9800;
const int TILE_MAP_2 = 0x9C00;
const int TILE_DATA_1 = 0x8000;
const int TILE_DATA_2 = 0x9000;

PPU::PPU() {
	ram = new RAM();
}

PPU::PPU(RAM* ram) {
	this->ram = ram;
}

PPU::~PPU() {
	delete ram;
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
	uint8_t map_addr = (LCDC_reg & 0x08) ? TILE_MAP_2 : TILE_MAP_1;
	// start at 0x8000 w/ unsigned offsets OR at 0x9000 w/ signed offsets
	bool simple_addressing_mode = (LCDC_reg & 0x10) == 0;
	COLOR full_map[256][256];
	uint16_t tiles_addr = simple_addressing_mode ? TILE_DATA_1 : TILE_DATA_2;
	for (int i = 0; i < 256; i += 8) {
		for (int j = 0; j < 256; j += 8) {
			// get tiles in this row
			uint8_t tile_addr;
			if (simple_addressing_mode) {
				uint8_t tile_offset = ram->read_mem(map_addr + (i / 8) * 32 + (j / 8));
				tile_addr = tiles_addr + tile_offset * 16;
			}
			else {
				int8_t tile_offset = static_cast<int8_t>(ram->read_mem(map_addr + (i / 8) * 32 + (j / 8)));
				tile_addr = tiles_addr + tile_offset * 16;
			}
			// transform tile data into pixel data
			for (int k = 0; k < 8; k++) {
				uint8_t lsbs = ram->read_mem(tile_addr + k*2);
				uint8_t msbs = ram->read_mem(tile_addr + k*2 + 1);
				for (int l = 0; l < 8; l++) {
					// get color from each bit pair and store in map
					uint8_t color = ((lsbs >> (7 - l)) & 1) | (((msbs >> (7 - l)) & 1) << 1);
					full_map[i + k][j + l] = static_cast<COLOR>(color);
				}
			}
		}
	}

	// now, apply scroll
	for (int i = 0; i < 160; i++) {
		for (int j = 0; j < 144; j++) {
			int x = (i + SCX_reg) % 256;
			int y = (j + SCY_reg) % 256;
			backgroundData[i][j] = full_map[x][y];
		}
	}
}