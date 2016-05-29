#pragma once
 
/* Author: jrmwng @ 2016 */

// R for random

#include <type_traits>
#include <functional>
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
	Rtype<T const> r(T const & t)
	{
		return Rtype<T const>(t);
	}

	template <typename T>
	struct Rvar
		: Rexpr
	{
		using type = T;

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
	};
	template <typename T1, typename T2>
	std::enable_if_t<(std::is_base_of<Rexpr, T1>::value != std::is_base_of<Rexpr, T2>::value), T1 &> operator += (T1 &t1, T2 const &t2)
	{
		r(t1).m_t += r(t2).eval<std::decay_t<typename Rtype<T1>::type>>();
		return t1;
	}
	template <typename T1, typename T2>
	std::enable_if_t<(std::is_base_of<Rexpr, T1>::value != std::is_base_of<Rexpr, T2>::value), T1 &> operator -= (T1 &t1, T2 const &t2)
	{
		r(t1).m_t -= r(t2).eval<std::decay_t<typename Rtype<T1>::type>>();
		return t1;
	}
	template <typename T1, typename T2>
	std::enable_if_t<(std::is_base_of<Rexpr, T1>::value != std::is_base_of<Rexpr, T2>::value), T1 &> operator *= (T1 &t1, T2 const &t2)
	{
		r(t1).m_t *= r(t2).eval<std::decay_t<typename Rtype<T1>::type>>();
		return t1;
	}
	template <typename T1, typename T2>
	std::enable_if_t<(std::is_base_of<Rexpr, T1>::value != std::is_base_of<Rexpr, T2>::value), T1 &> operator /= (T1 &t1, T2 const &t2)
	{
		r(t1).m_t /= r(t2).eval<std::decay_t<typename Rtype<T1>::type>>();
		return t1;
	}
	template <typename T1, typename T2>
	std::enable_if_t<(std::is_base_of<Rexpr, T1>::value != std::is_base_of<Rexpr, T2>::value), T1 &> operator %= (T1 &t1, T2 const &t2)
	{
		r(t1).m_t %= r(t2).eval<std::decay_t<typename Rtype<T1>::type>>();
		return t1;
	}
	template <typename T1, typename T2>
	std::enable_if_t<(std::is_base_of<Rexpr, T1>::value != std::is_base_of<Rexpr, T2>::value), T1 &> operator &= (T1 &t1, T2 const &t2)
	{
		r(t1).m_t &= r(t2).eval<std::decay_t<typename Rtype<T1>::type>>();
		return t1;
	}
	template <typename T1, typename T2>
	std::enable_if_t<(std::is_base_of<Rexpr, T1>::value != std::is_base_of<Rexpr, T2>::value), T1 &> operator |= (T1 &t1, T2 const &t2)
	{
		r(t1).m_t |= r(t2).eval<std::decay_t<typename Rtype<T1>::type>>();
		return t1;
	}
	template <typename T1, typename T2>
	std::enable_if_t<(std::is_base_of<Rexpr, T1>::value != std::is_base_of<Rexpr, T2>::value), T1 &> operator ^= (T1 &t1, T2 const &t2)
	{
		r(t1).m_t ^= r(t2).eval<std::decay_t<typename Rtype<T1>::type>>();
		return t1;
	}
	template <typename T1, typename T2>
	std::enable_if_t<(std::is_base_of<Rexpr, T1>::value != std::is_base_of<Rexpr, T2>::value), T1 &> operator <<= (T1 &t1, T2 const &t2)
	{
		r(t1).m_t <<= r(t2).eval<std::decay_t<typename Rtype<T1>::type>>();
		return t1;
	}
	template <typename T1, typename T2>
	std::enable_if_t<(std::is_base_of<Rexpr, T1>::value != std::is_base_of<Rexpr, T2>::value), T1 &> operator >>= (T1 &t1, T2 const &t2)
	{
		r(t1).m_t >>= r(t2).eval<std::decay_t<typename Rtype<T1>::type>>();
		return t1;
	}

	template <template <typename T> typename Top, typename... TArgs>
	struct Rop
		: Rexpr
	{
		std::tuple<TArgs...> m_Tuple;

		template <typename... TParams>
		Rop(TParams ... tParams)
			: m_Tuple((tParams)...)
		{}

		template <typename TR, size_t... uIndex>
		auto eval(std::index_sequence<uIndex...>) const
		{
			return Top<TR>()(std::get<uIndex>(m_Tuple).eval<TR>()...);
		}

		template <typename TR>
		auto eval() const
		{
			return eval<TR>(std::index_sequence_for<TArgs...>());
		}
	};
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value != std::is_base_of<Rexpr, T2>::value)>> bool operator < (T1 t1, T2 t2)
	{
		return Rop<std::less, Rtype<T1>, Rtype<T2>>(t1, t2).eval<std::conditional_t<std::is_base_of<Rexpr, T1>::value, T2, T1>>();
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value != std::is_base_of<Rexpr, T2>::value)>> bool operator <= (T1 t1, T2 t2)
	{
		return Rop<std::less_equal, Rtype<T1>, Rtype<T2>>(t1, t2).eval<std::conditional_t<std::is_base_of<Rexpr, T1>::value, T2, T1>>();
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value != std::is_base_of<Rexpr, T2>::value)>> bool operator > (T1 t1, T2 t2)
	{
		return Rop<std::greater, Rtype<T1>, Rtype<T2>>(t1, t2).eval<std::conditional_t<std::is_base_of<Rexpr, T1>::value, T2, T1>>();
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value != std::is_base_of<Rexpr, T2>::value)>> bool operator >= (T1 t1, T2 t2)
	{
		return Rop<std::greater_equal, Rtype<T1>, Rtype<T2>>(t1, t2).eval<std::conditional_t<std::is_base_of<Rexpr, T1>::value, T2, T1>>();
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value != std::is_base_of<Rexpr, T2>::value)>> bool operator != (T1 t1, T2 t2)
	{
		return Rop<std::not_equal_to, Rtype<T1>, Rtype<T2>>(t1, t2).eval<std::conditional_t<std::is_base_of<Rexpr, T1>::value, T2, T1>>();
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value != std::is_base_of<Rexpr, T2>::value)>> bool operator == (T1 t1, T2 t2)
	{
		return Rop<std::equal_to, Rtype<T1>, Rtype<T2>>(t1, t2).eval<std::conditional_t<std::is_base_of<Rexpr, T1>::value, T2, T1>>();
	}
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
	template <typename T>
	struct r_shiftleft
	{
		T operator() (T const &t1, T const &t2) const
		{
			return t1 << t2;
		}
	};
	template <typename T>
	struct r_shiftright
	{
		T operator() (T const &t1, T const &t2) const
		{
			return t1 >> t2;
		}
	};
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator << (T1 t1, T2 t2)
	{
		return Rop<r_shiftleft, Rtype<T1>, Rtype<T2>>(t1, t2);
	}
	template <typename T1, typename T2, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value || std::is_base_of<Rexpr, T2>::value)>> auto operator >> (T1 t1, T2 t2)
	{
		return Rop<r_shiftright, Rtype<T1>, Rtype<T2>>(t1, t2);
	}
	template <typename T1, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value)>> auto operator ~ (T1 t1)
	{
		return Rop<std::bit_not, Rtype<T1>>(t1);
	}
	template <typename T1, typename TEnableIf = std::enable_if_t<(std::is_base_of<Rexpr, T1>::value)>> auto operator - (T1 t1)
	{
		return Rop<std::negate, Rtype<T1>>(t1);
	}
}
