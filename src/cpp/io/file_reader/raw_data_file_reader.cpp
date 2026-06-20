#include "raw_data_file_reader.h"
#include <fstream>

namespace GCT::IO
{
std::vector<uint8_t> RawDataFileReader::readDataBytesFile(std::string const &data_filename,
                                                          ArrayMetadata const &md) const
{
    return readDataFromBinFile(data_filename, md, 0u);
}

}//GCT::IO namespaces.
