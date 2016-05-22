#pragma once

/* Author: jrmwng @ 2016 */

#include <Intrin.h>
#include <string>
#include <algorithm>
#include <type_traits>
#include "big_number.h"

namespace jrmwng
{
	template <unsigned uIndex, unsigned uThat>
	static decltype(auto) get_base(big_number<unsigned, uThat> const & bnThat)
	{
		return static_cast<std::conditional_t<(big_number<unsigned, uThat>::THIS_INDEX < uIndex), big_number<unsigned, uThat>, typename big_number<unsigned, uThat>::base_type> const &>(bnThat);
	}
	template <unsigned uIndex, unsigned uThat>
	static decltype(auto) get_base(big_number<unsigned, uThat> & bnThat)
	{
		return static_cast<std::conditional_t<(big_number<unsigned, uThat>::THIS_INDEX < uIndex), big_number<unsigned, uThat>, typename big_number<unsigned, uThat>::base_type> &>(bnThat);
	}
	template <unsigned uIndex, unsigned uThat>
	static std::enable_if_t<(uIndex <= big_number<unsigned, uThat>::THIS_INDEX), unsigned> get_bits(big_number<unsigned, uThat> const & bnThat)
	{
		return bnThat.uBits;
	}
	template <unsigned uIndex, unsigned uThat>
	static std::enable_if_t<(big_number<unsigned, uThat>::THIS_INDEX < uIndex), unsigned> get_bits(big_number<unsigned, uThat> const & bnThat)
	{
		return 0;
	}
	template <unsigned uIndex, unsigned uThat>
	static std::enable_if_t<(uIndex <= big_number<unsigned, uThat>::THIS_INDEX)> set_bits(big_number<unsigned, uThat> & bnThat, unsigned uNewBits)
	{
		bnThat.uBits = uNewBits;
	}
	template <unsigned uIndex, unsigned uThat>
	static std::enable_if_t<(big_number<unsigned, uThat>::THIS_INDEX < uIndex)> set_bits(big_number<unsigned, uThat> & bnThat, unsigned uNewBits)
	{
		if (uNewBits)
		{
#ifdef _DEBUG
			__debugbreak();
#endif
		}
	}

	template <unsigned uIndex, unsigned uThis, unsigned uThat>
	static std::enable_if_t<(0 < uIndex)> assign(big_number<unsigned, uThis> & bnThis, big_number<unsigned, uThat> const & bnThat)
	{
		assign<uIndex - 1>(get_base<uIndex>(bnThis), get_base<uIndex>(bnThat));
		set_bits<uIndex>(bnThis, get_bits<uIndex>(bnThat));
	}
	template <unsigned uIndex, unsigned uThis, unsigned uThat>
	static std::enable_if_t<(0 == uIndex)> assign(big_number<unsigned, uThis> & bnThis, big_number<unsigned, uThat> const & bnThat)
	{
		set_bits<uIndex>(bnThis, get_bits<uIndex>(bnThat));
	}

	template <unsigned uIndex, unsigned uThis, unsigned uLeft, unsigned uRight>
	static std::enable_if_t<(0 < uIndex), unsigned char> assign_add(big_number<unsigned, uThis> & bnThis, big_number<unsigned, uLeft> const & bnLeft, big_number<unsigned, uRight> const & bnRight)
	{
		unsigned char const ubBaseCarry = assign_add<uIndex - 1>(get_base<uIndex>(bnThis), get_base<uIndex>(bnLeft), get_base<uIndex>(bnRight));
		unsigned uThisBits;
		unsigned char const ubThisCarry = _addcarryx_u32(ubBaseCarry, get_bits<uIndex>(bnLeft), get_bits<uIndex>(bnRight), &uThisBits);

		set_bits<uIndex>(bnThis, uThisBits);
		return ubThisCarry;
	}
	template <unsigned uIndex, unsigned uThis, unsigned uLeft, unsigned uRight>
	static std::enable_if_t<(0 == uIndex), unsigned char> assign_add(big_number<unsigned, uThis> & bnThis, big_number<unsigned, uLeft> const & bnLeft, big_number<unsigned, uRight> const & bnRight)
	{
		unsigned uThisBits;
		unsigned char const ubThisCarry = _addcarryx_u32(0, get_bits<0>(bnLeft), get_bits<0>(bnRight), &uThisBits);

		set_bits<0>(bnThis, uThisBits);
		return ubThisCarry;
	}

	template <unsigned uIndex, unsigned uThis, unsigned uLeft, unsigned uRight>
	static std::enable_if_t<(0 < uIndex), unsigned char> assign_sub(big_number<unsigned, uThis> & bnThis, big_number<unsigned, uLeft> const & bnLeft, big_number<unsigned, uRight> const & bnRight)
	{
		unsigned char const ubBaseBorrow = assign_sub<uIndex - 1>(get_base<uIndex>(bnThis), get_base<uIndex>(bnLeft), get_base<uIndex>(bnRight));

		unsigned uThisBits;
		unsigned char const ubThisBorrow = _addcarryx_u32(ubBaseBorrow, ~get_bits<uIndex>(bnLeft), get_bits<uIndex>(bnRight), &uThisBits);

		set_bits<uIndex>(bnThis, ~uThisBits);
		return ubThisBorrow;
	}
	template <unsigned uIndex, unsigned uThis, unsigned uLeft, unsigned uRight>
	static std::enable_if_t<(0 == uIndex), unsigned char> assign_sub(big_number<unsigned, uThis> & bnThis, big_number<unsigned, uLeft> const & bnLeft, big_number<unsigned, uRight> const & bnRight)
	{
		unsigned uThisBits;
		unsigned char const ubThisBorrow = _addcarryx_u32(0, ~get_bits<0>(bnLeft), get_bits<0>(bnRight), &uThisBits);

		set_bits<0>(bnThis, ~uThisBits);
		return ubThisBorrow;
	}

	template <unsigned uBitCount>
	struct big_number<unsigned, uBitCount>
		: big_number<unsigned, (uBitCount - 1)&~31U>
	{
		using this_type = big_number<unsigned, uBitCount>;
		using base_type = big_number<unsigned, (uBitCount - 1)&~31U>;

		enum { THIS_BIT_COUNT = (uBitCount + 31U)&~31U };
		enum { BASE_BIT_COUNT = (uBitCount - 1U)&~31U };
		enum { THIS_INDEX = ((uBitCount - 1U) / 32U) };

		unsigned uBits;

		big_number()
			: uBits(0)
		{}

		__m128i to_w8char() const
		{
			// 0xFEDCBA98
			// 0x76543210
			__m128i const l4Bits0 = _mm_cvtsi32_si128(_byteswap_ulong(uBits));

			// 0x00FE00DC 0x00BA0098
			// 0x00760054 0x00320010
			__m128i const w4Bits1 = _mm_unpacklo_epi8(l4Bits0, _mm_setzero_si128());

			// 0xE000C000 0xA0008000
			// 0x60004000 0x20000000
			__m128i const w4Bits2a = _mm_slli_epi16(w4Bits1, 12);
			// 0x000F000D 0x000B0009
			// 0x00070005 0x00030001
			__m128i const w4Bits2b = _mm_srli_epi16(w4Bits1, 4);

			// 0x000E000C 0x000A0008
			// 0x00060004 0x00020000
			__m128i const w4Bits3a = _mm_srli_epi16(w4Bits2a, 12);

			// 0x000F000E 0x000D000C 0x000B000A 0x00090008
			// 0x00070006 0x00050004 0x00030002 0x00010000
			__m128i const w8Bits5 = _mm_unpacklo_epi16(w4Bits2b, w4Bits3a);

			// 0x00000000 0x00000000 0x00000000 0x00FF00FF
			// 0x00FF00FF 0x00FF00FF 0x00FF00FF 0x00FF00FF
			__m128i const w8Mask6 = _mm_cmplt_epi8(w8Bits5, _mm_set1_epi16(0xA));

			// 0x00000000 0x00000000 0x00000000 0x000F000F
			// 0x000F000F 0x000F000F 0x000F000F 0x000F000F
			__m128i const w8Mask7 = _mm_srli_epi16(w8Mask6, 4);

			// 0x00370037 0x00370037 0x00370037 0x00300030
			// 0x00300030 0x00300030 0x00300030 0x00300030
			__m128i const w8Char8 = _mm_andnot_si128(w8Mask7, _mm_set1_epi16(0x37));

			//    'F' 'E'    'D' 'C'    'B' 'A'    '9' '8'
			// 0x00460045 0x00440043 0x00420041 0x00390038
			//    '7' '6'    '5' '4'    '3' '2'    '1' '0'
			// 0x00370036 0x00350034 0x00330032 0x00310030
			__m128i const w8Char9 = _mm_add_epi16(w8Char8, w8Bits5);

			return w8Char9;
		}

		big_number(unsigned const uThat)
			: base_type(uThat)
			, uBits(THIS_INDEX == 0 ? uThat : 0U)
		{}

		big_number(unsigned long long const uxlThat)
			: base_type(uxlThat)
			, uBits(THIS_INDEX == 0 ? static_cast<unsigned>(uxlThat) : THIS_INDEX == 1 ? static_cast<unsigned>(uxlThat >> 32) : 0U)
		{}

		template <unsigned uThat>
		big_number(big_number<unsigned, uThat> const & bnThat)
		{
			assign<std::max<unsigned>(THIS_INDEX, big_number<unsigned, uThat>::THIS_INDEX)>(bnThat);
		}

		template <unsigned uLeft, unsigned uRight>
		big_number(big_number_add<big_number<unsigned, uLeft>const&, big_number<unsigned, uRight>const&> stAdd)
		{
			auto       & bnThis = *this;
			auto const & bnLeft = std::get<0>(stAdd);
			auto const & bnRight = std::get<1>(stAdd);
			if (assign_add<std::max<unsigned>(THIS_INDEX, std::max<unsigned>(big_number<unsigned, uLeft>::THIS_INDEX, big_number<unsigned, uRight>::THIS_INDEX))>(bnThis, bnLeft, bnRight))
			{
#ifdef _DEBUG
				__debugbreak();
#endif
			}
		}

		template <unsigned uLeft, unsigned uRight>
		big_number(big_number_sub<big_number<unsigned, uLeft> const &, big_number<unsigned, uRight> const &> stSub)
		{
			auto       & bnThis = *this;
			auto const & bnLeft = std::get<0>(stSub);
			auto const & bnRight = std::get<1>(stSub);
			if (assign_sub<std::max<unsigned>(THIS_INDEX, std::max<unsigned>(big_number<unsigned, uLeft>::THIS_INDEX, big_number<unsigned, uRight>::THIS_INDEX))>(bnThis, bnLeft, bnRight))
			{
#ifdef _DEBUG
				__debugbreak();
#endif
			}
		}

		//       ABC
		// *     DEF
		// ---------
		//       CHL (C*F) 0 0
		// +    CHL0 (B*F) 1 0
		// +    CHL0 (C*E) 0 1
		// +   CHL00 (A*F) 2 0
		// +   CHL00 (B*E) 1 1
		// +   CHL00 (C*D) 0 2
		// +  CHL000 (A*E) 2 1
		// +  CHL000 (B*D) 1 2
		// + CHL0000 (A*D) 2 2
		template <unsigned uSum, unsigned uLeft, unsigned uRight>
		std::enable_if_t<(uSum - 1 == big_number<unsigned, uLeft>::THIS_INDEX + big_number<unsigned, uRight>::THIS_INDEX)> assign_mul_inner_product_right(big_number<unsigned, uLeft> const & bnLeft, big_number<unsigned, uRight> const & bnRight, unsigned & uCarry, unsigned & uHigh, unsigned & uLow)
		{
			unsigned uMulHigh;
			unsigned const uMulLow = _mulx_u32(get_bits<big_number<unsigned, uLeft>::THIS_INDEX>(bnLeft), get_bits<big_number<unsigned, uRight>::THIS_INDEX>(bnRight), &uMulHigh);

			unsigned char const ubCarryLow = _addcarryx_u32(0, uLow, uMulLow, &uLow);
			unsigned char const ubCarryHigh = _addcarryx_u32(ubCarryLow, uHigh, uMulHigh, &uHigh);

			if (_addcarryx_u32(ubCarryHigh, uCarry, 0, &uCarry))
			{
#ifdef _DEBUG
				__debugbreak();
#endif
			}
		}
		template <unsigned uSum, unsigned uLeft, unsigned uRight>
		std::enable_if_t<(uSum - 1 < big_number<unsigned, uLeft>::THIS_INDEX + big_number<unsigned, uRight>::THIS_INDEX && uRight <= 32)> assign_mul_inner_product_right(big_number<unsigned, uLeft> const & bnLeft, big_number<unsigned, uRight> const & bnRight, unsigned & uCarry, unsigned & uHigh, unsigned & uLow)
		{
			// NOP
		}
		template <unsigned uSum, unsigned uLeft, unsigned uRight>
		std::enable_if_t<(uSum - 1 < big_number<unsigned, uLeft>::THIS_INDEX + big_number<unsigned, uRight>::THIS_INDEX && 32 < uRight)> assign_mul_inner_product_right(big_number<unsigned, uLeft> const & bnLeft, big_number<unsigned, uRight> const & bnRight, unsigned & uCarry, unsigned & uHigh, unsigned & uLow)
		{
			assign_mul_inner_product_right<uSum>(bnLeft, static_cast<typename big_number<unsigned, uRight>::base_type const&>(bnRight), uCarry, uHigh, uLow);
		}
		template <unsigned uSum, unsigned uLeft, unsigned uRight>
		std::enable_if_t<(uSum - 1 <= big_number<unsigned, uLeft>::THIS_INDEX + big_number<unsigned, uRight>::THIS_INDEX && uLeft <= 32)> assign_mul_inner_product_left(big_number<unsigned, uLeft> const & bnLeft, big_number<unsigned, uRight> const & bnRight, unsigned & uCarry, unsigned & uHigh, unsigned & uLow)
		{
			assign_mul_inner_product_right<uSum>(bnLeft, bnRight, uCarry, uHigh, uLow);
		}
		template <unsigned uSum, unsigned uLeft, unsigned uRight>
		std::enable_if_t<(uSum - 1 == big_number<unsigned, uLeft>::THIS_INDEX + big_number<unsigned, uRight>::THIS_INDEX && 32 < uLeft)> assign_mul_inner_product_left(big_number<unsigned, uLeft> const & bnLeft, big_number<unsigned, uRight> const & bnRight, unsigned & uCarry, unsigned & uHigh, unsigned & uLow)
		{
			assign_mul_inner_product_right<uSum>(bnLeft, bnRight, uCarry, uHigh, uLow);
		}
		template <unsigned uSum, unsigned uLeft, unsigned uRight>
		std::enable_if_t<(uSum - 1 < big_number<unsigned, uLeft>::THIS_INDEX + big_number<unsigned, uRight>::THIS_INDEX && 32 < uLeft)> assign_mul_inner_product_left(big_number<unsigned, uLeft> const & bnLeft, big_number<unsigned, uRight> const & bnRight, unsigned & uCarry, unsigned & uHigh, unsigned & uLow)
		{
			assign_mul_inner_product_right<uSum>(bnLeft, bnRight, uCarry, uHigh, uLow);
			assign_mul_inner_product_left<uSum>(static_cast<typename big_number<unsigned, uLeft>::base_type const&>(bnLeft), bnRight, uCarry, uHigh, uLow);
		}
		template <unsigned uSum, unsigned uLeft, unsigned uRight>
		std::enable_if_t<(big_number<unsigned, uLeft>::THIS_INDEX + big_number<unsigned, uRight>::THIS_INDEX < uSum - 1)> assign_mul_inner_product_left(big_number<unsigned, uLeft> const & bnLeft, big_number<unsigned, uRight> const & bnRight, unsigned & uCarry, unsigned & uHigh, unsigned & uLow)
		{
			// NOP
		}
		template <unsigned u, unsigned uLeft, unsigned uRight>
		std::enable_if_t<(0 == u && THIS_INDEX == u), unsigned> assign_mul(big_number<unsigned, uLeft> const & bnLeft, big_number<unsigned, uRight> const & bnRight, unsigned & uCarry, unsigned uHigh)
		{
			uBits = uHigh;
			return 0;
		}
		template <unsigned u, unsigned uLeft, unsigned uRight>
		std::enable_if_t<(0 < u && THIS_INDEX == u), unsigned> assign_mul(big_number<unsigned, uLeft> const & bnLeft, big_number<unsigned, uRight> const & bnRight, unsigned & uCarry, unsigned uHigh)
		{
			unsigned uLocal = 0;
			unsigned uLow = 0;
			assign_mul_inner_product_left<u>(bnLeft, bnRight, uLocal, uHigh, uLow);

			unsigned char const ubCarry = base_type::template assign_mul<u - 1>(bnLeft, bnRight, uHigh, uLow);

			uBits = uHigh;
			return _addcarryx_u32(ubCarry, uLocal, uCarry, &uCarry);
		}
		template <unsigned u, unsigned uLeft, unsigned uRight>
		std::enable_if_t<(0 < u && THIS_INDEX < u), unsigned> assign_mul(big_number<unsigned, uLeft> const & bnLeft, big_number<unsigned, uRight> const & bnRight, unsigned & uCarry, unsigned uHigh)
		{
			unsigned uLocal = 0;
			unsigned uLow = 0;
			assign_mul_inner_product_left<u>(bnLeft, bnRight, uLocal, uHigh, uLow);

			unsigned char const ubCarry = assign_mul<u - 1>(bnLeft, bnRight, uHigh, uLow);

			return _addcarryx_u32(ubCarry, uLocal, uCarry, &uCarry);
		}
		template <unsigned uLeft, unsigned uRight>
		big_number(big_number_mul<big_number<unsigned, uLeft> const &, big_number<unsigned, uRight> const &> stMul)
		{
			auto const & bnLeft = std::get<0>(stMul);
			auto const & bnRight = std::get<1>(stMul);
			unsigned uBaseCarry = 0;
			if (assign_mul<std::max<unsigned>(THIS_INDEX, big_number<unsigned, uLeft>::THIS_INDEX + big_number<unsigned, uRight>::THIS_INDEX + 1)>(bnLeft, bnRight, uBaseCarry, 0))
			{
#ifdef _DEBUG
				__debugbreak();
#endif
			}
		}
		template <unsigned uLeftLeft, unsigned uLeftRight, unsigned uRightLeft, unsigned uRightRight>
		big_number(big_number_mul< big_number_sub<big_number<unsigned, uLeftLeft> const &, big_number<unsigned, uLeftRight> const &>, big_number_sub< big_number<unsigned, uRightLeft> const &, big_number<unsigned, uRightRight> const &> > stExpr)
		{
			big_number<unsigned, std::max<unsigned>(uLeftLeft, uLeftRight)> const bnLeft(std::get<0>(stExpr));
			big_number<unsigned, std::max<unsigned>(uRightLeft, uRightRight)> const bnRight(std::get<1>(stExpr));
			unsigned uBaseCarry = 0;
			if (assign_mul<std::max<unsigned>(THIS_INDEX, bnLeft.THIS_INDEX + bnRight.THIS_INDEX + 1)>(bnLeft, bnRight, uBaseCarry, 0))
			{
#ifdef _DEBUG
				__debugbreak();
#endif
			}
		}

		//   ABCDEF/GHI
		// = a00000 + (A00000 - a00000*GHI + 0BCDEF)/GHI
		// = ab0000 + (AB0000 - ab0000*GHI + 00CDEF)/GHI
		// = abc000 + (ABC000 - abc000*GHI + 000DEF)/GHI
		// = abcd00 + (ABCD00 - abcd00*GHI + 0000EF)/GHI
		// = abcde0 + (ABCDE0 - abcde0*GHI + 00000F)/GHI
		// = abcdef + (ABCDEF - abcdef*GHI + 000000)/GHI

		int rand()
		{
			int n = base_type::rand();
			while (_rdrand32_step(&uBits) == 0 && ++n)
			{
				_mm_pause();
			}
			return n;
		}
	};
	template <>
	struct big_number<unsigned, 0>
		: big_number_expr
	{
		enum { THIS_BIT_COUNT = 0 };
		enum { BASE_BIT_COUNT = 0 };

		big_number()
		{}
		template <typename T1>
		big_number(T1)
		{}

		int rand()
		{
			return 0;
		}
	};

	template <unsigned uBitCount>
	using big_uint32 = big_number<unsigned, uBitCount>;

	template <unsigned uThat>
	inline void ToString(big_uint32<uThat> const & bnThat, wchar_t *pcBuffer)
	{
		_mm_storeu_si128((__m128i*)pcBuffer, bnThat.to_w8char());

		ToString(static_cast<typename big_uint32<uThat>::base_type const &>(bnThat), pcBuffer + 8);
	}
	template <>
	inline void ToString<0>(big_uint32<0> const &, wchar_t *)
	{
		// NOP
	}
	template <unsigned uThat>
	static std::wstring ToString(big_uint32<uThat> const & bnThat)
	{
		std::wstring str;
		{
			str.resize(2 + (big_uint32<uThat>::THIS_INDEX + 1) * 8);
			str[0] = L'0';
			str[1] = L'x';
			ToString(bnThat, &str[2]);
			str[2 + (big_uint32<uThat>::THIS_INDEX + 1) * 8] = '\0';
		}
		return str;
	}


	template <unsigned uThat>
	inline __m128i CmpEq(big_uint32<uThat> const & bnThat, wchar_t const *pcThat)
	{
		__m128i const w8CmpEq0 = CmpEq(static_cast<typename big_uint32<uThat>::base_type const &>(bnThat), pcThat + 8);
		__m128i const w8That0 = _mm_loadu_si128((__m128i const*)pcThat);
		__m128i const w8This0 = bnThat.to_w8char();

		__m128i const w8CmpEq1 = _mm_cmpeq_epi16(w8This0, w8That0);

		__m128i const w8CmpEq2 = _mm_and_si128(w8CmpEq0, w8CmpEq1);

		return w8CmpEq2;
	}
	template <>
	inline __m128i CmpEq<0>(big_uint32<0> const &, wchar_t const *)
	{
		return _mm_cmpeq_epi16(_mm_setzero_si128(), _mm_setzero_si128());
	}
	template <unsigned uThat>
	static bool operator == (big_uint32<uThat> const & bnThat, wchar_t const *pcThat)
	{
		return pcThat[0] == L'0' && pcThat[1] == L'x' && 0xFFFF == _mm_movemask_epi8(CmpEq(bnThat, pcThat + 2));
	}


	template <unsigned uThat>
	inline bool CmpEq(big_uint32<uThat> const & bnLeft, big_uint32<uThat> const & bnRight)
	{
		auto const & bnLeftBase = static_cast<typename big_uint32<uThat>::base_type const &>(bnLeft);
		auto const & bnRightBase = static_cast<typename big_uint32<uThat>::base_type const &>(bnRight);
		return get_bits<big_uint32<uThat>::THIS_INDEX>(bnLeft) == get_bits<big_uint32<uThat>::THIS_INDEX>(bnRight) && CmpEq(bnLeftBase, bnRightBase);
	}
	template <>
	inline bool CmpEq<0>(big_uint32<0> const &, big_uint32<0> const &)
	{
		return true;
	}
	template <unsigned uThat>
	static bool operator == (big_uint32<uThat> const & bnLeft, big_uint32<uThat> const & bnRight)
	{
		return CmpEq(bnLeft, bnRight);
	}
	template <unsigned uLeft, typename TRight>
	static bool operator == (big_uint32<uLeft> const & bnLeft, TRight const & tRight)
	{
		return CmpEq(bnLeft, big_uint32<uLeft>(tRight));
	}
}
