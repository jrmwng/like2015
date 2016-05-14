#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\jrmwng\vt_set.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_jrmwng
{		
	TEST_CLASS(UnitTest_jrmwng_vt_set)
	{
	public:
		TEST_METHOD(TestMethod_jrmwng_vt_set)
		{
			Assert::IsFalse(jrmwng::vt_set<int>::exists_t<0>::value);
			Assert::IsFalse(jrmwng::vt_set<int, 1>::exists_t<0>::value);
			Assert::IsFalse(jrmwng::vt_set<int, 1, 2>::exists_t<0>::value);

			Assert::IsTrue(jrmwng::vt_set<int, 0>::exists_t<0>::value);
			Assert::IsTrue(jrmwng::vt_set<int, 0, 1>::exists_t<0>::value);
			Assert::IsTrue(jrmwng::vt_set<int, 1, 0>::exists_t<0>::value);

			Assert::IsFalse(std::is_same<jrmwng::vt_set<int>, jrmwng::vt_set<int, 0>>::value);
			Assert::IsFalse(std::is_same<jrmwng::vt_set<int>, jrmwng::vt_set<int, 0, 1>>::value);

			Assert::IsTrue(std::is_same<jrmwng::vt_set<int>, jrmwng::vt_set<int>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int, 0>, jrmwng::vt_set<int, 0>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int, 0, 1>, jrmwng::vt_set<int, 0, 1>>::value);

			Assert::IsTrue(std::is_same<jrmwng::vt_set<int>, jrmwng::vt_set<int>::unionset_t<>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int, 0>, jrmwng::vt_set<int>::unionset_t<0>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int, 0>, jrmwng::vt_set<int, 0>::unionset_t<>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int, 0>, jrmwng::vt_set<int, 0>::unionset_t<0>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int, 0, 1>, jrmwng::vt_set<int>::unionset_t<0, 1>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int, 0, 1>, jrmwng::vt_set<int, 0>::unionset_t<1>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int, 0, 1>, jrmwng::vt_set<int, 0, 1>::unionset_t<>>::value);

			Assert::IsTrue(std::is_same<jrmwng::vt_set<int>, jrmwng::vt_set<int>::intersectionset_t<>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int>, jrmwng::vt_set<int>::intersectionset_t<0>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int>, jrmwng::vt_set<int>::intersectionset_t<0, 1>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int>, jrmwng::vt_set<int, 0>::intersectionset_t<>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int>, jrmwng::vt_set<int, 0, 1>::intersectionset_t<>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int>, jrmwng::vt_set<int, 0>::intersectionset_t<1>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int>, jrmwng::vt_set<int, 0, 1>::intersectionset_t<2, 3>>::value);

			Assert::IsTrue(std::is_same<jrmwng::vt_set<int, 1>, jrmwng::vt_set<int, 1>::intersectionset_t<1>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int, 1>, jrmwng::vt_set<int, 1>::intersectionset_t<1, 2>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int, 1>, jrmwng::vt_set<int, 0, 1>::intersectionset_t<1, 2>>::value);
			Assert::IsTrue(std::is_same<jrmwng::vt_set<int, 1, 2>, jrmwng::vt_set<int, 1, 2>::intersectionset_t<1, 2>>::value);
		}
	};
}