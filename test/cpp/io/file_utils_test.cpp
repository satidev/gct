#include "catch.hpp"
#include "../../../src/cpp/io/file_utils.h"
#include <filesystem>
#include "../../test_data.h"

namespace GCT::Test
{
TEST_CASE("catch_cat.hpp does not exist", "[io][file_utils]")
{
    REQUIRE_FALSE(IO::doesFileExist("catch_cat.hpp"));
}

TEST_CASE("Test file in the directory exists", "[io][file_utils]")
{
    auto const filename = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"io"} / std::string{"float_2d"} / std::string{"01.bin"};
    if (std::filesystem::exists(filename)) {
        REQUIRE(IO::doesFileExist(filename));
    }
}

TEST_CASE("File size is 24 bytes", "[io][file_utils]")
{
    auto const filename = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"io"} / std::string{"float_2d"} / std::string{"01.bin"};
    if (std::filesystem::exists(filename)) {
        REQUIRE(IO::fileSize(filename) == 24UL);
    }
}

TEST_CASE("Exception is thrown since the file catch_cat.hpp does not exist",
          "[io][file_utils]")
{
    REQUIRE_THROWS_AS(IO::getAllFiles("catch_cat.hpp"), std::runtime_error);
}

TEST_CASE("TEST directory is writable", "[io][file_utils]")
{
    SECTION("Writable directory") {
        REQUIRE(IO::isWritable(TEST_FILE_DIR));
    }
}

}//GCT::Test namespace.


