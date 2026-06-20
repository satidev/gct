#include "catch.hpp"
#include "../../../../src/cpp/io/file_reader/raw_data_file_reader.h"
#include "test_filenames.h"
#include "array_file_reader_test_fix.h"

namespace GCT::Test
{
TEST_CASE("Exception is thrown since the binary file does not exist",
          "[io][array_file_reader]")
{
    auto const fix = ArrayFileReaderTestFix<float, 2u>{std::string{}, std::string{}};
    REQUIRE_THROWS_AS(GCT::IO::RawDataFileReader(std::string{}, fix.metaData()), std::runtime_error);
}

TEMPLATE_TEST_CASE_SIG("Image file is read correctly",
                       "[io][array_file_reader]",
                       ((typename ElemType, unsigned Dim, typename FileReader), ElemType, Dim, FileReader),
                       (float, 2u, IO::RawDataFileReader),
                       (float, 3u, IO::RawDataFileReader))

{
    auto const &[first_file, first_sec] = testFileNames<FileReader>();
    auto const fix = ArrayFileReaderTestFix<ElemType, Dim>{first_file, first_sec};

    if (fix.areFilesFound()) {
        SECTION("No exception is thrown since the file exists") {
            REQUIRE_NOTHROW(FileReader{fix.nameFirstFile(), fix.metaData()});
        }

        auto const reader = FileReader{fix.nameFirstFile(), fix.metaData()};

        SECTION("Meta data is read correctly") {
            REQUIRE(fix.areEqual(reader.readMetadata()));
        }

        SECTION("Data from the file is read correctly") {
            auto const data = reader.template readData<ElemType>();
            REQUIRE(data == fix.dataFirstFile());
        }
    }
}

TEMPLATE_TEST_CASE_SIG("A series of image files is read correctly",
                       "[io][array_file_reader]",
                       ((typename ElemType, unsigned Dim, typename FileReader), ElemType, Dim, FileReader),
                       (float, 2u, IO::RawDataFileReader),
                       (float, 3u, IO::RawDataFileReader))

{
    auto const &[first_file, first_sec] = testFileNames<FileReader>();
    auto const fix = ArrayFileReaderTestFix<ElemType, Dim>{first_file, first_sec};

    if (fix.areFilesFound()) {
        SECTION("No exception is thrown since the file exists") {
            REQUIRE_NOTHROW(FileReader{fix.nameFirstFile(), fix.metaData(), true});
        }

        auto const reader = FileReader{fix.nameFirstFile(), fix.metaData(), true};

        SECTION("Meta data is read correctly") {
            REQUIRE(fix.areEqual(reader.readMetadata()));
        }

        SECTION("Data from the file is read correctly") {
            auto const data = reader.template readData<ElemType>();
            REQUIRE(data == fix.dataAllFiles());
        }
    }
}

}//GCT::Test namespaces.
