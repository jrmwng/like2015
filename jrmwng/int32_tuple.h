#pragma once

/* Author: jrmwng @ 2016 */

#include <intrin.h>

namespace jrmwng
{
	template <unsigned uCount, typename TInt32 = void>
	struct int32_tuple;

	namespace internals
	{
		template <size_t uIndex, typename Tint32>
		struct int32_tuple_element;
		template <size_t uIndex1>
		struct int32_tuple_element<uIndex1, int>
		{
			int nValue;

			int32_tuple_element()
				: nValue(0)
			{}

			template <size_t uIndex>
			int get() const
			{
				static_assert(uIndex == uIndex1, "Programming error");
				return nValue;
			}
			template <size_t uIndex>
			void set(int nNewValue)
			{
				static_assert(uIndex == uIndex1, "Programming error");
				nValue = nNewValue;
			}
		};
		template <size_t uIndex4>
		struct int32_tuple_element<uIndex4, __m128i>
		{
			__m128i xmmValue;

			int32_tuple_element()
				: xmmValue(_mm_setzero_si128())
			{}

			template <size_t uIndex>
			int get() const
			{
				static_assert((uIndex >> 2) == uIndex4, "Programming error");
				return _mm_extract_epi32(xmmValue, uIndex & 3);
			}
			template <size_t uIndex>
			void set(int nNewValue)
			{
				static_assert((uIndex >> 2) == uIndex4, "Programming error");
				xmmValue = _mm_insert_epi32(xmmValue, nNewValue, uIndex & 3);
			}
		};
		template <size_t uIndex8>
		struct int32_tuple_element<uIndex8, __m256i>
		{
			__m256i ymmValue;

			int32_tuple_element()
				: ymmValue(_mm256_setzero_si128())
			{}

			template <size_t uIndex>
			int get() const
			{
				static_assert((uIndex >> 3) == uIndex8, "Programming error");
				return _mm256_extract_epi32(ymmValue, uIndex & 7);
			}
			template <size_t uIndex>
			void set(int nNewValue)
			{
				static_assert((uIndex >> 3) == uIndex8, "Programming error");
				ymmValue = _mm256_insert_epi32(ymmValue, nNewValue, uIndex & 7);
			}
		};
		template <typename Tint32, size_t uStart, size_t uEnd, size_t uSize>
		struct int32_tuple
			: int32_tuple<Tint32, uStart, (uStart + uEnd) / 2>
			, int32_tuple<Tint32, (uStart + uEnd) / 2, uEnd>
		{};
		template <typename Tint32, size_t uStart, size_t uEnd>
		struct int32_tuple<Tint32, uStart, uEnd, 1>
			: int32_tuple_element<uStart, Tint32>
		{};
		template <typename TInt32Tuple, size_T uIndex>
		struct int32_tuple_ref
		{
			TInt32Tuple & m_refTuple;

			int32_tuple_ref(TInt32Tuple & refTuple)
				: m_refTuple(refTuple)
			{}

			operator int() const
			{
				return m_refTuple.get<uIndex>();
			}
			int32_tuple_ref & operator = (int nNewValue)
			{
				m_refTuple.set<uIndex>(nNewValue);
				return *this;
			}
		};
	}

	template <unsigned uCount>
	struct int32_tuple<uCount, int>
		: internals::int32_tuple<int, 0, uCount>
	{};
	template <unsigned uCount>
	struct int32_tuple<uCount, __m128i>
		: internals::int32_tuple<__m128i, 0, ((uCount + 3U) / 4)>
	{};
	template <unsigned uCount>
	struct int32_tuple<uCount, __m256i>
		: internals::int32_tuple<__m256i, ((uCount + 7U) / 8)>
	{};

	template <size_t uIndex, size_t uCount, typename T>
	static decltype(auto) get_int32(int32_tuple<uCount, T> const & refTuple)
	{
		size_t constexpr uCardinality = sizeof(T) / sizeof(int);
		using element_type = internals::int32_tuple_element<(uIndex + uCardinality - 1U) / uCardinality, T>;
		return internals::int32_tuple_ref<element_type, uIndex>(refTuple);
	}
}
