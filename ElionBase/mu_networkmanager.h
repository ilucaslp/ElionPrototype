#ifndef __MU_NETWORKMANAGER_H__
#define __MU_NETWORKMANAGER_H__

#pragma once

#define NETWORK_BUFFER_SIZE (32 * 1024) // 32KB (Receive and Send buffer size)

class MUNetworkClient : public ElionNetwork::ELNetworkClient
{
public:
	explicit MUNetworkClient(ElionNetwork::ELNetworkManager *networkManager);
	~MUNetworkClient();

protected:
	virtual void OnReport(REPORT_TYPE reportType, mu_string report, mu_int32 errorCode) override;
};

extern ElionNetwork::ELNetworkManager *g_NetworkManager;
extern MUNetworkClient *g_NetworkClient;

enum : mu_uint32
{
	NETWORKSTATE_NONE = 0,
	NETWORKSTATE_CONNECTING,
	NETWORKSTATE_CONNECTED,
	NETWORKSTATE_DISCONNECTED,
};

extern mu_atomic_uint32_t g_NetworkNextState;
extern mu_uint32 g_NetworkCurrentState;
extern mu_double g_NetworkStatusFPS;
extern mu_double g_NetworkStatusElapsed;
extern mu_uint32 g_NetworkStatusDots;

void SetNetworkState(mu_uint32 nextState);
void ProcessNetworkState();

#endif