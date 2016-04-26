#pragma once

/* Author: jrmwng @ 2016 */

#include <intrin.h>
#include <type_traits>

namespace jrmwng
{
	template <size_t szByteCount>
	std::enable_if_t<(4096 < szByteCount), void> memset(void *p, unsigned char ub)
	{
		__stosb((unsigned char*) p, ub, szByteCount);
	}
	template <size_t szByteCount>
#ifdef __AVX2__
	std::enable_if_t<(0x7F < szByteCount && szByteCount <= 4096), void> memset(void *p, unsigned char ub)
#else
	std::enable_if_t<(0x3F < szByteCount && szByteCount <= 4096), void> memset(void *p, unsigned char ub)
#endif
	{
		memset<(szByteCount & 3)>(p, ub);
		__stosd(reinterpret_cast<unsigned long*>(p) + (szByteCount & 3), ((static_cast<unsigned long>(ub)) | (static_cast<unsigned long>(ub) << 8) | (static_cast<unsigned long>(ub) << 16) | (static_cast<unsigned long>(ub) << 24)), (szByteCount >> 2));
	}
	template <size_t szByteCount>
#ifdef __AVX2__
	std::enable_if_t<(szByteCount <= 0x7F), void> memset(void *p, unsigned char ub)
#else
	std::enable_if_t<(szByteCount <= 0x3F), void> memset(void *p, unsigned char ub)
#endif
	{
		memset<(szByteCount & 0x40)>(reinterpret_cast<unsigned char*>(p) + (szByteCount & 0x00), ub);
		memset<(szByteCount & 0x20)>(reinterpret_cast<unsigned char*>(p) + (szByteCount & 0x40), ub);
		memset<(szByteCount & 0x10)>(reinterpret_cast<unsigned char*>(p) + (szByteCount & 0x60), ub);
		memset<(szByteCount & 0x08)>(reinterpret_cast<unsigned char*>(p) + (szByteCount & 0x70), ub);
		memset<(szByteCount & 0x04)>(reinterpret_cast<unsigned char*>(p) + (szByteCount & 0x78), ub);
		memset<(szByteCount & 0x02)>(reinterpret_cast<unsigned char*>(p) + (szByteCount & 0x7C), ub);
		memset<(szByteCount & 0x01)>(reinterpret_cast<unsigned char*>(p) + (szByteCount & 0x7D), ub);
	}


	template <>
	void memset<0>(void *, unsigned char)
	{
		// NOP
	}
	template <>
	void memset<1>(void *p, unsigned char ub)
	{
		*reinterpret_cast<unsigned char*>(p) = ub;
	}
	template <>
	void memset<2>(void *p, unsigned char ub)
	{
		*reinterpret_cast<unsigned short*>(p) = (static_cast<unsigned short>(ub)) | (static_cast<unsigned short>(ub) << 8);
	}
	template <>
	void memset<4>(void *p, unsigned char ub)
	{
		*reinterpret_cast<unsigned long*>(p) = (static_cast<unsigned long>(ub)) | (static_cast<unsigned long>(ub) << 8) | (static_cast<unsigned long>(ub) << 16) | (static_cast<unsigned long>(ub) << 24);
	}
	template <>
	void memset<8>(void *p, unsigned char ub)
	{
#ifdef _M_X64
		*reinterpret_cast<unsigned long long*>(p) = (static_cast<unsigned long long>(ub)) | (static_cast<unsigned long long>(ub) << 8) | (static_cast<unsigned long long>(ub) << 16) | (static_cast<unsigned long long>(ub) << 24) | (static_cast<unsigned long long>(ub) << 32) | (static_cast<unsigned long long>(ub) << 40) | (static_cast<unsigned long long>(ub) << 48) | (static_cast<unsigned long long>(ub) << 56);
#else
		reinterpret_cast<unsigned long*>(p)[0] = (static_cast<unsigned long>(ub)) | (static_cast<unsigned long>(ub) << 8) | (static_cast<unsigned long>(ub) << 16) | (static_cast<unsigned long>(ub) << 24);
		reinterpret_cast<unsigned long*>(p)[1] = (static_cast<unsigned long>(ub)) | (static_cast<unsigned long>(ub) << 8) | (static_cast<unsigned long>(ub) << 16) | (static_cast<unsigned long>(ub) << 24);
#endif
	}
#ifdef __AVX2__
	template <>
	void memset<32>(void *p, unsigned char ub)
	{
		reinterpret_cast<__m256i*>(p)[0] = _mm256_set1_epi8(ub);
	}
	template <>
	void memset<64>(void *p, unsigned char ub)
	{
		reinterpret_cast<__m256i*>(p)[0] = _mm256_set1_epi8(ub);
		reinterpret_cast<__m256i*>(p)[1] = _mm256_set1_epi8(ub);
	}
#else
	template <>
	void memset<16>(void *p, unsigned char ub)
	{
		reinterpret_cast<__m128i*>(p)[0] = _mm_set1_epi8(ub);
	}
	template <>
	void memset<32>(void *p, unsigned char ub)
	{
		reinterpret_cast<__m128i*>(p)[0] = _mm_set1_epi8(ub);
		reinterpret_cast<__m128i*>(p)[1] = _mm_set1_epi8(ub);
	}
#endif

	template <typename T>
	void memset(T *pt, unsigned char ub)
	{
		memset<sizeof(T)>(pt, ub);
	}
	template <typename T, size_t szCount>
	void memset(T(&at)[szCount], unsigned char ub)
	{
		memset<sizeof(at)>(at, ub);
	}
}
