#include "stdafx.h"
#include "gs_networkobject.h"

using namespace ElionNetwork;

void GSNetworkObject::ProcessPackets()
{
	ELAutoCritical lock(GetSocketLock());

	if (IsConnected() == false)
	{
		return;
	}

	while (true)
	{
		GSNetworkPacket *packet = (GSNetworkPacket*)PopPacket();

		if (packet == nullptr)
		{
			break;
		}

		MU_CLIENT_TO_SERVER_HEADER Header;
		mu_memcpy(&Header, packet->GetBuffer() + PACKET_MINIMUM_SIZE, sizeof(Header));

		mu_boolean forceBreak = false;

		switch (Header.Code)
		{
		case GS2CL_PROT_CLIENTDENIED:
			OnReceiveClientDenied(packet);
			break;

		case GS2CL_PROT_LOGOUT:
			OnReceiveLogout(packet);
			break;

		case GS2CL_PROT_CHAT:
			OnReceiveChat(packet);
			break;

		case GS2CL_PROT_CHATKEY:
			OnReceiveChatKey(packet);
			break;

		case GS2CL_PROT_WHISPER:
			OnReceiveWhisper(packet);
			break;

		case GS2CL_PROT_WHISPERFAIL:
			OnReceiveWhisperFail(packet);
			break;

		case GS2CL_PROT_NOTICE:
			OnReceiveNotice(packet);
			break;

		case GS2CL_PROT_CONNECTED:
			OnReceiveConnected(packet);
			break;

		case GS2CL_PROT_LOGIN:
			forceBreak = OnReceiveLogin(packet);
			break;

		case GS2CL_PROT_CHARACTERLIST:
			OnReceiveCharacterList(packet);
			break;

		case GS2CL_PROT_CREATECHARACTER:
			OnReceiveCreateCharacter(packet);
			break;

		case GS2CL_PROT_DELETECHARACTER:
			OnReceiveDeleteCharacter(packet);
			break;

		case GS2CL_PROT_SELECTCHARACTER:
			forceBreak = OnReceiveSelectCharacter(packet);
			break;

		case GS2CL_PROT_DAMAGE:
			OnReceiveDamage(packet);
			break;

		case GS2CL_PROT_SIMPLEDAMAGE:
			OnReceiveSimpleDamage(packet);
			break;

		case GS2CL_PROT_KILLPLAYER:
			OnReceiveKillPlayer(packet);
			break;

		case GS2CL_PROT_ITEMINSERT:
			OnReceiveItemInsert(packet);
			break;

		case GS2CL_PROT_ITEMDELETE:
			OnReceiveItemDelete(packet);
			break;

		case GS2CL_PROT_ITEMMODIFY:
			OnReceiveItemModify(packet);
			break;

		case GS2CL_PROT_ITEMDURABILITY:
			OnReceiveItemDurability(packet);
			break;

		case GS2CL_PROT_ITEMMOVERESULT:
			OnReceiveItemMoveResult(packet);
			break;

		case GS2CL_PROT_SHOPLIST:
			OnReceiveShopList(packet);
			break;

		case GS2CL_PROT_TAXRATE:
			OnReceiveTaxRate(packet);
			break;

		case GS2CL_PROT_PICKITEM:
			OnReceivePickItem(packet);
			break;

		case GS2CL_PROT_PICKGOLD:
			OnReceivePickGold(packet);
			break;
			
		case GS2CL_PROT_RELOADARROW:
			OnReloadArrow(packet);
			break;

		case GS2CL_PROT_SERVERCOMMAND:
			OnServerCommand(packet);
			break;

		case GS2CL_PROT_GUILDJOIN_PETITION:
			OnRequestGuildJoin(packet);
			break;

		case GS2CL_PROT_PARTYREQUEST:
			OnRequestParty(packet);
			break;

		case GS2CL_PROT_PARTYKICK:
			OnLeaveParty(packet);
			break;

		case GS2CL_PROT_TALK:
			OnReceiveTalk(packet);
			break;

		case GS2CL_PROT_STORAGESTATE:
			OnReceiveStorageState(packet);
			break;

		case GS2CL_PROT_BUYITEM:
			OnReceiveBuy(packet);
			break;

		case GS2CL_PROT_SELLITEM:
			OnReceiveSell(packet);
			break;

		case GS2CL_PROT_MIX:
			OnReceiveMixResult(packet);
			break;

		case GS2CL_PROT_TRADEREQUEST:
			OnRequestTrade(packet);
			break;

		case GS2CL_PROT_TRADEANSWER:
			OnReceiveTradeAnswer(packet);
			break;

		case GS2CL_PROT_TRADERESULT:
			OnReceiveTradeResult(packet);
			break;

		case GS2CL_PROT_TRADEOK:
			OnReceiveTradeOk(packet);
			break;

		case GS2CL_PROT_CREATEGUILD:
			OnReceiveCreateGuild(packet);
			break;

		case GS2CL_PROT_GUILDINFO:
			OnReceiveGuildInfo(packet);
			break;

		case GS2CL_PROT_LEAVEGUILD:
			OnReceiveGuildLeave(packet);
			break;

		case GS2CL_PROT_QUESTANSWER:
			OnReceiveQuestAnswer(packet);
			break;

		case GS2CL_PROT_SETATTRIBUTE:
			OnReceiveSetAttribute(packet);
			break;

		case GS2CL_PROT_EVENTENTER:
			OnReceiveEventEnter(packet);
			break;

		case GS2CL_PROT_EVENTTIMER:
			OnReceiveEventTimer(packet);
			break;

		case GS2CL_PROT_EVENTRANK:
			OnReceiveEventRank(packet);
			break;

		case GS2CL_PROT_EVENTSTATE:
			OnReceiveEventState(packet);
			break;

		case GS2CL_PROT_HELPERSTART:
			OnReceiveHelperStarted(packet);
			break;

		case GS2CL_PROT_HELPERSTOP:
			OnReceiveHelperStopped(packet);
			break;

		case GS2CL_PROT_VIEWPORT_PACKAGE:
			OnReceiveViewportPackage(packet);
			break;
		}

		delete packet;

		if (forceBreak == true)
		{
			break;
		}
	}
}

void GSNetworkObject::OnReceiveClientDenied(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_CLIENTDENIED ReceiveClientDenied;

	if (ReceiveClientDenied.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}
	
	g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
	g_pOkBox->ClearMessages();
	g_pOkBox->AddMessage(FONT_NORMAL, _T("Client is out-of-date, please update it!"));
	g_pOkBox->Configure();
}

void GSNetworkObject::OnReceiveLogout(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_LOGOUT ReceiveLogout;

	if (ReceiveLogout.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	mu_int32 i;
	for (i = 0; i < 2; ++i)
	{
		Hero->Weapon[i].Type = -1;
		Hero->Weapon[i].Level = 0;
		Hero->Weapon[i].ExcOption = 0;
		Hero->Weapon[i].SetOption = 0;
	}
	for (i = 0; i < MAX_BODYPART; ++i)
	{
		Hero->BodyPart[i].Type = -1;
		Hero->BodyPart[i].Level = 0;
		Hero->BodyPart[i].ExcOption = 0;
		Hero->BodyPart[i].SetOption = 0;
	}

	Hero->Wing.Type = -1;
	Hero->Helper.Type = -1;
	Hero->Flag.Type = -1;

	switch (ReceiveLogout.Type)
	{
	case EREQUEST_LOGOUT::ERL_EXITGAME:
		{
			SDL_Event event;
			SDL_memset(&event, 0, sizeof(event));
			event.type = SDL_QUIT;
			SDL_PushEvent(&event);
		}
		break;

	case EREQUEST_LOGOUT::ERL_SERVER:
		{
			g_GameServerObject->Disconnect();

			SCENES::GameScene.SetNextScene(&SCENES::LoginScene);
			g_pLoginListBox->Clear();
		}
		break;

	case EREQUEST_LOGOUT::ERL_CHARACTER:
		{
			SCENES::GameScene.SetNextScene(&SCENES::CharacterScene);
			g_pCharacterListBox->Clear();
		}
		break;
	}
}

void GSNetworkObject::OnReceiveConnected(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_CONNECT ReceiveConnect;

	if (ReceiveConnect.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	g_pLoginRoot->Show(LOGINUI::INTERFACE_LOGINWINDOW);
	g_pLoginWindow->_PasswordBox->ClearAllText();
	g_pLoginWindow->_AccountBox->GiveFocus();
	g_pLoginWindow->_AccountBox->SelectAll();

	mu_sprintf8_s(g_PasswordSecret, mu_countof(g_PasswordSecret), PASSWORDSECRET_FORMAT, ReceiveConnect.PasswordSecret);
}

mu_boolean GSNetworkObject::OnReceiveLogin(GSNetworkPacket *packet)
{
	GS2CL_RESPONSE_LOGIN ResponseLogin;

	if (ResponseLogin.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return false;
	}

	switch (ResponseLogin.Response)
	{
	case GS2CL::LOGIN_RESPONSE::LR_SUCCESS:
		{
			SCENES::LoginScene.SetNextScene(&SCENES::CharacterScene);
			g_pCharacterListBox->Clear();

			for (mu_uint32 n = 0; n < MAX_CHARACTERLIST_COUNT; ++n)
			{
				g_pCharacterFrame->_Balloon[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			}

			g_pCharacterFrame->_CreateButton->ChangeState(UIButton::UI_DISABLED);
			g_pCharacterFrame->_ConnectButton->ChangeState(UIButton::UI_DISABLED);
			g_pCharacterFrame->_DeleteButton->ChangeState(UIButton::UI_DISABLED);

			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_WAITINGBOX);
			g_pWaitingBox->SetMessage(_T("Waiting for character list"));
		}
		break;

	default:
	case GS2CL::LOGIN_RESPONSE::LR_WRONG_LOGIN:
		{
			g_pLoginRoot->Show(LOGINUI::INTERFACE_LOGINWINDOW);
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, _T("Your account or password is invalid"));
			g_pOkBox->Configure();
			g_pLoginWindow->_AccountBox->GiveFocus();
			g_pLoginWindow->_AccountBox->SelectAll();
		}
		break;

	case GS2CL::LOGIN_RESPONSE::LR_ALREADY_CONNECTED:
		{
			g_pLoginRoot->Show(LOGINUI::INTERFACE_LOGINWINDOW);
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[415]);
			g_pOkBox->Configure();
			g_pLoginWindow->_AccountBox->GiveFocus();
			g_pLoginWindow->_AccountBox->SelectAll();
		}
		break;

	case GS2CL::LOGIN_RESPONSE::LR_WRONG_VERSION:
		{
			g_pLoginRoot->Show(LOGINUI::INTERFACE_LOGINWINDOW);
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			mu_string message = mu_string(GlobalText[405]) + _T("\n") + GlobalText[406];
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, message.c_str());
			g_pOkBox->Configure();
			g_pLoginWindow->_AccountBox->GiveFocus();
			g_pLoginWindow->_AccountBox->SelectAll();
		}
		break;

	case GS2CL::LOGIN_RESPONSE::LR_UNEXPECTED_ERROR:
		{
			g_pLoginRoot->Show(LOGINUI::INTERFACE_LOGINWINDOW);
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[20000]);
			g_pOkBox->Configure();
			g_pLoginWindow->_AccountBox->GiveFocus();
			g_pLoginWindow->_AccountBox->SelectAll();
		}
		break;

	case GS2CL::LOGIN_RESPONSE::LR_ATTEMPT_FAILED:
		{
			g_pLoginRoot->Show(LOGINUI::INTERFACE_LOGINWINDOW);
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[20000]);
			g_pOkBox->Configure();
			g_pLoginWindow->_AccountBox->GiveFocus();
			g_pLoginWindow->_AccountBox->SelectAll();
		}
		break;

	case GS2CL::LOGIN_RESPONSE::LR_HELPER_DISCONNECTING:
		{
			g_pLoginRoot->Show(LOGINUI::INTERFACE_LOGINWINDOW);
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[20001]);
			g_pOkBox->Configure();
			g_pLoginWindow->_AccountBox->GiveFocus();
			g_pLoginWindow->_AccountBox->SelectAll();
		}
		break;
	}

	return ResponseLogin.Response == GS2CL::LOGIN_RESPONSE::LR_SUCCESS;
}

void GSNetworkObject::OnReceiveCharacterList(GSNetworkPacket *packet)
{
	GS2CL_RESPONSE_CHARACTERLIST ResponseCharacterList;

	if (ResponseCharacterList.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	g_pMessageRoot->Hide(MESSAGEBOX::INTERFACE_WAITINGBOX);

	if (ResponseCharacterList.Characters.size() < MAX_CHARACTERLIST_COUNT)
	{
		g_pCharacterFrame->_CreateButton->ChangeState(UIButton::UI_NORMAL);
	}
	else
	{
		g_pCharacterFrame->_CreateButton->ChangeState(UIButton::UI_DISABLED);
	}

	for (mu_uint32 n = 0; n < GAME::MAX_CLASS; ++n)
	{
		if (ResponseCharacterList.EnabledClasses[n] == false)
		{
			g_pCreateCharacter->_ClassesButton[n]->ChangeState(UIRadioButton::UI_DISABLED);
		}
		else
		{
			g_pCreateCharacter->_ClassesButton[n]->ChangeState(UIRadioButton::UI_NORMAL);
		}
	}

	if (ResponseCharacterList.Characters.size() == 0)
	{
		if(g_pCharacterRoot->IsVisible(CHARACTERUI::INTERFACE_CREATECHARACTER) == false)
		{
			g_pCharacterRoot->Show(CHARACTERUI::INTERFACE_CREATECHARACTER);
			g_pCreateCharacter->SetSelectedClass(GAME::CLASS_KNIGHT);
			g_pCreateCharacter->_InputBox->ClearAllText();
			g_pCreateCharacter->_InputBox->GiveFocus();
		}
	}
	else
	{
		for (auto it = ResponseCharacterList.Characters.begin(); it != ResponseCharacterList.Characters.end(); ++it)
		{
			GS2CL_RESPONSE_CHARACTERLIST::CHARACTER_DATA &character = *it;

			mu_float Angle;
			EVector2 Position;

			switch (character.SlotIndex)
			{
			case 0:
				{
					Position = EVector2(8008.0f, 18885.0f);
					Angle = 115.0f;
				}
				break;

			case 1:
				{
					Position = EVector2(7986.0f, 19145.0f);
					Angle = 90.0f;
				}
				break;

			case 2:
				{
					Position = EVector2(8046.0f, 19400.0f);
					Angle = 75.0f;
				}
				break;

			case 3:
				{
					Position = EVector2(8133.0f, 19645.0f);
					Angle = 60.0f;
				}
				break;

			case 4:
			default:
				{
					Position = EVector2(8282.0f, 19845.0f);
					Angle = 35.0f;
				}
				break;
			}

			mu_uint8 Class = (character.Class & 0x07) | ((character.SubClass & 0x03) << 3);

			CHARACTER *c = CreateHero(0, character.SlotIndex + BEGIN_CHARACTERLIST_COUNT, Class, 0, Position.x, Position.y, Angle);

			c->Level = character.Level;
			c->GuildStatus = character.GuildStatus;
			c->PK = character.PkLevel;
			c->CtlCode = character.ControlCode;
			c->PositionX = Position.x * g_TerrainScaleRev;
			c->PositionY = Position.y * g_TerrainScaleRev;
			mu_strcpy_s(c->ID, mu_countof(c->ID), character.Name.c_str());

			ChangeCharacterExt(character.SlotIndex + BEGIN_CHARACTERLIST_COUNT, character.Equipment);

			SHARED::CHARSET_PACKET &WeaponLeft = character.Equipment[GAME::EQUIPMENT_WEAPON_LEFT];
			if (WeaponLeft.Enabled == true &&
				WeaponLeft.Type == 13 &&
				WeaponLeft.Index == 5 &&
				c->m_pPet != nullptr)
			{
				CSPetDarkSpirit* pPet = c->m_pPet;

				pPet->m_PetCharacter.Object.WeaponLevel = character.DarkRavenLevel;
			}

			g_pCharacterFrame->_Balloon[character.SlotIndex]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			g_pCharacterFrame->_Balloon[character.SlotIndex]->ForceUpdate();
		}
	}
}

void GSNetworkObject::OnReceiveCreateCharacter(GSNetworkPacket *packet)
{
	GS2CL_RESPONSE_CREATECHARACTER ResponseCreateCharacter;

	if (ResponseCreateCharacter.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	if (g_CurrentScene != CHARACTER_SCENE)
	{
		return;
	}

	g_pMessageRoot->Hide(MESSAGEBOX::INTERFACE_WAITINGBOX);

	switch (ResponseCreateCharacter.ResponseCode)
	{
	case GS2CL::CREATECHARACTER_RESPONSE::CCR_SUCCESS:
		{
			g_pCharacterRoot->Hide(CHARACTERUI::INTERFACE_CREATECHARACTER);

			for (mu_uint32 n = 0; n < GAME::MAX_CLASS; ++n)
			{
				if (ResponseCreateCharacter.EnabledClasses[n] == false)
				{
					g_pCreateCharacter->_ClassesButton[n]->ChangeState(UIRadioButton::UI_DISABLED);
				}
				else
				{
					g_pCreateCharacter->_ClassesButton[n]->ChangeState(UIRadioButton::UI_NORMAL);
				}
			}

			mu_float Angle;
			EVector2 Position;

			switch (ResponseCreateCharacter.SlotIndex)
			{
			case 0:
				{
					Position = EVector2(8008.0f, 18885.0f);
					Angle = 115.0f;
				}
				break;

			case 1:
				{
					Position = EVector2(7986.0f, 19145.0f);
					Angle = 90.0f;
				}
				break;

			case 2:
				{
					Position = EVector2(8046.0f, 19400.0f);
					Angle = 75.0f;
				}
				break;

			case 3:
				{
					Position = EVector2(8133.0f, 19645.0f);
					Angle = 60.0f;
				}
				break;

			case 4:
			default:
				{
					Position = EVector2(8282.0f, 19845.0f);
					Angle = 35.0f;
				}
				break;
			}

			mu_uint8 Class = (ResponseCreateCharacter.Class & 0x07) | ((ResponseCreateCharacter.SubClass & 0x03) << 3);

			CHARACTER *c = CreateHero(0, ResponseCreateCharacter.SlotIndex + BEGIN_CHARACTERLIST_COUNT, Class, 0, Position.x, Position.y, Angle);

			c->Level = ResponseCreateCharacter.Level;
			c->GuildStatus = ResponseCreateCharacter.GuildStatus;
			c->PK = ResponseCreateCharacter.PkLevel;
			c->CtlCode = ResponseCreateCharacter.ControlCode;
			c->PositionX = Position.x * g_TerrainScaleRev;
			c->PositionY = Position.y * g_TerrainScaleRev;
			mu_strcpy_s(c->ID, mu_countof(c->ID), ResponseCreateCharacter.Name.c_str());

			ChangeCharacterExt(ResponseCreateCharacter.SlotIndex + BEGIN_CHARACTERLIST_COUNT, ResponseCreateCharacter.Equipment);

			SHARED::CHARSET_PACKET &WeaponLeft = ResponseCreateCharacter.Equipment[GAME::EQUIPMENT_WEAPON_LEFT];
			if (WeaponLeft.Enabled == true &&
				WeaponLeft.Type == 13 &&
				WeaponLeft.Index == 5 &&
				c->m_pPet != nullptr)
			{
				CSPetDarkSpirit* pPet = c->m_pPet;

				pPet->m_PetCharacter.Object.WeaponLevel = ResponseCreateCharacter.DarkRavenLevel;
			}

			g_pCharacterFrame->_Balloon[ResponseCreateCharacter.SlotIndex]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			g_pCharacterFrame->_Balloon[ResponseCreateCharacter.SlotIndex]->ForceUpdate();

			mu_uint32 charactersCount = 0;

			for (mu_uint32 n = 0; n < MAX_CHARACTERLIST_COUNT; ++n)
			{
				if (CharactersClient[BEGIN_CHARACTERLIST_COUNT + n].Object.Flags.Get(OBJECT::eOF_LIVE) == true)
				{
					++charactersCount;
				}
			}

			if (charactersCount < MAX_CHARACTERLIST_COUNT)
			{
				g_pCharacterFrame->_CreateButton->ChangeState(UIButton::UI_NORMAL);
			}
			else
			{
				g_pCharacterFrame->_CreateButton->ChangeState(UIButton::UI_DISABLED);
			}
		}
		break;

	case GS2CL::CREATECHARACTER_RESPONSE::CCR_WRONG_NAME:
		{
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, _T("Character name is wrong"));
			g_pOkBox->Configure();
			g_pCreateCharacter->_InputBox->GiveFocus();
			g_pCreateCharacter->_InputBox->SelectAll();
		}
		break;

	case GS2CL::CREATECHARACTER_RESPONSE::CCR_NO_FREESLOT:
		{
			g_pCharacterRoot->Hide(CHARACTERUI::INTERFACE_CREATECHARACTER);

			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, _T("No slots available"));
			g_pOkBox->Configure();
			g_pCreateCharacter->_InputBox->GiveFocus();
			g_pCreateCharacter->_InputBox->SelectAll();
			g_pCharacterFrame->_CreateButton->ChangeState(GFX_FLAG::GFX_DISABLED);
		}
		break;

	case GS2CL::CREATECHARACTER_RESPONSE::CCR_ALREADYEXISTS:
		{
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, _T("Character name already exists"));
			g_pOkBox->Configure();
			g_pCreateCharacter->_InputBox->GiveFocus();
			g_pCreateCharacter->_InputBox->SelectAll();
		}
		break;

	default:
	case GS2CL::CREATECHARACTER_RESPONSE::CCR_UNEXPECTED_ERROR:
		{
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, _T("Unexpected error"));
			g_pOkBox->Configure();
			g_pCreateCharacter->_InputBox->GiveFocus();
			g_pCreateCharacter->_InputBox->SelectAll();
		}
		break;
	}
}

void GSNetworkObject::OnReceiveDeleteCharacter(GSNetworkPacket *packet)
{
	GS2CL_RESPONSE_DELETECHARACTER ResponseDeleteCharacter;

	if (ResponseDeleteCharacter.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	if (g_CurrentScene != CHARACTER_SCENE)
	{
		return;
	}

	g_pMessageRoot->Hide(MESSAGEBOX::INTERFACE_WAITINGBOX);

	switch (ResponseDeleteCharacter.ResponseCode)
	{
	case GS2CL::DELETECHARACTER_RESPONSE::DCR_SUCCESS:
		{
			if (ResponseDeleteCharacter.SlotIndex >= MAX_CHARACTERLIST_COUNT)
			{
				g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
				g_pOkBox->ClearMessages();
				g_pOkBox->AddMessage(FONT_NORMAL, _T("Failed to delete character"));
				g_pOkBox->Configure();
			}
			else
			{
				CHARACTER *c = &CharactersClient[static_cast<mu_uint32>(ResponseDeleteCharacter.SlotIndex) + BEGIN_CHARACTERLIST_COUNT];
				OBJECT *o = &c->Object;

				DeleteCharacter(c, o);

				SelectedHero = -1;

				g_pCharacterFrame->_Balloon[ResponseDeleteCharacter.SlotIndex]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
				g_pCharacterFrame->_CreateButton->ChangeState(UIButton::UI_NORMAL);
				g_pCharacterFrame->_ConnectButton->ChangeState(UIButton::UI_DISABLED);
				g_pCharacterFrame->_DeleteButton->ChangeState(UIButton::UI_DISABLED);
				
				g_pMessageRoot->Hide(MESSAGEBOX::INTERFACE_DELETECHARACTER);
				g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
				g_pOkBox->ClearMessages();
				g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[1714]);
				g_pOkBox->Configure();
			}
		}
		break;

	default:
	case GS2CL::DELETECHARACTER_RESPONSE::DCR_FAILED:
		{
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, _T("Failed to delete character"));
			g_pOkBox->Configure();
		}
		break;
	}
}

mu_boolean GSNetworkObject::OnReceiveSelectCharacter(GSNetworkPacket *packet)
{
	GS2CL_RESPONSE_SELECTCHARACTER ResponseSelectCharacter;

	if (ResponseSelectCharacter.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return false;
	}

	if (g_CurrentScene != WAITING_SCENE)
	{
		return false;
	}

	mu_uint8 Class = (ResponseSelectCharacter.Class & 0x07) | ((ResponseSelectCharacter.SubClass & 0x03) << 3);

	CharacterAttribute->Name = ResponseSelectCharacter.Name;
	CharacterAttribute->Class = Class;

	// Event Clear
	{
		g_pBloodCastleTimer->Reset();
	}

	if (g_Terrain.Load(ResponseSelectCharacter.Map, 0) == false)
	{
		Disconnect();
	}

	g_Tasking.ReleaseModelsMemory();

	if (InHellas() == true)
	{
		g_pCSWaterTerrain = new MUWaterTerrain();

		if (g_pCSWaterTerrain->Create() == false)
		{
			mu_assert(!_T("Failed to create water terrain!"));
		}
	}
	else
	{
		MU_SAFE_DELETE(g_pCSWaterTerrain);
	}

	HeroKey = ResponseSelectCharacter.Key;
	CHARACTER *c = &CharactersClient[HeroIndex];
	OBJECT *o = &c->Object;
	Hero = c;

	CreateCharacterPointer(c, MODEL_PLAYER, ResponseSelectCharacter.PositionX, ResponseSelectCharacter.PositionY, ResponseSelectCharacter.Direction);

	o->Flags.Set(OBJECT::eOF_LIVE);

	c->Key = HeroKey;
	CharactersKeys[HeroKey] = HeroIndex;
	c->Class = CharacterAttribute->Class;

	c->Skin = 0;
	c->PK = ResponseSelectCharacter.PkLevel;
	c->CtlCode = ResponseSelectCharacter.ControlCode;
	o->Kind = KIND_PLAYER;
	SetCharacterClass(0, c);

	mu_strcpy_s(c->ID, mu_countof(c->ID), CharacterAttribute->Name.c_str());

	for (mu_uint32 i = 0; i < GAME::MAX_EQUIPMENT; ++i)
	{
		CharacterMachine->Equipment[i].Type = -1;
		CharacterMachine->Equipment[i].Level = 0;
		CharacterMachine->Equipment[i].ExcOption = 0;
		CharacterMachine->Equipment[i].SetOption = 0;
	}

	CreateEffect(0, BITMAP_MAGIC + 2, o->Position, o->Angle, o->Light, 0, o);

	LoadingWorld = 30;
	MouseUpdateTime = 0;
	MouseUpdateTimeMax = 6;

	giPetManager::CreatePetDarkSpirit_Now(Hero);

	CreateEffect(0, BITMAP_MAGIC + 2, o->Position, o->Angle, o->Light, 0, o);
	o->Alpha = 0.0f;

	g_pNewUISystem->HideAll();

	SelectedItem = -1;
	SelectedNpc = -1;
	SelectedCharacter = -1;
	SelectedOperate = -1;
	Attacking = -1;

#if 0 // FIX
	RepairEnable = 0;
#endif

	Hero->Movement = false;
	SetPlayerStop(0, Hero);

	if (InBloodCastle() == false)
	{
		StopBackground(SOUND_BLOODCASTLE);
	}

	if (InChaosCastle() == false)
	{
		StopBackground(SOUND_CHAOSCASTLE);
		StopBackground(SOUND_CHAOS_ENVIR);
	}

	if (IsEmpireGuardian1() == false &&
		IsEmpireGuardian2() == false &&
		IsEmpireGuardian3() == false &&
		IsEmpireGuardian4() == false)
	{
		StopBackground(SOUND_EMPIREGUARDIAN_WEATHER_RAIN);
		StopBackground(SOUND_EMPIREGUARDIAN_WEATHER_FOG);
		StopBackground(SOUND_EMPIREGUARDIAN_WEATHER_STORM);
		StopBackground(SOUND_EMPIREGUARDIAN_INDOOR_SOUND);
	}

	g_pMapName->Show();

#if 0 // FIX
	CreateMyGensInfluenceGroundEffect();
#endif

	if (GET_WORLD >= WD_65DOPPLEGANGER1 && GET_WORLD <= WD_68DOPPLEGANGER4)
	{

	}
	else
	{
		mu_text Text[256];

		mu_sprintf_s(Text, mu_countof(Text), _T("") SPF_STRING _T("") SPF_STRING _T(""), GlobalText[484], GetMapName(GET_WORLD));

		g_pChatListBox->AddText(_T(""), Text, TYPE_SYSTEM_MESSAGE);
	}

#if 0 // FIX
	if (GET_WORLD == WD_30BATTLECASTLE)
	{
		if (g_pSiegeWarfare)
			g_pSiegeWarfare->CreateMiniMapUI();
	}
#endif

	if (GET_WORLD < WD_65DOPPLEGANGER1 || GET_WORLD > WD_68DOPPLEGANGER4)
	{
		g_pNewUISystem->Hide(GAMEUI::INTERFACE_DOPPELGANGER_FRAME);
	}

	if (GET_WORLD < WD_69EMPIREGUARDIAN1 || WD_72EMPIREGUARDIAN4 < GET_WORLD)
	{
		g_pNewUISystem->Hide(GAMEUI::INTERFACE_EMPIREGUARDIAN_TIMER);
	}

	mu_printf(_T("Received Select Character\n"));
	SCENES::WaitingScene.SetNextScene(&SCENES::GameScene);

	g_pSkillFrame->UpdateSkills();
	g_MoveManager.Update();

	g_IsHelperStarted = false;
	g_pHeroPosition->SetHelperButton(false);
	g_HelperSettings = ResponseSelectCharacter.HelperSetting;
	g_pHelperSetup->Configure(g_HelperSettings);

	return true;
}

void GSNetworkObject::OnReceiveDamage(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_DAMAGE ReceiveDamage;

	if (ReceiveDamage.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	if (InChaosCastle())
	{
		mu_int32 Index = FindCharacterIndex(ReceiveDamage.Key);

		if (Index != MAX_CHARACTERS_CLIENT)
		{
			CHARACTER *c = &CharactersClient[Index];
			OBJECT *o = &c->Object;

			mu_int32 DamageType = (ReceiveDamage.DamageType) & 0x3f;
			mu_boolean bDoubleEnable = (ReceiveDamage.DamageType >> 6) & 0x01;
			mu_boolean bComboEnable = (ReceiveDamage.DamageType >> 7) & 0x01;

			EVector3 Light;

			mu_int32 accumDamage = ReceiveDamage.ShieldDamage + ReceiveDamage.Damage;
			mu_int32 rstDamage = -1;
			mu_float rstScale = 0.8f;
			Vector(0.5f, 0.5f, 0.5f, Light);
			if (accumDamage > 0)
			{
				rstDamage = -2;
				if (accumDamage > 0 && accumDamage < 1000)
				{
					Vector(1.0f, 0.0f, 0.0f, Light);
				}
				else if (accumDamage >= 1000 && accumDamage < 3000)
				{
					Vector(0.95f, 0.7f, 0.15f, Light);
				}
				else if (accumDamage >= 3000)
				{
					Vector(0.95f, 0.7f, 0.15f, Light);
					rstScale = 1.5f;
				}
			}
			else
			{
				if (ReceiveDamage.Key == HeroKey)
				{
					Vector(1.0f, 1.0f, 1.0f, Light);
				}
				else
				{
					Vector(0.5f, 0.5f, 0.5f, Light);
				}
			}

			if (ReceiveDamage.MSBFlag)
			{
				SetPlayerShock(0, c, ReceiveDamage.Damage);
				Vector(1.0f, 0.0f, 0.0f, Light);
				if (ReceiveDamage.Key == HeroKey)
				{
					if (ReceiveDamage.Damage >= CharacterAttribute->Life)
						CharacterAttribute->Life = 0;
					else
						CharacterAttribute->Life -= ReceiveDamage.Damage;

					if (ReceiveDamage.ShieldDamage >= CharacterAttribute->Shield)
						CharacterAttribute->Shield = 0;
					else
						CharacterAttribute->Shield -= ReceiveDamage.ShieldDamage;

				}

				CreatePoint(o->Position, rstDamage, Light, rstScale);
			}
			else
			{
				if (ReceiveDamage.Key == HeroKey)
				{
					if (ReceiveDamage.Damage >= CharacterAttribute->Life)
						CharacterAttribute->Life = 0;
					else
						CharacterAttribute->Life -= ReceiveDamage.Damage;

					if (ReceiveDamage.ShieldDamage >= CharacterAttribute->Shield)
						CharacterAttribute->Shield = 0;
					else
						CharacterAttribute->Shield -= ReceiveDamage.ShieldDamage;

					if (g_isCharacterBuff(o, eBuff_PhysDefense) && o->Type == MODEL_PLAYER)
					{
						CHARACTER *cm = &CharactersClient[AttackPlayer];
						OBJECT *om = &cm->Object;
						mu_float fAngle = CreateAngle(om->Position[0], om->Position[1], o->Position[0], o->Position[1]);
						if (EMath::Abs(fAngle - om->Angle[2]) < 10.0f)
						{
							EVector3 Angle(0.0f, 0.0f, fAngle + 180.0f);
							CreateEffect(0, MODEL_MAGIC_CAPSULE2, o->Position, Angle, o->Light, 0, o);
						}
					}
				}
				else
				{
					if (c->MonsterIndex == 275);
					else if (GetLargeRand(2) == 0)
						SetPlayerShock(0, c, ReceiveDamage.Damage);
				}

				if (DamageType == 4)
				{
					Vector(1.0f, 0.0f, 1.0f, Light);
				}

				CreatePoint(o->Position, rstDamage, Light, rstScale);
			}

			c->Hit = ReceiveDamage.Damage;
		}
	}
	else
	{
		mu_int32 Index = FindCharacterIndex(ReceiveDamage.Key);

		if (Index != MAX_CHARACTERS_CLIENT)
		{
			CHARACTER *c = &CharactersClient[Index];
			OBJECT *o = &c->Object;

			mu_int32 DamageType = (ReceiveDamage.DamageType) & 0x3f;
			mu_boolean bDoubleEnable = (ReceiveDamage.DamageType >> 6) & 0x01;
			mu_boolean bComboEnable = (ReceiveDamage.DamageType >> 7) & 0x01;

			EVector3 Light;

			if (ReceiveDamage.MSBFlag)
			{
				SetPlayerShock(0, c, ReceiveDamage.Damage);
				Vector(1.0f, 0.0f, 0.0f, Light);

				CreatePoint(o->Position, ReceiveDamage.Damage, Light);

				if (ReceiveDamage.ShieldDamage > 0)
				{
					EVector3 nPosShieldDamage;
					Vector(0.8f, 1.0f, 0.0f, Light);
					nPosShieldDamage[0] = o->Position[0];
					nPosShieldDamage[1] = o->Position[1];
					nPosShieldDamage[2] = o->Position[2] + 25.0f;
					CreatePoint(nPosShieldDamage, ReceiveDamage.ShieldDamage, Light);
				}

				if (ReceiveDamage.Key == HeroKey)
				{
					if (ReceiveDamage.Damage >= CharacterAttribute->Life)
						CharacterAttribute->Life = 0;
					else
						CharacterAttribute->Life -= ReceiveDamage.Damage;

					if (ReceiveDamage.ShieldDamage >= CharacterAttribute->Shield)
						CharacterAttribute->Shield = 0;
					else
						CharacterAttribute->Shield -= ReceiveDamage.ShieldDamage;
				}
			}
			else
			{
				if (ReceiveDamage.Key == HeroKey)
				{
					if (ReceiveDamage.Damage >= CharacterAttribute->Life)
						CharacterAttribute->Life = 0;
					else
						CharacterAttribute->Life -= ReceiveDamage.Damage;

					if (ReceiveDamage.ShieldDamage >= CharacterAttribute->Shield)
						CharacterAttribute->Shield = 0;
					else
						CharacterAttribute->Shield -= ReceiveDamage.ShieldDamage;

					if (g_isCharacterBuff(o, eBuff_PhysDefense) && o->Type == MODEL_PLAYER)
					{
						CHARACTER *cm = &CharactersClient[AttackPlayer];
						OBJECT *om = &cm->Object;
						mu_float fAngle = CreateAngle(om->Position[0], om->Position[1], o->Position[0], o->Position[1]);
						if (EMath::Abs(fAngle - om->Angle[2]) < 10.0f)
						{
							EVector3 Angle(0.0f, 0.0f, fAngle + 180.0f);
							CreateEffect(0, MODEL_MAGIC_CAPSULE2, o->Position, Angle, o->Light, 0, o);
						}
					}
				}
				else
				{
					if (c->MonsterIndex == 275)
					{

					}
					else if (GetLargeRand(2) == 0)
					{
						SetPlayerShock(0, c, ReceiveDamage.Damage);
					}
				}

				mu_float scale = 15.0f;

				if (ReceiveDamage.Damage == 0)
				{
					if (ReceiveDamage.Key == HeroKey)
					{
						Vector(1.0f, 1.0f, 1.0f, Light);
					}
					else
					{
						Vector(0.5f, 0.5f, 0.5f, Light);
					}
				}
				else
				{
					switch (DamageType)
					{
					case 0:
						if (ReceiveDamage.Key == HeroKey)
						{
							Vector(1.0f, 0.0f, 0.0f, Light);
						}
						else
						{
							Vector(1.0f, 0.6f, 0.0f, Light);
						}
						break;
					case 1:
						scale = 50.0f;
						Vector(0.0f, 1.0f, 1.0f, Light);
						break;
					case 2:
						scale = 50.0f;
						Vector(0.0f, 1.0f, 0.6f, Light);
						break;
					case 3:
						scale = 50.0f;
						Vector(0.0f, 0.6f, 1.0f, Light);
						break;
					case 4:
						Vector(1.0f, 0.0f, 1.0f, Light);
						break;
					case 5:
						Vector(0.0f, 1.0f, 0.0f, Light);
						break;
					case 6:
						Vector(0.7f, 0.4f, 1.0f, Light);
						break;
					default:
						Vector(1.0f, 1.0f, 1.0f, Light);
						break;
					}

					if (bComboEnable)
					{
						scale = 50.0f;
					}
				}

				if (ReceiveDamage.Damage == 0)
				{
					CreatePoint(o->Position, -1, Light);
				}
				else
				{
					if (bComboEnable)
					{
						EVector3 Position, Light2;
						VectorCopy(o->Position, Position);
						Vector(Light[0] - 0.4f, Light[1] - 0.4f, Light[2] - 0.4f, Light2);
						CreatePoint(Position, ReceiveDamage.Damage, Light2, scale);
						Position[2] += 10.0f;
						Vector(Light[0] - 0.2f, Light[1] - 0.2f, Light[2] - 0.2f, Light2);
						CreatePoint(Position, ReceiveDamage.Damage, Light2, scale + 5.0f);
						Position[2] += 10.0f;
						CreatePoint(Position, ReceiveDamage.Damage, Light, scale + 10.0f);
					}
					else if (bDoubleEnable)
					{
						EVector3 Position, Light2;
						VectorCopy(o->Position, Position);
						Vector(Light[0] - 0.4f, Light[1] - 0.4f, Light[2] - 0.4f, Light2);
						CreatePoint(Position, ReceiveDamage.Damage, Light2, scale);
						Position[2] += 10.0f;
						Vector(Light[0] - 0.2f, Light[1] - 0.2f, Light[2] - 0.2f, Light2);
						CreatePoint(Position, ReceiveDamage.Damage, Light2, scale + 5.0f);
					}

					CreatePoint(o->Position, ReceiveDamage.Damage, Light, scale);
				}

				if (ReceiveDamage.ShieldDamage > 0)
				{
					EVector3 nPosShieldDamage;
					Vector(0.8f, 1.0f, 0.0f, Light);
					nPosShieldDamage[0] = o->Position[0];
					nPosShieldDamage[1] = o->Position[1];
					nPosShieldDamage[2] = o->Position[2] + 25.0f;
					CreatePoint(nPosShieldDamage, ReceiveDamage.ShieldDamage, Light);
				}
			}

			c->Hit = ReceiveDamage.Damage;
		}
	}
}

void GSNetworkObject::OnReceiveSimpleDamage(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_SIMPLEDAMAGE ReceiveSimpleDamage;

	if (ReceiveSimpleDamage.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	if (CharacterAttribute->Life >= ReceiveSimpleDamage.Damage)
		CharacterAttribute->Life -= ReceiveSimpleDamage.Damage;
	else
		CharacterAttribute->Life = 0.0f;

	if (CharacterAttribute->Shield >= ReceiveSimpleDamage.ShieldDamage)
		CharacterAttribute->Shield = ReceiveSimpleDamage.ShieldDamage;
	else
		CharacterAttribute->Shield = 0.0f;
}

void GSNetworkObject::OnReceiveKillPlayer(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_KILLPLAYER ReceiveKillPlayer;

	if (ReceiveKillPlayer.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	mu_int32 Index = FindCharacterIndex(ReceiveKillPlayer.Key);

	if (Index != MAX_CHARACTERS_CLIENT)
	{
		CHARACTER *c = &CharactersClient[Index];
		OBJECT *o = &c->Object;

		EVector3 Light;
		Vector(1.0f, 0.6f, 0.0f, Light);

		if (ReceiveKillPlayer.MSBFlag == false)
		{
			Hero->AttackFlag = ATTACK_DIE;
			Hero->Damage = ReceiveKillPlayer.Damage;
			Hero->TargetCharacter = Index;
		}

		CreatePoint(o->Position, ReceiveKillPlayer.Damage, Light);

		if (IsMasterLevel(Hero->Class) == true)
		{
			g_pMainFrame->SetPreMasterExperience(Master_Level_Data.lMasterLevel_Experince);
			g_pMainFrame->SetGetMasterExperience(ReceiveKillPlayer.Experience);

			Master_Level_Data.lMasterLevel_Experince += ReceiveKillPlayer.Experience;
		}
		else
		{
			g_pMainFrame->SetPreExperience(CharacterAttribute->Experience);
			g_pMainFrame->SetGetExperience(ReceiveKillPlayer.Experience);

			CharacterAttribute->Experience += ReceiveKillPlayer.Experience;
		}

		if (ReceiveKillPlayer.Experience > 0)
		{
			mu_text Text[100];

			if (IsMasterLevel(Hero->Class) == true)
			{
				mu_sprintf_s(Text, mu_countof(Text), GlobalText[1750], ReceiveKillPlayer.Experience);
			}
			else
			{
				mu_sprintf_s(Text, mu_countof(Text), GlobalText[486], ReceiveKillPlayer.Experience);
			}

			g_pChatListBox->AddText(_T(""), Text, TYPE_SYSTEM_MESSAGE);
		}
	}
}

void GSNetworkObject::OnReceiveItemInsert(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_ITEMINSERT ReceiveItemInsert;

	if (ReceiveItemInsert.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	SHARED::ITEM_DATA &Item = ReceiveItemInsert.ItemData;

	switch (Item.InventoryType)
	{
	case INVENTORY_TYPE::IT_EQUIPMENT:
		{
			if (Item.InventoryPosX < GAME::MAX_EQUIPMENT)
			{
				g_pMyInventory->EquipItem(Item.InventoryPosX, Item);

				switch (Item.InventoryPosX)
				{
				case GAME::EQUIPMENT_WING:
				case GAME::EQUIPMENT_HELPER:
				case GAME::EQUIPMENT_RING_RIGHT:
				case GAME::EQUIPMENT_RING_LEFT:
					{
						g_MoveManager.Update();
					}
					break;
				}

				g_pSkillFrame->UpdateSkills();
			}
		}
		break;

	case INVENTORY_TYPE::IT_INVENTORY:
		{
			if (Item.InventoryPosX < COLUMN_INVENTORY &&
				Item.InventoryPosY < ROW_INVENTORY)
			{
				g_pMyInventory->InsertItem(Item.InventoryPosX, Item.InventoryPosY, Item);
			}
		}
		break;

	case INVENTORY_TYPE::IT_STORAGE:
		{
			if (Item.InventoryPosX < COLUMN_STORAGE &&
				Item.InventoryPosY < ROW_STORAGE)
			{
				g_pStorageInventory->InsertItem(Item.InventoryPosX, Item.InventoryPosY, Item);
			}
		}
		break;

	case INVENTORY_TYPE::IT_MIX:
		{
			if (Item.InventoryPosX < COLUMN_MIX_INVENTORY &&
				Item.InventoryPosY < ROW_MIX_INVENTORY)
			{
				g_pMixInventory->InsertItem(Item.InventoryPosX, Item.InventoryPosY, Item);
				g_pMixInventory->UpdateMix();
			}
		}
		break;

	case INVENTORY_TYPE::IT_TRADE:
	case INVENTORY_TYPE::IT_YOURTRADE:
		{
			if (Item.InventoryPosX < COLUMN_TRADE_INVENTORY &&
				Item.InventoryPosY < ROW_TRADE_INVENTORY)
			{
				UIInventoryControl *inventoryControl = Item.InventoryType == INVENTORY_TYPE::IT_YOURTRADE ?
					g_pTrade->GetYourInventoryControl() :
					g_pTrade->GetMyInventoryControl();

				ITEM *NewItem = GenerateItem();

				ParseItem(Item, NewItem);

				if (inventoryControl->AddItem(Item.InventoryPosX, Item.InventoryPosY, NewItem) == false)
				{
					DestroyItem(NewItem);
				}

				if (Item.InventoryType == INVENTORY_TYPE::IT_YOURTRADE)
				{
					PlayBackground(SOUND_GET_ITEM01);
					g_pTrade->AlertYourTradeInventory();
				}
			}
		}
		break;
	}
}

void GSNetworkObject::OnReceiveItemDelete(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_ITEMDELETE ReceiveItemDelete;

	if (ReceiveItemDelete.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	switch (ReceiveItemDelete.Type)
	{
	case INVENTORY_TYPE::IT_EQUIPMENT:
		{
			if (ReceiveItemDelete.Slot < GAME::MAX_EQUIPMENT)
			{
				g_pMyInventory->UnequipItem(ReceiveItemDelete.Slot);

				switch (ReceiveItemDelete.Slot)
				{
				case GAME::EQUIPMENT_WING:
				case GAME::EQUIPMENT_HELPER:
				case GAME::EQUIPMENT_RING_RIGHT:
				case GAME::EQUIPMENT_RING_LEFT:
					{
						g_MoveManager.Update();
					}
					break;
				}

				g_pSkillFrame->UpdateSkills();
			}
		}
		break;

	case INVENTORY_TYPE::IT_INVENTORY:
		{
			if (ReceiveItemDelete.Slot < MAX_INVENTORY)
			{
				g_pMyInventory->DeleteItem(ReceiveItemDelete.Slot % COLUMN_INVENTORY, ReceiveItemDelete.Slot / COLUMN_INVENTORY);
			}
		}
		break;

	case INVENTORY_TYPE::IT_STORAGE:
		{
			if (ReceiveItemDelete.Slot < MAX_STORAGE)
			{
				g_pStorageInventory->DeleteItem(ReceiveItemDelete.Slot % COLUMN_STORAGE, ReceiveItemDelete.Slot / COLUMN_STORAGE);
			}
		}
		break;

	case INVENTORY_TYPE::IT_MIX:
		{
			if (ReceiveItemDelete.Slot < MAX_MIX_INVENTORY)
			{
				g_pMixInventory->DeleteItem(ReceiveItemDelete.Slot % COLUMN_MIX_INVENTORY, ReceiveItemDelete.Slot / COLUMN_MIX_INVENTORY);
				g_pMixInventory->UpdateMix();
			}
		}
		break;

	case INVENTORY_TYPE::IT_TRADE:
	case INVENTORY_TYPE::IT_YOURTRADE:
		{
			if (ReceiveItemDelete.Slot < MAX_TRADE_INVENTORY)
			{
				UIInventoryControl *inventoryControl = ReceiveItemDelete.Type == INVENTORY_TYPE::IT_YOURTRADE ?
					g_pTrade->GetYourInventoryControl() :
					g_pTrade->GetMyInventoryControl();

				ITEM* pItem = inventoryControl->FindItem(ReceiveItemDelete.Slot % COLUMN_TRADE_INVENTORY, ReceiveItemDelete.Slot / COLUMN_TRADE_INVENTORY);
				if (pItem != nullptr)
				{
					inventoryControl->RemoveItem(pItem);
				}

				if (ReceiveItemDelete.Type == INVENTORY_TYPE::IT_YOURTRADE)
				{
					PlayBackground(SOUND_GET_ITEM01);
					g_pTrade->AlertYourTradeInventory();
				}
			}
		}
		break;
	}
}

void GSNetworkObject::OnReceiveItemModify(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_ITEMMODIFY ReceiveItemModify;

	if (ReceiveItemModify.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	SHARED::ITEM_DATA &Item = ReceiveItemModify.ItemData;

	switch (Item.InventoryType)
	{
	case INVENTORY_TYPE::IT_EQUIPMENT:
		{
			if (Item.InventoryPosX < GAME::MAX_EQUIPMENT)
			{
				g_pMyInventory->EquipItem(Item.InventoryPosX, Item);
			}
		}
		break;

	case INVENTORY_TYPE::IT_INVENTORY:
		{
			if (Item.InventoryPosX < COLUMN_INVENTORY &&
				Item.InventoryPosY < ROW_INVENTORY)
			{
				if (g_pMyInventory->FindItem(Item.InventoryPosX, Item.InventoryPosY) != nullptr)
				{
					g_pMyInventory->DeleteItem(Item.InventoryPosX, Item.InventoryPosY);
				}

				g_pMyInventory->InsertItem(Item.InventoryPosX, Item.InventoryPosY, Item);
			}
		}
		break;

	case INVENTORY_TYPE::IT_STORAGE:
		{
			if (Item.InventoryPosX < COLUMN_STORAGE &&
				Item.InventoryPosY < ROW_STORAGE)
			{
				if (g_pStorageInventory->FindItem(Item.InventoryPosX, Item.InventoryPosY) != nullptr)
				{
					g_pStorageInventory->DeleteItem(Item.InventoryPosX, Item.InventoryPosY);
				}

				g_pStorageInventory->InsertItem(Item.InventoryPosX, Item.InventoryPosY, Item);
			}
		}
		break;

	case INVENTORY_TYPE::IT_MIX:
		{
			if (Item.InventoryPosX < COLUMN_MIX_INVENTORY &&
				Item.InventoryPosY < ROW_MIX_INVENTORY)
			{
				if (g_pMixInventory->FindItem(Item.InventoryPosX, Item.InventoryPosY) != nullptr)
				{
					g_pMixInventory->DeleteItem(Item.InventoryPosX, Item.InventoryPosY);
				}

				g_pMixInventory->InsertItem(Item.InventoryPosX, Item.InventoryPosY, Item);
				g_pMixInventory->UpdateMix();
			}
		}
		break;

	case INVENTORY_TYPE::IT_TRADE:
	case INVENTORY_TYPE::IT_YOURTRADE:
		{
			UIInventoryControl *inventoryControl = Item.InventoryType == INVENTORY_TYPE::IT_YOURTRADE ?
				g_pTrade->GetYourInventoryControl() :
				g_pTrade->GetMyInventoryControl();

			ITEM* pItem = inventoryControl->FindItem(Item.InventoryPosX, Item.InventoryPosY);
			if (pItem != nullptr)
			{
				inventoryControl->RemoveItem(pItem);
			}

			ITEM *NewItem = GenerateItem();

			ParseItem(Item, NewItem);

			if (inventoryControl->AddItem(Item.InventoryPosX, Item.InventoryPosY, NewItem) == false)
			{
				DestroyItem(NewItem);
			}

			if (Item.InventoryType == INVENTORY_TYPE::IT_YOURTRADE)
			{
				PlayBackground(SOUND_GET_ITEM01);
				g_pTrade->AlertYourTradeInventory();
			}
		}
		break;
	}
}

void GSNetworkObject::OnReceiveItemDurability(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_ITEMDURABILITY ReceiveItemDurability;

	if (ReceiveItemDurability.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	switch (ReceiveItemDurability.Type)
	{
	case INVENTORY_TYPE::IT_EQUIPMENT:
		{
			CharacterMachine->Equipment[ReceiveItemDurability.Slot].Durability = ReceiveItemDurability.Durability;
		}
		break;

	case INVENTORY_TYPE::IT_INVENTORY:
		{
			ITEM* pItem = g_pMyInventory->FindItem(ReceiveItemDurability.Slot);

			if (pItem)
			{
				pItem->Durability = ReceiveItemDurability.Durability;
			}
		}
		break;

	case INVENTORY_TYPE::IT_PERSONALSHOP:
		{
		}
		break;

	case INVENTORY_TYPE::IT_STORAGE:
		{
			ITEM* pItem = g_pStorageInventory->FindItem(ReceiveItemDurability.Slot);

			if (pItem)
			{
				pItem->Durability = ReceiveItemDurability.Durability;
			}
		}
		break;

	case INVENTORY_TYPE::IT_MIX:
		{
			ITEM* pItem = g_pMixInventory->FindItem(ReceiveItemDurability.Slot);

			if (pItem)
			{
				pItem->Durability = ReceiveItemDurability.Durability;
				g_pMixInventory->UpdateMix();
			}
		}
		break;
	}
}

void GSNetworkObject::OnReceiveShopList(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_SHOPLIST ReceiveShopList;

	if (ReceiveShopList.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	g_pNpcShopInventory->DeleteAllItems();

	for (auto it = ReceiveShopList.Items.begin(); it != ReceiveShopList.Items.end(); ++it)
	{
		SHARED::ITEM_DATA &item = *it;

		switch (item.InventoryType)
		{
		case INVENTORY_TYPE::IT_SHOP:
			{
				if (item.InventoryPosX < COLUMN_SHOP_INVENTORY &&
					item.InventoryPosY < ROW_SHOP_INVENTORY)
				{
					g_pNpcShopInventory->InsertItem(item.InventoryPosX, item.InventoryPosY, item);
				}
			}
			break;
		}
	}
}

void GSNetworkObject::OnReceiveTaxRate(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_TAXRATE ReceiveTaxRate;

	if (ReceiveTaxRate.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	switch (ReceiveTaxRate.Type)
	{
	case GS2CL_RECEIVE_TAXRATE::TAXRATE_CHAOS:
		{
			g_ChaosTaxRate = ReceiveTaxRate.TaxRate;
		}
		break;

	case GS2CL_RECEIVE_TAXRATE::TAXRATE_SHOP:
		{
			g_pNpcShopInventory->SetTaxRate(ReceiveTaxRate.TaxRate);
		}
		break;
	}
}

void GSNetworkObject::OnReceiveItemMoveResult(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_ITEMMOVERESULT ReceiveItemMoveResult;

	if (ReceiveItemMoveResult.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	if (g_pPickedItem.GetItem() != nullptr)
	{
		switch (ReceiveItemMoveResult.Type)
		{
		case INVENTORY_TYPE::IT_EQUIPMENT:
			{
				if (g_pPickedItem.GetOwnerInventory() == nullptr &&
					g_pPickedItem.GetItem()->ex_src_type == ITEM_EX_SRC_EQUIPMENT &&
					g_pPickedItem.GetItem()->lineal_pos == ReceiveItemMoveResult.X)
				{
					g_pPickedItem.BackupPickedItem();
				}
			}
			break;

		default:
			{
				if (g_pPickedItem.GetOwnerInventory() != nullptr &&
					g_pPickedItem.GetOwnerInventory()->GetInventoryType() == ReceiveItemMoveResult.Type &&
					g_pPickedItem.GetItem()->x == ReceiveItemMoveResult.X &&
					g_pPickedItem.GetItem()->y == ReceiveItemMoveResult.Y)
				{
					g_pPickedItem.BackupPickedItem();
				}
			}
			break;
		}
	}
}

void GSNetworkObject::OnReceivePickItem(GSNetworkPacket *packet)
{
	GS2CL_RESPONSE_PICKITEM ResponsePickItem;

	if (ResponsePickItem.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	for (auto it = ResponsePickItem.Items.begin(); it != ResponsePickItem.Items.end(); ++it)
	{
		SHARED::ITEM_DATA &ItemData = *it;

		if (ResponsePickItem.InsertItem == true)
		{
			switch (ItemData.InventoryType)
			{
			case INVENTORY_TYPE::IT_EQUIPMENT:
				{
					g_pMyInventory->EquipItem(ItemData.InventoryPosX, ItemData);
				}
				break;

			case INVENTORY_TYPE::IT_INVENTORY:
				{
					g_pMyInventory->InsertItem(ItemData.InventoryPosX, ItemData.InventoryPosY, ItemData);
				}
				break;
			}
		}

		mu_int32 Type = ItemData.ItemType * MAX_ITEM_INDEX + ItemData.ItemIndex;

		mu_text szItem[256] = { 0 };
		GetItemName(Type, ItemData.Level, szItem, mu_countof(szItem));

		mu_text szMessage[128];
		mu_sprintf_s(szMessage, mu_countof(szMessage), _T("") SPF_STRING _T(" ") SPF_STRING _T(""), szItem, GlobalText[918]);
		g_pChatListBox->AddText(_T(""), szMessage, TYPE_SYSTEM_MESSAGE);

		if (Type == ITEM_POTION + 13 || Type == ITEM_POTION + 14 || Type == ITEM_POTION + 16 || Type == ITEM_WING + 15 || Type == ITEM_POTION + 22
			|| Type == INDEX_COMPILED_CELE || Type == INDEX_COMPILED_SOUL || Type == ITEM_POTION + 31)
			PlayBackground(SOUND_JEWEL01);
		else if (Type == ITEM_POTION + 41)
			PlayBackground(SOUND_JEWEL02);
		else
			PlayBackground(SOUND_GET_ITEM01);
	}
}

void GSNetworkObject::OnReceivePickGold(GSNetworkPacket *packet)
{
	GS2CL_RESPONSE_PICKGOLD ResponsePickGold;

	if (ResponsePickGold.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	CharacterMachine->Gold += ResponsePickGold.Money;

	if (ResponsePickGold.Money > 0)
	{
		mu_text szMessage[256];
		mu_sprintf_s(szMessage, mu_countof(szMessage), _T("%lld ") SPF_STRING _T(" ") SPF_STRING _T(""), ResponsePickGold.Money, GlobalText[224], GlobalText[918]);
		g_pChatListBox->AddText(_T(""), szMessage, TYPE_SYSTEM_MESSAGE);

		g_MoveManager.Update();
	}
}

void GSNetworkObject::OnReloadArrow(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_RELOADARROW ReceiveReloadArrow;

	if (ReceiveReloadArrow.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	if (ReceiveReloadArrow.Success == true)
	{
		g_pChatListBox->AddText(_T(""), GlobalText[250], TYPE_SYSTEM_MESSAGE);
	}
	else
	{
		g_pChatListBox->AddText(_T(""), GlobalText[251], TYPE_ERROR_MESSAGE);
	}
}

void GSNetworkObject::OnServerCommand(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_SERVERCOMMAND ReceiveServerCommand;

	if (ReceiveServerCommand.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	switch (ReceiveServerCommand.CommandType)
	{
	case 0:
		{
			EVector3 Position, Angle(0.0f, 0.0f, 0.0f), Light(1.0f, 1.0f, 1.0f);
			Position[0] = ((mu_float)ReceiveServerCommand.CommandX + 0.5f)*g_TerrainScale;
			Position[1] = ((mu_float)ReceiveServerCommand.CommandY + 0.5f)*g_TerrainScale;
			Position[2] = RequestTerrainHeight(Position[0], Position[1]);
			CreateEffect(0, BITMAP_FIRECRACKER0001, Position, Angle, Light, 0);
		}
		break;

	case 1:
		{
			if (ReceiveServerCommand.CommandX >= 20)
			{
				g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
				g_pOkBox->ClearMessages();
				g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[830 + ReceiveServerCommand.CommandX - 20]);
				g_pOkBox->Configure();
			}
			else
			{
				g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
				g_pOkBox->ClearMessages();
				g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[650 + ReceiveServerCommand.CommandX]);
				g_pOkBox->Configure();
			}
		}
		break;

	case 2:
		{
			PlayBackground(SOUND_MEDAL);
		}
		break;

	case 3:
		{
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[710 + ReceiveServerCommand.CommandX]);
			g_pOkBox->Configure();
		}
		break;

	case 5:
		{
#if 0 // FIX
			CDialogMsgBox* pMsgBox = nullptr;
			CreateMessageBox(MSGBOX_LAYOUT_CLASS(CDialogMsgBoxLayout), &pMsgBox);
			if (pMsgBox)
			{
				pMsgBox->AddMsg(g_DialogScript[ReceiveServerCommand.CommandX].m_lpszText);
			}
#endif
		}
		break;

	case 6:
		{
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[449]);
			g_pOkBox->Configure();
		}
		break;

	case 7:
		if (ReceiveServerCommand.CommandX == 0)
		{
#if 0 // FIX
			ShowCheckBox(1, 2020, MESSAGE_PCROOM_EVENT);
#endif
		}
		else
		{
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2020 + ReceiveServerCommand.CommandX]);
			g_pOkBox->Configure();
		}
		break;

	case 13:
		{
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[1826]);
			g_pOkBox->Configure();
		}
		break;

	case 14:
		{
			switch (ReceiveServerCommand.CommandX)
			{
			case 0:
#if 0 // FIX
				CreateMessageBox(MSGBOX_LAYOUT_CLASS(CWhiteAngelEventLayout));
#endif
				break;

			case 1:
				{
					g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
					g_pOkBox->ClearMessages();
					g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2024]);
					g_pOkBox->Configure();
				}
				break;

			case 2:
				{
					g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
					g_pOkBox->ClearMessages();
					g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2022]);
					g_pOkBox->Configure();
				}
				break;

			case 3:
				{
					g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
					g_pOkBox->ClearMessages();
					g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2023]);
					g_pOkBox->Configure();
				}
				break;

			case 4:
				break;

			case 5:
				{
					g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
					g_pOkBox->ClearMessages();
					g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2023]);
					g_pOkBox->Configure();
				}
				break;
			}
		}
		break;

	case 15:
		{
			switch (ReceiveServerCommand.CommandX)
			{
			case 0:
				{
					g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
					g_pOkBox->ClearMessages();
					g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2022]);
					g_pOkBox->Configure();
				}
				break;
			case 1:
#if 0 // FIX
				CreateMessageBox(MSGBOX_LAYOUT_CLASS(CHarvestEventLayout));
#endif
				break;
			case 2:
				{
					g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
					g_pOkBox->ClearMessages();
					g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2023]);
					g_pOkBox->Configure();
				}
				break;
			}
		}
		break;

	case 16:
		{
#if 0 // FIX
			CNewUICommonMessageBox* pMsgBox = nullptr;
#endif

			switch (ReceiveServerCommand.CommandX)
			{
			case 0:
#if 0 // FIX
				CreateMessageBox(MSGBOX_LAYOUT_CLASS(CSantaTownSantaMsgBoxLayout), &pMsgBox);
				pMsgBox->AddMsg(GlobalText[2588]);
#endif
				break;
			case 1:
#if 0 // FIX
				CreateMessageBox(MSGBOX_LAYOUT_CLASS(CSantaTownSantaMsgBoxLayout), &pMsgBox);
				pMsgBox->AddMsg(GlobalText[2585]);
#endif
				break;
			case 2:
				{
					g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
					g_pOkBox->ClearMessages();
					g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2587]);
					g_pOkBox->Configure();
				}
				break;
			case 3:
				{
					g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
					g_pOkBox->ClearMessages();
					g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2023]);
					g_pOkBox->Configure();
				}
				break;
			}
		}
		break;

	case 17:
#if 0 // FIX
		CreateMessageBox(MSGBOX_LAYOUT_CLASS(CSantaTownLeaveMsgBoxLayout));
#endif
		break;

	case 47:
	case 48:
	case 49:
		{
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[1823 + ReceiveServerCommand.CommandType - 47]);
			g_pOkBox->Configure();
		}
		break;

	case 55:
		{
			mu_text buffer[256];
			mu_sprintf_s(buffer, mu_countof(buffer), GlobalText[2043], GlobalText[39]);
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, buffer);
			g_pOkBox->Configure();
		}
		break;

	case 56:
		{
			mu_text buffer[256];
			mu_sprintf_s(buffer, mu_countof(buffer), GlobalText[2043], GlobalText[56]);
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, buffer);
			g_pOkBox->Configure();
		}
		break;

	case 57:
		{
			mu_text buffer[256];
			mu_sprintf_s(buffer, mu_countof(buffer), GlobalText[2043], GlobalText[57]);
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, buffer);
			g_pOkBox->Configure();
		}
		break;

	case 58:
		{
			mu_int32 Index = FindCharacterIndex(ReceiveServerCommand.Key);
			if (Index >= 0 && Index < MAX_CHARACTERS_CLIENT)
			{
				OBJECT* to = &CharactersClient[Index].Object;
				if (to != nullptr)
				{
					CreateEffect(0, MODEL_EFFECT_SKURA_ITEM, to->Position, to->Angle, to->Light, 0, to);
					PlayObject(SOUND_CHERRYBLOSSOM_EFFECT0, to);
				}
			}
		}
		break;

	case 59:
		{
			EVector3 Position, Angle(0.0f, 0.0f, 0.0f), Light(1.0f, 1.0f, 1.0f);
			Position[0] = ((mu_float)ReceiveServerCommand.CommandX + 0.5f)*g_TerrainScale;
			Position[1] = ((mu_float)ReceiveServerCommand.CommandY + 0.5f)*g_TerrainScale;
			Position[2] = RequestTerrainHeight(Position[0], Position[1]);
			CreateEffect(0, BITMAP_FIRECRACKER0001, Position, Angle, Light, 1);
		}
		break;

	case 60:
#if 0 // FIX
		g_PortalMgr.ResetRevivePosition();
#endif
		break;

	case 61:
#if 0 // FIX
		g_PortalMgr.ResetPortalPosition();
#endif
		break;

	case 200:
		{
			PlayBackground(SOUND_REPAIR);
		}
		break;

	case 240:
		{
			switch (ReceiveServerCommand.CommandX)
			{
			case 0:
				PlayBackground(SOUND_RING_EVENT_READY);
				break;
			case 1:
				PlayBackground(SOUND_RING_EVENT_START);
				break;
			case 2:
				PlayBackground(SOUND_RING_EVENT_END);
				break;
			}
		}
		break;
	}
}

void GSNetworkObject::OnRequestGuildJoin(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_GUILDJOIN_PETITION ReceiveGuildJoinPetition;

	if (ReceiveGuildJoinPetition.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_GUILDREQUEST);
	g_pGuildRequestBox->SetPlayerKey(ReceiveGuildJoinPetition.Key);
}

void GSNetworkObject::OnRequestParty(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_PARTYREQUEST ReceivePartyRequest;

	if (ReceivePartyRequest.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_PARTYREQUEST);
	g_pPartyRequestBox->SetPlayerKey(ReceivePartyRequest.Key);
}

void GSNetworkObject::OnLeaveParty(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_PARTYKICK ReceivePartyKick;

	if (ReceivePartyKick.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	PartyNumber = 0;
	for (mu_uint32 n = 0; n < MAX_PARTYS; ++n)
	{
		Party[n].Clear();
	}
	g_pChatListBox->AddText(_T(""), GlobalText[502], TYPE_ERROR_MESSAGE);
}

void GSNetworkObject::OnReceiveTalk(GSNetworkPacket *packet)
{
	GS2CL_RESPONSE_TALK ResponseTalk;

	if (ResponseTalk.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	g_pNewUISystem->HideAll();

	switch (ResponseTalk.Response)
	{
	case TALK::NT_SHOP:
		{
			g_pNewUISystem->Show(GAMEUI::INTERFACE_NPCSHOP);
			g_pNpcShopInventory->SetShopType(UIShopInventory::SHOPTYPE_NORMAL);

			TALK::SHOP_DATA *ShopData = (TALK::SHOP_DATA*)ResponseTalk.TalkData.data();
			g_pNpcShopInventory->SetRepairShop(ShopData->IsRepairShop);
		}
		break;

	case TALK::NT_STORAGE:
		{
			g_pNewUISystem->Show(GAMEUI::INTERFACE_STORAGE);
		}
		break;

	case TALK::NT_MIX:
		{
			g_MixManager.SetMixType(MIXTYPE_GOBLIN_NORMAL);
			g_pNewUISystem->Show(GAMEUI::INTERFACE_MIXINVENTORY);
		}
		break;

	case TALK::NT_GUILDCREATE:
		{
			g_pNewUISystem->Show(GAMEUI::INTERFACE_NPCGUILDMASTER);
		}
		break;

	case TALK::NT_BLOODCASTLE:
		{
			g_pNewUISystem->Show(GAMEUI::INTERFACE_BLOODCASTLE);
		}
		break;

	case TALK::NT_JERRIDON:
		{
			g_MixManager.SetMixType(MIXTYPE_JERRIDON);
			g_pNewUISystem->Show(GAMEUI::INTERFACE_MIXINVENTORY);
		}
		break;

	case TALK::NT_CHAOSCARD:
		{
			g_MixManager.SetMixType(MIXTYPE_CHAOS_CARD);
			g_pNewUISystem->Show(GAMEUI::INTERFACE_MIXINVENTORY);
		}
		break;

	case TALK::NT_CHERRYBLOSSOM:
		{
			g_MixManager.SetMixType(MIXTYPE_CHERRYBLOSSOM);
			g_pNewUISystem->Show(GAMEUI::INTERFACE_MIXINVENTORY);
		}
		break;

	case TALK::NT_GAMBLE:
		{
			g_pNewUISystem->Show(GAMEUI::INTERFACE_NPCSHOP);
			g_pNpcShopInventory->SetShopType(UIShopInventory::SHOPTYPE_GAMBLE);
			g_pNpcShopInventory->SetRepairShop(false);
		}
		break;

	case TALK::NT_QUESTDIALOG:
		{
			TALK::QUEST_DATA *questData = (TALK::QUEST_DATA*)ResponseTalk.TalkData.data();

			g_QuestManager.SetCheckNpc(questData->CheckNpc);
			g_QuestManager.SetQuestList(questData->QuestIndex, questData->State);
			g_pNewUISystem->HideAll();
			g_pNewUISystem->Show(GAMEUI::INTERFACE_NPCQUEST);
		}
		break;
	}
}

void GSNetworkObject::OnReceiveStorageState(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_STORAGESTATE ReceiveStorageState;

	if (ReceiveStorageState.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	switch (ReceiveStorageState.State)
	{
	case STORAGE::SR_UNLOCKED:
		{
			g_pStorageInventory->ClearAuto();
			g_pStorageInventory->SetStorageLock(false);
			g_pStorageInventory->SetStorageAllowed(false);
		}
		break;

	case STORAGE::SR_LOCKED:
		{
			g_pStorageInventory->SetStorageLock(true);
			g_pStorageInventory->SetStorageAllowed(false);
		}
		break;

	case STORAGE::SR_INVALIDPASSWORD:
		{
			g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
			g_pOkBox->ClearMessages();
			g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[440]);
			g_pOkBox->Configure();
			g_pStorageInventory->ClearAuto();
			g_pStorageInventory->SetStorageLock(true);
			g_pStorageInventory->SetStorageAllowed(false);
		}
		break;

	case STORAGE::SR_ALLOWED:
		{
			g_pStorageInventory->ProcessAutoAction();
			g_pStorageInventory->SetStorageLock(true);
			g_pStorageInventory->SetStorageAllowed(true);
		}
		break;
	}
}

void GSNetworkObject::OnReceiveBuy(GSNetworkPacket *packet)
{
	GS2CL_RESPONSE_BUYITEM ReceiveBuyItem;

	if (ReceiveBuyItem.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	switch (ReceiveBuyItem.Result)
	{
	case GS2CL_RESPONSE_BUYITEM::BUY_FAILED:
		{
			g_pChatListBox->AddText(Hero->ID, GlobalText[732], TYPE_ERROR_MESSAGE);
		}
		break;

	case GS2CL_RESPONSE_BUYITEM::BUY_TIMEOUT:
		{
			g_pNewUISystem->HideAll();
			g_pChatListBox->AddText(Hero->ID, GlobalText[732], TYPE_ERROR_MESSAGE);
		}
		break;

	case GS2CL_RESPONSE_BUYITEM::BUY_SUCCEED:
		{
			PlayBackground(SOUND_GET_ITEM01);
		}
		break;
	}
}

void GSNetworkObject::OnReceiveSell(GSNetworkPacket *packet)
{
	GS2CL_RESPONSE_SELLITEM ReceiveSellItem;

	if (ReceiveSellItem.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	switch (ReceiveSellItem.Result)
	{
	case GS2CL_RESPONSE_SELLITEM::SELL_FAILED:
		{
			g_pChatListBox->AddText(Hero->ID, GlobalText[733], TYPE_ERROR_MESSAGE);
		}
		break;

	case GS2CL_RESPONSE_SELLITEM::SELL_TIMEOUT:
		{
			g_pNewUISystem->HideAll();
			g_pChatListBox->AddText(Hero->ID, GlobalText[733], TYPE_ERROR_MESSAGE);
		}
		break;

	case GS2CL_RESPONSE_SELLITEM::SELL_SUCCEED:
		{
			PlayBackground(SOUND_GET_ITEM01);
		}
		break;
	}
}

void GSNetworkObject::OnReceiveMixResult(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_MIX ReceiveMix;

	if (ReceiveMix.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	switch (ReceiveMix.Result)
	{
	case MIX::MIXR_FAILED:
		{
			g_pMixInventory->SetMixState(UIMixInventory::MIX_FINISHED);
			mu_text szText[256] = { 0, };
			switch (g_MixManager.GetMixType())
			{
			case MIXTYPE_GOBLIN_NORMAL:
			case MIXTYPE_GOBLIN_CHAOSITEM:
			case MIXTYPE_GOBLIN_ADD380:
			case MIXTYPE_EXTRACT_SEED:
			case MIXTYPE_SEED_SPHERE:
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[594]);
				g_pChatListBox->AddText(_T(""), szText, TYPE_ERROR_MESSAGE);
				break;
			case MIXTYPE_OSBOURNE:
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[2105], GlobalText[2061]);
				g_pChatListBox->AddText(_T(""), szText, TYPE_ERROR_MESSAGE);
				break;
			case MIXTYPE_JERRIDON:
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[2105], GlobalText[2062]);
				g_pChatListBox->AddText(_T(""), szText, TYPE_ERROR_MESSAGE);
				break;
			case MIXTYPE_ELPIS:
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[2112], GlobalText[2063]);
				g_pChatListBox->AddText(_T(""), szText, TYPE_ERROR_MESSAGE);
				break;
			case MIXTYPE_CHAOS_CARD:
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[2112], GlobalText[2265]);
				g_pChatListBox->AddText(_T(""), szText, TYPE_ERROR_MESSAGE);
				break;
			case MIXTYPE_CHERRYBLOSSOM:
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[2112], GlobalText[2560]);
				g_pChatListBox->AddText(_T(""), szText, TYPE_ERROR_MESSAGE);
				break;
			}
		}
		break;
	case MIX::MIXR_SUCCEED:
		{
			g_pMixInventory->SetMixState(UIMixInventory::MIX_FINISHED);
			mu_text szText[256] = { 0, };
			switch (g_MixManager.GetMixType())
			{
			case MIXTYPE_GOBLIN_NORMAL:
			case MIXTYPE_GOBLIN_CHAOSITEM:
			case MIXTYPE_GOBLIN_ADD380:
			case MIXTYPE_EXTRACT_SEED:
			case MIXTYPE_SEED_SPHERE:
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[595]);
				g_pChatListBox->AddText(_T(""), szText, TYPE_SYSTEM_MESSAGE);
				break;
			case MIXTYPE_OSBOURNE:
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[2106], GlobalText[2061]);
				g_pChatListBox->AddText(_T(""), szText, TYPE_SYSTEM_MESSAGE);
				break;
			case MIXTYPE_JERRIDON:
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[2106], GlobalText[2062]);
				g_pChatListBox->AddText(_T(""), szText, TYPE_SYSTEM_MESSAGE);
				break;
			case MIXTYPE_ELPIS:
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[2113], GlobalText[2063]);
				g_pChatListBox->AddText(_T(""), szText, TYPE_SYSTEM_MESSAGE);
				break;
			case MIXTYPE_CHAOS_CARD:
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[2113], GlobalText[2265]);
				g_pChatListBox->AddText(_T(""), szText, TYPE_SYSTEM_MESSAGE);
				break;
			case MIXTYPE_CHERRYBLOSSOM:
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[2113], GlobalText[2560]);
				g_pChatListBox->AddText(_T(""), szText, TYPE_SYSTEM_MESSAGE);
				break;
			}

			PlayBackground(SOUND_MIX01);
			PlayBackground(SOUND_JEWEL01);
		}
		break;
	case MIX::MIXR_GOLDERROR:
		{
			g_pMixInventory->SetMixState(UIMixInventory::MIX_WAITING);
			g_pChatListBox->AddText(_T(""), GlobalText[596], TYPE_ERROR_MESSAGE);
		}
		break;
	case MIX::MIXR_DSLEVELERROR:
		g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
		g_pOkBox->ClearMessages();
		g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[649]);
		g_pMixInventory->SetMixState(UIMixInventory::MIX_WAITING);
		break;
	case MIX::MIXR_BSLEVELERROR:
		g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
		g_pOkBox->ClearMessages();
		g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[689]);
		g_pMixInventory->SetMixState(UIMixInventory::MIX_WAITING);
		break;
	case MIX::MIXR_SUCCEED2:
		g_pMixInventory->SetMixState(UIMixInventory::MIX_FINISHED);
		break;
	case MIX::MIXR_BREAK:
		g_pMixInventory->SetMixState(UIMixInventory::MIX_FINISHED);
		PlayBackground(SOUND_MIX01);
		PlayBackground(SOUND_BREAK01);
		g_pMixInventory->DeleteAllItems();
		break;
	case MIX::MIXR_BREAKPET:
		g_pChatListBox->AddText(_T(""), GlobalText[1208], TYPE_ERROR_MESSAGE);
		PlayBackground(SOUND_MIX01);
		PlayBackground(SOUND_BREAK01);
		g_pMixInventory->SetMixState(UIMixInventory::MIX_FINISHED);
		g_pMixInventory->DeleteAllItems();
		break;

	case MIX::MIXR_DENIED:
	default:
		g_pMixInventory->SetMixState(UIMixInventory::MIX_WAITING);
		break;
	}
}

void GSNetworkObject::OnRequestTrade(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_TRADEREQUEST ReceiveTradeRequest;

	if (ReceiveTradeRequest.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_TRADEREQUEST);
	g_pTradeRequestBox->SetPlayerKey(ReceiveTradeRequest.Key);
}

void GSNetworkObject::OnReceiveTradeAnswer(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_TRADEANSWER ReceiveTradeAnswer;

	if (ReceiveTradeAnswer.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	g_pMessageRoot->Hide(MESSAGEBOX::INTERFACE_TRADEREQUEST);

	switch (ReceiveTradeAnswer.Result)
	{
	case TRADE_RESULT::RESULT_FAILED:
		{
			g_pChatListBox->AddText(_T(""), GlobalText[493], TYPE_ERROR_MESSAGE);
		}
		break;

	case TRADE_RESULT::RESULT_DENIED:
		{
			g_pChatListBox->AddText(_T(""), GlobalText[492], TYPE_ERROR_MESSAGE);
		}
		break;

	case TRADE_RESULT::RESULT_OK:
		{
			mu_int32 iIndex = FindCharacterIndex(ReceiveTradeAnswer.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT)
			{
				this->SendCloseWindow();
				break;
			}

			CHARACTER* c = &CharactersClient[iIndex];

			g_pNewUISystem->Show(GAMEUI::INTERFACE_TRADE);

			mu_string tmpName = g_pTrade->_YourName;

			g_pTrade->ResetInfo();
			g_pTrade->SetMyData(Hero->ID, Hero->Level);
			g_pTrade->SetYourData(c->ID, ReceiveTradeAnswer.Level, ReceiveTradeAnswer.GuildKey);

			if (g_pTrade->IsAlertTrade() == false && mu_strcmp(Hero->ID, tmpName.c_str()) != 0)
			{
				g_pTrade->InitYourInventoryBackup();
			}

			g_pTrade->SetAlertTrade(false);
		}
		break;
	}
}

void GSNetworkObject::OnReceiveTradeResult(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_TRADERESULT ReceiveTradeResult;

	if (ReceiveTradeResult.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	g_pNewUISystem->Hide(GAMEUI::INTERFACE_INVENTORY);

	switch (ReceiveTradeResult.Result)
	{
	case TRADE_RESULT::RESULT_FAILED:
	case TRADE_RESULT::RESULT_CANCELLED:
		{
			g_pChatListBox->AddText(_T(""), GlobalText[492], TYPE_ERROR_MESSAGE);

			g_pTrade->SetAlertTrade(false);
			g_pTrade->BackupYourInventory();
		}
		break;

	case TRADE_RESULT::RESULT_INVENTORYFULL:
		{
			g_pChatListBox->AddText(_T(""), GlobalText[495], TYPE_ERROR_MESSAGE);
		}
		break;

	case TRADE_RESULT::RESULT_BLOCKEDITEM:
		{
			g_pChatListBox->AddText(_T(""), GlobalText[2108], TYPE_ERROR_MESSAGE);
		}
		break;
	}
}

void GSNetworkObject::OnReceiveTradeOk(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_TRADEOK ReceiveTradeOk;

	if (ReceiveTradeOk.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	g_pTrade->ChangeYourOkButton(ReceiveTradeOk.Result, ReceiveTradeOk.WaitTime);
}

void GSNetworkObject::OnReceiveCreateGuild(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_CREATEGUILD ReceiveCreateGuild;

	if (ReceiveCreateGuild.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	switch (ReceiveCreateGuild.Result)
	{
	case CREATEGUILD_RESULT::RESULT_SUCCESS:
		{
			// guild created...
		}
		break;

	case CREATEGUILD_RESULT::RESULT_FAILED:
		{
			g_pChatListBox->AddText(_T(""), _T("unexpected server error."), TYPE_ERROR_MESSAGE);
		}
		break;

	case CREATEGUILD_RESULT::RESULT_ALREADYEXISTS:
		{
			g_pChatListBox->AddText(_T(""), GlobalText[516], TYPE_ERROR_MESSAGE);
		}
		break;

	case CREATEGUILD_RESULT::RESULT_TOOSHORT:
		{
			g_pChatListBox->AddText(_T(""), GlobalText[517], TYPE_ERROR_MESSAGE);
		}
		break;

	case CREATEGUILD_RESULT::RESULT_ALREADYINGUILD:
		{
			g_pChatListBox->AddText(_T(""), GlobalText[518], TYPE_ERROR_MESSAGE);
		}
		break;

	case CREATEGUILD_RESULT::RESULT_INVALIDNAME:
		{
			g_pChatListBox->AddText(_T(""), GlobalText[940], TYPE_ERROR_MESSAGE);
		}
		break;

	case CREATEGUILD_RESULT::RESULT_RESERVEDNAME:
		{
			g_pChatListBox->AddText(_T(""), GlobalText[942], TYPE_ERROR_MESSAGE);
		}
		break;
	}
}

void GSNetworkObject::OnReceiveGuildInfo(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_GUILDINFO ReceiveGuildInfo;

	if (ReceiveGuildInfo.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	g_GuildCache.SetGuildMark(ReceiveGuildInfo.GuildIndex, ReceiveGuildInfo.GuildUnion, ReceiveGuildInfo.GuildName, ReceiveGuildInfo.MarkBuffer);
}

void GSNetworkObject::OnReceiveGuildLeave(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_LEAVEGUILD ReceiveLeaveGuild;

	if (ReceiveLeaveGuild.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	switch (ReceiveLeaveGuild.Result)
	{
	case LEAVEGUILD_RESULT::RESULT_FAILED:
		{
			g_pChatListBox->AddText(_T(""), _T("Unexpected server error"), TYPE_ERROR_MESSAGE);
		}
		break;

	case LEAVEGUILD_RESULT::RESULT_SUCCESS_DISBAND:
		{
			g_pChatListBox->AddText(_T(""), GlobalText[515], TYPE_ERROR_MESSAGE);

			g_pGuildWindow->ClearAlliances();
			g_pGuildWindow->ClearMembers();
		}
		break;

	case LEAVEGUILD_RESULT::RESULT_SUCCESS_LEAVE:
		{
			if (ReceiveLeaveGuild.Key != -1 &&
				ReceiveLeaveGuild.Key == HeroKey)
			{
				g_pChatListBox->AddText(_T(""), GlobalText[512], TYPE_ERROR_MESSAGE);
				g_pGuildWindow->ClearAlliances();
				g_pGuildWindow->ClearMembers();
			}
			else
			{
				g_pChatListBox->AddText(_T(""), GlobalText[568], TYPE_ERROR_MESSAGE);
				g_pGuildWindow->DeleteMember(ReceiveLeaveGuild.MemberIndex);
			}
		}
		break;
	}
}

void GSNetworkObject::OnReceiveQuestAnswer(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_QUESTANSWER ReceiveQuestAnswer;

	if (ReceiveQuestAnswer.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	if (ReceiveQuestAnswer.Result == 0)
	{
		g_QuestManager.SetQuestList(ReceiveQuestAnswer.QuestIndex, ReceiveQuestAnswer.State);
		g_pNewUISystem->HideAll();
		g_pNewUISystem->Show(GAMEUI::INTERFACE_NPCQUEST);
	}
}

void GSNetworkObject::OnReceiveSetAttribute(GSNetworkPacket *packet)
{
	GS2CL_RECEIVE_SETATTRIBUTE ReceiveSetAttribute;

	if (ReceiveSetAttribute.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	switch (ReceiveSetAttribute.Type)
	{
	case 0:
		{
			if (InBloodCastle() == true &&
				ReceiveSetAttribute.MapAttr == TW_NOGROUND)
			{
				SetActionObject(GET_WORLD, 36, 0, 1.0f);
			}

			for (mu_size k = 0; k < ReceiveSetAttribute.Positions.size(); k += 2)
			{
				auto &PosBegin = ReceiveSetAttribute.Positions[k + 0];
				auto &PosEnd = ReceiveSetAttribute.Positions[k + 1];

				mu_int32 dx = PosEnd.x - PosBegin.x + 1;
				mu_int32 dy = PosEnd.y - PosBegin.y + 1;

				AddTerrainAttributeRange(PosBegin.x, PosBegin.y, dx, dy, ReceiveSetAttribute.MapAttr, ReceiveSetAttribute.MapSetType == 0 ? true : false);
			}
		}
		break;

	case 1:
		{
			for (mu_int32 k = 0; k < ReceiveSetAttribute.Positions.size(); ++k)
			{
				auto &Pos = ReceiveSetAttribute.Positions[k];

				if (ReceiveSetAttribute.MapSetType > 0)
				{
					SubTerrainAttribute(Pos.x, Pos.y, ReceiveSetAttribute.MapAttr);
				}
				else
				{
					AddTerrainAttribute(Pos.x, Pos.y, ReceiveSetAttribute.MapAttr);
				}
			}
		}
		break;
	}
}

#define MAX_DECOMPRESSED_BUFFER (2 * 1024 * 1024)
mu_uint8 DecompressedBuffer[MAX_DECOMPRESSED_BUFFER];

void GSNetworkObject::OnReceiveViewportPackage(GSNetworkPacket *packet)
{
	GS2CL_VIEWPORT_PACKAGE ViewportPackage;

	if (ViewportPackage.FromBuffer(packet->GetBuffer() + PACKET_MINIMUM_SIZE, packet->GetPacketSize() - PACKET_MINIMUM_SIZE) == false)
	{
#ifndef NDEBUG
		mu_printf(_T("Received corrupted packet!\n"));
#endif
		return;
	}

	mu_uint32 ProcessedSize = 0;
	mu_uint8 *PacketBuffer = nullptr;

	if (ViewportPackage.Compressed == true)
	{
		if (FileHeaders::Decompress(packet->GetBuffer() + PACKET_MINIMUM_SIZE + ViewportPackage.Length(),
			DecompressedBuffer,
			ViewportPackage.CompressedSize,
			ViewportPackage.PackageSize) == false)
		{
			return;
		}

		PacketBuffer = DecompressedBuffer;
	}
	else
	{
		PacketBuffer = packet->GetBuffer() + PACKET_MINIMUM_SIZE + ViewportPackage.Length();
	}

	while (ProcessedSize < ViewportPackage.PackageSize)
	{
		MU_VIEWPORT_HEADER *Header = (MU_VIEWPORT_HEADER*)PacketBuffer;

		InternalViewportProcess(PacketBuffer);

		PacketBuffer += Header->Size;
		ProcessedSize += Header->Size;
	}
}

EINLINE void InternalViewportProcess(mu_uint8 *PacketBuffer, mu_boolean InternalUsage)
{
	MU_VIEWPORT_HEADER *Header = (MU_VIEWPORT_HEADER*)PacketBuffer;

	switch (Header->Code)
	{
	case GS2CL_PROT_VIEWPORT_CREATECHARACTER:
		{
			GS2CL_VIEWPORT_CREATECHARACTER ViewportCreateCharacter;

			if (ViewportCreateCharacter.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_uint8 Class = (ViewportCreateCharacter.Class & 0x07) | ((ViewportCreateCharacter.SubClass & 0x03) << 3);

			CHARACTER *c = CreateCharacter(ViewportCreateCharacter.Key, MODEL_PLAYER, ViewportCreateCharacter.PositionX, ViewportCreateCharacter.PositionY, 0);
			DeleteCloth(c, &c->Object);

			for (mu_uint32 n = 0; n < PartyNumber; ++n)
			{
				PARTY_t &member = Party[n];

				if (member.Key == ViewportCreateCharacter.Key)
				{
					member.index = FindCharacterIndex(ViewportCreateCharacter.Key);
					break;
				}
			}

			OBJECT *o = &c->Object;
			c->Class = Class;
			c->Skin = 0;
			c->PK = ViewportCreateCharacter.PkLevel;
			o->Kind = KIND_PLAYER;

			c->PositionX = ViewportCreateCharacter.PositionX;
			c->PositionY = ViewportCreateCharacter.PositionY;
			c->TargetX = ViewportCreateCharacter.PositionX;
			c->TargetY = ViewportCreateCharacter.PositionY;

			c->Object.Angle[2] = ViewportCreateCharacter.Direction;

			c->Object.Position[0] = ((c->PositionX) + 0.5f)*g_TerrainScale;
			c->Object.Position[1] = ((c->PositionY) + 0.5f)*g_TerrainScale;

			c->EnabledInternalAction = INTERNAL_ACTION::IA_NONE;

			if (ViewportCreateCharacter.Create == true)
			{
				CreateEffect(0, BITMAP_MAGIC + 2, o->Position, o->Angle, o->Light, 0, o);
				c->Object.Alpha = 0.0f;
			}

			if (InHellas())
			{
				CreateJoint(0, BITMAP_FLARE + 1, o->Position, o->Position, o->Angle, 8, o, 20.0f);
			}

			ChangeCharacterExt(FindCharacterIndex(ViewportCreateCharacter.Key), ViewportCreateCharacter.Equipment);

			SHARED::CHARSET_PACKET &WeaponLeft = ViewportCreateCharacter.Equipment[GAME::EQUIPMENT_WEAPON_LEFT];
			if (WeaponLeft.Enabled == true &&
				WeaponLeft.Type == 13 &&
				WeaponLeft.Index == 5 &&
				c->m_pPet != nullptr)
			{
				CSPetDarkSpirit* pPet = c->m_pPet;

				pPet->m_PetCharacter.Object.WeaponLevel = ViewportCreateCharacter.DarkRavenLevel;
			}

			mu_strcpy_s(c->ID, mu_countof(c->ID), ViewportCreateCharacter.Name.c_str());

			if (c == Hero)
			{
				g_MoveManager.Update();
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_DELETECHARACTER:
		{
			GS2CL_VIEWPORT_DELETECHARACTER ViewportDeleteCharacter;

			if (ViewportDeleteCharacter.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportDeleteCharacter.Key, false);
			if(iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* pCha = &CharactersClient[iIndex];

			mu_uint32 buffSize = g_CharacterBuffSize((&pCha->Object));

			for (mu_uint32 k = 0; k < buffSize; ++k)
			{
				UnRegisterBuff(g_CharacterBuff((&pCha->Object), k), &pCha->Object);
			}

			DeleteCharacter(ViewportDeleteCharacter.Key);
		}
		break;

	case GS2CL_PROT_VIEWPORT_CREATEMONEY:
		{
			GS2CL_VIEWPORT_CREATEMONEY ViewportCreateMoney;

			if (ViewportCreateMoney.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			if (ViewportCreateMoney.Key >= 0 &&
				ViewportCreateMoney.Key < MAX_ITEMS)
			{
				CreateMoney(&Items[ViewportCreateMoney.Key], ViewportCreateMoney.Money, EVector3((mu_float)ViewportCreateMoney.X + 0.5f, (mu_float)ViewportCreateMoney.Y + 0.5f, 0.0f) * g_TerrainScale, ViewportCreateMoney.Create);
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_CREATEITEM:
		{
			GS2CL_VIEWPORT_CREATEITEM ViewportCreateItem;

			if (ViewportCreateItem.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			if (ViewportCreateItem.Key >= 0 &&
				ViewportCreateItem.Key < MAX_ITEMS)
			{
				CreateItem(&Items[ViewportCreateItem.Key], ViewportCreateItem.ItemData, EVector3((mu_float)ViewportCreateItem.ItemData.InventoryPosX + 0.5f, (mu_float)ViewportCreateItem.ItemData.InventoryPosY + 0.5f, 0.0f) * g_TerrainScale, ViewportCreateItem.Create);
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_DELETEITEM:
		{
			GS2CL_VIEWPORT_DELETEITEM ViewportDeleteItem;

			if (ViewportDeleteItem.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			if (ViewportDeleteItem.Key >= 0 &&
				ViewportDeleteItem.Key < MAX_ITEMS)
			{
				Items[ViewportDeleteItem.Key].Object.Flags.Clear();
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_CREATEMONSTER:
		{
			GS2CL_VIEWPORT_CREATEMONSTER ViewportCreateMonster;

			if (ViewportCreateMonster.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			CHARACTER *c = CreateMonster(0, ViewportCreateMonster.Class, ViewportCreateMonster.PositionX, ViewportCreateMonster.PositionY, ViewportCreateMonster.Key);

			if (c == nullptr) break;

			OBJECT *o = &c->Object;

			c->EnabledInternalAction = INTERNAL_ACTION::IA_NONE;

			mu_float fAngle = 45.0f;
			if (o->Type == MODEL_MONSTER01 + 99)
			{
				if (c->PositionY == 90)
					fAngle = 0.0f;
				if (c->PositionX == 62)
					fAngle = -135.0f;
				if (c->PositionX == 183)
					fAngle = 90.0f;
			}
			else if (o->Type == MODEL_KANTURU2ND_ENTER_NPC)
			{
				o->m_fEdgeScale = 1.0f;
				fAngle = 90.0f;
			}

			c->Object.Angle[2] = ViewportCreateMonster.Direction;

			if (o->Type >= MODEL_LITTLESANTA + 1 && o->Type <= MODEL_LITTLESANTA + 4)
			{
				c->Object.Angle[2] = 90.0f;
			}
			else if (o->Type >= MODEL_LITTLESANTA + 6 && o->Type <= MODEL_LITTLESANTA + 7)
			{
				c->Object.Angle[2] = 90.0f;
			}
			else if (o->Type == MODEL_XMAS2008_SANTA_NPC)
			{
				c->Object.Angle[2] = 20.0f;
			}
			else if (o->Type == MODEL_UNITEDMARKETPLACE_JULIA)
			{
				c->Object.Angle[2] = 49.0f;
			}

			c->PositionX = ViewportCreateMonster.PositionX;
			c->PositionY = ViewportCreateMonster.PositionY;
			c->TargetX = ViewportCreateMonster.PositionX;
			c->TargetY = ViewportCreateMonster.PositionY;

#if 0 // FIX
			c->m_byFriend = bMyMob;
			o->m_byBuildTime = byBuildTime;
#endif

			if (battleCastle::InBattleCastle() && c->MonsterIndex == 277)
			{
				if (g_isCharacterBuff((&c->Object), eBuff_CastleGateIsOpen))
				{
					SetAction(&c->Object, 1);
					battleCastle::SetCastleGate_Attribute(ViewportCreateMonster.PositionX, ViewportCreateMonster.PositionY, 0);
				}
				else
				{
					SetAction(&c->Object, 0);
					battleCastle::SetCastleGate_Attribute(ViewportCreateMonster.PositionX, ViewportCreateMonster.PositionY, 1);
				}
			}

			if (ViewportCreateMonster.Create == true)
			{
				AppearMonster(0, c);
			}
			else if (GET_WORLD == WD_39KANTURU_3RD && o->Type == MODEL_MONSTER01 + 121 && ViewportCreateMonster.Teleport == true)
			{
				EVector3 Light;
				Vector(1.0f, 1.0f, 1.0f, Light);
				o->AlphaTarget = 1.0f;
				CreateEffect(0, BITMAP_SPARK + 1, o->Position, o->Angle, Light);
				CreateEffect(0, BITMAP_SPARK + 1, o->Position, o->Angle, Light);
				PlayBackground(SOUND_KANTURU_3RD_NIGHTMARE_TELE);
			}

			c->Movement = false;
		}
		break;

	case GS2CL_PROT_VIEWPORT_DELETEMONSTER:
		{
			GS2CL_VIEWPORT_DELETEMONSTER ViewportDeleteMonster;

			if (ViewportDeleteMonster.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportDeleteMonster.Key, false);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* pCha = &CharactersClient[iIndex];

			mu_uint32 buffSize = g_CharacterBuffSize((&pCha->Object));

			for (mu_uint32 k = 0; k < buffSize; ++k)
			{
				UnRegisterBuff(g_CharacterBuff((&pCha->Object), k), &pCha->Object);
			}

			DeleteCharacter(ViewportDeleteMonster.Key);
		}
		break;

	case GS2CL_PROT_VIEWPORT_CREATESUMMON:
		{
			GS2CL_VIEWPORT_CREATESUMMON ViewportCreateSummon;

			if (ViewportCreateSummon.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			CHARACTER *c;
			if (ViewportCreateSummon.Class >= 152 && ViewportCreateSummon.Class <= 158)
			{
				c = CreateHellGate(0, ViewportCreateSummon.Name.c_str(), ViewportCreateSummon.Key, ViewportCreateSummon.Class, ViewportCreateSummon.PositionX, ViewportCreateSummon.PositionY, ViewportCreateSummon.Create);
			}
			else
			{
				c = CreateMonster(0, ViewportCreateSummon.Class, ViewportCreateSummon.PositionX, ViewportCreateSummon.PositionY, ViewportCreateSummon.Key);
			}

			if (c == nullptr) break;

			OBJECT *o = &c->Object;

			c->EnabledInternalAction = INTERNAL_ACTION::IA_NONE;

			mu_float fAngle = 45.0f;
			if (o->Type == MODEL_MONSTER01 + 99)
			{
				if (c->PositionY == 90)
					fAngle = 0.0f;
				if (c->PositionX == 62)
					fAngle = -135.0f;
				if (c->PositionX == 183)
					fAngle = 90.0f;
			}
			else if (o->Type == MODEL_KANTURU2ND_ENTER_NPC)
			{
				o->m_fEdgeScale = 1.0f;
				fAngle = 90.0f;
			}

			c->Object.Angle[2] = ViewportCreateSummon.Direction;

			if (o->Type >= MODEL_LITTLESANTA + 1 && o->Type <= MODEL_LITTLESANTA + 4)
			{
				c->Object.Angle[2] = 90.0f;
			}
			else if (o->Type >= MODEL_LITTLESANTA + 6 && o->Type <= MODEL_LITTLESANTA + 7)
			{
				c->Object.Angle[2] = 90.0f;
			}
			else if (o->Type == MODEL_XMAS2008_SANTA_NPC)
			{
				c->Object.Angle[2] = 20.0f;
			}
			else if (o->Type == MODEL_UNITEDMARKETPLACE_JULIA)
			{
				c->Object.Angle[2] = 49.0f;
			}

			c->PositionX = ViewportCreateSummon.PositionX;
			c->PositionY = ViewportCreateSummon.PositionY;
			c->TargetX = ViewportCreateSummon.PositionX;
			c->TargetY = ViewportCreateSummon.PositionY;
			o->Kind = KIND_PLAYER;

#if 0 // FIX
			c->m_byFriend = bMyMob;
			o->m_byBuildTime = byBuildTime;
#endif

			if (battleCastle::InBattleCastle() && c->MonsterIndex == 277)
			{
				if (g_isCharacterBuff((&c->Object), eBuff_CastleGateIsOpen))
				{
					SetAction(&c->Object, 1);
					battleCastle::SetCastleGate_Attribute(ViewportCreateSummon.PositionX, ViewportCreateSummon.PositionY, 0);
				}
				else
				{
					SetAction(&c->Object, 0);
					battleCastle::SetCastleGate_Attribute(ViewportCreateSummon.PositionX, ViewportCreateSummon.PositionY, 1);
				}
			}

			if (ViewportCreateSummon.Create == true)
			{
				AppearMonster(0, c);
				CreateEffect(0, MODEL_MAGIC_CIRCLE1, o->Position, o->Angle, o->Light, 0, o);
				CreateParticle(0, BITMAP_LIGHTNING + 1, o->Position, o->Angle, o->Light, 2, 1.f, o);
				if (ViewportCreateSummon.Class >= 152 && ViewportCreateSummon.Class <= 158)
				{
					o->PriorAnimationFrame = 0.f;
					o->AnimationFrame = 0.f;
					PlayBackground(SOUND_CHAOS_FALLING_STONE);
				}
			}
			else if (GET_WORLD == WD_39KANTURU_3RD && o->Type == MODEL_MONSTER01 + 121 && ViewportCreateSummon.Teleport == true)
			{
				EVector3 Light;
				Vector(1.0f, 1.0f, 1.0f, Light);
				o->AlphaTarget = 1.0f;
				CreateEffect(0, BITMAP_SPARK + 1, o->Position, o->Angle, Light);
				CreateEffect(0, BITMAP_SPARK + 1, o->Position, o->Angle, Light);
				PlayBackground(SOUND_KANTURU_3RD_NIGHTMARE_TELE);
			}

			c->Movement = false;
		}
		break;

	case GS2CL_PROT_VIEWPORT_CHARSETCHANGE:
		{
			GS2CL_VIEWPORT_CHARSETCHANGE ViewportCharset;

			if (ViewportCharset.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_uint8 Class = (ViewportCharset.Class & 0x07) | ((ViewportCharset.SubClass & 0x03) << 3);

			mu_int32 iIndex = FindCharacterIndex(ViewportCharset.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* c = &CharactersClient[iIndex];

			DeleteCloth(c, &c->Object);

			c->Class = Class;
			c->Skin = 0;
			c->PK = ViewportCharset.PkLevel;

			ChangeCharacterExt(iIndex, ViewportCharset.Equipment);

			SHARED::CHARSET_PACKET &WeaponLeft = ViewportCharset.Equipment[GAME::EQUIPMENT_WEAPON_LEFT];
			if (WeaponLeft.Enabled == true &&
				WeaponLeft.Type == 13 &&
				WeaponLeft.Index == 5 &&
				c->m_pPet != nullptr)
			{
				CSPetDarkSpirit* pPet = c->m_pPet;

				pPet->m_PetCharacter.Object.WeaponLevel = ViewportCharset.DarkRavenLevel;
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_LIFE:
		{
			GS2CL_VIEWPORT_LIFE ViewportLife;

			if (ViewportLife.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			CharacterAttribute->Life = ViewportLife.Life;
			CharacterAttribute->LifeMax = ViewportLife.MaxLife;
			CharacterAttribute->Shield = ViewportLife.Shield;
			CharacterAttribute->ShieldMax = ViewportLife.MaxShield;
		}
		break;

	case GS2CL_PROT_VIEWPORT_MANA:
		{
			GS2CL_VIEWPORT_MANA ViewportMana;

			if (ViewportMana.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			CharacterAttribute->Mana = ViewportMana.Mana;
			CharacterAttribute->ManaMax = ViewportMana.MaxMana;
			CharacterAttribute->SkillMana = ViewportMana.BP;
			CharacterAttribute->SkillManaMax = ViewportMana.MaxBP;
		}
		break;

	case GS2CL_PROT_VIEWPORT_LEVELUP:
		{
			GS2CL_VIEWPORT_LEVELUP ViewportLevelUp;

			if (ViewportLevelUp.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportLevelUp.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* c = &CharactersClient[iIndex];
			OBJECT *o = &c->Object;

			if (c == Hero)
			{
				g_pMyInventory->Configure();
			}

			if (IsMasterLevel(c->Class) == true)
			{
				CreateJoint(0, BITMAP_FLARE, o->Position, o->Position, o->Angle, 45, o, 80, 2);

				for (mu_int32 i = 0; i < 19; ++i)
				{
					CreateJoint(0, BITMAP_FLARE, o->Position, o->Position, o->Angle, 46, o, 80, 2);
				}
			}
			else
			{
				for (mu_int32 i = 0; i < 15; ++i)
				{
					CreateJoint(0, BITMAP_FLARE, o->Position, o->Position, o->Angle, 0, o, 40, 2);
				}

				CreateEffect(0, BITMAP_MAGIC + 1, o->Position, o->Angle, o->Light, 0, o);
			}

			PlayBackground(SOUND_LEVEL_UP);
		}
		break;

	case GS2CL_PROT_VIEWPORT_EXPERIENCE:
		{
			GS2CL_VIEWPORT_EXPERIENCE ViewportExperience;

			if (ViewportExperience.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			CharacterAttribute->Level = ViewportExperience.Level;
			Master_Level_Data.nMLevel = ViewportExperience.MasterLevel;

			if (IsMasterLevel(CharacterAttribute->Class) == true)
			{
				Master_Level_Data.lMasterLevel_Experince = ViewportExperience.Experience;
				Master_Level_Data.lNext_MasterLevel_Experince = ViewportExperience.NextExperience;
			}
			else
			{
				CharacterAttribute->Experience = ViewportExperience.Experience;
				CharacterAttribute->NextExperince = ViewportExperience.NextExperience;
			}

			g_pSkillFrame->UpdateSkills();
			g_MoveManager.Update();
		}
		break;

	case GS2CL_PROT_VIEWPORT_LEVELUPPOINTS:
		{
			GS2CL_VIEWPORT_LEVELUPPOINTS ViewportPoints;

			if (ViewportPoints.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			CharacterAttribute->LevelUpPoint = ViewportPoints.LevelUpPoints;
			Master_Level_Data.nMLevelUpMPoint = ViewportPoints.MasterLevelUpPoints;

			if (CharacterAttribute->LevelUpPoint > 0)
			{
				g_pCharacterInfoWindow->EnableStatButtons();
			}
			else
			{
				g_pCharacterInfoWindow->DisableStatButtons();
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_STATS:
		{
			GS2CL_VIEWPORT_STATS ViewportStats;

			if (ViewportStats.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			CharacterAttribute->Strength = ViewportStats.Strength;
			CharacterAttribute->Dexterity = ViewportStats.Dexterity;
			CharacterAttribute->Vitality = ViewportStats.Vitality;
			CharacterAttribute->Energy = ViewportStats.Energy;
			CharacterAttribute->Charisma = ViewportStats.Leadership;

			CharacterMachine->CalculateAll();
			g_pSkillFrame->UpdateSkills();
			g_MoveManager.Update();
		}
		break;

	case GS2CL_PROT_VIEWPORT_MONEY:
		{
			GS2CL_VIEWPORT_MONEY ViewportMoney;

			if (ViewportMoney.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			CharacterMachine->Gold = ViewportMoney.Money;
			CharacterMachine->StorageGold = ViewportMoney.StorageMoney;

			g_MoveManager.Update();
		}
		break;

	case GS2CL_PROT_VIEWPORT_SHIELDBROKE:
		{
			GS2CL_VIEWPORT_SHIELDBROKE ViewportShieldBroke;

			if (ViewportShieldBroke.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportShieldBroke.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* c = &CharactersClient[iIndex];
			OBJECT *o = &c->Object;

			if (InChaosCastle() == false)
			{
				CreateEffect(0, MODEL_SHIELD_CRASH, o->Position, o->Angle, o->Light, 0, o);
				PlayBackground(SOUND_SHIELDCLASH);
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_POTIONSD:
		{
			GS2CL_VIEWPORT_POTIONSD ViewportPotionSD;

			if (ViewportPotionSD.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportPotionSD.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* c = &CharactersClient[iIndex];
			OBJECT *o = &c->Object;

			CreateEffect(0, BITMAP_MAGIC + 1, o->Position, o->Angle, o->Light, 5, o);
		}
		break;

	case GS2CL_PROT_VIEWPORT_QUESTEFFECT:
		{
			GS2CL_VIEWPORT_QUESTEFFECT ViewportQuestEffect;

			if (ViewportQuestEffect.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportQuestEffect.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* c = &CharactersClient[iIndex];
			OBJECT *o = &c->Object;

			c->EnabledInternalAction = INTERNAL_ACTION::IA_NONE;

			switch (ViewportQuestEffect.EffectType)
			{
			case QuestEffectType::QET_STATUP:
				{
					EVector3 Position;
					VectorCopy(o->Position, Position);

					for (mu_int32 i = 0; i < 15; ++i)
					{
						CreateJoint(0, BITMAP_FLARE, o->Position, o->Position, o->Angle, 0, o, 40, 2);
					}

					CreateEffect(0, BITMAP_MAGIC + 1, o->Position, o->Angle, o->Light, 0, o);
					PlayBackground(SOUND_CHANGE_UP);
				}
				break;

			case QuestEffectType::QET_CHANGEUP:
				{
					EVector3 Position;
					VectorCopy(o->Position, Position);
					Position[2] += 200.0f;

					for (mu_int32 i = 0; i < 15; ++i)
					{
						CreateJoint(0, BITMAP_FLARE, o->Position, o->Position, o->Angle, 0, o, 40, 2);
						CreateJoint(0, BITMAP_FLARE, Position, Position, o->Angle, 10, o, 40, 2);
					}

					CreateEffect(0, BITMAP_MAGIC + 1, o->Position, o->Angle, o->Light, 0, o);
					SetAction(o, PLAYER_SALUTE1);
					PlayBackground(SOUND_CHANGE_UP);
				}
				break;

			case QuestEffectType::QET_PLUSSTAT:
				{
					EVector3 Position;
					VectorCopy(o->Position, Position);
					Position[2] += 200.0f;

					for (mu_int32 i = 0; i < 15; ++i)
					{
						CreateJoint(0, BITMAP_FLARE, o->Position, o->Position, o->Angle, 0, o, 40, 2);
						CreateJoint(0, BITMAP_FLARE, Position, Position, o->Angle, 10, o, 40, 2);
					}

					CreateEffect(0, BITMAP_MAGIC + 1, o->Position, o->Angle, o->Light, 0, o);
					SetAction(o, PLAYER_SALUTE1);
					PlayBackground(SOUND_CHANGE_UP);
				}
				break;

			case QuestEffectType::QET_USINGCOMBOSKILL:
				{
					EVector3 Position;
					VectorCopy(o->Position, Position);
					Position[2] += 200.0f;

					for (mu_int32 i = 0; i < 15; ++i)
					{
						CreateJoint(0, BITMAP_FLARE, o->Position, o->Position, o->Angle, 0, o, 40, 2);
						CreateJoint(0, BITMAP_FLARE, Position, Position, o->Angle, 10, o, 40, 2);
					}

					CreateEffect(0, BITMAP_MAGIC + 1, o->Position, o->Angle, o->Light, 0, o);
					SetAction(o, PLAYER_SALUTE1);
					PlayBackground(SOUND_CHANGE_UP);
				}
				break;

			case QuestEffectType::QET_THIRD_CHANGEUP:
				{
					CreateEffect(0, MODEL_CHANGE_UP_EFF, c->Object.Position, c->Object.Angle, c->Object.Light, 0, &c->Object);
					CreateEffect(0, MODEL_CHANGE_UP_NASA, c->Object.Position, c->Object.Angle, c->Object.Light, 0, &c->Object);
					CreateEffect(0, MODEL_CHANGE_UP_CYLINDER, c->Object.Position, c->Object.Angle, c->Object.Light, 0, &c->Object);
					CreateEffect(0, MODEL_INFINITY_ARROW3, c->Object.Position, c->Object.Angle, c->Object.Light, 1, &c->Object);

					SetAction(&c->Object, PLAYER_CHANGE_UP);
					PlayBackground(SOUND_CHANGE_UP);
				}
				break;
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_QUESTHISTORY:
		{
			GS2CL_VIEWPORT_QUESTHISTORY ViewportQuestHistory;

			if (ViewportQuestHistory.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			g_QuestManager.SetQuestLists(ViewportQuestHistory.Quest, Hero->Class);
			
			g_pMyQuestWindow->Configure();

			g_pSkillFrame->UpdateSkills();
		}
		break;

	case GS2CL_PROT_VIEWPORT_MAGICLIST:
		{
			GS2CL_VIEWPORT_MAGICLIST ViewportMagicList;

			if (ViewportMagicList.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			ZeroArray(CharacterAttribute->Skill);

			for (mu_uint32 n = 0; n < ViewportMagicList.MagicCount; ++n)
			{
				auto magic = &ViewportMagicList.Magics[n];

				CharacterAttribute->Skill[magic->Pos] = magic->SkillNum;
			}

			if (GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD)
			{
				for (mu_int32 i = 0; i < PET_CMD_END; ++i)
				{
					CharacterAttribute->Skill[AT_PET_COMMAND_DEFAULT + i] = AT_PET_COMMAND_DEFAULT + i;
				}
			}

			CharacterAttribute->SkillNumber = 0;
			CharacterAttribute->SkillMasterNumber = 0;

			mu_int32 SkillType = 0;

			for (mu_int32 i = 0; i < MAX_MAGIC; ++i)
			{
				SkillType = CharacterAttribute->Skill[i];

				if (SkillType != 0)
				{
					++CharacterAttribute->SkillNumber;

					if (SkillAttribute[SkillType].SkillUseType == SKILL_USE_TYPE_MASTER)
					{
						++CharacterAttribute->SkillMasterNumber;
					}
				}
			}

			for (mu_int32 i = 0; i < PET_CMD_END; ++i)
			{
				SkillType = CharacterAttribute->Skill[AT_PET_COMMAND_DEFAULT + i];

				if (SkillType != 0)
				{
					++CharacterAttribute->SkillNumber;

					if (SkillAttribute[SkillType].SkillUseType == SKILL_USE_TYPE_MASTER)
					{
						++CharacterAttribute->SkillMasterNumber;
					}
				}
			}

			if (Hero->CurrentSkill >= CharacterAttribute->SkillNumber)
			{
				Hero->CurrentSkill = 0;
			}

			if (CharacterAttribute->SkillNumber == 1)
			{
				Hero->CurrentSkill = 0;
			}

			if (Hero->CurrentSkill >= 0 && CharacterAttribute->Skill[Hero->CurrentSkill] == 0)
			{
				Hero->CurrentSkill = 0;
			}

			mu_int32 Skill = 0;

			for (mu_int32 i = 0; i < MAX_MAGIC; ++i)
			{
				Skill = CharacterAttribute->Skill[Hero->CurrentSkill];
				if (Skill >= AT_SKILL_STUN && Skill <= AT_SKILL_REMOVAL_BUFF)
					++Hero->CurrentSkill;
				else
					break;
			}

			mu_int32 Master_Skill_Bool = -1;
			mu_int32 Skill_Bool = -1;

			for (mu_int32 i = 0; i < MAX_MAGIC; ++i)
			{
				Skill = CharacterAttribute->Skill[i];

				if ((AT_SKILL_POWER_SLASH_UP <= Skill && AT_SKILL_POWER_SLASH_UP + 4 >= Skill) ||
					(AT_SKILL_MANY_ARROW_UP <= Skill && AT_SKILL_MANY_ARROW_UP + 4 >= Skill))
				{
					Master_Skill_Bool = i;
				}

				if (AT_SKILL_ICE_BLADE == Skill || Skill == AT_SKILL_CROSSBOW)
				{
					Skill_Bool = i;
				}
			}

			if (Master_Skill_Bool > -1 && Skill_Bool > -1)
			{
				CharacterAttribute->Skill[Skill_Bool] = 0;
			}

			g_pSkillFrame->UpdateSkills();
			g_pHelperSetup->Configure(g_HelperSettings);
		}
		break;

	case GS2CL_PROT_VIEWPORT_FORCEPOSITION:
		{
			GS2CL_VIEWPORT_FORCEPOSITION ViewportForcePosition;

			if (ViewportForcePosition.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportForcePosition.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* c = &CharactersClient[iIndex];

			OBJECT *o = &c->Object;
			
			if (ViewportForcePosition.IsTeleport == true)
			{
				g_pPickedItem.BackupPickedItem();

				c->PositionX = ViewportForcePosition.X;
				c->PositionY = ViewportForcePosition.Y;
				o->Position[0] = ((mu_float)(c->PositionX) + 0.5f)*g_TerrainScale;
				o->Position[1] = ((mu_float)(c->PositionY) + 0.5f)*g_TerrainScale;

				if (GET_WORLD == -1 || c->Helper.Type != MODEL_HELPER + 3 || c->SafeZone)
				{
					o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
				}
				else
				{
					if (GET_WORLD == WD_8TARKAN || GET_WORLD == WD_10HEAVEN)
						o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 90.0f;
					else
						o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 30.0f;
				}

				CreateTeleportEnd(0, o);
			}
			else
			{
				if (o->Type == MODEL_BALL)
				{
					o->Gravity = 20.0f;
					o->Direction[1] = (ViewportForcePosition.X - (c->PositionX))*400.0f;
					o->Direction[0] = -(ViewportForcePosition.Y - (c->PositionY))*400.0f;
					PlayBackground(SOUND_MENU01);
					CreateBomb(0, o->Position, false);
				}

				c->PositionX = ViewportForcePosition.X;
				c->PositionY = ViewportForcePosition.Y;
				c->TargetX = ViewportForcePosition.X;
				c->TargetY = ViewportForcePosition.Y;
				c->JumpTime = 1;
			}

			c->EnabledInternalAction = INTERNAL_ACTION::IA_NONE;
		}
		break;

	case GS2CL_PROT_VIEWPORT_REFRESH:
		{
			GS2CL_VIEWPORT_REFRESH ViewportRefresh;

			if (ViewportRefresh.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			ClearItems();

			for (mu_int32 i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
			{
				CHARACTER *c = &CharactersClient[i];

				if (c != Hero)
				{
					DeleteCharacter(c, &c->Object);
				}
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_STAND:
		{
			GS2CL_VIEWPORT_STAND ViewportStand;

			if (ViewportStand.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportStand.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* pCha = &CharactersClient[iIndex];

			pCha->Object.Angle[2] = ViewportStand.Dir;
			pCha->TargetX = pCha->PositionX;
			pCha->TargetY = pCha->PositionY;
			pCha->TargetAngle = ViewportStand.Dir;
			pCha->Movement = false;
			pCha->StormTime = ViewportStand.StormTime;
			SetPlayerStop(0, pCha);

			pCha->EnabledInternalAction = INTERNAL_ACTION::IA_NONE;
		}
		break;

	case GS2CL_PROT_VIEWPORT_ANIMATION:
		{
			GS2CL_VIEWPORT_ANIMATION ViewportAnimation;

			if (ViewportAnimation.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportAnimation.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER *c = &CharactersClient[iIndex];
			OBJECT *o = &c->Object;

			if (InternalUsage == false)
			{
				c->EnabledInternalAction = INTERNAL_ACTION::IA_NORMAL;
				mu_memcpy(c->InternalActionPacket, PacketBuffer, Header->Size);
			}
			else
			{
				c->Object.Angle[2] = ViewportAnimation.Dir;
				
				c->Path.PathNum = 0;
				c->Path.CurrentPath = 0;
				c->Path.CurrentPathFloat = 0;
				c->Movement = false;

				SetAction(o, ViewportAnimation.Animation);

				// 				c->Object.Position[0] = c->TargetX*TERRAIN_SCALE + TERRAIN_SCALE*0.5f;
				// 				c->Object.Position[1] = c->TargetY*TERRAIN_SCALE + TERRAIN_SCALE*0.5f;

				if (o->Type == MODEL_PLAYER)
				{
					// Players Actions
					switch (ViewportAnimation.Animation)
					{
					case PLAYER_DEFENSE1:
						{
							PlayBackground(SOUND_SKILL_DEFENSE);
						}
						break;

					case PLAYER_JACK_1:
					case PLAYER_JACK_2:
						{
							o->m_iAnimation = 0;
						}
						break;

					case PLAYER_SANTA_1:
						{
							g_XmasEvent.CreateXmasEventEffect(0, c, o, ViewportAnimation.SubAnimation);
							o->m_iAnimation = 0;
							PlayBackground(SOUND_XMAS_JUMP_SANTA + ViewportAnimation.SubAnimation);
						}
						break;

					case PLAYER_SANTA_2:
						{
							g_XmasEvent.CreateXmasEventEffect(0, c, o, ViewportAnimation.SubAnimation);
							o->m_iAnimation = 0;
							PlayBackground(SOUND_XMAS_TURN);
						}
						break;

					case PLAYER_KOREA_HANDCLAP:
						{
							PlayBackground(SOUND_WORLDCUP_RND1 + ViewportAnimation.SubAnimation);
						}
						break;
					}
				}
				else
				{
					// Monsters Actions
				}
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_MOVE:
		{
			GS2CL_VIEWPORT_MOVE ViewportMove;

			if (ViewportMove.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportMove.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* pCha = &CharactersClient[iIndex];
			if (pCha->Path.PathNum == 0 && pCha->PositionX == ViewportMove.X && pCha->PositionY == ViewportMove.Y)
			{
			}
			else if (pCha->Path.PathNum > 0 && pCha->Path.PathX[pCha->Path.PathNum - 1] == ViewportMove.X && pCha->Path.PathY[pCha->Path.PathNum - 1] == ViewportMove.Y)
			{
			}
			else
			{
				mu_uint8 pathX;
				mu_uint8 pathY;
				mu_uint8 pathFloat = 0;

				mu_int32 currentX = pCha->PositionX;
				mu_int32 currentY = pCha->PositionY;

				if (pCha->Movement == true && pCha->Path.CurrentPathFloat > 0)
				{
					pathX = pCha->Path.PathX[pCha->Path.CurrentPath];
					pathY = pCha->Path.PathY[pCha->Path.CurrentPath];
					pathFloat = pCha->Path.CurrentPathFloat;

					currentX = pCha->Path.PathX[pCha->Path.CurrentPath + 1];
 					currentY = pCha->Path.PathY[pCha->Path.CurrentPath + 1];
				}

				pCha->TargetX = ViewportMove.X;
				pCha->TargetY = ViewportMove.Y;
				pCha->TargetAngle = ViewportMove.Dir;
				pCha->Run = ViewportMove.Run;

				pCha->Path.PathNum = 0;
				pCha->Path.CurrentPath = 0;
				pCha->Path.CurrentPathFloat = 0;
				pCha->Movement = false;

				mu_boolean pathFound = PathFinding2(0, currentX, currentY, ViewportMove.X, ViewportMove.Y, &pCha->Path, 0.0f, TW_NOMOVE, pathFloat > 0 ? 1 : 0);
				if (pathFound == true ||
					pathFloat > 0)
				{
					if (pathFloat > 0)
					{
						++pCha->Path.PathNum;
						pCha->Path.PathX[0] = pathX;
						pCha->Path.PathY[0] = pathY;
						pCha->Path.CurrentPathFloat = pathFloat;
					}
					pCha->Movement = true;
				}
				else
				{
					pCha->Movement = false;
					SetPlayerStop(0, pCha);
				}
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_REGEN:
		{
			GS2CL_VIEWPORT_REGEN ViewportRegen;

			if (ViewportRegen.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			if (MU_INPUT->Is3DMousePressing(MOUSE_BUTTON_LEFT) == true)
			{
				MU_INPUT->SetMouseUsed(MOUSE_BUTTON_LEFT, MOUSE_USED_ALL);
			}

			Teleport = false;

			mu_uint8 Temp = Hero->PK;
			mu_int32 TempGuild = Hero->GuildMarkIndex;

			CHARACTER *c = &CharactersClient[HeroIndex];
			OBJECT *o = &c->Object;

			c->EnabledInternalAction = INTERNAL_ACTION::IA_NONE;

			o->Flags.Clear(OBJECT::eOF_LIVE);

			mu_int16 BackUpGuildStatus = c->GuildStatus;
			mu_uint8 BackUpGuildType = c->GuildType;
			mu_uint8 BackUpGuildRelationShip = c->GuildRelationShip;
			mu_uint8 byBackupEtcPart = c->EtcPart;

			mu_boolean CreateDarkRaven = false;
			mu_uint16 BackupDarkRavenLevel = 0;
			if (GetBaseClass(c->Class) == GAME::CLASS_DARK_LORD)
			{
				if (c->m_pPet != nullptr)
				{
					CSPetDarkSpirit *pPet = c->m_pPet;
					CreateDarkRaven = true;
					BackupDarkRavenLevel = pPet->m_PetCharacter.Object.WeaponLevel;
				}
			}

			g_Tasking.CleanCharacters();

			CreateCharacterPointer(c, MODEL_PLAYER, ViewportRegen.X, ViewportRegen.Y, ((mu_float)ViewportRegen.Dir - 1.0f)*45.0f);

			o->Flags.Set(OBJECT::eOF_LIVE);
			c->Key = HeroKey;
			CharactersKeys[HeroKey] = HeroIndex;
			c->GuildStatus = BackUpGuildStatus;
			c->GuildType = BackUpGuildType;
			c->GuildRelationShip = BackUpGuildRelationShip;
			c->EtcPart = byBackupEtcPart;
			c->EnabledInternalAction = INTERNAL_ACTION::IA_NONE;

			for (mu_uint32 n = 0; n < PartyNumber; ++n)
			{
				PARTY_t &member = Party[n];

				if (member.Key == HeroKey)
				{
					member.index = FindCharacterIndex(HeroKey);
					break;
				}
			}

			c->Class = CharacterAttribute->Class;
			c->Skin = 0;
			c->PK = Temp;
			o->Kind = KIND_PLAYER;
			c->GuildMarkIndex = TempGuild;
			c->SafeZone = true;
			SetCharacterClass(0, c);

			SetPlayerStop(0, c);
			CreateEffect(0, BITMAP_MAGIC + 2, o->Position, o->Angle, o->Light, 0, o);
#if 0 // FIX
			RemoveAllShopTitleExceptHero();
#endif

			if (GET_WORLD >= WD_45CURSEDTEMPLE_LV1 && GET_WORLD <= WD_45CURSEDTEMPLE_LV6)
			{
				if (!(ViewportRegen.MapNumber >= WD_45CURSEDTEMPLE_LV1 && ViewportRegen.MapNumber <= WD_45CURSEDTEMPLE_LV6))
				{
					g_CursedTemple.ResetCursedTemple();
					g_pNewUISystem->Hide(GAMEUI::INTERFACE_CURSEDTEMPLE_GAMESYSTEM);
				}
			}

			if (GET_WORLD != ViewportRegen.MapNumber)
			{
				mu_int32 OldWorld = GET_WORLD;

				EMusicBackend::Stop();
				g_Tasking.CleanSounds();
				g_Tasking.CleanEffects();
				g_Tasking.CleanBoids();
				g_Tasking.CleanPets();
				g_Tasking.CleanLeaves();
				g_Tasking.CleanOperates();
				g_Tasking.CleanWorldObjects();
				g_Tasking.CleanProxy();

				// Event Clear
				{
					g_pBloodCastleTimer->Reset();
				}

				g_Terrain.Load(ViewportRegen.MapNumber, 0);

				g_Tasking.ReleaseModelsMemory();

				if (InHellas() == true)
				{
					g_pCSWaterTerrain = new MUWaterTerrain();

					if (g_pCSWaterTerrain->Create() == false)
					{
						mu_assert(!_T("Failed to create water terrain!"));
					}
				}
				else
				{
					MU_SAFE_DELETE(g_pCSWaterTerrain);
				}

				g_pMyInventory->Configure();

				if ((InChaosCastle(OldWorld) == true && OldWorld != GET_WORLD)
					|| InChaosCastle() == true)
				{
					SetCharacterClass(0, Hero);
				}

				if (InChaosCastle() == false)
				{
					StopBackground(SOUND_CHAOSCASTLE);
					StopBackground(SOUND_CHAOS_ENVIR);
				}

				if (IsEmpireGuardian1() == false &&
					IsEmpireGuardian2() == false &&
					IsEmpireGuardian3() == false &&
					IsEmpireGuardian4() == false)
				{
					StopBackground(SOUND_EMPIREGUARDIAN_WEATHER_RAIN);
					StopBackground(SOUND_EMPIREGUARDIAN_WEATHER_FOG);
					StopBackground(SOUND_EMPIREGUARDIAN_WEATHER_STORM);
					StopBackground(SOUND_EMPIREGUARDIAN_INDOOR_SOUND);
				}

				if (GET_WORLD == -1 || c->Helper.Type != MODEL_HELPER + 3 || c->SafeZone)
				{
					o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
				}
				else
				{
					if (GET_WORLD == WD_8TARKAN || GET_WORLD == WD_10HEAVEN)
						o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 90.0f;
					else
						o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 30.0f;
				}
			}

			if (EventMask != ViewportRegen.EventMask)
			{
				EventMask = ViewportRegen.EventMask;
				g_Tasking.CleanBoids();
			}

			if (CreateDarkRaven == true)
			{
				giPetManager::CreatePetDarkSpirit(c);
				if (c->m_pPet != nullptr)
				{
					CSPetDarkSpirit *pPet = c->m_pPet;
					pPet->m_PetCharacter.Object.WeaponLevel = BackupDarkRavenLevel;
				}
			}

#if 0 // FIX
			SummonLife = 0;
#endif

			if(o->Flags.Get(OBJECT::eOF_LIVE) == true)
			{
				GuildTeam(c);
			}

			g_pMapName->Show();
			g_pSimpleChat->Clear();

#if 0 // FIX
			CreateMyGensInfluenceGroundEffect();
#endif

			if (GET_WORLD < WD_65DOPPLEGANGER1 || GET_WORLD > WD_68DOPPLEGANGER4)
			{
				g_pNewUISystem->Hide(GAMEUI::INTERFACE_DOPPELGANGER_FRAME);
			}

			if (GET_WORLD < WD_69EMPIREGUARDIAN1 || WD_72EMPIREGUARDIAN4 < GET_WORLD)
			{
				g_pNewUISystem->Hide(GAMEUI::INTERFACE_EMPIREGUARDIAN_TIMER);
			}

			g_pNewUISystem->HideAll();
		}
		break;

	case GS2CL_PROT_VIEWPORT_ATTACK:
		{
			GS2CL_VIEWPORT_ATTACK ViewportAttack;

			if (ViewportAttack.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportAttack.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* c = &CharactersClient[iIndex];
			OBJECT* o = &c->Object;

			mu_int32 iTargetIndex = FindCharacterIndex(ViewportAttack.TargetKey);
			if (iTargetIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* tc = &CharactersClient[iTargetIndex];
			OBJECT* to = &tc->Object;

			c->EnabledInternalAction = INTERNAL_ACTION::IA_NONE;
			c->TargetAngle = ViewportAttack.Dir;
			c->Object.Angle[2] = CreateAngle(o->Position.x, o->Position.y, to->Position.x, to->Position.y);// ((mu_float)(ViewportAttack.Dir) - 1.0f)*45.0f;
			
			c->Path.PathNum = 0;
			c->Path.CurrentPath = 0;
			c->Path.CurrentPathFloat = 0;
			c->Movement = false;

			SetPlayerAttack(0, c);
			c->AttackTime = 1;
			c->Object.AnimationFrame = 0;

			c->TargetCharacter = HeroIndex;

			AttackPlayer = iIndex;
		}
		break;

	case GS2CL_PROT_VIEWPORT_MAGIC:
		{
			GS2CL_VIEWPORT_MAGIC ViewportMagic;

			if (ViewportMagic.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			if (ViewportMagic.MagicNumber == AT_SKILL_ATTACK || ViewportMagic.MagicNumber == AT_SKILL_DEFENSE
				|| (AT_SKILL_DEF_POWER_UP <= ViewportMagic.MagicNumber && ViewportMagic.MagicNumber <= AT_SKILL_DEF_POWER_UP + 4)
				|| (AT_SKILL_ATT_POWER_UP <= ViewportMagic.MagicNumber && ViewportMagic.MagicNumber <= AT_SKILL_ATT_POWER_UP + 4)
				)
			{
				if (ViewportMagic.SkillSuccess == false)
				{
					g_pChatListBox->AddText(_T(""), GlobalText[2249], TYPE_SYSTEM_MESSAGE);
					break;
				}
			}

			mu_int32 Index = FindCharacterIndex(ViewportMagic.Key);
			mu_int32 TargetIndex = FindCharacterIndex(ViewportMagic.TargetKey);
			if (Index == MAX_CHARACTERS_CLIENT ||
				TargetIndex == MAX_CHARACTERS_CLIENT)
				break;

			CHARACTER *sc = &CharactersClient[Index];
			CHARACTER *tc = &CharactersClient[TargetIndex];
			OBJECT *so = &sc->Object;
			OBJECT *to = &tc->Object;

			AttackPlayer = Index;

			sc->EnabledInternalAction = INTERNAL_ACTION::IA_NONE;
			
			sc->Path.PathNum = 0;
			sc->Path.CurrentPath = 0;
			sc->Path.CurrentPathFloat = 0;
			sc->Movement = false;

			if (ViewportMagic.MagicNumber != AT_SKILL_COMBO)
			{
				if (ViewportMagic.TargetKey == ViewportMagic.Key)
				{
					so->Angle[2] = ViewportMagic.Dir;
				}
				else if (ViewportMagic.MagicNumber == AT_SKILL_TELEPORT ||
					ViewportMagic.MagicNumber == AT_SKILL_BLAST_HELL_BEGIN)
				{
					so->Angle[2] = ViewportMagic.Dir;
				}
				else if (ViewportMagic.MagicNumber != AT_SKILL_TELEPORT_B &&
					ViewportMagic.MagicNumber != AT_SKILL_BLAST_HELL &&
					to->Flags.Get(OBJECT::eOF_VISIBLE) == true)
					so->Angle[2] = CreateAngle(so->Position[0], so->Position[1], to->Position[0], to->Position[1]);
				
				sc->TargetCharacter = TargetIndex;

				if (ViewportMagic.MagicNumber == AT_SKILL_PLASMA_STORM_FENRIR)
					sc->m_iFenrirSkillTarget = TargetIndex;

				sc->SkillSuccess = (ViewportMagic.SkillSuccess != 0);
				sc->Skill = ViewportMagic.MagicNumber;
				sc->SkillX = tc->PositionX;
				sc->SkillY = tc->PositionY;
			}

			switch (ViewportMagic.MagicNumber)
			{
			case AT_SKILL_MONSTER_SUMMON:
				SetPlayerAttack(0, sc);
				break;
			case AT_SKILL_RECOVER:
				{
					OBJECT* o = so;
					EVector3 Light, Position, P, dp;
					EVector3 vFirePosition;

					EBone Matrix;

					Vector(0.0f, -220.0f, 130.0f, P);
					AngleMatrix(o->Angle, Matrix);
					VectorRotate(P, Matrix, dp);
					VectorAdd(dp, o->Position, Position);
					Vector(0.7f, 0.6f, 0.0f, Light);
					CreateEffect(0, BITMAP_IMPACT, Position, o->Angle, Light, 0, o);
					SetAction(o, PLAYER_RECOVER_SKILL);
					sc->AttackTime = 1;

					o = to;

					Vector(0.4f, 0.6f, 1.0f, Light);
					Vector(0.0f, 0.0f, 0.0f, P);
					AngleMatrix(o->Angle, Matrix);
					VectorRotate(P, Matrix, dp);
					VectorAdd(dp, o->Position, Position);
					Position[2] += 130;
					VectorCopy(o->Position, Position);
					for (mu_int32 i = 0; i < 19; ++i)
					{
						CreateJoint(0, BITMAP_FLARE, Position, Position, o->Angle, 47, o, 40, 2);
					}
					Vector(0.3f, 0.2f, 0.1f, Light);
					CreateEffect(0, MODEL_SUMMON, Position, o->Angle, Light, 0);
					CreateEffect(0, BITMAP_TWLIGHT, Position, o->Angle, Light, 0);
					CreateEffect(0, BITMAP_TWLIGHT, Position, o->Angle, Light, 1);
					CreateEffect(0, BITMAP_TWLIGHT, Position, o->Angle, Light, 2);

					const MUModel *b = &MODELS::Data[o->Type];
					const mu_int32 iNumBones = b->_BoneNonDummyEnd;
					for (mu_int32 i = 0; i < 2; ++i)
					{
						b->TransformByObjectBoneDirect(0, vFirePosition, o, GetLargeRand(iNumBones));
						CreateEffect(0, BITMAP_FLARE, vFirePosition, o->Angle, Light, 3);
					}
				}

				break;
			case AT_SKILL_MONSTER_MAGIC_DEF:
				sc->AttackTime = 1;
				g_CharacterRegisterBuff(so, eBuff_PhysDefense);
				SetPlayerAttack(0, sc);
				break;
			case AT_SKILL_MONSTER_PHY_DEF:
				sc->AttackTime = 1;
				g_CharacterRegisterBuff(so, eBuff_Defense);
				SetPlayerAttack(0, sc);
				break;
			case AT_SKILL_HEAL_UP:
			case AT_SKILL_HEAL_UP + 1:
			case AT_SKILL_HEAL_UP + 2:
			case AT_SKILL_HEAL_UP + 3:
			case AT_SKILL_HEAL_UP + 4:
			case AT_SKILL_HEALING:
			case AT_SKILL_ATT_POWER_UP:
			case AT_SKILL_ATT_POWER_UP + 1:
			case AT_SKILL_ATT_POWER_UP + 2:
			case AT_SKILL_ATT_POWER_UP + 3:
			case AT_SKILL_ATT_POWER_UP + 4:
			case AT_SKILL_ATTACK:
			case AT_SKILL_DEF_POWER_UP:
			case AT_SKILL_DEF_POWER_UP + 1:
			case AT_SKILL_DEF_POWER_UP + 2:
			case AT_SKILL_DEF_POWER_UP + 3:
			case AT_SKILL_DEF_POWER_UP + 4:
			case AT_SKILL_DEFENSE:
			case AT_SKILL_SUMMON:
			case AT_SKILL_SUMMON + 1:
			case AT_SKILL_SUMMON + 2:
			case AT_SKILL_SUMMON + 3:
			case AT_SKILL_SUMMON + 4:
			case AT_SKILL_SUMMON + 5:
			case AT_SKILL_SUMMON + 6:
				if (sc->MonsterIndex != 77)
				{
					PlayBackground(SOUND_SKILL_DEFENSE);
				}
			case AT_SKILL_SOUL_UP:
			case AT_SKILL_SOUL_UP + 1:
			case AT_SKILL_SOUL_UP + 2:
			case AT_SKILL_SOUL_UP + 3:
			case AT_SKILL_SOUL_UP + 4:
			case AT_SKILL_WIZARDDEFENSE:
				SetPlayerMagic(0, sc);
				so->AnimationFrame = 0;
				sc->AttackTime = 1;
				break;
			case AT_SKILL_THUNDER:
				if (so->Type != MODEL_MONSTER01 + 83)
					PlayBackground(SOUND_THUNDER01);
				if (to->CurrentAction == PLAYER_POSE1 || to->CurrentAction == PLAYER_POSE_FEMALE1 ||
					to->CurrentAction == PLAYER_SIT1 || to->CurrentAction == PLAYER_SIT_FEMALE1)
					SetPlayerStop(0, tc);
			case AT_SKILL_FIREBALL:
			case AT_SKILL_METEO:
			case AT_SKILL_SLOW:
			case AT_SKILL_ENERGYBALL:
			case AT_SKILL_POWERWAVE:
			case AT_SKILL_POISON:
			case AT_SKILL_FLAME:
				if (so->Type == MODEL_PLAYER)
				{
					SetPlayerMagic(0, sc);
					so->AnimationFrame = 0;
				}
				else
				{
					SetPlayerAttack(0, sc);
					so->AnimationFrame = 0;
				}
				sc->AttackTime = 1;
				break;
			case AT_SKILL_TELEPORT_B:
				CreateTeleportBegin(0, to);

				CreateTeleportEnd(0, so);
				if (sc == Hero)
					Teleport = false;

				PlayObject(SOUND_TELEKINESIS, so);
				break;
			case AT_SKILL_TELEPORT:
				if (GET_WORLD == WD_39KANTURU_3RD && so->Type == MODEL_MONSTER01 + 121)
				{
					EVector3 Light;
					Vector(1.0f, 1.0f, 1.0f, Light);
					so->AlphaTarget = 0.0f;
					CreateEffect(0, BITMAP_SPARK + 1, so->Position, so->Angle, Light);
					CreateEffect(0, BITMAP_SPARK + 1, so->Position, so->Angle, Light);
					PlayBackground(SOUND_KANTURU_3RD_NIGHTMARE_TELE);
				}
				else
					CreateTeleportBegin(0, so);
				break;
			case AT_SKILL_STRONG_PIER:
				if ((sc->Helper.Type >= MODEL_HELPER + 2 && sc->Helper.Type <= MODEL_HELPER + 4) && !sc->SafeZone)
				{
					SetAction(so, PLAYER_ATTACK_RIDE_STRIKE);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 37 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_FENRIR_ATTACK_DARKLORD_STRIKE);
				}
				else
				{
					SetAction(so, PLAYER_ATTACK_STRIKE);
				}
				sc->AttackTime = 1;
				PlayBackground(SOUND_SKILL_SWORD1);
				break;
			case AT_SKILL_FIRE_BUST_UP:
			case AT_SKILL_FIRE_BUST_UP + 1:
			case AT_SKILL_FIRE_BUST_UP + 2:
			case AT_SKILL_FIRE_BUST_UP + 3:
			case AT_SKILL_FIRE_BUST_UP + 4:
			case AT_SKILL_LONGPIER_ATTACK:
				if ((sc->Helper.Type >= MODEL_HELPER + 2 && sc->Helper.Type <= MODEL_HELPER + 4) && !sc->SafeZone)
				{
					SetAction(so, PLAYER_ATTACK_RIDE_STRIKE);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 37 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_FENRIR_ATTACK_DARKLORD_STRIKE);
				}
				else
				{
					SetAction(so, PLAYER_ATTACK_STRIKE);
				}
				sc->AttackTime = 1;
				PlayBackground(SOUND_ATTACK_FIRE_BUST);
				break;
			case AT_SKILL_SWORD1:
				SetAction(so, PLAYER_ATTACK_SKILL_SWORD1 + ViewportMagic.MagicNumber - AT_SKILL_SWORD1);
				sc->AttackTime = 1;
				PlayBackground(SOUND_SKILL_SWORD1);
				break;
			case AT_SKILL_SWORD2:
				SetAction(so, PLAYER_ATTACK_SKILL_SWORD1 + ViewportMagic.MagicNumber - AT_SKILL_SWORD1);
				sc->AttackTime = 1;
				PlayBackground(SOUND_SKILL_SWORD2);
				break;
			case AT_SKILL_SWORD3:
				SetAction(so, PLAYER_ATTACK_SKILL_SWORD1 + ViewportMagic.MagicNumber - AT_SKILL_SWORD1);
				sc->AttackTime = 1;
				PlayBackground(SOUND_SKILL_SWORD3);
				break;
			case AT_SKILL_SWORD4:
				SetAction(so, PLAYER_ATTACK_SKILL_SWORD1 + ViewportMagic.MagicNumber - AT_SKILL_SWORD1);
				sc->AttackTime = 1;
				PlayBackground(SOUND_SKILL_SWORD4);
				break;
			case AT_SKILL_SWORD5:
				if (sc->SwordCount % 2 == 0)
				{
					SetAction(so, PLAYER_ATTACK_SKILL_SWORD1 + ViewportMagic.MagicNumber - AT_SKILL_SWORD1);
				}
				else
				{
					SetAction(so, PLAYER_ATTACK_TWO_HAND_SWORD1 + 2);
				}
				++sc->SwordCount;
				sc->AttackTime = 1;
				PlayBackground(SOUND_SKILL_SWORD4);
				break;
			case AT_SKILL_POWER_SLASH_UP:
			case AT_SKILL_POWER_SLASH_UP + 1:
			case AT_SKILL_POWER_SLASH_UP + 2:
			case AT_SKILL_POWER_SLASH_UP + 3:
			case AT_SKILL_POWER_SLASH_UP + 4:
			case AT_SKILL_ICE_BLADE:
				SetAction(so, PLAYER_ATTACK_TWO_HAND_SWORD_TWO);
				sc->AttackTime = 1;
				PlayBackground(SOUND_SKILL_SWORD4);
				break;
			case AT_SKILL_SPEAR:
				if (sc->Helper.Type == MODEL_HELPER + 37)
					SetAction(so, PLAYER_FENRIR_ATTACK_SPEAR);
				else
					SetAction(so, PLAYER_ATTACK_SKILL_SPEAR);
				sc->AttackTime = 1;
				break;
			case AT_SKILL_BLOW_UP:
			case AT_SKILL_BLOW_UP + 1:
			case AT_SKILL_BLOW_UP + 2:
			case AT_SKILL_BLOW_UP + 3:
			case AT_SKILL_BLOW_UP + 4:
			case AT_SKILL_ONETOONE:
				SetAction(so, PLAYER_ATTACK_ONETOONE);
				if (so->Type == MODEL_PLAYER)
				{
					so->AnimationFrame = 0;
				}
				sc->AttackTime = 1;
				break;
			case AT_SKILL_MANY_ARROW_UP:
			case AT_SKILL_MANY_ARROW_UP + 1:
			case AT_SKILL_MANY_ARROW_UP + 2:
			case AT_SKILL_MANY_ARROW_UP + 3:
			case AT_SKILL_MANY_ARROW_UP + 4:
			case AT_SKILL_CROSSBOW:
				SetPlayerBow(sc);
				sc->AttackTime = 1;
				break;
			case AT_SKILL_MULTI_SHOT:
				SetPlayerBow(sc);
				if (so->Type == MODEL_PLAYER)
				{
					so->AnimationFrame = 0;
				}
				sc->AttackTime = 1;
				break;
			case AT_SKILL_BLAST_CROSSBOW4:
				SetPlayerBow(sc);
				sc->AttackTime = 1;
				break;

			case AT_SKILL_PIERCING:
				SetPlayerBow(sc);
				sc->AttackTime = 1;
				break;

			case AT_SKILL_PARALYZE:
				SetPlayerBow(sc);
				if (sc->SkillSuccess)
				{
					//DeleteEffect(MODEL_ICE, to, 1);

					EVector3 Angle;
					VectorCopy(to->Angle, Angle);

					CreateEffect(0, MODEL_ICE, to->Position, Angle, to->Light, 1, to);

					Angle[2] += 180.0f;
					CreateEffect(0, MODEL_ICE, to->Position, Angle, to->Light, 2, to);

					tc->Movement = false;
					SetPlayerStop(0, tc);

					g_CharacterRegisterBuff(to, eDeBuff_Harden);
				}
				sc->AttackTime = 1;
				break;

			case AT_SKILL_RUSH:
				SetAction(so, PLAYER_ATTACK_RUSH);
				sc->AttackTime = 1;
				PlayBackground(SOUND_SKILL_SWORD2);
				break;

			case AT_SKILL_JAVELIN:
				if (so->Type == MODEL_PLAYER)
				{
					SetPlayerMagic(0, sc);
					so->AnimationFrame = 0;
				}
				so->AnimationFrame = 0;
				sc->AttackTime = 1;
				break;

			case AT_SKILL_DEATH_CANNON:
				SetAction(so, PLAYER_ATTACK_DEATH_CANNON);
				so->AnimationFrame = 0;
				sc->AttackTime = 1;
				break;

			case AT_SKILL_DEEPIMPACT:
				SetPlayerHighBow(sc);
				sc->AttackTime = 1;
				break;

			case AT_SKILL_SPACE_SPLIT:
				if ((sc->Helper.Type >= MODEL_HELPER + 2 && sc->Helper.Type <= MODEL_HELPER + 4) && !sc->SafeZone)
				{
					SetAction(so, PLAYER_ATTACK_RIDE_STRIKE);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 37 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_FENRIR_ATTACK_DARKLORD_STRIKE);
				}
				else
				{
					SetAction(so, PLAYER_ATTACK_STRIKE);
				}
				sc->AttackTime = 1;
				PlayBackground(SOUND_ATTACK_FIRE_BUST);
				break;

			case AT_SKILL_BRAND_OF_SKILL:

				g_CharacterRegisterBuff(to, eBuff_AddSkill);

				if (sc->Helper.Type == MODEL_HELPER + 4 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_ATTACK_RIDE_ATTACK_MAGIC);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 37 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_FENRIR_ATTACK_MAGIC);
				}
				else
				{
					SetAction(so, PLAYER_SKILL_HAND1);
				}
				sc->AttackTime = 1;
				break;

			case AT_SKILL_ONEFLASH:
				SetAction(so, PLAYER_ATTACK_ONE_FLASH);
				sc->AttackTime = 1;
				PlayBackground(SOUND_SKILL_SWORD2);
				break;

			case AT_SKILL_STUN:
				if (sc->Helper.Type == MODEL_HELPER + 4 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_ATTACK_RIDE_ATTACK_MAGIC);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 2 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_SKILL_RIDER);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 3 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_SKILL_RIDER_FLY);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 37 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_FENRIR_ATTACK_MAGIC);
				}
				else
				{
					SetAction(so, PLAYER_SKILL_VITALITY);
				}
				sc->AttackTime = 1;
				break;

			case AT_SKILL_REMOVAL_STUN:
				if (sc->Helper.Type == MODEL_HELPER + 4 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_ATTACK_RIDE_ATTACK_MAGIC);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 2 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_SKILL_RIDER);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 3 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_SKILL_RIDER_FLY);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 37 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_FENRIR_ATTACK_MAGIC);
				}
				else
				{
					SetAction(so, PLAYER_ATTACK_REMOVAL);
				}

				if (sc->SkillSuccess)
				{
					UnRegisterBuff(eDeBuff_Stun, to);
				}
				sc->AttackTime = 1;
				break;

			case AT_SKILL_INVISIBLE:
				if (sc->Helper.Type == MODEL_HELPER + 4 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_ATTACK_RIDE_ATTACK_MAGIC);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 2 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_SKILL_RIDER);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 3 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_SKILL_RIDER_FLY);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 37 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_FENRIR_ATTACK_MAGIC);
				}
				else
					SetAction(so, PLAYER_SKILL_VITALITY);

				if (sc->SkillSuccess)
				{
					if (!g_isCharacterBuff(to, eBuff_Cloaking))
					{
						if (so != to)
						{
							g_CharacterRegisterBuff(to, eBuff_Cloaking);
						}
					}
				}
				sc->AttackTime = 1;
				break;

			case AT_SKILL_REMOVAL_INVISIBLE:
				if (sc->Helper.Type == MODEL_HELPER + 4 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_ATTACK_RIDE_ATTACK_MAGIC);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 2 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_SKILL_RIDER);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 3 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_SKILL_RIDER_FLY);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 37 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_FENRIR_ATTACK_MAGIC);
				}
				else
					SetAction(so, PLAYER_ATTACK_REMOVAL);
				if (sc->SkillSuccess)
				{
					UnRegisterBuff(eBuff_Cloaking, to);
				}
				sc->AttackTime = 1;
				break;

			case AT_SKILL_MANA:
				if (sc->Helper.Type == MODEL_HELPER + 4 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_ATTACK_RIDE_ATTACK_MAGIC);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 2 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_SKILL_RIDER);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 3 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_SKILL_RIDER_FLY);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 37 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_FENRIR_ATTACK_MAGIC);
				}
				else
					SetAction(so, PLAYER_SKILL_VITALITY);
				if (sc->SkillSuccess)
				{
					g_CharacterRegisterBuff(so, eBuff_AddMana);

					CreateEffect(0, MODEL_MANA_RUNE, so->Position, so->Angle, so->Light);
				}
				sc->AttackTime = 1;
				break;

			case AT_SKILL_REMOVAL_BUFF:
				if (sc->SkillSuccess)
				{
					UnRegisterBuff(eBuff_Attack, to);
					UnRegisterBuff(eBuff_Defense, to);
					UnRegisterBuff(eBuff_HpRecovery, to);
					UnRegisterBuff(eBuff_PhysDefense, to);
					UnRegisterBuff(eBuff_AddCriticalDamage, to);
					UnRegisterBuff(eBuff_AddMana, to);
				}
				if (sc->Helper.Type == MODEL_HELPER + 4 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_ATTACK_RIDE_ATTACK_MAGIC);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 2 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_SKILL_RIDER);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 3 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_SKILL_RIDER_FLY);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 37 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_FENRIR_ATTACK_MAGIC);
				}
				else
					SetAction(so, PLAYER_SKILL_VITALITY);
				sc->AttackTime = 1;
				break;
			case AT_SKILL_IMPROVE_AG:
				if (!g_isCharacterBuff(to, eBuff_AddAG))
				{
					DeleteEffect(BITMAP_LIGHT, to, 2);
					DeleteJoint(BITMAP_JOINT_HEALING, to, 9);

					CreateEffect(0, BITMAP_LIGHT, to->Position, to->Angle, to->Light, 2, to);
				}

				g_CharacterRegisterBuff(to, eBuff_AddAG);

				SetPlayerMagic(0, sc);
				so->AnimationFrame = 0;
				sc->AttackTime = 1;
				break;
			case AT_SKILL_ADD_CRITICAL:
				g_CharacterRegisterBuff(to, eBuff_AddCriticalDamage);

				if (sc->Helper.Type == MODEL_HELPER + 4 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_ATTACK_RIDE_ATTACK_MAGIC);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 37 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_FENRIR_ATTACK_MAGIC);
				}
				else
				{
					SetAction(so, PLAYER_SKILL_HAND1);
				}
				sc->AttackTime = 1;
				break;
			case AT_SKILL_PARTY_TELEPORT:
				if ((sc->Helper.Type >= MODEL_HELPER + 2 && sc->Helper.Type <= MODEL_HELPER + 4) && !sc->SafeZone)
				{
					SetAction(so, PLAYER_ATTACK_RIDE_TELEPORT);
				}
				else if (sc->Helper.Type == MODEL_HELPER + 37 && !sc->SafeZone)
				{
					SetAction(so, PLAYER_FENRIR_ATTACK_DARKLORD_TELEPORT);
				}
				else
				{
					SetAction(so, PLAYER_ATTACK_TELEPORT);
				}
				if (so->Type != MODEL_PLAYER)
				{
					SetPlayerAttack(0, sc);
					so->AnimationFrame = 0;
				}
				sc->AttackTime = 1;
				break;
			case AT_SKILL_LIFE_UP:
			case AT_SKILL_LIFE_UP + 1:
			case AT_SKILL_LIFE_UP + 2:
			case AT_SKILL_LIFE_UP + 3:
			case AT_SKILL_LIFE_UP + 4:
			case AT_SKILL_VITALITY:
				if (!g_isCharacterBuff(to, eBuff_HpRecovery))
				{
					DeleteEffect(BITMAP_LIGHT, to, 1);

					CreateEffect(0, BITMAP_LIGHT, to->Position, to->Angle, to->Light, 1, to);
				}

				g_CharacterRegisterBuff(to, eBuff_HpRecovery);

				SetAction(so, PLAYER_SKILL_VITALITY);
				sc->AttackTime = 1;
				break;

			case AT_SKILL_RIDER:
				if (GET_WORLD == WD_8TARKAN || GET_WORLD == WD_10HEAVEN || g_Direction.m_CKanturu.IsMayaScene())
					SetAction(so, PLAYER_SKILL_RIDER_FLY);
				else
					SetAction(so, PLAYER_SKILL_RIDER);
				sc->AttackTime = 1;
				break;

			case AT_SKILL_BOSS:
				sc->AttackTime = 1;
				break;

			case AT_SKILL_COMBO:
				CreateEffect(0, MODEL_COMBO, so->Position, so->Angle, so->Light);
				PlayBackground(SOUND_COMBO);
				break;
			case AT_SKILL_BLAST_HELL_BEGIN:
				SetAction(so, PLAYER_SKILL_HELL_BEGIN);
				PlayBackground(SOUND_NUKE1);
				so->SpecialSkillTick = g_CurrentTime;
				break;
			case AT_SKILL_BLAST_HELL:
				SetAction(so, PLAYER_SKILL_HELL_START);
				sc->AttackTime = 1;
				break;
			case AT_SKILL_FIRE_SCREAM_UP:
			case AT_SKILL_FIRE_SCREAM_UP + 1:
			case AT_SKILL_FIRE_SCREAM_UP + 2:
			case AT_SKILL_FIRE_SCREAM_UP + 3:
			case AT_SKILL_FIRE_SCREAM_UP + 4:
			case AT_SKILL_DARK_SCREAM:
				{
					if (sc->Helper.Type == MODEL_HELPER + 37)
					{
						SetAction(so, PLAYER_FENRIR_ATTACK_DARKLORD_STRIKE);
					}
					else if ((sc->Helper.Type >= MODEL_HELPER + 2) && (sc->Helper.Type <= MODEL_HELPER + 4))
					{
						SetAction(so, PLAYER_ATTACK_RIDE_STRIKE);
					}
					else
					{
						SetAction(so, PLAYER_ATTACK_STRIKE);
					}
					PlayBackground(SOUND_FIRE_SCREAM);
				}
				break;
			case AT_SKILL_EXPLODE:
				{
					EVector3 Pos;
					mu_float addx, addz;
					for (mu_int32 iaa = 0; iaa < 3; ++iaa)
					{
						addx = GetLargeRand(160);
						addz = GetLargeRand(160);
						Vector(to->Position[0] + addx, to->Position[1] + addz, to->Position[2], Pos);

						CreateBomb(0, Pos, true);
					}
				}
				break;
			case AT_SKILL_INFINITY_ARROW:
				{
					CharacterMachine->InfinityArrowAdditionalMana = 10;
					CreateEffect(0, MODEL_INFINITY_ARROW, so->Position, so->Angle, so->Light, 0, so);
					PlayBackground(SOUND_INFINITYARROW);
				}
				break;
			case AT_SKILL_ALICE_CHAINLIGHTNING_UP:
			case AT_SKILL_ALICE_CHAINLIGHTNING_UP + 1:
			case AT_SKILL_ALICE_CHAINLIGHTNING_UP + 2:
			case AT_SKILL_ALICE_CHAINLIGHTNING_UP + 3:
			case AT_SKILL_ALICE_CHAINLIGHTNING_UP + 4:
			case AT_SKILL_ALICE_CHAINLIGHTNING:
				{
					sc->AttackTime = 1;

					switch (sc->Helper.Type)
					{
					case MODEL_HELPER + 2:
						SetAction(so, PLAYER_SKILL_CHAIN_LIGHTNING_UNI);
						break;
					case MODEL_HELPER + 3:
						SetAction(so, PLAYER_SKILL_CHAIN_LIGHTNING_DINO);
						break;
					case MODEL_HELPER + 37:
						SetAction(so, PLAYER_SKILL_CHAIN_LIGHTNING_FENRIR);
						break;
					default:
						SetAction(so, PLAYER_SKILL_CHAIN_LIGHTNING);
						break;
					}

					OBJECT *pTempObject = so;

					mu_uint32 i = 0;
					for (auto it = ViewportMagic.TargetsList.begin(); it != ViewportMagic.TargetsList.end(); ++it, ++i)
					{
						mu_int32 charIndex = FindCharacterIndex(*it);
						CHARACTER *pTargetChar = &CharactersClient[charIndex];
						OBJECT *pTargetObject = &pTargetChar->Object;

						if (pTempObject != pTargetObject &&
							pTargetObject != nullptr &&
							pTargetObject->Flags.Get(OBJECT::eOF_LIVE) == true)
						{
							EVector3 vAngle;
							Vector(-60.0f, 0.0f, so->Angle[2], vAngle);

							CreateEffect(0, MODEL_CHAIN_LIGHTNING, so->Position, vAngle, so->Light, i,
								pTempObject, -1, 0, 0, 0, 0.0f, *it);
						}

						pTempObject = pTargetObject;
					}

					PlayBackground(SOUND_SKILL_CHAIN_LIGHTNING);
				}
				break;
			case AT_SKILL_ALICE_SLEEP_UP:
			case AT_SKILL_ALICE_SLEEP_UP + 1:
			case AT_SKILL_ALICE_SLEEP_UP + 2:
			case AT_SKILL_ALICE_SLEEP_UP + 3:
			case AT_SKILL_ALICE_SLEEP_UP + 4:
			case AT_SKILL_ALICE_SLEEP:
			case AT_SKILL_ALICE_BLIND:
			case AT_SKILL_ALICE_THORNS:
			case AT_SKILL_ALICE_BERSERKER:
				{
					sc->AttackTime = 1;

					switch (sc->Helper.Type)
					{
					case MODEL_HELPER + 2:
						SetAction(so, PLAYER_SKILL_SLEEP_UNI);
						break;
					case MODEL_HELPER + 3:
						SetAction(so, PLAYER_SKILL_SLEEP_DINO);
						break;
					case MODEL_HELPER + 37:
						SetAction(so, PLAYER_SKILL_SLEEP_FENRIR);
						break;
					default:
						SetAction(so, PLAYER_SKILL_SLEEP);
						break;
					}

					if (ViewportMagic.MagicNumber == AT_SKILL_ALICE_SLEEP
						|| (AT_SKILL_ALICE_SLEEP_UP <= ViewportMagic.MagicNumber && ViewportMagic.MagicNumber <= AT_SKILL_ALICE_SLEEP_UP + 4)
						)
					{
						PlayBackground(SOUND_SUMMON_SKILL_SLEEP);
					}
					else
						if (ViewportMagic.MagicNumber == AT_SKILL_ALICE_BLIND)
						{
							PlayBackground(SOUND_SUMMON_SKILL_BLIND);
						}
						else
						{
							PlayBackground(SOUND_SUMMON_SKILL_THORNS);
						}
				}
				break;
			case AT_SKILL_SWELL_OF_MAGICPOWER:
				{
					SetAction(so, PLAYER_SKILL_SWELL_OF_MP);

					EVector3 vLight;
					Vector(0.3f, 0.2f, 0.9f, vLight);
					CreateEffect(0, MODEL_SWELL_OF_MAGICPOWER, so->Position, so->Angle, vLight, 0, so);
					PlayBackground(SOUND_SKILL_SWELL_OF_MAGICPOWER);
				}
				break;
			case AT_SKILL_DOPPELGANGER_SELFDESTRUCTION:
				{
					SetAction(so, MONSTER01_APEAR);
					so->Flags.Clear(OBJECT::eOF_ACTIONSTART);
				}
				break;
			case AT_SKILL_GAOTIC:
				{
					if (so->Type == MODEL_PLAYER)
					{
						if (sc->Helper.Type == MODEL_HELPER + 37)
						{
							SetAction(&sc->Object, PLAYER_FENRIR_ATTACK_DARKLORD_STRIKE);
						}
						else if ((sc->Helper.Type >= MODEL_HELPER + 2) && (sc->Helper.Type <= MODEL_HELPER + 4))
						{
							SetAction(&sc->Object, PLAYER_ATTACK_RIDE_STRIKE);
						}
						else
						{
							SetAction(&sc->Object, PLAYER_ATTACK_STRIKE);
						}
					}
					else
					{
						SetPlayerAttack(0, sc);
					}

					OBJECT* o = so;
					EVector3 Light, Position, P, dp;

					EBone Matrix;
					Vector(0.0f, -20.0f, 0.0f, P);
					Vector(0.0f, 0.0f, 0.0f, P);
					AngleMatrix(o->Angle, Matrix);
					VectorRotate(P, Matrix, dp);
					VectorAdd(dp, o->Position, Position);

					Vector(0.5f, 0.5f, 0.5f, Light);
					for (mu_int32 i = 0; i < 5; ++i)
					{
						CreateEffect(0, BITMAP_SHINY + 6, Position, o->Angle, Light, 3, o, -1, 0, 0, 0, 0.5f);
					}

					VectorCopy(o->Position, Position);

					for (mu_int32 i = 0; i < 8; ++i)
					{
						Position[0] = (o->Position[0] - 119.0f) + (mu_float)(GetLargeRand(240));
						Position[2] = (o->Position[2] + 49.0f) + (mu_float)(GetLargeRand(60));
						CreateJoint(0, BITMAP_2LINE_GHOST, Position, o->Position, o->Angle, 0, o, 20.0f, o->PKKey, 0, o->m_bySkillSerialNum);
					}
					if (sc == Hero && SelectedCharacter != -1)
					{
						EVector3 Pos;
						CHARACTER *st = &CharactersClient[SelectedCharacter];
						VectorCopy(st->Object.Position, Pos);
						CreateBomb(0, Pos, true);
					}
					PlayBackground(SOUND_SKILL_CAOTIC);
				}
				break;
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_MAGIC2:
		{
			GS2CL_VIEWPORT_MAGIC2 ViewportMagic2;

			if (ViewportMagic2.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 scindex = FindCharacterIndex(ViewportMagic2.Key);
			if (scindex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER *sc = &CharactersClient[scindex];
			OBJECT *so = &sc->Object;

			CHARACTER *tc = nullptr;
			OBJECT *to = nullptr;

			mu_int32 tcindex = ViewportMagic2.TargetKey < 0 ? MAX_CHARACTERS_CLIENT : FindCharacterIndex(ViewportMagic2.TargetKey);
			if (tcindex != MAX_CHARACTERS_CLIENT)
			{
				tc = &CharactersClient[tcindex];
				to = &tc->Object;
			}

			sc->EnabledInternalAction = INTERNAL_ACTION::IA_NONE;
			sc->Skill = ViewportMagic2.MagicNumber;
			
			sc->Path.PathNum = 0;
			sc->Path.CurrentPath = 0;
			sc->Path.CurrentPathFloat = 0;
			sc->Movement = false;

			if (tc != nullptr)
			{
				so->Angle[2] = CreateAngle(so->Position[0], so->Position[1], to->Position[0], to->Position[1]);
				sc->SkillX = tc->PositionX;
				sc->SkillY = tc->PositionY;
			}
			else
			{
				so->Angle[2] = ViewportMagic2.Dir;
				sc->SkillX = ViewportMagic2.PositionX;
				sc->SkillY = ViewportMagic2.PositionY;
			}

			sc->AttackTime = 1;
			sc->TargetCharacter = tcindex == MAX_CHARACTERS_CLIENT ? -1 : tcindex;
			sc->AttackFlag = ATTACK_FAIL;

			if (so->Type == MODEL_PLAYER)
			{
				switch (ViewportMagic2.MagicNumber)
				{
				case AT_SKILL_MANY_ARROW_UP:
				case AT_SKILL_MANY_ARROW_UP + 1:
				case AT_SKILL_MANY_ARROW_UP + 2:
				case AT_SKILL_MANY_ARROW_UP + 3:
				case AT_SKILL_MANY_ARROW_UP + 4:
				case AT_SKILL_CROSSBOW:
					SetPlayerAttack(0, sc);
					break;
				case AT_SKILL_BLAST_CROSSBOW4:
					SetPlayerAttack(0, sc);
					break;
				case AT_SKILL_BLAST_POISON:
				case AT_SKILL_ICE_UP:
				case AT_SKILL_ICE_UP + 1:
				case AT_SKILL_ICE_UP + 2:
				case AT_SKILL_ICE_UP + 3:
				case AT_SKILL_ICE_UP + 4:
				case AT_SKILL_BLAST_FREEZE:
					SetPlayerMagic(0, sc);
					break;

				case AT_SKILL_PIERCING:
					SetPlayerAttack(0, sc);
					break;

				case AT_SKILL_FLASH:
					SetAction(so, PLAYER_SKILL_FLASH);
					break;

				case AT_SKILL_HELL_FIRE_UP:
				case AT_SKILL_HELL_FIRE_UP + 1:
				case AT_SKILL_HELL_FIRE_UP + 2:
				case AT_SKILL_HELL_FIRE_UP + 3:
				case AT_SKILL_HELL_FIRE_UP + 4:
				case AT_SKILL_HELL:
					SetAction(so, PLAYER_SKILL_HELL);
					break;

				case AT_SKILL_INFERNO:
					SetAction(so, PLAYER_SKILL_INFERNO);
					break;
				case AT_SKILL_TORNADO_SWORDA_UP:
				case AT_SKILL_TORNADO_SWORDA_UP + 1:
				case AT_SKILL_TORNADO_SWORDA_UP + 2:
				case AT_SKILL_TORNADO_SWORDA_UP + 3:
				case AT_SKILL_TORNADO_SWORDA_UP + 4:

				case AT_SKILL_TORNADO_SWORDB_UP:
				case AT_SKILL_TORNADO_SWORDB_UP + 1:
				case AT_SKILL_TORNADO_SWORDB_UP + 2:
				case AT_SKILL_TORNADO_SWORDB_UP + 3:
				case AT_SKILL_TORNADO_SWORDB_UP + 4:
				case AT_SKILL_WHEEL:
					{
						SetAction(so, PLAYER_ATTACK_SKILL_WHEEL);

						so->Weapon = sc->Weapon[0].Type - MODEL_SWORD;
						so->WeaponLevel = (mu_uint8)sc->Weapon[0].Level;
						so->WeaponExcOption = sc->Weapon[0].ExcOption;
						so->WeaponSetOption = sc->Weapon[0].SetOption;
						CreateEffect(0, MODEL_SKILL_WHEEL1, so->Position, so->Angle, so->Light, 0, so, so->PKKey, FindHotKey((sc->Skill)));
					}
					break;

				case AT_SKILL_EVIL_SPIRIT_UP_M:
				case AT_SKILL_EVIL_SPIRIT_UP_M + 1:
				case AT_SKILL_EVIL_SPIRIT_UP_M + 2:
				case AT_SKILL_EVIL_SPIRIT_UP_M + 3:
				case AT_SKILL_EVIL_SPIRIT_UP_M + 4:
				case AT_SKILL_EVIL_SPIRIT_UP:
				case AT_SKILL_EVIL_SPIRIT_UP + 1:
				case AT_SKILL_EVIL_SPIRIT_UP + 2:
				case AT_SKILL_EVIL_SPIRIT_UP + 3:
				case AT_SKILL_EVIL_SPIRIT_UP + 4:
				case AT_SKILL_EVIL:
					{
						SetPlayerMagic(0, sc);
						CreateEffect(0, MODEL_EVILSPIRIT, so->Position, so->Angle, so->Light, 0, so, -1, ViewportMagic2.MagicNumber);
					}
					break;

				case AT_SKILL_FIRE_SCREAM_UP:
				case AT_SKILL_FIRE_SCREAM_UP + 1:
				case AT_SKILL_FIRE_SCREAM_UP + 2:
				case AT_SKILL_FIRE_SCREAM_UP + 3:
				case AT_SKILL_FIRE_SCREAM_UP + 4:
				case AT_SKILL_DARK_SCREAM:
					{
						if (sc->Helper.Type == MODEL_HELPER + 37)
						{
							SetAction(so, PLAYER_FENRIR_ATTACK_DARKLORD_STRIKE);
						}
						else if ((sc->Helper.Type >= MODEL_HELPER + 2) && (sc->Helper.Type <= MODEL_HELPER + 4))
						{
							SetAction(so, PLAYER_ATTACK_RIDE_STRIKE);
						}
						else
						{
							SetAction(so, PLAYER_ATTACK_STRIKE);
						}
						PlayBackground(SOUND_FIRE_SCREAM);
					}
					break;

				case AT_SKILL_THUNDER_STRIKE:
					if (sc->Helper.Type == MODEL_HELPER + 37)
						SetAction(so, PLAYER_FENRIR_ATTACK_DARKLORD_FLASH);
					else
						SetAction(so, PLAYER_SKILL_FLASH);
					break;

				case AT_SKILL_ASHAKE_UP:
				case AT_SKILL_ASHAKE_UP + 1:
				case AT_SKILL_ASHAKE_UP + 2:
				case AT_SKILL_ASHAKE_UP + 3:
				case AT_SKILL_ASHAKE_UP + 4:
				case AT_SKILL_DARK_HORSE:
					SetAction(so, PLAYER_ATTACK_DARKHORSE);
					PlayBackground(SOUND_EARTH_QUAKE);
					break;

				case AT_SKILL_ANGER_SWORD_UP:
				case AT_SKILL_ANGER_SWORD_UP + 1:
				case AT_SKILL_ANGER_SWORD_UP + 2:
				case AT_SKILL_ANGER_SWORD_UP + 3:
				case AT_SKILL_ANGER_SWORD_UP + 4:
				case AT_SKILL_FURY_STRIKE:
					{
						SetAction(so, PLAYER_ATTACK_SKILL_FURY_STRIKE);

						so->Weapon = sc->Weapon[0].Type - MODEL_SWORD;
						so->WeaponLevel = (mu_uint8)sc->Weapon[0].Level;
						so->WeaponExcOption = sc->Weapon[0].ExcOption;
						so->WeaponSetOption = sc->Weapon[0].SetOption;
						CreateEffect(0, MODEL_SKILL_FURY_STRIKE, so->Position, so->Angle, so->Light, 0, so, so->PKKey, FindHotKey(sc->Skill));

						PlayBackground(SOUND_FURY_STRIKE1);
					}
					break;

				case AT_SKILL_BLOW_OF_DESTRUCTION:
					SetAction(so, PLAYER_SKILL_BLOW_OF_DESTRUCTION);
					break;

				case AT_SKILL_SPEAR:
					if (sc->Helper.Type == MODEL_HELPER + 37)
						SetAction(so, PLAYER_FENRIR_ATTACK_SPEAR);
					else
						SetAction(so, PLAYER_ATTACK_SKILL_SPEAR);
					break;

				case AT_SKILL_BLOOD_ATT_UP:
				case AT_SKILL_BLOOD_ATT_UP + 1:
				case AT_SKILL_BLOOD_ATT_UP + 2:
				case AT_SKILL_BLOOD_ATT_UP + 3:
				case AT_SKILL_BLOOD_ATT_UP + 4:
				case AT_SKILL_REDUCEDEFENSE:
					{
						for (auto it = ViewportMagic2.TargetsList.begin(); it != ViewportMagic2.TargetsList.end(); ++it)
						{
							CHARACTER *pTargetChar = &CharactersClient[FindCharacterIndex(*it)];
							OBJECT *pTargetObject = &pTargetChar->Object;

							DeleteEffect(BITMAP_SKULL, pTargetObject, 0);
							EVector3 Light = { 1.0f, 1.0f, 1.0f };
							CreateEffect(0, BITMAP_SKULL, pTargetObject->Position, pTargetObject->Angle, Light, 0, pTargetObject);

							PlayObject(SOUND_BLOODATTACK, pTargetObject);
							g_CharacterRegisterBuff(pTargetObject, eDeBuff_Defense);
						}

						if (so->Type != MODEL_PLAYER)
						{
							SetPlayerAttack(0, sc);
							so->AnimationFrame = 0;
							sc->AttackTime = 1;
						}
						else
						{
							SetAction(so, PLAYER_ATTACK_SKILL_WHEEL);
						}
					}
					break;

				case AT_SKILL_POWER_SLASH_UP:
				case AT_SKILL_POWER_SLASH_UP + 1:
				case AT_SKILL_POWER_SLASH_UP + 2:
				case AT_SKILL_POWER_SLASH_UP + 3:
				case AT_SKILL_POWER_SLASH_UP + 4:
				case AT_SKILL_ICE_BLADE:
					SetAction(so, PLAYER_ATTACK_TWO_HAND_SWORD_TWO);
					break;

				case AT_SKILL_BLOW_UP:
				case AT_SKILL_BLOW_UP + 1:
				case AT_SKILL_BLOW_UP + 2:
				case AT_SKILL_BLOW_UP + 3:
				case AT_SKILL_BLOW_UP + 4:
				case AT_SKILL_ONETOONE:
					SetAction(so, PLAYER_ATTACK_ONETOONE);
					break;

				case AT_SKILL_STUN:
					SetAction(so, PLAYER_SKILL_VITALITY);
					break;

				case AT_SKILL_INVISIBLE:
					SetAction(so, PLAYER_SKILL_VITALITY);
					break;

				case AT_SKILL_PLASMA_STORM_FENRIR:
					SetAction_Fenrir_Skill(sc, so);
					break;

				case AT_SKILL_GAOTIC:
					{
						if (sc->Helper.Type == MODEL_HELPER + 37)
						{
							SetAction(&sc->Object, PLAYER_FENRIR_ATTACK_DARKLORD_STRIKE);
						}
						else if ((sc->Helper.Type >= MODEL_HELPER + 2) && (sc->Helper.Type <= MODEL_HELPER + 4))
						{
							SetAction(&sc->Object, PLAYER_ATTACK_RIDE_STRIKE);
						}
						else
						{
							SetAction(&sc->Object, PLAYER_ATTACK_STRIKE);
						}

						OBJECT* o = so;
						EVector3 Light, Position, P, dp;

						EBone Matrix;
						Vector(0.0f, -20.0f, 0.0f, P);
						Vector(0.0f, 0.0f, 0.0f, P);
						AngleMatrix(o->Angle, Matrix);
						VectorRotate(P, Matrix, dp);
						VectorAdd(dp, o->Position, Position);

						Vector(0.5f, 0.5f, 0.5f, Light);
						for (mu_int32 i = 0; i < 5; ++i)
						{
							CreateEffect(0, BITMAP_SHINY + 6, Position, o->Angle, Light, 3, o, -1, 0, 0, 0, 0.5f);
						}

						VectorCopy(o->Position, Position);

						for (mu_int32 i = 0; i < 8; ++i)
						{
							Position[0] = (o->Position[0] - 119.0f) + (mu_float)(GetLargeRand(240));
							Position[2] = (o->Position[2] + 49.0f) + (mu_float)(GetLargeRand(60));
							CreateJoint(0, BITMAP_2LINE_GHOST, Position, o->Position, o->Angle, 0, o, 20.0f, o->PKKey, 0, o->m_bySkillSerialNum);
						}

						if (sc == Hero && SelectedCharacter != -1)
						{
							EVector3 Pos;
							CHARACTER *st = &CharactersClient[SelectedCharacter];
							VectorCopy(st->Object.Position, Pos);
							CreateBomb(0, Pos, true);
						}

						PlayBackground(SOUND_SKILL_CAOTIC);
					}
					break;
				case AT_SKILL_MULTI_SHOT:
					{
						SetPlayerBow(sc);
						OBJECT* o = so;
						EVector3 Light, Position, P, dp;
						EVector3 pos;

						MUModel *b = &MODELS::Data[MODEL_PLAYER];
						b->TransformByObjectBone(0, pos, o, 47);
						pos[2] -= 120.0f;

						EBone Matrix;
						Vector(0.0f, 20.0f, 0.0f, P);
						AngleMatrix(o->Angle, Matrix);
						VectorRotate(P, Matrix, dp);
						VectorAdd(dp, pos, Position);
						Vector(0.8f, 0.9f, 1.6f, Light);
						CreateEffect(0, MODEL_MULTI_SHOT3, Position, o->Angle, Light, 0);
						CreateEffect(0, MODEL_MULTI_SHOT3, Position, o->Angle, Light, 0);

						Vector(0.0f, -20.0f, 0.0f, P);
						Vector(0.0f, 0.0f, 0.0f, P);
						AngleMatrix(o->Angle, Matrix);
						VectorRotate(P, Matrix, dp);
						VectorAdd(dp, pos, Position);

						CreateEffect(0, MODEL_MULTI_SHOT1, Position, o->Angle, Light, 0);
						CreateEffect(0, MODEL_MULTI_SHOT1, Position, o->Angle, Light, 0);
						CreateEffect(0, MODEL_MULTI_SHOT1, Position, o->Angle, Light, 0);

						Vector(0.0f, 20.0f, 0.0f, P);
						AngleMatrix(o->Angle, Matrix);
						VectorRotate(P, Matrix, dp);
						VectorAdd(dp, pos, Position);
						CreateEffect(0, MODEL_MULTI_SHOT2, Position, o->Angle, Light, 0);
						CreateEffect(0, MODEL_MULTI_SHOT2, Position, o->Angle, Light, 0);

						Vector(0.0f, -120.0f, 145.0f, P);
						AngleMatrix(o->Angle, Matrix);
						VectorRotate(P, Matrix, dp);
						VectorAdd(dp, pos, Position);

						CreateEffect(0, MODEL_BLADE_SKILL, Position, o->Angle, Light, 1, o, scindex);
					}
					break;
				case AT_SKILL_RECOVER:
					{
						EVector3 Light, Position, P, dp;
						EVector3 vFirePosition;

						EBone Matrix;

						OBJECT* o = so;

						Vector(0.4f, 0.6f, 1.0f, Light);
						Vector(0.0f, 0.0f, 0.0f, P);
						AngleMatrix(o->Angle, Matrix);
						VectorRotate(P, Matrix, dp);
						VectorAdd(dp, o->Position, Position);
						Position[2] += 130;
						VectorCopy(o->Position, Position);
						for (mu_int32 i = 0; i < 19; ++i)
						{
							CreateJoint(0, BITMAP_FLARE, Position, Position, o->Angle, 47, o, 40, 2);
						}
						Vector(0.3f, 0.2f, 0.1f, Light);
						CreateEffect(0, MODEL_SUMMON, Position, o->Angle, Light, 0);
						CreateEffect(0, BITMAP_TWLIGHT, Position, o->Angle, Light, 0);
						CreateEffect(0, BITMAP_TWLIGHT, Position, o->Angle, Light, 1);
						CreateEffect(0, BITMAP_TWLIGHT, Position, o->Angle, Light, 2);

						const MUModel *b = &MODELS::Data[o->Type];
						const mu_int32 iNumBones = b->_BoneNonDummyEnd;
						for (mu_int32 i = 0; i < 2; ++i)
						{
							b->TransformByObjectBoneDirect(0, vFirePosition, o, GetLargeRand(iNumBones));
							CreateEffect(0, BITMAP_FLARE, vFirePosition, o->Angle, Light, 3);
						}
						PlayBackground(SOUND_SKILL_RECOVER);
					}
					break;


				case AT_SKILL_ALICE_LIGHTNINGORB:
					{
						switch (sc->Helper.Type)
						{
						case MODEL_HELPER + 2:
							SetAction(so, PLAYER_SKILL_LIGHTNING_ORB_UNI);
							break;
						case MODEL_HELPER + 3:
							SetAction(so, PLAYER_SKILL_LIGHTNING_ORB_DINO);
							break;
						case MODEL_HELPER + 37:
							SetAction(so, PLAYER_SKILL_LIGHTNING_ORB_FENRIR);
							break;
						default:
							SetAction(so, PLAYER_SKILL_LIGHTNING_ORB);
							break;
						}

#ifdef ENABLE_SKILLPROCESSORS
						CreateSkill(sc, sc->TargetCharacter, sc->SkillX, sc->SkillY, ViewportMagic2.MagicNumber, so->Position, so->Angle);
#else
						CHARACTER *tc = &CharactersClient[sc->TargetCharacter];
						OBJECT *to = &tc->Object;

						EVector3 vLight;
						Vector(1.0f, 1.0f, 1.0f, vLight);

						CreateEffect(0, MODEL_LIGHTNING_ORB, so->Position, so->Angle, vLight, 0, to);
#endif
					}
					break;

				case AT_SKILL_ALICE_DRAINLIFE_UP:
				case AT_SKILL_ALICE_DRAINLIFE_UP + 1:
				case AT_SKILL_ALICE_DRAINLIFE_UP + 2:
				case AT_SKILL_ALICE_DRAINLIFE_UP + 3:
				case AT_SKILL_ALICE_DRAINLIFE_UP + 4:
				case AT_SKILL_ALICE_DRAINLIFE:
					{
						switch (sc->Helper.Type)
						{
						case MODEL_HELPER + 2:
							SetAction(so, PLAYER_SKILL_DRAIN_LIFE_UNI);
							break;
						case MODEL_HELPER + 3:
							SetAction(so, PLAYER_SKILL_DRAIN_LIFE_DINO);
							break;
						case MODEL_HELPER + 37:
							SetAction(so, PLAYER_SKILL_DRAIN_LIFE_FENRIR);
							break;
						default:
							SetAction(so, PLAYER_SKILL_DRAIN_LIFE);
							break;
						}

#ifdef ENABLE_SKILLPROCESSORS
						CreateSkill(sc, sc->TargetCharacter, sc->SkillX, sc->SkillY, ViewportMagic2.MagicNumber, so->Position, so->Angle);
#else
						CHARACTER *pTargetChar = &CharactersClient[sc->TargetCharacter];
						OBJECT* pSourceObj = so;
						pSourceObj->Owner = &(pTargetChar->Object);

						CreateEffect(0, MODEL_ALICE_DRAIN_LIFE, pSourceObj->Position, pSourceObj->Angle, pSourceObj->Light, 0, pSourceObj);
#endif
					}
					break;

				case AT_SKILL_ALICE_WEAKNESS:
				case AT_SKILL_ALICE_ENERVATION:
					switch (sc->Helper.Type)
					{
					case MODEL_HELPER + 2:
						SetAction(so, PLAYER_SKILL_SLEEP_UNI);
						break;
					case MODEL_HELPER + 3:
						SetAction(so, PLAYER_SKILL_SLEEP_DINO);
						break;
					case MODEL_HELPER + 37:
						SetAction(so, PLAYER_SKILL_SLEEP_FENRIR);
						break;
					default:
						SetAction(so, PLAYER_SKILL_SLEEP);
						break;
					}
					break;

				case AT_SKILL_SUMMON_EXPLOSION:
				case AT_SKILL_SUMMON_REQUIEM:
				case AT_SKILL_SUMMON_POLLUTION:
					{
						g_SummonSystem.CastSummonSkill(0, ViewportMagic2.MagicNumber, sc, so, ViewportMagic2.PositionX, ViewportMagic2.PositionY);
					}
					break;
				case AT_SKILL_FLAME_STRIKE:
					SetAction(so, PLAYER_SKILL_FLAMESTRIKE);
					break;
				case AT_SKILL_GIGANTIC_STORM:
					SetAction(so, PLAYER_SKILL_GIGANTICSTORM);
					break;
				case AT_SKILL_LIGHTNING_SHOCK_UP:
				case AT_SKILL_LIGHTNING_SHOCK_UP + 1:
				case AT_SKILL_LIGHTNING_SHOCK_UP + 2:
				case AT_SKILL_LIGHTNING_SHOCK_UP + 3:
				case AT_SKILL_LIGHTNING_SHOCK_UP + 4:
				case AT_SKILL_LIGHTNING_SHOCK:
					SetAction(so, PLAYER_SKILL_LIGHTNING_SHOCK);
					break;
				default:
					SetPlayerMagic(0, sc);
					break;
				}
				so->AnimationFrame = 0;
			}
			else
			{
				SetPlayerAttack(0, sc);
				so->AnimationFrame = 0;
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_DIE:
		{
			GS2CL_VIEWPORT_DIE ViewportDie;

			if (ViewportDie.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportDie.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* c = &CharactersClient[iIndex];
			OBJECT* o = &c->Object;

			c->EnabledInternalAction = INTERNAL_ACTION::IA_NONE;
			c->Dead = 1;
			c->Movement = false;

			c->m_byDieType = ViewportDie.Skill;

			if (InBloodCastle() == true)
			{
				FallingStartCharacter(c, o);
			}
			else if (InChaosCastle() == true)
			{
				FallingStartCharacter(c, o);
			}
			else
			{
#if 0 // FIX
				if (c == Hero && g_PortalMgr.IsPortalUsable())
				{
					g_PortalMgr.SaveRevivePosition();
				}
#endif

				o->Flags.Clear(OBJECT::eOF_ACTIONSTART);

				switch (ViewportDie.Skill)
				{
				case AT_SKILL_BLAST_HELL:
				case AT_SKILL_COMBO:
					o->Flags.Set(OBJECT::eOF_ACTIONSTART);
					c->StormTime = 0;
					o->Velocity = (GetLargeRand(5) + 10.0f)*0.1f;
					o->SpecialSkillTick = 0;

					Vector(0.0f, GetLargeRand(15) + 40.0f, 0.0f, o->Direction);
					VectorCopy(o->Position, o->m_vDeadPosition);
					break;
				}

				if (ViewportDie.Skill == AT_SKILL_BLAST_HELL || ViewportDie.Skill == AT_SKILL_COMBO)
				{
					mu_int32 TIndex = FindCharacterIndex(ViewportDie.KillerKey);
					if (TIndex == MAX_CHARACTERS_CLIENT) break;

					CHARACTER* tc = &CharactersClient[TIndex];
					OBJECT*    to = &tc->Object;

					o->Angle[2] = CreateAngle(o->Position[0], o->Position[1], to->Position[0], to->Position[1]);

					VectorCopy(o->Angle, o->HeadAngle);
				}
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_SPEED:
		{
			GS2CL_VIEWPORT_SPEED ViewportSpeed;

			if (ViewportSpeed.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportSpeed.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* c = &CharactersClient[iIndex];
			OBJECT* o = &c->Object;

			o->AttackSpeed = ViewportSpeed.AttackSpeed;
			o->MagicSpeed = ViewportSpeed.MagicSpeed;
		}
		break;

	case GS2CL_PROT_VIEWPORT_BUFFEFFECT:
		{
			GS2CL_VIEWPORT_BUFFEFFECT ViewportBuffEffect;

			if (ViewportBuffEffect.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportBuffEffect.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* c = &CharactersClient[iIndex];
			OBJECT* o = &c->Object;
			MUBuff &BuffMap = o->m_BuffMap;

			mu_boolean BuffStatus[eBuffState::eBuff_Count];
			mu_zeromem(BuffStatus, sizeof(BuffStatus));

			for (mu_uint32 n = 0; n < MAX_BUFF_SLOT_INDEX; ++n)
			{
				GS2CL_VIEWPORT_BUFFEFFECT::BuffData &data = ViewportBuffEffect.BuffList[n];

				if (data.BuffType > eBuffState::eBuffNone &&
					data.BuffType < eBuffState::eBuff_Count)
				{
					eBuffClass classType = g_IsBuffClass((eBuffState)data.BuffType);

					if (classType == eBuffClass_Count) continue;

					BuffStatus[data.BuffType] = true;

					RegisterBuff(0, (eBuffState)data.BuffType, data.AddEffect, o, data.EffectTime);
				}
			}

			eBuffState BuffUnregister[eBuffState::eBuff_Count];
			mu_int32 BuffUnregisterCount = 0;

			for (auto it = BuffMap.m_Buff.begin(); it != BuffMap.m_Buff.end(); ++it)
			{
				if (BuffStatus[it->first] == false)
				{
					BuffUnregister[BuffUnregisterCount++] = it->first;
				}
			}

			for (mu_uint32 n = 0; n < BuffUnregisterCount; ++n)
			{
				UnRegisterBuff(BuffUnregister[n], o);
			}

			if (c == Hero)
			{
				g_pBuffWindow->UpdateBuffList();
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_PKLEVEL:
		{
			GS2CL_VIEWPORT_PKLEVEL ViewportPkLevel;

			if (ViewportPkLevel.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportPkLevel.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* c = &CharactersClient[iIndex];

			c->PK = ViewportPkLevel.PkLevel;

			if(c == Hero)
			{
				g_MoveManager.Update();
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_TELEPORT:
		{
			GS2CL_VIEWPORT_TELEPORT ViewportTeleport;

			if (ViewportTeleport.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			g_pPickedItem.BackupPickedItem();

			Hero->JumpTime = 0;
			Hero->PositionX = ViewportTeleport.X;
			Hero->PositionY = ViewportTeleport.Y;
			Hero->EnabledInternalAction = INTERNAL_ACTION::IA_NONE;

			OBJECT *o = &Hero->Object;
			o->Position[0] = ((mu_float)(Hero->PositionX) + 0.5f)*g_TerrainScale;
			o->Position[1] = ((mu_float)(Hero->PositionY) + 0.5f)*g_TerrainScale;
			o->Angle[2] = ViewportTeleport.Direction;

			if (GET_WORLD >= WD_45CURSEDTEMPLE_LV1 && GET_WORLD <= WD_45CURSEDTEMPLE_LV6)
			{
				if (!(ViewportTeleport.Map >= WD_45CURSEDTEMPLE_LV1 &&
					ViewportTeleport.Map <= WD_45CURSEDTEMPLE_LV6))
				{
					g_CursedTemple.ResetCursedTemple();
					g_pNewUISystem->Hide(GAMEUI::INTERFACE_CURSEDTEMPLE_GAMESYSTEM);
				}
			}

			mu_int32 OldWorld = GET_WORLD;

			g_pSimpleChat->Clear();

			if (GET_WORLD != ViewportTeleport.Map)
			{
				EMusicBackend::Stop();
				g_Tasking.CleanSounds();
				g_Tasking.CleanEffects();
				g_Tasking.CleanBoids();
				g_Tasking.CleanPets();
				g_Tasking.CleanLeaves();
				g_Tasking.CleanOperates();
				g_Tasking.CleanWorldObjects();
				g_Tasking.CleanProxy();

				// Event Clear
				{
					g_pBloodCastleTimer->Reset();
				}

				if (g_Terrain.Load(ViewportTeleport.Map, 0) == false)
				{
					g_GameServerObject->Disconnect();
					break;
				}

				g_Tasking.ReleaseModelsMemory();

				if (InHellas() == true)
				{
					g_pCSWaterTerrain = new MUWaterTerrain();

					if (g_pCSWaterTerrain->Create() == false)
					{
						mu_assert(!_T("Failed to create water terrain!"));
					}
				}
				else
				{
					MU_SAFE_DELETE(g_pCSWaterTerrain);
				}

				g_pMyInventory->Configure();

				g_Tasking.SettingBoids();

				if (GET_WORLD == WD_34CRYWOLF_1ST)
				{
#if 0 // FIX
					SendRequestCrywolfInfo();
#endif
				}

				if ((InChaosCastle(OldWorld) == true && OldWorld != GET_WORLD) || InChaosCastle() == true)
				{
					PlayBackground(SOUND_CHAOS_ENVIR, true);

					g_pNewUISystem->Hide(GAMEUI::INTERFACE_FRIEND);

					SetCharacterClass(0, Hero);
					DeleteBug(&Hero->Object);
				}
				if (InChaosCastle() == false)
				{
					StopBackground(SOUND_CHAOSCASTLE);
					StopBackground(SOUND_CHAOS_ENVIR);
				}

				if (IsEmpireGuardian1() == false &&
					IsEmpireGuardian2() == false &&
					IsEmpireGuardian3() == false &&
					IsEmpireGuardian4() == false)
				{
					StopBackground(SOUND_EMPIREGUARDIAN_WEATHER_RAIN);
					StopBackground(SOUND_EMPIREGUARDIAN_WEATHER_FOG);
					StopBackground(SOUND_EMPIREGUARDIAN_WEATHER_STORM);
					StopBackground(SOUND_EMPIREGUARDIAN_INDOOR_SOUND);
				}

#if 0 // FIX
				matchEvent::CreateEventMatch(GET_WORLD);
#endif

				if (GET_WORLD >= WD_65DOPPLEGANGER1 && GET_WORLD <= WD_68DOPPLEGANGER4);
				else
				{
					mu_text Text[256];

					mu_sprintf_s(Text, mu_countof(Text), _T("") SPF_STRING _T("") SPF_STRING _T(""), GlobalText[484], GetMapName(GET_WORLD));

					g_pChatListBox->AddText(_T(""), Text, TYPE_SYSTEM_MESSAGE);
				}
			}

			if (GET_WORLD == -1 || Hero->Helper.Type != MODEL_HELPER + 3 || Hero->SafeZone)
			{
				o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
			}
			else
			{
				if (GET_WORLD == WD_8TARKAN || GET_WORLD == WD_10HEAVEN)
					o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 90.0f;
				else
					o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 30.0f;
			}

			LoadingWorld = 30;

			MouseUpdateTime = 0;
			MouseUpdateTimeMax = 6;

			if (GET_WORLD < WD_65DOPPLEGANGER1 || GET_WORLD > WD_68DOPPLEGANGER4)
			{
				g_pNewUISystem->Hide(GAMEUI::INTERFACE_DOPPELGANGER_FRAME);
			}

			if (GET_WORLD < WD_69EMPIREGUARDIAN1 || WD_72EMPIREGUARDIAN4 < GET_WORLD)
			{
				g_pNewUISystem->Hide(GAMEUI::INTERFACE_EMPIREGUARDIAN_TIMER);
			}

			g_pNewUISystem->HideAll();

			giPetManager::CreatePetDarkSpirit_Now(Hero);
			CreateEffect(0, BITMAP_MAGIC + 2, o->Position, o->Angle, o->Light, 0, o);
			o->Alpha = 0.0f;

			if (EventMask != ViewportTeleport.EventMask)
			{
				EventMask = ViewportTeleport.EventMask;
				g_Tasking.CleanBoids();
			}

			SelectedItem = -1;
			SelectedNpc = -1;
			SelectedCharacter = -1;
			SelectedOperate = -1;
			Attacking = -1;

#if 0 // FIX
			RepairEnable = 0;
#endif

			Hero->Movement = false;
			SetPlayerStop(0, Hero);

			if (OldWorld != GET_WORLD)
			{
				g_pMapName->Show();
			}

#if 0 // FIX
			CreateMyGensInfluenceGroundEffect();

			if (battleCastle::InBattleCastle() && battleCastle::IsBattleCastleStart())
			{
				Hero->EtcPart = iEtcPart;
			}

			if (GET_WORLD == WD_30BATTLECASTLE)
			{
				if (g_pSiegeWarfare)
					g_pSiegeWarfare->CreateMiniMapUI();
			}
			else
			{
				g_pSiegeWarfare->InitMiniMapUI();
			}
#endif
		}
		break;

	case GS2CL_PROT_VIEWPORT_TELEPORTEND:
		{
			GS2CL_VIEWPORT_TELEPORTEND ViewportTeleportEnd;

			if (ViewportTeleportEnd.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportTeleportEnd.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER* c = &CharactersClient[iIndex];
			OBJECT* o = &c->Object;

			c->TargetX = c->PositionX;
			c->TargetY = c->PositionY;
			c->Movement = false;
			CreateTeleportEnd(0, o);

			c->EnabledInternalAction = INTERNAL_ACTION::IA_NONE;
		}
		break;

	case GS2CL_PROT_VIEWPORT_WEATHER:
		{
			GS2CL_VIEWPORT_WEATHER ViewportWeather;

			if (ViewportWeather.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			if (ViewportWeather.Weather == 0)
				RainTarget = 0;
			else if (ViewportWeather.Weather == 1)
				RainTarget = ViewportWeather.WeatherVariation * 6;
		}
		break;

	case GS2CL_PROT_VIEWPORT_EVENTMASK:
		{
			GS2CL_VIEWPORT_EVENTMASK ViewportEventMask;

			if (ViewportEventMask.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			EventMask = ViewportEventMask.EventMask;
			g_Tasking.CleanBoids();
		}
		break;

	case GS2CL_PROT_VIEWPORT_INVENTORY:
		{
			GS2CL_VIEWPORT_INVENTORY ViewportInventory;

			if (ViewportInventory.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			switch (ViewportInventory.Type)
			{
			case GS2CL_VIEWPORT_INVENTORY::INVENTORY_DATA:
				{
					for (mu_uint32 i = 0; i < GAME::MAX_EQUIPMENT; ++i)
					{
						CharacterMachine->Equipment[i].Type = -1;
						CharacterMachine->Equipment[i].Level = 0;
						CharacterMachine->Equipment[i].ExcOption = 0;
						CharacterMachine->Equipment[i].SetOption = 0;
					}

					g_PlayItemSoundEnable = false;

					g_pMyInventory->UnequipAllItems();
					g_pMyInventory->DeleteAllItems();
					//g_pMyShopInventory->DeleteAllItems();

					for (auto it = ViewportInventory.Items.begin(); it != ViewportInventory.Items.end(); ++it)
					{
						SHARED::ITEM_DATA &Item = *it;

						switch (Item.InventoryType)
						{
						case INVENTORY_TYPE::IT_EQUIPMENT:
							{
								if (Item.InventoryPosX < GAME::MAX_EQUIPMENT)
								{
									g_pMyInventory->EquipItem(Item.InventoryPosX, Item);
								}
							}
							break;

						case INVENTORY_TYPE::IT_INVENTORY:
							{
								if (Item.InventoryPosX < COLUMN_INVENTORY &&
									Item.InventoryPosY < ROW_INVENTORY)
								{
									g_pMyInventory->InsertItem(Item.InventoryPosX, Item.InventoryPosY, Item);
								}
							}
							break;
						}
					}

					g_PlayItemSoundEnable = true;

					g_pSkillFrame->UpdateSkills();
					g_MoveManager.Update();
				}
				break;

			case GS2CL_VIEWPORT_INVENTORY::WAREHOUSE_DATA:
				{
					g_PlayItemSoundEnable = false;

					g_pStorageInventory->DeleteAllItems();

					for (auto it = ViewportInventory.Items.begin(); it != ViewportInventory.Items.end(); ++it)
					{
						SHARED::ITEM_DATA &Item = *it;

						switch (Item.InventoryType)
						{
						case INVENTORY_TYPE::IT_STORAGE:
							{
								if (Item.InventoryPosX < COLUMN_STORAGE &&
									Item.InventoryPosY < ROW_STORAGE)
								{
									g_pStorageInventory->InsertItem(Item.InventoryPosX, Item.InventoryPosY, Item);
								}
							}
							break;
						}
					}

					g_PlayItemSoundEnable = true;
				}
				break;

			case GS2CL_VIEWPORT_INVENTORY::CHAOSMACHINE_DATA:
				{
					g_PlayItemSoundEnable = false;

					g_pMixInventory->DeleteAllItems();

					for (auto it = ViewportInventory.Items.begin(); it != ViewportInventory.Items.end(); ++it)
					{
						SHARED::ITEM_DATA &Item = *it;

						switch (Item.InventoryType)
						{
						case INVENTORY_TYPE::IT_MIX:
							{
								if (Item.InventoryPosX < COLUMN_MIX_INVENTORY &&
									Item.InventoryPosY < ROW_MIX_INVENTORY)
								{
									g_pMixInventory->InsertItem(Item.InventoryPosX, Item.InventoryPosY, Item);
								}
							}
							break;
						}
					}

					g_PlayItemSoundEnable = true;
				}
				break;
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_PARTYLIST:
		{
			GS2CL_VIEWPORT_PARTYLIST ViewportPartyList;

			if (ViewportPartyList.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			for (mu_uint32 n = 0; n < MAX_PARTYS; ++n)
			{
				Party[n].Clear();
			}

			PartyNumber = (mu_int32)ViewportPartyList.Members.size();

			for (mu_int32 n = 0; n < PartyNumber; ++n)
			{
				auto &member = ViewportPartyList.Members[n];

				PARTY_t *p = &Party[n];

				p->Name = member.Name;
				p->Key = member.Key;
				p->Number = member.Number;
				p->Map = member.Map;
				p->x = member.X;
				p->y = member.Y;
				p->currHP = member.Life;
				p->maxHP = member.MaxLife;

				p->index = FindCharacterIndex(member.Key);
				if (p->index == MAX_CHARACTERS_CLIENT) p->index = -2;
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_PARTYINFO:
		{
			GS2CL_VIEWPORT_PARTYINFO ViewportPartyInfo;

			if (ViewportPartyInfo.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			for (auto it = ViewportPartyInfo.Members.begin(); it != ViewportPartyInfo.Members.end(); ++it)
			{
				auto &member = *it;

				PARTY_t *p = &Party[member.Index];

				p->Map = member.Map;
				p->x = member.X;
				p->y = member.Y;
				p->currHP = member.Life;
				p->maxHP = member.MaxLife;
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_TRADEMONEY:
		{
			GS2CL_VIEWPORT_TRADEMONEY ViewportTradeMoney;

			if (ViewportTradeMoney.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			if (ViewportTradeMoney.IsYourMoney == false)
			{
				g_pTrade->SetTradeMoney(ViewportTradeMoney.Money, g_pTrade->GetYourGold(), true);
			}
			else
			{
				g_pTrade->SetTradeMoney(g_pTrade->GetMyGold(), ViewportTradeMoney.Money, true);
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_GUILDINFO:
		{
			GS2CL_VIEWPORT_GUILDINFO ViewportGuildInfo;

			if (ViewportGuildInfo.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = FindCharacterIndex(ViewportGuildInfo.Key);
			if (iIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER *c = &CharactersClient[iIndex];

			c->GuildStatus = ViewportGuildInfo.GuildStatus;
			c->GuildType = ViewportGuildInfo.GuildType;
			c->GuildRelationShip = ViewportGuildInfo.GuildRelationship;

			if (ViewportGuildInfo.Key == HeroKey)
			{
				g_MoveManager.SetCastleOwner(ViewportGuildInfo.CastleOwner);
				if (ViewportGuildInfo.GuildIndex == -1)
				{
					EnableGuildWar = false;
					g_pNewUISystem->Hide(GAMEUI::INTERFACE_GUILDINFO);
				}
			}

			if (ViewportGuildInfo.GuildIndex != -1)
			{
				if (g_GuildCache.ExistsGuildMark(ViewportGuildInfo.GuildIndex) == false)
				{
					g_GameServerObject->SendRequestGuildInfo(ViewportGuildInfo.GuildIndex);
				}

				c->GuildMarkIndex = g_GuildCache.AddGuildMarkIndex(ViewportGuildInfo.GuildIndex);
			}
			else
			{
				if (c->GuildMarkIndex != -1)
				{
					g_GuildCache.ReleaseGuildMarkIndex(c->GuildMarkIndex);
					c->GuildMarkIndex = -1;
				}
			}

			if(c->Object.Flags.Get(OBJECT::eOF_LIVE) == true)
			{
				GuildTeam(c);
			}

#if 0 // FIX
			if (GET_WORLD == WD_30BATTLECASTLE)
			{
				if (g_pSiegeWarfare != nullptr)
				{
					if (g_pSiegeWarfarse->IsCreated() == false)
					{
						g_pSiegeWarfarse->InitMiniMapUI();
						g_pSiegeWarfarse->SetGuildData(Hero);
						g_pSiegeWarfarse->CreateMiniMapUI();
					}
				}
			}
#endif
		}
		break;

	case GS2CL_PROT_VIEWPORT_GUILDLIST:
		{
			GS2CL_VIEWPORT_GUILDLIST ViewportGuildList;

			if (ViewportGuildList.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			g_pGuildWindow->ClearMembers();

			for (mu_uint32 n = 0; n < ViewportGuildList.MembersCount; ++n)
			{
				GS2CL_VIEWPORT_GUILDLIST::MEMBER_DATAEX &member = ViewportGuildList.Members[n];

				GUILD_LIST_t newMember;

				newMember.Name = member.CharacterName;
				newMember.CharacterIndex = member.CharacterIndex;
				newMember.GroupIndex = member.GroupIndex;
				newMember.ServerIndex = member.ServerIndex;
				newMember.GuildStatus = member.GuildStatus;

				g_pGuildWindow->AddMember(newMember);
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_GUILDUPDATE:
		{
			GS2CL_VIEWPORT_GUILDUPDATE ViewportGuildUpdate;

			if (ViewportGuildUpdate.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			for (auto ait = ViewportGuildUpdate.Actions.begin(); ait != ViewportGuildUpdate.Actions.end(); ++ait)
			{
				GS2CL_VIEWPORT_GUILDUPDATE::ACTION_DATAEX &action = *ait;

				switch (action.ActionType)
				{
				case GS2CL_VIEWPORT_GUILDUPDATE::ACTION_DELETE:
					{
						g_pGuildWindow->DeleteMember(action.CharacterIndex);
					}
					break;

				case GS2CL_VIEWPORT_GUILDUPDATE::ACTION_OFFLINE:
					{
						g_pGuildWindow->SetMemberOffline(action.CharacterIndex);
					}
					break;

				case GS2CL_VIEWPORT_GUILDUPDATE::ACTION_UPDATE:
					{
						GUILD_LIST_t newMember;

						newMember.Name = action.CharacterName;
						newMember.CharacterIndex = action.CharacterIndex;
						newMember.GroupIndex = action.GroupIndex;
						newMember.ServerIndex = action.ServerIndex;
						newMember.GuildStatus = action.GuildStatus;

						g_pGuildWindow->AddMember(newMember);
					}
					break;
				}
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_PETUPDATE:
		{
			GS2CL_VIEWPORT_PETUPDATE ViewportPetUpdate;

			if (ViewportPetUpdate.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 iIndex = -1;

			if (ViewportPetUpdate.PetType == GS2CL_VIEWPORT_PETUPDATE::TYPE_DARKHORSE)
			{
				iIndex = GAME::EQUIPMENT_HELPER;
			}
			else if (ViewportPetUpdate.PetType == GS2CL_VIEWPORT_PETUPDATE::TYPE_DARKRAVEN)
			{
				iIndex = GAME::EQUIPMENT_WEAPON_LEFT;
			}
			
			if (iIndex == -1)
				break;

			ITEM *pTargetItemSlot = &CharacterMachine->Equipment[iIndex];

			if (pTargetItemSlot->Type > 0)
			{
				ParseItem(ViewportPetUpdate.ItemData, pTargetItemSlot);
			}

			if (ViewportPetUpdate.PetType == GS2CL_VIEWPORT_PETUPDATE::TYPE_DARKRAVEN &&
				Hero->m_pPet != nullptr)
			{
				CSPetDarkSpirit* pPet = Hero->m_pPet;
				pPet->m_PetCharacter.Object.WeaponLevel = ViewportPetUpdate.ItemData.PetLevel;

				switch (ViewportPetUpdate.UpdateType)
				{
				case GS2CL_VIEWPORT_PETUPDATE::PET_LEVELUP:
					{
						pPet->Eff_LevelUp(0);
					}
					break;

				case GS2CL_VIEWPORT_PETUPDATE::PET_LEVELDOWN:
					{
						pPet->Eff_LevelDown(0);
					}
					break;
				}
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_PETEFFECT:
		{
			GS2CL_VIEWPORT_PETEFFECT ViewportPetEffect;

			if (ViewportPetEffect.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 CharacterIndex = FindCharacterIndex(ViewportPetEffect.Key);
			if (CharacterIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER *c = &CharactersClient[CharacterIndex];

			if (ViewportPetEffect.PetType == GS2CL_VIEWPORT_PETUPDATE::TYPE_DARKRAVEN &&
				c->m_pPet != nullptr)
			{
				CSPetDarkSpirit* pPet = c->m_pPet;
				pPet->m_PetCharacter.Object.WeaponLevel = ViewportPetEffect.Level;

				switch (ViewportPetEffect.UpdateType)
				{
				case GS2CL_VIEWPORT_PETUPDATE::PET_LEVELUP:
					{
						pPet->Eff_LevelUp(0);
					}
					break;

				case GS2CL_VIEWPORT_PETUPDATE::PET_LEVELDOWN:
					{
						pPet->Eff_LevelDown(0);
					}
					break;
				}
			}
		}
		break;

	case GS2CL_PROT_VIEWPORT_PETCOMMAND:
		{
			GS2CL_VIEWPORT_PETCOMMAND ViewportPetCommand;

			if (ViewportPetCommand.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			 // FIX
		}
		break;

	case GS2CL_PROT_VIEWPORT_PETATTACK:
		{
			GS2CL_VIEWPORT_PETATTACK ViewportPetAttack;

			if (ViewportPetAttack.FromBuffer(PacketBuffer, Header->Size) == false)
			{
#ifndef NDEBUG
				mu_printf(_T("Received corrupted packet!\n"));
#endif
				break;
			}

			mu_int32 CharacterIndex = FindCharacterIndex(ViewportPetAttack.Key);
			if (CharacterIndex == MAX_CHARACTERS_CLIENT) break;

			CHARACTER *c = &CharactersClient[CharacterIndex];

			giPetManager::SetAttack(c, ViewportPetAttack.TargetKey, ViewportPetAttack.SkillType);
		}
		break;

	default:
#ifndef NDEBUG
		mu_printf(_T("Received wrong viewport code!\n"));
#endif
		break;
	}
}