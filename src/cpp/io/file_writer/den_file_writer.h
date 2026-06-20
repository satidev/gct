#ifndef DEN_FILE_WRITER_H
#define DEN_FILE_WRITER_H

#include <string>
#include <fstream>
#include "../file_utils.h"

namespace GCT::IO::ArrayFileWriter
{
struct DenMetaData
{
    unsigned short num_rows;
    unsigned short num_cols;
    unsigned short num_slices;
};

template<typename Container>
void write(DenMetaData const &metadata, Container const &data,
           std::string const &filename);

template<typename Container>
void write(DenMetaData const &metadata, Container const &data,
           std::string const &filename)
{
    //Check the consistency between metadata and data.
    auto const num_data_elems = size_t{static_cast<size_t>(metadata.num_rows * metadata.num_cols *
        metadata.num_slices)};
    if (data.size() != num_data_elems) {
        throw std::runtime_error{"Error during the writing of Dennerlein file " +
            filename + "\n" +
            std::string{"Mismatch in the number of elements between data and metadata.\n"}};
    }

    //Check whether the file is writable.
    if (!isWritable(std::filesystem::path{filename}.parent_path())) {
        throw std::runtime_error{"Error during the writing of Dennerlein file " +
            filename + "\n" +
            std::string{"Parent directory is not writable.\n"}};
    }

    auto file = std::ofstream{filename, std::ios::binary};
    if (!file) {
        throw std::runtime_error{"Error during the writing of Dennerlein file " +
            filename + "\n" +
            std::string{"File cannot be opened.\n"}};
    }

    //Write header.
    auto const header = std::array<unsigned short, 3u>{metadata.num_rows,
                                                       metadata.num_cols,
                                                       metadata.num_slices};
    file.write(reinterpret_cast<char const *>(header.data()),
               header.size() * sizeof(unsigned short));

    //Write data.
    auto const size_bytes = sizeof(typename Container::value_type) * data.size();
    file.write(reinterpret_cast<char const *>(data.data()), size_bytes);
    file.close();
}
}//GCT::IO::ArrayFileWriter namespaces.

#endif //DEN_FILE_WRITER_H


