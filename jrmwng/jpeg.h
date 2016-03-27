#pragma once

/* Author: jrmwng @ 2016 */

namespace jrmwng
{
	enum jpeg_marker_e : unsigned char
	{
		// TEM
		JPEG_MARKER_TEM = 0x01,
		// SOF: Start of Frame
		// SOF0: Baseline
		JPEG_MARKER_SOF0 = 0xC0,
		// SOF1: Extended sequential, Huffman
		JPEG_MARKER_SOF1 = 0xC1,
		// SOF2: Progressive, Huffman
		JPEG_MARKER_SOF2 = 0xC2,
		// SOF3: Lossless, Huffman
		JPEG_MARKER_SOF3 = 0xC3,
		// DHT: Define Huffman Table
		JPEG_MARKER_DHT = 0xC4,
		// SOF5: Differential sequential, Huffman
		JPEG_MARKER_SOF5 = 0xC5,
		// SOF6: Differential progressive, Huffman
		JPEG_MARKER_SOF6 = 0xC6,
		// SOF7: Differential lossless, Huffman
		JPEG_MARKER_SOF7 = 0xC7,
		// JPG: Reserved for JPEG extensions
		JPEG_MARKER_JPG = 0xC8,
		// SOF9: Extended sequential, arithmetic
		JPEG_MARKER_SOF9 = 0xC9,
		// SOF10: Progressive, arithmetic
		JPEG_MARKER_SOF10 = 0xCA,
		// SOF11: Lossless, arithmetic
		JPEG_MARKER_SOF11 = 0xCB,
		// DAC
		JPEG_MARKER_DAC = 0xCC,
		// SOF13: Differential sequential, arithmetic
		JPEG_MARKER_SOF13 = 0xCD,
		// SOF14: Differential progressive, arithmetic
		JPEG_MARKER_SOF14 = 0xCE,
		// SOF15: Lossless, arithmetic
		JPEG_MARKER_SOF15 = 0xCF,
		// RST
		JPEG_MARKER_RST0 = 0xD0,
		JPEG_MARKER_RST1 = 0xD1,
		JPEG_MARKER_RST2 = 0xD2,
		JPEG_MARKER_RST3 = 0xD3,
		JPEG_MARKER_RST4 = 0xD4,
		JPEG_MARKER_RST5 = 0xD5,
		JPEG_MARKER_RST6 = 0xD6,
		JPEG_MARKER_RST7 = 0xD7,
		// SOI: Start of Image
		JPEG_MARKER_SOI = 0xD8,
		// EOI: End of Image
		JPEG_MARKER_EOI = 0xD9,
		// SOS: Start of Scan
		JPEG_MARKER_SOS = 0xDA,
		// DQT: Define Quantization Table
		JPEG_MARKER_DQT = 0xDB,
		// DNL
		JPEG_MARKER_DNL = 0xDC,
		// DRI: Define Restart Interoperability
		JPEG_MARKER_DRI = 0xDD,
		// DHP
		JPEG_MARKER_DHP = 0xDE,
		// EXP
		JPEG_MARKER_EXP = 0xDF,
		// APP
		JPEG_MARKER_APP0 = 0xE0,
		JPEG_MARKER_APP1 = 0xE1,
		JPEG_MARKER_APP2 = 0xE2,
		JPEG_MARKER_APP3 = 0xE3,
		JPEG_MARKER_APP4 = 0xE4,
		JPEG_MARKER_APP5 = 0xE5,
		JPEG_MARKER_APP6 = 0xE6,
		JPEG_MARKER_APP7 = 0xE7,
		JPEG_MARKER_APP8 = 0xE8,
		JPEG_MARKER_APP9 = 0xE9,
		JPEG_MARKER_APP10 = 0xEA,
		JPEG_MARKER_APP11 = 0xEB,
		JPEG_MARKER_APP12 = 0xEC,
		JPEG_MARKER_APP13 = 0xED,
		JPEG_MARKER_APP14 = 0xEE,
		JPEG_MARKER_APP15 = 0xEF,
		// JPG0
		JPEG_MARKER_JPG0 = 0xF0,
		// JPG8: LSE
		JPEG_MARKER_JPG8 = 0xF8,
		// JPG13
		JPEG_MARKER_JPG13 = 0xFD,
		// COM: Comment
		JPEG_MARKER_COM = 0xFE,
	};

	struct jpeg_marker_base_s
	{
		unsigned char ubFF;
		jpeg_marker_e emMarker : 8;
		unsigned short uwLength;

		jpeg_marker_base_s(jpeg_marker_e const em, unsigned short uw)
			: ubFF(0xFF)
			, emMarker(em)
			, uwLength(uw)
		{}
	};

	template <jpeg_marker_e emMarker, int... n0>
	struct jpeg_marker_s;

#pragma pack(push,1)
	// SOS
	template <int n0> struct jpeg_marker_s<JPEG_MARKER_SOS, n0>
		: jpeg_marker_base_s
	{
		// number of components
		unsigned char ubN;

		struct component_s
		{
			// component id
			unsigned char ubID;
			// AC table no.
			unsigned char ubAC : 4;
			// DC table no.
			unsigned char ubDC : 4;
		} astComponent[n0];

		unsigned char ubSs;
		unsigned char ubSe;
		unsigned char ubAl : 4;
		unsigned char ubAh : 4;

		jpeg_marker_s()
			: jpeg_marker_base_s(JPEG_MARKER_SOS, sizeof(jpeg_marker_s) - sizeof(jpeg_marker_base_s))
			, ubN(n0)
		{}
	};

	// DAC
	template <int n0, int n1> struct jpeg_marker_s<JPEG_MARKER_DAC, n0, n1>
		: jpeg_marker_base_s
	{
		struct arithmetic_dc_s
		{
			unsigned char ubIndex;
			unsigned char ubL : 4;
			unsigned char ubU : 4;
		} astDC[n0];
		struct arithmetic_ac_s
		{
			unsigned char ubIndex;
			unsigned char ubK;
		} astAC[n1];

		jpeg_marker_s()
			: jpeg_marker_base_s(JPEG_MARKER_DAC, sizeof(jpeg_marker_s) - sizeof(jpeg_marker_base_s))
		{}
	};
	template <int nN>
	struct jpeg_huffman_table_s
	{
		unsigned char ubIndex;
		unsigned char aubBits[16];
		unsigned char aubValue[nN];
	};

	// DHT
	template <int nM, int... nN> struct jpeg_marker_s<JPEG_MARKER_DHT, nM, nN...>
		: jpeg_marker_s<JPEG_MARKER_DHT, nN...>
	{
		jpeg_huffman_table_s<nM> stHuffmanTable;

		jpeg_marker_s()
			: jpeg_marker_s<JPEG_MARKER_DHT, nN...>(sizeof(jpeg_marker_s) - sizeof(jpeg_marker_base_s))
		{}
		jpeg_marker_s(unsigned short uw)
			: jpeg_marker_s<JPEG_MARKER_DHT, nN...>(uw)
		{}
	};
	template <> struct jpeg_marker_s<JPEG_MARKER_DHT>
		: jpeg_marker_base_s
	{
		jpeg_marker_s(unsigned short uw)
			: jpeg_marker_base_s(JPEG_MARKER_DHT, uw)
		{}
	};

	enum jpeg_quantization_type_e
	{
		JPEG_QUANT_BYTE = 0,
		JPEG_QUANT_WORD = 1,
	};
	template <jpeg_quantization_type_e emType>
	struct jpeg_quantization_traits;
	template <> struct jpeg_quantization_traits<JPEG_QUANT_BYTE>
	{
		typedef unsigned char type;
	};
	template <> struct jpeg_quantization_traits<JPEG_QUANT_WORD>
	{
		typedef unsigned short type;
	};
	template <jpeg_quantization_type_e emType>
	using jpeg_quantization_t = typename jpeg_quantization_traits<emType>::type;

	template <typename T, int nN>
	struct jpeg_quantization_table_s
	{
		unsigned char ubIndex : 4;
		unsigned char ubPrec : 4;

		union
		{
			T atValue[nN * nN];
			T aatValue[nN][nN];
		};

		jpeg_quantization_table_s()
			: ubPrec(sizeof(T) - 1)
		{}
	};

	// DQT
	template <int nL, int nM, int... nN> struct jpeg_marker_s<JPEG_MARKER_DQT, nL, nM, nN...>
		: jpeg_marker_s<JPEG_MARKER_DQT, nN...>
	{
		jpeg_quantization_table_s<jpeg_quantization_t<(jpeg_quantization_type_e)nL>, nM> stQuantizationTable;

		jpeg_marker_s()
			: jpeg_marker_s<JPEG_MARKER_DQT, nN...>(sizeof(jpeg_marker_s) - sizeof(jpeg_marker_base_s))
		{}
		jpeg_marker_s(unsigned short uw)
			: jpeg_marker_s<JPEG_MARKER_DQT, nN...>(uw)
		{}
	};
	template <> struct jpeg_marker_s<JPEG_MARKER_DQT>
		: jpeg_marker_base_s
	{
		jpeg_marker_s(unsigned short uw)
			: jpeg_marker_base_s(JPEG_MARKER_DQT, uw)
		{}
	};
#pragma pack(pop)
}
