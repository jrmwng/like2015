#pragma once

/* Author: jrmwng @ 2015~2016 */

#include <intrin.h>
#include <stdint.h>
#include <algorithm>
#include <numeric>
#include "atomic.h"

namespace jrmwng
{
	struct alignas(32) shared_obj_lock
	{
		static thread_local unsigned g_uIndex;

		enum
		{
			SHARED_OBJ_LOCK_SIZE = 16,
		};

		__m128i xmmShared;
		__m128i xmmStatic;

		shared_obj_lock(void)
			: xmmShared(_mm_cvtsi32_si128(0x81))
			, xmmStatic(_mm_setzero_si128())
		{}

		unsigned read_begin(void)
		{
#ifdef RTM
			unsigned const uStatus = _xbegin();

			if (uStatus == _XBEGIN_STARTED)
			{
				int const nShared = _mm_movemask_epi8(xmmShared); // read-set: xmmShared
				int const nStatic = _mm_movemask_epi8(xmmStatic); // read-set: xmmShared, xmmStatic

				if (nShared == 1 && nStatic == 0)
					return 0; // only if no outstanding non-transaction
				else
					_xabort(0xFF);
			}
#endif

			unsigned uIndex = g_uIndex;

			if (uIndex >= SHARED_OBJ_LOCK_SIZE)
			{
				atomic_store<uint8_t>(&(xmmStatic.m128i_u8[uIndex % SHARED_OBJ_LOCK_SIZE]), 0x80);
				return uIndex;
			}
			else
			{
				for (;;)
				{
					for (unsigned i = 0; i < SHARED_OBJ_LOCK_SIZE; i++, uIndex++)
					{
						if (xmmShared.m128i_u8[uIndex % SHARED_OBJ_LOCK_SIZE] == 0)
						{
							if (atomic_exchange(&static_cast<__m128i volatile&>(xmmShared).m128i_u8[uIndex % SHARED_OBJ_LOCK_SIZE], 0x80) == 0)
							{
								g_uIndex = uIndex % SHARED_OBJ_LOCK_SIZE;
								return uIndex % SHARED_OBJ_LOCK_SIZE;
							}
						}
					}
					_mm_pause();
				}
			}
		}
		void read_end(unsigned uIndex)
		{
#ifdef RTM
			if (uIndex == 0)
			{
				_xend();
			}
			else
#endif
			{
				if (uIndex >= SHARED_OBJ_LOCK_SIZE)
					atomic_store<uint8_t>(&(xmmStatic.m128i_u8[uIndex % SHARED_OBJ_LOCK_SIZE]), 0);
				else
					atomic_store<uint8_t>(&(xmmShared.m128i_u8[uIndex % SHARED_OBJ_LOCK_SIZE]), 0);
			}
		}
		void read_sync(void)
		{
			for (int nShared = _mm_movemask_epi8(xmmShared), nStatic = _mm_movemask_epi8(xmmStatic); nShared || nStatic; nShared &= _mm_movemask_epi8(xmmShared), nStatic &= _mm_movemask_epi8(xmmStatic))
			{
				_mm_pause();
			}
		}

		static __m128i g_xmmLock;
		static void release(void)
		{
			unsigned uIndex = g_uIndex;

			if (uIndex >= SHARED_OBJ_LOCK_SIZE)
			{
				g_uIndex = uIndex - SHARED_OBJ_LOCK_SIZE;
			}
		}
		static bool acquire(void)
		{
			unsigned uIndex = g_uIndex;

			if (uIndex >= SHARED_OBJ_LOCK_SIZE)
			{
				g_uIndex = uIndex + SHARED_OBJ_LOCK_SIZE;
				return true;
			}
			else
			{
				for (unsigned i = 0; i < SHARED_OBJ_LOCK_SIZE; i++, uIndex++)
				{
					if (g_xmmLock.m128i_u8[uIndex % SHARED_OBJ_LOCK_SIZE] == 0 &&
						atomic_exchange(&static_cast<__m128i volatile&>(g_xmmLock).m128i_u8[uIndex % SHARED_OBJ_LOCK_SIZE], 0x80) == 0)
					{
						g_uIndex = (uIndex % SHARED_OBJ_LOCK_SIZE) + SHARED_OBJ_LOCK_SIZE;
						return true;
					}
				}
				return false;
			}
		}
	};
	thread_local unsigned shared_obj_lock::g_uIndex = 0;
	__m128i shared_obj_lock::g_xmmLock = _mm_setzero_si128();

	template <typename TObj, typename TSync = std::identity<TObj>>
	class shared_obj
		: public TObj
	{
		shared_obj_lock mutable m_Lock;
	public:
		template <class... TArgs>
		shared_obj(TArgs &&... _Args)
			: TObj(std::forward<TArgs>(_Args)...)
		{}

		template <typename TFunc>
		void load(TFunc tFunc) const
		{
#ifdef RTM
			unsigned const uStatus = _xbegin();

			if (uStatus == _XBEGIN_STARTED)
			{
				tFunc(*static_cast<TObj const*>(this));
				_xend();
				return;
			}
#endif
			unsigned const uKey = m_Lock.read_begin();

			char ac[sizeof(TObj)];
			{
				atomic_lock_load<sizeof(TObj)>(static_cast<TObj const*>(this), ac); // with acquire semantics
			}
			tFunc(reinterpret_cast<TObj const&>(ac));

			_mm_sfence(); // not perfect, but sufficient
			m_Lock.read_end(uKey);
		}

		TObj load(void) const
		{
			register TObj tObj;
			{
				load([&](TObj const & that)
				{
					tObj = that;
				});
			}
			return std::move(tObj);
		}

		void swap(TObj & that)
		{
#if 0 // this is an invalid RTM application, because *this could be read by another thread before the memory transaction
			unsigned const uStatus = _xbegin();

			if (uStatus == _XBEGIN_STARTED)
			{
				std::swap<TObj>(*this, that);
				_xend();
				return;
			}
#endif
			atomic_lock_swap<sizeof(TObj)>(static_cast<TObj volatile*>(this), std::addressof(that));

			if (TSync()(that))
				m_Lock.read_sync();
		}

		template <typename TCompare>
		std::enable_if_t<sizeof(TObj) == sizeof(TCompare), bool> cas(TObj & that, TCompare const & tCompare)
		{
#if 0 // this is an invalid RTM application, because *this could be read by another thread before the memory transaction
			unsigned const uStatus = _xbegin();

			if (uStatus == _XBEGIN_STARTED)
			{
				// TODO: CMPXCHG
				bool const bRET = atomic_lock_cas(static_cast<TObj volatile*>(this), that, tCompare);
				_xend();
				return bRET;
			}
#endif
			if (atomic_lock_cas<sizeof(TObj)>(static_cast<TObj volatile*>(this), std::addressof(that), std::addressof(tCompare)))
			{
				if (TSync()(that))
					m_Lock.read_sync();
				return true;
			}
			else
				return false;
		}

		std::enable_if_t<std::is_default_constructible<TObj>::value, bool> cas(TObj & that)
		{
			return cas(that, TObj());
		}

		template <typename TCompare>
		std::enable_if_t<sizeof(TObj) == sizeof(TCompare), bool> cmov(TObj && that, TCompare const & tCompare)
		{
			return cas(TObj(std::forward<TObj>(that)), tCompare);
		}
		std::enable_if_t<std::is_default_constructible<TObj>::value, bool> cmov(TObj && that)
		{
			return cas(TObj(std::forward<TObj>(that)), TObj());
		}

		template <class... TArgs>
		void reset(TArgs &&... _Args)
		{
			swap(TObj(std::forward<TArgs>(_Args)...));
		}

		typename shared_obj & operator = (TObj const & that)
		{
			swap(TObj(that));
			return *this;
		}
		template <typename T1>
		typename shared_obj & operator = (T1 const & that)
		{
			swap(TObj(that));
			return *this;
		}
		typename shared_obj & operator = (TObj && that)
		{
			swap(TObj(std::forward<TObj>(that)));
			return *this;
		}
		template <typename T1>
		typename shared_obj & operator = (T1 && that)
		{
			swap(TObj(std::forward<T1>(that)));
			return *this;
		}
	};
}
