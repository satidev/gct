#include "catch.hpp"
#include "../../../src/cpp/io/string_utils.h"

namespace GCT::Test
{
TEST_CASE("Returns empty string if an empty list of strings is passed",
          "[io][string_utils]")
{
    auto string_list = std::vector<std::string>{};
    REQUIRE(IO::join(string_list, std::string{","}).empty());
}

TEST_CASE("R, T and X are joined to R,T,X", "[io][string_utils]")
{
    auto string_list = std::vector<std::string>{};
    string_list.emplace_back("R");
    string_list.emplace_back("T");
    string_list.emplace_back("X");

    REQUIRE(std::string{"R,T,X"} == IO::join(string_list, std::string{","}));
}

TEST_CASE("Leading and trailing spaces are removed from the string", "[io][string_utils]")
{
    auto const str = std::string{" TEMP     "};
    REQUIRE(std::string{"TEMP"} == IO::trim(str));
}

TEST_CASE("Each character is converted to lowercase", "[io][string_utils]")
{
    REQUIRE(IO::toLower(".mha") == std::string{".mha"});
    REQUIRE(IO::toLower(".1mha") == std::string{".1mha"});
    REQUIRE(IO::toLower(".1MHA") == std::string{".1mha"});
    REQUIRE(IO::toLower(std::string{}).empty());
}

TEST_CASE("Each character is converted to uppercase", "[io][string_utils]")
{
    REQUIRE(IO::toUpper(".MHA") == std::string{".MHA"});
    REQUIRE(IO::toUpper(".1mha") == std::string{".1MHA"});
    REQUIRE(IO::toUpper(std::string{}).empty());
}

}//GCT::Test namespaces.


