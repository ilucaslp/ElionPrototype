#ifndef __MU_UITRADEINVENTORY_H__
#define __MU_UITRADEINVENTORY_H__

#pragma once

#define TRADE_POSX 260.0f
#define TRADE_POSY 0.0f
#define TRADE_WIDTH 190.0f
#define TRADE_HEIGHT 429.0f

class UITradeInventory : public UIBase
{
public:
	UITradeInventory(UISceneManager *manager);
	~UITradeInventory();

	void InitYourInventoryBackup();
	void BackupYourInventoryItem(ITEM *item);
	void BackupYourInventory();
	void ResetInfo();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void Draw3D() override;
	virtual void PostDraw() override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void DrawFrames();
	void DrawDetails();
	void DrawButtons();
	void PostDrawButtons();

	void ConvertYourLevel(mu_int32 &rLevel, mu_uint32 &rColor);

	void ChangeYourOkButton(mu_boolean isOk, mu_boolean waitTime);

	void SetWaitTime(mu_uint64 waitTime);
	void SetTradeMoney(mu_int64 MyMoney, mu_int64 YourMoney, mu_boolean setWaitTime = false);

	void AlertYourTradeInventory();

public:
	EINLINE UIInventoryControl *GetMyInventoryControl()
	{
		return _MyInventoryControl;
	}

	EINLINE UIInventoryControl *GetYourInventoryControl()
	{
		return _YourInventoryControl;
	}

	EINLINE void SetMyData(const mu_text *characterName, mu_int32 characterLevel)
	{
		_MyName = characterName;
		_MyLevel = characterLevel;
	}

	EINLINE void SetYourData(const mu_text *characterName, mu_int32 characterLevel, mu_int32 guildIndex)
	{
		_YourName = characterName;
		_YourLevel = characterLevel;
		_YourGuildIndex = guildIndex;

		_YourGuildIcon->SetTexture(nullptr);

		for (mu_uint32 n = 0; n < MAX_MARKS; ++n)
		{
			if (GuildMark[n].GuildIndex != -1 &&
				GuildMark[n].GuildIndex == _YourGuildIndex)
			{
				_YourGuildMarkIndex = n;
				_YourGuildIcon->SetTexture(&TEXTURES::Data[BITMAP_GUILD2_BEGIN + n]);
				break;
			}
		}
	}

	EINLINE mu_int64 GetMyGold()
	{
		return _MyTradeGold;
	}

	EINLINE mu_int64 GetYourGold()
	{
		return _YourTradeGold;
	}

	EINLINE void SetAlertTrade(mu_boolean alert)
	{
		_AlertTrade = alert;
	}

	EINLINE mu_boolean IsAlertTrade()
	{
		return _AlertTrade;
	}

private:
	friend class UIGameRoot;
	friend class UIMessageRoot;
	friend class GSNetworkObject;
	UISprite *_BackFrame;
	UISprite *_TopFrame;
	UISprite *_BottomFrame;
	UISprite *_LeftFrame;
	UISprite *_RightFrame;
	UISprite *_MiddleFrame;

	UISprite *_MyNameFrame;
	UISprite *_YourNameFrame;
	UISprite *_MyMoneyIcon;
	UISprite *_YourMoneyIcon;
	UISprite *_YourGuildIcon;

	UISprite *_WarningArrow[MAX_TRADE_INVENTORY];

	ITEM _YourInventoryBackup[MAX_TRADE_INVENTORY];
	UIInventoryControl *_MyInventoryControl;
	UIInventoryControl *_YourInventoryControl;

	UIButton *_XButton;
	UIButton *_CancelButton;
	UIButton *_MoneyButton;
	UIToggleButton *_OkButton;
	UIToggleButton *_YourOkButton;

	mu_int32 _MyLevel;
	mu_string _MyName;
	mu_int64 _MyTradeGold;

	mu_int32 _YourLevel;
	mu_string _YourName;
	mu_int64 _YourTradeGold;
	mu_int32 _YourGuildIndex;
	mu_int32 _YourGuildMarkIndex;

	mu_uint64 _TradeWaitTime;
	mu_boolean _AlertTrade;
};

#endif