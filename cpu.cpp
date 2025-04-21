/**
 * This file deals with the operation of the GameBoy CPU. 
 * All operation emulation functions should be defined here.
 */

#pragma once
#include "cpu.hpp"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

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

uint16_t CPU::get_bc() {
    return (static_cast<uint16_t>(regs[B_REGISTER]) << 8) | regs[C_REGISTER];
}

uint16_t CPU::get_de() {
    return (static_cast<uint16_t>(regs[D_REGISTER]) << 8) | regs[E_REGISTER];
}

uint16_t CPU::get_af() {
    return (static_cast<uint16_t>(regs[A_REGISTER]) << 8) | regs[FLAGS_REGISTER];
}

void CPU::set_hl(uint16_t val) {
    regs[H_REGISTER] = static_cast<uint8_t>((val >> 8) & 0xFF);
    regs[L_REGISTER] = static_cast<uint8_t>(val & 0xFF);
}

void CPU::set_hl(uint16_t val) {
    regs[B_REGISTER] = static_cast<uint8_t>((val >> 8) & 0xFF);
    regs[C_REGISTER] = static_cast<uint8_t>(val & 0xFF);
}

void CPU::set_de(uint16_t val) {
    regs[D_REGISTER] = static_cast<uint8_t>((val >> 8) & 0xFF);
    regs[E_REGISTER] = static_cast<uint8_t>(val & 0xFF);
}

void CPU::set_af(uint16_t val) {
    regs[A_REGISTER] = static_cast<uint8_t>((val >> 8) & 0xFF);
    regs[FLAGS_REGISTER] = static_cast<uint8_t>(val & 0xFF);
}

// DECODE
// Jai
bool CPU::decode_LD_20(uint32_t instruction) {
    // LD r, r'
    bool outcome = ((instruction >> 22) << 6) == 0x40;

    return outcome;
}

bool CPU::decode_LD_21(uint32_t instruction) {
    // LD r, n (immediate)
    bool outcome = ((instruction >> 16) & 0xc7) == 0x06;

    return outcome;
}

bool CPU::decode_LD_22(uint32_t instruction) {
    // LD r, (HL)
    bool outcome = ((instruction >> 16) & 0xc7) == 0x46;

    return outcome;
}

bool CPU::decode_LD_23(uint32_t instruction) {
    // LD (HL), r
    bool outcome = ((instruction >> 16) & 0xf8) == 0x70;

    return outcome;
}

bool CPU::decode_LD_24(uint32_t instruction) {
    // LD (HL), n (immediate)
    bool outcome = (instruction >> 16) == 0x36;

    return outcome;
}

bool CPU::decode_LD_25(uint32_t instruction) {
    // LD A, (BC)
    bool outcome = (instruction >> 16) == 0x0A;

    return outcome;
}

bool CPU::decode_LD_26(uint32_t instruction) {
    // LD A, (DE)
    bool outcome = (instruction >> 16) == 0x1A;

    return outcome;
}

bool CPU::decode_LD_27(uint32_t instruction) {
    // LD (BC), A
    bool outcome = (instruction >> 16) == 0x02;

    return outcome;
}

bool CPU::decode_LD_28(uint32_t instruction) {
    // LD (DE), A
    bool outcome = (instruction >> 16) == 0x12;

    return outcome;
}

bool CPU::decode_LD_29(uint32_t instruction) {
    // LD A, (nn)
    bool outcome = (instruction >> 16) == 0xFA;

    return outcome;
}

bool CPU::decode_LD_30(uint32_t instruction) {
    // LD (nn), A
    bool outcome = (instruction >> 16) == 0xEA;

    return outcome;
}

bool CPU::decode_LD_31(uint32_t instruction) {
    // LD A, (C)
    bool outcome = (instruction >> 16) == 0xF2;

    return outcome;
}

bool CPU::decode_LD_32(uint32_t instruction) {
    // LD (C), A
    bool outcome = (instruction >> 16) == 0xE2;

    return outcome;
}

bool CPU::decode_LD_33(uint32_t instruction) {
    // LD A, (n)
    bool outcome = (instruction >> 16) == 0xF0;

    return outcome;
}

bool CPU::decode_LD_34(uint32_t instruction) {
    // LD (n), A
    bool outcome = (instruction >> 16) == 0xE0;

    return outcome;
}

bool CPU::decode_LD_35(uint32_t instruction) {
    // LD A, (HL-)
    bool outcome = (instruction >> 16) == 0x3A;

    return outcome;
}

bool CPU::decode_LD_36(uint32_t instruction) {
    // LD (HL-), A
    bool outcome = (instruction >> 16) == 0x32;

    return outcome;
}

bool CPU::decode_LD_37(uint32_t instruction) {
    // LD A, (HL+)
    bool outcome = (instruction >> 16) == 0x2A;

    return outcome;
}

bool CPU::decode_LD_38(uint32_t instruction) {
    // LD (HL+), A
    bool outcome = (instruction >> 16) == 0x22;

    return outcome;
}

bool CPU::decode_LD_39(uint32_t instruction) {
    // LD rr, nn
    bool outcome = ((instruction >> 16) & 0xCF) == 0x01;

    return outcome;
}

bool CPU::decode_LD_40(uint32_t instruction) {
    // LD (nn), SP
    bool outcome = (instruction >> 16) == 0x08;

    return outcome;
}

bool CPU::decode_LD_41(uint32_t instruction) {
    // LD SP, HL
    bool outcome = (instruction >> 16) == 0xF9;

    return outcome;
}

bool CPU::decode_PUSH_42(uint32_t instruction) {
    // PUSH rr
    bool outcome = ((instruction >> 16) & 0xCF) == 0xC5;

    return outcome;
}

bool CPU::decode_POP_43(uint32_t instruction) {
    // POP rr
    bool outcome = ((instruction >> 16) & 0xCF) == 0xC1;

    return outcome;
}

bool CPU::decode_LD_44(uint32_t instruction) {
    // LD HL, SP+e
    bool outcome = (instruction >> 16) == 0xF8;

    return outcome;
}

bool CPU::decode_ADD_45(uint32_t instruction) {
    // ADD A, r
    bool outcome = (instruction >> 16) == 0x80;

    return outcome;
}

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
bool CPU::decode_SWAP_98(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   ((((instruction >> 8) & 0xFF) & 0b11111000) == 0b00110000); // opcode

    if (outcome)
        CPU::execute_SWAP_98(instruction);

    return outcome;
}

bool CPU::decode_SWAP_99(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   (((instruction >> 8) & 0xFF) == 0x36); // opcode

    return outcome;
}

bool CPU::decode_SRL_100(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   ((((instruction >> 8) & 0xFF) & 0b11111000) == 0b00111000); // opcode

    return outcome;
}

bool CPU::decode_SRL_101(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   (((instruction >> 8) & 0xFF) == 0x3E); // opcode

    return outcome;
}

bool CPU::decode_BIT_102(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   ((((instruction >> 8) & 0xFF) & 0b11000000) == 0b01000000); // opcode

    return outcome;
}

bool CPU::decode_BIT_103(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   ((((instruction >> 8) & 0xFF) & 0b11000111) == 0b01000110); // opcode

    return outcome;
}

bool CPU::decode_RES_104(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   ((((instruction >> 8) & 0xFF) & 0b11000000) == 0b10000000); // opcode

    return outcome;
}

bool CPU::decode_RES_105(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   ((((instruction >> 8) & 0xFF) & 0b11000111) == 0b10000110); // opcode

    return outcome;
}

bool CPU::decode_SET_106(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   ((((instruction >> 8) & 0xFF) & 0b11000000) == 0b11000000); // opcode

    return outcome;
}

bool CPU::decode_SET_107(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   ((((instruction >> 8) & 0xFF) & 0b11000111) == 0b11000110); // opcode

    return outcome;
}

bool CPU::decode_JP_109(uint32_t instruction) { // 3-byte instruction
    bool outcome = ((instruction >> 16) & 0xFF) == 0xC3; // opcode

    return outcome;
}

bool CPU::decode_JP_110(uint32_t instruction) { // 1-byte instruction
    bool outcome = ((instruction >> 16) & 0xFF) == 0xE9; // opcode

    return outcome;
}

bool CPU::decode_JP_111(uint32_t instruction) { // 3-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) & 0b11100111) == 11000010; // opcode

    return outcome;
}

bool CPU::decode_JR_113(uint32_t instruction) { // 2-byte instruction
    bool outcome = ((instruction >> 16) & 0xFF) == 0x18; // opcode

    return outcome;
}

bool CPU::decode_JR_114(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) & 0b11100111) == 0b00100000; // opcode

    return outcome;
}

bool CPU::decode_CALL_116(uint32_t instruction) { // 3-byte instruction
    bool outcome = ((instruction >> 16) & 0xFF) == 0xCD; // opcode

    return outcome;
}

bool CPU::decode_CALL_117(uint32_t instruction) { // 3-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) & 0b11100111) == 0b11000100; // opcode

    return outcome;
}

bool CPU::decode_RET_119(uint32_t instruction) { // 1-byte instruction
    bool outcome = ((instruction >> 16) & 0xFF) == 0xC9; // opcode

    return outcome;
}

bool CPU::decode_RET_120(uint32_t instruction) { // 1-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) & 0b11100111) == 0b11000000; // opcode

    return outcome;
}

bool CPU::decode_RETI_121(uint32_t instruction) { // 1-byte instruction
    bool outcome = ((instruction >> 16) & 0xFF) == 0xD9; // opcode

    return outcome;
}

bool CPU::decode_RST_122(uint32_t instruction) { // 1-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) & 11000111) == 0b11000111; // opcode

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

bool CPU::decode_DI_123(uint32_t instruction) { // 1-byte instruction
    bool outcome = ((instruction >> 16) & 0xFF) == 0xF3; // opcode

    return outcome;
}

bool CPU::decode_EI_124(uint32_t instruction) { // 1-byte instruction
    bool outcome = ((instruction >> 16) & 0xFF) == 0xFB; // opcode

    return outcome;
}

// EXECUTE
// Jai
void CPU::execute_LD_20(uint32_t instruction) {
    // LD r, r'
    uint8_t operation = static_cast<uint8_t>((instruction >> 16) & 0xFF);
    uint8_t reg1 = (operation & 0b00111000) >> 3;
    uint8_t reg2 = operation & 0b00000111;
    regs[reg1] = regs[reg2];
}

void CPU::execute_LD_21(uint32_t instruction) {
    // LD r, n (immediate)
    uint8_t operation = static_cast<uint8_t>((instruction >> 16) & 0xFF);
    uint8_t reg = (operation & 0b00111000) >> 3;
    uint8_t n = static_cast<uint8_t>((instruction >> 8) & 0xFF); // Get the immediate value

    regs[reg] = n;
}

void CPU::execute_LD_22(uint32_t instruction) {
    // LD r, (HL)
    uint8_t operation = static_cast<uint8_t>((instruction >> 16) & 0xFF);
    uint8_t reg = (operation & 0b00111000) >> 3;
    uint16_t addr = get_hl(); // Get the HL register value
    uint8_t data = read_memory(addr); 

    regs[reg] = data;
}

void CPU::execute_LD_23(uint32_t instruction) {
    // LD (HL), r
    uint8_t operation = static_cast<uint8_t>((instruction >> 16) & 0xFF);
    uint8_t reg = (operation & 0b00111000) >> 3;
    uint16_t addr = get_hl(); // Get the HL register value
    uint8_t data = regs[reg]; 

   write_memory(addr, data);
}

void CPU::execute_LD_24(uint32_t instruction) {
    // LD (HL), n (immediate)
    uint8_t operation = static_cast<uint8_t>((instruction >> 16) & 0xFF);
    uint16_t addr = get_hl(); // Get the HL register value
    uint8_t n = static_cast<uint8_t>((instruction >> 8) & 0xFF); // Get the immediate value

    write_memory(addr, n);
}

void CPU::execute_LD_25(uint32_t instruction) {
    // LD A, (BC)
    uint16_t addr = get_bc(); // Get the BC register value
    uint8_t data = read_memory(addr); 

    regs[A_REGISTER] = data;
}

void CPU::execute_LD_26(uint32_t instruction) {
    // LD A, (DE)
    uint16_t addr = get_de(); // Get the DE register value
    uint8_t data = read_memory(addr); 

    regs[A_REGISTER] = data;
}

void CPU::execute_LD_27(uint32_t instruction) {
    // LD (BC), A
    uint16_t addr = get_bc(); // Get the BC register value
    uint8_t data = regs[A_REGISTER]; 

    write_memory(addr, data);
}

void CPU::execute_LD_28(uint32_t instruction) {
    // LD (DE), A
    uint16_t addr = get_de(); // Get the DE register value
    uint8_t data = regs[A_REGISTER]; 

    write_memory(addr, data);
}

void CPU::execute_LD_29(uint32_t instruction) {
    // LD A, (nn)
    uint8_t lsb = static_cast<uint8_t>((instruction >> 8) & 0xFF); // Get the LSB of the immediate value
    uint8_t msb = static_cast<uint8_t>(instruction & 0xFF); // Get the MSB of the immediate value
    uint16_t addr = msb;
    addr <<= 8;
    addr |= lsb; // Combine LSB and MSB to form the address
    uint8_t data = read_memory(addr); 

    regs[A_REGISTER] = data;
}

void CPU::execute_LD_30(uint32_t instruction) {
    // LD (nn), A
    uint8_t lsb = static_cast<uint8_t>((instruction >> 8) & 0xFF); // Get the LSB of the immediate value
    uint8_t msb = static_cast<uint8_t>(instruction & 0xFF); // Get the MSB of the immediate value
    uint16_t addr = msb;
    addr <<= 8;
    addr |= lsb; // Combine LSB and MSB to form the address
    uint8_t data = regs[A_REGISTER]; 

    write_memory(addr, data);
}

void CPU::execute_LD_31(uint32_t instruction) {
    // LD A, (C)
    uint8_t c_val = regs[C_REGISTER]; // Get the C register value
    uint16_t addr = 0xFF00 | c_val; // Address is 0xFF00 + C

    uint8_t data = read_memory(addr); 

    regs[A_REGISTER] = data;
}

void CPU::execute_LD_32(uint32_t instruction) {
    // LD (C), A
    uint8_t c_val = regs[C_REGISTER]; // Get the C register value
    uint16_t addr = 0xFF00 | c_val; // Address is 0xFF00 + C

    uint8_t data = regs[A_REGISTER]; 

    write_memory(addr, data);
}

void CPU::execute_LD_33(uint32_t instruction) {
    // LD A, (n)
    uint8_t n = static_cast<uint8_t>((instruction >> 8) & 0xFF); // Get the immediate value
    uint16_t addr = 0xFF00 | n; // Address is 0xFF00 + n

    uint8_t data = read_memory(addr); 

    regs[A_REGISTER] = data;
}

void CPU::execute_LD_34(uint32_t instruction) {
    // LD (n), A
    uint8_t n = static_cast<uint8_t>((instruction >> 8) & 0xFF); // Get the immediate value
    uint16_t addr = 0xFF00 | n; // Address is 0xFF00 + n

    uint8_t data = regs[A_REGISTER]; 

    write_memory(addr, data);
}

void CPU::execute_LD_35(uint32_t instruction) {
    // LD A, (HL-)
    uint16_t addr = get_hl(); // Get the HL register value
    uint8_t data = read_memory(addr); 

    regs[A_REGISTER] = data;

    // Decrement HL after reading
    set_hl(addr - 1);
}

void CPU::execute_LD_36(uint32_t instruction) {
    // LD (HL-), A
    uint16_t addr = get_hl(); // Get the HL register value
    uint8_t data = regs[A_REGISTER]; 

    write_memory(addr, data);

    // Decrement HL after writing
    set_hl(addr - 1);
}

void CPU::execute_LD_37(uint32_t instruction) {
    // LD A, (HL+)
    uint16_t addr = get_hl(); // Get the HL register value
    uint8_t data = read_memory(addr); 

    regs[A_REGISTER] = data;

    // Increment HL after reading
    set_hl(addr + 1);
}

void CPU::execute_LD_38(uint32_t instruction) {
    // LD (HL+), A
    uint16_t addr = get_hl(); // Get the HL register value
    uint8_t data = regs[A_REGISTER]; 

    write_memory(addr, data);

    // Increment HL after writing
    set_hl(addr + 1);
}

void CPU::execute_LD_39(uint32_t instruction) {
    // LD rr, nn
    uint8_t operation = static_cast<uint8_t>((instruction >> 16) & 0xFF);
    uint16_t nn = static_cast<uint16_t>(((instruction >> 8) & 0xFF) | ((instruction & 0xFF) << 8)); // Get the immediate value

    uint8_t reg = (operation & 0b00110000) >> 4;

    if (reg == 0) {
        set_bc(nn); // LD BC, nn
    } else if (reg == 1) {
        set_de(nn); // LD DE, nn
    } else if (reg == 2) {
        set_hl(nn); // LD HL, nn
    } else if (reg == 3) {
        sp = nn; // LD SP, nn
    }
}

void CPU::execute_LD_40(uint32_t instruction) {
    // LD (nn), SP
    uint8_t lsb = static_cast<uint8_t>((instruction >> 8) & 0xFF);
    uint8_t msb = static_cast<uint8_t>(instruction & 0xFF);
    uint16_t addr = msb;
    addr <<= 8;
    addr |= lsb; // Combine LSB and MSB to form the address

    write_memory(addr, static_cast<uint8_t>(sp & 0xFF)); // Store LSB of SP
    write_memory(addr + 1, static_cast<uint8_t>((sp >> 8) & 0xFF)); // Store MSB of SP
}

void CPU::execute_LD_41(uint32_t instruction) {
    // LD SP, HL
    sp = get_hl();
}

void CPU::execute_PUSH_42(uint32_t instruction) {
    // PUSH rr
    uint8_t operation = static_cast<uint8_t>((instruction >> 16) & 0xFF);
    uint16_t rr;
    if (((operation & 0b00110000) >> 4) == 0) {
        rr = get_bc();
    } else if (((operation & 0b00110000) >> 4) == 1) {
        rr = get_de();
    } else if (((operation & 0b00110000) >> 4) == 2) {
        rr = get_hl();
    } else {
        rr = get_af();
    }

    // Push the value of rr onto the stack
    sp--;
    write_memory(sp, static_cast<uint8_t>(rr >> 8)); // Store MSB of rr
    sp--;
    write_memory(sp, static_cast<uint8_t>(rr & 0xFF)); // Store LSB of rr
}

void CPU::execute_POP_43(uint32_t instruction) {
    // POP rr
    uint8_t operation = static_cast<uint8_t>((instruction >> 16) & 0xFF);
    uint16_t data;

    // Pop the value from the stack into rr
    uint8_t lsb = read_memory(sp++); // Get lsb from stack, increment SP
    uint8_t msb = read_memory(sp++); // Get msb from stack, increment SP

    data = (static_cast<uint16_t>(msb) << 8) | lsb; // Combine MSB and LSB

    if (((operation & 0b00110000) >> 4) == 0) {
        set_bc(data);
    } else if (((operation & 0b00110000) >> 4) == 1) {
        set_de(data);
    } else if (((operation & 0b00110000) >> 4) == 2) {
        set_hl(data);
    } else {
        set_af(data);
    }
}

void CPU::execute_LD_44(uint32_t instruction) {
    // LD HL, SP+e
    uint8_t e = static_cast<uint8_t>((instruction >> 8) & 0xFF); // Get the immediate value
    uint16_t sp_plus_e = sp + static_cast<int8_t>(e); // Sign-extend the offset

    set_hl(sp_plus_e);
    // set flags!
    set_flag(Z_FLAG_BIT, false);
    set_flag(N_FLAG_BIT, false);
    set_flag(H_FLAG_BIT, ((sp & 0x0F) + (e & 0x0F)) > 0x0F); // Half Carry: Check carry from bit 3 to bit 4
    set_flag(C_FLAG_BIT, ((sp & 0xFF) + e) > 0xFF); // Carry: Check carry from bit 7
}

void CPU::execute_ADD_45(uint32_t instruction) {
    // ADD A, r
    uint8_t operation = static_cast<uint8_t>((instruction >> 16) & 0xFF);
    uint8_t reg = operation & 0b00000111;
    uint8_t data = regs[reg]; // Get the value of the register

    uint8_t a_val = regs[A_REGISTER];

    // Two additions for flag setting
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
}

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
void CPU::execute_SWAP_98(uint32_t instruction) {
    uint8_t opcode = (instruction >> 8) & 0xFF;
    uint8_t reg = opcode & 0b00000111;

    uint8_t cur_data;
    // Retrieve value
    if (reg != 6) {
        cur_data = regs[reg];
    } else {
        cur_data = get_hl(); 
    }
    // Swap nibbles
    uint8_t new_data = ((cur_data & 0x0F) << 4) | ((cur_data & 0xF0) >> 4);
    // Store value
    if (reg != 6) {
        regs[reg] = new_data;
    } else {
        //mem[0xHL] = new_data;
    }

    // Set flags
    set_flag(Z_FLAG_BIT, new_data == 0);
    set_flag(N_FLAG_BIT, false);
    set_flag(H_FLAG_BIT, false);
    set_flag(C_FLAG_BIT, false);

    pc += 2; // 2-byte instruction
}
