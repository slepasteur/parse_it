#include <algorithm>
#include <array>

#include "parser.h"
#include "utils/utils.h"

#include <doctest/doctest.h>

using namespace parse_it;

TEST_CASE("Any byte parser")
{
  constexpr auto parser = any_byte();

  SUBCASE("succeeds when applied to an input containing multiple bytes")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b};
    const auto result = parser(data);
    REQUIRE(result);

    SUBCASE("and returns the first byte.") { REQUIRE(result->first == 0x1_b); }

    SUBCASE("and consumes the first byte.")
    {
      const auto expected_remaining = std::span(&data[1], data.size() - 1);
      REQUIRE(std::ranges::equal(result->second, expected_remaining));
    }
  }

  SUBCASE("fails if input is empty.")
  {
    constexpr auto data = std::array<std::byte, 0>{};
    const auto result = parser(data);

    REQUIRE(!result);
  }
}
