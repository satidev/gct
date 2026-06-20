#include "den_file_reader.h"
#include <fstream>
#include <numeric>

namespace GCT::IO
{
ArrayMetadata DenFileReader::readMetadata() const
{
    auto constexpr num_elem_header = 3UL;
    auto header = std::array<unsigned short, num_elem_header>{};
    auto constexpr header_bytes = sizeof(unsigned short) * num_elem_header;

    //Read 6 byte header.
    auto file = std::ifstream{};
    file.open(filename_.c_str(), std::ios::binary | std::ios::in);
    file.read(reinterpret_cast<char *>(header.data()), header_bytes);
    file.seekg(header_bytes, std::ios::beg);
    file.close();

    auto md = ArrayMetadata{};
    md.num_rows = header[0];
    md.num_cols = header[1];
    md.num_slices = header[2];
    auto const type_info = dataType(header, fileSize(filename_));
    md.type = type_info.first;

    return md;
}

std::pair<ArrayElemType, unsigned> DenFileReader::dataType(
    std::array<unsigned short, 3UL> const &header, size_t file_size) const
{
    auto const header_bytes = 3 * sizeof(unsigned short);
    auto const data_bytes = file_size - header_bytes;
    auto const num_elements = std::accumulate(std::cbegin(header), std::cend(header), 1UL,
                                              std::multiplies<>());
    auto const element_size_bytes = data_bytes / num_elements;

    auto type = ArrayElemType::T_UINT8;
    switch (element_size_bytes) {
        case 1UL:
            type = ArrayElemType::T_UINT8;
            break;
        case 2UL:
            type = ArrayElemType::T_UINT16;
            break;
        case 4UL:
            type = ArrayElemType::T_FLOAT;
            break;
        case 8UL:
            type = ArrayElemType::T_DOUBLE;
            break;
        default:
            break;
    }
    return std::make_pair(type, static_cast<unsigned>(element_size_bytes));
}

std::vector<uint8_t> DenFileReader::readDataBytesFile(std::string const &data_filename,
                                                      ArrayMetadata const &md) const
{
    auto constexpr num_elem_header = 3UL;
    auto constexpr skip_bytes = sizeof(unsigned short) * num_elem_header;
    return readDataFromBinFile(data_filename, md, skip_bytes);
}

}//GCT::IO namespaces.
