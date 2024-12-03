#include "stdafx.h"
#include "cs_networkobject.h"

using namespace ElionNetwork;

mu_uint8 g_ConnectServerKey[CTS_ENCRYPTION_KEYSIZE] = { 0xb7, 0xee, 0xeb, 0x23, 0x2b, 0xee, 0xff, 0xf0, 0x42, 0x35, 0x3f, 0x9e, 0xdc, 0xa8, 0x74, 0xd1, 0xf8, 0x61, 0x78, 0xfa, 0x2f, 0xd1, 0xb2, 0x4a, 0x71, 0xb0, 0xcf, 0x9e, 0xf9, 0x96, 0x4a, 0x26, 0x26, 0xf2, 0x69, 0x40, 0x6f, 0x90, 0x70, 0x17, 0xd2, 0xce, 0xf5, 0xd3, 0xe4, 0x00, 0x80, 0x21, 0x97, 0xef, 0x73, 0xcc, 0xc2, 0x4c, 0xb3, 0x7c };
mu_uint8 g_ConnectServerIV[CTS_ENCRYPTION_BLOCKSIZE] = { 0xb5, 0xfd, 0x78, 0x8d, 0x50, 0xde, 0x24, 0xb2 };

CSNetworkObject *g_ConnectServerObject = nullptr;
MUBlowfish g_ConnectServerCrypt(g_ConnectServerKey, CTS_ENCRYPTION_KEYSIZE);

CSNetworkObject::CSNetworkObject(ElionNetwork::ELNetworkManager *networkManager, mu_uint32 uniqueId, mu_uint32 bufferSize) : ELNetworkObject(networkManager, uniqueId, bufferSize)
{
	_PacketBuffer = nullptr;
	_PacketBufferSize = 0;
}

CSNetworkObject::~CSNetworkObject()
{
	MU_SAFE_FREE(_PacketBuffer);
}

mu_boolean CSNetworkObject::Send(mu_uint8 *buffer, mu_uint32 bufferSize)
{
	g_ConnectServerCrypt.Encode(buffer, buffer, bufferSize);

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

mu_boolean CSNetworkObject::Send(MU_CLIENT_TO_SERVER_HEADEREX *packet)
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

	g_ConnectServerCrypt.Encode(_PacketBuffer + PACKET_MINIMUM_SIZE, _PacketBuffer + PACKET_MINIMUM_SIZE, totalSize);

	// Prevent unaligned access
	MU_PACKET_HEADER header;
	header.Size = totalSize + PACKET_MINIMUM_SIZE;
	mu_memcpy(_PacketBuffer, &header, PACKET_MINIMUM_SIZE);

	return ELNetworkObject::Send(_PacketBuffer, totalSize + PACKET_MINIMUM_SIZE);
}

mu_boolean CSNetworkObject::OnConnect()
{
	_PacketBuffer = (mu_uint8*)mu_realloc(_PacketBuffer, PACKET_BUFFER_SIZE);
	_PacketBufferSize = PACKET_BUFFER_SIZE;

	return true;
}

void CSNetworkObject::OnDisconnect()
{
	if (_PacketBufferSize != 0)
	{
		mu_free(_PacketBuffer);
		_PacketBuffer = nullptr;
		_PacketBufferSize = 0;
	}
}

ELNetworkPacket *CSNetworkObject::CreatePacket()
{
	return new CSNetworkPacket();
}

void CSNetworkObject::OnReport(REPORT_TYPE reportType, mu_string report, mu_int32 errorCode)
{
#ifndef NDEBUG
	switch (reportType)
	{
	case REPORT_ERROR:
		mu_printf(_T("[ConnectServer Network Object] Error : ") SPF_STRING _T("\n"), report.c_str());
		break;

	case REPORT_WSAERROR:
		mu_printf(_T("[ConnectServer Network Object] WSA Error[%d] : ") SPF_STRING _T("\n"), errorCode, report.c_str());
		break;

	case REPORT_MESSAGE:
		mu_printf(_T("[ConnectServer Network Object] Message : ") SPF_STRING _T("\n"), report.c_str());
		break;

	case REPORT_WARNING:
		mu_printf(_T("[ConnectServer Network Object] Message : ") SPF_STRING _T("\n"), report.c_str());
		break;
	}
#endif
}

void CSNetworkObject::ProcessPackets()
{
	ELAutoCritical lock(GetSocketLock());

	if (IsConnected() == false)
	{
		return;
	}

	while (true)
	{
		CSNetworkPacket *packet = (CSNetworkPacket*)PopPacket();

		if (packet == nullptr)
		{
			break;
		}

		MU_CLIENT_TO_SERVER_HEADER Header;
		mu_memcpy(&Header, packet->GetBuffer() + PACKET_MINIMUM_SIZE, sizeof(Header));

		switch (Header.Code)
		{
		case CL2CS_PROT_GROUPLIST:
			{
				CS2CL_RECEIVE_GROUPLIST ParsedPacket;

				if (ParsedPacket.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
				{
#ifndef NDEBUG
					mu_printf(_T("Received corrupted packet!\n"));
#endif
					break;
				}

				SetNetworkState(NETWORKSTATE_CONNECTED);

				g_pServerList->Clear();

				for (auto it = ParsedPacket.GroupList.begin(); it != ParsedPacket.GroupList.end(); ++it)
				{
					CS2CL_RECEIVE_GROUPLIST::GROUP_LIST &group = *it;
					GROUP_INFO &groupInfo = g_pServerList->_ServerGroups[group.GroupIndex];

					groupInfo.Active = true;
					groupInfo.GroupName = group.GroupName;
				}

				g_pServerList->Setup();
			}
			break;

		case CL2CS_PROT_SERVERLIST:
			{
				CS2CL_RECEIVE_SERVERLIST ParsedPacket;

				if (ParsedPacket.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
				{
#ifndef NDEBUG
					mu_printf(_T("Received corrupted packet!\n"));
#endif
					break;
				}

				if (ParsedPacket.GroupIndex < MAX_GROUP_COUNT)
				{
					GROUP_INFO &groupInfo = g_pServerList->_ServerGroups[ParsedPacket.GroupIndex];

					g_pServerList->ClearServer(ParsedPacket.GroupIndex);

					mu_text nameBuffer[256];

					for (auto it = ParsedPacket.ServerList.begin(); it != ParsedPacket.ServerList.end(); ++it)
					{
						CS2CL_RECEIVE_SERVERLIST::SERVER_LIST &server = *it;
						SERVER_INFO &serverInfo = groupInfo.Servers[server.ServerIndex];

						serverInfo.Active = true;
						serverInfo.ServerType = server.ServerType;
						serverInfo.ServerFlag = server.ServerFlag;
						serverInfo.Percent = server.Percent;

						const mu_text *extraInfo = _T("");

						switch (serverInfo.Percent)
						{
						case CS2CL::SERVER_OFFLINE:
							extraInfo = _T("(Offline)");
							break;

						case 100:
							extraInfo = GlobalText[561];
							break;

						default:
							extraInfo = GlobalText[562];
							break;
						}

						switch (serverInfo.ServerType)
						{
						default:
						case CS2CL::SERVER_TYPE::ST_NORMAL:
							{
								if ((serverInfo.ServerFlag & CS2CL::SERVER_FLAG::SF_NONPVP) != 0)
								{
									mu_sprintf_s(nameBuffer, mu_countof(nameBuffer), _T("") SPF_STRING _T("-%u(Non-PVP) ") SPF_STRING _T(""), groupInfo.GroupName.c_str(), server.ServerIndex + 1, extraInfo);
								}
								else
								{
									mu_sprintf_s(nameBuffer, mu_countof(nameBuffer), _T("") SPF_STRING _T("-%u ") SPF_STRING _T(""), groupInfo.GroupName.c_str(), server.ServerIndex + 1, extraInfo);
								}
							}
							break;

						case CS2CL::SERVER_TYPE::ST_GOLD:
							{
								if ((serverInfo.ServerFlag & CS2CL::SERVER_FLAG::SF_NONPVP) != 0)
								{
									mu_sprintf_s(nameBuffer, mu_countof(nameBuffer), _T("") SPF_STRING _T("-%u(Gold) ") SPF_STRING _T(""), groupInfo.GroupName.c_str(), server.ServerIndex + 1, extraInfo);
								}
								else
								{
									mu_sprintf_s(nameBuffer, mu_countof(nameBuffer), _T("") SPF_STRING _T("-%u(Gold PVP) ") SPF_STRING _T(""), groupInfo.GroupName.c_str(), server.ServerIndex + 1, extraInfo);
								}
							}
							break;
						}

						serverInfo.ServerName = nameBuffer;
					}

					g_pServerList->Setup();
				}
			}
			break;

		case CL2CS_PROT_CONNECT:
			{
				CS2CL_RECEIVE_CONNECT ReceiveConnect;

				if (ReceiveConnect.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
				{
#ifndef NDEBUG
					mu_printf(_T("Received corrupted packet!\n"));
#endif
					break;
				}

				switch (ReceiveConnect.Response)
				{
				case CS2GS::CONNECT_RESPONSE::CR_ALLOWED:
					{
						g_GameServerConnected = false;

						if (g_NetworkClient->Connect(g_GameServerObject, ReceiveConnect.ServerIP.c_str(), ReceiveConnect.ServerPort) == false)
						{
							g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_FAILEDCONNECT);
							g_pLoginRoot->Show(LOGINUI::INTERFACE_SERVERLIST);
						}
					}
					break;

				case CS2GS::CONNECT_RESPONSE::CR_BLOCKED:
					{
						g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_BLOCKEDFROMSERVER);
					}
					break;

				case CS2GS::CONNECT_RESPONSE::CR_BANNED:
					{
						g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_BANNEDFROMSERVER);
					}
					break;

				case CS2GS::CONNECT_RESPONSE::CR_FULL:
					{
						g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_SERVERISFULL);
					}
					break;
				}

				switch (ReceiveConnect.Response)
				{
				case CS2GS::CONNECT_RESPONSE::CR_BLOCKED:
				case CS2GS::CONNECT_RESPONSE::CR_BANNED:
				case CS2GS::CONNECT_RESPONSE::CR_FULL:
					{
						g_pLoginRoot->Show(LOGINUI::INTERFACE_SERVERLIST);

						CL2CS_REQUEST_SERVERLIST RequestServerList;

						RequestServerList.GroupIndex = g_pServerList->_GroupSelected;

						g_ConnectServerObject->Send(&RequestServerList);
					}
					break;
				}
			}
			break;
		}

		delete packet;
	}
}