#pragma once

/* Author: jrmwng @ 2016 */

#include <intrin.h>
#include <array>
#include <atomic>
#include <utility>
#include <algorithm>
#include <functional>

namespace jrmwng
{
	namespace
	{
		template <typename T = void>
		struct allocator32x_avg16;
		template <>
		struct allocator32x_avg16<unsigned>
		{
			typedef unsigned first_argument_type;
			typedef unsigned second_argument_type;
			typedef unsigned result_type;

			unsigned operator () (unsigned const & t1, unsigned const & t2) const
			{
				return _mm_cvtsi128_si32(_mm_avg_epu16(_mm_cvtsi32_si128(t1), _mm_cvtsi32_si128(t2)));
			}
		};

		template <template <typename T> class Top, typename... Tops>
		struct allocator32x_op;

		struct allocator32x_base
		{};

		template <typename Tmm, unsigned uX>
		struct allocator32x4_bitmap
			: allocator32x4_bitmap<Tmm, uX - 1>
		{
			using base_type = allocator32x4_bitmap<Tmm, uX - 1>;

			Tmm m_u4Bitmap;

			template <typename T>
			struct Ttraits;

			template <> struct Ttraits<__m128i>
			{
				static __m128i get_bitmap(unsigned uBitmap)
				{
					return _mm_set1_epi32(uBitmap);
				}
				static Tmm get_bitmap(allocator32x4_bitmap const & that)
				{
					return that.m_u4Bitmap;
				}
				static __m128i setzero()
				{
					return _mm_setzero_si128();
				}
				template <typename T>
				static __m128i load(T *pt)
				{
					return _mm_loadu_si128(reinterpret_cast<__m128i const*>(const_cast<std::remove_volatile_t<T>*>(pt)) + (uX - 1));
				}
				static __m128i xor (__m128i const & t1, __m128i const & t2)
				{
					return _mm_xor_si128(t1, t2);
				}
				static __m128i and (__m128i const & t1, __m128i const & t2)
				{
					return _mm_and_si128(t1, t2);
				}
				static __m128i avg16(__m128i const & t1, __m128i const & t2)
				{
					return _mm_avg_epu16(t1, t2);
				}
				static __m128i srli16(__m128i const & t1, int nCount)
				{
					return _mm_srli_epi16(t1, nCount);
				}
				static int cvt_si32(__m128i const & t1)
				{
					return _mm_cvtsi128_si32(t1);
				}
				static __m128i rotate32(__m128i const & t1)
				{
					return _mm_shuffle_epi32(t1, _MM_SHUFFLE(0, 3, 2, 1));
				}
			};
			template <> struct Ttraits<__m256i>
			{
				static __m256i get_bitmap(unsigned uBitmap)
				{
					return _mm256_set1_epi32(uBitmap);
				}
				static Tmm get_bitmap(allocator32x4_bitmap const & that)
				{
					return that.m_u4Bitmap;
				}
				static __m256i setzero()
				{
					return _mm256_setzero_si256();
				}
				template <typename T>
				static __m256i load(T *pt)
				{
					return _mm256_loadu_si256(reinterpret_cast<__m256i const*>(const_cast<std::remove_volatile_t<T>*>(pt)) + (uX - 1));
				}
				static __m256i xor (__m256i const & t1, __m256i const & t2)
				{
					return _mm256_xor_si256(t1, t2);
				}
				static __m256i and (__m256i const & t1, __m256i const & t2)
				{
					return _mm256_and_si256(t1, t2);
				}
				static __m256i avg16(__m256i const & t1, __m256i const & t2)
				{
					return _mm256_avg_epu16(t1, t2);
				}
				static __m256i srli16(__m256i const & t1, int nCount)
				{
					return _mm256_srli_epi16(t1, nCount);
				}
				static int cvt_si32(__m256i const & t1)
				{
					return _mm_cvtsi128_si32(_mm256_castsi256_si128(t1));
				}
				static __m256i rotate32(__m256i const & t1)
				{
					return _mm256_permutevar8x32_epi32(t1, _mm256_set_epi32(0, 7, 6, 5, 4, 3, 2, 1));
				}
			};
			using TT = Ttraits<Tmm>;

			allocator32x4_bitmap()
				: m_u4Bitmap(TT::setzero())
			{}
			template <typename T, size_t uSize>
			allocator32x4_bitmap(T(&auThat)[uSize])
				: base_type(auThat)
				, m_u4Bitmap(TT::load(auThat))
			{
				static_assert((sizeof(Tmm) * uX <= sizeof(T) * uSize), "Bounds exception");
			}
			template <typename T, size_t uSize>
			allocator32x4_bitmap(std::array<T, uSize> const & that)
				: base_type(that)
				, m_u4Bitmap(TT::load(that.data()))
			{
				static_assert((sizeof(Tmm) * uX <= sizeof(T) * uSize), "Bounds exception");
			}
			template <typename T1, typename T2>
			allocator32x4_bitmap(std::bit_xor<unsigned> const & tOp, T1 const & t1, T2 const & t2)
				: base_type(tOp, t1, t2)
				, m_u4Bitmap(TT::xor(TT::get_bitmap(t1), TT::get_bitmap(t2)))
			{}
			template <typename T1, typename T2>
			allocator32x4_bitmap(std::bit_and<unsigned> const & tOp, T1 const & t1, T2 const & t2)
				: base_type(tOp, t1, t2)
				, m_u4Bitmap(TT::and(TT::get_bitmap(t1), TT::get_bitmap(t2)))
			{}
			template <typename T1, typename T2>
			allocator32x4_bitmap(allocator32x_avg16<unsigned> const & tOp, T1 const & t1, T2 const & t2)
				: base_type(tOp, t1, t2)
				, m_u4Bitmap(TT::avg16(TT::get_bitmap(t1), TT::get_bitmap(t2)))
			{}
			template <template <typename T> class Top, typename T1, typename T2>
			allocator32x4_bitmap(allocator32x_op<Top, T1, T2> const & stOp)
				: allocator32x4_bitmap(Top<unsigned>(), std::get<0>(stOp.m_Tuple), std::get<1>(stOp.m_Tuple))
			{}

			template <typename Tfunc>
			static bool find_mask(Tmm const & u4Bitmap0, unsigned uLength, Tfunc const & tFunc)
			{
				Tmm u4BitmapH = u4Bitmap0;
				Tmm u4BitmapL = TT::srli16(u4Bitmap0, uLength - 1);
				for (unsigned u32x = 0; u32x < sizeof(Tmm) / sizeof(unsigned); u32x++)
				{
					for (unsigned uBitmapH = TT::cvt_si32(u4BitmapH), uBitmapL = TT::cvt_si32(u4BitmapL); uBitmapH; uBitmapH = _blsr_u32(uBitmapH), uBitmapL = _blsr_u32(uBitmapL))
					{
						unsigned uMask = (_blsi_u32(uBitmapH) - _blsi_u32(uBitmapL)) + _blsi_u32(uBitmapH);
						if (tFunc((uX - 1) * sizeof(Tmm) / sizeof(unsigned) + u32x, uMask))
						{
							return true;
						}
					}
					u4BitmapH = TT::rotate32(u4BitmapH);
					u4BitmapL = TT::rotate32(u4BitmapL);
				}
				return false;
			}
			template <typename Tfunc>
			bool find_mask(unsigned uLength, Tfunc const & tFunc) const
			{
				return base_type::find_mask(uLength, tFunc) || find_mask(m_u4Bitmap, uLength, tFunc);
			}
		};
		template <typename Tmm>
		struct allocator32x4_bitmap<Tmm, 0>
			: allocator32x_base
		{
			template <typename... Targs>
			allocator32x4_bitmap(Targs const &... tArgs)
			{}
			template <typename Tfunc>
			bool find_mask(unsigned uLength, Tfunc const & tFunc) const
			{
				return false;
			}
		};

		template <size_t u32x>
		struct allocator32x_bitmap
			: allocator32x_bitmap<u32x - 1>
		{
			using base_type = allocator32x_bitmap<u32x - 1>;

			unsigned m_uBitmap;

			static unsigned get_bitmap(unsigned uBitmap)
			{
				return uBitmap;
			}
			static unsigned get_bitmap(allocator32x_bitmap const & that)
			{
				return that.m_uBitmap;
			}

			allocator32x_bitmap()
				: m_uBitmap(0)
			{}
			allocator32x_bitmap(unsigned uBitmap)
				: m_uBitmap(uBitmap)
			{}
			template <typename T, size_t uSize>
			allocator32x_bitmap(T (&auThat)[uSize])
				: base_type(auThat)
				, m_uBitmap(auThat[u32x - 1])
			{
				static_assert(u32x <= uSize, "Bounds exception");
			}
			template <typename T, size_t uSize>
			allocator32x_bitmap(std::array<T, uSize> const & that)
				: base_type(that)
				, m_uBitmap(that[u32x - 1])
			{
				static_assert(u32x <= uSize, "Bounds exception");
			}

			template <typename Top, typename T1, typename T2>
			allocator32x_bitmap(Top const & tOp, T1 const & t1, T2 const & t2)
				: base_type(tOp, t1, t2)
				, m_uBitmap(tOp(get_bitmap(t1), get_bitmap(t2)))
			{}
			template <template <typename T> class Top, typename T1, typename T2>
			allocator32x_bitmap(allocator32x_op<Top, T1, T2> const & stOp)
				: allocator32x_bitmap(Top<unsigned>(), std::get<0>(stOp.m_Tuple), std::get<1>(stOp.m_Tuple))
			{}

			template <typename Tfunc>
			static bool find_mask(unsigned uBitmap, unsigned uLength, Tfunc const & tFunc)
			{
				for (unsigned uBitmapH = uBitmap, uBitmapL = uBitmap >> (uLength - 1); uBitmapH; uBitmapH = _blsr_u32(uBitmapH), uBitmapL = _blsr_u32(uBitmapL))
				{
					unsigned uMask = (_blsi_u32(uBitmapH) - _blsi_u32(uBitmapL)) + _blsi_u32(uBitmapH);
					if (tFunc((u32x - 1), uMask))
					{
						return true;
					}
				}
				return false;
			}
			template <typename Tfunc>
			bool find_mask(unsigned uLength, Tfunc const & tFunc) const
			{
				return base_type::find_mask(uLength, tFunc) || find_mask(m_uBitmap, uLength, tFunc);
			}
		};
		template <>
		struct allocator32x_bitmap<0>
			: allocator32x_base
		{
			template <typename... Targs>
			allocator32x_bitmap(Targs const &...)
			{}
			template <typename Tfunc>
			bool find_mask(unsigned uLength, Tfunc const & tFunc) const
			{
				return false;
			}
		};

		template <typename Texpr>
		using allocator32x_op_t = std::conditional_t<
			(std::is_base_of<allocator32x_base, Texpr>::value),
			Texpr const &,
			Texpr>;

		template <template <typename T> class Top, typename... Tops>
		struct allocator32x_op
		{
			std::tuple<Tops...> m_Tuple;

			allocator32x_op(Tops... tArgs)
				: m_Tuple(tArgs...)
			{}
		};

		static unsigned g_auAlignMask [] =
		{
			0x00000000U, // 0x00
			0xFFFFFFFFU, // 0x01
			0x55555555U, // 0x02
			0x11111111U, // 0x03
			0x11111111U, // 0x04
			0x01010101U, // 0x05
			0x01010101U, // 0x06
			0x01010101U, // 0x07
			0x01010101U, // 0x08
			0x00010001U, // 0x09
			0x00010001U, // 0x0A
			0x00010001U, // 0x0B
			0x00010001U, // 0x0C
			0x00010001U, // 0x0D
			0x00010001U, // 0x0E
			0x00010001U, // 0x0F
			0x00010001U, // 0x10
		};
		template <typename T1, typename T2, typename TenableIf = std::enable_if_t<(std::is_base_of<allocator32x_base, T1>::value || std::is_base_of<allocator32x_base, T2>::value)> >
		auto operator ^ (T1 const & t1, T2 const & t2)
		{
			return allocator32x_op<std::bit_xor, allocator32x_op_t<T1>, allocator32x_op_t<T2> >(t1, t2);
		}
		template <typename T1, typename T2, typename TenableIf = std::enable_if_t<(std::is_base_of<allocator32x_base, T1>::value || std::is_base_of<allocator32x_base, T2>::value)> >
		auto operator & (T1 const & t1, T2 const & t2)
		{
			return allocator32x_op<std::bit_and, allocator32x_op_t<T1>, allocator32x_op_t<T2> >(t1, t2);
		}
		template <typename T1, typename T2, typename TenableIf = std::enable_if_t<(std::is_base_of<allocator32x_base, T1>::value || std::is_base_of<allocator32x_base, T2>::value)> >
		auto avg16(T1 const & t1, T2 const & t2)
		{
			return allocator32x_op<allocator32x_avg16, allocator32x_op_t<T1>, allocator32x_op_t<T2> >(t1, t2);
		}
	}
	template <size_t u32x>
	struct allocator32x
	{
		using bitmap32x_type = allocator32x_bitmap<u32x>;
		using bitmap32x4_type = allocator32x4_bitmap<__m128i, ((u32x + 3) / 4)>;
		using bitmap32x8_type = allocator32x4_bitmap<__m256i, ((u32x + 7) / 8)>;

		using bitmap_type = std::conditional_t<(u32x == 1), bitmap32x_type, std::conditional_t<(u32x <= 4), bitmap32x4_type, bitmap32x8_type> >;

		std::atomic<unsigned> m_auBitmap[u32x];

		allocator32x()
		{
			std::fill(std::begin(m_auBitmap), std::end(m_auBitmap), 0xFFFFFFFF);
		}
		allocator32x(allocator32x && that)
		{
			std::transform(std::begin(that.m_auBitmap), std::end(that.m_auBitmap), m_auBitmap, [=](std::atomic<unsigned> & uBitmap)->unsigned
			{
				return uBitmap.exchange(0, std::memory_order_relaxed);
			});
		}

		constexpr unsigned end() const
		{
			return u32x * 32;
		}

		unsigned allocate(unsigned uLength)
		{
			if (uLength < std::size(g_auAlignMask))
			{
				unsigned const uAlignMask = g_auAlignMask[uLength];

				bitmap_type const OldBitmap0(m_auBitmap);
				// case A: avg(0011, 0101) = 0100
				// case B: avg(0111, 0101) = 0110
				bitmap_type const AddBitmap1 = avg16(OldBitmap0, uAlignMask);
				// case A: 0011 ^ 0101 = 0110
				// case B: 0111 ^ 0101 = 0010
				bitmap_type const XorBitmap1 = OldBitmap0 ^ uAlignMask;
				// case A: avg(0110, 0000) = 0011
				// case B: avg(0010, 0000) = 0001
				bitmap_type const AvgBitmap2 = avg16(XorBitmap1, 0);
				// case A: 1000 ^ 0110 = 1110
				// case B: 1100 ^ 0010 = 1110
				bitmap_type const XorBitmap3 = AddBitmap1 ^ AvgBitmap2;
				// case A: 1110 & 0101 = 0100
				// case B: 1110 & 0101 = 0100
				bitmap_type const AndBitmap4 = XorBitmap3 & (uAlignMask << (uLength - 1));

				unsigned uReturnIndex = end();
				{
					AndBitmap4.find_mask(uLength, [=, &uReturnIndex](unsigned uIndex32, unsigned uMask)->bool
					{
						unsigned uOldBitmap = m_auBitmap[uIndex32];
						unsigned uNewBitmap = uOldBitmap ^ uMask;
						if ((uOldBitmap & uMask) != uMask)
						{
#ifdef _DEBUG
							__debugbreak();
#endif
						}
						else if (std::atomic_compare_exchange_strong_explicit(&m_auBitmap[uIndex32], &uOldBitmap, uNewBitmap, std::memory_order_acquire, std::memory_order_relaxed))
						{
							uReturnIndex = uIndex32 * 32 + _tzcnt_u32(uMask);
							return true;
						}
						return false;
					});
				}
				return uReturnIndex;
			}
			return end();
		}
		void deallocate(unsigned uIndex, unsigned uLength)
		{
			unsigned const uIndex32 = uIndex / 32;
			unsigned const uIndex1 = uIndex % 32;
			unsigned const uPattern = _bzhi_u32(~0, uLength);

			unsigned const uMask = uPattern << uIndex1;

			std::atomic_fetch_or_explicit(&m_auBitmap[uIndex32], uMask, std::memory_order_release);
		}
	};
}