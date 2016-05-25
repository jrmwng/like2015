
#include <intrin.h>
#include <tchar.h>
#include <Windows.h>
#include <functional>
#include <algorithm>
#include <iterator>
#include <memory>

#ifdef max
#undef max
#endif

#include "..\jrmwng\tiff.h"
#include "..\jrmwng\jpeg.h"
#include "..\jrmwng\memory.h"

namespace
{
	template <HANDLE hInvalidHandleValue>
	struct win32_close_handle_deleter
	{
		void operator () (HANDLE hHandle) const
		{
			if (hHandle != hInvalidHandleValue)
			{
				if (CloseHandle(hHandle) == FALSE)
				{
					__debugbreak();
				}
			}
		}
	};
	struct win32_find_close_deleter
	{
		void operator () (HANDLE hFindFile) const
		{
			if (hFindFile != INVALID_HANDLE_VALUE)
			{
				if (FindClose(hFindFile) == FALSE)
				{
					__debugbreak();
				}
			}
		}
	};
	struct win32_file_name
	{
		TCHAR m_acFileName[MAX_PATH];

		win32_file_name(LPCTSTR pcFormat, ...)
		{
			va_list vaList;
			va_start(vaList, pcFormat);
			int const nRet = _vstprintf_s(m_acFileName, pcFormat, vaList);
			va_end(vaList);
			if (nRet < 0)
			{
				m_acFileName[0] = 0;
				__debugbreak();
				return;
			}
		}

		operator LPCTSTR () const
		{
			return m_acFileName;
		}
	};
	struct win32_find_file_iterator
		: std::iterator<std::forward_iterator_tag, WIN32_FIND_DATA>
	{
		std::unique_ptr<void, win32_find_close_deleter> m_upFindFile;
		WIN32_FIND_DATA m_stWin32FindData;

		win32_find_file_iterator()
			: m_upFindFile(INVALID_HANDLE_VALUE)
		{}
		win32_find_file_iterator(win32_find_file_iterator && that)
			: m_upFindFile(std::move(that.m_upFindFile))
			, m_stWin32FindData(that.m_stWin32FindData)
		{}
		template <typename... TArgs>
		win32_find_file_iterator(TArgs... tArgs)
			: m_upFindFile(INVALID_HANDLE_VALUE)
		{
			win32_file_name const acFileName(std::forward<TArgs>(tArgs)...);

			HANDLE const hFindFile = FindFirstFileEx(acFileName, FINDEX_INFO_LEVELS::FindExInfoBasic, &m_stWin32FindData, FINDEX_SEARCH_OPS::FindExSearchNameMatch, NULL, 0);
			{
				if (hFindFile == INVALID_HANDLE_VALUE)
				{
					__debugbreak();
					return;
				}
			}
			m_upFindFile.reset(hFindFile);
		}

		operator bool() const
		{
			return m_upFindFile.get() != INVALID_HANDLE_VALUE;
		}
		bool operator != (win32_find_file_iterator const & that) const
		{
			return m_upFindFile.get() != that.m_upFindFile.get();
		}

		WIN32_FIND_DATA const * operator -> () const
		{
			return &m_stWin32FindData;
		}
		WIN32_FIND_DATA const & operator * () const
		{
			return m_stWin32FindData;
		}

		win32_find_file_iterator & operator ++ ()
		{
			if (FindNextFile(m_upFindFile.get(), &m_stWin32FindData) == FALSE)
			{
				if (GetLastError() != ERROR_NO_MORE_FILES)
				{
					__debugbreak();
				}
				m_upFindFile.reset(INVALID_HANDLE_VALUE);
			}
			return *this;
		}
	};
	template <size_t szCapacity, size_t szOverflow = szCapacity>
	struct win32_file
	{
		unsigned char m_aubOverflow[szOverflow];
		unsigned char m_aubBuffer[szCapacity];

		std::unique_ptr<void, win32_close_handle_deleter<INVALID_HANDLE_VALUE> > m_upFile;
		size_t m_uBegin;
		size_t m_uEnd;

		win32_file(std::unique_ptr<void, win32_close_handle_deleter<INVALID_HANDLE_VALUE> > && upFile)
			: m_upFile(std::move(upFile))
			, m_uBegin(0)
			, m_uEnd(0)
		{}

		template <typename T>
		BOOL read(std::function<BOOL(T &)> tFunc, size_t const szAdvance = sizeof(T), size_t const szPeek = sizeof(T))
		{
			if (sizeof(T) > szOverflow)
			{
				return FALSE;
			}
			if (m_uEnd - m_uBegin < std::max({ sizeof(T), szAdvance, szPeek }))
			{
				if (m_uEnd >= szCapacity)
				{
					__movsb(std::begin(m_aubBuffer) - (m_uEnd - m_uBegin), std::end(m_aubBuffer) - (m_uEnd - m_uBegin), m_uEnd - m_uBegin);

					m_uBegin -= szCapacity;
					m_uEnd -= szCapacity;
				}
				DWORD dwNumberOfBytesRead;
				{
					if (ReadFile(m_upFile.get(), m_aubBuffer + m_uEnd, static_cast<DWORD>(szCapacity - m_uEnd), &dwNumberOfBytesRead, NULL) == FALSE)
					{
						__debugbreak();
						return FALSE;
					}
				}
				m_uEnd += dwNumberOfBytesRead;

				if (m_uEnd - m_uBegin < sizeof(T))
				{
					return FALSE;
				}
			}
			T & t = *reinterpret_cast<T*>(m_aubBuffer + m_uBegin);
			m_uBegin += szAdvance;
			return tFunc(t);
		}
	};
}

int _tmain(int nArg, TCHAR *apcArg[])
{
	alignas(32) CREATEFILE2_EXTENDED_PARAMETERS stCreateFile2Params;
	{
		jrmwng::memset(&stCreateFile2Params, 0);
		stCreateFile2Params.dwSize = sizeof(stCreateFile2Params);
		stCreateFile2Params.dwFileFlags = OPEN_EXISTING;
		stCreateFile2Params.dwFileAttributes = FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING;
	}

	std::for_each(apcArg + 1, apcArg + nArg, [&](TCHAR *pcArg)
	{
		for (win32_find_file_iterator itFindFile(_T("%s\\%s"), pcArg, _T("*.orf")); itFindFile; ++itFindFile)
		{
			std::unique_ptr<void, win32_close_handle_deleter<INVALID_HANDLE_VALUE> > upFile;
			{
				win32_file_name const acFileName(_T("%s\\%s"), pcArg, itFindFile->cFileName);

				HANDLE const hFile = CreateFile2(acFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, &stCreateFile2Params);
				{
					if (hFile == INVALID_HANDLE_VALUE)
					{
						return;
					}
				}
				upFile.reset(hFile);
			}
			win32_file<0x11000> stFile(std::move(upFile));

			auto fnTiffFileHandler = [&](auto const & stTiffSubfile, auto & tt)->bool
			{
				return jrmwng::tiff_enumerate_field(stTiffSubfile, [&](auto const & stTiffField, auto & tt)->bool
				{
					auto pcTiffValue = reinterpret_cast<char const*>(std::get<0>(tt)) + tt.convert(stTiffField.ulByteOffset);
					auto pstExifSubfile = reinterpret_cast<jrmwng::tiff_subfile<jrmwng::exif_field_base_t>const*>(pcTiffValue);
					auto pstGpsSubfile = reinterpret_cast<jrmwng::tiff_subfile<jrmwng::exif_gps_field_base_t>const*>(pcTiffValue);
					switch (stTiffField.emTag)
					{
					case jrmwng::TIFF_TAG_EXIF_IFD:
						return jrmwng::tiff_enumerate_subfile(pstExifSubfile, [&](auto const & stExifSubfile, auto & tt)->bool
						{
							return jrmwng::tiff_enumerate_field(stExifSubfile, [&](auto const & stExifField, auto & tt)->bool
							{
								auto pcExifValue = reinterpret_cast<char const*>(std::get<0>(tt)) + tt.convert(stExifField.ulByteOffset);
								switch (stExifField.emTag)
								{
								case jrmwng::EXIF_TAG_INTEROPERABILITY_IFD:
									auto pstInteropSubfile = reinterpret_cast<jrmwng::tiff_subfile<jrmwng::exif_interoperability_field_base_t>const*>(reinterpret_cast<char const*>(std::get<0>(tt)) + tt.convert(stExifField.ulByteOffset));
									return jrmwng::tiff_enumerate_subfile(pstInteropSubfile, [&](auto const & stInteropSubfile, auto & tt)->bool
									{
										return jrmwng::tiff_enumerate_field(stInteropSubfile, [&](auto const & stInteropField, auto & tt)->bool
										{
											auto pcInteropValue = reinterpret_cast<char const*>(std::get<0>(tt)) + tt.convert(stInteropField.ulByteOffset);
											return true;
										}, tt);
									}, tt);
								}
								return true;
							}, tt);
						}, tt);
					case jrmwng::TIFF_TAG_GPS_IFD:
						return jrmwng::tiff_enumerate_subfile(pstGpsSubfile, [&](auto const & stGpsSubfile, auto & tt)->bool
						{
							return jrmwng::tiff_enumerate_field(stGpsSubfile, [&](auto const & stGpsField, auto & tt)->bool
							{
								auto pcGpsValue = reinterpret_cast<char const*>(std::get<0>(tt)) + tt.convert(stGpsField.ulByteOffset);
								return true;
							}, tt);
						}, tt);
					}
					return true;
				}, tt);
			};

			if (stFile.read<jrmwng::tiff_file_header>([&](jrmwng::tiff_file_header const & stTiffFileHeader)->BOOL
			{
				auto p1 = reinterpret_cast<char const*>(&stTiffFileHeader) + 0x10000;
				return jrmwng::tiff_enumerate_subfile(&stTiffFileHeader, p1, fnTiffFileHandler);
			}, 0, 0x10000))
			{
				continue;
			}


			if (stFile.read< jrmwng::jpeg_marker_s<jrmwng::JPEG_MARKER_SOI> >([&](auto & stSOI)->BOOL
				{
					if (!stSOI)
					{
						return FALSE;
					}
					for (size_t szAdvance = sizeof(stSOI);  stFile.read<jrmwng::jpeg_marker_base_s>([&](jrmwng::jpeg_marker_base_s & stMarker)->BOOL
					{
						size_t szAdvance = 2 + _byteswap_ushort(stMarker.uwLength);
						switch (stMarker.emMarker)
						{
						case jrmwng::JPEG_MARKER_APP0:
							return stFile.read< jrmwng::jpeg_marker_s<jrmwng::JPEG_MARKER_APP0> >([&](auto & stAPP0)->BOOL
							{
								void *p0 = reinterpret_cast<char*>(&stAPP0 + 1);
								void *p1 = reinterpret_cast<char*>(&stAPP0) + szAdvance;

								auto *pstJFIF = reinterpret_cast<jrmwng::jpeg_marker_jfif_s<0, 0>*>(&stAPP0);
								{
									if (pstJFIF + 1 > p1)
									{
										// TODO
										return FALSE;
									}
								}
								if (memcmp(pstJFIF->acIdentifier, "JFIF", 5) == 0)
								{
									unsigned short const uwDensityX = _byteswap_ushort(pstJFIF->uwDensityX);
									unsigned short const uwDensityY = _byteswap_ushort(pstJFIF->uwDensityY);
								}
								return TRUE;
							}, szAdvance);
						case jrmwng::JPEG_MARKER_APP1:
							return stFile.read< jrmwng::jpeg_marker_s<jrmwng::JPEG_MARKER_APP1> >([&](jrmwng::jpeg_marker_s<jrmwng::JPEG_MARKER_APP1> & stAPP1)->BOOL
							{
								void const *p0 = &stAPP1 + 1;
								void const *p1 = reinterpret_cast<char*>(&stAPP1) + szAdvance;

								auto const *pstExif = reinterpret_cast<jrmwng::jpeg_marker_exif_s const*>(&stAPP1);
								{
									if (pstExif + 1 > p1)
									{
										// TODO
										return FALSE;
									}
								}
								if (memcmp(pstExif->acIdentifier, "Exif", 5) == 0)
								{
									auto const *pstTiffFileHeader = reinterpret_cast<jrmwng::tiff_file_header const*>(pstExif + 1);
									return jrmwng::tiff_enumerate_subfile(pstTiffFileHeader, p1, fnTiffFileHandler);
								}
								return TRUE;
							}, szAdvance);
						case jrmwng::JPEG_MARKER_DQT:
							return stFile.read< jrmwng::jpeg_marker_s<jrmwng::JPEG_MARKER_DQT> >([&](auto & stDQT)->BOOL
							{
								for (void *p0 = &stDQT + 1, *p1 = reinterpret_cast<char*>(&stDQT) + szAdvance; p0 < p1; )
								{
									auto *pstByteDQT = reinterpret_cast<jrmwng::jpeg_quantization_table_s<jrmwng::jpeg_quantization_t<jrmwng::JPEG_QUANT_BYTE>, 1>*>(p0);
									auto *pstWordDQT = reinterpret_cast<jrmwng::jpeg_quantization_table_s<jrmwng::jpeg_quantization_t<jrmwng::JPEG_QUANT_WORD>, 1>*>(p0);

									if (pstByteDQT + 1 <= p1 &&
										pstByteDQT->ubPrec == 0)
									{
										p0 = pstByteDQT + 1;
										continue;
									}
									if (pstWordDQT + 1 <= p1 &&
										pstWordDQT->ubPrec != 0)
									{
										p0 = pstWordDQT + 1;
										continue;
									}
									// TODO: revise behavior
									break;
								}
								return TRUE;
							}, szAdvance);
						case jrmwng::JPEG_MARKER_SOF0:
							return stFile.read< jrmwng::jpeg_marker_s<jrmwng::JPEG_MARKER_SOF0, 3> >([&](jrmwng::jpeg_marker_s<jrmwng::JPEG_MARKER_SOF0, 3> & stSOF0)->BOOL
							{
								if (stSOF0.astComponent + stSOF0.ubNumberOfComponents > (void*) (reinterpret_cast<char*>(&stSOF0) + szAdvance))
								{
									// buffer overflow
									return FALSE;
								}
								unsigned short const uwImageWidth = _byteswap_ushort(stSOF0.uwImageWidth);
								unsigned short const uwImageHeight = _byteswap_ushort(stSOF0.uwImageHeight);
								for (int i = 0, j = stSOF0.ubNumberOfComponents; i < j; i++)
								{
									stSOF0.astComponent[i];
								}
								return TRUE;
							}, szAdvance);
						case jrmwng::JPEG_MARKER_DHT:
							return stFile.read< jrmwng::jpeg_marker_s<jrmwng::JPEG_MARKER_DHT> >([&](auto & stDHT)->BOOL
							{
								for (void *p0 = &stDHT + 1, *p1 = reinterpret_cast<char*>(&stDHT) + szAdvance; p0 < p1; )
								{
									auto *pstHuffmanTable = reinterpret_cast<jrmwng::jpeg_huffman_table_s<1>*>(p0);

									__m128i const ub16Bits0 = _mm_loadu_si128((__m128i const*)pstHuffmanTable->aubBits);
									__m128i const xl2SumBits1 = _mm_sad_epu8(ub16Bits0, _mm_setzero_si128());

									p0 = reinterpret_cast<char*>(pstHuffmanTable + 1) + xl2SumBits1.m128i_i64[0] + xl2SumBits1.m128i_i64[1];
								}
								return TRUE;
							}, szAdvance);
						case jrmwng::JPEG_MARKER_SOS:
							return stFile.read< jrmwng::jpeg_marker_s<jrmwng::JPEG_MARKER_SOS, 3> >([&](jrmwng::jpeg_marker_s<jrmwng::JPEG_MARKER_SOS, 3> & stSOS)->BOOL
							{
								return FALSE;
							}, szAdvance);
						default:
							return FALSE;
						}
					}, szAdvance); szAdvance = 0)
					{
						// NOP
					}
					return TRUE;
				}), 0)
			{
				continue;
			}
		}
	});
	return 0;
}
