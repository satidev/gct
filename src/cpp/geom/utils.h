#ifndef UTILS_H
#define UTILS_H

#include <numbers>
#include<cmath>

namespace GCT::Geom::Utils
{
template<typename T>
requires std::floating_point<T>
constexpr T degToRad(T ang_deg)
{
    return static_cast<T>(ang_deg * std::numbers::pi / 180.0);
}

template<typename T>
requires std::floating_point<T>
constexpr T radToDeg(T ang_deg)
{
    return static_cast<T>(ang_deg * 180.0 / std::numbers::pi);
}

// Wrap angle in the interval [0, 2*pi).
template<typename T>
requires std::floating_point<T>
constexpr T wrapAngle(T ang_rad)
{
    auto constexpr pi_2 = static_cast<T>(2.0 * std::numbers::pi);
    auto angle_wrapped = static_cast<T>(ang_rad);
    while (angle_wrapped < 0.0) {
        angle_wrapped += pi_2;
    }
    angle_wrapped = (angle_wrapped >= pi_2) ?
                    static_cast<T>(remainder(angle_wrapped, pi_2)) : angle_wrapped;
    return angle_wrapped;
}

// Wrap angle in the interval [0, 2*pi).
constexpr Units::Radians wrapFrom0To2PI(Units::Radians ang)
{
    auto constexpr pi_2 = 2.0f * std::numbers::pi_v<float>;
    auto angle_wrapped = ang.val;
    while (angle_wrapped < 0.0f) {
        angle_wrapped += pi_2;
    }
    angle_wrapped = (angle_wrapped >= pi_2) ?
                    remainder(angle_wrapped, pi_2) : angle_wrapped;
    return Units::Radians{angle_wrapped};
}

// Find in which quadrant the rotation angle lies.
constexpr unsigned quadrantIndex(float ang_rad)
{
    auto const ang_wrapped = wrapAngle(static_cast<double>(ang_rad));
    auto constexpr pi = std::numbers::pi;
    if ((ang_wrapped >= 0.0) && (ang_wrapped < pi / 2.0)) {//Interval [0, PI/2)
        return 1u;
    }
    else if ((ang_wrapped >= pi / 2.0) && (ang_wrapped < pi)) {//Interval [PI/2, PI)
            return 2u;
    }
    else if ((ang_wrapped >= pi) && (ang_wrapped < (3.0 / 2.0) * pi)) {//Interval [PI, 3*PI/2)
        return 3u;
    }
    else {//Interval [3*PI/2, 2PI)
        return 4u;
    }
}

// Find in which quadrant the rotation angle lies.
constexpr unsigned quadrantIndex(Units::Radians ang)
{
    auto const ang_wrapped = wrapFrom0To2PI(ang).val;
    auto constexpr pi = std::numbers::pi_v<float>;

    if ((ang_wrapped >= 0.0f) && (ang_wrapped < pi / 2.0f)) {//Interval [0, PI/2)
        return 1u;
    }
    else if ((ang_wrapped >= pi / 2.0f)
        && (ang_wrapped < pi)) {//Interval [PI/2, PI)
        return 2u;
    }
    else if ((ang_wrapped >= pi)
        && (ang_wrapped < (3.0f / 2.0f) * pi)) {//Interval [PI, 3*PI/2)
        return 3u;
    }
    else {//Interval [3*PI/2, 2PI)
        return 4u;
    }
}

}//GCT::Geom::Utils namespace.

#endif //UTILS_H
