#ifndef __MU_UISCROLLBAR_H__
#define __MU_UISCROLLBAR_H__

#pragma once

#define UI_SCROLLBAR_FLAG (GFX_FLAG::GFX_NONE)
#define UI_SCROLLBAR_EVENT (GFX_EVENT::GFX_MOUSE_EVENT)

class UIScrollBar : public UIBase
{
public:
	enum ORIENTATION
	{
		ORI_VERTICAL,
		ORI_HORIZONTAL,
	};

	enum
	{
		UI_DISABLED,
		UI_NORMAL,
		UI_OVER,
		UI_PRESSED,
		UI_MAX,
	};

public:
	UIScrollBar(mu_uint32 objectId, UISceneManager *manager, UIBase *owner, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, ORIENTATION orientation, mu_uint32 flag = UI_SCROLLBAR_FLAG, mu_uint32 event = UI_SCROLLBAR_EVENT);
	~UIScrollBar();

	virtual void SystemConfigure() override;

	virtual void Draw() override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;
	virtual mu_boolean IsInside(mu_int32 mouseX, mu_int32 mouseY) override;

	virtual void OnMouseMove(mu_int32 mouse_x, mu_int32 mouse_y) override;
	virtual void OnMouseOver(mu_int32 mouse_x, mu_int32 mouse_y) override;
	virtual void OnMouseLeave() override;
	virtual void OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys) override;
	virtual void OnMouseReleased(mu_uint32 mouse_flags, mu_uint32 virtual_keys) override;
	virtual void OnMouseCancelled(mu_uint32 mouse_flags) override;

	void ConfigureTop(const MUTexture *texture, const EBlendState blendState, const mu_float width, const mu_float height);
	void ConfigureLeft(const MUTexture *texture, const EBlendState blendState, const mu_float width, const mu_float height);
	
	void ConfigureMiddle(const MUTexture *texture, const EBlendState blendState, const mu_float width, const mu_float height);
	void ConfigureScroll(const EBlendState blendState, const mu_float width, const mu_float height, mu_boolean inverse = false);

	void ConfigureBottom(const MUTexture *texture, const EBlendState blendState, const mu_float width, const mu_float height);
	void ConfigureRight(const MUTexture *texture, const EBlendState blendState, const mu_float width, const mu_float height);

	void ConfigureScrollInfo(mu_float minValue, mu_float maxValue, mu_float currentValue);
	void ConfigureScrollDimension(mu_float barSize);

	void MoveBar();
	void ApplyChanges();

	void SetStateData(const mu_uint32 index, const UI_STATE_DATA &state);
	void ChangeState(mu_uint32 state);
	mu_uint32 GetState();
	void SetTooltip(MUTextSimpleRender *fontIndex, const mu_text *message, mu_uint32 messageColor, mu_uint32 backgroundColor, mu_boolean isTop);

public:
	EINLINE mu_float GetMinValue()
	{
		return _MinValue;
	}

	EINLINE mu_float GetMaxValue()
	{
		return _MaxValue;
	}

	EINLINE mu_float GetCurrentValue()
	{
		return _CurrentValue;
	}

	EINLINE void ChangePosition(mu_float x, mu_float y)
	{
		UIBase::SetPosition(x, y);
		this->ApplyChanges();
	}

private:
	// Sprites
	UISprite *_Top;
	UISprite *_Middle;
	UISprite *_Bottom;
	UISprite *_Bar;

public:
	// Internal Data
	ORIENTATION _Orientation;
	mu_boolean _InverseScroll;
	mu_float _BarMinPos, _BarMaxPos;
	mu_float _BarSize;
	mu_float _MinValue, _MaxValue;
	mu_float _DiffValue;
	mu_float _CurrentValue;
	mu_float _RelativePos;

	UI_STATE_DATA _States[UI_MAX];
	mu_uint32 _CurrentState;

	// Tooltip
	mu_boolean _TooltipTop;
	mu_uint32 _TooltipMessageIndex;
	mu_uint32 _TooltipMessageColor;
	mu_uint32 _TooltipMessageBackColor;
	mu_string _TooltipMessage;
	MUTextSimpleRender *_TooltipFont;
};

#endif