#pragma once

/* Author: jrmwng @ 2015 */

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

template <typename TR, typename TF>
typename std::enable_if<std::is_void<TR>::value, void>::type tsx(TF const & tF)
{
#ifdef RTM
	_xbegin();
	tF();
	if (_xtest())
		_xend();
#else
	tF();
#endif
}
template <typename TR, typename TF>
typename std::enable_if<!std::is_void<TR>::value, TR>::type tsx(TF const & tF)
{
#ifdef RTM
	_xbegin();
	TR tR = std::move(tF());
	if (_xtest())
		_xend();
	return std::move(tR);
#else
	return std::move(tF());
#endif
}
