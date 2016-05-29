#pragma once

/* Author: jrmwng @ 2016 */

#include <type_traits>
#include <utility>
#include <numeric>

namespace jrmwng
{
	template <typename Tfunc, typename Tinteger, Tinteger... tInteger>
	constexpr void for_each_integer_sequence(std::integer_sequence<Tinteger, tInteger...>, Tfunc && tFunc)
	{
		using type = bool [];
		(void) type { (std::forward<Tfunc>(tFunc)(std::integral_constant<Tinteger, tInteger>()), false)... };
	}
	template <typename Tfunc, typename Tinteger, Tinteger... tInteger>
	constexpr void for_each(std::integer_sequence<Tinteger, tInteger...>, Tfunc && tFunc)
	{
		using type = bool [];
		(void) type { (std::forward<Tfunc>(tFunc)(std::integral_constant<Tinteger, tInteger>()), false)... };
	}
}
