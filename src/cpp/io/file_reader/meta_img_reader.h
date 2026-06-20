#ifndef META_IMG_READER_H
#define META_IMG_READER_H

#include <utility>
#include "../metadata.h"
#include "../file_utils.h"
#include "array_file_reader.h"
#include <regex>

namespace GCT::IO
{
class MetaImgFileReader: public ArrayFileReader
{
public:
    explicit MetaImgFileReader(std::string filename, bool series = false);

    ArrayMetadata readMetadata() const final;

protected:

    std::vector<uint8_t> readDataBytesFile(
        std::string const &data_filename, ArrayMetadata const &md) const final;

    std::vector<std::string> dataFileNames() const override;

private:
    //If the data and header are in separate files,
    //data file contains the data and the input file contains the header.
    std::string data_fname_;

    //Reads the value corresponding to the tag "ElementDataFile".
    [[nodiscard]] std::string readElementDataFile() const;

    //Read data bytes from a mha file where the header and data are
    // in a single file. The function skips the text header and reads
    //only the binary data.
    [[nodiscard]] std::vector<uint8_t> readDataBytesMHAFile(
        std::string const &data_filename, ArrayMetadata const &md) const;

    //Map between meta type strings (e.g., MET_FLOAT) and the supported data types.
    std::map<std::string, ArrayElemType> meta_types_;

    void setTypeMap();
};

}//GCT::IO namespaces.


#endif //META_IMG_READER_H


