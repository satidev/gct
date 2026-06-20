#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <regex>

namespace GCT::IO
{
std::string join(std::vector<std::string> const &strings,
                 std::string delim);

inline std::string trim(std::string const &str)
{
    return std::regex_replace(str, std::regex("^ +| +$|( ) +"), "$1");
}

std::string toLower(std::string const &str);
std::string toUpper(std::string const &str);
}

#endif //STRING_UTILS_H
