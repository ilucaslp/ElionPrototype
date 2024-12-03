#ifndef __MU_LOGIN_SCENE_H__
#define __MU_LOGIN_SCENE_H__

#pragma once

class MULoginScene : public MUSceneBase
{
public:
	MULoginScene();

	virtual mu_boolean IsLoaded() override;
	virtual void OnLoad() override;
	virtual void OnUnload() override;

	virtual void OnUpdateGFX(mu_uint64 CurrentTime) override;
	virtual void OnUpdate(mu_double ElapsedTime, mu_uint64 CurrentTime) override;
	virtual void OnPostUpdate() override;
	virtual mu_uint32 OnWindowMessage(const SDL_Event *event, mu_size *result) override;

	virtual mu_boolean RequireSceneSwitch() override;
	virtual MUSceneBase* NextScene() override;

public:
	EINLINE void SetNextScene(MUSceneBase *nextScene)
	{
		_SwitchScene = nextScene;
	}

private:
	mu_boolean _Initialized;
	MUWalkCamera _Camera;
	MUSceneBase *_SwitchScene;
};

#endif