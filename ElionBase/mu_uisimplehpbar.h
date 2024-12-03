#ifndef __MU_UISIMPLEHPBAR_H__
#define __MU_UISIMPLEHPBAR_H__

#pragma once

class UISimpleHPBar : public UIBase
{
public:
	UISimpleHPBar(UISceneManager *manager);
	~UISimpleHPBar();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;

private:
	UISprite *_HPBack[MAX_PARTYS];
	UISprite *_HPBar[MAX_PARTYS];
};

#endif