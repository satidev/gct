#include "catch.hpp"
#include "../../../../src/cpp/io/file_writer/den_file_writer.h"
#include "../../../../src/cpp/io/file_reader/den_file_reader.h"
#include "../../../test_data.h"
#include "../file_reader/test_filenames.h"
#include "../file_reader/array_file_reader_test_fix.h"

namespace GCT::Test
{
TEST_CASE("Exception is thrown due to the invalid path of Dennerlein file",
          "[io][den_file_writer]")
{
    auto const data = std::vector<float>{1.0f};
    auto const fname = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"invalid_dir"} / std::string{"raw.den"};
    auto const meta_data = IO::ArrayFileWriter::DenMetaData{1u, 1u, 1u};
    REQUIRE_THROWS_AS(IO::ArrayFileWriter::write(meta_data, data, fname), std::runtime_error);
}

TEST_CASE("Exception is thrown due to the mismatch between data and metadata of the Dennerlein file",
          "[io][den_file_writer]")
{
    auto const data = std::vector<float>{1.0f};
    auto const fname = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"raw.den"};
    auto const meta_data = IO::ArrayFileWriter::DenMetaData{10u, 1u, 1u};
    REQUIRE_THROWS_AS(IO::ArrayFileWriter::write(meta_data, data, fname), std::runtime_error);
}


TEMPLATE_TEST_CASE_SIG("Dennerlein file is written correctly",
                       "[io][den_file_writer]",
                       ((typename ElemType, unsigned Dim), ElemType, Dim),
                       (float, 2u), (float, 3u))
{
    auto const &[first_file, first_sec] = testFileNames<IO::DenFileReader>();
    auto const fix = ArrayFileReaderTestFix<ElemType, Dim>{first_file, first_sec};
    auto const data = fix.dataFirstFile();
    auto const metadata_arr = fix.metaData();
    auto const metadata_den = IO::ArrayFileWriter::DenMetaData{
        static_cast<unsigned short>(metadata_arr.num_rows),
        static_cast<unsigned short>(metadata_arr.num_cols),
        static_cast<unsigned short>(metadata_arr.num_slices)
    };
    auto const fname = std::filesystem::path{TEST_FILE_DIR} / std::string{"temp.den"};

    REQUIRE_NOTHROW(IO::ArrayFileWriter::write(metadata_den, data, fname));

    //Read the written file and verify the data.
    auto const reader = IO::DenFileReader{fname};
    REQUIRE(data == reader.readData<ElemType>());

    std::filesystem::remove(fname);
}
}//GCT::Test namespaces.