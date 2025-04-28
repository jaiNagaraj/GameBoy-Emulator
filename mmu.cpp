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
        return ram->read_mem(addr);
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
            case 0xFF00: { // JOYP - Joypad Input Register
                return input->get_joyp_state(mmap->read_mem(addr));
            }

            case 0xFF04: { // DIV - Divider Register (Timer)
                return ram->read_mem(addr);
            }

            case 0xFF05: { // TIMA - Timer Counter
                return ram->read_mem(addr);
            }

            case 0xFF06: { // TMA - Timer Modulo
                return ram->read_mem(addr);
            }

            case 0xFF07: { // TAC - Timer Control
                return ram->read_mem(addr);
            }

            case 0xFF0F: { // IF - Interrupt Flag
                return ram->read_mem(addr) | 0xE0; // Top 3 bits always read as 1
            }

            case 0xFF40: { // LCDC - LCD Control
                return ram->read_mem(addr);
            }
            
            case 0xFF41: { // STAT - LCD Status
                return ram->read_mem(addr);
            }

            case 0xFF42: { // SCY - Scroll Y
                return ram->read_mem(addr);
            }

            case 0xFF43: { // SCX - Scroll X
                return ram->read_mem(addr);
            }

            case 0xFF44: { // LY - LCD Y-Coordinate (Read-Only, updated by PPU)
                return ram->read_mem(addr);
            }

            case 0xFF45: { // LYC - LY Compare
                return ram->read_mem(addr);
            }

            case 0xFF46: { // DMA - DMA Transfer Start Address
                return ram->read_mem(addr);
            }

            case 0xFF47: { // BGP - Background Palette Data
                return ram->read_mem(addr);
            }

            case 0xFF48: { // OBP0 - Object Palette 0 Data
                return ram->read_mem(addr);
            }

            case 0xFF49: { // OBP1 - Object Palette 1 Data
                return ram->read_mem(addr);
            }

            case 0xFF4A: { // WY - Window Y Position
                return ram->read_mem(addr);
            }
            
            case 0xFF4B: { // WX - Window X Position minus 7
                return ram->read_mem(addr);
            }

            default: {
                return ram->read_mem(addr);
            }
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
    // ROM Bank 0 : 0x0000 - 0x3FFF
    if (addr <= 0x3FFF) {
        return;
    }

    // Switchable ROM : 0x4000 - 0x7FFF
    if (addr >= 0x4000 && addr <= 0x7FFF) {
        return; // Tetris does not use MBC
    }

    // VRAM : 0x8000 - 0x9FFF
    if (addr >= 0x8000 && addr <= 0x9FFF) {
        ram->write_mem(addr, data);
        return;
        uint8_t stat = ram->read_mem(0xFF41);

        if ((stat & 0b00000011) == 0b00000010 || (stat & 0b00000011) == 0b00000011) { // Mode 2 or Mode 3
            return;
        } else {
            ram->write_mem(addr, data);
            return;
        }
    }

    // External RAM : 0xA000 - 0xBFFF
    if (addr >= 0xA000 && addr <= 0xBFFF) {
        return; // Tetris cartridge does not contain external RAM chip
    }

    // WRAM : 0xC000 - 0xDFFF
    if (addr >= 0xC000 && addr <= 0xDFFF) {
        ram->write_mem(addr, data);
        return;
    }

    // Echo RAM (unusable) : 0xE000 - 0xFDFF
    if (addr >= 0xE000 && addr <= 0xFDFF) {
        ram->write_mem(addr - 0x2000, data);
        return;
    }

    // OAM : 0xFE00 - 0xFE9F
    if (addr >= 0xFE00 && addr <= 0xFE9F) {
        uint8_t stat = ram->read_mem(0xFF41);

        if ((stat & 0b00000011) == 0b00000011) { // Mode 3
            return;
        } else {
            ram->write_mem(addr, data);
            return;
        }
    }

    // Unusable : 0xFEA0 - 0xFEFF
    if (addr >= 0xFEA0 && addr <= 0xFEFF) {
        return;
    }

    // I/O Registers : 0xFF00 - 0xFF7F
    if (addr >= 0xFF00 && addr <= 0xFF7F) {
        switch (addr) {
            case 0xFF00: { // JOYP - Joypad Input Register
                ram->write_mem(addr, (mmap->read_mem(addr) & 0xCF) | (data & 0x30));
                return;
            }

            case 0xFF04: { // DIV - Divider Register (Timer)
                ram->write_mem(addr, 0); // Any write resets DIV to 0
                return;
            }

            case 0xFF05: { // TIMA - Timer Counter
                ram->write_mem(addr, data);
                return;
            }

            case 0xFF06: { // TMA - Timer Modulo
                ram->write_mem(addr, data);
                return;
            }

            case 0xFF07: { // TAC - Timer Control
                ram->write_mem(addr, data);
                return;
            }

            case 0xFF0F: { // IF - Interrupt Flag
                ram->write_mem(addr, data); // Restriction on top 3 bits read-enforced
                return;
            }

            case 0xFF40: { // LCDC - LCD Control
                ram->write_mem(addr, data);
                return;
            }
            
            case 0xFF41: { // STAT - LCD Status
                ram->write_mem(addr, (ram->read_mem(addr) & 0x87) | (data & 0x78)); // Combine R/O and W bits
                return;
            }

            case 0xFF42: { // SCY - Scroll Y
                ram->write_mem(addr, data);
                return;
            }

            case 0xFF43: { // SCX - Scroll X
                ram->write_mem(addr, data);
                return;
            }

            case 0xFF44: { // LY - LCD Y-Coordinate (Read-Only, updated by PPU)
                return;
            }

            case 0xFF45: { // LYC - LY Compare
                ram->write_mem(addr, data);
                return;
            }

            case 0xFF46: { // DMA - DMA Transfer Start Address
                ram->write_mem(addr, data);
                dma_transfer(static_cast<uint16_t>(data) << 8);
                return;
            }

            case 0xFF47: { // BGP - Background Palette Data
                ram->write_mem(addr, data);    
                return;
            }

            case 0xFF48: { // OBP0 - Object Palette 0 Data
                ram->write_mem(addr, data);    
                return;
            }

            case 0xFF49: { // OBP1 - Object Palette 1 Data
                ram->write_mem(addr, data);
                return;
            }

            case 0xFF4A: { // WY - Window Y Position
                uint8_t stat = ram->read_mem(0xFF41);

                if ((stat & 0b00000011) == 0b00000011) { // Mode 3
                    return;
                } else {
                    ram->write_mem(addr, data);
                    return;
                }
            }
            
            case 0xFF4B: { // WX - Window X Position minus 7
                uint8_t stat = ram->read_mem(0xFF41);

                if ((stat & 0b00000011) == 0b00000011) { // Mode 3
                    return;
                } else {
                    ram->write_mem(addr, data);
                    return;
                }
            }

            default: {
                ram->write_mem(addr, data);
                return;
            }
        }
    }

    // HRAM : 0xFF80 - 0xFFFE
    if (addr >= 0xFF80 && addr <= 0xFFFE) {
        ram->write_mem(addr, data);
        return;
    }

    // IE Register : 0xFFFF
    if (addr == 0xFFFF) {
        ram->write_mem(addr, data);
        return;
    }

    // Invalid address range
    std::cerr << "MMU Warning: Write to invalid address: 0x" << std::hex << addr << std::endl;
    return;
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

void MMU::dma_transfer(uint16_t addr) {
    std::cout << "DMA TRANSFER!\n";
    std::cout << "Loading from address " << std::hex << (int)addr << '\n';
    for (int i = 0; i < 160; i++) {
        ram->write_mem(0xFE00 + i, ram->read_mem(addr + i)); // 0xFE00 = OAM_START
    }
}