#pragma once

#include <intrin.h>

namespace jrmwng
{
	struct Qbase;

	template <typename Tchar, Tchar... vcRange>
	struct Qrange;

	namespace internals
	{
		template <typename Tchar, Tchar... vChar>
		struct Qchar;

		template <char... vChar>
		struct Qchar<char, vChar...>
		{
			static __m128i xmm()
			{
				return Qchar<char, vChar..., '\0'>::xmm();
			}
		};
		template <char c0, char c1, char c2, char c3, char c4, char c5, char c6, char c7, char c8, char c9, char cA, char cB, char cC, char cD, char cE, char cF, char... vChar>
		struct Qchar<char, c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, cA, cB, cC, cD, cE, cF, vChar...>
		{
			static __m128i xmm()
			{
				return _mm_set_epi8(cF, cE, cD, cC, cB, cA, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0);
			}
		};
		template <typename... Tchar>
		static __m128i q_char_xmm(Tchar... vc)
		{
			return q_char_xmm(vc..., '\0');
		}
		template <typename... Tchar>
		static __m128i q_char_xmm<char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, Tchar...>
			(char c0, char c1, char c2, char c3, char c4, char c5, char c6, char c7, char c8, char c9, char cA, char cB, char cC, char cD, char cE, char cF, char... vChar)
		{
			return _mm_set_epi8(cF, cE, cD, cC, cB, cA, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0);
		}
	}

	template <> struct Qrange<char>
	{
		__m128i const m_xmmPattern;
		int const m_nCount;

		Qrange(__m128i const & xmmPattern, int const nCount)
			: m_xmmPattern(xmmPattern)
			, m_nCount(nCount)
		{}

		Qrange(char const *pcPattern)
			: m_xmmPattern(_mm_loadu_si128(reinterpret_cast<__m128i const*>(pcPattern)))
			, m_nCount(std::min<int>(16, strlen(pcPattern)))
		{}

		template <typename Tchar...>
		Qrange(Tchar... vcParam)
			: m_xmmPattern(internals::Qchar<char>::xmm(vcParam...))
			, m_nCount(std::min<int>(16, sizeof...(Tchar)))
		{}

		int match(__m128i const & xmmInput) const
		{
			return _mm_cmpestri(xmmInput, 16, m_xmmPattern, m_nCount, _SIDD_UBYTE_OPS | _SIDD_CMP_RANGES | _SIDD_BIT_MASK | _SIDD_MASKED_POSITIVE_POLARITY);
		}
	};
	template <char... vcRange>
	struct Qrange<char, vcRange...>
	{
		int match(__m128i const & xmmInput0) const
		{
			__m128i xmmPattern0 = internals::Qchar<char, vcRange...>::xmm();

			Qrange<char> qRange1(xmmPattern0);

			int nMask2 = qRange1.match(xmmInput0);

			return nMask2;
		}
	};
	template <char c0, char c1, char c2, char c3, char c4, char c5, char c6, char c7, char c8, char c9, char cA, char cB, char cC, char cD, char cE, char cF, char cG, char... vcRange>
	struct Qrange<char, c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, cA, cB, cC, cD, cE, cF, cG, vcRange...>
	{
		int match(__m128i const & xmmInput0) const
		{
			return
				Qrange<char, c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, cA, cB, cC, cD, cE, cF>().match(xmmInput0)
				|
				Qrange<char, cG, vcRange...>().match(xmmInput0);
		}
	};
	template <typename... Tchar>
	Qrange<char> q_range(Tchar... vChar)
	{
		return Qrange<char>(vChar...);
	}

	template <typename Tchar, Tchar... vcSet>
	struct Qset;

	template <char... vcSet>
	struct Qset<char, vcSet...>
	{
		int match(__m128i const & xmmInput0, __m128i const & xmmPattern0)
		{
			return _mm_cmpestri(xmmInput0, 16, xmmPattern0, sizeof...(vcSet), _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_BIT_MASK | _SIDD_MASKED_POSITIVE_POLARITY);
		}
	};

	template <typename Tchar, Tchar... vcExact>
	struct Qexact;

	template <typename Texpr, size_t uMin, size_t uMax = 0>
	struct Qtimes;

	template <typename... vtExpr>
	struct Qexpr;

	template <typename... vtExpr>
	struct Qor;

	template <typename Texpr>
	struct Qnot;

	// using Qdecimal        = Qrange<char, '0','9'>;
	// using Qalpha          = Qrange<char, 'a','z', 'A','Z'>;
	// using Qhexadecimal    = Qrange<char, 'a','z', 'A','Z', '0','9'>;
	// using Qhexadecimal    = Qor   <Qdecimal, Qalpha>;
	// using Qwhitespace     = Qset  <char, ' ', '\t', '\r', '\n', '\v', '\t'>;
	// using QnonWhitespace  = Qnot  <Qwhitespace>;
	// using Qif             = Qexact<char, 'i','f'>;
	// using Qfor            = Qexact<char, 'f','o','r'>;
	// using Qelse           = Qexact<char, 'e','l','s','e'>;
	// using Qwhile          = Qexact<char, 'w','h','i','l','e'>;
	// using QpositiveNumber = Qtimes<Qdecimal, 1>;
	// using Qsign           = Qset  <char, '+', '-'>;
	// using Qnumber         = Qexpr <Qsign, QpositiveNumber>;
	// using Qnumber         = Qexpr <Qset<char, '+', '-'>, Qtimes<Qrange<char, '0','9'>, 1>>;
	// using QhkidLetters    = Qtimes<Qalpha, 1, 2>;
	// using QhkidLetters    = Qtimes<Qrange<char, 'A', 'Z'>, 1, 2>;
	// using QhkidNumbers    = Qtimes<Qdecimal, 6, 6>;
	// using QhkidNumbers    = Qtimes<Qrange<char, '0', '9'>, 6, 6>;
	// using QhkidCheckDigit = Qrange<char, '0', '9', 'A', 'A'>;
	// using Qhkid           = Qexpr <QhkidLetters, QhkidNumbers, Qset<char, '('>, QhkidCheckDigit, Qset<char, ')'>>;
	// using Qhkid           = Qexpr <Qtimes<Qrange<char, 'A','Z'>, 1, 2>, Qtimes<Qrange<char, '0', '9'>, 6, 6>, Qset<char, '('>, Qrange<char, '0', '9', 'A', 'A'>, Qset<char, ')'>>;

	// Qrange<char> qDecimal       ('0', '9');
	// Qrange<char> qDecimal       ("09");
	// Qrange<char> qAlpha         ('A', 'Z', 'a', 'z');
	// Qrange<char> qAlpha         ("AZaz");
	// Qrange<char> qHexadecimal   ('A', 'Z', 'a', 'z', '0', '9');
	// Qrange<char> qHexadecimal   ("AZaz09");
	// Qrange<char> qHexadecimal   (qAlpha , qDecimal);
	// Qrange<char> qHexadecimal   (qAlpha | qDecimal);
	// Qset  <char> qWhitespace    (' ', '\t', '\r', '\n', '\v', '\t');
	// Qset  <char> qWhitespace    (" \t\r\n\v\t");
	// Qset  <char> qNonWhitespace (!qWhiteSpace);
	// Qtimes<char> qPositiveNumber(qDecimal, 1);
	// Qset  <char> qSign          ('+', '-');
	// Qset  <char> qSign          ("+-");
	// Qtimes<char> qHkidLetters   (qAlpha, 1, 2);
	// Qtimes<char> qHkIdNumbers   (qDecimal, 6, 6);
	// Qrange<char> qHkidCheckDigit(qDecimal, 'A', 'A');

	// auto qDecimal        = q_range('0','9');
	// auto qDecimal        = q_range("09");
	// auto qAlpha          = q_range('A','Z', 'a','z');
	// auto qAlpha          = q_range("AZaz");
	// auto qHexadecimal    = q_range('A','Z', 'a','z', '0','9');
	// auto qHexadecimal    = q_range("AZaz09");
	// auto qHexadecimal    = q_range(qAlpha , qDecimal);
	// auto qHexadecimal    = q_range(qAlpha | qDecimal);
	// auto qWhitespace     = q_set  (' ', '\t', '\r', '\n', '\v', '\t');
	// auto qWhitespace     = q_set  (" \t\r\n\v\t");
	// auto qNonWhitespace  = q_set  (!qWhitespace);
	// auto qPositiveNumber = q_times(qDecimal, 1);
	// auto qSign           = q_sign ('+', '-');
	// auto qSign           = q_sign ("+-");
	// auto qNumber         = q_expr (qSign, qPositiveNumber);
	// auto qNumber         = q_expr (q_set('+', '-'), q_times(q_range('0', '9'), 1));
	// auto qHkidLetters    = q_times(qAlpha, 1, 2);
	// auto qHkidLetters    = q_times(q_range('A', 'Z'), 1, 2);
	// auto qHkidNumbers    = q_times(qDecimal, 6, 6);
	// auto qHkidNumbers    = q_times(q_range('0', '9'), 6, 6);
	// auto qHkidCheckDigit = q_range(qDecimal, 'A', 'A');
	// auto qHkidCheckDigit = q_range('0', '9', 'A', 'A');
	// auto qHKID           = q_expr (qHkidLetters, qHkidNumbers, '(', qHkidCheckDigit, ')');
	// auto qHKID           = q_expr (q_times(q_set('A', 'Z'), 1, 2), q_times(q_set('0', '9'), 6, 6), q_set('('), q_range('0', '9', 'A', 'A'), q_set(')'));

	// int nHKID           = qHKID          ("HKID: A123456(7)");
	//                                        0000001000000000
	// int nAlpha          = qAlpha         ("HKID: A123456(7)"); // "AZ"
	//                                        1111001000000000
	// int nHkidLetters    = qHkidLetters   ("HKID: A123456(7)"); // "AZ"
	//                                        1111001000000000
	// int nDecimal        = qDecimal       ("HKID: A123456(7)"); // "09"
	//                                        0000000111111010
	// int nHkidNumbers    = qHkidNumbers   ("HKID: A123456(7)"); // "09"
	//                                        0000000100000000
	// int nHkidCheckDigit = qHkidCheckDigit("HKID: A123456(7)"); // "09AA"
	//                                        0000001111111010

	//                                        1111001000000000 // "AZ"
	//                                        0000000111111010 //   "09"
	//                                        0000000000000100 //     "("
	//                                        0000001111111010 //      "09AA"
	//                                        0000000000000001 //          ")"
	//                                                         // "AZ09(09AA)"
	//                                                         // "AZ09AA()"
}
