#ifndef __MU_RESOLUTIONINFO_H__
#define __MU_RESOLUTIONINFO_H__

#pragma once

enum FONTS
{
	NORMAL_FONT = 0,
	BOLD_FONT,
	BIG_FONT,
	NUMBER_FONT,
	MAX_FONT,
};

extern const mu_text *g_FontPath[MAX_FONT];

#endif