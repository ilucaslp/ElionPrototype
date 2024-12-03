#ifndef __MU_ITEMATTRIBUTE_H__
#define __MU_ITEMATTRIBUTE_H__

#pragma once

class ITEM;

class ITEM_ATTRIBUTE_BASE
{
public:
	mu_boolean TwoHand;
	mu_uint16 Level;
	mu_uint8 m_byItemSlot;
	mu_uint16 m_bySkillIndex;
	mu_uint8 Width;
	mu_uint8 Height;
	mu_uint16 DamageMin;
	mu_uint16 DamageMax;
	mu_uint16 SuccessfulBlocking;
	mu_uint16 Defense;
	mu_uint16 MagicDefense;
	mu_uint16 WeaponSpeed;
	mu_uint16 WalkSpeed;
	mu_uint16 Durability;
	mu_uint16 MagicDur;
	mu_uint16 MagicPower;
	mu_uint16 RequireStrength;
	mu_uint16 RequireDexterity;
	mu_uint16 RequireEnergy;
	mu_uint16 RequireVitality;
	mu_uint16 RequireCharisma;
	mu_uint16 RequireLevel;
	mu_uint8 Value;
	mu_int32 iZen;
	mu_uint8 AttType;
	mu_uint8 RequireClass[GAME::MAX_CLASS];
	mu_uint8 Resistance[MAX_RESISTANCE];
};

class ITEM_ATTRIBUTE : public ITEM_ATTRIBUTE_BASE
{
public:
	mu_string Name;
};

extern ITEM_ATTRIBUTE ItemAttribute[MAX_ITEM];

mu_boolean OpenItemScript(const mu_text *filename);

mu_float CalcDurabilityPercent(mu_uint8 dur, mu_uint8 maxDur, mu_int32 Level, mu_int32 ExcOption, mu_int32 SetOption = 0);
mu_boolean IsRequireEquipItem(ITEM* pItem);
void PlusSpecial(mu_uint16 *Value, mu_int32 Special, ITEM *Item);
void PlusSpecialPercent(mu_uint16 *Value, mu_int32 Special, ITEM *Item, mu_uint16 Percent);
void PlusSpecialPercent2(mu_uint16 *Value, mu_int32 Special, ITEM *Item);
mu_uint16 ItemDefense(ITEM *Item);
mu_uint16 ItemMagicDefense(ITEM *Item);
mu_uint16 ItemWalkSpeed(ITEM *Item);

mu_uint16 calcMaxDurability(const ITEM* ip, ITEM_ATTRIBUTE *p, mu_int32 Level);
mu_int64 ItemValue(ITEM *ip, mu_int32 goldType);

mu_boolean GetAttackDamage(mu_int32* iMinDamage, mu_int32* iMaxDamage);

mu_uint8 getSkillIndexByBook(mu_int32 Type);

void GetSpecialOptionText(mu_int32 Type, mu_unicode *Text, const mu_size TextLength, mu_uint16 Option, mu_uint8 Value, mu_int32 iMana);

mu_boolean IsInvenItem(const mu_int16 sType);
mu_boolean IsRequireClassRenderItem(const mu_int16 sType);
mu_boolean IsPartChargeItem(ITEM* pItem);
mu_boolean IsHighValueItem(ITEM* pItem);
mu_boolean IsPersonalShopBan(ITEM* pItem);
mu_boolean IsTradeBan(ITEM* pItem);
mu_boolean IsStoreBan(ITEM* pItem);
mu_boolean IsDropBan(ITEM* pItem);
mu_boolean IsSellingBan(ITEM* pItem);
mu_boolean IsRepairBan(ITEM* pItem);
mu_boolean IsWingItem(ITEM* pItem);
mu_boolean IsCepterItem(mu_int32 iType);

void GetItemName(mu_int32 iType, mu_int32 iLevel, mu_text *Text, mu_size TextCount);

void RequireClass(ITEM_ATTRIBUTE* pItem);
void RenderItemInfo(mu_int32 sx, mu_int32 sy, ITEM *ip, mu_boolean Sell, mu_boolean useAlternativePos, mu_int32 Inventype = 0, mu_boolean bItemTextListBoxUse = false, mu_boolean bIsEquippedItem = false, mu_boolean useBG = true);
void RenderRepairInfo(mu_int32 sx, mu_int32 sy, ITEM *ip, mu_boolean Sell, mu_boolean useAlternativePos);
void RenderSkillInfo(mu_int32 sx, mu_int32 sy, mu_int32 Type, mu_int32 SkillNum = 0, mu_int32 iRenderPoint = STRP_NONE);

#endif