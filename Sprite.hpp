#pragma once // <<< Add include guard
#include <stdint.h>

class Sprite
{
public:
    uint8_t y;
    uint8_t x;
    uint8_t tileIndex;
    uint8_t flags;
    uint8_t oam_index; // <<< Add OAM index

    Sprite() : y(0), x(0), tileIndex(0), flags(0), oam_index(0) {} // <<< Init oam_index

    // <<< Update constructor
    Sprite(uint8_t y_, uint8_t x_, uint8_t tileIndex_, uint8_t flags_, uint8_t oam_index_)
        : y(y_), x(x_), tileIndex(tileIndex_), flags(flags_), oam_index(oam_index_) {}

    // <<< Add comparison operator for sorting
    bool operator<(const Sprite& other) const {
        if (x != other.x) {
            return x < other.x; // Lower X has higher priority
        }
        // If X is the same, lower OAM index has higher priority
        return oam_index < other.oam_index;
    }
};