#ifndef __MU_UIPARTYLIST_H__
#define __MU_UIPARTYLIST_H__

#pragma once

#define PARTYLIST_POSX (561.0f)
#define PARTYLIST_POSY (14.0f)

class UIPartyList : public UIBase
{
public:
	UIPartyList(UISceneManager *manager);

	void UpdateWindow(mu_int32 screenWidth);

	virtual void Draw() override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	mu_int32 GetSelectedCharacter();

private:
	UISpriteColor *_MemberBack[MAX_PARTYS];
	UISpriteColor *_MemberEffect[MAX_PARTYS];
	UISpriteColor *_MemberOffline[MAX_PARTYS];
	UIButton *_MemberSelected[MAX_PARTYS];
	UISprite *_MemberFrame[MAX_PARTYS];
	UISprite *_MemberBar[MAX_PARTYS];
	UISprite *_MemberFlag;

	UIButton *_LeaveBtn[MAX_PARTYS];
};

#endif