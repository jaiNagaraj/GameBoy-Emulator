#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

class MMAP {
private:
    uint8_t mem[0xFFFF]; // 64KB of memory

public:
    MMAP();

    // Memory operation
    uint8_t read_mem(uint16_t addr);
    void write_mem(uint16_t addr, uint8_t data);
};