#ifndef __MU_INPUT_MANAGER_H__
#define __MU_INPUT_MANAGER_H__

#pragma once

enum
{
	MOUSE_STATE_NONE = 0,
	MOUSE_STATE_CLICK = 1,
	MOUSE_STATE_DOUBLECLICK = 2,

	MOUSE_USED_NONE = 0,
	MOUSE_USED_PRESSED = 1,
	MOUSE_USED_ALL = 2,
};

enum
{
	KEYBOARD_KEY_SIZE = 1024,
	KEYBOARD_KEY_PRESSED = 0x80,

	MOUSE_BUTTON_LEFT = 0,
	MOUSE_BUTTON_MIDDLE = 1,
	MOUSE_BUTTON_RIGHT = 2,
	MOUSE_BUTTON_MAX,

	MOUSE_FLAG_PRESSED = 0,
	MOUSE_FLAG_DOUBLECLICK,
	MOUSE_FLAG_PRESSING,
	MOUSE_FLAG_RELEASED,
	MOUSE_FLAGS,

	MOUSE_FLAG_LEFT = 1,
	MOUSE_FLAG_MIDDLE = 2,
	MOUSE_FLAG_RIGHT = 4,
};

enum
{
	VFLAG_LBUTTON	= (1 << 0),
	VFLAG_RBUTTON	= (1 << 1),
	VFLAG_SHIFT		= (1 << 2),
	VFLAG_CONTROL	= (1 << 3),
	VFLAG_MBUTTON	= (1 << 4),
	VFLAG_XBUTTON1	= (1 << 5),
	VFLAG_XBUTTON2	= (1 << 6),
};

namespace CURSOR
{
	enum
	{
		CURSOR_NORMAL,
		CURSOR_PUSH,
		CURSOR_ATTACK,
		CURSOR_ATTACK2,
		CURSOR_GET,
		CURSOR_TALK,
		CURSOR_REPAIR,
		CURSOR_REPAIR_ANIM,
		CURSOR_LEANAGAINST,
		CURSOR_SITDOWN,
		CURSOR_DONTMOVE,
		CURSOR_IDSELECT,
		CURSOR_MAX,
	};

	extern mu_uint32 Texture[CURSOR_MAX];
}

class UISprite;

class MUInputManager
{
public:
	MUInputManager();
	~MUInputManager();

	void Destroy();

	void Update();
	void PostUpdate();
	void RenderCursor(mu_uint64 CurrentTime);
	void ShowCursor(mu_boolean show);

	void SystemConfigure();

	mu_boolean IsCaptureChanged();
	void SetMouseOverInterface(mu_boolean isOver);
	mu_boolean IsMouseOverInterface();

	void ClearKeyState();
	mu_boolean GetKeyState(mu_uint32 Key);
	void SetKeyDown(mu_uint32 Key);
	void SetKeyUp(mu_uint32 Key);
	void ProcessKeys();

	mu_boolean IsKeyPressed(mu_int32 Key);
	mu_boolean IsKeyPressing(mu_int32 Key);
	mu_boolean IsKeyHolding(mu_int32 Key);
	mu_boolean IsKeyReleased(mu_int32 Key);

	void ClearMouseButton();
	void SetMouse(mu_int32 mouse_x, mu_int32 mouse_y);
	void SetMouseButton(mu_uint32 index, mu_uint8 state);
	void SetMouseUsed(mu_uint32 index, mu_uint8 state);
	void SetMouseWheel(mu_int32 mouse_wheel);
	void Set3DMouse(mu_boolean is3D);

	mu_boolean IsAnyMousePressed();
	mu_boolean IsMousePressed(mu_int32 index);
	mu_boolean IsMouseDoublePressed(mu_int32 index);
	mu_boolean IsMousePressing(mu_uint32 index);

	mu_boolean Is3DMouse();
	mu_boolean Is3DMousePressed(mu_int32 index);
	mu_boolean Is3DMouseDoublePressed(mu_int32 index);
	mu_boolean Is3DMousePressedAny(mu_int32 index);
	mu_boolean Is3DMousePressing(mu_uint32 index);

	mu_int32 GetMouseX();
	mu_int32 GetMouseY();
	mu_float GetMouseWheel();

public:
	FORCEINLINE mu_boolean IsShiftPressing()
	{
		return IsKeyPressing(SDL_SCANCODE_LSHIFT) == true || IsKeyPressing(SDL_SCANCODE_RSHIFT) == true;
	}

	FORCEINLINE mu_boolean IsCtrlPressing()
	{
		return IsKeyPressing(SDL_SCANCODE_LCTRL) == true || IsKeyPressing(SDL_SCANCODE_RCTRL) == true;
	}

private:
	friend class MURoot;
	friend class MUTasking;
	friend class MUVideoManager;
	friend class UIOptionWindow;
	friend class UICommandWindow;
	mu_boolean _CaptureChanged;
	mu_boolean _ShowCursor;
	UISprite *_Cursor;
	mu_uint32 _CursorIndex;

	mu_boolean _MouseOverInterface;
	mu_int32 _MouseX, _MouseY;
	mu_float _MouseWheel;
	mu_uint8 _MouseState[MOUSE_BUTTON_MAX];
	mu_uint8 _MouseUsed[MOUSE_BUTTON_MAX];
	mu_boolean _Is3DMouse;

	// _KeyState is message-based keys
	mu_boolean _KeyPressed[KEYBOARD_KEY_SIZE];
	mu_uint8 _KeyState[KEYBOARD_KEY_SIZE];
};

#endif