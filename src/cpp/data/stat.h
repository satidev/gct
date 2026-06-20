#ifndef STAT_H
#define STAT_H

#include "nd_array.h"

namespace GCT::Data::Stat
{
template<typename PixType, size_t Dim>
double sum(NDArray<PixType, Dim> const &img)
{
    auto const &data = img.elements();
    return std::accumulate(std::cbegin(data), std::cend(data), double{0.0});
}

template<typename PixType, size_t Dim>
double mean(NDArray<PixType, Dim> const &img)
{
    return sum(img) / static_cast<double>(img.numElements());
}

}//GCT::Data::Stat namespace.

#endif //STAT_H
