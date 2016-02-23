#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_jrmwng
{		
	TEST_CLASS(UnitTest_jrmwng)
	{
	public:
		
		TEST_METHOD(TestMethod_jrmwng_align)
		{
			auto const stAlignPage = jrmwng::align_page(0);
			auto const stAlignSect = jrmwng::align_section(0);
			auto const stAlignLine = jrmwng::align_line(0);
			auto const stAlignLock = jrmwng::align_lock(0);

			auto const uAlignPage = reinterpret_cast<uintptr_t>(&stAlignPage);
			auto const uAlignSect = reinterpret_cast<uintptr_t>(&stAlignSect);
			auto const uAlignLine = reinterpret_cast<uintptr_t>(&stAlignLine);
			auto const uAlignLock = reinterpret_cast<uintptr_t>(&stAlignLock);
		}

	};
}