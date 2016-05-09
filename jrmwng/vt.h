#pragma once

/* Author: jrmwng @ 2016 */

// variadic templates

#include <type_traits>

namespace jrmwng
{
	template <typename T, T... vtElement>
	struct vt_set;

	template <typename T, T tTest, T... vtSet>
	struct vt_set_exists;
	template <typename T, T tTest, T tElement, T... vtSet>
	struct vt_set_exists<T, tTest, tElement, vtSet...>
	{
		typedef std::conditional_t<tTest == tElement, std::true_type, typename vt_set_exists<T, tTest, vtSet...>::type> type;
	};
	template <typename T, T tTest>
	struct vt_set_exists<T, tTest>
	{
		typedef std::false_type type;
	};

	template <typename T, T tTest, T... vtSet>
	struct vt_set_rindex_of;
	template <typename T, T tTest, T tElement, T... vtSet>
	struct vt_set_rindex_of<T, tTest, tElement, vtSet...>
	{
		typedef std::conditional_t<tTest == tElement, std::integral_constant<int, sizeof...(vtSet)>, typename vt_set_rindex_of<T, tTest, vtSet...>::type> type;
	};
	template <typename T, T tTest>
	struct vt_set_rindex_of<T, tTest>
		: std::integral_constant<int, -1>
	{};

	template <size_t szCount, typename T, T... vtSet>
	struct vt_set_uniqueset;
	template <size_t szCount, typename T, T tElement, T... vtSet>
	struct vt_set_uniqueset<szCount, T, tElement, vtSet...>
	{
		typedef std::conditional_t<vt_set_exists<T, tElement, vtSet...>::type::value, typename vt_set_uniqueset<szCount - 1, T, vtSet...>::type, typename vt_set_uniqueset<szCount - 1, T, vtSet..., tElement>::type> type;
	};
	template <typename T, T tElement, T... vtSet>
	struct vt_set_uniqueset<0, T, tElement, vtSet...>
	{
		typedef vt_set<T, tElement, vtSet...> type;
	};
	template <size_t szCount, typename T>
	struct vt_set_uniqueset<szCount, T>
	{
		typedef vt_set<T> type;
	};
	//template <typename T, template <T... vtSetA> vt_set<T, vtSetA...>, T tElementB, T... vtSetB>
	//struct vt_set_unionset_t;
	//template <typename T, template <T... vtSetA> vt_set<T, vtSetA...>, T tElementB, T... vtSetB>
	//struct vt_set_unionset_t
	//	: std::conditional_t<vt_set_exists_t<T, tElementB, vtSetA...>::value, vt_set_unionset_t<T, vt_set<T, vtSetA...>, vtSetB...>, vt_set_unionset_t<T, vt_set<T, vtSetA..., tElementB>, vtSetB...> >
	//{};
	//template <typename T, template <T... vtSetA> vt_set<T, vtSetA...> >
	//struct vt_set_unionset_t
	//	: vt_set<T, vtSetA...>
	//{};

	template <size_t szCountA, size_t szCountB, typename T, T... vtSetAB>
	struct vt_set_intersectionset;
	template <size_t szCountA, size_t szCountB, typename T, T tElementA, T... vtSetABC>
	struct vt_set_intersectionset<szCountA, szCountB, T, tElementA, vtSetABC...>
	{
		typedef std::conditional_t<(1 + sizeof...(vtSetABC) - (szCountA + szCountB) < 1 + vt_set_rindex_of<T, tElementA, vtSetABC...>::type::value && vt_set_rindex_of<T, tElementA, vtSetABC...>::type::value <= sizeof...(vtSetABC) -szCountA), typename vt_set_intersectionset<szCountA - 1, szCountB, T, vtSetABC..., tElementA>::type, typename vt_set_intersectionset<szCountA - 1, szCountB, T, vtSetABC...>::type> type;
	};
	template <size_t szCountB, typename T, T tElementB, T... vtSetBC>
	struct vt_set_intersectionset<0, szCountB, T, tElementB, vtSetBC...>
	{
		typedef typename vt_set_intersectionset<0, szCountB - 1, T, vtSetBC...>::type type;
	};
	template <typename T, T... vtSetC>
	struct vt_set_intersectionset<0, 0, T, vtSetC...>
	{
		typedef typename vt_set_uniqueset<sizeof...(vtSetC), T, vtSetC...>::type type;
	};

	template <typename T>
	struct vt_set<T>
	{
		using subset_t = vt_set<T>;

		template <T tTest>
		struct exists
		{
			typedef std::false_type type;
		};
		template <T tTest>
		using exists_t = typename exists<tTest>::type;

		template <T... vtThatSet>
		struct unionset
		{
			typedef vt_set<T, vtThatSet...> type;
		};
		template <T... vtThatSet>
		using unionset_t = typename unionset<vtThatSet...>::type;

		template <T... vtThatSet>
		struct intersectionset
		{
			typedef vt_set<T> type;
		};
		template <T... vtThatSet>
		using intersectionset_t = typename intersectionset<vtThatSet...>::type;
	};
	template <typename T, T tElement, T... vtSubSet>
	struct vt_set<T, tElement, vtSubSet...>
	{
		using subset_t = vt_set<T, vtSubSet...>;

		template <T tTest>
		struct exists
		{
			typedef typename vt_set_exists<T, tTest, tElement, vtSubSet...>::type type;
		};
		template <T tTest>
		using exists_t = typename exists<tTest>::type;

		template <T... vtThatSet>
		struct unionset
		{
			typedef typename vt_set_uniqueset<1 + sizeof...(vtSubSet) +sizeof...(vtThatSet), T, tElement, vtSubSet..., vtThatSet...>::type type;
		};
		template <T... vtThatSet>
		using unionset_t = typename unionset<vtThatSet...>::type;

		template <T... vtThatSet>
		struct intersectionset
		{
			typedef typename vt_set_intersectionset<1 + sizeof...(vtSubSet), sizeof...(vtThatSet), T, tElement, vtSubSet..., vtThatSet...>::type type;
		};
		template <T... vtThatSet>
		using intersectionset_t = typename intersectionset<vtThatSet...>::type;

		static_assert(vt_set_exists<T, tElement, vtSubSet...>::type::value == false, "");
	};
}