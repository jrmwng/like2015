#pragma once

/* Author: jrmwng @ 2016 */

#include <tuple>
#include <functional>

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

	template <template <typename T> class Top, typename... Targs>
	struct big_number_operation
		: std::tuple<Targs...>
		, big_number_op
	{
		big_number_operation(Targs... tArgs)
			: std::tuple<Targs...>((tArgs)...)
		{}
	};

	template <typename T1, typename T2>
	using big_number_add = big_number_operation<std::plus, T1, T2>;
	template <typename T1, typename T2>
	using big_number_sub = big_number_operation<std::minus, T1, T2>;
	template <typename T1, typename T2>
	using big_number_mul = big_number_operation<std::multiplies, T1, T2>;
	template <typename T1, typename T2>
	using big_number_div = big_number_operation<std::divides, T1, T2>;
	template <typename T1, typename T2>
	using big_number_mod = big_number_operation<std::modulus, T1, T2>;

	template <typename TLeft, typename TRight, typename TEnableIf = std::enable_if_t<std::is_base_of<big_number_expr, TLeft>::value && std::is_base_of<big_number_expr, TRight>::value>>
	decltype(auto) operator + (TLeft const & tLeft, TRight const & tRight)
	{
		return big_number_add< big_number_expr_t<TLeft>, big_number_expr_t<TRight> >(tLeft, tRight);
	}
	template <typename TLeft, typename TRight, typename TEnableIf = std::enable_if_t<std::is_base_of<big_number_expr, TLeft>::value && std::is_base_of<big_number_expr, TRight>::value>>
	decltype(auto) operator - (TLeft const & tLeft, TRight const & tRight)
	{
		return big_number_sub< big_number_expr_t<TLeft>, big_number_expr_t<TRight> >(tLeft, tRight);
	}
	template <typename TLeft, typename TRight, typename TEnableIf = std::enable_if_t<std::is_base_of<big_number_expr, TLeft>::value && std::is_base_of<big_number_expr, TRight>::value>>
	decltype(auto) operator * (TLeft const & tLeft, TRight const & tRight)
	{
		return big_number_mul< big_number_expr_t<TLeft>, big_number_expr_t<TRight> >(tLeft, tRight);
	}
	template <typename TLeft, typename TRight, typename TEnableIf = std::enable_if_t<std::is_base_of<big_number_expr, TLeft>::value && std::is_base_of<big_number_expr, TRight>::value>>
	decltype(auto) operator / (TLeft const & tLeft, TRight const & tRight)
	{
		return big_number_div< big_number_expr_t<TLeft>, big_number_expr_t<TRight> >(tLeft, tRight);
	}
	template <typename TLeft, typename TRight, typename TEnableIf = std::enable_if_t<std::is_base_of<big_number_expr, TLeft>::value && std::is_base_of<big_number_expr, TRight>::value>>
	decltype(auto) operator % (TLeft const & tLeft, TRight const & tRight)
	{
		return big_number_mod< big_number_expr_t<TLeft>, big_number_expr_t<TRight> >(tLeft, tRight);
	}
}
