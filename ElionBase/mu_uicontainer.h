#ifndef __MU_UI_CONTAINER_H__
#define __MU_UI_CONTAINER_H__

#pragma once

class UISimpleContainer
{
public:
	UISimpleContainer(UISceneManager *manager);
	~UISimpleContainer();

	mu_boolean AddElement(UIBase *uiBase);
};

#endif