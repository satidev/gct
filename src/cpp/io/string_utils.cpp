#include "string_utils.h"
#include <numeric>
#include <vector>
#include <algorithm>
#include <cctype>

namespace GCT::IO
{
std::string join(std::vector<std::string> const &strings,
                 std::string delim)
{
    if (strings.empty()) {
        return std::string{};
    }
    else {
        return std::accumulate(std::cbegin(strings) + 1u,
                               std::cend(strings), strings.front(),
                               [&delim](auto const &concat_str,
                                        auto const &str)
                               {
                                   return concat_str + delim + str;
                               }
        );
    }
}

std::string toLower(std::string const &str)
{
    auto str_low = str;
    std::transform(std::begin(str), std::end(str), std::begin(str_low),
                   [](unsigned char c)
                   {
                       return std::tolower(c);
                   }
    );
    return str_low;
}

std::string toUpper(std::string const &str)
{
    auto str_up = str;
    std::transform(std::begin(str), std::end(str), std::begin(str_up),
                   [](unsigned char c)
                   {
                       return std::toupper(c);
                   }
    );
    return str_up;
}
}//GCT::IO namespaces.
