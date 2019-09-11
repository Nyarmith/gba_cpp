#ifndef __GBA_TILES_HH__
#define __GBA_TILES_HH__

#include "common.hh"

// Table: How tile-modes renders the vram
// // bpp = "bits-per-pixel"
// //
// // mode | bpp  | backgrounds | rotateable
// //------+------+-------------+----------
// //   0  | 16   | 4           |  0           - probably the most commonly used video mode
// //   1  | 8    | 3           |  1
// //   2  | 16   | 2           |  2

struct GbaObject
{
    uint8_t  y           : 8;
    uint8_t  rotate      : 1;
    uint8_t  scale       : 1;
    uint8_t  dispMode    : 2;
    uint8_t  mosaic      : 1;
    uint8_t  paletteMode : 1;
    uint8_t  shapeType   : 2;
    uint16_t x           : 9;
    uint8_t  transform   : 5;
    uint8_t  size        : 2;
    uint16_t tileNum     : 10;
    uint8_t  priority    : 2;
    uint8_t  palNum      : 4;
};

enum class PaletteMode {
    m16x32 = 0x00, /* 32 palettes of 16 colors */
    m256x2 = 0x40  /* 2  palettes of 32 colors */
};

void setPaletteMode(PaletteMode mode)
{
    ioram[1] |= m256x2;
}

void defineColor(uint16_t id, uint16_t col)
{
    palette[id] = col;
}

void defineTile(uint16_t addr, uint16_t *data, uint8_t len)
{
    for (uint8_t i=0; i<len; ++i)
    {
        vram[addr + i] = data[i];
    }
}

void makeObject(uint16_t id, uint8_t shapeType, uint8_t size, uint16_t tile)
{
    Object *o = static_cast<Object*>(oam[id*3]);
    o.shapeType = shapeType & 0x2;
    o.size = size & 0x3;
    o.tileNum = tile;
}

void moveObject(uint16_t id, uint16_t x, uint8_t y)
{
    Object *o = static_cast<Object*>(oam[id*3]);
    o.x = x & 0x1FF;
    o.y = y;
}

#endif
