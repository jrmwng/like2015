// sudoku.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "sudoku.h"

std::ostream & operator<< (std::ostream & os, like::sudoku_t<like::sudoku_9x9_traits>::number_t const & stSudokuNumber)
{
	long lCandidateSet = stSudokuNumber.lCandidateSet;

	if (__popcnt(lCandidateSet) == 1)
	{
		unsigned long ulCandidateIndex;
		{
			_BitScanForward(&ulCandidateIndex, lCandidateSet);
		}
		os << std::ends << (ulCandidateIndex + 1) << std::ends;
	}
	else
	{
		os << ((lCandidateSet >> 6) & 7) << ((lCandidateSet >> 3) & 7) << ((lCandidateSet) & 7);
	}
	return os;
}

int main()
{
	std::shared_ptr<like::sudoku_t<like::sudoku_9x9_traits>> spSudoku(new like::sudoku_t<like::sudoku_9x9_traits>);

	for (;;)
	{
		for (int y0 = 0; y0 < 9; y0 += 3)
		{
			for (int y1 = 0; y1 < 3; y1++)
			{
				for (int x0 = 0; x0 < 9; x0 += 3)
				{
					for (int x1 = 0; x1 < 3; x1++)
					{
						std::cout << spSudoku->astNumber[(x0 + x1) + (y0 + y1) * 9];
					}
					std::cout << std::ends;
				}

				std::cout << std::endl;
			}
			std::cout << std::endl;
		}

		char c;
		{
			std::cin >> c;
			if (c == 'q')
				break;
		}
		if (c == 'l') // line
		{
			int y;
			int n;
			{
				std::cin >> y >> n;
			}
			long lGroupSet = 0;
			{
				for (unsigned x = 0; x < 9; x++, n /= 10)
				{
					lGroupSet |= spSudoku->set(8 - x, y, n % 10);
				}
			}
			spSudoku->update(lGroupSet);
		}
		if (c == 'r') // reset
		{
			spSudoku.reset(new like::sudoku_t<like::sudoku_9x9_traits>);
		}
	}

    return 0;
}
