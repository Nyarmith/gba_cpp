#ifndef __GBA_COMMON_HH__
#define __GBA_COMMON_HH__

#include <cstdint>

namespace gba {

// special memory segments
static volatile uint8_t  * const ioram =  (uint8_t*) 0x04000000;
static volatile uint16_t * const pal   =  (uint16_t*)0x05000000;
static uint16_t * const vram  =  (uint16_t*)0x06000000;
static uint16_t * const oam   =  (uint16_t*)0x07000000;

constexpr const uint16_t g_screenHeight = 160;
constexpr const uint16_t g_screenWidth  = 240;

// Table: How video-mode renders the vram
// bpp = "bits-per-pixel"
//
// mode | width  | height  | bpp  | size     | page-flip
//------+--------+---------+------+----------+----------
//   3  |  240   | 160     | 16   | 1x12C00h |  No
//   4  |  240   | 160     | 8    | 2x9600h  |  Yes
//   5  |  160   | 128     | 16   | 2xA000h  |  Yes

enum class VideoMode : uint8_t {
    VG0 = 0x00, VG1 = 0x01, VG2 = 0x02, /* tiling modes */
    VG3 = 0x03, VG4 = 0x04, VG5 = 0x05  /* bitmap modes */
};

void setVideoMode(const VideoMode mode)
{
    ioram[0] = static_cast<uint8_t>(mode);
}

// Table: Interaction between video and tile-mode
// on background tile type (regular or affine)
//
//  mode |  BG0  |  BG1  |  BG2  |  BG3
// ------+-------+-------+-------+-----
//    0  |  reg  |  reg  |  reg  |  reg
//    1  |  reg  |  reg  |  aff  |  -
//    2  |  -    |  -    |  aff  |  aff

enum class TileMode : uint8_t { BG0 = 1, BG1 = 2, BG2 = 4, BG3 = 8 };

void setTileMode(const TileMode mode)
{
    ioram[1] = static_cast<uint8_t>(mode);
}

/**
 * Input handling class
 */
struct InputPoller
{
    enum Button : uint16_t
    {
        A      = 0x0001, B     = 0x0002,
        Select = 0x0004, Start = 0x0008,
        Right  = 0x0010, Left  = 0x0020,
        Up     = 0x0040, Down  = 0x0080,
        R      = 0x0100, L     = 0x0200
    };

    const uint16_t Mask {0x03FF};

    uint16_t poll()
    {
        const uint16_t InputReg {0x0130};
        prev_ = key_;
        key_ = ~*(ioram + InputReg) & Mask;
        return key_;
    }

    uint16_t prev()
    {
        return prev_;
    }
private:
    uint16_t key_;
    uint16_t prev_;
};

/**
 * Create a 15-bit rgb value with the first 5 bits of each arg
 */
uint16_t rgb15(uint8_t r, uint8_t g, uint8_t b)
{
    return r | (g<<5) | (b<<10);
}

/**
 * Primitive synchronization mechanism that wastes a lot of battery
 */
void wait_frame()
{
    // wait for vcount i/o to finish
    const uint8_t vcount_reg = 0x0006;
    while (ioram[vcount_reg] >= 160);
    while (ioram[vcount_reg] < 160);
}


}   //namespace gba
#endif
