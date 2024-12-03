#include "stdafx.h"
#include "mu_uioverbutton.h"

UIOverButton::UIOverButton(mu_uint32 objectId, UISceneManager *manager, UIBase *owner, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_float width, mu_float height, mu_uint32 flag, mu_uint32 event) : UIRenderTextureBase()
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
}

UIOverButton::~UIOverButton()
{
}

void UIOverButton::Draw()
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
}

void UIOverButton::SetStateData(const mu_uint32 index, const UI_STATE_DATA &state)
{
	_UpdateGPU = true;

	mu_memcpy(&_States[index], &state, sizeof(state));
}

void UIOverButton::ChangeState(mu_uint32 state)
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

void UIOverButton::OnMouseOver(mu_int32 mouse_x, mu_int32 mouse_y)
{
	if (_CurrentState == UI_DISABLED)
	{
		return;
	}

	if (MU_INPUT->IsMousePressing(MOUSE_BUTTON_LEFT) == true)
	{
		_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
	}
}

void UIOverButton::OnMouseLeave()
{

}

void UIOverButton::OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys)
{
	if (_CurrentState == UI_DISABLED)
	{
		return;
	}

	if (_SceneManager)
	{
		_SceneManager->OnGFxMessagePush(this->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
	}
}