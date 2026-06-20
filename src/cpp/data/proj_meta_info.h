#ifndef GCT_CORE_CPP_GEOM_PROJ_SIZE_H_
#define GCT_CORE_CPP_GEOM_PROJ_SIZE_H_

#include <array>
#include <cstdio>
#include "../geom/geom_constraints.h"

namespace GCT::Data
{
// Presence or absence of information about detector column,
// determines the dimension of projection (1D or 2D).
template<typename ...DetCol>
constexpr size_t projDim()
{
    return sizeof...(DetCol) + 1u;
}

template<size_t ProjDim> requires Geom::ValidProjDim<ProjDim>
struct ProjSize;

template<>
struct ProjSize<1u>
{
    unsigned num_cols;
};

template<>
struct ProjSize<2u>
{
    unsigned num_cols, num_rows;
};

template<size_t ProjDim>
requires Geom::ValidProjDim<ProjDim>
struct ProjMetaInfo
{
    ProjSize<ProjDim> size;
    unsigned num_proj;
};

inline unsigned numProjPixels(ProjSize<1u> const &size)
{
    return size.num_cols;
}

inline unsigned numProjPixels(ProjSize<2u> const &size)
{
    return size.num_cols*size.num_rows;
}

inline unsigned numProjRows(ProjSize<1u> const &size)
{
    return 1u;
}

inline unsigned numProjRows(ProjSize<2u> const &size)
{
    return size.num_rows;
}

inline bool isEqual(ProjSize<1u> const &size_1, ProjSize<1u> const &size_2)
{
    return (size_1.num_cols == size_2.num_cols);
}
inline bool isEqual(ProjSize<2u> const &size_1, ProjSize<2u> const &size_2)
{
    return (size_1.num_cols == size_2.num_cols) &&
        (size_1.num_rows == size_2.num_rows);
}

template<size_t ProjDim>
std::array<unsigned, ProjDim> projSizeToArr(ProjSize<ProjDim> const &size);

template<size_t ProjDim>
std::array<unsigned, ProjDim + 1u> projSizeToArr(ProjSize<ProjDim> const &size,
                                                 unsigned num_proj);

template<size_t ProjDim>
ProjSize<ProjDim> arrToProjSize(std::array<unsigned, ProjDim> const &arr);

}//GCT::Data namespace.

#endif //GCT_CORE_CPP_GEOM_PROJ_SIZE_H_
