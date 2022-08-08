#pragma once

#include <stdint.h>

#define RGBA(r, g, b, a) \
    (((a) << 24) | ((r) << 16) | ((g) << 8) | ((b) << 0))

#define RGB(r, g, b) RGBA(r, g, b, 255)

typedef uint32_t colour_t;
