#pragma once
#ifndef PARSE_IT_PARSER_TYPES_H
#define PARSE_IT_PARSER_TYPES_H

/**
 * Base types used by parsers.
 *
 * @see parser.h for more information about parsers.
 */

#include <cstddef>
#include <optional>

#include <span>

namespace parse_it {

/**
 * Parser input and output types.
 */
using parse_input_t = std::span<const std::byte>;
template <typename T>
using parse_result_t = std::optional<std::pair<T, parse_input_t>>;

/**
 * A unit struct representing empty data.
 */
struct unit
{};

/**
 * Tag struct used to represent little endian parsing.
 */
struct little_endian
{};

/**
 * Tag struct used to represent big endian parsing.
 */
struct big_endian
{};

} // namespace parse_it

#endif
