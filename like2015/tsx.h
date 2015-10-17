#pragma once

/* Author: jrmwng @ 2015 */

#include <intrin.h>
#include <type_traits>

#ifndef TSX
#ifdef __AVX2__
#define TSX
#endif
#endif

template <typename TR, typename TF>
typename std::enable_if<std::is_void<TR>::value, void>::type tsx(TF const & tF)
{
#ifdef TSX
	_xbegin();
	tF();
	if (_xtest()) // should I use result of _xbegin() instead?
		xend();
#else
	tF();
#endif
}
template <typename TR, typename TF>
typename std::enable_if<!std::is_void<TR>::value, TR>::type tsx(TF const & tF)
{
#ifdef TSX
	_xbegin();
	register TR tR = std::move(tF());
	if (_xtest()) // should I use result of _xbegin() instead?
		_xend();
	return std::move(tR);
#else
	return std::move(tF());
#endif
}
