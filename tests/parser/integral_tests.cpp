#include "../doctest.h"

#include <vector>

#include "parser.h"
#include "utils/utils.h"

using namespace parse_it;

TEST_CASE("Uint8 parser retrieve one byte as uint8.")
{
    constexpr auto parser = uint8_parser();

    SUBCASE("Data is parsed correctly.")
    {
        constexpr auto data = std::array{0xFF_b};
        const auto result = parser(data);

        REQUIRE(result);
        CHECK(result->first == 255);
        CHECK(result->second.empty());
    }
}

TEST_CASE("Uint16 parser can be used to retrieve uint16 data.")
{
    constexpr auto parser = uint16_parser();

    SUBCASE("Uint16 data are parsed in little endian format.")
    {
        constexpr auto data = std::array{0x1_b, 0x2_b};
        const auto result = parser(data);

        REQUIRE(result);
        CHECK(result->first == 0x0201);
        CHECK(result->second.empty());
    }

    SUBCASE("Parser fails if input is too small.")
    {
        constexpr auto data = std::array{0x1_b};
        const auto result = parser(data);

        REQUIRE(!result);
    }
}

TEST_CASE("Uint32 parser can be used to retrieve uint32 data.")
{
    constexpr auto parser = uint32_parser();

    SUBCASE("Uint32 data are parsed in little endian format.")
    {
        constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b};
        const auto result = parser(data);

        REQUIRE(result);
        CHECK(result->first == 0x04030201);
        CHECK(result->second.empty());
    }

    SUBCASE("Parser fails if input is too small.")
    {
        constexpr auto data = std::array{0x1_b, 0x2_b, 0x3_b};
        const auto result = parser(data);

        REQUIRE(!result);
    }
}