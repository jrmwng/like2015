#pragma once

#include <intrin.h>
#include <atomic>

namespace like2015
{
	template <typename T>
	class shared_ptr;
	template <typename T>
	class weak_ptr;

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

		virtual void delete_this(void) = 0;
		virtual void delete_that(void) = 0;

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
	template <typename T>
	struct shared_ptr_align
	{
		char ac[alignof(T) +sizeof(T)];
		T * get(void) { return reinterpret_cast<T*>((reinterpret_cast<uintptr_t>(ac) + (alignof(T) -1)) & ~(alignof(T) -1)); }
	};
	template <typename T, typename TDeleter = void>
	struct shared_ptr_count_t
		: shared_ptr_align<TDeleter> // TODO: aligned by allocator instead
		, shared_ptr_count
	{
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
			(*(get()))(m_pt);
		}
	};
	template <typename T>
	struct shared_ptr_count_t<T,void>
		: shared_ptr_count
	{
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
	template <typename T>
	struct make_shared_ptr_count_t
		: shared_ptr_align<T> // TODO: aligned by allocator instead
		, shared_ptr_count
	{
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
	template <typename T>
	class shared_ptr_base
	{
		friend class shared_ptr<T>;
		friend class weak_ptr<T>;
	protected:
		T *m_pt;
		shared_ptr_count *m_pCount;

		shared_ptr_base(T *pt, shared_ptr_count *pCount)
			: m_pt(pt)
			, m_pCount(pCount)
		{}

		long use_count(void) const
		{
			if (m_pCount != nullptr)
				return (m_pCount->use_count());
			else
				return 0;
		}
		bool unique(void) const
		{
			if (m_pCount != nullptr)
				return m_pCount->unique();
			else
				return false;
		}
	};
	template <typename T>
	class weak_ptr_base
		: public shared_ptr_base<T>
	{
		typedef shared_ptr_base<T> base_type;
	protected:
		template<class... TArgs>
		weak_ptr_base(TArgs &&... _Args)
			: base_type(std::forward<TArgs>(_Args)...)
		{}
	};

	template <typename T>
	class shared_ptr
		: public shared_ptr_base<T>
	{
		typedef shared_ptr_base<T> base_type;
		typedef shared_ptr<T> this_type;
	public:
		shared_ptr(void)
			: base_type(nullptr, nullptr)
		{}
		shared_ptr(make_shared_ptr_count_t<T> *p)
			: base_type(p != nullptr ? p->get() : nullptr, p)
		{}
		shared_ptr(T *pt)
			: base_type(pt, new shared_ptr_count_t<T>(pt))
		{}
		template <typename TDeleter>
		shared_ptr(T *pt, TDeleter && tDeleter)
			: base_type(pt, new shared_ptr_count_t<T, TDeleter>(pt, std::forward<TDeleter>(tDeleter)))
		{}
		shared_ptr(shared_ptr<T> const & spThat)
			: base_type(spThat.m_pt, spThat.m_pCount)
		{
			if (m_pCount != nullptr)
				m_pCount->acquire_shared();
		}
		template <typename U>
		shared_ptr(shared_ptr<U> const & spThat)
			: base_type(spThat.m_pt, spThat.m_pCount)
		{
			if (m_pCount != nullptr)
				m_pCount->acquire_shared();
		}
		template <typename U>
		shared_ptr(shared_ptr<U> && spThat)
			: base_type(spThat.m_pt, spThat.m_pCount)
		{
			spThat.m_pt = nullptr;
			spThat.m_pCount = nullptr;
		}
		template <typename U>
		shared_ptr(shared_ptr<U> const & spThat, T *pt)
			: base_type(pt, spThat.m_pCount)
		{
			if (m_pCount != nullptr)
				m_pCount->acquire_shared();
		}
		template <typename U>
		shared_ptr(shared_ptr<U> && spThat, T *pt)
			: base_type(pt, spThat.m_pCount)
		{
			spThat.m_pt = nullptr;
			spThat.m_pCount = nullptr;
		}
		template <typename U>
		shared_ptr(weak_ptr_base<U> const & wpThat)
			: base_type(wpThat.m_pt, wpThat.m_pCount)
		{
			if (m_pCount == nullptr ||
				m_pCount->try_shared() == false)
			{
				m_pt = nullptr;
				m_pCount = nullptr;
			}
		}
		template <typename U>
		shared_ptr(weak_ptr_base<U> && wpThat)
			: base_type(wpThat.m_pt, wpThat.m_pCount)
		{
			wpThat.m_pt = nullptr;
			wpThat.m_pCount = nullptr;

			if (m_pCount == nullptr ||
				m_pCount->convert_weak_into_shared() == false)
			{
				m_pt = nullptr;
				m_pCount = nullptr;
			}
		}
		template <typename U>
		shared_ptr(weak_ptr_base<U> const & wpThat, T *pt)
			: base_type(pt, wpThat.m_pCount)
		{
			if (m_pCount == nullptr ||
				m_pCount->try_shared() == false)
			{
				m_pt = nullptr;
				m_pCount = nullptr;
			}
		}
		template <typename U>
		shared_ptr(weak_ptr_base<U> && wpThat, T *pt)
			: base_type(pt, wpThat.m_pCount)
		{
			wpThat.m_pt = nullptr;
			wpThat.m_pCount = nullptr;

			if (m_pCount == nullptr ||
				m_pCount->convert_weak_into_shared() == false)
			{
				m_pt = nullptr;
				m_pCount = nullptr;
			}
		}
		~shared_ptr(void)
		{
			if (m_pCount != nullptr)
				m_pCount->release_shared();
		}

		T * operator -> (void) const { return m_pt; }
		T & operator * (void) const { return *m_pt; }

		T * get(void) const { return m_pt; }
		operator bool(void) const { return m_pt != nullptr; }

		void swap(this_type & that)
		{
			std::swap(m_pt, that.m_pt);
			std::swap(m_pCount, that.m_pCount);
		}
		template<class... TArgs>
		void reset(TArgs &&... _Args)
		{
			swap(this_type(std::forward<TArgs>(_Args)...));
		}
	};
	template <typename T>
	class weak_ptr
		: public weak_ptr_base<T>
	{
		typedef weak_ptr_base<T> base_type;
		typedef weak_ptr<T> this_type;
	public:
		weak_ptr(weak_ptr<T> const & wpThat)
			: base_type(wpThat.m_pt, wpThat.m_pCount)
		{
			if (m_pCount != nullptr)
				m_pCount->acquire_weak();
		}
		template <typename U>
		weak_ptr(weak_ptr<U> const & wpThat)
			: base_type(wpThat.m_pt, wpThat.m_pCount)
		{
			if (m_pCount != nullptr)
				m_pCount->acquire_weak();
		}
		template <typename U>
		weak_ptr(weak_ptr<U> && wpThat)
			: base_type(wpThat.m_pt, wpThat.m_pCount)
		{
			wpThat.m_pt = nullptr;
			wpThat.m_pCount = nullptr;
		}
		template <typename U>
		weak_ptr(weak_ptr<U> const & wpThat, T *pt)
			: base_type(pt, wpThat.m_pCount)
		{
			if (m_pCount != nullptr)
				m_pCount->acquire_weak();
		}
		template <typename U>
		weak_ptr(weak_ptr<U> && wpThat, T *pt)
			: base_type(pt, wpThat.m_pCount)
		{
			wpThat.m_pt = nullptr;
			wpThat.m_pCount = nullptr;
		}
		template <typename U>
		weak_ptr(shared_ptr<U> const & spThat)
			: base_type(spThat.m_pt, spThat.m_pCount)
		{
			if (m_pCount != nullptr)
				m_pCount->acquire_weak();
		}
		template <typename U>
		weak_ptr(shared_ptr<U> && spThat)
			: base_type(spThat.m_pt, spThat.m_pCount)
		{
			spThat.m_pt = nullptr;
			spThat.m_pCount = nullptr;

			if (m_pCount != nullptr)
				m_pCount->convert_shared_into_weak();
		}
		template <typename U>
		weak_ptr(shared_ptr<U> const & spThat, T *pt)
			: base_type(pt, spThat.m_pCount)
		{
			if (m_pCount != nullptr)
				m_pCount->acquire_weak();
		}
		template <typename U>
		weak_ptr(shared_ptr<U> && spThat, T *pt)
			: base_type(pt, spThat.m_pCount)
		{
			spThat.m_pt = nullptr;
			spThat.m_pCount = nullptr;

			if (m_pCount != nullptr)
				m_pCount->convert_shared_into_weak();
		}
		~weak_ptr(void)
		{
			if (m_pCount != nullptr)
				m_pCount->release_weak();
		}

		bool expired(void) const
		{
			if (m_pCount != nullptr)
				return m_pCount->expired();
			else
				return true;
		}

		void swap(this_type & that)
		{
			std::swap(m_pt, that.m_pt);
			std::swap(m_pCount, that.m_pCount);
		}
		template<class... TArgs>
		void reset(TArgs &&... _Args)
		{
			swap(this_type(std::forward<TArgs>(_Args)...));
		}
	};
	template<typename T, class... TArgs>
	shared_ptr<T> make_shared(TArgs &&... _Args)
	{
		return std::move(shared_ptr<T>(new make_shared_ptr_count_t<T>(std::forward<TArgs>(_Args)...)));
	}
}
