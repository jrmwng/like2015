#include "stdafx.h"
#include "allocator32x.h"

template <unsigned u32x>
void construct(jrmwng::allocator32x<u32x> *p)
{
	new (p) jrmwng::allocator32x<u32x>();
}
template __declspec(dllexport) void construct<1>(jrmwng::allocator32x<1> *p);
template __declspec(dllexport) void construct<4>(jrmwng::allocator32x<4> *p);
template __declspec(dllexport) void construct<8>(jrmwng::allocator32x<8> *p);
template __declspec(dllexport) void construct<16>(jrmwng::allocator32x<16> *p);

template <unsigned u32x>
void destruct(jrmwng::allocator32x<u32x> *p)
{
	p->~allocator32x();
}
template __declspec(dllexport) void destruct<1>(jrmwng::allocator32x<1> *p);
template __declspec(dllexport) void destruct<4>(jrmwng::allocator32x<4> *p);
template __declspec(dllexport) void destruct<8>(jrmwng::allocator32x<8> *p);
template __declspec(dllexport) void destruct<16>(jrmwng::allocator32x<16> *p);

template <unsigned u32x>
unsigned allocate(jrmwng::allocator32x<u32x> *p, unsigned uLength)
{
	return p->allocate(uLength);
}
template __declspec(dllexport) unsigned allocate<1>(jrmwng::allocator32x<1> *p, unsigned uLength);
template __declspec(dllexport) unsigned allocate<4>(jrmwng::allocator32x<4> *p, unsigned uLength);
template __declspec(dllexport) unsigned allocate<8>(jrmwng::allocator32x<8> *p, unsigned uLength);
template __declspec(dllexport) unsigned allocate<16>(jrmwng::allocator32x<16> *p, unsigned uLength);

template <unsigned u32x>
void deallocate(jrmwng::allocator32x<u32x> *p, unsigned uIndex, unsigned uLength)
{
	return p->deallocate(uIndex, uLength);
}
template __declspec(dllexport) void deallocate<1>(jrmwng::allocator32x<1> *p, unsigned uIndex, unsigned uLength);
template __declspec(dllexport) void deallocate<4>(jrmwng::allocator32x<4> *p, unsigned uIndex, unsigned uLength);
template __declspec(dllexport) void deallocate<8>(jrmwng::allocator32x<8> *p, unsigned uIndex, unsigned uLength);
template __declspec(dllexport) void deallocate<16>(jrmwng::allocator32x<16> *p, unsigned uIndex, unsigned uLength);
