#ifndef __MU_UIBLOODCASTLERANK_H__
#define __MU_UIBLOODCASTLERANK_H__

#pragma once

class UIBloodCastleRank : public UIBase
{
public:
	UIBloodCastleRank(UISceneManager *manager);
	~UIBloodCastleRank();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

public:
	EINLINE void SetRankData(mu_boolean isSucceed, const GS2CL_RECEIVE_EVENTRANK_DATAEX::RankDataEx &rankData)
	{
		_IsSucceed = isSucceed;
		_RankData = rankData;
	}

private:
	UISprite *_Back;
	UISprite *_Top;
	UISprite *_Middle;
	UISprite *_Bottom;
	UIButton *_OkButton;

	mu_boolean _IsSucceed;
	GS2CL_RECEIVE_EVENTRANK_DATAEX::RankDataEx _RankData;
};

#endif