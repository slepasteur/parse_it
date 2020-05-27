#include <algorithm>
#include <array>

#include "parse_it/parser.h"
#include "parse_it/utils/utils.h"

#include <doctest/doctest.h>

using namespace parse_it;

TEST_CASE("fmap parser")
{
  constexpr auto parser = fmap([](std::byte b) { return std::to_integer<uint32_t>(b) * 100; }, any_byte());

  SUBCASE("succeeds when applied to an input where the first parser succeeds")
  {
    constexpr auto data = std::array{0x5_b};
    const auto result = parser(data);
    REQUIRE(result);

    SUBCASE("and returns F applied to the first parser result.") { REQUIRE(result->first == 500); }
  }
}
