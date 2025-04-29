#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL.h>
#include "../include/gb.hpp"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <rom_file>\n";
		std::cerr << "Please place the ROM file in the /games/ directory, and just type the game file name (not path).\n";
		return 1;
	}
	std::string rom_path (argv[1]);
	rom_path = "./games/" + rom_path;
	GheithBoy gb;
	gb.run_gb(rom_path);
}