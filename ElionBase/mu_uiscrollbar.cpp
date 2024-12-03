#include "stdafx.h"
#include "mu_uiscrollbar.h"

UIScrollBar::UIScrollBar(mu_uint32 objectId, UISceneManager *manager, UIBase *owner, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, ORIENTATION orientation, mu_uint32 flag, mu_uint32 event) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_ObjectID = objectId;
	_SceneManager = manager;

	UIBase::SetAspectRatio(aspectWidth, aspectHeight);
	UIBase::SetPosition(x, y);
	UIBase::SetParent(owner);

	SetGfxFlag(flag);
	SetEvent(event);

	mu_zeromem(_States, sizeof(_States));
	_CurrentState = UI_NORMAL;

	_Orientation = orientation;

	// Tooltip
	_TooltipMessageIndex = g_RandomTextIndex.fetch_add(1);
	_TooltipFont = FONT_NORMAL;
	_TooltipMessage = _T("");
	_TooltipMessageColor = MU::MakeARGB(255, 255, 255, 255);
	_TooltipMessageBackColor = MU::MakeARGB(0, 0, 0, 180);
	_TooltipTop = true;

	_Top = new UISprite(manager, this, nullptr, aspectWidth, aspectHeight, x, y, 0.0f, 0.0f);
	_Middle = new UISprite(manager, this, nullptr, aspectWidth, aspectHeight, x, y, 0.0f, 0.0f);
	_Bottom = new UISprite(manager, this, nullptr, aspectWidth, aspectHeight, x, y, 0.0f, 0.0f);
	_Bar = new UISprite(manager, this, nullptr, aspectWidth, aspectHeight, x, y, 0.0f, 0.0f);
}

UIScrollBar::~UIScrollBar()
{
	
}

void UIScrollBar::SystemConfigure()
{
	mu_float barSize = _Orientation == ORI_VERTICAL ?
		MU_TO_VIEWPORT(_BarSize + _Bar->_Height, _AspectHeight) :
		MU_TO_VIEWPORT(_BarSize + _Bar->_Width, _AspectWidth);

	UIBase::SystemConfigure();

	ConfigureScrollDimension(barSize);
}

void UIScrollBar::Draw()
{
	if (IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == true)
	{
		return;
	}

	if (_UpdateGPU == true)
	{
		_UpdateGPU = false;

		UI_STATE_DATA &CurrentState = _States[_CurrentState];

		_Bar->SetTexture(CurrentState.Texture);
		_Bar->SetColor(CurrentState.Color);
		_Bar->SetUV(CurrentState.UVBegin, CurrentState.UVEnd);
	}

	_Top->Draw();
	_Middle->Draw();
	_Bottom->Draw();
	_Bar->Draw();
}

UIBase *UIScrollBar::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
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

mu_boolean UIScrollBar::IsInside(mu_int32 mouseX, mu_int32 mouseY)
{
	if (_Bar->IsInside(mouseX, mouseY) == true)
	{
		return true;
	}
	else if (_Middle->IsInside(mouseX, mouseY) == true)
	{
		return true;
	}
	else if (_Top->IsInside(mouseX, mouseY) == true)
	{
		return true;
	}
	else if (_Bottom->IsInside(mouseX, mouseY) == true)
	{
		return true;
	}

	return false;
}

void UIScrollBar::OnMouseMove(mu_int32 mouse_x, mu_int32 mouse_y)
{
	if (_CurrentState == UI_PRESSED)
	{
		if (_Orientation == ORI_VERTICAL)
		{
			mu_float diffPos = (mouse_y - _Bar->_NativeY) - _RelativePos;

			if (diffPos != 0.0f)
			{
				mu_float diff = EMath::Abs(_MinValue - _MaxValue);
				mu_float oldValue = _CurrentValue;
				mu_float oldY = _Bar->_NativeY;
				mu_float newY = _Bar->_NativeY + diffPos;

				if (newY < _BarMinPos)
				{
					newY = _BarMinPos;
				}
				else if (newY > _BarMaxPos)
				{
					newY = _BarMaxPos;
				}

				_Bar->SetPosition(_Bar->_NativeX, newY, false);

				mu_float diff2;
				if (newY <= _BarMinPos)
				{
					diff2 = 0.0f;
				}
				else if (newY >= _BarMaxPos)
				{
					diff2 = 1.0f;
				}
				else
				{
					diff2 = (newY - _BarMinPos) / _BarSize;
				}

				_CurrentValue = _MinValue + (diff * diff2);

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
		else
		{
			mu_float diffPos = (mouse_x - _Bar->_NativeX) - _RelativePos;

			if (diffPos != 0.0f)
			{
				mu_float diff = EMath::Abs(_MinValue - _MaxValue);
				mu_float oldValue = _CurrentValue;
				mu_float oldX = _Bar->_NativeX;
				mu_float newX = _Bar->_NativeX + diffPos;

				if (newX < _BarMinPos)
				{
					newX = _BarMinPos;
				}
				else if (newX > _BarMaxPos)
				{
					newX = _BarMaxPos;
				}

				_Bar->SetPosition(newX, _Bar->_NativeY, false);
				
				mu_float diff2;
				if (newX <= _BarMinPos)
				{
					diff2 = 0.0f;
				}
				else if (newX >= _BarMaxPos)
				{
					diff2 = 1.0f;
				}
				else
				{
					diff2 = (newX - _BarMinPos) / _BarSize;
				}

				_CurrentValue = _MinValue + (diff * diff2);

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
	}
	else if (_CurrentState != UI_DISABLED)
	{
		if (_Bar->IsInside(mouse_x, mouse_y) == true)
		{
			ChangeState(UI_OVER);
		}
	}
}

void UIScrollBar::OnMouseOver(mu_int32 mouse_x, mu_int32 mouse_y)
{
	if (!IsMouseFlag(MOUSE_FLAG_LEFT) &&
		_CurrentState != UI_DISABLED)
	{
		if (_Bar->IsInside(mouse_x, mouse_y) == true)
		{
			ChangeState(UI_OVER);
		}
		else if (_CurrentState == UI_OVER)
		{
			ChangeState(UI_NORMAL);
		}
	}
}

void UIScrollBar::OnMouseLeave()
{
	if (_CurrentState == UI_OVER)
	{
		ChangeState(UI_NORMAL);
	}
}

void UIScrollBar::OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys)
{
	if (_CurrentState == UI_DISABLED)
	{
		return;
	}

	if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_LEFT))
	{
		mu_float mouseX = MU_INPUT->GetMouseX();
		mu_float mouseY = MU_INPUT->GetMouseY();

		if (_CurrentState == UI_OVER &&
			_Bar->IsInside(mouseX, mouseY) == true)
		{
			ChangeState(UI_PRESSED);

			if (_Orientation == ORI_VERTICAL)
			{
				_RelativePos = mouseY - _Bar->_NativeY;
			}
			else
			{
				_RelativePos = mouseX - _Bar->_NativeX;
			}
		}
		else
		{
			if (_Top->IsInside(mouseX, mouseY) == true)
			{
				ConfigureScrollInfo(_MinValue, _MaxValue, _MinValue);
			}
			else if (_Bottom->IsInside(mouseX, mouseY) == true)
			{
				ConfigureScrollInfo(_MinValue, _MaxValue, _MaxValue);
			}
			else if (_Middle->IsInside(mouseX, mouseY) == true)
			{
				if (_Orientation == ORI_VERTICAL)
				{
					mu_float oldY = _Bar->_NativeY;
					mu_float newY = mouseY - _Bar->_Height * 0.5f;

					if (newY < _BarMinPos)
					{
						newY = _BarMinPos;
					}
					else if (newY > _BarMaxPos)
					{
						newY = _BarMaxPos;
					}

					mu_float diff = EMath::Abs(_MinValue - _MaxValue);
					mu_float oldValue = _CurrentValue;
					mu_float diffY = mouseY - _Middle->_NativeY;
					mu_float perc = diff * (diffY / _BarSize);

					_Bar->SetPosition(_Bar->_NativeX, newY, false);

					mu_float diff2;
					if (newY <= _BarMinPos)
					{
						diff2 = 0.0f;
					}
					else if (newY >= _BarMaxPos)
					{
						diff2 = 1.0f;
					}
					else
					{
						diff2 = (newY - _BarMinPos) / _BarSize;
					}

					_CurrentValue = _MinValue + (diff * diff2);

					if (_CurrentValue != oldValue)
					{
						if (_MessageInheritor == nullptr ||
							_MessageInheritor->OnGFxMessage(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_SLIDE, 0) == false)
						{
							_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_SLIDE, 0);
						}
					}
				}
				else
				{
					mu_float oldX = _Bar->_NativeX;
					mu_float newX = mouseX - _Bar->_Width * 0.5f;

					if (newX < _BarMinPos)
					{
						newX = _BarMinPos;
					}
					else if (newX > _BarMaxPos)
					{
						newX = _BarMaxPos;
					}

					mu_float diff = EMath::Abs(_MinValue - _MaxValue);
					mu_float oldValue = _CurrentValue;
					mu_float diffX = mouseX - _Middle->_NativeX;
					mu_float perc = diff * (diffX / _BarSize);

					_Bar->SetPosition(newX, _Bar->_NativeY, false);

					mu_float diff2;
					if (newX <= _BarMinPos)
					{
						diff2 = 0.0f;
					}
					else if (newX >= _BarMaxPos)
					{
						diff2 = 1.0f;
					}
					else
					{
						diff2 = (newX - _BarMinPos) / _BarSize;
					}

					_CurrentValue = _MinValue + (diff * diff2);

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
		}
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

void UIScrollBar::OnMouseReleased(mu_uint32 mouse_flags, mu_uint32 virtual_keys)
{
	if (_CurrentState == UI_DISABLED)
	{
		return;
	}

	if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_LEFT))
	{
		if (_CurrentState == UI_PRESSED)
		{
			if (_Bar->IsInside(MU_INPUT->GetMouseX(), MU_INPUT->GetMouseY()) == true)
			{
				ChangeState(UI_OVER);
			}
			else
			{
				ChangeState(UI_NORMAL);
			}
		}
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

void UIScrollBar::OnMouseCancelled(mu_uint32 mouse_flags)
{
	if (_CurrentState == UI_DISABLED)
	{
		return;
	}

	if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_LEFT))
	{
		ChangeState(UI_NORMAL);
	}

	if (_SceneManager)
	{
		if (_MessageInheritor == nullptr ||
			_MessageInheritor->OnGFxMessage(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_MOUSE_CANCELLED, (mu_size)mouse_flags) == false)
		{
			_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_MOUSE_CANCELLED, (mu_size)mouse_flags);
		}
	}
}

void UIScrollBar::ConfigureTop(const MUTexture *texture, const EBlendState blendState, const mu_float width, const mu_float height)
{
	_Top->SetTexture(texture);
	_Top->SetBlendState(blendState);
	_Top->SetDimensions(width, height);
}

void UIScrollBar::ConfigureLeft(const MUTexture *texture, const EBlendState blendState, const mu_float width, const mu_float height)
{
	ConfigureTop(texture, blendState, width, height);
}

void UIScrollBar::ConfigureMiddle(const MUTexture *texture, const EBlendState blendState, const mu_float width, const mu_float height)
{
	_Middle->SetTexture(texture);
	_Middle->SetBlendState(blendState);
	_Middle->SetDimensions(width, height);
}

void UIScrollBar::ConfigureScroll(const EBlendState blendState, const mu_float width, const mu_float height, mu_boolean inverse)
{
	_Bar->SetBlendState(blendState);
	_Bar->SetDimensions(width, height);
	_InverseScroll = inverse;
}

void UIScrollBar::ConfigureBottom(const MUTexture *texture, const EBlendState blendState, const mu_float width, const mu_float height)
{
	_Bottom->SetTexture(texture);
	_Bottom->SetBlendState(blendState);
	_Bottom->SetDimensions(width, height);
}

void UIScrollBar::ConfigureRight(const MUTexture *texture, const EBlendState blendState, const mu_float width, const mu_float height)
{
	ConfigureBottom(texture, blendState, width, height);
}

void UIScrollBar::ConfigureScrollInfo(mu_float minValue, mu_float maxValue, mu_float currentValue)
{
	_UpdateGPU = true;

	if (currentValue < minValue) currentValue = minValue;
	else if (currentValue > maxValue) currentValue = maxValue;

	if (minValue > maxValue)
	{
		mu_float tmpValue = maxValue;
		maxValue = minValue;
		minValue = tmpValue;
	}

	_MinValue = minValue;
	_MaxValue = maxValue;
	_DiffValue = _MaxValue - _MinValue;
	_CurrentValue = currentValue;

	mu_float diff = EMath::Abs(_MinValue - _MaxValue);
	mu_float perc;

	if (_InverseScroll == true)
	{
		perc = 1.0f - (EMath::Abs(_CurrentValue - _MaxValue) / diff);
	}
	else
	{
		perc = EMath::Abs(_CurrentValue - _MaxValue) / diff;
	}

	if(_Orientation == ORI_VERTICAL)
	{
		mu_float diffPos = _BarSize * perc;

		_Bar->SetPosition(_Bar->_NativeX, _BarMinPos + diffPos, false);
	}
	else
	{
		mu_float diffPos = _BarSize * perc;

		_Bar->SetPosition(_BarMinPos + diffPos, _Bar->_NativeY, false);
	}
}

void UIScrollBar::ConfigureScrollDimension(mu_float barSize)
{
	if (_Orientation == ORI_VERTICAL)
	{
		if (barSize < _Bar->_ViewHeight)
		{
			barSize = _Bar->_ViewHeight;
		}

		barSize = MU_TO_WINDOW(barSize, _AspectHeight);
		barSize -= _Bar->_Height;

		_BarMinPos = _Top->_NativeY + _Top->_Height;
		_BarMaxPos = _BarMinPos + barSize;
		_BarSize = barSize;
	}
	else
	{
		if (barSize < _Bar->_ViewWidth)
		{
			barSize = _Bar->_ViewWidth;
		}

		barSize = MU_TO_WINDOW(barSize, _AspectWidth);
		barSize -= _Bar->_Width;

		_BarMinPos = _Top->_NativeX + _Top->_Width;
		_BarMaxPos = _BarMinPos + barSize;
		_BarSize = barSize;
	}

	this->MoveBar();
}

void UIScrollBar::MoveBar()
{
	mu_float diff = EMath::Abs(_MinValue - _MaxValue);
	mu_float perc;

	if (_InverseScroll == true)
	{
		perc = 1.0f - (EMath::Abs(_CurrentValue - _MaxValue) / diff);
	}
	else
	{
		perc = EMath::Abs(_CurrentValue - _MaxValue) / diff;
	}

	if (_Orientation == ORI_VERTICAL)
	{
		mu_float diffPos = _BarSize * perc;

		_Bar->SetPosition(_Bar->_NativeX, _BarMinPos + diffPos, false);
	}
	else
	{
		mu_float diffPos = _BarSize * perc;

		_Bar->SetPosition(_BarMinPos + diffPos, _Bar->_NativeY, false);
	}
}

void UIScrollBar::ApplyChanges()
{
	mu_float maxWidth = _Top->_ViewWidth;
	mu_float maxHeight = _Top->_ViewHeight;

	if (_Orientation == ORI_VERTICAL)
	{
		if (maxWidth < _Middle->_ViewWidth)
		{
			maxWidth = _Middle->_ViewWidth;
		}

		if (maxWidth < _Bottom->_ViewWidth)
		{
			maxWidth = _Bottom->_ViewWidth;
		}

		if (maxWidth < _Bar->_ViewWidth)
		{
			maxWidth = _Bar->_ViewWidth;
		}

		_Top->SetPosition(_ViewX + maxWidth * 0.5f - _Top->_ViewWidth * 0.5f, _ViewY);
		_Middle->SetPosition(_ViewX + maxWidth * 0.5f - _Middle->_ViewWidth * 0.5f, _Top->_ViewY + _Top->_ViewHeight);
		_Bottom->SetPosition(_ViewX + maxWidth * 0.5f - _Bottom->_ViewWidth * 0.5f, _Middle->_ViewY + _Middle->_ViewHeight);
	}
	else
	{
		if (maxHeight < _Middle->_ViewHeight)
		{
			maxHeight = _Middle->_ViewHeight;
		}

		if (maxHeight < _Bottom->_ViewHeight)
		{
			maxHeight = _Bottom->_ViewHeight;
		}

		if (maxHeight < _Bar->_ViewHeight)
		{
			maxHeight = _Bar->_ViewHeight;
		}

		_Top->SetPosition(_ViewX, _ViewY + maxHeight * 0.5f - _Top->_ViewHeight * 0.5f);
		_Middle->SetPosition(_Top->_ViewX + _Top->_ViewWidth, _ViewY + maxHeight * 0.5f - _Middle->_ViewHeight * 0.5f);
		_Bottom->SetPosition(_Middle->_ViewX + _Middle->_ViewWidth, _ViewY + maxHeight * 0.5f - _Bottom->_ViewHeight * 0.5f);
	}

	this->MoveBar();

	if (_Orientation == ORI_VERTICAL)
	{
		this->SetDimensions(maxWidth, _Top->_ViewHeight + _Middle->_ViewHeight + _Bottom->_ViewHeight);
	}
	else
	{
		this->SetDimensions(_Top->_ViewWidth + _Middle->_ViewWidth + _Bottom->_ViewWidth, maxHeight);
	}
}

void UIScrollBar::SetStateData(const mu_uint32 index, const UI_STATE_DATA &state)
{
	_UpdateGPU = true;
	mu_memcpy(&_States[index], &state, sizeof(state));
}

void UIScrollBar::ChangeState(mu_uint32 state)
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
	}
}

mu_uint32 UIScrollBar::GetState()
{
	return _CurrentState;
}

void UIScrollBar::SetTooltip(MUTextSimpleRender *fontIndex, const mu_text *message, mu_uint32 messageColor, mu_uint32 backgroundColor, mu_boolean isTop)
{
	_TooltipFont = fontIndex;
	_TooltipMessage = message;
	_TooltipMessageColor = messageColor;
	_TooltipMessageBackColor = backgroundColor;
	_TooltipTop = isTop;
}