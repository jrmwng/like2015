#pragma once

/* Author: jrmwng @ 2016 */

#include <type_traits>

namespace jrmwng
{
	namespace
	{
		template <typename T, bool b = std::is_class<T>::value>
		struct latency_switch
			: T
		{
			latency_switch(T const & t)
				: T(t)
			{}
		};
		template <typename T>
		struct latency_switch<T, false>
		{
			T m_tValue;

			latency_switch(T const & tValue)
				: m_tValue(tValue)
			{}

			operator T const & () const
			{
				return m_tValue;
			}
		};
		struct latency_base
		{};
	}
	template <typename T, unsigned uLatency = 0>
	struct latency_s
		: latency_switch<T>
		, latency_base
	{
		latency_s(T const & t)
			: latency_switch<T>(t)
		{}
	};
	template <typename T, unsigned uLatency, typename... Trest>
	constexpr unsigned get_latency(latency_s<T, uLatency> const &, Trest const &... tRest)
	{
		return uLatency < get_latency(tRest...) ? get_latency(tRest...) : uLatency;
	}
	template <typename T, typename... Trest>
	constexpr std::enable_if_t<!(std::is_base_of<latency_base, T>::value), unsigned> get_latency(T const &, T const &... tRest)
	{
		return get_latency(tRest...);
	}
	template <typename T, unsigned u>
	T const & remove_latency(latency_s<T, u> const & t)
	{
		return t;
	}
	template <typename T>
	std::enable_if_t<!(std::is_base_of<latency_base, T>::value), T const &> remove_latency(T const & t)
	{
		return t;
	}
	template <unsigned uLatency, typename T1, unsigned u1>
	auto latency(latency_s<T1, u1> const & t1)
	{
		return latency_s<T1, uLatency + u1>(t1);
	}
	template <unsigned uLatency, typename T1>
	std::enable_if_t<!(std::is_base_of<latency_base, T1>::value), latency_s<T1, uLatency> > latency(T1 const & t1)
	{
		return latency_s<T1, uLatency>(t1);
	}
	template <unsigned uLatency, typename Tfunc, typename... Targs>
	auto apply_latency(tFunc const & tFunc, Targs const &... tArgs)
	{
		return latency<uLatency + get_latency(tArgs...)>(tFunc(remove_latency(tArgs)...));
	}
}
