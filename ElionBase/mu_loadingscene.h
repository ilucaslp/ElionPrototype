#ifndef __MU_LOADING_SCENE_H__
#define __MU_LOADING_SCENE_H__

#pragma once

class MULoadingScene : public MUSceneBase
{
public:
	MULoadingScene();

	virtual mu_boolean IsLoaded() override;
	virtual void OnLoad() override;
	virtual void OnUnload() override;

	virtual void OnUpdateGFX(mu_uint64 CurrentTime) override;
	virtual void OnUpdate(mu_double ElapsedTime, mu_uint64 CurrentTime) override;

	virtual mu_boolean RequireSceneSwitch() override;
	virtual MUSceneBase* NextScene() override;

private:
	mu_boolean _Initialized, _IsLastRender;

private:
	enum
	{
		UIM_TS_BACK0,
		UIM_TS_BACK1,
		UIM_TS_BACK2,
		UIM_TS_BACK3,
		UIM_TS_BACK4,
		UIM_TS_BACK5,
		UIM_TS_BACK6,
		UIM_TS_BACK7,
		UIM_TS_BACK8,
		UIM_TS_BACK9,
		UIM_TS_BAR,
		UIM_TS_MAX,
	};

	UISprite *_Sprites[UIM_TS_MAX];
	MUTexture *_Textures[UIM_TS_MAX];

	mu_boolean _IsVerticalSync;
};

#endif