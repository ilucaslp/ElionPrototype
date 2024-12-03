#ifndef __MU_UIBLOODCASTLETIMER_H__
#define __MU_UIBLOODCASTLETIMER_H__

#pragma once

enum
{
	BC_STATE_NONE = 0xFF,
	BC_STATE_START = 0,
	BC_STATE_PLAY = 1,
	BC_STATE_END = 2,
	BC_STATE_MONSTEREND = 3,
	BC_STATE_PLAY_BOSS = 4,
};

class UIBloodCastleTimer : public UIBase
{
public:
	UIBloodCastleTimer(UISceneManager *manager);
	~UIBloodCastleTimer();

	void Reset();

	void UpdateWindow(mu_int32 screenWidth);

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void SetMatchCountDown(mu_uint8 TimerType);

public:
	EINLINE void SetKillMonsterStatue(mu_int32 curKill, mu_int32 maxKill)
	{
		_CurKillMonster = curKill;
		_MaxKillMonster = maxKill;
	}

	EINLINE void SetMatchType(mu_uint8 matchType)
	{
		_MatchType = matchType;
	}

	EINLINE void SetTime(mu_int32 Time)
	{
		_Time = Time;
	}

private:
	UISprite *_BackFrame;

private:
	mu_uint8 _MatchCountDownType;
	mu_uint64 _MatchCountDownStart;

	mu_int32 _Time;
	mu_int32 _MaxKillMonster;
	mu_int32 _CurKillMonster;
	mu_uint8 _MatchType;
};

#endif