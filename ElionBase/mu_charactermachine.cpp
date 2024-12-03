#include "stdafx.h"
#include "mu_charactermachine.h"

MASTER_LEVEL_VALUE Master_Level_Data;
CLASS_ATTRIBUTE ClassAttribute[GAME::MAX_CLASS];
CHARACTER_MACHINE *CharacterMachine = nullptr;
CHARACTER_ATTRIBUTE *CharacterAttribute = nullptr;

mu_boolean g_AddDefense = false;

void CreateClassAttribute(mu_int32 Class, mu_int32 Strength, mu_int32 Dexterity, mu_int32 Vitality, mu_int32 Energy, mu_int32 Life, mu_int32 Mana, mu_int32 LevelLife, mu_int32 LevelMana, mu_int32 VitalityToLife, mu_int32 EnergyToMana)
{
	CLASS_ATTRIBUTE *c = &ClassAttribute[Class];
	c->Strength = Strength;
	c->Dexterity = Dexterity;
	c->Vitality = Vitality;
	c->Energy = Energy;
	c->Life = Life;
	c->Mana = Mana;
	c->LevelLife = LevelLife;
	c->LevelMana = LevelMana;
	c->VitalityToLife = VitalityToLife;
	c->EnergyToMana = EnergyToMana;
	c->Shield = 0;
}

void CreateClassAttributes()
{
	CreateClassAttribute(0, 18, 18, 15, 30, 80, 60, 1, 2, 1, 2);
	CreateClassAttribute(1, 28, 20, 25, 10, 110, 20, 2, 1, 2, 1);
	CreateClassAttribute(2, 50, 50, 50, 30, 110, 30, 110, 30, 6, 3);
	CreateClassAttribute(3, 30, 30, 30, 30, 120, 80, 1, 1, 2, 2);
	CreateClassAttribute(4, 30, 30, 30, 30, 120, 80, 1, 1, 2, 2);
	CreateClassAttribute(5, 50, 50, 50, 30, 110, 30, 110, 30, 6, 3);
}

void CHARACTER_MACHINE::Init()
{
	CreateClassAttributes();
	for (mu_int32 i = 0; i < GAME::MAX_EQUIPMENT; ++i)
	{
		Equipment[i].Type = -1;
	}
	Gold = 0;
	StorageGold = 0;
	InfinityArrowAdditionalMana = 0;
}

void CHARACTER_MACHINE::InitAddValue()
{
	Character.AddStrength = 0;
	Character.AddDexterity = 0;
	Character.AddVitality = 0;
	Character.AddEnergy = 0;
	Character.AddCharisma = 0;
	Character.AddLifeMax = 0;
	Character.AddManaMax = 0;
}

void CHARACTER_MACHINE::SetCharacter(mu_uint8 Class)
{
	CLASS_ATTRIBUTE *c = &ClassAttribute[Class];
	Character.Class = Class;
	Character.Level = 1;
	Character.Strength = c->Strength;
	Character.Dexterity = c->Dexterity;
	Character.Vitality = c->Vitality;
	Character.Energy = c->Energy;
	Character.Life = c->Life;
	Character.Mana = c->Mana;
	Character.LifeMax = c->Life;
	Character.ManaMax = c->Mana;
	Character.Shield = c->Shield;
	Character.ShieldMax = c->Shield;

	InitAddValue();

	for (mu_int32 j = 0; j < MAX_MAGIC; ++j)
	{
		Character.Skill[j] = 0;
	}
}

void CHARACTER_MACHINE::InputEnemyAttribute(MONSTER *e)
{
	memcpy(&Enemy, e, sizeof(MONSTER));
}

void CHARACTER_MACHINE::CalculateDamage()
{
	mu_uint16 DamageMin, DamageMax;

	ITEM *Right = &Equipment[GAME::EQUIPMENT_WEAPON_RIGHT];
	ITEM *Left = &Equipment[GAME::EQUIPMENT_WEAPON_LEFT];
	ITEM *Amulet = &Equipment[GAME::EQUIPMENT_AMULET];
	ITEM *RRing = &Equipment[GAME::EQUIPMENT_RING_RIGHT];
	ITEM *LRing = &Equipment[GAME::EQUIPMENT_RING_LEFT];

	mu_uint16 Strength, Dexterity, Energy;

	Strength = Character.Strength + Character.AddStrength;
	Dexterity = Character.Dexterity + Character.AddDexterity;
	Energy = Character.Energy + Character.AddEnergy;

	mu_int32 CharacterClass = GetBaseClass(Character.Class);

	if (((GetEquipedBowType(Left) == BOWTYPE_BOW) && (Left->Durability != 0))
		|| ((GetEquipedBowType(Right) == BOWTYPE_CROSSBOW) && (Right->Durability != 0)))
	{
		Character.AttackDamageMinRight = Dexterity / 7 + Strength / 14;
		Character.AttackDamageMaxRight = Dexterity / 4 + Strength / 8;
		Character.AttackDamageMinLeft = Dexterity / 7 + Strength / 14;
		Character.AttackDamageMaxLeft = Dexterity / 4 + Strength / 8;
	}
	else
	{
		switch (CharacterClass)
		{
		case GAME::CLASS_ELF:
			Character.AttackDamageMinRight = (Strength + Dexterity) / 7;
			Character.AttackDamageMaxRight = (Strength + Dexterity) / 4;
			Character.AttackDamageMinLeft = (Strength + Dexterity) / 7;
			Character.AttackDamageMaxLeft = (Strength + Dexterity) / 4;
			break;

		case GAME::CLASS_KNIGHT:
			{
				mu_int32 minValue = 7;
				mu_int32 maxValue = 4;

				minValue = 6; maxValue = 4;

				Character.AttackDamageMinRight = Strength / minValue;
				Character.AttackDamageMaxRight = Strength / maxValue;
				Character.AttackDamageMinLeft = Strength / minValue;
				Character.AttackDamageMaxLeft = Strength / maxValue;
			}
			break;

		case GAME::CLASS_DARK:
			{
				mu_int32 minValue[2] = { 7, 10 };
				mu_int32 maxValue[2] = { 4, 5 };

				minValue[0] = 6; minValue[1] = 12;
				maxValue[0] = 4; maxValue[1] = 9;

				Character.AttackDamageMinRight = (Strength / minValue[0]) + Energy / minValue[1];
				Character.AttackDamageMaxRight = (Strength / maxValue[0]) + Energy / maxValue[1];
				Character.AttackDamageMinLeft = (Strength / minValue[0]) + Energy / minValue[1];
				Character.AttackDamageMaxLeft = (Strength / maxValue[0]) + Energy / maxValue[1];
			}
			break;

		case GAME::CLASS_DARK_LORD:
			Character.AttackDamageMinRight = Strength / 7 + Energy / 14;
			Character.AttackDamageMaxRight = Strength / 5 + Energy / 10;
			Character.AttackDamageMinLeft = Strength / 7 + Energy / 14;
			Character.AttackDamageMaxLeft = Strength / 5 + Energy / 10;
			break;

		case GAME::CLASS_SUMMONER:
			Character.AttackDamageMinRight = (Strength + Dexterity) / 7;
			Character.AttackDamageMaxRight = (Strength + Dexterity) / 4;
			Character.AttackDamageMinLeft = (Strength + Dexterity) / 7;
			Character.AttackDamageMaxLeft = (Strength + Dexterity) / 4;
			break;

		default:
			Character.AttackDamageMinRight = Strength / 8;
			Character.AttackDamageMaxRight = Strength / 4;
			Character.AttackDamageMinLeft = Strength / 8;
			Character.AttackDamageMaxLeft = Strength / 4;
			break;
		}
	}

	if (Equipment[GAME::EQUIPMENT_WING].Type != -1)
	{
		ITEM_ATTRIBUTE *p = &ItemAttribute[Equipment[GAME::EQUIPMENT_WING].Type];
		if (p->Durability != 0)
		{
			mu_float percent = CalcDurabilityPercent(Equipment[GAME::EQUIPMENT_WING].Durability, p->Durability, Equipment[GAME::EQUIPMENT_WING].Level, 0, 0);

			DamageMin = 0; DamageMax = 0;
			PlusSpecial(&DamageMin, AT_IMPROVE_DAMAGE, &Equipment[GAME::EQUIPMENT_WING]);
			PlusSpecial(&DamageMax, AT_IMPROVE_DAMAGE, &Equipment[GAME::EQUIPMENT_WING]);

			DamageMin = DamageMin - (mu_uint16)(DamageMin*percent);
			DamageMax = DamageMax - (mu_uint16)(DamageMax*percent);

			Character.AttackDamageMinRight += DamageMin;
			Character.AttackDamageMaxRight += DamageMax;
			Character.AttackDamageMinLeft += DamageMin;
			Character.AttackDamageMaxLeft += DamageMax;
		}
	}

	if (Right->Type != -1 && Right->Durability != 0)
	{
		ITEM_ATTRIBUTE *p = &ItemAttribute[Right->Type];
		mu_float   percent = CalcDurabilityPercent(Right->Durability, p->Durability, Right->Level, Right->ExcOption, Right->SetOption);

		DamageMin = Right->DamageMin;
		DamageMax = Right->DamageMax;

		PlusSpecial(&DamageMin, AT_IMPROVE_DAMAGE, Right);
		PlusSpecial(&DamageMax, AT_IMPROVE_DAMAGE, Right);

		DamageMin = DamageMin - (mu_uint16)(DamageMin*percent);
		DamageMax = DamageMax - (mu_uint16)(DamageMax*percent);

		if (Right->Type >= ITEM_STAFF && Right->Type <= ITEM_SHIELD)
		{
			Character.AttackDamageMinLeft += (mu_uint16)(DamageMin);
			Character.AttackDamageMaxLeft += (mu_uint16)(DamageMax);
		}
		else
		{
			Character.AttackDamageMinRight += DamageMin;
			Character.AttackDamageMaxRight += DamageMax;
		}

		PlusSpecial(&Character.AttackDamageMinRight, AT_IMPROVE_DAMAGE_LEVEL, Right);
		PlusSpecial(&Character.AttackDamageMaxRight, AT_IMPROVE_DAMAGE_LEVEL, Right);
		PlusSpecialPercent(&Character.AttackDamageMinRight, AT_IMPROVE_DAMAGE_PERCENT, Right, 2);
		PlusSpecialPercent(&Character.AttackDamageMaxRight, AT_IMPROVE_DAMAGE_PERCENT, Right, 2);
	}

	if (Left->Type != -1 && Left->Durability != 0)
	{
		ITEM_ATTRIBUTE *p = &ItemAttribute[Left->Type];
		mu_float   percent = CalcDurabilityPercent(Left->Durability, p->Durability, Left->Level, Left->ExcOption, Left->SetOption);

		DamageMin = Left->DamageMin;
		DamageMax = Left->DamageMax;

		PlusSpecial(&DamageMin, AT_IMPROVE_DAMAGE, Left);
		PlusSpecial(&DamageMax, AT_IMPROVE_DAMAGE, Left);

		DamageMin = DamageMin - (mu_uint16)(DamageMin*percent);
		DamageMax = DamageMax - (mu_uint16)(DamageMax*percent);

		if (Left->Type >= ITEM_STAFF && Left->Type <= ITEM_SHIELD)
		{
			Character.AttackDamageMinLeft += (mu_uint16)(DamageMin);
			Character.AttackDamageMaxLeft += (mu_uint16)(DamageMax);
		}
		else
		{
			Character.AttackDamageMinLeft += DamageMin;
			Character.AttackDamageMaxLeft += DamageMax;
		}

		PlusSpecial(&Character.AttackDamageMinLeft, AT_IMPROVE_DAMAGE_LEVEL, Left);
		PlusSpecial(&Character.AttackDamageMaxLeft, AT_IMPROVE_DAMAGE_LEVEL, Left);
		PlusSpecialPercent(&Character.AttackDamageMinLeft, AT_IMPROVE_DAMAGE_PERCENT, Left, 2);
		PlusSpecialPercent(&Character.AttackDamageMaxLeft, AT_IMPROVE_DAMAGE_PERCENT, Left, 2);
	}

	if (Amulet->Type != -1 && Amulet->Durability != 0)
	{
		PlusSpecial(&Character.AttackDamageMinRight, AT_IMPROVE_DAMAGE_LEVEL, Amulet);
		PlusSpecial(&Character.AttackDamageMaxRight, AT_IMPROVE_DAMAGE_LEVEL, Amulet);
		PlusSpecialPercent(&Character.AttackDamageMinRight, AT_IMPROVE_DAMAGE_PERCENT, Amulet, 2);
		PlusSpecialPercent(&Character.AttackDamageMaxRight, AT_IMPROVE_DAMAGE_PERCENT, Amulet, 2);

		PlusSpecial(&Character.AttackDamageMinLeft, AT_IMPROVE_DAMAGE_LEVEL, Amulet);
		PlusSpecial(&Character.AttackDamageMaxLeft, AT_IMPROVE_DAMAGE_LEVEL, Amulet);
		PlusSpecialPercent(&Character.AttackDamageMinLeft, AT_IMPROVE_DAMAGE_PERCENT, Amulet, 2);
		PlusSpecialPercent(&Character.AttackDamageMaxLeft, AT_IMPROVE_DAMAGE_PERCENT, Amulet, 2);
	}

	if (RRing->Type != -1 && RRing->Durability != 0)
	{
		PlusSpecialPercent(&Character.AttackDamageMinRight, AT_IMPROVE_DAMAGE_PERCENT, RRing, RRing->SpecialValue[1]);
		PlusSpecialPercent(&Character.AttackDamageMaxRight, AT_IMPROVE_DAMAGE_PERCENT, RRing, RRing->SpecialValue[1]);

		PlusSpecialPercent(&Character.AttackDamageMinLeft, AT_IMPROVE_DAMAGE_PERCENT, RRing, RRing->SpecialValue[1]);
		PlusSpecialPercent(&Character.AttackDamageMaxLeft, AT_IMPROVE_DAMAGE_PERCENT, RRing, RRing->SpecialValue[1]);
	}

	if (LRing->Type != -1 && LRing->Durability != 0)
	{
		PlusSpecialPercent(&Character.AttackDamageMinRight, AT_IMPROVE_DAMAGE_PERCENT, LRing, LRing->SpecialValue[1]);
		PlusSpecialPercent(&Character.AttackDamageMaxRight, AT_IMPROVE_DAMAGE_PERCENT, LRing, LRing->SpecialValue[1]);

		PlusSpecialPercent(&Character.AttackDamageMinLeft, AT_IMPROVE_DAMAGE_PERCENT, LRing, LRing->SpecialValue[1]);
		PlusSpecialPercent(&Character.AttackDamageMaxLeft, AT_IMPROVE_DAMAGE_PERCENT, LRing, LRing->SpecialValue[1]);
	}

	mu_uint16 Damage = 0;

	mu_zeromem(g_csItemOption.m_bySetOptionListOnOff, sizeof(g_csItemOption.m_bySetOptionListOnOff));

	g_csItemOption.PlusSpecial(&Damage, AT_SET_OPTION_IMPROVE_ATTACK_MIN);
	Character.AttackDamageMinLeft += Damage;
	Character.AttackDamageMinRight += Damage;

	Damage = 0;
	g_csItemOption.PlusSpecial(&Damage, AT_SET_OPTION_IMPROVE_ATTACK_MAX);
	Character.AttackDamageMaxLeft += Damage;
	Character.AttackDamageMaxRight += Damage;

	Damage = 0;
	g_csItemOption.PlusSpecialLevel(&Damage, Dexterity, AT_SET_OPTION_IMPROVE_ATTACK_1);
	Character.AttackDamageMinRight += Damage;
	Character.AttackDamageMaxRight += Damage;
	Character.AttackDamageMinLeft += Damage;
	Character.AttackDamageMaxLeft += Damage;

	Damage = 0;
	g_csItemOption.PlusSpecialLevel(&Damage, Strength, AT_SET_OPTION_IMPROVE_ATTACK_2);
	Character.AttackDamageMinRight += Damage;
	Character.AttackDamageMaxRight += Damage;
	Character.AttackDamageMinLeft += Damage;
	Character.AttackDamageMaxLeft += Damage;

	if ((Right->Type >= ITEM_BOW && Right->Type < ITEM_STAFF) &&
		(Left->Type >= ITEM_BOW && Left->Type < ITEM_STAFF))
	{
		mu_int32 LLevel = Left->Level;
		mu_int32 RLevel = Right->Level;

		if (Left->Type == ITEM_BOW + 7 && LLevel >= 1)
		{
			Character.AttackDamageMinRight += (mu_uint16)(Character.AttackDamageMinRight*((LLevel * 2 + 1)*0.01f) + 1);
			Character.AttackDamageMaxRight += (mu_uint16)(Character.AttackDamageMaxRight*((LLevel * 2 + 1)*0.01f) + 1);
		}
		else
			if (Right->Type == ITEM_BOW + 15 && RLevel >= 1)
			{
				Character.AttackDamageMinLeft += (mu_uint16)(Character.AttackDamageMinLeft*((RLevel * 2 + 1)*0.01f) + 1);
				Character.AttackDamageMaxLeft += (mu_uint16)(Character.AttackDamageMaxLeft*((RLevel * 2 + 1)*0.01f) + 1);
			}
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll3))
	{
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 74);
		Character.AttackDamageMinRight += Item_data.m_byValue1;
		Character.AttackDamageMaxRight += Item_data.m_byValue1;
		Character.AttackDamageMinLeft += Item_data.m_byValue1;
		Character.AttackDamageMaxLeft += Item_data.m_byValue1;
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin2))
	{
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 47);
		Character.AttackDamageMinRight += Item_data.m_byValue1;
		Character.AttackDamageMaxRight += Item_data.m_byValue1;
		Character.AttackDamageMinLeft += Item_data.m_byValue1;
		Character.AttackDamageMaxLeft += Item_data.m_byValue1;
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_CherryBlossom_Petal))
	{
		const ITEM_ADD_OPTION& Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 87);
		Character.AttackDamageMinRight += Item_data.m_byValue1;
		Character.AttackDamageMaxRight += Item_data.m_byValue1;
		Character.AttackDamageMinLeft += Item_data.m_byValue1;
		Character.AttackDamageMaxLeft += Item_data.m_byValue1;
	}
	
	Character.AttackDamageMinRight += g_SocketItemMgr.m_StatusBonus.m_iAttackDamageMinBonus;
	Character.AttackDamageMaxRight += g_SocketItemMgr.m_StatusBonus.m_iAttackDamageMaxBonus;
	Character.AttackDamageMinLeft += g_SocketItemMgr.m_StatusBonus.m_iAttackDamageMinBonus;
	Character.AttackDamageMaxLeft += g_SocketItemMgr.m_StatusBonus.m_iAttackDamageMaxBonus;

	if (g_isCharacterBuff((&Hero->Object), eBuff_BlessingOfXmax))
	{
		mu_int32 _Temp = 0;
		_Temp = Character.Level / 3 + 45;

		Character.AttackDamageMinRight += _Temp;
		Character.AttackDamageMaxRight += _Temp;
		Character.AttackDamageMinLeft += _Temp;
		Character.AttackDamageMaxLeft += _Temp;
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_StrengthOfSanta))
	{
		mu_int32 _Temp = 30;

		Character.AttackDamageMinRight += _Temp;
		Character.AttackDamageMaxRight += _Temp;
		Character.AttackDamageMinLeft += _Temp;
		Character.AttackDamageMaxLeft += _Temp;
	}
}

void CHARACTER_MACHINE::CalculateCriticalDamage()
{
	Character.CriticalDamage = 0;
	PlusSpecial(&Character.CriticalDamage, AT_LUCK, &Equipment[GAME::EQUIPMENT_WEAPON_RIGHT]);
	PlusSpecial(&Character.CriticalDamage, AT_LUCK, &Equipment[GAME::EQUIPMENT_WEAPON_LEFT]);
	PlusSpecial(&Character.CriticalDamage, AT_LUCK, &Equipment[GAME::EQUIPMENT_HELM]);
	PlusSpecial(&Character.CriticalDamage, AT_LUCK, &Equipment[GAME::EQUIPMENT_ARMOR]);
	PlusSpecial(&Character.CriticalDamage, AT_LUCK, &Equipment[GAME::EQUIPMENT_PANTS]);
	PlusSpecial(&Character.CriticalDamage, AT_LUCK, &Equipment[GAME::EQUIPMENT_GLOVES]);
	PlusSpecial(&Character.CriticalDamage, AT_LUCK, &Equipment[GAME::EQUIPMENT_BOOTS]);
	PlusSpecial(&Character.CriticalDamage, AT_LUCK, &Equipment[GAME::EQUIPMENT_WING]);
}

void CHARACTER_MACHINE::CalculateMagicDamage()
{
	mu_uint16 Energy;
	Energy = Character.Energy + Character.AddEnergy;
	Character.MagicDamageMin = Energy / 9;
	Character.MagicDamageMax = Energy / 4;

	ITEM* Right = &Equipment[GAME::EQUIPMENT_WEAPON_RIGHT];
	ITEM* Left = &Equipment[GAME::EQUIPMENT_WEAPON_LEFT];
	ITEM* Amulet = &Equipment[GAME::EQUIPMENT_AMULET];
	ITEM* RRing = &Equipment[GAME::EQUIPMENT_RING_RIGHT];
	ITEM* LRing = &Equipment[GAME::EQUIPMENT_RING_LEFT];

	mu_float percent;
	mu_uint16 DamageMin = 0;
	mu_uint16 DamageMax = 0;

	if (Equipment[GAME::EQUIPMENT_WING].Type != -1)
	{
		ITEM_ATTRIBUTE *p = &ItemAttribute[Equipment[GAME::EQUIPMENT_WING].Type];
		ITEM *ipWing = &Equipment[GAME::EQUIPMENT_WING];
		percent = CalcDurabilityPercent(ipWing->Durability, p->Durability, ipWing->Level, 0);

		DamageMin = 0;
		DamageMax = 0;
		PlusSpecial(&DamageMin, AT_IMPROVE_MAGIC, &Equipment[GAME::EQUIPMENT_WING]);
		PlusSpecial(&DamageMax, AT_IMPROVE_MAGIC, &Equipment[GAME::EQUIPMENT_WING]);

		DamageMin = DamageMin - (mu_uint16)(DamageMin*percent);
		DamageMax = DamageMax - (mu_uint16)(DamageMax*percent);

		Character.MagicDamageMin += DamageMin;
		Character.MagicDamageMax += DamageMax;
	}

	if (Right->Type != -1 && Right->Durability != 0)
	{
		ITEM_ATTRIBUTE *p = &ItemAttribute[Right->Type];
		percent = CalcDurabilityPercent(Right->Durability, p->Durability, Right->Level, Right->ExcOption, Right->SetOption);
		DamageMin = 0; DamageMax = 0;

		if (Right->Type == ITEM_SWORD + 21
			|| Right->Type == ITEM_SWORD + 31
			|| Right->Type == ITEM_SWORD + 23
			|| Right->Type == ITEM_SWORD + 25
			|| Right->Type == ITEM_SWORD + 28
			)
		{
			PlusSpecial(&DamageMin, AT_IMPROVE_DAMAGE, Right);
			PlusSpecial(&DamageMax, AT_IMPROVE_DAMAGE, Right);
		}
		else
		{
			PlusSpecial(&DamageMin, AT_IMPROVE_MAGIC, Right);
			PlusSpecial(&DamageMax, AT_IMPROVE_MAGIC, Right);
		}

		Character.MagicDamageMin += DamageMin - (mu_uint16)(DamageMin*percent);
		Character.MagicDamageMax += DamageMax - (mu_uint16)(DamageMax*percent);

		PlusSpecial(&Character.MagicDamageMin, AT_IMPROVE_MAGIC_LEVEL, Right);
		PlusSpecial(&Character.MagicDamageMax, AT_IMPROVE_MAGIC_LEVEL, Right);
		PlusSpecialPercent(&Character.MagicDamageMin, AT_IMPROVE_MAGIC_PERCENT, Right, 2);
		PlusSpecialPercent(&Character.MagicDamageMax, AT_IMPROVE_MAGIC_PERCENT, Right, 2);
	}

	if (Left->Type != -1 && Left->Durability != 0)
	{
		if (GAME::CLASS_SUMMONER != GetBaseClass(Character.Class))
		{
			ITEM_ATTRIBUTE *p = &ItemAttribute[Left->Type];
			percent = CalcDurabilityPercent(Left->Durability, p->Durability, Left->Level, Left->ExcOption, Left->SetOption);
			DamageMin = 0; DamageMax = 0;

			if (Left->Type >= ITEM_SWORD && Left->Type < ITEM_SHIELD)
			{
				if (Left->Type == ITEM_SWORD + 28)
				{
					PlusSpecial(&DamageMin, AT_IMPROVE_DAMAGE, Left);
					PlusSpecial(&DamageMax, AT_IMPROVE_DAMAGE, Left);
				}
				else
				{
					PlusSpecial(&DamageMin, AT_IMPROVE_MAGIC, Left);
					PlusSpecial(&DamageMax, AT_IMPROVE_MAGIC, Left);
				}

				if (GetBaseClass(Character.Class) != GAME::CLASS_DARK)
				{
					Character.MagicDamageMin += DamageMin - (mu_uint16)(DamageMin*percent);
					Character.MagicDamageMax += DamageMax - (mu_uint16)(DamageMax*percent);
				}

				PlusSpecial(&Character.MagicDamageMin, AT_IMPROVE_MAGIC_LEVEL, Left);
				PlusSpecial(&Character.MagicDamageMax, AT_IMPROVE_MAGIC_LEVEL, Left);
				PlusSpecialPercent(&Character.MagicDamageMin, AT_IMPROVE_MAGIC_PERCENT, Left, 2);
				PlusSpecialPercent(&Character.MagicDamageMax, AT_IMPROVE_MAGIC_PERCENT, Left, 2);
			}
		}
	}

	if (Amulet->Type != -1 && Amulet->Durability != 0)
	{
		PlusSpecial(&Character.MagicDamageMin, AT_IMPROVE_MAGIC_LEVEL, Amulet);
		PlusSpecial(&Character.MagicDamageMax, AT_IMPROVE_MAGIC_LEVEL, Amulet);
		PlusSpecialPercent(&Character.MagicDamageMin, AT_IMPROVE_MAGIC_PERCENT, Amulet, 2);
		PlusSpecialPercent(&Character.MagicDamageMax, AT_IMPROVE_MAGIC_PERCENT, Amulet, 2);
	}

	if (RRing->Type != -1 && RRing->Durability != 0)
	{
		PlusSpecialPercent(&Character.MagicDamageMin, AT_IMPROVE_MAGIC_PERCENT, RRing, RRing->SpecialValue[0]);
		PlusSpecialPercent(&Character.MagicDamageMax, AT_IMPROVE_MAGIC_PERCENT, RRing, RRing->SpecialValue[0]);
	}

	if (LRing->Type != -1 && LRing->Durability != 0)
	{
		PlusSpecialPercent(&Character.MagicDamageMin, AT_IMPROVE_MAGIC_PERCENT, LRing, LRing->SpecialValue[0]);
		PlusSpecialPercent(&Character.MagicDamageMax, AT_IMPROVE_MAGIC_PERCENT, LRing, LRing->SpecialValue[0]);
	}

	mu_uint16 MagicDamage = 0;

	MagicDamage = 0;
	g_csItemOption.PlusSpecialLevel(&MagicDamage, Energy, AT_SET_OPTION_IMPROVE_MAGIC);
	Character.MagicDamageMin += MagicDamage;
	Character.MagicDamageMax += MagicDamage;

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll4))
	{
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 75);
		Character.MagicDamageMin += Item_data.m_byValue1;
		Character.MagicDamageMax += Item_data.m_byValue1;
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin2))
	{
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 47);
		Character.MagicDamageMin += Item_data.m_byValue1;
		Character.MagicDamageMax += Item_data.m_byValue1;
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_CherryBlossom_Petal))
	{
		const ITEM_ADD_OPTION& Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 87);
		Character.MagicDamageMin += Item_data.m_byValue1;
		Character.MagicDamageMax += Item_data.m_byValue1;
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_SwellOfMagicPower))
	{
		mu_int32 iAdd_MP = (CharacterAttribute->Energy / 9)*0.2f;
		Character.MagicDamageMin += iAdd_MP;
	}

	Character.MagicDamageMin += g_SocketItemMgr.m_StatusBonus.m_iSkillAttackDamageBonus;
	Character.MagicDamageMax += g_SocketItemMgr.m_StatusBonus.m_iSkillAttackDamageBonus;
	Character.MagicDamageMin += g_SocketItemMgr.m_StatusBonus.m_iMagicPowerBonus;
	Character.MagicDamageMax += g_SocketItemMgr.m_StatusBonus.m_iMagicPowerBonus;

	if (g_isCharacterBuff((&Hero->Object), eBuff_StrengthOfSanta))
	{
		mu_int32 _Temp = 30;

		Character.MagicDamageMin += _Temp;
		Character.MagicDamageMax += _Temp;
	}
}

void CHARACTER_MACHINE::CalculateCurseDamage()
{
	if (GAME::CLASS_SUMMONER != GetBaseClass(Character.Class))
		return;

	mu_uint16 wEnergy = Character.Energy + Character.AddEnergy;
	Character.CurseDamageMin = wEnergy / 9;
	Character.CurseDamageMax = wEnergy / 4;

	ITEM* pEquipWing = &Equipment[GAME::EQUIPMENT_WING];
	if (pEquipWing->Type != -1)
	{
		ITEM_ATTRIBUTE* pAttribute = &ItemAttribute[pEquipWing->Type];
		mu_uint16 wDamageMin = 0;
		mu_uint16 wDamageMax = 0;

		PlusSpecial(&wDamageMin, AT_IMPROVE_CURSE, pEquipWing);
		PlusSpecial(&wDamageMax, AT_IMPROVE_CURSE, pEquipWing);

		mu_float fPercent = CalcDurabilityPercent(pEquipWing->Durability,
			pAttribute->Durability, pEquipWing->Level, 0);
		wDamageMin -= mu_uint16(wDamageMin * fPercent);
		wDamageMax -= mu_uint16(wDamageMax * fPercent);

		Character.CurseDamageMin += wDamageMin;
		Character.CurseDamageMax += wDamageMax;
	}

	ITEM* pEquipLeft = &Equipment[GAME::EQUIPMENT_WEAPON_LEFT];
	if (pEquipLeft->Type != -1 && pEquipLeft->Durability != 0)
	{
		ITEM_ATTRIBUTE* pAttribute = &ItemAttribute[pEquipLeft->Type];
		mu_uint16 wDamageMin = 0;
		mu_uint16 wDamageMax = 0;

		PlusSpecial(&wDamageMin, AT_IMPROVE_CURSE, pEquipLeft);
		PlusSpecial(&wDamageMax, AT_IMPROVE_CURSE, pEquipLeft);

		mu_float fPercent = CalcDurabilityPercent(pEquipLeft->Durability,
			pAttribute->Durability, pEquipLeft->Level, pEquipLeft->ExcOption,
			pEquipLeft->SetOption);

		Character.CurseDamageMin += wDamageMin - mu_uint16(wDamageMin * fPercent);
		Character.CurseDamageMax += wDamageMax - mu_uint16(wDamageMax * fPercent);

		PlusSpecial(&Character.CurseDamageMin, AT_IMPROVE_MAGIC_LEVEL, pEquipLeft);
		PlusSpecial(&Character.CurseDamageMax, AT_IMPROVE_MAGIC_LEVEL, pEquipLeft);
		PlusSpecialPercent(&Character.CurseDamageMin, AT_IMPROVE_MAGIC_PERCENT, pEquipLeft, 2);
		PlusSpecialPercent(&Character.CurseDamageMax, AT_IMPROVE_MAGIC_PERCENT, pEquipLeft, 2);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_StrengthOfSanta))
	{
		mu_int32 _Temp = 30;

		Character.CurseDamageMin += _Temp;
		Character.CurseDamageMax += _Temp;
	}
}

void CHARACTER_MACHINE::CalculateAttackRating()
{
	mu_uint16 Strength, Dexterity, Charisma;

	Strength = Character.Strength + Character.AddStrength;
	Dexterity = Character.Dexterity + Character.AddDexterity;
	Charisma = Character.Charisma + Character.AddCharisma;

	if (GetBaseClass(Character.Class) == GAME::CLASS_DARK_LORD)
		Character.AttackRating = ((Character.Level * 5) + (Dexterity * 5) / 2) + (Strength / 6) + (Charisma / 10);
	else
		Character.AttackRating = ((Character.Level * 5) + (Dexterity * 3) / 2) + (Strength / 4);

	g_csItemOption.PlusSpecial(&Character.AttackRating, AT_SET_OPTION_IMPROVE_ATTACKING_PERCENT);

	Character.AttackRating += g_SocketItemMgr.m_StatusBonus.m_iAttackRateBonus;
}

void CHARACTER_MACHINE::CalculateAttackRatingPK()
{
	mu_uint16 Dexterity;
	Dexterity = Character.Dexterity + Character.AddDexterity;
	mu_int32 CharacterClass = GetBaseClass(Character.Class);

	mu_float tmpf = 0.0f;
	switch (CharacterClass)
	{
	case GAME::CLASS_KNIGHT:
		tmpf = (mu_float)Character.Level * 3 + (mu_float)Dexterity * 4.5f;
		break;
	case GAME::CLASS_DARK_LORD:
		tmpf = (mu_float)Character.Level * 3 + (mu_float)Dexterity * 4.0f;
		break;
	case GAME::CLASS_ELF:
		tmpf = (mu_float)Character.Level * 3 + (mu_float)Dexterity * 0.6f;
		break;
	case GAME::CLASS_DARK:
	case GAME::CLASS_SUMMONER:
		tmpf = (mu_float)Character.Level * 3 + (mu_float)Dexterity * 3.5f;
		break;
	case GAME::CLASS_WIZARD:
		tmpf = (mu_float)Character.Level * 3 + (mu_float)Dexterity * 4.0f;
		break;
	}

	Character.AttackRatingPK = (mu_uint16)tmpf;
}

void CHARACTER_MACHINE::CalculateAttackSpeed()
{
	mu_uint16 Dexterity;

	Dexterity = Character.Dexterity + Character.AddDexterity;

	mu_int32 CharacterClass = GetBaseClass(Character.Class);

	if (CharacterClass == GAME::CLASS_ELF)
	{
		Character.AttackSpeed = Dexterity / 50;
		Character.MagicSpeed = Dexterity / 50;
	}
	else if (CharacterClass == GAME::CLASS_KNIGHT || CharacterClass == GAME::CLASS_DARK)
	{
		Character.AttackSpeed = Dexterity / 15;
		Character.MagicSpeed = Dexterity / 20;
	}

	else if (CharacterClass == GAME::CLASS_DARK_LORD)
	{
		Character.AttackSpeed = Dexterity / 10;
		Character.MagicSpeed = Dexterity / 10;
	}
	else if (CharacterClass == GAME::CLASS_SUMMONER)
	{
		Character.AttackSpeed = Dexterity / 20;
		Character.MagicSpeed = Dexterity / 20;
	}
	else
	{
		Character.AttackSpeed = Dexterity / 20;
		Character.MagicSpeed = Dexterity / 10;
	}

	ITEM *r = &Equipment[GAME::EQUIPMENT_WEAPON_RIGHT];
	ITEM *l = &Equipment[GAME::EQUIPMENT_WEAPON_LEFT];
	ITEM *g = &Equipment[GAME::EQUIPMENT_GLOVES];
	ITEM *rl = &Equipment[GAME::EQUIPMENT_RING_LEFT];
	ITEM *rr = &Equipment[GAME::EQUIPMENT_RING_RIGHT];
	ITEM *phelper = &Equipment[GAME::EQUIPMENT_HELPER];

	mu_boolean Right = false;
	mu_boolean Left = false;
	mu_boolean Glove = false;
	mu_boolean RingRight = false;
	mu_boolean RingLeft = false;
	mu_boolean Helper = false;
	if ((r->Type != ITEM_BOW + 7 && r->Type != ITEM_BOW + 15) && r->Type >= ITEM_SWORD && r->Type < ITEM_SHIELD)
	{
		if (IsRequireEquipItem(r) && r->Durability != 0)
		{
			Right = true;
		}
	}

	if ((l->Type != ITEM_BOW + 7 && l->Type != ITEM_BOW + 15) && l->Type >= ITEM_SWORD && l->Type < ITEM_SHIELD)
	{
		if (IsRequireEquipItem(l) && l->Durability != 0)
		{
			Left = true;
		}
	}

	if (g->Durability != 0 && g->Type != -1)
	{
		if (IsRequireEquipItem(g))
		{
			Glove = true;
		}
	}

	if (rl->Durability != 0 &&
		(rl->Type == ITEM_HELPER + 20
			|| rl->Type == ITEM_HELPER + 107))
	{
		if (IsRequireEquipItem(rl))
		{
			switch (rl->Level)
			{
			case 0:
			case 3:
				RingRight = true;
				break;
			}
		}
	}

	if (rr->Durability != 0 && (rr->Type == ITEM_HELPER + 20
		|| rr->Type == ITEM_HELPER + 107
		))
	{
		if (IsRequireEquipItem(rr))
		{
			switch (rr->Level)
			{
			case 0:
			case 3:
				RingLeft = true;
				break;
			}
		}
	}

	if (phelper->Durability != 0 &&
		(phelper->Type == ITEM_HELPER + 64
			|| phelper->Type == ITEM_HELPER + 123)
		)
	{
		if (IsRequireEquipItem(phelper))
		{
			Helper = true;
		}
	}

	mu_int32 RightWeaponSpeed = r->WeaponSpeed;
	mu_int32 LeftWeaponSpeed = l->WeaponSpeed;

	if (r->Durability <= 0) RightWeaponSpeed = 0;
	if (l->Durability <= 0) LeftWeaponSpeed = 0;

	if (Right && Left)
	{
		Character.AttackSpeed += (RightWeaponSpeed + LeftWeaponSpeed) / 2;
		Character.MagicSpeed += (RightWeaponSpeed + LeftWeaponSpeed) / 2;
	}
	else if (Right)
	{
		Character.AttackSpeed += RightWeaponSpeed;
		Character.MagicSpeed += RightWeaponSpeed;
	}
	else if (Left)
	{
		Character.AttackSpeed += LeftWeaponSpeed;
		Character.MagicSpeed += LeftWeaponSpeed;
	}

	if (RingRight)
	{
		Character.AttackSpeed += 10;
		Character.MagicSpeed += 10;
		CharacterAttribute->Ability |= ABILITY_FAST_ATTACK_RING;
	}
	else if (RingLeft)
	{
		Character.AttackSpeed += 10;
		Character.MagicSpeed += 10;
		CharacterAttribute->Ability |= ABILITY_FAST_ATTACK_RING;
	}
	else if (CharacterAttribute->Ability & ABILITY_FAST_ATTACK_RING)
	{
		CharacterAttribute->Ability &= (~ABILITY_FAST_ATTACK_RING);
	}

	if (Glove)
	{
		Character.AttackSpeed += g->WeaponSpeed;
		Character.MagicSpeed += g->WeaponSpeed;
	}

	if (Helper)
	{
		const ITEM_ADD_OPTION& Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(phelper->Type);
		Character.AttackSpeed += Item_data.m_byValue2;
		Character.MagicSpeed += Item_data.m_byValue2;
	}

	if (Character.Ability & ABILITY_FAST_ATTACK_SPEED)
	{
		Character.AttackSpeed += 20;
		Character.MagicSpeed += 20;
	}
	else if (Character.Ability&ABILITY_FAST_ATTACK_SPEED2)
	{
		Character.AttackSpeed += 20;
		Character.MagicSpeed += 20;
	}

	ITEM *Amulet = &Equipment[GAME::EQUIPMENT_AMULET];

	if (Amulet->Type == ITEM_HELPER + 114)
	{
		mu_int32 _Temp = 7;

		Character.AttackSpeed += _Temp;
		Character.MagicSpeed += _Temp;
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll1))
	{
		const ITEM_ADD_OPTION& Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 72);
		Character.AttackSpeed += Item_data.m_byValue1;
		Character.MagicSpeed += Item_data.m_byValue1;
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin1))
	{
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 46);
		Character.AttackSpeed += Item_data.m_byValue1;
		Character.MagicSpeed += Item_data.m_byValue1;
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_QuickOfSanta))
	{
		mu_int32 _Temp = 15;
		Character.AttackSpeed += _Temp;
		Character.MagicSpeed += _Temp;
	}

	PlusSpecial(&Character.AttackSpeed, AT_IMPROVE_ATTACK_SPEED, &Equipment[GAME::EQUIPMENT_WEAPON_RIGHT]);
	PlusSpecial(&Character.MagicSpeed, AT_IMPROVE_ATTACK_SPEED, &Equipment[GAME::EQUIPMENT_WEAPON_RIGHT]);
	PlusSpecial(&Character.AttackSpeed, AT_IMPROVE_ATTACK_SPEED, &Equipment[GAME::EQUIPMENT_WEAPON_LEFT]);
	PlusSpecial(&Character.MagicSpeed, AT_IMPROVE_ATTACK_SPEED, &Equipment[GAME::EQUIPMENT_WEAPON_LEFT]);
	PlusSpecial(&Character.AttackSpeed, AT_IMPROVE_ATTACK_SPEED, &Equipment[GAME::EQUIPMENT_AMULET]);
	PlusSpecial(&Character.MagicSpeed, AT_IMPROVE_ATTACK_SPEED, &Equipment[GAME::EQUIPMENT_AMULET]);
	PlusSpecial(&Character.AttackSpeed, AT_IMPROVE_ATTACK_SPEED, &Equipment[GAME::EQUIPMENT_HELPER]);
	PlusSpecial(&Character.MagicSpeed, AT_IMPROVE_ATTACK_SPEED, &Equipment[GAME::EQUIPMENT_HELPER]);

	Character.AttackSpeed += g_SocketItemMgr.m_StatusBonus.m_iAttackSpeedBonus;
	Character.MagicSpeed += g_SocketItemMgr.m_StatusBonus.m_iAttackSpeedBonus;
}

void CHARACTER_MACHINE::CalculateSuccessfulBlockingPK()
{
	mu_uint16 Dexterity;
	Dexterity = Character.Dexterity + Character.AddDexterity;
	mu_int32 CharacterClass = GetBaseClass(Character.Class);

	mu_float tmpf = 0.0f;
	switch (CharacterClass)
	{
	case GAME::CLASS_KNIGHT:
		tmpf = (mu_float)Character.Level * 2 + (mu_float)Dexterity * 0.5f;
		break;
	case GAME::CLASS_DARK_LORD:
		tmpf = (mu_float)Character.Level * 2 + (mu_float)Dexterity * 0.5f;
		break;
	case GAME::CLASS_ELF:
		tmpf = (mu_float)Character.Level * 2 + (mu_float)Dexterity * 0.1f;
		break;
	case GAME::CLASS_DARK:
		tmpf = (mu_float)Character.Level * 2 + (mu_float)Dexterity * 0.25f;
		break;
	case GAME::CLASS_WIZARD:
		tmpf = (mu_float)Character.Level * 2 + (mu_float)Dexterity * 0.25f;
		break;
	case GAME::CLASS_SUMMONER:
		tmpf = (mu_float)Character.Level * 2 + (mu_float)Dexterity * 0.5f;
		break;
	}
	Character.SuccessfulBlockingPK = (mu_uint16)tmpf;
}

void CHARACTER_MACHINE::CalculateSuccessfulBlocking()
{
	mu_uint16 Dexterity;

	Dexterity = Character.Dexterity + Character.AddDexterity;

	mu_int32 CharacterClass = GetBaseClass(Character.Class);

	if (CharacterClass == GAME::CLASS_ELF || CharacterClass == GAME::CLASS_SUMMONER)
	{
		Character.SuccessfulBlocking = Dexterity / 4;
	}
	else if (CharacterClass == GAME::CLASS_DARK_LORD)
	{
		Character.SuccessfulBlocking = Dexterity / 7;
	}
	else
	{
		Character.SuccessfulBlocking = Dexterity / 3;
	}

	ITEM* Left = &Equipment[GAME::EQUIPMENT_WEAPON_LEFT];
	if (Left->Type != -1 && Left->Durability != 0)
	{
		ITEM_ATTRIBUTE* p = &ItemAttribute[Left->Type];
		mu_float percent = CalcDurabilityPercent(Left->Durability, p->Durability, Left->Level, Left->ExcOption, Left->SetOption);

		mu_uint16 SuccessBlocking = 0;
		SuccessBlocking = Left->SuccessfulBlocking - (mu_uint16)(Left->SuccessfulBlocking*percent);

		Character.SuccessfulBlocking += SuccessBlocking;

		SuccessBlocking = 0;
		PlusSpecial(&SuccessBlocking, AT_IMPROVE_BLOCKING, Left);

		Character.SuccessfulBlocking += SuccessBlocking - (mu_uint16)(SuccessBlocking*percent);

		PlusSpecialPercent(&Character.SuccessfulBlocking, AT_IMPROVE_BLOCKING_PERCENT, Left, 10);
	}

	PlusSpecialPercent(&Character.SuccessfulBlocking, AT_IMPROVE_BLOCKING_PERCENT, &Equipment[GAME::EQUIPMENT_HELM], 10);
	PlusSpecialPercent(&Character.SuccessfulBlocking, AT_IMPROVE_BLOCKING_PERCENT, &Equipment[GAME::EQUIPMENT_ARMOR], 10);
	PlusSpecialPercent(&Character.SuccessfulBlocking, AT_IMPROVE_BLOCKING_PERCENT, &Equipment[GAME::EQUIPMENT_PANTS], 10);
	PlusSpecialPercent(&Character.SuccessfulBlocking, AT_IMPROVE_BLOCKING_PERCENT, &Equipment[GAME::EQUIPMENT_GLOVES], 10);
	PlusSpecialPercent(&Character.SuccessfulBlocking, AT_IMPROVE_BLOCKING_PERCENT, &Equipment[GAME::EQUIPMENT_BOOTS], 10);
	PlusSpecialPercent(&Character.SuccessfulBlocking, AT_IMPROVE_BLOCKING_PERCENT, &Equipment[GAME::EQUIPMENT_RING_LEFT], 10);
	PlusSpecialPercent(&Character.SuccessfulBlocking, AT_IMPROVE_BLOCKING_PERCENT, &Equipment[GAME::EQUIPMENT_RING_RIGHT], 10);
	
	Character.SuccessfulBlocking *= g_SocketItemMgr.m_StatusBonus.m_fDefenceRateBonus;
}

void CHARACTER_MACHINE::CalculateDefense()
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[0];

	mu_uint16 Dexterity, Vitality;

	Dexterity = Character.Dexterity + Character.AddDexterity;
	Vitality = Character.Vitality + Character.AddVitality;

	mu_int32 CharacterClass = GetBaseClass(Character.Class);

	if (CharacterClass == GAME::CLASS_ELF)
	{
		Character.Defense = Dexterity / 10;
	}
	else if (CharacterClass == GAME::CLASS_KNIGHT || CharacterClass == GAME::CLASS_SUMMONER)
	{
		Character.Defense = Dexterity / 3;
	}
	else if (CharacterClass == GAME::CLASS_WIZARD)
	{
		Character.Defense = Dexterity / 4;
	}
	else if (CharacterClass == GAME::CLASS_DARK_LORD)
	{
		Character.Defense = Dexterity / 7;
	}
	else
	{
		Character.Defense = Dexterity / 5;
	}

	mu_uint16    Defense = 0;
	for (mu_int32 i = GAME::EQUIPMENT_WEAPON_LEFT; i <= GAME::EQUIPMENT_WING; ++i)
	{
		if (Equipment[i].Durability != 0)
		{
			mu_uint16 defense = ItemDefense(&Equipment[i]);

			ITEM_ATTRIBUTE *p = &ItemAttribute[Equipment[i].Type];
			mu_float percent;
			if (i == GAME::EQUIPMENT_WING)
			{
				percent = CalcDurabilityPercent(Equipment[i].Durability, p->Durability, Equipment[i].Level, 0);
			}
			else
			{
				percent = CalcDurabilityPercent(Equipment[i].Durability, p->Durability, Equipment[i].Level, Equipment[i].ExcOption, Equipment[i].SetOption);
			}

			defense -= (mu_uint16)(defense*percent);

			Defense += defense;
		}
	}
	Character.Defense += Defense;

	if (g_AddDefense)
	{
		mu_float addDefense = 0.0f;
		switch (ThreadInfo.EquipmentLevelSet)
		{
		case 10:
			addDefense = 0.05f;
			break;
		case 11:
			addDefense = 0.1f;
			break;
		case 12:
			addDefense = 0.15f;
			break;
		case 13:
			addDefense = 0.2f;
			break;
		case 14:
			addDefense = 0.25f;
			break;
		case 15:
			addDefense = 0.3f;
			break;
		}
		Character.Defense += (mu_uint16)(Character.Defense*addDefense);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll2))
	{
		const ITEM_ADD_OPTION& Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 73);
		Character.Defense += (mu_uint16)Item_data.m_byValue1;
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin3))
	{
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 48);
		Character.Defense += (mu_uint16)Item_data.m_byValue1;
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_BlessingOfXmax))
	{
		mu_int32 _Temp = 0;
		_Temp = Character.Level / 5 + 50;

		Character.Defense += _Temp;
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_DefenseOfSanta))
	{
		mu_int32 _Temp = 100;
		Character.Defense += _Temp;
	}

	g_csItemOption.PlusSpecial(&Character.Defense, AT_SET_OPTION_IMPROVE_DEFENCE);

	g_csItemOption.PlusSpecialLevel(&Character.Defense, Dexterity, AT_SET_OPTION_IMPROVE_DEFENCE_3);
	g_csItemOption.PlusSpecialLevel(&Character.Defense, Vitality, AT_SET_OPTION_IMPROVE_DEFENCE_4);

	if (Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Type >= ITEM_SHIELD && Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Type < ITEM_HELM)
	{
		g_csItemOption.PlusSpecialPercent(&Character.Defense, AT_SET_OPTION_IMPROVE_SHIELD_DEFENCE);
	}

	PlusSpecial(&Character.Defense, AT_SET_OPTION_IMPROVE_DEFENCE, &Equipment[GAME::EQUIPMENT_HELPER]);
	if (Equipment[GAME::EQUIPMENT_WING].Type == ITEM_HELPER + 30)
	{
		PlusSpecial(&Character.Defense, AT_SET_OPTION_IMPROVE_DEFENCE, &Equipment[GAME::EQUIPMENT_WING]);
	}

	Character.Defense += g_SocketItemMgr.m_StatusBonus.m_iDefenceBonus;

	if (Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Type != -1 && Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Durability != 0)
	{
		Character.Defense += Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Defense * g_SocketItemMgr.m_StatusBonus.m_iShieldDefenceBonus * 0.01f;
	}
}

void CHARACTER_MACHINE::CalculateMagicDefense()
{
	for (mu_int32 i = GAME::EQUIPMENT_HELM; i <= GAME::EQUIPMENT_WING; ++i)
	{
		if (Equipment[i].Durability != 0)
		{
			Character.MagicDefense = ItemMagicDefense(&Equipment[i]);
		}
	}
}

void CHARACTER_MACHINE::CalculateWalkSpeed()
{
	if (Equipment[GAME::EQUIPMENT_BOOTS].Durability != 0)
	{
		Character.WalkSpeed = ItemWalkSpeed(&Equipment[GAME::EQUIPMENT_BOOTS]);
	}

	if (Equipment[GAME::EQUIPMENT_WING].Durability != 0)
	{
		Character.WalkSpeed += ItemWalkSpeed(&Equipment[GAME::EQUIPMENT_WING]);
	}
}

void CHARACTER_MACHINE::CalculateNextExperince()
{
	Character.Experience = Character.NextExperince;

	const mu_int64 characterLevel = static_cast<mu_int64>(Character.Level);
	Character.NextExperince = (9 + characterLevel)*(characterLevel)*(characterLevel) * 10;

	if (characterLevel > 255)
	{
		const mu_int64 LevelOver_N = characterLevel - 255;
		Character.NextExperince += (9 + LevelOver_N)*LevelOver_N*LevelOver_N * 1000;
	}
}

void CHARACTER_MACHINE::CalulateMasterLevelNextExperience()
{
	Master_Level_Data.lMasterLevel_Experince = Master_Level_Data.lNext_MasterLevel_Experince;

	const mu_int64 level = static_cast<mu_int64>(CharacterAttribute->Level);
	const mu_int64 masterLevel = static_cast<mu_int64>(Master_Level_Data.nMLevel);

	const mu_int64 iTotalLevel = level + masterLevel + 1;
	const mu_int64 iTOverLevel = iTotalLevel - 255;

	const mu_int64 iData_Master =
		(
		(
			(mu_int64)9 + (mu_int64)iTotalLevel
			)
			* (mu_int64)iTotalLevel
			* (mu_int64)iTotalLevel
			* (mu_int64)10
			)
		+
		(
		(
			(mu_int64)9 + (mu_int64)iTOverLevel
			)
			* (mu_int64)iTOverLevel
			* (mu_int64)iTOverLevel
			* (mu_int64)1000
			);

	Master_Level_Data.lNext_MasterLevel_Experince = (iData_Master - 3892250000ll) / 2ll;
}

mu_boolean CHARACTER_MACHINE::IsZeroDurability()
{
	for (mu_int32 i = GAME::EQUIPMENT_WEAPON_RIGHT; i < GAME::MAX_EQUIPMENT; ++i)
	{
		if (Equipment[i].Durability == 0)
		{
			return  true;
		}
	}

	return  false;
}

void CHARACTER_MACHINE::CalculateBasicState()
{
	if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion1))
	{
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 78);
		Character.AddStrength += (mu_uint16)Item_data.m_byValue1;
	}
	else if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion2))
	{
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 79);
		Character.AddDexterity += (mu_uint16)Item_data.m_byValue1;
	}
	else if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion3))
	{
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 80);
		Character.AddVitality += (mu_uint16)Item_data.m_byValue1;
	}
	else if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion4))
	{
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 81);
		Character.AddEnergy += (mu_uint16)Item_data.m_byValue1;
	}
	else if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion5))
	{
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 82);
		Character.AddCharisma += (mu_uint16)Item_data.m_byValue1;
	}
}

void CHARACTER_MACHINE::getAllAddStateOnlyExValues(mu_int32 &iAddStrengthExValues,
	mu_int32 &iAddDexterityExValues,
	mu_int32 &iAddVitalityExValues,
	mu_int32 &iAddEnergyExValues,
	mu_int32 &iAddCharismaExValues)
{
	if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion1))
	{
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 78);
		iAddStrengthExValues += (mu_uint16)Item_data.m_byValue1;
	}
	else if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion2))
	{
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 79);
		iAddDexterityExValues += (mu_uint16)Item_data.m_byValue1;
	}
	else if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion3))
	{
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 80);
		iAddVitalityExValues += (mu_uint16)Item_data.m_byValue1;
	}
	else if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion4))
	{
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 81);
		iAddEnergyExValues += (mu_uint16)Item_data.m_byValue1;
	}
	else if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion5))
	{
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEM_POTION + 82);
		iAddCharismaExValues += (mu_uint16)Item_data.m_byValue1;
	}

	iAddStrengthExValues += g_SocketItemMgr.m_StatusBonus.m_iStrengthBonus;
	iAddDexterityExValues += g_SocketItemMgr.m_StatusBonus.m_iDexterityBonus;
	iAddVitalityExValues += g_SocketItemMgr.m_StatusBonus.m_iVitalityBonus;
	iAddEnergyExValues += g_SocketItemMgr.m_StatusBonus.m_iEnergyBonus;
}

void CHARACTER_MACHINE::CalculateAll()
{
	CalculateBasicState();

	g_csItemOption.CheckItemSetOptions();
	g_csItemOption.ClearListOnOff();
	InitAddValue();

	g_SocketItemMgr.CheckSocketSetOption();
	g_SocketItemMgr.CalcSocketStatusBonus();
	CharacterMachine->Character.AddStrength += g_SocketItemMgr.m_StatusBonus.m_iStrengthBonus;
	CharacterMachine->Character.AddDexterity += g_SocketItemMgr.m_StatusBonus.m_iDexterityBonus;
	CharacterMachine->Character.AddVitality += g_SocketItemMgr.m_StatusBonus.m_iVitalityBonus;
	CharacterMachine->Character.AddEnergy += g_SocketItemMgr.m_StatusBonus.m_iEnergyBonus;

	CalculateBasicState();

	mu_uint16	wAddStrength, wAddDexterity, wAddEnergy, wAddVitality,
		wStrengthResult, wDexterityResult, wEnergyResult, wVitalityResult;
	wAddStrength = wAddDexterity = wAddEnergy = wAddVitality = 0;
	wStrengthResult = wDexterityResult = wEnergyResult = wVitalityResult = 0;

	mu_uint16 wAddCharisma, wCharismaResult;
	wAddCharisma = wCharismaResult = 0;

	g_csItemOption.getAllAddStateOnlyAddValue(&wAddStrength, &wAddDexterity, &wAddEnergy, &wAddVitality, &wAddCharisma);

	mu_int32 iAddStrengthByExValues, iAddDexterityByExValues,
		iAddEnergyByExValues, iAddVitalityByExValues,
		iAddCharismaExValues;

	iAddStrengthByExValues = iAddDexterityByExValues
		= iAddEnergyByExValues = iAddVitalityByExValues = iAddCharismaExValues = 0;

	getAllAddStateOnlyExValues(iAddStrengthByExValues,
		iAddDexterityByExValues,
		iAddVitalityByExValues,
		iAddEnergyByExValues,
		iAddCharismaExValues);

	wAddStrength += iAddStrengthByExValues;
	wAddDexterity += iAddDexterityByExValues;
	wAddEnergy += iAddEnergyByExValues;
	wAddVitality += iAddVitalityByExValues;

	wStrengthResult = CharacterMachine->Character.Strength + wAddStrength;
	wDexterityResult = CharacterMachine->Character.Dexterity + wAddDexterity;
	wEnergyResult = CharacterMachine->Character.Energy + wAddEnergy;
	wVitalityResult = CharacterMachine->Character.Vitality + wAddVitality;

	wCharismaResult = CharacterMachine->Character.Charisma + wAddCharisma;

	g_csItemOption.getAllAddOptionStatesbyCompare(&Character.AddStrength,
		&Character.AddDexterity, &Character.AddEnergy, &Character.AddVitality, &Character.AddCharisma,
		wStrengthResult, wDexterityResult, wEnergyResult, wVitalityResult, wCharismaResult);

	g_csItemOption.CheckItemSetOptions();

	if ((CharacterMachine->Equipment[GAME::EQUIPMENT_WING].Type + MODEL_ITEM) == MODEL_HELPER + 30)
	{
		PlusSpecial(&Character.AddCharisma, AT_SET_OPTION_IMPROVE_CHARISMA, &CharacterMachine->Equipment[GAME::EQUIPMENT_WING]);
	}

	CalculateBasicState();

	CalculateDamage();
	CalculateMagicDamage();
	CalculateCurseDamage();
	CalculateAttackRating();
	CalculateAttackSpeed();
	CalculateSuccessfulBlocking();
	CalculateDefense();
	CalculateMagicDefense();
	CalculateWalkSpeed();

	CalculateSuccessfulBlockingPK();
	CalculateAttackRatingPK();

	MONSTER_ATTRIBUTE *c = &Enemy.Attribute;
	FinalAttackDamageRight = AttackDamageRight - c->Defense;
	FinalAttackDamageLeft = AttackDamageLeft - c->Defense;
	mu_int32 EnemyAttackDamage = c->AttackDamageMin + GetLargeRand(c->AttackDamageMax - c->AttackDamageMin + 1);
	FinalHitPoint = EnemyAttackDamage - Character.Defense;
	FinalAttackRating = Character.AttackRating - c->SuccessfulBlocking;
	FinalDefenseRating = Character.SuccessfulBlocking - c->AttackRating;

	if (FinalAttackRating > 100)
		FinalAttackRating = 100;
	if (FinalDefenseRating > 100)
		FinalDefenseRating = 100;
}

void CHARACTER_MACHINE::GetMagicSkillDamage(mu_int32 iType, mu_int32 *piMinDamage, mu_int32 *piMaxDamage)
{
	if (AT_SKILL_SUMMON_EXPLOSION <= iType && iType <= AT_SKILL_SUMMON_REQUIEM)
	{
		*piMinDamage = Character.MagicDamageMin;
		*piMaxDamage = Character.MagicDamageMax;
		return;
	}

	SKILL_ATTRIBUTE *p = &SkillAttribute[iType];

	mu_int32 Damage = p->Damage;

	*piMinDamage = Character.MagicDamageMin + Damage;
	*piMaxDamage = Character.MagicDamageMax + Damage + Damage / 2;

	Damage = 0;
	g_csItemOption.PlusSpecial((mu_uint16*)&Damage, AT_SET_OPTION_IMPROVE_MAGIC_POWER);
	if (Damage != 0)
	{
		mu_float fratio = 1.0f + (mu_float)Damage / 100.0f;
		*piMinDamage *= fratio;
		*piMaxDamage *= fratio;
	}

	Damage = 0;
	g_csItemOption.ClearListOnOff();
	g_csItemOption.PlusMastery(&Damage, p->MasteryType);
	g_csItemOption.PlusSpecial((mu_uint16*)&Damage, AT_SET_OPTION_IMPROVE_SKILL_ATTACK);
	*piMinDamage += Damage;
	*piMaxDamage += Damage;
}

void CHARACTER_MACHINE::GetCurseSkillDamage(mu_int32 iType, mu_int32 *piMinDamage, mu_int32 *piMaxDamage)
{
	if (GAME::CLASS_SUMMONER != GetBaseClass(Character.Class))
		return;

	if (AT_SKILL_SUMMON_EXPLOSION <= iType && iType <= AT_SKILL_SUMMON_REQUIEM)
	{
		SKILL_ATTRIBUTE *p = &SkillAttribute[iType];
		*piMinDamage = Character.CurseDamageMin + p->Damage;
		*piMaxDamage = Character.CurseDamageMax + p->Damage + p->Damage / 2;
	}
	else
	{
		*piMinDamage = Character.CurseDamageMin;
		*piMaxDamage = Character.CurseDamageMax;
	}
}

void CHARACTER_MACHINE::GetSkillDamage(mu_int32 iType, mu_int32 *piMinDamage, mu_int32 *piMaxDamage)
{
	SKILL_ATTRIBUTE *p = &SkillAttribute[iType];

	mu_int32 Damage = p->Damage;

	*piMinDamage = Damage;
	*piMaxDamage = Damage + Damage / 2;

	Damage = 0;
	g_csItemOption.ClearListOnOff();
	g_csItemOption.PlusMastery(&Damage, p->MasteryType);
	g_csItemOption.PlusSpecial((mu_uint16*)&Damage, AT_SET_OPTION_IMPROVE_SKILL_ATTACK);
	*piMinDamage += Damage;
	*piMaxDamage += Damage;
}

mu_uint8 CHARACTER_MACHINE::GetSkillMasteryType(mu_int32 iType)
{
	return SkillAttribute[iType].MasteryType;
}