#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\jrmwng\for_each_tuple.h"

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
		template <typename TInput1, typename TInput2, typename TOutcome3, typename TFunc4>
		void TestMethod_jrmwng_big_number_unsigned_operate(TInput1 const & tupleA, TInput2 const & tupleB, TOutcome3 const & tupleCD, TFunc4 const & tupleE)
		{
			jrmwng::for_each_tuple(tupleA, [&](auto const & bnA)
			{
				jrmwng::for_each_tuple(tupleB, [&](auto const & bnB)
				{
					jrmwng::for_each_tuple(tupleCD, [&](auto const & elemCD)
					{
						auto & bnC = std::get<0>(elemCD);
						auto const & strD = std::get<1>(elemCD);
						jrmwng::for_each_tuple(tupleE, [&](auto const & fnE)
						{
							bnC.rand();
							bnC = fnE(bnA, bnB);
							Assert::AreEqual(strD, bnC.ToString());
						});
					});
				});
			});
		}
	public:
		TEST_METHOD(TestMethod_jrmwng_big_number_unsigned)
		{
			jrmwng::big_number<unsigned, 128> bn128_10000000000000000;
			jrmwng::big_number<unsigned, 96> bn96_10000000000000000;
			jrmwng::big_number<unsigned, 96> bn96_100000000;
			jrmwng::big_number<unsigned, 64> bn64_100000000;
			jrmwng::big_number<unsigned, 32> bn32_100000000;
			jrmwng::big_number<unsigned, 96> bn96_FFFFFFFF;
			jrmwng::big_number<unsigned, 64> bn64_FFFFFFFF;
			jrmwng::big_number<unsigned, 32> bn32_FFFFFFFF;
			jrmwng::big_number<unsigned, 96> bn96_1;
			jrmwng::big_number<unsigned, 64> bn64_1;
			jrmwng::big_number<unsigned, 32> bn32_1;
			jrmwng::big_number<unsigned, 96> bn96;
			jrmwng::big_number<unsigned, 64> bn64;
			jrmwng::big_number<unsigned, 32> bn32;
			{
				bn32_1.rand();
				bn32_1 = 1U;
				Assert::AreEqual(std::wstring(L"0x00000001"), bn32_1.ToString());
				Assert::IsTrue(bn32_1 == 1U);
				bn64_1.rand();
				bn64_1 = 1U;
				Assert::AreEqual(std::wstring(L"0x0000000000000001"), bn64_1.ToString());
				Assert::IsTrue(bn64_1 == 1U);
				bn96_1.rand();
				bn96_1 = 1U;
				Assert::AreEqual(std::wstring(L"0x000000000000000000000001"), bn96_1.ToString());
				Assert::IsTrue(bn96_1 == 1U);

				bn32_FFFFFFFF.rand();
				bn32_FFFFFFFF = 0xFFFFFFFFU;
				Assert::AreEqual(std::wstring(L"0xFFFFFFFF"), bn32_FFFFFFFF.ToString());
				Assert::IsTrue(bn32_FFFFFFFF == 0xFFFFFFFFU);
				bn64_FFFFFFFF.rand();
				bn64_FFFFFFFF = 0xFFFFFFFFU;
				Assert::AreEqual(std::wstring(L"0x00000000FFFFFFFF"), bn64_FFFFFFFF.ToString());
				Assert::IsTrue(bn64_FFFFFFFF == 0xFFFFFFFFU);
				bn96_FFFFFFFF.rand();
				bn96_FFFFFFFF = 0xFFFFFFFFU;
				Assert::AreEqual(std::wstring(L"0x0000000000000000FFFFFFFF"), bn96_FFFFFFFF.ToString());
				Assert::IsTrue(bn96_FFFFFFFF == 0xFFFFFFFFU);

				bn64_100000000.rand();
				bn64_100000000 = bn64_FFFFFFFF + bn64_1;
				Assert::AreEqual(std::wstring(L"0x0000000100000000"), bn64_100000000.ToString());
				Assert::IsTrue(bn64_100000000 == 0x100000000ULL);
				bn96_100000000.rand();
				bn96_100000000 = bn64_FFFFFFFF + bn64_1;
				Assert::AreEqual(std::wstring(L"0x000000000000000100000000"), bn96_100000000.ToString());
				Assert::IsTrue(bn96_100000000 == 0x100000000ULL);

				bn96_10000000000000000.rand();
				bn96_10000000000000000 = bn64_100000000 * bn64_100000000;
				Assert::AreEqual(std::wstring(L"0x000000010000000000000000"), bn96_10000000000000000.ToString());
				bn128_10000000000000000.rand();
				bn128_10000000000000000 = bn64_100000000 * bn64_100000000;
				Assert::AreEqual(std::wstring(L"0x00000000000000010000000000000000"), bn128_10000000000000000.ToString());

				//

				TestMethod_jrmwng_big_number_unsigned_operate(std::tie(bn32_1, bn64_1, bn96_1), std::tie(bn32_1, bn64_1, bn96_1),
					std::make_tuple(
						std::tie(bn32, std::wstring(L"0x00000002")),
						std::tie(bn64, std::wstring(L"0x0000000000000002")),
						std::tie(bn96, std::wstring(L"0x000000000000000000000002"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA + bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB + bnA; }
					)
				);
				TestMethod_jrmwng_big_number_unsigned_operate(std::tie(bn32_1, bn64_1, bn96_1), std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF),
					std::make_tuple(
						std::tie(bn64, std::wstring(L"0x0000000100000000")),
						std::tie(bn96, std::wstring(L"0x000000000000000100000000"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA + bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB + bnA; }
					)
				);
				TestMethod_jrmwng_big_number_unsigned_operate(std::tie(bn32_1, bn64_1, bn96_1), std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF),
					std::make_tuple(
						std::tie(bn32, std::wstring(L"0xFFFFFFFF")),
						std::tie(bn64, std::wstring(L"0x00000000FFFFFFFF")),
						std::tie(bn96, std::wstring(L"0x0000000000000000FFFFFFFF"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA * bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB * bnA; }
					)
				);

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