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
	like::cpuid_t<0x00> cpuid00;
	__m128i xmmVendorID = cpuid00.vendor_identification_string();
	like::cpuid_t<0x01> cpuid01;
	like::cpuid_t<0x02> cpuid02;
	like::cpuid_t<0x04, 0> cpuid04_0;
	like::cpuid_t<0x04, 1> cpuid04_1;
	like::cpuid_t<0x04, 2> cpuid04_2;
	like::cpuid_t<0x04, 3> cpuid04_3;
	like::cpuid_t<0x04, 4> cpuid04_4;
	like::cpuid_t<0x05> cpuid05;
	like::cpuid_t<0x06> cpuid06;
	like::cpuid_t<0x07> cpuid07;
	like::cpuid_t<0x09> cpuid09;
	like::cpuid_t<0x0A> cpuid0A;
	like::cpuid_t<0x0B, 0> cpuid0B0;
	like::cpuid_t<0x0B, 1> cpuid0B1;
	like::cpuid_t<0x0B, 2> cpuid0B2;
	like::cpuid_t<0x0C> cpuid0C;
	like::cpuid_t<0x0D> cpuid0D;
	like::cpuid_t<0x0D, 1> cpuid0D1;
	like::cpuid_t<0x0D, 2> cpuid0D2_AVX;
	like::cpuid_t<0x0D, 3> cpuid0D3_BNDREG;
	like::cpuid_t<0x0D, 4> cpuid0D4_BNDCSR;
	like::cpuid_t<0x0D, 5> cpuid0D5_Opmask;
	like::cpuid_t<0x0D, 6> cpuid0D6_ZMM_Hi256;
	like::cpuid_t<0x0D, 7> cpuid0D7_Hi16_ZMM;
	like::cpuid_t<0x0D, 8> cpuid0D8_PT;
	like::cpuid_t<0x0D, 9> cpuid0D9_PKRU;
	like::cpuid_t<0x0F> cpuid0F;
	like::cpuid_t<0x0F, 1> cpuid0F1;
	like::cpuid_t<0x10> cpuid10;
	like::cpuid_t<0x10, 1> cpuid101;
	like::cpuid_t<0x14> cpuid14;
	like::cpuid_t<0x14, 1> cpuid141;
	like::cpuid_t<0x15> cpuid15;
	like::cpuid_t<0x16> cpuid16;
	like::cpuid_t<0x17> cpuid17;
	like::cpuid_soc_vendor_brand_string_t cpuidSocVendorBrandString;
	like::cpuid_t<0x80000000> cpuid80000000;
	like::cpuid_t<0x80000001> cpuid80000001;
	like::cpuid_t<0x80000002> cpuid80000002;
	like::cpuid_t<0x80000003> cpuid80000003;
	like::cpuid_t<0x80000004> cpuid80000004;
	like::cpuid_processor_brand_string_t cpuidProcessorBrandString;
	like::cpuid_t<0x80000006> cpuid80000006;
	like::cpuid_t<0x80000007> cpuid80000007;
	like::cpuid_t<0x80000008> cpuid80000008;

	char const *pcSocVendorBrandString = cpuidSocVendorBrandString;
	char const *pcProcessorBrandString = cpuidProcessorBrandString;

	std::cout <<
		std::setw(8) << "EAX" << std::ends <<
		std::setw(3) << "ECX" << std::ends <<
		std::setw(8) << "EAX" << std::ends <<
		std::setw(8) << "EBX" << std::ends <<
		std::setw(8) << "ECX" << std::ends <<
		std::setw(8) << "EDX" << std::endl <<
		cpuid00 <<
		cpuid01 <<
		cpuid04_0 <<
		cpuid04_1 <<
		cpuid04_2 <<
		cpuid04_3 <<
		cpuid04_4 <<
		cpuid05 <<
		cpuid06 <<
		cpuid07 <<
		cpuid09 <<
		cpuid0A <<
		cpuid0B0 <<
		cpuid0B1 <<
		cpuid0B2 <<
		cpuid0C <<
		cpuid0D <<
		cpuid0D1 <<
		cpuid0D2_AVX <<
		cpuid80000000 <<
		cpuid80000001 <<
		cpuid80000002 <<
		cpuid80000003 <<
		cpuid80000004 <<
		cpuid80000006 <<
		cpuid80000007 <<
		cpuid80000008
		;

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