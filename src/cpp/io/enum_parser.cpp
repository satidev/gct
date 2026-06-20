#include "enum_parser.h"
#include "metadata.h"

namespace GCT::IO
{
template<>
EnumParser<GCT::IO::ArrayElemType>::EnumParser()
{
    enum_str_map_[ArrayElemType::T_UINT8] = "T_UINT8";
    enum_str_map_[ArrayElemType::T_INT8] = "T_INT8";
    enum_str_map_[ArrayElemType::T_UINT16] = "T_UINT16";
    enum_str_map_[ArrayElemType::T_INT16] = "T_INT16";
    enum_str_map_[ArrayElemType::T_UINT32] = "T_UINT32";
    enum_str_map_[ArrayElemType::T_INT32] = "T_INT32";
    enum_str_map_[ArrayElemType::T_UINT64] = "T_UINT64";
    enum_str_map_[ArrayElemType::T_INT64] = "T_INT64";
    enum_str_map_[ArrayElemType::T_FLOAT] = "T_FLOAT";
    enum_str_map_[ArrayElemType::T_DOUBLE] = "T_DOUBLE";

    str_enum_map_["T_UINT8"] = ArrayElemType::T_UINT8;
    str_enum_map_["T_INT8"] = ArrayElemType::T_INT8;
    str_enum_map_["T_UINT16"] = ArrayElemType::T_UINT16;
    str_enum_map_["T_INT16"] = ArrayElemType::T_INT16;
    str_enum_map_["T_UINT32"] = ArrayElemType::T_UINT32;
    str_enum_map_["T_INT32"] = ArrayElemType::T_INT32;
    str_enum_map_["T_UINT64"] = ArrayElemType::T_UINT64;
    str_enum_map_["T_INT64"] = ArrayElemType::T_INT64;
    str_enum_map_["T_FLOAT"] = ArrayElemType::T_FLOAT;
    str_enum_map_["T_DOUBLE"] = ArrayElemType::T_DOUBLE;
}

}//GCT::IO namespaces.