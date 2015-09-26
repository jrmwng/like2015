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

	struct __declspec(align(16)) shared_ptr_lock
	{
		__declspec(align(16)) __m128i xmm;

		static __m128i g_xmmLockPool;
		static thread_local unsigned g_uLockIndex;

		static bool acquire(void)
		{
			static unsigned g_uTryIndex = 0;

			unsigned uIndex0 = g_uTryIndex;
			unsigned uIndex1;
			{
				for (uIndex1 = uIndex0; uIndex1 - uIndex0 < sizeof(uintptr_t); uIndex1++)
				{
					if (g_xmmLockPool.m128i_u8[uIndex1 % sizeof(uintptr_t)] != 0)
					{
						if (atomic_exchange<unsigned char>(g_xmmLockPool.m128i_u8, 0) != 0)
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
				return true;
			}
			else
				return false;
		}
		static void release(void)
		{
			_mm_sfence();

			unsigned uIndex = std::exchange(g_uLockIndex, static_cast<unsigned>(sizeof(uintptr_t)));
			g_xmmLockPool.m128i_u8[uIndex] = ~0;
		}
		static unsigned index(void)
		{
			unsigned uLockIndex = g_uLockIndex;
			{
#ifdef _DEBUG
				if (uLockIndex >= sizeof(uintptr_t))
					__debugbreak();
#endif
			}
			return uLockIndex;
		}

		shared_ptr_lock(void)
			: xmm(_mm_set1_epi32(0x80808080))
		{
			xmm.m128i_i8[0]++;
			xmm.m128i_i8[sizeof(uintptr_t)]++;
		}

		virtual void delete_this(void) { __debugbreak(); }
		virtual void delete_that(void) { __debugbreak(); }

		static __m128i unpack_bin(__m128i const & b16BIN0)
		{
			if (sizeof(uintptr_t) == 4)
				return _mm_unpacklo_epi8(b16BIN0, _mm_setzero_si128());
			else
				return b16BIN0;
		}

		long use_count(void) const
		{
			__m128i b16BIN0 = xmm;
			__m128i b16BIN1 = unpack_bin(b16BIN0);
			__m128i w8SAD2 = _mm_sad_epu8(b16BIN1, _mm_setzero_si128());
			return w8SAD2.m128i_i32[0] - 0x80 * 8;
		}
		bool unique(void) const
		{
			__m128i b16BIN0 = xmm;
			__m128i b16BIN1 = unpack_bin(b16BIN0);
			__m128i w8SAD2 = _mm_sad_epu8(b16BIN1, _mm_setzero_si128());
			return w8SAD2.m128i_i32[0] == 0x80 * 8 && w8SAD2.m128i_i32[2] == 0x80 * 8;
		}
		bool expired(void) const
		{
			__m128i b16BIN0 = xmm;
			__m128i b16BIN1 = unpack_bin(b16BIN0);
			__m128i w8SAD2 = _mm_sad_epu8(b16BIN1, _mm_setzero_si128());
			return w8SAD2.m128i_i32[0] == 0x80 * 8;
		}

		void acquire_shared(unsigned uLockIndex)
		{
			xmm.m128i_i8[uLockIndex]++;
			xmm.m128i_i8[uLockIndex + sizeof(uintptr_t)]++;
		}
		void acquire_shared(void)
		{
			acquire_shared(index());
		}
		void acquire_weak(unsigned uLockIndex)
		{
			xmm.m128i_i8[uLockIndex + sizeof(uintptr_t)]++;
		}
		void acquire_weak(void)
		{
			acquire_weak(index());
		}
		void release_weak(unsigned uLockIndex)
		{
			__m128i b16Delta;
			{
				if (sizeof(uintptr_t) == 4)
					b16Delta = _mm_set_epi32(0, 0, 1 << (uLockIndex * 8), 0);
				else
					b16Delta = _mm_set_epi64x(1LL << (uLockIndex * 8), 0);
			}
			__m128i b16BIN0;
			__m128i b16BIN1a;
			__m128i b16BIN1b;
			__m128i w8SAD2;
			do
			{
				b16BIN0 = xmm;
				b16BIN1a = _mm_sub_epi8(b16BIN0, b16Delta);
				b16BIN1b = unpack_bin(b16BIN0);
				w8SAD2 = _mm_sad_epu8(b16BIN1b, _mm_setzero_si128());
			} while (!atomic_cas<sizeof(uintptr_t)>(xmm.m128i_i8 + sizeof(uintptr_t), b16BIN1a.m128i_i8 + sizeof(uintptr_t), b16BIN0.m128i_i8 + sizeof(uintptr_t)));

			if (w8SAD2.m128i_i32[2] == 0x80 * sizeof(uintptr_t) + 1) // delete_this() if it is the last weak_ptr
			{
				delete_this();
			}
		}
		void release_weak(void)
		{
			release_weak(index());
		}
		void release_shared(unsigned uLockIndex)
		{
			__m128i b16Delta;
			{
				if (sizeof(uintptr_t) == 4)
					b16Delta = _mm_set_epi32(0, 0, 1 << (uLockIndex * 8), 1 << (uLockIndex * 8));
				else
					b16Delta = _mm_set_epi64x(1LL << (uLockIndex * 8), 1LL << (uLockIndex * 8));
			}
			__m128i b16BIN0;
			__m128i b16BIN1a;
			__m128i b16BIN1b;
			__m128i w8SAD2;
			do
			{
				b16BIN0 = xmm;
				b16BIN1a = _mm_sub_epi8(b16BIN0, b16Delta);
				b16BIN1b = unpack_bin(b16BIN0);
				w8SAD2 = _mm_sad_epu8(b16BIN1b, _mm_setzero_si128());
			} while (!atomic_cas<sizeof(uintptr_t)*2>(xmm.m128i_i8, b16BIN1a.m128i_i8, b16BIN0.m128i_i8));

			if (w8SAD2.m128i_i32[0] == 0x80 * 8 + 1) // delete_that() if it is the last shared_ptr
			{
				delete_that();

				if (w8SAD2.m128i_i32[2] == 0x80 * 8 + 1) // delete_this() if it is the last weak_ptr
				{
					delete_this();
				}
			}
		}
		void release_shared(void)
		{
			release_shared(index());
		}
		bool try_shared(unsigned uLockIndex)
		{
			__m128i b16Delta;
			{
				if (sizeof(uintptr_t) == 4)
					b16Delta = _mm_set_epi32(0, 0, 1 << (uLockIndex * 8), 1 << (uLockIndex * 8));
				else
					b16Delta = _mm_set_epi64x(1LL << (uLockIndex * 8), 1LL << (uLockIndex * 8));
			}
			__m128i b16BIN0;
			__m128i b16BIN1a;
			__m128i b16BIN1b;
			__m128i w8SAD2;
			do
			{
				b16BIN0 = xmm;
				b16BIN1a = _mm_add_epi8(b16BIN0, b16Delta);
				b16BIN1b = unpack_bin(b16BIN0);
				w8SAD2 = _mm_sad_epu8(b16BIN1b, _mm_setzero_si128());
			} while (w8SAD2.m128i_i32[2] > 0x80 * 8 && !atomic_cas<sizeof(uintptr_t)*2>(xmm.m128i_i8, b16BIN1a.m128i_i8, b16BIN0.m128i_i8));

			return w8SAD2.m128i_i32[2] > 0x80 * 8;
		}
		bool try_shared(void)
		{
			return try_shared(index());
		}
		void convert_shared_into_weak(unsigned uLockIndex)
		{
			xmm.m128i_i8[uLockIndex]--;
		}
		void convert_shared_into_weak(void)
		{
			return convert_shared_into_weak(index());
		}
		bool convert_weak_into_shared(unsigned uLockIndex)
		{
			__m128i b16Delta;
			{
#ifdef _M_X64
				b16Delta = _mm_cvtsi64x_si128(1LL << (uLockIndex * 8));
#else
				b16Delta = _mm_cvtsi32_si128(1 << (uLockIndex * 8));
#endif
			}
			__m128i b16BIN0;
			__m128i b16BIN1a;
			__m128i b16BIN1b;
			__m128i w8SAD2;
			do
			{
				b16BIN0 = xmm;
				b16BIN1a = _mm_add_epi8(b16BIN0, b16Delta);
				b16BIN1b = unpack_bin(b16BIN0);
				w8SAD2 = _mm_sad_epu8(b16BIN1b, _mm_setzero_si128());
			} while (w8SAD2.m128i_i32[0] > 0x80 * 8 && !atomic_cas<sizeof(uintptr_t)>(xmm.m128i_i8, b16BIN1a.m128i_i8, b16BIN0.m128i_i8));

			if (w8SAD2.m128i_i32[0] > 0x80 * 8)
			{
				return true;
			}
			else
			{
				release_weak(uLockIndex);
				return false;
			}
		}
		bool convert_weak_into_shared(void)
		{
			return convert_weak_into_shared(index());
		}
	};
	__m128i shared_ptr_lock::g_xmmLockPool = _mm_cmplt_epi8(_mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100), _mm_set1_epi8(sizeof(uintptr_t)));
	thread_local unsigned shared_ptr_lock::g_uLockIndex = sizeof(uintptr_t);

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
			count_type lRefCount0 = lRefCount.fetch_sub(SHARED_PTR_COUNT_BOTH);

			if (lRefCount0 < SHARED_PTR_COUNT_BOTH + SHARED_PTR_COUNT_THAT)
			{
				delete_that(); // assume nothrow for simplicity

				if (lRefCount0 < SHARED_PTR_COUNT_BOTH + SHARED_PTR_COUNT_BASE + SHARED_PTR_COUNT_THIS
					||
					lRefCount.fetch_sub(SHARED_PTR_COUNT_BASE) < SHARED_PTR_COUNT_BASE + SHARED_PTR_COUNT_THIS
					)
				{
					delete_this();
				}
			}
		}
		void release_weak(void)
		{
			count_type lRefCount0 = lRefCount.fetch_sub(SHARED_PTR_COUNT_THIS);

			if (lRefCount0 < SHARED_PTR_COUNT_THIS + SHARED_PTR_COUNT_BASE)
				delete_this();
		}
		void acquire_weak(void)
		{
			lRefCount.fetch_add(SHARED_PTR_COUNT_THIS);
		}
		void acquire_shared(void)
		{
			lRefCount.fetch_add(SHARED_PTR_COUNT_BOTH);
		}
		bool try_shared(void)
		{
			count_type lRefCount0;
			count_type lRefCount1;
			{
				for (lRefCount0 = lRefCount; lRefCount0 >= SHARED_PTR_COUNT_THAT && !lRefCount.compare_exchange_weak(lRefCount0, lRefCount1 = lRefCount0 + SHARED_PTR_COUNT_BOTH); lRefCount0 = lRefCount)
				{
					_mm_pause();
				}
			}
			return lRefCount0 >= SHARED_PTR_COUNT_THAT;
		}
		void convert_shared_into_weak(void)
		{
			count_type lRefCount0 = lRefCount.fetch_sub(SHARED_PTR_COUNT_BOTH - SHARED_PTR_COUNT_THIS);

			if (lRefCount0 < (SHARED_PTR_COUNT_BOTH - SHARED_PTR_COUNT_THIS) + SHARED_PTR_COUNT_THAT)
				delete_that();
		}
		bool convert_weak_into_shared(void)
		{
			count_type lRefCount0;
			count_type lRefCount1;
			{
				for (lRefCount0 = lRefCount; lRefCount0 >= SHARED_PTR_COUNT_THAT && !lRefCount.compare_exchange_weak(lRefCount0, lRefCount1 = lRefCount0 + SHARED_PTR_COUNT_BOTH - SHARED_PTR_COUNT_THIS); lRefCount0 = lRefCount)
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
	//template <typename T>
	//struct shared_ptr_align
	//{
	//	char ac[alignof(T) +sizeof(T)];
	//	T * get(void) { return reinterpret_cast<T*>((reinterpret_cast<uintptr_t>(ac) + (alignof(T) -1)) & ~(alignof(T) -1)); }
	//};
	template <typename T, typename TLock, typename TDeleter = void>
	struct shared_ptr_count_t
		: std::array<char,sizeof(TDeleter)> //shared_ptr_align<TDeleter> // TODO: aligned by allocator instead
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

		TDeleter * get(void) { return reinterpret_cast<TDeleter*>(this); }

		T * const m_pt;

		shared_ptr_count_t(T *pt, TDeleter && tDeleter)
			: m_pt(pt)
		{
			::new (get()) TDeleter(std::forward<TDeleter>(tDeleter));
		}
		~shared_ptr_count_t(void)
		{
			get()->~TDeleter();
		}

		virtual void delete_this(void)
		{
			delete this;
		}
		virtual void delete_that(void)
		{
			(*get())(m_pt);
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
		: std::array<char,sizeof(T)>//shared_ptr_align<T> // TODO: aligned by allocator instead
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

		T * get(void) { return reinterpret_cast<T*>(this); }

		template<class... TArgs>
		make_shared_ptr_count_t(TArgs &&... _Args)
		{
			::new (get()) T(std::forward<TArgs>(_Args)...);
		}

		virtual void delete_this(void)
		{
			delete this;
		}
		virtual void delete_that(void)
		{
			get()->~T();
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

		long use_count(void) const
		{
			if (m_pLock != nullptr)
				return (m_pLock->use_count());
			else
				return 0;
		}
		bool unique(void) const
		{
			if (m_pLock != nullptr)
				return m_pLock->unique();
			else
				return false;
		}
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

	template <typename T, typename TLock = shared_ptr_count>
	class shared_ptr
		: public shared_ptr_base<T,TLock>
	{
		typedef shared_ptr_base<T,TLock> base_type;
		typedef shared_ptr<T,TLock> this_type;
	public:
		shared_ptr(void)
			: base_type(nullptr, nullptr)
		{}
		shared_ptr(make_shared_ptr_count_t<T,TLock> *p)
			: base_type(p != nullptr ? /*p->get()*/reinterpret_cast<T*>(p) : nullptr, p)
		{}
		shared_ptr(T *pt)
			: base_type(pt, new shared_ptr_count_t<T,TLock>(pt))
		{}
		template <typename TDeleter>
		shared_ptr(T *pt, TDeleter && tDeleter)
			: base_type(pt, new shared_ptr_count_t<T,TLock,TDeleter>(pt, std::forward<TDeleter>(tDeleter)))
		{}
		shared_ptr(shared_ptr<T,TLock> const & spThat)
			: base_type(spThat.m_pt, spThat.m_pLock)
		{
			if (m_pLock != nullptr)
				m_pLock->acquire_shared();
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
	};
	template <typename T, typename TLock = shared_ptr_count>
	class weak_ptr
		: public weak_ptr_base<T,TLock>
	{
		typedef weak_ptr_base<T,TLock> base_type;
		typedef weak_ptr<T,TLock> this_type;
	public:
		weak_ptr(weak_ptr<T,TLock> const & wpThat)
			: base_type(wpThat.m_pt, wpThat.m_pLock)
		{
			if (m_pLock != nullptr)
				m_pLock->acquire_weak();
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
	};
	template<typename T, typename TLock = shared_ptr_count, class... TArgs>
	shared_ptr<T,TLock> make_shared(TArgs &&... _Args)
	{
		return std::move(shared_ptr<T,TLock>(new make_shared_ptr_count_t<T,TLock>(std::forward<TArgs>(_Args)...)));
	}
}
