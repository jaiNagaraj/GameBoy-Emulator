/**
 * Header file for cpu.cpp - declare all operation execution functions in the CPU class.
 * It also contains the structures within the cpu (ex. registers, flags, etc)
 */

#pragma once
#include <stdint.h>
const int A_REGISTER = 0;
const int B_REGISTER = 1;
const int C_REGISTER = 2;
const int D_REGISTER = 3;
const int E_REGISTER = 4;
const int FLAGS_REGISTER = 5;
const int H_REGISTER = 6;
const int L_REGISTER = 7;

enum INSTRUCTION {
    // Jai

    // Archit

    // Ella

    // Rishi

};

class CPU {
    uint8_t regs[8]; // 0: A, 1: B, 2: C, 3: D, 4: E, 5: F, 6: H, 7: L
    uint8_t sp; // Stack Pointer
    uint8_t pc; // Program Counter

    // #### FUNCTION DECLARATIONS ####
};

// Jai

// Archit

// Ella

// Rishi
