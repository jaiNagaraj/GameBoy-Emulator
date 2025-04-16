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
    SWAP_98,
    SWAP_99,
    SRL_100,
    SRL_101,
    BIT_102,
    BIT_103,
    RES_104,
    RES_105,
    SET_106,
    SET_107,
    JP_109,
    JP_110,
    JP_111,
    JR_113,
    JR_114,
    CALL_116,
    CALL_117,
    RET_119,
    RET_120,
    RETI_121,
    RST_122,
    HALT_123,
    STOP_123,
    DI_123,
    EI_124
};

class CPU {
    uint8_t regs[8]; // 0: A, 1: B, 2: C, 3: D, 4: E, 5: F, 6: H, 7: L
    uint8_t sp; // Stack Pointer
    uint8_t pc; // Program Counter

    // #### FUNCTION DECLARATIONS ####
    // Jai

    // Archit

    // Ella

    // Rishi
    bool decode_SWAP_98(uint8_t opcode);
    bool decode_SWAP_99(uint8_t opcode);
    bool decode_SRL_100(uint8_t opcode);
    bool decode_SRL_101(uint8_t opcode);
    bool decode_BIT_102(uint8_t opcode);
    bool decode_BIT_103(uint8_t opcode);
    bool decode_RES_104(uint8_t opcode);
    bool decode_RES_105(uint8_t opcode);
    bool decode_SET_106(uint8_t opcode);
    bool decode_SET_107(uint8_t opcode);
    bool decode_JP_109(uint8_t opcode);
    bool decode_JP_110(uint8_t opcode);
    bool decode_JP_111(uint8_t opcode);
    bool decode_JR_113(uint8_t opcode);
    bool decode_JR_114(uint8_t opcode);
    bool decode_CALL_116(uint8_t opcode);
    bool decode_CALL_117(uint8_t opcode);
    bool decode_RET_119(uint8_t opcode);
    bool decode_RET_120(uint8_t opcode);
    bool decode_RETI_121(uint8_t opcode);
    bool decode_RST_122(uint8_t opcode);
    bool decode_HALT_123(uint8_t opcode);
    bool decode_STOP_123(uint8_t opcode);
    bool decode_DI_123(uint8_t opcode);
    bool decode_EI_124(uint8_t opcode);

    void execute_SWAP_98(uint8_t opcode);
    void execute_SWAP_99(uint8_t opcode);
    void execute_SRL_100(uint8_t opcode);
    void execute_SRL_101(uint8_t opcode);
    void execute_BIT_102(uint8_t opcode);
    void execute_BIT_103(uint8_t opcode);
    void execute_RES_104(uint8_t opcode);
    void execute_RES_105(uint8_t opcode);
    void execute_SET_106(uint8_t opcode);
    void execute_SET_107(uint8_t opcode);
    void execute_JP_109(uint8_t opcode);
    void execute_JP_110(uint8_t opcode);
    void execute_JP_111(uint8_t opcode);
    void execute_JR_113(uint8_t opcode);
    void execute_JR_114(uint8_t opcode);
    void execute_CALL_116(uint8_t opcode);
    void execute_CALL_117(uint8_t opcode);
    void execute_RET_119(uint8_t opcode);
    void execute_RET_120(uint8_t opcode);
    void execute_RETI_121(uint8_t opcode);
    void execute_RST_122(uint8_t opcode);
    void execute_HALT_123(uint8_t opcode);
    void execute_STOP_123(uint8_t opcode);
    void execute_DI_123(uint8_t opcode);
    void execute_EI_124(uint8_t opcode);
};
