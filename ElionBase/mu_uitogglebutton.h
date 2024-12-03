#ifndef __MU_UI_TOGGLE_BUTTON_H__
#define __MU_UI_TOGGLE_BUTTON_H__

#pragma once

#define UI_TOGGLEBUTTON_FLAG (GFX_FLAG::GFX_NONE)
#define UI_TOGGLEBUTTON_EVENT (GFX_EVENT::GFX_MOUSE_EVENT | GFX_EVENT::GFX_MOUSE_DBLCLICK_AS_CLICK)

class UIToggleButton : public UIRenderTextureBase
{
public:
	UIToggleButton(mu_uint32 objectId, UISceneManager *manager, UIBase *owner, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_float width, mu_float height, mu_uint32 flag = UI_TOGGLEBUTTON_FLAG, mu_uint32 event = UI_TOGGLEBUTTON_EVENT);
	~UIToggleButton();

	virtual void Draw() override;
	virtual void PostDraw() override;

	void SetStateData(const mu_uint32 index, const UI_STATE_DATA &state);
	void SetStateText(const mu_uint32 index, MUTextSimpleRender *font = nullptr, const mu_text *text = _T(""), mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255), mu_uint32 backColor = MU::MakeARGB(0, 0, 0, 0), mu_float marginX = 0.0f, mu_float marginY = 0.0f);
	void ChangeState(mu_uint32 state);
	mu_uint32 GetState();
	mu_boolean IsPressed();
	void SetTooltip(MUTextSimpleRender *fontIndex, const mu_text *message, mu_uint32 messageColor, mu_uint32 backgroundColor, mu_boolean isTop);

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
		UI_PRESSED_OVER,
		UI_MAX,
	};

private:
	UI_STATE_DATA _States[UI_MAX];
	UI_STATE_TEXT _TextStates[UI_MAX];
	mu_uint32 _CurrentState;

	mu_uint32 _VertexIndex;

	// Text
	mu_uint32 _TextMessageIndex;

	// Tooltip
	mu_boolean _TooltipTop;
	mu_uint32 _TooltipMessageIndex;
	mu_uint32 _TooltipMessageColor;
	mu_uint32 _TooltipMessageBackColor;
	mu_string _TooltipMessage;
	MUTextSimpleRender *_TooltipFont;
};

#endif