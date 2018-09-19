#include "../doctest.h"

#include <vector>

#include "parser.h"
#include "utils/utils.h"

using namespace parse_it;

TEST_CASE("Skip parser can be used to skip some data in the input buffer.")
{
    constexpr auto skip_size = 2;
    constexpr auto parser = skip(skip_size);

    SUBCASE("Parser consumes everything if input is of skipped size.")
    {
        constexpr auto data = std::array{0x1_b, 0x2_b};
        const auto result = parser(data);

        REQUIRE(result);
        CHECK(result->second.empty());
    }

    SUBCASE("Parser consumes only the specified skipped size.")
    {
        constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b};
        const auto result = parser(data);

        REQUIRE(result);
        const auto expected_remaining = gsl::span(&data[skip_size], data.size() - skip_size);
        CHECK(result->second == expected_remaining);
    }

    SUBCASE("Parser fails if input is too small.")
    {
        const auto data = std::array{0x1_b};
        const auto result = parser(data);

        REQUIRE(!result);
    }
}