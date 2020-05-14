#include <algorithm>
#include <array>

#include "utils/utils.h"

#include "doctest.h"

using namespace parse_it;

TEST_CASE("Byte span of int gives access to the int's bytes in little endian by default.")
{
  int data = 0x01020304;
  auto expected_result = std::array{0x08_b, 0x06_b, 0x04_b, 0x02_b};
  auto span = byte_span<>{data};

  std::transform(span.begin(), span.end(), span.begin(), [](auto b) { return std::byte(std::to_integer<int>(b) * 2); });

  REQUIRE(std::equal(span.begin(), span.end(), expected_result.begin(), expected_result.end()));
}

TEST_CASE("Byte span of int can give access to the int's bytes in big endian.")
{
  int data = 0x01020304;
  auto expected_result = std::array{0x02_b, 0x04_b, 0x06_b, 0x08_b};
  auto span = byte_span<big_endian>{data};

  std::transform(span.begin(), span.end(), span.begin(), [](auto b) { return std::byte(std::to_integer<int>(b) * 2); });

  REQUIRE(std::equal(span.begin(), span.end(), expected_result.begin(), expected_result.end()));
}