#include <vector>

#include "parse_it/parser.h"
#include "parse_it/utils/utils.h"

#include <doctest/doctest.h>

using namespace parse_it;

TEST_CASE("Byte sequence parser")
{
  constexpr auto seq = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b};
  constexpr auto parser = byte_seq(seq);

  SUBCASE("succeeds on input starting with expected values")
  {
    constexpr auto remaining = std::array{0x5_b, 0x6_b, 0x7_b, 0x8_b};
    const auto data = [&]() {
      auto value = std::vector<std::byte>{};
      value.insert(value.begin(), seq.begin(), seq.end());
      value.insert(value.end(), remaining.begin(), remaining.end());
      return value;
    }();
    const auto result = parser(data);

    REQUIRE(result);

    SUBCASE("and returns the parsed bytes.")
    {
      REQUIRE(std::equal(result->first.begin(), result->first.end(), seq.begin(), seq.end()));
    }

    SUBCASE("and consumes the parsed bytes.")
    {
      REQUIRE(std::equal(result->second.begin(), result->second.end(), remaining.begin(), remaining.end()));
    }
  }

  SUBCASE("fails on empty input.")
  {
    const auto data = std::vector<std::byte>{};
    const auto result = parser(data);

    REQUIRE(!result);
  }

  SUBCASE("fails when expected values are not at the beginning of the input.")
  {
    const auto data = std::vector<std::byte>{0x5_b, 0x1_b, 0x2_b, 0x3_b, 0x4_b, 0x6_b};
    const auto result = parser(data);

    REQUIRE(!result);
  }
}