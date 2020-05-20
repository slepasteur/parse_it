#include <algorithm>
#include <array>

#include "parser.h"
#include "utils/utils.h"

#include <doctest/doctest.h>

using namespace parse_it;

TEST_CASE("One byte parser")
{
  constexpr auto value = 0xd4_b;
  constexpr auto parser = one_byte(value);

  SUBCASE("succeeds on input starting with the expected byte")
  {
    const auto data = std::vector<std::byte>{value, 0x01_b, 0x02_b};
    const auto result = parser(data);

    REQUIRE(result);

    SUBCASE("and return the byte value.") { REQUIRE(result->first == value); }

    SUBCASE("and consumes the first byte.")
    {
      const auto expected_remaining = std::span(&data[1], data.size() - 1);
      REQUIRE(std::ranges::equal(result->second, expected_remaining));
    }
  }

  SUBCASE("fails on empty input.")
  {
    const auto data = std::vector<std::byte>{};
    const auto result = parser(data);

    REQUIRE(!result);
  }

  SUBCASE("fails if the first byte is not the expected one.")
  {
    const auto data = std::vector<std::byte>{0x01_b, 0x02_b};
    const auto result = parser(data);

    REQUIRE(!result);
  }
}
