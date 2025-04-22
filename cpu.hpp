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
    LD_20,
    LD_21,
    LD_22,
    LD_23,
    LD_24,
    LD_25,
    LD_26,
    LD_27,
    LD_28,
    LD_29,
    LD_30,
    LD_31,
    LD_32,
    LD_33,
    LD_34,
    LD_35,
    LD_36,
    LD_37,
    LD_38,
    LD_39,
    LD_40,
    LD_41,
    PUSH_42,
    POP_43,
    LD_44,
    ADD_45,

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
    XOR_72,
    CCF_73,
    SCF_74,
    DAA_75,
    CPL_76,
    INC_77,
    DEC_78,
    ADD_79,
    ADD_80,
    RLCA_82,
    RRCA_83,
    RLA_84,
    RRA_85,
    RLC_86,
    RLC_87,
    RRC_88,
    RRC_89,
    RL_90,
    RL_91,
    RR_92,
    RR_93,
    SLA_94,
    SLA_95,
    SRA_96,
    SRA_97,

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
    bool ime; // Interrupt Master Enable

    // #### FUNCTION DECLARATIONS ####
    // Helper functions
    // Get a specific flag bit
    bool get_flag(int flag_bit);
    // Set a specific flag bit
    void set_flag(int flag_bit, bool value);
    // Get a 16-bit register value
    uint16_t get_hl();
    uint16_t get_bc();
    uint16_t get_de();
    uint16_t get_af();
    // Set a 16-bit register value
    void set_hl(uint16_t val);
    void set_bc(uint16_t val);
    void set_de(uint16_t val);
    void set_af(uint16_t val);

public:
    CPU();
    uint32_t fetch_instruction();
    // Decode & execute declarations
    // Jai
    bool decode_LD_20(uint32_t instruction);
    bool decode_LD_21(uint32_t instruction);
    bool decode_LD_22(uint32_t instruction);
    bool decode_LD_23(uint32_t instruction);
    bool decode_LD_24(uint32_t instruction);
    bool decode_LD_25(uint32_t instruction);
    bool decode_LD_26(uint32_t instruction);
    bool decode_LD_27(uint32_t instruction);
    bool decode_LD_28(uint32_t instruction);
    bool decode_LD_29(uint32_t instruction);
    bool decode_LD_30(uint32_t instruction);
    bool decode_LD_31(uint32_t instruction);
    bool decode_LD_32(uint32_t instruction);
    bool decode_LD_33(uint32_t instruction);
    bool decode_LD_34(uint32_t instruction);
    bool decode_LD_35(uint32_t instruction);
    bool decode_LD_36(uint32_t instruction);
    bool decode_LD_37(uint32_t instruction);
    bool decode_LD_38(uint32_t instruction);
    bool decode_LD_39(uint32_t instruction);
    bool decode_LD_40(uint32_t instruction);
    bool decode_LD_41(uint32_t instruction);
    bool decode_PUSH_42(uint32_t instruction);
    bool decode_POP_43(uint32_t instruction);
    bool decode_LD_44(uint32_t instruction);
    bool decode_ADD_45(uint32_t instruction);

    void execute_LD_20(uint32_t instruction);
    void execute_LD_21(uint32_t instruction);
    void execute_LD_22(uint32_t instruction);
    void execute_LD_23(uint32_t instruction);
    void execute_LD_24(uint32_t instruction);
    void execute_LD_25(uint32_t instruction);
    void execute_LD_26(uint32_t instruction);
    void execute_LD_27(uint32_t instruction);
    void execute_LD_28(uint32_t instruction);
    void execute_LD_29(uint32_t instruction);
    void execute_LD_30(uint32_t instruction);
    void execute_LD_31(uint32_t instruction);
    void execute_LD_32(uint32_t instruction);
    void execute_LD_33(uint32_t instruction);
    void execute_LD_34(uint32_t instruction);
    void execute_LD_35(uint32_t instruction);
    void execute_LD_36(uint32_t instruction);
    void execute_LD_37(uint32_t instruction);
    void execute_LD_38(uint32_t instruction);
    void execute_LD_39(uint32_t instruction);
    void execute_LD_40(uint32_t instruction);
    void execute_LD_41(uint32_t instruction);
    void execute_PUSH_42(uint32_t instruction);
    void execute_POP_43(uint32_t instruction);
    void execute_LD_44(uint32_t instruction);
    void execute_ADD_45(uint32_t instruction);

    // Archit
    bool decode_ADD_46( uint32_t instruction);
    bool decode_ADD_47( uint32_t instruction);
    bool decode_ADC_48( uint32_t instruction);
    bool decode_ADC_49( uint32_t instruction);
    bool decode_ADC_50( uint32_t instruction);
    bool decode_SUB_50( uint32_t instruction);
    bool decode_SUB_51( uint32_t instruction);
    bool decode_SUB_52( uint32_t instruction);
    bool decode_SUB_53( uint32_t instruction);
    bool decode_SBC_54( uint32_t instruction);
    bool decode_SBC_55( uint32_t instruction);
    bool decode_SBC_56( uint32_t instruction);
    bool decode_CP_57( uint32_t instruction);
    bool decode_CP_58( uint32_t instruction);
    bool decode_CP_59( uint32_t instruction);
    bool decode_INC_60( uint32_t instruction);
    bool decode_INC_61( uint32_t instruction);
    bool decode_DEC_62( uint32_t instruction);
    bool decode_DEC_63( uint32_t instruction);
    bool decode_AND_64( uint32_t instruction);
    bool decode_AND_65( uint32_t instruction);
    bool decode_AND_66( uint32_t instruction);
    bool decode_OR_67( uint32_t instruction);
    bool decode_OR_68( uint32_t instruction);
    bool decode_OR_69( uint32_t instruction);
    bool decode_XOR_70( uint32_t instruction);
    bool decode_XOR_71( uint32_t instruction);

    void execute_ADD_46( uint32_t instruction);
    void execute_ADD_47( uint32_t instruction);
    void execute_ADC_48( uint32_t instruction);
    void execute_ADC_49( uint32_t instruction);
    void execute_ADC_50( uint32_t instruction);
    void execute_SUB_50( uint32_t instruction);
    void execute_SUB_51( uint32_t instruction);
    void execute_SUB_52( uint32_t instruction);
    void execute_SUB_53( uint32_t instruction);
    void execute_SBC_54( uint32_t instruction);
    void execute_SBC_55( uint32_t instruction);
    void execute_SBC_56( uint32_t instruction);
    void execute_CP_57( uint32_t instruction);
    void execute_CP_58( uint32_t instruction);
    void execute_CP_59( uint32_t instruction);
    void execute_INC_60( uint32_t instruction);
    void execute_INC_61( uint32_t instruction);
    void execute_DEC_62( uint32_t instruction);
    void execute_DEC_63( uint32_t instruction);
    void execute_AND_64( uint32_t instruction);
    void execute_AND_65( uint32_t instruction);
    void execute_AND_66( uint32_t instruction);
    void execute_OR_67( uint32_t instruction);
    void execute_OR_68( uint32_t instruction);
    void execute_OR_69( uint32_t instruction);
    void execute_XOR_70( uint32_t instruction);
    void execute_XOR_71( uint32_t instruction);
    
    // Ella
    bool decode_XOR_72(uint32_t instruction);
    bool decode_CCF_73(uint32_t instruction);
    bool decode_SCF_74(uint32_t instruction);
    bool decode_DAA_75(uint32_t instruction);
    bool decode_CPL_76(uint32_t instruction);
    bool decode_INC_77(uint32_t instruction);
    bool decode_DEC_78(uint32_t instruction);
    bool decode_ADD_79(uint32_t instruction);
    bool decode_ADD_80(uint32_t instruction);
    bool decode_RLCA_82(uint32_t instruction);
    bool decode_RRCA_83(uint32_t instruction);
    bool decode_RLA_84(uint32_t instruction);
    bool decode_RRA_85(uint32_t instruction);
    bool decode_RLC_86(uint32_t instruction);
    bool decode_RLC_87(uint32_t instruction);
    bool decode_RRC_88(uint32_t instruction);
    bool decode_RRC_89(uint32_t instruction);
    bool decode_RL_90(uint32_t instruction);
    bool decode_RL_91(uint32_t instruction);
    bool decode_RR_92(uint32_t instruction);
    bool decode_RR_93(uint32_t instruction);
    bool decode_SLA_94(uint32_t instruction);
    bool decode_SLA_95(uint32_t instruction);
    bool decode_SRA_96(uint32_t instruction);
    bool decode_SRA_97(uint32_t instruction);

    void execute_XOR_72(uint32_t instruction);
    void execute_CCF_73(uint32_t instruction);
    void execute_SCF_74(uint32_t instruction);
    void execute_DAA_75(uint32_t instruction);
    void execute_CPL_76(uint32_t instruction);
    void execute_INC_77(uint32_t instruction);
    void execute_DEC_78(uint32_t instruction);
    void execute_ADD_79(uint32_t instruction);
    void execute_ADD_80(uint32_t instruction);
    void execute_RLCA_82(uint32_t instruction);
    void execute_RRCA_83(uint32_t instruction);
    void execute_RLA_84(uint32_t instruction);
    void execute_RRA_85(uint32_t instruction);
    void execute_RLC_86(uint32_t instruction);
    void execute_RLC_87(uint32_t instruction);
    void execute_RRC_88(uint32_t instruction);
    void execute_RRC_89(uint32_t instruction);
    void execute_RL_90(uint32_t instruction);
    void execute_RL_91(uint32_t instruction);
    void execute_RR_92(uint32_t instruction);
    void execute_RR_93(uint32_t instruction);
    void execute_SLA_94(uint32_t instruction);
    void execute_SLA_95(uint32_t instruction);
    void execute_SRA_96(uint32_t instruction);
    void execute_SRA_97(uint32_t instruction);

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
    bool decode_NOP_125(uint32_t instruction);

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
    void execute_NOP_125(uint32_t instruction);
};