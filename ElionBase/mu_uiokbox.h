#ifndef __MU_UIOKBOX_H__
#define __MU_UIOKBOX_H__

#pragma once

class UIOkBox : public UIBase
{
public:
	UIOkBox(UISceneManager *manager);
	~UIOkBox();

	void Configure();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void ClearMessages();
	void AddMessage(MUTextSimpleRender *font, const mu_text *message, mu_uint32 color = MU::MakeARGB(255, 255, 255, 255));

private:
	mu_float SeparateText(MUTextSimpleRender *font, const mu_text *message, mu_uint32 color);

private:
	struct MESSAGE_DATA
	{
		mu_string message;
		mu_uint32 color;
		MUTextSimpleRender *font;
	};

	mu_float _TextHeight;
	std::vector<MESSAGE_DATA> _Messages;

private:
	UISprite *_Back;
	UISprite *_Top;
	UISprite *_Middle;
	UISprite *_Bottom;
	UIButton *_OkButton;
};

#endif