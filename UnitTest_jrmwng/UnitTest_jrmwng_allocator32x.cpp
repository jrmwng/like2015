#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\jrmwng\allocator32x.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_jrmwng
{		
	TEST_CLASS(UnitTest_jrmwng_allocator32x)
	{
	public:
		TEST_METHOD(TestMethod_jrmwng_allocator32x)
		{
			jrmwng::allocator32x<1> stAllocator;

			unsigned uIndex;

			uIndex = stAllocator.allocate(1);
			Assert::AreEqual<unsigned>(0, uIndex);
			stAllocator.deallocate(uIndex, 1);


			// TODO: Your test code here
		}
	};
}