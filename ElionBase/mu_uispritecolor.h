#ifndef __MU_UISPRITECOLOR_H__
#define __MU_UISPRITECOLOR_H__

#pragma once

class UISpriteColor : public UIRenderTextureBase
{
public:
	UISpriteColor(UISceneManager *manager, UIBase *owner, mu_uint32 color, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_float width, mu_float height);
	~UISpriteColor();

	virtual void Draw() override;

	void SetRotation(mu_float angle);
	void SetColor(mu_uint32 color);

private:
	mu_uint32 _Color;
	mu_float _Angle;

	mu_uint32 _VertexIndex;
};

#endif