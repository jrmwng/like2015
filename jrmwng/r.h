#pragma once
 
/* Author: jrmwng @ 2016 */

// R for random

#include <type_traits>
#include <functional>
#include <tuple>

#include "tuple.h"

namespace jrmwng
{
	struct Rexpr
	{};

	template <typename T>
	struct Rvar;

	template <typename T>
	using Rtype = std::conditional_t<(std::is_base_of<Rexpr, T>::value), T, std::conditional_t<(std::is_const<T>::value), Rvar<T>, Rvar<T &>>>;
	template <typename T>
	Rtype<T> r(T & t)
	{
		return Rtype<T>(t);
	}
	template <typename T>
	Rtype<T const> r(T const & t)
	{
		return Rtype<T const>(t);
	}

	template <typename T>
	struct Rvar
		: Rexpr
	{
		T m_t;

		Rvar(T t)
			: m_t(t)
		{}

		template <typename TR>
		TR eval() const
		{
			return static_cast<TR>(m_t);
		}

		template <typename T1, typename TEnableIf = std::enable_if_t<std::is_base_of<Rexpr, T1>::value>> Rvar(T1 t1)
			: m_t(t1.eval<std::decay_t<T>>())
		{}
		template <typename T1> Rvar & operator = (T1 t1)
		{
			m_t = r(t1).eval<std::decay_t<T>>();
			return *this;
		}
		template <typename T1> Rvar & operator += (T1 t1)
		{
			m_t += r(t1).eval<std::decay_t<T>>();
			return *this;
		}
		template <typename T1> Rvar & operator -= (T1 t1)
		{
			m_t -= r(t1).eval<std::decay_t<T>>();
			return *this;
		}
		template <typename T1> Rvar & operator *= (T1 t1)
		{
			m_t *= r(t1).eval<std::decay_t<T>>();
			return *this;
		}
		template <typename T1> Rvar & operator /= (T1 t1)
		{
			m_t /= r(t1).eval<std::decay_t<T>>();
			return *this;
		}
		template <typename T1> Rvar & operator %= (T1 t1)
		{
			m_t %= r(t1).eval<std::decay_t<T>>();
			return *this;
		}
		template <typename T1> Rvar & operator &= (T1 t1)
		{
			m_t &= r(t1).eval<std::decay_t<T>>();
			return *this;
		}
		template <typename T1> Rvar & operator |= (T1 t1)
		{
			m_t |= r(t1).eval<std::decay_t<T>>();
			return *this;
		}
		template <typename T1> Rvar & operator ^= (T1 t1)
		{
			m_t ^= r(t1).eval<std::decay_t<T>>();
			return *this;
		}
	};

	template <template <typename T> typename Top, typename... TArgs>
	struct Rop
		: std::tuple<TArgs...>
		, Rexpr
	{
		template <typename... TParams>
		Rop(TParams &&... tParams)
			: std::tuple<TArgs...>(std::forward<TParams>(tParams)...)
		{}

		template <typename TR>
		TR eval() const
		{
			return unpack(static_cast<std::tuple<TArgs...> const &>(*this), Top<TR>(), [](auto const &refExpr)->TR { return refExpr.eval<TR>(); });
		}
	};
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator + (T1 t1, T2 t2)
	{
		return Rop<std::plus, Rtype<T1>, Rtype<T2>>(t1, t2);
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator - (T1 t1, T2 t2)
	{
		return Rop<std::minus, Rtype<T1>, Rtype<T2>>(t1, t2);
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator * (T1 t1, T2 t2)
	{
		return Rop<std::multiplies, Rtype<T1>, Rtype<T2>>(t1, t2);
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator / (T1 t1, T2 t2)
	{
		return Rop<std::divides, Rtype<T1>, Rtype<T2>>(t1, t2);
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator % (T1 t1, T2 t2)
	{
		return Rop<std::modulus, Rtype<T1>, Rtype<T2>>(t1, t2);
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator & (T1 t1, T2 t2)
	{
		return Rop<std::bit_and, Rtype<T1>, Rtype<T2>>(t1, t2);
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator | (T1 t1, T2 t2)
	{
		return Rop<std::bit_or, Rtype<T1>, Rtype<T2>>(t1, t2);
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator ^ (T1 t1, T2 t2)
	{
		return Rop<std::bit_xor, Rtype<T1>, Rtype<T2>>(t1, t2);
	}
	template <typename T1, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value)>> auto operator ~ (T1 t1)
	{
		return Rop<std::bit_not, Rtype<T1>>(t1);
	}
}
