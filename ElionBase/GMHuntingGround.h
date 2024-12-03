#ifndef _GMHUNTINGGROUND_H_
#define _GMHUNTINGGROUND_H_

class PARTICLE_HEADER;

namespace M31HuntingGround
{
	EINLINE mu_boolean IsInHuntingGround()
	{
		return (GET_WORLD == WD_31HUNTING_GROUND) ? true : false;
	}

	EINLINE mu_boolean IsInHuntingGroundSection2(const EVector3 &Position)
	{
		if (Position[0] > 5449.0f && Position[0] < 17822.0f && Position[1] > 6784.0f && Position[1] < 22419.0f)
			return true;

		return false;
	}

	mu_boolean CreateHuntingGroundObject(mu_uint32 objectIndex, OBJECT* pObject);
	mu_boolean MoveHuntingGroundObject(const mu_uint32 ThreadIndex, OBJECT* pObject);
	mu_boolean RenderHuntingGroundObjectVisual(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel);
	mu_boolean RenderHuntingGroundObjectMesh(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel,mu_boolean ExtraMon = 0);
	
	CHARACTER* CreateHuntingGroundMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key);

	mu_boolean MoveHuntingGroundMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel);
	void MoveHuntingGroundBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel);
	mu_boolean RenderHuntingGroundMonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel,mu_boolean ExtraMon);
	mu_boolean RenderHuntingGroundMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel);
	
	mu_boolean AttackEffectHuntingGroundMonster(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel);
	mu_boolean SetCurrentActionHuntingGroundMonster(CHARACTER* pCharacter, OBJECT* pObject);
	
	mu_boolean CreateMist(const mu_uint32 ThreadIndex, PARTICLE_HEADER *pParticleHeader, PARTICLE* pParticleObj);
}

#endif