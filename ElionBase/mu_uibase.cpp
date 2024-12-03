#include "stdafx.h"
#include "mu_uibase.h"

void UISceneManager::DestroyGFxObjects()
{
	for (auto it = _GfxObjects.begin(); it != _GfxObjects.end(); it = _GfxObjects.erase(it))
	{
		MU_SAFE_DELETE(*it);
	}
}

mu_uint16 UIBase::DefaultSquareIndex16[6] = { 0, 1, 2, 0, 2, 3 };
mu_uint32 UIBase::DefaultSquareIndex32[6] = { 0, 1, 2, 0, 2, 3 };

mu_boolean CheckInside(mu_float x, mu_float y, mu_float width, mu_float height)
{
	mu_uint32 mouseX = MU_INPUT->GetMouseX(), mouseY = MU_INPUT->GetMouseY();

	return (mouseX >= x && mouseX <= x + width &&
		mouseY >= y && mouseY <= y + height);
}

void UIBase::GiveFocus()
{
	SDL_Event event;
	SDL_memset(&event, 0, sizeof(event));
	event.type = g_ELFocusEventID;
	event.user.data1 = this;
	event.user.data2 = _SceneManager;
	SDL_PushEvent(&event);
}

void UIBase::SetAspectRatio(mu_float aspectWidth, mu_float aspectHeight, mu_boolean convert)
{
	mu_float oldW = _AspectWidth, oldH = _AspectHeight;

	if (convert)
	{
		_ViewAspectWidth = aspectWidth;
		_ViewAspectHeight = aspectHeight;
		_AspectWidth = MU_VIEWPORT_RATE(MU_RESOURCE->GetWindowWidth(), aspectWidth);
		_AspectHeight = MU_VIEWPORT_RATE(MU_RESOURCE->GetWindowHeight(), aspectHeight);
	}
	else
	{
		_ViewAspectWidth = MU_RVIEWPORT_RATE(MU_RESOURCE->GetWindowWidth(), aspectWidth);
		_ViewAspectHeight = MU_RVIEWPORT_RATE(MU_RESOURCE->GetWindowHeight(), aspectHeight);
		_AspectWidth = aspectWidth;
		_AspectHeight = aspectHeight;
	}

	if (oldW != _AspectWidth ||
		oldH != _AspectHeight)
	{
		_UpdateGPU = true;

		SetPosition(_ViewX, _ViewY);
		SetDimensions(_ViewWidth, _ViewHeight);
	}
}

void CheckTextInput(mu_uint32 managersCount, UISceneManager **managers)
{
	if (SDL_IsTextInputActive() == false)
	{
		mu_boolean showKeyboard = false;

		for (mu_uint32 n = 0; n < managersCount; ++n)
		{
			if (managers[n]->GetSelected() != nullptr)
			{
				if (managers[n]->GetSelected()->IsEvent(GFX_EVENT::GFX_KEYBOARD_EVENT) == true)
				{
					showKeyboard = true;
				}
				break;
			}
		}

		if (showKeyboard == true)
		{
			SDL_StartTextInput();
		}
	}
	else
	{
		mu_boolean hideKeyboard = true;

		for (mu_uint32 n = 0; n < managersCount; ++n)
		{
			if (managers[n]->GetSelected() != nullptr)
			{
				if (managers[n]->GetSelected()->IsEvent(GFX_EVENT::GFX_KEYBOARD_EVENT) == true)
				{
					hideKeyboard = false;
				}
				break;
			}
		}

		if (hideKeyboard == true)
		{
			SDL_StopTextInput();
		}
	}
}