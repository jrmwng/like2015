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

		template <typename T>
		void TestMethod_jrmwng_atomic_shared_ptr()
		{
			jrmwng::shared_ptr<T> sp0;
			jrmwng::shared_ptr<const T> spConst0;
			jrmwng::shared_ptr<volatile T> spVolatile0;
			jrmwng::shared_ptr<const volatile T> spConstVolatile0;
			{
				jrmwng::shared_ptr<T> sp;
				jrmwng::shared_ptr<const T> spConst;
				jrmwng::shared_ptr<volatile T> spVolatile;
				jrmwng::shared_ptr<const volatile T> spConstVolatile;

				sp = sp0;
				spConst = sp0;
				spVolatile = sp0;
				spConstVolatile = sp0;
				spConstVolatile = spConst0;
				spConstVolatile = spVolatile0;

				sp = std::move(sp0);
				spConst = std::move(sp0);
				spVolatile = std::move(sp0);
				spConstVolatile = std::move(sp0);
				spConstVolatile = std::move(spConst0);
				spConstVolatile = std::move(spVolatile0);

				sp.reset();
				spConst.reset();
				spVolatile.reset();
				spConstVolatile.reset();
			}
			jrmwng::shared_ptr<T> sp1(static_cast<T*>(nullptr));
			jrmwng::shared_ptr<const T> spConst1(static_cast<T*>(nullptr));
			jrmwng::shared_ptr<volatile T> spVolatile1(static_cast<T*>(nullptr));
			jrmwng::shared_ptr<const volatile T> spConstVolatile1(static_cast<T*>(nullptr));
			{
				jrmwng::shared_ptr<T> sp;
				jrmwng::shared_ptr<const T> spConst;
				jrmwng::shared_ptr<volatile T> spVolatile;
				jrmwng::shared_ptr<const volatile T> spConstVolatile;

				sp = sp1;
				spConst = sp1;
				spVolatile = sp1;
				spConstVolatile = sp1;
				spConstVolatile = spConst1;
				spConstVolatile = spVolatile1;

				sp = std::move(sp1);
				spConst = std::move(sp1);
				spVolatile = std::move(sp1);
				spConstVolatile = std::move(sp1);
				spConstVolatile = std::move(spConst1);
				spConstVolatile = std::move(spVolatile1);

				sp.reset(static_cast<T*>(nullptr));
				spConst.reset(static_cast<T*>(nullptr));
				spVolatile.reset(static_cast<T*>(nullptr));
				spConstVolatile.reset(static_cast<T*>(nullptr));
			}
			jrmwng::shared_ptr<T> sp2(new T);
			jrmwng::shared_ptr<const T> spConst2(new T);
			jrmwng::shared_ptr<volatile T> spVolatile2(new T);
			jrmwng::shared_ptr<const volatile T> spConstVolatile2(new T);
			{
				jrmwng::shared_ptr<T> sp(sp2);
				jrmwng::shared_ptr<const T> spConst(spConst2);
				jrmwng::shared_ptr<volatile T> spVolatile(spVolatile2);
				jrmwng::shared_ptr<const volatile T> spConstVolatile(spConstVolatile2);

				sp = sp2;
				spConst = spConst2;
				spVolatile = spVolatile2;
				spConstVolatile = spConstVolatile2;

				sp.reset(new T);
				spConst.reset(new T);
				spVolatile.reset(new T);
				spConstVolatile.reset(new T);

				sp.reset(sp);
				spConst.reset(sp);
				spConst.reset(spConst);
				spVolatile.reset(sp);
				spVolatile.reset(spVolatile);
				spConstVolatile.reset(sp);
				spConstVolatile.reset(spConst);
				spConstVolatile.reset(spVolatile);
				spConstVolatile.reset(spConstVolatile);
			}
			jrmwng::shared_ptr<T> sp3(new T);
			jrmwng::shared_ptr<const T> spConst3(new T);
			jrmwng::shared_ptr<volatile T> spVolatile3(new T);
			jrmwng::shared_ptr<const volatile T> spConstVolatile3(new T);
			{
				jrmwng::shared_ptr<T> sp(std::move(sp3));
				jrmwng::shared_ptr<const T> spConst(std::move(spConst3));
				jrmwng::shared_ptr<volatile T> spVolatile(std::move(spVolatile3));
				jrmwng::shared_ptr<const volatile T> spConstVolatile(std::move(spConstVolatile3));

				sp3 = std::move(sp);
				spConst3 = std::move(spConst);
				spVolatile3 = std::move(spVolatile);
				spConstVolatile3 = std::move(spConstVolatile);

				sp.reset(std::move(sp3));
				spConst.reset(std::move(spConst3));
				spConst.reset(std::move(sp3));
				spVolatile.reset(std::move(spVolatile3));
				spVolatile.reset(std::move(sp3));
				spConstVolatile.reset(std::move(spConstVolatile3));
				spConstVolatile.reset(std::move(spVolatile3));
				spConstVolatile.reset(std::move(spConst3));
				spConstVolatile.reset(std::move(sp3));
			}
			jrmwng::shared_ptr<T> sp4(new T);
			jrmwng::shared_ptr<const T> spConst4(new T);
			jrmwng::shared_ptr<volatile T> spVolatile4(new T);
			jrmwng::shared_ptr<const volatile T> spConstVolatile4(new T);
			{
				jrmwng::shared_ptr<T> sp(std::move(sp4));
				jrmwng::shared_ptr<const T> spConst(std::move(spConst4));
				jrmwng::shared_ptr<volatile T> spVolatile(std::move(spVolatile4));
				jrmwng::shared_ptr<const volatile T> spConstVolatile(std::move(spConstVolatile4));

				sp4 = std::move(sp);
				spConst4 = std::move(spConst);
				spVolatile4 = std::move(spVolatile);
				spConstVolatile4 = std::move(spConstVolatile);
			}
		}
		template <>
		void TestMethod_jrmwng_atomic_shared_ptr<void>()
		{
			jrmwng::shared_ptr<void> sp0;
			jrmwng::shared_ptr<const void> spConst0;
			jrmwng::shared_ptr<volatile void> spVolatile0;
			jrmwng::shared_ptr<const volatile void> spConstVolatile0;
			{
				spConst0 = sp0;
				spVolatile0 = sp0;
				spConstVolatile0 = sp0;
				spConstVolatile0 = spConst0;
				spConstVolatile0 = spVolatile0;
			}
			jrmwng::shared_ptr<void> sp1(static_cast<void*>(nullptr));
			jrmwng::shared_ptr<const void> spConst1(static_cast<void*>(nullptr));
			jrmwng::shared_ptr<volatile void> spVolatile1(static_cast<void*>(nullptr));
			jrmwng::shared_ptr<const volatile void> spConstVolatile1(static_cast<void*>(nullptr));
			{
				spConst1 = sp1;
				spVolatile1 = sp1;
				spConstVolatile1 = sp1;
				spConstVolatile1 = spConst1;
				spConstVolatile1 = spVolatile1;
			}
		}
	public:
		TEST_METHOD(TestMethod_jrmwng_atomic_shared_ptr)
		{
			TestMethod_jrmwng_atomic_shared_ptr<void>();
			TestMethod_jrmwng_atomic_shared_ptr<int>();
		}

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

		TEST_METHOD(TestMethod_jrmwng_tiff)
		{
			jrmwng::tiff_field<jrmwng::TIFF_TAG_IMAGE_WIDTH> stImageWidthField;
			jrmwng::tiff_field<jrmwng::TIFF_TAG_IMAGE_HEIGHT> stImageHeightField;
			jrmwng::tiff_field<jrmwng::TIFF_TAG_ROWS_PER_STRIP> stRowsPerStripField;
			jrmwng::tiff_field<jrmwng::TIFF_TAG_STRIP_OFFSETS> stStripOffsetsField;
			jrmwng::tiff_field<jrmwng::TIFF_TAG_X_RESOLUTION> stXResolutionField;
			jrmwng::tiff_field<jrmwng::TIFF_TAG_Y_RESOLUTION> stYResolutionField;
			jrmwng::tiff_field<jrmwng::TIFF_TAG_RESOLUTION_UNIT> stResolutionUnitField;

			typedef jrmwng::tiff_field<jrmwng::TIFF_TAG_IMAGE_WIDTH, jrmwng::TIFF_TAG_IMAGE_HEIGHT> image_size_field_t;
			image_size_field_t stImageSizeField;
			typedef jrmwng::tiff_field<jrmwng::TIFF_TAG_ROWS_PER_STRIP, jrmwng::TIFF_TAG_STRIP_OFFSETS> strip_field_t;
			strip_field_t stStripField;
			typedef jrmwng::tiff_field<jrmwng::TIFF_TAG_X_RESOLUTION, jrmwng::TIFF_TAG_Y_RESOLUTION, jrmwng::TIFF_TAG_RESOLUTION_UNIT> resolution_field_t;
			resolution_field_t stResolutionField;

			typedef jrmwng::tiff_subfile<image_size_field_t> image_size_subfile_t;
			image_size_subfile_t stImageSizeSubfile;
			typedef jrmwng::tiff_subfile<strip_field_t> strip_subfile_t;
			strip_subfile_t stStripSubfile;
			typedef jrmwng::tiff_subfile<resolution_field_t> resolution_subfile_t;
			resolution_subfile_t stResolutionSubfile;
			typedef jrmwng::tiff_subfile<jrmwng::tiff_baseline_fields> baseline_subfile_t;
			baseline_subfile_t stBaselineSubfile;

			//

			jrmwng::tiff_baseline_file stBaselineFile;
			{
				stBaselineFile.set<jrmwng::TIFF_TAG_IMAGE_WIDTH>(2560U);
				stBaselineFile.set<jrmwng::TIFF_TAG_IMAGE_HEIGHT>(1920U);
				stBaselineFile.set<jrmwng::TIFF_TAG_ROWS_PER_STRIP>(1920U);
				stBaselineFile.set<jrmwng::TIFF_TAG_STRIP_OFFSETS>(123); // TODO
				stBaselineFile.set<jrmwng::TIFF_TAG_RESOLUTION_UNIT>(jrmwng::TIFF_RESOLUTION_UNIT_CENTIMETER);
			}
		}

		TEST_METHOD(TestMethod_jrmwng_jpeg)
		{
			jrmwng::jpeg_marker_s<jrmwng::JPEG_MARKER_SOS,1> stJpegSOS_1;
			jrmwng::jpeg_marker_s<jrmwng::JPEG_MARKER_DAC, 16, 16> stJpegDAC_16_16;
			jrmwng::jpeg_marker_s<jrmwng::JPEG_MARKER_DHT, 16, 16> stJpegDHT_16_16;
			jrmwng::jpeg_marker_s<jrmwng::JPEG_MARKER_DQT, jrmwng::JPEG_QUANT_BYTE, 8> stJpegDQT_ub64;
			jrmwng::jpeg_marker_s<jrmwng::JPEG_MARKER_DRI> stJpegDRI;
		}
	};
}