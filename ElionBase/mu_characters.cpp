#include "stdafx.h"
#include "mu_characters.h"

std::array<mu_int16, MAX_KEYS> CharactersKeys;
std::array<CHARACTER, MAX_CHARACTERS_CLIENT + 1> CharactersClient;
std::array<mu_uint16, MAX_CHARACTERS_CLIENT + MAX_TASKING_THREADS> g_CharactersQueue;

mu_boolean CharacterViewLive = false;
mu_boolean CharacterViewVisible = false;
CHARACTER CharacterView;
MUCriticalSectionSpinLock CharactersClientLock;

mu_int32 HeroKey = 0;
const mu_int32 HeroIndex = 0;
CHARACTER *Hero = &CharactersClient[HeroIndex];
mu_int32 HeroAngle = 0;

mu_boolean Teleport = false;

mu_int32 AttackPlayer = 0;

mu_int32 StandTime = 0;
mu_int32 ActionTarget = -1;

MovementSkill g_MovementSkill;

PART_t::PART_t()
{
	Type = -1;
	Level = 0;
	ExcOption = 0;
	SetOption = 0;
	LinkBone = -1;
	CurrentAction = 0;
	PriorAction = 0;
	AnimationFrame = 0;
	PriorAnimationFrame = 0;
	PlaySpeed = 0;
	m_byNumCloth = 0;
	m_pCloth[0] = nullptr;
	m_pCloth[1] = nullptr;
}

PART_t::~PART_t()
{
	MU_SAFE_DELETE(m_pCloth[0]);
	MU_SAFE_DELETE(m_pCloth[1]);
}

CHARACTER::CHARACTER()
{
	Index = -1;

	Initialize();
}

CHARACTER::~CHARACTER()
{
	Destroy();
}

void CHARACTER::Initialize()
{
	Object.Initialize();

	Blood = false;
	Ride = false;
	SkillSuccess = false;
	m_bFixForm = false;
	Foot[0] = false;
	Foot[1] = false;
	SafeZone = false;
	Change = false;
	HideShadow = false;
	m_bIsSelected = false;
	Decoy = false;

	Class = 0;
	Skin = 0;
	CtlCode = 0;
	ExtendState = 0;
	EtcPart = 0;
	GuildStatus = GAME::G_NONE;
	GuildType = 0;
	GuildRelationShip = 0;
	GuildSkill = 0;
	GuildMasterKillCount = 0;
	BackupCurrentSkill = 0;
	GuildTeam = 0;
	m_byGensInfluence = 0;
	PK = 0;
	AttackFlag = 0;
	AttackTime = 0;
	TargetAngle = 0;
	Dead = 0;
	Run = 0;
	Skill = 0;
	SwordCount = 0;
	byExtensionSkill = 0;
	m_byDieType = 0;
	StormTime = 0;
	JumpTime = 0;
	TargetX = 0;
	TargetY = 0;
	SkillX = 0;
	SkillY = 0;
	Appear = 0;
	CurrentSkill = 0;
	CastRenderTime = 0;
	m_byFriend = 0;
	MonsterSkill = 0;

	mu_zeromem(ID, sizeof(ID));
	mu_zeromem(OwnerID, sizeof(OwnerID));

	Movement = false;
	CollisionTime = 0;
	GuildMarkIndex = -1;
	Key = -1;
	TargetCharacter = 0;

	Level = 0;
	MonsterIndex = 0;
	Damage = 0;
	Hit = 0;
	MoveSpeed = 0;

	Action = 0;
	ExtendStateTime = 0;
	LongRangeAttack = 0;
	SelectItem = 0;
	Item = 0;
	PriorPositionX = 0;
	PriorPositionY = 0;
	PositionX = 0;
	PositionY = 0;
	m_iDeleteTime = 0;
	m_iFenrirSkillTarget = -1;

	ProtectGuildMarkWorldTime = 0.0f;
	AttackRange = 0.0f;
	Duplication = 0.0f;
	Rot = 0.0f;

	TargetPosition = EVector3(0.0f, 0.0f, 0.0f);
	Light = EVector3(0.0f, 0.0f, 0.0f);

	m_pParts = nullptr;

	m_pPet = nullptr;

	m_pTempParts = nullptr;
	m_iTempKey = 0;
	m_CursedTempleCurSkill = AT_SKILL_CURSED_TEMPLE_PRODECTION;
	m_CursedTempleCurSkillPacket = false;
	m_byRankIndex = 0;
	m_nContributionPoint = 0;

	EnabledInternalAction = INTERNAL_ACTION::IA_NONE;
}

void CHARACTER::Destroy()
{

}

mu_uint8 vec_list[35] =
{
	5,  6, 33, 53, 35, 49, 50, 45, 46, 41,
	42, 37, 38, 11, 31, 13, 27, 28, 23, 24,
	19, 20, 15, 16, 54, 55, 62, 69, 70, 77,
	2, 79, 81, 84, 86
};
mu_uint8 wingLeft[15][2] =
{
	{ 0,2 },{ 2,3 },{ 2,4 },{ 4,5 },{ 5,6 },{ 4,7 },{ 7,8 },{ 4,9 },{ 9,10 },{ 4,11 },{ 11,12 },
	{ 6,5 },{ 8,7 },{ 10,9 },{ 12,11 }
};
mu_uint8 wingRight[15][2] =
{
	{ 0,13 },{ 13,14 },{ 13,15 },{ 15,16 },{ 16,17 },{ 15,18 },{ 18,19 },{ 15,20 },{ 20,21 },{ 15,22 },{ 22,23 },
	{ 17,16 },{ 19,18 },{ 21,20 },{ 23,22 }
};
mu_uint8 arm_leg_Left[4][2] =
{
	{ 29,28 },{ 28,27 },{ 34,33 },{ 33,30 },
};
mu_uint8 arm_leg_Right[4][2] =
{
	{ 26,25 },{ 25,24 },{ 32,31 },{ 31,30 },
};
mu_uint8 g_chStar[10] =
{
	10, 18, 37, 38, 51, 52, 58, 59, 66, 24
};

mu_int32 GetFenrirType(CHARACTER* c)
{
	if (c->Helper.ExcOption == 0x01)
		return FENRIR_TYPE_BLACK;
	else if (c->Helper.ExcOption == 0x02)
		return FENRIR_TYPE_BLUE;
	else if (c->Helper.ExcOption == 0x04)
		return FENRIR_TYPE_GOLD;

	return FENRIR_TYPE_RED;
}

void FallingMonster(CHARACTER *c, OBJECT* o)
{
	mu_float AngleY = 0.0f;

	o->Gravity += 2.5f;
	o->Angle[0] -= 4.0f;
	o->Flags.Set(OBJECT::eOF_ACTIONSTART);
	o->Direction[1] += o->Direction[0];

	if (o->Gravity >= 1.0f)
		AngleY = o->Angle[2];

	if (o->Angle[0] <= -90.0f)
		o->Angle[0] = -90.0f;

	o->Angle[2] = AngleY;

	if (o->Gravity >= 150.0f)
		o->Gravity = 150.0f;

	o->Position[2] -= o->Gravity;
}

mu_boolean PlayMonsterSoundGlobal(OBJECT * pObject)
{
	mu_float fDis_x, fDis_y;
	fDis_x = pObject->Position[0] - Hero->Object.Position[0];
	fDis_y = pObject->Position[1] - Hero->Object.Position[1];
	mu_float fDistance = EMath::Sqrt(fDis_x*fDis_x + fDis_y*fDis_y);

	if (fDistance > 500.0f) return true;

	switch (pObject->Type)
	{
	case MODEL_MONSTER01 + 155:
		if (pObject->CurrentAction == MONSTER01_STOP1)
		{
			if (GetLargeRand(2) == 0)
				PlayBackground(SOUND_XMAS_SANTA_IDLE_1);
			else
				PlayBackground(SOUND_XMAS_SANTA_IDLE_2);
		}
		else if (pObject->CurrentAction == MONSTER01_WALK)
		{
			if (GetLargeRand(2) == 0)
				PlayBackground(SOUND_XMAS_SANTA_WALK_1);
			else
				PlayBackground(SOUND_XMAS_SANTA_WALK_2);
		}
		else if (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2)
		{
			PlayBackground(SOUND_XMAS_SANTA_ATTACK_1);
		}
		else if (pObject->CurrentAction == MONSTER01_SHOCK)
		{
			if (GetLargeRand(2) == 0)
				PlayBackground(SOUND_XMAS_SANTA_DAMAGE_1);
			else
				PlayBackground(SOUND_XMAS_SANTA_DAMAGE_2);
		}
		else if (pObject->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_XMAS_SANTA_DEATH_1);
		}
		return true;

	case MODEL_XMAS2008_SNOWMAN:
		if (pObject->CurrentAction == MONSTER01_WALK)
		{
			PlayBackground(SOUND_XMAS_SNOWMAN_WALK_1);
		}
		else if (pObject->CurrentAction == MONSTER01_ATTACK1)
		{
			PlayBackground(SOUND_XMAS_SNOWMAN_ATTACK_1);
		}
		else if (pObject->CurrentAction == MONSTER01_ATTACK2)
		{
			PlayBackground(SOUND_XMAS_SNOWMAN_ATTACK_2);
		}
		else if (pObject->CurrentAction == MONSTER01_SHOCK)
		{
			PlayBackground(SOUND_XMAS_SNOWMAN_DAMAGE_1);
		}
		else if (pObject->CurrentAction == MONSTER01_DIE)
		{
			if (pObject->LifeTime == 100)
			{
				PlayBackground(SOUND_XMAS_SNOWMAN_DEATH_1);
			}
		}
		return true;

	case MODEL_DUEL_NPC_TITUS:
		if (pObject->CurrentAction == MONSTER01_STOP1)
		{
			PlayBackground(SOUND_DUEL_NPC_IDLE_1);
		}
		return true;

	case MODEL_DOPPELGANGER_NPC_LUGARD:
		if (pObject->CurrentAction == MONSTER01_STOP1)
		{
			if (GetLargeRand(2) == 0)
				PlayBackground(SOUND_DOPPELGANGER_LUGARD_BREATH);
		}
		return true;
	case MODEL_DOPPELGANGER_NPC_BOX:
		if (pObject->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_DOPPELGANGER_JEWELBOX_OPEN);
		}
		return true;
	case MODEL_DOPPELGANGER_NPC_GOLDENBOX:
		if (pObject->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_DOPPELGANGER_JEWELBOX_OPEN);
		}
		return true;
	}

	return false;
}

mu_boolean PlayMonsterSound(const mu_uint32 ThreadIndex, OBJECT * pObject)
{
	if (PlayMonsterSoundGlobal(pObject))
	{
		return true;
	}

	if (TheMapProcess().PlayMonsterSound(ThreadIndex, pObject) == true)
	{
		return true;
	}

	return false;
}

mu_float GetPlaySpeed(OBJECT *o, mu_int32 Type, mu_uint16 CurrentAction)
{
	if (Type == MODEL_PLAYER)
	{
		mu_float AttackSpeed1 = o->AttackSpeed * 0.004f;
		mu_float MagicSpeed1 = o->MagicSpeed * 0.004f;
		mu_float MagicSpeed2 = o->MagicSpeed * 0.002f;

		switch (CurrentAction)
		{
		case PLAYER_ATTACK_FIST:
			return 0.6f + AttackSpeed1;

		case PLAYER_ATTACK_SKILL_SWORD1:
			return 0.30f + AttackSpeed1;
		case PLAYER_ATTACK_SKILL_SWORD2:
			return 0.30f + AttackSpeed1;
		case PLAYER_ATTACK_SKILL_SWORD3:
			return 0.27f + AttackSpeed1;
		case PLAYER_ATTACK_SKILL_SWORD4:
			return 0.30f + AttackSpeed1;
		case PLAYER_ATTACK_SKILL_SWORD5:
			return 0.24f + AttackSpeed1;
		case PLAYER_ATTACK_SKILL_WHEEL:
			return o->Flags.Get(OBJECT::eOF_SECONDANIMATION) == true ? 0.54f + AttackSpeed1 : 0.24f + AttackSpeed1;

		case PLAYER_ATTACK_ONETOONE:
			return 0.25f + AttackSpeed1;
		case PLAYER_ATTACK_SKILL_SPEAR:
			return 0.30f + AttackSpeed1;

		case PLAYER_SKILL_RIDER:
			return 0.3f + AttackSpeed1;
		case PLAYER_SKILL_RIDER_FLY:
			return 0.3f + AttackSpeed1;

		case PLAYER_ATTACK_TWO_HAND_SWORD_TWO:
			return 0.25f + AttackSpeed1;

		case PLAYER_SKILL_ELF1:
			return 0.25f + MagicSpeed1;

		case PLAYER_SKILL_TELEPORT:
			return 0.30f + MagicSpeed2;
		case PLAYER_SKILL_FLASH:
			return 0.40f + MagicSpeed2;
		case PLAYER_SKILL_INFERNO:
			return 0.60f + MagicSpeed2;
		case PLAYER_SKILL_HELL:
			return 0.50f + MagicSpeed2;
		case PLAYER_RIDE_SKILL:
			return 0.30f + MagicSpeed2;

		case PLAYER_SKILL_HELL_BEGIN:
			return 0.50f + MagicSpeed2;
		case PLAYER_ATTACK_STRIKE:
			return 0.25f + AttackSpeed1;
		case PLAYER_ATTACK_RIDE_STRIKE:
			return 0.2f + AttackSpeed1;
		case PLAYER_ATTACK_RIDE_HORSE_SWORD:
			return 0.25f + AttackSpeed1;
		case PLAYER_ATTACK_RIDE_ATTACK_FLASH:
			return 0.40f + MagicSpeed2;
		case PLAYER_ATTACK_RIDE_ATTACK_MAGIC:
			return 0.3f + MagicSpeed2;

		case PLAYER_FENRIR_ATTACK:
			return 0.25f + AttackSpeed1;
		case PLAYER_FENRIR_ATTACK_DARKLORD_STRIKE:
			return 0.2f + AttackSpeed1;
		case PLAYER_FENRIR_ATTACK_DARKLORD_SWORD:
			return 0.25f + AttackSpeed1;
		case PLAYER_FENRIR_ATTACK_DARKLORD_FLASH:
			return 0.40f + MagicSpeed2;
		case PLAYER_FENRIR_ATTACK_TWO_SWORD:
			return 0.25f + AttackSpeed1;
		case PLAYER_FENRIR_ATTACK_MAGIC:
			return 0.37f + MagicSpeed2;
		case PLAYER_FENRIR_ATTACK_CROSSBOW:
			return 0.30f + AttackSpeed1;
		case PLAYER_FENRIR_ATTACK_SPEAR:
			return 0.25f + AttackSpeed1;
		case PLAYER_FENRIR_ATTACK_ONE_SWORD:
			return 0.25f + AttackSpeed1;
		case PLAYER_FENRIR_ATTACK_BOW:
			return 0.30f + AttackSpeed1;

		case PLAYER_ATTACK_ONE_FLASH:
			return 0.4f + AttackSpeed1;
		case PLAYER_ATTACK_RUSH:
			return 0.3f + AttackSpeed1;
		case PLAYER_ATTACK_DEATH_CANNON:
			return 0.2f + AttackSpeed1;

		case PLAYER_SKILL_SLEEP:
			return 0.3f + MagicSpeed2;
		case PLAYER_SKILL_SLEEP_UNI:
			return 0.3f + MagicSpeed2;
		case PLAYER_SKILL_SLEEP_DINO:
			return 0.3f + MagicSpeed2;
		case PLAYER_SKILL_SLEEP_FENRIR:
			return 0.3f + MagicSpeed2;

		case PLAYER_SKILL_LIGHTNING_ORB:
			return 0.4f + MagicSpeed2;
		case PLAYER_SKILL_LIGHTNING_ORB_UNI:
			return 0.25f + MagicSpeed2;
		case PLAYER_SKILL_LIGHTNING_ORB_DINO:
			return 0.25f + MagicSpeed2;
		case PLAYER_SKILL_LIGHTNING_ORB_FENRIR:
			return 0.25f + MagicSpeed2;

		case PLAYER_SKILL_CHAIN_LIGHTNING:
			return 0.25f + MagicSpeed2;
		case PLAYER_SKILL_CHAIN_LIGHTNING_UNI:
			return 0.15f + MagicSpeed2;
		case PLAYER_SKILL_CHAIN_LIGHTNING_DINO:
			return 0.15f + MagicSpeed2;
		case PLAYER_SKILL_CHAIN_LIGHTNING_FENRIR:
			return 0.15f + MagicSpeed2;

		case PLAYER_SKILL_DRAIN_LIFE:
			return 0.25f + MagicSpeed2;
		case PLAYER_SKILL_DRAIN_LIFE_UNI:
			return 0.25f + MagicSpeed2;
		case PLAYER_SKILL_DRAIN_LIFE_DINO:
			return 0.25f + MagicSpeed2;
		case PLAYER_SKILL_DRAIN_LIFE_FENRIR:
			return 0.25f + MagicSpeed2;

		case PLAYER_SKILL_GIGANTICSTORM:
			return 0.55f + MagicSpeed1;
		case PLAYER_SKILL_FLAMESTRIKE:
			return 0.69f + MagicSpeed2;
		case PLAYER_SKILL_LIGHTNING_SHOCK:
			return 0.35f + MagicSpeed2;

		case PLAYER_SKILL_SUMMON:
			return 0.25f;
		case PLAYER_SKILL_SUMMON_UNI:
			return 0.25f;
		case PLAYER_SKILL_SUMMON_DINO:
			return 0.25f;
		case PLAYER_SKILL_SUMMON_FENRIR:
			return 0.25f;

		case PLAYER_SKILL_BLOW_OF_DESTRUCTION:
			return 0.3f;

		case PLAYER_RECOVER_SKILL:
			return 0.33f;
		case PLAYER_SKILL_SWELL_OF_MP:
			return 0.2f;

		case PLAYER_ATTACK_SKILL_FURY_STRIKE:
			return 0.38f;
		case PLAYER_SKILL_VITALITY:
			return 0.34f;
		case PLAYER_SKILL_HELL_START:
			return 0.30f;
		case PLAYER_ATTACK_TELEPORT:
			return 0.28f;
		case PLAYER_ATTACK_RIDE_TELEPORT:
			return 0.3f;
		case PLAYER_ATTACK_DARKHORSE:
			return 0.3f;
		case PLAYER_FENRIR_ATTACK_DARKLORD_TELEPORT:
			return 0.3f;
		case PLAYER_ATTACK_REMOVAL:
			return 0.28f;

		case PLAYER_KOREA_HANDCLAP:
			return 0.3f;
		case PLAYER_POINT_DANCE:
			return 0.4f;

		case PLAYER_CHANGE_UP:
			return 0.049f;

		case PLAYER_RUN_RIDE_HORSE:
		case PLAYER_DARKLORD_WALK:
			{
				mu_float PlaySpeed = 0.33f;

				if (o != nullptr)
				{
					if (g_isCharacterBuff(o, eDeBuff_Freeze))
					{
						PlaySpeed *= 0.5f;
					}
					else if (g_isCharacterBuff(o, eDeBuff_BlowOfDestruction))
					{
						PlaySpeed *= 0.33f;
					}
				}

				return PlaySpeed;
			}
			break;

		case PLAYER_WALK_WAND:
		case PLAYER_RUN_WAND:
			{
				mu_float PlaySpeed = CurrentAction == PLAYER_WALK_WAND ? 0.44f : 0.76f;

				if (o != nullptr)
				{
					if (g_isCharacterBuff(o, eDeBuff_Freeze))
					{
						PlaySpeed *= 0.5f;
					}
					else if (g_isCharacterBuff(o, eDeBuff_BlowOfDestruction))
					{
						PlaySpeed *= 0.33f;
					}
				}

				return PlaySpeed;
			}
			break;

		case PLAYER_WALK_SWIM:
		case PLAYER_RUN_SWIM:
			{
				mu_float PlaySpeed = 0.35f;

				if (o != nullptr)
				{
					if (g_isCharacterBuff(o, eDeBuff_Freeze))
					{
						PlaySpeed *= 0.5f;
					}
					else if (g_isCharacterBuff(o, eDeBuff_BlowOfDestruction))
					{
						PlaySpeed *= 0.33f;
					}
				}

				return PlaySpeed;
			}
			break;

		default:
			{
				if (CurrentAction >= PLAYER_ATTACK_SWORD_RIGHT1 && CurrentAction <= PLAYER_ATTACK_RIDE_CROSSBOW)
					return 0.25f + AttackSpeed1;
				if (CurrentAction >= PLAYER_ATTACK_BOW && CurrentAction <= PLAYER_ATTACK_FLY_CROSSBOW)
					return 0.30f + AttackSpeed1;
				if (CurrentAction >= PLAYER_ATTACK_RIDE_BOW && CurrentAction <= PLAYER_ATTACK_RIDE_CROSSBOW)
					return 0.30f + AttackSpeed1;
				if (CurrentAction >= PLAYER_SKILL_HAND1 && CurrentAction <= PLAYER_SKILL_WEAPON2)
					return 0.29f + MagicSpeed2;
				if (CurrentAction >= PLAYER_ATTACK_BOW_UP && CurrentAction <= PLAYER_ATTACK_RIDE_CROSSBOW_UP)
					return 0.30f + AttackSpeed1;
				if (CurrentAction >= PLAYER_WALK_MALE && CurrentAction <= PLAYER_WALK_CROSSBOW)
				{
					mu_float PlaySpeed = 0.33f;

					if (o != nullptr)
					{
						if (g_isCharacterBuff(o, eDeBuff_Freeze))
						{
							PlaySpeed *= 0.5f;
						}
						else if (g_isCharacterBuff(o, eDeBuff_BlowOfDestruction))
						{
							PlaySpeed *= 0.33f;
						}
					}

					return PlaySpeed;
				}
				if (CurrentAction >= PLAYER_RUN && CurrentAction <= PLAYER_RUN_RIDE_WEAPON)
				{
					mu_float PlaySpeed = 0.34f;

					if (o != nullptr)
					{
						if (g_isCharacterBuff(o, eDeBuff_Freeze))
						{
							PlaySpeed *= 0.5f;
						}
						else if (g_isCharacterBuff(o, eDeBuff_BlowOfDestruction))
						{
							PlaySpeed *= 0.33f;
						}
					}

					return PlaySpeed;
				}
			}
			break;
		}
	}
	else if (CurrentAction == MONSTER01_WALK &&
		o->Flags.Get(OBJECT::eOF_DIRECTIONANIMATION) == true)
	{
		return o->DirectionSpeed;
	}

	return MODELS::Data[Type]._AnimationHeader[CurrentAction].Speed;
}

void SetPlayerStop(const mu_uint32 ThreadIndex, CHARACTER *c)
{
	c->Run = 0;

	OBJECT *o = &c->Object;
	if (c->Object.Type == MODEL_PLAYER)
	{
		if (c->Helper.Type == MODEL_HELPER + 37 &&
			!c->SafeZone)
		{
			if (c->Weapon[0].Type != -1 && c->Weapon[1].Type != -1)
				SetAction(&c->Object, PLAYER_FENRIR_STAND_TWO_SWORD);
			else if (c->Weapon[0].Type != -1 && c->Weapon[1].Type == -1)
				SetAction(&c->Object, PLAYER_FENRIR_STAND_ONE_RIGHT);
			else if (c->Weapon[0].Type == -1 && c->Weapon[1].Type != -1)
				SetAction(&c->Object, PLAYER_FENRIR_STAND_ONE_LEFT);
			else
				SetAction(&c->Object, PLAYER_FENRIR_STAND);
		}
		else if (c->Helper.Type == MODEL_HELPER + 4 &&
			!c->SafeZone)
		{
			if (c->Weapon[0].Type == -1 && c->Weapon[1].Type == -1)
				SetAction(&c->Object, PLAYER_STOP_RIDE_HORSE);
			else
				SetAction(&c->Object, PLAYER_STOP_RIDE_HORSE);
		}
		else if (c->SafeZone &&
			c->Weapon[1].Type == MODEL_HELPER + 5 &&
			!InChaosCastle())
		{
			SetAction(&c->Object, PLAYER_DARKLORD_STAND);
		}
		else if ((c->Helper.Type == MODEL_HELPER + 2 || c->Helper.Type == MODEL_HELPER + 3) && !c->SafeZone)
		{
			if (c->Weapon[0].Type == -1 && c->Weapon[1].Type == -1)
				SetAction(&c->Object, PLAYER_STOP_RIDE);
			else
				SetAction(&c->Object, PLAYER_STOP_RIDE_WEAPON);
		}
		else
		{
			mu_boolean Fly = false;

			if (!(c->Object.SubType == MODEL_CURSEDTEMPLE_ALLIED_PLAYER || c->Object.SubType == MODEL_CURSEDTEMPLE_ILLUSION_PLAYER) &&
				!c->SafeZone &&
				c->Wing.Type != -1)
				Fly = true;

			mu_int32 Index = TERRAIN_INDEX_REPEAT((mu_int32)(c->Object.Position[0] * g_TerrainScaleRev), (mu_int32)(c->Object.Position[1] * g_TerrainScaleRev));

			if (g_CurrentScene == MAIN_SCENE &&
				(GET_WORLD == WD_7ATLANSE ||
					InHellas() ||
					GET_WORLD == WD_67DOPPLEGANGER3) &&
					(TerrainWall[Index] & TW_SAFEZONE) != TW_SAFEZONE) Fly = true;

			if (c->MonsterIndex == 257)
				Fly = true;

			if (Fly)
			{
				if (g_isCharacterBuff((&c->Object), eBuff_CrywolfHeroContracted))
				{
					if (c->Object.CurrentAction != PLAYER_HEALING_FEMALE1)
					{
						SetAction(&c->Object, PLAYER_HEALING_FEMALE1);
#if 0 // FIX the server should handle this, not the client, this allow all kind of hacks, like always, webzen doing bad things
						SendRequestAction(AT_HEALING1, ((mu_uint8)((c->Object.Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
					}
				}
				else
				{
					if (GetEquipedBowType(c) == BOWTYPE_CROSSBOW)
					{
						SetAction(&c->Object, PLAYER_STOP_FLY_CROSSBOW);
					}
					else
					{
						SetAction(&c->Object, PLAYER_STOP_FLY);
					}

					if ((c->Weapon[0].Type >= MODEL_BOW + 8 && c->Weapon[0].Type < MODEL_BOW + 15) ||
						(c->Weapon[0].Type >= MODEL_BOW + 16 && c->Weapon[0].Type < MODEL_BOW + 17) ||
						(c->Weapon[0].Type >= MODEL_BOW + 18 && c->Weapon[0].Type < MODEL_STAFF))
						SetAction(&c->Object, PLAYER_STOP_FLY_CROSSBOW);
					else
						SetAction(&c->Object, PLAYER_STOP_FLY);
				}
			}
			else
			{
				if (g_isCharacterBuff((&c->Object), eBuff_CrywolfHeroContracted))
				{
					if (c->Object.CurrentAction != PLAYER_HEALING_FEMALE1)
					{
						SetAction(&c->Object, PLAYER_HEALING_FEMALE1);
#if 0 // FIX the server should handle this, not the client, this allow all kind of hacks, like always, webzen doing bad things
						SendRequestAction(AT_HEALING1, ((mu_uint8)((c->Object.Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
					}
				}
				else
				{
					if ((c->Weapon[0].Type == -1 && c->Weapon[1].Type == -1) || (c->SafeZone && (InBloodCastle() == false)))
					{
						if (GetBaseClass(c->Class) == GAME::CLASS_ELF)
							SetAction(&c->Object, PLAYER_STOP_FEMALE);
						else if (GetBaseClass(c->Class) == GAME::CLASS_SUMMONER && !InChaosCastle())
							SetAction(&c->Object, PLAYER_STOP_SUMMONER);
						else
							SetAction(&c->Object, PLAYER_STOP_MALE);
					}
					else
					{
						if (c->Weapon[0].Type >= MODEL_SWORD && c->Weapon[0].Type < MODEL_SPEAR)
						{
							if (!ItemAttribute[c->Weapon[0].Type - MODEL_ITEM].TwoHand)
							{
								SetAction(&c->Object, PLAYER_STOP_SWORD);
							}
							else if (c->Weapon[0].Type == MODEL_SWORD + 21 || c->Weapon[0].Type == MODEL_SWORD + 31 || c->Weapon[0].Type == MODEL_SWORD + 23 || c->Weapon[0].Type == MODEL_SWORD + 25)
							{
								SetAction(&c->Object, PLAYER_STOP_TWO_HAND_SWORD_TWO);
							}
							else
							{
								SetAction(&c->Object, PLAYER_STOP_TWO_HAND_SWORD);
							}
						}
						else if (c->Weapon[0].Type == MODEL_SPEAR + 1 || c->Weapon[0].Type == MODEL_SPEAR + 2)
						{
							SetAction(&c->Object, PLAYER_STOP_SPEAR);
						}
						else if (c->Weapon[0].Type >= MODEL_SPEAR && c->Weapon[0].Type < MODEL_BOW)
						{
							if (!ItemAttribute[c->Weapon[0].Type - MODEL_ITEM].TwoHand)
								SetAction(&c->Object, PLAYER_STOP_SWORD);
							else
								SetAction(&c->Object, PLAYER_STOP_SCYTHE);
						}
						else if (c->Weapon[0].Type >= MODEL_STAFF + 14 && c->Weapon[0].Type <= MODEL_STAFF + 20)
						{
							::SetAction(&c->Object, PLAYER_STOP_WAND);
						}
						else if (c->Weapon[0].Type >= MODEL_STAFF && c->Weapon[0].Type < MODEL_SHIELD)
						{
							if (!ItemAttribute[c->Weapon[0].Type - MODEL_ITEM].TwoHand)
								SetAction(&c->Object, PLAYER_STOP_SWORD);
							else
								SetAction(&c->Object, PLAYER_STOP_SCYTHE);
						}
						else if (GetEquipedBowType(c) == BOWTYPE_BOW)
						{
							SetAction(&c->Object, PLAYER_STOP_BOW);
						}
						else if (GetEquipedBowType(c) == BOWTYPE_CROSSBOW)
						{
							SetAction(&c->Object, PLAYER_STOP_CROSSBOW);
						}
						else if ((c->Weapon[1].Type >= MODEL_BOW && c->Weapon[1].Type < MODEL_BOW + 7)
							|| c->Weapon[1].Type == MODEL_BOW + 17
							|| c->Weapon[1].Type == MODEL_BOW + 20
							|| c->Weapon[1].Type == MODEL_BOW + 21
							|| c->Weapon[1].Type == MODEL_BOW + 22
							)
							SetAction(&c->Object, PLAYER_STOP_BOW);
						else if ((c->Weapon[0].Type >= MODEL_BOW + 8 && c->Weapon[0].Type < MODEL_BOW + 15) ||
							(c->Weapon[0].Type >= MODEL_BOW + 16 && c->Weapon[0].Type < MODEL_BOW + 17) ||
							(c->Weapon[0].Type >= MODEL_BOW + 18 && c->Weapon[0].Type < MODEL_STAFF))

							SetAction(&c->Object, PLAYER_STOP_CROSSBOW);
						else
						{
							if (GetBaseClass(c->Class) == GAME::CLASS_ELF)
								SetAction(&c->Object, PLAYER_STOP_FEMALE);
							else if (GetBaseClass(c->Class) == GAME::CLASS_SUMMONER)
								SetAction(&c->Object, PLAYER_STOP_SUMMONER);
							else
								SetAction(&c->Object, PLAYER_STOP_MALE);
						}
					}
				}
			}
		}
	}
	else
	{
		mu_int32 Index = TERRAIN_INDEX_REPEAT((c->PositionX), (c->PositionY));
		if (o->Type == MODEL_MONSTER01 + 32 && (TerrainWall[Index] & TW_SAFEZONE) == TW_SAFEZONE)
			SetAction(&c->Object, MONSTER01_APEAR);
		else
			SetAction(&c->Object, MONSTER01_STOP1);
	}

	if (GetLargeRand(16) == 0)
	{
		if (o->Type != MODEL_PLAYER || (o->SubType >= MODEL_SKELETON1 && o->SubType <= MODEL_SKELETON3))
		{
			MUModel *b = &MODELS::Data[o->Type];

			mu_int32 soundIndex = GetLargeRand(2);
			if (b->_Sounds[soundIndex] != INVALID_SOUND)
			{
				mu_int32 offset = 0;
				if (o->SubType == 9)
				{
					offset = 5;
				}
				PlayObject(b->_Sounds[soundIndex] + offset, o);
			}
		}
		else if (c->Helper.Type == MODEL_HELPER + 37)
		{
			if ((GetLargeRand(3)) == 0)
			{
				PlayObject(SOUND_FENRIR_IDLE_1 + GetLargeRand(2), o);
			}
		}
	}
}

void SetPlayerWalk(const mu_uint32 ThreadIndex, CHARACTER *c)
{
	if (c->SafeZone)
	{
		c->Run = 0;
	}

	ITEM* pItemBoots = &CharacterMachine->Equipment[GAME::EQUIPMENT_BOOTS];
	ITEM* pItemGloves = &CharacterMachine->Equipment[GAME::EQUIPMENT_GLOVES];
	mu_int32 iItemBootsLevel = pItemBoots->Level;
	mu_int32 iItemGlovesLevel = pItemGloves->Level;

	if (c->MonsterIndex >= 534 && c->MonsterIndex <= 539)
	{
		iItemBootsLevel = 0;
		iItemGlovesLevel = 0;
		c->Run = 0;
	}
	else if (!c->SafeZone && c->Run < 40)
	{
		if (GetBaseClass(c->Class) == GAME::CLASS_DARK ||
			GetBaseClass(c->Class) == GAME::CLASS_DARK_LORD ||
			((GET_WORLD != WD_7ATLANSE && !InHellas()
				&& GET_WORLD != WD_67DOPPLEGANGER3
				) && c->BodyPart[BODYPART_BOOTS].Type != -1 && c->BodyPart[BODYPART_BOOTS].Level >= 5)
			|| ((GET_WORLD == WD_7ATLANSE || InHellas()
				|| GET_WORLD == WD_67DOPPLEGANGER3
				) && c->BodyPart[BODYPART_GLOVES].Type != -1 && c->BodyPart[BODYPART_GLOVES].Level >= 5)
			|| ((GET_WORLD != WD_7ATLANSE && !InHellas()
				&& GET_WORLD != WD_67DOPPLEGANGER3
				) && iItemBootsLevel >= 40)
			|| ((GET_WORLD == WD_7ATLANSE || InHellas()
				|| GET_WORLD == WD_67DOPPLEGANGER3
				) && iItemGlovesLevel >= 40)
			|| c->Helper.Type == MODEL_HELPER + 37
			|| c->Object.SubType == MODEL_CURSEDTEMPLE_ALLIED_PLAYER
			|| c->Object.SubType == MODEL_CURSEDTEMPLE_ILLUSION_PLAYER)
		{
			++c->Run;
		}
	}

	OBJECT *o = &c->Object;

	if (c->Object.Type == MODEL_PLAYER)
	{
		if (c->Helper.Type == MODEL_HELPER + 37 && !c->SafeZone)
		{
			if (c->Run < FENRIR_RUN_DELAY)
			{
				SetAction_Fenrir_Walk(c, &c->Object);
			}
			else
			{
				SetAction_Fenrir_Run(c, &c->Object);
			}
		}
		else if (c->Helper.Type == MODEL_HELPER + 4 && !c->SafeZone)
		{
			SetAction(&c->Object, PLAYER_RUN_RIDE_HORSE);
		}
		else if (c->Weapon[1].Type == MODEL_HELPER + 5 && c->SafeZone)
		{
			SetAction(&c->Object, PLAYER_DARKLORD_WALK);
		}
		else if (c->Helper.Type == MODEL_HELPER + 2 && !c->SafeZone)
		{
			if (c->Weapon[0].Type == -1 && c->Weapon[1].Type == -1)
				SetAction(&c->Object, PLAYER_RUN_RIDE);
			else
				SetAction(&c->Object, PLAYER_RUN_RIDE_WEAPON);
		}
		else if (c->Helper.Type == MODEL_HELPER + 3 && !c->SafeZone)
		{
			if (GET_WORLD == WD_8TARKAN || GET_WORLD == WD_10HEAVEN || g_Direction.m_CKanturu.IsMayaScene())
			{
			}
			else
			{
				if (c->Weapon[0].Type == -1 && c->Weapon[1].Type == -1)
					SetAction(&c->Object, PLAYER_RUN_RIDE);
				else
					SetAction(&c->Object, PLAYER_RUN_RIDE_WEAPON);
			}
		}
		else
		{
			if (!(c->Object.SubType == MODEL_CURSEDTEMPLE_ALLIED_PLAYER ||
				c->Object.SubType == MODEL_CURSEDTEMPLE_ILLUSION_PLAYER) &&
				!c->SafeZone && c->Wing.Type != -1)
			{
				if (GetEquipedBowType(c) == BOWTYPE_CROSSBOW)
					SetAction(&c->Object, PLAYER_FLY_CROSSBOW);
				else
					SetAction(&c->Object, PLAYER_FLY);
			}
			else if (!c->SafeZone && (GET_WORLD == WD_7ATLANSE ||
				InHellas() ||
				GET_WORLD == WD_67DOPPLEGANGER3))
			{
				if (c->Run >= 40)
					SetAction(&c->Object, PLAYER_RUN_SWIM);
				else
					SetAction(&c->Object, PLAYER_WALK_SWIM);
			}
			else
			{
				if ((c->Weapon[0].Type == -1 && c->Weapon[1].Type == -1) || (c->SafeZone && (InBloodCastle() == false)))
				{
					if (c->Run >= 40)
						SetAction(&c->Object, PLAYER_RUN);
					else
					{
						if (!IsFemale(c->Class))
							SetAction(&c->Object, PLAYER_WALK_MALE);
						else if (GetBaseClass(c->Class) == GAME::CLASS_SUMMONER && InChaosCastle())
							SetAction(&c->Object, PLAYER_WALK_MALE);
						else
							SetAction(&c->Object, PLAYER_WALK_FEMALE);
					}
				}
				else
				{
					if (c->Run < 40)
					{
						if (c->Weapon[0].Type >= MODEL_SWORD && c->Weapon[0].Type < MODEL_SPEAR)
						{
							if (!ItemAttribute[c->Weapon[0].Type - MODEL_ITEM].TwoHand)
							{
								SetAction(&c->Object, PLAYER_WALK_SWORD);
							}
							else if (c->Weapon[0].Type == MODEL_SWORD + 21 || c->Weapon[0].Type == MODEL_SWORD + 31 || c->Weapon[0].Type == MODEL_SWORD + 23 || c->Weapon[0].Type == MODEL_SWORD + 25)
							{
								SetAction(&c->Object, PLAYER_WALK_TWO_HAND_SWORD_TWO);
							}
							else
							{
								SetAction(&c->Object, PLAYER_WALK_TWO_HAND_SWORD);
							}
						}
						else if (c->Weapon[0].Type >= MODEL_STAFF + 14 && c->Weapon[0].Type <= MODEL_STAFF + 20)
						{
							::SetAction(&c->Object, PLAYER_WALK_WAND);
						}
						else if (c->Weapon[0].Type >= MODEL_STAFF && c->Weapon[0].Type < MODEL_SHIELD)
						{
							if (!ItemAttribute[c->Weapon[0].Type - MODEL_ITEM].TwoHand)
								SetAction(&c->Object, PLAYER_WALK_SWORD);
							else
								SetAction(&c->Object, PLAYER_WALK_SCYTHE);
						}
						else if (c->Weapon[0].Type == MODEL_SPEAR + 1 || c->Weapon[0].Type == MODEL_SPEAR + 2)
							SetAction(&c->Object, PLAYER_WALK_SPEAR);
						else if (c->Weapon[0].Type >= MODEL_SPEAR && c->Weapon[0].Type < MODEL_BOW)
							SetAction(&c->Object, PLAYER_WALK_SCYTHE);
						else if (GetEquipedBowType(c) == BOWTYPE_BOW)
						{
							SetAction(&c->Object, PLAYER_WALK_BOW);
						}
						else if (GetEquipedBowType(c) == BOWTYPE_CROSSBOW)
						{
							SetAction(&c->Object, PLAYER_WALK_CROSSBOW);
						}
						else
						{
							if (!IsFemale(c->Class))
								SetAction(&c->Object, PLAYER_WALK_MALE);
							else
								SetAction(&c->Object, PLAYER_WALK_FEMALE);
						}
					}
					else
					{
						if (c->Weapon[0].Type >= MODEL_SWORD && c->Weapon[0].Type < MODEL_SPEAR)
						{
							if (c->Weapon[1].Type >= MODEL_SWORD && c->Weapon[1].Type < MODEL_SPEAR)
							{
								SetAction(&c->Object, PLAYER_RUN_TWO_SWORD);
							}
							else if (!ItemAttribute[c->Weapon[0].Type - MODEL_ITEM].TwoHand)
							{
								SetAction(&c->Object, PLAYER_RUN_SWORD);
							}
							else if (c->Weapon[0].Type == MODEL_SWORD + 21 || c->Weapon[0].Type == MODEL_SWORD + 31 || c->Weapon[0].Type == MODEL_SWORD + 23 || c->Weapon[0].Type == MODEL_SWORD + 25)
							{
								SetAction(&c->Object, PLAYER_RUN_TWO_HAND_SWORD_TWO);
							}
							else
							{
								SetAction(&c->Object, PLAYER_RUN_TWO_HAND_SWORD);
							}
						}
						else if (c->Weapon[0].Type >= MODEL_STAFF + 14 && c->Weapon[0].Type <= MODEL_STAFF + 20)
						{
							SetAction(&c->Object, PLAYER_RUN_WAND);
						}
						else if (c->Weapon[0].Type >= MODEL_STAFF && c->Weapon[0].Type < MODEL_SHIELD)
						{
							if (!ItemAttribute[c->Weapon[0].Type - MODEL_ITEM].TwoHand)
								SetAction(&c->Object, PLAYER_RUN_SWORD);
							else
								SetAction(&c->Object, PLAYER_RUN_SPEAR);
						}
						else if (c->Weapon[0].Type >= MODEL_SPEAR && c->Weapon[0].Type < MODEL_BOW)
						{
							SetAction(&c->Object, PLAYER_RUN_SPEAR);
						}
						else if (GetEquipedBowType(c) == BOWTYPE_BOW)
						{
							SetAction(&c->Object, PLAYER_RUN_BOW);
						}
						else if (GetEquipedBowType(c) == BOWTYPE_CROSSBOW)
						{
							SetAction(&c->Object, PLAYER_RUN_CROSSBOW);
						}
						else
						{
							SetAction(&c->Object, PLAYER_RUN);
						}
					}
				}
			}
		}
	}
	else
	{
		SetAction(&c->Object, MONSTER01_WALK);
	}

	PlayMonsterSound(ThreadIndex, o);

	if (o->Type == MODEL_MONSTER01 + 27)
		PlayObject(SOUND_BONE2, o);
	else if (GetBaseClass(c->Class) == GAME::CLASS_DARK_LORD && c->Helper.Type == MODEL_HELPER + 4 && !c->SafeZone)
	{
		PlayObject(SOUND_RUN_DARK_HORSE_1 + GetLargeRand(3), o);
	}
	else if (c->Helper.Type == MODEL_HELPER + 37 && !c->SafeZone
		&& (c->Object.CurrentAction >= PLAYER_FENRIR_RUN && c->Object.CurrentAction <= PLAYER_FENRIR_RUN_ONE_LEFT_ELF))
	{
		PlayObject(SOUND_FENRIR_RUN_1 + GetLargeRand(3), o);
	}
	else if (c->Helper.Type == MODEL_HELPER + 37 && !c->SafeZone
		&& (c->Object.CurrentAction >= PLAYER_FENRIR_WALK && c->Object.CurrentAction <= PLAYER_FENRIR_WALK_ONE_LEFT))
	{
		PlayObject(SOUND_FENRIR_RUN_1 + GetLargeRand(2), o);
	}
	else if ((c == Hero&&GetLargeRand(64) == 0) || (c != Hero&&GetLargeRand(16) == 0))
	{
		if (o->Type != MODEL_PLAYER || (o->SubType >= MODEL_SKELETON1 && o->SubType <= MODEL_SKELETON3))
		{
			if (o->SubType >= MODEL_SKELETON1 && o->SubType <= MODEL_SKELETON3)
				PlayObject(SOUND_BONE1, o);

			mu_int32 soundIndex = GetLargeRand(2);
			if (MODELS::Data[o->Type]._Sounds[soundIndex] != INVALID_SOUND)
			{
				mu_int32 offset = 0;

				if (o->SubType == 9)
				{
					offset = 5;
				}

				PlayObject(MODELS::Data[o->Type]._Sounds[soundIndex] + offset, o);
			}
		}
	}
}

void SetPlayerHighBowAttack(const mu_uint32 ThreadIndex, CHARACTER* c)
{
	OBJECT *o = &c->Object;
	if (o->Type == MODEL_PLAYER)
	{
		if ((c->Helper.Type == MODEL_HELPER + 2 || c->Helper.Type == MODEL_HELPER + 3) && !c->SafeZone)
		{
			if (GetEquipedBowType(c) == BOWTYPE_BOW)
			{
				SetAction(&c->Object, PLAYER_ATTACK_RIDE_BOW_UP);
			}
			else if (GetEquipedBowType(c) == BOWTYPE_CROSSBOW)
			{
				SetAction(&c->Object, PLAYER_ATTACK_RIDE_CROSSBOW_UP);
			}
		}
		else
		{
			if (GetEquipedBowType(c) == BOWTYPE_BOW)
			{
				if (c->Wing.Type != -1)
					SetAction(&c->Object, PLAYER_ATTACK_FLY_BOW_UP);
				else
					SetAction(&c->Object, PLAYER_ATTACK_BOW_UP);
			}
			else if (GetEquipedBowType(c) == BOWTYPE_CROSSBOW)
			{
				if (c->Wing.Type != -1)
					SetAction(&c->Object, PLAYER_ATTACK_FLY_CROSSBOW_UP);
				else
					SetAction(&c->Object, PLAYER_ATTACK_CROSSBOW_UP);
			}
		}
	}
	++c->SwordCount;
}

void SetPlayerAttack(const mu_uint32 ThreadIndex, CHARACTER *c)
{
	OBJECT *o = &c->Object;
	if (o->Type == MODEL_PLAYER)
	{
		if (c->Helper.Type == MODEL_HELPER + 37 && !c->SafeZone)
		{
			if (c->Weapon[0].Type >= MODEL_SPEAR && c->Weapon[0].Type < MODEL_SPEAR + 5)
				SetAction(&c->Object, PLAYER_FENRIR_ATTACK_SPEAR);
			else if (GetEquipedBowType(c) == BOWTYPE_BOW)
			{
				SetAction(&c->Object, PLAYER_FENRIR_ATTACK_BOW);
			}
			else if (GetEquipedBowType(c) == BOWTYPE_CROSSBOW)
			{
				SetAction(&c->Object, PLAYER_FENRIR_ATTACK_CROSSBOW);
			}
			else
			{
				if (c->Weapon[0].Type != -1 && c->Weapon[1].Type != -1)
					SetAction(&c->Object, PLAYER_FENRIR_ATTACK_TWO_SWORD);
				else if (c->Weapon[0].Type != -1 && c->Weapon[1].Type == -1)
					SetAction(&c->Object, PLAYER_FENRIR_ATTACK_ONE_SWORD);
				else if (c->Weapon[0].Type == -1 && c->Weapon[1].Type != -1)
					SetAction(&c->Object, PLAYER_FENRIR_ATTACK_ONE_SWORD);
				else
					SetAction(&c->Object, PLAYER_FENRIR_ATTACK);
			}

			if (GetBaseClass(c->Class) == GAME::CLASS_DARK_LORD)
			{
				SetAction(&c->Object, PLAYER_FENRIR_ATTACK_DARKLORD_SWORD);
			}
		}
		else if (c->Helper.Type == MODEL_HELPER + 4 && !c->SafeZone)
		{
			SetAction(&c->Object, PLAYER_ATTACK_RIDE_HORSE_SWORD);
		}
		else if ((c->Helper.Type == MODEL_HELPER + 2 || c->Helper.Type == MODEL_HELPER + 3) && !c->SafeZone)
		{
			if (c->Weapon[0].Type >= MODEL_SPEAR && c->Weapon[0].Type < MODEL_SPEAR + 5)
				SetAction(&c->Object, PLAYER_ATTACK_RIDE_SPEAR);
			else if (c->Weapon[0].Type >= MODEL_SPEAR + 5 && c->Weapon[0].Type < MODEL_BOW)
				SetAction(&c->Object, PLAYER_ATTACK_RIDE_SCYTHE);
			else if (GetEquipedBowType(c) == BOWTYPE_BOW)
			{
				SetAction(&c->Object, PLAYER_ATTACK_RIDE_BOW);
			}
			else if (GetEquipedBowType(c) == BOWTYPE_CROSSBOW)
			{
				SetAction(&c->Object, PLAYER_ATTACK_RIDE_CROSSBOW);
			}
			else
			{
				if (c->Weapon[0].Type == -1)
				{
					SetAction(&c->Object, PLAYER_ATTACK_RIDE_SWORD);
				}
				else
				{
					if (!ItemAttribute[c->Weapon[0].Type - MODEL_ITEM].TwoHand)
						SetAction(&c->Object, PLAYER_ATTACK_RIDE_SWORD);
					else
						SetAction(&c->Object, PLAYER_ATTACK_RIDE_TWO_HAND_SWORD);
				}
			}
		}
		else
		{
			if (c->Weapon[0].Type == -1 && c->Weapon[1].Type == -1)
			{
				SetAction(&c->Object, PLAYER_ATTACK_FIST);
			}
			else
			{
				if (c->Weapon[0].Type >= MODEL_SWORD && c->Weapon[0].Type < MODEL_SPEAR)
				{
					if (!ItemAttribute[c->Weapon[0].Type - MODEL_ITEM].TwoHand)
					{
						if (c->Weapon[1].Type >= MODEL_SWORD && c->Weapon[1].Type < MODEL_SPEAR)
						{
							switch (c->SwordCount % 4)
							{
							case 0:SetAction(&c->Object, PLAYER_ATTACK_SWORD_RIGHT1); break;
							case 1:SetAction(&c->Object, PLAYER_ATTACK_SWORD_LEFT1); break;
							case 2:SetAction(&c->Object, PLAYER_ATTACK_SWORD_RIGHT1 + 1); break;
							case 3:SetAction(&c->Object, PLAYER_ATTACK_SWORD_LEFT1 + 1); break;
							}
						}
						else
							SetAction(&c->Object, PLAYER_ATTACK_SWORD_RIGHT1 + c->SwordCount % 2);
					}
					else if (c->Weapon[0].Type == MODEL_SWORD + 21 || c->Weapon[0].Type == MODEL_SWORD + 31 || c->Weapon[0].Type == MODEL_SWORD + 23 || c->Weapon[0].Type == MODEL_SWORD + 25)
					{
						SetAction(&c->Object, PLAYER_ATTACK_TWO_HAND_SWORD_TWO);
					}
					else
					{
						SetAction(&c->Object, PLAYER_ATTACK_TWO_HAND_SWORD1 + c->SwordCount % 3);
					}
				}
				else if (c->Weapon[1].Type >= MODEL_SWORD && c->Weapon[1].Type < MODEL_SPEAR)
				{
					SetAction(&c->Object, PLAYER_ATTACK_SWORD_LEFT1 + GetLargeRand(2));
				}
				else if (c->Weapon[0].Type >= MODEL_STAFF && c->Weapon[0].Type < MODEL_SHIELD)
				{
					if (!ItemAttribute[c->Weapon[0].Type - MODEL_ITEM].TwoHand)
						SetAction(&c->Object, PLAYER_ATTACK_SWORD_RIGHT1 + GetLargeRand(2));
					else
						SetAction(&c->Object, PLAYER_SKILL_WEAPON1 + GetLargeRand(2));
				}
				else if (c->Weapon[0].Type == MODEL_SPEAR + 1 || c->Weapon[0].Type == MODEL_SPEAR + 2)
					SetAction(&c->Object, PLAYER_ATTACK_SPEAR1);
				else if (c->Weapon[0].Type >= MODEL_SPEAR && c->Weapon[0].Type < MODEL_BOW)
					SetAction(&c->Object, PLAYER_ATTACK_SCYTHE1 + c->SwordCount % 3);
				else if (GetEquipedBowType(c) == BOWTYPE_BOW)
				{
					if (c->Wing.Type != -1)
						SetAction(&c->Object, PLAYER_ATTACK_FLY_BOW);
					else
						SetAction(&c->Object, PLAYER_ATTACK_BOW);
				}
				else if (GetEquipedBowType(c) == BOWTYPE_CROSSBOW)
				{
					if (c->Wing.Type != -1)
						SetAction(&c->Object, PLAYER_ATTACK_FLY_CROSSBOW);
					else
						SetAction(&c->Object, PLAYER_ATTACK_CROSSBOW);
				}
				else
					SetAction(&c->Object, PLAYER_ATTACK_FIST);
			}
		}
	}

	else if (o->Type == 39)
	{
		CreateEffect(ThreadIndex, MODEL_SAW, o->Position, o->Angle, o->Light);
		PlayObject(SOUND_TRAP01, o);
	}
	else if (o->Type == 40)
	{
		SetAction(&c->Object, 1);
		PlayObject(SOUND_TRAP01, o);
	}
	else if (o->Type == 51)
	{
		CreateEffect(ThreadIndex, BITMAP_FIRE + 1, o->Position, o->Angle, o->Light);
		PlayBackground(SOUND_FLAME);
	}
	else
	{
		if (o->Type == MODEL_MONSTER01 + 32)
		{
			mu_int32 Action = GetLargeRand(8);
			if (Action > 2)
				SetAction(&c->Object, MONSTER01_ATTACK1 + GetLargeRand(2));
			else if (Action > 0)
				SetAction(&c->Object, MONSTER01_ATTACK3);
			else
				SetAction(&c->Object, MONSTER01_ATTACK4);
		}
		else
		{
			mu_boolean Success = true;

			if (TheMapProcess().SetCurrentActionMonster(ThreadIndex, c, o) == true)
			{
				Success = false;
			}

			if (Success)
			{
				if (c->SwordCount % 3 == 0)
					SetAction(&c->Object, MONSTER01_ATTACK1);
				else
					SetAction(&c->Object, MONSTER01_ATTACK2);
				++c->SwordCount;
			}
		}
	}

	if (GET_WORLD == WD_73NEW_LOGIN_SCENE)
	{

	}
	else if (c->Object.AnimationFrame == 0.0f)
	{
		PlayMonsterSound(ThreadIndex, o);
		if (o->Type != MODEL_PLAYER || (o->SubType >= MODEL_SKELETON1 && o->SubType <= MODEL_SKELETON3))
		{
			if (o->SubType >= MODEL_SKELETON1 && o->SubType <= MODEL_SKELETON3)
				PlayObject(SOUND_BRANDISH_SWORD01 + GetLargeRand(2), o);

			mu_int32 soundIndex = 2 + GetLargeRand(2);
			if (MODELS::Data[o->Type]._Sounds[soundIndex] != INVALID_SOUND)
			{
				mu_int32 offset = 0;
				if (o->SubType == 9)
				{
					offset = 5;
				}
				PlayObject(MODELS::Data[o->Type]._Sounds[soundIndex] + offset, o);
			}
		}
		else
		{
			if (GetEquipedBowType(c) == BOWTYPE_BOW)
			{
				PlayObject(SOUND_BOW01, o);
			}
			else if (GetEquipedBowType(c) == BOWTYPE_CROSSBOW)
			{
				PlayObject(SOUND_CROSSBOW01, o);
			}
			else if (c->Weapon[0].Type >= MODEL_BOW + 13 && c->Weapon[0].Type < MODEL_BOW + 15)
				PlayObject(SOUND_MAGIC, o);
			else if (c->Weapon[0].Type == MODEL_SWORD + 10 || c->Weapon[0].Type == MODEL_SPEAR)
				PlayObject(SOUND_BRANDISH_SWORD03, o);
			else if (c->Weapon[0].Type != -1 || c->Weapon[1].Type != -1)
				PlayObject(SOUND_BRANDISH_SWORD01 + GetLargeRand(2), o);
		}
	}

	++c->SwordCount;
}

void SetPlayerMagic(const mu_uint32 ThreadIndex, CHARACTER *c)
{
	OBJECT *o = &c->Object;
	if (o->Type == MODEL_PLAYER)
	{
		if ((c->Helper.Type == MODEL_HELPER + 2 || c->Helper.Type == MODEL_HELPER + 3) && !c->SafeZone)
		{
			SetAction(o, PLAYER_RIDE_SKILL);
		}
		else if (c->Helper.Type == MODEL_HELPER + 37 && !c->SafeZone)
		{
			SetAction(o, PLAYER_FENRIR_ATTACK_MAGIC);
		}
		else
		{
			if (IsFemale(c->Class))
				SetAction(o, PLAYER_SKILL_ELF1);
			else
				SetAction(o, PLAYER_SKILL_HAND1 + GetLargeRand(2));
		}
	}
	else
	{
		if (c->SwordCount++ % 3 == 0)
			SetAction(&c->Object, MONSTER01_ATTACK1);
		else
			SetAction(&c->Object, MONSTER01_ATTACK2);
	}
}

void SetPlayerTeleport(const mu_uint32 ThreadIndex, CHARACTER *c)
{
	OBJECT *o = &c->Object;
	if (o->Type == MODEL_PLAYER)
	{
		SetAction(o, PLAYER_SKILL_TELEPORT);
	}
	else
	{
		SetAction(o, MONSTER01_SHOCK);
	}
}

void SetPlayerShock(const mu_uint32 ThreadIndex, CHARACTER *c, mu_int32 Hit)
{
	if (c->Dead) return;
	if (c->Helper.Type == MODEL_HELPER + 2 || c->Helper.Type == MODEL_HELPER + 3) return;
	if (c->Helper.Type == MODEL_HELPER + 4) return;

	OBJECT *o = &c->Object;

	if (o->CurrentAction == PLAYER_ATTACK_SKILL_FURY_STRIKE || o->CurrentAction == PLAYER_SKILL_VITALITY) return;
	if (o->CurrentAction == PLAYER_SKILL_HELL_BEGIN) return;

	if (Hit > 0)
	{
		if (c->Object.Type == MODEL_PLAYER)
		{
			if (c->Helper.Type == MODEL_HELPER + 37)
			{
				SetAction_Fenrir_Damage(c, &c->Object);
				if ((GetLargeRand(3)) == 0)
					PlayObject(SOUND_FENRIR_DAMAGE_1 + GetLargeRand(2), o);
			}
			else
			{
				SetAction(&c->Object, PLAYER_SHOCK);
			}

			c->Movement = false;
		}
		else
		{
			if (o->CurrentAction<MONSTER01_ATTACK1 || o->CurrentAction>MONSTER01_ATTACK2)
				SetAction(&c->Object, MONSTER01_SHOCK);
		}
		if (c->Object.AnimationFrame == 0.0f)
		{
			PlayMonsterSound(ThreadIndex, o);

			if (o->Type != MODEL_PLAYER || (o->SubType >= MODEL_SKELETON1 && o->SubType <= MODEL_SKELETON3))
			{
				if (o->SubType >= MODEL_SKELETON1 && o->SubType <= MODEL_SKELETON3)
					PlayObject(SOUND_BONE1, o);
				else if (o->Type != MODEL_MONSTER01 + 14)
				{
					mu_int32 soundIndex = 2 + GetLargeRand(2);
					if (MODELS::Data[o->Type]._Sounds[soundIndex] != INVALID_SOUND)
					{
						mu_int32 offset = 0;
						if (o->SubType == 9)
						{
							offset = 5;
						}
						PlayObject(MODELS::Data[o->Type]._Sounds[soundIndex] + offset, o);
					}
				}
			}
			else
			{
				if (!IsFemale(c->Class))
				{
					if (GetBaseClass(c->Class) == GAME::CLASS_DARK_LORD && GetLargeRand(5))
						PlayObject(SOUND_DARKLORD_PAIN, o);
					else
						PlayObject(SOUND_HUMAN_SCREAM01 + GetLargeRand(3), o);
				}
				else
					PlayObject(SOUND_FEMALE_SCREAM01 + GetLargeRand(2), o);
			}
		}

		if (o->Type == MODEL_MONSTER01 + 61)
		{
			EVector3 Position;
			for (mu_int32 i = 0; i < 5; ++i)
			{
				if ((GetLargeRand(2)) == 0)
				{
					Position[0] = o->Position[0] + (GetLargeRand(128) - 64);
					Position[1] = o->Position[1];
					Position[2] = o->Position[2] + 200 + (GetLargeRand(50));

					CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, Position, o->Angle, o->Light);
				}
			}
			PlayBackground(SOUND_HIT_GATE);
		}
	}
}

void SetPlayerDie(const mu_uint32 ThreadIndex, CHARACTER *c)
{
	OBJECT *o = &c->Object;

	if (c == Hero)
	{
		CharacterAttribute->Life = 0;
	}

	if (c->Object.Type == MODEL_PLAYER)
	{
		if (o->SubType >= MODEL_SKELETON1 && o->SubType <= MODEL_SKELETON3)
		{
			o->Flags.Clear(OBJECT::eOF_LIVE);
			CreateEffect(ThreadIndex, MODEL_BONE1, o->Position, o->Angle, o->Light);
			for (mu_int32 j = 0; j < 10; ++j)
				CreateEffect(ThreadIndex, MODEL_BONE2, o->Position, o->Angle, o->Light);
			PlayObject(SOUND_BONE2, o);
		}
		else
			SetAction(&c->Object, PLAYER_DIE1);
	}
	else
	{
		switch (o->Type)
		{
		case MODEL_MONSTER01 + 30:
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);
				CreateEffect(ThreadIndex, MODEL_BONE1, o->Position, o->Angle, o->Light);
				for (mu_int32 j = 0; j < 10; ++j)
					CreateEffect(ThreadIndex, MODEL_BONE2, o->Position, o->Angle, o->Light);
				PlayObject(SOUND_BONE2, o);
			}
			break;
		case MODEL_MONSTER01 + 25:
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);
				for (mu_int32 j = 0; j < 8; ++j)
				{
					CreateEffect(ThreadIndex, MODEL_BIG_STONE1, o->Position, o->Angle, o->Light);
					CreateEffect(ThreadIndex, MODEL_BIG_STONE2, o->Position, o->Angle, o->Light);
				}
				PlayObject(SOUND_BONE2, o);
			}
			break;
		case MODEL_MONSTER01 + 110:
		case MODEL_MONSTER01 + 115:
			{
				if (GET_WORLD == WD_39KANTURU_3RD && g_Direction.m_CKanturu.m_iKanturuState == KANTURU_STATE_MAYA_BATTLE)
				{
					FallingMonster(c, o);
					SetAction(&c->Object, MONSTER01_STOP2);
				}
				else
					SetAction(&c->Object, MONSTER01_DIE);
			}
			break;
		case MODEL_MONSTER01 + 118:
		case MODEL_MONSTER01 + 119:
			{
				if (GET_WORLD == WD_39KANTURU_3RD && g_Direction.m_CKanturu.m_iMayaState >= KANTURU_MAYA_DIRECTION_MAYA3)
					SetAction(&c->Object, MONSTER01_STOP2);
				else
					SetAction(&c->Object, MONSTER01_DIE);
			}
			break;
		case MODEL_MONSTER01 + 191:
			{
				if (c->Object.CurrentAction != MONSTER01_APEAR)
				{
					SetAction(&c->Object, MONSTER01_DIE);
				}
			}
			break;
		default:
			SetAction(&c->Object, MONSTER01_DIE);
			break;
		}
	}

	if (c->Object.AnimationFrame == 0.0f)
	{
		PlayMonsterSound(ThreadIndex, o);

		if (InChaosCastle() == true)
		{
			PlayObject(SOUND_CHAOS_MOB_BOOM01 + GetLargeRand(2), o);
		}
		else
		{
			if (o->Type != MODEL_PLAYER || (o->SubType >= MODEL_SKELETON1 && o->SubType <= MODEL_SKELETON3))
			{
				if (MODELS::Data[o->Type]._Sounds[4] != INVALID_SOUND)
				{
					mu_int32 offset = 0;
					if (o->SubType == 9)
					{
						offset = 5;
					}
					PlayObject(MODELS::Data[o->Type]._Sounds[4] + offset, o);
				}
			}
			else
			{
				if (!IsFemale(c->Class))
				{
					if (GetBaseClass(c->Class) == GAME::CLASS_DARK_LORD)
						PlayObject(SOUND_DARKLORD_DEAD, o);
					else
						PlayObject(SOUND_HUMAN_SCREAM04, o);
				}
				else
				{
					PlayObject(SOUND_FEMALE_SCREAM02, o);
				}

				if (c->Helper.Type == MODEL_HELPER + 37)
					PlayObject(SOUND_FENRIR_DEATH, o);
			}
		}

		c->Object.AnimationFrame = 0.001f;
	}
}

void SetPlayerBow(CHARACTER *c)
{
	OBJECT *o = &c->Object;

	if (o->Type != MODEL_PLAYER || GetBaseClass(c->Class) != GAME::CLASS_ELF || c->SafeZone)
		return;

	switch (GetEquipedBowType(c))
	{
	case BOWTYPE_BOW:
		{
			if (c->Helper.Type == MODEL_HELPER + 37)
			{
				SetAction(&c->Object, PLAYER_FENRIR_ATTACK_BOW);
			}
			else if ((c->Helper.Type == MODEL_HELPER + 2) || (c->Helper.Type == MODEL_HELPER + 3))
			{
				SetAction(&c->Object, PLAYER_ATTACK_RIDE_BOW);
			}
			else if (c->Wing.Type != -1)
			{
				SetAction(&c->Object, PLAYER_ATTACK_FLY_BOW);
			}
			else
			{
				SetAction(&c->Object, PLAYER_ATTACK_BOW);
			}
		}
		break;
	case BOWTYPE_CROSSBOW:
		{
			if (c->Helper.Type == MODEL_HELPER + 37 && !c->SafeZone)
			{
				SetAction(&c->Object, PLAYER_FENRIR_ATTACK_CROSSBOW);
			}
			else if ((c->Helper.Type == MODEL_HELPER + 2) || (c->Helper.Type == MODEL_HELPER + 3))
			{
				SetAction(&c->Object, PLAYER_ATTACK_RIDE_CROSSBOW);
			}
			else if (c->Wing.Type != -1)
			{
				SetAction(&c->Object, PLAYER_ATTACK_FLY_CROSSBOW);
			}
			else
			{
				SetAction(&c->Object, PLAYER_ATTACK_CROSSBOW);
			}
		}
		break;
	}
}

void SetPlayerHighBow(CHARACTER* c)
{
	switch (GetEquipedBowType(c))
	{
	case BOWTYPE_BOW:
		{
			if (c->Helper.Type == MODEL_HELPER + 37)
			{
				SetAction(&c->Object, PLAYER_ATTACK_RIDE_BOW_UP);
			}
			else if ((c->Helper.Type == MODEL_HELPER + 2) || (c->Helper.Type == MODEL_HELPER + 3))
			{
				SetAction(&c->Object, PLAYER_ATTACK_RIDE_BOW_UP);
			}
			else if (c->Wing.Type != -1)
			{
				SetAction(&c->Object, PLAYER_ATTACK_FLY_BOW_UP);
			}
			else
			{
				SetAction(&c->Object, PLAYER_ATTACK_BOW_UP);
			}
		}
		break;
	case BOWTYPE_CROSSBOW:
		{
			if (c->Helper.Type == MODEL_HELPER + 37 && !c->SafeZone)
			{
				SetAction(&c->Object, PLAYER_ATTACK_RIDE_CROSSBOW_UP);
			}
			else if ((c->Helper.Type == MODEL_HELPER + 2) || (c->Helper.Type == MODEL_HELPER + 3))
			{
				SetAction(&c->Object, PLAYER_ATTACK_RIDE_CROSSBOW_UP);
			}
			else if (c->Wing.Type != -1)
			{
				SetAction(&c->Object, PLAYER_ATTACK_FLY_CROSSBOW_UP);
			}
			else
			{
				SetAction(&c->Object, PLAYER_ATTACK_CROSSBOW_UP);
			}
		}
		break;
	}
}

void CalcAddPosition(OBJECT *o, mu_float x, mu_float y, mu_float z, EVector3 &Position)
{
	EBone Matrix;
	AngleMatrix(o->Angle, Matrix);
	EVector3 p;
	Vector(x, y, z, p);
	VectorRotate(p, Matrix, Position);
	VectorAdd(Position, o->Position, Position);
}

void AttackEffect(const mu_uint32 ThreadIndex, CHARACTER *c)
{
	OBJECT *o = &c->Object;
	MUModel *b = &MODELS::Data[o->Type];
	mu_int32 i;
	EVector3 Angle, Light;
	EVector3 p, Position;
	mu_float Luminosity = (mu_float)(GetLargeRand(6) + 2)*0.1f;
	Vector(0.0f, 0.0f, 0.0f, p);
	Vector(1.0f, 1.0f, 1.0f, Light);

	if (TheMapProcess().AttackEffectMonster(ThreadIndex, c, o, b) == true)
		return;

	switch (c->MonsterIndex)
	{
	case 162:
	case 164:
	case 166:
	case 168:
	case 170:
	case 172:
	case 426:
		break;

	case 89:
	case 95:
	case 112:
	case 118:
	case 124:
	case 130:
	case 143:
		if ((c->Skill) == AT_SKILL_BOSS)
		{
			if ((GetLargeRand(2)) == 0)
			{
				if (c->AttackTime == 1)
				{
					CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, o->Position, o->Angle, o->Light, 1);
				}
			}
			else
			{
				Vector(o->Position[0] + GetLargeRand(1024) - 512, o->Position[1] + GetLargeRand(1024) - 512, o->Position[2], Position);
				CreateEffect(ThreadIndex, MODEL_FIRE, Position, o->Angle, o->Light);
				PlayBackground(SOUND_METEORITE01);
			}
		}
		break;

	case 68:
		break;
	case 69:
		break;
	case 70:
		if (c->AttackTime == 5)
		{
			if (c->TargetCharacter != -1)
			{
				CHARACTER *tc = &CharactersClient[c->TargetCharacter];
				OBJECT *to = &tc->Object;
				for (mu_int32 i = 0; i<20; ++i)
				{
					CreateEffect(ThreadIndex, BITMAP_BLIZZARD, to->Position, to->Angle, Light);
				}
			}
		}
		break;
	case 301:
	case 71:
	case 74:
		if (c->Object.CurrentAction == MONSTER01_ATTACK1 || c->Object.CurrentAction == MONSTER01_ATTACK2)
		{
			if (c->AttackTime == 5)
			{
				CreateInferno(ThreadIndex, o->Position);
				CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, o->Position, o->Angle, o->Light);
			}
		}
		break;
	case 72:
		if ((c->Skill) == AT_SKILL_BOSS)
		{
			if (c->AttackTime == 14)
			{
				EVector3 Angle = { 0.0f, 0.0f, 0.0f };
				mu_int32 iCount = 36;
				for (mu_int32 i = 0; i < iCount; ++i)
				{
					Angle[0] = (mu_float)(GetLargeRand(360));
					Angle[1] = (mu_float)(GetLargeRand(360));
					Angle[2] = (mu_float)(GetLargeRand(360));
					EVector3 Position;
					VectorCopy(o->Position, Position);
					Position[2] += 100.0f;
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, Position, Position, Angle, 1, nullptr, 60.0f, 0, 0);
				}
			}
		}
		break;
	case 73:
	case 75:
		if (c->Object.CurrentAction == MONSTER01_ATTACK1)
		{
			if (c->AttackTime == 11)
			{
				CreateInferno(ThreadIndex, o->Position);
				CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, o->Position, o->Angle, o->Light);

				for (mu_int32 k = 0; k < 5; ++k)
				{
					Vector(1.0f, 0.5f, 0.0f, Light);
					VectorCopy(o->Angle, Angle);
					Angle[0] += 45.0f;
					VectorCopy(o->Position, Position);
					Position[0] += (mu_float)(GetLargeRand(1001) - 500);
					Position[1] += (mu_float)(GetLargeRand(1001) - 500);
					Position[2] += 500.0f;
					VectorCopy(Position, o->StartPosition);
					CreateEffect(ThreadIndex, MODEL_PIERCING + 1, Position, Angle, Light, 1, o);
				}
			}
		}
		else
		{
			if (c->AttackTime == 13)
			{
				Vector(1.0f, 0.5f, 0.0f, Light);
				Vector(-50.0f, 100.0f, 0.0f, p);
				VectorCopy(o->Angle, Angle);
				Angle[0] += 45.0f;
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(11)], p, Position, true);
				VectorCopy(Position, o->StartPosition);
				CreateEffect(ThreadIndex, MODEL_PIERCING + 1, Position, Angle, Light, 1, o);
				PlayBackground(SOUND_METEORITE01);
			}
			else if (c->AttackTime == 9)
			{
				Vector(1.0f, 0.5f, 0.0f, Light);
				Vector(0.0f, 0.0f, 0.0f, p);
				VectorCopy(o->Angle, Angle);
				Angle[0] += 45.0f;
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(11)], p, Position, true);
			}
		}

		break;
	case 77:
		if ((c->Skill) == AT_SKILL_BOSS)
		{
			if (c->AttackTime == 2 || c->AttackTime == 6)
			{
				EVector3 Angle = { 0.0f, 0.0f, 0.0f };
				mu_int32 iCount = 40;
				for (i = 0; i < iCount; ++i)
				{
					Angle[0] = (mu_float)(GetLargeRand(360));
					Angle[1] = (mu_float)(GetLargeRand(360));
					Angle[2] = (mu_float)(GetLargeRand(360));
					EVector3 Position;
					VectorCopy(o->Position, Position);
					Position[2] += 100.0f;
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, Position, Position, Angle, 3, nullptr, 50.0f, 0, 0);
				}
			}
		}
		break;
	case 63:
	case 61:
		if (c->MonsterIndex == 63)
		{
			if (c->AttackTime == 1)
			{
				CreateInferno(ThreadIndex, o->Position);
				CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, o->Position, o->Angle, o->Light);
			}
			if ((c->Skill) == AT_SKILL_BOSS)
			{
				if (c->MonsterIndex == 63)
				{
					Vector(o->Position[0] + GetLargeRand(800) - 400, o->Position[1] + GetLargeRand(800) - 400, o->Position[2], Position);
					CreateEffect(ThreadIndex, MODEL_SKILL_BLAST, Position, o->Angle, o->Light);
				}
				if (c->AttackTime == 14)
				{
					for (mu_int32 i = 0; i<18; ++i)
					{
						VectorCopy(o->Angle, Angle);
						Angle[2] += i*20.0f;
						CreateEffect(ThreadIndex, MODEL_STAFF + 8, o->Position, Angle, o->Light);
				}
			}
		}
		}
		else
		{
			if (c->AttackTime == 1)
			{
				CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, o->Position, o->Angle, o->Light);
			}
		}
		break;
	case 66:
		if ((c->Skill) == AT_SKILL_BOSS)
		{
			if (c->AttackTime == 1)
			{
				CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, o->Position, o->Angle, o->Light, 1);
			}
		}
		break;
	case 54:
	case 57:
	case 151:
		if (c->AttackTime == 1)
		{
			Vector(60.0f, -110.0f, 0.0f, p);
			b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[0].LinkBone], p, Position, true);
			CreateEffect(ThreadIndex, MODEL_ARROW_BOMB, o->Position, o->Angle, o->Light, 0, o);
			if (c->MonsterIndex == 57)
			{
				EVector3 Angle;
				VectorCopy(o->Angle, Angle);
				Angle[2] += 20.0f;
				CreateEffect(ThreadIndex, MODEL_ARROW_BOMB, o->Position, Angle, o->Light, 0, o);
				Angle[2] -= 40.0f;
				CreateEffect(ThreadIndex, MODEL_ARROW_BOMB, o->Position, Angle, o->Light, 0, o);
			}
		}
		break;
	case 53:
	case 58:
	case 59:
		if (c->AttackTime == 1)
		{
			CreateInferno(ThreadIndex, o->Position);
		}
		if (c->AttackTime == 14)
		{
			if (c->MonsterIndex == 59)
			{
				if ((c->Skill) == AT_SKILL_BOSS)
				{
					for (i = 0; i<18; ++i)
					{
						VectorCopy(o->Angle, Angle);
						Angle[2] += i*20.0f;
						CreateEffect(ThreadIndex, MODEL_STAFF + 8, o->Position, Angle, o->Light);
				}
			}
		}
		}
		break;
	case 49:
		if (c->AttackTime % 5 == 1)
		{
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(63)], p, Position, true);
			CreateEffect(ThreadIndex, BITMAP_BOSS_LASER + 1, Position, o->Angle, o->Light);
		}
		if ((c->Skill) == AT_SKILL_BOSS)
		{
			if (c->AttackTime == 1)
			{
				VectorCopy(o->Angle, Angle); Angle[2] += 20.0f;
				VectorCopy(o->Position, p); p[2] += 50.0f;
				Luminosity = (15 - c->AttackTime)*0.1f;
				Vector(Luminosity*0.3f, Luminosity*0.6f, Luminosity*1.0f, Light);
				for (mu_int32 i = 0; i<9; ++i)
				{
					Angle[2] += 40.0f;
					CreateEffect(ThreadIndex, BITMAP_BOSS_LASER, p, Angle, Light);
				}
			}
		}
		break;
	case 42:
		if ((c->Skill) == AT_SKILL_BOSS)
		{
			if (c->AttackTime == 1)
			{
				Vector(0.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(11)], p, Position, true);
				Vector(o->Angle[0] - 20.0f, o->Angle[1], o->Angle[2] - 30.0f, Angle);
				CreateEffect(ThreadIndex, MODEL_FIRE, Position, Angle, o->Light, 2);
				Vector(o->Angle[0] - 30.0f, o->Angle[1], o->Angle[2], Angle);
				CreateEffect(ThreadIndex, MODEL_FIRE, Position, Angle, o->Light, 2);
				Vector(o->Angle[0] - 20.0f, o->Angle[1], o->Angle[2] + 30.0f, Angle);
				CreateEffect(ThreadIndex, MODEL_FIRE, Position, Angle, o->Light, 2);
				PlayBackground(SOUND_METEORITE01);
			}
			Vector(o->Position[0] + GetLargeRand(1024) - 512, o->Position[1] + GetLargeRand(1024) - 512, o->Position[2], Position);
			CreateEffect(ThreadIndex, MODEL_FIRE, Position, o->Angle, o->Light);
			PlayBackground(SOUND_METEORITE01);
		}
		break;
	case 35:
		if ((c->Skill) == AT_SKILL_BOSS)
		{
			if (c->AttackTime == 1)
			{
				for (mu_int32 i = 0; i<18; ++i)
				{
					Vector(0.0f, 0.0f, i*20.0f, Angle);
					CreateEffect(ThreadIndex, MODEL_FIRE, o->Position, Angle, o->Light, 1, o);
				}
				PlayBackground(SOUND_METEORITE01);
			}
		}
		break;
	case 38:
	case 67:
		if ((c->Skill) == AT_SKILL_BOSS)
		{
			if (c->AttackTime == 1)
			{
				CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, o->Light);
				CreateEffect(ThreadIndex, MODEL_CIRCLE_LIGHT, o->Position, o->Angle, o->Light);
				PlayBackground(SOUND_HELLFIRE);
			}
			Vector(o->Position[0] + GetLargeRand(1024) - 512, o->Position[1] + GetLargeRand(1024) - 512, o->Position[2], Position);
			CreateEffect(ThreadIndex, MODEL_FIRE, Position, o->Angle, o->Light);
			PlayBackground(SOUND_METEORITE01);
		}
		break;
	case 103:
		if ((c->Skill) == AT_SKILL_BOSS)
		{
			Vector(o->Position[0] + GetLargeRand(1024) - 512, o->Position[1] + GetLargeRand(1024) - 512, o->Position[2], Position);
			CreateEffect(ThreadIndex, MODEL_FIRE, Position, o->Angle, o->Light);
			PlayBackground(SOUND_METEORITE01);
		}
		break;
	case 45:
		for (mu_int32 i = 0; i<4; ++i)
		{
			Vector((mu_float)(GetLargeRand(32) - 16), (mu_float)(GetLargeRand(32) - 16), (mu_float)(GetLargeRand(32) - 16), p);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(2)], p, Position, true);
			CreateParticle(ThreadIndex, BITMAP_BUBBLE, Position, o->Angle, Light);
			CreateParticle(ThreadIndex, BITMAP_BLOOD + 1, Position, o->Angle, Light);
		}
		break;
	default:
		break;
	}

	if (c->TargetCharacter >= 0 && c->TargetCharacter<MAX_CHARACTERS_CLIENT)
	{
		CHARACTER *tc = &CharactersClient[c->TargetCharacter];
		OBJECT *to = &tc->Object;

		if (to->BoneTransform == nullptr)
		{
		}
		else if ((c->Skill) == AT_SKILL_ENERGYBALL)
		{
			switch (c->MonsterIndex)
			{
			case 163:
			case 165:
			case 167:
			case 169:
			case 171:
			case 173:
			case 427:
				if (c->Weapon[0].Type == MODEL_BOW + 19)
				{
					if (c->AttackTime == 8)
					{
						CreateArrows(ThreadIndex, c, o, o, 0, 0, 0);
					}
				}
				else if (c->Object.CurrentAction == MONSTER01_ATTACK1)
				{
					if (c->AttackTime == 15)
					{
						CalcAddPosition(o, -20.0f, -90.0f, 100.0f, Position);
						CreateEffect(ThreadIndex, BITMAP_BOSS_LASER, Position, o->Angle, Light, 0, o);
					}
				}
				else if (c->Object.CurrentAction == MONSTER01_ATTACK2)
				{
					if (c->AttackTime == 8)
					{
						if (GetLargeRand(2) == 0)
						{
							CreateEffect(ThreadIndex, MODEL_SKILL_BLAST, to->Position, o->Angle, o->Light, 0, o);
							CreateEffect(ThreadIndex, MODEL_SKILL_BLAST, to->Position, o->Angle, o->Light, 0, o);
						}
						else
						{
							Vector(0.8f, 0.5f, 0.1f, Light);
							CreateEffect(ThreadIndex, MODEL_FIRE, to->Position, o->Angle, Light, 6);
							CreateEffect(ThreadIndex, MODEL_FIRE, to->Position, o->Angle, Light, 6);
						}
					}
				}
				break;

			case 89:
			case 95:
			case 112:
			case 118:
			case 124:
			case 130:
			case 143:
				if (14 == c->AttackTime)
				{
					Vector(0.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(33)], p, Position, true);
					VectorCopy(o->Angle, Angle);
					CreateEffect(ThreadIndex, MODEL_PIERCING + 1, Position, Angle, Light, 1);
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, Position, Angle, 2, to, 50.0f);
				}
				break;

			case 87:
			case 93:
			case 99:
			case 116:
			case 122:
			case 128:
			case 141:
				if (c->AttackTime == 13)
				{
					Vector(1.0f, 1.0f, 1.0f, Light);
					Vector(60.0f, 30.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(6)], p, Position, true);

					Vector(o->Angle[0], o->Angle[1], o->Angle[2], Angle);
					CreateEffect(ThreadIndex, MODEL_FIRE, Position, Angle, o->Light, 5);
				}
				break;

			case 77:
				if (14 == c->AttackTime)
				{
					Vector(0.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(24)], p, Position, true);
					VectorCopy(o->Angle, Angle);
					CreateEffect(ThreadIndex, MODEL_PIERCING + 1, Position, Angle, Light, 1);
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, Position, Angle, 2, to, 50.0f);
				}
				break;
			case 73:
			case 75:
				if (c->Object.CurrentAction == MONSTER01_ATTACK2)
				{
					if (c->AttackTime == 13)
					{
						Vector(1.0f, 0.5f, 0.0f, Light);
						Vector(-50.0f, 100.0f, 0.0f, p);
						VectorCopy(o->Angle, Angle);
						Angle[0] += 45.0f;
						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(11)], p, Position, true);
						CreateEffect(ThreadIndex, MODEL_PIERCING + 1, Position, Angle, Light, 1);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, to->Position, Angle, 2, to, 50.0f);
					}
				}
				break;
			case 69:
				if (c->AttackTime == 1)
				{
					for (mu_int32 i = 0; i < 4; ++i)
					{
						CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, Angle, 7, to, 50.0f);
						CreateJoint(ThreadIndex, BITMAP_FLARE, Position, Position, Angle, 7, to, 50.0f);
					}
				}
				break;
			case 61:
				for (mu_int32 i = 0; i<6; ++i)
				{
					mu_int32 Hand = 0;
					if (i >= 3) Hand = 1;
					b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[Hand].LinkBone], p, Position, true);
					Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, to->Position, Angle, 2, to, 50.0f);
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, to->Position, Angle, 2, to, 10.0f);
				}
				if (c->AttackTime == 1)
					PlayBackground(SOUND_EVIL);
				for (mu_int32 i = 0; i<4; ++i)
				{
					mu_int32 Hand = 0;
					if (i >= 2) Hand = 1;
					b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[Hand].LinkBone], p, Position, true);
					Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_LASER + 1, Position, to->Position, Angle, 0, to, 50.0f);
					CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, o->Light);
				}
				break;
			case 46:
				if (c->AttackTime == 1)
					PlayBackground(SOUND_EVIL);
				for (mu_int32 i = 0; i<4; ++i)
				{
					mu_int32 Hand = 0;
					if (i >= 2) Hand = 1;
					Vector(0.0f, 0.0f, 0.0f, Angle);
					b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[Hand].LinkBone], p, Position, true);
					CreateJoint(ThreadIndex, BITMAP_BLUR + 1, Position, to->Position, Angle, 1, to, 50.0f);
					CreateJoint(ThreadIndex, BITMAP_BLUR + 1, Position, to->Position, Angle, 1, to, 10.0f);
				}
				break;
			case 37:
				if (c->AttackTime == 1)
					PlayBackground(SOUND_EVIL);
				for (mu_int32 i = 0; i<4; ++i)
				{
					mu_int32 Hand = 0;
					if (i >= 2) Hand = 1;
					b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[Hand].LinkBone], p, Position, true);
					Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_LASER + 1, Position, to->Position, Angle, 0, to, 50.0f);
					CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, o->Light);
				}
				break;
			case 66:
				{
					if (c->AttackTime == 1)
						PlayBackground(SOUND_THUNDER01);
					mu_float fAngle = (45.0f - static_cast<mu_float>((static_cast<mu_int64>(c->AttackTime) * 3 + IWorldTime / 10ll) % 90ll)) + 180.0f;
					for (mu_int32 i = 0; i<4; ++i)
					{
						b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[i % 2].LinkBone], p, Position, true);
						Vector(0.0f, 0.0f, fAngle, Angle);
						CreateJoint(ThreadIndex, BITMAP_JOINT_LASER + 1, Position, to->Position, Angle, 1, to, 50.0f);
						CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, o->Light);
						fAngle += 270.0f;
				}
				}
				break;

			default:
				break;
			}
		}
		else if ((c->Skill) == AT_SKILL_THUNDER)
		{
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
					if (c->AttackTime == 1)
						PlayBackground(SOUND_THUNDER01);
					mu_float fAngle = (mu_float)(45.0f - (c->AttackTime * 3 + IWorldTime / 10) % 90) + 180.0f;
					for (mu_int32 i = 0; i<4; ++i)
					{
						b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[i % 2].LinkBone], p, Position, true);
						Vector(0.0f, 0.0f, fAngle, Angle);
						CreateJoint(ThreadIndex, BITMAP_JOINT_LASER + 1, Position, to->Position, Angle, 1, to, 50.0f);
						CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, o->Light);
						fAngle += 270.0f;
					}
				}
				break;

			case 77:
				if (8 <= c->AttackTime)
				{
					Vector(0.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(23)], p, Position, true);
					for (mu_int32 i = 0; i < 4; ++i)
					{
						Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, to->Position, Angle, 0, to, 80.0f);
					}
				}
				break;
			case 37:
				if (c->AttackTime == 1)
					PlayBackground(SOUND_EVIL);
				for (mu_int32 i = 0; i<4; ++i)
				{
					mu_int32 Hand = 0;
					if (i >= 2) Hand = 1;
					b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[Hand].LinkBone], p, Position, true);
					Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_LASER + 1, Position, to->Position, Angle, 0, to, 50.0f);
					CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, o->Light);
				}
				break;
			case 34:
				for (mu_int32 i = 0; i<4; ++i)
				{
					mu_int32 Hand = 0;
					if (i >= 2) Hand = 1;
					b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[Hand].LinkBone], p, Position, true);
					Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, to->Position, Angle, 0, to, 50.0f);
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, to->Position, Angle, 0, to, 10.0f);
					CreateParticle(ThreadIndex, BITMAP_ENERGY, Position, o->Angle, Light);
				}
				break;
			case 48:
				for (mu_int32 i = 0; i<6; ++i)
				{
					mu_int32 Hand = 0;
					if (i >= 3) Hand = 1;
					b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[Hand].LinkBone], p, Position, true);
					Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, to->Position, Angle, 2, to, 50.0f);
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, to->Position, Angle, 2, to, 10.0f);
				}
				break;
			case 39:
				if (o->Type == MODEL_PLAYER)
				{
					Vector(0.0f, 0.0f, 0.0f, p);
				}
				else
				{
					Vector(0.0f, -130.0f, 0.0f, p);
				}
				b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[0].LinkBone], p, Position, true);
				Vector(-60.0f, 0.0f, o->Angle[2], Angle);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, to->Position, Angle, 0, to, 50.0f);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, to->Position, Angle, 0, to, 10.0f);
				CreateParticle(ThreadIndex, BITMAP_ENERGY, Position, o->Angle, Light);
				break;
			case 386:
			case 389:
			case 392:
			case 395:
			case 398:
			case 401:
				{
					Vector(8.0f, 0.0f, 0.0f, Light);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(17)], p, Position, true);
					Vector(-60.0f, 0.0f, o->Angle[2], Angle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, to->Position, Angle, 21, to, 50.0f);
					CreateParticle(ThreadIndex, BITMAP_ENERGY, Position, o->Angle, Light);

					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(41)], p, Position, true);
					Vector(-60.0f, 0.0f, o->Angle[2], Angle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, to->Position, Angle, 21, to, 50.0f);
					CreateParticle(ThreadIndex, BITMAP_ENERGY, Position, o->Angle, Light);
				}
				break;
			default:
				if (b->_BoneNonDummyEnd<=c->Weapon[0].LinkBone) break;
				const mu_int32 boneIndex = c->Weapon[0].LinkBone;

				if (o->Type == MODEL_PLAYER)
				{
					Vector(0.0f, 0.0f, 0.0f, p);
				}
				else
				{
					Vector(0.0f, -130.0f, 0.0f, p);
				}
				b->TransformPosition(ThreadIndex, o->BoneTransform[boneIndex], p, Position, true);
				Vector(-60.0f, 0.0f, o->Angle[2], Angle);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, to->Position, Angle, 0, to, 50.0f);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, to->Position, Angle, 0, to, 10.0f);
				CreateParticle(ThreadIndex, BITMAP_ENERGY, Position, o->Angle, Light);
				break;
			}
		}
	}
}

mu_boolean CheckMonsterSkill(CHARACTER* pCharacter, OBJECT* pObject)
{
	if (pCharacter->MonsterIndex == 364)
	{
		M39Kanturu3rd::MayaSceneMayaAction(pCharacter->MonsterSkill);
		return true;
	}

	mu_int32 iCheckAttackAni = -1;
	for (mu_int32 i = 0; i < MAX_MONSTERSKILL_NUM; ++i)
	{
		if (MonsterSkill[pCharacter->MonsterIndex].Skill_Num[i] == pCharacter->MonsterSkill)
		{
			iCheckAttackAni = i;
			break;
		}
		else
		{
			iCheckAttackAni = -1;
		}
	}

	switch (iCheckAttackAni)
	{
	case 0:
		SetAction(pObject, MONSTER01_ATTACK1);
		break;
	case 1:
		SetAction(pObject, MONSTER01_ATTACK2);
		break;
	case 2:
		SetAction(pObject, MONSTER01_ATTACK3);
		break;
	case 3:
		SetAction(pObject, MONSTER01_ATTACK4);
		break;
	case 4:
		SetAction(pObject, MONSTER01_ATTACK5);
		break;
	case 5:
		SetAction(pObject, MONSTER01_ATTACK5);
		break;
	case 6:
		SetAction(pObject, MONSTER01_ATTACK5);
		break;
	case 7:
		SetAction(pObject, MONSTER01_ATTACK5);
		break;
	case 8:
		SetAction(pObject, MONSTER01_ATTACK5);
		break;
	case 9:
		SetAction(pObject, MONSTER01_ATTACK5);
		break;
	}

	pCharacter->MonsterSkill = -1;

	if (iCheckAttackAni < 0)
	{
		SetAction(pObject, MONSTER01_ATTACK1);
	}

	if (iCheckAttackAni > 4)
		return false;

	return true;
}

mu_boolean CharacterAnimation(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o)
{
	mu_boolean Play;
	MUModel *b = &MODELS::Data[o->Type];

	mu_double PlaySpeed = 0.0f;
	if (b->_AnimationCount > 0)
	{
		PlaySpeed = GetPlaySpeed(o, o->Type, b->CurrentAction(ThreadIndex));
		if (PlaySpeed < 0.0)
			PlaySpeed = 0.0;
		if (c->Change && o->CurrentAction >= MONSTER01_ATTACK1 && o->CurrentAction <= MONSTER01_ATTACK2)
			PlaySpeed *= 1.5;
		if (o->CurrentAction == PLAYER_SKILL_VITALITY && o->AnimationFrame>6.0f)
		{
			PlaySpeed /= 2.0;
		}
		else if ((o->CurrentAction == PLAYER_ATTACK_TELEPORT || o->CurrentAction == PLAYER_ATTACK_RIDE_TELEPORT
			|| o->CurrentAction == PLAYER_FENRIR_ATTACK_DARKLORD_TELEPORT
			) && o->AnimationFrame>5.5f)
		{
			PlaySpeed /= 10.0;
		}
		else if (GetBaseClass(c->Class) == GAME::CLASS_DARK_LORD &&
			(o->CurrentAction == PLAYER_SKILL_FLASH || o->CurrentAction == PLAYER_ATTACK_RIDE_ATTACK_FLASH
				|| o->CurrentAction == PLAYER_FENRIR_ATTACK_DARKLORD_FLASH
				) && (o->AnimationFrame>1.0f && o->AnimationFrame<3.0f)
			)
		{
			if (PartyManager::IsPartyMemberChar(c) == false)
			{
				PlaySpeed /= 2.0;
			}
			else
			{
				PlaySpeed /= 8.0;
			}
		}
		if (o->CurrentAction == PLAYER_SKILL_HELL_BEGIN)
		{
			PlaySpeed /= 2.0;
		}
		if (o->Type != MODEL_PLAYER)
		{
			switch (o->Type)
			{
			case MODEL_MONSTER01 + 64:
				if (o->CurrentAction == MONSTER01_DIE && o->AnimationFrame > 6)
					PlaySpeed *= 4.0;
				break;
			case MODEL_FACE:
			case MODEL_FACE + 1:
			case MODEL_FACE + 2:
			case MODEL_FACE + 3:
			case MODEL_FACE + 4:
			case MODEL_FACE + 5:
				PlaySpeed *= 2.0;
				break;
			}
		}
		if (o->Type == MODEL_MONSTER01 + 87)
		{
			if (o->CurrentAction == MONSTER01_DIE)
				PlaySpeed /= 2.0;
		}
	}

	if (g_isCharacterBuff(o, eDeBuff_Stun) || g_isCharacterBuff(o, eDeBuff_Sleep))
	{
		return false;
	}

	Play = b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, PlaySpeed);
	if (o->CurrentAction == PLAYER_CHANGE_UP)
	{
		if (Play == false)
			SetPlayerStop(ThreadIndex, c);
	}

	if (o->CurrentAction == PLAYER_RECOVER_SKILL)
	{
		if (Play == false)
			SetPlayerStop(ThreadIndex, c);
	}

	if (o->CurrentAction == PLAYER_POINT_DANCE)
	{
		const mu_int32 x = (c->PositionX);
		const mu_int32 y = (c->PositionY);

		EDualQuat *bm = &b->_AnimationData[o->CurrentAction].FrameData[b->CurrentAnimationFrame(ThreadIndex)].BoneData[0];

		const mu_int32 ax = static_cast<mu_int32>(bm->Position[0] / g_TerrainScale);
		const mu_int32 ay = static_cast<mu_int32>(bm->Position[1] / g_TerrainScale);
		const mu_uint32 wall = TerrainWall[(y + ay)*TERRAIN_SIZE + (x + ax)];
		if ((wall&TW_NOMOVE) == TW_NOMOVE)
		{
			SetPlayerStop(ThreadIndex, c);
		}
	}

	return Play;
}

mu_int32 GetHandOfWeapon(OBJECT* o)
{
	mu_int32 Hand = 0;
	if (o->Type == MODEL_PLAYER)
	{
		if (o->CurrentAction == PLAYER_ATTACK_SWORD_LEFT1 || o->CurrentAction == PLAYER_ATTACK_SWORD_LEFT2)
			Hand = 1;
	}

	return (Hand);
}

mu_boolean AttackStage(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_int32 Hand = GetHandOfWeapon(o);

	mu_int32 iSkill = (c->Skill);

	ThreadInfo.LimitAttackTime = 15;
	switch (iSkill)
	{
	case AT_SKILL_BLOW_UP:
	case AT_SKILL_BLOW_UP + 1:
	case AT_SKILL_BLOW_UP + 2:
	case AT_SKILL_BLOW_UP + 3:
	case AT_SKILL_BLOW_UP + 4:
	case AT_SKILL_ONETOONE:
		{
			MUModel *b = &MODELS::Data[o->Type];

			if (c->Weapon[Hand].LinkBone >= b->_BoneNonDummyEnd)
			{
				break;
			}

			if (8 == c->AttackTime)
			{
				if (g_CurrentScene != LOG_IN_SCENE)
					PlayBackground(SOUND_SKILL_SWORD2);
			}

			if (2 <= c->AttackTime && c->AttackTime <= 8)
			{
				for (mu_int32 j = 0; j < 3; ++j)
				{
					EVector3 CurPos;
					VectorCopy(o->Position, CurPos);
					CurPos[2] += 120.0f;
					EVector3 TempPos;
					GetNearRandomPos(CurPos, 300, TempPos);
					mu_float fDistance = 1400.0f;
					TempPos[0] += -fDistance * EMath::Sin(o->Angle[2] * Q_PI / 180.0f);
					TempPos[1] += fDistance * EMath::Cos(o->Angle[2] * Q_PI / 180.0f);
					CreateJoint(ThreadIndex, MODEL_SPEARSKILL, TempPos, TempPos, o->Angle, 2, o, 40.0f);
				}
			}
			if (c->AttackTime <= 8)
			{
				EVector3 Position2 = { 0.0f, 0.0f, 0.0f };
				b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[Hand].LinkBone], Position2, o->m_vPosSword, true);

				mu_float fDistance = 300.0f;
				o->m_vPosSword[0] += fDistance * EMath::Sin(o->Angle[2] * Q_PI / 180.0f);
				o->m_vPosSword[1] += -fDistance * EMath::Cos(o->Angle[2] * Q_PI / 180.0f);
			}
			if (6 <= c->AttackTime && c->AttackTime <= 12)
			{
				EVector3 Position;

				EVector3 Position2 = { 0.0f, 0.0f, 0.0f };

				b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[Hand].LinkBone], Position2, Position, true);

				mu_float fDistance = 100.0f + (mu_float)(c->AttackTime - 8) * 10.0f;
				Position[0] += fDistance * EMath::Sin(o->Angle[2] * Q_PI / 180.0f);
				Position[1] += -fDistance * EMath::Cos(o->Angle[2] * Q_PI / 180.0f);
				EVector3 Light = { 1.0f, 1.0f, 1.0f };
				CreateEffect(ThreadIndex, MODEL_SPEAR, Position, o->Angle, Light, 1, o);
				CreateEffect(ThreadIndex, MODEL_SPEAR, Position, o->Angle, Light, 1, o);

				if (c->TargetCharacter != -1)
				{
					CHARACTER *tc = &CharactersClient[c->TargetCharacter];
					OBJECT *to = &tc->Object;
					if (to->Flags.Get(OBJECT::eOF_LIVE) == true &&
						10 <= c->AttackTime)
					{
						to->m_byHurtByOneToOne = 35;
					}
				}
			}
			if (c->AttackTime >= 12)
			{
				c->AttackTime = ThreadInfo.LimitAttackTime;
			}
		}
		break;
	case AT_SKILL_SPEAR:
		{
			MUModel *b = &MODELS::Data[o->Type];

			EVector3 p;
			if (c->AttackTime == 10)
			{
				PlayBackground(SOUND_RIDINGSPEAR);
			}
			else if (c->AttackTime == 4)
			{
				EVector3 Light = { 1.0f, 1.0f, .5f };
				EVector3 Position2 = { 0.0f, 0.0f, 0.0f };
				b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[Hand].LinkBone], Position2, p, true);
				CreateEffect(ThreadIndex, MODEL_SPEAR + 1, p, o->Angle, Light, c->Weapon[Hand].Type, o);
			}
			else if (8 == c->AttackTime)
			{
				EVector3 Position = o->Position;
				Position[0] += 50.0f * EMath::Sin(o->Angle[2] * Q_PI / 180.0f);
				Position[1] += -50.0f * EMath::Cos(o->Angle[2] * Q_PI / 180.0f);
				Position[2] += 110.0f;
				EVector3 Light = { 1.0f, 1.0f, 1.0f };
				CreateEffect(ThreadIndex, MODEL_SPEAR, Position, o->Angle, Light, 0, o);
				CreateEffect(ThreadIndex, MODEL_SPEAR, Position, o->Angle, Light, 0, o);
			}
			if (13 <= c->AttackTime && c->AttackTime <= 14)
			{
				for (mu_int32 i = 0; i < 3; ++i)
				{
					EVector3 Position = o->Position;
					Position[0] += 145.0f * EMath::Sin(o->Angle[2] * Q_PI / 180.0f);
					Position[1] += -145.0f * EMath::Cos(o->Angle[2] * Q_PI / 180.0f);
					Position[2] += 110.0f;
					EVector3 Light = { .3f, .3f, .3f };
					if (c->AttackTime == 11)
					{
					}
					else
					{
						Position[0] += (GetLargeRand(60) - 30);
						Position[1] += (GetLargeRand(60) - 30);
						Position[2] += (GetLargeRand(60) - 30);
					}
					CreateEffect(ThreadIndex, MODEL_SPEARSKILL, Position, o->Angle, Light, c->Weapon[Hand].Type, o);
				}
			}
		}
		break;

	case AT_SKILL_PIERCING:
		if (o->Type == MODEL_PLAYER && o->CurrentAction >= PLAYER_ATTACK_FIST && o->CurrentAction <= PLAYER_RIDE_SKILL)
		{
			if (o->AnimationFrame >= 5.0f)
			{
				o->PriorAnimationFrame = 4.0f;
				o->AnimationFrame = 5.0f;
			}
		}

		if (c->AttackTime == 3)
		{
			CreateEffect(ThreadIndex, BITMAP_GATHERING, o->Position, o->Angle, o->Light, 0, o);
			PlayObject(SOUND_PIERCING, o);
		}
		ThreadInfo.LimitAttackTime = 5;
		break;

	case AT_SKILL_BLOOD_ATT_UP:
	case AT_SKILL_BLOOD_ATT_UP + 1:
	case AT_SKILL_BLOOD_ATT_UP + 2:
	case AT_SKILL_BLOOD_ATT_UP + 3:
	case AT_SKILL_BLOOD_ATT_UP + 4:
	case AT_SKILL_REDUCEDEFENSE: 
		if (o->Type == MODEL_PLAYER)
		{
			SetAction(o, PLAYER_ATTACK_SKILL_WHEEL);

			if (c->AttackTime >= 1 && c->AttackTime <= 2)
			{
				EVector3 Angle;
				Vector(1.0f, 0.0f, 0.0f, Angle);
				CreateEffect(ThreadIndex, BITMAP_GATHERING, o->Position, o->Angle, o->Light, 1, o);
			}

			if (o->AnimationFrame >= 3.0f)
			{
				o->PKKey = getTargetCharacterKey(c, SelectedCharacter);

				PlayBackground(SOUND_SKILL_SWORD3);

				if (iSkill == AT_SKILL_BLOOD_ATT_UP + 4)
				{
					CreateEffect(ThreadIndex, BITMAP_SWORD_FORCE, o->Position, o->Angle, o->Light, 1, o, o->PKKey, FindHotKey(iSkill));
				}
				else
				{
					CreateEffect(ThreadIndex, BITMAP_SWORD_FORCE, o->Position, o->Angle, o->Light, 0, o, o->PKKey, FindHotKey(iSkill));
				}

				o->Flags.Set(OBJECT::eOF_SECONDANIMATION);
				c->AttackTime = 15;
			}
		}
		ThreadInfo.LimitAttackTime = 15;
		break;

	case AT_SKILL_POWER_SLASH_UP:
	case AT_SKILL_POWER_SLASH_UP + 1:
	case AT_SKILL_POWER_SLASH_UP + 2:
	case AT_SKILL_POWER_SLASH_UP + 3:
	case AT_SKILL_POWER_SLASH_UP + 4:
	case AT_SKILL_ICE_BLADE:
		if (o->Type == MODEL_PLAYER && o->CurrentAction == PLAYER_ATTACK_TWO_HAND_SWORD_TWO)
		{
			EVector3 Angle;

			VectorCopy(o->Angle, Angle);

			Angle[2] -= 40.0f;
			CreateEffect(ThreadIndex, MODEL_MAGIC2, o->Position, Angle, o->Light, 2, o);
			Angle[2] += 20.0f;
			CreateEffect(ThreadIndex, MODEL_MAGIC2, o->Position, Angle, o->Light, 2, o);
			Angle[2] += 20.0f;
			CreateEffect(ThreadIndex, MODEL_MAGIC2, o->Position, Angle, o->Light, 2, o);
			Angle[2] += 20.0f;
			CreateEffect(ThreadIndex, MODEL_MAGIC2, o->Position, Angle, o->Light, 2, o);
			Angle[2] += 20.0f;
			CreateEffect(ThreadIndex, MODEL_MAGIC2, o->Position, Angle, o->Light, 2, o);

			PlayBackground(SOUND_SKILL_SWORD3);

			c->AttackTime = 15;
		}
		ThreadInfo.LimitAttackTime = 15;
		break;

	case AT_SKILL_BLAST_HELL:
		if (o->AnimationFrame >= 14.0f && o->Type == MODEL_PLAYER && o->CurrentAction == PLAYER_SKILL_HELL_START)
		{
			c->AttackTime = 15;
		}
		break;
	case AT_SKILL_LIFE_UP:
	case AT_SKILL_LIFE_UP + 1:
	case AT_SKILL_LIFE_UP + 2:
	case AT_SKILL_LIFE_UP + 3:
	case AT_SKILL_LIFE_UP + 4:
	case AT_SKILL_VITALITY:
		if (c->AttackTime>9 && o->Type == MODEL_PLAYER && o->CurrentAction == PLAYER_SKILL_VITALITY)
		{
			c->AttackTime = 15;
		}
		break;
	case AT_SKILL_IMPROVE_AG:
		if (o->AnimationFrame >= 5.0f && ((o->Type == MODEL_PLAYER && o->CurrentAction >= PLAYER_ATTACK_FIST && o->CurrentAction <= PLAYER_RIDE_SKILL) ||
			((o->Type >= MODEL_MONSTER01 && o->Type<MODEL_MONSTER_END) && o->CurrentAction >= MONSTER01_ATTACK1 && o->CurrentAction <= MONSTER01_ATTACK2)))
		{
			c->AttackTime = 15;
		}
		break;
	case AT_SKILL_STRONG_PIER:
		if (o->AnimationFrame >= 3.0f && o->Type == MODEL_PLAYER && (o->CurrentAction == PLAYER_ATTACK_STRIKE || o->CurrentAction == PLAYER_ATTACK_RIDE_STRIKE
			|| o->CurrentAction == PLAYER_FENRIR_ATTACK_DARKLORD_STRIKE
			))
		{
			c->AttackTime = 15;
		}
		break;
	case AT_SKILL_FIRE_BUST_UP:
	case AT_SKILL_FIRE_BUST_UP + 1:
	case AT_SKILL_FIRE_BUST_UP + 2:
	case AT_SKILL_FIRE_BUST_UP + 3:
	case AT_SKILL_FIRE_BUST_UP + 4:
	case    AT_SKILL_LONGPIER_ATTACK:
		if (o->AnimationFrame >= 3.0f && o->Type == MODEL_PLAYER && (o->CurrentAction == PLAYER_ATTACK_STRIKE || o->CurrentAction == PLAYER_ATTACK_RIDE_STRIKE
			|| o->CurrentAction == PLAYER_FENRIR_ATTACK_DARKLORD_STRIKE
			))
		{
			c->AttackTime = 15;
		}
		break;
	case    AT_SKILL_THUNDER_STRIKE:
		if (o->AnimationFrame >= 5.5f && o->Type == MODEL_PLAYER && (o->CurrentAction == PLAYER_SKILL_FLASH || o->CurrentAction == PLAYER_ATTACK_RIDE_ATTACK_FLASH
			|| o->CurrentAction == PLAYER_FENRIR_ATTACK_DARKLORD_FLASH
			))
		{
			c->AttackTime = 15;
		}
		else
		{
			c->AttackTime = 10;
		}
		break;
	case AT_SKILL_ASHAKE_UP:
	case AT_SKILL_ASHAKE_UP + 1:
	case AT_SKILL_ASHAKE_UP + 2:
	case AT_SKILL_ASHAKE_UP + 3:
	case AT_SKILL_ASHAKE_UP + 4:
	case AT_SKILL_DARK_HORSE:
		if (o->AnimationFrame >= 5.0f && o->Type == MODEL_PLAYER && o->CurrentAction == PLAYER_ATTACK_DARKHORSE)
		{
			c->AttackTime = 15;
		}
		break;
	case AT_SKILL_PARTY_TELEPORT:
		if (c->AttackTime>5 && o->Type == MODEL_PLAYER && (o->CurrentAction == PLAYER_ATTACK_TELEPORT || o->CurrentAction == PLAYER_ATTACK_RIDE_TELEPORT
			|| o->CurrentAction == PLAYER_FENRIR_ATTACK_DARKLORD_TELEPORT
			))
		{
			c->AttackTime = 15;
		}
		break;
	case AT_SKILL_RIDER:
		if (o->AnimationFrame >= 5.0f && o->Type == MODEL_PLAYER && (o->CurrentAction == PLAYER_SKILL_RIDER || o->CurrentAction == PLAYER_SKILL_RIDER_FLY))
		{
			c->AttackTime = 15;
		}
		break;

	case AT_SKILL_STUN:
	case AT_SKILL_MANA:
		break;
	case AT_SKILL_INVISIBLE:
		c->AttackTime = 15;
		break;
	case AT_SKILL_REMOVAL_STUN:
	case AT_SKILL_REMOVAL_INVISIBLE:
		c->AttackTime = 15;
		break;
	case AT_SKILL_REMOVAL_BUFF:
		if (o->AnimationFrame >= 3.5f)
		{
			c->AttackTime = 15;
		}
		break;

	case AT_SKILL_RUSH:
		if (o->AnimationFrame>5.0f)
		{
			c->AttackTime = 15;
		}

		{
			EVector3 Position;
			EVector3	Angle;

			VectorCopy(o->Position, Position);
			Position[0] += GetLargeRand(30) - 15.0f;
			Position[1] += GetLargeRand(30) - 15.0f;
			Position[2] += 20.0f;
			for (mu_int32 i = 0; i<4; ++i)
			{
				Vector((mu_float)(GetLargeRand(60) + 60 + 90), 0.0f, o->Angle[2], Angle);
				CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, Position, Position, Angle);
				CreateParticle(ThreadIndex, BITMAP_FIRE, Position, Angle, o->Light, 2, 1.5f);
			}
		}
		break;
	case AT_SKILL_ONEFLASH:
		if (o->AnimationFrame>5.0f)
		{
			CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 23, nullptr, 40.0f, 0);
			CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 23, nullptr, 40.0f, 1);
			CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 23, nullptr, 40.0f, 4);
			c->AttackTime = 15;

			PlayBackground(SOUND_BCS_ONE_FLASH);
		}
		else if (o->AnimationFrame>2.3f && o->AnimationFrame<2.6f)
		{
			CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 23, nullptr, 40.0f, 2);
			CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 23, nullptr, 40.0f, 3);

			PlayBackground(SOUND_BCS_ONE_FLASH);
		}
		ThreadInfo.LimitAttackTime = 15;
		break;

	case AT_SKILL_SPACE_SPLIT:
		if (o->AnimationFrame >= 3.0f
			&& o->Type == MODEL_PLAYER
			&& (o->CurrentAction == PLAYER_ATTACK_STRIKE || o->CurrentAction == PLAYER_ATTACK_RIDE_STRIKE || o->CurrentAction == PLAYER_FENRIR_ATTACK_DARKLORD_STRIKE)
			)
		{
			c->AttackTime = 15;
		}
		break;

	case AT_SKILL_DEATH_CANNON:
		if (o->AnimationFrame >= 3.0f && o->Type == MODEL_PLAYER && o->CurrentAction == PLAYER_ATTACK_DEATH_CANNON)
		{
			c->AttackTime = 15;
		}
		break;
	case AT_SKILL_FLAME_STRIKE:
		{
			c->AttackTime = 15;
		}
		break;
	case AT_SKILL_GIGANTIC_STORM:
		if (o->AnimationFrame>7.0f)
		{
			c->AttackTime = 15;
		}
		break;
	case AT_SKILL_LIGHTNING_SHOCK_UP:
	case AT_SKILL_LIGHTNING_SHOCK_UP + 1:
	case AT_SKILL_LIGHTNING_SHOCK_UP + 2:
	case AT_SKILL_LIGHTNING_SHOCK_UP + 3:
	case AT_SKILL_LIGHTNING_SHOCK_UP + 4:
	case AT_SKILL_LIGHTNING_SHOCK:
		{
			c->AttackTime = 15;
		}
		break;
	case AT_SKILL_BLOW_OF_DESTRUCTION:
		{
			c->AttackTime = 15;
		}
		break;
	default:
		if (o->AnimationFrame >= 1.0f && o->Type == MODEL_PLAYER && o->CurrentAction == PLAYER_ATTACK_SKILL_FURY_STRIKE)
		{
			c->AttackTime = 15;
		}
		else if (o->AnimationFrame >= 5.0f && ((o->Type == MODEL_PLAYER && o->CurrentAction >= PLAYER_ATTACK_FIST && o->CurrentAction <= PLAYER_RIDE_SKILL) ||
			((o->Type >= MODEL_MONSTER01 && o->Type<MODEL_MONSTER_END) && o->CurrentAction >= MONSTER01_ATTACK1 && o->CurrentAction <= MONSTER01_ATTACK2)))
		{
			mu_int32 LeftType = CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Type;

			if (c->AttackTime >= 1 && LeftType == ITEM_BOW + 21 && o->Type == MODEL_PLAYER)
			{
				for (mu_int32 i = 0; i < 20; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_SPARK + 1, o->Position, o->Angle, o->Light, 12, 2.0f);
				}
			}

			c->AttackTime = 15;
		}

		break;
	}
	return true;
}

void SetCharacterScale(CHARACTER *c)
{
	if (c->Change)
		return;

	if (c->BodyPart[BODYPART_HELM].Type == MODEL_HELM ||
		c->BodyPart[BODYPART_HELM].Type == MODEL_HELM + 2 ||
		(c->BodyPart[BODYPART_HELM].Type >= MODEL_HELM + 10 && c->BodyPart[BODYPART_HELM].Type <= MODEL_HELM + 13))
	{
		c->BodyPart[BODYPART_HEAD].Type = MODEL_BODY_HELM + GetSkinModelIndex(c->Class);
	}
	else
	{
		c->BodyPart[BODYPART_HEAD].Type = -1;
	}

	if (g_CurrentScene == CHARACTER_SCENE)
	{
		if (c->Skin == 0)
		{
			switch (GetBaseClass(c->Class))
			{
			case GAME::CLASS_WIZARD:			c->Object.Scale = 1.2f; break;
			case GAME::CLASS_KNIGHT:			c->Object.Scale = 1.2f; break;
			case GAME::CLASS_ELF:				c->Object.Scale = 1.2f; break;
			case GAME::CLASS_DARK:			c->Object.Scale = 1.2f; break;
			case GAME::CLASS_DARK_LORD:		c->Object.Scale = 1.2f; break;
			case GAME::CLASS_SUMMONER:		c->Object.Scale = 1.2f; break;
			}
		}
		else
		{
			switch (GetBaseClass(c->Class))
			{
			case GAME::CLASS_WIZARD:			c->Object.Scale = 1.2f; break;
			case GAME::CLASS_KNIGHT:			c->Object.Scale = 1.2f; break;
			case GAME::CLASS_ELF:				c->Object.Scale = 1.2f; break;
			case GAME::CLASS_DARK:			c->Object.Scale = 1.2f; break;
			case GAME::CLASS_DARK_LORD:		c->Object.Scale = 1.2f; break;
			case GAME::CLASS_SUMMONER:		c->Object.Scale = 1.2f; break;
			}
		}
	}
	else
	{
		if (c->Skin == 0)
		{
			switch (GetBaseClass(c->Class))
			{
			case GAME::CLASS_WIZARD:			c->Object.Scale = 0.9f; break;
			case GAME::CLASS_KNIGHT:			c->Object.Scale = 0.9f; break;
			case GAME::CLASS_ELF:				c->Object.Scale = 0.88f; break;
			case GAME::CLASS_DARK:			c->Object.Scale = 0.95f; break;
			case GAME::CLASS_DARK_LORD:		c->Object.Scale = 0.92f; break;
			case GAME::CLASS_SUMMONER:		c->Object.Scale = 0.90f; break;
			}
		}
		else
		{
			switch (GetBaseClass(c->Class))
			{
			case GAME::CLASS_WIZARD:			c->Object.Scale = 0.93f; break;
			case GAME::CLASS_KNIGHT:			c->Object.Scale = 0.93f; break;
			case GAME::CLASS_ELF:				c->Object.Scale = 0.86f; break;
			case GAME::CLASS_DARK:			c->Object.Scale = 0.95f; break;
			case GAME::CLASS_DARK_LORD:		c->Object.Scale = 0.92f; break;
			case GAME::CLASS_SUMMONER:		c->Object.Scale = 0.90f; break;
			}
		}
	}
}

void SetCharacterClass(const mu_uint32 ThreadIndex, CHARACTER *c)
{
	if (c->Object.Type != MODEL_PLAYER)
	{
		return;
	}

	ITEM *p = CharacterMachine->Equipment;

	if (p[GAME::EQUIPMENT_WEAPON_RIGHT].Type == -1)
	{
		c->Weapon[0].Type = -1;
	}
	else
	{
		c->Weapon[0].Type = p[GAME::EQUIPMENT_WEAPON_RIGHT].Type + MODEL_ITEM;
	}

	if (p[GAME::EQUIPMENT_WEAPON_LEFT].Type == -1)
	{
		c->Weapon[1].Type = -1;
	}
	else
	{
		c->Weapon[1].Type = p[GAME::EQUIPMENT_WEAPON_LEFT].Type + MODEL_ITEM;
	}

	if (p[GAME::EQUIPMENT_WING].Type == -1)
	{
		c->Wing.Type = -1;
	}
	else
	{
		c->Wing.Type = p[GAME::EQUIPMENT_WING].Type + MODEL_ITEM;
	}

	if (p[GAME::EQUIPMENT_HELPER].Type == -1)
	{
		c->Helper.Type = -1;
	}
	else
	{
		c->Helper.Type = p[GAME::EQUIPMENT_HELPER].Type + MODEL_ITEM;
	}

	c->Weapon[0].Level = p[GAME::EQUIPMENT_WEAPON_RIGHT].Level;
	c->Weapon[1].Level = p[GAME::EQUIPMENT_WEAPON_LEFT].Level;
	c->Weapon[0].ExcOption = p[GAME::EQUIPMENT_WEAPON_RIGHT].ExcOption;
	c->Weapon[1].ExcOption = p[GAME::EQUIPMENT_WEAPON_LEFT].ExcOption;
	c->Wing.Level = p[GAME::EQUIPMENT_WING].Level;
	c->Helper.Level = p[GAME::EQUIPMENT_HELPER].Level;

	mu_boolean Success = true;

	if (InChaosCastle() == true)
		Success = false;

	if (c->Object.CurrentAction >= PLAYER_SIT1 && c->Object.CurrentAction <= PLAYER_POSE_FEMALE1)
	{
		Success = false;
	}
	if (c->Object.CurrentAction >= PLAYER_ATTACK_FIST && c->Object.CurrentAction <= PLAYER_RIDE_SKILL)
	{
		Success = false;
	}

	if (Success)
	{
		SetPlayerStop(ThreadIndex, c);
	}

	if (p[GAME::EQUIPMENT_HELM].Type == -1)
	{
		c->BodyPart[BODYPART_HELM].Type = MODEL_BODY_HELM + GetSkinModelIndex(c->Class);
	}
	else
	{
		c->BodyPart[BODYPART_HELM].Type = p[GAME::EQUIPMENT_HELM].Type + MODEL_ITEM;
	}

	if (p[GAME::EQUIPMENT_ARMOR].Type == -1)
	{
		c->BodyPart[BODYPART_ARMOR].Type = MODEL_BODY_ARMOR + GetSkinModelIndex(c->Class);
	}
	else
	{
		c->BodyPart[BODYPART_ARMOR].Type = p[GAME::EQUIPMENT_ARMOR].Type + MODEL_ITEM;
	}

	if (p[GAME::EQUIPMENT_PANTS].Type == -1)
	{
		c->BodyPart[BODYPART_PANTS].Type = MODEL_BODY_PANTS + GetSkinModelIndex(c->Class);
	}
	else
	{
		c->BodyPart[BODYPART_PANTS].Type = p[GAME::EQUIPMENT_PANTS].Type + MODEL_ITEM;
	}

	if (p[GAME::EQUIPMENT_GLOVES].Type == -1)
	{
		c->BodyPart[BODYPART_GLOVES].Type = MODEL_BODY_GLOVES + GetSkinModelIndex(c->Class);
	}
	else
	{
		c->BodyPart[BODYPART_GLOVES].Type = p[GAME::EQUIPMENT_GLOVES].Type + MODEL_ITEM;
	}

	if (p[GAME::EQUIPMENT_BOOTS].Type == -1)
	{
		c->BodyPart[BODYPART_BOOTS].Type = MODEL_BODY_BOOTS + GetSkinModelIndex(c->Class);
	}
	else
	{
		c->BodyPart[BODYPART_BOOTS].Type = p[GAME::EQUIPMENT_BOOTS].Type + MODEL_ITEM;
	}

	c->BodyPart[BODYPART_HELM].Level = p[GAME::EQUIPMENT_HELM].Level;
	c->BodyPart[BODYPART_ARMOR].Level = p[GAME::EQUIPMENT_ARMOR].Level;
	c->BodyPart[BODYPART_PANTS].Level = p[GAME::EQUIPMENT_PANTS].Level;
	c->BodyPart[BODYPART_GLOVES].Level = p[GAME::EQUIPMENT_GLOVES].Level;
	c->BodyPart[BODYPART_BOOTS].Level = p[GAME::EQUIPMENT_BOOTS].Level;
	c->BodyPart[BODYPART_HELM].ExcOption = p[GAME::EQUIPMENT_HELM].ExcOption;
	c->BodyPart[BODYPART_ARMOR].ExcOption = p[GAME::EQUIPMENT_ARMOR].ExcOption;
	c->BodyPart[BODYPART_PANTS].ExcOption = p[GAME::EQUIPMENT_PANTS].ExcOption;
	c->BodyPart[BODYPART_GLOVES].ExcOption = p[GAME::EQUIPMENT_GLOVES].ExcOption;
	c->BodyPart[BODYPART_BOOTS].ExcOption = p[GAME::EQUIPMENT_BOOTS].ExcOption;
	c->BodyPart[BODYPART_HELM].SetOption = p[GAME::EQUIPMENT_HELM].SetOption;
	c->BodyPart[BODYPART_ARMOR].SetOption = p[GAME::EQUIPMENT_ARMOR].SetOption;
	c->BodyPart[BODYPART_PANTS].SetOption = p[GAME::EQUIPMENT_PANTS].SetOption;
	c->BodyPart[BODYPART_GLOVES].SetOption = p[GAME::EQUIPMENT_GLOVES].SetOption;
	c->BodyPart[BODYPART_BOOTS].SetOption = p[GAME::EQUIPMENT_BOOTS].SetOption;

	ChangeChaosCastleUnit(c);

	SetCharacterScale(c);

	if (c == Hero)
	{
		CheckFullSet(ThreadIndex, Hero);
	}

	CharacterMachine->CalculateAll();
}

mu_boolean CheckFullSet(const mu_uint32 ThreadIndex, CHARACTER *c)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_int32 tmpLevel = 10;
	mu_boolean Success = true;
	mu_int32  start = BODYPART_BOOTS, end = BODYPART_HELM;

	ThreadInfo.EquipmentLevelSet = 0;

	mu_boolean bHero = false;

	if (c == Hero)
	{
		bHero = true;
		start = GAME::EQUIPMENT_BOOTS;

		if (GetBaseClass(c->Class) == GAME::CLASS_DARK)
		{
			end = GAME::EQUIPMENT_ARMOR;
		}
		else
		{
			end = GAME::EQUIPMENT_HELM;
		}
	}
	else
	{
		if (GetBaseClass(c->Class) == GAME::CLASS_DARK)
		{
			end = BODYPART_ARMOR;
		}
	}

	if (bHero == true)
	{
		for (mu_int32 i = start; i >= end; --i)
		{
			if (CharacterMachine->Equipment[i].Type == -1)
			{
				Success = false;
				break;
			}
		}

		if (Success)
		{
			mu_int32 Type = CharacterMachine->Equipment[GAME::EQUIPMENT_BOOTS].Type % MAX_ITEM_INDEX;
			tmpLevel = CharacterMachine->Equipment[GAME::EQUIPMENT_BOOTS].Level;

			for (mu_int32 i = start; i >= end; --i)
			{
				mu_int32 Level = CharacterMachine->Equipment[i].Level;
				if (Level < 9)
				{
					ThreadInfo.EquipmentLevelSet = 0;
					Success = false;
					break;
				}
				if (Type != (CharacterMachine->Equipment[i].Type % MAX_ITEM_INDEX))
				{
					ThreadInfo.EquipmentLevelSet = 0;
					Success = false;
					break;
				}

				if (Level >= 9 && tmpLevel >= Level)
				{
					tmpLevel = Level;
					ThreadInfo.EquipmentLevelSet = Level;
				}
			}
		}

		g_AddDefense = true;

		if (GetBaseClass(c->Class) == GAME::CLASS_DARK && Success)
		{
			if (CharacterMachine->Equipment[GAME::EQUIPMENT_ARMOR].Type != ITEM_ARMOR + 15
				&& CharacterMachine->Equipment[GAME::EQUIPMENT_ARMOR].Type != ITEM_ARMOR + 20
				&& CharacterMachine->Equipment[GAME::EQUIPMENT_ARMOR].Type != ITEM_ARMOR + 23
				&& CharacterMachine->Equipment[GAME::EQUIPMENT_ARMOR].Type != ITEM_ARMOR + 32
				&& CharacterMachine->Equipment[GAME::EQUIPMENT_ARMOR].Type != ITEM_ARMOR + 37
				&& CharacterMachine->Equipment[GAME::EQUIPMENT_ARMOR].Type != ITEM_ARMOR + 47
				&& CharacterMachine->Equipment[GAME::EQUIPMENT_ARMOR].Type != ITEM_ARMOR + 48)
			{
				g_AddDefense = false;
			}
		}
	}
	else
	{
		for (mu_int32 i = BODYPART_BOOTS; i >= end; --i)
		{
			if (c->BodyPart[i].Type == -1)
			{
				Success = false;
				break;
			}
		}

		if (Success)
		{
			mu_int32 Type = (c->BodyPart[BODYPART_BOOTS].Type - MODEL_ITEM) % MAX_ITEM_INDEX;
			tmpLevel = c->BodyPart[BODYPART_BOOTS].Level;
			for (mu_int32 i = BODYPART_BOOTS; i >= end; --i)
			{
				mu_int32 Level = c->BodyPart[i].Level;
				if (Level < 9)
				{
					ThreadInfo.EquipmentLevelSet = 0;
					Success = false;
					break;
				}
				if (Type != (c->BodyPart[i].Type - MODEL_ITEM) % MAX_ITEM_INDEX)
				{
					ThreadInfo.EquipmentLevelSet = 0;
					Success = false;
					break;
				}

				if (Level >= 9 && tmpLevel >= Level)
				{
					tmpLevel = Level;
					ThreadInfo.EquipmentLevelSet = Level;
				}
			}
		}
	}

	if (InChaosCastle())
	{
		ChangeChaosCastleUnit(c);
	}

	return Success;
}

void TerminateOwnerEffectObject(mu_int32 iOwnerObjectType)
{
	if (iOwnerObjectType < 0)
	{
		return;
	}

	g_EffectsLock.Lock();
	for (mu_int32 i = 0; i < g_EffectsCurCount; ++i)
	{
		OBJECT *o = &g_Effects[i];

		if (o->Owner != nullptr &&
			o->Type == MODEL_AIR_FORCE &&
			o->Owner->Type == iOwnerObjectType)
		{
			o->Owner = nullptr;
		}
	}
	g_EffectsLock.Unlock();
}

void DeleteCloth(CHARACTER *c, OBJECT *o, PART_t *p2)
{
	if (o && o->m_pCloth)
	{
		CPhysicsCloth *pCloth = o->m_pCloth;
		for (mu_int32 i = 0; i < o->m_byNumCloth; ++i)
		{
			pCloth[i].Destroy();
		}
		delete[] pCloth;
		o->m_pCloth = nullptr;
		o->m_byNumCloth = 0;
	}

	if (c)
	{
		for (mu_int32 i = 0; i < MAX_BODYPART; ++i)
		{
			PART_t *p = &c->BodyPart[i];
			if (p->m_pCloth[0])
			{
				for (mu_int32 i = 0; i < p->m_byNumCloth; ++i)
				{
					CPhysicsCloth *pCloth = p->m_pCloth[i];
					pCloth->Destroy();
					delete pCloth;
					p->m_pCloth[i] = nullptr;
				}
			}
		}
	}

	if (p2)
	{
		if (p2->m_pCloth[0])
		{
			for (mu_int32 i = 0; i < p2->m_byNumCloth; ++i)
			{
				CPhysicsCloth *pCloth = p2->m_pCloth[i];
				pCloth->Destroy();
				delete pCloth;
				p2->m_pCloth[i] = nullptr;
			}
		}
	}
}

void ReleaseCharacters()
{
	mu_memset(CharactersKeys.data(), 0xFF, SizeOfArray(CharactersKeys));

	for (mu_int32 i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
	{
		CHARACTER *c = &CharactersClient[i];
		OBJECT *o = &c->Object;

		c->Key = -1;
		o->Flags.Clear();

		o->BoneTransform = nullptr;

		DeleteBug(o);
		giPetManager::DeletePet(c);
		DeleteCloth(c, o);
		DeleteParts(c);
	}

	OBJECT *o = &CharacterView.Object;

	o->BoneTransform = nullptr;

	DeleteBug(o);
	giPetManager::DeletePet(&CharacterView);
	DeleteCloth(&CharacterView, o);
	DeleteParts(&CharacterView);
}

void CreateCharacterPointer(CHARACTER *c, mu_int32 Type, mu_uint8 PositionX, mu_uint8 PositionY, mu_float Rotation)
{
	OBJECT *o = &c->Object;
	MUModel *b = &MODELS::Data[Type];

	c->PositionX = PositionX;
	c->PositionY = PositionY;
	c->TargetX = PositionX;
	c->TargetY = PositionY;

	if (c != Hero)
	{
		c->byExtensionSkill = 0;
	}

	c->m_pParts = nullptr;

	giPetManager::DeletePet(c);

	mu_int32 Index = TERRAIN_INDEX_REPEAT((c->PositionX), (c->PositionY));

	if ((TerrainWall[Index] & TW_SAFEZONE) == TW_SAFEZONE)
		c->SafeZone = true;
	else
		c->SafeZone = false;

	o->Flags.Set(OBJECT::eOF_ALPHAENABLE | OBJECT::eOF_LIGHTENABLE);

	c->Path.PathNum = 0;
	c->Path.CurrentPath = 0;
	c->Movement = false;
	c->Dead = false;
	c->Blood = false;
	c->GuildTeam = 0;
	c->Run = 0;
	c->GuildMarkIndex = -1;
	c->PK = PVP_NEUTRAL;
	o->Type = Type;
	o->Scale = 0.9f;
	o->Timer = 0.0f;
	o->Alpha = 1.0f;
	o->AlphaTarget = 1.0f;
	o->Velocity = 0.0f;
	o->ShadowScale = 0.0f;
	o->m_byHurtByOneToOne = 0;
	o->AI = 0;

	o->m_byBuildTime = 10;
	c->m_iDeleteTime = -128;
	o->m_fEdgeScale = 1.2f;
	c->m_bIsSelected = true;
	c->ExtendState = 0;
	c->ExtendStateTime = 0;

	c->m_byGensInfluence = 0;

	c->GuildStatus = GAME::G_NONE;
	c->GuildType = 0;
	c->ProtectGuildMarkWorldTime = 0.0f;
	c->GuildRelationShip = 0;
	c->GuildSkill = AT_SKILL_STUN;
	c->BackupCurrentSkill = 255;
	c->GuildMasterKillCount = 0;
	c->m_byDieType = 0;
	o->Flags.Clear(OBJECT::eOF_ACTIONSTART);
	o->SpecialSkillTick = 0;
	c->m_bFixForm = false;
	c->CtlCode = 0;
	c->m_CursedTempleCurSkill = AT_SKILL_CURSED_TEMPLE_PRODECTION;
	c->m_CursedTempleCurSkillPacket = false;

	if (Type<MODEL_FACE ||
		Type>MODEL_FACE + 5)
		c->Class = 0;

	if (Type == MODEL_PLAYER)
	{
		o->PriorAction = 1;
		o->CurrentAction = 1;
	}
	else
	{
		o->PriorAction = 0;
		o->CurrentAction = 0;
	}
	o->AnimationFrame = 0.0f;
	o->PriorAnimationFrame = 0;
	c->JumpTime = 0;
	o->HiddenMesh = -1;
	c->MoveSpeed = 10;

	g_CharacterClearBuff(o);

	o->Teleport = TELEPORT_NONE;
	o->Kind = KIND_PLAYER;
	c->Change = false;
	o->SubType = 0;
	c->MonsterIndex = -1;
	o->BlendMeshTexCoordU = 0.0f;
	o->BlendMeshTexCoordV = 0.0f;

	c->Skill = 0;
	c->AttackTime = 0;
	c->TargetCharacter = -1;
	c->AttackFlag = ATTACK_FAIL;

	c->Weapon[0].Type = -1;
	c->Weapon[0].Level = 0;
	c->Weapon[1].Type = -1;
	c->Weapon[1].Level = 0;
	c->Wing.Type = -1;
	c->Helper.Type = -1;

	o->Position[0] = (mu_float)((c->PositionX)*g_TerrainScale) + 0.5f*g_TerrainScale;
	o->Position[1] = (mu_float)((c->PositionY)*g_TerrainScale) + 0.5f*g_TerrainScale;

	o->InitialSceneFrame = MoveSceneFrame;

	o->Flags.Clear(OBJECT::eOF_SECONDANIMATION | OBJECT::eOF_DIRECTIONANIMATION);
	o->DirectionSpeed = 0.0f;

	if (GET_WORLD == -1 ||
		c->Helper.Type != MODEL_HELPER + 3 ||
		c->SafeZone)
	{
		o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
	}
	else
	{
		if (GET_WORLD == WD_8TARKAN || GET_WORLD == WD_10HEAVEN)
			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 90.0f;
		else
			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 30.0f;
	}

	Vector(0.0f, 0.0f, Rotation, o->Angle);
	Vector(0.0f, 0.0f, 0.0f, c->Light);
	Vector(0.0f, 0.0f, 0.0f, o->Light);
	Vector(-60.0f, -60.0f, 0.0f, o->BoundingBoxMin);
	switch (Type)
	{
	case MODEL_PLAYER:
		Vector(40.0f, 40.0f, 120.0f, o->BoundingBoxMax);
		break;
	case MODEL_MONSTER01 + 70:
	case MODEL_MONSTER01 + 71:
	case MODEL_MONSTER01 + 72:
		Vector(40.0f, 40.0f, 120.0f, o->BoundingBoxMax);
		break;
	case MODEL_MONSTER01 + 2:
	case MODEL_MONSTER01 + 6:
	case MODEL_MONSTER01 + 9:
	case MODEL_MONSTER01 + 20:
	case MODEL_MONSTER01 + 19:
	case MODEL_MONSTER01 + 17:
		Vector(50.0f, 50.0f, 80.0f, o->BoundingBoxMax);
		break;
	case MODEL_MONSTER01 + 11:
	case MODEL_MONSTER01 + 31:
	case MODEL_MONSTER01 + 39:
	case MODEL_MONSTER01 + 42:
	case MODEL_MONSTER01 + 44:
		Vector(70.0f, 70.0f, 250.0f, o->BoundingBoxMax);
		break;
	case MODEL_MONSTER01 + 37:
		Vector(100.0f, 100.0f, 150.0f, o->BoundingBoxMax);
		break;
	case MODEL_MONSTER01 + 61:
		Vector(-120.0f, -120.0f, 0.0f, o->BoundingBoxMin);
		Vector(100.0f, 100.0f, 300.0f, o->BoundingBoxMax);
		break;
	case MODEL_MONSTER01 + 60:
		Vector(-90.0f, -50.0f, 0.0f, o->BoundingBoxMin);
		Vector(90.0f, 50.0f, 200.0f, o->BoundingBoxMax);
		break;
	case MODEL_MONSTER01 + 150:
		Vector(-150.0f, -150.0f, 0.0f, o->BoundingBoxMin);
		Vector(150.0f, 150.0f, 400.0f, o->BoundingBoxMax);
		break;
	case MODEL_MONSTER01 + 151:
	case MODEL_MONSTER01 + 152:
	case MODEL_MONSTER01 + 153:
		Vector(-100.0f, -100.0f, 0.0f, o->BoundingBoxMin);
		Vector(100.0f, 100.0f, 200.0f, o->BoundingBoxMax);
		break;
	case MODEL_LITTLESANTA:
	case MODEL_LITTLESANTA + 1:
	case MODEL_LITTLESANTA + 2:
	case MODEL_LITTLESANTA + 3:
	case MODEL_LITTLESANTA + 4:
	case MODEL_LITTLESANTA + 5:
	case MODEL_LITTLESANTA + 6:
	case MODEL_LITTLESANTA + 7:
		{
			Vector(-160.0f, -60.0f, -20.0f, o->BoundingBoxMin);
			Vector(10.0f, 80.0f, 50.0f, o->BoundingBoxMax);
		}
		break;
	case MODEL_MONSTER01 + 157:
		{
			Vector(-100.0f, -70.0f, 0.0f, o->BoundingBoxMin);
			Vector(100.0f, 70.0f, 150.0f, o->BoundingBoxMax);
		}
		break;
	case MODEL_MONSTER01 + 158:
		{
			Vector(-100.0f, -100.0f, 50.0f, o->BoundingBoxMin);
			Vector(100.0f, 100.0f, 150.0f, o->BoundingBoxMax);
		}
		break;
	case MODEL_MONSTER01 + 159:
		{
			Vector(-100.0f, -100.0f, 0.0f, o->BoundingBoxMin);
			Vector(100.0f, 100.0f, 180.0f, o->BoundingBoxMax);
		}
		break;
	case MODEL_MONSTER01 + 160:
		{
			Vector(-80.0f, -80.0f, 0.0f, o->BoundingBoxMin);
			Vector(80.0f, 80.0f, 130.0f, o->BoundingBoxMax);
		}
		break;
	case MODEL_MONSTER01 + 161:
		{
			Vector(-80.0f, -80.0f, 0.0f, o->BoundingBoxMin);
			Vector(80.0f, 80.0f, 130.0f, o->BoundingBoxMax);
		}
		break;
	case MODEL_MONSTER01 + 162:
		{
			Vector(-100.0f, -80.0f, 50.0f, o->BoundingBoxMin);
			Vector(100.0f, 70.0f, 280.0f, o->BoundingBoxMax);
		}
		break;
	case MODEL_MONSTER01 + 163:
		{
			Vector(-100.0f, -80.0f, 50.0f, o->BoundingBoxMin);
			Vector(100.0f, 70.0f, 280.0f, o->BoundingBoxMax);
		}
		break;
	default:
		Vector(50.0f, 50.0f, 150.0f, o->BoundingBoxMax);
		break;
	}

#if 1 // FIX // DELETE this shouldn't exists if fixed mouse collision to obb is fixed
	if (g_CurrentScene == CHARACTER_SCENE)
	{
		o->BoundingBoxMin.x += 10.0f;
		o->BoundingBoxMin.y += 10.0f;
		o->BoundingBoxMin.z *= 1.7f;
		o->BoundingBoxMin.z -= 50.0f;

		o->BoundingBoxMax.x += 10.0f;
		o->BoundingBoxMax.y += 10.0f;
		o->BoundingBoxMax.z *= 1.7f;
		o->BoundingBoxMax.z -= 50.0f;
	}
#endif

	o->BoneTransform = nullptr;

	for (mu_int32 i = 0; i < 2; ++i)
	{
		c->Weapon[i].Type = -1;
		c->Weapon[i].Level = 0;
		c->Weapon[i].ExcOption = 0;
		c->Weapon[i].SetOption = 0;
	}

	for (mu_int32 i = 0; i < MAX_BODYPART; ++i)
	{
		c->BodyPart[i].Type = -1;
		c->BodyPart[i].Level = 0;
		c->BodyPart[i].ExcOption = 0;
		c->BodyPart[i].SetOption = 0;
	}

	c->Wing.Type = -1;
	c->Helper.Type = -1;
	c->Flag.Type = -1;

	c->LongRangeAttack = -1;
	c->CollisionTime = 0;
	o->CollisionRange = 200.0f;
	c->Rot = 0.0f;
	c->Level = 0;
	c->Item = -1;

	for (mu_int32 i = 0; i < 32; ++i) c->OwnerID[i] = 0;

	o->BlendMesh = -1;
	o->BlendMeshLight = 1.0f;

	if(b->IsDataLoaded() == false ||
	   b->IsGPULoaded() == false)
	{
		b->RequestLoad();
	}

	switch (Type)
	{
	case MODEL_MONSTER01 + 70:
	case MODEL_MONSTER01 + 71:
	case MODEL_MONSTER01 + 72:
		c->Weapon[0].LinkBone = b->GetBoneIndex(33);
		c->Weapon[1].LinkBone = b->GetBoneIndex(42);
		break;
	case MODEL_MONSTER01 + 57:
		c->Weapon[0].LinkBone = b->GetBoneIndex(30);
		c->Weapon[1].LinkBone = b->GetBoneIndex(39);
		break;
	case MODEL_MONSTER01 + 59:
		c->Weapon[0].LinkBone = b->GetBoneIndex(33);
		c->Weapon[1].LinkBone = b->GetBoneIndex(20);
		break;
	case MODEL_MONSTER01 + 60:
		c->Weapon[0].LinkBone = b->GetBoneIndex(1);
		c->Weapon[1].LinkBone = b->GetBoneIndex(1);
		break;
	case MODEL_MONSTER01 + 55:
		c->Weapon[0].LinkBone = b->GetBoneIndex(27);
		c->Weapon[1].LinkBone = b->GetBoneIndex(18);
		break;
	case MODEL_MONSTER01 + 52:
		c->Weapon[0].LinkBone = b->GetBoneIndex(36);
		c->Weapon[1].LinkBone = b->GetBoneIndex(45);
		break;
	case MODEL_MONSTER01 + 53:
		c->Weapon[0].LinkBone = b->GetBoneIndex(30);
		c->Weapon[1].LinkBone = b->GetBoneIndex(39);
		break;
	case MODEL_MONSTER01 + 46:
		c->Weapon[0].LinkBone = b->GetBoneIndex(39);
		c->Weapon[1].LinkBone = b->GetBoneIndex(39);
		break;
	case MODEL_MONSTER01 + 47:
		c->Weapon[0].LinkBone = b->GetBoneIndex(27);
		c->Weapon[1].LinkBone = b->GetBoneIndex(38);
		break;
	case MODEL_MONSTER01 + 48:
		c->Weapon[0].LinkBone = b->GetBoneIndex(32);
		c->Weapon[1].LinkBone = b->GetBoneIndex(43);
		break;
	case MODEL_MONSTER01 + 44:
		c->Weapon[0].LinkBone = b->GetBoneIndex(55);
		c->Weapon[1].LinkBone = b->GetBoneIndex(70);
		break;
	case MODEL_MONSTER01 + 42:
		c->Weapon[0].LinkBone = b->GetBoneIndex(43);
		break;
	case MODEL_MONSTER01 + 41:
		c->Weapon[0].LinkBone = b->GetBoneIndex(23);
		break;
	case MODEL_MONSTER01 + 36:
		c->Weapon[0].LinkBone = b->GetBoneIndex(52);
		c->Weapon[1].LinkBone = b->GetBoneIndex(65);
		break;
	case MODEL_MONSTER01 + 35:
		c->Weapon[0].LinkBone = b->GetBoneIndex(30);
		c->Weapon[1].LinkBone = b->GetBoneIndex(39);
		break;
	case MODEL_MONSTER01 + 34:
		c->Weapon[0].LinkBone = b->GetBoneIndex(30);
		c->Weapon[1].LinkBone = b->GetBoneIndex(39);
		break;
	case MODEL_MONSTER01 + 26:
		c->Weapon[0].LinkBone = b->GetBoneIndex(16);
		c->Weapon[1].LinkBone = b->GetBoneIndex(25);
		break;
	case MODEL_MONSTER01 + 29:
		c->Weapon[0].LinkBone = b->GetBoneIndex(30);
		c->Weapon[1].LinkBone = b->GetBoneIndex(39);
		break;
	case MODEL_MONSTER01 + 27:
		c->Weapon[0].LinkBone = b->GetBoneIndex(17);
		c->Weapon[1].LinkBone = b->GetBoneIndex(28);
		break;
	case MODEL_MONSTER01 + 24:
		c->Weapon[0].LinkBone = b->GetBoneIndex(39);
		c->Weapon[1].LinkBone = b->GetBoneIndex(30);
		break;
	case MODEL_MONSTER01 + 22:
		c->Weapon[0].LinkBone = b->GetBoneIndex(25);
		c->Weapon[1].LinkBone = b->GetBoneIndex(16);
		break;
	case MODEL_MONSTER01 + 21:
		c->Weapon[0].LinkBone = b->GetBoneIndex(24);
		c->Weapon[1].LinkBone = b->GetBoneIndex(19);
		break;
	case MODEL_MONSTER01 + 19:
		c->Weapon[0].LinkBone = b->GetBoneIndex(31);
		c->Weapon[1].LinkBone = b->GetBoneIndex(22);
		break;
	case MODEL_MONSTER01 + 18:
		c->Weapon[0].LinkBone = b->GetBoneIndex(26);
		c->Weapon[1].LinkBone = b->GetBoneIndex(35);
		break;
	case MODEL_MONSTER01 + 16:
	case MODEL_MONSTER01 + 11:
		c->Weapon[0].LinkBone = b->GetBoneIndex(30);
		c->Weapon[1].LinkBone = b->GetBoneIndex(39);
		break;
	case MODEL_MONSTER01 + 3:
		c->Weapon[0].LinkBone = b->GetBoneIndex(26);
		c->Weapon[1].LinkBone = b->GetBoneIndex(36);
		break;
	case MODEL_MONSTER01:
	case MODEL_MONSTER01 + 30:
		c->Weapon[0].LinkBone = b->GetBoneIndex(42);
		c->Weapon[1].LinkBone = b->GetBoneIndex(33);
		break;
	case MODEL_MONSTER01 + 10:
	case MODEL_MONSTER01 + 4:
	case MODEL_MONSTER01 + 5:
		c->Weapon[0].LinkBone = b->GetBoneIndex(41);
		c->Weapon[1].LinkBone = b->GetBoneIndex(32);
		break;
	case MODEL_MONSTER01 + 1:
		c->Weapon[0].LinkBone = b->GetBoneIndex(19);
		c->Weapon[1].LinkBone = b->GetBoneIndex(14);
		break;
	case MODEL_MONSTER01 + 8:
		c->Weapon[0].LinkBone = b->GetBoneIndex(29);
		c->Weapon[1].LinkBone = b->GetBoneIndex(38);
		break;
	case MODEL_MONSTER01 + 40:
		c->Weapon[0].LinkBone = b->GetBoneIndex(20);
		c->Weapon[1].LinkBone = b->GetBoneIndex(33);
		break;

	case MODEL_FACE + 1:
		c->Weapon[0].LinkBone = -1;
		c->Weapon[1].LinkBone = -1;
		break;

	default:
		if (SettingHellasMonsterLinkBone(c, Type)) return;
		if (battleCastle::SettingBattleCastleMonsterLinkBone(c, Type)) return;
		c->Weapon[0].LinkBone = b->GetBoneIndex(33);
		c->Weapon[1].LinkBone = b->GetBoneIndex(42);
		break;
	}
}

// This function can only be called from the main thread at single-thread steps.
CHARACTER *CreateCharacter(mu_int32 Key, mu_int32 Type, mu_uint8 PositionX, mu_uint8 PositionY, mu_float Rotation, mu_boolean Live)
{
	// Runtime Model Loading (Prevent singe threading bones usage bug)
	{
		MUModel *b = &MODELS::Data[Type];
		if(b->IsDataLoaded() == false)
		{
			b->TryRealtimeLoad();
		}
	}

	MUAutoCriticalSpinLock lock(CharactersClientLock);

	mu_int16 tmpIndex = CharactersKeys[Key];

	if (tmpIndex != -1)
	{
		CHARACTER *c = &CharactersClient[tmpIndex];
		OBJECT *o = &c->Object;
		if(o->Flags.Get(OBJECT::eOF_LIVE) == true)
		{
			if (Live == true)
			{
				o->Flags.Set(OBJECT::eOF_LIVE);
				o->Flags.Clear(OBJECT::eOF_VISIBLE);
			}
			else
			{
				o->Flags.Clear(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE);
			}

			CreateCharacterPointer(c, Type, PositionX, PositionY, Rotation);
			g_CharacterClearBuff(o);
			return c;
		}
	}

	for (mu_int32 i = 1; i < MAX_CHARACTERS_CLIENT; ++i)
	{
		CHARACTER *c = &CharactersClient[i];
		OBJECT *o = &c->Object;
		if (o->Flags.Get(OBJECT::eOF_LIVE) == false)
		{
			if (c->Key == -1)
			{
				giPetManager::DeletePet(c);
				DeleteCloth(c, o);
				DeleteParts(c);

				if (Live)
				{
					o->Flags.Set(OBJECT::eOF_LIVE);
				}
				o->Flags.Clear(OBJECT::eOF_VISIBLE);

				CreateCharacterPointer(c, Type, PositionX, PositionY, Rotation);

				g_CharacterClearBuff(o);
				c->Key = Key;
				CharactersKeys[Key] = i;
				return c;
			}
		}
	}

	return &CharactersClient[MAX_CHARACTERS_CLIENT];
}

CHARACTER *CreateHero(const mu_uint32 ThreadIndex, mu_int32 Key, mu_int32 Class, mu_int32 Skin, mu_float x, mu_float y, mu_float Rotate)
{
	CHARACTER *c = &CharactersClient[Key];
	OBJECT *o = &c->Object;

	o->Flags.Set(OBJECT::eOF_LIVE);
	o->Flags.Clear(OBJECT::eOF_VISIBLE);

	CreateCharacterPointer(c, MODEL_PLAYER, 0, 0, Rotate);
	Vector(0.3f, 0.3f, 0.3f, o->Light);
	c->Key = -1;
	o->Position[0] = x;
	o->Position[1] = y;
	c->Class = Class;
	c->Skin = Skin;

	g_CharacterClearBuff(o);

	if (g_CurrentScene == LOG_IN_SCENE)
	{
		c->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 28;
		c->BodyPart[BODYPART_HELM].Level = 7;
		c->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 28;
		c->BodyPart[BODYPART_ARMOR].Level = 7;
		c->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 28;
		c->BodyPart[BODYPART_PANTS].Level = 7;
		c->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 28;
		c->BodyPart[BODYPART_GLOVES].Level = 7;
		c->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 28;
		c->BodyPart[BODYPART_BOOTS].Level = 7;
		c->Weapon[0].Type = MODEL_MACE + 14;
		c->Weapon[1].Type = MODEL_HELPER + 5;
		c->Weapon[0].Level = 13;
		c->Wing.Type = MODEL_HELPER + 30;
		c->Helper.Type = MODEL_HELPER + 4;
	}
	else
	{
		c->BodyPart[BODYPART_HELM].Type = MODEL_BODY_HELM + GetSkinModelIndex(Class);
		c->BodyPart[BODYPART_ARMOR].Type = MODEL_BODY_ARMOR + GetSkinModelIndex(Class);
		c->BodyPart[BODYPART_PANTS].Type = MODEL_BODY_PANTS + GetSkinModelIndex(Class);
		c->BodyPart[BODYPART_GLOVES].Type = MODEL_BODY_GLOVES + GetSkinModelIndex(Class);
		c->BodyPart[BODYPART_BOOTS].Type = MODEL_BODY_BOOTS + GetSkinModelIndex(Class);
		c->Weapon[0].Type = -1;
		c->Weapon[1].Type = -1;
		c->Wing.Type = -1;
		c->Helper.Type = -1;
	}

	UnRegisterBuff(eBuff_GMEffect, o);

	c->CtlCode = 0;
	SetCharacterScale(c);
	SetPlayerStop(ThreadIndex, c);
	return c;
}

void MakeElfHelper(CHARACTER* c)
{
	OBJECT* o = &c->Object;
	c->Wing.Type = MODEL_WING + 3;

	c->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 24;
	c->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 24;
	c->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 24;
	c->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 24;
	c->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 24;

	c->BodyPart[BODYPART_HELM].Level = 13;
	c->BodyPart[BODYPART_ARMOR].Level = 13;
	c->BodyPart[BODYPART_PANTS].Level = 13;
	c->BodyPart[BODYPART_GLOVES].Level = 13;
	c->BodyPart[BODYPART_BOOTS].Level = 13;

	o->Scale = 1.0f;

	o->CurrentAction = PLAYER_STOP_FLY;

	o->BoundingBoxMax[2] += 70.0f;
}

void Setting_Monster(CHARACTER *c, mu_int32 Type, mu_int32 PositionX, mu_int32 PositionY)
{
	OBJECT *o;

	mu_int32 nCastle = BLOODCASTLE_NUM + (GET_WORLD - WD_11BLOODCASTLE_END);
	if (nCastle > 0 && nCastle <= BLOODCASTLE_NUM)
	{
		if (Type >= 84 && Type <= 143)
		{
			c->Level = 0;
			c->Object.Scale += mu_int32(nCastle / 3)*0.05f;
		}
	}

	if (c != nullptr)
	{
		o = &c->Object;

		MONSTER_SCRIPT *m = &MonsterScript[Type];

		if (m->Used == true)
		{
			mu_strcpy_s(c->ID, mu_countof(c->ID), m->Name);
		}

		c->MonsterIndex = Type;
		c->Object.ExtState = 0;
		c->TargetCharacter = HeroIndex;
		if (Type == 200)
			o->Kind = KIND_MONSTER;
		else if (Type >= 260)
			o->Kind = KIND_MONSTER;
		else if (Type > 200)
			o->Kind = KIND_NPC;
		else if (Type >= 150)
			o->Kind = KIND_MONSTER;
		else if (Type > 110)
			o->Kind = KIND_MONSTER;
		else if (Type >= 100)
			o->Kind = KIND_TRAP;
		else
			o->Kind = KIND_MONSTER;

		if (Type == 368 || Type == 369 || Type == 370)
			o->Kind = KIND_NPC;
		if (Type == 367
			|| Type == 371
			|| Type == 375
			|| Type == 376
			|| Type == 377
			|| Type == 379
			|| Type == 380
			|| Type == 381
			|| Type == 382
			|| Type == 383
			|| Type == 384
			|| Type == 385
			|| Type == 406
			|| Type == 407
			|| Type == 408
			|| Type == 414
			|| Type == 415
			|| Type == 416
			|| Type == 417
			|| Type == 450
			|| Type == 452
			|| Type == 453
			|| Type == 464
			|| Type == 465
			|| Type == 467
			|| Type == 468
			|| Type == 469
			|| Type == 470
			|| Type == 471
			|| Type == 472
			|| Type == 473
			|| Type == 474
			|| Type == 475
			|| Type == 478
			|| Type == 479
			|| Type == 492
			|| Type == 540
			|| Type == 541
			|| Type == 542
			|| Type == 522
			|| Type == 543
			|| Type == 544
			|| Type == 545
			|| Type == 546
			|| Type == 547
			)
		{
			o->Kind = KIND_NPC;
		}
		else if (Type >= 480 && Type <= 491)
		{
			o->Kind = KIND_MONSTER;
		}
		else if (Type == 451)
		{
			o->Kind = KIND_TMP;
		}	
	}
}

CHARACTER *CreateMonster(const mu_uint32 ThreadIndex, mu_int32 Type, mu_int32 PositionX, mu_int32 PositionY, mu_int32 Key)
{
	CHARACTER *c = nullptr;
	OBJECT *o;
	mu_int32 Level;

	c = TheMapProcess().CreateMonster(ThreadIndex, Type, PositionX, PositionY, Key);
	if (c != nullptr)
	{
		Setting_Monster(c, Type, PositionX, PositionY);
		return c;
	}

	switch (Type)
	{
	case 224:
		c = CreateCharacter(Key, MODEL_NPC_CLERK, PositionX, PositionY);
		c->m_bFixForm = true;
		c->Object.Scale = 1.0f;
		c->Object.SubType = GetLargeRand(2) + 10;
		c->Weapon[0].Type = -1;
		c->Weapon[1].Type = -1;
		break;
	case 152:
	case 153:
	case 154:
	case 155:
	case 156:
	case 157:
	case 158:
		c = CreateCharacter(Key, MODEL_WARCRAFT, PositionX, PositionY);
		c->m_bFixForm = true;
		c->Weapon[0].Type = -1;
		c->Weapon[0].Level = 0;
		c->Object.Scale = 1.0f;
		c->HideShadow = false;
		o = &c->Object;
		o->PriorAnimationFrame = 10.0f;
		o->AnimationFrame = 10;
		o->BlendMesh = -1;
		break;
	case 162:
	case 164:
	case 166:
	case 168:
	case 170:
	case 172:
	case 426:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 70, PositionX, PositionY);
			c->Object.Scale = 0.9f;
			o = &c->Object;

			c->Weapon[0].Type = MODEL_SWORD + 16;
			c->Weapon[0].Level = 0;
			c->Weapon[1].Type = MODEL_SWORD + 16;
			c->Weapon[1].Level = 0;
		}
		break;

	case 163:
	case 165:
	case 167:
	case 169:
	case 171:
	case 173:
	case 427:
		{
			mu_int32 randType = GetLargeRand(2);

			c = CreateCharacter(Key, MODEL_MONSTER01 + 71 + randType, PositionX, PositionY);
			c->Object.Scale = 0.9f;
			o = &c->Object;

			c->Weapon[0].Type = -1;
			c->Weapon[0].Level = 0;
			c->Weapon[1].Type = -1;
			c->Weapon[1].Level = 0;

			if (randType == 0)
			{
				c->Weapon[0].Type = MODEL_BOW + 19;
				c->Weapon[0].Level = 0;
			}
			else
			{
				c->Weapon[0].Type = MODEL_STAFF + 5;
				c->Weapon[0].Level = 0;
			}
		}
		break;
	case 89:
	case 95:
	case 112:
	case 118:
	case 124:
	case 130:
	case 143:
	case 433:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 62, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_STAFF;
		c->Weapon[0].Level = 11;
		c->Object.Scale = 1.2f;
		break;
	case 131:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 61, PositionX, PositionY);
		c->m_bFixForm = true;
		c->Object.Scale = 0.8f;
		break;
	case 132:
	case 133:
	case 134:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 60, PositionX, PositionY);
		c->m_bFixForm = true;
		c->Object.Scale = 0.8f;
		break;
	case 84:
	case 90:
	case 96:
	case 113:
	case 119:
	case 125:
	case 138:
	case 428:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 47, PositionX, PositionY);
		c->Object.Scale = 1.1f;
		o = &c->Object;
		break;
	case 85:
	case 91:
	case 97:
	case 114:
	case 120:
	case 126:
	case 139:
	case 429:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 46, PositionX, PositionY);
		c->Object.Scale = 1.1f;
		c->Weapon[1].Type = MODEL_BOW + 3;
		c->Weapon[1].Level = 1;
		o = &c->Object;
		break;
	case 86:
	case 92:
	case 98:
	case 115:
	case 121:
	case 127:
	case 140:
	case 430:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 59, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_AXE + 8;
		c->Weapon[0].Level = 0;
		c->Weapon[1].Type = MODEL_AXE + 8;
		c->Weapon[1].Level = 0;
		c->Object.Scale = 1.0f;
		break;
	case 87:
	case 93:
	case 99:
	case 116:
	case 122:
	case 128:
	case 141:
	case 431:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 58, PositionX, PositionY);
		c->Object.Scale = 0.8f;
		break;
	case 88:
	case 94:
	case 111:
	case 117:
	case 123:
	case 129:
	case 142:
	case 432:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 57, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_MACE + 6;

		if (!mu_int32((7 + (GET_WORLD - WD_11BLOODCASTLE_END)) / 3))
			c->Weapon[0].Level = 8;
		else
			c->Weapon[0].Level = 0;

		c->Object.Scale = 1.19f;
		break;
	case 78:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 19, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_AXE;
		c->Weapon[0].Level = 9;
		c->Object.Scale = 0.8f;
		break;
	case 79:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 31, PositionX, PositionY);
		c->Object.Scale = 0.9f;
		break;
	case 80:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 36, PositionX, PositionY);
		c->Object.Scale = 1.4f;
		c->Weapon[0].Type = MODEL_STAFF + 7;
		c->Weapon[0].ExcOption = 63;
		break;
	case 81:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 34, PositionX, PositionY);
		c->Object.Scale = 1.0f;
		break;
	case 82:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 42, PositionX, PositionY);
		c->Object.BlendMesh = 2;
		c->Object.BlendMeshLight = 1.0f;
		o = &c->Object;
		c->Object.Scale = 1.8f;
		c->Weapon[0].Type = MODEL_SWORD + 16;
		c->Weapon[0].ExcOption = 63;
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 2, o, 30.0f);
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 3, o, 30.0f);
		break;
	case 83:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 41, PositionX, PositionY);
		c->Object.Scale = 1.4f;
		c->Weapon[0].Type = MODEL_BOW + 14;
		c->Weapon[0].ExcOption = 63;
		o = &c->Object;
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 2, o, 30.0f);
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 3, o, 30.0f);
		break;
	case 69:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 50, PositionX, PositionY);
		c->Object.Scale = 1.0f;
		c->Object.BlendMesh = 0;
		break;
	case 70:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 51, PositionX, PositionY);
		c->Object.Scale = 1.3f;
		c->Object.BlendMesh = -2;
		c->Object.BlendMeshLight = 1.0f;
		break;

	case 301:
	case 71:
	case 74:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 52, PositionX, PositionY);
		if (71 == Type)
		{
			c->Object.Scale = 1.1f;
			c->Weapon[0].Type = MODEL_SWORD + 18;
			c->Weapon[0].Level = 5;
			c->Weapon[1].Type = MODEL_SHIELD + 14;
			c->Weapon[1].Level = 0;
		}
		else
		{
			c->Object.Scale = 1.3f;
			c->Weapon[0].Type = MODEL_SWORD + 18;
			c->Weapon[0].Level = 9;
			c->Weapon[1].Type = MODEL_SHIELD + 14;
			c->Weapon[1].Level = 9;
		}
		c->Object.BlendMesh = 1;
		c->Object.BlendMeshLight = 1.0f;
		break;

	case 72: 
		c = CreateCharacter(Key, MODEL_MONSTER01 + 53, PositionX, PositionY);
		c->Object.Scale = 1.45f;
		c->Weapon[0].Type = MODEL_SWORD + 17;
		c->Weapon[0].Level = 5;
		break;

	case 73:    
	case 75:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 54, PositionX, PositionY);
		c->m_bFixForm = true;
		if (Type == 75)
		{
			c->Object.Scale = 1.0f;
		}
		else
		{
			c->Object.Scale = 0.8f;
		}
		break;
	case 77:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 55, PositionX, PositionY);
			c->m_bFixForm = true;
			c->Object.Scale = 1.0f;
		}
		break;
	case 64:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 46, PositionX, PositionY);
		c->Object.Scale = 1.2f;
		c->Weapon[1].Type = MODEL_BOW + 3;
		c->Weapon[1].Level = 3;
		o = &c->Object;
		o->HiddenMesh = 1;
		break;
	case 137:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 46, PositionX, PositionY);
		c->Object.Scale = 1.2f;
		c->Weapon[1].Type = MODEL_BOW + 3;
		c->Weapon[1].Level = 5;
		o = &c->Object;
		o->HiddenMesh = 1;
		break;
	case 65:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 47, PositionX, PositionY);
		c->Object.Scale = 1.3f;
		o = &c->Object;
		o->HiddenMesh = 2;
		break;
	case 136:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 47, PositionX, PositionY);
		c->Object.Scale = 1.3f;
		o = &c->Object;
		o->HiddenMesh = 2;
		break;
	case 66:
	case 135:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 48, PositionX, PositionY);
		c->Object.Scale = 1.7f;
		o = &c->Object;
		break;
	case 466:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 156, PositionX, PositionY);
		c->Object.Scale = 0.9f;
		o = &c->Object;
		break;
	case 476:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 155, PositionX, PositionY);
		c->Object.Scale = 1.7f;
		o = &c->Object;
		break;

	case 300:
	case 62:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 45, PositionX, PositionY);
		c->Object.Scale = 1.5f;
		o = &c->Object;
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 2, o, 30.0f);
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 3, o, 30.0f);
		break;
	case 63:
	case 61:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 44, PositionX, PositionY);
		if (Type == 63)
		{
			c->Object.Scale = 1.9f;
			c->Object.BlendMesh = -2;
			c->Object.BlendMeshLight = 1.0f;
		}
		else
		{
			c->Object.Scale = 1.5f;
		}
		o = &c->Object;
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 2, o, 30.0f);
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 3, o, 30.0f);
		break;
	case 60:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 43, PositionX, PositionY);
		c->Object.Scale = 2.2f;
		o = &c->Object;
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 2, o, 30.0f);
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 3, o, 30.0f);
		break;
	case 58:
	case 59:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 42, PositionX, PositionY);
		c->Object.BlendMesh = 2;
		c->Object.BlendMeshLight = 1.0f;
		o = &c->Object;
		if (Type == 58)
		{
			c->Object.Scale = 1.8f;
			c->Weapon[0].Type = MODEL_SWORD + 16;
		}
		else
		{
			c->Object.Scale = 2.1f;
			o->SubType = 1;
			c->Weapon[0].Type = MODEL_STAFF + 8;
		}
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 2, o, 30.0f);
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 3, o, 30.0f);
		break;
	case 57:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 41, PositionX, PositionY);
		c->Object.Scale = 1.4f;
		c->Weapon[0].Type = MODEL_BOW + 14;
		
		o = &c->Object;
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 2, o, 30.0f);
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 3, o, 30.0f);
		break;
	case 52:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 35, PositionX, PositionY);
		c->Object.Scale = 1.4f;
		c->Weapon[0].Type = MODEL_BOW + 13;
		break;
	case 51:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 33, PositionX, PositionY);
		c->Object.Scale = 1.0f;
		c->Level = 1;
		break;
	case 50:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 38, PositionX, PositionY);
		c->Object.Scale = 1.8f;
		break;
	case 49:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 37, PositionX, PositionY);
		c->Object.Scale = 1.0f;
		c->Object.BlendMesh = 5;
		c->Object.BlendMeshLight = 0.0f;
		break;
	case 48:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 36, PositionX, PositionY);
		c->Object.Scale = 1.4f;
		c->Weapon[0].Type = MODEL_STAFF + 6;
		break;
	case 47:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 35, PositionX, PositionY);
		c->Object.Scale = 1.1f;
		c->Weapon[0].Type = MODEL_BOW + 13;
		c->Object.BlendMesh = 0;
		c->Object.BlendMeshLight = 1.0f;
		break;
	case 46:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 34, PositionX, PositionY);
		c->Object.Scale = 1.0f;
		break;
	case 45:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 33, PositionX, PositionY);
		c->Object.Scale = 0.6f;
		break;
	case 150:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 32, PositionX, PositionY);
		c->Object.Scale = 0.12f;
		break;
	case 44:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 31, PositionX, PositionY);
		c->Object.Scale = 0.9f;
		break;
	case 43:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 2, PositionX, PositionY);
		c->Object.Scale = 0.7f;
		break;
	case 42:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 31, PositionX, PositionY);
		c->Object.Scale = 1.3f;
		Vector(200.0f, 150.0f, 280.0f, c->Object.BoundingBoxMax);
		break;
	case 41:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 30, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_MACE + 3;
		
		c->Object.Scale = 1.1f;
		break;
	case 40:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 29, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_SWORD + 15;
		c->Weapon[0].Type = MODEL_SWORD + 14;
		
		c->Object.Scale = 1.3f;
		break;
	case 39:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 28, PositionX, PositionY);
		c->Object.Scale = 1.2f;
		c->Level = 1;
		break;
	case 38:
	case 67:	
		c = CreateCharacter(Key, MODEL_MONSTER01 + 27, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_SPEAR + 9;
		c->Weapon[0].Level = 9;
		c->Object.Scale = 1.6f;
		break;
	case 37:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 26, PositionX, PositionY);
		c->Object.Scale = 1.1f;
		break;
	case 36:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 28, PositionX, PositionY);
		c->Object.Scale = 1.2f;
		break;
	case 35:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 11, PositionX, PositionY);
		c->Object.Scale = 1.3f;
		c->Weapon[0].Type = MODEL_AXE + 8;
		c->Weapon[1].Type = MODEL_AXE + 8;
		c->Object.BlendMesh = 1;
		c->Object.BlendMeshLight = 1.0f;
		c->Level = 2;
		break;
	case 34:
		c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
		c->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 3;
		c->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 3;
		c->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 3;
		c->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 3;
		c->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 3;
		c->Weapon[0].Type = MODEL_STAFF + 5;
		c->Weapon[1].Type = MODEL_SHIELD + 14;
		Level = 9;
		c->BodyPart[BODYPART_HELM].Level = Level;
		c->BodyPart[BODYPART_ARMOR].Level = Level;
		c->BodyPart[BODYPART_PANTS].Level = Level;
		c->BodyPart[BODYPART_GLOVES].Level = Level;
		c->BodyPart[BODYPART_BOOTS].Level = Level;
		c->PK = PVP_MURDERER2;
		SetCharacterScale(c);
		if (InDevilSquare() == true)
		{
			c->Object.Scale = 1.0f;
		}
		break;
	case 33:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 19, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_MACE + 1;
		c->Weapon[1].Type = MODEL_SHIELD + 1;
		c->Object.Scale = 1.2f;
		c->Level = 1;
		break;
	case 32:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 25, PositionX, PositionY);
		break;
	case 31:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 24, PositionX, PositionY);
		c->Object.Scale = 1.3f;
		c->Weapon[0].Type = MODEL_SWORD + 8;
		c->Weapon[1].Type = MODEL_SWORD + 8;
		break;
	case 30:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 23, PositionX, PositionY);
		c->Object.Scale = 0.75f;
		break;
	case 29:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 22, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_BOW + 10;
		c->Object.Scale = 0.95f;
		break;
	case 28:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 21, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_SPEAR + 1;
		c->Object.Scale = 0.8f;
		c->Object.BlendMesh = 1;
		break;
	case 27:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 20, PositionX, PositionY);
		c->Object.Scale = 1.1f;
		break;
	case 26:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 19, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_AXE;
		c->Object.Scale = 0.8f;
		break;
	case 25:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 18, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_STAFF + 1;
		c->Object.BlendMesh = 2;
		c->Object.BlendMeshLight = 1.0f;
		c->Object.Scale = 1.1f;
		c->Object.Flags.Clear(OBJECT::eOF_LIGHTENABLE);
		c->Level = 3;
		break;
	case 24:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 17, PositionX, PositionY);
		break;
	case 23:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 16, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_AXE + 7;
		c->Weapon[1].Type = MODEL_SHIELD + 10;
		c->Object.Scale = 1.15f;
		break;
	case 22:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 15, PositionX, PositionY);
		c->Object.BlendMesh = 0;
		c->Object.BlendMeshLight = 1.0f;
		break;
	case 21:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 14, PositionX, PositionY);
		c->Object.Scale = 0.95f;
		break;
	case 20:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 13, PositionX, PositionY);
		c->Object.Scale = 1.4f;
		break;
	case 19:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 12, PositionX, PositionY);
		c->Object.Scale = 1.1f;
		break;
	case 18:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 11, PositionX, PositionY);
		c->Object.Scale = 1.5f;
		c->Weapon[0].Type = MODEL_STAFF + 4;
		c->Object.BlendMesh = 1;
		c->Object.BlendMeshLight = 1.0f;
		break;
	case 3:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 9, PositionX, PositionY);
		c->Object.Scale = 0.4f;
		break;
	case 17:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 10, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_AXE + 8;
		break;
	case 0:
	case 4:
	case 8:
	default:
		c = CreateCharacter(Key, MODEL_MONSTER01, PositionX, PositionY);
		if (Type == 0)
		{
			c->Object.HiddenMesh = 0;
			c->Object.Scale = 0.8f;
			c->Weapon[0].Type = MODEL_AXE + 6;
		}
		else if (Type == 4)
		{
			c->Weapon[0].Type = MODEL_SPEAR + 7;
			c->Object.Scale = 1.15f;
			c->Level = 1;
		}
		else if (Type == 8)
		{
			c->Weapon[0].Type = MODEL_SPEAR + 8;
			c->Object.Scale = 1.0f;
			c->Level = 2;

			g_CharacterRegisterBuff((&c->Object), eDeBuff_Poison);
		}
		break;
	case 11:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 7, PositionX, PositionY);
		c->Object.AlphaTarget = 0.4f;
		c->MoveSpeed = 15;
		c->Blood = true;
		break;
	case 12:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 6, PositionX, PositionY);
		c->Object.Scale = 0.6f;
		break;
	case 13:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 8, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_STAFF + 2;
		c->Object.Scale = 1.1f;
		break;
	case 1:
	case 5:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 1, PositionX, PositionY);
		if (Type == 1)
		{
			c->Object.HiddenMesh = 0;
			c->Object.Scale = 0.85f;
			c->Weapon[0].Type = MODEL_SWORD + 4;
		}
		if (Type == 5)
		{
			c->Object.HiddenMesh = 1;
			c->Weapon[0].Type = MODEL_SWORD + 7;
			c->Weapon[1].Type = MODEL_SHIELD + 9;
			c->Object.Scale = 1.1f;
			c->Level = 1;
		}
		break;

	case 2:	
		c = CreateCharacter(Key, MODEL_MONSTER01 + 2, PositionX, PositionY);
		c->Object.Scale = 0.5f;
		break;

	case 10:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 3, PositionX, PositionY);
		c->Object.Scale = 0.8f;
		c->Level = 1;
		c->Weapon[0].Type = MODEL_SWORD + 13;
		break;
	case 6:
	case 9:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 4, PositionX, PositionY);
		if (Type == 6)
		{
			c->Weapon[0].Type = MODEL_STAFF + 2;
			c->Object.Scale = 0.85f;
		}
		else
		{
			c->Weapon[0].Type = MODEL_STAFF + 3;
			c->Level = 1;
			c->Object.Scale = 1.1f;
		}
		break;
	case 7:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 5, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_AXE + 2;
		c->Weapon[1].Type = MODEL_AXE + 2;
		c->Object.Scale = 1.6f;
		break;

	case 14:    
	case 55:    
	case 56:    
		c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
		c->Object.SubType = MODEL_SKELETON1;
		c->Blood = true;
		if (Type == 14)
		{
			c->Object.Scale = 0.95f;
			c->Weapon[0].Type = MODEL_SWORD + 6;
			c->Weapon[1].Type = MODEL_SHIELD + 4;
		}
		else if (Type == 56)
		{
			c->Object.Scale = 0.8f;
			c->Weapon[0].Type = MODEL_SPEAR + 8;
		}
		else
		{
			c->Level = 1;
			c->Object.Scale = 1.4f;
			c->Weapon[0].Type = MODEL_SPEAR + 9;
		}
		break;
	case 15:	
		c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
		c->Object.Scale = 1.1f;
		c->Weapon[1].Type = MODEL_BOW + 2;
		c->Object.SubType = MODEL_SKELETON2;
		c->Level = 1;
		c->Blood = true;
		break;
	case 16:	
		c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
		c->Object.Scale = 1.2f;
		c->Weapon[0].Type = MODEL_AXE + 3;
		c->Weapon[1].Type = MODEL_SHIELD + 6;
		c->Object.SubType = MODEL_SKELETON3;
		c->Level = 1;
		c->Blood = true;
		break;
		
	case 372:
		c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
		c->Object.Scale = 0.95f;
		c->Object.SubType = MODEL_SKELETON_PCBANG;
		break;
		
	case 373:	
		c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
		c->Object.Scale = 0.95f;
		c->Object.SubType = MODEL_HALLOWEEN;
		break;
		
	case 374:
		c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
		c->Object.Scale = 0.85f;
		c->Object.SubType = MODEL_XMAS_EVENT_CHANGE_GIRL;
		break;
		
	case 378:
		c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
		c->Object.Scale = 1.0f;
		c->Object.SubType = MODEL_GM_CHARACTER;
		break;
	case 53:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 39, PositionX, PositionY);
		c->Object.Scale = 1.8f;
		c->Object.BlendMesh = 2;
		c->Object.BlendMeshLight = 1.0f;
		o = &c->Object;
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 2, o, 30.0f);
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 3, o, 30.0f);
		break;
	case 54:    
	case 151:   
		c = CreateCharacter(Key, MODEL_MONSTER01 + 40, PositionX, PositionY);
		c->Weapon[1].Type = MODEL_BOW + 14;
		if (Type == 54)
			c->Object.Scale = 1.1f;
		else
			c->Object.Scale = 1.3f;
		break;
		
	case 100:
		c = CreateCharacter(Key, 39, PositionX, PositionY);
		break;
	case 101:
		c = CreateCharacter(Key, 40, PositionX, PositionY);
		break;
	case 102:
		c = CreateCharacter(Key, 51, PositionX, PositionY);
		break;
	case 103:
		c = CreateCharacter(Key, 25, PositionX, PositionY);
		break;
	case 106:	
		c = CreateCharacter(Key, 51, PositionX, PositionY);
		break;
	case 200:
		c = CreateCharacter(Key, MODEL_BALL, PositionX, PositionY);
		o = &c->Object;
		o->BlendMesh = 2;
		o->Scale = 1.8f;
		c->Level = 1;
		break;

	case 226:   
		c = CreateCharacter(Key, MODEL_NPC_BREEDER, PositionX, PositionY);
		break;

	case 229:	
		c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
		c->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 9;
		c->BodyPart[BODYPART_HELM].Level = 7;
		c->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 9;
		c->BodyPart[BODYPART_ARMOR].Level = 7;
		c->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 9;
		c->BodyPart[BODYPART_PANTS].Level = 7;
		c->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 9;
		c->BodyPart[BODYPART_GLOVES].Level = 7;
		c->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 9;
		c->BodyPart[BODYPART_BOOTS].Level = 7;
		c->Weapon[0].Type = MODEL_SPEAR + 7;
		c->Weapon[0].Level = 8;
		c->Weapon[1].Type = -1;
		SetCharacterScale(c);
		break;

	case 230:
		c = CreateCharacter(Key, MODEL_MERCHANT_MAN, PositionX, PositionY);
		c->BodyPart[BODYPART_HELM].Type = MODEL_MERCHANT_MAN_HEAD;
		c->BodyPart[BODYPART_ARMOR].Type = MODEL_MERCHANT_MAN_UPPER + 1;
		c->BodyPart[BODYPART_GLOVES].Type = MODEL_MERCHANT_MAN_GLOVES + 1;
		c->BodyPart[BODYPART_BOOTS].Type = MODEL_MERCHANT_MAN_BOOTS;
		break;
	case 231:	
		c = CreateCharacter(Key, MODEL_DEVIAS_TRADER, PositionX, PositionY);
		break;

	case 232:
		c = CreateCharacter(Key, MODEL_NPC_ARCHANGEL, PositionX, PositionY);
		o = &c->Object;
		o->Scale = 1.0f;
		o->Kind = KIND_NPC;
		break;
	case 233:
		c = CreateCharacter(Key, MODEL_NPC_ARCHANGEL_MESSENGER, PositionX, PositionY);
		o = &c->Object;
		o->Scale = 1.0f;
		o->Kind = KIND_NPC;
		break;

	case 234:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 19, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_STAFF;
		c->Weapon[0].Level = 4;
		c->Object.Scale = 1.5f;
		c->Object.Kind = KIND_NPC;
		SetAction(&c->Object, 0);
		break;

	case 235:
		c = CreateCharacter(Key, MODEL_NPC_SEVINA, PositionX, PositionY);
		o = &c->Object;
		o->Scale = 1.0f;
		o->Kind = KIND_NPC;
		break;

	case 236:   
		c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
		o = &c->Object;
		o->SubType = MODEL_SKELETON2;
		o->Scale = 1.0f;
		o->Kind = KIND_NPC;
		c->Level = 8;
		break;
	case 237:
		c = CreateCharacter(Key, MODEL_NPC_DEVILSQUARE, PositionX, PositionY);
		break;
		
	case 369:
		c = CreateCharacter(Key, MODEL_REFINERY_NPC, PositionX, PositionY);
		o = &c->Object;
		break;
	case 370:
		c = CreateCharacter(Key, MODEL_RECOVERY_NPC, PositionX, PositionY);
		o = &c->Object;
		break;
	case 238:
		c = CreateCharacter(Key, MODEL_MIX_NPC, PositionX, PositionY);
		o = &c->Object;
		o->BlendMesh = 1;
		break;
	case 239:
		c = CreateCharacter(Key, MODEL_TOURNAMENT, PositionX, PositionY);
		break;
	case 240:
		c = CreateCharacter(Key, MODEL_STORAGE, PositionX, PositionY);
		break;
	case 241:
		c = CreateCharacter(Key, MODEL_MASTER, PositionX, PositionY);
		break;
	case 256:  
		c = CreateCharacter(Key, MODEL_NPC_SERBIS, PositionX, PositionY);
		break;
	case 257: 
		c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
		MakeElfHelper(c);
		o = &c->Object;
		CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 42, o, 15.0f);
		break;
	case 242:
		c = CreateCharacter(Key, MODEL_ELF_WIZARD, PositionX, PositionY);
		o = &c->Object;
		o->BlendMesh = 1;
		o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 140.0f;
		break;
	case 243:
		c = CreateCharacter(Key, MODEL_ELF_MERCHANT, PositionX, PositionY);
		break;
	case 244:
		c = CreateCharacter(Key, MODEL_SNOW_MERCHANT, PositionX, PositionY);
		break;
	case 245:
		c = CreateCharacter(Key, MODEL_SNOW_WIZARD, PositionX, PositionY);
		break;
	case 246:
		c = CreateCharacter(Key, MODEL_SNOW_SMITH, PositionX, PositionY);
		break;
	case 247:
		c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
		
		c->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 9;
		c->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 9;
		c->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 9;
		c->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 9;
		c->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 9;
		c->Weapon[0].Type = MODEL_BOW + 11;
		c->Weapon[1].Type = MODEL_BOW + 7;
		SetCharacterScale(c);
		break;
	case 248:
		c = CreateCharacter(Key, MODEL_MERCHANT_MAN, PositionX, PositionY);
		c->BodyPart[BODYPART_HELM].Type = MODEL_MERCHANT_MAN_HEAD + 1;
		c->BodyPart[BODYPART_ARMOR].Type = MODEL_MERCHANT_MAN_UPPER + 1;
		c->BodyPart[BODYPART_GLOVES].Type = MODEL_MERCHANT_MAN_GLOVES + 1;
		c->BodyPart[BODYPART_BOOTS].Type = MODEL_MERCHANT_MAN_BOOTS + 1;
		break;
	case 249:
		c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
		c->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 9;
		c->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 9;
		c->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 9;
		c->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 9;
		c->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 9;
		c->Weapon[0].Type = MODEL_SPEAR + 7;
		SetCharacterScale(c);
		break;
	case 250:
		c = CreateCharacter(Key, MODEL_MERCHANT_MAN, PositionX, PositionY);
		c->BodyPart[BODYPART_HELM].Type = MODEL_MERCHANT_MAN_HEAD;
		c->BodyPart[BODYPART_ARMOR].Type = MODEL_MERCHANT_MAN_UPPER;
		c->BodyPart[BODYPART_GLOVES].Type = MODEL_MERCHANT_MAN_GLOVES;
		c->BodyPart[BODYPART_BOOTS].Type = MODEL_MERCHANT_MAN_BOOTS;
		break;
	case 251:
		c = CreateCharacter(Key, MODEL_SMITH, PositionX, PositionY);
		c->Object.Scale = 0.95f;
		break;
	case 253:
		c = CreateCharacter(Key, MODEL_MERCHANT_GIRL, PositionX, PositionY);
		c->BodyPart[BODYPART_HELM].Type = MODEL_MERCHANT_GIRL_HEAD;
		c->BodyPart[BODYPART_ARMOR].Type = MODEL_MERCHANT_GIRL_UPPER;
		c->BodyPart[BODYPART_PANTS].Type = MODEL_MERCHANT_GIRL_LOWER;
		break;
	case 254:
		c = CreateCharacter(Key, MODEL_SCIENTIST, PositionX, PositionY);
		break;
	case 255:
		c = CreateCharacter(Key, MODEL_MERCHANT_FEMALE, PositionX, PositionY);
		c->BodyPart[BODYPART_HELM].Type = MODEL_MERCHANT_FEMALE_HEAD + 1;
		c->BodyPart[BODYPART_ARMOR].Type = MODEL_MERCHANT_FEMALE_UPPER + 1;
		c->BodyPart[BODYPART_PANTS].Type = MODEL_MERCHANT_FEMALE_LOWER + 1;
		c->BodyPart[BODYPART_BOOTS].Type = MODEL_MERCHANT_FEMALE_BOOTS + 1;
		break;
	case 204: 
		c = CreateCharacter(Key, MODEL_CRYWOLF_STATUE, PositionX, PositionY, 0.0f, false);
		break;
	case 205:
		c = CreateCharacter(Key, MODEL_CRYWOLF_ALTAR1, PositionX, PositionY);
		c->Object.Position[2] -= 10.0f;
		c->Object.HiddenMesh = -2;
		break;
	case 206:
		c = CreateCharacter(Key, MODEL_CRYWOLF_ALTAR2, PositionX, PositionY);
		c->Object.HiddenMesh = -2;
		c->Object.Position[2] -= 10.0f;
		break;
	case 207:
		c = CreateCharacter(Key, MODEL_CRYWOLF_ALTAR3, PositionX, PositionY);
		c->Object.HiddenMesh = -2;
		c->Object.Position[2] -= 10.0f;
		break;
	case 208:
		c = CreateCharacter(Key, MODEL_CRYWOLF_ALTAR4, PositionX, PositionY);
		c->Object.HiddenMesh = -2;
		c->Object.Position[2] -= 10.0f;
		break;
	case 209:
		c = CreateCharacter(Key, MODEL_CRYWOLF_ALTAR5, PositionX, PositionY);
		c->Object.HiddenMesh = -2;
		c->Object.Position[2] -= 10.0f;
		break;
	case 368:
		c = CreateCharacter(Key, MODEL_SMELTING_NPC, PositionX + 1, PositionY - 1);
		c->Object.Scale = 2.5f;
		break;
	case 379:
		c = CreateCharacter(Key, MODEL_WEDDING_NPC, PositionX, PositionY);
		c->Object.Scale = 1.1f;
		break;
	case 258:
	case 371:
	case 414:
		c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
		c->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 9;
		c->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 9;
		c->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 9;
		c->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 9;
		c->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 9;
		c->Weapon[0].Type = -1;
		SetCharacterScale(c);
		c->Object.Flags.Set(OBJECT::eOF_PCROOM);
		break;
	case 259:
		c = CreateCharacter(Key, MODEL_KALIMA_SHOP, PositionX, PositionY);
		c->Object.Position[2] += 140.0f;
		break;
	case 375:
		{
			c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
			c->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 30;
			c->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 30;
			c->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 30;
			c->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 30;
			c->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 30;
			c->Wing.Type = MODEL_WING + 1;
			mu_int32 iLevel = 9;
			c->BodyPart[BODYPART_HELM].Level = iLevel;
			c->BodyPart[BODYPART_ARMOR].Level = iLevel;
			c->BodyPart[BODYPART_PANTS].Level = iLevel;
			c->BodyPart[BODYPART_GLOVES].Level = iLevel;
			c->BodyPart[BODYPART_BOOTS].Level = iLevel;
			c->Weapon[0].Type = -1;
			SetCharacterScale(c);
			c->Object.SubType = Type;
		}
		break;
	case 376:
		{
			c = CreateCharacter(Key, MODEL_BC_NPC1, PositionX, PositionY);
			c->Object.Scale = 1.0f;
			c->Object.Angle[2] = 0.0f;
			CreateObject(ThreadIndex, MODEL_BC_BOX, c->Object.Position, c->Object.Angle);
		}
		break;
	case 377:
		{
			c = CreateCharacter(Key, MODEL_BC_NPC2, PositionX, PositionY);
			c->Object.Scale = 1.0f;
			c->Object.Angle[2] = 90.0f;
			CreateObject(ThreadIndex, MODEL_BC_BOX, c->Object.Position, c->Object.Angle);
		}
		break;
	case 406:
		c = CreateCharacter(Key, MODEL_NPC_DEVIN, PositionX, PositionY);
		break;
	case 407:
		c = CreateCharacter(Key, MODEL_NPC_QUARREL, PositionX, PositionY);
		c->Object.Scale = 1.9f;
		break;
	case 408:
		c = CreateCharacter(Key, MODEL_NPC_CASTEL_GATE, PositionX, PositionY, 90.0f);
		o = &c->Object;
		o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 240.0f;
		c->Object.Scale = 1.2f;
		c->Object.m_fEdgeScale = 1.1f;
		break;
	case 413:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 127, PositionX, PositionY);
			c->Object.Scale = 0.8f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			c->Object.SubType = GetLargeRand(3);
			c->Object.m_iAnimation = 0;
		}

		break;
	case 450:
		{
			c = CreateCharacter(Key, MODEL_NPC_CHERRYBLOSSOM, PositionX, PositionY);
			c->Object.Scale = 0.65f;
			c->Object.m_fEdgeScale = 1.08f;
			o = &c->Object;
			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 170.0f;
		}
		break;
	case 451:
		{
			c = CreateCharacter(Key, MODEL_NPC_CHERRYBLOSSOMTREE, PositionX, PositionY);
			c->Object.Scale = 1.0f;
			c->Object.m_fEdgeScale = 0.0f;
		}
		break;
	case 452:
		c = CreateCharacter(Key, MODEL_SEED_MASTER, PositionX, PositionY);
		c->Object.Scale = 1.1f;
		c->Object.m_fEdgeScale = 1.2f;
		break;
	case 453:
		c = CreateCharacter(Key, MODEL_SEED_INVESTIGATOR, PositionX, PositionY);
		c->Object.Scale = 0.9f;
		c->Object.m_fEdgeScale = 1.15f;
		break;
	case 464:
		{
			c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);

			c->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 9;
			c->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 9;
			c->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 9;
			c->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 9;
			c->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 9;

			c->Object.m_fEdgeScale = 1.15f;
			c->Weapon[0].Type = MODEL_BOW + 11;
			c->Weapon[1].Type = MODEL_BOW + 7;
			SetCharacterScale(c);
		}
		break;
	case 477:
		c = CreateCharacter(Key, MODEL_XMAS2008_SNOWMAN, PositionX, PositionY);
		c->Object.LifeTime = 100;
		c->Object.Scale = 1.3f;
		break;
	case 503:
		c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
		c->Object.SubType = MODEL_PANDA;
		break;
	case 548:
		c = CreateCharacter(Key, MODEL_PLAYER, PositionX, PositionY);
		c->Object.SubType = MODEL_SKELETON_CHANGED;
		break;
	case 468:
	case 469:
	case 470:
	case 471:
	case 472:
	case 473:
	case 474:
	case 475:
		{
			mu_int32 _Model_NpcIndex = MODEL_LITTLESANTA + (Type - 468);

			c = CreateCharacter(Key, _Model_NpcIndex, PositionX, PositionY);

			c->Object.Scale = 0.43f;
		}
		break;
	case 478:
		c = CreateCharacter(Key, MODEL_NPC_SERBIS, PositionX, PositionY);
		break;
	case 479:
		c = CreateCharacter(Key, MODEL_DUEL_NPC_TITUS, PositionX, PositionY);
		c->Object.Scale = 1.1f;
		c->Object.m_fEdgeScale = 1.2f;
		break;
	case 492:
		{
			c = CreateCharacter(Key, MODEL_GAMBLE_NPC_MOSS, PositionX, PositionY);
			c->Object.LifeTime = 100;
			c->Object.Scale = 0.8f;
			c->Object.m_fEdgeScale = 1.1f;
		}
		break;
	case 502:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 128, PositionX, PositionY);
		c->Object.Scale = 1.0f * 0.95f;
		c->Weapon[0].Type = -1;
		c->Weapon[1].Type = -1;
		break;
	case 493:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 3, PositionX, PositionY);
		c->Object.Scale = 0.8f;
		c->Level = 1;
		c->Weapon[0].Type = MODEL_SWORD + 13;
		break;
		break;
	case 494:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 26, PositionX, PositionY);
		c->Object.Scale = 1.1f;
		break;
	case 495:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 25, PositionX, PositionY);
		c->Object.Scale = 1.35f;
		c->Weapon[0].Type = -1;
		c->Weapon[1].Type = -1;
		break;
	case 496:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 52, PositionX, PositionY);
		c->Object.Scale = 1.1f;
		c->Weapon[0].Type = MODEL_SWORD + 18;
		c->Weapon[0].Level = 5;
		c->Weapon[1].Type = MODEL_SHIELD + 14;
		c->Weapon[1].Level = 0;
		c->Object.BlendMesh = 1;
		c->Object.BlendMeshLight = 1.0f;
		break;
	case 497:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 109, PositionX, PositionY);
		c->Object.Scale = 1.3f;
		c->Weapon[0].Type = -1;
		c->Weapon[1].Type = -1;
		break;
	case 498:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 115, PositionX, PositionY);
		c->Object.Scale = 1.3f;
		c->Object.Angle[0] = 0.0f;
		c->Object.Gravity = 0.0f;
		c->Object.Distance = (mu_float)(GetLargeRand(20)) / 10.0f;
		c->Weapon[0].Type = -1;
		c->Weapon[1].Type = -1;
		break;
	case 499:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 149, PositionX, PositionY);
		c->Object.Scale = 1.5f;
		c->Weapon[0].Type = -1;
		c->Weapon[1].Type = -1;
		break;
	case 500:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 142, PositionX, PositionY);
		c->Object.Scale = 0.95f;
		c->Weapon[0].Type = -1;
		c->Weapon[1].Type = -1;
		break;
	case 501:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 31, PositionX, PositionY);
		c->Object.Scale = 0.88f;
		c->Weapon[0].Type = -1;
		c->Weapon[1].Type = -1;
		break;
	case 540:
		c = CreateCharacter(Key, MODEL_DOPPELGANGER_NPC_LUGARD, PositionX, PositionY);
		c->Object.Scale = 1.1f;
		c->Object.m_fEdgeScale = 1.2f;
		break;
	case 541:
		c = CreateCharacter(Key, MODEL_DOPPELGANGER_NPC_BOX, PositionX, PositionY);
		c->Object.Scale = 2.3f;
		c->Object.m_fEdgeScale = 1.1f;
		break;
	case 542:
		c = CreateCharacter(Key, MODEL_DOPPELGANGER_NPC_GOLDENBOX, PositionX, PositionY);
		c->Object.Scale = 3.3f;
		c->Object.m_fEdgeScale = 1.1f;
		break;
	case 543:
		c = CreateCharacter(Key, MODAL_GENS_NPC_DUPRIAN, PositionX, PositionY);
		c->Object.Scale = 1.0f;
		break;
	case 544:
		c = CreateCharacter(Key, MODAL_GENS_NPC_BARNERT, PositionX, PositionY);
		c->Object.Scale = 1.0f;
		break;
	case 545:
		c = CreateCharacter(Key, MODEL_UNITEDMARKETPLACE_CHRISTIN, PositionX, PositionY);
		c->Object.Scale = 1.1f;
		c->Object.m_fEdgeScale = 1.2f;
		break;
	case 546:
		c = CreateCharacter(Key, MODEL_UNITEDMARKETPLACE_RAUL, PositionX, PositionY);
		c->Object.Scale = 1.0f;
		c->Object.m_fEdgeScale = 1.15f;
		break;
	case 547:
		c = CreateCharacter(Key, MODEL_UNITEDMARKETPLACE_JULIA, PositionX, PositionY);
		c->Object.Scale = 1.0f;
		c->Object.m_fEdgeScale = 1.1f;
		break;
	}

	Setting_Monster(c, Type, PositionX, PositionY);

	return c;
}

CHARACTER *CreateHellGate(const mu_uint32 ThreadIndex, const mu_text *ID, mu_int32 Key, mu_int32 Index, mu_int32 x, mu_int32 y, mu_int32 CreateFlag)
{
	CHARACTER* portal = CreateMonster(0, Index, x, y, Key);
	portal->Level = Index - 152 + 1;
	mu_text Text[256];
	mu_sprintf_s(Text, mu_countof(Text), portal->ID, ID);

	if (portal->Level == 7)
	{
		portal->Object.SubType = 1;
	}

	mu_strcpy_s(portal->ID, mu_countof(portal->ID), Text);

	if (CreateFlag)
	{
		CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, portal->Object.Position, portal->Object.Position, portal->Object.Angle, 1, nullptr, 60.0f + GetLargeRand(10));
		CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, portal->Object.Position, portal->Object.Position, portal->Object.Angle, 1, nullptr, 50.0f + GetLargeRand(10));
		CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, portal->Object.Position, portal->Object.Position, portal->Object.Angle, 1, nullptr, 50.0f + GetLargeRand(10));
		CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, portal->Object.Position, portal->Object.Position, portal->Object.Angle, 1, nullptr, 60.0f + GetLargeRand(10));
	}

	return portal;
}

void DeleteCharacter(mu_int32 Key)
{
	mu_int16 tmpIndex = CharactersKeys[Key];
	CharactersKeys[Key] = -1;

	if (tmpIndex == -1)
		return;

	CHARACTER *c = &CharactersClient[tmpIndex];
	OBJECT *o = &c->Object;

	g_pSimpleChat->Delete(c);

	for (mu_uint32 n = 0; n < PartyNumber; ++n)
	{
		PARTY_t &member = Party[n];

		if (member.Key == Key)
		{
			member.index = -2;
			break;
		}
	}

	c->Key = -1;

	if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
	{
		o->Flags.Clear(OBJECT::eOF_LIVE);

		o->BoneTransform = nullptr;

		DeleteBug(o);
		giPetManager::DeletePet(c);
		DeleteCloth(c, o);
		DeleteParts(c);
		return;
	}
}

void DeleteCharacter(CHARACTER* c, OBJECT* o)
{
	g_pSimpleChat->Delete(c);

	for (mu_uint32 n = 0; n < PartyNumber; ++n)
	{
		PARTY_t &member = Party[n];

		if (member.Key == c->Key)
		{
			member.index = -2;
			break;
		}
	}

	if (c->Key != -1)
	{
		CharactersKeys[c->Key] = -1;
		c->Key = -1;
	}

	o->Flags.Clear(OBJECT::eOF_LIVE);

	DeleteBug(o);
	giPetManager::DeletePet(c);
	DeleteCloth(c, o);
	DeleteParts(c);
}

mu_float CharacterMoveSpeed(CHARACTER *c)
{
	OBJECT *o = &c->Object;
	mu_float Speed = (mu_float)c->MoveSpeed;
	if (o->Type == MODEL_PLAYER && o->Kind == KIND_PLAYER)
	{
#if 0 // FIX
		mu_boolean isholyitem = g_pCursedTempleWindow->CheckInventoryHolyItem(c);

		if (isholyitem)
		{
			c->Run = 40;
			Speed = 8;

			return Speed;
		}
#else
		mu_boolean isholyitem = false;
#endif

		if (c->Helper.Type == MODEL_HELPER + 37 && !c->SafeZone && !isholyitem)
		{
			if (c->Run < FENRIR_RUN_DELAY / 2)
				Speed = 15;
			else if (c->Run < FENRIR_RUN_DELAY)
				Speed = 16;
			else
			{
				if (c->Helper.ExcOption > 0)
					Speed = 19;
				else
					Speed = 17;
			}
		}
		else if (c->Helper.Type == MODEL_HELPER + 4 && !c->SafeZone && !isholyitem)
		{
			c->Run = 40;
			Speed = 17;
		}
		else if (!(c->Object.SubType == MODEL_CURSEDTEMPLE_ALLIED_PLAYER ||
				   c->Object.SubType == MODEL_CURSEDTEMPLE_ILLUSION_PLAYER) &&
			(c->Wing.Type != -1 ||
			 c->Helper.Type == MODEL_HELPER + 2 ||
			 c->Helper.Type == MODEL_HELPER + 3) &&
			!c->SafeZone &&
			!isholyitem)
		{
			if (c->Wing.Type == MODEL_WING + 5
				|| c->Wing.Type == MODEL_WING + 36)
			{
				c->Run = 40;
				Speed = 16;
			}
			else
			{
				c->Run = 40;
				Speed = 15;
			}
		}
		else if (!isholyitem)
		{
			if (c->Run < 40)
				Speed = 12;
			else
				Speed = 15;
		}
	}

	if (InChaosCastle() == true)
	{
		c->Run = 40;
		Speed = 15;
	}

	if (g_isCharacterBuff((&c->Object), eDeBuff_Freeze))
	{
		Speed *= 0.5f;
	}
	else if (g_isCharacterBuff((&c->Object), eDeBuff_BlowOfDestruction))
	{
		Speed *= 0.33f;
	}

	if (g_isCharacterBuff((&c->Object), eBuff_CursedTempleQuickness))
	{
		c->Run = 40;
		Speed = 20;
	}

	return Speed;
}

void MoveCharacterPosition(CHARACTER *c)
{
	OBJECT *o = &c->Object;
	EBone Matrix;
	AngleMatrix(o->Angle, Matrix);
	EVector3 v, Velocity;
	Vector(0.0f, -CharacterMoveSpeed(c), 0.0f, v);
	VectorRotate(v, Matrix, Velocity);
	VectorAdd(o->Position, Velocity, o->Position);

	if (GET_WORLD == -1 ||
		c->Helper.Type != MODEL_HELPER + 3 ||
		c->SafeZone)
	{
		o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
	}
	else
	{
		if (GET_WORLD == WD_8TARKAN || GET_WORLD == WD_10HEAVEN)
			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 90.0f;
		else
			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 30.0f;
	}

	if (o->Type == MODEL_MONSTER01 + 2)
	{
		o->Position[2] += -EMath::Abs(EMath::Sin(o->Timer))*70.0f + 70.0f;
	}

	o->Timer += 0.15f;
}

void SetMonsterSound(mu_int32 Type, mu_int32 s1, mu_int32 s2, mu_int32 s3, mu_int32 s4, mu_int32 s5, mu_int32 s6 = -1, mu_int32 s7 = -1, mu_int32 s8 = -1, mu_int32 s9 = -1, mu_int32 s10 = -1)
{
	MUModel *b = &MODELS::Data[Type];
	b->_Sounds[0] = s1;
	b->_Sounds[1] = s2;
	b->_Sounds[2] = s3;
	b->_Sounds[3] = s4;
	b->_Sounds[4] = s5;
	b->_Sounds[5] = s6;
	b->_Sounds[6] = s7;
	b->_Sounds[7] = s8;
	b->_Sounds[8] = s9;
	b->_Sounds[9] = s10;
}

void SetMonstersSounds()
{
	SetMonsterSound(MODEL_MONSTER01 + 0, SOUND_BULL_ACTION1, SOUND_BULL_ACTION2, SOUND_BULL_ATTACK1, SOUND_BULL_ATTACK2, SOUND_BULL_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 1, SOUND_HOUND_ACTION1, SOUND_HOUND_ACTION2, SOUND_HOUND_ATTACK1, SOUND_HOUND_ATTACK2, SOUND_HOUND_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 2, SOUND_BUDGE_DRAGON_ACTION1, SOUND_BUDGE_DRAGON_ATTACK1, SOUND_BUDGE_DRAGON_ATTACK1, SOUND_BUDGE_DRAGON_ATTACK1, SOUND_BUDGE_DRAGON_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 3, SOUND_DARK_KNIGHT_ACTION1, SOUND_DARK_KNIGHT_ACTION2, SOUND_DARK_KNIGHT_ATTACK1, SOUND_DARK_KNIGHT_ATTACK2, SOUND_DARK_KNIGHT_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 4, SOUND_LICH_ACTION1, SOUND_LICH_ACTION2, SOUND_LICH_ATTACK1, SOUND_LICH_ATTACK2, SOUND_LICH_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 5, SOUND_GIANT_ACTION1, SOUND_GIANT_ACTION2, SOUND_GIANT_ATTACK1, SOUND_GIANT_ATTACK2, SOUND_GIANT_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 6, SOUND_LARVA_ACTION1, SOUND_LARVA_ACTION2, SOUND_LARVA_ACTION1, SOUND_LARVA_ACTION2, SOUND_LARVA_ACTION2);
	SetMonsterSound(MODEL_MONSTER01 + 7, SOUND_GHOST_ACTION1, SOUND_GHOST_ACTION2, SOUND_GHOST_ATTACK1, SOUND_GHOST_ATTACK2, SOUND_GHOST_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 8, SOUND_HELL_SPIDER_ACTION1, SOUND_HELL_SPIDER_ATTACK1, SOUND_HELL_SPIDER_ATTACK1, SOUND_HELL_SPIDER_ATTACK1, SOUND_HELL_SPIDER_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 9, SOUND_SPIDER_ACTION1, SOUND_SPIDER_ACTION1, SOUND_SPIDER_ACTION1, SOUND_SPIDER_ACTION1, SOUND_SPIDER_ACTION1);
	SetMonsterSound(MODEL_MONSTER01 + 10, SOUND_CYCLOPS_ACTION1, SOUND_CYCLOPS_ACTION2, SOUND_CYCLOPS_ATTACK1, SOUND_CYCLOPS_ATTACK2, SOUND_CYCLOPS_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 11, SOUND_GORGON_ACTION1, SOUND_GORGON_ACTION2, SOUND_GORGON_ATTACK1, SOUND_GORGON_ATTACK2, SOUND_GORGON_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 12, SOUND_YETI_ACTION1, SOUND_YETI_ACTION1, SOUND_YETI_ATTACK1, SOUND_YETI_ATTACK1, SOUND_YETI_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 13, SOUND_YETI_ACTION1, SOUND_YETI_ACTION2, SOUND_YETI_ATTACK1, SOUND_YETI_ATTACK1, SOUND_YETI_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 14, -1, -1, SOUND_ASSASSIN_ATTACK1, SOUND_ASSASSIN_ATTACK2, SOUND_ASSASSIN_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 15, SOUND_ICE_MONSTER_ACTION1, SOUND_ICE_MONSTER_ACTION2, SOUND_ICE_MONSTER_ACTION1, SOUND_ICE_MONSTER_ACTION1, SOUND_ICE_MONSTER_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 16, SOUND_HOMMERD_ACTION1, SOUND_HOMMERD_ACTION2, SOUND_HOMMERD_ATTACK1, SOUND_HOMMERD_ATTACK1, SOUND_HOMMERD_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 17, SOUND_WORM_ACTION1, SOUND_WORM_ACTION1, SOUND_WORM_DIE, SOUND_WORM_DIE, SOUND_WORM_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 18, SOUND_ICE_QUEEN_ACTION1, SOUND_ICE_QUEEN_ACTION2, SOUND_ICE_QUEEN_ATTACK1, SOUND_ICE_QUEEN_ATTACK2, SOUND_ICE_QUEEN_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 19, SOUND_GOBLIN_ACTION1, SOUND_GOBLIN_ACTION2, SOUND_GOBLIN_ATTACK1, SOUND_GOBLIN_ATTACK2, SOUND_GOBLIN_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 20, SOUND_SCORPION_ACTION1, SOUND_SCORPION_ACTION2, SOUND_SCORPION_ATTACK1, SOUND_SCORPION_ATTACK2, SOUND_SCORPION_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 21, SOUND_BEETLE_MONSTER_ACTION1, SOUND_BEETLE_MONSTER_ACTION1, SOUND_BETTLE_MONSTER_ATTACK1, SOUND_BETTLE_MONSTER_ATTACK1, SOUND_BETTLE_MONSTER_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 22, SOUND_HUNTER_ACTION1, SOUND_HUNTER_ACTION2, SOUND_HUNTER_ATTACK1, SOUND_HUNTER_ATTACK2, SOUND_HUNTER_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 23, SOUND_FOREST_MONSTER_ACTION1, SOUND_FOREST_MONSTER_ACTION2, SOUND_FOREST_MONSTER_ATTACK1, SOUND_FOREST_MONSTER_ATTACK2, SOUND_FOREST_MONSTER_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 24, SOUND_AGON_ACTION1, SOUND_AGON_ACTION2, SOUND_AGON_ATTACK1, SOUND_AGON_ATTACK2, SOUND_AGON_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 25, SOUND_GOLEM_ACTION1, SOUND_GOLEM_ACTION2, SOUND_GOLEM_ATTACK1, SOUND_GOLEM_ATTACK2, SOUND_GOLEM_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 26, SOUND_YETI_ACTION1, SOUND_YETI_ACTION1, SOUND_DEVIL_ATTACK1, SOUND_DEVIL_ATTACK1, SOUND_YETI_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 27, SOUND_BALROG_ACTION1, SOUND_BALROG_ACTION2, SOUND_LICH_ATTACK2, SOUND_GORGON_ATTACK2, SOUND_BALROG_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 28, SOUND_SHADOW_ACTION1, SOUND_SHADOW_ACTION2, SOUND_SHADOW_ATTACK1, SOUND_SHADOW_ATTACK2, SOUND_SHADOW_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 29, SOUND_DARK_KNIGHT_ACTION1, SOUND_DARK_KNIGHT_ACTION2, SOUND_DARK_KNIGHT_ATTACK1, SOUND_DARK_KNIGHT_ATTACK2, SOUND_DARK_KNIGHT_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 30, SOUND_BULL_ACTION1, SOUND_BULL_ACTION2, SOUND_BULL_ATTACK1, SOUND_BULL_ATTACK2, SOUND_BULL_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 31, SOUND_YETI_ACTION1, SOUND_YETI_ACTION1, SOUND_BULL_ATTACK1, SOUND_BULL_ATTACK1, SOUND_YETI_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 32, SOUND_BALI_ACTION1, SOUND_BALI_ACTION2, SOUND_BALI_ATTACK1, SOUND_BALI_ATTACK2, SOUND_BALI_ACTION2);
	SetMonsterSound(MODEL_MONSTER01 + 33, SOUND_GREAT_BAHAMUT_ACTION1, SOUND_GREAT_BAHAMUT_ACTION1, SOUND_YETI_ACTION1, SOUND_YETI_ACTION1, SOUND_GREAT_BAHAMUT_ACTION1);
	SetMonsterSound(MODEL_MONSTER01 + 34, SOUND_VEPAR_ACTION1, SOUND_VEPAR_ACTION2, SOUND_GOLEM_DIE, SOUND_GOLEM_DIE, SOUND_VEPAR_ACTION2);
	SetMonsterSound(MODEL_MONSTER01 + 35, SOUND_VALKYRIE_ACTION1, SOUND_VALKYRIE_ACTION1, SOUND_BALI_ATTACK2, SOUND_BALI_ATTACK2, SOUND_VALKYRIE_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 36, SOUND_LIZARD_KING_ACTION1, SOUND_LIZARD_KING_ACTION2, SOUND_LIZARD_KING_ACTION1, SOUND_LIZARD_KING_ACTION2, SOUND_GORGON_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 37, SOUND_HYDRA_ACTION1, SOUND_HYDRA_ACTION1, SOUND_HYDRA_ATTACK1, SOUND_HYDRA_ATTACK1, SOUND_HYDRA_ACTION1);
	SetMonsterSound(MODEL_MONSTER01 + 39, SOUND_DARK_KNIGHT_ACTION1, SOUND_DARK_KNIGHT_ACTION2, SOUND_DARK_KNIGHT_ATTACK1, SOUND_DARK_KNIGHT_ATTACK2, SOUND_DARK_KNIGHT_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 40, SOUND_LIZARD_KING_ACTION1, SOUND_LIZARD_KING_ACTION2, SOUND_LIZARD_KING_ACTION1, SOUND_LIZARD_KING_ACTION2, SOUND_GORGON_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 41, SOUND_WHEEL_ACTION1, SOUND_WHEEL_ACTION1, SOUND_WHEEL_ATTACK1, SOUND_WHEEL_ATTACK1, SOUND_WHEEL_ATTACK1);
	SetMonsterSound(MODEL_MONSTER01 + 42, SOUND_TANTALOS_ACTION1, SOUND_TANTALOS_ACTION2, SOUND_TANTALOS_ATTACK1, SOUND_TANTALOS_ATTACK2, SOUND_TANTALOS_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 43, SOUND_BLOODY_WOLF_ACTION1, SOUND_BLOODY_WOLF_ACTION1, SOUND_BLOODY_WOLF_ATTACK1, SOUND_BLOODY_WOLF_ATTACK2, SOUND_BLOODY_WOLF_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 44, SOUND_BEAM_KNIGHT_ACTION1, SOUND_BEAM_KNIGHT_ACTION1, SOUND_BEAM_KNIGHT_ATTACK1, SOUND_BEAM_KNIGHT_ATTACK1, SOUND_BEAM_KNIGHT_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 45, SOUND_MUTANT_ACTION1, SOUND_MUTANT_ACTION2, SOUND_MUTANT_ATTACK1, SOUND_MUTANT_ATTACK1, SOUND_MUTANT_ATTACK1);
	SetMonsterSound(MODEL_MONSTER01 + 46, -1, -1, SOUND_ORC_ARCHER_ATTACK1, SOUND_ORC_ARCHER_ATTACK1, SOUND_BULL_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 47, SOUND_HUNTER_ACTION2, SOUND_HUNTER_ACTION2, SOUND_ORC_CAP_ATTACK1, SOUND_ORC_CAP_ATTACK1, SOUND_BULL_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 48, SOUND_CURSED_KING_ACTION1, SOUND_CURSED_KING_ACTION2, -1, -1, SOUND_CURSED_KING_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 50, SOUND_ALQUAMOS_ATTACK1, SOUND_ALQUAMOS_ATTACK1, SOUND_ALQUAMOS_ATTACK1, SOUND_ALQUAMOS_ATTACK1, SOUND_ALQUAMOS_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 51, SOUND_QUEEN_RAINNER_ACTION1, -1, SOUND_QUEEN_RAINNER_ATTACK1, SOUND_QUEEN_RAINNER_ATTACK1, SOUND_QUEEN_RAINNER_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 52, SOUND_MEGA_CRUST_ACTION1, SOUND_MEGA_CRUST_ACTION1, SOUND_MEGA_CRUST_ATTACK1, SOUND_MEGA_CRUST_ATTACK1, SOUND_MEGA_CRUST_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 53, SOUND_PHANTOM_KNIGHT_ACTION1, SOUND_PHANTOM_KNIGHT_ACTION1, SOUND_PHANTOM_KNIGHT_ATTACK1, SOUND_PHANTOM_KNIGHT_ATTACK1, SOUND_PHANTOM_KNIGHT_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 54, SOUND_DRAKAN_ACTION1, SOUND_DRAKAN_ACTION1, SOUND_DRAKAN_ATTACK1, SOUND_DRAKAN_ATTACK1, SOUND_DRAKAN_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 55, SOUND_DARKNESS_PHOENIX_ACTION1, SOUND_DARKNESS_PHOENIX_ACTION1, SOUND_DARKNESS_PHOENIX_ATTACK1, SOUND_DARKNESS_PHOENIX_ATTACK1, -1);
	SetMonsterSound(MODEL_MONSTER01 + 57, SOUND_RED_SKELETON_ACTION1, -1, SOUND_RED_SKELETON_ATTACK1, -1, SOUND_RED_SKELETON_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 58, SOUND_HUNTER_ACTION2, -1, SOUND_DARK_SKULL_ATTACK1, -1, SOUND_GIANT_OGRE_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 59, SOUND_HUNTER_ACTION2, -1, SOUND_DARK_SKULL_ATTACK1, -1, SOUND_DARK_SKULL_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 62, SOUND_MAGIC_SKELETON_ACTION1, -1, -1, -1, SOUND_MAGIC_SKELETON_ACTION1);
	SetMonsterSound(MODEL_MONSTER01 + 63, SOUND_DEATH_ANGEL_ACTION1, SOUND_DEATH_ANGEL_ACTION1, SOUND_DEATH_ANGEL_ATTACK1, SOUND_DEATH_ANGEL_ATTACK1, SOUND_DEATH_ANGEL_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 64, SOUND_KUNDUN_ACTION1, SOUND_KUNDUN_ACTION1, SOUND_KUNDUN_ATTACK1, SOUND_KUNDUN_ATTACK2, -1);
	SetMonsterSound(MODEL_MONSTER01 + 65, SOUND_BLOODY_SOLDIER_ACTION1, SOUND_BLOODY_SOLDIER_ACTION2, SOUND_BLOODY_SOLDIER_ATTACK1, SOUND_BLOODY_SOLDIER_ATTACK2, SOUND_BLOODY_SOLDIER_DIED);
	SetMonsterSound(MODEL_MONSTER01 + 66, SOUND_AEGIS_ACTION1, SOUND_AEGIS_ACTION1, SOUND_AEGIS_ATTACK1, SOUND_AEGIS_ATTACK2, SOUND_AEGIS_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 67, SOUND_CENTURION_ACTION1, SOUND_CENTURION_ACTION2, SOUND_CENTURION_ATTACK1, SOUND_CENTURION_ATTACK2, SOUND_CENTURION_DIE1, SOUND_CENTURION_ACTION3, SOUND_CENTURION_ACTION4, SOUND_CENTURION_ATTACK3, SOUND_CENTURION_ATTACK4, SOUND_CENTURION_DIE2);
	SetMonsterSound(MODEL_MONSTER01 + 68, SOUND_NECRON_ACTION1, SOUND_NECRON_ACTION2, SOUND_NECRON_ATTACK1, SOUND_NECRON_ATTACK2, SOUND_NECRON_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 69, SOUND_SCHRIKER_ACTION1, SOUND_SCHRIKER_ACTION2, SOUND_SCHRIKER_ATTACK1, SOUND_SCHRIKER_ATTACK2, SOUND_SCHIKER_DIE1, SOUND_SCHRIKER_ACTION3, SOUND_SCHRIKER_ACTION4, SOUND_SCHRIKER_ATTACK3, SOUND_SCHRIKER_ATTACK4, SOUND_SCHRIKER_DIE2);
	SetMonsterSound(MODEL_MONSTER01 + 70, -1, -1, SOUND_ORC_CAP_ATTACK1, SOUND_ORC_CAP_ATTACK1, -1);
	SetMonsterSound(MODEL_MONSTER01 + 71, -1, -1, SOUND_ORC_CAP_ATTACK1, SOUND_ORC_CAP_ATTACK1, -1);
	SetMonsterSound(MODEL_MONSTER01 + 72, -1, -1, SOUND_ORC_ARCHER_ATTACK1, SOUND_ORC_ARCHER_ATTACK1, -1);
	SetMonsterSound(MODEL_MONSTER01 + 73, -1, -1, -1, -1, SOUND_CASTLE_GATE_DESTROYED);
	SetMonsterSound(MODEL_MONSTER01 + 76, -1, -1, SOUND_BOW_MERCENARY_ATTACK1, SOUND_BOW_MERCENARY_ATTACK1, SOUND_BOW_MERCENARY_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 77, -1, -1, SOUND_SPEAR_MERCENARY_ATTACK1, SOUND_SPEAR_MERCENARY_ATTACK1, SOUND_SPEAR_MERCENARY_DIE);
	SetMonsterSound(MODEL_MONSTER01 + 156, SOUND_GOBLIN_ACTION1, SOUND_GOBLIN_ACTION2, SOUND_GOBLIN_ATTACK1, SOUND_GOBLIN_ATTACK2, SOUND_GOBLIN_DIE);
}

void RenderProtectGuildMark(const mu_uint32 ThreadIndex, CHARACTER * c)
{
	if (c->GuildType == GAME::G_TYPE_GUARD)
	{
		if (c->ProtectGuildMarkWorldTime == 0 || FWorldTime - c->ProtectGuildMarkWorldTime > 5000.0f)
		{
			OBJECT * o = &c->Object;
			CreateEffect(ThreadIndex, MODEL_PROTECTGUILD, o->Position, o->Angle, o->Light, 0, o);
			c->ProtectGuildMarkWorldTime = FWorldTime;
		}
	}
}

const mu_text *GetCharacterClassText(const mu_uint32 byClass)
{
	mu_uint8 byCharacterClass = GetCharacterClass(byClass);

	if (byCharacterClass == GAME::CLASS_WIZARD)
	{
		return GlobalText[20];
	}
	else if (byCharacterClass == GAME::CLASS_SOULMASTER)
	{
		return GlobalText[25];
	}
	else if (byCharacterClass == GAME::CLASS_GRANDMASTER)
	{
		return GlobalText[1669];
	}
	else if (byCharacterClass == GAME::CLASS_KNIGHT)
	{
		return GlobalText[21];
	}
	else if (byCharacterClass == GAME::CLASS_BLADEKNIGHT)
	{
		return GlobalText[26];
	}
	else if (byCharacterClass == GAME::CLASS_BLADEMASTER)
	{
		return GlobalText[1668];
	}
	else if (byCharacterClass == GAME::CLASS_ELF)
	{
		return GlobalText[22];
	}
	else if (byCharacterClass == GAME::CLASS_MUSEELF)
	{
		return GlobalText[27];
	}
	else if (byCharacterClass == GAME::CLASS_HIGHELF)
	{
		return GlobalText[1670];
	}
	else if (byCharacterClass == GAME::CLASS_DARK)
	{
		return GlobalText[23];
	}
	else if (byCharacterClass == GAME::CLASS_DUELMASTER)
	{
		return GlobalText[1671];
	}
	else if (byCharacterClass == GAME::CLASS_DARK_LORD)
	{
		return GlobalText[24];
	}
	else if (byCharacterClass == GAME::CLASS_LORDEMPEROR)
	{
		return GlobalText[1672];
	}
	else if (byCharacterClass == GAME::CLASS_SUMMONER)
	{
		return GlobalText[1687];
	}
	else if (byCharacterClass == GAME::CLASS_BLOODYSUMMONER)
	{
		return GlobalText[1688];
	}
	else if (byCharacterClass == GAME::CLASS_DIMENSIONMASTER)
	{
		return GlobalText[1689];
	}

	return GlobalText[2305];
}

void SetPlayerColor(mu_uint8 PK, mu_uint32 &Color)
{
	switch (PK)
	{
	case 0:Color = MU::MakeARGB(150, 255, 240, 255); break;
	case 1:Color = MU::MakeARGB(100, 120, 255, 255); break;
	case 2:Color = MU::MakeARGB(140, 180, 255, 255); break;
	case 3:Color = MU::MakeARGB(200, 220, 255, 255); break;
	case 4:Color = MU::MakeARGB(255, 150, 60, 255); break;
	case 5:Color = MU::MakeARGB(255, 80, 30, 255); break;
	default:Color = MU::MakeARGB(255, 0, 0, 255); break;
	}
}

mu_uint32 GetGuildRelationShipTextColor(mu_uint8 GuildRelationShip)
{
	mu_uint32 dwColor = 0;

	if (GuildRelationShip == GAME::GR_NONE)
		dwColor = MU::MakeARGB(230, 230, 255, 255);
	else if (GuildRelationShip == GAME::GR_RIVAL || GuildRelationShip == GAME::GR_RIVALUNION)
		dwColor = MU::MakeARGB(255, 30, 0, 255);
	else
		dwColor = MU::MakeARGB(200, 255, 0, 255);

	return dwColor;
}

mu_uint32 GetGuildRelationShipBGColor(mu_uint8 GuildRelationShip)
{
	mu_uint32 dwColor = 0;

	if (GuildRelationShip == GAME::GR_NONE)
		dwColor = MU::MakeARGB(10, 30, 50, 150);
	else if (GuildRelationShip == GAME::GR_RIVAL || GuildRelationShip == GAME::GR_RIVALUNION)
		dwColor = MU::MakeARGB(0, 0, 0, 150);
	else
		dwColor = MU::MakeARGB(20, 50, 80, 150);

	return dwColor;
}

void ChangeCharacterExt(mu_int32 Key, SHARED::CHARSET_PACKET Equipment[GAME::MAX_EQUIPMENT], CHARACTER * pCharacter, OBJECT * pHelper)
{
	CHARACTER *c;
	if (pCharacter == nullptr)
		c = &CharactersClient[Key];
	else
		c = pCharacter;

	OBJECT *o = &c->Object;
	if (o->Type != MODEL_PLAYER)
		return;

	SHARED::CHARSET_PACKET &RightWeapon = Equipment[GAME::EQUIPMENT_WEAPON_RIGHT];
	SHARED::CHARSET_PACKET &LeftWeapon = Equipment[GAME::EQUIPMENT_WEAPON_LEFT];

	if (RightWeapon.Enabled == false)
	{
		c->Weapon[0].Type = -1;
		c->Weapon[0].Level = 0;
		c->Weapon[0].ExcOption = 0;
		c->Weapon[0].SetOption = 0;
	}
	else
	{
		c->Weapon[0].Type = MODEL_ITEM + (RightWeapon.Type * MAX_ITEM_INDEX) + RightWeapon.Index;
		c->Weapon[0].Level = RightWeapon.Level;
		c->Weapon[0].ExcOption = RightWeapon.ExcOption;
		c->Weapon[0].SetOption = RightWeapon.SetOption;
	}

	if (LeftWeapon.Enabled == false)
	{
		c->Weapon[1].Type = -1;
		c->Weapon[1].Level = 0;
		c->Weapon[1].ExcOption = 0;
		c->Weapon[1].SetOption = 0;
	}
	else
	{
		mu_int16 ItemType = (LeftWeapon.Type * MAX_ITEM_INDEX) + LeftWeapon.Index;

		if (GetBaseClass(c->Class) == GAME::CLASS_DARK_LORD &&
			ItemType == ITEM_HELPER + 5)
		{
			ITEM* pEquipmentItemSlot = &CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT];
			giPetManager::CreatePetDarkSpirit(c);
			if (!InChaosCastle())
			{
				c->m_pPet->SetPetItem(pEquipmentItemSlot);
			}

			c->Weapon[1].Type = MODEL_ITEM + (LeftWeapon.Type * MAX_ITEM_INDEX) + LeftWeapon.Index;
			c->Weapon[1].Level = 0;
			c->Weapon[1].ExcOption = 0;
			c->Weapon[1].SetOption = 0;
		}
		else
		{
			c->Weapon[1].Type = MODEL_ITEM + (LeftWeapon.Type * MAX_ITEM_INDEX) + LeftWeapon.Index;
			c->Weapon[1].Level = LeftWeapon.Level;
			c->Weapon[1].ExcOption = LeftWeapon.ExcOption;
			c->Weapon[1].SetOption = LeftWeapon.SetOption;
		}
	}

	SHARED::CHARSET_PACKET &Wings = Equipment[GAME::EQUIPMENT_WING];

	c->Wing.Type = -1;
	c->Wing.Level = 0;
	c->Wing.ExcOption = 0;
	c->Wing.SetOption = 0;

	if (Wings.Enabled == true)
	{
		c->Wing.Type = MODEL_ITEM + (Wings.Type * MAX_ITEM_INDEX) + Wings.Index;
	}

	if (pHelper == nullptr)
	{
		DeleteBug(o);
		PetProcess::DeletePet(c, c->Helper.Type, true);
	}
	else
	{
		pHelper->Flags.Clear(OBJECT::eOF_LIVE);
	}

	SHARED::CHARSET_PACKET &Helper = Equipment[GAME::EQUIPMENT_HELPER];

	c->Helper.Type = -1;
	c->Helper.Level = 0;
	c->Helper.ExcOption = 0;
	c->Helper.SetOption = 0;

	if (Helper.Enabled == true)
	{
		mu_int16 ItemType = (Helper.Type * MAX_ITEM_INDEX) + Helper.Index;
		c->Helper.Type = MODEL_ITEM + ItemType;
		c->Helper.ExcOption = Helper.ExcOption;

		switch (c->Helper.Type)
		{
		case MODEL_HELPER + 0:
			{
				CreateBug(MODEL_HELPER, o->Position, o);
			}
			break;

		case MODEL_HELPER + 2:
			{
				CreateBug(MODEL_UNICON, o->Position, o);
			}
			break;

		case MODEL_HELPER + 3:
			{
				CreateBug(MODEL_PEGASUS, o->Position, o);
			}
			break;

		case MODEL_HELPER + 4:
			{
				CreateBug(MODEL_DARK_HORSE, o->Position, o);
			}
			break;

		case MODEL_HELPER + 37:
			{
				switch (c->Helper.ExcOption)
				{
				default:
				case 0:
					{
						CreateBug(MODEL_FENRIR_RED, o->Position, o);
					}
					break;
				case 1:
					{
						CreateBug(MODEL_FENRIR_BLACK, o->Position, o);
					}
					break;
				case 2:
					{
						CreateBug(MODEL_FENRIR_BLUE, o->Position, o);
					}
					break;
				case 4:
					{
						CreateBug(MODEL_FENRIR_GOLD, o->Position, o);
					}
					break;
				}
			}
			break;

		case MODEL_HELPER + 64:
			{
				PetProcess::CreatePet(ItemType, c->Helper.Type, o->Position, c);
			}
			break;

		case MODEL_HELPER + 65:
			{
				PetProcess::CreatePet(ItemType, c->Helper.Type, o->Position, c);
			}
			break;

		case MODEL_HELPER + 67:
			{
				PetProcess::CreatePet(ItemType, c->Helper.Type, o->Position, c);
			}
			break;

		case MODEL_HELPER + 80:
			{
				PetProcess::CreatePet(ItemType, c->Helper.Type, o->Position, c);
			}
			break;
			
		case MODEL_HELPER + 106:
			{
				PetProcess::CreatePet(ItemType, c->Helper.Type, o->Position, c);
			}
			break;

		case MODEL_HELPER + 123:
			{
				PetProcess::CreatePet(ItemType, c->Helper.Type, o->Position, c);
			}
			break;
		}
	}

	DeleteParts(c);
	// FIX add parts code here dunno what equipment part it is

	if (c->Change)
		return;

	mu_int16 code[MAX_BODYPART - 1] =
	{
		MODEL_BODY_HELM,
		MODEL_BODY_ARMOR,
		MODEL_BODY_PANTS,
		MODEL_BODY_GLOVES,
		MODEL_BODY_BOOTS
	};

	for (mu_uint32 n = BODYPART_HELM; n < MAX_BODYPART; ++n)
	{
		mu_uint32 j = GAME::EQUIPMENT_HELM + n - 1;

		SHARED::CHARSET_PACKET &charset = Equipment[j];
		PART_t &part = c->BodyPart[n];

		if (charset.Enabled == false)
		{
			part.Type = code[n - 1] + GetSkinModelIndex(c->Class);
			part.Level = 0;
			part.ExcOption = 0;
			part.SetOption = 0;
		}
		else
		{
			part.Type = MODEL_ITEM + (charset.Type * MAX_ITEM_INDEX) + charset.Index;
			part.Level = charset.Level;
			part.ExcOption = charset.ExcOption;
			part.SetOption = charset.SetOption;
		}
	}

	ChangeChaosCastleUnit(c);
	SetCharacterScale(c);
}

void GuildTeam(CHARACTER *c)
{
	c->GuildTeam = 0;

	if (Hero->GuildMarkIndex != -1 &&
		c->GuildMarkIndex == Hero->GuildMarkIndex)
	{
		c->GuildTeam = 1;
	}

	if (EnableGuildWar)
	{
		if (GuildWarIndex >= 0 &&
			c->GuildMarkIndex == GuildWarIndex)
		{
			c->GuildTeam = 2;
		}
	}
}