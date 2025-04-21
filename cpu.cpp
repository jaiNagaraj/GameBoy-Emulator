#include "cpu.hpp"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

CPU::CPU() {
    ram = new RAM();

    pc = 0x0100;
    sp = 0xFFFE;

    ime = true;

    // Register initialization
    regs[A_REGISTER] = 0x01;
    regs[FLAGS_REGISTER] = 0xB0; // Flag initialization
    regs[B_REGISTER] = 0x00;
    regs[C_REGISTER] = 0x13;
    regs[D_REGISTER] = 0x00;
    regs[E_REGISTER] = 0xD8;
    regs[H_REGISTER] = 0x01;
    regs[L_REGISTER] = 0x4D;
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

void CPU::set_bc(uint16_t val) {
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
 bool CPU::decode_ADD_46(uint32_t instruction) {
     // ADD A, (HL) - Opcode 0b10000110/0x86
     bool outcome = ((instruction >> 16) & 0xFF) == 0b10000110;
     return outcome;
 }
 
 bool CPU::decode_ADD_47(uint32_t instruction) {
     // ADD N: Add (immediate) - Opcode 0b11000110/0xC6
     bool outcome = ((instruction >> 16) & 0xFF) == 0xC6;
     return outcome;
 }
 
 bool CPU::decode_ADC_48(uint32_t instruction) {
     // ADC A, r (register) - Opcodes 0x88-0x8D, 0x8F
     // Pattern 0b10001xxx, excluding 0x8E
     uint8_t opcode = (instruction >> 16) & 0xFF;
     bool outcome = (opcode >= 0x88 && opcode <= 0x8F && opcode != 0x8E);
     return outcome;
 }
 
 bool CPU::decode_ADC_49(uint32_t instruction) {
     // ADC A, (HL) - Opcode 0x8E (0b10001110)
     bool outcome = (instruction >> 16) & 0xFF == 0x8E;
     return outcome;
 }
 
 bool CPU::decode_ADC_50(uint32_t instruction) {
     // ADC A, n (immediate) - Opcode 0xCE (0b11001110)
     bool outcome = (instruction >> 16) & 0xFF == 0xCE;
     return outcome;
 }
 
 bool CPU::decode_SUB_51(uint32_t instruction) {
     // SUB A, r (register) - Opcodes 0x90-0x95, 0x97
     // Pattern 0b10010xxx, excluding 0x96
     uint8_t opcode = (instruction >> 16) & 0xFF;
     bool outcome = (opcode >= 0x90 && opcode <= 0x97 && opcode != 0x96);
     return outcome;
 }
 
 bool CPU::decode_SUB_52(uint32_t instruction) {
     // SUB A, (HL) - Opcode 0x96 (0b10010110)
     bool outcome = (instruction >> 16) & 0xFF == 0x96;
     return outcome;
 }
 
 bool CPU::decode_SUB_53(uint32_t instruction) {
     // SUB A, n (immediate) - Opcode 0xD6 (0b11010110)
     bool outcome = (instruction >> 16) & 0xFF == 0xD6;
     return outcome;
 }
 
 bool CPU::decode_SBC_54(uint32_t instruction) {
     // SBC A, r (register) - Opcodes 0x98-0x9D, 0x9F
     // Pattern 0b10011xxx, excluding 0x9E
     uint8_t opcode = (instruction >> 16) & 0xFF;
     bool outcome = (opcode >= 0x98 && opcode <= 0x9F && opcode != 0x9E);
     return outcome;
 }
 
 bool CPU::decode_SBC_55(uint32_t instruction) {
     // SBC A, (HL) - Opcode 0x9E (0b10011110)
     bool outcome = (instruction >> 16) & 0xFF == 0x9E; // opcode
     return outcome;
 }
 
 bool CPU::decode_SBC_56(uint32_t instruction) {
     // SBC A, n (immediate) - Opcode 0xDE (0b11011110)
     bool outcome = (instruction >> 16) & 0xFF == 0xDE;
     return outcome;
 }
 
 bool CPU::decode_CP_57(uint32_t instruction) {
     // CP A, r (register) - Opcodes 0xB8-0xBD, 0xBF
     // Pattern 0b10111xxx, excluding 0xBE
     uint8_t opcode = (instruction >> 16) & 0xFF;
     bool outcome (opcode >= 0xB8 && opcode <= 0xBF && opcode != 0xBE);
     return outcome;
 }
 
 bool CPU::decode_CP_58(uint32_t instruction) {
     // CP A, (HL) - Opcode 0xBE (0b10111110)
     bool outcome = (instruction >> 16) & 0xFF == 0xBE;
     return outcome;
 }
 
 bool CPU::decode_CP_59(uint32_t instruction) {
     // CP A, n (immediate) - Opcode 0xFE (0b11111110)
     bool outcome = (instruction >> 16) & 0xFF == 0xFE;
     return outcome;
 }
 
 bool CPU::decode_INC_60(uint32_t instruction) {
     // INC r (register) - Opcodes 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x3C
     // Pattern 0b00xxx100 (excludes 0x34 INC (HL))
     uint8_t opcode = (instruction >> 16) & 0xFF;
     bool outcome = ((opcode & 0x07) == 0x04) && (opcode != 0x34);
     return outcome;
 }
 
 bool CPU::decode_INC_61(uint32_t instruction) {
     // INC (HL) - Opcode 0x34 (0b00110100)
     bool outcome = (instruction >> 16) & 0xFF == 0x34;
     return outcome;
 }
 
 bool CPU::decode_DEC_62(uint32_t instruction) {
     // DEC r (register) - Opcodes 0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x3D
     // Pattern 0b00xxx101 (excludes 0x35 DEC (HL))
     uint8_t opcode = (instruction >> 16) & 0xFF;
     bool outcome = ((opcode & 0x07) == 0x05) && (opcode != 0x35);
     return outcome;
 }
 
 bool CPU::decode_DEC_63(uint32_t instruction) {
     // DEC (HL) - Opcode 0x35 (0b00110101)
     bool outcome = (instruction >> 16) & 0xFF == 0x35;
     return outcome;
 }
 
 bool CPU::decode_AND_64(uint32_t instruction) {
     // AND A, r (register) - Opcodes 0xA0-0xA5, 0xA7
     // Pattern 0b10100xxx, excluding 0xA6
     uint8_t opcode = (instruction >> 16) & 0xFF;
     bool outcome = (opcode >= 0xA0 && opcode <= 0xA7 && opcode != 0xA6);
     return outcome;
 }
 
 bool CPU::decode_AND_65(uint32_t instruction) {
     // AND A, (HL) - Opcode 0xA6 (0b10100110)
     bool outcome = (instruction >> 16) & 0xFF == 0xA6;
     return outcome;
 }
 
 bool CPU::decode_AND_66(uint32_t instruction) {
     // AND A, n (immediate) - Opcode 0xE6 (0b11100110)
     bool outcome = (instruction >> 16) & 0xFF == 0xE6;
     return outcome;
 }
 
 bool CPU::decode_OR_67(uint32_t instruction) {
     // OR A, r (register) - Opcodes 0xB0-0xB5, 0xB7
     // Pattern 0b10110xxx, excluding 0xB6
     uint8_t opcode = (instruction >> 16) & 0xFF;
     bool outcome = (opcode >= 0xB0 && opcode <= 0xB7 && opcode != 0xB6);
     return outcome;
 }
 
 bool CPU::decode_OR_68(uint32_t instruction) {
     // OR A, (HL) - Opcode 0xB6 (0b10110110)
     bool outcome = (instruction >> 16) & 0xFF == 0xB6;
     return outcome;
 }
 
 bool CPU::decode_OR_69(uint32_t instruction) {
     // OR A, n (immediate) - Opcode 0xF6 (0b11110110)
     bool outcome = (instruction >> 16) & 0xFF == 0xF6;
     return outcome;
 }
 
 bool CPU::decode_XOR_70(uint32_t instruction) {
     // XOR A, r (register) - Opcodes 0xA8-0xAD, 0xAF
     // Pattern 0b10101xxx, excluding 0xAE
     uint8_t opcode = (instruction >> 16) & 0xFF;
     bool outcome = (opcode >= 0xA8 && opcode <= 0xAF && opcode != 0xAE);
     return outcome;
 }
 
 bool CPU::decode_XOR_71(uint32_t instruction) {
     // XOR A, (HL) - Opcode 0xAE (0b10101110)
     bool outcome = (instruction >> 16) & 0xFF == 0xAE;
     return outcome;
 }

// Ella

// Rishi
bool CPU::decode_SWAP_98(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   ((((instruction >> 8) & 0xFF) & 0b11111000) == 0b00110000) &&
                   (((instruction >> 8) & 0xFF) != 0x36); // opcode

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
                   ((((instruction >> 8) & 0xFF) & 0b11111000) == 0b00111000) &&
                   (((instruction >> 8) & 0xFF) == 0x3E); // opcode

    return outcome;
}

bool CPU::decode_SRL_101(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   (((instruction >> 8) & 0xFF) == 0x3E); // opcode

    return outcome;
}

bool CPU::decode_BIT_102(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   ((((instruction >> 8) & 0xFF) & 0b11000000) == 0b01000000) &&
                   ((((instruction >> 8) & 0xFF) & 0b11000111) == 0b01000110); // opcode

    return outcome;
}

bool CPU::decode_BIT_103(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   ((((instruction >> 8) & 0xFF) & 0b11000111) == 0b01000110); // opcode

    return outcome;
}

bool CPU::decode_RES_104(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   ((((instruction >> 8) & 0xFF) & 0b11000000) == 0b10000000) &&
                   ((((instruction >> 8) & 0xFF) & 0b11000111) == 0b10000110); // opcode

    return outcome;
}

bool CPU::decode_RES_105(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   ((((instruction >> 8) & 0xFF) & 0b11000111) == 0b10000110); // opcode

    return outcome;
}

bool CPU::decode_SET_106(uint32_t instruction) { // 2-byte instruction
    bool outcome = (((instruction >> 16) & 0xFF) == 0xCB) && // 0xCB prefix
                   ((((instruction >> 8) & 0xFF) & 0b11000000) == 0b11000000) &&
                   ((((instruction >> 8) & 0xFF) & 0b11000111) == 0b11000110); // opcode

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
    uint8_t data = ram->read_mem(addr); 

    regs[reg] = data;
}

void CPU::execute_LD_23(uint32_t instruction) {
    // LD (HL), r
    uint8_t operation = static_cast<uint8_t>((instruction >> 16) & 0xFF);
    uint8_t reg = (operation & 0b00111000) >> 3;
    uint16_t addr = get_hl(); // Get the HL register value
    uint8_t data = regs[reg]; 

   ram->write_mem(addr, data);
}

void CPU::execute_LD_24(uint32_t instruction) {
    // LD (HL), n (immediate)
    uint8_t operation = static_cast<uint8_t>((instruction >> 16) & 0xFF);
    uint16_t addr = get_hl(); // Get the HL register value
    uint8_t n = static_cast<uint8_t>((instruction >> 8) & 0xFF); // Get the immediate value

    ram->write_mem(addr, n);
}

void CPU::execute_LD_25(uint32_t instruction) {
    // LD A, (BC)
    uint16_t addr = get_bc(); // Get the BC register value
    uint8_t data = ram->read_mem(addr); 

    regs[A_REGISTER] = data;
}

void CPU::execute_LD_26(uint32_t instruction) {
    // LD A, (DE)
    uint16_t addr = get_de(); // Get the DE register value
    uint8_t data = ram->read_mem(addr); 

    regs[A_REGISTER] = data;
}

void CPU::execute_LD_27(uint32_t instruction) {
    // LD (BC), A
    uint16_t addr = get_bc(); // Get the BC register value
    uint8_t data = regs[A_REGISTER]; 

    ram->write_mem(addr, data);
}

void CPU::execute_LD_28(uint32_t instruction) {
    // LD (DE), A
    uint16_t addr = get_de(); // Get the DE register value
    uint8_t data = regs[A_REGISTER]; 

    ram->write_mem(addr, data);
}

void CPU::execute_LD_29(uint32_t instruction) {
    // LD A, (nn)
    uint8_t lsb = static_cast<uint8_t>((instruction >> 8) & 0xFF); // Get the LSB of the immediate value
    uint8_t msb = static_cast<uint8_t>(instruction & 0xFF); // Get the MSB of the immediate value
    uint16_t addr = msb;
    addr <<= 8;
    addr |= lsb; // Combine LSB and MSB to form the address
    uint8_t data = ram->read_mem(addr); 

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

    ram->write_mem(addr, data);
}

void CPU::execute_LD_31(uint32_t instruction) {
    // LD A, (C)
    uint8_t c_val = regs[C_REGISTER]; // Get the C register value
    uint16_t addr = 0xFF00 | c_val; // Address is 0xFF00 + C

    uint8_t data = ram->read_mem(addr); 

    regs[A_REGISTER] = data;
}

void CPU::execute_LD_32(uint32_t instruction) {
    // LD (C), A
    uint8_t c_val = regs[C_REGISTER]; // Get the C register value
    uint16_t addr = 0xFF00 | c_val; // Address is 0xFF00 + C

    uint8_t data = regs[A_REGISTER]; 

    ram->write_mem(addr, data);
}

void CPU::execute_LD_33(uint32_t instruction) {
    // LD A, (n)
    uint8_t n = static_cast<uint8_t>((instruction >> 8) & 0xFF); // Get the immediate value
    uint16_t addr = 0xFF00 | n; // Address is 0xFF00 + n

    uint8_t data = ram->read_mem(addr); 

    regs[A_REGISTER] = data;
}

void CPU::execute_LD_34(uint32_t instruction) {
    // LD (n), A
    uint8_t n = static_cast<uint8_t>((instruction >> 8) & 0xFF); // Get the immediate value
    uint16_t addr = 0xFF00 | n; // Address is 0xFF00 + n

    uint8_t data = regs[A_REGISTER]; 

    ram->write_mem(addr, data);
}

void CPU::execute_LD_35(uint32_t instruction) {
    // LD A, (HL-)
    uint16_t addr = get_hl(); // Get the HL register value
    uint8_t data = ram->read_mem(addr); 

    regs[A_REGISTER] = data;

    // Decrement HL after reading
    set_hl(addr - 1);
}

void CPU::execute_LD_36(uint32_t instruction) {
    // LD (HL-), A
    uint16_t addr = get_hl(); // Get the HL register value
    uint8_t data = regs[A_REGISTER]; 

    ram->write_mem(addr, data);

    // Decrement HL after writing
    set_hl(addr - 1);
}

void CPU::execute_LD_37(uint32_t instruction) {
    // LD A, (HL+)
    uint16_t addr = get_hl(); // Get the HL register value
    uint8_t data = ram->read_mem(addr); 

    regs[A_REGISTER] = data;

    // Increment HL after reading
    set_hl(addr + 1);
}

void CPU::execute_LD_38(uint32_t instruction) {
    // LD (HL+), A
    uint16_t addr = get_hl(); // Get the HL register value
    uint8_t data = regs[A_REGISTER]; 

    ram->write_mem(addr, data);

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

    ram->write_mem(addr, static_cast<uint8_t>(sp & 0xFF)); // Store LSB of SP
    ram->write_mem(addr + 1, static_cast<uint8_t>((sp >> 8) & 0xFF)); // Store MSB of SP
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
    ram->write_mem(sp, static_cast<uint8_t>(rr >> 8)); // Store MSB of rr
    sp--;
    ram->write_mem(sp, static_cast<uint8_t>(rr & 0xFF)); // Store LSB of rr
}

void CPU::execute_POP_43(uint32_t instruction) {
    // POP rr
    uint8_t operation = static_cast<uint8_t>((instruction >> 16) & 0xFF);
    uint16_t data;

    // Pop the value from the stack into rr
    uint8_t lsb = ram->read_mem(sp++); // Get lsb from stack, increment SP
    uint8_t msb = ram->read_mem(sp++); // Get msb from stack, increment SP

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
 void CPU::execute_ADD_46(uint32_t instruction) {
     // ADD A, (HL) - Opcode 0b10000110/0x86
     uint16_t addr = get_hl();
     uint8_t data = ram->read_mem(addr); 
     
     uint8_t a_val = regs[A_REGISTER];
 
     uint16_t result16 = static_cast<uint16_t>(a_val) + static_cast<uint16_t>(data);
     uint8_t result8 = static_cast<uint8_t>(result16);
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, false);
     set_flag(H_FLAG_BIT, ((a_val & 0x0F) + (data & 0x0F)) > 0x0F);
     set_flag(C_FLAG_BIT, result16 > 0xFF);
 
     regs[A_REGISTER] = result8;
     pc++;
 }
 
 void CPU::execute_ADD_47(uint32_t instruction) {
     // ADD A, n (immediate) - Opcode 0xC6, 2-byte instruction
     pc++;
 
     // Read immediate value 'n' from memory at the new PC location.
     uint8_t n = ram->read_mem(pc);
 
     pc++;
     uint8_t a_val = regs[A_REGISTER];
 
     uint16_t result16 = static_cast<uint16_t>(a_val) + static_cast<uint16_t>(n);
     uint8_t result8 = static_cast<uint8_t>(result16);
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0); // Z = 1 if result is 0
     set_flag(N_FLAG_BIT, false);       // N = 0 for ADD
     set_flag(H_FLAG_BIT, ((a_val & 0x0F) + (n & 0x0F)) > 0x0F); // H = 1 if carry from bit 3
     set_flag(C_FLAG_BIT, result16 > 0xFF); // C = 1 if carry from bit 7
 
     regs[A_REGISTER] = result8;
 }
 
 void CPU::execute_ADC_48(uint32_t instruction) {
     // ADC A, r (register) - Opcodes 0x88-0x8D, 0x8F
     uint8_t opcode = (instruction >> 16) & 0xFF;
     int source_reg_index = opcode & 0x07; // Lower 3 bits map to registers B,C,D,E,H,L,(HL),A
     uint8_t r_val = 0;
     // Map index to register value (B=0, C=1, D=2, E=3, H=4, L=5, A=7), (HL) is handled by ADC A, (HL)
     switch(source_reg_index) {
         case B_REGISTER: r_val = regs[B_REGISTER]; break; // 0
         case C_REGISTER: r_val = regs[C_REGISTER]; break; // 1
         case D_REGISTER: r_val = regs[D_REGISTER]; break; // 2
         case E_REGISTER: r_val = regs[E_REGISTER]; break; // 3
         case H_REGISTER: r_val = regs[H_REGISTER]; break; // 4
         case L_REGISTER: r_val = regs[L_REGISTER]; break; // 5
         // case 6 is (HL)
         case A_REGISTER: r_val = regs[A_REGISTER]; break; // 7
     }
 
     uint8_t a_val = regs[A_REGISTER];
     uint8_t carry = get_flag(C_FLAG_BIT) ? 1 : 0;
 
     uint16_t result16 = static_cast<uint16_t>(a_val) + static_cast<uint16_t>(r_val) + static_cast<uint16_t>(carry);
     uint8_t result8 = static_cast<uint8_t>(result16);
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, false);
     set_flag(H_FLAG_BIT, ((a_val & 0x0F) + (r_val & 0x0F) + carry) > 0x0F);
     set_flag(C_FLAG_BIT, result16 > 0xFF);
 
     regs[A_REGISTER] = result8;
     pc++; 
 }
 
 void CPU::execute_ADC_49(uint32_t instruction) {
     // ADC A, (HL) - Opcode 0x8E, 1-byte instruction.
 
     uint16_t addr = get_hl();
     uint8_t data = ram->read_mem(addr);
     uint8_t a_val = regs[A_REGISTER];
     uint8_t carry = get_flag(C_FLAG_BIT) ? 1 : 0;
 
     uint16_t result16 = static_cast<uint16_t>(a_val) + static_cast<uint16_t>(data) + static_cast<uint16_t>(carry);
     uint8_t result8 = static_cast<uint8_t>(result16);
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, false);
     set_flag(H_FLAG_BIT, ((a_val & 0x0F) + (data & 0x0F) + carry) > 0x0F);
     set_flag(C_FLAG_BIT, result16 > 0xFF);
 
     regs[A_REGISTER] = result8;
     pc++;
 }
 
 void CPU::execute_ADC_50(uint32_t instruction) {
     // ADC A, n (immediate) - Opcode 0xCE, 2-byte instruction (pc points to the 0xCE opcode itself on entry)(?)
 
     //Increment PC to point past the opcode (0xCE).
     pc++;
     uint8_t n = ram->read_mem(pc);
     pc++;
 
     uint8_t a_val = regs[A_REGISTER];
     uint8_t carry = get_flag(C_FLAG_BIT) ? 1 : 0;
 
     uint16_t result16 = static_cast<uint16_t>(a_val) + static_cast<uint16_t>(n) + static_cast<uint16_t>(carry);
     uint8_t result8 = static_cast<uint8_t>(result16);
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, false);
     set_flag(H_FLAG_BIT, ((a_val & 0x0F) + (n & 0x0F) + carry) > 0x0F);
     set_flag(C_FLAG_BIT, result16 > 0xFF);
 
     regs[A_REGISTER] = result8;
 }
 
 void CPU::execute_SUB_51(uint32_t instruction) {
     // SUB A, r (register) - Opcodes 0x90-0x95, 0x97, 1-byte instruction.
 
     uint8_t opcode = (instruction >> 16) & 0xFF;
     int source_reg_index = opcode & 0x07;
     uint8_t r_val = 0;
     switch(source_reg_index) {
         case B_REGISTER: r_val = regs[B_REGISTER]; break; // 0
         case C_REGISTER: r_val = regs[C_REGISTER]; break; // 1
         case D_REGISTER: r_val = regs[D_REGISTER]; break; // 2
         case E_REGISTER: r_val = regs[E_REGISTER]; break; // 3
         case H_REGISTER: r_val = regs[H_REGISTER]; break; // 4
         case L_REGISTER: r_val = regs[L_REGISTER]; break; // 5
         // case 6 is (HL)
         case A_REGISTER: r_val = regs[A_REGISTER]; break; // 7
     }
 
     uint8_t a_val = regs[A_REGISTER];
     uint8_t result8 = a_val - r_val;
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, true);
     set_flag(H_FLAG_BIT, (a_val & 0x0F) < (r_val & 0x0F));
     set_flag(C_FLAG_BIT, a_val < r_val);
 
     regs[A_REGISTER] = result8;
     pc++;
 }
 
 void CPU::execute_SUB_52(uint32_t instruction) {
     // SUB A, (HL) - Opcode 0x96, 1-byte instruction.
     uint16_t addr = get_hl();
 
     uint8_t data = ram->read_mem(addr);
     uint8_t a_val = regs[A_REGISTER];
 
     uint8_t result8 = a_val - data;
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, true);
     set_flag(H_FLAG_BIT, (a_val & 0x0F) < (data & 0x0F));
     set_flag(C_FLAG_BIT, a_val < data);
 
     regs[A_REGISTER] = result8;
     pc++;
 }
 
 void CPU::execute_SUB_53(uint32_t instruction) {
     // SUB A, n (immediate) - Opcode 0xD6, 2-byte instruction
 
     pc++;
     uint8_t n = ram->read_mem(pc);
     pc++;
 
     uint8_t a_val = regs[A_REGISTER];
     uint8_t result8 = a_val - n;
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, true);
     set_flag(H_FLAG_BIT, (a_val & 0x0F) < (n & 0x0F));
     set_flag(C_FLAG_BIT, a_val < n);
 
     regs[A_REGISTER] = result8;
 }
 
 void CPU::execute_SBC_54(uint32_t instruction) {
     // SBC A, r (register) - Opcodes 0x98-0x9D, 0x9F, 1-byte instruction
 
     uint8_t opcode = (instruction >> 16) & 0xFF;
     int source_reg_index = opcode & 0x07; // Lower 3 bits map to registers B,C,D,E,H,L,(HL),A
     uint8_t r_val = 0;
     switch(source_reg_index) {
         case B_REGISTER: r_val = regs[B_REGISTER]; break; // 0
         case C_REGISTER: r_val = regs[C_REGISTER]; break; // 1
         case D_REGISTER: r_val = regs[D_REGISTER]; break; // 2
         case E_REGISTER: r_val = regs[E_REGISTER]; break; // 3
         case H_REGISTER: r_val = regs[H_REGISTER]; break; // 4
         case L_REGISTER: r_val = regs[L_REGISTER]; break; // 5
         // case 6 is (HL)
         case A_REGISTER: r_val = regs[A_REGISTER]; break; // 7
     }
 
     uint8_t a_val = regs[A_REGISTER];
     uint8_t carry = get_flag(C_FLAG_BIT) ? 1 : 0;
 
     uint16_t temp_sub = static_cast<uint16_t>(r_val) + static_cast<uint16_t>(carry); // Combine subtrahends
     uint8_t result8 = a_val - static_cast<uint8_t>(temp_sub); // Perform subtraction
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, true);
     set_flag(H_FLAG_BIT, (a_val & 0x0F) < (r_val & 0x0F) + carry);
     set_flag(C_FLAG_BIT, static_cast<uint16_t>(a_val) < temp_sub);
 
     regs[A_REGISTER] = result8;
     pc++;
 }
 
 void CPU::execute_SBC_55(uint32_t instruction) {
     // SBC A, (HL) - Opcode 0x9E, 1-byte instruction
 
     uint16_t addr = get_hl();
     uint8_t data = ram->read_mem(addr);
 
     uint8_t a_val = regs[A_REGISTER];
     uint8_t carry = get_flag(C_FLAG_BIT) ? 1 : 0;
 
     uint16_t temp_sub = static_cast<uint16_t>(data) + static_cast<uint16_t>(carry); // Combine subtrahends
     uint8_t result8 = a_val - static_cast<uint8_t>(temp_sub); // Perform subtraction
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, true);
     set_flag(H_FLAG_BIT, (a_val & 0x0F) < (data & 0x0F) + carry);
     set_flag(C_FLAG_BIT, static_cast<uint16_t>(a_val) < temp_sub);
 
     regs[A_REGISTER] = result8;
     pc++;
 }
 
 void CPU::execute_SBC_56(uint32_t instruction) {
     // SBC A, n (immediate) - Opcode 0xDE, 2-byte instruction
     pc++;
     uint8_t n = ram->read_mem(pc);
     pc++;
 
     uint8_t a_val = regs[A_REGISTER];
     uint8_t carry = get_flag(C_FLAG_BIT) ? 1 : 0;
 
     uint16_t temp_sub = static_cast<uint16_t>(n) + static_cast<uint16_t>(carry); // Combine subtrahends
     uint8_t result8 = a_val - static_cast<uint8_t>(temp_sub); // Perform subtraction
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, true);
     set_flag(H_FLAG_BIT, (a_val & 0x0F) < (n & 0x0F) + carry);
     set_flag(C_FLAG_BIT, static_cast<uint16_t>(a_val) < temp_sub);
 
     regs[A_REGISTER] = result8;
 }
 
 void CPU::execute_CP_57(uint32_t instruction) {
     // CP A, r (register) - Opcodes 0xB8-0xBD, 0xBF, 1-byte instruction
     uint8_t opcode = (instruction >> 16) & 0xFF;
     int source_reg_index = opcode & 0x07;
     uint8_t r_val = 0;
     switch(source_reg_index) {
         case B_REGISTER: r_val = regs[B_REGISTER]; break; // 0
         case C_REGISTER: r_val = regs[C_REGISTER]; break; // 1
         case D_REGISTER: r_val = regs[D_REGISTER]; break; // 2
         case E_REGISTER: r_val = regs[E_REGISTER]; break; // 3
         case H_REGISTER: r_val = regs[H_REGISTER]; break; // 4
         case L_REGISTER: r_val = regs[L_REGISTER]; break; // 5
         // case 6 is (HL)
         case A_REGISTER: r_val = regs[A_REGISTER]; break; // 7
     }
 
     uint8_t a_val = regs[A_REGISTER];
     uint8_t result8 = a_val - r_val; // Temporary result for Z flag
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, true);
     set_flag(H_FLAG_BIT, (a_val & 0x0F) < (r_val & 0x0F));
     set_flag(C_FLAG_BIT, a_val < r_val);
 
     pc++;
 }
 
 void CPU::execute_CP_58(uint32_t instruction) {
     // CP A, (HL) - Opcode 0xBE, 1-byte instruction.
 
     uint16_t addr = get_hl();
     uint8_t data = ram->read_mem(addr);
 
     uint8_t a_val = regs[A_REGISTER];
     uint8_t result8 = a_val - data; // Temporary result for Z flag
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, true);
     set_flag(H_FLAG_BIT, (a_val & 0x0F) < (data & 0x0F));
     set_flag(C_FLAG_BIT, a_val < data);
 
     pc++;
 }
 
 void CPU::execute_CP_59(uint32_t instruction) {
     // CP A, n (immediate) - Opcode 0xFE, 2-byte instruction
     pc++;
     uint8_t n = ram->read_mem(pc);
     pc++;
 
     uint8_t a_val = regs[A_REGISTER];
     uint8_t result8 = a_val - n;
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, true);
     set_flag(H_FLAG_BIT, (a_val & 0x0F) < (n & 0x0F));
     set_flag(C_FLAG_BIT, a_val < n);
 }
 
 void CPU::execute_INC_60(uint32_t instruction) {
     // INC r (register) - Opcodes 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x3C, 1-byte instruction.
     uint8_t opcode = (instruction >> 16) & 0xFF;
     int target_reg_index = (opcode >> 3) & 0x07;
     uint8_t old_val = regs[target_reg_index];
     uint8_t new_val = old_val + 1;
 
     // Set flags
     set_flag(Z_FLAG_BIT, new_val == 0);
     set_flag(N_FLAG_BIT, false);
     set_flag(H_FLAG_BIT, ((old_val & 0x0F) + 1) > 0x0F);
     // no change to C flag
 
     regs[target_reg_index] = new_val;
     pc++;
 }
 
 void CPU::execute_INC_61(uint32_t instruction) {
     // INC (HL) - Opcode 0x34, 1-byte instruction
 
     uint16_t addr = get_hl();
     uint8_t old_val = ram->read_mem(addr);
     uint8_t new_val = old_val + 1;
 
     // Set flags
     set_flag(Z_FLAG_BIT, new_val == 0);
     set_flag(N_FLAG_BIT, false);
     set_flag(H_FLAG_BIT, ((old_val & 0x0F) + 1) > 0x0F);
     // no change to C flag
 
     ram->write_mem(addr, new_val);
     pc++;
 }
 
 void CPU::execute_DEC_62(uint32_t instruction) {
     // DEC r (register) - Opcodes 0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x3D, 1-byte instruction
     uint8_t opcode = (instruction >> 16) & 0xFF;
     int target_reg_index = (opcode >> 3) & 0x07;
     uint8_t old_val = regs[target_reg_index];
     uint8_t new_val = old_val - 1;
 
     // Set flags
     set_flag(Z_FLAG_BIT, new_val == 0);
     set_flag(N_FLAG_BIT, true);
     set_flag(H_FLAG_BIT, (old_val & 0x0F) == 0x00);
 
     regs[target_reg_index] = new_val;
     pc++;
 }
 
 void CPU::execute_DEC_63(uint32_t instruction) {
     // DEC (HL) - Opcode 0x35, 1-byte instruction
 
     uint16_t addr = get_hl();
     uint8_t old_val = ram->read_mem(addr);
     uint8_t new_val = old_val - 1;
 
     // Set flags
     set_flag(Z_FLAG_BIT, new_val == 0);
     set_flag(N_FLAG_BIT, true);
     set_flag(H_FLAG_BIT, (old_val & 0x0F) == 0x00);
 
     ram->write_mem(addr, new_val);
     pc++;
 }
 
 void CPU::execute_AND_64(uint32_t instruction) {
     // AND A, r (register) - Opcodes 0xA0-0xA5, 0xA7, 1-byte instruction
     uint8_t opcode = (instruction >> 16) & 0xFF;
     int source_reg_index = opcode & 0x07;
     uint8_t r_val = 0;
 
     switch(source_reg_index) {
         case B_REGISTER: r_val = regs[B_REGISTER]; break; // 0
         case C_REGISTER: r_val = regs[C_REGISTER]; break; // 1
         case D_REGISTER: r_val = regs[D_REGISTER]; break; // 2
         case E_REGISTER: r_val = regs[E_REGISTER]; break; // 3
         case H_REGISTER: r_val = regs[H_REGISTER]; break; // 4
         case L_REGISTER: r_val = regs[L_REGISTER]; break; // 5
         // case 6 is (HL)
         case A_REGISTER: r_val = regs[A_REGISTER]; break; // 7
     }
 
     uint8_t a_val = regs[A_REGISTER];
     uint8_t result8 = a_val & r_val;
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, false);
     set_flag(H_FLAG_BIT, true);
     set_flag(C_FLAG_BIT, false);
 
     regs[A_REGISTER] = result8;
     pc++;
 }
 
 void CPU::execute_AND_65(uint32_t instruction) {
     // AND A, (HL) - Opcode 0xA6,  1-byte instruction
     uint16_t addr = get_hl();
     uint8_t data = ram->read_mem(addr);
     uint8_t a_val = regs[A_REGISTER];
     uint8_t result8 = a_val & data;
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, false);
     set_flag(H_FLAG_BIT, true);
     set_flag(C_FLAG_BIT, false);
 
     regs[A_REGISTER] = result8;
     pc++;
 }
 
 void CPU::execute_AND_66(uint32_t instruction) {
     // AND A, n (immediate) - Opcode 0xE6, 2-byte instruction
     pc++;
     uint8_t n = ram->read_mem(pc);
     pc++;
 
     uint8_t a_val = regs[A_REGISTER];
     uint8_t result8 = a_val & n;
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, false);
     set_flag(H_FLAG_BIT, true);
     set_flag(C_FLAG_BIT, false);
 
     regs[A_REGISTER] = result8;
 }
 
 void CPU::execute_OR_67(uint32_t instruction) {
     // OR A, r (register) - Opcodes 0xB0-0xB5, 0xB7, 1-byte instruction
     uint8_t opcode = (instruction >> 16) & 0xFF;
     int source_reg_index = opcode & 0x07;
     uint8_t r_val = 0;
     switch(source_reg_index) {
         case B_REGISTER: r_val = regs[B_REGISTER]; break; // 0
         case C_REGISTER: r_val = regs[C_REGISTER]; break; // 1
         case D_REGISTER: r_val = regs[D_REGISTER]; break; // 2
         case E_REGISTER: r_val = regs[E_REGISTER]; break; // 3
         case H_REGISTER: r_val = regs[H_REGISTER]; break; // 4
         case L_REGISTER: r_val = regs[L_REGISTER]; break; // 5
         // case 6 is (HL)
         case A_REGISTER: r_val = regs[A_REGISTER]; break; // 7
     }
 
     uint8_t a_val = regs[A_REGISTER];
     uint8_t result8 = a_val | r_val;
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, false);
     set_flag(H_FLAG_BIT, false);
     set_flag(C_FLAG_BIT, false);
 
     regs[A_REGISTER] = result8;
     pc++;
 }
 
 void CPU::execute_OR_68(uint32_t instruction) {
     // OR A, (HL) - Opcode 0xB6, 1-byte instruction
     uint16_t addr = get_hl();
 
     uint8_t data = ram->read_mem(addr);
     uint8_t a_val = regs[A_REGISTER];
     uint8_t result8 = a_val | data;
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, false);
     set_flag(H_FLAG_BIT, false);
     set_flag(C_FLAG_BIT, false);
 
     regs[A_REGISTER] = result8;
     pc++;
 }
 
 void CPU::execute_OR_69(uint32_t instruction) {
     // OR A, n (immediate) - Opcode 0xF6, 2-byte instruction.
     pc++;
     uint8_t n = ram->read_mem(pc);
     pc++;
 
     uint8_t a_val = regs[A_REGISTER];
     uint8_t result8 = a_val | n;
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, false);
     set_flag(H_FLAG_BIT, false);
     set_flag(C_FLAG_BIT, false);
 
     regs[A_REGISTER] = result8;
 }
 
 void CPU::execute_XOR_70(uint32_t instruction) {
     // XOR A, r (register) - Opcodes 0xA8-0xAD, 0xAF, 1-byte instruction.
     uint8_t opcode = (instruction >> 16) & 0xFF;
 
     int source_reg_index = opcode & 0x07;
     uint8_t r_val = 0;
     switch(source_reg_index) {
         case B_REGISTER: r_val = regs[B_REGISTER]; break; // 0
         case C_REGISTER: r_val = regs[C_REGISTER]; break; // 1
         case D_REGISTER: r_val = regs[D_REGISTER]; break; // 2
         case E_REGISTER: r_val = regs[E_REGISTER]; break; // 3
         case H_REGISTER: r_val = regs[H_REGISTER]; break; // 4
         case L_REGISTER: r_val = regs[L_REGISTER]; break; // 5
         // case 6 is (HL)
         case A_REGISTER: r_val = regs[A_REGISTER]; break; // 7
     }
 
     uint8_t a_val = regs[A_REGISTER];
     uint8_t result8 = a_val ^ r_val;
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, false);
     set_flag(H_FLAG_BIT, false);
     set_flag(C_FLAG_BIT, false);
 
     regs[A_REGISTER] = result8;
     pc++;
 }
 
 void CPU::execute_XOR_71(uint32_t instruction) {
     // XOR A, (HL) - Opcode 0xAE, 1-byte instruction.
     // Assumption: pc points to the opcode itself on entry.
 
     uint16_t addr = get_hl();
     uint8_t data = ram->read_mem(addr);
     uint8_t a_val = regs[A_REGISTER];
     uint8_t result8 = a_val ^ data;
 
     // Set flags
     set_flag(Z_FLAG_BIT, result8 == 0);
     set_flag(N_FLAG_BIT, false);
     set_flag(H_FLAG_BIT, false);
     set_flag(C_FLAG_BIT, false);
 
     regs[A_REGISTER] = result8;
     pc++;
 }

// Ella

// Rishi
void CPU::execute_SWAP_98(uint32_t instruction) {
    uint8_t opcode = (instruction >> 8) & 0xFF;
    uint8_t reg = opcode & 0b00000111;

    // Retrieve value
    uint8_t cur_data = regs[reg];
    // Swap nibbles
    uint8_t new_data = ((cur_data & 0x0F) << 4) | ((cur_data & 0xF0) >> 4);
    // Store value
    regs[reg] = new_data;

    // Set flags
    set_flag(Z_FLAG_BIT, !new_data);
    set_flag(N_FLAG_BIT, false);
    set_flag(H_FLAG_BIT, false);
    set_flag(C_FLAG_BIT, false);

    pc += 2; // 2-byte instruction
}

void CPU::execute_SWAP_99(uint32_t instruction) {
    // Retrieve value
    uint8_t cur_data = (*ram).read_mem(get_hl());
    // Swap nibbles
    uint8_t new_data = ((cur_data & 0x0F) << 4) | ((cur_data & 0xF0) >> 4);
    // Store value
    (*ram).write_mem(get_hl(), new_data);

    // Set flags
    set_flag(Z_FLAG_BIT, !new_data);
    set_flag(N_FLAG_BIT, false);
    set_flag(H_FLAG_BIT, false);
    set_flag(C_FLAG_BIT, false);

    pc += 2; // 2-byte instruction
}

void CPU::execute_SRL_100(uint32_t instruction) {
    uint8_t opcode = (instruction >> 8) & 0xFF;
    uint8_t reg = opcode & 0b00000111;

    // Retrieve value
    uint8_t cur_data = regs[reg];
    // Shift right
    uint8_t new_data = cur_data >> 1;
    // Store value
    regs[reg] = new_data;

    // Set flags
    set_flag(Z_FLAG_BIT, !new_data);
    set_flag(N_FLAG_BIT, false);
    set_flag(H_FLAG_BIT, false);
    set_flag(C_FLAG_BIT, cur_data & 1);

    pc += 2; // 2-byte instruction
}

void CPU::execute_SRL_101(uint32_t instruction) {
    // Retrieve value
    uint8_t cur_data = (*ram).read_mem(get_hl());
    // Shift right
    uint8_t new_data = cur_data >> 1;
    // Store value
    (*ram).write_mem(get_hl(), new_data);

    // Set flags
    set_flag(Z_FLAG_BIT, !new_data);
    set_flag(N_FLAG_BIT, false);
    set_flag(H_FLAG_BIT, false);
    set_flag(C_FLAG_BIT, cur_data & 1);

    pc += 2; // 2-byte instruction
}

void CPU::execute_BIT_102(uint32_t instruction) {
    uint8_t opcode = (instruction >> 8) & 0xFF;
    uint8_t reg = opcode & 0b00000111;
    uint8_t bit = (opcode >> 3) & 0b00000111;

    // Retrieve value
    uint8_t cur_data = regs[reg];
    // Check bit
    bool val = (cur_data >> bit) & 1;

    // Set flags
    set_flag(Z_FLAG_BIT, !val);
    set_flag(N_FLAG_BIT, false);
    set_flag(H_FLAG_BIT, true);

    pc += 2; // 2-byte instruction
}

void CPU::execute_BIT_103(uint32_t instruction) {
    uint8_t opcode = (instruction >> 8) & 0xFF;
    uint8_t bit = (opcode >> 3) & 0b00000111;
    
    // Retrieve value
    uint8_t cur_data = (*ram).read_mem(get_hl());
    // Check bit
    bool val = (cur_data >> bit) & 1;

    // Set flags
    set_flag(Z_FLAG_BIT, !val);
    set_flag(N_FLAG_BIT, false);
    set_flag(H_FLAG_BIT, true);

    pc += 2; // 2-byte instruction
}

void CPU::execute_RES_104(uint32_t instruction) {
    uint8_t opcode = (instruction >> 8) & 0xFF;
    uint8_t reg = opcode & 0b00000111;
    uint8_t bit = (opcode >> 3) & 0b00000111;

    // Retrieve value
    uint8_t cur_data = regs[reg];
    // Clear bit
    uint8_t new_data = cur_data & ~(1 << bit);
    // Store value
    regs[reg] = new_data;

    pc += 2; // 2-byte instruction
}

void CPU::execute_RES_105(uint32_t instruction) {
    uint8_t opcode = (instruction >> 8) & 0xFF;
    uint8_t bit = (opcode >> 3) & 0b00000111;

    // Retrieve value
    uint8_t cur_data = (*ram).read_mem(get_hl());
    // Clear bit
    uint8_t new_data = cur_data & ~(1 << bit);
    // Store value
    (*ram).write_mem(get_hl(), new_data);

    pc += 2; // 2-byte instruction
}

void CPU::execute_SET_106(uint32_t instruction) {
    uint8_t opcode = (instruction >> 8) & 0xFF;
    uint8_t reg = opcode & 0b00000111;
    uint8_t bit = (opcode >> 3) & 0b00000111;

    // Retrieve value
    uint8_t cur_data = regs[reg];
    // Set bit
    uint8_t new_data = cur_data | (1 << bit);
    // Store value
    regs[reg] = new_data;

    pc += 2; // 2-byte instruction
}

void CPU::execute_SET_107(uint32_t instruction) {
    uint8_t opcode = (instruction >> 8) & 0xFF;
    uint8_t bit = (opcode >> 3) & 0b00000111;

    // Retrieve value
    uint8_t cur_data = (*ram).read_mem(get_hl());
    // Set bit
    uint8_t new_data = cur_data | (1 << bit);
    // Store value
    (*ram).write_mem(get_hl(), new_data);

    pc += 2; // 2-byte instruction
}

void CPU::execute_JP_109(uint32_t instruction) {
    uint8_t lsb = (instruction >> 8) & 0xFF;
    uint8_t msb = instruction & 0xFF;

    // Calculate jump address
    uint16_t jmp_addr = static_cast<uint16_t>(msb << 8) | lsb;

    pc = jmp_addr; // Unconditional jump
}

void CPU::execute_JP_110(uint32_t instruction) {
    // Calculate jump address
    uint16_t jmp_addr = get_hl();

    pc = jmp_addr; // Unconditional jump
}

void CPU::execute_JP_111(uint32_t instruction) {
    uint8_t opcode = (instruction >> 16) & 0xFF;
    uint8_t condition = (opcode >> 3) & 0b00000011;

    bool condition_met;
    switch(condition) {
        case 0b00: // NZ
            condition_met = !get_flag(Z_FLAG_BIT);
            break;

        case 0b01: // Z
            condition_met = get_flag(Z_FLAG_BIT);
            break;

        case 0b10: // NC
            condition_met = !get_flag(C_FLAG_BIT);
            break;

        case 0b11: // C
            condition_met = get_flag(C_FLAG_BIT);
            break;
    }

    if (condition_met) {
        uint8_t lsb = (instruction >> 8) & 0xFF;
        uint8_t msb = instruction & 0xFF;

        // Calculate jump address
        uint16_t jmp_addr = static_cast<uint16_t>(msb << 8) | lsb;

        pc = jmp_addr;
    } else {
        pc += 3; // 3-byte instruction
    }
}

void CPU::execute_JR_113(uint32_t instruction) {
    uint8_t offset = (instruction >> 8) & 0xFF;

    // Calculate jump address
    int8_t signed_offset = static_cast<int8_t>(offset);
    uint16_t jmp_addr = pc + signed_offset;

    pc = jmp_addr; // Unconditional jump
}

void CPU::execute_JR_114(uint32_t instruction) {
    uint8_t opcode = (instruction >> 16) & 0xFF;
    uint8_t condition = (opcode >> 3) & 0b00000011;

    bool condition_met;
    switch(condition) {
        case 0b00: // NZ
            condition_met = !get_flag(Z_FLAG_BIT);
            break;

        case 0b01: // Z
            condition_met = get_flag(Z_FLAG_BIT);
            break;

        case 0b10: // NC
            condition_met = !get_flag(C_FLAG_BIT);
            break;

        case 0b11: // C
            condition_met = get_flag(C_FLAG_BIT);
            break;
    }

    if (condition_met) {
        uint8_t offset = (instruction >> 8) & 0xFF;

        // Calculate jump address
        int8_t signed_offset = static_cast<int8_t>(offset);
        uint16_t jmp_addr = pc + signed_offset;

        pc = jmp_addr;
    } else {
        pc += 2; // 2-byte instruction
    }
}

// TODO: Implement safe stack operations

void CPU::execute_CALL_116(uint32_t instruction) {
    uint8_t lsb = (instruction >> 8) & 0xFF;
    uint8_t msb = instruction & 0xFF;

    // Calculate call address
    uint16_t call_addr = static_cast<uint16_t>(msb << 8) | lsb;

    // Push current PC onto stack
    sp -= 2;
    (*ram).write_mem(sp, pc & 0xFF); // PC LSB
    (*ram).write_mem(sp + 1, (pc >> 8) & 0xFF); // PC MSB

    pc = call_addr;
}

void CPU::execute_CALL_117(uint32_t instruction) {
    uint8_t opcode = (instruction >> 16) & 0xFF;
    uint8_t condition = (opcode >> 3) & 0b00000011;

    bool condition_met;
    switch(condition) {
        case 0b00: // NZ
            condition_met = !get_flag(Z_FLAG_BIT);
            break;

        case 0b01: // Z
            condition_met = get_flag(Z_FLAG_BIT);
            break;

        case 0b10: // NC
            condition_met = !get_flag(C_FLAG_BIT);
            break;

        case 0b11: // C
            condition_met = get_flag(C_FLAG_BIT);
            break;
    }

    if (condition_met) {
        uint8_t lsb = (instruction >> 8) & 0xFF;
        uint8_t msb = instruction & 0xFF;

        // Calculate call address
        uint16_t call_addr = static_cast<uint16_t>(msb << 8) | lsb;

        // Push current PC onto stack
        sp -= 2;
        (*ram).write_mem(sp, pc & 0xFF); // PC LSB
        (*ram).write_mem(sp + 1, (pc >> 8) & 0xFF); // PC MSB

        pc = call_addr;
    } else {
        pc += 3; // 3-byte instruction
    }
}

void CPU::execute_RET_119(uint32_t instruction) {
    // Pop address from stack
    uint8_t lsb = (*ram).read_mem(sp);
    uint8_t msb = (*ram).read_mem(sp + 1);
    sp += 2;

    // Calculate return address
    uint16_t ret_addr = static_cast<uint16_t>(msb << 8) | lsb;

    pc = ret_addr; // Unconditional return
}

void CPU::execute_RET_120(uint32_t instruction) {
    uint8_t opcode = (instruction >> 16) & 0xFF;
    uint8_t condition = (opcode >> 3) & 0b00000011;

    bool condition_met;
    switch(condition) {
        case 0b00: // NZ
            condition_met = !get_flag(Z_FLAG_BIT);
            break;

        case 0b01: // Z
            condition_met = get_flag(Z_FLAG_BIT);
            break;

        case 0b10: // NC
            condition_met = !get_flag(C_FLAG_BIT);
            break;

        case 0b11: // C
            condition_met = get_flag(C_FLAG_BIT);
            break;
    }

    if (condition_met) {
        // Pop address from stack
        uint8_t lsb = (*ram).read_mem(sp);
        uint8_t msb = (*ram).read_mem(sp + 1);
        sp += 2;

        // Calculate return address
        uint16_t ret_addr = static_cast<uint16_t>(msb << 8) | lsb;

        pc = ret_addr;
    } else {
        pc += 1; // 1-byte instruction
    }
}

void CPU::execute_RETI_121(uint32_t instruction) {
    // Pop address from stack
    uint8_t lsb = (*ram).read_mem(sp);
    uint8_t msb = (*ram).read_mem(sp + 1);
    sp += 2;

    // Calculate return address
    uint16_t ret_addr = static_cast<uint16_t>(msb << 8) | lsb;

    ime = true; // Enable interrupts

    pc = ret_addr; // Unconditional return
}

void CPU::execute_RST_122(uint32_t instruction) {
    uint8_t opcode = (instruction >> 16) & 0xFF;
    uint8_t addr = (opcode >> 3) & 0b00000111;

    // Push current PC onto stack
    sp -= 2;
    (*ram).write_mem(sp, pc & 0xFF); // PC LSB
    (*ram).write_mem(sp + 1, (pc >> 8) & 0xFF); // PC MSB

    // Jump based on addr
    switch (addr) {
        case 0b000:
            pc = 0x0000;
            break;

        case 0b001:
            pc = 0x0008;
            break;

        case 0b010:
            pc = 0x0010;
            break;

        case 0b011:
            pc = 0x0018;
            break;

        case 0b100:
            pc = 0x0020;
            break;

        case 0b101:
            pc = 0x0028;
            break;

        case 0b110:
            pc = 0x0030;
            break;

        case 0b111:
            pc = 0x0038;
            break;
    }
}

void CPU::execute_DI_123(uint32_t instruction) {
    ime = false; // Disable interrupts

    pc += 1; // 1-byte instruction
}

void CPU::execute_EI_124(uint32_t instruction) {
    ime = true; // Enable interrupts

    pc += 1; // 1-byte instruction
}
