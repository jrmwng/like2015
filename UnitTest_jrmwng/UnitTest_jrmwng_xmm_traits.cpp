#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\jrmwng\xmm_traits.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_jrmwng
{		
	TEST_CLASS(UnitTest_jrmwng_xmm_traits)
	{
		template <typename TT>
		struct jrmwng_xmm_traits
		{
			bool bPass;

			jrmwng_xmm_traits()
				: bPass(true)
			{
				jrmwng::for_each(std::make_integer_sequence<int, 0x100>(), [this](auto const tIndex)
				{
					int constexpr nShuffle = tIndex.value;
					__m128 const r4A = _mm_set_ps(3.0F, 2.0F, 1.0F, 0.0F);
					__m128 const r4B = _mm_set_ps(7.0F, 6.0F, 5.0F, 4.0F);
					__m128 const r4ExpectAB = _mm_shuffle_ps(r4A, r4B, nShuffle);
					__m128 const r4ExpectAA = _mm_shuffle_ps(r4A, r4A, nShuffle);
					__m128 const r4ActualAB = TT::shuffle_ps<nShuffle>(r4A, r4B);
					__m128 const r4ActualAA = TT::shuffle_ps<nShuffle>(r4A);
					__m128 const r4EqualAB = _mm_cmpeq_ps(r4ExpectAB, r4ActualAB);
					__m128 const r4EqualAA = _mm_cmpeq_ps(r4ExpectAA, r4ActualAA);
					int const nEqualAB = _mm_movemask_ps(r4EqualAB);
					int const nEqualAA = _mm_movemask_ps(r4EqualAA);
					bPass &= (0xF == nEqualAB);
					bPass &= (0xF == nEqualAA);
					//Assert::AreEqual(0xF, nEqualAB);
					//Assert::AreEqual(0xF, nEqualAA);
				});

				jrmwng::for_each(std::make_integer_sequence<int, 4>(), [this](auto const tIndex)
				{
					int constexpr nShuffle = tIndex.value;
					__m128d const lr2A = _mm_set_pd(3.0, 2.0);
					__m128d const lr2B = _mm_set_pd(1.0, 0.0);
					__m128d const lr2ExpectAB = _mm_shuffle_pd(lr2A, lr2B, nShuffle);
					__m128d const lr2ExpectAA = _mm_shuffle_pd(lr2A, lr2A, nShuffle);
					__m128d const lr2ActualAB = TT::shuffle_pd<nShuffle>(lr2A, lr2B);
					__m128d const lr2ActualAA = TT::shuffle_pd<nShuffle>(lr2A, lr2A);
					__m128d const lr2EqualAB = _mm_cmpeq_pd(lr2ExpectAB, lr2ActualAB);
					__m128d const lr2EqualAA = _mm_cmpeq_pd(lr2ExpectAA, lr2ActualAA);
					int const nEqualAB = _mm_movemask_pd(lr2EqualAB);
					int const nEqualAA = _mm_movemask_pd(lr2EqualAA);
					bPass &= (0x3 == nEqualAB);
					bPass &= (0x3 == nEqualAB);
					//Assert::AreEqual(0x3, nEqualAB);
					//Assert::AreEqual(0x3, nEqualAA);
				});

				jrmwng::for_each(std::make_integer_sequence<int, 0x100>(), [this](auto const tIndex)
				{
					int constexpr nShuffle = tIndex.value;
					__m128i const l4A = _mm_set_epi32(3, 2, 1, 0);
					__m128i const l4B = _mm_set_epi32(7, 6, 5, 4);
					__m128i const l4ExpectAB = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(l4A), _mm_castsi128_ps(l4B), nShuffle));
					__m128i const l4ExpectAA = _mm_shuffle_epi32(l4A, nShuffle);
					__m128i const l4ActualAB = TT::shuffle_epi32<nShuffle>(l4A, l4B);
					__m128i const l4ActualAA = TT::shuffle_epi32<nShuffle>(l4A);
					__m128i const l4EqualAB = _mm_cmpeq_epi32(l4ExpectAB, l4ActualAB);
					__m128i const l4EqualAA = _mm_cmpeq_epi32(l4ExpectAA, l4ActualAA);
					int const nEqualAB = _mm_movemask_epi8(l4EqualAB);
					int const nEqualAA = _mm_movemask_epi8(l4EqualAA);
					bPass &= (0xFFFF == nEqualAB);
					bPass &= (0xFFFF == nEqualAA);
					//Assert::AreEqual(0xFFFF, nEqualAB);
					//Assert::AreEqual(0xFFFF, nEqualAA);
				});

				jrmwng::for_each(std::make_integer_sequence<int, 0x10>(), [this](auto const tIndex)
				{
					int constexpr nIndex = tIndex.value;
					__m128i const b16A = _mm_set_epi8(0xF, 0xE, 0xD, 0xC, 0xB, 0xA, 0x9, 0x8, 0x7, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1, 0x0);
					__m128i const b16ExpectA = _mm_broadcastb_epi8(_mm_srli_si128(b16A, nIndex));
					__m128i const b16ActualA = TT::broadcastb_epi8<nIndex>(b16A);
					__m128i const b16EqualA = _mm_cmpeq_epi8(b16ExpectA, b16ActualA);
					int const nEqualA = _mm_movemask_epi8(b16EqualA);
					bPass &= (0xFFFF == nEqualA);
					//Assert::AreEqual(0xFFFF, nEqualA);
				});
			}
		};
	public:
		TEST_METHOD(TestMethod_jrmwng_xmm_traits)
		{
			typedef jrmwng::xmm_traits<> IA32;
			typedef jrmwng::xmm_traits<jrmwng::XMM_FEATURE_SSE> SSE;
			typedef jrmwng::xmm_traits<jrmwng::XMM_FEATURE_SSE, jrmwng::XMM_FEATURE_SSE_2> SSE_2;
			typedef jrmwng::xmm_traits<jrmwng::XMM_FEATURE_SSE, jrmwng::XMM_FEATURE_SSE_2, jrmwng::XMM_FEATURE_SSE_3> SSE_3;
			typedef jrmwng::xmm_traits<jrmwng::XMM_FEATURE_SSE, jrmwng::XMM_FEATURE_SSE_2, jrmwng::XMM_FEATURE_SSE_3, jrmwng::XMM_FEATURE_SSSE_3> SSSE_3;
			typedef jrmwng::xmm_traits<jrmwng::XMM_FEATURE_SSE, jrmwng::XMM_FEATURE_SSE_2, jrmwng::XMM_FEATURE_SSE_3, jrmwng::XMM_FEATURE_SSSE_3, jrmwng::XMM_FEATURE_SSE_4_1> SSE_4_1;
			typedef jrmwng::xmm_traits<jrmwng::XMM_FEATURE_SSE, jrmwng::XMM_FEATURE_SSE_2, jrmwng::XMM_FEATURE_SSE_3, jrmwng::XMM_FEATURE_SSSE_3, jrmwng::XMM_FEATURE_SSE_4_1, jrmwng::XMM_FEATURE_SSE_4_2> SSE_4_2;
			typedef jrmwng::xmm_traits<jrmwng::XMM_FEATURE_SSE, jrmwng::XMM_FEATURE_SSE_2, jrmwng::XMM_FEATURE_SSE_3, jrmwng::XMM_FEATURE_SSSE_3, jrmwng::XMM_FEATURE_SSE_4_1, jrmwng::XMM_FEATURE_SSE_4_2, jrmwng::XMM_FEATURE_AVX> AVX;
			typedef jrmwng::xmm_traits<jrmwng::XMM_FEATURE_SSE, jrmwng::XMM_FEATURE_SSE_2, jrmwng::XMM_FEATURE_SSE_3, jrmwng::XMM_FEATURE_SSSE_3, jrmwng::XMM_FEATURE_SSE_4_1, jrmwng::XMM_FEATURE_SSE_4_2, jrmwng::XMM_FEATURE_AVX, jrmwng::XMM_FEATURE_AVX_2> AVX_2;

			jrmwng_xmm_traits<IA32> stIA32; Assert::IsTrue(stIA32.bPass);
			jrmwng_xmm_traits<SSE> stSSE; Assert::IsTrue(stSSE.bPass);
			jrmwng_xmm_traits<SSE_2> stSSE_2; Assert::IsTrue(stSSE_2.bPass);
			jrmwng_xmm_traits<SSE_3> stSSE_3; Assert::IsTrue(stSSE_3.bPass);
			jrmwng_xmm_traits<SSSE_3> stSSSE_3; Assert::IsTrue(stSSSE_3.bPass);
			jrmwng_xmm_traits<SSE_4_1> stSSE_4_1; Assert::IsTrue(stSSE_4_1.bPass);
			jrmwng_xmm_traits<SSE_4_2> stSSE_4_2; Assert::IsTrue(stSSE_4_2.bPass);
			jrmwng_xmm_traits<AVX> stAVX; Assert::IsTrue(stAVX.bPass);
			jrmwng_xmm_traits<AVX_2> stAVX_2; Assert::IsTrue(stAVX_2.bPass);

			// TODO: Your test code here
		}
	};
}