#include <stdint.h>

class Sprite
{
public:
    uint8_t y;
    uint8_t x;
    uint8_t tileIndex;
    uint8_t flags;

    Sprite() : y(0), x(0), tileIndex(0), flags(0) {}

    Sprite(uint8_t y, uint8_t x, uint8_t tileIndex, uint8_t flags)
        : y(y), x(x), tileIndex(tileIndex), flags(flags) {}
};