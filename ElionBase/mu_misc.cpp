#include "stdafx.h"
#include "mu_misc.h"

#if 0
namespace MUMisc
{
	//--------------------------------------------------------------------------------------
	// Multimon API handling for OSes with or without multimon API support
	//--------------------------------------------------------------------------------------
#define MU_PRIMARY_MONITOR ((HMONITOR)0x12340042)
	typedef HMONITOR(WINAPI* LPMONITORFROMWINDOW)(HWND, DWORD);
	typedef BOOL(WINAPI* LPGETMONITORINFO)(HMONITOR, LPMONITORINFO);
	typedef HMONITOR(WINAPI* LPMONITORFROMRECT)(LPCRECT lprcScreenCoords, DWORD dwFlags);

#pragma warning( suppress : 6101 )
	BOOL WINAPI GetMonitorInfo(HMONITOR hMonitor, LPMONITORINFO lpMonitorInfo)
	{
		static mu_boolean s_bInited = false;
		static LPGETMONITORINFO s_pFnGetMonitorInfo = nullptr;
		if (!s_bInited)
		{
			s_bInited = true;
			HMODULE hUser32 = GetModuleHandle(L"USER32");
			if (hUser32)
			{
				s_pFnGetMonitorInfo = reinterpret_cast<LPGETMONITORINFO>(GetProcAddress(hUser32, "GetMonitorInfoW"));
			}
		}

		if (s_pFnGetMonitorInfo)
			return s_pFnGetMonitorInfo(hMonitor, lpMonitorInfo);

		RECT rcWork;
		if ((hMonitor == MU_PRIMARY_MONITOR) && lpMonitorInfo && (lpMonitorInfo->cbSize >= sizeof(MONITORINFO)) &&
			SystemParametersInfoA(SPI_GETWORKAREA, 0, &rcWork, 0))
		{
			lpMonitorInfo->rcMonitor.left = 0;
			lpMonitorInfo->rcMonitor.top = 0;
			lpMonitorInfo->rcMonitor.right = GetSystemMetrics(SM_CXSCREEN);
			lpMonitorInfo->rcMonitor.bottom = GetSystemMetrics(SM_CYSCREEN);
			lpMonitorInfo->rcWork = rcWork;
			lpMonitorInfo->dwFlags = MONITORINFOF_PRIMARY;
			return TRUE;
		}
		return FALSE;
	}

	HMONITOR WINAPI MonitorFromWindow(HWND hWnd, DWORD dwFlags)
	{
		static mu_boolean s_bInited = false;
		static LPMONITORFROMWINDOW s_pFnGetMonitorFromWindow = nullptr;
		if (!s_bInited)
		{
			s_bInited = true;
			HMODULE hUser32 = GetModuleHandle(L"USER32");
			if (hUser32) s_pFnGetMonitorFromWindow = reinterpret_cast<LPMONITORFROMWINDOW>(GetProcAddress(hUser32,
																										  "MonitorFromWindow"));
		}

		if (s_pFnGetMonitorFromWindow)
			return s_pFnGetMonitorFromWindow(hWnd, dwFlags);
		else
			return MU_PRIMARY_MONITOR;
	}

	HMONITOR WINAPI MonitorFromRect(LPCRECT lprcScreenCoords, DWORD dwFlags)
	{
		static mu_boolean s_bInited = false;
		static LPMONITORFROMRECT s_pFnGetMonitorFromRect = nullptr;
		if (!s_bInited)
		{
			s_bInited = true;
			HMODULE hUser32 = GetModuleHandle(L"USER32");
			if (hUser32) s_pFnGetMonitorFromRect = reinterpret_cast<LPMONITORFROMRECT>(GetProcAddress(hUser32, "MonitorFromRect"));
		}

		if (s_pFnGetMonitorFromRect)
			return s_pFnGetMonitorFromRect(lprcScreenCoords, dwFlags);
		else
			return MU_PRIMARY_MONITOR;
	}
}
#endif

void CutText(const mu_text* Text, mu_string &Text1, mu_string &Text2, mu_size Length)
{
	mu_string wstrText = Text;
	mu_size iClosestBlankFromCenter = GetClosestBlankFromCenter(wstrText);

	Text1 = wstrText.substr(iClosestBlankFromCenter + 1, mu_string::npos);
	Text2 = wstrText.substr(0, iClosestBlankFromCenter + 1);
}

mu_int32 CutStr(MUTextSimpleRender *font, const mu_text* SrcText, const mu_uint32 SrcLength, mu_text *DestText, const mu_uint32 DestLength, const mu_int32 TargetPixelWidth, const mu_int32 MaxOutLine, const mu_int32 OutStrLength, const mu_int32 FirstLineTab)
{
	if (FirstLineTab < 0)	return 0;
	if (SrcText == nullptr)
	{
		return 0;
	}

	const mu_text *pWorkStr = SrcText;
	mu_int32 iSrcTotalStrLength = mu_strlen(SrcText) + 1;
	mu_int32 iDestStrLineIndex = 0;
	mu_int32 iDestTotalStrLength = 0;
	mu_boolean bEndofSrcStr = false;

	mu_int32 iScreenRatePixelWidth = MU_TO_WINDOW(TargetPixelWidth, MU_RESOURCE->_640x480Rate[0]);

	for (iDestStrLineIndex = 0; iDestStrLineIndex < MaxOutLine; ++iDestStrLineIndex)
	{
		mu_int32 iDestStrLength = 0;
		EVector2 iDestStrPixelSize(0.0f, 0.0f);

		for (iDestStrLength = 0; iDestStrLength < OutStrLength; )
		{
			iDestStrLength += 1;
			iDestTotalStrLength += 1;

			font->GetTextExtentPoint(pWorkStr, iDestStrLength, ETA_LEFT, 1.0f, &iDestStrPixelSize);

			if (iDestTotalStrLength >= iSrcTotalStrLength)
			{
				bEndofSrcStr = true;
				break;
			}

			if (iDestStrPixelSize.x >= iScreenRatePixelWidth)
				break;
		}

		iDestStrLength -= 1;
		iDestTotalStrLength -= 1;

		mu_strncpy_s(DestText, DestLength, pWorkStr, iDestStrLength);
		DestText[iDestStrLength] = '\0';

		if (bEndofSrcStr == true)
			break;

		pWorkStr += iDestStrLength;
		DestText += OutStrLength;
	}

	return iDestStrLineIndex + 1;
}

mu_int32 SeparateTextIntoLines(const mu_text *lpszText, mu_text *lpszSeparated, mu_int32 iMaxLine, mu_int32 iLineSize)
{
	mu_int32 iLine = 0;
	const mu_text *lpLineStart = lpszText;
	mu_text *lpDst = lpszSeparated;
	const mu_text *lpSpace = nullptr;
	for (const mu_text* lpSeek = lpszText; *lpSeek; ++lpSeek, ++lpDst)
	{
		if (1 + (mu_int32)(lpSeek - lpLineStart) >= iLineSize)
		{
			if (lpSpace && (mu_int32)(lpSeek - lpSpace) < EMath::Min(10, iLineSize / 2))
			{
				lpDst -= (lpSeek - lpSpace - 1);
				lpSeek = lpSpace + 1;
			}

			lpLineStart = lpSeek;
			*lpDst = '\0';
			if (iLine >= iMaxLine - 1)
			{
				break;
			}
			++iLine;
			lpDst = lpszSeparated + iLine * iLineSize;
			lpSpace = nullptr;
		}

		*lpDst = *lpSeek;
		if (*lpSeek == ' ')
		{
			lpSpace = lpSeek;
		}
	}

	*lpDst = '\0';

	return (iLine + 1);
}