#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\jrmwng\allocator32x.h"
#include "..\jrmwng\tuple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_jrmwng
{		
	TEST_CLASS(UnitTest_jrmwng_allocator32x)
	{
	public:
		TEST_METHOD(TestMethod_jrmwng_allocator32x)
		{
			jrmwng::for_each(
				std::make_tuple(jrmwng::allocator32x<1>(), jrmwng::allocator32x<4>(), jrmwng::allocator32x<8>(), jrmwng::allocator32x<16>()),
				[=](auto & stAllocator)
			{
				for (unsigned k = 16; k > 0; k >>= 1)
				{
					for (unsigned j = (k >> 1) + 1; j <= k; j++)
					{
						for (unsigned i = 0; i < stAllocator.end(); i += k)
						{
							unsigned uIndex = stAllocator.allocate(j);
							if (uIndex != i)
							{
								Assert::AreEqual<unsigned>(i, uIndex);
							}
						}
						for (unsigned i = 0; i < stAllocator.end(); i += k)
						{
							stAllocator.deallocate(i, j);
						}
					}
				}

				unsigned uIndex = stAllocator.allocate(0);
				if (uIndex != stAllocator.end())
				{
					Assert::AreEqual<unsigned>(32, uIndex);
				}
			});
		}
	};
}