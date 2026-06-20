#ifndef ND_ARRAY_DEV_TEST_FIX_CUH
#define ND_ARRAY_DEV_TEST_FIX_CUH

#include <array>

namespace GCT::Test
{
template<size_t Dim>
constexpr std::array<unsigned, Dim> testArrSize()
{
    auto arr = std::array<unsigned, Dim>{};
    auto val = 2u;
    for (auto &elem: arr) {
        elem = val;
        val++;
    }
    return arr;
}
}//GCT::Test namespace.

#endif //ND_ARRAY_DEV_TEST_FIX_CUH
