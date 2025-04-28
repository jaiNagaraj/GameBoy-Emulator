#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// TODO: change to class and move function definitions/varible initialization to cpp file
struct MBC1 {
    bool ram_enable = false;
    uint8_t rom_bank_low5 = 1;
    uint8_t ram_bank_or_rom_bank_high2 = 0;
    bool banking_mode = false; // false = ROM banking mode; true = RAM banking mode

    // Select correct ROM bank
    uint8_t select_rom_bank() const {
        uint8_t bank = (ram_bank_or_rom_bank_high2 << 5) | (rom_bank_low5 & 0x1F);
        if ((bank & 0x1F) == 0) bank |= 0x01; // Cannot select bank 0 for 4000-7FFF
        return bank;
    }

    // Select correct RAM bank
    uint8_t select_ram_bank() const {
        if (!banking_mode) return 0; // In ROM mode, RAM bank is 0
        return ram_bank_or_rom_bank_high2 & 0x03;
    }
};

// MMU Header
class MMU {
    public:
        MBC1 mbc1;
        std::vector<uint8_t> rom;    // Full ROM data
        std::vector<uint8_t> ram;    // External cartridge RAM (if present)
    
        bool has_mbc1 = false;       // Set true if cartridge header says MBC1
    
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value);
    
        // Helper for initialization
        void load_cartridge(const std::vector<uint8_t>& rom_data);
    };

// MMU Body
void MMU::load_cartridge(const std::vector<uint8_t>& rom_data) {
    rom = rom_data;

    // Detect if cartridge is MBC1
    uint8_t cartridge_type = rom[0x0147];
    if (cartridge_type == 0x01 || cartridge_type == 0x02 || cartridge_type == 0x03) {
        has_mbc1 = true;
    }

    // Allocate cartridge RAM if needed
    ram.resize(32 * 1024); // 32 KB max external RAM for MBC1
}

uint8_t MMU::read(uint16_t address) {
    if (address <= 0x3FFF) {
        // Fixed ROM bank 0
        return rom[address];
    } else if (address >= 0x4000 && address <= 0x7FFF) {
        if (has_mbc1) {
            uint32_t bank = mbc1.select_rom_bank();
            uint32_t addr = (bank * 0x4000) + (address - 0x4000);
            if (addr < rom.size()) return rom[addr];
            else return 0xFF; // Out of ROM bounds
        } else {
            return rom[address];
        }
    } else if (address >= 0xA000 && address <= 0xBFFF) {
        if (has_mbc1 && mbc1.ram_enable) {
            uint32_t ram_bank = mbc1.select_ram_bank();
            uint32_t addr = (ram_bank * 0x2000) + (address - 0xA000);
            if (addr < ram.size()) return ram[addr];
            else return 0xFF;
        } else {
            return 0xFF; // Reading RAM disabled
        }
    }

    // Else: normal system memory, IO, etc
    return system_memory[address];
}

void MMU::write(uint16_t address, uint8_t value) {
    if (address <= 0x1FFF) {
        // RAM Enable
        if (has_mbc1) {
            mbc1.ram_enable = ((value & 0x0F) == 0x0A);
        }
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        // ROM bank number (lower 5 bits)
        if (has_mbc1) {
            mbc1.rom_bank_low5 = value & 0x1F;
            if ((mbc1.rom_bank_low5 & 0x1F) == 0) mbc1.rom_bank_low5 |= 0x01;
        }
    } else if (address >= 0x4000 && address <= 0x5FFF) {
        // RAM bank number or high ROM bank bits
        if (has_mbc1) {
            mbc1.ram_bank_or_rom_bank_high2 = value & 0x03;
        }
    } else if (address >= 0x6000 && address <= 0x7FFF) {
        // Banking mode select
        if (has_mbc1) {
            mbc1.banking_mode = (value & 0x01);
        }
    } else if (address >= 0xA000 && address <= 0xBFFF) {
        // External RAM write
        if (has_mbc1 && mbc1.ram_enable) {
            uint32_t ram_bank = mbc1.select_ram_bank();
            uint32_t addr = (ram_bank * 0x2000) + (address - 0xA000);
            if (addr < ram.size()) ram[addr] = value;
        }
    } else {
        // Else: normal system memory, IO, etc
        system_memory[address] = value;
    }
}
