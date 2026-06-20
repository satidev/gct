#include "catch.hpp"
#include "../../../../src/cpp/io/file_reader/meta_img_reader.h"
#include "test_filenames.h"
#include "array_file_reader_test_fix.h"

namespace GCT::Test
{

TEMPLATE_TEST_CASE_SIG("Meta image MHD file is read correctly",
                       "[io][array_file_reader]",
                       ((typename ElemType, unsigned Dim, typename FileReader), ElemType, Dim, FileReader),
                       (float, 3u, IO::MetaImgFileReader))

{
    auto const fix = ArrayFileReaderTestFix<ElemType, Dim>{"01.mhd", "02.mhd"};

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

TEMPLATE_TEST_CASE_SIG("A series of meta image MHD files is read correctly",
                       "[io][array_file_reader]",
                       ((typename ElemType, unsigned Dim, typename FileReader), ElemType, Dim, FileReader),
                       (float, 3u, IO::MetaImgFileReader))

{
    auto const fix = ArrayFileReaderTestFix<ElemType, Dim>{"01.mhd", "02.mhd"};

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
