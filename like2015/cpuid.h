#pragma once

/* Author: jrmwng @ 2015,2016 */

#include <intrin.h>
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

	template <> struct cpuid_base_t<0x00>
	{
		// eax
		unsigned uMaximumInputValue : 32;
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
		unsigned uAVX512 : 3; // [bits 7:5]
		unsigned : 1; // bit 8
		unsigned uPKRU : 1; // bit 9
		unsigned : 22;
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
	};
	template <> struct cpuid_base_t<0x17, 2>
	{
		char ac[16];
	};
	template <> struct cpuid_base_t<0x17, 3>
	{
		char ac[16];
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
	};
	template <int nEAX, int nECX>
	std::ostream & operator << (std::ostream & os, cpuid_t<nEAX, nECX> const & cpuid)
	{
		int nMask = os.setf(std::ios_base::hex);
		os.unsetf(std::ios_base::dec);
		os <<
			std::setw(8) << nEAX << std::ends <<
			std::setw(3) << nECX << std::ends <<
			std::setw(8) << reinterpret_cast<int const*>(&cpuid)[0] << std::ends <<
			std::setw(8) << reinterpret_cast<int const*>(&cpuid)[1] << std::ends <<
			std::setw(8) << reinterpret_cast<int const*>(&cpuid)[2] << std::ends <<
			std::setw(8) << reinterpret_cast<int const*>(&cpuid)[3] << std::endl;
		os.setf(nMask);
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
}