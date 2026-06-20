#include "catch.hpp"
#include "../../../src/cpp/data/proj_meta_info.h"
#include "../../matchers.h"

namespace GCT::Test
{
TEST_CASE("Returns 2 as the number of pixels of 1D projection",
          "[data][proj_meta_info]")
{
    auto constexpr size = Data::ProjSize<1u>{2u};
    REQUIRE(Data::numProjPixels(size) == 2u);
}

TEST_CASE("Returns 6 as the number of pixels of 2D projection",
          "[data][proj_meta_info]")
{
    auto constexpr size = Data::ProjSize<2u>{2u, 3u};
    REQUIRE(Data::numProjPixels(size) == 6u);
}

TEST_CASE("Returns 1 as the number of rows of 1D projection",
          "[data][proj_meta_info]")
{
    auto constexpr size = Data::ProjSize<1u>{2u};
    REQUIRE(Data::numProjRows(size) == 1u);
}

TEST_CASE("Returns 3 as the number of rows of 2D projection",
          "[data][proj_meta_info]")
{
    auto constexpr size = Data::ProjSize<2u>{2u, 3u};
    REQUIRE(Data::numProjRows(size) == 3u);
}

TEST_CASE("Returns an array containing 2",
          "[data][proj_meta_info]")
{
    auto constexpr size = Data::ProjSize<1u>{2u};
    REQUIRE(Data::projSizeToArr(size) == std::array{2u});
}

TEST_CASE("Returns an array containing 2 and 3",
          "[data][proj_meta_info]")
{
    auto constexpr size = Data::ProjSize<2u>{2u, 3u};
    REQUIRE(Data::projSizeToArr(size) == std::array{2u, 3u});
}

TEST_CASE("Returns true since the projection sizes are equal",
          "[data][proj_meta_info]")
{
    REQUIRE(Data::isEqual(Data::ProjSize<1u>{2u}, Data::ProjSize<1u>{2u}));
    REQUIRE(Data::isEqual(Data::ProjSize<2u>{2u, 3u},
                          Data::ProjSize<2u>{2u, 3u}));
}

TEST_CASE("Returns false since the projection sizes are not equal",
          "[data][proj_meta_info]")
{
    REQUIRE_FALSE(Data::isEqual(Data::ProjSize<1u>{1u},
                                Data::ProjSize<1u>{2u}));
    REQUIRE_FALSE(Data::isEqual(Data::ProjSize<2u>{1u, 3u},
                          Data::ProjSize<2u>{2u, 3u}));
}

TEST_CASE("Returns projection size containing 2",
          "[data][proj_meta_info]")
{
    REQUIRE(Data::isEqual(Data::arrToProjSize(std::array{2u}),
                          Data::ProjSize<1u>{2u}));
}

TEST_CASE("Returns projection size containing 2 and 3",
          "[data][proj_meta_info]")
{
    REQUIRE(Data::isEqual(Data::arrToProjSize(std::array{2u, 3u}),
                          Data::ProjSize<2u>{2u, 3u}));
}

TEST_CASE("Returns an array containing 2 and 10",
          "[data][proj_meta_info]")
{
    auto constexpr size = Data::ProjSize<1u>{2u};
    REQUIRE(Data::projSizeToArr(size, 10u) == std::array{2u, 10u});
}

TEST_CASE("Returns an array containing 2, 3 and 10",
          "[data][proj_meta_info]")
{
    auto constexpr size = Data::ProjSize<2u>{2u, 3u};
    REQUIRE(Data::projSizeToArr(size, 10u) == std::array{2u, 3u, 10u});
}

}//GCT::Test namespace.
