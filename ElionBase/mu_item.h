#ifndef __MU_ITEM_H__
#define __MU_ITEM_H__

#pragma once

class OBJECT;

class ITEM
{
public:
	// Internal
	mu_uint32 Key;

	// Received Values
	mu_int16 Type;

	mu_uint16 Level;
	mu_uint16 Durability;

	union
	{
		struct 
		{
			mu_uint8 Option;
			mu_uint8 ExcOption;
			mu_uint8 SetOption;
		};

		mu_int64 Money;
	};

	mu_uint8 HarmonyOption;
	mu_uint8 HarmonyOptionLevel;
	mu_uint8 SocketSeedSetOption;
	mu_uint8 SocketOption[MAX_SOCKETS];

	mu_boolean Skill;
	mu_boolean Luck;
	mu_boolean ItemOption;

	// Calculated Values
	mu_uint16 DamageMin;
	mu_uint16 DamageMax;
	mu_uint16 Defense;
	mu_uint16 MagicDefense;
	mu_uint16 WeaponSpeed;
	mu_uint16 WalkSpeed;
	mu_uint16 AttackSuccess;

	mu_uint16 RequireStrength;
	mu_uint16 RequireDexterity;
	mu_uint16 RequireEnergy;
	mu_uint16 RequireVitality;
	mu_uint16 RequireCharisma;
	mu_uint16 RequireLevel;

	mu_uint8 Part;
	mu_uint8 MagicPower;
	mu_uint8 SuccessfulBlocking;
	mu_uint8 SpecialNum;
	mu_uint16 Special[MAX_ITEM_SPECIAL];
	mu_uint8 SpecialValue[MAX_ITEM_SPECIAL];

	mu_uint8 bySelectedSlotIndex;

	union
	{
		mu_uint8  x;
		mu_uint8  lineal_pos;
	};
	union
	{
		mu_uint8  y;
		mu_uint8  ex_src_type;
	};

	mu_uint8 SocketCount;
	mu_uint8 SocketSeedID[MAX_SOCKETS];
	mu_uint8 SocketSphereLv[MAX_SOCKETS];
	mu_int32 Number;
	mu_uint8 Color;

	mu_uint8 byColorState;

	mu_int32 PetLevel;
	mu_int64 PetExperience;
	mu_int64 NextPetExperience;

	mu_uint32 RefCount;
};

typedef struct
{
	mu_int16  Key;
	ITEM   Item;
	OBJECT Object;
} ITEM_t;

extern std::array<mu_int32, 100> RandomTable;
extern std::array<ITEM_t, MAX_ITEMS> Items;
extern std::array<mu_uint16, MAX_ITEMS + MAX_TASKING_THREADS> g_ItemsQueue;

void ParseItem(SHARED::ITEM_DATA &Source, ITEM *Target);
void CreateMoney(ITEM_t *ip, mu_int64 Money, EVector3 Position, mu_boolean CreateFlag);
void CreateItem(ITEM_t *ip, SHARED::ITEM_DATA &Item, EVector3 Position, mu_boolean CreateFlag);

void ClearItems();

void ItemAngle(OBJECT *o);

mu_int32 CompareItem(ITEM *item1, ITEM *item2);

#endif