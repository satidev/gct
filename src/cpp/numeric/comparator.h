#ifndef GCT_COMPARATOR_H
#define GCT_COMPARATOR_H

#include "../geom/vec_wcs.h"
#include <algorithm>
#include <cmath>
#include <type_traits>
#include <vector>
#include "../data/data_constraints.h"
#include <span>

namespace GCT::Numeric::Comparator
{
template<std::floating_point T>
[[nodiscard]] constexpr bool isPositiveInf(T val)
{
    return (val > T{0}) && (std::isinf(val));
}

template<std::floating_point T>
[[nodiscard]] constexpr bool isNegativeInf(T val)
{
    return (val < T{0}) && std::isinf(val);
}

template<typename T>
[[nodiscard]] constexpr bool areEqual(T val_1, T val_2)
{
    return val_1 == val_2;
}

// Check the equality by computing the absolute difference.
template<std::floating_point T>
[[nodiscard]] constexpr bool areEqualAbs(T val_1, T val_2, T eps)
{
    return std::fabs(val_1 - val_2) <= eps;
}

// Checks equality utilizing the relative error.
template<std::floating_point T>
[[nodiscard]] constexpr bool areEqualRel(T val_1, T val_2, T eps)
{
    return std::fabs(val_1 - val_2) <= (eps * std::max(std::fabs(val_1), std::fabs(val_2)));
}

// Check equality utilizing both absolute and relative comparison.
template<std::floating_point T>
[[nodiscard]] constexpr bool areEqual(T val_1, T val_2, T eps)
{
    if ((std::isnan(val_1) && std::isnan(val_2)) ||
        (isPositiveInf(val_1) && isPositiveInf(val_2)) ||
        (isNegativeInf(val_1) && isNegativeInf(val_2))) {
        return true;
    }
    else {
        return areEqualAbs(val_1, val_2, eps) ||
            areEqualRel(val_1, val_2, eps);
    }
}

// Checks equality of 3D vectors in WCS.
[[nodiscard]] constexpr bool areEqual(Geom::VecWCS<3u> const &vec_1,
                                      Geom::VecWCS<3u> const &vec_2,
                                      float eps)
{
    return areEqual(vec_1.x.val, vec_2.x.val, eps) &&
        areEqual(vec_1.y.val, vec_2.y.val, eps) &&
        areEqual(vec_1.z.val, vec_2.z.val, eps);
}

// Checks equality of 2D vectors in WCS.
[[nodiscard]] constexpr bool areEqual(Geom::VecWCS<2u> const &vec_1,
                                      Geom::VecWCS<2u> const &vec_2,
                                      float eps)
{
    return areEqual(vec_1.x.val, vec_2.x.val, eps) &&
        areEqual(vec_1.y.val, vec_2.y.val, eps);
}

// Checks equality of floating point arrays.
template<std::floating_point T>
[[nodiscard]] constexpr bool areEqualArrays(std::span<const T> arr_1,
                                            std::span<const T> arr_2,
                                            T eps)
{
    if (arr_1.size() == arr_2.size()) {
        auto const cmp = [eps](T val_1, T val_2)
        {
            return areEqual(val_1, val_2, eps);
        };
        return std::equal(std::cbegin(arr_1), std::cend(arr_1), std::cbegin(arr_2), cmp);
    }
    else {
        return false;
    }
}

// Checks equality of STL arrays.
template<typename T>
[[nodiscard]] constexpr bool areEqualArrays(std::span<const T> arr_1, std::span<const T> arr_2)
{
    return std::equal(std::cbegin(arr_1), std::cend(arr_1), std::cbegin(arr_2));
}

// Checks whether all the elements of the float array are equal.
template<std::floating_point T>
[[nodiscard]] constexpr bool areAllElemsSame(std::span<const T> arr,
                                             T val, T eps)
{
    return std::all_of(std::cbegin(arr), std::cend(arr),
                       [val, eps](T elem)
                       {
                           return areEqual(val, elem, eps);
                       }
    );
}

// Checks whether all the elements of the array are equal.
template<typename T>
[[nodiscard]] constexpr bool areAllElemsSame(std::span<const T> arr, T val)
{
    return std::all_of(std::cbegin(arr), std::cend(arr), [val](T elem)
                       {
                           return elem == val;
                       }
    );
}

}// GCT::Numeric::Comparator namespace.

#endif//GCT_COMPARATOR_H
