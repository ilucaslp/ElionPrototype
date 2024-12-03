#include "stdafx.h"
#include "mu_tasking.h"

mu_uint8 ActionRequest = ACTION_REQUEST::AR_NONE;
mu_int32 ActionRequestTarget = -1;
mu_uint16 ActionRequestMagic = 0;
mu_uint8 ActionRequestX = 0;
mu_uint8 ActionRequestY = 0;
mu_float ActionRequestDirection = 0.0f;

void FallingCharacter(CHARACTER* c, OBJECT* o)
{
	EBone Matrix;
	EVector3 Position, p;

	Vector(0.0f, 0.0f, 0.0f, Position);
	Vector(0.0f, o->Direction[1], 0.0f, p);
	AngleMatrix(o->m_vDownAngle, Matrix);
	VectorRotate(p, Matrix, Position);

	o->Direction[1] += o->Direction[0];
	o->Gravity += o->Velocity;
	o->Velocity -= o->Direction[2];
	o->Angle[0] -= 5.0f;

	o->Position[0] = o->m_vDeadPosition[0] + Position[0];
	o->Position[1] = o->m_vDeadPosition[1] + Position[1];
	o->Position[2] = o->m_vDeadPosition[2] + o->Gravity;
}

void  PushingCharacter(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o)
{
	if (c->StormTime > 0)
	{
		o->Angle[2] += c->StormTime * 10;
		--c->StormTime;
	}
	if (c->JumpTime > 0)
	{
		mu_float Speed = 0.2f;
		if (o->Type == MODEL_MONSTER01 + 52)
		{
			Speed = 0.07f;
		}
		if (InChaosCastle() == true)
		{
			o->Position[0] += (((mu_float)c->TargetX)*g_TerrainScale - o->Position[0])*Speed;
			o->Position[1] += (((mu_float)c->TargetY)*g_TerrainScale - o->Position[1])*Speed;
			++c->JumpTime;
			if (c->JumpTime > 15)
			{
				SetPlayerStop(ThreadIndex, c);

				o->Position[0] = c->TargetX*g_TerrainScale;
				o->Position[1] = c->TargetY*g_TerrainScale;

				c->PositionX = c->TargetX;
				c->PositionY = c->TargetY;

				c->JumpTime = 0;
			}
		}
		else
		{
			o->Position[0] += (((mu_float)c->TargetX + 0.5f)*g_TerrainScale - o->Position[0])*Speed;
			o->Position[1] += (((mu_float)c->TargetY + 0.5f)*g_TerrainScale - o->Position[1])*Speed;
			if (o->Type != MODEL_BALL)
				o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (c->JumpTime++ > 15)
			{
				if (o->Type == MODEL_MONSTER01 + 52)
					SetPlayerStop(ThreadIndex, c);

				c->JumpTime = 0;
			}
		}
	}
	if (o->Type == MODEL_BALL)
	{
		o->Position[2] += o->Gravity;
		o->Gravity -= 6.0f;
		mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]) + 30.0f;
		if (o->Position[2] < Height)
		{
			o->Position[2] = Height;
			o->Gravity = -o->Gravity*0.4f;
		}
		VectorAdd(o->Angle, o->Direction, o->Angle);
		VectorScale(o->Direction, 0.8f, o->Direction);
	}
}

void DeadCharacterBuff(OBJECT* o)
{
	g_CharacterUnRegisterBuff(o, eDeBuff_Stun);
	g_CharacterUnRegisterBuff(o, eBuff_Cloaking);
	g_CharacterUnRegisterBuff(o, eDeBuff_Harden);
	g_CharacterUnRegisterBuff(o, eDeBuff_Sleep);
}

void DeadCharacter(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (c->Dead <= 0) return;

	DeadCharacterBuff(o);

	if (o->Type == MODEL_MONSTER01 + 32 && GET_WORLD == WD_7ATLANSE)
	{
		c->Rot += 0.05f;
	}
	else if (o->Type == MODEL_MONSTER01 + 64)
	{
		if (o->LifeTime >= 100) c->Rot += 0.01f;
		else c->Rot += 0.02f;
	}
	else
		c->Rot += 0.02f;
	mu_float RotTime = 1.0f;
	if (c->Rot >= RotTime)
	{
		if (o->Type != MODEL_MONSTER01 + 116)
		{
			o->Alpha = 1.0f - (c->Rot - RotTime);
		}

		if (o->Alpha >= 0.01f)
			o->Position[2] -= 0.4f;
		else if (c != Hero)
		{
			o->Flags.Clear(OBJECT::eOF_LIVE);
			c->m_byDieType = 0;
			o->Flags.Clear(OBJECT::eOF_ACTIONSTART);
			o->SpecialSkillTick = 0;
		}
		DeleteCloth(c, o);
	}

	if (InBloodCastle() == true && o->Flags.Get(OBJECT::eOF_ACTIONSTART) == true)
	{
		FallingCharacter(c, o);
	}
	else if (InChaosCastle() == true)
	{
		mu_int32 startDeadTime = 25;
		if (o->Flags.Get(OBJECT::eOF_ACTIONSTART) == true)
		{
			FallingCharacter(c, o);
			startDeadTime = 15;
		}
		if (c->Dead <= startDeadTime && c->Dead >= startDeadTime - 10 && (c->Dead % 2))
		{
			EVector3 Position;

			VectorCopy(o->Position, Position);

			Position[0] += GetLargeRand(160) - 80.0f;
			Position[1] += GetLargeRand(160) - 80.0f;
			Position[2] += GetLargeRand(160) - 80.0f + 50.0f;
			CreateBomb(ThreadIndex, Position, true);
		}
	}
	else
	{
		switch (c->m_byDieType)
		{
		case AT_SKILL_BLAST_HELL:
		case AT_SKILL_COMBO:
			if (c->Dead < 15)
			{
				o->Direction[1] -= o->Velocity;
				o->Velocity += 1.0f;

				if (o->Direction[1] < 0)
				{
					o->Direction[1] = 0.0f;
				}

				MoveParticle(o, o->HeadAngle);
			}

			if (c->Dead <= 30 && c->m_byDieType == AT_SKILL_BLAST_HELL)
			{
				EVector3 Light, p, Position;
				Vector(0.3f, 0.3f, 1.0f, Light);
				Vector(0.0f, 0.0f, 0.0f, p);
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(GetLargeRand(32))], p, Position, true);
					CreateParticle(ThreadIndex, BITMAP_LIGHT, Position, o->Angle, Light, 5, 0.5f + (GetLargeRand(100)) / 50.0f);
				}
			}
			break;
		}
	}
	if (g_CurrentScene == MAIN_SCENE &&
		(GET_WORLD == WD_7ATLANSE
		|| GET_WORLD == WD_67DOPPLEGANGER3))
	{
		for (mu_int32 i = 0; i < 4; ++i)
		{
			EVector3 Position;
			Vector((mu_float)(GetLargeRand(128) - 64), (mu_float)(GetLargeRand(128) - 64), (mu_float)(GetLargeRand(256)), Position);
			VectorAdd(Position, o->Position, Position);
			CreateParticle(ThreadIndex, BITMAP_BUBBLE, Position, o->Angle, o->Light);
		}
	}
}

static mu_int32 g_iOldPositionX = 0;
static mu_int32 g_iOldPositionY = 0;
static mu_float g_fStopTime = 0.0f;

void CalcStopTime()
{
	if (InChaosCastle() == false) return;

	if ((Hero->PositionX) != g_iOldPositionX || (Hero->PositionY) != g_iOldPositionY)
	{
		g_iOldPositionX = (Hero->PositionX);
		g_iOldPositionY = (Hero->PositionY);

		g_fStopTime = FWorldTime;
		return;
	}

	mu_float fStopTime = ((FWorldTime - g_fStopTime) / CLOCKS_PER_SEC);
	if (fStopTime >= 10)
	{
		mu_int32 index = TERRAIN_INDEX_REPEAT(g_iOldPositionX, g_iOldPositionY);

		if ((TerrainWall[index] & TW_NOGROUND) == TW_NOGROUND)
		{
#if 0 // FIX
			SendRequestCheckPosition(g_iOldPositionX, g_iOldPositionY);
#endif
		}
		g_fStopTime = FWorldTime;
	}
}

void HeroAttributeCalc(CHARACTER* c)
{
	if (c != Hero) return;

	if (CharacterAttribute->AbilityTime[0] > 0)
	{
		--CharacterAttribute->AbilityTime[0];
	}
	if (CharacterAttribute->AbilityTime[0] == 0)
	{
		CharacterAttribute->Ability &= (~ABILITY_FAST_ATTACK_SPEED);

		CharacterMachine->CalculateAttackSpeed();
	}
	if (CharacterAttribute->AbilityTime[1] > 0)
	{
		--CharacterAttribute->AbilityTime[1];
	}
	if (CharacterAttribute->AbilityTime[1] == 0)
	{
		CharacterAttribute->Ability &= (~ABILITY_PLUS_DAMAGE);

		g_csItemOption.ClearListOnOff();
		CharacterMachine->CalculateDamage();
		CharacterMachine->CalculateMagicDamage();
		CharacterMachine->CalculateCurseDamage();
	}
	if (CharacterAttribute->AbilityTime[2] > 0)
	{
		--CharacterAttribute->AbilityTime[2];
	}
	if (CharacterAttribute->AbilityTime[2] == 0)
	{
		CharacterAttribute->Ability &= (~ABILITY_FAST_ATTACK_SPEED2);
		CharacterMachine->CalculateAttackSpeed();
	}
}

void OnlyNpcChatProcess(CHARACTER* c, OBJECT* o)
{
	if (o->Kind == KIND_NPC && GetLargeRand(512) == 0)
	{
		switch (o->Type)
		{
		case MODEL_MERCHANT_GIRL:
			if (battleCastle::InBattleCastle() == false)
			{
				g_pSimpleChat->Create(c->ID, GlobalText[1974], c);
			}
			break;
		case MODEL_ELF_WIZARD:
			g_pSimpleChat->Create(c->ID, GlobalText[1975], c);
			break;
		case MODEL_MASTER:
			g_pSimpleChat->Create(c->ID, GlobalText[1976], c);
			break;
		case MODEL_PLAYER:
			if (c->MonsterIndex == 257)
				g_pSimpleChat->Create(c->ID, GlobalText[1827], c);
			break;
		}
	}
}

void PlayerNpcStopAnimationSetting(CHARACTER* c, OBJECT* o)
{
	mu_int32 action = GetLargeRand(100);

	if (o->CurrentAction != PLAYER_STOP_MALE)
	{
		SetAction(&c->Object, PLAYER_STOP_MALE);
	}
	else
	{
		if (action<80)
		{
			SetAction(&c->Object, PLAYER_STOP_MALE);
		}
		else if (action<85)
		{
			SetAction(&c->Object, PLAYER_CLAP1);
		}
		else if (action<90)
		{
			SetAction(&c->Object, PLAYER_CHEER1);
		}
		else if (action<95)
		{
			SetAction(&c->Object, PLAYER_SEE1);
		}
		else if (action<100)
		{
			SetAction(&c->Object, PLAYER_UNKNOWN1);
		}

		mu_int32 TextIndex = 0;
		if (GET_WORLD == WD_2DEVIAS)
		{
			mu_int32 iTextIndices[] = { 904, 905 };
			mu_int32 iNumTextIndices = 2;

			mu_int32 s_iTextIndex = iTextIndices[0];
			if (o->CurrentAction != o->PriorAction)
				s_iTextIndex = iTextIndices[GetLargeRand(iNumTextIndices)];
			TextIndex = s_iTextIndex;
		}
		else if (GET_WORLD == WD_0LORENCIA)
		{
			mu_int32 iTextIndices[] = { 823 };
			mu_int32 iNumTextIndices = 1;

			mu_int32 s_iTextIndex = iTextIndices[0];
			if (o->CurrentAction != o->PriorAction)
				s_iTextIndex = iTextIndices[GetLargeRand(iNumTextIndices)];
			TextIndex = s_iTextIndex;
		}

		mu_text szText[512];
		mu_sprintf_s(szText, mu_countof(szText), GlobalText[TextIndex]);
		g_pSimpleChat->Create(c->ID, szText, c);
	}
}

void PlayerStopAnimationSetting(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o)
{
	if (o->CurrentAction == PLAYER_DIE1 || o->CurrentAction == PLAYER_DIE2)
	{
		if (!c->Blood)
		{
			c->Blood = true;
			CreateBlood(ThreadIndex, o);
		}
		return;
	}

	if (o->CurrentAction < PLAYER_WALK_MALE ||
		(o->CurrentAction >= PLAYER_PROVOCATION && o->CurrentAction <= PLAYER_CHEERS) ||
		(o->CurrentAction >= PLAYER_IDLE1_DARKHORSE && o->CurrentAction <= PLAYER_IDLE2_DARKHORSE) ||
		(o->CurrentAction >= PLAYER_ATTACK_FIST && o->CurrentAction <= PLAYER_SHOCK
			&& o->CurrentAction != PLAYER_WALK_TWO_HAND_SWORD_TWO && o->CurrentAction != PLAYER_RUN_TWO_HAND_SWORD_TWO
			&& o->CurrentAction != PLAYER_FLY_RIDE && o->CurrentAction != PLAYER_FLY_RIDE_WEAPON
			&& o->CurrentAction != PLAYER_SKILL_HELL_BEGIN
			&& o->CurrentAction != PLAYER_DARKLORD_WALK && o->CurrentAction != PLAYER_RUN_RIDE_HORSE
			&& (o->CurrentAction < PLAYER_FENRIR_RUN || o->CurrentAction > PLAYER_FENRIR_RUN_ONE_LEFT_ELF)
			&& o->CurrentAction != PLAYER_RECOVER_SKILL
			))
	{
		if (o->CurrentAction != PLAYER_POINT_DANCE)
			SetPlayerStop(ThreadIndex, c);
	}

	if (o->CurrentAction == PLAYER_SKILL_HELL_BEGIN)
	{
		o->AnimationFrame = 0;
	}
}

void EtcStopAnimationSetting(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o)
{
	if (o->Type == MODEL_WARCRAFT)
	{
		o->CurrentAction = 1;
	}
	else if (battleCastle::StopBattleCastleMonster(c, o) == true)
	{
		CharacterAnimation(ThreadIndex, c, o);
		return;
	}
	else if (GET_WORLD == WD_1DUNGEON && o->Type == 40)
	{
		SetAction(&c->Object, 0);
	}
	else if (o->Type >= MODEL_MONSTER01 && o->Type < MODEL_MONSTER_END)
	{
		if (o->CurrentAction == MONSTER01_DIE)
		{
			if (!c->Blood)
			{
				c->Blood = true;
				CreateBlood(ThreadIndex, o);
			}
			return;
		}
		else if (o->CurrentAction == MONSTER01_STOP2 || o->CurrentAction == MONSTER01_SHOCK
			|| o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2
			|| o->CurrentAction == MONSTER01_ATTACK3 || o->CurrentAction == MONSTER01_ATTACK4
			|| o->CurrentAction == MONSTER01_ATTACK5
			)
		{
			SetAction(o, MONSTER01_STOP1);
		}

		if (o->CurrentAction == MONSTER01_APEAR
			&& (o->Type == MODEL_MONSTER01 + 118 || o->Type == MODEL_MONSTER01 + 119
				|| o->Type == MODEL_MONSTER01 + 150
				))
		{
			if (o->Type == MODEL_MONSTER01 + 150)
			{
				o->CurrentAction = MONSTER01_STOP1;
			}

			SetAction(o, MONSTER01_STOP1);
		}
	}
	else
	{
		switch (o->Type)
		{
		case MODEL_WEDDING_NPC:
			if ((GetLargeRand(16) < 4) && o->SubType == 0)
			{
				SetAction(o, 1);
				o->SubType = 1;
			}
			else
			{
				SetAction(o, 0);
				o->SubType = 0;
			}
			break;
		case MODEL_SMITH:
		case MODEL_SCIENTIST:
			if (GetLargeRand(16) < 12)
				SetAction(o, 0);
			else
				SetAction(o, GetLargeRand(2) + 1);
			break;
		case MODEL_FACE:
		case MODEL_FACE + 1:
		case MODEL_FACE + 2:
		case MODEL_FACE + 3:
		case MODEL_FACE + 4:
		case MODEL_FACE + 5:
			break;
		case MODEL_ELBELAND_SILVIA:
		case MODEL_ELBELAND_RHEA:
			if (GetLargeRand(5) < 4 || o->CurrentAction == 1)
				SetAction(o, 0);
			else
				SetAction(o, 1);
			break;
		case MODEL_NPC_DEVIN:
			if (GetLargeRand(5) < 4)
				SetAction(o, 0);
			else
				SetAction(o, 1);
			break;
		case MODEL_SEED_MASTER:
			if (GetLargeRand(3) < 2 || o->CurrentAction != 0)
				SetAction(o, 0);
			else
			{
				SetAction(o, GetLargeRand(3) + 1);
			}
			break;
		case MODEL_SEED_INVESTIGATOR:
			if (GetLargeRand(3) < 2 || o->CurrentAction == 1)
				SetAction(o, 0);
			else
				SetAction(o, 1);
			break;
		case MODEL_LITTLESANTA:
		case MODEL_LITTLESANTA + 1:
		case MODEL_LITTLESANTA + 2:
		case MODEL_LITTLESANTA + 3:
			if (GetLargeRand(5) < 2)
				SetAction(o, 0);
			else
				SetAction(o, GetLargeRand(3) + 2);
			break;
		case MODEL_LITTLESANTA + 4:
		case MODEL_LITTLESANTA + 5:
		case MODEL_LITTLESANTA + 6:
		case MODEL_LITTLESANTA + 7:
			if (GetLargeRand(5) < 2)
				SetAction(o, 1);
			else
				SetAction(o, GetLargeRand(3) + 2);
			break;
		case MODEL_XMAS2008_SANTA_NPC:
			if (GetLargeRand(3) < 2 || (o->CurrentAction == 1 || o->CurrentAction == 2))
			{
				SetAction(o, 0);
			}
			else
			{
				SetAction(o, GetLargeRand(2) + 1);
			}
			break;
		case MODEL_XMAS2008_SNOWMAN_NPC:
			SetAction(o, 0);
			break;
		case MODEL_GAMBLE_NPC_MOSS:
			if (GetLargeRand(5) < 4 || o->CurrentAction == 1)
			{
				SetAction(o, 0);
			}
			else
			{
				SetAction(o, 1);
			}
			break;
		case MODAL_GENS_NPC_DUPRIAN:
		case MODAL_GENS_NPC_BARNERT:
			if (GetLargeRand(5) < 4)
				SetAction(o, 0);
			else
				SetAction(o, GetLargeRand(2) + 1);
			break;
		case MODEL_UNITEDMARKETPLACE_RAUL:
		case MODEL_UNITEDMARKETPLACE_JULIA:
			if (GetLargeRand(5) < 4)
				SetAction(o, 0);
			else
				SetAction(o, GetLargeRand(2) + 1);
			break;
		case MODEL_UNITEDMARKETPLACE_CHRISTIN:
			if (GetLargeRand(5) < 3)
				SetAction(o, 0);
			else
				SetAction(o, GetLargeRand(2) + 1);
			break;
		default:
			SetAction(o, GetLargeRand(2));
			break;
		}
	}
}

void AnimationCharacter(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	mu_boolean bEventNpc = false;
	if (o->Kind == KIND_NPC && (GET_WORLD == WD_0LORENCIA || GET_WORLD == WD_2DEVIAS) && o->Type == MODEL_PLAYER && (o->SubType >= MODEL_SKELETON1&&o->SubType <= MODEL_SKELETON3))
	{
		if (GET_WORLD == WD_0LORENCIA)
		{
			Vector(0.0f, 0.0f, 90.0f, o->Angle);
		}
		else
		{
			Vector(0.0f, 0.0f, 0.0f, o->Angle);
		}

		bEventNpc = true;
	}

	OnlyNpcChatProcess(c, o);

	mu_boolean Play = CharacterAnimation(ThreadIndex, c, o);

	if (!Play)
	{
		c->LongRangeAttack = -1;
		c->Foot[0] = false;
		c->Foot[1] = false;
		if (o->Type == MODEL_PLAYER)
		{
			if (bEventNpc)
			{
				PlayerNpcStopAnimationSetting(c, o);
			}
			else
			{
				PlayerStopAnimationSetting(ThreadIndex, c, o);
			}
		}
		else
		{
			EtcStopAnimationSetting(ThreadIndex, c, o);

			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				PlayMonsterSound(ThreadIndex, o);
		}

		if (o->Type == MODEL_WARCRAFT)
		{
			o->AnimationFrame = 8.0f;
		}
	}

	switch (o->Type)
	{
	case MODEL_DEVIAS_TRADER:
		if (b->CurrentAnimationFrame(ThreadIndex) == b->_AnimationHeader[o->CurrentAction].FrameCount - 1)
		{
			if (GetLargeRand(32) == 0)
				SetAction(o, 1);
			else
				SetAction(o, 0);
		}
		break;
	case MODEL_MONSTER01 + 128:
		if (o->CurrentAction <= 1 && b->CurrentAnimationFrame(ThreadIndex) == b->_AnimationHeader[o->CurrentAction].FrameCount - 1)
		{
			if (GetLargeRand(10) == 0)
				SetAction(o, 1);
			else
				SetAction(o, 0);
		}
		break;
	}
}

void CreateWeaponBlur(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (o->AnimationFrame >= 3.0f
		|| (o->CurrentAction == PLAYER_ATTACK_TWO_HAND_SWORD_TWO)
		)
	{
		mu_int32 Hand = 0;
		mu_int32 Hand2 = 0;
		if (o->Type == MODEL_PLAYER)
		{
			if (o->CurrentAction == PLAYER_ATTACK_SWORD_LEFT1 || o->CurrentAction == PLAYER_ATTACK_SWORD_LEFT2)
			{
				Hand = 1;
				Hand2 = 1;
			}
		}
		mu_int32 BlurType = 0;
		mu_int32 BlurMapping = 0;
		mu_int32 Type = c->Weapon[Hand].Type;
		mu_int32 Level = c->Weapon[Hand].Level;
		if (o->Type == MODEL_PLAYER)
		{
			if (o->CurrentAction == PLAYER_ATTACK_ONE_FLASH || o->CurrentAction == PLAYER_ATTACK_RUSH)
			{
				BlurType = 1;
				BlurMapping = 2;
			}
			else if (o->CurrentAction == PLAYER_ATTACK_SKILL_SWORD2 || o->CurrentAction == PLAYER_ATTACK_SKILL_SWORD3 || o->CurrentAction == PLAYER_ATTACK_SKILL_SWORD4)
			{
				BlurType = 1;
				if (Type == MODEL_SWORD + 14 || Type == MODEL_SWORD + 21 || Type == MODEL_SWORD + 31)
					BlurMapping = 1;
				else
					BlurMapping = 2;
			}
			else if (o->CurrentAction == PLAYER_ATTACK_STRIKE)
			{
				BlurType = 1;
				BlurMapping = 2;
			}
			else if (o->CurrentAction == PLAYER_SKILL_LIGHTNING_ORB
				|| o->CurrentAction == PLAYER_SKILL_LIGHTNING_ORB_UNI
				|| o->CurrentAction == PLAYER_SKILL_LIGHTNING_ORB_DINO
				|| o->CurrentAction == PLAYER_SKILL_LIGHTNING_ORB_FENRIR)
			{
				BlurType = 1;
				BlurMapping = 1;
			}
			else if (o->CurrentAction == PLAYER_SKILL_BLOW_OF_DESTRUCTION && o->AnimationFrame >= 2.0f && o->AnimationFrame <= 8.0f)
			{
				BlurType = 1;
				BlurMapping = 2;
			}
			else if (o->CurrentAction == PLAYER_ATTACK_SKILL_SWORD5)
			{
				BlurType = 1;
				if (Type == MODEL_MACE + 5)
					BlurMapping = 1;
				else
					BlurMapping = 2;
			}
			else if (Type >= MODEL_SWORD && Type < MODEL_SWORD + MAX_ITEM_INDEX)
			{
				if ((o->CurrentAction >= PLAYER_ATTACK_SWORD_RIGHT1 && o->CurrentAction <= PLAYER_ATTACK_TWO_HAND_SWORD3)
					|| o->CurrentAction == PLAYER_ATTACK_TWO_HAND_SWORD_TWO
					)
				{
					BlurType = 1;
					if (Type == MODEL_SWORD + 17)
					{
						BlurMapping = 6;
					}
					else if (o->CurrentAction == PLAYER_ATTACK_TWO_HAND_SWORD3
						|| o->CurrentAction == PLAYER_ATTACK_TWO_HAND_SWORD_TWO)
					{
						if (Type == MODEL_SWORD + 25)
							BlurMapping = 2;
						else
							BlurMapping = 1;
					}
				}
			}
			else if (Type == MODEL_AXE + 3 || (Type >= MODEL_AXE + 5 && Type < MODEL_MACE + MAX_ITEM_INDEX))
			{
				if (o->CurrentAction >= PLAYER_ATTACK_SKILL_SWORD1 && o->CurrentAction <= PLAYER_ATTACK_SKILL_SWORD5)
				{
					BlurType = 1;
					BlurMapping = 2;
				}
			}
			else if (Type >= MODEL_SPEAR && Type < MODEL_SPEAR + MAX_ITEM_INDEX)
			{
				if (o->CurrentAction >= PLAYER_ATTACK_SPEAR1 && o->CurrentAction <= PLAYER_ATTACK_SCYTHE3)
				{
					BlurType = 3;
					if (Type == MODEL_SPEAR + 10)
					{
						BlurType = 1;
						BlurMapping = 0;
					}
					else if (o->CurrentAction == PLAYER_ATTACK_SCYTHE3)
						BlurMapping = 1;
				}
			}
		}
		else
		{
			if (c->MonsterIndex == 71 || c->MonsterIndex == 74 || c->MonsterIndex == 301)
			{
				if (o->CurrentAction >= MONSTER01_ATTACK1 && o->CurrentAction <= MONSTER01_ATTACK2)
				{
					BlurType = 1;
					BlurMapping = 6;
				}
			}
			else if (o->Type == MODEL_MONSTER01 + 66)
			{
				if (o->CurrentAction == MONSTER01_ATTACK1)
				{
					BlurType = 5;
					BlurMapping = 2;
					Hand = 0;
					Hand2 = 1;
				}
			}
			else if (o->Type == MODEL_MONSTER01 + 67)
			{
				if (o->CurrentAction >= MONSTER01_ATTACK1 && o->CurrentAction <= MONSTER01_ATTACK2)
				{
					BlurType = 4;
					BlurMapping = 0;
					Level = 99;
				}
			}
			else if (o->Type == MODEL_MONSTER01 + 69)
			{
				if (o->CurrentAction >= MONSTER01_ATTACK1 && o->CurrentAction <= MONSTER01_ATTACK2)
				{
					if (o->SubType == 9)
					{
						BlurType = 1;
						BlurMapping = 2;
					}
					else
					{
						BlurType = 1;
						BlurMapping = 0;
						Level = 99;
						Type = 0;
					}
				}
			}
			else if (Type >= MODEL_SWORD && Type < MODEL_SWORD + MAX_ITEM_INDEX)
			{
				if (o->CurrentAction >= MONSTER01_ATTACK1 && o->CurrentAction <= MONSTER01_ATTACK2)
					BlurType = 1;
			}
		}
		if (BlurType > 0)
		{
			EVector3  Light;
			EVector3  Pos1, Pos2;
			EVector3  p, p2;
			switch (BlurType)
			{
			case 1:Vector(0.0f, -20.0f, 0.0f, Pos1); break;
			case 2:Vector(0.0f, -80.0f, 0.0f, Pos1); break;
			case 3:Vector(0.0f, -100.0f, 0.0f, Pos1); break;
			}
			Vector(0.0f, -120.0f, 0.0f, Pos2);

			if (BlurType == 4)
			{
				Vector(0.0f, 0.0f, 0.0f, Pos1);
				Vector(0.0f, -200.0f, 0.0f, Pos2);
			}
			else if (BlurType == 5)
			{
				Vector(0.0f, 0.0f, 0.0f, Pos1);
				Vector(0.0f, -20.0f, 0.0f, Pos2);
			}

			if (Type == MODEL_SWORD + 13 || Type == MODEL_MACE + 6 || Type == MODEL_SPEAR + 9)
			{
				Vector(1.0f, 0.2f, 0.2f, Light);
			}
			else if (Level == 99)
			{
				Vector(0.3f, 0.2f, 1.0f, Light);
			}
			else if (BlurMapping == 0)
			{
				if (Level >= 7)
				{
					Vector(1.0f, 0.6f, 0.2f, Light);
				}
				else if (Level >= 5)
				{
					Vector(0.2f, 0.4f, 1.0f, Light);
				}
				else if (Level >= 3)
				{
					Vector(1.0f, 0.2f, 0.2f, Light);
				}
				else
				{
					Vector(0.8f, 0.8f, 0.8f, Light);
				}
			}
			else
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
			}

			if ((o->Type != MODEL_PLAYER || Type == MODEL_SWORD + 3 || Type == MODEL_SWORD + 6 || Type == MODEL_SWORD + 9 || Type == MODEL_SWORD + 11 || Type == MODEL_SPEAR + 4)
				&& o->Type != MODEL_MONSTER01 + 66 && o->Type != MODEL_MONSTER01 + 67 && o->Type != MODEL_MONSTER01 + 69
				)
			{
				b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[Hand].LinkBone], Pos1, p, true);
				b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[Hand2].LinkBone], Pos2, p2, true);
				CreateBlur(c, p, p2, Light, BlurMapping);
			}
			else
			{
				mu_float playSpeed = GetPlaySpeed(o, o->Type, b->CurrentAction(ThreadIndex));
				mu_float inter = 10.0f;
				mu_float animationFrame = o->AnimationFrame - playSpeed;
				mu_float priorAnimationFrame = o->PriorAnimationFrame;
				mu_float animationSpeed = playSpeed / inter;

				for (mu_int32 i = 0; i < (mu_int32)(inter); ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, animationFrame, priorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[c->Weapon[Hand].LinkBone], Pos1, p, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[c->Weapon[Hand2].LinkBone], Pos2, p2, false);

					if (o->Type == MODEL_MONSTER01 + 66 && i % 2)
					{
						CreateParticle(ThreadIndex, BITMAP_FIRE + 3, p2, o->Angle, Light, 12);
					}
					if (c->Weapon[0].Type != -1 || c->Weapon[1].Type != -1)
						CreateBlur(c, p, p2, Light, BlurMapping, true);

					animationFrame += animationSpeed;
				}
			}
		}

		TheMapProcess().MoveBlurEffect(ThreadIndex, c, o, b);
	}
	else
	{
		VectorCopy(o->Position, o->StartPosition);
	}
}

void PlayWalkSound()
{
	OBJECT *o = &Hero->Object;
	if (o->CurrentAction == PLAYER_FLY || o->CurrentAction == PLAYER_FLY_CROSSBOW)
	{
	}
	else
	{
		if (GET_WORLD == WD_2DEVIAS && (HeroTile != 3 && HeroTile < 10))
		{
			PlayBackground(SOUND_HUMAN_WALK_SNOW);
		}
		else if (GET_WORLD == WD_0LORENCIA && HeroTile == 0)
		{
			PlayBackground(SOUND_HUMAN_WALK_GRASS);
		}
		else if (GET_WORLD == WD_3NORIA && HeroTile == 0)
		{
			PlayBackground(SOUND_HUMAN_WALK_GRASS);
		}
		else if ((GET_WORLD == WD_7ATLANSE || InHellas()
			|| GET_WORLD == WD_67DOPPLEGANGER3
			) && !Hero->SafeZone)
		{
			PlayBackground(SOUND_HUMAN_WALK_SWIM);
		}
		else if (IsIceCity())
		{
			PlayBackground(SOUND_HUMAN_WALK_SNOW);
		}
		else if (IsSantaTown())
		{
			PlayBackground(SOUND_HUMAN_WALK_SNOW);
		}
		else
		{
			PlayBackground(SOUND_HUMAN_WALK_GROUND);
		}
	}
}

void MUTasking::MoveMonsterClient(const mu_uint32 ThreadIndex, CHARACTER *c, OBJECT *o)
{
	if (c->Dead == 0)
	{
		if (c->MonsterIndex == 384 || c->MonsterIndex == 383)
		{
			c->Movement = false;
		}

		if (!c->Movement)
		{
			if (c->EnabledInternalAction == INTERNAL_ACTION::IA_NORMAL ||
				c->EnabledInternalAction == INTERNAL_ACTION::IA_PATH)
			{
				c->EnabledInternalAction = INTERNAL_ACTION::IA_NONE;
				InternalViewportProcess(c->InternalActionPacket, true);
			}
		}
		else
		{
			if (o->Type == MODEL_MONSTER01 + 121 &&
				(o->CurrentAction == MONSTER01_ATTACK1
				|| o->CurrentAction == MONSTER01_ATTACK2
				|| o->CurrentAction == MONSTER01_ATTACK3
				|| o->CurrentAction == MONSTER01_ATTACK4
				|| o->CurrentAction == MONSTER01_ATTACK5))
			{
				c->Movement = true;
				SetAction(o, o->CurrentAction);
			}
			else
			{
				SetPlayerWalk(ThreadIndex, c);
			}

			if (MovePath(c))
			{
				c->Movement = false;
				SetPlayerStop(ThreadIndex, c);

				if (c != Hero)
				{
					c->Object.Angle[2] = c->TargetAngle;
				}
			}

			MoveCharacterPosition(c);
		}
	}
	else
	{
		if (o->Type == MODEL_MONSTER01 + 2)
		{
			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
		}
	}
}

void MUTasking::MoveCharacter(const mu_uint32 ThreadIndex, CHARACTER *c, OBJECT *o)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (o->Type == MODEL_WARCRAFT)
	{
		mu_text Text[100];
		mu_text ID[100];
		mu_sprintf_s(ID, mu_countof(ID), _T("") SPF_STRING _T(" ."), c->ID);
		mu_sprintf_s(Text, mu_countof(Text), GlobalText[1176], c->Level);
		mu_strcat(ID, mu_countof(ID), Text);

		AddObjectDescription(ID, o->Position);
	}

	MUModel *b = &MODELS::Data[o->Type];
	if(b->IsDataLoaded() == false)
	{
		b->RequestLoad();
	}

	VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));
	b->BodyScale(ThreadIndex) = o->Scale;
	b->CurrentAction(ThreadIndex) = o->CurrentAction;

	if (c == Hero)
	{
		CalcStopTime();
		HeroAttributeCalc(c);
	}

	PushingCharacter(ThreadIndex, c, o);
	DeadCharacter(ThreadIndex, c, o, b);
	Alpha(o);

	AnimationCharacter(ThreadIndex, c, o, b);

	if (c->Dead > 0)
	{
		++c->Dead;
		if (c->Dead >= 15)
		{
			SetPlayerDie(ThreadIndex, c);
		}
		if (InBloodCastle() && o->Flags.Get(OBJECT::eOF_ACTIONSTART) == true)
		{
			SetPlayerDie(ThreadIndex, c);
		}
	}

	EVector3 p(0.0f), Position;
	EVector3 Light(1.0f);

	if (battleCastle::InBattleCastle() == false &&
		o->m_byHurtByOneToOne > 0 &&
		b->IsDataLoaded() == true)
	{
		EVector3 pos1, pos2;

		Vector(0.0f, 0.0f, 0.0f, p);
		for (mu_int32 i = b->_BoneParentEnd; i < b->_BoneNonDummyEnd; ++i)
		{
			const mu_int32 iParent = b->_BoneRef[i].Parent;
			b->TransformPosition(ThreadIndex, o->BoneTransform[i], p, pos1, true);
			b->TransformPosition(ThreadIndex, o->BoneTransform[iParent], p, pos2, true);

			GetNearRandomPos(pos1, 20, pos1);
			GetNearRandomPos(pos2, 20, pos2);
			CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, pos1, pos2, o->Angle, 7, nullptr, 20.0f);
		}
		--o->m_byHurtByOneToOne;
	}

	if ((o->CurrentAction == PLAYER_ATTACK_TELEPORT ||
		o->CurrentAction == PLAYER_ATTACK_RIDE_TELEPORT ||
		o->CurrentAction == PLAYER_FENRIR_ATTACK_DARKLORD_TELEPORT) &&
		o->AnimationFrame > 5.5f &&
		b->IsDataLoaded() == true)
	{
		Vector(0.0f, 0.0f, 0.0f, p);
		Vector(0.3f, 0.5f, 1.0f, Light);
		b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(42)], p, Position, true);
		CreateParticle(ThreadIndex, BITMAP_LIGHT, Position, o->Angle, Light);
	}

	if (c->m_iDeleteTime > 0)
	{
		--c->m_iDeleteTime;
	}
	if (c->m_iDeleteTime != -128 && c->m_iDeleteTime <= 0)
	{
		c->m_iDeleteTime = -128;
		DeleteCharacter(c, o);
	}

	battleCastle::SetBuildTimeLocation(o);

	if(b->IsDataLoaded() == true)
	{
		CreateWeaponBlur(ThreadIndex, c, o, b);
	}

	switch (o->Type)
	{
	case MODEL_BALL:
		CreateFire(ThreadIndex, 0, o, 0.0f, 0.0f, 0.0f);
		break;
	}

	giPetManager::MovePet(ThreadIndex, c);

	if (c->Dead > 0)
	{
		if (g_isCharacterBuff(o, eBuff_BlessPotion)) g_CharacterUnRegisterBuff(o, eBuff_BlessPotion);
		if (g_isCharacterBuff(o, eBuff_SoulPotion)) g_CharacterUnRegisterBuff(o, eBuff_SoulPotion);
	}
}

void MUTasking::MoveCharacterVisual(const mu_uint32 ThreadIndex, CHARACTER *c, OBJECT *o, mu_boolean Visible)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];
	MUModel *b = &MODELS::Data[o->Type];
	if (b->IsDataLoaded() == false)
	{
		b->RequestLoad();
		VectorCopy(o->BoundingBoxMin, o->OBB.StartPos);
		o->OBB.XAxis[0] = (o->BoundingBoxMax[0] - o->BoundingBoxMin[0]);
		o->OBB.YAxis[1] = (o->BoundingBoxMax[1] - o->BoundingBoxMin[1]);
		o->OBB.ZAxis[2] = (o->BoundingBoxMax[2] - o->BoundingBoxMin[2]);
		VectorAdd(o->OBB.StartPos, o->Position, o->OBB.StartPos);
		o->OBB.XAxis[1] = 0.0f;
		o->OBB.XAxis[2] = 0.0f;
		o->OBB.YAxis[0] = 0.0f;
		o->OBB.YAxis[2] = 0.0f;
		o->OBB.ZAxis[0] = 0.0f;
		o->OBB.ZAxis[1] = 0.0f;
		return;
	}

	if (b->_AnimationCount == 0)
	{
		VectorCopy(o->BoundingBoxMin, o->OBB.StartPos);
		o->OBB.XAxis[0] = (o->BoundingBoxMax[0] - o->BoundingBoxMin[0]);
		o->OBB.YAxis[1] = (o->BoundingBoxMax[1] - o->BoundingBoxMin[1]);
		o->OBB.ZAxis[2] = (o->BoundingBoxMax[2] - o->BoundingBoxMin[2]);
		VectorAdd(o->OBB.StartPos, o->Position, o->OBB.StartPos);
		o->OBB.XAxis[1] = 0.0f;
		o->OBB.XAxis[2] = 0.0f;
		o->OBB.YAxis[0] = 0.0f;
		o->OBB.YAxis[2] = 0.0f;
		o->OBB.ZAxis[0] = 0.0f;
		o->OBB.ZAxis[1] = 0.0f;
		return;
	}

	o->BoneTransform = ThreadInfo.BoneTransform;
	b->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle, false, false);

	VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));
	b->BodyScale(ThreadIndex) = o->Scale;
	b->CurrentAction(ThreadIndex) = o->CurrentAction;

	if (Visible)
	{
		mu_int32 Index = TERRAIN_INDEX_REPEAT((c->PositionX), (c->PositionY));
		if ((TerrainWall[Index] & TW_SAFEZONE) == TW_SAFEZONE)
			c->SafeZone = true;
		else
			c->SafeZone = false;

		if (!g_isCharacterBuff(o, eDeBuff_Harden) && !g_isCharacterBuff(o, eDeBuff_Stun)
			&& !g_isCharacterBuff(o, eDeBuff_Sleep))
		{
			if (o->Type != MODEL_PLAYER)
				MoveHead(c);
			if (c != Hero && c->Dead == 0 && GetLargeRand(32) == 0)
			{
				o->HeadTargetAngle[0] = (mu_float)(GetLargeRand(128) - 64);
				o->HeadTargetAngle[1] = (mu_float)(GetLargeRand(32) - 16);
				for (mu_int32 i = 0; i<2; ++i)
					if (o->HeadTargetAngle[i] < 0) o->HeadTargetAngle[i] += 360.0f;
			}

			for (mu_int32 j = 0; j<2; ++j)
				o->HeadAngle[j] = TurnAngle2(o->HeadAngle[j], o->HeadTargetAngle[j], FarAngle(o->HeadAngle[j], o->HeadTargetAngle[j])*0.2f);
		}

		EVector3 p, Position;
		EVector3 Light;
		mu_float Luminosity = 0.8f;
		if (c->Appear > 0)
		{
			--c->Appear;
			for (mu_int32 i = 0; i<20; ++i)
			{
				Vector(1.0f, 1.0f, 1.0f, o->Light);
				Vector(o->Position[0] + (mu_float)(GetLargeRand(64) - 32),
					o->Position[1] + (mu_float)(GetLargeRand(64) - 32),
					o->Position[2] + (mu_float)(GetLargeRand(32) - 16), Position);
				if (GetLargeRand(10) == 0)
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, Position, o->Angle, o->Light, 1);
				if (GetLargeRand(10) == 0)
					CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), o->Position, o->Angle, o->Light);
			}
		}
		if (c->PK < PVP_MURDERER2)
		{
			for (mu_int32 j = 0; j<2; ++j)
			{
				if (c->Weapon[j].Type == MODEL_SWORD + 12)
				{
					Vector(Luminosity, Luminosity*0.8f, Luminosity*0.5f, Light);
					AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
				}
				else if (c->Weapon[j].Type == MODEL_SWORD + 19 || c->Weapon[j].Type == MODEL_BOW + 18 || c->Weapon[j].Type == MODEL_STAFF + 10 || c->Weapon[j].Type == MODEL_MACE + 13)
				{
					Vector(Luminosity*0.8f, Luminosity*0.5f, Luminosity*0.3f, Light);
					AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
				}
			}
		}

		Vector(1.0f, 1.0f, 1.0f, Light);
		Vector(0.0f, 0.0f, 0.0f, p);
		Luminosity = (mu_float)(GetLargeRand(8) + 2)*0.1f;
		mu_boolean Smoke = false;
		switch (o->Type)
		{
		case MODEL_PLAYER:
			if (g_CurrentScene == MAIN_SCENE &&
				(GET_WORLD == WD_7ATLANSE
				 || GET_WORLD == WD_67DOPPLEGANGER3) &&
				IWorldTime % 10000 < 1000)
			{
				Vector(0.0f, 20.0f, -10.0f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->_BoneHead], p, Position, true);
				CreateParticle(ThreadIndex, BITMAP_BUBBLE, Position, o->Angle, Light);
			}
			Vector(1.0f, 1.0f, 1.0f, Light);
			Vector(-15.0f, 0.0f, 0.0f, p);
			if (InDevilSquare() == true)
			{
				if (GetLargeRand(4) == 0)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(26)], p, Position, true);
					CreateParticle(ThreadIndex, BITMAP_RAIN_CIRCLE + 1, Position, o->Angle, Light);
				}
				if (GetLargeRand(4) == 0)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(35)], p, Position, true);
					CreateParticle(ThreadIndex, BITMAP_RAIN_CIRCLE + 1, Position, o->Angle, Light);
				}
			}
			if (o->CurrentAction == PLAYER_SKILL_HELL_BEGIN || o->CurrentAction == PLAYER_SKILL_HELL_START)
			{
				Vector(0.3f, 0.3f, 1.0f, Light);
				const mu_uint32 maxBone = b->_BoneNonDummyEnd < 40 ? b->_BoneNonDummyEnd : 40;
				for (mu_int32 i = GetLargeRand(2); i < maxBone; i += 2)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[i], p, Position, true);

					mu_int32 skillCount = (mu_int32)((g_CurrentTime - o->SpecialSkillTick) / 500ull);
					if (skillCount > 12)
					{
						skillCount = 12;
					}

					for (mu_int32 j = 0; j < skillCount + 1; ++j)
					{
						CreateParticle(ThreadIndex, BITMAP_LIGHT, Position, o->Angle, Light, 6, 1.3f + (skillCount*0.08f));
					}
				}
				VectorCopy(o->Position, Position);
				CreateForce(ThreadIndex, o, Position);
			}

			if (o->CurrentAction == PLAYER_SKILL_HELL)
			{
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[GetLargeRand(b->_BoneNonDummyEnd)], p, Position, true);
					CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, Light);
				}
			}

			Vector(0.0f, -30.0f, 0.0f, p);
			b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[0].LinkBone], p, Position, true);
			break;

		case MODEL_MONSTER01 + 49:
			break;
		case MODEL_MONSTER01 + 50:
			break;
		case MODEL_MONSTER01 + 51:
			{
				EVector3 pos1, pos2;

				for (mu_int32 i = 2; i < 5; ++i)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(i)], p, pos1, true);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(i + 1)], p, pos2, true);
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, pos1, pos2, o->Angle, 7, nullptr, 14.0f);
				}
				for (mu_int32 i = 9; i < 11; ++i)
				{
					if (i == 9)
						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(2)], p, pos1, true);
					else
						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(i)], p, pos1, true);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(i + 1)], p, pos2, true);
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, pos1, pos2, o->Angle, 7, nullptr, 14.0f);
				}

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(2)], p, pos1, true);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(18)], p, pos2, true);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, pos1, pos2, o->Angle, 7, nullptr, 14.0f);

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(22)], p, pos1, true);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, pos2, pos1, o->Angle, 7, nullptr, 14.0f);

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(23)], p, pos2, true);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, pos1, pos2, o->Angle, 7, nullptr, 14.0f);

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(24)], p, pos1, true);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, pos2, pos1, o->Angle, 7, nullptr, 14.0f);

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(25)], p, pos2, true);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, pos1, pos2, o->Angle, 7, nullptr, 14.0f);

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(18)], p, pos1, true);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(31)], p, pos2, true);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, pos1, pos2, o->Angle, 7, nullptr, 14.0f);

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(32)], p, pos1, true);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, pos2, pos1, o->Angle, 7, nullptr, 14.0f);

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(33)], p, pos2, true);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, pos1, pos2, o->Angle, 7, nullptr, 14.0f);

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(34)], p, pos1, true);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, pos2, pos1, o->Angle, 7, nullptr, 14.0f);
			}
			break;
		case MODEL_MONSTER01 + 52:
			o->BlendMeshTexCoordU = -(mu_float)((mu_int32)(FWorldTime) % 10000)*0.0004f;
			break;
		case MODEL_MONSTER01 + 55:
			o->BlendMeshTexCoordV = (mu_float)((mu_int32)(FWorldTime) % 10000)*0.0001f;
			break;
		case MODEL_MONSTER01 + 54:
			break;
		case MODEL_MONSTER01 + 48:
			if (0 == ((mu_int32)GetLargeRand(5)))
			{
				Position[0] = o->Position[0] + ((GetLargeRand(21)) - 10) * ((mu_float)TERRAIN_SIZE / 70);
				Position[1] = o->Position[1] + ((GetLargeRand(21)) - 10) * ((mu_float)TERRAIN_SIZE / 70);
				CreatePointer(BITMAP_BLOOD, Position, o->Angle[0], o->Light, 0.65f);
			}
			break;

		case MODEL_MONSTER01 + 88:
		case MODEL_MONSTER01 + 45:
			MoveEye(ThreadIndex, o, b, 8, 9);
			MonsterMoveSandSmoke(ThreadIndex, o);
			break;
		case MODEL_MONSTER01 + 44:
			MoveEye(ThreadIndex, o, b, 8, 9);
			if (c->MonsterIndex == 63)
			{
				mu_uint8  body[2] = { 30,0 };
				mu_uint8  head = 1;
				EVector3  vec[35];
				EVector3  angle;
				EVector3  dist;
				EVector3  p;

				Vector(0.0f, 0.0f, 0.0f, angle);
				Vector(0.0f, 0.0f, 0.0f, p);

				for (mu_int32 i = 0; i<35; ++i)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(vec_list[i])], p, vec[i], true);
				}

				mu_uint8 start, end;
				mu_float scale = 1.0f;
				for (mu_int32 i = 0; i<15; ++i)
				{
					if (i >= 11)
					{
						scale = 0.5f;
					}

					start = wingLeft[i][0];
					end = wingLeft[i][1];

					dist[0] = MoveHumming(vec[end], angle, vec[start], 360.0f);
					CreateParticle(ThreadIndex, BITMAP_FLAME, vec[start], angle, dist, 2, scale);

					start = wingRight[i][0];
					end = wingRight[i][1];

					dist[0] = MoveHumming(vec[end], angle, vec[start], 360.0f);
					CreateParticle(ThreadIndex, BITMAP_FLAME, vec[start], angle, dist, 2, scale);
				}

				for (mu_int32 i = 0; i<4; ++i)
				{
					start = arm_leg_Left[i][0];
					end = arm_leg_Left[i][1];

					dist[0] = MoveHumming(vec[end], angle, vec[start], 360.0f);
					CreateParticle(ThreadIndex, BITMAP_FLAME, vec[start], angle, dist, 2, 0.6f);

					start = arm_leg_Right[i][0];
					end = arm_leg_Right[i][1];

					dist[0] = MoveHumming(vec[end], angle, vec[start], 360.0f);
					CreateParticle(ThreadIndex, BITMAP_FLAME, vec[start], angle, dist, 2, 0.6f);
				}

				if (IWorldTime % 2 == 0)
				{
					start = body[0];
					end = body[1];

					dist[0] = MoveHumming(vec[end], angle, vec[start], 360.0f);
					CreateParticle(ThreadIndex, BITMAP_FLAME, vec[start], angle, dist, 2, 1.3f);

					CreateParticle(ThreadIndex, BITMAP_FLAME, vec[head], angle, dist, 3, 0.5f);
				}

				Vector(-1.3f, -1.3f, -1.3f, Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
			}
			else
			{
				EVector3  pos, angle;
				Vector(0.0f, 0.0f, 0.0f, angle);

				Luminosity = (mu_float)EMath::Sin(FWorldTime*0.002f)*0.3f + 0.7f;

				Vector(Luminosity, Luminosity*0.5f, Luminosity*0.5f, Light);

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(55)], p, pos, true);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(62)], p, Position, true);
				MoveHumming(pos, angle, Position, 360.0f);
				CreateParticle(ThreadIndex, BITMAP_FLAME, Position, angle, Light, 1, 0.2f);

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(70)], p, pos, true);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(77)], p, Position, true);
				MoveHumming(pos, angle, Position, 360.0f);
				CreateParticle(ThreadIndex, BITMAP_FLAME, Position, angle, Light, 1, 0.2f);

				MonsterMoveSandSmoke(ThreadIndex, o);
				MonsterDieSandSmoke(ThreadIndex, o);
			}
			break;
		case MODEL_MONSTER01 + 43:
			MoveEye(ThreadIndex, o, b, 11, 12);
			MonsterMoveSandSmoke(ThreadIndex, o);
			break;
		case MODEL_MONSTER01 + 42:
			MoveEye(ThreadIndex, o, b, 24, 25);
			if (o->SubType == 1)
			{
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(6)], p, Position, true);
				CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, Light);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(13)], p, Position, true);
				CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, Light);
				Vector(-1.3f, -1.3f, -1.3f, Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
			}
			else
			{
				MonsterMoveSandSmoke(ThreadIndex, o);
				MonsterDieSandSmoke(ThreadIndex, o);
			}
			break;
		case MODEL_MONSTER01 + 41:
			MoveEye(ThreadIndex, o, b, 8, 9);
			MonsterMoveSandSmoke(ThreadIndex, o);
			MonsterDieSandSmoke(ThreadIndex, o);
			break;
		case MODEL_MONSTER01 + 39:
			MoveEye(ThreadIndex, o, b, 28, 27);
			break;
		case MODEL_MONSTER01 + 37:
			if (o->CurrentAction >= MONSTER01_ATTACK1 && o->CurrentAction <= MONSTER01_ATTACK2)
			{
				o->BlendMeshLight += 0.1f;
				if (o->BlendMeshLight > 1.0f)
					o->BlendMeshLight = 1.0f;
			}
			else
			{
				o->BlendMeshLight -= 0.1f;
				if (o->BlendMeshLight < 0.0f)
					o->BlendMeshLight = 0.0f;
			}
			break;
		case MODEL_MONSTER01 + 29:
			o->BlendMesh = 3;
			o->BlendMeshTexCoordV = -(mu_float)((mu_int32)(FWorldTime) % 1000)*0.001f;
			if (GetLargeRand(2) == 0)
			{
				Vector(0.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(2)], p, Position, true);
				CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, Light);
			}
			break;
		case MODEL_MONSTER01 + 27:
			o->BlendMeshTexCoordV = -(mu_float)((mu_int32)(FWorldTime) % 1000)*0.001f;
			break;
		case MODEL_MONSTER01 + 26:
			o->BlendMeshTexCoordU = -(mu_float)((mu_int32)(FWorldTime) % 10000)*0.0001f;
			break;
		case MODEL_MONSTER01 + 32:
			Vector(0.0f, 0.0f, 0.0f, p);
			Vector(0.6f, 1.0f, 0.8f, Light);
			if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(33)], p, Position, true);
				CreateParticle(ThreadIndex, BITMAP_ENERGY, Position, o->Angle, Light);
				Vector(1.0f, 0.6f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, Light);
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(20)], p, Position, true);
				CreateParticle(ThreadIndex, BITMAP_ENERGY, Position, o->Angle, Light);
				Vector(1.0f, 0.6f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, Light);
			}
			else  if (o->CurrentAction == MONSTER01_ATTACK3)
			{
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(41)], p, Position, true);
				CreateParticle(ThreadIndex, BITMAP_ENERGY, Position, o->Angle, Light);
				Vector(1.0f, 0.6f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, Light);
			}
			else if (o->CurrentAction == MONSTER01_ATTACK4)
			{
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(49)], p, Position, true);
				CreateParticle(ThreadIndex, BITMAP_ENERGY, Position, o->Angle, Light);
				Vector(1.0f, 0.6f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, Light);
			}
			else if (o->CurrentAction == MONSTER01_DIE && o->AnimationFrame < 12.0f)
			{
				Vector(0.1f, 0.8f, 0.6f, Light);
				for (mu_int32 i = 0; i<20; ++i)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[GetLargeRand(b->_BoneNonDummyEnd)], p, Position, true);
					CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, Light);
				}
			}
			break;
		case MODEL_MONSTER01 + 11:
			o->BlendMeshLight = (mu_float)(GetLargeRand(10))*0.1f;
			if (c->Level == 2)
			{
				for (mu_int32 i = 0; i<10; ++i)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[GetLargeRand(b->_BoneNonDummyEnd)], p, Position, true);
					CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, Light);
				}
				Vector(Luminosity*1.0f, Luminosity*0.2f, Luminosity*0.0f, Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
			}
			break;
		case MODEL_MONSTER01 + 15:
			o->BlendMeshTexCoordV = -(mu_float)((mu_int32)(FWorldTime) % 2000)*0.0005f;
			break;
		case MODEL_MIX_NPC:
			if (GetLargeRand(64) == 0)
				PlayBackground(SOUND_GOBLIN_MIX);
			break;
		case MODEL_ELF_WIZARD:
			if (GetLargeRand(256) == 0)
				PlayBackground(SOUND_BLACKSMITH);
			break;
		case MODEL_SMITH:
			if (g_isCharacterBuff(o, eBuff_CrywolfNPCHide))
				break;
			if (o->CurrentAction == 0 && o->AnimationFrame >= 5.0f && o->AnimationFrame <= 10.0f)
				PlayBackground(SOUND_BLACKSMITH);
			o->BlendMesh = 4;
			o->BlendMeshLight = Luminosity;
			Vector(Luminosity*1.0f, Luminosity*0.4f, Luminosity*0.0f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
			Vector(1.0f, 1.0f, 1.0f, Light);
			Vector(0.0f, 0.0f, 0.0f, p);
			if (o->CurrentAction == 0 && o->AnimationFrame >= 5.0f && o->AnimationFrame <= 6.0f)
			{
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(17)], p, Position, true);
				EVector3 Angle;
				for (mu_int32 i = 0; i<4; ++i)
				{
					Vector((mu_float)(GetLargeRand(60) + 60 + 90), 0.0f, (mu_float)(GetLargeRand(30)), Angle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, Position, Position, Angle);
					CreateParticle(ThreadIndex, BITMAP_SPARK, Position, Angle, Light);
				}
			}
			break;

		case MODEL_DEVIAS_TRADER:
			Vector(1.0f, 1.0f, 1.0f, Light);
			Vector(0.0f, 5.0f, 10.0f, p);
			if (o->CurrentAction == 0)
			{
				Vector(Luminosity*0.5f, Luminosity*0.3f, Luminosity*0.0f, Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(37)], p, Position, true);
				EVector3 Angle;
				for (mu_int32 i = 0; i<4; ++i)
				{
					Vector((mu_float)(GetLargeRand(60) + 60 + 30), 0.0f, (mu_float)(GetLargeRand(30)), Angle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, Position, Position, Angle);
					if (GetLargeRand(2))
						CreateParticle(ThreadIndex, BITMAP_SPARK, Position, Angle, Light);
				}
			}
			break;
		case MODEL_WEDDING_NPC:
			if (o->CurrentAction == 1)
			{
				if (o->AnimationFrame > 4.5f && o->AnimationFrame <= 4.8f)
				{
					CreateEffect(ThreadIndex, BITMAP_FIRECRACKER0001, o->Position, o->Angle, o->Light, 0);
				}
			}
			break;
		case MODEL_MONSTER01 + 2:
			if (o->CurrentAction == MONSTER01_ATTACK1 && o->AnimationFrame <= 4.0f)
			{
				EVector3 Light;
				Vector(1.0f, 1.0f, 1.0f, Light);
				Vector(0.0f, (mu_float)(GetLargeRand(32) + 32), 0.0f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(7)], p, Position, true);
				CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, Light, 1);
			}
		case MODEL_MONSTER01 + 3:
		case MODEL_MONSTER01 + 6:
		case MODEL_MONSTER01 + 20:
			if (o->Type == MODEL_MONSTER01 + 20)
			{
				Vector(0.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(7)], p, Position, true);
				Vector(Luminosity*1.0f, Luminosity*0.4f, Luminosity*0.2f, Light);
				CreateSprite(BITMAP_LIGHT, Position, 1.0f, Light, o);
			}
			if (c->Dead == 0 && GetLargeRand(4) == 0)
			{
				Vector(o->Position[0] + (mu_float)(GetLargeRand(64) - 32),
					o->Position[1] + (mu_float)(GetLargeRand(64) - 32),
					o->Position[2] + (mu_float)(GetLargeRand(32) - 16), Position);
				if (GET_WORLD == WD_2DEVIAS)
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light);
				else
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, Position, o->Angle, Light);
			}
			break;
		case MODEL_MONSTER01 + 33:
			if (c->Dead == 0 && c->Level == 1 && GetLargeRand(4) == 0)
			{
				Vector(o->Position[0] + (mu_float)(GetLargeRand(64) - 32),
					o->Position[1] + (mu_float)(GetLargeRand(64) - 32),
					o->Position[2] + (mu_float)(GetLargeRand(32) - 16), Position);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, Position, o->Angle, Light);
			}
			break;
		case MODEL_MONSTER01 + 5:
			MonsterDieSandSmoke(ThreadIndex, o);
			break;
		case MODEL_MONSTER01 + 12:
		case MODEL_MONSTER01 + 13:
			if (GetLargeRand(4) == 0)
			{
				Vector(0.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(22)], p, Position, true);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, o->Light);
			}
			break;
		case MODEL_MONSTER01:
			if (o->CurrentAction == MONSTER01_STOP1 &&
				(o->AnimationFrame >= 15.0f&&o->AnimationFrame <= 20.0f)) Smoke = true;
			if (o->CurrentAction == MONSTER01_STOP2 &&
				(o->AnimationFrame >= 20.0f&&o->AnimationFrame <= 25.0f)) Smoke = true;
			if (o->CurrentAction == MONSTER01_WALK &&
				((o->AnimationFrame >= 2.0f&&o->AnimationFrame <= 3.0f) ||
				(o->AnimationFrame >= 5.0f&&o->AnimationFrame <= 6.0f))) Smoke = true;
			if (Smoke)
			{
				if (GetLargeRand(2) == 0)
				{
					Vector(0.0f, -4.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(24)], p, Position, true);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, o->Light);
				}
			}
			break;
	
		default:
			{
				if (TheMapProcess().MoveMonsterVisual(ThreadIndex, c, o, b) == true) break;
			}
			break;
		}

		if (o->Type == MODEL_PLAYER && c == Hero)
		{
			if ((o->CurrentAction >= PLAYER_WALK_MALE && o->CurrentAction <= PLAYER_RUN_RIDE_WEAPON)
				|| (o->CurrentAction == PLAYER_WALK_TWO_HAND_SWORD_TWO || o->CurrentAction == PLAYER_RUN_TWO_HAND_SWORD_TWO)
				|| (o->CurrentAction == PLAYER_RUN_RIDE_HORSE)
				)
			{
				Vector(0.0f, 0.0f, 0.0f, p);
				if (o->AnimationFrame >= 1.5f && !c->Foot[0])
				{
					c->Foot[0] = true;
					PlayWalkSound();
				}
				if (o->AnimationFrame >= 4.5f && !c->Foot[1])
				{
					c->Foot[1] = true;
					PlayWalkSound();
				}
			}
		}

		if ((o->CurrentAction == PLAYER_RUN_RIDE
			|| o->CurrentAction == PLAYER_RUN_RIDE_WEAPON || o->CurrentAction == PLAYER_RUN_SWIM || o->CurrentAction == PLAYER_WALK_SWIM || o->CurrentAction == PLAYER_FLY || o->CurrentAction == PLAYER_FLY_CROSSBOW || o->CurrentAction == PLAYER_RUN_RIDE_HORSE) &&
			o->Type == MODEL_PLAYER && InHellas())
		{
			EVector3 Light = { 0.3f, 0.3f, 0.3f };
			VectorCopy(o->Position, Position);

			EBone Matrix;

			Vector(0.0f, -40.0f, 0.0f, p);

			AngleMatrix(o->Angle, Matrix);
			VectorRotate(p, Matrix, Position);
			VectorAdd(o->Position, Position, Position);

			Position[0] += GetLargeRand(64) - 32.0f;
			Position[1] += GetLargeRand(64) - 32.0f;
			Position[2] += 50.0f;

			CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, Position, o->Angle, Light, 1);
		}
	}
}

void MUTasking::MoveCharacterView()
{
	CHARACTER *cc = &CharacterView;
	OBJECT *co = &cc->Object;

	if (CharacterView.Object.Flags.Get(OBJECT::eOF_LIVE) == true)
	{
		CharacterViewVisible = true;

		if (!CharacterAnimation(0, cc, co))
			SetAction(co, 0);

		MoveMonsterClient(0, cc, co);
		MoveCharacter(0, cc, co);
		MoveCharacterVisual(0, cc, co, true);
	}
}

void MUTasking::MoveHero()
{
	const mu_uint32 ThreadIndex = 0;
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	CHARACTER *c = Hero;
	OBJECT *o = &c->Object;

	if (o->CurrentAction == PLAYER_CHANGE_UP)
	{
		return;
	}

	if (g_Direction.IsDirection())
	{
		SetPlayerStop(ThreadIndex, c);
		return;
	}

	if (SelectedCharacter != -1 && g_isCharacterBuff((&Hero->Object), eBuff_CrywolfHeroContracted))
	{
		return;
	}

	if (c->Dead > 0
		|| g_isCharacterBuff(o, eDeBuff_Stun)
		|| g_isCharacterBuff(o, eDeBuff_Sleep))
	{
		return;
	}

	if (c->Object.Flags.Get(OBJECT::eOF_LIVE) == false)
	{
		return;
	}

	if (LoadingWorld > 0)
	{
		--LoadingWorld;
		if (LoadingWorld == 9999998 || LoadingWorld == 29)
			SetPlayerStop(ThreadIndex, c);
	}

	if (LoadingWorld > 30)
	{
		return;
	}

#if 0 // FIX
	if (g_pWindowMgr->GetAddFriendWindow() > 0)
	{
		if (MouseRButtonPush)
		{
			if (!::IsStrifeMap(GET_WORLD))
			{
				CUITextInputWindow * pWindow = (CUITextInputWindow *)g_pWindowMgr->GetWindow(g_pWindowMgr->GetAddFriendWindow());
				if (pWindow != nullptr)
				{
					pWindow->SetText(CharactersClient[SelectedCharacter].ID);
				}
	}
}
		}
#endif

	mu_int32 HeroX = GetScreenWidth() / 2;
	mu_int32 HeroY = 180;

	mu_int32 Angle;
	mu_boolean bLookAtMouse = true;
	mu_boolean NoAutoAttacking = false;

	if (g_isCharacterBuff(o, eDeBuff_Stun)
		|| g_isCharacterBuff(o, eDeBuff_Sleep)
		|| o->CurrentAction == PLAYER_POINT_DANCE
		)
	{
		Angle = (mu_int32)Hero->Object.Angle[2];
		bLookAtMouse = false;
	}
	else
	{
		Angle = (mu_int32)(Hero->Object.Angle[2] + CreateAngle((mu_float)HeroX, (mu_float)HeroY, (mu_float)MouseX, (mu_float)MouseY)) + 360 - 45;
		Angle %= 360;
		if (Angle < 120) Angle = 120;
		if (Angle > 240) Angle = 240;
		Angle += 180;
		Angle %= 360;
	}

	Hero->Object.HeadTargetAngle[2] = 0.0f;

	if ((MU_RESOURCE->IsAutoAttack() == true
		&& Attacking != -1
		&& GET_WORLD != WD_6STADIUM
		&& InChaosCastle() == false)
		|| o->CurrentAction == PLAYER_ATTACK_SKILL_FURY_STRIKE
		|| (o->CurrentAction >= PLAYER_WALK_MALE && o->CurrentAction <= PLAYER_RUN_RIDE_WEAPON)
		|| (o->CurrentAction >= PLAYER_ATTACK_FIST && o->CurrentAction <= PLAYER_ATTACK_END)
		|| (o->CurrentAction >= PLAYER_FLY_RIDE && o->CurrentAction <= PLAYER_FLY_RIDE_WEAPON)
		|| o->CurrentAction == PLAYER_DARKLORD_WALK
		|| o->CurrentAction == PLAYER_RUN_RIDE_HORSE
		|| (o->CurrentAction >= PLAYER_ATTACK_STRIKE && o->CurrentAction <= PLAYER_ATTACK_DARKHORSE)
		|| (o->CurrentAction >= PLAYER_FENRIR_ATTACK && o->CurrentAction <= PLAYER_FENRIR_WALK_ONE_LEFT)
		|| (o->CurrentAction >= PLAYER_ATTACK_BOW_UP && o->CurrentAction <= PLAYER_HIGH_SHOCK)
		|| (o->CurrentAction >= PLAYER_WALK_TWO_HAND_SWORD_TWO && o->CurrentAction <= PLAYER_ATTACK_TWO_HAND_SWORD_TWO)
		|| (o->CurrentAction >= PLAYER_SKILL_HAND1 && o->CurrentAction <= MAX_PLAYER_ACTION)
		)
	{
		bLookAtMouse = false;
	}

	if (bLookAtMouse)
	{
		mu_int32 mousePosY = MouseY;

		if (mousePosY > 480)
		{
			mousePosY = 480;
		}

		Hero->Object.HeadTargetAngle[0] = (mu_float)Angle;
		Hero->Object.HeadTargetAngle[1] = (HeroY - mousePosY)*0.05f;

		NoAutoAttacking = true;
	}
	else
	{
		Hero->Object.HeadTargetAngle[0] = 0.0f;
		Hero->Object.HeadTargetAngle[1] = 0.0f;
	}

	if (c->Movement == false)
	{
		++StandTime;

		if (StandTime >= 40 &&
			MU_INPUT->IsMouseOverInterface() == false &&
			MU_CAMERA->IsDragging() == false &&
			!Hero->Dead &&
			o->CurrentAction != PLAYER_POSE1 &&
			o->CurrentAction != PLAYER_POSE_FEMALE1 &&
			o->CurrentAction != PLAYER_SIT1 &&
			o->CurrentAction != PLAYER_SIT_FEMALE1 &&
			NoAutoAttacking &&
			o->CurrentAction != PLAYER_ATTACK_TELEPORT &&
			o->CurrentAction != PLAYER_ATTACK_RIDE_TELEPORT &&
			o->CurrentAction != PLAYER_FENRIR_ATTACK_DARKLORD_TELEPORT &&
			Hero->AttackTime == 0 &&
			ActionRequest == ACTION_REQUEST::AR_NONE)
		{
			StandTime = 0;
			HeroAngle = -(mu_int32)(CreateAngle((mu_float)MouseX, (mu_float)MouseY, (mu_float)HeroX, (mu_float)HeroY)) + 360 + 45;
			HeroAngle %= 360;
			mu_uint8 Angle1 = ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8);
			mu_uint8 Angle2 = ((mu_uint8)(((mu_float)HeroAngle + 22.5f) / 360.0f*8.0f + 1.0f) % 8);

			if (Angle1 != Angle2 &&
				o->CurrentAction != PLAYER_ATTACK_SKILL_SWORD2 &&
				Hero->Object.Angle[2] != HeroAngle)
			{
				g_GameServerObject->SendHeroAngle((mu_float)HeroAngle);
				Hero->Object.Angle[2] = (mu_float)HeroAngle;
			}
		}
	}

	if (MU_INPUT->IsMouseOverInterface() == false &&
		MU_CAMERA->IsDragging() == false)
	{
		mu_boolean Success = false;
		if (MouseUpdateTime >= MouseUpdateTimeMax)
		{
			if (MU_INPUT->Is3DMousePressed(MOUSE_BUTTON_LEFT) == true)
			{
				MU_INPUT->SetMouseUsed(MOUSE_BUTTON_LEFT, MOUSE_USED_PRESSED);
				Success = true;
			}
			if (MU_INPUT->Is3DMousePressing(MOUSE_BUTTON_LEFT) == true)
			{
				Success = true;
			}

			if ((MU_RESOURCE->IsAutoAttack() == true
				&& GET_WORLD != WD_6STADIUM
				&& InChaosCastle() == false)
				&& (Attacking == 1 && SelectedCharacter != -1))
			{
				Success = true;
			}

			if (Success
				&& !g_isCharacterBuff(o, eDeBuff_Stun)
				&& !g_isCharacterBuff(o, eDeBuff_Sleep))
			{
				g_iFollowCharacter = -1;

				LButtonPressTime = ((FWorldTime - LButtonPopTime) / CLOCKS_PER_SEC);

				if (LButtonPressTime >= AutoMouseLimitTime)
				{
					MU_INPUT->SetMouseUsed(MOUSE_BUTTON_LEFT, MOUSE_USED_ALL);
					Success = false;
				}
			}
			else
			{
				LButtonPopTime = FWorldTime;
				LButtonPressTime = 0.0f;
			}
		}

		if (g_iFollowCharacter != -1)
		{
			CHARACTER* followCharacter = &CharactersClient[g_iFollowCharacter];
			if (followCharacter->Object.Flags.Get(OBJECT::eOF_LIVE) == false)
			{
				g_iFollowCharacter = -1;
			}
			else if (g_pNewUISystem->AllowHeroAction() == true)
			{
				if (ActionRequest != ACTION_REQUEST::AR_FOLLOW ||
					ActionRequestTarget != followCharacter->Key)
				{
					CL2GS_REQUEST_FOLLOW RequestFollow;

					RequestFollow.Key = followCharacter->Key;

					g_GameServerObject->Send(&RequestFollow);

					ActionRequest = ACTION_REQUEST::AR_FOLLOW;
					ActionRequestTarget = followCharacter->Key;
				}
			}
		}
		else if ((g_pNewUISystem->GetSelected() == nullptr || g_pNewUISystem->GetSelected()->BypassMessage() == false) &&
			MU_INPUT->IsKeyPressed(SDL_SCANCODE_SPACE) == true)
		{
			if (g_pNewUISystem->AllowHeroAction() == true)
			{
#if 0 // FIX
				if (!g_bAutoGetItem)
				{
					if (CheckTile(c, o, 1.5f) == false)
					{
						break;
	}
					MouseUpdateTimeMax = 6;
					}
#endif

				for (mu_int32 i = 0; i < MAX_ITEMS; ++i)
				{
					OBJECT *pObj = &Items[i].Object;

					if (pObj->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == false)
					{
						continue;
					}

					EFloat dis = EMath::Distance(pObj->Position, Hero->Object.Position);
					if (dis <= 300.0f)
					{
						SelectedItem = i;
						break;
					}
				}

#define GOLD_TYPE_INDEX  (ITEM_POTION + 15)
				if (SelectedItem != -1)
				{
					if (Items[SelectedItem].Item.Type != GOLD_TYPE_INDEX &&
						g_pMyInventory->FindEmptySlot(&Items[SelectedItem].Item) == -1)
					{
						g_pChatListBox->AddText(_T(""), GlobalText[375], TYPE_SYSTEM_MESSAGE);

						OBJECT * pItem = &(Items[SelectedItem].Object);
						pItem->Position[2] = RequestTerrainHeight(pItem->Position[0], pItem->Position[1]) + 3.0f;
						pItem->Gravity = 50.0f;
					}
					else
					{
						if (ActionRequest != ACTION_REQUEST::AR_PICKITEM ||
							ActionRequestTarget != SelectedItem)
						{
							CL2GS_REQUEST_PICKITEM RequestPickItem;

							RequestPickItem.ItemKey = SelectedItem;

							g_GameServerObject->Send(&RequestPickItem);

							ActionRequest = ACTION_REQUEST::AR_PICKITEM;
							ActionRequestTarget = SelectedItem;
						}
					}
				}
				}
			}
		else if (Success)
		{
			mu_int32 RightType = CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT].Type;
			mu_int32 LeftType = CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Type;

			if (g_pPickedItem.GetItem() == nullptr && RightType == -1 &&
				((LeftType >= ITEM_SWORD && LeftType < ITEM_SPEAR)
					|| (LeftType >= ITEM_STAFF && LeftType < ITEM_SHIELD
						&& !(LeftType >= ITEM_STAFF + 21 && LeftType <= ITEM_STAFF + 29)
						)))
			{
				if (g_pMyInventory->IsEquipable(GAME::EQUIPMENT_WEAPON_LEFT, &CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT]))
				{
#if 0 // FIX
					memcpy(&PickItem, &CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT], sizeof(ITEM));
					CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type = -1;
					CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Level = 0;
					SetCharacterClass(ThreadIndex, Hero);
					SrcInventory = Inventory;
					SrcInventoryIndex = EQUIPMENT_WEAPON_LEFT;
					DstInventoryIndex = EQUIPMENT_WEAPON_RIGHT;
					SendRequestEquipmentItem(0, SrcInventoryIndex, 0, DstInventoryIndex);
#endif
		}
				}

			MouseUpdateTime = 0;
			Success = false;

			if (!c->SafeZone)
			{
				Success = CheckAttack();
			}

			if (Success)
			{
				if (SelectedCharacter >= -1)
				{
					mu_int16 TargetCharacter = SelectedCharacter >= 0 ? CharactersClient[SelectedCharacter].Key : -1;
					if (ActionRequest != ACTION_REQUEST::AR_ATTACK ||
						ActionRequestTarget != TargetCharacter)
					{
						if (g_pNewUISystem->AllowHeroAction() == true)
						{
							CL2GS_REQUEST_ATTACK RequestAttack;

							RequestAttack.Key = TargetCharacter;

							g_GameServerObject->Send(&RequestAttack);

							ActionRequest = ACTION_REQUEST::AR_ATTACK;
							ActionRequestTarget = TargetCharacter;
						}
					}
				}
			}
			else if (SelectedOperate != -1 &&
				(c->SafeZone ||
					c->Helper.Type < MODEL_HELPER + 2 ||
					(c->Helper.Type > MODEL_HELPER + 4 &&
						c->Helper.Type != MODEL_HELPER + 37)))
			{
				if (g_pNewUISystem->AllowHeroAction() == true)
				{
					TargetX = (mu_int32)(g_Operates[SelectedOperate].Owner->Position[0] * g_TerrainScaleRev);
					TargetY = (mu_int32)(g_Operates[SelectedOperate].Owner->Position[1] * g_TerrainScaleRev);
					mu_int32 wall = TerrainWall[TERRAIN_INDEX(TargetX, TargetY)];

					if (wall == TW_HEIGHT || wall < TW_CHARACTER)
					{
						if (ActionRequest != ACTION_REQUEST::AR_OPERATE ||
							ActionRequestX != TargetX ||
							ActionRequestY != TargetY)
						{
							CL2GS_REQUEST_OPERATE RequestOperate;

							RequestOperate.X = TargetX;
							RequestOperate.Y = TargetY;
							RequestOperate.Type = g_Operates[SelectedOperate].Owner->Type;
							RequestOperate.Angle = g_Operates[SelectedOperate].Owner->Angle[2];

							g_GameServerObject->Send(&RequestOperate);

							ActionRequest = ACTION_REQUEST::AR_OPERATE;
							ActionRequestX = TargetX;
							ActionRequestY = TargetY;
						}
					}
				}
			}
			else if (SelectedNpc >= 0
				&& !g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_NPCSHOP)
				&& !g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_STORAGE)
				)
			{
				if (g_pNewUISystem->AllowHeroAction() == true)
				{
					if (ActionRequest != ACTION_REQUEST::AR_TALK ||
						ActionRequestTarget != CharactersClient[SelectedNpc].Key)
					{
						CL2GS_REQUEST_TALK RequestTalk;

						RequestTalk.Key = CharactersClient[SelectedNpc].Key;

						g_GameServerObject->Send(&RequestTalk);

						ActionRequest = ACTION_REQUEST::AR_TALK;
						ActionRequestTarget = CharactersClient[SelectedNpc].Key;
					}
				}
			}
			else if (SelectedItem != -1)
			{
				if (g_pNewUISystem->AllowHeroAction() == true)
				{
#if 0 // FIX
					if (!g_bAutoGetItem)
					{
						if (CheckTile(c, o, 1.5f) == false)
						{
							break;
							}
						MouseUpdateTimeMax = 6;
						}
#endif

#define GOLD_TYPE_INDEX  (ITEM_POTION + 15)
					if (Items[SelectedItem].Item.Type != GOLD_TYPE_INDEX &&
						g_pMyInventory->FindEmptySlot(&Items[SelectedItem].Item) == -1)
					{
						g_pChatListBox->AddText(_T(""), GlobalText[375], TYPE_SYSTEM_MESSAGE);

						OBJECT * pItem = &(Items[SelectedItem].Object);
						pItem->Position[2] = RequestTerrainHeight(pItem->Position[0], pItem->Position[1]) + 3.0f;
						pItem->Gravity = 50.0f;
					}
					else
					{
						if (ActionRequest != ACTION_REQUEST::AR_PICKITEM ||
							ActionRequestTarget != SelectedItem)
						{
							CL2GS_REQUEST_PICKITEM RequestPickItem;

							RequestPickItem.ItemKey = SelectedItem;

							g_GameServerObject->Send(&RequestPickItem);

							ActionRequest = ACTION_REQUEST::AR_PICKITEM;
							ActionRequestTarget = SelectedItem;
						}
					}
					}
				}
			else if (MU_INPUT->IsShiftPressing() == false)
			{
				InitDetectTerrainCollision;
				mu_boolean Success = DetectTerrainCollision(ThreadIndex, SelectXF, SelectYF);

				if (Success && c->Object.Flags.Get(OBJECT::eOF_LIVE) == true)
				{
					if (g_pNewUISystem->AllowHeroAction() == true)
					{
						TargetX = (mu_uint8)(ThreadInfo.CollisionPosition[0] * g_TerrainScaleRev);
						TargetY = (mu_uint8)(ThreadInfo.CollisionPosition[1] * g_TerrainScaleRev);
						mu_int32 Wall;

						Wall = TW_NOGROUND;
						mu_uint16 CurrAtt = TerrainWall[TargetY * 256 + TargetX];

						if (CurrAtt >= Wall && (CurrAtt&TW_ACTION) != TW_ACTION
							&& (CurrAtt&TW_HEIGHT) != TW_HEIGHT)
						{
							DontMove = true;
						}
						else
						{
							DontMove = false;
						}

						mu_int32 xPos = (mu_int32)(o->Position[0] * g_TerrainScaleRev);
						mu_int32 yPos = (mu_int32)(o->Position[1] * g_TerrainScaleRev);

						if (!c->Movement || (EMath::Abs(c->PositionX - xPos) < 2 && EMath::Abs(c->PositionY - yPos) < 2))
						{
							if ((c->PositionX != TargetX || c->PositionY != TargetY || !c->Movement) &&
								TestPath(ThreadIndex, c->PositionX, c->PositionY, TargetX, TargetY))
							{
								CL2GS_REQUEST_MOVE RequestMove;
								RequestMove.X = TargetX;
								RequestMove.Y = TargetY;
								g_GameServerObject->Send(&RequestMove);

								ActionRequest = ACTION_REQUEST::AR_NONE;

								EVector3 EffectPos((TargetX + 0.5f) * g_TerrainScale,
									(TargetY + 0.5f) * g_TerrainScale,
									ThreadInfo.CollisionPosition.z);

								OBJECT* pHeroObj = &Hero->Object;
								EVector3 vLight;
								Vector(1.0f, 1.0f, 0.0f, vLight);
								DeleteEffect(MODEL_MOVE_TARGETPOSITION_EFFECT, nullptr);
								mu_int32 iTerrainIndex = TERRAIN_INDEX((mu_int32)ThreadInfo.SelectXF, (mu_int32)ThreadInfo.SelectYF);
								if ((TerrainWall[iTerrainIndex] & TW_NOMOVE) != TW_NOMOVE)
								{
									CreateEffect(ThreadIndex, MODEL_MOVE_TARGETPOSITION_EFFECT, EffectPos, pHeroObj->Angle, vLight, 0, pHeroObj, -1, 0, 0, 0, 0.6f);
								}
							}
							else
							{
								MouseUpdateTime = MouseUpdateTimeMax;
								MouseUpdateTime = 0;
							}
						}
					}
				}
			}

			if (Success == false &&
				ActionRequest == ACTION_REQUEST::AR_ATTACK)
			{
				mu_boolean cancelRequest = true;

				if (MU_RESOURCE->IsAutoAttack() == true)
				{
					mu_int32 characterIndex = FindCharacterIndex(ActionRequestTarget);
					if (characterIndex != MAX_CHARACTERS_CLIENT)
					{
						CHARACTER *tmpChar = &CharactersClient[characterIndex];
						if (tmpChar->Object.Kind != KIND_PLAYER)
						{
							cancelRequest = false;
						}
					}
				}

				if (cancelRequest == true)
				{
					CL2GS_REQUEST_CANCEL RequestCancelAction;
					g_GameServerObject->Send(&RequestCancelAction);

					ActionRequest = ACTION_REQUEST::AR_NONE;
				}
			}
			}
		else if (MouseUpdateTime >= MouseUpdateTimeMax &&
			ActionRequest == ACTION_REQUEST::AR_ATTACK)
		{
			mu_boolean cancelRequest = true;

			if (MU_RESOURCE->IsAutoAttack() == true)
			{
				mu_int32 characterIndex = FindCharacterIndex(ActionRequestTarget);
				if (characterIndex != MAX_CHARACTERS_CLIENT)
				{
					CHARACTER *tmpChar = &CharactersClient[characterIndex];
					if (tmpChar->Object.Kind != KIND_PLAYER)
					{
						cancelRequest = false;
					}
				}
			}

			if (cancelRequest == true)
			{
				CL2GS_REQUEST_CANCEL RequestCancelAction;
				g_GameServerObject->Send(&RequestCancelAction);

				ActionRequest = ACTION_REQUEST::AR_NONE;
			}
		}

		++MouseUpdateTime;
	}
	else if (ActionRequest == ACTION_REQUEST::AR_ATTACK)
	{
		mu_boolean cancelRequest = true;

		if (MU_RESOURCE->IsAutoAttack() == true)
		{
			mu_int32 characterIndex = FindCharacterIndex(ActionRequestTarget);
			if (characterIndex != MAX_CHARACTERS_CLIENT)
			{
				CHARACTER *tmpChar = &CharactersClient[characterIndex];
				if (tmpChar->Object.Kind != KIND_PLAYER)
				{
					cancelRequest = false;
				}
			}
		}

		if (cancelRequest == true)
		{
			CL2GS_REQUEST_CANCEL RequestCancelAction;
			g_GameServerObject->Send(&RequestCancelAction);

			ActionRequest = ACTION_REQUEST::AR_NONE;
		}
	}

	Attack(ThreadIndex, Hero);

	mu_int32 Index = ((mu_int32)Hero->Object.Position[1] / (mu_int32)g_TerrainScale) * 256 + ((mu_int32)Hero->Object.Position[0] / (mu_int32)g_TerrainScale);
	if (Index < 0) Index = 0;
	else if (Index > 65535) Index = 65535;
	HeroTile = TerrainMappingLayer1[Index];
}

void MUTasking::MoveCharacters(const mu_uint32 ThreadIndex)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	CHARACTER *c = &CharactersClient[ThreadIndex];
	for (mu_uint32 i = ThreadIndex; i < MAX_CHARACTERS_CLIENT; i += _ThreadsCount, c += _ThreadsCount)
	{
		OBJECT *o = &c->Object;

		if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
		{
			MoveMonsterClient(ThreadIndex, c, o);
			MoveCharacter(ThreadIndex, c, o);
			MoveCharacterVisual(ThreadIndex, c, o, o->Flags.Get(OBJECT::eOF_VISIBLE));
			battleCastle::MoveBattleCastleMonster(c, o);
		}
	}

	_ThreadBarrier->Sync();

	c = &CharactersClient[ThreadIndex];
	for (mu_uint32 i = ThreadIndex; i < MAX_CHARACTERS_CLIENT; i += _ThreadsCount, c += _ThreadsCount)
	{
		OBJECT *o = &c->Object;

		if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
		{
			MUModel *b = &MODELS::Data[o->Type];
			VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));
			b->BodyScale(ThreadIndex) = o->Scale;
			b->CurrentAction(ThreadIndex) = o->CurrentAction;

			EVector3 p(0.0f), Position;
			EVector3 Light(1.0f);

			if (c->AttackTime > 0)
			{
				AttackStage(ThreadIndex, c, o);
				AttackEffect(ThreadIndex, c);
				++c->AttackTime;
			}

			if (c->AttackTime >= ThreadInfo.LimitAttackTime)
			{
				c->AttackTime = 0;
				o->PKKey = getTargetCharacterKey(c, SelectedCharacter);

				switch ((c->Skill))
				{
				case AT_SKILL_SUMMON:
				case AT_SKILL_SUMMON + 1:
				case AT_SKILL_SUMMON + 2:
				case AT_SKILL_SUMMON + 3:
				case AT_SKILL_SUMMON + 4:
				case AT_SKILL_SUMMON + 5:
				case AT_SKILL_SUMMON + 6:
					CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, o->Position, o->Angle, o->Light, 3, o);
					break;
				case AT_SKILL_BLOW_OF_DESTRUCTION:
					o->Weapon = c->Weapon[0].Type - MODEL_SWORD;
					o->WeaponLevel = (mu_uint8)c->Weapon[0].Level;
					o->WeaponExcOption = c->Weapon[0].ExcOption;
					o->WeaponSetOption = c->Weapon[0].SetOption;

					Vector(0.0f, 0.0f, 0.0f, o->Light);
					o->Light[0] = (mu_float)(c->SkillX + 0.5f)*g_TerrainScale;
					o->Light[1] = (mu_float)(c->SkillY + 0.5f)*g_TerrainScale;
					o->Light[2] = o->Position[2];
					CreateEffect(ThreadIndex, MODEL_BLOW_OF_DESTRUCTION, o->Position, o->Angle, o->Light, 0, o);

					PlayBackground(SOUND_SKILL_BLOWOFDESTRUCTION);

					break;

				case AT_SKILL_BLOOD_ATT_UP:
				case AT_SKILL_BLOOD_ATT_UP + 1:
				case AT_SKILL_BLOOD_ATT_UP + 2:
				case AT_SKILL_BLOOD_ATT_UP + 3:
				case AT_SKILL_BLOOD_ATT_UP + 4:
				case AT_SKILL_REDUCEDEFENSE:
					o->Weapon = c->Weapon[0].Type - MODEL_SWORD;
					o->WeaponLevel = (mu_uint8)c->Weapon[0].Level;
					o->WeaponExcOption = c->Weapon[0].ExcOption;
					o->WeaponSetOption = c->Weapon[0].SetOption;
					break;
				case AT_SKILL_POWER_SLASH_UP:
				case AT_SKILL_POWER_SLASH_UP + 1:
				case AT_SKILL_POWER_SLASH_UP + 2:
				case AT_SKILL_POWER_SLASH_UP + 3:
				case AT_SKILL_POWER_SLASH_UP + 4:
				case AT_SKILL_ICE_BLADE:
					o->Weapon = c->Weapon[0].Type - MODEL_SWORD;
					o->WeaponLevel = (mu_uint8)c->Weapon[0].Level;
					o->WeaponExcOption = c->Weapon[0].ExcOption;
					o->WeaponSetOption = c->Weapon[0].SetOption;
					break;
				case AT_SKILL_LIFE_UP:
				case AT_SKILL_LIFE_UP + 1:
				case AT_SKILL_LIFE_UP + 2:
				case AT_SKILL_LIFE_UP + 3:
				case AT_SKILL_LIFE_UP + 4:
				case AT_SKILL_VITALITY:
					{
						EVector3 Angle = { 0.0f, 0.0f, 0.0f };
						mu_int32 iCount = 36;
						for (mu_int32 i = 0; i < iCount; ++i)
						{
							Angle[0] = -10.0f;
							Angle[1] = 0.0f;
							Angle[2] = i * 10.0f;
							EVector3 Position;
							VectorCopy(o->Position, Position);
							Position[2] += 100.0f;
							CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, Position, Position, Angle, 2, o, 60.0f, 0, 0);

							if ((i % 20) == 0)
							{
								CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, o->Position, Angle, o->Light, 4, o);
							}
						}
					}
					PlayBackground(SOUND_SWELLLIFE);
					break;

				case AT_SKILL_STUN:
					CreateJoint(ThreadIndex, BITMAP_FLASH, o->Position, o->Position, o->Angle, 7, nullptr);

					PlayBackground(SOUND_BMS_STUN);
					break;

				case AT_SKILL_REMOVAL_STUN:
					{
						if (c->TargetCharacter != -1)
						{
							CHARACTER *tc = &CharactersClient[c->TargetCharacter];
							OBJECT *to = &tc->Object;
							if (to != o)
							{
								VectorCopy(to->Position, Position);
								Position[2] += 1200.0f;
								CreateJoint(ThreadIndex, BITMAP_FLASH, Position, Position, to->Angle, 0, to, 120.0f);

								//PlayBackground(SOUND_BMS_STUN_REMOVAL);
							}
						}
					}
					break;

				case AT_SKILL_MANA:
					{
						EVector3 Angle = { 0.0f, 0.0f, 0.0f };
						mu_int32 iCount = 36;

						for (mu_int32 i = 0; i < iCount; ++i)
						{
							Angle[0] = -10.0f;
							Angle[1] = 0.0f;
							Angle[2] = i * 10.0f;
							EVector3 Position;
							VectorCopy(o->Position, Position);
							Position[2] += 100.0f;
							CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, Position, Position, Angle, 21, o, 60.0f, 0, 0);
							if ((i % 20) == 0)
								CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, o->Position, Angle, Light, 10, o);
						}
					}
					PlayBackground(SOUND_BMS_MANA);
					break;

				case AT_SKILL_INVISIBLE:
					{
						if (c->TargetCharacter != -1)
						{
							CHARACTER *tc = &CharactersClient[c->TargetCharacter];
							OBJECT *to = &tc->Object;

							DeleteJoint(MODEL_SPEARSKILL, to, 4);
							DeleteJoint(MODEL_SPEARSKILL, to, 9);
							CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, to->Position, to->Angle, to->Light, 6, to);
							PlayBackground(SOUND_BMS_INVISIBLE);
						}
					}
					break;
				case AT_SKILL_REMOVAL_INVISIBLE:
					{
						if (c->TargetCharacter != -1)
						{
							CHARACTER *tc = &CharactersClient[c->TargetCharacter];
							OBJECT *to = &tc->Object;

							if (to != o)
							{
								VectorCopy(to->Position, Position);
								Position[2] += 1200.0f;
								CreateJoint(ThreadIndex, BITMAP_FLASH, Position, Position, to->Angle, 1, to, 120.0f);

								//PlayBackground(SOUND_BMS_STUN_REMOVAL);
							}
						}
					}
					break;
				case AT_SKILL_REMOVAL_BUFF:
					{
						EVector3 Angle;
						EVector3 Position;
						VectorCopy(o->Position, Position);
						Position[2] += 100.0f;

						std::list<eBuffState> bufflist;

						bufflist.push_back(eDeBuff_Poison);
						bufflist.push_back(eDeBuff_Freeze);
						bufflist.push_back(eDeBuff_Harden);
						bufflist.push_back(eDeBuff_Defense);
						bufflist.push_back(eDeBuff_Stun);
						bufflist.push_back(eDeBuff_Sleep);
						bufflist.push_back(eDeBuff_BlowOfDestruction);

						bufflist.push_back(eBuff_HpRecovery); bufflist.push_back(eBuff_Attack);
						bufflist.push_back(eBuff_Defense); bufflist.push_back(eBuff_AddAG);
						bufflist.push_back(eBuff_Cloaking); bufflist.push_back(eBuff_AddSkill);
						bufflist.push_back(eBuff_PhysDefense); bufflist.push_back(eBuff_AddCriticalDamage);
						bufflist.push_back(eBuff_CrywolfAltarOccufied);

						g_CharacterUnRegisterBuffList(o, bufflist);

						Vector(0.0f, 0.0f, 45.0f, Angle);
						CreateJoint(ThreadIndex, MODEL_SPEARSKILL, Position, Position, Angle, 5, o, 170.0f);
						Position[2] -= 10.0f;
						Vector(0.0f, 0.0f, 135.0f, Angle);
						CreateJoint(ThreadIndex, MODEL_SPEARSKILL, Position, Position, Angle, 6, o, 170.0f);
						Position[2] -= 10.0f;
						Vector(0.0f, 0.0f, 225.0f, Angle);
						CreateJoint(ThreadIndex, MODEL_SPEARSKILL, Position, Position, Angle, 7, o, 170.0f);

						Vector(0.0f, 0.0f, 90.0f, Angle);
						CreateJoint(ThreadIndex, MODEL_SPEARSKILL, Position, Position, Angle, 5, o, 170.0f);
						Position[2] -= 10.0f;
						Vector(0.0f, 0.0f, 180.0f, Angle);
						CreateJoint(ThreadIndex, MODEL_SPEARSKILL, Position, Position, Angle, 6, o, 170.0f);
						Position[2] -= 10.0f;
						Vector(0.0f, 0.0f, 270.0f, Angle);
						CreateJoint(ThreadIndex, MODEL_SPEARSKILL, Position, Position, Angle, 7, o, 170.0f);
					}

					PlayBackground(SOUND_BMS_MAGIC_REMOVAL);
					break;
				case AT_SKILL_IMPROVE_AG:
					{
						EVector3 Angle = { -45.0f, 0.0f, 45.0f };
						EVector3 Position;

						Position[0] = o->Position[0] + EMath::Sin(45 * 0.1f)*80.0f;
						Position[1] = o->Position[1] + EMath::Cos(45 * 0.1f)*80.0f;
						Position[2] = o->Position[2] + 300;
						CreateJoint(ThreadIndex, BITMAP_JOINT_HEALING, Position, Position, Angle, 10, o, 15.0f);
						Angle[2] = 405.0f;
						CreateJoint(ThreadIndex, BITMAP_JOINT_HEALING, Position, Position, Angle, 10, o, 15.0f);

						Angle[2] = 225.0f;
						Position[0] = o->Position[0] + EMath::Cos(225 * 0.1f)*80.0f;
						Position[1] = o->Position[1] + EMath::Sin(225 * 0.1f)*80.0f;
						Position[2] = o->Position[2] + 300;
						CreateJoint(ThreadIndex, BITMAP_JOINT_HEALING, Position, Position, Angle, 10, o, 15.0f);
						Angle[2] = 450.0f;
						CreateJoint(ThreadIndex, BITMAP_JOINT_HEALING, Position, Position, Angle, 10, o, 15.0f);
					}
					PlayBackground(SOUND_SWELLLIFE);
					break;
				case AT_SKILL_ADD_CRITICAL:
					Vector(0.0f, 0.0f, 0.0f, p);
					Vector(1.0f, 0.6f, 0.3f, Light);
					if (c->Weapon[0].Type != -1 &&
						c->Weapon[0].Type != MODEL_BOW + 15)
					{
						b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[0].LinkBone], p, Position, true);
						CreateEffect(ThreadIndex, MODEL_DARKLORD_SKILL, Position, o->Angle, Light, 0);
					}
					if (c->Weapon[1].Type != -1 &&
						c->Weapon[1].Type != MODEL_BOW + 7 &&
						(c->Weapon[1].Type < MODEL_SHIELD || c->Weapon[1].Type >= MODEL_SHIELD + MAX_ITEM_INDEX))
					{
						b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[1].LinkBone], p, Position, true);
						CreateEffect(ThreadIndex, MODEL_DARKLORD_SKILL, Position, o->Angle, Light, 1);
					}
					PlayObject(SOUND_CRITICAL, o);
					break;
				case AT_SKILL_PARTY_TELEPORT:
					CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, o->Light, 2, o);
					CreateEffect(ThreadIndex, MODEL_CIRCLE_LIGHT, o->Position, o->Angle, o->Light, 3);

					PlayObject(SOUND_PART_TELEPORT, o);
					break;
				case AT_SKILL_THUNDER_STRIKE:
					CalcAddPosition(o, 0.0f, -90.0f, -50.0f, Position);
					if (o->CurrentAction == PLAYER_ATTACK_RIDE_ATTACK_FLASH)
					{
						Position[2] += 80.0f;
					}
					else if (o->CurrentAction == PLAYER_FENRIR_ATTACK_DARKLORD_FLASH)
					{
						Position[2] += 40.0f;
					}

					CreateEffect(ThreadIndex, BITMAP_FLARE_FORCE, Position, o->Angle, o->Light, 0, o);

					PlayObject(SOUND_ELEC_STRIKE, o);
					break;
				case AT_SKILL_RIDER:
					CreateEffect(ThreadIndex, BITMAP_SHOTGUN, o->Position, o->Angle, o->Light, 0, o, o->PKKey, ThreadInfo.CurrentSkill);

					PlayBackground(SOUND_SKILL_SWORD3);
					break;
				case AT_SKILL_HELL_FIRE_UP:
				case AT_SKILL_HELL_FIRE_UP + 1:
				case AT_SKILL_HELL_FIRE_UP + 2:
				case AT_SKILL_HELL_FIRE_UP + 3:
				case AT_SKILL_HELL_FIRE_UP + 4:
				case AT_SKILL_HELL:
					CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, o->Light, 0, o, o->PKKey, FindHotKey((c->Skill)));
					CreateEffect(ThreadIndex, MODEL_CIRCLE_LIGHT, o->Position, o->Angle, o->Light);

					if (InHellas() == true)
					{
						AddWaterWave((c->PositionX), (c->PositionY), 2, -1500);
					}

					PlayBackground(SOUND_HELLFIRE);
					break;
				case AT_SKILL_BLAST_HELL:
					CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, o->Light, 1, o);
					StopBackground(SOUND_NUKE1);
					PlayBackground(SOUND_NUKE2);
					break;
				case AT_SKILL_BLAST_POISON:
					Position[0] = (mu_float)(c->SkillX + 0.5f)*g_TerrainScale;
					Position[1] = (mu_float)(c->SkillY + 0.5f)*g_TerrainScale;
					Position[2] = RequestTerrainHeight(Position[0], Position[1]);

					Vector(0.8f, 0.5f, 0.1f, Light);
					CreateEffect(ThreadIndex, MODEL_FIRE, Position, o->Angle, Light, 6, nullptr, 0);
					CreateEffect(ThreadIndex, MODEL_FIRE, Position, o->Angle, Light, 6, nullptr, 0);
					PlayBackground(SOUND_DEATH_POISON1);
					break;
				case AT_SKILL_ICE_UP:
				case AT_SKILL_ICE_UP + 1:
				case AT_SKILL_ICE_UP + 2:
				case AT_SKILL_ICE_UP + 3:
				case AT_SKILL_ICE_UP + 4:
				case AT_SKILL_BLAST_FREEZE:
					{
						EVector3 TargetPosition, Pos;
						TargetPosition[0] = (mu_float)(c->SkillX + 0.5f)*g_TerrainScale;
						TargetPosition[1] = (mu_float)(c->SkillY + 0.5f)*g_TerrainScale;
						TargetPosition[2] = RequestTerrainHeight(TargetPosition[0], TargetPosition[1]);

						for (mu_int32 i = 0; i < 10; ++i)
						{
							Pos[0] = TargetPosition[0];
							Pos[1] = TargetPosition[1];
							Pos[2] = TargetPosition[2] + (GetLargeRand(50))*i;
							CreateEffect(ThreadIndex, MODEL_BLIZZARD, Pos, o->Angle, Light, 0, nullptr, i + 1);
						}

						PlayBackground(SOUND_SUDDEN_ICE1);
					}
					break;

				case AT_SKILL_FLAME:
					Position[0] = (mu_float)(c->SkillX + 0.5f)*g_TerrainScale;
					Position[1] = (mu_float)(c->SkillY + 0.5f)*g_TerrainScale;
					Position[2] = RequestTerrainHeight(Position[0], Position[1]);
					CreateEffect(ThreadIndex, BITMAP_FLAME, Position, o->Angle, o->Light, 0, o, o->PKKey, FindHotKey(AT_SKILL_FLAME));

					if (g_CurrentScene != LOG_IN_SCENE)
						PlayBackground(SOUND_FLAME);
					break;
				case AT_SKILL_STORM:
					CreateEffect(ThreadIndex, MODEL_STORM, o->Position, o->Angle, Light, 0, o, o->PKKey, FindHotKey(AT_SKILL_STORM));
					PlayBackground(SOUND_STORM);
					break;

				case AT_SKILL_FIRE_SCREAM_UP:
				case AT_SKILL_FIRE_SCREAM_UP + 1:
				case AT_SKILL_FIRE_SCREAM_UP + 2:
				case AT_SKILL_FIRE_SCREAM_UP + 3:
				case AT_SKILL_FIRE_SCREAM_UP + 4:
				case AT_SKILL_DARK_SCREAM:
					{
						mu_int32 SkillIndex = FindHotKey((c->Skill));
						OBJECT* pObj = o;
						EVector3 ap, P, dp;
						mu_float BkO = pObj->Angle[2];

						VectorCopy(pObj->Position, ap);
						CreateEffect(ThreadIndex, MODEL_DARK_SCREAM, pObj->Position, pObj->Angle, pObj->Light, 0, pObj, pObj->PKKey, SkillIndex);
						CreateEffect(ThreadIndex, MODEL_DARK_SCREAM_FIRE, pObj->Position, pObj->Angle, pObj->Light, 0, pObj, pObj->PKKey, SkillIndex);

						Vector(80.0f, 0.0f, 0.0f, P);

						pObj->Angle[2] += 10.0f;

						EMatrix4 matrix;
						AngleMatrix(pObj->Angle, matrix);
						VectorRotate(P, matrix, dp);
						VectorAdd(dp, pObj->Position, pObj->Position);
						CreateEffect(ThreadIndex, MODEL_DARK_SCREAM, pObj->Position, pObj->Angle, pObj->Light, 0, pObj, pObj->PKKey, SkillIndex);
						CreateEffect(ThreadIndex, MODEL_DARK_SCREAM_FIRE, pObj->Position, pObj->Angle, pObj->Light, 0, pObj, pObj->PKKey, SkillIndex);

						VectorCopy(ap, pObj->Position);
						VectorCopy(pObj->Position, ap);
						Vector(-80.0f, 0.0f, 0.0f, P);
						pObj->Angle[2] -= 20.0f;

						AngleMatrix(pObj->Angle, matrix);
						VectorRotate(P, matrix, dp);
						VectorAdd(dp, pObj->Position, pObj->Position);
						CreateEffect(ThreadIndex, MODEL_DARK_SCREAM, pObj->Position, pObj->Angle, pObj->Light, 0, pObj, pObj->PKKey, SkillIndex);
						CreateEffect(ThreadIndex, MODEL_DARK_SCREAM_FIRE, pObj->Position, pObj->Angle, pObj->Light, 0, pObj, pObj->PKKey, SkillIndex);
						VectorCopy(ap, pObj->Position);
						pObj->Angle[2] = BkO;

						if ((c->Helper.Type >= MODEL_HELPER + 2 && c->Helper.Type <= MODEL_HELPER + 4) && !c->SafeZone)
						{
							SetAction(o, PLAYER_ATTACK_RIDE_STRIKE);
						}
						else if (c->Helper.Type == MODEL_HELPER + 37 && !c->SafeZone)
						{
							SetAction(o, PLAYER_FENRIR_ATTACK_DARKLORD_STRIKE);
						}
						else
						{
							SetAction(o, PLAYER_ATTACK_STRIKE);
						}

						PlayBackground(SOUND_FIRE_SCREAM);
					}
					break;

				case AT_SKILL_FLASH:
					CalcAddPosition(o, -20.0f, -90.0f, 100.0f, Position);
					CreateEffect(ThreadIndex, BITMAP_BOSS_LASER, Position, o->Angle, Light, 0, o, o->PKKey, FindHotKey(AT_SKILL_FLASH));
					PlayBackground(SOUND_FLASH);
					break;
				case AT_SKILL_BLAST_UP:
				case AT_SKILL_BLAST_UP + 1:
				case AT_SKILL_BLAST_UP + 2:
				case AT_SKILL_BLAST_UP + 3:
				case AT_SKILL_BLAST_UP + 4:
				case AT_SKILL_BLAST:
					Position[0] = (mu_float)(c->SkillX + 0.5f)*g_TerrainScale;
					Position[1] = (mu_float)(c->SkillY + 0.5f)*g_TerrainScale;
					Position[2] = RequestTerrainHeight(Position[0], Position[1]);
					{
						mu_int32 SkillIndex = FindHotKey((c->Skill));
						CreateEffect(ThreadIndex, MODEL_SKILL_BLAST, Position, o->Angle, o->Light, 0, o, o->PKKey, SkillIndex);
						CreateEffect(ThreadIndex, MODEL_SKILL_BLAST, Position, o->Angle, o->Light, 0, o, o->PKKey, SkillIndex);
					}
					break;
				case AT_SKILL_INFERNO:
					CreateInferno(ThreadIndex, o->Position);
					CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, o->Position, o->Angle, o->Light, 0, o, o->PKKey, FindHotKey(AT_SKILL_INFERNO));
					break;
				case AT_SKILL_PLASMA_STORM_FENRIR:
					{
						PlayBackground(SOUND_FENRIR_SKILL);

						OBJECT* p_o[MAX_FENRIR_SKILL_MONSTER_NUM];
						mu_int32 iMonsterNum = 0;

						for (mu_int32 i = 0; i < MAX_CHARACTERS_CLIENT && iMonsterNum < 10; ++i)
						{
							CHARACTER *p_temp_c = &CharactersClient[i];
							if (p_temp_c->Object.Flags.Get(OBJECT::eOF_LIVE) == true)
							{
								if (p_temp_c->Object.Kind == KIND_MONSTER && p_temp_c->Object.CurrentAction != MONSTER01_DIE)
								{
									mu_float dx = c->Object.Position[0] - p_temp_c->Object.Position[0];
									mu_float dy = c->Object.Position[1] - p_temp_c->Object.Position[1];
									mu_float fDistance = EMath::Sqrt(dx*dx + dy * dy) * g_TerrainScaleRev;
									if (fDistance <= GetSkillDistance(AT_SKILL_PLASMA_STORM_FENRIR))
									{
										p_o[iMonsterNum] = &p_temp_c->Object;
										++iMonsterNum;
									}
								}
								else if (p_temp_c->Object.Kind == KIND_PLAYER && p_temp_c->Object.CurrentAction != PLAYER_DIE1)
								{
									if (CheckAttack_Fenrir(p_temp_c) == true && MU_INPUT->IsCtrlPressing() == true)
									{
										mu_float dx = c->Object.Position[0] - p_temp_c->Object.Position[0];
										mu_float dy = c->Object.Position[1] - p_temp_c->Object.Position[1];
										mu_float fDistance = EMath::Sqrt(dx*dx + dy * dy) * g_TerrainScaleRev;
										if (fDistance <= GetSkillDistance(AT_SKILL_PLASMA_STORM_FENRIR))
										{
											p_o[iMonsterNum] = &p_temp_c->Object;
											++iMonsterNum;
										}
									}
								}
							}
						}

						EVector3 vAngle;

						if (c->m_iFenrirSkillTarget != -1 && c->m_iFenrirSkillTarget < MAX_CHARACTERS_CLIENT)
						{
							CHARACTER* p_tc = &CharactersClient[c->m_iFenrirSkillTarget];
							OBJECT* p_to = &p_tc->Object;

							for (mu_int32 j = 0; j < 2; ++j)
							{
								CalcAddPosition(o, 0.0f, -140.0f, 130.0f, Position);
								Vector((mu_float)(GetLargeRand(360)), 0.0f, (mu_float)(GetLargeRand(360)), vAngle);

								CreateJoint(ThreadIndex, MODEL_FENRIR_SKILL_THUNDER, Position, p_to->Position, vAngle, 0 + GetFenrirType(c), p_to, 100.0f);
								CreateJoint(ThreadIndex, MODEL_FENRIR_SKILL_THUNDER, Position, p_to->Position, vAngle, 3 + GetFenrirType(c), p_to, 80.0f);
							}

							for (mu_int32 i = 0; i < iMonsterNum; ++i)
							{
								for (mu_int32 j = 0; j < 2; ++j)
								{
									CalcAddPosition(o, 0.0f, -140.0f, 130.0f, Position);
									Vector((mu_float)(GetLargeRand(360)), 0.0f, (mu_float)(GetLargeRand(360)), vAngle);

									CreateJoint(ThreadIndex, MODEL_FENRIR_SKILL_THUNDER, Position, p_o[i]->Position, vAngle, 0 + GetFenrirType(c), p_o[i], 100.0f);
									CreateJoint(ThreadIndex, MODEL_FENRIR_SKILL_THUNDER, Position, p_o[i]->Position, vAngle, 4 + GetFenrirType(c), p_o[i], 80.0f);
								}
							}

							for (mu_int32 k = 0; k < 6; ++k)
							{
								CalcAddPosition(o, 0.0f, 10.0f + (GetLargeRand(40) - 20), 130.0f, Position);
								Vector((mu_float)(GetLargeRand(360)), 0.0f, (mu_float)(GetLargeRand(360)), vAngle);
								CreateJoint(ThreadIndex, BITMAP_FLARE_FORCE, Position, Position, vAngle, 11 + GetFenrirType(c), nullptr, 60.0f);
							}
						}
						break;
					}
				case AT_SKILL_RUSH:
					CreateEffect(ThreadIndex, MODEL_SWORD_FORCE, o->Position, o->Angle, Light, 0, o);
					PlayBackground(SOUND_BCS_RUSH);
					break;
				case AT_SKILL_ONEFLASH:
					break;

				case AT_SKILL_BRAND_OF_SKILL:
					Vector(0.0f, 0.0f, 0.0f, p);
					Vector(1.0f, 1.0f, 1.0f, Light);
					if (c->Weapon[0].Type != MODEL_BOW + 15)
					{
						b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[0].LinkBone], p, Position, true);
						CreateEffect(ThreadIndex, MODEL_DARKLORD_SKILL, Position, o->Angle, Light, 0);
					}
					if (c->Weapon[1].Type != MODEL_BOW + 7 && (c->Weapon[1].Type < MODEL_SHIELD || c->Weapon[1].Type >= MODEL_SHIELD + MAX_ITEM_INDEX))
					{
						b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[1].LinkBone], p, Position, true);
						CreateEffect(ThreadIndex, MODEL_DARKLORD_SKILL, Position, o->Angle, Light, 1);
					}
					CreateEffect(ThreadIndex, MODEL_MANA_RUNE, o->Position, o->Angle, o->Light);

					PlayBackground(SOUND_BCS_BRAND_OF_SKILL);
					break;
				case AT_SKILL_ALICE_BERSERKER:
					Vector(1.0f, 0.1f, 0.2f, Light);
					CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, o->Position, o->Angle, Light, 11, o);
					CreateEffect(ThreadIndex, MODEL_ALICE_BUFFSKILL_EFFECT, o->Position, o->Angle, Light, 0, o);
					CreateEffect(ThreadIndex, MODEL_ALICE_BUFFSKILL_EFFECT2, o->Position, o->Angle, Light, 0, o);
					PlayBackground(SOUND_SKILL_BERSERKER);
					break;
				case AT_SKILL_ALICE_WEAKNESS:
					Vector(2.0f, 0.1f, 0.1f, Light);
					CreateEffect(ThreadIndex, BITMAP_MAGIC_ZIN, o->Position, o->Angle, Light, 1, nullptr, -1, 0, 0, 0, 7.0f);
					Vector(2.0f, 0.4f, 0.3f, Light);
					CreateEffect(ThreadIndex, BITMAP_MAGIC_ZIN, o->Position, o->Angle, Light, 0, nullptr, -1, 0, 0, 0, 2.0f);
					CreateEffect(ThreadIndex, BITMAP_MAGIC_ZIN, o->Position, o->Angle, Light, 2, nullptr, -1, 0, 0, 0, 1.0f);
					CreateEffect(ThreadIndex, BITMAP_MAGIC_ZIN, o->Position, o->Angle, Light, 2, nullptr, -1, 0, 0, 0, 0.2f);
					CreateEffect(ThreadIndex, BITMAP_MAGIC_ZIN, o->Position, o->Angle, Light, 2, nullptr, -1, 0, 0, 0, 0.1f);
					CreateEffect(ThreadIndex, MODEL_SUMMONER_CASTING_EFFECT2, o->Position, o->Angle, Light, 1, nullptr, -1, 0, 0, 0, 0.6f);
					CreateEffect(ThreadIndex, MODEL_SUMMONER_CASTING_EFFECT22, o->Position, o->Angle, Light, 1, nullptr, -1, 0, 0, 0, 0.6f);
					CreateEffect(ThreadIndex, MODEL_SUMMONER_CASTING_EFFECT222, o->Position, o->Angle, Light, 1, nullptr, -1, 0, 0, 0, 0.6f);
					Vector(1.4f, 0.2f, 0.2f, Light);
					CreateEffect(ThreadIndex, BITMAP_SHINY + 6, o->Position, o->Angle, Light, 0, nullptr, -1, 0, 0, 0, 0.5f);
					CreateEffect(ThreadIndex, BITMAP_PIN_LIGHT, o->Position, o->Angle, Light, 0, nullptr, -1, 0, 0, 0, 1.0f);
					PlayBackground(SOUND_SKILL_WEAKNESS);
					break;
				case AT_SKILL_ALICE_ENERVATION:
					Vector(0.25f, 1.0f, 0.7f, Light);
					CreateEffect(ThreadIndex, BITMAP_MAGIC_ZIN, o->Position, o->Angle, Light, 1, nullptr, -1, 0, 0, 0, 7.0f);
					CreateEffect(ThreadIndex, BITMAP_MAGIC_ZIN, o->Position, o->Angle, Light, 0, nullptr, -1, 0, 0, 0, 2.0f);
					CreateEffect(ThreadIndex, BITMAP_MAGIC_ZIN, o->Position, o->Angle, Light, 2, nullptr, -1, 0, 0, 0, 1.0f);
					CreateEffect(ThreadIndex, BITMAP_MAGIC_ZIN, o->Position, o->Angle, Light, 2, nullptr, -1, 0, 0, 0, 0.2f);
					CreateEffect(ThreadIndex, BITMAP_MAGIC_ZIN, o->Position, o->Angle, Light, 2, nullptr, -1, 0, 0, 0, 0.1f);
					CreateEffect(ThreadIndex, MODEL_SUMMONER_CASTING_EFFECT2, o->Position, o->Angle, Light, 1, nullptr, -1, 0, 0, 0, 0.6f);
					CreateEffect(ThreadIndex, MODEL_SUMMONER_CASTING_EFFECT22, o->Position, o->Angle, Light, 1, nullptr, -1, 0, 0, 0, 0.6f);
					CreateEffect(ThreadIndex, MODEL_SUMMONER_CASTING_EFFECT222, o->Position, o->Angle, Light, 1, nullptr, -1, 0, 0, 0, 0.6f);
					CreateEffect(ThreadIndex, BITMAP_SHINY + 6, o->Position, o->Angle, Light, 0, nullptr, -1, 0, 0, 0, 0.5f);
					CreateEffect(ThreadIndex, BITMAP_PIN_LIGHT, o->Position, o->Angle, Light, 0, nullptr, -1, 0, 0, 0, 1.0f);
					PlayBackground(SOUND_SKILL_ENERVATION);
					break;
				case AT_SKILL_FLAME_STRIKE:
					{
						CreateEffect(ThreadIndex, MODEL_EFFECT_FLAME_STRIKE, o->Position, o->Angle, o->Light, 0, o);
						PlayBackground(SOUND_SKILL_FLAME_STRIKE);
					}
					break;
				case AT_SKILL_GIGANTIC_STORM:
					{
						EBone Matrix;
						EVector3 vAngle, vDirection, vPosition;
						mu_float fAngle;
						for (mu_int32 i = 0; i < 5; ++i)
						{
							Vector(0.0f, 200.0f, 0.0f, vDirection);
							fAngle = o->Angle[2] + i * 72.0f;
							Vector(0.0f, 0.0f, fAngle, vAngle);
							AngleMatrix(vAngle, Matrix);
							VectorRotate(vDirection, Matrix, vPosition);
							VectorAdd(vPosition, o->Position, vPosition);

							CreateEffect(ThreadIndex, BITMAP_JOINT_THUNDER, vPosition, o->Angle, o->Light);
						}
						PlayBackground(SOUND_SKILL_GIGANTIC_STORM);
					}
					break;
				case AT_SKILL_LIGHTNING_SHOCK_UP:
				case AT_SKILL_LIGHTNING_SHOCK_UP + 1:
				case AT_SKILL_LIGHTNING_SHOCK_UP + 2:
				case AT_SKILL_LIGHTNING_SHOCK_UP + 3:
				case AT_SKILL_LIGHTNING_SHOCK_UP + 4:
				case AT_SKILL_LIGHTNING_SHOCK:
					{
						EVector3 vLight;
						Vector(1.0f, 1.0f, 1.0f, vLight);

						CreateEffect(ThreadIndex, MODEL_LIGHTNING_SHOCK, o->Position, o->Angle, vLight, 0, o);
						PlayBackground(SOUND_SKILL_LIGHTNING_SHOCK);
					}
					break;
				}

				if (c->TargetCharacter == -1)
				{
					mu_uint8 Skill = 0;
					if ((c->Skill) == AT_SKILL_CROSSBOW || (AT_SKILL_MANY_ARROW_UP <= (c->Skill) && (c->Skill) <= AT_SKILL_MANY_ARROW_UP + 4))
						Skill = 1;
					if ((o->Type == MODEL_PLAYER &&
						(o->CurrentAction == PLAYER_ATTACK_BOW || o->CurrentAction == PLAYER_ATTACK_CROSSBOW ||
							o->CurrentAction == PLAYER_ATTACK_FLY_BOW || o->CurrentAction == PLAYER_ATTACK_FLY_CROSSBOW ||
							o->CurrentAction == PLAYER_FENRIR_ATTACK_BOW || o->CurrentAction == PLAYER_FENRIR_ATTACK_CROSSBOW ||
							o->CurrentAction == PLAYER_ATTACK_RIDE_BOW || o->CurrentAction == PLAYER_ATTACK_RIDE_CROSSBOW)) ||
							(o->Type != MODEL_PLAYER && o->Kind == KIND_PLAYER)
						)
					{
						if (AT_SKILL_MULTI_SHOT != (c->Skill))
							CreateArrows(ThreadIndex, c, o, nullptr, FindHotKey((c->Skill)), Skill, (c->Skill));
					}

					if (o->Type == MODEL_MONSTER01 + 22 || o->Type == MODEL_MONSTER01 + 35 || o->Type == MODEL_MONSTER01 + 40
						|| o->Type == MODEL_MONSTER01 + 46)
					{
						CreateArrows(ThreadIndex, c, o, nullptr, 0, 0);
					}
				}
				else
				{
					CHARACTER *tc = &CharactersClient[c->TargetCharacter];
					OBJECT *to = &tc->Object;
					if (o->Type == MODEL_PLAYER &&
						(o->CurrentAction == PLAYER_ATTACK_BOW || o->CurrentAction == PLAYER_ATTACK_CROSSBOW ||
							o->CurrentAction == PLAYER_ATTACK_FLY_BOW || o->CurrentAction == PLAYER_ATTACK_FLY_CROSSBOW ||
							o->CurrentAction == PLAYER_ATTACK_RIDE_BOW || o->CurrentAction == PLAYER_ATTACK_RIDE_CROSSBOW
							|| o->CurrentAction == PLAYER_FENRIR_ATTACK_BOW || o->CurrentAction == PLAYER_FENRIR_ATTACK_CROSSBOW
							))
					{
						if (AT_SKILL_MULTI_SHOT != (c->Skill))
							CreateArrows(ThreadIndex, c, o, to, FindHotKey((c->Skill)), 0, (c->Skill));
					}

					if (o->Type == MODEL_MONSTER01 + 22 || o->Type == MODEL_MONSTER01 + 35 || o->Type == MODEL_MONSTER01 + 40)
					{
						CreateArrows(ThreadIndex, c, o, to, 0, 0);
					}

					if (tc->Hit >= 1)
					{
						tc->Hit = 0;
						if (to->Type != MODEL_MONSTER01 + 7)
						{
							for (mu_int32 i = 0; i < 10; ++i)
							{
								Vector(to->Position[0] + (mu_float)(GetLargeRand(64) - 32), to->Position[1] + (mu_float)(GetLargeRand(64) - 32), to->Position[2] + (mu_float)(GetLargeRand(64) + 90), Position);
								CreateParticle(ThreadIndex, BITMAP_BLOOD + 1, Position, o->Angle, Light);
							}
						}

						if (to->Type == MODEL_MONSTER01 + 60)
						{
							for (mu_int32 i = 0; i < 5; ++i)
							{
								if ((GetLargeRand(2)) == 0)
								{
									Position[0] = to->Position[0];
									Position[1] = to->Position[1];
									Position[2] = to->Position[2] + 50 + GetLargeRand(30);

									CreateEffect(ThreadIndex, MODEL_STONE_COFFIN + 1, Position, o->Angle, o->Light);
								}
							}
							PlayBackground(SOUND_HIT_CRISTAL);
						}

						battleCastle::RenderMonsterHitEffect(ThreadIndex, to);
					}
					if (o->CurrentAction >= PLAYER_ATTACK_SKILL_SWORD1 && o->CurrentAction <= PLAYER_ATTACK_SKILL_SWORD5)
					{
						CreateSpark(ThreadIndex, 0, tc, to->Position, o->Angle);
					}
					EVector3 Angle;
					VectorCopy(o->Angle, Angle);
					Angle[2] = CreateAngle(o->Position[0], o->Position[1], to->Position[0], to->Position[1]);
					switch ((c->Skill))
					{
					case AT_SKILL_MANY_ARROW_UP:
					case AT_SKILL_MANY_ARROW_UP + 1:
					case AT_SKILL_MANY_ARROW_UP + 2:
					case AT_SKILL_MANY_ARROW_UP + 3:
					case AT_SKILL_MANY_ARROW_UP + 4:
					case AT_SKILL_CROSSBOW:
						CreateArrows(ThreadIndex, c, o, nullptr, FindHotKey((c->Skill)), 1);
					case AT_SKILL_PIERCING:
						CreateArrows(ThreadIndex, c, o, nullptr, FindHotKey((c->Skill)), 0, (c->Skill));
						break;
					case AT_SKILL_PARALYZE:
						CreateArrows(ThreadIndex, c, o, nullptr, FindHotKey((c->Skill)), 0, (c->Skill));
						break;
					case AT_SKILL_DEEPIMPACT:
						CreateArrows(ThreadIndex, c, o, to, FindHotKey((c->Skill)), 0, (c->Skill));
						PlayBackground(SOUND_BCS_DEEP_IMPACT);
						break;
					case AT_SKILL_HEAL_UP:
					case AT_SKILL_HEAL_UP + 1:
					case AT_SKILL_HEAL_UP + 2:
					case AT_SKILL_HEAL_UP + 3:
					case AT_SKILL_HEAL_UP + 4:
					case AT_SKILL_HEALING:
						CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, to->Position, to->Angle, to->Light, 1, to);
						break;
					case AT_SKILL_DEF_POWER_UP:
					case AT_SKILL_DEF_POWER_UP + 1:
					case AT_SKILL_DEF_POWER_UP + 2:
					case AT_SKILL_DEF_POWER_UP + 3:
					case AT_SKILL_DEF_POWER_UP + 4:
					case AT_SKILL_DEFENSE:
						if (c->SkillSuccess)
						{
							if (g_isCharacterBuff(o, eBuff_Cloaking))
							{
								break;
							}

							CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, to->Position, to->Angle, to->Light, 2, to);

							if (!g_isCharacterBuff(to, eBuff_Defense))
							{
								g_CharacterRegisterBuff(to, eBuff_Defense);

								for (mu_int32 j = 0; j < 5; ++j)
								{
									CreateJoint(ThreadIndex, MODEL_SPEARSKILL, to->Position, to->Position, to->Angle, 4, to, 20.0f, -1, 0, 0, c->TargetCharacter);
								}
							}
							else if (!SearchJoint(MODEL_SPEARSKILL, to, 4)
								&& !SearchJoint(MODEL_SPEARSKILL, to, 9))
							{
								for (mu_int32 j = 0; j < 5; ++j)
								{
									CreateJoint(ThreadIndex, MODEL_SPEARSKILL, to->Position, to->Position, to->Angle, 4, to, 20.0f, -1, 0, 0, c->TargetCharacter);
								}
							}
						}
						break;
					case AT_SKILL_ATT_POWER_UP:
					case AT_SKILL_ATT_POWER_UP + 1:
					case AT_SKILL_ATT_POWER_UP + 2:
					case AT_SKILL_ATT_POWER_UP + 3:
					case AT_SKILL_ATT_POWER_UP + 4:
					case AT_SKILL_ATTACK:
						if (g_isCharacterBuff(o, eBuff_Cloaking)) break;
						CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, to->Position, to->Angle, to->Light, 3, to);
						if (c->SkillSuccess)
						{
							g_CharacterRegisterBuff(to, eBuff_Attack);
						}
						break;
					case AT_SKILL_SLOW:
						CreateEffect(ThreadIndex, MODEL_ICE, to->Position, o->Angle, Light);
						for (mu_int32 i = 0; i < 5; ++i)
							CreateEffect(ThreadIndex, MODEL_ICE_SMALL, to->Position, o->Angle, o->Light);

						if (c->SkillSuccess)
						{
							if (!g_isCharacterBuff(to, eDeBuff_Freeze))
							{
								g_CharacterRegisterBuff(to, eDeBuff_Freeze);
							}
						}
						PlayBackground(SOUND_ICE);
						break;
					case AT_SKILL_SOUL_UP:
					case AT_SKILL_SOUL_UP + 1:
					case AT_SKILL_SOUL_UP + 2:
					case AT_SKILL_SOUL_UP + 3:
					case AT_SKILL_SOUL_UP + 4:
					case AT_SKILL_WIZARDDEFENSE:
						if (o->Type == MODEL_MONSTER01 + 55)
						{
							g_CharacterRegisterBuff(o, eBuff_PhysDefense);
						}
						else
						{
							if (g_isCharacterBuff(to, eBuff_Cloaking)) break;
							g_CharacterRegisterBuff(to, eBuff_PhysDefense);

							PlayBackground(SOUND_SOULBARRIER);
							DeleteJoint(MODEL_SPEARSKILL, to, 0);
							for (mu_int32 j = 0; j < 5; ++j)
							{
								CreateJoint(ThreadIndex, MODEL_SPEARSKILL, to->Position, to->Position, to->Angle, 0, to, 20.0f);
							}
						}
						break;

					case AT_SKILL_POISON:
						if (o->Type == MODEL_PLAYER)
							CreateEffect(ThreadIndex, MODEL_POISON, to->Position, o->Angle, o->Light);
						Vector(0.4f, 0.6f, 1.0f, Light);
						for (mu_int32 i = 0; i < 10; ++i)
							CreateParticle(ThreadIndex, BITMAP_SMOKE, to->Position, o->Angle, Light, 1);

						if (c->SkillSuccess)
						{
							g_CharacterRegisterBuff(to, eDeBuff_Poison);
						}
						PlayBackground(SOUND_HEART);
						break;

					case AT_SKILL_METEO:
						CreateEffect(ThreadIndex, MODEL_FIRE, to->Position, to->Angle, o->Light);
						PlayBackground(SOUND_METEORITE01);
						break;

					case AT_SKILL_JAVELIN:
						CreateEffect(ThreadIndex, MODEL_SKILL_JAVELIN, o->Position, o->Angle, o->Light, 0, to);
						CreateEffect(ThreadIndex, MODEL_SKILL_JAVELIN, o->Position, o->Angle, o->Light, 1, to);
						CreateEffect(ThreadIndex, MODEL_SKILL_JAVELIN, o->Position, o->Angle, o->Light, 2, to);

						PlayBackground(SOUND_BCS_JAVELIN);
						break;

					case AT_SKILL_DEATH_CANNON:
						Vector(0.0f, 0.0f, o->Angle[2], Angle);
						VectorCopy(o->Position, Position);

						Position[2] += 130.0f;
						CreateJoint(ThreadIndex, BITMAP_JOINT_FORCE, Position, Position, Angle, 4, nullptr, 40.0f);

						PlayBackground(SOUND_BCS_DEATH_CANON);
						break;

					case AT_SKILL_SPACE_SPLIT:
						CreateEffect(ThreadIndex, MODEL_PIER_PART, o->Position, o->Angle, o->Light, 2, to);
						PlayBackground(SOUND_BCS_SPACE_SPLIT);
						break;

					case AT_SKILL_FIREBALL:
						CreateEffect(ThreadIndex, MODEL_FIRE, o->Position, Angle, o->Light, 1, to);
						PlayBackground(SOUND_METEORITE01);
						break;

					case AT_SKILL_FLAME:
						Position[0] = to->Position[0];
						Position[1] = to->Position[1];
						Position[2] = RequestTerrainHeight(Position[0], Position[1]);
						CreateEffect(ThreadIndex, BITMAP_FLAME, Position, o->Angle, o->Light, 5, o, o->PKKey, FindHotKey(AT_SKILL_FLAME));
						PlayBackground(SOUND_FLAME);
						break;

					case AT_SKILL_POWERWAVE:
						if (o->Type == MODEL_MONSTER01 + 18)
						{
							Angle[2] += 10.0f;
							CreateEffect(ThreadIndex, MODEL_MAGIC2, o->Position, Angle, o->Light);
							Angle[2] -= 20.0f;
							CreateEffect(ThreadIndex, MODEL_MAGIC2, o->Position, Angle, o->Light);
							Angle[2] += 10.0f;
						}
						CreateEffect(ThreadIndex, MODEL_MAGIC2, o->Position, Angle, o->Light);
						PlayBackground(SOUND_MAGIC);
						break;

					case AT_SKILL_STRONG_PIER:
						CreateEffect(ThreadIndex, MODEL_WAVES, o->Position, o->Angle, o->Light, 1);
						CreateEffect(ThreadIndex, MODEL_WAVES, o->Position, o->Angle, o->Light, 1);
						CreateEffect(ThreadIndex, MODEL_PIERCING2, o->Position, o->Angle, o->Light);
						PlayBackground(SOUND_ATTACK_SPEAR);
						break;
					case AT_SKILL_FIRE_BUST_UP:
					case AT_SKILL_FIRE_BUST_UP + 1:
					case AT_SKILL_FIRE_BUST_UP + 2:
					case AT_SKILL_FIRE_BUST_UP + 3:
					case AT_SKILL_FIRE_BUST_UP + 4:
					case AT_SKILL_LONGPIER_ATTACK:
						{
							EVector3 Angle = { 0.0f, 0.0f, o->Angle[2] };
							EVector3 Pos = { 0.0f, 0.0f, (to->BoundingBoxMax[2] / 1.0f) };

							Vector(80.0f, 0.0f, 20.0f, p);
							b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
							Angle[2] = o->Angle[2] + 90;
							CreateEffect(ThreadIndex, MODEL_PIER_PART, Position, Angle, Pos, 0, to);
							Pos[2] -= to->BoundingBoxMax[2] / 2;
							Angle[2] = o->Angle[2];
							CreateEffect(ThreadIndex, MODEL_PIER_PART, Position, Angle, Pos, 0, to);
							Angle[2] = o->Angle[2] - 90;
							CreateEffect(ThreadIndex, MODEL_PIER_PART, Position, Angle, Pos, 0, to);

							Vector(1.0f, 0.6f, 0.3f, Light);
							CreateEffect(ThreadIndex, MODEL_DARKLORD_SKILL, Position, o->Angle, Light, 0);
							CreateEffect(ThreadIndex, MODEL_DARKLORD_SKILL, Position, o->Angle, Light, 1);
						}
						break;

					case AT_SKILL_ENERGYBALL:
						switch (c->MonsterIndex)
						{
						case 37:
						case 46:
						case 61:
						case 66:
						case 69:
						case 70:
						case 73:
						case 75:
						case 77:

						case 89:
						case 95:
						case 112:
						case 118:
						case 124:
						case 130:
						case 143:
						case 87:
						case 93:
						case 99:
						case 116:
						case 122:
						case 128:
						case 141:
						case 163:
						case 165:
						case 167:
						case 169:
						case 171:
						case 173:
						case 427:
						case 303:
						case 293:
							break;
						default:
							if (o->Type == MODEL_MONSTER01 + 12)
							{
								CreateEffect(ThreadIndex, MODEL_SNOW1, o->Position, Angle, o->Light, 0, to);
							}
							else if (o->Type == MODEL_MONSTER01 + 134)
							{
								CreateEffect(ThreadIndex, MODEL_WOOSISTONE, o->Position, Angle, o->Light, 0, to);
							}
							else if (o->Type == MODEL_MONSTER01 + 138)
							{
								EVector3 vLight;
								Vector(1.0f, 1.0f, 1.0f, vLight);
								CreateEffect(ThreadIndex, MODEL_EFFECT_SAPITRES_ATTACK, o->Position, o->Angle, vLight, 0, to);
							}
							else
							{
								CreateEffect(ThreadIndex, BITMAP_ENERGY, o->Position, Angle, o->Light, 0, to);
								PlayBackground(SOUND_MAGIC);
							}
							break;
						}
						break;

					case AT_SKILL_ALICE_SLEEP:
					case AT_SKILL_ALICE_BLIND:
					case AT_SKILL_ALICE_SLEEP_UP:
					case AT_SKILL_ALICE_SLEEP_UP + 1:
					case AT_SKILL_ALICE_SLEEP_UP + 2:
					case AT_SKILL_ALICE_SLEEP_UP + 3:
					case AT_SKILL_ALICE_SLEEP_UP + 4:
					case AT_SKILL_ALICE_THORNS:
						{
							mu_int32 iSkillIndex = (c->Skill);
							EVector3 vLight;

							if (iSkillIndex == AT_SKILL_ALICE_SLEEP
								|| (AT_SKILL_ALICE_SLEEP_UP <= iSkillIndex && iSkillIndex <= AT_SKILL_ALICE_SLEEP_UP + 4))
							{
								Vector(0.7f, 0.3f, 0.8f, vLight);
							}
							else if (iSkillIndex == AT_SKILL_ALICE_BLIND)
							{
								Vector(1.0f, 1.0f, 1.0f, vLight);
							}
							else if (iSkillIndex == AT_SKILL_ALICE_THORNS
								)
							{
								Vector(0.8f, 0.5f, 0.2f, vLight);
							}

							if (iSkillIndex == AT_SKILL_ALICE_SLEEP || iSkillIndex == AT_SKILL_ALICE_THORNS
								|| (AT_SKILL_ALICE_SLEEP_UP <= iSkillIndex && iSkillIndex <= AT_SKILL_ALICE_SLEEP_UP + 4)
								)
							{
								CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, o->Position, o->Angle, vLight, 11, o);
							}
							else if (iSkillIndex == AT_SKILL_ALICE_BLIND)
							{
								CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, o->Position, o->Angle, vLight, 12, o);
							}

							if (iSkillIndex == AT_SKILL_ALICE_SLEEP
								|| (AT_SKILL_ALICE_SLEEP_UP <= iSkillIndex && iSkillIndex <= AT_SKILL_ALICE_SLEEP_UP + 4)
								)
							{
								Vector(0.8f, 0.3f, 0.9f, vLight);
								CreateEffect(ThreadIndex, MODEL_ALICE_BUFFSKILL_EFFECT, to->Position, to->Angle, vLight, 0, to);
								CreateEffect(ThreadIndex, MODEL_ALICE_BUFFSKILL_EFFECT2, to->Position, to->Angle, vLight, 0, to);
							}
							else if (iSkillIndex == AT_SKILL_ALICE_BLIND)
							{
								Vector(1.0f, 1.0f, 1.0f, vLight);
								CreateEffect(ThreadIndex, MODEL_ALICE_BUFFSKILL_EFFECT, to->Position, to->Angle, vLight, 1, to);
								CreateEffect(ThreadIndex, MODEL_ALICE_BUFFSKILL_EFFECT2, to->Position, to->Angle, vLight, 1, to);
							}
							else if (iSkillIndex == AT_SKILL_ALICE_THORNS)
							{
								Vector(0.8f, 0.5f, 0.2f, vLight);
								CreateEffect(ThreadIndex, MODEL_ALICE_BUFFSKILL_EFFECT, to->Position, to->Angle, vLight, 2, to);
								CreateEffect(ThreadIndex, MODEL_ALICE_BUFFSKILL_EFFECT2, to->Position, to->Angle, vLight, 2, to);
							}
						}
						break;
					}

					VectorCopy(to->Position, Position);
					Position[2] += 120.0f;

					mu_int32 Hand = 0;
					if (o->CurrentAction == PLAYER_ATTACK_SWORD_LEFT1 || o->CurrentAction == PLAYER_ATTACK_SWORD_LEFT2)
						Hand = 1;

					if (tc == Hero)
					{
						Vector(1.0f, 0.0f, 0.0f, Light);
					}
					else
					{
						Vector(1.0f, 0.6f, 0.0f, Light);
					}

					switch (c->AttackFlag)
					{
					case ATTACK_DIE:
						CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, to->Position, to->Position, o->Angle, 0, o, 20.0f);
						CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, to->Position, to->Position, o->Angle, 1, o, 20.0f);
						break;
					}

					switch ((c->Skill))
					{
					case AT_SKILL_HEALING:
					case AT_SKILL_ATT_POWER_UP:
					case AT_SKILL_ATT_POWER_UP + 1:
					case AT_SKILL_ATT_POWER_UP + 2:
					case AT_SKILL_ATT_POWER_UP + 3:
					case AT_SKILL_ATT_POWER_UP + 4:
					case AT_SKILL_ATTACK:
					case AT_SKILL_DEF_POWER_UP:
					case AT_SKILL_DEF_POWER_UP + 1:
					case AT_SKILL_DEF_POWER_UP + 2:
					case AT_SKILL_DEF_POWER_UP + 3:
					case AT_SKILL_DEF_POWER_UP + 4:
					case AT_SKILL_DEFENSE:
					case AT_SKILL_SUMMON:
					case AT_SKILL_SUMMON + 1:
					case AT_SKILL_SUMMON + 2:
					case AT_SKILL_SUMMON + 3:
					case AT_SKILL_SUMMON + 4:
					case AT_SKILL_SUMMON + 5:
					case AT_SKILL_SUMMON + 6:
					case AT_SKILL_HEAL_UP:
					case AT_SKILL_HEAL_UP + 1:
					case AT_SKILL_HEAL_UP + 2:
					case AT_SKILL_HEAL_UP + 3:
					case AT_SKILL_HEAL_UP + 4:

					case AT_SKILL_SOUL_UP:
					case AT_SKILL_SOUL_UP + 1:
					case AT_SKILL_SOUL_UP + 2:
					case AT_SKILL_SOUL_UP + 3:
					case AT_SKILL_SOUL_UP + 4:
					case AT_SKILL_WIZARDDEFENSE:
					case AT_SKILL_BLOW_UP:
					case AT_SKILL_BLOW_UP + 1:
					case AT_SKILL_BLOW_UP + 2:
					case AT_SKILL_BLOW_UP + 3:
					case AT_SKILL_BLOW_UP + 4:
					case AT_SKILL_ONETOONE:
					case AT_SKILL_SPEAR:
					case AT_SKILL_LIFE_UP:
					case AT_SKILL_LIFE_UP + 1:
					case AT_SKILL_LIFE_UP + 2:
					case AT_SKILL_LIFE_UP + 3:
					case AT_SKILL_LIFE_UP + 4:
					case AT_SKILL_VITALITY:
					case AT_SKILL_BLAST_HELL:
					case AT_SKILL_IMPROVE_AG:
					case AT_SKILL_ADD_CRITICAL:
					case AT_SKILL_PARTY_TELEPORT:
					case AT_SKILL_STUN:
					case AT_SKILL_REMOVAL_STUN:
					case AT_SKILL_MANA:
					case AT_SKILL_INVISIBLE:
					case AT_SKILL_REMOVAL_BUFF:
					case AT_SKILL_BRAND_OF_SKILL:
						break;

					default:
						if (68 <= c->MonsterIndex && c->MonsterIndex <= 75)
						{
						}
						else
						{
							ITEM *r = &CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT];
							ITEM *l = &CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT];

							if ((r->Type >= ITEM_BOW && r->Type < ITEM_STAFF) && (l->Type >= ITEM_BOW && l->Type < ITEM_STAFF))
							{
								PlayObject(SOUND_ATTACK01 + 5 + GetLargeRand(4), o);
							}
							else
							{
								PlayObject(SOUND_ATTACK01 + GetLargeRand(4), o);
							}
						}
						break;
					}
				}

				c->Skill = 0;

				c->Damage = 0;
				c->AttackFlag = ATTACK_FAIL;
			}
		}
	}
}

void MUTasking::RenderCharacter(const mu_uint32 ThreadIndex, CHARACTER *c, OBJECT *o, mu_int32 Select)
{
	if (g_isCharacterBuff(o, eBuff_CrywolfNPCHide))
	{
		return;
	}

	MUModel *b = &MODELS::Data[o->Type];
	if (b->_AnimationCount == 0) return;

	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_boolean Translate = true;
	EVector3 p, Position, Light;

	Vector(0.0f, 0.0f, 0.0f, p);
	Vector(1.0f, 1.0f, 1.0f, Light);

	mu_uint8 byRender = CHARACTER_NONE;

	if (o->Alpha >= 0.5f && c->HideShadow == false)
	{
		if (GET_WORLD != WD_10HEAVEN &&
			o->Type == MODEL_PLAYER &&
			InHellas() == false)
		{
			if (InBloodCastle() && o->Flags.Get(OBJECT::eOF_ACTIONSTART) == true && c->Dead > 0)
			{
				mu_float height = RequestTerrainHeight(o->Position[0], o->Position[1]);
				if (height < o->Position[2])
				{
					ThreadInfo.ShadowHeight = height;
				}
			}
			else
			{
				ThreadInfo.ShadowHeight = o->Position[2];
			}

			ThreadInfo.EnableShadow = true;
			if (GET_WORLD == WD_7ATLANSE)
			{
				ThreadInfo.ShadowLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 0.2f);
			}
			else
			{
				ThreadInfo.ShadowLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 1.0f);
			}
		}
	}

	if ((
		o->Type != MODEL_PLAYER && o->Kind != KIND_TRAP &&
		c->MonsterIndex != 25 && c->MonsterIndex != 22 && c->MonsterIndex != 42 && c->MonsterIndex != 242 && c->MonsterIndex != 59 && c->MonsterIndex != 63
		&& c->MonsterIndex != 152
		) && GET_WORLD != WD_10HEAVEN
		)
	{
		if (o->Alpha >= 0.3f)
		{
			if (InBloodCastle() == true && o->Flags.Get(OBJECT::eOF_ACTIONSTART) == true && c->Dead > 0)
			{
				mu_float height = RequestTerrainHeight(o->Position[0], o->Position[1]);
				if (height < o->Position[2])
				{
					ThreadInfo.ShadowHeight = height;
				}
			}
			else
			{
				ThreadInfo.ShadowHeight = o->Position[2];
			}

			if (o->Type != MODEL_MONSTER01 + 60 && o->Type != MODEL_MONSTER01 + 61
				&& !(o->Type >= MODEL_FACE
					&& o->Type <= MODEL_FACE + 5))
			{
				ThreadInfo.EnableShadow = true;
				if (GET_WORLD == WD_7ATLANSE)
				{
					ThreadInfo.ShadowLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 0.2f);
				}
				else
				{
					ThreadInfo.ShadowLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 1.0f);
				}
			}

			if (InHellas())
			{
				ThreadInfo.EnableShadow = false;
			}

			if (WD_10HEAVEN == GET_WORLD || g_Direction.m_CKanturu.IsMayaScene())
			{
				ThreadInfo.EnableShadow = false;
			}

			if (g_isCharacterBuff(o, eBuff_Cloaking))
			{
				ThreadInfo.EnableShadow = false;
			}
		}
	}

	switch (c->MonsterIndex)
	{
	case 89:
	case 95:
	case 112:
	case 118:
	case 124:
	case 130:
	case 143:
		{
			mu_boolean bRender = Calc_RenderObject(ThreadIndex, o, Translate, Select, 0);
			b->ClothMesh(ThreadIndex) = -1;

			if (!c->Object.m_pCloth)
			{
				CPhysicsClothMesh *pCloth = new CPhysicsClothMesh[1];
				pCloth[0].Create(ThreadIndex, &(c->Object), 2, 18, PCT_HEAVY);
				pCloth[0].AddCollisionSphere(0.0f, 0.0f, 0.0f, 50.0f, 18);
				pCloth[0].AddCollisionSphere(0.0f, -20.0f, 0.0f, 30.0f, 18);
				c->Object.m_pCloth = pCloth;
				c->Object.m_byNumCloth = 1;
			}
			CPhysicsCloth *pCloth = c->Object.m_pCloth;

			if (pCloth)
			{
				if (MU_UPDATECOUNTER > 0 && !pCloth[0].Move2(ThreadIndex, 0.005f, 5))
				{
					DeleteCloth(c, o);
				}
				else
				{
					pCloth[0].Render(ThreadIndex, b);
				}
			}

			if (bRender)
			{
				Draw_RenderObject(ThreadIndex, o, Translate, Select, 0);
			}
		}
		break;

	default:
		if (o->Type == MODEL_PLAYER)
		{
			byRender = CHARACTER_ANIMATION;
		}
		else
		{
			byRender = CHARACTER_RENDER_OBJ;
		}
		break;
	}

	if (o->Type == MODEL_PLAYER
		&& (o->SubType == MODEL_XMAS_EVENT_CHA_SSANTA
			|| o->SubType == MODEL_XMAS_EVENT_CHA_SNOWMAN
			|| o->SubType == MODEL_XMAS_EVENT_CHA_DEER))
	{
		OBJECT* pOwner = o->Owner;

		if (pOwner->Flags.Get(OBJECT::eOF_LIVE) == false)
		{
			o->Flags.Clear(OBJECT::eOF_LIVE);
		}

		VectorCopy(pOwner->Position, o->Position);
		VectorCopy(pOwner->Angle, o->Angle);
		o->PriorAction = pOwner->PriorAction;
		o->PriorAnimationFrame = pOwner->PriorAnimationFrame;
		o->CurrentAction = pOwner->CurrentAction;
		o->AnimationFrame = pOwner->AnimationFrame;

		if (g_CurrentTime - o->m_dwTime >= XMAS_EVENT_TIME)
		{
			DeleteCharacter(c, o);
		}
	}
	else if (o->Type == MODEL_PLAYER &&
		(o->SubType == MODEL_XMAS_EVENT_CHANGE_GIRL || 
		o->SubType == MODEL_PANDA ||
		o->SubType == MODEL_SKELETON_CHANGED))
	{
		if (o->m_iAnimation >= 1)
		{
			o->m_iAnimation = 0;
			SetPlayerStop(ThreadIndex, c);
		}
	}

	if (byRender == CHARACTER_ANIMATION)
	{
		if(Calc_ObjectAnimation(ThreadIndex, o, Translate, Select) == false)
		{
			return;
		}
	}

	if (byRender == CHARACTER_RENDER_OBJ)
	{
		if (67 == c->MonsterIndex || 74 == c->MonsterIndex || 75 == c->MonsterIndex
			|| 135 == c->MonsterIndex || 136 == c->MonsterIndex || 137 == c->MonsterIndex
			|| 300 == c->MonsterIndex || 301 == c->MonsterIndex || 302 == c->MonsterIndex || 303 == c->MonsterIndex
			|| 314 == c->MonsterIndex || 315 == c->MonsterIndex || 316 == c->MonsterIndex || 317 == c->MonsterIndex || 318 == c->MonsterIndex || 319 == c->MonsterIndex
			)
		{
			RenderObject(ThreadIndex, o, Translate, Select, c->MonsterIndex);
		}
		else
		{
			if ((c->MonsterIndex == 360 && o->CurrentAction == MONSTER01_ATTACK2))
			{
				RenderObject_AfterImage(ThreadIndex, o, Translate, Select, 0);
			}
			else
			{
				RenderObject(ThreadIndex, o, Translate, Select, 0);
			}
		}
	}

	if ((
		o->Type != MODEL_PLAYER && o->Kind != KIND_TRAP &&
		c->MonsterIndex != 25 && c->MonsterIndex != 22 && c->MonsterIndex != 42 && c->MonsterIndex != 242 && c->MonsterIndex != 59 && c->MonsterIndex != 63
		&& c->MonsterIndex != 152
		) && GET_WORLD != WD_10HEAVEN
		)
	{
		if (o->Alpha >= 0.3f)
		{
			if (InBloodCastle() == true && o->Flags.Get(OBJECT::eOF_ACTIONSTART) == true && c->Dead > 0)
			{
				mu_float height = RequestTerrainHeight(o->Position[0], o->Position[1]);
				if (height < o->Position[2])
				{
					ThreadInfo.ShadowHeight = height;
				}
			}

			if ((c->MonsterIndex == 232 || c->MonsterIndex == 233) &&
				MU_UPDATECOUNTER > 0)
			{
				EVector3 Position, Light;

				VectorCopy(o->Position, Position);
				Position[2] += 20.0f;

				mu_float Luminosity = EMath::Sin(FWorldTime*0.0015f)*0.3f + 0.8f;

				Vector(Luminosity*0.5f, Luminosity*0.5f, Luminosity, Light);
				RenderTerrainAlphaBitmap(BITMAP_MAGIC + 1, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 2.7f, 2.7f, Light, -o->Angle[2]);

				o->HiddenMesh = -1;
			}
		}
	}

	if (c->MonsterIndex == 38 || c->MonsterIndex == 43 || c->MonsterIndex == 52 || c->MonsterIndex == 59 || c->MonsterIndex == 67
		|| (78 <= c->MonsterIndex && c->MonsterIndex <= 83)
		|| (c->MonsterIndex >= 493 && c->MonsterIndex <= 502)
		)
	{
		EVector3 vBackupBodyLight;

		mu_float Bright = 1.0f;
		if (c->MonsterIndex == 59)
			Bright = 0.5f;
		if (c->MonsterIndex == 43
			|| (78 <= c->MonsterIndex && c->MonsterIndex <= 83)
			|| (c->MonsterIndex >= 493 && c->MonsterIndex <= 502)
			)
		{
			if (c->MonsterIndex >= 493)
			{
				VectorCopy(MODELS::Data[o->Type].BodyLight(ThreadIndex), vBackupBodyLight);

				Vector(1.0f, 0.6f, 0.3f, MODELS::Data[o->Type].BodyLight(ThreadIndex));
			}

			if (c->MonsterIndex == 501)
			{
				VectorCopy(MODELS::Data[o->Type].BodyLight(ThreadIndex), vBackupBodyLight);

				Vector(1.0f, 0.0f, 0.0f, MODELS::Data[o->Type].BodyLight(ThreadIndex));

				if(MU_UPDATECOUNTER > 0)
				{
					mu_float	fEffectScale = o->Scale * 1.6f;
					EVector3	v3EffectLightColor, v3EffectPosition;

					Vector(1.0f, 0.6f, 0.1f, v3EffectLightColor);

					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, v3EffectPosition, true);
					CreateSprite(BITMAP_LIGHTMARKS, v3EffectPosition, 2.5f, v3EffectLightColor, o);

					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(4)], p, v3EffectPosition, true);
					CreateSprite(BITMAP_LIGHTMARKS, v3EffectPosition, 2.5f, v3EffectLightColor, o);

					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(57)], p, v3EffectPosition, true);
					CreateSprite(BITMAP_LIGHTMARKS, v3EffectPosition, 2.5f, v3EffectLightColor, o);

					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(60)], p, v3EffectPosition, true);
					CreateSprite(BITMAP_LIGHTMARKS, v3EffectPosition, 2.5f, v3EffectLightColor, o);

					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(87)], p, v3EffectPosition, true);
					CreateSprite(BITMAP_LIGHTMARKS, v3EffectPosition, 2.5f, v3EffectLightColor, o);

					Vector(1.0f, 0.8f, 0.1f, v3EffectLightColor);

					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(57)], p, v3EffectPosition, true);
					CreateEffect(ThreadIndex, MODEL_EFFECT_FIRE_HIK3_MONO, v3EffectPosition, o->Angle, v3EffectLightColor, 1, nullptr, -1, 0, 0, 0, fEffectScale);

					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(60)], p, v3EffectPosition, true);
					CreateEffect(ThreadIndex, MODEL_EFFECT_FIRE_HIK3_MONO, v3EffectPosition, o->Angle, v3EffectLightColor, 1, nullptr, -1, 0, 0, 0, fEffectScale);

					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(66)], p, v3EffectPosition, true);
					CreateEffect(ThreadIndex, MODEL_EFFECT_FIRE_HIK3_MONO, v3EffectPosition, o->Angle, v3EffectLightColor, 1, nullptr, -1, 0, 0, 0, fEffectScale);

					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(78)], p, v3EffectPosition, true);
					CreateEffect(ThreadIndex, MODEL_EFFECT_FIRE_HIK3_MONO, v3EffectPosition, o->Angle, v3EffectLightColor, 1, nullptr, -1, 0, 0, 0, fEffectScale);

					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(91)], p, v3EffectPosition, true);
					CreateEffect(ThreadIndex, MODEL_EFFECT_FIRE_HIK3_MONO, v3EffectPosition, o->Angle, v3EffectLightColor, 1, nullptr, -1, 0, 0, 0, fEffectScale);
				}
			}

			RenderPartObjectBodyColor(ThreadIndex, &MODELS::Data[o->Type], o, o->Type, o->Alpha, RENDER_METAL | RENDER_BRIGHT, Bright);
		}

		if (c->MonsterIndex == 67)
		{
			RenderPartObjectBodyColor(ThreadIndex, &MODELS::Data[o->Type], o, o->Type, o->Alpha, RENDER_CHROME | RENDER_BRIGHT | RENDER_EXTRA, Bright);
		}
		else
		{
			RenderPartObjectBodyColor(ThreadIndex, &MODELS::Data[o->Type], o, o->Type, o->Alpha, RENDER_CHROME | RENDER_BRIGHT, Bright);
		}

		if (c->MonsterIndex >= 493 && c->MonsterIndex <= 502)
		{
			VectorCopy(vBackupBodyLight, MODELS::Data[o->Type].BodyLight(ThreadIndex));
		}
	}
	else if (c->MonsterIndex == 69)
	{
		if(MU_UPDATECOUNTER > 0)
		{
			mu_float Luminosity = (mu_float)(GetLargeRand(30) + 70)*0.01f;
			Vector(Luminosity*0.8f, Luminosity*0.9f, Luminosity*1.0f, Light);
			for (mu_int32 i = 0; i < 9; ++i)
			{
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(g_chStar[i])], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 0.6f, Light, o);
			}

			Vector(Luminosity*0.6f, Luminosity*0.7f, Luminosity*0.8f, Light);
			for (mu_int32 i = 0; i < 3; ++i)
			{
				Vector((mu_float)(GetLargeRand(20) - 10), (mu_float)(GetLargeRand(20) - 10), (mu_float)(GetLargeRand(20) - 10), p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[GetLargeRand(b->_BoneNonDummyEnd)], p, Position, true);
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, o->Angle, Light, 3);
			}
		}
	}
	else if (c->MonsterIndex == 70)
	{
		if(MU_UPDATECOUNTER > 0)
		{
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(20)], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, 0.8f, Light, o);
		}
	}
	else if (c->MonsterIndex == 71 || c->MonsterIndex == 74
		|| c->MonsterIndex == 301
		)
	{
		if (!c->Object.m_pCloth)
		{
			mu_int32 iTex = (c->MonsterIndex == 71) ? BITMAP_ROBE + 3 : BITMAP_ROBE + 5;
			CPhysicsCloth *pCloth = new CPhysicsCloth[1];
			pCloth[0].Create(ThreadIndex, &(c->Object), 19, 0.0f, 10.0f, 0.0f, 5, 15, 30.0f, 300.0f, iTex, iTex, PCT_RUBBER | PCT_MASK_ALPHA);
			c->Object.m_pCloth = pCloth;
			c->Object.m_byNumCloth = 1;
		}
		CPhysicsCloth *pCloth = c->Object.m_pCloth;
		if (MU_UPDATECOUNTER > 0 && !pCloth[0].Move2(ThreadIndex, 0.005f, 5))
		{
			DeleteCloth(c, o);
		}
		else
		{
			pCloth[0].Render(ThreadIndex, b);
		}
	}
	else if (c->MonsterIndex == 73 || c->MonsterIndex == 75)
	{
		if(MU_UPDATECOUNTER > 0)
		{
			EVector3 pos1, pos2;
			switch (c->MonsterIndex)
			{
			case 73:
				Vector(0.1f, 0.1f, 1.0f, Light);
				for (mu_int32 i = 13; i < 27; ++i)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(i)], p, Position, true);
					CreateSprite(BITMAP_LIGHT, Position, 0.8f, Light, o);

					VectorCopy(Position, pos2);
					if ((i >= 14 && i <= 16) || i == 23)
					{
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, pos1, pos2, o->Angle, 7, nullptr, 20.0f);
					}
					VectorCopy(Position, pos1);
				}

				for (mu_int32 i = 52; i < 59; ++i)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(i)], p, Position, true);
					CreateSprite(BITMAP_LIGHT, Position, 0.8f, Light, o);
				}
				break;
			case 75:
				Vector(1.0f, 1.0f, 1.0f, Light);
				for (mu_int32 i = 18; i < 19; ++i)
				{
					Vector(0.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(i)], p, Position, true);
					CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, Light, 0, 0.3f);
				}
				break;
			}
		}

		mu_int32 RenderType = (c->MonsterIndex == 73) ? 0 : RENDER_EXTRA;
		RenderPartObjectBodyColor(ThreadIndex, &MODELS::Data[o->Type], o, o->Type, o->Alpha, RENDER_CHROME | RENDER_BRIGHT | RenderType, 1.0f);
		
		if (c->MonsterIndex == 73)
		{
			RenderPartObjectBodyColor2(ThreadIndex, &MODELS::Data[o->Type], o, o->Type, o->Alpha, RENDER_CHROME2 | RENDER_LIGHTMAP | RENDER_BRIGHT, 1.0f);
		}
	}
	else if (c->MonsterIndex == 77)
	{
		mu_float fSin = 0.5f * (1.0f + EMath::Sin((mu_float)(IWorldTime % 10000) * 0.001f));
		RenderPartObjectBodyColor(ThreadIndex, &MODELS::Data[o->Type], o, o->Type, o->Alpha, RENDER_CHROME | RENDER_BRIGHT, 0.3f + fSin*0.7f);
		fSin = 0.3f * (1.0f - fSin) + 0.3f;
		o->BlendMeshLight = fSin;
		o->BlendMesh = 0;
		RenderObject(ThreadIndex, o, Translate, 2, 0);
		RenderObject(ThreadIndex, o, Translate, 3, 0);
		o->BlendMesh = -1;
		++o->Type;
		RenderObject(ThreadIndex, o, Translate, Select, 0);

		if (!c->Object.m_pCloth)
		{
			CPhysicsCloth *pCloth = new CPhysicsCloth[1];
			pCloth[0].Create(ThreadIndex, o, 10, 0.0f, -10.0f, 0.0f, 5, 12, 15.0f, 240.0f, BITMAP_PHO_R_HAIR, BITMAP_PHO_R_HAIR, PCT_RUBBER | PCT_MASK_ALPHA);
			pCloth[0].AddCollisionSphere(0.0f, 0.0f, 40.0f, 30.0f, 10);
			o->m_pCloth = pCloth;
			o->m_byNumCloth = 1;
		}
		CPhysicsCloth *pCloth = c->Object.m_pCloth;
		if (MU_UPDATECOUNTER > 0 && !pCloth[0].Move2(ThreadIndex, 0.005f, 5))
		{
			DeleteCloth(c, o);
		}
		else
		{
			pCloth[0].Render(ThreadIndex, b);
		}
		--o->Type;
	}

	if (c->MonsterIndex == 53 || c->MonsterIndex == 54)
	{
		RenderPartObjectBodyColor(ThreadIndex, &MODELS::Data[o->Type], o, o->Type, o->Alpha, RENDER_METAL | RENDER_BRIGHT, 1.0f, BITMAP_SHINY + 1);
	}

	if (c->MonsterIndex == 42)
	{
		PART_t *w = &c->Wing;
		w->Type = MODEL_BOSS_HEAD;
		w->LinkBone = b->GetBoneIndex(9);
		w->CurrentAction = 1;
		w->PriorAction = 1;
		w->PlaySpeed = 0.2f;
		RenderLinkObject(ThreadIndex, 0.0f, 0.0f, -40.0f, c, w, w->Type, 0, 0, false, Translate);

		w->Type = MODEL_PRINCESS;
		w->LinkBone = b->GetBoneIndex(61);
		mu_float TempScale = o->Scale;
		EVector3 TempLight;
		VectorCopy(c->Light, TempLight);
		Vector(1.0f, 1.0f, 1.0f, c->Light);
		o->Scale = 0.9f;
		RenderLinkObject(ThreadIndex, 0.0f, -40.0f, 45.0f, c, w, w->Type, 0, 0, false, Translate);
		VectorCopy(TempLight, c->Light);
		o->Scale = TempScale;
	}
	else if (c->MonsterIndex >= 132 && c->MonsterIndex <= 134)
	{
		PART_t *w = &c->Wing;
		w->LinkBone = b->GetBoneIndex(1);
		w->CurrentAction = 1;
		w->PriorAction = 1;
		w->PlaySpeed = 0.2f;
		mu_float TempScale = o->Scale;
		o->Scale = 0.7f;

		if (c->MonsterIndex == 132) w->Type = MODEL_STAFF + 10;
		if (c->MonsterIndex == 133) w->Type = MODEL_SWORD + 19;
		if (c->MonsterIndex == 134)
		{
			w->Type = MODEL_BOW + 18;
			o->Scale = 0.9f;
		}

		RenderLinkObject(ThreadIndex, 0.0f, 0.0f, 0.0f, c, w, w->Type, 0, 0, true, true);
		o->Scale = TempScale;
	}
	else if (c->MonsterIndex == 135)
	{
		RenderPartObjectBodyColor(ThreadIndex, &MODELS::Data[o->Type], o, o->Type, o->Alpha, RENDER_BRIGHT | RENDER_EXTRA | RENDER_PREBLEND, 1.0f);
	}

	if (o->Type == MODEL_MONSTER01 + 127 &&
		MU_UPDATECOUNTER > 0)
	{
		EVector3 vLight;
		EVector3 vPos, vRelatedPos;
		Vector(0.3f, 0.3f, 0.0f, vLight);
		Vector(0.0f, 0.0f, 0.0f, vRelatedPos);

		if (o->CurrentAction == MONSTER01_WALK)
		{
			++o->m_iAnimation;
			if (o->m_iAnimation % 20 == 0)
				PlayBackground(SOUND_MOONRABBIT_WALK);
		}

		if (o->CurrentAction == MONSTER01_SHOCK)
		{
			if (o->AnimationFrame > 2.0f && o->AnimationFrame <= 3.0f)
				PlayBackground(SOUND_MOONRABBIT_DAMAGE);
		}

		if (o->CurrentAction == MONSTER01_DIE)
		{
			if (o->AnimationFrame > 1.0f && o->AnimationFrame <= 2.0f)
				PlayBackground(SOUND_MOONRABBIT_DEAD);

			if (o->AnimationFrame > 9.0f)
			{
				if (o->SubType != -1)
				{
					Vector(0.5f, 0.5f, 0.0f, vLight);
					BoneManager::GetBonePosition(o, 3, vPos);

					Vector(0.7f, 1.0f, 0.6f, vLight);
					EVector3 vMoonPos;
					VectorCopy(vPos, vMoonPos);
					vMoonPos[2] += 28.0f;
					CreateEffect(ThreadIndex, MODEL_MOONHARVEST_MOON, vMoonPos, o->Angle, vLight, 0, nullptr, -1, 0, 0, 0, 0.5f);

					Vector(0.4f, 0.4f, 0.8f, vLight);
					CreateParticle(ThreadIndex, BITMAP_EXPLOTION_MONO, vPos, o->Angle, vLight, 10, 1.0f);

					Vector(1.0f, 1.0f, 1.0f, vLight);
					for (mu_int32 i = 0; i < 200; ++i)
					{
						CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, vLight, 21);
					}

					for (mu_int32 i = 0; i < 150; ++i)
					{
						CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, vLight, 22);
					}
					Vector(1.0f, 1.0f, 1.0f, vLight);
					switch (o->SubType)
					{
					case 0:
						{
							for (mu_int32 i = 0; i < 10; ++i)
							{
								CreateEffect(ThreadIndex, MODEL_MOONHARVEST_GAM, vPos, o->Angle, vLight);
							}
						}
						break;
					case 1:
						{
							for (mu_int32 i = 0; i < 5; ++i)
							{
								CreateEffect(ThreadIndex, MODEL_MOONHARVEST_SONGPUEN1, vPos, o->Angle, vLight);
								CreateEffect(ThreadIndex, MODEL_MOONHARVEST_SONGPUEN2, vPos, o->Angle, vLight);
							}
						}
						break;
					case 2:
						{
							for (mu_int32 i = 0; i < 10; ++i)
							{
								CreateEffect(ThreadIndex, MODEL_NEWYEARSDAY_EVENT_BEKSULKI, vPos, o->Angle, vLight);
							}
						}
						break;
					}

					o->SubType = -1;

					if (o->AnimationFrame <= 10.0f && MU_UPDATECOUNTER > 0)
					{
						Vector(1.0f, 0.0f, 0.0f, vLight);
						CreateSprite(BITMAP_LIGHT, vPos, 8.0f, vLight, o);
					}
				}
			}
		}
		else
		{
			BoneManager::GetBonePosition(o, 3, vPos);
			Vector(0.4f, 0.4f, 0.9f, vLight);
			CreateSprite(BITMAP_LIGHT, vPos, 3.0f, vLight, o);

			BoneManager::GetBonePosition(o, 16, vPos);
			Vector(0.4f, 0.4f, 0.9f, vLight);
			CreateSprite(BITMAP_LIGHT, vPos, 2.0f, vLight, o);

			BoneManager::GetBonePosition(o, 2, vPos);
			Vector(0.4f, 0.4f, 0.9f, vLight);
			CreateSprite(BITMAP_LIGHT, vPos, 1.0f, vLight, o);
		}
	}
	if (o->Type == MODEL_NPC_CHERRYBLOSSOM &&
		MU_UPDATECOUNTER > 0)
	{
		EVector3 vRelativePos, vtaWorldPos, vLight, vLight1, vLight2;
		MUModel *b = &MODELS::Data[MODEL_NPC_CHERRYBLOSSOM];

		Vector(0.0f, 0.0f, 0.0f, vRelativePos);
		Vector(1.0f, 0.6f, 0.8f, vLight);
		Vector(0.3f, 0.3f, 0.3f, vLight1);
		Vector(1.0f, 1.0f, 1.0f, vLight2);

		mu_float Scale = EMath::Abs(EMath::Sin(FWorldTime*0.002f))*0.2f;
		mu_float Luminosity = EMath::Abs(EMath::Sin(FWorldTime*0.002f))*0.4f;
		Vector(0.5f + Luminosity, 0.0f + Luminosity, 0.0f + Luminosity, Light);

		if (GetLargeRand(2) == 0)
		{
			VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));
			b->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(43)], vRelativePos, vtaWorldPos, false);
			vtaWorldPos[2] += 20.0f;

			CreateParticle(ThreadIndex, BITMAP_CHERRYBLOSSOM_EVENT_PETAL, vtaWorldPos, o->Angle, GetLargeRand(3) == 0 ? vLight : vLight1, 1, 0.3f);
		}

		VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));
		b->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);
		b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(43)], vRelativePos, vtaWorldPos, false);
		vtaWorldPos[2] += 20.0f;

		CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vtaWorldPos, o->Angle, GetLargeRand(3) == 0 ? vLight2 : vLight1, 25, Scale + 0.2f);
		CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vtaWorldPos, o->Angle, GetLargeRand(2) == 0 ? vLight2 : vLight1, 25, Scale + 0.3f);

		Vector(0.7f, 0.5f, 0.2f, vLight);
		CreateSprite(BITMAP_LIGHT, vtaWorldPos, 2.0f, vLight, o, 0.0f);

		Vector(0.7f, 0.2f, 0.6f, vLight);
		VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));
		b->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);
		b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(3)], vRelativePos, vtaWorldPos, false);
		CreateSprite(BITMAP_LIGHT, vtaWorldPos, 5.0f, vLight, o, 0.0f);

		mu_int32 iRedFlarePos[] = { 53, 56, 59, 62 };
		for (mu_int32 i = 0; i < 4; ++i)
		{
			Vector(0.9f, 0.4f, 0.8f, vLight);
			VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));
			b->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iRedFlarePos[i])], vRelativePos, vtaWorldPos, false);

			CreateSprite(BITMAP_LIGHT, vtaWorldPos, 1.5f, vLight, o, 0.0f);

			if (GetLargeRand(3) == 0)
			{
				mu_float randpos = (mu_float)(GetLargeRand(30) + 5);

				if (GetLargeRand(2) == 0) {
					vtaWorldPos[0] += randpos;
				}
				else {
					vtaWorldPos[0] -= randpos;
				}

				Vector(1.0f, 0.8f, 0.4f, vLight);
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vtaWorldPos, o->Angle, vLight, 15, Scale + 0.4f);
			}
		}
	}

	mu_boolean fullset = false;
	if (o->Type == MODEL_PLAYER)
	{
		if (c->HideShadow == false)
		{
			fullset = CheckFullSet(ThreadIndex, c);
		}
	}

	mu_boolean bCloak = false;

	if ((
		GetCharacterClass(c->Class) == GAME::CLASS_DARK
		|| GetBaseClass(c->Class) == GAME::CLASS_DARK_LORD
		) && o->Type == MODEL_PLAYER)
	{
		if (c->Change == false || (c->Change == true && c->Object.Type == MODEL_PLAYER))
		{
			bCloak = true;
		}
	}

	if (c->MonsterIndex == 55 || c->MonsterIndex == 361)
	{
		bCloak = true;
	}
	else if (c->MonsterIndex >= 529 && c->MonsterIndex <= 539)
	{
		bCloak = false;
	}

	if (InChaosCastle() == true)
	{
		bCloak = false;
	}

	EVector3 CloakLight;
	Vector(1.0f, 1.0f, 1.0f, CloakLight);
	if (c->GuildMarkIndex != -1)
	{
		if (EnableSoccer)
		{
			if (mu_strcmp(GuildMark[Hero->GuildMarkIndex].GuildName.c_str(), GuildMark[c->GuildMarkIndex].GuildName.c_str()) == 0)
			{
				bCloak = true;
				if (HeroSoccerTeam == 0)
				{
					Vector(1.0f, 0.2f, 0.0f, CloakLight);
				}
				else
				{
					Vector(0.0f, 0.2f, 1.0f, CloakLight);
				}
			}

			if (mu_strcmp(GuildWarName, GuildMark[c->GuildMarkIndex].GuildName.c_str()) == 0)
			{
				bCloak = true;
				if (HeroSoccerTeam == 0)
				{
					Vector(0.0f, 0.2f, 1.0f, CloakLight);
				}
				else
				{
					Vector(1.0f, 0.2f, 0.0f, CloakLight);
				}
			}
		}
		if (SoccerObserver)
		{
			if (mu_strcmp(SoccerTeamName[0], GuildMark[c->GuildMarkIndex].GuildName.c_str()) == 0)
			{
				bCloak = true;
				Vector(1.0f, 0.2f, 0.0f, CloakLight);
			}
			if (mu_strcmp(SoccerTeamName[1], GuildMark[c->GuildMarkIndex].GuildName.c_str()) == 0)
			{
				bCloak = true;
				Vector(0.0f, 0.2f, 1.0f, CloakLight);
			}
		}
	}

	if (g_DuelMgr.IsDuelEnabled())
	{
		if (g_DuelMgr.IsDuelPlayer(c, DUEL_ENEMY, false))
		{
			bCloak = true;
			Vector(1.0f, 0.2f, 0.0f, CloakLight);
		}
		else if (g_DuelMgr.IsDuelPlayer(c, DUEL_HERO, false))
		{ 
			bCloak = true;
			Vector(0.0f, 0.2f, 1.0f, CloakLight);
		}
	}

	if (g_CurrentScene == CHARACTER_SCENE)
	{
		Vector(0.4f, 0.4f, 0.4f, Light);
	}
	else
	{
		RequestTerrainLight(o->Position[0], o->Position[1], Light);
	}

	VectorAdd(Light, o->Light, c->Light);
	if (o->Type == MODEL_MONSTER01 + 55)
	{
		Vector(.6f, .6f, .6f, c->Light);
	}
	else if (c->MonsterIndex >= 529 && c->MonsterIndex <= 539)
	{
		c->HideShadow = true;

		if (GET_WORLD == WD_65DOPPLEGANGER1)
		{
			Vector(0.5f, 0.7f, 1.0f, c->Light);
			c->Object.Alpha = 0.7f;
		}
		else
		{
			Vector(1.0f, 0.3f, 0.1f, c->Light);
		}

		if (GET_WORLD == WD_65DOPPLEGANGER1);
		else if (o->CurrentAction != PLAYER_DIE1)
		{
			c->Object.Alpha = 1.0f;
		}
		else
		{
			c->Object.Alpha -= 0.07f;
			mu_float fAlpha = c->Object.Alpha;
			if (fAlpha < 0) fAlpha = 0;

			Vector(1.0f * fAlpha, 0.3f * fAlpha, 0.1f * fAlpha, c->Light);

			if(MU_UPDATECOUNTER > 0)
			{
				EVector3 vPos;
				const mu_int32 iNumBones = MODELS::Data[o->Type]._BoneNonDummyEnd;
				{
					MODELS::Data[o->Type].TransformByObjectBoneDirect(ThreadIndex, vPos, o, GetLargeRand(iNumBones));
					CreateParticle(ThreadIndex, BITMAP_TWINTAIL_WATER, vPos, o->Angle, c->Light, 2, 0.5f);
				}
			}
		}
		c->Object.BlendMesh = -1;

		if (g_isCharacterBuff((&c->Object), eBuff_Doppelganger_Ascension))
		{
			o->Position[2] += 2.0f;
		}
	}

	if (c == Hero)
	{
		EVector3 AbilityLight = { 1.0f, 1.0f, 1.0f };
		if (CharacterAttribute->Ability & ABILITY_FAST_ATTACK_SPEED)
		{
			AbilityLight[0] *= 0.9f; AbilityLight[1] *= 0.5f; AbilityLight[2] *= 0.5f;
		}
		if (CharacterAttribute->Ability & ABILITY_PLUS_DAMAGE)
		{
			AbilityLight[0] *= 0.5f; AbilityLight[1] *= 0.9f; AbilityLight[2] *= 0.5f;
		}
		if (CharacterAttribute->Ability & ABILITY_FAST_ATTACK_RING)
		{
			AbilityLight[0] *= 0.9f; AbilityLight[1] *= 0.5f; AbilityLight[2] *= 0.5f;
		}
		if (CharacterAttribute->Ability & ABILITY_FAST_ATTACK_SPEED2)
		{
			AbilityLight[0] *= 0.9f; AbilityLight[1] *= 0.5f; AbilityLight[2] *= 0.5f;
		}
		VectorCopy(AbilityLight, c->Light);
	}

	if (o->Kind == KIND_NPC && GET_WORLD == WD_0LORENCIA && o->Type == MODEL_PLAYER && (o->SubType >= MODEL_SKELETON1&&o->SubType <= MODEL_SKELETON3))
	{
		RenderPartObject(ThreadIndex, &c->Object, o->SubType, nullptr, c->Light, o->Alpha, c->Level << 3, 0, 0, false, false, Translate, Select);
	}
	else if (o->Kind == KIND_PLAYER && o->Type == MODEL_PLAYER && o->SubType == MODEL_SKELETON_PCBANG)
	{
		RenderPartObject(ThreadIndex, &c->Object, o->SubType, nullptr, c->Light, o->Alpha, 0, 0, 0, false, false, Translate, Select);
	}
	else if (o->Kind == KIND_PLAYER && o->Type == MODEL_PLAYER && o->SubType == MODEL_HALLOWEEN)
	{
		RenderPartObject(ThreadIndex, &c->Object, o->SubType, nullptr, c->Light, o->Alpha, 0, 0, 0, false, false, Translate, Select);
	}
	else if (o->Kind == KIND_PLAYER && o->Type == MODEL_PLAYER && o->SubType == MODEL_CURSEDTEMPLE_ALLIED_PLAYER)
	{
		RenderPartObject(ThreadIndex, &c->Object, o->SubType, nullptr, c->Light, o->Alpha, 0, 0, 0, false, false, Translate, Select);
	}
	else if (o->Kind == KIND_PLAYER && o->Type == MODEL_PLAYER && o->SubType == MODEL_CURSEDTEMPLE_ILLUSION_PLAYER)
	{
		RenderPartObject(ThreadIndex, &c->Object, o->SubType, nullptr, c->Light, o->Alpha, 0, 0, 0, false, false, Translate, Select);
	}
	else if (o->Kind == KIND_PLAYER && o->Type == MODEL_PLAYER && o->SubType == MODEL_PANDA)
	{
		RenderPartObject(ThreadIndex, &c->Object, o->SubType, nullptr, c->Light, o->Alpha, 0, 0, 0, false, false, Translate, Select);
	}
	else if (o->Kind == KIND_PLAYER && o->Type == MODEL_PLAYER && o->SubType == MODEL_SKELETON_CHANGED)
	{
		RenderPartObject(ThreadIndex, &c->Object, o->SubType, nullptr, c->Light, o->Alpha, 0, 0, 0, false, false, Translate, Select);
	}
	else if (o->Kind == KIND_PLAYER && o->Type == MODEL_PLAYER && o->SubType == MODEL_XMAS_EVENT_CHANGE_GIRL)
	{
		RenderPartObject(ThreadIndex, &c->Object, o->SubType, nullptr, c->Light, o->Alpha, 0, 0, 0, false, false, Translate, Select);

		if (c->m_pTempParts == nullptr)
		{
			c->m_pTempParts = new CSParts(MODEL_XMAS_EVENT_EARRING, 20);
			OBJECT* pObj = c->m_pTempParts->GetObject();
			pObj->Velocity = 0.25f;
			pObj->Owner = &c->Object;
		}
		else
		{
			RenderParts(ThreadIndex, c);
		}
	}
	else if (o->Kind == KIND_PLAYER && o->Type == MODEL_PLAYER && o->SubType == MODEL_GM_CHARACTER)
	{
		RenderPartObject(ThreadIndex, &c->Object, o->SubType, nullptr, c->Light, o->Alpha, 0, 0, 0, false, false, Translate, Select);

		if (!g_isCharacterBuff(o, eBuff_Cloaking))
		{
			if(MU_UPDATECOUNTER > 0)
			{
				EVector3 vLight, vPos;
				Vector(0.4f, 0.6f, 0.8f, vLight);
				VectorCopy(o->Position, vPos);
				vPos[2] += 100.0f;
				CreateSprite(BITMAP_LIGHT, vPos, 6.0f, vLight, o, 0.5f);

				mu_float fLumi;
				fLumi = EMath::Sin(FWorldTime*0.05f)*0.4f + 0.9f;
				Vector(fLumi*0.3f, fLumi*0.5f, fLumi*0.8f, vLight);
				CreateSprite(BITMAP_LIGHT, vPos, 2.0f, vLight, o);

				Vector(0.3f, 0.2f, 1.0f, vLight);
				RenderAurora(BITMAP_MAGIC + 1, RENDER_BRIGHT, o->Position[0], o->Position[1], 2.5f, 2.5f, vLight);

				Vector(1.0f, 1.0f, 1.0f, vLight);
				fLumi = EMath::Sin(FWorldTime*0.0015f)*0.3f + 0.5f;
				Vector(fLumi*vLight[0], fLumi*vLight[1], fLumi*vLight[2], vLight);
				RenderTerrainAlphaBitmap(BITMAP_GM_AURORA, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 1.5f, 1.5f, vLight, FWorldTime*0.01f);
				RenderTerrainAlphaBitmap(BITMAP_GM_AURORA, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 1.0f, 1.0f, vLight, -FWorldTime*0.01f);
			}

			if (c->Object.m_pCloth == nullptr)
			{
				CPhysicsCloth *pCloth = new CPhysicsCloth[2];

				pCloth[0].Create(ThreadIndex, o, 20, 0.0f, 5.0f, 10.0f, 6, 5, 30.0f, 70.0f, BITMAP_GM_HAIR_1, BITMAP_GM_HAIR_1, PCT_COTTON | PCT_SHORT_SHOULDER | PCT_MASK_ALPHA);
				pCloth[0].SetWindMinMax(10, 20);
				pCloth[0].AddCollisionSphere(-10.0f, 20.0f, 20.0f, 27.0f, 17);
				pCloth[0].AddCollisionSphere(10.0f, 20.0f, 20.0f, 27.0f, 17);

				pCloth[1].Create(ThreadIndex, o, 20, 0.0f, 5.0f, 25.0f, 4, 4, 30.0f, 40.0f, BITMAP_GM_HAIR_3, BITMAP_GM_HAIR_3, PCT_COTTON | PCT_SHORT_SHOULDER | PCT_MASK_ALPHA);
				pCloth[1].SetWindMinMax(8, 15);
				pCloth[1].AddCollisionSphere(-10.0f, 20.0f, 20.0f, 27.0f, 17);
				pCloth[1].AddCollisionSphere(10.0f, 20.0f, 20.0f, 27.0f, 17);

				o->m_pCloth = pCloth;
				o->m_byNumCloth = 2;
			}

			CPhysicsCloth *pCloth = c->Object.m_pCloth;

			if (g_isCharacterBuff(o, eBuff_Cloaking))
			{
				for (mu_int32 i = 0; i < o->m_byNumCloth; ++i)
				{
					if (MU_UPDATECOUNTER > 0 && pCloth[i].Move2(ThreadIndex, 0.005f, 5) == false)
					{
						DeleteCloth(c, o);
					}
					else
					{
						EVector3 vLight;
						Vector(1.0f, 1.0f, 1.0f, vLight);
						pCloth[i].Render(ThreadIndex, b, &vLight);
					}
				}
			}
		}
	}
	else if (o->Type == MODEL_PLAYER
		&& (o->SubType == MODEL_XMAS_EVENT_CHA_SSANTA
			|| o->SubType == MODEL_XMAS_EVENT_CHA_SNOWMAN
			|| o->SubType == MODEL_XMAS_EVENT_CHA_DEER))
	{
		EVector3 vOriginPos;
		VectorCopy(o->Position, vOriginPos);
		o->Position[0] += 50.0f;
		o->Position[1] += 50.0f;
		o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
		RenderPartObject(ThreadIndex, &c->Object, o->SubType, nullptr, c->Light, o->Alpha, 0, 0, 0, false, false, Translate, Select);

		VectorCopy(vOriginPos, o->Position);
		o->Position[0] -= 50.0f;
		o->Position[1] -= 50.0f;
		o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
		RenderPartObject(ThreadIndex, &c->Object, o->SubType, nullptr, c->Light, o->Alpha, 0, 0, 0, false, false, Translate, Select);

		VectorCopy(vOriginPos, o->Position);
		o->Position[0] += 50.0f;
		o->Position[1] -= 50.0f;
		o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
		RenderPartObject(ThreadIndex, &c->Object, o->SubType, nullptr, c->Light, o->Alpha, 0, 0, 0, false, false, Translate, Select);
	}
	else if (o->Type == MODEL_PLAYER && (o->SubType >= MODEL_SKELETON1&&o->SubType <= MODEL_SKELETON3))
	{
		RenderPartObject(ThreadIndex, &c->Object, o->SubType, nullptr, c->Light, o->Alpha, 0, 0, 0, false, false, Translate, Select);
	}
	else if (!c->Change)
	{
		if ((o->Kind == KIND_PLAYER && InChaosCastle() == true)
			)
		{
			mu_int32 RenderType = RENDER_TEXTURE;
			PART_t* p = &c->BodyPart[BODYPART_HEAD];
			if (c == Hero)
			{
				RenderType |= RENDER_CHROME;
			}

			RenderPartObject(ThreadIndex, &c->Object, MODEL_ANGEL, p, c->Light, o->Alpha, 0, 0, 0, false, false, Translate, Select, RenderType);
		}
		else
		{
			for (mu_int32 i = MAX_BODYPART - 1; i >= 0; --i)
			{
				PART_t *p = &c->BodyPart[i];
				if (p->Type != -1)
				{
					mu_int32 Type = p->Type;

					if (GAME::CLASS_SUMMONER == GetBaseClass(c->Class))
					{
						mu_int32 nItemType = (Type - MODEL_ITEM) / MAX_ITEM_INDEX;
						mu_int32 nItemSubType = (Type - MODEL_ITEM) % MAX_ITEM_INDEX;

						if (nItemType >= 7 && nItemType <= 11
							&& (nItemSubType == 10 || nItemSubType == 11))
						{
							Type = MODEL_HELM2
								+ (nItemType - 7) * MODEL_ITEM_COMMON_NUM
								+ nItemSubType - 10;
						}
					}

					MUModel *b = &MODELS::Data[Type];

					b->Skin(ThreadIndex) = GetBaseClass(c->Class) * 2 + IsSecondClass(c->Class);

					if (GetBaseClass(c->Class) == GAME::CLASS_DARK_LORD && i == BODYPART_HELM)
					{
						o->BlendMeshLight = EMath::Sin(FWorldTime*0.001f)*0.1f + 0.7f;
						if (i == BODYPART_HELM)
						{
							mu_int32 index = Type - MODEL_HELM;
							if (index == 0 || index == 5 || index == 6 || index == 8 || index == 9)
							{
								Type = MODEL_MASK_HELM + index;
								MODELS::Data[Type].Skin(ThreadIndex) = b->Skin(ThreadIndex);
							}
						}
					}

					if (c->MonsterIndex >= 529 && c->MonsterIndex <= 539)
					{
						if (GET_WORLD == WD_65DOPPLEGANGER1)
							RenderPartObject(ThreadIndex, &c->Object, Type, p, c->Light, o->Alpha, p->Level, p->ExcOption, p->SetOption, false, false, Translate,
								Select, RENDER_DOPPELGANGER | RENDER_TEXTURE);
						else
							RenderPartObject(ThreadIndex, &c->Object, Type, p, c->Light, o->Alpha, p->Level, p->ExcOption, p->SetOption, false, false, Translate,
								Select, RENDER_DOPPELGANGER | RENDER_BRIGHT | RENDER_TEXTURE);
					}
					else
					{
						RenderPartObject(ThreadIndex, &c->Object, Type, p, c->Light, o->Alpha, p->Level, p->ExcOption, p->SetOption, false, false, Translate, Select);
					}
				}
			}
		}

		if (InChaosCastle() == false)
		{
			if (c->GuildMarkIndex >= 0 &&
				o->Type == MODEL_PLAYER &&
				o->Alpha != 0.0f &&
				!g_isCharacterBuff(o, eBuff_Cloaking))
			{
				RenderGuild(o, c->BodyPart[BODYPART_ARMOR].Type, BITMAP_GUILD_BEGIN + c->GuildMarkIndex);
			}
		}
	}

	if (bCloak)
	{
		if (!c->Object.m_pCloth)
		{
			if (GetBaseClass(c->Class) == GAME::CLASS_DARK_LORD)
			{
				mu_int32 numCloth = 4;
				if (c->Wing.Type == MODEL_WING + 40)
				{
					numCloth = 6;
				}
				else
				{
					numCloth = 4;
				}

				CPhysicsCloth *pCloth = new CPhysicsCloth[numCloth];

				pCloth[0].Create(ThreadIndex, o, 20, 0.0f, 0.0f, 20.0f, 6, 5, 30.0f, 70.0f, BITMAP_ROBE + 6, BITMAP_ROBE + 6, PCT_CURVED | PCT_RUBBER2 | PCT_MASK_LIGHT | PLS_STRICTDISTANCE | PCT_SHORT_SHOULDER | PCT_NORMAL_THICKNESS | PCT_OPT_HAIR);
				pCloth[0].SetWindMinMax(10, 50);
				pCloth[0].AddCollisionSphere(-10.0f, 20.0f, 20.0f, 27.0f, 17);
				pCloth[0].AddCollisionSphere(10.0f, 20.0f, 20.0f, 27.0f, 17);

				pCloth[1].Create(ThreadIndex, o, 20, 0.0f, 5.0f, 18.0f, 5, 5, 30.0f, 70.0f, BITMAP_ROBE + 6, BITMAP_ROBE + 6, PCT_CURVED | PCT_RUBBER2 | PCT_MASK_BLEND | PLS_STRICTDISTANCE | PCT_SHORT_SHOULDER | PCT_NORMAL_THICKNESS | PCT_OPT_HAIR);
				pCloth[1].SetWindMinMax(8, 40);
				pCloth[1].AddCollisionSphere(-10.0f, 20.0f, 20.0f, 27.0f, 17);
				pCloth[1].AddCollisionSphere(10.0f, 20.0f, 20.0f, 27.0f, 17);

				if (c->Wing.Type == MODEL_WING + 40)
				{
					pCloth[2].Create(ThreadIndex, o, 19, 0.0f, 8.0f, 10.0f, 10, 10, 180.0f, 180.0f, BITMAP_ROBE + 9, BITMAP_ROBE + 9, PCT_CURVED | PCT_SHORT_SHOULDER | PCT_HEAVY | PCT_MASK_ALPHA);
					pCloth[2].AddCollisionSphere(-10.0f, -10.0f, -10.0f, 25.0f, 17);
					pCloth[2].AddCollisionSphere(10.0f, -10.0f, -10.0f, 25.0f, 17);
					pCloth[2].AddCollisionSphere(-10.0f, -10.0f, 20.0f, 27.0f, 17);
					pCloth[2].AddCollisionSphere(10.0f, -10.0f, 20.0f, 27.0f, 17);
				}
				else if (c->Wing.Type == MODEL_WING + 130)
				{
					pCloth[2].Create(ThreadIndex, o, 19, 0.0f, 8.0f, 10.0f, 10, 10, 100.0f, 100.0f, BITMAP_ROBE + 7, BITMAP_ROBE + 7, PCT_CURVED | PCT_SHORT_SHOULDER | PCT_MASK_ALPHA);
					pCloth[2].AddCollisionSphere(-10.0f, -10.0f, -10.0f, 25.0f, 17);
					pCloth[2].AddCollisionSphere(10.0f, -10.0f, -10.0f, 25.0f, 17);
					pCloth[2].AddCollisionSphere(-10.0f, -10.0f, 20.0f, 27.0f, 17);
					pCloth[2].AddCollisionSphere(10.0f, -10.0f, 20.0f, 27.0f, 17);
				}
				else
				{
					pCloth[2].Create(ThreadIndex, o, 19, 0.0f, 8.0f, 10.0f, 10, 10, 180.0f, 180.0f, BITMAP_ROBE + 7, BITMAP_ROBE + 7, PCT_CURVED | PCT_SHORT_SHOULDER | PCT_MASK_ALPHA);
					pCloth[2].AddCollisionSphere(-10.0f, -10.0f, -10.0f, 25.0f, 17);
					pCloth[2].AddCollisionSphere(10.0f, -10.0f, -10.0f, 25.0f, 17);
					pCloth[2].AddCollisionSphere(-10.0f, -10.0f, 20.0f, 27.0f, 17);
					pCloth[2].AddCollisionSphere(10.0f, -10.0f, 20.0f, 27.0f, 17);
				}

				if (c->Wing.Type == MODEL_WING + 40)
				{
					numCloth = 6;
				}
				else
				{
					numCloth = 3;
				}

				if (c->BodyPart[BODYPART_ARMOR].Type == MODEL_BODY_ARMOR + GAME::CLASS_DARK_LORD)
				{
					pCloth[3].Create(ThreadIndex, o, 18, 0.0f, 10.0f, -5.0f, 5, 5, 50.0f, 90.0f, BITMAP_DARK_LOAD_SKIRT, BITMAP_DARK_LOAD_SKIRT, PCT_MASK_ALPHA | PCT_HEAVY | PCT_STICKED | PCT_SHORT_SHOULDER);
					pCloth[3].AddCollisionSphere(0.0f, -15.0f, -20.0f, 30.0f, 2);
				}
				else if (c->BodyPart[BODYPART_ARMOR].Type == MODEL_BODY_ARMOR + 14)
				{
					pCloth[3].Create(ThreadIndex, o, 18, 0.0f, 10.0f, -5.0f, 5, 5, 50.0f, 90.0f, BITMAP_DARKLOAD_SKIRT_3RD, BITMAP_DARKLOAD_SKIRT_3RD, PCT_MASK_ALPHA | PCT_HEAVY | PCT_STICKED | PCT_SHORT_SHOULDER);
					pCloth[3].AddCollisionSphere(0.0f, -15.0f, -20.0f, 30.0f, 2);
				}

				if (c->Wing.Type == MODEL_WING + 40)
				{
					pCloth[4].Create(ThreadIndex, o, 19, 30.0f, 15.0f, 10.0f, 2, 5, 12.0f, 200.0f, BITMAP_ROBE + 10, BITMAP_ROBE + 10, PCT_FLAT | PCT_SHAPE_NORMAL | PCT_COTTON | PCT_MASK_ALPHA);
					pCloth[4].AddCollisionSphere(0.0f, -15.0f, -20.0f, 30.0f, 2);
					pCloth[4].AddCollisionSphere(0.0f, 0.0f, 0.0f, 35.0f, 17);

					pCloth[5].Create(ThreadIndex, o, 19, -30.0f, 20.0f, 10.0f, 2, 5, 12.0f, 200.0f, BITMAP_ROBE + 10, BITMAP_ROBE + 10, PCT_FLAT | PCT_SHAPE_NORMAL | PCT_COTTON | PCT_MASK_ALPHA);
					pCloth[5].AddCollisionSphere(0.0f, -15.0f, -20.0f, 30.0f, 2);
					pCloth[5].AddCollisionSphere(0.0f, 0.0f, 0.0f, 35.0f, 17);
				}

				o->m_pCloth = pCloth;
				o->m_byNumCloth = numCloth;
			}
			else
			{
				CPhysicsCloth *pCloth = new CPhysicsCloth[1];

				if (c->MonsterIndex == 55)
				{
					pCloth[0].Create(ThreadIndex, o, 19, 0.0f, 10.0f, 0.0f, 10, 10, 55.0f, 140.0f, BITMAP_ROBE + 2, BITMAP_ROBE + 2, PCT_CURVED | PCT_MASK_ALPHA);
					pCloth[0].AddCollisionSphere(-10.0f, -10.0f, -10.0f, 35.0f, 17);
					pCloth[0].AddCollisionSphere(10.0f, -10.0f, -10.0f, 35.0f, 17);
					pCloth[0].AddCollisionSphere(0.0f, -10.0f, -40.0f, 50.0f, 19);
				}
				else if (c->MonsterIndex == 361)
				{
					pCloth[0].Create(ThreadIndex, o, 2, 0.0f, 0.0f, 0.0f, 6, 6, 180.0f, 100.0f, BITMAP_NIGHTMARE_ROBE, BITMAP_NIGHTMARE_ROBE, PCT_CYLINDER | PCT_MASK_ALPHA);
				}
				else if (c->Wing.Type == MODEL_WING + 39)
				{
					pCloth[0].Create(ThreadIndex, o, 19, 0.0f, 15.0f, 0.0f, 10, 10, 120.0f, 120.0f, BITMAP_ROBE + 8, BITMAP_ROBE + 8, PCT_CURVED | PCT_SHORT_SHOULDER | PCT_HEAVY | PCT_MASK_ALPHA);
				}
				else
				{
					pCloth[0].Create(ThreadIndex, o, 19, 0.0f, 10.0f, 0.0f, 10, 10, /*45.0f*/75.0f, 120.0f, BITMAP_ROBE, BITMAP_ROBE + 1, PCT_CURVED | PCT_SHORT_SHOULDER | PCT_HEAVY);
				}
				o->m_pCloth = pCloth;
				o->m_byNumCloth = 1;
			}
		}
		else if (GetBaseClass(c->Class) == GAME::CLASS_DARK_LORD)
		{
			if ((c->BodyPart[BODYPART_ARMOR].Type != MODEL_BODY_ARMOR + GAME::CLASS_DARK_LORD
				&& c->BodyPart[BODYPART_ARMOR].Type != MODEL_BODY_ARMOR + 14
				) && o->m_byNumCloth == 4)
			{
				if (o && o->m_pCloth)
				{
					CPhysicsCloth* pCloth = o->m_pCloth;
					pCloth[3].Destroy();

					o->m_byNumCloth = 3;
				}
			}
			else if (c->BodyPart[BODYPART_ARMOR].Type == MODEL_BODY_ARMOR + GAME::CLASS_DARK_LORD && o->m_byNumCloth == 3)
			{
				if (o && o->m_pCloth)
				{
					CPhysicsCloth* pCloth = o->m_pCloth;

					pCloth[3].Create(ThreadIndex, o, 18, 0.0f, 8.0f, -5.0f, 5, 5, 50.0f, 90.0f, BITMAP_DARK_LOAD_SKIRT, BITMAP_DARK_LOAD_SKIRT, PCT_MASK_ALPHA | PCT_HEAVY | PCT_STICKED | PCT_CURVED | PCT_SHORT_SHOULDER);
					pCloth[3].AddCollisionSphere(0.0f, -15.0f, -20.0f, 30.0f, 2);
					o->m_byNumCloth = 4;
				}
			}
			else if (c->BodyPart[BODYPART_ARMOR].Type == MODEL_BODY_ARMOR + 14 && o->m_byNumCloth == 3)
			{
				if (o && o->m_pCloth)
				{
					CPhysicsCloth* pCloth = o->m_pCloth;

					pCloth[3].Create(ThreadIndex, o, 18, 0.0f, 8.0f, -5.0f, 5, 5, 50.0f, 90.0f, BITMAP_DARKLOAD_SKIRT_3RD, BITMAP_DARKLOAD_SKIRT_3RD, PCT_MASK_ALPHA | PCT_HEAVY | PCT_STICKED | PCT_CURVED | PCT_SHORT_SHOULDER);
					pCloth[3].AddCollisionSphere(0.0f, -15.0f, -20.0f, 30.0f, 2);
					o->m_byNumCloth = 4;
				}
			}
			else if (c->BodyPart[BODYPART_ARMOR].Type != MODEL_BODY_ARMOR + GAME::CLASS_DARK_LORD && o->m_byNumCloth == 6)
			{
				if (o && o->m_pCloth)
				{
					CPhysicsCloth* pCloth = o->m_pCloth;
					if (pCloth[3].GetOwner() != nullptr)
						pCloth[3].Destroy();
				}
			}
			else if (c->BodyPart[BODYPART_ARMOR].Type == MODEL_BODY_ARMOR + GAME::CLASS_DARK_LORD && o->m_byNumCloth == 6)
			{
				if (o && o->m_pCloth)
				{
					CPhysicsCloth* pCloth = o->m_pCloth;
					if (pCloth[3].GetOwner() == nullptr)
					{
						pCloth[3].Create(ThreadIndex, o, 18, 0.0f, 8.0f, -5.0f, 5, 5, 50.0f, 90.0f, BITMAP_DARK_LOAD_SKIRT, BITMAP_DARK_LOAD_SKIRT, PCT_MASK_ALPHA | PCT_HEAVY | PCT_STICKED | PCT_CURVED | PCT_SHORT_SHOULDER);
						pCloth[3].AddCollisionSphere(0.0f, -15.0f, -20.0f, 30.0f, 2);
					}
				}
			}
			else if (c->BodyPart[BODYPART_ARMOR].Type == MODEL_BODY_ARMOR + 14 && o->m_byNumCloth == 6)
			{
				if (o && o->m_pCloth)
				{
					CPhysicsCloth* pCloth = o->m_pCloth;
					if (pCloth[3].GetOwner() == nullptr)
					{
						pCloth[3].Create(ThreadIndex, o, 18, 0.0f, 8.0f, -5.0f, 5, 5, 50.0f, 90.0f, BITMAP_DARKLOAD_SKIRT_3RD, BITMAP_DARKLOAD_SKIRT_3RD, PCT_MASK_ALPHA | PCT_HEAVY | PCT_STICKED | PCT_CURVED | PCT_SHORT_SHOULDER);
						pCloth[3].AddCollisionSphere(0.0f, -15.0f, -20.0f, 30.0f, 2);
					}
				}
			}
		}

		CPhysicsCloth *pCloth = c->Object.m_pCloth;

		if (!g_isCharacterBuff(o, eBuff_Cloaking))
		{
			for (mu_int32 i = 0; i < o->m_byNumCloth; ++i)
			{
				if (GetBaseClass(c->Class) == GAME::CLASS_DARK_LORD)
				{
					if (i == 2 && (
						(c->Wing.Type != MODEL_HELPER + 30
							&& c->Wing.Type != MODEL_WING + 40
							&& c->Wing.Type != MODEL_WING + 130
							) && (CloakLight[0] == 1.0f && CloakLight[1] == 1.0f && CloakLight[2] == 1.0f)))
					{
						continue;
					}

					if ((i >= 0 && i <= 1) && g_ChangeRingMgr.CheckDarkLordHair(c->Object.SubType) == true)
					{
						continue;
					}

					if (i == 3 && g_ChangeRingMgr.CheckDarkLordHair(c->Object.SubType) == true)
					{
						continue;
					}
				}

				if (i == 0 && g_ChangeRingMgr.CheckDarkCloak(c->Class, c->Object.SubType) == true)
				{
					continue;
				}

				if (c->Object.SubType == MODEL_CURSEDTEMPLE_ALLIED_PLAYER || c->Object.SubType == MODEL_CURSEDTEMPLE_ILLUSION_PLAYER)
				{
					continue;
				}

				mu_float Flag = 0.005f;
				if (g_isCharacterBuff(o, eDeBuff_Stun)
					|| g_isCharacterBuff(o, eDeBuff_Sleep))
				{
					Flag = 0.0f;
				}

				if (pCloth[i].GetOwner() == nullptr)
				{
					continue;
				}
				else if (MU_UPDATECOUNTER > 0 && !pCloth[i].Move2(ThreadIndex, Flag, 5))
				{
					DeleteCloth(c, o);
				}
				else
				{
					pCloth[i].Render(ThreadIndex, b, &CloakLight); // FIX check if works well
				}
			}
		}
	}

	mu_float Luminosity = (mu_float)(GetLargeRand(30) + 70)*0.01f;
	if (c->PK >= PVP_MURDERER2)
	{
		Vector(1.0f, 0.1f, 0.1f, c->Light);
	}
	else
	{
		VectorAdd(Light, o->Light, c->Light);

		mu_int32 nCastle = BLOODCASTLE_NUM + (GET_WORLD - WD_11BLOODCASTLE_END);
		if (nCastle > 0 && nCastle <= BLOODCASTLE_NUM)
		{
			if ((c->MonsterIndex >= 86 && c->MonsterIndex <= 89) ||
				(c->MonsterIndex >= 92 && c->MonsterIndex <= 95) ||
				c->MonsterIndex == 99 || c->MonsterIndex == 111 || c->MonsterIndex == 112 || // FIX Changed (c->MonsterIndex == 99 && c->MonsterIndex == 111 && c->MonsterIndex == 112)
				(c->MonsterIndex >= 115 && c->MonsterIndex <= 118) ||
				(c->MonsterIndex >= 121 && c->MonsterIndex <= 124) ||
				(c->MonsterIndex >= 127 && c->MonsterIndex <= 130) ||
				(c->MonsterIndex >= 138 && c->MonsterIndex <= 143))
			{
				mu_int32 level = nCastle / 3;
				if (level)
					Vector(level*0.5f, 0.1f, 0.1f, c->Light);
			}
		}
	}

	if (!InChaosCastle() &&
		!(g_CursedTemple.IsCursedTemple() && !c->SafeZone)
		)
	{
		NextGradeObjectRender(ThreadIndex, c);
	}

	mu_boolean Bind = false;
	Bind = RenderCharacterBackItem(ThreadIndex, c, o, Translate);

	if (!Bind)
	{
		if (!g_isCharacterBuff(o, eBuff_Cloaking))
		{
			if (g_isCharacterBuff(o, eBuff_AddCriticalDamage) && o->Kind == KIND_PLAYER && o->Type == MODEL_PLAYER && (MoveSceneFrame % 30) == 0)
			{
				if(MU_UPDATECOUNTER > 0)
				{
					mu_boolean    renderSkillWave = (GetLargeRand(20)) ? true : false;
					mu_int16   weaponType = -1;
					Vector(0.0f, 0.0f, 0.0f, p);
					Vector(1.0f, 0.6f, 0.3f, Light);
					if (c->Weapon[0].Type != -1 &&
						c->Weapon[0].Type != MODEL_BOW + 15)
					{
						b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[0].LinkBone], p, Position, true);
						if (c->Weapon[0].Type >= MODEL_BOW && c->Weapon[0].Type < MODEL_BOW + MAX_ITEM_INDEX)
						{
							weaponType = 1;
						}
						CreateEffect(ThreadIndex, BITMAP_FLARE_FORCE, Position, o->Angle, o->Light, 1, o, weaponType, b->_ReverseBonesIndex[c->Weapon[0].LinkBone]);
						if (renderSkillWave == false)
						{
							CreateEffect(ThreadIndex, MODEL_DARKLORD_SKILL, Position, o->Angle, Light, 0);
						}
					}
					if (c->Weapon[1].Type != -1 &&
						c->Weapon[1].Type != MODEL_HELPER + 5 &&
						c->Weapon[1].Type != MODEL_BOW + 7 &&
						(c->Weapon[1].Type < MODEL_SHIELD || c->Weapon[1].Type >= MODEL_SHIELD + MAX_ITEM_INDEX))
					{
						b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[1].LinkBone], p, Position, true);
						if (c->Weapon[1].Type >= MODEL_BOW && c->Weapon[1].Type < MODEL_BOW + MAX_ITEM_INDEX)
						{
							weaponType = 1;
						}
						CreateEffect(ThreadIndex, BITMAP_FLARE_FORCE, Position, o->Angle, o->Light, 1, o, weaponType, b->_ReverseBonesIndex[c->Weapon[1].LinkBone]);
						if (renderSkillWave == false)
						{
							CreateEffect(ThreadIndex, MODEL_DARKLORD_SKILL, Position, o->Angle, Light, 1);
						}
					}

					PlayBackground(SOUND_CRITICAL, o);
				}
			}
		}

		for (mu_int32 i = 0; i < 2; ++i)
		{
			if (i == 0)
			{
				if (o->CurrentAction == PLAYER_ATTACK_SKILL_FURY_STRIKE && o->AnimationFrame <= 4.0f)
				{
					continue;
				}

				switch (c->Skill)
				{
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
				case AT_SKILL_WHEEL:
					{
						if (o->CurrentAction == PLAYER_ATTACK_SKILL_WHEEL &&
							i == 0)
						{
							continue;
						}
					}
					break;
				}
			}

			PART_t *w = &c->Weapon[i];
			if (
				w->Type != -1 && w->Type != MODEL_BOW + 7 && w->Type != MODEL_BOW + 15
				&& w->Type != MODEL_HELPER + 5
				)
			{
				if (o->CurrentAction == PLAYER_ATTACK_BOW || o->CurrentAction == PLAYER_ATTACK_CROSSBOW ||
					o->CurrentAction == PLAYER_ATTACK_FLY_BOW || o->CurrentAction == PLAYER_ATTACK_FLY_CROSSBOW)
				{
					if (w->Type == MODEL_BOW + 23)
					{
						w->CurrentAction = 1;
					}
					else
					{
						w->CurrentAction = 0;
					}
					w->PlaySpeed = GetPlaySpeed(o, MODEL_PLAYER, PLAYER_ATTACK_BOW);
				}
				else if (w->Type == MODEL_MACE + 2)
				{
					if (o->CurrentAction >= PLAYER_ATTACK_SWORD_RIGHT1 && o->CurrentAction <= PLAYER_ATTACK_SWORD_RIGHT2)
					{
						w->CurrentAction = 2;
						w->PlaySpeed = GetPlaySpeed(o, MODEL_PLAYER, PLAYER_ATTACK_SWORD_RIGHT1);
					}
					else
					{
						w->CurrentAction = 1;
						w->PlaySpeed = GetPlaySpeed(o, MODEL_PLAYER, PLAYER_STOP_MALE);
					}
				}
				else if (w->Type == MODEL_MACE + 5)
				{
					w->CurrentAction = 0;
					w->PlaySpeed = GetPlaySpeed(o, MODEL_PLAYER, PLAYER_STOP_MALE) * 2.0f;
				}
				else if (w->Type == MODEL_STAFF + 6)
				{
					w->CurrentAction = 0;
					w->PlaySpeed = GetPlaySpeed(o, MODEL_PLAYER, PLAYER_STOP_MALE) * 15.0f;
				}
				else if (w->Type >= MODEL_SWORD && w->Type < MODEL_SWORD + MAX_ITEM_INDEX)
				{
					w->CurrentAction = 0;
					w->PlaySpeed = GetPlaySpeed(o, MODEL_PLAYER, PLAYER_STOP_MALE);
				}
				else if (w->Type == MODEL_BOW + 23)
				{
					w->CurrentAction = 0;
					w->PlaySpeed = GetPlaySpeed(o, MODEL_PLAYER, PLAYER_STOP_MALE);
				}
				else if ((w->Type == MODEL_MACE + 16)
					|| (w->Type == MODEL_MACE + 17))
				{
					w->CurrentAction = 0;
					w->PlaySpeed = GetPlaySpeed(o, MODEL_PLAYER, PLAYER_STOP_MALE);
				}
				else if (w->Type == MODEL_STAFF + 34)
				{
					w->CurrentAction = 0;
					w->PlaySpeed = GetPlaySpeed(o, MODEL_PLAYER, PLAYER_STOP_MALE);
				}
				else
				{
					w->CurrentAction = 0;
					w->PlaySpeed = 0.0f;
					w->AnimationFrame = 0.0f;
					w->PriorAnimationFrame = 0.0f;
				}
				
				RenderLinkObject(ThreadIndex, 0.0f, 0.0f, 0.0f, c, w, w->Type, w->Level, w->ExcOption, false, Translate);

				if (w->Level >= 7)
				{
					Vector(Luminosity*0.5f, Luminosity*0.4f, Luminosity*0.3f, Light);
				}
				else if (w->Level >= 5)
				{
					Vector(Luminosity*0.3f, Luminosity*0.3f, Luminosity*0.5f, Light);
				}
				else if (w->Level >= 3)
				{
					Vector(Luminosity*0.5f, Luminosity*0.3f, Luminosity*0.3f, Light);
				}
				else
				{
					Vector(Luminosity*0.3f, Luminosity*0.3f, Luminosity*0.3f, Light);
				}
				mu_float Scale;
				if (c->PK < PVP_MURDERER2 &&
					c->Level != 4 &&
					MU_UPDATECOUNTER > 0)
				{
					mu_boolean Success = true;

					switch (w->Type)
					{
					case MODEL_SWORD + 4:
					case MODEL_SWORD + 5:
					case MODEL_SWORD + 13:
						Vector(0.0f, -110.0f, 5.0f, Position);
						break;
					case MODEL_SWORD + 8:
					case MODEL_SWORD + 9:
						Vector(0.0f, -110.0f, -5.0f, Position);
						break;
					case MODEL_SWORD + 7:
					case MODEL_SWORD + 10:
						Vector(0.0f, -110.0f, 0.0f, Position);
						break;
					case MODEL_SWORD + 14:
					case MODEL_SWORD + 11:
						Vector(0.0f, -150.0f, 0.0f, Position);
						break;
					case MODEL_SWORD + 12:
						Vector(Luminosity, Luminosity, Luminosity, Light);
						Vector(0.0f, -160.0f, 0.0f, Position);
						break;
					case MODEL_SWORD + 17:
						Success = false;
						Scale = EMath::Sin(FWorldTime*0.004f)*10.0f + 20.0f;
						{
							EVector3  pos1, pos2;

							Vector(0.0f, -20.0f, -40.0f, Position);
							MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, pos1, true);
							Vector(0.0f, -160.0f, -10.0f, Position);
							MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, pos2, true);
							CreateJoint(ThreadIndex, BITMAP_FLARE + 1, pos1, pos2, o->Angle, 4, o, Scale);

							Vector(0.0f, -10.0f, 28.0f, Position);
							MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, pos1, true);
							Vector(0.0f, -145.0f, 18.0f, Position);
							MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, pos2, true);
							CreateJoint(ThreadIndex, BITMAP_FLARE + 1, pos1, pos2, o->Angle, 4, o, Scale);
						}
						break;

					case MODEL_SWORD + 18:
						{
							Success = false;

							Vector(0.0f, -20.0f, 15.0f, Position);
							MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, p, true);

							Scale = EMath::Sin(FWorldTime*0.004f)*0.3f + 0.3f;
							Vector(Scale*0.2f, Scale*0.2f, Scale*1.0f, Light);
							CreateSprite(BITMAP_SHINY + 1, p, Scale + 1.0f, Light, o, 0);

							Vector(0.0f, -133.0f, 7.0f, Position);
							MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, Position, true);

							Scale = (Scale*20.0f) + 20.0f;
							CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, p, Position, o->Angle, 10, nullptr, Scale);
							CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, p, Position, o->Angle, 10, nullptr, Scale);
							CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, p, Position, o->Angle, 10, nullptr, Scale);
						}
						break;
					case MODEL_STAFF + 9:
						Success = false;
						Vector(0.0f, -120.0f, 5.0f, Position);
						Vector(Luminosity*0.6f, Luminosity*0.6f, Luminosity*2.0f, Light);
						MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, p, true);
						CreateSprite(BITMAP_SHINY + 1, p, 1.5f, Light, o);
						CreateSprite(BITMAP_LIGHT, p, Luminosity + 1.0f, Light, o);

						Vector(0.0f, 100.0f, 10.0f, Position);
						MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, p, true);
						CreateSprite(BITMAP_LIGHT, p, Luminosity + 1.0f, Light, o);
						break;
					case MODEL_STAFF + 4:
						Success = false;
						Vector(0.0f, -90.0f, 0.0f, Position);
						Vector(Luminosity*0.4f, Luminosity*0.8f, Luminosity*0.6f, Light);
						MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, p, true);
						CreateSprite(BITMAP_SHINY + 1, p, 2.0f, Light, o);
						break;
					case MODEL_SHIELD + 14:
						Success = false;
						Vector(20.0f, 0.0f, 0.0f, Position);
						Vector(Luminosity*0.4f, Luminosity*0.6f, Luminosity*1.5f, Light);
						MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, p, true);
						CreateSprite(BITMAP_SHINY + 1, p, 1.5f, Light, o);
						break;
					case MODEL_STAFF + 5:
						Success = false;
						Vector(0.0f, -145.0f, 0.0f, Position);
						Vector(Luminosity*0.4f, Luminosity*0.6f, Luminosity*1.0f, Light);
						MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, p, true);
						CreateSprite(BITMAP_SHINY + 1, p, 1.5f, Light, o);
						CreateSprite(BITMAP_LIGHTNING + 1, p, 0.3f, Light, o);
						break;
					case MODEL_STAFF + 7:
						Success = false;
						Vector(Luminosity*0.4f, Luminosity*0.6f, Luminosity*1.0f, Light);
						RenderBrightEffect(ThreadIndex, b, BITMAP_SHINY + 1, 27, 2.0f, Light, o);

						for (mu_int32 j = 28; j <= 37; ++j)
						{
							RenderBrightEffect(ThreadIndex, b, BITMAP_LIGHT, j, 1.5f, Light, o);
						}
						break;
					case MODEL_STAFF + 6:
						Success = false;
						Vector(0.0f, -145.0f, 0.0f, Position);
						MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, p, true);
						Vector(Luminosity*1.0f, Luminosity*0.6f, Luminosity*0.4f, Light);
						CreateSprite(BITMAP_SPARK, p, 3.0f, Light, o);
						CreateSprite(BITMAP_SHINY + 2, p, 1.5f, Light, o);

						for (mu_int32 j = 0; j < 4; ++j)
						{
							Vector((mu_float)(GetLargeRand(20) - 10), (mu_float)(GetLargeRand(20) - 10 - 90.0f), (mu_float)(GetLargeRand(20) - 10), Position);
							MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, p, true);
							CreateParticle(ThreadIndex, BITMAP_SPARK, p, o->Angle, Light, 1);
						}
						Vector(Luminosity*1.0f, Luminosity*0.2f, Luminosity*0.1f, Light);

						for (mu_int32 j = 0; j < 10; ++j)
						{
							if (GetLargeRand(4) < 3)
							{
								Vector(0.0f, -j * 20 + 60.0f, 0.0f, Position);
								MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, p, true);
								CreateSprite(BITMAP_LIGHT, p, 1.0f, Light, o);
							}
						}
						break;
					case MODEL_STAFF + 15:
						Success = false;
						Vector(Luminosity*0.2f, Luminosity*0.3f, Luminosity*1.4f, Light);
						Vector(0.0f, 0.0f, 0.0f, Position);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(1)], Position, p, true);
						CreateSprite(BITMAP_SHINY + 1, p, 1.0f, Light, o, -IWorldTime*0.1f);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(2)], Position, p, true);
						CreateSprite(BITMAP_SHINY + 1, p, 0.5f, Light, o, IWorldTime*0.1f);
						break;
					case MODEL_STAFF + 16:
						Success = false;
						Vector(Luminosity*1.0f, Luminosity*0.3f, Luminosity*0.4f, Light);
						Vector(0.0f, 0.0f, 0.0f, Position);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(1)], Position, p, true);
						CreateSprite(BITMAP_SHINY + 1, p, 1.0f, Light, o, -IWorldTime*0.1f);
						CreateSprite(BITMAP_SHINY + 1, p, 1.0f, Light, o, -IWorldTime*0.13f);
						break;
					case MODEL_STAFF + 17:
						Success = false;
						Scale = EMath::Abs(EMath::Sin(FWorldTime*0.002f))*0.2f;
						Luminosity = EMath::Abs(EMath::Sin(FWorldTime*0.002f))*0.4f;
						Vector(0.5f + Luminosity, 0.2f + Luminosity, 0.9f + Luminosity, Light);

						for (mu_int32 j = 1; j <= 4; ++j)
							RenderBrightEffect(ThreadIndex, b, BITMAP_LIGHT, j, Scale + 1.0f, Light, o);
						break;
					case MODEL_STAFF + 18:
						Success = false;
						Scale = EMath::Abs(EMath::Sin(FWorldTime*0.002f))*0.2f;
						Luminosity = EMath::Abs(EMath::Sin(FWorldTime*0.002f))*0.4f;
						Vector(0.5f + Luminosity, 0.2f + Luminosity, 0.9f + Luminosity, Light);

						for (mu_int32 j = 1; j <= 2; ++j)
						{
							RenderBrightEffect(ThreadIndex, b, BITMAP_SHINY + 1, j, Scale + 1.0f, Light, o);
							RenderBrightEffect(ThreadIndex, b, BITMAP_LIGHT, j, Scale + 1.0f, Light, o);
						}
						Vector(0.8f + Luminosity, 0.6f + Luminosity, 0.3f + Luminosity, Light);
						RenderBrightEffect(ThreadIndex, b, BITMAP_LIGHT, 3, Scale + 1.0f, Light, o);
						break;
					case MODEL_STAFF + 19:
						Success = false;
						Scale = EMath::Abs(EMath::Sin(FWorldTime*0.002f))*0.2f;
						Luminosity = EMath::Abs(EMath::Sin(FWorldTime*0.002f))*0.4f;
						Vector(0.5f + Luminosity, 0.2f + Luminosity, 0.9f + Luminosity, Light);

						for (mu_int32 j = 2; j <= 3; ++j)
							RenderBrightEffect(ThreadIndex, b, BITMAP_LIGHT, j, Scale + 1.0f, Light, o);
						Vector(0.8f + Luminosity, 0.6f + Luminosity, 0.3f + Luminosity, Light);
						RenderBrightEffect(ThreadIndex, b, BITMAP_SHINY + 2, 2, Scale + 1.0f, Light, o);
						break;
					case MODEL_STAFF + 20:
						Success = false;
						Vector(1.0f, 0.2f, 0.1f, Light);
						Vector(0.0f, 0.0f, 0.0f, Position);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(2)], Position, p, true);
						CreateSprite(BITMAP_EVENT_CLOUD, p, 0.25f, Light, o, -IWorldTime*0.1f);
						CreateSprite(BITMAP_EVENT_CLOUD, p, 0.25f, Light, o, -IWorldTime*0.2f);
						Vector(1.0f, 0.4f, 0.3f, Light);
						RenderBrightEffect(ThreadIndex, b, BITMAP_SHINY + 1, 2, 1.0f, Light, o);
						Vector(1.0f, 0.2f, 0.0f, Light);
						CreateParticle(ThreadIndex, BITMAP_SPARK + 1, p, o->Angle, Light, 16, 1.0f);
						CreateParticle(ThreadIndex, BITMAP_SPARK + 1, p, o->Angle, Light, 23, 1.0f);
						break;
					case MODEL_BOW + 16:
						Success = false;
						Vector(Luminosity*0.4f, Luminosity*0.6f, Luminosity*1.0f, Light);

						for (mu_int32 j = 0; j < 6; ++j)
						{
							Vector(0.0f, -10.0f, -j*20.0f, Position);
							MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, p, true);
							CreateSprite(BITMAP_LIGHT, p, 2.0f, Light, o);
						}
						break;
					case MODEL_MACE + 5:
						Success = false;
						Vector(Luminosity*1.0f, Luminosity*0.6f, Luminosity*0.4f, Light);

						for (mu_int32 j = 0; j < 8; ++j)
						{
							if (GetLargeRand(4) < 3)
							{
								Vector(0.0f, -j * 20 - 30.0f, 0.0f, Position);
								MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, p, true);
								CreateSprite(BITMAP_LIGHT, p, 1.0f, Light, o);
							}
						}
						break;
					case MODEL_MACE + 6:
						Success = false;
						Vector(0.0f, -84.0f, 0.0f, Position);
						MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, p, true);
						Scale = EMath::Sin(FWorldTime*0.004f)*0.3f + 0.7f;
						Vector(Scale*1.0f, Scale*0.2f, Scale*0.1f, Light);
						CreateSprite(BITMAP_SHINY + 1, p, Scale + 1.5f, Light, o);

						for (mu_int32 j = 0; j < 5; ++j)
						{
							Vector(0.0f, -j*20.0f - 10.0f, 0.0f, Position);
							MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, p, true);
							CreateSprite(BITMAP_SHINY + 1, p, 1.0f, Light, o);
						}
						Vector(Scale*0.5f, Scale*0.1f, Scale*0.05f, Light);
						RenderBrightEffect(ThreadIndex, b, BITMAP_SHINY + 1, 2, 1.0f, Light, o);
						RenderBrightEffect(ThreadIndex, b, BITMAP_SHINY + 1, 6, 1.0f, Light, o);
						break;
					case MODEL_BOW + 13:
					case MODEL_BOW + 14:
						Success = false;
						if (w->Type == MODEL_BOW + 13)
						{
							Vector(Luminosity*0.2f, Luminosity*0.4f, Luminosity*0.6f, Light);
						}
						else
						{
							Vector(Luminosity*0.6f, Luminosity*0.4f, Luminosity*0.2f, Light);
						}

						for (mu_int32 j = 0; j < 6; ++j)
						{
							Vector(0.0f, -20.0f, (mu_float)(-j * 20), Position);
							MODELS::Data[o->Type].TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, p, true);
							CreateSprite(BITMAP_LIGHT, p, 2.0f, Light, o);
						}
						break;
					default:
						Success = false;
					}
					if (Success)
					{
						MUModel *eb = &MODELS::Data[o->Type];
						if(eb->IsDataLoaded() == false)
						{
							eb->RequestLoad();
						}
						else
						{
							eb->TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], Position, p, true);
							CreateSprite(BITMAP_LIGHT, p, 1.4f, Light, o);
						}
					}
				}
			}
		}
	}

	if (MU_UPDATECOUNTER > 0)
	{
		switch (o->Type)
		{
		case MODEL_PLAYER:
			Vector(0.0f, 0.0f, 0.0f, p);

			if (GetCharacterClass(c->Class) == GAME::CLASS_SOULMASTER)
			{
				if (!g_isCharacterBuff(o, eBuff_Cloaking))
				{
					Vector(-4.0f, 11.0f, 0.0f, p);
					Vector(1.0f, 1.0f, 1.0f, Light);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(19)], p, Position, true);
					CreateSprite(BITMAP_SPARK + 1, Position, 0.6f, Light, nullptr);

					mu_float scale = EMath::Sin(FWorldTime*0.001f)*0.4f;
					CreateSprite(BITMAP_SHINY + 1, Position, scale, Light, nullptr);
				}
			}
			if (o->CurrentAction == PLAYER_SKILL_FLASH || o->CurrentAction == PLAYER_ATTACK_RIDE_ATTACK_FLASH
				|| o->CurrentAction == PLAYER_FENRIR_ATTACK_DARKLORD_FLASH
				)
			{
				if (GetBaseClass(c->Class) == GAME::CLASS_DARK_LORD || o->CurrentAction == PLAYER_ATTACK_RIDE_ATTACK_FLASH
					|| o->CurrentAction == PLAYER_FENRIR_ATTACK_DARKLORD_FLASH
					)
				{
					if (o->AnimationFrame >= 1.2f &&
						o->AnimationFrame < 1.6f)
					{
						EVector3 Angle;
						Vector(1.0f, 0.0f, 0.0f, Angle);
						CreateEffect(ThreadIndex, BITMAP_GATHERING, o->Position, o->Angle, o->Light, 2, o);

						PlayBackground(SOUND_ELEC_STRIKE_READY);
					}

					if (o->AnimationFrame < 2.0f)
					{
						if (PartyNumber > 0)
						{
							if (PartyManager::IsPartyMemberChar(c) == false)
								break;

							for (mu_int32 i = 0; i < PartyNumber; ++i)
							{
								PARTY_t* p = &Party[i];
								if (p->index < 0) continue;

								CHARACTER* tc = &CharactersClient[p->index];
								if (tc != c)
								{
									VectorCopy(tc->Object.Position, Position);
									Position[2] += 150.0f;
									CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, Position, Position, o->Angle, 12, o, 20.0f);
									CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, Position, Position, o->Angle, 13, o, 20.0f);
								}
							}
						}
					}

					if (o->AnimationFrame >= 7 &&
						o->AnimationFrame < 8)
					{
						EVector3 Angle, Light;

						b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[0].LinkBone], p, Position, true);

						Vector(0.8f, 0.5f, 1.0f, Light);
						Vector(180.0f, 45.0f, 0.0f, Angle);
						CreateEffect(ThreadIndex, MODEL_DARKLORD_SKILL, Position, Angle, Light, 2);

						Vector(0.0f, 0.0f, o->Angle[2], Angle);
						CreateEffect(ThreadIndex, MODEL_DARKLORD_SKILL, Position, Angle, Light, 2);
					}
				}
				else
				{
					if (g_isCharacterBuff(o, eBuff_Cloaking))
					{
						b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[0].LinkBone], p, Position, true);
						Vector(0.1f, 0.1f, 1.0f, Light);
						mu_float Scale = o->AnimationFrame*0.1f;
						CreateSprite(BITMAP_LIGHTNING + 1, Position, Scale*0.3f, Light, o);
						CreateSprite(BITMAP_LIGHTNING + 1, Position, Scale*1.0f, o->Light, o, -IWorldTime*0.1f);
						CreateSprite(BITMAP_LIGHTNING + 1, Position, Scale*2.5f, o->Light, o, IWorldTime*0.1f);
					}
				}
			}
			else if (o->CurrentAction == PLAYER_SKILL_LIGHTNING_ORB
				|| o->CurrentAction == PLAYER_SKILL_LIGHTNING_ORB_UNI
				|| o->CurrentAction == PLAYER_SKILL_LIGHTNING_ORB_DINO
				|| o->CurrentAction == PLAYER_SKILL_LIGHTNING_ORB_FENRIR
				)
			{
				EVector3 vLight, vRelativePos, vWorldPos;
				Vector(1.0f, 1.0f, 1.0f, vLight);
				Vector(0.0f, 0.0f, 0.0f, vRelativePos);
				Vector(0.0f, 0.0f, 0.0f, vWorldPos);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(27)], vRelativePos, vWorldPos, true);

				Vector(0.2f, 0.2f, 1.0f, vLight);
				CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, vWorldPos, o->Angle, vLight, 2, o);
				CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, vWorldPos, o->Angle, vLight, 2, o);
			}
			else if (o->CurrentAction >= PLAYER_SKILL_SLEEP
				&& o->CurrentAction <= PLAYER_SKILL_SLEEP_FENRIR)
			{
				mu_int32 iSkillType = CharacterAttribute->Skill[Hero->CurrentSkill];

				EVector3 vLight, vRelativePos, vWorldPos;
				Vector(1.0f, 1.0f, 1.0f, vLight);
				Vector(0.0f, 0.0f, 0.0f, vRelativePos);
				Vector(0.0f, 0.0f, 0.0f, vWorldPos);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(37)], vRelativePos, vWorldPos, true);

				mu_float fRot = (FWorldTime*0.0006f) * 360.0f;

				if (iSkillType == AT_SKILL_ALICE_SLEEP
					|| (AT_SKILL_ALICE_SLEEP_UP <= iSkillType && iSkillType <= AT_SKILL_ALICE_SLEEP_UP + 4)
					)
				{
					Vector(0.5f, 0.2f, 0.8f, vLight);
				}
				else if (iSkillType == AT_SKILL_ALICE_BLIND)
				{
					Vector(1.0f, 1.0f, 1.0f, vLight);
				}
				else if (iSkillType == AT_SKILL_ALICE_THORNS)
				{
					Vector(0.8f, 0.5f, 0.2f, vLight);
				}
				else if (iSkillType == AT_SKILL_ALICE_BERSERKER)
				{
					Vector(1.0f, 0.1f, 0.2f, vLight);
				}
				else if (iSkillType == AT_SKILL_ALICE_WEAKNESS)
				{
					Vector(0.8f, 0.1f, 0.1f, vLight);
				}
				else if (iSkillType == AT_SKILL_ALICE_ENERVATION)
				{
					Vector(0.25f, 1.0f, 0.7f, Light);
				}

				if (iSkillType == AT_SKILL_ALICE_SLEEP || iSkillType == AT_SKILL_ALICE_THORNS
					|| (AT_SKILL_ALICE_SLEEP_UP <= iSkillType && iSkillType <= AT_SKILL_ALICE_SLEEP_UP + 4)
					|| iSkillType == AT_SKILL_ALICE_BERSERKER
					|| iSkillType == AT_SKILL_ALICE_WEAKNESS || iSkillType == AT_SKILL_ALICE_ENERVATION
					)
				{
					CreateSprite(BITMAP_SHINY + 5, vWorldPos, 1.0f, vLight, o, fRot);
					CreateSprite(BITMAP_SHINY + 5, vWorldPos, 0.7f, vLight, o, -fRot);
				}
				else if (iSkillType == AT_SKILL_ALICE_BLIND)
				{
					CreateSprite(BITMAP_SHINY + 5, vWorldPos, 1.0f, vLight, o, fRot, 1);
					CreateSprite(BITMAP_SHINY + 5, vWorldPos, 0.7f, vLight, o, -fRot, 1);
				}

				if (iSkillType == AT_SKILL_ALICE_SLEEP
					|| (AT_SKILL_ALICE_SLEEP_UP <= iSkillType && iSkillType <= AT_SKILL_ALICE_SLEEP_UP + 4)
					)
				{
					Vector(0.7f, 0.0f, 0.8f, vLight);
				}
				else if (iSkillType == AT_SKILL_ALICE_BLIND)
				{
					Vector(1.0f, 1.0f, 1.0f, vLight);
				}
				else if (iSkillType == AT_SKILL_ALICE_THORNS)
				{
					Vector(0.8f, 0.5f, 0.2f, vLight);
				}
				else if (iSkillType == AT_SKILL_ALICE_BERSERKER)
				{
					Vector(1.0f, 0.1f, 0.2f, vLight);
				}
				else if (iSkillType == AT_SKILL_ALICE_THORNS)
				{
					Vector(0.8f, 0.1f, 0.1f, vLight);
				}
				else if (iSkillType == AT_SKILL_ALICE_ENERVATION)
				{
					Vector(0.25f, 1.0f, 0.7f, vLight);
				}

				if (iSkillType == AT_SKILL_ALICE_SLEEP || iSkillType == AT_SKILL_ALICE_THORNS
					|| (AT_SKILL_ALICE_SLEEP_UP <= iSkillType && iSkillType <= AT_SKILL_ALICE_SLEEP_UP + 4)
					|| iSkillType == AT_SKILL_ALICE_BERSERKER
					|| iSkillType == AT_SKILL_ALICE_WEAKNESS || iSkillType == AT_SKILL_ALICE_ENERVATION
					)
				{
					CreateSprite(BITMAP_PIN_LIGHT, vWorldPos, 1.7f, vLight, o, (mu_float)(GetLargeRand(360)));
					CreateSprite(BITMAP_PIN_LIGHT, vWorldPos, 1.5f, vLight, o, (mu_float)(GetLargeRand(360)));
				}
				else if (iSkillType == AT_SKILL_ALICE_BLIND)
				{
					CreateSprite(BITMAP_PIN_LIGHT, vWorldPos, 1.7f, vLight, o, (mu_float)(GetLargeRand(360)), 1);
					CreateSprite(BITMAP_PIN_LIGHT, vWorldPos, 1.5f, vLight, o, (mu_float)(GetLargeRand(360)), 1);
				}

				if (iSkillType == AT_SKILL_ALICE_SLEEP
					|| (AT_SKILL_ALICE_SLEEP_UP <= iSkillType && iSkillType <= AT_SKILL_ALICE_SLEEP_UP + 4)
					)
				{
					Vector(0.6f, 0.1f, 0.8f, vLight);
				}
				else if (iSkillType == AT_SKILL_ALICE_BLIND)
				{
					Vector(1.0f, 1.0f, 1.0f, vLight);
				}
				else if (iSkillType == AT_SKILL_ALICE_THORNS)
				{
					Vector(0.8f, 0.5f, 0.2f, vLight);
				}
				else if (iSkillType == AT_SKILL_ALICE_BERSERKER)
				{
					Vector(1.0f, 0.1f, 0.2f, vLight);
				}
				else if (iSkillType == AT_SKILL_ALICE_THORNS)
				{
					Vector(0.8f, 0.1f, 0.1f, vLight);
				}
				else if (iSkillType == AT_SKILL_ALICE_ENERVATION)
				{
					Vector(0.25f, 1.0f, 0.7f, vLight);
				}

				if (iSkillType == AT_SKILL_ALICE_SLEEP || iSkillType == AT_SKILL_ALICE_THORNS
					|| (AT_SKILL_ALICE_SLEEP_UP <= iSkillType && iSkillType <= AT_SKILL_ALICE_SLEEP_UP + 4)
					|| iSkillType == AT_SKILL_ALICE_BERSERKER
					|| iSkillType == AT_SKILL_ALICE_WEAKNESS || iSkillType == AT_SKILL_ALICE_ENERVATION
					)
				{
					CreateParticle(ThreadIndex, BITMAP_LIGHT + 2, vWorldPos, o->Angle, vLight, 0, 1.0f);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, vWorldPos, o->Angle, vLight, 3, 0.5f);
				}
				else if (iSkillType == AT_SKILL_ALICE_BLIND)
				{
					CreateParticle(ThreadIndex, BITMAP_LIGHT + 2, vWorldPos, o->Angle, vLight, 4, 1.0f);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, vWorldPos, o->Angle, vLight, 5, 0.5f);
				}
			}
			else if (o->CurrentAction == PLAYER_SKILL_CHAIN_LIGHTNING)
			{
				EVector3 vRelativePos, vWorldPos, vLight;
				Vector(0.0f, 0.0f, 0.0f, vRelativePos);
				Vector(0.4f, 0.4f, 0.8f, vLight);

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(37)], vRelativePos, vWorldPos, true);
				CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, vWorldPos, o->Angle, vLight, 2, o);
				CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, vWorldPos, o->Angle, vLight, 2, o);
				CreateSprite(BITMAP_LIGHT, vWorldPos, 1.5f, vLight, o, 0.0f);

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(28)], vRelativePos, vWorldPos, true);
				CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, vWorldPos, o->Angle, vLight, 2, o);
				CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, vWorldPos, o->Angle, vLight, 2, o);
				CreateSprite(BITMAP_LIGHT, vWorldPos, 1.5f, vLight, o, 0.0f);
			}

			if (g_isCharacterBuff(o, eBuff_Attack) || g_isCharacterBuff(o, eBuff_HelpNpc))
			{
				if (!g_isCharacterBuff(o, eBuff_Cloaking))
				{
					for (mu_int32 i = 0; i < 2; ++i)
					{
						Luminosity = (mu_float)(GetLargeRand(30) + 70)*0.01f;
						Vector(Luminosity*1.0f, Luminosity*0.3f, Luminosity*0.2f, Light);
						b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[i].LinkBone], p, Position, true);
						CreateSprite(BITMAP_SHINY + 1, Position, 1.5f, Light, o);
						b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[i].LinkBone - 6], p, Position, true);
						CreateSprite(BITMAP_SHINY + 1, Position, 1.5f, Light, o);
						b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[i].LinkBone - 7], p, Position, true);
						CreateSprite(BITMAP_SHINY + 1, Position, 1.5f, Light, o);
					}
					if (!SearchJoint(MODEL_SPEARSKILL, o, 4)
						&& !SearchJoint(MODEL_SPEARSKILL, o, 9))
					{
						for (mu_int32 i = 0; i < 5; ++i)
						{
							CreateJoint(ThreadIndex, MODEL_SPEARSKILL, o->Position, o->Position, o->Angle, 4, o, 20.0f, -1, 0, 0, c->TargetCharacter);
						}
					}
				}
			}
			else if (g_isCharacterBuff(o, eBuff_Defense))
			{
				if (!g_isCharacterBuff(o, eBuff_Cloaking))
				{
					if (SearchJoint(MODEL_SPEARSKILL, o, 4) == false && SearchJoint(MODEL_SPEARSKILL, o, 9) == false)
					{
						for (mu_int32 i = 0; i < 5; ++i)
						{
							CreateJoint(ThreadIndex, MODEL_SPEARSKILL, o->Position, o->Position, o->Angle, 4, o, 20.0f, -1, 0, 0, c->TargetCharacter);
						}
					}
				}
			}

			if (g_isCharacterBuff((&c->Object), eBuff_PcRoomSeal1)
				|| g_isCharacterBuff((&c->Object), eBuff_PcRoomSeal2)
				|| g_isCharacterBuff((&c->Object), eBuff_PcRoomSeal3)
				|| g_isCharacterBuff((&c->Object), eBuff_Seal1)
				|| g_isCharacterBuff((&c->Object), eBuff_Seal2)
				|| g_isCharacterBuff((&c->Object), eBuff_Seal3)
				|| g_isCharacterBuff((&c->Object), eBuff_Seal4))
			{
				if (SearchJoint(MODEL_SPEARSKILL, o, 10) == false
					&& SearchJoint(MODEL_SPEARSKILL, o, 11) == false)
				{
					for (mu_int32 i = 0; i < 3; ++i)
					{
						CreateJoint(ThreadIndex, MODEL_SPEARSKILL, o->Position, o->Position, o->Angle, 10, o, 12.0f, -1, 0, 0, c->Key);
					}
				}
			}
			else
			{
				DeleteJoint(MODEL_SPEARSKILL, o, 10);
				DeleteJoint(MODEL_SPEARSKILL, o, 11);
			}

			if (g_isCharacterBuff((&c->Object), eBuff_Thorns))
			{
				if (SearchJoint(BITMAP_FLARE, o, 43) == false)
				{
					EVector3 vLight;
					Vector(0.9f, 0.6f, 0.1f, vLight);
					CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 43, o, 50.0f, 0, 0, 0, 0, &vLight);
				}
			}
			else
			{
				DeleteJoint(BITMAP_FLARE, o, 43);
			}

			if (g_isCharacterBuff((&c->Object), eBuff_Berserker))
			{
				if (!SearchEffect(BITMAP_ORORA, o, 0))
				{
					EVector3 vLight[2];
					Vector(0.9f, 0.0f, 0.1f, vLight[0]);
					Vector(1.0f, 1.0f, 1.0f, vLight[1]);
					for (mu_int32 i = 0; i < 4; ++i)
					{
						CreateEffect(ThreadIndex, BITMAP_ORORA, o->Position, o->Angle, vLight[0], i, o);
						if (i == 2 || i == 3)
							CreateEffect(ThreadIndex, BITMAP_SPARK + 2, o->Position, o->Angle, vLight[1], i, o);
					}
					CreateEffect(ThreadIndex, BITMAP_LIGHT_MARKS, o->Position, o->Angle, vLight[0], 0, o);
				}
			}
			else
			{
				for (mu_int32 i = 0; i < 4; ++i)
				{
					DeleteEffect(BITMAP_ORORA, o, i);
					if (i == 2 || i == 3)
						DeleteEffect(BITMAP_SPARK + 2, o, i);
				}
				DeleteEffect(BITMAP_LIGHT_MARKS, o);
			}

			if (g_isCharacterBuff((&c->Object), eDeBuff_Blind))
			{
				if (SearchEffect(MODEL_ALICE_BUFFSKILL_EFFECT, o, 3) == false)
				{
					EVector3 vLight;
					Vector(1.0f, 1.0f, 1.0f, vLight);
					CreateEffect(ThreadIndex, MODEL_ALICE_BUFFSKILL_EFFECT, o->Position, o->Angle, vLight, 3, o);
				}
			}
			else
			{
				DeleteEffect(MODEL_ALICE_BUFFSKILL_EFFECT, o, 3);
			}

			if (g_isCharacterBuff((&c->Object), eDeBuff_Sleep))
			{
				if (SearchEffect(MODEL_ALICE_BUFFSKILL_EFFECT, o, 4) == false)
				{
					EVector3 vLight;
					Vector(0.8f, 0.3f, 0.9f, vLight);
					CreateEffect(ThreadIndex, MODEL_ALICE_BUFFSKILL_EFFECT, o->Position, o->Angle, vLight, 4, o);
				}
			}
			else
			{
				DeleteEffect(MODEL_ALICE_BUFFSKILL_EFFECT, o, 4);
			}

			if (g_isCharacterBuff((&c->Object), eDeBuff_AttackDown))
			{
				if (!SearchEffect(BITMAP_SHINY + 6, o, 1))
				{
					EVector3 vLight;
					Vector(1.4f, 0.2f, 0.2f, vLight);
					CreateEffect(ThreadIndex, BITMAP_SHINY + 6, o->Position, o->Angle, vLight, 1, o, -1, 0, 0, 0, 0.5f);
					CreateEffect(ThreadIndex, BITMAP_PIN_LIGHT, o->Position, o->Angle, vLight, 1, o, -1, 0, 0, 0, 1.0f);
				}
			}
			else
			{
				DeleteEffect(BITMAP_SHINY + 6, o, 1);
				DeleteEffect(BITMAP_PIN_LIGHT, o, 1);
			}

			if (g_isCharacterBuff((&c->Object), eDeBuff_DefenseDown))
			{
				if (!SearchEffect(BITMAP_SHINY + 6, o, 2))
				{
					EVector3 vLight(0.25f, 1.0f, 0.7f);
					CreateEffect(ThreadIndex, BITMAP_SHINY + 6, o->Position, o->Angle, vLight, 2, o, -1, 0, 0, 0, 0.5f);
					CreateEffect(ThreadIndex, BITMAP_PIN_LIGHT, o->Position, o->Angle, vLight, 2, o, -1, 0, 0, 0, 1.0f);
				}
			}
			else
			{
				DeleteEffect(BITMAP_SHINY + 6, o, 2);
				DeleteEffect(BITMAP_PIN_LIGHT, o, 2);
			}

			if (g_isCharacterBuff((&c->Object), eDeBuff_SahamuttDOT))
			{
				g_SummonSystem.CreateDamageOfTimeEffect(ThreadIndex, AT_SKILL_SUMMON_EXPLOSION, &c->Object);
			}
			else
			{
				g_SummonSystem.RemoveDamageOfTimeEffect(AT_SKILL_SUMMON_EXPLOSION, &c->Object);
			}

			if (g_isCharacterBuff((&c->Object), eDeBuff_NeilDOT))
			{
				g_SummonSystem.CreateDamageOfTimeEffect(ThreadIndex, AT_SKILL_SUMMON_REQUIEM, &c->Object);
			}
			else
			{
				g_SummonSystem.RemoveDamageOfTimeEffect(AT_SKILL_SUMMON_REQUIEM, &c->Object);
			}

			if (g_isCharacterBuff((&c->Object), eBuff_SwellOfMagicPower))
			{
				if (!g_isCharacterBuff((&c->Object), eBuff_Cloaking))
				{
					if (!SearchEffect(MODEL_SWELL_OF_MAGICPOWER_BUFF_EFF, o, 0))
					{
						EVector3 vLight;
						Vector(0.7f, 0.2f, 0.9f, vLight);
						CreateEffect(ThreadIndex, MODEL_SWELL_OF_MAGICPOWER_BUFF_EFF, o->Position, o->Angle, vLight, 0, o);
					}
				}
			}
			else
			{
				DeleteEffect(MODEL_SWELL_OF_MAGICPOWER_BUFF_EFF, o, 0);
			}

			if (InChaosCastle() == false)
			{
				if (c->ExtendState)
				{
					if ((o->CurrentAction < PLAYER_WALK_MALE ||
						o->CurrentAction == PLAYER_DARKLORD_STAND ||
						o->CurrentAction == PLAYER_STOP_RIDE_HORSE ||
						o->CurrentAction == PLAYER_STOP_TWO_HAND_SWORD_TWO
						) &&
						c->ExtendStateTime >= 100)
					{
						Vector(0.2f, 0.7f, 0.9f, Light);
						CreateEffect(ThreadIndex, BITMAP_LIGHT, o->Position, o->Angle, Light, 3, o);

						c->ExtendStateTime = 0;
					}
					++c->ExtendStateTime;
				}
				if (fullset)
				{
					PartObjectColor(c->BodyPart[5].Type, o->Alpha, 0.5f, Light);

					if (!g_isCharacterBuff(o, eBuff_Cloaking))
					{
						for (mu_int32 i = 0; i < 2; ++i)
						{
							b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[i].LinkBone], p, Position, true);
							CreateSprite(BITMAP_LIGHT, Position, 1.3f, Light, o);
							b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[i].LinkBone - 6], p, Position, true);
							CreateSprite(BITMAP_LIGHT, Position, 1.3f, Light, o);
							b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[i].LinkBone - 7], p, Position, true);
							CreateSprite(BITMAP_LIGHT, Position, 1.3f, Light, o);
						}
					}
					if ((c->BodyPart[BODYPART_BOOTS].Type >= MODEL_BOOTS + 29 && c->BodyPart[BODYPART_BOOTS].Type <= MODEL_BOOTS + 33)
						|| c->BodyPart[BODYPART_BOOTS].Type == MODEL_BOOTS + 43)
					{
						if (ThreadInfo.EquipmentLevelSet > 9)
						{
							VectorCopy(o->Light, Light);

							if (c->BodyPart[BODYPART_BOOTS].Type == MODEL_BOOTS + 29) Vector(0.65f, 0.3f, 0.1f, o->Light);
							if (c->BodyPart[BODYPART_BOOTS].Type == MODEL_BOOTS + 30) Vector(0.1f, 0.1f, 0.9f, o->Light);
							if (c->BodyPart[BODYPART_BOOTS].Type == MODEL_BOOTS + 31) Vector(0.0f, 0.32f, 0.24f, o->Light);
							if (c->BodyPart[BODYPART_BOOTS].Type == MODEL_BOOTS + 32) Vector(0.5f, 0.24f, 0.8f, o->Light);
							if (c->BodyPart[BODYPART_BOOTS].Type == MODEL_BOOTS + 33) Vector(0.6f, 0.4f, 0.0f, o->Light);
							if (c->BodyPart[BODYPART_BOOTS].Type == MODEL_BOOTS + 43)
								Vector(0.6f, 0.3f, 0.4f, o->Light);
							if (ThreadInfo.EquipmentLevelSet == 10)
							{
								if ((GetLargeRand(4)) == 0)
								{
									Vector(0.0f, -18.0f, 50.0f, p);
									b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
									CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
									Vector(0.0f, 0.0f, 70.0f, p);
									b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
									CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
									Vector(0.0f, 18.0f, 50.0f, p);
									b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
									CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
								}
							}
							else if (ThreadInfo.EquipmentLevelSet == 11)
							{
								if ((GetLargeRand(3)) == 0)
								{
									Vector(0.0f, -18.0f, 50.0f, p);
									b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
									CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
									Vector(0.0f, 0.0f, 70.0f, p);
									b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
									CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
									Vector(0.0f, 18.0f, 50.0f, p);
									b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
									CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
								}
							}
							else if (ThreadInfo.EquipmentLevelSet == 12)
							{
								if ((GetLargeRand(2)) == 0)
								{
									Vector(0.0f, -18.0f, 50.0f, p);
									b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
									CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
									Vector(0.0f, 0.0f, 70.0f, p);
									b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
									CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
									Vector(0.0f, 18.0f, 50.0f, p);
									b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
									CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
								}
							}
							else if (ThreadInfo.EquipmentLevelSet == 13)
							{
								Vector(0.0f, -20.0f, 50.0f, p);
								b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
								CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
								Vector(0.0f, 0.0f, 70.0f, p);
								b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
								CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
								Vector(0.0f, 20.0f, 50.0f, p);
								b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
								CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
							}
							else if (ThreadInfo.EquipmentLevelSet == 14)
							{
								Vector(0.0f, -20.0f, 50.0f, p);
								b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
								CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
								Vector(0.0f, 0.0f, 70.0f, p);
								b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
								CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
								Vector(0.0f, 20.0f, 50.0f, p);
								b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
								CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
							}
							else if (ThreadInfo.EquipmentLevelSet == 15)
							{
								Vector(0.0f, -20.0f, 50.0f, p);
								b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
								CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
								Vector(0.0f, 0.0f, 70.0f, p);
								b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
								CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
								Vector(0.0f, 20.0f, 50.0f, p);
								b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
								CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, o->Angle, o->Light, 3);
							}

							VectorCopy(Light, o->Light);
						}
					}

					if (ThreadInfo.EquipmentLevelSet > 9)
					{
						if ((GetLargeRand(20)) == 0)
						{
							VectorCopy(o->Light, Light);
							Vector(1.0f, 1.0f, 1.0f, o->Light);

							if (ThreadInfo.EquipmentLevelSet == 10)
							{
								CreateParticle(ThreadIndex, BITMAP_FLARE, o->Position, o->Angle, o->Light, 0, 0.19f, o);
							}
							else if (ThreadInfo.EquipmentLevelSet == 11)
							{
								if ((GetLargeRand(8)) == 0)
								{
									CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 0, o);
								}
								else
								{
									CreateParticle(ThreadIndex, BITMAP_FLARE, o->Position, o->Angle, o->Light, 0, 0.19f, o);
								}
							}
							else if (ThreadInfo.EquipmentLevelSet == 12)
							{
								if ((MoveSceneFrame % 6) == 0)
								{
									CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 18, o, 20, -1, 0);
									CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 18, o, 20, -1, 1);
								}
								else if ((MoveSceneFrame % 3) == 0)
								{
									CreateParticle(ThreadIndex, BITMAP_FLARE, o->Position, o->Angle, o->Light, 0, 0.19f, o);
								}
							}
							else if (ThreadInfo.EquipmentLevelSet == 13)
							{
								if ((MoveSceneFrame % 6) == 0)
								{

									CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 18, o, 20, -1, 0);
									CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 18, o, 20, -1, 1);
								}

								if ((MoveSceneFrame % 4) == 0)
								{
									CreateParticle(ThreadIndex, BITMAP_FLARE, o->Position, o->Angle, o->Light, 0, 0.19f, o);
									CreateJoint(ThreadIndex, BITMAP_FLARE + 1, o->Position, o->Position, o->Angle, 7, o, 20, 40, 1);
								}
							}
							else if (ThreadInfo.EquipmentLevelSet == 14)
							{
								if ((MoveSceneFrame % 6) == 0)
								{

									CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 18, o, 20, -1, 0);
									CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 18, o, 20, -1, 1);
								}

								if ((MoveSceneFrame % 4) == 0)
								{
									CreateParticle(ThreadIndex, BITMAP_FLARE, o->Position, o->Angle, o->Light, 0, 0.19f, o);
									CreateJoint(ThreadIndex, BITMAP_FLARE + 1, o->Position, o->Position, o->Angle, 7, o, 20, 40, 1);
								}
							}
							else if (ThreadInfo.EquipmentLevelSet == 15)
							{
								if ((MoveSceneFrame % 6) == 0)
								{

									CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 18, o, 20, -1, 0);
									CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 18, o, 20, -1, 1);
								}

								if ((MoveSceneFrame % 4) == 0)
								{
									CreateParticle(ThreadIndex, BITMAP_FLARE, o->Position, o->Angle, o->Light, 0, 0.19f, o);
									CreateJoint(ThreadIndex, BITMAP_FLARE + 1, o->Position, o->Position, o->Angle, 7, o, 20, 40, 1);
								}
							}

							VectorCopy(Light, o->Light);
						}

						if (ThreadInfo.EquipmentLevelSet == 15)
						{
							EVector3 vColor;
							Vector(13.0f, 10.0f, 3.0f, p);
							b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(20)], p, Position, true);
							VectorCopy(Position, o->EyeLeft);
							Vector(0.09f, 0.09f, 0.8f, vColor);
							CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, Position, o->Position, o->Angle, 55, o, 6.0f, -1, 0, 0, -1, &vColor);
							mu_float fRad = (mu_float)EMath::Sin((FWorldTime)*0.002f);
							Vector(0.2f, 0.4f, 0.8f, vColor);
							CreateSprite(BITMAP_SHINY + 6, Position, 0.5f*fRad, vColor, o);
							Vector(0.1f, 0.15f, 1.0f, vColor);
							CreateSprite(BITMAP_PIN_LIGHT, Position, 1.3f*fRad + 0.5f, vColor, o, 100.0f);

							Vector(13.0f, 10.0f, -3.0f, p);
							b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(20)], p, Position, true);
							VectorCopy(Position, o->EyeRight);
							Vector(0.09f, 0.09f, 0.8f, vColor);
							CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, Position, o->Position, o->Angle, 56, o, 6.0f, -1, 0, 0, -1, &vColor);
							Vector(0.2f, 0.4f, 0.8f, vColor);
							CreateSprite(BITMAP_SHINY + 6, Position, 0.5f*fRad, vColor, o);
							Vector(0.1f, 0.15f, 1.0f, vColor);
							CreateSprite(BITMAP_PIN_LIGHT, Position, 1.3f*fRad + 0.5f, vColor, o, 80.0f);
						}
					}
				}
			}
			break;
		case MODEL_MONSTER01:
		case MODEL_MONSTER01 + 30:
			if ((o->Type == MODEL_MONSTER01&&c->Level == 1) || (o->Type == MODEL_MONSTER01 + 30))
				RenderEye(ThreadIndex, o, 22, 23);
			break;
		case MODEL_MONSTER01 + 52:
			RenderEye(ThreadIndex, o, 26, 27, 2.0f);
			break;
		case MODEL_MONSTER01 + 37:
			RenderLight(ThreadIndex, o, BITMAP_LIGHTNING + 1, 1.0f, 63, 0.0f, 0.0f, 20.0f);
			RenderLight(ThreadIndex, o, BITMAP_SHINY + 2, 4.0f, 63, 0.0f, 0.0f, 20.0f);
			break;
		case MODEL_MONSTER01 + 34:
			RenderLight(ThreadIndex, o, BITMAP_LIGHTNING + 1, 0.5f, 30, 0.0f, 0.0f, -5.0f);
			RenderLight(ThreadIndex, o, BITMAP_LIGHTNING + 1, 0.5f, 39, 0.0f, 0.0f, -5.0f);
			RenderLight(ThreadIndex, o, BITMAP_SPARK, 4.0f, 30, 0.0f, 0.0f, -5.0f);
			RenderLight(ThreadIndex, o, BITMAP_SPARK, 4.0f, 39, 0.0f, 0.0f, -5.0f);
			RenderLight(ThreadIndex, o, BITMAP_SHINY + 2, 2.0f, 30, 0.0f, 0.0f, -5.0f);
			RenderLight(ThreadIndex, o, BITMAP_SHINY + 2, 2.0f, 39, 0.0f, 0.0f, -5.0f);
			break;
		case MODEL_MONSTER01 + 36:
			RenderEye(ThreadIndex, o, 42, 43);
			RenderLight(ThreadIndex, o, BITMAP_SPARK, 2.0f, 26, 0.0f, 0.0f, 0.0f);
			RenderLight(ThreadIndex, o, BITMAP_SPARK, 2.0f, 31, 0.0f, 0.0f, 0.0f);
			RenderLight(ThreadIndex, o, BITMAP_SPARK, 2.0f, 36, 0.0f, 0.0f, 0.0f);
			RenderLight(ThreadIndex, o, BITMAP_SPARK, 2.0f, 41, 0.0f, 0.0f, 0.0f);
			RenderLight(ThreadIndex, o, BITMAP_SHINY + 2, 1.0f, 26, 0.0f, 0.0f, 0.0f);
			RenderLight(ThreadIndex, o, BITMAP_SHINY + 2, 1.0f, 31, 0.0f, 0.0f, 0.0f);
			RenderLight(ThreadIndex, o, BITMAP_SHINY + 2, 1.0f, 36, 0.0f, 0.0f, 0.0f);
			RenderLight(ThreadIndex, o, BITMAP_SHINY + 2, 1.0f, 41, 0.0f, 0.0f, 0.0f);
			break;
		case MODEL_MONSTER01 + 33:
			RenderLight(ThreadIndex, o, BITMAP_SPARK, 4.0f, 9, 0.0f, 0.0f, 5.0f);
			RenderLight(ThreadIndex, o, BITMAP_SHINY + 2, 3.0f, 9, 0.0f, 0.0f, 5.0f);
			break;
		case MODEL_MIX_NPC:
			RenderLight(ThreadIndex, o, BITMAP_LIGHT, 1.5f, 32, 0.0f, 0.0f, 0.0f);
			break;
		case MODEL_NPC_SEVINA:
			RenderLight(ThreadIndex, o, BITMAP_LIGHT, 2.5f, 6, 0.0f, 0.0f, 0.0f);
			break;
		case MODEL_NPC_DEVILSQUARE:
			{
				Luminosity = (mu_float)EMath::Sin((FWorldTime)*0.002f)*0.35f + 0.65f;

				mu_float Scale = 0.3f;
				Vector(Luminosity, Luminosity, Luminosity, Light);

				Vector(3.5f, -12.0f, 10.0f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(20)], p, Position, 1);
				CreateSprite(BITMAP_LIGHTNING + 1, Position, Scale, Light, o, (FWorldTime / 50.0f));
				CreateSprite(BITMAP_LIGHTNING + 1, Position, Scale, Light, o, ((-FWorldTime) / 50.0f));

				if (GetLargeRand(30) == 0)
				{
					p[0] = Position[0] + GetLargeRand(100) - 50;
					p[1] = Position[1] + GetLargeRand(100) - 50;
					p[2] = Position[2] + GetLargeRand(100) - 50;

					CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, p, Position, o->Angle, 6, nullptr, 20.0f);
				}
			}
			break;
		case MODEL_NPC_CASTEL_GATE:
			{
				EVector3 vPos, vRelative;
				mu_float fLumi, fScale;

				fLumi = (EMath::Sin(FWorldTime*0.002f) + 2.0f) * 0.5f;
				Vector(fLumi*1.0f, fLumi*0.5f, fLumi*0.3f, Light);
				fScale = fLumi * 0.5f;

				Vector(4.0f, 3.0f, -4.0f, vRelative);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(2)], vRelative, vPos, true);

				CreateSprite(BITMAP_LIGHT, vPos, fScale, Light, o);
				CreateSprite(BITMAP_KANTURU_2ND_EFFECT1, vPos, fScale, Light, o);

				Vector(5.0f, 3.0f, 2.0f, vRelative);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(4)], vRelative, vPos, true);

				CreateSprite(BITMAP_LIGHT, vPos, fScale, Light, o);
				CreateSprite(BITMAP_KANTURU_2ND_EFFECT1, vPos, fScale, Light, o);

				if (GetLargeRand(4) == 0)
				{
					Vector(-20.0f, 10.0f, 0.0f, vRelative);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(3)], vRelative, vPos, true);

					EVector3 Angle;
					VectorCopy(o->Angle, Angle);
					Angle[0] += 120.0f;
					Angle[2] -= 60.0f;

					Vector(1.0f, 1.0f, 1.0f, Light);

					CreateParticle(ThreadIndex, BITMAP_FLAME, vPos, Angle, Light, 10, o->Scale / 2);
				}
			}
			break;
		case MODEL_MONSTER01 + 28:
			Vector(0.0f, 0.0f, 0.0f, p);
			Luminosity = 1.0f;
			if (c->Level == 0)
			{
				Vector(Luminosity*1.0f, Luminosity*1.0f, Luminosity*1.0f, Light);
			}
			else
			{
				Vector(Luminosity*1.0f, Luminosity*0.4f, Luminosity*0.0f, Light);
				Vector(Luminosity*0.2f, Luminosity*0.7f, Luminosity*0.1f, Light);
			}

			for (mu_int32 i = 0; i < b->_BoneNonDummyEnd; ++i)
			{
				const mu_int16 reverseIndex = b->_ReverseBonesIndex[i];
				if ((reverseIndex >= 15 && reverseIndex <= 20) || (reverseIndex >= 27 && reverseIndex <= 32))
				{
				}
				else
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[i], p, Position, true);
					if (c->Level == 0)
						CreateSprite(BITMAP_SHINY + 1, Position, 2.5f, Light, o, 0.0f, 1);
					else
						CreateSprite(BITMAP_MAGIC + 1, Position, 0.8f, Light, o, 0.0f);
					if (GetLargeRand(4) == 0 && o->CurrentAction >= MONSTER01_ATTACK1 && o->CurrentAction <= MONSTER01_ATTACK2)
					{
						CreateParticle(ThreadIndex, BITMAP_ENERGY, Position, o->Angle, Light);
					}
				}
			}
			break;
		case MODEL_SEED_MASTER:
			{
				mu_float fLumi, fScale;
				fScale = 2.0f;
				b->TransformByObjectBone(ThreadIndex, Position, o, 39);

				fLumi = (EMath::Sin(FWorldTime*0.001f) + 1.0f) * 0.3f + 0.4f;
				Vector(fLumi*0.5f, fLumi*0.5f, fLumi*0.5f, Light);
				CreateSprite(BITMAP_FLARE, Position, fScale, Light, o);
			}
			break;
		case MODEL_SEED_INVESTIGATOR:
			{
				mu_float fLumi, fScale;
				fScale = 1.0f;
				for (mu_int32 i = 69; i <= 70; ++i)
				{
					b->TransformByObjectBone(ThreadIndex, Position, o, i);

					fLumi = (EMath::Sin(FWorldTime*0.001f) + 1.0f) * 0.3f + 0.4f;
					Vector(fLumi*0.5f, fLumi*0.5f, fLumi*0.5f, Light);
					CreateSprite(BITMAP_FLARE_BLUE, Position, fScale, Light, o);
				}
			}
			break;
		default:
			{
				if (TheMapProcess().RenderMonsterVisual(ThreadIndex, c, o, b) == true) break;
			}
		}
	}

	if (o->Alpha >= 0.5f && c->HideShadow == false)
	{
		if (GET_WORLD != WD_10HEAVEN &&
			o->Type == MODEL_PLAYER &&
			InHellas() == false)
		{
			if (InBloodCastle() && o->Flags.Get(OBJECT::eOF_ACTIONSTART) == true && c->Dead > 0)
			{
				mu_float height = RequestTerrainHeight(o->Position[0], o->Position[1]);
				if (height < o->Position[2])
				{
					ThreadInfo.ShadowHeight = height;
				}
			}
			ThreadInfo.EnableShadow = false;
		}
	}

	if ((
		o->Type != MODEL_PLAYER && o->Kind != KIND_TRAP &&
		c->MonsterIndex != 25 && c->MonsterIndex != 22 && c->MonsterIndex != 42 && c->MonsterIndex != 242 && c->MonsterIndex != 59 && c->MonsterIndex != 63
		&& c->MonsterIndex != 152
		) && GET_WORLD != WD_10HEAVEN
		)
	{
		if (o->Alpha >= 0.3f)
		{
			if (InBloodCastle() == true && o->Flags.Get(OBJECT::eOF_ACTIONSTART) == true && c->Dead > 0)
			{
				mu_float height = RequestTerrainHeight(o->Position[0], o->Position[1]);
				if (height < o->Position[2])
				{
					ThreadInfo.ShadowHeight = height;
				}
			}

			if (o->Type != MODEL_MONSTER01 + 60 && o->Type != MODEL_MONSTER01 + 61
				&& !(o->Type >= MODEL_FACE
					&& o->Type <= MODEL_FACE + 5))
			{
				ThreadInfo.EnableShadow = false;
			}
		}
	}

	if (g_CurrentScene == MAIN_SCENE)
	{
		if ((o->Kind == KIND_PLAYER
			&& InBloodCastle() == false
			&& InChaosCastle() == false
			&& battleCastle::InBattleCastle()) == false)
		{
			RenderProtectGuildMark(ThreadIndex, c);
		}
	}

	giPetManager::RenderPet(ThreadIndex, c);

	if (GetBaseClass(c->Class) == GAME::CLASS_SUMMONER)
	{
		PART_t *w = &c->Weapon[1];
		g_SummonSystem.MoveEquipEffect(ThreadIndex, c, w->Type, w->Level, w->ExcOption);
	}
}

void MUTasking::CheckCharacters(const mu_uint32 ThreadIndex)
{
	mu_uint32 queueCount = 0;
	const mu_uint32 queueIndex = MUThreadQueues::CharactersQueueIndex[ThreadIndex];

	mu_uint32 i = ThreadIndex;
	CHARACTER *c = &CharactersClient[ThreadIndex];
	if (ThreadIndex == 0)
	{
		OBJECT *o = &c->Object;
		if (g_CurrentScene == MAIN_SCENE && (Hero->CtlCode & 0x04) != 0)
		{
			o->OBB.StartPos[0] = 1000.0f;
			o->OBB.XAxis[0] = o->OBB.YAxis[1] = o->OBB.ZAxis[2] = 1.0f;
		}
		else if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
		{
			const mu_boolean isVisible = TestFrustrum2D(o->Position, 100.0f);
			if (isVisible)
			{
				o->Flags.Set(OBJECT::eOF_VISIBLE);
			}
			else
			{
				o->Flags.Clear(OBJECT::eOF_VISIBLE);
			}

			if (isVisible)
			{
				MUThreadQueues::CharactersQueue[queueIndex + queueCount++] = i;
			}
		}

		i += _ThreadsCount;
		c += _ThreadsCount;
	}

	if (battleCastle::IsBattleCastleStart() == true)
	{
		mu_uint32 codePath = 0;

		if (Hero->EtcPart == PARTS_ATTACK_KING_TEAM_MARK || Hero->EtcPart == PARTS_ATTACK_TEAM_MARK)
		{
			codePath = 1;
		}
		else if (Hero->EtcPart == PARTS_ATTACK_KING_TEAM_MARK2 || Hero->EtcPart == PARTS_ATTACK_TEAM_MARK2)
		{
			codePath = 2;
		}
		else if (Hero->EtcPart == PARTS_ATTACK_KING_TEAM_MARK3 || Hero->EtcPart == PARTS_ATTACK_TEAM_MARK3)
		{
			codePath = 3;
		}
		else if (Hero->EtcPart == PARTS_DEFENSE_KING_TEAM_MARK || Hero->EtcPart == PARTS_DEFENSE_TEAM_MARK)
		{
			codePath = 4;
		}

		switch (codePath)
		{
		case 0:
			{
				for (; i < MAX_CHARACTERS_CLIENT; i += _ThreadsCount, c += _ThreadsCount)
				{
					CHARACTER *c = &CharactersClient[i];
					OBJECT *o = &c->Object;
					if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
					{
						const mu_boolean isVisible = TestFrustrum2D(o->Position, 100.0f);
						if (isVisible)
						{
							o->Flags.Set(OBJECT::eOF_VISIBLE);
						}
						else
						{
							o->Flags.Clear(OBJECT::eOF_VISIBLE);
						}

						if (isVisible)
						{
							MUThreadQueues::CharactersQueue[queueIndex + queueCount++] = i;
						}
					}
				}
			}
			break;

		case 1:
			{
				for (; i < MAX_CHARACTERS_CLIENT; i += _ThreadsCount, c += _ThreadsCount)
				{
					OBJECT *o = &c->Object;

					if (g_isCharacterBuff(o, eBuff_Cloaking))
					{
						if (!(c->EtcPart == PARTS_ATTACK_KING_TEAM_MARK || c->EtcPart == PARTS_ATTACK_TEAM_MARK))
						{
							if (Hero->TargetCharacter == c->Key)
								Hero->TargetCharacter = -1;
							continue;
						}
					}

					if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
					{
						const mu_boolean isVisible = TestFrustrum2D(o->Position, 100.0f);
						if (isVisible)
						{
							o->Flags.Set(OBJECT::eOF_VISIBLE);
						}
						else
						{
							o->Flags.Clear(OBJECT::eOF_VISIBLE);
						}

						if (isVisible)
						{
							MUThreadQueues::CharactersQueue[queueIndex + queueCount++] = i;
						}
					}
				}
			}
			break;

		case 2:
			{
				for (; i < MAX_CHARACTERS_CLIENT; i += _ThreadsCount, c += _ThreadsCount)
				{
					OBJECT *o = &c->Object;

					if (g_isCharacterBuff(o, eBuff_Cloaking))
					{
						if (!(c->EtcPart == PARTS_ATTACK_KING_TEAM_MARK2 || c->EtcPart == PARTS_ATTACK_TEAM_MARK2))
						{
							if (Hero->TargetCharacter == c->Key)
								Hero->TargetCharacter = -1;
							continue;
						}
					}
					
					if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
					{
						const mu_boolean isVisible = TestFrustrum2D(o->Position, 100.0f);
						if (isVisible)
						{
							o->Flags.Set(OBJECT::eOF_VISIBLE);
						}
						else
						{
							o->Flags.Clear(OBJECT::eOF_VISIBLE);
						}

						if (isVisible)
						{
							MUThreadQueues::CharactersQueue[queueIndex + queueCount++] = i;
						}
					}
				}
			}
			break;

		case 3:
			{
				for (; i < MAX_CHARACTERS_CLIENT; i += _ThreadsCount, c += _ThreadsCount)
				{
					OBJECT *o = &c->Object;

					if (g_isCharacterBuff(o, eBuff_Cloaking))
					{
						if (!(c->EtcPart == PARTS_ATTACK_KING_TEAM_MARK3 || c->EtcPart == PARTS_ATTACK_TEAM_MARK3))
						{
							if (Hero->TargetCharacter == c->Key)
								Hero->TargetCharacter = -1;
							continue;
						}
					}

					if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
					{
						const mu_boolean isVisible = TestFrustrum2D(o->Position, 100.0f);
						if (isVisible)
						{
							o->Flags.Set(OBJECT::eOF_VISIBLE);
						}
						else
						{
							o->Flags.Clear(OBJECT::eOF_VISIBLE);
						}

						if (isVisible)
						{
							MUThreadQueues::CharactersQueue[queueIndex + queueCount++] = i;
						}
					}
				}
			}
			break;

		case 4:
			{
				for (; i < MAX_CHARACTERS_CLIENT; i += _ThreadsCount, c += _ThreadsCount)
				{
					OBJECT *o = &c->Object;

					if (g_isCharacterBuff(o, eBuff_Cloaking))
					{
						if (!(c->EtcPart == PARTS_DEFENSE_KING_TEAM_MARK || c->EtcPart == PARTS_DEFENSE_TEAM_MARK))
						{
							if (Hero->TargetCharacter == c->Key)
								Hero->TargetCharacter = -1;
							continue;
						}
					}

					if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
					{
						const mu_boolean isVisible = TestFrustrum2D(o->Position, 100.0f);
						if (isVisible)
						{
							o->Flags.Set(OBJECT::eOF_VISIBLE);
						}
						else
						{
							o->Flags.Clear(OBJECT::eOF_VISIBLE);
						}

						if (isVisible)
						{
							MUThreadQueues::CharactersQueue[queueIndex + queueCount++] = i;
						}
					}
				}
			}
			break;
		}
	}
	else
	{
		for (; i < MAX_CHARACTERS_CLIENT; i += _ThreadsCount, c += _ThreadsCount)
		{
			CHARACTER *c = &CharactersClient[i];
			OBJECT *o = &c->Object;
			if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
			{
				const mu_boolean isVisible = TestFrustrum2D(o->Position, 100.0f);
				if (isVisible)
				{
					o->Flags.Set(OBJECT::eOF_VISIBLE);
				}
				else
				{
					o->Flags.Clear(OBJECT::eOF_VISIBLE);
				}

				if (isVisible)
				{
					MUThreadQueues::CharactersQueue[queueIndex + queueCount++] = i;
				}
			}
		}
	}

	MUThreadQueues::CharactersQueueCount[ThreadIndex] = queueCount;
}

void MUTasking::PrepareCharacters(const mu_uint32 ThreadIndex)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (battleCastle::InBattleCastle() == true &&
		battleCastle::IsBattleCastleStart() == true)
	{
		mu_uint32 q = ThreadIndex;
		for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
		{
			const mu_uint32 queueCount = MUThreadQueues::CharactersQueueCount[t];
			const mu_uint16 *queue = MUThreadQueues::CharactersQueue.data() + MUThreadQueues::CharactersQueueIndex[t];

			for (; q < queueCount; q += _ThreadsCount)
			{
				const mu_uint32 characterIndex = queue[q];
				CHARACTER *c = &CharactersClient[characterIndex];
				OBJECT *o = &c->Object;

				ThreadInfo.RenderIndex = 0;
				ThreadInfo.CulledType = CULLED_CHARACTERS;
				ThreadInfo.ObjectIndex = characterIndex;

				if (characterIndex != SelectedCharacter && characterIndex != SelectedNpc)
					RenderCharacter(ThreadIndex, c, o);
				else
					RenderCharacter(ThreadIndex, c, o, 1);

				if (o->Type == MODEL_PLAYER &&
					MU_UPDATECOUNTER > 0)
				{
					battleCastle::CreateGuardStoneHealingVisual(ThreadIndex, c, 380.0f);
				}
			}

			q -= queueCount;
		}
	}
	else
	{
		mu_uint32 q = ThreadIndex;
		for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
		{
			const mu_uint32 queueCount = MUThreadQueues::CharactersQueueCount[t];
			const mu_uint16 *queue = MUThreadQueues::CharactersQueue.data() + MUThreadQueues::CharactersQueueIndex[t];

			for (; q < queueCount; q += _ThreadsCount)
			{
				const mu_uint32 characterIndex = queue[q];
				CHARACTER *c = &CharactersClient[characterIndex];
				OBJECT *o = &c->Object;

				ThreadInfo.RenderIndex = 0;
				ThreadInfo.CulledType = CULLED_CHARACTERS;
				ThreadInfo.ObjectIndex = characterIndex;

				if (characterIndex != SelectedCharacter && characterIndex != SelectedNpc)
					RenderCharacter(ThreadIndex, c, o);
				else
					RenderCharacter(ThreadIndex, c, o, 1);
			}

			q -= queueCount;
		}
	}
}