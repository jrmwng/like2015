#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\like2015\functional.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace like2015UnitTest
{		
	TEST_CLASS(like2015UnitTest_functional)
	{
	public:
		int m_nDummy;

		struct SDummy
		{
			int nDummy;
		} m_stDummy;

		TEST_METHOD(compile_functional)
		{
			auto f1 = like::field(&like2015UnitTest_functional::m_nDummy);
			m_nDummy = 1;
			Assert::IsTrue(f1(*this) == m_nDummy);
			Assert::IsTrue(f1(this) == m_nDummy);

			auto f2 = like::field(&like2015UnitTest_functional::m_stDummy, &SDummy::nDummy);
			m_stDummy.nDummy = 2;
			Assert::IsTrue(f2(*this) == m_stDummy.nDummy);
			Assert::IsTrue(f2(this) == m_stDummy.nDummy);
			// TODO: Your test code here
		}
	};
}