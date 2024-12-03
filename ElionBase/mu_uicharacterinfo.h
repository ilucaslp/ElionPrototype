#ifndef __MU_UICHARACTERINFO_H__
#define __MU_UICHARACTERINFO_H__

#pragma once

class UICharacterInfo : public UIBase
{
public:
	UICharacterInfo(UISceneManager *manager, mu_float x, mu_float y, mu_float width, mu_float height);
	~UICharacterInfo();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawButtons();
	void PostDrawButtons();
	void DrawTexts();
	void DrawSubjectTexts();
	void DrawTableTexts();
	void DrawAttribute();

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void Configure();
	void ResetEquipmentLevel();

	void EnableStatButtons();
	void DisableStatButtons();

public:
	enum
	{
		STAT_STRENGTH = 0,
		STAT_DEXTERITY,
		STAT_VITALITY,
		STAT_ENERGY,
		STAT_COMMAND,
		STAT_MAX,
	};

private:
	friend class UIGameRoot;
	UISprite *_FrameBack;
	UISprite *_FrameTop;
	UISprite *_FrameLeft;
	UISprite *_FrameRight;
	UISprite *_FrameBottom;
	UISpriteColor *_FrameBackColor;
	UISprite *_TableTopLeft;
	UISprite *_TableTopRight;
	UISprite *_TableBottomLeft;
	UISprite *_TableBottomRight;
	UISprite *_TableTopPixel;
	UISprite *_TableBottomPixel1;
	UISprite *_TableBottomPixel2;
	UISprite *_TableLeftPixel;
	UISprite *_TableRightPixel;
	UISprite *_StatTextBox[STAT_MAX];

	UIButton *_StatButton[STAT_MAX];
	UIButton *_XButton;
	UIButton *_CloseButton;
	UIButton *_QuestButton;
	UIButton *_PetButton;
	UIButton *_MasterButton;
};

#endif