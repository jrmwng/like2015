// DynamicLibrary1.cpp : Defines the exported functions for the DLL application.
//

#include "DynamicLibrary1.h"

// This is an example of an exported variable
DynamicLibrary1_API int nDynamicLibrary1=0;

// This is an example of an exported function.
DynamicLibrary1_API int fnDynamicLibrary1(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see DynamicLibrary1.h for the class definition
CDynamicLibrary1::CDynamicLibrary1()
{
    return;
}
