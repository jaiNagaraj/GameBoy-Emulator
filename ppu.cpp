#include "ppu.hpp"

PPU::PPU() : mmu(nullptr) { // Initialize mmu pointer
}

PPU::~PPU() {
}

void PPU::connect_mmu(MMU* mmu_ptr) {
    this->mmu = mmu_ptr;
}

uint64_t** PPU::writePixels() {
	update_LCDC();
	updateBackground();
	updateWindow();
	updateSprites();
}

void PPU::updateBackground() {
	;
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