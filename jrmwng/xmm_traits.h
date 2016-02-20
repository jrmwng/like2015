#pragma once

/* Author: jrmwng @ 2015~2016 */

#include <intrin.h>
#include <stdint.h>
#include <utility>

namespace jrmwng
{
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
		typedef __m128i xmm1_type;

		typedef std::pair<ub16_type, ub16_type> ub32_type;
		typedef std::pair<b16_type, b16_type> b32_type;
		typedef std::pair<uw8_type, uw8_type> uw16_type;
		typedef std::pair<w8_type, w8_type> w16_type;
		typedef std::pair<ul4_type, ul4_type> ul8_type;
		typedef std::pair<l4_type, l4_type> l8_type;
		typedef std::pair<uxl2_type, uxl2_type> uxl4_type;
		typedef std::pair<xl2_type, xl2_type> xl4_type;
		typedef std::pair<xmm1_type, xmm1_type> xmm2_type;

		template <typename T>
		static std::enable_if_t<std::is_integral<T>::value, __m128i> xmm1_headmask(int n)
		{
			__m128i const axmm[2]{ _mm_setzero_si128(), _mm_set1_epi32(~0) };
			return _mm_loadu_si128(reinterpret_cast<__m128i const*>(axmm[0].m128i_i8 + (n & (16 / sizeof(T) - 1)) * sizeof(T)));
		}
		template <typename T>
		static std::enable_if_t<std::is_integral<T>::value, __m128i> xmm1_tailmask(int n)
		{
			__m128i const axmm[2]{ _mm_set1_epi32(~0), _mm_setzero_si128() };
			return _mm_loadu_si128(reinterpret_cast<__m128i const*>(axmm[1].m128i_i8 - (n & (16 / sizeof(T) - 1)) * sizeof(T)));
		}

		template <typename T, bool bAlign = false>
		static std::enable_if_t<std::is_integral<T>::value, typename xmm1_type> xmm1_load(T const *pt)
		{
			if ((!bAlign) && reinterpret_cast<uintptr_t>(pt) & (16 - 1))
				return _mm_loadu_si128(reinterpret_cast<__m128i const*>(pt));
			else
				return _mm_load_si128(reinterpret_cast<__m128i const*>(pt));
		}
		template <typename T, bool bAlign = false>
		static std::enable_if_t<std::is_integral<T>::value, typename xmm2_type> xmm2_load(T const *pt)
		{
			if ((!bAlign) && reinterpret_cast<uintptr_t>(pt) & (16 - 1))
			{
				__m128i const xmm1A = _mm_loadu_si128(reinterpret_cast<__m128i const*>(pt) + 0);
				__m128i const xmm1B = _mm_loadu_si128(reinterpret_cast<__m128i const*>(pt) + 1);
				return xmm2_type(xmm1A, xmm1B);
			}
			else
			{
				__m128i const xmm1A = _mm_load_si128(reinterpret_cast<__m128i const*>(pt) + 0);
				__m128i const xmm1B = _mm_load_si128(reinterpret_cast<__m128i const*>(pt) + 1);
				return xmm2_type(xmm1A, xmm1B);
			}
		}

		template <typename T, bool bAlign = false>
		static std::enable_if_t<std::is_integral<T>::value, void> xmm1_store(T *pt, typename xmm1_type const & xmm)
		{
			if ((!bAlign) && reinterpret_cast<uintptr_t>(pt) & (16 - 1))
				_mm_storeu_si128(reinterpret_cast<__m128i*>(pt), xmm);
			else
				_mm_store_si128(reinterpret_cast<__m128i*>(pt), xmm);
		}
		template <typename T, bool bAlign = false>
		static std::enable_if_t<std::is_integral<T>::value, void> xmm1_store(T *pt, typename xmm1_type const & xmm1, int n, __m128i const & xmmMask)
		{
			if (n >= 16 / sizeof(T))
			{
				xmm1_store<T, bAlign>(pt, xmm1);
			}
			else // if (n & (16 - 1))
			{
				_mm_maskmoveu_si128(xmm1, xmmMask, reinterpret_cast<char*>(pt));
			}
		}
		template <typename T, bool bAlign = false>
		static std::enable_if_t<std::is_integral<T>::value, void> xmm2_store(T *pt, typename xmm2_type const & xmm2)
		{
			if ((!bAlign) && reinterpret_cast<uintptr_t>(pt) & (16 - 1))
			{
				_mm_storeu_si128(reinterpret_cast<__m128i*>(pt) + 0, xmm2.first);
				_mm_storeu_si128(reinterpret_cast<__m128i*>(pt) + 1, xmm2.second);
			}
			else
			{
				_mm_store_si128(reinterpret_cast<__m128i*>(pt) + 0, xmm2.first);
				_mm_store_si128(reinterpret_cast<__m128i*>(pt) + 1, xmm2.second);
			}
		}
		template <typename T, bool bAlign = false>
		static std::enable_if_t<std::is_integral<T>::value, void> xmm2_store(T *pt, typename xmm2_type const & xmm2, int n, __m128i const & xmmMask)
		{
			__m128i xmmD = xmm2.first;

			if (n >= 16 / sizeof(T))
			{
				xmmD = xmm2.second;

				if ((!bAlign) && reinterpret_cast<uintptr_t>(pt) & (16 - 1))
				{
					_mm_storeu_si128(reinterpret_cast<__m128i*>(pt) + 0, xmm2.first);

					if (n >= 32 / sizeof(T))
					{
						_mm_storeu_si128(reinterpret_cast<__m128i*>(pt) + 1, xmm2.second);
						return;
					}
				}
				else
				{
					_mm_store_si128(reinterpret_cast<__m128i*>(pt) + 0, xmm2.first);

					if (n >= 32 / sizeof(T))
					{
						_mm_store_si128(reinterpret_cast<__m128i*>(pt) + 1, xmm2.second);
						return;
					}
				}
			}

			if (n & (16 / sizeof(T) - 1))
			{
				_mm_maskmoveu_si128(xmmD, xmmMask, reinterpret_cast<char*>(pt) + (n & (16 / sizeof(T))));
			}
		}
	};
}
