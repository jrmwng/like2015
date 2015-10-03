#pragma once

#include <intrin.h>
#include <stdint.h>
#include <algorithm>
#include <numeric>
#include "atomic.h"

namespace like
{
	struct shared_obj_lock
	{
		static thread_local unsigned g_uIndex;

		enum
		{
			SHARED_OBJ_LOCK_SIZE = 16,
		};

		__m128i xmmShared;
		__m128i xmmStatic;

		shared_obj_lock(void)
			: xmmShared(_mm_setzero_si128())
			, xmmStatic(_mm_setzero_si128())
		{}

		unsigned read_begin(void)
		{
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
						if (xmmShared.m128i_u8[uIndex % SHARED_OBJ_LOCK_SIZE] == 0 &&
							atomic_exchange(&static_cast<__m128i volatile&>(xmmShared).m128i_u8[uIndex % SHARED_OBJ_LOCK_SIZE], 0x80) == 0)
						{
							g_uIndex = uIndex % SHARED_OBJ_LOCK_SIZE;
							return uIndex % SHARED_OBJ_LOCK_SIZE;
						}
					}
					_mm_pause();
				}
			}
		}
		void read_end(unsigned uIndex)
		{
			if (uIndex >= SHARED_OBJ_LOCK_SIZE)
				atomic_store<uint8_t>(&(xmmStatic.m128i_u8[uIndex % SHARED_OBJ_LOCK_SIZE]), 0);
			else
				atomic_store<uint8_t>(&(xmmShared.m128i_u8[uIndex % SHARED_OBJ_LOCK_SIZE]), 0);
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

	template <typename TObj>
	class shared_obj
		: public TObj
	{
		typedef shared_obj<TObj> this_type;

		shared_obj_lock mutable m_Lock;
	public:
		template <class... TArgs>
		shared_obj(TArgs &&... _Args)
			: TObj(std::forward<TArgs>(_Args)...)
		{}

		template <typename TFunc>
		void load(TFunc tFunc) const
		{
			unsigned uKey = m_Lock.read_begin();

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
			TObj tObj;
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
			atomic_swap(static_cast<TObj volatile*>(this), std::addressof(that));
			
			if (that) // TODO
				m_Lock.read_sync();
		}

		template <typename TCompare>
		typename std::enable_if<sizeof(TObj)==sizeof(TCompare),bool>::type cas(TObj & that, TCompare const & tCompare)
		{
			if (atomic_cas(static_cast<TObj volatile*>(this), that, tCompare))
			{
				if (that) // TODO
					m_Lock.read_sync();
				return true;
			}
			else
				return false;
		}
		
		typename std::enable_if<std::is_default_constructible<TObj>::value, bool>::type cas(TObj & that)
		{
			return cas(that, TObj());
		}

		template <typename TCompare>
		typename std::enable_if<sizeof(TObj) == sizeof(TCompare), bool>::type cmov(TObj && that, TCompare const & tCompare)
		{
			return cas(TObj(std::forward<TObj>(that)), tCompare);
		}
		typename std::enable_if<std::is_default_constructible<TObj>::value, bool>::type cmov(TObj && that)
		{
			return cas(TObj(std::forward<TObj>(that)), TObj());
		}

		template <class... TArgs>
		void reset(TArgs &&... _Args)
		{
			swap(TObj(std::forward<TArgs>(_Args)...));
		}
		
		typename this_type & operator = (TObj const & that)
		{
			swap(TObj(that));
			return *this;
		}
		typename this_type & operator = (TObj && that)
		{
			swap(TObj(std::forward<TObj>(that)));
			return *this;
		}
	};
}