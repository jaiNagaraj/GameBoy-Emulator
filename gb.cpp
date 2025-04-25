#include "gb.hpp"
#include <iostream>
#include <fstream>
#include <vector>

// Constructor
GheithBoy::GheithBoy() : cpu(nullptr), window(nullptr), window_surface(nullptr) {}

// Destructor
GheithBoy::~GheithBoy() {
    delete input;
    delete ppu;
    delete mmu;
    delete ram;
    delete mmap;
    delete cpu;
}

bool GheithBoy::load_rom(MMAP* mmap, const std::string& rom_path) {
    if (!mmap) {
        std::cerr << "Error: MMAP object is null in load_rom." << std::endl;
        return false;
    }

    // Open the ROM file in binary mode, positioned at the end
    std::ifstream rom_file(rom_path, std::ios::binary | std::ios::ate);

    if (!rom_file.is_open()) {
        std::cerr << "Error: Failed to open ROM file: " << rom_path << std::endl;
        return false;
    }

    // Get the size of the file
    std::streamsize size = rom_file.tellg();
    rom_file.seekg(0, std::ios::beg); // Go back to the beginning

    if (size == 0) {
         std::cerr << "file is empty: " << rom_path << std::endl;
         return false;
    }

    std::cout << "Loading ROM: " << rom_path << " (" << size << " bytes)" << std::endl;

    // Read the ROM data into a buffer
    std::vector<char> buffer(size);
    if (!rom_file.read(buffer.data(), size)) {
        std::cerr << "Error: Failed to read ROM file: " << rom_path << std::endl;
        rom_file.close();
        return false;
    }

    rom_file.close();

    // Limit loading to 32KB for now
    size_t load_size = std::min((size_t)size, (size_t)0x8000);
    for (size_t i = 0; i < load_size; ++i) {
        mmap->write_mem(static_cast<uint16_t>(i), static_cast<uint8_t>(buffer[i]));
    }

    std::cout << "Loaded " << load_size << " bytes into memory." << std::endl;
    return true;
}

void GheithBoy::handle_input(const SDL_Event& event) {
    if (!input) return;

    bool pressed = (event.type == SDL_KEYDOWN);
    int button_index = -1;

    // Map SDL keys to your button indices (adjust keys as needed)
    switch (event.key.keysym.sym) {
        case SDLK_RIGHT: button_index = 0; break; // Right
        case SDLK_LEFT:  button_index = 1; break; // Left
        case SDLK_UP:    button_index = 2; break; // Up
        case SDLK_DOWN:  button_index = 3; break; // Down
        case SDLK_z:     button_index = 4; break; // A button
        case SDLK_x:     button_index = 5; break; // B button
        case SDLK_RSHIFT:/*fallthrough*/ // Use Right Shift for Select
        case SDLK_BACKSPACE: button_index = 6; break; // Select
        case SDLK_RETURN: button_index = 7; break; // Start (Enter key)
    }

    if (button_index != -1) {
        input->set_button_state(button_index, pressed);
        // Optional: Request Joypad interrupt if a button was pressed
        if (pressed && mmu) {
           uint8_t if_reg = mmu->read_mem(0xFF0F);
           mmu->write_mem(0xFF0F, if_reg | 0x10); // Set Joypad interrupt flag (bit 4)
        }
    }
}

void GheithBoy::run_gb(const std::string& rom_path) {
    cpu = new CPU();
    mmap = new MMAP();
    ram = new RAM();
    mmu = new MMU();
    ppu = new PPU();
    input = new Input();
    
    if (!load_rom(mmap, rom_path)) {
        std::cerr << "ROM path incorrect or it didn't load properly >:( \nI give up!" << std::endl;
        // Destructor will handle cleanup
        return;
    }

    ram->connect_mmap(mmap);
    mmu->connect_mmap(mmap);
    mmu->connect_ram(ram);
    ppu->connect_mmu(mmu);
    cpu->connect_mmu(mmu);
    mmu->connect_input(input);
    cpu->connect_mmu(mmu);
    ppu->connect_mmu(mmu);   

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
        // Event handling
        SDL_Event event;
        while (SDL_PollEvent(&event)) { // Process all pending events
            if (event.type == SDL_QUIT) {
                keep_window_open = false;
                continue;
            }
            // handle input events
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                handle_input(event);
            }
        }

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
        }
        else if (cpu->decode_XOR_72(instruction)) {
            cpu->execute_XOR_72(instruction);
        }
        else if (cpu->decode_CCF_73(instruction)) {
            cpu->execute_CCF_73(instruction);
        }
        else if (cpu->decode_SCF_74(instruction)) {
            cpu->execute_SCF_74(instruction);
        }
        else if (cpu->decode_DAA_75(instruction)) {
            cpu->execute_DAA_75(instruction);
        }
        else if (cpu->decode_CPL_76(instruction)) {
            cpu->execute_CPL_76(instruction);
        }
        else if (cpu->decode_INC_77(instruction)) {
            cpu->execute_INC_77(instruction);
        }
        else if (cpu->decode_DEC_78(instruction)) {
            cpu->execute_DEC_78(instruction);
        }
        else if (cpu->decode_ADD_79(instruction)) {
            cpu->execute_ADD_79(instruction);
        }
        else if (cpu->decode_ADD_80(instruction)) {
            cpu->execute_ADD_80(instruction);
        }
        else if (cpu->decode_RLCA_82(instruction)) {
            cpu->execute_RLCA_82(instruction);
        }
        else if (cpu->decode_RRCA_83(instruction)) {
            cpu->execute_RRCA_83(instruction);
        }
        else if (cpu->decode_RLA_84(instruction)) {
            cpu->execute_RLA_84(instruction);
        }
        else if (cpu->decode_RRA_85(instruction)) {
            cpu->execute_RRA_85(instruction);
        }
        else if (cpu->decode_RLC_86(instruction)) {
            cpu->execute_RLC_86(instruction);
        }
        else if (cpu->decode_RLC_87(instruction)) {
            cpu->execute_RLC_87(instruction);
        }
        else if (cpu->decode_RRC_88(instruction)) {
            cpu->execute_RRC_88(instruction);
        }
        else if (cpu->decode_RRC_89(instruction)) {
            cpu->execute_RRC_89(instruction);
        }
        else if (cpu->decode_RL_90(instruction)) {
            cpu->execute_RL_90(instruction);
        }
        else if (cpu->decode_RL_91(instruction)) {
            cpu->execute_RL_91(instruction);
        }
        else if (cpu->decode_RR_92(instruction)) {
            cpu->execute_RR_92(instruction);
        }
        else if (cpu->decode_RR_93(instruction)) {
            cpu->execute_RR_93(instruction);
        }
        else if (cpu->decode_SLA_94(instruction)) {
            cpu->execute_SLA_94(instruction);
        }
        else if (cpu->decode_SLA_95(instruction)) {
            cpu->execute_SLA_95(instruction);
        }
        else if (cpu->decode_SRA_96(instruction)) {
            cpu->execute_SRA_96(instruction);
        }
        else if (cpu->decode_SRA_97(instruction)) {
            cpu->execute_SRA_97(instruction);
        }
        else if (cpu->decode_SWAP_98(instruction)) {
            cpu->execute_SWAP_98(instruction);
        }
        else if (cpu->decode_SWAP_99(instruction)) {
            cpu->execute_SWAP_99(instruction);
        }
        else if (cpu->decode_SRL_100(instruction)) {
            cpu->execute_SRL_100(instruction);
        }
        else if (cpu->decode_SRL_101(instruction)) {
            cpu->execute_SRL_101(instruction);
        }
        else if (cpu->decode_BIT_102(instruction)) {
            cpu->execute_BIT_102(instruction);
        }
        else if (cpu->decode_BIT_103(instruction)) {
            cpu->execute_BIT_103(instruction);
        }
        else if (cpu->decode_RES_104(instruction)) {
            cpu->execute_RES_104(instruction);
        }
        else if (cpu->decode_RES_105(instruction)) {
            cpu->execute_RES_105(instruction);
        }
        else if (cpu->decode_SET_106(instruction)) {
            cpu->execute_SET_106(instruction);
        }
        else if (cpu->decode_SET_107(instruction)) {
            cpu->execute_SET_107(instruction);
        }
        else if (cpu->decode_JP_109(instruction)) {
            cpu->execute_JP_109(instruction);
        }
        else if (cpu->decode_JP_110(instruction)) {
            cpu->execute_JP_110(instruction);
        }
        else if (cpu->decode_JP_111(instruction)) {
            cpu->execute_JP_111(instruction);
        }
        else if (cpu->decode_JR_113(instruction)) {
            cpu->execute_JR_113(instruction);
        }
        else if (cpu->decode_JR_114(instruction)) {
            cpu->execute_JR_114(instruction);
        }
        else if (cpu->decode_CALL_116(instruction)) {
            cpu->execute_CALL_116(instruction);
		}
		else if (cpu->decode_CALL_117(instruction)) {
			cpu->execute_CALL_117(instruction);
		}
        else if (cpu->decode_RET_119(instruction)) {
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
        else if (cpu->decode_NOP_125(instruction)) {
            cpu->execute_NOP_125(instruction);
        } else {
			std::cout << "Unknown instruction: " << std::hex << instruction << std::endl;
        }

        // screen is updated, reflect that in SDL
        // Call PPU
        uint32_t** pixelsToWrite = ppu->writePixels();
		// Update the window surface with the pixel data
		SDL_LockSurface(window_surface);
		uint32_t* pixels = static_cast<uint32_t*>(window_surface->pixels);
		for (int y = 0; y < WINDOW_HEIGHT; y++) {
			for (int x = 0; x < WINDOW_WIDTH; x++) {
				pixels[y * WINDOW_WIDTH + x] = pixelsToWrite[y][x]; 
			}
		}
        for (int i = 0; i < WINDOW_HEIGHT; i++) {
            delete pixelsToWrite[i];
        }
        delete pixelsToWrite;
		SDL_UnlockSurface(window_surface);
		SDL_UpdateWindowSurface(window);

		// Delay to control frame rate
		SDL_Delay(16); // ~60 FPS
    }
    // Destroyer
    //SDL_DestroyTexture(texture);
    //SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(window);
    //SDL_Quit();
}