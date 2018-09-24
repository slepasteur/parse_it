#include "doctest.h"

#include <array>
#include <algorithm>

#include "utils/utils.h"

using namespace parse_it;

TEST_CASE("Byte view of int represents the int's bytes in little endian by default.")
{
    const int data = 0x01020304;
	auto bytes = std::array{0x04_b, 0x03_b, 0x02_b, 0x01_b};
	auto view = byte_view<>{data};

	REQUIRE(std::equal(view.begin(), view.end(), bytes.begin(), bytes.end()));
}

TEST_CASE("Byte view of int can represent the int's bytes in big endian.")
{
    const int data = 0x01020304;
	auto bytes = std::array{0x01_b, 0x02_b, 0x03_b, 0x04_b};
	auto view = byte_view<big_endian>{data};

	REQUIRE(std::equal(view.begin(), view.end(), bytes.begin(), bytes.end()));
}