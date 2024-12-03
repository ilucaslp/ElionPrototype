#ifndef __MU_UIGUILDREQUESTBOX_H__
#define __MU_UIGUILDREQUESTBOX_H__

#pragma once

class UIGuildRequestBox : public UIBase
{
public:
	UIGuildRequestBox(UISceneManager *manager);
	~UIGuildRequestBox();

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
	UISprite * _BackFrame;
	UIButton *_AcceptButton;
	UIButton *_CancelButton;

	mu_int16 _PlayerKey;
};

#endif