#include "stdafx.h"
#include "gs_networkobject.h"

using namespace ElionNetwork;

mu_uint8 g_GameServerKey[CTS_ENCRYPTION_KEYSIZE] = { 0xb7, 0xee, 0xeb, 0x23, 0x2b, 0xee, 0xff, 0xf0, 0x42, 0x35, 0x3f, 0x9e, 0xdc, 0xa8, 0x74, 0xd1, 0xf8, 0x61, 0x78, 0xfa, 0x2f, 0xd1, 0xb2, 0x4a, 0x71, 0xb0, 0xcf, 0x9e, 0xf9, 0x96, 0x4a, 0x26, 0x26, 0xf2, 0x69, 0x40, 0x6f, 0x90, 0x70, 0x17, 0xd2, 0xce, 0xf5, 0xd3, 0xe4, 0x00, 0x80, 0x21, 0x97, 0xef, 0x73, 0xcc, 0xc2, 0x4c, 0xb3, 0x7c };
mu_uint8 g_GameServerIV[CTS_ENCRYPTION_BLOCKSIZE] = { 0xb5, 0xfd, 0x78, 0x8d, 0x50, 0xde, 0x24, 0xb2 };

GSNetworkObject *g_GameServerObject = nullptr;
MUBlowfish g_GameServerCrypt(g_GameServerKey, CTS_ENCRYPTION_KEYSIZE);
mu_boolean g_GameServerConnected = false;

GSNetworkObject::GSNetworkObject(ELNetworkManager *networkManager, mu_uint32 uniqueId, mu_uint32 bufferSize) : ELNetworkObject(networkManager, uniqueId, bufferSize)
{
	_PacketBuffer = nullptr;
	_PacketBufferSize = 0;
}

GSNetworkObject::~GSNetworkObject()
{
	MU_SAFE_FREE(_PacketBuffer);
}

mu_boolean GSNetworkObject::Send(mu_uint8 *buffer, mu_uint32 bufferSize)
{
	g_GameServerCrypt.Encode(buffer, buffer, bufferSize);

	MUAutoCriticalSpinLock lock(_PacketCritical);
	mu_uint32 totalSize = bufferSize + PACKET_MINIMUM_SIZE;

	if (totalSize > _PacketBufferSize)
	{
		_PacketBuffer = (mu_uint8*)mu_realloc(_PacketBuffer, totalSize);
		_PacketBufferSize = totalSize;
	}

	// Prevent unaligned access
	MU_PACKET_HEADER header;
	header.Size = totalSize;
	mu_memcpy(_PacketBuffer, &header, PACKET_MINIMUM_SIZE);
	mu_memcpy(_PacketBuffer + PACKET_MINIMUM_SIZE, buffer, bufferSize);

	return ELNetworkObject::Send(_PacketBuffer, header.Size);
}

mu_boolean GSNetworkObject::Send(MU_CLIENT_TO_SERVER_HEADEREX *packet)
{
	MUAutoCriticalSpinLock lock(_PacketCritical);

	mu_uint32 totalSize = packet->Length();

	if (totalSize + PACKET_MINIMUM_SIZE > _PacketBufferSize)
	{
		_PacketBuffer = (mu_uint8*)mu_realloc(_PacketBuffer, totalSize + PACKET_MINIMUM_SIZE);
		_PacketBufferSize = totalSize + PACKET_MINIMUM_SIZE;
	}

	if (packet->ToBuffer(_PacketBuffer + PACKET_MINIMUM_SIZE, totalSize) == false)
	{
		return false;
	}

	g_GameServerCrypt.Encode(_PacketBuffer + PACKET_MINIMUM_SIZE, _PacketBuffer + PACKET_MINIMUM_SIZE, totalSize);

	// Prevent unaligned access
	MU_PACKET_HEADER header;
	header.Size = totalSize + PACKET_MINIMUM_SIZE;
	mu_memcpy(_PacketBuffer, &header, PACKET_MINIMUM_SIZE);

	return ELNetworkObject::Send(_PacketBuffer, totalSize + PACKET_MINIMUM_SIZE);
}

mu_boolean GSNetworkObject::OnConnect()
{
	g_GameServerConnected = true;

	_PacketBuffer = (mu_uint8*)mu_realloc(_PacketBuffer, PACKET_BUFFER_SIZE);
	_PacketBufferSize = PACKET_BUFFER_SIZE;

	CL2GS_REQUEST_CLIENTALLOW RequestClientAllow;

	RequestClientAllow.Version = ELION_VERSION;
	RequestClientAllow.Serial = ELION_SERIAL;

	this->Send(&RequestClientAllow);

	return true;
}

void GSNetworkObject::OnDisconnect()
{
	if (_PacketBufferSize != 0)
	{
		mu_free(_PacketBuffer);
		_PacketBuffer = nullptr;
		_PacketBufferSize = 0;
	}
}

ELNetworkPacket *GSNetworkObject::CreatePacket()
{
	return new GSNetworkPacket();
}

void GSNetworkObject::OnReport(REPORT_TYPE reportType, mu_string report, mu_int32 errorCode)
{
#ifndef NDEBUG
	switch (reportType)
	{
	case REPORT_ERROR:
		mu_printf(_T("[GameServer Network Object] Error : ") SPF_STRING _T("\n"), report.c_str());
		break;

	case REPORT_WSAERROR:
		mu_printf(_T("[GameServer Network Object] WSA Error[%d] : ") SPF_STRING _T("\n"), errorCode, report.c_str());
		break;

	case REPORT_MESSAGE:
		mu_printf(_T("[GameServer Network Object] Message : ") SPF_STRING _T("\n"), report.c_str());
		break;

	case REPORT_WARNING:
		mu_printf(_T("[GameServer Network Object] Message : ") SPF_STRING _T("\n"), report.c_str());
		break;
	}
#endif
}