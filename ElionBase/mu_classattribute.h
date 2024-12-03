#ifndef __MU_CLASSATTRIBUTE_H__
#define __MU_CLASSATTRIBUTE_H__

#pragma once

typedef struct
{
	mu_uint16 Strength;
	mu_uint16 Dexterity;
	mu_uint16 Vitality;
	mu_uint16 Energy;
	mu_uint16 Life;
	mu_uint16 Mana;
	mu_uint16 Shield;
	mu_uint8 LevelLife;
	mu_uint8 LevelMana;
	mu_uint8 VitalityToLife;
	mu_uint8 EnergyToMana;
} CLASS_ATTRIBUTE;

#endif