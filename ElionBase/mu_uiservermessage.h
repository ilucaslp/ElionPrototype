#ifndef __MU_UISERVERMESSAGE_H__
#define __MU_UISERVERMESSAGE_H__

#pragma once

#define MAX_SERVERMESSAGES (5)

class UIServerMessageData
{
public:
	mu_string Message;
	mu_float Width;
	mu_float Height;
};

class UIServerMessage : public UIBase
{
public:
	UIServerMessage(UISceneManager *manager);
	~UIServerMessage();

	void ChangePosition(mu_float x, mu_float y);

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;

	void Clear();
	void AddText(const mu_string &message);

private:
	UISprite *_Background;
	UISprite *_TopFrame;
	UISprite *_BottomFrame;
	UISprite *_LeftFrame;
	UISprite *_RightFrame;

	std::vector<UIServerMessageData> _Messages;
	mu_boolean _UpdateFrame;
};

#endif