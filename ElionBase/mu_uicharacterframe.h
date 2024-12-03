#ifndef __MU_UICHARACTERFRAME_H__
#define __MU_UICHARACTERFRAME_H__

#pragma once

class UICharacterFrame : public UIBase
{
public:
	UICharacterFrame(UISceneManager *manager);
	~UICharacterFrame();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	virtual mu_uint32 OnPreWindowMessage(const SDL_Event *event, mu_size *result) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

public:
	UISpriteColor *_InfoBack;
	UISprite *_DecoRight;

	UIButton *_CreateButton;
	UIButton *_MenuButton;
	UIButton *_ConnectButton;
	UIButton *_DeleteButton;

	UICharacterInfoBalloon *_Balloon[MAX_CHARACTERLIST_COUNT];
};

#endif