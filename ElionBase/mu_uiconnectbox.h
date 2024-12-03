#ifndef __MU_UICONNECTBOX_H__
#define __MU_UICONNECTBOX_H__

#pragma once

class UIConnectBox : public UIBase
{
public:
	UIConnectBox(UISceneManager *manager);
	~UIConnectBox();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();

private:
	UISprite *_BackFrame;
};

#endif