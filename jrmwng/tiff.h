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
		//Exif IFD
		TIFF_TAG_EXIF_IFD = 0x8769,
		//GPS information IFD
		TIFF_TAG_GPS_IFD = 0x8825,
	};
	enum exif_tag_e : unsigned short
	{
		//Interoperability IFD
		EXIF_TAG_INTEROPERABILITY_IFD = 0xA005,

		//A.

		//Exif version
		EXIF_TAG_VERSION = 0x9000,
		//Supported Flashpix version
		EXIF_TAG_FLASHPIX_VERSION = 0xA000,

		//B.

		//Color space information
		EXIF_TAG_COLOR_SPACE = 0xA001,
		//Gamma
		EXIF_TAG_GAMMA = 0xA500,

		//C.

		//Meaning of each component
		EXIF_TAG_COMPONENTS_CONFIGURATION = 0x9101,
		//Image compression mode
		EXIF_TAG_COMPRESSED_BITS_PER_PIXEL = 0x9102,
		//Valid image width
		EXIF_TAG_PIXEL_X_DIMENSION = 0xA002,
		//Valid image height
		EXIF_TAG_PIXEL_Y_DIMENSION = 0xA003,

		//D.

		//Manufacturer notes
		EXIF_TAG_MAKER_NOTE = 0x927C,
		//User comments
		EXIF_TAG_USER_COMMENT = 0x9286,

		//E.

		//Related audio file
		EXIF_TAG_RELATED_SOUND_FILE = 0xA004,

		//F.

		//Date and time of original data generation
		EXIF_TAG_DATE_TIME_ORIGINAL = 0x9003,
		//Date and time of digital data generation
		EXIF_TAG_DATE_TIME_DIGITIZED = 0x9004,
		//DateTime subseconds
		EXIF_TAG_SUB_SEC_TIME = 0x9290,
		//DateTimeOriginal subseconds
		EXIF_TAG_SUB_SEC_TIME_ORIGINAL = 0x9291,
		//DateTimeDigitized subseconds
		EXIF_TAG_SUB_SEC_TIME_DIGITIZED = 0x9292,

		//G.

		//Exposure time
		EXIF_TAG_EXPOSURE_TIME = 0x829A,
		//F number
		EXIF_TAG_F_NUMBER = 0x829D,
		//Exposure program
		EXIF_TAG_EXPOSURE_PROGRAM = 0x8822,
		//Spectral sensitivity
		EXIF_TAG_SPECTRAL_SENSITIVITY = 0x8824,
		//Photographic sensitivity
		EXIF_TAG_PHOTOGRAPHIC_SENSITIVITY = 0x8827,
		//Optoelectric conversion factor
		EXIF_TAG_OECF = 0x8828,
		//Sensitivity type
		EXIF_TAG_SENSITIVITY_TYPE = 0x8830,
		//Standard output sensitivity
		EXIF_TAG_STANDARD_OUTPUT_SENSITIVITY = 0x8831,
		//Recommended ExposureIndex
		EXIF_TAG_RECOMMENDED_EXPOSURE_INDEX = 0x8832,
		//ISO Speed
		EXIF_TAG_ISO_SPEED = 0x8833,
		//ISO Speed Latitude yyy
		EXIF_TAG_ISO_SPEED_LATITUDE_YYY = 0x8834,
		//ISO Speed Latitude zzz
		EXIF_TAG_ISO_SPEED_LATITUDE_ZZZ = 0x8835,
		//Shutter speed
		EXIF_TAG_SHUTTER_SPEED_VALUE = 0x9201,
		//Aperture
		EXIF_TAG_APERTURE_VALUE = 0x9202,
		//Brightness
		EXIF_TAG_BRIGHTNESS = 0x9203,
		//Exposure bias
		EXIF_TAG_EXPOSURE_BIAS = 0x9204,
		//Maximum lens aperture
		EXIF_TAG_MAX_APERTURE_VALUE = 0x9205,
		//Subject distance
		EXIF_TAG_SUBJECT_DISTANCE = 0x9206,
		//Metering mode
		EXIF_TAG_METERING_MODE = 0x9207,
		//Light source
		EXIF_TAG_LIGHT_SOURCE = 0x9208,
		//Flash
		EXIF_TAG_FLASH = 0x9209,
		//Lens focal length
		EXIF_TAG_FOCAL_LENGTH = 0x920A,
		//Subject area
		EXIF_TAG_SUBJECT_AREA = 0x9214,
		//Flash energy
		EXIF_TAG_FLASH_ENERGY = 0xA20B,
		//Spatial frequency response
		EXIF_TAG_SPATIAL_FREQUENCY_RESPONSE = 0xA20C,
		//Focal plane X resolution
		EXIF_TAG_FOCAL_PLANE_X_RESOLUTION = 0xA20E,
		//Focal plane Y resolution
		EXIF_TAG_FOCAL_PLANE_Y_RESOLUTION = 0xA20F,
		//Focal plane resolution unit
		EXIF_TAG_FOCAL_PLANE_RESOLUTION_UNIT = 0xA210,
		//Subject location
		EXIF_TAG_SUBJECT_LOCATION = 0xA214,
		//Exposure index
		EXIF_TAG_EXPOSURE_INDEX = 0xA215,
		//Sensing method
		EXIF_TAG_SENSING_METHOD = 0xA217,
		//File source
		EXIF_TAG_FILE_SOURCE = 0xA300,
		//Scene type
		EXIF_TAG_SCENE_TYPE = 0xA301,
		//CFA pattern
		EXIF_TAG_CFA_PATTERN = 0xA302,
		//Custom image processing
		EXIF_TAG_CUSTOM_RENDERED = 0xA401,
		//Exposure mode
		EXIF_TAG_EXPOSURE_MODE = 0xA402,
		//White balance
		EXIF_TAG_WHITE_BALANCE = 0xA403,
		//Digital zoom ratio
		EXIF_TAG_DIGITAL_ZOOM_RATIO = 0xA404,
		//Focal length in 35mm film
		EXIF_TAG_FOCAL_LENGTH_IN_35MM_FILM = 0xA405,
		//Scene capture type
		EXIF_TAG_SCENE_CAPTURE_TYPE = 0xA406,
		//Gain control
		EXIF_TAG_GAIN_CONTROL = 0xA407,
		//Contrast
		EXIF_TAG_CONTRAST = 0xA408,
		//Saturation
		EXIF_TAG_SATURATION = 0xA409,
		//Sharpness
		EXIF_TAG_SHARPNESS = 0xA40A,
		//Device settings description
		EXIF_TAG_DEVICE_SETTING_DESCRIPTION = 0xA40B,
		//Subject distance range
		EXIF_TAG_SUBJECT_DISTANCE_RANGE = 0xA40C,

		//H.

		//Unique image ID
		EXIF_TAG_IMAGE_UNIQUE_ID = 0xA420,
		//Camera Owner Name
		EXIF_TAG_CAMERA_OWNER_NAME = 0xA430,
		//Body Serial Number
		EXIF_TAG_BODY_SERIAL_NUMBER = 0xA431,
		//Lens Specification
		EXIF_TAG_LENS_SPECIFICATION = 0xA432,
		//Lens Make
		EXIF_TAG_LENS_MAKE = 0xA433,
		//Lens Model
		EXIF_TAG_LENS_MODEL = 0xA434,
		//Lens Serial Number
		EXIF_TAG_LENS_SERIAL_NUMBER = 0xA435,
	};
	enum exif_gps_tag_e : unsigned short
	{
		//GPS tag version
		EXIF_TAG_GPS_VERSION_ID = 0x0,
		//North or South Latitude
		EXIF_TAG_GPS_LATITUDE_REF = 0x01,
		//Latitude
		EXIF_TAG_GPS_LATITUDE = 0x02,
		//East or West Longitude
		EXIF_TAG_GPS_LONGITUDE_REF = 0x03,
		//Longitude
		EXIF_TAG_GPS_LONGITUDE = 0x04,
		//Altitude reference
		EXIF_TAG_GPS_ALTITUDE_REF = 0x05,
		//Altitude
		EXIF_TAG_GPS_ALTITUDE = 0x06,
		//GPS time (atomic clock)
		EXIF_TAG_GPS_TIME_STAMP = 0x07,
		//GPS satellites used for measurement
		EXIF_TAG_GPS_SATELLITES = 0x08,
		//GPS receiver status
		EXIF_TAG_GPS_STATUS = 0x09,
		//GPS measurement mode
		EXIF_TAG_GPS_MEASURE_MODE = 0x0A,
		//Measurement precision
		EXIF_TAG_GPS_DOP = 0x0B,
		//Speed unit
		EXIF_TAG_GPS_SPEED_REF = 0x0C,
		//Speed of GPS receiver
		EXIF_TAG_GPS_SPEED = 0x0D,
		//Reference for direction of movement
		EXIF_TAG_GPS_TRACK_REF = 0x0E,
		//Direction of movement
		EXIF_TAG_GPS_TRACK = 0x0F,
		//Reference for direction of image
		EXIF_TAG_GPS_IMG_DIRECTION_REF = 0x10,
		//Direction of image
		EXIF_TAG_GPS_IMG_DIRECTION = 0x11,
		//Geodetic survey data used
		EXIF_TAG_GPS_MAP_DATUM = 0x12,
		//Reference for latitude of destination
		EXIF_TAG_GPS_DEST_LATITUDE_REF = 0x13,
		//Latitude of destination
		EXIF_TAG_GPS_DEST_LATITUDE = 0x14,
		//Reference for longitude of destination
		EXIF_TAG_GPS_DEST_LONGITUDE_REF = 0x15,
		//Longitude of destination
		EXIF_TAG_GPS_DEST_LONGITUDE = 0x16,
		//Reference for bearing of destination
		EXIF_TAG_GPS_DEST_BEARING_REF = 0x17,
		//Bearing of destination
		EXIF_TAG_GPS_DEST_BEARING = 0x18,
		//Reference for distance to destination
		EXIF_TAG_GPS_DEST_DISTANCE_REF = 0x19,
		//Distance to destination
		EXIF_TAG_GPS_DEST_DISTANCE = 0x1A,
		//Name of GPS processing method
		EXIF_TAG_GPS_PROCESSING_METHOD = 0x1B,
		//Name of GPS area
		EXIF_TAG_GPS_AREA_INFORMATION = 0x1C,
		//GPS date
		EXIF_TAG_GPS_DATE_STAMP = 0x1D,
		//GPS differential correction
		EXIF_TAG_GPS_DIFFERENTIAL = 0x1E,
		//Horizontal positioning error
		EXIF_TAG_GPS_H_POSITIONING_ERROR = 0x1F,
	};
	enum exif_interoperability_tag_e : unsigned short
	{
		//Interoperability Identification
		EXIF_TAG_INTEROPERABILITY_INDEX = 0x01,
		//Interoperability version
		EXIF_TAG_INTEROPERABILITY_VERSION = 0x02,
		//File format of the corresponding DCF extended image file
		EXIF_TAG_RELATED_IMAGE_FILE_FORMAT = 0x1000,
		//Horizontal pixel count of the corresponding DCF extended image file
		EXIF_TAG_RELATED_IMAGE_WIDTH = 0x1001,
		//Vertical pixel count of the corresponding DCF extended image file
		EXIF_TAG_RELATED_IMAGE_HEIGHT = 0x1002,
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

#pragma pack(push,1)
	template <typename tag_e>
	struct tiff_field_base_s
	{
		tag_e const emTag;
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
		tiff_field_base_s(tag_e const _Tag, tiff_type_e _Type, tiff_long_t _Count, tiff_byte_offset_t _ByteOffset = 0)
			: emTag(_Tag)
			, emType(_Type)
			, ulCount(_Count)
			, ulByteOffset(_ByteOffset)
		{
			static_assert(sizeof(*this) == 12, "TIFF field size must be 12");
		}
		tiff_field_base_s(tag_e const _Tag, tiff_type_e _Type, tiff_long_t _Count, tiff_short_t uw0, tiff_short_t uw1)
			: emTag(_Tag)
			, emType(_Type)
			, ulCount(_Count)
		{
			static_assert(sizeof(*this) == 12, "TIFF field size must be 12");

			auwValue[0] = uw0;
			auwValue[1] = uw1;
		}
	};
	typedef tiff_field_base_s<tiff_tag_e> tiff_field_base_t;
	typedef tiff_field_base_s<exif_tag_e> exif_field_base_t;
	typedef tiff_field_base_s<exif_gps_tag_e> exif_gps_field_base_t;
	typedef tiff_field_base_s<exif_interoperability_tag_e> exif_interoperability_field_base_t;

	template <typename tag_e>
	static inline tiff_field_base_s<tag_e> byteswap(tiff_field_base_s<tag_e> const & stField)
	{
		unsigned long const ulCount = _byteswap_ulong(stField.ulCount);

		switch (_byteswap_ushort(stField.emType))
		{
		case TIFF_TYPE_BYTE:
		case TIFF_TYPE_ASCII:
		case TIFF_TYPE_UNDEFINED:
			if (ulCount <= 4)
			{
				return tiff_field_base_s<tag_e>(
					static_cast<tag_e>(_byteswap_ushort(stField.emTag)),
					static_cast<tiff_type_e>(_byteswap_ushort(stField.emType)),
					_byteswap_ulong(stField.ulCount),
					stField.ulByteOffset);
			}
			break;
		case TIFF_TYPE_SHORT:
		case TIFF_TYPE_SSHORT:
			if (ulCount <= 2)
			{
				return tiff_field_base_s<tag_e>(
					static_cast<tag_e>(_byteswap_ushort(stField.emTag)),
					static_cast<tiff_type_e>(_byteswap_ushort(stField.emType)),
					_byteswap_ulong(stField.ulCount),
					_byteswap_ushort(stField.auwValue[0]),
					_byteswap_ushort(stField.auwValue[1]));
			}
			break;
		case TIFF_TYPE_LONG:
		case TIFF_TYPE_SLONG:
		case TIFF_TYPE_FLOAT:
		case TIFF_TYPE_DOUBLE:
		case TIFF_TYPE_RATIONAL:
		case TIFF_TYPE_SRATIONAL:
			break;
		default:
			__debugbreak();
			break;
		}
		return tiff_field_base_s<tag_e>(
			static_cast<tag_e>(_byteswap_ushort(stField.emTag)),
			static_cast<tiff_type_e>(_byteswap_ushort(stField.emType)),
			_byteswap_ulong(stField.ulCount),
			_byteswap_ulong(stField.ulByteOffset));
	}

	template <tiff_tag_e... emTAG>
	struct tiff_field_s;

	template <tiff_tag_e emTAG>
	struct tiff_field_s<emTAG>
		: tiff_field_base_t
	{
		template <tiff_long_t ulCOUNT>
		tiff_field_s(tiff_byte_t const (&aubVALUE)[ulCOUNT])
			: tiff_field_base_t(emTAG, TIFF_TYPE_BYTE, ulCount)
		{
			std::copy(std::cbegin(aubVALUE), std::cend(aubVALUE), aubVALUE);
		}
	};

	//NewSubfileType 254 FE LONG 1
	template <> struct tiff_field_s<TIFF_TAG_NEW_SUBFILE_TYPE>
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

		tiff_field_s()
			: emTag(TIFF_TAG_NEW_SUBFILE_TYPE)
			, emType(TIFF_TYPE_LONG)
			, ulCount(1)
			, ulValue(0)
		{}
	};
	//SubfileType 255 FF SHORT 1
	//ImageWidth 256 100 SHORT or LONG 1
	template <> struct tiff_field_s<TIFF_TAG_IMAGE_WIDTH>
	{
		tiff_tag_e const emTag;
		tiff_type_e emType;
		tiff_long_t const ulCount;
		tiff_long_t ulValue;
		tiff_field_s()
			: emTag(TIFF_TAG_IMAGE_WIDTH)
			, emType(TIFF_TYPE_SHORT)
			, ulCount(1)
			, ulValue(0)
		{}
		tiff_field_s & operator = (tiff_long_t ulNewValue)
		{
			emType = (ulNewValue > 0xFFFF) ? TIFF_TYPE_LONG : TIFF_TYPE_SHORT;
			ulValue = ulNewValue;
			return *this;
		}
	};
	//ImageLength 257 101 SHORT or LONG 1
	template <> struct tiff_field_s<TIFF_TAG_IMAGE_HEIGHT>
	{
		tiff_tag_e const emTag;
		tiff_type_e emType;
		tiff_long_t const ulCount;
		tiff_long_t ulValue;
		tiff_field_s()
			: emTag(TIFF_TAG_IMAGE_HEIGHT)
			, emType(TIFF_TYPE_SHORT)
			, ulCount(1)
			, ulValue(0)
		{}
		tiff_field_s & operator = (tiff_long_t ulNewValue)
		{
			emType = (ulNewValue > 0xFFFF) ? TIFF_TYPE_LONG : TIFF_TYPE_SHORT;
			ulValue = ulNewValue;
			return *this;
		}
	};
	//BitsPerSample 258 102 SHORT SamplesPerPixel
	template <> struct tiff_field_s<TIFF_TAG_BITS_PER_SAMPLE>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t ulCount;
		union
		{
			tiff_byte_offset_t ulByteOffset;
			tiff_short_t auwValue[2];
		};
		tiff_field_s()
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
	template <> struct tiff_field_s<TIFF_TAG_COMPRESSION>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t const ulCount;
		union
		{
			tiff_short_t uwValue;
			tiff_compression_e emCompression;
		};
		tiff_field_s()
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
	template <> struct tiff_field_s<TIFF_TAG_PHOTOMETRIC_INTERPRETATION>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t const ulCount;
		tiff_interpretation_e emInterpretation;
		tiff_field_s()
			: emTag(TIFF_TAG_PHOTOMETRIC_INTERPRETATION)
			, emType(TIFF_TYPE_SHORT)
			, ulCount(1)
			, emInterpretation(TIFF_INTERPRETATION_WHITE_IS_ZERO)
		{}
		tiff_field_s & operator = (tiff_interpretation_e emNewValue)
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
	template <> struct tiff_field_s<TIFF_TAG_STRIP_OFFSETS>
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
		tiff_field_s()
			: emTag(TIFF_TAG_STRIP_OFFSETS)
			, emType(TIFF_TYPE_SHORT)
			, ulCount(0)
			, ulByteOffset(0)
		{}
		tiff_field_s & operator = (tiff_long_t ulNewValue)
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
	template <> struct tiff_field_s<TIFF_TAG_ROWS_PER_STRIP>
	{
		tiff_tag_e const emTag;
		tiff_type_e emType;
		tiff_long_t const ulCount;
		tiff_long_t ulValue;
		tiff_field_s()
			: emTag(TIFF_TAG_ROWS_PER_STRIP)
			, emType(TIFF_TYPE_SHORT)
			, ulCount(1)
			, ulValue(0)
		{}
		tiff_field_s & operator = (tiff_long_t ulNewValue)
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
	template <> struct tiff_field_s<TIFF_TAG_X_RESOLUTION>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t const ulCount;
		tiff_byte_offset_t ulByteOffset;
		tiff_field_s()
			: emTag(TIFF_TAG_X_RESOLUTION)
			, emType(TIFF_TYPE_RATIONAL)
			, ulCount(1)
			, ulByteOffset(0)
		{}
	};
	//YResolution 283 11B RATIONAL 1
	template <> struct tiff_field_s<TIFF_TAG_Y_RESOLUTION>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t const ulCount;
		tiff_byte_offset_t ulByteOffset;

		tiff_field_s()
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
	template <> struct tiff_field_s<TIFF_TAG_FREE_OFFSETS>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t const ulCount;
		tiff_long_t ulValue;
		tiff_field_s()
			: emTag(TIFF_TAG_FREE_OFFSETS)
			, emType(TIFF_TYPE_LONG)
			, ulCount(1)
			, ulValue(0)
		{}
		tiff_field_s & operator = (tiff_long_t ulNewValue)
		{
			ulValue = ulNewValue;
			return *this;
		}
	};
	//FreeByteCounts 289 121 LONG
	template <> struct tiff_field_s<TIFF_TAG_FREE_BYTE_COUNTS>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t const ulCount;
		tiff_long_t ulValue;
		tiff_field_s()
			: emTag(TIFF_TAG_FREE_BYTE_COUNTS)
			, emType(TIFF_TYPE_LONG)
			, ulCount(1)
			, ulValue(0)
		{}
		tiff_field_s & operator = (tiff_long_t ulNewValue)
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
	template <> struct tiff_field_s<TIFF_TAG_RESOLUTION_UNIT>
	{
		tiff_tag_e const emTag;
		tiff_type_e const emType;
		tiff_long_t const ulCount;
		tiff_resolution_unit_e emValue;
		tiff_field_s()
			: emTag(TIFF_TAG_RESOLUTION_UNIT)
			, emType(TIFF_TYPE_SHORT)
			, ulCount(1)
			, emValue(TIFF_RESOLUTION_UNIT_INCH)
		{}
		tiff_field_s & operator = (tiff_resolution_unit_e emNewValue)
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
	struct tiff_field_s<emTAG, emOTHERS...>
		: tiff_field_s<emTAG>
		, tiff_field_s<emOTHERS...>
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
			static_cast<tiff_field_s<emTag>&>(stFields) = t;
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
		: tiff_field_s<TIFF_TAG_FREE_OFFSETS, TIFF_TAG_FREE_BYTE_COUNTS>
	{};

	struct tiff_baseline_fields
		: tiff_field_s<TIFF_TAG_IMAGE_WIDTH, TIFF_TAG_IMAGE_HEIGHT, TIFF_TAG_ROWS_PER_STRIP, TIFF_TAG_STRIP_OFFSETS, TIFF_TAG_X_RESOLUTION, TIFF_TAG_Y_RESOLUTION, TIFF_TAG_RESOLUTION_UNIT>
	{};
	struct tiff_baseline_file
		: tiff_file<tiff_baseline_fields>
	{};

	//

	template <typename TT>
	struct little_endianness_traits
		: TT
	{
		template <typename... TArgs>
		little_endianness_traits(TArgs... tArgs)
			: TT(std::forward<TArgs>(tArgs)...)
		{}
		template <typename T>
		static inline T convert(T t)
		{
			return t;
		}
	};
	template <typename TT>
	struct  big_endianness_traits
		: TT
	{
		template <typename... TArgs>
		big_endianness_traits(TArgs... tArgs)
			: TT(std::forward<TArgs>(tArgs)...)
		{}
		template <typename T>
		static inline std::enable_if_t<std::is_integral<T>::value && sizeof(T) == 2, T> convert(T t)
		{
			return static_cast<T>(_byteswap_ushort(t));
		}
		template <typename T>
		static inline std::enable_if_t<std::is_integral<T>::value && sizeof(T) == 4, T> convert(T t)
		{
			return static_cast<T>(_byteswap_ulong(t));
		}
		template <typename T>
		static inline std::enable_if_t<std::is_integral<T>::value && sizeof(T) == 8, T> convert(T t)
		{
			return static_cast<T>(_byteswap_uint64(t));
		}
	};
	template <typename TFields, typename TFunc, typename TT>
	bool tiff_enumerate_field(jrmwng::tiff_subfile<TFields> const & stTiffSubfile, TFunc const & tFunc, TT & tt)
	{
		for (unsigned short i = 0, j = TT::convert(stTiffSubfile.uwNumOfDirectoryEntries); i < j; i++)
		{
			if (!tFunc((&stTiffSubfile.stFields)[i], tt))
			{
				return false;
			}
		}
		return true;
	}
	template <typename TFields, typename TFunc, typename TT>
	bool tiff_enumerate_subfile(jrmwng::tiff_subfile<TFields> const *pstSubfile, TFunc const & tFunc, TT & tt)
	{
		while (pstSubfile)
		{
			unsigned long const *pulNextSubfile = reinterpret_cast<unsigned long const*>(&pstSubfile->stFields + TT::convert(pstSubfile->uwNumOfDirectoryEntries));
			{
				if (pulNextSubfile > std::get<1>(tt))
				{
					__debugbreak();
					return false;
				}
			}
			if (!tFunc(*pstSubfile, tt))
			{
				return false;
			}
			pstSubfile = nullptr;
			unsigned long const ulNextSubfile = TT::convert(*pulNextSubfile);
			if (ulNextSubfile)
			{
				reinterpret_cast<char const*&>(pstSubfile) = reinterpret_cast<char const*>(std::get<0>(tt)) + ulNextSubfile;
			}
		}
		return true;
	}
	template <typename TFunc, typename TT>
	bool tiff_enumerate_subfile(TFunc const & tFunc, TT & tt)
	{
		auto const *pstFirstSubfile = reinterpret_cast<tiff_subfile<tiff_field_base_t>const*>(reinterpret_cast<char const*>(std::get<0>(tt)) + TT::convert(std::get<0>(tt)->ulFirstSubfile));
		return jrmwng::tiff_enumerate_subfile(pstFirstSubfile, tFunc, tt);
	}
	template <typename TFunc>
	bool tiff_enumerate_subfile(tiff_file_header const *pstTiffFileHeader, void const *pEnd, TFunc const & tFunc)
	{
		if (pstTiffFileHeader > pEnd)
		{
			__debugbreak();
			return false;
		}
		char const cByteOrder0 = pstTiffFileHeader->acByteOrder[0];
		char const cByteOrder1 = pstTiffFileHeader->acByteOrder[1];
		auto tt = std::make_tuple(pstTiffFileHeader, pEnd);
		if (cByteOrder0 == 'I' && cByteOrder1 == 'I')
		{
			return tiff_enumerate_subfile(tFunc, little_endianness_traits<decltype(tt)>(tt));
		}
		if (cByteOrder0 == 'M' && cByteOrder1 == 'M')
		{
			return tiff_enumerate_subfile(tFunc, big_endianness_traits<decltype(tt)>(tt));
		}
		return false;
	}
#pragma pack(pop)
}
