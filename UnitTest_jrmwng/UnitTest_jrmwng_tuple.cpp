#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\jrmwng\tuple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_jrmwng
{
	TEST_CLASS(UnitTest_jrmwng_tuple)
	{
	public:
		TEST_METHOD(TestMethod_jrmwng_tuple)
		{
			jrmwng::for_each(std::make_tuple(0, 0U, 0L, 0UL, 0LL, 0ULL), [](auto const value)
			{
				Assert::IsTrue(value == 0);
			});
			// TODO: Your test code here
		}
	};
}