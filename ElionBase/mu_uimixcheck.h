#ifndef __MU_UIMIXCHECK_H__
#define __MU_UIMIXCHECK_H__

#pragma once

class UIMixCheckBox : public UIBase
{
public:
	UIMixCheckBox(UISceneManager *manager);
	~UIMixCheckBox();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

private:
	TMSGBOX_VARS;

	UIButton *_AcceptButton;
	UIButton *_CancelButton;
};

#endif