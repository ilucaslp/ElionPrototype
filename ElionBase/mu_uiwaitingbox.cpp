#include "stdafx.h"
#include "mu_uiwaitingbox.h"

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 960.0f
#define WAITINGBOX_WIDTH 352.0f
#define WAITINGBOX_HEIGHT 113.0f
#define WAITINGBOX_POSX (SCREEN_WIDTH * 0.5f - WAITINGBOX_WIDTH * 0.5f)
#define WAITINGBOX_POSY (SCREEN_HEIGHT * 0.5f - WAITINGBOX_HEIGHT * 0.5f)

UIWaitingBox::UIWaitingBox(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(WAITINGBOX_POSX, WAITINGBOX_POSY);
	UIBase::SetDimensions(WAITINGBOX_WIDTH, WAITINGBOX_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_NO_FOCUS | GFX_FLAG::GFX_HIDDEN);

	// Initialize
	{
		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_WINDOW], SCREEN_WIDTH, SCREEN_HEIGHT, WAITINGBOX_POSX, WAITINGBOX_POSY, WAITINGBOX_WIDTH, WAITINGBOX_HEIGHT);
	}
}

UIWaitingBox::~UIWaitingBox()
{

}

void UIWaitingBox::Update(mu_uint64 CurrentTime)
{

}

void UIWaitingBox::PostDraw()
{
	DrawFrame();
	DrawText();
}

void UIWaitingBox::DrawFrame()
{
	_BackFrame->Draw();
}

void UIWaitingBox::DrawText()
{
	FONT_NORMAL->DrawStringC2D(TEXTINDEX::MESSAGEBOX_FAILEDCONNECT,
		MU_TO_WINDOW(_ViewX + (WAITINGBOX_WIDTH * 0.5f), _AspectWidth),
		MU_TO_WINDOW(_ViewY + (WAITINGBOX_HEIGHT * 0.5f), _AspectHeight),
		1.0f,
		ETA_CENTER,
		ETA_CENTER,
		5.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(0, 0, 0, 0),
		_Message.c_str());
}

UIBase *UIWaitingBox::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	return UIBase::GetFocus(mouseX, mouseY);
}