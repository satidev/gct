#include "catch.hpp"
#include "../../../../src/cpp/io/file_reader/raw_data_file_reader.h"
#include "../../../../src/cpp/io/file_writer/raw_data_file_writer.h"
#include "../file_reader/test_filenames.h"
#include "../file_reader/array_file_reader_test_fix.h"

namespace GCT::Test
{
TEST_CASE("Exception is thrown due to the invalid path",
          "[io][raw_data_file_writer]")
{
    auto const data = std::vector<float>{1.0f};
    auto const fname = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"invalid_dir"}/std::string{"raw.bin"};
    REQUIRE_THROWS_AS(IO::ArrayFileWriter::write(data, fname), std::runtime_error);
}

TEMPLATE_TEST_CASE_SIG("Binary file is written correctly",
                       "[io][raw_data_file_writer]",
                       ((typename ElemType, unsigned Dim), ElemType, Dim),
                       (float, 2u), (float, 3u))
{
    auto const &[first_file, first_sec] = testFileNames<IO::RawDataFileReader>();
    auto const fix = ArrayFileReaderTestFix<ElemType, Dim>{first_file, first_sec};
    auto const data = fix.dataFirstFile();
    auto const fname = std::filesystem::path{TEST_FILE_DIR}/std::string{"temp.bin"};

    REQUIRE_NOTHROW(IO::ArrayFileWriter::write(data, fname));

    //Read the written file and verify the data.
    auto const reader = IO::RawDataFileReader{fname, fix.metaData()};
    REQUIRE(data == reader.readData<ElemType>());

    std::filesystem::remove(fname);
}

}//GCT::Test namespaces.
