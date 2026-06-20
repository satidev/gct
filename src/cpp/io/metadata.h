#ifndef METADATA_H
#define METADATA_H

#include <array>
#include <string>
#include <vector>

namespace GCT::IO
{
enum class ArrayElemType
{
    T_UINT8,
    T_INT8,
    T_UINT16,
    T_INT16,
    T_UINT32,
    T_INT32,
    T_UINT64,
    T_INT64,
    T_FLOAT,
    T_DOUBLE
};

struct ArrayMetadata
{
    unsigned num_rows;
    unsigned num_cols;
    unsigned num_slices;//Number of matrices/images in each file.
    ArrayElemType type;
};

}//GCT::IO namespace.

#endif //METADATA_H


