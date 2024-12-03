#include "stdafx.h"
#include "mu_scenes.h"

namespace SCENES
{
	MULoadingScene LoadingScene;
	MULoginScene LoginScene;
 	MUCharacterScene CharacterScene;
	MUWaitingScene WaitingScene;
 	MUGameScene GameScene;
	MUSceneBase *DefaultScene = &LoadingScene;
};

mu_uint32 g_CurrentScene = STARTUP_SCENE;