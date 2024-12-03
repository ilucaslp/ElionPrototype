#ifndef __MU_SCENES_H__
#define __MU_SCENES_H__

#pragma once

namespace SCENES
{
	extern MULoadingScene LoadingScene;
	extern MULoginScene LoginScene;
 	extern MUCharacterScene CharacterScene;
	extern MUWaitingScene WaitingScene;
	extern MUGameScene GameScene;
	extern MUSceneBase *DefaultScene;
};

extern mu_uint32 g_CurrentScene;

#endif