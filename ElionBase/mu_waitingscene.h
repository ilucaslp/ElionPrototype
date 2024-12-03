#ifndef __MU_WAITINGSCENE_H__
#define __MU_WAITINGSCENE_H__

#pragma once

class MUWaitingScene : public MUSceneBase
{
public:
	MUWaitingScene();

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
	MUSceneBase *_SwitchScene;

private:
	enum
	{
		UIM_TS_BACK0,
		UIM_TS_BACK1,
		UIM_TS_BACK2,
		UIM_TS_BACK3,
		UIM_TS_MAX,
	};

	UISprite *_Sprites[UIM_TS_MAX];
	MUTexture *_Textures[UIM_TS_MAX];
};

#endif