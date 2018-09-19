#pragma once
#ifndef PARSE_IT_PARSER_DETAILS_H
#define PARSE_IT_PARSER_DETAILS_H

/**
 * Implementation details used by parsers.
 *
 * @see parser.h for more information about parsers.
 */

#include "parser_types.h"

namespace parse_it::details {

// Get the return type of parser P: optional<pair<T, parse_input_t>>.
template<typename P>
using parser_opt_pair_t = std::result_of_t<P(parse_input_t)>;
// Get the pair type contained in parser P's return type: pair<T, parse_input_t>.
template<typename P>
using parser_pair_t = typename parser_opt_pair_t<P>::value_type;
// Get type parsed by parser P.
template<typename P>
using parsed_t = typename parser_pair_t<P>::first_type;

/**
 * Utility class combining multiple parsers together.
 *
 * This class has a call operator executing every parser one after the other and combining their result
 * in a tuple. For parser P1 to PN, executing combiner<P1, ..., PN>(data) is equivalent to executing:
 *  auto r1 = P1(data);
 *  auto r2 = P2(r1.second);
 *  ...
 *  auto rN = PN(rN-1.second);
 *  auto result = tuple(r1.first, r2.first, ..., rN.first);
 * If any of the parsers fails, the combiner fails.
 *
 * @tparam Parsers The parsers to combine.
 */
template<typename... Parsers>
class combiner;

/**
 * Specialisation of combiner for multiple parsers.
 * @tparam Head The first parser.
 * @tparam Tail The rest of the parsers.
 */
template<typename Head, typename... Tail>
class combiner<Head, Tail...>
{
    using Result = parse_result_t<std::tuple<parsed_t<Head>, parsed_t<Tail>...>>;
    std::decay_t<Head> p_;
    combiner<Tail...> tail_;

public:
    template<typename H, typename... Ts, typename = typename std::enable_if<(sizeof...(Ts) > 0)>::type>
    combiner(H &&head, Ts &&... tail):
        p_{std::forward<H>(head)},
        tail_{std::forward<Ts>(tail)...}
    {}

    combiner(combiner&& other) = default;
    combiner(const combiner& other) = default;


    auto operator()(parse_input_t data) const -> Result
    {
        auto r = p_(data);
        if (!r) {
            return std::nullopt;
        }
        auto tail_result = tail_(r->second);
        if (!tail_result) return std::nullopt;
        return std::make_pair(std::tuple_cat(std::make_tuple(r->first), tail_result->first), tail_result->second);
    }
};

/**
 * Specialization of combiner for one parser.
 * @tparam Parser The parser.
 */
template<typename Parser>
class combiner<Parser>
{
    std::decay_t<Parser> p_;

public:
    using Result = parse_result_t<std::tuple<parsed_t<Parser>>>;

    template<typename P>
    combiner(P p): p_{std::move(p)} {}

    combiner(combiner&& other) = default;
    combiner(const combiner& other) = default;

    auto operator()(parse_input_t data) const -> Result
    {
        auto r = p_(data);
        if (!r) return std::nullopt;
        return std::make_pair(std::make_tuple(r->first), r->second);
    }
};

/**
 * Combiner factory.
 * @tparam Parsers The parsers to combine.
 * @return A combiner of parsers.
 */
template<typename... Parsers>
combiner<Parsers...> make_combiner(Parsers &&... parsers)
{
    return combiner<Parsers...>{parsers...};
}

}

#endif