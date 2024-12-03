#ifndef __CS_NETWORKOBJECT_H__
#define __CS_NETWORKOBJECT_H__

#pragma once

class CSNetworkObject : public ElionNetwork::ELNetworkObject
{
public:
	enum
	{
		PACKET_BUFFER_SIZE = (32 * 1024), // 32KB
	};

public:
	CSNetworkObject(ElionNetwork::ELNetworkManager *networkManager, mu_uint32 uniqueId, mu_uint32 bufferSize);
	~CSNetworkObject();

	mu_boolean Send(mu_uint8 *buffer, mu_uint32 bufferSize);
	mu_boolean Send(MU_CLIENT_TO_SERVER_HEADEREX *packet);

protected:
	virtual mu_boolean OnConnect() override;
	virtual void OnDisconnect() override;

	virtual ElionNetwork::ELNetworkPacket *CreatePacket() override;

	virtual void OnReport(REPORT_TYPE reportType, mu_string report, mu_int32 errorCode) override;

public:
	void ProcessPackets();

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
		return true;
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

extern CSNetworkObject *g_ConnectServerObject;
extern MUBlowfish g_ConnectServerCrypt;

#endif