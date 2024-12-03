#ifndef __MU_UI_OVERBUTTON_H__
#define __MU_UI_OVERBUTTON_H__

#pragma once

#define UI_OVERBUTTON_FLAG (GFX_FLAG::GFX_NONE)
#define UI_OVERBUTTON_EVENT (GFX_EVENT::GFX_MOUSE_EVENT | GFX_EVENT::GFX_MOUSE_DBLCLICK_AS_CLICK)

class UIOverButton : public UIRenderTextureBase
{
public:
	UIOverButton(mu_uint32 objectId, UISceneManager *manager, UIBase *owner, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_float width, mu_float height, mu_uint32 flag = UI_OVERBUTTON_FLAG, mu_uint32 event = UI_OVERBUTTON_EVENT);
	~UIOverButton();

	virtual void Draw() override;

	void SetStateData(const mu_uint32 index, const UI_STATE_DATA &state);
	void ChangeState(mu_uint32 state);

	virtual void OnMouseOver(mu_int32 mouse_x, mu_int32 mouse_y) override;
	virtual void OnMouseLeave() override;
	virtual void OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys) override;

public:
	EINLINE mu_uint32 GetState()
	{
		return _CurrentState;
	}

public:
	enum
	{
		UI_DISABLED,
		UI_NORMAL,
		UI_MAX,
	};

private:
	UI_STATE_DATA _States[UI_MAX];
	mu_uint32 _CurrentState;

	mu_uint32 _VertexIndex;
};

#endif