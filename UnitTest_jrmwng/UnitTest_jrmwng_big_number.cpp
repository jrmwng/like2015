#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\jrmwng\tuple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework
		{
#if 1
			//template <unsigned uBitCount> inline std::wstring ToString(jrmwng::big_uint32<uBitCount> const & bn) { return jrmwng::ToString(bn); }
#else
			template <> inline std::wstring ToString<jrmwng::big_uint32<32>>(jrmwng::big_uint32<32> const & bn) { return bn.ToString(); }
			template <> inline std::wstring ToString<jrmwng::big_uint32<64>>(jrmwng::big_uint32<64> const & bn) { return bn.ToString(); }
			template <> inline std::wstring ToString<jrmwng::big_uint32<96>>(jrmwng::big_uint32<96> const & bn) { return bn.ToString(); }
			template <> inline std::wstring ToString<jrmwng::big_uint32<128>>(jrmwng::big_uint32<128> const & bn) { return bn.ToString(); }
			template <> inline std::wstring ToString<jrmwng::big_uint32<160>>(jrmwng::big_uint32<160> const & bn) { return bn.ToString(); }
#endif
		}
	}
}

namespace UnitTest_jrmwng
{		
	TEST_CLASS(UnitTest_jrmwng_big_number)
	{
		template <typename TInput1, typename TInput2, typename TOutcome3, typename TFunc4>
		void TestMethod_jrmwng_big_unsigned_operate(TInput1 tupleA, TInput2 tupleB, TOutcome3 tupleCD, TFunc4 tupleE)
		{
			jrmwng::for_each(tupleA, [&](auto const & bnA)
			{
				jrmwng::for_each(tupleB, [&](auto const & bnB)
				{
					jrmwng::for_each(tupleCD, [&](auto const & elemCD)
					{
						auto & bnC = std::get<0>(elemCD);
						auto const & strD = std::get<1>(elemCD);
						jrmwng::for_each(tupleE, [&](auto const & fnE)
						{
							bnC.rand();
							bnC = fnE(bnA, bnB);
							if (!(bnC == strD))
							{
								Assert::AreEqual(std::wstring(strD), jrmwng::ToString(bnC));
							}
						});
					});
				});
			});
		}
	public:
		TEST_METHOD(TestMethod_jrmwng_big_unsigned)
		{
			jrmwng::big_uint32<160> bn160_10000000000000000;
			jrmwng::big_uint32<128> bn128_10000000000000000;
			jrmwng::big_uint32< 96>  bn96_10000000000000000;
			jrmwng::big_uint32<160> bn160_FFFFFFFFFFFFFFFF;
			jrmwng::big_uint32<128> bn128_FFFFFFFFFFFFFFFF;
			jrmwng::big_uint32< 96>  bn96_FFFFFFFFFFFFFFFF;
			jrmwng::big_uint32< 64>  bn64_FFFFFFFFFFFFFFFF;
			jrmwng::big_uint32<160> bn160_100000000;
			jrmwng::big_uint32<128> bn128_100000000;
			jrmwng::big_uint32< 96>  bn96_100000000;
			jrmwng::big_uint32< 64>  bn64_100000000;
			jrmwng::big_uint32<160> bn160_FFFFFFFF;
			jrmwng::big_uint32<128> bn128_FFFFFFFF;
			jrmwng::big_uint32< 96>  bn96_FFFFFFFF;
			jrmwng::big_uint32< 64>  bn64_FFFFFFFF;
			jrmwng::big_uint32< 32>  bn32_FFFFFFFF;
			jrmwng::big_uint32<160> bn160_1;
			jrmwng::big_uint32<128> bn128_1;
			jrmwng::big_uint32< 96>  bn96_1;
			jrmwng::big_uint32< 64>  bn64_1;
			jrmwng::big_uint32< 32>  bn32_1;
			jrmwng::big_uint32<160> bn160_0;
			jrmwng::big_uint32<128> bn128_0;
			jrmwng::big_uint32< 96>  bn96_0;
			jrmwng::big_uint32< 64>  bn64_0;
			jrmwng::big_uint32< 32>  bn32_0;
			jrmwng::big_uint32<160> bn160;
			jrmwng::big_uint32<128> bn128;
			jrmwng::big_uint32< 96>  bn96;
			jrmwng::big_uint32< 64>  bn64;
			jrmwng::big_uint32< 32>  bn32;
			{
				bn32_0.rand();
				bn32_0 = 0U;
				Assert::AreEqual(std::wstring(L"0x00000000"), jrmwng::ToString(bn32_0));
				Assert::IsTrue(bn32_0 == 0U);
				bn64_0.rand();
				bn64_0 = 0U;
				Assert::AreEqual(std::wstring(L"0x0000000000000000"), jrmwng::ToString(bn64_0));
				Assert::IsTrue(bn64_0 == 0U);
				bn96_0.rand();
				bn96_0 = 0U;
				Assert::AreEqual(std::wstring(L"0x000000000000000000000000"), jrmwng::ToString(bn96_0));
				Assert::IsTrue(bn96_0 == 0U);
				bn128_0.rand();
				bn128_0 = 0U;
				Assert::AreEqual(std::wstring(L"0x00000000000000000000000000000000"), jrmwng::ToString(bn128_0));
				Assert::IsTrue(bn128_0 == 0U);
				bn160_0.rand();
				bn160_0 = 0U;
				Assert::AreEqual(std::wstring(L"0x0000000000000000000000000000000000000000"), jrmwng::ToString(bn160_0));
				Assert::IsTrue(bn160_0 == 0U);

				bn32_1.rand();
				bn32_1 = 1U;
				Assert::AreEqual(std::wstring(L"0x00000001"), jrmwng::ToString(bn32_1));
				Assert::IsTrue(bn32_1 == 1U);
				bn64_1.rand();
				bn64_1 = 1U;
				Assert::AreEqual(std::wstring(L"0x0000000000000001"), jrmwng::ToString(bn64_1));
				Assert::IsTrue(bn64_1 == 1U);
				bn96_1.rand();
				bn96_1 = 1U;
				Assert::AreEqual(std::wstring(L"0x000000000000000000000001"), jrmwng::ToString(bn96_1));
				Assert::IsTrue(bn96_1 == 1U);
				bn128_1.rand();
				bn128_1 = 1U;
				Assert::AreEqual(std::wstring(L"0x00000000000000000000000000000001"), jrmwng::ToString(bn128_1));
				Assert::IsTrue(bn128_1 == 1U);
				bn160_1.rand();
				bn160_1 = 1U;
				Assert::AreEqual(std::wstring(L"0x0000000000000000000000000000000000000001"), jrmwng::ToString(bn160_1));
				Assert::IsTrue(bn160_1 == 1U);

				bn32_FFFFFFFF.rand();
				bn32_FFFFFFFF = 0xFFFFFFFFU;
				Assert::AreEqual(std::wstring(L"0xFFFFFFFF"), jrmwng::ToString(bn32_FFFFFFFF));
				Assert::IsTrue(bn32_FFFFFFFF == 0xFFFFFFFFU);
				bn64_FFFFFFFF.rand();
				bn64_FFFFFFFF = 0xFFFFFFFFU;
				Assert::AreEqual(std::wstring(L"0x00000000FFFFFFFF"), jrmwng::ToString(bn64_FFFFFFFF));
				Assert::IsTrue(bn64_FFFFFFFF == 0xFFFFFFFFU);
				bn96_FFFFFFFF.rand();
				bn96_FFFFFFFF = 0xFFFFFFFFU;
				Assert::AreEqual(std::wstring(L"0x0000000000000000FFFFFFFF"), jrmwng::ToString(bn96_FFFFFFFF));
				Assert::IsTrue(bn96_FFFFFFFF == 0xFFFFFFFFU);
				bn128_FFFFFFFF.rand();
				bn128_FFFFFFFF = 0xFFFFFFFFU;
				Assert::AreEqual(std::wstring(L"0x000000000000000000000000FFFFFFFF"), jrmwng::ToString(bn128_FFFFFFFF));
				Assert::IsTrue(bn128_FFFFFFFF == 0xFFFFFFFFU);
				bn160_FFFFFFFF.rand();
				bn160_FFFFFFFF = 0xFFFFFFFFU;
				Assert::AreEqual(std::wstring(L"0x00000000000000000000000000000000FFFFFFFF"), jrmwng::ToString(bn160_FFFFFFFF));
				Assert::IsTrue(bn160_FFFFFFFF == 0xFFFFFFFFU);

				bn64_100000000.rand();
				bn64_100000000 = 0x100000000ULL;
				Assert::AreEqual(std::wstring(L"0x0000000100000000"), jrmwng::ToString(bn64_100000000));
				Assert::IsTrue(bn64_100000000 == 0x100000000ULL);
				bn96_100000000.rand();
				bn96_100000000 = 0x100000000ULL;
				Assert::AreEqual(std::wstring(L"0x000000000000000100000000"), jrmwng::ToString(bn96_100000000));
				Assert::IsTrue(bn96_100000000 == 0x100000000ULL);
				bn128_100000000.rand();
				bn128_100000000 = 0x100000000ULL;
				Assert::AreEqual(std::wstring(L"0x00000000000000000000000100000000"), jrmwng::ToString(bn128_100000000));
				Assert::IsTrue(bn128_100000000 == 0x100000000ULL);
				bn160_100000000.rand();
				bn160_100000000 = 0x100000000ULL;
				Assert::AreEqual(std::wstring(L"0x0000000000000000000000000000000100000000"), jrmwng::ToString(bn160_100000000));
				Assert::IsTrue(bn160_100000000 == 0x100000000ULL);

				bn64_FFFFFFFFFFFFFFFF.rand();
				bn64_FFFFFFFFFFFFFFFF = 0xFFFFFFFFFFFFFFFFULL;
				Assert::AreEqual(std::wstring(L"0xFFFFFFFFFFFFFFFF"), jrmwng::ToString(bn64_FFFFFFFFFFFFFFFF));
				Assert::IsTrue(bn64_FFFFFFFFFFFFFFFF == 0xFFFFFFFFFFFFFFFFULL);
				bn96_FFFFFFFFFFFFFFFF.rand();
				bn96_FFFFFFFFFFFFFFFF = 0xFFFFFFFFFFFFFFFFULL;
				Assert::AreEqual(std::wstring(L"0x00000000FFFFFFFFFFFFFFFF"), jrmwng::ToString(bn96_FFFFFFFFFFFFFFFF));
				Assert::IsTrue(bn96_FFFFFFFFFFFFFFFF == 0xFFFFFFFFFFFFFFFFULL);
				bn128_FFFFFFFFFFFFFFFF.rand();
				bn128_FFFFFFFFFFFFFFFF = 0xFFFFFFFFFFFFFFFFULL;
				Assert::AreEqual(std::wstring(L"0x0000000000000000FFFFFFFFFFFFFFFF"), jrmwng::ToString(bn128_FFFFFFFFFFFFFFFF));
				Assert::IsTrue(bn128_FFFFFFFFFFFFFFFF == 0xFFFFFFFFFFFFFFFFULL);
				bn160_FFFFFFFFFFFFFFFF.rand();
				bn160_FFFFFFFFFFFFFFFF = 0xFFFFFFFFFFFFFFFFULL;
				Assert::AreEqual(std::wstring(L"0x000000000000000000000000FFFFFFFFFFFFFFFF"), jrmwng::ToString(bn160_FFFFFFFFFFFFFFFF));
				Assert::IsTrue(bn160_FFFFFFFFFFFFFFFF == 0xFFFFFFFFFFFFFFFFULL);

				bn96_10000000000000000.rand();
				bn96_10000000000000000 = bn64_100000000 * bn64_100000000;
				Assert::AreEqual(std::wstring(L"0x000000010000000000000000"), jrmwng::ToString(bn96_10000000000000000));
				bn128_10000000000000000.rand();
				bn128_10000000000000000 = bn64_100000000 * bn64_100000000;
				Assert::AreEqual(std::wstring(L"0x00000000000000010000000000000000"), jrmwng::ToString(bn128_10000000000000000));
				bn160_10000000000000000.rand();
				bn160_10000000000000000 = bn64_100000000 * bn64_100000000;
				Assert::AreEqual(std::wstring(L"0x0000000000000000000000010000000000000000"), jrmwng::ToString(bn160_10000000000000000));

				jrmwng::big_uint32<32> bn32_P(17U);
				jrmwng::big_uint32<32> bn32_Q(11U);
				jrmwng::big_uint32<64> bn64_N = bn32_P * bn32_Q;
				jrmwng::big_uint32<32> bn32_p = bn32_P - bn32_1;
				jrmwng::big_uint32<32> bn32_q = bn32_Q - bn32_1;
				jrmwng::big_uint32<64> bn64_M = (bn32_P - bn32_1) * (bn32_Q - bn32_1);

				//
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn32_0, bn64_0, bn96_0, bn128_0, bn160_0),
					std::tie(bn32_0, bn64_0, bn96_0, bn128_0, bn160_0),
					std::make_tuple(
						std::tie(bn32 , (L"0x00000000")),
						std::tie(bn64 , (L"0x0000000000000000")),
						std::tie(bn96 , (L"0x000000000000000000000000")),
						std::tie(bn128, (L"0x00000000000000000000000000000000")),
						std::tie(bn160, (L"0x0000000000000000000000000000000000000000"))
						),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA + bnB; },
						[](auto const & bnA, auto const & bnB) { return bnA - bnB; },
						[](auto const & bnA, auto const & bnB) { return bnA * bnB; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1),
					std::tie(bn32_0, bn64_0, bn96_0, bn128_0, bn160_0),
					std::make_tuple(
						std::tie(bn32, (L"0x00000001")),
						std::tie(bn64, (L"0x0000000000000001")),
						std::tie(bn96, (L"0x000000000000000000000001")),
						std::tie(bn128, (L"0x00000000000000000000000000000001")),
						std::tie(bn160, (L"0x0000000000000000000000000000000000000001"))
						),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA + bnB; },
						[](auto const & bnA, auto const & bnB) { return bnA - bnB; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1, bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF, bn128_FFFFFFFF, bn160_FFFFFFFF, bn64_100000000, bn96_100000000, bn128_100000000, bn160_100000000, bn96_10000000000000000, bn128_10000000000000000, bn160_10000000000000000),
					std::tie(bn32_0, bn64_0, bn96_0, bn128_0, bn160_0),
					std::make_tuple(
						std::tie(bn32 , (L"0x00000000")),
						std::tie(bn64 , (L"0x0000000000000000")),
						std::tie(bn96 , (L"0x000000000000000000000000")),
						std::tie(bn128, (L"0x00000000000000000000000000000000")),
						std::tie(bn160, (L"0x0000000000000000000000000000000000000000"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA * bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB * bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1),
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1),
					std::make_tuple(
						std::tie(bn32 , (L"0x00000002")),
						std::tie(bn64 , (L"0x0000000000000002")),
						std::tie(bn96 , (L"0x000000000000000000000002")),
						std::tie(bn128, (L"0x00000000000000000000000000000002")),
						std::tie(bn160, (L"0x0000000000000000000000000000000000000002"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA + bnB; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1),
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1),
					std::make_tuple(
						std::tie(bn32, (L"0x00000000")),
						std::tie(bn64, (L"0x0000000000000000")),
						std::tie(bn96, (L"0x000000000000000000000000")),
						std::tie(bn128, (L"0x00000000000000000000000000000000")),
						std::tie(bn160, (L"0x0000000000000000000000000000000000000000"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA - bnB; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1),
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1),
					std::make_tuple(
						std::tie(bn32, (L"0x00000001")),
						std::tie(bn64, (L"0x0000000000000001")),
						std::tie(bn96, (L"0x000000000000000000000001"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA * bnB; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF, bn128_FFFFFFFF, bn160_FFFFFFFF),
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1),
					std::make_tuple(
						std::tie(bn64 , (L"0x0000000100000000")),
						std::tie(bn96 , (L"0x000000000000000100000000")),
						std::tie(bn128, (L"0x00000000000000000000000100000000")),
						std::tie(bn160, (L"0x0000000000000000000000000000000100000000"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA + bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB + bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF, bn128_FFFFFFFF, bn160_FFFFFFFF),
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1),
					std::make_tuple(
						std::tie(bn32 , (L"0xFFFFFFFE")),
						std::tie(bn64 , (L"0x00000000FFFFFFFE")),
						std::tie(bn96 , (L"0x0000000000000000FFFFFFFE")),
						std::tie(bn128, (L"0x000000000000000000000000FFFFFFFE")),
						std::tie(bn160, (L"0x00000000000000000000000000000000FFFFFFFE"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA - bnB; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF, bn128_FFFFFFFF, bn160_FFFFFFFF),
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1),
					std::make_tuple(
						std::tie(bn32 , (L"0xFFFFFFFF")),
						std::tie(bn64 , (L"0x00000000FFFFFFFF")),
						std::tie(bn96 , (L"0x0000000000000000FFFFFFFF")),
						std::tie(bn128, (L"0x000000000000000000000000FFFFFFFF")),
						std::tie(bn160, (L"0x00000000000000000000000000000000FFFFFFFF"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA * bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB * bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF, bn128_FFFFFFFF, bn160_FFFFFFFF),
					std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF, bn128_FFFFFFFF, bn160_FFFFFFFF),
					std::make_tuple(
						std::tie(bn64 , (L"0x00000001FFFFFFFE")),
						std::tie(bn96 , (L"0x0000000000000001FFFFFFFE")),
						std::tie(bn128, (L"0x000000000000000000000001FFFFFFFE")),
						std::tie(bn160, (L"0x00000000000000000000000000000001FFFFFFFE"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA + bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB + bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF, bn128_FFFFFFFF, bn160_FFFFFFFF),
					std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF, bn128_FFFFFFFF, bn160_FFFFFFFF),
					std::make_tuple(
						std::tie(bn64 , (L"0x0000000000000000")),
						std::tie(bn96 , (L"0x000000000000000000000000")),
						std::tie(bn128, (L"0x00000000000000000000000000000000")),
						std::tie(bn160, (L"0x0000000000000000000000000000000000000000"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA - bnB; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF, bn128_FFFFFFFF, bn160_FFFFFFFF),
					std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF, bn128_FFFFFFFF, bn160_FFFFFFFF),
					std::make_tuple(
						std::tie(bn64 , (L"0xFFFFFFFE00000001")),
						std::tie(bn96 , (L"0x00000000FFFFFFFE00000001")),
						std::tie(bn128, (L"0x0000000000000000FFFFFFFE00000001")),
						std::tie(bn160, (L"0x000000000000000000000000FFFFFFFE00000001"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA * bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB * bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_100000000, bn96_100000000, bn128_100000000, bn160_100000000),
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1),
					std::make_tuple(
						std::tie(bn64 , (L"0x0000000100000001")),
						std::tie(bn96 , (L"0x000000000000000100000001")),
						std::tie(bn128, (L"0x00000000000000000000000100000001")),
						std::tie(bn160, (L"0x0000000000000000000000000000000100000001"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA + bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB + bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_100000000, bn96_100000000, bn128_100000000, bn160_100000000),
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1),
					std::make_tuple(
						std::tie(bn32 , (L"0xFFFFFFFF")),
						std::tie(bn64 , (L"0x00000000FFFFFFFF")),
						std::tie(bn96 , (L"0x0000000000000000FFFFFFFF")),
						std::tie(bn128, (L"0x000000000000000000000000FFFFFFFF")),
						std::tie(bn160, (L"0x00000000000000000000000000000000FFFFFFFF"))
					),
					std::make_tuple([](auto const & bnA, auto const & bnB) { return bnA - bnB; })
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_100000000, bn96_100000000, bn128_100000000, bn160_100000000),
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1),
					std::make_tuple(
						std::tie(bn64 , (L"0x0000000100000000")),
						std::tie(bn96 , (L"0x000000000000000100000000")),
						std::tie(bn128, (L"0x00000000000000000000000100000000")),
						std::tie(bn160, (L"0x0000000000000000000000000000000100000000"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA * bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB * bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_100000000, bn96_100000000, bn128_100000000, bn160_100000000),
					std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF, bn128_FFFFFFFF, bn160_FFFFFFFF),
					std::make_tuple(
						std::tie(bn64 , (L"0x00000001FFFFFFFF")),
						std::tie(bn96 , (L"0x0000000000000001FFFFFFFF")),
						std::tie(bn128, (L"0x000000000000000000000001FFFFFFFF")),
						std::tie(bn160, (L"0x00000000000000000000000000000001FFFFFFFF"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA + bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB + bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_100000000, bn96_100000000, bn128_100000000, bn160_100000000),
					std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF, bn128_FFFFFFFF, bn160_FFFFFFFF),
					std::make_tuple(
						std::tie(bn32 , (L"0x00000001")),
						std::tie(bn64 , (L"0x0000000000000001")),
						std::tie(bn96 , (L"0x000000000000000000000001")),
						std::tie(bn128, (L"0x00000000000000000000000000000001")),
						std::tie(bn160, (L"0x0000000000000000000000000000000000000001"))
					),
					std::make_tuple([](auto const & bnA, auto const & bnB) { return bnA - bnB; })
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_100000000, bn96_100000000, bn128_100000000, bn160_100000000),
					std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF, bn128_FFFFFFFF, bn160_FFFFFFFF),
					std::make_tuple(
						std::tie(bn64 , (L"0xFFFFFFFF00000000")),
						std::tie(bn96 , (L"0x00000000FFFFFFFF00000000")),
						std::tie(bn128, (L"0x0000000000000000FFFFFFFF00000000")),
						std::tie(bn160, (L"0x000000000000000000000000FFFFFFFF00000000"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA * bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB * bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_100000000, bn96_100000000, bn128_100000000, bn160_100000000),
					std::tie(bn64_100000000, bn96_100000000, bn128_100000000, bn160_100000000),
					std::make_tuple(
						std::tie(bn64 , (L"0x0000000200000000")),
						std::tie(bn96 , (L"0x000000000000000200000000")),
						std::tie(bn128, (L"0x00000000000000000000000200000000")),
						std::tie(bn160, (L"0x0000000000000000000000000000000200000000"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA + bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB + bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_100000000, bn96_100000000, bn128_100000000, bn160_100000000),
					std::tie(bn64_100000000, bn96_100000000, bn128_100000000, bn160_100000000),
					std::make_tuple(
						std::tie(bn96 , (L"0x000000010000000000000000")),
						std::tie(bn128, (L"0x00000000000000010000000000000000")),
						std::tie(bn160, (L"0x0000000000000000000000010000000000000000"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA * bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB * bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_FFFFFFFFFFFFFFFF, bn96_FFFFFFFFFFFFFFFF, bn128_FFFFFFFFFFFFFFFF, bn160_FFFFFFFFFFFFFFFF),
					std::tie(bn32_0, bn64_0, bn96_0, bn128_0, bn160_0),
					std::make_tuple(
						std::tie(bn64 , (L"0xFFFFFFFFFFFFFFFF")),
						std::tie(bn96 , (L"0x00000000FFFFFFFFFFFFFFFF")),
						std::tie(bn128, (L"0x0000000000000000FFFFFFFFFFFFFFFF")),
						std::tie(bn160, (L"0x000000000000000000000000FFFFFFFFFFFFFFFF"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA + bnB; },
						[](auto const & bnA, auto const & bnB) { return bnA - bnB; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_FFFFFFFFFFFFFFFF, bn96_FFFFFFFFFFFFFFFF, bn128_FFFFFFFFFFFFFFFF, bn160_FFFFFFFFFFFFFFFF),
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1),
					std::make_tuple(
						std::tie(bn96 , (L"0x000000010000000000000000")),
						std::tie(bn128, (L"0x00000000000000010000000000000000")),
						std::tie(bn160, (L"0x0000000000000000000000010000000000000000"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA + bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB + bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_FFFFFFFFFFFFFFFF, bn96_FFFFFFFFFFFFFFFF, bn128_FFFFFFFFFFFFFFFF, bn160_FFFFFFFFFFFFFFFF),
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1),
					std::make_tuple(
						std::tie(bn64 , (L"0xFFFFFFFFFFFFFFFE")),
						std::tie(bn96 , (L"0x00000000FFFFFFFFFFFFFFFE")),
						std::tie(bn128, (L"0x0000000000000000FFFFFFFFFFFFFFFE")),
						std::tie(bn160, (L"0x000000000000000000000000FFFFFFFFFFFFFFFE"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA - bnB; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_FFFFFFFFFFFFFFFF, bn96_FFFFFFFFFFFFFFFF, bn128_FFFFFFFFFFFFFFFF, bn160_FFFFFFFFFFFFFFFF),
					std::tie(bn32_1, bn64_1, bn96_1, bn128_1, bn160_1),
					std::make_tuple(
						std::tie(bn64 , (L"0xFFFFFFFFFFFFFFFF")),
						std::tie(bn96 , (L"0x00000000FFFFFFFFFFFFFFFF")),
						std::tie(bn128, (L"0x0000000000000000FFFFFFFFFFFFFFFF")),
						std::tie(bn160, (L"0x000000000000000000000000FFFFFFFFFFFFFFFF"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA * bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB * bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_FFFFFFFFFFFFFFFF, bn96_FFFFFFFFFFFFFFFF, bn128_FFFFFFFFFFFFFFFF, bn160_FFFFFFFFFFFFFFFF),
					std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF, bn128_FFFFFFFF, bn160_FFFFFFFF),
					std::make_tuple(
						std::tie(bn96 , (L"0x0000000100000000FFFFFFFE")),
						std::tie(bn128, (L"0x000000000000000100000000FFFFFFFE")),
						std::tie(bn160, (L"0x00000000000000000000000100000000FFFFFFFE"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA + bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB + bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_FFFFFFFFFFFFFFFF, bn96_FFFFFFFFFFFFFFFF, bn128_FFFFFFFFFFFFFFFF, bn160_FFFFFFFFFFFFFFFF),
					std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF, bn128_FFFFFFFF, bn160_FFFFFFFF),
					std::make_tuple(
						std::tie(bn64 , (L"0xFFFFFFFF00000000")),
						std::tie(bn96 , (L"0x00000000FFFFFFFF00000000")),
						std::tie(bn128, (L"0x0000000000000000FFFFFFFF00000000")),
						std::tie(bn160, (L"0x000000000000000000000000FFFFFFFF00000000"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA - bnB; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_FFFFFFFFFFFFFFFF, bn96_FFFFFFFFFFFFFFFF, bn128_FFFFFFFFFFFFFFFF, bn160_FFFFFFFFFFFFFFFF),
					std::tie(bn32_FFFFFFFF, bn64_FFFFFFFF, bn96_FFFFFFFF, bn128_FFFFFFFF, bn160_FFFFFFFF),
					std::make_tuple(
						std::tie(bn96 , (L"0xFFFFFFFEFFFFFFFF00000001")),
						std::tie(bn128, (L"0x00000000FFFFFFFEFFFFFFFF00000001")),
						std::tie(bn160, (L"0x0000000000000000FFFFFFFEFFFFFFFF00000001"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA * bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB * bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_FFFFFFFFFFFFFFFF, bn96_FFFFFFFFFFFFFFFF, bn128_FFFFFFFFFFFFFFFF, bn160_FFFFFFFFFFFFFFFF),
					std::tie(bn64_100000000, bn96_100000000, bn128_100000000, bn160_100000000),
					std::make_tuple(
						std::tie(bn96 , (L"0x0000000100000000FFFFFFFF")),
						std::tie(bn128, (L"0x000000000000000100000000FFFFFFFF")),
						std::tie(bn160, (L"0x00000000000000000000000100000000FFFFFFFF"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA + bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB + bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_FFFFFFFFFFFFFFFF, bn96_FFFFFFFFFFFFFFFF, bn128_FFFFFFFFFFFFFFFF, bn160_FFFFFFFFFFFFFFFF),
					std::tie(bn64_100000000, bn96_100000000, bn128_100000000, bn160_100000000),
					std::make_tuple(
						std::tie(bn64 , (L"0xFFFFFFFEFFFFFFFF")),
						std::tie(bn96 , (L"0x00000000FFFFFFFEFFFFFFFF")),
						std::tie(bn128, (L"0x0000000000000000FFFFFFFEFFFFFFFF")),
						std::tie(bn160, (L"0x000000000000000000000000FFFFFFFEFFFFFFFF"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA - bnB; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_FFFFFFFFFFFFFFFF, bn96_FFFFFFFFFFFFFFFF, bn128_FFFFFFFFFFFFFFFF, bn160_FFFFFFFFFFFFFFFF),
					std::tie(bn64_100000000, bn96_100000000, bn128_100000000, bn160_100000000),
					std::make_tuple(
						std::tie(bn96 , (L"0xFFFFFFFFFFFFFFFF00000000")),
						std::tie(bn128, (L"0x00000000FFFFFFFFFFFFFFFF00000000")),
						std::tie(bn160, (L"0x0000000000000000FFFFFFFFFFFFFFFF00000000"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA * bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB * bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_FFFFFFFFFFFFFFFF, bn96_FFFFFFFFFFFFFFFF, bn128_FFFFFFFFFFFFFFFF, bn160_FFFFFFFFFFFFFFFF),
					std::tie(bn64_FFFFFFFFFFFFFFFF, bn96_FFFFFFFFFFFFFFFF, bn128_FFFFFFFFFFFFFFFF, bn160_FFFFFFFFFFFFFFFF),
					std::make_tuple(
						std::tie(bn96 , (L"0x00000001FFFFFFFFFFFFFFFE")),
						std::tie(bn128, (L"0x0000000000000001FFFFFFFFFFFFFFFE")),
						std::tie(bn160, (L"0x000000000000000000000001FFFFFFFFFFFFFFFE"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA + bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB + bnA; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_FFFFFFFFFFFFFFFF, bn96_FFFFFFFFFFFFFFFF, bn128_FFFFFFFFFFFFFFFF, bn160_FFFFFFFFFFFFFFFF),
					std::tie(bn64_FFFFFFFFFFFFFFFF, bn96_FFFFFFFFFFFFFFFF, bn128_FFFFFFFFFFFFFFFF, bn160_FFFFFFFFFFFFFFFF),
					std::make_tuple(
						std::tie(bn32 , (L"0x00000000")),
						std::tie(bn64 , (L"0x0000000000000000")),
						std::tie(bn96 , (L"0x000000000000000000000000")),
						std::tie(bn128, (L"0x00000000000000000000000000000000")),
						std::tie(bn160, (L"0x0000000000000000000000000000000000000000"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA - bnB; }
					)
				);
				TestMethod_jrmwng_big_unsigned_operate(
					std::tie(bn64_FFFFFFFFFFFFFFFF, bn96_FFFFFFFFFFFFFFFF, bn128_FFFFFFFFFFFFFFFF, bn160_FFFFFFFFFFFFFFFF),
					std::tie(bn64_FFFFFFFFFFFFFFFF, bn96_FFFFFFFFFFFFFFFF, bn128_FFFFFFFFFFFFFFFF, bn160_FFFFFFFFFFFFFFFF),
					std::make_tuple(
						std::tie(bn128, (L"0xFFFFFFFFFFFFFFFE0000000000000001")),
						std::tie(bn160, (L"0x00000000FFFFFFFFFFFFFFFE0000000000000001"))
					),
					std::make_tuple(
						[](auto const & bnA, auto const & bnB) { return bnA * bnB; },
						[](auto const & bnA, auto const & bnB) { return bnB * bnA; }
					)
				);

				//

				Assert::AreEqual(bn32_0, jrmwng::big_uint32<32>(bn32_1 - bn32_1));
			}
		}
	};
}