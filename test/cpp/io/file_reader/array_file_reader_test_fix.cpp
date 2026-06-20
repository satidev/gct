#include "array_file_reader_test_fix.h"

namespace GCT::Test
{
template<>
void ArrayFileReaderTestFix<float, 2u>::setMetaData()
{
    md_gt.num_rows = 2u;
    md_gt.num_cols = 3u;
    md_gt.num_slices = 1u;
    md_gt.type = GCT::IO::ArrayElemType::T_FLOAT;
}

template<>
void ArrayFileReaderTestFix<float, 3u>::setMetaData()
{
    md_gt.num_rows = 2u;
    md_gt.num_cols = 3u;
    md_gt.num_slices = 4u;
    md_gt.type = GCT::IO::ArrayElemType::T_FLOAT;
}

template<>
void ArrayFileReaderTestFix<std::uint8_t , 2u>::setMetaData()
{
    md_gt.num_rows = 2u;
    md_gt.num_cols = 3u;
    md_gt.num_slices = 1u;
    md_gt.type = GCT::IO::ArrayElemType::T_UINT8;
}

template<>
std::string ArrayFileReaderTestFix<float, 2u>::dirName()
{
    return std::string{"float_2d"};
}

template<>
std::string ArrayFileReaderTestFix<float, 3u>::dirName()
{
    return std::string{"float_3d"};
}

template<>
std::string ArrayFileReaderTestFix<std::uint8_t , 2u>::dirName()
{
    return std::string{"uint8_2d"};
}

}//GCT::Test namespaces.
