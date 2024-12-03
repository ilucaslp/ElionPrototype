#ifndef __SHARED_STANDARD_TYPES_H__
#define __SHARED_STANDARD_TYPES_H__

#if defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#ifndef FORCEINLINE
#if __ANDROID__
#define FORCEINLINE inline
#else
#define FORCEINLINE __forceinline
#endif
#endif

#include <iostream>
#include <atomic>
#include <cwchar>
#include <array>
#include <boost\regex.hpp>
#include <boost\tokenizer.hpp>
#include <boost\lexical_cast.hpp>

#if DIRECTX_BACKEND_ENABLE == 1
#define mu_nullptr nullptr
#else
#define mu_nullptr 0
#endif

#ifndef MU_ALIGN
#if defined(_MSC_VER)
#define MU_BALIGN(x) __declspec(align(x))
#define MU_ALIGN(x, ...) MU_BALIGN(x) __VA_ARGS__
#else
#define MU_BALIGN(x) __attribute__((aligned(x)))
#define MU_ALIGN(x, ...) __VA_ARGS__ MU_BALIGN(x)
#endif
#endif

#if defined(__arm__)
#define ARM_PACKED __attribute__((packed))
#else
#define ARM_PACKED
#endif

#ifdef UNICODE
typedef wchar_t				mu_text;
typedef std::wstring		mu_string;
typedef std::wstringstream	mu_stringstream;
typedef boost::wregex		mu_regex;
#else
typedef char				mu_text;
typedef std::string			mu_string;
typedef std::stringstream	mu_stringstream;
typedef boost::regex		mu_regex;
#endif

typedef boost::char_separator<mu_text> mu_separator;
typedef boost::tokenizer<mu_separator, mu_string::const_iterator, mu_string> mu_tokenizer;

typedef std::string			mu_asciistr;
typedef std::wstring		mu_unicodestr;

typedef std::ifstream		mu_ifstream;
typedef std::ofstream		mu_ofstream;
typedef std::fstream		mu_fstream;

typedef char				mu_char;
typedef wchar_t				mu_unicode;

typedef bool				mu_boolean;
typedef signed char			mu_int8;
typedef signed short		mu_int16;
typedef signed int			mu_int32;
typedef signed long long	mu_int64;
typedef unsigned char		mu_uint8;
typedef unsigned short		mu_uint16;
typedef unsigned int		mu_uint32;
typedef unsigned long long	mu_uint64;

typedef size_t				mu_count;
typedef size_t				mu_size;

typedef double				mu_double;
typedef float				mu_float;

typedef void*				mu_ptr;

#if __ANDROID__
typedef std::atomic_bool		mu_atomic_bool;
typedef std::atomic<mu_int16>	mu_atomic_int16_t;
typedef std::atomic<mu_uint16>	mu_atomic_uint16_t;
typedef std::atomic<mu_int32>	mu_atomic_int32_t;
typedef std::atomic<mu_uint32>	mu_atomic_uint32_t;
#else
typedef std::atomic_bool		mu_atomic_bool;
typedef std::atomic_int16_t		mu_atomic_int16_t;
typedef std::atomic_uint16_t	mu_atomic_uint16_t;
typedef std::atomic_int32_t		mu_atomic_int32_t;
typedef std::atomic_uint32_t	mu_atomic_uint32_t;
#endif

#ifndef _T
#ifdef UNICODE
#define _T(x)      L ## x
#else
#define _T(x)      x
#endif
#endif

#ifndef _GETTEXT
#define __GETTEXT(x) #x
#define _GETTEXT(value) __GETTEXT(value)
#endif

#if __ANDROID__
#ifdef UNICODE
#define SPF_STRING _T("%ls")
#define SPF_CHAR _T("%lc")
#else
#define SPF_STRING _T("%s")
#define SPF_CHAR _T("%c")
#endif
#else
#define SPF_STRING _T("%s")
#define SPF_CHAR _T("%c")
#endif

#define mu_countof(array) (sizeof(array)/sizeof(array[0]))

#if __ANDROID__
#define _stricmp strcasecmp
#define strncpy_s strncpy

FORCEINLINE int sprintf_s(char *s, size_t n, const char *fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vsnprintf(s, n, fmt, ap);
	if (ret < n) s[ret] = '\0';
	va_end(ap);
	return ret;
}
FORCEINLINE void strcpy_s(char *dest, const mu_size destLength, const char *src)
{
	strcpy(dest, src);
}
FORCEINLINE void strncpy_s(char *dest, const mu_size destLength, const char *src, const mu_size copyLength)
{
	strncpy(dest, src, copyLength);
}
FORCEINLINE void strcat_s(char *dest, const mu_size destLength, const char *src)
{
	strcat(dest, src);
}

#define _wcsicmp wcscasecmp
#define _wcsnicmp wcsncasecmp
#define swscanf_s swscanf

FORCEINLINE int swprintf_s(wchar_t *s, size_t n, const wchar_t *fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vswprintf(s, n, fmt, ap);
	if (ret < n) s[ret] = L'\0';
	va_end(ap);
	return ret;
}
FORCEINLINE void wcscpy_s(wchar_t *dest, const mu_size destLength, const wchar_t *src)
{
	wcscpy(dest, src);
}
FORCEINLINE void wcsncpy_s(wchar_t *dest, const mu_size destLength, const wchar_t *src, const mu_size copyLength)
{
	wcsncpy(dest, src, copyLength);
}
FORCEINLINE void wcscat_s(wchar_t *dest, const mu_size destLength, const wchar_t *src)
{
	wcscat(dest, src);
}
#endif

#ifdef UNICODE
#define mu_strcmp wcscmp
#define mu_strlen wcslen
#define mu_strcpy_s wcscpy_s
#define mu_strncpy_s wcsncpy_s
#define mu_sprintf swprintf
#define mu_sprintf_s swprintf_s
#define mu_vsprintf_s vswprintf_s
#define mu_tolower towlower
#define mu_toupper towupper
#define mu_strcmp wcscmp
#define mu_strncmp wcsncmp
#define mu_strtok wcstok_s
#define mu_strcat_s wcscat_s
#else
#define mu_strcmp strcmp
#define mu_strlen strlen
#define mu_strcpy_s strcpy_s
#define mu_strncpy_s strncpy_s
#define mu_sprintf sprintf
#define mu_sprintf_s sprintf_s
#define mu_vsprintf_s vsprintf_s
#define mu_tolower tolower
#define mu_toupper toupper
#define mu_strcmp strcmp
#define mu_strncmp strncmp
#define mu_strtok strtok_s
#define mu_strcat_s strcat_s
#endif

#define mu_utf8ncmp strncmp
#define mu_sprintf8_s sprintf_s

#define mu_asciilen strlen
#define mu_unicodelen wcslen
#define mu_asciilower tolower
#define mu_unicodelower towlower

#define mu_mblen strlen
#define mu_widelen wcslen

#define mu_fclose fclose
#if __ANDROID__
#define mu_fseek fseek
#define mu_ftellg ftell
#else
#define mu_fseek _fseeki64
#define mu_ftellg _ftelli64
#endif
#define mu_fread fread
#define mu_fwrite fwrite

#define mu_memcmp memcmp
#define mu_memset memset
#if __ANDROID__
#define mu_zeromem(D, L) mu_memset(D, 0, L)
#define mu_memcpy memcpy
#else
#define mu_zeromem ZeroMemory
#define mu_memcpy CopyMemory
#endif
#define mu_memmove memmove
#define mu_malloc malloc
#define mu_realloc realloc
#define mu_free free
#define mu_sleep(ms) boost::this_thread::sleep_for(boost::chrono::milliseconds(ms))

#if __ANDROID__
typedef int errno_t;
#else
#define mu_messagebox ::MessageBox
#define mu_getdc ::GetDC
#endif

#ifdef UNICODE
#define mu_splitpath		_wsplitpath_s
#define mu_strcpy			wcscpy_s
#define mu_strncpy			wcsncpy_s
#define mu_strcpy_ns		wcscpy
#define mu_strncpy_ns		wcsncpy
#define mu_strcat			wcscat_s
#define mu_strcat_ns		wcscat
#define mu_stricmp			_wcsicmp
#define mu_strnicmp			_wcsnicmp
#define mu_strcmp			wcscmp
#define mu_printf			wprintf
#define mu_strtol			wcstol
#define mu_strtoul			wcstoul
#define mu_strstr			wcsstr
#define mu_sscanf			swscanf_s

#define mu_getc				getc
#define mu_ungetc			ungetc
#define mu_isspace			iswspace
#define mu_isalpha			iswalpha
#define mu_isdigit			iswdigit
#define mu_isalnum			iswalnum
#define mu_isxdigit			iswxdigit
#define mu_atof				_wtof
#define mu_itoa				_itow_s
#else
#define mu_splitpath		_splitpath_s
#define mu_strcpy			strcpy_s
#define mu_strncpy			strncpy_s
#define mu_strcpy_ns		strcpy
#define mu_strncpy_ns		strncpy
#define mu_strcat			strcat_s
#define mu_strcat_ns		strcat
#define mu_stricmp			_stricmp
#define mu_strnicmp			_strnicmp
#define mu_strcmp			strcmp
#define mu_printf			printf
#define mu_strtol			strtol
#define mu_strtoul			strtoul
#define mu_strstr			strstr
#define mu_sscanf			sscanf_s

#define mu_getc				getc
#define mu_ungetc			ungetc
#define mu_isspace			isspace
#define mu_isalpha			isalpha
#define mu_isdigit			isdigit
#define mu_isalnum			isalnum
#define mu_isxdigit			isxdigit
#define mu_atof				atof
#define mu_itoa				_itoa_s
#endif

#define mu_wstrcpy			wcscpy_s
#define mu_mstrcpy			strcpy_s

namespace MU
{
#define MAKE_ARGB(a,r,g,b) ((mu_uint32)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff)))

	inline constexpr mu_uint32 MakeARGB(const mu_uint8 R, const mu_uint8 G, const mu_uint8 B, const mu_uint8 A)
	{
		return MAKE_ARGB(A, R, G, B);
	}
	inline constexpr mu_uint32 GetRGB(const mu_uint32 ARGB)
	{
		return ARGB & 0x00FFFFFF;
	}
	inline constexpr mu_uint8 GetAlpha(const mu_uint32 ARGB)
	{
		return ((ARGB) >> 24);
	}
	inline constexpr mu_uint8 GetBlue(const mu_uint32 ARGB)
	{
		return (((ARGB) >> 16) & 0xff);
	}
	inline constexpr mu_uint8 GetGreen(const mu_uint32 ARGB)
	{
		return (((ARGB) >> 8) & 0xff);
	}
	inline constexpr mu_uint8 GetRed(const mu_uint32 ARGB)
	{
		return ((ARGB) & 0xff);
	}

	FORCEINLINE void PathFix(mu_string &path, mu_boolean isDir)
	{
#ifdef _WIN32
		std::replace(path.begin(), path.end(), _T('/'), _T('\\'));
#endif

		if (isDir == true && path.back() != _T('\\'))
		{
			path.push_back(_T('\\'));
		}
	}
};

struct ascii_icmp
{
	mu_boolean operator () (const mu_char *a, const mu_char *b) const
	{
		return _stricmp(a, b) < 0;
	}
	mu_boolean operator () (const mu_asciistr &a, const mu_asciistr &b) const
	{
		return _stricmp(a.c_str(), b.c_str()) < 0;
	}
};

struct unicode_icmp
{
	mu_boolean operator () (const mu_unicode *a, const mu_unicode *b) const
	{
		return _wcsicmp(a, b) < 0;
	}
	mu_boolean operator () (const mu_unicodestr &a, const mu_unicodestr &b) const
	{
		return _wcsicmp(a.c_str(), b.c_str()) < 0;
	}
};

struct textci_less
{
	struct nocase_compare
	{
		mu_boolean operator() (const mu_text& c1, const mu_text& c2) const
		{
			return mu_tolower(c1) < mu_tolower(c2);
		}
	};

	mu_boolean operator() (const mu_string & s1, const mu_string & s2) const
	{
		return std::lexicographical_compare
		(s1.begin(), s1.end(),
			s2.begin(), s2.end(),
			nocase_compare());
	}
};

#ifdef UNICODE
typedef unicode_icmp map_icmp;
#else
typedef ascii_icmp map_icmp;
#endif

#ifndef __ANDROID__
#include <windows.h>
#endif

/*
	Cross Platform to/from UTF-8 conversion functions!
	This will be used for decrease network usage and allow cross-platform compatibility
	Return buffer count used(exactly one buffer count, not characters count)
	* InputCount is characters count(not buffer size)
	* OutputSize is the max output size in bytes

	WARNING!!!
	Ensure always have an extra character slot for null-termination
*/
FORCEINLINE mu_boolean ConvertToUTF8(const mu_text *input, mu_uint32 inputCount, mu_char *output, mu_uint32 outputSize, mu_uint32 *outputCount)
{
#ifdef UNICODE
	mu_uint32 tmpCount;

	if (outputCount == nullptr)
	{
		outputCount = &tmpCount;
	}

	if (inputCount == 0)
	{
		return false;
	}

	if (input[inputCount - 1] == _T('\0'))
	{
		inputCount--;
	}

#if __ANDROID__
	*outputCount = wcstombs(output, input, inputCount);
#else
	*outputCount = WideCharToMultiByte(CP_UTF8, 0, input, inputCount, output, outputSize, nullptr, nullptr);
#endif
	
	if (output != nullptr &&
		*outputCount < outputSize)
	{
		output[*outputCount] = '\0';
	}

	return *outputCount > 0;
#else
	if (output != nullptr &&
		outputSize != inputCount)
		return false;

	mu_uint32 tmpCount;
	if (outputCount == nullptr)
	{
		outputCount = &tmpCount;
	}

	if (output != nullptr)
	{
		if (inputCount != outputSize)
		{
			return false;
		}

		mu_strncpy(output, outputSize, input, inputCount);

		if (inputCount < outputSize)
		{
			output[inputCount] = '\0';
		}
	}

	*outputCount = inputCount;

	return true;
#endif
}

FORCEINLINE mu_boolean ConvertFromUTF8(const mu_char *input, mu_uint32 inputCount, mu_text *output, mu_uint32 outputSize, mu_uint32 *outputCount)
{
#ifdef UNICODE
	mu_uint32 tmpCount;

	if (outputCount == nullptr)
	{
		outputCount = &tmpCount;
	}

	if (inputCount == 0)
	{
		return false;
	}

	if (input[inputCount - 1] == '\0')
	{
		inputCount--;
	}

#if __ANDROID__
	*outputCount = mbstowcs(output, input, inputCount);
#else
	*outputCount = MultiByteToWideChar(CP_UTF8, 0, input, inputCount, output, outputSize);
#endif

	if (output != nullptr &&
		*outputCount < outputSize)
	{
		output[*outputCount] = '\0';
	}

	return *outputCount > 0;
#else
	if (output != nullptr &&
		outputSize != inputCount)
		return false;

	mu_uint32 tmpCount;
	if (outputCount == nullptr)
	{
		outputCount = &tmpCount;
	}

	if (output != nullptr)
	{
		if (inputCount != outputSize)
		{
			return false;
		}

		mu_strncpy(output, outputSize, input, inputCount);

		if (inputCount < outputSize)
		{
			output[inputCount] = '\0';
		}
	}

	*outputCount = inputCount;

	return true;
#endif
}

FORCEINLINE mu_uint16 MakeWord(mu_uint8 Low, mu_uint8 High)
{
	return (mu_uint16)(Low) | ((mu_uint16)(High) << 8);
}

#if defined(__ANDROID__)
#define FORCED_DEBUGBREAK() raise(SIGTRAP)
#else
#define FORCED_DEBUGBREAK() __debugbreak()
#endif
//#ifndef NDEBUG
#define DEBUG_BREAK() FORCED_DEBUGBREAK()
//#else
//#define DEBUG_BREAK() (0)
//#endif

#ifndef NDEBUG
	#ifdef _MSC_VER
		#include <stdio.h>
		#define mu_assert(x) { if(!(x)){mu_printf(_T("Assert %hs:%u (") SPF_STRING _T(")\n"), __FILE__, __LINE__, _T(#x)); DEBUG_BREAK();} }
	#else
		#include <assert.h>
		#define mu_assert assert
	#endif
	#define mu_error mu_assert
#else
	#define mu_assert(x) (x)
#if __ANDROID__
	#define mu_error(x)
#else
	#define mu_error(x) mu_messagebox(nullptr, _T(x), _T("Error"), MB_OK)
#endif
#endif

#if defined(__ANDROID__) // Prevent Warnings
#include <linux/swab.h>
static inline void __swab32s_prevent(__u32 * p) {
	__swab32s(p);
}
static inline void __swab64s_prevent(__u64 * p) {
	__swab64s(p);
}
#endif

#if defined(_MSC_VER)
template<class T, mu_size L>
constexpr mu_size SizeOfArray(std::array<T, L> &I)
{
	return sizeof(T) * L;
}
template<class T, mu_size L, mu_size J>
constexpr mu_size SizeOfArray(std::array<std::array<T, J>, L> &I)
{
	return sizeof(T) * L * J;
}
template<class T, mu_size L>
FORCEINLINE void ZeroArray(std::array<T, L> &I)
{
	mu_zeromem(I.data(), SizeOfArray(I));
}
template<class T, mu_size L, mu_size J>
FORCEINLINE void ZeroArray(std::array<std::array<T, J>, L> &I)
{
	for(mu_uint32 n = 0;n < I.size();++n)
	{
		mu_zeromem(I[n].data(), SizeOfArray(I[0]));
	}
}
template<class T, mu_size L>
FORCEINLINE void FillArray(std::array<T, L> &I, const mu_uint32 StartIndex, const mu_uint32 Count, const T Value)
{
	auto Iter = I.begin() + StartIndex;
	std::fill(Iter, Iter + Count, Value);
}
#else
template<class T, auto L>
constexpr mu_size SizeOfArray(std::array<T, L> &I)
{
	return sizeof(T) * L;
}
template<class T, auto L, auto J>
constexpr mu_size SizeOfArray(std::array<std::array<T, J>, L> &I)
{
	return sizeof(T) * L * J;
}
template<class T, auto L>
FORCEINLINE void ZeroArray(std::array<T, L> &I)
{
	mu_zeromem(I.data(), SizeOfArray(I));
}
template<class T, auto L, auto J>
FORCEINLINE void ZeroArray(std::array<std::array<T, J>, L> &I)
{
	for (mu_uint32 n = 0; n < I.size(); ++n)
	{
		mu_zeromem(I[n].data(), SizeOfArray(I[0]));
	}
}
template<class T, mu_size L>
FORCEINLINE void FillArray(std::array<T, L> &I, const mu_uint32 StartIndex, const mu_uint32 Count, const T Value)
{
	auto Iter = I.begin() + StartIndex;
	std::fill(Iter, Iter + Count, Value);
}
#endif

#endif