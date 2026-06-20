#ifndef DCM_FILE_READER_H
#define DCM_FILE_READER_H

#include "../metadata.h"
#include "../file_utils.h"
#include "array_file_reader.h"
#include <dcmtk/dcmimgle/diutils.h>

namespace GCT::IO
{
class DcmFileReader: public ArrayFileReader
{
public:
    explicit DcmFileReader(std::string filename, bool series = false)
        :
        ArrayFileReader{std::move(filename), series}
    {
        setTypeMap();
    }

    ArrayMetadata readMetadata() const final;

protected:
    std::vector<uint8_t> readDataBytesFile(
        std::string const &data_filename, ArrayMetadata const &md) const final;

private:
    //Map between dicom pixel types and the supported data types in GCT.
    std::map<EP_Representation, ArrayElemType> dcm_types_;
    void setTypeMap();
};

}//GCT::IO namespaces.

#endif //DCM_FILE_READER_H


