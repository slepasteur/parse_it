#include <type_traits>
#include <vector>

#include "parser.h"
#include "utils/utils.h"

#include "../doctest.h"

using namespace parse_it;

TEST_CASE("Uint8 parser")
{
  constexpr auto parser = uint8_parser();

  SUBCASE("succeeds when given one byte")
  {
    constexpr auto data = std::array{0xFF_b};
    const auto result = parser(data);

    REQUIRE(result);

    SUBCASE("and returns the byte value converted as uint8.")
    {
      static_assert(std::is_same_v<decltype(result->first), uint8_t>);
      REQUIRE(result->first == 255);
    }

    SUBCASE("and consumes the input.") { REQUIRE(result->second.empty()); }
  }

  SUBCASE("fails if input is empty.")
  {
    const auto data = std::vector<std::byte>{};
    const auto result = parser(data);

    REQUIRE(!result);
  }
}

TEST_CASE("Uint16 parser")
{
  constexpr auto parser = uint16_parser();

  SUBCASE("succeeds when given two bytes")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b};
    const auto result = parser(data);

    REQUIRE(result);
    SUBCASE("and returns a uint16 parsed as little endian.") { REQUIRE(result->first == 0x0201); }

    SUBCASE("and consumes the input.") { REQUIRE(result->second.empty()); }
  }

  SUBCASE("fails if input is too small.")
  {
    constexpr auto data = std::array{0x1_b};
    const auto result = parser(data);

    REQUIRE(!result);
  }
}

TEST_CASE("Uint32 parser")
{
  constexpr auto parser = uint32_parser();

  SUBCASE("succeeds when given four bytes")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b};
    const auto result = parser(data);

    REQUIRE(result);
    SUBCASE("and returns a uint16 parsed as little endian.") { REQUIRE(result->first == 0x04030201); }

    SUBCASE("and consumes the input.") { REQUIRE(result->second.empty()); }
  }

  SUBCASE("fails if input is too small.")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b};
    const auto result = parser(data);

    REQUIRE(!result);
  }
}