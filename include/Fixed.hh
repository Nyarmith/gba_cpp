#include <cstdint>
#include <type_traits>
#include <iostream>
#include <string>

// I+F should be less than half the number of bits in the underlying type
// to maintain accuracy in multiplication and division
template<typename T, unsigned I, unsigned F,
    typename std::enable_if_t<sizeof(T)*8/2 >= I+F+1 >* = nullptr>
class Fixed
{
public:
    Fixed(){}

    Fixed(float f)
    {
        data = f*(1<<(F));
    };

    Fixed(double f) : Fixed(static_cast<float>(f)) {};

    Fixed(T t)
    {
        data = t<<F;
    };

    Fixed operator+(const Fixed &o) const
    {
        Fixed ret;
        ret.data = data + o.data;
        return ret;
    }
    Fixed operator-(const Fixed &o) const
    {
        Fixed ret;
        ret.data = data - o.data;
        return ret;
    }
    Fixed operator*(const Fixed &o) const
    {
        Fixed ret;
        ret.data = (data*o.data)>>F;
        return ret;
    }
    Fixed operator/(const Fixed &o) const
    {
        Fixed ret;
        ret.data = (data<<F)/o.data;
        return ret;
    }
    operator float() const
    {
        return static_cast<float>(data)/(1<<F);
    }
    operator T() const
    {
        return data>>F;
    }
    T round() const
    {
        return data+(1<<(F-1))>>F;
    }
    std::string to_string() const
    {
        return std::to_string(static_cast<float>(*this));
    }

private:
    T data{0};
};
