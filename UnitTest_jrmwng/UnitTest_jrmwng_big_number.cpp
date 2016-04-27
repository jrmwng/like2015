#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_jrmwng
{		
	TEST_CLASS(UnitTest_jrmwng_big_number)
	{
	public:
		TEST_METHOD(TestMethod_jrmwng_big_number)
		{
			jrmwng::big_number<unsigned, 512> bnA;
			jrmwng::big_number<unsigned, 512> bnB;

			bnA += bnB;
			// TODO: Your test code here
		}
	};
}