#include "stdafx.h"
#include "mu_uibutton.h"

UIButton::UIButton(mu_uint32 objectId, UISceneManager *manager, UIBase *owner, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_float width, mu_float height, mu_uint32 flag, mu_uint32 event) : UIRenderTextureBase()
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

	mu_zeromem(_States, sizeof(_States));
	_CurrentState = UI_NORMAL;

	SetGfxFlag(flag);
	SetEvent(event);

	// Device specific data
	_VertexIndex = EInvalidUInt32;
	
	// Text
	_TextMessageIndex = g_RandomTextIndex.fetch_add(1);

	// Tooltip
	_TooltipMessageIndex = g_RandomTextIndex.fetch_add(1);
	_TooltipFont = FONT_NORMAL;
	_TooltipMessage = _T("");
	_TooltipMessageColor = MU::MakeARGB(255, 255, 255, 255);
	_TooltipMessageBackColor = MU::MakeARGB(0, 0, 0, 180);
	_TooltipTop = true;
}

UIButton::~UIButton()
{
}

void UIButton::Draw()
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
		if (_States[_CurrentState].Texture == USE_COLOR_STATE ||
			_States[_CurrentState].Texture->IsLoaded() == true)
		{
			if (_States[_CurrentState].Texture == USE_COLOR_STATE)
			{
				EVideoPipeline::SetGraphicsPipelineAtRuntime(ECombinedShader::eUIColor, ERasterizer::eBackOnly, _DepthStencil, _BlendState, false);
			}
			else
			{
				EVideoPipeline::SetGraphicsPipelineAtRuntime(ECombinedShader::eUIDefault, ERasterizer::eBackOnly, _DepthStencil, _BlendState, false);
				EVideoPipeline::BindGraphicsUITextureAtRuntime({ _States[_CurrentState].Texture, _Sampler });
			}
			EVideoPipeline::BindGraphicsUIAtRuntime();

			EVideoPipeline::DrawAtRuntime(_VertexIndex, 4, ETopologyType::eTriangleStrip);
		}
		else
		{
			g_LoadingManager.AddTextureRuntime(_States[_CurrentState].Texture);
		}
	}

	UI_STATE_TEXT &textState = _TextStates[_CurrentState];

	if (textState.Font != nullptr &&
		textState.Text.empty() == false)
	{
		switch (textState.HorizontalAlign)
		{
		case ETA_LEFT:
			{
				textState.Font->DrawStringC2D(_TextMessageIndex,
					MU_TO_WINDOW(_ViewX + textState.MarginX, _AspectWidth),
					MU_TO_WINDOW(_ViewY + _ViewHeight * 0.5f + textState.MarginY, _AspectHeight),
					1.0f,
					ETA_LEFT,
					ETA_CENTER,
					2.0f,
					textState.TextColor,
					textState.BackColor,
					textState.Text.c_str());
			}
			break;

		case ETA_CENTER:
			{
				textState.Font->DrawStringC2D(_TextMessageIndex,
					MU_TO_WINDOW(_ViewX + _ViewWidth * 0.5f + textState.MarginX, _AspectWidth),
					MU_TO_WINDOW(_ViewY + _ViewHeight * 0.5f + textState.MarginY, _AspectHeight),
					1.0f,
					ETA_CENTER,
					ETA_CENTER,
					2.0f,
					textState.TextColor,
					textState.BackColor,
					textState.Text.c_str());
			}
			break;

		case ETA_RIGHT:
			{
				textState.Font->DrawStringC2D(_TextMessageIndex,
					MU_TO_WINDOW(_ViewX + _ViewWidth + textState.MarginX, _AspectWidth),
					MU_TO_WINDOW(_ViewY + _ViewHeight * 0.5f + textState.MarginY, _AspectHeight),
					1.0f,
					ETA_RIGHT,
					ETA_CENTER,
					2.0f,
					textState.TextColor,
					textState.BackColor,
					textState.Text.c_str());
			}
			break;
		}
	}
}

void UIButton::PostDraw()
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
			_TooltipFont->DrawStringC2D(_TooltipMessageIndex, _NativeX + _Width * 0.5f, _NativeY + _Height + 2, 1.0f, ETA_CENTER, ETA_BOTTOM, 2.0f, _TooltipMessageColor, _TooltipMessageBackColor, _TooltipMessage.c_str());
		}
	}
}

void UIButton::SetStateData(const mu_uint32 index, const UI_STATE_DATA &state)
{
	_UpdateGPU = true;

	mu_memcpy(&_States[index], &state, sizeof(state));
}

void UIButton::SetStateText(const mu_uint32 index, MUTextSimpleRender *font, const mu_text *text, mu_uint32 textColor, mu_uint32 backColor, mu_float marginX, mu_float marginY, mu_uint8 HorizontalAlign)
{
	_TextStates[index].Font = font;
	_TextStates[index].Text = text;
	_TextStates[index].TextColor = textColor;
	_TextStates[index].BackColor = backColor;
	_TextStates[index].MarginX = marginX;
	_TextStates[index].MarginY = marginY;
	_TextStates[index].HorizontalAlign = HorizontalAlign;
}

void UIButton::ChangeState(mu_uint32 state)
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

void UIButton::SetTooltip(MUTextSimpleRender *fontIndex, const mu_text *message, mu_uint32 messageColor, mu_uint32 backgroundColor, mu_boolean isTop)
{
	_TooltipFont = fontIndex;
	_TooltipMessage = message;
	_TooltipMessageColor = messageColor;
	_TooltipMessageBackColor = backgroundColor;
	_TooltipTop = isTop;
}

void UIButton::OnMouseOver(mu_int32 mouse_x, mu_int32 mouse_y)
{
	if (!IsMouseFlag(MOUSE_FLAG_LEFT) &&
		_CurrentState != UI_DISABLED)
	{
		ChangeState(UI_OVER);
	}
}

void UIButton::OnMouseLeave()
{
	if (_CurrentState == UI_OVER)
	{
		ChangeState(UI_NORMAL);
	}
}

void UIButton::OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys)
{
	if (_CurrentState == UI_DISABLED)
	{
		return;
	}

	if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_LEFT))
	{
		ChangeState(UI_PRESSED);
	}

	if (_SceneManager)
	{
		_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_MOUSE_PRESSED, (mu_size)mouse_flags);
	}
}

void UIButton::OnMouseReleased(mu_uint32 mouse_flags, mu_uint32 virtual_keys)
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
		
		if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_LEFT))
		{
			_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
		}
	}
}

void UIButton::OnMouseCancelled(mu_uint32 mouse_flags)
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