#pragma once

/* Author: jrmwng @ 2015~2016 */

#include <intrin.h>
#include <array>
#include <atomic>

namespace jrmwng
{
	template <typename T, typename TLock>
	class shared_ptr;
	template <typename T, typename TLock>
	class weak_ptr;
	template <typename T, typename TLock>
	class enable_shared_from_this;

	template <typename TR, bool bLock, typename TLockFunc, typename TFunc>
	std::enable_if_t<std::is_void<TR>::value, void> shared_ptr_tm(TLockFunc const & tLockFun, TFunc const & tFunc)
	{
		if (bLock)
		{
#ifdef RTM
			unsigned const uXBEGIN = _xbegin();

			if (uXBEGIN == _XBEGIN_STARTED)
			{
				tFunc();
				_xend();
			}
			else
#endif
			{
				tLockFunc();
			}
		}
		else
		{
			tFunc();
		}
	}
	template <typename TR, bool bLock, typename TLockFunc, typename TFunc>
	std::enable_if_t<!std::is_void<TR>::value, TR> shared_ptr_tm(TLockFunc const & tLockFunc, TFunc const & tFunc)
	{
		if (bLock)
		{
#ifdef RTM
			unsigned const uXBEGIN = _xbegin();

			if (uXBEGIN == _XBEGIN_STARTED)
			{
				TR tR(std::move(tFunc()));
				_xend();
				return std::move(tR);
			}
			else
#endif
			{
				return std::move(tLockFunc());
			}
		}
		else
		{
			return std::move(tFunc());
		}
	}

	template <bool bLock>
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
			return static_cast<long>(lRefCount.load(std::memory_order_relaxed) >> SHARED_PTR_COUNT_THAT_SHIFT);
		}
		bool unique(void) const
		{
			return lRefCount.load(std::memory_order_relaxed) == SHARED_PTR_COUNT_INIT; // unique without weak_ptr
		}
		bool expired(void) const
		{
			return lRefCount.load(std::memory_order_relaxed) < SHARED_PTR_COUNT_THAT;
		}

		void release_shared(void)
		{
			count_type const lRefCount0 = shared_ptr_tm<count_type, bLock>(
				[this](void)->count_type
				{
					return lRefCount.fetch_sub(SHARED_PTR_COUNT_BOTH, std::memory_order_release);
				}
				,
				[this](void)->count_type
				{
					count_type const lRefCount0 = lRefCount.load(std::memory_order_relaxed);
					count_type const lRefCount1 = lRefCount0 - SHARED_PTR_COUNT_BOTH;
					lRefCount.store(lRefCount1, std::memory_order_relaxed);
					return lRefCount0;
				}
				);

			if (lRefCount0 < SHARED_PTR_COUNT_BOTH + SHARED_PTR_COUNT_THAT)
			{
				delete_that(); // assume nothrow for simplicity

				if (lRefCount0 < SHARED_PTR_COUNT_BOTH + SHARED_PTR_COUNT_BASE + SHARED_PTR_COUNT_THIS
					||
					shared_ptr_tm<count_type, bLock>(
						[this](void)->count_type
						{
							return lRefCount.fetch_sub(SHARED_PTR_COUNT_BASE, std::memory_order_release);
						}
						,
						[this](void)->count_type
						{
							count_type const lRefCount0 = lRefCount.load(std::memory_order_relaxed);
							count_type const lRefCount1 = lRefCount0 - SHARED_PTR_COUNT_BASE;
							lRefCount.store(lRefCount1, std::memory_order_relaxed);
							return lRefCount0;
						}
						) < SHARED_PTR_COUNT_BASE + SHARED_PTR_COUNT_THIS
					)
				{
					delete_this();
				}
			}
		}
		void release_weak(void)
		{
			count_type const lRefCount0 = shared_ptr_tm<count_type, bLock>(
				[this](void)->count_type
				{
					return lRefCount.fetch_sub(SHARED_PTR_COUNT_THIS, std::memory_order_release);
				}
				,
				[this](void)->count_type
				{
					count_type const lRefCount0 = lRefCount.load(std::memory_order_relaxed);
					count_type const lRefCount1 = lRefCount0 - SHARED_PTR_COUNT_THIS;
					lRefCount.store(lRefCount1, std::memory_order_relaxed);
					return lRefCount0;
				}
				);

			if (lRefCount0 < SHARED_PTR_COUNT_THIS + SHARED_PTR_COUNT_BASE)
				delete_this();
		}
		void acquire_weak(void)
		{
			shared_ptr_tm<void, bLock>(
				[this](void)
				{
					lRefCount.fetch_add(SHARED_PTR_COUNT_THIS, std::memory_order_acquire);
				}
				,
				[this](void)
				{
					count_type const lRefCount0 = lRefCount.load(std::memory_order_relaxed);
					count_type const lRefCount1 = lRefCount0 + SHARED_PTR_COUNT_THIS;
					lRefCount.store(lRefCount1, std::memory_order_relaxed);
				}
				);
		}
		void acquire_shared(void)
		{
			shared_ptr_tm<void, bLock>(
				[this](void)
				{
					lRefCount.fetch_add(SHARED_PTR_COUNT_BOTH, std::memory_order_acquire);
				}
				,
				[this](void)
				{
					count_type const lRefCount0 = lRefCount.load(std::memory_order_relaxed);
					count_type const lRefCount1 = lRefCount0 + SHARED_PTR_COUNT_BOTH;
					lRefCount.store(lRefCount1, std::memory_order_relaxed);
				}
				);
		}
		bool try_shared(void)
		{
			return shared_ptr_tm<count_type, bLock>(
				[this](void)->count_type
				{
					count_type lRefCount0;
					count_type lRefCount1;
					{
						for (lRefCount0 = lRefCount.load(std::memory_order_relaxed); lRefCount0 >= SHARED_PTR_COUNT_THAT && !lRefCount.compare_exchange_weak(lRefCount0, lRefCount1 = lRefCount0 + SHARED_PTR_COUNT_BOTH, std::memory_order_acquire); lRefCount0 = lRefCount.load(std::memory_order_relaxed))
						{
							_mm_pause();
						}
					}
					return lRefCount0;
				}
				,
				[this](void)->count_type
				{
					count_type const lRefCount0 = lRefCount.load(std::memory_order_relaxed);
					count_type const lRefCount1 = lRefCount0 + SHARED_PTR_COUNT_BOTH;
					if (lRefCount0 >= SHARED_PTR_COUNT_THAT)
					{
						lRefCount.store(lRefCount1, std::memory_order_relaxed);
					}
					return lRefCount0;
				}
				) >= SHARED_PTR_COUNT_THAT;
		}
		void convert_shared_into_weak(void)
		{
			count_type const lRefCount0 = shared_ptr_tm<count_type, bLock>(
				[this](void)->count_type
				{
					return lRefCount.fetch_sub(SHARED_PTR_COUNT_BOTH - SHARED_PTR_COUNT_THIS, std::memory_order_release);
				}
				,
				[this](void)->count_type
				{
					count_type const lRefCount0 = lRefCount.load(std::memory_order_relaxed);
					count_type const lRefCount1 = lRefCount0 - (SHARED_PTR_COUNT_BOTH - SHARED_PTR_COUNT_THIS);
					lRefCount.store(lRefCount1, std::memory_order_relaxed);
					return lRefCount0;
				}
				);

			if (lRefCount0 < (SHARED_PTR_COUNT_BOTH - SHARED_PTR_COUNT_THIS) + SHARED_PTR_COUNT_THAT)
				delete_that();
		}
		bool convert_weak_into_shared(void)
		{
			if (shared_ptr_tm<count_type, bLock>(
				[this](void)->count_type
				{
					count_type lRefCount0;
					count_type lRefCount1;
					{
						for (lRefCount0 = lRefCount.load(std::memory_order_relaxed); lRefCount0 >= SHARED_PTR_COUNT_THAT && !lRefCount.compare_exchange_weak(lRefCount0, lRefCount1 = lRefCount0 + SHARED_PTR_COUNT_BOTH - SHARED_PTR_COUNT_THIS, std::memory_order_acquire); lRefCount0 = lRefCount.load(std::memory_order_relaxed))
						{
							_mm_pause();
						}
					}
					return lRefCount0;
				}
				,
				[this](void)->count_type
				{
					count_type const lRefCount0 = lRefCount.load(std::memory_order_relaxed);
					count_type const lRefCount1 = lRefCount0 + (SHARED_PTR_COUNT_BOTH - SHARED_PTR_COUNT_THIS);
					if (lRefCount0 >= SHARED_PTR_COUNT_THAT)
					{
						lRefCount.store(lRefCount1, std::memory_order_relaxed);
					}
					return lRefCount0;
				}
				) >= SHARED_PTR_COUNT_THAT)
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

	template <typename T, typename TLock, typename TDeleter = void>
	struct shared_ptr_count_t
		: std::array<char, sizeof(TDeleter)>
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
	struct shared_ptr_count_t<T, TLock, void>
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
		: std::array<char, sizeof(T)>
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
		friend class shared_ptr<T, TLock>;
		friend class weak_ptr<T, TLock>;
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
		: public shared_ptr_base<T, TLock>
	{
		typedef shared_ptr_base<T, TLock> base_type;
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

	template <typename T, typename TLock = shared_ptr_count<true> >
	class shared_ptr
		: public shared_ptr_base<T, TLock>
	{
		typedef shared_ptr_base<T, TLock> base_type;
		typedef shared_ptr<T, TLock> this_type;

		template <typename TEnableShared>
		std::enable_if_t<std::is_base_of<TEnableShared, T>::value, void> enable_shared(void)
		{
			static_cast<TEnableShared*>(m_pt)->enable_shared(m_pt, m_pLock);
		}
		template <typename TEnableShared>
		std::enable_if_t<!std::is_base_of<TEnableShared, T>::value, void> enable_shared(void)
		{}
	public:
		typedef T element_type;

		shared_ptr(void)
			: base_type(nullptr, nullptr)
		{}
		shared_ptr(make_shared_ptr_count_t<T, TLock> *p)
			: base_type(p != nullptr ? p->get_that() : nullptr, p)
		{
			enable_shared<enable_shared_from_this_base<T, TLock>>();
		}
		shared_ptr(T *pt)
			: base_type(pt, new shared_ptr_count_t<T, TLock>(pt))
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
			: base_type(pt, new shared_ptr_count_t<T, TLock, TDeleter>(pt, tDeleter))
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
		shared_ptr(shared_ptr<U, TLock> const & spThat)
			: base_type(spThat.m_pt, spThat.m_pLock)
		{
			if (m_pLock != nullptr)
				m_pLock->acquire_shared();
		}
		template <typename U>
		shared_ptr(shared_ptr<U, TLock> && spThat)
			: base_type(spThat.m_pt, spThat.m_pLock)
		{
			spThat.m_pt = nullptr;
			spThat.m_pLock = nullptr;
		}
		template <typename U>
		shared_ptr(shared_ptr<U, TLock> const & spThat, T *pt)
			: base_type(pt, spThat.m_pLock)
		{
			if (m_pLock != nullptr)
				m_pLock->acquire_shared();
		}
		template <typename U>
		shared_ptr(shared_ptr<U, TLock> && spThat, T *pt)
			: base_type(pt, spThat.m_pLock)
		{
			spThat.m_pt = nullptr;
			spThat.m_pLock = nullptr;
		}
		template <typename U>
		shared_ptr(weak_ptr_base<U, TLock> const & wpThat)
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
		shared_ptr(weak_ptr_base<U, TLock> && wpThat)
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
		shared_ptr(weak_ptr_base<U, TLock> const & wpThat, T *pt)
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
		shared_ptr(weak_ptr_base<U, TLock> && wpThat, T *pt)
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

		typename this_type & operator = (typename this_type const & that)
		{
			swap(this_type(that));
			return *this;
		}
		template <typename T1>
		typename this_type & operator = (T1 const & t1)
		{
			swap(this_type(t1));
			return *this;
		}
		typename this_type & operator = (typename this_type && that)
		{
			swap(this_type(std::forward<typename this_type>(that)));
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
	template <typename T, typename TLock = shared_ptr_count<true> >
	class weak_ptr
		: public weak_ptr_base<T, TLock>
	{
		typedef weak_ptr_base<T, TLock> base_type;
		typedef weak_ptr<T, TLock> this_type;
	public:
		weak_ptr(void)
			: base_type(nullptr, nullptr)
		{}
		weak_ptr(weak_ptr<T, TLock> const & wpThat)
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
		weak_ptr(weak_ptr<U, TLock> const & wpThat)
			: base_type(wpThat.m_pt, wpThat.m_pLock)
		{
			if (m_pLock != nullptr)
				m_pLock->acquire_weak();
		}
		template <typename U>
		weak_ptr(weak_ptr<U, TLock> && wpThat)
			: base_type(wpThat.m_pt, wpThat.m_pLock)
		{
			wpThat.m_pt = nullptr;
			wpThat.m_pLock = nullptr;
		}
		template <typename U>
		weak_ptr(weak_ptr<U, TLock> const & wpThat, T *pt)
			: base_type(pt, wpThat.m_pLock)
		{
			if (m_pLock != nullptr)
				m_pLock->acquire_weak();
		}
		template <typename U>
		weak_ptr(weak_ptr<U, TLock> && wpThat, T *pt)
			: base_type(pt, wpThat.m_pLock)
		{
			wpThat.m_pt = nullptr;
			wpThat.m_pLock = nullptr;
		}
		template <typename U>
		weak_ptr(shared_ptr<U, TLock> const & spThat)
			: base_type(spThat.m_pt, spThat.m_pLock)
		{
			if (m_pLock != nullptr)
				m_pLock->acquire_weak();
		}
		template <typename U>
		weak_ptr(shared_ptr<U, TLock> && spThat)
			: base_type(spThat.m_pt, spThat.m_pLock)
		{
			spThat.m_pt = nullptr;
			spThat.m_pLock = nullptr;

			if (m_pLock != nullptr)
				m_pLock->convert_shared_into_weak();
		}
		template <typename U>
		weak_ptr(shared_ptr<U, TLock> const & spThat, T *pt)
			: base_type(pt, spThat.m_pLock)
		{
			if (m_pLock != nullptr)
				m_pLock->acquire_weak();
		}
		template <typename U>
		weak_ptr(shared_ptr<U, TLock> && spThat, T *pt)
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

		operator bool(void) const
		{
			return m_pt != nullptr;
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

		typename this_type & operator = (typename this_type const & t1)
		{
			swap(this_type(t1));
			return *this;
		}
		template <typename T1>
		typename this_type & operator = (T1 const & t1)
		{
			swap(this_type(t1));
			return *this;
		}
		typename this_type & operator = (typename this_type && t1)
		{
			swap(this_type(std::forward<typename this_type>(t1)));
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

	template<typename T, typename TLock = shared_ptr_count<true>, class... TArgs>
	shared_ptr<T, TLock> make_shared(TArgs &&... _Args)
	{
		return std::move(shared_ptr<T, TLock>(new make_shared_ptr_count_t<T, TLock>(std::forward<TArgs>(_Args)...)));
	}

	template <typename T, typename TLock = shared_ptr_count<true> >
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

	struct alignas(32) shared_ptr_lock
	{
		__m128i volatile xmmLock;

		shared_ptr_lock(void)
			: xmmLock(_mm_setzero_si128())
		{}

		unsigned read_begin(void)
		{
			for (;;)
			{
				if (xmmLock.m128i_i32[0] == 0 && _InterlockedExchange_HLEAcquire(reinterpret_cast<long volatile*>(&xmmLock.m128i_i32[0]), ~0) == 0) return 0;
				if (xmmLock.m128i_i32[1] == 0 && _InterlockedExchange_HLEAcquire(reinterpret_cast<long volatile*>(&xmmLock.m128i_i32[1]), ~0) == 0) return 1;
				if (xmmLock.m128i_i32[2] == 0 && _InterlockedExchange_HLEAcquire(reinterpret_cast<long volatile*>(&xmmLock.m128i_i32[2]), ~0) == 0) return 2;
				if (xmmLock.m128i_i32[3] == 0 && _InterlockedExchange_HLEAcquire(reinterpret_cast<long volatile*>(&xmmLock.m128i_i32[3]), ~0) == 0) return 3;

				_mm_pause();
			}
		}
		void read_end(unsigned uIndex)
		{
			_Store_HLERelease(reinterpret_cast<long volatile*>(&xmmLock.m128i_i32[uIndex]), 0);
		}
		void read_sync(void) const
		{
			for (int nLock = _mm_movemask_epi8(const_cast<__m128i const&>(xmmLock)); nLock; nLock &= _mm_movemask_epi8(const_cast<__m128i const&>(xmmLock)))
			{
				_mm_pause();
			}
		}
	};

	template <unsigned uSize>
	struct shared_ptr_integral;

	template <>
	struct shared_ptr_integral<8>
	{
		typedef long long type;
	};
	template <>
	struct shared_ptr_integral<16>
	{
		typedef __m128i type;
	};

	template <unsigned uSize>
	using shared_ptr_integral_t = typename shared_ptr_integral<uSize>::type;

	template <typename T, typename TObj = shared_ptr<T>, typename TSync = std::identity<TObj>>
	class alignas(64) atomic_shared_ptr
		: public TObj
	{
		shared_ptr_lock mutable m_Lock;

		typedef shared_ptr_integral_t<sizeof(TObj)> integral_t;
	public:
		template <class... TArgs>
		atomic_shared_ptr(TArgs &&... _Args)
			: TObj(std::forward<TArgs>(_Args)...)
		{}

		template <typename TFunc>
		void load(TFunc tFunc) const
		{
			unsigned const uKey = m_Lock.read_begin();

			integral_t stLocal;
			{
				TObj const *pThisObj = static_cast<TObj const*>(this);

				std::atomic<integral_t> const *pThisIntegral = reinterpret_cast<std::atomic<integral_t>const*>(pThisObj);

				stLocal = pThisIntegral->load(std::memory_order_acquire);
			}

			tFunc(reinterpret_cast<TObj const&>(stLocal));

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
			TObj *pThisObj = static_cast<TObj*>(this);

			std::atomic<integral_t> *pThisIntegral = reinterpret_cast<std::atomic<integral_t>*>(pThisObj);

			reinterpret_cast<integral_t&>(that) = pThisIntegral->exchange(reinterpret_cast<integral_t const&>(that));

			if (TSync()(that))
				m_Lock.read_sync();
		}

		template <typename TCompare>
		std::enable_if_t<sizeof(TObj) == sizeof(TCompare), bool> cas(TObj & that, TCompare const & tCompare)
		{
			TObj *pThisObj = static_cast<TObj*>(this);

			std::atomic<integral_t> *pThisIntegral = reinterpret_cast<std::atomic<integral_t>*>(pThisObj);

			if (pThisIntegral->compare_exchange(reinterpret_cast<integral_t&>(that), reinterpret_cast<integral_t const&>(tCompare), std::memory_order_relaxed))
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

		typename atomic_shared_ptr & operator = (TObj const & that)
		{
			swap(TObj(that));
			return *this;
		}
		template <typename T1>
		typename atomic_shared_ptr & operator = (T1 const & that)
		{
			swap(TObj(that));
			return *this;
		}
		typename atomic_shared_ptr & operator = (TObj && that)
		{
			swap(TObj(std::forward<TObj>(that)));
			return *this;
		}
		template <typename T1>
		typename atomic_shared_ptr & operator = (T1 && that)
		{
			swap(TObj(std::forward<T1>(that)));
			return *this;
		}
	};
}
