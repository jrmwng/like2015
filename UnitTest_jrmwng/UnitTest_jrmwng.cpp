#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_jrmwng
{		
	TEST_CLASS(UnitTest_jrmwng)
	{
		template <typename T, typename U = T>
		void TestMethod_jrmwng_atomic(T t, U u)
		{
			t = jrmwng::atomic_load(&t);
			t = jrmwng::atomic_lock_load(&t);
			t = jrmwng::atomic_rtm_load(&t);

			jrmwng::atomic_store(&t, t);
			jrmwng::atomic_lock_store(&t, t);
			jrmwng::atomic_rtm_store(&t, t);

			t = jrmwng::atomic_exchange(&t, u);

			jrmwng::atomic_swap(&t, t);
			jrmwng::atomic_lock_swap(&t, t);
			jrmwng::atomic_rtm_swap(&t, t);

			jrmwng::atomic_cas(&t, t, t);
			jrmwng::atomic_lock_cas(&t, t, t);
			jrmwng::atomic_rtm_cas(&t, t, t);
		}
	public:
		TEST_METHOD(TestMethod_jrmwng_atomic)
		{
			TestMethod_jrmwng_atomic<char>(0, 0);
			TestMethod_jrmwng_atomic<char, signed char>(0, 0);
			TestMethod_jrmwng_atomic<signed char>(0, 0);
			TestMethod_jrmwng_atomic<signed char, char>(0, 0);
			TestMethod_jrmwng_atomic<unsigned char>(0, 0);

			TestMethod_jrmwng_atomic<short>(0, 0);
			TestMethod_jrmwng_atomic<short, char>(0, 0);
			TestMethod_jrmwng_atomic<short, signed char>(0, 0);
			TestMethod_jrmwng_atomic<short, unsigned char>(0, 0);
			TestMethod_jrmwng_atomic<short, signed short>(0, 0);
			TestMethod_jrmwng_atomic<signed short>(0, 0);
			TestMethod_jrmwng_atomic<signed short, char>(0, 0);
			TestMethod_jrmwng_atomic<signed short, signed char>(0, 0);
			TestMethod_jrmwng_atomic<signed short, unsigned char>(0, 0);
			TestMethod_jrmwng_atomic<signed short, char>(0, 0);
			TestMethod_jrmwng_atomic<signed short, signed char>(0, 0);
			TestMethod_jrmwng_atomic<signed short, unsigned char>(0, 0);
			TestMethod_jrmwng_atomic<signed short, short>(0, 0);
			TestMethod_jrmwng_atomic<unsigned short>(0, 0);
			TestMethod_jrmwng_atomic<unsigned short, unsigned char>(0, 0);

			TestMethod_jrmwng_atomic<int>(0, 0);
			TestMethod_jrmwng_atomic<int, char>(0, 0);
			TestMethod_jrmwng_atomic<int, signed char>(0, 0);
			TestMethod_jrmwng_atomic<int, unsigned char>(0, 0);
			TestMethod_jrmwng_atomic<int, short>(0, 0);
			TestMethod_jrmwng_atomic<int, signed short>(0, 0);
			TestMethod_jrmwng_atomic<int, unsigned short>(0, 0);
			TestMethod_jrmwng_atomic<int, signed int>(0, 0);
			TestMethod_jrmwng_atomic<signed int>(0, 0);
			TestMethod_jrmwng_atomic<signed int, char>(0, 0);
			TestMethod_jrmwng_atomic<signed int, signed char>(0, 0);
			TestMethod_jrmwng_atomic<signed int, unsigned char>(0, 0);
			TestMethod_jrmwng_atomic<signed int, short>(0, 0);
			TestMethod_jrmwng_atomic<signed int, signed short>(0, 0);
			TestMethod_jrmwng_atomic<signed int, unsigned short>(0, 0);
			TestMethod_jrmwng_atomic<signed int, signed int>(0, 0);
			TestMethod_jrmwng_atomic<signed int, long>(0, 0);
			TestMethod_jrmwng_atomic<signed int, signed long>(0, 0);
			TestMethod_jrmwng_atomic<unsigned int>(0, 0);
			TestMethod_jrmwng_atomic<unsigned int, unsigned char>(0, 0);
			TestMethod_jrmwng_atomic<unsigned int, unsigned short>(0, 0);
			TestMethod_jrmwng_atomic<unsigned int, unsigned long>(0, 0);

			TestMethod_jrmwng_atomic<long>(0, 0);
			TestMethod_jrmwng_atomic<long, char>(0, 0);
			TestMethod_jrmwng_atomic<long, signed char>(0, 0);
			TestMethod_jrmwng_atomic<long, unsigned char>(0, 0);
			TestMethod_jrmwng_atomic<long, short>(0, 0);
			TestMethod_jrmwng_atomic<long, signed short>(0, 0);
			TestMethod_jrmwng_atomic<long, unsigned short>(0, 0);
			TestMethod_jrmwng_atomic<long, signed long>(0, 0);
			TestMethod_jrmwng_atomic<signed long>(0, 0);
			TestMethod_jrmwng_atomic<signed long, char>(0, 0);
			TestMethod_jrmwng_atomic<signed long, signed char>(0, 0);
			TestMethod_jrmwng_atomic<signed long, unsigned char>(0, 0);
			TestMethod_jrmwng_atomic<signed long, short>(0, 0);
			TestMethod_jrmwng_atomic<signed long, signed short>(0, 0);
			TestMethod_jrmwng_atomic<signed long, unsigned short>(0, 0);
			TestMethod_jrmwng_atomic<signed long, signed long>(0, 0);
			TestMethod_jrmwng_atomic<signed long, int>(0, 0);
			TestMethod_jrmwng_atomic<signed long, signed int>(0, 0);
			TestMethod_jrmwng_atomic<unsigned long>(0, 0);
			TestMethod_jrmwng_atomic<unsigned long, unsigned char>(0, 0);
			TestMethod_jrmwng_atomic<unsigned long, unsigned short>(0, 0);
			TestMethod_jrmwng_atomic<unsigned long, unsigned int>(0, 0);

			TestMethod_jrmwng_atomic<long long>(0, 0);
			TestMethod_jrmwng_atomic<long long, char>(0, 0);
			TestMethod_jrmwng_atomic<long long, signed char>(0, 0);
			TestMethod_jrmwng_atomic<long long, unsigned char>(0, 0);
			TestMethod_jrmwng_atomic<long long, short>(0, 0);
			TestMethod_jrmwng_atomic<long long, signed short>(0, 0);
			TestMethod_jrmwng_atomic<long long, unsigned short>(0, 0);
			TestMethod_jrmwng_atomic<long long, int>(0, 0);
			TestMethod_jrmwng_atomic<long long, signed int>(0, 0);
			TestMethod_jrmwng_atomic<long long, unsigned int>(0, 0);
			TestMethod_jrmwng_atomic<long long, long>(0, 0);
			TestMethod_jrmwng_atomic<long long, signed long>(0, 0);
			TestMethod_jrmwng_atomic<long long, unsigned long>(0, 0);
			TestMethod_jrmwng_atomic<long long, signed long long>(0, 0);
			TestMethod_jrmwng_atomic<signed long long>(0, 0);
			TestMethod_jrmwng_atomic<signed long long, char>(0, 0);
			TestMethod_jrmwng_atomic<signed long long, signed char>(0, 0);
			TestMethod_jrmwng_atomic<signed long long, unsigned char>(0, 0);
			TestMethod_jrmwng_atomic<signed long long, short>(0, 0);
			TestMethod_jrmwng_atomic<signed long long, signed short>(0, 0);
			TestMethod_jrmwng_atomic<signed long long, unsigned short>(0, 0);
			TestMethod_jrmwng_atomic<signed long long, int>(0, 0);
			TestMethod_jrmwng_atomic<signed long long, signed int>(0, 0);
			TestMethod_jrmwng_atomic<signed long long, unsigned int>(0, 0);
			TestMethod_jrmwng_atomic<signed long long, long>(0, 0);
			TestMethod_jrmwng_atomic<signed long long, signed long>(0, 0);
			TestMethod_jrmwng_atomic<signed long long, unsigned long>(0, 0);
			TestMethod_jrmwng_atomic<signed long long, signed long long>(0, 0);
			TestMethod_jrmwng_atomic<signed long long, long>(0, 0);
			TestMethod_jrmwng_atomic<signed long long, signed long>(0, 0);
			TestMethod_jrmwng_atomic<unsigned long long>(0, 0);
			TestMethod_jrmwng_atomic<unsigned long long, unsigned char>(0, 0);
			TestMethod_jrmwng_atomic<unsigned long long, unsigned short>(0, 0);
			TestMethod_jrmwng_atomic<unsigned long long, unsigned int>(0, 0);
			TestMethod_jrmwng_atomic<unsigned long long, unsigned long>(0, 0);

			TestMethod_jrmwng_atomic<__m128i>(_mm_setzero_si128(), _mm_setzero_si128());

			TestMethod_jrmwng_atomic<std::pair<long long, long long>>(std::pair<long long, long long>(0, 0), std::pair<long long, long long>(0, 0));
		}

		TEST_METHOD(TestMethod_jrmwng_align)
		{
			auto const stAlignPage = jrmwng::align_page(0);
			auto const stAlignSect = jrmwng::align_section(0);
			auto const stAlignLine = jrmwng::align_line(0);
			auto const stAlignLock = jrmwng::align_lock(0);

			auto const uAlignPage = reinterpret_cast<uintptr_t>(&stAlignPage);
			auto const uAlignSect = reinterpret_cast<uintptr_t>(&stAlignSect);
			auto const uAlignLine = reinterpret_cast<uintptr_t>(&stAlignLine);
			auto const uAlignLock = reinterpret_cast<uintptr_t>(&stAlignLock);
		}

	};
}