#ifndef __MU_UI_SPRITE_H__
#define __MU_UI_SPRITE_H__

#pragma once

class UISprite : public UIRenderTextureBase
{
public:
	UISprite(UISceneManager *manager, UIBase *owner, MUTexture *texture, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_float width, mu_float height, EVector2 uvBegin = EVector2(0.0f, 0.0f), EVector2 uvEnd = EVector2(1.0f, 1.0f), mu_uint32 color = MU::MakeARGB(255, 255, 255, 255));
	~UISprite();

	virtual void Draw() override;

	void SetStateText(MUTextSimpleRender *font = nullptr, const mu_text *text = _T(""), mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255), mu_uint32 backColor = MU::MakeARGB(0, 0, 0, 0), mu_float marginX = 0.0f, mu_float marginY = 0.0f);
	void SetUV(const EVector2 begin, const EVector2 end);
	void SetRotation(const mu_float angle);
	void SetTexture(const MUTexture *texture);
	void SetColor(const mu_uint32 color);

private:
	UI_STATE_TEXT _TextStates;
	mu_uint32 _TextMessageIndex;

	const MUTexture *_Texture;
	EVector2 _UVBegin, _UVEnd;
	mu_uint32 _Color;
	mu_float _Angle;

	mu_uint32 _VertexIndex;
};

#endif