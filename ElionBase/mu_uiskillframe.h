#ifndef __MU_SKILLFRAME_H__
#define __MU_SKILLFRAME_H__

#pragma once

enum KINDOFSKILL
{
	KOS_COMMAND = 1,
	KOS_SKILL1,
	KOS_SKILL2,
};

class UISkillFrame : public UIBase
{
public:
	enum
	{
		VISIBLEKEY_COUNT = 5,
		HOTKEY_COUNT = 10,
	};

public:
	UISkillFrame(UISceneManager *manager);
	~UISkillFrame();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	void ClearSkills();
	void ConfigureSkill(UIButton *button, UISpriteColor *delay, mu_uint32 skillIndex, mu_float x, mu_float y);
	void UpdateSkills();
	void UpdateCurrentSkill();
	void UpdateHotkeySkill();

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

public:
	EINLINE void ShowList(mu_boolean show)
	{
		_ShowSkillList = show;
	}

	EINLINE mu_boolean IsShowList()
	{
		return _ShowSkillList;
	}

	EINLINE mu_int16 GetHotkey(mu_int32 hotkeyIndex)
	{
		return _HotkeyIndex[hotkeyIndex];
	}

	EINLINE void SetHotkey(mu_int32 Hotkey, mu_int32 SkillType)
	{
		for (mu_uint32 n = 0; n < HOTKEY_COUNT; ++n)
		{
			if (_HotkeyIndex[n] == SkillType)
			{
				_HotkeyIndex[n] = -1;
			}
		}

		_HotkeyIndex[Hotkey] = SkillType;
	}

private:
	friend class UIGameRoot;
	UIButton *_SkillIcon[MAX_MAGIC];
	UIButton *_CommandIcon[4];
	UIButton *_HotkeyIcon[VISIBLEKEY_COUNT];
	UIButton *_CurrentIcon;

	UISpriteColor *_SkillDelay[MAX_MAGIC];
	UISpriteColor *_CommandDelay[4];
	UISpriteColor *_HotkeyDelay[VISIBLEKEY_COUNT];
	UISpriteColor *_CurrentDelay;

	UISprite *_SkillUsebox[MAX_MAGIC];
	UISprite *_CommandUsebox[MAX_MAGIC];
	UISprite *_HotkeyUsebox[VISIBLEKEY_COUNT];

	mu_boolean _ShowSkillList;
	mu_int16 _HotkeyIndex[HOTKEY_COUNT];
	mu_int16 _HotkeyUseboxIndex[HOTKEY_COUNT];

	mu_int16 _SkillsTypeIndex[MAX_SKILLS];
};

#endif