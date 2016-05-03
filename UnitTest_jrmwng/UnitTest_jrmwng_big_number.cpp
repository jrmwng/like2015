#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework
		{
			template <> inline std::wstring ToString<jrmwng::big_number<unsigned, 64>>(jrmwng::big_number<unsigned, 64> const & bn) { return bn.ToString(); }
		}
	}
}

namespace UnitTest_jrmwng
{		
	TEST_CLASS(UnitTest_jrmwng_big_number)
	{
	public:
		TEST_METHOD(TestMethod_jrmwng_big_number_unsigned)
		{
			jrmwng::big_number<unsigned, 64> bn64A;
			jrmwng::big_number<unsigned, 64> bn64B;
			jrmwng::big_number<unsigned, 64> bn64C;
			jrmwng::big_number<unsigned, 64> bn64D;
			{
				bn64A.rand();
				bn64A = 0xFFFFFFFFU;
				Assert::IsTrue(bn64A == 0xFFFFFFFFU);

				bn64B.rand();
				bn64B = 0x00000001U;
				Assert::IsTrue(bn64B == 0x00000001U);

				bn64C.rand();
				bn64C = bn64A + bn64B;
				Assert::IsTrue(bn64C == 0x100000000ULL);

				std::wstring str = bn64A.ToString();

				bn64D.rand();
				bn64D = bn64C - bn64A;
				Assert::IsTrue(bn64D == bn64B);

				bn64D = bn64C - bn64B;
				Assert::IsTrue(bn64D == bn64A);
				Assert::AreEqual(bn64A, bn64D);
			}
		}
	};
}