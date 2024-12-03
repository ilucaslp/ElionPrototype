#include "stdafx.h"
#include "GM_Kanturu_3rd.h"

mu_boolean KanturuSuccessMap = false;
mu_boolean KanturuSuccessMapBackup = false;
mu_int32 iMayaAction = -1;
mu_boolean bMayaSkill2 = false;
mu_int32 iMayaSkill2_Counter = 0;
mu_int32 iMayaDie_Counter = 0;
mu_int32 iKanturuResult = -1;
mu_float fAlpha = 0.1f;
mu_int32 UserCount = 0;
mu_int32 MonsterCount = 0;

void M39Kanturu3rd::Kanturu3rdInit()
{
	KanturuSuccessMap		= false;
	KanturuSuccessMapBackup = false;
	iMayaAction				= -1;
	bMayaSkill2				= false;
	iMayaSkill2_Counter		= 0;
	iMayaDie_Counter		= 0;
	iKanturuResult			= -1;
	fAlpha					= 0.1f;
	UserCount				= 0;
	MonsterCount			= 0;
}

mu_boolean M39Kanturu3rd::IsSuccessBattle()
{
	if(KanturuSuccessMap)
		return true;
	return false;
}

void M39Kanturu3rd::CheckSuccessBattle(mu_uint8 State, mu_uint8 DetailState)
{
	if(State == KANTURU_STATE_TOWER && (DetailState == KANTURU_TOWER_REVITALIXATION || DetailState == KANTURU_TOWER_NOTIFY))
		KanturuSuccessMap = true;
	else
		KanturuSuccessMap = false;

	if(KanturuSuccessMap == KanturuSuccessMapBackup )
		return;

	KanturuSuccessMapBackup = KanturuSuccessMap;

	if(GET_WORLD != WD_39KANTURU_3RD)
		return;

	g_Terrain.Load(GET_WORLD, KanturuSuccessMap == true ? 1 : 0, false);
}

mu_boolean M39Kanturu3rd::CreateKanturu3rdObject(mu_uint32 objectIndex, OBJECT* o)
{
	switch(o->Type)
	{
		case 0:
			o->Position[2] -= 2000.0f;
			break;
		case 32:
		case 47:
		case 51:
		case 52:
		case 53:
		case 54:
		case 57:
		case 58:
		case 70:
			o->HiddenMesh = -2;
			break;		
	}

	return true;
}

mu_boolean M39Kanturu3rd::MoveKanturu3rdObject(OBJECT* o)
{
	if(!IsInKanturu3rd())
		return false;

	mu_float Luminosity;
	EVector3 Light;
	
	switch(o->Type)
	{
	case 0:
		{
		}
		break;
	case 25:
		{
			PlayBackground (SOUND_KANTURU_3RD_MAP_SOUND05);
		}
		break;
	case 40:
	case 41:
	case 42:
		{
			PlayBackground (SOUND_KANTURU_3RD_MAP_SOUND01);
		}
		break;
	case 45:
		{
			if(GetLargeRand(3) == 0)
			{
				o->HiddenMesh = -2;
				Luminosity = (mu_float)(GetLargeRand(4)+3)*0.3f;
				Vector(Luminosity,Luminosity,Luminosity,Light);
				AddTerrainLight(o->Position[0], o->Position[1],Light,1+o->Scale/2.0f,PrimaryTerrainLight);
			}
		}
		break;
	case 48:
		{
			o->HiddenMesh = -2;
		}
		break;
	case 50:
		{
			if(GetLargeRand(3) == 0)
			{
				Luminosity = (mu_float)(GetLargeRand(10))*0.2f;
				Vector(Luminosity,Luminosity,Luminosity,Light);
				AddTerrainLight(o->Position[0], o->Position[1],Light,1+o->Scale,PrimaryTerrainLight);
			}
		}
		break;
	case 54:
		{
			if(o->HiddenMesh == -1)
			{
				o->HiddenMesh = -2;
				Luminosity = (mu_float)(GetLargeRand(4)+3)*0.3f;
				Vector(Luminosity,Luminosity,Luminosity,Light);
				AddTerrainLight(o->Position[0], o->Position[1],Light,1+o->Scale/2.0f,PrimaryTerrainLight);
			}
		}
		break;
	case 71:
		{
			PlayBackground (SOUND_KANTURU_3RD_MAP_SOUND04);
		}
		break;
	case 73:
		{
			PlayBackground (SOUND_KANTURU_3RD_MAP_SOUND03);
		}
		break;
	}

	PlayBackground ( SOUND_KANTURU_3RD_AMBIENT );
	
	return true;
}

mu_boolean M39Kanturu3rd::RenderKanturu3rdObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 Position, Light;

	switch (o->Type)
	{
	case 0:
		{
			EVector3 p, Pos, Light;

			if (!g_Direction.m_CKanturu.GetMayaExplotion())
				o->HiddenMesh = 0;

			if (g_Direction.m_CKanturu.m_iMayaState >= KANTURU_MAYA_DIRECTION_MONSTER1 && g_Direction.m_CKanturu.m_iMayaState <= KANTURU_MAYA_DIRECTION_ENDCYCLE)
				o->Position[2] = -749.5f;

			if (g_Direction.m_CKanturu.GetMayaAppear())
			{
				if (o->Position[2] >= -749.5f)
				{
					g_Direction.m_CKanturu.SetMayaAppear(false);
					o->Position[2] = -749.5f;
				}

				EarthQuake.Store((mu_float)(GetLargeRand(6) - 3)*2.0f);
				o->Position[2] += 13.0f;

				PlayBackground(SOUND_KANTURU_3RD_MAYA_INTRO);
			}
			else
			{
				Vector(0.08f, 0.08f, 0.08f, Light);
				Vector(GetLargeRand(20) - 30.0f, GetLargeRand(20) - 30.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(34)], p, Pos, false);
				if (o->AnimationFrame >= 5.0f && o->AnimationFrame < 12.5f)
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Pos, o->Angle, Light, 43, 1.5f);
			}

			if (g_Direction.m_CKanturu.m_iMayaState == KANTURU_MAYA_DIRECTION_ENDCYCLE || g_Direction.m_CKanturu.m_iKanturuState == KANTURU_STATE_STANDBY)
			{
				o->Position[2] = -2749.5f;
			}

			MayaAction(ThreadIndex, o, b);
		}
		break;
	case 1:
		{
			if (o->HiddenMesh != -2)
			{
				EVector3  Light;
				Vector(0.0f, 0.03f, 0.04f, Light);
				for (mu_int32 i = 0; i < 10; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 1, o->Scale, o);
				}
			}
			o->HiddenMesh = -2;
		}
		break;
	case 5:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 2.0f) * 0.5f;
			Vector(fLumi*0.3f, fLumi*0.5f, fLumi*1.0f, Light);
			EVector3 vPos;
			Vector(-1.0f, 0.0f, 0.0f, vPos);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(1)], vPos, Position, false);
			CreateSprite(BITMAP_LIGHT, Position, fLumi / 2.4, Light, o);
			CreateSprite(BITMAP_SHINY + 1, Position, fLumi / 2.4, Light, o, (mu_float)((mu_int32)(FWorldTime / 2) % 360));
			CreateSprite(BITMAP_SHINY + 1, Position, fLumi / 2.4, Light, o, (mu_float)((mu_int32)(FWorldTime / 4) % 360));
		}
		break;
	case 11:
		{
			if (o->HiddenMesh != -2)
			{
				EVector3  Light;
				Vector(0.1f, 0.1f, 0.1f, Light);
				for (mu_int32 i = 0; i < 10; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 3, o->Scale, o);
				}
			}
			o->HiddenMesh = -2;
		}
		break;
	case 32:
		{
			if (GetLargeRand(3) == 0)
			{
				Vector(0.4f, 0.6f, 0.7f, Light);
				CreateParticle(ThreadIndex, BITMAP_TWINTAIL_WATER, o->Position, o->Angle, Light, 1);
			}
		}
		break;
	case 46:
		{
			if (o->HiddenMesh != -2)
			{
				EVector3  Light;
				Vector(0.25f, 0.25f, 0.25f, Light);
				for (mu_int32 i = 0; i < 20; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 7, o->Scale, o);
				}
			}
			o->HiddenMesh = -2;
		}
		break;
	case 74:
		{
			if (KanturuSuccessMap)
				o->HiddenMesh = -2;
			else
			{
				if (GetLargeRand(5) == 0)
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, o->Position, o->Position, o->Angle, 11, nullptr, o->Scale*15.0f);
			}
		}
		break;
	case 47:
		{
			if (GetLargeRand(5) == 0)
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, o->Position, o->Position, o->Angle, 11, nullptr, o->Scale*15.0f);
		}
		break;
	case 48:
		{
			if (GetLargeRand(3) == 0)
			{
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, o->Light, 10, o->Scale, o);
			}
		}
		break;
	case 49:
		{
			if (o->HiddenMesh != -2)
			{
				EVector3  Light;
				Vector(0.0f, 0.06f, 0.10f, Light);
				for (mu_int32 i = 0; i < 10; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 1, o->Scale, o);
				}
			}
			o->HiddenMesh = -2;
		}
		break;
	case 50:
		{
			if (o->HiddenMesh != -2)
			{
				EVector3  Light;
				Vector(0.1f, 0.1f, 0.1f, Light);
				for (mu_int32 i = 0; i < 10; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 4, o->Scale, o);
				}
			}
			o->HiddenMesh = -2;
		}
		break;
	case 52:
		{
			if (GetLargeRand(3) == 0)
			{
				Vector(0.5f, 1.0f, 0.7f, o->Light);
				CreateParticle(ThreadIndex, BITMAP_TRUE_BLUE, o->Position, o->Angle, o->Light, 1, o->Scale);
			}
		}
		break;
	case 53:
		{
			if (GetLargeRand(3) == 0)
			{
				Vector(0.3f, 0.3f, 0.3f, o->Light);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 46, o->Scale);
			}
		}
		break;
	case 54:
		{
			if (GetLargeRand(3) == 0)
			{
				o->HiddenMesh = -1;
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, o->Position, o->Position, o->Angle, 19, nullptr, o->Scale*10.0f);
			}
		}
		break;
	}
	return true;
}

mu_boolean M39Kanturu3rd::RenderKanturu3rdObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	switch (o->Type)
	{
	case 0:
		{
			if (g_Direction.m_CKanturu.GetMayaExplotion())
			{
				o->Light[0] /= 1.02f;
				o->Light[1] /= 1.02f;
				o->Light[2] /= 1.02f;


				VectorCopy(o->Light, b->BodyLight(ThreadIndex));

				if (o->Light[0] <= 0.05f)
				{
					o->Light[0] = 0.05f;
					o->Light[1] = 0.05f;
					o->Light[2] = 0.05f;
					o->HiddenMesh = -2;
				}
			}
			else
				Vector(1.0f, 1.0f, 1.0f, o->Light);

			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 5, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 6, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 7, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 8, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			if (g_Direction.m_CKanturu.GetMayaExplotion())
			{
				VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			}
			else
			{
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			}
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);

			if (g_Direction.m_CKanturu.GetMayaExplotion())
			{
				o->StartPosition[0] /= 1.02f;
				o->StartPosition[1] /= 1.02f;
				o->StartPosition[2] /= 1.02f;

				VectorCopy(o->StartPosition, b->BodyLight(ThreadIndex));
			}
			else
			{
				b->BodyLight(ThreadIndex)[0] = EMath::Sin(FWorldTime*0.001f)*0.12f + 0.05f;
				b->BodyLight(ThreadIndex)[1] = EMath::Sin(FWorldTime*0.001f)*0.12f + 0.3f;
				b->BodyLight(ThreadIndex)[2] = EMath::Sin(FWorldTime*0.001f)*0.12f + 0.21f;

				VectorCopy(b->BodyLight(ThreadIndex), o->StartPosition);
			}

			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_MAYA_BODY);
			if (g_Direction.m_CKanturu.GetMayaExplotion())
			{
				VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			}
			else
			{
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			}
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case 2:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME2);
			Vector(0.3f, 0.3f, 0.3f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case 15:
	case 16:
	case 17:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 10000 * 0.0005f);
		}
		return true;
	case 33:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 2000 * 0.0005f);
		}
		return true;
	case 40:
	case 41:
	case 42:
		{
			Vector(0.6f, 0.7f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 1, RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(0.7f, 0.6, 0.7f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 200000 * 0.00001f);
		}
		return true;
	case 43:
		{
			EVector3 Light;
			VectorCopy(b->BodyLight(ThreadIndex), Light);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, -0.1f + IWorldTime % 5000 * 0.0001f, BITMAP_KANTURU3RD_OBJECT);
			VectorCopy(Light, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			mu_float lumi = EMath::Sin(FWorldTime*0.0015f)*0.2f + 0.4f;
			b->BodyLight(ThreadIndex) = EVector3(lumi, lumi, lumi);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case MODEL_SMELTING_NPC:
		{
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_PREBLEND, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_PREBLEND, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(10.0f, 10.0f, 10.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 3, o->BlendMeshLight, o->BlendMeshTexCoordU, IWorldTime % 2000 * 0.001f);
		}
		return true;
	case MODEL_MAYASTAR:
		{
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		}
		return true;
	case MODEL_STORM3:
		{
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		}
		return true;
	case MODEL_MAYAHANDSKILL:
		{
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 2000 * 0.001f);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, o->BlendMeshLight, -IWorldTime % 2000 * 0.001f, o->BlendMeshTexCoordV);
		}
		return true;
	case MODEL_MAYASTONE1:
		{
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
	case MODEL_MAYASTONE2:
	case MODEL_MAYASTONE3:
	case MODEL_MAYASTONE4:
	case MODEL_MAYASTONE5:
	case MODEL_MAYASTONEFIRE:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case 8:
		{
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 5, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case 10:
		{
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 5, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 6, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 7, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case 19:
		{
			o->HiddenMesh = 7;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			o->HiddenMesh = -1;

			b->RenderMesh(ThreadIndex, 7, RENDER_TEXTURE, o->Alpha, 7, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 10000 * 0.0001f);
		}
		return true;
	case 20:
		{
			o->HiddenMesh = 6;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			o->HiddenMesh = -1;

			b->RenderMesh(ThreadIndex, 6, RENDER_TEXTURE, o->Alpha, 6, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 10000 * 0.0001f);
		}
		return true;
	case 21:
		{
			mu_float lumi = EMath::Sin(FWorldTime*0.002f)*0.2f + 0.5f;
			b->BodyLight(ThreadIndex) = EVector3(lumi, lumi, lumi);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case 24:
	case 25:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case 73:
		{
			if (!KanturuSuccessMap)
			{
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, IWorldTime % 10000 * 0.0002f);
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->BlendMeshLight, IWorldTime % 10000 * 0.0002f, o->BlendMeshTexCoordV);
			}
		}
		return true;
	}

	return false;
}

CHARACTER* M39Kanturu3rd::CreateKanturu3rdMonster(const mu_uint32 ThreadIndex, mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key)
{
	if (!IsInKanturu3rd())
		return nullptr;

	CHARACTER* c = nullptr;

	switch (iType)
	{
	case 361:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 121, PosX, PosY);
			c->Object.Scale = 1.6f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;

			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, c->Object.Position, c->Object.Position, c->Object.Angle, 20, &c->Object, 10.0f);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, c->Object.Position, c->Object.Position, c->Object.Angle, 21, &c->Object, 10.0f);

			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, c->Object.Position, c->Object.Position, c->Object.Angle, 18, &c->Object, 10.0f);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, c->Object.Position, c->Object.Position, c->Object.Angle, 19, &c->Object, 10.0f);
		}
		break;
	case 362:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 118, PosX, PosY);
			c->Object.Scale = 2.28f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;

			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, c->Object.Position, c->Object.Position, c->Object.Angle, 18, &c->Object, 15.0f);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, c->Object.Position, c->Object.Position, c->Object.Angle, 19, &c->Object, 15.0f);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, c->Object.Position, c->Object.Position, c->Object.Angle, 20, &c->Object, 15.0f);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, c->Object.Position, c->Object.Position, c->Object.Angle, 21, &c->Object, 15.0f);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, c->Object.Position, c->Object.Position, c->Object.Angle, 27, &c->Object, 15.0f);
		}
		break;
	case 363:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 119, PosX, PosY);
			c->Object.Scale = 2.28f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;

			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, c->Object.Position, c->Object.Position, c->Object.Angle, 28, &c->Object, 15.0f);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, c->Object.Position, c->Object.Position, c->Object.Angle, 29, &c->Object, 15.0f);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, c->Object.Position, c->Object.Position, c->Object.Angle, 30, &c->Object, 15.0f);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, c->Object.Position, c->Object.Position, c->Object.Angle, 31, &c->Object, 15.0f);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, c->Object.Position, c->Object.Position, c->Object.Angle, 33, &c->Object, 15.0f);
		}
		break;
	case 364:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 120, PosX, PosY);
			c->Object.Scale = 0.2f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			c->Object.HiddenMesh = -2;
		}
		break;
	}

	return c;
}

mu_boolean M39Kanturu3rd::MoveKanturu3rdMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	if (!IsInKanturu3rd())
		return false;

	EVector3 Position, Direction, Angle, Light;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 121:
		{
			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_KANTURU_3RD_NIGHTMARE_IDLE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->SubType == 0) {
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_3RD_NIGHTMARE_ATT1);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0) {
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_3RD_NIGHTMARE_ATT2);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK3)
			{
				if (o->SubType == 0) {
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_3RD_NIGHTMARE_ATT3);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK4)
			{
				if (o->SubType == 0) {
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_3RD_NIGHTMARE_ATT4);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0) {
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_3RD_NIGHTMARE_DIE);
				}
			}

			if (o->CurrentAction == MONSTER01_ATTACK3)
			{
				Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);

				for (mu_int32 i = 0; i < 2; ++i)
				{
					Position[0] = o->StartPosition[0] + GetLargeRand(440) - 220;
					Position[1] = o->StartPosition[1] + GetLargeRand(440) - 220;
					Position[2] = o->StartPosition[2];
					CreateJoint(ThreadIndex, BITMAP_FLARE, Position, Position, Angle, 2, nullptr, 40);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2 && (o->AnimationFrame >= 5.2f && o->AnimationFrame <= 5.8f))
			{
				EBone Matrix;
				Vector(0.0f, 0.0f, 0.0f, Angle);
				Vector(0.0f, -GetLargeRand(20) - 40.0f, 0.0f, Direction);
				AngleMatrix(Angle, Matrix);
				VectorRotate(Direction, Matrix, Position);

				Vector(0.5f, 0.7f, 1.0f, Light);
				BoneManager::GetBonePosition(o, 14, Position);
				CreateEffect(ThreadIndex, MODEL_STORM2, Position, o->Angle, Light, 0);
				CreateEffect(ThreadIndex, BITMAP_BOSS_LASER, Position, o->Angle, Light, 2);
			}
			else if (o->CurrentAction == MONSTER01_DIE && o->AnimationFrame >= 3.0f)
			{
				EVector3 Position;
				BoneManager::GetBonePosition(o, 63, Position);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, Position, o->Angle, Light, 3, 1.5f);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, Position, o->Angle, Light, 3, 1.5f);

				EVector3 Angle;
				Vector((mu_float)(GetLargeRand(100)) - 50.0f, (mu_float)(GetLargeRand(100)) - 50.0f, (mu_float)(GetLargeRand(360)), Angle);
				mu_float Scale = (mu_float)(GetLargeRand(40)) + 30.0f;
				CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT2, Position, Position, Angle, 8, nullptr, Scale, 0, 0);
			}
			else if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;

		}
		break;
	case MODEL_MONSTER01 + 118:
		{
			EVector3 Pos;

			if (o->CurrentAction == MONSTER01_DIE && g_Direction.m_CKanturu.m_iMayaState < KANTURU_MAYA_DIRECTION_MAYA3)
			{
				o->BlendMesh = -2;
				Vector(0.0f, 0.0f, 0.0f, Position);
				Vector(0.4f, 0.6f, 1.0f, Light);

				for (mu_int32 i = 0; i < 5; ++i)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(GetLargeRand(45))], Position, Pos, true);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, Pos, o->Angle, Light, 6, 2.0f, o);
				}
			}
			else
				o->BlendMesh = -1;

			if (o->CurrentAction == MONSTER01_STOP2 && g_Direction.m_CKanturu.m_iMayaState >= KANTURU_MAYA_DIRECTION_MAYA3)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				Vector(0.0f, -50.0f, 0.0f, Position);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(24)], Position, Pos, true);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, Pos, o->Angle, Light, 3, 1.5f);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, Pos, o->Angle, Light, 4, 0.8f);
			}

		}
		break;
	case MODEL_MONSTER01 + 119:
		{
			EVector3 Pos;

			if (o->CurrentAction == MONSTER01_DIE && g_Direction.m_CKanturu.m_iMayaState < KANTURU_MAYA_DIRECTION_MAYA3)
			{
				o->BlendMesh = -2;
				Vector(0.0f, 0.0f, 0.0f, Position);
				Vector(0.4f, 0.6f, 1.0f, Light);

				for (mu_int32 i = 0; i < 5; ++i)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(GetLargeRand(45))], Position, Pos, true);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, Pos, o->Angle, Light, 6, 2.0f, o);
				}
			}
			else
				o->BlendMesh = -1;

			if (o->CurrentAction == MONSTER01_STOP2 && g_Direction.m_CKanturu.m_iMayaState >= KANTURU_MAYA_DIRECTION_MAYA3)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				Vector(50.0f, 0.0f, 0.0f, Position);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(12)], Position, Pos, true);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, Pos, o->Angle, Light, 3, 1.5f);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, Pos, o->Angle, Light, 4, 0.8f);
			}
		}
		break;
	case MODEL_MONSTER01 + 120:
		{

		}
		break;
	case MODEL_SMELTING_NPC:
		{
			o->Angle[2] = 0.0f;
			o->Position[2] = -60.0f + EMath::Sin(FWorldTime*0.002f)*5.8f;
		}
		break;
	}

	return false;
}

void M39Kanturu3rd::MoveKanturu3rdBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (!IsInKanturu3rd())
		return;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 121:
		{
			if ((o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK4) && o->AnimationFrame <= 5.9f)
			{
				EVector3  Light;
				Vector(1.0f, 1.0f, 1.0f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					fAnimationFrame += fSpeedPerFrame;

					Vector(10.0f, 0.0f, 0.0f, StartRelative);
					Vector(10.0f, -10.0f, 0.0f, EndRelative);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(32)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(38)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 3, true, 80);
				}
			}

		}
		break;
	case MODEL_MONSTER01 + 118:
		{
		}
		break;
	case MODEL_MONSTER01 + 119:
		{
		}
		break;
	case MODEL_MONSTER01 + 120:
		{
		}
		break;
	}
}

mu_boolean M39Kanturu3rd::RenderKanturu3rdMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (!IsInKanturu3rd())
		return false;

	EVector3 Position, Light;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 121:
		{
			if (o->CurrentAction == MONSTER01_DIE)
				return true;

			MoveEye(ThreadIndex, o, b, 9, 10, 39, 40);

			Vector(1.0f, 1.0f, 1.0f, Light);
			Vector(0.0f, 0.0f, 0.0f, Position);

			BoneManager::GetBonePosition(o, 61, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.3f, Light, o);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.5f, Light, o);
			BoneManager::GetBonePosition(o, 62, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.3f, Light, o);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.5f, Light, o);
			BoneManager::GetBonePosition(o, 54, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.3f, Light, o);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.5f, Light, o);
			BoneManager::GetBonePosition(o, 55, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.3f, Light, o);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.5f, Light, o);
			Vector(5.0f, 0.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 21, Position, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.3f, Light, o);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.5f, Light, o);
			Vector(9.0f, 0.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 22, Position, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.3f, Light, o);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.5f, Light, o);
			Vector(6.0f, 0.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 25, Position, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.7f, Light, o);
			BoneManager::GetBonePosition(o, 26, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.7f, Light, o);
			BoneManager::GetBonePosition(o, 71, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.7f, Light, o);
			BoneManager::GetBonePosition(o, 70, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.7f, Light, o);
			Vector(10.0f, 0.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 43, Position, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.7f, Light, o);
			BoneManager::GetBonePosition(o, 44, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.7f, Light, o);

			BoneManager::GetBonePosition(o, 63, Position);
			CreateParticle(ThreadIndex, BITMAP_FIRE + 1, Position, o->Angle, Light, 3, 1.7f);

			Vector(3.0f, 0.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 39, Position, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.3f, Light, o);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.6f, Light, o);
			Vector(3.0f, 0.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 40, Position, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.3f, Light, o);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.6f, Light, o);

			Vector(0.0f, -2.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 9, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.3f, Light, o);
			Vector(4.0f, -4.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 10, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.3f, Light, o);

			BoneManager::GetBonePosition(o, 47, Position);
			if (GetLargeRand(2) == 0)
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, o->Angle, Light, 16, 1.0f);
		}
		return true;
	case MODEL_MONSTER01 + 118:
		{
			if (o->CurrentAction == MONSTER01_DIE)
				return true;

			MoveEye(ThreadIndex, o, b, 5, 11, 17, 29, 23);

			Vector(0.5f, 0.5f, 0.8f, Light);

			Vector(0.0f, 0.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 7, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 1.3f, Light, o);
			Vector(-10.0f, 0.0f, 3.0f, Position);
			BoneManager::GetBonePosition(o, 7, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.7f, Light, o);

			Vector(0.0f, 0.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 13, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 1.3f, Light, o);
			Vector(-12.0f, 5.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 13, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.7f, Light, o);

			Vector(0.0f, 0.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 19, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 1.3f, Light, o);
			Vector(-12.0f, 5.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 19, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.7f, Light, o);

			Vector(0.0f, 0.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 25, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 1.3f, Light, o);
			Vector(-10.0f, 5.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 25, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.7f, Light, o);

			Vector(0.0f, 0.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 31, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 1.3f, Light, o);
			Vector(-8.0f, 9.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 31, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 0.7f, Light, o);

			Vector(0.0f, 0.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 12, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 1.0f, Light, o);
			Vector(0.0f, 0.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 18, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 1.0f, Light, o);
			Vector(0.0f, 0.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 24, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 1.0f, Light, o);
			Vector(0.0f, 0.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 30, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 1.0f, Light, o);
			Vector(0.0f, 0.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 6, Position, Position);
			CreateSprite(BITMAP_FLARE_BLUE, Position, 1.0f, Light, o);
		}
		return true;
	case MODEL_MONSTER01 + 119:
		{
			if (o->CurrentAction == MONSTER01_DIE)
				return true;

			MoveEye(ThreadIndex, o, b, 5, 20, 31, 42, 53);

			Vector(0.5f, 0.3f, 0.3f, Light);

			Vector(-2.0f, 0.0f, 4.0f, Position);
			BoneManager::GetBonePosition(o, 59, Position, Position);
			CreateSprite(BITMAP_FLARE_RED, Position, 1.5f, Light, o);
			Vector(-10.0f, -3.0f, 3.0f, Position);
			BoneManager::GetBonePosition(o, 59, Position, Position);
			CreateSprite(BITMAP_FLARE_RED, Position, 0.7f, Light, o);

			Vector(-1.0f, -2.0f, 4.0f, Position);
			BoneManager::GetBonePosition(o, 48, Position, Position);
			CreateSprite(BITMAP_FLARE_RED, Position, 1.5f, Light, o);
			Vector(-10.0f, -3.0f, 5.0f, Position);
			BoneManager::GetBonePosition(o, 48, Position, Position);
			CreateSprite(BITMAP_FLARE_RED, Position, 0.7f, Light, o);

			Vector(0.0f, 0.0f, 4.0f, Position);
			BoneManager::GetBonePosition(o, 11, Position, Position);
			CreateSprite(BITMAP_FLARE_RED, Position, 1.3f, Light, o);
			Vector(-10.0f, 0.0f, 4.0f, Position);
			BoneManager::GetBonePosition(o, 11, Position, Position);
			CreateSprite(BITMAP_FLARE_RED, Position, 0.7f, Light, o);

			Vector(0.0f, 0.0f, 4.0f, Position);
			BoneManager::GetBonePosition(o, 37, Position, Position);
			CreateSprite(BITMAP_FLARE_RED, Position, 1.3f, Light, o);
			Vector(-10.0f, 1.0f, 4.0f, Position);
			BoneManager::GetBonePosition(o, 37, Position, Position);
			CreateSprite(BITMAP_FLARE_RED, Position, 0.7f, Light, o);

			Vector(6.0f, 3.0f, 3.0f, Position);
			BoneManager::GetBonePosition(o, 26, Position, Position);
			CreateSprite(BITMAP_FLARE_RED, Position, 1.3f, Light, o);
			Vector(-5.0f, 10.0f, 5.0f, Position);
			BoneManager::GetBonePosition(o, 26, Position, Position);
			CreateSprite(BITMAP_FLARE_RED, Position, 0.7f, Light, o);

			Vector(0.0f, 0.0f, -5.0f, Position);
			BoneManager::GetBonePosition(o, 54, Position, Position);
			CreateSprite(BITMAP_FLARE_RED, Position, 1.0f, Light, o);
			Vector(0.0f, 0.0f, -2.0f, Position);
			BoneManager::GetBonePosition(o, 43, Position, Position);
			CreateSprite(BITMAP_FLARE_RED, Position, 1.0f, Light, o);
			Vector(0.0f, 0.0f, -5.0f, Position);
			BoneManager::GetBonePosition(o, 32, Position, Position);
			CreateSprite(BITMAP_FLARE_RED, Position, 1.0f, Light, o);
			Vector(0.0f, 0.0f, -5.0f, Position);
			BoneManager::GetBonePosition(o, 6, Position, Position);
			CreateSprite(BITMAP_FLARE_RED, Position, 1.0f, Light, o);
			Vector(10.0f, -4.0f, 0.0f, Position);
			BoneManager::GetBonePosition(o, 21, Position, Position);
			CreateSprite(BITMAP_FLARE_RED, Position, 1.0f, Light, o);

		}
		return true;
	case MODEL_MONSTER01 + 120:
		{
		}
		return true;
	case MODEL_SMELTING_NPC:
		{
			o->Scale = 2.5f;
			Position[0] = o->Position[0] + (mu_float)(GetLargeRand(250) - 125);
			Position[1] = o->Position[1] + (mu_float)(GetLargeRand(250) - 125);
			Position[2] = o->Position[2] - (mu_float)(GetLargeRand(100)) + 50.0f;

			if (GetLargeRand(10) == 0)
			{
				Vector(0.5f, 1.0f, 0.8f, Light);
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, o->Angle, Light, 17, 0.6f);
			}
		}
		return true;
	}

	return false;
}

mu_boolean M39Kanturu3rd::RenderKanturu3rdMonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	switch (o->Type)
	{
	case MODEL_MONSTER01 + 114:
		{
			mu_float fLumi2 = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi2, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_PRSONA_EFFECT);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 2, fLumi2, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_PRSONA_EFFECT2);
			return true;
		}
		break;
	case MODEL_MONSTER01 + 115:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
			mu_float fLumi2 = (EMath::Sin(FWorldTime*0.002f) + 1.0f);

			if (o->CurrentAction != MONSTER01_DIE)
			{
				Vector(0.9f, 0.9f, 1.0f, b->BodyLight(ThreadIndex));
			}
			else
			{
				Vector(0.3f, 1.0f, 0.2f, b->BodyLight(ThreadIndex));
			}

			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 2, fLumi2, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_TWINTAIL_EFFECT);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 3, fLumi);
			return true;
		}
		break;
	case MODEL_MONSTER01 + 116:
		{
			if (o->CurrentAction == MONSTER01_DIE)
			{
				o->Alpha -= 0.1f;
				if (o->Alpha <= 0.0f)
				{
					o->Alpha = 0.0f;
				}
			}
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			return true;
		}
		break;
	case MODEL_MONSTER01 + 121:
		{
			b->BodyLight(ThreadIndex) = EVector3(1.0f, 1.0f, 1.0f);

			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			mu_float fLumi = (EMath::Sin(FWorldTime*0.003f) + 1.0f) * 0.35f;

			b->BodyLight(ThreadIndex) = EVector3(fLumi, fLumi, fLumi);

			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_NIGHTMARE_EFFECT2);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_NIGHTMARE_EFFECT1);

			if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				b->BodyLight(ThreadIndex) = EVector3(0.7f, 0.7f, 1.0f);

				b->RenderMesh(ThreadIndex, 2, RENDER_BRIGHT | RENDER_CHROME2, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, 3, RENDER_BRIGHT | RENDER_CHROME2, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}
		}
		return true;
	case MODEL_MONSTER01 + 118:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
		}
		return true;
	case MODEL_MONSTER01 + 119:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case MODEL_SUMMON:
	case MODEL_STORM2:
		{
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		}
		return true;
	}
	return false;
}

mu_boolean M39Kanturu3rd::AttackEffectKanturu3rdMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (!IsInKanturu3rd())
		return false;

	EVector3 Light, Position, Direction, Angle;

	switch (c->MonsterIndex)
	{
	case 354:
		{
			if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (c->AttackTime == 14)
				{
					EVector3 vPos, vRelative, Light;
					Vector(140.0f, 0.0f, -30.0f, vRelative);
					Vector(0.2f, 0.2f, 1.0f, Light);
					BoneManager::GetBonePosition(o, 12, vRelative, vPos);

					CreateEffect(ThreadIndex, MODEL_BLADE_SKILL, vPos, o->Angle, Light, 0);

					CreateParticle(ThreadIndex, BITMAP_EXPLOTION + 1, vPos, o->Angle, o->Light, 0, 1.3f);
					CreateParticle(ThreadIndex, BITMAP_EXPLOTION + 1, vPos, o->Angle, o->Light, 0, 2.3f);
					CreateParticle(ThreadIndex, BITMAP_EXPLOTION + 1, vPos, o->Angle, o->Light, 0, 1.8f);
					CreateParticle(ThreadIndex, BITMAP_EXPLOTION + 1, vPos, o->Angle, o->Light, 0, 1.3f);
					CreateParticle(ThreadIndex, BITMAP_EXPLOTION + 1, vPos, o->Angle, o->Light, 0, 2.3f);
					CreateParticle(ThreadIndex, BITMAP_EXPLOTION + 1, vPos, o->Angle, o->Light, 0, 1.8f);

				}
			}
		}
		return true;
	case 361:
		{
			if (o->CurrentAction == MONSTER01_ATTACK3 && c->AttackTime >= 14)
			{
				Vector(0.3f, 0.2f, 0.1f, Light);
				CreateEffect(ThreadIndex, MODEL_SUMMON, o->Position, o->Angle, Light, 3);
			}
			else if (o->CurrentAction == MONSTER01_ATTACK4 && c->AttackTime >= 10)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateInferno(ThreadIndex, o->Position);
				if (c->AttackTime == 10)
					CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
				else if (c->AttackTime == 14)
					CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
			}
		}
		return true;
	case 362:
		{
			if (o->CurrentAction == MONSTER01_ATTACK1 && c->AttackTime == 14)
			{
				CreateInferno(ThreadIndex, o->Position, 2);
				Vector(0.0f, 0.5f, 1.0f, Light);
				CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, o->Position, o->Angle, Light, 7);

				PlayBackground(SOUND_KANTURU_3RD_MAYAHAND_ATTACK1);
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2 && c->AttackTime == 14)
			{
				EBone Matrix;
				Vector(0.0f, 0.0f, 0.0f, Angle);
				Vector(0.0f, -160.0f, 0.0f, Direction);
				AngleMatrix(Angle, Matrix);
				VectorRotate(Direction, Matrix, Position);
				VectorAdd(o->Position, Position, Position);

				Vector(0.3f, 0.5f, 1.0f, Light);
				CreateEffect(ThreadIndex, MODEL_MAYAHANDSKILL, Position, o->Angle, Light, 0, nullptr, -1, 0, 0, 0, 1.0f);

				PlayBackground(SOUND_KANTURU_3RD_MAYAHAND_ATTACK2);
			}
		}
		return true;
	case 363:
		{
			if (o->CurrentAction == MONSTER01_ATTACK1 && c->AttackTime == 14)
			{
				CreateInferno(ThreadIndex, o->Position, 3);
				Vector(1.0f, 0.5f, 0.0f, Light);
				CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, o->Position, o->Angle, Light, 7);

				PlayBackground(SOUND_KANTURU_3RD_MAYAHAND_ATTACK1);
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2 && c->AttackTime == 14)
			{
				EBone Matrix;
				Vector(0.0f, 0.0f, 0.0f, Angle);
				Vector(0.0f, -160.0f, 0.0f, Direction);
				AngleMatrix(Angle, Matrix);
				VectorRotate(Direction, Matrix, Position);
				VectorAdd(o->Position, Position, Position);

				Vector(1.0f, 0.6f, 0.4f, Light);
				CreateEffect(ThreadIndex, MODEL_MAYAHANDSKILL, Position, o->Angle, Light, 0, nullptr, -1, 0, 0, 0, 1.0f);

				PlayBackground(SOUND_KANTURU_3RD_MAYAHAND_ATTACK2);
			}
		}
		return true;
	case 364:
		return true;
	}

	return false;
}

mu_boolean M39Kanturu3rd::SetCurrentActionKanturu3rdMonster(CHARACTER* c, OBJECT* o)
{
	if(!IsInKanturu3rd())
		return false;

	switch(c->MonsterIndex)
	{
	case 361:
	case 362:
	case 363:
	case 364:
		return CheckMonsterSkill(c, o);
	}

	return false;
}

void M39Kanturu3rd::MayaSceneMayaAction(mu_uint8 Skill)
{
	iMayaAction = Skill;

	switch(iMayaAction)
	{
	case 0:
		break;
	case 1:
		bMayaSkill2 = true;
		break;
	}
}

void M39Kanturu3rd::MayaAction(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 Angle, Direction, Position, Pos, Light;

	if (g_Direction.m_CKanturu.GetMayaExplotion())
		iMayaAction = 2;

	switch (iMayaAction)
	{
	case 0:
		{
			EBone Matrix;

			Vector(0.0f, 0.0f, 0.0f, Angle);
			Vector(-200.0f + (GetLargeRand(60) + 50.0f), 0.0f, 0.0f, Direction);
			AngleMatrix(Angle, Matrix);
			VectorRotate(Direction, Matrix, Position);

			Vector(0.2f, 0.2f, 0.4f, Light);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(34)], Position, Pos, false);
			CreateEffect(ThreadIndex, MODEL_STORM3, Pos, o->Angle, Light);

			for (mu_int32 i = 0; i < 100; ++i)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				Pos[0] = Hero->Object.Position[0] + (mu_float)(GetLargeRand(20) - 10)*90.0f;
				Pos[1] = Hero->Object.Position[1] + (mu_float)(GetLargeRand(20) - 10)*90.0f;
				Pos[2] = Hero->Object.Position[2] - (mu_float)(GetLargeRand(5))*100.0f - 500.0f;
				CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), Pos, o->Angle, Light, 2);
			}
			iMayaAction = -1;

			PlayBackground(SOUND_KANTURU_3RD_MAYA_STORM);
		}
		break;
	case 1:
		{
			if (bMayaSkill2)
			{
				if (iMayaSkill2_Counter == 0 || g_Time.GetTimeCheck(0, 1000))
				{
					for (mu_int32 i = 0; i < 10; ++i)
					{
						Pos[0] = Hero->Object.Position[0] + (mu_float)(GetLargeRand(20) - 10)*80.0f + 500.0f;
						Pos[1] = Hero->Object.Position[1] + (mu_float)(GetLargeRand(20) - 10)*80.0f;
						Pos[2] = Hero->Object.Position[2] + 300.0f + (mu_float)(GetLargeRand(10))*100.0f;
						mu_float Scale = 5.0f + GetLargeRand(10) / 3.0f;
						mu_int32 index = MODEL_MAYASTONE1 + GetLargeRand(3);
						Vector(1.0f, 1.0f, 1.0f, Light);
						CreateEffect(ThreadIndex, index, Pos, o->Angle, Light, 0, nullptr, -1, 0, 0, 0, Scale);
						CreateEffect(ThreadIndex, MODEL_MAYASTONEFIRE, Pos, o->Angle, Light, index, nullptr, -1, 0, 0, 0, Scale);
					}

					++iMayaSkill2_Counter;
				}

				if (iMayaSkill2_Counter > 3)
				{
					iMayaAction = -1;
					bMayaSkill2 = false;
					iMayaSkill2_Counter = 0;
				}
			}
		}
		break;
	case 2:
		{
			if (g_Direction.m_CKanturu.GetMayaExplotion())
			{
				mu_int32 Index[11] = { 3, 5, 18, 19, 23, 25, 26, 27, 28, 29, 30 };

				if (iMayaDie_Counter == 0)
				{
					if (GetLargeRand(5) == 0)
					{
						for (mu_int32 j = 0; j < 3; ++j)
						{
							Vector(GetLargeRand(20) - 10.0f, GetLargeRand(20) - 10.0f, GetLargeRand(20) - 10.0f, Position);
							Vector(0.0f, 0.0f, 0.0f, Position);
							Vector(1.0f, 1.0f, 1.0f, Light);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(33 + GetLargeRand(42))], Position, Pos, false);
							CreateParticle(ThreadIndex, BITMAP_EXPLOTION, Pos, o->Angle, Light, 1, (1.5f + GetLargeRand(10) / 20.0f));
						}
					}

					if (g_Time.GetTimeCheck(1, 1200))
						++iMayaDie_Counter;

					PlayBackground(SOUND_KANTURU_3RD_MAYA_END);
				}
				else if (iMayaDie_Counter == 1 || iMayaDie_Counter == 3)
				{
					Vector(0.0f, -100.0f, 0.0f, Position);
					Vector(1.0f, 1.0f, 1.0f, Light);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(11)], Position, Pos, false);
					CreateEffect(ThreadIndex, MODEL_MAYASTAR, Pos, o->Angle, Light);

					Vector(0.0f, -0.0f, 0.0f, Position);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(21)], Position, Pos, false);
					CreateEffect(ThreadIndex, MODEL_MAYASTAR, Pos, o->Angle, Light);

					Vector(0.0f, -0.0f, 0.0f, Position);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(24)], Position, Pos, false);
					CreateEffect(ThreadIndex, MODEL_MAYASTAR, Pos, o->Angle, Light);

					++iMayaDie_Counter;
				}
				else if (iMayaDie_Counter >= 5)
				{
					iMayaAction = -1;
					g_Direction.m_CKanturu.SetMayaExplotion(false);
					iMayaDie_Counter = 0;
				}
				else
				{
					for (mu_int32 i = 0; i < 3; ++i)
					{
						Vector(1.0f, 1.0f, 1.0f, Light);
						Vector(GetLargeRand(100) - 50.0f, GetLargeRand(100) - 50.0f, GetLargeRand(100) - 50.0f, Position);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(Index[GetLargeRand(11)])], Position, Pos, false);
						CreateParticle(ThreadIndex, BITMAP_EXPLOTION, Pos, o->Angle, Light, 1, 1.0f + (i*1.5f) + GetLargeRand(10) / 10.0f);

						Vector(1.0f, 0.5f, 0.3f, Light);
						Vector(GetLargeRand(200) - 100.0f, GetLargeRand(200) - 100.0f, GetLargeRand(200) - 100.0f, Position);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(Index[GetLargeRand(11)])], Position, Pos, false);
						CreateParticle(ThreadIndex, BITMAP_SMOKE, Pos, o->Angle, Light, 47, 1.5f + GetLargeRand(4));
					}

					if (g_Time.GetTimeCheck(2, 1200))
						++iMayaDie_Counter;
				}
			}
		}
		break;
	}
}

void M39Kanturu3rd::Kanturu3rdState(mu_uint8 State, mu_uint8 DetailState)
{
	g_Direction.m_CKanturu.GetKanturuAllState(State, DetailState);
}

void M39Kanturu3rd::Kanturu3rdResult(mu_uint8 Result)
{
	switch (Result)
	{
	case 0:
		iKanturuResult = 0;
		break;
	case 1:
		{
			if (g_Direction.m_CKanturu.m_iKanturuState != KANTURU_STATE_NIGHTMARE_BATTLE)
				break;

			iKanturuResult = 1;
		}
		break;
	}
}

void M39Kanturu3rd::Kanturu3rdUserandMonsterCount(mu_int32 Count1, mu_int32 Count2)
{
	UserCount = Count2;
	MonsterCount = Count1;
}

void M39Kanturu3rd::RenderKanturu3rdinterface()
{
	if(!IsInKanturu3rd() || iKanturuResult == -1)
	{
		return;
	}

	RenderKanturu3rdResultInterface();
}

void M39Kanturu3rd::RenderKanturu3rdResultInterface()
{
	if(iKanturuResult == 1)
		Kanturu3rdSuccess();
	else if(iKanturuResult == 0)
		Kanturu3rdFailed();
	else
	{
		iKanturuResult = -1;
		fAlpha = 0.1f;
	}
}

void M39Kanturu3rd::Kanturu3rdSuccess()
{
#if 0 // FIX
	mu_float fPosX, fPosY, fWidth, fHeight, tu, tv;

	fWidth = 372.0f;
	fHeight = 99.0f;
	fPosX = (640.0f - fWidth) * 0.5f;
	fPosY = (480.0f - fWidth) * 0.5f;
	tu = fWidth / 512.0f;
	tv = fHeight / 128.0f;

	if (fAlpha <= 1.0f)
		fAlpha += 0.01f;
	else if (fAlpha >= 0.99f && g_Time.GetTimeCheck(3, 5000))
	{
		fAlpha = 0.1f;
		iKanturuResult = -1;
	}

	EnableAlphaTest();
	RenderBitmap(BITMAP_KANTURU_SUCCESS, fPosX, fPosY, fWidth, fHeight, 0.0f, 0.0f, tu, tv, true, true, fAlpha);
#endif
}

void M39Kanturu3rd::Kanturu3rdFailed()
{
#if 0 // FIX
	mu_float fPosX, fPosY, fWidth, fHeight, tu, tv;

	fWidth = 372.0f;
	fHeight = 99.0f;
	fPosX = (640.0f - fWidth) * 0.5f;
	fPosY = (480.0f - fWidth) * 0.5f;
	tu = fWidth / 512.0f;
	tv = fHeight / 128.0f;

	if (fAlpha <= 1.0f)
		fAlpha += 0.01f;
	else if (fAlpha >= 0.99f && g_Time.GetTimeCheck(4, 5000))
	{
		fAlpha = 0.1f;
		iKanturuResult = -1;
	}

	EnableAlphaTest();
	RenderBitmap(BITMAP_KANTURU_FAILED, fPosX, fPosY, fWidth, fHeight, 0.0f, 0.0f, tu, tv, true, true, fAlpha);
#endif
}

void M39Kanturu3rd::PlayBGM()
{
	if(GET_WORLD == WD_39KANTURU_3RD)
	{
		if(g_Direction.m_CKanturu.m_iKanturuState == KANTURU_STATE_MAYA_BATTLE || g_Direction.m_CKanturu.m_iKanturuState == KANTURU_STATE_STANDBY) 
		{
			EMusicBackend::Stop(MUSICS::Data[MUSIC_KANTURU_TOWER]);
			EMusicBackend::Play(MUSICS::Data[MUSIC_KANTURU_MAYA_BATTLE]);
		}
		else if(g_Direction.m_CKanturu.m_iKanturuState == KANTURU_STATE_NIGHTMARE_BATTLE)
		{
			EMusicBackend::Stop(MUSICS::Data[MUSIC_KANTURU_TOWER]);
			EMusicBackend::Stop(MUSICS::Data[MUSIC_KANTURU_MAYA_BATTLE]);
			EMusicBackend::Play(MUSICS::Data[MUSIC_KANTURU_NIGHTMARE_BATTLE]);
		}
		else
		{
			EMusicBackend::Stop(MUSICS::Data[MUSIC_KANTURU_MAYA_BATTLE]);
			EMusicBackend::Stop(MUSICS::Data[MUSIC_KANTURU_NIGHTMARE_BATTLE]);			
			EMusicBackend::Play(MUSICS::Data[MUSIC_KANTURU_TOWER]);
		}
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_KANTURU_MAYA_BATTLE]);
		EMusicBackend::Stop(MUSICS::Data[MUSIC_KANTURU_NIGHTMARE_BATTLE]);
		EMusicBackend::Stop(MUSICS::Data[MUSIC_KANTURU_TOWER]);
	}
}