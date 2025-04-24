/**
 * Header file for RAM.hpp - declare all memory operations in the RAM class.
 * It also contains the structures within the RAM (ex. memory, etc)
 */

#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdexcept>
#include <iostream> 

#include "mmap.hpp"

class RAM {
private:
    MMAP *mmap;

public:
    // Constructor
    RAM();

    void connect_mmap(MMAP *mmap);

    // Memory operation
    uint8_t read_mem(uint16_t addr);
    void write_mem(uint16_t addr, uint8_t data);

    // Stack operations
    uint16_t push_stack(uint16_t sp, uint16_t data);
    uint16_t pop_stack(uint16_t sp);
};
