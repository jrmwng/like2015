#pragma once

/* Author: jrmwng @ 2015~2016 */

#include <intrin.h>
#include <stdint.h>
#include <utility>

namespace jrmwng
{
	enum xmm_feature
	{
		XMM_FEATURE_SSE,
		XMM_FEATURE_SSE_2,
		XMM_FEATURE_SSE_3,
		XMM_FEATURE_SSSE_3,
		XMM_FEATURE_SSE_4_1,
		XMM_FEATURE_SSE_4_2,
		XMM_FEATURE_AVX,
		XMM_FEATURE_AVX_2,
		XMM_FEATURE_FMA,
	};

	template <xmm_feature emTest, xmm_feature... vemFeature>
	struct xmm_feature_test;
	template <xmm_feature emTest>
	struct xmm_feature_test<emTest>
	{
		typedef std::false_type type;
	};
	template <xmm_feature emTest, xmm_feature emFeature, xmm_feature... vemFeature>
	struct xmm_feature_test<emTest, emFeature, vemFeature...>
	{
		typedef std::conditional_t<emTest == emFeature, std::true_type, typename xmm_feature_test<emTest, vemFeature...>::type> type;
	};
	template <xmm_feature emTest, xmm_feature... vemFeature>
	using xmm_feature_test_t = typename xmm_feature_test<emTest, vemFeature...>::type;

	template <xmm_feature... vemFeature>
	struct xmm_feature_set
	{
		template <xmm_feature emTest>
		using exists_t = typename xmm_feature_test<emTest, vemFeature...>::type;
	};

	template <typename T1, typename T2>
	struct alignas(16) xmm_traits_pair
		: std::pair<T1, T2>
	{
		template <typename... TArgs>
		xmm_traits_pair(TArgs &&... Args)
			: std::pair<T1, T2>(std::forward<TArgs>(Args)...)
		{}
	};

	template <xmm_feature... vemFeature>
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

		typedef xmm_traits_pair<ub16_type, ub16_type> ub32_type;
		typedef xmm_traits_pair<b16_type, b16_type> b32_type;
		typedef xmm_traits_pair<uw8_type, uw8_type> uw16_type;
		typedef xmm_traits_pair<w8_type, w8_type> w16_type;
		typedef xmm_traits_pair<ul4_type, ul4_type> ul8_type;
		typedef xmm_traits_pair<l4_type, l4_type> l8_type;
		typedef xmm_traits_pair<uxl2_type, uxl2_type> uxl4_type;
		typedef xmm_traits_pair<xl2_type, xl2_type> xl4_type;
		typedef xmm_traits_pair<xmm1_type, xmm1_type> xmm2_type;

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

		template <int nShuffle>
		static __m128 sse_shuffle_ps(__m128 const & xmmA, __m128 const & xmmB)
		{
			return _mm_shuffle_ps(xmmA, xmmB, nShuffle);
		}
		template <>
		static __m128 sse_shuffle_ps<_MM_SHUFFLE(3, 2, 3, 2)>(__m128 const & xmmA, __m128 const & xmmB)
		{
			return _mm_movehl_ps(xmmB, xmmA);
		}
		template <>
		static __m128 sse_shuffle_ps<_MM_SHUFFLE(1, 0, 1, 0)>(__m128 const & xmmA, __m128 const & xmmB)
		{
			return _mm_movelh_ps(xmmA, xmmB);
		}
		template <int nShuffle>
		static __m128 sse_shuffle_ps(__m128 const & xmmA)
		{
			return _mm_shuffle_ps(xmmA, xmmA, nShuffle);
		}
		template <>
		static __m128 sse_shuffle_ps<_MM_SHUFFLE(3, 3, 1, 1)>(__m128 const & xmmA)
		{
			return _mm_movehdup_ps(xmmA);
		}
		template <>
		static __m128 sse_shuffle_ps<_MM_SHUFFLE(2, 2, 0, 0)>(__m128 const & xmmA)
		{
			return _mm_moveldup_ps(xmmA);
		}
		template <int nShuffle, typename TFeatureSet>
		static std::enable_if_t<(TFeatureSet::template exists_t<XMM_FEATURE_SSE>::value), __m128> xmm_shuffle_ps(__m128 const & xmmA, __m128 const & xmmB)
		{
			return sse_shuffle_ps<nShuffle>(xmmA, xmmB);
		}
		template <int nShuffle, typename TFeatureSet>
		static std::enable_if_t<(TFeatureSet::template exists_t<XMM_FEATURE_SSE>::value), __m128> xmm_shuffle_ps(__m128 const & xmmA)
		{
			return sse_shuffle_ps<nShuffle>(xmmA);
		}
		template <int nShuffle, typename TFeatureSet>
		static std::enable_if_t<!(TFeatureSet::template exists_t<XMM_FEATURE_SSE>::value), __m128> xmm_shuffle_ps(__m128 const & xmmA, __m128 const & xmmB)
		{
			__m128 xmmC;
			{
				xmmC.m128_i32[0] = xmmA.m128_i32[((nShuffle & 0x03) >> 0)];
				xmmC.m128_i32[1] = xmmA.m128_i32[((nShuffle & 0x0C) >> 2)];
				xmmC.m128_i32[2] = xmmB.m128_i32[((nShuffle & 0x30) >> 4)];
				xmmC.m128_i32[3] = xmmB.m128_i32[((nShuffle & 0xC0) >> 6)];
			}
			return xmmC;
		}
		template <int nShuffle, typename TFeatureSet>
		static std::enable_if_t<!(TFeatureSet::template exists_t<XMM_FEATURE_SSE>::value), __m128> xmm_shuffle_ps(__m128 const & xmmA)
		{
			__m128 xmmC;
			{
				xmmC.m128_i32[0] = xmmA.m128_i32[((nShuffle & 0x03) >> 0)];
				xmmC.m128_i32[1] = xmmA.m128_i32[((nShuffle & 0x0C) >> 2)];
				xmmC.m128_i32[2] = xmmA.m128_i32[((nShuffle & 0x30) >> 4)];
				xmmC.m128_i32[3] = xmmA.m128_i32[((nShuffle & 0xC0) >> 6)];
			}
			return xmmC;
		}
		template <int nShuffle>
		static __m128 shuffle_ps(__m128 const & xmmA, __m128 const & xmmB)
		{
			return xmm_shuffle_ps<nShuffle, xmm_feature_set<vemFeature...>>(xmmA, xmmB);
		}
		template <int nShuffle>
		static __m128 shuffle_ps(__m128 const & xmmA)
		{
			return xmm_shuffle_ps<nShuffle, xmm_feature_set<vemFeature...>>(xmmA);
		}

		template <int nShuffle>
		static __m128d sse_2_shuffle_pd(__m128d const & xmmA, __m128d const & xmmB)
		{
			return _mm_shuffle_pd(xmmA, xmmB, nShuffle);
		}
		template <int nShuffle>
		static __m128d sse_2_shuffle_pd(__m128d const & xmmA)
		{
			return _mm_shuffle_pd(xmmA, xmmA, nShuffle);
		}
		template <>
		static __m128d sse_2_shuffle_pd<_MM_SHUFFLE2(0, 0)>(__m128d const & xmmA)
		{
			return _mm_movedup_pd(xmmA);
		}
		template <int nShuffle, typename TFeatureSet>
		static std::enable_if_t<(TFeatureSet::template exists_t<XMM_FEATURE_SSE_2>::value), __m128d> xmm_shuffle_pd(__m128d const & xmmA, __m128d const & xmmB)
		{
			return sse_2_shuffle_pd<nShuffle>(xmmA, xmmB);
		}
		template <int nShuffle, typename TFeatureSet>
		static std::enable_if_t<(TFeatureSet::template exists_t<XMM_FEATURE_SSE_2>::value), __m128d> xmm_shuffle_pd(__m128d const & xmmA)
		{
			return sse_2_shuffle_pd<nShuffle>(xmmA);
		}
		template <int nShuffle, typename TFeatureSet>
		static std::enable_if_t<!(TFeatureSet::template exists_t<XMM_FEATURE_SSE_2>::value), __m128d> xmm_shuffle_pd(__m128d const & xmmA, __m128d const & xmmB)
		{
			__m128d xmmC;
			{
				xmmC.m128d_f64[0] = xmmA.m128d_f64[(nShuffle & 1) >> 0];
				xmmC.m128d_f64[1] = xmmB.m128d_f64[(nShuffle & 2) >> 1];
			}
			return xmmC;
		}
		template <int nShuffle, typename TFeatureSet>
		static std::enable_if_t<!(TFeatureSet::template exists_t<XMM_FEATURE_SSE_2>::value), __m128d> xmm_shuffle_pd(__m128d const & xmmA)
		{
			__m128d xmmC;
			{
				xmmC.m128d_f64[0] = xmmA.m128d_f64[(nShuffle & 1) >> 0];
				xmmC.m128d_f64[1] = xmmA.m128d_f64[(nShuffle & 2) >> 1];
			}
			return xmmC;
		}
		template <int nShuffle>
		static __m128d shuffle_pd(__m128d const & xmmA, __m128d const & xmmB)
		{
			return xmm_shuffle_pd<nShuffle, xmm_feature_set<vemFeature...>>(xmmA, xmmB);
		}
		template <int nShuffle>
		static __m128d shuffle_pd(__m128d const & xmmA)
		{
			return xmm_shuffle_pd<nShuffle, xmm_feature_set<vemFeature...>>(xmmA);
		}

		template <int nShuffle, typename TFeatureSet>
		static std::enable_if_t<(TFeatureSet::template exists_t<XMM_FEATURE_SSE_2>::value), __m128i> xmm_shuffle_epi32(__m128i const & xmmA, __m128i const & xmmB)
		{
			return _mm_castps_si128(sse_shuffle_ps<nShuffle>(_mm_castsi128_ps(xmmA), _mm_castsi128_ps(xmmB)));
		}
		template <int nShuffle, typename TFeatureSet>
		static std::enable_if_t<(TFeatureSet::template exists_t<XMM_FEATURE_SSE_2>::value), __m128i> xmm_shuffle_epi32(__m128i const & xmmA)
		{
			return _mm_shuffle_epi32(xmmA, nShuffle);
		}
		template <int nShuffle, typename TFeatureSet>
		static std::enable_if_t<!(TFeatureSet::template exists_t<XMM_FEATURE_SSE_2>::value), __m128i> xmm_shuffle_epi32(__m128i const & xmmA, __m128i const & xmmB)
		{
			__m128i xmmC;
			{
				xmmC.m128i_i32[0] = xmmA.m128i_i32[(nShuffle & 0x03) >> 0];
				xmmC.m128i_i32[1] = xmmA.m128i_i32[(nShuffle & 0x0C) >> 2];
				xmmC.m128i_i32[2] = xmmB.m128i_i32[(nShuffle & 0x30) >> 4];
				xmmC.m128i_i32[3] = xmmB.m128i_i32[(nShuffle & 0xC0) >> 6];
			}
			return xmmC;
		}
		template <int nShuffle, typename TFeatureSet>
		static std::enable_if_t<!(TFeatureSet::template exists_t<XMM_FEATURE_SSE_2>::value), __m128i> xmm_shuffle_epi32(__m128i const & xmmA)
		{
			__m128i xmmC;
			{
				xmmC.m128i_i32[0] = xmmA.m128i_i32[(nShuffle & 0x03) >> 0];
				xmmC.m128i_i32[1] = xmmA.m128i_i32[(nShuffle & 0x0C) >> 2];
				xmmC.m128i_i32[2] = xmmA.m128i_i32[(nShuffle & 0x30) >> 4];
				xmmC.m128i_i32[3] = xmmA.m128i_i32[(nShuffle & 0xC0) >> 6];
			}
			return xmmC;
		}
		template <int nShuffle>
		static __m128i shuffle_epi32(__m128i const & xmmA, __m128i const & xmmB)
		{
			return xmm_shuffle_epi32<nShuffle, xmm_feature_set<vemFeature...>>(xmmA, xmmB);
		}
		template <int nShuffle>
		static __m128i shuffle_epi32(__m128i const & xmmA)
		{
			return xmm_shuffle_epi32<nShuffle, xmm_feature_set<vemFeature...>>(xmmA);
		}

		template <int nIndex, typename TFeatureSet>
		static std::enable_if_t<(TFeatureSet::template exists_t<XMM_FEATURE_AVX_2>::value), std::enable_if_t<(TFeatureSet::template exists_t<XMM_FEATURE_SSE_2>::value), __m128i>> xmm_broadcastb_epi8(__m128i const & xmmA)
		{
			return _mm_broadcastb_epi8(_mm_srli_si128(xmmA, nIndex));
		}
		template <int nIndex, typename TFeatureSet>
		static std::enable_if_t<!(TFeatureSet::template exists_t<XMM_FEATURE_AVX_2>::value), std::enable_if_t<(TFeatureSet::template exists_t<XMM_FEATURE_SSE_2>::value), __m128i>> xmm_broadcastb_epi8(__m128i const & xmmA)
		{
			__m128i const xmmB = _mm_srli_si128(xmmA, nIndex);
			__m128i const xmmC = _mm_unpacklo_epi8(xmmB, xmmB);
			__m128i const xmmD = _mm_unpacklo_epi8(xmmC, xmmC);
			__m128i const xmmE = _mm_shuffle_epi32(xmmD, 0);
			return xmmE;
		}
		template <int nIndex, typename TFeatureSet>
		static std::enable_if_t<!(TFeatureSet::template exists_t<XMM_FEATURE_AVX_2>::value), std::enable_if_t<!(TFeatureSet::template exists_t<XMM_FEATURE_SSE_2>::value), __m128i>> xmm_broadcastb_epi8(__m128i const & xmmA)
		{
			__m128i xmmB;
			{
				std::fill(std::begin(xmmB.m128i_i8), std::end(xmmB.m128i_i8), xmmA.m128i_i8[nIndex & 0xF]);
			}
			return xmmB;
		}
		template <int nIndex>
		static __m128i broadcastb_epi8(__m128i const & xmmA)
		{
			return xmm_broadcastb_epi8<nIndex, xmm_feature_set<vemFeature...>>(xmmA);
		}

	}; // xmm_traits


#if 0

	template <int nIndex>
	static enable_if_feature_t<XMM_TRAITS_SSE_4_1, int> extract_epi32(__m128i const & xmmA)
	{
		return _mm_extract_epi32(xmmA, nIndex);
	}
	template <int nIndex>
	static disable_if_feature_t<XMM_TRAITS_SSE_4_1, enable_if_feature_t<XMM_TRAITS_SSE_2, int>> extract_epi32(__m128i const & xmmA)
	{
		return _mm_cvtsi128_si32(shuffle_epi32<_MM_SHUFFLE(nIndex, nIndex, nIndex, nIndex)>(xmmA));
	}
	template <int nIndex>
	static disable_if_feature_t<XMM_TRAITS_SSE_4_1, disable_if_feature_t<XMM_TRAITS_SSE_2, int>> extract_epi32(__m128i const & xmmA)
	{
		return xmmA.m128i_i32[nIndex & 3];
	}

#endif
}
