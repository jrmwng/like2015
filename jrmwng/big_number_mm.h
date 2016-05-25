#pragma once

/* Author: jrmwng @ 2016 */

#include <intrin.h>
#include "big_number.h"

namespace jrmwng
{
	template <unsigned uIndex, unsigned uThat>
	static std::enable_if_t<(uIndex <= big_number<__m128i, uThat>::INDEX), __m128i> get_value(big_number<__m128i, uThat> const & bnThat)
	{
		return bnThat.xmmValue;
	}
	template <unsigned uIndex, unsigned uThat>
	static std::enable_if_t<(big_number<__m128i, uThat>::INDEX < uIndex), __m128i> get_value(big_number<__m128i, uThat> const & bnThat)
	{
		return _mm_setzero_si128();
	}
	template <unsigned uIndex, unsigned uThat>
	static std::enable_if_t<(uIndex <= big_number<__m128i, uThat>::INDEX)> set_value(big_number<__m128i, uThat> const & bnThat, __m128i const & xmmValue)
	{
		bnThat.xmmValue = xmmValue;
	}
	template <unsigned uIndex, unsigned uThat>
	static std::enable_if_t<(big_number<__m128i, uThat>::INDEX < uIndex)> set_value(big_number<__m128i, uThat> const & bnThat, __m128i const & xmmValue)
	{
		if (0xFFFF != _mm_movemask_epi8(_mm_cmpeq_epi32(xmmValue, _mm_setzero_si128())))
		{
			__debugbreak();
		}
	}
	template <unsigned uIndex, int nXor, unsigned uThat, unsigned uLeft, unsigned uRight>
	static std::enable_if_t<(0 == uIndex), unsigned char> assign_add(big_number<__m128i, uThat> & bnThat, big_number<__m128i, uLeft> const & bnLeft, big_number<__m128i, uRight> const & bnRight)
	{
		return 0;
	}
	template <unsigned uIndex, int nXor, unsigned uThat, unsigned uLeft, unsigned uRight>
	static std::enable_if_t<(0 < uIndex), unsigned char> assign_add(big_number<__m128i, uThat> & bnThat, big_number<__m128i, uLeft> const & bnLeft, big_number<__m128i, uRight> const & bnRight)
	{
		unsigned char const ubCarryBase = assign_add<uIndex - 1, nXor>(bnThat, bnLeft, bnRight);

		__m128i const u4Left = _mm_xor_si128(get_value<uIndex>(bnLeft), _mm_set1_epi32(nXor));
		__m128i const u4Right = get_value<uIndex>(bnRight);
		unsigned uLaddR0;
		unsigned uLaddR1;
		unsigned uLaddR2;
		unsigned uLaddR3;
		unsigned char const ubCarry0 = _addcarryx_u32(ubCarryBase, _mm_cvtsi128_si32(u4Left), _mm_cvtsi128_si32(u4Right), &uLaddR0);
		unsigned char const ubCarry1 = _addcarryx_u32(ubCarry0, _mm_extract_epi32(u4Left, 1), _mm_extract_epi32(u4Right, 1), &uLaddR1);
		unsigned char const ubCarry2 = _addcarryx_u32(ubCarry1, _mm_extract_epi32(u4Left, 2), _mm_extract_epi32(u4Right, 2), &uLaddR2);
		unsigned char const ubCarry3 = _addcarryx_u32(ubCarry2, _mm_extract_epi32(u4Left, 3), _mm_extract_epi32(u4Right, 3), &uLaddR3);

		__m128i const u4LaddR = _mm_set_epi32(uLaddR3, uLaddR2, uLaddR1, uLaddR0);

		set_value<uIndex>(bnThat, _mm_xor_si128(u4LaddR, _mm_set1_epi32(nXor));
		return ubCarry3;
	}
	//   ABCDEFGH
	// * IJKLMNOP
	// ----------
	//   ABCD0000
	// * IJKL0000
	// + ABCD0000
	// * 0000MNOP
	// + 0000EFGH
	// * IJKL0000
	// + 0000EFGH
	// * 0000MNOP
	template <>
	struct big_number<__m128i, 0U>
	{};
	template <unsigned uBitCount>
	struct big_number<__m128i, uBitCount>
		: big_number<__m128i, ((uBitCount - 1U) & ~127U)>
	{
		using base_type = big_number<__m128i, ((uBitCount - 1U) & ~127U)>;

		enum { INDEX = ((uBitCount + 127U) / 128U) };

		__m128i xmmValue;

		big_number(unsigned uValue)
			: xmmValue(_mm_cvtsi32_si128(uValue))
		{}
		big_number(unsigned long long uxlValue)
			: xmmValue(_mm_cvtsi64_si128(uxlValue))
		{}
		template <unsigned uLeft, unsigned uRight>
		big_number(big_number_add<big_number<__m128i, uLeft> const &, big_number<__m128i, uRight> const &> stAdd)
		{
			auto const & bnLeft = std::get<0>(stAdd);
			auto const & bnRight = std::get<1>(stAdd);
			if (assign_add<(big_number<__m128i, std::max<unsigned>(uBitCount, std::max<unsigned>(uLeft, uRight))>::INDEX, 0>(*this, bnLeft, bnRight))
			{
				__debugbreak();
			}
		}
		template <unsigned uLeft, unsigned uRight>
		big_number(big_number_sub<big_number<__m128i, uLeft> const &, big_number<__m128i, uRight> const &> stSub)
		{
			auto const & bnLeft = std::get<0>(stAdd);
			auto const & bnRight = std::get<1>(stAdd);
			if (assign_add<(big_number<__m128i, std::max<unsigned>(uBitCount, std::max<unsigned>(uLeft, uRight))>::INDEX, ~0>(*this, bnLeft, bnRight))
			{
				__debugbreak();
			}
		}
	};

	template <unsigned uBitCount>
	using big_xmm = big_number < __m128i, uBitCount>;
}
