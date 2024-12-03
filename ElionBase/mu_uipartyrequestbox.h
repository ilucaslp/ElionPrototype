#ifndef __MU_UIPARTYREQUESTBOX_H__
#define __MU_UIPARTYREQUESTBOX_H__

#pragma once

class UIPartyRequestBox : public UIBase
{
public:
	UIPartyRequestBox(UISceneManager *manager);
	~UIPartyRequestBox();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

public:
	EINLINE void SetPlayerKey(mu_int16 PlayerKey)
	{
		_PlayerKey = PlayerKey;
	}

private:
	UISprite *_BackFrame;
	UIButton *_AcceptButton;
	UIButton *_CancelButton;

	mu_int16 _PlayerKey;
};

#endif