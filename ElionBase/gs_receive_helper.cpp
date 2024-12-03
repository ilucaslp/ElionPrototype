#include "stdafx.h"
#include "gs_networkobject.h"

void GSNetworkObject::OnReceiveHelperStarted(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_HELPERSTART ReceiveHelperStarted;

	if (ReceiveHelperStarted.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	g_IsHelperStarted = true;
	g_pHeroPosition->SetHelperButton(true);
}

void GSNetworkObject::OnReceiveHelperStopped(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_HELPERSTOP ReceiveHelperStopped;

	if (ReceiveHelperStopped.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	g_IsHelperStarted = false;
	g_pHeroPosition->SetHelperButton(false);
}