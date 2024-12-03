#include "stdafx.h"
#include "mu_uimainframe.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

UIMainFrame::UIMainFrame(UISceneManager *manager, mu_float x, mu_float y, mu_float width, mu_float height) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(x, y);
	UIBase::SetDimensions(width, height);

	// Initialize
	{
		_Frame1 = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MAINFRAME_MENU_1], SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 429.0f, 256.0f, 51.0f);
		_Frame2 = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MAINFRAME_MENU_2], SCREEN_WIDTH, SCREEN_HEIGHT, 256.0f, 429.0f, 128.0f, 51.0f);
		_Frame3 = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MAINFRAME_MENU_3], SCREEN_WIDTH, SCREEN_HEIGHT, 384.0f, 429.0f, 256.0f, 51.0f);
		_FrameSkill = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MAINFRAME_MENU_2_1], SCREEN_WIDTH, SCREEN_HEIGHT, 222.0f, 429.0f, 160.0f, 40.0f);
		_LifeBar = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MAINFRAME_GAUGE_RED], SCREEN_WIDTH, SCREEN_HEIGHT, 158.0f, 432.0f, 45.0f, 39.0f);
		_ManaBar = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MAINFRAME_GAUGE_BLUE], SCREEN_WIDTH, SCREEN_HEIGHT, 437.0f, 432.0f, 45.0f, 39.0f);
		_ShieldBar = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MAINFRAME_GAUGE_SD], SCREEN_WIDTH, SCREEN_HEIGHT, 204.0f, 431.0f, 16.0f, 39.0f);
		_AgBar = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MAINFRAME_GAUGE_AG], SCREEN_WIDTH, SCREEN_HEIGHT, 420.0f, 431.0f, 16.0f, 39.0f);
		_ExperienceBar = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MAINFRAME_GAUGE_EXBAR], SCREEN_WIDTH, SCREEN_HEIGHT, 3.7f, 473.0f, 625.7f, 3.5f, EVector2(0.0f, 0.0f), EVector2(0.75f, 1.0f), MU::MakeARGB(255, 255, 255, 208));
		_ExperienceBar2 = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MAINFRAME_GAUGE_EXBAR], SCREEN_WIDTH, SCREEN_HEIGHT, 3.7f, 473.0f, 625.7f, 3.5f, EVector2(0.0f, 0.0f), EVector2(0.75f, 1.0f), MU::MakeARGB(255, 255, 255, 208));
		_ExperienceColor = new UISpriteColor(manager, this, MU::MakeARGB(255, 255, 255, 153), SCREEN_WIDTH, SCREEN_HEIGHT, 3.7f, 473.0f, 625.7f, 3.5f);
		_ExperienceColor->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_CharInfoBtn = new UIToggleButton(GAMEUI::MESSAGE_CHARACTERINFO_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, 519.0f, 429.0f, 30.0f, 41.0f);
		_CharInfoBtn->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_CHARINFO], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(255, 255, 255, 255)));
		_CharInfoBtn->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_CHARINFO], EVector2(0.0f, 0.25f), EVector2(1.0f, 0.50f), MU::MakeARGB(255, 255, 255, 255)));
		_CharInfoBtn->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_CHARINFO], EVector2(0.0f, 0.50f), EVector2(1.0f, 0.75f), MU::MakeARGB(255, 255, 255, 255)));
		_CharInfoBtn->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_CHARINFO], EVector2(0.0f, 0.75f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_CharInfoBtn->SetTooltip(FONT_NORMAL, GlobalText[362], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_MyInvenBtn = new UIToggleButton(GAMEUI::MESSAGE_MYINVENTORY_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, 549.0f, 429.0f, 30.0f, 41.0f);
		_MyInvenBtn->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_INVENTORY], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(255, 255, 255, 255)));
		_MyInvenBtn->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_INVENTORY], EVector2(0.0f, 0.25f), EVector2(1.0f, 0.50f), MU::MakeARGB(255, 255, 255, 255)));
		_MyInvenBtn->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_INVENTORY], EVector2(0.0f, 0.50f), EVector2(1.0f, 0.75f), MU::MakeARGB(255, 255, 255, 255)));
		_MyInvenBtn->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_INVENTORY], EVector2(0.0f, 0.75f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_MyInvenBtn->SetTooltip(FONT_NORMAL, GlobalText[363], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_FriendBtn = new UIToggleButton(GAMEUI::MESSAGE_FRIENDLIST_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, 579.0f, 429.0f, 30.0f, 41.0f);
		_FriendBtn->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_FRIEND], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(255, 255, 255, 255)));
		_FriendBtn->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_FRIEND], EVector2(0.0f, 0.25f), EVector2(1.0f, 0.50f), MU::MakeARGB(255, 255, 255, 255)));
		_FriendBtn->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_FRIEND], EVector2(0.0f, 0.50f), EVector2(1.0f, 0.75f), MU::MakeARGB(255, 255, 255, 255)));
		_FriendBtn->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_FRIEND], EVector2(0.0f, 0.75f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_FriendBtn->SetTooltip(FONT_NORMAL, GlobalText[1043], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_OptionsBtn = new UIToggleButton(GAMEUI::MESSAGE_WINDOW_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, 609.0f, 429.0f, 30.0f, 41.0f);
		_OptionsBtn->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_OPTION], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(255, 255, 255, 255)));
		_OptionsBtn->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_OPTION], EVector2(0.0f, 0.25f), EVector2(1.0f, 0.50f), MU::MakeARGB(255, 255, 255, 255)));
		_OptionsBtn->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_OPTION], EVector2(0.0f, 0.50f), EVector2(1.0f, 0.75f), MU::MakeARGB(255, 255, 255, 255)));
		_OptionsBtn->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_OPTION], EVector2(0.0f, 0.75f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_OptionsBtn->SetTooltip(FONT_NORMAL, GlobalText[1744], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_CashShopBtn = new UIToggleButton(GAMEUI::MESSAGE_CASHSHOP_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, 489.0f, 429.0f, 30.0f, 41.0f);
		_CashShopBtn->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_CASHSHOP], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(255, 255, 255, 255)));
		_CashShopBtn->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_CASHSHOP], EVector2(0.0f, 0.25f), EVector2(1.0f, 0.50f), MU::MakeARGB(255, 255, 255, 255)));
		_CashShopBtn->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_CASHSHOP], EVector2(0.0f, 0.50f), EVector2(1.0f, 0.75f), MU::MakeARGB(255, 255, 255, 255)));
		_CashShopBtn->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MAINFRAME_MENU_BTN_CASHSHOP], EVector2(0.0f, 0.75f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_CashShopBtn->SetTooltip(FONT_NORMAL, GlobalText[2277], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);
	}

	_ExperienceEffect = false;
	_EffectTime = 0;
	_PreExperience = 0;
	_GetExperience = 0;
	_PreMasterExperience = 0;
	_GetMasterExperience = 0;
}

UIMainFrame::~UIMainFrame()
{

}

void UIMainFrame::Update(mu_uint64 CurrentTime)
{
	if (_ExperienceEffect == true)
	{
		if (g_CurrentTime - _EffectTime > 2000)
		{
			_ExperienceEffect = false;
			_EffectTime = 0;
			_GetExperience = 0;
		}
	}
}

void UIMainFrame::Draw()
{
	DrawFrames();
	DrawLifeMana();
	DrawGaugeSD();
	DrawGaugeAG();
	DrawButtons();
	DrawExperience();
}

void UIMainFrame::PostDraw()
{
	PostDrawButtons();
}

void UIMainFrame::DrawFrames()
{
	_Frame1->Draw();
	_Frame2->Draw();
	_Frame3->Draw();
}

void UIMainFrame::DrawButtons()
{
	_CharInfoBtn->Draw();
	_MyInvenBtn->Draw();
	_FriendBtn->Draw();
	_OptionsBtn->Draw();
	_CashShopBtn->Draw();
}

void UIMainFrame::PostDrawButtons()
{
	_CharInfoBtn->PostDraw();
	_MyInvenBtn->PostDraw();
	_FriendBtn->PostDraw();
	_OptionsBtn->PostDraw();
	_CashShopBtn->PostDraw();
}

void UIMainFrame::DrawLifeMana()
{
	mu_float wLifeMax, wLife, wManaMax, wMana;

	wLifeMax = CharacterAttribute->LifeMax;
	wLife = EMath::Min(EMath::Max(0.0f, CharacterAttribute->Life), wLifeMax);
	wManaMax = CharacterAttribute->ManaMax;
	wMana = EMath::Min(EMath::Max(0.0f, CharacterAttribute->Mana), wManaMax);

	mu_float Life = wLifeMax == 0.0f ? 0.0f : (mu_float)wLife / (mu_float)wLifeMax;
	mu_float Mana = wManaMax == 0.0f ? 0.0f : (mu_float)wMana / (mu_float)wManaMax;
	mu_float RLife = 1.0f - Life;
	mu_float RMana = 1.0f - Mana;

	if (Life > 0.0f && Life < 0.2f)
	{
		PlayBackground(SOUND_HEART);
	}

	// Life
	mu_float width = 45.0f, height = 39.0f;
	mu_float x = 158.0f, y = 432.0f;
	mu_float fY = y + (RLife * height);
	mu_float fH = height - (RLife * height);

	if (Life > 0.0f)
	{
		if (g_isCharacterBuff((&Hero->Object), eDeBuff_Poison))
		{
			_LifeBar->SetTexture(&INTERFACES::Data[INTERFACES::MAINFRAME_GAUGE_GREEN]);
		}
		else
		{
			_LifeBar->SetTexture(&INTERFACES::Data[INTERFACES::MAINFRAME_GAUGE_RED]);
		}

		_LifeBar->SetPosition(x, fY);
		_LifeBar->SetDimensions(width, fH);
		_LifeBar->SetUV(EVector2(0.0f, RLife), EVector2(1.0f, RLife + Life));
		_LifeBar->Draw();
	}

	mu_text buffer[128];
	mu_sprintf_s(buffer, mu_countof(buffer), _T("%d"), (mu_int32)wLife);

	FONT_NUMBER->DrawStringOutlineC2D(TEXTINDEX::LIFE_NUMBER,
		MU_TO_WINDOW(x + 25.0f, _AspectWidth),
		MU_TO_WINDOW(480.0f - 18.0f, _AspectHeight),
		0.8f,
		ETA_CENTER,
		ETA_BOTTOM,
		0.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(0, 0, 0, 0),
		MU::MakeARGB(0, 0, 0, 255),
		buffer);

	if (CheckInside(MU_TO_WINDOW(x, _AspectWidth),
					MU_TO_WINDOW(y, _AspectHeight),
					MU_TO_WINDOW(width, _AspectWidth),
					MU_TO_WINDOW(height, _AspectHeight)) == true)
	{
		mu_sprintf_s(buffer, mu_countof(buffer), GlobalText[358], (mu_int32)wLife, (mu_int32)wLifeMax);

		FONT_NORMAL->DrawStringC2D(TEXTINDEX::LIFE_TOOLTIP,
			MU_TO_WINDOW(x + width * 0.5f, _AspectWidth),
			MU_TO_WINDOW(y - 4.0f, _AspectHeight),
			1.0f,
			ETA_CENTER,
			ETA_TOP,
			2.0f,
			MU::MakeARGB(255, 255, 255, 255),
			MU::MakeARGB(0, 0, 0, 180),
			buffer);
	}

	// Mana
	width = 45.0f, height = 39.0f;
	x = 437.0f, y = 432.0f;
	fY = y + (RMana * height);
	fH = height - (RMana * height);

	if (Mana > 0.0f)
	{
		_ManaBar->SetPosition(x, fY);
		_ManaBar->SetDimensions(width, fH);
		_ManaBar->SetUV(EVector2(0.0f, RMana), EVector2(1.0f, RMana + Mana));
		_ManaBar->Draw();
	}

	mu_sprintf_s(buffer, mu_countof(buffer), _T("%d"), (mu_int32)wMana);

	FONT_NUMBER->DrawStringOutlineC2D(TEXTINDEX::MANA_NUMBER,
		MU_TO_WINDOW(x + 20.0f, _AspectWidth),
		MU_TO_WINDOW(480.0f - 18.0f, _AspectHeight),
		0.8f,
		ETA_CENTER,
		ETA_BOTTOM,
		0.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(0, 0, 0, 0),
		MU::MakeARGB(0, 0, 0, 255),
		buffer);

	if (CheckInside(MU_TO_WINDOW(x, _AspectWidth),
					MU_TO_WINDOW(y, _AspectHeight),
					MU_TO_WINDOW(width, _AspectWidth),
					MU_TO_WINDOW(height, _AspectHeight)) == true)
	{
		mu_sprintf_s(buffer, mu_countof(buffer), GlobalText[359], (mu_int32)wMana, (mu_int32)wManaMax);

		FONT_NORMAL->DrawStringC2D(TEXTINDEX::MANA_TOOLTIP,
			MU_TO_WINDOW(x + width * 0.5f, _AspectWidth),
			MU_TO_WINDOW(y - 4.0f, _AspectHeight),
			1.0f,
			ETA_CENTER,
			ETA_TOP,
			2.0f,
			MU::MakeARGB(255, 255, 255, 255),
			MU::MakeARGB(0, 0, 0, 180),
			buffer);
	}
}

void UIMainFrame::DrawGaugeSD()
{
	mu_float wMaxShield, wShield;

	wMaxShield = CharacterAttribute->ShieldMax;
	wShield = EMath::Min(EMath::Max(0.0f, CharacterAttribute->Shield), wMaxShield);

	mu_float Shield = wMaxShield == 0.0f ? 0.0f : wShield / wMaxShield;
	mu_float RShield = 1.0f - Shield;

	// Shield
	mu_float width = 16.0f, height = 39.0f;
	mu_float x = 204.0f, y = 431.0f;
	mu_float fY = y + (RShield * height);
	mu_float fH = height - (RShield * height);

	if (Shield > 0.0f)
	{
		_ShieldBar->SetPosition(x, fY);
		_ShieldBar->SetDimensions(width, fH);
		_ShieldBar->SetUV(EVector2(0.0f, RShield), EVector2(1.0f, RShield + Shield));
		_ShieldBar->Draw();
	}

	mu_text buffer[128];
	mu_sprintf_s(buffer, mu_countof(buffer), _T("%d"), (mu_int32)wShield);

	FONT_NUMBER->DrawStringOutlineC2D(TEXTINDEX::SHIELD_NUMBER,
		MU_TO_WINDOW(x + 8.0f, _AspectWidth),
		MU_TO_WINDOW(480.0f - 18.0f, _AspectHeight),
		0.8f,
		ETA_CENTER,
		ETA_BOTTOM,
		0.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(0, 0, 0, 0),
		MU::MakeARGB(0, 0, 0, 255),
		buffer);

	if (CheckInside(MU_TO_WINDOW(x, _AspectWidth),
		MU_TO_WINDOW(y, _AspectHeight),
		MU_TO_WINDOW(width, _AspectWidth),
		MU_TO_WINDOW(height, _AspectHeight)) == true)
	{
		mu_sprintf_s(buffer, mu_countof(buffer), GlobalText[2037], (mu_int32)wShield, (mu_int32)wMaxShield);

		FONT_NORMAL->DrawStringC2D(TEXTINDEX::SHIELD_TOOLTIP,
			MU_TO_WINDOW(x + width * 0.5f, _AspectWidth),
			MU_TO_WINDOW(y - 4.0f, _AspectHeight),
			1.0f,
			ETA_CENTER,
			ETA_TOP,
			2.0f,
			MU::MakeARGB(255, 255, 255, 255),
			MU::MakeARGB(0, 0, 0, 180),
			buffer);
	}
}

void UIMainFrame::DrawGaugeAG()
{
	mu_float wMaxBP, wBP;

	wMaxBP = CharacterAttribute->SkillManaMax;
	wBP = EMath::Min(EMath::Max(0.0f, CharacterAttribute->SkillMana), wMaxBP);

	mu_float BP = wMaxBP == 0.0f ? 0.0f : wBP / wMaxBP;
	mu_float RBP = 1.0f - BP;

	// AG
	mu_float width = 16.0f, height = 39.0f;
	mu_float x = 420.0f, y = 431.0f;
	mu_float fY = y + (RBP * height);
	mu_float fH = height - (RBP * height);

	if (BP > 0.0f)
	{
		_AgBar->SetPosition(x, fY);
		_AgBar->SetDimensions(width, fH);
		_AgBar->SetUV(EVector2(0.0f, RBP), EVector2(1.0f, RBP + BP));
		_AgBar->Draw();
	}

	mu_text buffer[128];
	mu_sprintf_s(buffer, mu_countof(buffer), _T("%d"), (mu_int32)wBP);

	FONT_NUMBER->DrawStringOutlineC2D(TEXTINDEX::AG_NUMBER,
		MU_TO_WINDOW(x + 8.0f, _AspectWidth),
		MU_TO_WINDOW(480.0f - 18.0f, _AspectHeight),
		0.8f,
		ETA_CENTER,
		ETA_BOTTOM,
		0.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(0, 0, 0, 0),
		MU::MakeARGB(0, 0, 0, 255),
		buffer);

	if (CheckInside(MU_TO_WINDOW(x, _AspectWidth),
		MU_TO_WINDOW(y, _AspectHeight),
		MU_TO_WINDOW(width, _AspectWidth),
		MU_TO_WINDOW(height, _AspectHeight)) == true)
	{
		mu_sprintf_s(buffer, mu_countof(buffer), GlobalText[214], (mu_int32)wBP, (mu_int32)wMaxBP);

		FONT_NORMAL->DrawStringC2D(TEXTINDEX::AG_TOOLTIP,
			MU_TO_WINDOW(x + width * 0.5f, _AspectWidth),
			MU_TO_WINDOW(y - 4.0f, _AspectHeight),
			1.0f,
			ETA_CENTER,
			ETA_TOP,
			2.0f,
			MU::MakeARGB(255, 255, 255, 255),
			MU::MakeARGB(0, 0, 0, 180),
			buffer);
	}
}

void UIMainFrame::DrawExperience()
{
	mu_int64 wLevel;
	mu_int64 dwNexExperience;
	mu_int64 dwExperience;
	mu_double x, y, width, height;

	mu_text buffer[256];

	if (IsMasterLevel(CharacterAttribute->Class) == true)
	{
		wLevel = (mu_int64)Master_Level_Data.nMLevel;
		dwNexExperience = Master_Level_Data.lNext_MasterLevel_Experince;
		dwExperience = Master_Level_Data.lMasterLevel_Experince;
	}
	else
	{
		wLevel = CharacterAttribute->Level;
		dwNexExperience = CharacterAttribute->NextExperince;
		dwExperience = CharacterAttribute->Experience;
	}

	_ExperienceBar->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
	_ExperienceBar2->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

	if (IsMasterLevel(CharacterAttribute->Class) == true)
	{
		x = 0; y = 470; width = 6; height = 4;

		mu_int64 iTotalLevel = wLevel + 400;
		mu_int64 iTOverLevel = iTotalLevel - 255;
		mu_int64 iBaseExperience = 0;

		mu_int64 iData_Master =
			(
			(
				(mu_int64)9 + (mu_int64)iTotalLevel
				)
				* (mu_int64)iTotalLevel
				* (mu_int64)iTotalLevel
				* (mu_int64)10
				)
			+
			(
			(
				(mu_int64)9 + (mu_int64)iTOverLevel
				)
				* (mu_int64)iTOverLevel
				* (mu_int64)iTOverLevel
				* (mu_int64)1000
				);
		iBaseExperience = (iData_Master - (mu_int64)3892250000) / (mu_int64)2;

		mu_double fNeedExp = (mu_double)dwNexExperience - (mu_double)iBaseExperience;
		mu_double fExp = (mu_double)dwExperience - (mu_double)iBaseExperience;

		if (dwExperience < iBaseExperience)
		{
			fExp = 0.0f;
		}

		mu_double fExpBarNum = 0.0f;
		if (fExp > 0.0f && fNeedExp > 0)
		{
			fExpBarNum = ((mu_double)fExp / (mu_double)fNeedExp) * (mu_double)10.0f;
		}

		mu_double fProgress = fExpBarNum - mu_int64(fExpBarNum);

		_ExperienceBar->SetTexture(&INTERFACES::Data[INTERFACES::MAINFRAME_MASTER_GAUGE_BAR]);
		_ExperienceBar2->SetTexture(&INTERFACES::Data[INTERFACES::MAINFRAME_MASTER_GAUGE_BAR]);

		if (_ExperienceEffect == true)
		{
			mu_double fPreProgress = 0.0f;
			mu_double fPreExp = (mu_double)_PreMasterExperience - (mu_double)iBaseExperience;
			if (_PreMasterExperience < iBaseExperience)
			{
				x = 3.7f; y = 473.0f; width = fProgress * 625.7f; height = 3.5f;
				_ExperienceBar->SetDimensions(width, height);
				_ExperienceBar->Draw();
				_ExperienceColor->SetPosition(x, y);
				_ExperienceColor->SetColor(MU::MakeARGB(255, 255, 255, 153));
				_ExperienceColor->SetDimensions(width, height);
				_ExperienceColor->Draw();
			}
			else
			{
				mu_int32 iPreExpBarNum = 0;
				mu_int32 iExpBarNum = 0;
				if (fPreExp > 0.0f && fNeedExp > 0.0f)
				{
					fPreProgress = ((mu_double)fPreExp / (mu_double)fNeedExp) * (mu_double)10.0f;
					iPreExpBarNum = (mu_int32)fPreProgress;
					fPreProgress = (mu_double)fPreProgress - mu_int64(fPreProgress);
				}
				iExpBarNum = (mu_int32)fExpBarNum;

				if (iExpBarNum > iPreExpBarNum)
				{
					x = 3.7f; y = 473.0f; width = fProgress * 625.7f; height = 3.5f;
					_ExperienceBar->SetDimensions(width, height);
					_ExperienceBar->Draw();
					_ExperienceColor->SetPosition(x, y);
					_ExperienceColor->SetColor(MU::MakeARGB(255, 255, 255, 153));
					_ExperienceColor->SetDimensions(width, height);
					_ExperienceColor->Draw();
				}
				else
				{
					mu_double fGapProgress = 0.0f;
					fGapProgress = (mu_double)fProgress - (mu_double)fPreProgress;
					x = 3.7f; y = 473.0f; width = (mu_double)fPreProgress * 625.7f; height = 3.5f;
					_ExperienceBar->SetDimensions(width, height);
					_ExperienceBar->Draw();
					x += width; width = (mu_double)fGapProgress * 625.7f;
					_ExperienceBar2->SetPosition(x, y);
					_ExperienceBar2->SetDimensions(width, height);
					_ExperienceColor->SetPosition(x, y);
					_ExperienceColor->SetColor(MU::MakeARGB(255, 255, 255, 153));
					_ExperienceColor->SetDimensions(width, height);
					_ExperienceColor->Draw();
				}
			}
		}
		else
		{
			x = 3.7f; y = 473.0f; width = fProgress * 625.7f; height = 3.5f;
			_ExperienceBar->SetDimensions(width, height);
			_ExperienceBar->Draw();
		}

		mu_int32 iExp = (mu_int32)fExpBarNum;
		x = 635.0f; y = 474.5f;
		mu_sprintf_s(buffer, mu_countof(buffer), _T("%d"), iExp);
		FONT_NUMBER->DrawStringOutlineC2D(TEXTINDEX::EXPERIENCE_NUMBER,
			MU_TO_WINDOW(x, _AspectWidth),
			MU_TO_WINDOW(y, _AspectHeight),
			1.0f,
			ETA_CENTER,
			ETA_CENTER,
			0.0f,
			MU::MakeARGB(255, 255, 255, 255),
			MU::MakeARGB(0, 0, 0, 0),
			MU::MakeARGB(0, 0, 0, 255),
			buffer);

		x = 2.0f; y = 472.0f; width = 629.0f; height = 4.0f;
		if (CheckInside(MU_TO_WINDOW(x, _AspectWidth),
			MU_TO_WINDOW(y, _AspectHeight),
			MU_TO_WINDOW(width, _AspectWidth),
			MU_TO_WINDOW(height, _AspectHeight)) == true)
		{
			mu_sprintf_s(buffer, mu_countof(buffer), GlobalText[1748], dwExperience, dwNexExperience);

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::EXPERIENCE_TOOLTIP,
				MU_TO_WINDOW(280, _AspectWidth),
				MU_TO_WINDOW(418, _AspectHeight),
				1.0f,
				ETA_CENTER,
				ETA_TOP,
				2.0f,
				MU::MakeARGB(255, 255, 255, 255),
				MU::MakeARGB(0, 0, 0, 180),
				buffer);
		}
	}
	else
	{
		x = 0; y = 470; width = 6; height = 4;

		mu_uint32 wPriorLevel = wLevel - 1;
		mu_uint32 dwPriorExperience = 0;

		if (wPriorLevel > 0)
		{
			dwPriorExperience = (9 + wPriorLevel) * wPriorLevel * wPriorLevel * 10;

			if (wPriorLevel > 255)
			{
				mu_int32 iLevelOverN = wPriorLevel - 255;
				dwPriorExperience += (9 + iLevelOverN) * iLevelOverN * iLevelOverN * 1000;
			}
		}

		mu_double fNeedExp = dwNexExperience - dwPriorExperience;
		mu_double fExp = dwExperience - dwPriorExperience;

		if (dwExperience < dwPriorExperience)
		{
			fExp = 0.0;
		}

		mu_double fExpBarNum = 0.0;
		if (fExp > 0.0 && fNeedExp > 0.0)
		{
			fExpBarNum = (fExp / fNeedExp) * 10.0;
		}

		mu_float fProgress = fExpBarNum;
		fProgress = fProgress - (mu_int32)fProgress;

		_ExperienceBar->SetTexture(&INTERFACES::Data[INTERFACES::MAINFRAME_GAUGE_EXBAR]);
		_ExperienceBar2->SetTexture(&INTERFACES::Data[INTERFACES::MAINFRAME_GAUGE_EXBAR]);

		if (_ExperienceEffect == true)
		{
			mu_float fPreProgress = 0.0f;
			fExp = _PreExperience - dwPriorExperience;
			if (_PreExperience < dwPriorExperience)
			{
				x = 3.7f; y = 473.0f; width = fProgress * 625.7f; height = 3.5f;
				_ExperienceBar->SetDimensions(width, height);
				_ExperienceBar->Draw();
				_ExperienceColor->SetPosition(x, y);
				_ExperienceColor->SetColor(MU::MakeARGB(255, 255, 255, 102));
				_ExperienceColor->SetDimensions(width, height);
				_ExperienceColor->Draw();
			}
			else
			{
				mu_int32 iPreExpBarNum = 0;
				mu_int32 iExpBarNum = 0;
				if (fExp > 0.0 && fNeedExp > 0.0)
				{
					fPreProgress = (fExp / fNeedExp) * 10.0;
					iPreExpBarNum = (mu_int32)fPreProgress;
					fPreProgress = fPreProgress - (mu_int32)fPreProgress;
				}

				iExpBarNum = (mu_int32)fExpBarNum;

				if (iExpBarNum > iPreExpBarNum)
				{
					x = 3.7f; y = 473.0f; width = fProgress * 625.7f; height = 3.5f;
					_ExperienceBar->SetDimensions(width, height);
					_ExperienceBar->Draw();
					_ExperienceColor->SetPosition(x, y);
					_ExperienceColor->SetColor(MU::MakeARGB(255, 255, 255, 102));
					_ExperienceColor->SetDimensions(width, height);
					_ExperienceColor->Draw();
				}
				else
				{
					mu_float fGapProgress = 0.0f;
					fGapProgress = fProgress - fPreProgress;
					x = 3.7f; y = 473.0f; width = fPreProgress * 625.7f; height = 3.5f;
					_ExperienceBar->SetDimensions(width, height);
					_ExperienceBar->Draw();
					x += width; width = (mu_double)fGapProgress * (mu_double)625.7f;
					_ExperienceBar2->SetPosition(x, y);
					_ExperienceBar2->SetDimensions(width, height);
					_ExperienceColor->SetPosition(x, y);
					_ExperienceColor->SetColor(MU::MakeARGB(255, 255, 255, 102));
					_ExperienceColor->SetDimensions(width, height);
					_ExperienceColor->Draw();
				}
			}
		}
		else
		{
			x = 3.7f; y = 473.0f; width = fProgress * 625.7f; height = 3.5f;
			_ExperienceBar->SetDimensions(width, height);
			_ExperienceBar->Draw();
		}

		mu_int32 iExp = (mu_int32)fExpBarNum;
		x = 635.0f; y = 474.5f;
		mu_sprintf_s(buffer, mu_countof(buffer), _T("%d"), iExp);
		FONT_NUMBER->DrawStringOutlineC2D(TEXTINDEX::EXPERIENCE_NUMBER,
			MU_TO_WINDOW(x, _AspectWidth),
			MU_TO_WINDOW(y, _AspectHeight),
			1.0f,
			ETA_CENTER,
			ETA_CENTER,
			0.0f,
			MU::MakeARGB(255, 255, 255, 255),
			MU::MakeARGB(0, 0, 0, 0),
			MU::MakeARGB(0, 0, 0, 255),
			buffer);

		x = 2.0f; y = 472.0f; width = 629.0f; height = 4.0f;
		if (CheckInside(MU_TO_WINDOW(x, _AspectWidth),
			MU_TO_WINDOW(y, _AspectHeight),
			MU_TO_WINDOW(width, _AspectWidth),
			MU_TO_WINDOW(height, _AspectHeight)) == true)
		{
			mu_sprintf_s(buffer, mu_countof(buffer), GlobalText[1748], dwExperience, dwNexExperience);

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::EXPERIENCE_TOOLTIP,
				MU_TO_WINDOW(640.0f * 0.5f, _AspectWidth),
				MU_TO_WINDOW(427.0f, _AspectHeight),
				1.0f,
				ETA_CENTER,
				ETA_TOP,
				2.0f,
				MU::MakeARGB(255, 255, 255, 255),
				MU::MakeARGB(0, 0, 0, 180),
				buffer);
		}
	}
}

UIBase *UIMainFrame::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);
	
	if(focus != nullptr)
	{
		UIToggleButton *buttons[] = { _CharInfoBtn, _MyInvenBtn, _FriendBtn, _OptionsBtn, _CashShopBtn };

		for (mu_uint32 index = 0; index < mu_countof(buttons); ++index)
		{
			UIBase *object = buttons[index]->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}
		}

		UISprite *frames[] = { _Frame1, _Frame2 ,_Frame3 };

		for (mu_uint32 index = 0; index < mu_countof(frames); ++index)
		{
			UIBase *object = frames[index]->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}
		}
	}

	return focus;
}

void UIMainFrame::SetPreMasterExperience(mu_int64 PreExp)
{
	_PreMasterExperience = PreExp;
}

void UIMainFrame::SetGetMasterExperience(mu_int64 GetExp)
{
	_GetMasterExperience = GetExp;

	if (_GetMasterExperience > 0)
	{
		_ExperienceEffect = true;
		_EffectTime = g_CurrentTime;
	}
}

void UIMainFrame::SetPreExperience(mu_int64 PreExp)
{
	_PreExperience = PreExp;
}

void UIMainFrame::SetGetExperience(mu_int64 GetExp)
{
	_GetExperience = GetExp;

	if (_GetExperience > 0)
	{
		_ExperienceEffect = true;
		_EffectTime = g_CurrentTime;
	}
}

void UIMainFrame::ChangeOptionButton()
{
	if (g_pOptionRoot->IsVisible(OPTIONUI::INTERFACE_GAMEMENU) == true)
	{
		_OptionsBtn->ChangeState(UIToggleButton::UI_PRESSED);
	}
	else
	{
		_OptionsBtn->ChangeState(_SceneManager->GetFocused() == _OptionsBtn ? UIToggleButton::UI_OVER : UIToggleButton::UI_NORMAL);
	}
}