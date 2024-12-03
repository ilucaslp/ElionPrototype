#ifndef __MU_UIGAMEROOT_H__
#define __MU_UIGAMEROOT_H__

#pragma once

class UIGameRoot : public UIBase, public UISceneManager
{
public:
	UIGameRoot();
	~UIGameRoot();

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

	void Toggle(mu_uint32 Key);
	void Show(mu_uint32 Key);
	void Hide(mu_uint32 Key);
	mu_boolean IsVisible(mu_uint32 Key);

	void UpdateWindowPosition();

	void ToggleCharacterInfo(mu_boolean isShow);
	void ToggleMyQuest(mu_boolean isShow);
	void ToggleNpcQuest(mu_boolean isShow);
	void ToggleMyInventory(mu_boolean isShow);
	void TogglePartyWindow(mu_boolean isShow);
	void ToggleGuildWindow(mu_boolean isShow);
	void ToggleGuildCreate(mu_boolean isShow);
	void ToggleChatInput(mu_boolean isShow);
	void ToggleChatFrame(mu_boolean isShow, mu_boolean changeFrame = true);
	void UpdateChatWindowSize();
	void UpdateChatWindowAlpha();
	void ToggleChatInputType(INPUT_MESSAGE_TYPE inputType);
	void ToggleChatLog();
	void ToggleChatMessageType(MESSAGE_TYPE messageType);
	void SetBlockWhisper(mu_boolean blockWhisper);

	mu_boolean IsAlwaysVisibleInterface(mu_uint32 Key);
	void HideAll();
	void HideAllGroupA();
	void HideAllGroupB();
	void HideAllGroupC();

	mu_boolean AllowHeroAction();

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
	UISimpleHPBar *_SimpleHpBar;
	UISimpleName *_SimpleName;
	UISimpleChat *_SimpleChat;
	UISimpleNotice *_SimpleNotice;
	UIMainFrame *_MainFrame;
	UIItemHotkey *_HotkeyFrame;
	UISkillFrame *_SkillFrame;
	UICharacterInfo *_CharacterInfo;
	UIQuestWindow *_MyQuestWindow;
	UINpcQuestWindow *_NpcQuestWindow;
	UIChatInput *_ChatInput;
	UIChatWindow *_ChatWindow;
	UIMyInventory *_MyInventoryWindow;
	UIMixInventory *_MixInventoryWindow;
	UIStorageInventory *_StorageWindow;
	UIShopInventory *_ShopWindow;
	UITradeInventory *_TradeWindow;
	UIPartyWindow *_PartyWindow;
	UIPartyList *_PartyList;
	UIGuildWindow *_GuildWindow;
	UIGuildCreate *_GuildCreate;
	UIMapName *_MapName;
	UIBuffStatus *_BuffWindow;
	UIHeroPosition *_HeroPosition;
	UIMoveWindow *_MoveWindow;
	UIBloodCastleTimer *_BloodCastleTimer;
	UIBloodCastleEnter *_BloodCastleEnter;
	UICommandWindow *_CommandWindow;
	UIHelperSetup *_HelperSetup;
};

extern UIGameRoot *g_pNewUISystem;

#define g_pSimpleName g_pNewUISystem->_SimpleName
#define g_pSimpleChat g_pNewUISystem->_SimpleChat
#define g_pSimpleNotice g_pNewUISystem->_SimpleNotice
#define g_pMainFrame g_pNewUISystem->_MainFrame
#define g_pSkillFrame g_pNewUISystem->_SkillFrame
#define g_pCharacterInfoWindow g_pNewUISystem->_CharacterInfo
#define g_pMyQuestWindow g_pNewUISystem->_MyQuestWindow
#define g_pNPCQuest g_pNewUISystem->_NpcQuestWindow
#define g_pChatListBox g_pNewUISystem->_ChatWindow
#define g_pChatInputBox g_pNewUISystem->_ChatInput
#define g_pMyInventory g_pNewUISystem->_MyInventoryWindow
#define g_pMixInventory g_pNewUISystem->_MixInventoryWindow
#define g_pStorageInventory g_pNewUISystem->_StorageWindow
#define g_pNpcShopInventory g_pNewUISystem->_ShopWindow
#define g_pTrade g_pNewUISystem->_TradeWindow
#define g_pGuildWindow g_pNewUISystem->_GuildWindow
#define g_pMapName g_pNewUISystem->_MapName
#define g_pBuffWindow g_pNewUISystem->_BuffWindow
#define g_pMoveWindow g_pNewUISystem->_MoveWindow
#define g_pPartyListWindow g_pNewUISystem->_PartyList
#define g_pBloodCastleTimer g_pNewUISystem->_BloodCastleTimer
#define g_pBloodCastleEnter g_pNewUISystem->_BloodCastleEnter
#define g_pCommandWindow g_pNewUISystem->_CommandWindow
#define g_pHeroPosition g_pNewUISystem->_HeroPosition
#define g_pHelperSetup g_pNewUISystem->_HelperSetup

#endif