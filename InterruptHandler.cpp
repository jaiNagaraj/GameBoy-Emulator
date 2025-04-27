#include "InterruptHandler.hpp"

InterruptHandler::InterruptHandler() : mmu(nullptr) {}

void InterruptHandler::connect_mmu(MMU* mmu) {
	this->mmu = mmu;
}

bool InterruptHandler::interrupts_enabled() {
	return interrupts_enabled;
}

uint8_t InterruptHandler::get_IE() {
	return mmu->read_mem(0xFFFF);
}

uint8_t InterruptHandler::get_IF() {
	return mmu->read_mem(0xFF0F);
}

void InterruptHandler::enable_VBLANK_interrupt() {
	uint8_t currrent_IF = get_IF();
	currrent_IF |= 0x01; // Set lower bit - the VBLANK flag
	mmu->write_mem(0xFF0F, currrent_IF);
}

void InterruptHandler::disable_VBLANK_interrupt() {
	uint8_t currrent_IF = get_IF();
	currrent_IF &= 0xFFFFFFF0; // Set lower bit - the VBLANK flag
	mmu->write_mem(0xFF0F, currrent_IF);
}