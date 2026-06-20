#include "catch.hpp"
#include "../../../../src/cpp/io/file_reader/den_file_reader.h"
#include "../../../../src/cpp/io/file_reader/meta_img_reader.h"
#include "test_filenames.h"
#include "array_file_reader_test_fix.h"

namespace GCT::Test
{

TEMPLATE_TEST_CASE("Exception is thrown since the file does not exist",
                   "[io][array_file_reader]",
                   IO::DenFileReader,
                   IO::MetaImgFileReader)
{
    REQUIRE_THROWS_AS(TestType{std::string{}}, std::runtime_error);
}


TEMPLATE_TEST_CASE_SIG("Image file is read correctly",
                       "[io][array_file_reader]",
                       ((typename ElemType, unsigned Dim, typename FileReader), ElemType, Dim, FileReader),
                       (float, 2u, IO::DenFileReader),
                       (float, 3u, IO::DenFileReader),
                       (float, 2u, IO::MetaImgFileReader),
                       (float, 3u, IO::MetaImgFileReader))

{
    auto const &[first_file, first_sec] = testFileNames<FileReader>();
    auto const fix = ArrayFileReaderTestFix<ElemType, Dim>{first_file, first_sec};

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


TEMPLATE_TEST_CASE_SIG("A series of image files is read correctly",
                       "[io][array_file_reader]",
                       ((typename ElemType, unsigned Dim, typename FileReader), ElemType, Dim, FileReader),
                       (float, 2u, IO::DenFileReader),
                       (float, 3u, IO::DenFileReader),
                       (float, 2u, IO::MetaImgFileReader),
                       (float, 3u, IO::MetaImgFileReader))

{
    auto const &[first_file, first_sec] = testFileNames<FileReader>();
    auto const fix = ArrayFileReaderTestFix<ElemType, Dim>{first_file, first_sec};

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

