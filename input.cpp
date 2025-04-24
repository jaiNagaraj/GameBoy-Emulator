#include "input.hpp"

void Input::set_button_state(uint8_t button_index, bool pressed) {
    // This needs mapping from SDL keycodes to button indices
    // Example mapping:
    // 0: Right, 1: Left, 2: Up, 3: Down
    // 4: A, 5: B, 6: Select, 7: Start
    switch (button_index) {
        case 0: right = pressed; break;
        case 1: left = pressed; break;
        case 2: up = pressed; break;
        case 3: down = pressed; break;
        case 4: a = pressed; break;
        case 5: b = pressed; break;
        case 6: select = pressed; break;
        case 7: start = pressed; break;
    }
}

uint8_t Input::get_joyp_state(uint8_t mmap_joyp_value) const {
    // mmap_joyp_value contains the selection bits (4 & 5) written by the game
    uint8_t joyp_select = mmap_joyp_value & 0x30; // Isolate bits 4 and 5

    // Start with bits 7-6 high, bits 5-4 from selection, bits 3-0 high (unpressed)
    uint8_t result = joyp_select | 0xC0 | 0x0F;

    // Check if Direction buttons are selected (bit 4 == 0)
    if ((joyp_select & 0x10) == 0) {
        if (right)  result &= ~0x01; // Clear bit 0 if Right pressed
        if (left)   result &= ~0x02; // Clear bit 1 if Left pressed
        if (up)     result &= ~0x04; // Clear bit 2 if Up pressed
        if (down)   result &= ~0x08; // Clear bit 3 if Down pressed
    }

    // Check if Action buttons are selected (bit 5 == 0)
    if ((joyp_select & 0x20) == 0) {
        if (a)      result &= ~0x01; // Clear bit 0 if A pressed
        if (b)      result &= ~0x02; // Clear bit 1 if B pressed
        if (select) result &= ~0x04; // Clear bit 2 if Select pressed
        if (start)  result &= ~0x08; // Clear bit 3 if Start pressed
    }

    return result;
}