#pragma once
#include <stdint.h>
#include "mmu.hpp"

class InterruptHandler {
private:
	MMU* mmu;
	bool interrupts_enabled;
public:
	InterruptHandler();
	~InterruptHandler();
	void connect_mmu(MMU* mmu);
	void enable_VBLANK_interrupt();
	void disable_VBLANK_interrupt();
	bool is_VBLANK_interrupt();
	void enable_STAT_interrupt();
	void disable_STAT_interrupt();
	bool is_STAT_interrupt();
	void enable_TIMER_interrupt();
	void disable_TIMER_interrupt();
	bool is_TIMER_interrupt();
	void enable_SERIAL_interrupt();
	void disable_SERIAL_interrupt();
	bool is_SERIAL_interrupt();
	void enable_JOYPAD_interrupt();
	void disable_JOYPAD_interrupt();
	bool is_JOYPAD_interrupt();
	uint8_t get_IE();
	uint8_t get_IF();
};