#include <deque>
#include <cstdint>
#include <cstring>

#include <stdlib.h>

struct Point { uint8_t x, y; }; 

// # some useful constants
constexpr Point g_screenDims { 240, 160 };
constexpr int GameOver = 1;

// special memory
static volatile uint8_t * ioram =   (uint8_t*) 0x04000000;
static  uint16_t * vram =   (uint16_t*) 0x06000000;

// game specific constants
constexpr Point g_gridSize  {15, 15};
constexpr Point g_gridBlockSize {8, 8};
constexpr Point g_gridCenter
{ 
  g_screenDims.x/2 + (g_gridSize.x%2 == 0 ? 0 : -g_gridBlockSize.x/2),
  g_screenDims.y/2 + (g_gridSize.y%2 == 0 ? 0 : -g_gridBlockSize.y/2)
};

constexpr Point g_gridStart
{
    g_gridCenter.x - g_gridSize.x*g_gridBlockSize.x/2,
    g_gridCenter.y - g_gridSize.y*g_gridBlockSize.y/2,
};

// # drawing functions
constexpr uint16_t rgb15(uint8_t r, uint8_t g, uint8_t b)
{
    return r | (g << 5) | (b << 10);
}

void bmpDrawPixel(uint8_t x, uint8_t y, uint16_t col)
{
    if (x < 0 || x >= g_screenDims.x ||
        y < 0 || y >= g_screenDims.y)
        return;

    vram[y*g_screenDims.x + x] = col;
}

void bmpDrawPixel(Point pt, uint16_t col)
{
    bmpDrawPixel(pt.x, pt.y, col);
}

// input is in terms of grid
void drawGridBlock(Point pt, uint16_t col)
{
    if (pt.x < 0 || pt.x >= g_gridSize.x ||
        pt.y < 0 || pt.y >= g_gridSize.y)
        return;

    for (int y_t=0; y_t<g_gridBlockSize.y; ++y_t)
        for (int x_t=0; x_t<g_gridBlockSize.x; ++x_t)
            bmpDrawPixel(g_gridStart.x+pt.x*g_gridBlockSize.x+x_t,
                         g_gridStart.y+pt.y*g_gridBlockSize.y+y_t,
                         col);
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

    uint16_t poll()
    {
        const uint16_t InputReg {0x0130};
        prev_ = key_;
        key_ = ~*(ioram + InputReg);
        return key_ & Mask;
    }
private:
    uint16_t key_;
    uint16_t prev_;
};

// stupid synchronization mechanism based on vertical draw state, TODO: use interrupts
void frame_sync()
{
    volatile uint32_t *vcount = reinterpret_cast<volatile uint32_t*>(ioram + 0x6);
    while (*vcount >= 160);
    while (*vcount < 160);
}

struct Snack
{
    void newPos()
    {
        pos.x = rand() % g_gridSize.x;
        pos.y = rand() % g_gridSize.y;
    };
    Point pos{5,5};
};

// snake and board drawing representation
struct gameSnakeBoard
{
    bool GridRep[g_gridSize.x][g_gridSize.y];
    std::deque<Point> snakePos {{4,3}, {3,3}}; // 1st element is head

    gameSnakeBoard()
    {
        resetGridRep();
    };

    void resetGridRep()
    {
        for (uint8_t x=0; x<g_gridSize.x; ++x)
            for (uint8_t y=0; y<g_gridSize.y; ++y)
                GridRep[x][y] = false;

        for (auto e : snakePos)
            GridRep[e.x][e.y] = true;
    }

    int update()
    {
        auto first = snakePos.front();
        const auto last = snakePos.back();

        GridRep[last.x][last.y] = false;

        switch (lastDir)
        {
            case Input::Button::Left:
                --first.x;
                break;
            case Input::Button::Right:
                ++first.x;
                break;
            case Input::Button::Up:
                --first.y;
                break;
            case Input::Button::Down:
                ++first.y;
                break;
        }
        //TODO: turning around

        // self-collision
        for (auto snakeP : snakePos)
        {
            if (first.x == snakeP.x && first.y == snakeP.y)
                return GameOver;
        }

        // wall collision
        if (first.x > g_gridSize.x - 1 || first.x < 0 || first.y > g_gridSize.y - 1 || first.y < 0)
            return GameOver;

        // eating block makes the nsnake not reduce its length and redraw a snack
        if (first.x == snack.pos.x && first.y == snack.pos.y)
        {
            bool snackCheck = false;
            while (!snackCheck)
            {
                snack.newPos();
                for(auto snakeP : snakePos)
                {
                    if (snakeP.x == snack.pos.x && snakeP.y == snack.pos.y)
                        break;
                }
                snackCheck = true;
            }
            drawGridBlock({snack.pos.x, snack.pos.y}, snackCol);
        } else {
            snakePos.pop_back();
        }

        snakePos.push_front(first);
        GridRep[first.x][first.y] = true;

        drawGridBlock({first.x, first.y}, snakeCol);
        drawGridBlock({last.x, last.y}, bgCol);

        return 0;
    };

    void handleInput(uint16_t in)
    {
        if (in & Input::Button::Left)       lastDir = Input::Button::Left;
        else if (in & Input::Button::Right) lastDir = Input::Button::Right;
        else if (in & Input::Button::Up)    lastDir = Input::Button::Up;
        else if (in & Input::Button::Down)  lastDir = Input::Button::Down;
    }

    void draw()
    {
        for (uint8_t x=0; x<g_gridSize.x; ++x)
            for (uint8_t y=0; y<g_gridSize.y; ++y)
                if (GridRep[x][y])
                    drawGridBlock({x, y}, snakeCol);
                else
                    drawGridBlock({x, y}, bgCol);

        drawGridBlock({snack.pos.x, snack.pos.y}, snackCol);
    };

    void reset()
    {
        snakePos = {{4,3}, {3,3}};
        snack = {5,5};
        lastDir = Input::Button::Right;
        resetGridRep();
    }

    const uint16_t snakeCol = rgb15(0,120,20);
    const uint16_t bgCol    = rgb15(20,20,20);
    const uint16_t snackCol = rgb15(120,120,0);

    Snack snack {5,5};
    uint16_t lastDir{Input::Button::Right};
} g_snakeGame;

int main()
{
    Input poller;

    // setup initial snake and snack

    ioram[0] = 0x3; // video mode 3 = bitmap mode
    ioram[1] = 0x4;

    // g_snakeGame.draw();
    while (true)
    {
        g_snakeGame.draw();
        // wait for start button
        while (! (poller.poll() & Input::Button::Start) );
        
        const uint8_t frameReset = 32; // 1 frame == 16.66 milliseconds, 32 frame ~= .533 seconds
        uint8_t frameCtr = frameReset;
        for (;;)
        {
            frame_sync();
            auto in = poller.poll();
            if (in & 0xF0)
                g_snakeGame.handleInput(in);

            if (frameCtr > 0)
            {
                --frameCtr;
                continue;
            }
            frameCtr = frameReset;

            auto stat = g_snakeGame.update();
            
            if (stat == GameOver)
                break;
        }

        g_snakeGame.reset();
    }
}
