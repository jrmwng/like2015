#pragma once

/* Author: jrmwng @ 2015 */

#include <intrin.h>
#include <stdint.h>
#include <algorithm>
#include <numeric>
#include <functional>
#include "atomic.h"

namespace like
{
	template <uint8_t uCount16>
	struct __declspec(align(16)) shared_lock
	{
		__m128i axmm[uCount16];

		shared_lock(void)
		{
			std::fill(std::begin(axmm), std::end(axmm), _mm_setzero_si128());
		}

		void read_begin(unsigned uIndex) volatile
		{
			atomic_store<uint8_t>(&(axmm->m128i_u8[uIndex]), 0x80);
		}
		void read_end(unsigned uIndex) volatile
		{
			atomic_store<uint8_t>(&(axmm->m128i_u8[uIndex]), 0);
		}
		void read_sync(void)
		{
			int an[uCount16];
			{
				std::transform(std::cbegin(axmm), std::cend(axmm), an, [](__m128i const & xmm)->int
				{
					return _mm_movemask_epi8(xmm);
				});
			}
			while (std::accumulate(std::cbegin(an), std::cend(an), 0, std::bit_or<int>()))
			{
				_mm_pause();

				std::transform(std::cbegin(axmm), std::cend(axmm), an, an, [](__m128i const & xmm, int n)->int
				{
					return _mm_movemask_epi8(xmm) & n;
				});
			}
		}
	};
}
