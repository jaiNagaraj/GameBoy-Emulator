/**
 * This file deals with the operation of the GameBoy CPU. 
 * All operation emulation functions should be defined here.
 */

#include "cpu.hpp"
#include <stdlib.h>
#include <stdio.h>

CPU::CPU() {
    // Register initialization
    for (size_t i = 0; i < 8; i++)
        regs[i] = 0;
    sp = 0;
    pc = 0;
}

 // DECODE
 // Jai

 // Archit

 // Ella

 // Rishi
 bool CPU::decode_SWAP_98(uint8_t opcode) {
    bool outcome = (opcode && 0b11111000) == 0b00110000;

    return outcome;
}

bool CPU::decode_SWAP_99(uint8_t opcode) {
    bool outcome = opcode == 0x36;

    return outcome;
}

bool CPU::decode_SRL_100(uint8_t opcode) {
    bool outcome = (opcode && 0b11111000) == 0b00111000;

    return outcome;
}

bool CPU::decode_SRL_101(uint8_t opcode) {
    bool outcome = opcode == 0x3E;

    return outcome;
}

bool CPU::decode_BIT_102(uint8_t opcode) {
    bool outcome = (opcode && 0b11000000) == 0b01000000;

    return outcome;
}

bool CPU::decode_BIT_103(uint8_t opcode) {
    bool outcome = (opcode && 0b11000111) == 0b01000110;

    return outcome;
}

 // EXECUTE
 // Jai

 // Archit

 // Ella

 // Rishi
