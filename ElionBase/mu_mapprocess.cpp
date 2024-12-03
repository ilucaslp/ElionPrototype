#include "stdafx.h"
#include "mu_mapprocess.h"

MUMapProcess g_MapProcess;

MUMapProcess::MUMapProcess()
{

}

MUMapProcess::~MUMapProcess()
{

}

mu_boolean MUMapProcess::CreateObject(const mu_uint32 ThreadIndex, mu_uint32 objectIndex, OBJECT *o)
{
	switch (GET_WORLD)
	{
	case WD_0LORENCIA:
		switch (o->Type)
		{
		case MODEL_BRIDGE:
			o->CollisionRange = -50.0f;
			break;
		case MODEL_HOUSE01 + 2:
			o->BlendMesh = 4;
			break;
		case MODEL_HOUSE01 + 3:
			o->BlendMesh = 8;
			break;
		case MODEL_HOUSE01 + 4:
			o->BlendMesh = 2;
			break;
		case MODEL_HOUSE_WALL01 + 1:
			o->BlendMesh = 4;
			break;
		case MODEL_WATERSPOUT:
			o->BlendMesh = 3;
			break;
		case MODEL_BONFIRE:
			o->BlendMesh = 1;
			break;
		case MODEL_CARRIAGE01:
			o->BlendMesh = 2;
			break;
		case MODEL_TREE01:
		case MODEL_TREE01 + 1:
			Vector(-150.0f, -150.0f, 0.0f, o->BoundingBoxMin);
			Vector(150.0f, 150.0f, 500.0f, o->BoundingBoxMax);
			o->Velocity = 1.0f / o->Scale*0.4f;
			break;
		case MODEL_STREET_LIGHT:
			o->BlendMesh = 1;
			o->Velocity = 0.3f;
			break;
		case MODEL_CANDLE:
			o->BlendMesh = 1;
			o->Velocity = 0.3f;
			break;
		case MODEL_TREASURE_CHEST:
			o->Velocity = 0.0f;
			break;
		case MODEL_SIGN01:
		case MODEL_SIGN01 + 1:
			o->Velocity = 0.3f;
			break;
		case MODEL_POSE_BOX:
			CreateOperate(objectIndex, o);
			Vector(40.0f, 40.0f, 160.0f, o->BoundingBoxMax);
			o->HiddenMesh = -2;
			break;
		case MODEL_TREE01 + 6:
		case MODEL_FURNITURE01 + 5:
		case MODEL_FURNITURE01 + 6:
			CreateOperate(objectIndex, o);
			break;
		}
		break;
	case WD_1DUNGEON:
		switch (o->Type)
		{
		case 59:
			CreateOperate(objectIndex, o);
			break;
		case 60:
			CreateOperate(objectIndex, o);
			Vector(40.0f, 40.0f, 160.0f, o->BoundingBoxMax);
			o->HiddenMesh = -2;
			break;
		}
		break;
	case WD_2DEVIAS:
		switch (o->Type)
		{
		case MODEL_WARP:
			{
				EVector3 Position;
				Vector(o->Position[0], o->Position[1], o->Position[2] + 360.0f, Position);
				CreateEffect(ThreadIndex, MODEL_WARP, Position, o->Angle, o->Light, 1);

				Vector(o->Position[0], o->Position[1] + 4.0f, o->Position[2] + 360.0f, Position);
				CreateEffect(ThreadIndex, MODEL_WARP2, Position, o->Angle, o->Light, 1);

				Vector(o->Position[0], o->Position[1] + 20.0f, o->Position[2] + 360.0f, Position);
				CreateEffect(ThreadIndex, MODEL_WARP3, Position, o->Angle, o->Light, 1);
			}
			break;
		case 22:
		case 25:
		case 40:
		case 45:
		case 55:
		case 73:
			CreateOperate(objectIndex, o);
			break;
		case 91:
			CreateOperate(objectIndex, o);
			Vector(40.0f, 40.0f, 160.0f, o->BoundingBoxMax);
			o->HiddenMesh = -2;
			break;
		case 19:
		case 92:
		case 93:
			o->BlendMesh = 0;
			break;
		case 54:
		case 56:
			o->BlendMesh = 1;
			break;
		case 78:
			o->BlendMesh = 3;
			break;
		case 20:
		case 65:
		case 88:
		case 86:
			o->Angle[2] = (mu_float)((mu_int32)o->Angle[2] % 360);
			VectorCopy(o->Angle, o->HeadAngle);
			VectorCopy(o->Position, o->HeadTargetAngle);
			break;
		case 100:
			o->HiddenMesh = -2;
			break;
		}
		break;
	case WD_3NORIA:
		switch (o->Type)
		{
		case MODEL_WARP:
			{
				EVector3 Position;
				Vector(o->Position[0], o->Position[1], o->Position[2] + 350.0f, Position);
				CreateEffect(ThreadIndex, MODEL_WARP, Position, o->Angle, o->Light);

				Vector(o->Position[0], o->Position[1] + 4.0f, o->Position[2] + 350.0f, Position);
				CreateEffect(ThreadIndex, MODEL_WARP2, Position, o->Angle, o->Light);

				Vector(o->Position[0], o->Position[1] + 8.0f, o->Position[2] + 350.0f, Position);
				CreateEffect(ThreadIndex, MODEL_WARP, Position, o->Angle, o->Light);

				Vector(o->Position[0], o->Position[1] + 12.0f, o->Position[2] + 350.0f, Position);
				CreateEffect(ThreadIndex, MODEL_WARP2, Position, o->Angle, o->Light);

				Vector(o->Position[0], o->Position[1] + 20.0f, o->Position[2] + 350.0f, Position);
				CreateEffect(ThreadIndex, MODEL_WARP3, Position, o->Angle, o->Light);
			}
			break;
		case 8:
			CreateOperate(objectIndex, o);
			break;
		case 1:
			o->BlendMesh = 1;
			break;
		case 9:
			o->BlendMesh = 3;
			break;
		case 38:
			CreateOperate(objectIndex, o);
			o->HiddenMesh = -2;
			break;
		case 17:
		case 37:
			o->BlendMesh = 0;
			break;
		case 19:
			o->BlendMesh = 0;
			break;
		case 18:
			o->BlendMesh = 2;
			break;
		}
		break;

	case WD_7ATLANSE:
		switch (o->Type)
		{
		case 39:
			CreateOperate(objectIndex, o);
			o->HiddenMesh = -2;
			break;
		}
		break;

	case WD_8TARKAN:
		switch (o->Type)
		{
		case 78:
			CreateOperate(objectIndex, o);
			break;
		}
		break;

	case WD_18CHAOS_CASTLE:
	case WD_18CHAOS_CASTLE + 1:
	case WD_18CHAOS_CASTLE + 2:
	case WD_18CHAOS_CASTLE + 3:
	case WD_18CHAOS_CASTLE + 4:
	case WD_18CHAOS_CASTLE + 5:
	case WD_53CAOSCASTLE_MASTER_LEVEL:
		{
			CreateChaosCastleObject(objectIndex, o);
		}
		break;

	case WD_30BATTLECASTLE:
		{
			battleCastle::CreateBattleCastleObject(ThreadIndex, objectIndex, o);
		}
		break;

	case WD_31HUNTING_GROUND:
		{
			M31HuntingGround::CreateHuntingGroundObject(objectIndex, o);
		}
		break;

	case WD_38KANTURU_2ND:
		{
			M38Kanturu2nd::Create_Kanturu2nd_Object(objectIndex, o);
		}
		break;

	case WD_39KANTURU_3RD:
		{
			M39Kanturu3rd::CreateKanturu3rdObject(objectIndex, o);
		}
		break;

	case WD_51HOME_6TH_CHAR:
		{
			GMNewTown::CreateObject(objectIndex, o);
		}
		break;

	case WD_57ICECITY:
	case WD_58ICECITY_BOSS:
		return g_Raklion.CreateObject(ThreadIndex, objectIndex, o);

	case WD_62SANTA_TOWN:
		return g_SantaTown.CreateObject(objectIndex, o);

	case WD_63PK_FIELD:
		return g_PKField.CreateObject(objectIndex, o);

	case WD_64DUELARENA:
		return g_DuelArena.CreateObject(objectIndex, o);

	case WD_65DOPPLEGANGER1:
		return false;

	case WD_66DOPPLEGANGER2:
		return g_DoppelGanger2.CreateObject(objectIndex, o);

	case WD_67DOPPLEGANGER3:
		return false;

	case WD_68DOPPLEGANGER4:
		return false;

	case WD_69EMPIREGUARDIAN1:
		return g_EmpireGuardian1.CreateObject(objectIndex, o);

	case WD_70EMPIREGUARDIAN2:
		return g_EmpireGuardian2.CreateObject(objectIndex, o);

	case WD_71EMPIREGUARDIAN3:
		return g_EmpireGuardian3.CreateObject(objectIndex, o);

	case WD_72EMPIREGUARDIAN4:
		return g_EmpireGuardian4.CreateObject(objectIndex, o);

	case WD_73NEW_LOGIN_SCENE:
	case WD_74NEW_CHARACTER_SCENE:
		{
			if (g_EmpireGuardian4.CreateObject(objectIndex, o))
			{
				switch (o->Type)
				{
				case 129:	case 79:	case 83:
				case 82:	case 85:	case 86:
				case 130:	case 131:	case 158:
					o->HiddenMesh = -2;
					break;
				}

				return true;
			}
		}
		break;

	case WD_79UNITEDMARKETPLACE:
		return g_UnitedMarketPlace.CreateObject(objectIndex, o);
	}

	return false;
}

mu_boolean MUMapProcess::MoveObject(const mu_uint32 ThreadIndex, OBJECT *o)
{
	MUModel *b = &MODELS::Data[o->Type];

	EVector3 p;
	EVector3 Light;
	mu_float Luminosity;
	Vector(0.0f, 0.0f, 0.0f, p);

	switch (GET_WORLD)
	{
	case WD_0LORENCIA:
		switch (o->Type)
		{
		case MODEL_HOUSE01 + 3:
		case MODEL_HOUSE01 + 4:
			o->BlendMeshTexCoordV = -IWorldTime % 1000 * 0.001f;
			break;
		case MODEL_HOUSE01 + 2:
		case MODEL_HOUSE_WALL01 + 1:
			o->BlendMeshLight = (mu_float)(GetLargeRand(4) + 4)*0.1f;
			break;
		case MODEL_LIGHT01:
			CreateFire(ThreadIndex, 0, o, 0.0f, 0.0f, 0.0f);
			o->HiddenMesh = -2;
			break;
		case MODEL_LIGHT01 + 1:
			CreateFire(ThreadIndex, 1, o, 0.0f, 0.0f, 0.0f);
			o->HiddenMesh = -2;
			break;
		case MODEL_LIGHT01 + 2:
			CreateFire(ThreadIndex, 2, o, 0.0f, 0.0f, 0.0f);
			o->HiddenMesh = -2;
			break;
		case MODEL_BRIDGE:
			CreateFire(ThreadIndex, 0, o, 90.0f, -200.0f, 30.0f);
			CreateFire(ThreadIndex, 0, o, 90.0f, 200.0f, 30.0f);
			break;
		case MODEL_DUNGEON_GATE:
			CreateFire(ThreadIndex, 0, o, -150.0f, -150.0f, 140.0f);
			CreateFire(ThreadIndex, 0, o, 150.0f, -150.0f, 140.0f);
			break;
		case MODEL_FIRE_LIGHT01:
			CreateFire(ThreadIndex, 0, o, 0.0f, 0.0f, 200.0f);
			break;
		case MODEL_FIRE_LIGHT01 + 1:
			CreateFire(ThreadIndex, 0, o, 0.0f, -30.0f, 60.0f);
			break;
		case MODEL_STREET_LIGHT:
			Luminosity = (mu_float)(GetLargeRand(2) + 6)*0.1f;
			Vector(Luminosity, Luminosity*0.8f, Luminosity*0.6f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
			break;
		case MODEL_BONFIRE:
			CreateFire(ThreadIndex, 0, o, 0.0f, 0.0f, 60.0f);
			o->BlendMeshLight = (mu_float)(GetLargeRand(6) + 4)*0.1f;
			break;
		case MODEL_CANDLE:
			Luminosity = (mu_float)(GetLargeRand(4) + 3)*0.1f;
			Vector(Luminosity, Luminosity*0.6f, Luminosity*0.2f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
			break;
		case MODEL_SIGN01 + 1:
			Vector(50.0f, -10.0f, 120.0f, p);
			break;
		}
		break;

	case WD_1DUNGEON:
		switch (o->Type)
		{
		case 52:
			if (GetLargeRand(3) == 0)
				CreateEffect(ThreadIndex, MODEL_DUNGEON_STONE01, o->Position, o->Angle, o->Light);
			o->HiddenMesh = -2;
			break;
		case 22:
		case 23:
		case 24:
			b->StreamMesh(ThreadIndex) = 1;
			o->BlendMeshTexCoordV = -(mu_float)(IWorldTime % 1000)*0.001f;
			break;
		case 41:
			CreateFire(ThreadIndex, 0, o, 0.0f, -30.0f, 240.0f);
			break;
		case 42:
			CreateFire(ThreadIndex, 0, o, 0.0f, 0.0f, 190.0f);
			break;
		case 39:
		case 40:
		case 51:
			o->HiddenMesh = -2;
			break;
		}
		break;

	case WD_2DEVIAS:
		switch (o->Type)
		{
		case 78:
			o->BlendMeshLight = (mu_float)(GetLargeRand(4) + 4)*0.1f;
			break;
		case 30:
			{
				EVector3 Position;
				Position[0] = o->Position[0];
				Position[1] = o->Position[1];
				Position[2] = o->Position[2] + 160.0f;
				CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, Position, o->Angle,
					o->Light, 0, o->Scale);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle,
					o->Light, 21, 0.5f + ((GetLargeRand(9))*0.1f));
			}
			break;
		case 66:
			CreateFire(ThreadIndex, 0, o, 0.0f, 0.0f, 50.0f);
			break;
		case 86:
		case 20:
		case 65:
		case 88:
			mu_float dx = Hero->Object.Position[0] - o->HeadTargetAngle[0];
			mu_float dy = Hero->Object.Position[1] - o->HeadTargetAngle[1];
			mu_float Distance = EMath::Sqrt(dx*dx + dy*dy);
			if (Distance < 200.0f)
			{
				if (o->Type == 86)
				{
					if (o->Angle[2] == 90.0f)
						o->Position[1] = o->HeadTargetAngle[1] + (200.0f - Distance)*2.0f;
					if (o->Angle[2] == 270.0f)
						o->Position[1] = o->HeadTargetAngle[1] - (200.0f - Distance)*2.0f;
					if (o->Angle[2] == 0.0f)
						o->Position[0] = o->HeadTargetAngle[0] + (200.0f - Distance)*2.0f;
					if (o->Angle[2] == 180.0f)
						o->Position[0] = o->HeadTargetAngle[0] - (200.0f - Distance)*2.0f;
					PlayBackground(SOUND_DOOR02);
				}
				else
				{
					if (o->HeadAngle[2] == 90.0f)
						o->Angle[2] = 30.0f - (200.0f - Distance)*0.5f;
					if (o->HeadAngle[2] == 270.0f)
						o->Angle[2] = 330.0f + (200.0f - Distance)*0.5f;
					if (o->HeadAngle[2] == 0.0f)
						o->Angle[2] = 300.0f - (200.0f - Distance)*0.5f;
					if (o->HeadAngle[2] == 180.0f)
						o->Angle[2] = 240.0f + (200.0f - Distance)*0.5f;
					PlayBackground(SOUND_DOOR01);
				}
			}
			else
			{
				o->Angle[2] = TurnAngle2(o->Angle[2], o->HeadAngle[2], 10.0f);
				o->Position[0] += (o->HeadTargetAngle[0] - o->Position[0])*0.2f;
				o->Position[1] += (o->HeadTargetAngle[1] - o->Position[1])*0.2f;
			}
			break;
		}
		break;

	case WD_3NORIA:
		switch (o->Type)
		{
		case MODEL_WARP:
			{
				EVector3 Position;
				Vector(o->Position[0], o->Position[1] - 50.0f, o->Position[2] + 350.0f, Position);
				Vector(0.5f, 0.5f, 0.5f, Light);
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, o->Angle, Light, 9, 1.4f);
			}
			break;
		case 18:
			o->BlendMeshTexCoordV = IWorldTime % 1000 * 0.001f;
			break;
		case 39:
			o->BlendMesh = 1;
			break;
		case 41:
			o->BlendMesh = 0;
			o->BlendMeshTexCoordV = IWorldTime % 2000 * 0.0005f;
			break;
		case 42:
			b->StreamMesh(ThreadIndex) = 0;
			o->BlendMeshTexCoordU = -(mu_float)(IWorldTime % 500)*0.002f;
			break;
		case 43:
			b->StreamMesh(ThreadIndex) = 0;
			o->BlendMeshTexCoordU = (mu_float)(IWorldTime % 500)*0.002f;
			break;
		}
		break;

	case WD_4LOSTTOWER:
		switch (o->Type)
		{
		case 38:
		case 39:
			CheckSkull(o);
			break;
		case 3:
		case 4:
			o->BlendMeshTexCoordU = -IWorldTime % 1000 * 0.001f;
			break;
		case 19:
		case 20:
			o->BlendMesh = 4;
			o->BlendMeshTexCoordU = -IWorldTime % 1000 * 0.001f;
			break;
		case 18:
			o->BlendMesh = 1;
			break;
		case 23:
			o->BlendMesh = 1;
			break;
		case 24:
			o->HiddenMesh = -2;
			if (GetLargeRand(64) == 0)
				CreateEffect(ThreadIndex, BITMAP_FLAME, o->Position, o->Angle, o->Light);
			break;
		case 25:
			o->HiddenMesh = -2;
			break;
		}
		break;

	case WD_5UNKNOWN:
		switch (o->Type)
		{
		case 2:
			o->BlendMesh = 0;
			break;
		case 3:
			o->BlendMesh = 0;
			o->BlendMeshLight = (mu_float)(GetLargeRand(4) + 6)*0.1f;
			break;
		}
		break;

	case WD_6STADIUM:
		switch (o->Type)
		{
		case 21:
			o->BlendMesh = 3;
			o->BlendMeshTexCoordV = -IWorldTime % 1000 * 0.001f;
			break;
		case 38:
			o->HiddenMesh = -2;
			break;
		}
		break;

	case WD_7ATLANSE:
		switch (o->Type)
		{
		case 22:
			o->HiddenMesh = -2;
			o->Timer += 0.1f;
			if (o->Timer > 10.0f)
				o->Timer = 0.0f;
			if (o->Timer > 5.0f)
				CreateParticle(ThreadIndex, BITMAP_BUBBLE, o->Position, o->Angle, o->Light);
			break;
		case 23:
			o->BlendMesh = 0;
			o->BlendMeshLight = EMath::Sin(FWorldTime*0.002f)*0.3f + 0.5f;
			break;
		case 32:
		case 34:
			o->BlendMesh = 1;
			o->BlendMeshLight = EMath::Sin(FWorldTime*0.004f)*0.5f + 0.5f;
			break;
		case 38:
			o->BlendMesh = 0;
			break;
		case 40:
			o->BlendMesh = 0;
			o->BlendMeshLight = EMath::Sin(FWorldTime*0.004f)*0.3f + 0.5f;
			o->Velocity = 0.05f;
			break;
		}
		break;

	case WD_8TARKAN:
		switch (o->Type)
		{
		case 2:
			o->BlendMesh = 0;
			o->BlendMeshTexCoordU = -IWorldTime % 1000 * 0.001f;
			break;

		case 4:
			{
				mu_float   sine = (mu_float)EMath::Sin(FWorldTime*0.002f)*0.35f + 0.65f;
				o->BlendMesh = 0;
				o->BlendMeshLight = sine;
				o->BlendMeshTexCoordV = -IWorldTime % 10000 * 0.0001f;

				Luminosity = sine;
				Vector(Luminosity, Luminosity, Luminosity, Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
			}
			break;

		case 7:
			{
				mu_float sine = (mu_float)EMath::Sin((FWorldTime + (o->Angle[2] * 100))*0.002f)*0.35f + 0.65f;

				o->BlendMesh = 0;
				o->BlendMeshLight = sine;

				Luminosity = sine;
				Vector(Luminosity, Luminosity*0.6f, Luminosity*0.2f, Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
			}
			break;

		case 11:
			o->BlendMeshTexCoordV = -IWorldTime % 10000 * 0.0002f;
			break;

		case 12:
			o->BlendMeshTexCoordU = -IWorldTime % 50000 * 0.00005f;
			o->BlendMeshTexCoordV = -IWorldTime % 50000 * 0.00005f;
			break;

		case 13:
			o->BlendMeshTexCoordV = -IWorldTime % 10000 * 0.0002f;
			break;

		case 61:
			o->BlendMesh = 1;
			o->BlendMeshTexCoordV = -IWorldTime % 1000 * 0.001f;

			Luminosity = (mu_float)EMath::Sin(FWorldTime*0.002f)*0.35f + 0.65f;
			Vector(Luminosity, Luminosity*0.6f, Luminosity*0.2f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
			break;

		case 63:
		case 64:
			o->HiddenMesh = -2;
			break;

		case 65:
		case 66:
			o->BlendMesh = 1;
			o->BlendMeshTexCoordV = -IWorldTime % 1000 * 0.001f;

			Luminosity = (mu_float)EMath::Sin(FWorldTime*0.002f)*0.35f + 0.65f;
			Vector(Luminosity, Luminosity*0.6f, Luminosity*0.2f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
			break;

		case 72:
			o->BlendMesh = 0;
			o->BlendMeshTexCoordV = -IWorldTime % 10000 * 0.0002f;
			break;

		case 73:
			o->BlendMeshTexCoordV = -IWorldTime % 10000 * 0.0002f;
			break;

		case 75:
			o->BlendMeshTexCoordV = -IWorldTime % 10000 * 0.0002f;
			break;

		case 79:
			o->BlendMeshTexCoordV = -IWorldTime % 10000 * 0.0002f;
			break;

		case 82:
			o->BlendMesh = 0;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			break;
		}
		break;

	case WD_10HEAVEN:
		break;

	case WD_11BLOODCASTLE1:
	case WD_11BLOODCASTLE1 + 1:
	case WD_11BLOODCASTLE1 + 2:
	case WD_11BLOODCASTLE1 + 3:
	case WD_11BLOODCASTLE1 + 4:
	case WD_11BLOODCASTLE1 + 5:
	case WD_11BLOODCASTLE1 + 6:
	case WD_52BLOODCASTLE_MASTER_LEVEL:
		switch (o->Type)
		{
		case 9:
		case 10:
			if (o->PKKey != 4)
				o->HiddenMesh = -2;
			break;
		}
		break;

	case WD_24HELLAS:
	case WD_24HELLAS + 1:
	case WD_24HELLAS + 2:
	case WD_24HELLAS + 3:
	case WD_24HELLAS + 4:
	case WD_24HELLAS + 5:
	case WD_24HELLAS_7:
		{
			MoveHellasVisual(o);
		}
		break;

	case WD_30BATTLECASTLE:
		{
			battleCastle::MoveBattleCastleVisual(o);
		}
		break;

	case WD_31HUNTING_GROUND:
		{M31HuntingGround::MoveHuntingGroundObject(ThreadIndex, o);
		}
		break;

	case WD_33AIDA:
		{
			M33Aida::MoveAidaObject(o);
		}
		break;

	case WD_34CRYWOLF_1ST:
		{
			M34CryWolf1st::MoveCryWolf1stObject(o);
		}
		break;

	case WD_37KANTURU_1ST:
	case WD_40AREA_FOR_GM:
		{
			M37Kanturu1st::MoveKanturu1stObject(ThreadIndex, o);
		}
		break;

	case WD_38KANTURU_2ND:
		{
			M38Kanturu2nd::Move_Kanturu2nd_Object(o);
		}
		break;

	case WD_39KANTURU_3RD:
		{
			M39Kanturu3rd::MoveKanturu3rdObject(o);
		}
		break;

	case WD_41CHANGEUP3RD_1ST:
	case WD_42CHANGEUP3RD_2ND:
		{
			g_3rdChangeUp.MoveObject(o);
		}
		break;

	case WD_45CURSEDTEMPLE_LV1:
	case WD_45CURSEDTEMPLE_LV2:
	case WD_45CURSEDTEMPLE_LV3:
	case WD_45CURSEDTEMPLE_LV4:
	case WD_45CURSEDTEMPLE_LV5:
	case WD_45CURSEDTEMPLE_LV6:
		return g_CursedTemple.MoveObject(o);

	case WD_51HOME_6TH_CHAR:
		GMNewTown::PlayObjectSound(o);
		return GMNewTown::MoveObject(o);

	case WD_56MAP_SWAMP_OF_QUIET:
		return GMSwampOfQuiet::MoveObject(o);

	case WD_57ICECITY:
	case WD_58ICECITY_BOSS:
		g_Raklion.PlayObjectSound(o);
		return g_Raklion.MoveObject(ThreadIndex, o);

	case WD_62SANTA_TOWN:
		return g_SantaTown.MoveObject(o);

	case WD_63PK_FIELD:
		g_PKField.PlayObjectSound(o);
		return g_PKField.MoveObject(o);

	case WD_64DUELARENA:
		return g_DuelArena.MoveObject(o);

	case WD_65DOPPLEGANGER1:
		return g_DoppelGanger1.MoveObject(o);

	case WD_66DOPPLEGANGER2:
		return g_DoppelGanger2.MoveObject(o);

	case WD_67DOPPLEGANGER3:
		return g_DoppelGanger3.MoveObject(ThreadIndex, o);

	case WD_68DOPPLEGANGER4:
		return g_DoppelGanger4.MoveObject(ThreadIndex, o);

	case WD_69EMPIREGUARDIAN1:
		return g_EmpireGuardian1.MoveObject(ThreadIndex, o);

	case WD_70EMPIREGUARDIAN2:
		return g_EmpireGuardian2.MoveObject(ThreadIndex, o);

	case WD_71EMPIREGUARDIAN3:
		return g_EmpireGuardian3.MoveObject(ThreadIndex, o);

	case WD_72EMPIREGUARDIAN4:
		return g_EmpireGuardian4.MoveObject(ThreadIndex, o);

	case WD_73NEW_LOGIN_SCENE:
	case WD_74NEW_CHARACTER_SCENE:
		return g_EmpireGuardian4.MoveObject(ThreadIndex, o);

	case WD_79UNITEDMARKETPLACE:
		return g_UnitedMarketPlace.MoveObject(ThreadIndex, o);
	}

	return false;
}

mu_boolean MUMapProcess::RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT *o, MUModel *b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 p, Position;
	EVector3 Light;
	mu_float Luminosity = (mu_float)(GetLargeRand(30) + 70)*0.01f;
	mu_int32 Bitmap;
	mu_float Scale;
	mu_float Rotation;
	Vector(0.0f, 0.0f, 0.0f, p);

	switch (GET_WORLD)
	{
	case WD_0LORENCIA:
		switch (o->Type)
		{
		case MODEL_WATERSPOUT:
			o->BlendMeshLight = 1.0f;
			o->BlendMeshTexCoordV = -IWorldTime % 1000 * 0.001f;
			if (GetLargeRand(2) == 0)
			{
				Vector((mu_float)(GetLargeRand(32) - 16), -20.0f, (mu_float)(GetLargeRand(32) - 16), p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(1)], p, Position);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, o->Light);
				Vector((mu_float)(GetLargeRand(32) - 16), -80.0f, (mu_float)(GetLargeRand(32) - 16), p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(4)], p, Position);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, o->Light);
			}
			break;
		case MODEL_MERCHANT_ANIMAL01:
			Scale = Luminosity*5.0f;
			Vector(Luminosity*0.6f, Luminosity*0.3f, Luminosity*0.1f, Light);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(48)], Position, p);
			CreateSprite(BITMAP_LIGHT, p, Scale, Light, o);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(57)], Position, p);
			CreateSprite(BITMAP_LIGHT, p, Scale, Light, o);
			break;
		}
		break;
	case WD_2DEVIAS:
		switch (o->Type)
		{
		case 100:
			Vector(Luminosity*1.0f, Luminosity*1.0f, Luminosity*1.0f, Light);
			Rotation = (mu_float)((mu_int32)(FWorldTime*0.1f) % 360);
			Vector(0.0f, 0.0f, 150.0f, p);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(0)], p, Position);
			CreateSprite(BITMAP_LIGHTNING + 1, Position, 2.5f, Light, o, Rotation);
			CreateSprite(BITMAP_LIGHTNING + 1, Position, 2.5f, Light, o, -Rotation);
			break;
		case 103:
			if ((mu_uint32)o->AnimationFrame == b->_AnimationHeader[o->CurrentAction].FrameCount - 1)
			{
				if (GetLargeRand(32) == 0)
					SetAction(o, 1);
				else
					SetAction(o, 0);
			}
			break;
		}
		break;
	case WD_3NORIA:
		switch (o->Type)
		{
		case 9:
			Vector(Luminosity*0.4f, Luminosity*0.7f, Luminosity*1.0f, Light);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(1)], p, Position);
			CreateSprite(BITMAP_LIGHT, Position, 1.5f, Light, o);
			break;
		case 35:
			Vector(Luminosity*0.4f, Luminosity*0.7f, Luminosity*1.0f, Light);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(3)], p, Position);
			CreateSprite(BITMAP_LIGHT, Position, 1.5f, Light, o);
			break;
		case 1:
			Vector(Luminosity*0.4f, Luminosity*0.7f, Luminosity*1.0f, Light);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(2)], p, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.5f, Light, o);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(4)], p, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.5f, Light, o);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(6)], p, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.5f, Light, o);
			break;
		case 17:
			Vector(Luminosity*0.4f, Luminosity*0.7f, Luminosity*1.0f, Light);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(4)], p, Position);
			CreateSprite(BITMAP_LIGHT, Position, 1.0f, Light, o);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(7)], p, Position);
			CreateSprite(BITMAP_LIGHT, Position, 1.0f, Light, o);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(10)], p, Position);
			CreateSprite(BITMAP_LIGHT, Position, 1.0f, Light, o);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(13)], p, Position);
			CreateSprite(BITMAP_LIGHT, Position, 1.0f, Light, o);
			break;
		case 39:
			Vector(Luminosity*0.4f, Luminosity*0.8f, Luminosity*1.0f, Light);
			Rotation = (mu_float)((mu_int32)(FWorldTime*0.1f) % 360);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(57)], p, Position);
			CreateSprite(BITMAP_LIGHTNING + 1, Position, 1.0f, Light, o, Rotation);
			CreateSprite(BITMAP_LIGHTNING + 1, Position, 1.0f, Light, o, -Rotation);

			Vector(1.0f, 1.0f, 1.0f, Light);
			for (mu_int32 i = 61; i <= 65; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(i)], p, Position);
				CreateSprite(BITMAP_LIGHT, Position, 1.0f, Light, o);
				if (GetLargeRand(32) == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_SHINY, Position, o->Angle, Light);
					CreateParticle(ThreadIndex, BITMAP_SHINY, Position, o->Angle, Light, 1);
				}
			}
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(58)], p, Position);
			if (GetLargeRand(8) == 0)
			{
				EVector3 Angle;
				for (mu_int32 i = 0; i < 8; ++i)
				{
					Vector((mu_float)(GetLargeRand(60) + 60), 90.0f + 50.0f, (mu_float)(GetLargeRand(30)), Angle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, Position, Position, Angle);
					CreateParticle(ThreadIndex, BITMAP_SPARK, Position, Angle, Light);
				}
			}
			break;
		}
		break;
	case WD_4LOSTTOWER:
		switch (o->Type)
		{
		case 19:
		case 20:
			if (o->Type == 19)
			{
				Bitmap = BITMAP_MAGIC + 1;
				Vector(Luminosity*1.0f, Luminosity*0.2f, Luminosity*0.0f, Light);
			}
			else
			{
				Bitmap = BITMAP_LIGHTNING + 1;
				Vector(Luminosity*0.4f, Luminosity*0.8f, Luminosity*1.0f, Light);
			}
			Rotation = (mu_float)((mu_int32)(FWorldTime*0.1f) % 360);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(15)], p, Position);
			CreateSprite(Bitmap, Position, 0.3f, Light, o, Rotation);
			CreateSprite(Bitmap, Position, 0.3f, Light, o, -Rotation);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(19)], p, Position);
			CreateSprite(Bitmap, Position, 0.3f, Light, o, Rotation);
			CreateSprite(Bitmap, Position, 0.3f, Light, o, -Rotation);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(21)], p, Position);
			CreateSprite(Bitmap, Position, 1.5f, Light, o, Rotation);
			CreateSprite(Bitmap, Position, 1.5f, Light, o, -Rotation);
			break;
		case 40:
			Vector(Luminosity*1.0f, Luminosity*1.0f, Luminosity*1.0f, Light);
			Rotation = (mu_float)((mu_int32)(FWorldTime*0.1f) % 360);
			VectorCopy(o->Position, Position);
			Vector(0.0f, 0.0f, 260.0f, p);
			VectorAdd(Position, p, Position);
			CreateSprite(BITMAP_LIGHTNING + 1, Position, 2.5f, Light, o, Rotation);
			CreateSprite(BITMAP_LIGHTNING + 1, Position, 2.5f, Light, o, -Rotation);
			break;
		}
		break;
	case WD_6STADIUM:
		switch (o->Type)
		{
		case 9:
			Scale = Luminosity*5.0f;
			Vector(Luminosity*0.6f, Luminosity*0.3f, Luminosity*0.1f, Light);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(1)], Position, p);
			CreateSprite(BITMAP_LIGHT, p, Scale, Light, o);
			break;
		}
		break;
	case WD_8TARKAN:
		switch (o->Type)
		{
		case 60:
			if (o->HiddenMesh != -2)
			{
				for (mu_int32 i = 0; i < 20; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 6, o->Scale);
				}
			}
			o->HiddenMesh = -2;
			break;

		case 63:
			Luminosity = (mu_float)EMath::Sin((FWorldTime + (o->Angle[2] * 5))*0.002f)*0.3f + 0.7f;

			Scale = Luminosity*1.5f;
			Vector(Luminosity / 1.7f, Luminosity, Luminosity, Light);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(2)], p, Position);
			CreateSprite(BITMAP_IMPACT, Position, Scale, Light, o);
			break;

		case 64:
			Luminosity = (mu_float)EMath::Sin((FWorldTime + (o->Angle[2] * 5))*0.002f)*0.3f + 0.7f;

			Scale = Luminosity*1.5f;
			Vector(Luminosity, Luminosity*0.32f, Luminosity*0.32f, Light);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(2)], p, Position);
			CreateSprite(BITMAP_IMPACT, Position, Scale, Light, o);
			break;

		case 70:
			o->HiddenMesh = -2;
			if (GetLargeRand(5) == 0)
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 7, o->Scale);
			break;

		case 76:
			o->HiddenMesh = -2;
			{
				mu_boolean Smoke = false;

				if ((IWorldTime % 5000) > 4500) Smoke = true;

				if (Smoke)
					CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 4, o->Scale);
			}
			break;
		case 83:
			o->HiddenMesh = -2;
			{
				mu_boolean Smoke = false;
				mu_int32  inter = (mu_int32)o->Angle[2] * 10;
				mu_int32  timing = IWorldTime % 10000;

				if (timing > 3500 + inter && timing < 4000 + inter) Smoke = true;
				if (Smoke)
				{
					Vector(1.0f, 1.0f, 1.0f, Light);

					CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 8, o->Scale);
					if (GetLargeRand(3) == 0)
					{
						Position[0] = o->Position[0] + (GetLargeRand(128) - 64);
						Position[1] = o->Position[1] + (GetLargeRand(128) - 64);
						Position[2] = o->Position[2];

						CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, o->Light, 4, o->Scale*0.5f);
						CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), o->Position, o->Angle, o->Light);
					}
				}
			}
			break;
		}
		break;
	case WD_9DEVILSQUARE:
		switch (o->Type)
		{
		case 2:
			Vector(1.0f, 1.0f, 1.0f, Light);
			Vector(-15.0f, 0.0f, 0.0f, p);
			if (GetLargeRand(4) == 0)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(23)], p, Position);
				CreateParticle(ThreadIndex, BITMAP_RAIN_CIRCLE + 1, Position, o->Angle, Light);
			}
			if (GetLargeRand(4) == 0)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(31)], p, Position);
				CreateParticle(ThreadIndex, BITMAP_RAIN_CIRCLE + 1, Position, o->Angle, Light);
			}
			Vector(-15.0f, 0.0f, 0.0f, p);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(23)], p, Position);
			CreateParticle(ThreadIndex, BITMAP_RAIN_CIRCLE + 1, Position, o->Angle, Light);
			break;
		}
		break;
	case WD_10HEAVEN:
		switch (o->Type)
		{
		case 0:
			if (o->HiddenMesh != -2)
			{
				EVector3  Light;
				Vector(0.1f, 0.1f, 0.1f, Light);
				for (mu_int32 i = 0; i < 20; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 0, o->Scale, o);
				}
			}
			o->HiddenMesh = -2;
			break;

		case 1:
			if (o->HiddenMesh != -2)
			{
				EVector3  Light;
				Vector(0.1f, 0.1f, 0.1f, Light);
				for (mu_int32 i = 0; i < 20; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 1, o->Scale, o);
				}
			}
			o->HiddenMesh = -2;
			break;

		case 2:
			if (o->HiddenMesh != -2)
			{
				EVector3  Light;
				Vector(0.1f, 0.1f, 0.1f, Light);
				for (mu_int32 i = 0; i < 20; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 2, o->Scale, o);
				}
			}
			o->HiddenMesh = -2;
			break;

		case 3:
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
			break;

		case 4:
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
			break;

		case 5:
			if (o->HiddenMesh != -2)
			{
				EVector3  Light;
				Vector(0.1f, 0.1f, 0.1f, Light);
				for (mu_int32 i = 0; i < 10; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 5, o->Scale, o);
				}
			}
			o->HiddenMesh = -2;
			break;

		case 10:
			{
				Vector(0.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(3)], p, Position);

				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_LIGHT, Position, o->Angle, Light, 0, 1.0f);
			}
			break;
		}
		break;

	case WD_11BLOODCASTLE1:
	case WD_11BLOODCASTLE1 + 1:
	case WD_11BLOODCASTLE1 + 2:
	case WD_11BLOODCASTLE1 + 3:
	case WD_11BLOODCASTLE1 + 4:
	case WD_11BLOODCASTLE1 + 5:
	case WD_11BLOODCASTLE1 + 6:
	case WD_52BLOODCASTLE_MASTER_LEVEL:
		switch (o->Type)
		{
		case 11:
			{
				static const mu_uint8 indexLight[7] = { 1, 2, 4, 6, 9, 10, 11 };

				Luminosity = EMath::Sin((o->Angle[2] * 20 + FWorldTime)*0.001f)*0.5f + 0.5f;
				Vector(Luminosity*1.0f, Luminosity*0.5f, 0.0f, Light);

				for (mu_int32 i = 0; i < 7; ++i)
				{
					Vector(0.0f, 0.0f, 2.0f, p);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(indexLight[i])], p, Position);
					CreateSprite(BITMAP_LIGHT, Position, 0.5f, Light, o);
				}
			}
			break;
		case 13:
			Luminosity = EMath::Sin(FWorldTime*0.001f)*0.3f + 0.7f;
			Vector(Luminosity, Luminosity, Luminosity, Light);
			Vector(0.0f, 0.0f, 0.0f, p);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(3)], p, Position);
			CreateSprite(BITMAP_FLARE, Position, Luminosity + 0.5f, Light, o);
			break;
		case 37:
			Vector(1.0f, 1.0f, 1.0f, Light);
			if (GetLargeRand(2) == 0)
			{
				if ((mu_int32)((o->Timer++) + 2) % 4 == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE + 1, o->Position, o->Angle, Light);
					CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE, o->Position, o->Angle, Light, 0);
				}
			}
			if (GetLargeRand(2) == 0)
			{
				if ((mu_int32)(o->Timer++) % 4 == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 6);
					CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE, o->Position, o->Angle, Light, 1);

					Vector(1.0f, 0.8f, 0.8f, Light);
					CreateParticle(ThreadIndex, BITMAP_FLARE, o->Position, o->Angle, Light, 4, 0.19f, nullptr);
				}
			}
			break;
		}
		break;

	case WD_18CHAOS_CASTLE:
	case WD_18CHAOS_CASTLE + 1:
	case WD_18CHAOS_CASTLE + 2:
	case WD_18CHAOS_CASTLE + 3:
	case WD_18CHAOS_CASTLE + 4:
	case WD_18CHAOS_CASTLE + 5:
	case WD_53CAOSCASTLE_MASTER_LEVEL:
		{
			RenderChaosCastleVisual(ThreadIndex, o, b);
		}
		break;

	case WD_24HELLAS:
	case WD_24HELLAS + 1:
	case WD_24HELLAS + 2:
	case WD_24HELLAS + 3:
	case WD_24HELLAS + 4:
	case WD_24HELLAS + 5:
	case WD_24HELLAS_7:
		{
			RenderHellasVisual(ThreadIndex, o, b);
		}
		break;

	case WD_30BATTLECASTLE:
		{
			battleCastle::RenderBattleCastleVisual(ThreadIndex, o, b);
		}
		break;

	case WD_31HUNTING_GROUND:
		{
			M31HuntingGround::RenderHuntingGroundObjectVisual(ThreadIndex, o, b);
		}
		break;

	case WD_33AIDA:
		{
			M33Aida::RenderAidaObjectVisual(ThreadIndex, o, b);
		}
		break;

	case WD_34CRYWOLF_1ST:
		{
			M34CryWolf1st::RenderCryWolf1stObjectVisual(ThreadIndex, o, b);
		}
		break;

	case WD_37KANTURU_1ST:
	case WD_40AREA_FOR_GM:
		{
			M37Kanturu1st::RenderKanturu1stObjectVisual(ThreadIndex, o, b);
		}
		break;

	case WD_38KANTURU_2ND:
		{
			M38Kanturu2nd::Render_Kanturu2nd_ObjectVisual(ThreadIndex, o, b);
		}
		break;

	case WD_39KANTURU_3RD:
		{
			M39Kanturu3rd::RenderKanturu3rdObjectVisual(ThreadIndex, o, b);
		}
		break;

	case WD_41CHANGEUP3RD_1ST:
	case WD_42CHANGEUP3RD_2ND:
		{
			g_3rdChangeUp.RenderObjectVisual(ThreadIndex, o, b);
		}
		break;


	case WD_45CURSEDTEMPLE_LV1:
	case WD_45CURSEDTEMPLE_LV2:
	case WD_45CURSEDTEMPLE_LV3:
	case WD_45CURSEDTEMPLE_LV4:
	case WD_45CURSEDTEMPLE_LV5:
	case WD_45CURSEDTEMPLE_LV6:
		return g_CursedTemple.RenderObjectVisual(ThreadIndex, o, b);

	case WD_51HOME_6TH_CHAR:
		return GMNewTown::RenderObjectVisual(ThreadIndex, o, b);

	case WD_56MAP_SWAMP_OF_QUIET:
		return GMSwampOfQuiet::RenderObjectVisual(ThreadIndex, o, b);

	case WD_57ICECITY:
	case WD_58ICECITY_BOSS:
		return g_Raklion.RenderObjectVisual(ThreadIndex, o, b);

	case WD_62SANTA_TOWN:
		return g_SantaTown.RenderObjectVisual(o, b);

	case WD_63PK_FIELD:
		return g_PKField.RenderObjectVisual(ThreadIndex, o, b);

	case WD_64DUELARENA:
		return g_DuelArena.RenderObjectVisual(ThreadIndex, o, b);

	case WD_65DOPPLEGANGER1:
		return g_DoppelGanger1.RenderObjectVisual(ThreadIndex, o, b);

	case WD_66DOPPLEGANGER2:
		return g_DoppelGanger2.RenderObjectVisual(ThreadIndex, o, b);

	case WD_67DOPPLEGANGER3:
		return g_DoppelGanger3.RenderObjectVisual(ThreadIndex, o, b);

	case WD_68DOPPLEGANGER4:
		return g_DoppelGanger4.RenderObjectVisual(ThreadIndex, o, b);

	case WD_69EMPIREGUARDIAN1:
		return g_EmpireGuardian1.RenderObjectVisual(ThreadIndex, o, b);

	case WD_70EMPIREGUARDIAN2:
		return g_EmpireGuardian2.RenderObjectVisual(ThreadIndex, o, b);

	case WD_71EMPIREGUARDIAN3:
		return g_EmpireGuardian3.RenderObjectVisual(ThreadIndex, o, b);

	case WD_72EMPIREGUARDIAN4:
		return g_EmpireGuardian4.RenderObjectVisual(ThreadIndex, o, b);

	case WD_73NEW_LOGIN_SCENE:
	case WD_74NEW_CHARACTER_SCENE:
		return g_EmpireGuardian4.RenderObjectVisual(ThreadIndex, o, b);

	case WD_79UNITEDMARKETPLACE:
		return g_UnitedMarketPlace.RenderObjectVisual(ThreadIndex, o, b);
	}

	return false;
}

mu_boolean MUMapProcess::RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT *o, MUModel *b, mu_int32 ExtraMon)
{
	if (RenderHellasObjectMesh(ThreadIndex, o, b) == true)
		return true;
	if (M31HuntingGround::RenderHuntingGroundMonsterObjectMesh(ThreadIndex, o, b, ExtraMon) == true)
		return true;
	if (M33Aida::RenderAidaMonsterObjectMesh(ThreadIndex, o, b, ExtraMon) == true)
		return true;
	if (M37Kanturu1st::RenderKanturu1stMonsterObjectMesh(ThreadIndex, o, b, ExtraMon) == true)
		return true;
	if (M38Kanturu2nd::Render_Kanturu2nd_MonsterObjectMesh(ThreadIndex, o, b, ExtraMon) == true)
		return true;
	if (M39Kanturu3rd::RenderKanturu3rdMonsterObjectMesh(ThreadIndex, o, b, ExtraMon) == true)
		return true;
	if (GMSwampOfQuiet::RenderObject(ThreadIndex, o, b, ExtraMon) == true)
		return true;
	if (g_EmpireGuardian4.RenderMonster(ThreadIndex, o, b, ExtraMon) == true)
		return true;

	switch (GET_WORLD)
	{
	case WD_30BATTLECASTLE:
		return battleCastle::RenderBattleCastleObjectMesh(ThreadIndex, o, b);

	case WD_31HUNTING_GROUND:
		return M31HuntingGround::RenderHuntingGroundObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_33AIDA:
		return M33Aida::RenderAidaObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_34CRYWOLF_1ST:
		return M34CryWolf1st::RenderCryWolf1stObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_37KANTURU_1ST:
	case WD_40AREA_FOR_GM:
		return M37Kanturu1st::RenderKanturu1stObjectMesh(ThreadIndex, o, b);

	case WD_38KANTURU_2ND:
		return M38Kanturu2nd::Render_Kanturu2nd_ObjectMesh(ThreadIndex, o, b);

	case WD_39KANTURU_3RD:
		return M39Kanturu3rd::RenderKanturu3rdObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_41CHANGEUP3RD_1ST:
	case WD_42CHANGEUP3RD_2ND:
		return g_3rdChangeUp.RenderObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_45CURSEDTEMPLE_LV1:
	case WD_45CURSEDTEMPLE_LV2:
	case WD_45CURSEDTEMPLE_LV3:
	case WD_45CURSEDTEMPLE_LV4:
	case WD_45CURSEDTEMPLE_LV5:
	case WD_45CURSEDTEMPLE_LV6:
		return g_CursedTemple.RenderObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_51HOME_6TH_CHAR:
		return GMNewTown::RenderObject(ThreadIndex, o, b, ExtraMon);

	case WD_57ICECITY:
	case WD_58ICECITY_BOSS:
		return g_Raklion.RenderObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_62SANTA_TOWN:
		return g_SantaTown.RenderObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_63PK_FIELD:
		return g_PKField.RenderObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_64DUELARENA:
		return false;

	case WD_65DOPPLEGANGER1:
		return g_DoppelGanger1.RenderObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_66DOPPLEGANGER2:
		return g_DoppelGanger2.RenderObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_67DOPPLEGANGER3:
		return g_DoppelGanger3.RenderObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_68DOPPLEGANGER4:
		return g_DoppelGanger4.RenderObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_69EMPIREGUARDIAN1:
		return g_EmpireGuardian1.RenderObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_70EMPIREGUARDIAN2:
		return g_EmpireGuardian2.RenderObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_71EMPIREGUARDIAN3:
		return g_EmpireGuardian3.RenderObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_72EMPIREGUARDIAN4:
		return g_EmpireGuardian4.RenderObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_73NEW_LOGIN_SCENE:
	case WD_74NEW_CHARACTER_SCENE:
		return g_EmpireGuardian4.RenderObjectMesh(ThreadIndex, o, b, ExtraMon);

	case WD_79UNITEDMARKETPLACE:
		return g_UnitedMarketPlace.RenderObjectMesh(ThreadIndex, o, b, ExtraMon);
	}

	return false;
}

CHARACTER *MUMapProcess::CreateMonster(const mu_uint32 ThreadIndex, mu_int32 Type, mu_int32 PositionX, mu_int32 PositionY, mu_int32 Key)
{
	CHARACTER *c = nullptr;

	c = CreateHellasMonster(Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = battleCastle::CreateBattleCastleMonster(ThreadIndex, Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = M31HuntingGround::CreateHuntingGroundMonster(Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = M33Aida::CreateAidaMonster(Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = M34CryWolf1st::CreateCryWolf1stMonster(ThreadIndex, Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = M37Kanturu1st::CreateKanturu1stMonster(ThreadIndex, Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = M38Kanturu2nd::Create_Kanturu2nd_Monster(Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = M39Kanturu3rd::CreateKanturu3rdMonster(ThreadIndex, Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = g_3rdChangeUp.CreateBalgasBarrackMonster(Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = g_NewYearsDayEvent.CreateMonster(Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = g_09SummerEvent.CreateMonster(Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = GMNewTown::CreateNewTownMonster(Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = GMSwampOfQuiet::CreateSwampOfQuietMonster(Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = g_Raklion.CreateMonster(Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = g_SantaTown.CreateMonster(Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = g_PKField.CreateMonster(Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = g_DoppelGanger1.CreateMonster(Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = g_EmpireGuardian1.CreateMonster(ThreadIndex, Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = g_EmpireGuardian2.CreateMonster(ThreadIndex, Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = g_EmpireGuardian3.CreateMonster(ThreadIndex, Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = g_EmpireGuardian4.CreateMonster(ThreadIndex, Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	c = g_CursedTemple.CreateCharacters(Type, PositionX, PositionY, Key);
	if (c != nullptr)
		return c;

	return nullptr;
}

mu_boolean MUMapProcess::MoveMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER *c, OBJECT* o, MUModel* b)
{
	if (MoveHellasMonsterVisual(ThreadIndex, o, b))
		return true;
	if (M31HuntingGround::MoveHuntingGroundMonsterVisual(ThreadIndex, o, b))
		return true;
	if (M33Aida::MoveAidaMonsterVisual(ThreadIndex, o, b))
		return true;
	if (M34CryWolf1st::MoveCryWolf1stMonsterVisual(ThreadIndex, c, o, b))
		return true;
	if (M37Kanturu1st::MoveKanturu1stMonsterVisual(c, o, b))
		return true;
	if (M38Kanturu2nd::Move_Kanturu2nd_MonsterVisual(ThreadIndex, c, o, b))
		return true;
	if (M39Kanturu3rd::MoveKanturu3rdMonsterVisual(ThreadIndex, o, b))
		return true;
	if (g_NewYearsDayEvent.MoveMonsterVisual(ThreadIndex, c, o, b) == true)
		return true;
	if (g_09SummerEvent.MoveMonsterVisual(ThreadIndex, c, o, b) == true)
		return true;

	switch (GET_WORLD)
	{
	case WD_30BATTLECASTLE:
		return battleCastle::MoveBattleCastleMonsterVisual(o, b);

	case WD_41CHANGEUP3RD_1ST:
	case WD_42CHANGEUP3RD_2ND:
		return g_3rdChangeUp.MoveBalgasBarrackMonsterVisual(ThreadIndex, c, o, b);

	case WD_45CURSEDTEMPLE_LV1:
	case WD_45CURSEDTEMPLE_LV2:
	case WD_45CURSEDTEMPLE_LV3:
	case WD_45CURSEDTEMPLE_LV4:
	case WD_45CURSEDTEMPLE_LV5:
	case WD_45CURSEDTEMPLE_LV6:
		return g_CursedTemple.MoveMonsterVisual(o, b);

	case WD_56MAP_SWAMP_OF_QUIET:
		return GMSwampOfQuiet::MoveMonsterVisual(ThreadIndex, o, b);

	case WD_57ICECITY:
	case WD_58ICECITY_BOSS:
		return g_Raklion.MoveMonsterVisual(ThreadIndex, o, b);

	case WD_62SANTA_TOWN:
		return false;

	case WD_63PK_FIELD:
		return g_PKField.MoveMonsterVisual(ThreadIndex, o, b);

	case WD_64DUELARENA:
		return false;

	case WD_65DOPPLEGANGER1:
		return g_DoppelGanger1.MoveMonsterVisual(ThreadIndex, o, b);

	case WD_66DOPPLEGANGER2:
		return g_DoppelGanger2.MoveMonsterVisual(ThreadIndex, o, b);

	case WD_67DOPPLEGANGER3:
		return g_DoppelGanger3.MoveMonsterVisual(ThreadIndex, o, b);

	case WD_68DOPPLEGANGER4:
		return g_DoppelGanger4.MoveMonsterVisual(ThreadIndex, o, b);

	case WD_69EMPIREGUARDIAN1:
		return g_EmpireGuardian1.MoveMonsterVisual(ThreadIndex, c, o, b);

	case WD_70EMPIREGUARDIAN2:
		return g_EmpireGuardian2.MoveMonsterVisual(ThreadIndex, c, o, b);

	case WD_71EMPIREGUARDIAN3:
		return g_EmpireGuardian3.MoveMonsterVisual(ThreadIndex, c, o, b);

	case WD_72EMPIREGUARDIAN4:
		return g_EmpireGuardian4.MoveMonsterVisual(ThreadIndex, c, o, b);

	case WD_79UNITEDMARKETPLACE:
		return false;
	}

	return false;
}

void MUMapProcess::MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	M31HuntingGround::MoveHuntingGroundBlurEffect(ThreadIndex, c, o, b);
	M33Aida::MoveAidaBlurEffect(ThreadIndex, c, o, b);
	M34CryWolf1st::MoveCryWolf1stBlurEffect(ThreadIndex, c, o, b);
	M37Kanturu1st::MoveKanturu1stBlurEffect(ThreadIndex, c, o, b);
	M38Kanturu2nd::Move_Kanturu2nd_BlurEffect(ThreadIndex, c, o, b);
	M39Kanturu3rd::MoveKanturu3rdBlurEffect(ThreadIndex, c, o, b);
	GMNewTown::MoveBlurEffect(ThreadIndex, c, o, b);
	GMSwampOfQuiet::MoveBlurEffect(ThreadIndex, c, o, b);

	switch (GET_WORLD)
	{
	case WD_41CHANGEUP3RD_1ST:
	case WD_42CHANGEUP3RD_2ND:
		g_3rdChangeUp.MoveBalgasBarrackBlurEffect(ThreadIndex, c, o, b);
		break;

	case WD_45CURSEDTEMPLE_LV1:
	case WD_45CURSEDTEMPLE_LV2:
	case WD_45CURSEDTEMPLE_LV3:
	case WD_45CURSEDTEMPLE_LV4:
	case WD_45CURSEDTEMPLE_LV5:
	case WD_45CURSEDTEMPLE_LV6:
		g_CursedTemple.MoveBlurEffect(ThreadIndex, c, o, b);
		break;

	case WD_57ICECITY:
	case WD_58ICECITY_BOSS:
		g_Raklion.MoveBlurEffect(ThreadIndex, c, o, b);
		break;

	case WD_62SANTA_TOWN:
		return;

	case WD_63PK_FIELD:
		g_PKField.MoveBlurEffect(ThreadIndex, c, o, b);
		break;

	case WD_64DUELARENA:
		return;

	case WD_65DOPPLEGANGER1:
		g_DoppelGanger1.MoveBlurEffect(ThreadIndex, c, o, b);
		break;

	case WD_66DOPPLEGANGER2:
		g_DoppelGanger2.MoveBlurEffect(ThreadIndex, c, o, b);
		break;

	case WD_67DOPPLEGANGER3:
		g_DoppelGanger3.MoveBlurEffect(ThreadIndex, c, o, b);
		break;

	case WD_68DOPPLEGANGER4:
		g_DoppelGanger4.MoveBlurEffect(ThreadIndex, c, o, b);
		break;

	case WD_69EMPIREGUARDIAN1:
		g_EmpireGuardian1.MoveBlurEffect(ThreadIndex, c, o, b);
		break;

	case WD_70EMPIREGUARDIAN2:
		g_EmpireGuardian2.MoveBlurEffect(ThreadIndex, c, o, b);
		break;

	case WD_71EMPIREGUARDIAN3:
		g_EmpireGuardian3.MoveBlurEffect(ThreadIndex, c, o, b);
		break;

	case WD_72EMPIREGUARDIAN4:
		g_EmpireGuardian4.MoveBlurEffect(ThreadIndex, c, o, b);
		break;

	case WD_79UNITEDMARKETPLACE:
		return;
	}
}

mu_boolean MUMapProcess::RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (RenderHellasMonsterVisual(ThreadIndex, c, o, b))
		return true;
	if (M31HuntingGround::RenderHuntingGroundMonsterVisual(ThreadIndex, c, o, b))
		return true;
	if (M33Aida::RenderAidaMonsterVisual(ThreadIndex, c, o, b))
		return true;
	if (M34CryWolf1st::RenderCryWolf1stMonsterVisual(ThreadIndex, c, o, b))
		return true;
	if (M37Kanturu1st::RenderKanturu1stMonsterVisual(ThreadIndex, c, o, b))
		return true;
	if (M38Kanturu2nd::Render_Kanturu2nd_MonsterVisual(ThreadIndex, c, o, b))
		return true;
	if (M39Kanturu3rd::RenderKanturu3rdMonsterVisual(ThreadIndex, c, o, b))
		return true;
	if (GMNewTown::RenderMonsterVisual(ThreadIndex, c, o, b))
		return true;

	switch (GET_WORLD)
	{
	case WD_30BATTLECASTLE:
		return battleCastle::RenderBattleCastleMonsterVisual(ThreadIndex, c, o, b);

	case WD_41CHANGEUP3RD_1ST:
	case WD_42CHANGEUP3RD_2ND:
		return g_3rdChangeUp.RenderBalgasBarrackMonsterVisual(ThreadIndex, c, o, b);

	case WD_45CURSEDTEMPLE_LV1:
	case WD_45CURSEDTEMPLE_LV2:
	case WD_45CURSEDTEMPLE_LV3:
	case WD_45CURSEDTEMPLE_LV4:
	case WD_45CURSEDTEMPLE_LV5:
	case WD_45CURSEDTEMPLE_LV6:
		return g_CursedTemple.RenderMonsterVisual(ThreadIndex, c, o, b);

	case WD_56MAP_SWAMP_OF_QUIET:
		return GMSwampOfQuiet::RenderMonsterVisual(ThreadIndex, c, o, b);

	case WD_57ICECITY:
	case WD_58ICECITY_BOSS:
		return g_Raklion.RenderMonsterVisual(ThreadIndex, c, o, b);

	case WD_62SANTA_TOWN:
		return false;

	case WD_63PK_FIELD:
		return g_PKField.RenderMonsterVisual(ThreadIndex, c, o, b);

	case WD_64DUELARENA:
		return false;

	case WD_65DOPPLEGANGER1:
		return g_DoppelGanger1.RenderMonsterVisual(ThreadIndex, c, o, b);

	case WD_66DOPPLEGANGER2:
		return g_DoppelGanger2.RenderMonsterVisual(ThreadIndex, c, o, b);

	case WD_67DOPPLEGANGER3:
		return g_DoppelGanger3.RenderMonsterVisual(ThreadIndex, c, o, b);

	case WD_68DOPPLEGANGER4:
		return g_DoppelGanger4.RenderMonsterVisual(ThreadIndex, c, o, b);

	case WD_69EMPIREGUARDIAN1:
		return g_EmpireGuardian1.RenderMonsterVisual(ThreadIndex, c, o, b);

	case WD_70EMPIREGUARDIAN2:
		return g_EmpireGuardian2.RenderMonsterVisual(ThreadIndex, c, o, b);

	case WD_71EMPIREGUARDIAN3:
		return g_EmpireGuardian3.RenderMonsterVisual(ThreadIndex, c, o, b);

	case WD_72EMPIREGUARDIAN4:
		return g_EmpireGuardian4.RenderMonsterVisual(ThreadIndex, c, o, b);

	case WD_79UNITEDMARKETPLACE:
		return g_UnitedMarketPlace.RenderMonsterVisual(ThreadIndex, c, o, b);
	}

	return false;
}

mu_boolean MUMapProcess::AttackEffectMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (M34CryWolf1st::AttackEffectCryWolf1stMonster(ThreadIndex, c, o, b) == true)
		return true;
	if (M38Kanturu2nd::AttackEffect_Kanturu2nd_Monster(ThreadIndex, c, o, b) == true)
		return true;

	switch (GET_WORLD)
	{
	case WD_24HELLAS + 0:
	case WD_24HELLAS + 1:
	case WD_24HELLAS + 2:
	case WD_24HELLAS + 3:
	case WD_24HELLAS + 4:
	case WD_24HELLAS + 5:
	case WD_24HELLAS_7:
		{
			CHARACTER *tc = nullptr;
			OBJECT *to = nullptr;

			if (c->TargetCharacter >= 0 && c->TargetCharacter < MAX_CHARACTERS_CLIENT)
			{
				tc = &CharactersClient[c->TargetCharacter];
				to = &tc->Object;
			}

			return AttackEffect_HellasMonster(ThreadIndex, c, tc, o, to, b);
		}
		
	case WD_30BATTLECASTLE:
		return battleCastle::AttackEffect_BattleCastleMonster(ThreadIndex, c, o, b);

	case WD_31HUNTING_GROUND:
		return M31HuntingGround::AttackEffectHuntingGroundMonster(ThreadIndex, c, o, b);

	case WD_33AIDA:
		return M33Aida::AttackEffectAidaMonster(ThreadIndex, c, o, b);

	case WD_37KANTURU_1ST:
		return M37Kanturu1st::AttackEffectKanturu1stMonster(ThreadIndex, c, o, b);

	case WD_39KANTURU_3RD:
		return M39Kanturu3rd::AttackEffectKanturu3rdMonster(ThreadIndex, c, o, b);

	case WD_41CHANGEUP3RD_1ST:
	case WD_42CHANGEUP3RD_2ND:
		return g_3rdChangeUp.AttackEffectBalgasBarrackMonster(ThreadIndex, c, o, b);

	case WD_45CURSEDTEMPLE_LV1:
	case WD_45CURSEDTEMPLE_LV2:
	case WD_45CURSEDTEMPLE_LV3:
	case WD_45CURSEDTEMPLE_LV4:
	case WD_45CURSEDTEMPLE_LV5:
	case WD_45CURSEDTEMPLE_LV6:
		return g_CursedTemple.AttackEffectMonster(ThreadIndex, c, o, b);

	case WD_57ICECITY:
	case WD_58ICECITY_BOSS:
		return g_Raklion.AttackEffectMonster(ThreadIndex, c, o, b);

	case WD_62SANTA_TOWN:
		return false;

	case WD_63PK_FIELD:
		return false;

	case WD_64DUELARENA:
		return false;

	case WD_65DOPPLEGANGER1:
		return false;

	case WD_66DOPPLEGANGER2:
		return false;

	case WD_67DOPPLEGANGER3:
		return false;

	case WD_68DOPPLEGANGER4:
		return false;

	case WD_69EMPIREGUARDIAN1:
		return false;

	case WD_70EMPIREGUARDIAN2:
		return g_EmpireGuardian2.AttackEffectMonster(ThreadIndex, c, o, b);

	case WD_71EMPIREGUARDIAN3:
		return false;

	case WD_72EMPIREGUARDIAN4:
		return false;

	case WD_79UNITEDMARKETPLACE:
		return false;
	}

	return false;
}

mu_boolean MUMapProcess::SetCurrentActionMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o)
{
	if (SetCurrentAction_HellasMonster(c, o) == true)
		return true;
	if (battleCastle::SetCurrentAction_BattleCastleMonster(c, o) == true)
		return true;
	if (M34CryWolf1st::SetCurrentActionCrywolfMonster(c, o) == true)
		return true;
	if (M38Kanturu2nd::Set_CurrentAction_Kanturu2nd_Monster(c, o) == true)
		return true;
	if (M39Kanturu3rd::SetCurrentActionKanturu3rdMonster(c, o) == true)
		return true;

	switch (GET_WORLD)
	{
	case WD_31HUNTING_GROUND:
		return M31HuntingGround::SetCurrentActionHuntingGroundMonster(c, o);

	case WD_33AIDA:
		return M33Aida::SetCurrentActionAidaMonster(c, o);

	case WD_37KANTURU_1ST:
		return M37Kanturu1st::SetCurrentActionKanturu1stMonster(c, o);

	case WD_41CHANGEUP3RD_1ST:
	case WD_42CHANGEUP3RD_2ND:
		return g_3rdChangeUp.SetCurrentActionBalgasBarrackMonster(c, o);

	case WD_45CURSEDTEMPLE_LV1:
	case WD_45CURSEDTEMPLE_LV2:
	case WD_45CURSEDTEMPLE_LV3:
	case WD_45CURSEDTEMPLE_LV4:
	case WD_45CURSEDTEMPLE_LV5:
	case WD_45CURSEDTEMPLE_LV6:
		return g_CursedTemple.SetCurrentActionMonster(c, o);

	case WD_56MAP_SWAMP_OF_QUIET:
		return GMSwampOfQuiet::SetCurrentActionMonster(c, o);

	case WD_57ICECITY:
	case WD_58ICECITY_BOSS:
		return g_Raklion.SetCurrentActionMonster(ThreadIndex, c, o);

	case WD_62SANTA_TOWN:
		return false;

	case WD_63PK_FIELD:
		return false;

	case WD_64DUELARENA:
		return false;

	case WD_65DOPPLEGANGER1:
		return false;

	case WD_66DOPPLEGANGER2:
		return false;

	case WD_67DOPPLEGANGER3:
		return false;

	case WD_68DOPPLEGANGER4:
		return false;

	case WD_69EMPIREGUARDIAN1:
		return g_EmpireGuardian1.SetCurrentActionMonster(c, o);

	case WD_70EMPIREGUARDIAN2:
		return g_EmpireGuardian2.SetCurrentActionMonster(c, o);

	case WD_71EMPIREGUARDIAN3:
		return g_EmpireGuardian3.SetCurrentActionMonster(c, o);

	case WD_72EMPIREGUARDIAN4:
		return g_EmpireGuardian4.SetCurrentActionMonster(c, o);
	}

	return false;
}

mu_boolean MUMapProcess::PlayMonsterSound(const mu_uint32 ThreadIndex, OBJECT* o)
{
	switch (GET_WORLD)
	{
	case WD_51HOME_6TH_CHAR:
		return GMNewTown::PlayMonsterSound(o);

	case WD_56MAP_SWAMP_OF_QUIET:
		return GMSwampOfQuiet::PlayMonsterSound(o);

	case WD_57ICECITY:
	case WD_58ICECITY_BOSS:
		return g_Raklion.PlayMonsterSound(o);

	case WD_62SANTA_TOWN:
		return false;

	case WD_63PK_FIELD:
		return g_PKField.PlayMonsterSound(o);

	case WD_64DUELARENA:
		return false;

	case WD_65DOPPLEGANGER1:
		return g_DoppelGanger1.PlayMonsterSound(o);

	case WD_66DOPPLEGANGER2:
		return g_DoppelGanger1.PlayMonsterSound(o);

	case WD_67DOPPLEGANGER3:
		return g_DoppelGanger1.PlayMonsterSound(o);

	case WD_68DOPPLEGANGER4:
		return g_DoppelGanger1.PlayMonsterSound(o);

	case WD_69EMPIREGUARDIAN1:
		return g_EmpireGuardian1.PlayMonsterSound(o);

	case WD_70EMPIREGUARDIAN2:
		return g_EmpireGuardian2.PlayMonsterSound(o);

	case WD_71EMPIREGUARDIAN3:
		return g_EmpireGuardian3.PlayMonsterSound(o);

	case WD_72EMPIREGUARDIAN4:
		return g_EmpireGuardian4.PlayMonsterSound(o);

	case WD_79UNITEDMARKETPLACE:
		return false;
	}

	return false;
}
