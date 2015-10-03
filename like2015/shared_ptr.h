#pragma once

#include <intrin.h>
#include <array>
#include <atomic>
#include "atomic.h"

namespace like
{
	template <typename T, typename TLock>
	class shared_ptr;
	template <typename T, typename TLock>
	class weak_ptr;
	template <typename T, typename TLock>
	class enable_shared_from_this;

	struct __declspec(align(16)) shared_ptr_lock
	{
		__declspec(align(16)) __m128i xmm;

		static __m128i g_xmmLockPool;
		static thread_local unsigned g_uLockIndex;
		static thread_local __m128i g_xmmDelta;

		static bool acquire(void)
		{
			static unsigned g_uTryIndex = 0;

			if (g_uLockIndex > 0)
			{
				g_uLockIndex += sizeof(uintptr_t);
				return true;
			}
			unsigned uIndex0 = g_uTryIndex;
			unsigned uIndex1;
			{
				for (uIndex1 = uIndex0; uIndex1 - uIndex0 < sizeof(uintptr_t); uIndex1++)
				{
					if (g_xmmLockPool.m128i_u8[uIndex1 % sizeof(uintptr_t)] != 0)
					{
						if (atomic_exchange<unsigned char>(&g_xmmLockPool.m128i_u8[uIndex1 % sizeof(uintptr_t)], 0) != 0)
						{
							g_uTryIndex = uIndex1 + 1;
							break;
						}

						_mm_pause(); // no hurry
					}
				}
			}
			if (uIndex1 - uIndex0 < sizeof(uintptr_t))
			{
				g_uLockIndex = uIndex1 % sizeof(uintptr_t);
				g_xmmDelta = _mm_cmpeq_epi16(_mm_set1_epi16(uIndex1 % sizeof(uintptr_t)), _mm_set_epi16(7, 6, 5, 4, 3, 2, 1, 0));
				return true;
			}
			else
				return false;
		}
		static void release(void)
		{
			_mm_sfence();

			if (g_uLockIndex >= sizeof(uintptr_t))
			{
				g_uLockIndex -= sizeof(uintptr_t);
				g_xmmDelta = _mm_cvtsi32_si128(0xFFFF);
				return;
			}

			unsigned uIndex = std::exchange(g_uLockIndex, 0);

			if (0 < uIndex && uIndex < sizeof(uintptr_t))
				g_xmmLockPool.m128i_u8[uIndex] = ~0;
		}

		shared_ptr_lock(void)
			: xmm(_mm_set_epi32(0x80808080, 0x80808080, 0x80808080, 0x80808181))
		{}

		virtual void delete_this(void) { __debugbreak(); }
		virtual void delete_that(void) { __debugbreak(); }

		long use_count(void) const
		{
			__m128i const w8BIN0 = xmm;
			__m128i const w8BIN1s = _mm_srli_epi16(w8BIN0, 8);
			__m128i const w8BIN2s = _mm_packus_epi16(w8BIN1s, _mm_setzero_si128());
			__m128i const w8SAD3s = _mm_sad_epu8(w8BIN2s, _mm_setzero_si128());
			int const nSAD4s = _mm_cvtsi128_si32(w8SAD3s);
			return nSAD4s - 0x80 * sizeof(uintptr_t);
		}
		bool unique(void) const
		{
			__m128i const w8BIN0 = xmm;
			__m128i const w8BIN1s = _mm_srli_epi16(w8BIN0, 8);
			__m128i const w8BIN1w = _mm_slli_epi16(w8BIN0, 8);
			__m128i const w8BIN2s = _mm_packus_epi16(w8BIN1s, _mm_setzero_si128());
			__m128i const w8SAD2w = _mm_sad_epu8(w8BIN1w, _mm_setzero_si128());
			__m128i const w8SAD3s = _mm_sad_epu8(w8BIN2s, _mm_setzero_si128());
			__m128i const w8SAD3w = _mm_srli_si128(w8SAD2w, 8);
			int const nSAD3w = _mm_cvtsi128_si32(w8SAD2w);
			int const nSAD4s = _mm_cvtsi128_si32(w8SAD3s);
			int const nSAD4w = _mm_cvtsi128_si32(w8SAD3w);
			return nSAD4s == 0x80 * sizeof(uintptr_t) + 1 && nSAD3w + nSAD4w == 0x80 * sizeof(uintptr_t) + 1;
		}
		bool expired(void) const
		{
			__m128i const w8BIN0 = xmm;
			__m128i const w8BIN1s = _mm_srli_epi16(w8BIN0, 8);
			__m128i const w8BIN2s = _mm_packus_epi16(w8BIN1s, _mm_setzero_si128());
			__m128i const w8SAD3s = _mm_sad_epu8(w8BIN2s, _mm_setzero_si128());
			int const nSAD4s = _mm_cvtsi128_si32(w8SAD3s);
			return nSAD4s == 0x80 * sizeof(uintptr_t);
		}

		void acquire_shared(void)
		{
			unsigned const uLockIndex = g_uLockIndex % sizeof(uintptr_t);

			if (0 < uLockIndex)
			{
				xmm.m128i_i16[uLockIndex] += 0x101;
			}
			else
			{
				_InterlockedExchangeAdd16(&xmm.m128i_i16[uLockIndex], 0x101);
			}
		}
		void acquire_weak(void)
		{
			unsigned const uLockIndex = g_uLockIndex % sizeof(uintptr_t);

			if (0 < uLockIndex)
			{
				xmm.m128i_i8[uLockIndex + uLockIndex]++;
			}
			else
			{
				_InterlockedExchangeAdd8(&xmm.m128i_i8[uLockIndex + uLockIndex], 1);
			}
		}
		void release_weak(void)
		{
			__m128i const w8Delta = _mm_srli_epi16(g_xmmDelta, 8);
			__m128i w8BIN0;
			__m128i w8BIN1;
			do
			{
				w8BIN0 = xmm;
				w8BIN1 = _mm_add_epi8(w8BIN0, w8Delta);
			} while (!atomic_cas<sizeof(uintptr_t)*2>(xmm.m128i_i8, w8BIN1.m128i_i8, w8BIN0.m128i_i8));

			__m128i const b16BIN1w = _mm_slli_epi16(w8BIN0, 8);
			__m128i const l4SAD2w = _mm_sad_epu8(b16BIN1w, _mm_setzero_si128());
			__m128i const l4SAD3w = _mm_srli_si128(l4SAD2w, 8);
			int const nSAD3w = _mm_cvtsi128_si32(l4SAD2w);
			int const nSAD4w = _mm_cvtsi128_si32(l4SAD3w);

			if (nSAD3w + nSAD4w == 0x80 * sizeof(uintptr_t) + 1)
			{
				delete_this();
			}
		}
		void release_shared(void)
		{
			__m128i const w8Delta = g_xmmDelta;
			__m128i w8BIN0;
			__m128i w8BIN1;
			do
			{
				w8BIN0 = xmm;
				w8BIN1 = _mm_add_epi8(w8BIN0, w8Delta);
			} while (!atomic_cas<sizeof(uintptr_t) * 2>(xmm.m128i_i8, w8BIN1.m128i_i8, w8BIN0.m128i_i8));

			__m128i const b16BIN1s = _mm_srli_epi16(w8BIN0, 8);
			__m128i const b16BIN1w = _mm_slli_epi16(w8BIN0, 8);
			__m128i const b16BIN2s = _mm_packus_epi16(b16BIN1s, _mm_setzero_si128());
			__m128i const l4SAD2w = _mm_sad_epu8(b16BIN1w, _mm_setzero_si128());
			__m128i const l4SAD3s = _mm_sad_epu8(b16BIN2s, _mm_setzero_si128());
			__m128i const l4SAD3w = _mm_srli_si128(l4SAD2w, 8);
			int const nSAD3w = _mm_cvtsi128_si32(l4SAD2w);
			int const nSAD4s = _mm_cvtsi128_si32(l4SAD3s);
			int const nSAD4w = _mm_cvtsi128_si32(l4SAD3w);

			if (nSAD4s == 0x80 * sizeof(uintptr_t) + 1) // delete_that() if it is the last shared_ptr
			{
				delete_that();

				if (nSAD3w + nSAD4w == 0x80 * sizeof(uintptr_t) + 1) // delete_this() if it is the last weak_ptr
				{
					delete_this();
				}
			}
		}
		bool try_shared(void)
		{
			__m128i const w8Delta = g_xmmDelta;
			__m128i w8BIN0;
			__m128i w8BIN1;
			int nSAD4s;
			do
			{
				w8BIN0 = xmm;
				w8BIN1 = _mm_sub_epi8(w8BIN0, w8Delta);
				__m128i const b16BIN1s = _mm_srli_epi16(w8BIN0, 8);
				__m128i const b16BIN2 = _mm_packus_epi16(b16BIN1s, _mm_setzero_si128());
				__m128i const l4SAD3 = _mm_sad_epu8(b16BIN2, _mm_setzero_si128());
				nSAD4s = _mm_cvtsi128_si32(l4SAD3);
			} while (nSAD4s > 0x80 * sizeof(uintptr_t) && !atomic_cas<sizeof(uintptr_t) * 2>(xmm.m128i_i8, w8BIN1.m128i_i8, w8BIN0.m128i_i8));

			return nSAD4s > 0x80 * sizeof(uintptr_t);
		}
		void convert_shared_into_weak(void)
		{
			unsigned const uLockIndex = g_uLockIndex % sizeof(uintptr_t);

			if (0 < uLockIndex)
			{
				xmm.m128i_i8[uLockIndex + uLockIndex + 1]--;
			}
			else
			{
				_InterlockedExchangeAdd8(&xmm.m128i_i8[uLockIndex + uLockIndex + 1], -1);
			}
		}
		bool convert_weak_into_shared(void)
		{
			__m128i const w8Delta = _mm_slli_epi16(g_xmmDelta, 8);
			__m128i w8BIN0;
			__m128i w8BIN1;
			int nSAD4s;
			do
			{
				w8BIN0 = xmm;
				w8BIN1 = _mm_sub_epi8(w8BIN0, w8Delta);
				__m128i const b16BIN1s = _mm_srli_epi16(w8BIN0, 8);
				__m128i const b16BIN2 = _mm_packus_epi16(b16BIN1s, _mm_setzero_si128());
				__m128i const l4SAD3 = _mm_sad_epu8(b16BIN2, _mm_setzero_si128());
				nSAD4s = _mm_cvtsi128_si32(l4SAD3);
			} while (nSAD4s > 0x80 * sizeof(uintptr_t) && !atomic_cas<sizeof(uintptr_t) * 2>(xmm.m128i_i8, w8BIN1.m128i_i8, w8BIN0.m128i_i8));

			if (nSAD4s > 0x80 * sizeof(uintptr_t))
			{
				return true;
			}
			else
			{
				release_weak();
				return false;
			}
		}
	};
	__m128i shared_ptr_lock::g_xmmLockPool = _mm_cmplt_epi8(_mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x0302017F), _mm_set1_epi8(sizeof(uintptr_t)));
	thread_local unsigned shared_ptr_lock::g_uLockIndex = 0;
	thread_local __m128i shared_ptr_lock::g_xmmDelta = _mm_cvtsi32_si128(0xFFFF);

	struct shared_ptr_count
	{
		typedef intptr_t count_type;
		std::atomic<count_type> lRefCount;

		enum
		{
			SHARED_PTR_COUNT_BASE = 1,
			SHARED_PTR_COUNT_THIS = SHARED_PTR_COUNT_BASE,
			SHARED_PTR_COUNT_THAT_SHIFT = 12 + sizeof(count_type), // 16 for long, 20 for long long
			SHARED_PTR_COUNT_THAT = (1 << SHARED_PTR_COUNT_THAT_SHIFT),
			SHARED_PTR_COUNT_BOTH = SHARED_PTR_COUNT_THIS + SHARED_PTR_COUNT_THAT,
			SHARED_PTR_COUNT_INIT = SHARED_PTR_COUNT_BASE + SHARED_PTR_COUNT_BOTH,
		};

		shared_ptr_count(void)
			: lRefCount(SHARED_PTR_COUNT_INIT)
		{}

		virtual void delete_this(void) { __debugbreak(); }
		virtual void delete_that(void) { __debugbreak(); }

		long use_count(void) const
		{
			return static_cast<long>(lRefCount >> SHARED_PTR_COUNT_THAT_SHIFT);
		}
		bool unique(void) const
		{
			return lRefCount == SHARED_PTR_COUNT_INIT; // unique without weak_ptr
		}
		bool expired(void) const
		{
			return lRefCount < SHARED_PTR_COUNT_THAT;
		}

		void release_shared(void)
		{
			count_type lRefCount0 = lRefCount.fetch_sub(SHARED_PTR_COUNT_BOTH, std::memory_order_release);

			if (lRefCount0 < SHARED_PTR_COUNT_BOTH + SHARED_PTR_COUNT_THAT)
			{
				delete_that(); // assume nothrow for simplicity

				if (lRefCount0 < SHARED_PTR_COUNT_BOTH + SHARED_PTR_COUNT_BASE + SHARED_PTR_COUNT_THIS
					||
					lRefCount.fetch_sub(SHARED_PTR_COUNT_BASE, std::memory_order_release) < SHARED_PTR_COUNT_BASE + SHARED_PTR_COUNT_THIS
					)
				{
					delete_this();
				}
			}
		}
		void release_weak(void)
		{
			count_type lRefCount0 = lRefCount.fetch_sub(SHARED_PTR_COUNT_THIS, std::memory_order_release);

			if (lRefCount0 < SHARED_PTR_COUNT_THIS + SHARED_PTR_COUNT_BASE)
				delete_this();
		}
		void acquire_weak(void)
		{
			lRefCount.fetch_add(SHARED_PTR_COUNT_THIS, std::memory_order_acquire);
		}
		void acquire_shared(void)
		{
			lRefCount.fetch_add(SHARED_PTR_COUNT_BOTH, std::memory_order_acquire);
		}
		bool try_shared(void)
		{
			count_type lRefCount0;
			count_type lRefCount1;
			{
				for (lRefCount0 = lRefCount; lRefCount0 >= SHARED_PTR_COUNT_THAT && !lRefCount.compare_exchange_weak(lRefCount0, lRefCount1 = lRefCount0 + SHARED_PTR_COUNT_BOTH, std::memory_order_acquire); lRefCount0 = lRefCount)
				{
					_mm_pause();
				}
			}
			return lRefCount0 >= SHARED_PTR_COUNT_THAT;
		}
		void convert_shared_into_weak(void)
		{
			count_type lRefCount0 = lRefCount.fetch_sub(SHARED_PTR_COUNT_BOTH - SHARED_PTR_COUNT_THIS, std::memory_order_release);

			if (lRefCount0 < (SHARED_PTR_COUNT_BOTH - SHARED_PTR_COUNT_THIS) + SHARED_PTR_COUNT_THAT)
				delete_that();
		}
		bool convert_weak_into_shared(void)
		{
			count_type lRefCount0;
			count_type lRefCount1;
			{
				for (lRefCount0 = lRefCount; lRefCount0 >= SHARED_PTR_COUNT_THAT && !lRefCount.compare_exchange_weak(lRefCount0, lRefCount1 = lRefCount0 + SHARED_PTR_COUNT_BOTH - SHARED_PTR_COUNT_THIS, std::memory_order_acquire); lRefCount0 = lRefCount)
				{
					_mm_pause();
				}
			}
			if (lRefCount0 >= SHARED_PTR_COUNT_THAT)
				return true;
			else
			{
				release_weak();
				return false;
			}
		}
	};

	template <typename T, typename TLock, typename TDeleter = void>
	struct shared_ptr_count_t
		: std::array<char,sizeof(TDeleter)>
		, TLock
	{
		void * operator new (size_t uByteCount)
		{
			return _aligned_malloc(uByteCount, std::max<size_t>(alignof(TDeleter), alignof(TLock)));
		}
		void operator delete (void *p)
		{
			_aligned_free(p);
		}

		TDeleter * get_deleter(void) { return reinterpret_cast<TDeleter*>(static_cast<std::array<char,sizeof(TDeleter)>*>(this)); }

		T * const m_pt;

		shared_ptr_count_t(T *pt, TDeleter const & tDeleter)
			: m_pt(pt)
		{
			::new (get_deleter()) TDeleter(tDeleter);
		}
		shared_ptr_count_t(T *pt, TDeleter && tDeleter)
			: m_pt(pt)
		{
			::new (get_deleter()) TDeleter(std::forward<TDeleter>(tDeleter));
		}
		~shared_ptr_count_t(void)
		{
			get_deleter()->~TDeleter();
		}

		virtual void delete_this(void)
		{
			delete this;
		}
		virtual void delete_that(void)
		{
			(*get_deleter())(m_pt);
		}
	};
	template <typename T, typename TLock>
	struct shared_ptr_count_t<T,TLock,void>
		: TLock
	{
		void * operator new (size_t uByteCount)
		{
			return _aligned_malloc(uByteCount, alignof(TLock));
		}
		void operator delete (void *p)
		{
			_aligned_free(p);
		}

		T * const m_pt;

		shared_ptr_count_t(T *pt)
			: m_pt(pt)
		{}

		virtual void delete_this(void)
		{
			delete this;
		}
		virtual void delete_that(void)
		{
			delete m_pt;
		}
	};
	template <typename T, typename TLock>
	struct make_shared_ptr_count_t
		: std::array<char,sizeof(T)>
		, TLock
	{
		void * operator new (size_t uByteCount)
		{
			return _aligned_malloc(uByteCount, std::max<size_t>(alignof(T), alignof(TLock)));
		}
		void operator delete (void *p)
		{
			_aligned_free(p);
		}

		T * get_that(void) { return reinterpret_cast<T*>(static_cast<std::array<char,sizeof(T)>*>(this)); }

		template<class... TArgs>
		make_shared_ptr_count_t(TArgs &&... _Args)
		{
			::new (get_that()) T(std::forward<TArgs>(_Args)...);
		}

		virtual void delete_this(void)
		{
			delete this;
		}
		virtual void delete_that(void)
		{
			get_that()->~T();
		}
	};

	template <typename T, typename TLock>
	class shared_ptr_base
	{
		friend class shared_ptr<T,TLock>;
		friend class weak_ptr<T,TLock>;
	protected:
		T *m_pt;
		TLock *m_pLock;

		shared_ptr_base(T *pt, TLock *pLock)
			: m_pt(pt)
			, m_pLock(pLock)
		{}
	};
	template <typename T, typename TLock>
	class weak_ptr_base
		: public shared_ptr_base<T,TLock>
	{
		typedef shared_ptr_base<T,TLock> base_type;
	protected:
		template<class... TArgs>
		weak_ptr_base(TArgs &&... _Args)
			: base_type(std::forward<TArgs>(_Args)...)
		{}
	};
	template <typename T, typename TLock>
	class enable_shared_from_this_base
		: public weak_ptr_base<T, TLock>
	{
		typedef weak_ptr_base<T, TLock> base_type;

		friend class shared_ptr<T, TLock>;
	protected:
		template<class... TArgs>
		enable_shared_from_this_base(TArgs &&... _Args)
			: base_type(std::forward<TArgs>(_Args)...)
		{}

		void enable_shared(T *pt, TLock *pLock)
		{
			m_pt = pt;
			m_pLock = pLock;

			pLock->acquire_weak();
		}
	};

	template <typename T, typename TLock = shared_ptr_count>
	class shared_ptr
		: public shared_ptr_base<T,TLock>
	{
		typedef shared_ptr_base<T,TLock> base_type;
		typedef shared_ptr<T,TLock> this_type;

		template <typename TEnableShared>
		typename std::enable_if<std::is_base_of<TEnableShared, T>::value, void>::type enable_shared(void)
		{
			static_cast<TEnableShared*>(m_pt)->enable_shared(m_pt, m_pLock);
		}
		template <typename TEnableShared>
		typename std::enable_if<!std::is_base_of<TEnableShared, T>::value, void>::type enable_shared(void)
		{}
	public:
		shared_ptr(void)
			: base_type(nullptr, nullptr)
		{}
		shared_ptr(make_shared_ptr_count_t<T,TLock> *p)
			: base_type(p != nullptr ? p->get_that() : nullptr, p)
		{
			enable_shared<enable_shared_from_this_base<T, TLock>>();
		}
		shared_ptr(T *pt)
			: base_type(pt, new shared_ptr_count_t<T,TLock>(pt))
		{
			enable_shared<enable_shared_from_this_base<T, TLock>>();
		}
		shared_ptr(shared_ptr<T, TLock> const & spThat)
			: base_type(spThat.m_pt, spThat.m_pLock)
		{
			if (m_pLock != nullptr)
				m_pLock->acquire_shared();
		}
		shared_ptr(shared_ptr<T, TLock> && spThat)
			: base_type(spThat.m_pt, spThat.m_pLock)
		{
			spThat.m_pt = nullptr;
			spThat.m_pLock = nullptr;
		}
		template <typename TDeleter>
		shared_ptr(T *pt, TDeleter const & tDeleter)
			: base_type(pt, new shared_ptr_count_t<T,TLock,TDeleter>(pt, tDeleter))
		{
			enable_shared<enable_shared_from_this_base<T, TLock>>();
		}
		template <typename TDeleter>
		shared_ptr(T *pt, TDeleter && tDeleter)
			: base_type(pt, new shared_ptr_count_t<T, TLock, TDeleter>(pt, std::forward<TDeleter>(tDeleter)))
		{
			enable_shared<enable_shared_from_this_base<T, TLock>>();
		}
		template <typename U>
		shared_ptr(shared_ptr<U,TLock> const & spThat)
			: base_type(spThat.m_pt, spThat.m_pLock)
		{
			if (m_pLock != nullptr)
				m_pLock->acquire_shared();
		}
		template <typename U>
		shared_ptr(shared_ptr<U,TLock> && spThat)
			: base_type(spThat.m_pt, spThat.m_pLock)
		{
			spThat.m_pt = nullptr;
			spThat.m_pLock = nullptr;
		}
		template <typename U>
		shared_ptr(shared_ptr<U,TLock> const & spThat, T *pt)
			: base_type(pt, spThat.m_pLock)
		{
			if (m_pLock != nullptr)
				m_pLock->acquire_shared();
		}
		template <typename U>
		shared_ptr(shared_ptr<U,TLock> && spThat, T *pt)
			: base_type(pt, spThat.m_pLock)
		{
			spThat.m_pt = nullptr;
			spThat.m_pLock = nullptr;
		}
		template <typename U>
		shared_ptr(weak_ptr_base<U,TLock> const & wpThat)
			: base_type(wpThat.m_pt, wpThat.m_pLock)
		{
			if (m_pLock == nullptr ||
				m_pLock->try_shared() == false)
			{
				m_pt = nullptr;
				m_pLock = nullptr;
			}
		}
		template <typename U>
		shared_ptr(weak_ptr_base<U,TLock> && wpThat)
			: base_type(wpThat.m_pt, wpThat.m_pLock)
		{
			wpThat.m_pt = nullptr;
			wpThat.m_pLock = nullptr;

			if (m_pLock == nullptr ||
				m_pLock->convert_weak_into_shared() == false)
			{
				m_pt = nullptr;
				m_pLock = nullptr;
			}
		}
		template <typename U>
		shared_ptr(weak_ptr_base<U,TLock> const & wpThat, T *pt)
			: base_type(pt, wpThat.m_pLock)
		{
			if (m_pLock == nullptr ||
				m_pLock->try_shared() == false)
			{
				m_pt = nullptr;
				m_pLock = nullptr;
			}
		}
		template <typename U>
		shared_ptr(weak_ptr_base<U,TLock> && wpThat, T *pt)
			: base_type(pt, wpThat.m_pLock)
		{
			wpThat.m_pt = nullptr;
			wpThat.m_pLock = nullptr;

			if (m_pLock == nullptr ||
				m_pLock->convert_weak_into_shared() == false)
			{
				m_pt = nullptr;
				m_pLock = nullptr;
			}
		}
		~shared_ptr(void)
		{
			if (m_pLock != nullptr)
				m_pLock->release_shared();
		}

		T * operator -> (void) const { return m_pt; }
		T & operator * (void) const { return *m_pt; }

		T * get(void) const { return m_pt; }
		operator bool(void) const { return m_pt != nullptr; }

		long use_count(void) const
		{
			return (m_pLock != nullptr) ? m_pLock->use_count() : 0;
		}
		bool unique(void) const
		{
			return (m_pLock != nullptr) ? m_pLock->unique() : false;
		}

		void swap(this_type & that)
		{
			std::swap(m_pt, that.m_pt);
			std::swap(m_pLock, that.m_pLock);
		}
		template<class... TArgs>
		void reset(TArgs &&... _Args)
		{
			swap(this_type(std::forward<TArgs>(_Args)...));
		}

		template <typename T1>
		typename this_type & operator = (T1 const & t1)
		{
			swap(this_type(t1));
			return *this;
		}
		template <typename T1>
		typename this_type & operator = (T1 && t1)
		{
			swap(this_type(std::forward<T1>(t1)));
			return *this;
		}

		bool operator == (typename this_type const & that) const
		{
			return m_pt == that.m_pt;
		}
		bool operator != (typename this_type const & that) const
		{
			return m_pt != that.m_pt;
		}
	};
	template <typename T, typename TLock = shared_ptr_count>
	class weak_ptr
		: public weak_ptr_base<T,TLock>
	{
		typedef weak_ptr_base<T,TLock> base_type;
		typedef weak_ptr<T,TLock> this_type;
	public:
		weak_ptr(void)
			: base_type(nullptr, nullptr)
		{}
		weak_ptr(weak_ptr<T,TLock> const & wpThat)
			: base_type(wpThat.m_pt, wpThat.m_pLock)
		{
			if (m_pLock != nullptr)
				m_pLock->acquire_weak();
		}
		weak_ptr(weak_ptr<T, TLock> && wpThat)
			: base_type(wpThat.m_pt, wpThat.m_pLock)
		{
			wpThat.m_pt = nullptr;
			wpThat.m_pLock = nullptr;
		}
		template <typename U>
		weak_ptr(weak_ptr<U,TLock> const & wpThat)
			: base_type(wpThat.m_pt, wpThat.m_pLock)
		{
			if (m_pLock != nullptr)
				m_pLock->acquire_weak();
		}
		template <typename U>
		weak_ptr(weak_ptr<U,TLock> && wpThat)
			: base_type(wpThat.m_pt, wpThat.m_pLock)
		{
			wpThat.m_pt = nullptr;
			wpThat.m_pLock = nullptr;
		}
		template <typename U>
		weak_ptr(weak_ptr<U,TLock> const & wpThat, T *pt)
			: base_type(pt, wpThat.m_pLock)
		{
			if (m_pLock != nullptr)
				m_pLock->acquire_weak();
		}
		template <typename U>
		weak_ptr(weak_ptr<U,TLock> && wpThat, T *pt)
			: base_type(pt, wpThat.m_pLock)
		{
			wpThat.m_pt = nullptr;
			wpThat.m_pLock = nullptr;
		}
		template <typename U>
		weak_ptr(shared_ptr<U,TLock> const & spThat)
			: base_type(spThat.m_pt, spThat.m_pLock)
		{
			if (m_pLock != nullptr)
				m_pLock->acquire_weak();
		}
		template <typename U>
		weak_ptr(shared_ptr<U,TLock> && spThat)
			: base_type(spThat.m_pt, spThat.m_pLock)
		{
			spThat.m_pt = nullptr;
			spThat.m_pLock = nullptr;

			if (m_pLock != nullptr)
				m_pLock->convert_shared_into_weak();
		}
		template <typename U>
		weak_ptr(shared_ptr<U,TLock> const & spThat, T *pt)
			: base_type(pt, spThat.m_pLock)
		{
			if (m_pLock != nullptr)
				m_pLock->acquire_weak();
		}
		template <typename U>
		weak_ptr(shared_ptr<U,TLock> && spThat, T *pt)
			: base_type(pt, spThat.m_pLock)
		{
			spThat.m_pt = nullptr;
			spThat.m_pLock = nullptr;

			if (m_pLock != nullptr)
				m_pLock->convert_shared_into_weak();
		}
		~weak_ptr(void)
		{
			if (m_pLock != nullptr)
				m_pLock->release_weak();
		}

		bool expired(void) const
		{
			if (m_pLock != nullptr)
				return m_pLock->expired();
			else
				return true;
		}

		void swap(this_type & that)
		{
			std::swap(m_pt, that.m_pt);
			std::swap(m_pLock, that.m_pLock);
		}
		template<class... TArgs>
		void reset(TArgs &&... _Args)
		{
			swap(this_type(std::forward<TArgs>(_Args)...));
		}

		template <typename T1>
		typename this_type & operator = (T1 const & t1)
		{
			swap(this_type(t1));
			return *this;
		}
		template <typename T1>
		typename this_type & operator = (T1 && t1)
		{
			swap(this_type(std::forward<T1>(t1)));
			return *this;
		}

		bool operator == (typename this_type const & that) const
		{
			return m_pt == that.m_pt;
		}
		bool operator != (typename this_type const & that) const
		{
			return m_pt != that.m_pt;
		}
	};

	template<typename T, typename TLock = shared_ptr_count, class... TArgs>
	shared_ptr<T,TLock> make_shared(TArgs &&... _Args)
	{
		return std::move(shared_ptr<T,TLock>(new make_shared_ptr_count_t<T,TLock>(std::forward<TArgs>(_Args)...)));
	}

	template <typename T, typename TLock = shared_ptr_count>
	class enable_shared_from_this
		: public enable_shared_from_this_base<T, TLock>
	{
		typedef  enable_shared_from_this_base<T, TLock> base_type;
	public:
		enable_shared_from_this(void)
			: base_type(nullptr, nullptr)
		{}
		~enable_shared_from_this(void)
		{
			if (m_pLock != nullptr)
				m_pLock->release_weak();
		}
		shared_ptr<T, TLock> shared_from_this(void)
		{
			return std::move(shared_ptr<T, TLock>(*this));
		}
		shared_ptr<T const, TLock> shared_from_this(void) const
		{
			return std::move(shared_ptr<T const, TLock>(*this));
		}
		shared_ptr<T volatile, TLock> shared_from_this(void) volatile
		{
			return std::move(shared_ptr<T volatile, TLock>(*this));
		}
	};
}
