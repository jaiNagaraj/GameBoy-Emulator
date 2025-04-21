/**
 * This file deals with the operation of the GameBoy CPU. 
 * All operation emulation functions should be defined here.
 */

#include "cpu.hpp"

CPU::CPU() {
    ram = new RAM();

    pc = 0x0100;
    sp = 0xFFFE;

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
        cur_data = (*ram).read_mem(get_hl());
    }
    // Swap nibbles
    uint8_t new_data = ((cur_data & 0x0F) << 4) | ((cur_data & 0xF0) >> 4);
    // Store value
    if (reg != 6) {
        regs[reg] = new_data;
    } else {
        (*ram).write_mem(get_hl(), new_data);
    }

    // Set flags
    set_flag(Z_FLAG_BIT, new_data == 0);
    set_flag(N_FLAG_BIT, false);
    set_flag(H_FLAG_BIT, false);
    set_flag(C_FLAG_BIT, false);

    pc += 2; // 2-byte instruction
}
