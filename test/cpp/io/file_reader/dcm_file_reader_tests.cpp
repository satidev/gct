#include "catch.hpp"
#include "../../../../src/cpp/io/file_reader/dcm_file_reader.h"
#include "test_filenames.h"
#include "array_file_reader_test_fix.h"

namespace GCT::Test
{
TEMPLATE_TEST_CASE("Exception is thrown since the file does not exist",
                   "[io][array_file_reader]",
                   IO::DcmFileReader)
{
    REQUIRE_THROWS_AS(TestType{std::string{}}, std::runtime_error);
}


TEMPLATE_TEST_CASE_SIG("DICOM image file is read correctly",
                       "[io][array_file_reader]",
                       ((typename ElemType, unsigned Dim, typename FileReader), ElemType, Dim, FileReader),
                       (std::uint8_t, 2u, IO::DcmFileReader))

{
    auto const fix = ArrayFileReaderTestFix<ElemType, Dim>{"01.dcm", "02.dcm"};

    if (fix.areFilesFound()) {
        SECTION("No exception is thrown since the file exists") {
            REQUIRE_NOTHROW(FileReader{fix.nameFirstFile()});
        }

        auto const reader = FileReader{fix.nameFirstFile()};

        SECTION("Meta data is read correctly") {
            REQUIRE(fix.areEqual(reader.readMetadata()));
        }

        SECTION("Data from the file is read correctly") {
            auto const data = reader.template readData<ElemType>();
            REQUIRE(data == fix.dataFirstFile());
        }
    }
}

TEMPLATE_TEST_CASE_SIG("A series of DICOM image files is read correctly",
                       "[io][array_file_reader]",
                       ((typename ElemType, unsigned Dim, typename FileReader), ElemType, Dim, FileReader),
                       (std::uint8_t, 2u, IO::DcmFileReader))

{
    auto const fix = ArrayFileReaderTestFix<ElemType, Dim>{"01.dcm", "02.dcm"};

    if (fix.areFilesFound()) {
        SECTION("No exception is thrown since the file exists") {
            REQUIRE_NOTHROW(FileReader{fix.nameFirstFile(), true});
        }

        auto const reader = FileReader{fix.nameFirstFile(), true};

        SECTION("Meta data is read correctly") {
            REQUIRE(fix.areEqual(reader.readMetadata()));
        }

        SECTION("Data from the file is read correctly") {
            auto const data = reader.template readData<ElemType>();
            REQUIRE(data == fix.dataAllFiles());
        }
    }
}

}//GCT::Test namespace.
