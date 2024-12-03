#include "stdafx.h"
#include "mu_petmanager.h"

namespace giPetManager
{
	mu_uint8 g_byTabBar = 0;
	mu_uint32 g_iRenderItemIndexBackup = 0;
	ITEM g_RenderItemInfoBackup;

	const mu_uint8 g_byCommandNum = 4;

	void InitPetManager(void)
	{
		g_byTabBar = 0;
	}

	void CreatePetDarkSpirit(CHARACTER*c)
	{
		giPetManager::DeletePet(c);

		if (InChaosCastle() == true) return;

		c->m_pPet = new CSPetDarkSpirit(c);
		c->m_PetType = CS_PETTYPE_DARKSPIRIT;
	}

	void CreatePetDarkSpirit_Now(CHARACTER* c)
	{
		if (c->Weapon[1].Type == MODEL_HELPER + 5 &&
			c->m_pPet != nullptr)
		{
			mu_uint16 petLevel = c->m_pPet->m_PetCharacter.Object.WeaponLevel;

			giPetManager::DeletePet(c);

			c->m_pPet = new CSPetDarkSpirit(c);
			c->m_pPet->m_PetCharacter.Object.WeaponLevel = petLevel;
			c->m_PetType = CS_PETTYPE_DARKSPIRIT;
		}
	}

	void MovePet(const mu_uint32 ThreadIndex, CHARACTER*c)
	{
		if (c->m_pPet != nullptr)
		{
			switch (c->m_PetType)
			{
			case CS_PETTYPE_BASE:
				{
				}
				break;

			case CS_PETTYPE_DARKSPIRIT:
				{
					c->m_pPet->MovePet(ThreadIndex);
				}
				break;
			}
		}
	}

	void RenderPet(const mu_uint32 ThreadIndex, CHARACTER* c)
	{
		OBJECT* o = &c->Object;
		if (c->m_pPet != nullptr)
		{
			switch (c->m_PetType)
			{
			case CS_PETTYPE_BASE:
				{
					CSPetSystem* pPet = c->m_pPet;

					if (g_isCharacterBuff(o, eBuff_Cloaking))
					{

					}
					else
					{
						ITEM* pEquipmentItemSlot = &CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT];
						pPet->SetPetItem(pEquipmentItemSlot);
					}
				}
				break;

			case CS_PETTYPE_DARKSPIRIT:
				{
					CSPetDarkSpirit* pPet = c->m_pPet;

					if (g_isCharacterBuff(o, eBuff_Cloaking))
					{
						pPet->RenderPet(ThreadIndex, 10);
					}
					else
					{
						pPet->RenderPet(ThreadIndex);
					}
				}
				break;
			}
		}
	}

	mu_boolean SendPetCommand(CHARACTER* c, mu_int32 Index)
	{
#if 0 // FIX
		if (c->m_pPet != nullptr)
		{
			if (MouseRButtonPush || MouseRButton)
			{
				if (Index >= AT_PET_COMMAND_DEFAULT && Index < AT_PET_COMMAND_END)
				{
					if (g_pNewUISystem->AllowHeroAction() == true)
					{
						CSPetDarkSpirit* pPet = c->m_pPet;

						if (Index == AT_PET_COMMAND_TARGET)
						{
							if (CheckAttack())
							{
								CHARACTER* tc = &CharactersClient[SelectedCharacter];
								if (SelectedCharacter != -1 && (tc->Object.Kind == KIND_MONSTER || tc->Object.Kind == KIND_PLAYER))
								{
									SendRequestPetCommand(pPet->GetPetType(), Index - AT_PET_COMMAND_DEFAULT, tc->Key);
								}
							}
						}
						else
						{
							SendRequestPetCommand(pPet->GetPetType(), Index - AT_PET_COMMAND_DEFAULT, 0xffff);
						}
						MouseRButtonPop = false;
						MouseRButtonPush = false;
						MouseRButton = false;

						MouseRButtonPress = 0;
					}
					return true;
				}
			}
		}
#endif

		return false;
	}

	void SetPetCommand(CHARACTER* c, mu_int32 Key, mu_uint8 Cmd)
	{
		if (c->m_pPet != nullptr)
		{
			switch (c->m_PetType)
			{
			case CS_PETTYPE_BASE:
				{
					c->m_pPet->SetCommand(Key, Cmd);
				}
				break;

			case CS_PETTYPE_DARKSPIRIT:
				{
					c->m_pPet->SetCommand(Key, Cmd);
				}
				break;
			}
		}
	}

	void SetAttack(CHARACTER* c, mu_int32 Key, mu_int32 attackType)
	{
		if (c->m_pPet != nullptr)
		{
			switch (c->m_PetType)
			{
			case CS_PETTYPE_BASE:
				{
					c->m_pPet->SetAttack(Key, attackType);
				}
				break;

			case CS_PETTYPE_DARKSPIRIT:
				{
					c->m_pPet->SetAttack(Key, attackType);
				}
				break;
			}
		}
	}

	mu_boolean RenderPetCmdInfo(mu_int32 sx, mu_int32 sy, mu_int32 Type, mu_int32 iRenderPoint)
	{
		if (Type < AT_PET_COMMAND_DEFAULT || Type >= AT_PET_COMMAND_END) return false;

		mu_int32  TextNum = 0;
		mu_int32  SkipNum = 0;

		if (GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD)
		{
			mu_int32 cmdType = Type - AT_PET_COMMAND_DEFAULT;

			TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = true;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1219 + cmdType]); ++TextNum; ++SkipNum;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

			TextListColor[TextNum] = TEXT_COLOR_WHITE;
			switch (cmdType)
			{
			case PET_CMD_DEFAULT: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1223]); ++TextNum; ++SkipNum; break;
			case PET_CMD_RANDOM: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1224]); ++TextNum; ++SkipNum; break;
			case PET_CMD_OWNER: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1225]); ++TextNum; ++SkipNum; break;
			case PET_CMD_TARGET: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1226]); ++TextNum; ++SkipNum; break;
			}

			RenderTipTextList(sx, sy, TextNum, 0, ETA_CENTER, iRenderPoint);
			return true;
		}

		return false;
	}

	void DeletePet(CHARACTER* c)
	{
		if (c->m_pPet != nullptr)
		{
			mu_int32 iObjectType = -1;

			switch (c->m_PetType)
			{
			case CS_PETTYPE_BASE:
				{
					iObjectType = c->m_pPet->GetObjectType();
				}
				break;

			case CS_PETTYPE_DARKSPIRIT:
				{
					iObjectType = c->m_pPet->GetObjectType();
				}
				break;
			}

			if (iObjectType != -1)
			{
				TerminateOwnerEffectObject(iObjectType);
			}

			delete c->m_pPet;
			c->m_pPet = nullptr;
		}
	}

	void InitItemBackup(void)
	{
		mu_zeromem(&g_RenderItemInfoBackup, sizeof(g_RenderItemInfoBackup));
	}

	void SetPetInfo(const mu_uint32 ThreadIndex, mu_uint8 InvType, mu_uint8 InvPos, void* pPetinfo)
	{
#if 0 // FIX
		CalcPetInfo(pPetinfo);

		if ((InvType == 0) || (InvType == 254) || (InvType == 255))
		{
			if ((InvPos == GAME::EQUIPMENT_HELPER) || (InvPos == GAME::EQUIPMENT_WEAPON_LEFT))
			{
				PET_INFO* pHeroPetInfo = Hero->GetEquipedPetInfo(pPetinfo->m_dwPetType);
				memcpy(pHeroPetInfo, pPetinfo, sizeof(PET_INFO));

				if (pPetinfo->m_dwPetType == PET_TYPE_DARK_SPIRIT)
				{
					if (Hero->m_pPet == nullptr)
						return;

					CSPetDarkSpirit* pPet = Hero->m_pPet;
					pPet->SetPetInfo(pHeroPetInfo);

					if (InvType == 254)
					{
						pPet->Eff_LevelUp(ThreadIndex);
					}
					else if (InvType == 255)
					{
						pPet->Eff_LevelDown(ThreadIndex);
					}
				}
				else if (pPetinfo->m_dwPetType == PET_TYPE_DARK_HORSE)
				{
					if (InvType == 254 || InvType == 255)
					{
						Hero->Object.ExtState = InvType - 253;
					}

					SetPetItemConvert(&CharacterMachine->Equipment[GAME::EQUIPMENT_HELPER], pHeroPetInfo);
				}

				CharacterMachine->CalculateAll();
			}
		}

		mu_memcpy(&gs_PetInfo, pPetinfo, sizeof(PET_INFO));
#endif
	}

	mu_int64 GetPetItemValue(ITEM *pItem)
	{
		mu_int64 dwGold = 0;

		switch (pItem->Type)
		{
		case ITEM_HELPER + 4:
			dwGold = pItem->PetLevel * 2000000ll;
			break;

		case ITEM_HELPER + 5:
			dwGold = pItem->PetLevel * 1000000ll;
			break;
		}

		return dwGold;
	}

	mu_boolean RenderPetItemInfo(mu_int32 sx, mu_int32 sy, ITEM* pItem, mu_boolean useAlternativePos, mu_int32 iInvenType)
	{
		if (pItem->Type != ITEM_HELPER + 4 &&
			pItem->Type != ITEM_HELPER + 5)
			return false;

		mu_int32 TextNum = 0;
		mu_int32 SkipNum = 0;
		mu_int32 RequireLevel = 0;
		mu_int32 RequireCharisma = 0;

		if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_NPCSHOP))
		{
			mu_unicode Text[128];
			mu_int64 Gold = (GetPetItemValue(pItem) / 3);
			Gold = Gold / 100 * 100;

			ConvertGold(Gold, Text, mu_countof(Text));
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[63], Text);

			TextListColor[TextNum] = TEXT_COLOR_WHITE;
			TextBold[TextNum] = true;
			++TextNum;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;
		}
		else if ((iInvenType == TOOLTIP_TYPE_MY_SHOP) || (iInvenType == TOOLTIP_TYPE_PURCHASE_SHOP))
		{
			mu_int32 price = 0;
			mu_unicode Text[128];

#if 0 // FIX
			mu_int32 indexInv = (GAME::MAX_EQUIPMENT + MAX_INVENTORY) + (pItem->y*COL_PERSONALSHOP_INVEN) + pItem->x;
			if (GetPersonalItemPrice(indexInv, price, g_IsPurchaseShop))
			{
				ConvertGold(price, Text, mu_countof(Text));
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[63], Text);

				if (price >= 10000000)
					TextListColor[TextNum] = TEXT_COLOR_RED;
				else if (price >= 1000000)
					TextListColor[TextNum] = TEXT_COLOR_YELLOW;
				else if (price >= 100000)
					TextListColor[TextNum] = TEXT_COLOR_GREEN;
				else
					TextListColor[TextNum] = TEXT_COLOR_WHITE;
				TextBold[TextNum] = true;
				++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

				mu_int64 gold = CharacterMachine->Gold;
				if ((gold < price) && (g_IsPurchaseShop == PSHOPWNDTYPE_PURCHASE))
				{
					TextListColor[TextNum] = TEXT_COLOR_RED;
					TextBold[TextNum] = true;
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[423]);
					++TextNum;
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;
				}
			}
			else if (g_IsPurchaseShop == PSHOPWNDTYPE_SALE)
			{
				TextListColor[TextNum] = TEXT_COLOR_RED;
				TextBold[TextNum] = true;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1101]);
				++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;
			}
#endif
		}

		TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = true;
		if (pItem->Type == ITEM_HELPER + 4)
		{
			RequireLevel = (218 + (pItem->PetLevel * 2));
			RequireCharisma = 0;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1187]); ++TextNum; ++SkipNum;
		}
		else if (pItem->Type == ITEM_HELPER + 5)
		{
			RequireCharisma = (185 + (pItem->PetLevel * 15));

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1214]); ++TextNum; ++SkipNum;
		}

		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[201], pItem->PetExperience, pItem->NextPetExperience); ++TextNum; ++SkipNum;

		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" : %d"), GlobalText[368], pItem->PetLevel); ++TextNum; ++SkipNum;

		if (pItem->Type == ITEM_HELPER + 5)
		{
			mu_uint32 Charisma = CharacterAttribute->Charisma + CharacterAttribute->AddCharisma;
			TextListColor[TextNum] = TEXT_COLOR_WHITE;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[203], pItem->DamageMin + (Charisma / 8), pItem->DamageMax + (Charisma / 4), pItem->AttackSuccess); ++TextNum; ++SkipNum;
			TextListColor[TextNum] = TEXT_COLOR_WHITE;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[64], pItem->WeaponSpeed + (Charisma / 50)); ++TextNum; ++SkipNum;
		}

		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[70], pItem->Durability); ++TextNum; ++SkipNum;

		if (pItem->Type == ITEM_HELPER + 4)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[76], RequireLevel);

			if (CharacterAttribute->Level < RequireLevel)
			{
				TextListColor[TextNum] = TEXT_COLOR_RED;
				TextBold[TextNum] = false;
				++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[74], RequireLevel - CharacterAttribute->Level);
				TextListColor[TextNum] = TEXT_COLOR_RED;
				TextBold[TextNum] = false;
				++TextNum;
			}
			else
			{
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
				TextBold[TextNum] = false;
				++TextNum;
			}
		}
		else if (pItem->Type == ITEM_HELPER + 5)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[698], RequireCharisma);

			mu_uint16 Charisma;
			Charisma = CharacterAttribute->Charisma + CharacterAttribute->AddCharisma;

			if (Charisma < RequireCharisma)
			{
				TextListColor[TextNum] = TEXT_COLOR_RED;
				TextBold[TextNum] = false;
				++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[74], RequireCharisma - Charisma);
				TextListColor[TextNum] = TEXT_COLOR_RED;
				TextBold[TextNum] = false;
				++TextNum;
			}
			else
			{
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
				TextBold[TextNum] = false;
				++TextNum;
			}
		}

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;
		if (GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD)
			TextListColor[TextNum] = TEXT_COLOR_WHITE;
		else
			TextListColor[TextNum] = TEXT_COLOR_DARKRED;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[24]); ++TextNum; ++SkipNum;

		for (mu_int32 i = 0; i < pItem->SpecialNum; ++i)
		{
			mu_int32 SpecialValue = pItem->SpecialValue[i];

			if (pItem->Type == ITEM_HELPER + 4 &&
				pItem->Special[i] == AT_SET_OPTION_IMPROVE_DEFENCE)
			{
				SpecialValue += CharacterAttribute->Dexterity / 20;
			}

			GetSpecialOptionText(pItem->Type, TextList[TextNum], TEXTLIST_LENGTH, pItem->Special[i], SpecialValue, 0);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false; ++TextNum; ++SkipNum;
		}

		if (pItem->Type == ITEM_HELPER + 4)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[744], (30 + pItem->PetLevel) / 2);
			TextListColor[TextNum] = TEXT_COLOR_BLUE; ++TextNum; ++SkipNum;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1188], 2);
			TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;
		}

		mu_boolean isrendertooltip = true;

		isrendertooltip = g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_PARTCHARGE_SHOP) ? false : true;

		if (isrendertooltip)
		{
			if (useAlternativePos == false)
				RenderTipTextList(sx, sy, TextNum, 0, ETA_CENTER, STRP_ITEMINFO);
			else
				RenderTipTextList(sx, sy, TextNum, 0, ETA_CENTER, STRP_ITEMINFO2);
		}

		return true;
	}
};