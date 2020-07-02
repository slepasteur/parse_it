#pragma once

#include <concepts>

namespace parse_it {

template <typename T>
concept arithmetic = std::integral<T> || std::floating_point<T>;
}
