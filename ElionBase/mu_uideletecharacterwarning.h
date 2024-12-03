#ifndef __MU_UIDELETECHARACTERWARNING_H__
#define __MU_UIDELETECHARACTERWARNING_H__

#pragma once

class UIDeleteCharacterWarning : public UIBase
{
public:
	UIDeleteCharacterWarning(UISceneManager *manager);
	~UIDeleteCharacterWarning();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

public:
	EINLINE void SetMessage(const mu_text *message)
	{
		_Message = message;
	}

private:
	mu_string _Message;
	UISprite *_BackFrame;
	UIButton *_AcceptButton;
	UIButton *_CancelButton;
};

#endif