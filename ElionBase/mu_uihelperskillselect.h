#ifndef __MU_UIHELPERSKILLSELECT_H__
#define __MU_UIHELPERSKILLSELECT_H__

#pragma once

void GenerateSkillIcon(mu_uint32 skillIndex, mu_boolean &CantSkill, mu_float &fU, mu_float &fV, mu_int32 &iKindofSkill);
void ConfigureSkill(UIButton *button, mu_uint32 skillIndex);
void ConfigureSkill(UISprite *icon, mu_uint32 skillIndex);

enum ESKILL_TYPE
{
	EST_ATTACK,
	EST_DEFENSE,
	EST_BUFF,
};

mu_boolean CheckSkillType(mu_uint32 SkillIndex, ESKILL_TYPE eSkillType);

class UIHelperSkillSelect : public UIBase
{
public:
	enum
	{
		MAX_SKILL_SHOW = 8,
	};

public:
	UIHelperSkillSelect(UISceneManager *manager);
	~UIHelperSkillSelect();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void ConfigureIcons();
	void Configure(mu_uint32 SkillSlot, mu_int32 SkillIndex, ESKILL_TYPE eSkillType);

public:
	EINLINE void IncreasePage()
	{
		++_SkillPage;
	}

	EINLINE void DecreasePage()
	{
		if (_SkillPage > 0)
		{
			--_SkillPage;
		}
	}

	EINLINE void SetSelectedSkill(mu_uint32 selectedSkill)
	{
		_SelectedSkill = selectedSkill;
	}

private:
	friend class UIMessageRoot;
	TMSGBOX_VARS;
	TGLAYOUT_VARS(SkillBack);

	UISprite *_SkillDiv;

	UIButton *_LeftArrow;
	UIButton *_RightArrow;

	mu_uint32 _MaxSkillPage;
	mu_uint32 _SkillPage;
	mu_int32 _SkillSelectIndex[MAX_SKILL_SHOW];
	UIButton *_SkillSelect[MAX_SKILL_SHOW];
	UISpriteColor *_SkillSelectBorder[MAX_SKILL_SHOW];

	mu_uint32 _SkillSlot;
	mu_uint32 _SelectedSkill;
	ESKILL_TYPE _SkillType;

	UIButton *_OkButton;
};

#endif