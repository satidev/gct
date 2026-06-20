#ifndef UNITS_H
#define UNITS_H

#include <numbers>

namespace GCT::Geom::Units
{
// Distance in millimeters.
struct MilliMeters
{
    float val;
};

// Unit of distance in detector pixels.
struct DetPixels
{
    float val;
};

// Unit of angular measure.
struct Radians
{
    float val;
};

constexpr Radians operator ""_rad(long double val)
{
    return Radians{static_cast<float>(val)};
}

constexpr Radians operator ""_deg(long double val)
{
    auto const rad = std::numbers::pi_v<long double> * val / 180.0;
    return Radians{static_cast<float>(rad)};
}

constexpr MilliMeters operator ""_mm(long double val)
{
    return MilliMeters{static_cast<float>(val)};
}

constexpr DetPixels operator ""_pix(long double val)
{
    return DetPixels{static_cast<float>(val)};
}

template<typename Unit>
constexpr bool operator==(Unit a, Unit b)
{
    return a.val == b.val;
}

template<typename Unit>
constexpr bool operator!=(Unit a, Unit b)
{
    return a.val != b.val;
}

template<typename Unit>
constexpr bool operator<(Unit a, Unit b)
{
    return a.val < b.val;
}

template<typename Unit>
constexpr bool operator<=(Unit a, Unit b)
{
    return a.val <= b.val;
}

template<typename Unit>
constexpr bool operator>(Unit a, Unit b)
{
    return a.val > b.val;
}

template<typename Unit>
constexpr bool operator>=(Unit a, Unit b)
{
    return a.val >= b.val;
}

template<typename Unit>
constexpr Unit operator-(Unit a)
{
    return Unit{-a.val};
}

template<typename Unit>
constexpr Unit operator+(Unit a, Unit b)
{
    return Unit{a.val + b.val};
}

template<typename Unit>
constexpr Unit operator-(Unit a, Unit b)
{
    return Unit{a.val - b.val};
}

// Unit will be canceled after division, hence returns the number.
template<typename Unit>
constexpr float operator/(Unit a, Unit b)
{
    return a.val / b.val;
}

template<typename Unit>
constexpr Unit operator+(Unit unit, float scalar)
{
    return Unit{unit.val + scalar};
}

template<typename Unit>
constexpr Unit operator-(Unit unit, float scalar)
{
    return Unit{unit.val - scalar};
}

template<typename Unit>
constexpr Unit operator*(Unit unit, float scalar)
{
    return Unit{unit.val * scalar};
}

template<typename Unit>
constexpr Unit operator/(Unit unit, float scalar)
{
    return Unit{unit.val / scalar};
}

}//GCT::Geom::Units namespace.

#endif //UNITS_H
