#ifndef __MU_UILOGINROOT_H__
#define __MU_UILOGINROOT_H__

#pragma once

class UILoginRoot : public UIBase, public UISceneManager
{
public:
	UILoginRoot();
	~UILoginRoot();

	void AddObject(mu_uint32 index, UIBase *object);

	virtual mu_uint32 OnWindowMessage(const SDL_Event *event, mu_size *result) override;
	virtual mu_uint32 OnPostWindowMessage(const SDL_Event *event, mu_size *result) override;

	void OnWindowCancelled();

	virtual void SystemConfigure() override;

	virtual void InitializeGPU() override;
	virtual void ReleaseGPU() override;

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void Draw3D() override;
	virtual void PostDraw() override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;
	virtual mu_uint32 OnKeyPressed(const SDL_Keysym &key) override;

	virtual void OnGFxSceneMessage(GFX_MESSAGE &objMsg) override;

	void Show(mu_uint32 Key);
	void Hide(mu_uint32 Key);
	mu_boolean IsVisible(mu_uint32 Key);

public:
	virtual UIBase *GetSelected() override
	{
		return _SelectedObject;
	}

	virtual UIBase *GetFocused() override
	{
		return IsValidFocus(_FocusObject) ? _FocusObject : nullptr;
	}

	virtual UIBase *GetInternalFocused() override
	{
		return _FocusObject;
	}

private:
	mu_uint32 _MouseState;
	std::map<mu_uint32, UIBase*> _ObjectsMap;
	UIBase *_SelectedObject;
	UIBase *_FocusObject;

public:
	UILogo *_Logo;
	UILoginFrame *_LoginFrame;
	UIServerList *_ServerList;
	UILoginWindow *_LoginWindow;
	UIServerMessage *_ServerMessage;
};

extern UILoginRoot *g_pLoginRoot;
extern mu_char g_PasswordSecret[8 + 1];

#define g_pLoginLogo g_pLoginRoot->_Logo
#define g_pLoginFrame g_pLoginRoot->_LoginFrame
#define g_pServerList g_pLoginRoot->_ServerList
#define g_pLoginWindow g_pLoginRoot->_LoginWindow
#define g_pLoginListBox g_pLoginRoot->_ServerMessage

#endif