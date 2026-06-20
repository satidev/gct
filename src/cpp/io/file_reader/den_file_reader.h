#ifndef DEN_FILE_READER_H
#define DEN_FILE_READER_H

#include <utility>

#include "../metadata.h"
#include "../file_utils.h"
#include "array_file_reader.h"

namespace GCT::IO
{
//Can only be read if the data elements are of the following types:
//1. unsigned char, unsigned short, float (float32), double (float64)
class DenFileReader: public ArrayFileReader
{
public:
    explicit DenFileReader(std::string filename, bool series = false)
        :
        ArrayFileReader{std::move(filename), series}
    {}

    ArrayMetadata readMetadata() const final;

protected:

    std::vector<uint8_t> readDataBytesFile(
        std::string const &data_filename, ArrayMetadata const &md) const final;

private:

    std::pair<ArrayElemType, unsigned> dataType(
        std::array<unsigned short, 3UL> const &header, size_t file_size) const;

};

}//GCT::IO namespaces.

#endif //DEN_FILE_READER_H


