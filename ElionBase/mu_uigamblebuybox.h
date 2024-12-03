#ifndef __MU_UIGAMBLEBUYBOX_H__
#define __MU_UIGAMBLEBUYBOX_H__

#pragma once

class UIGambleBuyBox : public UIBase
{
public:
	UIGambleBuyBox(UISceneManager *manager);
	~UIGambleBuyBox();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw3D() override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

private:
	friend class UIMessageRoot;
	UISprite *_Back;
	UISprite *_Top;
	UISprite *_Middle;
	UISprite *_Bottom;
	UIButton *_AcceptButton;
	UIButton *_CancelButton;

	mu_uint32 _SlotIndex;
	mu_uint32 _SlotCount;
};

#endif