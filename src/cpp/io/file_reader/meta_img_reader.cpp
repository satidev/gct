#include "meta_img_reader.h"
#include <fstream>
#include <string>
#include <sstream>
#include "raw_data_file_reader.h"
#include "../string_utils.h"

namespace GCT::IO
{
MetaImgFileReader::MetaImgFileReader(std::string filename, bool series)
    :
    ArrayFileReader{std::move(filename), series}
{
    setTypeMap();
    auto const data_file = readElementDataFile();
    auto const header_filename = filename_;
    if (data_file == "LOCAL") {//Header and data are in the same file
        data_fname_ = header_filename;
    }
    else {//Header and data are in different files.
        auto const dir_path = std::filesystem::path{header_filename}.parent_path();
        auto const raw_filename = dir_path / data_file;
        if (!doesFileExist(raw_filename)) {
            throw std::runtime_error{"Data filename " +
                raw_filename.string() + " is not found.\n" +
                "Header and data files should be in the same directory.\n"};
        }
        data_fname_ = raw_filename;
    }
}

ArrayMetadata MetaImgFileReader::readMetadata() const
{
    auto ifs = std::ifstream{};
    ifs.open(filename_.c_str(), std::ios::in);

    auto data = ArrayMetadata{};
    data.num_rows = 1u;
    data.num_cols = 1u;
    data.num_slices = 1u;

    if (ifs) {
        auto line = std::string{};

        while (!ifs.eof()) {
            std::getline(ifs, line);

            //Check the last header tag of mha file.
            if (line.find("ElementDataFile = LOCAL") != std::string::npos) {
                break;
            }

            if (line.find("DimSize ") != std::string::npos) {
                auto stream = std::stringstream{line.substr(line.find('=') + 1)};
                auto word = std::string{};
                auto dims = std::vector<int>{};
                while (stream >> word) { // Extract word from the stream.
                    dims.push_back(std::stoi(word));
                }
                data.num_rows = static_cast<unsigned>(dims[1]);
                data.num_cols = static_cast<unsigned>(dims[0]);
                if (dims.size() == 3UL) {
                    data.num_slices = static_cast<unsigned>(dims[2]);
                }

            }
            if (line.find("ElementType ") != std::string::npos) {
                auto const val_str = trim(line.substr(line.find('=') + 1));
                auto const iter = meta_types_.find(val_str);
                if (iter != meta_types_.end()) {
                    data.type = meta_types_.find(val_str)->second;
                }
                else {
                    throw std::runtime_error{
                        "Error during reading meta image file " + filename_ + "\n" +
                            val_str + " is not a valid ElementType\n"};
                }
            }
        }

        ifs.close();
    }
    return data;
}
std::vector<uint8_t> MetaImgFileReader::readDataBytesFile(std::string const &data_filename,
                                                          ArrayMetadata const &md) const
{
    if (data_fname_ != filename_) {
        return readDataFromBinFile(data_filename, md, 0u);
    }
    else {
        return readDataBytesMHAFile(data_filename, md);
    }
}

void MetaImgFileReader::setTypeMap()
{
    meta_types_["MET_UCHAR"] = ArrayElemType::T_UINT8;
    meta_types_["MET_CHAR"] = ArrayElemType::T_INT8;

    meta_types_["MET_USHORT"] = ArrayElemType::T_UINT16;
    meta_types_["MET_SHORT"] = ArrayElemType::T_INT16;

    meta_types_["MET_UINT"] = ArrayElemType::T_UINT32;
    meta_types_["MET_INT"] = ArrayElemType::T_INT32;

    meta_types_["MET_ULONG_LONG"] = ArrayElemType::T_UINT64;
    meta_types_["MET_LONG_LONG"] = ArrayElemType::T_INT64;

    meta_types_["MET_FLOAT"] = ArrayElemType::T_FLOAT;
    meta_types_["MET_DOUBLE"] = ArrayElemType::T_DOUBLE;
}
std::vector<std::string> MetaImgFileReader::dataFileNames() const
{
    if (series_) {
        return getAllFiles(data_fname_);
    }
    else {
        return std::vector<std::string>{data_fname_};
    }
}

std::string MetaImgFileReader::readElementDataFile() const
{
    auto data_file = std::string{};
    auto ifs = std::ifstream{};
    ifs.open(filename_.c_str(), std::ios::in);

    if (ifs) {
        auto line = std::string{};

        while (!ifs.eof()) {
            std::getline(ifs, line);

            if (line.find("ElementDataFile") != std::string::npos) {
                data_file = trim(line.substr(line.find('=') + 1));
                break;
            }
        }
        ifs.close();
    }
    return data_file;
}
std::vector<uint8_t> MetaImgFileReader::readDataBytesMHAFile(std::string const &data_filename,
                                                             ArrayMetadata const &md) const
{
    auto file = std::ifstream{};
    file.open(data_filename.c_str(), std::ios::in);

    // Skip the text header information.
    auto line = std::string{};
    do {
        std::getline(file, line);
    }
    while (line != "ElementDataFile = LOCAL");

    // Read the data bytes
    auto data_bytes = std::vector<uint8_t>{};
    data_bytes.assign(numBytes(md), 0);
    file.read(static_cast<char *>(static_cast<void *>(data_bytes.data())), data_bytes.size());

    return data_bytes;
}

}//GCT::IO namespaces.
