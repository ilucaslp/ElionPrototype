#include "stdafx.h"
#include "mu_itemattribute.h"

ITEM_ATTRIBUTE ItemAttribute[MAX_ITEM];

mu_int32 ChaosEventName[] = {
	14000,
	14001,
	14002,
	14003,
	14004,
	14005,
	14006,
	14007,
	14008,
	14009,
};

mu_int32 SommonTable[] = { 2,7,14,8,9,41 };

const mu_int32 g_iChaosCastleLevel[12][2] = {
	{ 15, 49 },{ 50, 119 },{ 120, 179 },{ 180, 239 },{ 240, 299 },{ 300, 999 },
	{ 15, 29 },{ 30,  99 },{ 100, 159 },{ 160, 219 },{ 220, 279 },{ 280, 999 }
};
const mu_int32 g_iChaosCastleZen[6] = { 25, 80, 150, 250, 400, 650 };

mu_boolean OpenItemScript(const mu_text *filename)
{
	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, filename, "rb") == false)
	{
		return false;
	}

	static const mu_size HeaderSize = FileHeaders::HEADER_SIZE;
	static const mu_size CompressedHeaderSize = FileHeaders::COMPRESSED_HEADER_SIZE;

	mu_int64 fileLength = SDL_RWsize(file);
	mu_int64 encryptedLength = fileLength - HeaderSize;

	if (encryptedLength % MUBlowfish::BytesLength != 0)
	{
		return false;
	}

	mu_uint8 *dataBuffer = new mu_uint8[fileLength];
	SDL_RWread(file, dataBuffer, fileLength, 1);
	SDL_RWclose(file);

	FileHeaders::FileHeader *header = (FileHeaders::FileHeader*)dataBuffer;

	if (header->Signature != FILE_HEADERS::DATA_HEADER)
	{
		delete[] dataBuffer;
		return false;
	}

	if (GenerateCRC32(dataBuffer + HeaderSize, encryptedLength) != header->CRC)
	{
		delete[] dataBuffer;
		return false;
	}

	g_DataCrypt.Decode(dataBuffer + HeaderSize, dataBuffer + HeaderSize, encryptedLength);

	FileHeaders::CompressedHeader *compressedHeader = (FileHeaders::CompressedHeader*)(dataBuffer + HeaderSize);

	mu_uint8 *decompressedBuffer = new mu_uint8[compressedHeader->DecompressedSize];

	mu_boolean decompressed = FileHeaders::Decompress(dataBuffer + HeaderSize + CompressedHeaderSize,
		decompressedBuffer,
		compressedHeader->CompressedSize,
		compressedHeader->DecompressedSize);
	delete[] dataBuffer;

	if (decompressed == false)
	{
		delete[] decompressedBuffer;
		return false;
	}

	mu_uint8 *readBuffer = decompressedBuffer;
	mu_asciistr textData;

	for (mu_uint32 i = 0; i < MAX_ITEM; ++i)
	{
		ITEM_ATTRIBUTE &item = ItemAttribute[i];

		mu_uint32 textLength = 0;
		FileHeaders::ReadBuffer(readBuffer, textLength);
		textData.resize(textLength);
		FileHeaders::ReadBuffer(readBuffer, (mu_char*)textData.data(), textLength);
#ifdef UNICODE
		item.Name = boost::locale::conv::utf_to_utf<mu_text>(textData);
#else
		item.Name = textData;
#endif
		FileHeaders::ReadBuffer(readBuffer, *static_cast<ITEM_ATTRIBUTE_BASE*>(&item));
	}

	delete[] decompressedBuffer;

	return true;
}

mu_float CalcDurabilityPercent(mu_uint8 dur, mu_uint8 maxDur, mu_int32 Level, mu_int32 ExcOption, mu_int32 SetOption)
{
	mu_int32 maxDurability = maxDur;
	mu_int32 itemLevel = Level;
	for (mu_int32 i = 0; i < itemLevel; ++i)
	{
		if (i >= 4)
		{
			maxDurability += 2;
		}
		else
		{
			++maxDurability;
		}
	}

	if ((SetOption % 0x04) == EXT_A_SET_OPTION || (SetOption % 0x04) == EXT_B_SET_OPTION)
	{
		maxDurability += 20;
	}
	else if (ExcOption > 0)
	{
		maxDurability += 15;
	}

	mu_float durP = 1.0f - (dur / (mu_float)maxDurability);

	if (durP > 0.8f)
	{
		return  0.5f;
	}
	else if (durP > 0.7f)
	{
		return  0.3f;
	}
	else if (durP > 0.5f)
	{
		return  0.2f;
	}

	return 0.0f;
}

mu_boolean IsRequireEquipItem(ITEM* pItem)
{
	if (pItem == nullptr)
	{
		return false;
	}

	ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[pItem->Type];

	mu_boolean bEquipable = false;

	if (pItemAttr->RequireClass[GetBaseClass(Hero->Class)])
	{
		bEquipable = true;
	}
	else if (GetBaseClass(Hero->Class) == GAME::CLASS_DARK &&
		pItemAttr->RequireClass[GAME::CLASS_WIZARD] &&
		pItemAttr->RequireClass[GAME::CLASS_KNIGHT])
	{
		bEquipable = true;
	}

	mu_uint8 byFirstClass = GetBaseClass(Hero->Class);
	mu_uint8 byStepClass = GetStepClass(Hero->Class);
	if (pItemAttr->RequireClass[byFirstClass] > byStepClass)
	{
		return false;
	}

	if (bEquipable == false)
		return false;

	mu_uint16 wStrength = CharacterAttribute->Strength + CharacterAttribute->AddStrength;
	mu_uint16 wDexterity = CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;
	mu_uint16 wEnergy = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;
	mu_uint16 wVitality = CharacterAttribute->Vitality + CharacterAttribute->AddVitality;
	mu_uint16 wCharisma = CharacterAttribute->Charisma + CharacterAttribute->AddCharisma;
	mu_uint16 wLevel = CharacterAttribute->Level;

	mu_int32 iItemLevel = pItem->Level;

	mu_int32 iDecNeedStrength = 0, iDecNeedDex = 0;

	if (iItemLevel >= pItem->HarmonyOptionLevel)
	{
		StrengthenCapability SC;
		g_pUIJewelHarmonyinfo.GetStrengthenCapability(&SC, pItem, 0);

		if (SC.SI_isNB)
		{
			iDecNeedStrength = SC.SI_NB.SI_force;
			iDecNeedDex = SC.SI_NB.SI_activity;
		}
	}

	if (pItem->RequireStrength - iDecNeedStrength > wStrength)
		return false;
	if (pItem->RequireDexterity - iDecNeedDex > wDexterity)
		return false;
	if (pItem->RequireEnergy > wEnergy)
		return false;
	if (pItem->RequireVitality > wVitality)
		return false;
	if (pItem->RequireCharisma > wCharisma)
		return false;
	if (pItem->RequireLevel > wLevel)
		return false;

	if (pItem->Type == ITEM_HELPER + 5)   
	{
		mu_int16 wRequireCharisma = (185 + (pItem->PetLevel * 15));
		if (wRequireCharisma > wCharisma)
		{
			return false;
		}
	}

	return bEquipable;
}

void PlusSpecial(mu_uint16 *Value, mu_int32 Special, ITEM *Item)
{
	if (Item->Type == -1) return;

	if (IsRequireEquipItem(Item))
	{
		for (mu_int32 i = 0; i < Item->SpecialNum; ++i)
		{
			if (Item->Special[i] == Special && Item->Durability != 0)
				*Value += Item->SpecialValue[i];
		}
	}
}

void PlusSpecialPercent(mu_uint16 *Value, mu_int32 Special, ITEM *Item, mu_uint16 Percent)
{
	if (Item->Type == -1) return;

	if (IsRequireEquipItem(Item))
	{
		for (mu_int32 i = 0; i < Item->SpecialNum; ++i)
		{
			if (Item->Special[i] == Special && Item->Durability != 0)
				*Value += *Value*Percent / 100;
		}
	}
}

void PlusSpecialPercent2(mu_uint16 *Value, mu_int32 Special, ITEM *Item)
{
	if (Item->Type == -1) return;

	if (IsRequireEquipItem(Item))
	{
		for (mu_int32 i = 0; i < Item->SpecialNum; ++i)
		{
			if (Item->Special[i] == Special && Item->Durability != 0)
				*Value += (mu_uint16)(*Value*(Item->SpecialValue[i] / 100.0f));
		}
	}
}

mu_uint16 ItemDefense(ITEM *Item)
{
	if (Item->Type == -1) return 0;
	mu_uint16 Defense = Item->Defense;
	PlusSpecial(&Defense, AT_IMPROVE_DEFENSE, Item);
	return Defense;
}

mu_uint16 ItemMagicDefense(ITEM *Item)
{
	if (Item->Type == -1) return 0;
	mu_uint16 MagicDefense = Item->MagicDefense;
	return MagicDefense;
}

mu_uint16 ItemWalkSpeed(ITEM *Item)
{
	if (Item->Type == -1) return 0;
	mu_uint16 WalkSpeed = Item->WalkSpeed;
	return WalkSpeed;
}

mu_uint16 calcMaxDurability(const ITEM* ip, ITEM_ATTRIBUTE *p, mu_int32 Level)
{
	mu_uint16 maxDurability = p->Durability;

	if (ip->Type >= ITEM_STAFF && ip->Type < ITEM_SHIELD)
	{
		maxDurability = p->MagicDur;
	}
	for (mu_int32 i = 0; i < Level; ++i)
	{
		if (ip->Type >= ITEM_HELPER + 51)
		{
			break;
		}
		else if (i >= 14)
		{
			maxDurability = (maxDurability + 8 >= 255 ? 255 : maxDurability + 8);
		}
		else if (i >= 13)
		{
			maxDurability = (maxDurability + 7 >= 255 ? 255 : maxDurability + 7);
		}
		else if (i >= 12)
		{
			maxDurability += 6;
		}
		else if (i >= 11)
		{
			maxDurability += 5;
		}
		else if (i >= 10)
		{
			maxDurability += 4;
		}
		else if (i >= 9)
		{
			maxDurability += 3;
		}
		else if (i >= 4)
		{
			maxDurability += 2;
		}
		else
		{
			++maxDurability;
		}
	}

	if (ip->Type == ITEM_HELPER + 4 || ip->Type == ITEM_HELPER + 5)
	{
		maxDurability = 255;
	}

	if ((ip->SetOption % 0x04) == EXT_A_SET_OPTION || (ip->SetOption % 0x04) == EXT_B_SET_OPTION)
	{
		maxDurability += 20;
	}
	else if (ip->ExcOption > 0 &&
		(ip->Type<ITEM_WING + 3 || ip->Type>ITEM_WING + 6) &&
		(ip->Type != ITEM_SWORD + 19 && ip->Type != ITEM_BOW + 18 && ip->Type != ITEM_STAFF + 10)
		&& ip->Type != ITEM_HELPER + 30
		&& (ip->Type<ITEM_WING + 36 || ip->Type>ITEM_WING + 40)
		&& (ip->Type<ITEM_WING + 42 || ip->Type>ITEM_WING + 43)
		&& ip->Type != ITEM_MACE + 13
		)
	{
		maxDurability += 15;
	}

	return  maxDurability;
}

mu_int64 ItemValue(ITEM *ip, mu_int32 goldType)
{
	if (ip->Type == -1) return 0;

	ITEM_ATTRIBUTE *p = &ItemAttribute[ip->Type];

	mu_int64 Gold = 0;

	if (p->iZen != 0)
	{
		Gold = p->iZen;

		if (goldType)
		{
			Gold = Gold / 3;
		}

		if (Gold >= 1000)
			Gold = Gold / 100 * 100;
		else if (Gold >= 100)
			Gold = Gold / 10 * 10;

		return Gold;
	}

	mu_int32 Type = ip->Type / MAX_ITEM_INDEX;
	mu_int32 Level = ip->Level;
	mu_boolean Excellent = false;
	for (mu_int32 i = 0; i < ip->SpecialNum; ++i)
	{
		switch (ip->Special[i])
		{
		case AT_IMPROVE_LIFE:
		case AT_IMPROVE_MANA:
		case AT_DECREASE_DAMAGE:
		case AT_REFLECTION_DAMAGE:
		case AT_IMPROVE_BLOCKING_PERCENT:
		case AT_IMPROVE_GAIN_GOLD:
		case AT_EXCELLENT_DAMAGE:
		case AT_IMPROVE_DAMAGE_LEVEL:
		case AT_IMPROVE_DAMAGE_PERCENT:
		case AT_IMPROVE_MAGIC_LEVEL:
		case AT_IMPROVE_MAGIC_PERCENT:
		case AT_IMPROVE_ATTACK_SPEED:
		case AT_IMPROVE_GAIN_LIFE:
		case AT_IMPROVE_GAIN_MANA:
			Excellent = true;
			break;
		}
	}
	mu_int32 Level2 = p->Level + Level * 3;
	if (Excellent)
		Level2 += 25;

	if (ip->Type == ITEM_BOW + 7)
	{
		mu_int64 sellMoney = 0;

		switch (Level)
		{
		case    0:
			sellMoney = 100;
			break;
		case    1:
			sellMoney = 1400;
			break;
		case    2:
			sellMoney = 2200;
			break;
		case    3:
			sellMoney = 3000;
			break;
		}
		if (p->Durability > 0)
			Gold = (sellMoney*ip->Durability / p->Durability);
	}
	else if (ip->Type == ITEM_BOW + 15)
	{
		mu_int64 sellMoney = 0;

		switch (Level)
		{
		case    0:
			sellMoney = 70;
			break;
		case    1:
			sellMoney = 1200;
			break;
		case    2:
			sellMoney = 2000;
			break;
		case    3:
			sellMoney = 2800;
			break;
		}
		if (p->Durability > 0)
			Gold = (sellMoney*ip->Durability / p->Durability);
	}
	else if (COMGEM::isCompiledGem(ip))
	{
		Gold = COMGEM::CalcItemValue(ip);
	}
	else if (ip->Type == ITEM_POTION + 13)
	{
		Gold = 9000000;
	}
	else if (ip->Type == ITEM_POTION + 14)
	{
		Gold = 6000000;
	}
	else if (ip->Type == ITEM_WING + 15)
	{
		Gold = 810000;
	}
	else if (ip->Type == ITEM_POTION + 16)
	{
		Gold = 45000000;
	}
	else if (ip->Type == ITEM_POTION + 22)
	{
		Gold = 36000000;
	}
	else if (ip->Type == ITEM_POTION + 141)
	{
		Gold = 224000 * 3;
	}
	else if (ip->Type == ITEM_POTION + 142)
	{
		Gold = 182000 * 3;
	}
	else if (ip->Type == ITEM_POTION + 143)
	{
		Gold = 157000 * 3;
	}
	else if (ip->Type == ITEM_POTION + 144)
	{
		Gold = 121000 * 3;
	}
	else if (ip->Type == ITEM_HELPER + 14)
	{
		switch (Level)
		{
		case 0: Gold = 180000;  break;
		case 1: Gold = 7500000; break;
		}
	}
	else if (ip->Type == ITEM_HELPER + 3)
	{
		Gold = 960000;
		for (mu_int32 i = 0; i < ip->SpecialNum; ++i)
		{
			switch (ip->Special[i])
			{
			case AT_DAMAGE_ABSORB:
			case AT_IMPROVE_AG_MAX:
			case AT_IMPROVE_ATTACK_SPEED:
				Gold += 300000;
				break;
			}
		}
	}
	else if (ip->Type == ITEM_HELPER + 15)
	{
		Gold = 33000000;
	}
	else if (ip->Type == ITEM_HELPER + 16 || ip->Type == ITEM_HELPER + 17)
	{
		switch (Level)
		{
		case 1: Gold = 10000; break;
		case 2: Gold = 50000; break;
		case 3: Gold = 100000; break;
		case 4: Gold = 300000; break;
		case 5: Gold = 500000; break;
		case 6: Gold = 800000; break;
		case 7: Gold = 1000000; break;
		case 8: Gold = 1200000; break;
		default:
			Gold = 0;
		}
	}
	else if (ip->Type == ITEM_HELPER + 18)
	{
		Gold = 200000 + 20000 * (Level - 1);
		if (Level == 1)
		{
			Gold = 50000;
		}
		Gold *= 3;
	}
	else if (ip->Type == ITEM_POTION + 28)
	{
		Gold = 200000 * 3;
	}
	else if (ip->Type == ITEM_POTION + 29)
	{
		Gold = (ip->Durability * 10000) * 3;
	}
	else if (ip->Type == ITEM_POTION + 111)
	{
		Gold = 200000 * 3;
	}
	else if (ip->Type == ITEM_POTION + 110)
	{
		Gold = (ip->Durability * 10000) * 3;
	}
	else if (ITEM_POTION + 102 == ip->Type)
	{
		Gold = 10000 * 3;
	}
	else if (ITEM_POTION + 109 == ip->Type)
	{
		Gold = 10000 * 3;
	}
	else if (ITEM_POTION + 101 == ip->Type ||
		(ip->Type >= ITEM_POTION + 103 && ip->Type <= ITEM_POTION + 108))
	{
		Gold = (ip->Durability * 10000) * 3;
	}
	else if (ip->Type == ITEM_HELPER + 29)
	{
		Gold = 5000;
	}
	else if (ip->Type == ITEM_POTION + 21)
	{
		if (Level == 0)
			Gold = 9000;
		else if (Level == 1)
			Gold = 9000;
		else if (Level == 3)
		{
			Gold = ip->Durability * 3900;
		}
	}
	else if (ip->Type == ITEM_POTION + 17)
	{
		mu_int32 iValue[8] = { 30000, 10000, 50000, 100000, 300000, 500000, 800000, 1000000 };
		Gold = iValue[EMath::Min(EMath::Max(0, Level), 7)];
	}
	else if (ip->Type == ITEM_POTION + 18)
	{
		mu_int32 iValue[8] = { 30000, 15000, 75000, 150000, 450000, 750000, 1200000, 1500000 };
		Gold = iValue[EMath::Min(EMath::Max(0, Level), 7)];
	}
	else if (ip->Type == ITEM_POTION + 19)
	{
		mu_int32 iValue[8] = { 120000, 60000, 84000, 120000, 180000, 240000, 300000, 180000 };
		Gold = iValue[EMath::Min(EMath::Max(0, Level), 7)];
	}
	else if (ip->Type == ITEM_HELPER + 49 || ip->Type == ITEM_HELPER + 50 || ip->Type == ITEM_HELPER + 51)
	{
		switch (Level)
		{
		case 1: Gold = 500000; break;
		case 2: Gold = 600000; break;
		case 3: Gold = 800000; break;
		case 4: Gold = 1000000; break;
		case 5: Gold = 1200000; break;
		case 6: Gold = 1400000; break;
		default:
			Gold = 3000 * 3;
		}
	}
	else if (ip->Type == ITEM_POTION + 23 || ip->Type == ITEM_POTION + 24 || ip->Type == ITEM_POTION + 25 || ip->Type == ITEM_POTION + 26
		|| ip->Type == ITEM_POTION + 65 || ip->Type == ITEM_POTION + 66 || ip->Type == ITEM_POTION + 67 || ip->Type == ITEM_POTION + 68
		)
	{
		Gold = 9000;
	}
	else if (ip->Type == ITEM_POTION + 9 && Level == 1)
	{
		Gold = 1000;
	}
	else if (ip->Type == ITEM_POTION + 20)
	{
		Gold = 900;
	}
	else if (ip->Type == ITEM_POTION + 51)
	{
		Gold = 200000;
	}
	else if (ip->Type == ITEM_POTION + 63)
	{
		Gold = 200000;
	}
	else if (ip->Type == ITEM_POTION + 52)
	{
		Gold = 33 * 3;
	}
	else if (ip->Type == ITEM_POTION + 10)
	{
		Gold = 750;
	}
	else if (ip->Type == ITEM_POTION + 31)
	{
		Gold = 60000000;
	}
	else if (ip->Type == ITEM_POTION + 7)
	{
		switch (Level)
		{
		case 0: Gold = 900000 * ip->Durability; break;
		case 1: Gold = 450000 * ip->Durability; break;
		}
	}
	else if (ip->Type == ITEM_HELPER + 7)
	{
		switch (Level)
		{
		case 0: Gold = 1500000; break;
		case 1: Gold = 1200000; break;
		}
	}
	else if (ip->Type == ITEM_HELPER + 11)
	{
		switch (Level)
		{
		case 0: Gold = 100000; break;
		case 1: Gold = 2400000; break;
		}
	}
	else if (ip->Type >= ITEM_POTION + 35 && ip->Type <= ITEM_POTION + 37)
	{
		switch (ip->Type)
		{
		case ITEM_POTION + 35:
			Gold = 2000;
			break;
		case ITEM_POTION + 36:
			Gold = 4000;
			break;
		case ITEM_POTION + 37:
			Gold = 6000;
			break;
		}
		Gold *= ip->Durability;
	}
	else if (ip->Type >= ITEM_POTION + 38 && ip->Type <= ITEM_POTION + 40)
	{
		switch (ip->Type)
		{
		case ITEM_POTION + 38:
			Gold = 2500;
			break;
		case ITEM_POTION + 39:
			Gold = 5000;
			break;
		case ITEM_POTION + 40:
			Gold = 7500;
			break;
		}
		Gold *= ip->Durability;
	}
	else if (ip->Type == ITEM_POTION + 100)
	{
		Gold = 100 * 3;
		Gold *= ip->Durability;
	}
	else if (ip->Type == ITEM_POTION + 90 || ip->Type == ITEM_POTION + 85 || ip->Type == ITEM_POTION + 86 || ip->Type == ITEM_POTION + 87)
	{
		Gold = 300;
		Gold *= ip->Durability;
	}
	else if (p->Value > 0)
	{
		Gold = p->Value*p->Value * 10 / 12;

		if (ip->Type == ITEM_POTION + 3 || ip->Type == ITEM_POTION + 6)
		{
			Gold = 1500;
		}

		if (ip->Type >= ITEM_POTION && ip->Type <= ITEM_POTION + 8)	//¹°¾à
		{
			if (Level > 0)
				Gold *= (mu_int64)EMath::Pow((double)2, (double)Level);

			Gold = Gold / 10 * 10;
			Gold *= (mu_int64)ip->Durability;

			if (goldType)
			{
				Gold = Gold / 3;
				Gold = Gold / 10 * 10;
			}
			return Gold;
		}
	}
	else if (ip->Type == ITEM_HELPER + 20)
	{
		if (Level == 0)
			Gold = 3000;
		else if (Level == 1)
			Gold = 0;
	}
	else if (ip->Type == ITEM_HELPER + 52 || ip->Type == ITEM_HELPER + 53)
	{
		Gold = 3000000;
	}
	else if (ip->Type == ITEM_HELPER + 31)
	{
		switch (Level)
		{
		case 0: Gold = 10000000 * 3; break;
		case 1: Gold = 5000000 * 3;  break;
		}
	}
	else if (((Type == 12 && (ip->Type > ITEM_WING + 6
		&& !(ip->Type >= ITEM_WING + 36 && ip->Type <= ITEM_WING + 43)
		)) || Type == 13 || Type == 15))
	{
		Gold = 100 + Level2*Level2*Level2;
		for (mu_int32 i = 0; i < ip->SpecialNum; ++i)
		{
			switch (ip->Special[i])
			{
			case AT_LIFE_REGENERATION:
				Gold += Gold*ip->SpecialValue[i];
				break;
			}
		}
	}
	else if (ip->Type == ITEM_POTION + 53)
	{
		Gold = 0;
	}
	else
	{
		switch (Level)
		{
		case 5:Level2 += 4; break;
		case 6:Level2 += 10; break;
		case 7:Level2 += 25; break;
		case 8:Level2 += 45; break;
		case 9:Level2 += 65; break;
		case 10:Level2 += 95; break;
		case 11:Level2 += 135; break;
		case 12:Level2 += 185; break;
		case 13:Level2 += 245; break;
		case 14:Level2 += 305; break;
		case 15:Level2 += 365; break;
		}

		if ((Type == 12 && ip->Type <= ITEM_WING + 6) || ip->Type == ITEM_HELPER + 30
			|| (ip->Type >= ITEM_WING + 36 && ip->Type <= ITEM_WING + 43)
			)
		{
			Gold = 40000000 + ((40 + Level2)*Level2*Level2 * 11);
		}
		else
		{
			Gold = 100 + (40 + Level2)*Level2*Level2 / 8;
		}
		if (Type >= 0 && Type <= 6)
		{
			if (!p->TwoHand)
				Gold = Gold * 80 / 100;
		}
		for (mu_int32 i = 0; i < ip->SpecialNum; ++i)
		{
			switch (ip->Special[i])
			{
			case AT_SKILL_BLOCKING:
			case AT_SKILL_SWORD1:
			case AT_SKILL_SWORD2:
			case AT_SKILL_SWORD3:
			case AT_SKILL_SWORD4:
			case AT_SKILL_SWORD5:
			case AT_SKILL_MANY_ARROW_UP:
			case AT_SKILL_MANY_ARROW_UP + 1:
			case AT_SKILL_MANY_ARROW_UP + 2:
			case AT_SKILL_MANY_ARROW_UP + 3:
			case AT_SKILL_MANY_ARROW_UP + 4:
			case AT_SKILL_CROSSBOW:
			case AT_SKILL_RECOVER:
			case AT_SKILL_GAOTIC:
			case AT_SKILL_MULTI_SHOT:
			case AT_SKILL_POWER_SLASH_UP:
			case AT_SKILL_POWER_SLASH_UP + 1:
			case AT_SKILL_POWER_SLASH_UP + 2:
			case AT_SKILL_POWER_SLASH_UP + 3:
			case AT_SKILL_POWER_SLASH_UP + 4:
			case AT_SKILL_ICE_BLADE:
				Gold += (mu_int64)((mu_float)Gold*1.5f);
				break;
			case AT_IMPROVE_DAMAGE:
			case AT_IMPROVE_MAGIC:
			case AT_IMPROVE_CURSE:
			case AT_IMPROVE_DEFENSE:
			case AT_LIFE_REGENERATION:
				if ((Type == 12 && ip->Type <= ITEM_WING + 6)
					|| (ip->Type >= ITEM_WING + 36 && ip->Type <= ITEM_WING + 43)
					)
				{
					mu_int32 iOption = ip->SpecialValue[i];
					if (AT_LIFE_REGENERATION == ip->Special[i])
					{
						iOption *= 4;
					}
					switch (iOption)
					{
					case 4:Gold += (mu_int64)((double)Gold * 6 / 10); break;
					case 8:Gold += (mu_int64)((double)Gold * 14 / 10); break;
					case 12:Gold += (mu_int64)((double)Gold * 28 / 10); break;
					case 16:Gold += (mu_int64)((double)Gold * 56 / 10); break;
					}
				}
				else
				{
					switch (ip->SpecialValue[i])
					{
					case 4:Gold += (mu_int64)((double)Gold * 6 / 10); break;
					case 8:Gold += (mu_int64)((double)Gold * 14 / 10); break;
					case 12:Gold += (mu_int64)((double)Gold * 28 / 10); break;
					case 16:Gold += (mu_int64)((double)Gold * 56 / 10); break;
					}
				}
				break;
			case AT_IMPROVE_BLOCKING:
				switch (ip->SpecialValue[i])
				{
				case 5:Gold += (mu_int64)((double)Gold * 6 / 10); break;
				case 10:Gold += (mu_int64)((double)Gold * 14 / 10); break;
				case 15:Gold += (mu_int64)((double)Gold * 28 / 10); break;
				case 20:Gold += (mu_int64)((double)Gold * 56 / 10); break;
				}
				break;
			case AT_LUCK:
				Gold += (mu_int64)((double)Gold * 25 / 100);
				break;
			case AT_IMPROVE_LIFE:
			case AT_IMPROVE_MANA:
			case AT_DECREASE_DAMAGE:
			case AT_REFLECTION_DAMAGE:
			case AT_IMPROVE_BLOCKING_PERCENT:
			case AT_IMPROVE_GAIN_GOLD:
			case AT_EXCELLENT_DAMAGE:
			case AT_IMPROVE_DAMAGE_LEVEL:
			case AT_IMPROVE_DAMAGE_PERCENT:
			case AT_IMPROVE_MAGIC_LEVEL:
			case AT_IMPROVE_MAGIC_PERCENT:
			case AT_IMPROVE_GAIN_LIFE:
			case AT_IMPROVE_GAIN_MANA:
			case AT_IMPROVE_ATTACK_SPEED:
				Gold += Gold;
				break;

			case AT_IMPROVE_EVADE:
				Gold += Gold;
				break;

			case AT_IMPROVE_HP_MAX:
			case AT_IMPROVE_MP_MAX:
			case AT_ONE_PERCENT_DAMAGE:
			case AT_IMPROVE_AG_MAX:
			case AT_DAMAGE_ABSORB:
			case AT_DAMAGE_REFLECTION:
			case AT_RECOVER_FULL_LIFE:
			case AT_RECOVER_FULL_MANA:
				Gold += (mu_int64)((double)Gold * 25 / 100);
				break;
			}
		}

		Gold += g_SocketItemMgr.CalcSocketBonusItemValue(ip, Gold);
	}
	Gold = EMath::Min(Gold, 3000000000ll);

	if (goldType == 2)
	{
		if (Gold >= 1000)
		{
			Gold = (Gold / 100) * 100;
		}
		else if (Gold >= 100)
		{
			Gold = (Gold / 10) * 10;
		}
	}

	if (goldType)
	{
		Gold = Gold / 3;
	}

	if (ip->Type == ITEM_HELPER + 32)
		Gold = ip->Durability * 50;
	else if (ip->Type == ITEM_HELPER + 33)
		Gold = ip->Durability * 100;
	else if (ip->Type == ITEM_HELPER + 34)
		Gold = ip->Durability * 1000;
	else if (ip->Type == ITEM_HELPER + 35)
		Gold = ip->Durability * 10000;
	else if (ip->Type == ITEM_HELPER + 36)
		Gold = 30000;
	else if (ip->Type == ITEM_HELPER + 37)
		Gold = 50000;

#if 0 // FIX
	if (CPCRoomPtSys::Instance().IsPCRoomPointShopMode())
	{
		switch (ip->Type)
		{
		case ITEM_HELPER + 43:
			Gold = 6;
			break;
		case ITEM_HELPER + 44:
			Gold = 9;
			break;
		case ITEM_HELPER + 45:
			Gold = 15;
			break;
		}
	}
#endif

	if (ip->Type >= ITEM_POTION + 45 && ip->Type <= ITEM_POTION + 50)
	{
		Gold = ip->Durability * 50;
	}

	if (ip->Type == ITEM_HELPER + 71 || ip->Type == ITEM_HELPER + 72 || ip->Type == ITEM_HELPER + 73 || ip->Type == ITEM_HELPER + 74 || ip->Type == ITEM_HELPER + 75)
	{
		Gold = 2000000;
	}

	if ((ip->Type == ITEM_HELPER + 4) || (ip->Type == ITEM_HELPER + 5))
	{
		Gold = giPetManager::GetPetItemValue(ip);
	}

	switch (ip->Type)
	{
	case ITEM_POTION + 112:
	case ITEM_POTION + 113:
	case ITEM_POTION + 121:
	case ITEM_POTION + 122:
	case ITEM_POTION + 123:
	case ITEM_POTION + 124:
	case ITEM_HELPER + 80:
	case ITEM_HELPER + 76:
	case ITEM_HELPER + 64:
	case ITEM_HELPER + 65:
	case ITEM_HELPER + 109:
	case ITEM_HELPER + 110:
	case ITEM_HELPER + 111:
	case ITEM_HELPER + 112:
	case ITEM_HELPER + 113:
	case ITEM_HELPER + 114:
	case ITEM_HELPER + 115:
		Gold = 1000;
		break;
	case ITEM_HELPER + 122:
	case ITEM_HELPER + 123:
		Gold = 2000;
		break;
	case ITEM_WING + 130:
	case ITEM_WING + 131:
	case ITEM_WING + 132:
	case ITEM_WING + 133:
	case ITEM_WING + 134:
		Gold = 80;
		break;
	}

	if (ip->Type == ITEM_HELPER + 10 || ip->Type == ITEM_HELPER + 20 || ip->Type == ITEM_HELPER + 29)
		goto EXIT_CALCULATE;
	if (ip->Type == ITEM_BOW + 7 || ip->Type == ITEM_BOW + 15 || ip->Type >= ITEM_POTION)
		goto EXIT_CALCULATE;
	if (ip->Type >= ITEM_WING + 7 && ip->Type <= ITEM_WING + 19)
		goto EXIT_CALCULATE;
	if ((ip->Type >= ITEM_HELPER + 14 && ip->Type <= ITEM_HELPER + 19) || ip->Type == ITEM_POTION + 21)
		goto EXIT_CALCULATE;
	if (ip->Type == ITEM_POTION + 7 || ip->Type == ITEM_HELPER + 7 || ip->Type == ITEM_HELPER + 11)
		goto EXIT_CALCULATE;
	if ((ip->Type >= ITEM_HELPER + 49) && (ip->Type <= ITEM_HELPER + 51))
		goto EXIT_CALCULATE;

	switch (ip->Type)
	{
	case ITEM_POTION + 112:	goto EXIT_CALCULATE;
	case ITEM_POTION + 113:	goto EXIT_CALCULATE;
	case ITEM_POTION + 121:	goto EXIT_CALCULATE;
	case ITEM_POTION + 122:	goto EXIT_CALCULATE;
	case ITEM_POTION + 123:	goto EXIT_CALCULATE;
	case ITEM_POTION + 124:	goto EXIT_CALCULATE;
	case ITEM_WING + 130:
	case ITEM_WING + 131:
	case ITEM_WING + 132:
	case ITEM_WING + 133:
	case ITEM_WING + 134:
		goto EXIT_CALCULATE;
	case ITEM_HELPER + 122:
		goto EXIT_CALCULATE;
	case ITEM_HELPER + 123:
		goto EXIT_CALCULATE;
	case ITEM_HELPER + 80:
		goto EXIT_CALCULATE;
	case ITEM_HELPER + 76:
		goto EXIT_CALCULATE;
	case ITEM_HELPER + 64:
	case ITEM_HELPER + 65:
		goto EXIT_CALCULATE;
	case ITEM_HELPER + 109:
		goto EXIT_CALCULATE;
	case ITEM_HELPER + 110:
		goto EXIT_CALCULATE;
	case ITEM_HELPER + 111:
		goto EXIT_CALCULATE;
	case ITEM_HELPER + 112:
		goto EXIT_CALCULATE;
	case ITEM_HELPER + 113:
		goto EXIT_CALCULATE;
	case ITEM_HELPER + 114:
		goto EXIT_CALCULATE;
	case ITEM_HELPER + 115:
		goto EXIT_CALCULATE;
	case ITEM_HELPER + 128:
	case ITEM_HELPER + 129:
	case ITEM_HELPER + 134:
		goto EXIT_CALCULATE;
	case ITEM_HELPER + 130:
	case ITEM_HELPER + 131:
	case ITEM_HELPER + 132:
	case ITEM_HELPER + 133:
		goto EXIT_CALCULATE;
	}

	if (goldType == 1 && !(ip->Type >= ITEM_HELPER + 32 && ip->Type <= ITEM_HELPER + 37))
	{
		mu_int64 maxDurability = calcMaxDurability(ip, p, Level);
		mu_float persent = 1.0f - ((mu_float)ip->Durability / (mu_float)maxDurability);
		mu_int64 repairGold = (mu_int64)(Gold * 0.6f * persent);

		if (ip->Type == ITEM_HELPER + 31)
			repairGold = 0;

		Gold -= repairGold;
	}
EXIT_CALCULATE:

	if (Gold >= 1000)
	{
		Gold = (Gold / 100) * 100;
	}
	else if (Gold >= 100)
	{
		Gold = (Gold / 10) * 10;
	}

	return (mu_int64)Gold;
}

mu_boolean GetAttackDamage(mu_int32* MinDamage, mu_int32* MaxDamage)
{
	mu_int32 AttackDamageMin;
	mu_int32 AttackDamageMax;

	ITEM *r = &CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT];
	ITEM *l = &CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT];

	if (GetEquipedBowType() == BOWTYPE_CROSSBOW)
	{
		AttackDamageMin = CharacterAttribute->AttackDamageMinRight;
		AttackDamageMax = CharacterAttribute->AttackDamageMaxRight;
	}
	else if (GetEquipedBowType() == BOWTYPE_BOW)
	{
		AttackDamageMin = CharacterAttribute->AttackDamageMinLeft;
		AttackDamageMax = CharacterAttribute->AttackDamageMaxLeft;
	}
	else if (r->Type == -1)
	{
		AttackDamageMin = CharacterAttribute->AttackDamageMinLeft;
		AttackDamageMax = CharacterAttribute->AttackDamageMaxLeft;
	}
	else if (r->Type >= ITEM_STAFF && r->Type < ITEM_SHIELD)
	{
		AttackDamageMin = CharacterAttribute->AttackDamageMinLeft;
		AttackDamageMax = CharacterAttribute->AttackDamageMaxLeft;
	}
	else
	{
		AttackDamageMin = CharacterAttribute->AttackDamageMinRight;
		AttackDamageMax = CharacterAttribute->AttackDamageMaxRight;
	}

	mu_boolean Alpha = false;
	if (GetBaseClass(Hero->Class) == GAME::CLASS_KNIGHT || GetBaseClass(Hero->Class) == GAME::CLASS_DARK)
	{
		if (l->Type >= ITEM_SWORD && l->Type < ITEM_SHIELD && r->Type >= ITEM_SWORD && r->Type < ITEM_SHIELD)
		{
			Alpha = true;
			AttackDamageMin = ((CharacterAttribute->AttackDamageMinRight * 55) / 100 + (CharacterAttribute->AttackDamageMinLeft * 55) / 100);
			AttackDamageMax = ((CharacterAttribute->AttackDamageMaxRight * 55) / 100 + (CharacterAttribute->AttackDamageMaxLeft * 55) / 100);
		}
	}
	else if (GetBaseClass(Hero->Class) == GAME::CLASS_ELF)
	{
		if ((r->Type >= ITEM_BOW && r->Type < ITEM_STAFF) &&
			(l->Type >= ITEM_BOW && l->Type < ITEM_STAFF))
		{
			if ((l->Type == ITEM_BOW + 7 && l->Level >= 1) || (r->Type == ITEM_BOW + 15 && r->Level >= 1))
			{
				Alpha = true;
			}
		}
	}

	if (CharacterAttribute->Ability&ABILITY_PLUS_DAMAGE)
	{
		AttackDamageMin += 15;
		AttackDamageMax += 15;
	}

	*MinDamage = AttackDamageMin;
	*MaxDamage = AttackDamageMax;

	return Alpha;
}

mu_uint8 getSkillIndexByBook(mu_int32 Type)
{
	mu_int32 SkillIndex = Type - ITEM_ETC + 1;
	switch (SkillIndex)
	{
	case 17: SkillIndex = AT_SKILL_BLAST_POISON; break;
	case 18: SkillIndex = AT_SKILL_BLAST_FREEZE; break;
	case 19: SkillIndex = AT_SKILL_BLAST_HELL;   break;
	}

	return SkillIndex;
}

void GetSpecialOptionText(mu_int32 Type, mu_unicode *Text, const mu_size TextLength, mu_uint16 Option, mu_uint8 Value, mu_int32 iMana)
{
	switch (Option)
	{
	case AT_SKILL_BLOCKING:
		GetSkillInformation(Option, 1, nullptr, 0, &iMana, nullptr);
		mu_sprintf_s(Text, TextLength, GlobalText[80], iMana);
		break;
	case AT_SKILL_SWORD1:
		GetSkillInformation(Option, 1, nullptr, 0, &iMana, nullptr);
		mu_sprintf_s(Text, TextLength, GlobalText[81], iMana);
		break;
	case AT_SKILL_SWORD2:
		GetSkillInformation(Option, 1, nullptr, 0, &iMana, nullptr);
		mu_sprintf_s(Text, TextLength, GlobalText[82], iMana);
		break;
	case AT_SKILL_SWORD3:
		GetSkillInformation(Option, 1, nullptr, 0, &iMana, nullptr);
		mu_sprintf_s(Text, TextLength, GlobalText[83], iMana);
		break;
	case AT_SKILL_SWORD4:
		GetSkillInformation(Option, 1, nullptr, 0, &iMana, nullptr);
		mu_sprintf_s(Text, TextLength, GlobalText[84], iMana);
		break;
	case AT_SKILL_SWORD5:
		GetSkillInformation(Option, 1, nullptr, 0, &iMana, nullptr);
		mu_sprintf_s(Text, TextLength, GlobalText[85], iMana);
		break;
	case AT_SKILL_MANY_ARROW_UP:
	case AT_SKILL_MANY_ARROW_UP + 1:
	case AT_SKILL_MANY_ARROW_UP + 2:
	case AT_SKILL_MANY_ARROW_UP + 3:
	case AT_SKILL_MANY_ARROW_UP + 4:

	case AT_SKILL_CROSSBOW:
		GetSkillInformation(Option, 1, nullptr, 0, &iMana, nullptr);
		mu_sprintf_s(Text, TextLength, GlobalText[86], iMana);
		break;
	case AT_SKILL_BLAST_CROSSBOW4:
		GetSkillInformation(Option, 1, nullptr, 0, &iMana, nullptr);
		mu_sprintf_s(Text, TextLength, GlobalText[920], iMana);
		break;
	case AT_SKILL_MULTI_SHOT:
		GetSkillInformation(Option, 1, nullptr, 0, &iMana, nullptr);
		mu_sprintf_s(Text, TextLength, GlobalText[920], iMana);
		break;
	case AT_SKILL_RECOVER:
		GetSkillInformation(Option, 1, nullptr, 0, &iMana, nullptr);
		mu_sprintf_s(Text, TextLength, GlobalText[920], iMana);
		break;
	case AT_SKILL_POWER_SLASH_UP:
	case AT_SKILL_POWER_SLASH_UP + 1:
	case AT_SKILL_POWER_SLASH_UP + 2:
	case AT_SKILL_POWER_SLASH_UP + 3:
	case AT_SKILL_POWER_SLASH_UP + 4:
	case AT_SKILL_ICE_BLADE:
		GetSkillInformation(Option, 1, nullptr, 0, &iMana, nullptr);
		mu_sprintf_s(Text, TextLength, GlobalText[98], iMana);
		break;
	case AT_LUCK:
		mu_sprintf_s(Text, TextLength, GlobalText[87]);
		break;
	case AT_IMPROVE_DAMAGE:
		mu_sprintf_s(Text, TextLength, GlobalText[88], Value);
		break;
	case AT_IMPROVE_MAGIC:
		mu_sprintf_s(Text, TextLength, GlobalText[89], Value);
		break;
	case AT_IMPROVE_CURSE:
		mu_sprintf_s(Text, TextLength, GlobalText[1697], Value);
		break;
	case AT_IMPROVE_BLOCKING:
		mu_sprintf_s(Text, TextLength, GlobalText[90], Value);
		break;
	case AT_IMPROVE_DEFENSE:
		mu_sprintf_s(Text, TextLength, GlobalText[91], Value);
		break;
	case AT_LIFE_REGENERATION:
		if (!(ITEM_HELPER + 14 <= Type && Type <= ITEM_HELPER + 18))
		{
			mu_sprintf_s(Text, TextLength, GlobalText[92], Value);
		}
		break;
	case AT_IMPROVE_LIFE:
		mu_sprintf_s(Text, TextLength, GlobalText[622]);
		break;
	case AT_IMPROVE_MANA:
		mu_sprintf_s(Text, TextLength, GlobalText[623]);
		break;
	case AT_DECREASE_DAMAGE:
		mu_sprintf_s(Text, TextLength, GlobalText[624]);
		break;

	case AT_REFLECTION_DAMAGE:
		mu_sprintf_s(Text, TextLength, GlobalText[625]);
		break;
	case AT_IMPROVE_BLOCKING_PERCENT:
		mu_sprintf_s(Text, TextLength, GlobalText[626]);
		break;
	case AT_IMPROVE_GAIN_GOLD:
		mu_sprintf_s(Text, TextLength, GlobalText[627]);
		break;
	case AT_EXCELLENT_DAMAGE:
		mu_sprintf_s(Text, TextLength, GlobalText[628]);
		break;
	case AT_IMPROVE_DAMAGE_LEVEL:
		mu_sprintf_s(Text, TextLength, GlobalText[629]);
		break;
	case AT_IMPROVE_DAMAGE_PERCENT:
		mu_sprintf_s(Text, TextLength, GlobalText[630], Value);
		break;
	case AT_IMPROVE_MAGIC_LEVEL:
		mu_sprintf_s(Text, TextLength, GlobalText[631]);
		break;
	case AT_IMPROVE_MAGIC_PERCENT:
		mu_sprintf_s(Text, TextLength, GlobalText[632], Value);
		break;
	case AT_IMPROVE_ATTACK_SPEED:
		mu_sprintf_s(Text, TextLength, GlobalText[633], Value);
		break;
	case AT_IMPROVE_GAIN_LIFE:
		mu_sprintf_s(Text, TextLength, GlobalText[634]);
		break;
	case AT_IMPROVE_GAIN_MANA:
		mu_sprintf_s(Text, TextLength, GlobalText[635]);
		break;
	case AT_IMPROVE_EVADE:
		mu_sprintf_s(Text, TextLength, GlobalText[746]);
		break;
	case AT_SKILL_RIDER:
		GetSkillInformation(Option, 1, nullptr, 0, &iMana, nullptr);
		mu_sprintf_s(Text, TextLength, GlobalText[745], iMana);
		break;
	case AT_SKILL_STRONG_PIER:  //  
		GetSkillInformation(Option, 1, nullptr, 0, &iMana, nullptr);
		mu_sprintf_s(Text, TextLength, GlobalText[1210], iMana);
		break;
	case AT_SKILL_LONG_SPEAR:
		GetSkillInformation(Option, 1, nullptr, 0, &iMana, nullptr);
		mu_sprintf_s(Text, TextLength, GlobalText[1186], iMana);
		break;
	case AT_SKILL_ASHAKE_UP:
	case AT_SKILL_ASHAKE_UP + 1:
	case AT_SKILL_ASHAKE_UP + 2:
	case AT_SKILL_ASHAKE_UP + 3:
	case AT_SKILL_ASHAKE_UP + 4:
	case AT_SKILL_DARK_HORSE:
		GetSkillInformation(Option, 1, nullptr, 0, &iMana, nullptr);
		mu_sprintf_s(Text, TextLength, GlobalText[1189], iMana);
		break;
	case AT_SKILL_PLASMA_STORM_FENRIR:
		GetSkillInformation(Option, 1, nullptr, 0, &iMana, nullptr);
		mu_sprintf_s(Text, TextLength, GlobalText[1928], iMana);
		break;
	case AT_SET_OPTION_IMPROVE_DEFENCE:
		mu_sprintf_s(Text, TextLength, GlobalText[959], Value);
		break;
	case AT_SET_OPTION_IMPROVE_CHARISMA:
		mu_sprintf_s(Text, TextLength, GlobalText[954], Value);
		break;
	case AT_SET_OPTION_IMPROVE_DAMAGE:
		mu_sprintf_s(Text, TextLength, GlobalText[577], Value);
		break;
	case AT_IMPROVE_HP_MAX:
		mu_sprintf_s(Text, TextLength, GlobalText[740], Value);
		break;
	case AT_IMPROVE_MP_MAX:
		mu_sprintf_s(Text, TextLength, GlobalText[741], Value);
		break;
	case AT_ONE_PERCENT_DAMAGE:
		mu_sprintf_s(Text, TextLength, GlobalText[742], Value);
		break;
	case AT_IMPROVE_AG_MAX:
		mu_sprintf_s(Text, TextLength, GlobalText[743], Value);
		break;
	case AT_DAMAGE_ABSORB:
		mu_sprintf_s(Text, TextLength, GlobalText[744], Value);
		break;
	case AT_SET_OPTION_IMPROVE_STRENGTH:
		mu_sprintf_s(Text, TextLength, GlobalText[985], Value);
		break;

	case AT_SET_OPTION_IMPROVE_DEXTERITY:
		mu_sprintf_s(Text, TextLength, GlobalText[986], Value);
		break;

	case AT_SET_OPTION_IMPROVE_VITALITY:
		mu_sprintf_s(Text, TextLength, GlobalText[987], Value);
		break;

	case AT_SET_OPTION_IMPROVE_ENERGY:
		mu_sprintf_s(Text, TextLength, GlobalText[988], Value);
		break;

	case AT_IMPROVE_MAX_MANA:
		mu_sprintf_s(Text, TextLength, GlobalText[1087], Value);
		break;

	case AT_IMPROVE_MAX_AG:
		mu_sprintf_s(Text, TextLength, GlobalText[1088], Value);
		break;
	case AT_DAMAGE_REFLECTION:
		mu_sprintf_s(Text, TextLength, GlobalText[1673], Value);
		break;
	case AT_RECOVER_FULL_LIFE:
		mu_sprintf_s(Text, TextLength, GlobalText[1674], Value);
		break;
	case AT_RECOVER_FULL_MANA:
		mu_sprintf_s(Text, TextLength, GlobalText[1675], Value);
		break;
	case AT_SKILL_SUMMON_EXPLOSION:
		mu_sprintf_s(Text, TextLength, GlobalText[1695], iMana);
		break;
	case AT_SKILL_SUMMON_REQUIEM:
		mu_sprintf_s(Text, TextLength, GlobalText[1696], iMana);
		break;
	case AT_SKILL_SUMMON_POLLUTION:
		mu_sprintf_s(Text, TextLength, GlobalText[1789], iMana);
		break;
	}
}

mu_boolean IsInvenItem(const mu_int16 sType)
{
	if ((sType == ITEM_HELPER + 128 || sType == ITEM_HELPER + 129 || sType == ITEM_HELPER + 134)
		|| (sType >= ITEM_HELPER + 130 && sType <= ITEM_HELPER + 133))
		return true;

	return false;
}

mu_boolean IsRequireClassRenderItem(const mu_int16 sType)
{
	if (sType == ITEM_HELPER + 19
		|| sType == ITEM_HELPER + 29
		|| sType == ITEM_WING + 26
		|| (sType >= ITEM_WING + 30 && sType <= ITEM_WING + 31)
		|| (sType >= ITEM_HELPER + 43 && sType <= ITEM_HELPER + 45)
		|| sType == ITEM_HELPER + 10
		|| (sType >= ITEM_HELPER + 39 && sType <= ITEM_HELPER + 42)
		|| sType == ITEM_HELPER + 37
		|| sType == ITEM_WING + 15
		|| sType == ITEM_WING + 32
		|| sType == ITEM_WING + 33
		|| sType == ITEM_WING + 34)
	{
		return false;
	}

	if ((sType >= ITEM_HELPER + 43 && sType <= ITEM_HELPER + 45)
		|| (sType >= ITEM_HELPER + 46 && sType <= ITEM_HELPER + 48)
		|| (sType >= ITEM_HELPER + 125 && sType <= ITEM_HELPER + 127)
		|| (sType == ITEM_POTION + 54)
		|| (sType >= ITEM_POTION + 58 && sType <= ITEM_POTION + 62)
		|| (sType == ITEM_POTION + 53)
		|| (sType >= ITEM_POTION + 70 && sType <= ITEM_POTION + 71)
		|| (sType >= ITEM_POTION + 72 && sType <= ITEM_POTION + 77)
		|| (sType >= ITEM_TYPE_CHARM_MIXWING + EWS_BEGIN
			&& sType <= ITEM_TYPE_CHARM_MIXWING + EWS_END)
		|| (sType == ITEM_HELPER + 59)
		|| (sType >= ITEM_HELPER + 54 && sType <= ITEM_HELPER + 58)
		|| (sType >= ITEM_POTION + 78 && sType <= ITEM_POTION + 82)
		|| (sType == ITEM_HELPER + 60)
		|| (sType == ITEM_HELPER + 61)
		|| (sType == ITEM_POTION + 91)
		|| (sType == ITEM_POTION + 94)
		|| (sType >= ITEM_POTION + 92 && sType <= ITEM_POTION + 93)
		|| (sType == ITEM_POTION + 95)
		|| (sType >= ITEM_HELPER + 62 && sType <= ITEM_HELPER + 63)
		|| (sType >= ITEM_POTION + 97 && sType <= ITEM_POTION + 98)
		|| (sType == ITEM_POTION + 96)
		|| (sType == ITEM_HELPER + 64 || sType == ITEM_HELPER + 65)
		|| (sType == ITEM_HELPER + 67)
		|| (sType == ITEM_HELPER + 68)
		|| (sType == ITEM_HELPER + 76)
		|| (sType == ITEM_HELPER + 122)
		|| (sType == ITEM_HELPER + 69)
		|| (sType == ITEM_HELPER + 70)
		|| (sType == ITEM_HELPER + 71 || sType == ITEM_HELPER + 72 || sType == ITEM_HELPER + 73 || sType == ITEM_HELPER + 74 || sType == ITEM_HELPER + 75)
		|| (sType == ITEM_HELPER + 80)
		|| (sType == ITEM_HELPER + 106)
		|| sType == ITEM_HELPER + 81
		|| sType == ITEM_HELPER + 82
		|| sType == ITEM_HELPER + 93
		|| sType == ITEM_HELPER + 94
		|| sType == ITEM_HELPER + 121
		|| (sType >= ITEM_POTION + 145 && sType <= ITEM_POTION + 150)
		|| IsInvenItem(sType)
		|| (sType == ITEM_POTION + 133)
		)
	{
		return false;
	}

	return true;
}

mu_boolean IsPartChargeItem(ITEM* pItem)
{
	if ((pItem->Type >= ITEM_HELPER + 43 && pItem->Type <= ITEM_HELPER + 48)
		|| (pItem->Type >= ITEM_HELPER + 54 && pItem->Type <= ITEM_HELPER + 65)
		|| (pItem->Type == ITEM_HELPER + 69)
		|| (pItem->Type == ITEM_HELPER + 70)
		|| (pItem->Type == ITEM_HELPER + 76)
		|| (pItem->Type == ITEM_HELPER + 80)
		|| (pItem->Type >= ITEM_HELPER + 81 && pItem->Type <= ITEM_HELPER + 94)
		|| pItem->Type == ITEM_HELPER + 103
		|| pItem->Type == ITEM_HELPER + 104
		|| pItem->Type == ITEM_HELPER + 105
		|| pItem->Type == ITEM_HELPER + 106
		|| pItem->Type == ITEM_HELPER + 107
		|| pItem->Type == ITEM_HELPER + 109
		|| pItem->Type == ITEM_HELPER + 110
		|| pItem->Type == ITEM_HELPER + 111
		|| pItem->Type == ITEM_HELPER + 112
		|| pItem->Type == ITEM_HELPER + 113
		|| pItem->Type == ITEM_HELPER + 114
		|| pItem->Type == ITEM_HELPER + 115
		|| pItem->Type == ITEM_HELPER + 116
		|| pItem->Type == ITEM_HELPER + 121
		|| pItem->Type == ITEM_HELPER + 122
		|| pItem->Type == ITEM_HELPER + 123
		|| pItem->Type == ITEM_HELPER + 124
		|| (pItem->Type >= ITEM_HELPER + 125 && pItem->Type <= ITEM_HELPER + 127))
	{
		return true;
	}

	if (pItem->Type == ITEM_POTION + 53
		|| pItem->Type == ITEM_POTION + 54
		|| (pItem->Type >= ITEM_POTION + 58 && pItem->Type <= ITEM_POTION + 62)
		|| (pItem->Type >= ITEM_POTION + 70 && pItem->Type <= ITEM_POTION + 83)
		|| (pItem->Type >= ITEM_POTION + 91 && pItem->Type <= ITEM_POTION + 98)
		|| pItem->Type == ITEM_POTION + 112
		|| pItem->Type == ITEM_POTION + 113
		|| pItem->Type == ITEM_POTION + 114
		|| pItem->Type == ITEM_POTION + 115
		|| pItem->Type == ITEM_POTION + 116
		|| pItem->Type == ITEM_POTION + 117
		|| pItem->Type == ITEM_POTION + 118
		|| pItem->Type == ITEM_POTION + 119
		|| pItem->Type == ITEM_POTION + 120
		|| pItem->Type == ITEM_POTION + 123
		|| pItem->Type == ITEM_POTION + 124
		|| pItem->Type == ITEM_POTION + 126
		|| pItem->Type == ITEM_POTION + 127
		|| pItem->Type == ITEM_POTION + 128
		|| pItem->Type == ITEM_POTION + 129
		|| pItem->Type == ITEM_POTION + 130
		|| pItem->Type == ITEM_POTION + 131
		|| pItem->Type == ITEM_POTION + 132
		|| pItem->Type == ITEM_POTION + 133
		|| pItem->Type == ITEM_POTION + 134
		|| pItem->Type == ITEM_POTION + 135
		|| pItem->Type == ITEM_POTION + 136
		|| pItem->Type == ITEM_POTION + 137
		|| pItem->Type == ITEM_POTION + 138
		|| pItem->Type == ITEM_POTION + 139
		|| pItem->Type == ITEM_POTION + 140
		|| (pItem->Type >= ITEM_POTION + 145 && pItem->Type <= ITEM_POTION + 150))
	{
		return true;
	}

	if (pItem->Type == ITEM_WING + 130
		|| pItem->Type == ITEM_WING + 131
		|| pItem->Type == ITEM_WING + 132
		|| pItem->Type == ITEM_WING + 133
		|| pItem->Type == ITEM_WING + 134
		|| pItem->Type == ITEM_WING + 135)
	{
		return true;
	}

	return false;
}

mu_boolean IsHighValueItem(ITEM* pItem)
{
	mu_int32 iLevel = pItem->Level;

	if (pItem->Type == ITEM_HELPER + 3 ||
		pItem->Type == ITEM_POTION + 13 ||
		pItem->Type == ITEM_POTION + 14 ||
		pItem->Type == ITEM_POTION + 16 ||
		pItem->Type == ITEM_POTION + 22 ||
		pItem->Type == ITEM_WING + 15 ||
		pItem->Type == ITEM_POTION + 31 ||
		pItem->Type == ITEM_WING + 30 ||
		pItem->Type == ITEM_WING + 31 ||
		(pItem->Type >= ITEM_WING && pItem->Type <= ITEM_WING + 6) ||
		pItem->Type == ITEM_HELPER + 4 ||
		pItem->Type == ITEM_HELPER + 5 ||
		pItem->Type == ITEM_HELPER + 30 ||
		(pItem->Type >= ITEM_WING + 36 && pItem->Type <= ITEM_WING + 43) ||
		((pItem->SetOption % 0x04) == EXT_A_SET_OPTION || (pItem->SetOption % 0x04) == EXT_B_SET_OPTION) ||
		pItem->Type == ITEM_SWORD + 19 ||
		pItem->Type == ITEM_STAFF + 10 ||
		pItem->Type == ITEM_BOW + 18 ||
		pItem->Type == ITEM_MACE + 13 ||
		pItem->Type == ITEM_HELPER + 14 ||
		pItem->Type == ITEM_HELPER + 15 ||
		pItem->Type == ITEM_HELPER + 19 ||
		pItem->Type == ITEM_HELPER + 31 ||
		(pItem->Type >= ITEM_POTION + 41 && pItem->Type <= ITEM_POTION + 44) ||
		(iLevel > 6 && pItem->Type < ITEM_WING) ||
		pItem->ExcOption > 0 ||
		(pItem->Type >= ITEM_HELPER + 34 && pItem->Type <= ITEM_HELPER + 37)
		|| pItem->Type == ITEM_HELPER + 52
		|| pItem->Type == ITEM_HELPER + 53
		|| pItem->Type == ITEM_POTION + 121
		|| pItem->Type == ITEM_POTION + 122
		|| pItem->Type == ITEM_WING + 130
		|| pItem->Type == ITEM_WING + 131
		|| pItem->Type == ITEM_WING + 132
		|| pItem->Type == ITEM_WING + 133
		|| pItem->Type == ITEM_WING + 134
		|| pItem->Type == ITEM_HELPER + 80
		|| pItem->Type == ITEM_HELPER + 76
		|| pItem->Type == ITEM_HELPER + 122
		|| pItem->Type == ITEM_HELPER + 123
		|| pItem->Type == ITEM_HELPER + 64
		|| pItem->Type == ITEM_HELPER + 65
		|| pItem->Type == ITEM_HELPER + 109
		|| pItem->Type == ITEM_HELPER + 110
		|| pItem->Type == ITEM_HELPER + 111
		|| pItem->Type == ITEM_HELPER + 112
		|| pItem->Type == ITEM_HELPER + 113
		|| pItem->Type == ITEM_HELPER + 114
		|| pItem->Type == ITEM_HELPER + 115
		|| pItem->Type == ITEM_POTION + 112
		|| pItem->Type == ITEM_POTION + 113
		|| (pItem->Type == ITEM_HELPER + 20 && iLevel == 0)
		|| (IsInvenItem(pItem->Type) && pItem->Durability == 255))
	{
		if (pItem->Type == ITEM_HELPER + 80
			|| pItem->Type == ITEM_HELPER + 76
			|| pItem->Type == ITEM_HELPER + 64
			|| pItem->Type == ITEM_HELPER + 65
			|| pItem->Type == ITEM_HELPER + 122
			|| pItem->Type == ITEM_HELPER + 123)
		{
			return false;
		}

		return true;
	}

	return false;
}

mu_boolean IsPersonalShopBan(ITEM* pItem)
{
	if (pItem == nullptr)
	{
		return false;
	}

	if (pItem->Type == ITEM_HELPER + 64
		|| pItem->Type == ITEM_HELPER + 65
		|| pItem->Type == ITEM_HELPER + 80
		|| pItem->Type == ITEM_HELPER + 76
		|| pItem->Type == ITEM_HELPER + 122
		|| pItem->Type == ITEM_HELPER + 123
		|| (IsInvenItem(pItem->Type) && pItem->Durability == 255)
		|| (pItem->Type == ITEM_HELPER + 20 && pItem->Level == 0))
	{
		return false;
	}

	if (pItem->Type == ITEM_HELPER + 38
		|| pItem->Type == ITEM_HELPER + 39
		|| (pItem->Type == ITEM_POTION + 21 && pItem->Level != 3)
		|| (pItem->Type >= ITEM_POTION + 23 && pItem->Type <= ITEM_POTION + 26)
		|| pItem->Type == ITEM_HELPER + 19
		|| (pItem->Type == ITEM_POTION + 11 && pItem->Level == 13)
		|| (pItem->Type >= ITEM_HELPER + 43 && pItem->Type <= ITEM_HELPER + 45)
		|| (pItem->Type == ITEM_HELPER + 20 && pItem->Level != 0)
		|| pItem->Type == ITEM_POTION + 65
		|| pItem->Type == ITEM_POTION + 66
		|| pItem->Type == ITEM_POTION + 67
		|| pItem->Type == ITEM_POTION + 68
		|| IsPartChargeItem(pItem)
		|| pItem->Type == ITEM_HELPER + 80
		|| pItem->Type == ITEM_HELPER + 76
		|| pItem->Type == ITEM_HELPER + 122
		|| pItem->Type == ITEM_HELPER + 123
		|| pItem->Type == ITEM_HELPER + 109
		|| pItem->Type == ITEM_HELPER + 110
		|| pItem->Type == ITEM_HELPER + 111
		|| pItem->Type == ITEM_HELPER + 112
		|| pItem->Type == ITEM_HELPER + 113
		|| pItem->Type == ITEM_HELPER + 114
		|| pItem->Type == ITEM_HELPER + 115
		|| IsInvenItem(pItem->Type))
	{
		return true;
	}

	return false;
}

mu_boolean IsTradeBan(ITEM* pItem)
{
	if (pItem->Type == ITEM_HELPER + 38
		|| pItem->Type == ITEM_HELPER + 39
		|| (pItem->Type == ITEM_POTION + 21 && pItem->Level != 3)
		|| (pItem->Type >= ITEM_POTION + 23 && pItem->Type <= ITEM_POTION + 26)
		|| pItem->Type == ITEM_HELPER + 19
		|| (pItem->Type == ITEM_POTION + 11 && pItem->Level == 13)
		|| (pItem->Type >= ITEM_HELPER + 43 && pItem->Type <= ITEM_HELPER + 45)
		|| (pItem->Type == ITEM_HELPER + 20 && pItem->Level != 0)
		|| pItem->Type == ITEM_POTION + 64
		|| pItem->Type == ITEM_POTION + 65
		|| pItem->Type == ITEM_POTION + 66
		|| pItem->Type == ITEM_POTION + 67
		|| pItem->Type == ITEM_POTION + 68
		|| IsPartChargeItem(pItem)
		|| pItem->Type == ITEM_HELPER + 80
		|| pItem->Type == ITEM_HELPER + 76
		|| pItem->Type == ITEM_HELPER + 122
		|| pItem->Type == ITEM_HELPER + 123
		|| IsInvenItem(pItem->Type))
	{
		return true;
	}

	if (pItem->Type == ITEM_POTION + 52)
	{
		if (g_isCharacterBuff((&Hero->Object), eBuff_GMEffect) ||
			(Hero->CtlCode == CTLCODE_ADMIN) || (Hero->CtlCode == CTLCODE_EVENT_GM))
			return false;
		else
			return true;
	}

	return false;
}

mu_boolean IsStoreBan(ITEM* pItem)
{
	if ((pItem->Type >= ITEM_POTION + 23 && pItem->Type <= ITEM_POTION + 26)
		|| (pItem->Type == ITEM_POTION + 21 && pItem->Level != 3)
		|| pItem->Type == ITEM_HELPER + 19
		|| (pItem->Type == ITEM_POTION + 11 && pItem->Level == 13)
		|| (pItem->Type >= ITEM_HELPER + 43 && pItem->Type <= ITEM_HELPER + 45)
		|| pItem->Type == ITEM_HELPER + 93
		|| pItem->Type == ITEM_HELPER + 94
		|| (pItem->Type == ITEM_HELPER + 20 && pItem->Level != 0)
		|| pItem->Type == ITEM_POTION + 65
		|| pItem->Type == ITEM_POTION + 66
		|| pItem->Type == ITEM_POTION + 67
		|| pItem->Type == ITEM_POTION + 68
		|| (pItem->Type == ITEM_HELPER + 70 && pItem->Durability == 1)
		|| (IsInvenItem(pItem->Type) && pItem->Durability == 254))
	{
		return true;
	}

	return false;
}

mu_boolean IsDropBan(ITEM* pItem)
{
	if (pItem->Type == ITEM_POTION + 96
		|| pItem->Type == ITEM_POTION + 54
		|| pItem->Type == ITEM_HELPER + 64
		|| pItem->Type == ITEM_HELPER + 65
		|| pItem->Type == ITEM_HELPER + 80
		|| pItem->Type == ITEM_HELPER + 76
		|| pItem->Type == ITEM_HELPER + 122
		|| pItem->Type == ITEM_HELPER + 123
		|| (pItem->Type == ITEM_HELPER + 20 && pItem->Level == 0))
	{
		return false;
	}

	if (pItem->Type == ITEM_POTION + 123
		|| pItem->Type == ITEM_POTION + 124)
	{
		return false;
	}

	if ((pItem->Type >= ITEM_POTION + 23 && pItem->Type <= ITEM_POTION + 26)
		|| (pItem->Type >= ITEM_POTION + 65 && pItem->Type <= ITEM_POTION + 68)
		|| IsPartChargeItem(pItem)
		|| ((pItem->Type >= ITEM_TYPE_CHARM_MIXWING + EWS_BEGIN)
			&& (pItem->Type <= ITEM_TYPE_CHARM_MIXWING + EWS_END))
		|| pItem->Type == ITEM_HELPER + 80
		|| pItem->Type == ITEM_HELPER + 76
		|| pItem->Type == ITEM_HELPER + 122
		|| pItem->Type == ITEM_HELPER + 123
		|| pItem->Type == ITEM_POTION + 121
		|| pItem->Type == ITEM_POTION + 122
		|| IsInvenItem(pItem->Type)
		|| pItem->Type == ITEM_HELPER + 42)
	{
		return true;
	}

	return false;
}

mu_boolean IsSellingBan(ITEM* pItem)
{
	mu_int32 Level = pItem->Level;

	if (pItem->Type == ITEM_POTION + 112
		|| pItem->Type == ITEM_POTION + 113
		|| pItem->Type == ITEM_POTION + 121
		|| pItem->Type == ITEM_POTION + 122
		|| pItem->Type == ITEM_POTION + 123
		|| pItem->Type == ITEM_POTION + 124
		|| pItem->Type == ITEM_WING + 130
		|| pItem->Type == ITEM_WING + 131
		|| pItem->Type == ITEM_WING + 132
		|| pItem->Type == ITEM_WING + 133
		|| pItem->Type == ITEM_WING + 134
		|| pItem->Type == ITEM_HELPER + 80
		|| pItem->Type == ITEM_HELPER + 76
		|| pItem->Type == ITEM_HELPER + 122
		|| pItem->Type == ITEM_HELPER + 123
		|| pItem->Type == ITEM_HELPER + 64
		|| pItem->Type == ITEM_HELPER + 65
		|| pItem->Type == ITEM_HELPER + 109
		|| pItem->Type == ITEM_HELPER + 110
		|| pItem->Type == ITEM_HELPER + 111
		|| pItem->Type == ITEM_HELPER + 112
		|| pItem->Type == ITEM_HELPER + 113
		|| pItem->Type == ITEM_HELPER + 114
		|| pItem->Type == ITEM_HELPER + 115
		|| (IsInvenItem(pItem->Type) && pItem->Durability != 254)
		|| ((pItem->Type == ITEM_HELPER + 20) && (Level == 0))
		|| (pItem->Type == ITEM_HELPER + 106)
		|| (pItem->Type == ITEM_HELPER + 107)
		)
	{
		return false;
	}

	if (pItem->Type == ITEM_POTION + 11
		|| (pItem->Type == ITEM_POTION + 21 && Level == 1)
		|| pItem->Type == ITEM_HELPER + 19
		|| (pItem->Type == ITEM_POTION + 20 && Level >= 1 && Level <= 5)
		|| IsPartChargeItem(pItem)
		|| ((pItem->Type >= ITEM_TYPE_CHARM_MIXWING + EWS_BEGIN)
			&& (pItem->Type <= ITEM_TYPE_CHARM_MIXWING + EWS_END))
		|| pItem->Type == ITEM_HELPER + 80
		|| pItem->Type == ITEM_HELPER + 76
		|| pItem->Type == ITEM_HELPER + 122
		|| pItem->Type == ITEM_HELPER + 123
		|| (IsInvenItem(pItem->Type))
		|| (pItem->Type == ITEM_HELPER + 106)
		|| (pItem->Type == ITEM_HELPER + 107)
		)
	{
		return true;
	}

	return false;
}

mu_boolean IsRepairBan(ITEM* pItem)
{
	if (IsPartChargeItem(pItem) == true)
	{
		return true;
	}

	if (pItem->Type >= ITEM_POTION)
	{
		return true;
	}

	if (pItem->Type == ITEM_BOW + 7
		|| pItem->Type == ITEM_BOW + 15)
	{
		return true;
	}

	if ((pItem->Type >= ITEM_WING && pItem->Type <= ITEM_WING + 6) ||
		(pItem->Type >= ITEM_WING + 36 && pItem->Type <= ITEM_WING + 43))
	{
		return false;
	}

	if (pItem->Type >= ITEM_WING &&
		pItem->Type < ITEM_HELPER)
	{
		return true;
	}

	if (IsInvenItem(pItem->Type) == true)
	{
		return true;
	}

	if ((pItem->Type >= ITEM_HELPER + 4 && pItem->Type <= ITEM_HELPER + 5) ||
		(pItem->Type >= ITEM_HELPER + 8 && pItem->Type <= ITEM_HELPER + 9) ||
		(pItem->Type >= ITEM_HELPER + 12 && pItem->Type <= ITEM_HELPER + 13) ||
		(pItem->Type >= ITEM_HELPER + 21 && pItem->Type <= ITEM_HELPER + 28) ||
		pItem->Type == ITEM_HELPER + 30)
	{
		return false;
	}

	if (pItem->Type >= ITEM_HELPER &&
		pItem->Type < ITEM_POTION)
	{
		return true;
	}

	return false;
}

mu_boolean IsWingItem(ITEM* pItem)
{
	switch (pItem->Type)
	{
	case ITEM_WING:
	case ITEM_WING + 1:
	case ITEM_WING + 2:
	case ITEM_WING + 3:
	case ITEM_WING + 4:
	case ITEM_WING + 5:
	case ITEM_WING + 6:
	case ITEM_HELPER + 30:
	case ITEM_WING + 36:
	case ITEM_WING + 37:
	case ITEM_WING + 38:
	case ITEM_WING + 39:
	case ITEM_WING + 40:
	case ITEM_WING + 41:
	case ITEM_WING + 42:
	case ITEM_WING + 43:
	case ITEM_WING + 130:
	case ITEM_WING + 131:
	case ITEM_WING + 132:
	case ITEM_WING + 133:
	case ITEM_WING + 134:
		return true;
	}

	return false;
}

mu_boolean IsCepterItem(mu_int32 iType)
{
	if ((iType >= ITEM_MACE + 8 && iType <= ITEM_MACE + 15)
		|| iType == ITEM_MACE + 17
		|| iType == ITEM_MACE + 18)
	{
		return true;
	}

	return false;
}

void GetItemName(mu_int32 iType, mu_int32 iLevel, mu_text *Text, mu_size TextCount)
{
	ITEM_ATTRIBUTE *p = &ItemAttribute[iType];

	if (iType >= ITEM_POTION + 23 && iType <= ITEM_POTION + 26)
	{
		if (iType == ITEM_POTION + 23)
		{
			switch (iLevel)
			{
			case 0: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
			case 1: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[906]); break;
			}
		}
		else if (iType == ITEM_POTION + 24)
		{
			switch (iLevel)
			{
			case 0: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str()); break;	
			case 1: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[907]); break;
			}
		}
		else
		{
			mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str());
		}
	}
	else if (iType == ITEM_POTION + 12)
	{
		switch (iLevel)
		{
		case 0: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[100]); break;
		case 1:	mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[101]); break;
		case 2:	mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[ChaosEventName[p->Durability]]); break;
		}
	}
	else if (iType == ITEM_POTION + 11)
	{
		switch (iLevel)
		{
		case 0: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
		case 1: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[105]); break;
		case 2: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[106]); break;
		case 3: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[107]); break;
		case 5: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[109]); break;
		case 6: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[110]); break;
		case 7: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[111]); break;
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
			mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" +%d"), GlobalText[115], iLevel - 7);
			break;
		case 13:
			mu_sprintf_s(Text, TextCount, GlobalText[117]); break;
		case 14:
			mu_sprintf_s(Text, TextCount, GlobalText[1650]); break;
		case 15:
			mu_sprintf_s(Text, TextCount, GlobalText[1651]); break;
		}
	}
	else if (iType == ITEM_HELPER + 15)
	{
		switch (iLevel)
		{
		case 0:mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[168], p->Name.c_str()); break;
		case 1:mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[169], p->Name.c_str()); break;
		case 2:mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[167], p->Name.c_str()); break;
		case 3:mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[166], p->Name.c_str()); break;
		case 4:mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[1900], p->Name.c_str()); break;
		}
	}
	else if (iType == ITEM_HELPER + 14)
	{
		switch (iLevel)
		{
		case 0: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
		case 1: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[1235]); break;
		}
	}
	else if (iType == ITEM_HELPER + 31)
	{
		switch (iLevel)
		{
		case 0: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[1187], p->Name.c_str()); break;
		case 1: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[1214], p->Name.c_str()); break;
		}
	}
	else if (iType == ITEM_POTION + 20)
	{
		switch (iLevel)
		{
		case 0: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" +%d"), GlobalText[116], iLevel); break;
		}
	}
	else if (iType == ITEM_POTION + 21)
	{
		switch (iLevel)
		{
		case 0: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
		case 1: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[810]); break;
		case 2: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[1098]); break;
		case 3: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[1290]); break;
		}
	}
	else if (iType == ITEM_HELPER + 19)
	{
		mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[809]);
	}
	else if (iType == ITEM_HELPER + 20)
	{
		switch (iLevel)
		{
		case 0: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
		case 1: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[922]); break;
		case 2: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[928]); break;
		case 3: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[929]); break;
		}
	}
	else if (iType == ITEM_POTION + 9)
	{
		switch (iLevel)
		{
		case 0: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
		case 1:	mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[108]); break;
		}
	}
	else if (iType == ITEM_WING + 11)
	{
		mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), SkillAttribute[30 + iLevel].Name, GlobalText[102]);
	}
	else if (iType == ITEM_WING + 32)
	{
		mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str());
	}
	else if (iType == ITEM_WING + 33)
	{
		mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str());
	}
	else if (iType == ITEM_WING + 34)
	{
		mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str());
	}
	else if (iType == ITEM_WING + 35)
	{
		mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str());
	}
	else if (iType >= ITEM_POTION + 45 && iType <= ITEM_POTION + 50)
	{
		mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str());
	}
	else if (iType == ITEM_POTION + 32)
	{
		switch (iLevel)
		{
		case 0: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
		case 1:	mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[2012]); break;
		}
	}
	else if (iType == ITEM_POTION + 33)
	{
		switch (iLevel)
		{
		case 0: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
		case 1:	mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[2013]); break;
		}
	}
	else if (iType == ITEM_POTION + 34)
	{
		switch (iLevel)
		{
		case 0: mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
		case 1:	mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[2014]); break;
		}
	}
	else if (iType == ITEM_HELPER + 10)
	{
		mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GetMonsterName(SommonTable[iLevel]), GlobalText[103]);
	}
	else if (iType >= ITEM_WING + 3 && iType <= ITEM_WING + 6)
	{
		if (iLevel == 0)
			mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str());
		else
			mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" +%d"), p->Name.c_str(), iLevel);
	}
	else if ((iType >= ITEM_WING + 36 && iType <= ITEM_WING + 40) || (iType >= ITEM_WING + 42 && iType <= ITEM_WING + 43))
	{
		if (iLevel == 0)
			mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str());
		else
			mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" +%d"), p->Name.c_str(), iLevel);
	}
	else if (iType == ITEM_SWORD + 19 || iType == ITEM_BOW + 18 || iType == ITEM_STAFF + 10 || iType == ITEM_MACE + 13)
	{
		if (iLevel == 0)
			mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str());
		else
			mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" +%d"), p->Name.c_str(), iLevel);
	}
	else if (iType == INDEX_COMPILED_CELE)
	{
		mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" +%d"), GlobalText[1806], iLevel + 1);
	}
	else if (iType == INDEX_COMPILED_SOUL)
	{
		mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" +%d"), GlobalText[1807], iLevel + 1);
	}
	else if ((iType >= ITEM_WING + 60 && iType <= ITEM_WING + 65)
		|| (iType >= ITEM_WING + 70 && iType <= ITEM_WING + 74)
		|| (iType >= ITEM_WING + 100 && iType <= ITEM_WING + 129))
	{
		mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str());
	}
	else if (iType == ITEM_POTION + 7)
	{
		mu_int32 iTextIndex = 0;

		iTextIndex = (iLevel == 0) ? 1413 : 1414;
		mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), GlobalText[iTextIndex]);
	}
	else
	{
		if (iLevel == 0)
			mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(""), p->Name.c_str());
		else
			mu_sprintf_s(Text, TextCount, _T("") SPF_STRING _T(" +%d"), p->Name.c_str(), iLevel);
	}
}

void RequireClass(ITEM_ATTRIBUTE* pItem)
{
	if (pItem == nullptr)
		return;

	mu_uint8 byFirstClass = GetBaseClass(Hero->Class);
	mu_uint8 byStepClass = GetStepClass(Hero->Class);

	mu_int32 iTextColor = 0;

	TextListColor[TextNum + 2] = TextListColor[TextNum + 3] = TEXT_COLOR_WHITE;
	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

	mu_int32 iCount = 0;
	for (mu_int32 i = 0; i < GAME::MAX_CLASS; ++i)
	{
		if (pItem->RequireClass[i] == 1)
		{
			++iCount;
		}
	}
	if (iCount == GAME::MAX_CLASS)
		return;

	for (mu_int32 i = 0; i < GAME::MAX_CLASS; ++i)
	{
		mu_uint8 byRequireClass = pItem->RequireClass[i];

		if (byRequireClass == 0)
			continue;

		if (i == byFirstClass && byRequireClass <= byStepClass)
		{
			iTextColor = TEXT_COLOR_WHITE;
		}
		else
		{
			iTextColor = TEXT_COLOR_DARKRED;
		}

		switch (i)
		{
		case GAME::CLASS_WIZARD:
			{
				if (byRequireClass == 1)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[20]);
					TextListColor[TextNum] = iTextColor;
				}
				else if (byRequireClass == 2)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[25]);
					TextListColor[TextNum] = iTextColor;
				}
				else if (byRequireClass == 3)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[1669]);
					TextListColor[TextNum] = iTextColor;
				}

				TextBold[TextNum] = false;	++TextNum;
			}
			break;
		case GAME::CLASS_KNIGHT:
			{
				if (byRequireClass == 1)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[21]);
					TextListColor[TextNum] = iTextColor;
				}
				else if (byRequireClass == 2)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[26]);
					TextListColor[TextNum] = iTextColor;
				}
				else if (byRequireClass == 3)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[1668]);
					TextListColor[TextNum] = iTextColor;
				}

				TextBold[TextNum] = false;	++TextNum;
			}
			break;
		case GAME::CLASS_ELF:
			{
				if (byRequireClass == 1)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[22]);
					TextListColor[TextNum] = iTextColor;
				}
				else if (byRequireClass == 2)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[27]);
					TextListColor[TextNum] = iTextColor;
				}
				else if (byRequireClass == 3)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[1670]);
					TextListColor[TextNum] = iTextColor;
				}

				TextBold[TextNum] = false;	++TextNum;
			}
			break;
		case GAME::CLASS_DARK:
			{
				if (byRequireClass == 1)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[23]);
					TextListColor[TextNum] = iTextColor;
				}
				else if (byRequireClass == 3)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[1671]);
					TextListColor[TextNum] = iTextColor;
				}

				TextBold[TextNum] = false;	++TextNum;
			}
			break;
		case GAME::CLASS_DARK_LORD:
			{
				if (byRequireClass == 1)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[24]);
					TextListColor[TextNum] = iTextColor;
				}
				else if (byRequireClass == 3)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[1672]);
					TextListColor[TextNum] = iTextColor;
				}

				TextBold[TextNum] = false;	++TextNum;
			}
			break;
		case GAME::CLASS_SUMMONER:
			{
				if (byRequireClass == 1)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[1687]);
					TextListColor[TextNum] = iTextColor;
				}
				else if (byRequireClass == 2)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[1688]);
					TextListColor[TextNum] = iTextColor;
				}
				else if (byRequireClass == 3)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[1689]);
					TextListColor[TextNum] = iTextColor;
				}

				TextBold[TextNum] = false;	++TextNum;
			}
			break;
		}
	}
}

void RenderItemInfo(mu_int32 sx, mu_int32 sy, ITEM *ip, mu_boolean Sell, mu_boolean useAlternativePos, mu_int32 Inventype, mu_boolean bItemTextListBoxUse, mu_boolean bIsEquippedItem, mu_boolean useBG)
{
	if (ip->Type == -1)
		return;

	ITEM_ATTRIBUTE *p = &ItemAttribute[ip->Type];
	TextNum = 0;
	SkipNum = 0;

	if (ip->Type == ITEM_HELPER + 4 || ip->Type == ITEM_HELPER + 5)
	{
#if 0 // FIX
		mu_uint8 PetType;
		if (ip->Type == ITEM_HELPER + 4)
		{
			PetType = PET_TYPE_DARK_HORSE;
			if ((g_pMyInventory->GetPointedItemIndex()) == EQUIPMENT_HELPER)
				SendRequestPetInfo(PetType, Inventype, EQUIPMENT_HELPER);
		}
		else
		{
			PetType = PET_TYPE_DARK_SPIRIT
			if ((g_pMyInventory->GetPointedItemIndex()) == EQUIPMENT_WEAPON_LEFT)
				SendRequestPetInfo(PetType, Inventype, EQUIPMENT_WEAPON_LEFT);
		}
#endif

		giPetManager::RenderPetItemInfo(sx, sy, ip, useAlternativePos, Inventype);
		return;
	}

	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

	mu_int32 Level = ip->Level;
	mu_int32 Color;

	if (ip->Type == ITEM_POTION + 13 || ip->Type == ITEM_POTION + 14 || ip->Type == ITEM_WING + 15 || ip->Type == ITEM_POTION + 31 ||
		(COMGEM::isCompiledGem(ip)) ||
		ip->Type == ITEM_POTION + 65 || ip->Type == ITEM_POTION + 66 || ip->Type == ITEM_POTION + 67 ||
		ip->Type == ITEM_POTION + 68 ||
		ip->Type == ITEM_HELPER + 52 || ip->Type == ITEM_HELPER + 53 ||
		ip->Type == ITEM_POTION + 100 ||
		(ip->Type >= ITEM_POTION + 141 && ip->Type <= ITEM_POTION + 144) ||
		ip->Type == ITEM_POTION + 16 || ip->Type == ITEM_POTION + 22)
	{
		Color = TEXT_COLOR_YELLOW;
	}
	else if (ip->Type == ITEM_STAFF + 10 || ip->Type == ITEM_SWORD + 19 || ip->Type == ITEM_BOW + 18 || ip->Type == ITEM_MACE + 13)
	{
		Color = TEXT_COLOR_PURPLE;
	}
	else if (ip->Type == ITEM_POTION + 17 || ip->Type == ITEM_POTION + 18 || ip->Type == ITEM_POTION + 19)
	{
		Color = TEXT_COLOR_YELLOW;
	}
	else if (ip->Type == ITEM_HELPER + 16 || ip->Type == ITEM_HELPER + 17)
	{
		Color = TEXT_COLOR_YELLOW;
	}
	else if ((ip->SetOption % 0x04) == EXT_A_SET_OPTION || (ip->SetOption % 0x04) == EXT_B_SET_OPTION)
	{
		Color = TEXT_COLOR_GREEN_BLUE;
	}
	else if (g_SocketItemMgr.IsSocketItem(ip))
	{
		Color = TEXT_COLOR_VIOLET;
	}
	else if (ip->SpecialNum > 0 && ip->ExcOption > 0)
	{
		Color = TEXT_COLOR_GREEN;
	}
	else if (Level >= 7)
	{
		Color = TEXT_COLOR_YELLOW;
	}
	else
	{
		if (ip->SpecialNum > 0)
		{
			Color = TEXT_COLOR_BLUE;
		}
		else
		{
			Color = TEXT_COLOR_WHITE;
		}
	}

	if ((ip->Type >= ITEM_WING + 3 && ip->Type <= ITEM_WING + 6) || ip->Type == ITEM_HELPER + 30
		|| (ip->Type >= ITEM_WING + 36 && ip->Type <= ITEM_WING + 40)
		|| (ip->Type >= ITEM_WING + 42 && ip->Type <= ITEM_WING + 43))
	{
		if (Level >= 7)
		{
			Color = TEXT_COLOR_YELLOW;
		}
		else
		{
			if (ip->SpecialNum > 0)
			{
				Color = TEXT_COLOR_BLUE;
			}
			else
			{
				Color = TEXT_COLOR_WHITE;
			}
		}
	}

	if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_NPCSHOP) && !IsSellingBan(ip))
	{
		mu_unicode Text[128];

#if 0 // FIX
		CPCRoomPtSys& rPCRoomPtSys = CPCRoomPtSys::Instance();
		if (rPCRoomPtSys.IsPCRoomPointShopMode())
		{
			if (Sell)
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2331], ItemValue(ip, 0));
				TextListColor[TextNum] = Color;
				++TextNum;

				::mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n"));
				++TextNum;
				++SkipNum;
			}
		}
		else
#endif
		{
			if (Sell)
			{
				mu_uint32 dwValue = ItemValue(ip, 0);
				ConvertGold(dwValue, Text, mu_countof(Text));
				mu_unicode Text2[128];

				ConvertTaxGold(ItemValue(ip, 0), Text2, mu_countof(Text2));
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1620], Text2, Text);
			}
			else
			{
				ConvertGold(ItemValue(ip, 1), Text, mu_countof(Text));
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[63], Text);
			}

			TextListColor[TextNum] = Color;
			++TextNum;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;
		}
	}

	if ((Inventype == TOOLTIP_TYPE_MY_SHOP || Inventype == TOOLTIP_TYPE_PURCHASE_SHOP)
		&& !IsPersonalShopBan(ip))
	{
		mu_int32 price = 0;
		mu_unicode Text[100];

#if 0 // FIX
		mu_int32 indexInv = (GAME::MAX_EQUIPMENT + MAX_INVENTORY) + (ip->y*COL_PERSONALSHOP_INVEN) + ip->x;
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
			if (gold < price && g_IsPurchaseShop == PSHOPWNDTYPE_PURCHASE)
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

	if (ip->Type >= ITEM_POTION + 23 && ip->Type <= ITEM_POTION + 26)
	{
		if (ip->Type == ITEM_POTION + 23)
		{
			Color = TEXT_COLOR_YELLOW;
			switch (Level)
			{
			case 0: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
			case 1: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[906]); break;
			}
		}
		else if (ip->Type == ITEM_POTION + 24)
		{
			Color = TEXT_COLOR_YELLOW;
			switch (Level)
			{
			case 0: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
			case 1: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[907]); break;
			}
		}
		else
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
			Color = TEXT_COLOR_YELLOW;
		}
	}
	else if (ip->Type == ITEM_POTION + 12)
	{
		switch (Level)
		{
		case 0:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[100]); break;
		case 1:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[101]); break;
		case 2:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), GlobalText[ChaosEventName[ip->Durability]]); break;
		}
	}
	else if (ip->Type == ITEM_POTION + 11)
	{
		switch (Level)
		{
		case 0:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
		case 1:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[105]); break;
		case 2:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[106]); break;
		case 3:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[107]); break;
		case 5:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[109]); break;
		case 6:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[110]); break;
		case 7:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[111]); break;
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" +%d"), GlobalText[115], Level - 7);
			break;
		case 13:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[117]);
			break;
		case 14:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1650]);
			break;
		case 15:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1651]);
			break;
		}
	}
	else if (ip->Type == ITEM_POTION + 12)
	{
		switch (Level)
		{
		case 0:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[100]); break;
		case 1:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[101]); break;
		case 2:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), GlobalText[ChaosEventName[ip->Durability]]); break;
		}
	}
	else if (ip->Type == ITEM_HELPER + 15)
	{
		Color = TEXT_COLOR_YELLOW;
		switch (Level)
		{
		case 0:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[168], p->Name.c_str()); break;
		case 1:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[169], p->Name.c_str()); break;
		case 2:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[167], p->Name.c_str()); break;
		case 3:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[166], p->Name.c_str()); break;
		case 4:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[1900], p->Name.c_str()); break;
		}
	}
	else if (ip->Type == ITEM_HELPER + 14)
	{
		Color = TEXT_COLOR_YELLOW;
		switch (Level)
		{
		case 0: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
		case 1: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), GlobalText[1235]); break;
		}
	}
	else if (ip->Type == ITEM_POTION + 21)
	{
		Color = TEXT_COLOR_YELLOW;
		switch (Level)
		{
		case 0: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
		case 1: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[810]); break;
		case 3: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1290]); break;
		}
	}
	else if (ip->Type == ITEM_HELPER + 19)
	{
		Color = TEXT_COLOR_YELLOW;
		switch (Level)
		{
		case 0: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[811]); break;
		case 1: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[812]); break;
		case 2: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[817]); break;
		}
	}
	else if (ip->Type == ITEM_HELPER + 20)
	{
		Color = TEXT_COLOR_YELLOW;
		switch (Level)
		{
		case 0: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, p->Name.c_str()); break;
		case 1: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[922]); break;
		case 2: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[928]); break;
		case 3: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[929]); break;
		}
	}
	else if (ip->Type == ITEM_HELPER + 107)
	{
		Color = TEXT_COLOR_YELLOW;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, p->Name.c_str());
	}
	else if (ip->Type == ITEM_POTION + 7)
	{
		switch (Level)
		{
		case 0: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1413]); break;
		case 1: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1414]); break;
		}
	}
	else if (ip->Type == ITEM_HELPER + 7)
	{
		switch (Level)
		{
		case 0: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1460]); break;
		case 1: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1461]); break;
		}
	}
	else if (ip->Type == ITEM_HELPER + 11)
	{
		switch (Level)
		{
		case 0: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1416]); break;
		case 1: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1462]); break;
		}
	}
	else if (ip->Type == ITEM_POTION + 9)
	{
		switch (Level)
		{
		case 0:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
		case 1:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[108]); break;
		}
	}
	else if (ip->Type == ITEM_WING + 11)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), SkillAttribute[30 + Level].Name, GlobalText[102]);
	}
	else if (ip->Type == ITEM_HELPER + 10)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GetMonsterName(SommonTable[Level]), GlobalText[103]);
	}
	else if (ip->Type >= ITEM_WING + 3 && ip->Type <= ITEM_WING + 6)
	{
		if (Level == 0)
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
		else
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" +%d"), p->Name.c_str(), Level);
	}
	else if ((ip->Type >= ITEM_WING + 36 && ip->Type <= ITEM_WING + 40) || (ip->Type >= ITEM_WING + 42 && ip->Type <= ITEM_WING + 43))
	{
		if (Level == 0)
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
		else
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" +%d"), p->Name.c_str(), Level);
	}
	else if (ip->Type == ITEM_HELPER + 31)
	{
		switch (Level)
		{
		case 0: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" of ") SPF_STRING _T(""), p->Name.c_str(), GlobalText[1187]); break;
		case 1: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" of ") SPF_STRING _T(""), p->Name.c_str(), GlobalText[1214]); break;
		}
	}
	else if (ip->Type == ITEM_HELPER + 30)
	{
		if (Level == 0)
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
		else
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" +%d"), p->Name.c_str(), Level);
	}
	else if (ip->Type == ITEM_POTION + 29)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1180], Level);
	}
	else if (ip->Type == ITEM_POTION + 28)
	{
		Color = TEXT_COLOR_YELLOW;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" +%d"), p->Name.c_str(), Level);
	}
	else if (ip->Type == ITEM_WING + 32)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
	}
	else if (ip->Type == ITEM_WING + 33)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
	}
	else if (ip->Type == ITEM_WING + 34)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
	}
	else if (ip->Type == ITEM_WING + 35)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
	}
	else if (ip->Type >= ITEM_POTION + 45 && ip->Type <= ITEM_POTION + 50)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
	}
	else if (ip->Type == ITEM_POTION + 32)
	{
		switch (Level)
		{
		case 0:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
		case 1:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), GlobalText[2012]); break;
		}
	}
	else if (ip->Type == ITEM_POTION + 33)
	{
		switch (Level)
		{
		case 0:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
		case 1:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), GlobalText[2013]); break;
		}
	}
	else if (ip->Type == ITEM_POTION + 34)
	{
		switch (Level)
		{
		case 0:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str()); break;
		case 1:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), GlobalText[2014]); break;
		}
	}
	else if (ip->Type >= ITEM_HELPER + 32 && ip->Type <= ITEM_HELPER + 37)
	{
		if (ip->Type == ITEM_HELPER + 37)
		{
			Color = TEXT_COLOR_BLUE;
			if (ip->ExcOption == 0x01)
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), p->Name.c_str(), GlobalText[1863]);
			else if (ip->ExcOption == 0x02)
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), p->Name.c_str(), GlobalText[1864]);
			else if (ip->ExcOption == 0x04)
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), p->Name.c_str(), GlobalText[1866]);
			else
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
		}
		else
		{
			Color = TEXT_COLOR_WHITE;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
		}
	}
	else if (ip->Type == ITEM_SWORD + 19 || ip->Type == ITEM_BOW + 18 || ip->Type == ITEM_STAFF + 10 || ip->Type == ITEM_MACE + 13)
	{
		if (Level == 0)
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
		else
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" +%d"), p->Name.c_str(), Level);
	}
	else if (ip->Type == INDEX_COMPILED_CELE)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" +%d"), GlobalText[1806], Level + 1);
	}
	else if (ip->Type == INDEX_COMPILED_SOUL)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" +%d"), GlobalText[1807], Level + 1);
	}
	else if (ip->Type == ITEM_POTION + 41 || ip->Type == ITEM_POTION + 42 ||
		ip->Type == ITEM_POTION + 43 || ip->Type == ITEM_POTION + 44 ||
		ip->Type == ITEM_HELPER + 38
		)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
		Color = TEXT_COLOR_YELLOW;
	}
	else if ((ip->Type >= ITEM_WING + 60 && ip->Type <= ITEM_WING + 65)
		|| (ip->Type >= ITEM_WING + 70 && ip->Type <= ITEM_WING + 74)
		|| (ip->Type >= ITEM_WING + 100 && ip->Type <= ITEM_WING + 129))
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
		Color = TEXT_COLOR_VIOLET;
	}
	else if (ip->Type == ITEM_POTION + 111)
	{
		Color = TEXT_COLOR_YELLOW;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
	}
	else if (ITEM_POTION + 101 <= ip->Type && ip->Type <= ITEM_POTION + 109)
	{
		Color = TEXT_COLOR_YELLOW;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
	}
	else
	{
		mu_unicode TextName[64];
		if (g_csItemOption.GetSetItemName(TextName, ip->Type, ip->SetOption))
		{
			mu_strcat(TextName, mu_countof(TextName), p->Name.c_str());
		}
		else
		{
			mu_strcpy_s(TextName, mu_countof(TextName), p->Name.c_str());
		}

		if (ip->ExcOption > 0)
		{
			if (Level == 0)
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[620], TextName);
			else
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(" +%d"), GlobalText[620], TextName, Level);
		}
		else
		{
			if (Level == 0)
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), TextName);
			else
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" +%d"), TextName, Level);
		}
	}

	TextListColor[TextNum] = Color; TextBold[TextNum] = true; ++TextNum;
	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

	if (ip->Type == ITEM_HELPER + 19)
	{
		mu_int32 iMana;
		mu_int32 iWeaponSpeed;
		mu_int32 iNeedStrength;
		mu_int32 iNeedDex;

		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[730]); TextBold[TextNum] = false; ++TextNum;

		TextListColor[TextNum] = TEXT_COLOR_DARKRED;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[815]); TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); TextBold[TextNum] = false; ++TextNum; ++SkipNum;

		switch (Level)
		{
		case 0:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(": %d ~ %d"), GlobalText[42], 107, 110);  TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
			iWeaponSpeed = 20;
			iNeedStrength = 132;
			iNeedDex = 32;
			break;
		case 1:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(": %d ~ %d"), GlobalText[40], 110, 120);  TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
			iWeaponSpeed = 35;
			iNeedStrength = 381;
			iNeedDex = 149;
			break;
		case 2:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(": %d ~ %d"), GlobalText[41], 120, 140);  TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
			iWeaponSpeed = 35;
			iNeedStrength = 140;
			iNeedDex = 350;
			break;
		}

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[64], iWeaponSpeed); TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[73], iNeedStrength); TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[75], iNeedDex); TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); TextBold[TextNum] = false; ++TextNum; ++SkipNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[87]);     TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[94], 20); TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;

		switch (Level)
		{
		case 0:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[79], 53); TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = true; ++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[631]); TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[632], 2); TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;
			}
			break;
		case 1:
			{
				GetSkillInformation(AT_SKILL_SWORD4, 1, nullptr, 0, &iMana, nullptr);
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[84], iMana); TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[629]); TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[630], 2); TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;
			}
			break;
		case 2:
			{
				GetSkillInformation(AT_SKILL_CROSSBOW, 1, nullptr, 0, &iMana, nullptr);
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[86], iMana); TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[629]); TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[630], 2); TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;
			}
			break;
		}
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[628]); TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[633], 7); TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[634]); TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[635]); TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;

	}

	if (ip->Type >= ITEM_POTION + 23 && ip->Type <= ITEM_POTION + 26)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[730]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[731]);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[732]);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 12)
	{
		if (Level <= 1)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[119]);
			TextListColor[TextNum] = TEXT_COLOR_WHITE;
			TextBold[TextNum] = false;
			++TextNum;
		}
	}
	else if (ip->Type >= ITEM_HELPER + 46 && ip->Type <= ITEM_HELPER + 48)
	{
		mu_int32 iMap = 0;
		if (ip->Type == ITEM_HELPER + 46)
			iMap = 39;
		else if (ip->Type == ITEM_HELPER + 47)
			iMap = 56;
		else if (ip->Type == ITEM_HELPER + 48)
			iMap = 58;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2259], GlobalText[iMap]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2270]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n"));
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type >= ITEM_HELPER + 125 && ip->Type <= ITEM_HELPER + 127)
	{
		mu_int32 iMap = 0;
		if (ip->Type == ITEM_HELPER + 125)
			iMap = 3057;
		else if (ip->Type == ITEM_HELPER + 126)
			iMap = 2806;
		else if (ip->Type == ITEM_HELPER + 127)
			iMap = 3107;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2259], GlobalText[iMap]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n"));
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 54)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2261]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type >= ITEM_POTION + 58 && ip->Type <= ITEM_POTION + 62)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2269]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 83)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2269]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type >= ITEM_POTION + 145 && ip->Type <= ITEM_POTION + 150)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2269]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 53)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2250]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 43)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2256]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2297]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2567]);
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2568]);
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 44)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2257]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2297]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2567]);
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2568]);
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 45)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2258]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2297]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2566]);
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type >= ITEM_POTION + 70 && ip->Type <= ITEM_POTION + 71)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[69], ip->Durability);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_int32 index = ip->Type - (ITEM_POTION + 70);

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2500 + index]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type >= ITEM_POTION + 72 && ip->Type <= ITEM_POTION + 77)
	{
		const ITEM_ADD_OPTION& Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ip->Type);

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2503 + (ip->Type - (ITEM_POTION + 72))], Item_data.m_byValue1);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2502]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 59)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2509]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type >= ITEM_HELPER + 54 && ip->Type <= ITEM_HELPER + 58)
	{
		mu_uint32 statpoint = 0;
		statpoint = ip->Durability * 10;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2511], statpoint);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2510]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		TextListColor[TextNum] = TEXT_COLOR_DARKRED;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1908]);
		++TextNum;

		if (ip->Type == ITEM_HELPER + 58)
		{
			if (GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD)
			{
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
			}
			else
			{
				TextListColor[TextNum] = TEXT_COLOR_DARKRED;
			}

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[24]);
			++TextNum;
		}
	}
	else if (ip->Type >= ITEM_POTION + 78 && ip->Type <= ITEM_POTION + 82)
	{
		mu_int32 index = ip->Type - (ITEM_POTION + 78);
		mu_uint32 value = 0;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[69], ip->Durability);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		const ITEM_ADD_OPTION& Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ip->Type);
		value = Item_data.m_byValue1;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2512 + index], value);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2517]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2518]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		if (ip->Type == ITEM_POTION + 82)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3115]);
			TextListColor[TextNum] = TEXT_COLOR_YELLOW;
			TextBold[TextNum] = false;
			++TextNum;
		}

		mu_string timetext;
		g_StringTime(Item_data.m_Time, timetext, true);
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, timetext.c_str());
		TextListColor[TextNum] = TEXT_COLOR_PURPLE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2302]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 60)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2519]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 62)
	{
		const ITEM_ADD_OPTION& Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ip->Type);

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2253], Item_data.m_byValue1);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2569], Item_data.m_byValue2);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2570]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2566]);
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 63)
	{
		const ITEM_ADD_OPTION& Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ip->Type);

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2254], Item_data.m_byValue1);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2571], Item_data.m_byValue2);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2572]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 97)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2580]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2502]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 98)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2581]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2502]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 140)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[92], 3);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2188], 100);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 96)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2573]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2574]);
		TextListColor[TextNum] = TEXT_COLOR_PURPLE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2708]);
		TextListColor[TextNum] = TEXT_COLOR_PURPLE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 64)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2575]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2576]);
		TextListColor[TextNum] = TEXT_COLOR_PURPLE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 65)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2577]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2578]);
		TextListColor[TextNum] = TEXT_COLOR_PURPLE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 67)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2600]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 123)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2600]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3068]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3069]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3070]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 80)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2746]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2747]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2748]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 106)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2746]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2744]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2748]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 107)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[926]);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 104)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2968]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 105)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2969]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 103)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2970], 170);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 69)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2602]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
#if 0 // FIX
		if (g_PortalMgr.IsRevivePositionSaved())
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2603]);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false;
			++TextNum;
			g_PortalMgr.GetRevivePositionText(TextList[TextNum]);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false;
			++TextNum;
		}
#endif
	}
	else if (ip->Type == ITEM_HELPER + 70)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2604]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 81)
	{
		::mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2714]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum++] = false;
		::mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2729]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum++] = false;
		::mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3084]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum++] = false;
	}
	else if (ip->Type == ITEM_HELPER + 82)
	{
		::mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2715]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum++] = false;
		::mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2730]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum++] = false;
		::mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2716]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum++] = false;
	}
	else if (ip->Type == ITEM_HELPER + 93)
	{
		::mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2256]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum++] = false;
		::mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2297]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum++] = false;
	}
	else if (ip->Type == ITEM_HELPER + 94)
	{
		::mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2257]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum++] = false;
		::mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2297]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum++] = false;
	}
	else if (ip->Type == ITEM_HELPER + 61)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2259], GlobalText[2369]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2270]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n"));
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 91)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2551]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 92)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2261]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2553]);
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 93)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2261]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2556]);
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 95)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2261]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2552]);
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 94)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[69], ip->Durability);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2559]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 84)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2011]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 85)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2549]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_string timetext;
		const ITEM_ADD_OPTION& Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ip->Type);
		g_StringTime(Item_data.m_Time, timetext, true);
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, timetext.c_str());
		TextListColor[TextNum] = TEXT_COLOR_PURPLE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 86)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2550]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_string timetext;
		const ITEM_ADD_OPTION& Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ip->Type);
		g_StringTime(Item_data.m_Time, timetext, true);
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, timetext.c_str());
		TextListColor[TextNum] = TEXT_COLOR_PURPLE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 87)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2532]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_string timetext;
		const ITEM_ADD_OPTION& Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ip->Type);
		g_StringTime(Item_data.m_Time, timetext, true);
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, timetext.c_str());
		TextListColor[TextNum] = TEXT_COLOR_PURPLE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 88)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[69], ip->Durability);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2534]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2535]);
		TextListColor[TextNum] = TEXT_COLOR_DARKYELLOW;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 89)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[69], ip->Durability);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2534]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2536]);
		TextListColor[TextNum] = TEXT_COLOR_DARKYELLOW;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 90)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[69], ip->Durability);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2534]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2537]);
		TextListColor[TextNum] = TEXT_COLOR_DARKYELLOW;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 49)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2397]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 50)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2398]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 51)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2399]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 64)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2420]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type >= ITEM_POTION + 65 && ip->Type <= ITEM_POTION + 68)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[730]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[731]);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[732]);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 52)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1665]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 53)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1665]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if ((ip->Type >= ITEM_WING + 60 && ip->Type <= ITEM_WING + 65)
		|| (ip->Type >= ITEM_WING + 70 && ip->Type <= ITEM_WING + 74)
		|| (ip->Type >= ITEM_WING + 100 && ip->Type <= ITEM_WING + 129))
	{
		TextNum = g_SocketItemMgr.AttachToolTipForSeedSphereItem(ip, TextNum);
	}
	else if (ip->Type == ITEM_HELPER + 71 || ip->Type == ITEM_HELPER + 72 || ip->Type == ITEM_HELPER + 73 || ip->Type == ITEM_HELPER + 74 || ip->Type == ITEM_HELPER + 75)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2709]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 38)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[926]);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 20)
	{
		switch (Level)
		{
		case 0:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[926]);
				TextListColor[TextNum] = TEXT_COLOR_RED;
				TextBold[TextNum] = false;
				++TextNum;
			}
			break;

		case 1:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[924], 40);
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
				TextBold[TextNum] = false;
				++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[731]);
				TextListColor[TextNum] = TEXT_COLOR_RED;
				TextBold[TextNum] = false;
				++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[732]);
				TextListColor[TextNum] = TEXT_COLOR_RED;
				TextBold[TextNum] = false;
				++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[733]);
				TextListColor[TextNum] = TEXT_COLOR_RED;
				TextBold[TextNum] = false;
				++TextNum;
			}
			break;
		case 2:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[924], 80);
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
				TextBold[TextNum] = false;
				++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[731]);
				TextListColor[TextNum] = TEXT_COLOR_RED;
				TextBold[TextNum] = false;
				++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[732]);
				TextListColor[TextNum] = TEXT_COLOR_RED;
				TextBold[TextNum] = false;
				++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[733]);
				TextListColor[TextNum] = TEXT_COLOR_RED;
				TextBold[TextNum] = false;
				++TextNum;
			}
			break;
		case 3:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[926]);
				TextListColor[TextNum] = TEXT_COLOR_RED;
				TextBold[TextNum] = false;
				++TextNum;
			}
			break;
		}
	}
	else if (ip->Type >= ITEM_TYPE_CHARM_MIXWING + EWS_BEGIN
		&& ip->Type <= ITEM_TYPE_CHARM_MIXWING + EWS_END)
	{
		const ITEM_ADD_OPTION& Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ip->Type);

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2717]);
		TextBold[TextNum] = false;
		++TextNum;

		switch (ip->Type)
		{
		case ITEM_TYPE_CHARM_MIXWING + EWS_KNIGHT_1_CHARM:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2718], Item_data.m_byValue1);
			}break;
		case ITEM_TYPE_CHARM_MIXWING + EWS_MAGICIAN_1_CHARM:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2720], Item_data.m_byValue1);
			}break;
		case ITEM_TYPE_CHARM_MIXWING + EWS_ELF_1_CHARM:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2722], Item_data.m_byValue1);
			}break;
		case ITEM_TYPE_CHARM_MIXWING + EWS_SUMMONER_1_CHARM:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2724], Item_data.m_byValue1);
			}break;
		case ITEM_TYPE_CHARM_MIXWING + EWS_DARKLORD_1_CHARM:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2727], Item_data.m_byValue1);
			}break;
		case ITEM_TYPE_CHARM_MIXWING + EWS_KNIGHT_2_CHARM:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2719], Item_data.m_byValue1);
			}break;
		case ITEM_TYPE_CHARM_MIXWING + EWS_MAGICIAN_2_CHARM:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2721], Item_data.m_byValue1);
			}break;
		case ITEM_TYPE_CHARM_MIXWING + EWS_ELF_2_CHARM:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2723], Item_data.m_byValue1);
			}break;
		case ITEM_TYPE_CHARM_MIXWING + EWS_SUMMONER_2_CHARM:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2725], Item_data.m_byValue1);
			}break;
		case ITEM_TYPE_CHARM_MIXWING + EWS_DARKKNIGHT_2_CHARM:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2726], Item_data.m_byValue1);
			}break;
		}

		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 110)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2773]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2774]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2775]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1181], ip->Durability, 5);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2776], 5 - ip->Durability);
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 111)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2777]);
		TextListColor[TextNum] = TEXT_COLOR_DARKBLUE;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2778]);
		TextListColor[TextNum] = TEXT_COLOR_DARKBLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ITEM_POTION + 101 <= ip->Type && ip->Type <= ITEM_POTION + 109)
	{
		switch (ip->Type)
		{
		case ITEM_POTION + 101:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1181], ip->Durability, 5);
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
				TextBold[TextNum] = false;
				++TextNum;

				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2788]);
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
				TextBold[TextNum] = false;
				++TextNum;
			}break;
		case ITEM_POTION + 102:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2784]);
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
				TextBold[TextNum] = false;
				++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2785]);
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
				TextBold[TextNum] = false;
				++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2786]);
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
				TextBold[TextNum] = false;
				++TextNum;
			}break;
		case ITEM_POTION + 103:
		case ITEM_POTION + 104:
		case ITEM_POTION + 105:
		case ITEM_POTION + 106:
		case ITEM_POTION + 107:
		case ITEM_POTION + 108:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2790]);
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
				TextBold[TextNum] = false;
				++TextNum;
			}break;
		case ITEM_POTION + 109:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2792]);
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
				TextBold[TextNum] = false;
				++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2793]);
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
				TextBold[TextNum] = false;
				++TextNum;
			}break;
		}
	}
	else if (ITEM_HELPER + 109 == ip->Type)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[92], 3);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3058], 4);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ITEM_HELPER + 110 == ip->Type)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[92], 3);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[622], 4);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ITEM_HELPER + 111 == ip->Type)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[92], 3);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[627], 50);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ITEM_HELPER + 112 == ip->Type)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[92], 3);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[624], 4);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ITEM_HELPER + 113 == ip->Type)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[92], 3);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[628], 10);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ITEM_HELPER + 114 == ip->Type)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[92], 3);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2229], 7);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ITEM_HELPER + 115 == ip->Type)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[92], 3);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[635]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ITEM_POTION + 112 == ip->Type)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2876]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ITEM_POTION + 113 == ip->Type)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2875]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 120)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2971]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ITEM_POTION + 121 == ip->Type)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2877]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ITEM_POTION + 122 == ip->Type)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2878]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ITEM_POTION + 123 == ip->Type)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2879]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ITEM_POTION + 124 == ip->Type)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2880]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ITEM_POTION + 134 <= ip->Type && ITEM_POTION + 139 >= ip->Type)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2972]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ITEM_HELPER + 116 == ip->Type)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3018]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;

		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ITEM_HELPER + 121 == ip->Type)
	{
		mu_int32 iMap = 57;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2259], GlobalText[iMap]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2270]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2260], ip->Durability);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 124)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3116]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3127]);
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		TextBold[TextNum] = true;
		++TextNum;
	}
	else if (ip->Type >= ITEM_POTION + 141 && ip->Type <= ITEM_POTION + 144)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[571]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 133)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[69], ip->Durability);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3267]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}

	Color = TEXT_COLOR_YELLOW;
	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());

	if (ip->Type == ITEM_POTION + 19)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[638]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[639]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}

	if (ip->DamageMin > 0)
	{
		mu_int32 minindex = 0, maxindex = 0, magicalindex = 0;

		if (Level >= ip->HarmonyOptionLevel)
		{
			StrengthenCapability SC;
			g_pUIJewelHarmonyinfo.GetStrengthenCapability(&SC, ip, 1);

			if (SC.SI_isSP)
			{
				minindex = SC.SI_SP.SI_minattackpower;
				maxindex = SC.SI_SP.SI_maxattackpower;
				magicalindex = SC.SI_SP.SI_magicalpower;
			}
		}
		mu_int32 DamageMin = ip->DamageMin;
		mu_int32 DamageMax = ip->DamageMax;
		if (ip->Type != ITEM_ETC + 5 && ip->Type != ITEM_ETC + 14 && ip->Type != ITEM_ETC + 15)
		{
			if (ip->Type >= ITEM_ETC && ip->Type<ITEM_ETC + MAX_ITEM_INDEX)
			{
				mu_int32 SkillIndex = getSkillIndexByBook(ip->Type);

				SKILL_ATTRIBUTE *skillAtt = &SkillAttribute[SkillIndex];

				DamageMin = skillAtt->Damage;
				DamageMax = skillAtt->Damage + skillAtt->Damage / 2;

				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(": %d ~ %d"), GlobalText[42], DamageMin, DamageMax);
			}
			else
			{
				if (DamageMin + minindex >= DamageMax + maxindex)
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(": %d ~ %d"), GlobalText[40 + p->TwoHand], DamageMax + maxindex, DamageMax + maxindex);
				else
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(": %d ~ %d"), GlobalText[40 + p->TwoHand], DamageMin + minindex, DamageMax + maxindex);
			}
		}
		else
		{
			--TextNum;
		}

		if (DamageMin > 0)
		{
			if (minindex != 0 || maxindex != 0)
			{
				TextListColor[TextNum] = TEXT_COLOR_YELLOW;
				TextBold[TextNum] = false;
				++TextNum;
			}
			else
			{
				if (ip->ExcOption > 0)
					TextListColor[TextNum] = TEXT_COLOR_BLUE;
				else
					TextListColor[TextNum] = TEXT_COLOR_WHITE;
				TextBold[TextNum] = false;
				++TextNum;
			}
		}
		else
		{
			--TextNum;
		}
	}

	if (ip->Defense > 0)
	{
		mu_int32 maxdefense = 0;

		if (Level >= ip->HarmonyOptionLevel)
		{
			StrengthenCapability SC;
			g_pUIJewelHarmonyinfo.GetStrengthenCapability(&SC, ip, 2);

			if (SC.SI_isSD)
			{
				maxdefense = SC.SI_SD.SI_defense;
			}
		}
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[65], ip->Defense + maxdefense);

		if (maxdefense != 0)
			TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		else
		{
			if (ip->Type >= ITEM_HELM && ip->Type<ITEM_BOOTS + MAX_ITEM_INDEX && ip->ExcOption > 0)
				TextListColor[TextNum] = TEXT_COLOR_BLUE;
			else
				TextListColor[TextNum] = TEXT_COLOR_WHITE;

		}

		TextBold[TextNum] = false;
		++TextNum;
	}

	if (ip->MagicDefense > 0)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[66], ip->MagicDefense);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}

	if (p->SuccessfulBlocking > 0)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[67], ip->SuccessfulBlocking);
		if (ip->ExcOption > 0)
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
		else
			TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}

	if (p->WeaponSpeed)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[64], p->WeaponSpeed);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}

	if (p->WalkSpeed)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[68], p->WalkSpeed);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}

	if (ip->Type >= ITEM_WING + 32 && ip->Type <= ITEM_WING + 34)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[571]);
		switch (ip->Type)
		{
		case ITEM_WING + 32:
			TextListColor[TextNum] = TEXT_COLOR_RED;
			break;
		case ITEM_WING + 33:
			TextListColor[TextNum] = TEXT_COLOR_GREEN;
			break;
		case ITEM_WING + 34:
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			break;
		}
		TextBold[TextNum] = false; ++TextNum;
	}

	if (ip->Type >= ITEM_POTION + 45 && ip->Type <= ITEM_POTION + 50)
	{
		mu_unicode Text_data[300];
		ITEM_ADD_OPTION Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ip->Type);

		switch (ip->Type)
		{
		case ITEM_POTION + 45:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2011]);
			TextListColor[TextNum] = TEXT_COLOR_DARKYELLOW;
			break;
		case ITEM_POTION + 46:
			mu_sprintf_s(Text_data, mu_countof(Text_data), GlobalText[2229], Item_data.m_byValue1);
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, Text_data);
			TextListColor[TextNum] = TEXT_COLOR_DARKYELLOW;
			break;
		case ITEM_POTION + 47:
			mu_sprintf_s(Text_data, mu_countof(Text_data), GlobalText[2230], Item_data.m_byValue1);
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, Text_data);
			TextListColor[TextNum] = TEXT_COLOR_DARKYELLOW;
			break;
		case ITEM_POTION + 48:
			mu_sprintf_s(Text_data, mu_countof(Text_data), GlobalText[2231], Item_data.m_byValue1);
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, Text_data);
			TextListColor[TextNum] = TEXT_COLOR_DARKYELLOW;
			break;
		case ITEM_POTION + 49:
			mu_sprintf_s(Text_data, mu_countof(Text_data), GlobalText[960], Item_data.m_byValue1);
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, Text_data);
			TextListColor[TextNum] = TEXT_COLOR_DARKYELLOW;
			break;
		case ITEM_POTION + 50:
			mu_sprintf_s(Text_data, mu_countof(Text_data), GlobalText[961], Item_data.m_byValue1);
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, Text_data);
			TextListColor[TextNum] = TEXT_COLOR_DARKYELLOW;
			break;
		}
		TextBold[TextNum] = false; ++TextNum;
	}

	if (ip->Type >= ITEM_POTION + 32 && ip->Type <= ITEM_POTION + 34)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2011]);
		switch (ip->Type)
		{
		case ITEM_POTION + 32:
			if (Level == 0)
				TextListColor[TextNum] = TEXT_COLOR_PURPLE;
			else
				if (Level == 1)
					TextListColor[TextNum] = TEXT_COLOR_PURPLE;
			break;
		case ITEM_POTION + 33:
			if (Level == 0)
				TextListColor[TextNum] = TEXT_COLOR_RED;
			else
				if (Level == 1)
					TextListColor[TextNum] = TEXT_COLOR_RED;
			break;
		case ITEM_POTION + 34:
			if (Level == 0)
				TextListColor[TextNum] = TEXT_COLOR_BLUE;
			else
				if (Level == 1)
					TextListColor[TextNum] = TEXT_COLOR_BLUE;
			break;
		}
		TextBold[TextNum] = false; ++TextNum;
	}

	if (ip->Type == ITEM_POTION + 11)
	{
		if (Level == 7)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[112]);
			TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[113]);
			TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[114]);
			TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		}
		else if (Level == 14)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1652]);
			TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1653]);
			TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		}
		else
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[571]);
			TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		}

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[733]);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		TextBold[TextNum] = false;
		++TextNum;

		if (Level == 13)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[731]);
			TextListColor[TextNum] = TEXT_COLOR_RED;
			TextBold[TextNum] = false;
			++TextNum;
		}
	}

	mu_unicode tCount = COMGEM::CalcCompiledCount(ip);
	if (tCount > 0)
	{
		switch (COMGEM::CheckOneItem(ip))
		{
		case COMGEM::COMCELE:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1819], tCount, GlobalText[1806]);
				TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1820]);
				TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
			}
			break;
		case COMGEM::COMSOUL:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1819], tCount, GlobalText[1807]);
				TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1820]);
				TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
			}
			break;
		}
	}
	if (ip->Type == ITEM_POTION + 13)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[572]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	if (ip->Type == ITEM_POTION + 14)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[573]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	if (ip->Type == ITEM_POTION + 16)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[621]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	if (ip->Type == ITEM_POTION + 17 || ip->Type == ITEM_POTION + 18)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[637]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	if (ip->Type == ITEM_POTION + 20 && Level >= 1 && Level <= 5)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[571]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	if (ip->Type == ITEM_WING + 15)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[574]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	if (ip->Type == ITEM_POTION + 22)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[619]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	if (ip->Type == ITEM_POTION + 31)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1289]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	if (ip->Type == ITEM_HELPER + 0)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[578], 20);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[739], 50);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	if (ip->Type == ITEM_HELPER + 1)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[576]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	if ((ip->Type >= ITEM_WING && ip->Type <= ITEM_WING + 2) || ip->Type == ITEM_WING + 41)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[577], 12 + Level * 2);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[578], 12 + Level * 2);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[579]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 38)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2207]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 41)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2208]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 42)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2209]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 43)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2210]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 44)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2210]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	else if ((ip->Type >= ITEM_WING + 3 && ip->Type <= ITEM_WING + 6) || ip->Type == ITEM_WING + 42)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[577], 32 + Level);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[578], 25 + Level * 2);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[579]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	else if ((ip->Type >= ITEM_WING + 36 && ip->Type <= ITEM_WING + 40) || ip->Type == ITEM_WING + 43)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[577], 39 + Level * 2);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		if (ip->Type == ITEM_WING + 40)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[578], 24 + Level * 2);
		}
		else
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[578], 39 + Level * 2);
		}
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[579]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	else if (ITEM_WING + 130 <= ip->Type &&
		ip->Type <= ITEM_WING + 134)
	{
		switch (ip->Type)
		{
		case ITEM_WING + 130:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[577], 20 + Level * 2);
				TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[578], 20 + Level * 2);
				TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
			}break;
		case ITEM_WING + 131:
		case ITEM_WING + 132:
		case ITEM_WING + 133:
		case ITEM_WING + 134:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[577], 12 + Level * 2);
				TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[578], 12 + Level * 2);
				TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
			}break;
		}
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[579]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 3)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[577], 15);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[578], 10);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 31)
	{
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		switch (Level)
		{
		case 0: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1215]); ++TextNum; break;
		case 1: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1216]); ++TextNum; break;
		}
	}
	else if (ip->Type == ITEM_HELPER + 14)
	{
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		switch (Level)
		{
		case 0: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), GlobalText[748]); ++TextNum; break;
		case 1: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), GlobalText[1236]); ++TextNum; break;
		}
	}
	else if (ip->Type == ITEM_HELPER + 15)
	{
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		switch (Level)
		{
		case 0:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[168], GlobalText[636]); break;
		case 1:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[169], GlobalText[636]); break;
		case 2:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[167], GlobalText[636]); break;
		case 3:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[166], GlobalText[636]); break;
		case 4:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[1900], GlobalText[636]); break;
		}
		++TextNum;
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		switch (Level)
		{
		case 0:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[168], GlobalText[1910]); break;
		case 1:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[169], GlobalText[1910]); break;
		case 2:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[167], GlobalText[1910]); break;
		case 3:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[166], GlobalText[1910]); break;
		case 4:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[1900], GlobalText[1910]); break;
		}
		++TextNum;
		TextListColor[TextNum] = TEXT_COLOR_DARKRED;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1908]);

		if (Level == 4)
		{
			++TextNum;
			TextListColor[TextNum] = TEXT_COLOR_WHITE;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[61], GlobalText[24]);
		}
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 16)
	{
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[816]);
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 17)
	{
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[816]);
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 18)
	{
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[814]);
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n"));
		++TextNum; ++SkipNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[638]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[639]);
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 7)
	{
		switch (Level)
		{
		case 0:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1417]); TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1418]); TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1419]); TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
			}
			break;
		case 1:
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1638]); TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1639]); TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1472]); TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
			}
			break;
		}
	}
	else if (ip->Type == ITEM_HELPER + 7)
	{
		switch (Level)
		{
		case 0: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1460]); break;
		case 1: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1461]); break;
		}
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 11)
	{
		switch (Level)
		{
		case 0: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1416]); break;
		case 1: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1462]); break;
		}
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 29)
	{
		mu_int32 startIndex = 0;
		if (GetBaseClass(Hero->Class) == GAME::CLASS_DARK || GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD)
		{
			startIndex = 6;
		}

		mu_int32 HeroLevel = CharacterAttribute->Level;

		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T("    ") SPF_STRING _T("      ") SPF_STRING _T("    "), GlobalText[1147], GlobalText[368], GlobalText[935], GlobalText[936]); TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;

		for (mu_int32 i = 0; i<6; ++i)
		{
			mu_int32 Zen = g_iChaosCastleZen[i];

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("        %d             %3d~%3d     %3d,000"), i + 1, g_iChaosCastleLevel[startIndex + i][0], EMath::Min(400, g_iChaosCastleLevel[startIndex + i][1]), Zen);
			if ((HeroLevel >= g_iChaosCastleLevel[startIndex + i][0] && HeroLevel <= g_iChaosCastleLevel[startIndex + i][1])
				&& IsMasterLevel(Hero->Class) == false)
			{
				TextListColor[TextNum] = TEXT_COLOR_DARKYELLOW;
			}
			else
			{
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
			}
			TextBold[TextNum] = false; ++TextNum;
		}

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("         %d          ") SPF_STRING _T("   %3d,000"), 7, GlobalText[737], 1000);
		if (IsMasterLevel(Hero->Class) == true)
		{
			TextListColor[TextNum] = TEXT_COLOR_DARKYELLOW;
		}
		else
		{
			TextListColor[TextNum] = TEXT_COLOR_WHITE;
		}
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n"));
		++TextNum;
		++SkipNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1157]);
		TextListColor[TextNum] = TEXT_COLOR_DARKBLUE;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 21)
	{
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		switch (Level)
		{
		case 3: mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1291]); break;
		default: break;
		}
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 28 || ip->Type == ITEM_POTION + 29)
	{
		TextNum = RenderHellasItemInfo(ip, TextNum);
	}
	else if (ip->Type == ITEM_WING + 49 || ip->Type == ITEM_HELPER + 30)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[577], 20 + Level * 2);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		mu_int32 _iDamage = (ip->Type == ITEM_WING + 49) ? 10 + Level * 2 : 10 + Level;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[578], _iDamage);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}

	mu_boolean bDurExist = false;
	if ((p->Durability || p->MagicDur) &&
		(((ip->Type<ITEM_WING || ip->Type >= ITEM_HELPER) && ip->Type<ITEM_POTION) ||
		(ip->Type >= ITEM_WING && ip->Type <= ITEM_WING + 6)
		|| (ip->Type >= ITEM_WING + 36 && ip->Type <= ITEM_WING + 43)))
	{
		bDurExist = true;
	}
	if ((bDurExist || ip->Durability) &&
		(ip->Type<ITEM_HELPER + 14 || ip->Type>ITEM_HELPER + 19)
		&& !(ip->Type == ITEM_HELPER + 20 && Level == 1)
		&& !(ip->Type == ITEM_HELPER + 20 && Level == 2)
		&& !(ip->Type == ITEM_HELPER + 29)
		&& ip->Type != ITEM_POTION + 7 && ip->Type != ITEM_HELPER + 7 && ip->Type != ITEM_HELPER + 11
		&& ip->Type != ITEM_HELPER + 35
		&& !(ip->Type >= ITEM_POTION + 70 && ip->Type <= ITEM_POTION + 71)
		&& !(ip->Type >= ITEM_HELPER + 54 && ip->Type <= ITEM_HELPER + 58)
		&& !(ip->Type >= ITEM_POTION + 78 && ip->Type <= ITEM_POTION + 82)
		&& !(ip->Type == ITEM_HELPER + 66)
		&& !(ip->Type == ITEM_HELPER + 71 || ip->Type == ITEM_HELPER + 72 || ip->Type == ITEM_HELPER + 73 || ip->Type == ITEM_HELPER + 74 || ip->Type == ITEM_HELPER + 75)
		&& !(ip->Type == ITEM_POTION + 133))
	{
		mu_int32 Success = false;
		mu_int32 arrow = false;
		if (ip->Type >= ITEM_POTION && ip->Type <= ITEM_POTION + 8)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[69], ip->Durability);
			Success = true;
		}
		else if (ip->Type == ITEM_POTION + 21 && Level == 3)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[69], ip->Durability);
			Success = true;
		}
		else if (ip->Type == ITEM_BOW + 7 || ip->Type == ITEM_BOW + 15)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[69], ip->Durability);
			Success = true;
			arrow = true;
		}
		else if (ip->Type >= ITEM_POTION + 35 && ip->Type <= ITEM_POTION + 40)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[69], ip->Durability);
			Success = true;
		}
		else if (ip->Type >= ITEM_POTION + 46 && ip->Type <= ITEM_POTION + 50)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[69], ip->Durability);
			Success = true;
		}
		else if (ip->Type >= ITEM_HELPER + 32 && ip->Type <= ITEM_HELPER + 33)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1181], ip->Durability, 20);
			Success = true;
		}
		else if (ip->Type == ITEM_HELPER + 34)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1181], ip->Durability, 10);
			Success = true;
		}
		else if (ip->Type == ITEM_HELPER + 37)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[70], ip->Durability);
			Success = true;
		}
		else if (ip->Type >= ITEM_HELPER && ip->Type <= ITEM_HELPER + 7)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[70], ip->Durability);
			Success = true;
		}
		else if (ip->Type == ITEM_HELPER + 10)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[95], ip->Durability);
			Success = true;
		}
		else if (ip->Type == ITEM_HELPER + 64 || ip->Type == ITEM_HELPER + 65)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[70], ip->Durability);
			Success = true;
		}
		else if (ip->Type == ITEM_HELPER + 67 || ip->Type == ITEM_HELPER + 80
			|| ip->Type == ITEM_HELPER + 106 || ip->Type == ITEM_HELPER + 123)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[70], ip->Durability);
			Success = true;
		}
		else if (ip->Type >= ITEM_HELPER + 46 && ip->Type <= ITEM_HELPER + 48)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2260], ip->Durability);
			Success = true;
		}
		else if (ip->Type >= ITEM_HELPER + 125 && ip->Type <= ITEM_HELPER + 127)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2260], ip->Durability);

			if (ip->Type == ITEM_HELPER + 126)
			{
				++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3105]);

			}
			else if (ip->Type == ITEM_HELPER + 127)
			{
				++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3106]);
			}
			Success = true;
		}
		else if (ip->Type == ITEM_POTION + 53)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2296], ip->Durability);
			Success = true;
		}
		else if (ip->Type == ITEM_HELPER + 61)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2260], ip->Durability);
			Success = true;
		}
		else if (ip->Type == ITEM_POTION + 100)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[69], ip->Durability);
			Success = true;
		}
		else if (ip->Type == ITEM_HELPER + 70)
		{
			if (ip->Durability == 2)
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2605]);
				TextListColor[TextNum] = TEXT_COLOR_BLUE;
				TextBold[TextNum] = false;
				++TextNum;
			}
			else if (ip->Durability == 1)
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2606]);
				TextListColor[TextNum] = TEXT_COLOR_BLUE;
				TextBold[TextNum] = false;
				++TextNum;
#if 0 // FIX
				g_PortalMgr.GetPortalPositionText(TextList[TextNum]);
				TextListColor[TextNum] = TEXT_COLOR_BLUE;
				TextBold[TextNum] = false;
				++TextNum;
#endif
			}
		}
		else if (bDurExist)
		{
			mu_int32 maxDurability = calcMaxDurability(ip, p, Level);

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[71], ip->Durability, maxDurability);
			Success = true;
		}
		else if (ip->Type >= ITEM_TYPE_CHARM_MIXWING + EWS_BEGIN
			&& ip->Type <= ITEM_TYPE_CHARM_MIXWING + EWS_END)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2732 + (ip->Type - (MODEL_TYPE_CHARM_MIXWING + EWS_BEGIN))]);

			Success = true;
		}
		else if (ip->Type == ITEM_HELPER + 121)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2260], ip->Durability);
			Success = true;
		}

		if (Success)
		{
			TextListColor[TextNum] = TEXT_COLOR_WHITE;
			TextBold[TextNum] = false;
			++TextNum;
		}
	}
	else
	{
		if (ip->Type == ITEM_HELPER + 10)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[95], ip->Durability);

			TextListColor[TextNum] = TEXT_COLOR_WHITE;
			TextBold[TextNum] = false;
			++TextNum;
		}
	}

	if (ip->Type == ITEM_BOW + 7 || ip->Type == ITEM_BOW + 15)
	{
		if (Level >= 1)
		{
			mu_int32 value = Level * 2 + 1;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[577], value);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false;
			++TextNum;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[88], 1);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false;
			++TextNum;
		}
	}

	for (mu_int32 i = 0; i<MAX_RESISTANCE; ++i)
	{
		if (p->Resistance[i])
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[72], GlobalText[48 + i], Level + 1);
			TextListColor[TextNum] = TEXT_COLOR_WHITE;
			TextBold[TextNum] = false;
			++TextNum;
		}
	}

	if (ip->RequireLevel && ip->Type != ITEM_HELPER + 14)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[76], ip->RequireLevel);
		if (CharacterAttribute->Level < ip->RequireLevel)
		{
			TextListColor[TextNum] = TEXT_COLOR_RED;
			TextBold[TextNum] = false;
			++TextNum;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[74], ip->RequireLevel - CharacterAttribute->Level);
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

	mu_int32 si_iNeedStrength = 0, si_iNeedDex = 0;
	mu_boolean bRequireStat = true;

	if (Level >= ip->HarmonyOptionLevel)
	{
		StrengthenCapability SC;
		g_pUIJewelHarmonyinfo.GetStrengthenCapability(&SC, ip, 0);

		if (SC.SI_isNB)
		{
			si_iNeedStrength = SC.SI_NB.SI_force;
			si_iNeedDex = SC.SI_NB.SI_activity;
		}
	}

	if (ip->SocketCount > 0)
	{
		for (mu_int32 i = 0; i < ip->SocketCount; ++i)
		{
			if (ip->SocketSeedID[i] == 38)
			{
				mu_int32 iReqStrengthDown = g_SocketItemMgr.GetSocketOptionValue(ip, i);
				si_iNeedStrength += iReqStrengthDown;
			}
			else if (ip->SocketSeedID[i] == 39)
			{
				mu_int32 iReqDexterityDown = g_SocketItemMgr.GetSocketOptionValue(ip, i);
				si_iNeedDex += iReqDexterityDown;
			}
		}
	}

	if (ip->RequireStrength && bRequireStat)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[73], ip->RequireStrength - si_iNeedStrength);

		mu_uint16 Strength = CharacterAttribute->Strength + CharacterAttribute->AddStrength;
		if (Strength < ip->RequireStrength - si_iNeedStrength)
		{
			TextListColor[TextNum] = TEXT_COLOR_RED;
			TextBold[TextNum] = false;
			++TextNum;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[74], (ip->RequireStrength - Strength) - si_iNeedStrength);
			TextListColor[TextNum] = TEXT_COLOR_RED;
			TextBold[TextNum] = false;
			++TextNum;
		}
		else
		{
			if (si_iNeedStrength != 0)
			{
				TextListColor[TextNum] = TEXT_COLOR_YELLOW;
			}
			else
			{
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
			}

			TextBold[TextNum] = false;
			++TextNum;
		}
	}
	if (ip->RequireDexterity && bRequireStat)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[75], ip->RequireDexterity - si_iNeedDex);
		mu_uint16 Dexterity = CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;
		if (Dexterity < (ip->RequireDexterity - si_iNeedDex))
		{
			TextListColor[TextNum] = TEXT_COLOR_RED;
			TextBold[TextNum] = false;
			++TextNum;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[74], (ip->RequireDexterity - Dexterity) - si_iNeedDex);
			TextListColor[TextNum] = TEXT_COLOR_RED;
			TextBold[TextNum] = false;
			++TextNum;
		}
		else
		{
			if (si_iNeedDex != 0)
			{
				TextListColor[TextNum] = TEXT_COLOR_YELLOW;
			}
			else
			{
				TextListColor[TextNum] = TEXT_COLOR_WHITE;
			}
			TextBold[TextNum] = false;
			++TextNum;
		}
	}

	if (ip->RequireVitality && bRequireStat)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1930], ip->RequireVitality);

		mu_uint16 Vitality = CharacterAttribute->Vitality + CharacterAttribute->AddVitality;
		if (Vitality < ip->RequireVitality)
		{
			TextListColor[TextNum] = TEXT_COLOR_RED;
			TextBold[TextNum] = false;
			++TextNum;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[74], ip->RequireVitality - Vitality);
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

	if (ip->RequireEnergy && bRequireStat)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[77], ip->RequireEnergy);

		mu_uint16 Energy = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;

		if (Energy < ip->RequireEnergy)
		{
			TextListColor[TextNum] = TEXT_COLOR_RED;
			TextBold[TextNum] = false;
			++TextNum;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[74], ip->RequireEnergy - Energy);
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

	if (ip->RequireCharisma && bRequireStat)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[698], ip->RequireCharisma);

		mu_uint16 Charisma = CharacterAttribute->Charisma + CharacterAttribute->AddCharisma;
		if (Charisma < ip->RequireCharisma)
		{
			TextListColor[TextNum] = TEXT_COLOR_RED;
			TextBold[TextNum] = false;
			++TextNum;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[74], ip->RequireCharisma - Charisma);
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

	if (IsRequireClassRenderItem(ip->Type))
	{
		RequireClass(p);
	}

	if (ip->Type >= ITEM_BOOTS && ip->Type<ITEM_BOOTS + MAX_ITEM_INDEX)
	{
		if (Level >= 5)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[78]);
			TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = true; ++TextNum;
		}
	}

	if (ip->Type >= ITEM_GLOVES && ip->Type<ITEM_GLOVES + MAX_ITEM_INDEX)
	{
		if (Level >= 5)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[93]);
			TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = true; ++TextNum;
		}
	}

	if ((ip->Type >= ITEM_STAFF && ip->Type<ITEM_STAFF + MAX_ITEM_INDEX)
		|| (ip->Type == (ITEM_SWORD + 31))
		|| (ip->Type == (ITEM_SWORD + 23))
		|| (ip->Type == (ITEM_SWORD + 25))
		|| (ip->Type == (ITEM_SWORD + 21))
		|| (ip->Type == (ITEM_SWORD + 28))
		)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		mu_int32 nText = ITEM_STAFF + 21 <= ip->Type && ip->Type <= ITEM_STAFF + 29 ? 1691 : 79;
		::mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[nText], ip->MagicPower);

		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = true;
		++TextNum;
	}

	if (IsCepterItem(ip->Type) == true)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1234], ip->MagicPower);
		TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = true; ++TextNum;
	}

	if (ip->SpecialNum > 0)
	{
		if (!(ITEM_HELPER + 109 <= ip->Type  && ITEM_HELPER + 115 >= ip->Type))
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;
		}
	}

	if (ip->ItemOption != false)
	{
		std::vector<mu_string> Text380;

		g_pItemAddOptioninfo.GetItemAddOtioninfoText(Text380, ip->Type);

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		for (mu_int32 i = 0; i < (mu_int32)Text380.size(); ++i)
		{
			mu_strncpy_s(TextList[TextNum], TEXTLIST_LENGTH, Text380[i].c_str(), 100);
			TextListColor[TextNum] = TEXT_COLOR_REDPURPLE; TextBold[TextNum] = true; ++TextNum;
		}

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;
	}

	if (g_SocketItemMgr.IsSocketItem(ip));
	else if (ip->HarmonyOption != 0)
		{
			StrengthenItem type = g_pUIJewelHarmonyinfo.GetItemType(static_cast<mu_int32>(ip->Type));

			if (type < SI_None)
			{
				if (g_pUIJewelHarmonyinfo.IsHarmonyJewelOption(type, ip->HarmonyOption))
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

					HARMONYJEWELOPTION harmonyjewel = g_pUIJewelHarmonyinfo.GetHarmonyJewelOptionInfo(type, ip->HarmonyOption);

					if (type == SI_Defense && ip->HarmonyOption == 7)
					{
						mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" +%d%%"), harmonyjewel.Name.c_str(), harmonyjewel.HarmonyJewelLevel[ip->HarmonyOptionLevel]);
					}
					else
					{
						mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" +%d"), harmonyjewel.Name.c_str(), harmonyjewel.HarmonyJewelLevel[ip->HarmonyOptionLevel]);
					}

					if (Level >= ip->HarmonyOptionLevel) TextListColor[TextNum] = TEXT_COLOR_YELLOW;
					else TextListColor[TextNum] = TEXT_COLOR_GRAY;

					TextBold[TextNum] = true; ++TextNum;

					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;
				}
				else
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" : %d %d %d")
						, GlobalText[2204]
						, (mu_int32)type
						, (mu_int32)ip->HarmonyOption
						, (mu_int32)ip->HarmonyOptionLevel
					);

					TextListColor[TextNum] = TEXT_COLOR_DARKRED;
					TextBold[TextNum] = true; ++TextNum;

					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2205]);

					TextListColor[TextNum] = TEXT_COLOR_DARKRED;
					TextBold[TextNum] = true; ++TextNum;

					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;
				}
			}
		}

	TextNum = g_csItemOption.RenderDefaultOptionText(ip, TextNum);

	mu_int32 iMana;
	for (mu_int32 i = 0; i<ip->SpecialNum; ++i)
	{
		if (ITEM_HELPER + 109 <= ip->Type  && ITEM_HELPER + 115 >= ip->Type)
		{
			break;
		}

		GetSkillInformation(ip->Special[i], 1, nullptr, 0, &iMana, nullptr);
		GetSpecialOptionText(ip->Type, TextList[TextNum], TEXTLIST_LENGTH, ip->Special[i], ip->SpecialValue[i], iMana);

		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false; ++TextNum;

		if (ip->Special[i] == AT_LUCK)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[94], ip->SpecialValue[i]);
			TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;
		}
		else if (ip->Special[i] == AT_SKILL_RIDER)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[179]);
			TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
		}
		else if (ip->Special[i] == AT_SKILL_DARK_HORSE || (AT_SKILL_ASHAKE_UP <= ip->Special[i] && ip->Special[i] <= AT_SKILL_ASHAKE_UP + 4))
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1201]);
			TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
		}
		else if ((ip->Special[i] == AT_IMPROVE_DAMAGE) &&
			((ip->Type == ITEM_SWORD + 31)
				|| (ip->Type == ITEM_SWORD + 21)
				|| (ip->Type == ITEM_SWORD + 23)
				|| (ip->Type == ITEM_SWORD + 25)
				|| (ip->Type == ITEM_SWORD + 28)
				)
			)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[89], ip->SpecialValue[i]);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false; ++TextNum;
		}
	}

	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

	if (ip->Type == ITEM_HELPER + 32 || ip->Type == ITEM_HELPER + 33)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1917]);
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 34 || ip->Type == ITEM_HELPER + 35)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1918]);
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 36)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1919]);
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 37)
	{
		GetSpecialOptionText(0, TextList[TextNum], TEXTLIST_LENGTH, AT_SKILL_PLASMA_STORM_FENRIR, 0, 0);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false; ++TextNum; ++SkipNum;
		if (ip->ExcOption == 0x01)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1860], 10);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			++TextNum;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[579]);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			++TextNum;
		}
		else if (ip->ExcOption == 0x02)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1861], 10);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			++TextNum;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[579]);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			++TextNum;
		}
		else if (ip->ExcOption == 0x04)
		{
			mu_uint16 wLevel = CharacterAttribute->Level;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1867], (wLevel / 2));
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			++TextNum;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1868], (wLevel / 2));
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			++TextNum;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1869], (wLevel / 12));
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			++TextNum;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1870], (wLevel / 25));
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			++TextNum;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n"));
			++TextNum;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1871], (Hero->Level / 2));
			TextListColor[TextNum] = TEXT_COLOR_GREEN;
			++TextNum;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1872], (Hero->Level / 2));
			TextListColor[TextNum] = TEXT_COLOR_GREEN;
			++TextNum;
		}

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1920]);
		TextListColor[TextNum] = TEXT_COLOR_YELLOW;
		++TextNum;

		if (ip->ExcOption == 0x00)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1929]);
			TextListColor[TextNum] = TEXT_COLOR_YELLOW;
			++TextNum;
		}
	}
	else if (ip->Type == ITEM_HELPER + 10)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3088]);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 39)
	{
		mu_unicode strText[100];
		mu_sprintf_s(strText, mu_countof(strText), GlobalText[959], 10);
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T("%%"), strText);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		++TextNum;

		mu_uint16 wlevel = CharacterAttribute->Level;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2225], wlevel);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3088]);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 63)
	{
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2244]); ++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 52)
	{
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2323]); ++TextNum;
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2011]); ++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 40)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), GlobalText[2232]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3088]);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3088]);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 41)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[88], 20);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[89], 20);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), GlobalText[2248]);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3088]);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 76)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2743]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2744]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2745]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3088]);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 122)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3065]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3066]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3067]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3072]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3088]);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 51)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), GlobalText[2244]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 42)
	{
		for (mu_int32 i = 0; i<7; ++i)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[976 + i], 255);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			++TextNum;
		}

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3088]);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 66)
	{
		--TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2260], ip->Durability);
		TextListColor[TextNum] = TEXT_COLOR_RED;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), GlobalText[2589]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		++TextNum;
	}
	else if (ip->Type == ITEM_POTION + 100)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1887], ip->Durability);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		++TextNum;
	}
	else if (ip->Type == ITEM_HELPER + 123)
	{
		--TextNum; --SkipNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3071]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3072]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}
	else if (IsInvenItem(ip->Type))
	{
		--TextNum; --SkipNum;

		if (ip->Durability == 254)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3143]);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false;
			++TextNum;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;
		}

		switch (ip->Type)
		{
		case ITEM_HELPER + 128:
		case ITEM_HELPER + 129:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3121]);
			break;

		case ITEM_HELPER + 134:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3123]);
			break;

		case ITEM_HELPER + 130:
		case ITEM_HELPER + 131:
		case ITEM_HELPER + 132:
		case ITEM_HELPER + 133:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3122]);
			break;
		}

		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		switch (ip->Type)
		{
		case ITEM_HELPER + 128:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[965], 10);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false;
			++TextNum;
			break;
		case ITEM_HELPER + 129:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[967], 10);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false;
			++TextNum;
			break;
		case ITEM_HELPER + 134:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3126], 20);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false;
			++TextNum;
			break;
		case ITEM_HELPER + 130:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3132], 50);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false;
			++TextNum;
			break;
		case ITEM_HELPER + 131:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3134], 50);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false;
			++TextNum;
			break;
		case ITEM_HELPER + 132:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3132], 100);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false;
			++TextNum;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3133], 500);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false;
			++TextNum;
			break;
		case ITEM_HELPER + 133:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3134], 150);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false;
			++TextNum;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3135], 50);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false;
			++TextNum;
			break;
		}

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[3124]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false;
		++TextNum;
	}

	if (bItemTextListBoxUse == false)
	{
		TextNum = g_csItemOption.RenderSetOptionListInItem(ip, TextNum, bIsEquippedItem);
		TextNum = g_SocketItemMgr.AttachToolTipForSocketItem(ip, TextNum);
	}

	mu_boolean isrendertooltip = true;

	isrendertooltip = g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_PARTCHARGE_SHOP) ? false : true;

	if (isrendertooltip)
	{
		if (bItemTextListBoxUse)
			RenderTipTextList(sx, sy, TextNum, 0, ETA_CENTER, STRP_BOTTOMCENTER, useBG);
		else if(useAlternativePos == false)
			RenderTipTextList(sx, sy, TextNum, 0, ETA_CENTER, STRP_ITEMINFO, useBG);
		else
			RenderTipTextList(sx, sy, TextNum, 0, ETA_CENTER, STRP_ITEMINFO2, useBG);
	}
}

void RenderRepairInfo(mu_int32 sx, mu_int32 sy, ITEM *ip, mu_boolean Sell, mu_boolean useAlternativePos)
{
	if (IsRepairBan(ip) == true)
	{
		return;
	}

	if (ip->Type >= MODEL_TYPE_CHARM_MIXWING + EWS_BEGIN
		&& ip->Type <= MODEL_TYPE_CHARM_MIXWING + EWS_END)
	{
		return;
	}
	if (ip->Type == ITEM_HELPER + 107)
	{
		return;
	}
	if (ip->Type == ITEM_HELPER + 104)
	{
		return;
	}
	if (ip->Type == ITEM_HELPER + 105)
	{
		return;
	}
	if (ip->Type == ITEM_HELPER + 103)
	{
		return;
	}
	if (ip->Type == ITEM_POTION + 133)
	{
		return;
	}
	if (ip->Type == ITEM_HELPER + 109)
	{
		return;
	}
	if (ip->Type == ITEM_HELPER + 110)
	{
		return;
	}
	if (ip->Type == ITEM_HELPER + 111)
	{
		return;
	}
	if (ip->Type == ITEM_HELPER + 112)
	{
		return;
	}
	if (ip->Type == ITEM_HELPER + 113)
	{
		return;
	}
	if (ip->Type == ITEM_HELPER + 114)
	{
		return;
	}
	if (ip->Type == ITEM_HELPER + 115)
	{
		return;
	}
	if (ip->Type == ITEM_POTION + 112)
	{
		return;
	}
	if (ip->Type == ITEM_POTION + 113)
	{
		return;
	}
	if (ip->Type == ITEM_POTION + 120)
	{
		return;
	}
	if (ip->Type == ITEM_POTION + 121)
	{
		return;
	}
	if (ip->Type == ITEM_POTION + 122)
	{
		return;
	}
	if (ITEM_POTION + 123 == ip->Type)
	{
		return;
	}
	if (ITEM_POTION + 124 == ip->Type)
	{
		return;
	}
	if (ITEM_POTION + 134 <= ip->Type && ip->Type <= ITEM_POTION + 139)
	{
		return;
	}
	if (ITEM_WING + 130 <= ip->Type &&
		ip->Type <= ITEM_WING + 134)
	{
		return;
	}
	if (ITEM_POTION + 114 <= ip->Type && ip->Type <= ITEM_POTION + 119)
	{
		return;
	}
	if (ITEM_POTION + 126 <= ip->Type && ip->Type <= ITEM_POTION + 129)
	{
		return;
	}
	if (ITEM_POTION + 130 <= ip->Type && ip->Type <= ITEM_POTION + 132)
	{
		return;
	}
	if (ITEM_HELPER + 121 == ip->Type)
	{
		return;
	}

	ITEM_ATTRIBUTE *p = &ItemAttribute[ip->Type];
	TextNum = 0;
	SkipNum = 0;
	for (mu_int32 i = 0; i<30; ++i)
	{
		TextList[i][0] = _T('\0');
	}

	mu_int32 Level = ip->Level;
	mu_int32 Color;

	if (ip->Type == ITEM_POTION + 13 || ip->Type == ITEM_POTION + 14 || ip->Type == ITEM_WING + 15)
	{
		Color = TEXT_COLOR_YELLOW;
	}
	else if (COMGEM::isCompiledGem(ip))
	{
		Color = TEXT_COLOR_YELLOW;
	}
	else if (ip->Type == ITEM_STAFF + 10 || ip->Type == ITEM_SWORD + 19 || ip->Type == ITEM_BOW + 18 || ip->Type == ITEM_MACE + 13)
	{
		Color = TEXT_COLOR_PURPLE;
	}
	else if (ip->Type == ITEM_POTION + 17 || ip->Type == ITEM_POTION + 18 || ip->Type == ITEM_POTION + 19)
	{
		Color = TEXT_COLOR_YELLOW;
	}
	else if (ip->SpecialNum > 0 && ip->ExcOption > 0)
	{
		Color = TEXT_COLOR_GREEN;
	}
	else if (Level >= 7)
	{
		Color = TEXT_COLOR_YELLOW;
	}
	else
	{
		if (ip->SpecialNum > 0)
		{
			Color = TEXT_COLOR_BLUE;
		}
		else
		{
			Color = TEXT_COLOR_WHITE;
		}
	}

	if ((ip->Type >= ITEM_WING + 3 && ip->Type <= ITEM_WING + 6)
		|| ip->Type >= ITEM_HELPER + 30
		|| (ip->Type >= ITEM_WING + 36 && ip->Type <= ITEM_WING + 40)
		|| (ip->Type >= ITEM_WING + 42 && ip->Type <= ITEM_WING + 43))
	{
		if (Level >= 7)
		{
			Color = TEXT_COLOR_YELLOW;
		}
		else
		{
			if (ip->SpecialNum > 0)
			{
				Color = TEXT_COLOR_BLUE;
			}
			else
			{
				Color = TEXT_COLOR_WHITE;
			}
		}
	}

	if (ip->Type<ITEM_POTION)
	{
		mu_int32 maxDurability;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

		mu_unicode Text[100];

		maxDurability = calcMaxDurability(ip, p, Level);
		if (ip->Durability<maxDurability)
		{
#if 0 // FIX
			RepairEnable = 2;
#endif

			mu_int32 iGold = ItemValue(ip, 2);
			if (iGold == -1)
				return;
			ConvertRepairGold(iGold, ip->Durability, maxDurability, ip->Type, Text, mu_countof(Text));
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[238], Text);

			TextListColor[TextNum] = Color;
		}
		else
		{
#if 0 // FIX
			RepairEnable = 1;
#endif

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[238], "0");

			TextListColor[TextNum] = Color;
		}
		TextBold[TextNum] = true;
		++TextNum;
	}
	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

	if (ip->Type == ITEM_WING + 11)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), SkillAttribute[30 + Level].Name, GlobalText[102]);
	}
	else if (ip->Type == ITEM_HELPER + 10)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GetMonsterName(SommonTable[Level]), GlobalText[103]);
	}
	else if ((ip->Type == ITEM_HELPER + 4) || (ip->Type == ITEM_HELPER + 5))
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
	}
	else if ((ip->Type >= ITEM_WING + 3 && ip->Type <= ITEM_WING + 6)
		|| ip->Type >= ITEM_HELPER + 30
		|| (ip->Type >= ITEM_WING + 36 && ip->Type <= ITEM_WING + 40)
		|| (ip->Type >= ITEM_WING + 42 && ip->Type <= ITEM_WING + 43))
	{
		if (Level == 0)
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
		else
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" +%d"), p->Name.c_str(), Level);
	}
	else
	{
		if (ip->ExcOption > 0)
		{
			if (Level == 0)
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[620], p->Name.c_str());
			else
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(" +%d"), GlobalText[620], p->Name.c_str(), Level);
		}
		else
		{
			if (Level == 0)
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), p->Name.c_str());
			else
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" +%d"), p->Name.c_str(), Level);
		}
	}
	TextListColor[TextNum] = Color; TextBold[TextNum] = true; ++TextNum;
	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

	if (ip->Type<ITEM_POTION)
	{
		mu_int32 maxDurability = calcMaxDurability(ip, p, Level);

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[71], ip->Durability, maxDurability);

		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = false;
		++TextNum;
	}

	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

	if (useAlternativePos == false)
		RenderTipTextList(sx, sy, TextNum, 0, ETA_CENTER, STRP_ITEMINFO);
	else
		RenderTipTextList(sx, sy, TextNum, 0, ETA_CENTER, STRP_ITEMINFO2);
}

void RenderSkillInfo(mu_int32 sx, mu_int32 sy, mu_int32 Type, mu_int32 SkillNum, mu_int32 iRenderPoint)
{
	mu_text Name[256];
	mu_int32 MinDamage, MaxDamage;
	mu_int32 HeroClass = GetBaseClass(Hero->Class);
	mu_int32 Mana, Distance, SkillMana;
	mu_int32 TextNum = 0;
	mu_int32 SkipNum = 0;

	if (giPetManager::RenderPetCmdInfo(sx, sy, Type, iRenderPoint))
	{
		return;
	}

	mu_int32 AttackDamageMin, AttackDamageMax;
	mu_int32 iSkillMinDamage, iSkillMaxDamage;

	mu_int32 SkillType = CharacterAttribute->Skill[Type];
	CharacterMachine->GetMagicSkillDamage(CharacterAttribute->Skill[Type], &MinDamage, &MaxDamage);
	CharacterMachine->GetSkillDamage(CharacterAttribute->Skill[Type], &iSkillMinDamage, &iSkillMaxDamage);

	GetAttackDamage(&AttackDamageMin, &AttackDamageMax);

	iSkillMinDamage += AttackDamageMin;
	iSkillMaxDamage += AttackDamageMax;
	GetSkillInformation(CharacterAttribute->Skill[Type], 1, Name, mu_countof(Name), &Mana, &Distance, &SkillMana);

	if (CharacterAttribute->Skill[Type] == AT_SKILL_STRONG_PIER && Hero->Weapon[0].Type != -1)
	{
		for (mu_int32 i = 0; i < CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT].SpecialNum; ++i)
		{
			if (CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT].Special[i] == AT_SKILL_LONG_SPEAR)
			{
				mu_sprintf_s(Name, mu_countof(Name), _T("") SPF_STRING _T(""), GlobalText[1200]);
				break;
			}
		}
	}

	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;
	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), Name);
	TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = true; ++TextNum;
	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

	mu_uint32 Dexterity;
	mu_uint32 Energy;
	mu_uint32 Strength;
	mu_uint32 Vitality;
	mu_uint32 Charisma;

	Dexterity = CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;
	Energy = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;

	Strength = CharacterAttribute->Strength + CharacterAttribute->AddStrength;
	Vitality = CharacterAttribute->Vitality + CharacterAttribute->AddVitality;
	Charisma = CharacterAttribute->Charisma + CharacterAttribute->AddCharisma;

	mu_int32 skillattackpowerRate = 0;

	StrengthenCapability rightinfo, leftinfo;

	ITEM* rightweapon = &CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT];
	ITEM* leftweapon = &CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT];

	mu_int32 rightlevel = rightweapon->Level;

	if (rightlevel >= rightweapon->HarmonyOptionLevel)
	{
		g_pUIJewelHarmonyinfo.GetStrengthenCapability(&rightinfo, rightweapon, 1);
	}

	mu_int32 leftlevel = leftweapon->Level;

	if (leftlevel >= leftweapon->HarmonyOptionLevel)
	{
		g_pUIJewelHarmonyinfo.GetStrengthenCapability(&leftinfo, leftweapon, 1);
	}

	if (rightinfo.SI_isSP)
	{
		skillattackpowerRate += rightinfo.SI_SP.SI_skillattackpower;
		skillattackpowerRate += rightinfo.SI_SP.SI_magicalpower;
	}

	if (leftinfo.SI_isSP)
	{
		skillattackpowerRate += leftinfo.SI_SP.SI_skillattackpower;
	}

	if (HeroClass == GAME::CLASS_WIZARD || HeroClass == GAME::CLASS_SUMMONER)
	{
		if (CharacterAttribute->Skill[Type] == AT_SKILL_WIZARDDEFENSE || (AT_SKILL_SOUL_UP <= CharacterAttribute->Skill[Type] && CharacterAttribute->Skill[Type] <= AT_SKILL_SOUL_UP + 4))
		{
			mu_int32 iDamageShield = 0;
			if (CharacterAttribute->Skill[Type] == AT_SKILL_WIZARDDEFENSE)
			{
				iDamageShield = (mu_int32)(10 + (Dexterity / 50.0f) + (Energy / 200.0f));
			}
			else if (CharacterAttribute->Skill[Type] >= AT_SKILL_SOUL_UP  &&
				(CharacterAttribute->Skill[Type] <= AT_SKILL_SOUL_UP + 4))
			{
				iDamageShield = (mu_int32)(10 + (Dexterity / 50.0f) + (Energy / 200.0f)) + ((CharacterAttribute->Skill[Type] - AT_SKILL_SOUL_UP + 1) * 5);
			}

			mu_int32 iDeleteMana = (mu_int32)(CharacterAttribute->ManaMax*0.02f);
			mu_int32 iLimitTime = (mu_int32)(60 + (Energy / 40.0f));

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[578], iDamageShield);
			TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[880], iDeleteMana);
			TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[881], iLimitTime);
			TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		}
		else if (SkillType != AT_SKILL_SWELL_OF_MAGICPOWER &&
			SkillType != AT_SKILL_ALICE_SLEEP &&
			!(AT_SKILL_ALICE_SLEEP_UP <= SkillType &&
				SkillType <= AT_SKILL_ALICE_SLEEP_UP + 4))
		{
			mu_uint8 bySkill = CharacterAttribute->Skill[Type];
			if (!(AT_SKILL_STUN <= bySkill && bySkill <= AT_SKILL_MANA)
				&& !(AT_SKILL_ALICE_THORNS <= bySkill && bySkill <= AT_SKILL_ALICE_ENERVATION)
				&& bySkill != AT_SKILL_TELEPORT
				&& bySkill != AT_SKILL_TELEPORT_B)
			{
				if (AT_SKILL_SUMMON_EXPLOSION <= bySkill && bySkill <= AT_SKILL_SUMMON_POLLUTION)
				{
					CharacterMachine->GetCurseSkillDamage(bySkill, &MinDamage, &MaxDamage);
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1692], MinDamage, MaxDamage);
				}
				else
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[170], MinDamage + skillattackpowerRate, MaxDamage + skillattackpowerRate);
				TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
			}
		}
	}

	if (HeroClass == GAME::CLASS_KNIGHT ||
		HeroClass == GAME::CLASS_DARK ||
		HeroClass == GAME::CLASS_ELF ||
		HeroClass == GAME::CLASS_DARK_LORD)
	{
		switch (CharacterAttribute->Skill[Type])
		{
		case AT_SKILL_TELEPORT:
		case AT_SKILL_TELEPORT_B:
		case AT_SKILL_SOUL_UP:
		case AT_SKILL_SOUL_UP + 1:
		case AT_SKILL_SOUL_UP + 2:
		case AT_SKILL_SOUL_UP + 3:
		case AT_SKILL_SOUL_UP + 4:

		case AT_SKILL_HEAL_UP:
		case AT_SKILL_HEAL_UP + 1:
		case AT_SKILL_HEAL_UP + 2:
		case AT_SKILL_HEAL_UP + 3:
		case AT_SKILL_HEAL_UP + 4:

		case AT_SKILL_LIFE_UP:
		case AT_SKILL_LIFE_UP + 1:
		case AT_SKILL_LIFE_UP + 2:
		case AT_SKILL_LIFE_UP + 3:
		case AT_SKILL_LIFE_UP + 4:

		case AT_SKILL_WIZARDDEFENSE:
		case AT_SKILL_BLOCKING:
		case AT_SKILL_VITALITY:
		case AT_SKILL_HEALING:

		case AT_SKILL_DEF_POWER_UP:
		case AT_SKILL_DEF_POWER_UP + 1:
		case AT_SKILL_DEF_POWER_UP + 2:
		case AT_SKILL_DEF_POWER_UP + 3:
		case AT_SKILL_DEF_POWER_UP + 4:

		case AT_SKILL_DEFENSE:

		case AT_SKILL_ATT_POWER_UP:
		case AT_SKILL_ATT_POWER_UP + 1:
		case AT_SKILL_ATT_POWER_UP + 2:
		case AT_SKILL_ATT_POWER_UP + 3:
		case AT_SKILL_ATT_POWER_UP + 4:

		case AT_SKILL_ATTACK:
		case AT_SKILL_SUMMON:
		case AT_SKILL_SUMMON + 1:
		case AT_SKILL_SUMMON + 2:
		case AT_SKILL_SUMMON + 3:
		case AT_SKILL_SUMMON + 4:
		case AT_SKILL_SUMMON + 5:
		case AT_SKILL_SUMMON + 6:

		case AT_SKILL_IMPROVE_AG:
		case AT_SKILL_STUN:
		case AT_SKILL_REMOVAL_STUN:
		case AT_SKILL_MANA:
		case AT_SKILL_INVISIBLE:
		case AT_SKILL_REMOVAL_INVISIBLE:
		case AT_SKILL_REMOVAL_BUFF:
			break;
		case AT_SKILL_PARTY_TELEPORT:
		case AT_SKILL_ADD_CRITICAL:
			break;
		case AT_SKILL_ASHAKE_UP:
		case AT_SKILL_ASHAKE_UP + 1:
		case AT_SKILL_ASHAKE_UP + 2:
		case AT_SKILL_ASHAKE_UP + 3:
		case AT_SKILL_ASHAKE_UP + 4:
		case AT_SKILL_DARK_HORSE:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1237]);
			TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
			break;
		case AT_SKILL_BRAND_OF_SKILL:
			break;
		case AT_SKILL_PLASMA_STORM_FENRIR:
		case AT_SKILL_RECOVER:
			break;
		default:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[879], iSkillMinDamage, iSkillMaxDamage + skillattackpowerRate);
			TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
			break;
		}
	}

	if (CharacterAttribute->Skill[Type] == AT_SKILL_PLASMA_STORM_FENRIR)
	{
		mu_int32 iSkillDamage;
		GetSkillInformation_Damage(AT_SKILL_PLASMA_STORM_FENRIR, &iSkillDamage);

		if (HeroClass == GAME::CLASS_KNIGHT || HeroClass == GAME::CLASS_DARK)
		{
			iSkillMinDamage = (Strength / 3) + (Dexterity / 5) + (Vitality / 5) + (Energy / 7) + iSkillDamage;
		}
		else if (HeroClass == GAME::CLASS_WIZARD || HeroClass == GAME::CLASS_SUMMONER)
		{
			iSkillMinDamage = (Strength / 5) + (Dexterity / 5) + (Vitality / 7) + (Energy / 3) + iSkillDamage;
		}
		else if (HeroClass == GAME::CLASS_ELF)
		{
			iSkillMinDamage = (Strength / 5) + (Dexterity / 3) + (Vitality / 7) + (Energy / 5) + iSkillDamage;
		}
		else if (HeroClass == GAME::CLASS_DARK_LORD)
		{
			iSkillMinDamage = (Strength / 5) + (Dexterity / 5) + (Vitality / 7) + (Energy / 3) + (Charisma / 3) + iSkillDamage;
		}

		iSkillMaxDamage = iSkillMinDamage + 30;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[879], iSkillMinDamage, iSkillMaxDamage + skillattackpowerRate);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}

	if (GetBaseClass(Hero->Class) == GAME::CLASS_ELF)
	{
		bool Success = true;
		switch (CharacterAttribute->Skill[Type])
		{
		case AT_SKILL_HEAL_UP:
		case AT_SKILL_HEAL_UP + 1:
		case AT_SKILL_HEAL_UP + 2:
		case AT_SKILL_HEAL_UP + 3:
		case AT_SKILL_HEAL_UP + 4:
			{
				mu_int32 Cal = (Energy / 5) + 5;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[171], (Cal)+(mu_int32)((mu_float)Cal*(mu_float)(SkillAttribute[CharacterAttribute->Skill[Type]].Damage / (mu_float)100)));
			}
			break;
		case AT_SKILL_HEALING:
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[171], Energy / 5 + 5);
			break;
		case AT_SKILL_DEF_POWER_UP:
		case AT_SKILL_DEF_POWER_UP + 1:
		case AT_SKILL_DEF_POWER_UP + 2:
		case AT_SKILL_DEF_POWER_UP + 3:
		case AT_SKILL_DEF_POWER_UP + 4:
			{
				mu_int32 Cal = Energy / 8 + 2;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[172], (Cal)+(mu_int32)((mu_float)Cal / (mu_float)((mu_float)SkillAttribute[CharacterAttribute->Skill[Type]].Damage / (mu_float)10)));
			}
			break;
		case AT_SKILL_DEFENSE:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[172], Energy / 8 + 2); break;
		case AT_SKILL_ATT_POWER_UP:
		case AT_SKILL_ATT_POWER_UP + 1:
		case AT_SKILL_ATT_POWER_UP + 2:
		case AT_SKILL_ATT_POWER_UP + 3:
		case AT_SKILL_ATT_POWER_UP + 4:
			{
				mu_int32 Cal = Energy / 7 + 3;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[173], (Cal)+(mu_int32)((mu_float)Cal / (mu_float)((mu_float)SkillAttribute[CharacterAttribute->Skill[Type]].Damage / (mu_float)10)));
			}
			break;
		case AT_SKILL_ATTACK:mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[173], Energy / 7 + 3); break;
		case AT_SKILL_RECOVER:
			{
				mu_int32 Cal = Energy / 4;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1782], (mu_int32)((mu_float)Cal + (mu_float)CharacterAttribute->Level));
			}
			break;
		default:Success = false;
		}
		if (Success)
		{
			TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
		}
	}

	if (SkillType != AT_SKILL_SWELL_OF_MAGICPOWER)
	{
		if (Distance)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[174], Distance);
			TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;//++SkipNum;
		}
	}

	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[175], Mana);
	TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	if (SkillMana > 0)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[360], SkillMana);
		TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
	}
	if (GetBaseClass(Hero->Class) == GAME::CLASS_KNIGHT)
	{
		if (CharacterAttribute->Skill[Type] == AT_SKILL_SPEAR)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[96]);
			TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
		}

		if (Hero->byExtensionSkill == 1 && CharacterAttribute->Level >= 220)
		{
			if ((CharacterAttribute->Skill[Type] >= AT_SKILL_SWORD1 && CharacterAttribute->Skill[Type] <= AT_SKILL_SWORD5)
				|| CharacterAttribute->Skill[Type] == AT_SKILL_WHEEL || CharacterAttribute->Skill[Type] == AT_SKILL_FURY_STRIKE
				|| CharacterAttribute->Skill[Type] == AT_SKILL_ONETOONE
				|| (AT_SKILL_ANGER_SWORD_UP <= CharacterAttribute->Skill[Type] && CharacterAttribute->Skill[Type] <= AT_SKILL_ANGER_SWORD_UP + 4)
				|| (AT_SKILL_BLOW_UP <= CharacterAttribute->Skill[Type] && CharacterAttribute->Skill[Type] <= AT_SKILL_BLOW_UP + 4)
				|| (AT_SKILL_TORNADO_SWORDA_UP <= CharacterAttribute->Skill[Type] && CharacterAttribute->Skill[Type] <= AT_SKILL_TORNADO_SWORDA_UP + 4)
				|| (AT_SKILL_TORNADO_SWORDB_UP <= CharacterAttribute->Skill[Type] && CharacterAttribute->Skill[Type] <= AT_SKILL_TORNADO_SWORDB_UP + 4)
				)
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[99]);
				TextListColor[TextNum] = TEXT_COLOR_DARKRED;
				TextBold[TextNum] = false;
				++TextNum;
			}
			else if (CharacterAttribute->Skill[Type] == AT_SKILL_BLOW_OF_DESTRUCTION)
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2115]);
				TextListColor[TextNum] = TEXT_COLOR_DARKRED;
				TextBold[TextNum] = false;
				++TextNum;
			}
		}
	}

	mu_uint8 MasteryType = CharacterMachine->GetSkillMasteryType(CharacterAttribute->Skill[Type]);
	if (MasteryType != 255)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1080 + MasteryType]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;
	}


	mu_int32 SkillUseType;
	mu_int32 BrandType = SkillAttribute[SkillType].SkillBrand;
	SkillUseType = SkillAttribute[SkillType].SkillUseType;
	if (SkillUseType == SKILL_USE_TYPE_BRAND)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1480], SkillAttribute[BrandType].Name);
		TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1481], SkillAttribute[BrandType].Damage);
		TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
	}
	SkillUseType = SkillAttribute[SkillType].SkillUseType;
	if (SkillUseType == SKILL_USE_TYPE_MASTER)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1482]);
		TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1483], SkillAttribute[SkillType].KillCount);
		TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
	}

	if (GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD)
	{
		if (CharacterAttribute->Skill[Type] == AT_SKILL_PARTY_TELEPORT && PartyNumber <= 0)
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1185]);
			TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
		}
	}

	if (CharacterAttribute->Skill[Type] == AT_SKILL_PLASMA_STORM_FENRIR)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1926]);
		TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1927]);
		TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
	}

	if (CharacterAttribute->Skill[Type] == AT_SKILL_INFINITY_ARROW)
	{
		mu_sprintf_s(TextList[1], TEXTLIST_LENGTH, Name);
		TextListColor[1] = TEXT_COLOR_BLUE; TextBold[1] = true;
		mu_sprintf_s(TextList[2], TEXTLIST_LENGTH, _T("\n"));
		mu_sprintf_s(TextList[3], TEXTLIST_LENGTH, GlobalText[2040]);
		TextListColor[3] = TEXT_COLOR_DARKRED; TextBold[3] = false;
		mu_sprintf_s(TextList[4], TEXTLIST_LENGTH, GlobalText[175], Mana);
		TextListColor[4] = TEXT_COLOR_WHITE; TextBold[4] = false;
		mu_sprintf_s(TextList[5], TEXTLIST_LENGTH, GlobalText[360], SkillMana);
		TextListColor[5] = TEXT_COLOR_WHITE; TextBold[5] = false;
		TextNum = 6; SkipNum = 2;
	}

	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum; ++SkipNum;

	if (CharacterAttribute->Skill[Type] == AT_SKILL_RUSH || CharacterAttribute->Skill[Type] == AT_SKILL_SPACE_SPLIT
		|| CharacterAttribute->Skill[Type] == AT_SKILL_DEEPIMPACT || CharacterAttribute->Skill[Type] == AT_SKILL_JAVELIN
		|| CharacterAttribute->Skill[Type] == AT_SKILL_ONEFLASH || CharacterAttribute->Skill[Type] == AT_SKILL_DEATH_CANNON)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2047]);
		TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
	}
	if (CharacterAttribute->Skill[Type] == AT_SKILL_STUN || CharacterAttribute->Skill[Type] == AT_SKILL_REMOVAL_STUN
		|| CharacterAttribute->Skill[Type] == AT_SKILL_INVISIBLE || CharacterAttribute->Skill[Type] == AT_SKILL_REMOVAL_INVISIBLE
		|| CharacterAttribute->Skill[Type] == AT_SKILL_REMOVAL_BUFF)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2048]);
		TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
	}
	if (CharacterAttribute->Skill[Type] == AT_SKILL_SPEAR)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2049]);
		TextListColor[TextNum] = TEXT_COLOR_DARKRED; TextBold[TextNum] = false; ++TextNum;
	}

	if (SkillType == AT_SKILL_SWELL_OF_MAGICPOWER)
	{
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2054]);
		TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; ++TextNum;
	}

	RenderTipTextList(sx, sy, TextNum, 0, ETA_CENTER, iRenderPoint);
}