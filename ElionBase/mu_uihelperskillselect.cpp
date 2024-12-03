#include "stdafx.h"
#include "mu_uihelperskillselect.h"
#include "mu_uihelperskillselect_defines.h"

#define SKILL_U (20.0f/256.0f)
#define SKILL_V (28.0f/256.0f)

UIHelperSkillSelect::UIHelperSkillSelect(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(WINBOX_POSX, WINBOX_POSY);
	UIBase::SetDimensions(WINBOX_WIDTH, WINBOX_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_NO_FOCUS | GFX_FLAG::GFX_HIDDEN);

	// Initialize
	{
		TMSGBOX_PREPARE(manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, WINBOX_POSX, WINBOX_POSY, WINBOX_WIDTH, WINBOX_HEIGHT);
		TGLAYOUT_PREPARE(SkillBack, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, SKILL_BACK_POSX, SKILL_BACK_POSY, SKILL_BACK_WIDTH, SKILL_BACK_HEIGHT);
		
		_SkillDiv = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], SCREEN_WIDTH, SCREEN_HEIGHT, SKILL_DIV_POSX, SKILL_DIV_POSY, SKILL_DIV_WIDTH, SKILL_DIV_HEIGHT, EVector2(0.0f, 0.0f), EVector2(1.0f, 4.0f / 12.0f));
		_SkillDiv->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_LeftArrow = new UIButton(MESSAGEBOX::MESSAGE_HELPERSKILLSELECT_LEFT_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, LEFTARROW_POSX, LEFTARROW_POSY, LEFTARROW_WIDTH, LEFTARROW_HEIGHT);
		_LeftArrow->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_LeftArrow->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::INGAMESHOP_PAGE_LEFT], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_LeftArrow->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::INGAMESHOP_PAGE_LEFT], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_LeftArrow->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::INGAMESHOP_PAGE_LEFT], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_LeftArrow->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::INGAMESHOP_PAGE_LEFT], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));

		_RightArrow = new UIButton(MESSAGEBOX::MESSAGE_HELPERSKILLSELECT_RIGHT_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, RIGHTARROW_POSX, RIGHTARROW_POSY, RIGHTARROW_WIDTH, RIGHTARROW_HEIGHT);
		_RightArrow->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_RightArrow->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::INGAMESHOP_PAGE_RIGHT], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_RightArrow->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::INGAMESHOP_PAGE_RIGHT], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_RightArrow->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::INGAMESHOP_PAGE_RIGHT], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_RightArrow->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::INGAMESHOP_PAGE_RIGHT], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));

		for (mu_uint32 n = 0; n < MAX_SKILL_SHOW; ++n)
		{
			_SkillSelect[n] = new UIButton(MESSAGEBOX::MESSAGE_HELPERSKILLSELECT_SKILL_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, SKILL_ICON_POSX(n), SKILL_ICON_POSY, SKILL_ICON_WIDTH, SKILL_ICON_HEIGHT);
			_SkillSelect[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_SkillSelect[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);

			_SkillSelectBorder[n] = new UISpriteColor(manager, this, MU::MakeARGB(0, 50, 0, 100), SCREEN_WIDTH, SCREEN_HEIGHT, SKILL_ICON_POSX(n), SKILL_ICON_POSY, SKILL_ICON_WIDTH, SKILL_ICON_HEIGHT);
			_SkillSelectBorder[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_SkillSelectBorder[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		
		_OkButton = new UIButton(MESSAGEBOX::MESSAGE_HELPERSKILLSELECT_OKBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, OK_POSX, OK_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_OkButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_OkButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_OkButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
	}
}

UIHelperSkillSelect::~UIHelperSkillSelect()
{

}

void UIHelperSkillSelect::Update(mu_uint64 CurrentTime)
{
}

void UIHelperSkillSelect::PostDraw()
{
	DrawFrame();
	DrawText();
	DrawButton();
}

void UIHelperSkillSelect::DrawFrame()
{
	TMSGBOX_RENDERBACK;
	TGLAYOUT_RENDERBACK(SkillBack);
	_SkillDiv->Draw();
	TGLAYOUT_RENDERFRONT(SkillBack);
	TMSGBOX_RENDERFRONT;
}

void UIHelperSkillSelect::DrawText()
{
	mu_float screenX = _AspectWidth;
	mu_float screenY = _AspectHeight;
	mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255);
	mu_uint32 bgColor = MU::MakeARGB(0, 0, 0, 0);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::HELPERSKILLSELECT_TITLE,
		MU_TO_WINDOW(SKILL_TITLE_POSX, screenX),
		MU_TO_WINDOW(SKILL_TITLE_POSY, screenY),
		1.0f,
		ETA_CENTER,
		ETA_TOP,
		0.0f,
		textColor,
		bgColor,
		GlobalText[15000]);
}

void UIHelperSkillSelect::DrawButton()
{
	_LeftArrow->Draw();
	_RightArrow->Draw();

	for (mu_uint32 n = 0; n < MAX_SKILL_SHOW; ++n)
	{
		_SkillSelect[n]->Draw();
		_SkillSelectBorder[n]->Draw();
	}

	_OkButton->Draw();
	_OkButton->PostDraw();
}

mu_uint32 UIHelperSkillSelect::OnPreKeyPressed(const SDL_Keysym &key)
{
	switch (key.scancode)
	{
	case SDL_SCANCODE_RETURN:
	case SDL_SCANCODE_KP_ENTER:
		{
			_SceneManager->OnGFxMessagePush(_OkButton->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
		}
		return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

UIBase *UIHelperSkillSelect::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _OkButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _LeftArrow->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _RightArrow->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		for (mu_uint32 n = 0; n < MAX_SKILL_SHOW; ++n)
		{
			object = _SkillSelect[n]->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}
		}
	}

	return focus;
}

void GenerateSkillIcon(mu_uint32 skillIndex, mu_boolean &CantSkill, mu_float &fU, mu_float &fV, mu_int32 &iKindofSkill)
{
	mu_uint16 skillType = -1;
	if (skillIndex >= AT_PET_COMMAND_DEFAULT &&
		skillIndex < AT_PET_COMMAND_END)
	{
		skillType = skillIndex;
	}
	else
	{
		skillType = CharacterAttribute->Skill[skillIndex];
	}

	SKILL_ATTRIBUTE &Skill = SkillAttribute[skillType];

	CantSkill = false;

	if (DemendConditionCheckSkill(skillType) == false)
	{
		CantSkill = true;
	}
	else if (IsCanBCSkill(skillType) == false)
	{
		CantSkill = true;
	}
	else if (g_isCharacterBuff((&Hero->Object), eBuff_AddSkill) && Skill.SkillUseType == SKILL_USE_TYPE_BRAND)
	{
		CantSkill = true;
	}

	if (skillType == AT_SKILL_SPEAR)
	{
		if (Hero->Helper.Type != MODEL_HELPER + 2 &&
			Hero->Helper.Type != MODEL_HELPER + 3 &&
			Hero->Helper.Type != MODEL_HELPER + 37)
		{
			CantSkill = true;
		}
		else
		{
			mu_int32 iTypeL = CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Type;
			mu_int32 iTypeR = CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT].Type;

			if ((iTypeL < ITEM_SPEAR || iTypeL >= ITEM_BOW)
				&& (iTypeR < ITEM_SPEAR || iTypeR >= ITEM_BOW))
			{
				CantSkill = true;
			}
		}
	}

	if (skillType >= AT_SKILL_BLOCKING &&
		skillType <= AT_SKILL_SWORD5 &&
		(Hero->Helper.Type == MODEL_HELPER + 2 ||
			Hero->Helper.Type == MODEL_HELPER + 3 ||
			Hero->Helper.Type == MODEL_HELPER + 37))
	{
		CantSkill = true;
	}

	if ((skillType == AT_SKILL_ICE_BLADE ||
		(AT_SKILL_POWER_SLASH_UP <= skillType &&
			AT_SKILL_POWER_SLASH_UP + 4 >= skillType)) &&
			(Hero->Helper.Type == MODEL_HELPER + 2 ||
				Hero->Helper.Type == MODEL_HELPER + 3 ||
				Hero->Helper.Type == MODEL_HELPER + 37))
	{
		CantSkill = true;
	}

	mu_int32 Energy = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;
	mu_int32 Charisma = CharacterAttribute->Charisma + CharacterAttribute->AddCharisma;

	if (g_csItemOption.IsDisableSkill(skillType, Energy, Charisma) == true)
	{
		CantSkill = true;
	}

	if (skillType == AT_SKILL_PARTY_TELEPORT &&
		PartyNumber <= 0)
	{
		CantSkill = true;
	}

	if (skillType == AT_SKILL_PARTY_TELEPORT &&
		(IsDoppelGanger1() ||
			IsDoppelGanger2() ||
			IsDoppelGanger3() ||
			IsDoppelGanger4()))
	{
		CantSkill = true;
	}

	if (skillType == AT_SKILL_DARK_HORSE ||
		(AT_SKILL_ASHAKE_UP <= skillType &&
			skillType <= AT_SKILL_ASHAKE_UP + 4))
	{
		mu_uint8 byDarkHorseLife = CharacterMachine->Equipment[GAME::EQUIPMENT_HELPER].Durability;

		if (byDarkHorseLife == 0 ||
			Hero->Helper.Type != MODEL_HELPER + 4)
		{
			CantSkill = true;
		}
	}

	if (skillType == AT_SKILL_INFINITY_ARROW ||
		skillType == AT_SKILL_SWELL_OF_MAGICPOWER)
	{
		if (g_csItemOption.IsDisableSkill(skillType, Energy))
		{
			CantSkill = true;
		}

		if (g_isCharacterBuff((&Hero->Object), eBuff_InfinityArrow) ||
			g_isCharacterBuff((&Hero->Object), eBuff_SwellOfMagicPower))
		{
			CantSkill = true;
		}
	}

	if (skillType == AT_SKILL_REDUCEDEFENSE ||
		(AT_SKILL_BLOOD_ATT_UP <= skillType &&
			skillType <= AT_SKILL_BLOOD_ATT_UP + 4))
	{
		const mu_uint32 wRequireStrength = 596;
		if (CharacterAttribute->Strength + CharacterAttribute->AddStrength < wRequireStrength)
		{
			CantSkill = true;
		}

		mu_int32 iTypeL = CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Type;
		mu_int32 iTypeR = CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT].Type;

		if (!(iTypeR != -1 &&
			(iTypeR < ITEM_STAFF || iTypeR >= ITEM_STAFF + MAX_ITEM_INDEX) &&
			(iTypeL < ITEM_STAFF || iTypeL >= ITEM_STAFF + MAX_ITEM_INDEX)))
		{
			CantSkill = true;
		}
	}

	switch (skillType)
	{
	case AT_SKILL_PARALYZE:
		{
			const mu_uint32 wRequireDexterity = 646;
			if (CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity < wRequireDexterity)
			{
				CantSkill = true;
			}
		}
		break;
	}

	if (skillType == AT_SKILL_WHEEL ||
		(AT_SKILL_TORNADO_SWORDA_UP <= skillType && skillType <= AT_SKILL_TORNADO_SWORDA_UP + 4) ||
		(AT_SKILL_TORNADO_SWORDB_UP <= skillType && skillType <= AT_SKILL_TORNADO_SWORDB_UP + 4))
	{
		mu_int32 iTypeL = CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Type;
		mu_int32 iTypeR = CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT].Type;

		if (!(iTypeR != -1 && (iTypeR < ITEM_STAFF || iTypeR >= ITEM_STAFF + MAX_ITEM_INDEX) && (iTypeL < ITEM_STAFF || iTypeL >= ITEM_STAFF + MAX_ITEM_INDEX)))
		{
			CantSkill = true;
		}
	}

	if (InChaosCastle() == true)
	{
		if (skillType == AT_SKILL_DARK_HORSE || skillType == AT_SKILL_RIDER
			|| (skillType >= AT_PET_COMMAND_DEFAULT && skillType < AT_PET_COMMAND_END)
			|| (AT_SKILL_ASHAKE_UP <= skillType && skillType <= AT_SKILL_ASHAKE_UP + 4))
		{
			CantSkill = true;
		}
	}
	else
	{
		if (skillType == AT_SKILL_DARK_HORSE ||
			(AT_SKILL_ASHAKE_UP <= skillType &&
				skillType <= AT_SKILL_ASHAKE_UP + 4))
		{
			mu_uint16 byDarkHorseLife = CharacterMachine->Equipment[GAME::EQUIPMENT_HELPER].Durability;

			if (byDarkHorseLife == 0)
			{
				CantSkill = true;
			}
		}
	}

	iKindofSkill = 0;

	if (g_csItemOption.Special_Option_Check() == false &&
		(skillType == AT_SKILL_ICE_BLADE ||
		(AT_SKILL_POWER_SLASH_UP <= skillType &&
			AT_SKILL_POWER_SLASH_UP + 4 >= skillType)))
	{
		CantSkill = true;
	}

	if (g_csItemOption.Special_Option_Check(1) == false &&
		(skillType == AT_SKILL_CROSSBOW ||
		(AT_SKILL_MANY_ARROW_UP <= skillType &&
			AT_SKILL_MANY_ARROW_UP + 4 >= skillType)))
	{
		CantSkill = true;
	}

	if (skillType >= AT_PET_COMMAND_DEFAULT && skillType <= AT_PET_COMMAND_END)
	{
		fU = ((skillType - AT_PET_COMMAND_DEFAULT) % 8) * SKILL_U;
		fV = ((skillType - AT_PET_COMMAND_DEFAULT) / 8) * SKILL_V;
		iKindofSkill = KOS_COMMAND;
	}
	else if (skillType == AT_SKILL_PLASMA_STORM_FENRIR)
	{
		fU = 4 * SKILL_U;
		fV = 0.0f;
		iKindofSkill = KOS_COMMAND;
	}
	else if (skillType >= AT_SKILL_ALICE_DRAINLIFE && skillType <= AT_SKILL_ALICE_THORNS)
	{
		fU = ((skillType - AT_SKILL_ALICE_DRAINLIFE) % 8) * SKILL_U;
		fV = 3 * SKILL_V;
		iKindofSkill = KOS_SKILL2;
	}
	else if (skillType >= AT_SKILL_ALICE_SLEEP && skillType <= AT_SKILL_ALICE_BLIND)
	{
		fU = ((skillType - AT_SKILL_ALICE_SLEEP + 4) % 8) * SKILL_U;
		fV = 3 * SKILL_V;
		iKindofSkill = KOS_SKILL2;
	}
	else if (skillType == AT_SKILL_ALICE_BERSERKER)
	{
		fU = 10 * SKILL_U;
		fV = 3 * SKILL_V;
		iKindofSkill = KOS_SKILL2;
	}
	else if (skillType >= AT_SKILL_ALICE_WEAKNESS && skillType <= AT_SKILL_ALICE_ENERVATION)
	{
		fU = (skillType - AT_SKILL_ALICE_WEAKNESS + 8) * SKILL_U;
		fV = 3 * SKILL_V;
		iKindofSkill = KOS_SKILL2;
	}
	else if (skillType >= AT_SKILL_SUMMON_EXPLOSION && skillType <= AT_SKILL_SUMMON_REQUIEM)
	{
		fU = ((skillType - AT_SKILL_SUMMON_EXPLOSION + 6) % 8) * SKILL_U;
		fV = 3 * SKILL_V;
		iKindofSkill = KOS_SKILL2;
	}
	else if (skillType == AT_SKILL_SUMMON_POLLUTION)
	{
		fU = 11 * SKILL_U;
		fV = 3 * SKILL_V;
		iKindofSkill = KOS_SKILL2;
	}
	else if (skillType == AT_SKILL_BLOW_OF_DESTRUCTION)
	{
		fU = 7 * SKILL_U;
		fV = 2 * SKILL_V;
		iKindofSkill = KOS_SKILL2;
	}
	else if (skillType == AT_SKILL_GAOTIC)
	{
		fU = 3 * SKILL_U;
		fV = 8 * SKILL_V;
		iKindofSkill = KOS_SKILL2;
	}
	else if (skillType == AT_SKILL_RECOVER)
	{
		fU = 9 * SKILL_U;
		fV = 2 * SKILL_V;
		iKindofSkill = KOS_SKILL2;
	}
	else if (skillType == AT_SKILL_MULTI_SHOT)
	{
		if (GetEquipedBowType_Skill() == BOWTYPE_NONE)
		{
			CantSkill = true;
		}

		fU = 0 * SKILL_U;
		fV = 8 * SKILL_V;
		iKindofSkill = KOS_SKILL2;
	}
	else if (skillType == AT_SKILL_FLAME_STRIKE)
	{
		mu_int32 iTypeL = CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Type;
		mu_int32 iTypeR = CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT].Type;

		if (!(iTypeR != -1 &&
			(iTypeR < ITEM_STAFF || iTypeR >= ITEM_STAFF + MAX_ITEM_INDEX) &&
			(iTypeL < ITEM_STAFF || iTypeL >= ITEM_STAFF + MAX_ITEM_INDEX)))
		{
			CantSkill = true;
		}

		fU = 1 * SKILL_U;
		fV = 8 * SKILL_V;
		iKindofSkill = KOS_SKILL2;
	}
	else if (skillType == AT_SKILL_GIGANTIC_STORM)
	{
		fU = 2 * SKILL_U;
		fV = 8 * SKILL_V;
		iKindofSkill = KOS_SKILL2;
	}
	else if (skillType == AT_SKILL_LIGHTNING_SHOCK)
	{
		fU = 2 * SKILL_U;
		fV = 3 * SKILL_V;
		iKindofSkill = KOS_SKILL2;
	}
	else if (AT_SKILL_LIGHTNING_SHOCK_UP <= skillType && skillType <= AT_SKILL_LIGHTNING_SHOCK_UP + 4)
	{
		fU = 6 * SKILL_U;
		fV = 8 * SKILL_V;
		iKindofSkill = KOS_SKILL2;
	}
	else if (skillType == AT_SKILL_SWELL_OF_MAGICPOWER)
	{
		fU = 8 * SKILL_U;
		fV = 2 * SKILL_V;
		iKindofSkill = KOS_SKILL2;
	}
	else if (Skill.SkillUseType == SKILL_USE_TYPE_MASTERACTIVE)
	{
		fU = SKILL_U * (Skill.Magic_Icon % 12);
		fV = SKILL_V * ((Skill.Magic_Icon / 12) + 4);
		iKindofSkill = KOS_SKILL2;
	}
	else if (skillType >= 57)
	{
		fU = ((skillType - 57) % 8) * SKILL_U;
		fV = ((skillType - 57) / 8) * SKILL_V;
		iKindofSkill = KOS_SKILL2;
	}
	else
	{
		fU = ((skillType - 1) % 8) * SKILL_U;
		fV = ((skillType - 1) / 8) * SKILL_V;
		iKindofSkill = KOS_SKILL1;
	}
}

void ConfigureSkill(UIButton *button, mu_uint32 skillIndex)
{
	mu_boolean CantSkill;
	mu_float fU, fV;
	mu_int32 iKindofSkill;
	GenerateSkillIcon(skillIndex, CantSkill, fU, fV, iKindofSkill);

	if (CantSkill == true)
	{
		switch (iKindofSkill)
		{
		case KOS_COMMAND:
			{
				button->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_COMMAND], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(100, 100, 100, 255)));
				button->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_COMMAND], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(200, 200, 200, 255)));
				button->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_COMMAND], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_COMMAND], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(150, 150, 150, 255)));
			}
			break;
		default:
		case KOS_SKILL1:
			{
				button->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL1], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(100, 100, 100, 255)));
				button->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL1], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(200, 200, 200, 255)));
				button->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL1], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL1], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(150, 150, 150, 255)));
			}
			break;
		case KOS_SKILL2:
			{
				button->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL2], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(100, 100, 100, 255)));
				button->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL2], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(200, 200, 200, 255)));
				button->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL2], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL2], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(150, 150, 150, 255)));
			}
			break;
		}
	}
	else
	{
		switch (iKindofSkill)
		{
		case KOS_COMMAND:
			{
				button->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_COMMAND], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(100, 100, 100, 255)));
				button->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_COMMAND], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(200, 200, 200, 255)));
				button->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_COMMAND], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_COMMAND], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(150, 150, 150, 255)));
			}
			break;
		default:
		case KOS_SKILL1:
			{
				button->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(100, 100, 100, 255)));
				button->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(200, 200, 200, 255)));
				button->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(150, 150, 150, 255)));
			}
			break;
		case KOS_SKILL2:
			{
				button->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL2], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(100, 100, 100, 255)));
				button->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL2], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(200, 200, 200, 255)));
				button->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL2], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL2], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(150, 150, 150, 255)));
			}
			break;
		}
	}
}

void ConfigureSkill(UISprite *icon, mu_uint32 skillIndex)
{
	mu_boolean CantSkill;
	mu_float fU, fV;
	mu_int32 iKindofSkill;
	GenerateSkillIcon(skillIndex, CantSkill, fU, fV, iKindofSkill);

	if (CantSkill == true)
	{
		switch (iKindofSkill)
		{
		case KOS_COMMAND:
			{
				icon->SetTexture(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_COMMAND]);
			}
			break;
		default:
		case KOS_SKILL1:
			{
				icon->SetTexture(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL1]);
			}
			break;
		case KOS_SKILL2:
			{
				icon->SetTexture(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL2]);
			}
			break;
		}
	}
	else
	{
		switch (iKindofSkill)
		{
		case KOS_COMMAND:
			{
				icon->SetTexture(&INTERFACES::Data[INTERFACES::MAINFRAME_COMMAND]);
			}
			break;
		default:
		case KOS_SKILL1:
			{
				icon->SetTexture(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1]);
			}
			break;
		case KOS_SKILL2:
			{
				icon->SetTexture(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL2]);
			}
			break;
		}
	}

	icon->SetUV(EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V));
}

mu_boolean CheckSkillType(mu_uint32 SkillIndex, ESKILL_TYPE eSkillType)
{
	switch (eSkillType)
	{
	case EST_ATTACK:
		return CheckSkillType(SkillIndex, EST_BUFF) == false;

	case EST_DEFENSE:
		return CheckSkillType(SkillIndex, EST_BUFF) == false;

	case EST_BUFF:
		{
			switch (SkillIndex)
			{
				// Dark Knight
			case AT_SKILL_VITALITY: return true;

				// Dark Wizard
			case AT_SKILL_WIZARDDEFENSE: return true;
			case AT_SKILL_SWELL_OF_MAGICPOWER: return true;

				// Elf
			case AT_SKILL_DEFENSE: return true;
			case AT_SKILL_ATTACK: return true;
			case AT_SKILL_INFINITY_ARROW: return true;

				// Dark Lord
			case AT_SKILL_ADD_CRITICAL: return true;

				// Summoner
			case AT_SKILL_ALICE_THORNS: return true;
			case AT_SKILL_ALICE_BERSERKER: return true;
			}
		}
		break;
	}

	return false;
}

void UIHelperSkillSelect::ConfigureIcons()
{
	mu_uint32 maxMagic = MAX_MAGIC;
	for (mu_int32 n = MAX_MAGIC - 1; n >= 0; --n)
	{
		mu_int32 skillType = CharacterAttribute->Skill[n];

		if (skillType != 0 &&
			(skillType < AT_SKILL_STUN ||
				skillType > AT_SKILL_REMOVAL_BUFF))
		{
			mu_uint8 SkillUseType = SkillAttribute[skillType].SkillUseType;

			if (SkillUseType != SKILL_USE_TYPE_MASTER &&
				SkillUseType != SKILL_USE_TYPE_MASTERLEVEL)
			{
				break;
			}
		}

		--maxMagic;
	}

	mu_uint32 avalMagics = 0;
	for (mu_uint32 n = 0; n < maxMagic; ++n)
	{
		mu_int32 skillType = CharacterAttribute->Skill[n];

		if (skillType != 0 &&
			(skillType < AT_SKILL_STUN ||
				skillType > AT_SKILL_REMOVAL_BUFF))
		{
			mu_uint8 SkillUseType = SkillAttribute[skillType].SkillUseType;

			if (SkillUseType != SKILL_USE_TYPE_MASTER &&
				SkillUseType != SKILL_USE_TYPE_MASTERLEVEL &&
				CheckSkillType(skillType, _SkillType) == true)
			{
				++avalMagics;
			}
		}
	}

	_MaxSkillPage = (mu_uint32)EMath::Ceil((mu_float)avalMagics / (mu_float)MAX_SKILL_SHOW);

	for (mu_uint32 n = 0; n < MAX_SKILL_SHOW; ++n)
	{
		_SkillSelect[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		_SkillSelectBorder[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}

	if (_MaxSkillPage == 0)
	{
		_SkillPage = 0;
	}
	else if (_SkillPage >= _MaxSkillPage)
	{
		_SkillPage = _MaxSkillPage - 1;
	}

	mu_uint32 n = 0;
	for (mu_uint32 k = 0; k < _SkillPage; ++k)
	{
		mu_uint32 j = 0;
		for (; n < maxMagic; ++n)
		{
			mu_int32 skillType = CharacterAttribute->Skill[n];

			if (skillType != 0 &&
				(skillType < AT_SKILL_STUN ||
					skillType > AT_SKILL_REMOVAL_BUFF))
			{
				mu_uint8 SkillUseType = SkillAttribute[skillType].SkillUseType;

				if (SkillUseType != SKILL_USE_TYPE_MASTER &&
					SkillUseType != SKILL_USE_TYPE_MASTERLEVEL &&
					CheckSkillType(skillType, _SkillType) == true)
				{
					if (++j == MAX_SKILL_SHOW)
					{
						++n;
						break;
					}
				}
			}
		}

		if (j != MAX_SKILL_SHOW)
			break;
	}

	for (mu_uint32 k = 0; n < maxMagic && k < MAX_SKILL_SHOW; ++n)
	{
		mu_int32 skillType = CharacterAttribute->Skill[n];

		if (skillType != 0 &&
			(skillType < AT_SKILL_STUN ||
				skillType > AT_SKILL_REMOVAL_BUFF))
		{
			mu_uint8 SkillUseType = SkillAttribute[skillType].SkillUseType;

			if (SkillUseType != SKILL_USE_TYPE_MASTER &&
				SkillUseType != SKILL_USE_TYPE_MASTERLEVEL &&
				CheckSkillType(skillType, _SkillType) == true)
			{
				_SkillSelectIndex[k] = n;
				_SkillSelect[k]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
				ConfigureSkill(_SkillSelect[k], n);

				if (_SelectedSkill == n)
				{
					_SkillSelectBorder[k]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
				}

				++k;
			}
		}
	}
}

void UIHelperSkillSelect::Configure(mu_uint32 SkillSlot, mu_int32 SkillIndex, ESKILL_TYPE eSkillType)
{
	_SkillPage = 0;
	_SkillSlot = SkillSlot;
	_SelectedSkill = SkillIndex;
	_SkillType = eSkillType;

	ConfigureIcons();
}