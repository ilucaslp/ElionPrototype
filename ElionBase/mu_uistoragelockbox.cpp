#include "stdafx.h"
#include "mu_uistoragelockbox.h"

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 960.0f

#define BOX_WIDTH 352.0f
#define BOX_HEIGHT 113.0f
#define BOX_POSX (SCREEN_WIDTH * 0.5f - BOX_WIDTH * 0.5f)
#define BOX_POSY (SCREEN_HEIGHT * 0.5f - BOX_HEIGHT * 0.5f)

#define BUTTON_WIDTH 54.0f
#define BUTTON_HEIGHT 30.0f

#define ACCEPT_POSX (BOX_POSX + 98.0f)
#define ACCEPT_POSY (BOX_POSY + 72.0f)
#define CANCEL_POSX (BOX_POSX + BOX_WIDTH - 98.0f - BUTTON_WIDTH)
#define CANCEL_POSY (BOX_POSY + 72.0f)

#define PASSWORDINPUT_WIDTH 300.0f
#define PASSWORDINPUT_HEIGHT 12.0f

UIStorageLockBox::UIStorageLockBox(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(BOX_POSX, BOX_POSY);
	UIBase::SetDimensions(BOX_WIDTH, BOX_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_NO_FOCUS | GFX_FLAG::GFX_HIDDEN);

	// Initialize
	{
		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_WINDOW], SCREEN_WIDTH, SCREEN_HEIGHT, BOX_POSX, BOX_POSY, BOX_WIDTH, BOX_HEIGHT);

		_InputBack = new UISpriteColor(manager, this, MU::MakeARGB(0, 0, 0, 240), SCREEN_WIDTH, SCREEN_HEIGHT, BOX_POSX + (BOX_WIDTH - (PASSWORDINPUT_WIDTH + 5.0f)) * 0.5f, BOX_POSY + MSGBOX_TOP_HEIGHT - 16.0f - 5.0f + 1.0f, PASSWORDINPUT_WIDTH + 10.0f, PASSWORDINPUT_HEIGHT + 10.0f);

		_PasswordInput = new UIEditBox(MESSAGEBOX::MESSAGE_STORAGELOCK_PASSWORDINPUT, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, BOX_POSX + (BOX_WIDTH - PASSWORDINPUT_WIDTH) * 0.5f, BOX_POSY + MSGBOX_TOP_HEIGHT - 16.0f, PASSWORDINPUT_WIDTH, PASSWORDINPUT_HEIGHT);
		_PasswordInput->SetLimitText(STORAGE_MAXIMUM);
		_PasswordInput->SetTextColor(255, 230, 210, 255);
		_PasswordInput->SetBackColor(0, 0, 0, 0);
		_PasswordInput->SetHighlightTextColor(0, 0, 0, 255);
		_PasswordInput->SetHighlightColor(255, 230, 210, 255);

		_AcceptButton = new UIButton(MESSAGEBOX::MESSAGE_STORAGELOCK_ACCEPTBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ACCEPT_POSX, ACCEPT_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_AcceptButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AcceptButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_AcceptButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_AcceptButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_AcceptButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));

		_CancelButton = new UIButton(MESSAGEBOX::MESSAGE_STORAGELOCK_CANCELBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CANCEL_POSX, CANCEL_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_CancelButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CancelButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_CancelButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
	}
}

UIStorageLockBox::~UIStorageLockBox()
{

}

void UIStorageLockBox::Update(mu_uint64 CurrentTime)
{
	_PasswordInput->Update(CurrentTime);
}

void UIStorageLockBox::PostDraw()
{
	DrawFrame();
	DrawText();
	DrawButton();
}

void UIStorageLockBox::DrawFrame()
{
	_BackFrame->Draw();
	_InputBack->Draw();
}

void UIStorageLockBox::DrawText()
{
	FONT_NORMAL->DrawStringC2D(TEXTINDEX::STORAGE_LOCKBOX,
		MU_TO_WINDOW(_ViewX + (BOX_WIDTH * 0.5f), _AspectWidth),
		MU_TO_WINDOW(_ViewY + MSGBOX_TEXT_TOP_BLANK + 15.0f, _AspectHeight),
		1.0f,
		ETA_CENTER,
		ETA_TOP,
		5.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(0, 0, 0, 0),
		_T("Type lock password"));

	_PasswordInput->Draw();
}

void UIStorageLockBox::DrawButton()
{
	_AcceptButton->Draw();
	_CancelButton->Draw();
	_AcceptButton->PostDraw();
	_CancelButton->PostDraw();
}

mu_uint32 UIStorageLockBox::OnPreKeyPressed(const SDL_Keysym &key)
{
	switch (key.scancode)
	{
	case SDL_SCANCODE_RETURN:
	case SDL_SCANCODE_KP_ENTER:
		{
			_SceneManager->OnGFxMessagePush(_AcceptButton->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
		}
		return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;

	case SDL_SCANCODE_ESCAPE:
		{
			_SceneManager->OnGFxMessagePush(_CancelButton->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
		}
		return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

UIBase *UIStorageLockBox::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _PasswordInput->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _AcceptButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _CancelButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return focus;
}