#pragma once
#ifndef PARSE_IT_UTILS_UTILS_H
#define PARSE_IT_UTILS_UTILS_H

#include <cstddef>
#include <iterator>

namespace parse_it {

constexpr std::byte operator"" _b (unsigned long long b)
{
    return static_cast<std::byte>(b);
}

struct big_endian {};
struct little_endian {};

namespace details
{

struct const_view {};
struct mutable_view {};

template <typename Endianness, typename Constness>
class byte_sequence_base
{
public:
	using base_iterator_type = typename std::conditional<
		std::is_same<Constness, const_view>::value,
		const std::byte*,
		std::byte*
	>::type;

	using iterator_type = typename std::conditional<
		std::is_same<Endianness, big_endian>::value,
		std::reverse_iterator<base_iterator_type>,
		base_iterator_type
	>::type;

	template <typename T>
	explicit byte_sequence_base(T& data):
		m_begin(reinterpret_cast<base_iterator_type>(&data)),
		m_size(sizeof(T))
	{
		static_assert(std::is_trivial<T>::value, "Only trivial types can be viewed as byte ranges.");
	}

	size_t size() const { return m_size; }
	iterator_type begin() const { return begin(Endianness{}); }
	iterator_type end() const { return end(Endianness{}); }

private:
	iterator_type begin(little_endian) const { return iterator_type{first()}; }
	iterator_type end(little_endian) const { return iterator_type{last()}; }
	iterator_type begin(big_endian) const { return std::make_reverse_iterator(last()); }
	iterator_type end(big_endian) const { return std::make_reverse_iterator(first()); }

	base_iterator_type first() const { return m_begin; }
	base_iterator_type last() const { return m_begin + m_size; }

	base_iterator_type m_begin;
	const size_t m_size;
};

}

/**
 * Byte view is a range giving read-only access to the bytes of the input type.
 * Requirements: input type must be trivial.
 * Endianness can be provided via the template parameter (either little_endian or big_endian).
 */
template <typename Endianness = little_endian>
class byte_view : public details::byte_sequence_base<Endianness, details::const_view>
{
public:
	using details::byte_sequence_base<Endianness, details::const_view>::byte_sequence_base;
};

/**
 * Byte view is a range giving read/write access to the bytes of the input type.
 * Requirements: input type must be trivial.
 * Endianness can be provided via the template parameter (either little_endian or big_endian).
 */
template <typename Endianness = little_endian>
class byte_span : public details::byte_sequence_base<Endianness, details::mutable_view>
{
public:
	using details::byte_sequence_base<Endianness, details::mutable_view>::byte_sequence_base;
};

}

#endif