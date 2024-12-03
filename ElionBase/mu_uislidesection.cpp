#include "stdafx.h"
#include "mu_uislidesection.h"

UISlideSection::UISlideSection(mu_uint32 objectId, UISceneManager *manager, UIBase *owner, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_float width, mu_float height, mu_uint32 flag, mu_uint32 event) : UIRenderTextureBase()
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

	// Tooltip
	_TooltipMessageIndex = g_RandomTextIndex.fetch_add(1);
	_TooltipFont = FONT_NORMAL;
	_TooltipMessage = _T("");
	_TooltipMessageColor = MU::MakeARGB(255, 255, 255, 255);
	_TooltipMessageBackColor = MU::MakeARGB(0, 0, 0, 180);
	_TooltipTop = true;
}

UISlideSection::~UISlideSection()
{
}

void UISlideSection::Draw()
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

void UISlideSection::PostDraw()
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

void UISlideSection::SetStateData(const mu_uint32 index, const UI_STATE_DATA &state)
{
	_UpdateGPU = true;
	mu_memcpy(&_States[index], &state, sizeof(state));
}

void UISlideSection::ChangeState(mu_uint32 state)
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

void UISlideSection::SetTooltip(MUTextSimpleRender *fontIndex, const mu_text *message, mu_uint32 messageColor, mu_uint32 backgroundColor, mu_boolean isTop)
{
	_TooltipFont = fontIndex;
	_TooltipMessage = message;
	_TooltipMessageColor = messageColor;
	_TooltipMessageBackColor = backgroundColor;
	_TooltipTop = isTop;
}

void UISlideSection::SetSectionInfo(mu_uint32 totalSection, mu_uint32 currentSection, mu_float sectionHeight, mu_float extraHeight)
{
	_SectionCount = totalSection;
	_CurrentSection = currentSection;
	_SectionHeight = sectionHeight;
	_ExtraHeight = extraHeight;
}

mu_uint32 UISlideSection::GetCurrentSection()
{
	return _CurrentSection;
}

void UISlideSection::SetCurrentSection(mu_uint32 currentSection)
{
	if (currentSection < 1)
		currentSection = 1;
	else if (currentSection > _SectionCount)
		currentSection = _SectionCount;

	if (_CurrentSection == currentSection) return;

	mu_float sectionHeight = MU_TO_WINDOW(_SectionHeight, _AspectHeight);
	_UpdateGPU = true;

	if (currentSection < _CurrentSection)
	{
		mu_uint32 diffSection = _CurrentSection - currentSection;
		_NativeY = _NativeY + (mu_float)diffSection * sectionHeight;
	}
	else
	{
		mu_uint32 diffSection = currentSection - _CurrentSection;
		_NativeY = _NativeY - (mu_float)diffSection * sectionHeight;
	}

	_CurrentSection = currentSection;
}

void UISlideSection::OnMouseMove(mu_int32 mouse_x, mu_int32 mouse_y)
{
	if (_CurrentState == UI_PRESSED)
	{
		mu_float sectionHeight = MU_TO_WINDOW(_SectionHeight, _AspectHeight);
		mu_float extraHeight = MU_TO_WINDOW(_ExtraHeight, _AspectHeight);
		mu_uint32 oldSection = _CurrentSection;

		if (mouse_y < _NativeY)
		{
			if (_CurrentSection < _SectionCount)
			{
				mu_uint32 leftCount = _SectionCount - _CurrentSection;
				for (mu_uint32 i = 1; i <= leftCount - 1; ++i)
				{
					mu_float y = _NativeY - (mu_float)i * sectionHeight;
					if (CheckMouseInY(y, sectionHeight))
					{
						_UpdateGPU = true;
						_NativeY = y;
						_CurrentSection = _CurrentSection + i;
						break;
					}
				}

				if (_CurrentSection == oldSection)
				{
					mu_float y = _NativeY - (mu_float)leftCount * sectionHeight;
					if (CheckMouseInY(0.0f, y + sectionHeight))
					{
						_UpdateGPU = true;
						_NativeY = y;
						_CurrentSection = _CurrentSection + leftCount;
					}
				}
			}
		}
		else if (mouse_y >= _NativeY + sectionHeight + extraHeight + _Height)
		{
			if (_CurrentSection > 1)
			{
				mu_uint32 leftCount = _CurrentSection - 1;
				for (mu_uint32 i = 1; i <= leftCount - 1; ++i)
				{
					mu_float y = _NativeY + (mu_float)i * sectionHeight;
					if (CheckMouseInY(y + extraHeight + _Height, sectionHeight))
					{
						_UpdateGPU = true;
						_NativeY = y;
						_CurrentSection = _CurrentSection - i;
						break;
					}
				}

				if (_CurrentSection == oldSection)
				{
					mu_float y = _NativeY + (mu_float)leftCount * sectionHeight;
					if (CheckMouseInY(y + extraHeight + _Height, MU_RESOURCE->GetWindowHeight() - (y + extraHeight + _Height)))
					{
						_UpdateGPU = true;
						_NativeY = y;
						_CurrentSection = _CurrentSection - leftCount;
					}
				}
			}
		}

		if (_CurrentSection != oldSection)
		{
			_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_SLIDE_SECTION, 0);
		}
	}
}

void UISlideSection::OnMouseOver(mu_int32 mouse_x, mu_int32 mouse_y)
{
	if (!IsMouseFlag(MOUSE_FLAG_LEFT) &&
		_CurrentState != UI_DISABLED)
	{
		ChangeState(UI_OVER);
	}
}

void UISlideSection::OnMouseLeave()
{
	if (_CurrentState == UI_OVER)
	{
		ChangeState(UI_NORMAL);
	}
}

void UISlideSection::OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys)
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

void UISlideSection::OnMouseReleased(mu_uint32 mouse_flags, mu_uint32 virtual_keys)
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

void UISlideSection::OnMouseCancelled(mu_uint32 mouse_flags)
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