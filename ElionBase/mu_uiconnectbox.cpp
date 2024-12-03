#include "stdafx.h"
#include "mu_uiconnectbox.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define CONNECTBOX_WIDTH 176.0f
#define CONNECTBOX_HEIGHT 56.5f
#define CONNECTBOX_POSX (SCREEN_WIDTH * 0.5f - CONNECTBOX_WIDTH * 0.5f)
#define CONNECTBOX_POSY (SCREEN_HEIGHT * 0.5f - CONNECTBOX_HEIGHT * 0.5f)

UIConnectBox::UIConnectBox(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(CONNECTBOX_POSX, CONNECTBOX_POSY);
	UIBase::SetDimensions(CONNECTBOX_WIDTH, CONNECTBOX_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_NO_FOCUS | GFX_FLAG::GFX_HIDDEN);

	// Initialize
	{
		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_WINDOW], SCREEN_WIDTH, SCREEN_HEIGHT, CONNECTBOX_POSX, CONNECTBOX_POSY, CONNECTBOX_WIDTH, CONNECTBOX_HEIGHT);
		_BackFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
	}
}

UIConnectBox::~UIConnectBox()
{

}

void UIConnectBox::Update(mu_uint64 CurrentTime)
{

}

void UIConnectBox::PostDraw()
{
	DrawFrame();
	DrawText();
}

void UIConnectBox::DrawFrame()
{
	_BackFrame->Draw();
}

void UIConnectBox::DrawText()
{
	g_NetworkStatusElapsed += g_Tasking._ElapsedTime;
	if (g_NetworkStatusElapsed > g_NetworkStatusFPS)
	{
		mu_uint32 elapsedCount = g_NetworkStatusElapsed / g_NetworkStatusFPS;
		g_NetworkStatusElapsed -= elapsedCount * g_NetworkStatusFPS;
		g_NetworkStatusDots += elapsedCount;
		g_NetworkStatusDots %= 4;
	}

	mu_text information[512];

	mu_strcpy_s(information, mu_countof(information), _T("Waiting for server list"));

	switch (g_NetworkStatusDots)
	{
	case 0:
		mu_strcat_s(information, mu_countof(information), _T("   "));
		break;

	case 1:
		mu_strcat_s(information, mu_countof(information), _T(".  "));
		break;

	case 2:
		mu_strcat_s(information, mu_countof(information), _T(".. "));
		break;

	case 3:
		mu_strcat_s(information, mu_countof(information), _T("..."));
		break;
	}

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::MESSAGEBOX_WAITINGLIST,
		MU_TO_WINDOW(512.0f, MU_RESOURCE->_1024x1024Rate[0]),
		MU_TO_WINDOW(512.5f, MU_RESOURCE->_1024x1024Rate[1]),
		1.0f,
		ETA_CENTER,
		ETA_CENTER,
		5.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(0, 0, 0, 0),
		information,
		MU_TO_WINDOW(128.0f, MU_RESOURCE->_1024x1024Rate[0]));
}