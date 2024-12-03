#include "stdafx.h"
#include "gs_networkobject.h"

void GSNetworkObject::OnReceiveEventEnter(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_EVENTENTER ReceiveEventEnter;

	if (ReceiveEventEnter.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	mu_text textBuffer[256];
	switch (ReceiveEventEnter.ResultCode)
	{
	case EVENT_ENTER::EE_KILLERRESTRICTED_BC:
		{
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[2043], GlobalText[56]);
			g_pOkBox->AddMessage(FONT_NORMAL, textBuffer);
			g_pOkBox->Configure();
		}
		break;
	case EVENT_ENTER::EE_TIMEPASSED_BC:
		{
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[852], GlobalText[1146]);
			g_pOkBox->AddMessage(FONT_NORMAL, textBuffer);
			g_pOkBox->Configure();
		}
		break;
	case EVENT_ENTER::EE_ITEMINVALID_BC:
		{
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[854]);
			g_pOkBox->Configure();
		}
		break;
	case EVENT_ENTER::EE_TOOWEAK_BC:
		{
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[687]);
			g_pOkBox->Configure();
		}
		break;
	case EVENT_ENTER::EE_TOOSTRONG_BC:
		{
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[686]);
			g_pOkBox->Configure();
		}
		break;
	case EVENT_ENTER::EE_FULL_BC:
		{
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[853], GlobalText[1146], MAX_BLOODCASTLE_USER_COUNT);
			g_pOkBox->AddMessage(FONT_NORMAL, textBuffer);
			g_pOkBox->Configure();
		}
		break;
	}
}

void GSNetworkObject::OnReceiveEventTimer(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_EVENTTIMER ReceiveEventTimer;

	if (ReceiveEventTimer.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	switch (ReceiveEventTimer.EventType)
	{
	case EVENT_TYPE::ERT_BLOODCASTLE:
		{
			g_pBloodCastleTimer->SetMatchCountDown(ReceiveEventTimer.TimerType);
		}
		break;
	}
}

void GSNetworkObject::OnReceiveEventRank(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_EVENTRANK ReceiveEventRank;

	if (ReceiveEventRank.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	switch (ReceiveEventRank.EventType)
	{
	case EVENT_TYPE::ERT_BLOODCASTLE:
		{
			for (mu_int32 i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
			{
				CharactersClient[i].EtcPart = 0;
			}

			if (ReceiveEventRank.List.empty() == false)
			{
				g_pBloodCastleRank->SetRankData(ReceiveEventRank.IsSucceed, *ReceiveEventRank.List.begin());
			}
			else
			{
				g_pBloodCastleRank->SetRankData(ReceiveEventRank.IsSucceed, GS2CL_RECEIVE_EVENTRANK_DATAEX::RankDataEx());
			}

			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_BLOODCASTLE_RANK);
		}
		break;
	}
}

void GSNetworkObject::OnReceiveEventState(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_EVENTSTATE ReceiveEventState;

	if (ReceiveEventState.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	switch (ReceiveEventState.EventType)
	{
	case EVENT_TYPE::ERT_BLOODCASTLE:
		{
			g_pBloodCastleTimer->SetMatchType(ReceiveEventState.PlayState);
			g_pBloodCastleTimer->SetTime(ReceiveEventState.RemainSec);
			g_pBloodCastleTimer->SetKillMonsterStatue(ReceiveEventState.CurKillMonster, ReceiveEventState.MaxKillMonster);

			switch (ReceiveEventState.PlayState)
			{
			case BC_STATE_START:
				{
					SetAllAction(PLAYER_RUSH1);
					PlayBackground(SOUND_BLOODCASTLE, true);
				}
				break;

			case BC_STATE_PLAY:
				{
					if (ReceiveEventState.UserHaveWeapon != -1 &&
						ReceiveEventState.WeaponNum != (mu_uint8)-1)
					{
						mu_int32 index = HangerBloodCastleQuestItem(ReceiveEventState.UserHaveWeapon);

						if (index != MAX_CHARACTERS_CLIENT)
						{
							CHARACTER *c = &CharactersClient[index];
							c->EtcPart = ReceiveEventState.WeaponNum;
						}
					}
				}
				break;

			case BC_STATE_END:
				{
					g_pBloodCastleTimer->Reset();
					StopBackground(SOUND_BLOODCASTLE);
				}
				break;

			case BC_STATE_MONSTEREND:
				{
					SetActionObject(GET_WORLD, 36, 20, 1.0f);
				}
				break;

			case BC_STATE_PLAY_BOSS:
				{
					if (ReceiveEventState.UserHaveWeapon != -1 &&
						ReceiveEventState.WeaponNum != (mu_uint8)-1)
					{
						mu_int32 index = HangerBloodCastleQuestItem(ReceiveEventState.UserHaveWeapon);

						if (index != MAX_CHARACTERS_CLIENT)
						{
							CHARACTER *c = &CharactersClient[index];
							c->EtcPart = ReceiveEventState.WeaponNum;
						}
					}
				}
				break;
			}
		}
		break;
	}
}