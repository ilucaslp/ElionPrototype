#include "stdafx.h"
#include "mu_uispriteuv.h"

UISpriteUV::UISpriteUV(UISceneManager *manager, UIBase *owner, MUTexture *texture, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_float width, mu_float height, EVector2 uv, EVector2 uvWH, mu_uint32 color) : UIRenderTextureBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_ObjectID = MESSAGE_UNKNOWN;
	_SceneManager = manager;
	_Texture = texture;

	UIBase::SetAspectRatio(aspectWidth, aspectHeight);
	UIBase::SetPosition(x, y);
	UIBase::SetDimensions(width, height);
	UIBase::SetParent(owner);

	_Angle = 0.0f;
	_UV = uv;
	_UVWH = uvWH;
	_Color = color;

	SetGfxFlag(GFX_FLAG::GFX_NO_FOCUS);
	SetEvent(GFX_EVENT::GFX_NONE);

	// Device specific data
	_VertexIndex = EInvalidUInt32;
}

UISpriteUV::~UISpriteUV()
{
}

void UISpriteUV::Draw()
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

		const mu_float tmpX = _NativeX, tmpY = _NativeY;

		// Triangle Strip method (Order: LT, RT, BL, BR)
		if (_Angle != 0.0f)
		{
			EMatrix4 rotation = EMath::Rotate(_Angle, EVector3(0.0f, 0.0f, 1.0f));

			mu_float hw = _Width * 0.5f, hh = _Height * 0.5f;
			vertices[0].Position = EVector2(-hw, -hh);
			vertices[1].Position = EVector2(+hw, -hh);
			vertices[2].Position = EVector2(-hw, +hh);
			vertices[3].Position = EVector2(+hw, +hh);

			for (mu_uint32 index = 0; index < 4; ++index)
			{
				vertices[index].Position = EVector2(EMath::Transform(rotation, EVector3(vertices[index].Position, 0.0f)));
			}

			vertices[0].Position += EVector2(tmpX, tmpY);
			vertices[1].Position += EVector2(tmpX, tmpY);
			vertices[2].Position += EVector2(tmpX, tmpY);
			vertices[3].Position += EVector2(tmpX, tmpY);
		}
		else
		{
			vertices[0].Position = EVector2(tmpX, tmpY);
			vertices[1].Position = EVector2(tmpX + _Width, tmpY);
			vertices[2].Position = EVector2(tmpX, tmpY + _Height);
			vertices[3].Position = EVector2(tmpX + _Width, tmpY + _Height);
		}

		vertices[0].UV = EVector2(_UV.x, _UV.y + _UVWH.y * 0.25f);
		vertices[1].UV = EVector2(_UV.x + _UVWH.x, _UV.y);
		vertices[2].UV = EVector2(_UV.x, _UV.y + _UVWH.y + _UVWH.y * 0.25f);
		vertices[3].UV = EVector2(_UV.x + _UVWH.x, _UV.y + _UVWH.y);

		vertices[0].Color = _Color;
		vertices[1].Color = _Color;
		vertices[2].Color = _Color;
		vertices[3].Color = _Color;

		EVideoBackend::UIRequestUpdateData(_VertexIndex, 4);

		_UpdateGPU = false;
	}

	if (_VertexIndex != EInvalidUInt32 &&
		_Texture != nullptr)
	{
		if(_Texture->IsLoaded() == true)
		{
			EVideoPipeline::SetGraphicsPipelineAtRuntime(ECombinedShader::eUILimitedAlpha, ERasterizer::eBackOnly, _DepthStencil, _BlendState, false);
			EVideoPipeline::BindGraphicsUITextureAtRuntime({ _Texture, _Sampler });
			EVideoPipeline::BindGraphicsUIAtRuntime();

			EVideoPipeline::DrawAtRuntime(_VertexIndex, 4, ETopologyType::eTriangleStrip);
		}
		else
		{
			g_LoadingManager.AddTextureRuntime(_Texture);
		}
	}
}

void UISpriteUV::SetUV(const EVector2 uv, const EVector2 uvWH)
{
	_UpdateGPU = true;
	_UV = uv;
	_UVWH = uvWH;
}

void UISpriteUV::SetRotation(const mu_float angle)
{
	_UpdateGPU = true;
	_Angle = angle;
}

void UISpriteUV::SetTexture(const MUTexture *texture)
{
	_UpdateGPU = true;
	_Texture = texture;
}

void UISpriteUV::SetColor(const mu_uint32 color)
{
	_UpdateGPU = true;
	_Color = color;
}