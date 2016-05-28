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
	void for_each(TTuple const & stTuple, TFunc const & tFunc)
	{
		for_each_tuple_s<std::tuple_size<TTuple>::value>::apply(stTuple, tFunc);
	}

	template <size_t szIndex>
	struct unpack_tuple_s
		: unpack_tuple_s<szIndex - 1>
	{
		template <typename TTuple, typename TFunc, typename Ttransform, typename... TArgs>
		static auto apply(TTuple const & stTuple, TFunc const & tFunc, Ttransform const & tTransform, TArgs &&... tArgs)
		{
			return unpack_tuple_s<szIndex - 1>::apply(stTuple, tFunc, tTransform, tTransform(std::get<szIndex - 1>(stTuple)), std::forward<TArgs>(tArgs)...);
		}
	};
	template <> struct unpack_tuple_s<0>
	{
		template <typename TTuple, typename TFunc, typename Ttransform, typename... TArgs>
		static auto apply(TTuple const & stTuple, TFunc const & tFunc, Ttransform const & tTransform, TArgs &&... tArgs)
		{
			return tFunc(std::forward<TArgs>(tArgs)...);
		}
	};
	template <typename TTuple, typename TFunc, typename Ttransform>
	auto unpack_tuple(TTuple const & stTuple, TFunc const & tFunc, Ttransform const & tTransform)
	{
		return unpack_tuple_s<std::tuple_size<TTuple>::value>::apply(stTuple, tFunc, tTransform);
	}
}
