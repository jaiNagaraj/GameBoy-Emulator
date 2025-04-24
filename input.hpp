#pragma once
#include <stdint.h>

class Input {
public:
    // Button states (true = pressed, false = not pressed)
    bool right = false;
    bool left = false;
    bool up = false;
    bool down = false;
    bool a = false;
    bool b = false;
    bool select = false;
    bool start = false;

    // Function to update button state (called from SDL event loop)
    void set_button_state(uint8_t button_index, bool pressed);

    // Function called by MMU to get the JOYP register value
    uint8_t get_joyp_state(uint8_t mmap_joyp_value) const;
};