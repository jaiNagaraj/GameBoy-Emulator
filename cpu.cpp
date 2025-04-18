/**
 * This file deals with the operation of the GameBoy CPU. 
 * All operation emulation functions should be defined here.
 */

#pragma once
#include "cpu.hpp"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

CPU::CPU() {
    // Register initialization
    for (size_t i = 0; i < 8; i++)
        regs[i] = 0;
    sp = 0;
    pc = 0;
}

// Helper function to set/clear a specific flag bit
void CPU::set_flag(int flag_bit, bool value) {
    if (value) {
        regs[FLAGS_REGISTER] |= (1 << flag_bit);
    } else {
        regs[FLAGS_REGISTER] &= ~(1 << flag_bit);
    }
}

bool CPU::get_flag(int flag_bit) {
    return (regs[FLAGS_REGISTER] >> flag_bit) & 1;
}

uint16_t CPU::get_hl() {
    return (static_cast<uint16_t>(regs[H_REGISTER]) << 8) | regs[L_REGISTER];
}

// DECODE
// Jai

// Archit
bool CPU::decode_ADD_46(uint8_t opcode) {
    // ADD A, (HL) - Opcode 0b10000110/0x86
    return opcode == 0x86;
}

bool CPU::decode_ADD_47(uint8_t opcode) {
    // ADD N: Add (immediate) - Opcode 0b11000110/0xC6
    return opcode == 0xC6;
}

// Ella

// Rishi
bool CPU::decode_SWAP_98(uint32_t instruction) {
    bool outcome = (instruction & 0b11111000) == 0b00110000;

    return outcome;
}

bool CPU::decode_SWAP_99(uint32_t instruction) {
    bool outcome = instruction == 0x36;

    return outcome;
}

bool CPU::decode_SRL_100(uint32_t instruction) {
    bool outcome = (instruction & 0b11111000) == 0b00111000;

    return outcome;
}

bool CPU::decode_SRL_101(uint32_t instruction) {
    bool outcome = instruction == 0x3E;

    return outcome;
}

bool CPU::decode_BIT_102(uint32_t instruction) {
    bool outcome = (instruction & 0b11000000) == 0b01000000;

    return outcome;
}

bool CPU::decode_BIT_103(uint32_t instruction) {
    bool outcome = (instruction & 0b11000111) == 0b01000110;

    return outcome;
}

bool CPU::decode_RES_104(uint32_t instruction) {
    bool outcome = (instruction & 0b11000000) == 0b10000000;

    return outcome;
}

bool CPU::decode_RES_105(uint32_t instruction) {
    bool outcome = (instruction & 0b11000111) == 0b10000110;

    return outcome;
}

bool CPU::decode_SET_106(uint32_t instruction) {
    bool outcome = (instruction & 0b11000000) == 0b11000000;

    return outcome;
}

bool CPU::decode_SET_107(uint32_t instruction) {
    bool outcome = (instruction & 0b11000111) == 0b11000110;

    return outcome;
}

bool CPU::decode_JP_109(uint32_t instruction) {
    bool outcome = instruction == 0xC3;

    return outcome;
}

bool CPU::decode_JP_110(uint32_t instruction) {
    bool outcome = instruction == 0xE9;

    return outcome;
}

bool CPU::decode_JP_111(uint32_t instruction) {
    bool outcome = (instruction & 0b11100111) == 11000010;

    return outcome;
}

bool CPU::decode_JR_113(uint32_t instruction) {
    bool outcome = instruction == 0x18;

    return outcome;
}

bool CPU::decode_JR_114(uint32_t instruction) {
    bool outcome = (instruction & 0b11100111) == 0b00100000;

    return outcome;
}

bool CPU::decode_CALL_116(uint32_t instruction) {
    bool outcome = instruction == 0xCD;

    return outcome;
}

bool CPU::decode_CALL_117(uint32_t instruction) {
    bool outcome = (instruction & 0b11100111) == 0b11000100;

    return outcome;
}

bool CPU::decode_RET_119(uint32_t instruction) {
    bool outcome = instruction == 0xC9;

    return outcome;
}

bool CPU::decode_RET_120(uint32_t instruction) {
    bool outcome = (instruction & 0b11100111) == 0b11000000;

    return outcome;
}

bool CPU::decode_RETI_121(uint32_t instruction) {
    bool outcome = instruction == 0xD9;

    return outcome;
}

bool CPU::decode_RST_122(uint32_t instruction) {
    bool outcome = (instruction & 11000111) == 0b11000111;

    return outcome;
}

// DNE in gekkio documentation
/*
bool CPU::decode_HALT_123(uint32_t instruction) {
    bool outcome = [instruction];

    return outcome;
}

bool CPU::decode_STOP_123(uint32_t instruction) {
    bool outcome = [instruction];

    return outcome;
}
*/

bool CPU::decode_DI_123(uint32_t instruction) {
    bool outcome = instruction == 0xF3;

    return outcome;
}

bool CPU::decode_EI_124(uint32_t instruction) {
    bool outcome = instruction == 0xFB;

    return outcome;
}

// EXECUTE
// Jai

// Archit
void CPU::execute_ADD_46(uint8_t opcode) {
    // ADD A, (HL) - Opcode 0b10000110/0x86
    uint16_t addr = get_hl();
    // How to read from memory?
    uint8_t data = 0; // read_memory(addr); 
    
    uint8_t a_val = regs[A_REGISTER];

    // Do the addition
    uint16_t result16 = static_cast<uint16_t>(a_val) + static_cast<uint16_t>(data);
    uint8_t result8 = static_cast<uint8_t>(result16);

    // Calculate and set flags
    set_flag(Z_FLAG_BIT, result8 == 0);
    set_flag(N_FLAG_BIT, false);
    // Half Carry: Check carry from bit 3 to bit 4
    set_flag(H_FLAG_BIT, ((a_val & 0x0F) + (data & 0x0F)) > 0x0F);
    // Carry: Check carry from bit 7
    set_flag(C_FLAG_BIT, result16 > 0xFF);

    // Store result back in A register
    regs[A_REGISTER] = result8;

    pc++;
}

void CPU::execute_ADD_47(uint8_t opcode) {
    // ADD N: Add (immediate) - Opcode 0b11000110/0xC6
    uint16_t addr = get_hl();
    // How to read from memory?
    uint8_t n = 0; // read_memory(addr); 
    
    uint8_t result8;
    uint8_t ca;



    pc++;
}

// Ella

// Rishi
