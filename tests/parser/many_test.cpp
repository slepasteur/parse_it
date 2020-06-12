#include <algorithm>
#include <array>

#include "parse_it/parser.h"
#include "parse_it/utils/byte_litterals.h"

#include <doctest/doctest.h>

using namespace parse_it;
using namespace parse_it::byte_litterals;

TEST_CASE("Many return a parser accepting a repetition of the given parser.")
{
  auto one_counter = many(one_byte(0x01_b), 0, [](auto acc, auto) { return ++acc; });
  SUBCASE("Parser succeeds multiple time.")
  {
    constexpr auto data = std::array{0x1_b, 0x1_b, 0x1_b, 0x2_b, 0x3_b};
    auto result = one_counter(data);
    REQUIRE(result);
    CHECK(result->first == 3);
    const auto expected_remaining = std::span(&data[3], data.size() - 3);
    CHECK(std::ranges::equal(result->second, expected_remaining));
  }

  SUBCASE("parser fails")
  {
    constexpr auto data = std::array{0x2_b, 0x1_b, 0x1_b, 0x2_b, 0x3_b};
    auto result = one_counter(data);
    REQUIRE(result);
    CHECK(result->first == 0);
    const auto expected_remaining = std::span(&data[0], data.size());
    CHECK(std::ranges::equal(result->second, expected_remaining));
  }
}
