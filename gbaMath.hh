namespace math
{
    constexpr float Pi = 3.1415927;

    constexpr float sin(float x)
    {
        if (x < -Pi/2.f)
            return sin(x + Pi);
        else if (x > Pi/2.f)
            return sin(x - Pi);
        return x - x*x*x/6.f + x*x*x*x*x/120.f;
    }

    constexpr float cos(float x)
    {
        if (x < -Pi/2.f)
            return cos(x + Pi);
        else if (x > Pi/2.f)
            return cos(x - Pi);
        return 1.f - x*x/2.f + x*x*x*x/24.f;
    }
}
