// sudoku.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "sudoku.h"

std::ostream & operator<< (std::ostream & os, like::sudoku_t<like::sudoku_9x9_traits>::number_t const & stSudokuNumber)
{
	long lCandidateSet = stSudokuNumber.lCandidateSet;

	unsigned long ulCandidateIndex0;
	unsigned long ulCandidateIndex1;
	{
		_BitScanForward(&ulCandidateIndex0, lCandidateSet);
		_BitScanReverse(&ulCandidateIndex1, lCandidateSet);
	}
	if (lCandidateSet && ulCandidateIndex0 == ulCandidateIndex1)
	{
		os << std::ends << (ulCandidateIndex0 + 1) << std::ends;
	}
	else
	{
		os << ((lCandidateSet >> 6) & 7) << ((lCandidateSet >> 3) & 7) << ((lCandidateSet) & 7);
	}
	return os;
}
std::ostream & operator<< (std::ostream & os, like::sudoku_t<like::sudoku_9x9_traits> const & stSudoku)
{
	for (int y0 = 0; y0 < 9; y0 += 3)
	{
		for (int y1 = 0; y1 < 3; y1++)
		{
			for (int x0 = 0; x0 < 9; x0 += 3)
			{
				for (int x1 = 0; x1 < 3; x1++)
				{
					std::cout << stSudoku.astNumber[(x0 + x1) + (y0 + y1) * 9];
				}
				std::cout << std::ends;
			}

			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	return os;
}

int main()
{
	std::shared_ptr<like::sudoku_t<like::sudoku_9x9_traits>> spSudoku(new like::sudoku_t<like::sudoku_9x9_traits>);
	long lGroupSet = 0;

	for (;;)
	{
		std::cout << *spSudoku;

		char c;
		{
			std::cin >> c;
			if (c == 'q')
				break;
		}
		if (c == 's') // set
		{
			int y;
			int n;
			{
				std::cin >> y >> n;
			}
			for (unsigned x = 0; x < 9; x++, n /= 10)
			{
				lGroupSet |= spSudoku->set(8 - x, y, n % 10);
			}
		}
		if (c == 'p') // peek
		{
			auto stSudoku = *spSudoku;
			unsigned long long uxl0 = __rdtsc();
			stSudoku.update(lGroupSet);
			unsigned long long uxl1 = __rdtsc();
			std::cout << stSudoku;
			std::cout << uxl1 - uxl0 << " cycles" << std::endl;
		}
		if (c == 'r') // reset
		{
			spSudoku.reset(new like::sudoku_t<like::sudoku_9x9_traits>);
			lGroupSet = 0;
		}
	}

    return 0;
}
