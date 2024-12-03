#ifndef __MU_STANDARD_TYPES_H__
#define __MU_STANDARD_TYPES_H__

#include "shared_standard_types.h"

#include <boost/filesystem.hpp>

// VersionMajor.VersionMinor.Status.Revision (Status : 0 (Alpha), 1 (Beta), 2 (Release Candidate), 3 (Final Release), 4 (Internal))
#ifdef INTERNAL_SERVER
#define ELION_VERSION_MAJOR 0
#define ELION_VERSION_MINOR 1
#define ELION_VERSION_STATUS 1
#define ELION_VERSION_REVISION 30
#define ELION_SERIAL _T("nmdk@*34r489NF(*n4nifHJ($3nifunido$(88h9#H93d9nfnndno3o8d83(F)&")
#else
#define ELION_VERSION_MAJOR 0
#define ELION_VERSION_MINOR 1
#define ELION_VERSION_STATUS 1
#define ELION_VERSION_REVISION 30
#define ELION_SERIAL _T("nmdk@*34r489NF(*n4nifHJ($3nifunido$(88h9#H93d9nfnndno3o8d83(F)&")
#endif

#define ELION_VERSION _T("" _GETTEXT(ELION_VERSION_MAJOR) "." _GETTEXT(ELION_VERSION_MINOR) "." _GETTEXT(ELION_VERSION_STATUS) "." _GETTEXT(ELION_VERSION_REVISION)) // VersionMajor.VersionMinor.Status.Revision (Status : 0 (Alpha), 1 (Beta), 2 (Release Candidate), 3 (Final Release)
#define ELION_COMPILETIME _T(__DATE__)

#if 1
typedef std::map<const mu_char*, mu_uint32, ascii_icmp> ResourceNameMap;
#else
typedef std::map<mu_asciistr, mu_uint32> ResourceNameMap;
#endif

#if __ANDROID__
extern mu_asciistr MUBasePrefPath;
extern mu_string MUBasePrefPathUnicode;
#endif

namespace MU
{
	static constexpr mu_float RcpToFloat = 1.0f / 255.0f;
	FORCEINLINE EVector4 ARGBToFloat4(const mu_uint8 A, const mu_uint8 R, const mu_uint8 G, const mu_uint8 B)
	{
		return EVector4(R * RcpToFloat, G * RcpToFloat, B * RcpToFloat, A * RcpToFloat);
	}
	FORCEINLINE EVector4 ARGBToFloat4(const mu_uint32 ARGB)
	{
		mu_uint8 A = MU::GetAlpha(ARGB);
		mu_uint8 R = MU::GetRed(ARGB);
		mu_uint8 G = MU::GetGreen(ARGB);
		mu_uint8 B = MU::GetBlue(ARGB);

		return EVector4(R * RcpToFloat, G * RcpToFloat, B * RcpToFloat, A * RcpToFloat);
	}
	FORCEINLINE constexpr mu_float Abs(const mu_float Value)
	{
		return Value < 0.0f ? -Value : Value;
	}
	FORCEINLINE constexpr mu_uint8 To255(const mu_float Color)
	{
		return Color <= 0.0f ? 0 : (Color >= 1.0f ? 255 : (mu_uint8)(Color * 255.0f));
	}
	FORCEINLINE constexpr mu_uint32 Float4ToARGB(const EVector4 &Color)
	{
		return MakeARGB(To255(Color.x), To255(Color.y), To255(Color.z), To255(Color.w));
	}
	FORCEINLINE constexpr mu_uint32 Float4ToARGB(const EVector3 &Color, const mu_float Alpha)
	{
		return MakeARGB(To255(Color.x), To255(Color.y), To255(Color.z), To255(Alpha));
	}
	FORCEINLINE constexpr mu_uint32 Float4ToARGB(const mu_float r, const mu_float g, const mu_float b, const mu_float a = 1.0f)
	{
		return MakeARGB(To255(r), To255(g), To255(b), To255(a));
	}
	FORCEINLINE constexpr mu_uint32 Float3AlphaToARGB(const mu_float r, const mu_float g, const mu_float b, const mu_uint8 a)
	{
		return MakeARGB(To255(r), To255(g), To255(b), a);
	}

	FORCEINLINE void MakeDirectory(mu_string Path)
	{
		std::replace(Path.begin(), Path.end(), _T('\\'), _T('/'));

#if __ANDROID__
		boost::filesystem::path dirPath(MUBasePrefPathUnicode + Path);
#else
		boost::filesystem::path dirPath(Path);
#endif
		boost::system::error_code ec;
		boost::filesystem::create_directories(dirPath, ec);
	}
};

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((mu_uint32)(mu_uint8)(ch0) | ((mu_uint32)(mu_uint8)(ch1) << 8) |       \
                ((mu_uint32)(mu_uint8)(ch2) << 16) | ((mu_uint32)(mu_uint8)(ch3) << 24 ))
#endif /* defined(MAKEFOURCC) */

template<class StreamType>
FORCEINLINE mu_boolean mu_openfile(StreamType &stream, const mu_text *filename, std::ios_base::openmode openmode)
{
#ifdef UNICODE
	mu_uint32 filenameSize = mu_strlen(filename);
	mu_uint32 neededSize = 0;
	if (ConvertToUTF8(filename, filenameSize, nullptr, 0, &neededSize) == false)
	{
		return false;
	}

	mu_char *filenameBuffer = new mu_char[neededSize + 1];
	mu_uint32 convertedSize = 0;
	if (ConvertToUTF8(filename, filenameSize, filenameBuffer, neededSize + 1, &convertedSize) == false)
	{
		delete[] filenameBuffer;
		return false;
	}

#if __ANDROID__
	stream.open((MUBasePrefPath + filenameBuffer).c_str(), openmode);
#else
	stream.open(filenameBuffer, openmode);
#endif
	delete[] filenameBuffer;
#else
#if __ANDROID__
	stream.open((MUBasePrefPath + filename).c_str(), openmode);
#else
	stream.open(filename, openmode);
#endif
#endif

	return stream.is_open();
}

FORCEINLINE mu_boolean mu_fopen_s(FILE **file, const mu_text *filename, const mu_char *mode)
{
#ifdef UNICODE
	mu_uint32 filenameSize = (mu_uint32)mu_strlen(filename);
	mu_uint32 neededSize = 0;
	if (ConvertToUTF8(filename, filenameSize, nullptr, 0, &neededSize) == false)
	{
		return false;
	}

	mu_char *filenameBuffer = new mu_char[neededSize + 1];
	mu_uint32 convertedSize = 0;
	if (ConvertToUTF8(filename, filenameSize, filenameBuffer, neededSize + 1, &convertedSize) == false)
	{
		delete[] filenameBuffer;
		return false;
	}

#if __ANDROID__
	*file = fopen((MUBasePrefPath + filenameBuffer).c_str(), mode);
	delete[] filenameBuffer;

	return *file != nullptr;
#else
	errno_t error = fopen_s(file, filenameBuffer, mode);
	delete[] filenameBuffer;

	return error == 0;
#endif
#else
#if __ANDROID__
	*file = fopen((MUBasePrefPath + filename).c_str(), mode);
	return *file != nullptr;
#else
	return fopen_s(file, filename, mode) == 0;
#endif
#endif
}

FORCEINLINE mu_boolean mu_rwfromfile(SDL_RWops **file, const mu_text *filename, const mu_char *mode)
{
#ifdef UNICODE
	mu_uint32 filenameSize = (mu_uint32)mu_strlen(filename);
	mu_uint32 neededSize = 0;
	if (ConvertToUTF8(filename, filenameSize, nullptr, 0, &neededSize) == false)
	{
		return false;
	}

	mu_char *filenameBuffer = new mu_char[neededSize + 1];
	mu_uint32 convertedSize = 0;
	if (ConvertToUTF8(filename, filenameSize, filenameBuffer, neededSize + 1, &convertedSize) == false)
	{
		delete[] filenameBuffer;
		return false;
	}

#if __ANDROID__
	*file = SDL_RWFromFile((MUBasePrefPath + filenameBuffer).c_str(), mode);
#else
	*file = SDL_RWFromFile(filenameBuffer, mode);
#endif
	delete[] filenameBuffer;

	return *file != nullptr;
#else
#if __ANDROID__
	*file = SDL_RWFromFile((MUBasePrefPath + filename).c_str(), mode);
#else
	*file = SDL_RWFromFile(filename, mode);
#endif
	return *file != nullptr;
#endif
}

FORCEINLINE mu_boolean mu_rwexists(const mu_text *filename)
{
#ifdef UNICODE
	mu_uint32 filenameSize = (mu_uint32)mu_strlen(filename);
	mu_uint32 neededSize = 0;
	if (ConvertToUTF8(filename, filenameSize, nullptr, 0, &neededSize) == false)
	{
		return false;
	}

	mu_char *filenameBuffer = new mu_char[neededSize + 1];
	mu_uint32 convertedSize = 0;
	if (ConvertToUTF8(filename, filenameSize, filenameBuffer, neededSize + 1, &convertedSize) == false)
	{
		delete[] filenameBuffer;
		return false;
	}

#if __ANDROID__
	SDL_RWops *file = SDL_RWFromFile((MUBasePrefPath + filenameBuffer).c_str(), "r");
#else
	SDL_RWops *file = SDL_RWFromFile(filenameBuffer, "r");
#endif
	if (file != nullptr)
	{
		SDL_RWclose(file);
	}
	delete[] filenameBuffer;

	return file != nullptr;
#else
#if __ANDROID__
	SDL_RWops *file = SDL_RWFromFile((MUBasePrefPath + filename).c_str(), "r");
#else
	SDL_RWops *file = SDL_RWFromFile(filename, "r");
#endif
	if (file != nullptr)
	{
		SDL_RWclose(file);
	}

	return file != nullptr;
#endif
}

#endif