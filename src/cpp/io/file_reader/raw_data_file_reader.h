#ifndef RAW_DATA_FILE_READER_H
#define RAW_DATA_FILE_READER_H

#include <utility>

#include "../metadata.h"
#include "../file_utils.h"
#include "array_file_reader.h"

namespace GCT::IO
{
//To read binary files containing only arrays without any header.
class RawDataFileReader: public ArrayFileReader
{
public:
    explicit RawDataFileReader(std::string filename, ArrayMetadata md,
                               bool series = false)
        :
        ArrayFileReader{std::move(filename), series},
        metadata_{md}
    {}

    ArrayMetadata readMetadata() const final
    {
        return metadata_;
    }

protected:

    std::vector<uint8_t> readDataBytesFile(
        std::string const &data_filename, ArrayMetadata const &md) const final;

private:
    ArrayMetadata metadata_;
};

}//GCT::IO namespaces.

#endif //RAW_DATA_FILE_READER_H


