#ifndef __MU_UISERVERLIST_H__
#define __MU_UISERVERLIST_H__

#pragma once

struct SERVER_INFO
{
	mu_boolean Active;
	mu_string ServerName;
	mu_uint8 ServerType;
	mu_uint8 ServerFlag;
	mu_int32 Percent;
};

struct GROUP_INFO
{
	mu_boolean Active;
	mu_string GroupName;
	SERVER_INFO Servers[SERVERLIST::MAX_SERVER];
};

class UIServerList : public UIBase
{
public:
	UIServerList(UISceneManager *manager);
	~UIServerList();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void Clear();
	void ClearServer(mu_int32 groupIndex);

	void Setup();

	const mu_string &GetSelectedGroupName();
	mu_int32 GetSelectedServerIndex();
	mu_uint8 GetSelectedServerType();
	mu_uint8 GetSelectedServerFlag();

	const mu_string &GetGroupName(mu_int32 GroupIndex);

public:
	UIToggleButton *_GroupButton[SERVERLIST::MAX_SERVER_GROUP];
	UIButton *_ServerButton[SERVERLIST::MAX_SERVER];
	UISprite *_ServerGauge[SERVERLIST::MAX_SERVER];

	mu_int32 _GroupSelected;
	mu_int32 _ServerSelected;
	GROUP_INFO _ServerGroups[SERVERLIST::MAX_SERVER_GROUP];
};

#endif