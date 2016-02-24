#pragma once

/* Author: jrmwng @ 2015~2016 */

#include <intrin.h>
#include <type_traits>

namespace jrmwng
{
	template <unsigned uSize>
	struct atomic_integral;

	template <> struct atomic_integral<1> { typedef char type; };
	template <> struct atomic_integral<2> { typedef short type; };
	template <> struct atomic_integral<4> { typedef long type; };
	template <> struct atomic_integral<8> { typedef long long type; };
	template <> struct atomic_integral<16> { typedef __m128i type; };

	template <unsigned uSize>
	using atomic_integral_t = typename atomic_integral<uSize>::type;

	template <unsigned uSize>
	void atomic_lock_load(void const volatile *pv, void *p);
	template <unsigned uSize>
	void atomic_lock_store(void volatile *pv, void const *p);
	template <unsigned uSize>
	void atomic_lock_swap(void volatile *pv, void *p);
	template <unsigned uSize>
	bool atomic_lock_cas(void volatile *pv, void *p, void const *pCMP);

	template <typename T>
	T atomic_rtm_load(T const volatile *pvt)
	{
		typedef atomic_integral_t<sizeof(T)> TLocal;
		
		TLocal stLocal;
		{
#ifdef RTM
			unsigned const uStatus = _xbegin();

			if (uStatus == _XBEGIN_STARTED)
			{
				stLocal = reinterpret_cast<TLocal const volatile&>(*pvt);
				_xend();
			}
			else
#endif
			{
				atomic_lock_load<sizeof(T)>(pvt, &stLocal);
			}
		}
		return reinterpret_cast<T const&>(stLocal);
	}
	template <typename T>
	void atomic_rtm_store(T volatile *pvt, T const & t)
	{
		T t0 = t;
		{
#ifdef RTM
			typedef atomic_integral_t<sizeof(T)> TLocal;

			unsigned const uStatus = _xbegin();

			if (uStatus == _XBEGIN_STARTED)
			{
				// read-set: *pvt
				TLocal const stLocal = reinterpret_cast<TLocal volatile&>(*pvt);

				// write-set: *pvt
				reinterpret_cast<TLocal volatile&>(*pvt) = reinterpret_cast<TLocal const&>(t0);
				_xend();

				reinterpret_cast<TLocal&>(t0) = stLocal;
			}
			else
#endif
			{
				if (std::is_pod<T>::value)
					atomic_lock_store<sizeof(T)>(pvt, std::addressof(t0));
				else
					atomic_lock_swap<sizeof(T)>(pvt, std::addressof(t0));
			}
		}
	}
	template <typename T>
	void atomic_rtm_swap(T volatile *pvt, T & t)
	{
#ifdef RTM
		typedef atomic_integral_t<sizeof(T)> TLocal;

		TLocal const stLocal0 = reinterpret_cast<TLocal&>(t);

		unsigned const uStatus = _xbegin();

		if (uStatus == _XBEGIN_STARTED)
		{
			// read-set: *pvt
			TLocal const stLocal1 = reinterpret_cast<TLocal volatile&>(*pvt);

			// write-set: *pvt
			reinterpret_cast<TLocal&>(*pvt) = stLocal0;
			_xend();

			reinterpret_cast<TLocal&>(t) = stLocal1;
		}
		else
#endif
		{
			atomic_lock_swap<sizeof(T)>(pvt, std::addressof(t));
		}
	}
	template <typename T>
	bool atomic_rtm_cas(T volatile *pvt, T & t, T const & tCMP)
	{
#ifdef RTM
		typedef atomic_integral_t<sizeof(T)> TLocal;

		TLocal const stLocal0 = reinterpret_cast<TLocal&>(t);
		TLocal const stLocalCMP = reinterpret_cast<TLocal const&>(tCMP);

		unsigned const uStatus = _xbegin();

		if (uStatus == _XBEGIN_STARTED)
		{
			// read-set: *pvt
			TLocal const stLocal1 = reinterpret_cast<TLocal volatile&>(*pvt);

			if (stLocal1 == stLocalCMP)
			{
				// write-set: *pvt
				reinterpret_cast<TLocal volatile&>(*pvt) = stLocal0;
				_xend();

				reinterpret_cast<TLocal&>(t) = stLocal1;
				return true;
			}
			else
			{
				return false;
			}
		}
		else
#endif
		{
			return atomic_lock_cas<sizeof(T)>(pvt, std::addressof(t), std::addressof(tCMP));
		}
	}

	template <unsigned uSize>
	void atomic_load(void const volatile *pv, void *p);
	template <unsigned uSize>
	void atomic_store(void volatile *pv, void const *p);
	template <unsigned uSize>
	void atomic_swap(void volatile *pv, void *p);
	template <unsigned uSize>
	bool atomic_cas(void volatile *pv, void *p, void const *pCMP);

	template <>
	inline void atomic_lock_load<1>(void const volatile *pv, void *p)
	{
		char volatile *pvb = reinterpret_cast<char volatile*>(const_cast<void volatile*>(pv));
		char *pb = reinterpret_cast<char*>(p);
		*pb = _InterlockedCompareExchange8(pvb, 0, 0);
	}
	template <>
	inline void atomic_load<1>(void const volatile *pv, void *p)
	{
		char const volatile *pvb = reinterpret_cast<char const volatile*>(pv);
		char *pb = reinterpret_cast<char*>(p);
		*pb = *pvb;
	}
	template <>
	inline void atomic_lock_store<1>(void volatile *pv, void const *p)
	{
		char volatile *pvb = reinterpret_cast<char volatile*>(pv);
		char const *pb = reinterpret_cast<char const*>(p);
		_InterlockedExchange8(pvb, *pb);
	}
	template <>
	inline void atomic_store<1>(void volatile *pv, void const *p)
	{
		char volatile *pvb = reinterpret_cast<char volatile*>(pv);
		char const *pb = reinterpret_cast<char const*>(p);
		*pvb = *pb;
	}
	template <>
	inline void atomic_lock_swap<1>(void volatile *pv, void *p)
	{
		char volatile *pvb = reinterpret_cast<char volatile*>(pv);
		char *pb = reinterpret_cast<char*>(p);
		*pb = _InterlockedExchange8(pvb, *pb);
	}
	template <>
	inline void atomic_swap<1>(void volatile *pv, void *p)
	{
		char volatile *pvb = reinterpret_cast<char volatile*>(pv);
		char *pb = reinterpret_cast<char*>(p);
		atomic_rtm_swap(pvb, *pb);
	}
	template <>
	inline bool atomic_lock_cas<1>(void volatile *pv, void *p, void const *pCMP)
	{
		char volatile *pvb = reinterpret_cast<char volatile*>(pv);
		char *pb = reinterpret_cast<char*>(p);

		char const bCMP = *reinterpret_cast<char const*>(pCMP);
		char const b = _InterlockedCompareExchange8(pvb, *pb, bCMP);
		{
			if (b == bCMP)
			{
				*pb = b;
				return true;
			}
			else
				return false;
		}
	}
	template <>
	inline bool atomic_cas<1>(void volatile *pv, void *p, void const *pCMP)
	{
		char volatile *pvb = reinterpret_cast<char volatile*>(pv);
		char *pb = reinterpret_cast<char*>(p);
		char const *pbCMP = reinterpret_cast<char const*>(pCMP);
		return atomic_rtm_cas(pvb, *pb, *pbCMP);
	}

	template <>
	inline void atomic_lock_load<2>(void const volatile *pv, void *p)
	{
		short volatile *pvw = reinterpret_cast<short volatile*>(const_cast<void volatile*>(pv));
		short *pw = reinterpret_cast<short*>(p);
		*pw = _InterlockedCompareExchange16(pvw, 0, 0);
	}
	template <>
	inline void atomic_load<2>(void const volatile *pv, void *p)
	{
		short const volatile *pvw = reinterpret_cast<short const volatile*>(pv);
		short *pw = reinterpret_cast<short*>(p);
		*pw = *pvw;
	}
	template <>
	inline void atomic_lock_store<2>(void volatile *pv, void const *p)
	{
		short volatile *pvw = reinterpret_cast<short volatile*>(pv);
		short const *pw = reinterpret_cast<short const*>(p);
		_InterlockedExchange16(pvw, *pw);
	}
	template <>
	inline void atomic_store<2>(void volatile *pv, void const *p)
	{
		short volatile *pvw = reinterpret_cast<short volatile*>(pv);
		short const *pw = reinterpret_cast<short const*>(p);
		*pvw = *pw;
	}
	template <>
	inline void atomic_lock_swap<2>(void volatile *pv, void *p)
	{
		short volatile *pvw = reinterpret_cast<short volatile*>(pv);
		short *pw = reinterpret_cast<short*>(p);
		*pw = _InterlockedExchange16(pvw, *pw);
	}
	template <>
	inline void atomic_swap<2>(void volatile *pv, void *p)
	{
		short volatile *pvw = reinterpret_cast<short volatile*>(pv);
		short *pw = reinterpret_cast<short*>(p);
		atomic_rtm_swap(pvw, *pw);
	}
	template <>
	inline bool atomic_lock_cas<2>(void volatile *pv, void *p, void const *pCMP)
	{
		short volatile *pvw = reinterpret_cast<short volatile*>(pv);
		short *pw = reinterpret_cast<short*>(p);
		short const wCMP = *reinterpret_cast<short const*>(pCMP);
		short w = _InterlockedCompareExchange16(pvw, *pw, wCMP);
		{
			if (w == wCMP)
			{
				*pw = w;
				return true;
			}
			else
				return false;
		}
	}
	template <>
	inline bool atomic_cas<2>(void volatile *pv, void *p, void const *pCMP)
	{
		short volatile *pvw = reinterpret_cast<short volatile*>(pv);
		short *pw = reinterpret_cast<short*>(p);
		short const *pwCMP = reinterpret_cast<short const*>(pCMP);
		return atomic_rtm_cas(pvw, *pw, *pwCMP);
	}

	template <>
	inline void atomic_lock_load<4>(void const volatile *pv, void *p)
	{
		long volatile *pvl = reinterpret_cast<long volatile*>(const_cast<void volatile*>(pv));
		long *pl = reinterpret_cast<long*>(p);
		*pl = _InterlockedCompareExchange(pvl, 0, 0);
	}
	template <>
	inline void atomic_load<4>(void const volatile *pv, void *p)
	{
		long const volatile *pvl = reinterpret_cast<long const volatile*>(pv);
		long *pl = reinterpret_cast<long*>(p);
		*pl = *pvl;
	}
	template <>
	inline void atomic_lock_store<4>(void volatile *pv, void const *p)
	{
		long volatile *pvl = reinterpret_cast<long volatile*>(pv);
		long const *pl = reinterpret_cast<long const*>(p);
		_InterlockedExchange(pvl, *pl);
	}
	template <>
	inline void atomic_store<4>(void volatile *pv, void const *p)
	{
		long volatile *pvl = reinterpret_cast<long volatile*>(pv);
		long const *pl = reinterpret_cast<long const*>(p);
		*pvl = *pl;
	}
	template <>
	inline void atomic_lock_swap<4>(void volatile *pv, void *p)
	{
		long volatile *pvl = reinterpret_cast<long volatile*>(pv);
		long *pl = reinterpret_cast<long*>(p);
		*pl = _InterlockedExchange(pvl, *pl);
	}
	template <>
	inline void atomic_swap<4>(void volatile *pv, void *p)
	{
		long volatile *pvl = reinterpret_cast<long volatile*>(pv);
		long *pl = reinterpret_cast<long*>(p);
		atomic_rtm_swap(pvl, *pl);
	}
	template <>
	inline bool atomic_lock_cas<4>(void volatile *pv, void *p, void const *pCMP)
	{
		long volatile *pvl = reinterpret_cast<long volatile*>(pv);
		long *pl = reinterpret_cast<long*>(p);
		long const lCMP = *reinterpret_cast<long const*>(pCMP);
		long l = _InterlockedCompareExchange(pvl, *pl, lCMP);
		{
			if (l == lCMP)
			{
				*pl = l;
				return true;
			}
			else
				return false;
		}
	}
	template <>
	inline bool atomic_cas<4>(void volatile *pv, void *p, void const *pCMP)
	{
		long volatile *pvl = reinterpret_cast<long volatile*>(pv);
		long *pl = reinterpret_cast<long*>(p);
		long const *plCMP = reinterpret_cast<long const*>(pCMP);
		return atomic_rtm_cas(pvl, *pl, *plCMP);
	}

	template <>
	inline bool atomic_lock_cas<8>(void volatile *pv, void *p, void const *pCMP)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long *pxl = reinterpret_cast<long long*>(p);
		long long const xlCMP = *reinterpret_cast<long long const*>(pCMP);
		long long xl = _InterlockedCompareExchange64(pvxl, *pxl, xlCMP);
		{
			if (xl == xlCMP)
			{
				*pxl = xl;
				return true;
			}
			else
				return false;
		}
	}
	template <>
	inline bool atomic_cas<8>(void volatile *pv, void *p, void const *pCMP)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long *pxl = reinterpret_cast<long long*>(p);
		long long const *pxlCMP = reinterpret_cast<long long const*>(pCMP);
		return atomic_rtm_cas(pvxl, *pxl, *pxlCMP);
	}
#ifdef _M_X64
	template <>
	inline void atomic_lock_load<8>(void const volatile *pv, void *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(const_cast<void volatile*>(pv));
		long long *pxl = reinterpret_cast<long long*>(p);
		*pxl = _InterlockedCompareExchange64(pvxl, 0, 0);
	}
	template <>
	inline void atomic_load<8>(void const volatile *pv, void *p)
	{
		long long const volatile *pvxl = reinterpret_cast<long long const volatile*>(pv);
		long long *pxl = reinterpret_cast<long long*>(p);
		*pxl = *pvxl;
	}
	template <>
	inline void atomic_lock_store<8>(void volatile *pv, void const *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long const *pxl = reinterpret_cast<long long const*>(p);
		_InterlockedExchange64(pvxl, *pxl);
	}
	template <>
	inline void atomic_store<8>(void volatile *pv, void const *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long const *pxl = reinterpret_cast<long long const*>(p);
		*pvxl = *pxl;
	}
	template <>
	inline void atomic_lock_swap<8>(void volatile *pv, void *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long *pxl = reinterpret_cast<long long*>(p);
		*pxl = _InterlockedExchange64(pvxl, *pxl);
	}
	template <>
	inline void atomic_swap<8>(void volatile *pv, void *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long *pxl = reinterpret_cast<long long*>(p);
		atomic_rtm_swap(pvxl, *pxl);
	}
	template <>
	inline void atomic_lock_load<16>(void const volatile *pv, void *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(const_cast<void volatile*>(pv));
		long long *pxl = reinterpret_cast<long long*>(p);
		_InterlockedCompareExchange128(pvxl, pxl[1], pxl[0], pxl);
	}
	template <>
	inline void atomic_load<16>(void const volatile *pv, void *p)
	{
		__m128i const volatile *pvst = reinterpret_cast<__m128i const volatile*>(pv);
		__m128i *pst = reinterpret_cast<__m128i*>(p);
		*pst = atomic_rtm_load(pvst);
	}
	template <>
	inline void atomic_lock_store<16>(void volatile *pv, void const *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long const *pxl = reinterpret_cast<long long const*>(p);

		long long const xl0 = pxl[0];
		long long const xl1 = pxl[1];

		long long axl[2];
		do
		{
			axl[0] = pvxl[0];
			axl[1] = pvxl[1];
		} while (_InterlockedCompareExchange128(pvxl, xl1, xl0, axl) == 0);
	}
	template <>
	inline void atomic_store<16>(void volatile *pv, void const *p)
	{
		__m128i volatile *pvst = reinterpret_cast<__m128i volatile*>(pv);
		__m128i const *pst = reinterpret_cast<__m128i const*>(p);
		atomic_rtm_store(pvst, *pst);
	}
	template <>
	inline void atomic_lock_swap<16>(void volatile *pv, void *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long *pxl = reinterpret_cast<long long*>(p);
		long long axl[2];
		do
		{
			axl[0] = pvxl[0];
			axl[1] = pvxl[1];
		} while (_InterlockedCompareExchange128(pvxl, pxl[1], pxl[0], axl) == 0);
		pxl[0] = axl[0];
		pxl[1] = axl[1];
	}
	template <>
	inline void atomic_swap<16>(void volatile *pv, void *p)
	{
		__m128i volatile *pvst = reinterpret_cast<__m128i volatile*>(pv);
		__m128i *pst = reinterpret_cast<__m128i*>(p);
		atomic_rtm_swap(pvst, *pst);
	}
	template <>
	inline bool atomic_lock_cas<16>(void volatile *pv, void *p, void const *pCMP)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long *pxl = reinterpret_cast<long long*>(p);
		long long const *pxlCMP = reinterpret_cast<long long const*>(pCMP);
		long long axl[2];
		{
			axl[0] = pxlCMP[0];
			axl[1] = pxlCMP[1];
		}
		if (_InterlockedCompareExchange128(pvxl, pxl[1], pxl[0], axl))
		{
			pxl[0] = axl[0];
			pxl[1] = axl[1];
			return true;
		}
		else
			return false;
	}
	template <>
	inline bool atomic_cas<16>(void volatile *pv, void *p, void const *pCMP)
	{
		__m128i volatile *pvst = reinterpret_cast<__m128i volatile*>(pv);
		__m128i *pst = reinterpret_cast<__m128i*>(p);
		__m128i const *pstCMP = reinterpret_cast<__m128i const*>(pCMP);
		return atomic_rtm_cas(pvst, *pst, *pstCMP);
	}
#else
	template <>
	inline void atomic_lock_load<8>(void const volatile *pv, void *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(const_cast<void volatile*>(pv));
		long long *pxl = reinterpret_cast<long long*>(p);
		*pxl = _InterlockedCompareExchange64(pvxl, 0, 0);
	}
	template <>
	inline void atomic_load<8>(void const volatile *pv, void *p)
	{
		long long const volatile *pvxl = reinterpret_cast<long long const volatile*>(pv);
		long long *pxl = reinterpret_cast<long long*>(p);
		*pxl = atomic_rtm_load(pvxl);
	}
	template <>
	inline void atomic_lock_store<8>(void volatile *pv, void const *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long const *pxl = reinterpret_cast<long long const*>(p);

		long long const xl0 = *pxl;
		long long xl1;
		do
		{
			xl1 = *pvxl;
		} while (_InterlockedCompareExchange64(pvxl, xl0, xl1) != xl1);
	}
	template <>
	inline void atomic_store<8>(void volatile *pv, void const *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long const *pxl = reinterpret_cast<long long const*>(p);
		atomic_rtm_store(pvxl, *pxl);
	}
	template <>
	inline void atomic_lock_swap<8>(void volatile *pv, void *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long *pxl = reinterpret_cast<long long*>(p);
		long long xl;
		do
		{
			xl = *pvxl;
		} while (_InterlockedCompareExchange64(pvxl, *pxl, xl) != xl);
		*pxl = xl;
	}
	template <>
	inline void atomic_swap<8>(void volatile *pv, void *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long *pxl = reinterpret_cast<long long*>(p);
		atomic_rtm_swap(pvxl, *pxl);
	}
#endif

	template <typename T>
	inline T atomic_lock_load(T const volatile *pvt)
	{
		char ac[sizeof(T)];
		{
			atomic_lock_load<sizeof(T)>(pvt, ac);
		}
		return reinterpret_cast<T const&>(ac);
	}
	template <typename T>
	inline T atomic_load(T const volatile *pvt)
	{
		char ac[sizeof(T)];
		{
			atomic_load<sizeof(T)>(pvt, ac);
		}
		return reinterpret_cast<T const&>(ac);
	}
	template <typename T>
	inline std::enable_if_t<std::is_pod<T>::value, void> atomic_lock_store(T volatile *pvt, T const & t)
	{
		atomic_lock_store<sizeof(T)>(pvt, std::addressof(t));
	}
	template <typename T>
	inline std::enable_if_t<!std::is_pod<T>::value, void> atomic_lock_store(T volatile *pvt, T t)
	{
		atomic_lock_swap<sizeof(T)>(pvt, std::addressof(t));
	}
	template <typename T>
	inline std::enable_if_t<std::is_pod<T>::value, void> atomic_store(T volatile *pvt, T const & t)
	{
		atomic_store<sizeof(T)>(pvt, std::addressof(t));
	}
	template <typename T>
	inline std::enable_if_t<!std::is_pod<T>::value, void> atomic_store(T volatile *pvt, T t)
	{
		atomic_swap<sizeof(T)>(pvt, std::addressof(t));
	}
	template <typename T>
	inline T atomic_exchange(T volatile *pvt, T t)
	{
		atomic_swap<sizeof(T)>(pvt, std::addressof(t));
		return std::move(t);
	}
	template <typename T, typename U>
	inline T atomic_exchange(T volatile *pvt, U u)
	{
		return std::move(atomic_exchange(pvt, T(u)));
	}
	template <typename T>
	inline void atomic_lock_swap(T volatile *pvt, T & t)
	{
		atomic_lock_swap<sizeof(T)>(pvt, std::addressof(t));
	}
	template <typename T>
	inline void atomic_swap(T volatile *pvt, T & t)
	{
		atomic_swap<sizeof(T)>(pvt, std::addressof(t));
	}
	template <typename T, typename U>
	inline std::enable_if_t<sizeof(T) == sizeof(U), bool> atomic_lock_cas(T volatile *pvt, T & t, U const & uCMP)
	{
		return atomic_lock_cas<sizeof(T)>(pvt, std::addressof(t), std::addressof(uCMP));
	}
	template <typename T, typename U>
	inline std::enable_if_t<sizeof(T) == sizeof(U), bool> atomic_cas(T volatile *pvt, T & t, U const & uCMP)
	{
		return atomic_cas<sizeof(T)>(pvt, std::addressof(t), std::addressof(uCMP));
	}
}
