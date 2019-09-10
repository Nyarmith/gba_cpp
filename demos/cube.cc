#include <cstdint>
#include <type_traits>
#include <memory>

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

constexpr const fixed_t cubeLength_ = 50;
constexpr const Vec3 cubePos_ {g_screenWidth/2, g_screenHeight/2, cubeLength_};


//our coord system is:
// positive x-axis right
// positive y-axis down
// positive z-axis going into screen
// 
// this will be an orthographic drawing, so
// the range of 240 on x and 160 on y (with >0 z) will be viziblw

constexpr const Vec3 cubeVertices_[]
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

using LineInd=Vec<uint8_t,2>;
constexpr const LineInd lineIndices_[]
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

fixed_t xRot = 0.75_fx; // up-down changes rotation about x
fixed_t yRot = 0.75_fx; // left-right changes rotation about y


void drawLine(fixed_t x0, fixed_t y0, fixed_t x1, fixed_t y1, uint16_t col)
{
    int16_t ystep = y0 < y1 ? 1 : -1;
    int16_t xstep = x0 < x1 ? 1 : -1;

    // we don't need the fixed-point for actual drawing
    int16_t from_x = static_cast<int32_t>(x0);
    int16_t from_y = static_cast<int32_t>(y0);
    int16_t to_x   = static_cast<int32_t>(x1);
    int16_t to_y   = static_cast<int32_t>(y1);

    // vertical
    if (from_x == to_x)
    {
        while (from_y != to_y)
        {
            bmpDrawPixel(to_x, from_y, col);
            from_y += ystep;
        }
        return;
    }

    // horizontal
    else if (from_y == to_y)
    {
        while (from_x != to_x)
        {
            bmpDrawPixel(from_x, to_y, col);
            from_x += xstep;
        }
        return;
    }

    fixed_t dydx = (y1-y0)/(x1-x0)*static_cast<fixed_t>(static_cast<int32_t>(xstep));
    // along x
    //
    // consider swapping for greater y for linear access of vram
    if (dydx < 1_fx && dydx > -1_fx)
    {
        while (from_x != to_x)
        {
            bmpDrawPixel(from_x,static_cast<int32_t>(y0),col);
            from_x += xstep;
            y0 += dydx;
        }
    }
    else // along y
    {
        fixed_t dxdy = (x1-x0)/(y1-y0)*static_cast<fixed_t>(static_cast<int32_t>(ystep));
        while (from_y != to_y)
        {
            bmpDrawPixel(static_cast<int32_t>(x0),from_y,col);
            from_y += ystep;
            x0 += dxdy;
        }
    }
    

}

void drawCubeLines(const Mat3 &XR, const Mat3 &YR, const Vec3 translation,
                   const Vec3 vertices[], size_t sizeV,
                   const LineInd indices[], size_t sizeI,
                   const uint16_t color)
{
    auto txVs = std::make_unique<Vec3[]>(sizeV);
    
    auto R = cubeLength_*XR*YR;
    for (size_t i=0; i<sizeV; ++i)
        txVs[i] = vertices[i]*R + translation;

    for (size_t i=0; i<sizeI; ++i)
    {
        auto &from = txVs[indices[i][0]];
        auto &to   = txVs[indices[i][1]];
        drawLine(from[0], from[1], to[0], to[1], color);
    }

}

int main()
{

    InputPoller poller;
    setVideoMode(VideoMode::VG3);
    setTileMode(TileMode::BG2);

    // clear screen
    uint16_t bgCol = rgb15(20,20,20);
    for (uint16_t i=0; i<g_screenHeight*g_screenWidth; ++i)
        vram[i] = bgCol;
    wait_frame();

    // wait for start to start demo
    while (! (poller.poll() & InputPoller::Button::Start) );

    Mat3 xMat {};
    Mat3 yMat {};

    const uint16_t cubeCol = rgb15(20,220,90);
    for (;;)
    {
        uint16_t key = poller.poll();

        if (key & InputPoller::Left)
            xRot += .75;
        else if (key & InputPoller::Right)
            xRot -= .75;
        else if (key & InputPoller::Up)
            yRot += .75;
        else if (key & InputPoller::Down)
            yRot -= .75;

        // clear old cube
        drawCubeLines(xMat, yMat, cubePos_,
                      cubeVertices_, sizeof(cubeVertices_)/sizeof(Vec3),
                      lineIndices_, sizeof(lineIndices_)/sizeof(LineInd),
                      bgCol);
        // roate cube
        xMat = Mat3{ cos(xRot),-sin(xRot), 0,
                     sin(xRot), cos(xRot), 0,
                     0,         0,         1 };

        yMat = Mat3{ cos(yRot), 0,         sin(yRot),
                     0        , 1,         0        ,
                    -sin(yRot), 0,         cos(yRot) };

        // draw cube
        drawCubeLines(xMat, yMat, cubePos_,
                      cubeVertices_, sizeof(cubeVertices_)/sizeof(Vec3),
                      lineIndices_, sizeof(lineIndices_)/sizeof(LineInd),
                      cubeCol);

        // draw new cube
        wait_frame();
        wait_frame();
    }
}
