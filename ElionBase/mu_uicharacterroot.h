#ifndef __MU_UICHARACTERROOT_H__
#define __MU_UICHARACTERROOT_H__

#pragma once

class UICharacterRoot : public UIBase, public UISceneManager
{
public:
	UICharacterRoot();
	~UICharacterRoot();

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
	UICharacterFrame *_CharacterFrame;
	UICreateCharacter *_CreateCharacter;
	UIServerMessage *_ServerMessage;
};

extern UICharacterRoot *g_pCharacterRoot;

#define g_pCharacterFrame g_pCharacterRoot->_CharacterFrame
#define g_pCreateCharacter g_pCharacterRoot->_CreateCharacter
#define g_pCharacterListBox g_pCharacterRoot->_ServerMessage

#endif