#include "stdafx.h"
#include "mu_petsystem.h"

CSPetSystem::CSPetSystem()
{
	m_pPetItem = nullptr;
}

CSPetSystem::~CSPetSystem()
{
	m_PetCharacter.Object.BoneTransform = nullptr;
}

void CSPetSystem::CreatePetPointer(mu_int32 Type, mu_uint8 PositionX, mu_uint8 PositionY, mu_float Rotation)
{
	CHARACTER*  c = &m_PetCharacter;
	OBJECT*     o = &c->Object;

	m_PetTarget = nullptr;
	m_byCommand = PET_CMD_DEFAULT;
	m_pPetItem = nullptr;

	o->Initialize();

	c->PositionX = PositionX;
	c->PositionY = PositionY;
	c->TargetX = PositionX;
	c->TargetY = PositionY;

	c->byExtensionSkill = 0;
	c->m_pPet = nullptr;

	mu_int32 Index = TERRAIN_INDEX_REPEAT(c->PositionX, c->PositionY);
	if ((TerrainWall[Index] & TW_SAFEZONE) == TW_SAFEZONE)
		c->SafeZone = true;
	else
		c->SafeZone = false;

	o->Flags.Clear();
	o->Flags.Set(OBJECT::eOF_LIVE | OBJECT::eOF_ALPHAENABLE | OBJECT::eOF_LIGHTENABLE);

	c->Path.PathNum = 0;
	c->Path.CurrentPath = 0;
	c->Movement = false;
	c->Dead = 0;
	c->Blood = false;
	c->GuildTeam = 0;
	c->Run = 0;
	c->GuildMarkIndex = -1;
	c->PK = PVP_NEUTRAL;
	o->Type = Type;
	o->Scale = 0.7f;
	o->Timer = 0.0f;
	o->Alpha = 1.0f;
	o->AlphaTarget = 1.0f;
	o->Velocity = 0.0f;
	o->ShadowScale = 0.0f;
	o->m_byHurtByOneToOne = 0;
	o->AI = 0;
	o->Velocity = 1.0f;
	o->Gravity = 13;
	c->ExtendState = 0;
	c->ExtendStateTime = 0;

	c->GuildStatus = GAME::G_NONE;
	c->GuildType = 0;
	c->ProtectGuildMarkWorldTime = 0.0f;

	c->m_byDieType = 0;
	o->SpecialSkillTick = 0;

	c->Class = 0;
	o->PriorAction = 0;
	o->CurrentAction = 0;
	o->AnimationFrame = 0.0f;
	o->PriorAnimationFrame = 0;
	c->JumpTime = 0;
	o->HiddenMesh = -1;
	c->MoveSpeed = 10;

	g_CharacterClearBuff(o);

	o->Teleport = TELEPORT_NONE;
	o->Kind = KIND_PET;
	c->Change = false;
	o->SubType = 0;
	c->MonsterIndex = -1;
	o->BlendMeshTexCoordU = 0.0f;
	o->BlendMeshTexCoordV = 0.0f;
	o->Position[0] = (mu_float)(PositionX*g_TerrainScale) + 0.5f*g_TerrainScale;
	o->Position[1] = (mu_float)(PositionY*g_TerrainScale) + 0.5f*g_TerrainScale;

	o->InitialSceneFrame = MoveSceneFrame;
	o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);

	Vector(0.0f, 0.0f, Rotation, o->Angle);
	Vector(0.5f, 0.5f, 0.5f, o->Light);
	Vector(-60.0f, -60.0f, 0.0f, o->BoundingBoxMin);
	switch (Type)
	{
	case MODEL_DARK_SPIRIT:
		Vector(50.0f, 50.0f, 150.0f, o->BoundingBoxMax);
		break;
	default:
		Vector(50.0f, 50.0f, 150.0f, o->BoundingBoxMax);
		break;
	}

	o->BoneTransform = nullptr;

	mu_int32 i;
	for (i = 0; i < 2; ++i)
	{
		c->Weapon[i].Type = -1;
		c->Weapon[i].Level = 0;
		c->Weapon[i].ExcOption = 0;
		c->Weapon[i].SetOption = 0;
	}
	for (i = 0; i < MAX_BODYPART; ++i)
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

	o->BlendMesh = -1;
	o->BlendMeshLight = 1.0f;

	const MUModel *b = &MODELS::Data[MODEL_PLAYER];

	switch (Type)
	{
	case MODEL_DARK_SPIRIT:
		c->Weapon[0].LinkBone = b->GetBoneIndex(0);
		c->Weapon[1].LinkBone = b->GetBoneIndex(0);
		break;
	default:
		c->Weapon[0].LinkBone = b->GetBoneIndex(0);
		c->Weapon[1].LinkBone = b->GetBoneIndex(0);
		break;
	}

	m_byCommand = PET_CMD_DEFAULT;
}

mu_boolean CSPetSystem::PlayAnimation(const mu_uint32 ThreadIndex, OBJECT* o)
{
	MUModel *b = &MODELS::Data[o->Type];
	mu_float playSpeed = 0.1;

	switch (m_PetType)
	{
	default: break;

	case PET_TYPE_DARK_SPIRIT:
		playSpeed = 0.4;
		break;
	}

	b->CurrentAction(ThreadIndex) = o->CurrentAction;
	return b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, playSpeed);
}

void CSPetSystem::SetAI(mu_int32 AI)
{
	m_PetCharacter.Object.AI = AI;
	m_PetCharacter.Object.LifeTime = 0;
}

void CSPetSystem::SetCommand(mu_int32 Key, mu_uint8 cmd)
{
	m_byCommand = cmd;
	if (m_PetCharacter.Object.AI != PET_ATTACK && m_PetCharacter.Object.AI != PET_ATTACK_MAGIC)
	{
		m_PetCharacter.Object.Flags.Clear(OBJECT::eOF_ACTIONSTART);
	}
	if (cmd == PET_CMD_TARGET)
	{
		mu_int32 Index = FindCharacterIndex(Key);
		m_PetTarget = &CharactersClient[Index];

		m_PetCharacter.Object.Flags.Set(OBJECT::eOF_ACTIONSTART);
	}
}

void CSPetSystem::SetAttack(mu_int32 Key, mu_int32 attackType)
{
	mu_int32 Index = FindCharacterIndex(Key);
	m_PetTarget = &CharactersClient[Index];
	OBJECT* Owner = &m_PetOwner->Object;

	if (g_isCharacterBuff(Owner, eDeBuff_Stun))
	{
		return;
	}
	else if (g_isCharacterBuff(Owner, eBuff_Cloaking))
	{
		m_PetCharacter.TargetCharacter = Index;
		SetAI(PET_ATTACK + attackType);
		return;
	}

	m_PetCharacter.TargetCharacter = Index;
	m_PetCharacter.AttackTime = 0;
	SetAI(PET_ATTACK + attackType);

	if (m_PetCharacter.Object.AI == PET_ATTACK)
	{
		OBJECT* o = &m_PetCharacter.Object;

		o->Flags.Set(OBJECT::eOF_ACTIONSTART);
		o->Velocity = GetLargeRand(10) + 20.0f;
		o->Gravity = 0.5f;

		PlayBackground(SOUND_DSPIRIT_RUSH);
	}
	else if (m_PetCharacter.Object.AI == PET_ATTACK_MAGIC)
	{
		PlayBackground(SOUND_DSPIRIT_MISSILE);
	}
}

void CSPetSystem::MoveInventory(void)
{
}

CSPetDarkSpirit::CSPetDarkSpirit(CHARACTER* c)
{
	m_PetType = PET_TYPE_DARK_SPIRIT;
	m_PetOwner = c;

	m_PetCharacter.Object.BoneTransform = nullptr;
	CreatePetPointer(MODEL_DARK_SPIRIT, (c->PositionX), (c->PositionY), 0.0f);

	m_PetCharacter.Object.Position[2] += 300.0f;
	m_PetCharacter.Object.CurrentAction = 0;
}

CSPetDarkSpirit::~CSPetDarkSpirit(void)
{
	m_PetCharacter.Object.BoneTransform = nullptr;
}

void CSPetDarkSpirit::MovePet(const mu_uint32 ThreadIndex)
{
	mu_boolean    Play;
	CHARACTER* c = &m_PetCharacter;
	OBJECT* o = &c->Object;
	OBJECT* Owner = &m_PetOwner->Object;

	if (g_isCharacterBuff(Owner, eDeBuff_Stun))
		return;

	MUModel* b = &MODELS::Data[Owner->Type];

	if (!g_DuelMgr.IsPetDuelEnabled())
	{
		m_byCommand = 0;
#if 0 // FIX
		SendRequestPetCommand(m_PetOwner->m_pPet->GetPetType(), AT_PET_COMMAND_DEFAULT, 0xffff);
#endif
		g_DuelMgr.EnablePetDuel(true);
	}

	if (c->TargetCharacter >= 0 && c->TargetCharacter < MAX_CHARACTERS_CLIENT)
	{
		CHARACTER* tc = &CharactersClient[c->TargetCharacter];

		if ((g_isCharacterBuff((&tc->Object), eBuff_Cloaking) || g_isCharacterBuff(Owner, eBuff_Cloaking))
			&& (o->AI == PET_ATTACK || o->AI == PET_ESCAPE || o->AI == PET_ATTACK_MAGIC))
		{
			CSPetDarkSpirit* pPet = m_PetOwner->m_pPet;

			mu_float dx = o->Position[0] - Owner->Position[0];
			mu_float dy = o->Position[1] - Owner->Position[1];
			mu_float Distance = EMath::Sqrt(dx*dx + dy*dy);

			c->TargetCharacter = -1;
#if 0 // FIX
			SendRequestPetCommand(pPet->GetPetType(), AT_PET_COMMAND_DEFAULT, 0xffff);
#endif
			SetAI(PET_STAND);
			if (Distance > 50 || (o->AI != PET_STAND_START && o->AI >= PET_FLYING && o->AI <= PET_STAND))
			{
				SetAI(PET_STAND_START);
				o->Velocity = 3.0f;
			}
		}
	}
	else if ((g_isCharacterBuff(Owner, eBuff_Cloaking))
		&& (o->AI == PET_ATTACK || o->AI == PET_ESCAPE || o->AI == PET_ATTACK_MAGIC))
	{
		CSPetDarkSpirit* pPet = m_PetOwner->m_pPet;

		mu_float dx = o->Position[0] - Owner->Position[0];
		mu_float dy = o->Position[1] - Owner->Position[1];
		mu_float Distance = EMath::Sqrt(dx*dx + dy*dy);

		c->TargetCharacter = -1;
#if 0 // FIX
		SendRequestPetCommand(pPet->GetPetType(), AT_PET_COMMAND_DEFAULT, 0xffff);
#endif
		SetAI(PET_STAND);
		if (Distance > 50 || (o->AI != PET_STAND_START && o->AI >= PET_FLYING && o->AI <= PET_STAND))
		{
			SetAI(PET_STAND_START);
			o->Velocity = 3.0f;
		}
	}

	if (m_PetOwner->SafeZone == true)
	{
		if (o->AI != PET_STAND && o->AI != PET_STAND_START)
		{
			mu_float dx = o->Position[0] - Owner->Position[0];
			mu_float dy = o->Position[1] - Owner->Position[1];
			mu_float Distance = EMath::Sqrt(dx*dx + dy*dy);

			SetAI(PET_STAND);
			if (Distance > 50 || (o->AI != PET_STAND_START && o->AI >= PET_FLYING && o->AI <= PET_STAND))
			{
				SetAI(PET_STAND_START);
				o->Velocity = 3.0f;
			}
		}
	}
	else if (o->AI == PET_STAND || o->AI == PET_STAND_START)
	{
		SetAI(PET_FLYING);
	}

	Play = PlayAnimation(ThreadIndex, o);
	if (Play == false)
	{
		switch (o->AI)
		{
		case PET_FLY:           SetAction(o, 0); break;
		case PET_FLYING:        SetAction(o, 1); break;
		case PET_STAND:         SetAction(o, 2); break;
		case PET_STAND_START:   SetAction(o, 1); break;
		case PET_ATTACK:
		case PET_ESCAPE:        SetAction(o, 3); break;
		default:               SetAction(o, 0); break;
		}
	}

	EVector3 Range, TargetPosition;
	mu_float FlyRange = 150.0f;

	if (o->Flags.Get(OBJECT::eOF_ACTIONSTART) == true)
	{
		OBJECT* to = &m_PetTarget->Object;

		VectorCopy(to->Position, TargetPosition);
		VectorSubtract(TargetPosition, o->Position, Range);
	}
	else
	{
		VectorCopy(Owner->Position, TargetPosition);
		VectorSubtract(TargetPosition, o->Position, Range);
	}

	if (o->AI == PET_FLY || o->AI == PET_FLYING)
	{
		o->Flags.Clear(OBJECT::eOF_ACTIONSTART);

		EVector3 p, Pos, Light;
		Vector(0.3f, 0.4f, 0.7f, Light);
		Vector(0.0f, 0.0f, 0.0f, p);

		if (!g_isCharacterBuff(Owner, eBuff_Cloaking))
		{
			for (mu_int32 i = 0; i < 1; ++i)
			{
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(GetLargeRand(66))], p, Pos);

				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Pos, o->Angle, Light, 5, 0.8f);
			}
		}

		mu_float Distance = Range[0] * Range[0] + Range[1] * Range[1];
		if (Distance >= FlyRange*FlyRange)
		{
			mu_float Angle = CreateAngle(o->Position[0], o->Position[1], TargetPosition[0], TargetPosition[1]);
			o->Angle[2] = TurnAngle2(o->Angle[2], Angle, GetLargeRand(15) + 5.0f);
		}

		EMatrix4 matrix;
		AngleMatrix(o->Angle, matrix);

		EVector3 Direction;
		VectorRotate(o->Direction, matrix, Direction);
		VectorAdd(o->Position, Direction, o->Position);

		mu_int32 speedRandom = 28;
		mu_int32 CharacterHeight = 250;

		if (m_PetOwner->Helper.Type == ITEM_HELPER + 4)
		{
			CharacterHeight = 350;
		}

		mu_float Height = TargetPosition[2] + CharacterHeight;
		if (o->Position[2] < Height)
		{
			speedRandom = 10;
			o->Angle[0] -= 2.0f;
			if (o->Angle[0] < -15.0f) o->Angle[0] = -15.0f;
		}
		else if (o->Position[2] > Height + 100)
		{
			speedRandom = 20;
			o->Angle[0] += 2.0f;
			if (o->Angle[0] > 15.0f) o->Angle[0] = 15.0f;
		}

		mu_float Speed = 0;
		if (GetLargeRand(speedRandom) == 0)
		{
			if (Distance >= FlyRange*FlyRange)
			{
				Speed = -(mu_float)(GetLargeRand(64) + 128)*0.1f;
			}
			else
			{
				Speed = -(mu_float)(GetLargeRand(8) + 32)*0.1f;
				o->Angle[2] += (mu_float)(GetLargeRand(60));
			}

			Speed += o->Direction[1];
			Speed = Speed * 0.5f;

			o->Direction[0] = 0.0f;
			o->Direction[1] = Speed;
			o->Direction[2] = (mu_float)(GetLargeRand(64) - 32)*0.1f;
		}

		if (o->Direction[1] < -12.0f)
		{
			if (o->AI != PET_FLYING)
			{
				SetAI(PET_FLYING);
			}
		}
		else if (o->AI != PET_FLY)
		{
			SetAI(PET_FLY);
		}
	}
	else if (o->AI == PET_ATTACK || o->AI == PET_ESCAPE)
	{
		EVector3 p, Pos;
		EMatrix4 matrix;

		AngleMatrix(o->Angle, matrix);
		Vector(0.0f, -o->Velocity, 0.0f, p);
		VectorRotate(p, matrix, Pos);
		VectorAdd(o->Position, Pos, o->Position);

		if (o->AI == PET_ATTACK)
		{
			TargetPosition[2] += 50.0f;
			mu_float Distance = MoveHumming(o->Position, o->Angle, TargetPosition, o->Velocity);
			if (Distance < 20 || o->LifeTime>20)
			{
				SetAI(PET_ESCAPE);
				o->Angle[0] = -45.0f;
				if (m_byCommand != PET_CMD_TARGET)
				{
					o->Flags.Clear(OBJECT::eOF_ACTIONSTART);
				}
			}
			o->Velocity += o->Gravity;
			o->Gravity += 0.2f;
			++o->LifeTime;
		}
		else if (o->AI == PET_ESCAPE)
		{
			mu_float Distance = Range[0] * Range[0] + Range[1] * Range[1];
			if (Distance >= (FlyRange + 100)*(FlyRange + 100))
			{
				SetAI(PET_FLYING);
			}
			o->Velocity -= 1.0f;
		}
		SetAction(o, 3);
	}
	else if (o->AI == PET_ATTACK_MAGIC)
	{
		if (c->TargetCharacter != -1)
		{
			CHARACTER* tc = &CharactersClient[c->TargetCharacter];
			OBJECT* to = &tc->Object;

			mu_float Angle = CreateAngle(o->Position[0], o->Position[1], to->Position[0], to->Position[1]);
			o->Angle[2] = TurnAngle2(o->Angle[2], Angle, 40.0f);
		}
	}

	if (o->AI >= PET_ATTACK && o->AI <= PET_ATTACK_MAGIC)
	{
		if (++c->AttackTime >= 15)
		{
			c->AttackTime = 15;
		}
	}

	if ((GetLargeRand(100)) == 0 && (MoveSceneFrame % 60) == 0)
	{
		PlayObject(SOUND_DSPIRIT_SHOUT, o);
	}

	if(o->AI != PET_STAND &&
		o->AI != PET_STAND_START)
	{
		VectorCopy(m_PetOwner->Object.Position, TargetPosition);
		VectorSubtract(TargetPosition, o->Position, Range);
		mu_float Distance = Range[0] * Range[0] + Range[1] * Range[1];
		if (o->Position[2]<(TargetPosition[2] - 200.0f) || Distance>409600.0f)
		{
			++o->LifeTime;
		}
		if (o->LifeTime > 90)
		{
			o->LifeTime = 0;
			VectorCopy(TargetPosition, o->Position);
			o->Position[2] += 250.0f;
		}
	}
}

void CSPetDarkSpirit::RenderPetInventory(void)
{
	RenderCmdType();
}

void CSPetDarkSpirit::RenderPet(const mu_uint32 ThreadIndex, mu_int32 PetState)
{
	CHARACTER* c = &m_PetCharacter;
	OBJECT* o = &c->Object;

	if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
	{
		const mu_boolean isVisible = TestFrustrum2D(o->Position, 100.0f);
		if (isVisible == true)
		{
			o->Flags.Set(OBJECT::eOF_VISIBLE);
		}
		else
		{
			o->Flags.Clear(OBJECT::eOF_VISIBLE);
		}

		if (isVisible == true)
		{
			if (m_PetOwner->Object.Type != MODEL_PLAYER && o->Type != MODEL_DARK_SPIRIT)
				return;

			OBJECT* Owner = &m_PetOwner->Object;
			MUModel* b = &MODELS::Data[Owner->Type];
			EVector3 Range, TargetPosition;

			for (mu_uint32 n = 0; n < MU_UPDATECOUNTER; ++n)
			{
				if (o->Flags.Get(OBJECT::eOF_ACTIONSTART) == true)
				{
					OBJECT* to = &m_PetTarget->Object;

					VectorCopy(to->Position, TargetPosition);
					VectorSubtract(TargetPosition, o->Position, Range);
				}
				else
				{
					VectorCopy(Owner->Position, TargetPosition);
					VectorSubtract(TargetPosition, o->Position, Range);
				}

				if (o->AI == PET_STAND || o->AI == PET_STAND_START)
				{
					if (o->AI == PET_STAND)
					{
						EVector3 p;

						Vector(-10.0f, 0.0f, 10.0f, p);
						b->TransformPosition(ThreadIndex, Owner->BoneTransform[b->GetBoneIndex(37)], p, o->Position, true);
						VectorCopy(Owner->Angle, o->Angle);
						o->Angle[2] -= 120.0f;
					}
					else if (o->AI == PET_STAND_START)
					{
						EVector3 p, Pos;

						if (o->Velocity != 0.0f)
						{
							EMatrix4 matrix;
							AngleMatrix(o->Angle, matrix);
							Vector(0.0f, -o->Velocity, 0.0f, p);
							VectorRotate(p, matrix, Pos);
							VectorAdd(o->Position, Pos, o->Position);
						}

						Vector(0.0f, 0.0f, 0.0f, p);
						b->TransformPosition(ThreadIndex, Owner->BoneTransform[b->GetBoneIndex(42)], p, Pos, true);

						mu_float Distance = MoveHumming(o->Position, o->Angle, Pos, o->Velocity);
						++o->Velocity;
						if (Distance < 50)
						{
							SetAI(PET_STAND);
						}
					}

					VectorCopy(m_PetOwner->Object.Position, TargetPosition);
					VectorSubtract(TargetPosition, o->Position, Range);
					mu_float Distance = Range[0] * Range[0] + Range[1] * Range[1];
					if (o->Position[2]<(TargetPosition[2] - 200.0f) || Distance>409600.0f)
					{
						++o->LifeTime;
					}
					if (o->LifeTime > 90)
					{
						o->LifeTime = 0;
						VectorCopy(TargetPosition, o->Position);
						o->Position[2] += 250.0f;
					}
				}
			}

			RenderObject(ThreadIndex, o, false, 0, PetState);

			if (MU_UPDATECOUNTER > 0)
			{
				AttackEffect(ThreadIndex, c, o);
			}
		}
		else
		{
			o->Velocity = 3.0f;
			if (o->AI != PET_STAND_START)
			{
				o->AI = PET_FLYING;
			}
		}
	}
}

void CSPetDarkSpirit::Eff_LevelUp(const mu_uint32 ThreadIndex)
{
	OBJECT* o = &m_PetCharacter.Object;

	EVector3 Angle = { 0.0f, 0.0f, 0.0f };
	EVector3 Position = { o->Position[0], o->Position[1], o->Position[2] };

	for (mu_int32 i = 0; i < 5; ++i)
	{
		CreateJoint(ThreadIndex, BITMAP_FLARE, Position, Position, Angle, 0, o, 40, 2);
	}
}

void CSPetDarkSpirit::Eff_LevelDown(const mu_uint32 ThreadIndex)
{
	OBJECT* o = &m_PetCharacter.Object;

	EVector3 Position = { o->Position[0], o->Position[1], o->Position[2] };

	for (mu_int32 i = 0; i < 15; ++i)
	{
		CreateJoint(ThreadIndex, BITMAP_FLARE, Position, o->Position, o->Angle, 0, o, 40, 2);
	}
}

void CSPetDarkSpirit::RenderCmdType(void)
{
#if 0 // FIX
	mu_float   x, y, Width, Height;
	mu_float   PartyWidth = 0.0f;

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(220, 220, 220, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 128);

	if (PartyNumber > 0)
	{
		PartyWidth = 50.0f;
	}
	if ((Hero->Helper.Type >= MODEL_HELPER + 0 && Hero->Helper.Type <= MODEL_HELPER + 4)
		|| Hero->Helper.Type == MODEL_HELPER + 37
		)
	{
		PartyWidth += 60.0f;
	}


	mu_int32 Dur = 255;
	Width = 50; Height = 2; x = GetScreenWidth() - Width - PartyWidth - 15; y = 4;
	mu_int32 Life = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Durability*(mu_int32)Width / Dur;

	EnableAlphaTest();

	g_pRenderText->RenderText((mu_int32)x + 50, (mu_int32)y, GlobalText[1214], 0, 0, RT3_WRITE_RIGHT_TO_LEFT);

	RenderBar(x, y + 12, Width, Height, (mu_float)Life);

	glColor3f(1.0f, 1.0f, 1.0f);

	Width = 20.0f; Height = 28.0f; x = GetScreenWidth() - Width - PartyWidth - 65.0f; y = 5.0f;
	RenderBitmap(BITMAP_SKILL_INTERFACE + 2, (mu_float)x, (mu_float)y, (mu_float)Width - 4, (mu_float)Height - 8, (((m_byCommand) % 8) * 32 + 6.0f) / 256.0f, (((m_byCommand) / 8)*Height + 3.0f) / 256.0f, Width / 256.0f, (Height - 1.0f) / 256.0f);

	Width -= 8.0f; Height -= 8.0f;
	
	if (MouseX >= x && MouseX <= x + Width && MouseY >= y && MouseY <= y + Height)
	{
		RenderTipText((mu_int32)x, (mu_int32)(y + Height), GlobalText[1219 + m_byCommand]);
	}
#endif
}

void CSPetDarkSpirit::AttackEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o)
{
	MUModel* b = &MODELS::Data[o->Type];
	EVector3 p, Pos, Light;

	switch (o->AI)
	{
	case PET_ATTACK:
		if (c->AttackTime >= 0 && c->AttackTime <= 2 && m_PetTarget != nullptr)
		{
			for (mu_int32 i = 0; i < 10; ++i)
			{
				CreateJoint(ThreadIndex, BITMAP_LIGHT, o->Position, o->Position, o->Angle, 1, nullptr, (mu_float)(GetLargeRand(40) + 20));
			}
			if (c->AttackTime == 1)
			{
				EVector3 Angle, Light;

				Vector(45.0f, (mu_float)(GetLargeRand(180)) - 90, 0.0f, Angle);
				Vector(1.0f, 0.8f, 0.6f, Light);
				CreateEffect(ThreadIndex, MODEL_DARKLORD_SKILL, o->Position, Angle, Light, 3);
			}
		}
		if (c->AttackTime > 3 && c->AttackTime % 2)
		{
			if (o->Position[2] > (m_PetOwner->Object.Position[2] + 100.0f))
			{
				Vector(50.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(6)], p, Pos);

				CreateEffect(ThreadIndex, MODEL_AIR_FORCE, Pos, o->Angle, o->Light, 0, o);
			}
		}
		break;

	case PET_ATTACK_MAGIC:
		if (c->AttackTime <= 14)
		{
			if (o->BoneTransform != nullptr)
			{
				Vector(1.0f, 0.6f, 0.4f, Light);
				Vector(0.0f, 0.0f, 0.0f, p);
				mu_int32 boneCount = EMath::Min(b->_BoneNonDummyEnd, 66U);
				for (mu_int32 i = GetLargeRand(2); i < boneCount; i += 2)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[i], p, Pos);
					CreateParticle(ThreadIndex, BITMAP_LIGHT, Pos, o->Angle, Light, 6, 1.3f);
				}
			}
		}
		else if (c->AttackTime == 15)
		{
			if (c->TargetCharacter != -1)
			{
				CHARACTER* tc = &CharactersClient[c->TargetCharacter];
				OBJECT* to = &tc->Object;

				if (to != nullptr)
				{
					b->RenderMeshEffect(ThreadIndex, o->BoneTransform, 1, BITMAP_LIGHT, 1, o->Angle, to);
				}

				SetAI(PET_FLYING);
			}
			c->AttackTime = 0;
		}
		break;
	}
}