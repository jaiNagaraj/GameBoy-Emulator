/**
 * Header file for cpu.cpp - declare all operation execution functions in the CPU class.
 * It also contains the structures within the cpu (ex. registers, flags, etc)
 */

#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "RAM.hpp"

const int A_REGISTER = 7;
const int B_REGISTER = 0;
const int C_REGISTER = 1;
const int D_REGISTER = 2;
const int E_REGISTER = 3;
const int FLAGS_REGISTER = 6;
const int H_REGISTER = 4;
const int L_REGISTER = 5;

// flag bit positions
const int Z_FLAG_BIT = 7;
const int N_FLAG_BIT = 6;
const int H_FLAG_BIT = 5;
const int C_FLAG_BIT = 4;

enum INSTRUCTION {
    // Jai

    // Archit
    ADD_46,
    ADD_47,
    ADC_48,
    ADC_49,
    ADC_50,
    SUB_50,
    SUB_51,
    SUB_52,
    SUB_53,
    SBC_54,
    SBC_55,
    SBC_56,
    CP_57,
    CP_58,
    CP_59,
    INC_60,
    INC_61,
    DEC_62,
    DEC_63,
    AND_64,
    AND_65,
    AND_66,
    OR_67,
    OR_68,
    OR_69,
    XOR_70,
    XOR_71,
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
private:
    RAM *ram;
    uint8_t regs[8]; // 0: B, 1: C, 2: D, 3: E, 4: H, 5: L, 6: F, 7: A
    uint16_t pc; // Program Counter
    uint16_t sp; // Stack Pointer

    // #### FUNCTION DECLARATIONS ####
    // Helper functions
    // Set a specific flag bit
    void set_flag(int flag_bit, bool value);
    // Get a specific flag bit
    bool get_flag(int flag_bit);
    uint16_t get_hl();

public:
    // Decode & execute declarations
    // Jai

    // Archit
    bool decode_ADD_46(uint8_t opcode);
    bool decode_ADD_47(uint8_t opcode);
    bool decode_ADC_48(uint8_t opcode);
    bool decode_ADC_49(uint8_t opcode);
    bool decode_ADC_50(uint8_t opcode);
    bool decode_SUB_50(uint8_t opcode);
    bool decode_SUB_51(uint8_t opcode);
    bool decode_SUB_52(uint8_t opcode);
    bool decode_SUB_53(uint8_t opcode);
    bool decode_SBC_54(uint8_t opcode);
    bool decode_SBC_55(uint8_t opcode);
    bool decode_SBC_56(uint8_t opcode);
    bool decode_CP_57(uint8_t opcode);
    bool decode_CP_58(uint8_t opcode);
    bool decode_CP_59(uint8_t opcode);
    bool decode_INC_60(uint8_t opcode);
    bool decode_INC_61(uint8_t opcode);
    bool decode_DEC_62(uint8_t opcode);
    bool decode_DEC_63(uint8_t opcode);
    bool decode_AND_64(uint8_t opcode);
    bool decode_AND_65(uint8_t opcode);
    bool decode_AND_66(uint8_t opcode);
    bool decode_OR_67(uint8_t opcode);
    bool decode_OR_68(uint8_t opcode);
    bool decode_OR_69(uint8_t opcode);
    bool decode_XOR_70(uint8_t opcode);
    bool decode_XOR_71(uint8_t opcode);

    void execute_ADD_46(uint8_t opcode);
    void execute_ADD_47(uint8_t opcode);
    void execute_ADC_48(uint8_t opcode);
    void execute_ADC_49(uint8_t opcode);
    void execute_ADC_50(uint8_t opcode);
    void execute_SUB_50(uint8_t opcode);
    void execute_SUB_51(uint8_t opcode);
    void execute_SUB_52(uint8_t opcode);
    void execute_SUB_53(uint8_t opcode);
    void execute_SBC_54(uint8_t opcode);
    void execute_SBC_55(uint8_t opcode);
    void execute_SBC_56(uint8_t opcode);
    void execute_CP_57(uint8_t opcode);
    void execute_CP_58(uint8_t opcode);
    void execute_CP_59(uint8_t opcode);
    void execute_INC_60(uint8_t opcode);
    void execute_INC_61(uint8_t opcode);
    void execute_DEC_62(uint8_t opcode);
    void execute_DEC_63(uint8_t opcode);
    void execute_AND_64(uint8_t opcode);
    void execute_AND_65(uint8_t opcode);
    void execute_AND_66(uint8_t opcode);
    void execute_OR_67(uint8_t opcode);
    void execute_OR_68(uint8_t opcode);
    void execute_OR_69(uint8_t opcode);
    void execute_XOR_70(uint8_t opcode);
    void execute_XOR_71(uint8_t opcode);
    // Ella

    // Rishi
    bool decode_SWAP_98(uint32_t instruction);
    bool decode_SWAP_99(uint32_t instruction);
    bool decode_SRL_100(uint32_t instruction);
    bool decode_SRL_101(uint32_t instruction);
    bool decode_BIT_102(uint32_t instruction);
    bool decode_BIT_103(uint32_t instruction);
    bool decode_RES_104(uint32_t instruction);
    bool decode_RES_105(uint32_t instruction);
    bool decode_SET_106(uint32_t instruction);
    bool decode_SET_107(uint32_t instruction);
    bool decode_JP_109(uint32_t instruction);
    bool decode_JP_110(uint32_t instruction);
    bool decode_JP_111(uint32_t instruction);
    bool decode_JR_113(uint32_t instruction);
    bool decode_JR_114(uint32_t instruction);
    bool decode_CALL_116(uint32_t instruction);
    bool decode_CALL_117(uint32_t instruction);
    bool decode_RET_119(uint32_t instruction);
    bool decode_RET_120(uint32_t instruction);
    bool decode_RETI_121(uint32_t instruction);
    bool decode_RST_122(uint32_t instruction);
    /*bool decode_HALT_123(uint32_t instruction);
    bool decode_STOP_123(uint32_t instruction);*/
    bool decode_DI_123(uint32_t instruction);
    bool decode_EI_124(uint32_t instruction);

    void execute_SWAP_98(uint32_t instruction);
    void execute_SWAP_99(uint32_t instruction);
    void execute_SRL_100(uint32_t instruction);
    void execute_SRL_101(uint32_t instruction);
    void execute_BIT_102(uint32_t instruction);
    void execute_BIT_103(uint32_t instruction);
    void execute_RES_104(uint32_t instruction);
    void execute_RES_105(uint32_t instruction);
    void execute_SET_106(uint32_t instruction);
    void execute_SET_107(uint32_t instruction);
    void execute_JP_109(uint32_t instruction);
    void execute_JP_110(uint32_t instruction);
    void execute_JP_111(uint32_t instruction);
    void execute_JR_113(uint32_t instruction);
    void execute_JR_114(uint32_t instruction);
    void execute_CALL_116(uint32_t instruction);
    void execute_CALL_117(uint32_t instruction);
    void execute_RET_119(uint32_t instruction);
    void execute_RET_120(uint32_t instruction);
    void execute_RETI_121(uint32_t instruction);
    void execute_RST_122(uint32_t instruction);
    /*void execute_HALT_123(uint32_t instruction);
    void execute_STOP_123(uint32_t instruction);*/
    void execute_DI_123(uint32_t instruction);
    void execute_EI_124(uint32_t instruction);
};
