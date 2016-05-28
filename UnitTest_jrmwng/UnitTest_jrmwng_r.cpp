#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\jrmwng\r.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_jrmwng
{		
	TEST_CLASS(UnitTest_jrmwng_r)
	{
	public:
		TEST_METHOD(TestMethod_jrmwng_r)
		{
			short w0 = 0;
			short w1 = 1;

			jrmwng::Rvar<short> wA = jrmwng::r(w0) + jrmwng::r(w1);
			jrmwng::Rvar<short> wB = jrmwng::r(w0) - jrmwng::r(w1);
			jrmwng::Rvar<short> wC = jrmwng::r(w0) * jrmwng::r(w1);
			jrmwng::Rvar<short> wD = jrmwng::r(w0) / jrmwng::r(w1);
			jrmwng::Rvar<short> wE = jrmwng::r(w0) % jrmwng::r(w1);
			jrmwng::Rvar<short> wF = jrmwng::r(w0) & jrmwng::r(w1);
			jrmwng::Rvar<short> wG = jrmwng::r(w0) | jrmwng::r(w1);
			jrmwng::Rvar<short> wH = jrmwng::r(w0) ^ jrmwng::r(w1);
			jrmwng::Rvar<short> wI = jrmwng::r(w0) << jrmwng::r(w1);
			jrmwng::Rvar<short> wJ = jrmwng::r(w0) >> jrmwng::r(w1);
			jrmwng::Rvar<short> wK = ~jrmwng::r(w0);
			jrmwng::Rvar<short> wL = -jrmwng::r(w0);
			if (wA < 0) __nop();
			if (wA <= 0) __nop();
			if (wA > 0) __nop();
			if (wA >= 0) __nop();
			if (wA == 0) __nop();
			if (wA != 0) __nop();
			if ((-wA + wB - wC * wD / ~wE % ~wF & wG | wH ^ wI << wJ >> wK) == 0) __nop();
			jrmwng::r(w0) += 1;
			jrmwng::r(w0) -= 1;
			jrmwng::r(w0) *= 1;
			jrmwng::r(w0) /= 1;
			jrmwng::r(w0) %= 1;
			jrmwng::r(w0) &= 1;
			jrmwng::r(w0) |= 1;
			jrmwng::r(w0) ^= 1;
			jrmwng::r(w0) <<= 1;
			jrmwng::r(w0) >>= 1;
			w0 += jrmwng::r(1);
			w0 -= jrmwng::r(1);
			w0 *= jrmwng::r(1);
			w0 /= jrmwng::r(1);
			w0 %= jrmwng::r(1);
			w0 &= jrmwng::r(1);
			w0 |= jrmwng::r(1);
			w0 ^= jrmwng::r(1);
			w0 <<= jrmwng::r(1);
			w0 >>= jrmwng::r(1);
		}
	};
}