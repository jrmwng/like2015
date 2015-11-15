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
		, xmm_ptr_c11<uIndex + 2, TC...>
	{
		typedef xmm_ptr_c11<uIndex, TA, TB> base_type;
		typedef xmm_ptr_c11<uIndex + 2, TC...> next_type;

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
		, xmm_ptr_c11<uIndex + 4, TE...>
	{
		typedef xmm_ptr_c11<uIndex, TA, TB, TC, TD> base_type;
		typedef xmm_ptr_c11<uIndex + 4, TE...> next_type;

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
		typedef void type;
	};
	template <unsigned uIndex, typename TA, typename... TB>
	struct xmm_ptr_element<uIndex, xmm_ptr<TA, TB...>>
		: xmm_ptr_element<uIndex - 1, xmm_ptr<TB...>>
	{};

#ifdef _M_X64
	template <unsigned uIndex, typename... TPointers>
	typename std::enable_if<(uIndex % 2 != 0), typename xmm_ptr_element<uIndex, xmm_ptr<TPointers...>>::type*>::type get_ptr(xmm_ptr<TPointers...> const & xmmPtr)
	{
		return reinterpret_cast<typename xmm_ptr_element<uIndex, xmm_ptr<TPointers...>>::type*>(_mm_extract_epi64(static_cast<xmm_ptr_c11<uIndex&(~(2 - 1))>const&>(xmmPtr).xmm, uIndex & (2 - 1)));
	}
	template <unsigned uIndex, typename... TPointers>
	typename std::enable_if<(uIndex % 2 == 0), typename xmm_ptr_element<uIndex, xmm_ptr<TPointers...>>::type*>::type get_ptr(xmm_ptr<TPointers...> const & xmmPtr)
	{
		return reinterpret_cast<typename xmm_ptr_element<uIndex, xmm_ptr<TPointers...>>::type*>(_mm_cvtsi128_si64(static_cast<xmm_ptr_c11<uIndex>const&>(xmmPtr).xmm));
	}
#else
	template <unsigned uIndex, typename... TPointers>
	typename std::enable_if<(uIndex % 4 != 0), typename xmm_ptr_element<uIndex, xmm_ptr<TPointers...>>::type*>::type get_ptr(xmm_ptr<TPointers...> const & xmmPtr)
	{
		return reinterpret_cast<typename xmm_ptr_element<uIndex, xmm_ptr<TPointers...>>::type*>(_mm_extract_epi32(static_cast<xmm_ptr_c11<uIndex&(~(4 - 1))>const&>(xmmPtr).xmm, uIndex & (4 - 1)));
	}
	template <unsigned uIndex, typename... TPointers>
	typename std::enable_if<(uIndex % 4 == 0), typename xmm_ptr_element<uIndex, xmm_ptr<TPointers...>>::type*>::type get_ptr(xmm_ptr<TPointers...> const & xmmPtr)
	{
		return reinterpret_cast<typename xmm_ptr_element<uIndex, xmm_ptr<TPointers...>>::type*>(_mm_cvtsi128_si32(static_cast<xmm_ptr_c11<uIndex>const&>(xmmPtr).xmm));
	}
#endif
}
