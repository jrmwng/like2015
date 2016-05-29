#pragma once

/* Author: jrmwng @ 2016 */

#include <type_traits>
#include <utility>

namespace jrmwng
{
	template <typename Tfunc, typename Tinteger, Tinteger... tInteger>
	void for_each_integer_sequence(std::integer_sequence<Tinteger, tInteger...>, Tfunc && tFunc)
	{
		auto const an = { (std::forward<Tfunc>(tFunc)(std::integral_constant<Tinteger, tInteger>()), 0)... };
	}
	template <typename Tfunc, typename Tinteger, Tinteger... tInteger>
	void for_each(std::integer_sequence<Tinteger, tInteger...>, Tfunc && tFunc)
	{
		auto const an = { (std::forward<Tfunc>(tFunc)(std::integral_constant<Tinteger, tInteger>()), 0)... };
	}
}
