#ifndef __MU_UIGUILDCANCELSTATUSBOX_H__
#define __MU_UIGUILDCANCELSTATUSBOX_H__

#pragma once

class UIGuildCancelStatusBox : public UIBase
{
public:
	UIGuildCancelStatusBox(UISceneManager *manager);
	~UIGuildCancelStatusBox();

	void Prepare(GUILD_LIST_t *member);

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

private:
	friend class UIMessageRoot;
	UISprite * _Back;
	UISprite *_Top;
	UISprite *_Middle;
	UISprite *_Bottom;

	UIButton *_AcceptButton;
	UIButton *_CancelButton;

	mu_string _LeaveMessage;
	GUILD_LIST_t _Member;
};

#endif