#include "catch.hpp"
#include "../../../src/cpp/data/projection_stack.h"
#include "../../../src/cpp/data/stat.h"
#include "../../matchers.h"

namespace GCT::Test
{
TEST_CASE("1D projection stack is initialized correctly",
          "[data][projection_stack]")
{
    auto constexpr num_cols = 23u;
    auto constexpr num_proj = 3u;
    auto constexpr init_val = 7.0f;
    auto constexpr size = Data::ProjSize<1u>{num_cols};
    auto const proj = Data::ProjectionStack{size, num_proj, init_val};

    REQUIRE(proj.numElements() == num_cols * num_proj);
    REQUIRE(proj.dimensions() == std::array{num_cols, num_proj});
    REQUIRE(proj.projSize().num_cols == num_cols);
    REQUIRE(proj.numProj() == num_proj);
    REQUIRE_THAT(GCT::Data::Stat::mean(proj), AreEqualFloat(init_val));
}

TEST_CASE("2D projection stack is initialized correctly",
          "[data][projection_stack]")
{
    auto constexpr num_cols = 2u;
    auto constexpr num_rows = 3u;
    auto constexpr num_proj = 4u;
    auto constexpr init_val = 7.0f;
    auto constexpr size = Data::ProjSize<2u>{num_cols, num_rows};
    auto const proj = Data::ProjectionStack{size, num_proj, init_val};

    REQUIRE(proj.numElements() == num_cols * num_rows * num_proj);
    REQUIRE(proj.dimensions() == std::array{num_cols, num_rows, num_proj});
    REQUIRE(proj.projSize().num_cols == num_cols);
    REQUIRE(proj.projSize().num_rows == num_rows);
    REQUIRE(proj.numProj() == num_proj);
    REQUIRE_THAT(GCT::Data::Stat::mean(proj), AreEqualFloat(init_val));
}

TEST_CASE("2D projection is extracted from the stack",
          "[data][projection_stack]")
{
    auto constexpr num_cols = 23;
    auto constexpr num_rows = 34;
    auto constexpr num_proj = 3u;
    auto constexpr init_val = 7.0f;
    auto constexpr size = Data::ProjSize<2u>{num_cols, num_rows};
    auto proj_stack = Data::ProjectionStack{size, num_proj, init_val};

    // Generate two different projections.
    auto constexpr num_pix = num_cols * num_rows;
    auto const proj_1 = std::vector(num_pix, 1.0f);
    auto proj_2 = std::vector(num_pix, 0.0f);
    std::iota(std::begin(proj_2), std::end(proj_2), 100.0f);
    auto const proj_3 = std::vector(num_pix, -67.3f);

    // Copy the projections into stack.
    auto all_proj = proj_1;
    all_proj.insert(std::end(all_proj), std::begin(proj_2), std::end(proj_2));
    all_proj.insert(std::end(all_proj), std::begin(proj_3), std::end(proj_3));
    proj_stack.setData(std::move(all_proj));

    auto constexpr proj_idx = 1u;
    auto const proj_extr = proj_stack.getProj(proj_idx);
    REQUIRE_THAT(proj_extr.elements(), AreEqualFloatArrays(proj_2));
    REQUIRE_THROWS_AS(proj_stack.getProj(num_proj), std::out_of_range);
}

TEST_CASE("1D projection is extracted from the stack",
          "[data][projection_stack]")
{
    auto constexpr num_cols = 23;
    auto constexpr num_proj = 3u;
    auto constexpr init_val = 7.0f;
    auto constexpr size = Data::ProjSize<1u>{num_cols};
    auto proj_stack = Data::ProjectionStack{size, num_proj, init_val};

    // Generate two different projections.
    auto constexpr num_pix = num_cols;
    auto const proj_1 = std::vector(num_pix, 1.0f);
    auto proj_2 = std::vector(num_pix, 0.0f);
    std::iota(std::begin(proj_2), std::end(proj_2), 100.0f);
    auto const proj_3 = std::vector(num_pix, -67.3f);

    // Copy the projections into stack.
    auto all_proj = proj_1;
    all_proj.insert(std::end(all_proj), std::begin(proj_2), std::end(proj_2));
    all_proj.insert(std::end(all_proj), std::begin(proj_3), std::end(proj_3));
    proj_stack.setData(std::move(all_proj));

    auto constexpr proj_idx = 1u;
    auto const proj_extr = proj_stack.getProj(proj_idx);
    REQUIRE(proj_extr.projSize().num_cols == num_cols);
    REQUIRE_THAT(proj_extr.elements(), AreEqualFloatArrays(proj_2));
    REQUIRE_THROWS_AS(proj_stack.getProj(num_proj), std::out_of_range);
}

TEST_CASE("Exception is thrown if the arguments are invalid",
          "[data][projection_stack]")
{
    auto constexpr num_cols = 12u;
    auto constexpr num_rows_valid = 34u;
    auto constexpr num_rows_invalid = 0u;
    auto constexpr num_proj = 3u;

    REQUIRE_NOTHROW(Data::ProjectionStack<float, 2u>{
        Data::ProjSize<2u>{num_cols, num_rows_valid},
        num_proj});
    REQUIRE_THROWS(Data::ProjectionStack<float, 2u>{
        Data::ProjSize<2u>{num_cols, num_rows_invalid},
        num_proj});
}

TEST_CASE("2D projection is copied to stack",
          "[data][projection_stack]")
{
    auto constexpr num_cols = 45u;
    auto constexpr num_rows = 33u;
    auto constexpr num_proj = 23u;

    auto proj_stack = Data::ProjectionStack{Data::ProjSize<2u>{num_cols, num_rows},
                                            num_proj, 34.0f};
    auto const proj = Data::Projection{Data::ProjSize<2u>{num_cols, num_rows},
                                       25.0f};

    auto constexpr proj_idx = 10u;
    proj_stack.setProj(proj, proj_idx);
    auto const proj_extr = proj_stack.getProj(proj_idx);
    REQUIRE_THAT(proj_extr.elements(), AreEqualFloatArrays(proj.elements()));
}

TEST_CASE("1D projection is copied to stack",
          "[data][projection_stack]")
{
    auto constexpr num_cols = 45u;
    auto constexpr num_proj = 23u;

    auto proj_stack = Data::ProjectionStack{Data::ProjSize<1u>{num_cols}, num_proj, 34.0f};
    auto const proj = Data::Projection{Data::ProjSize<1u>{num_cols}, 25.0f};

    auto constexpr proj_idx = 10u;
    proj_stack.setProj(proj, proj_idx);
    auto const proj_extr = proj_stack.getProj(proj_idx);
    REQUIRE_THAT(proj_extr.elements(), AreEqualFloatArrays(proj.elements()));
}

}//GCT::Test namespace.




