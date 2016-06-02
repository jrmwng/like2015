#pragma once

#include <intrin.h>

namespace jrmwng
{
	struct Qbase;

	template <typename Tchar, typename Tmm>
	struct Qtraits
	{
		using char_type = Tchar;
		using mm_type = Tmm;
	};

	namespace internals
	{
		template <typename Tmm, typename Tchar, typename... Targs>
		struct QmmChar
		{
			static Tmm to_mm(Targs const &... vArgs)
			{
				return QmmChar<Tmm, Tchar, Targs..., Tchar>::to_mm(vArgs..., Tchar(0));
			}
		};
		template <typename... Tchar>
		struct QmmChar<__m128i, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, Tchar...>
		{
			static __m128i to_mm(char c0, char c1, char c2, char c3, char c4, char c5, char c6, char c7, char c8, char c9, char cA, char cB, char cC, char cD, char cE, char cF, Tchar...)
			{
				return _mm_setr_epi8(c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, cA, cB, cC, cD, cE, cF);
			}
		};
		template <typename... Tchar>
		struct QmmChar<__m128i, wchar_t, wchar_t, wchar_t, wchar_t, wchar_t, wchar_t, wchar_t, wchar_t, wchar_t, Tchar...>
		{
			static __m128i to_mm(wchar_t c0, wchar_t c1, wchar_t c2, wchar_t c3, wchar_t c4, wchar_t c5, wchar_t c6, wchar_t c7, Tchar...)
			{
				return _mm_setr_epi16(c0, c1, c2, c3, c4, c5, c6, c7);
			}
		};
		template <typename Tchar, typename... Tmm>
		struct QmmChar<__m128i, Tchar, __m128i, Tmm...>
		{
			static __m128i to_mm(__m128i const & xmm, Tmm const &...)
			{
				return xmm;
			}
		};
		template <typename Tmm, typename... Targs>
		static Tmm q_char_mm(char c0, Targs... vArgs)
		{
			return QmmChar<Tmm, char, char, Targs...>::to_mm(c0, vArgs...);
		}
		template <typename Tmm, typename... Targs>
		static Tmm q_char_mm(wchar_t c0, Targs... vArgs)
		{
			return QmmChar<Tmm, wchar_t, wchar_t, Targs...>::to_mm(c0, vArgs...);
		}
		template <typename Tchar, typename... Targs>
		static __m128i q_char_mm(__m128i const & xmm0, Targs const &... vArgs)
		{
			return QmmChar<__m128i, Tchar, __m128i, Targs...>::to_mm(xmm0, vArgs...);
		}
		template <int nMode, size_t uSize, typename Tchar, typename Tmm>
		struct Qpattern;

		template <int nMode>
		struct Qpattern<nMode, 0, char, __m128i>
		{
			int match(__m128i const &) const
			{
				return 0;
			}
		};
		template <int nMode, size_t uSize>
		struct Qpattern<nMode, uSize, char, __m128i>
			: Qpattern<nMode, (uSize > 16U ? uSize - 16U : 0U), char, __m128i>
		{
			using base_type = Qpattern<nMode, (uSize > 16U ? uSize - 16U : 0U), char, __m128i>;

			__m128i const m_xmmPattern;

			template <typename... Tchar>
			Qpattern(char c0, Tchar... vChar)
				: m_xmmPattern(q_char_mm<__m128i>(c0, vChar...))
			{}
			template <typename... Tchar>
			Qpattern(char c0, char c1, char c2, char c3, char c4, char c5, char c6, char c7, char c8, char c9, char cA, char cB, char cC, char cD, char cE, char cF, Tchar... vChar)
				: base_type(vChar...)
				, m_xmmPattern(q_char_mm<__m128i>(c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, cA, cB, cC, cD, cE, cF))
			{}
			template <typename... Tmm>
			Qpattern(__m128i const & xmm0, Tmm const &... vArgs)
				: base_type(vArgs...)
				, m_xmmPattern(xmm0)
			{}

			int match(__m128i const & xmmInput) const
			{
				return
					_mm_cmpestri(xmmInput, 16, m_xmmPattern, (uSize > 16U ? 16 : uSize), (nMode & ~3) | _SIDD_UBYTE_OPS)
					|
					base_type::match(xmmInput);
			}
			int match(char const *pc) const
			{
				return match(_mm_loadu_si128(reinterpret_cast<__m128i const*>(pc)));
			}
		};
	}

	template <typename Tmm, typename... Tchar>
	auto q_range(char c0, Tchar... vArgs)
	{
		return internals::Qpattern<(_SIDD_CMP_RANGES), 1 + sizeof...(Tchar), char, Tmm>(c0, vArgs...);
	}
	template <size_t uSize, typename Tchar, typename... Tmm>
	auto q_range(__m128i const & xmm0, Tmm... vArgs)
	{
		return internals::Qpattern<(_SIDD_CMP_RANGES), uSize, Tchar, __m128i>(xmm0, vArgs...);
	}

	template <typename Tmm, typename... Tchar>
	auto q_set(char c0, Tchar... vArgs)
	{
		return internals::Qpattern<(_SIDD_CMP_EQUAL_ANY), 1 + sizeof...(Tchar), char, Tmm>(c0, vArgs...);
	}

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
