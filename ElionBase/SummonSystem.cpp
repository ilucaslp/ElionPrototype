#include "stdafx.h"
#include "SummonSystem.h"

CSummonSystem g_SummonSystem;

CSummonSystem::CSummonSystem()
{

}

CSummonSystem::~CSummonSystem()
{
	m_EquipEffectRandom.clear();
}

void CSummonSystem::MoveEquipEffect(const mu_uint32 ThreadIndex, CHARACTER * pCharacter, mu_int32 iItemType, mu_int32 iItemLevel,mu_int32 iItemExcOption)
{
	if (iItemType >= MODEL_STAFF+21 && iItemType <= MODEL_STAFF+29)
	{
		CreateEquipEffect_WristRing(ThreadIndex, pCharacter, iItemType, iItemLevel, iItemExcOption);
		CreateEquipEffect_Summon(ThreadIndex, pCharacter, iItemType, iItemLevel, iItemExcOption);
	}
	else
	{
		RemoveEquipEffect_WristRing(pCharacter);
		RemoveEquipEffect_Summon(pCharacter);
	}
}

void CSummonSystem::RemoveEquipEffects(CHARACTER * pCharacter)
{
	RemoveEquipEffect_WristRing(pCharacter);
	RemoveEquipEffect_Summon(pCharacter);
}

void CSummonSystem::SetPlayerSummon(CHARACTER * pCharacter, OBJECT * pObject)
{
	switch(pCharacter->Helper.Type)
	{
	case MODEL_HELPER+2:
		SetAction(pObject, PLAYER_SKILL_SUMMON_UNI);
		break;
	case MODEL_HELPER+3:
		SetAction(pObject, PLAYER_SKILL_SUMMON_DINO);
		break;
	case MODEL_HELPER+37:
		SetAction(pObject, PLAYER_SKILL_SUMMON_FENRIR);
		break;
	default:
		SetAction(pObject, PLAYER_SKILL_SUMMON);
		break;
	}
}

void CSummonSystem::CastSummonSkill(const mu_uint32 ThreadIndex, mu_int32 iSkill, CHARACTER * pCharacter, OBJECT * pObject, mu_int32 iTargetPos_X, mu_int32 iTargetPos_Y)
{
 	pObject->Angle[2] = CreateAngle(pObject->Position[0], pObject->Position[1], (mu_float)iTargetPos_X*g_TerrainScale, (mu_float)iTargetPos_Y*g_TerrainScale);
	SetPlayerSummon(pCharacter, pObject);
	CreateCastingEffect(ThreadIndex, pObject->Position, pObject->Angle, iSkill);
	RemoveEquipEffect_Summon(pCharacter);
	CreateSummonObject(ThreadIndex, iSkill, pCharacter, pObject, (mu_float)iTargetPos_X*g_TerrainScale, (mu_float)iTargetPos_Y*g_TerrainScale);
}

mu_boolean CSummonSystem::SendRequestSummonSkill(const mu_uint32 ThreadIndex, mu_int32 iSkill, CHARACTER * pCharacter, OBJECT * pObject)
{
	if (iSkill < AT_SKILL_SUMMON_EXPLOSION || iSkill > AT_SKILL_SUMMON_POLLUTION)
		return false;

	CheckTarget(ThreadIndex, pCharacter);

	mu_int32 iTargetKey = -1;
	if (CheckAttack())
	{
		iTargetKey = CharactersClient[SelectedCharacter].Key;
	}
	if (iTargetKey != -1)
	{
		CHARACTER * pTargetCharacter = &CharactersClient[SelectedCharacter];
		TargetX = (mu_int32)(pTargetCharacter->Object.Position[0] * g_TerrainScaleRev);
		TargetY = (mu_int32)(pTargetCharacter->Object.Position[1] * g_TerrainScaleRev);
	}

#if 0 // FIX
	g_MovementSkill.m_bMagic = false;
	g_MovementSkill.m_iSkill = iSkill;
	g_MovementSkill.m_iTarget = SelectedCharacter;

	mu_float fDistance = GetSkillDistance(iSkill, pCharacter);

	if (CheckTile(pCharacter, pObject, fDistance))
	{
		if (iTargetKey != -1)
		{
			SendRequestMagicContinue(iSkill, (mu_uint8)(pCharacter->TargetPosition[0] * TerrainScaleRev), (mu_uint8)(pCharacter->TargetPosition[1] * TerrainScaleRev),
				(mu_uint8)(pObject->Angle[2] / 360.0f*256.0f), 0, 0, (mu_uint16)iTargetKey, 0);
		}
		else
		{
			SendRequestMagicContinue(iSkill, (mu_uint8)TargetX, (mu_uint8)TargetY,
				(mu_uint8)(pObject->Angle[2] / 360.0f*256.0f), 0, 0, 0, 0);
		}
		CastSummonSkill(ThreadIndex, iSkill, pCharacter, pObject, TargetX, TargetY);
	}
#endif

	return false;
}

void CSummonSystem::CreateSummonObject(const mu_uint32 ThreadIndex, mu_int32 iSkill, CHARACTER * pCharacter, OBJECT * pObject, mu_float fTargetPos_X, mu_float fTargetPos_Y)
{
	PART_t * pWeapon = &pCharacter->Weapon[1];
	mu_int32 iSummonLevel = pWeapon->Level;
	if (iSummonLevel >= 11) iSummonLevel = 2;
	else if (iSummonLevel >= 7) iSummonLevel = 1;
	else iSummonLevel = 0;

	switch (iSkill)
	{
	case AT_SKILL_SUMMON_EXPLOSION:
		{
			EVector3 vPos;
			VectorCopy(pObject->Position, vPos);
			if (GetLargeRand(2) == 0) vPos[0] += GetLargeRand(300) + 150;
			else vPos[0] -= GetLargeRand(250) + 150;
			if (GetLargeRand(2) == 0) vPos[1] += GetLargeRand(300) + 150;
			else vPos[1] -= GetLargeRand(250) + 150;

			EVector3 vTargetPos;
			Vector(fTargetPos_X, fTargetPos_Y, RequestTerrainHeight(fTargetPos_X, fTargetPos_Y), vTargetPos);

			CreateEffect(ThreadIndex, MODEL_SUMMONER_SUMMON_SAHAMUTT, vPos, pObject->Angle, vTargetPos, iSummonLevel);

			PlayBackground(SOUND_SUMMON_SAHAMUTT);
		}
		break;
	case AT_SKILL_SUMMON_REQUIEM:
		{
			EBone Matrix;
			EVector3 vMoveDir, vPosition;
			AngleMatrix(pObject->Angle, Matrix);
			Vector(0, -100.0f, 0, vMoveDir);
			VectorRotate(vMoveDir, Matrix, vPosition);
			VectorAdd(pObject->Position, vPosition, vPosition);

			EVector3 vTargetPos;
			Vector(fTargetPos_X, fTargetPos_Y, RequestTerrainHeight(fTargetPos_X, fTargetPos_Y), vTargetPos);

			CreateEffect(ThreadIndex, MODEL_SUMMONER_SUMMON_NEIL, vPosition, pObject->Angle, vTargetPos, iSummonLevel);

			PlayBackground(SOUND_SUMMON_NEIL);
		}
		break;
	case AT_SKILL_SUMMON_POLLUTION:
		{
			EVector3 vTargetPos;
			Vector(fTargetPos_X, fTargetPos_Y, RequestTerrainHeight(fTargetPos_X, fTargetPos_Y), vTargetPos);
			EVector3 vLight, vAngle;

			Vector(1.0f, 1.0f, 1.0f, vLight);
			Vector(1.0f, 1.0f, 1.0f, vAngle);
			CreateEffect(ThreadIndex, MODEL_SUMMONER_SUMMON_LAGUL, vTargetPos, vAngle, vLight, 0, nullptr, iSummonLevel);

			mu_int32 anRargle[3] = { 1, 2, 4 };
			for (mu_int32 i = 0; i < anRargle[iSummonLevel]; ++i)
			{
				Vector(0.0f, 0.0f, i*90.0f, vAngle);
				CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, vTargetPos, vTargetPos, vAngle, 24, nullptr, 100.0f);
				CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, vTargetPos, vTargetPos, vAngle, 24, nullptr, 20.0f);
			}
			PlayBackground(SOUND_SUMMOM_RARGLE);
		}
		break;
	}
}
	
void CSummonSystem::CreateCastingEffect(const mu_uint32 ThreadIndex, EVector3 &vPosition, EVector3 &vAngle, mu_int32 iSubType)
{
	EVector3 vLight;

	Vector(1.0f, 1.0f, 1.0f, vLight);
	CreateEffect(ThreadIndex, BITMAP_MAGIC, vPosition, vAngle, vLight, 10);

	switch (iSubType)
	{
	case AT_SKILL_SUMMON_EXPLOSION:	Vector(1.0f, 0.6f, 0.4f, vLight); break;
	case AT_SKILL_SUMMON_REQUIEM:	Vector(0.7f, 0.7f, 1.0f, vLight); break;
	case AT_SKILL_SUMMON_POLLUTION:	Vector(0.6f, 0.6f, 0.9f, vLight); break;
	default: Vector(0.7f, 0.7f, 1.0f, vLight); break;
	}

	CreateEffect(ThreadIndex, BITMAP_MAGIC, vPosition, vAngle, vLight, 9);

	switch (iSubType)
	{
	case AT_SKILL_SUMMON_EXPLOSION:	Vector(1.0f, 0.5f, 0.0f, vLight); break;
	case AT_SKILL_SUMMON_REQUIEM:	Vector(0.0f, 0.7f, 1.0f, vLight); break;
	case AT_SKILL_SUMMON_POLLUTION:	Vector(0.6f, 0.3f, 0.9f, vLight); break;
	default: Vector(0.0f, 0.7f, 1.0f, vLight); break;
	}
	CreateEffect(ThreadIndex, MODEL_SUMMONER_CASTING_EFFECT1, vPosition, vAngle, vLight);
	CreateEffect(ThreadIndex, MODEL_SUMMONER_CASTING_EFFECT11, vPosition, vAngle, vLight);
	CreateEffect(ThreadIndex, MODEL_SUMMONER_CASTING_EFFECT111, vPosition, vAngle, vLight);
	switch (iSubType)
	{
	case AT_SKILL_SUMMON_EXPLOSION:	Vector(1.0f, 0.5f, 0.0f, vLight); break;
	case AT_SKILL_SUMMON_REQUIEM:	Vector(0.0f, 0.0f, 1.0f, vLight); break;
	case AT_SKILL_SUMMON_POLLUTION:	Vector(0.8f, 0.1f, 0.6f, vLight); break;
	default: Vector(0.0f, 0.0f, 1.0f, vLight); break;
	}
	CreateEffect(ThreadIndex, MODEL_SUMMONER_CASTING_EFFECT2, vPosition, vAngle, vLight);
	CreateEffect(ThreadIndex, MODEL_SUMMONER_CASTING_EFFECT22, vPosition, vAngle, vLight);
	CreateEffect(ThreadIndex, MODEL_SUMMONER_CASTING_EFFECT222, vPosition, vAngle, vLight);
	switch (iSubType)
	{
	case AT_SKILL_SUMMON_EXPLOSION:	Vector(1.0f, 0.5f, 0.8f, vLight); break;
	case AT_SKILL_SUMMON_REQUIEM:	Vector(0.8f, 0.5f, 1.0f, vLight); break;
	case AT_SKILL_SUMMON_POLLUTION:	Vector(0.9f, 0.1f, 1.0f, vLight); break;
	default: Vector(0.8f, 0.5f, 1.0f, vLight); break;
	}
	CreateEffect(ThreadIndex, MODEL_SUMMONER_CASTING_EFFECT4, vPosition, vAngle, vLight);
}

void CSummonSystem::CreateEquipEffect_WristRing(const mu_uint32 ThreadIndex, CHARACTER * pCharacter, mu_int32 iItemType, mu_int32 iItemLevel, mu_int32 iItemExcOption)
{
	OBJECT * pObject = &pCharacter->Object;
	MUModel * pModel = &MODELS::Data[pObject->Type];

	EVector3 vPos, vRelative, vLight;
	Vector(0.0f, 0.0f, 0.0f, vRelative);
	pModel->TransformPosition(ThreadIndex, pObject->BoneTransform[pModel->GetBoneIndex(37)], vRelative, vPos, true);

	switch (iItemLevel)
	{
	case 0: case 1: case 2:		Vector(1.0f, 1.0f, 0.0f, vLight);	break;
	case 3: case 4:				Vector(0.5f, 1.0f, 0.5f, vLight);	break;
	case 5: case 6:				Vector(0.5f, 0.1f, 1.0f, vLight);	break;
	case 7: case 8:				Vector(1.0f, 0.5f, 0.0f, vLight);	break;
	case 9: case 10:			Vector(1.0f, 0.2f, 0.2f, vLight);	break;
	case 11: case 12: case 13:	Vector(0.3f, 0.5f, 1.0f, vLight);	break;
	}

	if (!SearchJoint(MODEL_SPEARSKILL, pObject, 14))
	{
		for (mu_int32 i = 0; i < 4; ++i)
		{
			CreateJoint(ThreadIndex, MODEL_SPEARSKILL, vPos, vPos, pObject->Angle, 14, pObject, 18.0f, -1, 0, 0,
				pCharacter->TargetCharacter, &vLight);
		}

		if (iItemLevel >= 12)
		{
			CreateEffect(ThreadIndex, MODEL_SUMMONER_WRISTRING_EFFECT, vPos, pObject->Angle, vLight, 0, pObject);
		}
	}

	if (iItemLevel >= 5)
	{
		CreateSprite(BITMAP_FLARE, vPos, 0.7f, vLight, pObject);
	}
	if (iItemLevel >= 7)
	{
		CreateSprite(BITMAP_SHINY + 1, vPos, 0.8f, vLight, pObject);
	}
	if (iItemLevel >= 9)
	{
		CreateSprite(BITMAP_SHINY + 1, vPos, 1.0f, vLight, pObject, (mu_float)((mu_int32)(FWorldTime / 20) % 360));
	}
	if (iItemLevel >= 13)
	{
		if (GetLargeRand(4))
		{
			CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, pObject->Angle, vLight, 23, 0.5f, pObject);
		}
	}
}

void CSummonSystem::RemoveEquipEffect_WristRing(CHARACTER * pCharacter)
{
	OBJECT * pObject = &pCharacter->Object;

	DeleteJoint(MODEL_SPEARSKILL, pObject, 14);
	DeleteEffect(MODEL_SUMMONER_WRISTRING_EFFECT, pObject);
}

void CSummonSystem::CreateEquipEffect_Summon(const mu_uint32 ThreadIndex, CHARACTER * pCharacter, mu_int32 iItemType, mu_int32 iItemLevel, mu_int32 iItemExcOption)
{
	OBJECT * pObject = &pCharacter->Object;

	if (GET_WORLD == WD_74NEW_CHARACTER_SCENE) return;

	if (pObject->CurrentAction == PLAYER_SKILL_SUMMON || pObject->CurrentAction == PLAYER_SKILL_SUMMON_UNI
		|| pObject->CurrentAction == PLAYER_SKILL_SUMMON_DINO || pObject->CurrentAction == PLAYER_SKILL_SUMMON_FENRIR)
	{
		return;
	}

	EVector3 vLight;
	Vector(1.0f, 1.0f, 1.0f, vLight);

	mu_uint8 byRandom;
	std::map<mu_int16, mu_uint8>::iterator iter = m_EquipEffectRandom.find(pCharacter->Key);
	if (iter == m_EquipEffectRandom.end())
	{
		byRandom = GetLargeRand(256);
		m_EquipEffectRandom.insert(std::make_pair(pCharacter->Key, byRandom));
	}
	else
	{
		byRandom = iter->second;
	}

	switch (iItemType)
	{
	case MODEL_STAFF + 21:
		if (!SearchEffect(MODEL_SUMMONER_EQUIP_HEAD_SAHAMUTT, pObject, 0) && !pCharacter->SafeZone && EMath::Sin(FWorldTime*0.0004f + byRandom*0.024f) > 0.3f)
		{
			CreateEffect(ThreadIndex, MODEL_SUMMONER_EQUIP_HEAD_SAHAMUTT, pObject->Position, pObject->Angle, vLight, 0, pObject, -1, byRandom);
		}
		break;
	case MODEL_STAFF + 22:
		if (!SearchEffect(MODEL_SUMMONER_EQUIP_HEAD_NEIL, pObject, 0) && !pCharacter->SafeZone && EMath::Sin(FWorldTime*0.0004f + byRandom*0.024f) > 0.3f)
		{
			CreateEffect(ThreadIndex, MODEL_SUMMONER_EQUIP_HEAD_NEIL, pObject->Position, pObject->Angle, vLight, 0, pObject, -1, byRandom);
		}
		break;
	case MODEL_STAFF + 23:
		if (!SearchEffect(MODEL_SUMMONER_EQUIP_HEAD_LAGUL, pObject, 0) && !pCharacter->SafeZone && EMath::Sin(FWorldTime*0.0004f + byRandom*0.024f) > 0.3f)
		{
			CreateEffect(ThreadIndex, MODEL_SUMMONER_EQUIP_HEAD_LAGUL, pObject->Position, pObject->Angle, vLight, 0, pObject, -1, byRandom);
		}
		break;
	}
}

void CSummonSystem::RemoveEquipEffect_Summon(CHARACTER * pCharacter)
{
	OBJECT * pObject = &pCharacter->Object;
	DeleteEffect(MODEL_SUMMONER_EQUIP_HEAD_SAHAMUTT, pObject, 0);
	DeleteEffect(MODEL_SUMMONER_EQUIP_HEAD_NEIL, pObject, 0);
	DeleteEffect(MODEL_SUMMONER_EQUIP_HEAD_LAGUL, pObject, 0);
}

void CSummonSystem::CreateDamageOfTimeEffect(const mu_uint32 ThreadIndex, mu_int32 iSkill, OBJECT * pObject)
{
	switch (iSkill)
	{
	case AT_SKILL_SUMMON_EXPLOSION:
		if (!SearchEffect(MODEL_SUMMONER_EQUIP_HEAD_SAHAMUTT, pObject, 1))
		{
			CreateEffect(ThreadIndex, MODEL_SUMMONER_EQUIP_HEAD_SAHAMUTT, pObject->Position, pObject->Angle, pObject->Light, 1, pObject, -1);
		}
		break;
	case AT_SKILL_SUMMON_REQUIEM:
		if (!SearchEffect(MODEL_SUMMONER_EQUIP_HEAD_NEIL, pObject, 1))
		{
			CreateEffect(ThreadIndex, MODEL_SUMMONER_EQUIP_HEAD_NEIL, pObject->Position, pObject->Angle, pObject->Light, 1, pObject, -1);
		}
		break;
	}
}

void CSummonSystem::RemoveDamageOfTimeEffect(mu_int32 iSkill, OBJECT * pObject)
{
	switch (iSkill)
	{
	case AT_SKILL_SUMMON_EXPLOSION:
		DeleteEffect(MODEL_SUMMONER_EQUIP_HEAD_SAHAMUTT, pObject, 1);
		break;
	case AT_SKILL_SUMMON_REQUIEM:
		DeleteEffect(MODEL_SUMMONER_EQUIP_HEAD_NEIL, pObject, 1);
		break;
	}
}

void CSummonSystem::RemoveAllDamageOfTimeEffect(OBJECT * pObject)
{
	RemoveDamageOfTimeEffect(AT_SKILL_SUMMON_EXPLOSION, pObject);
	RemoveDamageOfTimeEffect(AT_SKILL_SUMMON_REQUIEM, pObject);
}