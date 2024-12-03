#include "stdafx.h"
#include "mu_uiskillframe.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define SKILL_WIDTH 20.0f
#define SKILL_HEIGHT 28.0f
#define SKILL_U (20.0f/256.0f)
#define SKILL_V (28.0f/256.0f)
#define SKILL_PERLINE 14

#define USEBOX_WIDTH 32.0f
#define USEBOX_HEIGHT 38.0f

#define SKILLLIST_POSX 304.0f
#define SKILLLIST_POSY 390.0f

#define HOTKEY_POSX 222.0f
#define HOTKEY_POSY 432.0f

#define CURRENT_POSX 392.0f
#define CURRENT_POSY 438.0f

UISkillFrame::UISkillFrame(UISceneManager *manager)
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Initialize
	{
		for (mu_uint32 n = 0; n < MAX_MAGIC; ++n)
		{
			_SkillIcon[n] = new UIButton(GAMEUI::MESSAGE_SKILLFRAME_SKILLLIST_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, SKILL_WIDTH, SKILL_HEIGHT);
			_SkillIcon[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_SkillIcon[n]->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(100, 100, 100, 255)));
			_SkillIcon[n]->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
			_SkillIcon[n]->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
			_SkillIcon[n]->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));

			_SkillDelay[n] = new UISpriteColor(manager, this, MU::MakeARGB(255, 127, 127, 127), SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
			_SkillDelay[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_SkillDelay[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

			_SkillUsebox[n] = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MAINFRAME_SKILLBOX2], SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, USEBOX_WIDTH, USEBOX_HEIGHT);
			_SkillUsebox[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}

		for (mu_uint32 n = 0; n < 4; ++n)
		{
			_CommandIcon[n] = new UIButton(GAMEUI::MESSAGE_SKILLFRAME_COMMANDLIST_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, SKILL_WIDTH, SKILL_HEIGHT);
			_CommandIcon[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_CommandIcon[n]->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(100, 100, 100, 255)));
			_CommandIcon[n]->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
			_CommandIcon[n]->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
			_CommandIcon[n]->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));

			_CommandDelay[n] = new UISpriteColor(manager, this, MU::MakeARGB(255, 127, 127, 127), SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
			_CommandDelay[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_CommandDelay[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

			_CommandUsebox[n] = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MAINFRAME_SKILLBOX2], SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, USEBOX_WIDTH, USEBOX_HEIGHT);
			_CommandUsebox[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}

		for (mu_uint32 n = 0; n < VISIBLEKEY_COUNT; ++n)
		{
			_HotkeyIcon[n] = new UIButton(GAMEUI::MESSAGE_SKILLFRAME_HOTKEYLIST_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, SKILL_WIDTH, SKILL_HEIGHT);
			_HotkeyIcon[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_HotkeyIcon[n]->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(100, 100, 100, 255)));
			_HotkeyIcon[n]->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
			_HotkeyIcon[n]->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
			_HotkeyIcon[n]->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));

			_HotkeyDelay[n] = new UISpriteColor(manager, this, MU::MakeARGB(255, 127, 127, 127), SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
			_HotkeyDelay[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_HotkeyDelay[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

			_HotkeyUsebox[n] = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MAINFRAME_SKILLBOX2], SCREEN_WIDTH, SCREEN_HEIGHT, HOTKEY_POSX + (n * USEBOX_WIDTH), HOTKEY_POSY, USEBOX_WIDTH, USEBOX_HEIGHT);
			_HotkeyUsebox[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}

		_CurrentIcon = new UIButton(GAMEUI::MESSAGE_SKILLFRAME_CURRENTSKILL, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CURRENT_POSX, CURRENT_POSY, SKILL_WIDTH, SKILL_HEIGHT);
		_CurrentIcon->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(100, 100, 100, 255)));
		_CurrentIcon->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_CurrentIcon->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_CurrentIcon->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));

		_CurrentDelay = new UISpriteColor(manager, this, MU::MakeARGB(255, 127, 127, 127), SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
		_CurrentDelay->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		_CurrentDelay->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_ShowSkillList = false;

		for (mu_uint32 n = 0; n < HOTKEY_COUNT; ++n)
		{
			_HotkeyIndex[n] = -1;
		}
	}
}

UISkillFrame::~UISkillFrame()
{

}

void UISkillFrame::Update(mu_uint64 CurrentTime)
{
	for (mu_uint32 n = 0; n < MAX_MAGIC; ++n)
	{
		if (CharacterAttribute->Skill[n] > 0 &&
			CharacterAttribute->SkillDelay[n] > 0 &&
			_SkillIcon[n]->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			_SkillDelay[n]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);

			SKILL_ATTRIBUTE &Skill = SkillAttribute[CharacterAttribute->Skill[n]];
			mu_float Percent = (mu_float)CharacterAttribute->SkillDelay[n] / (mu_float)Skill.Delay;

			_SkillDelay[n]->SetPosition(_SkillIcon[n]->_ViewX, _SkillIcon[n]->_ViewY + (_SkillIcon[n]->_ViewHeight * (1.0f - Percent)));
			_SkillDelay[n]->SetDimensions(_SkillIcon[n]->_ViewWidth, _SkillIcon[n]->_ViewHeight * Percent);
		}
		else
		{
			_SkillDelay[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
	}

	for (mu_uint32 n = 0; n < 4; ++n)
	{
		if (CharacterAttribute->Skill[AT_PET_COMMAND_DEFAULT + n] > 0 &&
			CharacterAttribute->SkillDelay[AT_PET_COMMAND_DEFAULT + n] > 0 &&
			_CommandIcon[n]->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			_SkillDelay[n]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);

			SKILL_ATTRIBUTE &Skill = SkillAttribute[CharacterAttribute->Skill[AT_PET_COMMAND_DEFAULT + n]];
			mu_float Percent = (mu_float)CharacterAttribute->SkillDelay[AT_PET_COMMAND_DEFAULT + n] / (mu_float)Skill.Delay;

			_SkillDelay[n]->SetPosition(_SkillIcon[n]->_ViewX, _SkillIcon[n]->_ViewY + (_SkillIcon[n]->_ViewHeight * (1.0f - Percent)));
			_SkillDelay[n]->SetDimensions(_SkillIcon[n]->_ViewWidth, _SkillIcon[n]->_ViewHeight * Percent);
		}
		else
		{
			_SkillDelay[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
	}

	for (mu_uint32 n = 0; n < VISIBLEKEY_COUNT; ++n)
	{
		mu_int16 skillIndex = _HotkeyUseboxIndex[n + 1];

		if (skillIndex != -1 &&
			CharacterAttribute->Skill[skillIndex] > 0 &&
			CharacterAttribute->SkillDelay[skillIndex] > 0)
		{
			_HotkeyDelay[n]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);

			SKILL_ATTRIBUTE &Skill = SkillAttribute[CharacterAttribute->Skill[skillIndex]];
			mu_float Percent = (mu_float)CharacterAttribute->SkillDelay[skillIndex] / (mu_float)Skill.Delay;

			_HotkeyDelay[n]->SetPosition(_HotkeyIcon[n]->_ViewX, _HotkeyIcon[n]->_ViewY + (_HotkeyIcon[n]->_ViewHeight * (1.0f - Percent)));
			_HotkeyDelay[n]->SetDimensions(_HotkeyIcon[n]->_ViewWidth, _HotkeyIcon[n]->_ViewHeight * Percent);
		}
		else
		{
			_HotkeyDelay[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
	}

	if (CharacterAttribute->Skill[Hero->CurrentSkill] > 0 &&
		CharacterAttribute->SkillDelay[Hero->CurrentSkill] > 0)
	{
		_CurrentDelay->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);

		SKILL_ATTRIBUTE &Skill = SkillAttribute[CharacterAttribute->Skill[Hero->CurrentSkill]];
		mu_float Percent = (mu_float)CharacterAttribute->SkillDelay[Hero->CurrentSkill] / (mu_float)Skill.Delay;

		_CurrentDelay->SetPosition(_CurrentIcon->_ViewX, _CurrentIcon->_ViewY + (_CurrentIcon->_ViewHeight * (1.0f - Percent)));
		_CurrentDelay->SetDimensions(_CurrentIcon->_ViewWidth, _CurrentIcon->_ViewHeight * Percent);
	}
	else
	{
		_CurrentDelay->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}
}

void UISkillFrame::Draw()
{
	mu_boolean renderCurrent = CharacterAttribute->Skill[Hero->CurrentSkill] > 0;

	for (mu_uint32 n = 0; n < VISIBLEKEY_COUNT; ++n)
	{
		_HotkeyUsebox[n]->Draw();
	}

	//_CurrentUsebox->Draw();

	for (mu_uint32 n = 0; n < 4; ++n)
	{
		_CommandUsebox[n]->Draw();
	}

	for (mu_uint32 n = 0; n < MAX_MAGIC; ++n)
	{
		_SkillUsebox[n]->Draw();
	}

	///////////////////////////////////////////////////

	for (mu_uint32 n = 0; n < VISIBLEKEY_COUNT; ++n)
	{
		_HotkeyIcon[n]->Draw();
	}

	if (renderCurrent == true)
	{
		_CurrentIcon->Draw();
	}

	for (mu_uint32 n = 0; n < 4; ++n)
	{
		_CommandIcon[n]->Draw();
	}

	for (mu_uint32 n = 0; n < MAX_MAGIC; ++n)
	{
		_SkillIcon[n]->Draw();
	}

	///////////////////////////////////////////////////

	for (mu_uint32 n = 0; n < VISIBLEKEY_COUNT; ++n)
	{
		_HotkeyDelay[n]->Draw();
	}

	if (renderCurrent == true)
	{
		_CurrentDelay->Draw();
	}

	for (mu_uint32 n = 0; n < 4; ++n)
	{
		_CommandDelay[n]->Draw();
	}

	for (mu_uint32 n = 0; n < MAX_MAGIC; ++n)
	{
		_SkillDelay[n]->Draw();
	}

	static const mu_text *HotkeyStrings[10] = { _T("0"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9") };
	mu_uint32 hotkeyColor = MU::MakeARGB(255, 229, 204, 255);
	mu_uint32 backColor = MU::MakeARGB(0, 0, 0, 0);

	for (mu_uint32 n = 0; n < HOTKEY_COUNT; ++n)
	{
		mu_int32 skillType = _HotkeyIndex[n];

		if (skillType >= AT_PET_COMMAND_DEFAULT && skillType < AT_PET_COMMAND_END)
		{
			mu_int32 useboxIndex = _HotkeyUseboxIndex[n];

			if (useboxIndex != -1)
			{
				UISprite *usebox = _CommandUsebox[useboxIndex];

				if (usebox->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
				{
					FONT_NUMBER->DrawStringOutlineC2D(TEXTINDEX::BEGIN_SKILL_HOTKEY_NUMBER + n,
						MU_TO_WINDOW(usebox->_ViewX + 20.0f, _AspectWidth),
						MU_TO_WINDOW(usebox->_ViewY + 20.0f, _AspectHeight),
						1.0f,
						ETA_LEFT,
						ETA_BOTTOM,
						0.0f,
						hotkeyColor,
						backColor,
						MU::MakeARGB(0, 0, 0, 255),
						HotkeyStrings[n]);
				}
			}
		}
		else
		{
			mu_int32 useboxIndex = _HotkeyUseboxIndex[n];

			if (useboxIndex != -1)
			{
				UISprite *usebox = _SkillUsebox[useboxIndex];

				if (usebox->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
				{
					FONT_NUMBER->DrawStringOutlineC2D(TEXTINDEX::BEGIN_SKILL_HOTKEY_NUMBER + n,
						MU_TO_WINDOW(usebox->_ViewX + 20.0f, _AspectWidth),
						MU_TO_WINDOW(usebox->_ViewY + 20.0f, _AspectHeight),
						1.0f,
						ETA_LEFT,
						ETA_BOTTOM,
						0.0f,
						hotkeyColor,
						backColor,
						MU::MakeARGB(0, 0, 0, 255),
						HotkeyStrings[n]);
				}
			}
		}
	}
}

void UISkillFrame::PostDraw()
{
	mu_int32 mouseX = MU_INPUT->GetMouseX();
	mu_int32 mouseY = MU_INPUT->GetMouseY();

	for (mu_uint32 n = 0; n < VISIBLEKEY_COUNT; ++n)
	{
		mu_int32 hotkeyIndex = _HotkeyUseboxIndex[n + 1];
		if (hotkeyIndex != -1 &&
			CharacterAttribute->Skill[hotkeyIndex] > 0 &&
			_HotkeyIcon[n]->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false &&
			_HotkeyIcon[n]->IsInside(mouseX, mouseY) == true)
		{
			RenderSkillInfo(_HotkeyIcon[n]->_ViewX + SKILL_WIDTH * 0.5f, _HotkeyIcon[n]->_ViewY - 10.0f, hotkeyIndex, 0, STRP_BOTTOMCENTER);
			return;
		}
	}

	if (CharacterAttribute->Skill[Hero->CurrentSkill] > 0 &&
		_CurrentIcon->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false &&
		_CurrentIcon->IsInside(mouseX, mouseY) == true)
	{
		RenderSkillInfo(_CurrentIcon->_ViewX + SKILL_WIDTH * 0.5f, _CurrentIcon->_ViewY - 10.0f, Hero->CurrentSkill, 0, STRP_BOTTOMCENTER);
		return;
	}

	for (mu_uint32 n = 0; n < 4; ++n)
	{
		if (_CommandIcon[n]->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false &&
			_CommandIcon[n]->IsInside(mouseX, mouseY) == true)
		{
			RenderSkillInfo(_CommandIcon[n]->_ViewX + SKILL_WIDTH * 0.5f, _CommandIcon[n]->_ViewY - 10.0f, AT_PET_COMMAND_DEFAULT + n, 0, STRP_BOTTOMCENTER);
			return;
		}
	}

	for (mu_uint32 n = 0; n < MAX_MAGIC; ++n)
	{
		if (_SkillIcon[n]->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false &&
			_SkillIcon[n]->IsInside(mouseX, mouseY) == true)
		{
			RenderSkillInfo(_SkillIcon[n]->_ViewX + SKILL_WIDTH * 0.5f, _SkillIcon[n]->_ViewY - 10.0f, n, 0, STRP_BOTTOMCENTER);
			return;
		}
	}
}

void UISkillFrame::ClearSkills()
{
	mu_memset(_HotkeyIndex, -1, sizeof(_HotkeyIndex));
	UpdateHotkeySkill();
}

void UISkillFrame::ConfigureSkill(UIButton *button, UISpriteColor *delay, mu_uint32 skillIndex, mu_float x, mu_float y)
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
				button->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_COMMAND], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_COMMAND], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_COMMAND], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
			}
			break;
		default:
		case KOS_SKILL1:
			{
				button->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL1], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(100, 100, 100, 255)));
				button->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL1], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL1], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL1], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
			}
			break;
		case KOS_SKILL2:
			{
				button->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL2], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(100, 100, 100, 255)));
				button->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL2], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL2], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_NON_SKILL2], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
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
				button->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_COMMAND], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_COMMAND], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_COMMAND], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
			}
			break;
		default:
		case KOS_SKILL1:
			{
				button->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(100, 100, 100, 255)));
				button->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL1], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
			}
			break;
		case KOS_SKILL2:
			{
				button->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL2], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(100, 100, 100, 255)));
				button->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL2], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL2], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
				button->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILL2], EVector2(fU, fV), EVector2(fU + SKILL_U, fV + SKILL_V), MU::MakeARGB(255, 255, 255, 255)));
			}
			break;
		}
	}

	button->SetPosition(x, y);
	delay->SetPosition(x, y);
}

void UISkillFrame::UpdateSkills()
{
	mu_memset(_SkillsTypeIndex, -1, sizeof(_SkillsTypeIndex));

	for (mu_uint32 n = 0; n < MAX_MAGIC; ++n)
	{
		mu_int32 skillType = CharacterAttribute->Skill[n];

		if (skillType == 0)
		{
			_SkillsTypeIndex[skillType] = -1;
		}
		else
		{
			_SkillsTypeIndex[skillType] = n;
		}
	}

	mu_int32 SkillCount = 0;

	for (mu_uint32 n = 0; n < MAX_MAGIC; ++n)
	{
		mu_uint16 skillType = CharacterAttribute->Skill[n];

		if (skillType > 0)
		{
			mu_uint8 SkillUseType = SkillAttribute[skillType].SkillUseType;

			if ((skillType < AT_SKILL_STUN || skillType > AT_SKILL_REMOVAL_BUFF) &&
				SkillUseType != SKILL_USE_TYPE_MASTER &&
				SkillUseType != SKILL_USE_TYPE_MASTERLEVEL)
			{
				++SkillCount;
			}
		}
	}

	mu_float OriginX[(MAX_MAGIC / SKILL_PERLINE) + 1] = { 0 };
	mu_int32 TmpCount = SkillCount;
	mu_int32 TmpIndex = 0, TmpIndex2 = 0;

	do 
	{
		mu_int32 count = EMath::Min(TmpCount, SKILL_PERLINE);

		mu_float tmpX = count * USEBOX_WIDTH;
		OriginX[TmpIndex] = SKILLLIST_POSX - tmpX * 0.5f;

		TmpCount -= count;
		++TmpIndex;
	} while (TmpCount > 0);

	TmpIndex = 0;
	
	mu_float x = OriginX[0], y = SKILLLIST_POSY;

	for (mu_uint32 n = 0; n < MAX_MAGIC; ++n)
	{
		mu_int32 skillType = CharacterAttribute->Skill[n];

		if (skillType == 0 ||
			(skillType >= AT_SKILL_STUN && skillType <= AT_SKILL_REMOVAL_BUFF))
		{
			_SkillIcon[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_SkillUsebox[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		else
		{
			mu_uint8 SkillUseType = SkillAttribute[skillType].SkillUseType;

			if (SkillUseType == SKILL_USE_TYPE_MASTER ||
				SkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
			{
				_SkillIcon[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
				_SkillUsebox[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			}
			else
			{
				if (_ShowSkillList == true)
				{
					_SkillIcon[n]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
					_SkillUsebox[n]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
				}
				else
				{
					_SkillIcon[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
					_SkillUsebox[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
				}

				if (n == Hero->CurrentSkill)
				{
					_SkillUsebox[n]->SetTexture(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILLBOX2]);
				}
				else
				{
					_SkillUsebox[n]->SetTexture(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILLBOX1]);
				}

				_SkillUsebox[n]->SetPosition(x + (USEBOX_WIDTH * TmpIndex2), y);
				ConfigureSkill(_SkillIcon[n], _SkillDelay[n], n, x + (USEBOX_WIDTH * TmpIndex2) + 6.0f, y + 6.0f);

				++TmpIndex;

				if (TmpIndex > 0 && TmpIndex % SKILL_PERLINE == 0)
				{
					x = OriginX[TmpIndex / SKILL_PERLINE];
					y -= USEBOX_HEIGHT;
					TmpIndex2 = 0;
				}
				else
				{
					++TmpIndex2;
				}
			}
		}
	}

	x = SKILLLIST_POSX - (USEBOX_WIDTH * 4.0f) * 0.5f;

	if (SkillCount > 0)
	{
		y -= USEBOX_HEIGHT;
	}

	for (mu_uint32 n = 0; n < 4; ++n)
	{
		if (CharacterAttribute->Skill[AT_PET_COMMAND_DEFAULT + n] == 0)
		{
			_CommandIcon[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_CommandUsebox[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		else
		{
			if (_ShowSkillList == true)
			{
				_CommandIcon[n]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
				_CommandUsebox[n]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			}
			else
			{
				_CommandIcon[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
				_CommandUsebox[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			}

			if (AT_PET_COMMAND_DEFAULT + n == Hero->CurrentSkill)
			{
				_CommandUsebox[n]->SetTexture(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILLBOX2]);
			}
			else
			{
				_CommandUsebox[n]->SetTexture(&INTERFACES::Data[INTERFACES::MAINFRAME_SKILLBOX1]);
			}

			_CommandUsebox[n]->SetPosition(x + (USEBOX_WIDTH * n), y);
			ConfigureSkill(_CommandIcon[n], _CommandDelay[n], AT_PET_COMMAND_DEFAULT + n, x + (USEBOX_WIDTH * n) + 6.0f, y + 6.0f);
		}
	}

	UpdateHotkeySkill();
	UpdateCurrentSkill();
}

void UISkillFrame::UpdateCurrentSkill()
{
	if (Hero->CurrentSkill >= AT_PET_COMMAND_DEFAULT &&
		Hero->CurrentSkill < AT_PET_COMMAND_END)
	{
		ConfigureSkill(_CurrentIcon, _CurrentDelay, Hero->CurrentSkill, CURRENT_POSX, CURRENT_POSY);
	}
	else if (CharacterAttribute->Skill[Hero->CurrentSkill] > 0)
	{
		ConfigureSkill(_CurrentIcon, _CurrentDelay, Hero->CurrentSkill, CURRENT_POSX, CURRENT_POSY);
	}
}

void UISkillFrame::UpdateHotkeySkill()
{
	for (mu_uint32 n = 0; n < HOTKEY_COUNT; ++n)
	{
		mu_int32 skillType = _HotkeyIndex[n];
		if (skillType == -1)
		{
			_HotkeyUseboxIndex[n] = -1;
		}
		else if (skillType >= AT_PET_COMMAND_DEFAULT && skillType < AT_PET_COMMAND_END)
		{
			_HotkeyUseboxIndex[n] = skillType - AT_PET_COMMAND_DEFAULT;
		}
		else
		{
			_HotkeyUseboxIndex[n] = _SkillsTypeIndex[skillType];
		}
	}

	for (mu_uint32 n = 0; n < HOTKEY_COUNT; ++n)
	{
		mu_int32 skillType = _HotkeyIndex[n];
		mu_int32 skillIndex = _HotkeyUseboxIndex[n];

		if (skillType >= AT_PET_COMMAND_DEFAULT &&
			skillType < AT_PET_COMMAND_END)
			continue;

		if (!(skillIndex >= 0 && skillIndex <= MAX_SKILLS) ||
			CharacterAttribute->Skill[skillIndex] == 0)
		{
			_HotkeyIndex[n] = -1;
			_HotkeyUseboxIndex[n] = -1;
		}
	}

	for (mu_uint32 n = 0; n < VISIBLEKEY_COUNT; ++n)
	{
		mu_int16 skillType = _HotkeyIndex[n + 1];

		if (skillType == -1 ||
			skillType == 0)
		{
			_HotkeyIcon[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_HotkeyUsebox[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		else
		{
			mu_int16 skillIndex = _HotkeyUseboxIndex[n + 1];

			if (_ShowSkillList == true)
			{
				_HotkeyIcon[n]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			}

			if (_HotkeyIcon[n]->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == true)
			{
				_HotkeyUsebox[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			}
			else
			{
				if (skillType >= AT_PET_COMMAND_DEFAULT &&
					skillType < AT_PET_COMMAND_END)
				{
					if (skillType == Hero->CurrentSkill)
					{
						_HotkeyUsebox[n]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
					}
					else
					{
						_HotkeyUsebox[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
					}
				}
				else
				{
					if (skillIndex == Hero->CurrentSkill)
					{
						_HotkeyUsebox[n]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
					}
					else
					{
						_HotkeyUsebox[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
					}
				}
			}

			if (skillType >= AT_PET_COMMAND_DEFAULT && skillType < AT_PET_COMMAND_END)
			{
				ConfigureSkill(_HotkeyIcon[n], _HotkeyDelay[n], skillType, HOTKEY_POSX + (n * USEBOX_WIDTH) + 6.0f, HOTKEY_POSY + 6.0f);
			}
			else
			{
				ConfigureSkill(_HotkeyIcon[n], _HotkeyDelay[n], skillIndex, HOTKEY_POSX + (n * USEBOX_WIDTH) + 6.0f, HOTKEY_POSY + 6.0f);
			}
		}
	}
}

UIBase *UISkillFrame::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *object = nullptr;

	if (_ShowSkillList == true)
	{
		for (mu_uint32 index = 0; index < MAX_MAGIC; ++index)
		{
			object = _SkillIcon[index]->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}
		}

		for (mu_uint32 index = 0; index < 4; ++index)
		{
			object = _CommandIcon[index]->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}
		}
	}

	for (mu_uint32 index = 0; index < VISIBLEKEY_COUNT; ++index)
	{
		object = _HotkeyUsebox[index]->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	object = _CurrentIcon->GetFocus(mouseX, mouseY);

	if (object != nullptr)
	{
		return object;
	}

	return nullptr;
}