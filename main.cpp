#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL.h>
#include "gb.hpp"

int main() {
	std::string rom_path = "cpu_instrs.gb";
	GheithBoy gb;
	gb.run_gb(rom_path);
}