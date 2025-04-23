#include "mmu.hpp"

MMU::MMU() {

}

void MMU::connect_cpu(CPU *cpu) {
    this->cpu = cpu;
}
void MMU::connect_ppu(PPU *ppu) {
    this->ppu = ppu;
}
void MMU::connect_ram(RAM *ram) {
    this->ram = ram;
}
void MMU::connect_mmap(MMAP *mmap) {
    this->mmap = mmap;
}

uint8_t MMU::read_mem(uint16_t addr) {
    // Validation
    if (addr >= 0xC000 && addr <= 0xDFFF) {
        // WRAM
        return (*ram).read_mem(addr);
    } else if (addr >= 0xE000 && addr <= 0xFDFF) {
        // Echo RAM (unusable)
        return 0xFF; // Return a default value for unusable memory
    } else if (addr >= 0xFE00 && addr <= 0xFE9F) {
        // OAM
        return (*ram).read_mem(addr);
    } else if (addr >= 0xFF00 && addr <= 0xFF7F) {
        // I/O Registers
        return (*ram).read_mem(addr);
    } else if (addr >= 0xFF80 && addr <= 0xFFFF) {
        // HRAM
        return (*ram).read_mem(addr);
    }

    return (*ram).read_mem(addr);
}

void MMU::write_mem(uint16_t addr, uint8_t data) {
    // Validation

   (*ram).write_mem(addr, data);
}

uint16_t MMU::push_stack(uint16_t sp, uint16_t data) {
    (*ram).push_stack(sp, data);
}

uint16_t MMU::pop_stack(uint16_t sp) {
    return (*ram).pop_stack(sp);
}