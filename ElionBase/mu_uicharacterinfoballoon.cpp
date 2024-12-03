#include "stdafx.h"
#include "mu_uicharacterinfoballoon.h"

#define SCREEN_WIDTH 800.0f
#define SCREEN_HEIGHT 600.0f

#define BALLOON_WIDTH 118.0f
#define BALLOON_HEIGHT 54.0f

UICharacterInfoBalloon::UICharacterInfoBalloon(UISceneManager *manager, UIBase *owner) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(0.0f, 0.0f);
	UIBase::SetDimensions(BALLOON_WIDTH, BALLOON_HEIGHT);
	UIBase::SetParent(owner);

	UIBase::SetGfxFlag(GFX_FLAG::GFX_HIDDEN | GFX_FLAG::GFX_NO_FOCUS);

	// Initialize
	{
		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::CHARACTER_BALLOON], SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, BALLOON_WIDTH, BALLOON_HEIGHT);
		_BackFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_NameIndex = g_RandomTextIndex.fetch_add(1);
		_StatusIndex = g_RandomTextIndex.fetch_add(1);
		_ClassIndex = g_RandomTextIndex.fetch_add(1);

		_Update = false;
		_Character = nullptr;
	}
}

UICharacterInfoBalloon::~UICharacterInfoBalloon()
{

}

void UICharacterInfoBalloon::SystemConfigure()
{
	UIBase::SystemConfigure();

	_Update = true;
}

void UICharacterInfoBalloon::Update(mu_uint64 CurrentTime)
{

}

void UICharacterInfoBalloon::Draw()
{
	if (IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == true)
	{
		return;
	}

	if (_Character != nullptr)
	{
		if (_Update == true)
		{
			_Update = false;

			EVector3 Position = _Character->Object.Position + EVector3(0.0f, 0.0f, 350.0f);
			EVector3 Projected = EMath::Project(Position, MU_VIDEO->_CurrentView, MU_VIDEO->_CurrentRayProjection, EVector4(MU_VIDEO->_3DViewport.OffsetX, MU_VIDEO->_3DViewport.OffsetY, MU_VIDEO->_3DViewport.OffsetX + MU_VIDEO->_3DViewport.Width, MU_VIDEO->_3DViewport.OffsetY + MU_VIDEO->_3DViewport.Height));

			SetPosition(Projected.x - MU_TO_WINDOW(BALLOON_WIDTH * 0.5f, _AspectWidth),
				Projected.y - MU_TO_WINDOW(BALLOON_HEIGHT, _AspectHeight),
				false);
			_BackFrame->SetPosition(_NativeX, _NativeY, false);

			if (_Character->CtlCode & CTLCODE_01BLOCKCHAR)
				_NameColor = MU::MakeARGB(0, 255, 255, 255);
			else if (_Character->CtlCode & (CTLCODE_02BLOCKITEM | CTLCODE_10ACCOUNT_BLOCKITEM))
				_NameColor = MU::MakeARGB(255, 217, 39, 255);
			else if (_Character->CtlCode & CTLCODE_EVENT_GM)
				_NameColor = MU::MakeARGB(255, 0, 0, 255);
			else if (_Character->CtlCode & CTLCODE_ADMIN)
				_NameColor = MU::MakeARGB(255, 0, 0, 255);
			else
				_NameColor = MU::MakeARGB(255, 255, 255, 255);

			mu_text textBuffer[512] = { 0 };

			_NameBuffer = _Character->ID;
			
			mu_uint32 statusCode = 0;
			switch (_Character->GuildStatus)
			{
			case GAME::G_NONE: statusCode = 488;	break;
			case GAME::G_PERSON: statusCode = 1330;	break;
			case GAME::G_BATTLE_MASTER: statusCode = 1302;	break;
			case GAME::G_SUB_MASTER: statusCode = 1301;	break;
			case GAME::G_MASTER: statusCode = 1300;	break;
			}

			mu_uint32 backColor = 0;
			mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255);
			mu_uint32 guildColor = MU::MakeARGB(255, 255, 255, 255);
			mu_uint32 pkColor = 0;
			SetPlayerColor(_Character->PK, pkColor);

			const mu_text *pkStatus[6] =
			{
				_T("Hero"),
				_T("Hero"),
				_T("Commoner"),
				_T("Commoner"),
				_T("Murderer"),
				_T("Murderer"),
			};
			mu_uint8 pkLevel = _Character->PK > mu_countof(pkStatus) - 1 ? mu_countof(pkStatus) - 1 : _Character->PK;

			mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("\uE001%08X\uE001\uE000%08X\uE000(\uE000%08X\uE000") SPF_STRING _T("\uE000%08X\uE000 - \uE000%08X\uE000") SPF_STRING _T("\uE000%08X\uE000)"), backColor, textColor, pkColor, pkStatus[pkLevel], textColor, guildColor, GlobalText[statusCode], textColor);
			_StatusBuffer = textBuffer;

			mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("") SPF_STRING _T(" %u"), GetCharacterClassText(_Character->Class), _Character->Level);
			_ClassBuffer = textBuffer;
		}

		_BackFrame->Draw();

		FONT_NORMAL->DrawStringC2D(_NameIndex, _NativeX + _Width * 0.5f, _NativeY + MU_TO_WINDOW(7.0f, _AspectHeight),
			1.0f,
			ETA_CENTER, ETA_BOTTOM, 0.0f, _NameColor, 0, _NameBuffer.c_str());
		FONT_NORMAL->DrawString2D(_StatusIndex, _NativeX + _Width * 0.5f, _NativeY + MU_TO_WINDOW(23.0f, _AspectHeight),
			1.0f,
			ETA_CENTER, ETA_BOTTOM, 0.0f, _StatusBuffer.c_str());
		FONT_NORMAL->DrawStringC2D(_ClassIndex, _NativeX + _Width * 0.5f, _NativeY + MU_TO_WINDOW(39.0f, _AspectHeight),
			1.0f,
			ETA_CENTER, ETA_BOTTOM, 0.0f, MU::MakeARGB(255, 217, 39, 255), 0, _ClassBuffer.c_str());
	}
}