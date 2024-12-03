#include "stdafx.h"
#include "GM_Kanturu_2nd.h"

CTrapCanon g_TrapCanon;

using namespace M38Kanturu2nd;

mu_boolean M38Kanturu2nd::Create_Kanturu2nd_Object(mu_uint32 objectIndex, OBJECT* o)
{
	if(!Is_Kanturu2nd())
		return false;

	switch(o->Type) 
	{
	case 3:
		{
			CreateOperate(objectIndex, o);
		}
		break;
	}
	
	return true;
}

CHARACTER* M38Kanturu2nd::Create_Kanturu2nd_Monster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key)
{
	CHARACTER* pCharacter = nullptr;

	switch(iType)
	{
	case 438:
	case 358:
		{
		    pCharacter = CreateCharacter(Key,MODEL_MONSTER01+114,PosX,PosY);
		    pCharacter->Object.Scale = 1.0f;
		    pCharacter->Weapon[0].Type = -1;
		    pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 359:
		{
		    pCharacter = CreateCharacter(Key,MODEL_MONSTER01+115,PosX,PosY);
		    pCharacter->Object.Scale = 1.3f;
			pCharacter->Object.Angle[0] = 0.0f;
			pCharacter->Object.Gravity = 0.0f;
			pCharacter->Object.Distance = (mu_float)GetLargeRand(20)/10.0f;
		    pCharacter->Weapon[0].Type = -1;
		    pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 439:
	case 360:
		{
		    pCharacter = CreateCharacter(Key,MODEL_MONSTER01+116,PosX,PosY);
		    pCharacter->Object.Scale = 1.3f;
			pCharacter->Object.Angle[0] = 0.0f;
			pCharacter->Object.Gravity = 0.0f;
			pCharacter->Object.Distance = (mu_float)GetLargeRand(20)/10.0f;
		    pCharacter->Weapon[0].Type = -1;
		    pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 367:
		{
			pCharacter = CreateCharacter(Key, MODEL_KANTURU2ND_ENTER_NPC, PosX, PosY);
			pCharacter->Object.Scale = 4.76f;
			pCharacter->Object.Position[0] -= 20.0f;
			pCharacter->Object.Position[1] -= 200.0f;

			SetAction(&pCharacter->Object, KANTURU2ND_NPC_ANI_STOP);

#if 0 // FIX
			g_pKanturu2ndEnterNpc->SetNpcObject(&pCharacter->Object);
			g_pKanturu2ndEnterNpc->SetNpcAnimation(false);
#endif
		}
		break;
	case 105:
		{
			pCharacter = g_TrapCanon.Create_TrapCanon(PosX, PosY, Key);
		}
		break;
	}
	
	return pCharacter;
}

mu_boolean M38Kanturu2nd::Set_CurrentAction_Kanturu2nd_Monster(CHARACTER* c, OBJECT* o)
{
	if(Is_Kanturu2nd_3rd() == false)
		return false;

	switch(c->MonsterIndex) 
	{
	case 358:
	case 359:
	case 360:
		{
			return CheckMonsterSkill(c, o);
		}
		break;
	}

	return false;
}

mu_boolean M38Kanturu2nd::AttackEffect_Kanturu2nd_Monster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	switch(o->Type) 
	{
	case MODEL_MONSTER01+114:
		{
			return true;
		}
		break;
	case MODEL_MONSTER01+115:
		{
			return true;
		}
		break;
	case MODEL_MONSTER01+116:
		{
			return true;
		}
		break;
	case MODEL_TRAP_CANON:
		{
			g_TrapCanon.Render_AttackEffect(ThreadIndex, c, o, b);
			return true;
		}
		break;
	}

	return false;
}

void M38Kanturu2nd::Sound_Kanturu2nd_Object(OBJECT* o)
{
	PlayBackground(SOUND_KANTURU_2ND_MAPSOUND_GLOBAL);
	
	switch(o->Type)
	{
	case 9:
		PlayBackground(SOUND_KANTURU_2ND_MAPSOUND_GEAR);
		break;
	case 31:
	case 35:	
	case 36:
	case 37:
		PlayBackground(SOUND_KANTURU_2ND_MAPSOUND_INCUBATOR);
		break;
	}
}

mu_boolean M38Kanturu2nd::Move_Kanturu2nd_Object(OBJECT* o)
{
	Sound_Kanturu2nd_Object(o);
	
	switch(o->Type)
	{
		case 10:
			{
				o->Velocity = 0.04f;
				o->BlendMeshLight = (mu_float)EMath::Sin(FWorldTime*0.0015f) + 1.0f;

				if(o->BlendMeshLight <= 0.1f)
				{
					o->BlendMeshLight = 0.1f;
				}
				else if(o->BlendMeshLight >= 0.9f)
				{
					o->BlendMeshLight = 0.9f;
				}
			}
			break;
		case 38:
			{
				o->BlendMeshLight = (mu_float)EMath::Sin(FWorldTime*0.0010f)+1.0f;
			}
			break;
		case 42:
			{
				o->BlendMeshTexCoordU = -IWorldTime%10000 * 0.0002f;
				o->BlendMeshTexCoordV = -IWorldTime%10000 * 0.0002f;
			}
			break;
		case 44:
			{
				o->Velocity = 0.02f;
			}
			break;
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case 54:
		case 55:
		case 56:
		case 65:
			{
				o->HiddenMesh = -2;
			}
			break;
	}

	return true;
}

mu_boolean M38Kanturu2nd::Move_Kanturu2nd_MonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c,OBJECT* o, MUModel* b)
{
	switch(o->Type)
	{
	case MODEL_MONSTER01+114:
		{
		}
		break;
	case MODEL_MONSTER01+115:
		{
			if(o->CurrentAction == MONSTER01_ATTACK2 &&
				(o->AnimationFrame >= 5.5f && o->AnimationFrame <= 6.5f))
			{
				CHARACTER *tc = &CharactersClient[c->TargetCharacter];
				OBJECT *to = &tc->Object;
				EVector3 vLight;

				if(GET_WORLD == WD_39KANTURU_3RD)
				{
					Vector(0.4f,0.9f,0.6f,vLight);
					for(mu_int32 i=0; i<2; ++i)
						CreateParticle(ThreadIndex, BITMAP_SMOKE, to->Position, to->Angle, vLight, 1);

					Vector(0.4f,1.0f,0.6f,vLight);
					CreateParticle(ThreadIndex, BITMAP_TWINTAIL_WATER, to->Position, to->Angle, vLight, 0);
				}
				else
				{
					Vector(0.4f,0.9f,0.6f,vLight);
					for(mu_int32 i=0; i<5; ++i)
					{
						CreateParticle(ThreadIndex, BITMAP_SMOKE, to->Position, to->Angle, vLight, 1);
					}

					Vector(0.4f,1.0f,0.6f,vLight);
					for(mu_int32 i=0; i<2; ++i)
					{
						CreateParticle(ThreadIndex, BITMAP_TWINTAIL_WATER, to->Position, to->Angle, vLight, 0);
					}
				}							
			}
		}
		break;
	case MODEL_MONSTER01+116:
		{
			
		}
		break;
	}
	
	return false;
}

mu_boolean M38Kanturu2nd::Render_Kanturu2nd_ObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 Position, Light;

	switch(o->Type)
	{
	case 4:
		{
			EVector3 vPos, Light;
			mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 2.0f) * 0.5f;
			Vector ( fLumi*0.3f, fLumi*0.5f, fLumi*1.0f, Light );
			Vector(-1.0f,  0.0f, 0.0f, vPos);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(1)],vPos,Position,false);
			CreateSprite(BITMAP_LIGHT, Position, fLumi/3.2, Light, o);
			CreateSprite(BITMAP_KANTURU_2ND_EFFECT1, Position, fLumi/3.2, Light, o);
			CreateSprite(BITMAP_KANTURU_2ND_EFFECT1, Position, fLumi/3.2, Light, o);
		}
		break;
	case 8:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime*0.001f) + 1.0f) * 0.5f;
			EVector3 vPos;
			Vector(0.0f,  0.0f, 0.0f, vPos);
			Vector ( fLumi, fLumi, fLumi, Light );
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(4)],vPos,Position,false);
			CreateParticle(ThreadIndex, BITMAP_ENERGY,Position,o->Angle,Light, 0, 1.5f);
			CreateSprite(BITMAP_SPARK+1, Position, 10.0f, Light, o);	
			EVector3 StartPos, EndPos;
			VectorCopy(Position, StartPos);
			VectorCopy(Position, EndPos);
			StartPos[0] -= 50.0f;
			StartPos[0] -= GetLargeRand(100);
			EndPos[0] += GetLargeRand(80);
			StartPos[1] -= GetLargeRand(50);
			EndPos[1] += GetLargeRand(50);
			StartPos[2] += 10.0f;
			EndPos[2] += 10.0f;
			if (GetLargeRand(20) == 0)
			{
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, StartPos, EndPos, o->Angle, 18, nullptr, 40.0f);
			}
		}
		break;
	case 10:
		{
			if(GetLargeRand(6) == 0) 
			{
				EVector3 vPos;
				VectorCopy(o->Position, vPos);
				vPos[2] = 410.0f;
				Vector ( 1.0f, 1.0f, 1.0f, Light );
				CreateParticle(ThreadIndex, BITMAP_WATERFALL_3,vPos,o->Angle,Light,7,o->Scale);
			}
		}
		break;
	case 45:
		{
			if (o->HiddenMesh != -2)
			{
				EVector3  Light;
				Vector(0.06f, 0.06f, 0.06f, Light);
				for (mu_int32 i = 0; i < 20; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 1, o->Scale, o);
				}
			}
			o->HiddenMesh = -2;
		}
		break;
	case 46:
		{
			if (o->HiddenMesh != -2)
			{
				EVector3  Light;
				Vector(0.06f, 0.06f, 0.06f, Light);
				for (mu_int32 i = 0; i < 20; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 2, o->Scale, o);
				}
			}
			o->HiddenMesh = -2;
		}
		break;
	case 47:
		{
			if (o->HiddenMesh != -2)
			{
				EVector3  Light;
				Vector(0.2f, 0.2f, 0.2f, Light);
				for (mu_int32 i = 0; i < 20; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 7, o->Scale, o);
				}
			}
			o->HiddenMesh = -2;
		}
		break;
	case 48:
		{
			if (GetLargeRand(3) == 0)
			{
				EVector3  Light;
				Vector(0.2f, 0.2f, 0.2f, Light);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 10, o->Scale, o);
			}
		}
		break;
	case 49:
		{
			if (o->HiddenMesh != -2)
			{
				EVector3  Light;
				Vector(0.0f, 0.01f, 0.03f, Light);
				for (mu_int32 i = 0; i < 5; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 1, o->Scale, o);
				}
			}
			o->HiddenMesh = -2;
		}
		break;
	case 50:
		{
			if (GetLargeRand(3) == 0)
			{
				mu_float fBlue = (GetLargeRand(3))*0.01f + 0.02f;
				EVector3  Light;
				Vector(0.0f, 0.01f, fBlue, Light);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 11, o->Scale, o);
			}
		}
		break;
	case 51:
		{
			if (GetLargeRand(3) == 0)
			{
				mu_float fRed = (GetLargeRand(3))*0.01f + 0.01f;
				EVector3  Light;
				Vector(fRed, 0.00f, 0.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 11, o->Scale, o);
			}
		}
		break;
	case 52:
		{
			if(GetLargeRand(6) == 0) 
			{
				Vector ( 1.0f, 1.0f, 1.0f, Light );
				CreateParticle(ThreadIndex, BITMAP_WATERFALL_3,o->Position,o->Angle,Light,7, o->Scale);
			}
		}
		break;
	case 53:
		{
			if(GetLargeRand(3) == 0)
			{
				Vector(0.4f,0.6f,0.7f,Light);
				CreateParticle(ThreadIndex, BITMAP_TWINTAIL_WATER, o->Position, o->Angle, Light, 1);
			}
		}
		break;
	case 54:
		{
			if(GetLargeRand(20) == 0)
			{
				CreateJoint(ThreadIndex,  BITMAP_JOINT_THUNDER+1, o->Position, o->Position, o->Angle, 8, nullptr, 30.0f+GetLargeRand(10) );
			}
		}
		break;
	case 55:
		{
			if(GetLargeRand(5) == 0)
			{
				Vector(0.8f,0.8f,1.0f,Light);
				CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, o->Position, o->Angle, Light, 1, o);
			}
			
		}
		break;
	case 56:
		{
			if(GetLargeRand(10) == 0)
			{
				CreateJoint(ThreadIndex,  BITMAP_JOINT_THUNDER+1, o->Position, o->Position, o->Angle, 9, nullptr, 30.0f+GetLargeRand(10) );
			}
		}
		break;
	case 65:
		{
			if(GetLargeRand(10) == 0)
			{
				CreateJoint(ThreadIndex,  BITMAP_JOINT_THUNDER+1, o->Position, o->Position, o->Angle, 10, nullptr, 30.0f+GetLargeRand(10) );
			}
		}
		break;
	}

	return true;
}

mu_boolean M38Kanturu2nd::Render_Kanturu2nd_ObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	switch (o->Type)
	{
	case 1:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
			o->HiddenMesh = 1;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, 1, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			return true;
		}
		break;
	case 2:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
			o->HiddenMesh = 1;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, 1, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			return true;
		}
		break;
	case 12:
		{
			o->HiddenMesh = 0;
			b->StreamMesh(ThreadIndex) = 1;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 2000 * 0.0005f, o->HiddenMesh);
			b->StreamMesh(ThreadIndex) = -1;
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, 0, o->BlendMeshLight, IWorldTime % 2000 * 0.0005f, IWorldTime % 2000 * 0.0005f);
			b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_DARK, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);

			return true;
		}
		break;
	case 13:
		{
			b->StreamMesh(ThreadIndex) = 1;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 2000 * 0.0005f, o->HiddenMesh);
			b->StreamMesh(ThreadIndex) = -1;

			return true;
		}
		break;
	case 14:
		{
			b->StreamMesh(ThreadIndex) = 1;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 2000 * 0.0005f, o->HiddenMesh);
			b->StreamMesh(ThreadIndex) = -1;

			return true;
		}
		break;
	case 15:
	case 16:
		{
			b->StreamMesh(ThreadIndex) = 3;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 2000 * 0.0005f, o->HiddenMesh);
			b->StreamMesh(ThreadIndex) = -1;

			return true;
		}
		break;
	case 27:
		{
			o->HiddenMesh = 2;
			b->StreamMesh(ThreadIndex) = 4;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 2000 * 0.0005f, o->HiddenMesh);
			b->StreamMesh(ThreadIndex) = -1;
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			return true;
		}
		break;
	case 8:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
		}
		return true;
	case 10:
		{
			o->HiddenMesh = 2;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 2, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
		}
		return true;
	case 31:
		{
			o->HiddenMesh = 0;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case 35:
	case 36:
		{
			o->HiddenMesh = 1;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case 76:
		{
			b->BodyLight(ThreadIndex) = EVector3(0.4f, 0.4f, 0.4f);

			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_CHROME7 | RENDER_DARK, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);

			if(MU_UPDATECOUNTER > 0)
			{
				EVector3 Light, p, Position;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					Vector(0.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(i)], p, Position, false);
					Vector(0.1f, 0.1f, 0.3f, Light);
					CreateSprite(BITMAP_SPARK + 1, Position, 7.5f, Light, o);
				}
			}
		}
		return true;
	case 33:
	case 59:
	case 80:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		return true;
	}

	return false;
}

mu_boolean M38Kanturu2nd::Render_Kanturu2nd_MonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b,mu_int32 ExtraMon)
{
	switch(o->Type)
	{
	case MODEL_MONSTER01+114:
		{
			mu_float fLumi2 = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE,o->Alpha,o->BlendMesh,o->BlendMeshLight,o->BlendMeshTexCoordU,o->BlendMeshTexCoordV,o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE|RENDER_BRIGHT, o->Alpha, 0, fLumi2, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_PRSONA_EFFECT);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE|RENDER_BRIGHT, o->Alpha, 2, fLumi2, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_PRSONA_EFFECT2);
			return true;	
		}
		break;
	case MODEL_MONSTER01+115:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
			mu_float fLumi2 = (EMath::Sin(FWorldTime*0.002f) + 1.0f);

			if(o->CurrentAction != MONSTER01_DIE)
			{
				Vector(0.9f,0.9f,1.0f,b->BodyLight(ThreadIndex));
			}
			else
			{
				Vector(0.3f,1.0f,0.2f,b->BodyLight(ThreadIndex));
			}

			b->RenderBody(ThreadIndex, RENDER_TEXTURE,o->Alpha,o->BlendMesh,o->BlendMeshLight,o->BlendMeshTexCoordU,o->BlendMeshTexCoordV,o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE|RENDER_BRIGHT, o->Alpha, 2, fLumi2, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_TWINTAIL_EFFECT);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE|RENDER_BRIGHT, o->Alpha, 3, fLumi);
			return true;	
		}
		break;
	case MODEL_MONSTER01+116:
		{
			if(o->CurrentAction == MONSTER01_DIE)
			{
				o->Alpha -= 0.1f;
				if(o->Alpha <= 0.0f)
				{
					o->Alpha = 0.0f;
				}
			}
			b->RenderBody(ThreadIndex, RENDER_TEXTURE,o->Alpha,o->BlendMesh,o->BlendMeshLight,o->BlendMeshTexCoordU,o->BlendMeshTexCoordV,o->HiddenMesh);
			return true;
		}
		break;
	case MODEL_KANTURU2ND_ENTER_NPC:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			
			b->RenderMesh(ThreadIndex, 1, RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV,BITMAP_CHROME);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE|RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_KANTURU_2ND_NPC3);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			
			b->RenderMesh(ThreadIndex, 2, RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV,BITMAP_CHROME);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE|RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_KANTURU_2ND_NPC2);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, 3, o->BlendMeshLight, o->BlendMeshTexCoordU, -FWorldTime*0.0003f,BITMAP_CHROME);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE|RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_KANTURU_2ND_NPC1);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		
			return true;
		}
		break;
	case MODEL_TRAP_CANON:
		{
			g_TrapCanon.Render_Object(ThreadIndex, o, b);
			return true;
		}
		break;
	}

	return false;
}

mu_boolean M38Kanturu2nd::Render_Kanturu2nd_MonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	switch(o->Type)
	{
	case MODEL_MONSTER01+114:
		{
			if (o->CurrentAction == MONSTER01_WALK)
			{
				if (GetLargeRand(15) == 0)
				{
					PlayBackground(SOUND_KANTURU_2ND_PERSO_MOVE1 + GetLargeRand(2));
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_2ND_PERSO_ATTACK1);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_2ND_PERSO_ATTACK2);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				o->SubType = 1;
				PlayBackground(SOUND_KANTURU_2ND_PERSO_DIE);
			}

			if (o->CurrentAction == MONSTER01_STOP1
				|| o->CurrentAction == MONSTER01_STOP2
				|| o->CurrentAction == MONSTER01_WALK)
			{
				o->SubType = 0;
			}

			EVector3 vPos;
			EVector3 vLight = { 1.0f, 1.0f, 1.0f };
			mu_float Luminosity = (mu_float)(GetLargeRand(30) + 70)*0.01f;
			Vector(Luminosity*0.5f, Luminosity*0.6f, Luminosity*1.0f, vLight);

			BoneManager::GetBonePosition(o, 73, vPos);
			CreateSprite(BITMAP_LIGHT, vPos, 2.0f, vLight, o);

			if (GetLargeRand(5) == 0)
			{
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, vLight, 40);
			}

			if (GetLargeRand(3) == 0)
			{
				Vector(0.5f, 0.5f, 0.5f, vLight);
				BoneManager::GetBonePosition(o, 76, vPos);
				CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vPos, o->Angle, vLight, 5, 0.8f, o);
				BoneManager::GetBonePosition(o, 77, vPos);
				CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vPos, o->Angle, vLight, 5, 0.8f, o);
			}

			if (o->CurrentAction == MONSTER01_ATTACK1 && (o->AnimationFrame >= 3.0f && o->AnimationFrame <= 4.0f))
			{
				Vector(0.6f, 0.6f, 1.0f, vLight);
				CreateEffect(ThreadIndex, MODEL_STORM, o->Position, o->Angle, vLight, 0);
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2 && (o->AnimationFrame >= 3.0f && o->AnimationFrame <= 4.0f))
			{
				EVector3 vPos;
				VectorCopy(o->Position, vPos);
				vPos[2] += 100.0f;

				mu_int32 i;
				for (mu_int32 j = 0; j < 2; ++j)
				{
					i = GetLargeRand(4);
					EVector3 vAngle;
					Vector(0.0f, 0.0f, i*90.0f, vAngle);

					CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, vPos, o->Position, vAngle, 0, o, 80.0f);
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, vPos, o->Position, vAngle, 0, o, 20.0f);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				o->BlendMesh = -2;
				Vector(0.5f, 0.5f, 0.5f, vLight);
				BoneManager::GetBonePosition(o, 76, vPos);
				CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vPos, o->Angle, vLight, 6, 0.8f, o);
				BoneManager::GetBonePosition(o, 77, vPos);
				CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vPos, o->Angle, vLight, 6, 0.8f, o);

				for (mu_int32 i = 0; i < 5; ++i)
				{
					mu_int32 j = GetLargeRand(90);
					EVector3 p;
					Vector(0.0f, 0.0f, 0.0f, p);
					Vector(0.5f, 0.5f, 0.5f, vLight);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(j)], p, vPos, true);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vPos, o->Angle, vLight, 6, 0.8f, o);
				}
			}

			return true;
		}
		break;
	case MODEL_MONSTER01+115:
		{
			if(GET_WORLD == WD_39KANTURU_3RD && g_Direction.m_CKanturu.m_iKanturuState == KANTURU_STATE_MAYA_BATTLE)
			{
				EVector3 Light;
				mu_float Scale, Angle;

				o->Distance += 0.05f; 
				Scale = o->Distance;

				if(Scale >= 3.0f)
					o->Distance = 0.1f;

				Angle = (g_CurrentTime%9000)/10.0f;

				Light[0] = 0.6f - (Scale/5.0f);
				Light[1] = 0.6f - (Scale/5.0f);
				Light[2] = 0.0f;

				RenderTerrainAlphaBitmap ( BITMAP_ENERGY_RING, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], Scale, Scale, Light, Angle );

				Vector(0.5f, 0.5f, 0.1f, Light);
				RenderTerrainAlphaBitmap ( BITMAP_ENERGY_FIELD, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 2.0f, 2.0f, Light, Angle );
			}

			if(o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_2ND_TWIN_ATTACK1);
				}
			}
			else if(o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_2ND_TWIN_ATTACK2);
				}
			}
			else if(o->CurrentAction == MONSTER01_DIE)
			{
				o->SubType = 1;
				PlayBackground(SOUND_KANTURU_2ND_TWIN_DIE);
			}

			if (o->CurrentAction == MONSTER01_STOP1 
				|| o->CurrentAction == MONSTER01_STOP2
				|| o->CurrentAction == MONSTER01_WALK)
			{
				o->SubType = 0;
			}

			EVector3 vRelative, vPos;
			EVector3 vLight = {0.6f, 1.0f, 0.4f};

			if(o->CurrentAction == MONSTER01_WALK)
			{
				mu_uint64 dwTime = g_CurrentTime - o->m_dwTime;
				if(dwTime > 500)
				{
					if(o->m_iAnimation % 2 == 1)
					{
						Vector(0.0f, 0.0f, 0.0f, vRelative);
						BoneManager::GetBonePosition(o, 24, vRelative, vPos);
						CreateEffect(ThreadIndex, MODEL_TWINTAIL_EFFECT, vPos, o->Angle, vLight, 0, o);
					}
					else if(o->m_iAnimation % 2 == 0)
					{
						Vector(0.0f, 0.0f, 0.0f, vRelative);
						BoneManager::GetBonePosition(o, 16, vRelative, vPos);
						CreateEffect(ThreadIndex, MODEL_TWINTAIL_EFFECT, vPos, o->Angle, vLight, 0, o);
					}

					if(o->m_iAnimation % 2 == 0)
					{
						o->m_iAnimation = 1;
					}
					else if(o->m_iAnimation  % 2 == 1)
					{
						o->m_iAnimation = 0;
					}
					o->m_dwTime = g_CurrentTime;
					
					PlayBackground(SOUND_KANTURU_2ND_TWIN_MOVE1 + GetLargeRand(2));
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				o->BlendMesh = -2;
				if (o->AnimationFrame <= 3.0f)
				{
					Vector(0.1f, 1.0f, 0.2f, vLight);
					for (mu_int32 i = 0; i < 5; ++i)
					{
						CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, vLight, 39);
					}

					Vector(0.4f, 1.0f, 0.6f, vLight);
					CreateParticle(ThreadIndex, BITMAP_TWINTAIL_WATER, o->Position, o->Angle, vLight, 0);

					CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, o->Position, o->Angle, o->Light, 9, o);

					if (o->AnimationFrame <= 0.2f)
					{
						Vector(0.4f, 1.0f, 0.6f, vLight);
						CreateEffect(ThreadIndex, MODEL_TWINTAIL_EFFECT, o->Position, o->Angle, vLight, 1, o);
						CreateEffect(ThreadIndex, MODEL_TWINTAIL_EFFECT, o->Position, o->Angle, vLight, 2, o);
					}
				}
			}
			return true;	
		}
		break;
	case MODEL_MONSTER01+116:
		{
			if(GET_WORLD == WD_39KANTURU_3RD && g_Direction.m_CKanturu.m_iKanturuState == KANTURU_STATE_MAYA_BATTLE)
			{
				EVector3 Light;
				mu_float Scale, Angle;

				o->Distance += 0.05f; 
				Scale = o->Distance;

				if(Scale >= 3.0f)
					o->Distance = 0.1f;

				Angle = (g_CurrentTime%9000)/10.0f;

				Light[0] = 1.0f - (Scale/3.0f);
				Light[1] = 0.0f;
				Light[2] = 0.0f;

				RenderTerrainAlphaBitmap ( BITMAP_ENERGY_RING, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], Scale, Scale, Light, Angle );

				Vector(1.0f, 0.3f, 0.1f, Light);
				RenderTerrainAlphaBitmap ( BITMAP_ENERGY_FIELD, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 2.0f, 2.0f, Light, Angle );
			}

			if (o->CurrentAction == MONSTER01_WALK)
			{
				if (GetLargeRand(15) == 0)
				{
					PlayBackground(SOUND_KANTURU_2ND_DRED_MOVE1 + GetLargeRand(2));
				}
			}
			else if(o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_2ND_DRED_ATTACK1);
				}
			}
			else if(o->CurrentAction == MONSTER01_ATTACK2 && (o->AnimationFrame >= 0 && o->AnimationFrame <= 2))
			{
				o->SubType = 1;
				PlayBackground(SOUND_KANTURU_2ND_DRED_ATTACK2);
			}
			else if(o->CurrentAction == MONSTER01_DIE)
			{
				o->SubType = 1;
				PlayBackground(SOUND_KANTURU_2ND_DRED_DIE);
			}

			if (o->CurrentAction == MONSTER01_STOP1 
				|| o->CurrentAction == MONSTER01_STOP2
				|| o->CurrentAction == MONSTER01_WALK)
			{
				o->SubType = 0;
			}

			EVector3 vPos, vLight;
			mu_float fScale;
			Vector(1.0f, 1.0f, 1.0f, vLight);

			if(o->CurrentAction != MONSTER01_STOP1 && o->CurrentAction != MONSTER01_DIE && GET_WORLD != WD_39KANTURU_3RD)
			{
				if(GetLargeRand(3) == 0) 
				{
					BoneManager::GetBonePosition(o, 71, vPos);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, vLight, 0);
					BoneManager::GetBonePosition(o, 68, vPos);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, vLight, 0);
				}
				if(GetLargeRand(3) == 0)
				{
					BoneManager::GetBonePosition(o, 50, vPos);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, vLight, 0);
					BoneManager::GetBonePosition(o, 47, vPos);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, vLight, 0);
				}	
			}
			else if(o->CurrentAction == MONSTER01_DIE)
			{
				EVector3 vRelative;
				Vector(0.0f, 0.0f, 0.0f, vRelative);
				BoneManager::GetBonePosition(o, 71, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE+3, vPos, o->Angle, vLight, 3, 1.0f);
				BoneManager::GetBonePosition(o, 68, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE+3, vPos, o->Angle, vLight, 3, 1.0f);
				BoneManager::GetBonePosition(o, 50, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE+3, vPos, o->Angle, vLight, 3, 1.0f);
				BoneManager::GetBonePosition(o, 47, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE+3, vPos, o->Angle, vLight, 3, 1.0f);
			}
						
			Vector(0.25f, 0.7f, 0.6f, vLight);
			fScale = (GetLargeRand(10)-5)*0.01f;
			BoneManager::GetBonePosition(o, 9, vPos);
			CreateSprite(BITMAP_LIGHT+1, vPos, 0.5f+fScale, vLight, o);
			BoneManager::GetBonePosition(o, 10, vPos);
			CreateSprite(BITMAP_LIGHT+1, vPos, 0.5f+fScale, vLight, o);

			return true;
		}
		break;
	case MODEL_KANTURU2ND_ENTER_NPC:
		{
			if(o->CurrentAction == KANTURU2ND_NPC_ANI_ROT)
			{
				PlayBackground(SOUND_KANTURU_2ND_MAPSOUND_HOLE);

				mu_int32 iAnimationFrame = (mu_int32)o->AnimationFrame;

				EVector3 vPos, vLight;
				Vector(0.0f, 0.0f, 0.0f, vPos);
				Vector(1.0f,1.0f,2.0f,vLight);
				if(iAnimationFrame < 40)
				{
					if(GetLargeRand(4) == 0)
					{
						BoneManager::GetBonePosition(o, 37, vPos);
						CreateParticle(ThreadIndex, BITMAP_SPARK+1, vPos, o->Angle, vLight, 18, 1.0f);
						BoneManager::GetBonePosition(o, 38, vPos);
						CreateParticle(ThreadIndex, BITMAP_SPARK+1, vPos, o->Angle, vLight, 18, 1.0f);
						BoneManager::GetBonePosition(o, 39, vPos);
						CreateParticle(ThreadIndex, BITMAP_SPARK+1, vPos, o->Angle, vLight, 18, 1.0f);
						BoneManager::GetBonePosition(o, 40, vPos);
						CreateParticle(ThreadIndex, BITMAP_SPARK+1, vPos, o->Angle, vLight, 18, 1.0f);
						BoneManager::GetBonePosition(o, 41, vPos);
						CreateParticle(ThreadIndex, BITMAP_SPARK+1, vPos, o->Angle, vLight, 18, 1.0f);
						BoneManager::GetBonePosition(o, 42, vPos);
						CreateParticle(ThreadIndex, BITMAP_SPARK+1, vPos, o->Angle, vLight, 18, 1.0f);
						BoneManager::GetBonePosition(o, 43, vPos);
						CreateParticle(ThreadIndex, BITMAP_SPARK+1, vPos, o->Angle, vLight, 18, 1.0f);
					}
				}

				Vector(1.0f, 1.0f, 2.0f, vLight);
				if(GetLargeRand(4) == 0)
				{
					BoneManager::GetBonePosition(o, 6, vPos);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, vLight, 2, 2.0f);
					BoneManager::GetBonePosition(o, 7, vPos);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, vLight, 2, 2.0f);
					BoneManager::GetBonePosition(o, 8, vPos);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, vLight, 2, 2.0f);
					BoneManager::GetBonePosition(o, 15, vPos);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, vLight, 2, 2.0f);
					BoneManager::GetBonePosition(o, 16, vPos);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, vLight, 2, 2.0f);
					BoneManager::GetBonePosition(o, 17, vPos);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, vLight, 2, 2.0f);
				}

				if(iAnimationFrame <= 10)
				{
					Vector(0.0f,0.0f,0.0f,vLight);
				}
				else if(iAnimationFrame == 10)
				{
					Vector(0.1f,0.1f,0.1f,vLight);
				}
				else if(iAnimationFrame >= 11 && iAnimationFrame <= 28)
				{
					vLight[0] = (iAnimationFrame - 11)*0.06f;
					vLight[1] = (iAnimationFrame - 11)*0.06f;
					vLight[2] = (iAnimationFrame - 11)*0.06f;
				}
				else if(iAnimationFrame >= 29)
				{
					Vector(1.0f,1.0f,1.0f,vLight);
				}
				VectorCopy(o->Position, vPos);
				vPos[2] += 310;
				vPos[1] -= 100;	
				vPos[0] += 40;
				CreateSprite(BITMAP_LIGHTNING+1,vPos,1.0,vLight,o,(FWorldTime/10.0f));
				
				if(iAnimationFrame >= 12 && iAnimationFrame <= 25)
				{
					EVector3 vPos, vPos2;
					VectorCopy(o->Position, vPos2);
					vPos2[2] += 290.0f;
					VectorCopy(o->Position, vPos);
					vPos[0] += (GetLargeRand(600) - 300.0f);
					vPos[1] -= (GetLargeRand(200) + 350.0f);
					vPos[2] += (GetLargeRand(600) - 100.0f);
					CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, vPos, vPos2, o->Angle, 42, o, 10.0f);
					CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, vPos, vPos2, o->Angle, 42, o, 10.0f);
				}

				VectorCopy(o->Position, vPos);
				vPos[0] += 260.0f;
				vPos[1] -= 115.0f;
				vPos[2] += 50.0f;
				if(iAnimationFrame >= 1 && iAnimationFrame <= 9)
				{
					vPos[0] += iAnimationFrame*4;
				}
				else if(iAnimationFrame >= 10 && iAnimationFrame <= 18)
				{
					vPos[0] += 36.0f;
					vPos[0] -= (iAnimationFrame-9)*4;
				}

				if(iAnimationFrame >= 1 && iAnimationFrame <= 20)
				{
					EVector3 vAngle;
					Vector((mu_float)(GetLargeRand(60)+290),0.0f,(mu_float)(GetLargeRand(30)+90),vAngle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, vPos, vPos, vAngle, 0);
				}
				
				if(iAnimationFrame >= 42)
				{
#if 0 // FIX
					if(g_pKanturu2ndEnterNpc->IsEnterRequest() == false)
					{
						if(Hero->Dead == 0)
						{
							g_pKanturu2ndEnterNpc->SendRequestKanturu3rdEnter();
						}
					}
#endif
				}

				if(iAnimationFrame >= 50)
				{
					o->AnimationFrame = 0;
					SetAction(o, KANTURU2ND_NPC_ANI_STOP);
#if 0 // FIX
					g_pKanturu2ndEnterNpc->SetNpcAnimation(false);
					g_pKanturu2ndEnterNpc->SetEnterRequest(false);
#endif
				}
			}
		}
		break;
	case MODEL_TRAP_CANON:
		{
			g_TrapCanon.Render_Object_Visual(c, o, b);
		}
		break;
	}
	return false;
}

void M38Kanturu2nd::Move_Kanturu2nd_BlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	switch(o->Type)
	{
	case MODEL_MONSTER01+114:
		{
		}
		break;
	case MODEL_MONSTER01+115:
		{
			if((o->AnimationFrame <= 4.12f && o->CurrentAction == MONSTER01_ATTACK1))
			{
				EVector3  Light;
				Vector(1.0f, 0.2f, 0.5f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;
				
				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed/10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				for(mu_int32 i=0; i<10; ++i) 
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					Vector(-150.0f, 50.0f, 0.0f, StartRelative);
					Vector(150.0f, -200.0f, 0.0f, EndRelative);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(58)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(59)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 1);
					
					fAnimationFrame += fSpeedPerFrame;
				}
			}
			else if(o->CurrentAction == MONSTER01_ATTACK2)
			{
				CHARACTER *tc = &CharactersClient[c->TargetCharacter];
				OBJECT *to = &tc->Object;
				
				EVector3 vPos, vRelative;
				Vector(0.0f, 0.0f, 0.0f, vRelative);
				if(GET_WORLD == WD_39KANTURU_3RD)
				{
					if(GetLargeRand(4) == 0)
					{
						BoneManager::GetBonePosition(o, 16, vRelative, vPos);
						CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, vPos, to->Position, o->Angle, 0, to, 30.0f);
					}
					
					if(GetLargeRand(4) == 0)
					{
						BoneManager::GetBonePosition(o, 24, vRelative, vPos);
						CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, vPos, to->Position, o->Angle, 0, to, 30.0f);
					}
				}
				else
				{
					if(GetLargeRand(2) == 0)
					{
						BoneManager::GetBonePosition(o, 16, vRelative, vPos);
						CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, vPos, to->Position, o->Angle, 0, to, 30.0f);
					}
					
					if(GetLargeRand(2) == 0)
					{
						BoneManager::GetBonePosition(o, 24, vRelative, vPos);
						CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, vPos, to->Position, o->Angle, 0, to, 30.0f);
					}
				}
			}
		}
		break;
	case MODEL_MONSTER01+116:
		{
			if((o->AnimationFrame <= 5.0f && o->CurrentAction == MONSTER01_ATTACK1) 
				|| (o->AnimationFrame <= 9.0f && o->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(1.0f, 1.0f, 1.0f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;
				
				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed/10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				for(mu_int32 i=0; i<10; ++i) 
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					Vector(0.0f, 0.0f, 0.0f, StartRelative);
					Vector(100.0f, 120.0f, 0.0f, EndRelative);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(33)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(33)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 4);
					
					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	}
}

CTrapCanon::CTrapCanon()
{
	Initialize();
}
CTrapCanon::~CTrapCanon()
{
	Destroy();	
}

void CTrapCanon::Initialize()
{

}

void CTrapCanon::Destroy()
{

}

void CTrapCanon::Open_TrapCanon()
{
}

CHARACTER* CTrapCanon::Create_TrapCanon(mu_int32 iPosX, mu_int32 iPosY, mu_int32 iKey)
{
	CHARACTER* pCha = nullptr;

	pCha = CreateCharacter(iKey, MODEL_TRAP_CANON, iPosX, iPosY);
	pCha->Object.Scale = 1.0f;
	pCha->AttackTime = 0;

	return pCha;
}

void CTrapCanon::Render_Object(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
	b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE|RENDER_BRIGHT, o->Alpha, 2, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
}

void CTrapCanon::Render_Object_Visual(CHARACTER* c, OBJECT* o, MUModel* b)
{
	if(c->AttackTime == 0)
	{
		mu_float fLumi;
		EVector3 vPos, vLight;
		VectorCopy(o->Position, vPos);
		fLumi = (EMath::Sin(FWorldTime*0.003f) + 1.0f);
		vPos[1] -= 5.0f;
		vPos[2] += 80.0f + fLumi*5.0f;
		fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.1f;
		Vector(2.0f+fLumi, 1.0f+fLumi, 1.0f+fLumi, vLight);
		CreateSprite(BITMAP_POUNDING_BALL,vPos,0.7f+fLumi,vLight,o,(FWorldTime/10.0f));
		fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 1.0f;
		Vector(2.0f+(GetLargeRand(10))*0.03f, 0.4f+(GetLargeRand(10))*0.03f, 0.4f+(GetLargeRand(10))*0.03f, vLight);
		CreateSprite(BITMAP_LIGHT,vPos,2.0f,vLight,o,-(FWorldTime*0.1f));
		CreateSprite(BITMAP_LIGHT,vPos,2.0f,vLight,o,(FWorldTime*0.12f));
	}
}

void CTrapCanon::Render_AttackEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if(c->AttackTime == 1)
	{
		CHARACTER *tc = &CharactersClient[c->TargetCharacter];
		OBJECT *to = &tc->Object;
		EVector3 vPos, vPos2;
		VectorCopy(o->Position, vPos);
		VectorCopy(to->Position, vPos2);
		vPos[2] += 85.0f;
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, vPos, vPos2, to->Angle, 43, to, 30.0f);
	}
}

void M38Kanturu2nd::PlayBGM()
{
	if (GET_WORLD == WD_38KANTURU_2ND)
	{
		EMusicBackend::Play(MUSICS::Data[MUSIC_KANTURU_2ND]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_KANTURU_2ND]);
	}
}