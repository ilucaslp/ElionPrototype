#ifndef __MU_SCENE_BASE_H__
#define __MU_SCENE_BASE_H__

#pragma once

class MUSceneBase
{
public:
	virtual mu_boolean IsLoaded() { return true; }
	virtual void OnLoad() {}
	virtual void OnUnload() {}

	virtual void OnUpdateGFX(mu_uint64 CurrentTime) {}
	virtual void OnUpdate(mu_double ElapsedTime, mu_uint64 CurrentTime) {}
	virtual void OnPostUpdate() {}
	virtual mu_uint32 OnWindowMessage(const SDL_Event *event, mu_size *result) { return false; }

	virtual mu_boolean RequireSceneSwitch() { return false; }
	virtual MUSceneBase* NextScene() { return nullptr; }
};

#endif