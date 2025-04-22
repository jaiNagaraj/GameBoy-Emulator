#pragma once
#include <SDL.h>
#include "cpu.hpp"

class GheithBoy {
    public:
        void run_gb();
        void load_rom();
    private:
        CPU* cpu;
        SDL_Window* window;
        SDL_Surface* window_surface;
        const int WINDOW_WIDTH = 160;
        const int WINDOW_HEIGHT = 144;
};