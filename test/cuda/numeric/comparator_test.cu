#include "../../../thirdparty/catch.hpp"
#include "../../../src/cuda/numeric/comparator.cuh"

using namespace GCT::Geom::Comparator::CUDA;

TEST_CASE("Returns true since the input is positive infinity", "[comparator_cu]")
{
    REQUIRE(isPositiveInf(std::numeric_limits<float>::infinity()));
}
TEST_CASE("Returns false since the input is not positive infinity", "[comparator_cu]")
{
    REQUIRE_FALSE(isPositiveInf(std::numeric_limits<float>::epsilon()));
    REQUIRE_FALSE(isPositiveInf(-1.0f * std::numeric_limits<float>::infinity()));
    REQUIRE_FALSE(isPositiveInf(std::numeric_limits<float>::quiet_NaN()));
}

TEST_CASE("Returns true since the input is negative infinity", "[comparator_cu]")
{
    REQUIRE(isNegativeInf(-1.0f * std::numeric_limits<float>::infinity()));
}

TEST_CASE("Returns false since the input is not negative infinity", "[comparator_cu]")
{
    REQUIRE_FALSE(isNegativeInf(std::numeric_limits<float>::epsilon()));
    REQUIRE_FALSE(isNegativeInf(std::numeric_limits<float>::infinity()));
    REQUIRE_FALSE(isNegativeInf(std::numeric_limits<float>::quiet_NaN()));
}

TEST_CASE("Returns true since the two floating point numbers are equal", "[comparator_cu]")
{
    constexpr auto eps = 0.00001f;
    REQUIRE(isEqual(2.0f, 2.0f, eps));
}

TEST_CASE("Returns true since the two numbers are NaN", "[comparator_cu]")
{
    constexpr auto eps = 0.00001f;
    REQUIRE(isEqual(std::numeric_limits<float>::quiet_NaN(),
                    std::numeric_limits<float>::quiet_NaN(), eps));
}

TEST_CASE("Returns false since the two floating point numbers are not equal", "[comparator_cu]")
{
    constexpr auto eps = 0.00001f;
    REQUIRE_FALSE(isEqual(2.0f, -2.0f, eps));
}

TEST_CASE("Returns false since the numbers are positive and negative infinities", "[comparator_cu]")
{
    constexpr auto eps = 0.00001f;
    REQUIRE_FALSE(isEqual(std::numeric_limits<float>::infinity(),
                          -1.0f * std::numeric_limits<float>::infinity(), eps));
}

TEST_CASE("Returns true since the container has same value for all it's element", "[comparator_cu]")
{
    constexpr auto eps = 0.00001f;
    thrust::device_vector<float> vec(10, -2.0f);
    REQUIRE(hasSameVal(vec, -2.0f, eps));
    REQUIRE_FALSE(hasSameVal(vec, 2.0f, eps));
}

TEST_CASE("Returns true since the two containers are of same size and same elements",
          "[comparator_cu]")
{
    constexpr auto eps = 0.00001f;
    thrust::device_vector<float> vec_1(10, -2.0f);
    thrust::device_vector<float> vec_2(10, -2.0f);
    REQUIRE(isEqualVec<thrust::device_vector<float>>(vec_1, vec_2, eps));
}

TEST_CASE("Returns false since the two containers are of different size", "[comparator_cu]")
{
    constexpr auto eps = 0.00001f;
    thrust::device_vector<float> vec_1(10, -2.0f);
    thrust::device_vector<float> vec_2(9, -2.0f);
    REQUIRE_FALSE(isEqualVec<thrust::device_vector<float>>(vec_1, vec_2, eps));
}

TEST_CASE("Returns false since the two containers are of same size but different elements",
          "[comparator_cu]")
{
    constexpr auto eps = 0.00001f;
    thrust::device_vector<float> vec_1(10, -2.0f);
    thrust::device_vector<float> vec_2(10, 2.0f);
    REQUIRE_FALSE(isEqualVec<thrust::device_vector<float>>(vec_1, vec_2, eps));
}
