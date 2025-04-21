/**
 * This file deals with the operation of the GameBoy CPU. 
 * All operation emulation functions should be defined here.
 */

#include "cpu.hpp"

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
