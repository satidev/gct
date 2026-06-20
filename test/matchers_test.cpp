#include "../thirdparty/catch.hpp"
#include "matchers.h"

using namespace GCT::Test;

TEST_CASE("Matcher returns true since the two numbers are equal", "[matchers]")
{
    auto constexpr eps_f = 0.0000000001f;
    REQUIRE(AreEqualFloat<float>{-1.0f, eps_f}.match(-1.0f));
    REQUIRE_FALSE(AreEqualFloat<float>{1.0f, eps_f}.match(2.0f));
}

TEST_CASE("Matcher returns true since the two containers are equal", "[matchers]")
{
    auto constexpr eps_f = 0.0000000001f;
    using Vec = std::vector<float>;
    Vec v{1.0f, 1.0f, 3.0f};
    REQUIRE(AreEqualFloatArrays<Vec>{v, eps_f}.match(v));
    REQUIRE_FALSE(AreEqualFloatArrays<Vec>{v, eps_f}.match(Vec{1.0f, 1.0f, 4.0f}));
}

