#pragma once

/* Author: jrmwng @ 2015 */

#include <intrin.h>
#include <numeric>
#include <utility>

namespace like
{
	template<class... TArgs>
	struct journal_tuple;

	template<class TArg>
	struct journal_tuple
	{
		TArg const m_Arg;

		journal_tuple(TArg && _Arg)
			: m_Arg(std::forward<TAr>(_Arg))
		{}
	};
	template<class TThis, class... TRest>
	struct journal_tuple<TThis, TRest>
		: std::pair<TThis, journal_tuple<TRest>>
	{
		template<class TThis2, class... TRest2>
		journal_tuple(TThis2 && _This, TRest2 && _Rest)
			: std::pair<TThis, journal_tuple<TRest>>(std::forward<TThis2>(_This), std::forward<TRest2>(_Rest)...)
		{}
	};

	class journal
	{
		unsigned char * const m_pBufferBegin;
		unsigned char * const m_pBufferEnd;
		unsigned char *       m_pBuffer;
	public:
		journal(unsigned char *pBufferBegin, unsigned char *pBufferEnd)
			: m_pBufferBegin(pBufferBegin)
			, m_pBufferEnd(pBufferEnd)
			, m_pBuffer(pBufferBegin)
		{}
		unsigned char * record_begin(size_t uByteCount)
		{
			unsigned char *pBuffer = m_pBuffer;
			{
				if (pBuffer + uByteCount >= m_pBufferEnd)
					return nullptr;
			}
			return pBuffer;
		}
		ptrdiff_t record_end(unsigned char *pRecord, size_t uByteCount)
		{
			m_pBuffer = pRecord + uByteCount;
			return pRecord - m_pBufferBegin;
		}
		template <unsigned uSize>
		ptrdiff_t record_batch(unsigned char const *(const & apData)[uSize], size_t(const & auSize)[uSize])
		{
			size_t const uTotalSize = sizeof(unsigned) + std::accumulate(std::cbegin(auSize), std::cend(auSize), 0U);

			unsigned char * const pRecord = record_begin(uTotalSize);
			{
				if (pRecord == nullptr)
					return -1;
			}

			unsigned char *pBuffer = pRecord;
			{
				for (unsigned i = 0; i < uSize; i++)
				{
					std::copy(apData[i], apData[i] + auSize[i], pBuffer);
					pBuffer += auSize[i];
				}
			}

			return record_end(pRecord, uTotalSize);
		}
		template<class... TArgs>
		ptrdiff_t record(TArgs &&... _Args)
		{
			journal_tuple<TArgs> const stData(std::forward<TArgs>(_Args)...);
			unsigned char const * const apData [] = { (unsigned char const*) &stData };
			size_t const auSize [] = { sizeof(stData) };
			return record_batch(apData, auSize);
		}
	};
}
