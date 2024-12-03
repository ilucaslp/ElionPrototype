#ifndef __MU_UILOGO_H__
#define __MU_UILOGO_H__

#pragma once

class UILogo : public UIBase
{
public:
	UILogo(UISceneManager *manager);
	~UILogo();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

private:
	mu_uint64 _LastTime;
	mu_double _ElapsedTime;
	mu_float _LogoAlpha;
	UISprite *_Logo;
};

#endif