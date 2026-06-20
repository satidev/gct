#ifndef DATA_CONSTRAINTS_H
#define DATA_CONSTRAINTS_H

#include <concepts>
#include <cstddef>

namespace GCT::Data
{
// Compile-time checks for std::array and std::vector.
template<typename T>
concept STLArrays = requires {
    typename T::value_type;
    typename T::reference;
    typename T::const_reference;
    typename T::iterator;
    typename T::const_iterator;
    typename T::size_type;

    { std::declval<T>().begin() } -> std::same_as<typename T::iterator>;
    { std::declval<T>().end() } -> std::same_as<typename T::iterator>;
    { std::declval<T>().cbegin() } -> std::same_as<typename T::const_iterator>;
    { std::declval<T>().cend() } -> std::same_as<typename T::const_iterator>;

    { std::declval<T>().size() } -> std::same_as<typename T::size_type>;
    { std::declval<T>().empty() } -> std::same_as<bool>;
    { std::declval<T>().data() } -> std::same_as<typename T::value_type *>;
};

template<typename T>
concept STLFloatArrays = STLArrays<T> && std::floating_point<typename T::value_type>;

template<typename T>
concept STLIntegralArrays = STLArrays<T> && std::integral<typename T::value_type>;

template<typename T>
concept STLNumArrays = STLIntegralArrays<T> || STLFloatArrays<T>;

template<typename T>
concept NumericType = std::integral<T> || std::floating_point<T>;

template<size_t Dim>
concept ValidDim = (Dim > 0u);

}//GCT::Data namespace.

#endif //DATA_CONSTRAINTS_H
