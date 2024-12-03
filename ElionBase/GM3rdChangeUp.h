// GM3rdChangeUp.h: interface for the CGM3rdChangeUp class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __GM3RDCHANGEUP_H__
#define __GM3RDCHANGEUP_H__

#pragma once

EINLINE mu_boolean IsBalgasBarrackMap()
{
	return WD_41CHANGEUP3RD_1ST == GET_WORLD;
}

EINLINE mu_boolean IsBalgasRefugeMap()
{
	return WD_42CHANGEUP3RD_2ND == GET_WORLD;
}

namespace SEASON3A
{
	class CGM3rdChangeUp  
	{
	protected:
		mu_boolean m_nDarkElfAppearance;

	public:
		CGM3rdChangeUp();
		~CGM3rdChangeUp();

		mu_boolean MoveObject(OBJECT* pObject);
		mu_boolean RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel);
		mu_boolean RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);

		mu_boolean CreateFireSnuff( PARTICLE* o );
		void PlayEffectSound(OBJECT* o);
		void PlayBGM();

		CHARACTER* CreateBalgasBarrackMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key);
		mu_boolean SetCurrentActionBalgasBarrackMonster(CHARACTER* c, OBJECT* o);
		mu_boolean AttackEffectBalgasBarrackMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
		mu_boolean MoveBalgasBarrackMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c,OBJECT* o, MUModel* b);
		void MoveBalgasBarrackBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
		mu_boolean RenderMonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b,mu_int32 ExtraMon);
		mu_boolean RenderBalgasBarrackMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	};
}

extern SEASON3A::CGM3rdChangeUp g_3rdChangeUp;

#endif