#include "stdafx.h"
#include "big_number_unsigned.h"

template <unsigned uA, unsigned uB, unsigned uC>
void BN_AmulB_minusA_minusB_minus1(jrmwng::big_uint32<uA> const *pbnA, jrmwng::big_uint32<uB> const *pbnB, jrmwng::big_uint32<uC> *pbnC)
{
	jrmwng::big_uint32<uA> bnA_1 = 1U;
	jrmwng::big_uint32<uB> bnB_1 = 1U;

	*pbnC = (*pbnA - bnA_1) * (*pbnB - bnB_1);
}

template __declspec(dllexport)
void BN_AmulB_minusA_minusB_minus1<32, 32, 64>(jrmwng::big_uint32<32> const *, jrmwng::big_uint32<32> const *, jrmwng::big_uint32<64> *);

template __declspec(dllexport)
void BN_AmulB_minusA_minusB_minus1<64, 64, 128>(jrmwng::big_uint32<64> const *, jrmwng::big_uint32<64> const *, jrmwng::big_uint32<128> *);

template __declspec(dllexport)
void BN_AmulB_minusA_minusB_minus1<128, 128, 256>(jrmwng::big_uint32<128> const *, jrmwng::big_uint32<128> const *, jrmwng::big_uint32<256> *);
