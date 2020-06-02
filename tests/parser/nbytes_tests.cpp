#include <algorithm>
#include <array>

#include "parse_it/parser.h"
#include "parse_it/utils/byte_litterals.h"

#include <doctest/doctest.h>

using namespace parse_it;
using namespace parse_it::byte_litterals;

TEST_CASE("N bytes parser")
{
  constexpr auto parser = n_bytes(5);

  SUBCASE("succeeds when applied to an input containing N bytes")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b, 0x5_b};
    const auto result = parser(data);
    REQUIRE(result);

    SUBCASE("and returns the first N bytes.") { REQUIRE(std::ranges::equal(result->first, data)); }

    SUBCASE("and consumes the first N bytes.") { REQUIRE(result->second.empty()); }
  }

  SUBCASE("fails if input is not big enough.")
  {
    constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b};
    const auto result = parser(data);

    REQUIRE(!result);
  }
}
