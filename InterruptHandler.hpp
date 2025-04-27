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
	bool interrupts_enabled();
	void enable_VBLANK_interrupt();
	void disable_VBLANK_interrupt();
	void handle_interrupt(uint8_t interrupt);
	void enable_interrupt(uint8_t interrupt);
	void disable_interrupt(uint8_t interrupt);
	bool is_interrupt_enabled(uint8_t interrupt);
	uint8_t get_IE();
	uint8_t get_IF();
};