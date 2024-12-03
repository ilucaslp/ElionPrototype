#include "stdafx.h"
#include "mu_uichangeserverbox.h"

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 960.0f
#define CHANGESERVERBOX_WIDTH 352.0f
#define CHANGESERVERBOX_HEIGHT 113.0f
#define CHANGESERVERBOX_POSX (SCREEN_WIDTH * 0.5f - CHANGESERVERBOX_WIDTH * 0.5f)
#define CHANGESERVERBOX_POSY (SCREEN_HEIGHT * 0.5f - CHANGESERVERBOX_HEIGHT * 0.5f)

UIChangeServerBox::UIChangeServerBox(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(CHANGESERVERBOX_POSX, CHANGESERVERBOX_POSY);
	UIBase::SetDimensions(CHANGESERVERBOX_WIDTH, CHANGESERVERBOX_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_NO_FOCUS | GFX_FLAG::GFX_HIDDEN);

	// Initialize
	{
		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_WINDOW], SCREEN_WIDTH, SCREEN_HEIGHT, CHANGESERVERBOX_POSX, CHANGESERVERBOX_POSY, CHANGESERVERBOX_WIDTH, CHANGESERVERBOX_HEIGHT);
	}
}

UIChangeServerBox::~UIChangeServerBox()
{

}

void UIChangeServerBox::Update(mu_uint64 CurrentTime)
{
	if (CurrentTime >= _ExitTime)
	{
		g_GameServerObject->Disconnect();
		SCENES::CharacterScene.SetNextScene(&SCENES::LoginScene);

		g_pMessageRoot->Hide(MESSAGEBOX::INTERFACE_CHANGESERVER);
	}
}

void UIChangeServerBox::PostDraw()
{
	DrawFrame();
	DrawText();
}

void UIChangeServerBox::DrawFrame()
{
	_BackFrame->Draw();
}

void UIChangeServerBox::DrawText()
{
	mu_int32 secondsLeft = _ExitTime <= g_CurrentTime ? 0 : (mu_int32)EMath::Ceil((mu_double)(_ExitTime - g_CurrentTime) / 1000.0);
	mu_text textBuffer[128] = { 0 };

	mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("Change server in %d seconds"), secondsLeft);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::MESSAGEBOX_FAILEDCONNECT,
		MU_TO_WINDOW(_ViewX + (CHANGESERVERBOX_WIDTH * 0.5f), _AspectWidth),
		MU_TO_WINDOW(_ViewY + 56.5f, _AspectHeight),
		1.0f,
		ETA_CENTER,
		ETA_CENTER,
		5.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(0, 0, 0, 0),
		textBuffer);
}

UIBase *UIChangeServerBox::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	return UIBase::GetFocus(mouseX, mouseY);
}