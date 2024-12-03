#include "stdafx.h"
#include "gs_networkobject.h"

void GSNetworkObject::OnReceiveChat(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_CHAT ReceiveChat;

	if (ReceiveChat.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	if (ReceiveChat.Message[0] == _T('~'))
	{
		g_pChatListBox->AddText(ReceiveChat.Name, &ReceiveChat.Message.c_str()[1], TYPE_PARTY_MESSAGE);
	}
	else if (ReceiveChat.Message[0] == '@' && ReceiveChat.Message[1] == '@')
	{
		g_pChatListBox->AddText(ReceiveChat.Name, &ReceiveChat.Message.c_str()[2], TYPE_UNION_MESSAGE);
	}
	else if (ReceiveChat.Message[0] == '@')
	{
		g_pChatListBox->AddText(ReceiveChat.Name, &ReceiveChat.Message.c_str()[1], TYPE_GUILD_MESSAGE);
	}
	else if (ReceiveChat.Message[0] == '#')
	{
		CHARACTER* pFindGm = nullptr;

		if (CharactersKeys[ReceiveChat.Key] != -1)
		{
			CHARACTER *c = &CharactersClient[CharactersKeys[ReceiveChat.Key]];
			OBJECT *o = &c->Object;

			if (o->Flags.Get(OBJECT::eOF_LIVE) &&
				o->Kind == KIND_PLAYER &&
				(g_isCharacterBuff((&c->Object), eBuff_GMEffect) ||
				(c->CtlCode == CTLCODE_ADMIN) ||
					(c->CtlCode == CTLCODE_EVENT_GM)))
			{
				pFindGm = c;
			}
		}

		if (pFindGm)
		{
			g_pSimpleChat->Assign(ReceiveChat.Key, ReceiveChat.Name.c_str(), &ReceiveChat.Message.c_str()[1]);
			g_pChatListBox->AddText(ReceiveChat.Name, &ReceiveChat.Message.c_str()[1], TYPE_GM_MESSAGE);
		}
		else
		{
			g_pSimpleChat->Assign(ReceiveChat.Key, ReceiveChat.Name.c_str(), &ReceiveChat.Message.c_str()[1], 1);
		}
	}
	else if (ReceiveChat.Message[0] == _T('!'))
	{
		g_pChatListBox->AddText(_T("[POST]") + ReceiveChat.Name, &ReceiveChat.Message.c_str()[1], TYPE_WHISPER_MESSAGE);
	}
	else
	{
		CHARACTER* pFindGm = nullptr;

		if (CharactersKeys[ReceiveChat.Key] != -1)
		{
			CHARACTER *c = &CharactersClient[CharactersKeys[ReceiveChat.Key]];
			OBJECT *o = &c->Object;

			if (o->Flags.Get(OBJECT::eOF_LIVE) &&
				o->Kind == KIND_PLAYER &&
				(g_isCharacterBuff((&c->Object), eBuff_GMEffect) ||
				(c->CtlCode == CTLCODE_ADMIN) ||
					(c->CtlCode == CTLCODE_EVENT_GM)))
			{
				pFindGm = c;
			}
		}

		if (pFindGm)
		{
			g_pSimpleChat->Assign(ReceiveChat.Key, ReceiveChat.Name.c_str(), ReceiveChat.Message.c_str());
			g_pChatListBox->AddText(ReceiveChat.Name, ReceiveChat.Message.c_str(), TYPE_GM_MESSAGE);
		}
		else
		{
			g_pSimpleChat->Assign(ReceiveChat.Key, ReceiveChat.Name.c_str(), ReceiveChat.Message.c_str());
			g_pChatListBox->AddText(ReceiveChat.Name, ReceiveChat.Message.c_str(), TYPE_CHAT_MESSAGE);
		}
	}
}

void GSNetworkObject::OnReceiveChatKey(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_CHATKEY ReceiveChatKey;

	if (ReceiveChatKey.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	mu_int32 Index = FindCharacterIndex(ReceiveChatKey.Key);

	if (Index != MAX_CHARACTERS_CLIENT)
	{
		CHARACTER *c = &CharactersClient[Index];
		g_pSimpleChat->Assign(ReceiveChatKey.Key, c->ID, ReceiveChatKey.Message.c_str());
	}
}

void GSNetworkObject::OnReceiveWhisper(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_WHISPER ReceiveWhisper;

	if (ReceiveWhisper.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	if (g_pChatInputBox->IsBlockWhisper() == true)
	{
		return;
	}

#if 0 // FIX
	RegistWhisperID(10, ID);
#endif

	if (g_pOption->IsWhisperSound())
	{
		PlayBackground(SOUND_WHISPER);
	}

	g_pChatListBox->AddText(ReceiveWhisper.Name, ReceiveWhisper.Message, TYPE_WHISPER_MESSAGE);
}

void GSNetworkObject::OnReceiveWhisperFail(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_WHISPERFAIL ReceiveWhisperFail;

	if (ReceiveWhisperFail.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	g_pChatListBox->AddText(ReceiveWhisperFail.Name, GlobalText[482], TYPE_ERROR_MESSAGE, TYPE_WHISPER_MESSAGE);
}

void GSNetworkObject::OnReceiveNotice(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_NOTICE ReceiveNotice;

	if (ReceiveNotice.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	switch (ReceiveNotice.Type)
	{
	case 0:
		{
			g_pSimpleNotice->Create(ReceiveNotice.Message, 0);
		}
		break;

	case 1:
		{
			switch (g_CurrentScene)
			{
			default:
			case LOG_IN_SCENE:
				{
					g_pLoginListBox->AddText(ReceiveNotice.Message);
				}
				break;

			case CHARACTER_SCENE:
				{
					g_pCharacterListBox->AddText(ReceiveNotice.Message);
				}
				break;

			case MAIN_SCENE:
				{
					g_pChatListBox->AddText(_T(""), ReceiveNotice.Message, TYPE_SYSTEM_MESSAGE);
				}
				break;
			}
		}
		break;

	case 2:
		{
			g_pSimpleNotice->Create(ReceiveNotice.Message, 1);
			g_pGuildWindow->SetGuildNotice(ReceiveNotice.Message);
		}
		break;

	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		{
#if 0 // FIX
			g_pSildeHelperMgr->AddSlide(ReceiveNotice.Count, ReceiveNotice.Delay, ReceiveNotice.Message, ReceiveNotice.Type - 10, ReceiveNotice.Speed / 10.0f, ReceiveNotice.Color);
#endif
		}
		break;
	}
}