#include "../doctest.h"

#include <vector>

#include "parser.h"
#include "utils/utils.h"

using namespace parse_it;

TEST_CASE("Multiple parsers can be combined together.")
{
    const auto parser = combine([](auto first, auto, auto second){
        return std::to_integer<int>(first) + std::to_integer<int>(second);
    }, one_byte(0x01_b), skip(1), one_byte(0x03_b));

    SUBCASE("Results are combined if every parser succeeds.")
    {
        constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b};
        const auto result = parser(data);

        REQUIRE(result);
        REQUIRE(result->first == 4);
        const auto expected_remaining = gsl::span(&data[3], data.size() - 3);
        CHECK(result->second == expected_remaining);
    }

    SUBCASE("Combined parser fails if one parser fails.")
    {
        constexpr auto data = std::array{0x1_b, 0x2_b, 0x2_b};
        const auto result = parser(data);
        REQUIRE(!result);
    }

    SUBCASE("Combined parser fails if input is too small.")
    {
        constexpr auto data = std::array{0x1_b, 0x2_b};
        const auto result = parser(data);
        REQUIRE(!result);
    }
}