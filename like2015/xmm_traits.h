#pragma once

#include <intrin.h>
#include <stdint.h>
#include <utility>

struct xmm_traits
{
	typedef uint8_t ub1_type;
	typedef int8_t b1_type;

	typedef uint16_t uw1_type;
	typedef int16_t w1_type;

	typedef uint32_t ul1_type;
	typedef int32_t l1_type;

	typedef uint64_t uxl1_type;
	typedef int64_t xl1_type;

	typedef float r1_type;
	typedef double lr1_type;

	typedef __m128i ub16_type;
	typedef __m128i b16_type;
	typedef __m128i uw8_type;
	typedef __m128i w8_type;
	typedef __m128i ul4_type;
	typedef __m128i l4_type;
	typedef __m128i uxl2_type;
	typedef __m128i xl2_type;

	typedef std::pair<ub16_type, ub16_type> ub32_type;
	typedef std::pair<b16_type, b16_type> b32_type;
	typedef std::pair<uw8_type, uw8_type> uw16_type;
	typedef std::pair<w8_type, w8_type> w16_type;
	typedef std::pair<ul4_type, ul4_type> ul8_type;
	typedef std::pair<l4_type, l4_type> l8_type;
	typedef std::pair<uxl2_type, uxl2_type> uxl4_type;
	typedef std::pair<xl2_type, xl2_type> xl4_type;

	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, __m128i>::type xmm1_input(T const *pt)
	{
		if (reinterpret_cast<uintptr_t>(pt) & (16 - 1))
			return _mm_loadu_si128(reinterpret_cast<__m128i const*>(pt));
		else
			return _mm_load_si128(reinterpret_cast<__m128i const*>(pt));
	}
	typename ub16_type ub16_input(ub1_type const *pub) { return xmm1_input(pub); }
	typename b16_type b16_input(b1_type const *pb) { return xmm1_input(pb); }
	typename uw8_type uw8_input(uw1_type const *puw) { return xmm1_input(puw); }
	typename w8_type w8_input(w1_type const *pw) { return xmm1_input(pw); }
	typename ul4_type ul4_input(ul1_type const *pul) { return xmm1_input(pul); }
	typename l4_type l4_input(l1_type const *pl) { return xmm1_input(pl); }
	typename uxl2_type uxl2_input(uxl1_type const *puxl) { return xmm1_input(puxl); }
	typename xl2_type xl2_input(xl1_type const *pxl) { return xmm1_input(pxl); }

	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, __m128i>::type xmm1_tailmask(int n)
	{
		__m128i const axmm[2]{ _mm_cmpeq_epi8(_mm_setzero_si128(), _mm_setzero_si128()), _mm_setzero_si128() };
		return _mm_loadu_si128(reinterpret_cast<__m128i const*>(axmm[1].m128i_i8 - (n & (16 / sizeof(T) - 1)) * sizeof(T)));
	}

	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, void>::type xmm1_output(T *pt, int n, __m128i const & xmm, __m128i const & xmmMask)
	{
		if (n >= 16 / sizeof(T))
		{
			if (reinterpret_cast<uintptr_t>(pt) & (16 - 1))
				_mm_storeu_si128(reinterpret_cast<__m128i*>(pt), xmm);
			else
				_mm_store_si128(reinterpret_cast<__m128i*>(pt), xmm);
		}
		else // if (n & (16 - 1))
		{
			_mm_maskmoveu_si128(xmm, xmmMask, reinterpret_cast<char*>(pt));
		}
	}
	void ub16_output(ub1_type *pub, int n, ub16_type const & ub16, __m128i const & xmmMask) { xmm1_output(pub, n, ub16, xmmMask); }
	void b16_output(ub1_type *pub, int n, b16_type const & b16, __m128i const & xmmMask) { xmm1_output(pub, n, b16, xmmMask); }
	void uw8_output(ub1_type *pub, int n, uw8_type const & uw8, __m128i const & xmmMask) { xmm1_output(pub, n, uw8, xmmMask); }
	void w8_output(ub1_type *pub, int n, w8_type const & w8, __m128i const & xmmMask) { xmm1_output(pub, n, w8, xmmMask); }
	void ul4_output(ub1_type *pub, int n, ul4_type const & ul4, __m128i const & xmmMask) { xmm1_output(pub, n, ul4, xmmMask); }
	void l4_output(ub1_type *pub, int n, l4_type const & l4, __m128i const & xmmMask) { xmm1_output(pub, n, l4, xmmMask); }
	void uxl2_output(ub1_type *pub, int n, uxl2_type const & uxl2, __m128i const & xmmMask) { xmm1_output(pub, n, uxl2, xmmMask); }
	void xl2_output(ub1_type *pub, int n, xl2_type const & xl2, __m128i const & xmmMask) { xmm1_output(pub, n, xl2, xmmMask); }
};
