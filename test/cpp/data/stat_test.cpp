#include "catch.hpp"
#include "nd_array_test_fix.h"
#include "../../../src/cpp/data/stat.h"
#include "../../matchers.h"

namespace GCT::Test
{
TEST_CASE_METHOD(NDArrayTestFix,
                 "Sum of element values is computed correctly",
                 "[data][stat]")
{
    auto const sum_val = static_cast<float>(img_.numElements());
    REQUIRE_THAT(Data::Stat::sum(img_), AreEqualFloat(init_val_ * sum_val));
}

TEST_CASE_METHOD(NDArrayTestFix,
                 "Mean of element values is computed correctly",
                 "[data][stat]")
{
    REQUIRE_THAT(Data::Stat::mean(img_), AreEqualFloat(init_val_));
}
}//GCT::Test namespace.


