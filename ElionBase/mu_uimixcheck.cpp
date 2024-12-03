#include "stdafx.h"
#include "mu_uimixcheck.h"
#include "mu_uimixcheck_defines.h"

UIMixCheckBox::UIMixCheckBox(UISceneManager *manager) : UIBase()
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
		
		_AcceptButton = new UIButton(MESSAGEBOX::MESSAGE_MIXCHECK_ACCEPTBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ACCEPT_POSX, ACCEPT_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_AcceptButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AcceptButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_AcceptButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_AcceptButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_AcceptButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));

		_CancelButton = new UIButton(MESSAGEBOX::MESSAGE_MIXCHECK_CANCELBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CANCEL_POSX, CANCEL_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_CancelButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CancelButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_CancelButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
	}
}

UIMixCheckBox::~UIMixCheckBox()
{
}

void UIMixCheckBox::Update(mu_uint64 CurrentTime)
{

}

void UIMixCheckBox::PostDraw()
{
	DrawFrame();
	DrawText();
	DrawButton();
}

void UIMixCheckBox::DrawFrame()
{
	TMSGBOX_RENDERBACK;
	TMSGBOX_RENDERFRONT;
}

void UIMixCheckBox::DrawText()
{
	mu_text strText[256];
	if (g_MixManager.GetCurRecipe()->MixName[1] == 0)
	{
		mu_sprintf_s(strText, mu_countof(strText), _T("") SPF_STRING _T(""), GlobalText[g_MixManager.GetCurRecipe()->MixName[0]]);
	}
	else if (g_MixManager.GetCurRecipe()->MixName[2] == 0)
	{
		mu_sprintf_s(strText, mu_countof(strText), _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[g_MixManager.GetCurRecipe()->MixName[0]],
			GlobalText[g_MixManager.GetCurRecipe()->MixName[1]]);
	}
	else
	{
		mu_sprintf_s(strText, mu_countof(strText), _T("") SPF_STRING _T(" ") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[g_MixManager.GetCurRecipe()->MixName[0]],
			GlobalText[g_MixManager.GetCurRecipe()->MixName[1]],
			GlobalText[g_MixManager.GetCurRecipe()->MixName[2]]);
	}

	mu_float messageX = MU_TO_WINDOW(_ViewX + (WINBOX_WIDTH * 0.5f) - TPADDING_MSGBOX_GETLEFT(SCREEN_WIDTH), _AspectWidth);
	mu_float messageY = _Int_MsgBox_MiddleLeft->_NativeY - MU_TO_WINDOW(TTEXT_MSGBOX_MOVEUP(SCREEN_HEIGHT), _AspectHeight);

	MUText *buffer = nullptr;
	FONT_BOLD->DrawStringC2D(TEXTINDEX::MIXCHECK_BEGIN + 0,
		messageX,
		messageY,
		1.0f,
		ETA_CENTER,
		ETA_BOTTOM,
		5.0f,
		MU::MakeARGB(255, 255, 0, 255),
		MU::MakeARGB(0, 0, 0, 0),
		strText, 0.0f, 0.0f, &buffer);

	if (buffer != nullptr)
	{
		messageY += buffer->GetSize().y;
	}

	messageY += FONT_NORMAL->GetFont()->GetLineHeight();

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::MIXCHECK_BEGIN + 1,
		messageX,
		messageY,
		1.0f,
		ETA_CENTER,
		ETA_BOTTOM,
		5.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(0, 0, 0, 0),
		GlobalText[539]);
}

void UIMixCheckBox::DrawButton()
{
	_AcceptButton->Draw();
	_CancelButton->Draw();
	_AcceptButton->PostDraw();
	_CancelButton->PostDraw();
}

mu_uint32 UIMixCheckBox::OnPreKeyPressed(const SDL_Keysym &key)
{
	switch (key.scancode)
	{
	case SDL_SCANCODE_RETURN:
	case SDL_SCANCODE_KP_ENTER:
		{
			_SceneManager->OnGFxMessagePush(_AcceptButton->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
		}
		return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

UIBase *UIMixCheckBox::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _AcceptButton->GetFocus(mouseX, mouseY);

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