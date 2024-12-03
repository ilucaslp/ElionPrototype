#ifndef __GS_NETWORKOBJECT_H__
#define __GS_NETWORKOBJECT_H__

#pragma once

class GSNetworkObject : public ElionNetwork::ELNetworkObject
{
public:
	enum
	{
		PACKET_BUFFER_SIZE = (32 * 1024), // 32KB
	};

public:
	GSNetworkObject(ElionNetwork::ELNetworkManager *networkManager, mu_uint32 uniqueId, mu_uint32 bufferSize);
	~GSNetworkObject();

	mu_boolean Send(mu_uint8 *buffer, mu_uint32 bufferSize);
	mu_boolean Send(MU_CLIENT_TO_SERVER_HEADEREX *packet);

protected:
	virtual mu_boolean OnConnect() override;
	virtual void OnDisconnect() override;

	virtual ElionNetwork::ELNetworkPacket *CreatePacket() override;

	virtual void OnReport(REPORT_TYPE reportType, mu_string report, mu_int32 errorCode) override;

public:
	void ProcessPackets();
	
private: // Receive functions
	void OnReceiveConnected(GSNetworkPacket *packet);
	mu_boolean OnReceiveLogin(GSNetworkPacket *packet);
	void OnReceiveCharacterList(GSNetworkPacket *packet);
	void OnReceiveCreateCharacter(GSNetworkPacket *packet);
	void OnReceiveDeleteCharacter(GSNetworkPacket *packet);
	mu_boolean OnReceiveSelectCharacter(GSNetworkPacket *packet);
	void OnReceiveDamage(GSNetworkPacket *packet);
	void OnReceiveSimpleDamage(GSNetworkPacket *packet);
	void OnReceiveKillPlayer(GSNetworkPacket *packet);
	void OnReceiveItemInsert(GSNetworkPacket *packet);
	void OnReceiveItemDelete(GSNetworkPacket *packet);
	void OnReceiveItemModify(GSNetworkPacket *packet);
	void OnReceiveItemDurability(GSNetworkPacket *packet);
	void OnReceiveItemMoveResult(GSNetworkPacket *packet);
	void OnReceiveShopList(GSNetworkPacket *packet);
	void OnReceiveTaxRate(GSNetworkPacket *packet);
	void OnReceivePickItem(GSNetworkPacket *packet);
	void OnReceivePickGold(GSNetworkPacket *packet);
	void OnReloadArrow(GSNetworkPacket *packet);
	void OnServerCommand(GSNetworkPacket *packet);

	void OnRequestGuildJoin(GSNetworkPacket *packet);

	void OnRequestParty(GSNetworkPacket *packet);
	void OnLeaveParty(GSNetworkPacket *packet);

	void OnReceiveTalk(GSNetworkPacket *packet);

	void OnReceiveStorageState(GSNetworkPacket *packet);

	void OnReceiveBuy(GSNetworkPacket *packet);
	void OnReceiveSell(GSNetworkPacket *packet);

	void OnReceiveMixResult(GSNetworkPacket *packet);

	void OnRequestTrade(GSNetworkPacket *packet);
	void OnReceiveTradeAnswer(GSNetworkPacket *packet);
	void OnReceiveTradeResult(GSNetworkPacket *packet);
	void OnReceiveTradeOk(GSNetworkPacket *packet);

	void OnReceiveCreateGuild(GSNetworkPacket *packet);
	void OnReceiveGuildInfo(GSNetworkPacket *packet);
	void OnReceiveGuildLeave(GSNetworkPacket *packet);

	void OnReceiveQuestAnswer(GSNetworkPacket *packet);

	void OnReceiveSetAttribute(GSNetworkPacket *packet);

	void OnReceiveViewportPackage(GSNetworkPacket *packet);

private:
	void OnReceiveEventEnter(GSNetworkPacket *packet);
	void OnReceiveEventTimer(GSNetworkPacket *packet);
	void OnReceiveEventRank(GSNetworkPacket *packet);

	// Blood Castle
	void OnReceiveEventState(GSNetworkPacket *packet);
	void OnReceiveHelperStarted(GSNetworkPacket *packet);
	void OnReceiveHelperStopped(GSNetworkPacket *packet);

private: // Receive general functions
	void OnReceiveClientDenied(GSNetworkPacket *packet);
	void OnReceiveLogout(GSNetworkPacket *packet);

private: // Receive chat functions
	void OnReceiveChat(GSNetworkPacket *packet);
	void OnReceiveChatKey(GSNetworkPacket *packet);
	void OnReceiveWhisper(GSNetworkPacket *packet);
	void OnReceiveWhisperFail(GSNetworkPacket *packet);
	void OnReceiveNotice(GSNetworkPacket *packet);

public: // Send functions
	void SendRequestLogout(mu_uint8 Type);
	void SendChatWhisper(const mu_text *WhisperID, const mu_text *ChatText);
	void SendChat(const mu_text *ChatText);
	void SendHeroAngle(mu_float Angle);
	void SendUseItem(mu_uint8 SourceIndex, mu_uint8 TargetIndex = 0xFF, mu_uint8 UseType = 0);
	void SendDropItem(mu_uint8 InventoryType, mu_uint8 InventoryX, mu_uint8 InventoryY, mu_uint8 MapX, mu_uint8 MapY);
	void SendGuildLeave(mu_int32 memberIndex);
	void SendGuildAnswer(mu_boolean Answer);
	void SendGuildCancelStatus(mu_int32 CharacterIndex);
	void SendGuildGiveStatus(mu_int32 CharacterIndex, mu_int16 GuildStatus);
	void SendRequestParty(mu_int16 Key);
	void SendPartyAnswer(mu_boolean Answer);
	void SendPartyKick(mu_uint8 Number);
	void SendStorageDeposit(mu_int64 Gold);
	void SendStorageWithdraw(mu_int64 Gold);
	void SendCloseWindow();
	void SendRequestRepair(mu_uint8 SrcType, mu_uint8 SrcX, mu_uint8 SrcY, mu_boolean isShopRepair);
	void SendRequestRepairAll();
	void SendRequestSell(mu_uint8 SrcType, mu_uint8 SrcX, mu_uint8 SrcY);
	void SendRequestBuy(mu_uint8 SrcX, mu_uint8 SrcY);
	void SendRequestTrade(mu_int16 Key);
	void SendTradeAnswer(mu_boolean Answer);
	void SendTradeMoney(mu_int64 Money);
	void SendTradeOk(mu_boolean isOk);
	void SendRequestGuildInfo(mu_int32 GuildIndex);
	void SendRequestBloodCastleEnter(mu_int32 BloodLevel, mu_int32 ItemIndex);

	void SendHelperStart(mu_uint8 mode);
	void SendHelperStop();

public:
	/**
	*	VIRTUAL
	*	Socket send buffer size (setsockopt used)
	**/
	virtual mu_uint32 GetSocketSendSize() override
	{
		return (mu_uint32)NETWORK_BUFFER_SIZE;
	}

	/**
	*	VIRTUAL
	*	Socket recv buffer size (setsockopt used)
	**/
	virtual mu_uint32 GetSocketRecvSize() override
	{
		return (mu_uint32)NETWORK_BUFFER_SIZE;
	}

	/**
	*	VIRTUAL
	*	Use TCP no delay (setsockopt used)
	**/
	virtual mu_boolean IsTcpNoDelay() override
	{
		return false;
	}

	/**
	*	VIRTUAL
	*	Use TCP keep alive (setsockopt used)
	**/
	virtual mu_boolean IsTcpKeepAlive() override
	{
		return false;
	}

	/**
	*	Should disable auto-tuning?
	*	\return true if yes, false if not.
	**/
	virtual mu_boolean DisableAutotuning() override
	{
		return false;
	}

	/**
	*	Enable manual packet processing
	*	\return true if yes, false if not.
	**/
	virtual mu_boolean EnableManualProcessing() override
	{
		return true;
	}

private:
	MUCriticalSectionSpinLock _PacketCritical;
	mu_uint8 *_PacketBuffer;
	mu_uint32 _PacketBufferSize;
};

extern GSNetworkObject *g_GameServerObject;
extern MUBlowfish g_GameServerCrypt;
extern mu_boolean g_GameServerConnected;

void InternalViewportProcess(mu_uint8 *PacketBuffer, mu_boolean InternalUsage = false);

#endif