#include "../doctest.h"

#include <vector>

#include "parser.h"
#include "utils/utils.h"

using namespace parse_it;

TEST_CASE("Skip parser")
{
    constexpr auto skip_size = 2;
    constexpr auto parser = skip(skip_size);

    SUBCASE("succeeds if the input is large enough")
    {
        constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b};
        const auto result = parser(data);

        REQUIRE(result);

        SUBCASE("and consumes the given number of bytes.")
        {
            const auto expected_remaining = gsl::span(&data[skip_size], data.size() - skip_size);
            CHECK(result->second == expected_remaining);
        }
    }

    SUBCASE("fails if input is too small.")
    {
        const auto data = std::array{0x1_b};
        const auto result = parser(data);

        REQUIRE(!result);
    }
}