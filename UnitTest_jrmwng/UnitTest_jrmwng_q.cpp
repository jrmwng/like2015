#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\jrmwng\q.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_jrmwng
{		
	TEST_CLASS(UnitTest_jrmwng_q)
	{
	public:
		TEST_METHOD(TestMethod_jrmwng_q)
		{
			using namespace jrmwng;

			auto const qHKID = q_exact<__m128i>('H', 'K', 'I', 'D');
			auto const qHKIDothers = q_set<__m128i>(':', '(', ')');
			auto const qWhitespace = q_set<__m128i>(' ', '\t', '\r', '\n', '\v', '\t');
			auto const qHKIDletter = q_range<__m128i>('A', 'Z');
			auto const qHKIDdigits = q_range<__m128i>('0', '9');
			auto const qHKIDcheck = q_range<__m128i>('0', '9', 'A', 'A');

			char const *pcInput = "HKID: A123456(7)";
								// A000000000000000
								// 0000E00000000N0P
								// 00000F0000000000
								// 111100G000000000
								// 0000000HIJKLM010
								// 00000011111110O0

			int const nHKID = qHKID.match(pcInput);
			int const nHKIDothers = qHKIDothers.match(pcInput);
			int const nWhitespace = qWhitespace.match(pcInput);
			int const nHKIDletter = qHKIDletter.match(pcInput);
			int const nHKIDdigits = qHKIDdigits.match(pcInput);
			int const nHKIDcheck = qHKIDcheck.match(pcInput);

			Assert::AreEqual(1, nHKID);
			Assert::AreEqual(0xA010, nHKIDothers);
			Assert::AreEqual(0x20, nWhitespace);
			Assert::AreEqual(0x4F, nHKIDletter);
			Assert::AreEqual(0x5F80, nHKIDdigits);
			Assert::AreEqual(0x5FC0, nHKIDcheck);
		}
	};
}