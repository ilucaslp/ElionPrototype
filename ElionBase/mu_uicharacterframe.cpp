#include "stdafx.h"
#include "mu_uicharacterframe.h"

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 960.0f
#define BLACK_HEIGHT 50.0f

#define BUTTON_WIDTH 54.0f
#define BUTTON_HEIGHT 30.0f

#define DECO_WIDTH 189.0f
#define DECO_HEIGHT 103.0f
#define DECO_POSX (SCREEN_WIDTH - DECO_WIDTH)
#define DECO_POSY (SCREEN_HEIGHT - BLACK_HEIGHT - DECO_HEIGHT - 1.0f)

#define CREATE_POSX (22.0f + (BUTTON_WIDTH + 1.0f) * 0.0f)
#define CREATE_POSY (SCREEN_HEIGHT - BUTTON_HEIGHT - BLACK_HEIGHT - 18.0f)

#define MENU_POSX (22.0f + (BUTTON_WIDTH + 1.0f) * 1.0f)
#define MENU_POSY (SCREEN_HEIGHT - BUTTON_HEIGHT - BLACK_HEIGHT - 18.0f)

#define CONNECT_POSX (SCREEN_WIDTH - BUTTON_WIDTH - 22.0f - (BUTTON_WIDTH + 1.0f))
#define CONNECT_POSY (SCREEN_HEIGHT - BUTTON_HEIGHT - BLACK_HEIGHT - 18.0f)

#define DELETE_POSX (SCREEN_WIDTH - BUTTON_WIDTH - 22.0f)
#define DELETE_POSY (SCREEN_HEIGHT - BUTTON_HEIGHT - BLACK_HEIGHT - 18.0f)

#define INFO_WIDTH (SCREEN_WIDTH - 266.0f)
#define INFO_HEIGHT 21.0f
#define INFO_POSX (22.0f + (BUTTON_WIDTH + 1.0f) * 2.0f)
#define INFO_POSY (SCREEN_HEIGHT - BUTTON_HEIGHT - BLACK_HEIGHT - 18.0f + 5.0f)

UICharacterFrame::UICharacterFrame(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(0.0f, 0.0f);
	UIBase::SetDimensions(0.0f, 0.0f);

	// Initialize
	{
		_InfoBack = new UISpriteColor(manager, this, MU::MakeARGB(0, 0, 0, 143), SCREEN_WIDTH, SCREEN_HEIGHT, INFO_POSX, INFO_POSY, INFO_WIDTH, INFO_HEIGHT);
		_InfoBack->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_DecoRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_DECORATION], SCREEN_WIDTH, SCREEN_HEIGHT, DECO_POSX, DECO_POSY, DECO_WIDTH, DECO_HEIGHT);
		_DecoRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_CreateButton = new UIButton(CHARACTERUI::MESSAGE_CREATE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CREATE_POSX, CREATE_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_CreateButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CreateButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARACTER_CREATE_BUTTON], EVector2(0.0f, 0.75f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_CreateButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARACTER_CREATE_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(255, 255, 255, 255)));
		_CreateButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARACTER_CREATE_BUTTON], EVector2(0.0f, 0.25f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CreateButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARACTER_CREATE_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 0.75f), MU::MakeARGB(255, 255, 255, 255)));

		_MenuButton = new UIButton(CHARACTERUI::MESSAGE_MENU_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MENU_POSX, MENU_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_MenuButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MenuButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_MENU_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_MenuButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_MENU_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_MenuButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_MENU_BUTTON], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_MenuButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_MENU_BUTTON], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));

		_ConnectButton = new UIButton(CHARACTERUI::MESSAGE_CONNECT_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CONNECT_POSX, CONNECT_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_ConnectButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_ConnectButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARACTER_CONNECT_BUTTON], EVector2(0.0f, 0.75f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ConnectButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARACTER_CONNECT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(255, 255, 255, 255)));
		_ConnectButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARACTER_CONNECT_BUTTON], EVector2(0.0f, 0.25f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_ConnectButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARACTER_CONNECT_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 0.75f), MU::MakeARGB(255, 255, 255, 255)));

		_DeleteButton = new UIButton(CHARACTERUI::MESSAGE_DELETE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, DELETE_POSX, DELETE_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_DeleteButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_DeleteButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARACTER_DELETE_BUTTON], EVector2(0.0f, 0.75f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_DeleteButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARACTER_DELETE_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(255, 255, 255, 255)));
		_DeleteButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARACTER_DELETE_BUTTON], EVector2(0.0f, 0.25f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_DeleteButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARACTER_DELETE_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 0.75f), MU::MakeARGB(255, 255, 255, 255)));

		for (mu_uint32 n = 0; n < MAX_CHARACTERLIST_COUNT; ++n)
		{
			_Balloon[n] = new UICharacterInfoBalloon(manager, this);
			_Balloon[n]->SetCharacter(&CharactersClient[BEGIN_CHARACTERLIST_COUNT + n]);
		}
	}
}

UICharacterFrame::~UICharacterFrame()
{

}

void UICharacterFrame::Update(mu_uint64 CurrentTime)
{

}

void UICharacterFrame::Draw()
{
	_InfoBack->Draw();
	_DecoRight->Draw();
	_CreateButton->Draw();
	_MenuButton->Draw();
	_ConnectButton->Draw();
	_DeleteButton->Draw();

	for (mu_uint32 n = 0; n < MAX_CHARACTERLIST_COUNT; ++n)
	{
		_Balloon[n]->Draw();
	}
}

void UICharacterFrame::PostDraw()
{
	_CreateButton->PostDraw();
	_MenuButton->PostDraw();
	_ConnectButton->PostDraw();
	_DeleteButton->PostDraw();
}

mu_uint32 UICharacterFrame::OnPreWindowMessage(const SDL_Event *event, mu_size *result)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		{
			if (MU_INPUT->IsShiftPressing() == false &&
				MU_INPUT->IsCtrlPressing() == false)
			{
				switch (event->key.keysym.scancode)
				{
				case SDL_SCANCODE_RETURN:
				case SDL_SCANCODE_KP_ENTER:
					{
						if (_ConnectButton->GetState() != UIButton::UI_DISABLED &&
							SelectedHero >= BEGIN_CHARACTERLIST_COUNT + 0 && SelectedHero < MAX_CHARACTERLIST_COUNT)
						{
							_SceneManager->OnGFxMessagePush(_ConnectButton->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
						}
					}
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
				}
			}
		}
		break;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

UIBase *UICharacterFrame::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = _CreateButton->GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		return focus;
	}

	focus = _MenuButton->GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		return focus;
	}

	focus = _ConnectButton->GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		return focus;
	}

	focus = _DeleteButton->GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		return focus;
	}

	return nullptr;
}