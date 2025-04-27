#include "mmu.hpp"

MMU::MMU() {

}

void MMU::connect_ram(RAM *ram) {
    this->ram = ram;
}

void MMU::connect_mmap(MMAP *mmap) {
    this->mmap = mmap;
}

void MMU::connect_input(Input *input) {
    this->input = input; 
}

uint8_t MMU::read_mem(uint16_t addr) {
    // ROM Bank 0 : 0x0000 - 0x3FFF
    if (addr <= 0x3FFF) {
        return ram->read_mem(addr);
    }

    // Switchable ROM : 0x4000 - 0x7FFF
    if (addr >= 0x4000 && addr <= 0x7FFF) {
        return ram->read_mem(addr);
    }

    // VRAM : 0x8000 - 0x9FFF
    if (addr >= 0x8000 && addr <= 0x9FFF) {
        uint8_t stat = ram->read_mem(0xFF41);

        if ((stat & 0b00000011) == 0b00000011) { // Mode 3
            return 0xFF;
        } else {
            return ram->read_mem(addr);
        }
    }

    // External RAM : 0xA000 - 0xBFFF
    if (addr >= 0xA000 && addr <= 0xBFFF) {
        return 0xFF; // Tetris cartridge does not contain external RAM chip
    }

    // WRAM : 0xC000 - 0xDFFF
    if (addr >= 0xC000 && addr <= 0xDFFF) {
        return ram->read_mem(addr);
    }

    // Echo RAM (unusable) : 0xE000 - 0xFDFF
    if (addr >= 0xE000 && addr <= 0xFDFF) {
        return ram->read_mem(addr - 0x2000);
    }

    // OAM : 0xFE00 - 0xFE9F
    if (addr >= 0xFE00 && addr <= 0xFE9F) {
        uint8_t stat = ram->read_mem(0xFF41);

        if ((stat & 0b00000011) == 0b00000010 || (stat & 0b00000011) == 0b00000011) { // Mode 2 or Mode 3
            return 0xFF;
        } else {
            return ram->read_mem(addr);
        }
    }

    // Unusable : 0xFEA0 - 0xFEFF
    if (addr >= 0xFEA0 && addr <= 0xFEFF) {
        return 0xFF;
    }

    // I/O Registers : 0xFF00 - 0xFF7F
    if (addr >= 0xFF00 && addr <= 0xFF7F) {
        switch (addr) {
            case 0xFF00: // JOYP - Joypad Input Register
                // TODO: Interrupt handling?
                return input->get_joyp_state(mmap->read_mem(addr));

            case 0xFF04: // DIV - Divider Register (Timer)
                return mmap->read_mem(addr);

            case 0xFF05: // TIMA - Timer Counter
                return mmap->read_mem(addr);

            case 0xFF06: // TMA - Timer Modulo
                return mmap->read_mem(addr);

            case 0xFF07: // TAC - Timer Control
                return mmap->read_mem(addr);

            case 0xFF0F: // IF - Interrupt Flag
                return mmap->read_mem(addr) | 0xE0; // Top 3 bits always read as 1

            case 0xFF40: // LCDC - LCD Control
                return mmap->read_mem(addr);
            
            case 0xFF41: // STAT - LCD Status
                return mmap->read_mem(addr);

            case 0xFF42: // SCY - Scroll Y
                return mmap->read_mem(addr);

            case 0xFF43: // SCX - Scroll X
                return mmap->read_mem(addr);

            case 0xFF44: // LY - LCD Y-Coordinate (Read-Only, updated by PPU)
                return mmap->read_mem(addr);

            case 0xFF45: // LYC - LY Compare
                return mmap->read_mem(addr);

            case 0xFF46: // DMA - DMA Transfer Start Address
                return mmap->read_mem(addr);

            case 0xFF47: // BGP - Background Palette Data
                return mmap->read_mem(addr);

            case 0xFF48: // OBP0 - Object Palette 0 Data
                return mmap->read_mem(addr);

            case 0xFF49: // OBP1 - Object Palette 1 Data
                return mmap->read_mem(addr);

            case 0xFF4A: // WY - Window Y Position
                return mmap->read_mem(addr);
            
            case 0xFF4B: // WX - Window X Position minus 7
                return mmap->read_mem(addr);

            default:
                return mmap->read_mem(addr);
                break;
        }
    }

    // HRAM : 0xFF80 - 0xFFFE
    if (addr >= 0xFF80 && addr <= 0xFFFE) {
        return ram->read_mem(addr);
    }

    // IE Register : 0xFFFF
    if (addr == 0xFFFF) {
        return ram->read_mem(addr);
    }

    // Invalid address range
    std::cerr << "MMU Warning: Read from invalid address: 0x" << std::hex << addr << std::endl;
    return 0xFF;
}

void MMU::write_mem(uint16_t addr, uint8_t data) {
    // --- VRAM Write ---
     if (addr >= 0x8000 && addr <= 0x9FFF) {
        // TODO: Add PPU Mode 3 check here later
        // if (ppu && ppu->get_current_mode() == PPU::DRAWING) return;
        if (!mmap) throw std::runtime_error("MMU Error: MMAP not connected for VRAM write!");
        mmap->write_mem(addr, data);
        return;
    }
    // --- OAM Write ---
    else if (addr >= 0xFE00 && addr <= 0xFE9F) {
        // TODO: Add PPU Mode 2 & 3 check here later
        // if (ppu && (ppu->get_current_mode() == PPU::OAM_SCAN || ppu->get_current_mode() == PPU::DRAWING)) return;
        if (!mmap) throw std::runtime_error("MMU Error: MMAP not connected for OAM write!");
        mmap->write_mem(addr, data);
        return;
    }
    // --- WRAM / HRAM ---
    else if ((addr >= 0xC000 && addr <= 0xDFFF) || (addr >= 0xFF80 && addr <= 0xFFFE)) {
         if (!ram) throw std::runtime_error("MMU Error: RAM not connected for WRAM/HRAM write!");
        ram->write_mem(addr, data);
    }
    // --- Echo RAM ---
    else if (addr >= 0xE000 && addr <= 0xFDFF) {
         if (!ram) throw std::runtime_error("MMU Error: RAM not connected for Echo RAM write!");
        ram->write_mem(addr, data);
        ram->write_mem(addr - 0x2000, data);
    }
    // --- I/O Registers ---
    else if (addr >= 0xFF00 && addr <= 0xFF7F) {
        if (!mmap) throw std::runtime_error("MMU Error: MMAP not connected for I/O write!");

        switch (addr) {
            case 0xFF00: // JOYP - Joypad Input Register
                // Only write bits (4 and 5)
                mmap->write_mem(addr, (mmap->read_mem(addr) & 0xCF) | (data & 0x30));
                break;

            case 0xFF01: // SB - Serial Transfer Data
            case 0xFF02: // SC - Serial Transfer Control
                // TODO: Handle Serial communication logic
                mmap->write_mem(addr, data);
                break;

            case 0xFF04: // DIV - Divider Register (Timer)
                // Any write resets DIV to 0
                // std::cout << "MMU Write: DIV (0xFF04) - Resetting to 0" << std::endl;
                // TODO: Notify Timer component if it tracks DIV internally
                mmap->write_mem(addr, 0);
                break;
            case 0xFF05: // TIMA - Timer Counter
            case 0xFF06: // TMA - Timer Modulo
            case 0xFF07: // TAC - Timer Control
                // TODO: Notify Timer component of the write
                // std::cout << "MMU Write: Timer Register (0x" << std::hex << addr << ") = 0x" << (int)data << " - Needs Timer Component" << std::endl;
                mmap->write_mem(addr, data);
                break;

            case 0xFF0F: // IF - Interrupt Flag
                // Only bottom 5 bits are writable
                mmap->write_mem(addr, data & 0x1F);
                break;

            // --- PPU Registers ---
            case 0xFF40: // LCDC - LCD Control
                // TODO: Notify PPU of the change
                // if (ppu) ppu->write_lcdc(data); else mmap->write_mem(addr, data);
                // std::cout << "MMU Write: LCDC (0xFF40) = 0x" << std::hex << (int)data << " - Needs PPU Notification" << std::endl;
                mmap->write_mem(addr, data);
                break;
            case 0xFF41: // STAT - LCD Status
                // Bits 0, 1, 2 are read-only (Mode, LYC flag)
                // Bits 3, 4, 5, 6 are writable (Interrupt enables)
                // TODO: Notify PPU of the change to writable bits
                // if (ppu) ppu->write_stat(data); else mmap->write_mem(addr, (mmap->read_mem(addr) & 0x07) | (data & 0x78)); // Combine R/O and W bits
                 // std::cout << "MMU Write: STAT (0xFF41) = 0x" << std::hex << (int)data << " - Needs PPU Notification/Handling" << std::endl;
                 mmap->write_mem(addr, (mmap->read_mem(addr) & 0x07) | (data & 0x78)); // Basic masking
                break;
            case 0xFF42: // SCY - Scroll Y
            case 0xFF43: // SCX - Scroll X
            case 0xFF45: // LYC - LY Compare
            case 0xFF47: // BGP - Background Palette Data
            case 0xFF48: // OBP0 - Object Palette 0 Data
            case 0xFF49: // OBP1 - Object Palette 1 Data
            case 0xFF4A: // WY - Window Y Position
            case 0xFF4B: // WX - Window X Position minus 7
                mmap->write_mem(addr, data);
                break;
            case 0xFF44: // LY - LCD Y-Coordinate (Read-Only)
                // Writes are ignored
                break;
            // --- End PPU Registers ---

            case 0xFF46: // DMA - DMA Transfer Start Address
                // std::cout << "MMU Write: DMA (0xFF46) = 0x" << std::hex << (int)data << " - Triggering DMA Transfer" << std::endl;
                mmap->write_mem(addr, data); // Store the source address base
                // start_dma_transfer(data); // Call function to handle the transfer
                break;

            // TODO: Add cases for Sound Registers (0xFF10 - 0xFF26) - Write directly for now
            // DNE
            // TODO: Add cases for Waveform RAM (0xFF30 - 0xFF3F) - Write directly for now

            default:
                // For unhandled or simple I/O registers, write directly to MMAP
                // Check documentation - some unused registers ignore writes
                 if ((addr >= 0xFF10 && addr <= 0xFF26) || // Sound
                    (addr >= 0xFF30 && addr <= 0xFF3F)) { // Waveform RAM
                    mmap->write_mem(addr, data);
                    break;
                }
                if (addr >= 0xFF4C && addr <= 0xFF7F) { // Ignore writes to unused range
                    break;
                }
                // Default for unknown/simple registers in the range
                mmap->write_mem(addr, data);
        }
    }
    // --- ROM ---
    else if (addr <= 0x7FFF) {
        // TODO: Handle MBC writes
        return;
    }
    // --- External RAM / IE ---
    else if ((addr >= 0xA000 && addr <= 0xBFFF) || (addr == 0xFFFF)) {
        if (!mmap) throw std::runtime_error("MMU Error: MMAP not connected for direct write!");
        mmap->write_mem(addr, data);
    }
    // --- Unusable Memory ---
    else if (addr >= 0xFEA0 && addr <= 0xFEFF) {
        return;
    }
    // --- Should not happen ---
    else {
         std::cerr << "MMU Warning: Write to unhandled address: 0x" << std::hex << addr << std::endl;
    }
}

void MMU::push_stack(uint16_t sp, uint16_t data) {
    if (!ram) throw std::runtime_error("MMU Error: RAM not connected for push_stack!");

    ram->push_stack(sp, data);
}

uint16_t MMU::pop_stack(uint16_t sp) {
    if (!ram) throw std::runtime_error("MMU Error: RAM not connected for pop_stack!");
    
    return ram->pop_stack(sp);
}

MMAP* MMU::get_mmap() {
    // Add null check for safety, although it should be connected
    if (!mmap) throw std::runtime_error("MMU Error: get_mmap called but MMAP is not connected!");

    return mmap;
}