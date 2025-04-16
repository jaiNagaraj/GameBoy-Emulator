/**
 * Header file for cpu.cpp - declare all operation execution functions in the CPU class.
 * It also contains the structures within the cpu (ex. registers, flags, etc)
 */

#pragma once
#include <stdint.h>
const int FLAGS_REGISTER = 5;

enum INSTRUCTION {

};

class CPU {
    uint8_t regs[8]; // 0: A, 1: B, 2: C, 3: D, 4: E, 5: F, 6: H, 7: L
    uint8_t sp; // Stack Pointer
    uint8_t pc; // Program Counter

    // #### FUNCTION DECLARATIONS ####
};