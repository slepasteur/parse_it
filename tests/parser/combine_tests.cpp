#include <algorithm>
#include <array>

#include "parse_it/parser.h"
#include "parse_it/utils/byte_litterals.h"

#include <doctest/doctest.h>

using namespace parse_it;
using namespace parse_it::byte_litterals;

TEST_CASE("combine parser")
{
  constexpr auto parser = combine(
    [](auto first, auto, auto second) { return std::to_integer<int>(first) + std::to_integer<int>(second); },
    one_byte(0x01_b), skip(1), one_byte(0x03_b));

  SUBCASE("succeeds if all parsers succeed one after the other")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b};
    const auto result = parser(data);

    REQUIRE(result);

    SUBCASE("and returns the result of the combine function.") { REQUIRE(result->first == 4); }

    SUBCASE("and consumes the parsed data from the input.")
    {
      const auto expected_remaining = std::span(&data[3], data.size() - 3);
      REQUIRE(std::ranges::equal(result->second, expected_remaining));
    }
  }

  SUBCASE("fails if one parser fails.")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b, 0x2_b};
    const auto result = parser(data);
    REQUIRE(!result);
  }

  SUBCASE("fails if input is too small.")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b};
    const auto result = parser(data);
    REQUIRE(!result);
  }
}
