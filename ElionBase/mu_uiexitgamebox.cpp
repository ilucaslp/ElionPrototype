#include "stdafx.h"
#include "mu_uiexitgamebox.h"

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 960.0f
#define EXITGAMEBOX_WIDTH 352.0f
#define EXITGAMEBOX_HEIGHT 113.0f
#define EXITGAMEBOX_POSX (SCREEN_WIDTH * 0.5f - EXITGAMEBOX_WIDTH * 0.5f)
#define EXITGAMEBOX_POSY (SCREEN_HEIGHT * 0.5f - EXITGAMEBOX_HEIGHT * 0.5f)

UIExitGameBox::UIExitGameBox(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(EXITGAMEBOX_POSX, EXITGAMEBOX_POSY);
	UIBase::SetDimensions(EXITGAMEBOX_WIDTH, EXITGAMEBOX_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_NO_FOCUS | GFX_FLAG::GFX_HIDDEN);

	// Initialize
	{
		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_WINDOW], SCREEN_WIDTH, SCREEN_HEIGHT, EXITGAMEBOX_POSX, EXITGAMEBOX_POSY, EXITGAMEBOX_WIDTH, EXITGAMEBOX_HEIGHT);
	}
}

UIExitGameBox::~UIExitGameBox()
{

}

void UIExitGameBox::Update(mu_uint64 CurrentTime)
{
	if (CurrentTime >= _ExitTime)
	{
		SDL_Event event;
		SDL_memset(&event, 0, sizeof(event));
		event.type = SDL_QUIT;
		SDL_PushEvent(&event);
	}
}

void UIExitGameBox::PostDraw()
{
	DrawFrame();
	DrawText();
}

void UIExitGameBox::DrawFrame()
{
	_BackFrame->Draw();
}

void UIExitGameBox::DrawText()
{
	mu_int32 secondsLeft = _ExitTime <= g_CurrentTime ? 0 : (mu_int32)EMath::Ceil((mu_double)(_ExitTime - g_CurrentTime) / 1000.0);
	mu_text textBuffer[128] = { 0 };

	mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[380], secondsLeft);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::MESSAGEBOX_FAILEDCONNECT,
		MU_TO_WINDOW(_ViewX + (EXITGAMEBOX_WIDTH * 0.5f), _AspectWidth),
		MU_TO_WINDOW(_ViewY + 56.5f, _AspectHeight),
		1.0f,
		ETA_CENTER,
		ETA_CENTER,
		5.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(0, 0, 0, 0),
		textBuffer);
}

UIBase *UIExitGameBox::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	return UIBase::GetFocus(mouseX, mouseY);
}