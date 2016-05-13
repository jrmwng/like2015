#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework
		{
			template <> inline std::wstring ToString<jrmwng::big_number<unsigned, 32>>(jrmwng::big_number<unsigned, 32> const & bn) { return bn.ToString(); }
			template <> inline std::wstring ToString<jrmwng::big_number<unsigned, 64>>(jrmwng::big_number<unsigned, 64> const & bn) { return bn.ToString(); }
			template <> inline std::wstring ToString<jrmwng::big_number<unsigned, 96>>(jrmwng::big_number<unsigned, 96> const & bn) { return bn.ToString(); }
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
			jrmwng::big_number<unsigned, 128> bn128_10000000000000000;
			jrmwng::big_number<unsigned, 64> bn64_100000000;
			jrmwng::big_number<unsigned, 64> bn64_FFFFFFFF;
			jrmwng::big_number<unsigned, 64> bn64_1;
			jrmwng::big_number<unsigned, 32> bn32_1;
			jrmwng::big_number<unsigned, 32> bn32;
			jrmwng::big_number<unsigned, 64> bn64;
			jrmwng::big_number<unsigned, 96> bn96;
			{
				bn32_1.rand();
				bn32_1 = 1U;
				Assert::AreEqual(std::wstring(L"0x00000001"), bn32_1.ToString());

				bn64_1.rand();
				bn64_1 = bn32_1;
				Assert::AreEqual(std::wstring(L"0x0000000000000001"), bn64_1.ToString());

				bn64_FFFFFFFF.rand();
				bn64_FFFFFFFF = 0xFFFFFFFFU;
				Assert::AreEqual(std::wstring(L"0x00000000FFFFFFFF"), bn64_FFFFFFFF.ToString());

				bn64_100000000.rand();
				bn64_100000000 = bn64_FFFFFFFF + bn32_1;
				Assert::AreEqual(std::wstring(L"0x0000000100000000"), bn64_100000000.ToString());
				Assert::IsTrue(bn64_100000000 == 0x100000000ULL);

				bn128_10000000000000000.rand();
				bn128_10000000000000000 = bn64_100000000 * bn64_100000000;
				Assert::AreEqual(std::wstring(L"0x00000000000000010000000000000000"), bn128_10000000000000000.ToString());

				//

				bn64.rand();
				bn64 = bn32_1 + bn64_FFFFFFFF;
				Assert::AreEqual(bn64_100000000, bn64);
				bn64.rand();
				bn64 = bn64_1 + bn64_FFFFFFFF;
				Assert::AreEqual(bn64_100000000, bn64);

				bn64.rand();
				bn64 = bn64_FFFFFFFF + bn32_1;
				Assert::AreEqual(bn64_100000000, bn64);
				bn64.rand();
				bn64 = bn64_FFFFFFFF + bn64_1;
				Assert::AreEqual(bn64_100000000, bn64);

				//

				bn64.rand();
				bn64 = bn64_100000000 - bn64_FFFFFFFF;
				Assert::AreEqual(bn64_1, bn64);

				bn64.rand();
				bn64 = bn64_100000000 - bn32_1;
				Assert::AreEqual(bn64_FFFFFFFF, bn64);

				//

				bn64.rand();
				bn64 = bn64_FFFFFFFF * bn32_1;
				Assert::AreEqual(bn64_FFFFFFFF, bn64);

				bn64.rand();
				bn64 = bn32_1 * bn64_FFFFFFFF;
				Assert::AreEqual(bn64_FFFFFFFF, bn64);

				bn96.rand();
				bn96 = bn64_100000000 * bn64_100000000;
			}
		}
	};
}