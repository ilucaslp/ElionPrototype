#ifndef __MU_UICHANGESERVERBOX_H__
#define __MU_UICHANGESERVERBOX_H__

#pragma once

class UIChangeServerBox : public UIBase
{
public:
	UIChangeServerBox(UISceneManager *manager);
	~UIChangeServerBox();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

public:
	EINLINE void SetExitTime(mu_uint64 exitTime)
	{
		_ExitTime = exitTime;
	}

private:
	UISprite *_BackFrame;

	mu_uint64 _ExitTime;
};

#endif