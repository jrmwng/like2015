#include <ppl.h>
#include <assert.h>
#include <future>
#include "shared_ptr.h"
#include "shared_lock.h"
#include "atomic.h"
#include "xmm_ptr.h"
#include "xmm_int32.h"
#include "cpuid.h"

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
	like::cpuid_tree_t<0x17> stCPUID;
	like::cpuid_tree_t<0x80000008> stExtendedCPUID;

	__m128i const xmmVendorIdentString = stCPUID.vendor_identification_string();
	char const *pcSocVendorBrandString = stCPUID.soc_vendor_brand_string();
	char const *pcProcessorBrandString = stExtendedCPUID.processor_brand_string();

	std::cout << xmmVendorIdentString.m128i_i8 << std::endl;
	std::cout << pcSocVendorBrandString << std::endl;
	std::cout << pcProcessorBrandString << std::endl;

	std::cout <<
		std::setw(8) << "EAX" << std::ends <<
		std::setw(3) << "ECX" << std::ends <<
		std::setw(8) << "EAX" << std::ends <<
		std::setw(8) << "EBX" << std::ends <<
		std::setw(8) << "ECX" << std::ends <<
		std::setw(8) << "EDX" << std::endl <<
		stCPUID <<
		stExtendedCPUID;

 	like::xmm_int32<1> xmmInt1(1);
	like::xmm_int32<2> xmmInt2(1, 2);
	like::xmm_int32<3> xmmInt3(1, 2, 3);
	like::xmm_int32<4> xmmInt4(1, 2, 3, 4);
	like::xmm_int32<5> xmmInt5(1, 2, 3, 4, 5);
	like::xmm_int32<8> xmmInt8(1, 2, 3, 4, 5, 6, 7, 8);
	like::xmm_int32<9> xmmInt9(1, 2, 3, 4, 5, 6, 7, 8, 9);

	assert(like::get_int<0>(xmmInt1) == 1);
	assert(like::get_int<0>(xmmInt2) == 1);
	assert(like::get_int<1>(xmmInt2) == 2);
	assert(like::get_int<0>(xmmInt3) == 1);
	like::get_int<1>(xmmInt3);
	like::get_int<2>(xmmInt3);
	like::get_int<0>(xmmInt4);
	like::get_int<1>(xmmInt4);
	like::get_int<2>(xmmInt4);
	like::get_int<3>(xmmInt4);
	like::get_int<0>(xmmInt5);
	like::get_int<1>(xmmInt5);
	like::get_int<2>(xmmInt5);
	like::get_int<3>(xmmInt5);
	like::get_int<4>(xmmInt5);
	like::get_int<0>(xmmInt8);
	like::get_int<1>(xmmInt8);
	like::get_int<2>(xmmInt8);
	like::get_int<3>(xmmInt8);
	like::get_int<4>(xmmInt8);
	like::get_int<5>(xmmInt8);
	like::get_int<6>(xmmInt8);
	like::get_int<7>(xmmInt8);
	like::get_int<0>(xmmInt9);
	like::get_int<1>(xmmInt9);
	like::get_int<2>(xmmInt9);
	like::get_int<3>(xmmInt9);
	like::get_int<4>(xmmInt9);
	like::get_int<5>(xmmInt9);
	like::get_int<6>(xmmInt9);
	like::get_int<7>(xmmInt9);
	like::get_int<8>(xmmInt9);

	like::xmm_ptr<int> xmmPtr1(&nArgc);
	like::xmm_ptr<int, int> xmmPtr2(&nArgc, &nArgc + 1);
	like::xmm_ptr<int, int, int> xmmPtr3(&nArgc, &nArgc + 1, &nArgc + 2);
	like::xmm_ptr<int, int, int, int> xmmPtr4(&nArgc, &nArgc + 1, &nArgc + 2, &nArgc + 3);
	like::xmm_ptr<int, int, int, int, int> xmmPtr5(&nArgc, &nArgc + 1, &nArgc + 2, &nArgc + 3, &nArgc + 4);
	like::xmm_ptr<int, int, int, int, int, int, int, int> xmmPtr8(&nArgc, &nArgc + 1, &nArgc + 2, &nArgc + 3, &nArgc + 4, &nArgc + 5, &nArgc + 6, &nArgc + 7);
	like::xmm_ptr<int, int, int, int, int, int, int, int, int> xmmPtr9(&nArgc, &nArgc + 1, &nArgc + 2, &nArgc + 3, &nArgc + 4, &nArgc + 5, &nArgc + 6, &nArgc + 7, &nArgc + 8);

	assert(like::get_ptr<0>(xmmPtr1) == &nArgc);
	assert(like::get_ptr<0>(xmmPtr2) == &nArgc);
	assert(like::get_ptr<0>(xmmPtr2) == like::get_ptr<0>(xmmPtr1));
	assert(like::get_ptr<1>(xmmPtr2) == &nArgc + 1);
	assert(like::get_ptr<1>(xmmPtr2) != like::get_ptr<0>(xmmPtr2));
	assert(like::get_ptr<0>(xmmPtr3) == &nArgc);
	like::get_ptr<1>(xmmPtr3);
	like::get_ptr<2>(xmmPtr3);
	like::get_ptr<0>(xmmPtr4);
	like::get_ptr<1>(xmmPtr4);
	like::get_ptr<2>(xmmPtr4);
	like::get_ptr<3>(xmmPtr4);
	like::get_ptr<0>(xmmPtr5);
	like::get_ptr<1>(xmmPtr5);
	like::get_ptr<2>(xmmPtr5);
	like::get_ptr<3>(xmmPtr5);
	assert(like::get_ptr<4>(xmmPtr5) == &nArgc + 4);
	like::get_ptr<0>(xmmPtr8);
	like::get_ptr<1>(xmmPtr8);
	like::get_ptr<2>(xmmPtr8);
	like::get_ptr<3>(xmmPtr8);
	like::get_ptr<4>(xmmPtr8);
	like::get_ptr<5>(xmmPtr8);
	like::get_ptr<6>(xmmPtr8);
	like::get_ptr<7>(xmmPtr8);
	like::get_ptr<0>(xmmPtr9);
	like::get_ptr<1>(xmmPtr9);
	like::get_ptr<2>(xmmPtr9);
	like::get_ptr<3>(xmmPtr9);
	like::get_ptr<4>(xmmPtr9);
	like::get_ptr<5>(xmmPtr9);
	like::get_ptr<6>(xmmPtr9);
	assert(like::get_ptr<7>(xmmPtr9) == &nArgc + 7);
	assert(like::get_ptr<8>(xmmPtr9) == &nArgc + 8);

	xmmPtr1 += like::xmm_ptr<int>(1) + like::xmm_ptr<int>(2) - like::xmm_ptr<int>(3) * 1;
	assert(like::get_ptr<0>(xmmPtr1) == &nArgc);
	xmmPtr1 -= like::xmm_ptr<int>(0);
	assert(like::get_ptr<0>(xmmPtr1) == &nArgc);
	xmmPtr2 += like::xmm_ptr<int, int>(1, 2) + like::xmm_ptr<int, int>(2, 1) - like::xmm_ptr<int, int>(3, 3) * 1;
	xmmPtr2 -= like::xmm_ptr<int, int>(0, 0);
	xmmPtr3 += like::xmm_ptr<int, int, int>(1, 2, 1) + like::xmm_ptr<int, int, int>(2, 1, 2) - like::xmm_ptr<int, int, int>(3, 3, 3) * 1;
	xmmPtr3 -= like::xmm_ptr<int, int, int>(0, 0, 0);
	xmmPtr4 += like::xmm_ptr<int, int, int, int>(1, 2, 1, 2) + like::xmm_ptr<int, int, int, int>(2, 1, 2, 1) - like::xmm_ptr<int, int, int, int>(3, 3, 3, 3) * 1;
	xmmPtr4 -= like::xmm_ptr<int, int, int, int>(0, 0, 0, 0);
	xmmPtr5 += like::xmm_ptr<int, int, int, int, int>(1, 2, 1, 2, 1) + like::xmm_ptr<int, int, int, int, int>(2, 1, 2, 1, 2) - like::xmm_ptr<int, int, int, int, int>(3, 3, 3, 3, 3) * 1;
	xmmPtr5 -= like::xmm_ptr<int, int, int, int, int>(0, 0, 0, 0, 0);
	xmmPtr8 += like::xmm_ptr<int, int, int, int, int, int, int, int>(1, 2, 1, 2, 1, 2, 1, 2) + like::xmm_ptr<int, int, int, int, int, int, int, int>(2, 1, 2, 1, 2, 1, 2, 1) - like::xmm_ptr<int, int, int, int, int, int, int, int>(3, 3, 3, 3, 3, 3, 3, 3) * 1;
	xmmPtr8 -= like::xmm_ptr<int, int, int, int, int, int, int, int>(0, 0, 0, 0, 0, 0, 0, 0);
	xmmPtr9 += like::xmm_ptr<int, int, int, int, int, int, int, int, int>(1, 2, 1, 2, 1, 2, 1, 2, 1) + like::xmm_ptr<int, int, int, int, int, int, int, int, int>(2, 1, 2, 1, 2, 1, 2, 1, 2) - like::xmm_ptr<int, int, int, int, int, int, int, int, int>(3, 3, 3, 3, 3, 3, 3, 3, 3) * 1;
	assert(like::get_ptr<8>(xmmPtr9) == &nArgc + 8);
	xmmPtr9 -= like::xmm_ptr<int, int, int, int, int, int, int, int, int>(0, 0, 0, 0, 0, 0, 0, 0, 0);
	assert(like::get_ptr<8>(xmmPtr9) == &nArgc + 8);

	assert(like::get_ptr<0>(xmmPtr1) == &nArgc);
	assert((like::get_ptr<0>(xmmPtr1) += 1) == &nArgc + 1);
	assert((like::get_ptr<0>(xmmPtr1) -= 1) == &nArgc);
	assert(like::get_ptr<0>(xmmPtr2) == &nArgc);
	assert((like::get_ptr<0>(xmmPtr2) += 1) == &nArgc + 1);
	assert((like::get_ptr<0>(xmmPtr2) -= 1) == &nArgc);
	assert(like::get_ptr<1>(xmmPtr2) == &nArgc + 1);
	assert((like::get_ptr<1>(xmmPtr2) += 1) == &nArgc + 2);
	assert((like::get_ptr<1>(xmmPtr2) -= 1) == &nArgc + 1);

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

			for (int i = 0; i < n; i++)
				nRET += i + *sp;

			//_mm_pause();
#else
			like_weak_ptr<int> wp(sp4);
			like_shared_ptr<int> sp(std::move(wp));

			nRET += n + *sp;

			wp = std::move(sp);
#endif
		};

		int const N = 1000;
		unsigned long long uxl0 = __rdtsc();
		for (int n = 0; n < N; n++)
			_Test(n);
		unsigned long long uxl1 = __rdtsc();
		concurrency::parallel_for(1, N, _Test, concurrency::static_partitioner());
		unsigned long long uxl2 = __rdtsc();

		printf("%I64u\n", uxl1 - uxl0);
		printf("%I64u\n", uxl2 - uxl1);
	}

	return nRET;
}