#ifndef META_IMG_FILE_WRITER_H
#define META_IMG_FILE_WRITER_H

#include <string>
#include <fstream>
#include "../file_utils.h"
#include "../string_utils.h"
#include <array>
#include <numeric>
#include <iostream>
#include "raw_data_file_writer.h"

enum class ByteOrdier
{
    LITTLE_END
};

namespace GCT::IO::ArrayFileWriter
{
template<size_t ImgDim>
struct MetaImgMetaData
{
    std::array<unsigned, ImgDim> dim_size;
    std::array<float, ImgDim> spacing;
    std::array<float, ImgDim> offset;
    //Other headers
};
template<size_t ImgDim, typename Container>
void write(MetaImgMetaData<ImgDim> const &metadata, Container const &data,
           std::string const &filename);

namespace Detail::MetaImage
{
template<typename DataElemType>
std::string typeStr();

template<size_t ImgDim>
size_t numDataElems(std::array<unsigned, ImgDim> const &dim_size)
{
    return std::accumulate(std::cbegin(dim_size), std::cend(dim_size),
                           size_t{1UL}, std::multiplies<>());
}

template<size_t ImgDim>
void writeHeader(MetaImgMetaData<ImgDim> const &metadata,
                 std::string const &filename, std::string const &type_str,
                 std::basic_ofstream<char> &file);

template<typename Container>
void writeHeaderArray(std::string const &tag, Container const &val,
                      std::basic_ofstream<char> &file);

void writeHeaderEntry(std::string const &tag, std::string const &val,
                      std::basic_ofstream<char> &file);

template<typename Container>
void writeHeaderArray(std::string const &tag, Container const &val,
                      std::basic_ofstream<char> &file)
{
    auto str_list = std::vector<std::string>{};
    str_list.reserve(val.size());
    for (auto const &elem: val) {
        str_list.push_back(std::to_string(elem));
    }
    writeHeaderEntry(tag, join(str_list, " "), file);
}

template<size_t ImgDim>
void writeHeader(MetaImgMetaData<ImgDim> const &metadata,
                 std::string const &filename, std::string const &type_str,
                 std::basic_ofstream<char> &file)
{
    writeHeaderEntry("ObjectType", "Image", file);
    writeHeaderEntry("NDims", std::to_string(static_cast<int>(ImgDim)), file);
    writeHeaderEntry("BinaryData", "True", file);
    writeHeaderArray("DimSize", metadata.dim_size, file);
    writeHeaderArray("ElementSpacing", metadata.spacing, file);
    writeHeaderArray("Offset", metadata.offset, file);
    writeHeaderEntry("ElementType", type_str, file);
    writeHeaderEntry("BinaryDataByteOrderMSB", "False", file);

    //Extract file extension to confirm whether the header and data should be
    //in the same file or not.
    auto const file_path = std::filesystem::path{filename};
    auto const file_ext = toLower(file_path.extension());

    if (file_ext == std::string{".mha"}) {
        writeHeaderEntry("ElementDataFile", "LOCAL", file);
    }
    else {//Header and data are in separate files.
        auto const data_file = std::string{file_path.stem()} + std::string{".raw"};
        writeHeaderEntry("ElementDataFile", data_file, file);
    }
}

}//Detail::MetaImage namespaces.

template<size_t ImgDim, typename Container>
void write(MetaImgMetaData<ImgDim> const &metadata, Container const &data,
           std::string const &filename)
{
    using namespace Detail::MetaImage;
    //Check the consistency between metadata and data.
    if (data.size() != numDataElems(metadata.dim_size)) {
        throw std::runtime_error{"Error during the writing of Meta image file " +
            filename + "\n" +
            std::string{"Mismatch in the number of elements between data and metadata.\n"}};
    }

    if (!isWritable(std::filesystem::path{filename}.parent_path())) {
        throw std::runtime_error{"Error during the writing of Meta image file " +
            filename + "\n" +
            std::string{"Parent directory is not writable.\n"}};
    }

    auto file = std::ofstream{filename, std::ios::binary};
    if (!file) {
        throw std::runtime_error{"Error during the writing of Meta image file " +
            filename + "\n" +
            std::string{"File cannot be opened.\n"}};
    }

    //Write header entries.
    writeHeader(metadata, filename, typeStr<typename Container::value_type>(),
                file);

    //Extract file extension to confirm whether the header and data should be
    //in the same file or not.
    auto const file_path = std::filesystem::path{filename};
    auto const dir_path = file_path.parent_path();
    auto const file_ext = toLower(file_path.extension());

    if (file_ext == std::string{".mha"}) {//Header and data are in the same file.
        //Write data.
        auto const size_bytes = sizeof(typename Container::value_type) * data.size();
        file.write(reinterpret_cast<char const *>(data.data()), size_bytes);
    }
    else {//Header and data are in separate files.
        //Write data to a separate binary file.
        try {
            auto const data_file = std::string{file_path.stem()} + std::string{".raw"};
            auto const data_filename = dir_path / data_file;
            write(data, data_filename);
        }
        catch (std::exception const &e) {
            throw std::runtime_error{"Error during the writing of meta image file " +
                filename + "\n" +
                std::string{e.what()} + "\n"};
        }
    }
    file.close();
}

}//GCT::IO::ArrayFileWriter namespaces.

#endif //META_IMG_FILE_WRITER_H


