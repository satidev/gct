#ifndef ENUM_PARSER_H
#define ENUM_PARSER_H

#include <iostream>
#include <string>
#include <map>
#include <typeinfo>
#include <vector>
#include <numeric>
#include "string_utils.h"

namespace GCT::IO
{
template<typename EnumClass>
class EnumParser
{
public:

    EnumParser();
    EnumClass getEnum(std::string const &value) const
    {
        auto const iter = str_enum_map_.find(value);
        if (iter == str_enum_map_.end())
            throw std::runtime_error{"Error during parsing string " + value + " to enum.\n" +
                "Valid values are:: " + getAllStrings() + ".\n"};
        return iter->second;
    }

    std::string getString(EnumClass value) const
    {
        auto const iter_val = enum_str_map_.find(value);
        if (iter_val == enum_str_map_.end())
            throw std::runtime_error{"Error during enum to string parsing.\n"};
        return iter_val->second;
    }

private:
    std::string getAllStrings() const;
    std::map<std::string, EnumClass> str_enum_map_;
    std::map<EnumClass, std::string> enum_str_map_;
};

template<typename EnumClass>
std::string EnumParser<EnumClass>::getAllStrings() const
{
    auto str_list = std::vector<std::string>{};
    str_list.reserve(enum_str_map_.size());
    for (auto const &[key, val]: enum_str_map_)
        str_list.push_back(val);

    return join(str_list, std::string{","});
}

}//GCT::IO namespaces.

#endif //ENUM_PARSER_H


