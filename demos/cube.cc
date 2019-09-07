#include <cstdint>
#include <type_traits>

#include "s-mat.hh"
#include "bitmap.hh"
#include "math.hh"
#include "common.hh"

using namespace gba;
using namespace SMat;

using Vec2=Vec<fixed_t,2>;
using Vec3=Vec<fixed_t,3>;
using Vec4=Vec<fixed_t,4>;

using Mat3=Matrix<fixed_t,3,3>;
using Mat4=Matrix<fixed_t,4,4>;

constexpr const fixed_t cubeLength = 50;
constexpr const Vec3 g_cubePos {g_screenWidth/2, g_screenHeight/2, cubeLength};

constexpr const Vec3 g_cubeVertices[]
    {
     { .5_fx, .5_fx, .5_fx},
     { .5_fx, .5_fx,-.5_fx},
     { .5_fx,-.5_fx, .5_fx},
     { .5_fx,-.5_fx,-.5_fx},
     {-.5_fx, .5_fx, .5_fx},
     {-.5_fx, .5_fx,-.5_fx},
     {-.5_fx,-.5_fx, .5_fx},
     {-.5_fx,-.5_fx,-.5_fx},
    };

constexpr const Vec<uint8_t,2> lineIndices[]
{
    {0,1},
    {0,2},
    {0,4},
    {1,3},
    {1,5},
    {2,3},
    {2,6},
    {3,7},
    {4,5},
    {4,6},
    {5,7},
    {6,7},
};

int main()
{

    InputPoller poller;
    // wait for start to start demo
    while (! (poller.poll() & InputPoller::Button::Start) );
    setVideoMode(VideoMode::VG3);

    // clear screen
    uint16_t bgCol = rgb(120,120,120);
    for (uint16_t i=0; i<g_screenHeight*g_screenHeight; ++i)
        vram[i] = bgCol;
     wait_frame();

    for (;;)
    {
        uint16_t key = poller.poll();

        //undraw old cube
        drawLines( offset, )
        //draw new cube
        drawLines(
        wait_frame();
        auto in = poller.poll();
        if (in & 0xF0)
        {
        }
    }
}
