#pragma once

/* Author: jrmwng @ 2015 */

#include <intrin.h>

namespace like
{
	template <unsigned uIndex, typename... TPointers>
	struct xmm_ptr_c11;

	template <unsigned uIndex>
	struct xmm_ptr_c11<uIndex>
	{
		__m128i xmm;

		xmm_ptr_c11(__m128i const & that)
			: xmm(that)
		{}

		void operator += (xmm_ptr_c11 const & that)
		{
#ifdef _M_X64
			xmm = _mm_add_epi64(xmm, that.xmm);
#else
			xmm = _mm_add_epi32(xmm, that.xmm);
#endif
		}
		void operator -= (xmm_ptr_c11 const & that)
		{
#ifdef _M_X64
			xmm = _mm_sub_epi64(xmm, that.xmm);
#else
			xmm = _mm_sub_epi32(xmm, that.xmm);
#endif
		}
		void operator *= (intptr_t n) // scalar multiplication only
		{
#ifdef _M_X64
			xmm = _mm_mul_epi32(xmm, _mm_set1_epi64x(n));
#else
			xmm = _mm_mullo_epi32(xmm, _mm_set1_epi32(n));
#endif
		}
	};
	
	template <unsigned uIndex, typename TA>
	struct xmm_ptr_c11<uIndex, TA>
		: xmm_ptr_c11<uIndex>
	{
		typedef xmm_ptr_c11<uIndex> base_type;

		xmm_ptr_c11(__m128i const & that)
			: base_type(that)
		{}
		xmm_ptr_c11(intptr_t n) // for pointer arithmetic
#ifdef _M_X64
			: base_type(_mm_cvtsi64_si128(n * sizeof(TA)))
#else
			: base_type(_mm_cvtsi32_si128(n * sizeof(TA)))
#endif
		{}
		xmm_ptr_c11(TA *ptA)
#ifdef _M_X64
			: base_type(_mm_cvtsi64_si128(reinterpret_cast<intptr_t>(ptA)))
#else
			: base_type(_mm_cvtsi32_si128(reinterpret_cast<intptr_t>(ptA)))
#endif
		{}
	};

	template <unsigned uIndex, typename TA, typename TB>
	struct xmm_ptr_c11<uIndex, TA, TB>
		: xmm_ptr_c11<uIndex, TA>
	{
		typedef xmm_ptr_c11<uIndex, TA> base_type;

		xmm_ptr_c11(__m128i const & that)
			: base_type(that)
		{}
		template <typename T1, typename T2>
		xmm_ptr_c11(T1 t1, T2 t2)
#ifdef _M_X64
			: base_type(_mm_unpacklo_epi64(xmm_ptr_c11<0, TA>(t1).xmm, xmm_ptr_c11<0, TB>(t2).xmm))
#else
			: base_type(_mm_unpacklo_epi32(xmm_ptr_c11<0, TA>(t1).xmm, xmm_ptr_c11<0, TB>(t2).xmm))
#endif
		{}
	};

#ifdef _M_X64
	template <unsigned uIndex, typename TA, typename TB, typename... TC>
	struct xmm_ptr_c11<uIndex, TA, TB, TC...>
		: xmm_ptr_c11<uIndex, TA, TB>
		, xmm_ptr_c11<uIndex + 1, TC...>
	{
		typedef xmm_ptr_c11<uIndex, TA, TB> base_type;
		typedef xmm_ptr_c11<uIndex + 1, TC...> next_type;

		template <typename T1, typename T2, typename...T3>
		xmm_ptr_c11(T1 t1, T2 t2, T3... t3)
			: base_type(t1, t2)
			, next_type(t3...)
		{}

		void operator += (xmm_ptr_c11 const & that)
		{
			base_type::operator += (that);
			next_type::operator += (that);
		}
		void operator -= (xmm_ptr_c11 const & that)
		{
			base_type::operator -= (that);
			next_type::operator -= (that);
		}
		void operator *= (intptr_t n)
		{
			base_type::operator *= (n);
			next_type::operator *= (n);
		}
	};
#else
	template <unsigned uIndex, typename TA, typename TB, typename TC>
	struct xmm_ptr_c11<uIndex, TA, TB, TC>
		: xmm_ptr_c11<uIndex, TA, TB>
	{
		typedef xmm_ptr_c11<uIndex, TA, TB> base_type;

		xmm_ptr_c11(__m128i const & that)
			: base_type(that)
		{}
		template <typename T1, typename T2, typename T3>
		xmm_ptr_c11(T1 t1, T2 t2, T3 t3)
			: base_type(_mm_unpacklo_epi64(xmm_ptr_c11<0, TA, TB>(t1, t2).xmm, xmm_ptr_c11<0, TC>(t3).xmm))
		{}
	};
	template <unsigned uIndex, typename TA, typename TB, typename TC, typename TD>
	struct xmm_ptr_c11<uIndex, TA, TB, TC, TD>
		: xmm_ptr_c11<uIndex, TA, TB, TC>
	{
		typedef xmm_ptr_c11<uIndex, TA, TB, TC> base_type;

		xmm_ptr_c11(__m128i const & that)
			: base_type(that)
		{}
		template <typename T1, typename T2, typename T3, typename T4>
		xmm_ptr_c11(T1 t1, T2 t2, T3 t3, T4 t4)
			: base_type(_mm_unpacklo_epi64(xmm_ptr_c11<0, TA, TB>(t1, t2).xmm, xmm_ptr_c11<0, TC, TD>(t3, t4).xmm))
		{}
	};
	template <unsigned uIndex, typename TA, typename TB, typename TC, typename TD, typename... TE>
	struct xmm_ptr_c11<uIndex, TA, TB, TC, TD, TE...>
		: xmm_ptr_c11<uIndex, TA, TB, TC, TD>
		, xmm_ptr_c11<uIndex + 1, TE...>
	{
		typedef xmm_ptr_c11<uIndex, TA, TB, TC, TD> base_type;
		typedef xmm_ptr_c11<uIndex + 1, TE...> next_type;

		template <typename T1, typename T2, typename T3, typename T4, typename... T5>
		xmm_ptr_c11(T1 t1, T2 t2, T3 t3, T4 t4, T5... t5)
			: base_type(t1, t2, t3, t4)
			, next_type(t5...)
		{}

		void operator += (xmm_ptr_c11 const & that)
		{
			base_type::operator += (that);
			next_type::operator += (that);
		}
		void operator -= (xmm_ptr_c11 const & that)
		{
			base_type::operator -= (that);
			next_type::operator -= (that);
		}
		void operator *= (intptr_t n)
		{
			base_type::operator *= (n);
			next_type::operator *= (n);
		}
	};
#endif

	template <typename... TPointers>
	struct xmm_ptr
		: xmm_ptr_c11<0, TPointers...>
	{
		typedef xmm_ptr_c11<0, TPointers...> base_type;

		template <typename... TArgs>
		xmm_ptr(TArgs... Args)
			: base_type(Args...)
		{}

		xmm_ptr & operator += (xmm_ptr const & that)
		{
			base_type::operator += (that);
			return *this;
		}
		xmm_ptr operator + (xmm_ptr const & that) const
		{
			return xmm_ptr(*this) += that;
		}
		xmm_ptr & operator -= (xmm_ptr const & that)
		{
			base_type::operator -= (that);
			return *this;
		}
		xmm_ptr operator - (xmm_ptr const & that) const
		{
			return xmm_ptr(*this) -= that;
		}
		xmm_ptr & operator *= (intptr_t n)
		{
			base_type::operator *= (n);
			return *this;
		}
		xmm_ptr operator * (intptr_t n) const
		{
			return xmm_ptr(*this) *= n;
		}
	};

	template <unsigned uIndex, typename TPtr>
	struct xmm_ptr_element;

	template <typename TA, typename... TB>
	struct xmm_ptr_element<0, xmm_ptr<TA, TB...>>
	{
		typedef TA type;
	};
	template <unsigned uIndex, typename TA, typename... TB>
	struct xmm_ptr_element<uIndex, xmm_ptr<TA, TB...>>
		: xmm_ptr_element<uIndex - 1, xmm_ptr<TB...>>
	{};

	template <unsigned uIndex, typename TPtr>
	struct xmm_ptr_access
	{
		typedef typename xmm_ptr_element<uIndex, TPtr>::type type;

		TPtr & xmmPtr;

		xmm_ptr_access(TPtr & that)
			: xmmPtr(that)
		{}

		operator typename type * (void) const
		{
#ifdef _M_X64
			return reinterpret_cast<typename type*>(_mm_extract_epi64(static_cast<xmm_ptr_c11<uIndex / 2>const&>(xmmPtr).xmm, uIndex % 2));
#else
			return reinterpret_cast<typename type*>(_mm_extract_epi32(static_cast<xmm_ptr_c11<uIndex / 4>const&>(xmmPtr).xmm, uIndex % 4));
#endif
		}

		template <unsigned uIndex1, typename TPtr1>
		bool operator == (xmm_ptr_access<uIndex1, TPtr1> const & that) const
		{
			return static_cast<typename type*>(*this) == static_cast<typename xmm_ptr_access<uIndex1, TPtr1>::type*>(that);
		}
		template <unsigned uIndex1, typename TPtr1>
		bool operator != (xmm_ptr_access<uIndex1, TPtr1> const & that) const
		{
			return static_cast<typename type*>(*this) != static_cast<typename xmm_ptr_access<uIndex1, TPtr1>::type*>(that);
		}
		template <unsigned uIndex1, typename TPtr1>
		bool operator <= (xmm_ptr_access<uIndex1, TPtr1> const & that) const
		{
			return static_cast<typename type*>(*this) <= static_cast<typename xmm_ptr_access<uIndex1, TPtr1>::type*>(that);
		}
		template <unsigned uIndex1, typename TPtr1>
		bool operator >= (xmm_ptr_access<uIndex1, TPtr1> const & that) const
		{
			return static_cast<typename type*>(*this) >= static_cast<typename xmm_ptr_access<uIndex1, TPtr1>::type*>(that);
		}
		template <unsigned uIndex1, typename TPtr1>
		bool operator < (xmm_ptr_access<uIndex1, TPtr1> const & that) const
		{
			return static_cast<typename type*>(*this) < static_cast<typename xmm_ptr_access<uIndex1, TPtr1>::type*>(that);
		}
		template <unsigned uIndex1, typename TPtr1>
		bool operator > (xmm_ptr_access<uIndex1, TPtr1> const & that) const
		{
			return static_cast<typename type*>(*this) > static_cast<typename xmm_ptr_access<uIndex1, TPtr1>::type*>(that);
		}

		typename std::enable_if<!std::is_const<TPtr>::value, xmm_ptr_access>::type & operator = (type *ptA)
		{
#ifdef _M_X64
			static_cast<xmm_ptr_c11<uIndex / 2>&>(xmmPtr).xmm = _mm_insert_epi64(static_cast<xmm_ptr_c11<uIndex / 2>const&>(xmmPtr).xmm, reinterpret_cast<intptr_t>(ptA), uIndex % 2);
#else
			static_cast<xmm_ptr_c11<uIndex / 4>&>(xmmPtr).xmm = _mm_insert_epi32(static_cast<xmm_ptr_c11<uIndex / 4>const&>(xmmPtr).xmm, reinterpret_cast<intptr_t>(ptA), uIndex % 4);
#endif
			return *this;
		}

		typename std::enable_if<!std::is_const<TPtr>::value, xmm_ptr_access>::type & operator += (intptr_t n)
		{
#ifdef _M_X64
			static_cast<xmm_ptr_c11<uIndex / 2>&>(xmmPtr).xmm = _mm_add_epi64(static_cast<xmm_ptr_c11<uIndex / 2>const&>(xmmPtr).xmm, _mm_insert_epi64(_mm_setzero_si128(), n * sizeof(type), uIndex % 2));
#else
			static_cast<xmm_ptr_c11<uIndex / 4>&>(xmmPtr).xmm = _mm_add_epi32(static_cast<xmm_ptr_c11<uIndex / 4>const&>(xmmPtr).xmm, _mm_insert_epi32(_mm_setzero_si128(), n * sizeof(type), uIndex % 4));
#endif
			return *this;
		}
		typename std::enable_if<!std::is_const<TPtr>::value, xmm_ptr_access>::type & operator -= (intptr_t n)
		{
#ifdef _M_X64
			static_cast<xmm_ptr_c11<uIndex / 2>&>(xmmPtr).xmm = _mm_sub_epi64(static_cast<xmm_ptr_c11<uIndex / 2>const&>(xmmPtr).xmm, _mm_insert_epi64(_mm_setzero_si128(), n * sizeof(type), uIndex % 2));
#else
			static_cast<xmm_ptr_c11<uIndex / 4>&>(xmmPtr).xmm = _mm_sub_epi32(static_cast<xmm_ptr_c11<uIndex / 4>const&>(xmmPtr).xmm, _mm_insert_epi32(_mm_setzero_si128(), n * sizeof(type), uIndex % 4));
#endif
			return *this;
		}
	};

	template <unsigned uIndex, typename... TPointers>
	xmm_ptr_access<uIndex, xmm_ptr<TPointers...>> get_ptr(xmm_ptr<TPointers...> & xmmPtr)
	{
		return xmm_ptr_access<uIndex, xmm_ptr<TPointers...>>(xmmPtr);
	}
	template <unsigned uIndex, typename... TPointers>
	xmm_ptr_access<uIndex, xmm_ptr<TPointers...>const> get_ptr(xmm_ptr<TPointers...> const & xmmPtr)
	{
		return xmm_ptr_access<uIndex, xmm_ptr<TPointers...>const>(xmmPtr);
	}
}
