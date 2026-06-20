#ifndef ARRAY_FILE_READER_H
#define ARRAY_FILE_READER_H

#include <string>
#include "../metadata.h"
#include <vector>
#include "../file_utils.h"
#include <iostream>
#include <map>

namespace GCT::IO
{
//Abstract base class for reading files containing numeric array element values.
class ArrayFileReader
{
public:
    ArrayFileReader(std::string filename, bool series);

    //Read metadata, implementation varies depending on the type of file.
    virtual ArrayMetadata readMetadata() const = 0;

    //Read data from the file or files and convert the type of data element.
    template<typename OpType>
    std::vector<OpType> readData() const;

    virtual ~ArrayFileReader() = default;

protected:
    std::string filename_;
    bool series_;
    std::map<ArrayElemType, size_t> type_size_;

    //Read bytes from a single data file, file-specific implementation.
    virtual std::vector<uint8_t> readDataBytesFile(
        std::string const &data_filename, ArrayMetadata const &md) const = 0;

    //Returns name/names of the files where the array elements are stored.
    //Override if the header and data are in separate files.
    virtual std::vector<std::string> dataFileNames() const;

    size_t numBytes(ArrayMetadata const &md,
                    [[maybe_unused]] size_t num_files = 1) const
    {
        auto const num_elem_data = md.num_rows * md.num_cols *
            md.num_slices;
        auto const elem_size_bytes = type_size_.find(md.type)->second;
        return num_elem_data * elem_size_bytes;
    }

    std::vector<std::uint8_t> readDataFromBinFile(std::string const &filename,
                                                  ArrayMetadata const &md,
                                                  size_t num_bytes_skip) const;

private:

    void setTypeInfo();

    //Read data from the file or files as an array of bytes.
    std::vector<uint8_t> readDataBytes() const;

    //Convert array of bytes via type conversion.
    template<typename OpType>
    std::vector<OpType> convertData(std::vector<uint8_t> const &bytes,
                                    ArrayElemType type) const;
    template<typename IpType, typename OpType>
    void convertBytesToNumbers(IpType const *ip_data,
                               std::vector<OpType> &op_data) const;
};

template<typename OpType>
std::vector<OpType> ArrayFileReader::readData() const
{
    auto const md = readMetadata();
    auto const data_bytes = readDataBytes();
    return convertData<OpType>(data_bytes, md.type);
}

template<typename IpType, typename OpType>
void ArrayFileReader::convertBytesToNumbers(IpType const *ip_data,
                                            std::vector<OpType> &op_data) const
{
    for (auto elem_idx = size_t{0}; elem_idx < op_data.size(); elem_idx++) {
        op_data[elem_idx] = static_cast<OpType>(ip_data[elem_idx]);
    }
}

template<typename OpType>
std::vector<OpType> ArrayFileReader::convertData(
    std::vector<uint8_t> const &bytes, ArrayElemType type) const
{
    auto const elem_size_bytes = type_size_.find(type)->second;
    auto const num_elem = bytes.size() / elem_size_bytes;
    if (bytes.size() % elem_size_bytes != 0) {
        std::cerr
            << "The byte data cannot be converted to numerical array due to the invalid number of elements"
            << std::endl;
        return std::vector<OpType>{};
    }

    auto vec = std::vector<OpType>(num_elem);

    switch (type) {
        case ArrayElemType::T_UINT8:
            convertBytesToNumbers(
                static_cast<std::uint8_t const *>(static_cast<void const *> (bytes.data())), vec);
            break;
        case ArrayElemType::T_INT8:
            convertBytesToNumbers(
                static_cast<std::int8_t const *>(static_cast<void const *> (bytes.data())), vec);
            break;

        case ArrayElemType::T_UINT16:
            convertBytesToNumbers(
                static_cast<std::uint16_t const *>(static_cast<void const *> (bytes.data())), vec);
            break;
        case ArrayElemType::T_INT16:
            convertBytesToNumbers(
                static_cast<std::int16_t const *>(static_cast<void const *> (bytes.data())), vec);
            break;

        case ArrayElemType::T_UINT32:
            convertBytesToNumbers(
                static_cast<std::uint32_t const *>(static_cast<void const *> (bytes.data())), vec);
            break;
        case ArrayElemType::T_INT32:
            convertBytesToNumbers(
                static_cast<std::int32_t const *>(static_cast<void const *> (bytes.data())), vec);
            break;

        case ArrayElemType::T_UINT64:
            convertBytesToNumbers(
                static_cast<std::uint64_t const *>(static_cast<void const *> (bytes.data())), vec);
            break;
        case ArrayElemType::T_INT64:
            convertBytesToNumbers(
                static_cast<std::int64_t const *>(static_cast<void const *> (bytes.data())), vec);
            break;

        case ArrayElemType::T_FLOAT:
            convertBytesToNumbers(
                static_cast<float const *>(static_cast<void const *> (bytes.data())), vec);
            break;
        case ArrayElemType::T_DOUBLE:
            convertBytesToNumbers(
                static_cast<double const *>(static_cast<void const *> (bytes.data())), vec);
            break;
    }
    return vec;
}

}//GCT::IO namespaces.

#endif //ARRAY_FILE_READER_H

