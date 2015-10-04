#pragma once

#include <intrin.h>
#include <type_traits>

#define TSX // TODO

namespace like
{
	template <unsigned uSize>
	void atomic_lock_load(void const volatile *pv, void *p);
	template <unsigned uSize>
	void atomic_lock_store(void volatile *pv, void const *p);
	template <unsigned uSize>
	void atomic_lock_swap(void volatile *pv, void *p);
	template <unsigned uSize>
	bool atomic_lock_cas(void volatile *pv, void *p, void const *pCMP);

	template <typename T>
	void atomic_swap(T volatile *pvt, T *pt)
	{
#ifdef TSX
		register T const t0 = *pt;
		register unsigned const uStatus = _xbegin();

		if (uStatus == _XBEGIN_STARTED)
		{
			register T const t1 = *const_cast<T*>(pvt);
			*const_cast<T*>(pvt) = t0;
			_xend();
			*pt = t1;
		}
		else
#endif // TSX
		{
			atomic_lock_swap<sizeof(T)>(pvt, pt);
		}
	}
	template <typename T>
	bool atomic_cas(T volatile *pvt, T *pt, T const *ptCMP)
	{
#ifdef TSX
		register T const tCMP = *reinterpret_cast<T const*>(ptCMP);
		register T const t0 = *pt;
		register unsigned const uStatus = _xbegin();

		if (uStatus == _XBEGIN_STARTED)
		{
			register T const t1 = *const_cast<T*>(pvt); // read pvt
			if (memcmp(const_cast<T const*>(pvt), ptCMP, sizeof(T)) == 0)
			{
				*const_cast<T*>(pvt) = t0; // write pvt
				_xend();
				*pt = t1;
				return true;
			}
			else
			{
				_xend();
				return false;
			}
		}
		else
#endif
		{
			return atomic_lock_cas<sizeof(T)>(pvt, pt, ptCMP);
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
	void atomic_lock_load<1>(void const volatile *pv, void *p)
	{
		char volatile *pvb = reinterpret_cast<char volatile*>(const_cast<void volatile*>(pv));
		char *pb = reinterpret_cast<char*>(p);
		*pb = _InterlockedCompareExchange8(pvb, 0, 0);
	}
	template <>
	void atomic_load<1>(void const volatile *pv, void *p)
	{
		char const volatile *pvb = reinterpret_cast<char const volatile*>(pv);
		char *pb = reinterpret_cast<char*>(p);
		*pb = *pvb;
	}
	template <>
	void atomic_lock_store<1>(void volatile *pv, void const *p)
	{
		char volatile *pvb = reinterpret_cast<char volatile*>(pv);
		char const *pb = reinterpret_cast<char const*>(p);
		_InterlockedExchange8(pvb, *pb);
	}
	template <>
	void atomic_store<1>(void volatile *pv, void const *p)
	{
		char volatile *pvb = reinterpret_cast<char volatile*>(pv);
		char const *pb = reinterpret_cast<char const*>(p);
		*pvb = *pb;
	}
	template <>
	void atomic_lock_swap<1>(void volatile *pv, void *p)
	{
		char volatile *pvb = reinterpret_cast<char volatile*>(pv);
		char *pb = reinterpret_cast<char*>(p);
		*pb = _InterlockedExchange8(pvb, *pb);
	}
	template <>
	void atomic_swap<1>(void volatile *pv, void *p)
	{
		char volatile *pvb = reinterpret_cast<char volatile*>(pv);
		char *pb = reinterpret_cast<char*>(p);
		atomic_swap(pvb, pb);
	}
	template <>
	bool atomic_lock_cas<1>(void volatile *pv, void *p, void const *pCMP)
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
	bool atomic_cas<1>(void volatile *pv, void *p, void const *pCMP)
	{
		char volatile *pvb = reinterpret_cast<char volatile*>(pv);
		char *pb = reinterpret_cast<char*>(p);
		char const *pbCMP = reinterpret_cast<char const*>(pCMP);
		return atomic_cas(pvb, pb, pbCMP);
	}

	template <>
	void atomic_lock_load<2>(void const volatile *pv, void *p)
	{
		short volatile *pvw = reinterpret_cast<short volatile*>(const_cast<void volatile*>(pv));
		short *pw = reinterpret_cast<short*>(p);
		*pw = _InterlockedCompareExchange16(pvw, 0, 0);
	}
	template <>
	void atomic_load<2>(void const volatile *pv, void *p)
	{
		short const volatile *pvw = reinterpret_cast<short const volatile*>(pv);
		short *pw = reinterpret_cast<short*>(p);
		*pw = *pvw;
	}
	template <>
	void atomic_lock_store<2>(void volatile *pv, void const *p)
	{
		short volatile *pvw = reinterpret_cast<short volatile*>(pv);
		short const *pw = reinterpret_cast<short const*>(p);
		_InterlockedExchange16(pvw, *pw);
	}
	template <>
	void atomic_store<2>(void volatile *pv, void const *p)
	{
		short volatile *pvw = reinterpret_cast<short volatile*>(pv);
		short const *pw = reinterpret_cast<short const*>(p);
		*pvw = *pw;
	}
	template <>
	void atomic_lock_swap<2>(void volatile *pv, void *p)
	{
		short volatile *pvw = reinterpret_cast<short volatile*>(pv);
		short *pw = reinterpret_cast<short*>(p);
		*pw = _InterlockedExchange16(pvw, *pw);
	}
	template <>
	void atomic_swap<2>(void volatile *pv, void *p)
	{
		short volatile *pvw = reinterpret_cast<short volatile*>(pv);
		short *pw = reinterpret_cast<short*>(p);
		atomic_swap(pvw, pw);
	}
	template <>
	bool atomic_lock_cas<2>(void volatile *pv, void *p, void const *pCMP)
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
	bool atomic_cas<2>(void volatile *pv, void *p, void const *pCMP)
	{
		short volatile *pvw = reinterpret_cast<short volatile*>(pv);
		short *pw = reinterpret_cast<short*>(p);
		short const *pwCMP = reinterpret_cast<short const*>(pCMP);
		return atomic_cas(pvw, pw, pwCMP);
	}

	template <>
	void atomic_lock_load<4>(void const volatile *pv, void *p)
	{
		long volatile *pvl = reinterpret_cast<long volatile*>(const_cast<void volatile*>(pv));
		long *pl = reinterpret_cast<long*>(p);
		*pl = _InterlockedCompareExchange(pvl, 0, 0);
	}
	template <>
	void atomic_load<4>(void const volatile *pv, void *p)
	{
		long const volatile *pvl = reinterpret_cast<long const volatile*>(pv);
		long *pl = reinterpret_cast<long*>(p);
		*pl = *pvl;
	}
	template <>
	void atomic_lock_store<4>(void volatile *pv, void const *p)
	{
		long volatile *pvl = reinterpret_cast<long volatile*>(pv);
		long const *pl = reinterpret_cast<long const*>(p);
		_InterlockedExchange(pvl, *pl);
	}
	template <>
	void atomic_store<4>(void volatile *pv, void const *p)
	{
		long volatile *pvl = reinterpret_cast<long volatile*>(pv);
		long const *pl = reinterpret_cast<long const*>(p);
		*pvl = *pl;
	}
	template <>
	void atomic_lock_swap<4>(void volatile *pv, void *p)
	{
		long volatile *pvl = reinterpret_cast<long volatile*>(pv);
		long *pl = reinterpret_cast<long*>(p);
		*pl = _InterlockedExchange(pvl, *pl);
	}
	template <>
	void atomic_swap<4>(void volatile *pv, void *p)
	{
		long volatile *pvl = reinterpret_cast<long volatile*>(pv);
		long *pl = reinterpret_cast<long*>(p);
		atomic_swap(pvl, pl);
	}
	template <>
	bool atomic_lock_cas<4>(void volatile *pv, void *p, void const *pCMP)
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
	bool atomic_cas<4>(void volatile *pv, void *p, void const *pCMP)
	{
		long volatile *pvl = reinterpret_cast<long volatile*>(pv);
		long *pl = reinterpret_cast<long*>(p);
		long const *plCMP = reinterpret_cast<long const*>(pCMP);
		return atomic_cas(pvl, pl, plCMP);
	}

	template <>
	bool atomic_lock_cas<8>(void volatile *pv, void *p, void const *pCMP)
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
	bool atomic_cas<8>(void volatile *pv, void *p, void const *pCMP)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long *pxl = reinterpret_cast<long long*>(p);
		long long const *pxlCMP = reinterpret_cast<long long const*>(pCMP);
		return atomic_cas(pvxl, pxl, pxlCMP);
	}
#ifdef _M_X64
	template <>
	void atomic_lock_load<8>(void const volatile *pv, void *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(const_cast<void volatile*>(pv));
		long long *pxl = reinterpret_cast<long long*>(p);
		*pxl = _InterlockedCompareExchange64(pvxl, 0, 0);
	}
	template <>
	void atomic_load<8>(void const volatile *pv, void *p)
	{
		long long const volatile *pvxl = reinterpret_cast<long long const volatile*>(pv);
		long long *pxl = reinterpret_cast<long long*>(p);
		*pxl = *pvxl;
	}
	template <>
	void atomic_lock_store<8>(void volatile *pv, void const *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long const *pxl = reinterpret_cast<long long const*>(p);
		_InterlockedExchange64(pvxl, *pxl);
	}
	template <>
	void atomic_store<8>(void volatile *pv, void const *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long const *pxl = reinterpret_cast<long long const*>(p);
		*pvxl = *pxl;
	}
	template <>
	void atomic_lock_swap<8>(void volatile *pv, void *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long *pxl = reinterpret_cast<long long*>(p);
		*pxl = _InterlockedExchange64(pvxl, *pxl);
	}
	template <>
	void atomic_swap<8>(void volatile *pv, void *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long *pxl = reinterpret_cast<long long*>(p);
		atomic_swap(pvxl, pxl);
	}
	template <>
	void atomic_lock_load<16>(void const volatile *pv, void *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(const_cast<void volatile*>(pv));
		long long *pxl = reinterpret_cast<long long*>(p);
		_InterlockedCompareExchange128(pvxl, pxl[1], pxl[0], pxl);
	}
	template <>
	void atomic_load<16>(void const volatile *pv, void *p)
	{
#ifdef TSX
		long long const volatile *pvxl = reinterpret_cast<long long const volatile*>(pv);
		long long *pxl = reinterpret_cast<long long*>(p);

		unsigned uStatus = _xbegin();

		if (uStatus == _XBEGIN_STARTED)
		{
			long long const xl0 = pvxl[0];
			long long const xl1 = pvxl[1];
			_xend();
			pxl[0] = xl0;
			pxl[1] = xl1;
		}
		else
#endif // TSX
		{
			atomic_lock_load<16>(pv, p);
		}
	}
	template <>
	void atomic_lock_store<16>(void volatile *pv, void const *p)
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
	void atomic_store<16>(void volatile *pv, void const *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long const *pxl = reinterpret_cast<long long const*>(p);

		register long long const xl0 = pxl[0];
		register long long const xl1 = pxl[1];
		register unsigned const uStatus = _xbegin();

		if (uStatus == _XBEGIN_STARTED)
		{
			pvxl[0] = xl0;
			pvxl[1] = xl1;
			_xend();
		}
		else
		{
			atomic_lock_store<16>(pv, p);
		}
	}
	template <>
	void atomic_lock_swap<16>(void volatile *pv, void *p)
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
	void atomic_swap<16>(void volatile *pv, void *p)
	{
		std::pair<long long, long long> volatile *pvst = reinterpret_cast<std::pair<long long, long long> volatile*>(pv);
		std::pair<long long, long long> *pst = reinterpret_cast<std::pair<long long, long long>*>(p);
		atomic_swap(pvst, pst);
	}
	template <>
	bool atomic_lock_cas<16>(void volatile *pv, void *p, void const *pCMP)
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
	bool atomic_cas<16>(void volatile *pv, void *p, void const *pCMP)
	{
		std::pair<long long, long long> volatile *pvst = reinterpret_cast<std::pair<long long, long long> volatile*>(pv);
		std::pair<long long, long long> *pst = reinterpret_cast<std::pair<long long, long long>*>(p);
		std::pair<long long, long long> const *pstCMP = reinterpret_cast<std::pair<long long, long long> const*>(pCMP);
		return atomic_cas(pvst, pst, pstCMP);
	}
#else
	template <>
	void atomic_lock_load<8>(void const volatile *pv, void *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(const_cast<void volatile*>(pv));
		long long *pxl = reinterpret_cast<long long*>(p);
		*pxl = _InterlockedCompareExchange64(pvxl, 0, 0);
	}
	template <>
	void atomic_load<8>(void const volatile *pv, void *p)
	{
		atomic_lock_load<8>(pv, p);
	}
	template <>
	void atomic_lock_store<8>(void volatile *pv, void const *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long const *pxl = reinterpret_cast<long long const*>(p);
		long long xl;
		do
		{
			xl = *pvxl;
		} while (_InterlockedCompareExchange64(pvxl, *pxl, xl) != xl);
	}
	template <>
	void atomic_store<8>(void volatile *pv, void const *p)
	{
		atomic_lock_store<8>(pv, p);
	}
	template <>
	void atomic_lock_swap<8>(void volatile *pv, void *p)
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
	void atomic_swap<8>(void volatile *pv, void *p)
	{
		long long volatile *pvxl = reinterpret_cast<long long volatile*>(pv);
		long long *pxl = reinterpret_cast<long long*>(p);
		atomic_swap(pvxl, pxl);
	}
#endif

	template <typename T>
	T atomic_lock_load(T const volatile *pvt)
	{
		char ac[sizeof(T)];
		{
			atomic_lock_load<sizeof(T)>(pvt, ac);
		}
		return reinterpret_cast<T const&>(ac);
	}
	template <typename T>
	T atomic_load(T const volatile *pvt)
	{
		char ac[sizeof(T)];
		{
			atomic_load<sizeof(T)>(pvt, ac);
		}
		return reinterpret_cast<T const&>(ac);
	}
	template <typename T>
	typename std::enable_if<std::is_pod<T>::value,void>::type atomic_lock_store(T volatile *pvt, T t)
	{
		atomic_lock_store<sizeof(T)>(pvt, std::addressof(t));
	}
	template <typename T>
	typename std::enable_if<!std::is_pod<T>::value,void>::type atomic_lock_store(T volatile *pvt, T t)
	{
		atomic_lock_swap<sizeof(T)>(pvt, std::addressof(t));
	}
	template <typename T>
	typename std::enable_if<std::is_pod<T>::value,void>::type atomic_store(T volatile *pvt, T t)
	{
		atomic_store<sizeof(T)>(pvt, std::addressof(t));
	}
	template <typename T>
	typename std::enable_if<!std::is_pod<T>::value,void>::type atomic_store(T volatile *pvt, T t)
	{
		atomic_swap<sizeof(T)>(pvt, std::addressof(t));
	}
	template <typename T>
	T atomic_exchange(T volatile *pvt, T t)
	{
		atomic_swap<sizeof(T)>(pvt, std::addressof(t));
		return std::forward<T>(t);
	}
	template <typename T, typename U>
	T atomic_exchange(T volatile *pvt, U u)
	{
		return std::move(atomic_exchange(pvt, T(u)));
	}
	template <typename T>
	typename std::enable_if<std::is_pod<T>::value, void>::type atomic_swap(T volatile *pvt, T & t)
	{
		atomic_swap<T>(pvt, std::addressof(t));
	}
	template <typename T>
	typename std::enable_if<!std::is_pod<T>::value, void>::type atomic_swap(T volatile *pvt, T & t)
	{
		atomic_swap<sizeof(T)>(pvt, std::addressof(t));
	}
	template <typename T>
	typename std::enable_if<std::is_pod<T>::value, bool>::type atomic_cas(T volatile *pvt, T & t, T const & tCMP)
	{
		return atomic_cas<T>(pvt, std::addressof(t), std::addressof(tCMP));
	}
	template <typename T>
	typename std::enable_if<!std::is_pod<T>::value, bool>::type atomic_cas(T volatile *pvt, T & t, T const & tCMP)
	{
		return atomic_cas<sizeof(T)>(pvt, std::addressof(t), std::addressof(tCMP));
	}
	template <typename T, typename U>
	typename std::enable_if<sizeof(T)==sizeof(U),bool>::type atomic_cas(T volatile *pvt, T & t, U const & uCMP)
	{
		return atomic_cas<sizeof(T)>(pvt, std::addressof(t), std::addressof(uCMP));
	}
}
