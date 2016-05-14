#pragma once

/* Author: jrmwng @ 2016 */

#include <tuple>

namespace jrmwng
{
	template <size_t szIndex>
	struct for_each_tuple_s
		: for_each_tuple_s<szIndex - 1>
	{
		template <typename TTuple, typename TFunc>
		static void apply(TTuple const & stTuple, TFunc const & tFunc)
		{
			for_each_tuple_s<szIndex - 1>::apply(stTuple, tFunc);
			tFunc(std::get<szIndex - 1>(stTuple));
		}
	};
	template <>
	struct for_each_tuple_s<0>
	{
		template <typename TTuple, typename TFunc>
		static void apply(TTuple const & stTuple, TFunc const & tFunc)
		{}
	};
	template <typename TTuple, typename TFunc>
	inline void for_each_tuple(TTuple const & stTuple, TFunc const & tFunc)
	{
		for_each_tuple_s<std::tuple_size<TTuple>::value>::apply(stTuple, tFunc);
	}
}
