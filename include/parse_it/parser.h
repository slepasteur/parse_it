#pragma once
#ifndef PARSE_IT_PARSER_H
#define PARSE_IT_PARSER_H

#include "parser_types.h"

namespace parse_it {

/**
 * Create a parser of one byte of value b.
 * @param b The byte to parse.
 * @return A parser of type: i -> optional<(b, i)>
 */
inline auto one_byte(std::byte b)
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
inline auto any_byte()
{
    return [](parse_input_t input) -> parse_result_t<std::byte> {
        if (input.empty())
        {
            return std::nullopt;
        }
        return std::pair(input[0], input.subspan(1));
    };
}

}

#endif