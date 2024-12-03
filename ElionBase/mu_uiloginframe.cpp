#include "stdafx.h"
#include "mu_uiloginframe.h"

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 960.0f
#define BLACK_HEIGHT 50.0f

#define DECO_WIDTH 189.0f
#define DECO_HEIGHT 103.0f
#define DECO_POSX (SCREEN_WIDTH - DECO_WIDTH)
#define DECO_POSY (SCREEN_HEIGHT - BLACK_HEIGHT - DECO_HEIGHT - 1.0f)

#define MENU_WIDTH 54.0f
#define MENU_HEIGHT 30.0f
#define MENU_POSX 22.0f
#define MENU_POSY (SCREEN_HEIGHT - MENU_HEIGHT - BLACK_HEIGHT - 18.0f)

#define WEBSITE_WIDTH 60.0f
#define WEBSITE_HEIGHT 30.0f
#define FACEBOOK_POSX (MENU_POSX)
#define FACEBOOK_POSY (MENU_POSY - 5.0f - WEBSITE_HEIGHT)

#define FORUM_POSX (MENU_POSX)
#define FORUM_POSY (FACEBOOK_POSY - 2.0f - WEBSITE_HEIGHT)

#define WEBSITE_POSX (MENU_POSX)
#define WEBSITE_POSY (FORUM_POSY - 2.0f - WEBSITE_HEIGHT)

UILoginFrame::UILoginFrame(UISceneManager *manager) : UIBase()
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
		_DecoRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_DECORATION], SCREEN_WIDTH, SCREEN_HEIGHT, DECO_POSX, DECO_POSY, DECO_WIDTH, DECO_HEIGHT);
		_DecoRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_MenuButton = new UIButton(LOGINUI::MESSAGE_MENU_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MENU_POSX, MENU_POSY, MENU_WIDTH, MENU_HEIGHT);
		_MenuButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MenuButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_MENU_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_MenuButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_MENU_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_MenuButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_MENU_BUTTON], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_MenuButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_MENU_BUTTON], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));

		_WebsiteButton = new UIButton(LOGINUI::MESSAGE_WEBSITE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, WEBSITE_POSX, WEBSITE_POSY, WEBSITE_WIDTH, WEBSITE_HEIGHT);
		_WebsiteButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_WebsiteButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_WebsiteButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_WebsiteButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_WebsiteButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_WebsiteButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[10000], MU::MakeARGB(170, 170, 170, 255), MU::MakeARGB(0, 0, 0, 0), -1.5f);
		_WebsiteButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[10000], MU::MakeARGB(200, 200, 200, 255), MU::MakeARGB(0, 0, 0, 0), -1.5f);
		_WebsiteButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[10000], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 0), -1.5f);
		_WebsiteButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[10000], MU::MakeARGB(200, 200, 200, 255), MU::MakeARGB(0, 0, 0, 0), -1.5f);

		_ForumButton = new UIButton(LOGINUI::MESSAGE_FORUM_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, FORUM_POSX, FORUM_POSY, WEBSITE_WIDTH, WEBSITE_HEIGHT);
		_ForumButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_ForumButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_ForumButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_ForumButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_ForumButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_ForumButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[10001], MU::MakeARGB(170, 170, 170, 255), MU::MakeARGB(0, 0, 0, 0), -1.5f);
		_ForumButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[10001], MU::MakeARGB(200, 200, 200, 255), MU::MakeARGB(0, 0, 0, 0), -1.5f);
		_ForumButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[10001], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 0), -1.5f);
		_ForumButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[10001], MU::MakeARGB(200, 200, 200, 255), MU::MakeARGB(0, 0, 0, 0), -1.5f);

		_FacebookButton = new UIButton(LOGINUI::MESSAGE_FACEBOOK_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, FACEBOOK_POSX, FACEBOOK_POSY, WEBSITE_WIDTH, WEBSITE_HEIGHT);
		_FacebookButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_FacebookButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_FacebookButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_FacebookButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_FacebookButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_FacebookButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[10002], MU::MakeARGB(170, 170, 170, 255), MU::MakeARGB(0, 0, 0, 0), -1.5f);
		_FacebookButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[10002], MU::MakeARGB(200, 200, 200, 255), MU::MakeARGB(0, 0, 0, 0), -1.5f);
		_FacebookButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[10002], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 0), -1.5f);
		_FacebookButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[10002], MU::MakeARGB(200, 200, 200, 255), MU::MakeARGB(0, 0, 0, 0), -1.5f);
	}
}

UILoginFrame::~UILoginFrame()
{

}

void UILoginFrame::Update(mu_uint64 CurrentTime)
{

}

void UILoginFrame::Draw()
{
	_DecoRight->Draw();
	_MenuButton->Draw();
	_WebsiteButton->Draw();
	_ForumButton->Draw();
	_FacebookButton->Draw();
}

void UILoginFrame::PostDraw()
{
	_MenuButton->PostDraw();
	_WebsiteButton->PostDraw();
	_ForumButton->PostDraw();
	_FacebookButton->PostDraw();
}

UIBase *UILoginFrame::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = _MenuButton->GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		return focus;
	}

	focus = _WebsiteButton->GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		return focus;
	}

	focus = _ForumButton->GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		return focus;
	}

	focus = _FacebookButton->GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		return focus;
	}

	return nullptr;
}