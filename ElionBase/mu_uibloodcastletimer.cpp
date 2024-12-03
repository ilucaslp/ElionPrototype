#include "stdafx.h"
#include "mu_uibloodcastletimer.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define BCT_WIDTH (124.0f)
#define BCT_HEIGHT (81.0f)
#define BCT_POSX (SCREEN_WIDTH - BCT_WIDTH - 3.0f)
#define BCT_POSY (SCREEN_HEIGHT - BCT_HEIGHT - 51.0f)

#define MAX_KILL_MONSTER (65535)

UIBloodCastleTimer::UIBloodCastleTimer(UISceneManager *manager)
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(BCT_POSX, BCT_POSY);
	UIBase::SetDimensions(BCT_WIDTH, BCT_HEIGHT);

	// Initialize
	{
		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::BLOODCASTLE_TIMER], SCREEN_WIDTH, SCREEN_HEIGHT, BCT_POSX, BCT_POSY, BCT_WIDTH, BCT_HEIGHT);
		_BackFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		Reset();
	}
}

UIBloodCastleTimer::~UIBloodCastleTimer()
{
}

void UIBloodCastleTimer::Reset()
{
	_MatchType = BC_STATE_NONE;
	_Time = 0;
	_MaxKillMonster = MAX_KILL_MONSTER;
	_CurKillMonster = 0;

	_MatchCountDownType = EVENTTIMER_TYPE::ETT_NONE;
	_MatchCountDownStart = 0;
}

void UIBloodCastleTimer::UpdateWindow(mu_int32 screenWidth)
{
	mu_float x = (mu_float)screenWidth - BCT_WIDTH - 3.0f;
	mu_float y = PARTYLIST_POSY;

#define SETPOSITION_HELPER(var) var->SetPosition(var->_ViewX - oldX + x, var->_ViewY - oldY + y)

	mu_float oldX = _ViewX, oldY = _ViewY;

	SETPOSITION_HELPER(this);

	SETPOSITION_HELPER(_BackFrame);

#undef SETPOSITION_HELPER
}

void UIBloodCastleTimer::Update(mu_uint64 CurrentTime)
{

}

void UIBloodCastleTimer::Draw()
{
	if (_MatchCountDownType != EVENTTIMER_TYPE::ETT_NONE)
	{
		if (_MatchCountDownType >= EVENTTIMER_TYPE::ETT_BLOODCASTLE_ENTER_CLOSE &&
			_MatchCountDownType <= EVENTTIMER_TYPE::ETT_BLOODCASTLE_END)
		{
			mu_uint64 iTime = (g_CurrentTime - _MatchCountDownStart) / 1000;

			if (iTime > 30)
			{
				_MatchCountDownType = EVENTTIMER_TYPE::ETT_NONE;
			}
			else
			{
				mu_text textBuffer[256];
				mu_uint32 textColor = MU::MakeARGB(128, 128, 255, 255);
				mu_uint32 backColor = MU::MakeARGB(0, 0, 0, 128);

				mu_int32 textNum = 824 + _MatchCountDownType - TYPE_MATCH_CASTLE_ENTER_CLOSE;
				mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[textNum], GlobalText[1146], 30 - iTime);

				FONT_NORMAL->DrawStringC2D(TEXTINDEX::BCT_COUNTDOWN,
					MU_TO_WINDOW(SCREEN_WIDTH * 0.5f, _AspectWidth),
					MU_TO_WINDOW(SCREEN_HEIGHT - 70.0f, _AspectHeight),
					1.0f,
					ETA_CENTER,
					ETA_BOTTOM,
					0.0f,
					textColor,
					backColor,
					textBuffer);
			}
		}
	}

	if (InBloodCastle() == false || _MatchType == BC_STATE_NONE)
	{
		return;
	}

	_BackFrame->Draw();

	mu_text textBuffer[256];
	mu_uint32 textColor = MU::MakeARGB(255, 150, 0, 255);
	mu_uint32 backColor = MU::MakeARGB(0, 0, 0, 0);

	if (_MaxKillMonster != MAX_KILL_MONSTER)
	{
		if (_MatchType == EVENTTIMER_TYPE::ETT_BLOODCASTLE_INFILTRATION)
		{
			mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[866], _CurKillMonster, _MaxKillMonster);
		}
		else
		{
			mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[864], _CurKillMonster, _MaxKillMonster);
		}

		FONT_NORMAL->DrawStringC2D(TEXTINDEX::BCT_COUNT,
			_NativeX + _Width * 0.5f,
			_NativeY + MU_TO_WINDOW(13.0f, _AspectHeight),
			1.0f,
			ETA_CENTER,
			ETA_BOTTOM,
			0.0f,
			textColor,
			backColor,
			textBuffer);
	}

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::BCT_TITLE,
		_NativeX + _Width * 0.5f,
		_NativeY + MU_TO_WINDOW(38.0f, _AspectHeight),
		1.0f,
		ETA_CENTER,
		ETA_BOTTOM,
		0.0f,
		textColor,
		backColor,
		GlobalText[865]);

	mu_int32 Minute = _Time / 60;
	mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("%02d:%02d:%04lld"), Minute, _Time % 60, IWorldTime % 1000);

	if (Minute < 5)
	{
		textColor = MU::MakeARGB(255, 32, 32, 255);
	}

	FONT_BIG->DrawStringC2D(TEXTINDEX::BCT_TIME,
		_NativeX + _Width * 0.5f,
		_NativeY + MU_TO_WINDOW(50.0f, _AspectHeight),
		1.0f,
		ETA_CENTER,
		ETA_BOTTOM,
		0.0f,
		textColor,
		backColor,
		textBuffer);
}

UIBase *UIBloodCastleTimer::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	if (InBloodCastle() == false || _MatchType == BC_STATE_NONE)
	{
		return nullptr;
	}

	return UIBase::GetFocus(mouseX, mouseY);
}

void UIBloodCastleTimer::SetMatchCountDown(mu_uint8 TimerType)
{
	_MatchCountDownType = TimerType;
	_MatchCountDownStart = g_CurrentTime;
}