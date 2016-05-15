#pragma once

/* Author: jrmwng @ 2016 */

#include <intrin.h>
#include <string>

namespace jrmwng
{
	template <typename T, unsigned uBitCount>
	struct big_number;
	template <typename T>
	struct big_number<T, 0>
	{};


	template <typename TLeft, typename TRight>
	struct big_number_add
		: std::tuple<TLeft, TRight>
	{
		big_number_add(TLeft tLeft, TRight tRight)
			: std::tuple<TLeft, TRight>(tLeft, tRight)
		{}
	};
	template <typename TLeft, unsigned uLeft, typename TRight, unsigned uRight>
	decltype(auto) operator + (big_number<TLeft, uLeft> const & Left, big_number<TRight, uRight> const & Right)
	{
		return big_number_add<decltype(Left), decltype(Right)>(Left, Right);
	}

	template <typename TLeft, typename TRight>
	struct big_number_sub
		: std::tuple<TLeft, TRight>
	{
		big_number_sub(TLeft tLeft, TRight tRight)
			: std::tuple<TLeft, TRight>(tLeft, tRight)
		{}
	};
	template <typename TLeft, unsigned uLeft, typename TRight, unsigned uRight>
	decltype(auto) operator - (big_number<TLeft, uLeft> const & Left, big_number<TRight, uRight> const & Right)
	{
		return big_number_sub<decltype(Left), decltype(Right)>(Left, Right);
	}
	template <typename TLeft, unsigned uLeft>
	decltype(auto) operator - (big_number<TLeft, uLeft> const & bnLeft, int nRight)
	{
		return big_number_sub<decltype(bnLeft), decltype(nRight)>(bnLeft, nRight);
	}

	template <typename TLeft, typename TRight>
	struct big_number_mul
		: std::tuple<TLeft, TRight>
	{
		big_number_mul(TLeft tLeft, TRight tRight)
			: std::tuple<TLeft, TRight>(tLeft, tRight)
		{}
	};
	template <typename TLeft, unsigned uLeft, typename TRight, unsigned uRight>
	decltype(auto) operator * (big_number<TLeft, uLeft> const & Left, big_number<TRight, uRight> const & Right)
	{
		return big_number_mul<decltype(Left), decltype(Right)>(Left, Right);
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

		template <unsigned u>
		void to_string(wchar_t *pcBuffer) const
		{
			to_string<0>(pcBuffer);

			base_type::to_string<u - 1>(pcBuffer + 8);
		}
		template <>
		void to_string<0>(wchar_t *pcBuffer) const
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

			_mm_storeu_si128((__m128i*)pcBuffer, w8Char9);
		}
		std::wstring ToString() const
		{
			std::wstring str;
			{
				str.resize(2 + (THIS_INDEX + 1) * 8);
				str[0] = L'0';
				str[1] = L'x';
				to_string<THIS_INDEX>(&str[2]);
				//str[str.length() - 1] = L'\0';
			}
			return str;
		}
		bool operator == (wchar_t const *pcThat) const
		{
			wchar_t acBuffer[2 + (THIS_INDEX + 1) * 8];
			{
				acBuffer[0] = L'0';
				acBuffer[1] = L'x';
				to_string<THIS_INDEX>(acBufer + 2);
				acBuffer[2 + (THIS_INDEX + 1)] = L'\0';
			}
			return strcmp(acBuffer, pcThat) == 0;
		}

		template <unsigned u>
		bool cmpeq(big_number const & that) const
		{
			return uBits == that.uBits && base_type::cmpeq<u - 1>(static_cast<big_number const&>(that));
		}
		template <>
		bool cmpeq<0>(big_number const & that) const
		{
			return uBits == that.uBits;
		}
		bool operator == (big_number const & that) const
		{
			return cmpeq<THIS_INDEX>(that);
		}

		template <unsigned u>
		bool cmpeq(unsigned const uThat) const
		{
			return uBits == 0 && base_type::cmpeq<u - 1>(uThat);
		}
		template <>
		bool cmpeq<0>(unsigned const uThat) const
		{
			return uBits == uThat;
		}
		bool operator == (unsigned const uThat) const
		{
			return cmpeq<THIS_INDEX>(uThat);
		}

		template <unsigned u>
		bool cmpeq(unsigned long long const uxlThat) const
		{
			return uBits == 0 && base_type::cmpeq<u - 1>(uxlThat);
		}
		template <>
		bool cmpeq<1>(unsigned long long const uxlThat) const
		{
			return uBits == static_cast<unsigned>(uxlThat >> 32) && base_type::cmpeq<0>(uxlThat);
		}
		template <>
		bool cmpeq<0>(unsigned long long const uxlThat) const
		{
			return uBits == static_cast<unsigned>(uxlThat);
		}
		bool operator == (unsigned long long const uxlThat) const
		{
			return cmpeq<THIS_INDEX>(uxlThat);
		}

		template <unsigned uIndex, unsigned uThat>
		static std::enable_if_t<(uIndex <= big_number<unsigned, uThat>::THIS_INDEX), typename big_number<unsigned, uThat>::base_type const &> get_base(big_number<unsigned, uThat> const & bnThat)
		{
			return static_cast<big_number<unsigned, uThat>::base_type const &>(bnThat);
		}
		template <unsigned uIndex, unsigned uThat>
		static std::enable_if_t<(big_number<unsigned, uThat>::THIS_INDEX < uIndex), big_number<unsigned, uThat> const &> get_base(big_number<unsigned, uThat> const & bnThat)
		{
			return bnThat;
		}
		template <unsigned uIndex, unsigned uThat>
		static std::enable_if_t<(uIndex <= big_number<unsigned, uThat>::THIS_INDEX), typename big_number<unsigned, uThat>::base_type &> get_base(big_number<unsigned, uThat> & bnThat)
		{
			return static_cast<big_number<unsigned, uThat>::base_type &>(bnThat);
		}
		template <unsigned uIndex, unsigned uThat>
		static std::enable_if_t<(big_number<unsigned, uThat>::THIS_INDEX < uIndex), big_number<unsigned, uThat> &> get_base(big_number<unsigned, uThat> & bnThat)
		{
			return bnThat;
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
				__debugbreak();
			}
		}

		template <unsigned u>
		void assign(unsigned const uThat)
		{
			base_type::assign<u - 1>(uThat);
			uBits = 0;
		}
		template <>
		void assign<0>(unsigned const uThat)
		{
			uBits = uThat;
		}
		big_number(unsigned const uThat)
		{
			assign<THIS_INDEX>(uThat);
		}

		template <unsigned u>
		void assign(unsigned long long const uxlThat)
		{
			base_type::assign<u - 1>(uxlThat);
			uBits = 0;
		}
		template <>
		void assign<1>(unsigned long long const uxlThat)
		{
			base_type::assign<0>(uxlThat);
			uBits = static_cast<unsigned>(uxlThat >> 32);
		}
		template <>
		void assign<0>(unsigned long long const uxlThat)
		{
			uBits = static_cast<unsigned>(uxlThat);
		}
		big_number(unsigned long long const uxlThat)
		{
			assign<THIS_INDEX>(uxlThat);
			return *this;
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
		template <unsigned uThat>
		big_number(big_number<unsigned, uThat> const & bnThat)
		{
			assign<std::max<unsigned>(THIS_INDEX, big_number<unsigned, uThat>::THIS_INDEX)>(bnThat);
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
			return _addcarryx_u32(0, bnLeft.uBits, bnRight.uBits, &bnThis.uBits);
		}
		template <unsigned uLeft, unsigned uRight>
		big_number(big_number_add<big_number<unsigned, uLeft>const&, big_number<unsigned, uRight>const&> stAdd)
		{
			auto       & bnThis = *this;
			auto const & bnLeft = std::get<0>(stAdd);
			auto const & bnRight = std::get<1>(stAdd);
			if (assign_add<std::max<unsigned>(THIS_INDEX, std::max<unsigned>(big_number<unsigned, uLeft>::THIS_INDEX, big_number<unsigned, uRight>::THIS_INDEX))>(bnThis, bnLeft, bnRight))
			{
				__debugbreak();
			}
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
			unsigned char const ubThisBorrow = _addcarryx_u32(0, ~bnLeft.uBits, bnRight.uBits, &uThisBits);

			bnThis.uBits = ~uThisBits;
			return ubThisBorrow;
		}
		template <unsigned uLeft, unsigned uRight>
		big_number(big_number_sub<big_number<unsigned, uLeft> const &, big_number<unsigned, uRight> const &> stSub)
		{
			auto       & bnThis = *this;
			auto const & bnLeft = std::get<0>(stSub);
			auto const & bnRight = std::get<1>(stSub);
			if (assign_sub<std::max<unsigned>(THIS_INDEX, std::max<unsigned>(big_number<unsigned, uLeft>::THIS_INDEX, big_number<unsigned, uRight>::THIS_INDEX))>(bnThis, bnLeft, bnRight))
			{
				__debugbreak();
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
			unsigned const uMulLow = _mulx_u32(bnLeft.uBits, bnRight.uBits, &uMulHigh);

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
			assign_mul_inner_product_right<uSum>(bnLeft, static_cast<big_number<unsigned, uRight>::base_type const&>(bnRight), uCarry, uHigh, uLow);
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
			assign_mul_inner_product_left<uSum>(static_cast<big_number<unsigned, uLeft>::base_type const&>(bnLeft), bnRight, uCarry, uHigh, uLow);
		}
		template <unsigned uSum, unsigned uLeft, unsigned uRight>
		std::enable_if_t<(big_number<unsigned, uLeft>::THIS_INDEX + big_number<unsigned, uRight>::THIS_INDEX < uSum - 1)> assign_mul_inner_product_left(big_number<unsigned, uLeft> const & bnLeft, big_number<unsigned, uRight> const & bnRight, unsigned & uCarry, unsigned & uHigh, unsigned & uLow)
		{
			// NOP
		}
		template <unsigned u, unsigned uLeft, unsigned uRight>
		std::enable_if_t<(0 == u && THIS_INDEX == u),unsigned> assign_mul(big_number<unsigned, uLeft> const & bnLeft, big_number<unsigned, uRight> const & bnRight, unsigned & uCarry, unsigned uHigh)
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

			unsigned char const ubCarry = base_type::assign_mul<u - 1>(bnLeft, bnRight, uHigh, uLow);

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
				__debugbreak();
			}
		}

		template <unsigned u>
		int rand()
		{
			int n = base_type::rand<u - 1>();
			while (_rdrand32_step(&uBits) == 0 && ++n)
			{
				_mm_pause();
			}
			return n;
		}
		template <>
		int rand<0>()
		{
			int n = 0;
			while (_rdrand32_step(&uBits) == 0 && ++n)
			{
				_mm_pause();
			}
			return n;
		}
		int rand()
		{
			return rand<THIS_INDEX>();
		}
	};
	template <>
	struct big_number<unsigned, 0>
	{
		enum { THIS_BIT_COUNT = 0 };
		enum { BASE_BIT_COUNT = 0 };
	};

	//template <unsigned uBitCount>
	//struct big_number<__m128i, uBitCount>
	//	: big_number<__m128i, (uBitCount - 1)&~31U>
	//{
	//	using base_type = big_number<__m128i, (uBitCount - 1)&~31U>;

	//	enum { THIS_BIT_COUNT = (uBitCount + 31U)&~31U };
	//	enum { BASE_BIT_COUNT = (uBitCount - 1)&~31U };

	//	__m128i xmmBits;

	//	big_number()
	//		: xmmBits(_mm_setzero_si128())
	//	{}

	//	int rand()
	//	{
	//		int n = base_type::rand();

	//		unsigned uRand0;
	//		unsigned uRand1;
	//		unsigned uRand2;
	//		unsigned uRand3;
	//		{
	//			while (_rdrand32_step(&uRand0) == 0 && ++n) _mm_pause();
	//			while (_rdrand32_step(&uRand1) == 0 && ++n) _mm_pause();
	//			while (_rdrand32_step(&uRand2) == 0 && ++n) _mm_pause();
	//			while (_rdrand32_step(&uRand3) == 0 && ++n) _mm_pause();
	//		}
	//		xmmBits = _mm_set_epi32(uRand3, uRand2, uRand1, uRand0);

	//		return n;
	//	}

	//	std::tuple<int, int, int> cmp(__m128i const & n4B0) const
	//	{
	//		__m128i const n4A0 = xmmBits;

	//		__m128i const n4A1 = _mm_sub_epi32(n4A0, _mm_set1_epi32(-0x80000000));
	//		__m128i const n4B1 = _mm_sub_epi32(n4B0, _mm_set1_epi32(-0x80000000));

	//		int const nMaskEQ = _mm_movemask_epi8(_mm_cmpeq_epi32(n4A0, n4B0));
	//		int const nMaskGT = _mm_movemask_epi8(_mm_cmplt_epi32(n4B1, n4A1));
	//		int const nMaskLT = _mm_movemask_epi8(_mm_cmplt_epi32(n4A1, n4B1));

	//		return std::make_tuple(nMaskEQ, nMaskGT, nMaskLT);
	//	}

	//	template <unsigned uThatBitCount>
	//	std::enable_if_t<(big_number<__m128i, uThatBitCount>::THIS_BIT_COUNT == THIS_BIT_COUNT), int> cmpeq_carry (big_number<__m128i, uThatBitCount> const & that, int const nMaskAND) const
	//	{
	//		auto const n3MaskCMP = cmp(that);
	//		int const nMaskEQ = std::get<0>(n3MaskCMP) & nMaskAND;
	//		int const nMaskGT = std::get<1>(n3MaskCMP) & nMaskAND;
	//		int const nMaskLT = std::get<2>(n3MaskCMP) & nMaskAND;

	//		return nMaskEQ == 0 ? 0 : bast_type::cmpeq_carry(static_cast<base_type const&>(that), nMaskEQ);
	//	}
	//	template <unsigned uThatBitCount>
	//	std::enable_if_t<(big_number<__m128i, uThatBitCount>::THIS_BIT_COUNT < THIS_BIT_COUNT), int> cmpeq_carry(big_number<__m128i, uThatBitCount> const & that, int const nMaskAND) const
	//	{
	//		auto const n3MaskCMP = cmp(_mm_setzero_si128());
	//		int const nMaskEQ = std::get<0>(n3MaskCMP) & nMaskAND;
	//		int const nMaskGT = std::get<1>(n3MaskCMP) & nMaskAND;
	//		int const nMaskLT = std::get<2>(n3MaskCMP) & nMaskAND;

	//		return nMaskEQ == 0 ? 0 : bast_type::cmpeq_carry(that, nMaskEQ);
	//	}
	//	template <unsigned uThatBitCount>
	//	int operator == (big_number<__m128i, uThatBitCount> const & that) const
	//	{
	//		return cmpeq_carry(that, 0xFFFF);
	//	}

	//	template <unsigned uThatBitCount>
	//	std::enable_if_t<(big_number<__m128i, uThatBitCount>::THIS_BIT_COUNT == THIS_BIT_COUNT), int> cmplt_carry (big_number<__m128i, uThatBitCount> const & that, int const nMaskAND, int const nMaskOR) const
	//	{
	//		auto const n3MaskCMP = cmp(that.xmmBits);
	//		int const nMaskEQ = std::get<0>(n3MaskCMP) & nMaskAND;
	//		int const nMaskGT = std::get<1>(n3MaskCMP) & nMaskAND;
	//		int const nMaskLT = std::get<2>(n3MaskCMP) & nMaskAND;

	//		return nMaskEQ == 0 ? nMaskOR : base_type::cmplt_carry(static_cast<base_type const&>(that), nMaskEQ, nMaskLT | nMaskOR);
	//	}
	//	template <unsigned uThatBitCount>
	//	std::enable_if_t<(big_number<__m128i, uThatBitCount>::THIS_BIT_COUNT < THIS_BIT_COUNT), int> cmplt_carry (big_number<__m128i, uThatBitCount> const & that, int const nMaskAND, int const nMaskOR) const
	//	{
	//		auto const n3MaskCMP = cmp(_mm_setzero_si128());
	//		int const nMaskEQ = std::get<0>(n3MaskCMP) & nMaskAND;
	//		int const nMaskGT = std::get<1>(n3MaskCMP) & nMaskAND;
	//		int const nMaskLT = std::get<2>(n3MaskCMP) & nMaskAND;

	//		return nMaskEQ == 0 ? 0 : base_type::cmplt_carry(that, nMaskEQ, 0);
	//	}
	//	template <unsigned uThatBitCount>
	//	int operator < (big_number<__m128i, uThatBitCount> const & that) const
	//	{
	//		return cmplt_carry(that, 0xFFFF, 0);
	//	}

	//	__m128i add(__m128i const & n4B0, __m128i const & n4C0)
	//	{
	//		__m128i const n4A0 = xmmBits;

	//		__m128i const n4AB1 = _mm_add_epi32(n4A0, n4B0);

	//		__m128i const n4ABC2 = _mm_sub_epi32(n4AB1, n4C0);

	//		__m128i const n4A3LT = _mm_cmplt_epi32(n4ABC2, n4A0);
	//		__m128i const n4B3LT = _mm_cmplt_epi32(n4ABC2, n4B0);

	//		__m128i const n4AB4LT = _mm_or_si128(n4A3LT, n4B3LT);

	//		xmmBits = n4ABC2;
	//		return n4AB4LT;
	//	}
	//	template <unsigned uThatBitCount>
	//	std::enable_if_t<(big_number<__m128i, uThatBitCount>::THIS_BIT_COUNT == THIS_BIT_COUNT), __m128i> operator += (big_number<__m128i, uThatBitCount> const & that)
	//	{
	//		__m128i const n4C0 = base_type::operator += (static_cast<base_type const&>(that));
	//		__m128i const n4B0 = that.xmmBits;

	//		return add(n4B0, n4C0);
	//	}
	//	template <unsigned uThatBitCount>
	//	std::enable_if_t<(big_number<__m128i, uThatBitCount>::THIS_BIT_COUNT < THIS_BIT_COUNT), __m128i> operator += (big_number<__m128i, uThatBitCount> const & that)
	//	{
	//		return static_cast<big_number<__m128i, big_number<__m128i, uThatBitCount>::THIS_BIT_COUNT>*>(this)->operator += (that);
	//	}
	//};
	//template <>
	//struct big_number<__m128i, 0>
	//{
	//	enum { THIS_BIT_COUNT = 0 };
	//	enum { BASE_BIT_COUNT = 0 };
	//	int rand() const
	//	{
	//		return 0;
	//	}
	//	int cmpeq_carry(big_number const &, int const nMaskAND) const
	//	{
	//		return nMaskAND;
	//	}
	//	int cmplt_carry(big_number const &, int const nMaskAND, int const nMaskOR) const
	//	{
	//		return nMaskOR;
	//	}
	//	int operator == (big_number const &) const
	//	{
	//		return 0xFFFF;
	//	}
	//	int operator < (big_number const &) const
	//	{
	//		return 0;
	//	}
	//	__m128i operator += (big_number const &)
	//	{
	//		return _mm_setzero_si128();
	//	}
	//};

	//template <unsigned uBitCount>
	//inline std::tuple<int, int> cmpeq(std::tuple<big_number<__m128i, uBitCount>const&, big_number<__m128i, uBitCount>const&> const & bn2A, std::tuple<big_number<__m128i, uBitCount>const&, big_number<__m128i, uBitCount>const&> const & bn2B, std::tuple<int, int> const n2MaskAND)
	//{
	//	auto const & bnA0 = static_cast<big_number<__m128i, (uBitCount - 1)&~31U>const&>(std::get<0>(bn2A));
	//	auto const & bnA1 = static_cast<big_number<__m128i, (uBitCount - 1)&~31U>const&>(std::get<1>(bn2A));
	//	auto const & bnB0 = static_cast<big_number<__m128i, (uBitCount - 1)&~31U>const&>(std::get<0>(bn2B));
	//	auto const & bnB1 = static_cast<big_number<__m128i, (uBitCount - 1)&~31U>const&>(std::get<1>(bn2B));

	//	auto const n3MaskCMPa = std::get<0>(bn2A).cmp(std::get<0>(bn2B).xmmBits);
	//	auto const n3MaskCMPb = std::get<1>(bn2A).cmp(std::get<1>(bn2B).xmmBits);

	//	int const nMaskEQa = std::get<0>(n3MaskCMPa) & std::get<0>(n2MaskAND);
	//	int const nMaskEQb = std::get<0>(n3MaskCMPb) & std::get<1>(n2MaskAND);
	//	int const nMaskGTa = std::get<1>(n3MaskCMPa) & std::get<0>(n2MaskAND);
	//	int const nMaskGTb = std::get<1>(n3MaskCMPb) & std::get<1>(n2MaskAND);
	//	int const nMaskLTa = std::get<2>(n3MaskCMPa) & std::get<0>(n2MaskAND);
	//	int const nMaskLTb = std::get<2>(n3MaskCMPb) & std::get<1>(n2MaskAND);

	//	return (nMaskEQa && nMaskEQb)
	//		? cmpeq(std::tie(bnA0, bnA1), std::tie(bnB0, bnB1), std::make_tuple(nMaskEQa, nMaskEQb))
	//		: std::make_tuple(nMaskEQa == 0 ? 0 : bnA0.cmpeq(bnB0, nMaskEQa), nMaskEQb == 0 ? 0 : bnA1.cmpeq(bnB1, nMaskEQb));
	//}
}

//inline std::tuple<__m128i, __m128i> operator += (std::tuple<jrmwng::big_number<__m128i, 0>&, jrmwng::big_number<__m128i, 0>&> & bnA, std::tuple<jrmwng::big_number<__m128i, 0>const&, jrmwng::big_number<__m128i, 0>const&> const & bnB)
//{
//	return std::make_tuple(_mm_setzero_si128(), _mm_setzero_si128());
//}
//template <unsigned uBitCount>
//inline std::tuple<int, int> operator == (std::tuple<jrmwng::big_number<__m128i, uBitCount>const&, jrmwng::big_number<__m128i, uBitCount>const&> const & bnA, std::tuple<jrmwng::big_number<__m128i, uBitCount>const&, jrmwng::big_number<__m128i, uBitCount>const&> const & bnB)
//{
//	return jrmwng::cmpeq(bnA, bnB);
//}
//template <unsigned uBitCount>
//inline std::tuple<__m128i, __m128i> operator += (std::tuple<jrmwng::big_number<__m128i, uBitCount>&, jrmwng::big_number<__m128i, uBitCount>&> & bnA, std::tuple<jrmwng::big_number<__m128i, uBitCount>const&, jrmwng::big_number<__m128i, uBitCount>const&> const & bnB)
//{
//	auto       & bnA0 = static_cast<jrmwng::big_number<__m128i, (uBitCount - 1)&~31U>     &>(std::get<0>(bnA));
//	auto       & bnA1 = static_cast<jrmwng::big_number<__m128i, (uBitCount - 1)&~31U>     &>(std::get<1>(bnA));
//	auto const & bnB0 = static_cast<jrmwng::big_number<__m128i, (uBitCount - 1)&~31U>const&>(std::get<0>(bnB));
//	auto const & bnB1 = static_cast<jrmwng::big_number<__m128i, (uBitCount - 1)&~31U>const&>(std::get<1>(bnB));
//
//	auto const n8C0 = (std::tie(bnA0, bnA1) += std::tie(bnB0, bnB1));
//
//	__m128i const n4C1a = std::get<0>(bnA).add(std::get<0>(bnB).xmmBits, std::get<0>(n8C0));
//	__m128i const n4C1b = std::get<1>(bnA).add(std::get<1>(bnB).xmmBits, std::get<1>(n8C0));
//
//	return std::make_tuple(n4C1a, n4C1b);
//}
//template <unsigned uBitCount>
//inline std::tuple<__m128i, __m128i> operator += (std::tuple<jrmwng::big_number<__m128i, uBitCount>&, jrmwng::big_number<__m128i, uBitCount>&> & bnA, std::tuple<jrmwng::big_number<__m128i, uBitCount>&, jrmwng::big_number<__m128i, uBitCount>&> const & bnB)
//{
//	auto const & bnB0 = std::get<0>(bnB);
//	auto const & bnB1 = std::get<1>(bnB);
//
//	return bnA += std::tie(bnB0, bnB1);
//}
