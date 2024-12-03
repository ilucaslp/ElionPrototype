#include "stdafx.h"
#include "GMBattleCastle.h"

namespace battleCastle
{
    typedef struct 
    {
        EVector3 m_vPosition;
        mu_uint8 m_byBuildTime;
    }BuildTime;

	const mu_uint32 g_byGateLocation[6][2] = { { 67, 114 },{ 93, 114 },{ 119, 114 },{ 81, 161 },{ 107, 161 },{ 93, 204 } };
	const mu_float g_fGuardStoneLocation[4][2] = { { 8200, 13000 },{ 10700, 13000 },{ 9400, 18200 },{ 9400, 22700 } };

    mu_uint8 g_byGuardAI = 0;
    mu_boolean g_bBeGate = false;
	mu_float g_fLifeStoneLocation[2]     = { 0.0f, 0.0f };
	mu_boolean g_isCrownState = false;
    mu_boolean g_bBattleCastleStart = false;
    mu_boolean g_bBattleCastleStartBackup = false;
    mu_uint64 g_iMp3PlayTime = 0;

    std::queue<BuildTime> g_qBuildTimeLocation;

    enum
    {
        GUARD_STOP = 0,
        GUARD_READY,
        GUARD_ATTACK_READY,
        GUARD_ATTACK
    };

	mu_boolean IsBattleCastleStart()
	{
		return g_bBattleCastleStart;
	}

	void SetBattleCastleStart(mu_boolean bResult)
	{
		g_bBattleCastleStartBackup = g_bBattleCastleStart;
		g_bBattleCastleStart = bResult;

		if (GET_WORLD != -1 && battleCastle::InBattleCastle())
		{
			g_Terrain.Load(GET_WORLD, g_bBattleCastleStart == true ? 1 : 0, false);

			g_iMp3PlayTime = 0;
		}
	}

	mu_boolean InArea(mu_float x, mu_float y, EVector3 Position, mu_float Range)
	{
		mu_float dx = x - Position[0];
		mu_float dy = y - Position[1];
		mu_float Distance = EMath::Sqrt(dx*dx + dy*dy);
		if (Distance <= Range)
			return true;
		return false;
	}

	void CollisionHeroCharacter(EVector3 Position, mu_float Range, mu_int32 AniType)
	{
		OBJECT* o = &Hero->Object;

		if (InArea(Position[0], Position[1], o->Position, Range) && o->CurrentAction != AniType)
		{
			if (Hero->Helper.Type == MODEL_HELPER + 37 && AniType == PLAYER_HIGH_SHOCK)
			{
				SetAction_Fenrir_Damage(Hero, o);
#if 0 // FIX
				SendRequestAction(AniType, ((mu_uint8)((Hero->Object.Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
			}
			else
			{
				SetAction(o, AniType);
#if 0 // FIX
				SendRequestAction(AniType, ((mu_uint8)((Hero->Object.Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
			}
		}
	}

	void CollisionTempCharacter(EVector3 Position, mu_float Range, mu_int32 AniType)
	{
		for (mu_int32 i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
		{
			OBJECT* o = &CharactersClient[i].Object;
			if (o->Flags.GetAny(OBJECT::eOF_LIVE) == true)
			{
				if (o->Kind == KIND_TMP &&
					o->Type == MODEL_PLAYER &&
					o->CurrentAction != AniType)
				{
					if (InArea(Position[0], Position[1], o->Position, Range))
					{
						SetAction(o, AniType);
					}
				}
			}
		}
	}

	mu_boolean CollisionEffectToObject(OBJECT* eff, mu_float Range, mu_float RangeZ, mu_boolean bCollisionGround, mu_boolean bRealCollision)
	{
		mu_int32 i = (mu_int32)(eff->Position[0] / (4 * g_TerrainScale));
		mu_int32 j = (mu_int32)(eff->Position[1] / (4 * g_TerrainScale));
		if (i < 0 || j < 0 || i >= CPUCulling::BLOCK_SIZE || j >= CPUCulling::BLOCK_SIZE) return false;

		mu_uint32 BlockIndex = MUCPUCulling::GetBlockIndex(i, j);
		auto &Block = g_EnvironmentBlocks[BlockIndex];
		mu_uint16 BlockCount = g_EnvironmentBlocksCount[BlockIndex];

		for (mu_uint32 n = 0; n < BlockCount; ++n)
		{
			OBJECT *o = &g_EnvironmentObjects[Block[n]];
			if (o->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_COLLISION) == true)
			{
				if (o->HiddenMesh == -1)
				{
					mu_float dx = eff->Position[0] - o->Position[0];
					mu_float dy = eff->Position[1] - o->Position[1];
					mu_float Distance = EMath::Sqrt(dx*dx + dy * dy);
					if (Distance <= Range)
					{
						if (bCollisionGround)
						{
							if (o->ExtState == 0)
							{
								o->Timer += 0.3f;
								if (o->Timer >= 5)
								{
									o->ExtState = 99;
								}
							}
							return true;
						}
						else if (EMath::Abs(eff->Position[2] - o->Position[2]) < RangeZ)
						{
							if (o->ExtState == 0)
							{
								if (++o->Timer >= 5)
								{
									o->ExtState = 99;
								}
							}
							eff->LifeTime = 15;
							eff->SubType = 99;
							eff->HiddenMesh = 99;
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	mu_boolean CalcDistanceChrToChr(OBJECT* o, mu_uint8 Type, mu_float fRange)
	{
		for (mu_int32 i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
		{
			CHARACTER* tc = &CharactersClient[i];
			OBJECT*    to = &tc->Object;
			if (to->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == true)
			{
				if (to->Kind == KIND_PLAYER && to->Type == MODEL_PLAYER)
				{
					if (Type == 0)
					{
						mu_float dx = o->Position[0] - to->Position[0];
						mu_float dy = o->Position[1] - to->Position[1];
						if (dx < fRange && dy < 100) return true;
					}
					else if (Type == 1)
					{
						mu_float dx = EMath::Abs(o->Position[0] - to->Position[0]);
						mu_float dy = o->Position[1] - to->Position[1];
						if (dx < 1 && dy>0 && dy < fRange)
						{
							return true;
						}
					}
					else if (Type == 2)
					{
						mu_float dx = o->Position[0] - to->Position[0];
						mu_float dy = o->Position[1] - to->Position[1];
						mu_float Distance = EMath::Sqrt(dx*dx + dy * dy);
						if (Distance < fRange) return true;
					}
				}
			}
		}
		return false;
	}

	void SetCastleGate_Attribute(mu_int32 x, mu_int32 y, mu_uint8 Operator, mu_boolean bAllClear)
	{
		for (mu_int32 i = 0; i < 6; ++i)
		{
			if (bAllClear == true)
			{
				mu_uint32 wall = TerrainWall[(g_byGateLocation[i][1] + 1)*TERRAIN_SIZE + (g_byGateLocation[i][0])];
				if ((wall&TW_NOMOVE) == TW_NOMOVE)
				{
					AddTerrainAttributeRange(g_byGateLocation[i][0] - 1, g_byGateLocation[i][1] + 1, 4, 1, TW_NOMOVE, Operator);
				}
			}
			else
			{
				if (x == g_byGateLocation[i][0] && y == g_byGateLocation[i][1])
				{
					AddTerrainAttributeRange(g_byGateLocation[i][0] - 1, g_byGateLocation[i][1] + 1, 4, 1, TW_NOMOVE, Operator);
					return;
				}
			}
		}
	}

	void SetBuildTimeLocation(OBJECT* o)
	{
		if (o->Type == MODEL_MONSTER01 + 86 && o->m_byBuildTime < 5)
		{
			BuildTime bt;

			VectorCopy(o->Position, bt.m_vPosition);
			bt.m_vPosition[2] += o->BoundingBoxMax[2] + 100.0f;
			bt.m_byBuildTime = o->m_byBuildTime;
			g_qBuildTimeLocation.push(bt);
		}
	}

	void RenderBuildTimes()
	{
#if 0 // FIX
		BuildTime bt;
		for (mu_int32 i = 0; i < (mu_int32)g_qBuildTimeLocation.size(); ++i)
		{
			bt = g_qBuildTimeLocation.front();

			mu_int32 ScreenX, ScreenY;
			mu_int32 Width = 50, Height = 2;

			Projection(bt.m_vPosition, &ScreenX, &ScreenY);
			ScreenX -= (mu_int32)(Width / 2);

			mu_int32 Time = bt.m_byBuildTime / 5.0f*(mu_int32)Width;
			RenderBar(ScreenX, ScreenY + 12, Width, Height, (mu_float)Time);

			g_qBuildTimeLocation.pop();
		}
#endif
	}

	void Init(const mu_uint32 ThreadIndex)
	{
		if (InBattleCastle() == false) return;

		g_byGuardAI = 0;
		g_iMp3PlayTime = 0;

		EVector3 Angle, Position;
		Vector(0.0f, 0.0f, 0.0f, Angle);
		Vector(0.0f, 0.0f, 270.0f, Position);

#if 0 // FIX
		SendGetCastleGuildMark();
#endif

		Position[0] = 65 * g_TerrainScale; Position[1] = 113 * g_TerrainScale;
		CreateObject(ThreadIndex, MODEL_MONSTER01 + 77, Position, Angle);

		Position[0] = 71 * g_TerrainScale; Position[1] = 113 * g_TerrainScale;
		CreateObject(ThreadIndex, MODEL_MONSTER01 + 77, Position, Angle);

		Position[0] = 91 * g_TerrainScale; Position[1] = 113 * g_TerrainScale;
		CreateObject(ThreadIndex, MODEL_MONSTER01 + 77, Position, Angle);

		Position[0] = 118 * g_TerrainScale; Position[1] = 113 * g_TerrainScale;
		CreateObject(ThreadIndex, MODEL_MONSTER01 + 77, Position, Angle);

		Position[0] = 123 * g_TerrainScale; Position[1] = 113 * g_TerrainScale;
		CreateObject(ThreadIndex, MODEL_MONSTER01 + 77, Position, Angle);
	}
    
	mu_boolean SettingBattleFormation(CHARACTER* c, eBuffState state)
	{
		if (InBattleCastle() == false)  return false;

		if (state == eBuff_CastleRegimentAttack1)
		{
			if (c->EtcPart == PARTS_DEFENSE_TEAM_MARK)
			{
				DeleteParts(c);
			}

			c->EtcPart = PARTS_ATTACK_TEAM_MARK;
		}

		else if (state == eBuff_CastleRegimentAttack2)
		{
			if (c->EtcPart == PARTS_DEFENSE_TEAM_MARK)
			{
				DeleteParts(c);
			}

			c->EtcPart = PARTS_ATTACK_TEAM_MARK2;
		}
		else if (state == eBuff_CastleRegimentAttack3)
		{
			if (c->EtcPart == PARTS_DEFENSE_TEAM_MARK)
			{
				DeleteParts(c);
			}

			c->EtcPart = PARTS_ATTACK_TEAM_MARK3;
		}
		else if (state == eBuff_CastleRegimentDefense)
		{
			if (c->EtcPart == PARTS_ATTACK_TEAM_MARK
				|| c->EtcPart == PARTS_ATTACK_TEAM_MARK2
				|| c->EtcPart == PARTS_ATTACK_TEAM_MARK3
				)
			{
				DeleteParts(c);
			}

			c->EtcPart = PARTS_DEFENSE_TEAM_MARK;
		}
		else if (c->EtcPart == PARTS_ATTACK_TEAM_MARK || c->EtcPart == PARTS_DEFENSE_TEAM_MARK
			|| c->EtcPart == PARTS_ATTACK_TEAM_MARK2
			|| c->EtcPart == PARTS_ATTACK_TEAM_MARK3
			)
		{
			DeleteParts(c);
			return false;
		}
		return true;
	}

	mu_boolean GetGuildMaster(CHARACTER* c)
	{
		if (GuildMark[c->GuildMarkIndex].GuildName.size() == 0) return false;
		if (GuildMark[c->GuildMarkIndex].UnionName.size() == 0 && c->GuildStatus != GAME::G_MASTER) return false;
		if (mu_strcmp(GuildMark[c->GuildMarkIndex].UnionName.c_str(), GuildMark[c->GuildMarkIndex].GuildName.c_str()) == 0 && c->GuildStatus != GAME::G_MASTER) return false;

		return true;
	}

	void SettingBattleKing(CHARACTER* c)
	{
		OBJECT* o = &c->Object;

		if (GetGuildMaster(c) == false) return;

		if (g_isCharacterBuff(o, eBuff_CastleRegimentAttack1))
		{
			DeleteParts(c);
			c->EtcPart = PARTS_ATTACK_KING_TEAM_MARK;
		}
		else if (g_isCharacterBuff(o, eBuff_CastleRegimentAttack2))
		{
			DeleteParts(c);
			c->EtcPart = PARTS_ATTACK_KING_TEAM_MARK2;
		}
		else if (g_isCharacterBuff(o, eBuff_CastleRegimentAttack3))
		{
			DeleteParts(c);
			c->EtcPart = PARTS_ATTACK_KING_TEAM_MARK3;
		}
		else if (g_isCharacterBuff(o, eBuff_CastleRegimentDefense))
		{
			DeleteParts(c);
			c->EtcPart = PARTS_DEFENSE_KING_TEAM_MARK;
		}
	}

	void DeleteBattleFormation(CHARACTER* c, eBuffState state)
	{
		if (InBattleCastle() == false)  return;

		if (eBuff_CastleRegimentAttack1 != state &&
			eBuff_CastleRegimentAttack2 != state &&
			eBuff_CastleRegimentAttack3 != state &&
			eBuff_CastleRegimentDefense != state)
		{
			return;
		}

		OBJECT* o = &c->Object;

		if (g_isCharacterBuff(o, eBuff_CastleRegimentAttack1) || g_isCharacterBuff(o, eBuff_CastleRegimentAttack2)
			|| g_isCharacterBuff(o, eBuff_CastleRegimentAttack3) || g_isCharacterBuff(o, eBuff_CastleRegimentDefense))
		{
			if (c->EtcPart == PARTS_DEFENSE_TEAM_MARK || c->EtcPart == PARTS_ATTACK_TEAM_MARK
				|| c->EtcPart == PARTS_ATTACK_TEAM_MARK2 || c->EtcPart == PARTS_ATTACK_TEAM_MARK3)
			{
				DeleteParts(c);
			}
		}
	}

	void ChangeBattleFormation(mu_text* GuildName, mu_boolean bEffect)
	{
		for (mu_int32 i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
		{
			CHARACTER* c = &CharactersClient[i];
			OBJECT *o = &c->Object;
			if (o->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == true)
			{
				DeleteParts(c);
				if (mu_strcmp(GuildMark[c->GuildMarkIndex].UnionName.c_str(), GuildName) == 0)
				{
					g_TokenCharacterBuff(o, eBuff_CastleRegimentDefense);
					c->EtcPart = PARTS_DEFENSE_TEAM_MARK;
				}
				else if (g_isCharacterBuff(o, eBuff_CastleRegimentAttack1)
					|| g_isCharacterBuff(o, eBuff_CastleRegimentDefense))
				{
					if (g_isCharacterBuff(o, eBuff_CastleRegimentDefense))
						g_CharacterUnRegisterBuff(o, eBuff_CastleRegimentDefense);

					g_CharacterRegisterBuff(o, eBuff_CastleRegimentAttack1);
					c->EtcPart = PARTS_ATTACK_TEAM_MARK;
				}
				else if (g_isCharacterBuff(o, eBuff_CastleRegimentAttack2)
					|| g_isCharacterBuff(o, eBuff_CastleRegimentDefense))
				{
					if (g_isCharacterBuff(o, eBuff_CastleRegimentDefense))
						g_CharacterUnRegisterBuff(o, eBuff_CastleRegimentDefense);

					g_CharacterRegisterBuff(o, eBuff_CastleRegimentAttack2);
					c->EtcPart = PARTS_ATTACK_TEAM_MARK2;
				}
				else if (g_isCharacterBuff(o, eBuff_CastleRegimentAttack3)
					|| g_isCharacterBuff(o, eBuff_CastleRegimentDefense))
				{
					if (g_isCharacterBuff(o, eBuff_CastleRegimentDefense))
						g_CharacterUnRegisterBuff(o, eBuff_CastleRegimentDefense);

					g_CharacterRegisterBuff(o, eBuff_CastleRegimentAttack3);
					c->EtcPart = PARTS_ATTACK_TEAM_MARK3;
				}
			}
		}
	}

	void DeleteTmpCharacter()
	{
		for (mu_int32 i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
		{
			CHARACTER* c = &CharactersClient[i];
			OBJECT *o = &c->Object;
			if (o->Flags.Get(OBJECT::eOF_LIVE) == true && o->Kind == KIND_TMP)
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);

				if (c->Key != -1)
				{
					CharactersKeys[c->Key] = -1;
					c->Key = -1;
				}

				DeleteBug(o);
				giPetManager::DeletePet(c);
				DeleteCloth(c, o);
				DeleteParts(c);
			}
		}
	}

	void StartFog(EVector3 Color)
	{
#if 0 // FIX
		glEnable(GL_FOG);

		glFogfv(GL_FOG_COLOR, Color);
		glFogf(GL_FOG_MODE, GL_LINEAR);
		glFogf(GL_FOG_START, 2000.0f);
		glFogf(GL_FOG_END, 2700.0f);
#endif
	}

	void EndFog()
	{
#if 0 // FIX
		glDisable(GL_FOG);
#endif
	}

	mu_boolean CreateFireSnuff(PARTICLE* o)
	{
		if (InBattleCastle() == false) return false;
		if (IsBattleCastleStart() == false) return false;
		if (InBattleCastle3(Hero->Object.Position) == false) return false;

		o->Type = BITMAP_FIRE_SNUFF;
		o->Scale = GetLargeRand(50) / 100.0f + 0.5f;
		EVector3 Position;

		Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(1600) - 800), Hero->Object.Position[1] + (mu_float)(GetLargeRand(1400) - 500), Hero->Object.Position[2] + (mu_float)(GetLargeRand(300) + 50), Position);

		VectorCopy(Position, o->Position);
		VectorCopy(Position, o->StartPosition);
		o->Velocity[0] = -(mu_float)(GetLargeRand(64) + 64)*0.1f;
		if (Position[1] < MU_CAMERA->GetCameraPosition()[1] + 400.0f)
		{
			o->Velocity[0] = -o->Velocity[0] + 3.2f;
		}
		o->Velocity[1] = (mu_float)(GetLargeRand(32) - 16)*0.1f;
		o->Velocity[2] = (mu_float)(GetLargeRand(32) - 16)*0.1f;
		o->TurningForce[0] = (mu_float)(GetLargeRand(16) - 8)*0.1f;
		o->TurningForce[1] = (mu_float)(GetLargeRand(64) - 32)*0.1f;
		o->TurningForce[2] = (mu_float)(GetLargeRand(16) - 8)*0.1f;

		Vector(1.0f, 0.0f, 0.0f, o->Light);

		return true;
	}

	void SetAttackDefenseObjectType(OBJECT* o)
	{
		switch (o->Type)
		{
		case 16:
		case 38:
		case 39:
		case 40:
		case 50:
		case 58:
		case 59:
		case 78:
		case 80:
		case 81:
		case 82:
		case 83:
		case 84:
		case 85:
		case 86:
		case 87:
			if (IsBattleCastleStart())
			{
				o->HiddenMesh = -2;
			}
			else
			{
				o->HiddenMesh = -1;
			}
			break;

		case 4:
		case 11:
		case 37:
		case 41:
		case 43:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 67:
		case 68:
		case 69:
		case 71:
		case 72:
		case 73:
		case 74:
		case 75:
		case 76:
		case 88:
			if (IsBattleCastleStart())
			{
				o->HiddenMesh = -1;
			}
			else
			{
				o->HiddenMesh = -2;
			}
			break;
		}
	}

	mu_boolean MoveBattleCastleObject(const mu_uint32 ThreadIndex, OBJECT* o)
	{
		switch (o->Type)
		{
		case 81:
			o->BlendMesh = 1;
			o->BlendMeshLight = 1.0f;
			o->BlendMeshTexCoordV = FWorldTime*0.0002f;
			break;

		case 83:
			o->BlendMesh = 1;
			o->BlendMeshLight = 1.0f;
			o->BlendMeshTexCoordV = -FWorldTime*0.0004f;
			break;

		case BATTLE_CASTLE_WALL1:
		case BATTLE_CASTLE_WALL2:
			if (IsBattleCastleStart() == false) break;

			if (o->ExtState == 0)
			{
				o->HiddenMesh = -1;
			}
			else if (o->ExtState == 99)
			{
				if (o->HiddenMesh == -1)
				{
					CreateEffect(ThreadIndex, o->Type, o->Position, o->Angle, o->Light, 1);
					PlayBackground(SOUND_BC_WALL_HIT);
				}
				o->HiddenMesh = 0;
			}
			break;
		case BATTLE_CASTLE_WALL3:
		case BATTLE_CASTLE_WALL4:
			if (IsBattleCastleStart() == false) break;

			if (o->ExtState == 0)
			{
				o->HiddenMesh = -1;
			}
			else if (o->ExtState == 99)
			{
				if (o->HiddenMesh == -1)
				{
					CreateEffect(ThreadIndex, o->Type, o->Position, o->Angle, o->Light, 1);
					PlayBackground(SOUND_BC_WALL_HIT);
				}
				o->HiddenMesh = 0;
			}
			break;
		}

		SetAttackDefenseObjectType(o);
		return false;
	}

	mu_boolean CreateBattleCastleObject(const mu_uint32 ThreadIndex, mu_uint32 objectIndex, OBJECT* o)
	{
		switch (o->Type)
		{
		case    8:
		case    9:
			o->ExtState = 0;
			break;

		case    7:
		case    10:
		case    13:
		case    14:
			o->ExtState = 0;
			o->Flags.Set(OBJECT::eOF_COLLISION);
			break;

		case    BATTLE_CASTLE_WALL1:
		case    BATTLE_CASTLE_WALL2:
		case    BATTLE_CASTLE_WALL3:
		case    BATTLE_CASTLE_WALL4:
			o->ExtState = 0;
			o->Flags.Set(OBJECT::eOF_COLLISION);
			break;

		case    19:
			o->Scale = 1.0f;
			break;

		case    MODEL_MONSTER01 + 77:
			o->Scale = 1.0f;
			if (IsBattleCastleStart())
			{
				o->HiddenMesh = -2;
			}
			else
			{
				o->HiddenMesh = -1;
			}
			break;

		case 39:
			o->HiddenMesh = -2;
			break;

		case 41:
			o->Timer = mu_float(GetLargeRand(1000)) * 0.01f;
			break;

		case 77:
		case 84:
			CreateOperate(objectIndex, o);
			break;

		case 79:
			CreateEffect(ThreadIndex, MODEL_TOWER_GATE_PLANE, o->Position, o->Angle, o->Light, 0, o);
			break;
		}

		SetAttackDefenseObjectType(o);
		return true;
	}

	mu_boolean MoveBattleCastleVisual(OBJECT* o)
	{
		mu_float Luminosity;
		EVector3 Light;

		switch (o->Type)
		{
		case 4:
			o->HiddenMesh = -1;
			o->BlendMeshTexCoordU = FWorldTime*0.0001f;
			break;

		case    19:
			break;

		case	42:
		case	52:
		case	54:
			o->HiddenMesh = -2;
			break;

		case    0:
			if (IsBattleCastleStart() == false)
			{
				Luminosity = (mu_float)(GetLargeRand(4) + 3)*0.1f;
				Vector(Luminosity, Luminosity*0.6f, Luminosity*0.2f, Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
			}
			o->HiddenMesh = -2;
			break;

		case	53:
			if (IsBattleCastleStart())
			{
				Luminosity = (mu_float)(GetLargeRand(4) + 3)*0.1f;
				Vector(Luminosity, Luminosity*0.6f, Luminosity*0.2f, Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
			}
			o->HiddenMesh = -2;
			break;

		case 66:
			o->HiddenMesh = -2;
			if (IsBattleCastleStart())
			{
				if (g_isCrownState == false)
				{
					o->HiddenMesh = -1;
				}
				o->BlendMeshTexCoordV = FWorldTime*0.001f;
			}
			break;

		case    BATTLE_CASTLE_WALL1:
		case    BATTLE_CASTLE_WALL2:
		case    BATTLE_CASTLE_WALL3:
		case    BATTLE_CASTLE_WALL4:
			break;

		case    MODEL_MONSTER01 + 77:
			if (IsBattleCastleStart())
			{
				o->HiddenMesh = -2;
			}
			else
			{
				o->HiddenMesh = -1;
			}
			break;

		}
		return true;
	}

	mu_boolean RenderBattleCastleVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
	{
		EVector3 Light;

		switch (o->Type)
		{
		case 0:
			if (IsBattleCastleStart() == false && GetLargeRand(3) == 0)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, o->Position, o->Angle, Light, 0, o->Scale);
			}
			break;
		case 41:
			o->BlendMeshLight = EMath::Sin(FWorldTime*0.002f)*0.3f + 0.7f;
			break;
		case 42:
			if (IsBattleCastleStart() && GetLargeRand(3) == 0)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 21, o->Scale);
			}
			break;
		case 52:
			if (IsBattleCastleStart() == false)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, o->Position, o->Angle, Light, 6, o->Scale);
			}
			break;
		case 53:
			if (IsBattleCastleStart() && GetLargeRand(3) == 0)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, o->Position, o->Angle, Light, 0, o->Scale);
			}
			break;
		case 54:
			if (IsBattleCastleStart() == false)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_WATERFALL_3 + (GetLargeRand(2)), o->Position, o->Angle, Light, 0);
			}
			break;
		}

		return true;
	}

	mu_boolean RenderBattleCastleObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
	{
		MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

		if (o->Type == 12)
		{
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_INTERFACE_MAP);
			return true;
		}
		else if (o->Type == 17)
		{
			b->StreamMesh(ThreadIndex) = 0;
			Vector(0.45f, 0.45f, 0.45f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, IWorldTime % 10000 * 0.0002f, o->HiddenMesh);
			b->StreamMesh(ThreadIndex) = -1;
			return true;
		}
		else if (o->Type == 51)
		{
			b->StreamMesh(ThreadIndex) = 0;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, -IWorldTime % 10000 * 0.0002f, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->StreamMesh(ThreadIndex) = -1;
			return true;
		}
		else if (o->Type == 66)
		{
			Vector(0.1f, 0.4f, 0.6f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, EMath::Sin(FWorldTime*0.001f)*0.2f + 0.3f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			return true;
		}
		else if (o->Type == 50 || o->Type == 83 || o->Type == 84 || o->Type == 85)
		{
			if (IsBattleCastleStart() == false)
			{
				ThreadInfo.EnableShadow = true;
				ThreadInfo.ShadowHeight = b->BodyOrigin(ThreadIndex)[2];
				ThreadInfo.ShadowLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 1.0f);
			}

			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

			ThreadInfo.EnableShadow = false;

			return true;
		}
		else if (o->Type == 19)
		{
			if (IsBattleCastleStart())
			{
				b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				Vector(0.3f, 0.3f, 0.3f, b->BodyLight(ThreadIndex));
				b->RenderMesh(ThreadIndex, 3, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
				Vector(0.2f, 0.2f, 0.5f, b->BodyLight(ThreadIndex));
				b->RenderMesh(ThreadIndex, 3, RENDER_BRIGHT | RENDER_METAL, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			}
			else
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				Vector(0.3f, 0.3f, 0.3f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, 0.5f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
				Vector(0.2f, 0.2f, 0.5f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_METAL, 0.5f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			}
			return true;
		}
		else if (o->Type >= BATTLE_CASTLE_WALL1 && o->Type <= BATTLE_CASTLE_WALL4)
		{
			if (o->SubType == 0)
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
			else
			{
				b->RenderMeshEffect(ThreadIndex, o->BoneTransform, 0, MODEL_WALL_PART1);
			}
			return true;
		}
		else
		{
			return RenderBattleCastleMonsterObjectMesh(ThreadIndex, o, b);
		}

		return false;
	}

	CHARACTER* CreateBattleCastleMonster(const mu_uint32 ThreadIndex, mu_int32 Type, mu_int32 PositionX, mu_int32 PositionY, mu_int32 Key)
	{
		if (InBattleCastle() == false) return nullptr;

		CHARACTER*  c = nullptr;
		switch (Type)
		{
		case 104:
			c = CreateCharacter(Key, 11, PositionX, PositionY);
			c->m_bFixForm = true;
			break;

		case 215:
			c = CreateCharacter(Key, MODEL_NPC_BARRIER, PositionX, PositionY);
			c->m_bFixForm = true;
			c->m_bIsSelected = false;
			c->Object.Scale = 1.52f;
			c->Object.LifeTime = 0;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			break;

		case 216:
			c = CreateCharacter(Key, MODEL_NPC_CROWN, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			break;

		case 217:
			c = CreateCharacter(Key, MODEL_NPC_CHECK_FLOOR, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			c->Object.Velocity = c->Object.Position[2];
			if (IsBattleCastleStart() == false)
				c->Object.Position[2] -= 100.0f;
			break;

		case 218:
			c = CreateCharacter(Key, MODEL_NPC_CHECK_FLOOR, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			c->Object.Velocity = c->Object.Position[2];
			if (IsBattleCastleStart() == false)
				c->Object.Position[2] -= 100.0f;
			break;

		case 219:
			c = CreateCharacter(Key, MODEL_NPC_GATE_SWITCH, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Object.Scale = 1.1f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			break;

		case 220:
			c = CreateCharacter(Key, MODEL_MONSTER01 + 77, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Object.Scale = 1.1f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			break;

		case 221:
			c = CreateCharacter(Key, MODEL_NPC_CAPATULT_ATT, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Object.Scale = 0.8f;
			c->Object.m_fEdgeScale = 1.03f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			break;

		case 222:
			c = CreateCharacter(Key, MODEL_NPC_CAPATULT_DEF, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Object.Scale = 0.8f;
			c->Object.m_fEdgeScale = 1.03f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			break;

		case 223:
			c = CreateCharacter(Key, MODEL_NPC_SENATUS, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Object.Scale = 1.1f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			break;

		case 224:
			c = CreateCharacter(Key, MODEL_NPC_CLERK, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Object.Scale = 1.0f;
			c->Object.SubType = GetLargeRand(2) + 10;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			break;

		case 277:
			c = CreateCharacter(Key, MODEL_MONSTER01 + 73, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Object.Scale = 1.0f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			break;

		case 278:
			c = CreateCharacter(Key, MODEL_MONSTER01 + 86, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Object.Scale = 1.0f;
			c->Object.BlendMesh = 3;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;

			CreateEffect(ThreadIndex, MODEL_AURORA, c->Object.Position, c->Object.Angle, c->Object.Light, 0, &c->Object, 120);
			break;

		case 283:
			c = CreateCharacter(Key, MODEL_MONSTER01 + 74, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Object.Scale = 1.0f;
			c->Object.m_fEdgeScale = 1.03f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;

			if (IsBattleCastleStart())
			{
				c->Object.HiddenMesh = -1;
			}
			else
			{
				c->Object.HiddenMesh = -2;
			}

			CreateEffect(ThreadIndex, MODEL_AURORA, c->Object.Position, c->Object.Angle, c->Object.Light, 0, &c->Object, 120);
			break;

		case 285:
			c = CreateCharacter(Key, MODEL_MONSTER01 + 74, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Object.Scale = 1.0f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			break;

		case 286:
			c = CreateCharacter(Key, MODEL_MONSTER01 + 76, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Object.Scale = 1.0f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			break;

		case 287:
			c = CreateCharacter(Key, MODEL_MONSTER01 + 77, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Object.Scale = 1.0f;
			c->Object.SubType = 30;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			break;

		case 288:
			c = CreateCharacter(Key, MODEL_MONSTER01 + 79, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Object.Scale = 1.0f;
			c->Object.m_fEdgeScale = 1.04f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;

			if (IsBattleCastleStart())
			{
				c->Object.HiddenMesh = -1;
			}
			else
			{
				c->Object.HiddenMesh = -2;
			}
			c->Object.HiddenMesh = -1;
			break;
		}

		return c;
	}

	mu_boolean SettingBattleCastleMonsterLinkBone(CHARACTER* c, mu_int32 Type)
	{
		switch (Type)
		{
		case MODEL_NPC_CAPATULT_ATT:
			Vector(-200.0f, -50.0f, -100.0f, c->Object.BoundingBoxMin);
			Vector(200.0f, 50.0f, 100.0f, c->Object.BoundingBoxMax);
			break;

		case MODEL_MONSTER01 + 73:
			Vector(-140.0f, -140.0f, 0.0f, c->Object.BoundingBoxMin);
			Vector(140.0f, 140.0f, 300.0f, c->Object.BoundingBoxMax);
			c->SwordCount = 0;
			return true;
		}

		return false;
	}

	mu_boolean StopBattleCastleMonster(CHARACTER* c, OBJECT* o)
	{
		if (InBattleCastle() == false)  return false;

		switch (c->MonsterIndex)
		{
		case 286:
		case 287:
			if (o->CurrentAction == 5)
			{
				o->PriorAction = 4;
				o->PriorAnimationFrame = 0.0f;
				o->CurrentAction = 4;
				o->AnimationFrame = 0.0f;

				g_byGuardAI = GUARD_ATTACK_READY;
				o->AI = GUARD_READY;
				o->AttackPoint[0] = 300;
				return true;
			}
			else if (o->CurrentAction == 4)
			{
				o->PriorAction = 4;
				o->PriorAnimationFrame = 0.0f;
				o->CurrentAction = 4;
				o->AnimationFrame = 0.0f;
				return true;
			}
			else if (o->CurrentAction == 2)
			{
				o->PriorAction = 4;
				o->PriorAnimationFrame = 0.0f;
				o->CurrentAction = 4;
				o->AnimationFrame = 0.0f;
				return true;
			}
			else if (o->CurrentAction == 0)
			{
				o->PriorAction = 0;
				o->PriorAnimationFrame = 0.0f;
				o->CurrentAction = 0;
				o->AnimationFrame = 0.0f;
				return true;
			}
			return false;

		case 221:
		case 222:
			SetAction(o, 0);
			return true;

		case 277:
			{
				if (g_isCharacterBuff(o, eBuff_CastleGateIsOpen))
				{
					c->m_bIsSelected = false;
					SetAction(o, 2);
				}
				else
				{
					c->m_bIsSelected = true;
					SetAction(o, 0);
				}
			}
			return true;
		}
		return false;
	}

	void InitGateAttribute()
	{
		if (g_bBeGate == false)
		{
			SetCastleGate_Attribute(0, 0, 0, true);
		}
	}

	void InitEtcSetting()
	{
		g_fLifeStoneLocation[0] = 0.0f;
		g_fLifeStoneLocation[1] = 0.0f;

		InitGateAttribute();
		g_bBeGate = false;
	}

	mu_boolean MoveBattleCastleMonster(CHARACTER* c, OBJECT* o)
	{
		if (InBattleCastle() == false)  return false;

		switch (c->MonsterIndex)
		{
		case 216:
			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 200.0f;
			g_isCrownState = g_isCharacterBuff(o, eBuff_CastleCrown);
			c->m_bIsSelected = (g_isCrownState ? true : false);
			break;

		case 277:
			g_bBeGate = true;
			break;

		case 283:
			o->HiddenMesh = -1;
			c->m_bIsSelected = true;
			if (IsBattleCastleStart() == false)
			{
				o->HiddenMesh = -2;
				c->m_bIsSelected = false;
			}
			break;

		case 286:
		case 287:
			{
				switch (o->AI)
				{
				case GUARD_STOP:
					{
						mu_float fRange = 500.0f;

						o->AttackPoint[0] = 0;

						if (CalcDistanceChrToChr(o, 1, fRange) || g_byGuardAI == GUARD_ATTACK_READY)
						{
							SetAction(o, 4);

							g_byGuardAI = GUARD_ATTACK_READY;
							o->AI = GUARD_READY;
							o->AttackPoint[0] = 300;
						}
					}
					break;

				case GUARD_READY:
					break;

				case GUARD_ATTACK_READY:
					break;

				case GUARD_ATTACK:
					break;
				}
				if (o->CurrentAction != MONSTER01_SHOCK && o->AI != GUARD_STOP && (o->AttackPoint[0] < 0 || g_byGuardAI == GUARD_STOP))
				{
					mu_float fRange = 500.0f;

					if (CalcDistanceChrToChr(o, 1, fRange))
					{
						o->AttackPoint[0] = 300;
						g_byGuardAI = GUARD_ATTACK_READY;
					}
					else
					{
						SetAction(o, 0);
						o->AI = GUARD_STOP;
						g_byGuardAI = GUARD_STOP;
					}
				}
				--o->AttackPoint[0];
			}
			break;

		case 278:
			if (c->m_byFriend == 128)
			{
				g_fLifeStoneLocation[0] = o->Position[0];
				g_fLifeStoneLocation[1] = o->Position[1];
			}
			break;
		}
		return false;
	}

	mu_boolean SetCurrentAction_BattleCastleMonster(CHARACTER* c, OBJECT* o)
	{
		switch (o->Type)
		{
		case 11:
			break;

		case MODEL_MONSTER01 + 76:
		case MODEL_MONSTER01 + 77:
			o->AI = GUARD_ATTACK;
			SetAction(o, 5);
			return true;
		}
		return false;
	}

	mu_boolean AttackEffect_BattleCastleMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
	{
		if (InBattleCastle() == false) return false;

		CHARACTER*  tc = nullptr;
		OBJECT*     to = nullptr;
		EVector3      Light;
		EVector3      p, Position;

		Vector(0.0f, 0.0f, 0.0f, p);
		Vector(1.0f, 1.0f, 1.0f, Light);
		if (c->TargetCharacter >= 0 && c->TargetCharacter < MAX_CHARACTERS_CLIENT)
		{
			tc = &CharactersClient[c->TargetCharacter];
			to = &tc->Object;
		}

		switch (c->MonsterIndex)
		{
		case 104:
			if (c->AttackTime == 5)
			{
				VectorCopy(o->Position, Position);
				Position[2] += 500.0f;
				CreateEffect(ThreadIndex, MODEL_MONSTER01 + 77, Position, o->Angle, o->Light, 0);
			}
			return true;

		case 288:
			CreateEffect(ThreadIndex, BITMAP_JOINT_FORCE, o->Position, o->Angle, o->Light);
			return true;
		}
		return false;
	}

	void CreateGuardStoneHealingVisual(const mu_uint32 ThreadIndex, CHARACTER* c, mu_float Range)
	{
		OBJECT* o = &c->Object;
		mu_boolean bHealing = false;

		if (g_isCharacterBuff(o, eBuff_CastleRegimentDefense))
		{
			for (mu_int32 i = 0; i < 4; ++i)
			{
				if (InArea(g_fGuardStoneLocation[i][0], g_fGuardStoneLocation[i][1], o->Position, Range))
				{
					bHealing = true;
					break;
				}
			}
		}
		else
		{
			if (g_fLifeStoneLocation[0] != 0.0f && g_fLifeStoneLocation[1] != 0.0f && Hero->GuildMarkIndex == c->GuildMarkIndex)
			{
				if (InArea(g_fLifeStoneLocation[0], g_fLifeStoneLocation[1], o->Position, Range))
				{
					bHealing = true;
				}
			}
		}

		if (bHealing && MoveSceneFrame % 10 == 0)
		{
			CreateParticle(ThreadIndex, BITMAP_PLUS, o->Position, o->Angle, o->Light);
		}
	}

	mu_boolean MoveBattleCastleMonsterVisual(OBJECT* o, MUModel* b)
	{
		if (InBattleCastle() == false) return false;

		mu_float  Luminosity;
		EVector3 Light;

		switch (o->Type)
		{
		case 11:
			break;

		case MODEL_NPC_CROWN:
			o->HiddenMesh = -1;
			if (IsBattleCastleStart())
			{
				if (g_isCrownState == false)
				{
					o->HiddenMesh = -2;
				}
			}
			break;

		case MODEL_MONSTER01 + 73:
			o->Angle[2] = 0.0f;
			return true;

		case MODEL_MONSTER01 + 74:
			o->Angle[2] = 0.0f;

			Luminosity = EMath::Sin(FWorldTime*0.0005)*0.58f + 0.42f;
			Vector(Luminosity*0.2f, Luminosity*0.7f, Luminosity*1.0f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
			return true;

		case MODEL_MONSTER01 + 75:
			o->Angle[2] = 0.0f;
			return true;

		case MODEL_MONSTER01 + 76:
			o->Angle[2] = 0.0f;
			return true;

		case MODEL_MONSTER01 + 77:
			o->Angle[2] = 0.0f;
			return true;

		case MODEL_MONSTER01 + 78:
			o->Angle[2] = 0.0f;
			return true;

		case MODEL_MONSTER01 + 79:
			o->Angle[2] = 45.0f;
			return true;
		}

		return false;
	}

	mu_boolean RenderBattleCastleMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
	{
		if (InBattleCastle() == false) return false;

		EVector3 Position, Light, p;

		switch (o->Type)
		{
		case MODEL_MONSTER01 + 73:
			break;

		case MODEL_MONSTER01 + 74:
			if (IsBattleCastleStart())
			{
				Vector(0.3f, 0.2f, 0.0f, Light);
				RenderAurora(BITMAP_MAGIC + 1, RENDER_BRIGHT, o->Position[0], o->Position[1], 9.0f, 9.0f, Light);
			}
			else
			{
				RenderAurora(BITMAP_MAGIC + 1, RENDER_DARK, o->Position[0], o->Position[1], 5.5f, 5.5f, o->Light);
			}
			return true;

		case MODEL_MONSTER01 + 86:
			if (o->m_byBuildTime >= 5)
			{
				o->BlendMeshLight = EMath::Sin(FWorldTime*0.001f)*0.3 + 0.3f;

				Vector(0.3f, 0.2f, 0.0f, Light);
				RenderAurora(BITMAP_MAGIC + 1, RENDER_BRIGHT, o->Position[0], o->Position[1], 9.0f, 9.0f, Light);
			}
			else
			{
				Vector(0.8f, 0.6f, 0.3f, Light);
				VectorCopy(o->Position, Position);
				Position[2] += 50.0f;
				CreateSprite(BITMAP_LIGHT, Position, 4.0f, Light, nullptr);
				CreateSprite(BITMAP_SHINY + 1, Position, 2.5f, Light, nullptr, GetLargeRand(360));
			}
			return true;

		case MODEL_MONSTER01 + 79:
			if (IsBattleCastleStart())
			{
				c->m_bIsSelected = true;

				Vector(0.0f, 0.0f, 0.0f, p);
				Vector(0.8f, 0.6f, 0.3f, Light);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(1)], p, Position, true);

				Position[2] += EMath::Sin(FWorldTime*0.001f)*60.0f + 60.0f;

				CreateSprite(BITMAP_LIGHT, Position, 3.0f, Light, nullptr);
				CreateSprite(BITMAP_SHINY + 1, Position, 1.5f, Light, nullptr, GetLargeRand(360));

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(2)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 4.0f, Light, nullptr);

				RenderAurora(BITMAP_MAGIC + 1, RENDER_DARK, o->Position[0], o->Position[1], 5.5f, 5.5f, o->Light);
			}
			else
			{
				c->m_bIsSelected = false;
				o->HiddenMesh = -2;
			}
			return true;

		case MODEL_NPC_CHECK_FLOOR:
			if (IsBattleCastleStart())
			{
				o->Position[2] = o->Velocity;
				c->m_bIsSelected = true;
			}
			else
			{
				o->Position[2] = o->Velocity - 100.0f;
				c->m_bIsSelected = false;
			}
			o->BlendMeshLight = EMath::Sin(FWorldTime*0.001f)*0.3f + 0.5f;
			return true;
		}

		return false;
	}

	mu_boolean RenderBattleCastleMonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
	{
		MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];
		mu_boolean    success = false;

		switch (o->Type)
		{
		case MODEL_NPC_CAPATULT_ATT:
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

				OBB_t   OBB;
				EVector3  Temp, p, Position;
				MUModel*    linkBmd = &MODELS::Data[MODEL_FLY_BIG_STONE1];
				linkBmd->BodyHeight(ThreadIndex) = 0.0f;
				BodyLight(ThreadIndex, o, linkBmd);
				linkBmd->BodyScale(ThreadIndex) = 1.3f;
				linkBmd->CurrentAction(ThreadIndex) = 0;

				if (o->CurrentAction == 0 || o->AnimationFrame < 2.0f || o->AnimationFrame >= 22.0f)
				{
					Vector(0.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(12)], p, Position, true);
					VectorCopy(Position, linkBmd->BodyOrigin(ThreadIndex));

					linkBmd->Animation(ThreadIndex, ThreadInfo.BoneTransform, 0.0f, 0.0f, 0, o->Angle, o->Angle, true);
					ThreadInfo.SkeletalIndex = g_Tasking.RegisterSkeletal(ThreadInfo.BoneTransform, linkBmd->_BoneNonDummyEnd);
					ThreadInfo.Translate = true;
					linkBmd->Transform(ThreadIndex, Temp, Temp, &OBB, true);
					
					linkBmd->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				}

				if (o->CurrentAction == 1 && o->AnimationFrame > 13.0f && o->AnimationFrame < 22.0f)
				{
					Vector(0.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(44)], p, Position, true);
					VectorCopy(Position, linkBmd->BodyOrigin(ThreadIndex));

					linkBmd->Animation(ThreadIndex, ThreadInfo.BoneTransform, 0.0f, 0.0f, 0, o->Angle, o->Angle, true);
					ThreadInfo.SkeletalIndex = g_Tasking.RegisterSkeletal(ThreadInfo.BoneTransform, linkBmd->_BoneNonDummyEnd);
					ThreadInfo.Translate = true;
					linkBmd->Transform(ThreadIndex, Temp, Temp, &OBB, true);
					
					linkBmd->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				}
			}
			success = true;
			break;

		case MODEL_NPC_BARRIER:
			Vector(0.3f, 0.3f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_WATER + WaterTextureNumber);
			Vector(0.3f, 1.0f, 0.3f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_WATER + ((mu_int32)(FWorldTime*0.01f) % 32));
			success = true;
			break;

		case MODEL_MONSTER01 + 73:
			if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->CurrentAction == MONSTER01_DIE)
				{
					o->Flags.Clear(OBJECT::eOF_LIVE);
				}
				PlayBackground(SOUND_HIT_GATE2);
				b->RenderMeshEffect(ThreadIndex, o->BoneTransform, 0, MODEL_GATE_PART1);
			}
			else
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
			break;

		case MODEL_MONSTER01 + 74:
			if (IsBattleCastleStart())
			{
				if (o->CurrentAction != MONSTER01_DIE)
					o->HiddenMesh = -1;

				if (o->CurrentAction == MONSTER01_DIE)
				{
					if (o->CurrentAction == MONSTER01_DIE)
					{
						o->Flags.Clear(OBJECT::eOF_LIVE);
					}

					PlayBackground(SOUND_BC_GUARD_STONE_DIS);
					b->RenderMeshEffect(ThreadIndex, o->BoneTransform, 0, MODEL_STONE_COFFIN);
				}
				else
				{
					b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
					Vector(0.3f, 0.3f, 0.3f, b->BodyLight(ThreadIndex));
					b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, 0.5f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
					Vector(0.2f, 0.2f, 0.5f, b->BodyLight(ThreadIndex));
					b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_METAL, 0.5f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
					Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
				}
			}
			success = true;
			break;

		case MODEL_MONSTER01 + 86:
			o->BlendMeshLight = o->m_byBuildTime / 10.0f;
			Vector(o->BlendMeshLight, o->BlendMeshLight, o->BlendMeshLight, b->BodyLight(ThreadIndex));
			if (o->m_byBuildTime >= 2)
			{
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE);
			}
			if (o->m_byBuildTime >= 3)
			{
				b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT);
			}
			if (o->m_byBuildTime >= 4)
			{
				Vector(o->BlendMeshLight*0.5f, o->BlendMeshLight*0.5f, o->BlendMeshLight*0.5f, b->BodyLight(ThreadIndex));
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT);
			}
			if (o->m_byBuildTime >= 5)
			{
				Vector(0.5f, 0.5f, 0.5f, b->BodyLight(ThreadIndex));
				b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, 1.0f, 3, 1.0f, FWorldTime*0.0001f);

				Vector(0.2f, 0.2f, 0.2f, b->BodyLight(ThreadIndex));
				b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE | RENDER_BRIGHT, 1.0f, 3, 1.0f, FWorldTime*0.0001f);
			}
			success = true;
			break;
		}

		return success;
	}

	void RenderMonsterHitEffect(const mu_uint32 ThreadIndex, OBJECT* o)
	{
		if (InBattleCastle() == false) return;

		switch (o->Type)
		{
		case MODEL_MONSTER01 + 73:
			{
				EVector3 Position;
				VectorCopy(o->Position, Position);

				Position[0] += 60.0f;
				Position[1] -= 100.0f;
				Position[2] += 150.0f;

				if (!g_isCharacterBuff(o, eBuff_CastleGateIsOpen))
				{
					if (!g_isCharacterBuff((&Hero->Object), eBuff_BlessPotion) && !g_isCharacterBuff((&Hero->Object), eBuff_SoulPotion))
					{
						CreateEffect(ThreadIndex, MODEL_WAVES, Position, o->Angle, o->Light, 6, nullptr, 0);
					}

					for (mu_int32 i = 0; i < 5; ++i)
					{
						Position[0] = o->Position[0];
						Position[1] = o->Position[1];
						Position[2] = o->Position[2] + 200 + GetLargeRand(30);

						CreateEffect(ThreadIndex, MODEL_GATE_PART1 + GetLargeRand(3), Position, o->Angle, o->Light, GetLargeRand(2) + 1);
					}
				}
			}
			break;

		case MODEL_MONSTER01 + 74:
			{
				EVector3 Position;
				for (mu_int32 i = 0; i < 5; ++i)
				{
					if ((GetLargeRand(2)) == 0)
					{
						Position[0] = o->Position[0];
						Position[1] = o->Position[1];
						Position[2] = o->Position[2] + 200 + GetLargeRand(30);

						CreateEffect(ThreadIndex, MODEL_STONE_COFFIN + 1, Position, o->Angle, o->Light);
					}
				}
			}
			break;
		}
	}
}