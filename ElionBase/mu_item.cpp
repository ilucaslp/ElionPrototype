#include "stdafx.h"
#include "mu_item.h"

std::array<mu_int32, 100> RandomTable;
std::array<ITEM_t, MAX_ITEMS> Items;
std::array<mu_uint16, MAX_ITEMS + MAX_TASKING_THREADS> g_ItemsQueue;

void ItemConvert(ITEM *ip, SHARED::ITEM_DATA &Item)
{
	ip->Level = Item.Level;
	mu_int32     Level = Item.Level;
	mu_int32     excel = Item.ExcOption;
	mu_int32     excelWing = excel;
	mu_int32     excelAddValue = 0;
	mu_boolean    bExtOption = false;

	if ((ip->Type >= ITEM_WING + 3 && ip->Type <= ITEM_WING + 6)
		|| (ip->Type >= ITEM_WING + 36 && ip->Type <= ITEM_WING + 40)
		|| (ip->Type >= ITEM_WING + 42 && ip->Type <= ITEM_WING + 43)
		|| ip->Type == ITEM_SWORD + 19
		|| ip->Type == ITEM_BOW + 18
		|| ip->Type == ITEM_STAFF + 10
		|| ip->Type == ITEM_MACE + 13
		|| ip->Type == ITEM_HELPER + 30
		|| (ITEM_WING + 130 <= ip->Type && ip->Type <= ITEM_WING + 134)
		)
	{
		excel = 0;
	}

	if ((Item.SetOption % 0x4) == EXT_A_SET_OPTION || (Item.SetOption % 0x4) == EXT_B_SET_OPTION)
	{
		excel = 1;
		bExtOption = true;
	}

	ITEM_ATTRIBUTE *p = &ItemAttribute[ip->Type];
	ip->WeaponSpeed = p->WeaponSpeed;
	ip->DamageMin = p->DamageMin;
	ip->DamageMax = p->DamageMax;
	ip->SuccessfulBlocking = p->SuccessfulBlocking;
	ip->Defense = p->Defense;
	ip->MagicDefense = p->MagicDefense;
	ip->WalkSpeed = p->WalkSpeed;
	ip->MagicPower = p->MagicPower;

	mu_int32 setItemDropLevel = p->Level + 30;

	if (ip->Type == ITEM_MACE + 6)
	{
		excelAddValue = 15;
	}
	else if (ip->Type == ITEM_BOW + 6)
	{
		excelAddValue = 30;
	}
	else if (ip->Type == ITEM_STAFF + 7)
	{
		excelAddValue = 25;
	}

	if (p->DamageMin > 0)
	{
		if (excel > 0)
		{
			if (p->Level)
			{
				if (excelAddValue)
					ip->DamageMin += excelAddValue;
				else
					ip->DamageMin += p->DamageMin * 25 / p->Level + 5;
			}
		}
		if (bExtOption)
		{
			ip->DamageMin += 5 + (setItemDropLevel / 40);
		}
		ip->DamageMin += (EMath::Min(9, Level) * 3);
		switch (Level - 9)
		{
		case 6: ip->DamageMin += 9;
		case 5: ip->DamageMin += 8;
		case 4: ip->DamageMin += 7;
		case 3: ip->DamageMin += 6;
		case 2: ip->DamageMin += 5;
		case 1: ip->DamageMin += 4;
		default: break;
		};
	}
	if (p->DamageMax > 0)
	{
		if (excel > 0)
		{
			if (p->Level)
			{
				if (excelAddValue)
					ip->DamageMax += excelAddValue;
				else
					ip->DamageMax += p->DamageMin * 25 / p->Level + 5;
			}
		}
		if (bExtOption)
		{
			ip->DamageMax += 5 + (setItemDropLevel / 40);
		}
		ip->DamageMax += (EMath::Min(9, Level) * 3);
		switch (Level - 9)
		{
		case 6: ip->DamageMax += 9;
		case 5: ip->DamageMax += 8;
		case 4: ip->DamageMax += 7;
		case 3: ip->DamageMax += 6;
		case 2: ip->DamageMax += 5;
		case 1: ip->DamageMax += 4;
		default: break;
		};
	}
	if (p->MagicPower > 0)
	{
		if (excel > 0)
		{
			if (p->Level)
			{
				if (excelAddValue)
					ip->MagicPower += excelAddValue;
				else
					ip->MagicPower += p->MagicPower * 25 / p->Level + 5;
			}
		}
		if (bExtOption)
		{
			ip->MagicPower += 2 + (setItemDropLevel / 60);
		}
		ip->MagicPower += (EMath::Min(9, Level) * 3);
		switch (Level - 9)
		{
		case 6: ip->MagicPower += 9;
		case 5: ip->MagicPower += 8;
		case 4: ip->MagicPower += 7;
		case 3: ip->MagicPower += 6;
		case 2: ip->MagicPower += 5;
		case 1: ip->MagicPower += 4;
		default: break;
		};

		ip->MagicPower /= 2;

		if (IsCepterItem(ip->Type) == false)
		{
			ip->MagicPower += Level * 2;
		}
	}

	if (p->SuccessfulBlocking > 0)
	{
		if (excel > 0)
		{
			if (p->Level)
				ip->SuccessfulBlocking += p->SuccessfulBlocking * 25 / p->Level + 5;
		}
		ip->SuccessfulBlocking += (EMath::Min(9, Level) * 3);
		switch (Level - 9)
		{
		case 6: ip->SuccessfulBlocking += 9;
		case 5: ip->SuccessfulBlocking += 8;
		case 4: ip->SuccessfulBlocking += 7;
		case 3: ip->SuccessfulBlocking += 6;
		case 2: ip->SuccessfulBlocking += 5;
		case 1: ip->SuccessfulBlocking += 4;
		default: break;
		};
	}

	if (p->Defense > 0)
	{
		if (ip->Type >= ITEM_SHIELD && ip->Type < ITEM_SHIELD + MAX_ITEM_INDEX)
		{
			ip->Defense += Level;
			if (bExtOption)
			{
				ip->Defense = ip->Defense + (ip->Defense * 20 / setItemDropLevel + 2);
			}
		}
		else
		{
			if (excel > 0)
			{
				if (p->Level)
					ip->Defense += p->Defense * 12 / p->Level + 4 + p->Level / 5;
			}
			if (bExtOption)
			{
				ip->Defense = ip->Defense + (ip->Defense * 3 / setItemDropLevel + 2 + setItemDropLevel / 30);
			}

			if ((ip->Type >= ITEM_WING + 3 && ip->Type <= ITEM_WING + 6) || ip->Type == ITEM_WING + 42)
			{
				ip->Defense += (EMath::Min(9, Level) * 2);
			}
			else if (ip->Type == ITEM_HELPER + 30)
			{
				ip->Defense += (EMath::Min(9, Level) * 2);
			}
			else if ((ip->Type >= ITEM_WING + 36 && ip->Type <= ITEM_WING + 40) || ip->Type == ITEM_WING + 43)
			{
				ip->Defense += (EMath::Min(9, Level) * 4);
			}
			else
			{
				ip->Defense += (EMath::Min(9, Level) * 3);
			}
			if ((ip->Type >= ITEM_WING + 36 && ip->Type <= ITEM_WING + 40) || ip->Type == ITEM_WING + 43)
			{
				switch (Level - 9)
				{
				case 6: ip->Defense += 10;
				case 5: ip->Defense += 9;
				case 4: ip->Defense += 8;
				case 3: ip->Defense += 7;
				case 2: ip->Defense += 6;
				case 1: ip->Defense += 5;
				default: break;
				};
			}
			else
			{
				switch (Level - 9)
				{
				case 6: ip->Defense += 9;
				case 5: ip->Defense += 8;
				case 4: ip->Defense += 7;
				case 3: ip->Defense += 6;
				case 2: ip->Defense += 5;
				case 1: ip->Defense += 4;
				default: break;
				};
			}
		}
	}
	if (p->MagicDefense > 0)
	{
		ip->MagicDefense += (EMath::Min(9, Level) * 3);
		switch (Level - 9)
		{
		case 6: ip->MagicDefense += 9;
		case 5: ip->MagicDefense += 8;
		case 4: ip->MagicDefense += 7;
		case 3: ip->MagicDefense += 6;
		case 2: ip->MagicDefense += 5;
		case 1: ip->MagicDefense += 4;
		default: break;
		};
	}

	mu_int32 ItemLevel = p->Level;

	if (excel)	ItemLevel = p->Level + 25;
	else if (bExtOption) ItemLevel = p->Level + 30;

	mu_int32 addValue = 4;

	if ((ip->Type >= ITEM_WING + 3 && ip->Type <= ITEM_WING + 6) || ip->Type == ITEM_WING + 42)
	{
		addValue = 5;
	}

	if (p->RequireLevel && ((ip->Type >= ITEM_SWORD && ip->Type < ITEM_WING)
		|| (ip->Type == ITEM_HELPER + 37)
		|| (ip->Type >= ITEM_WING + 7 && ip->Type <= ITEM_WING + 40)
		|| (ip->Type >= ITEM_WING + 43 && ip->Type < ITEM_HELPER)
		))
		ip->RequireLevel = p->RequireLevel;
	else if (p->RequireLevel && ((ip->Type >= ITEM_WING && ip->Type <= ITEM_WING + 7)
		|| (ip->Type >= ITEM_WING + 41 && ip->Type <= ITEM_WING + 42)
		|| ip->Type >= ITEM_HELPER))
		ip->RequireLevel = p->RequireLevel + Level*addValue;
	else
		ip->RequireLevel = 0;

	if (p->RequireStrength)
		ip->RequireStrength = 20 + (p->RequireStrength)*(ItemLevel + Level * 3) * 3 / 100;
	else	ip->RequireStrength = 0;

	if (p->RequireDexterity)
		ip->RequireDexterity = 20 + (p->RequireDexterity)*(ItemLevel + Level * 3) * 3 / 100;
	else	ip->RequireDexterity = 0;

	if (p->RequireVitality)
		ip->RequireVitality = 20 + (p->RequireVitality)*(ItemLevel + Level * 3) * 3 / 100;
	else	ip->RequireVitality = 0;

	if (p->RequireEnergy)
	{
		if (ip->Type >= ITEM_STAFF + 21 && ip->Type <= ITEM_STAFF + 29)
		{
			ip->RequireEnergy = 20 + (p->RequireEnergy)*(ItemLevel + Level * 1) * 3 / 100;
		}
		else if ((p->RequireLevel > 0) && (ip->Type >= ITEM_ETC && ip->Type < ITEM_ETC + MAX_ITEM_INDEX))
		{
			ip->RequireEnergy = 20 + (p->RequireEnergy)*(p->RequireLevel) * 4 / 100;
		}
		else
		{
			ip->RequireEnergy = 20 + (p->RequireEnergy)*(ItemLevel + Level * 3) * 4 / 100;
		}
	}
	else
	{
		ip->RequireEnergy = 0;
	}

	if (p->RequireCharisma)
		ip->RequireCharisma = 20 + (p->RequireCharisma)*(ItemLevel + Level * 3) * 3 / 100;
	else	ip->RequireCharisma = 0;

	if (ip->Type == ITEM_WING + 11)
	{
		mu_uint16 Energy = 0;

		switch (Level)
		{
		case 0:Energy = 30; break;
		case 1:Energy = 60; break;
		case 2:Energy = 90; break;
		case 3:Energy = 130; break;
		case 4:Energy = 170; break;
		case 5:Energy = 210; break;
		case 6:Energy = 300; break;
		}
		ip->RequireEnergy = Energy;
	}

	if (p->RequireCharisma)
	{
		if (ip->Type == MODEL_HELPER + 5)
			ip->RequireCharisma = (185 + (p->RequireCharisma * 15));
		else
			ip->RequireCharisma = p->RequireCharisma;
	}

	if (ip->Type == ITEM_HELPER + 10)
	{
		if (Level <= 2)
			ip->RequireLevel = 20;
		else
			ip->RequireLevel = 50;
	}

	if ((ip->Type >= ITEM_HELM + 29 && ip->Type <= ITEM_HELM + 33) ||
		(ip->Type >= ITEM_ARMOR + 29 && ip->Type <= ITEM_ARMOR + 33) ||
		(ip->Type >= ITEM_PANTS + 29 && ip->Type <= ITEM_PANTS + 33) ||
		(ip->Type >= ITEM_GLOVES + 29 && ip->Type <= ITEM_GLOVES + 33) ||
		(ip->Type >= ITEM_BOOTS + 29 && ip->Type <= ITEM_BOOTS + 33) ||
		ip->Type == ITEM_SWORD + 22 ||
		ip->Type == ITEM_SWORD + 23 ||
		ip->Type == ITEM_STAFF + 12 ||
		ip->Type == ITEM_BOW + 21 ||
		ip->Type == ITEM_MACE + 14
		|| ITEM_STAFF + 19 == ip->Type
		|| ITEM_HELM + 43 == ip->Type
		|| ITEM_ARMOR + 43 == ip->Type
		|| ITEM_PANTS + 43 == ip->Type
		|| ITEM_GLOVES + 43 == ip->Type
		|| ITEM_BOOTS + 43 == ip->Type)
	{
		excel = 0;
	}

	if (excel > 0)
	{
		if (ip->RequireLevel > 0 && ip->Type != ITEM_HELPER + 37)
			ip->RequireLevel += 20;
	}

	ip->SpecialNum = 0;

	if ((ip->Type >= ITEM_WING + 3 && ip->Type <= ITEM_WING + 6) || ip->Type == ITEM_WING + 42)
	{
		if (excelWing & 0x01)
		{
			ip->SpecialValue[ip->SpecialNum] = 50 + Level * 5;
			ip->Special[ip->SpecialNum] = AT_IMPROVE_HP_MAX; ++ip->SpecialNum;
		}
		if ((excelWing >> 1) & 0x01)
		{
			ip->SpecialValue[ip->SpecialNum] = 50 + Level * 5;
			ip->Special[ip->SpecialNum] = AT_IMPROVE_MP_MAX; ++ip->SpecialNum;
		}
		if ((excelWing >> 2) & 0x01)
		{
			ip->SpecialValue[ip->SpecialNum] = 3;
			ip->Special[ip->SpecialNum] = AT_ONE_PERCENT_DAMAGE; ++ip->SpecialNum;
		}
		if ((excelWing >> 3) & 0x01)
		{
			ip->SpecialValue[ip->SpecialNum] = 50;
			ip->Special[ip->SpecialNum] = AT_IMPROVE_AG_MAX; ++ip->SpecialNum;
		}
		if ((excelWing >> 4) & 0x01)
		{
			ip->SpecialValue[ip->SpecialNum] = 5;
			ip->Special[ip->SpecialNum] = AT_IMPROVE_ATTACK_SPEED; ++ip->SpecialNum;
		}
	}
	else if (ip->Type == ITEM_HELPER + 30)
	{
		mu_int32 Cal = 0;
		if (Level <= 9)
			Cal = Level;
		else
			Cal = 9;
		ip->SpecialValue[ip->SpecialNum] = 15 + Cal * 2;
		switch (Level - 9)
		{
		case 6: ip->SpecialValue[ip->SpecialNum] += 9;
		case 5: ip->SpecialValue[ip->SpecialNum] += 8;
		case 4: ip->SpecialValue[ip->SpecialNum] += 7;
		case 3: ip->SpecialValue[ip->SpecialNum] += 6;
		case 2: ip->SpecialValue[ip->SpecialNum] += 5;
		case 1: ip->SpecialValue[ip->SpecialNum] += 4;
		default: break;
		};

		ip->Special[ip->SpecialNum] = AT_SET_OPTION_IMPROVE_DEFENCE; ++ip->SpecialNum;

		ip->SpecialValue[ip->SpecialNum] = 20 + Level * 2;
		ip->Special[ip->SpecialNum] = AT_SET_OPTION_IMPROVE_DAMAGE; ++ip->SpecialNum;

		if (excelWing & 0x01)
		{
			ip->SpecialValue[ip->SpecialNum] = 50 + Level * 5;
			ip->Special[ip->SpecialNum] = AT_IMPROVE_HP_MAX; ++ip->SpecialNum;
		}

		if ((excelWing >> 1) & 0x01)
		{
			ip->SpecialValue[ip->SpecialNum] = 50 + Level * 5;
			ip->Special[ip->SpecialNum] = AT_IMPROVE_MP_MAX; ++ip->SpecialNum;
		}

		if ((excelWing >> 2) & 0x01) 
		{
			ip->SpecialValue[ip->SpecialNum] = 3;
			ip->Special[ip->SpecialNum] = AT_ONE_PERCENT_DAMAGE; ++ip->SpecialNum;
		}

		if ((excelWing >> 3) & 0x01)
		{
			ip->SpecialValue[ip->SpecialNum] = 10 + Level * 5;
			ip->Special[ip->SpecialNum] = AT_SET_OPTION_IMPROVE_CHARISMA; ++ip->SpecialNum;
		}
	}
	else if ((ip->Type >= ITEM_WING + 36 && ip->Type <= ITEM_WING + 40) || ip->Type == ITEM_WING + 43)
	{
		if (excelWing & 0x01)
		{
			ip->SpecialValue[ip->SpecialNum] = 5;
			ip->Special[ip->SpecialNum] = AT_ONE_PERCENT_DAMAGE; ++ip->SpecialNum;
		}
		if ((excelWing >> 1) & 0x01)
		{
			ip->SpecialValue[ip->SpecialNum] = 5;
			ip->Special[ip->SpecialNum] = AT_DAMAGE_REFLECTION; ++ip->SpecialNum;
		}
		if ((excelWing >> 2) & 0x01)
		{
			ip->SpecialValue[ip->SpecialNum] = 5;
			ip->Special[ip->SpecialNum] = AT_RECOVER_FULL_LIFE; ++ip->SpecialNum;
		}
		if ((excelWing >> 3) & 0x01)
		{
			ip->SpecialValue[ip->SpecialNum] = 5;
			ip->Special[ip->SpecialNum] = AT_RECOVER_FULL_MANA; ++ip->SpecialNum;
		}
	}

	if (Item.Skill)
	{
		if (p->m_bySkillIndex != 0)
		{
			ip->Special[ip->SpecialNum] = p->m_bySkillIndex; ++ip->SpecialNum;
		}
	}

	if (Item.Luck)
	{
		if (ip->Type >= ITEM_SWORD && ip->Type < ITEM_BOOTS + MAX_ITEM_INDEX)
		{
			if (ip->Type != ITEM_BOW + 7 && ip->Type != ITEM_BOW + 15)
			{
				ip->Special[ip->SpecialNum] = AT_LUCK; ++ip->SpecialNum;
			}
		}

		if ((ip->Type >= ITEM_WING && ip->Type <= ITEM_WING + 6) || (ip->Type >= ITEM_WING + 41 && ip->Type <= ITEM_WING + 42))
		{
			ip->Special[ip->SpecialNum] = AT_LUCK; ++ip->SpecialNum;
		}
		if (ip->Type == ITEM_HELPER + 30)
		{
			ip->Special[ip->SpecialNum] = AT_LUCK; ++ip->SpecialNum;
		}
		if ((ip->Type >= ITEM_WING + 36 && ip->Type <= ITEM_WING + 40) || ip->Type == ITEM_WING + 43)
		{
			ip->Special[ip->SpecialNum] = AT_LUCK; ++ip->SpecialNum;
		}
	}

	mu_int32 Option3 = Item.Option;
	if (Option3)
	{
		if (ip->Type == ITEM_HELPER + 3)
		{
			if (Option3 & 0x01)
			{
				ip->SpecialValue[ip->SpecialNum] = 5;
				ip->Special[ip->SpecialNum] = AT_DAMAGE_ABSORB; ++ip->SpecialNum;
			}
			if ((Option3 >> 1) & 0x01)
			{
				ip->SpecialValue[ip->SpecialNum] = 50;
				ip->Special[ip->SpecialNum] = AT_IMPROVE_AG_MAX; ++ip->SpecialNum;
			}
			if ((Option3 >> 2) & 0x01)
			{
				ip->SpecialValue[ip->SpecialNum] = 5;
				ip->Special[ip->SpecialNum] = AT_IMPROVE_ATTACK_SPEED; ++ip->SpecialNum;
			}
		}
		else
		{
			if (ip->Type >= ITEM_SWORD && ip->Type < ITEM_BOW + MAX_ITEM_INDEX)
			{
				if (ip->Type != ITEM_BOW + 7 && ip->Type != ITEM_BOW + 15)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_DAMAGE; ++ip->SpecialNum;
					ip->RequireStrength += Option3 * 5;
				}
			}
			if (ip->Type >= ITEM_STAFF && ip->Type < ITEM_STAFF + MAX_ITEM_INDEX)
			{
				ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
				if (ip->Type >= ITEM_STAFF + 21 && ip->Type <= ITEM_STAFF + 29)
					ip->Special[ip->SpecialNum] = AT_IMPROVE_CURSE;
				else
					ip->Special[ip->SpecialNum] = AT_IMPROVE_MAGIC;
				++ip->SpecialNum;
				ip->RequireStrength += Option3 * 5;
			}
			if (ip->Type >= ITEM_SHIELD && ip->Type < ITEM_SHIELD + MAX_ITEM_INDEX)
			{
				ip->SpecialValue[ip->SpecialNum] = Option3 * 5;
				ip->Special[ip->SpecialNum] = AT_IMPROVE_BLOCKING; ++ip->SpecialNum;
				ip->RequireStrength += Option3 * 5;
			}
			if (ip->Type >= ITEM_HELM && ip->Type < ITEM_BOOTS + MAX_ITEM_INDEX)
			{
				ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
				ip->Special[ip->SpecialNum] = AT_IMPROVE_DEFENSE; ++ip->SpecialNum;
				ip->RequireStrength += Option3 * 5;
			}
			if (ip->Type >= ITEM_HELPER + 8 && ip->Type < ITEM_HELPER + MAX_ITEM_INDEX && ip->Type != ITEM_HELPER + 30)
			{
				if (ip->Type == ITEM_HELPER + 24)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_MAX_MANA; ++ip->SpecialNum;
				}
				else if (ip->Type == ITEM_HELPER + 28)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_MAX_AG; ++ip->SpecialNum;
				}
				else
				{
					ip->SpecialValue[ip->SpecialNum] = Option3;
					ip->Special[ip->SpecialNum] = AT_LIFE_REGENERATION; ++ip->SpecialNum;
				}
			}
			if (ip->Type == ITEM_WING)
			{
				ip->SpecialValue[ip->SpecialNum] = Option3;
				ip->Special[ip->SpecialNum] = AT_LIFE_REGENERATION; ++ip->SpecialNum;
			}
			else if (ip->Type == ITEM_WING + 1 || ip->Type == ITEM_WING + 41)
			{
				ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
				ip->Special[ip->SpecialNum] = AT_IMPROVE_MAGIC; ++ip->SpecialNum;
			}
			else if (ip->Type == ITEM_WING + 2)
			{
				ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
				ip->Special[ip->SpecialNum] = AT_IMPROVE_DAMAGE; ++ip->SpecialNum;
			}
			else if (ip->Type == ITEM_WING + 3)
			{
				if ((excelWing >> 5) & 0x01)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3;
					ip->Special[ip->SpecialNum] = AT_LIFE_REGENERATION; ++ip->SpecialNum;
				}
				else
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_DAMAGE; ++ip->SpecialNum;
				}
			}
			else if (ip->Type == ITEM_WING + 4)
			{
				if ((excelWing >> 5) & 0x01)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_MAGIC; ++ip->SpecialNum;
				}
				else
				{
					ip->SpecialValue[ip->SpecialNum] = Option3;
					ip->Special[ip->SpecialNum] = AT_LIFE_REGENERATION; ++ip->SpecialNum;
				}
			}
			else if (ip->Type == ITEM_WING + 5)
			{
				if ((excelWing >> 5) & 0x01)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_DAMAGE; ++ip->SpecialNum;
				}
				else
				{
					ip->SpecialValue[ip->SpecialNum] = Option3;
					ip->Special[ip->SpecialNum] = AT_LIFE_REGENERATION; ++ip->SpecialNum;
				}
			}
			else if (ip->Type == ITEM_WING + 6)
			{
				if ((excelWing >> 5) & 0x01)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_DAMAGE; ++ip->SpecialNum;
				}
				else
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_MAGIC; ++ip->SpecialNum;
				}
			}
			else if (ip->Type == ITEM_HELPER + 30)
			{
				ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
				ip->Special[ip->SpecialNum] = AT_IMPROVE_DAMAGE; ++ip->SpecialNum;
			}
			else if (ip->Type == ITEM_WING + 42)
			{
				ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
				if ((excelWing >> 5) & 0x01)
					ip->Special[ip->SpecialNum] = AT_IMPROVE_MAGIC;
				else
					ip->Special[ip->SpecialNum] = AT_IMPROVE_CURSE;
				++ip->SpecialNum;
			}
			else if (ip->Type == ITEM_WING + 36)
			{
				if ((excelWing >> 4) & 0x01)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_DAMAGE; ++ip->SpecialNum;
				}
				else if ((excelWing >> 5) & 0x01)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_DEFENSE; ++ip->SpecialNum;
				}
				else
				{
					ip->SpecialValue[ip->SpecialNum] = Option3;
					ip->Special[ip->SpecialNum] = AT_LIFE_REGENERATION; ++ip->SpecialNum;
				}
			}
			else if (ip->Type == ITEM_WING + 37)
			{
				if ((excelWing >> 4) & 0x01)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_MAGIC; ++ip->SpecialNum;
				}
				else if ((excelWing >> 5) & 0x01)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_DEFENSE; ++ip->SpecialNum;
				}
				else
				{
					ip->SpecialValue[ip->SpecialNum] = Option3;
					ip->Special[ip->SpecialNum] = AT_LIFE_REGENERATION; ++ip->SpecialNum;
				}
			}
			else if (ip->Type == ITEM_WING + 38)
			{
				if ((excelWing >> 4) & 0x01)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_DAMAGE; ++ip->SpecialNum;
				}
				else if ((excelWing >> 5) & 0x01)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_DEFENSE; ++ip->SpecialNum;
				}
				else
				{
					ip->SpecialValue[ip->SpecialNum] = Option3;
					ip->Special[ip->SpecialNum] = AT_LIFE_REGENERATION; ++ip->SpecialNum;
				}
			}
			else if (ip->Type == ITEM_WING + 39)
			{
				if ((excelWing >> 4) & 0x01)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_DAMAGE; ++ip->SpecialNum;
				}
				else if ((excelWing >> 5) & 0x01)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_MAGIC; ++ip->SpecialNum;
				}
				else
				{
					ip->SpecialValue[ip->SpecialNum] = Option3;
					ip->Special[ip->SpecialNum] = AT_LIFE_REGENERATION; ++ip->SpecialNum;
				}
			}
			else if (ip->Type == ITEM_WING + 40)
			{
				if ((excelWing >> 4) & 0x01)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_DAMAGE; ++ip->SpecialNum;
				}
				else if ((excelWing >> 5) & 0x01)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_DEFENSE; ++ip->SpecialNum;
				}
				else
				{
					ip->SpecialValue[ip->SpecialNum] = Option3;
					ip->Special[ip->SpecialNum] = AT_LIFE_REGENERATION; ++ip->SpecialNum;
				}
			}
			else if (ip->Type == ITEM_WING + 43)
			{
				if ((excelWing >> 4) & 0x01)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_MAGIC; ++ip->SpecialNum;
				}
				else if ((excelWing >> 5) & 0x01)
				{
					ip->SpecialValue[ip->SpecialNum] = Option3 * 4;
					ip->Special[ip->SpecialNum] = AT_IMPROVE_CURSE; ++ip->SpecialNum;
				}
				else
				{
					ip->SpecialValue[ip->SpecialNum] = Option3;
					ip->Special[ip->SpecialNum] = AT_LIFE_REGENERATION; ++ip->SpecialNum;
				}
			}
		}
	}

	if ((ip->Type >= ITEM_SHIELD && ip->Type < ITEM_BOOTS + MAX_ITEM_INDEX) ||
		(ip->Type >= ITEM_HELPER + 8 && ip->Type <= ITEM_HELPER + 9)
		|| (ip->Type >= ITEM_HELPER + 21 && ip->Type <= ITEM_HELPER + 24))
	{
		if ((Item.ExcOption >> 5) & 1)
		{
			ip->Special[ip->SpecialNum] = AT_IMPROVE_LIFE; ++ip->SpecialNum;
		}
		if ((Item.ExcOption >> 4) & 1)
		{
			ip->Special[ip->SpecialNum] = AT_IMPROVE_MANA; ++ip->SpecialNum;
		}
		if ((Item.ExcOption >> 3) & 1)
		{
			ip->Special[ip->SpecialNum] = AT_DECREASE_DAMAGE; ++ip->SpecialNum;
		}
		if ((Item.ExcOption >> 2) & 1)
		{
			ip->Special[ip->SpecialNum] = AT_REFLECTION_DAMAGE; ++ip->SpecialNum;
		}
		if ((Item.ExcOption >> 1) & 1)
		{
			ip->Special[ip->SpecialNum] = AT_IMPROVE_BLOCKING_PERCENT; ++ip->SpecialNum;
		}
		if ((Item.ExcOption) & 1)
		{
			ip->Special[ip->SpecialNum] = AT_IMPROVE_GAIN_GOLD; ++ip->SpecialNum;
		}
	}

	if ((ip->Type >= ITEM_SWORD && ip->Type < ITEM_STAFF + MAX_ITEM_INDEX) ||
		(ip->Type >= ITEM_HELPER + 12 && ip->Type <= ITEM_HELPER + 13)
		|| (ip->Type >= ITEM_HELPER + 25 && ip->Type <= ITEM_HELPER + 28))
	{
		if ((Item.ExcOption >> 5) & 1)
		{
			ip->Special[ip->SpecialNum] = AT_EXCELLENT_DAMAGE; ++ip->SpecialNum;
		}
		if ((ip->Type >= ITEM_STAFF && ip->Type < ITEM_STAFF + MAX_ITEM_INDEX) ||
			(ip->Type == ITEM_HELPER + 12)
			|| (ip->Type == ITEM_HELPER + 25 || ip->Type == ITEM_HELPER + 27))
		{
			if ((Item.ExcOption >> 4) & 1)
			{
				ip->SpecialValue[ip->SpecialNum] = CharacterAttribute->Level / 20;
				ip->Special[ip->SpecialNum] = AT_IMPROVE_MAGIC_LEVEL; ++ip->SpecialNum;
			}
			if ((Item.ExcOption >> 3) & 1)
			{
				ip->SpecialValue[ip->SpecialNum] = 2;
				ip->Special[ip->SpecialNum] = AT_IMPROVE_MAGIC_PERCENT; ++ip->SpecialNum;
			}
		}
		else
		{
			if ((Item.ExcOption >> 4) & 1)
			{
				ip->SpecialValue[ip->SpecialNum] = CharacterAttribute->Level / 20;
				ip->Special[ip->SpecialNum] = AT_IMPROVE_DAMAGE_LEVEL; ++ip->SpecialNum;
			}
			if ((Item.ExcOption >> 3) & 1)
			{
				ip->SpecialValue[ip->SpecialNum] = 2;
				ip->Special[ip->SpecialNum] = AT_IMPROVE_DAMAGE_PERCENT; ++ip->SpecialNum;
			}
		}
		if ((Item.ExcOption >> 2) & 1)
		{
			ip->SpecialValue[ip->SpecialNum] = 7;
			ip->Special[ip->SpecialNum] = AT_IMPROVE_ATTACK_SPEED; ++ip->SpecialNum;
		}
		if ((Item.ExcOption >> 1) & 1)
		{
			ip->Special[ip->SpecialNum] = AT_IMPROVE_GAIN_LIFE; ++ip->SpecialNum;
		}
		if ((Item.ExcOption) & 1)
		{
			ip->Special[ip->SpecialNum] = AT_IMPROVE_GAIN_MANA; ++ip->SpecialNum;
		}
	}
	if (ip->Type == ITEM_HELPER + 20)
	{
		switch (Level)
		{
		case 0:
			ip->SpecialValue[ip->SpecialNum] = 10;
			ip->Special[ip->SpecialNum] = AT_IMPROVE_MAGIC_PERCENT; ++ip->SpecialNum;
			ip->SpecialValue[ip->SpecialNum] = 10;
			ip->Special[ip->SpecialNum] = AT_IMPROVE_DAMAGE_PERCENT; ++ip->SpecialNum;
			ip->SpecialValue[ip->SpecialNum] = 10;
			ip->Special[ip->SpecialNum] = AT_IMPROVE_ATTACK_SPEED; ++ip->SpecialNum;
			break;

		case 3:
			ip->SpecialValue[ip->SpecialNum] = 10;
			ip->Special[ip->SpecialNum] = AT_IMPROVE_MAGIC_PERCENT; ++ip->SpecialNum;
			ip->SpecialValue[ip->SpecialNum] = 10;
			ip->Special[ip->SpecialNum] = AT_IMPROVE_DAMAGE_PERCENT; ++ip->SpecialNum;
			ip->SpecialValue[ip->SpecialNum] = 10;
			ip->Special[ip->SpecialNum] = AT_IMPROVE_ATTACK_SPEED; ++ip->SpecialNum;
			break;
		}
	}

	if (ip->Type == ITEM_HELPER + 107)
	{
		ip->SpecialValue[ip->SpecialNum] = 15;
		ip->Special[ip->SpecialNum] = AT_IMPROVE_MAGIC_PERCENT; ++ip->SpecialNum;
		ip->SpecialValue[ip->SpecialNum] = 15;
		ip->Special[ip->SpecialNum] = AT_IMPROVE_DAMAGE_PERCENT; ++ip->SpecialNum;
		ip->SpecialValue[ip->SpecialNum] = 10;
		ip->Special[ip->SpecialNum] = AT_IMPROVE_ATTACK_SPEED; ++ip->SpecialNum;
	}

	if ((ip->Type >= ITEM_BOW && ip->Type < ITEM_BOW + 8) ||
		ip->Type == ITEM_BOW + 17)
		ip->Part = GAME::EQUIPMENT_WEAPON_LEFT;
	if (ip->Type >= ITEM_STAFF + 21 && ip->Type <= ITEM_STAFF + 29)
		ip->Part = GAME::EQUIPMENT_WEAPON_LEFT;
	else if (ip->Type >= ITEM_SWORD && ip->Type < ITEM_STAFF + MAX_ITEM_INDEX)
		ip->Part = GAME::EQUIPMENT_WEAPON_RIGHT;
	else if (ip->Type >= ITEM_SHIELD && ip->Type < ITEM_SHIELD + MAX_ITEM_INDEX)
		ip->Part = GAME::EQUIPMENT_WEAPON_LEFT;
	else if (ip->Type >= ITEM_HELM && ip->Type < ITEM_HELM + MAX_ITEM_INDEX)
		ip->Part = GAME::EQUIPMENT_HELM;
	else if (ip->Type >= ITEM_ARMOR && ip->Type < ITEM_ARMOR + MAX_ITEM_INDEX)
		ip->Part = GAME::EQUIPMENT_ARMOR;
	else if (ip->Type >= ITEM_PANTS && ip->Type < ITEM_PANTS + MAX_ITEM_INDEX)
		ip->Part = GAME::EQUIPMENT_PANTS;
	else if (ip->Type >= ITEM_GLOVES && ip->Type < ITEM_GLOVES + MAX_ITEM_INDEX)
		ip->Part = GAME::EQUIPMENT_GLOVES;
	else if (ip->Type >= ITEM_BOOTS && ip->Type < ITEM_BOOTS + MAX_ITEM_INDEX)
		ip->Part = GAME::EQUIPMENT_BOOTS;
	else if (ip->Type >= ITEM_WING && ip->Type < ITEM_WING + 7)
		ip->Part = GAME::EQUIPMENT_WING;
	else if (ip->Type >= ITEM_WING + 36 && ip->Type <= ITEM_WING + 43)
		ip->Part = GAME::EQUIPMENT_WING;
	else if (ip->Type == ITEM_HELPER + 5)
		ip->Part = GAME::EQUIPMENT_WEAPON_LEFT;
	else if (ip->Type >= ITEM_HELPER && ip->Type < ITEM_HELPER + 8)
		ip->Part = GAME::EQUIPMENT_HELPER;
	else if ((ip->Type >= ITEM_HELPER + 8 && ip->Type < ITEM_HELPER + 12) || (ip->Type == ITEM_HELPER + 20 && Level == 0)
		|| (ip->Type == ITEM_HELPER + 20 && Level == 3)
		)
		ip->Part = GAME::EQUIPMENT_RING_RIGHT;
	else if (ip->Type >= ITEM_HELPER + 21 && ip->Type <= ITEM_HELPER + 24)
		ip->Part = GAME::EQUIPMENT_RING_RIGHT;
	else if (ip->Type >= ITEM_HELPER + 25 && ip->Type <= ITEM_HELPER + 28)
		ip->Part = GAME::EQUIPMENT_AMULET;
	else if (ip->Type >= ITEM_HELPER + 12 && ip->Type < ITEM_HELPER + MAX_ITEM_INDEX)
		ip->Part = GAME::EQUIPMENT_AMULET;
	else
		ip->Part = -1;

	if (ip->Type == ITEM_HELPER + 4 ||
		ip->Type == ITEM_HELPER + 5)
	{
		mu_int32 NextLevel = ip->PetLevel + 1;

		switch (ip->Type)
		{
		case ITEM_HELPER + 5:
			{
				ip->NextPetExperience = ((10 + NextLevel)*NextLevel*NextLevel*NextLevel * 100);
				ip->DamageMin = (180 + (ip->PetLevel * 15));
				ip->DamageMax = (200 + (ip->PetLevel * 15));
				ip->WeaponSpeed = (20 + (ip->PetLevel * 4 / 5));
				ip->AttackSuccess = (1000 + ip->PetLevel) + (ip->PetLevel * 15);
			}
			break;

		case ITEM_HELPER + 4:
			{
				ip->NextPetExperience = ((10 + NextLevel)*NextLevel*NextLevel*NextLevel * 100);
				ip->DamageMin = (ip->PetLevel * 5);
				ip->DamageMax = ip->DamageMin + (ip->DamageMin / 2);
				ip->WeaponSpeed = (20 + (ip->PetLevel * 4 / 5));
				ip->AttackSuccess = (1000 + ip->PetLevel) + (ip->PetLevel * 15);

				mu_int32 Index = 0;
				for (mu_int32 i = 0; i < ip->SpecialNum; ++i)
				{
					if (ip->Special[i] == AT_SET_OPTION_IMPROVE_DEFENCE)
					{
						Index = i;
						break;
					}
				}

				if (Index == 0)
				{
					ip->SpecialValue[ip->SpecialNum] = (5 + ip->PetLevel * 2);
					ip->Special[ip->SpecialNum] = AT_SET_OPTION_IMPROVE_DEFENCE; ++ip->SpecialNum;
				}
				else
				{
					ip->SpecialValue[Index] = (5 + ip->PetLevel * 2);
					ip->Special[Index] = AT_SET_OPTION_IMPROVE_DEFENCE;
				}
			}
			break;
		}
	}
}

void ParseItem(SHARED::ITEM_DATA &Source, ITEM *Target)
{
	mu_int32 itemType = Source.ItemType * MAX_ITEM_INDEX + Source.ItemIndex;

	Target->Type = itemType;

	Target->Level = Source.Level;
	Target->Durability = Source.Durability;

	Target->Option = Source.Option;
	Target->ExcOption = Source.ExcOption;
	Target->SetOption = Source.SetOption;
	Target->HarmonyOption = (Source.HarmonyOption >> 4) & 0x0F;
	Target->HarmonyOptionLevel = Source.HarmonyOption & 0x0F;
	Target->SocketSeedSetOption = Source.BonusSocketOption;
	mu_memcpy(Target->SocketOption, Source.SocketOption, sizeof(Source.SocketOption));

	Target->Skill = Source.Skill;
	Target->Luck = Source.Luck;
	Target->ItemOption = Source.ItemOption;

	Target->SocketCount = MAX_SOCKETS;

	for (mu_uint32 i = 0; i < MAX_SOCKETS; ++i)
	{
		if (Target->SocketOption[i] == 0xFF)
		{
			Target->SocketCount = i;
			break;
		}
		else if (Target->SocketOption[i] == 0xFE)
		{
			Target->SocketSeedID[i] = SOCKET_EMPTY;
		}
		else
		{
			Target->SocketSeedID[i] = Target->SocketOption[i] % MAX_SOCKET_OPTION;
			Target->SocketSphereLv[i] = (mu_int32)(Target->SocketOption[i] / MAX_SOCKET_OPTION) + 1;
		}
	}

	if (g_SocketItemMgr.IsSocketItem(Target))
	{
		Target->HarmonyOption = 0;
		Target->HarmonyOptionLevel = 0;
	}
	else
	{
		Target->SocketSeedSetOption = SOCKET_EMPTY;
	}

	Target->byColorState = ITEM_COLOR_NORMAL;

	Target->PetLevel = Source.PetLevel;
	Target->PetExperience = Source.PetExperience;

	ItemConvert(Target, Source);
}

void CreateMoney(ITEM_t *ip, mu_int64 Money, EVector3 Position, mu_boolean CreateFlag)
{
	mu_int32 Type = ITEM_POTION + 15;

	ITEM *n = &ip->Item;

	n->Type = Type;
	n->Money = Money;
	n->Durability = 0;

	if (CreateFlag)
	{
		PlayBackground(SOUND_DROP_GOLD01);
	}

	OBJECT *o = &ip->Object;

	o->Flags.Set(OBJECT::eOF_LIVE);
	o->Type = MODEL_SWORD + Type;
	o->SubType = 1;

	ItemObjectAttribute(o);
	Vector(-50.0f, -50.0f, -30.0f, o->BoundingBoxMin);
	Vector(50.0f, 50.0f, 30.0f, o->BoundingBoxMax);
	VectorCopy(Position, o->Position);

	if (CreateFlag)
	{
		o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 180.0f;
		o->Gravity = 20.0f;
	}
	else
	{
		o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 30.0f;
	}

	ItemAngle(o);
}

void CreateItem(ITEM_t *ip, SHARED::ITEM_DATA &Item, EVector3 Position, mu_boolean CreateFlag)
{
	mu_int32 Type = Item.ItemType * MAX_ITEM_INDEX + Item.ItemIndex;

	ITEM *n = &ip->Item;

	ParseItem(Item, n);

	if (CreateFlag)
	{
		if (Type == ITEM_POTION + 13 ||
			Type == ITEM_POTION + 14 ||
			Type == ITEM_POTION + 16 ||
			Type == ITEM_WING + 15 ||
			Type == ITEM_POTION + 22 ||
			Type == ITEM_POTION + 31)
			PlayObject(SOUND_JEWEL01, &ip->Object);
		else if (Type == ITEM_POTION + 41)
			PlayObject(SOUND_JEWEL02, &ip->Object);
		else
			PlayObject(SOUND_DROP_ITEM01, &ip->Object);
	}

	OBJECT *o = &ip->Object;

	o->Flags.Set(OBJECT::eOF_LIVE);
	o->Type = MODEL_SWORD + Type;
	o->SubType = 1;
	if (Type == ITEM_POTION + 11)
	{
		switch (n->Level)
		{
		case 1:
			o->Type = MODEL_EVENT + 4;
			break;
		case 2:
			o->Type = MODEL_EVENT + 5;
			break;
		case 3:
			o->Type = MODEL_EVENT + 6;
			break;
		case 5:
			o->Type = MODEL_EVENT + 8;
			break;
		case 6:
			o->Type = MODEL_EVENT + 9;
			break;
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
			o->Type = MODEL_EVENT + 10;
			break;
		case 13:
			o->Type = MODEL_EVENT + 6;
			break;
		case 14:
		case 15:
			o->Type = MODEL_EVENT + 5;
			break;
		}
	}

	if (Type >= ITEM_POTION + 46 && Type <= ITEM_POTION + 48)
	{
		o->Type = MODEL_ITEM + Type;
	}

	if (Type >= ITEM_POTION + 32 && Type <= ITEM_POTION + 34)
	{
		if (n->Level == 1)
		{
			mu_int32 Num = Type - (ITEM_POTION + 32);
			o->Type = MODEL_EVENT + 21 + Num;
		}
	}
	else if (Type == (mu_int32)(ITEM_POTION + 21))
	{
		switch (n->Level)
		{
		case 1:
		case 2:
			o->Type = MODEL_EVENT + 11;
			break;
		}
	}
	else if (Type == (mu_int32)(ITEM_HELPER + 19))
	{
		switch (n->Level)
		{
		case 0:
			o->Type = MODEL_STAFF + 10;
			n->Level = 0;
			break;
		case 1:
			o->Type = MODEL_SWORD + 19;
			n->Level = 0;
			break;
		case 2:
			o->Type = MODEL_BOW + 18;
			n->Level = 0;
			break;
		}
	}
	else if (Type == ITEM_POTION + 23)
	{
		switch (n->Level)
		{
		case 1:
			o->Type = MODEL_EVENT + 12;
			break;
		}
	}
	else if (Type == ITEM_POTION + 24)
	{
		switch (n->Level)
		{
		case 1:
			o->Type = MODEL_EVENT + 13;
			break;
		}
	}
	else if (Type == ITEM_HELPER + 20)
	{
		switch (n->Level)
		{
		case 0:
			o->Type = MODEL_EVENT + 15;
			break;
		case 1:
		case 3:
			o->Type = MODEL_EVENT + 14;
			break;
		}
	}
	else if (Type == (mu_int32)(ITEM_POTION + 9))
	{
		switch (n->Level)
		{
		case 1:
			o->Type = MODEL_EVENT + 7;
			break;
		}
	}
	else if (Type == ITEM_HELPER + 14 && n->Level == 1)
	{
		o->Type = MODEL_EVENT + 16;
	}
	else if (Type == ITEM_HELPER + 11 && n->Level == 1)
	{
		o->Type = MODEL_EVENT + 18;
	}
	else if (Type == ITEM_POTION + 41)
	{
		o->Type = MODEL_POTION + 41;
	}
	else if (Type == ITEM_POTION + 42)
	{
		o->Type = MODEL_POTION + 42;
	}
	else if (Type == ITEM_POTION + 43)
	{
		o->Type = MODEL_POTION + 43;
	}
	else if (Type == ITEM_POTION + 44)
	{
		o->Type = MODEL_POTION + 44;
	}

	ItemObjectAttribute(o);

	Vector(-30.0f, -30.0f, -30.0f, o->BoundingBoxMin);
	Vector(30.0f, 30.0f, 30.0f, o->BoundingBoxMax);
	VectorCopy(Position, o->Position);

	if (CreateFlag)
	{
		if (o->Type == MODEL_EVENT + 8 || o->Type == MODEL_EVENT + 9)
		{
			mu_int16 scale = 55;
			EVector3 Angle;
			EVector3 light;
			Vector(1.0f, 1.0f, 1.0f, light);
			Vector(0.0f, 0.0f, 0.0f, Angle);
			EVector3 NewPosition;
			VectorCopy(Position, NewPosition);
			NewPosition[2] = RequestTerrainHeight(Position[0], Position[1]) + 3.0f;
			CreateEffect(0, MODEL_SKILL_FURY_STRIKE + 6, NewPosition, Angle, light, 0, o, scale);
			CreateEffect(0, MODEL_SKILL_FURY_STRIKE + 4, NewPosition, Angle, light, 0, o, scale);

			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 33.0f;
			o->Gravity = 50.0f;
		}
		else
		{
			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 180.0f;
			o->Gravity = 20.0f;
		}
	}
	else
	{
		o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 30.0f;
	}

	ItemAngle(o);
}

void ClearItems()
{
	for (mu_int32 i = 0; i < MAX_ITEMS; ++i)
	{
		OBJECT *o = &Items[i].Object;
		o->Flags.Clear();
	}
}

void ItemAngle(OBJECT *o)
{
	Vector(0.0f, 0.0f, -45.0f, o->Angle);

	if (o->Type >= MODEL_SWORD && o->Type < MODEL_AXE + MAX_ITEM_INDEX)
	{
		o->Angle[0] = 60.0f;
		if (o->Type == MODEL_SWORD + 19)
			o->Scale = 0.7f;
	}
	else if (o->Type == MODEL_BOW + 20 || o->Type == MODEL_BOW + 21 || o->Type == MODEL_BOW + 22)
	{
		o->Angle[0] = 0.0f;
		o->Angle[1] = 0.0f;
	}
	else if ((o->Type >= MODEL_BOW + 8 && o->Type < MODEL_BOW + 17) ||
		(o->Type >= MODEL_BOW + 18 && o->Type < MODEL_BOW + 20))
	{
		o->Angle[0] = 90.0f;
		o->Angle[1] = 0.0f;
	}
	else if (o->Type >= MODEL_MACE && o->Type < MODEL_STAFF + MAX_ITEM_INDEX)
	{
		o->Angle[0] = 0.0f;
		o->Angle[1] = 270.0f;
	}
	else if (o->Type >= MODEL_SHIELD && o->Type < MODEL_SHIELD + MAX_ITEM_INDEX)
	{
		o->Angle[1] = 270.0f;
		o->Angle[2] = 270.0f - 45.0f;
	}
	else if (MODEL_HELM + 39 <= o->Type && MODEL_HELM + 44 >= o->Type)
	{
		o->Scale = 1.5f;
		o->Angle[2] = 45.0f;
	}
	else if (o->Type >= MODEL_ARMOR && o->Type < MODEL_GLOVES + MAX_ITEM_INDEX)
		o->Angle[0] = 270.0f;
	else if (o->Type >= MODEL_WING + 32 && o->Type <= MODEL_WING + 34)
	{
		o->Scale = 0.3f;
		o->Angle[0] = 0.0f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type >= MODEL_WING + 60 && o->Type <= MODEL_WING + 65)
	{
		o->Angle[0] = 0.0f;
		o->Scale = 0.6f;
	}
	else if (o->Type >= MODEL_WING + 70 && o->Type <= MODEL_WING + 74)
	{
		o->Angle[0] = 0.0f;
		o->Scale = 0.6f;
	}
	else if (o->Type >= MODEL_WING + 100 && o->Type <= MODEL_WING + 129)
	{
		o->Angle[0] = 0.0f;
		o->Scale = 0.6f;
	}
	else if (o->Type == MODEL_POTION + 45)
	{
		o->Scale = 0.9f;
		o->Angle[0] = 0.0f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type >= MODEL_POTION + 46 && o->Type <= MODEL_POTION + 48)
	{
		o->Scale = 0.7f;
		o->Angle[0] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 49)
	{
		o->Scale = 0.9f;
		o->Angle[0] = 0.0f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 50)
	{
		o->Scale = 0.26f;
		o->Angle[0] = 0.0f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type >= MODEL_POTION + 32 && o->Type <= MODEL_POTION + 34)
	{
		o->Scale = 0.7f;
		o->Angle[0] = 0.0f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type >= MODEL_EVENT + 21 && o->Type <= MODEL_EVENT + 23)
	{
		o->Scale = 0.7f;
		o->Angle[0] = 0.0f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type >= MODEL_HELPER + 46 && o->Type <= MODEL_HELPER + 48)
	{
		o->Scale = 0.5f;
		o->Angle[0] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 54)
	{
		o->Scale = 0.5f;
		o->Angle[0] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 58)
	{
		o->Scale = 0.3f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 59 || o->Type == MODEL_POTION + 60)
	{
		o->Scale = 0.3f;
		o->Angle[0] = 90.0f;
		o->Angle[1] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 61 || o->Type == MODEL_POTION + 62)
	{
		o->Scale = 0.3f;
		o->Angle[0] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 53)
	{
		o->Scale = 0.2f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_HELPER + 43 || o->Type == MODEL_HELPER + 44 || o->Type == MODEL_HELPER + 45)
	{
		o->Scale = 0.5f;
		o->Angle[0] = 90.0f;
	}
	else if (o->Type >= MODEL_POTION + 70 && o->Type <= MODEL_POTION + 71)
	{
		o->Scale = 0.6f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type >= MODEL_POTION + 72 && o->Type <= MODEL_POTION + 77)
	{
		o->Scale = 0.5f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_HELPER + 59)
	{
		o->Scale = 0.2f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type >= MODEL_HELPER + 54 && o->Type <= MODEL_HELPER + 58)
	{
		o->Scale = 0.7f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type >= MODEL_POTION + 78 && o->Type <= MODEL_POTION + 82)
	{
		o->Scale = 0.5f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_HELPER + 60)
	{
		o->Scale = 1.5f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_HELPER + 61)
	{
		o->Scale = 0.5f;
		o->Angle[0] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 83)
	{
		o->Scale = 0.3f;
		o->Angle[0] = 90.0f;
	}
	else if (o->Type >= MODEL_POTION + 145 && o->Type <= MODEL_POTION + 150)
	{
		o->Scale = 0.3f;
		o->Angle[0] = 90.0f;
	}
	else if (o->Type >= MODEL_HELPER + 125 && o->Type <= MODEL_HELPER + 127)
	{
		o->Scale = 0.5f;
		o->Angle[0] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 91)
	{
		o->Scale = 0.5f;
		o->Angle[0] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 92)
	{
		o->Scale = 0.5f;
		o->Angle[0] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 93)
	{
		o->Scale = 0.5f;
		o->Angle[0] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 95)
	{
		o->Scale = 0.5f;
		o->Angle[0] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 94)
	{
		o->Scale = 0.6f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 84)
	{
		o->Scale = 0.8f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 85)
	{
		o->Scale = 0.9f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 86)
	{
		o->Scale = 0.7f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 87)
	{
		o->Scale = 1.3f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 88)
	{
		o->Scale = 0.7f;
		o->Angle[0] = 180.0f;
		o->Angle[1] = 180.0f;
	}
	else if (o->Type == MODEL_POTION + 89)
	{
		o->Scale = 0.7f;
		o->Angle[0] = 30.0f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 90)
	{
		o->Scale = 0.7f;
		o->Angle[0] = 30.0f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type >= MODEL_HELPER + 62 && o->Type <= MODEL_HELPER + 63)
	{
		o->Scale = 0.5f;
		o->Angle[0] = 90.0f;
	}
	else if (o->Type >= MODEL_POTION + 97 && o->Type <= MODEL_POTION + 98)
	{
		o->Scale = 0.5f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 140)
	{
		o->Scale = 0.5f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 96)
	{
		o->Scale = 0.2f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type >= MODEL_HELPER + 64 && o->Type <= MODEL_HELPER + 65)
	{
		switch (o->Type)
		{
		case MODEL_HELPER + 64: o->Scale = 0.21f; break;
		case MODEL_HELPER + 65: o->Scale = 0.5f; break;
		}
		o->Angle[2] = 70.0f;
	}
	else if (o->Type == MODEL_HELPER + 49)
	{
		o->Angle[0] = 90.0f;
		o->Angle[1] = 0.0f;
		o->Scale = 0.3f;
	}
	else if (o->Type == MODEL_HELPER + 50)
	{
		o->Angle[0] = 0.0f;
		o->Scale = 0.6f;
	}
	else if (o->Type == MODEL_HELPER + 51)
	{
		o->Angle[0] = 90.0f;
		o->Scale = 0.45f;
	}
	else if (o->Type == MODEL_POTION + 64)
	{
		o->Angle[0] = 0.0f;
		o->Scale = 0.8f;
	}
	else if (o->Type == MODEL_HELPER + 52)
	{
		o->Angle[0] = 0.0f;
		o->Scale = 1.2f;
	}
	else if (o->Type == MODEL_HELPER + 53)
	{
		o->Angle[0] = 0.0f;
		o->Scale = 1.2f;
	}
	else if (o->Type == MODEL_POTION + 65)
	{
		o->Angle[0] = 90.0f;
		o->Scale = 0.6f;
	}
	else if (o->Type == MODEL_POTION + 66)
	{
		o->Angle[0] = 90.0f;
		o->Scale = 0.8f;
	}
	else if (o->Type == MODEL_POTION + 67)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 0.8f;
	}
	else if (o->Type == MODEL_POTION + 68)
	{
		o->Angle[2] = -135.0f;
		o->Scale = 0.6f;
	}
	else if (o->Type == MODEL_EVENT + 4)
	{
		o->Angle[0] = 90.0f;
	}
	else if (o->Type == MODEL_EVENT + 8 || o->Type == MODEL_EVENT + 9)
	{
		o->Angle[0] = 270.0f;
		o->Angle[2] = 45.0f;
	}
	else if (o->Type == MODEL_EVENT + 10)
	{
		o->Scale = .2f;
	}
	else if (o->Type == MODEL_EVENT + 5)
	{
		o->Angle[0] = 90.0f;
	}
	else if (o->Type == MODEL_EVENT + 17)
	{
		o->Angle[0] = 270.0f;
		o->Angle[2] = 45.0f;
	}
	else if (o->Type == MODEL_POTION + 23)
	{
		o->Angle[1] = 45.0f;
		o->Angle[2] = 45.0f;
	}
	else if (o->Type == MODEL_POTION + 24)
	{
		o->Angle[2] = 45.0f;
	}
	else if ((o->Type >= MODEL_POTION + 25 && o->Type < MODEL_POTION + 27) || o->Type == MODEL_HELPER + 14)
	{
		o->Angle[2] = 45.0f;
	}
	else if (o->Type == MODEL_POTION + 17)
	{
		o->Angle[0] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 63)
	{
		o->Angle[0] = 70.0f;
		o->Scale = 1.5f;
	}
	else if (o->Type == MODEL_POTION + 99)
	{
		o->Angle[0] = 70.0f;
		o->Angle[2] = 0.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_POTION + 52)
	{
		o->Angle[2] = -10.0f;
		o->Scale = 0.4f;
	}
	else if (o->Type == MODEL_POTION + 18)
	{
		o->Angle[0] = o->Angle[2] = 270.0f;
	}
	else if (o->Type == MODEL_POTION + 19)
	{
		o->Angle[0] = 270.0f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 29)
	{
		o->Angle[0] = 90.0f;
		o->Angle[2] = 70.0f;
	}
	else if (o->Type == MODEL_EVENT + 11)
	{
		o->Angle[0] = 115.0f;
		o->Angle[1] = 75.0f;
		o->Angle[2] = 8.0f;
		o->Scale = 0.4f;
	}
	else if (o->Type == MODEL_HELPER + 16 || o->Type == MODEL_HELPER + 17)
	{
		o->Angle[0] = -45.0f;
		o->Angle[1] = -5.0f;
		o->Angle[2] = 18.0f;
		o->Scale = 0.48f;
	}
	else if (o->Type == MODEL_HELPER + 18)
	{
		o->Angle[0] = 165.0f;
		o->Angle[1] = -168.0f;
		o->Angle[2] = 198.0f;
		o->Scale = 0.48f;
	}
	else if (o->Type == MODEL_HELPER + 30)//  ¸ÁÅä.
	{
		o->Angle[0] = -45.0f;
		o->Angle[1] = 0.0f;
		o->Angle[2] = 45.0f;
		o->Scale = 0.5f;
	}
	else if (o->Type == MODEL_EVENT + 16)
	{
		o->Angle[2] = 45.0f;
		o->Scale = 0.5f;
	}
	else if (o->Type == MODEL_EVENT + 12)
	{
		o->Angle[0] = 160.0f;
		o->Angle[1] = -183.0f;
		o->Angle[2] = 198.0f;
		o->Scale = 0.38f;
	}
	else if (o->Type == MODEL_EVENT + 13)
	{
		o->Angle[0] = 160.0f;
		o->Angle[1] = -183.0f;
		o->Angle[2] = 198.0f;
		o->Scale = 0.54f;
	}
	else if (o->Type == MODEL_POTION + 21)
	{
		o->Angle[0] = 270.0f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_EVENT + 7)
	{
		o->Angle[2] = 45.0f;
	}
	else if (o->Type == MODEL_POTION + 20)
	{
		o->Angle[2] = 45.0f;
	}
	else if (o->Type >= MODEL_HELPER + 21 && o->Type <= MODEL_HELPER + 24)
	{
		o->Angle[2] = 20.0f;
	}
	else if (o->Type == MODEL_HELPER + 33)
	{
		o->Angle[2] = 45.0f;
		o->Scale = 1.2f;
	}
	else if (o->Type == MODEL_HELPER + 34)
	{
		o->Angle[0] = 90.0f;
	}
	else if (o->Type == MODEL_HELPER + 35)
	{
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_HELPER + 36)
	{
		o->Angle[2] = 90.0f;
		o->Scale = 1.3f;
	}
	else if (o->Type == MODEL_HELPER + 37)
	{
		o->Angle[2] = 180.0f;
	}
	else if (o->Type == MODEL_POTION + 16)
	{
		o->Angle[0] = 270.0f;
		o->Angle[2] = 90.0f - 45.0f;
	}
	else if (o->Type == MODEL_POTION + 42)
	{
		o->Angle[0] = 270.0f;
		o->Angle[2] = -15.0f;
		o->Scale = 1.3f;
	}
	else if (o->Type == MODEL_POTION + 43 || o->Type == MODEL_POTION + 44)
	{
		o->Angle[0] = 270.0f;
		o->Angle[2] = -15.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type >= MODEL_ETC + 19 && o->Type <= MODEL_ETC + 27)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 0.8f;
	}
	else if (o->Type == MODEL_HELPER + 66)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_POTION + 100)
	{
		o->Angle[0] = 180.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type >= MODEL_TYPE_CHARM_MIXWING + EWS_BEGIN &&
		o->Type <= MODEL_TYPE_CHARM_MIXWING + EWS_END)
	{
		o->Scale = 0.5f;
		o->Angle[2] = 90.0f;
	}
	else if (o->Type == MODEL_POTION + 110 || o->Type == MODEL_POTION + 111)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_HELPER + 107)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_HELPER + 104)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_HELPER + 105)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_HELPER + 103)
	{
		o->Angle[0] = 0.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_POTION + 133)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_HELPER + 109)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_HELPER + 110)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_HELPER + 111)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_HELPER + 112)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_HELPER + 113)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_HELPER + 114)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_HELPER + 115)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_POTION + 112)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_POTION + 113)
	{
		o->Angle[0] = 270.0f;
		o->Scale = 1.0f;
	}
	else if (o->Type == MODEL_HELPER + 116)
	{
		o->Scale = 0.5f;
		o->Angle[0] = 90.0f;
	}
	else if (o->Type == MODEL_HELPER + 121)
	{
		o->Scale = 0.5f;
		o->Angle[0] = 90.0f;
	}
	else if (o->Type == MODEL_HELPER + 123)
	{
		o->Scale = 0.4f;
		o->Angle[0] = 30.0f;
	}
	else if (o->Type >= MODEL_WING && o->Type < MODEL_WING + MAX_ITEM_INDEX)
	{
		{
			o->Angle[0] = 270.0f;
			o->Angle[2] = 90.0f - 45.0f;
		}
	}
	else
	{
		o->Angle[0] = 0.0f;
	}
}

mu_int32 CompareItem(ITEM *item1, ITEM *item2)
{
	mu_int32 equal = 0;
	if (item1->Type != item2->Type)
	{
		return  2;
	}
	
	mu_int32 level1 = item1->Level;
	mu_int32 level2 = item2->Level;
	mu_int32 option1 = item1->ExcOption;
	mu_int32 option2 = item2->ExcOption;
	mu_int32 skill1 = item1->Skill;
	mu_int32 skill2 = item2->Skill;

	equal = 1;
	if (level1 == level2)
	{
		equal = 0;
	}
	else if (level1 < level2)
	{
		equal = -1;
	}

	if (equal == 0)
	{
		if (skill1 < skill2)
		{
			equal = -1;
		}
		else if (skill1 > skill2)
		{
			equal = 1;
		}
	}

	if (equal == 0)
	{
		if (option1 < option2)
		{
			equal = -1;
		}
		else if (option1 > option2)
		{
			equal = 1;
		}
	}

	if (equal == 0)
	{
		if (item1->SpecialNum < item2->SpecialNum)
		{
			equal = -1;
		}
		else if (item1->SpecialNum > item2->SpecialNum)
		{
			equal = 1;
		}
		else
		{
			mu_int32 Num = EMath::Max(item1->SpecialNum, item2->SpecialNum);
			mu_int32 addOption1 = 0;
			mu_int32 addOptionV1 = 0;
			mu_int32 addOption2 = 0;
			mu_int32 addOptionV2 = 0;

			for (mu_int32 i = 0; i < Num; ++i)
			{
				switch (item1->Special[i])
				{
				case AT_IMPROVE_DAMAGE:
				case AT_IMPROVE_MAGIC:
				case AT_IMPROVE_CURSE:
				case AT_IMPROVE_BLOCKING:
				case AT_IMPROVE_DEFENSE:
					addOption1 = 1;
					addOptionV1 = item1->SpecialValue[i];
					break;
				}
				switch (item2->Special[i])
				{
				case AT_IMPROVE_DAMAGE:
				case AT_IMPROVE_MAGIC:
				case AT_IMPROVE_CURSE:
				case AT_IMPROVE_BLOCKING:
				case AT_IMPROVE_DEFENSE:
					addOption2 = 1;
					addOptionV2 = item2->SpecialValue[i];
					break;
				}
			}

			if (addOption1 < addOption2 || addOptionV1 < addOptionV2)
			{
				equal = -1;
			}
			else if (addOption1 != addOption2 && addOptionV1 != addOptionV2)
			{
				equal = 1;
			}
		}
	}

	if (equal == 0)
	{
		if (item1->Durability < item2->Durability)
			equal = -1;
		if (item1->Durability > item2->Durability)
			equal = 1;
	}

	return  equal;
}