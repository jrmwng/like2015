#pragma once

/* Author: jrmwng @ 2016 */

#include <intrin.h>

namespace jrmwng
{
	template <unsigned uCount, typename TInt32 = void>
	struct int32_tuple;

	template <>
	struct int32_tuple<0U, int>
	{};
	template <unsigned uCount>
	struct int32_tuple<uCount, int>
		: int32_tuple<uCount - 1U, int>
	{
		int nValue;

		int32_tuple()
			: nValue(0)
		{}

		template <unsigned uIndex>
		int get() const
		{
			static_assert(uIndex < uCount, "Index out of bountds");
			return static_cast<int32_tuple<uIndex + 1U, int> const *>(this)->nValue;
		}
		template <unsigned uIndex>
		void set(int nNewValue)
		{
			static_assert(uIndex < uCount, "Index out of bountds");
			static_cast<int32_tuple<uIndex + 1U, int> *>(this)->nValue = nNewValue;
		}
	};
	template <>
	struct int32_tuple<0U, __m128i>
	{};
	template <unsigned uCount>
	struct int32_tuple<uCount, __m128i>
		: int32_tuple<((uCount + 3U) & ~3U) - 4U, __m128i>
	{
		__m128i xmmValue;

		int32_tuple()
			: xmmValue(_mm_setzero_si128())
		{}

		template <unsigned uIndex>
		int get() const
		{
			static_assert(uIndex < uCount, "Index out of bountds");
			return _mm_extract_epi32(static_cast<int32_tuple<((uIndex + 1U) >> 2), __m128i> const *>(this)->xmmValue, static_cast<int>(uIndex & 3U));
		}
		template <unsigned uIndex>
		void set(int nNewValue) const
		{
			static_assert(uIndex < uCount, "Index out of bountds");
			_mm_insert_epi32(static_cast<int32_tuple<((uIndex + 1U) >> 2), __m128i> *>(this)->xmmValue, nNewValue, static_cast<int>(uIndex & 3U));
		}
	};
	template <>
	struct int32_tuple<0U, __m256i>
	{};
	template <unsigned uCount>
	struct int32_tuple<uCount, __m256i>
		: int32_tuple<((uCount + 7U) & ~7U) - 8U, __m256i>
	{
		__m256i ymmValue;

		template <unsigned uIndex>
		int get() const
		{
			static_assert(uIndex < uCount, "Index out of bountds");
			return _mm256_extract_epi32(static_cast<int32_tuple<((uIndex + 1U) >> 3)> const *>(this)->ymmValue, static_cast<int>(uIndex & 7U));
		}
		template <unsigned uIndex>
		void set(int nNewValue) const
		{
			static_assert(uIndex < uCount, "Index out of bountds");
			_mm256_insert_epi32(static_cast<int32_tuple<((uIndex + 1U) >> 3)> const *>(this)->ymmValue, nNewValue, static_cast<int>(uIndex & 7U));
		}
	};
	template <typename TInt32Tuple, unsigned uIndex>
	struct int32_ref
	{
		TInt32Tuple & m_refTuple;

		int32_ref(TInt32Tuple & refTuple)
			: m_refTuple(refTuple)
		{}

		operator int() const
		{
			return m_refTuple.get<uIndex>();
		}
		int32_ref & operator = (int nNewValue)
		{
			m_refTuple.set<uIndex>(nNewValue);
			return *this;
		}
	};

	template <unsigned uIndex, unsigned uCount, typename T>
	static decltype(auto) get_int32(int32_tuple<uCount, T> const & refTuple)
	{
		return int32_ref<decltype(refTuple), uIndex>(refTuple);
	}
}
