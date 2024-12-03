#ifndef __MU_UIOPTIONROOT_H__
#define __MU_UIOPTIONROOT_H__

#pragma once

class UIOptionRoot : public UIBase, public UISceneManager
{
public:
	UIOptionRoot();
	~UIOptionRoot();

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
	virtual mu_uint32 OnPostKeyPressed(const SDL_Keysym &key) override;
	virtual mu_uint32 OnKeyPressed(const SDL_Keysym &key) override;

	virtual void OnGFxSceneMessage(GFX_MESSAGE &objMsg) override;

	void Show(mu_uint32 Key);
	void Hide(mu_uint32 Key);
	mu_boolean IsVisible(mu_uint32 Key);

	void HideAll();
	
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

public:
	EINLINE mu_boolean AnyVisible()
	{
		return _VisibleBitset.any();
	}

private:
	mu_uint32 _MouseState;
	std::map<mu_uint32, UIBase*> _ObjectsMap;
	UIBase *_SelectedObject;
	UIBase *_FocusObject;

public:
	std::bitset<256> _VisibleBitset;

	UISpriteColor *_Background;

	UILoginMenu *_LoginMenu;
	UICharacterMenu *_CharacterMenu;
	UIGameMenu *_GameMenu;

	UIOptionWindow *_OptionWindow;
};

extern UIOptionRoot *g_pOptionRoot;

#define g_pLoginMenu g_pOptionRoot->_LoginMenu
#define g_pCharacterMenu g_pOptionRoot->_CharacterMenu
#define g_pGameMenu g_pOptionRoot->_GameMenu
#define g_pOption g_pOptionRoot->_OptionWindow

#endif