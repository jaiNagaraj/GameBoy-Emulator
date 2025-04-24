#include <stdint.h>
#include "RAM.hpp"

class PPU {
    const static int SCREEN_WIDTH = 160;
    const static int SCREEN_HEIGHT = 144;

    enum COLOR {
        TRANSPARENT = 0,
        WHITE = 1,
        LIGHT_GRAY = 2,
        DARK_GRAY = 3,
    };

    RAM* ram;
    uint8_t LCDC_reg;
    uint8_t SCX_reg;
    uint8_t SCY_reg;
    uint8_t WX_reg;
    uint8_t WY_reg;
    uint8_t OBP0_reg;
    uint8_t OBP1_reg;

    COLOR pixelData[SCREEN_HEIGHT][SCREEN_WIDTH];
	COLOR backgroundData[SCREEN_HEIGHT][SCREEN_WIDTH];
	COLOR windowData[SCREEN_HEIGHT][SCREEN_WIDTH];
	COLOR spriteData[SCREEN_HEIGHT][SCREEN_WIDTH];

    PPU();
    PPU(RAM* ram);
    ~PPU();

    uint64_t** writePixels();
	void updateRegs();
    void updateBackground();
    void updateWindow();
    void updateSprites();
};