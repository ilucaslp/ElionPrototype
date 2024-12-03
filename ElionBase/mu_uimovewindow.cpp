#include "stdafx.h"
#include "mu_uimovewindow.h"

#define SCREEN_WIDTH 800.0f
#define SCREEN_HEIGHT 600.0f

#define MOVEWINDOW_POSX 0.0f
#define MOVEWINDOW_POSY 0.0f
#define MOVEWINDOW_WIDTH 190.0f
#define MOVEWINDOW_BASEHEIGHT 60.0f

#define SCROLLBAR_POSX (MOVEWINDOW_POSX + MOVEWINDOW_WIDTH - 14.0f)
#define SCROLLBAR_POSY (MOVEWINDOW_POSY + 35.0f)
#define SCROLLBAR_WIDTH 7.0f
#define SCROLLBAR_EXHEIGHT 3.0f

#define SCROLLBAR_BTN_WIDTH 15.0f
#define SCROLLBAR_BTN_HEIGHT 30.0f

#define CLOSE_POSX (MOVEWINDOW_POSX + 2.5f)
#define CLOSE_WIDTH (MOVEWINDOW_WIDTH - 5.0f)

#define TITLE_POSX (MOVEWINDOW_POSX + MOVEWINDOW_WIDTH * 0.5f)
#define TITLE_POSY (MOVEWINDOW_POSY + 4.0f)
#define BATTLEZONE_POSX (MOVEWINDOW_POSX + 3.0f)
#define BATTLEZONE_POSY (MOVEWINDOW_POSY + 24.0f)
#define MAPNAME_POSX (MOVEWINDOW_POSX + 64.0f)
#define MAPNAME_POSY (MOVEWINDOW_POSY + 24.0f)
#define LEVEL_POSX (MOVEWINDOW_POSX + 119.0f)
#define LEVEL_POSY (MOVEWINDOW_POSY + 24.0f)
#define COST_POSX (MOVEWINDOW_POSX + 159.0f)
#define COST_POSY (MOVEWINDOW_POSY + 24.0f)

#define LINE_POSY (MOVEWINDOW_POSY + 38.0f)

UIMoveWindow::UIMoveWindow(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(MOVEWINDOW_POSX, MOVEWINDOW_POSY);

	SetGfxFlag(GFX_FLAG::GFX_HIDDEN);

	// Initialize
	{
		mu_float MaxAdvanceY = FONT_NORMAL->GetFont()->_MaxAdvanceY;
		mu_float RealHeight = MOVEWINDOW_BASEHEIGHT + (MaxAdvanceY * MOVEWINDOW_MAXLINES);

		_Background = new UISpriteColor(manager, this, MU::MakeARGB(0, 0, 0, 204), SCREEN_WIDTH, SCREEN_HEIGHT, MOVEWINDOW_POSX, MOVEWINDOW_POSY, MOVEWINDOW_WIDTH, RealHeight);
		_Background->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_CloseButton = new UIButton(GAMEUI::MESSAGE_MOVEWINDOW_CLOSE, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CLOSE_POSX, MOVEWINDOW_POSY + RealHeight - MaxAdvanceY - 6.0f, CLOSE_WIDTH, MaxAdvanceY);
		_CloseButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CloseButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(61, 0, 0, 255)));
		_CloseButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(153, 0, 0, 255)));
		_CloseButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(153, 0, 0, 255)));
		_CloseButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(153, 0, 0, 255)));
		_CloseButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[1002], MU::MakeARGB(150, 150, 150, 200));
		_CloseButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[1002], MU::MakeARGB(255, 255, 255, 255));
		_CloseButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[1002], MU::MakeARGB(255, 255, 255, 255));
		_CloseButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[1002], MU::MakeARGB(255, 255, 255, 255));

		mu_float ScrollHeight = (MOVEWINDOW_MAXLINES * MaxAdvanceY) - SCROLLBAR_EXHEIGHT * 2.0f;

		_ScrollBar = new UIScrollBar(GAMEUI::MESSAGE_MOVEWINDOW_SCROLLBAR, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, SCROLLBAR_POSX, SCROLLBAR_POSY, UIScrollBar::ORI_VERTICAL);
		_ScrollBar->ConfigureTop(&INTERFACES::Data[INTERFACES::SCROLLBAR_TOP], EBlendState::eAlphaSrc_InvAlphaSrc, SCROLLBAR_WIDTH, SCROLLBAR_EXHEIGHT);
		_ScrollBar->ConfigureMiddle(&INTERFACES::Data[INTERFACES::SCROLLBAR_MIDDLE], EBlendState::eAlphaSrc_InvAlphaSrc, SCROLLBAR_WIDTH, ScrollHeight);
		_ScrollBar->ConfigureBottom(&INTERFACES::Data[INTERFACES::SCROLLBAR_BOTTOM], EBlendState::eAlphaSrc_InvAlphaSrc, SCROLLBAR_WIDTH, SCROLLBAR_EXHEIGHT);
		_ScrollBar->ConfigureScroll(EBlendState::eAlphaSrc_InvAlphaSrc, SCROLLBAR_BTN_WIDTH, SCROLLBAR_BTN_HEIGHT, true);

		_ScrollBar->SetStateData(UIScrollBar::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_OFF], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ScrollBar->SetStateData(UIScrollBar::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ScrollBar->SetStateData(UIScrollBar::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ScrollBar->SetStateData(UIScrollBar::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(178, 178, 178, 255)));

		if (g_MoveManager.MoveData.size() > MOVEWINDOW_MAXLINES)
		{
			_ScrollBar->ConfigureScrollInfo(0.0f, (mu_float)g_MoveManager.MoveData.size() - MOVEWINDOW_MAXLINES, 0.0f);
		}
		else
		{
			_ScrollBar->ChangeState(UIScrollBar::UI_DISABLED);
		}

		_ScrollBar->ConfigureScrollDimension(ScrollHeight);

		_ScrollBar->ApplyChanges();

		UIBase::SetDimensions(_Background->_ViewWidth, _Background->_ViewHeight);

		for (mu_uint32 n = 0; n < MOVEWINDOW_MAXLINES; ++n)
		{
			_SelectButton[n] = new UIButton(GAMEUI::MESSAGE_MOVEWINDOW_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MOVEWINDOW_POSX + 2.5f, LINE_POSY + MaxAdvanceY * n, MOVEWINDOW_WIDTH - 17.5f, MaxAdvanceY);
			_SelectButton[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_SelectButton[n]->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 0, 0, 0)));
			_SelectButton[n]->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 0, 0, 0)));
			_SelectButton[n]->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(204, 204, 25, 153)));
			_SelectButton[n]->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(204, 204, 25, 153)));
		}

		_BeginIndex = 0;
		_SelectedIndex = -1;
	}
}

UIMoveWindow::~UIMoveWindow()
{

}

void UIMoveWindow::SystemConfigure()
{
	UIBase::SystemConfigure();

	mu_float MaxAdvanceY = FONT_NORMAL->GetFont()->_MaxAdvanceY;
	mu_float RealHeight = MOVEWINDOW_BASEHEIGHT + (MaxAdvanceY * MOVEWINDOW_MAXLINES);

	_Background->SetDimensions(MOVEWINDOW_WIDTH, RealHeight);

	_CloseButton->SetPosition(CLOSE_POSX, MOVEWINDOW_POSY + RealHeight - MaxAdvanceY - 6.0f);
	_CloseButton->SetDimensions(CLOSE_WIDTH, MaxAdvanceY);

	mu_float ScrollHeight = (MOVEWINDOW_MAXLINES * MaxAdvanceY) - SCROLLBAR_EXHEIGHT * 2.0f;

	_ScrollBar->ConfigureTop(&INTERFACES::Data[INTERFACES::SCROLLBAR_TOP], EBlendState::eAlphaSrc_InvAlphaSrc, SCROLLBAR_WIDTH, SCROLLBAR_EXHEIGHT);
	_ScrollBar->ConfigureMiddle(&INTERFACES::Data[INTERFACES::SCROLLBAR_MIDDLE], EBlendState::eAlphaSrc_InvAlphaSrc, SCROLLBAR_WIDTH, ScrollHeight);
	_ScrollBar->ConfigureBottom(&INTERFACES::Data[INTERFACES::SCROLLBAR_BOTTOM], EBlendState::eAlphaSrc_InvAlphaSrc, SCROLLBAR_WIDTH, SCROLLBAR_EXHEIGHT);
	_ScrollBar->ConfigureScroll(EBlendState::eAlphaSrc_InvAlphaSrc, SCROLLBAR_BTN_WIDTH, SCROLLBAR_BTN_HEIGHT, true);

	if (g_MoveManager.MoveData.size() > MOVEWINDOW_MAXLINES)
	{
		_ScrollBar->ConfigureScrollInfo(0.0f, (mu_float)g_MoveManager.MoveData.size() - MOVEWINDOW_MAXLINES, 0.0f);
	}
	else
	{
		_ScrollBar->ChangeState(UIScrollBar::UI_DISABLED);
	}

	_ScrollBar->ConfigureScrollDimension(ScrollHeight);

	_ScrollBar->ApplyChanges();

	UIBase::SetDimensions(_Background->_ViewWidth, _Background->_ViewHeight);

	for (mu_uint32 n = 0; n < MOVEWINDOW_MAXLINES; ++n)
	{
		_SelectButton[n]->SetPosition(MOVEWINDOW_POSX + 2.5f, LINE_POSY + MaxAdvanceY * n);
		_SelectButton[n]->SetDimensions(MOVEWINDOW_WIDTH - 17.5f, MaxAdvanceY);
	}
}

void UIMoveWindow::Draw()
{
	_Background->Draw();
	_CloseButton->Draw();
	_ScrollBar->Draw();

	mu_uint32 backColor = MU::MakeARGB(0, 0, 0, 0);
	mu_uint32 columnColor = MU::MakeARGB(127, 178, 255, 255);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::MAPWINDOW_TITLE,
		MU_TO_WINDOW(TITLE_POSX, _AspectWidth),
		MU_TO_WINDOW(TITLE_POSY, _AspectHeight),
		1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f,
		MU::MakeARGB(255, 204, 26, 255), backColor,
		GlobalText[933]);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::MAPWINDOW_BATTLEZONE_COLUMN,
		MU_TO_WINDOW(BATTLEZONE_POSX, _AspectWidth),
		MU_TO_WINDOW(BATTLEZONE_POSY, _AspectHeight),
		1.0f, ETA_LEFT, ETA_CENTER, 0.0f,
		columnColor, backColor,
		GlobalText[2988]);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::MAPWINDOW_MAPNAME_COLUMN,
		MU_TO_WINDOW(MAPNAME_POSX, _AspectWidth),
		MU_TO_WINDOW(MAPNAME_POSY, _AspectHeight),
		1.0f, ETA_CENTER, ETA_CENTER, 0.0f,
		columnColor, backColor,
		GlobalText[934]);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::MAPWINDOW_LEVEL_COLUMN,
		MU_TO_WINDOW(LEVEL_POSX, _AspectWidth),
		MU_TO_WINDOW(LEVEL_POSY, _AspectHeight),
		1.0f, ETA_CENTER, ETA_CENTER, 0.0f,
		columnColor, backColor,
		GlobalText[935]);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::MAPWINDOW_COST_COLUMN,
		MU_TO_WINDOW(COST_POSX, _AspectWidth),
		MU_TO_WINDOW(COST_POSY, _AspectHeight),
		1.0f, ETA_CENTER, ETA_CENTER, 0.0f,
		columnColor, backColor,
		GlobalText[936]);

	mu_boolean isSpecialLevel = false;
	if (GetBaseClass(CharacterAttribute->Class) == GAME::CLASS_DARK ||
		GetBaseClass(CharacterAttribute->Class) == GAME::CLASS_DARK_LORD)
	{
		isSpecialLevel = true;
	}

	mu_int32 PlayerLevel = CharacterAttribute->Level;
	mu_int64 CurrentGold = CharacterMachine->Gold;

	mu_uint32 maxRender = _BeginIndex + MOVEWINDOW_MAXLINES;
	if (maxRender > (mu_uint32)g_MoveManager.MoveData.size())
	{
		maxRender = (mu_uint32)g_MoveManager.MoveData.size();
	}

	mu_float MaxAdvanceY = FONT_NORMAL->GetFont()->_MaxAdvanceY;
	for (mu_uint32 j = 0, n = _BeginIndex; n < maxRender; ++j, ++n)
	{
		_SelectButton[j]->Draw();

		MoveRequirementDataEx &data = g_MoveManager.MoveData[n];
		mu_float liney = LINE_POSY + MaxAdvanceY * j + MaxAdvanceY * 0.5f;
		mu_uint32 textColor = data.CanMove == true ? MU::MakeARGB(255, 255, 255, 255) : MU::MakeARGB(164, 39, 17, 255);

		mu_text textBuffer[256];
		mu_int32 RequiredLevel = data.MinimumLevel;

		if (isSpecialLevel == true &&
			data.MinimumLevel != MAX_LEVEL)
		{
			mu_boolean isKalima = false;
			mu_uint32 kalimaLevel = 0;

			const mu_string &kalimaName = GlobalText.Get(58);
			if (mu_strnicmp(data.MainMapName, kalimaName.c_str(), kalimaName.size()) == 0)
			{
				if (kalimaName.empty() == false &&
					kalimaName.size() < mu_countof(data.MainMapName) &&
					mu_isdigit(data.MainMapName[kalimaName.size()]))
				{
					isKalima = true;
					kalimaLevel = data.MainMapName[kalimaName.size()] - _T('0');
				}
			}

			if (isKalima == true)
			{
				if (kalimaLevel <= 6)
				{
					RequiredLevel = RequiredLevel - 20;
				}
			}
			else
			{
				RequiredLevel = ((RequiredLevel / 3) * 2);
			}
		}

		if(data.IsStrife == true)
		{
			FONT_NORMAL->DrawStringC2D(TEXTINDEX::MAPWINDOW_BATTLEZONE_BEGIN + j,
				MU_TO_WINDOW(BATTLEZONE_POSX, _AspectWidth),
				MU_TO_WINDOW(liney, _AspectHeight),
				1.0f, ETA_LEFT, ETA_CENTER, 0.0f,
				textColor, backColor,
				GlobalText[2987]);
		}

		FONT_NORMAL->DrawStringC2D(TEXTINDEX::MAPWINDOW_MAPNAME_BEGIN + j,
			MU_TO_WINDOW(MAPNAME_POSX, _AspectWidth),
			MU_TO_WINDOW(liney, _AspectHeight),
			1.0f, ETA_CENTER, ETA_CENTER, 0.0f,
			textColor, backColor,
			data.MainMapName);

		mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("%d"), RequiredLevel);

		if(PlayerLevel >= RequiredLevel)
		{
			FONT_NORMAL->DrawStringC2D(TEXTINDEX::MAPWINDOW_LEVEL_BEGIN + j,
				MU_TO_WINDOW(LEVEL_POSX, _AspectWidth),
				MU_TO_WINDOW(liney, _AspectHeight),
				1.0f, ETA_CENTER, ETA_CENTER, 0.0f,
				textColor, backColor,
				textBuffer);
		}
		else
		{
			FONT_NORMAL->DrawStringC2D(TEXTINDEX::MAPWINDOW_LEVEL_BEGIN + j,
				MU_TO_WINDOW(LEVEL_POSX, _AspectWidth),
				MU_TO_WINDOW(liney, _AspectHeight),
				1.0f, ETA_CENTER, ETA_CENTER, 0.0f,
				MU::MakeARGB(255, 51, 26, 255), backColor,
				textBuffer);
		}

		mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("%lld"), data.MoveCost);

		if (CurrentGold >= data.MoveCost)
		{
			FONT_NORMAL->DrawStringC2D(TEXTINDEX::MAPWINDOW_COST_BEGIN + j,
				MU_TO_WINDOW(COST_POSX, _AspectWidth),
				MU_TO_WINDOW(liney, _AspectHeight),
				1.0f, ETA_CENTER, ETA_CENTER, 0.0f,
				textColor, backColor,
				textBuffer);
		}
		else
		{
			FONT_NORMAL->DrawStringC2D(TEXTINDEX::MAPWINDOW_COST_BEGIN + j,
				MU_TO_WINDOW(COST_POSX, _AspectWidth),
				MU_TO_WINDOW(liney, _AspectHeight),
				1.0f, ETA_CENTER, ETA_CENTER, 0.0f,
				MU::MakeARGB(255, 51, 26, 255), backColor,
				textBuffer);
		}
	}
}

void UIMoveWindow::PostDraw()
{
	_CloseButton->PostDraw();
}

UIBase *UIMoveWindow::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _ScrollBar->GetFocus(mouseX, mouseY);
		if (object != nullptr)
		{
			return object;
		}

		object = _CloseButton->GetFocus(mouseX, mouseY);
		if (object != nullptr)
		{
			return object;
		}

		mu_uint32 maxRender = _BeginIndex + MOVEWINDOW_MAXLINES;
		if (maxRender > (mu_uint32)g_MoveManager.MoveData.size())
		{
			maxRender = (mu_uint32)g_MoveManager.MoveData.size();
		}

		for (mu_uint32 j = 0, n = _BeginIndex; n < maxRender; ++j, ++n)
		{
			if (g_MoveManager.MoveData[n].CanMove == false)
			{
				continue;
			}

			object = _SelectButton[j]->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}
		}

		return focus;
	}

	return nullptr;
}

mu_uint32 UIMoveWindow::OnPreWindowMessage(const SDL_Event *event, mu_size *result)
{
	UIBase *focusObject = _SceneManager->GetFocused();
	if (focusObject == this ||
		(focusObject != nullptr &&
			focusObject->_Parent == this))
	{
		mu_int32 mouseY = MU_INPUT->GetMouseY();

		if (mouseY >= _ScrollBar->_NativeY &&
			mouseY <= _ScrollBar->_NativeY + _ScrollBar->_Height)
		{
			switch (event->type)
			{
			case SDL_MOUSEWHEEL:
				{
					if (_ScrollBar->GetState() != UIScrollBar::UI_DISABLED)
					{
						mu_float mouseWheel = event->wheel.y;
						mu_float diffValue = mouseWheel * _ScrollBar->_DiffValue * -0.03f;
						_ScrollBar->ConfigureScrollInfo(_ScrollBar->_MinValue, _ScrollBar->_MaxValue, _ScrollBar->_CurrentValue + diffValue);
						this->OnScrolling();
					}
				}
				return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
			}
		}
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

void UIMoveWindow::OnScrolling()
{
	_BeginIndex = (mu_int32)_ScrollBar->GetCurrentValue();
}