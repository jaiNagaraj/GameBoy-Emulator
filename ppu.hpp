#include <stdint.h>
#include "RAM.hpp"

class PPU {
    class Tile {
        // This is an 8x8 pixel grid with four colors.
		// The colors are represented as 2 bits per pixel.
        ;
    };

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

    COLOR pixelData[160][144];
	COLOR backgroundData[160][144];
	COLOR windowData[160][144];
	COLOR spriteData[160][144];

    PPU();
    PPU(RAM* ram);
    ~PPU();

    uint64_t** writePixels();
	void updateRegs();
    void updateBackground();
    void updateWindow();
    void updateSprites();
};