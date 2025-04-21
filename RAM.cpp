#include "RAM.hpp"

RAM::RAM() {
    // Memory initialization
    // ROM Bank 0 : 0x0000 - 0x3FFF
    /*Loaded from ROM file*/

    // Switchable ROM : 0x4000 - 0x7FFF
    /*Loaded from ROM file*/

    // VRAM : 0x8000 - 0x9FFF
    for (size_t i = 0xA000; i < 0xC000; i++)
        RAM::mem[i] = 0;

    // External RAM : 0xA000 - 0xBFFF
    for (size_t i = 0xA000; i < 0xC000; i++)
        RAM::mem[i] = 0;

    // WRAM : 0xC000 - 0xDFFF
    for (size_t i = 0xC000; i < 0xE000; i++)
        RAM::mem[i] = 0;

    // Echo RAM (unusable) : 0xE000 - 0xFDFF

    // OAM : 0xFE00 - 0xFE9F
    for (size_t i = 0xFE00; i < 0xFEA0; i++)
        RAM::mem[i] = 0;

    // Unusable : 0xFEA0 - 0xFEFF

    // I/O Registers : 0xFF00 - 0xFF7F
    for (size_t i = 0xFF00; i < 0xFF80; i++)
        RAM::mem[i] = 0;
    // Post-boot initialization
    RAM::mem[0xFF05] = 0x00; // TIMA
    RAM::mem[0xFF06] = 0x00; // TMA
    RAM::mem[0xFF07] = 0x00; // TAC
    RAM::mem[0xFF10] = 0x80; // NR10
    RAM::mem[0xFF11] = 0xBF; // NR11
    RAM::mem[0xFF12] = 0xF3; // NR12
    RAM::mem[0xFF14] = 0xBF; // NR14
    RAM::mem[0xFF16] = 0x3F; // NR21
    RAM::mem[0xFF17] = 0x00; // NR22
    RAM::mem[0xFF19] = 0xBF; // NR24
    RAM::mem[0xFF1A] = 0x7F; // NR30
    RAM::mem[0xFF1B] = 0xFF; // NR31
    RAM::mem[0xFF1C] = 0x9F; // NR32
    RAM::mem[0xFF1E] = 0xBF; // NR33
    RAM::mem[0xFF20] = 0xFF; // NR41
    RAM::mem[0xFF21] = 0x00; // NR42
    RAM::mem[0xFF22] = 0x00; // NR43
    RAM::mem[0xFF23] = 0xBF; // NR30
    RAM::mem[0xFF24] = 0x77; // NR50
    RAM::mem[0xFF25] = 0xF3; // NR51
    RAM::mem[0xFF26] = 0xF1; // NR52
    RAM::mem[0xFF40] = 0x91; // LCDC
    RAM::mem[0xFF42] = 0x00; // SCY
    RAM::mem[0xFF43] = 0x00; // SCX
    RAM::mem[0xFF45] = 0x00; // LYC
    RAM::mem[0xFF47] = 0xFC; // BGP
    RAM::mem[0xFF48] = 0xFF; // OBP0
    RAM::mem[0xFF49] = 0xFF; // OBP1
    RAM::mem[0xFF4A] = 0x00; // WY
    RAM::mem[0xFF4B] = 0x00; // WX

    // HRAM : 0xFF80 - 0xFFFE
    for (size_t i = 0xFF80; i < 0xFFFF; i++)
        RAM::mem[i] = 0;

    // IE Register : 0xFFFF
    RAM::mem[0xFFFF] = 0;
}

uint8_t RAM::read_mem(uint16_t addr) {
    return RAM::mem[addr];
}

void RAM::write_mem(uint16_t addr, uint8_t data) {
    RAM::mem[addr] = data;
}

void RAM::push_stack(uint16_t data) {
    // TODO
}

uint8_t RAM::pop_stack() {
    // TODO
}
