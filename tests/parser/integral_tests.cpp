#include <type_traits>
#include <vector>

#include "parse_it/parser.h"
#include "parse_it/utils/utils.h"

#include <doctest/doctest.h>

using namespace parse_it;

TEST_CASE("Uint8 parser")
{
  constexpr auto parser = integral_parser<std::uint8_t>();

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
  constexpr auto parser = integral_parser<std::uint16_t>();

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

TEST_CASE("Uint16 parser big endian")
{
  constexpr auto parser = integral_parser<std::uint16_t>(big_endian{});

  SUBCASE("succeeds when given two bytes")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b};
    const auto result = parser(data);

    REQUIRE(result);
    SUBCASE("and returns a uint16 parsed as little endian.") { REQUIRE(result->first == 0x0102); }

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
  constexpr auto parser = integral_parser<std::uint32_t>();

  SUBCASE("succeeds when given four bytes")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b};
    const auto result = parser(data);

    REQUIRE(result);
    SUBCASE("and returns a uint32 parsed as little endian.") { REQUIRE(result->first == 0x04030201); }

    SUBCASE("and consumes the input.") { REQUIRE(result->second.empty()); }
  }

  SUBCASE("fails if input is too small.")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b};
    const auto result = parser(data);

    REQUIRE(!result);
  }
}

TEST_CASE("Uint32 parser big endian")
{
  constexpr auto parser = integral_parser<std::uint32_t>(big_endian{});

  SUBCASE("succeeds when given four bytes")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b};
    const auto result = parser(data);

    REQUIRE(result);
    SUBCASE("and returns a uint16 parsed as little endian.") { REQUIRE(result->first == 0x01020304); }

    SUBCASE("and consumes the input.") { REQUIRE(result->second.empty()); }
  }

  SUBCASE("fails if input is too small.")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b};
    const auto result = parser(data);

    REQUIRE(!result);
  }
}

TEST_CASE("Uint64 parser")
{
  constexpr auto parser = integral_parser<std::uint64_t>();

  SUBCASE("succeeds when given 8 bytes")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b, 0x5_b, 0x6_b, 0x7_b, 0x8_b};
    const auto result = parser(data);

    REQUIRE(result);
    SUBCASE("and returns a uint64 parsed as little endian.") { REQUIRE(result->first == 0x0807060504030201); }

    SUBCASE("and consumes the input.") { REQUIRE(result->second.empty()); }
  }

  SUBCASE("fails if input is too small.")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b, 0x5_b, 0x6_b, 0x7_b};
    const auto result = parser(data);

    REQUIRE(!result);
  }
}

TEST_CASE("Uint64 parser big endian")
{
  constexpr auto parser = integral_parser<std::uint64_t>(big_endian{});

  SUBCASE("succeeds when given 8 bytes")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b, 0x5_b, 0x6_b, 0x7_b, 0x8_b};
    const auto result = parser(data);

    REQUIRE(result);
    SUBCASE("and returns a uint64 parsed as big endian.") { REQUIRE(result->first == 0x0102030405060708); }

    SUBCASE("and consumes the input.") { REQUIRE(result->second.empty()); }
  }

  SUBCASE("fails if input is too small.")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b, 0x5_b, 0x6_b, 0x7_b};
    const auto result = parser(data);

    REQUIRE(!result);
  }
}
