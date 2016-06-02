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

			auto const qAlpha = q_range<__m128i>('A', 'Z', 'a', 'z');
			auto const qDecimal = q_range<__m128i>('0', '9');

			int const nMask = qDecimal.match("0123456789ABCDEF");

			Assert::AreEqual(0x3FF, nMask);
		}
	};
}