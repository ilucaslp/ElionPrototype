#ifndef __MU_UI_BASE_H__
#define __MU_UI_BASE_H__

#pragma once

namespace GFX_MESSAGE_TYPE
{
	enum
	{
		GFX_MOUSE_PRESSED,
		GFX_MOUSE_RELEASED,
		GFX_MOUSE_CANCELLED,
		GFX_PRESSED,
		GFX_TOGGLE,
		GFX_TEXT_READY,
		GFX_SLIDE,
		GFX_SLIDE_SECTION,
	};
};

namespace GFX_FLAG
{
	enum
	{
		GFX_NONE,
		GFX_NO_ACTION = 1 << 0,
		GFX_NO_FOCUS = 1 << 1,
		GFX_DISABLED = 1 << 2,
		GFX_HIDDEN = 1 << 3,
		GFX_FORCEMESSAGE = 1 << 4,
	};
};

namespace GFX_EVENT
{
	enum
	{
		GFX_NONE = 0,
		GFX_MOUSE_EVENT = 1 << 0,
		GFX_KEYBOARD_EVENT = 1 << 1,
		GFX_JOYSTICK_EVENT = 1 << 2,

		GFX_PREMESSAGE_FOCUS = 1 << 6, // Receive OnPreWindowMessage when focus (1st order)
		GFX_PREMESSAGE_FIRST = 1 << 7, // Receive OnPreWindowMessage (2nd order)
		GFX_MOUSE_DBLCLICK_AS_CLICK = 1 << 8, // Handle double-click as normal click (fast button)
		GFX_MOUSE_DBLCLICK_MARK = 1 << 9, // Mark double-click as mouse flag
	};
};

namespace GFX_MESSAGE_RETURN
{
	enum
	{
		GFX_MESSAGE_NONE,
		GFX_MESSAGE_BYPASS,
		GFX_MESSAGE_ACCEPT,
		GFX_MESSAGE_PARSE,
	};
};

#define USE_COLOR_STATE (MUTexture*)(-1)

struct UI_STATE_DATA
{
	UI_STATE_DATA()
	{
		Texture = nullptr;
		UVBegin = EVector2(0.0f, 0.0f);
		UVEnd = EVector2(1.0f, 1.0f);
		Color = MU::MakeARGB(255, 255, 255, 255);
	}

	UI_STATE_DATA(MUTexture *texture, EVector2 uvBegin, EVector2 uvEnd, mu_uint32 color)
	{
		Texture = texture;
		UVBegin = uvBegin;
		UVEnd = uvEnd;
		Color = color;
	}

	MUTexture *Texture;
	EVector2 UVBegin, UVEnd;
	mu_uint32 Color;
};

struct UI_STATE_TEXT
{
	UI_STATE_TEXT()
	{
		Font = nullptr;
		Text.clear();
		TextColor = MU::MakeARGB(255, 255, 255, 255);
		BackColor = MU::MakeARGB(0, 0, 0, 0);
		MarginX = 0.0f;
		MarginY = 0.0f;
	}

	UI_STATE_TEXT(MUTextSimpleRender *font, const mu_text *text, mu_uint32 textColor, mu_uint32 backColor, mu_float marginX, mu_float marginY)
	{
		Font = font;
		Text = text;
		TextColor = textColor;
		BackColor = backColor;
		MarginX = marginX;
		MarginY = marginY;
	}

	MUTextSimpleRender *Font;
	mu_string Text;
	mu_uint32 TextColor;
	mu_uint32 BackColor;
	mu_float MarginX;
	mu_float MarginY;
	mu_uint8 HorizontalAlign;
};

class UIBase;

#define GFX_BYPASS_FOCUS (UIBase*)(-1)

EINLINE mu_boolean IsValidFocus(UIBase *focus)
{
	return focus != nullptr && focus != GFX_BYPASS_FOCUS;
}

struct GFX_MESSAGE
{
	GFX_MESSAGE(mu_uint32 objectId, mu_uint32 message, mu_size messageData)
	{
		ObjectID = objectId;
		Message = message;
		MessageData = messageData;
	}
	mu_uint32 ObjectID;
	mu_uint32 Message;
	mu_size MessageData;
};

class UISceneManager
{
public:
	~UISceneManager()
	{
		DestroyGFxObjects();
	}

	virtual void UpdateGFX()
	{
		mu_size size = _GfxQueue.size();
		while(size-- > 0)
		{
			OnGFxSceneMessage(_GfxQueue.front());
			_GfxQueue.pop();
		}
	}

	virtual void OnGFxMessagePush(mu_uint32 objectid, mu_uint32 message, mu_size message_data)
	{
		_GfxQueue.push(GFX_MESSAGE(objectid, message, message_data));
	}

	virtual void OnGFxSceneMessage(GFX_MESSAGE &objMsg) {}

	virtual void AddGFxObject(UIBase *object)
	{
		_GfxObjects.push_back(object);
	}

	void DestroyGFxObjects();

public:
	virtual UIBase *GetSelected()
	{
		return nullptr;
	}

	virtual UIBase *GetFocused()
	{
		return nullptr;
	}

	virtual UIBase *GetInternalFocused()
	{
		return nullptr;
	}

public:
	EINLINE void SetRoot(UIBase *root)
	{
		_Root = root;
	}

	EINLINE UIBase *GetRoot()
	{
		return _Root;
	}

protected:
	UIBase *_Root;
	std::queue<GFX_MESSAGE> _GfxQueue;
	std::vector<UIBase*> _GfxObjects;
};

class UIBase
{
public:
	UIBase()
	{
		_ObjectID = MESSAGE_UNKNOWN;
		_SceneManager = nullptr;
		_Parent = nullptr;
		_MessageInheritor = nullptr;
		_GfxFlag = GFX_FLAG::GFX_NONE;
		_Event = GFX_EVENT::GFX_NONE;
		_MouseFlag = 0;
		_UpdateGPU = true;

		_ViewAspectWidth = 0.0f;
		_ViewAspectHeight = 0.0f;
		_AspectWidth = 0.0f;
		_AspectHeight = 0.0f;

		_RouteToParent = false;
	}

	virtual ~UIBase()
	{

	}

	/* Initializers */
	virtual void SystemConfigure()
	{
		_UpdateGPU = true;

		SetAspectRatio(_ViewAspectWidth, _ViewAspectHeight);

		for (auto it : _InheritedObjects)
		{
			it->SystemConfigure();
		}
	}

	virtual void InitializeGPU()
	{
		for (auto it : _InheritedObjects)
		{
			it->InitializeGPU();
		}
	}

	virtual void ReleaseGPU()
	{
		for (auto it : _InheritedObjects)
		{
			it->ReleaseGPU();
		}
	}

	/* Update and Draw */
	virtual void Update(mu_uint64 CurrentTime) {}
	virtual void Draw() {}
	virtual void PostDraw() {}
	virtual void Draw3D() {}

	/* Focus function (windows based, should change for linux) */
	void GiveFocus();

	/* Helpers */
	virtual mu_int32 GetDepthLayer() { return 0; }
	virtual void SetParent(UIBase *parent)
	{
		if (_Parent != nullptr)
		{
			_Parent->_InheritedObjects.erase(std::remove(_Parent->_InheritedObjects.begin(), _Parent->_InheritedObjects.end(), this));
		}

		_Parent = parent;

		if (_Parent != nullptr)
		{
			_Parent->_InheritedObjects.push_back(this);
		}
	}
	virtual void SetMessageInheritor(UIBase *inheritor) { _MessageInheritor = inheritor; }
	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY)
	{
		if (IsGfxFlag(GFX_FLAG::GFX_HIDDEN))
		{
			return nullptr;
		}

		if (IsInside(mouseX, mouseY) == false)
		{
			return nullptr;
		}

		if (IsGfxFlag(GFX_FLAG::GFX_NO_FOCUS))
		{
			return GFX_BYPASS_FOCUS;
		}

		return this;
	}

	virtual mu_boolean IsInside(mu_int32 mouseX, mu_int32 mouseY)
	{
		return (mouseX >= _NativeX && mouseX <= _NativeX + _Width) &&
			(mouseY >= _NativeY && mouseY <= _NativeY + _Height);
	}

	virtual mu_boolean DisableKeyTags() { return false; }
	virtual mu_boolean BypassMessage() { return false; }

	/* Events */
	virtual void OnFocus() {}
	virtual void OnFocusCancelled() {}
	virtual mu_uint32 OnPreWindowMessage(const SDL_Event *event, mu_size *result) { return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE; }
	virtual mu_uint32 OnWindowMessage(const SDL_Event *event, mu_size *result) { return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE; }
	virtual mu_uint32 OnPostWindowMessage(const SDL_Event *event, mu_size *result) { return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE; }
	virtual void OnKeyTag() {}
	virtual void OnMouseMove(mu_int32 mouse_x, mu_int32 mouse_y) {}
	virtual void OnMouseOver(mu_int32 mouse_x, mu_int32 mouse_y) {}
	virtual void OnMouseLeave() {}
	virtual void OnMouseWheel(mu_float mouse_wheel) {}
	virtual void OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys) {}
	virtual void OnMouseReleased(mu_uint32 mouse_flags, mu_uint32 virtual_keys) {}
	virtual void OnMouseCancelled(mu_uint32 mouse_flags) { _MouseFlag &= ~mouse_flags; }
	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) { return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE; }
	virtual mu_uint32 OnPostKeyPressed(const SDL_Keysym &key) { return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE; }
	virtual mu_uint32 OnKeyPressed(const SDL_Keysym &key) { return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE; }
	virtual mu_boolean OnGFxMessage(mu_uint32 objectid, mu_uint32 message, mu_size message_data) { return false; }

	/* Flags Helpers */
	EINLINE mu_uint32 GetObjectID()
	{
		return _ObjectID;
	}

	EINLINE UIBase *GetParent()
	{
		return _Parent;
	}

	EINLINE UIBase *GetMessageInheritor()
	{
		return _MessageInheritor;
	}

	EINLINE mu_boolean IsGfxFlag(mu_uint32 flag)
	{
		return (_GfxFlag & flag) == flag;
	}

	EINLINE void SetGfxFlag(mu_uint32 flag)
	{
		_GfxFlag = flag;
	}

	EINLINE void MarkGfxFlag(mu_uint32 flag)
	{
		_GfxFlag |= flag;
	}

	EINLINE void UnmarkGfxFlag(mu_uint32 flag)
	{
		_GfxFlag &= ~flag;
	}

	EINLINE void ToggleGfxFlag(mu_uint32 flag)
	{
		_GfxFlag ^= flag;
	}

	EINLINE mu_boolean IsMouseFlag(mu_uint32 flag)
	{
		return (_MouseFlag & flag) == flag;
	}

	EINLINE void MarkMouseFlag(mu_uint32 flag)
	{
		_MouseFlag |= flag;
	}

	EINLINE void UnmarkMouseFlag(mu_uint32 flag)
	{
		_MouseFlag &= ~flag;
	}
	
	EINLINE mu_boolean IsEvent(mu_uint32 event)
	{
		return (_Event & event) == event;
	}

	EINLINE void SetEvent(mu_uint32 event)
	{
		_Event = event;
	}

	EINLINE void MarkEvent(mu_uint32 event)
	{
		_Event |= event;
	}

	EINLINE void UnmarkEvent(mu_uint32 event)
	{
		_Event &= ~event;
	}

	void SetAspectRatio(mu_float aspectWidth, mu_float aspectHeight, mu_boolean convert = true);

#if 0
#define GUI_ROUND(x) EMath::Round(x)
#else
#define GUI_ROUND(x) (x)
#endif

	EINLINE void SetPosition(mu_float x, mu_float y, mu_boolean convert = true)
	{
		mu_float oldX = _ViewX, oldY = _ViewY;

		x = GUI_ROUND(x);
		y = GUI_ROUND(y);

		if (convert)
		{
			_ViewX = x;
			_ViewY = y;
			_NativeX = GUI_ROUND(MU_TO_WINDOW(x, _AspectWidth));
			_NativeY = GUI_ROUND(MU_TO_WINDOW(y, _AspectHeight));
		}
		else
		{
			_ViewX = GUI_ROUND(MU_TO_VIEWPORT(x, _AspectWidth));
			_ViewY = GUI_ROUND(MU_TO_VIEWPORT(y, _AspectHeight));
			_NativeX = x;
			_NativeY = y;
		}

		if (oldX != _ViewX ||
			oldY != _ViewY)
		{
			_UpdateGPU = true;
		}
	}

	EINLINE void SetDimensions(mu_float width, mu_float height, mu_boolean convert = true)
	{
		mu_float oldWidth = _ViewWidth, oldHeight = _ViewHeight;

		width = GUI_ROUND(width);
		height = GUI_ROUND(height);

		if (convert)
		{
			_ViewWidth = width;
			_ViewHeight = height;
			_Width = GUI_ROUND(MU_TO_WINDOW(width, _AspectWidth));
			_Height = GUI_ROUND(MU_TO_WINDOW(height, _AspectHeight));
		}
		else
		{
			_ViewWidth = GUI_ROUND(MU_TO_VIEWPORT(width, _AspectWidth));
			_ViewHeight = GUI_ROUND(MU_TO_VIEWPORT(height, _AspectHeight));
			_Width = width;
			_Height = height;
		}

		if (oldWidth != _ViewWidth ||
			oldHeight != _ViewHeight)
		{
			_UpdateGPU = true;
		}
	}

	EINLINE mu_float GetAspectWidth()
	{
		return _AspectWidth;
	}

	EINLINE mu_float GetAspectHeight()
	{
		return _AspectHeight;
	}

	EINLINE mu_boolean IsRouteToParent()
	{
		return _RouteToParent;
	}

	EINLINE void SetRouteToParent(mu_boolean route)
	{
		_RouteToParent = route;
	}

public:
	friend class UIGameRoot;
	mu_uint32 _ObjectID;
	UISceneManager *_SceneManager;
	UIBase *_Parent;
	UIBase *_MessageInheritor;
	mu_uint32 _GfxFlag, _Event;
	mu_uint32 _MouseFlag;
	mu_float _ViewAspectWidth, _ViewAspectHeight;
	mu_float _AspectWidth, _AspectHeight;
	mu_float _NativeX, _NativeY;
	mu_float _Width, _Height;
	mu_float _ViewX, _ViewY;
	mu_float _ViewWidth, _ViewHeight;

	mu_boolean _UpdateGPU;
	mu_boolean _RouteToParent;

	std::vector<UIBase*> _InheritedObjects;

public:
	static mu_uint16 DefaultSquareIndex16[6];
	static mu_uint32 DefaultSquareIndex32[6];
};

class UIRenderTextureBase : public UIBase
{
public:
	UIRenderTextureBase() : UIBase(), _BlendState(EBlendState::eOne_Zero), _DepthStencil(EDepthStencil::eNoZBuffer), _Sampler(ESampler::eUcVcWcMinLMagLMipL)
	{

	}

	EINLINE void SetBlendState(const EBlendState state)
	{
		_BlendState = state;
	}
	EINLINE void SetDepthStencil(const EDepthStencil state)
	{
		_DepthStencil = state;
	}
	EINLINE void SetSampler(const ESampler state)
	{
		_Sampler = state;
	}

protected:
	EBlendState _BlendState;
	EDepthStencil _DepthStencil;
	ESampler _Sampler;
};

mu_boolean CheckInside(mu_float x, mu_float y, mu_float width, mu_float height);
void CheckTextInput(mu_uint32 managersCount, UISceneManager **managers);

#endif