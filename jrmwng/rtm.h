#pragma once

/* Author: jrmwng @ 2015-2016 */

#include <intrin.h>
#include <type_traits>

namespace jrmwng
{
	template <typename TR, typename T1, typename T2>
	std::enable_if_t<std::is_void<TR>::value, void> rtm(T1 const & t1, T2 const & t2)
	{
#ifdef RTM
		unsigned const uXBEGIN = _xbegin();

		if (uXBEGIN == _XBEGIN_STARTED)
		{
			t2();
			_xend();
		}
		else
#endif
		{
			t1();
		}
	}
	template <typename TR, typename T1, typename T2>
	std::enable_if_t<!std::is_void<TR>::value, TR> rtm(T1 const & t1, T2 const & t2)
	{
#ifdef RTM
		unsigned const uXBEGIN = _xbegin();

		if (uXBEGIN == _XBEGIN_STARTED)
		{
			TR tR(std::move(t2()));
			_xend();
			return std::move(tR);
		}
		else
#endif
		{
			return std::move(t1());
		}
	}

	template <typename TR, typename T1>
	std::enable_if_t<std::is_void<TR>::value, void> rtm(T1 const & t1)
	{
		rtm<TR>(t1, t1);
	}
	template <typename TR, typename T1>
	std::enable_if_t<!std::is_void<TR>::value, TR> rtm(T1 const & t1)
	{
		return std::move(rtm<TR>(t1, t1));
	}
}
