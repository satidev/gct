#include "proj_meta_info.h"

namespace GCT::Data
{
template<>
std::array<unsigned, 1u> projSizeToArr(ProjSize<1u> const &size)
{
    return std::array<unsigned, 1u>{size.num_cols};
}

template<>
std::array<unsigned, 2u> projSizeToArr(ProjSize<1u> const &size,
                                       unsigned num_proj)
{
    return std::array<unsigned, 2u>{size.num_cols, num_proj};
}

template<>
std::array<unsigned, 2u> projSizeToArr(ProjSize<2u> const &size)
{
    return std::array<unsigned, 2u>{size.num_cols, size.num_rows};
}

template<>
std::array<unsigned, 3u> projSizeToArr(ProjSize<2u> const &size,
                                       unsigned num_proj)
{
    return std::array<unsigned, 3u>{size.num_cols, size.num_rows,
                                    num_proj};
}

template<>
ProjSize<1u> arrToProjSize(std::array<unsigned, 1u> const &arr)
{
    return ProjSize<1u>{arr.at(0)};
}

template<>
ProjSize<2u> arrToProjSize(std::array<unsigned, 2u> const &arr)
{
    return ProjSize<2u>{arr.at(0), arr.at(1)};
}
}//GCT::Data namespace.
