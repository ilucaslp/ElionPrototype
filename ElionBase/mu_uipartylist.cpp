#include "stdafx.h"
#include "mu_uipartylist.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define MEMBER_WIDTH (77.0f)
#define MEMBER_HEIGHT (23.0f)

#define HPBAR_WIDTH (69.0f)
#define HPBAR_HEIGHT (3.0f)

UIPartyList::UIPartyList(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(PARTYLIST_POSX, PARTYLIST_POSY);

	// Initialize
	{
		for (mu_uint32 n = 0; n < MAX_PARTYS; ++n)
		{
			mu_float memberPosY = n * MEMBER_HEIGHT;

			_MemberBack[n] = new UISpriteColor(manager, this, MU::MakeARGB(0, 0, 0, 229), SCREEN_WIDTH, SCREEN_HEIGHT, PARTYLIST_POSX + 2.0f, PARTYLIST_POSY + memberPosY, MEMBER_WIDTH - 3.0f, MEMBER_HEIGHT - 7.0f);
			_MemberBack[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

			_MemberEffect[n] = new UISpriteColor(manager, this, MU::MakeARGB(51, 255, 51, 51), SCREEN_WIDTH, SCREEN_HEIGHT, PARTYLIST_POSX + 2.0f, PARTYLIST_POSY + memberPosY, MEMBER_WIDTH - 3.0f, MEMBER_HEIGHT - 7.0f);
			_MemberEffect[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

			_MemberOffline[n] = new UISpriteColor(manager, this, MU::MakeARGB(76, 0, 0, 127), SCREEN_WIDTH, SCREEN_HEIGHT, PARTYLIST_POSX + 2.0f, PARTYLIST_POSY + memberPosY, MEMBER_WIDTH - 3.0f, MEMBER_HEIGHT - 7.0f);
			_MemberOffline[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

			_MemberSelected[n] = new UIButton(GAMEUI::MESSAGE_PARTYLIST_SELECT_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, PARTYLIST_POSX + 2.0f, PARTYLIST_POSY + memberPosY, MEMBER_WIDTH - 3.0f, MEMBER_HEIGHT - 7.0f);
			_MemberSelected[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_MemberSelected[n]->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(102, 102, 102, 178)));
			_MemberSelected[n]->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(102, 102, 102, 178)));

			_MemberFrame[n] = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::PARTY_LIST_BACK], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYLIST_POSX, PARTYLIST_POSY + memberPosY, MEMBER_WIDTH, MEMBER_HEIGHT);
			_MemberFrame[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

			_LeaveBtn[n] = new UIButton(GAMEUI::MESSAGE_PARTYLIST_LEAVE_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, PARTYLIST_POSX + 63.0f, PARTYLIST_POSY + memberPosY + 3.0f, 11.0f, 11.0f);
			_LeaveBtn[n]->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::PARTY_EXIT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
			_LeaveBtn[n]->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::PARTY_EXIT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
			_LeaveBtn[n]->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::PARTY_EXIT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
			_LeaveBtn[n]->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::PARTY_EXIT], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));

			_MemberBar[n] = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::PARTYLIST_HPBAR], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYLIST_POSX + 4.0f, PARTYLIST_POSY + memberPosY + 16.0f, HPBAR_WIDTH, HPBAR_HEIGHT);
			_MemberBar[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		}

		_MemberFlag = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::PARTY_FLAG], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYLIST_POSX + 53.0f, PARTYLIST_POSY + 3.0f, 9.0f, 10.0f);
		_MemberFlag->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
	}
}

void UIPartyList::UpdateWindow(mu_int32 screenWidth)
{
	mu_float x = (mu_float)screenWidth - MEMBER_WIDTH - 2.0f;
	mu_float y = PARTYLIST_POSY;

#define SETPOSITION_HELPER(var) var->SetPosition(var->_ViewX - oldX + x, var->_ViewY - oldY + y)

	mu_float oldX = _ViewX, oldY = _ViewY;

	SETPOSITION_HELPER(this);

	for (mu_int32 n = 0; n < MAX_PARTYS; ++n)
	{
		SETPOSITION_HELPER(_MemberBack[n]);
		SETPOSITION_HELPER(_MemberEffect[n]);
		SETPOSITION_HELPER(_MemberSelected[n]);
		SETPOSITION_HELPER(_MemberFrame[n]);
		SETPOSITION_HELPER(_LeaveBtn[n]);
		SETPOSITION_HELPER(_MemberBar[n]);
	}

	SETPOSITION_HELPER(_MemberFlag);

#undef SETPOSITION_HELPER
}

void UIPartyList::Draw()
{
	if (PartyNumber > 0)
	{
		mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255);
		mu_uint32 backColor = MU::MakeARGB(0, 0, 0, 0);

		for (mu_uint32 n = 0; n < PartyNumber; ++n)
		{
			mu_float memberPosY = n * MEMBER_HEIGHT;
			PARTY_t &member = Party[n];

			_MemberBack[n]->Draw();

			if (member.index == -1)
			{
				_MemberOffline[n]->Draw();
			}
			else
			{
				if (member.index >= 0 && member.index < MAX_CHARACTERS_CLIENT)
				{
					CHARACTER* pChar = &CharactersClient[member.index];
					OBJECT* pObj = &pChar->Object;

					if (g_isCharacterBuff(pObj, eBuff_Defense) == true)
					{
						_MemberEffect[n]->Draw();
					}
				}

				_MemberSelected[n]->Draw();
			}

			_MemberFrame[n]->Draw();

			if (n == 0)
			{
				if (member.index == -1)
				{
					textColor = MU::MakeARGB(128, 75, 11, 255);
				}
				else
				{
					textColor = MU::MakeARGB(255, 148, 22, 255);
				}
			}
			else
			{
				if (member.index == -1)
				{
					textColor = MU::MakeARGB(128, 128, 128, 255);
				}
				else
				{
					textColor = MU::MakeARGB(255, 255, 255, 255);
				}
			}

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::PARTYLIST_MEMBERNAME_BEGIN + n,
				MU_TO_WINDOW(_ViewX + 4.0f, _AspectWidth),
				MU_TO_WINDOW(_ViewY + memberPosY + 4.0f, _AspectHeight),
				1.0f,
				ETA_LEFT,
				ETA_BOTTOM,
				0.0f,
				textColor,
				backColor,
				member.Name.c_str());

			if (Party[0].Key == HeroKey ||
				member.Key == HeroKey)
			{
				_LeaveBtn[n]->Draw();
			}

			mu_float hpPercent = member.maxHP > 0.0f ? member.currHP / member.maxHP : 0.0f;
			if (hpPercent > 1.0f) hpPercent = 1.0f;

			if (hpPercent > 0.0f)
			{
				_MemberBar[n]->SetUV(EVector2(0.0f, 0.0f), EVector2(hpPercent, 1.0f));
				_MemberBar[n]->SetDimensions(HPBAR_WIDTH * hpPercent, HPBAR_HEIGHT);
				_MemberBar[n]->Draw();
			}
		}

		_MemberFlag->Draw();
	}
}

UIBase *UIPartyList::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	if (PartyNumber > 0)
	{
		for (mu_uint32 n = 0; n < PartyNumber; ++n)
		{
			PARTY_t &member = Party[n];

			UIBase *object = nullptr;

			if (Party[0].Key == HeroKey ||
				member.Key == HeroKey)
			{
				object = _LeaveBtn[n]->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}
			}

			mu_int32 Skill = CharacterAttribute->Skill[Hero->CurrentSkill];

			if (Skill == AT_SKILL_HEALING
				|| Skill == AT_SKILL_DEFENSE
				|| (AT_SKILL_DEF_POWER_UP <= Skill && Skill <= AT_SKILL_DEF_POWER_UP + 4)
				|| (AT_SKILL_ATT_POWER_UP <= Skill && Skill <= AT_SKILL_ATT_POWER_UP + 4)
				|| Skill == AT_SKILL_ATTACK
				|| Skill == AT_SKILL_TELEPORT_B
				|| Skill == AT_SKILL_WIZARDDEFENSE
				|| (AT_SKILL_HEAL_UP <= Skill && Skill <= AT_SKILL_HEAL_UP + 4)
				|| (AT_SKILL_SOUL_UP <= Skill && Skill <= AT_SKILL_SOUL_UP + 4)
				|| Skill == AT_SKILL_ALICE_THORNS
				|| Skill == AT_SKILL_RECOVER
				)
			{
				object = _MemberSelected[n]->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}
			}
		}
	}

	return nullptr;
}

mu_int32 UIPartyList::GetSelectedCharacter()
{
	if (PartyNumber <= 0)
	{
		return -1;
	}

	UIBase *focused = _SceneManager->GetFocused();

	if (focused != nullptr)
	{
		for (mu_uint32 n = 0; n < PartyNumber; ++n)
		{
			if (_MemberSelected[n] == focused)
			{
				return Party[n].index;
			}
		}
	}

	return -1;
}