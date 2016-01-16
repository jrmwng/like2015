
#include <stdio.h>
#include <intrin.h>

void print_cpuid(int nFirst, int nLast)
{
	for (int i = nFirst, iend = nLast; i <= iend; i++)
	{
		__m128i xmmCPUID0 = _mm_setzero_si128();
		__m128i xmmCPUID1;

		for (int j = 0; j < 256; j++)
		{
			__cpuidex(xmmCPUID1.m128i_i32, i, j);

			if (_mm_movemask_epi8(_mm_cmpeq_epi8(xmmCPUID0, xmmCPUID1)) == 0xFFFF)
			{
				break;
			}
			xmmCPUID0 = xmmCPUID1;

			printf("[%08X][%02X]: %08X %08X %08X %08X\n", i, j, xmmCPUID1.m128i_i32[0], xmmCPUID1.m128i_i32[1], xmmCPUID1.m128i_i32[2], xmmCPUID1.m128i_i32[3]);
		}
	}
}

int main(void)
{
	__m128i xmmCPUID;
	{
		__cpuid(xmmCPUID.m128i_i32, 0);
		printf("%4.4s%4.4s%4.4s\n",
			reinterpret_cast<char const*>(xmmCPUID.m128i_i32 + 1),
			reinterpret_cast<char const*>(xmmCPUID.m128i_i32 + 3),
			reinterpret_cast<char const*>(xmmCPUID.m128i_i32 + 2));
	}
	print_cpuid(1, xmmCPUID.m128i_i32[0]);

	__cpuid(xmmCPUID.m128i_i32, 0x80000000);
	print_cpuid(0x80000001, xmmCPUID.m128i_i32[0]);
	return 0;
}