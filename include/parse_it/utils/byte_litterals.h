#pragma once
#ifndef PARSE_IT_UTILS_UTILS_H
#define PARSE_IT_UTILS_UTILS_H

#include <cstddef>
#include <iterator>

namespace parse_it::byte_litterals {

constexpr std::byte operator"" _b(unsigned long long b)
{
  return static_cast<std::byte>(b);
}

constexpr std::byte operator"" _b(char c)
{
  return static_cast<std::byte>(c);
}

} // namespace parse_it

#endif