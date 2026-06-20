#ifndef MATCHERS_H
#define MATCHERS_H

#include <sstream>
#include "../thirdparty/catch.hpp"
#include "../src/cpp/numeric/comparator.h"

namespace GCT::Test
{
// Matcher for floating point number comparison.
template<std::floating_point T>
class AreEqualFloat: public Catch::MatcherBase<T>
{
public:

    AreEqualFloat(T val, T eps)
        : val_{val}, cmp_threshold_{eps}
    {}
    explicit AreEqualFloat(T val)
        : val_{val}
    {}

    bool match(T const &in) const override
    {
        return Numeric::Comparator::areEqual(in, val_, cmp_threshold_);
    }

    std::string describe() const override
    {
        std::ostringstream ss;
        ss << "== " << val_;
        return ss.str();
    }

private:
    T val_;//One of the values to be compared.
    T cmp_threshold_ = std::numeric_limits<T>::epsilon();//Threshold value for comparison.
};

// Matcher for floating point number comparison.
template<typename Unit>
class AreEqualUnit: public Catch::MatcherBase<Unit>
{
public:

    AreEqualUnit(Unit val, float eps)
        : val_{val}, cmp_threshold_{eps}
    {}
    explicit AreEqualUnit(Unit val)
        : val_{val}
    {}

    bool match(Unit const &in) const override
    {
        return Numeric::Comparator::areEqual<float>(in.val, val_.val, cmp_threshold_);
    }

    std::string describe() const override
    {
        std::ostringstream ss;
        ss << "== " << val_.val;
        return ss.str();
    }

private:
    Unit val_;//One of the values to be compared.
    float cmp_threshold_ = std::numeric_limits<float>::epsilon();//Threshold value for comparison.
};

template<typename ArrayType> requires std::floating_point<typename ArrayType::value_type>
class AreEqualFloatArrays: public Catch::MatcherBase<ArrayType>
{
public:
    using ElemType = typename ArrayType::value_type;

    AreEqualFloatArrays(ArrayType arr, ElemType eps)
        : arr_{std::move(arr)}, cmp_threshold_{eps}
    {}
    explicit AreEqualFloatArrays(ArrayType arr)
        : arr_{std::move(arr)}
    {}

    bool match(ArrayType const &in) const override
    {
        return Numeric::Comparator::areEqualArrays<ElemType>(in, arr_, cmp_threshold_);
    }
    std::string describe() const override
    {
        std::ostringstream ss;
        ss << "\n";
        ss << "==";
        ss << Catch::StringMaker<ArrayType>::convert(arr_);
        return ss.str();
    }
private:
    ArrayType arr_;//One of the arrays to be compared.
    //Threshold for comparison.
    ElemType cmp_threshold_ = std::numeric_limits<ElemType>::epsilon();
};

template<typename Vec>
class AreEqualVecWCS: public Catch::MatcherBase<Vec>
{
public:

    AreEqualVecWCS(Vec const &vec, float eps)
        : vec_{vec}, cmp_threshold_{eps}
    {}
    explicit AreEqualVecWCS(Vec const &vec)
        : vec_{vec}
    {}

    bool match(Vec const &in) const override
    {
        return Numeric::Comparator::areEqual(in, vec_, cmp_threshold_);
    }

    std::string describe() const override
    {
        std::ostringstream ss;
        ss << "==";
        ss << "\n";
        ss << vec_;
        return ss.str();
    }

private:
    Vec vec_;
    float cmp_threshold_ = std::numeric_limits<float>::epsilon();//Threshold value for comparison.
};

template<typename UnitArray>
std::vector<float> valArray(const UnitArray &array);

template<typename UnitArray>
std::vector<float> valArray(const UnitArray &array)
{
    auto vals = std::vector<float>{};
    vals.reserve(array.size());
    std::transform(std::begin(array), std::end(array),
                   std::back_inserter(vals),
                   [](typename UnitArray::value_type unit)
                   {
                       return unit.val;
                   }
    );
    return vals;
}

}//GCT::Test namespace

#endif //MATCHERS_H

