#include "stdafx.h"
#include "mu_uisimplechat.h"

UISimpleChat::UISimpleChat(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;
}

UISimpleChat::~UISimpleChat()
{

}

void UISimpleChat::Update(mu_uint64 CurrentTime)
{

}

void UISimpleChat::Draw()
{
	EVector4 Limits = EVector4(MU_VIDEO->_3DViewport.OffsetX,
		MU_VIDEO->_3DViewport.OffsetY,
		MU_VIDEO->_3DViewport.OffsetX + MU_VIDEO->_3DViewport.Width,
		EMath::Floor(MU_VIDEO->_3DViewport.OffsetY + MU_VIDEO->_3DViewport.Height) - MU_TO_VIEWPORT(8.0f, MU_RESOURCE->_1024x768Rate[1]));

	for (mu_int32 i = 0; i < MAX_CHAT; ++i)
	{
		UIChatData *ci = &_Chat[i];

		if (ci->IDLifeTime > 0 || ci->LifeTime[0] > 0)
		{
			EVector3 Position, Projected;

			if (ci->Owner != nullptr)
			{
				OBJECT *o = &ci->Owner->Object;
				
				if (o->Type >= MODEL_LITTLESANTA && o->Type <= MODEL_LITTLESANTA_END)
				{
					Vector(o->Position[0], o->Position[1], o->Position[2] + o->BoundingBoxMax[2] + 20.0f, Position);
				}
				else if (o->Type == MODEL_MERCHANT_MAN)
				{
					Vector(o->Position[0], o->Position[1], o->Position[2] + o->BoundingBoxMax[2] + 20.0f, Position);
				}
				else
				{
					Vector(o->Position[0], o->Position[1], o->Position[2] + o->BoundingBoxMax[2] + 60.0f, Position);
				}

				ci->Visible = TestFrustrum2D(Position, 200.0f);
				if (ci->Visible == true)
				{
					Projected = EMath::Project(Position, MU_VIDEO->_CurrentView, MU_VIDEO->_CurrentRayProjection, Limits);
				}
			}
			else
			{
				ci->Visible = TestFrustrum2D(ci->Position, 200.0f);
				if (ci->Visible == true)
				{
					Projected = EMath::Project(ci->Position, MU_VIDEO->_CurrentView, MU_VIDEO->_CurrentRayProjection, Limits);
				}
			}

			if(ci->Visible == true)
			{
				SetBooleanPosition(ci);

				ci->X = Projected.x - (ci->Width * 0.5f);
				ci->Y = (Limits.w - Projected.y) - ci->Height;
			}
		}
	}

	for (mu_int32 i = 0; i < MAX_CHAT; ++i)
	{
		UIChatData *ci = &_Chat[i];
		if (ci->Visible == true &&
			(ci->IDLifeTime > 0 ||
				ci->LifeTime[0] > 0))
		{
			for (mu_int32 j = 0; j < MAX_CHAT; ++j)
			{
				UIChatData *cj = &_Chat[j];
				if (i != j &&
					cj->Visible == true &&
					(cj->IDLifeTime > 0 ||
						cj->LifeTime[0] > 0))
				{
					if (ci->X + ci->Width > cj->X && ci->X < cj->X + cj->Width &&
						ci->Y + ci->Height > cj->Y && ci->Y < cj->Y + cj->Height)
					{
						if (ci->Y < cj->Y + cj->Height * 0.5f)
							ci->Y = cj->Y - ci->Height;
						else
							ci->Y = cj->Y + cj->Height;
					}
				}
			}
		}
	}

	for (mu_int32 i = 0; i < MAX_CHAT; ++i)
	{
		UIChatData *ci = &_Chat[i];
		if (ci->Visible == true &&
			(ci->IDLifeTime > 0 ||
				ci->LifeTime[0] > 0))
		{
			if (ci->X < Limits.x) ci->X = Limits.x;
			else if (ci->X >= Limits.z - ci->Width) ci->X = Limits.z - ci->Width;
			if (ci->Y < Limits.y) ci->Y = Limits.y;
			else if (ci->Y >= Limits.w - ci->Height) ci->Y = Limits.w - ci->Height;

			RenderBoolean(ci->X, ci->Y, ci);
		}
	}
}

void UISimpleChat::RenderBoolean(mu_float x, mu_float y, UIChatData *c)
{
	if (g_isCharacterBuff((&c->Owner->Object), eBuff_CrywolfNPCHide))
	{
		return;
	}

	if (c->Owner != Hero && battleCastle::IsBattleCastleStart() == true && g_isCharacterBuff((&c->Owner->Object), eBuff_Cloaking))
	{
		if ((Hero->EtcPart == PARTS_ATTACK_KING_TEAM_MARK || Hero->EtcPart == PARTS_ATTACK_TEAM_MARK))
		{
			if (!(c->Owner->EtcPart == PARTS_ATTACK_KING_TEAM_MARK || c->Owner->EtcPart == PARTS_ATTACK_TEAM_MARK))
			{
				return;
			}
		}
		else if ((Hero->EtcPart == PARTS_ATTACK_KING_TEAM_MARK2 || Hero->EtcPart == PARTS_ATTACK_TEAM_MARK2))
		{
			if (!(c->Owner->EtcPart == PARTS_ATTACK_KING_TEAM_MARK2 || c->Owner->EtcPart == PARTS_ATTACK_TEAM_MARK2))
			{
				return;
			}
		}
		else if ((Hero->EtcPart == PARTS_ATTACK_KING_TEAM_MARK3 || Hero->EtcPart == PARTS_ATTACK_TEAM_MARK3))
		{
			if (!(c->Owner->EtcPart == PARTS_ATTACK_KING_TEAM_MARK3 || c->Owner->EtcPart == PARTS_ATTACK_TEAM_MARK3))
			{
				return;
			}
		}
		else if ((Hero->EtcPart == PARTS_DEFENSE_KING_TEAM_MARK || Hero->EtcPart == PARTS_DEFENSE_TEAM_MARK))
		{
			if (!(c->Owner->EtcPart == PARTS_DEFENSE_KING_TEAM_MARK || c->Owner->EtcPart == PARTS_DEFENSE_TEAM_MARK))
			{
				return;
			}
		}
	}

	mu_uint32 TextColor = 0;
	mu_uint32 BackColor = 0;

	EVector2 RenderBoxSize(c->Width, c->Height);
	EVector2 RenderPos(x, y);

#if 0 // FIX
	if (IsShopInViewport(c->Owner))
	{
		BackColor = GetShopBGColor(c->Owner);
		TextColor = GetShopTextColor(c->Owner);

		MUText *TextBuffer = nullptr;
		FONT_BOLD->DrawStringC2D(c->TextIndex[0], RenderPos.x, RenderPos.y, 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, TextColor, BackColor, GlobalText[1104], 0.0f, 0.0f, &TextBuffer);
		RenderPos.x += TextBuffer->_Size.x;

		TextColor = GetShopText2Color(c->Owner);
		FONT_BOLD->DrawStringC2D(c->TextIndex[1], RenderPos.x, RenderPos.y, 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, TextColor, BackColor, c->ShopTitle.c_str(), RenderBoxSize.x - TextBuffer->_Size.x);

		RenderPos.x = x;
		RenderPos.y += TextBuffer->_Size.y;
	}
	else
#endif
		if (IsStrifeMap(GET_WORLD) == true &&
			Hero->m_byGensInfluence != c->Owner->m_byGensInfluence
			&& IsGMCharacter() == false)
		{
			if (c->Owner->m_nContributionPoint <= 0)
			{
				return;
			}

			if (c->Owner->Object.Kind == KIND_PLAYER &&
				c->Owner->Object.Type == MODEL_PLAYER)
			{
#if 0 // FIX
				mu_float _TempX = (mu_float)(c->X + c->Width*0.5f + 20.0f);
				switch (c->Owner->m_byGensInfluence)
				{
				case 1:
					g_pNewUIGensRanking->RanderMark(_TempX, y, c->Owner->m_byGensInfluence, c->Owner->m_byRankIndex, SEASON3B::CNewUIGensRanking::MARK_BOOLEAN, (mu_float)RenderPos.y);
					return;
				case 2:
					g_pNewUIGensRanking->RanderMark(_TempX, y, c->Owner->m_byGensInfluence, c->Owner->m_byRankIndex, SEASON3B::CNewUIGensRanking::MARK_BOOLEAN, (mu_float)RenderPos.y);
					return;
				}
#endif
			}
		}

	mu_boolean bGmMode = false;

	if (g_isCharacterBuff((&c->Owner->Object), eBuff_GMEffect) ||
		c->Owner->CtlCode == CTLCODE_ADMIN ||
		c->Owner->CtlCode == CTLCODE_EVENT_GM)
	{
		bGmMode = true;
	}

	if (c->Owner == Hero)
	{
		BackColor = MU::MakeARGB(60, 100, 0, 150);
		TextColor = MU::MakeARGB(200, 255, 0, 255);
	}
	else if (c->Owner->GuildMarkIndex == Hero->GuildMarkIndex)
	{
		BackColor = GetGuildRelationShipBGColor(GAME::GR_UNION);
		TextColor = GetGuildRelationShipTextColor(GAME::GR_UNION);
	}
	else
	{
		BackColor = GetGuildRelationShipBGColor(c->Owner->GuildRelationShip);
		TextColor = GetGuildRelationShipTextColor(c->Owner->GuildRelationShip);
	}

	if (c->Union.empty() == false)
	{
		MUText *TextBuffer = nullptr;
		FONT_NORMAL->DrawStringC2D(c->TextIndex[2], RenderPos.x, RenderPos.y, 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, TextColor, BackColor, c->Union.c_str(), RenderBoxSize.x, 0.0f, &TextBuffer);
		RenderPos.y += TextBuffer->_Size.y;
	}

	if (c->Guild.empty() == false)
	{
		MUText *TextBuffer = nullptr;
		FONT_NORMAL->DrawStringC2D(c->TextIndex[3], RenderPos.x, RenderPos.y, 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, TextColor, BackColor, c->Guild.c_str(), RenderBoxSize.x, 0.0f, &TextBuffer);
		RenderPos.y += TextBuffer->_Size.y;
	}

	if (bGmMode)
	{
		TextColor = MU::MakeARGB(100, 250, 250, 255);
	}
	else
	{
		SetPlayerColor(c->PkColor, TextColor);
	}

	if (Hero->SafeZone == false ||
		mu_strcmp(c->ID.c_str(), Hero->ID) == 0)
	{
		c->MouseOver = false;
	}
	else
	{
		if (c->X <= MU_INPUT->GetMouseX() && MU_INPUT->GetMouseX() < (mu_int32)(c->X + c->Width) &&
			c->Y <= MU_INPUT->GetMouseY() && MU_INPUT->GetMouseY() < (mu_int32)(c->Y + c->Height))
		{
			if (c->MouseOver == false)
			{
				c->MouseOver = true;
				c->MouseTime = g_CurrentTime;
			}

			if ((g_CurrentTime - c->MouseTime) % 1000 < 500)
			{
				mu_uint32 Temp = BackColor;
				BackColor = TextColor;
				TextColor = Temp;
			}
		}
		else
		{
			c->MouseOver = false;
		}
	}

	if (bGmMode)
	{
		MUText *TextBuffer = nullptr;
		FONT_BOLD->DrawStringC2D(c->TextIndex[4], RenderPos.x, RenderPos.y, 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, TextColor, BackColor, c->ID.c_str(), RenderBoxSize.x, 0.0f, &TextBuffer);
		RenderPos.y += TextBuffer->_Size.y;
	}
	else
	{
		MUText *TextBuffer = nullptr;
		FONT_NORMAL->DrawStringC2D(c->TextIndex[4], RenderPos.x, RenderPos.y, 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, TextColor, BackColor, c->ID.c_str(), RenderBoxSize.x, 0.0f, &TextBuffer);
		RenderPos.y += TextBuffer->_Size.y;
	}

	if (c->GuildColor == 0)
		BackColor = MU::MakeARGB(10, 30, 50, 150);
	else if (c->GuildColor == 1)
		BackColor = MU::MakeARGB(30, 50, 0, 150);
	else if (bGmMode)
		BackColor = MU::MakeARGB(30, 30, 30, 200);
	else
		BackColor = MU::MakeARGB(50, 0, 0, 150);

	mu_uint32 dwTextColor[2];
	mu_uint8 byAlpha[2] = { 255, 255 };
	if ((c->LifeTime[0] > 0 && c->LifeTime[0] < 10))
		byAlpha[0] = 128;
	if ((c->LifeTime[1] > 0 && c->LifeTime[1] < 10))
		byAlpha[1] = 128;

	if (bGmMode)
	{
		dwTextColor[0] = MU::MakeARGB(250, 200, 50, byAlpha[0]);
		dwTextColor[1] = MU::MakeARGB(250, 200, 50, byAlpha[1]);
	}
	else
	{
		dwTextColor[0] = MU::MakeARGB(230, 220, 200, byAlpha[0]);
		dwTextColor[1] = MU::MakeARGB(230, 220, 200, byAlpha[1]);
	}

	if (c->LifeTime[1] > 0)
	{
		TextColor = dwTextColor[1];
		MUText *TextBuffer = nullptr;
		FONT_NORMAL->DrawStringC2D(c->TextIndex[5], RenderPos.x, RenderPos.y, 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, TextColor, BackColor, c->Text[1].c_str(), RenderBoxSize.x, 0.0f, &TextBuffer);
		RenderPos.y += TextBuffer->_Size.y;

		TextColor = dwTextColor[0];
		FONT_NORMAL->DrawStringC2D(c->TextIndex[6], RenderPos.x, RenderPos.y, 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, TextColor, BackColor, c->Text[0].c_str(), RenderBoxSize.x);
		RenderPos.y += TextBuffer->_Size.y;
	}
	else if (c->LifeTime[0] > 0)
	{
		TextColor = dwTextColor[0];
		FONT_NORMAL->DrawStringC2D(c->TextIndex[6], RenderPos.x, RenderPos.y, 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, TextColor, BackColor, c->Text[0].c_str(), RenderBoxSize.x);
	}

#if 0 // FIX
	if (KIND_PLAYER == c->Owner->Object.Kind && MODEL_PLAYER == c->Owner->Object.Type)
	{
		if (1 == c->Owner->m_byGensInfluence)
			g_pNewUIGensRanking->RanderMark(x, y, c->Owner->m_byGensInfluence, c->Owner->m_byRankIndex, SEASON3B::CNewUIGensRanking::MARK_BOOLEAN, RenderPos.y);
		else if (2 == c->Owner->m_byGensInfluence)
			g_pNewUIGensRanking->RanderMark(x, y, c->Owner->m_byGensInfluence, c->Owner->m_byRankIndex, SEASON3B::CNewUIGensRanking::MARK_BOOLEAN, RenderPos.y);
	}
#endif
}

UIBase *UISimpleChat::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	return nullptr;
}

void UISimpleChat::MultithreadUpdate(const mu_uint32 ThreadIndex)
{
	mu_uint32 threadsCount = g_Tasking.GetThreadsCount();
	for (mu_uint32 i = ThreadIndex; i < MAX_CHAT; i += threadsCount)
	{
		UIChatData *c = &_Chat[i];
		if (c->IDLifeTime > 0)
			--c->IDLifeTime;
		if (c->LifeTime[0] > 0)
			--c->LifeTime[0];
		if (c->LifeTime[1] > 0)
			--c->LifeTime[1];

		if (c->Owner != nullptr && c->Owner->Object.Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == false)
		{
			c->IDLifeTime = 0;
			c->LifeTime[0] = 0;
			c->LifeTime[1] = 0;
		}

#if 0 // FIX
		if (c->LifeTime[0] > 0)
		{
			DisableShopTitleDraw(c->Owner);
		}
		else if (c->Owner != nullptr && c->LifeTime[0] == 0)
		{
			EnableShopTitleDraw(c->Owner);
		}
#endif
	}
}

void UISimpleChat::Clear()
{
	for (mu_uint32 i = 0; i < MAX_CHAT; ++i)
	{
		UIChatData *c = &_Chat[i];
		c->IDLifeTime = 0;
		c->LifeTime[0] = 0;
		c->LifeTime[1] = 0;
	}
}

void UISimpleChat::Create(const mu_text *ID, const mu_text *Text, CHARACTER *Owner, mu_int32 Flag, mu_int32 PkColor)
{
	OBJECT *o = &Owner->Object;
	if (o->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == false)
	{
		return;
	}

	mu_int32 Color = PkColor;

	if (PkColor == -1)
	{
		Color = Owner->PK;

		if (o->Kind == KIND_NPC)
		{
			Color = 0;
		}
	}

	mu_size TextLength = mu_strlen(Text);

	for (mu_uint32 i = 0; i < MAX_CHAT; ++i)
	{
		UIChatData *c = &_Chat[i];

		if (c->Owner == Owner)
		{
			c->ID = ID;
			c->PkColor = Color;

			AddGuildName(c, Owner);

			if (TextLength == 0)
			{
				c->IDLifeTime = 10;
			}
			else
			{
				if (c->LifeTime[0] > 0)
				{
					c->Text[1] = c->Text[0];
					c->LifeTime[1] = c->LifeTime[0];
				}

				c->Owner = Owner;
				AddChat(c, Text, TextLength, Flag);
			}
			return;
		}
	}

	for (mu_uint32 i = 0; i < MAX_CHAT; ++i)
	{
		UIChatData *c = &_Chat[i];

		if (c->IDLifeTime <= 0 && c->LifeTime[0] <= 0)
		{
			c->Owner = Owner;
			c->ID = ID;
			c->PkColor = Color;
			AddGuildName(c, Owner);

			if (TextLength == 0)
			{
				c->IDLifeTime = 100;
			}
			else
			{
				AddChat(c, Text, TextLength, Flag);
			}
			return;
		}
	}
}

mu_int32 UISimpleChat::Create(const mu_text *ID, const mu_text *Text, OBJECT *Owner, mu_int32 LifeTime, mu_int32 PkColor)
{
	OBJECT *o = Owner;
	if (o->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == false)
	{
		return 0;
	}

	mu_int32 Color = 0;

	if (PkColor != -1)
	{
		Color = PkColor;
	}

	for (mu_int32 i = 0; i < MAX_CHAT; ++i)
	{
		UIChatData *c = &_Chat[i];
		if (c->IDLifeTime <= 0 && c->LifeTime[0] <= 0)
		{
			c->Owner = nullptr;
			c->ID = ID;
			c->PkColor = Color;
			c->GuildColor = 0;
			c->Guild.clear();
			AddChat(c, Text, mu_strlen(Text), 0);
			c->LifeTime[0] = LifeTime;

			Vector(o->Position[0], o->Position[1], o->Position[2] + o->BoundingBoxMax[2] + 60.0f, c->Position);
			
			return c->LifeTime[0];
		}
	}

	return 0;
}

void UISimpleChat::Assign(mu_int32 Key, const mu_text *ID, const mu_text *Text, mu_int32 Flag)
{
	if (Key == -1)
	{
		for (mu_int32 i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
		{
			CHARACTER *c = &CharactersClient[i];
			OBJECT *o = &c->Object;
			if (o->Flags.Get(OBJECT::eOF_LIVE) && o->Kind == KIND_PLAYER)
			{
				if (mu_stricmp(c->ID, ID) == 0)
				{
					Create(ID, Text, c, Flag);
					return;
				}
			}
		}

		for (mu_int32 i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
		{
			CHARACTER *c = &CharactersClient[i];
			OBJECT *o = &c->Object;
			if (o->Flags.Get(OBJECT::eOF_LIVE) && o->Kind == KIND_MONSTER)
			{
				if (mu_stricmp(c->ID, ID) == 0)
				{
					Create(ID, Text, c, Flag);
					return;
				}
			}
		}
	}
	else
	{
		mu_int16 Index = CharactersKeys[Key];

		if (Index != -1)
		{
			CHARACTER *c = &CharactersClient[Index];
			OBJECT *o = &c->Object;

			if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
			{
				if (mu_stricmp(c->ID, ID) == 0)
				{
					Create(ID, Text, c, Flag);
				}
			}
		}
	}
}

void UISimpleChat::Delete(CHARACTER *Owner)
{
	if (Owner == nullptr) return;

	for (mu_int32 i = 0; i < MAX_CHAT; ++i)
	{
		UIChatData *ci = &_Chat[i];

		if (ci->IDLifeTime > 0 || ci->LifeTime[0] > 0)
		{
			if (ci->Owner == Owner)
			{
				ci->IDLifeTime = 0;
				ci->LifeTime[0] = 0;
				ci->LifeTime[1] = 0;
			}
		}
	}
}

void UISimpleChat::AddChat(UIChatData *c, const mu_text *Text, mu_size TextLength, mu_int32 Flag)
{
	mu_int32 Time;

	switch (Flag)
	{
	default:
	case 0:
		{
			Time = TextLength * 2 + 160;
		}
		break;

	case 1:
		{
			Time = 1000;
			g_pChatListBox->AddText(c->ID, Text, TYPE_CHAT_MESSAGE);
		}
		break;
	}

	if (TextLength >= 20)
	{
		CutText(Text, c->Text[0], c->Text[1], TextLength);
		c->LifeTime[0] = Time;
		c->LifeTime[1] = Time;
	}
	else
	{
		c->Text[0] = Text;
		c->LifeTime[0] = Time;
	}
}

void UISimpleChat::AddGuildName(UIChatData *c, CHARACTER *Owner)
{
#if 0 // FIX
	if (IsShopInViewport(Owner))
	{
		GetShopTitleSummary(Owner, c->ShopTitle);
	}
	else
#endif
	{
		c->ShopTitle.clear();
	}

	if (Owner->GuildMarkIndex >= 0 && GuildMark[Owner->GuildMarkIndex].UnionName[0])
	{
		mu_text unionBuffer[512];

		if (Owner->GuildRelationShip == GAME::GR_UNION)
		{
			mu_sprintf_s(unionBuffer, mu_countof(unionBuffer), _T("<") SPF_STRING _T("> ") SPF_STRING _T(""), GuildMark[Owner->GuildMarkIndex].UnionName.c_str(), GlobalText[1295]);
		}
		else if (Owner->GuildRelationShip == GAME::GR_UNIONMASTER)
		{
			if (Owner->GuildStatus == GAME::G_MASTER)
				mu_sprintf_s(unionBuffer, mu_countof(unionBuffer), _T("<") SPF_STRING _T("> ") SPF_STRING _T(""), GuildMark[Owner->GuildMarkIndex].UnionName.c_str(), GlobalText[1296]);
			else
				mu_sprintf_s(unionBuffer, mu_countof(unionBuffer), _T("<") SPF_STRING _T("> ") SPF_STRING _T(""), GuildMark[Owner->GuildMarkIndex].UnionName.c_str(), GlobalText[1295]);
		}
		else if (Owner->GuildRelationShip == GAME::GR_RIVAL)
		{
			if (Owner->GuildStatus == GAME::G_MASTER)
				mu_sprintf_s(unionBuffer, mu_countof(unionBuffer), _T("<") SPF_STRING _T("> ") SPF_STRING _T(""), GuildMark[Owner->GuildMarkIndex].UnionName.c_str(), GlobalText[1298]);
			else
				mu_sprintf_s(unionBuffer, mu_countof(unionBuffer), _T("<") SPF_STRING _T("> ") SPF_STRING _T(""), GuildMark[Owner->GuildMarkIndex].UnionName.c_str(), GlobalText[1297]);
		}
		else if (Owner->GuildRelationShip == GAME::GR_RIVALUNION)
		{
			mu_sprintf_s(unionBuffer, mu_countof(unionBuffer), _T("<") SPF_STRING _T("> ") SPF_STRING _T(""), GuildMark[Owner->GuildMarkIndex].UnionName.c_str(), GlobalText[1299]);
		}
		else
		{
			mu_sprintf_s(unionBuffer, mu_countof(unionBuffer), _T("<") SPF_STRING _T(">"), GuildMark[Owner->GuildMarkIndex].UnionName.c_str());
		}

		c->Union = unionBuffer;
	}
	else
	{
		c->Union.clear();
	}

	if (Owner->GuildMarkIndex >= 0)
	{
		c->GuildColor = Owner->GuildTeam;

		mu_text guildBuffer[512];

		if (Owner->GuildStatus == GAME::G_PERSON)
		{
			mu_sprintf_s(guildBuffer, mu_countof(guildBuffer), _T("[") SPF_STRING _T("] ") SPF_STRING _T(""), GuildMark[Owner->GuildMarkIndex].GuildName.c_str(), GlobalText[1330]);
		}
		else if (Owner->GuildStatus == GAME::G_MASTER)
		{
			mu_sprintf_s(guildBuffer, mu_countof(guildBuffer), _T("[") SPF_STRING _T("] ") SPF_STRING _T(""), GuildMark[Owner->GuildMarkIndex].GuildName.c_str(), GlobalText[1300]);
		}
		else if (Owner->GuildStatus == GAME::G_SUB_MASTER)
		{
			mu_sprintf_s(guildBuffer, mu_countof(guildBuffer), _T("[") SPF_STRING _T("] ") SPF_STRING _T(""), GuildMark[Owner->GuildMarkIndex].GuildName.c_str(), GlobalText[1301]);
		}
		else if (Owner->GuildStatus == GAME::G_BATTLE_MASTER)
		{
			mu_sprintf_s(guildBuffer, mu_countof(guildBuffer), _T("[") SPF_STRING _T("] ") SPF_STRING _T(""), GuildMark[Owner->GuildMarkIndex].GuildName.c_str(), GlobalText[1302]);
		}
		else
		{
			mu_sprintf_s(guildBuffer, mu_countof(guildBuffer), _T("[") SPF_STRING _T("]"), GuildMark[Owner->GuildMarkIndex].GuildName.c_str());
		}

		c->Guild = guildBuffer;
	}
	else
	{
		c->GuildColor = 0;
		c->Guild.clear();
	}
}

void UISimpleChat::SetBooleanPosition(UIChatData *c)
{
	EVector2 Size[5];

	mu_zeromem(Size, sizeof(Size));

	if (g_isCharacterBuff((&c->Owner->Object), eBuff_GMEffect) ||
		c->Owner->CtlCode == CTLCODE_ADMIN ||
		c->Owner->CtlCode == CTLCODE_EVENT_GM)
	{
		FONT_BOLD->GetTextExtentPoint(c->ID.c_str(), c->ID.size(), ETA_LEFT, 1.0f, &Size[0]);
	}
	else
	{
		FONT_NORMAL->GetTextExtentPoint(c->ID.c_str(), c->ID.size(), ETA_LEFT, 1.0f, &Size[0]);
	}

	if (c->Text[0].empty() == false)
	{
		FONT_NORMAL->GetTextExtentPoint(c->Text[0].c_str(), c->Text[0].size(), ETA_LEFT, 1.0f, &Size[1]);
	}

	if (c->Text[1].empty() == false)
	{
		FONT_NORMAL->GetTextExtentPoint(c->Text[1].c_str(), c->Text[1].size(), ETA_LEFT, 1.0f, &Size[2]);
	}

	if (c->Union.empty() == false)
	{
		FONT_NORMAL->GetTextExtentPoint(c->Union.c_str(), c->Union.size(), ETA_LEFT, 1.0f, &Size[3]);
	}

	if (c->Guild.empty() == false)
	{
		FONT_NORMAL->GetTextExtentPoint(c->Guild.c_str(), c->Guild.size(), ETA_LEFT, 1.0f, &Size[4]);
	}

	Size[0].x += MU_TO_WINDOW(3.0f, MU_RESOURCE->_1024x768Rate[0]);

	if (c->LifeTime[1] > 0)
		c->Width = EMath::Max(EMath::Max(EMath::Max(Size[0].x, Size[1].x), EMath::Max(Size[2].x, Size[3].x)), Size[4].x);
	else if (c->LifeTime[0] > 0)
		c->Width = EMath::Max(EMath::Max(Size[0].x, Size[1].x), EMath::Max(Size[3].x, Size[4].x));
	else
		c->Width = EMath::Max(EMath::Max(Size[0].x, Size[3].x), Size[4].x);

	c->Height = Size[0].y + Size[1].y + Size[2].y + Size[3].y + Size[4].y;

	if (c->ShopTitle.empty() == false)
	{
		const mu_string &globalText = GlobalText.Get(1104);

		if (c->ShopTitle.empty() == false &&
			globalText.empty() == false)
		{
			EVector2 sizeT[2];
			mu_zeromem(sizeT, sizeof(sizeT));
			FONT_BOLD->GetTextExtentPoint(c->ShopTitle.c_str(), c->ShopTitle.size(), ETA_LEFT, 1.0f, &sizeT[0]);
			FONT_BOLD->GetTextExtentPoint(globalText.c_str(), globalText.size(), ETA_LEFT, 1.0f, &sizeT[1]);

			if (c->Width < sizeT[0].x + sizeT[1].x)
				c->Width = sizeT[0].x + sizeT[1].x;
			c->Height += EMath::Max(sizeT[0].y, sizeT[1].y);
		}
	}
}