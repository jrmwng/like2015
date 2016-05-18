#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DynamicLibrary1_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DynamicLibrary1_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DynamicLibrary1_EXPORTS
#define DynamicLibrary1_API __declspec(dllexport)
#else
#define DynamicLibrary1_API __declspec(dllimport)
#endif

// This is an example of a class exported from the DynamicLibrary1.dll
class DynamicLibrary1_API CDynamicLibrary1
{
public:
    CDynamicLibrary1();
    // TODO: add your methods here.
};

// This is an example of an exported variable
extern DynamicLibrary1_API int nDynamicLibrary1;

// This is an example of an exported function.
DynamicLibrary1_API int fnDynamicLibrary1(void);
