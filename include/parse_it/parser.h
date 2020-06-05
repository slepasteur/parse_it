#pragma once
#ifndef PARSE_IT_PARSER_H
#define PARSE_IT_PARSER_H

#include <algorithm>
#include <bit>
#include <concepts>
#include <iterator>
#include <type_traits>

#include "parser_details.h"
#include "parser_types.h"

namespace parse_it {

/**
 * Create a parser of one byte of value b.
 * @param b The byte to parse.
 * @return A parser of type: i -> optional<(b, i)>
 */
constexpr inline auto one_byte(std::byte b)
{
  return [b](parse_input_t input) -> parse_result_t<std::byte> {
    if (input.empty())
    {
      return std::nullopt;
    }
    if (input[0] == b)
      return std::pair(b, input.subspan(1));
    return std::nullopt;
  };
}

/**
 * Create a parser of one byte of any value.
 * @return A parser of type: i -> optional<(b, i)>
 */
constexpr inline auto any_byte()
{
  return [](parse_input_t input) -> parse_result_t<std::byte> {
    if (input.empty())
    {
      return std::nullopt;
    }
    return std::pair(input[0], input.subspan(1));
  };
}

/**
 * Create a parser for a sequence of bytes.
 * @param seq The sequence of byte to parse.
 * @return A parser of type: i -> optional<(seq, i)>
 */
template <typename SEQ>
constexpr inline auto byte_seq(SEQ&& seq)
{
  using sequence_type = std::remove_cv_t<std::remove_reference_t<SEQ>>;
  return [seq = std::forward<SEQ>(seq)](parse_input_t input) -> parse_result_t<sequence_type> {
    if (seq.size() > input.size())
    {
      return std::nullopt;
    }
    if (std::equal(seq.begin(), seq.end(), input.begin()))
    {
      return std::pair(seq, input.subspan(seq.size()));
    }
    return std::nullopt;
  };
}

/**
 * Create a parser skipping n bytes.
 * @param n The number of bytes to skip.
 * @return A parser of type: i -> optional<(unit, i)>
 */
constexpr inline auto skip(size_t n)
{
  return [n](parse_input_t input) -> parse_result_t<unit> {
    if (input.size() >= n)
    {
      return std::pair(unit{}, input.subspan(n));
    }
    return std::nullopt;
  };
}

/**
 * Create a parser of n bytes.
 * @param n The number of bytes to parse.
 * @return A parser of type: i -> optional<(span, i)>
 */
constexpr inline auto n_bytes(size_t n)
{
  return [n](parse_input_t input) -> parse_result_t<std::span<const std::byte>> {
    if (input.size() < n)
    {
      return std::nullopt;
    }
    return std::pair(input.subspan(0, n), input.subspan(n));
  };
}

/**
 * Create a parser of an arithmetic values of type T using the given endianness.
 * @return A parser of type: i -> optional<(t, i)>
 */
template <typename T>
concept arithmetic = std::integral<T> || std::floating_point<T>;
template <arithmetic T, std::endian FROM_ENDIAN = std::endian::big>
constexpr inline auto arithmetic_parser()
{
  return [](parse_input_t input) -> parse_result_t<T> {
    static_assert(
      std::endian::native == std::endian::little || std::endian::native == std::endian::big,
      "Only little en big endian platforms are supported.");
    constexpr auto size = sizeof(T);
    if (input.size() < size)
    {
      return std::nullopt;
    }

    T value = 0;
    if constexpr (FROM_ENDIAN == std::endian::native)
    {
      std::copy(input.begin(), std::next(input.begin(), size), reinterpret_cast<std::byte*>(&value));
    }
    else
    {
      std::reverse_copy(input.begin(), std::next(input.begin(), size), reinterpret_cast<std::byte*>(&value));
    }
    return std::pair(value, input.subspan(size));
  };
}

/**
 * Apply a function to the result of a parser.
 * @tparam F A function from a to b: a -> b
 * @tparam P A parser of a: i -> optional<(a, i)>
 * @return A parser of type: i -> optional<(b, i)>
 */
template <typename F, typename P>
constexpr inline auto fmap(F&& f, P&& p)
{
  using R = parse_result_t<decltype(f(details::parsed_t<P>{}))>;
  return [f = std::forward<F>(f), p = std::forward<P>(p)](parse_input_t data) -> R {
    auto r = p(data);
    if (!r)
    {
      return std::nullopt;
    }
    return std::make_pair(f(r->first), r->second);
  };
}

/**
 * Combine multiple parsers through a combining function.
 *
 * If one of the parser fails, the combined parser fails. If every parser succeeds, f is called with all the results
 * to provide the final result.
 *
 * @tparam F A function of type: 't1 -> t2 -> ... -> tN -> a' where t1 to tN are the results of parsers P1 to PN.
 * @tparam Ps The parsers to combine.
 * @return A parser of type: i -> optional<(a, i)>
 */
template <typename F, typename... Ps>
constexpr inline auto combine(F&& f, Ps&&... ps)
{
  auto combiner = details::make_combiner(std::forward<Ps>(ps)...);
  using T = std::invoke_result_t<F, details::parsed_t<Ps>...>;
  return [f = std::forward<F>(f), combiner = std::move(combiner)](parse_input_t data) -> parse_result_t<T> {
    auto result = combiner(data);
    if (!result)
    {
      return std::nullopt;
    }
    return std::make_pair(std::apply(f, result->first), result->second);
  };
}

/**
 * Try the first parser and, if it fails try the second one.
 * @tparam P1 A parser of a: sv -> optional<(a, sv)>.
 * @tparam P2 A parser of a: sv -> optional<(a, sv)>.
 * @return A parser of a: sv -> optional<(a, sv)>.
 */
template <
  typename P1, typename P2,
  typename = std::enable_if_t<std::is_invocable_v<P1, parse_input_t> && std::is_invocable_v<P2, parse_input_t>>>
constexpr inline auto operator||(P1&& p1, P2&& p2)
{
  static_assert(
    std::is_same<details::parsed_t<P1>, details::parsed_t<P2>>::value,
    "Both parser used in a || should parse the same type.");
  using T = details::parsed_t<P1>;
  return [p1 = std::forward<P1>(p1), p2 = std::forward<P2>(p2)](parse_input_t data) -> parse_result_t<T> {
    if (data.empty())
    {
      return std::nullopt;
    }
    auto r1 = p1(data);
    if (r1)
      return r1;
    return p2(data);
  };
}

} // namespace parse_it

#endif
