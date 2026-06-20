#include "array_file_reader.h"
#include <fstream>

namespace GCT::IO
{
ArrayFileReader::ArrayFileReader(std::string filename, bool series)
    :
    filename_{std::move(filename)}, series_{series}
{
    if (!doesFileExist(filename_))
        throw std::runtime_error{"File " + filename_ + " is not found.\n"};

    setTypeInfo();
}

void ArrayFileReader::setTypeInfo()
{
    type_size_[ArrayElemType::T_UINT8] = sizeof(std::uint8_t);
    type_size_[ArrayElemType::T_INT8] = sizeof(std::int8_t);

    type_size_[ArrayElemType::T_UINT16] = sizeof(std::uint16_t);
    type_size_[ArrayElemType::T_INT16] = sizeof(std::int16_t);

    type_size_[ArrayElemType::T_UINT32] = sizeof(std::uint32_t);
    type_size_[ArrayElemType::T_INT32] = sizeof(std::int32_t);

    type_size_[ArrayElemType::T_UINT64] = sizeof(std::uint64_t);
    type_size_[ArrayElemType::T_INT64] = sizeof(std::int64_t);

    type_size_[ArrayElemType::T_FLOAT] = sizeof(float);
    type_size_[ArrayElemType::T_DOUBLE] = sizeof(double);
}

std::vector<uint8_t> ArrayFileReader::readDataBytes() const
{
    auto const md = readMetadata();
    auto const files = dataFileNames();
    auto data = std::vector<uint8_t>{};
    data.reserve(numBytes(md, files.size()));

    for (auto const &filename: files) {
        auto const file_data = readDataBytesFile(filename, md);
        data.insert(std::end(data), std::begin(file_data),
                    std::end(file_data));
    }
    return data;
}

std::vector<std::string> ArrayFileReader::dataFileNames() const
{
    if (series_) {
        return getAllFiles(filename_);
    }
    else {
        return std::vector<std::string>{filename_};
    }
}

template<>
std::vector<std::uint8_t> ArrayFileReader::readData() const
{
    return readDataBytes();
}

std::vector<std::uint8_t> ArrayFileReader::readDataFromBinFile(std::string const &filename,
                                                               ArrayMetadata const &md,
                                                               size_t num_bytes_skip) const
{
    auto file = std::ifstream{};
    file.open(filename.c_str(), std::ios::binary | std::ios::in);
    if (!file)
        throw std::runtime_error{"File " + filename_ + " cannot be opened for reading data.\n"};

    // Get the file size.
    file.seekg(0, std::ios::end);
    auto const file_size_bytes = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);

    if (num_bytes_skip > file_size_bytes) {
        throw std::runtime_error{"Error during the reading of file " + filename + ".\n" +
            std::string{"The actual file size :: " + std::to_string(file_size_bytes) + " bytes.\n"}
                                     +
                                         std::string{"The number of bytes to be skipped :: "
                                                         + std::to_string(num_bytes_skip)
                                                         + " bytes.\n"}};
    }

    auto const num_bytes_req = numBytes(md);
    if (num_bytes_req != (file_size_bytes - num_bytes_skip)) {
        throw std::runtime_error{"Error during the reading of file " + filename + ".\n" +
            std::string{"The actual file size :: " + std::to_string(file_size_bytes) + " bytes.\n"}
                                     +
                                         std::string{"The number of bytes requested :: "
                                                         + std::to_string(num_bytes_req)
                                                         + " bytes.\n"}};
    }

    // Skip the bytes.
    file.seekg(num_bytes_skip, std::ios::beg);

    //Read bytes.
    auto data_bytes = std::vector<uint8_t>{};
    data_bytes.assign(num_bytes_req, 0);
    file.read(static_cast<char *>(static_cast<void *>(data_bytes.data())), num_bytes_req);
    file.close();

    return data_bytes;
}

}//GCT::IO namespaces.
