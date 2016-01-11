#pragma once

/* Author: jrmwng @ 2015 */

#include <intrin.h>

namespace like
{
	template <int nEAX, int nECX = 0>
	struct cpuid_base_t
	{
		// eax
		unsigned : 32;
		// ebx
		unsigned : 32;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 32;
	};
	
	template <> struct cpuid_base_t<0x01>
	{
		// eax
		unsigned : 32;
		// ebx
		unsigned uProcessorBrandIndex : 8; // [bits 7:0]
		unsigned uLineSizeCLFLUSH : 8; // [bits 15:8]
		unsigned uMaxNumOfID : 8; // [bits 23:16]
		unsigned uInitialAPIC_ID: 8; // [bits 31:24]
		// ecx
		unsigned uSSE3: 1;
		unsigned uPCLMULQDQ : 1; // bit 1
		unsigned : 1;
		unsigned uMONITOR : 1; // bit 3
		unsigned : 5;
		unsigned uSSSE3 : 1; // bit 9
		unsigned : 2;
		unsigned uFMA : 1; // bit 12
		unsigned uCMPXCHG16B : 1; // bit 13
		unsigned : 5;
		unsigned uSSE4_2 : 1; // bit 19
		unsigned uSSE4_1 : 1; // bit 20
		unsigned : 1;
		unsigned uMOVBE: 1; // bit 22
		unsigned uPOPCNT : 1; // bit 23
		unsigned : 1;
		unsigned uAES : 1; // bit 25
		unsigned uXSAVE: 1; // bit 26
		unsigned uOSXSAVE : 1; // bit 27
		unsigned uAVX : 1; // bit 28
		unsigned uF16C : 1; // bit 29
		unsigned uRDRAND : 1; // bit 30
		unsigned : 1;
		// edx
		unsigned : 19;
		unsigned uCLFSH : 1; // bit 19
		unsigned : 3;
		unsigned uMMX : 1; // bit 23
		unsigned uFXSR: 1; // bit 24
		unsigned uSSE : 1; // bit 25
		unsigned uSSE2 : 1; // bit 26
		unsigned : 1;
		unsigned uHTT : 1; // bit 28
		unsigned : 3;
	};
	template <>
	struct cpuid_base_t<0x05>
	{
		// eax
		unsigned uSmallestLineSizeMONITOR : 16; // [bits 15:0]
		unsigned : 16;
		// ebx
		unsigned uLargestLineSizeMONITOR : 16; // [bits 15:0]
		unsigned : 16;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x06>
	{
		// eax
		unsigned uDigitalThermalSensor : 1; // bit 0
		unsigned : 3;
		unsigned uPowerLimitNotification : 1; // bit 4
		unsigned uSoftwareControlledClockModulationExtension : 1; // bit 5
		unsigned : 7;
		unsigned uHDC : 1; // bit 13
		unsigned : 18;
		// ebx
		unsigned uNumOfThermalThreshold : 4;
		unsigned : 28;
		// ecx
		unsigned uPStateHardwareCoordination : 1; // bit 0
		unsigned : 2;
		unsigned uSETBH : 1; // bit 3
		unsigned : 28;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x07>
	{
		// eax
		unsigned : 32;
		// ebx
		unsigned : 2;
		unsigned uIntelSGX : 1; // bit 2
		unsigned uBMI1 : 1; // bit 3
		unsigned uHLE : 1; // bit 4
		unsigned uAVX2 : 1; // bit 5
		unsigned uFIP_FDP : 1; // bit 6: x87 FPU Instruction Pointer, Data (Operand) Pointer
		unsigned : 1;
		unsigned uBMI2 : 1; // bit 8
		unsigned uFastStringOperation : 1; // bit 9
		unsigned : 1;
		unsigned uRTM : 1; // bit 11
		unsigned : 1;
		unsigned uFCS_FDS : 1; // bit 13: suppress FCS, FDS
		unsigned uIntelMPX : 1; // bit 14
		unsigned : 3;
		unsigned uRDSEED : 1; // bit 18
		unsigned uADX : 1; // bit 19
		unsigned uSMAP : 1; // bit 20
		unsigned : 2;
		unsigned uCLFLUSHOPT : 1; // bit 23
		unsigned : 1;
		unsigned uIntelProcessorTrace : 1; // bit 25
		unsigned : 6;
		// ecx
		unsigned uPREFTEHCHWT1 : 1; // bit 0
		unsigned : 31;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x0C>
	{
		// eax
		unsigned uSGX1 : 1; // bit 0
		unsigned uSGX2 : 1; // bit 1
		unsigned : 30;
		// ebx
		unsigned uMISCSELECT : 32;
		// ecx
		unsigned : 32;
		// edx
		unsigned uMaxEnclaveSize_Not64 : 8; // [7:0]
		unsigned uMaxEnclaveSize_64 : 8; // [15:8]
		unsigned : 16;
	};
	template <> struct cpuid_base_t<0x0D>
	{
		// eax
		unsigned uStateX87 : 1; // bit 0
		unsigned uStateSSE : 1; // bit 1
		unsigned : 1;
		unsigned uBNDREGS : 1; // bit 3
		unsigned uBNDCSR : 1; // bit 4
		unsigned : 27;
		// ebx
		unsigned uSizeXCR0: 32;
		// ecx
		unsigned uSize : 32;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x0D, 1>
	{
		// eax
		unsigned uXSAVEOPT : 1; // bit 0
		unsigned uCompactionExtensions : 1; // bit 1
		unsigned uXGETBV : 1; // bit 2
		unsigned uXSAVES : 1; // bit 3
		unsigned : 28;
		// ebx
		unsigned uSize : 32;
		// ecx
		// edx
		unsigned long long uBitmap;
	};
	template <> struct cpuid_base_t<0x0D, 2> // AVX state
	{
		// eax
		unsigned uSize;
		// ebx
		unsigned uOffset;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x0D, 3> // BNDREG state
	{
		// eax
		unsigned uSize;
		// ebx
		unsigned uOffset;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x0D, 4> // BNDCSR state
	{
		// eax
		unsigned uSize;
		// ebx
		unsigned uOffset;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x0D, 5> // Opmask state
	{
		// eax
		unsigned uSize;
		// ebx
		unsigned uOffset;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x0D, 6> // ZMM_Hi256 state
	{
		// eax
		unsigned uSize;
		// ebx
		unsigned uOffset;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x0D, 7> // Hi16_ZMM state
	{
		// eax
		unsigned uSize;
		// ebx
		unsigned uOffset;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x0D, 8> // PT state
	{
		// eax
		unsigned uSize;
		// ebx
		unsigned uOffset;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x0D, 9> // PKRU state
	{
		// eax
		unsigned uSize;
		// ebx
		unsigned uOffset;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x80000000>
	{
		// eax
		unsigned uProcessorMaxExtendedCpuidFunctionIndex : 32;
		// ebx
		unsigned : 32;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x80000001>
	{
		// eax
		unsigned : 32;
		//ebx
		unsigned : 32;
		// ecx
		unsigned uLAHF_SAHF : 1; // bit 0
		unsigned : 4;
		unsigned uLZCNT : 1; // bit 5
		unsigned : 2;
		unsigned uPREFTEHCHW : 1; // bit 8
		unsigned : 23;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x80000002>
	{
		char ac[16];
	};
	template <> struct cpuid_base_t<0x80000003>
	{
		char ac[16];
	};
	template <> struct cpuid_base_t<0x80000004>
	{
		char ac[16];
	};

	template <int nEAX, int nECX = 0>
	struct cpuid_t
		: cpuid_base_t<nEAX, nECX>
	{
		cpuid_t()
		{
			__cpuidex(reinterpret_cast<int*>(this), nEAX, nECX);

			if (sizeof(cpuid_base_t<nEAX, nECX>) != 16)
				__debugbreak();
		}
	};

	struct cpuid_processor_brand_string_t
		: cpuid_t<0x80000002>
		, cpuid_t<0x80000003>
		, cpuid_t<0x80000004>
	{
		operator char const * (void) const
		{
			return reinterpret_cast<char const*>(this);
		}
	};
}