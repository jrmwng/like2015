#pragma once

/* Author: jrmwng @ 2016 */

#include <tuple>

namespace jrmwng
{
	namespace internals
	{
		template <size_t uStart, size_t uEnd, size_t uSize = uEnd - uStart>
		struct for_each_tuple_s
			: for_each_tuple_s<uStart, (uStart + uEnd) / 2>
			, for_each_tuple_s<(uStart + uEnd) / 2, uEnd>
		{
			template <typename TTuple, typename TFunc>
			static void apply(TTuple const & stTuple, TFunc const & tFunc)
			{
				for_each_tuple_s<uStart, (uStart + uEnd) / 2>::apply(stTuple, tFunc);
				for_each_tuple_s<(uStart + uEnd) / 2, uEnd>::apply(stTuple, tFunc);
			}
		};
		template <size_t uStart, size_t uEnd>
		struct for_each_tuple_s<uStart, uEnd, 2>
		{
			template <typename TTuple, typename TFunc>
			static void apply(TTuple const & stTuple, TFunc const & tFunc)
			{
				tFunc(std::get<uStart + 0>(stTuple));
				tFunc(std::get<uStart + 1>(stTuple));
			}
		};
		template <size_t uStart, size_t uEnd>
		struct for_each_tuple_s<uStart, uEnd, 1>
		{
			template <typename TTuple, typename TFunc>
			static void apply(TTuple const & stTuple, TFunc const & tFunc)
			{
				tFunc(std::get<uStart>(stTuple));
			}
		};
		template <size_t uStart, size_t uEnd>
		struct for_each_tuple_s<uStart, uEnd, 0>
		{
			template <typename TTuple, typename TFunc>
			static void apply(TTuple const & stTuple, TFunc const & tFunc)
			{}
		};
	}
	template <typename Ttuple, typename TFunc>
	void for_each_tuple(Ttuple const & stTuple, TFunc const & tFunc)
	{
		internals::for_each_tuple_s<0, std::tuple_size<Ttuple>::value>::apply(stTuple, tFunc);
	}
	template <typename TFunc, typename... Ttuple>
	void for_each(std::tuple<Ttuple...> const & stTuple, TFunc const & tFunc)
	{
		for_each_tuple(stTuple, tFunc);
	}

	namespace internals
	{
		template <typename Ttuple, typename Tfunc, size_t... uIndex>
		auto transform_tuple(Ttuple && tTuple, Tfunc && tFunc, std::index_sequence<uIndex...>)
		{
			return std::make_tuple(std::forward<Tfunc>(tFunc)(std::get<uIndex>(tTuple))...);
		}
	}
	template <typename Ttuple, typename Tfunc>
	auto transform_tuple(Ttuple && tTuple, Tfunc && tFunc)
	{
		return internals::transform_tuple(std::forward<Ttuple>(tTuple), std::forward<Tfunc>(tFunc), std::make_index_sequence<std::tuple_size<std::decay_t<Ttuple>>::value>());
	}
	template <typename Tinput, typename Tfunc, typename... Toutput>
	void transform(Tinput && tInput, std::tuple<Toutput...> & tOutput, Tfunc && tFunc)
	{
		tOutput = internals::transform_tuple(std::forward<Tinput>(tInput), std::forward<Tfunc>(tFunc), std::index_sequence_for<Toutput...>());
	}

	namespace internals
	{
		template <typename Tfunc, typename Ttuple, size_t... uIndex>
		static auto apply_tuple(Tfunc && tFunc, Ttuple && tTuple, std::index_sequence<uIndex...>)
		{
			return std::forward<Tfunc>(tFunc)(std::get<uIndex>(std::forward<Ttuple>(tTuple))...);
		}
	}
	template <typename Tfunc, typename Ttuple>
	auto apply_tuple(Tfunc && tFunc, Ttuple && tTuple)
	{
		return internals::apply_tuple(std::forward<Tfunc>(tFunc), std::forward<Ttuple>(tTuple), std::make_index_sequence<std::tuple_size<std::decay_t<Ttuple>>::value>());
	}
	template <typename Tfunc, typename... Ttuple>
	auto apply(Tfunc && tFunc, std::tuple<Ttuple...> && tTuple)
	{
		return apply_tuple(tFunc, tTuple);
	}
}
