#include "catch.hpp"
#include "nd_array_test_fix.h"
#include "../../../src/cpp/data/stat.h"
#include <iostream>
#include "../../matchers.h"

namespace GCT::Test
{
TEST_CASE_METHOD(NDArrayTestFix,
                 "Image size is initialized correctly",
                 "[data][nd_array]")
{
    REQUIRE(img_.dimensions() == size_);
}
TEST_CASE_METHOD(NDArrayTestFix,
                 "Number of elements is computed correctly",
                 "[data][nd_array]")
{
    REQUIRE(img_.numElements() == num_elem_);
}

TEST_CASE_METHOD(NDArrayTestFix,
                 "The array buffer is initialized correctly",
                 "[data][nd_array]")
{
    auto const exp_op = std::vector(num_elem_, init_val_);
    REQUIRE_THAT(img_.elements(), AreEqualFloatArrays(exp_op));
}

TEST_CASE_METHOD(NDArrayTestFix,
                 "Exception is thrown since the image size is invalid",
                 "[data][nd_array]")
{
    auto constexpr invalid_size = std::array{1u, 2u, 0u};
    REQUIRE_THROWS(Data::NDArray<float, 3u>{invalid_size});
}

TEST_CASE_METHOD(NDArrayTestFix,
                 "All the element values are set to 6.0",
                 "[data][nd_array]")
{
    img_.setVal(6.0f);
    REQUIRE_THAT(Data::Stat::mean(img_), AreEqualFloat(6.0f));
}

TEST_CASE_METHOD(NDArrayTestFix,
                 "3 is added to all element values",
                 "[data][nd_array]")
{
    auto constexpr val = 3.0f;
    img_ += val;
    REQUIRE_THAT(Data::Stat::mean(img_), AreEqualFloat(init_val_ + val));
}

TEST_CASE_METHOD(NDArrayTestFix,
                 "3 is subtracted from all element values",
                 "[data][nd_array]")
{
    auto constexpr val = 3.0f;
    img_ -= val;
    REQUIRE_THAT(Data::Stat::mean(img_), AreEqualFloat(init_val_ - val));
}

TEST_CASE_METHOD(NDArrayTestFix,
                 "3 is multiplied to all element values",
                 "[data][nd_array]")
{
    auto constexpr val = 3.0f;
    img_ *= val;
    REQUIRE_THAT(Data::Stat::mean(img_), AreEqualFloat(init_val_ * val));
}

TEST_CASE_METHOD(NDArrayTestFix,
                 "Element value is divided by 2",
                 "[data][nd_array]")
{
    auto constexpr val = 2.0f;
    img_ /= val;
    REQUIRE_THAT(Data::Stat::mean(img_), AreEqualFloat(init_val_ / val));
}

TEST_CASE_METHOD(NDArrayTestFix,
                 "Element-wise arithmetic",
                 "[data][nd_array]")
{
    auto constexpr init_val_2 = 3.0f;
    auto const img_2 = Data::NDArray{size_, init_val_2};

    SECTION("Element-wise addition") {
        REQUIRE_NOTHROW(img_ += img_2);
        REQUIRE_THAT(Data::Stat::mean(img_), AreEqualFloat(init_val_ + init_val_2));
    }

    SECTION("Element-wise subtraction") {
        REQUIRE_NOTHROW(img_ -= img_2);
        REQUIRE_THAT(Data::Stat::mean(img_), AreEqualFloat(init_val_ - init_val_2));
    }

    SECTION("Element-wise multiplication") {
        REQUIRE_NOTHROW(img_ *= img_2);
        REQUIRE_THAT(Data::Stat::mean(img_), AreEqualFloat(init_val_ * init_val_2));
    }

    SECTION("Element-wise division") {
        REQUIRE_NOTHROW(img_ /= img_2);
        REQUIRE_THAT(Data::Stat::mean(img_), AreEqualFloat(init_val_ / init_val_2));
    }
}

TEST_CASE_METHOD(NDArrayTestFix,
                 "Exception is thrown during element-wise arithmetic due to the size mismatch"
                 "[data][nd_array]")
{
    // Change input image size by incrementing.
    auto incorr_size = size_;
    incorr_size[0]++;

    auto const incorr_img = Data::NDArray{incorr_size, 5.0f};
    REQUIRE_THROWS_AS(img_ /= incorr_img, std::invalid_argument);
}


TEST_CASE_METHOD(NDArrayTestFix,
                 "Returns false since the arrays are not equal due to size mismatch",
                 "[data][nd_array]")
{
    auto new_size = size_;
    new_size[0] += 1u;
    auto const img = Data::NDArray{new_size, init_val_};
    REQUIRE_FALSE(img_.isEqual(img));
}

TEST_CASE_METHOD(NDArrayTestFix,
                 "Returns false since the elements values of arrays are not equal",
                 "[data][nd_array]")
{
    auto const img = Data::NDArray{size_, init_val_ + 1.0f};
    REQUIRE_FALSE(img_.isEqual(img));
}

TEST_CASE_METHOD(NDArrayTestFix,
                 "Element values are copied correctly from STL vector",
                 "[data][nd_array]")
{
    auto const vec = std::vector(num_elem_, 100.0f);
    img_.setData(vec);
    REQUIRE(GCT::Data::Stat::mean(img_) == 100.0);
}

TEST_CASE_METHOD(NDArrayTestFix,
                 "Exception is thrown during copy due to size mismatch",
                 "[data][nd_array]")
{
    auto const vec = std::vector(num_elem_ + 1, 100.0f);
    REQUIRE_THROWS_AS(img_.setData(vec), std::invalid_argument);
}

TEST_CASE_METHOD(NDArrayTestFix,
                 "Elements are moved from STL vector",
                 "[data][nd_array]")
{
    auto vec = std::vector(num_elem_, 200.0f);
    img_.setData(std::move(vec));
    REQUIRE(Data::Stat::mean(img_) == 200.0);
    REQUIRE_FALSE(vec.data());
}

TEST_CASE_METHOD(NDArrayTestFix,
                 "Exception is thrown during move due to size mismatch",
                 "[data][nd_array]")
{
    auto vec = std::vector(num_elem_ + 1, 100.0f);
    REQUIRE_THROWS_AS(img_.setData(std::move(vec)), std::invalid_argument);
}

TEST_CASE_METHOD(NDArrayTestFix,
                 "Returns false since the data pointer is not a nullptr",
                 "[data][nd_array]")
{
    REQUIRE(img_.dataPointer() != nullptr);
}

}//GCT::Test namespace.






