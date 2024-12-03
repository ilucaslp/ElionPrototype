#ifndef __MU_INLINE_H__
#define __MU_INLINE_H__

#pragma once

#define RENDER_COLOR        0x00000001
#define RENDER_TEXTURE      0x00000002
#define RENDER_CHROME       0x00000004
#define RENDER_METAL        0x00000008
#define RENDER_LIGHTMAP     0x00000010
#define RENDER_SHADOWMAP    0x00000020
#define RENDER_BRIGHT       0x00000040
#define RENDER_DARK         0x00000080
#define RENDER_EXTRA        0x00000100
#define RENDER_CHROME2      0x00000200
#define RENDER_WAVE			0x00000400
#define RENDER_CHROME3      0x00000800
#define RENDER_CHROME4      0x00001000
#define RENDER_NODEPTH      0x00002000
#define RENDER_CHROME5      0x00004000
#define RENDER_OIL          0x00008000
#define RENDER_CHROME6      0x00010000
#define RENDER_CHROME7      0x00020000
#define RENDER_DOPPELGANGER 0x00040000
#define RENDER_CHROME8      0x00080000
#define RENDER_BACKCULL		0x20000000
#define RENDER_FRONTCULL	0x40000000
#define RENDER_PREBLEND		0x80000000

// Class
EINLINE mu_uint8 GetBaseClass(mu_uint8 Class) { return Class & 0x07; }
EINLINE mu_uint8 GetSecondClass(mu_uint8 Class) { return (Class >> 3) & 0x01; }
EINLINE mu_uint8 GetThirdClass(mu_uint8 Class) { return (Class >> 4) & 0x01; }
EINLINE mu_boolean IsSecondClass(mu_uint8 Class) { return GetSecondClass(Class); }
EINLINE mu_boolean IsThirdClass(mu_uint8 Class) { return GetThirdClass(Class); }
EINLINE mu_boolean IsMasterLevel(mu_uint8 Class) { return IsThirdClass(Class); }
EINLINE mu_boolean IsFemale(mu_uint8 Class) { return (GetBaseClass(Class) == GAME::CLASS_ELF || GetBaseClass(Class) == GAME::CLASS_SUMMONER); }

EINLINE mu_uint8 GetStepClass(mu_uint8 Class)
{
	if (IsThirdClass(Class) == true)
	{
		return 3;
	}

	if (IsSecondClass(Class) == true)
	{
		return 2;
	}

	return 1;
}

EINLINE mu_uint8 GetCharacterClass(const mu_uint8 byClass)
{
	mu_uint8 byCharacterClass = 0;

	mu_uint8 byFirstClass = GetBaseClass(byClass);
	mu_uint8 bySecondClass = GetSecondClass(byClass);
	mu_uint8 byThirdClass = GetThirdClass(byClass);
	switch (byFirstClass)
	{
	case 0:
		{
			if (byThirdClass)
			{
				byCharacterClass = GAME::CLASS_GRANDMASTER;
			}
			else if (bySecondClass)
			{
				byCharacterClass = GAME::CLASS_SOULMASTER;
			}
			else
			{
				byCharacterClass = GAME::CLASS_WIZARD;
			}
		}

		break;
	case 1:
		{
			if (byThirdClass)
			{
				byCharacterClass = GAME::CLASS_BLADEMASTER;
			}
			else if (bySecondClass)
			{
				byCharacterClass = GAME::CLASS_BLADEKNIGHT;
			}
			else
			{
				byCharacterClass = GAME::CLASS_KNIGHT;
			}
		}
		break;
	case 2:
		{
			if (byThirdClass)
			{
				byCharacterClass = GAME::CLASS_HIGHELF;
			}
			else if (bySecondClass)
			{
				byCharacterClass = GAME::CLASS_MUSEELF;
			}
			else
			{
				byCharacterClass = GAME::CLASS_ELF;
			}
		}
		break;
	case 3:
		{
			if (byThirdClass)
			{
				byCharacterClass = GAME::CLASS_DUELMASTER;
			}
			else
			{
				byCharacterClass = GAME::CLASS_DARK;
			}
		}
		break;
	case 4:
		{
			if (byThirdClass)
			{
				byCharacterClass = GAME::CLASS_LORDEMPEROR;
			}
			else
			{
				byCharacterClass = GAME::CLASS_DARK_LORD;
			}
		}
		break;
	case 5:
		{
			if (byThirdClass)
				byCharacterClass = GAME::CLASS_DIMENSIONMASTER;
			else if (bySecondClass)
				byCharacterClass = GAME::CLASS_BLOODYSUMMONER;
			else
				byCharacterClass = GAME::CLASS_SUMMONER;
		}
		break;
	}

	return byCharacterClass;
}

EINLINE mu_uint16 GetSkinModelIndex(mu_uint8 Class)
{
	mu_uint8 bySkinIndex = 0;

	mu_uint8 byFirstClass = GetBaseClass(Class);
	mu_uint8 bySecondClass = GetSecondClass(Class);
	mu_uint8 byThirdClass = GetThirdClass(Class);

	if (byFirstClass == GAME::CLASS_WIZARD
		|| byFirstClass == GAME::CLASS_KNIGHT
		|| byFirstClass == GAME::CLASS_ELF
		|| byFirstClass == GAME::CLASS_SUMMONER)
	{
		bySkinIndex = byFirstClass + (bySecondClass + byThirdClass) * GAME::MAX_CLASS;
	}
	else
	{
		bySkinIndex = byFirstClass + (byThirdClass * 2) * GAME::MAX_CLASS;
	}

	return bySkinIndex;
}

// General
EINLINE void GetNearRandomPos(EVector3 &Pos, mu_int32 iRange, EVector3 &NewPos)
{
	VectorCopy(Pos, NewPos);
	NewPos[0] += (mu_float)(GetLargeRand(iRange * 2 + 1) - iRange);
	NewPos[1] += (mu_float)(GetLargeRand(iRange * 2 + 1) - iRange);
	NewPos[2] += (mu_float)(GetLargeRand(iRange * 2 + 1) - iRange);
}

EINLINE mu_float DirectionToFloat(mu_uint8 Dir)
{
	return ((mu_float)Dir - 1.0f) * 45.0f;
}

EINLINE mu_uint8 DirectionToUint8(mu_float Dir)
{
	return (mu_uint8)((Dir + 22.5f) / 360.0f * 8.0f + 1.0f) % 8;
}

#endif