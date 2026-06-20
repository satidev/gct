#include "dcm_file_reader.h"
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>

namespace GCT::IO
{
ArrayMetadata DcmFileReader::readMetadata() const
{
    auto file = DcmFileFormat{};
    auto status = file.loadFile(filename_.c_str());
    if (!status.good()) {
        throw std::runtime_error{"Error during the loading of DICOM file " + filename_ + "\n"};
    }

    auto md = ArrayMetadata{};

    auto num_rows_str = OFString{};
    status = file.getDataset()->findAndGetOFStringArray(DCM_Rows, num_rows_str);
    if (status.good()) {
        md.num_rows = std::stoi(num_rows_str.c_str());
    }
    else {
        throw std::runtime_error{
            "Error during the reading of DICOM tag corresponding to the number of rows.\n"};
    }

    auto num_col_str = OFString{};
    status = file.getDataset()->findAndGetOFStringArray(DCM_Columns, num_col_str);
    if (status.good()) {
        md.num_cols = std::stoi(num_col_str.c_str());
    }
    else {
        throw std::runtime_error{
            "Error during the reading of DICOM tag corresponding to the number of columns.\n"};
    }

    auto num_slices_str = OFString{};
    status = file.getDataset()->findAndGetOFStringArray(DCM_NumberOfFrames, num_slices_str);
    if (status.good()) {
        md.num_slices = std::stoi(num_slices_str.c_str());
    }
    else {
        md.num_slices = 1u;
    }

    auto type_str = OFString{};
    status = file.getDataset()->findAndGetOFStringArray(DCM_PixelRepresentation, type_str);
    if (status.good()) {
        auto const iter = dcm_types_.find(
            static_cast<EP_Representation const>(std::stoi(type_str.c_str())));
        if (iter != dcm_types_.end()) {
            md.type = iter->second;
        }
        else {
            throw std::runtime_error{"Pixel type of the DICOM image is not supported.\n"};
        }
    }
    else {
        throw std::runtime_error{
            "Error during the reading of DICOM tag corresponding to the pixel type.\n"};
    }
    return md;
}
std::vector<uint8_t>
DcmFileReader::readDataBytesFile(std::string const &data_filename,
                                 ArrayMetadata const &md) const
{
    auto file = DcmFileFormat{};
    auto status = file.loadFile(data_filename.c_str());
    if (!status.good()) {
        throw std::runtime_error{"Error during the loading of DICOM file " + data_filename + "\n"};
    }

    auto image = std::make_unique<DicomImage>(data_filename.c_str());
    if (!image || image->getStatus() != EIS_Normal) {
        throw std::runtime_error{
            std::string{"Error during the loading of DICOM image from the file " +
                data_filename}};
    }
    auto const interm_data = image->getInterData();
    auto data_bytes = static_cast<std::uint8_t const *const>(interm_data->getData());
    return std::vector<uint8_t>{data_bytes, data_bytes + numBytes(md)};
}
void DcmFileReader::setTypeMap()
{
    dcm_types_[EPR_Uint8] = ArrayElemType::T_UINT8;
    dcm_types_[EPR_MinUnsigned] = ArrayElemType::T_UINT8;

    dcm_types_[EPR_Sint8] = ArrayElemType::T_INT8;
    dcm_types_[EPR_MinSigned] = ArrayElemType::T_INT8;

    dcm_types_[EPR_Uint16] = ArrayElemType::T_UINT16;
    dcm_types_[EPR_Sint16] = ArrayElemType::T_INT16;

    dcm_types_[EPR_Uint32] = ArrayElemType::T_UINT32;
    dcm_types_[EPR_MaxUnsigned] = ArrayElemType::T_UINT32;

    dcm_types_[EPR_Sint32] = ArrayElemType::T_INT32;
    dcm_types_[EPR_MaxSigned] = ArrayElemType::T_INT32;
}
}//GCT::IO namespaces.


