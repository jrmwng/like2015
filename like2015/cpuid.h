#pragma once

/* Author: jrmwng @ 2015-2016 */

#include <intrin.h>
#include <type_traits>
#include <iostream>
#include <iomanip>

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
	template <int nEAX, int nECX>
	std::ostream & operator << (std::ostream & os, cpuid_base_t<nEAX, nECX> const & cpuid)
	{
		return os;
	}

	template <> struct cpuid_base_t<0x00>
	{
		// eax
		unsigned uMaximumLeaf : 32;
		// ebx
		unsigned uEBX : 32;
		// ecx
		unsigned uECX : 32;
		// edx
		unsigned uEDX : 32;

		__m128i vendor_identification_string() const
		{
			return _mm_set_epi32(0, uECX, uEDX, uEBX);
		}
	};
	template <>
	std::ostream & operator << (std::ostream & os, cpuid_base_t<0x00> const & cpuid)
	{
		return os << cpuid.vendor_identification_string().m128i_i8;
	}

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
		unsigned uSSE3: 1; // bit 0
		unsigned uPCLMULQDQ : 1; // bit 1
		unsigned uDTES64 : 1; // bit 2
		unsigned uMONITOR : 1; // bit 3
		unsigned uDS_CPL : 1; // bit 4
		unsigned uVMX : 1; // bit 5
		unsigned uSMX : 1; // bit 6: Safer Mode Extensions
		unsigned uEIST : 1; // bit 7: Enhanced Intel SpeedStep technology
		unsigned uTM2 : 1; // bit 8: Thermal Monitor 2
		unsigned uSSSE3 : 1; // bit 9
		unsigned uCNXT_ID : 1; // bit 10: L1 Context ID
		unsigned uSDBG : 1; // bit 11
		unsigned uFMA : 1; // bit 12
		unsigned uCMPXCHG16B : 1; // bit 13
		unsigned u_xTPR_UpdateControl : 1; // bit 14
		unsigned uPDCM : 1; // bit 15: Perfmon and Debug Capability
		unsigned : 1; // bit 16
		unsigned uPCID : 1; // bit 17: Process-context identifiers
		unsigned uDCA : 1; // bit 18
		unsigned uSSE4_1 : 1; // bit 19
		unsigned uSSE4_2 : 1; // bit 20
		unsigned u_x2APIC : 1; // bit 21
		unsigned uMOVBE: 1; // bit 22
		unsigned uPOPCNT : 1; // bit 23
		unsigned uTSC_Deadline : 1; // bit 24
		unsigned uAES : 1; // bit 25
		unsigned uXSAVE: 1; // bit 26
		unsigned uOSXSAVE : 1; // bit 27
		unsigned uAVX : 1; // bit 28
		unsigned uF16C : 1; // bit 29
		unsigned uRDRAND : 1; // bit 30
		unsigned : 1;
		// edx
		unsigned uFPU : 1; // bit 0
		unsigned uVME : 1; // bit 1: Virtual 8086 Mode Enhancements
		unsigned uDE : 1; // bit 2: Debugging Extensions
		unsigned uPSE : 1; // bit 3: Page Size Extension
		unsigned uTSC : 1; // bit 4
		unsigned uMSR : 1; // bit 5
		unsigned uPAE : 1; // bit 6
		unsigned uMCE : 1; // bit 7: Machine Check Exception
		unsigned uCX8 : 1; // bit 8: CMPXCHG8B Instruction
		unsigned uAPIC : 1; // bit 9
		unsigned : 1; // bit 10
		unsigned uSEP : 1; // bit 11: SYSENTER and SYSEXIT Instructions
		unsigned uMTRR : 1; // bit 12
		unsigned uPGE : 1; // bit 13: Page Global Bit
		unsigned uMCA : 1; // bit 14 Machine Check Architecture
		unsigned uCMOV : 1; // bit 15
		unsigned uPAT : 1; // bit 16: Page Attribute Table
		unsigned uPSE36 : 1; // bit 17: 36-Bit Page Size Extension
		unsigned uPSN : 1; // bit 18: Processor Serial Number
		unsigned uCLFSH : 1; // bit 19
		unsigned : 1; // bit 20
		unsigned uDS : 1; // bit 21: Debug Store
		unsigned uACPI : 1; // bit 22: Thermal Monitor and Software Controlled Clock Facilities
		unsigned uMMX : 1; // bit 23
		unsigned uFXSR: 1; // bit 24
		unsigned uSSE : 1; // bit 25
		unsigned uSSE2 : 1; // bit 26
		unsigned uSS : 1; // bit 27: Self Snoop
		unsigned uHTT : 1; // bit 28: Max APIC IDs reserved field is Valid
		unsigned uTM : 1; // bit 29: Thermal Monitor
		unsigned : 1; // bit 30
		unsigned uPBE : 1; // bit 31: Pending Break Enable
	};
	template <> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x01> const & cpuid)
	{
		return os <<
			(cpuid.uSSE3 ? '+' : '-') << "SSE3" << std::ends << //unsigned uSSE3 : 1; // bit 0
			(cpuid.uPCLMULQDQ ? '+' : '-') << "PCLMULQDQ" << std::ends << //unsigned uPCLMULQDQ : 1; // bit 1
			//unsigned uDTES64 : 1; // bit 2
			(cpuid.uMONITOR ? '+' : '-') << "MONITOR" << std::ends << //unsigned uMONITOR : 1; // bit 3
			//unsigned uDS_CPL : 1; // bit 4
			(cpuid.uVMX ? '+' : '-') << "VMX" << std::ends << //unsigned uVMX : 1; // bit 5
			(cpuid.uSMX ? '+' : '-') << "SMX" << std::ends << //unsigned uSMX : 1; // bit 6: Safer Mode Extensions
			(cpuid.uEIST ? '+' : '-') << "EIST" << std::ends << //unsigned uEIST : 1; // bit 7: Enhanced Intel SpeedStep technology
			(cpuid.uTM2 ? '+' : '-') << "TM2" << std::ends << //unsigned uTM2 : 1; // bit 8: Thermal Monitor 2
			(cpuid.uSSSE3 ? '+' : '-') << "SSSE3" << std::ends << //unsigned uSSSE3 : 1; // bit 9
			//unsigned uCNXT_ID : 1; // bit 10: L1 Context ID
			//unsigned uSDBG : 1; // bit 11
			(cpuid.uFMA ? '+' : '-') << "FMA" << std::ends << //unsigned uFMA : 1; // bit 12
			(cpuid.uCMPXCHG16B ? '+' : '-') << "CMPXCHG16B" << std::ends << //unsigned uCMPXCHG16B : 1; // bit 13
			//unsigned u_xTPR_UpdateControl : 1; // bit 14
			(cpuid.uPDCM ? '+' : '-') << "PDCM" << std::ends << //unsigned uPDCM : 1; // bit 15: Perfmon and Debug Capability
			//unsigned : 1; // bit 16
			//unsigned uPCID : 1; // bit 17: Process-context identifiers
			//unsigned uDCA : 1; // bit 18
			(cpuid.uSSE4_1 ? '+' : '-') << "SSE4.1" << std::ends << //unsigned uSSE4_1 : 1; // bit 19
			(cpuid.uSSE4_2 ? '+' : '-') << "SSE4.2" << std::ends << //unsigned uSSE4_2 : 1; // bit 20
			(cpuid.u_x2APIC ? '+' : '-') << "x2APIC" << std::ends << //unsigned u_x2APIC : 1; // bit 21
			(cpuid.uMOVBE ? '+' : '-') << "MOVBE" << std::ends << //unsigned uMOVBE : 1; // bit 22
			(cpuid.uPOPCNT ? '+' : '-') << "POPCNT" << std::ends << //unsigned uPOPCNT : 1; // bit 23
			(cpuid.uTSC_Deadline ? '+' : '-') << "TSC-Deadline" << std::ends << //unsigned uTSC_Deadline : 1; // bit 24
			(cpuid.uAES ? '+' : '-') << "AES" << std::ends << //unsigned uAES : 1; // bit 25
			(cpuid.uXSAVE ? '+' : '-') << "XSAVE" << std::ends << //unsigned uXSAVE : 1; // bit 26
			(cpuid.uOSXSAVE ? '+' : '-') << "OSXSAVE" << std::ends << //unsigned uOSXSAVE : 1; // bit 27
			(cpuid.uAVX ? '+' : '-') << "AVX" << std::ends << //unsigned uAVX : 1; // bit 28
			(cpuid.uF16C ? '+' : '-') << "F16C" << std::ends << //unsigned uF16C : 1; // bit 29
			(cpuid.uRDRAND ? '+' : '-') << "RDRAND" << std::ends << //unsigned uRDRAND : 1; // bit 30
			//unsigned : 1;
			(cpuid.uFPU ? '+' : '-') << "FPU" << std::ends << //unsigned uFPU : 1; // bit 0
			(cpuid.uVME ? '+' : '-') << "VME" << std::ends << //unsigned uVME : 1; // bit 1: Virtual 8086 Mode Enhancements
			(cpuid.uDE ? '+' : '-') << "DE" << std::ends << //unsigned uDE : 1; // bit 2: Debugging Extensions
			(cpuid.uPSE ? '+' : '-') << "PSE" << std::ends << //unsigned uPSE : 1; // bit 3: Page Size Extension
			(cpuid.uTSC ? '+' : '-') << "TSC" << std::ends << //unsigned uTSC : 1; // bit 4
			(cpuid.uMSR ? '+' : '-') << "MSR" << std::ends << //unsigned uMSR : 1; // bit 5
			(cpuid.uPAE ? '+' : '-') << "PAE" << std::ends << //unsigned uPAE : 1; // bit 6
			(cpuid.uMCE ? '+' : '-') << "MCE" << std::ends << //unsigned uMCE : 1; // bit 7: Machine Check Exception
			(cpuid.uCX8 ? '+' : '-') << "CX8" << std::ends << //unsigned uCX8 : 1; // bit 8: CMPXCHG8B Instruction
			(cpuid.uAPIC ? '+' : '-') << "APIC" << std::ends << //unsigned uAPIC : 1; // bit 9
			//unsigned : 1; // bit 10
			(cpuid.uSEP ? '+' : '-') << "SEP" << std::ends << //unsigned uSEP : 1; // bit 11: SYSENTER and SYSEXIT Instructions
			(cpuid.uMTRR ? '+' : '-') << "MTRR" << std::ends << //unsigned uMTRR : 1; // bit 12
			(cpuid.uPGE ? '+' : '-') << "PGE" << std::ends << //unsigned uPGE : 1; // bit 13: Page Global Bit
			(cpuid.uMCA ? '+' : '-') << "MCA" << std::ends << //unsigned uMCA : 1; // bit 14 Machine Check Architecture
			(cpuid.uCMOV ? '+' : '-') << "CMOV" << std::ends << //unsigned uCMOV : 1; // bit 15
			(cpuid.uPAT ? '+' : '-') << "PAT" << std::ends << //unsigned uPAT : 1; // bit 16: Page Attribute Table
			(cpuid.uPSE36 ? '+' : '-') << "PSE36" << std::ends << //unsigned uPSE36 : 1; // bit 17: 36-Bit Page Size Extension
			(cpuid.uPSN ? '+' : '-') << "PSN" << std::ends << //unsigned uPSN : 1; // bit 18: Processor Serial Number
			(cpuid.uCLFSH ? '+' : '-') << "CLFSH" << std::ends << //unsigned uCLFSH : 1; // bit 19
			//unsigned : 1; // bit 20
			(cpuid.uDS ? '+' : '-') << "DS" << std::ends << //unsigned uDS : 1; // bit 21: Debug Store
			(cpuid.uACPI ? '+' : '-') << "ACPI" << std::ends << //unsigned uACPI : 1; // bit 22: Thermal Monitor and Software Controlled Clock Facilities
			(cpuid.uMMX ? '+' : '-') << "MMX" << std::ends << //unsigned uMMX : 1; // bit 23
			(cpuid.uFXSR ? '+' : '-') << "FXSR" << std::ends << //unsigned uFXSR : 1; // bit 24
			(cpuid.uSSE ? '+' : '-') << "SSE" << std::ends << //unsigned uSSE : 1; // bit 25
			(cpuid.uSSE2 ? '+' : '-') << "SSE2" << std::ends << //unsigned uSSE2 : 1; // bit 26
			(cpuid.uSS ? '+' : '-') << "SS" << std::ends << //unsigned uSS : 1; // bit 27: Self Snoop
			(cpuid.uHTT ? '+' : '-') << "HTT" << std::ends << //unsigned uHTT : 1; // bit 28: Max APIC IDs reserved field is Valid
			(cpuid.uTM ? '+' : '-') << "TM" << std::ends << //unsigned uTM : 1; // bit 29: Thermal Monitor
			//unsigned : 1; // bit 30
			(cpuid.uPBE ? '+' : '-') << "PBE" << std::ends;//unsigned uPBE : 1; // bit 31: Pending Break Enable
	}
	template <> struct cpuid_base_t<0x02>
	{
		// eax
		unsigned uEAX;
		// ebx
		unsigned uEBX;
		// ecx
		unsigned uECX;
		// edx
		unsigned uEDX;
	};
	template <int nECX> struct cpuid_base_t<0x04, nECX>
	{
		// eax
		unsigned uCacheTypeField : 5; // [bits 04:00]
		unsigned uCacheLevel : 3; // [bits 07:05]
		unsigned uSelfInitializingCacheLevel : 1; // bit 8
		unsigned uFullyAssociativeCache : 1; // bit 9
		unsigned : 4; // [bits 13:10]
		unsigned uMaximumNumberOfAddressableIDsForLogicalProcessors : 12; // [bits 25:14]
		unsigned uMaximumNumberOfAddressableIDsForProcessorCores : 6; // [bits 31:26]
		// ebx
		unsigned uSystemCoherencyLineSize : 12; // [bits 11:0]
		unsigned uPhysicalLinePartitions : 10; // [bits 21:12]
		unsigned uWaysOfAssociativity : 10; // [bits 31:22]
		// ecx
		unsigned uNumberOfSets : 32;
		// edx
		unsigned uWriteBackInvalidate_Invalidate : 1; // bit 0
		unsigned uCacheInclusiveness : 1; // bit 1
		unsigned uComplexCacheIndexing : 1; // bit 2
		unsigned : 29;
	};
	template <> struct cpuid_base_t<0x05>
	{
		// eax
		unsigned uSmallestLineSizeMONITOR : 16; // [bits 15:0]
		unsigned : 16;
		// ebx
		unsigned uLargestLineSizeMONITOR : 16; // [bits 15:0]
		unsigned : 16;
		// ecx
		unsigned uEnumeration : 1; // bit 0
		unsigned uInterruptsAsBreakEvent : 1; // bit 1
		unsigned : 30;
		// edx
		unsigned uNumberOfC0 : 4;
		unsigned uNumberOfC1 : 4;
		unsigned uNumberOfC2 : 4;
		unsigned uNumberOfC3 : 4;
		unsigned uNumberOfC4 : 4;
		unsigned uNumberOfC5 : 4;
		unsigned uNumberOfC6 : 4;
		unsigned uNumberOfC7 : 4;
	};
	template <> struct cpuid_base_t<0x06>
	{
		// eax
		unsigned uDigitalThermalSensor : 1; // bit 0
		unsigned uIntelTurboBoostTechnology : 1; // bit 1
		unsigned uARAT : 1; // bit 2: APIC-Timer always-running feature is supported if set
		unsigned : 1;
		unsigned uPLN : 1; // bit 4: Power limit notification controls are supported if set
		unsigned uECMD : 1; // bit 5: Clock modulation duty cycle extension is supported if set
		unsigned uPTM : 1; // bit 6: Package thermal management is supported if set.
		unsigned uHWP : 1; // bit 7
		unsigned uHWP_Notification : 1; // bit 8
		unsigned uHWP_ActivityWindow : 1; // bit 9
		unsigned uHWP_EnergyPerformancePreference : 1; // bit 10
		unsigned uHWP_PackageLevelRequest : 1; // bit 11
		unsigned : 1;
		unsigned uHDC : 1; // bit 13
		unsigned : 18;
		// ebx
		unsigned uNumOfThermalThreshold : 4; // [bits 3:0]: Number of Interrupt Thresholds in Digital Thermal Sensor
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
		unsigned uFSGSBASE : 1; // bit 0
		unsigned uIA32_TSC_ADJUST : 1; // bit 1
		unsigned uIntelSGX : 1; // bit 2
		unsigned uBMI1 : 1; // bit 3
		unsigned uHLE : 1; // bit 4
		unsigned uAVX2 : 1; // bit 5
		unsigned uFDP_EXCPTN_ONLY : 1; // bit 6: x87 FPU Data Pointer updated only on x87 exceptions if 1
		unsigned uSMEP : 1; // bit 7: Supports Supervisor-Mode Execution Prevention if 1.
		unsigned uBMI2 : 1; // bit 8
		unsigned uFastStringOperation : 1; // bit 9: Supports Enhanced REP MOVSB/STOSB if 1.
		unsigned uINVPCID : 1; // bit 10: If 1, supports INVPCID instruction for system software that manages process-context identifiers.
		unsigned uRTM : 1; // bit 11
		unsigned uPQM : 1; // bit 12: Supports Platform Quality of Service Monitoring (PQM) capability if 1.
		unsigned uFCS_FDS : 1; // bit 13: suppress FCS, FDS
		unsigned uIntelMPX : 1; // bit 14
		unsigned uPQE : 1; // bit 15: Supports Platform Quality of Service Enforcement (PQE) capability if 1.
		unsigned : 2;
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
		unsigned : 2;
		unsigned uPKU : 1; // bit 3: Supports protection keys for user-mode pages if 1.
		unsigned uOSPKE : 1; // bit 4: If 1, OS has set CR4.PKE to enable protection keys (and the RDPKRU/WRPKRU instructions)
		unsigned : 27;
		// edx
		unsigned : 32;
	};
	template <> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x07> const & cpuid)
	{
		return os <<
			(cpuid.uFSGSBASE ? '+' : '-') << "FSGSBASE" << std::ends <<
			(cpuid.uIntelSGX ? '+' : '-') << "SGX" << std::ends <<
			(cpuid.uBMI1 ? '+' : '-') << "BMI1" << std::ends <<
			(cpuid.uHLE ? '+' : '-') << "HLE" << std::ends <<
			(cpuid.uAVX2 ? '+' : '-') << "AVX2" << std::ends <<
			(cpuid.uSMEP ? '+' : '-') << "SMEP" << std::ends <<
			(cpuid.uBMI2 ? '+' : '-') << "BMI2" << std::ends <<
			(cpuid.uFastStringOperation ? '+' : '-') << "Fast-String" << std::ends <<
			(cpuid.uINVPCID ? '+' : '-') << "INVPCID" << std::ends <<
			(cpuid.uRTM ? '+' : '-') << "RTM" << std::ends <<
			(cpuid.uPQM ? '+' : '-') << "PQM" << std::ends <<
			(cpuid.uIntelMPX ? '+' : '-') << "MPX" << std::ends <<
			(cpuid.uPQE ? '+' : '-') << "PQE" << std::ends <<
			(cpuid.uRDSEED ? '+' : '-') << "RDSEED" << std::ends <<
			(cpuid.uADX ? '+' : '-') << "ADX" << std::ends <<
			(cpuid.uSMAP ? '+' : '-') << "SMAP" << std::ends <<
			(cpuid.uCLFLUSHOPT ? '+' : '-') << "CLFLUSHOPT" << std::ends <<
			(cpuid.uIntelProcessorTrace ? '+' : '-') << "PT" << std::ends <<
			(cpuid.uPREFTEHCHWT1 ? '+' : '-') << "PREFTEHCHWT1" << std::ends <<
			(cpuid.uPKU ? '+' : '-') << "PKU" << std::ends <<
			(cpuid.uOSPKE ? '+' : '-') << "OSPKE" << std::ends;
	}
	template <> struct cpuid_base_t<0x09>
	{
		// eax
		unsigned uIA32_PLATFORM_DCA_CAP : 32;
		// ebx
		unsigned : 32;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x0A>
	{
		// eax
		unsigned uVersionID : 8; // [bits 7:0]: Version ID of architectural performance monitoring
		unsigned uNumOfGP : 8; // [bits 15:8]: Number of general-purpose performance monitoring counter per logical processor
		unsigned uBitWidthOfGP : 8; // [bits 23:16]: Bit width of general-purpose, performance monitoring counter
		unsigned uLengethOfEBX : 8; // [bits 31:24]: Length of EBX bit vector to enumerate architectural performance monitoring events
		// ebx
		unsigned uCoreCycleEventUnavailable : 1; // bit 0
		unsigned uInstructionRetiredEventUnavailable : 1; // bit 1
		unsigned uReferenceCyclesEventUnavailable : 1; // bit 2
		unsigned uLLC_ReferenceEventUnavailable : 1; // bit 3
		unsigned uLLC_MissesEventUnavailable : 1; // bit 4
		unsigned uBranchInstructionRetiredEventUnavailable : 1; // bit 5
		unsigned uBranchMispredictRetiredEventUnavailable : 1; // bit 6
		unsigned : 25;
		// ecx
		unsigned : 32;
		// edx
		unsigned uNumOfFixedFunctionPerformanceCounters : 5; // [bits 4:0]
		unsigned uBitWidthOfFixedFunctionPerformanceCounters : 8; // [bits 12:5]
		unsigned : 19;
	};
	template <int nECX> struct cpuid_base_t<0x0B, nECX>
	{
		// eax
		unsigned uNumOfBitsToShift : 5; // [bits 4:0]
		unsigned : 27;
		// ebx
		unsigned uNumOfLogicalProcessors : 16;
		unsigned : 16;
		// ecx
		unsigned uLevelNumber : 8; // [bits 7:0]
		unsigned uLevelType : 8; // [bits 15:8]
		unsigned : 16;
		// edx
		unsigned u_x2APIC_ID : 32;
	};
	template <int nECX> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x0B, nECX> const & cpuid)
	{
		switch (cpuid.uLevelType)
		{
		case 0:
			os << "Invalid" << std::ends;
			break;
		case 1:
			os << "SMT" << std::ends;
			break;
		case 2:
			os << "Core" << std::ends;
			break;
		default:
			os << "Reserved" << std::ends;
			break;
		}
		return os;
	}
	template <> struct cpuid_base_t<0x0D>
	{
		// eax
		unsigned uStateX87 : 1; // bit 0
		unsigned uStateSSE : 1; // bit 1
		unsigned uStateAVX : 1;
		unsigned uStateBNDREGS : 1; // bit 3
		unsigned uStateBNDCSR : 1; // bit 4
		unsigned uStateAVX512 : 3; // [bits 7:5]
		unsigned uStatePT : 1; // bit 8
		unsigned uStatePKRU : 1; // bit 9
		unsigned : 22;
		// ebx
		unsigned uSizeXCR0: 32;
		// ecx
		unsigned uSize : 32;
		// edx
		unsigned : 32;
	};
	template <> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x0D> const & cpuid)
	{
		return os <<
			(cpuid.uStateX87 ? '+' : '-') << "x87" << std::ends <<
			(cpuid.uStateSSE ? '+' : '-') << "SSE" << std::ends <<
			(cpuid.uStateAVX ? '+' : '-') << "AVX" << std::ends <<
			(cpuid.uStateBNDREGS ? '+' : '-') << "BNDREGS" << std::ends <<
			(cpuid.uStateBNDCSR ? '+' : '-') << "BNDCSR" << std::ends <<
			(cpuid.uStatePT ? '+' : '-') << "PT" << std::ends <<
			(cpuid.uStatePKRU ? '+' : '-') << "PKRU" << std::ends;
	}
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
	template <> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x0D, 1> const & cpuid)
	{
		return os <<
			(cpuid.uXSAVEOPT ? '+' : '-') << "XSAVEOPT" << std::ends <<
			(cpuid.uCompactionExtensions ? '+' : '-') << "Compaction-Extensions" << std::ends <<
			(cpuid.uXGETBV ? '+' : '-') << "XGETBV" << std::ends <<
			(cpuid.uXSAVES ? '+' : '-') << "XSAVES" << std::ends;
	}
	template <int nECX> struct cpuid_base_t<0x0D, nECX> // AVX state, BNGREG state, BNDCSR state, Opmask state, ZMM_Hi256 state, Hi16_ZMM state, PT state, PKRU state
	{
		// eax
		unsigned uSize;
		// ebx
		unsigned uOffset;
		// ecx
		unsigned uIA32_XSS : 1; // bit 0
		unsigned uAlign64 : 1; // bit 1
		unsigned : 30;
		// edx
		unsigned : 32;
	};
	template <> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x0D, 2> const & cpuid) { return os << "AVX"; }
	template <> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x0D, 3> const & cpuid) { return os << "BNGREG"; }
	template <> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x0D, 4> const & cpuid) { return os << "BNGCSR"; }
	template <> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x0D, 5> const & cpuid) { return os << "Opmask"; }
	template <> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x0D, 6> const & cpuid) { return os << "ZMM_Hi256"; }
	template <> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x0D, 7> const & cpuid) { return os << "Hi16_ZMM"; }
	template <> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x0D, 8> const & cpuid) { return os << "PT"; }
	template <> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x0D, 9> const & cpuid) { return os << "PKRU"; }

	template <> struct cpuid_base_t<0x0F>
	{
		// eax
		unsigned : 32;
		// ebx
		unsigned uMaxOfRMID : 32;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 1;
		unsigned uL3CacheQoS_Monitoring : 1; // bit 1
		unsigned : 30;
	};
	template <> struct cpuid_base_t<0x0F, 1>
	{
		// eax
		unsigned : 32;
		// ebx
		unsigned uConversionFactor : 32;
		// ecx
		unsigned uMaxOfRMID : 32;
		// edx
		unsigned uL3OccupancyMonitoring : 1; // bit 0
		unsigned : 31;
	};
	template <> struct cpuid_base_t<0x10>
	{
		// eax
		unsigned : 32;
		// ebx
		unsigned : 1;
		unsigned uL3CacheQoS_Enforcement : 1; // bit 1
		unsigned : 30;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x10, 1>
	{
		// eax
		unsigned uNumOfCapacity : 5; // [bits 4:0]
		unsigned : 27;
		// ebx
		unsigned uAllocationUnitsBitmap : 32;
		// ecx
		unsigned : 1;
		unsigned uInfrequentUpdateOfCOS : 1; // bit 1
		unsigned uPrioritizationTechnology : 1; // bit 2
		unsigned : 29;
		// edx
		unsigned uMaxOfCOS : 16; // [bits 15:0]
		unsigned : 16;
	};
	template <> struct cpuid_base_t<0x12>
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
	template <> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x12> const & cpuid)
	{
		return os <<
			(cpuid.uSGX1 ? '+' : '-') << "SGX1" << std::ends <<
			(cpuid.uSGX2 ? '+' : '-') << "SGX2" << std::ends;
	}
	template <> struct cpuid_base_t<0x14>
	{
		// eax
		unsigned uMaxSubLeaf : 32;
		// ebx
		unsigned uIA32_RTIT_CR3 : 1; // bit 0: IA32_RTIT_CTL.CR3Filter can be set to 1, and that IA32_RTIT_CR3_MATCH MSR can be accessed
		unsigned uConfigurablePSB : 1; // bit 1: Configurable PSB and Cycle-Accurate Mode.
		unsigned uIntelPT : 1; // bit 2: IP Filtering, TraceStop filtering, and preservation of Intel PT MSRs across warm reset.
		unsigned uMTC : 1; // bit 3: MTC timing packet and suppression of COFI-based packets
		unsigned : 28;
		// ecx
		unsigned uIA32_RTIT_CTL_ToPA : 1; // bit 0
		unsigned uIA32_RTIT_OUTPUT_MASK_PTRS : 1; // bit 1
		unsigned uSingleRangeOutputScheme : 1; // bit 2
		unsigned uOutputToTraceTransportSubsystem : 1; // bit 3
		unsigned : 27;
		unsigned uLIP : 1; // bit 31: Generated packets which contain IP payloads have LIP values, which include the CS base component.
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x14, 1>
	{
		// eax
		unsigned uNumOfConfigurableAddressRanges : 3; // [bits 2:0]
		unsigned : 13;
		unsigned uMTC_PeriodEncodingsBitmap : 16; // [bits 31:16]
		// ebx
		unsigned uCycleThresholdValueEncodingsBitmap : 16; // [bits 15:0]
		unsigned uConfigurablePSB_FrequencyEncodingsBitmap : 16; // [bits 31:16]
		// ecx
		unsigned : 32;
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x15>
	{
		// eax
		unsigned uCoreCrystalClock : 32;
		// ebx
		unsigned uTSC : 32;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 32;
	};
	template <> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x15> const & cpuid)
	{
		return os <<
			cpuid.uTSC << '/' << cpuid.uCoreCrystalClock << std::ends;
	}
	template <> struct cpuid_base_t<0x16>
	{
		// eax
		unsigned uProcessorBaseFrequencyMHz : 16; // [bits 15:0]
		unsigned : 16;
		// ebx
		unsigned uMaximumFrequencyMHz : 16; // [bits 15:0]
		unsigned : 16;
		// ecx
		unsigned uBusFrequencyMHz : 16; // [bits 15:0]
		unsigned : 16;
		// edx
		unsigned : 32;
	};
	template <> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x16> const & cpuid)
	{
		return os <<
			cpuid.uProcessorBaseFrequencyMHz << "MHz" << std::ends <<
			cpuid.uMaximumFrequencyMHz << "MHz" << std::ends <<
			cpuid.uBusFrequencyMHz << "MHz" << std::ends;
	}
	template <> struct cpuid_base_t<0x17>
	{
		// eax
		unsigned uMaxSOCID_Index : 32;
		// ebx
		unsigned uSOC_VendorID : 16;
		unsigned uIsVendorScheme : 1;
		unsigned : 15;
		// ecx
		unsigned uProjectID : 32;
		// edx
		unsigned uSteppingID : 32;
	};
	template <> struct cpuid_base_t<0x17, 1>
	{
		char ac[16];

		char const * soc_vendor_brand_string(void) const
		{
			return ac;
		}
	};
	template <> struct cpuid_base_t<0x17, 2>
	{
		char ac[16];
	};
	template <> struct cpuid_base_t<0x17, 3>
	{
		char ac[16];
	};
	template <int nECX> std::enable_if_t<2 <= nECX && nECX <= 4, std::ostream &> operator << (std::ostream & os, cpuid_base_t<0x17, nECX> const & cpuid)
	{
		std::for_each(std::cbegin(cpuid.ac), std::cend(cpuid.ac), [&](char c)
		{
			os << c;
		});
		return os;
	}

	template <> struct cpuid_base_t<0x80000000>
	{
		// eax
		unsigned uMaximumLeaf : 32;
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
		unsigned uExtendedProcessorSignatureAndFeatureBits : 32;
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
		unsigned : 11;
		unsigned uSYSCALL_SYSRET_64bitMode : 1; // bit 11
		unsigned : 8;
		unsigned uExecuteDiableBit : 1; // bit 20
		unsigned : 5;
		unsigned u1GBytePages : 1; // bit 26
		unsigned uRDTSCP : 1; // bit 27
		unsigned : 1;
		unsigned uIntel64 : 1; // bit 29
		unsigned : 2;
	};
	template <> std::ostream & operator << (std::ostream & os, cpuid_base_t<0x80000001> const & cpuid)
	{
		return os <<
			(cpuid.uLZCNT ? '+' : '-') << "LZCNT" << std::ends <<
			(cpuid.uPREFTEHCHW ? '+' : '-') << "PREFTEHCHW" << std::ends <<
			(cpuid.uRDTSCP ? '+' : '-') << "RDTSCP" << std::ends <<
			(cpuid.uIntel64 ? '+' : '-') << "x64" << std::ends;
	}
	template <> struct cpuid_base_t<0x80000002>
	{
		char ac[16];

		char const * processor_brand_string(void) const
		{
			return ac;
		}
	};
	template <> struct cpuid_base_t<0x80000003>
	{
		char ac[16];
	};
	template <> struct cpuid_base_t<0x80000004>
	{
		char ac[16];
	};
	template <int nEAX> std::enable_if_t<0x80000002 <= nEAX && nEAX <= 0x80000004, std::ostream &> operator << (std::ostream & os, cpuid_base_t<nEAX> const & cpuid)
	{
		std::for_each(std::cbegin(cpuid.ac), std::cend(cpuid.ac), [&](char c)
		{
			os << c;
		});
		return os;
	}
	template <> struct cpuid_base_t<0x80000006>
	{
		// eax
		unsigned : 32;
		// ebx
		unsigned : 32;
		// ecx
		unsigned uCacheLineSize : 8; // [bits 7:0]
		unsigned : 4;
		unsigned uL2Associativity : 4; // [bits 15:12]
		unsigned uCacheSize1K : 16; // [bits 31:16]
		// edx
		unsigned : 32;
	};
	template <> struct cpuid_base_t<0x80000007>
	{
		// eax
		unsigned : 32;
		// ebx
		unsigned : 32;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 8;
		unsigned uInvariantTSC : 1; // bit 8
		unsigned : 23;
	};
	template <> struct cpuid_base_t<0x80000008>
	{
		// eax
		unsigned uNumOfPhysicalAddressBits : 8; // [bits 7:0]
		unsigned uNumOfLinearAddressBits : 8; // [bits 15:8]
		unsigned : 16;
		// ebx
		unsigned : 32;
		// ecx
		unsigned : 32;
		// edx
		unsigned : 32;
	};

	template <int nEAX, int nECX = 0>
	struct cpuid_t
		: cpuid_base_t<nEAX, nECX>
	{
		cpuid_t()
		{
			__cpuidex(reinterpret_cast<int*>(this), nEAX, nECX);

			static_assert(sizeof(cpuid_base_t<nEAX, nECX>) == 16, "CPUID expects 4 32-bit integers");
		}

		cpuid_t const & operator >> (std::ostream & os) const
		{
			int nMask = os.setf(std::ios_base::hex);
			os.unsetf(std::ios_base::dec);
			os <<
				std::setw(8) << nEAX << std::ends <<
				std::setw(3) << nECX << std::ends <<
				std::setw(8) << reinterpret_cast<int const*>(this)[0] << std::ends <<
				std::setw(8) << reinterpret_cast<int const*>(this)[1] << std::ends <<
				std::setw(8) << reinterpret_cast<int const*>(this)[2] << std::ends <<
				std::setw(8) << reinterpret_cast<int const*>(this)[3] << std::ends;
			os.setf(nMask);
			os <<
				static_cast<cpuid_base_t<nEAX, nECX>const&>(*this) << std::endl;
			return *this;
		}
	};
	template <int nEAX, int nECX>
	std::ostream & operator << (std::ostream & os, cpuid_t<nEAX, nECX> const & cpuid)
	{
		cpuid >> os;
		return os;
	}

	struct cpuid_soc_vendor_brand_string_t
		: cpuid_t<0x17, 1>
		, cpuid_t<0x17, 2>
		, cpuid_t<0x17, 3>
	{
		operator char const * (void) const
		{
			return reinterpret_cast<char const*>(this);
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

	//
	template <int nEAX, int nECX = 0>
	struct cpuid_tree_traits;

	template <int nEAX, int nECX>
	struct cpuid_tree_traits
	{
		enum { DEC_EAX = 0, INC_ECX = 1 };
	};
	template <int nEAX>
	struct cpuid_tree_traits<nEAX>
	{
		enum { DEC_EAX = 1, INC_ECX = 0 };
	};
	template <> struct cpuid_tree_traits<0>
	{
		enum { DEC_EAX = 0, INC_ECX = 0 };
	};
	template <> struct cpuid_tree_traits<0x04>
	{
		enum { DEC_EAX = 1, INC_ECX = 1 };
	};
	template <> struct cpuid_tree_traits<0x04, 4>
	{
		enum { DEC_EAX = 0, INC_ECX = 0 };
	};
	template <int nECX> struct cpuid_tree_traits<0x04, nECX>
	{
		enum { DEC_EAX = 0, INC_ECX = 1 };
	};
	template <> struct cpuid_tree_traits<0x0B>
	{
		enum { DEC_EAX = 1, INC_ECX = 1 };
	};
	template <> struct cpuid_tree_traits<0x0B, 2>
	{
		enum { DEC_EAX = 0, INC_ECX = 0 };
	};
	template <int nECX> struct cpuid_tree_traits<0x0B, nECX>
	{
		enum { DEC_EAX = 0, INC_ECX = 1 };
	};
	template <> struct cpuid_tree_traits<0x0D>
	{
		enum { DEC_EAX = 1, INC_ECX = 1 };
	};
	template <> struct cpuid_tree_traits<0x0D, 9>
	{
		enum { DEC_EAX = 0, INC_ECX = 0 };
	};
	template <int nECX> struct cpuid_tree_traits<0x0D, nECX>
	{
		enum { DEC_EAX = 0, INC_ECX = 1 };
	};
	template <> struct cpuid_tree_traits<0x0F>
	{
		enum { DEC_EAX = 1, INC_ECX = 1 };
	};
	template <> struct cpuid_tree_traits<0x0F, 2>
	{
		enum { DEC_EAX = 0, INC_ECX = 0 };
	};
	template <int nECX> struct cpuid_tree_traits<0x0F, nECX>
	{
		enum { DEC_EAX = 0, INC_ECX = 1 };
	};
	template <> struct cpuid_tree_traits<0x10>
	{
		enum { DEC_EAX = 1, INC_ECX = 1 };
	};
	template <> struct cpuid_tree_traits<0x10, 2>
	{
		enum { DEC_EAX = 0, INC_ECX = 0 };
	};
	template <int nECX> struct cpuid_tree_traits<0x10, nECX>
	{
		enum { DEC_EAX = 0, INC_ECX = 1 };
	};
	template <> struct cpuid_tree_traits<0x14>
	{
		enum { DEC_EAX = 1, INC_ECX = 1 };
	};
	template <> struct cpuid_tree_traits<0x14, 2>
	{
		enum { DEC_EAX = 0, INC_ECX = 0 };
	};
	template <int nECX> struct cpuid_tree_traits<0x14, nECX>
	{
		enum { DEC_EAX = 0, INC_ECX = 1 };
	};
	template <> struct cpuid_tree_traits<0x17>
	{
		enum { DEC_EAX = 1, INC_ECX = 1 };
	};
	template <> struct cpuid_tree_traits<0x17, 3>
	{
		enum { DEC_EAX = 0, INC_ECX = 0 };
	};
	template <int nECX> struct cpuid_tree_traits<0x17, nECX>
	{
		enum { DEC_EAX = 0, INC_ECX = 1 };
	};
	template <> struct cpuid_tree_traits<0x80000000>
	{
		enum { DEC_EAX = 0, INC_ECX = 0 };
	};

	template <int nEAX, int nECX = 0, typename TT = cpuid_tree_traits<nEAX, nECX> >
	struct cpuid_tree_t
		: std::conditional_t<TT::DEC_EAX, cpuid_tree_t<nEAX - TT::DEC_EAX, nECX>, cpuid_tree_traits<nEAX, nECX> >
		, cpuid_t<nEAX, nECX>
		, std::conditional_t<TT::INC_ECX, cpuid_tree_t<nEAX, nECX + TT::INC_ECX>, cpuid_tree_traits<nEAX, nECX + 1> >
	{
		template <typename T>
		void print(std::ostream & os) const
		{
			static_cast<T const&>(*this) >> os;
		}
		template <> void print<cpuid_tree_traits<nEAX, nECX>>(std::ostream &) const {}
		template <> void print<cpuid_tree_traits<nEAX, nECX + 1>>(std::ostream &) const {}

		template <bool bZeroECX>
		bool should_print_leaf(void) const;

		template <> bool should_print_leaf<true>(void) const
		{
			return nEAX <= uMaximumLeaf;
		}
		template <> bool should_print_leaf<false>(void) const
		{
			return true;
		}

		void print(std::ostream & os, int nMaxEAX, int nMaxECX)
		{
			if (nEAX <= nMaxEAX && nECX <= nMaxECX)
			{
			}
		}

		cpuid_tree_t const & operator >> (std::ostream & os) const
		{
			print<std::conditional_t<TT::DEC_EAX, cpuid_tree_t<nEAX - TT::DEC_EAX, nECX>, cpuid_tree_traits<nEAX, nECX> >>(os);
			if (should_print_leaf<nECX == 0>())
			{
				print<cpuid_t<nEAX, nECX>>(os);
				print<std::conditional_t<TT::INC_ECX, cpuid_tree_t<nEAX, nECX + TT::INC_ECX>, cpuid_tree_traits<nEAX, nECX + 1> >>(os);
			}
			return *this;
		}
	};

	template <int nEAX, int nECX>
	std::ostream & operator << (std::ostream & os, cpuid_tree_t<nEAX, nECX> const & cpuid)
	{
		cpuid >> os;
		return os;
	}
}
