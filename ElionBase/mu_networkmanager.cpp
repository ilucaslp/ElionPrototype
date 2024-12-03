#include "stdafx.h"
#include "mu_networkmanager.h"

using namespace ElionNetwork;

ELNetworkManager *g_NetworkManager = nullptr;
MUNetworkClient *g_NetworkClient = nullptr;

mu_atomic_uint32_t g_NetworkNextState(NETWORKSTATE_NONE);
mu_uint32 g_NetworkCurrentState = NETWORKSTATE_NONE;
mu_double g_NetworkStatusFPS = 1000.0 / 1.0;
mu_double g_NetworkStatusElapsed = 0.0;
mu_uint32 g_NetworkStatusDots = 0;

MUNetworkClient::MUNetworkClient(ELNetworkManager *networkManager) : ELNetworkClient(networkManager)
{

}

MUNetworkClient::~MUNetworkClient()
{

}

void MUNetworkClient::OnReport(REPORT_TYPE reportType, mu_string report, mu_int32 errorCode)
{
#ifndef NDEBUG
	mu_printf(_T("Message ( ") SPF_STRING _T(" ) : ( %d / %u )\n"), report.c_str(), errorCode, errorCode); // sometimes the error code is unsigned.
#endif
}

void SetNetworkState(mu_uint32 nextState)
{
	g_NetworkNextState = nextState;
}

void ProcessNetworkState()
{
	mu_uint32 nextState = g_NetworkNextState.load();

	if (g_NetworkCurrentState != nextState)
	{
		g_NetworkCurrentState = nextState;

		g_NetworkStatusElapsed = 0.0;
		g_NetworkStatusDots = 0;

		switch (g_NetworkCurrentState)
		{
		case NETWORKSTATE_NONE:
			{
				g_pMessageRoot->Hide(MESSAGEBOX::INTERFACE_CONNECT);
				g_pMessageRoot->Hide(MESSAGEBOX::INTERFACE_DISCONNECTED);
			}
			break;

		case NETWORKSTATE_CONNECTING:
			{
				g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_CONNECT);
				g_pMessageRoot->Hide(MESSAGEBOX::INTERFACE_DISCONNECTED);
			}
			break;

		case NETWORKSTATE_CONNECTED:
			{
				g_pMessageRoot->Hide(MESSAGEBOX::INTERFACE_CONNECT);
				g_pMessageRoot->Hide(MESSAGEBOX::INTERFACE_DISCONNECTED);
			}
			break;

		case NETWORKSTATE_DISCONNECTED:
			{
				g_pMessageRoot->Hide(MESSAGEBOX::INTERFACE_CONNECT);
				g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_DISCONNECTED);
			}
			break;
		}
	}
}