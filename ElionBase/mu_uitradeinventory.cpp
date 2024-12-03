#include "stdafx.h"
#include "mu_uitradeinventory.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define BACKTEXTURE_WIDTH 225.0f
#define BACKTEXTURE_HEIGHT 512.0f

#define BACK_POSX TRADE_POSX
#define BACK_POSY TRADE_POSY
#define BACK_WIDTH TRADE_WIDTH
#define BACK_HEIGHT TRADE_HEIGHT
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND EVector2(BACK_WIDTH / BACKTEXTURE_WIDTH, BACK_HEIGHT / BACKTEXTURE_HEIGHT)

#define TOP_WIDTH TRADE_WIDTH
#define TOP_HEIGHT 64.0f
#define TOP_POSX TRADE_POSX
#define TOP_POSY TRADE_POSY

#define BOTTOM_WIDTH TRADE_WIDTH
#define BOTTOM_HEIGHT 45.0f
#define BOTTOM_POSX TRADE_POSX
#define BOTTOM_POSY (TRADE_POSY + TRADE_HEIGHT - BOTTOM_HEIGHT)

#define LEFT_WIDTH 21.0f
#define LEFT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define LEFT_POSX TRADE_POSX
#define LEFT_POSY (TOP_POSY + TOP_HEIGHT)

#define RIGHT_WIDTH 21.0f
#define RIGHT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define RIGHT_POSX (TRADE_POSX + TRADE_WIDTH - RIGHT_WIDTH)
#define RIGHT_POSY (TOP_POSY + TOP_HEIGHT)

#define MIDDLELINE_WIDTH 188.0f
#define MIDDLELINE_HEIGHT 21.0f
#define MIDDLELINE_POSX (TRADE_POSX + 1.0f)
#define MIDDLELINE_POSY (TRADE_POSY + 220.0f)

#define XBUTTON_WIDTH 16.0f
#define XBUTTON_HEIGHT 16.0f
#define XBUTTON_POSX (TOP_POSX + TOP_WIDTH - 7.0f - XBUTTON_WIDTH)
#define XBUTTON_POSY (TOP_POSY + 7.0f)

UITradeInventory::UITradeInventory(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(TRADE_POSX, TRADE_POSY);
	UIBase::SetDimensions(TRADE_WIDTH, TRADE_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_HIDDEN);
	SetEvent(GFX_EVENT::GFX_MOUSE_EVENT | GFX_EVENT::GFX_MOUSE_DBLCLICK_AS_CLICK);

	// Initialize
	{
		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_BACK], SCREEN_WIDTH, SCREEN_HEIGHT, BACK_POSX, BACK_POSY, BACK_WIDTH, BACK_HEIGHT, BACK_UVBEGIN, BACK_UVEND);
		_TopFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_TOP], SCREEN_WIDTH, SCREEN_HEIGHT, TOP_POSX, TOP_POSY, TOP_WIDTH, TOP_HEIGHT);
		_TopFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BottomFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_BOTTOM], SCREEN_WIDTH, SCREEN_HEIGHT, BOTTOM_POSX, BOTTOM_POSY, BOTTOM_WIDTH, BOTTOM_HEIGHT);
		_BottomFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_LeftFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, LEFT_POSX, LEFT_POSY, LEFT_WIDTH, LEFT_HEIGHT);
		_LeftFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_RightFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, RIGHT_POSX, RIGHT_POSY, RIGHT_WIDTH, RIGHT_HEIGHT);
		_RightFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MiddleFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MYQUEST_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, MIDDLELINE_POSX, MIDDLELINE_POSY, MIDDLELINE_WIDTH, MIDDLELINE_HEIGHT);
		_MiddleFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_MyInventoryControl = new UIInventoryControl(manager, this, TEXTINDEX::BEGIN_MYTRADE_ITEMNUMBER, SCREEN_WIDTH, SCREEN_HEIGHT, TRADE_POSX + 16.0f, TRADE_POSY + 274.0f, COLUMN_TRADE_INVENTORY, ROW_TRADE_INVENTORY, INVENTORY_TYPE::IT_TRADE);
		_YourInventoryControl = new UIInventoryControl(manager, this, TEXTINDEX::BEGIN_YOURTRADE_ITEMNUMBER, SCREEN_WIDTH, SCREEN_HEIGHT, TRADE_POSX + 16.0f, TRADE_POSY + 68.0f, COLUMN_TRADE_INVENTORY, ROW_TRADE_INVENTORY, INVENTORY_TYPE::IT_YOURTRADE);
		_YourInventoryControl->Lock();

		_MyNameFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TRADE_ACCOUNT_TITLE], SCREEN_WIDTH, SCREEN_HEIGHT, TRADE_POSX + 11.0f, TRADE_POSY + 243.0f, 171.0f, 26.0f);
		_MyNameFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_YourNameFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TRADE_ACCOUNT_TITLE], SCREEN_WIDTH, SCREEN_HEIGHT, TRADE_POSX + 11.0f, TRADE_POSY + 37.0f, 171.0f, 26.0f);
		_YourNameFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MyMoneyIcon = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::ICON_MONEY1], SCREEN_WIDTH, SCREEN_HEIGHT, TRADE_POSX + 11.0f, TRADE_POSY + 356.0f, 170.0f, 26.0f);
		_MyMoneyIcon->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_YourMoneyIcon = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::ICON_MONEY1], SCREEN_WIDTH, SCREEN_HEIGHT, TRADE_POSX + 11.0f, TRADE_POSY + 150.0f, 170.0f, 26.0f);
		_YourMoneyIcon->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_YourGuildIcon = new UISprite(manager, this, &TEXTURES::Data[BITMAP_GUILD2_BEGIN], SCREEN_WIDTH, SCREEN_HEIGHT, TRADE_POSX + 18.0f, TRADE_POSY + 42.0f, 16.0f, 16.0f);
		_YourGuildIcon->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_YourGuildIcon->SetSampler(ESampler::eUcVcWcMinNMagNMipN);

		_XButton = new UIButton(GAMEUI::MESSAGE_MYINVENTORY_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, XBUTTON_POSX, XBUTTON_POSY, XBUTTON_WIDTH, XBUTTON_HEIGHT);

		_CancelButton = new UIButton(GAMEUI::MESSAGE_MYINVENTORY_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, TRADE_POSX + 13.0f, TRADE_POSY + 390.0f, 36.0f, 29.0f);
		_CancelButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CancelButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_CancelButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetTooltip(FONT_NORMAL, GlobalText[1002], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_MoneyButton = new UIButton(GAMEUI::MESSAGE_TRADEWINDOW_MONEY_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, TRADE_POSX + 104.0f, TRADE_POSY + 390.0f, 36.0f, 29.0f);
		_MoneyButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MoneyButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_DEPOSIT_MONEY], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_MoneyButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_DEPOSIT_MONEY], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_MoneyButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_DEPOSIT_MONEY], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_MoneyButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_DEPOSIT_MONEY], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_MoneyButton->SetTooltip(FONT_NORMAL, GlobalText[227], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_OkButton = new UIToggleButton(GAMEUI::MESSAGE_TRADEWINDOW_OK_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, TRADE_POSX + 144.0f, TRADE_POSY + 390.0f, 36.0f, 29.0f);
		_OkButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_OkButton->SetStateData(UIToggleButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::TRADE_BTN_ACCEPT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 0, 0, 255)));
		_OkButton->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::TRADE_BTN_ACCEPT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::TRADE_BTN_ACCEPT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::TRADE_BTN_ACCEPT], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::TRADE_BTN_ACCEPT], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetTooltip(FONT_NORMAL, GlobalText[228], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_YourOkButton = new UIToggleButton(GAMEUI::MESSAGE_TRADEWINDOW_OK_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, TRADE_POSX + 146.0f, TRADE_POSY + 186.0f, 36.0f, 29.0f);
		_YourOkButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_YourOkButton->SetStateData(UIToggleButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::TRADE_BTN_ACCEPT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_YourOkButton->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::TRADE_BTN_ACCEPT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_YourOkButton->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::TRADE_BTN_ACCEPT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_YourOkButton->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::TRADE_BTN_ACCEPT], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_YourOkButton->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::TRADE_BTN_ACCEPT], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_YourOkButton->SetTooltip(FONT_NORMAL, GlobalText[228], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		for (mu_int32 y = 0; y < ROW_TRADE_INVENTORY; ++y)
		{
			for (mu_int32 x = 0; x < COLUMN_TRADE_INVENTORY; ++x)
			{
				mu_float fX = _YourInventoryControl->_ViewX + x * INVENTORY_SQUARE_WIDTH;
				mu_float fY = _YourInventoryControl->_ViewY + y * INVENTORY_SQUARE_HEIGHT;
				mu_uint32 warningSlot = y * COLUMN_TRADE_INVENTORY + x;

				_WarningArrow[warningSlot] = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::CURSOR_SITDOWN],
					SCREEN_WIDTH, SCREEN_HEIGHT,
					fX, fY + 5.0f,
					24.0f, 24.0f,
					EVector2(0.0f, 0.4f), EVector2(1.0f, 1.0f),
					MU::MakeARGB(0, 255, 255, 255));
				_WarningArrow[warningSlot]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			}
		}
	
		this->InitYourInventoryBackup();

		ResetInfo();
	}
}

UITradeInventory::~UITradeInventory()
{

}

void UITradeInventory::InitYourInventoryBackup()
{
	for (mu_int32 n = 0; n < MAX_TRADE_INVENTORY; ++n)
	{
		_YourInventoryBackup[n].Type = -1;
	}
}

void UITradeInventory::BackupYourInventoryItem(ITEM *item)
{
	if ((item->Type >= ITEM_HELPER && item->Type <= ITEM_HELPER + 4)
		|| (item->Type == ITEM_POTION + 13 || item->Type == ITEM_POTION + 14 || item->Type == ITEM_POTION + 16)
		|| (item->Type >= ITEM_POTION + 31)
		|| (COMGEM::isCompiledGem(item))
		|| (item->Type >= ITEM_WING && item->Type <= ITEM_WING + 6)
		|| (item->Type >= ITEM_HELPER + 30)	
		|| (item->Type >= ITEM_WING + 36 && item->Type <= ITEM_WING + 43)
		|| (item->Type == ITEM_WING + 15)
		|| (item->Level > 4 && item->Type < ITEM_WING)
		|| item->ExcOption > 0)
	{
		mu_int32 nCompareValue;
		mu_boolean bSameItem = false;

		for (mu_int32 i = 0; i < MAX_TRADE_INVENTORY; ++i)
		{
			if (_YourInventoryBackup[i].Type == -1)
				continue;

			nCompareValue = CompareItem(&_YourInventoryBackup[i], item);
			if (0 == nCompareValue)
			{
				bSameItem = true;
				break;
			}
			else if (-1 == nCompareValue)
			{
				bSameItem = true;
				_YourInventoryBackup[i] = *item;
				break;
			}
			else if (2 != nCompareValue)
			{
				bSameItem = true;
			}
		}

		if (!bSameItem)
		{
			for (mu_int32 i = 0; i < MAX_TRADE_INVENTORY; ++i)
			{
				if (_YourInventoryBackup[i].Type == -1)
				{
					_YourInventoryBackup[i] = *item;
					break;
				}
			}
		}
	}
}

void UITradeInventory::BackupYourInventory()
{
	InitYourInventoryBackup();

	auto &itemsmap = _YourInventoryControl->GetItems();

	for (auto it = itemsmap.begin(); it != itemsmap.end(); ++it)
	{
		BackupYourInventoryItem(it->second);
	}
}

void UITradeInventory::ResetInfo()
{
	_MyLevel = 0;
	_MyName.clear();
	_MyTradeGold = 0;

	_YourLevel = 0;
	_YourName.clear();
	_YourTradeGold = 0;
	_YourGuildIndex = -1;
	_YourGuildMarkIndex = -1;

	_OkButton->ChangeState(UIToggleButton::UI_NORMAL);
	_YourOkButton->ChangeState(UIToggleButton::UI_NORMAL);

	_TradeWaitTime = 0;
	_AlertTrade = false;
}

void UITradeInventory::Update(mu_uint64 CurrentTime)
{
	if (_TradeWaitTime != 0 &&
		_TradeWaitTime <= g_CurrentTime)
	{
		_TradeWaitTime = 0;
		_OkButton->ChangeState(UIToggleButton::UI_NORMAL);
	}
}

void UITradeInventory::Draw()
{
	DrawFrames();
	DrawDetails();
	DrawButtons();
	_MyInventoryControl->Draw();
	_YourInventoryControl->Draw();
}

void UITradeInventory::Draw3D()
{
	_MyInventoryControl->Draw3D();
	_YourInventoryControl->Draw3D();
}

void UITradeInventory::PostDraw()
{
	PostDrawButtons();

	mu_float sinTime = EMath::Sin(FWorldTime * 0.015f);
	auto &itemsmap = _YourInventoryControl->GetItems();

	for (auto it = itemsmap.begin(); it != itemsmap.end(); ++it)
	{
		ITEM *item = it->second;

		if (item->byColorState == ITEM_COLOR_TRADE_WARNING)
		{
			mu_float fX = _YourInventoryControl->_ViewX +
				item->x * INVENTORY_SQUARE_WIDTH;
			mu_float fY = _YourInventoryControl->_ViewY +
				item->y * INVENTORY_SQUARE_HEIGHT + sinTime;

			UISprite *WarningArrow = _WarningArrow[item->y * COLUMN_TRADE_INVENTORY + item->x];

			WarningArrow->SetPosition(fX, fY + 5.0f);
			WarningArrow->Draw();
		}
	}

	mu_int32 textCode = 0;
	for (auto it = itemsmap.begin(); it != itemsmap.end(); ++it)
	{
		ITEM *item = it->second;

		if (item->byColorState == ITEM_COLOR_TRADE_WARNING)
		{
			ITEM_ATTRIBUTE *itemAttr = &ItemAttribute[item->Type];
			mu_float fX = _YourInventoryControl->_ViewX +
				item->x * INVENTORY_SQUARE_WIDTH +
				(mu_float)(itemAttr->Width * INVENTORY_SQUARE_WIDTH) * 0.5f;
			mu_float fY = _YourInventoryControl->_ViewY +
				item->y * INVENTORY_SQUARE_HEIGHT + sinTime;

			FONT_BOLD->DrawStringC2D(TEXTINDEX::TRADE_WARNING_BEGIN + textCode++,
				MU_TO_WINDOW(fX, _AspectWidth),
				MU_TO_WINDOW(fY, _AspectHeight),
				1.0f, ETA_CENTER, ETA_CENTER, 0.0f,
				MU::MakeARGB(255, 255, 255, 255),
				MU::MakeARGB(210, 0, 0, 255),
				GlobalText[370]);
		}
	}

	_MyInventoryControl->PostDraw();
	_YourInventoryControl->PostDraw();
}

UIBase *UITradeInventory::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _MyInventoryControl->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _YourInventoryControl->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _XButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _CancelButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _MoneyButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _OkButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return focus;
}

void UITradeInventory::DrawFrames()
{
	_BackFrame->Draw();
	_TopFrame->Draw();
	_BottomFrame->Draw();
	_LeftFrame->Draw();
	_RightFrame->Draw();
	_MiddleFrame->Draw();

	_MyNameFrame->Draw();
	_YourNameFrame->Draw();
	_MyMoneyIcon->Draw();
	_YourMoneyIcon->Draw();
}

void UITradeInventory::DrawDetails()
{
	mu_text szTemp[256];

	FONT_BOLD->DrawStringC2D(TEXTINDEX::TRADE_HEADER,
		_NativeX + MU_TO_WINDOW(STORAGE_WIDTH * 0.5f, _AspectWidth),
		_NativeY + MU_TO_WINDOW(15.0f, _AspectHeight),
		1.0f, ETA_CENTER, ETA_CENTER, 0.0f,
		MU::MakeARGB(216, 216, 216, 255),
		MU::MakeARGB(0, 0, 0, 0),
		GlobalText[226]);

	MUTextSimpleRender *font = FONT_BOLD;
	mu_uint32 backColor = MU::MakeARGB(0, 0, 0, 0);
	mu_uint32 textColor = MU::MakeARGB(216, 216, 216, 255);
	textColor = MU::MakeARGB(216, 216, 216, 255);

	if (_YourGuildMarkIndex >= 0)
	{
		MARK_t &mark = GuildMark[_YourGuildMarkIndex];
		_YourGuildIcon->Draw(); 
		FONT_BIG->DrawStringC2D(TEXTINDEX::TRADE_YOURGUILD,
			MU_TO_WINDOW(_YourNameFrame->_ViewX + 24.0f, _AspectWidth),
			MU_TO_WINDOW(_YourNameFrame->_ViewY + _YourNameFrame->_ViewHeight * 0.5f, _AspectHeight),
			0.6f, ETA_LEFT, ETA_TOP, 0.0f,
			MU::MakeARGB(216, 216, 216, 255),
			MU::MakeARGB(0, 0, 0, 0),
			mark.GuildName.c_str());
	}

	font = FONT_BIG;

	textColor = MU::MakeARGB(210, 230, 255, 255);
	if (_YourGuildMarkIndex >= 0)
	{
		font->DrawStringC2D(TEXTINDEX::TRADE_YOURNAME,
			MU_TO_WINDOW(_YourNameFrame->_ViewX + 24.0f, _AspectWidth),
			MU_TO_WINDOW(_YourNameFrame->_ViewY + _YourNameFrame->_ViewHeight * 0.5f, _AspectHeight),
			0.6f, ETA_LEFT, ETA_BOTTOM, 0.0f,
			textColor, backColor, _YourName.c_str());
	}
	else
	{
		font->DrawStringC2D(TEXTINDEX::TRADE_YOURNAME,
			MU_TO_WINDOW(_YourNameFrame->_ViewX + 5.0f, _AspectWidth),
			MU_TO_WINDOW(_YourNameFrame->_ViewY + _YourNameFrame->_ViewHeight * 0.5f, _AspectHeight),
			1.0f, ETA_LEFT, ETA_CENTER, 0.0f,
			textColor, backColor, _YourName.c_str());
	}

	font = FONT_BOLD;

	mu_int32 nLevel;
	ConvertYourLevel(nLevel, textColor);

	if (nLevel == 400)
	{
		mu_sprintf_s(szTemp, mu_countof(szTemp), _T("Lv. %d"), nLevel);
	}
	else
	{
		mu_text szTemp2[256];
		mu_sprintf_s(szTemp2, mu_countof(szTemp2), GlobalText[369], nLevel);
		mu_sprintf_s(szTemp, mu_countof(szTemp), _T("Lv. ") SPF_STRING _T(""), szTemp2);
	}

	font->DrawStringC2D(TEXTINDEX::TRADE_LEVEL,
		MU_TO_WINDOW(_YourNameFrame->_ViewX + _YourNameFrame->_ViewWidth - 10.0f, _AspectWidth),
		MU_TO_WINDOW(_YourNameFrame->_ViewY + _YourNameFrame->_ViewHeight * 0.5f, _AspectHeight),
		1.0f, ETA_RIGHT, ETA_CENTER, 0.0f,
		textColor, backColor, szTemp);

	::ConvertGold(_YourTradeGold, szTemp, mu_countof(szTemp));
	textColor = ::getGoldColor(_YourTradeGold);
	FONT_BOLD->DrawStringC2D(TEXTINDEX::TRADE_YOURGOLD,
		_YourMoneyIcon->_NativeX + _YourMoneyIcon->_Width - MU_TO_WINDOW(10.0f, _AspectWidth),
		_YourMoneyIcon->_NativeY + _YourMoneyIcon->_Height * 0.5f - MU_TO_WINDOW(1.0f, _AspectHeight),
		1.0f, ETA_RIGHT, ETA_CENTER, 0.0f,
		textColor, backColor, szTemp);

	::ConvertGold(_MyTradeGold, szTemp, mu_countof(szTemp));
	textColor = ::getGoldColor(_MyTradeGold);
	FONT_BOLD->DrawStringC2D(TEXTINDEX::TRADE_MYGOLD,
		_MyMoneyIcon->_NativeX + _MyMoneyIcon->_Width - MU_TO_WINDOW(10.0f, _AspectWidth),
		_MyMoneyIcon->_NativeY + _MyMoneyIcon->_Height * 0.5f - MU_TO_WINDOW(1.0f, _AspectHeight),
		1.0f, ETA_RIGHT, ETA_CENTER, 0.0f,
		textColor, backColor, szTemp);

	textColor = MU::MakeARGB(210, 230, 255, 255);
	FONT_BIG->DrawStringC2D(TEXTINDEX::TRADE_MYNAME,
		MU_TO_WINDOW(_MyNameFrame->_ViewX + 5.0f, _AspectWidth),
		MU_TO_WINDOW(_MyNameFrame->_ViewY + _YourNameFrame->_ViewHeight * 0.5f, _AspectHeight),
		1.0f, ETA_LEFT, ETA_CENTER, 0.0f,
		textColor, backColor, _MyName.c_str());

	mu_int32 nAlpha = mu_int32(EMath::Min((mu_int64)255, (mu_int64)(EMath::Sin((mu_float)(FWorldTime / 200.0f)) * 200.0f) + 275));
	textColor = MU::MakeARGB(210, 0, 0, nAlpha);
	font->DrawStringC2D(TEXTINDEX::BEGIN_TRADE_EXTRA + 0,
		MU_TO_WINDOW(_ViewX + 20.0f, _AspectWidth),
		MU_TO_WINDOW(_ViewY + 185.0f, _AspectHeight),
		1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f,
		textColor, backColor, GlobalText[370]);
	textColor = MU::MakeARGB(255, 220, 150, 255);
	font->DrawStringC2D(TEXTINDEX::BEGIN_TRADE_EXTRA + 1,
		MU_TO_WINDOW(_ViewX + 50.0f, _AspectWidth),
		MU_TO_WINDOW(_ViewY + 185.0f, _AspectHeight),
		1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f,
		textColor, backColor, GlobalText[365]);
	font->DrawStringC2D(TEXTINDEX::BEGIN_TRADE_EXTRA + 2,
		MU_TO_WINDOW(_ViewX + 20.0f, _AspectWidth),
		MU_TO_WINDOW(_ViewY + 200.0f, _AspectHeight),
		1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f,
		textColor, backColor, GlobalText[366]);
	font->DrawStringC2D(TEXTINDEX::BEGIN_TRADE_EXTRA + 3,
		MU_TO_WINDOW(_ViewX + 20.0f, _AspectWidth),
		MU_TO_WINDOW(_ViewY + 215.0f, _AspectHeight),
		1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f,
		textColor, backColor, GlobalText[367]);
}

void UITradeInventory::DrawButtons()
{
	_CancelButton->Draw();
	_MoneyButton->Draw();
	_OkButton->Draw();
	_YourOkButton->Draw();
}

void UITradeInventory::PostDrawButtons()
{
	_CancelButton->PostDraw();
	_MoneyButton->PostDraw();
	_OkButton->PostDraw();
	_YourOkButton->PostDraw();
}

void UITradeInventory::ConvertYourLevel(mu_int32 &rLevel, mu_uint32 &rColor)
{
	if (_YourLevel >= 400)
	{
		rLevel = 400;
		rColor = MU::MakeARGB(153, 153, 255, 255);
	}
	else if (_YourLevel >= 300)
	{
		rLevel = 300;
		rColor = MU::MakeARGB(255, 153, 255, 255);
	}
	else if (_YourLevel >= 200)
	{
		rLevel = 200;
		rColor = MU::MakeARGB(255, 230, 210, 255);
	}
	else if (_YourLevel >= 100)
	{
		rLevel = 100;
		rColor = MU::MakeARGB(24, 201, 0, 255);
	}
	else if (_YourLevel >= 50)
	{
		rLevel = 50;
		rColor = MU::MakeARGB(0, 150, 255, 255);
	}
	else
	{
		rLevel = 10;
		rColor = MU::MakeARGB(0, 0, 255, 255);
	}
}

void UITradeInventory::ChangeYourOkButton(mu_boolean isOk, mu_boolean waitTime)
{
	if (isOk)
	{
		_YourOkButton->ChangeState(UIToggleButton::UI_PRESSED);
	}
	else
	{
		_YourOkButton->ChangeState(UIToggleButton::UI_NORMAL);
	}

	if (waitTime == true)
	{
		SetWaitTime(2500);
	}
}

void UITradeInventory::SetWaitTime(mu_uint64 waitTime)
{
	_TradeWaitTime = g_CurrentTime + waitTime;
	_OkButton->ChangeState(UIToggleButton::UI_DISABLED);
}

void UITradeInventory::SetTradeMoney(mu_int64 MyMoney, mu_int64 YourMoney, mu_boolean setWaitTime)
{
	_MyTradeGold = MyMoney;
	_YourTradeGold = YourMoney;

	if (setWaitTime == true)
	{
		SetWaitTime(2500);
	}
}

void UITradeInventory::AlertYourTradeInventory()
{
	mu_int32 nCount = 0;
	mu_int32 nCompareItemType[10];

	_AlertTrade = false;

	auto &itemsmap = _YourInventoryControl->GetItems();

	ITEM* pYourItemObj;
	mu_int32 nCompareValue;

	for(auto it = itemsmap.begin(); it != itemsmap.end(); ++it)
	{
		pYourItemObj = it->second;

		for (mu_int32 j = 0; j < MAX_TRADE_INVENTORY; ++j)
		{
			if (_YourInventoryBackup[j].Type == pYourItemObj->Type)
			{
				nCompareValue = CompareItem(&_YourInventoryBackup[j], pYourItemObj);
				if (1 == nCompareValue)
				{
					_AlertTrade = true;
					pYourItemObj->byColorState = ITEM_COLOR_TRADE_WARNING;
				}
				else
				{
					if (0 == nCompareValue)
						nCompareItemType[nCount++] = _YourInventoryBackup[j].Type;

					pYourItemObj->byColorState = ITEM_COLOR_NORMAL;
					break;
				}
			}
		}
	}

	if (nCount > 0)
	{
		_AlertTrade = false;

		for (mu_int32 i = 0; i < nCount; ++i)
		{
			for (auto it = itemsmap.begin(); it != itemsmap.end(); ++it)
			{
				pYourItemObj = it->second;
				if (nCompareItemType[i] == pYourItemObj->Type)
					pYourItemObj->byColorState = ITEM_COLOR_NORMAL;
			}
		}
	}
}