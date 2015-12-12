#pragma once

/* Author: jrmwng @ 2015 */

#include <type_traits>

namespace like
{
	template <typename... TF>
	struct field_t;

	template <>
	struct field_t<>
	{
		template <typename T1>
		T1 & operator () (T1 & t1) const
		{
			return t1;
		}
	};

	template <typename TF1, typename... TFRest>
	struct field_t<TF1, TFRest...>
		: field_t<TFRest...>
	{
		typedef field_t<TFRest...> base_type;

		TF1 const m_tF1;

		template <typename... TArgs>
		field_t(TF1 const & tF1, TArgs... _Args)
			: base_type(std::forward<TArgs>(_Args)...)
			, m_tF1(tF1)
		{}

		template <typename T1>
		decltype(auto) operator () (T1 & t1) const
		{
			return base_type::operator()(t1.*m_tF1);
		}
		template <typename T1>
		decltype(auto) operator () (T1 *pt1) const
		{
			return base_type::operator()(pt1->*m_tF1);
		}
	};

	template <typename... TF>
	field_t<TF...> field(TF... tF)
	{
		return field_t<TF...>(std::forward<TF>(tF)...);
	}
}