#include "meta_img_file_writer.h"

namespace GCT::IO::ArrayFileWriter::Detail::MetaImage
{
void writeHeaderEntry(std::string const &tag, std::string const &val,
                      std::basic_ofstream<char> &file)
{
    file << tag << " = " << val << "\n";
}

template<>
std::string typeStr<std::uint8_t>()
{
    return std::string{"MET_UCHAR"};
}

template<>
std::string typeStr<std::int8_t>()
{
    return std::string{"MET_CHAR"};
}

template<>
std::string typeStr<std::uint16_t>()
{
    return std::string{"MET_USHORT"};
}

template<>
std::string typeStr<std::int16_t>()
{
    return std::string{"MET_SHORT"};
}

template<>
std::string typeStr<std::uint32_t>()
{
    return std::string{"MET_UINT"};
}

template<>
std::string typeStr<std::int32_t>()
{
    return std::string{"MET_INT"};
}

template<>
std::string typeStr<std::int64_t>()
{
    return std::string{"MET_LONG_LONG"};
}

template<>
std::string typeStr<std::uint64_t>()
{
    return std::string{"MET_ULONG_LONG"};
}

template<>
std::string typeStr<float>()
{
    return std::string{"MET_FLOAT"};
}
template<>
std::string typeStr<double>()
{
    return std::string{"MET_DOUBLE"};
}

}//GCT::IO::ArrayFileWriter::Detail::MetaImage namespaces.

