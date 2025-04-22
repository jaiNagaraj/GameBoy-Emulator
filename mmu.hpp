#pragma once
#include <stdint.h>

// Forward declarations of components the MMU interacts with
class CPU;
class PPU;
class RAM;
class Cartridge;
class Timer;
class Joypad;

class MMU {
public:
    MMU();

    // Connection methods (use if using Option 2 constructor)
    void connect_cpu(CPU* cpu_ptr);
    void connect_ppu(PPU* ppu_ptr);
    void connect_wram(RAM* ram_ptr);
    void connect_hram(RAM* ram_ptr);
    void connect_cartridge(Cartridge* cart_ptr);
    void connect_timer(Timer* timer_ptr);
    void connect_joypad(Joypad* joypad_ptr);

    // --- Core Memory Access ---
    // Called by CPU to read from the memory map
    uint8_t read(uint16_t addr);
    // Called by CPU to write to the memory map
    void write(uint16_t addr, uint8_t value);

    // --- Interrupt Handling ---
    // Called by components (PPU, Timer, Joypad) to request an interrupt
    void request_interrupt(int bit);
    // Called by CPU during interrupt check
    uint8_t get_interrupt_enable() const;
    uint8_t get_interrupt_flags() const;
    void set_interrupt_enable(uint8_t value);
    void set_interrupt_flags(uint8_t value); // Usually only bits are set via request_interrupt

private:
    // Pointers to connected components
    CPU*       cpu = nullptr;
    PPU*       ppu = nullptr;
    RAM*       wram = nullptr; // Work RAM (C000-DFFF)
    RAM*       hram = nullptr; // High RAM (FF80-FFFE) - Could also be a small internal array
    Cartridge* cartridge = nullptr;
    Timer*     timer = nullptr;
    Joypad*    joypad = nullptr;

    // Interrupt Registers state
    uint8_t interrupt_enable_register = 0x00; // IE (0xFFFF)
    uint8_t interrupt_flag_register = 0x00;   // IF (0xFF0F)

    // Internal representation for HRAM if not using a separate RAM instance
    // uint8_t hram_internal[127]; // Size 0xFFFE - 0xFF80 + 1 = 127 bytes
};

// Interrupt Bit definitions (matching IF/IE registers)
const int VBLANK_INT_BIT = 0;
const int LCD_STAT_INT_BIT = 1;
const int TIMER_INT_BIT = 2;
const int SERIAL_INT_BIT = 3;
const int JOYPAD_INT_BIT = 4;