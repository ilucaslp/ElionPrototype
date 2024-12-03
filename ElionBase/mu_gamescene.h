#ifndef __MU_GAME_SCENE_H__
#define __MU_GAME_SCENE_H__

#pragma once

#define MAX_OUTSIDES 2

class MUGameScene : public MUSceneBase
{
public:
	MUGameScene();

	virtual mu_boolean IsLoaded() override;
	virtual void OnLoad() override;
	virtual void OnUnload() override;

	virtual void OnUpdateGFX(mu_uint64 CurrentTime) override;
	virtual void OnUpdate(mu_double ElapsedTime, mu_uint64 CurrentTime) override;
	virtual void OnPostUpdate() override;
	virtual mu_uint32 OnWindowMessage(const SDL_Event *event, mu_size *result) override;

	virtual mu_boolean RequireSceneSwitch() override;
	virtual MUSceneBase* NextScene() override;

	void SettingOutsides();
	void RenderOutsides();

	void SettingCamera();

public:
	EINLINE void SetNextScene(MUSceneBase *nextScene)
	{
		_SwitchScene = nextScene;
	}

private:
	mu_boolean _Initialized;
	MUOrbitCamera _Camera;
	mu_int32 _OutsideWorld;
	UISpriteUV *_Outsides[MAX_OUTSIDES];
	MUSceneBase *_SwitchScene;
};

#endif