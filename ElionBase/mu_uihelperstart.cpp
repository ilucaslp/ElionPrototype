#include "stdafx.h"
#include "mu_uihelperstart.h"
#include "mu_uihelperstart_defines.h"

UIHelperStart::UIHelperStart(UISceneManager *manager) : UIBase()
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

		_OnlineButton = new UIButton(MESSAGEBOX::MESSAGE_HELPERSTART_ONLINE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ONLINE_POSX, ONLINE_POSY, GBUTTON_WIDTH, GBUTTON_HEIGHT);
		_OnlineButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_OnlineButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_OnlineButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_OnlineButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_OnlineButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_OnlineButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[15210], MU::MakeARGB(170, 170, 170, 255));
		_OnlineButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[15210], MU::MakeARGB(200, 200, 200, 255));
		_OnlineButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[15210], MU::MakeARGB(255, 255, 255, 255));
		_OnlineButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[15210], MU::MakeARGB(200, 200, 200, 255));

		_OfflineButton = new UIButton(MESSAGEBOX::MESSAGE_HELPERSTART_OFFLINE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, OFFLINE_POSX, OFFLINE_POSY, GBUTTON_WIDTH, GBUTTON_HEIGHT);
		_OfflineButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_OfflineButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_OfflineButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_OfflineButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_OfflineButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_OfflineButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[15211], MU::MakeARGB(170, 170, 170, 255));
		_OfflineButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[15211], MU::MakeARGB(200, 200, 200, 255));
		_OfflineButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[15211], MU::MakeARGB(255, 255, 255, 255));
		_OfflineButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[15211], MU::MakeARGB(200, 200, 200, 255));

		_CancelButton = new UIButton(MESSAGEBOX::MESSAGE_HELPERSTART_CANCEL_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CANCEL_POSX, CANCEL_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_CancelButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CancelButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_CancelButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
	}
}

UIHelperStart::~UIHelperStart()
{

}

void UIHelperStart::Update(mu_uint64 CurrentTime)
{

}

void UIHelperStart::PostDraw()
{
	DrawFrame();
	DrawText();
	DrawButton();
}

void UIHelperStart::DrawFrame()
{
	TMSGBOX_RENDERBACK;
	TMSGBOX_RENDERFRONT;
}

void UIHelperStart::DrawText()
{
}

void UIHelperStart::DrawButton()
{
	_OnlineButton->Draw();
	_OfflineButton->Draw();
	_CancelButton->Draw();
	_CancelButton->PostDraw();
}

mu_uint32 UIHelperStart::OnPreKeyPressed(const SDL_Keysym &key)
{
	switch (key.scancode)
	{
	case SDL_SCANCODE_ESCAPE:
		{
			_SceneManager->OnGFxMessagePush(_CancelButton->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
		}
		return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

UIBase *UIHelperStart::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _CancelButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _OnlineButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _OfflineButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return focus;
}