#ifndef __MU_FUNCTIONS_HELPER_H__
#define __MU_FUNCTIONS_HELPER_H__

#pragma once

#ifndef NDEBUG
#define MU_SAFE_RELEASE(p) \
	if (p) \
				{ \
		ULONG release_result = p->Release(); \
		p = nullptr; \
		mu_assert(SUCCEEDED(release_result)); \
				}
#else
#define MU_SAFE_RELEASE(p) \
	if (p) \
			{ \
		p->Release(); \
		p = nullptr; \
			}
#endif

#define MU_SAFE_CLOSE(p) \
	if(p) \
			{ \
		CloseHandle(p); \
		p = nullptr; \
			}

#define MU_SAFE_DELETE(p) \
	if(p) \
			{ \
		delete p; \
		p = nullptr; \
			}

#define MU_SAFE_DELETE_ARRAY(p) \
	if(p) \
			{ \
		delete[] p; \
		p = nullptr; \
			}

#define MU_SAFE_FREE(p) \
	if(p) \
			{ \
		mu_free(p); \
		p = nullptr; \
			}

#if defined(_MSC_VER)
#define MU_ALIGNED_ALLOC(var, type, count, align) var = ((type*)_aligned_malloc(sizeof(type)*count, align))
#define MU_ALIGNED_FREE(p) \
	if(p) \
			{ \
		_aligned_free(p); \
		p = nullptr; \
			}
#else
#define MU_ALIGNED_ALLOC(var, type, count, align) var = new type[count];
#define MU_ALIGNED_FREE(p) \
	if(p) \
			{ \
		delete[] p; \
		p = nullptr; \
			}
#endif

#define MU_GUI_DELETE(p) p = nullptr;

#define MU_MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MU_MAX(a,b) (((a) > (b)) ? (a) : (b))

#define MU_MIN_MAX(value, minimum, maximum) MU_MIN(maximum, MU_MAX(minimum, value))

mu_unicodestr AsciiToUnicode(const mu_asciistr& s);
mu_asciistr UnicodeToAscii(const mu_unicodestr& s);

#endif