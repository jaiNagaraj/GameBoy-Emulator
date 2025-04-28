#include "gb.hpp"
#include <iostream>
#include <fstream>
#include <vector>

//#define ENABLE_INSTR_LOG
//#define ENABLE_BOOT

// Constructor
GheithBoy::GheithBoy() : cpu(nullptr), window(nullptr), window_surface(nullptr) {}

// Destructor
GheithBoy::~GheithBoy()
{
    delete input;
    delete ppu;
    delete mmu;
    delete ram;
    delete mmap;
    delete cpu;
}

bool GheithBoy::load_boot(MMAP *mmap)
{
    std::string boot_path = "boot.bin";
    if (!mmap)
    {
        std::cerr << "Error: MMAP object is null in load_boot." << std::endl;
        return false;
    }

    // Open the ROM file in binary mode, positioned at the end
    std::ifstream rom_file(boot_path, std::ios::binary | std::ios::ate);

    if (!rom_file.is_open())
    {
        std::cerr << "Error: Failed to open ROM file: " << boot_path << std::endl;
        return false;
    }

    // Get the size of the file
    std::streamsize size = rom_file.tellg();
    rom_file.seekg(0, std::ios::beg); // Go back to the beginning

    if (size == 0)
    {
        std::cerr << "file is empty: " << boot_path << std::endl;
        return false;
    }

    std::cout << "Loading ROM: " << boot_path << " (" << size << " bytes)" << std::endl;

    // Read the ROM data into a buffer
    std::vector<char> buffer(size);
    if (!rom_file.read(buffer.data(), size))
    {
        std::cerr << "Error: Failed to read ROM file: " << boot_path << std::endl;
        rom_file.close();
        return false;
    }

    rom_file.close();

    // Limit loading to 32KB for now
    size_t load_size = std::min((size_t)size, (size_t)0x8000);
    for (size_t i = 0; i < load_size; ++i)
    {
        mmap->write_mem(static_cast<uint16_t>(i), static_cast<uint8_t>(buffer[i]));
    }

    std::cout << "Loaded " << load_size << " bytes into memory." << std::endl;
    return true;
}

bool GheithBoy::load_rom(MMAP *mmap, const std::string &rom_path)
{
    if (!mmap)
    {
        std::cerr << "Error: MMAP object is null in load_rom." << std::endl;
        return false;
    }

    // Open the ROM file in binary mode, positioned at the end
    std::ifstream rom_file(rom_path, std::ios::binary | std::ios::ate);

    if (!rom_file.is_open())
    {
        std::cerr << "Error: Failed to open ROM file: " << rom_path << std::endl;
        return false;
    }

    // Get the size of the file
    std::streamsize size = rom_file.tellg();
    rom_file.seekg(0, std::ios::beg); // Go back to the beginning

    if (size == 0)
    {
        std::cerr << "file is empty: " << rom_path << std::endl;
        return false;
    }

    std::cout << "Loading ROM: " << rom_path << " (" << size << " bytes)" << std::endl;

    // Read the ROM data into a buffer
    std::vector<char> buffer(size);
    if (!rom_file.read(buffer.data(), size))
    {
        std::cerr << "Error: Failed to read ROM file: " << rom_path << std::endl;
        rom_file.close();
        return false;
    }

    rom_file.close();

    // Limit loading to 32KB for now
    size_t load_size = std::min((size_t)size, (size_t)0x8000);
    for (size_t i = 0; i < load_size; ++i)
    {
        mmap->write_mem(static_cast<uint16_t>(i), static_cast<uint8_t>(buffer[i]));
    }

    std::cout << "Loaded " << load_size << " bytes into memory." << std::endl;
    return true;
}

void GheithBoy::handle_input(const SDL_Event &event)
{
    if (!input)
        return;

    bool pressed = (event.type == SDL_KEYDOWN);
    int button_index = -1;

    // Map SDL keys to your button indices (adjust keys as needed)
    switch (event.key.keysym.sym)
    {
    case SDLK_RIGHT:
        button_index = 0;
        break; // Right
    case SDLK_LEFT:
        button_index = 1;
        break; // Left
    case SDLK_UP:
        button_index = 2;
        break; // Up
    case SDLK_DOWN:
        button_index = 3;
        break; // Down
    case SDLK_z:
        button_index = 4;
        break; // A button
    case SDLK_x:
        button_index = 5;
        break;                        // B button
    case SDLK_RSHIFT: /*fallthrough*/ // Use Right Shift for Select
    case SDLK_BACKSPACE:
        button_index = 6;
        break; // Select
    case SDLK_RETURN:
        button_index = 7;
        break; // Start (Enter key)
    }

    if (button_index != -1)
    {
        input->set_button_state(button_index, pressed);
        // Optional: Request Joypad interrupt if a button was pressed
        if (pressed && mmu)
        {
            uint8_t if_reg = mmu->read_mem(0xFF0F);
            mmu->write_mem(0xFF0F, if_reg | 0x10); // Set Joypad interrupt flag (bit 4)
        }
    }
}

void GheithBoy::run_gb(const std::string &rom_path)
{
    cpu = new CPU();
    mmap = new MMAP();
    ram = new RAM();
    mmu = new MMU();
    ppu = new PPU();
    input = new Input();
    IH = new InterruptHandler();
    timer = new Timer();


    if (!load_rom(mmap, rom_path))
    {
        std::cerr << "ROM path incorrect or it didn't load properly >:( \nI give up!" << std::endl;
        // Destructor will handle cleanup
        return;
    }
#ifdef ENABLE_BOOT
    if (!load_boot(mmap))
    {
        std::cerr << "ROM path incorrect or it didn't load properly >:( \nI give up!" << std::endl;
        // Destructor will handle cleanup
        return;
    }
#endif // ENABLE_BOOT

    ram->connect_mmap(mmap);
    mmu->connect_mmap(mmap);
    mmu->connect_ram(ram);
    mmu->connect_input(input);
    ppu->connect_mmu(mmu);
    ppu->connect_interrupt_handler(IH);
    ppu->connect_mmu(mmu);
    ppu->connect_ram(ram);
    cpu->connect_mmu(mmu);
    cpu->connect_interrupt_handler(IH);
    cpu->connect_mmu(mmu);
    IH->connect_mmu(mmu);
    timer->connect_mmu(mmu);
    timer->connect_ram(ram);

    // Use this space to run graphics (will include the main loop)
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        // return -1;
    }

    window = SDL_CreateWindow("GheithBoy",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH, WINDOW_HEIGHT,
                              0);

    if (!window)
    {
        std::cout << "Failed to create window\n";
        // return -1;
    }

    window_surface = SDL_GetWindowSurface(window);

    if (!window_surface)
    {
        std::cout << "Failed to get the surface from the window\n";
        // return -1;
    }

    bool keep_window_open = true;
    while (keep_window_open)
    {
        // Event handling
        SDL_Event event;
        while (SDL_PollEvent(&event))
        { // Process all pending events
            if (event.type == SDL_QUIT)
            {
                keep_window_open = false;
                continue;
            }
            // handle input events
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            {
                handle_input(event);
            }
        }

        // Interrupt handling
        cpu->handle_interrupts();

        // fetch instruction
        //std::cout << "PC: " << std::hex << cpu->get_pc() << std::dec << '\n';
        uint32_t instruction = cpu->fetch_instruction();

        // decode switch! (i hate ts </3)
        if (cpu->decode_LD_20(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 20\n";
#endif // ENABLE_INSTR_LOG

            cpu->execute_LD_20(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_21(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 21\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_21(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_22(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 22\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_22(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_23(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 23\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_23(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_24(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 24\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_24(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_25(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 25\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_25(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_26(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 26\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_26(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_27(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 27\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_27(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_28(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 28\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_28(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_29(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 29\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_29(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_30(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 30\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_30(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_31(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 31\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_31(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_32(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 32\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_32(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_33(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 33\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_33(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_34(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 34\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_34(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_35(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 35\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_35(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_36(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 36\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_36(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_37(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 37\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_37(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_38(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 38\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_38(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_39(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 39\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_39(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_40(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 40\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_40(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_41(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 41\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_41(instruction);
            // other stuff
        }
        else if (cpu->decode_PUSH_42(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "PUSH 42\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_PUSH_42(instruction);
            // other stuff
        }
        else if (cpu->decode_POP_43(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "POP 43\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_POP_43(instruction);
            // other stuff
        }
        else if (cpu->decode_LD_44(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "LD 44\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_LD_44(instruction);
            // other stuff
        }
        else if (cpu->decode_ADD_45(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "ADD 45\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_ADD_45(instruction);
            // other stuff
        }
        else if (cpu->decode_ADD_46(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "ADD 46\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_ADD_46(instruction);
            // other stuff
        }
        else if (cpu->decode_ADD_47(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "ADD 47\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_ADD_47(instruction);
            // other stuff
        }
        else if (cpu->decode_ADC_48(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "ADD 48\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_ADC_48(instruction);
            // other stuff
        }
        else if (cpu->decode_ADC_49(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "ADD 49\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_ADC_49(instruction);
            // other stuff
        }
        else if (cpu->decode_ADC_50(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "ADD 50\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_ADC_50(instruction);
            // other stuff
        }
        else if (cpu->decode_SUB_51(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SUB 51\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SUB_51(instruction);
            // other stuff
        }
        else if (cpu->decode_SUB_52(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SUB 52\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SUB_52(instruction);
            // other stuff
        }
        else if (cpu->decode_SUB_53(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SUB 53\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SUB_53(instruction);
            // other stuff
        }
        else if (cpu->decode_SBC_54(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SBC 54\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SBC_54(instruction);
            // other stuff
        }
        else if (cpu->decode_SBC_55(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SBC 55\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SBC_55(instruction);
            // other stuff
        }
        else if (cpu->decode_SBC_56(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SBC 56\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SBC_56(instruction);
            // other stuff
        }
        else if (cpu->decode_CP_57(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "CP 57\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_CP_57(instruction);
            // other stuff
        }
        else if (cpu->decode_CP_58(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "CP 58\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_CP_58(instruction);
            // other stuff
        }
        else if (cpu->decode_CP_59(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "CP 59\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_CP_59(instruction);
            // other stuff
        }
        else if (cpu->decode_INC_60(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "INC 60\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_INC_60(instruction);
            // other stuff
        }
        else if (cpu->decode_INC_61(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "INC 61\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_INC_61(instruction);
            // other stuff
        }
        else if (cpu->decode_DEC_62(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "DEC 62\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_DEC_62(instruction);
            // other stuff
        }
        else if (cpu->decode_DEC_63(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "DEC 63\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_DEC_63(instruction);
            // other stuff
        }
        else if (cpu->decode_AND_64(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "AND 64\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_AND_64(instruction);
            // other stuff
        }
        else if (cpu->decode_AND_65(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "AND 65\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_AND_65(instruction);
            // other stuff
        }
        else if (cpu->decode_AND_66(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "AND 66\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_AND_66(instruction);
            // other stuff
        }
        else if (cpu->decode_OR_67(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "OR 67\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_OR_67(instruction);
            // other stuff
        }
        else if (cpu->decode_OR_68(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "OR 68\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_OR_68(instruction);
            // other stuff
        }
        else if (cpu->decode_OR_69(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "OR 69\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_OR_69(instruction);
            // other stuff
        }
        else if (cpu->decode_XOR_70(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "XOR 70\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_XOR_70(instruction);
            // other stuff
        }
        else if (cpu->decode_XOR_71(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "XOR 71\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_XOR_71(instruction);
            // other stuff
        }
        else if (cpu->decode_XOR_72(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "XOR 72\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_XOR_72(instruction);
        }
        else if (cpu->decode_CCF_73(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "CCF 73\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_CCF_73(instruction);
        }
        else if (cpu->decode_SCF_74(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SCF 74\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SCF_74(instruction);
        }
        else if (cpu->decode_DAA_75(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "DAA 75\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_DAA_75(instruction);
        }
        else if (cpu->decode_CPL_76(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "CPL 76\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_CPL_76(instruction);
        }
        else if (cpu->decode_INC_77(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "INC 77\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_INC_77(instruction);
        }
        else if (cpu->decode_DEC_78(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "DEC 78\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_DEC_78(instruction);
        }
        else if (cpu->decode_ADD_79(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "ADD 79\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_ADD_79(instruction);
        }
        else if (cpu->decode_ADD_80(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "ADD 80\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_ADD_80(instruction);
        }
        else if (cpu->decode_RLCA_82(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RLCA 82\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RLCA_82(instruction);
        }
        else if (cpu->decode_RRCA_83(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RRCA 83\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RRCA_83(instruction);
        }
        else if (cpu->decode_RLA_84(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RLA 84\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RLA_84(instruction);
        }
        else if (cpu->decode_RRA_85(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RRA 85\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RRA_85(instruction);
        }
        else if (cpu->decode_RLC_86(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RLC 86\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RLC_86(instruction);
        }
        else if (cpu->decode_RLC_87(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RLC 87\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RLC_87(instruction);
        }
        else if (cpu->decode_RRC_88(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RRC 88\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RRC_88(instruction);
        }
        else if (cpu->decode_RRC_89(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RRC 89\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RRC_89(instruction);
        }
        else if (cpu->decode_RL_90(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RL 90\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RL_90(instruction);
        }
        else if (cpu->decode_RL_91(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RL 91\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RL_91(instruction);
        }
        else if (cpu->decode_RR_92(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RR 92\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RR_92(instruction);
        }
        else if (cpu->decode_RR_93(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RR 93\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RR_93(instruction);
        }
        else if (cpu->decode_SLA_94(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SLA 94\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SLA_94(instruction);
        }
        else if (cpu->decode_SLA_95(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SLA 95\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SLA_95(instruction);
        }
        else if (cpu->decode_SRA_96(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SRA 96\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SRA_96(instruction);
        }
        else if (cpu->decode_SRA_97(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SRA 97\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SRA_97(instruction);
        }
        else if (cpu->decode_SWAP_98(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SWAP 98\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SWAP_98(instruction);
        }
        else if (cpu->decode_SWAP_99(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SWAP 99\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SWAP_99(instruction);
        }
        else if (cpu->decode_SRL_100(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SRL 100\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SRL_100(instruction);
        }
        else if (cpu->decode_SRL_101(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SRL 101\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SRL_101(instruction);
        }
        else if (cpu->decode_BIT_102(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "BIT 102\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_BIT_102(instruction);
        }
        else if (cpu->decode_BIT_103(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "BIT 103\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_BIT_103(instruction);
        }
        else if (cpu->decode_RES_104(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RES 104\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RES_104(instruction);
        }
        else if (cpu->decode_RES_105(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RES 105\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RES_105(instruction);
        }
        else if (cpu->decode_SET_106(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SET 106\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SET_106(instruction);
        }
        else if (cpu->decode_SET_107(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "SET 107\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_SET_107(instruction);
        }
        else if (cpu->decode_JP_109(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "JP 109\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_JP_109(instruction);
        }
        else if (cpu->decode_JP_110(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "JP 110\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_JP_110(instruction);
        }
        else if (cpu->decode_JP_111(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "JP 111\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_JP_111(instruction);
        }
        else if (cpu->decode_JR_113(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "JR 113\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_JR_113(instruction);
        }
        else if (cpu->decode_JR_114(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "JR 114\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_JR_114(instruction);
        }
        else if (cpu->decode_CALL_116(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "CALL 116\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_CALL_116(instruction);
        }
        else if (cpu->decode_CALL_117(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "CALL 117\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_CALL_117(instruction);
        }
        else if (cpu->decode_RET_119(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RET 119\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RET_119(instruction);
            // other stuff
        }
        else if (cpu->decode_RET_120(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RET 120\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RET_120(instruction);
            // other stuff
        }
        else if (cpu->decode_RETI_121(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RETI 121\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RETI_121(instruction);
            // other stuff
        }
        else if (cpu->decode_RST_122(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "RST 122\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_RST_122(instruction);
            // other stuff
        }
        else if (cpu->decode_HALT_123(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "HALT 123\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_HALT_123(instruction);
        }
        else if (cpu->decode_STOP_123(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "STOP 123\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_STOP_123(instruction);
        }
        else if (cpu->decode_DI_123(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "DI 123\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_DI_123(instruction);
            // other stuff
        }
        else if (cpu->decode_EI_124(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "EI 124\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_EI_124(instruction);
            // other stuff
        }
        else if (cpu->decode_NOP_125(instruction))
        {
#ifdef ENABLE_INSTR_LOG
            std::cout << "NOP 125\n";
#endif // ENABLE_INSTR_LOG
            cpu->execute_NOP_125(instruction);
        }
        else
        {
            std::cout << "Unknown instruction: " << std::hex << instruction << std::endl;
            keep_window_open = false;
        }

        // std::cout << "HL: " << std::hex << cpu->get_hl() << '\n';
        timer->tick(cpu->get_cycles());

        // screen is updated, reflect that in SDL
        // Call PPU
        bool render = ppu->tick(cpu->get_cycles());
        if (render)
        {
            // Update the window surface with the pixel data
            SDL_LockSurface(window_surface);
            uint32_t *pixels = static_cast<uint32_t *>(window_surface->pixels);
            for (int y = 0; y < WINDOW_HEIGHT; y++)
            {
                for (int x = 0; x < WINDOW_WIDTH; x++)
                {
                    pixels[y * WINDOW_WIDTH + x] = ppu->pixelsToRender[y / SCALE_FACTOR][x / SCALE_FACTOR];
                }
            }
            SDL_UnlockSurface(window_surface);
            SDL_UpdateWindowSurface(window);
        }
    }

    // Destroyer
    // SDL_DestroyTexture(texture);
    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();
}