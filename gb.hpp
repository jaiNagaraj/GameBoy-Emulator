#pragma once
#include <SDL.h>
#include <string>
#include "cpu.hpp"
#include "mmap.hpp"
#include "mmu.hpp"
#include "ppu.hpp"
#include "RAM.hpp"
#include "input.hpp"
#include "InterruptHandler.hpp"
#include "timer.hpp"

const int TARGET_FPS = 60;
const float TARGET_FRAME_TIME_MS = 1000.0f / TARGET_FPS;
class GheithBoy
{
public:
    void run_gb(const std::string &rom_path);
    GheithBoy();
    ~GheithBoy();

private:
    bool load_boot(MMAP *mmap);
    bool load_rom(MMAP *mmap, const std::string &rom_path);
    CPU *cpu;
    Input *input;
    MMAP *mmap;
    MMU *mmu;
    PPU *ppu;
    RAM *ram;
    InterruptHandler *IH;
    Timer* timer;
    SDL_Window *window;
    SDL_Surface *window_surface;
    const int SCALE_FACTOR = 4;
    const int WINDOW_WIDTH = 160 * SCALE_FACTOR;
    const int WINDOW_HEIGHT = 144 * SCALE_FACTOR;

    void handle_input(const SDL_Event &event);
};
