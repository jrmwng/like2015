#pragma once

/* Author: jrmwng @ 2015 */

#include <intrin.h>

namespace like
{
	template <unsigned uIndex, unsigned uSize>
	struct xmm_int32_c11;

	template <unsigned uIndex>
	struct xmm_int32_c11<uIndex, 0>
	{
		__m128i xmm;

		xmm_int32_c11(__m128i const & that)
			: xmm(that)
		{}
	};

	template <unsigned uIndex>
	struct xmm_int32_c11<uIndex, 1>
		: xmm_int32_c11<uIndex, 0>
	{
		typedef xmm_int32_c11<uIndex, 0> base_type;

		xmm_int32_c11(__m128i const & that)
			: base_type(that)
		{}
		xmm_int32_c11(int n)
			: base_type(_mm_cvtsi32_si128(n))
		{}
	};

	template <unsigned uIndex>
	struct xmm_int32_c11<uIndex, 2>
		: xmm_int32_c11<uIndex, 1>
	{
		typedef xmm_int32_c11<uIndex, 1> base_type;

		xmm_int32_c11(__m128i const & that)
			: base_type(that)
		{}
		template <typename T1, typename T2>
		xmm_int32_c11(T1 t1, T2 t2)
			: base_type(_mm_unpacklo_epi32(xmm_int32_c11<0, 1>(t1).xmm, xmm_int32_c11<0, 1>(t2).xmm))
		{}
	};

	template <unsigned uIndex>
	struct xmm_int32_c11<uIndex, 3>
		: xmm_int32_c11<uIndex, 2>
	{
		typedef xmm_int32_c11<uIndex, 2> base_type;

		xmm_int32_c11(__m128i const & that)
			: base_type(that)
		{}
		template <typename T1, typename T2, typename T3>
		xmm_int32_c11(T1 t1, T2 t2, T3 t3)
			: base_type(_mm_unpacklo_epi64(xmm_int32_c11<0, 2>(t1, t2).xmm, xmm_int32_c11<0, 1>(t3).xmm))
		{}
	};
	template <unsigned uIndex>
	struct xmm_int32_c11<uIndex, 4>
		: xmm_int32_c11<uIndex, 3>
	{
		typedef xmm_int32_c11<uIndex, 3> base_type;

		xmm_int32_c11(__m128i const & that)
			: base_type(that)
		{}
		template <typename T1, typename T2, typename T3, typename T4>
		xmm_int32_c11(T1 t1, T2 t2, T3 t3, T4 t4)
			: base_type(_mm_unpacklo_epi64(xmm_int32_c11<0, 2>(t1, t2).xmm, xmm_int32_c11<0, 2>(t3, t4).xmm))
		{}
	};
	template <unsigned uIndex, unsigned uSize>
	struct xmm_int32_c11
		: xmm_int32_c11<uIndex, 4>
		, xmm_int32_c11<uIndex + 1, uSize - 4>
	{
		typedef xmm_int32_c11<uIndex, 4> base_type;
		typedef xmm_int32_c11<uIndex + 1, uSize - 4> next_type;

		template <typename T1, typename T2, typename T3, typename T4, typename... T5>
		xmm_int32_c11(T1 t1, T2 t2, T3 t3, T4 t4, T5... t5)
			: base_type(t1, t2, t3, t4)
			, next_type(t5...)
		{}
	};

	template <unsigned uSize>
	struct xmm_int32
		: xmm_int32_c11<0, uSize>
	{
		typedef xmm_int32_c11<0, uSize> base_type;

		template <typename... TArgs>
		xmm_int32(TArgs... Args)
			: base_type(Args...)
		{}
	};

	template <unsigned uIndex, unsigned uSize>
	typename std::enable_if<(uIndex < uSize) && (uIndex % 4 != 0), int>::type get_int(xmm_int32<uSize> const & xmmPtr)
	{
		return _mm_extract_epi32(static_cast<xmm_int32_c11<uIndex / 4, 0>const&>(xmmPtr).xmm, uIndex % 4);
	}
	template <unsigned uIndex, unsigned uSize>
	typename std::enable_if<(uIndex < uSize) && (uIndex % 4 == 0), int>::type get_int(xmm_int32<uSize> const & xmmPtr)
	{
		return _mm_cvtsi128_si32(static_cast<xmm_int32_c11<uIndex / 4, 0>const&>(xmmPtr).xmm);
	}
}
