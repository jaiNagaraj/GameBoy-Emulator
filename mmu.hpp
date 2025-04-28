#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

//#include "cpu.hpp"
//#include "ppu.hpp"
#include "RAM.hpp"
#include "mmap.hpp"
#include "input.hpp"

class MMU {
private:
    RAM *ram;
    MMAP *mmap;
    Input *input;

public:
    MMU();
    bool dma_transfer;

    void connect_ram(RAM *ram);
    void connect_mmap(MMAP *mmap);
    void connect_input(Input *input);

    uint8_t read_mem(uint16_t addr);
    void write_mem(uint16_t addr, uint8_t data);

    void push_stack(uint16_t sp, uint16_t data);
    uint16_t pop_stack(uint16_t sp);

    MMAP* get_mmap();
};