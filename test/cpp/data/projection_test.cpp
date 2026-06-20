#include "catch.hpp"
#include "../../../src/cpp/data/projection.h"
#include "../../../src/cpp/data/stat.h"
#include "../../matchers.h"

namespace GCT::Test
{
TEST_CASE("1D projection is initialized", "[data][projection]")
{
    auto constexpr num_cols = 23u;
    auto constexpr init_val = 7.0f;
    auto const proj = Data::Projection{Data::ProjSize<1u>{num_cols}, init_val};

    REQUIRE(proj.numElements() == num_cols);
    REQUIRE(proj.dimensions() == std::array{num_cols});
    REQUIRE(proj.projSize().num_cols == num_cols);
    REQUIRE_THAT(GCT::Data::Stat::mean(proj), AreEqualFloat(init_val));
}

TEST_CASE("2D projection image is initialized", "[data][projection]")
{
    auto constexpr num_cols = 3u;
    auto constexpr num_rows = 4u;
    auto constexpr init_val = 7.0f;
    auto constexpr proj_size = Data::ProjSize<2u>{num_cols, num_rows};
    auto const proj = Data::Projection{proj_size, init_val};

    REQUIRE(proj.numElements() == num_cols * num_rows);
    REQUIRE(proj.dimensions() == std::array{num_cols, num_rows});
    REQUIRE(proj.projSize().num_cols == num_cols);
    REQUIRE(proj.projSize().num_rows == num_rows);
    REQUIRE_THAT(GCT::Data::Stat::mean(proj), AreEqualFloat(init_val));
}

TEST_CASE("Exception is thrown due to invalid arguments", "[data][projection]")
{
    auto constexpr num_cols = 12u;
    auto constexpr num_rows_valid = 34u;
    auto constexpr num_rows_invalid = 0u;
    REQUIRE_NOTHROW(Data::Projection<float, 2u>{Data::ProjSize<2u>{num_cols, num_rows_valid}});
    REQUIRE_THROWS(Data::Projection<float, 2u>{Data::ProjSize<2u>{num_cols, num_rows_invalid}});
}

}//GCT::Test namespace.

