#include <ppl.h>
#include <future>
#include "shared_ptr.h"
#include "shared_lock.h"
#include "atomic.h"

#if 0
template <typename T>
using like_shared_ptr = std::shared_ptr<T>;
template <typename T>
using like_weak_ptr = std::weak_ptr<T>;

template<typename T, class... TArgs>
like_shared_ptr<T> like_make_shared(TArgs &&... _Args)
{
	return std::move(std::make_shared<T>(std::forward<TArgs>(_Args)...));
}
#elif 1
template <typename T>
using like_shared_ptr = like::shared_ptr<T, like::shared_ptr_count>;
template <typename T>
using like_weak_ptr = like::weak_ptr<T, like::shared_ptr_count>;
template <typename T>
using like_atomic_shared_ptr = like::atomic_shared_ptr<T, like::shared_ptr_count>;

template<typename T, class... TArgs>
like::shared_ptr<T, like::shared_ptr_count> like_make_shared(TArgs &&... _Args)
{
	return std::move(like::make_shared<T, like::shared_ptr_count>(std::forward<TArgs>(_Args)...));
}
#else
template <typename T>
using like_shared_ptr = like::shared_ptr<T, like::shared_ptr_lock>;
template <typename T>
using like_weak_ptr = like::weak_ptr<T, like::shared_ptr_lock>;

template<typename T, class... TArgs>
like::shared_ptr<T, like::shared_ptr_lock> like_make_shared(TArgs &&... _Args)
{
	return std::move(like::make_shared<T, like::shared_ptr_lock>(std::forward<TArgs>(_Args)...));
}
#endif

struct SMain;
struct SMain
	: like::enable_shared_from_this<SMain>
{};

int main(int nArgc)
{
	int nRET = nArgc;
	{
		like_shared_ptr<SMain> spMain0(new SMain);
		like_shared_ptr<SMain> spMain1(spMain0->shared_from_this());
		like_shared_ptr<int> sp0(std::move(like_make_shared<int>(nArgc)));
		like_shared_ptr<int> sp0a(new int(nArgc));
		like_shared_ptr<int> sp0b(new int(nArgc), [=](int *p)
		{
			delete p;
			//throw nArgc;
		});

		like_weak_ptr<int> wp1(sp0);
		like_shared_ptr<int> sp1(sp0);

		like_weak_ptr<int> wp2(wp1);
		like_shared_ptr<int> sp2(wp1);

		like_weak_ptr<int> wp3(std::move(wp1));
		like_shared_ptr<int> sp3(std::move(wp2));

		like_weak_ptr<int> wp4(std::move(sp1));
		like_shared_ptr<int> sp4(std::move(sp2));

		//like_weak_ptr<int> wp5(sp4, sp4.get());
		//like_shared_ptr<int> sp5(sp4, sp4.get());

		//like_weak_ptr<int> wp6(wp5, sp5.get());
		//like_shared_ptr<int> sp6(wp5, sp5.get());

		//like_weak_ptr<int> wp7(std::move(wp5), sp5.get());
		//like_shared_ptr<int> sp7(std::move(wp6), sp6.get());

		//like_weak_ptr<int> wp8(std::move(sp5), sp5.get());
		//like_shared_ptr<int> sp8(std::move(sp6), sp6.get());

		like::shared_lock<1> sl;

		sl.read_begin(0);
		sl.read_end(0);
		sl.read_sync();

		__m128i xmm0 = _mm_setzero_si128();

#ifdef _M_X64
		__m128i xmm1 = like::atomic_load(&xmm0);
		like::atomic_cas(&xmm0, xmm1, xmm1);
		like::atomic_swap(&xmm0, xmm1);
		like::atomic_store(&xmm0, xmm1);
#endif

		long long xl1 = like::atomic_load(xmm0.m128i_i64);
		like::atomic_cas(xmm0.m128i_i64, xl1, xl1);
		like::atomic_swap(xmm0.m128i_i64, xl1);
		like::atomic_store(xmm0.m128i_i64, xl1);

		int l1 = like::atomic_load(xmm0.m128i_i32);
		like::atomic_cas(xmm0.m128i_i32, l1, l1);
		like::atomic_swap(xmm0.m128i_i32, l1);
		like::atomic_store(xmm0.m128i_i32, l1);

		short w1 = like::atomic_load(xmm0.m128i_i16);
		like::atomic_cas(xmm0.m128i_i16, w1, w1);
		like::atomic_swap(xmm0.m128i_i16, w1);
		like::atomic_store(xmm0.m128i_i16, w1);

		char b1 = like::atomic_load(xmm0.m128i_i8);
		like::atomic_cas(xmm0.m128i_i8, b1, b1);
		like::atomic_swap(xmm0.m128i_i8, b1);
		like::atomic_store(xmm0.m128i_i8, b1);

		like_atomic_shared_ptr<int> spX(sp4);

		auto _Test = [&](int n)
		{
			//like::shared_ptr_lock::acquire();
			//like::shared_obj_lock::acquire();
#if 1
			like_shared_ptr<int> sp(std::move(spX.load()));

			nRET += n + *sp;
#else
			like_weak_ptr<int> wp(sp4);
			like_shared_ptr<int> sp(std::move(wp));

			nRET += n + *sp;

			wp = std::move(sp);
#endif
		};

		unsigned long long uxl0 = __rdtsc();
		for (int n = 0; n < 1000000; n++)
			_Test(n);
		unsigned long long uxl1 = __rdtsc();
		concurrency::parallel_for(1, 1000000, _Test, concurrency::static_partitioner());
		unsigned long long uxl2 = __rdtsc();

		printf("%I64u\n", uxl1 - uxl0);
		printf("%I64u\n", uxl2 - uxl1);
	}

	return nRET;
}