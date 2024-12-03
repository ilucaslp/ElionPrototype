#ifndef __MU_UI_SPRITE_UV_H__
#define __MU_UI_SPRITE_UV_H__

#pragma once

class UISpriteUV : public UIRenderTextureBase
{
public:
	UISpriteUV(UISceneManager *manager, UIBase *owner, MUTexture *texture, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_float width, mu_float height, EVector2 uv = EVector2(0.0f, 0.0f), EVector2 uvWH = EVector2(1.0f, 1.0f), mu_uint32 color = MU::MakeARGB(255, 255, 255, 255));
	~UISpriteUV();

	virtual void Draw() override;

	void SetUV(const EVector2 uv, const EVector2 uvWH);
	void SetRotation(const mu_float angle);
	void SetTexture(const MUTexture *texture);
	void SetColor(const mu_uint32 color);

private:
	const MUTexture *_Texture;
	EVector2 _UV, _UVWH;
	mu_uint32 _Color;
	mu_float _Angle;

	mu_uint32 _VertexIndex;
};

#endif