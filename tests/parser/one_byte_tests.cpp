#include "../doctest.h"

#include <vector>

#include "parser.h"
#include "utils/utils.h"

using namespace parse_it;

TEST_CASE("A single byte can be parsed.")
{
    constexpr auto value = 0xd4_b;
    constexpr auto parser = one_byte(value);

    SUBCASE("Input containing only parsed value.")
    {
        const auto data = std::vector<std::byte>{value};
        const auto result = parser(data);

        REQUIRE(result);
        CHECK(result->first == value);
        CHECK(result->second.empty());
    }

    SUBCASE("Input containing parsed value and more.")
    {
        const auto data = std::vector<std::byte>{value, 0x01_b, 0x02_b};
        const auto result = parser(data);

        REQUIRE(result);
        CHECK(result->first == value);
        const auto expected_remaining = gsl::span(&data[1], data.size() - 1);
        CHECK(result->second == expected_remaining);
    }

    SUBCASE("Empty input.")
    {
        const auto data = std::vector<std::byte>{};
        const auto result = parser(data);

        REQUIRE(!result);
    }

    SUBCASE("Input without expected byte at beginning.")
    {
        const auto data = std::vector<std::byte>{0x01_b, 0x02_b};
        const auto result = parser(data);

        REQUIRE(!result);
    }
}