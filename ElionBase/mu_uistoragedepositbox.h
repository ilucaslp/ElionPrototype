#ifndef __MU_UISTORAGEDEPOSITBOX_H__
#define __MU_UISTORAGEDEPOSITBOX_H__

#pragma once

class UIStorageDepositBox : public UIBase
{
public:
	UIStorageDepositBox(UISceneManager *manager);
	~UIStorageDepositBox();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

private:
	friend class UIMessageRoot;
	UISprite *_BackFrame;
	UISpriteColor *_InputBack;
	UIEditBox *_GoldInput;
	UIButton *_AcceptButton;
	UIButton *_CancelButton;
};

#endif