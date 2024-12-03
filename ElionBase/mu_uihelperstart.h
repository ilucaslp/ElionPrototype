#ifndef __MU_UIHELPERSTART_H__
#define __MU_UIHELPERSTART_H__

#pragma once

class UIHelperStart : public UIBase
{
public:
	UIHelperStart(UISceneManager *manager);
	~UIHelperStart();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

private:
	friend class UIMessageRoot;
	TMSGBOX_VARS;

	UIButton *_OnlineButton;
	UIButton *_OfflineButton;
	UIButton *_CancelButton;
};

#endif