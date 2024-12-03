#ifndef __MU_UISLIDE_H__
#define __MU_UISLIDE_H__

#pragma once

#define UI_SLIDE_FLAG (GFX_FLAG::GFX_NONE)
#define UI_SLIDE_EVENT (GFX_EVENT::GFX_MOUSE_EVENT)

class UISlideVertical : public UIRenderTextureBase
{
public:
	UISlideVertical(mu_uint32 objectId, UISceneManager *manager, UIBase *owner, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float miny, mu_float maxy, mu_float width, mu_float height, mu_uint32 flag = UI_SLIDE_FLAG, mu_uint32 event = UI_SLIDE_EVENT);
	~UISlideVertical();

	virtual void Draw() override;
	virtual void PostDraw() override;

	void SetStateData(const mu_uint32 index, const UI_STATE_DATA &state);
	void ChangeState(mu_uint32 state);
	mu_uint32 GetState();
	void SetTooltip(MUTextSimpleRender *fontIndex, const mu_text *message, mu_uint32 messageColor, mu_uint32 backgroundColor, mu_boolean isTop);
	void SetScrollInfo(mu_float minValue, mu_float maxValue, mu_float currentValue);
	void SetScrollDimension(mu_float minY, mu_float maxY);
	mu_float GetCurrentValue();

	virtual void OnMouseMove(mu_int32 mouse_x, mu_int32 mouse_y) override;
	virtual void OnMouseOver(mu_int32 mouse_x, mu_int32 mouse_y) override;
	virtual void OnMouseLeave() override;
	virtual void OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys) override;
	virtual void OnMouseReleased(mu_uint32 mouse_flags, mu_uint32 virtual_keys) override;
	virtual void OnMouseCancelled(mu_uint32 mouse_flags) override;

public:
	enum
	{
		UI_DISABLED,
		UI_NORMAL,
		UI_OVER,
		UI_PRESSED,
		UI_MAX,
	};

private:
	UI_STATE_DATA _States[UI_MAX];
	mu_uint32 _CurrentState;
	mu_float _MinY, _MaxY;
	mu_float _DiffY;
	mu_float _MinValue, _MaxValue;
	mu_float _CurrentValue;
	mu_float _RelativeY;

	mu_uint32 _VertexIndex;

	// Tooltip
	mu_boolean _TooltipTop;
	mu_uint32 _TooltipMessageIndex;
	mu_uint32 _TooltipMessageColor;
	mu_uint32 _TooltipMessageBackColor;
	mu_string _TooltipMessage;
	MUTextSimpleRender *_TooltipFont;
};

#endif