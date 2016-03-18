#pragma once

/* Author: jrmwng @ 2016 */

// http://partners.adobe.com/public/developer/en/tiff/TIFF6.pdf

#include <type_traits>
#include <algorithm>

namespace jrmwng
{
	typedef unsigned long tiff_byte_offset_t;

	enum tiff_tag_e : unsigned short
	{
		//NewSubfileType 254 FE LONG 1
		TIFF_TAG_NEW_SUBFILE_TYPE = 0x00FE,
		//SubfileType 255 FF SHORT 1
		TIFF_TAG_SUBFILE_TYPE = 0x00FF,
		//ImageWidth 256 100 SHORT or LONG 1
		TIFF_TAG_IMAGE_WIDTH = 0x0100,
		//ImageLength 257 101 SHORT or LONG 1
		TIFF_TAG_IMAGE_HEIGHT = 0x0101,
		//BitsPerSample 258 102 SHORT SamplesPerPixel
		TIFF_TAG_BITS_PER_SAMPLE = 0x0102,
		//Compression 259 103 SHORT 1
		//Uncompressed 1
		//CCITT 1D 2
		//Group 3 Fax 3
		//Group 4 Fax 4
		//LZW 5
		//JPEG 6
		//PackBits 32773
		TIFF_TAG_COMPRESSION = 0x0103,
		//PhotometricInterpretation 262 106 SHORT 1
		//WhiteIsZero 0
		//BlackIsZero 1
		//RGB 2
		//RGB Palette 3
		//Transparency mask 4
		//CMYK 5
		//YCbCr 6
		//CIELab 8
		TIFF_TAG_PHOTOMETRIC_INTERPRETATION = 0x0106,
		//Threshholding 263 107 SHORT 1
		TIFF_TAG_THRESHOLDING = 0x0107,
		//CellWidth 264 108 SHORT 1
		TIFF_TAG_CELL_WIDTH = 0x0108,
		//CellLength 265 109 SHORT 1
		TIFF_TAG_CELL_LENGTH = 0x0109,
		//FillOrder 266 10A SHORT 1
		TIFF_TAG_FILL_ORDER = 0x010A,
		//DocumentName 269 10D ASCII
		TIFF_TAG_DOCUMENT_NAME = 0x010D,
		//ImageDescription 270 10E ASCII
		TIFF_TAG_IMAGE_DESCRIPTION = 0x010E,
		//Make 271 10F ASCII
		TIFF_TAG_MAKE = 0x010F,
		//Model 272 110 ASCII
		TIFF_TAG_MODEL = 0x0110,
		//StripOffsets 273 111 SHORT or LONG StripsPerImage
		TIFF_TAG_STRIP_OFFSETS = 0x0111,
		//Orientation 274 112 SHORT 1
		TIFF_TAG_ORIENTATION = 0x0112,
		//SamplesPerPixel 277 115 SHORT 1
		TIFF_TAG_SAMPLES_PER_PIXEL = 0x0115,
		//RowsPerStrip 278 116 SHORT or LONG 1
		TIFF_TAG_ROWS_PER_STRIP = 0x0116,
		//StripByteCounts 279 117 LONG or SHORT StripsPerImage
		TIFF_TAG_STRIP_BYTE_COUNTS = 0x0117,
		//MinSampleValue 280 118 SHORT SamplesPerPixel
		TIFF_TAG_MIN_SAMPLE_VALUE = 0x0118,
		//MaxSampleValue 281 119 SHORT SamplesPerPixel
		TIFF_TAG_MAX_SAMPLE_VALUE = 0x0119,
		//XResolution 282 11A RATIONAL 1
		TIFF_TAG_X_RESOLUTION = 0x011A,
		//YResolution 283 11B RATIONAL 1
		TIFF_TAG_Y_RESOLUTION = 0x011B,
		//PlanarConfiguration 284 11C SHORT 1
		TIFF_TAG_PLANAR_CONFIGURATION = 0x011C,
		//PageName 285 11D ASCII
		TIFF_TAG_PAGE_NAME = 0x011D,
		//XPosition 286 11E RATIONAL
		TIFF_TAG_X_POSITION = 0x011E,
		//YPosition 287 11F RATIONAL
		TIFF_TAG_Y_POSITION = 0x011F,
		//FreeOffsets 288 120 LONG
		TIFF_TAG_FREE_OFFSETS = 0x0120,
		//FreeByteCounts 289 121 LONG
		TIFF_TAG_FREE_BYTE_COUNTS = 0x0121,
		//GrayResponseUnit 290 122 SHORT 1
		TIFF_TAG_GRAY_RESPONSE_UNIT = 0x0122,
		//GrayResponseCurve 291 123 SHORT 2 * *BitsPerSample
		TIFF_TAG_GRAY_RESPONSE_CURVE = 0x0123,
		//T4Options 292 124 LONG 1
		TIFF_TAG_T4_OPTIONS = 0x0124,
		//T6Options 293 125 LONG 1
		TIFF_TAG_T6_OPTIONS = 0x0125,
		//ResolutionUnit 296 128 SHORT 1
		TIFF_TAG_RESOLUTION_UNIT = 0x0128,
		//PageNumber 297 129 SHORT 2
		TIFF_TAG_PAGE_NUMBER = 0x0129,
		//TransferFunction 301 12D SHORT{ 1 or SamplesPerPixel }* 2 * * BitsPerSample
		TIFF_TAG_TRANSFER_FUNCTION = 0x012D,
		//Software 305 131 ASCII
		TIFF_TAG_SOFTWARE = 0x0131,
		//DateTime 306 132 ASCII 20
		TIFF_TAG_DATE_TIME = 0x0132,
		//Artist 315 13B ASCII
		TIFF_TAG_ARTIST = 0x013B,
		//HostComputer 316 13C ASCII
		TIFF_TAG_HOST_COMPUTER = 0x013C,
		//Predictor 317 13D SHORT 1
		TIFF_TAG_PREDICTOR = 0x013D,
		//WhitePoint 318 13E RATIONAL 2
		TIFF_TAG_WHITE_POINT = 0x013E,
		//PrimaryChromaticities 319 13F RATIONAL 6
		TIFF_TAG_PRIMARY_CHROMATICITIES = 0x013F,
		//ColorMap 320 140 SHORT 3 * (2 * *BitsPerSample)
		TIFF_TAG_COLOR_MAP = 0x0140,
		//HalftoneHints 321 141 SHORT 2
		TIFF_TAG_HALFTONE_HINTS = 0x0141,
		//TileWidth 322 142 SHORT or LONG 1
		TIFF_TAG_TILE_WIDTH = 0x0142,
		//TileLength 323 143 SHORT or LONG 1
		TIFF_TAG_TILE_LENGTH = 0x0143,
		//TileOffsets 324 144 LONG TilesPerImage
		TIFF_TAG_TILE_OFFSETS = 0x0144,
		//TileByteCounts 325 145 SHORT or LONG TilesPerImage
		TIFF_TAG_TILE_BYTE_COUNTS = 0x0145,
		//InkSet 332 14C SHORT 1
		TIFF_TAG_INK_SET = 0x014C,
		//InkNames 333 14D ASCII total number of characters in all ink name strings, including zeros
		TIFF_TAG_INK_NAMES = 0x014D,
		//NumberOfInks 334 14E SHORT 1
		TIFF_TAG_NUMBER_OF_INKS = 0x014E,
		//DotRange 336 150 BYTE or SHORT 2, or 2 * NumberOfInks
		TIFF_TAG_DOT_RANGE = 0x0150,
		//TargetPrinter 337 151 ASCII any
		TIFF_TAG_TARGET_PRINTER = 0x0151,
		//ExtraSamples 338 152 BYTE number of extra components per pixel
		TIFF_TAG_EXTRA_SAMPLES = 0x0152,
		//SampleFormat 339 153 SHORT SamplesPerPixel
		TIFF_TAG_SAMPLE_FORMAT = 0x0153,
		//SMinSampleValue 340 154 Any SamplesPerPixel
		TIFF_TAG_S_MIN_SAMPLE_VALUE = 0x0154,
		//SMaxSampleValue 341 155 Any SamplesPerPixel
		TIFF_TAG_S_MAX_SAMPLE_VALUE = 0x0155,
		//TransferRange 342 156 SHORT 6
		TIFF_TAG_TRANSFER_RANGE = 0x0156,
		//JPEGProc 512 200 SHORT 1
		TIFF_TAG_JPEG_PROC = 0x0200,
		//JPEGInterchangeFormat 513 201 LONG 1
		TIFF_TAG_JPEG_INTERCHANGE_FORMAT = 0x0201,
		//JPEGInterchangeFormatLngth 514 202 LONG 1
		TIFF_TAG_JPEG_INTERCHANGE_FORMAT_LENGTH = 0x0202,
		//JPEGRestartInterval 515 203 SHORT 1
		TIFF_TAG_JPEG_RESTART_INTERVAL = 0x0203,
		//JPEGLosslessPredictors 517 205 SHORT SamplesPerPixel
		TIFF_TAG_JPEG_LOSSLESS_PREDICTORS = 0x0205,
		//JPEGPointTransforms 518 206 SHORT SamplesPerPixel
		TIFF_TAG_JPEG_POINT_TRANSFORMS = 0x0206,
		//JPEGQTables 519 207 LONG SamplesPerPixel
		TIFF_TAG_JPEG_Q_TABLES = 0x0207,
		//JPEGDCTables 520 208 LONG SamplesPerPixel
		TIFF_TAG_JPEG_DC_TABLES = 0x0208,
		//JPEGACTables 521 209 LONG SamplesPerPixel
		TIFF_TAG_JPEG_AC_TABLES = 0x0209,
		//YCbCrCoefficients 529 211 RATIONAL 3
		TIFF_TAG_Y_CB_CR_COEFFICIENTS = 0x0211,
		//YCbCrSubSampling 530 212 SHORT 2
		TIFF_TAG_Y_CB_CR_SUB_SAMPLING = 0x0212,
		//YCbCrPositioning 531 213 SHORT 1
		TIFF_TAG_Y_CB_CR_POSITIONING = 0x0213,
		//ReferenceBlackWhite 532 214 LONG 2 * SamplesPerPixel
		TIFF_TAG_REFERENCE_BLACK_WHITE = 0x0214,
		//Copyright 33432 8298 ASCII Any
		TIFF_TAG_COPYRIGHT = 0x8298,
	};

	enum tiff_type_e : unsigned short
	{
		//1 = BYTE 8 - bit unsigned integer.
		TIFF_TYPE_BYTE = 1,
		//2 = ASCII 8 - bit byte that contains a 7 - bit ASCII code; the last byte must be NUL(binary zero).
		TIFF_TYPE_ASCII = 2,
		//3 = SHORT 16 - bit(2 - byte) unsigned integer.
		TIFF_TYPE_SHORT = 3,
		//4 = LONG 32 - bit(4 - byte) unsigned integer.
		TIFF_TYPE_LONG = 4,
		//5 = RATIONAL Two LONGs : the first represents the numerator of a fraction; the second, the denominator.
		TIFF_TYPE_RATIONAL = 5,
		//6 = SBYTE An 8 - bit signed(twos - complement) integer.
		TIFF_TYPE_SBYTE = 6,
		//7 = UNDEFINED An 8 - bit byte that may contain anything, depending on the definition of the field.
		TIFF_TYPE_UNDEFINED = 7,
		//8 = SSHORT A 16 - bit(2 - byte) signed(twos - complement) integer.
		TIFF_TYPE_SSHORT = 8,
		//9 = SLONG A 32 - bit(4 - byte) signed(twos - complement) integer.
		TIFF_TYPE_SLONG = 9,
		//10 = SRATIONAL Two SLONG’s : the first represents the numerator of a fraction, the second the denominator.
		TIFF_TYPE_SRATIONAL = 10,
		//11 = FLOAT Single precision(4 - byte) IEEE format.
		TIFF_TYPE_FLOAT = 11,
		//12 = DOUBLE Double precision(8 - byte) IEEE format.
		TIFF_TYPE_DOUBLE = 12,
	};

	//1 = BYTE 8 - bit unsigned integer.
	typedef unsigned char tiff_byte_t;
	//2 = ASCII 8 - bit byte that contains a 7 - bit ASCII code; the last byte must be NUL(binary zero).
	typedef char tiff_ascii_t;
	//3 = SHORT 16 - bit(2 - byte) unsigned integer.
	typedef unsigned short tiff_short_t;
	//4 = LONG 32 - bit(4 - byte) unsigned integer.
	typedef unsigned long tiff_long_t;
	//5 = RATIONAL Two LONGs : the first represents the numerator of a fraction; the second, the denominator.
	struct tiff_rational_t
	{
		tiff_long_t ulNumerator;
		tiff_long_t ulDenominator;
	};
	//6 = SBYTE An 8 - bit signed(twos - complement) integer.
	typedef signed char tiff_sbyte_t;
	//7 = UNDEFINED An 8 - bit byte that may contain anything, depending on the definition of the field.
	typedef unsigned char tiff_undefined_t;
	//8 = SSHORT A 16 - bit(2 - byte) signed(twos - complement) integer.
	typedef signed short tiff_sshort_t;
	//9 = SLONG A 32 - bit(4 - byte) signed(twos - complement) integer.
	typedef signed long tiff_slong_t;
	//10 = SRATIONAL Two SLONG’s : the first represents the numerator of a fraction, the second the denominator.
	struct tiff_srational_t
	{
		tiff_slong_t lNumerator;
		tiff_slong_t lDenominator;
	};
	//11 = FLOAT Single precision(4 - byte) IEEE format.
	typedef float tiff_float_t;
	//12 = DOUBLE Double precision(8 - byte) IEEE format.
	typedef double tiff_double_t;

	template <tiff_tag_e... emTAG>
	struct tiff_field;

	template <tiff_tag_e emTAG>
	struct tiff_field<emTAG>
	{
		tiff_tag_e const emTag;
		tiff_type_e emType;
		tiff_long_t ulCount;
		union
		{
			tiff_byte_offset_t ulByteOffset;
			//1 = BYTE 8 - bit unsigned integer.
			tiff_byte_t aubValue[4];
			//2 = ASCII 8 - bit byte that contains a 7 - bit ASCII code; the last byte must be NUL(binary zero).
			tiff_ascii_t acValue[4];
			//3 = SHORT 16 - bit(2 - byte) unsigned integer.
			tiff_short_t auwValue[2];
			//4 = LONG 32 - bit(4 - byte) unsigned integer.
			tiff_long_t aulValue[1];
			//5 = RATIONAL Two LONGs : the first represents the numerator of a fraction; the second, the denominator.
			//6 = SBYTE An 8 - bit signed(twos - complement) integer.
			tiff_sbyte_t abValue[4];
			//7 = UNDEFINED An 8 - bit byte that may contain anything, depending on the definition of the field.
			tiff_undefined_t aValue[4];
			//8 = SSHORT A 16 - bit(2 - byte) signed(twos - complement) integer.
			tiff_sshort_t awValue[2];
			//9 = SLONG A 32 - bit(4 - byte) signed(twos - complement) integer.
			tiff_slong_t alValue[1];
			//10 = SRATIONAL Two SLONG’s : the first represents the numerator of a fraction, the second the denominator.
			//11 = FLOAT Single precision(4 - byte) IEEE format.
			tiff_float_t arValue[1];
			//12 = DOUBLE Double precision(8 - byte) IEEE format.
		};
		template <tiff_long_t ulCOUNT>
		tiff_field(tiff_byte_t const (&aubVALUE)[ulCOUNT])
			: emTag(emTAG)
			, emType(TIFF_TYPE_BYTE)
			, ulCount(ulCOUNT)
		{
			std::copy(std::cbegin(aubVALUE), std::cend(aubVALUE), aubVALUE);
		}
	};

	//NewSubfileType 254 FE LONG 1
	template <> struct tiff_field<TIFF_TAG_NEW_SUBFILE_TYPE>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t const ulCount;
		union
		{
			tiff_long_t ulValue;
			struct
			{
				tiff_long_t ulReducedResolution : 1;
				tiff_long_t ulSinglePage : 1;
				tiff_long_t ulTransparencyMask : 1;
				tiff_long_t: 29;
			};
		};

		tiff_field()
			: emTag(TIFF_TAG_NEW_SUBFILE_TYPE)
			, emType(TIFF_TYPE_LONG)
			, ulCount(1)
			, ulValue(0)
		{}
	};
	//SubfileType 255 FF SHORT 1
	//ImageWidth 256 100 SHORT or LONG 1
	template <> struct tiff_field<TIFF_TAG_IMAGE_WIDTH>
	{
		tiff_tag_e const emTag;
		tiff_type_e emType;
		tiff_long_t const ulCount;
		tiff_long_t ulValue;
		tiff_field()
			: emTag(TIFF_TAG_IMAGE_WIDTH)
			, emType(TIFF_TYPE_SHORT)
			, ulCount(1)
			, ulValue(0)
		{}
		tiff_field & operator = (tiff_long_t ulNewValue)
		{
			emType = (ulNewValue > 0xFFFF) ? TIFF_TYPE_LONG : TIFF_TYPE_SHORT;
			ulValue = ulNewValue;
			return *this;
		}
	};
	//ImageLength 257 101 SHORT or LONG 1
	template <> struct tiff_field<TIFF_TAG_IMAGE_HEIGHT>
	{
		tiff_tag_e const emTag;
		tiff_type_e emType;
		tiff_long_t const ulCount;
		tiff_long_t ulValue;
		tiff_field()
			: emTag(TIFF_TAG_IMAGE_HEIGHT)
			, emType(TIFF_TYPE_SHORT)
			, ulCount(1)
			, ulValue(0)
		{}
		tiff_field & operator = (tiff_long_t ulNewValue)
		{
			emType = (ulNewValue > 0xFFFF) ? TIFF_TYPE_LONG : TIFF_TYPE_SHORT;
			ulValue = ulNewValue;
			return *this;
		}
	};
	//BitsPerSample 258 102 SHORT SamplesPerPixel
	template <> struct tiff_field<TIFF_TAG_BITS_PER_SAMPLE>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t ulCount;
		union
		{
			tiff_byte_offset_t ulByteOffset;
			tiff_short_t auwValue[2];
		};
		tiff_field()
			: emTag(TIFF_TAG_BITS_PER_SAMPLE)
			, emType(TIFF_TYPE_SHORT)
			, ulCount(0)
			, ulByteOffset(0)
		{}
	};
	enum tiff_compression_e : tiff_short_t
	{
		TIFF_COMPRESSION_UNCOMPRESSED = 1,
		TIFF_COMPRESSION_CCITT_1D = 2,
		TIFF_COMPRESSION_GROUP_3_FAX = 3,
		TIFF_COMPRESSION_GROUP_4_FAX = 4,
		TIFF_COMPRESSION_LZW = 5,
		TIFF_COMPRESSION_JPEG = 6,
		TIFF_COMPRESSION_PACK_BITS = 32773,
	};
	//Compression 259 103 SHORT 1
	//Uncompressed 1
	//CCITT 1D 2
	//Group 3 Fax 3
	//Group 4 Fax 4
	//LZW 5
	//JPEG 6
	//PackBits 32773
	template <> struct tiff_field<TIFF_TAG_COMPRESSION>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t const ulCount;
		union
		{
			tiff_short_t uwValue;
			tiff_compression_e emCompression;
		};
		tiff_field()
			: emTag(TIFF_TAG_COMPRESSION)
			, emType(TIFF_TYPE_SHORT)
			, ulCount(1)
			, emCompression(TIFF_COMPRESSION_UNCOMPRESSED)
		{}
	};
	enum tiff_interpretation_e : tiff_short_t
	{
		TIFF_INTERPRETATION_WHITE_IS_ZERO = 0,
		TIFF_INTERPRETATION_BLACK_IS_ZERO = 1,
		TIFF_INTERPRETATION_RGB = 2,
		TIFF_INTERPRETATION_RGB_PALETTE = 3,
		TIFF_INTERPRETATION_TRANSPARENCY_MASK = 4,
		TIFF_INTERPRETATION_CMYK = 5,
		TIFF_INTERPRETATION_Y_CB_CR = 6,
		TIFF_INTERPRETATION_CIE_LAB = 8,
	};
	//PhotometricInterpretation 262 106 SHORT 1
	//WhiteIsZero 0
	//BlackIsZero 1
	//RGB 2
	//RGB Palette 3
	//Transparency mask 4
	//CMYK 5
	//YCbCr 6
	//CIELab 8
	template <> struct tiff_field<TIFF_TAG_PHOTOMETRIC_INTERPRETATION>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t const ulCount;
		tiff_interpretation_e emInterpretation;
		tiff_field()
			: emTag(TIFF_TAG_PHOTOMETRIC_INTERPRETATION)
			, emType(TIFF_TYPE_SHORT)
			, ulCount(1)
			, emInterpretation(TIFF_INTERPRETATION_WHITE_IS_ZERO)
		{}
		tiff_field & operator = (tiff_interpretation_e emNewValue)
		{
			emInterpretation = emNewValue;
			return *this;
		}
	};
	//Threshholding 263 107 SHORT 1
	//CellWidth 264 108 SHORT 1
	//CellLength 265 109 SHORT 1
	//FillOrder 266 10A SHORT 1
	//DocumentName 269 10D ASCII
	//ImageDescription 270 10E ASCII
	//Make 271 10F ASCII
	//Model 272 110 ASCII
	//StripOffsets 273 111 SHORT or LONG StripsPerImage
	template <> struct tiff_field<TIFF_TAG_STRIP_OFFSETS>
	{
		tiff_tag_e const emTag;
		tiff_type_e emType;
		tiff_long_t ulCount;
		union
		{
			tiff_short_t auwValue[2];
			tiff_long_t ulValue;
			tiff_byte_offset_t ulByteOffset;
		};
		tiff_field()
			: emTag(TIFF_TAG_STRIP_OFFSETS)
			, emType(TIFF_TYPE_SHORT)
			, ulCount(0)
			, ulByteOffset(0)
		{}
		tiff_field & operator = (tiff_long_t ulNewValue)
		{
			emType = ulNewValue <= std::numeric_limits<tiff_short_t>::max() ? TIFF_TYPE_SHORT : TIFF_TYPE_LONG;
			ulCount = 1;
			ulValue = ulNewValue;
			return *this;
		}
	};
	//Orientation 274 112 SHORT 1
	//SamplesPerPixel 277 115 SHORT 1
	//RowsPerStrip 278 116 SHORT or LONG 1
	template <> struct tiff_field<TIFF_TAG_ROWS_PER_STRIP>
	{
		tiff_tag_e const emTag;
		tiff_type_e emType;
		tiff_long_t const ulCount;
		tiff_long_t ulValue;
		tiff_field()
			: emTag(TIFF_TAG_ROWS_PER_STRIP)
			, emType(TIFF_TYPE_SHORT)
			, ulCount(1)
			, ulValue(0)
		{}
		tiff_field & operator = (tiff_long_t ulNewValue)
		{
			emType = (ulNewValue > 0xFFFF) ? TIFF_TYPE_LONG : TIFF_TYPE_SHORT;
			ulValue = ulNewValue;
			return *this;
		}
	};
	//StripByteCounts 279 117 LONG or SHORT StripsPerImage
	//MinSampleValue 280 118 SHORT SamplesPerPixel
	//MaxSampleValue 281 119 SHORT SamplesPerPixel
	//XResolution 282 11A RATIONAL 1
	template <> struct tiff_field<TIFF_TAG_X_RESOLUTION>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t const ulCount;
		tiff_byte_offset_t ulByteOffset;
		tiff_field()
			: emTag(TIFF_TAG_X_RESOLUTION)
			, emType(TIFF_TYPE_RATIONAL)
			, ulCount(1)
			, ulByteOffset(0)
		{}
	};
	//YResolution 283 11B RATIONAL 1
	template <> struct tiff_field<TIFF_TAG_Y_RESOLUTION>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t const ulCount;
		tiff_byte_offset_t ulByteOffset;

		tiff_field()
			: emTag(TIFF_TAG_Y_RESOLUTION)
			, emType(TIFF_TYPE_RATIONAL)
			, ulCount(1)
			, ulByteOffset(0)
		{}
	};
	//PlanarConfiguration 284 11C SHORT 1
	//PageName 285 11D ASCII
	//XPosition 286 11E RATIONAL
	//YPosition 287 11F RATIONAL
	//FreeOffsets 288 120 LONG
	template <> struct tiff_field<TIFF_TAG_FREE_OFFSETS>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t const ulCount;
		tiff_long_t ulValue;
		tiff_field()
			: emTag(TIFF_TAG_FREE_OFFSETS)
			, emType(TIFF_TYPE_LONG)
			, ulCount(1)
			, ulValue(0)
		{}
		tiff_field & operator = (tiff_long_t ulNewValue)
		{
			ulValue = ulNewValue;
			return *this;
		}
	};
	//FreeByteCounts 289 121 LONG
	template <> struct tiff_field<TIFF_TAG_FREE_BYTE_COUNTS>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t const ulCount;
		tiff_long_t ulValue;
		tiff_field()
			: emTag(TIFF_TAG_FREE_BYTE_COUNTS)
			, emType(TIFF_TYPE_LONG)
			, ulCount(1)
			, ulValue(0)
		{}
		tiff_field & operator = (tiff_long_t ulNewValue)
		{
			ulValue = ulNewValue;
			return *this;
		}
	};
	//GrayResponseUnit 290 122 SHORT 1
	//GrayResponseCurve 291 123 SHORT 2 * *BitsPerSample
	//T4Options 292 124 LONG 1
	//T6Options 293 125 LONG 1
	//ResolutionUnit 296 128 SHORT 1
	enum tiff_resolution_unit_e : tiff_short_t
	{
		TIFF_RESOLUTION_UNIT_NONE = 1,
		TIFF_RESOLUTION_UNIT_INCH = 2,
		TIFF_RESOLUTION_UNIT_CENTIMETER = 3,
	};
	template <> struct tiff_field<TIFF_TAG_RESOLUTION_UNIT>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t const ulCount;
		tiff_resolution_unit_e emValue;
		tiff_field()
			: emTag(TIFF_TAG_RESOLUTION_UNIT)
			, emType(TIFF_TYPE_SHORT)
			, ulCount(1)
			, emValue(TIFF_RESOLUTION_UNIT_INCH)
		{}
		tiff_field & operator = (tiff_resolution_unit_e emNewValue)
		{
			emValue = emNewValue;
			return *this;
		}
	};
	//PageNumber 297 129 SHORT 2
	//TransferFunction 301 12D SHORT{ 1 or SamplesPerPixel }* 2 * * BitsPerSample
	//Software 305 131 ASCII
	//DateTime 306 132 ASCII 20
	//Artist 315 13B ASCII
	//HostComputer 316 13C ASCII
	//Predictor 317 13D SHORT 1
	//WhitePoint 318 13E RATIONAL 2
	//PrimaryChromaticities 319 13F RATIONAL 6
	//ColorMap 320 140 SHORT 3 * (2 * *BitsPerSample)
	//HalftoneHints 321 141 SHORT 2
	//TileWidth 322 142 SHORT or LONG 1
	//TileLength 323 143 SHORT or LONG 1
	//TileOffsets 324 144 LONG TilesPerImage
	//TileByteCounts 325 145 SHORT or LONG TilesPerImage
	//InkSet 332 14C SHORT 1
	//InkNames 333 14D ASCII total number of characters in all ink name strings, including zeros
	//NumberOfInks 334 14E SHORT 1
	//DotRange 336 150 BYTE or SHORT 2, or 2 * NumberOfInks
	//TargetPrinter 337 151 ASCII any
	//ExtraSamples 338 152 BYTE number of extra components per pixel
	//SampleFormat 339 153 SHORT SamplesPerPixel
	//SMinSampleValue 340 154 Any SamplesPerPixel
	//SMaxSampleValue 341 155 Any SamplesPerPixel
	//TransferRange 342 156 SHORT 6
	//JPEGProc 512 200 SHORT 1
	//JPEGInterchangeFormat 513 201 LONG 1
	//JPEGInterchangeFormatLngth 514 202 LONG 1
	//JPEGRestartInterval 515 203 SHORT 1
	//JPEGLosslessPredictors 517 205 SHORT SamplesPerPixel
	//JPEGPointTransforms 518 206 SHORT SamplesPerPixel
	//JPEGQTables 519 207 LONG SamplesPerPixel
	//JPEGDCTables 520 208 LONG SamplesPerPixel
	//JPEGACTables 521 209 LONG SamplesPerPixel
	//YCbCrCoefficients 529 211 RATIONAL 3
	//YCbCrSubSampling 530 212 SHORT 2
	//YCbCrPositioning 531 213 SHORT 1
	//ReferenceBlackWhite 532 214 LONG 2 * SamplesPerPixel
	//Copyright 33432 8298 ASCII Any

	template <tiff_tag_e emTAG, tiff_tag_e... emOTHERS>
	struct tiff_field<emTAG, emOTHERS...>
		: tiff_field<emTAG>
		, tiff_field<emOTHERS...>
	{};

	template <typename... TFields>
	struct tiff_subfile;
	template <typename TFields>
	struct tiff_subfile<TFields>
	{
		unsigned short uwNumOfDirectoryEntries;
		TFields stFields;
		tiff_byte_offset_t ulNextSubfile;

		template <tiff_tag_e emTag, typename T>
		void set(T t)
		{
			static_cast<tiff_field<emTag>&>(stFields) = t;
		}
	};
	template <typename TFields, typename... TOthers>
	struct tiff_subfile<TFields, TOthers...>
		: tiff_subfile<TFields>
		, tiff_subfile<TOthers...>
	{};

	struct tiff_file_header
	{
		char acByteOrder[2]; // "II" for little-endian; "MM" for big-endian
		short w42; // 0x0042
		tiff_byte_offset_t ulFirstSubfile;
	};
	
	template <typename... TFields>
	struct tiff_file
		: tiff_file_header
		, tiff_subfile<TFields...>
	{};

	struct tiff_free_fields
		: tiff_field<TIFF_TAG_FREE_OFFSETS, TIFF_TAG_FREE_BYTE_COUNTS>
	{};

	struct tiff_baseline_fields
		: tiff_field<TIFF_TAG_IMAGE_WIDTH, TIFF_TAG_IMAGE_HEIGHT, TIFF_TAG_ROWS_PER_STRIP, TIFF_TAG_STRIP_OFFSETS, TIFF_TAG_X_RESOLUTION, TIFF_TAG_Y_RESOLUTION, TIFF_TAG_RESOLUTION_UNIT>
	{};
	struct tiff_baseline_file
		: tiff_file<tiff_baseline_fields>
	{};
}
