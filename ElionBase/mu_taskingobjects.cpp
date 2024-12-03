#include "stdafx.h"
#include "mu_tasking.h"

mu_int32 Time_Effect = 0;
mu_int32 Change_weather = 0;

void MoveHeavenThunder(const mu_uint32 ThreadIndex)
{
	mu_float   Luminosity;
	EVector3  Light;

	if ((GetLargeRand(50)) == 0)
	{
		EVector3  position, Angle;

		position[0] = Hero->Object.Position[0] + GetLargeRand(300) - 150.0f;
		position[1] = Hero->Object.Position[1];
		position[2] = Hero->Object.Position[2] + GetLargeRand(300) - 150.0f;

		Luminosity = (mu_float)(GetLargeRand(4) + 4)*0.05f;
		Vector(Luminosity*0.3f, Luminosity*0.3f, Luminosity*0.081f, Light);
		AddTerrainLight(position[0], position[1], Light, 2, PrimaryTerrainLight);
		Vector(0.0f, 0.0f, 0.0f, Angle);
		CreateEffect(ThreadIndex, MODEL_CLOUD, Hero->Object.Position, Angle, Light);

		if ((GetLargeRand(5)) == 0)
		{
			EMatrix4 Matrix1, Matrix2;
			EVector3  pos, position, position2, angle;

			Vector(0.0f, 0.0f, -45.0f, angle);
			Matrix1 = EMath::EulerToMatrix(angle);

			switch (GetLargeRand(4))
			{
			case 0:
				Vector(-400.0f, -1000.0f, 0.0f, position);
				position = EMath::TransformNormal(Matrix1, position);
				VectorAdd(Hero->Object.Position, position, pos);

				Vector(0.0f, 0.0f, 240.0f, angle);
				Matrix2 = EMath::EulerToMatrix(angle);
				Vector(-200.0f, -1000.0f, 0.0f, position);
				break;

			case 1:
				Vector(-300.0f, -400.0f, 0.0f, position);
				position = EMath::TransformNormal(Matrix1, position);
				VectorSubtract(Hero->Object.Position, position, pos);

				Vector(0.0f, 0.0f, 210.0f, angle);
				Matrix2 = EMath::EulerToMatrix(angle);
				Vector(-500.0f, -1000.0f, 0.0f, position);
				break;

			case 2:
				Vector(-200.0f, -400.0f, 0.0f, position);
				position = EMath::TransformNormal(Matrix1, position);
				VectorAdd(Hero->Object.Position, position, pos);

				Vector(0.0f, 0.0f, 235.0f, angle);
				Matrix2 = EMath::EulerToMatrix(angle);
				Vector(-1000.0f, -1500.0f, 0.0f, position);
				break;

			case 3:
				Vector(-200.0f, 400.0f, 0.0f, position);
				position = EMath::TransformNormal(Matrix1, position);
				VectorAdd(Hero->Object.Position, position, pos);

				Vector(0.0f, 0.0f, 200.0f, angle);
				Matrix2 = EMath::EulerToMatrix(angle);
				Vector(-600.0f, -1200.0f, 0.0f, position);
				break;
			}

			position = EMath::TransformNormal(Matrix2, position);
			VectorSubtract(pos, position, position2);
			VectorAdd(pos, position, position);
			position[2] -= 300.0f;
			position2[2] -= 300.0f;
			angle[2] = 0;
			CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, position, position2, angle, 0, nullptr, 40.0f + GetLargeRand(10));
			CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, position, position2, angle, 0, nullptr, 40.0f + GetLargeRand(10));
		}
	}
}

void MoveObject(const mu_uint32 ThreadIndex, mu_uint32 objectIndex, OBJECT *o)
{
	if (GET_WORLD == WD_9DEVILSQUARE)
	{
		if (IWorldTime % 4000 < 1000)
		{
			if (GetLargeRand(100) == 0)
			{
				mu_float Luminosity = (mu_float)(GetLargeRand(12) + 4)*0.1f;
				EVector3 Light;
				Vector(Luminosity*0.2f, Luminosity*0.3f, Luminosity*0.5f, Light);
				AddTerrainLight(Hero->Object.Position[0] + GetLargeRand(1200) - 600,
					Hero->Object.Position[1] + GetLargeRand(1200) - 600,
					Light, 12, PrimaryTerrainLight);
			}
		}
		PlayBackground(SOUND_RAIN01, true);
	}

	if (GET_WORLD == WD_0LORENCIA)
	{
		if (o->Type == MODEL_HOUSE_WALL01 + 4 ||
			o->Type == MODEL_HOUSE_WALL01 + 5)
		{
			if (HeroTile == 4)
				o->AlphaTarget = 0.0f;
			else
				o->AlphaTarget = 1.0f;
		}
	}

	if (GET_WORLD == WD_2DEVIAS)
	{
		if (o->Type == 81 ||
			o->Type == 82 ||
			o->Type == 96 ||
			o->Type == 98 ||
			o->Type == 99)
		{
			if (HeroTile == 3 || HeroTile >= 10)
				o->AlphaTarget = 0.0f;
			else
				o->AlphaTarget = 1.0f;
		}
	}

	Alpha(o);

	if (o->Alpha < 0.01f)
	{
		return;
	}

	MUModel *b = &MODELS::Data[o->Type];
	b->CurrentAction(ThreadIndex) = o->CurrentAction;

	mu_float fSpeed = o->Velocity;
	if (GET_WORLD == WD_8TARKAN)
	{
		switch (o->Type)
		{
		case 8:
			fSpeed *= 4.0f;
			break;
		}
	}
	else if (IsIceCity() == true)
	{
		switch (o->Type)
		{
		case 16:
		case 17:
		case 68:
			fSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
			break;
		}
	}
	else if (IsEmpireGuardian())
	{
		switch (o->Type)
		{
		case 20:
			{
				fSpeed *= 2.0f;
			}
			break;

		case 122:
		case 123:
		case 124:
			{
				fSpeed *= 3.0f;
			}
			break;

		case 128:
			{
				fSpeed *= 6.0f;
			}
			break;

		case 10:
			{
				if (IsEmpireGuardian4())
				{
					if (o->SubType > 0)
					{
						o->SubType -= 1;
						o->AnimationFrame = 0.0f;
						o->PriorAnimationFrame = 0.0f;
					}
				}
			}
			break;
		}
	}

	b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, fSpeed);

	TheMapProcess().MoveObject(ThreadIndex, o);
}

void MUTasking::MoveObjects(const mu_uint32 ThreadIndex)
{
	switch (GET_WORLD)
	{
	case WD_18CHAOS_CASTLE + 0:
	case WD_18CHAOS_CASTLE + 1:
	case WD_18CHAOS_CASTLE + 2:
	case WD_18CHAOS_CASTLE + 3:
	case WD_18CHAOS_CASTLE + 4:
	case WD_18CHAOS_CASTLE + 5:
	case WD_53CAOSCASTLE_MASTER_LEVEL:
		{
			if (ThreadIndex == 0)
			{
				if (!Hero->SafeZone && (GetLargeRand(10)) == 0)
				{
					EVector3 Position;

					Position[0] = Hero->Object.Position[0] + GetLargeRand(800) - 400.0f;
					Position[1] = Hero->Object.Position[1] + GetLargeRand(800) - 400.0f;
					Position[2] = Hero->Object.Position[2] - 150.0f;

					CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT2, Position, Position, Hero->Object.Angle, 9, nullptr, GetLargeRand(10) + 50.0f);
				}
			}
		}
		break;

	case WD_24HELLAS + 0:
	case WD_24HELLAS + 1:
	case WD_24HELLAS + 2:
	case WD_24HELLAS + 3:
	case WD_24HELLAS + 4:
	case WD_24HELLAS + 5:
	case WD_24HELLAS_7:
		{
			if (ThreadIndex == 0)
			{
				PlayBackground(SOUND_KALIMA_AMBIENT);
				if ((MoveSceneFrame % 100) == 0)
				{
					PlayBackground(SOUND_KALIMA_AMBIENT2 + GetLargeRand(2));
				}

				if (GetHellasLevel(Hero->Class, CharacterAttribute->Level) == KUNDUN_ZONE)
				{
					mu_int32 CurrX = (Hero->PositionX);
					mu_int32 CurrY = (Hero->PositionY);

					if ((CurrX >= 25 && CurrY >= 44) && (CurrX <= 51 && CurrY <= 119) && (MoveSceneFrame % 50) == 0)
					{
						PlayBackground(SOUND_KUNDUN_AMBIENT1 + GetLargeRand(2));
					}
				}

				if (GetLargeRand(5) == 0)
				{
					EVector3 Position, Light;

					Vector(0.3f, 0.8f, 1.0f, Light);

					Position[0] = Hero->Object.Position[0] + GetLargeRand(800) - 400.0f;
					Position[1] = Hero->Object.Position[1] + GetLargeRand(800) - 400.0f;
					Position[2] = Hero->Object.Position[2] + 50.0f;

					CreateParticle(ThreadIndex, BITMAP_LIGHT, Position, Hero->Object.Angle, Light, 7, 1.0f, &Hero->Object);

					if (GetLargeRand(15) == 0)
					{
						EVector3 Angle = { 0.0f, 0.0f, 0.0f };
						Position[2] = Hero->Object.Position[2] + 800.0f;
						CreateEffect(ThreadIndex, 9, Position, Angle, Light);

						PlayBackground(SOUND_KALIMA_FALLING_STONE);
					}
				}
			}
		}
		break;

	case WD_30BATTLECASTLE:
		{
			if (ThreadIndex == 0)
			{
				mu_uint64 current = g_CurrentTime;
				mu_double dif = (mu_double)(current - battleCastle::g_iMp3PlayTime) / CLOCKS_PER_SEC;
				if (dif > 1800 || battleCastle::g_iMp3PlayTime == 0)
				{
					battleCastle::g_iMp3PlayTime = current;
					if (battleCastle::IsBattleCastleStart())
					{
						EMusicBackend::Stop(MUSICS::Data[MUSIC_CASTLE_PEACE]);
						EMusicBackend::Play(MUSICS::Data[MUSIC_CASTLE_BATTLE_START]);
					}
					else
					{
						EMusicBackend::Stop(MUSICS::Data[MUSIC_CASTLE_BATTLE_START]);
						EMusicBackend::Play(MUSICS::Data[MUSIC_CASTLE_PEACE]);
					}
				}

				PlayBackground(SOUND_BC_AMBIENT);
				if (battleCastle::IsBattleCastleStart() && GetLargeRand(10) == 0)
				{
					if (GetLargeRand(100) == 0)
					{
						PlayBackground(SOUND_BC_AMBIENT_BATTLE1);
					}
					if (GetLargeRand(100) == 0)
					{
						PlayBackground(SOUND_BC_AMBIENT_BATTLE2);
					}
					if (GetLargeRand(100) == 0)
					{
						PlayBackground(SOUND_BC_AMBIENT_BATTLE3);
					}
					if (GetLargeRand(100) == 0)
					{
						PlayBackground(SOUND_BC_AMBIENT_BATTLE4);
					}
					if (GetLargeRand(100) == 0)
					{
						PlayBackground(SOUND_BC_AMBIENT_BATTLE5);
					}
				}

				if (battleCastle::IsBattleCastleStart())
				{
					if ((MoveSceneFrame % 20) == 0)
					{
						mu_int32 HeroY = (Hero->PositionY);
						if (GetLargeRand(3))
						{
							if (HeroY > 50 && HeroY < 131)
							{
								mu_int32 dx = GetLargeRand(1000) - 500;
								EVector3 Position;
								EVector3 Angle = { 0.0f, 0.0f, 0.0f };
								EVector3 Light = { 1.0f, 1.0f, 1.0f };

								if (dx < 30 && dx>0) dx = 100 + GetLargeRand(30);
								else if (dx > -30 && dx < 0) dx -= 100 + GetLargeRand(30);

								Position[0] = Hero->Object.Position[0] + dx;
								Position[2] = 500.0f;

								mu_int32 SiegePositionY;
								if (HeroY > 55)
								{
									SiegePositionY = 45;
								}
								else
								{
									SiegePositionY = HeroY - (GetLargeRand(10) + 10);
								}
								Position[1] = SiegePositionY * g_TerrainScale + (GetLargeRand(40) - 20);
								CreateEffect(ThreadIndex, MODEL_FLY_BIG_STONE1, Position, Angle, Light, 0);
							}
						}
						else
						{
							if (HeroY > 70 && HeroY < 177)
							{
								mu_int32 dx = GetLargeRand(1000) - 500;
								EVector3 Position;
								EVector3 Angle = { 0.0f, 0.0f, 0.0f };
								EVector3 Light = { 1.0f, 1.0f, 1.0f };

								if (dx < 100 && dx>0) dx = 100 + GetLargeRand(30);
								else if (dx > -100 && dx < 0) dx -= 100 + GetLargeRand(30);

								Position[0] = Hero->Object.Position[0] + dx;
								Position[2] = 500.0f;

								mu_int32 SiegePositionY;
								if (HeroY < 179)
								{
									SiegePositionY = 179;
								}
								else
								{
									SiegePositionY = HeroY - (GetLargeRand(10) + 10);
								}
								Position[1] = SiegePositionY * g_TerrainScale - (GetLargeRand(40) - 20);
								CreateEffect(ThreadIndex, MODEL_FLY_BIG_STONE2, Position, Angle, Light, 0);
							}
						}
					}

					mu_int32 HeroY = (Hero->PositionY);
					if ((HeroY > 58 && HeroY < 113) || (HeroY > 117 && HeroY < 159))
					{
						if ((MoveSceneFrame % 60) == 0)
						{
							mu_int32 length = (GetLargeRand(4) + 2) / 2;
							mu_int32 dx = (GetLargeRand(1400) - 700) - 60.0f*length;

							if (dx < 100 && dx>0) dx = 100 + GetLargeRand(30);
							else if (dx > -100 && dx < 0) dx -= 100 + GetLargeRand(30);

							EVector3 Position;
							EVector3 Angle = { 0.0f, 0.0f, 0.0f };
							EVector3 Light = { 1.0f, 1.0f, 1.0f };
							for (mu_int32 i = -length; i < length; ++i)
							{
								dx += 60.0f;

								Position[0] = Hero->Object.Position[0] + dx;
								Position[2] = 350.0f;

								mu_boolean attArrow = GetLargeRand(2);
								if (HeroY < 113)
								{
									attArrow = true;
								}
								else if (HeroY > 117)
								{
									attArrow = false;
								}

								if (attArrow)
								{
									mu_int32 SiegePositionY = HeroY - (GetLargeRand(1) + 10);
									Position[1] = SiegePositionY * g_TerrainScale;

									Angle[2] = 180.0f;
									CreateEffect(ThreadIndex, MODEL_ARROW, Position, Angle, Light, 4);
								}
								else
								{
									mu_int32 SiegePositionY = HeroY + (GetLargeRand(1) + 10);
									Position[1] = SiegePositionY * g_TerrainScale;

									CreateEffect(ThreadIndex, MODEL_ARROW, Position, Angle, Light, 4);
								}
							}
						}
					}

					if ((MoveSceneFrame % 10) == 0)
					{
						if (HeroY > 58 && HeroY < 159)
						{
							EVector3 Position;
							EVector3 Angle = { 0.0f, 0.0f, 0.0f };
							EVector3 Light = { 1.0f, 1.0f, 1.0f };

							mu_int32 dx = GetLargeRand(1400) - 700;
							if (dx < 100 && dx>0) dx = 100 + GetLargeRand(30);
							else if (dx > -100 && dx < 0) dx -= 100 + GetLargeRand(30);

							Position[0] = Hero->Object.Position[0] + dx;
							Position[2] = 350.0f;

							mu_uint8 subtype = 3;
							if (HeroY < 82 || HeroY>112 || GetLargeRand(10) == 0)
							{
								subtype = 4;
								if (GetLargeRand(10) == 0)
								{
									subtype = 3;
								}
							}

							mu_boolean attArrow = GetLargeRand(2);
							if (HeroY < 82)
							{
								attArrow = true;
							}
							else if (HeroY > 112)
							{
								attArrow = false;
							}

							if (attArrow)
							{
								mu_int32 SiegePositionY = HeroY - (GetLargeRand(3) + 10);
								Position[1] = SiegePositionY * g_TerrainScale + (GetLargeRand(60) - 30);

								Angle[2] = 180.0f;
								CreateEffect(ThreadIndex, MODEL_ARROW, Position, Angle, Light, subtype);
							}
							else
							{
								mu_int32 SiegePositionY = HeroY + (GetLargeRand(3) + 10);
								Position[1] = SiegePositionY * g_TerrainScale - (GetLargeRand(60) - 30);

								CreateEffect(ThreadIndex, MODEL_ARROW, Position, Angle, Light, subtype);
							}
						}
					}
				}
			}
		}
		break;

	case WD_10HEAVEN:
		{
			if (ThreadIndex == 0)
			{
				MoveHeavenThunder(ThreadIndex);

				if (0 == (GetLargeRand(10)))
				{
					EVector3 Position;
					Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(5000) - 2500),
						Hero->Object.Position[1] + (mu_float)(GetLargeRand(5000) - 2500),
						Hero->Object.Position[2] - 1000.0f, Position);
					EVector3 Light = { 1.0f, 1.0f, 1.0f };
					EVector3 Angle = { 0.0f, 0.0f, 0.0f };
					CreateEffect(ThreadIndex, BITMAP_LIGHT, Position, Angle, Light);
				}
			}
		}
		break;

	case WD_11BLOODCASTLE1 + 0:
	case WD_11BLOODCASTLE1 + 1:
	case WD_11BLOODCASTLE1 + 2:
	case WD_11BLOODCASTLE1 + 3:
	case WD_11BLOODCASTLE1 + 4:
	case WD_11BLOODCASTLE1 + 5:
	case WD_11BLOODCASTLE1 + 6:
	case WD_52BLOODCASTLE_MASTER_LEVEL:
		{
			if (ThreadIndex == 0)
			{
				EVector3 Position, Angle;
				EVector3 Light;

				Vector(0.0f, 0.0f, 0.0f, Angle);
				Vector(1.0f, 1.0f, 1.0f, Light);

				Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(900) - 300),
					Hero->Object.Position[1] + (mu_float)(GetLargeRand(900) - 300),
					Hero->Object.Position[2] + GetLargeRand(50) + 250.0f, Position);

				if ((GetLargeRand(4)) == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_FLARE, Position, Angle, Light, 3, 0.19f, nullptr);
				}
			}
		}
		break;

	case WD_69EMPIREGUARDIAN1:
	case WD_70EMPIREGUARDIAN2:
	case WD_71EMPIREGUARDIAN3:
	case WD_72EMPIREGUARDIAN4:
	case WD_73NEW_LOGIN_SCENE:
	case WD_74NEW_CHARACTER_SCENE:
		{
			MUModel *b = &MODELS::Data[20];
			mu_float fSpeed = 0.32f;

			b->CurrentAction(ThreadIndex) = 0;
			b->PlayAnimation(ThreadIndex, &g_EmpireGuardianAnimationFrame[ThreadIndex], &g_EmpireGuardianPriorAnimationFrame[ThreadIndex], &g_EmpireGuardianPriorAction[ThreadIndex], fSpeed);
		}
		break;
	}

	switch (GET_WORLD)
	{
	case WD_10HEAVEN:
		{
			mu_uint32 q = ThreadIndex;
			for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
			{
				const mu_uint32 queueCount = MUThreadQueues::EnvironmentObjectsQueueCount[t];
				const mu_uint16 *queue = MUThreadQueues::EnvironmentObjectsQueue.data() + MUThreadQueues::EnvironmentObjectsQueueIndex[t];

				for (; q < queueCount; q += _ThreadsCount)
				{
					const mu_uint32 objectIndex = queue[q];
					OBJECT *o = &g_EnvironmentObjects[objectIndex];

					MoveObject(ThreadIndex, objectIndex, o);

					if ((GetLargeRand(500)) == 0 && o->Type >= 0 && o->Type <= 5)
					{
						EVector3 Light;
						Vector(GetLargeRand(10) / 50.0f, GetLargeRand(10) / 50.0f, GetLargeRand(10) / 50.0f, Light);
						CreateSprite(BITMAP_CLOUD + 1, o->Position, 0.5f, Light, &Hero->Object);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, o->Position, o->Position, o->Angle, 6, o, GetLargeRand(20) + 10.0f);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, o->Position, o->Position, o->Angle, 6, o, GetLargeRand(20) + 10.0f);
					}
				}

				q -= queueCount;
			}
		}
		break;

	case WD_18CHAOS_CASTLE + 0:
	case WD_18CHAOS_CASTLE + 1:
	case WD_18CHAOS_CASTLE + 2:
	case WD_18CHAOS_CASTLE + 3:
	case WD_18CHAOS_CASTLE + 4:
	case WD_18CHAOS_CASTLE + 5:
	case WD_53CAOSCASTLE_MASTER_LEVEL:
		{
			mu_uint32 q = ThreadIndex;
			for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
			{
				const mu_uint32 queueCount = MUThreadQueues::EnvironmentObjectsQueueCount[t];
				const mu_uint16 *queue = MUThreadQueues::EnvironmentObjectsQueue.data() + MUThreadQueues::EnvironmentObjectsQueueIndex[t];

				for (; q < queueCount; q += _ThreadsCount)
				{
					const mu_uint32 objectIndex = queue[q];
					OBJECT *o = &g_EnvironmentObjects[objectIndex];

					MoveObject(ThreadIndex, objectIndex, o);
					MoveChaosCastleObject(o);
				}

				q -= queueCount;
			}
		}
		break;

	case WD_30BATTLECASTLE:
		{
			mu_uint32 q = ThreadIndex;
			for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
			{
				const mu_uint32 queueCount = MUThreadQueues::EnvironmentObjectsQueueCount[t];
				const mu_uint16 *queue = MUThreadQueues::EnvironmentObjectsQueue.data() + MUThreadQueues::EnvironmentObjectsQueueIndex[t];

				for (; q < queueCount; q += _ThreadsCount)
				{
					const mu_uint32 objectIndex = queue[q];
					OBJECT *o = &g_EnvironmentObjects[objectIndex];

					MoveObject(ThreadIndex, objectIndex, o);
					battleCastle::MoveBattleCastleObject(ThreadIndex, o);
				}

				q -= queueCount;
			}
		}
		break;

	default:
		{
			mu_uint32 q = ThreadIndex;
			for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
			{
				const mu_uint32 queueCount = MUThreadQueues::EnvironmentObjectsQueueCount[t];
				const mu_uint16 *queue = MUThreadQueues::EnvironmentObjectsQueue.data() + MUThreadQueues::EnvironmentObjectsQueueIndex[t];

				for (; q < queueCount; q += _ThreadsCount)
				{
					const mu_uint32 objectIndex = queue[q];
					OBJECT *o = &g_EnvironmentObjects[objectIndex];

					MoveObject(ThreadIndex, objectIndex, o);
				}

				q -= queueCount;
			}
		}
		break;
	}

	for (mu_uint32 i = ThreadIndex; i < g_EnvironmentObjectsCount; i += _ThreadsCount)
	{
		OBJECT *o = &g_EnvironmentObjects[i];
		ActionObject(ThreadIndex, o);
	}

	PostProcessActionObject(ThreadIndex);
}

void RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT *o)
{
	TheMapProcess().RenderObjectVisual(ThreadIndex, o, &MODELS::Data[o->Type]);
}

void MUTasking::CheckObjects(const mu_uint32 ThreadIndex)
{
	mu_uint32 queueCount = 0;
	const mu_uint32 queueIndex = MUThreadQueues::EnvironmentObjectsQueueIndex[ThreadIndex];

	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, g_EnvironmentObjectsCount, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
	{
		mu_boolean isVisible = false;

		const EObjectCullingData data = g_EnvironmentObjectsNodes[i];
		const mu_uint32 count = data.EX - data.SX + 1;
		for (mu_uint32 y = data.SY; y <= data.EY; ++y)
		{
			mu_int32 index = y * CPUCulling::BLOCK_SIZE;
			if (mu_memcmp(g_FastCullingCheck.data(), &g_CulledBlockVisible[index + data.SX], count) != 0)
			{
				isVisible = true;
				break;
			}
		}

		if (isVisible)
		{
#if 1
			//if (i == 1744) continue;
			OBJECT *o = &g_EnvironmentObjects[i];
			MUModel *b = &MODELS::Data[o->Type];
			EMatrix4 m = EMath::MakeMatrix(o->Position, o->Angle, o->Scale);

			EMathAABB aabb;
			b->_BoundingBox.Transform(aabb, m);

			isVisible = EMath::Contains(g_CpuCulling._Frustum, aabb) != EMath::FR_EXCLUDED;
#endif
		}

		if (isVisible)
		{
			MUThreadQueues::EnvironmentObjectsQueue[queueIndex + queueCount++] = i;
			g_EnvironmentObjects[i].Flags.Set(OBJECT::eOF_VISIBLE);
		}
		else
		{
			g_EnvironmentObjects[i].Flags.Clear(OBJECT::eOF_VISIBLE);
		}
	}

	MUThreadQueues::EnvironmentObjectsQueueCount[ThreadIndex] = queueCount;
}

void MUTasking::PrepareObjects(const mu_uint32 ThreadIndex)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_uint32 q = ThreadIndex;
	for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
	{
		const mu_uint32 queueCount = MUThreadQueues::EnvironmentObjectsQueueCount[t];
		const mu_uint16 *queue = MUThreadQueues::EnvironmentObjectsQueue.data() + MUThreadQueues::EnvironmentObjectsQueueIndex[t];

		for (; q < queueCount; q += _ThreadsCount)
		{
			const mu_uint32 objectIndex = queue[q];
			OBJECT *o = &g_EnvironmentObjects[objectIndex];

			ThreadInfo.RenderIndex = 0;
			ThreadInfo.CulledType = CULLED_OBJECTS;
			ThreadInfo.ObjectIndex = objectIndex;

			RenderObject(ThreadIndex, o);
			if (_UpdateCounter > 0)
			{
				RenderObjectVisual(ThreadIndex, o);
			}
		}

		q -= queueCount;
	}
}