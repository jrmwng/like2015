#pragma once

/* Author: jrmwng @ 2015-2016 */

#include <intrin.h>
#include <type_traits>

#define TSX

#ifdef TSX
#ifndef RTM
#define RTM
#endif
#ifndef HLE
#define HLE
#endif
#endif

template <typename TR, typename T1, typename T2>
typename std::enable_if<std::is_void<TR>::value, void>::type tsx(T1 const & t1, T2 const & t2)
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
typename std::enable_if<!std::is_void<TR>::value, TR>::type tsx(T1 const & t1, T2 const & t2)
{
#ifdef RTM
	unsigned const uXBEGIN = _xbegin();

	if (uXBEGIN == _XBEGIN_STARTED)
	{
		TR tR = std::move(t2());
		_xend();
		return std::move(tR);
	}
#endif
	{
		return std::move(t1());
	}
}

template <typename TR, typename T1>
typename std::enable_if<std::is_void<TR>::value, void>::type tsx(T1 const & t1)
{
	tsx<TR>(t1, t1);
}
template <typename TR, typename T1>
typename std::enable_if<!std::is_void<TR>::value, TR>::type tsx(T1 const & t1)
{
	return std::move(tsx<TR>(t1, t1));
}
