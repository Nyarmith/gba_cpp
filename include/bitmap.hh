#ifdef  __GBA_BITMAP_HH__
#define __GBA_BITMAP_HH__

#include "common.hh"

namespace gba {

void bmpDrawPixel(uint8_t x, uint8_t y, uint16_t col)
{
    if (x < 0 || x >= g_screenWidth ||
        y < 0 || y >= g_screenHeight)
        return;

    vram[y*g_screenDims.x + x] = col;
}

}

#endif
