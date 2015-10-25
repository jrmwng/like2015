#pragma once

/* Author: jrmwng @ 2015 */

#include <intrin.h>

namespace like
{
	struct sudoku_9x9_traits
	{
		enum
		{
			SUDOKU_SIZE = 3,
			SUDOKU_CANDIDATE_COUNT = 9,
			SUDOKU_NUMBER_COUNT = 81,
			SUDOKU_GROUP_COUNT = 27,
		};
	};

	template <typename TT>
	struct sudoku_t
	{
		struct number_t
		{
			long lCandidateSet;
			long lGroupSet;
		} astNumber[TT::SUDOKU_NUMBER_COUNT];

		struct group_t
		{
			long alNumberIndex[TT::SUDOKU_CANDIDATE_COUNT];
		} astGroup[TT::SUDOKU_GROUP_COUNT];

		sudoku_t(void)
		{
			for (unsigned y0 = 0; y0 < TT::SUDOKU_SIZE; y0++)
			{
				for (unsigned y1 = 0; y1 < TT::SUDOKU_SIZE; y1++)
				{
					for (unsigned x0 = 0; x0 < TT::SUDOKU_SIZE; x0++)
					{
						for (unsigned x1 = 0; x1 < TT::SUDOKU_SIZE; x1++)
						{
							auto & stNumber = astNumber[(x1 + x0 * TT::SUDOKU_SIZE) + (y1 + y0 * TT::SUDOKU_SIZE) * TT::SUDOKU_CANDIDATE_COUNT];
							auto & stGroupBox = astGroup[TT::SUDOKU_CANDIDATE_COUNT * 0 + x0 + y0 * TT::SUDOKU_SIZE];
							auto & stGroupCol = astGroup[TT::SUDOKU_CANDIDATE_COUNT * 1 + x1 + x0 * TT::SUDOKU_SIZE];
							auto & stGroupRow = astGroup[TT::SUDOKU_CANDIDATE_COUNT * 2 + y1 + y0 * TT::SUDOKU_SIZE];
							{
								stNumber.lCandidateSet = (1 << TT::SUDOKU_CANDIDATE_COUNT) - 1;
								stNumber.lGroupSet = 0;
								stNumber.lGroupSet |= (1 << (&stGroupBox - astGroup));
								stNumber.lGroupSet |= (1 << (&stGroupCol - astGroup));
								stNumber.lGroupSet |= (1 << (&stGroupRow - astGroup));
								stGroupBox.alNumberIndex[x1 + y1 * 3] = &stNumber - astNumber;
								stGroupCol.alNumberIndex[y1 + y0 * 3] = &stNumber - astNumber;
								stGroupRow.alNumberIndex[x1 + x0 * 3] = &stNumber - astNumber;
							}
						}
					}
				}
			}
		}
		void update(long lGroupSet0)
		{
			// loop for dirty group-set
			for (long lGroupSet1 = 0; lGroupSet0; lGroupSet0 = lGroupSet1, lGroupSet1 = 0)
			{
				// for each dirty group
				for (unsigned long ulGroupIndex0; _BitScanForward(&ulGroupIndex0, lGroupSet0); _bittestandreset(&lGroupSet0, ulGroupIndex0))
				{
					auto const & stGroup = astGroup[ulGroupIndex0];

					for (long lNumberSet0 = 1; lNumberSet0 < (1 << TT::SUDOKU_CANDIDATE_COUNT) - 1; lNumberSet0++)
					{
						long lCandidateSet0 = 0;
						{
							long lNumberSet1 = lNumberSet0;

							for (unsigned long ulNumberIndex1; _BitScanForward(&ulNumberIndex1, lNumberSet1); _bittestandreset(&lNumberSet1, ulNumberIndex1))
							{
								auto const & stNumber = astNumber[stGroup.alNumberIndex[ulNumberIndex1]];

								lCandidateSet0 |= stNumber.lCandidateSet;
							}
						}
						if (__popcnt(lCandidateSet0) == __popcnt(lNumberSet0)) // the only condition of inference
						{
							long lNumberSet2 = (~lNumberSet0) & ((1 << TT::SUDOKU_CANDIDATE_COUNT) - 1);
							for (unsigned long ulNumberIndex2; _BitScanForward(&ulNumberIndex2, lNumberSet2); _bittestandreset(&lNumberSet2, ulNumberIndex2))
							{
								auto & stNumber = astNumber[stGroup.alNumberIndex[ulNumberIndex2]];

								if (stNumber.lCandidateSet & lCandidateSet0)
								{
									lGroupSet1 |= stNumber.lGroupSet;
									stNumber.lCandidateSet &= ~lCandidateSet0;
								}
							}
						}
					}
				}
			}
		}
		long set(unsigned uX, unsigned uY, unsigned uZ)
		{
			auto & stNumber = astNumber[uX + uY * TT::SUDOKU_CANDIDATE_COUNT];

			if (uZ-- > 0)
			{
				if (stNumber.lCandidateSet != (1 << uZ))
				{
					stNumber.lCandidateSet = (1 << uZ);
					return stNumber.lGroupSet;
				}
			}
			return 0;
		}
	};
}
