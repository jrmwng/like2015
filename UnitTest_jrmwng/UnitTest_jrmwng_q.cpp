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

			Qrange<char, '0', '9'> const qDecimal;

			qDecimal.match(_mm_setzero_si128());
			// TODO: Your test code here
		}
	};
}