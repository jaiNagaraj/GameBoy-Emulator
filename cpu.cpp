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
     uint8_t data = ram->read_memory(addr);
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
     uint8_t n = ram->read_memory(pc);
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
 
     uint8_t data = ram->read_memory(addr);
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
     uint8_t n = ram->read_memory(pc);
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
     uint8_t data = ram->read_memory(addr);
 
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
     uint8_t n = ram->read_memory(pc);
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
     uint8_t data = ram->read_memory(addr);
 
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
     uint8_t n = ram->read_memory(pc);
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
     uint8_t old_val = ram->read_memory(addr);
     uint8_t new_val = old_val + 1;
 
     // Set flags
     set_flag(Z_FLAG_BIT, new_val == 0);
     set_flag(N_FLAG_BIT, false);
     set_flag(H_FLAG_BIT, ((old_val & 0x0F) + 1) > 0x0F);
     // no change to C flag
 
     ram->write_memory(addr, new_val);
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
     uint8_t old_val = ram->read_memory(addr);
     uint8_t new_val = old_val - 1;
 
     // Set flags
     set_flag(Z_FLAG_BIT, new_val == 0);
     set_flag(N_FLAG_BIT, true);
     set_flag(H_FLAG_BIT, (old_val & 0x0F) == 0x00);
 
     ram->write_memory(addr, new_val);
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
     uint8_t data = ram->read_memory(addr);
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
     uint8_t n = ram->read_memory(pc);
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
 
     uint8_t data = ram->read_memory(addr);
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
     uint8_t n = ram->read_memory(pc);
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
     uint8_t data = ram->read_memory(addr);
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