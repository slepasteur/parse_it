#pragma once
#ifndef PARSE_IT_PARSER_H
#define PARSE_IT_PARSER_H

#include "parser_types.h"

#include <iostream>
#include <iterator>
#include <type_traits>

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
        if (input[0] == b) return std::pair(b, input.subspan(1));
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
 * Create a parser of an uint8 values.
 * @return A parser of type: i -> optional<(uint8, i)>
 */
constexpr inline auto uint8_parser()
{
    return [](parse_input_t input) -> parse_result_t<std::uint8_t> {
        if (input.size() < 1)
        {
            return std::nullopt;
        }
        return std::pair(std::to_integer<std::uint8_t>(input[0]), input.subspan(1));
    };
}

/**
 * Create a parser of an uint16 values in little endian format.
 * @return A parser of type: i -> optional<(uint16, i)>
 */
constexpr inline auto uint16_parser()
{
    return [](parse_input_t input) -> parse_result_t<std::uint16_t> {
        if (input.size() < 2)
        {
            return std::nullopt;
        }
        std::uint16_t value = 
            std::to_integer<std::uint8_t>(input[0])
            | std::to_integer<std::uint8_t>(input[1]) << 8;
        return std::pair(value, input.subspan(2));
    };
}

/**
 * Create a parser of an uint32 value in little endian format.
 * @return A parser of type: i -> optional<(uint32, i)>
 */
constexpr inline auto uint32_parser()
{
    return [](parse_input_t input) -> parse_result_t<std::uint32_t> {
        if (input.size() < 4)
        {
            return std::nullopt;
        }
        std::uint32_t value = 
            std::to_integer<std::uint8_t>(input[0])
            | std::to_integer<std::uint8_t>(input[1]) << 8
            | std::to_integer<std::uint8_t>(input[2]) << 16
            | std::to_integer<std::uint8_t>(input[3]) << 24;
        return std::pair(value, input.subspan(4));
    };
}

}

#endif