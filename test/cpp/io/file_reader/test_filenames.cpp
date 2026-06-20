#include "test_filenames.h"
#include "../../../../src/cpp/io/file_reader/raw_data_file_reader.h"
#include "../../../../src/cpp/io/file_reader/den_file_reader.h"
#include "../../../../src/cpp/io/file_reader/meta_img_reader.h"

namespace GCT::Test
{
template<>
std::pair<std::string, std::string> testFileNames<IO::RawDataFileReader>()
{
    return std::make_pair(std::string{"01.bin"},
                          std::string{"02.bin"});
}

template<>
std::pair<std::string, std::string> testFileNames<IO::DenFileReader>()
{
    return std::make_pair(std::string{"01.den"},
                          std::string{"02.den"});
}

template<>
std::pair<std::string, std::string> testFileNames<IO::MetaImgFileReader>()
{
    return std::make_pair(std::string{"01.mha"},
                          std::string{"02.mha"});
}

/*
template<>
std::pair<std::string, std::string> testFileNames<IO::DcmFileReader>()
{
    return std::make_pair(std::string{"01.dcm"},
                          std::string{"02.dcm"});
}
*/
}//GCT::Test namespace.
