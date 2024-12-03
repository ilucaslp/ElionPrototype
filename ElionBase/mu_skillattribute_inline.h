#ifndef __MU_SKILLATTRIBUTE_INLINE_H__
#define __MU_SKILLATTRIBUTE_INLINE_H__

#pragma once

EINLINE mu_boolean IsValidateSkillIdx(mu_int32 iSkillIdx)
{
	if (iSkillIdx < 0 || iSkillIdx > MAX_SKILLS - 1)
	{
		return false;
	}

	return true;
}

EINLINE mu_boolean IsCorrectSkillType(mu_int32 iSkillSeq, eTypeSkill iSkillTypeIdx)
{
	if (IsValidateSkillIdx(iSkillSeq) == false)
	{
		return false;
	}

	SKILL_ATTRIBUTE &CurSkillAttribute = SkillAttribute[iSkillSeq];

	if (CurSkillAttribute.TypeSkill == (mu_uint8)iSkillTypeIdx)
	{
		return true;
	}

	return false;
}

EINLINE mu_boolean IsCorrectSkillType_FrendlySkill(mu_int32 iSkillSeq)
{
	return IsCorrectSkillType(iSkillSeq, eTypeSkill_FrendlySkill);
}

EINLINE mu_boolean IsCorrectSkillType_Buff(mu_int32 iSkillSeq)
{
	return IsCorrectSkillType(iSkillSeq, eTypeSkill_Buff);
}

EINLINE mu_boolean IsCorrectSkillType_DeBuff(mu_int32 iSkillSeq)
{
	return IsCorrectSkillType(iSkillSeq, eTypeSkill_DeBuff);
}

EINLINE mu_boolean IsCorrectSkillType_CommonAttack(mu_int32 iSkillSeq)
{
	return IsCorrectSkillType(iSkillSeq, eTypeSkill_CommonAttack);
}

EINLINE mu_int32 MasterSkillToBaseSkillIndex(mu_int32 iMasterSkillIndex)
{
	mu_int32 iBaseSkillIndex = iMasterSkillIndex;

	switch (iMasterSkillIndex)
	{
	case AT_SKILL_EVIL_SPIRIT_UP:
	case AT_SKILL_EVIL_SPIRIT_UP + 1:
	case AT_SKILL_EVIL_SPIRIT_UP + 2:
	case AT_SKILL_EVIL_SPIRIT_UP + 3:
	case AT_SKILL_EVIL_SPIRIT_UP + 4:
	case AT_SKILL_EVIL_SPIRIT_UP_M:
	case AT_SKILL_EVIL_SPIRIT_UP_M + 1:
	case AT_SKILL_EVIL_SPIRIT_UP_M + 2:
	case AT_SKILL_EVIL_SPIRIT_UP_M + 3:
	case AT_SKILL_EVIL_SPIRIT_UP_M + 4:
		{
			iBaseSkillIndex = AT_SKILL_EVIL;
		}
		break;
	case AT_SKILL_SOUL_UP:
	case AT_SKILL_SOUL_UP + 1:
	case AT_SKILL_SOUL_UP + 2:
	case AT_SKILL_SOUL_UP + 3:
	case AT_SKILL_SOUL_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_WIZARDDEFENSE;
		}
		break;
	case AT_SKILL_HELL_FIRE_UP:
	case AT_SKILL_HELL_FIRE_UP + 1:
	case AT_SKILL_HELL_FIRE_UP + 2:
	case AT_SKILL_HELL_FIRE_UP + 3:
	case AT_SKILL_HELL_FIRE_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_BLAST_HELL;
		}
		break;
	case AT_SKILL_ICE_UP:
	case AT_SKILL_ICE_UP + 1:
	case AT_SKILL_ICE_UP + 2:
	case AT_SKILL_ICE_UP + 3:
	case AT_SKILL_ICE_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_BLAST_FREEZE;
		}
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
		{
			iBaseSkillIndex = AT_SKILL_WHEEL;
		}
		break;
	case AT_SKILL_BLOW_UP:
	case AT_SKILL_BLOW_UP + 1:
	case AT_SKILL_BLOW_UP + 2:
	case AT_SKILL_BLOW_UP + 3:
	case AT_SKILL_BLOW_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_ONETOONE;
		}
		break;
	case AT_SKILL_ANGER_SWORD_UP:
	case AT_SKILL_ANGER_SWORD_UP + 1:
	case AT_SKILL_ANGER_SWORD_UP + 2:
	case AT_SKILL_ANGER_SWORD_UP + 3:
	case AT_SKILL_ANGER_SWORD_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_FURY_STRIKE;
		}
		break;
	case AT_SKILL_LIFE_UP:
	case AT_SKILL_LIFE_UP + 1:
	case AT_SKILL_LIFE_UP + 2:
	case AT_SKILL_LIFE_UP + 3:
	case AT_SKILL_LIFE_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_VITALITY;
		}
		break;
	case AT_SKILL_HEAL_UP:
	case AT_SKILL_HEAL_UP + 1:
	case AT_SKILL_HEAL_UP + 2:
	case AT_SKILL_HEAL_UP + 3:
	case AT_SKILL_HEAL_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_HEALING;
		}
		break;
	case AT_SKILL_DEF_POWER_UP:
	case AT_SKILL_DEF_POWER_UP + 1:
	case AT_SKILL_DEF_POWER_UP + 2:
	case AT_SKILL_DEF_POWER_UP + 3:
	case AT_SKILL_DEF_POWER_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_DEFENSE;
		}
		break;
	case AT_SKILL_ATT_POWER_UP:
	case AT_SKILL_ATT_POWER_UP + 1:
	case AT_SKILL_ATT_POWER_UP + 2:
	case AT_SKILL_ATT_POWER_UP + 3:
	case AT_SKILL_ATT_POWER_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_ATTACK;
		}
		break;
	case AT_SKILL_MANY_ARROW_UP:
	case AT_SKILL_MANY_ARROW_UP + 1:
	case AT_SKILL_MANY_ARROW_UP + 2:
	case AT_SKILL_MANY_ARROW_UP + 3:
	case AT_SKILL_MANY_ARROW_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_CROSSBOW;
		}
		break;
	case AT_SKILL_BLOOD_ATT_UP:
	case AT_SKILL_BLOOD_ATT_UP + 1:
	case AT_SKILL_BLOOD_ATT_UP + 2:
	case AT_SKILL_BLOOD_ATT_UP + 3:
	case AT_SKILL_BLOOD_ATT_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_REDUCEDEFENSE;
		}
		break;
	case AT_SKILL_POWER_SLASH_UP:
	case AT_SKILL_POWER_SLASH_UP + 1:
	case AT_SKILL_POWER_SLASH_UP + 2:
	case AT_SKILL_POWER_SLASH_UP + 3:
	case AT_SKILL_POWER_SLASH_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_ICE_BLADE;
		}
		break;
	case AT_SKILL_BLAST_UP:
	case AT_SKILL_BLAST_UP + 1:
	case AT_SKILL_BLAST_UP + 2:
	case AT_SKILL_BLAST_UP + 3:
	case AT_SKILL_BLAST_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_BLAST;
		}
		break;
	case AT_SKILL_ASHAKE_UP:
	case AT_SKILL_ASHAKE_UP + 1:
	case AT_SKILL_ASHAKE_UP + 2:
	case AT_SKILL_ASHAKE_UP + 3:
	case AT_SKILL_ASHAKE_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_DARK_HORSE;
		}
		break;
	case AT_SKILL_FIRE_BUST_UP:
	case AT_SKILL_FIRE_BUST_UP + 1:
	case AT_SKILL_FIRE_BUST_UP + 2:
	case AT_SKILL_FIRE_BUST_UP + 3:
	case AT_SKILL_FIRE_BUST_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_LONGPIER_ATTACK;
		}
		break;
	case AT_SKILL_FIRE_SCREAM_UP:
	case AT_SKILL_FIRE_SCREAM_UP + 1:
	case AT_SKILL_FIRE_SCREAM_UP + 2:
	case AT_SKILL_FIRE_SCREAM_UP + 3:
	case AT_SKILL_FIRE_SCREAM_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_DARK_SCREAM;
		}
		break;
	case AT_SKILL_ALICE_SLEEP_UP:
	case AT_SKILL_ALICE_SLEEP_UP + 1:
	case AT_SKILL_ALICE_SLEEP_UP + 2:
	case AT_SKILL_ALICE_SLEEP_UP + 3:
	case AT_SKILL_ALICE_SLEEP_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_ALICE_SLEEP;
		}
		break;
	case AT_SKILL_ALICE_CHAINLIGHTNING_UP:
	case AT_SKILL_ALICE_CHAINLIGHTNING_UP + 1:
	case AT_SKILL_ALICE_CHAINLIGHTNING_UP + 2:
	case AT_SKILL_ALICE_CHAINLIGHTNING_UP + 3:
	case AT_SKILL_ALICE_CHAINLIGHTNING_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_ALICE_CHAINLIGHTNING;
		}
		break;
	case AT_SKILL_LIGHTNING_SHOCK_UP:
	case AT_SKILL_LIGHTNING_SHOCK_UP + 1:
	case AT_SKILL_LIGHTNING_SHOCK_UP + 2:
	case AT_SKILL_LIGHTNING_SHOCK_UP + 3:
	case AT_SKILL_LIGHTNING_SHOCK_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_ALICE_LIGHTNINGORB;
		}
		break;
	case AT_SKILL_ALICE_DRAINLIFE_UP:
	case AT_SKILL_ALICE_DRAINLIFE_UP + 1:
	case AT_SKILL_ALICE_DRAINLIFE_UP + 2:
	case AT_SKILL_ALICE_DRAINLIFE_UP + 3:
	case AT_SKILL_ALICE_DRAINLIFE_UP + 4:
		{
			iBaseSkillIndex = AT_SKILL_ALICE_DRAINLIFE;
		}
		break;
	}

	return iBaseSkillIndex;
}

EINLINE mu_boolean DemendConditionCheckSkill(mu_uint16 SkillType)
{
	if (SkillType >= MAX_SKILLS)
	{
		return false;
	}

	mu_int32 baseSkillType = MasterSkillToBaseSkillIndex(SkillType);
	SKILL_ATTRIBUTE &Skill = SkillAttribute[SkillType];
	SKILL_ATTRIBUTE &BaseSkill = SkillAttribute[baseSkillType];

	if (IsEmpireGuardian() == true &&
		(SkillType == AT_SKILL_TELEPORT_B || SkillType == AT_SKILL_TELEPORT))
	{
		return false;
	}

	if (Skill.Energy == 0)
	{
		return true;
	}

	mu_uint32 SkillEnergy = (20 + ((mu_uint32)BaseSkill.Energy * (mu_uint32)BaseSkill.Level) / 25);

	if (CharacterAttribute->Level < BaseSkill.Level ||
		CharacterAttribute->Strength + CharacterAttribute->AddStrength < BaseSkill.Strength ||
		CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity < BaseSkill.Dexterity ||
		CharacterAttribute->Energy + CharacterAttribute->AddEnergy < SkillEnergy ||
		CharacterAttribute->Charisma + CharacterAttribute->AddCharisma < BaseSkill.Charisma)
	{
		return false;
	}

	return true;
}

EINLINE mu_boolean IsCanBCSkill(mu_uint16 SkillType)
{
	switch (SkillType)
	{
	case AT_SKILL_RUSH:
	case AT_SKILL_JAVELIN:
	case AT_SKILL_DEEPIMPACT:
	case AT_SKILL_ONEFLASH:
	case AT_SKILL_DEATH_CANNON:
	case AT_SKILL_SPACE_SPLIT:
		{
			if (battleCastle::InBattleCastle() == false ||
				battleCastle::IsBattleCastleStart() == false)
			{
				return false;
			}
		}
		break;
	}

	return true;
}

EINLINE mu_boolean CheckSkillUseCondition(OBJECT* o, mu_int32 Type)
{
#if 0 // FIX
	if (g_bExit) return false;
#endif

	if (IsCanBCSkill(Type) == false)
	{
		return false;
	}

	mu_uint8 SkillUseType = SkillAttribute[Type].SkillUseType;

	if (SkillUseType == SKILL_USE_TYPE_BRAND && g_isCharacterBuff(o, eBuff_AddSkill) == false)
	{
		return false;
	}

	return true;
}

#endif