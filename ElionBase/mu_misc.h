#ifndef __MU_MISC_H__
#define __MU_MISC_H__

#pragma once

typedef struct
{
	EVector3 StartPos;
	EVector3 XAxis;
	EVector3 YAxis;
	EVector3 ZAxis;
} OBB_t;

namespace MUMisc
{
#if 0
	HMONITOR WINAPI MonitorFromWindow(_In_ HWND hWnd, _In_ DWORD dwFlags);
	HMONITOR WINAPI MonitorFromRect(_In_ LPCRECT lprcScreenCoords, _In_ DWORD dwFlags);
	BOOL WINAPI GetMonitorInfo(_In_ HMONITOR hMonitor, _Out_ LPMONITORINFO lpMonitorInfo);
#endif

	template<typename T, typename T2>
	mu_boolean IsFlag(T &value, T2 flag)
	{
		return (value & (T)flag) == (T)flag;
	}

	template<typename T, typename T2>
	void MarkFlag(T &value, T2 flag)
	{
		value |= (T)flag;
	}

	template<typename T, typename T2>
	void UnmarkFlag(T &value, T2 flag)
	{
		value &= ~((T)flag);
	}
};

class MUTextSimpleRender;

void CutText(const mu_text* Text, mu_string &Text1, mu_string &Text2, mu_size Length);
mu_int32 CutStr(MUTextSimpleRender *font, const mu_text* SrcText, const mu_uint32 SrcLength, mu_text *DestText, const mu_uint32 DestLength, const mu_int32 TargetPixelWidth, const mu_int32 MaxOutLine, const mu_int32 OutStrLength, const mu_int32 FirstLineTab = 0);
mu_int32 SeparateTextIntoLines(const mu_text *lpszText, mu_text *lpszSeparated, mu_int32 iMaxLine, mu_int32 iLineSize);

#endif