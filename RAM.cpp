#include "RAM.hpp"

RAM::RAM() : mmap(nullptr) {}

void RAM::connect_mmap(MMAP *mmap) {
    this->mmap = mmap;
}

uint8_t RAM::read_mem(uint16_t addr) {
    if (!mmap) throw std::runtime_error("RAM Error: MMAP not connected!");
    return mmap->read_mem(addr);
}

void RAM::write_mem(uint16_t addr, uint8_t data) {
    if (!mmap) std::runtime_error("RAM Error: MMAP not connected!");
    mmap->write_mem(addr, data);
}

uint16_t RAM::push_stack(uint16_t sp, uint16_t data) {
    if (sp == 0xBFFF) {
        // Halt program -> stack overflow
    }
    
    write_mem(sp, static_cast<uint8_t>(data >> 8)); // Store MSB of rr
    sp--;
    write_mem(sp, static_cast<uint8_t>(data & 0xFF)); // Store LSB of rr
    sp--;

    if (sp == 0xFF7F) {
        sp = 0xDFFF; // Set sp to end of WRAM
    }

    return sp;
}

uint16_t RAM::pop_stack(uint16_t sp) {
    if (sp == 0xFFFE)
        // Halt program -> stack underflow
    
    if (sp == 0xDFFF)
        sp = 0xFF7F;

    sp++;
    uint8_t lsb = read_mem(sp); // Get lsb from stack, increment SP
    sp++;
    uint8_t msb = read_mem(sp); // Get msb from stack, increment SP

    uint16_t data = (static_cast<uint16_t>(msb) << 8) | lsb; // Combine MSB and LSB

    return data;
}
