#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "cpu.hpp"
#include "mmap.hpp"
#include "mmu.hpp"
#include "ppu.hpp"
#include "RAM.hpp"
#include "input.hpp"
#include "InterruptHandler.hpp"

class GheithBoy {
    public:
        void run_gb(const std::string& rom_path);
        GheithBoy();
        ~GheithBoy();
    private:
        bool load_boot(MMAP* mmap);
        bool load_rom(MMAP* mmap, const std::string& rom_path);
        CPU* cpu;
        Input* input;
		MMAP* mmap;
		MMU* mmu;
		PPU* ppu;
		RAM* ram;
		InterruptHandler* IH;
        SDL_Window* window;
        SDL_Surface* window_surface;
        const int WINDOW_WIDTH = 160;
        const int WINDOW_HEIGHT = 144;

        void handle_input(const SDL_Event& event);
};

