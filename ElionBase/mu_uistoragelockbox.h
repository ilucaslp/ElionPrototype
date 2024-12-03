#ifndef __MU_UISTORAGELOCKBOX_H__
#define __MU_UISTORAGELOCKBOX_H__

#pragma once

class UIStorageLockBox : public UIBase
{
public:
	UIStorageLockBox(UISceneManager *manager);
	~UIStorageLockBox();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

public:
	EINLINE void SetRequestState(mu_uint8 State)
	{
		_RequestState = State;
	}

private:
	friend class UIMessageRoot;
	UISprite *_BackFrame;
	UISpriteColor *_InputBack;
	UIEditBox *_PasswordInput;
	UIButton *_AcceptButton;
	UIButton *_CancelButton;

	mu_uint8 _RequestState;
};

#endif