#ifndef __GBA_FIXED_HH__
#define __GBA_FIXED_HH__

#include <cstdint>
#include <type_traits>
#include <iostream>
#include <string>

namespace gba {
// I+F should be less than half the number of bits in the underlying type
// to maintain accuracy in multiplication and division
template<typename T, unsigned I, unsigned F,
    typename std::enable_if_t<sizeof(T)*8/2 >= I+F+1 >* = nullptr>
class Fixed
{
public:
    constexpr Fixed(){}

    constexpr explicit Fixed(float f)
    {
        data = f*(1<<(F));
    };

    constexpr explicit Fixed(double f) : Fixed(static_cast<float>(f)) {};

    constexpr Fixed(T t)
    {
        data = t<<F;
    };

    constexpr Fixed operator+(const Fixed &o) const
    {
        Fixed ret;
        ret.data = data + o.data;
        return ret;
    }
    constexpr Fixed operator-(const Fixed &o) const
    {
        Fixed ret;
        ret.data = data - o.data;
        return ret;
    }
    constexpr Fixed operator*(const Fixed &o) const
    {
        Fixed ret;
        ret.data = (data*o.data)>>F;
        return ret;
    }
    constexpr Fixed operator/(const Fixed &o) const
    {
        Fixed ret;
        ret.data = (data<<F)/o.data;
        return ret;
    }
    constexpr Fixed operator-() const
    {
        Fixed ret;
        ret.data = -data;
        return ret;
    }
    constexpr operator float() const
    {
        return static_cast<float>(data)/(1<<F);
    }
    constexpr operator T() const
    {
        return data>>F;
    }
    constexpr T round() const
    {
        return data+(1<<(F-1))>>F;
    }

    constexpr bool operator<(const Fixed o) const
    {
        return data<o.data;
    }

    constexpr bool operator>(const Fixed o) const
    {
        return data>o.data;
    }

    constexpr bool operator==(const Fixed o) const
    {
        return data==o.data;
    }
    
    std::string to_string() const
    {
        return std::to_string(static_cast<float>(*this));
    }

private:
    T data{0};
};

} // namespace gba

#endif
