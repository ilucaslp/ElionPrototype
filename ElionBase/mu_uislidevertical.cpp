#include "stdafx.h"
#include "mu_uislidevertical.h"

UISlideVertical::UISlideVertical(mu_uint32 objectId, UISceneManager *manager, UIBase *owner, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float miny, mu_float maxy, mu_float width, mu_float height, mu_uint32 flag, mu_uint32 event) : UIRenderTextureBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_ObjectID = objectId;
	_SceneManager = manager;

	UIBase::SetAspectRatio(aspectWidth, aspectHeight);
	UIBase::SetPosition(x, miny);
	UIBase::SetDimensions(width, height);
	UIBase::SetParent(owner);

	_MinValue = 0.0f;
	_MaxValue = 1.0f;
	_CurrentValue = 0.0f;

	SetScrollDimension(miny, maxy);

	_MinY = MU_TO_WINDOW(miny, _AspectHeight);
	_MaxY = MU_TO_WINDOW(maxy, _AspectHeight);

	mu_zeromem(_States, sizeof(_States));
	_CurrentState = UI_NORMAL;

	SetGfxFlag(flag);
	SetEvent(event);

	// Device specific data
	_VertexIndex = EInvalidUInt32;

	// Tooltip
	_TooltipMessageIndex = g_RandomTextIndex.fetch_add(1);
	_TooltipFont = FONT_NORMAL;
	_TooltipMessage = _T("");
	_TooltipMessageColor = MU::MakeARGB(255, 255, 255, 255);
	_TooltipMessageBackColor = MU::MakeARGB(0, 0, 0, 180);
	_TooltipTop = true;
}

UISlideVertical::~UISlideVertical()
{
}

void UISlideVertical::Draw()
{
	if (IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == true)
	{
		return;
	}

	const mu_boolean updateReady = _VertexIndex == EInvalidUInt32;
	if (updateReady == true)
	{
		_VertexIndex = EVideoBackend::UIAllocateDataIndex(4);
	}

	if (_VertexIndex != EInvalidUInt32 &&
		(_UpdateGPU == true || updateReady == true))
	{
		SHUIDefaultVertex *vertices = EVideoBackend::UIRetrieveDataBuffer(_VertexIndex);

		EVector2 &UVBegin = _States[_CurrentState].UVBegin;
		EVector2 &UVEnd = _States[_CurrentState].UVEnd;
		mu_uint32 &Color = _States[_CurrentState].Color;

		const mu_float tmpX = _NativeX, tmpY = _NativeY;

		// Triangle Strip method (Order: LT, RT, BL, BR)
		vertices[0].Position = EVector2(tmpX, tmpY);
		vertices[1].Position = EVector2(tmpX + _Width, tmpY);
		vertices[2].Position = EVector2(tmpX, tmpY + _Height);
		vertices[3].Position = EVector2(tmpX + _Width, tmpY + _Height);

		vertices[0].UV = UVBegin;
		vertices[1].UV = EVector2(UVEnd.x, UVBegin.y);
		vertices[2].UV = EVector2(UVBegin.x, UVEnd.y);
		vertices[3].UV = UVEnd;

		vertices[0].Color = Color;
		vertices[1].Color = Color;
		vertices[2].Color = Color;
		vertices[3].Color = Color;

		EVideoBackend::UIRequestUpdateData(_VertexIndex, 4);

		_UpdateGPU = false;
	}

	if (_VertexIndex != EInvalidUInt32 &&
		_States[_CurrentState].Texture != nullptr)
	{
		if (_States[_CurrentState].Texture->IsLoaded() == true)
		{
			EVideoPipeline::SetGraphicsPipelineAtRuntime(ECombinedShader::eUIDefault, ERasterizer::eBackOnly, _DepthStencil, _BlendState, false);
			EVideoPipeline::BindGraphicsUITextureAtRuntime({ _States[_CurrentState].Texture, _Sampler });
			EVideoPipeline::BindGraphicsUIAtRuntime();

			EVideoPipeline::DrawAtRuntime(_VertexIndex, 4, ETopologyType::eTriangleStrip);
		}
		else
		{
			g_LoadingManager.AddTextureRuntime(_States[_CurrentState].Texture);
		}
	}
}

void UISlideVertical::PostDraw()
{
	if (_CurrentState == UI_OVER &&
		_TooltipMessage.empty() == false)
	{
		if (_TooltipTop == true)
		{
			_TooltipFont->DrawStringC2D(_TooltipMessageIndex, _NativeX + _Width * 0.5f, _NativeY - 2, 1.0f, ETA_CENTER, ETA_TOP, 2.0f, _TooltipMessageColor, _TooltipMessageBackColor, _TooltipMessage.c_str());
		}
		else
		{
			_TooltipFont->DrawStringC2D(_TooltipMessageIndex, _NativeX + _Width * 0.5f, _NativeY + _Height + 2, 1.0f, ETA_CENTER, ETA_TOP, 2.0f, _TooltipMessageColor, _TooltipMessageBackColor, _TooltipMessage.c_str());
		}
	}
}

void UISlideVertical::SetStateData(const mu_uint32 index, const UI_STATE_DATA &state)
{
	_UpdateGPU = true;
	mu_memcpy(&_States[index], &state, sizeof(state));
}

void UISlideVertical::ChangeState(mu_uint32 state)
{
	_UpdateGPU = true;
	_CurrentState = state;

	if (_MouseFlag)
	{
		if (_SceneManager)
		{
			_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_MOUSE_CANCELLED, 0);
		}

		_MouseFlag = 0;
	}
}

mu_uint32 UISlideVertical::GetState()
{
	return _CurrentState;
}

void UISlideVertical::SetTooltip(MUTextSimpleRender *fontIndex, const mu_text *message, mu_uint32 messageColor, mu_uint32 backgroundColor, mu_boolean isTop)
{
	_TooltipFont = fontIndex;
	_TooltipMessage = message;
	_TooltipMessageColor = messageColor;
	_TooltipMessageBackColor = backgroundColor;
	_TooltipTop = isTop;
}

void UISlideVertical::SetScrollInfo(mu_float minValue, mu_float maxValue, mu_float currentValue)
{
	_UpdateGPU = true;

	_MinValue = minValue;
	_MaxValue = maxValue;
	_CurrentValue = currentValue;

	mu_float diff = EMath::Abs(_MinValue - _MaxValue);
	mu_float perc = 1.0f - (EMath::Abs(_CurrentValue - _MaxValue) / diff);
	mu_float diffY = _DiffY * perc;

	_NativeY = _MinY + diffY;
}

void UISlideVertical::SetScrollDimension(mu_float minY, mu_float maxY)
{
	_UpdateGPU = true;

	_MinY = MU_TO_WINDOW(minY, _AspectHeight);
	_MaxY = MU_TO_WINDOW(maxY, _AspectHeight);
	_DiffY = _MaxY - _MinY;

	mu_float diff = EMath::Abs(_MinValue - _MaxValue);
	mu_float perc = 1.0f - (EMath::Abs(_CurrentValue - _MaxValue) / diff);
	mu_float diffY = _DiffY * perc;

	_NativeY = _MinY + diffY;
}

mu_float UISlideVertical::GetCurrentValue()
{
	return _CurrentValue;
}

void UISlideVertical::OnMouseMove(mu_int32 mouse_x, mu_int32 mouse_y)
{
	if (_CurrentState == UI_PRESSED)
	{
		mu_float diffY = (mouse_y - _NativeY) - _RelativeY;

		if (diffY != 0.0f)
		{
			mu_float diff = EMath::Abs(_MinValue - _MaxValue);
			mu_float oldValue = _CurrentValue;
			mu_float oldY = _NativeY;

			_UpdateGPU = true;

			_NativeY += diffY;

			if (_NativeY < _MinY)
			{
				_NativeY = _MinY;
			}
			else if (_NativeY > _MaxY)
			{
				_NativeY = _MaxY;
			}

			mu_float diff2 = (_NativeY - _MinY) / _DiffY;

			_CurrentValue = _MinValue + (diff * diff2);

			if (_CurrentValue != oldValue)
			{
				_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_SLIDE, 0);
			}
		}
	}
}

void UISlideVertical::OnMouseOver(mu_int32 mouse_x, mu_int32 mouse_y)
{
	if (!IsMouseFlag(MOUSE_FLAG_LEFT) &&
		_CurrentState != UI_DISABLED)
	{
		ChangeState(UI_OVER);
	}
}

void UISlideVertical::OnMouseLeave()
{
	if (_CurrentState == UI_OVER)
	{
		ChangeState(UI_NORMAL);
	}
}

void UISlideVertical::OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys)
{
	if (_CurrentState == UI_DISABLED)
	{
		return;
	}

	if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_LEFT))
	{
		ChangeState(UI_PRESSED);
		_RelativeY = MU_INPUT->GetMouseY() - _NativeY;
	}

	if (_SceneManager)
	{
		_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_MOUSE_PRESSED, (mu_size)mouse_flags);
	}
}

void UISlideVertical::OnMouseReleased(mu_uint32 mouse_flags, mu_uint32 virtual_keys)
{
	if (_CurrentState == UI_DISABLED)
	{
		return;
	}

	if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_LEFT))
	{
		ChangeState(UI_OVER);
	}

	if (_SceneManager)
	{
		_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_MOUSE_RELEASED, (mu_size)mouse_flags);
	}
}

void UISlideVertical::OnMouseCancelled(mu_uint32 mouse_flags)
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
		_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_MOUSE_CANCELLED, (mu_size)mouse_flags);
	}
}