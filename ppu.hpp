#pragma once
#include <stdint.h>
#include "mmu.hpp"

enum COLOR
{
    WHITE_OR_TRANSPARENT = 0,
    LIGHT_GRAY = 1,
    DARK_GRAY = 2,
    BLACK = 3,
    WINDOW_TRANSPARENT = 4,
};

class PPU
{
private:
    const static int SCREEN_WIDTH = 160;
    const static int SCREEN_HEIGHT = 144;

    uint8_t LCDC_reg;
    uint8_t SCX_reg;
    uint8_t SCY_reg;
    uint8_t WX_reg;
    uint8_t WY_reg;
    uint8_t OBP0_reg;
    uint8_t OBP1_reg;
    uint8_t BGP_reg;
    MMU *mmu;

    COLOR pixelData[SCREEN_HEIGHT][SCREEN_WIDTH];
    COLOR backgroundData[SCREEN_HEIGHT][SCREEN_WIDTH];
    COLOR windowData[SCREEN_HEIGHT][SCREEN_WIDTH];
    COLOR spriteData[SCREEN_HEIGHT][SCREEN_WIDTH];

public:
    uint64_t clock;

    PPU();
    ~PPU();
    void connect_mmu(MMU *mmu);

    uint32_t **writePixels();
    void updateRegs();
    void updateBackground(uint8_t row);
    void updateWindow(uint8_t row);
    void updateSprites(uint8_t row);

    uint8_t read_mem(uint16_t addr);
    void write_mem(uint16_t addr, uint8_t data);
};