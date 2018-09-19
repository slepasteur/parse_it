#include "../doctest.h"

#include <vector>

#include "parser.h"
#include "utils/utils.h"

using namespace parse_it;

TEST_CASE("A sequence of bytes can be parsed.")
{
    constexpr auto seq = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b};
    constexpr auto parser = byte_seq(seq);

    SUBCASE("Input containing only parsed value.")
    {
        const auto result = parser(seq);

        REQUIRE(result);
        CHECK(result->second.empty());
        CHECK(std::equal(result->first.begin(), result->first.end(), seq.begin(), seq.end()));
    }

    SUBCASE("Input containing parsed value and more.")
    {
        constexpr auto remaining = std::array{0x5_b, 0x6_b, 0x7_b, 0x8_b};
        const auto data = [&](){
            auto value = std::vector<std::byte>{};
            value.insert(value.begin(), seq.begin(), seq.end());
            value.insert(value.end(), remaining.begin(), remaining.end());
            return value;
        }();
        const auto result = parser(data);

        REQUIRE(result);
        CHECK(std::equal(result->first.begin(), result->first.end(), seq.begin(), seq.end()));
        CHECK(std::equal(result->second.begin(), result->second.end(), remaining.begin(), remaining.end()));
    }

    SUBCASE("Empty input.")
    {
        const auto data = std::vector<std::byte>{};
        const auto result = parser(data);

        REQUIRE(!result);
    }

    SUBCASE("Input without expected byte at beginning.")
    {
        const auto data = std::vector<std::byte>{0x5_b, 0x6_b, 0x7_b, 0x8_b};
        const auto result = parser(data);

        REQUIRE(!result);
    }
}