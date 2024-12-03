#include "stdafx.h"
#include "gs_networkobject.h"

void GSNetworkObject::SendRequestLogout(mu_uint8 Type)
{
	CL2GS_REQUEST_LOGOUT RequestLogout;

	RequestLogout.Type = Type;

	this->Send(&RequestLogout);
}

void GSNetworkObject::SendChatWhisper(const mu_text *WhisperID, const mu_text *ChatText)
{
	CL2GS_REQUEST_WHISPER RequestWhisper;

	RequestWhisper.Name = WhisperID;
	RequestWhisper.Message = ChatText;

	this->Send(&RequestWhisper);
}

void GSNetworkObject::SendChat(const mu_text *ChatText)
{
	static mu_string LastChat;
	if (ChatTime > 50)
	{
		return;
	}
	else if (ChatTime > 0)
	{
		if (mu_stricmp(LastChat.c_str(), ChatText) == 0) return;
	}
	LastChat = ChatText;
	ChatTime = 70;

	if (Hero->Dead > 0)
	{
		if (mu_strlen(GlobalText[260]) > 0 && !mu_strncmp(ChatText, GlobalText[260], mu_strlen(GlobalText[260])))
		{
			return;
		}
	}
	else if (ChatText[0] == _T('/'))
	{
		if (mu_strlen(GlobalText[264]) > 0 && !mu_strncmp(ChatText, GlobalText[264], mu_strlen(GlobalText[264])))
		{
			g_pNewUISystem->SetBlockWhisper(true);
			g_pChatListBox->AddText(_T(""), GlobalText[267], TYPE_SYSTEM_MESSAGE);
			return;
		}
		else if (mu_strlen(GlobalText[265]) && !mu_strncmp(ChatText, GlobalText[265], mu_strlen(GlobalText[265])))
		{
			g_pNewUISystem->SetBlockWhisper(false);
			g_pChatListBox->AddText(_T(""), GlobalText[268], TYPE_SYSTEM_MESSAGE);
			return;
		}
	}

	CL2GS_REQUEST_CHAT RequestChat;

	RequestChat.Message = ChatText;

	this->Send(&RequestChat);
}

void GSNetworkObject::SendHeroAngle(mu_float Angle)
{
	CL2GS_REQUEST_HEROANGLE RequestHeroAngle;

	RequestHeroAngle.Angle = Angle;

	this->Send(&RequestHeroAngle);
}

void GSNetworkObject::SendUseItem(mu_uint8 SourceIndex, mu_uint8 TargetIndex, mu_uint8 UseType)
{
	CL2GS_REQUEST_USEITEM RequestUseItem;

	RequestUseItem.SourceIndex = SourceIndex;
	RequestUseItem.TargetIndex = TargetIndex;
	RequestUseItem.ItemUseType = UseType;

	this->Send(&RequestUseItem);

	ITEM *Item = g_pMyInventory->FindItem(SourceIndex);

	if (Item != nullptr)
	{
		if (Item->Type == ITEM_POTION)
		{
			PlayBackground(SOUND_EAT_APPLE01);
		}
		else if (Item->Type >= ITEM_POTION + 1 && Item->Type <= ITEM_POTION + 9)
		{
			PlayBackground(SOUND_DRINK01);
		}
	}
}

void GSNetworkObject::SendDropItem(mu_uint8 InventoryType, mu_uint8 InventoryX, mu_uint8 InventoryY, mu_uint8 MapX, mu_uint8 MapY)
{
	CL2GS_REQUEST_DROPITEM RequestDropItem;

	RequestDropItem.InventoryType = InventoryType;
	RequestDropItem.InventoryX = InventoryX;
	RequestDropItem.InventoryY = InventoryY;
	RequestDropItem.MapX = MapX;
	RequestDropItem.MapY = MapY;

	this->Send(&RequestDropItem);
}

void GSNetworkObject::SendGuildLeave(mu_int32 memberIndex)
{
	CL2GS_REQUEST_LEAVEGUILD RequestLeaveGuild;
	RequestLeaveGuild.MemberIndex = memberIndex;

	this->Send(&RequestLeaveGuild);
}

void GSNetworkObject::SendGuildAnswer(mu_boolean Answer)
{
	CL2GS_RECEIVE_GUILDJOIN_PETITION GuildAnswer;

	GuildAnswer.Answer = Answer;

	this->Send(&GuildAnswer);
}

void GSNetworkObject::SendGuildCancelStatus(mu_int32 CharacterIndex)
{
	CL2GS_REQUEST_GUILDSTATUS_CANCEL RequestGuildStatusCancel;

	RequestGuildStatusCancel.CharacterIndex = CharacterIndex;

	this->Send(&RequestGuildStatusCancel);
}

void GSNetworkObject::SendGuildGiveStatus(mu_int32 CharacterIndex, mu_int16 GuildStatus)
{
	CL2GS_REQUEST_GUILDSTATUS_GIVE RequestGuildStatusGive;

	RequestGuildStatusGive.CharacterIndex = CharacterIndex;
	RequestGuildStatusGive.GuildStatus = GuildStatus;

	this->Send(&RequestGuildStatusGive);
}

void GSNetworkObject::SendRequestParty(mu_int16 Key)
{
	CL2GS_RECEIVE_PARTYREQUEST RequestParty;

	RequestParty.Key = Key;

	this->Send(&RequestParty);
}

void GSNetworkObject::SendPartyAnswer(mu_boolean Answer)
{
	CL2GS_RECEIVE_PARTYANSWER PartyAnswer;

	PartyAnswer.Answer = Answer;

	this->Send(&PartyAnswer);
}

void GSNetworkObject::SendPartyKick(mu_uint8 Number)
{
	CL2GS_RECEIVE_PARTYKICK RequestPartyKick;

	RequestPartyKick.Number = Number;

	this->Send(&RequestPartyKick);
}

void GSNetworkObject::SendStorageDeposit(mu_int64 Gold)
{
	CL2GS_REQUEST_STORAGEGOLD RequestDeposit;

	RequestDeposit.IsWithdraw = false;
	RequestDeposit.Gold = Gold;

	this->Send(&RequestDeposit);
}

void GSNetworkObject::SendStorageWithdraw(mu_int64 Gold)
{
	CL2GS_REQUEST_STORAGEGOLD RequestWithdraw;

	RequestWithdraw.IsWithdraw = true;
	RequestWithdraw.Gold = Gold;

	this->Send(&RequestWithdraw);
}

void GSNetworkObject::SendCloseWindow()
{
	if (ActionRequest == ACTION_REQUEST::AR_TALK)
	{
		ActionRequest = ACTION_REQUEST::AR_NONE;
	}

	CL2GS_REQUEST_CLOSEWINDOW RequestCloseWindow;

	this->Send(&RequestCloseWindow);
}

void GSNetworkObject::SendRequestRepair(mu_uint8 SrcType, mu_uint8 SrcX, mu_uint8 SrcY, mu_boolean isShopRepair)
{
	CL2GS_REQUEST_REPAIRITEM RequestRepairItem;

	RequestRepairItem.InventoryType = SrcType;
	RequestRepairItem.InventoryX = SrcX;
	RequestRepairItem.InventoryY = SrcY;
	RequestRepairItem.RepairAll = false;
	RequestRepairItem.ShopRepair = isShopRepair;

	this->Send(&RequestRepairItem);
}

void GSNetworkObject::SendRequestRepairAll()
{
	CL2GS_REQUEST_REPAIRITEM RequestRepairItem;

	RequestRepairItem.InventoryType = 0;
	RequestRepairItem.InventoryX = 0;
	RequestRepairItem.InventoryY = 0;
	RequestRepairItem.RepairAll = true;
	RequestRepairItem.ShopRepair = true;

	this->Send(&RequestRepairItem);
}

void GSNetworkObject::SendRequestSell(mu_uint8 SrcType, mu_uint8 SrcX, mu_uint8 SrcY)
{
	CL2GS_REQUEST_SELLITEM RequestSellItem;

	RequestSellItem.InventoryType = SrcType;
	RequestSellItem.InventoryX = SrcX;
	RequestSellItem.InventoryY = SrcY;

	this->Send(&RequestSellItem);
}

void GSNetworkObject::SendRequestBuy(mu_uint8 SrcX, mu_uint8 SrcY)
{
	CL2GS_REQUEST_BUYITEM RequestBuyItem;

	RequestBuyItem.InventoryX = SrcX;
	RequestBuyItem.InventoryY = SrcY;

	this->Send(&RequestBuyItem);
}

void GSNetworkObject::SendRequestTrade(mu_int16 Key)
{
	CL2GS_REQUEST_TRADE RequestTrade;

	RequestTrade.Key = Key;

	this->Send(&RequestTrade);
}

void GSNetworkObject::SendTradeAnswer(mu_boolean Answer)
{
	CL2GS_RECEIVE_TRADEANSWER TradeAnswer;

	TradeAnswer.Answer = Answer;

	this->Send(&TradeAnswer);
}

void GSNetworkObject::SendTradeMoney(mu_int64 Money)
{
	CL2GS_RECEIVE_TRADEMONEY TradeMoney;

	TradeMoney.Money = Money;

	this->Send(&TradeMoney);
}

void GSNetworkObject::SendTradeOk(mu_boolean isOk)
{
	CL2GS_RECEIVE_TRADEOK TradeOk;

	TradeOk.Answer = isOk;

	this->Send(&TradeOk);
}

void GSNetworkObject::SendRequestGuildInfo(mu_int32 GuildIndex)
{
	CL2GS_REQUEST_GUILDINFO RequestGuildInfo;

	RequestGuildInfo.GuildIndex = GuildIndex;

	this->Send(&RequestGuildInfo);
}

void GSNetworkObject::SendRequestBloodCastleEnter(mu_int32 BloodLevel, mu_int32 ItemIndex)
{
	CL2GS_REQUEST_BLOODCASTLE_ENTER RequestBloodCastleEnter;

	RequestBloodCastleEnter.Level = BloodLevel;
	RequestBloodCastleEnter.ItemIndex = ItemIndex;

	this->Send(&RequestBloodCastleEnter);
}

void GSNetworkObject::SendHelperStart(mu_uint8 mode)
{
	CL2GS_REQUEST_HELPERSTART RequestHelperStart;

	RequestHelperStart.StartType = mode;

	this->Send(&RequestHelperStart);
}

void GSNetworkObject::SendHelperStop()
{
	CL2GS_REQUEST_HELPERSTOP RequestHelperStop;

	this->Send(&RequestHelperStop);
}