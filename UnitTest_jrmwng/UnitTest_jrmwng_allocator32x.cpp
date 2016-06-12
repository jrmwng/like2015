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

			for (unsigned j = 16; j > 0; j >>= 1)
			{
				for (unsigned i = 0; i < 32; i+=j)
				{
					unsigned uIndex = stAllocator.allocate(j);
					if (uIndex != i)
					{
						Assert::AreEqual<unsigned>(i, uIndex);
					}
				}
				for (unsigned i = 0; i < 32; i+=j)
				{
					stAllocator.deallocate(i, j);
				}
			}


			// TODO: Your test code here
		}
	};
}