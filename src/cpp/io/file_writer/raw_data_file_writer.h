#ifndef RAW_DATA_FILE_WRITER_H
#define RAW_DATA_FILE_WRITER_H

#include <string>
#include <fstream>
#include "../file_utils.h"

namespace GCT::IO::ArrayFileWriter
{
template<typename Container>
void write(Container const &data, std::string const &filename);

template<typename Container>
void write(Container const &data, std::string const &filename)
{
    // Check whether the file is writable.
    if(!isWritable(std::filesystem::path{filename}.parent_path())){
        throw std::runtime_error{"Error during the writing of binary file " +
            filename + "\n" +
            std::string{"Parent directory is not writable.\n"}};
    }

    auto file = std::ofstream{filename, std::ios::binary};
    if (!file) {
        throw std::runtime_error{"Error during the writing of binary file " +
            filename + "\n" +
            std::string{"File cannot be opened.\n"}};
    }

    //Write data.
    auto const size_bytes = sizeof(typename Container::value_type) * data.size();
    file.write(reinterpret_cast<char const *>(data.data()), size_bytes);
    file.close();
}
}//GCT::IO::ArrayFileWriter namespaces.

#endif //RAW_DATA_FILE_WRITER_H


