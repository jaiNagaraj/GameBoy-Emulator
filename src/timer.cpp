#include "../include/timer.hpp"

Timer::Timer() {
	this->mmu = nullptr;
}

void Timer::connect_mmu(MMU* mmu) {
	this->mmu = mmu;
}

void Timer::connect_ram(RAM* ram) {
	this->ram = ram;
}

void Timer::tick(uint64_t outsideClock) {
	outsideClock *= 4;
	// Update the DIV register every 256 cycles
	if (outsideClock % 256 == 0) {
		uint8_t current_div = ram->read_mem(DIVIDER_REG);
		current_div++;
		ram->write_mem(DIVIDER_REG, current_div);
	}	
}