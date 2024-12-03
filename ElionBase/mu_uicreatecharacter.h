#ifndef __MU_UICREATECHARACTER_H__
#define __MU_UICREATECHARACTER_H__

#pragma once

class UICreateCharacter : public UIBase
{
public:
	UICreateCharacter(UISceneManager *manager);
	~UICreateCharacter();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void Draw3D() override;
	virtual void PostDraw() override;

	virtual mu_uint32 OnPreWindowMessage(const SDL_Event *event, mu_size *result) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void SetSelectedClass(mu_uint32 Class);

public:
	UISpriteColor *_Background;
	UISprite *_InputFrame;
	UISpriteColor *_StatBack;
	UISpriteColor *_DescBack;

	UIEditBox *_InputBox;
	UIButton *_OkButton;
	UIButton *_CancelButton;
	
	mu_uint32 _SelectedClass;
	UIRadioButton *_ClassesButton[GAME::MAX_CLASS];

	mu_uint32 _SlotIndex;
	mu_uint32 _SlotCount;
};

#endif