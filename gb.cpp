#include "gb.hpp"
#include <iostream>

void GheithBoy::run_gb() {
    cpu = new CPU();
    // Use this space to load the ROM


    // Use this space to run graphics (will include the main loop)
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        //return -1;
    }

    window = SDL_CreateWindow("GheithBoy",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0);

    if (!window)
    {
        std::cout << "Failed to create window\n";
        //return -1;
    }

    window_surface = SDL_GetWindowSurface(window);

    if (!window_surface)
    {
        std::cout << "Failed to get the surface from the window\n";
        //return -1;
    }

    bool keep_window_open = true;
    while (keep_window_open) {
        // fetch instruction
        uint32_t instruction = cpu->fetch_instruction();

        // decode switch! (i hate ts </3)
        if (cpu->decode_LD_20(instruction)) {
            cpu->execute_LD_20(instruction);
            // other stuff
        } else if (cpu->decode_LD_21(instruction)) {
            cpu->execute_LD_21(instruction);
            // other stuff
        } else if (cpu->decode_LD_22(instruction)) {
            cpu->execute_LD_22(instruction);
            // other stuff
        } else if (cpu->decode_LD_23(instruction)) {
            cpu->execute_LD_23(instruction);
            // other stuff
        } else if (cpu->decode_LD_24(instruction)) {
            cpu->execute_LD_24(instruction);
            // other stuff
        } else if (cpu->decode_LD_25(instruction)) {
            cpu->execute_LD_25(instruction);
            // other stuff
        } else if (cpu->decode_LD_26(instruction)) {
            cpu->execute_LD_26(instruction);
            // other stuff
        } else if (cpu->decode_LD_27(instruction)) {
            cpu->execute_LD_27(instruction);
            // other stuff
        } else if (cpu->decode_LD_28(instruction)) {
            cpu->execute_LD_28(instruction);
            // other stuff
        } else if (cpu->decode_LD_29(instruction)) {
            cpu->execute_LD_29(instruction);
            // other stuff
        } else if (cpu->decode_LD_30(instruction)) {
            cpu->execute_LD_30(instruction);
            // other stuff
        } else if (cpu->decode_LD_31(instruction)) {
            cpu->execute_LD_31(instruction);
            // other stuff
        } else if (cpu->decode_LD_32(instruction)) {
            cpu->execute_LD_32(instruction);
            // other stuff
        } else if (cpu->decode_LD_33(instruction)) {
            cpu->execute_LD_33(instruction);
            // other stuff
        } else if (cpu->decode_LD_34(instruction)) {
            cpu->execute_LD_34(instruction);
            // other stuff
        } else if (cpu->decode_LD_35(instruction)) {
            cpu->execute_LD_35(instruction);
            // other stuff
        } else if (cpu->decode_LD_36(instruction)) {
            cpu->execute_LD_36(instruction);
            // other stuff
        } else if (cpu->decode_LD_37(instruction)) {
            cpu->execute_LD_37(instruction);
            // other stuff
        } else if (cpu->decode_LD_38(instruction)) {
            cpu->execute_LD_38(instruction);
            // other stuff
        } else if (cpu->decode_LD_39(instruction)) {
            cpu->execute_LD_39(instruction);
            // other stuff
        } else if (cpu->decode_LD_40(instruction)) {
            cpu->execute_LD_40(instruction);
            // other stuff
        } else if (cpu->decode_LD_41(instruction)) {
            cpu->execute_LD_41(instruction);
            // other stuff
        } else if (cpu->decode_PUSH_42(instruction)) {
            cpu->execute_PUSH_42(instruction);
            // other stuff
        } else if (cpu->decode_POP_43(instruction)) {
            cpu->execute_POP_43(instruction);
            // other stuff
        } else if (cpu->decode_LD_44(instruction)) {
            cpu->execute_LD_44(instruction);
            // other stuff
        } else if (cpu->decode_ADD_45(instruction)) {
            cpu->execute_ADD_45(instruction);
            // other stuff
        } else if (cpu->decode_ADD_46(instruction)) {
            cpu->execute_ADD_46(instruction);
            // other stuff
        } else if (cpu->decode_ADD_47(instruction)) {
            cpu->execute_ADD_47(instruction);
            // other stuff
        } else if (cpu->decode_ADC_48(instruction)) {
            cpu->execute_ADC_48(instruction);
            // other stuff
        } else if (cpu->decode_ADC_49(instruction)) {
            cpu->execute_ADC_49(instruction);
            // other stuff
        } else if (cpu->decode_ADC_50(instruction)) {
            cpu->execute_ADC_50(instruction);
            // other stuff
        } else if (cpu->decode_SUB_50(instruction)) {
            cpu->execute_SUB_50(instruction);
            // other stuff
        } else if (cpu->decode_SUB_51(instruction)) {
            cpu->execute_SUB_51(instruction);
            // other stuff
        } else if (cpu->decode_SUB_52(instruction)) {
            cpu->execute_SUB_52(instruction);
            // other stuff
        } else if (cpu->decode_SUB_53(instruction)) {
            cpu->execute_SUB_53(instruction);
            // other stuff
        } else if (cpu->decode_SBC_54(instruction)) {
            cpu->execute_SBC_54(instruction);
            // other stuff
        } else if (cpu->decode_SBC_55(instruction)) {
            cpu->execute_SBC_55(instruction);
            // other stuff
        } else if (cpu->decode_SBC_56(instruction)) {
            cpu->execute_SBC_56(instruction);
            // other stuff
        } else if (cpu->decode_CP_57(instruction)) {
            cpu->execute_CP_57(instruction);
            // other stuff
        } else if (cpu->decode_CP_58(instruction)) {
            cpu->execute_CP_58(instruction);
            // other stuff
        } else if (cpu->decode_CP_59(instruction)) {
            cpu->execute_CP_59(instruction);
            // other stuff
        } else if (cpu->decode_INC_60(instruction)) {
            cpu->execute_INC_60(instruction);
            // other stuff
        } else if (cpu->decode_INC_61(instruction)) {
            cpu->execute_INC_61(instruction);
            // other stuff
        } else if (cpu->decode_DEC_62(instruction)) {
            cpu->execute_DEC_62(instruction);
            // other stuff
        } else if (cpu->decode_DEC_63(instruction)) {
            cpu->execute_DEC_63(instruction);
            // other stuff
        } else if (cpu->decode_AND_64(instruction)) {
            cpu->execute_AND_64(instruction);
            // other stuff
        } else if (cpu->decode_AND_65(instruction)) {
            cpu->execute_AND_65(instruction);
            // other stuff
        } else if (cpu->decode_AND_66(instruction)) {
            cpu->execute_AND_66(instruction);
            // other stuff
        } else if (cpu->decode_OR_67(instruction)) {
            cpu->execute_OR_67(instruction);
            // other stuff
        } else if (cpu->decode_OR_68(instruction)) {
            cpu->execute_OR_68(instruction);
            // other stuff
        } else if (cpu->decode_OR_69(instruction)) {
            cpu->execute_OR_69(instruction);
            // other stuff
        } else if (cpu->decode_XOR_70(instruction)) {
            cpu->execute_XOR_70(instruction);
            // other stuff
        } else if (cpu->decode_XOR_71(instruction)) {
            cpu->execute_XOR_71(instruction);
            // other stuff
        } else if (cpu->decode_RET_119(instruction)) {
            cpu->execute_RET_119(instruction);
            // other stuff
        } else if (cpu->decode_RET_120(instruction)) {
            cpu->execute_RET_120(instruction);
            // other stuff
        } else if (cpu->decode_RETI_121(instruction)) {
            cpu->execute_RETI_121(instruction);
            // other stuff
        } else if (cpu->decode_RST_122(instruction)) {
            cpu->execute_RST_122(instruction);
            // other stuff
        } else if (cpu->decode_DI_123(instruction)) {
            cpu->execute_DI_123(instruction);
            // other stuff
        } else if (cpu->decode_EI_124(instruction)) {
            cpu->execute_EI_124(instruction);
            // other stuff
        }

        // screen is updated, reflect that in SDL
        ;
    }
}