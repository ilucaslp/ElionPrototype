#ifndef __MU_UIMOVEWINDOW_H__
#define __MU_UIMOVEWINDOW_H__

#pragma once

#define MOVEWINDOW_MAXLINES 20

class UIMoveWindow : public UIBase
{
public:
	UIMoveWindow(UISceneManager *manager);
	~UIMoveWindow();

	virtual void SystemConfigure() override;

	virtual void Draw() override;
	virtual void PostDraw() override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	virtual mu_uint32 OnPreWindowMessage(const SDL_Event *event, mu_size *result) override;

	void OnScrolling();
	
private:
	friend class MUMoveManager;
	friend class UIGameRoot;
	UISpriteColor *_Background;
	UIScrollBar *_ScrollBar;
	UIButton *_CloseButton;
	UIButton *_SelectButton[MOVEWINDOW_MAXLINES];

	mu_int32 _BeginIndex;
	mu_int32 _SelectedIndex;
};

#endif