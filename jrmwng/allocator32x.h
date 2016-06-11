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
	namespace internals
	{
		template <template <typename T> class Top, typename... Tops>
		struct allocator32x_op;

		struct allocator32x_base
		{};

		struct allocator32x4_bitmap
			: allocator32x_base
		{
			__m128i m_u4Bitmap;

			static __m128i get_bitmap(unsigned uBitmap)
			{
				return _mm_set1_epi32(uBitmap);
			}
			static __m128i get_bitmap(allocator32x4_bitmap const & that)
			{
				return that.m_u4Bitmap;
			}

			allocator32x4_bitmap()
				: m_u4Bitmap(_mm_setzero_si128())
			{}
			template <typename T, size_t uSize, typename TenableIf = std::enable_if_t<(sizeof(__m128i) <= sizeof(T) * uSize)> >
			allocator32x4_bitmap(T(&auThat)[uSize])
				: m_u4Bitmap(_mm_loadu_si128(reinterpret_cast<__m128i const*>(auThat)))
			{
				static_assert((sizeof(__m128i) <= sizeof(T) * uSize), "Bounds exception");
			}
			template <typename T, size_t uSize>
			allocator32x4_bitmap(std::array<T, uSize> const & that)
				: m_u4Bitmap(_mm_loadu_si128(reinterpret_cast<__m128i const*>(that.data())))
			{
				static_assert((sizeof(__m128i) <= sizeof(T) * uSize), "Bounds exception");
			}
#if 1
			template <typename T1, typename T2>
			allocator32x4_bitmap(std::plus<unsigned> const & tAdd, T1 const & t1, T2 const & t2)
				: m_u4Bitmap(_mm_add_epi32(get_bitmap(t1), get_bitmap(t2)))
			{}
#else
			template <typename T1, typename T2>
			allocator32x4_bitmap(std::plus<unsigned> const & tAdd, T1 const & t1, T2 const & t2)
			{
				__m128i u4BitmapA = get_bitmap(t1);
				__m128i u4BitmapB = get_bitmap(t2);

				unsigned long long uxl0, uxl1;
				unsigned char ubCarry0 = 0;
				unsigned char ubCarry1 = _addcarryx_u64(ubCarry0, _mm_extract_epi64(u4BitmapA, 0), _mm_extract_epi64(u4BitmapB, 0), &uxl0);
				unsigned char ubCarry2 = _addcarryx_u64(ubCarry1, _mm_extract_epi64(u4BitmapA, 1), _mm_extract_epi64(u4BitmapB, 1), &uxl1);

				m_u4Bitmap = _mm_set_epi64x(uxl1, uxl0);
			}
#endif
			template <typename T1, typename T2>
			allocator32x4_bitmap(std::bit_xor<unsigned> const & tAdd, T1 const & t1, T2 const & t2)
				: m_u4Bitmap(_mm_xor_si128(get_bitmap(t1), get_bitmap(t2)))
			{}
			template <typename T1, typename T2>
			allocator32x4_bitmap(std::bit_and<unsigned> const & tAdd, T1 const & t1, T2 const & t2)
				: m_u4Bitmap(_mm_and_si128(get_bitmap(t1), get_bitmap(t2)))
			{}
			template <template <typename T> class Top, typename T1, typename T2>
			allocator32x4_bitmap(allocator32x_op<Top, T1, T2> const & stOp)
				: allocator32x4_bitmap(Top<unsigned>(), std::get<0>(stOp.m_Tuple), std::get<1>(stOp.m_Tuple))
			{}

			template <typename Tfunc>
			bool find_bit(Tfunc const & tFunc) const
			{
				__m128i u4Bitmap = m_u4Bitmap;
				for (unsigned uIndex32 = 0; uIndex32 < 128; uIndex32 += 32)
				{
					for (unsigned uBitmap = _mm_cvtsi128_si32(u4Bitmap), uIndex1 = _tzcnt_u32(uBitmap); uIndex1 < 32; uIndex1 = _tzcnt_u32(uBitmap))
					{
						uBitmap = _blsr_u32(uBitmap);
						if (tFunc(uIndex32 + uIndex1))
						{
							return true;
						}
					}
					u4Bitmap = _mm_shuffle_epi32(u4Bitmap, _MM_SHUFFLE(0, 3, 2, 1));
				}
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
			static unsigned get_base(unsigned uBitmap)
			{
				return uBitmap;
			}
			static base_type get_base(allocator32x_bitmap const & that)
			{
				return that;
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
				: base_type(tOp, get_base(t1), get_base(t2))
				, m_uBitmap(tOp(get_bitmap(t1), get_bitmap(t2)))
			{}
			template <typename T1>
			unsigned char assign_plus(T1 const & t1)
			{
				unsigned char const ubCarry = base_type::assign_plus(t1);
				return _addcarryx_u32(ubCarry, m_uBitmap, get_bitmap(t1), &m_uBitmap);
			}
			template <typename T1, typename T2>
			allocator32x_bitmap(std::plus<unsigned> const & tAdd, T1 const & t1, T2 const & t2)
				: allocator32x_bitmap(t1)
			{
				assign_plus(t2);
			}
			template <template <typename T> class Top, typename T1, typename T2>
			allocator32x_bitmap(allocator32x_op<Top, T1, T2> const & stOp)
				: allocator32x_bitmap(Top<unsigned>(), std::get<0>(stOp.m_Tuple), std::get<1>(stOp.m_Tuple))
			{}

			template <typename Tfunc>
			bool find_bit(Tfunc const & tFunc) const
			{
				if (base_type::find_bit(tFunc))
				{
					return true;
				}
				for (unsigned uBitmap = m_uBitmap, uIndex = _tzcnt_u32(uBitmap); uIndex < 32; uIndex = _tzcnt_u32(uBitmap))
				{
					uBitmap = _blsr_u32(uBitmap);
					if (tFunc((u32x - 1) * 32 + uIndex))
					{
						return true;
					}
				}
				return false;
			}
		};
		template <>
		struct allocator32x_bitmap<0>
			: allocator32x_base
		{
			template <typename... Targs>
			allocator32x_bitmap(Targs const &...)
			{}
			template <typename T1>
			unsigned char assign_plus(T1 const &)
			{
				return 0;
			}
			template <typename Tfunc>
			bool find_bit(Tfunc const & tFunc) const
			{
				return false;
			}
		};
		template <>
		struct allocator32x_bitmap<4>
			: allocator32x4_bitmap
		{
			template <typename... Targs>
			allocator32x_bitmap(Targs const &... tArgs)
				: allocator32x4_bitmap(tArgs...)
			{}
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
		};
	}
	template <typename T1, typename T2, typename TenableIf = std::enable_if_t<(std::is_base_of<internals::allocator32x_base, T1>::value || std::is_base_of<internals::allocator32x_base, T2>::value)> >
	auto operator + (T1 const & t1, T2 const & t2)
	{
		return internals::allocator32x_op<std::plus, internals::allocator32x_op_t<T1>, internals::allocator32x_op_t<T2> >(t1, t2);
	}
	template <typename T1, typename T2, typename TenableIf = std::enable_if_t<(std::is_base_of<internals::allocator32x_base, T1>::value || std::is_base_of<internals::allocator32x_base, T2>::value)> >
	auto operator ^ (T1 const & t1, T2 const & t2)
	{
		return internals::allocator32x_op<std::bit_xor, internals::allocator32x_op_t<T1>, internals::allocator32x_op_t<T2> >(t1, t2);
	}
	template <typename T1, typename T2, typename TenableIf = std::enable_if_t<(std::is_base_of<internals::allocator32x_base, T1>::value || std::is_base_of<internals::allocator32x_base, T2>::value)> >
	auto operator & (T1 const & t1, T2 const & t2)
	{
		return internals::allocator32x_op<std::bit_and, internals::allocator32x_op_t<T1>, internals::allocator32x_op_t<T2> >(t1, t2);
	}
	template <size_t u32x>
	struct __declspec(dllexport) allocator32x
	{
		using bitmap_type = internals::allocator32x_bitmap<u32x>;

		std::array<std::atomic<unsigned>, u32x> m_auBitmap;

		allocator32x()
		{
			std::fill(m_auBitmap.begin(), m_auBitmap.end(), 0x7FFFFFFF);
		}

		unsigned allocate(unsigned uLength)
		{
			if (uLength < std::size(internals::g_auAlignMask))
			{
				unsigned const uAlignMask = internals::g_auAlignMask[uLength];

				if (0 < uLength)
				{
					bitmap_type const OldBitmap0(m_auBitmap);
					// case A: 0011 + 0101 = 1000
					// case B: 0111 + 0101 = 1100
					bitmap_type const AddBitmap1 = OldBitmap0 + uAlignMask;
					// case A: 0011 ^ 0101 = 0110
					// case B: 0111 ^ 0101 = 0010
					bitmap_type const XorBitmap1 = OldBitmap0 ^ uAlignMask;
					// case A: 1000 ^ 0110 = 1110
					// case B: 1100 ^ 0010 = 1110
					bitmap_type const XorBitmap2 = AddBitmap1 ^ XorBitmap1;
					// case A: 1110 & 0101 = 0100
					// case B: 1110 & 0101 = 0100
					bitmap_type const AndBitmap3 = XorBitmap2 & (uAlignMask << uLength);

					unsigned const uPattern = (1 << uLength) - 1;

					unsigned uReturnIndex = u32x * 32;
					{
						AndBitmap3.find_bit([=, &uReturnIndex](unsigned uIndex)->bool
						{
							unsigned const uIndex32 = uIndex / 32;
							unsigned const uIndex1 = uIndex % 32;
							unsigned uOldBitmap = m_auBitmap[uIndex32];
							//if (_bextr_u32(uOldBitmap, uIndex1 - uLength, uLength) == uPattern)
							{
								unsigned uMask = uPattern << (uIndex1 - uLength);

								unsigned uNewBitmap = uOldBitmap ^ uMask;
								if ((uOldBitmap & uMask) != uMask)
								{
#ifdef _DEBUG
									__debugbreak();
#endif
								}
								else if (std::atomic_compare_exchange_strong_explicit(&m_auBitmap[uIndex32], &uOldBitmap, uNewBitmap, std::memory_order_acquire, std::memory_order_relaxed))
								{
									uReturnIndex = uIndex32 * 32 + (uIndex1 - uLength);
									return true;
								}
							}
							return false;
						});
					}
					return uReturnIndex;
				}
			}
			return u32x * 32;
		}
		void deallocate(unsigned uIndex, unsigned uLength)
		{
			unsigned const uIndex32 = uIndex / 32;
			unsigned const uIndex1 = uIndex % 32;
			unsigned const uPattern = (1 << uLength);

			unsigned const uMask = uPattern << uIndex1;

			std::atomic_fetch_or_explicit(&m_auBitmap[uIndex32], uMask, std::memory_order_release);
		}
	};
}