#pragma once

/* Author: jrmwng @ 2016 */

#include <intrin.h>

namespace jrmwng
{
	template <typename T, unsigned uBitCount>
	struct big_number;
	template <typename T>
	struct big_number<T, 0>
	{};

	template <unsigned uBitCount>
	struct big_number<unsigned, uBitCount>
		: big_number<unsigned, uBitCount - 32>
	{
		using base_type = big_number<unsigned, uBitCount - 32>;

		unsigned uBits;

		big_number()
			: uBits(0)
		{}

		unsigned char operator += (big_number const & that)
		{
			unsigned char ubC0 = base_type::operator += (that);

			unsigned char ubC1 = _addcarryx_u32(ubC0, this->uBits, that.uBits, &this->uBits);

			return ubC1; // carry
		}
	};
	template <>
	struct big_number<unsigned, 0>
	{
		unsigned char operator += (big_number const &)
		{
			return 0; // carry
		}
	};

	template <unsigned uBitCount>
	struct big_number<__m128i, uBitCount>
		: big_number<__m128i, uBitCount - 32>
	{
		using base_type = big_number<__m128i, uBitCount - 32>;

		__m128i xmmBits;

		big_number()
			: xmmBits(_mm_setzero_si128())
		{}

		__m128i operator += (big_number const & that)
		{
			__m128i const n4C0 = base_type::operator += (that);

			__m128i const n4A0 = this->xmmBits;
			__m128i const n4B0 = that.xmmBits;

			__m128i const n4AB1 = _mm_add_epi32(n4A0, n4B0);

			__m128i const n4ABC2 = _mm_sub_epi32(n4AB1, n4C0);

			__m128i const n4A3LT = _mm_cmplt_epi32(n4AB2, n4A0);
			__m128i const n4B3LT = _mm_cmplt_epi32(n4AB2, n4B0);

			__m128i const n4C4LT = _mm_or_si128(n4C4LT);

			this->xmmBits = n4ABC2;
			return n4C4LT; // carry
		}
	};
	template <>
	struct big_number<__m128i, 0>
	{
		__m128i operator += (big_number const &)
		{
			return _mm_setzero_si128();
		}
	};
}
