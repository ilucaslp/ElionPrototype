#ifndef __MU_UIMAPNAME_H__
#define __MU_UIMAPNAME_H__

#pragma once

class UIMapName : public UIBase
{
public:
	enum
	{
		ST_FADEIN,
		ST_FADEOUT,
		ST_SHOW,
		ST_HIDE,
	};

public:
	UIMapName(UISceneManager *manager);
	~UIMapName();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void Show();

private:
	std::map<mu_int32, mu_uint32> _WorldMap;

	mu_float _DeltaTime;
	mu_uint32 _State;
	mu_int32 _World;
	mu_float _Alpha;
	mu_boolean _Strife;
	UISprite *_Logo;
};

#endif