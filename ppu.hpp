#include <stdint.h>
#include "mmu.hpp"

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

    MMU* mmu;
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
    ~PPU();
    void connect_mmu(MMU* mmu);

    uint64_t** writePixels();
    void update_LCDC();
	void updateRegs();
    void updateBackground();
    void updateWindow();
    void updateSprites();

    uint8_t read_memory(uint16_t addr);
    void write_memory(uint16_t addr, uint8_t data);
};