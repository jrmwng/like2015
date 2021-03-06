#pragma once

/* Author: jrmwng @ 2016 */

#include <tuple>

namespace jrmwng
{
	namespace
	{
		template <typename Ttuple, typename Tfunc, size_t... uIndex>
		constexpr void for_each_tuple(Ttuple && tTuple, Tfunc && tFunc, std::index_sequence<uIndex...>)
		{
			std::make_tuple((std::forward<Tfunc>(tFunc)(std::get<uIndex>(tTuple)), 0)...);
		}
	}
	template <typename Ttuple, typename Tfunc>
	constexpr void for_each_tuple(Ttuple && tTuple, Tfunc && tFunc)
	{
		for_each_tuple(std::forward<Ttuple>(tTuple), std::forward<Tfunc>(tFunc), std::make_index_sequence<std::tuple_size<std::decay_t<Ttuple>>::value>());
	}
	template <typename Tfunc, typename... Ttuple>
	constexpr void for_each(std::tuple<Ttuple...> && tTuple, Tfunc && tFunc)
	{
		for_each_tuple(std::forward<std::tuple<Ttuple...>>(tTuple), std::forward<Tfunc>(tFunc), std::index_sequence_for<Ttuple...>());
	}
	template <typename Tfunc, typename... Ttuple>
	constexpr void for_each(std::tuple<Ttuple...> const & tTuple, Tfunc && tFunc)
	{
		for_each_tuple(tTuple, std::forward<Tfunc>(tFunc), std::index_sequence_for<Ttuple...>());
	}

	namespace
	{
		template <typename Ttuple, typename Tfunc, size_t... uIndex>
		constexpr auto transform_tuple(Ttuple && tTuple, Tfunc && tFunc, std::index_sequence<uIndex...>)
		{
			return std::make_tuple(std::forward<Tfunc>(tFunc)(std::get<uIndex>(tTuple))...);
		}
	}
	template <typename Ttuple, typename Tfunc>
	constexpr auto transform_tuple(Ttuple && tTuple, Tfunc && tFunc)
	{
		return transform_tuple(std::forward<Ttuple>(tTuple), std::forward<Tfunc>(tFunc), std::make_index_sequence<std::tuple_size<std::decay_t<Ttuple>>::value>());
	}
	template <typename Tinput, typename Tfunc, typename... Toutput>
	constexpr void transform(Tinput && tInput, std::tuple<Toutput...> & tOutput, Tfunc && tFunc)
	{
		tOutput = transform_tuple(std::forward<Tinput>(tInput), std::forward<Tfunc>(tFunc), std::index_sequence_for<Toutput...>());
	}

	namespace
	{
		template <typename Tfunc, typename Ttuple, size_t... uIndex>
		constexpr auto apply_tuple(Tfunc && tFunc, Ttuple && tTuple, std::index_sequence<uIndex...>)
		{
			return std::forward<Tfunc>(tFunc)(std::get<uIndex>(std::forward<Ttuple>(tTuple))...);
		}
	}
	template <typename Tfunc, typename Ttuple>
	constexpr auto apply_tuple(Tfunc && tFunc, Ttuple && tTuple)
	{
		return apply_tuple(std::forward<Tfunc>(tFunc), std::forward<Ttuple>(tTuple), std::make_index_sequence<std::tuple_size<std::decay_t<Ttuple>>::value>());
	}
	template <typename Tfunc, typename... Ttuple>
	constexpr auto apply(Tfunc && tFunc, std::tuple<Ttuple...> && tTuple)
	{
		return apply_tuple(std::forward<Tfunc>(tFunc), std::forward<std::tuple<Ttuple...>>(tTuple), std::index_sequence_for<Ttuple...>());
	}
	template <typename Tfunc, typename... Ttuple>
	constexpr auto apply(Tfunc && tFunc, std::tuple<Ttuple...> const & tTuple)
	{
		return apply_tuple(std::forward<Tfunc>(tFunc), tTuple, std::index_sequence_for<Ttuple...>());
	}
}
