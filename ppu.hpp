#pragma once
#include <stdint.h>

// Screen dimensions
const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;

// Forward declaration for the mmu/MMU
class mmu;

class PPU {
public:
    // Constructor - needs access to the mmu for interrupts/DMA
    PPU(mmu& mmu_ref);

    // Main step function, advances PPU state by given CPU cycles
    // Returns true if a frame is complete and ready to be drawn
    bool step(int cpu_cycles);

    // Register access (called by mmu)
    uint8_t read_register(uint16_t addr);
    void write_register(uint16_t addr, uint8_t value);

    // VRAM/OAM access (called by mmu)
    // These might include checks based on current PPU mode
    uint8_t read_vram(uint16_t addr);
    void write_vram(uint16_t addr, uint8_t value);
    uint8_t read_oam(uint16_t addr);
    void write_oam(uint16_t addr, uint8_t value);

    // DMA Transfer (initiated by CPU writing to 0xFF46 via mmu)
    void start_dma(uint8_t start_addr_high_byte);

    // Get access to the framebuffer for rendering by the frontend
    // Returns raw pixel data (e.g., color indices 0-3)
    const uint8_t* get_framebuffer() const;

private:
    mmu& mmu; // Reference to the mmu for memory access (DMA) and interrupt requests

    // --- Memory ---
    uint8_t vram[8192]; // 0x8000 - 0x9FFF (8KB)
    uint8_t oam[160];   // 0xFE00 - 0xFE9F (160 bytes)

    // Framebuffer - stores raw color index (0-3) per pixel
    uint8_t framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

    // --- PPU Registers (Internal state) ---
    uint8_t lcdc; // 0xFF40 LCD Control
    uint8_t stat; // 0xFF41 LCD Status
    uint8_t scy;  // 0xFF42 Scroll Y
    uint8_t scx;  // 0xFF43 Scroll X
    uint8_t ly;   // 0xFF44 LCD Y Coordinate (Current scanline)
    uint8_t lyc;  // 0xFF45 LY Compare
    uint8_t bgp;  // 0xFF47 Background Palette Data
    uint8_t obp0; // 0xFF48 Object Palette 0 Data
    uint8_t obp1; // 0xFF49 Object Palette 1 Data
    uint8_t wy;   // 0xFF4A Window Y Position
    uint8_t wx;   // 0xFF4B Window X Position
    // Note: DMA register 0xFF46 is write-only, handled by start_dma

    // --- Internal State ---
    int scanline_cycles; // Cycles accumulated for the current scanline
    bool frame_complete; // Flag set when a frame is ready

    // DMA state
    bool dma_active;
    int dma_cycles_remaining;
    uint16_t dma_source_addr;
    uint8_t dma_oam_index;


    // --- Helper Methods ---
    void render_scanline();
    void set_ppu_mode(uint8_t mode);
    uint8_t get_ppu_mode() const;
    void check_stat_interrupts();
    void request_interrupt(int bit); // Helper to set IF register bit via mmu

    // Color mapping helper (maps 0-3 index based on palette register)
    uint8_t map_color_index(uint8_t color_index, uint8_t palette_reg) const;

    // Check if CPU access to VRAM/OAM is allowed in current mode
    bool is_vram_accessible() const;
    bool is_oam_accessible() const;

    // DMA cycle handling
    void step_dma();
};

// --- Constants ---

// PPU Modes (Stored in STAT register bits 0-1)
const uint8_t PPU_MODE_HBLANK = 0;
const uint8_t PPU_MODE_VBLANK = 1;
const uint8_t PPU_MODE_OAM_SCAN = 2;
const uint8_t PPU_MODE_DRAWING = 3;

// STAT Register Bits
const int STAT_LYC_EQ_LY_BIT = 2; // Bit 2: Coincidence Flag (Read-only)
const int STAT_MODE0_HBLANK_INT_BIT = 3; // Bit 3: Mode 0 H-Blank Interrupt Enable
const int STAT_MODE1_VBLANK_INT_BIT = 4; // Bit 4: Mode 1 V-Blank Interrupt Enable
const int STAT_MODE2_OAM_INT_BIT = 5;    // Bit 5: Mode 2 OAM Interrupt Enable
const int STAT_LYC_EQ_LY_INT_BIT = 6;    // Bit 6: LYC=LY Coincidence Interrupt Enable

// LCDC Register Bits
const int LCDC_BG_WINDOW_ENABLE_BIT = 0; // DMG: BG Display Enable (0=Off, 1=On)
const int LCDC_SPRITE_ENABLE_BIT = 1;    // OBJ (Sprite) Display Enable (0=Off, 1=On)
const int LCDC_SPRITE_SIZE_BIT = 2;      // OBJ (Sprite) Size (0=8x8, 1=8x16)
const int LCDC_BG_TILE_MAP_AREA_BIT = 3; // BG Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
const int LCDC_TILE_DATA_AREA_BIT = 4;   // BG & Window Tile Data Select (0=8800-97FF, 1=8000-8FFF)
const int LCDC_WINDOW_ENABLE_BIT = 5;    // Window Display Enable (0=Off, 1=On)
const int LCDC_WINDOW_TILE_MAP_AREA_BIT = 6; // Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
const int LCDC_PPU_ENABLE_BIT = 7;       // LCD PPU Enable (0=Off, 1=On)

// Interrupt Request Bits (in IF Register 0xFF0F)
const int VBLANK_INT_BIT = 0;
const int LCD_STAT_INT_BIT = 1;
// Timer = 2, Serial = 3, Joypad = 4

// PPU Timing Constants
const int CYCLES_PER_SCANLINE = 456;
const int SCANLINES_PER_FRAME = 154; // 144 visible + 10 vblank
const int VBLANK_START_LINE = 144;
const int CYCLES_MODE_2_OAM = 80;
const int CYCLES_MODE_3_DRAWING_MIN = 172; // Variable duration
const int CYCLES_MODE_0_HBLANK_MAX = 204; // Variable duration
const int DMA_CYCLES = 160; // OAM DMA takes 160 machine cycles (640 clock cycles?) - Check this! Usually specified in M-cycles. 160 M-cycles = 640 T-cycles.