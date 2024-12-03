#include "stdafx.h"
#include "mu_uiserverfull.h"

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 960.0f
#define DISCONNECTEDBOX_WIDTH 352.0f
#define DISCONNECTEDBOX_HEIGHT 113.0f
#define DISCONNECTEDBOX_POSX (SCREEN_WIDTH * 0.5f - DISCONNECTEDBOX_WIDTH * 0.5f)
#define DISCONNECTEDBOX_POSY (SCREEN_HEIGHT * 0.5f - DISCONNECTEDBOX_HEIGHT * 0.5f)

UIServerFull::UIServerFull(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(DISCONNECTEDBOX_POSX, DISCONNECTEDBOX_POSY);
	UIBase::SetDimensions(DISCONNECTEDBOX_WIDTH, DISCONNECTEDBOX_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_NO_FOCUS | GFX_FLAG::GFX_HIDDEN);

	// Initialize
	{
		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_WINDOW], SCREEN_WIDTH, SCREEN_HEIGHT, DISCONNECTEDBOX_POSX, DISCONNECTEDBOX_POSY, DISCONNECTEDBOX_WIDTH, DISCONNECTEDBOX_HEIGHT);

		_OkButton = new UIButton(MESSAGEBOX::MESSAGE_SERVERISFULL_OKBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, _ViewX + 149.0f, _ViewY + 72.0f, MSGBOX_BTN_WIDTH, MSGBOX_BTN_HEIGHT);
		_OkButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_OkButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_OkButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
	}
}

UIServerFull::~UIServerFull()
{

}

void UIServerFull::Update(mu_uint64 CurrentTime)
{

}

void UIServerFull::PostDraw()
{
	DrawFrame();
	DrawText();
	DrawButton();
}

void UIServerFull::DrawFrame()
{
	_BackFrame->Draw();
}

void UIServerFull::DrawText()
{
	FONT_NORMAL->DrawStringC2D(TEXTINDEX::MESSAGEBOX_SERVERISFULL,
		MU_TO_WINDOW(_ViewX + (DISCONNECTEDBOX_WIDTH * 0.5f), _AspectWidth),
		MU_TO_WINDOW(_ViewY + MSGBOX_TEXT_TOP_BLANK, _AspectHeight),
		1.0f,
		ETA_CENTER,
		ETA_BOTTOM,
		5.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(0, 0, 0, 0),
		GlobalText[416]);
}

void UIServerFull::DrawButton()
{
	_OkButton->Draw();
	_OkButton->PostDraw();
}

mu_uint32 UIServerFull::OnPreKeyPressed(const SDL_Keysym &key)
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

UIBase *UIServerFull::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _OkButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return focus;
}