#pragma once
#include "mmu.hpp"
#include "RAM.hpp"

class Timer {
private:
	const int DIVIDER_REG = 0xFF04;
	const int TIMA_REG = 0xFF05;
	const int TMA_REG = 0xFF06;
	const int TAC_REG = 0xFF07;
	MMU* mmu;
	RAM* ram;
public:
	Timer();
	void connect_mmu(MMU* mmu);
	void connect_ram(RAM* ram);
	void tick(uint64_t outsideClock);
};