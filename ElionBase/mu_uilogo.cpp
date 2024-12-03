#include "stdafx.h"
#include "mu_uilogo.h"

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 960.0f

#define LOGO_WIDTH 480.0f
#define LOGO_HEIGHT 100.0f
#define LOGO_POSX (SCREEN_WIDTH * 0.5f - LOGO_WIDTH * 0.5f)
#define LOGO_POSY (SCREEN_HEIGHT * 0.20f - LOGO_HEIGHT * 0.5f)

UILogo::UILogo(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	// Initialize
	{
		_LastTime = 0;
		_ElapsedTime = 0.0;
		_LogoAlpha = 0.0f;

		_Logo = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_LOGO], SCREEN_WIDTH, SCREEN_HEIGHT, LOGO_POSX, LOGO_POSY, LOGO_WIDTH, LOGO_HEIGHT);
		_Logo->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
	}
}

UILogo::~UILogo()
{
}

void UILogo::Update(mu_uint64 CurrentTime)
{
	mu_uint32 updateCount = 0.0f;

	if (_LastTime == 0)
	{
		_LastTime = CurrentTime;
		updateCount = 1;
	}
	else
	{
		_ElapsedTime += CurrentTime - _LastTime;
		_LastTime = CurrentTime;

		updateCount = EMath::Floor(_ElapsedTime / GAME_FPS);
		_ElapsedTime -= updateCount * GAME_FPS;
	}

	if (_LogoAlpha < 1.3f && updateCount > 0)
	{
		_LogoAlpha += 0.02f * updateCount;

		if (_LogoAlpha > 1.3f)
		{
			_LogoAlpha = 1.3f;
		}

		mu_float clampAlpha = EMath::Min(_LogoAlpha, 1.0f);

		_Logo->SetColor(MU::Float4ToARGB(clampAlpha, clampAlpha, clampAlpha, clampAlpha));
	}
}

void UILogo::Draw()
{
	if (_LogoAlpha > 0.25f)
	{
		_Logo->Draw();
	}
}

UIBase *UILogo::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	return nullptr;
}