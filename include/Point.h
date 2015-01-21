#ifndef __POINT__
#define __POINT__

#include <stdint.h>
#include "Vector.h"

struct Color {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
};

struct ScreenPoint {
    Color color;
    uint16_t x;
    uint16_t y;
    uint32_t d;

    ScreenPoint() {
    }
    ScreenPoint(const Vector& v){
        x = v.x+0.5;
        y = v.y+0.5;
        d = v.z+0.5;
    }
};

#endif
