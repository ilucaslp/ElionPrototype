#include "stdafx.h"
#include "mu_uisprite.h"

UISprite::UISprite(UISceneManager *manager, UIBase *owner, MUTexture *texture, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_float width, mu_float height, EVector2 uvBegin, EVector2 uvEnd, mu_uint32 color) : UIRenderTextureBase()
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
	_UVBegin = uvBegin;
	_UVEnd = uvEnd;
	_Color = color;

	SetGfxFlag(GFX_FLAG::GFX_NO_FOCUS);
	SetEvent(GFX_EVENT::GFX_NONE);

	_VertexIndex = EInvalidUInt32;

	// Text
	_TextMessageIndex = -1;
}

UISprite::~UISprite()
{
}

void UISprite::Draw()
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

		vertices[0].UV = _UVBegin;
		vertices[1].UV = EVector2(_UVEnd.x, _UVBegin.y);
		vertices[2].UV = EVector2(_UVBegin.x, _UVEnd.y);
		vertices[3].UV = _UVEnd;

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
		if (_Texture->IsLoaded() == true)
		{
			EVideoPipeline::SetGraphicsPipelineAtRuntime(ECombinedShader::eUIDefault, ERasterizer::eBackOnly, _DepthStencil, _BlendState, false);
			EVideoPipeline::BindGraphicsUITextureAtRuntime({ _Texture, _Sampler });
			EVideoPipeline::BindGraphicsUIAtRuntime();

			EVideoPipeline::DrawAtRuntime(_VertexIndex, 4, ETopologyType::eTriangleStrip);
		}
		else
		{
			g_LoadingManager.AddTextureRuntime(_Texture);
		}
	}

	if (_TextStates.Font != nullptr &&
		_TextStates.Text.empty() == false)
	{
		_TextStates.Font->DrawStringC2D(_TextMessageIndex,
			MU_TO_WINDOW(_ViewX + _ViewWidth * 0.5f + _TextStates.MarginX, _AspectWidth),
			MU_TO_WINDOW(_ViewY + _ViewHeight * 0.5f + _TextStates.MarginY, _AspectHeight),
			1.0f,
			ETA_CENTER,
			ETA_CENTER,
			2.0f,
			_TextStates.TextColor,
			_TextStates.BackColor,
			_TextStates.Text.c_str());
	}
}

void UISprite::SetStateText(MUTextSimpleRender *font, const mu_text *text, mu_uint32 textColor, mu_uint32 backColor, mu_float marginX, mu_float marginY)
{
	if (_TextMessageIndex == -1)
	{
		_TextMessageIndex = g_RandomTextIndex.fetch_add(1);
	}

	_TextStates.Font = font;
	_TextStates.Text = text;
	_TextStates.TextColor = textColor;
	_TextStates.BackColor = backColor;
	_TextStates.MarginX = marginX;
	_TextStates.MarginY = marginY;
}

void UISprite::SetUV(const EVector2 begin, const EVector2 end)
{
	_UpdateGPU = true;
	_UVBegin = begin;
	_UVEnd = end;
}

void UISprite::SetRotation(const mu_float angle)
{
	_UpdateGPU = true;
	_Angle = angle;
}

void UISprite::SetTexture(const MUTexture *texture)
{
	_Texture = texture;
}

void UISprite::SetColor(const mu_uint32 color)
{
	_UpdateGPU = true;
	_Color = color;
}