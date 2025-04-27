#include "InterruptHandler.hpp"

const int VBLANK_FLAG = 0x01;
const int STAT_FLAG = 0x02;
const int TIMER_FLAG = 0x04;
const int SERIAL_FLAG = 0x08;
const int JOYPAD_FLAG = 0x10;

InterruptHandler::InterruptHandler() : mmu(nullptr) {}

void InterruptHandler::connect_mmu(MMU* mmu) {
	this->mmu = mmu;
}

uint8_t InterruptHandler::get_IE() {
	return mmu->read_mem(0xFFFF);
}

uint8_t InterruptHandler::get_IF() {
	return mmu->read_mem(0xFF0F);
}

void InterruptHandler::enable_VBLANK_interrupt() {
	uint8_t current_IF = get_IF();
	current_IF |= VBLANK_FLAG; // Set the VBLANK flag
	mmu->write_mem(0xFF0F, current_IF);
}

void InterruptHandler::disable_VBLANK_interrupt() {
	uint8_t current_IF = get_IF();
	current_IF &= ~(VBLANK_FLAG); // Set the VBLANK flag
	mmu->write_mem(0xFF0F, current_IF);
}

bool InterruptHandler::is_VBLANK_interrupt() {
	return get_IF() & VBLANK_FLAG;
}

void InterruptHandler::enable_STAT_interrupt() {
	uint8_t current_IF = get_IF();
	current_IF |= STAT_FLAG; // Set the STAT flag
	mmu->write_mem(0xFF0F, current_IF);
}

void InterruptHandler::disable_STAT_interrupt() {
	uint8_t currrent_IF = get_IF();
	currrent_IF &= ~(STAT_FLAG); // Set the STAT flag
	mmu->write_mem(0xFF0F, currrent_IF);
}

bool InterruptHandler::is_STAT_interrupt() {
	return get_IF() & STAT_FLAG;
}

void InterruptHandler::enable_TIMER_interrupt() {
	uint8_t current_IF = get_IF();
	current_IF |= TIMER_FLAG; // Set the TIMER flag
	mmu->write_mem(0xFF0F, current_IF);
}

void InterruptHandler::disable_TIMER_interrupt() {
	uint8_t currrent_IF = get_IF();
	currrent_IF &= ~(TIMER_FLAG); // Set the TIMER flag
	mmu->write_mem(0xFF0F, currrent_IF);
}

bool InterruptHandler::is_TIMER_interrupt() {
	return get_IF() & TIMER_FLAG;
}

void InterruptHandler::enable_SERIAL_interrupt() {
	uint8_t current_IF = get_IF();
	current_IF |= SERIAL_FLAG; // Set the SERIAL flag
	mmu->write_mem(0xFF0F, current_IF);
}

void InterruptHandler::disable_SERIAL_interrupt() {
	uint8_t currrent_IF = get_IF();
	currrent_IF &= ~(SERIAL_FLAG); // Set the SERIAL flag
	mmu->write_mem(0xFF0F, currrent_IF);
}

bool InterruptHandler::is_SERIAL_interrupt() {
	return get_IF() & SERIAL_FLAG;
}

void InterruptHandler::enable_JOYPAD_interrupt() {
	uint8_t current_IF = get_IF();
	current_IF |= JOYPAD_FLAG; // Set the JOYPAD flag
	mmu->write_mem(0xFF0F, current_IF);
}

void InterruptHandler::disable_JOYPAD_interrupt() {
	uint8_t currrent_IF = get_IF();
	currrent_IF &= ~(JOYPAD_FLAG); // Set the JOYPAD flag
	mmu->write_mem(0xFF0F, currrent_IF);
}

bool InterruptHandler::is_JOYPAD_interrupt() {
	return get_IF() & JOYPAD_FLAG;
}