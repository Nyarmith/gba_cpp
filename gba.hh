#include <cstdint>

static volatile uint8_t  * const ioram =  (uint8_t*) 0x04000000;
static volatile uint16_t * const pal   =  (uint16_t*)0x05000000;
static volatile uint16_t * const vram  =  (uint16_t*)0x06000000;
static volatile uint16_t * const oam   =  (uint16_t*)0x07000000;



// Table: How video-mode renders the vram
// bpp = "bits-per-pixel"
//
// mode | width  | height  | bpp  | size     | page-flip
//------+--------+---------+------+----------+----------
//   3  |  240   | 160     | 16   | 1x12C00h |  No
//   4  |  240   | 160     | 8    | 2x9600h  |  Yes
//   5  |  160   | 128     | 16   | 2xA000h  |  Yes

enum class VideoMode : uint8_t { VG3 = 0x03, VG4 = 0x04, VG5 = 0x05 };

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

struct Input
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
private:
    uint16_t key_;
    uint16_t prev_;
    uint16_t poll()
    {
        const uint16_t InputReg {0x0130};
        prev_ = key_;
        key_ = *(ioram + InputReg);
        return key_;
    }
};
