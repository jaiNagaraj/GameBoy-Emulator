#include "ppu.hpp"

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
	update_LCDC();
	updateBackground();
	updateWindow();
	updateSprites();
}

void updateBackground() {
	;
}