
static inline unsigned char _addcarryx_u32(unsigned char __cf, unsigned int __x, unsigned int __y, unsigned int *__p)
{
	unsigned long long __res = (unsigned long long) __x + __y + __cf;

	*__p = __res;
	return (unsigned char) (__res >> 32);
}
static inline unsigned int _mulx_u32(unsigned int __X, unsigned int __Y, unsigned int *__P)
{
	unsigned long long __res = (unsigned long long) __X * __Y;
	*__P = (unsigned int) (__res >> 32);
	return (unsigned int) __res;
}

#include "big_number.h"

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
