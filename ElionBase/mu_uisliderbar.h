#ifndef __MU_UISLIDERBAR_H__
#define __MU_UISLIDERBAR_H__

#pragma once

#define UI_SLIDERBAR_FLAG (GFX_FLAG::GFX_NONE)
#define UI_SLIDERBAR_EVENT (GFX_EVENT::GFX_MOUSE_EVENT)

class UISliderBar : public UIBase
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
		UI_MAX,
	};

public:
	UISliderBar(mu_uint32 objectId, UISceneManager *manager, UIBase *owner, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_float width, mu_float height, ORIENTATION orientation, mu_uint32 flag = UI_SLIDERBAR_FLAG, mu_uint32 event = UI_SLIDERBAR_EVENT);
	~UISliderBar();

	virtual void Draw() override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	virtual void OnMouseMove(mu_int32 mouse_x, mu_int32 mouse_y) override;
	virtual void OnMouseOver(mu_int32 mouse_x, mu_int32 mouse_y) override;
	virtual void OnMouseLeave() override;
	virtual void OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys) override;
	virtual void OnMouseReleased(mu_uint32 mouse_flags, mu_uint32 virtual_keys) override;
	virtual void OnMouseCancelled(mu_uint32 mouse_flags) override;

	void SetStateData(const mu_uint32 index, const UI_STATE_DATA &state);
	void ChangeState(mu_uint32 state);
	mu_uint32 GetState();

	void ConfigureBack(MUTexture *Texture, const EVector2 &uvBegin, const EVector2 &uvEnd);
	void ConfigureFront(MUTexture *Texture, const EVector2 &uvBegin, const EVector2 &uvEnd);

	void SetLockUV(mu_boolean isLockUV);

	void SetCurrentValue(mu_float currentValue);

public:
	EINLINE mu_float GetCurrentValue()
	{
		return _CurrentValue;
	}

private:
	UISprite *_Back;
	UISprite *_Front;

private:
	ORIENTATION _Orientation;

	EVector2 _BeginUV;
	EVector2 _EndUV;
	mu_boolean _LockUV;

	mu_boolean _IsPressed;

	mu_float _CurrentValue;

	UI_STATE_DATA _States[UI_MAX];
	mu_uint32 _CurrentState;
};

#endif