#include "stdafx.h"
#include "mu_uisliderbar.h"

UISliderBar::UISliderBar(mu_uint32 objectId, UISceneManager *manager, UIBase *owner, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_float width, mu_float height, ORIENTATION orientation, mu_uint32 flag, mu_uint32 event) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_ObjectID = objectId;
	_SceneManager = manager;

	UIBase::SetAspectRatio(aspectWidth, aspectHeight);
	UIBase::SetPosition(x, y);
	UIBase::SetDimensions(width, height);
	UIBase::SetParent(owner);

	SetGfxFlag(flag);
	SetEvent(event);

	mu_zeromem(_States, sizeof(_States));
	_CurrentState = UI_NORMAL;

	_Orientation = orientation;

	_Back = new UISprite(manager, this, nullptr, aspectWidth, aspectHeight, x, y, width, height);
	_Front = new UISprite(manager, this, nullptr, aspectWidth, aspectHeight, x, y, 0.0f, 0.0f);

	_CurrentValue = 0.0f;
	_LockUV = false;
}

UISliderBar::~UISliderBar()
{

}

void UISliderBar::Draw()
{
	if (IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == true)
	{
		return;
	}

	if (_UpdateGPU == true)
	{
		_UpdateGPU = false;

		UI_STATE_DATA &CurrentState = _States[_CurrentState];

		ConfigureFront(CurrentState.Texture, CurrentState.UVBegin, CurrentState.UVEnd);

		_Front->SetColor(CurrentState.Color);
	}

	_Back->Draw();
	_Front->Draw();
}

UIBase *UISliderBar::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
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

void UISliderBar::OnMouseMove(mu_int32 mouse_x, mu_int32 mouse_y)
{
	if (_IsPressed == true)
	{
		mu_float oldValue = GetCurrentValue();

		if (_Orientation == ORI_VERTICAL)
		{
			if (mouse_y < _Back->_NativeY)
			{
				SetCurrentValue(0.0f);
			}
			else if (mouse_y > _Back->_NativeY + _Back->_Height)
			{
				SetCurrentValue(1.0f);
			}
			else
			{
				SetCurrentValue((mouse_y - _Back->_NativeY) / _Back->_Height);
			}
		}
		else
		{
			if (mouse_x < _Back->_NativeX)
			{
				SetCurrentValue(0.0f);
			}
			else if (mouse_x > _Back->_NativeX + _Back->_Width)
			{
				SetCurrentValue(1.0f);
			}
			else
			{
				SetCurrentValue((mouse_x - _Back->_NativeX) / _Back->_Width);
			}

			if (_CurrentValue != oldValue)
			{
				if (_MessageInheritor == nullptr ||
					_MessageInheritor->OnGFxMessage(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_SLIDE, 0) == false)
				{
					_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_SLIDE, 0);
				}
			}
		}

		if (_CurrentValue != oldValue)
		{
			if (_MessageInheritor == nullptr ||
				_MessageInheritor->OnGFxMessage(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_SLIDE, 0) == false)
			{
				_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_SLIDE, 0);
			}
		}
	}
}

void UISliderBar::OnMouseOver(mu_int32 mouse_x, mu_int32 mouse_y)
{
}

void UISliderBar::OnMouseLeave()
{
}

void UISliderBar::OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys)
{
	if (_CurrentState == UI_DISABLED)
	{
		return;
	}

	if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_LEFT))
	{
		mu_float mouseX = MU_INPUT->GetMouseX();
		mu_float mouseY = MU_INPUT->GetMouseY();

		_IsPressed = true;

		OnMouseMove(mouseX, mouseY);
	}

	if (_SceneManager)
	{
		if (_MessageInheritor == nullptr ||
			_MessageInheritor->OnGFxMessage(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_MOUSE_PRESSED, (mu_size)mouse_flags) == false)
		{
			_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_MOUSE_PRESSED, (mu_size)mouse_flags);
		}
	}
}

void UISliderBar::OnMouseReleased(mu_uint32 mouse_flags, mu_uint32 virtual_keys)
{
	if (_CurrentState == UI_DISABLED)
	{
		return;
	}

	if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_LEFT))
	{
		_IsPressed = false;
	}

	if (_SceneManager)
	{
		if (_MessageInheritor == nullptr ||
			_MessageInheritor->OnGFxMessage(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_MOUSE_RELEASED, (mu_size)mouse_flags) == false)
		{
			_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_MOUSE_RELEASED, (mu_size)mouse_flags);
		}
	}
}

void UISliderBar::OnMouseCancelled(mu_uint32 mouse_flags)
{
	if (_CurrentState == UI_DISABLED)
	{
		return;
	}

	_IsPressed = false;

	if (_SceneManager)
	{
		if (_MessageInheritor == nullptr ||
			_MessageInheritor->OnGFxMessage(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_MOUSE_CANCELLED, (mu_size)mouse_flags) == false)
		{
			_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_MOUSE_CANCELLED, (mu_size)mouse_flags);
		}
	}
}

void UISliderBar::SetStateData(const mu_uint32 index, const UI_STATE_DATA &state)
{
	_UpdateGPU = true;
	mu_memcpy(&_States[index], &state, sizeof(state));
}

void UISliderBar::ChangeState(mu_uint32 state)
{
	_UpdateGPU = true;
	_CurrentState = state;

	if (_MouseFlag)
	{
		if (_SceneManager)
		{
			if (_MessageInheritor == nullptr ||
				_MessageInheritor->OnGFxMessage(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_MOUSE_CANCELLED, 0) == false)
			{
				_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_MOUSE_CANCELLED, 0);
			}
		}

		_MouseFlag = 0;
		_IsPressed = false;
	}
}

mu_uint32 UISliderBar::GetState()
{
	return _CurrentState;
}

void UISliderBar::ConfigureBack(MUTexture *Texture, const EVector2 &uvBegin, const EVector2 &uvEnd)
{
	_Back->SetTexture(Texture);
	_Back->SetUV(uvBegin, uvEnd);
}

void UISliderBar::ConfigureFront(MUTexture *Texture, const EVector2 &uvBegin, const EVector2 &uvEnd)
{
	_Front->SetTexture(Texture);
	_Front->SetUV(uvBegin, uvEnd);
	_BeginUV = uvBegin, _EndUV = uvEnd;

	SetCurrentValue(_CurrentValue);
}

void UISliderBar::SetLockUV(mu_boolean isLockUV)
{
	_Front->SetUV(_BeginUV, _EndUV);
	_LockUV = isLockUV;

	SetCurrentValue(_CurrentValue);
}

void UISliderBar::SetCurrentValue(mu_float currentValue)
{
	if (currentValue < 0.0f) currentValue = 0.0f;
	else if (currentValue > 1.0f) currentValue = 1.0f;

	if (_Orientation == ORI_HORIZONTAL)
	{
		_Front->SetDimensions(_Back->_ViewWidth * currentValue, _Back->_ViewHeight);
	}
	else
	{
		_Front->SetDimensions(_Back->_ViewWidth, _Back->_ViewHeight * currentValue);
	}

	if (_LockUV == false)
	{
		if (_Orientation == ORI_HORIZONTAL)
		{
			_Front->SetUV(_BeginUV, _BeginUV + (_EndUV - _BeginUV) * EVector2(currentValue, 1.0f));
		}
		else
		{
			_Front->SetUV(_BeginUV, _BeginUV + (_EndUV - _BeginUV) * EVector2(1.0f, currentValue));
		}
	}

	_CurrentValue = currentValue;
}