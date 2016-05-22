#pragma once

/* Author: jrmwng @ 2016 */

#include <tuple>

namespace jrmwng
{
	template <typename T, unsigned uBitCount>
	struct big_number;
	template <typename T>
	struct big_number<T, 0>
	{};

	struct big_number_expr
	{};
	struct big_number_op
		: big_number_expr
	{};

	template <typename TExpr>
	using big_number_expr_t = std::conditional_t<(std::is_base_of<big_number_op, TExpr>::value), TExpr, std::add_lvalue_reference_t< std::add_const_t<TExpr> > >;

	template <typename TLeft, typename TRight>
	struct big_number_add
		: std::tuple<TLeft, TRight>
		, big_number_op
	{
		big_number_add(TLeft tLeft, TRight tRight)
			: std::tuple<TLeft, TRight>(tLeft, tRight)
		{}
	};
	template <typename TLeft, typename TRight, typename TEnableIf = std::enable_if_t<std::is_base_of<big_number_expr, TLeft>::value && std::is_base_of<big_number_expr, TRight>::value>>
	decltype(auto) operator + (TLeft const & tLeft, TRight const & tRight)
	{
		return big_number_add< big_number_expr_t<TLeft>, big_number_expr_t<TRight> >(tLeft, tRight);
	}

	template <typename TLeft, typename TRight>
	struct big_number_sub
		: std::tuple<TLeft, TRight>
		, big_number_op
	{
		big_number_sub(TLeft tLeft, TRight tRight)
			: std::tuple<TLeft, TRight>(tLeft, tRight)
		{}
	};
	template <typename TLeft, typename TRight, typename TEnableIf = std::enable_if_t<std::is_base_of<big_number_expr, TLeft>::value && std::is_base_of<big_number_expr, TRight>::value>>
	decltype(auto) operator - (TLeft const & tLeft, TRight const & tRight)
	{
		return big_number_sub< big_number_expr_t<TLeft>, big_number_expr_t<TRight> >(tLeft, tRight);
	}

	template <typename TLeft, typename TRight>
	struct big_number_mul
		: std::tuple<TLeft, TRight>
		, big_number_op
	{
		big_number_mul(TLeft tLeft, TRight tRight)
			: std::tuple<TLeft, TRight>(tLeft, tRight)
		{}
	};
	template <typename TLeft, typename TRight, typename TEnableIf = std::enable_if_t<std::is_base_of<big_number_expr, TLeft>::value && std::is_base_of<big_number_expr, TRight>::value>>
	decltype(auto) operator * (TLeft const & tLeft, TRight const & tRight)
	{
		return big_number_mul< big_number_expr_t<TLeft>, big_number_expr_t<TRight> >(tLeft, tRight);
	}

	template <typename TLeft, typename TRight>
	struct big_number_div
		: std::tuple<TLeft, TRight>
		, big_number_op
	{
		big_number_div(TLeft tLeft, TRight tRight)
			: std::tuple<TLeft, TRight>(tLeft, tRight)
		{}
	};
	template <typename TLeft, typename TRight, typename TEnableIf = std::enable_if_t<std::is_base_of<big_number_expr, TLeft>::value && std::is_base_of<big_number_expr, TRight>::value>>
	decltype(auto) operator / (TLeft const & tLeft, TRight const & tRight)
	{
		return big_number_div< big_number_expr_t<TLeft>, big_number_expr_t<TRight> >(tLeft, tRight);
	}

	template <typename TLeft, typename TRight>
	struct big_number_mod
		: std::tuple<TLeft, TRight>
		, big_number_op
	{
		big_number_mod(TLeft tLeft, TRight tRight)
			: std::tuple<TLeft, TRight>(tLeft, tRight)
		{}
	};
	template <typename TLeft, typename TRight, typename TEnableIf = std::enable_if_t<std::is_base_of<big_number_expr, TLeft>::value && std::is_base_of<big_number_expr, TRight>::value>>
	decltype(auto) operator % (TLeft const & tLeft, TRight const & tRight)
	{
		return big_number_mod< big_number_expr_t<TLeft>, big_number_expr_t<TRight> >(tLeft, tRight);
	}
}
