#ifndef __MU_UIMESSAGEROOT_H__
#define __MU_UIMESSAGEROOT_H__

#pragma once

#define MSGBOX_WIDTH 230.0f
#define MSGBOX_TOP_HEIGHT 67.0f
#define MSGBOX_BOTTOM_HEIGHT 50.0f
#define MSGBOX_MIDDLE_HEIGHT 15.0f

#define MSGBOX_BACK_BLANK_WIDTH 8.0f
#define MSGBOX_BACK_BLANK_HEIGHT 10.0f

#define MSGBOX_TEXT_TOP_BLANK 35.0f
#define MSGBOX_TEXT_MAXWIDTH 180.0f

#define MSGBOX_LINE_WIDTH 223.0f
#define MSGBOX_LINE_HEIGHT 21.0f

#define MSGBOX_SEPARATE_LINE_WIDTH 205.0f
#define MSGBOX_SEPARATE_LINE_HEIGHT 2.0f

#define MSGBOX_BTN_WIDTH 54.0f
#define MSGBOX_BTN_HEIGHT 30.0f
#define MSGBOX_BTN_BOTTOM_BLANK 20.0f

#define MSGBOX_BTN_EMPTY_SMALL_WIDTH 64.0f
#define MSGBOX_BTN_EMPTY_WIDTH 108.0f
#define MSGBOX_BTN_EMPTY_BIG_WIDTH 180.0f
#define MSGBOX_BTN_EMPTY_HEIGHT 29.0f

class UIMessageRoot : public UIBase, public UISceneManager
{
public:
	UIMessageRoot();
	~UIMessageRoot();

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

	UIOkBox *_OkBox;
	UIFailedConnect *_FailedConnect;
	UIServerFull *_ServerFull;
	UIBannedFromServer *_BannedFromServer;
	UIBlockedFromServer *_BlockedFromServer;
	UIWaitingBox *_WaitingBox;
	UIConnectBox *_ConnectBox;
	UIDeleteCharacterWarning *_DeleteCharacter;
	UIDisconnectedBox *_DisconnectedBox;
	UIExitGameBox *_ExitGameBox;
	UIChangeServerBox *_ChangeServerBox;
	UIGuildLeaveBox *_GuildLeaveBox;
	UIGuildRequestBox *_GuildRequestBox;
	UIGuildCancelStatusBox *_GuildCancelStatusBox;
	UIGuildGiveStatusBox *_GuildGiveStatusBox;
	UIPartyRequestBox *_PartyRequestBox;
	UITradeRequestBox *_TradeRequestBox;
	UITradeMoneyBox *_TradeMoneyBox;
	UITradeAcceptBox *_TradeAcceptBox;
	UIStorageDepositBox *_StorageDepositBox;
	UIStorageWithdrawBox *_StorageWithdrawBox;
	UIStorageLockBox *_StorageLockBox;
	UIHighValueWarningBox *_HighValueWarningBox;
	UIGambleBuyBox *_GambleBuyBox;
	UIPCPointBuyBox *_PCPointBuyBox;
	UIChaosMixMenu *_ChaosMixMenuBox;
	UIMixCheckBox *_MixCheckBox;
	UIBloodCastleRank *_BloodCastleRank;
	UIHelperSkillSetting *_HelperSkillSetting;
	UIHelperPotionSetting *_HelperPotionSetting;
	UIHelperElfPotionSetting *_HelperElfPotionSetting;
	UIHelperPartySetting *_HelperPartySetting;
	UIHelperWizardPartySetting *_HelperWizardPartySetting;
	UIHelperSkillSelect *_HelperSkillSelect;
	UIHelperItemAdd *_HelperItemAdd;
	UIHelperStart *_HelperStart;
};

extern UIMessageRoot *g_pMessageRoot;

#define g_pExitGameBox g_pMessageRoot->_ExitGameBox
#define g_pOkBox g_pMessageRoot->_OkBox
#define g_pWaitingBox g_pMessageRoot->_WaitingBox
#define g_pChangeServerBox g_pMessageRoot->_ChangeServerBox
#define g_pStorageLockBox g_pMessageRoot->_StorageLockBox
#define g_pHighValueWarningBox g_pMessageRoot->_HighValueWarningBox
#define g_pGuildLeaveBox g_pMessageRoot->_GuildLeaveBox
#define g_pGuildRequestBox g_pMessageRoot->_GuildRequestBox
#define g_pGuildGiveStatus g_pMessageRoot->_GuildGiveStatusBox
#define g_pGuildCancelStatus g_pMessageRoot->_GuildCancelStatusBox
#define g_pPartyRequestBox g_pMessageRoot->_PartyRequestBox
#define g_pTradeRequestBox g_pMessageRoot->_TradeRequestBox
#define g_pTradeMoneyBox g_pMessageRoot->_TradeMoneyBox
#define g_pBloodCastleRank g_pMessageRoot->_BloodCastleRank
#define g_pHelperSkillSetting g_pMessageRoot->_HelperSkillSetting
#define g_pHelperPotionSetting g_pMessageRoot->_HelperPotionSetting
#define g_pHelperElfPotionSetting g_pMessageRoot->_HelperElfPotionSetting
#define g_pHelperPartySetting g_pMessageRoot->_HelperPartySetting
#define g_pHelperWizardPartySetting g_pMessageRoot->_HelperWizardPartySetting
#define g_pHelperSkillSelect g_pMessageRoot->_HelperSkillSelect
#define g_pHelperItemAdd g_pMessageRoot->_HelperItemAdd

#endif