#pragma once

/* Author: jrmwng @ 2015 */

#include <type_traits>
#include <functional>

namespace like
{
	template <typename TR, typename... TM>
	struct member_t;

	template <typename TR>
	struct member_t<TR>
	{
		typedef TR type;

		template <typename T1>
		typename std::enable_if<(!std::is_void<TR>::value), TR>::type operator () (T1 & t1) const
		{
			return t1;
		}
		template <typename T1>
		typename std::enable_if<(std::is_void<TR>::value), T1 &>::type operator () (T1 & t1) const
		{
			return t1;
		}
	};

	template <typename TR, typename TM1, typename... TMRest>
	struct member_t<TR, TM1, TMRest...>
		: member_t<TR, TMRest...>
	{
		typedef member_t<TR, TMRest...> base_type;

		TM1 const m_tM1;

		template <typename... TArgs>
		member_t(TM1 const & tM1, TArgs... _Args)
			: base_type(std::forward<TArgs>(_Args)...)
			, m_tM1(tM1)
		{}

		template <typename T1, typename TM, typename TV>
		decltype(auto) eval(T1 *pt1, TV(TM::*pm)) const
		{
			return base_type::operator()(pt1->*pm);
		}
		template <typename T1, typename TM, typename TV, typename TProtect = std::enable_if<(std::is_reference<TV>::value) || (!std::is_reference<TR>::value && !std::is_void<TR>::value)>::type>
		decltype(auto) eval(T1 *pt1, TV(TM::*pm)()) const
		{
			TV tV((pt1->*pm)());
			return base_type::operator()(tV);
		}
		template <typename T1, typename TM, typename TV, typename TProtect = std::enable_if<(std::is_reference<TV>::value) || (!std::is_reference<TR>::value && !std::is_void<TR>::value)>::type>
		decltype(auto) eval(T1 *pt1, TV(TM::*pm)()const) const
		{
			TV tV((pt1->*pm)());
			return base_type::operator()(tV);
		}
		template <typename T1, typename TM, typename TV, typename TProtect = std::enable_if<(std::is_reference<TV>::value) || (!std::is_reference<TR>::value && !std::is_void<TR>::value)>::type>
		decltype(auto) eval(T1 *pt1, TV(TM::*pm)()volatile) const
		{
			TV tV((pt1->*pm)());
			return base_type::operator()(tV);
		}
		template <typename T1, typename TM, typename TV, typename TProtect = std::enable_if<(std::is_reference<TV>::value) || (!std::is_reference<TR>::value && !std::is_void<TR>::value)>::type>
		decltype(auto) eval(T1 *pt1, TV(TM::*pm)()const volatile) const
		{
			TV tV((pt1->*pm)());
			return base_type::operator()(tV);
		}

		template <typename T1>
		decltype(auto) operator () (T1 & t1) const
		{
			return eval(std::addressof(t1), m_tM1);
		}
		template <typename T1>
		decltype(auto) operator () (T1 *pt1) const
		{
			return eval(pt1, m_tM1);
		}
	};

	template <typename TR = void, typename... TM>
	member_t<TR, TM...> member(TM... tM)
	{
		return member_t<TR, TM...>(std::forward<TM>(tM)...);
	}

	//

	template <typename TM, typename TOrder>
	struct order_by_t
	{
		TM const m_tM;
		TOrder const m_tOrder;

		order_by_t(TM const & tM, TOrder const & tOrder)
			: m_tM(tM)
			, m_tOrder(tOrder)
		{}

		bool operator () (typename TM::type const & t1, typename TM::type const & t2) const
		{
			return m_tOrder(t1, t2);
		}
		template <typename T1>
		bool operator () (T1 const & t1, typename TM::type const & t2) const
		{
			return m_tOrder(m_tM(t1), t2);
		}
		template <typename T2>
		bool operator () (typename TM::type const & t1, T2 const & t2) const
		{
			return m_tOrder(t1, m_tM(t2));
		}
		template <typename T1, typename T2>
		bool operator () (T1 const & t1, T2 const & t2) const
		{
			return m_tOrder(m_tM(t1), m_tM(t2));
		}
	};

	template <typename TM, typename TOrder = std::less<typename TM::type>>
	order_by_t<TM, TOrder> order_by(TM const & tM, TOrder const & tOrder = TOrder())
	{
		return order_by_t<TM, TOrder>(tM, tOrder);
	}
	template <typename TS, typename TV, typename TM = member_t<TV, TV(TS::*)>, typename TOrder = std::less<typename TM::type>>
	order_by_t<TM, TOrder> order_by(TV(TS::*pM), TOrder const & tOrder = TOrder())
	{
		return order_by_t<TM, TOrder>(TM(pM), tOrder);
	}
}
