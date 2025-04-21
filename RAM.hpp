/**
 * Header file for RAM.hpp - declare all memory operations in the RAM class.
 * It also contains the structures within the RAM (ex. memory, etc)
 */

#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

class RAM {
private:
    uint8_t mem[0xFFFF]; // 64KB of RAM

public:
    // Constructor
    RAM();

    // Memory operations
    uint8_t read_mem(uint16_t addr);
    void write_mem(uint16_t addr, uint8_t data);
};
