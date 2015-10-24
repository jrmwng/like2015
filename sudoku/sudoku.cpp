// sudoku.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

template <unsigned uSize>
struct sudoku_number
{
	long lNumberSet;

	sudoku_number()
		: lNumberSet((1 << uSize) - 1)
	{}

	void reset(void)
	{
		lNumberSet = (1 << uSize) - 1;
	}

	void set_index(int nIndex)
	{
		if (nIndex > 0)
		{
			lNumberSet = (1 << (nIndex - 1));
		}
	}
};

template <unsigned uSize>
std::ostream & operator<< (std::ostream & os, sudoku_number<uSize> const & stSudokuNumber)
{
	long lNumberSet = stSudokuNumber.lNumberSet;

	if (__popcnt(lNumberSet) == 1)
	{
		unsigned long ulIndex;
		{
			_BitScanForward(&ulIndex, lNumberSet);
		}
		os << std::ends << (ulIndex + 1) << std::ends;
	}
	else
	{
		os << ((lNumberSet >> 6) & 7) << ((lNumberSet >> 3) & 7) << ((lNumberSet) & 7);
	}
	return os;
}
template <unsigned uSize>
std::istream & operator>> (std::istream & is, sudoku_number<uSize> & stSudokuNumber)
{
	int nIndex;
	{
		is >> nIndex;
	}
	stSudokuNumber.set_index(nIndex);
	return is;
}

template <unsigned uSize>
struct sudoku_group
{
	unsigned uCount;
	sudoku_number<uSize> *apNumber[uSize];

	sudoku_group()
		: uCount(0)
	{
		std::fill(std::begin(apNumber), std::end(apNumber), nullptr);
	}
	void add_number(sudoku_number<uSize> *pNumber)
	{
		if (uCount < std::size(apNumber))
		{
			apNumber[uCount++] = pNumber;
		}
		else
			__debugbreak();
	}
	bool discover_sets(void)
	{
		bool bModified = false;

		long alNumberSet[uSize];
		{
			std::transform(std::cbegin(apNumber), std::cend(apNumber), alNumberSet, [&](sudoku_number<uSize> const *pNumber)->long
			{
				return pNumber->lNumberSet;
			});
		}

		for (long lBits0 = 1; lBits0 < (1 << uSize); lBits0++)
		{
			long lNumberSet0 = 0;
			{
				long lBits1 = lBits0;
				for (unsigned long ulIndex0; _BitScanForward(&ulIndex0, lBits1); _bittestandreset(&lBits1, ulIndex0))
				{
					lNumberSet0 |= alNumberSet[ulIndex0];
				}
			}
			if (__popcnt(lNumberSet0) == __popcnt(lBits0))
			{
				long lBits1 = (~lBits0) & ((1 << uSize) - 1);
				long lNumberSet1 = (~lNumberSet0) & ((1 << uSize) - 1);
				for (unsigned long ulIndex0; _BitScanForward(&ulIndex0, lBits1); _bittestandreset(&lBits1, ulIndex0))
				{
					if (alNumberSet[ulIndex0] & lNumberSet0)
					{
						alNumberSet[ulIndex0] &= lNumberSet1;
						bModified = true;
					}
				}
			}
		}
		if (bModified)
		{
			for (unsigned i = 0; i < uSize; i++)
			{
				if (apNumber[i]->lNumberSet != alNumberSet[i])
				{
					apNumber[i]->lNumberSet = alNumberSet[i];
				}
			}
		}
		return bModified;
	}
};

int main()
{
	sudoku_number<9> astNumber[81];
	sudoku_group<9> astGroup[27];
	{
		auto astGroupBox = astGroup + 0;
		auto astGroupRow = astGroup + 9;
		auto astGroupCol = astGroup + 18;
		for (int y0 = 0; y0 < 3; y0++)
		{
			for (int y1 = 0; y1 < 3; y1++)
			{
				for (int x0 = 0; x0 < 3; x0++)
				{
					for (int x1 = 0; x1 < 3; x1++)
					{
						auto & stNumber = astNumber[(x0 * 3 + x1) + (y0 * 3 + y1) * 9];

						astGroupBox[x0 + y0 * 3].add_number(&stNumber);
						astGroupRow[y1 + y0 * 3].add_number(&stNumber);
						astGroupCol[x1 + x0 * 3].add_number(&stNumber);
					}
				}
			}
		}
	}

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
						std::cout << astNumber[(x0 + x1) + (y0 + y1) * 9];
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
			for (unsigned x = 0; x < 9; x++, n /= 10)
			{
				astNumber[(8 - x) + y * 9].set_index(n % 10);
			}
		}
		if (c == 'd') // discover
		{
			bool abModified[27];
			do
			{
				std::transform(std::begin(astGroup), std::end(astGroup), abModified, std::mem_fun_ref(&sudoku_group<9>::discover_sets));
			} while (std::accumulate(std::cbegin(abModified), std::cend(abModified), false, std::logical_or<bool>()));
		}
		if (c == 'r') // reset
		{
			std::for_each(std::begin(astNumber), std::end(astNumber), std::mem_fun_ref(&sudoku_number<9>::reset));
		}
	}

    return 0;
}
