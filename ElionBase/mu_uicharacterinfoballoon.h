#ifndef __MU_UICHARACTERINFOBALLOON_H__
#define __MU_UICHARACTERINFOBALLOON_H__

#pragma once

class UICharacterInfoBalloon : public UIBase
{
public:
	UICharacterInfoBalloon(UISceneManager *manager, UIBase *owner);
	~UICharacterInfoBalloon();

	virtual void SystemConfigure() override;

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;

public:
	EINLINE void SetCharacter(CHARACTER *Character)
	{
		_Update = true;
		_Character = Character;
	}

	EINLINE void ForceUpdate()
	{
		_Update = true;
	}

private:
	UISprite *_BackFrame;
	mu_string _NameBuffer;
	mu_uint32 _NameColor;
	mu_uint32 _NameIndex;
	mu_string _StatusBuffer;
	mu_uint32 _StatusIndex;
	mu_string _ClassBuffer;
	mu_uint32 _ClassIndex;

	mu_boolean _Update;
	CHARACTER *_Character;
};

#endif