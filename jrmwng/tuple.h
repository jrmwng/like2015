#pragma once

/* Author: jrmwng @ 2016 */

#include <tuple>

namespace jrmwng
{
	namespace internals
	{
		template <typename Ttuple, typename Tfunc, size_t... uIndex>
		void for_each_tuple(Ttuple && tTuple, Tfunc && tFunc, std::index_sequence<uIndex...>)
		{
			std::make_tuple((std::forward<Tfunc>(tFunc)(std::get<uIndex>(tTuple)), 0)...);
		}
	}
	template <typename Ttuple, typename Tfunc>
	void for_each_tuple(Ttuple && tTuple, Tfunc && tFunc)
	{
		internals::for_each_tuple(std::forward<Ttuple>(tTuple), std::forward<Tfunc>(tFunc), std::make_index_sequence<std::tuple_size<std::decay_t<Ttuple>>::value>());
	}
	template <typename Tfunc, typename... Ttuple>
	void for_each(std::tuple<Ttuple...> && tTuple, Tfunc && tFunc)
	{
		internals::for_each_tuple(std::forward<std::tuple<Ttuple...>>(tTuple), std::forward<Tfunc>(tFunc), std::index_sequence_for<Ttuple...>());
	}
	template <typename Tfunc, typename... Ttuple>
	void for_each(std::tuple<Ttuple...> const & tTuple, Tfunc && tFunc)
	{
		internals::for_each_tuple(tTuple, std::forward<Tfunc>(tFunc), std::index_sequence_for<Ttuple...>());
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
		return internals::apply_tuple(std::forward<Tfunc>(tFunc), std::forward<std::tuple<Ttuple...>>(tTuple), std::index_sequence_for<Ttuple...>());
	}
	template <typename Tfunc, typename... Ttuple>
	auto apply(Tfunc && tFunc, std::tuple<Ttuple...> const & tTuple)
	{
		return internals::apply_tuple(std::forward<Tfunc>(tFunc), tTuple, std::index_sequence_for<Ttuple...>());
	}
}
