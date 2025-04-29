#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL.h>
#include "gb.hpp"

int main(int argc, char* argv[]) {
    std::string rom_path;
    if (argc > 1) {
        rom_path = argv[1];
        std::cout << "Using ROM path from command line: " << rom_path << std::endl;
    }

    GheithBoy gb;
    gb.run_gb(rom_path);

    return 0;
}