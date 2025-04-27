#pragma once
#include <stdint.h>
#include "mmu.hpp"
#include "InterruptHandler.hpp"
#include <list>

enum COLOR
{
    WHITE_OR_TRANSPARENT = 0,
    LIGHT_GRAY = 1,
    DARK_GRAY = 2,
    BLACK = 3,
    WINDOW_TRANSPARENT = 4,
};

class Sprite
{
public:
    uint8_t y;
    uint8_t x;
    uint8_t tileIndex;
    uint8_t flags;

    Sprite(uint8_t y, uint8_t x, uint8_t tileIndex, uint8_t flags)
        : y(y), x(x), tileIndex(tileIndex), flags(flags) {}
    bool comp_sprite(const Sprite& sp1, const Sprite& sp2) const
    {
        return sp1.x < sp2.x;
    }
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
    RAM *ram;
    InterruptHandler *IH;

    int mode;
    uint8_t scanLine;
    uint64_t clock;

    COLOR pixelData[SCREEN_HEIGHT][SCREEN_WIDTH];
    COLOR backgroundData[SCREEN_HEIGHT][SCREEN_WIDTH];
    COLOR windowData[SCREEN_HEIGHT][SCREEN_WIDTH];
    COLOR spriteData[SCREEN_HEIGHT][SCREEN_WIDTH];

    std::list<Sprite> spriteBuffer;

public:
    uint32_t pixelsToRender[SCREEN_HEIGHT][SCREEN_WIDTH];

    PPU();
    ~PPU();
    void connect_mmu(MMU *mmu);
    void connect_ram(RAM *ram);
    void connect_interrupt_handler(InterruptHandler *IH);

    bool tick(uint64_t outsideClock);
    void update_LY();
    void update_LCDSTAT();
    void updatePixelData(uint8_t row);
    void updateRegs();
    void updateBackground(uint8_t row);
    void updateWindow(uint8_t row);
    void updateSprites(uint8_t row);
    void scanOAM(uint8_t row);

    uint8_t read_mem(uint16_t addr);
    void write_mem(uint16_t addr, uint8_t data);
};