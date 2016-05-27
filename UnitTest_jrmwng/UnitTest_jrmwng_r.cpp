#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\jrmwng\r.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_jrmwng
{		
	TEST_CLASS(UnitTest_jrmwng)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			jrmwng::Rvar<int> nA = 0;
			jrmwng::Rvar<long> lB = 1L;
			jrmwng::Rvar<short> wC = 2;

			jrmwng::Rvar<long long> xlZ = ~nA + lB - wC * jrmwng::r(-1);
			// TODO: Your test code here
		}
	};
}