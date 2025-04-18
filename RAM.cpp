#include "RAM.hpp"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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
    /*Specific mapping*/

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
