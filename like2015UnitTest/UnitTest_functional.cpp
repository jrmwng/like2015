#include "stdafx.h"
#include "CppUnitTest.h"

#include <algorithm>
#include "..\like2015\functional.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace like2015UnitTest
{		
	TEST_CLASS(like2015UnitTest_functional)
	{
		template <typename TIterator1, typename TIterator2, typename T, typename TBinaryPredicate>
		void test_binary_predicate(TIterator1 itFirst1, TIterator1 itLast1, TIterator2 itFirst2, TIterator2 itLast2, T t, TBinaryPredicate tPred)
		{
			std::adjacent_find(itFirst1, itLast1, tPred);
			std::binary_search(itFirst1, itLast1, t, tPred);
			std::equal(itFirst1, itLast1, itFirst2, tPred);
			std::equal_range(itFirst1, itLast1, t, tPred);
			std::find_end(itFirst1, itLast1, itFirst2, itLast2, tPred);
			std::find_first_of(itFirst1, itLast1, itFirst2, itLast2, tPred);
			std::includes(itFirst1, itLast1, itFirst2, itLast2, tPred);
			std::inplace_merge(itFirst1, itLast1, itLast1, tPred);
			std::is_heap(itFirst1, itLast1, tPred);
			std::is_heap_until(itFirst1, itLast1, tPred);
			std::is_permutation(itFirst1, itLast1, itFirst2, itLast2, tPred);
			std::is_sorted(itFirst1, itLast1, tPred);
			std::is_sorted_until(itFirst1, itLast1, tPred);
			std::lexicographical_compare(itFirst1, itLast1, itFirst2, itLast2, tPred);
			std::lower_bound(itFirst1, itLast1, t, tPred);
			std::make_heap(itFirst1, itLast1, tPred);
			std::max(t, t, tPred);
			std::max({ t,t,t }, tPred);
			std::max_element(itFirst1, itLast1, tPred);
			//std::merge(itFirst1, itLast1, itFirst2, itLast2, itFirst1, tPred);
			std::min(t, t, tPred);
			std::min({ t,t,t }, tPred);
			std::min_element(itFirst1, itLast1, tPred);
			std::minmax(t, t, tPred);
			std::minmax({ t,t,t }, tPred);
			std::minmax_element(itFirst1, itLast1, tPred);
			std::mismatch(itFirst1, itLast1, itFirst2, tPred);
			std::next_permutation(itFirst1, itLast1, tPred);
			std::nth_element(itFirst1, itLast1, itLast1, tPred);
			std::partial_sort(itFirst1, itLast1, itLast1, tPred);
			std::partial_sort_copy(itFirst2, itLast2, itFirst1, itLast1, tPred);
			std::pop_heap(itFirst1, itLast1, tPred);
			std::prev_permutation(itFirst1, itLast1, tPred);
			std::push_heap(itFirst1, itLast1, tPred);
			std::search(itFirst1, itLast1, itFirst2, itLast2, tPred);
			std::search_n(itFirst1, itLast1, 1, t, tPred);
			std::set_difference(itFirst1, itLast1, itFirst2, itLast2, itFirst1, tPred);
			std::set_intersection(itFirst1, itLast1, itFirst2, itLast2, itFirst1, tPred);
			std::set_symmetric_difference(itFirst1, itLast1, itFirst2, itLast2, itFirst1, tPred);
			std::set_union(itFirst1, itLast1, itFirst2, itLast2, itFirst1, tPred);
			std::sort(itFirst1, itLast1, tPred);
			std::sort_heap(itFirst1, itLast1, tPred);
			std::stable_sort(itFirst1, itLast1, tPred);
			std::unique(itFirst1, itLast1, tPred);
			std::unique_copy(itFirst2, itLast2, itFirst1, tPred);
			std::upper_bound(itFirst1, itLast1, t, tPred);
		}

		int m_nDummy;

		struct SDummy
		{
			int nDummy;

			int & dummy(void) { return nDummy; }
			int const & const_dummy(void) const { return nDummy; }
			int volatile & volatile_dummy(void) volatile { return nDummy; }
			int const volatile & cv_dummy(void) const volatile { return nDummy; }
		} m_stDummy, m_astDummy[2];

		int & dummy(void) { return m_nDummy; }
		int const & const_dummy(void) const { return m_nDummy; }
		int volatile & volatile_dummy(void) volatile { return m_nDummy; }
		int const volatile & cv_dummy(void) const volatile { return m_nDummy; }

		template <typename TM>
		void test_member(TM tM)
		{
			m_stDummy.nDummy = ++m_nDummy;
			Assert::IsTrue(tM(*this) == m_nDummy);
			Assert::IsTrue(tM(this) == m_nDummy);
		}
	public:
		TEST_METHOD(compile_functional)
		{
			m_nDummy = 1;

			test_member(like::member<int>(&like2015UnitTest_functional::m_nDummy));
			test_member(like::member<int>(&like2015UnitTest_functional::m_stDummy, &SDummy::nDummy));
			test_member(like::member<int>(&like2015UnitTest_functional::dummy));
			test_member(like::member<int>(&like2015UnitTest_functional::const_dummy));
			test_member(like::member<int>(&like2015UnitTest_functional::volatile_dummy));
			test_member(like::member<int>(&like2015UnitTest_functional::cv_dummy));
			test_member(like::member<int>(&like2015UnitTest_functional::m_stDummy, &SDummy::dummy));
			test_member(like::member<int>(&like2015UnitTest_functional::m_stDummy, &SDummy::const_dummy));
			test_member(like::member<int>(&like2015UnitTest_functional::m_stDummy, &SDummy::volatile_dummy));
			test_member(like::member<int>(&like2015UnitTest_functional::m_stDummy, &SDummy::cv_dummy));

			test_member(like::member(&like2015UnitTest_functional::m_nDummy));
			test_member(like::member(&like2015UnitTest_functional::m_stDummy, &SDummy::nDummy));
			test_member(like::member(&like2015UnitTest_functional::dummy));
			test_member(like::member(&like2015UnitTest_functional::const_dummy));
			test_member(like::member(&like2015UnitTest_functional::volatile_dummy));
			test_member(like::member(&like2015UnitTest_functional::cv_dummy));
			test_member(like::member(&like2015UnitTest_functional::m_stDummy, &SDummy::dummy));
			test_member(like::member(&like2015UnitTest_functional::m_stDummy, &SDummy::const_dummy));
			test_member(like::member(&like2015UnitTest_functional::m_stDummy, &SDummy::volatile_dummy));
			test_member(like::member(&like2015UnitTest_functional::m_stDummy, &SDummy::cv_dummy));

			test_binary_predicate(std::begin(m_astDummy), std::end(m_astDummy), std::cbegin(m_astDummy), std::cend(m_astDummy), 0, like::order_by(like::member<int>(&SDummy::nDummy)));
			test_binary_predicate(std::begin(m_astDummy), std::end(m_astDummy), std::cbegin(m_astDummy), std::cend(m_astDummy), 0, like::order_by(like::member<int>(&SDummy::nDummy), std::greater<int>()));
			//test_binary_predicate(std::begin(m_astDummy), std::end(m_astDummy), std::cbegin(m_astDummy), std::cend(m_astDummy), 0, like::order_by(like::member(&SDummy::nDummy)));
			//test_binary_predicate(std::begin(m_astDummy), std::end(m_astDummy), std::cbegin(m_astDummy), std::cend(m_astDummy), 0, like::order_by(like::member(&SDummy::nDummy), std::greater<int>()));
			test_binary_predicate(std::begin(m_astDummy), std::end(m_astDummy), std::cbegin(m_astDummy), std::cend(m_astDummy), 0, like::order_by(&SDummy::nDummy));
			test_binary_predicate(std::begin(m_astDummy), std::end(m_astDummy), std::cbegin(m_astDummy), std::cend(m_astDummy), 0, like::order_by(&SDummy::nDummy, std::greater<int>()));
		}
	};
}