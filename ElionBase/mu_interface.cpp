#include "stdafx.h"
#include "mu_interface.h"

mu_int32 TextNum = 0;
mu_int32 SkipNum = 0;
mu_unicode TextList[TEXTLIST_COUNT][TEXTLIST_LENGTH];
mu_uint32 TextListColor[TEXTLIST_COUNT];
mu_boolean TextBold[TEXTLIST_COUNT];
EVector2 Size[TEXTLIST_COUNT];

mu_unicode MacroText[10][256];

mu_int32 MouseX = 0;
mu_int32 MouseY = 0;
mu_int32 BackMouseX = MouseX;
mu_int32 BackMouseY = MouseY;
mu_uint32 MouseRButtonPress = 0;
mu_int32 MouseUpdateTime = 0;
mu_int32 MouseUpdateTimeMax = 6;

const mu_float AutoMouseLimitTime = (1.0f * 60.0f * 60.0f);
mu_float LButtonPopTime = 0.0f;
mu_float LButtonPressTime = 0.0f;
mu_float RButtonPopTime = 0.0f;
mu_float RButtonPressTime = 0.0f;

mu_boolean LogOut = false;

mu_int32 SelectMonster = 0;
mu_int32 SelectModel = 0;
mu_int32 SelectMapping = 0;
mu_int32 SelectColor = 0;
mu_int32 SelectWall = 0;
mu_float SelectMappingAngle = 0.0f;
mu_int32 SelectedItem = -1;
mu_int32 SelectedNpc = -1;
mu_int32 SelectedCharacter = -1;
mu_int32 SelectedHero = -1;
mu_int32 SelectedOperate = -1;
mu_int32 Attacking = -1;
mu_int32 g_iFollowCharacter = -1;

mu_int32 RequestedAttackCharacter = -1;

mu_int32 HeroTile;
mu_int32 TargetNpc = -1;
mu_int32 TargetType;
mu_int32 TargetX;
mu_int32 TargetY;
mu_float TargetAngle;

mu_boolean g_bAutoGetItem = false;
mu_boolean bCheckNPC = false;
mu_boolean DontMove = false;

mu_boolean g_Ashies = false;
mu_boolean g_GMObservation = false;

mu_int32 LoadingWorld = 0;

EVector3 MousePosition;
EVector3 MouseTarget;
EVector3 MouseMin;
EVector3 MouseMax;
EVector3 MouseDirection;
EVector3 MouseAbsDirection;
EFloat MouseAbsMin;
mu_uint8 MouseAbsMinIndex;

mu_float CollisionDistance;
EVector3 CollisionPosition;

mu_boolean SelectFlag;
mu_float SelectXF;
mu_float SelectYF;

mu_int32 ItemHelp = 0;

void RenderTipTextList(mu_float sx, mu_float sy, mu_int32 TextNum, mu_int32 Tab, mu_int32 iSort, mu_int32 iRenderPoint, mu_boolean bUseBG)
{
	mu_float onePoint = MU_TO_WINDOW(1.0f, MU_RESOURCE->_640x480Rate[0]);
	mu_float borderWidth = onePoint * 2.0f;

	MUTextSimpleRender *Font;
	EVector2 TextSize(0.0f, 0.0f);
	mu_int32 TextLine = 0;
	mu_int32 EmptyLine = 0;
	mu_float fWidth = 0.0f;
	mu_float fHeight = 0.0f;

	sx = MU_TO_WINDOW(sx, MU_RESOURCE->_640x480Rate[0]);
	sy = MU_TO_WINDOW(sy, MU_RESOURCE->_640x480Rate[1]);

	for (mu_int32 i = 0; i < TextNum; ++i)
	{
		if (TextList[i][0] == '\0')
		{
			TextNum = i;
			break;
		}

		if (TextBold[i])
		{
			Font = FONT_BOLD;
		}
		else
		{
			Font = FONT_NORMAL;
		}

		Font->GetTextExtentPoint(TextList[i], mu_strlen(TextList[i]), ETA_LEFT, 1.0f, &TextSize);

		if (fWidth < TextSize.x)
			fWidth = TextSize.x;

		if (TextList[i][0] == _T('\n'))
		{
			fHeight += TextSize.y * 0.5f;
			++EmptyLine;
		}
		else
		{
			fHeight += TextSize.y + borderWidth;
			++TextLine;
		}
	}

	if (Tab > 0)
	{
		mu_float TmpWidth = MU_TO_WINDOW((mu_float)Tab / MU_RESOURCE->_640x480Rate[0] * 2.0f, MU_RESOURCE->_640x480Rate[0]);

		if (fWidth < TmpWidth)
		{
			fWidth = TmpWidth;
		}
	}

	fWidth += onePoint * 4.0f;
	fWidth = EMath::Ceil(fWidth);

	mu_float iPos_x = sx - fWidth * 0.5f;
	if (iPos_x < 0) iPos_x = 0;
	if (bUseBG == true && TextNum > 0)
	{
		if (iPos_x + fWidth + onePoint * 3.0f >= MU_RESOURCE->GetWindowWidth())
		{
			iPos_x = (mu_float)MU_RESOURCE->GetWindowWidth() - fWidth - onePoint * 3.0f;
		}
	}
	else
	{
		if (iPos_x + fWidth >= MU_RESOURCE->GetWindowWidth())
		{
			iPos_x = (mu_float)MU_RESOURCE->GetWindowWidth() - fWidth;
		}
	}

	mu_float fsx = iPos_x;
	mu_float fsy = onePoint;

	switch (iRenderPoint)
	{
	case STRP_BOTTOMCENTER:
		{
			fsy = sy - fHeight;
		}
		break;

	case STRP_ITEMINFO:
		{
#if 0 // Alternative (Bottom instead of Upside) - BUGGED(Mouse is over text, so it is annoying)
			const mu_float ScreenHeight = MU_TO_WINDOW(420.0f, MU_RESOURCE->_640x480Rate[1]);
			fsy = sy;
			if (fsy + fHeight > ScreenHeight)
			{
				fsy = ScreenHeight - (fsy + fHeight);
			}
#else
			fsy = sy - fHeight;

			if (fsy < 0.0f)
			{
				fsy = 0.0f;
// 				mu_float extraHeight = (fHeight - MU_RESOURCE->GetWindowHeight());
// 
// 				if (extraHeight > 0.0f)
// 					fsy = sy - extraHeight;
// 				else
// 					fsy = sy;
			}
#endif
		}
		break;

	case STRP_ITEMINFO2:
		{
			fsy = sy;

			if (fsy + fHeight >= MU_RESOURCE->GetWindowHeight())
			{
				fsy = MU_RESOURCE->GetWindowHeight() - fHeight;
			}
		}
		break;

	default:
		{
			fsy = sy;
		}
		break;
	}

	if (fsy < 0.0f)
		fsy = 0.0f;

	if (bUseBG == true && TextNum > 0)
	{
		static UISpriteColor *ToolTipBack = nullptr;

		if (ToolTipBack == nullptr)
		{
			ToolTipBack = new UISpriteColor(g_pNewUISystem, nullptr, MU::MakeARGB(0, 0, 0, 204), 640.0f, 480.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ToolTipBack->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		}

		ToolTipBack->SetPosition(iPos_x - onePoint * 1.5f, fsy, false);
		ToolTipBack->SetDimensions(fWidth + onePoint * 3.0f, fHeight, false);
		ToolTipBack->Draw();
	}

	mu_uint32 textColor;
	mu_uint32 backColor;

	switch (iSort)
	{
	case ETA_CENTER:
		fsx = EMath::Ceil(fsx + (fWidth * 0.5f));
		break;
	case ETA_RIGHT:
		fsx = EMath::Ceil(fsx + fWidth);
		break;
	}

	for (mu_int32 i = 0; i < TextNum; ++i)
	{
		if (TextBold[i])
		{
			Font = FONT_BOLD;
		}
		else
		{
			Font = FONT_NORMAL;
		}

		mu_float fHeight = 0;
		if (TextList[i][0] == _T('\n') || (TextList[i][0] == _T(' ') && TextList[i][1] == _T('\0')))
		{
			Font->GetTextExtentPoint(TextList[i], mu_strlen(TextList[i]), ETA_LEFT, 1.0f, &TextSize);
			fHeight = (mu_float)TextSize.y * (TextList[i][0] == _T('\n') ? 0.5f : 1.0f);
		}
		else
		{
			textColor = MU::MakeARGB(255, 255, 255, 255);
			switch (TextListColor[i])
			{
			case TEXT_COLOR_WHITE:
			case TEXT_COLOR_DARKRED:
			case TEXT_COLOR_DARKBLUE:
			case TEXT_COLOR_DARKYELLOW:
				textColor = MU::MakeARGB(255, 255, 255, 255);
				break;
			case TEXT_COLOR_BLUE:
				textColor = MU::MakeARGB(127, 178, 255, 255);
				break;
			case TEXT_COLOR_GRAY:
				textColor = MU::MakeARGB(102, 102, 102, 255);
				break;
			case TEXT_COLOR_GREEN_BLUE:
				textColor = MU::MakeARGB(0, 255, 0, 255);
				break;
			case TEXT_COLOR_RED:
				textColor = MU::MakeARGB(255, 51, 25, 255);
				break;
			case TEXT_COLOR_YELLOW:
				textColor = MU::MakeARGB(255, 204, 25, 255);
				break;
			case TEXT_COLOR_GREEN:
				textColor = MU::MakeARGB(25, 255, 127, 255);
				break;
			case TEXT_COLOR_PURPLE:
				textColor = MU::MakeARGB(255, 25, 255, 255);
				break;
			case TEXT_COLOR_REDPURPLE:
				textColor = MU::MakeARGB(204, 127, 204, 255);
				break;
			case TEXT_COLOR_VIOLET:
				textColor = MU::MakeARGB(178, 102, 255, 255);
				break;
			case TEXT_COLOR_ORANGE:
				textColor = MU::MakeARGB(229, 107, 10, 255);
				break;
			}

			if (TEXT_COLOR_DARKRED == TextListColor[i])
			{
				backColor = MU::MakeARGB(160, 0, 0, 255);
			}
			else if (TEXT_COLOR_DARKBLUE == TextListColor[i])
			{
				backColor = MU::MakeARGB(0, 0, 160, 255);
			}
			else if (TEXT_COLOR_DARKYELLOW == TextListColor[i])
			{
				backColor = MU::MakeARGB(160, 102, 0, 255);
			}
			else if (TEXT_COLOR_GREEN_BLUE == TextListColor[i])
			{
				backColor = MU::MakeARGB(60, 60, 200, 255);
			}
			else
			{
				backColor = MU::MakeARGB(0, 0, 0, 0);
			}

			MUText *buffer = nullptr;

			switch (iSort)
			{
			case ETA_LEFT:
				Font->DrawStringC2D(TEXTINDEX::BEGIN_RENDERTOOLTIP + i, fsx, fsy, 1.0f, ETA_LEFT, ETA_BOTTOM, 2.0f, textColor, backColor, TextList[i], fWidth, 0.0f, &buffer);
				break;
			case ETA_CENTER:
				Font->DrawStringC2D(TEXTINDEX::BEGIN_RENDERTOOLTIP + i, fsx, fsy, 1.0f, ETA_CENTER, ETA_BOTTOM, 2.0f, textColor, backColor, TextList[i], fWidth, 0.0f, &buffer);
				break;
			case ETA_RIGHT:
				Font->DrawStringC2D(TEXTINDEX::BEGIN_RENDERTOOLTIP + i, fsx, fsy, 1.0f, ETA_RIGHT, ETA_BOTTOM, 2.0f, textColor, backColor, TextList[i], fWidth, 0.0f, &buffer);
				break;
			}

			fHeight = buffer->GetSize().y;
		}

		fsy += fHeight;
	}
}

mu_int32 FindHotKey(mu_int32 Skill)
{
	mu_int32 SkillIndex = 0;

	for (mu_int32 i = 0; i < MAX_MAGIC; ++i)
	{
		if (CharacterAttribute->Skill[i] == Skill)
		{
			SkillIndex = i;
			break;
		}
	}

	return SkillIndex;
}

mu_boolean CheckTile(CHARACTER *c, OBJECT *o, mu_float Range)
{
	mu_float dx = o->Position[0] - (TargetX*g_TerrainScale + g_TerrainScale*0.5f);
	mu_float dy = o->Position[1] - (TargetY*g_TerrainScale + g_TerrainScale*0.5f);

	mu_float Distance = EMath::Sqrt(dx*dx + dy*dy);
	if (Distance <= g_TerrainScale*Range)
	{
		return true;
	}

	return false;
}

mu_boolean CheckWall(mu_int32 sx1, mu_int32 sy1, mu_int32 sx2, mu_int32 sy2)
{
	mu_int32 Index = TERRAIN_INDEX_REPEAT(sx1, sy1);

	mu_int32 nx1, ny1, d1, d2, len1, len2;
	mu_int32 px1 = sx2 - sx1;
	mu_int32 py1 = sy2 - sy1;

	if (px1 < 0) { px1 = -px1; nx1 = -1; }
	else nx1 = 1;

	if (py1 < 0) { py1 = -py1; ny1 = -TERRAIN_SIZE; }
	else ny1 = TERRAIN_SIZE;

	if (px1 > py1) { len1 = px1; len2 = py1; d1 = ny1; d2 = nx1; }
	else { len1 = py1; len2 = px1; d1 = nx1; d2 = ny1; }

	mu_int32 error = 0, count = 0;
	mu_int32 _type = (SelectedCharacter >= 0 ? CharactersClient[SelectedCharacter].Object.Type : 0);

	if ((_type != MODEL_MONSTER01 + 183 &&
		_type != MODEL_MONSTER01 + 184 &&
		_type != MODEL_MONSTER01 + 186 &&
		_type != MODEL_MONSTER01 + 187))
	{
		do
		{
			if (TerrainWall[Index] >= TW_NOMOVE &&
				(TerrainWall[Index] & TW_ACTION) != TW_ACTION &&
				(TerrainWall[Index] & TW_HEIGHT) != TW_HEIGHT &&
				(TerrainWall[Index] & TW_CAMERA_UP) != TW_CAMERA_UP)
			{
				return false;
			}

			error += len2;

			if (error > len1 / 2)
			{
				Index += d1;
				error -= len1;
			}

			Index += d2;
		} while (++count <= len1);
	}

	return true;
}

mu_boolean CheckAttack_Fenrir(CHARACTER* c)
{
	if (g_pPickedItem.GetItem() != nullptr)
	{
		return false;
	}

	if (InChaosCastle() == true && c != Hero)
	{
		return true;
	}
	else if (IsStrifeMap(GET_WORLD) && c != Hero && c->m_byGensInfluence != Hero->m_byGensInfluence)
	{
		if (((mu_strcmp(GuildMark[Hero->GuildMarkIndex].GuildName.c_str(), GuildMark[c->GuildMarkIndex].GuildName.c_str()) == 0) ||
			(PartyManager::IsPartyMember(SelectedCharacter)))
			&& MU_INPUT->IsCtrlPressing() == true)
		{
			return true;
		}
		else if ((mu_strcmp(GuildMark[Hero->GuildMarkIndex].GuildName.c_str(), GuildMark[c->GuildMarkIndex].GuildName.c_str()) != 0) &&
			!PartyManager::IsPartyMember(SelectedCharacter))
		{
			return true;
		}
	}
	if (c->Object.Kind == KIND_MONSTER)
	{
		if (EnableGuildWar && EnableSoccer)
		{
			return true;
		}
		else if (EnableGuildWar)
		{
			return false;
		}

		return true;
	}
	else if (c->Object.Kind == KIND_PLAYER)
	{
		if (battleCastle::IsBattleCastleStart())
		{
			if ((Hero->EtcPart == PARTS_ATTACK_KING_TEAM_MARK || Hero->EtcPart == PARTS_ATTACK_TEAM_MARK
				|| Hero->EtcPart == PARTS_ATTACK_KING_TEAM_MARK2
				|| Hero->EtcPart == PARTS_ATTACK_KING_TEAM_MARK3
				|| Hero->EtcPart == PARTS_ATTACK_TEAM_MARK2
				|| Hero->EtcPart == PARTS_ATTACK_TEAM_MARK3
				) &&
				(c->EtcPart == PARTS_DEFENSE_KING_TEAM_MARK || c->EtcPart == PARTS_DEFENSE_TEAM_MARK))
			{
				if (!g_isCharacterBuff((&c->Object), eBuff_Cloaking))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if ((Hero->EtcPart == PARTS_DEFENSE_KING_TEAM_MARK || Hero->EtcPart == PARTS_DEFENSE_TEAM_MARK)
				&& (c->EtcPart == PARTS_ATTACK_KING_TEAM_MARK || c->EtcPart == PARTS_ATTACK_TEAM_MARK
					|| c->EtcPart == PARTS_ATTACK_KING_TEAM_MARK2 || c->EtcPart == PARTS_ATTACK_KING_TEAM_MARK3
					|| c->EtcPart == PARTS_ATTACK_TEAM_MARK2 || c->EtcPart == PARTS_ATTACK_TEAM_MARK3))
			{
				if (!g_isCharacterBuff((&c->Object), eBuff_Cloaking))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if (g_isCharacterBuff((&Hero->Object), eBuff_CastleRegimentAttack1)
				|| g_isCharacterBuff((&Hero->Object), eBuff_CastleRegimentAttack2)
				|| g_isCharacterBuff((&Hero->Object), eBuff_CastleRegimentAttack3))
			{
				OBJECT* o = &c->Object;
				if (!g_isCharacterBuff(o, eBuff_CastleRegimentAttack1)
					&& !g_isCharacterBuff(o, eBuff_CastleRegimentAttack2)
					&& !g_isCharacterBuff(o, eBuff_CastleRegimentAttack3))
				{
					if (!g_isCharacterBuff((&c->Object), eBuff_Cloaking))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			else if (g_isCharacterBuff((&Hero->Object), eBuff_CastleRegimentDefense))
			{
				OBJECT* o = &c->Object;

				if (!g_isCharacterBuff(o, eBuff_CastleRegimentDefense))
				{
					if (!g_isCharacterBuff((&c->Object), eBuff_Cloaking))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
		}

		if (c->GuildRelationShip == GAME::GR_RIVAL || c->GuildRelationShip == GAME::GR_RIVALUNION)
		{
			return true;
		}

		if (EnableGuildWar && c->PK >= PVP_MURDERER2 && c->GuildMarkIndex != -1
			&& mu_strcmp(GuildMark[Hero->GuildMarkIndex].GuildName.c_str(), GuildMark[c->GuildMarkIndex].GuildName.c_str()) == 0)
		{
			return  false;
		}
		else if (g_DuelMgr.IsDuelEnabled())
		{
			if (g_DuelMgr.IsDuelPlayer(c, DUEL_ENEMY))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (EnableGuildWar)
		{
			if (c->GuildTeam == 2 && c != Hero)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (c->PK >= PVP_MURDERER2 || (MU_INPUT->IsCtrlPressing() == true && c != Hero))
		{
			return true;
		}
		else if (g_CursedTemple.IsCursedTemple() && !g_CursedTemple.IsPartyMember(SelectedCharacter))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return false;
}

mu_boolean CheckAttack()
{
	if (g_pPickedItem.GetItem() != nullptr)
	{
		return false;
	}

	if (SelectedCharacter == -1)
	{
		return false;
	}

	CHARACTER *c = &CharactersClient[SelectedCharacter];

	if (InChaosCastle() == true && c != Hero)
	{
		return true;
	}
	else if (IsStrifeMap(GET_WORLD) && c != Hero && c->m_byGensInfluence != Hero->m_byGensInfluence)
	{
		if (g_pCommandWindow->GetCommandSelected() != -1)
		{
			return false;
		}

		if (MU_INPUT->IsKeyPressing(SDL_SCANCODE_MENU) == true)
		{
			return false;
		}

		if (MU_INPUT->IsCtrlPressing() == true)
		{
			if (EnableGuildWar)
			{
				if (c->GuildTeam == 2 && c != Hero)
					return true;
				else
					return false;
			}
			else
				return true;
		}
		else if (GuildMark[Hero->GuildMarkIndex].GuildIndex == GuildMark[c->GuildMarkIndex].GuildIndex)
		{
			if (PartyManager::IsPartyMember(SelectedCharacter))
			{
				return false;
			}

			if (EnableGuildWar)
			{
				if (c->GuildTeam == 2 && c != Hero)
					return true;
				else
					return false;
			}

			if (c->GuildRelationShip == GAME::GR_NONE)
				return true;
			else
				return false;
		}
		else if ((c->GuildRelationShip == GAME::GR_UNION) || (c->GuildRelationShip == GAME::GR_UNIONMASTER))
		{
			return false;
		}
		else if (EnableGuildWar)
		{
			if (c->GuildTeam == 2 && c != Hero)
				return true;
			else
				return false;
		}
		else if (PartyManager::IsPartyMember(SelectedCharacter))
		{
			if ((c->GuildRelationShip == GAME::GR_RIVAL) || (c->GuildRelationShip == GAME::GR_RIVALUNION))
			{
				return true;
			}
			else
				return false;
		}
		else
		{
			return true;
		}
	}

	if (c->Object.Kind == KIND_MONSTER)
	{
		if (EnableGuildWar && EnableSoccer)
		{
			return true;
		}
		else if (EnableGuildWar)
		{
			return false;
		}
		else if (g_isCharacterBuff((&Hero->Object), eBuff_DuelWatch))
		{
			return false;
		}

		return true;
	}
	else if (c->Object.Kind == KIND_PLAYER)
	{
		if (battleCastle::IsBattleCastleStart())
		{
			if ((Hero->EtcPart == PARTS_ATTACK_KING_TEAM_MARK || Hero->EtcPart == PARTS_ATTACK_TEAM_MARK
				|| Hero->EtcPart == PARTS_ATTACK_KING_TEAM_MARK2 || Hero->EtcPart == PARTS_ATTACK_KING_TEAM_MARK3
				|| Hero->EtcPart == PARTS_ATTACK_TEAM_MARK2 || Hero->EtcPart == PARTS_ATTACK_TEAM_MARK3)
				&& (c->EtcPart == PARTS_DEFENSE_KING_TEAM_MARK || c->EtcPart == PARTS_DEFENSE_TEAM_MARK))
			{
				if (!g_isCharacterBuff((&c->Object), eBuff_Cloaking))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if ((Hero->EtcPart == PARTS_DEFENSE_KING_TEAM_MARK || Hero->EtcPart == PARTS_DEFENSE_TEAM_MARK)
				&& (c->EtcPart == PARTS_ATTACK_KING_TEAM_MARK || c->EtcPart == PARTS_ATTACK_TEAM_MARK
					|| c->EtcPart == PARTS_ATTACK_KING_TEAM_MARK2 || c->EtcPart == PARTS_ATTACK_KING_TEAM_MARK3
					|| c->EtcPart == PARTS_ATTACK_TEAM_MARK2 || c->EtcPart == PARTS_ATTACK_TEAM_MARK3))
			{
				if (!g_isCharacterBuff((&c->Object), eBuff_Cloaking))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if (g_isCharacterBuff((&Hero->Object), eBuff_CastleRegimentAttack1)
				|| g_isCharacterBuff((&Hero->Object), eBuff_CastleRegimentAttack2)
				|| g_isCharacterBuff((&Hero->Object), eBuff_CastleRegimentAttack3))
			{
				OBJECT* o = &c->Object;

				if (!g_isCharacterBuff(o, eBuff_CastleRegimentAttack1)
					&& !g_isCharacterBuff(o, eBuff_CastleRegimentAttack2)
					&& !g_isCharacterBuff(o, eBuff_CastleRegimentAttack3))
				{
					if (!g_isCharacterBuff((&c->Object), eBuff_Cloaking))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			else if (g_isCharacterBuff((&Hero->Object), eBuff_CastleRegimentDefense))
			{
				OBJECT* o = &c->Object;

				if (!g_isCharacterBuff(o, eBuff_CastleRegimentDefense))
				{
					if (!g_isCharacterBuff((&c->Object), eBuff_Cloaking))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
		}

		if (c->GuildRelationShip == GAME::GR_RIVAL || c->GuildRelationShip == GAME::GR_RIVALUNION)
		{
			return true;
		}

		if (EnableGuildWar && c->PK >= PVP_MURDERER2 && c->GuildMarkIndex != -1
			&& GuildMark[Hero->GuildMarkIndex].GuildIndex == GuildMark[c->GuildMarkIndex].GuildIndex)
		{
			return  false;
		}
		else if (g_DuelMgr.IsDuelEnabled())
		{
			if (g_DuelMgr.IsDuelPlayer(c, DUEL_ENEMY))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (g_isCharacterBuff((&Hero->Object), eBuff_DuelWatch))
		{
			return false;
		}
		else if (EnableGuildWar)
		{
			if (c->GuildTeam == 2 && c != Hero)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (c->PK >= PVP_MURDERER2 || (MU_INPUT->IsCtrlPressing() == true && c != Hero))
		{
			return true;
		}
		else if (g_CursedTemple.IsCursedTemple() && !g_CursedTemple.IsPartyMember(SelectedCharacter))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return false;
}

mu_int32 getTargetCharacterKey(CHARACTER* c, mu_int32 selected)
{
	if (g_pPickedItem.GetItem() != nullptr)
	{
		return -1;
	}

	if (c != Hero || selected < 0 || selected >= MAX_CHARACTERS_CLIENT)
	{
		return -1;
	}

	CHARACTER *sc = &CharactersClient[selected];

	if (InChaosCastle() == true)
	{
		return sc->Key;
	}

	if (EnableGuildWar && sc->PK >= PVP_MURDERER2 && sc->GuildMarkIndex != -1 && GuildMark[Hero->GuildMarkIndex].GuildIndex == GuildMark[sc->GuildMarkIndex].GuildIndex)
		return  -1;
	else if (g_DuelMgr.IsDuelEnabled())
	{
		if (g_DuelMgr.IsDuelPlayer(sc, DUEL_ENEMY))
			return sc->Key;
		else
			return -1;
	}
	else if (sc->GuildRelationShip == GAME::GR_RIVAL || sc->GuildRelationShip == GAME::GR_RIVALUNION)
	{
		return sc->Key;
	}
	else if (EnableGuildWar)
	{
		if (sc->GuildTeam == 2 && sc != Hero)
			return sc->Key;
		else
			return -1;
	}
	else if (IsStrifeMap(GET_WORLD) && sc != Hero && sc->m_byGensInfluence != Hero->m_byGensInfluence
		&& MU_INPUT->IsKeyPressing(SDL_SCANCODE_MENU) == false)
	{
		if (sc->GuildRelationShip == GAME::GR_NONE && !PartyManager::IsPartyMember(SelectedCharacter))
		{
			return sc->Key;
		}
		else if (GuildMark[Hero->GuildMarkIndex].GuildIndex == GuildMark[c->GuildMarkIndex].GuildIndex ||
			PartyManager::IsPartyMember(SelectedCharacter))
		{
			if (MU_INPUT->IsCtrlPressing() == true)
				return sc->Key;
			else
				return -1;
		}
	}
	else if (sc->PK >= PVP_MURDERER2 || (MU_INPUT->IsCtrlPressing() == true && sc != Hero))
	{
		return sc->Key;
	}
	else if (g_CursedTemple.IsCursedTemple())
	{
		if (g_CursedTemple.IsPartyMember(selected))
		{
			return -1;
		}
		else
		{
			return sc->Key;
		}
	}

	return -1;
}

mu_boolean CheckTarget(const mu_uint32 ThreadIndex, CHARACTER *c)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (SelectedCharacter != -1)
	{
		TargetX = (mu_int32)(CharactersClient[SelectedCharacter].Object.Position[0] * g_TerrainScaleRev);
		TargetY = (mu_int32)(CharactersClient[SelectedCharacter].Object.Position[1] * g_TerrainScaleRev);
		VectorCopy(CharactersClient[SelectedCharacter].Object.Position, c->TargetPosition);
		return true;
	}
	else
	{
		InitDetectTerrainCollision;
		if (DetectTerrainCollision(ThreadIndex, SelectXF, SelectYF))
		{
			VectorCopy(ThreadInfo.CollisionPosition, c->TargetPosition);
			TargetX = (mu_int32)(c->TargetPosition[0] * g_TerrainScaleRev);
			TargetY = (mu_int32)(c->TargetPosition[1] * g_TerrainScaleRev);
			return true;
		}
	}
	return false;
}

mu_int32 GetScreenWidth()
{
	mu_int32 iWidth = 0;

	if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_INVENTORY)
		&& (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_CHARACTER)
			|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_NPCSHOP)
			|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_STORAGE)
			|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_MIXINVENTORY)
			|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_TRADE)
			|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_MYSHOP_INVENTORY)
			|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_PURCHASESHOP_INVENTORY)
			|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_LUCKYCOIN_REGISTRATION)))
	{
		iWidth = 640 - (190 * 2);
	}
	else if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_CHARACTER)
		&& (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_MYQUEST)
			|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_QUEST_PROGRESS_ETC)))
	{
		iWidth = 640 - (190 * 2);
	}
	else if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_CHARACTER)
		&& g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_PET)
		)
	{
		iWidth = 640 - (190 * 2);
	}
	else if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_REFINERY))
	{
		iWidth = 640 - (190 * 2);
	}
	else if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_INVENTORY)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_CHARACTER)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_PARTY)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_NPCGUILDMASTER)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_GUILDINFO)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_GUARDSMAN)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_SENATUS)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_GATEKEEPER)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_MYQUEST)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_SERVERDIVISION)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_COMMAND)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_NPCQUEST)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_GATESWITCH)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_CATAPULT)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_DEVILSQUARE)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_BLOODCASTLE)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_GOLD_BOWMAN)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_GOLD_BOWMAN_LENA)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_DUELWATCH)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_NPC_DIALOGUE)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_QUEST_PROGRESS)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_QUEST_PROGRESS_ETC)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_EMPIREGUARDIAN_NPC)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_DOPPELGANGER_NPC)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_UNITEDMARKETPLACE_NPC_JULIA)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_GENSRANKING)
		|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_HELPERSETUP))
	{
		iWidth = 640 - 190;
	}
	else
	{
		iWidth = 640;
	}

	return iWidth;
}

void CheckChatText(const mu_unicode *Text)
{
	CHARACTER *c = Hero;
	OBJECT *o = &c->Object;
	if (mu_strstr(Text, GlobalText[270]) != nullptr ||
		mu_strstr(Text, GlobalText[271]) != nullptr ||
		mu_strstr(Text, GlobalText[272]) != nullptr ||
		mu_strstr(Text, GlobalText[273]) != nullptr ||
		mu_strstr(Text, GlobalText[274]) != nullptr ||
		mu_strstr(Text, GlobalText[275]) != nullptr ||
		mu_strstr(Text, GlobalText[276]) != nullptr ||
		mu_strstr(Text, GlobalText[277]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_GREETING1, AT_GREETING1);
#if 0 // FIX
		SendRequestAction(AT_GREETING1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[278]) != nullptr ||
		mu_strstr(Text, GlobalText[279]) != nullptr ||
		mu_strstr(Text, GlobalText[280]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_GOODBYE1, AT_GOODBYE1);
#if 0 // FIX
		SendRequestAction(AT_GOODBYE1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[281]) != nullptr ||
		mu_strstr(Text, GlobalText[282]) != nullptr ||
		mu_strstr(Text, GlobalText[283]) != nullptr ||
		mu_strstr(Text, GlobalText[284]) != nullptr ||
		mu_strstr(Text, GlobalText[285]) != nullptr ||
		mu_strstr(Text, GlobalText[286]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_CLAP1, AT_CLAP1);
#if 0 // FIX
		SendRequestAction(AT_CLAP1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[287]) != nullptr ||
		mu_strstr(Text, GlobalText[288]) != nullptr ||
		mu_strstr(Text, GlobalText[289]) != nullptr ||
		mu_strstr(Text, GlobalText[290]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_GESTURE1, AT_GESTURE1);
#if 0 // FIX
		SendRequestAction(AT_GESTURE1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[292]) != nullptr ||
		mu_strstr(Text, GlobalText[293]) != nullptr ||
		mu_strstr(Text, GlobalText[294]) != nullptr ||
		mu_strstr(Text, GlobalText[295]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_DIRECTION1, AT_DIRECTION1);
#if 0 // FIX
		SendRequestAction(AT_DIRECTION1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[296]) != nullptr ||
		mu_strstr(Text, GlobalText[297]) != nullptr ||
		mu_strstr(Text, GlobalText[298]) != nullptr ||
		mu_strstr(Text, GlobalText[299]) != nullptr ||
		mu_strstr(Text, GlobalText[300]) != nullptr ||
		mu_strstr(Text, GlobalText[301]) != nullptr ||
		mu_strstr(Text, GlobalText[302]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_UNKNOWN1, AT_UNKNOWN1);
#if 0 // FIX
		SendRequestAction(AT_UNKNOWN1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, _T(";")) != nullptr ||
		mu_strstr(Text, GlobalText[303]) != nullptr ||
		mu_strstr(Text, GlobalText[304]) != nullptr ||
		mu_strstr(Text, GlobalText[305]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_AWKWARD1, AT_AWKWARD1);
#if 0 // FIX
		SendRequestAction(AT_AWKWARD1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, _T("T_T")) != nullptr ||
		mu_strstr(Text, GlobalText[306]) != nullptr ||
		mu_strstr(Text, GlobalText[307]) != nullptr ||
		mu_strstr(Text, GlobalText[308]) != nullptr ||
		mu_strstr(Text, GlobalText[309]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_CRY1, AT_CRY1);
#if 0 // FIX
		SendRequestAction(AT_CRY1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, _T("-.-")) != nullptr ||
		mu_strstr(Text, _T("-_-")) != nullptr ||
		mu_strstr(Text, GlobalText[310]) != nullptr ||
		mu_strstr(Text, GlobalText[311]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_SEE1, AT_SEE1);
#if 0 // FIX
		SendRequestAction(AT_SEE1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, _T("^^")) != nullptr ||
		mu_strstr(Text, _T("^.^")) != nullptr ||
		mu_strstr(Text, _T("^_^")) != nullptr ||
		mu_strstr(Text, GlobalText[312]) != nullptr ||
		mu_strstr(Text, GlobalText[313]) != nullptr ||
		mu_strstr(Text, GlobalText[314]) != nullptr ||
		mu_strstr(Text, GlobalText[315]) != nullptr ||
		mu_strstr(Text, GlobalText[316]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_SMILE1, AT_SMILE1);
#if 0 // FIX
		SendRequestAction(AT_SMILE1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[318]) != nullptr ||
		mu_strstr(Text, GlobalText[319]) != nullptr ||
		mu_strstr(Text, GlobalText[320]) != nullptr ||
		mu_strstr(Text, GlobalText[321]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_CHEER1, AT_CHEER1);
#if 0 // FIX
		SendRequestAction(AT_CHEER1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[322]) != nullptr ||
		mu_strstr(Text, GlobalText[323]) != nullptr ||
		mu_strstr(Text, GlobalText[324]) != nullptr ||
		mu_strstr(Text, GlobalText[325]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_WIN1, AT_WIN1);
#if 0 // FIX
		SendRequestAction(AT_WIN1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[326]) != nullptr ||
		mu_strstr(Text, GlobalText[327]) != nullptr ||
		mu_strstr(Text, GlobalText[328]) != nullptr ||
		mu_strstr(Text, GlobalText[329]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_SLEEP1, AT_SLEEP1);
#if 0 // FIX
		SendRequestAction(AT_SLEEP1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[330]) != nullptr ||
		mu_strstr(Text, GlobalText[331]) != nullptr ||
		mu_strstr(Text, GlobalText[332]) != nullptr ||
		mu_strstr(Text, GlobalText[333]) != nullptr ||
		mu_strstr(Text, GlobalText[334]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_COLD1, AT_COLD1);
#if 0 // FIX
		SendRequestAction(AT_COLD1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[335]) != nullptr ||
		mu_strstr(Text, GlobalText[336]) != nullptr ||
		mu_strstr(Text, GlobalText[337]) != nullptr ||
		mu_strstr(Text, GlobalText[338]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_AGAIN1, AT_AGAIN1);
#if 0 // FIX
		SendRequestAction(AT_AGAIN1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[339]) != nullptr ||
		mu_strstr(Text, GlobalText[340]) != nullptr ||
		mu_strstr(Text, GlobalText[341]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_RESPECT1, AT_RESPECT1);
#if 0 // FIX
		SendRequestAction(AT_RESPECT1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[342]) != nullptr ||
		mu_strstr(Text, GlobalText[343]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_SALUTE1, AT_SALUTE1);
#if 0 // FIX
		SendRequestAction(AT_SALUTE1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[344]) != nullptr ||
		mu_strstr(Text, GlobalText[345]) != nullptr ||
		mu_strstr(Text, GlobalText[346]) != nullptr ||
		mu_strstr(Text, GlobalText[347]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_RUSH1, AT_RUSH1);
#if 0 // FIX
		SendRequestAction(AT_RUSH1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[783]) != nullptr ||
		mu_strstr(Text, _T("hustle")) != nullptr)
	{
		SetActionClass(c, o, PLAYER_HUSTLE, AT_HUSTLE);
#if 0 // FIX
		SendRequestAction(AT_HUSTLE, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[291]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_PROVOCATION, AT_PROVOCATION);
#if 0 // FIX
		SendRequestAction(AT_PROVOCATION, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[317]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_CHEERS, AT_CHEERS);
#if 0 // FIX
		SendRequestAction(AT_CHEERS, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[348]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_LOOK_AROUND, AT_LOOK_AROUND);
#if 0 // FIX
		SendRequestAction(AT_LOOK_AROUND, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[2041]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_KOREA_HANDCLAP, AT_HANDCLAP);
#if 0 // FIX
		SendRequestAction(AT_HANDCLAP, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[2042]) != nullptr)
	{
		SetActionClass(c, o, PLAYER_POINT_DANCE, AT_POINTDANCE);
#if 0 // FIX
		SendRequestAction(AT_POINTDANCE, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
	}
	else if (mu_strstr(Text, GlobalText[2228]) != nullptr)
	{
		ITEM* pItem_rr = &CharacterMachine->Equipment[GAME::EQUIPMENT_RING_RIGHT];
		ITEM* pItem_rl = &CharacterMachine->Equipment[GAME::EQUIPMENT_RING_LEFT];

		if (pItem_rr->Type == ITEM_HELPER + 40 || pItem_rl->Type == ITEM_HELPER + 40)
		{
			if (GetLargeRand(2) == 0)
			{
				SetAction(o, PLAYER_JACK_1);
#if 0 // FIX
				SendRequestAction(AT_JACK1, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
			}
			else
			{
				SetAction(o, PLAYER_JACK_2);
#if 0 // FIX
				SendRequestAction(AT_JACK2, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
			}

			o->m_iAnimation = 0;
		}

	}
	else if (mu_strstr(Text, GlobalText[2243]) != nullptr)
	{
		ITEM* pItem_rr = &CharacterMachine->Equipment[GAME::EQUIPMENT_RING_RIGHT];
		ITEM* pItem_rl = &CharacterMachine->Equipment[GAME::EQUIPMENT_RING_LEFT];

		if (pItem_rr->Type == ITEM_HELPER + 41 || pItem_rl->Type == ITEM_HELPER + 41)
		{
			if (o->CurrentAction != PLAYER_SANTA_1 && o->CurrentAction != PLAYER_SANTA_2)
			{
				mu_int32 i = GetLargeRand(3);
				if (GetLargeRand(2))
				{
					SetAction(o, PLAYER_SANTA_1);
#if 0 // FIX
					SendRequestAction(AT_SANTA1_1 + i, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
					PlayBackground(SOUND_XMAS_JUMP_SANTA + i);
				}
				else
				{
					SetAction(o, PLAYER_SANTA_2);
#if 0 // FIX
					SendRequestAction(AT_SANTA2_1 + i, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
					PlayBackground(SOUND_XMAS_TURN);
				}

				g_XmasEvent.CreateXmasEventEffect(0, c, o, i);
			}

			o->m_iAnimation = 0;
		}
	}
}

mu_boolean CheckMacroLimit(const mu_unicode* Text)
{
	const mu_unicode *string = Text + 3;
	mu_int32 length;

	length = mu_strlen(GlobalText[258]);
	if (mu_strcmp(string, GlobalText[258]) == 0 ||
		mu_strcmp(string, GlobalText[259]) == 0 ||
		mu_stricmp(string, _T("/trade")) == 0)
	{
		return  true;
	}

	if (mu_strcmp(string, GlobalText[256]) == 0 ||
		mu_stricmp(string, _T("/party")) == 0 ||
		mu_stricmp(string, _T("/pt")) == 0)
	{
		return  true;
	}

	if (mu_strcmp(string, GlobalText[254]) == 0 ||
		mu_stricmp(string, _T("/guild")) == 0)
	{
		return  true;
	}

	if (mu_strcmp(string, GlobalText[248]) == 0 ||
		mu_stricmp(string, _T("/GuildWar")) == 0)
	{
		return  true;
	}

	if (mu_strcmp(string, GlobalText[249]) == 0 ||
		mu_stricmp(string, _T("/BattleSoccer")) == 0)
	{
		return  true;
	}

	return  false;
}

mu_boolean IsIllegalMovementByUsingMsg(const mu_unicode *szChatText)
{
	mu_boolean bCantFly = false;
	mu_boolean bCantSwim = false;
	mu_boolean bEquipChangeRing = false;

	mu_boolean bMoveAtlans = false;
	mu_boolean bMoveIcarus = false;

	mu_string szChatTextUpperChars = szChatText;
	std::transform(szChatTextUpperChars.begin(), szChatTextUpperChars.end(), szChatTextUpperChars.begin(), mu_tolower);

	mu_int16 pEquipedRightRingType = (&CharacterMachine->Equipment[GAME::EQUIPMENT_RING_RIGHT])->Type;
	mu_int16 pEquipedLeftRingType = (&CharacterMachine->Equipment[GAME::EQUIPMENT_RING_LEFT])->Type;
	mu_int16 pEquipedHelperType = (&CharacterMachine->Equipment[GAME::EQUIPMENT_HELPER])->Type;
	mu_int16 pEquipedWingType = (&CharacterMachine->Equipment[GAME::EQUIPMENT_WING])->Type;

	if ((pEquipedWingType == -1 && pEquipedHelperType != ITEM_HELPER + 3 &&
		pEquipedHelperType != ITEM_HELPER + 37 && pEquipedHelperType != ITEM_HELPER + 4) ||
		pEquipedHelperType == ITEM_HELPER + 2)
	{
		bCantFly = true;
	}

	if (pEquipedHelperType == ITEM_HELPER + 2 || pEquipedHelperType == ITEM_HELPER + 3)
	{
		bCantSwim = true;
	}

	if (g_ChangeRingMgr.CheckMoveMap(pEquipedLeftRingType, pEquipedRightRingType))
	{
		bEquipChangeRing = true;
	}

	if ((mu_strstr(szChatTextUpperChars.c_str(), _T("/MOVE")) != nullptr) ||
		(mu_strlen(GlobalText[260]) > 0 && mu_strstr(szChatTextUpperChars.c_str(), GlobalText[260]) != nullptr))
	{
#if 0 // FIX
		std::list<CMoveCommandData::MOVEINFODATA*> m_listMoveInfoData;
		m_listMoveInfoData = CMoveCommandData::GetInstance()->GetMoveCommandDatalist();

		std::list<CMoveCommandData::MOVEINFODATA*>::iterator li = m_listMoveInfoData.begin();

		while (li != m_listMoveInfoData.end())
		{
			mu_string cMapNameUpperChars = (*li)->_ReqInfo.szSubMapName;
			std::transform(cMapNameUpperChars.begin(), cMapNameUpperChars.end(), cMapNameUpperChars.begin(), mu_tolower);

			if (mu_strstr(szChatText, ((*li)->_ReqInfo.szMainMapName)) != nullptr ||
				mu_strstr(szChatTextUpperChars, cMapNameUpperChars.c_str()) != nullptr)
				break;

			++li;
		}

		if (li != m_listMoveInfoData.end())
		{
			if (mu_stricmp((*li)->_ReqInfo.szMainMapName, GlobalText[37]) == 0)
			{
				bMoveAtlans = true;
			}
			else if (mu_stricmp((*li)->_ReqInfo.szMainMapName, GlobalText[55]) == 0)
			{
				bMoveIcarus = true;
			}
		}
#endif
	}

	if (bCantSwim && bMoveAtlans)
	{
		g_pChatListBox->AddText(_T(""), GlobalText[261], TYPE_SYSTEM_MESSAGE);
		return true;
	}

	if ((bCantFly || bEquipChangeRing) && bMoveIcarus)
	{
		g_pChatListBox->AddText(_T(""), GlobalText[263], TYPE_SYSTEM_MESSAGE);
		return true;
	}

	return false;
}

mu_boolean CheckAbuseFilter(mu_string Text, mu_boolean bCheckSlash)
{
	if (bCheckSlash == true)
	{
		if (Text[0] == _T('/'))
		{
			return false;
		}
	}

	std::transform(Text.begin(), Text.end(), Text.begin(), mu_tolower);

	for (auto it = AbuseFilter.begin();it != AbuseFilter.end(); ++it)
	{
		if (Text.find(*it, 0) != mu_string::npos)
		{
			return true;
		}
	}

	return false;
}

mu_boolean CheckAbuseNameFilter(mu_string Text)
{
	std::transform(Text.begin(), Text.end(), Text.begin(), mu_tolower);

	for (auto it = AbuseNameFilter.begin(); it != AbuseNameFilter.end(); ++it)
	{
		if (Text.find(*it, 0) != mu_string::npos)
		{
			return true;
		}
	}

	return false;
}

mu_boolean CheckCommand(const mu_unicode *Text, mu_boolean isMacroText)
{
	if (isMacroText == false &&
		LogOut == false)
	{
		mu_unicode Name[256];
		mu_uint32 textSize = 0;
		while (textSize < mu_countof(Name) - 1 &&
			Text[textSize] != _T(' ') &&
			Text[textSize] != _T('\0'))
		{
			Name[textSize] = Text[textSize];
			++textSize;
		}

		Name[textSize] = _T('\0');

		if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_STORAGE) == false)
		{
			if (mu_strcmp(Name, GlobalText[258]) == 0 ||
				mu_strcmp(Name, GlobalText[259]) == 0 ||
				mu_stricmp(Name, _T("/trade")) == 0)
			{
				g_pCommandWindow->CommandTrade();
				return true;
			}
		}

		if (mu_strcmp(Name, GlobalText[260]) == 0 ||
			mu_stricmp(Name, _T("/move")) == 0)
		{
			if (IsGMCharacter() == true)
			{
				return false;
			}

			return true;
		}

		if (mu_strcmp(Text, GlobalText[688]) == 0)
		{
			return false;
		}

		if (mu_strcmp(Text, GlobalText[1117]) == 0 || mu_stricmp(Text, _T("/personalshop")) == 0)
		{
			if (InChaosCastle() == true)
			{
				g_pChatListBox->AddText(_T(""), GlobalText[1150], TYPE_SYSTEM_MESSAGE);
				return false;
			}

			if (CharacterAttribute->Level >= 6)
			{
				g_pNewUISystem->Show(GAMEUI::INTERFACE_MYSHOP_INVENTORY);
			}
			else
			{
				mu_unicode szError[48];
				mu_sprintf_s(szError, mu_countof(szError), GlobalText[1123], 6);
				g_pChatListBox->AddText(_T(""), szError, TYPE_SYSTEM_MESSAGE);
			}

			return true;
		}

		if (mu_strstr(Text, GlobalText[1118]) != nullptr ||
			mu_strstr(Text, _T("/purchase")) != nullptr)
		{
			if (InChaosCastle() == true)
			{
				g_pChatListBox->AddText(_T(""), GlobalText[1150], TYPE_SYSTEM_MESSAGE);
				return false;
			}

			if (IsStrifeMap(GET_WORLD))
			{
				g_pChatListBox->AddText(_T(""), GlobalText[3147], TYPE_SYSTEM_MESSAGE);
				return false;
			}

			if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_NPCSHOP)
				|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_STORAGE)
				|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_TRADE)
				|| g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_MIXINVENTORY))
			{
				g_pChatListBox->AddText(_T(""), GlobalText[1121], TYPE_SYSTEM_MESSAGE);
				return false;
			}

			mu_unicode szCmd[24];
			mu_unicode szId[ACCOUNT_MAXIMUM];
			mu_sscanf(Text, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), szCmd, (mu_uint32)mu_countof(szCmd), szId, (mu_uint32)mu_countof(szId));

			if (SelectedCharacter != -1)
			{
#if 0 // FIX
				CHARACTER *c = &CharactersClient[SelectedCharacter];
				OBJECT *o = &c->Object;
				if (o->Kind == KIND_PLAYER && c != Hero && (o->Type == MODEL_PLAYER || c->Change))
					SendRequestOpenPersonalShop(c->Key, c->ID);
#endif
			}

			return true;
		}

		if (mu_strcmp(Text, GlobalText[1136]) == 0)
		{
#if 0 // FIX
			ShowShopTitles();
#endif
			g_pChatListBox->AddText(_T(""), GlobalText[1138], TYPE_SYSTEM_MESSAGE);
		}

		if (mu_strcmp(Text, GlobalText[1137]) == 0)
		{
#if 0 // FIX
			HideShopTitles();
#endif
			g_pChatListBox->AddText(_T(""), GlobalText[1139], TYPE_ERROR_MESSAGE);
		}

		if (mu_strcmp(Text, GlobalText[908]) == 0 ||
			mu_stricmp(Text, _T("/duelstart")) == 0)
		{
			g_pCommandWindow->CommandDuel();
			return true;
		}

		if (mu_strcmp(Text, GlobalText[909]) == 0 ||
			mu_stricmp(Text, _T("/duelend")) == 0)
		{
			if (InChaosCastle() == true)
			{
				g_pChatListBox->AddText(_T(""), GlobalText[1150], TYPE_SYSTEM_MESSAGE);
				return false;
			}

			if (g_DuelMgr.IsDuelEnabled())
			{
#if 0 // FIX
				SendRequestDuelEnd();
#endif
			}
		}

		if (mu_strcmp(Text, GlobalText[254]) == 0 ||
			mu_stricmp(Text, _T("/guild")) == 0)
		{
			g_pCommandWindow->CommandGuild();
			return true;
		}

		if (!mu_strcmp(Text, GlobalText[1354]) || !mu_stricmp(Text, _T("/union")) ||
			!mu_strcmp(Text, GlobalText[1356]) || !mu_stricmp(Text, _T("/rival")) ||
			!mu_strcmp(Text, GlobalText[1357]) || !mu_stricmp(Text, _T("/rivaloff")))
		{
			if (InChaosCastle() == true)
			{
				g_pChatListBox->AddText(_T(""), GlobalText[1150], TYPE_SYSTEM_MESSAGE);
				return false;
			}

			if (Hero->GuildStatus == GAME::G_NONE)
			{
				g_pChatListBox->AddText(_T(""), GlobalText[1355], TYPE_SYSTEM_MESSAGE);
				return true;
			}

			if (SelectedCharacter != -1)
			{
				CHARACTER *c = &CharactersClient[SelectedCharacter];
				OBJECT *o = &c->Object;
				EFloat dis = EMath::Distance(o->Position, Hero->Object.Position);
				if (o->Kind == KIND_PLAYER && c != Hero && (o->Type == MODEL_PLAYER || c->Change) &&
					dis <= 300.0f)
				{
					if (!mu_strcmp(Text, GlobalText[1354]) || !mu_stricmp(Text, _T("/union")))
					{
#if 0 // FIX
						SendRequestGuildRelationShip(0x01, 0x01, HIBYTE(CharactersClient[SelectedCharacter].Key), LOBYTE(CharactersClient[SelectedCharacter].Key));
#endif
					}
					else if (!mu_strcmp(Text, GlobalText[1356]) || !mu_stricmp(Text, _T("/rival")))
					{
#if 0 // FIX
						SendRequestGuildRelationShip(0x02, 0x01, HIBYTE(CharactersClient[SelectedCharacter].Key), LOBYTE(CharactersClient[SelectedCharacter].Key));
#endif
					}
					else
					{
						SetAction(&Hero->Object, PLAYER_RESPECT1);
#if 0 // FIX
						SendRequestAction(AT_RESPECT1, ((mu_uint8)((Hero->Object.Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
						SendRequestGuildRelationShip(0x02, 0x02, HIBYTE(CharactersClient[SelectedCharacter].Key), LOBYTE(CharactersClient[SelectedCharacter].Key));
#endif
					}
				}
			}
			return true;
		}
		if (mu_strcmp(Text, GlobalText[256]) == 0 ||
			mu_stricmp(Text, _T("/party")) == 0 ||
			mu_stricmp(Text, _T("/pt")) == 0)
		{
			g_pCommandWindow->CommandParty();
			return true;
		}

		if (mu_stricmp(Text, _T("/charactername")) == 0)
		{
			if (IsGMCharacter() == true)
			{
				g_GMObservation = !g_GMObservation;
				return true;
			}
		}

		for (mu_int32 i = 0; i < 10; ++i)
		{
			mu_unicode Name[256];
			if (i != 9)
				mu_sprintf_s(Name, mu_countof(Name), _T("/%d"), i + 1);
			else
				mu_sprintf_s(Name, mu_countof(Name), _T("/%d"), 0);

			if (Text[0] == Name[0] && Text[1] == Name[1])
			{
				if (CheckMacroLimit(Text) == true)
				{
					return  false;
				}

				mu_strcpy_s(MacroText[i], mu_countof(MacroText[0]), &Text[3]);

				PlayBackground(SOUND_CLICK01);
				return true;
			}
		}

		const mu_unicode *lpszFilter = _T("/filter");
		mu_size filterLength = mu_strlen(GlobalText[753]);
		if ((filterLength > 0 && mu_strncmp(Text, GlobalText[753], filterLength) == 0)
			|| (mu_strncmp(Text, lpszFilter, mu_strlen(lpszFilter)) == 0))
		{
			g_pChatListBox->SetFilterText(Text);
		}
	}

	if (IsIllegalMovementByUsingMsg(Text))
		return true;

	for (mu_int32 i = 0; i < ITEM_ETC + MAX_ITEM_INDEX; ++i)
	{
		ITEM_ATTRIBUTE *p = &ItemAttribute[i];

		if (p->Width != 0)
		{
			mu_unicode Name[256];
			mu_sprintf_s(Name, mu_countof(Name), _T("/") SPF_STRING _T(""), p->Name.c_str());
			if (mu_stricmp(Text, Name) == 0)
			{
				g_csItemOption.ClearOptionHelper();

				g_pNewUISystem->Show(GAMEUI::INTERFACE_ITEM_EXPLANATION);

				ItemHelp = i;
				PlayBackground(SOUND_CLICK01);
				return true;
			}
		}
	}

	g_csItemOption.CheckRenderOptionHelper(Text);
					
	return (IsIllegalMovementByUsingMsg(Text)) ? true : false;
}

EINLINE mu_boolean InsideTest(EVector3 &dirPos, EVector3 vIn[4], mu_int32 flag, mu_float type)
{
	if (type > 0.0f)
		flag <<= 3;

	EVector3 v[4];

	for (mu_uint32 k = 0; k < 3; ++k)
	{
		v[k + 1] = vIn[k] - dirPos;
	}
	v[0] = v[3];

	mu_int32 i = 1;
	mu_int32 j = 0;

	switch (flag)
	{
	case 1:
		for (; j < 3; ++i, ++j)
		{
			mu_float d = v[i][1] * v[j][2] - v[j][1] * v[i][2];
			if (d <= 0.0f)
				return false;
		}
		break;
	case 2:
		for (; j < 3; ++i, ++j)
		{
			mu_float d = v[i][2] * v[j][0] - v[j][2] * v[i][0];
			if (d <= 0.0f)
				return false;
		}
		break;
	case 4:
		for (; j < 3; ++i, ++j)
		{
			mu_float d = v[i][0] * v[j][1] - v[j][0] * v[i][1];
			if (d <= 0.0f)
				return false;
		}
		break;
	case 8:
		for (; j < 3; ++i, ++j)
		{
			mu_float d = v[i][1] * v[j][2] - v[j][1] * v[i][2];
			if (d >= 0.0f)
				return false;
		}
		break;
	case 16:
		for (; j < 3; ++i, ++j)
		{
			mu_float d = v[i][2] * v[j][0] - v[j][2] * v[i][0];
			if (d >= 0.0f)
				return false;
		}
		break;
	case 32:
		for (; j < 3; ++i, ++j)
		{
			mu_float d = v[i][0] * v[j][1] - v[j][0] * v[i][1];
			if (d >= 0.0f)
				return false;
		}
		break;
	}

	return true;
}

mu_boolean CollisionDetectLineToFace(EVector3 v[4], EVector3 &Normal, mu_boolean Collision, mu_float &Distance, EVector3 &CollisionPosition)
{
	mu_float a = DotProduct(MouseDirection, Normal);
	if (a >= 0.0f) return false;
	mu_float b = DotProduct(MousePosition, Normal) - DotProduct(v[0], Normal);
	mu_float t = -b / a;
	if (t >= 0.0f && t <= Distance)
	{
		EVector3 dirPos = MouseDirection * t + MousePosition;

		switch (MouseAbsMinIndex)
		{
		case 0:
			{
				if (!(dirPos[1] >= MouseMin[1] && dirPos[1] <= MouseMax[1] &&
					dirPos[2] >= MouseMin[2] && dirPos[2] <= MouseMax[2]))
				{
					return false;
				}
			}
			break;

		case 1:
			{
				if (!(dirPos[2] >= MouseMin[2] && dirPos[2] <= MouseMax[2] &&
					dirPos[0] >= MouseMin[0] && dirPos[0] <= MouseMax[0]))
				{
					return false;
				}
			}
			break;
			
		default:
			{
				if (!(dirPos[0] >= MouseMin[0] && dirPos[0] <= MouseMax[0] &&
					dirPos[1] >= MouseMin[1] && dirPos[1] <= MouseMax[1]))
				{
					return false;
				}
			}
			break;
		}

		mu_boolean isInside = false;
		EVector3 absNormal = EMath::Abs(Normal);
		if (absNormal[0] >= 0.5f)
		{
			isInside = InsideTest(dirPos, v, 1, Normal[0]);
		}
		else if (absNormal[1] >= 0.5f)
		{
			isInside = InsideTest(dirPos, v, 2, Normal[1]);
		}
		else
		{
			isInside = InsideTest(dirPos, v, 4, Normal[2]);
		}
		if (isInside == false) return false;
		if (Collision)
		{
			Distance = t;
			CollisionPosition = dirPos;
		}
		return true;
	}
	return false;
}

mu_boolean ProjectLineBox(EVector3 ax, EVector3 p1, EVector3 p2, OBB_t obb)
{
	mu_float P1 = DotProduct(ax, p1);
	mu_float P2 = DotProduct(ax, p2);

	mu_float mx1 = EMath::Max(P1, P2);
	mu_float mn1 = EMath::Min(P1, P2);

	mu_float ST = DotProduct(ax, obb.StartPos);
	mu_float Q1 = DotProduct(ax, obb.XAxis);
	mu_float Q2 = DotProduct(ax, obb.YAxis);
	mu_float Q3 = DotProduct(ax, obb.ZAxis);

	mu_float mx2 = ST;
	mu_float mn2 = ST;

	if (Q1 > 0)	mx2 += Q1; else mn2 += Q1;
	if (Q2 > 0)	mx2 += Q2; else mn2 += Q2;
	if (Q3 > 0) mx2 += Q3; else mn2 += Q3;

	if (mn1 > mx2) return false;
	if (mn2 > mx1) return false;

	return true;
}

mu_boolean CollisionDetectLineToOBB(EVector3 p1, EVector3 p2, OBB_t obb)
{
	EVector3 e1;
	EVector3 eq11, eq12, eq13;

	VectorSubtract(p2, p1, e1);

	CrossProduct(e1, obb.XAxis, eq11);
	CrossProduct(e1, obb.YAxis, eq12);
	CrossProduct(e1, obb.ZAxis, eq13);

	if (!ProjectLineBox(eq11, p1, p2, obb)) return false;
	if (!ProjectLineBox(eq12, p1, p2, obb)) return false;
	if (!ProjectLineBox(eq13, p1, p2, obb)) return false;

	if (!ProjectLineBox(obb.XAxis, p1, p2, obb)) return false;
	if (!ProjectLineBox(obb.YAxis, p1, p2, obb)) return false;
	if (!ProjectLineBox(obb.ZAxis, p1, p2, obb)) return false;

	return true;
}

mu_boolean DetectTerrainCollision(const mu_uint32 ThreadIndex, mu_int32 x, mu_int32 y)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_uint32 TerrainIndex1 = TERRAIN_INDEX_REPEAT(x, y);
	mu_uint32 TerrainIndex2 = TERRAIN_INDEX_REPEAT(x + 1, y);
	mu_uint32 TerrainIndex3 = TERRAIN_INDEX_REPEAT(x + 1, y + 1);
	mu_uint32 TerrainIndex4 = TERRAIN_INDEX_REPEAT(x, y + 1);

	mu_float xf = x * g_TerrainScale;
	mu_float yf = y * g_TerrainScale;

	EVector3 TerrainVertex[3];
	Vector(xf, yf, BackTerrainHeight[TerrainIndex1], TerrainVertex[0]);
	Vector(xf + g_TerrainScale, yf, BackTerrainHeight[TerrainIndex2], TerrainVertex[1]);
	Vector(xf + g_TerrainScale, yf + g_TerrainScale, BackTerrainHeight[TerrainIndex3], TerrainVertex[2]);

	mu_boolean Success = CollisionDetectLineToFace(TerrainVertex, g_TerrainCollisionNormal[TerrainIndex1][0], true, ThreadInfo.CollisionDistance, ThreadInfo.CollisionPosition);
	if (Success == false)
	{
		TerrainVertex[1] = TerrainVertex[2];
		Vector(xf, yf + g_TerrainScale, BackTerrainHeight[TerrainIndex4], TerrainVertex[2]);

		Success = CollisionDetectLineToFace(TerrainVertex, g_TerrainCollisionNormal[TerrainIndex1][1], true, ThreadInfo.CollisionDistance, ThreadInfo.CollisionPosition);
	}

	if (Success == true)
	{
		ThreadInfo.SelectFlag = true;
		ThreadInfo.SelectXF = x;
		ThreadInfo.SelectYF = y;
	}

	return Success;
}

mu_int32 SelectItem()
{
	for (mu_int32 i = 0; i < MAX_ITEMS; ++i)
	{
		OBJECT *o = &Items[i].Object;
		if (o->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == true)
		{
			o->Flags.Set(OBJECT::eOF_LIGHTENABLE);
			Vector(0.2f, 0.2f, 0.2f, o->Light);
		}
	}

	mu_float Luminosity = 1.5f;

	for (mu_int32 i = 0; i < MAX_ITEMS; ++i)
	{
		OBJECT *o = &Items[i].Object;
		if (o->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == true)
		{
			if (CollisionDetectLineToOBB(MousePosition, MouseTarget, o->OBB))
			{
				o->Flags.Clear(OBJECT::eOF_LIGHTENABLE);
				Vector(Luminosity, Luminosity, Luminosity, o->Light);
				return i;
			}
		}
	}
	return -1;
}

mu_int32 SelectCharacter(mu_uint8 Kind)
{
	mu_boolean Main = true;
	if (g_CurrentScene == CHARACTER_SCENE)
		Main = false;

	EVector3 Light;
	Vector(0.8f, 0.8f, 0.8f, Light);
	mu_int32 iSelected = -1;
	mu_float fNearestDist = FLT_MAX;
	
	for (mu_int32 i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
	{
		CHARACTER *c = &CharactersClient[i];
		OBJECT *o = &c->Object;
		if (o->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == false)
		{
			continue;
		}

		if (o->Alpha <= 0.0f)
		{
			continue;
		}

		if ((Kind&o->Kind) == o->Kind)
		{
			o->Flags.Set(OBJECT::eOF_LIGHTENABLE);

			switch (c->Level)
			{
			case 0:Vector(0.2f, 0.2f, 0.2f, o->Light); break;
			case 1:Vector(-0.4f, -0.4f, -0.4f, o->Light); break;
			case 2:Vector(0.2f, -0.6f, -0.6f, o->Light); break;
			case 3:Vector(1.5f, 1.5f, 1.5f, o->Light); break;
			case 4:Vector(0.3f, 0.2f, -0.5f, o->Light); break;
			}

			if (c->PK >= PVP_MURDERER2)
			{
				Vector(-0.4f, -0.4f, -0.4f, o->Light);
			}
		}

		if (c->Dead == 0 && !g_isCharacterBuff(o, eBuff_CrywolfNPCHide))
		{
			if (Main && c == Hero) continue;

			if (c->m_bIsSelected == false)
			{
				continue;
			}

			if ((Kind&o->Kind) == o->Kind)
			{
				if (o->Type == MODEL_SMELTING_NPC)
				{
					o->OBB.StartPos[2] += 300.0f;
				}
				else if (o->Type == MODEL_MONSTER01 + 118 || o->Type == MODEL_MONSTER01 + 119)
				{
					o->OBB.StartPos[2] += 200.0f;
				}
				else if (o->Type == MODEL_KANTURU2ND_ENTER_NPC)
				{
					o->OBB.StartPos[0] -= 100.0f;
					o->OBB.StartPos[2] += 100.0f;
					o->OBB.XAxis[0] += 100.0f;
					o->OBB.ZAxis[2] += 100.0f;
				}

				if (CollisionDetectLineToOBB(MousePosition, MouseTarget, o->OBB))
				{
					EVector3 vSub;
					VectorSubtract(o->Position, MU_CAMERA->GetCameraPosition(), vSub);

					mu_float fNewDist = DotProduct(vSub, vSub);

					if (fNewDist < fNearestDist)
					{
						mu_boolean bCanTalk = true;
						if (GET_WORLD == WD_0LORENCIA || GET_WORLD == WD_2DEVIAS)
						{
							mu_int32 Index = ((mu_int32)o->Position[1] / (mu_int32)g_TerrainScale) * 256 + ((mu_int32)o->Position[0] / (mu_int32)g_TerrainScale);
							if ((GET_WORLD == WD_0LORENCIA && TerrainMappingLayer1[Index] == 4) ||
								(GET_WORLD == WD_2DEVIAS && TerrainMappingLayer1[Index] == 3))
							{
								if (TerrainMappingLayer1[Index] != HeroTile
									&& (GET_WORLD == WD_2DEVIAS && HeroTile != 11)
									)
									bCanTalk = false;
							}
						}
						if (bCanTalk == true)
						{
							iSelected = i;
							fNearestDist = fNewDist;
						}
					}
				}
			}
		}
	}

	return iSelected;
}

mu_int32 SelectOperate()
{
	for (mu_int32 i = 0; i < g_OperatesCount; ++i)
	{
		MUOperates &operate = g_Operates[i];

		if (operate.Owner->Flags.Get(OBJECT::eOF_VISIBLE) == false ||
			operate.Owner->HiddenMesh != -1) continue;

		Vector(0.2f, 0.2f, 0.2f, operate.Owner->Light);
	}

	if (battleCastle::IsBattleCastleStart() && GET_WORLD == WD_30BATTLECASTLE)
	{
		return -1;
	}

	for (mu_int32 i = 0; i < g_OperatesCount; ++i)
	{
		MUOperates &operate = g_Operates[i];

		if (operate.Owner->Flags.Get(OBJECT::eOF_VISIBLE) == false) continue;

		OBJECT *o = operate.Owner;
		if (CollisionDetectLineToOBB(MousePosition, MouseTarget, o->OBB))
		{
			Vector(1.5f, 1.5f, 1.5f, o->Light);
			return i;
		}
	}
	return -1;
}

void SelectObjects()
{
	mu_uint8 CKind_1, CKind_2;

	if (MU_RESOURCE->IsAutoAttack() == true
		&& GET_WORLD != WD_6STADIUM
		&& InChaosCastle() == false)
	{
		if (SelectedCharacter < 0 || SelectedCharacter >= MAX_CHARACTERS_CLIENT + 1)
		{
			SelectedCharacter = -1;
			Attacking = -1;
		}
		else
		{
			CHARACTER* sc = &CharactersClient[SelectedCharacter];

			if (sc->Dead > 0 || sc->Object.Kind != KIND_MONSTER)
			{
				SelectedCharacter = -1;
				Attacking = -1;
			}

			if (Attacking != -1)
			{
				if (MU_INPUT->Is3DMousePressing(MOUSE_BUTTON_LEFT) == true ||
					MU_INPUT->Is3DMousePressing(MOUSE_BUTTON_RIGHT) == true ||
						Hero->Dead > 0)
				{
					SelectedCharacter = -1;
				}
			}
			else
			{
				SelectedCharacter = -1;
			}
		}
	}
	else
	{
		SelectedCharacter = -1;
		Attacking = -1;
	}

	SelectedItem = -1;
	SelectedNpc = -1;
	SelectedOperate = -1;
	if ((MU_INPUT->IsMouseOverInterface() == false ||
		(g_pCommandWindow->GetCommandSelected() != -1 &&
			g_pNewUISystem->GetFocused() != nullptr &&
			g_pNewUISystem->GetFocused()->GetObjectID() == GAMEUI::MESSAGE_COMMANDWINDOW_PLAYER_BUTTON)) &&
		MU_CAMERA->IsDragging() == false &&
		CheckMouseIn(0, 0, MU_TO_WINDOW(GetScreenWidth(), MU_RESOURCE->_640x480Rate[0]), MU_TO_WINDOW(429, MU_RESOURCE->_640x480Rate[1])))
	{
		if (MU_INPUT->IsKeyPressing(SDL_SCANCODE_MENU) == true)
		{
			if (g_pPickedItem.GetItem() == nullptr)
			{
				SelectedItem = SelectItem();
			}

			if (SelectedItem == -1)
			{
				SelectedNpc = SelectCharacter(KIND_NPC);
				if (SelectedNpc == -1)
				{
					SelectedCharacter = SelectCharacter(KIND_MONSTER | KIND_EDIT);
					if (SelectedCharacter == -1)
					{
						SelectedCharacter = SelectCharacter(KIND_PLAYER);
						if (SelectedCharacter == -1)
						{
							SelectedOperate = SelectOperate();
						}
					}
				}
			}
		}
		else
		{
			CKind_1 = KIND_MONSTER | KIND_EDIT;
			CKind_2 = KIND_PLAYER;

			if (GetBaseClass(Hero->Class) == GAME::CLASS_ELF || GetBaseClass(Hero->Class) == GAME::CLASS_WIZARD)
			{
				mu_int32 Skill = CharacterAttribute->Skill[Hero->CurrentSkill];

				if (((Skill >= AT_SKILL_HEALING) && (Skill <= AT_SKILL_ATTACK))
					|| ((AT_SKILL_HEAL_UP <= Skill) && (Skill <= AT_SKILL_HEAL_UP + 4))
					|| ((AT_SKILL_ATT_POWER_UP <= Skill) && (Skill <= AT_SKILL_ATT_POWER_UP + 4))
					|| ((AT_SKILL_DEF_POWER_UP <= Skill) && (Skill <= AT_SKILL_DEF_POWER_UP + 4))
					|| (Skill == AT_SKILL_TELEPORT_B)
					|| (Skill == AT_SKILL_WIZARDDEFENSE)
					|| ((AT_SKILL_SOUL_UP <= Skill) && (Skill <= AT_SKILL_SOUL_UP + 4))
					)
				{
					CKind_1 = KIND_PLAYER;
					CKind_2 = KIND_MONSTER | KIND_EDIT;
				}
			}

			SelectedCharacter = g_pPartyListWindow->GetSelectedCharacter();
			if (SelectedCharacter  == -1)
			{
				if (SelectedCharacter == -1)
				{
					SelectedCharacter = SelectCharacter(CKind_1);
				}
				if (SelectedCharacter == -1)
				{
					SelectedCharacter = SelectCharacter(CKind_2);
					if (SelectedCharacter == -1)
					{
						SelectedNpc = SelectCharacter(KIND_NPC);
						if (SelectedNpc == -1)
						{
							if (g_pPickedItem.GetItem() == nullptr)
							{
								SelectedItem = SelectItem();
							}

							if (SelectedItem == -1)
							{
								SelectedOperate = SelectOperate();
							}
						}
					}
				}
			}
		}
	}
	else
	{
		SelectedCharacter = g_pPartyListWindow->GetSelectedCharacter();
	}

	if (SelectedCharacter == -1)
	{
		Attacking = -1;
	}

#if 0 // FIX
	if (g_pPartyListWindow)
	{
		g_pPartyListWindow->SetListBGColor();
	}
#endif
}

void CHARACTERSCENE::SelectCharacter()
{
	SelectedCharacter = -1;
	Attacking = -1;
	SelectedItem = -1;
	SelectedNpc = -1;
	SelectedOperate = -1;

	if (MU_INPUT->IsMouseOverInterface() == false &&
		MU_CAMERA->IsDragging() == false &&
		CheckMouseIn(MU_VIDEO->_3DViewport.OffsetX,
			MU_VIDEO->_3DViewport.OffsetY,
			MU_VIDEO->_3DViewport.Width,
			MU_VIDEO->_3DViewport.Height))
	{
		EVector3 Light;
		Vector(0.8f, 0.8f, 0.8f, Light);
		mu_int32 iSelected = -1;
		mu_float fNearestDist = FLT_MAX;

		for (mu_int32 i = 0; i < MAX_CHARACTERLIST_COUNT; ++i)
		{
			CHARACTER *c = &CharactersClient[BEGIN_CHARACTERLIST_COUNT + i];
			OBJECT *o = &c->Object;
			if (o->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == false)
				continue;

			if (o->Alpha > 0.0f && c->Dead == 0 && !g_isCharacterBuff(o, eBuff_CrywolfNPCHide))
			{
				if (c->m_bIsSelected == false)
				{
					continue;
				}

				if ((KIND_PLAYER & o->Kind) == o->Kind)
				{
					if (o->Type == MODEL_SMELTING_NPC)
					{
						o->OBB.StartPos[2] += 300.0f;
					}
					else if (o->Type == MODEL_MONSTER01 + 118 || o->Type == MODEL_MONSTER01 + 119)
					{
						o->OBB.StartPos[2] += 200.0f;
					}
					else if (o->Type == MODEL_KANTURU2ND_ENTER_NPC)
					{
						o->OBB.StartPos[0] -= 100.0f;
						o->OBB.StartPos[2] += 100.0f;
						o->OBB.XAxis[0] += 100.0f;
						o->OBB.ZAxis[2] += 100.0f;
					}

					if (CollisionDetectLineToOBB(MousePosition, MouseTarget, o->OBB))
					{
						EVector3 vSub;
						VectorSubtract(o->Position, MU_CAMERA->GetCameraPosition(), vSub);

						mu_float fNewDist = DotProduct(vSub, vSub);

						if (fNewDist < fNearestDist)
						{
							iSelected = BEGIN_CHARACTERLIST_COUNT + i;
							fNearestDist = fNewDist;
						}
					}
				}
			}
		}

		SelectedCharacter = iSelected;
	}
}

void ConvertGold(mu_double dGold, mu_unicode* szText, const mu_uint32 TextLength, mu_int32 iDecimals)
{
	mu_unicode szTemp[256];
	mu_int32 iCipherCnt = 0;
	mu_uint32 dwValueTemp = (mu_uint32)dGold;

	while (dwValueTemp / 1000 > 0)
	{
		iCipherCnt = iCipherCnt + 3;
		dwValueTemp = dwValueTemp / 1000;
	}

	mu_sprintf_s(szText, TextLength, _T("%u"), dwValueTemp);

	while (iCipherCnt > 0)
	{
		dwValueTemp = (mu_uint32)dGold;
		dwValueTemp = (dwValueTemp % (mu_int32)EMath::Pow(10.0f, (mu_float)iCipherCnt)) / (mu_int32)EMath::Pow(10.0f, (mu_float)(iCipherCnt - 3));
		mu_sprintf_s(szTemp, mu_countof(szTemp), _T(",%03u"), dwValueTemp);
		mu_strcat_s(szText, TextLength, szTemp);
		iCipherCnt = iCipherCnt - 3;
	}

	if (iDecimals > 0)
	{
		dwValueTemp = (mu_int32)(dGold*EMath::Pow(10.0f, (mu_float)iDecimals)) % (mu_int32)EMath::Pow(10.0f, (mu_float)iDecimals);
		mu_sprintf_s(szTemp, mu_countof(szTemp), _T(".%u"), dwValueTemp);
		mu_strcat_s(szText, TextLength, szTemp);
	}
}

void ConvertGold64(mu_int64 Gold, mu_unicode *Text, const mu_uint32 TextLength)
{
	mu_int32 Gold1 = Gold % 1000;
	mu_int32 Gold2 = Gold % 1000000 / 1000;
	mu_int32 Gold3 = Gold % 1000000000 / 1000000;
	mu_int32 Gold4 = Gold % 1000000000000 / 1000000000;
	mu_int32 Gold5 = Gold % 1000000000000000 / 1000000000000;
	mu_int32 Gold6 = Gold / 1000000000000000;
	if (Gold >= 1000000000000000)
		mu_sprintf_s(Text, TextLength, _T("%d,%03d,%03d,%03d,%03d,%03d"), Gold6, Gold5, Gold4, Gold3, Gold2, Gold1);
	else if (Gold >= 1000000000000)
		mu_sprintf_s(Text, TextLength, _T("%d,%03d,%03d,%03d,%03d"), Gold5, Gold4, Gold3, Gold2, Gold1);
	else if (Gold >= 1000000000)
		mu_sprintf_s(Text, TextLength, _T("%d,%03d,%03d,%03d"), Gold4, Gold3, Gold2, Gold1);
	else if (Gold >= 1000000)
		mu_sprintf_s(Text, TextLength, _T("%d,%03d,%03d"), Gold3, Gold2, Gold1);
	else if (Gold >= 1000)
		mu_sprintf_s(Text, TextLength, _T("%d,%03d"), Gold2, Gold1);
	else
		mu_sprintf_s(Text, TextLength, _T("%d"), Gold1);
}

void ConvertTaxGold(mu_int64 Gold, mu_unicode *Text, const mu_uint32 TextLength)
{
	Gold += (Gold * g_pNpcShopInventory->GetTaxRate()) / 100;

	mu_int32 Gold1 = Gold % 1000;
	mu_int32 Gold2 = Gold % 1000000 / 1000;
	mu_int32 Gold3 = Gold % 1000000000 / 1000000;
	mu_int32 Gold4 = Gold % 1000000000000 / 1000000000;
	mu_int32 Gold5 = Gold % 1000000000000000 / 1000000000000;
	mu_int32 Gold6 = Gold / 1000000000000000;
	if (Gold >= 1000000000000000)
		mu_sprintf_s(Text, TextLength, _T("%d,%03d,%03d,%03d,%03d,%03d"), Gold6, Gold5, Gold4, Gold3, Gold2, Gold1);
	else if (Gold >= 1000000000000)
		mu_sprintf_s(Text, TextLength, _T("%d,%03d,%03d,%03d,%03d"), Gold5, Gold4, Gold3, Gold2, Gold1);
	else if (Gold >= 1000000000)
		mu_sprintf_s(Text, TextLength, _T("%d,%03d,%03d,%03d"), Gold4, Gold3, Gold2, Gold1);
	else if (Gold >= 1000000)
		mu_sprintf_s(Text, TextLength, _T("%d,%03d,%03d"), Gold3, Gold2, Gold1);
	else if (Gold >= 1000)
		mu_sprintf_s(Text, TextLength, _T("%d,%03d"), Gold2, Gold1);
	else
		mu_sprintf_s(Text, TextLength, _T("%d"), Gold1);
}

void ConvertChaosTaxGold(mu_int64 Gold, mu_unicode *Text, const mu_uint32 TextLength)
{
#if 0 // FIX
	Gold += (Gold * g_nChaosTaxRate) / 100;
#endif

	mu_int32 Gold1 = Gold % 1000;
	mu_int32 Gold2 = Gold % 1000000 / 1000;
	mu_int32 Gold3 = Gold % 1000000000 / 1000000;
	mu_int32 Gold4 = Gold % 1000000000000 / 1000000000;
	mu_int32 Gold5 = Gold % 1000000000000000 / 1000000000000;
	mu_int32 Gold6 = Gold / 1000000000000000;
	if (Gold >= 1000000000000000)
		mu_sprintf_s(Text, TextLength, _T("%d,%03d,%03d,%03d,%03d,%03d"), Gold6, Gold5, Gold4, Gold3, Gold2, Gold1);
	else if (Gold >= 1000000000000)
		mu_sprintf_s(Text, TextLength, _T("%d,%03d,%03d,%03d,%03d"), Gold5, Gold4, Gold3, Gold2, Gold1);
	else if (Gold >= 1000000000)
		mu_sprintf_s(Text, TextLength, _T("%d,%03d,%03d,%03d"), Gold4, Gold3, Gold2, Gold1);
	else if (Gold >= 1000000)
		mu_sprintf_s(Text, TextLength, _T("%d,%03d,%03d"), Gold3, Gold2, Gold1);
	else if (Gold >= 1000)
		mu_sprintf_s(Text, TextLength, _T("%d,%03d"), Gold2, Gold1);
	else
		mu_sprintf_s(Text, TextLength, _T("%d"), Gold1);
}

mu_int64 ConvertRepairGold(mu_int64 Gold, mu_int32 Durability, mu_int32 MaxDurability, mu_int16 Type, mu_unicode *Text, const mu_uint32 TextLength)
{
	Gold = EMath::Min(Gold, 400000000ll);

	mu_double repairGold = (mu_double)Gold;
	mu_double persent = 1.0 - (mu_double)(Durability / (mu_double)MaxDurability);
	mu_boolean doubleP = false;

	if (persent > 0)
	{
		mu_double fRoot = (mu_double)EMath::Sqrt(repairGold);
		mu_double fRootRoot = (mu_double)EMath::Sqrt(EMath::Sqrt(repairGold));
		repairGold = 3.0 * fRoot * fRootRoot;

		if (doubleP)
		{
			repairGold *= 2.0;
		}

		repairGold *= persent;
		repairGold += 1.0;

		if (Durability <= 0)
		{
			if (Type == ITEM_HELPER + 4 || Type == ITEM_HELPER + 5)
			{
				repairGold *= 2.0;
			}
			else
			{
				repairGold += repairGold*0.4;
			}
		}
	}
	else
	{
		repairGold = 0;
	}

	if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_NPCSHOP) == true
#if 0 // FIX
		&& g_pNPCShop->IsRepairShop()
#endif
		)
	{
		Gold = (mu_int64)(repairGold);
	}
	else if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_INVENTORY) && !g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_NPCSHOP))
	{
		Gold = (mu_int64)(repairGold + (repairGold * 1.5f));
	}
	else
	{
		Gold = (mu_int64)(repairGold);
	}

	if (Gold >= 1000)
	{
		Gold = (Gold / 100) * 100;
	}
	else if (Gold >= 100)
	{
		Gold = (Gold / 10) * 10;
	}

	ConvertGold(Gold, Text, TextLength);

	return Gold;
}

mu_uint32 getGoldColor(mu_int64 Gold)
{
	if (Gold >= 10000000)
	{
		return MU::MakeARGB(255, 0, 0, 255);
	}
	else if (Gold >= 1000000)
	{
		return MU::MakeARGB(255, 150, 0, 255);
	}
	else if (Gold >= 100000)
	{
		return MU::MakeARGB(0, 201, 24, 255);
	}

	return MU::MakeARGB(255, 220, 150, 255);
}

const mu_text* GetMapName(mu_int32 iMap)
{
	if (iMap == WD_34CRYWOLF_1ST)
	{
		return(GlobalText[1851]);
	}
	else if (iMap == WD_33AIDA)
	{
		return(GlobalText[1850]);
	}
	else if (iMap == WD_37KANTURU_1ST)
	{
		return(GlobalText[2177]);
	}
	else if (iMap == WD_38KANTURU_2ND)
	{
		return(GlobalText[2178]);
	}
	else if (iMap == WD_39KANTURU_3RD)
	{
		return(GlobalText[2179]);
	}
	else if (iMap == WD_40AREA_FOR_GM)
	{
		return(GlobalText[2324]);
	}
	else if (iMap == WD_51HOME_6TH_CHAR)
	{
		return(GlobalText[1853]);
	}
	else if (iMap == WD_57ICECITY)
	{
		return(GlobalText[1855]);
	}
	else if (iMap == WD_58ICECITY_BOSS)
	{
		return(GlobalText[1856]);
	}
	else if (battleCastle::InBattleCastle(iMap) == true)
	{
		return (GlobalText[669]);
	}
	else if (iMap == WD_31HUNTING_GROUND)
	{
		return (GlobalText[59]);
	}
	else if (InChaosCastle(iMap) == true)
	{
		return (GlobalText[57]);
	}
	else if (InHellas(iMap) == true)
	{
		if (InHiddenHellas(iMap) == true)
			return (GlobalText[1852]);
		return (GlobalText[58]);
	}
	else if (InBloodCastle(iMap) == true)
	{
		return (GlobalText[56]);
	}
	else if (iMap == WD_10HEAVEN)
	{
		return (GlobalText[55 + iMap - WD_10HEAVEN]);
	}
	else if (iMap == 32)
	{
		return (GlobalText[39]);
	}
	else if (IsBalgasBarrackMap())
	{
		return GlobalText[1678];
	}
	else if (IsBalgasRefugeMap())
	{
		return GlobalText[1679];
	}
	else if (g_CursedTemple.IsCursedTemple())
	{
		return (GlobalText[2369]);
	}
	else if (iMap == WD_51HOME_6TH_CHAR)
	{
		return (GlobalText[1853]);
	}
	else if (iMap == WD_56MAP_SWAMP_OF_QUIET)
	{
		return (GlobalText[1854]);
	}
	else if (iMap == WD_62SANTA_TOWN)
	{
		return (GlobalText[2611]);
	}
	else if (iMap == WD_64DUELARENA)
	{
		return (GlobalText[2703]);
	}
	else if (iMap == WD_63PK_FIELD)
	{
		return (GlobalText[2686]);
	}
	else if (iMap == WD_65DOPPLEGANGER1)
	{
		return (GlobalText[3057]);
	}
	else if (iMap == WD_66DOPPLEGANGER2)
	{
		return (GlobalText[3057]);
	}
	else if (iMap == WD_67DOPPLEGANGER3)
	{
		return (GlobalText[3057]);
	}
	else if (iMap == WD_68DOPPLEGANGER4)
	{
		return (GlobalText[3057]);
	}
	else if (iMap == WD_69EMPIREGUARDIAN1)
	{
		return (GlobalText[2806]);
	}
	else if (iMap == WD_70EMPIREGUARDIAN2)
	{
		return (GlobalText[2806]);
	}
	else if (iMap == WD_71EMPIREGUARDIAN3)
	{
		return (GlobalText[2806]);
	}
	else if (iMap == WD_72EMPIREGUARDIAN4)
	{
		return (GlobalText[2806]);
	}
	else if (iMap == WD_79UNITEDMARKETPLACE)
	{
		return (GlobalText[3017]);
	}

	return (GlobalText[30 + iMap]);
}