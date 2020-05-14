#include <vector>

#include "parser.h"
#include "utils/utils.h"

#include "../doctest.h"

using namespace parse_it;

TEST_CASE("Operator ||")
{
  constexpr auto parser = one_byte(0x01_b) || one_byte(0x02_b) || one_byte(0x03_b);

  SUBCASE("returns first alternative if it was successful.")
  {
    constexpr auto data = std::array{0x1_b, 0x5_b};
    auto result = parser(data);
    REQUIRE(result);
    CHECK(result->first == 0x1_b);
    const auto expected_remaining = gsl::span(&data[1], data.size() - 1);
    CHECK(result->second == expected_remaining);
  }

  SUBCASE("returns second alternative if the first one failed.")
  {
    constexpr auto data = std::array{0x2_b, 0x5_b};
    auto result = parser(data);
    REQUIRE(result);
    CHECK(result->first == 0x2_b);
    const auto expected_remaining = gsl::span(&data[1], data.size() - 1);
    CHECK(result->second == expected_remaining);
  }

  SUBCASE("fails if none of the parsers succeeds.")
  {
    constexpr auto data = std::array{0x4_b, 0x5_b};
    auto result = parser(data);
    REQUIRE(!result);
  }

  SUBCASE("fails if input is empty.")
  {
    constexpr auto data = std::array<std::byte, 0>{};
    auto result = parser(data);
    REQUIRE(!result);
  }
}