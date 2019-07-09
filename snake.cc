#include "gba.hh"

int main()
{
    setVideoMode(VideoMode::VG3);
    setTileMode(TileMode::BG2);

    vram[80*240 + 115] = 0x001F;
    vram[80*240 + 120] = 0x03E0;
    vram[80*240 + 125] = 0x7C00;

    for (;;) {}

    return 0;
}
