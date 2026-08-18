// include/colors.hpp
#pragma once
#include <string_view>

namespace organise::color {
    // ANSI color codes
    constexpr std::string_view purple = "\033[35m";
    constexpr std::string_view reset  = "\033[0m";
    constexpr std::string_view red    = "\033[31m";
    constexpr std::string_view green  = "\033[32m";
    constexpr std::string_view yellow = "\033[33m";
    constexpr std::string_view blue   = "\033[34m";
    constexpr std::string_view cyan   = "\033[36m";
    constexpr std::string_view bold   = "\033[1m";
}
