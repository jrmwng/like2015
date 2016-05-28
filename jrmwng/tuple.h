#pragma once

/* Author: jrmwng @ 2016 */

#include <tuple>

namespace jrmwng
{
	namespace internals
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
	}
	template <typename Ttuple, typename TFunc>
	void for_each_tuple(Ttuple const & stTuple, TFunc const & tFunc)
	{
		internals::for_each_tuple_s<std::tuple_size<Ttuple>::value>::apply(stTuple, tFunc);
	}
	template <typename TFunc, typename... Ttuple>
	void for_each(std::tuple<Ttuple...> const & stTuple, TFunc const & tFunc)
	{
		for_each_tuple(stTuple, tFunc);
	}

	namespace internals
	{
		template <size_t szIndex>
		struct transform_tuple_s
			: transform_tuple_s<szIndex - 1>
		{
			template <typename Ttuple, typename Tfunc>
			static auto apply(Ttuple const & tTuple, Tfunc const & tFunc)
			{
				return std::tuple_cat(transform_tuple_s<szIndex - 1>::apply(tTuple, tFunc), tFunc(std::get<szIndex - 1>(tTuple)));
			}
		};
		template <>
		struct transform_tuple_s<0>
		{
			template <typename Ttuple, typename Tfunc>
			static auto apply(Ttuple const & tTuple, Tfunc const & tFunc)
			{
				return std::tuple<>();
			}
		};
	}
	template <typename Ttuple, typename Tfunc>
	auto transform_tuple(Ttuple const & stTuple, Tfunc const & tFunc)
	{
		return internals::transform_tuple_s::apply(stTuple, tFunc);
	}
	template <typename Ttuple, typename Tfunc, typename... Toutput>
	void transform(Ttuple const & stInput, std::tuple<Toutput...> & stOutput, Tfunc const & stFunc)
	{
		stOutput = transform_tuple(stInput, tFunc);
	}

	namespace internals
	{
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
	}
	template <typename TTuple, typename TFunc, typename Ttransform>
	auto unpack_tuple(TTuple const & stTuple, TFunc const & tFunc, Ttransform const & tTransform)
	{
		return internals::unpack_tuple_s<std::tuple_size<TTuple>::value>::apply(stTuple, tFunc, tTransform);
	}
}
