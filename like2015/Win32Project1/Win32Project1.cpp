// Win32Project1.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


#include "..\jrmwng\allocator32x.h"

template struct jrmwng::allocator32x<8>;
template struct jrmwng::allocator32x<4>;
template struct jrmwng::allocator32x<1>;
