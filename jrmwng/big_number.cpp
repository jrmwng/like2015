#include "big_number.h"

void BN64_AmulB_minusA_minusB_plus1(jrmwng::big_uint32<32> const *pbn32A, jrmwng::big_uint32<32> const *pbn32B, jrmwng::big_uint32<64> *pbn64C)
{
	jrmwng::big_uint32<32> bn32_1;
	auto const bn32_A = *pbn32A;
	auto const bn32_B = *pbn32B;

	auto const bn64_C = (bn32_A - bn32_1) * (bn32_B - bn32_1);

	*pbn64C = bn64_C;
}
