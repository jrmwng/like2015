#pragma once
 
/* Author: jrmwng @ 2016 */

// R for random

#include <type_traits>
#include <tuple>

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

	enum r_op2_e
	{
		R_PLUS,
		R_MINUS,
		R_TIMES,
		R_DIVIDES,
		R_BITAND,
		R_BITOR,
		R_BITXOR
	};
	template <r_op2_e emOp2> struct Rop2Traits;
	template <> struct Rop2Traits<R_PLUS>
	{
		template <typename T> static T eval(T t1, T t2) { return t1 + t2; }
	};
	template <> struct Rop2Traits<R_MINUS>
	{
		template <typename T> static T eval(T t1, T t2) { return t1 - t2; }
	};
	template <> struct Rop2Traits<R_TIMES>
	{
		template <typename T> static T eval(T t1, T t2) { return t1 * t2; }
	};
	template <> struct Rop2Traits<R_DIVIDES>
	{
		template <typename T> static T eval(T t1, T t2) { return t1 / t2; }
	};
	template <> struct Rop2Traits<R_BITAND>
	{
		template <typename T> static T eval(T t1, T t2) { return t1 & t2; }
	};
	template <> struct Rop2Traits<R_BITOR>
	{
		template <typename T> static T eval(T t1, T t2) { return t1 | t2; }
	};
	template <> struct Rop2Traits<R_BITXOR>
	{
		template <typename T> static T eval(T t1, T t2) { return t1 ^ t2; }
	};
	template <typename T1, typename T2, r_op2_e emOp2>
	struct Rop2
		: std::tuple<T1, T2>
		, Rexpr
	{
		Rop2(T1 t1, T2 t2)
			: std::tuple<T1, T2>(t1, t2)
		{}

		template <typename TR>
		TR eval() const
		{
			return Rop2Traits<emOp2>::eval(std::get<0>(*this).eval<TR>(), std::get<1>(*this).eval<TR>());
		}
	};
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator + (T1 t1, T2 t2)
	{
		return Rop2<Rtype<T1>, Rtype<T2>, R_PLUS>(t1, t2);
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator - (T1 t1, T2 t2)
	{
		return Rop2<Rtype<T1>, Rtype<T2>, R_MINUS>(t1, t2);
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator * (T1 t1, T2 t2)
	{
		return Rop2<Rtype<T1>, Rtype<T2>, R_TIMES>(t1, t2);
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator / (T1 t1, T2 t2)
	{
		return Rop2<Rtype<T1>, Rtype<T2>, R_DIVIDES>(t1, t2);
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator & (T1 t1, T2 t2)
	{
		return Rop2<Rtype<T1>, Rtype<T2>, R_BITAND>(t1, t2);
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator | (T1 t1, T2 t2)
	{
		return Rop2<Rtype<T1>, Rtype<T2>, R_BITOR>(t1, t2);
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator ^ (T1 t1, T2 t2)
	{
		return Rop2<Rtype<T1>, Rtype<T2>, R_BITXOR>(t1, t2);
	}
}
