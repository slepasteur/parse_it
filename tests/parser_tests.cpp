#include "doctest.h"

#include <vector>

#include "parser.h"
#include "utils/utils.h"

using namespace parse_it;

TEST_CASE("A single byte can be parsed.")
{
    auto constexpr value = 0xd4_b;
    const auto parser = one_byte(value);

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

TEST_CASE("Any byte parser retrieve the first byte of data.")
{
    const auto parser = any_byte();

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

TEST_CASE("A sequence of bytes can be parsed.")
{
    constexpr auto seq = std::array{0x1_b, 0x2_b, 0x3_b, 0x4_b};
    const auto parser = byte_seq(seq);

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

TEST_CASE("Skip parser can be used to skip some data in the input buffer.")
{
    constexpr auto skip_size = 2;
    const auto parser = skip(skip_size);

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