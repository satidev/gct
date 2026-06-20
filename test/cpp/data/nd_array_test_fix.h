#ifndef ND_ARRAY_TEST_FIX_H
#define ND_ARRAY_TEST_FIX_H

#include "../../../src/cpp/data/nd_array.h"

class NDArrayTestFix
{
protected:
    static constexpr auto init_val_{5.0f};
    static constexpr std::array size_{2u, 3u, 4u};
    static constexpr unsigned num_elem_ = std::accumulate(std::cbegin(size_),
                                                          std::cend(size_),
                                                          1u, std::multiplies<>());
    GCT::Data::NDArray<float, 3> img_{size_, init_val_};
};

#endif //ND_ARRAY_TEST_FIX_H
