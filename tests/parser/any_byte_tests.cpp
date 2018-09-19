#include "../doctest.h"

#include <vector>

#include "parser.h"
#include "utils/utils.h"

using namespace parse_it;

TEST_CASE("Any byte parser retrieve the first byte of data.")
{
    constexpr auto parser = any_byte();

    SUBCASE("Input contains one byte.")
    {
        constexpr auto data = std::array{0x1_b};
        const auto result = parser(data);

        REQUIRE(result);
        CHECK(result->first == 0x1_b);
        CHECK(result->second.empty());
    }

    SUBCASE("Input contains more than one byte.")
    {
        constexpr auto data = std::array{0x1_b, 0x2_b};
        const auto result = parser(data);

        REQUIRE(result);
        CHECK(result->first == 0x1_b);
        const auto expected_remaining = gsl::span(&data[1], data.size() - 1);
        CHECK(result->second == expected_remaining);
    }

    SUBCASE("Input is empty.")
    {
        constexpr auto data = std::array<std::byte, 0>{};
        const auto result = parser(data);

        REQUIRE(!result);
    }
}