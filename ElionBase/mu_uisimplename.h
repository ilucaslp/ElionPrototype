#ifndef __MU_UISIMPLENAME_H__
#define __MU_UISIMPLENAME_H__

#pragma once

class UISimpleName : public UIBase
{
public:
	UISimpleName(UISceneManager *manager);
	~UISimpleName();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;

	void RenderItemName(mu_int32 i, OBJECT *o, ITEM &Item, mu_boolean Sort, EVector4 &Limits);

	template<mu_size Length>
	void ItemNameColor(OBJECT *o, mu_text(&Name)[Length], MUTextSimpleRender* &Font, mu_uint32 &TextColor, mu_uint32 &BackColor, ITEM &Item);

	virtual mu_uint32 OnPreWindowMessage(const SDL_Event *event, mu_size *result) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

private:
	mu_boolean _ShowItemName;
};

#endif