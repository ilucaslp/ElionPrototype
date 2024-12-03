#ifndef __MU_UIWAITINGBOX_H__
#define __MU_UIWAITINGBOX_H__

#pragma once

class UIWaitingBox : public UIBase
{
public:
	UIWaitingBox(UISceneManager *manager);
	~UIWaitingBox();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

public:
	EINLINE void SetMessage(const mu_text *message)
	{
		_Message = message;
	}

private:
	mu_string _Message;
	UISprite *_BackFrame;
};

#endif