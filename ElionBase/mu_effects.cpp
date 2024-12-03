#include "stdafx.h"
#include "mu_effects.h"

MUCriticalSectionSpinLock g_EffectsLock;
MUCriticalSectionSpinLock g_EffectsEraseLock;

std::array<OBJECT, MAX_EFFECTS> g_Effects;
std::array<MUObjectExtra, MAX_EFFECTS> g_EffectsExtra;
std::array<mu_uint16, MAX_EFFECTS + MAX_TASKING_THREADS> g_EffectsQueue;
std::array<mu_uint16, MAX_EFFECTS> g_EffectsFree;

mu_uint16 g_EffectsCurCount = 0;
mu_uint16 g_EffectsFreeCount = 0;
mu_atomic_int32_t g_EffectsDeleteCount(0);
mu_boolean g_EffectsLocked = false;

std::array<EFFECT_WAIT, MAX_EFFECTS_WAIT> g_EffectsWait;
mu_atomic_uint32_t g_EffectsWaitCount(0);
std::array<EFFECT_DELETE, MAX_EFFECTS_WAIT> g_EffectsErase;
mu_uint16 g_EffectsEraseCount = 0;

void CreateInferno(const mu_uint32 ThreadIndex, EVector3 Position, mu_int32 SubType)
{
	EVector3 p, p2, Angle, Light;
	EBone Matrix;
	Vector(1.0f, 1.0f, 1.0f, Light);
	for (mu_int32 j = 0; j < 8; ++j)
	{
		if (SubType == 2 || SubType == 3 || SubType == 5)
		{
			Vector(0.0f, -240.0f, 0.0f, p);
		}
		else
			Vector(0.0f, -220.0f, 0.0f, p);
		Vector(0.0f, 0.0f, j*45.0f, Angle);
		AngleMatrix(Angle, Matrix);
		VectorRotate(p, Matrix, p2);
		VectorAdd(Position, p2, p2);
		CreateBomb(ThreadIndex, p2, true, SubType);
		if (SubType == 1)
		{
			CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), p2, Angle, Light, 0);
			CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), p2, Angle, Light, 0);
		}
		else if (SubType == 2 || SubType == 3)
		{
			CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), p2, Angle, Light, 10);
			CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), p2, Angle, Light, 10);
			CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), p2, Angle, Light, 10);
		}
		else if (SubType == 5)
		{
			Vector(0.3f, 0.6f, 1.0f, Light);
			for (mu_int32 i = 0; i < 3; ++i)
				CreateEffect(ThreadIndex, MODEL_EFFECT_BROKEN_ICE0 + GetLargeRand(3), p2, Angle, Light, 0);
		}
		else
		{
			CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), p2, Angle, Light, 1);
			CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), p2, Angle, Light, 0);
		}
	}
}

template<mu_boolean isLockedAllowed>
void CreateEffect(const mu_uint32 ThreadIndex, mu_int32 Type, EVector3 Position, EVector3 Angle,
	EVector3 Light, mu_int32 SubType, OBJECT *Owner, mu_int16 PKKey, mu_uint16 SkillIndex, mu_uint16 Skill, mu_uint16 SkillSerialNum,
	mu_float Scale, mu_int16 sTargetIndex)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (ThreadInfo.RenderGUI == true)
	{
		return;
	}

	if constexpr(isLockedAllowed)
	{
		if (ThreadInfo.NoLockedAllowed == false &&
			g_EffectsLocked == true)
		{
			const mu_uint32 i = g_EffectsWaitCount.fetch_add(1);

			if (i < MAX_EFFECTS_WAIT)
			{
				EFFECT_WAIT &o = g_EffectsWait[i];

				o.Type = Type;
				o.Position = Position;
				o.Angle = Angle;
				o.Light = Light;
				o.SubType = SubType;
				o.Owner = Owner;
				o.PKKey = PKKey;
				o.SkillIndex = SkillIndex;
				o.Skill = Skill;
				o.SkillSerialNum = SkillSerialNum;
				o.Scale = Scale;
				o.sTargetIndex = sTargetIndex;
			}

			return;
		}
	}

	mu_uint32 i = 0;

	g_EffectsLock.Lock();

	if (g_EffectsFreeCount > 0)
	{
		i = g_EffectsFree[--g_EffectsFreeCount];
	}
	else if (g_EffectsCurCount < MAX_EFFECTS)
	{
		i = g_EffectsCurCount++;
	}
	else
	{
		g_EffectsLock.Unlock();
		return;
	}

	g_EffectsLock.Unlock();

	OBJECT *o = &g_Effects[i];

	o->CreationTime = g_CreationTime;
	o->Flags.Clear();
	o->Flags.Set(OBJECT::eOF_LIVE | OBJECT::eOF_LIGHTENABLE);

	o->Type = Type;
	o->SubType = SubType;
	o->HiddenMesh = -1;
	o->BlendMesh = -1;
	o->BlendMeshLight = 1.0f;
	o->BlendMeshTexCoordU = 0.0f;
	o->BlendMeshTexCoordV = 0.0f;
	o->AnimationFrame = 0.0f;
	o->Alpha = 1.0f;
	o->PriorAnimationFrame = 0.0f;

	if (Scale <= 0.0f)
		o->Scale = 0.9f;
	else
		o->Scale = Scale;

	if (Owner)
		o->Owner = Owner;
	else
		o->Owner = nullptr;

	o->Velocity = 0.3f;
	o->PKKey = PKKey;
	o->Kind = Skill;
	o->Skill = SkillIndex;
	o->RenderType = 0;
	o->AttackPoint[0] = 0;
	o->CurrentAction = 0;
	o->m_bySkillSerialNum = (mu_uint8)SkillSerialNum;
	o->m_sTargetIndex = sTargetIndex;

	VectorCopy(Light, o->Light);
	VectorCopy(Angle, o->Angle);
	VectorCopy(Position, o->Position);
	Vector(0.0f, 0.0f, 0.0f, o->Direction);
	EBone Matrix;
	EVector3 p1, p2;

	switch (Type)
	{
		// Skills Re-factoring
	case MODEL_EVILSPIRIT:
		{
			mu_float PlaySpeed = GetPlaySpeed(Owner, Owner->Type, Owner->CurrentAction);
			mu_float FrameCount = 5.0f;

			o->LifeTime = (mu_int32)EMath::Ceil(FrameCount / PlaySpeed);
		}
		break;

	case MODEL_DRAGON:
		{
			o->CollisionRange = true;
			o->Kind = 0;
			o->Timer = 0.0f;

			o->Velocity = 1.0f;
			o->Angle[1] = 0.0f;
			o->LifeTime = 1000;
			o->Gravity = 1.0f;
			o->Distance = 1.0f;
			o->Position[2] += 3400.0f;
			Vector(0.0f, -35.0f, 0.0f, o->Direction);
			VectorCopy(o->Position, o->StartPosition);
			Vector(1.0f, 1.0f, 1.0f, o->Light);
		}
		break;
	case MODEL_ARROW_AUTOLOAD:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 40;
			}
			else if (o->SubType == 1)
			{
				o->LifeTime = 40;
				o->Scale = 1.2f;
				Vector(1.0f, 0.8f, 0.2f, o->Light);
				VectorCopy(o->Light, o->Direction);
			}
		}
		break;
	case MODEL_INFINITY_ARROW:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 40;
				CreateEffect(ThreadIndex, MODEL_INFINITY_ARROW, Owner->Position, o->Angle, o->Light, 1, Owner);
				CreateEffect(ThreadIndex, MODEL_INFINITY_ARROW1, Owner->Position, o->Angle, o->Light, 0, Owner);
				CreateEffect(ThreadIndex, MODEL_INFINITY_ARROW2, Owner->Position, o->Angle, o->Light, 0, Owner);
				CreateEffect(ThreadIndex, MODEL_INFINITY_ARROW3, Owner->Position, o->Angle, o->Light, 0, Owner);
				CreateEffect(ThreadIndex, MODEL_INFINITY_ARROW4, Owner->Position, o->Angle, o->Light, 0, Owner);
			}
			else if (o->SubType == 1)
			{
				o->LifeTime = 40;
				o->Scale = 1.0f;
				Vector(1.0f, 1.0f, 1.0f, o->Light);
				VectorCopy(o->Light, o->Direction);
			}
		}
		break;
	case MODEL_DESAIR:
		{
			o->LifeTime = 52;
			o->Scale = 1.4f;
		}
		break;
	case MODEL_INFINITY_ARROW1:
	case MODEL_INFINITY_ARROW2:
	case MODEL_INFINITY_ARROW3:
		{
			o->LifeTime = 40.0f;
			o->Scale = 1.0f;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			VectorCopy(o->Light, o->Direction);

			if (o->SubType == 1 || o->SubType == 2 || o->SubType == 3)
			{
				o->LifeTime = 60.0f;
				o->Scale = 1.0f;
			}
		}
		break;

	case MODEL_INFINITY_ARROW4:
		{
			o->LifeTime = 15.0f;
			o->Scale = 1.0f;
			Vector(1.0f, 0.5f, 0.3f, o->Light);
			VectorCopy(o->Light, o->Direction);
		}
		break;

	case MODEL_SHIELD_CRASH:
		{
			o->LifeTime = 24;
			o->Scale = 1.1f;
			o->Gravity = o->Velocity;
			Vector(0.5f, 0.5f, 1.0f, o->Light);
			VectorCopy(o->Light, o->Direction);

			if (o->SubType == 0)
			{
				CreateEffect(ThreadIndex, MODEL_SHIELD_CRASH2, Owner->Position, Owner->Angle, o->Light, 0, o->Owner);
			}
			else
			{
				if (o->SubType == 2)
				{
					o->Scale = 0.7;
				}
				o->HiddenMesh = 0;
			}
		}
		break;

	case MODEL_SHIELD_CRASH2:
		{
			o->LifeTime = 24;
			o->Scale = 1.1f;
			o->Gravity = o->Velocity;
			Vector(0.5f, 0.5f, 1.0f, o->Light);
			VectorCopy(o->Light, o->Direction);
		}
		break;

	case MODEL_IRON_RIDER_ARROW:
		{
			o->LifeTime = 12;
			o->Scale = 0.5f;
			o->Velocity = 70.0f;
			Vector(0.8f, 1.0f, 0.8f, o->Light);

			EVector3 vDir;
			EBone vMat;
			Vector(0.0f, -1.0f, 0.0f, vDir);
			AngleMatrix(o->Angle, vMat);
			VectorRotate(vDir, vMat, o->Direction);
			CreateJoint(ThreadIndex, BITMAP_JOINT_HEALING, o->Position, o->Position, o->Angle, 14, o, 30.0f);
		}
		break;
	case MODEL_BLADE_SKILL:
		{
			o->LifeTime = 10;
			o->Scale = 1.5f;
			if (o->SubType == 1)
			{
				o->Scale = 1.0f;
				o->LifeTime = 14;
			}
		}
		break;
	case MODEL_KENTAUROS_ARROW:
		{
			o->LifeTime = 34;
			o->Scale = 0.7f;
			o->Velocity = 70.0f;
			o->Alpha = 0.0f;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
		}
		break;
	case MODEL_WARP3:
	case MODEL_WARP6:
		o->LifeTime = 0xffffff;
		o->BlendMesh = -2;
		o->Scale = 0.6f;
		break;
	case MODEL_WARP2:
	case MODEL_WARP:
	case MODEL_WARP5:
	case MODEL_WARP4:
		{
			Vector(0.0f, 0.0f, 0.0f, Light);
			o->BlendMesh = -2;
			o->LifeTime = 0xffffff;
			o->Scale = 1.3f + static_cast<mu_float>(GetLargeRand(50)) / 100.0f;
			o->Gravity = static_cast<mu_float>(GetLargeRand(80)) / 10.0f;
			o->Velocity = static_cast<mu_float>(GetLargeRand(100)) / 1000.0f + 0.01f;
		}
		break;
	case MODEL_GHOST:
		{
			o->Kind = 0;
			o->BlendMeshLight = 1.0f;
			o->Velocity = 1.0f;
			o->LifeTime = 1000;
			o->Gravity = 1.0f;
			o->Distance = 1.0f;
			o->Position[2] += static_cast<mu_float>(GetLargeRand(200)) - 100.0f;
			o->Position[1] += static_cast<mu_float>(GetLargeRand(200)) - 100.0f;
			o->Position[0] += static_cast<mu_float>(GetLargeRand(200)) - 100.0f;
			o->Angle[2] += static_cast<mu_float>(GetLargeRand(360));
			Vector(0.0f, static_cast<mu_float>(GetLargeRand(8))*0.1f - 5.0f, 0.0f, o->Direction);
			o->Scale = o->Scale + static_cast<mu_float>(GetLargeRand(5)) / 40.0f;
			VectorCopy(o->Position, o->StartPosition);
			Vector(0.01f, 0.01f, 0.03f, o->Light);
		}
		break;
	case MODEL_TREE_ATTACK:
		o->Position[2] += 20.0f;
		o->LifeTime = 20;
		o->Flags.Clear(OBJECT::eOF_LIGHTENABLE);
		o->Scale = 0.05f;
		break;
	case MODEL_BUTTERFLY01:		
		if (g_CurrentScene == CHARACTER_SCENE)
			o->HiddenMesh = -2;
		o->LifeTime = 250;
		o->Velocity = 0.3f;
		o->Flags.Clear(OBJECT::eOF_LIGHTENABLE);
		o->Kind = GetLargeRand(2);		

		Vector(0.0f, -5.0f*o->Owner->Scale, 0.0f, o->Direction);

		if (o->SubType == 3)
			o->Scale = 0.9f;
		else
			o->Scale = 0.25f;
		break;
	case 9:                         
		o->LifeTime = 30;
		o->Scale = (GetLargeRand(3)) / 10.0f + 0.6f;
		o->Velocity = 10.0f;
		o->PKKey = 0;
		Vector(-0.1f, -0.1f, -0.5f, o->Direction);
		break;
	case BITMAP_SKULL:	
		o->LifeTime = 1000;
		if (1 == o->SubType)
		{	
			o->LifeTime -= 60;
		}
		else if (2 == o->SubType)
		{	
			o->LifeTime = 8;
			o->Position[2] += 150.0f;
		}
		else if (o->SubType == 4)
		{
			o->LifeTime = 20;
			o->Velocity = 2.0f;
			Vector(0.0f, -10.0f, 0.0f, o->Direction);
		}
		break;
	case MODEL_MAGIC_CAPSULE2:	
		o->LifeTime = 20;
		o->BlendMesh = 0;
		o->BlendMeshLight = 1.0f;
		break;
	case MODEL_SPEAR:	
		o->LifeTime = 10;
		break;
	case MODEL_SPEAR + 1:	
		o->Angle[0] = o->Angle[1] = o->Angle[2] = 0.0f;
		o->LifeTime = 5;
		break;

	case MODEL_SPEARSKILL:
		o->LifeTime = 20;
		o->Scale = 1.5f;
		o->Direction[0] = 5.0f * EMath::Sin(o->Angle[2] * Q_PI / 180.0f);
		o->Direction[1] = -5.0f * EMath::Cos(o->Angle[2] * Q_PI / 180.0f);
		break;
	case BITMAP_FIRE_CURSEDLICH:
		if (o->SubType == 0)
		{
			o->BlendMesh = -2;
			o->LifeTime = 10;
			o->Scale = 0.7f;
			VectorCopy(Position, o->Position);
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 50;
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = 20;
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = 10;
		}
		else if (o->SubType == 12)
		{
			o->LifeTime = 20;
		}
		break;
	case MODEL_SWELL_OF_MAGICPOWER:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 45;
			}
		}break;
	case MODEL_ARROWSRE06:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 30;
			}
			else if (o->SubType == 1)
			{
				o->LifeTime = 40;
			}
			else if (o->SubType == 2)
			{
				o->LifeTime = 30;
				VectorCopy(Position, o->Position);
			}
		}break;
	case MODEL_SWELL_OF_MAGICPOWER_BUFF_EFF:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 999;
				o->Timer = FWorldTime;
			}
		}break;
	case MODEL_SUMMONER_WRISTRING_EFFECT:
		o->BlendMesh = -2;
		o->LifeTime = 100;
		o->Scale = 0.7f;
		VectorCopy(Position, o->Position);
		Vector(0.0f, 0.0f, 0.0f, o->Direction);
		break;
	case MODEL_SUMMONER_EQUIP_HEAD_SAHAMUTT:
		if (o->SubType == 0)
		{
			o->LifeTime = 100;
			o->Scale = 0.8f;
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			o->Alpha = 0.0f;
			OBJECT * pObject = o->Owner;
			o->Position[0] = pObject->Position[0] + EMath::Cos(FWorldTime*0.003f) * 40.0f;
			o->Position[1] = pObject->Position[1] + EMath::Sin(FWorldTime*0.003f) * 40.0f;
			o->Position[2] = pObject->Position[2] + (EMath::Sin(FWorldTime*0.0010f) + 2.0f) * 80.0f - 60.0f;
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 100000;
		}
		break;
	case MODEL_SUMMONER_EQUIP_HEAD_NEIL:
		if (o->SubType == 0)
		{
			o->LifeTime = 100;
			o->Scale = 0.8f;
			o->Alpha = 0.0f;
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			OBJECT * pObject = o->Owner;
			o->Position[0] = pObject->Position[0] + EMath::Cos(FWorldTime*0.003f) * 40.0f;
			o->Position[1] = pObject->Position[1] + EMath::Sin(FWorldTime*0.003f) * 40.0f;
			o->Position[2] = pObject->Position[2] + (EMath::Sin(FWorldTime*0.0010f) + 2.0f) * 80.0f - 60.0f;
			CreateJoint(ThreadIndex, MODEL_SPEARSKILL, o->Position, o->Position, o->Angle, 15, o, 40.0f, -1, 0, 0);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 100000;
		}
		break;
	case MODEL_SUMMONER_EQUIP_HEAD_LAGUL:	
		{
			o->LifeTime = 100;
			o->Scale = 0.8f;
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			o->Alpha = 0.0f;
			OBJECT * pObject = o->Owner;
			o->Position[0] = pObject->Position[0] + EMath::Cos(FWorldTime*0.003f) * 40.0f;
			o->Position[1] = pObject->Position[1] + EMath::Sin(FWorldTime*0.003f) * 40.0f;
			o->Position[2] = pObject->Position[2] + (EMath::Sin(FWorldTime*0.0010f) + 2.0f) * 80.0f - 60.0f;
			CreateJoint(ThreadIndex, MODEL_SPEARSKILL, o->Position, o->Position, o->Angle, 17, o, 40.0f, -1, 0, 0);
		}
		break;
	case MODEL_SUMMONER_CASTING_EFFECT1:
	case MODEL_SUMMONER_CASTING_EFFECT11:
	case MODEL_SUMMONER_CASTING_EFFECT111:
	case MODEL_SUMMONER_CASTING_EFFECT2:
	case MODEL_SUMMONER_CASTING_EFFECT22:
	case MODEL_SUMMONER_CASTING_EFFECT222:
		{
			o->LifeTime = 40;

			if (o->SubType == 0)
				o->Scale = 1.0f;

			o->Alpha = 1.0f;
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			o->BlendMesh = 0;
			o->BlendMeshLight = 0.0f;
		}
		break;
	case MODEL_SUMMONER_CASTING_EFFECT4:
		o->LifeTime = 25;
		o->Scale = 1.0f;
		o->Alpha = 1.0f;
		Vector(0.0f, 0.0f, 0.0f, o->Direction);
		o->BlendMesh = 0;
		o->BlendMeshLight = 0.0f;
		break;
	case MODEL_SUMMONER_SUMMON_SAHAMUTT:
		o->LifeTime = 80;
		if (o->SubType == 2)
			o->Scale = 0.7f;
		else if (o->SubType == 1)
			o->Scale = 0.5f;
		else if (o->SubType == 0)
			o->Scale = 0.35f;
		o->Alpha = 0.0f;
		o->Velocity = 0.5f;

		VectorCopy(Light, o->HeadTargetAngle);	
		Vector(1.0f, 1.0f, 1.0f, o->Light);
		break;
	case MODEL_SUMMONER_SUMMON_NEIL:
		o->LifeTime = 80;
		o->Scale = 1.0f;
		o->Alpha = 0.0f;
		o->Velocity = 0.35f;
		o->Skill = 0;
		o->Position[2] += 10.0f;

		VectorCopy(Light, o->HeadTargetAngle);	
		Vector(1.0f, 1.0f, 1.0f, o->Light);
		break;
	case MODEL_SUMMONER_SUMMON_NEIL_NIFE1:
	case MODEL_SUMMONER_SUMMON_NEIL_NIFE2:
	case MODEL_SUMMONER_SUMMON_NEIL_NIFE3:
		o->LifeTime = 50;
		o->Scale = 1.0f;
		o->Alpha = 1.0f;
		break;
	case MODEL_SUMMONER_SUMMON_NEIL_GROUND1:
	case MODEL_SUMMONER_SUMMON_NEIL_GROUND2:
	case MODEL_SUMMONER_SUMMON_NEIL_GROUND3:
		o->LifeTime = 50;
		o->Scale = 1.0f;
		o->Alpha = 0.0f;
		break;
	case MODEL_SUMMONER_SUMMON_LAGUL:
		if (o->SubType == 0)		
		{
			o->LifeTime = 160;
			o->Scale = 1.0f;
			o->Velocity = 0.5f;
			VectorCopy(Position, o->HeadTargetAngle);
		}
		else if (o->SubType == 1)	
		{
			o->LifeTime = 160;
			o->Alpha = 0.0f;

			((JOINT*)o->Owner)->Target = o;
			o->Owner = nullptr;

			for (mu_int32 i = 48; i <= 53; ++i)
			{	
				CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, i, o, 10.0f);
			}
		}
		break;
		
	case BITMAP_ENERGY:
		if (o->SubType == 0)
		{
			o->LifeTime = 20;
			Vector(0.0f, -60.0f, 0.0f, o->Direction);
			o->Position[2] += 100.0f;
		}
		break;
		

	case MODEL_LIGHTNING_ORB:
		{
			if (o->SubType == 0)
			{
#ifndef ENABLE_SKILLPROCESSORS
				mu_float PlaySpeed = GetPlaySpeed(Owner, Owner->Type, Owner->CurrentAction);
				mu_float FrameCount = 5.0f;

				o->LifeTime = 20 + (mu_int32)EMath::Ceil(FrameCount / PlaySpeed);
#else
				o->LifeTime = 20;
#endif
				
				Vector(0.0f, -60.0f, 0.0f, o->Direction);
				
				o->Position[2] += 100.0f;
			}
			else if (o->SubType == 1)
			{
				o->LifeTime = 18;
			}
		}
		break;
		
	case MODEL_CHAIN_LIGHTNING:
		{
			switch (o->SubType)
			{
			case 0:
			case 1:
			case 2:
				{
					o->LifeTime = 20;
				}
				break;
			}
		}
		break;
		
	case MODEL_ALICE_DRAIN_LIFE:
		{
			if (o->SubType == 0)
			{
#ifndef ENABLE_SKILLPROCESSORS
				mu_float PlaySpeed = GetPlaySpeed(Owner, Owner->Type, Owner->CurrentAction);
				mu_float FrameCount = 5.0f;

				o->LifeTime = 70 + (mu_int32)EMath::Ceil(FrameCount / PlaySpeed);
#else
				o->LifeTime = 70;
#endif
			}
		}
		break;
	case MODEL_ALICE_BUFFSKILL_EFFECT:
		{
			if (o->SubType == 0 || o->SubType == 1 || o->SubType == 2)
			{
				o->LifeTime = 34;
				o->Position[2] += 100;
				VectorCopy(Light, o->Light);
				o->Flags.Set(OBJECT::eOF_ALPHAENABLE);
				o->Alpha = 0.0f;
				o->BlendMeshLight = 0.0f;
				o->Angle[2] = 0.0f;
				o->Scale = 0.1f;
			}
			else if (o->SubType == 3 || o->SubType == 4)
			{
				o->LifeTime = 100;	

				if (o->SubType == 3)
				{
					o->Scale = 1.5f;
				}
				else if (o->SubType == 4)
				{
					o->Scale = 1.0f;
				}

			}
		}
		break;

	case MODEL_ALICE_BUFFSKILL_EFFECT2:
		{
			o->LifeTime = 35;
			o->Position[2] += 100;
			VectorCopy(Light, o->Light);
			o->Flags.Set(OBJECT::eOF_ALPHAENABLE);
			o->Alpha = 0.0f;
			o->BlendMeshLight = 0.0f;
			o->Angle[2] = 0.0f;
			o->Scale = 0.15f;
		}
		break;

	case MODEL_LIGHTNING_SHOCK:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 20;
				
				o->Position[2] += 280.0f;
				o->Velocity = 0.0f;
				o->Gravity = 1.0f;
			}
			else if (o->SubType == 1)
			{
				o->LifeTime = 12;
				EVector3 vLight;
				
				Vector(1.0f, 0.8f, 0.5f, vLight);
				CreateEffect(ThreadIndex, BITMAP_DAMAGE_01_MONO, o->Position, o->Angle, vLight, 1);
				Vector(1.0f, 0.0f, 0.0f, vLight);
				CreateEffect(ThreadIndex, BITMAP_DAMAGE_01_MONO, o->Position, o->Angle, vLight, 1);
				
				EBone Matrix;
				EVector3 vAngle, vDirection, vPosition;
				mu_float fAngle;
				Vector(1.0f, 0.2f, 0.05f, vLight);
				for (mu_int32 i = 0; i < 5; ++i)
				{
					Vector(0.0f, 150.0f, 0.0f, vDirection);
					fAngle = o->Angle[2] + i * 72.0f;
					Vector(0.0f, 0.0f, fAngle, vAngle);
					AngleMatrix(vAngle, Matrix);
					VectorRotate(vDirection, Matrix, vPosition);
					VectorAdd(vPosition, o->Position, vPosition);

					CreateEffect(ThreadIndex, BITMAP_MAGIC, vPosition, o->Angle, vLight, 12);
				}

				
				Vector(1.0f, 0.4f, 0.2f, vLight);
				VectorCopy(o->Position, vPosition);
				vPosition[2] = RequestTerrainHeight(vPosition[0], vPosition[1]) + 10;
				for (mu_int32 i = 0; i < 3; ++i)
					CreateEffect(ThreadIndex, MODEL_KNIGHT_PLANCRACK_A, vPosition, o->Angle, vLight, 1, o, 0, 0, 0, 0, GetLargeRand(4) * 0.1f + 1.0f);
			}
			else if (o->SubType == 2)
			{
				o->LifeTime = 15;
				
				EVector3 vLight;
				EVector3 vPosition;
				Vector(1.0f, 0.4f, 0.2f, vLight);
				VectorCopy(o->Position, vPosition);
				vPosition[2] = RequestTerrainHeight(vPosition[0], vPosition[1]) + 10;
				for (mu_int32 i = 0; i < 3; ++i)
					CreateEffect(ThreadIndex, MODEL_KNIGHT_PLANCRACK_A, o->Position, o->Angle, vLight, 1, o, 0, 0, 0, 0, GetLargeRand(4) * 0.1f + 0.5f);
			}
		}
		break;

	case BITMAP_SPARK + 1:
		o->LifeTime = 10;
		break;
	case BITMAP_BOSS_LASER:
	case BITMAP_BOSS_LASER + 1:
	case BITMAP_BOSS_LASER + 2:
		o->LifeTime = 20;
		switch (Type)
		{
		case BITMAP_BOSS_LASER:
			Vector(0.5f, 0.7f, 1.0f, o->Light);
			Vector(0.0f, -50.0f, 0.0f, p1);
			o->Scale = 16.0f;

			if (SubType == 1)
			{
				Vector(0.0f, -50.0f, 0.0f, p1);
				o->Scale = 3.0f;
				Vector(1.0f, 1.0f, 1.0f, o->Light);
			}
			else if (SubType == 2)
			{
				o->LifeTime = 35;
				Vector(0.0f, -50.0f, 0.0f, p1);
				o->Scale = 2.5f;
				Vector(1.0f, 1.0f, 1.0f, o->Light);
			}
			break;
		case BITMAP_BOSS_LASER + 1:
			Vector(1.0f, 0.4f, 0.2f, o->Light);
			Vector(0.0f, -50.0f, 0.0f, p1);
			o->Scale = 16.0f;
			break;
		case BITMAP_BOSS_LASER + 2:
			Vector(1.0f, 0.4f, 0.2f, o->Light);
			Vector(0.0f, -15.0f, 0.0f, p1);
			o->Scale = 5.0f;
			break;
		}
		
		AngleMatrix(o->Angle, Matrix);
		VectorRotate(p1, Matrix, o->Direction);
		break;
	case BITMAP_LIGHTNING + 1:
		if (o->SubType == 0)
		{
			o->LifeTime = 10;
			o->Scale = 1.5f;
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 50;
			o->Alpha = 0.01f;
		}
		break;
	case BITMAP_LIGHT:
		if (o->SubType == 0)
		{
			Vector(0.3f, 0.3f, 0.3f, o->Light);
			mu_float fAngle = 70.0f * Q_PI / 180.0f;
			mu_float fAngle2 = 30.0f;
			mu_float fSpeed = static_cast<mu_float>(9 + GetLargeRand(5))*0.5f;
			o->Direction[0] = fSpeed * EMath::Cos(fAngle) * EMath::Sin(fAngle2 * Q_PI / 180.0f);
			o->Direction[1] = fSpeed * EMath::Cos(fAngle) * EMath::Cos(fAngle2 * Q_PI / 180.0f);
			o->Direction[2] = fSpeed * EMath::Sin(fAngle);
			o->Scale = 3.0f;
			
			o->LifeTime = 400;
		}
		else if (o->SubType == 1 || o->SubType == 2)
		{
			o->LifeTime = 1000;
			o->Velocity = 0.0f;
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = 30;
			o->Velocity = 0.0f;
		}
		break;

	case BITMAP_FIRE:
		o->LifeTime = 1000;
		break;

	case BITMAP_FIRE + 1:
		o->LifeTime = 10;
		AngleMatrix(o->Angle, Matrix);
		Vector(0.0f, -60.0f, 0.0f, p1);
		VectorRotate(p1, Matrix, p2);
		VectorAdd(o->Position, p2, o->Position);
		o->Position[2] += 130.0f;
		break;

	case BITMAP_FLAME:
		if (o->SubType == 0)
		{
			o->LifeTime = 40;
			o->Weapon = 0;
		}
		else if (o->SubType == 1 || o->SubType == 2)
		{
			o->LifeTime = 10;
			Vector(0.0f, 0.0f, 0.0f, o->Angle);
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = 15;
		}
		else if (o->SubType == 4)
		{
			o->Scale = 0.01f;
			o->LifeTime = 10;
			Vector(0.0f, 0.0f, 0.0f, o->Angle);
		}
		else if (o->SubType == 5)
		{
			o->LifeTime = 20;
		}
		else if (o->SubType == 6)
		{
			o->LifeTime = 40;
		}
		break;

	case MODEL_RAKLION_BOSS_CRACKEFFECT:
		if (o->SubType == 0)
		{
			o->LifeTime = 40;
			o->Scale = Scale + 1.0f;
			o->Position[2] += 30.0f;
			o->Angle[2] = GetLargeRand(360);
		}
		break;

	case MODEL_RAKLION_BOSS_MAGIC:
		if (o->SubType == 0)
		{
			o->LifeTime = 35;
			o->Scale = Scale;
		}
		break;

	case BITMAP_FIRE_HIK2_MONO:
		if (o->SubType == 0)
		{
			o->LifeTime = 60;
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 20;
			Vector(0.0f, 0.0f, 0.0f, o->Angle);
		}
		break;

	case BITMAP_CLOUD:
		if (o->SubType == 0)
		{
			o->LifeTime = 60;
			o->Scale = Scale;
			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
		}
		break;

	case BITMAP_FIRE_RED:
		o->LifeTime = 40;
		break;

	case BITMAP_IMPACT:
		{
			o->LifeTime = 80;
			o->Scale = 0.0f;
			o->BlendMesh = -2;
		}
		break;

	case BITMAP_MAGIC:
		o->LifeTime = 20;
		o->Scale = 0.5f;
		if (o->SubType == 0)
		{
			o->LifeTime = 15;
		}
		else if (o->SubType == 8)
		{
			o->LifeTime = 30;
			o->Scale = 1.0f;
		}
		else if (o->SubType == 9)
		{
			o->LifeTime = 40;
			o->Scale = 2.4f;
			Vector(0, 0, 0, o->HeadAngle);
		}
		else if (o->SubType == 10)
		{
			o->LifeTime = 44;
			o->Scale = 12.0f;
			o->Alpha = 0.0f;
		}
		else if (o->SubType == 11)
		{
			o->LifeTime = 24;
			o->Scale = 0.8f;
			Vector(0, 0, 0, o->HeadAngle);
		}
		else if (o->SubType == 12)
		{
			o->LifeTime = 20;
			o->Scale = Scale * 0.1f;
		}
		break;

	case BITMAP_MAGIC + 1:
	case BITMAP_MAGIC + 2:
		o->LifeTime = 20;
		if (o->SubType == 4 || o->SubType == 10)
		{
			o->LifeTime = 40;
			o->Scale = ((GetLargeRand(50)) + 50) / 100.0f*4.0f;
		}
		else if (o->SubType == 6)
		{
			o->LifeTime = 60;
			o->Scale = ((GetLargeRand(50)) + 50) / 100.0f*4.0f;
			VectorCopy(Position, o->StartPosition);
		}
		else if (o->SubType == 7)
		{
			o->LifeTime = 40;
			o->Angle[2] = GetLargeRand(360);
		}
		else if (o->SubType == 8)
		{
			o->LifeTime = 20;
		}
		else if (o->SubType == 9)
		{
			o->LifeTime = 10;
			o->Scale = 0.1f;
		}
		else if (o->SubType == 11)
		{
			o->LifeTime = 20;
		}
		else if (o->SubType == 12)
		{
			o->LifeTime = 20;
		}
		else if (o->SubType == 13)
		{
			o->LifeTime = 40;
			o->Scale = Scale;
		}
		break;

	case BITMAP_OUR_INFLUENCE_GROUND:
	case BITMAP_ENEMY_INFLUENCE_GROUND:
		if (o->SubType == 0)
		{
			o->LifeTime = 50;
			o->Scale = 0.6f;
			o->Alpha = 1.0f;
			o->AlphaTarget = 0.75f;	
		}
		break;

	case BITMAP_MAGIC_ZIN:
		switch (o->SubType)
		{
		case 0:	o->LifeTime = 50;	break;
		case 1:
			o->LifeTime = 40;
			o->Alpha = 0.0f;
			break;
		case 2:	o->LifeTime = 30;	break;
		}
		break;

	case BITMAP_SHINY + 6:
		switch (o->SubType)	
		{
		case 0:	o->LifeTime = 24;	break;
		case 1:
		case 2:	o->LifeTime = 100;	break;
		case 3:
			o->LifeTime = 24;
			o->RenderType = BLEND::TXB_BLENDMINUS;
			break;
		}
		break;

	case BITMAP_PIN_LIGHT:
		switch (o->SubType)	
		{
		case 3:
		case 0:	o->LifeTime = 40;	break;
		case 1:
		case 2:	o->LifeTime = 100;	break;
		case 4:
			{
				o->Alpha = 1.0f;
				o->LifeTime = 30;
				o->Scale += static_cast<mu_float>(GetLargeRand(5)) / 10.0f;
				o->Angle[1] = GetLargeRand(360);
			}break;
		}
		break;

	case BITMAP_ORORA:		
		switch (o->SubType)
		{
		case 0:
		case 1:	o->LifeTime = 100;	break;
		case 2:
		case 3:	o->LifeTime = 25;	break;
		}
		CreateParticle(ThreadIndex, o->Type, o->Position, o->Angle, o->Light, o->SubType, 1.0f, o->Owner);
		break;

	case BITMAP_SPARK + 2:	
		o->LifeTime = 100;
		break;

	case BITMAP_LIGHT_MARKS:
		o->LifeTime = 65;
		break;

	case BITMAP_GATHERING:
		o->LifeTime = 10;

		switch (o->SubType)
		{
		case 0:
			Vector(0.0f, -100.0f, 0.0f, p1);
			AngleMatrix(o->Angle, Matrix);
			VectorRotate(p1, Matrix, p2);
			VectorAdd(o->Position, p2, o->Position);

			o->Position[2] += 150.0f;
			break;
		case 1:
		case 2:
			VectorCopy(Position, o->StartPosition);
			o->LifeTime = 20;
			break;
		case 3:
			o->LifeTime = 10;
			Vector(-10.0f, 10.0f, 0.0f, p1);
			AngleMatrix(o->Angle, Matrix);
			VectorRotate(p1, Matrix, p2);
			VectorAdd(o->Position, p2, o->Position);
			break;
		}
		break;

	case BITMAP_JOINT_THUNDER:
		{
			o->LifeTime = 20;

			VectorCopy(o->Position, o->StartPosition);
			o->StartPosition[2] += 800.0f;	
		}
		break;

	case MODEL_STAFF + 8:
		o->LifeTime = 30;
		o->BlendMesh = -2;
		o->Scale = 1.0f;
		o->Position[2] += 280.0f;
		o->Angle[0] += 20.0f;
		Vector(0.0f, -80.0f, -10.0f, o->Direction);
		break;
	case MODEL_WAVE:
		o->BlendMesh = 0;
		o->BlendMeshLight = 1.5f;
		o->Scale = 0.5f;
		o->LifeTime = 15;
		o->Position[2] -= 15.0f;

		Vector(1.0f, 1.0f, 1.0f, o->Light);
		break;
	case MODEL_TAIL:
		o->BlendMesh = -2;
		o->LifeTime = 6;
		o->Scale = 1.0f;
		o->Gravity = 80.0f;
		Vector(0.5f, 0.5f, 0.5f, o->Light);
		Vector(0.0f, 0.0f, 45.0f, o->Angle);
		break;
	case MODEL_SKILL_BLAST:
		o->LifeTime = 30;
		o->BlendMesh = 0;
		o->Scale = static_cast<mu_float>(GetLargeRand(8) + 10)*0.1f;
		o->Position[0] += GetLargeRand(100) + 200;
		o->Position[1] += GetLargeRand(100) - 50;
		o->Position[2] += GetLargeRand(500) + 300;
		Vector(0.0f, 0.0f, -50.0f - GetLargeRand(50), o->Direction);
		Vector(0.0f, 20.0f, 0.0f, o->Angle);
		VectorCopy(o->Position, o->EyeLeft);
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 5, o, 100.0f);
		o->Weapon = 0;
		break;
	case MODEL_WAVE_FORCE:
		o->BlendMesh = -2;
		o->Scale = 0.9f;
		o->Velocity = 0.5f;
		o->LifeTime = 12;
		o->Skill = 0;
		o->PKKey = -1;
		o->Scale = PKKey / 100.0f;
		o->BlendMeshLight = 0.1f;
		break;
	case MODEL_SKILL_INFERNO:
		o->BlendMesh = -2;
		o->Scale = 0.9f;
		o->Velocity = 0.5f;
		switch (o->SubType)
		{
		case 0: 
			Vector(0.8f, 0.8f, 0.8f, o->Light);
			o->LifeTime = 15;
			break;
		case 1: 
			Vector(1.0f, .5f, .2f, o->Light);
			o->LifeTime = 35;
			break;
		case 2: 
			o->LifeTime = 12;
			o->HiddenMesh = SkillIndex;
			o->Skill = 0;
			o->PKKey = -1;
			o->Distance = PKKey;
			o->Scale = PKKey / 100.0f;
			o->BlendMeshLight = 0.1f;
			break;
		case 8:
			o->LifeTime = 12;
			o->HiddenMesh = SkillIndex;
			o->Skill = 0;
			o->PKKey = -1;
			o->Distance = PKKey;
			o->Scale = PKKey / 100.0f;
			o->BlendMeshLight = 0.1f;
			o->Velocity *= 4;
			break;
		case 3: 
			Vector(0.8f, 0.8f, 0.8f, o->Light);
			o->LifeTime = 4;
			o->HiddenMesh = 1;
			o->Scale = 0.02f;
			o->Velocity = 1.0f;
			Vector(90.0f, 0.0f, 0.0f, o->Angle);
			break;
		case 4: 
			Vector(0.8f, 0.8f, 0.8f, o->Light);
			o->LifeTime = 35;
			o->HiddenMesh = 1;
			o->Scale = 0.1f;
			o->Gravity = 0.0f;
			break;
		case 5: 
			Vector(0.8f, 0.8f, 0.8f, o->Light);
			o->LifeTime = 15;
			break;
		case 6: 
			o->LifeTime = 5;
			o->HiddenMesh = SkillIndex;
			o->Skill = 0;
			o->PKKey = -1;
			o->Distance = PKKey;
			o->Scale = PKKey / 100.0f;
			o->Velocity = 0.1f;
			o->BlendMeshLight = 0.1f;
			break;
		case 9:
			{
				o->LifeTime = 13;
				Vector(0.1f, 1.0f, 0.2f, o->Light);
				o->Scale = 0.45f;
				o->BlendMeshLight = 0.1f;
				o->Gravity = 2.0f;
			}
			break;
		case 10:
			{
				o->LifeTime = 12;
				o->HiddenMesh = SkillIndex;
				o->Skill = 0;
				o->PKKey = -1;
				o->Distance = PKKey;
				o->Scale = PKKey / 100.0f;
				o->BlendMeshLight = 0.1f;
			}
			break;
		}
		break;
	case MODEL_BOSS_ATTACK:
		o->LifeTime = 50;
		o->Scale = 0.8f;
		o->Velocity = 1.0f;
		o->Angle[0] = 0.0f;
		o->BlendMesh = 0;
		o->Direction[1] = -50.0f;
		o->Gravity = -10.0f;
		break;
	case MODEL_MAGIC_CIRCLE1:
		o->LifeTime = 30;
		o->Scale = 0.7f;
		o->BlendMesh = -2;
		if (o->SubType == 2)
		{
			o->LifeTime = 15;
			o->Velocity = 0.3f;
			o->Scale = 0.7f;
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 20;
			o->Velocity = 0.1f;
			o->HiddenMesh = 0;
		}
		else
		{
			o->Velocity = 0.1f;
		}
		break;
	case MODEL_BIG_METEO1:
	case MODEL_BIG_METEO2:
	case MODEL_BIG_METEO3:
		o->LifeTime = 100;
		o->Scale = static_cast<mu_float>(GetLargeRand(10) + 4)*0.1f;
		Vector(0.0f, -15.0f / o->Scale, -30.0f / o->Scale, o->Direction);
		o->SubType = 1;
		break;
	case MODEL_PROTECT:
		o->LifeTime = 10000;
		o->Velocity = 0.3f;
		o->BlendMesh = 0;
		break;

	case MODEL_PIERCING:
		o->LifeTime = 100;
		o->BlendMesh = 0;
		o->BlendMeshLight = 1.0f;
		o->Gravity = 0;

		switch (SubType)
		{
		case 0:
			{
				o->Scale = 12.0f;
				VectorCopy(o->Owner->Position, o->StartPosition);
			}
			break;

		case 1:
			o->HiddenMesh = 0;
			o->Scale = 24.0f;
			break;

		case 2:
			o->HiddenMesh = 0;
			o->Scale = 12.0f;
			o->LifeTime = 10;
			break;

		case 3:
			{
				o->Scale = 10.0f;
				Vector(0.9f, 0.4f, 0.6f, o->Light);
				VectorCopy(o->Owner->Position, o->StartPosition);
			}
			break;
		}

		CreateJoint(ThreadIndex, BITMAP_FLARE + 1, o->Position, o->Position, o->Angle, 0, o, o->Scale, 30, SubType);
		CreateJoint(ThreadIndex, BITMAP_FLARE + 1, o->Position, o->Position, o->Angle, 1, o, o->Scale, 30, SubType);
		CreateJoint(ThreadIndex, BITMAP_FLARE + 1, o->Position, o->Position, o->Angle, 2, o, o->Scale, 30, SubType);
		CreateJoint(ThreadIndex, BITMAP_FLARE + 1, o->Position, o->Position, o->Angle, 3, o, o->Scale, 30, SubType);
		PlayBackground(SOUND_FLASH);
		o->Scale = 1.0f;
		break;

	case MODEL_PIERCING + 1:
		o->Velocity = 1.0f;
		o->LifeTime = 30;
		AngleMatrix(o->Angle, Matrix);
		Vector(-10.0f, -60.0f, 135.0f, p1);
		VectorRotate(p1, Matrix, p2);
		VectorAdd(o->Position, p2, o->Position);

		o->Scale = 0.8f;
		o->Direction[1] = -70.0f;

		CreateEffect(ThreadIndex, MODEL_PIERCING, o->Position, o->Angle, o->Light, SubType, o);
		break;

	case MODEL_ARROW_BEST_CROSSBOW:
		o->LifeTime = 30;
		o->BlendMesh = -2;
		o->Scale = 1.0f;
		o->Position[2] += 130.0f;
		Vector(0.0f, -70.0f, 0.0f, o->Direction);
		VectorCopy(o->Position, o->EyeLeft);
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 5, o, 100.0f);

		if (o->SubType != 0)
		{
			CreateEffect(ThreadIndex, MODEL_PIERCING, o->Position, o->Angle, o->Light, 0, o);
			o->AttackPoint[0] = 0;
			o->Kind = 1;
		}

		o->Weapon = 0;
		break;

	case MODEL_ARROW_DOUBLE:
		o->LifeTime = 30;
		o->BlendMesh = -2;
		o->Scale = 1.0f;
		o->Position[2] += 130.0f;
		Vector(0.0f, -70.0f, 0.0f, o->Direction);
		VectorCopy(o->Position, o->EyeLeft);
		CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 5, o, 100.0f);

		if (o->SubType != 0)
		{
			CreateEffect(ThreadIndex, MODEL_PIERCING, o->Position, o->Angle, o->Light, 0, o);
			o->AttackPoint[0] = 0;
			o->Kind = 1;
		}

		o->Weapon = 0;
		break;

	case MODEL_ARROW_HOLY:
		o->LifeTime = 30;
		o->BlendMesh = -2;
		o->Scale = 1.0f;
		o->Position[2] += 130.0f;

		Vector(0.0f, -60.0f, 0.0f, o->Direction);
		AngleMatrix(o->Angle, Matrix);
		Vector(-10.0f, -100.0f, 15.0f, p1);
		VectorRotate(p1, Matrix, p2);
		VectorCopy(o->Position, o->StartPosition);
		VectorAdd(o->StartPosition, p2, o->StartPosition);

		Vector(0.0f, 0.0f, o->Angle[2], Angle);

		if (o->SubType == 1)
		{
			for (mu_int32 i = 0; i<3; ++i)
			{
				if (i == 1)
					CreateJoint(ThreadIndex, BITMAP_FLARE, o->StartPosition, o->StartPosition, Angle, 25, o, 50.0f, -1, 1);
				else
					CreateJoint(ThreadIndex, BITMAP_FLARE, o->StartPosition, o->StartPosition, Angle, 25, o, 50.0f);
			}
		}
		else
		{
			for (mu_int32 i = 0; i<4; ++i)
			{
				if (i == 1)
					CreateJoint(ThreadIndex, BITMAP_FLARE, o->StartPosition, o->StartPosition, Angle, 11, o, 50.0f, -1, 1);
				else
					CreateJoint(ThreadIndex, BITMAP_FLARE, o->StartPosition, o->StartPosition, Angle, 11, o, 50.0f);
			}
		}

		o->Weapon = 0;
		break;
	case MODEL_MULTI_SHOT1:
	case MODEL_MULTI_SHOT2:
	case MODEL_MULTI_SHOT3:
	case MODEL_ARROW:
	case MODEL_ARROW_STEEL:
	case MODEL_ARROW_THUNDER:
	case MODEL_ARROW_LASER:
	case MODEL_ARROW_V:
	case MODEL_ARROW_SAW:
	case MODEL_ARROW_NATURE:
	case MODEL_ARROW_WING:
	case MODEL_LACEARROW:
	case MODEL_DARK_SCREAM:
	case MODEL_DARK_SCREAM_FIRE:
	case MODEL_ARROW_SPARK:
	case MODEL_ARROW_RING:
	case MODEL_ARROW_TANKER:
	case MODEL_ARROW_DARKSTINGER:
	case MODEL_ARROW_GAMBLE:
		if (Type == MODEL_ARROW)
			o->BlendMesh = 1;
		else if (Type == MODEL_ARROW_NATURE)
			o->BlendMesh = -2;
		else if (Type == MODEL_ARROW_THUNDER || Type == MODEL_ARROW_LASER || Type == MODEL_ARROW_V || Type == MODEL_ARROW_WING)
			o->BlendMesh = 0;

		o->Velocity = 1.0f;
		o->LifeTime = 30;
		AngleMatrix(o->Angle, Matrix);
		Vector(-10.0f, -60.0f, 135.0f, p1);
		VectorRotate(p1, Matrix, p2);
		VectorAdd(o->Position, p2, o->Position);
		if (Type == MODEL_ARROW_WING)
		{
			o->Scale = 1.8f;
			o->Direction[1] = -50.0f;
			o->Gravity = -10.0f;
		}
		else if (Type == MODEL_ARROW_TANKER)
		{
			o->Angle[0] -= 18.0f;
			o->Scale = 1.0f;
			o->Direction[1] = -42.0f;
			o->Direction[2] = 4.0f;
		}
		else if (Type == MODEL_MULTI_SHOT1)
		{
			o->LifeTime = 16;
			o->BlendMesh = -2;
			o->Scale = 0.0f;
		}
		else if (Type == MODEL_MULTI_SHOT2)
		{
			o->LifeTime = 13;
			o->BlendMesh = -2;
			o->Scale = 0.0f;
		}
		else if (Type == MODEL_MULTI_SHOT3)
		{
			o->LifeTime = 12;
			o->BlendMesh = -2;
			o->Scale = 0.0f;
		}
		else if (Type == MODEL_DARK_SCREAM || Type == MODEL_DARK_SCREAM_FIRE)
		{
			o->LifeTime = 19;
			o->Scale = 2.3f;
			o->Direction[1] = -35.0f;

			if (Type == MODEL_DARK_SCREAM)
			{
				o->Scale = 0.9f;
				o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 3.0f;
				EVector3 Light;
				Vector(0.0f, 1.0f, 0.0f, Light);
				if (SubType == 0)
				{
					CreateJoint(ThreadIndex, BITMAP_JOINT_FORCE, o->Position, o->Position, o->Angle, 7, o, 150.0f, 40);
				}
				else if (SubType == 1)
				{
					CreateJoint(ThreadIndex, BITMAP_JOINT_FORCE, o->Position, o->Position, o->Angle, 20, o, 400.0f, 40);
				}
				o->Position[2] += 20.0f;
				EVector3 ap, P, dp;
				EMatrix4 matrix;
				VectorCopy(o->Position, ap);
				Vector(0.0f, -20.0f, 0.0f, P);
				AngleMatrix(o->Angle, matrix);
				VectorRotate(P, matrix, dp);
				VectorAdd(dp, o->Position, o->Position);
				CreateParticle(ThreadIndex, BITMAP_BLUE_BLUR, o->Position, o->Angle, Light, 1, 1.0f);
			}
		}
		else if (Type == MODEL_ARROW_SPARK)
		{
			o->Scale = 1.0f;
			o->Direction[1] = -70.0f;
			CreateJoint(ThreadIndex, BITMAP_FLARE + 1, o->Position, o->Position, o->Angle, 14, o, 50.0f, 40);
		}
		else if (Type == MODEL_ARROW_DARKSTINGER)
		{
			o->Scale = 0.8f;
			o->Direction[1] = -70.0f;
			o->LifeTime = 30;
		}
		else if (Type == MODEL_ARROW_GAMBLE)
		{
			o->Scale = 0.8f;
			o->Direction[1] = -70.0f;
			o->LifeTime = 30;
		}
		else
		{
			o->Scale = 0.8f;
			o->Direction[1] = -70.0f;
		}

		if (o->SubType == 2)
		{
			CreateEffect(ThreadIndex, MODEL_PIERCING, o->Position, o->Angle, o->Light, 0, o);
			o->AttackPoint[0] = 0;
			o->Kind = 1;
		}

		if (Type == MODEL_ARROW_NATURE && o->SubType == 1)
		{
			CreateJoint(ThreadIndex, BITMAP_FLARE + 1, o->Position, o->Position, o->Angle, 13, o, 20.0f, 40);
		}

		if (Type == MODEL_ARROW && (o->SubType == 3 || o->SubType == 4))
		{
			o->LifeTime = 40;
			o->Scale = 1.5f;
			o->Gravity = (GetLargeRand(100) + 50) / 15.0f;
			VectorCopy(Hero->Object.Position, o->StartPosition);
			if (o->SubType == 3)
			{
				o->Direction[1] = -(GetLargeRand(30) + 50.0f);
				o->Angle[0] = -(GetLargeRand(20) + 45.0f);
			}
			else
			{
				o->Direction[1] = -(GetLargeRand(15) + 55.0f);
				o->Angle[0] = -10.0f;
			}
		}
		else if (Type == MODEL_LACEARROW)
		{
			o->BlendMesh = 1;
			o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
			CreateJoint(ThreadIndex, BITMAP_FLARE + 1, o->Position, o->Position, o->Angle, 12, o, 40.0f, 80);
		}
		else if (Type == MODEL_ARROW_RING)
		{
			o->BlendMesh = 0;
			o->Scale = 1.0f;
			o->Direction[1] = -70.0f;
			o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
			CreateJoint(ThreadIndex, BITMAP_FLARE + 1, o->Position, o->Position, o->Angle, 15, o, 50.0f, 40);
		}

		o->Weapon = 0;
		break;
	case MODEL_CURSEDTEMPLE_HOLYITEM:
		{
			o->LifeTime = 9999999;
		}
		break;
	case MODEL_CURSEDTEMPLE_PRODECTION_SKILL:
		{
			o->LifeTime = 9999999;
		}
		break;
	case MODEL_CURSEDTEMPLE_RESTRAINT_SKILL:
		{
			o->LifeTime = 9999999;
		}
		break;
	case MODEL_ARROW_BOMB:
		o->Velocity = 1.0f;
		o->LifeTime = 30;
		AngleMatrix(o->Angle, Matrix);
		Vector(-10.0f, -60.0f, 135.0f, p1);
		VectorRotate(p1, Matrix, p2);
		VectorAdd(o->Position, p2, o->Position);

		o->BlendMesh = 0;
		o->Scale = 1.0f;
		o->Direction[1] = -30.0f;
		o->Gravity = -10.0f;
		o->LifeTime = 40;

		if (o->SubType == 2)
		{
			o->AttackPoint[0] = 0;
			o->Kind = 1;
		}

		o->Weapon = 0;
		break;
	case MODEL_SAW:
		o->LifeTime = 10;
		o->Position[2] += 130.0f;
		AngleMatrix(o->Angle, Matrix);
		Vector(0.0f, -60.0f, 0.0f, p1);
		VectorRotate(p1, Matrix, o->Direction);
		break;
	case MODEL_LASER:
		if (o->SubType == 0 || o->SubType == 3)
		{
			o->LifeTime = 1;
			o->BlendMesh = 0;
			o->BlendMeshLight = o->Light[0];
			o->Scale = 1.3f;
			o->RenderType = RENDER_DARK;
		}
		else
		{
			o->LifeTime = 30;
			o->Velocity = 1.0f;
			o->BlendMesh = 0;
			o->BlendMeshLight = 1.0f;
			o->Scale = 1.3f;
			o->RenderType = RENDER_DARK;

			o->Position[2] += 150.0f;
			Vector(0.0f, -1.0f, 0.0f, o->Direction);
			Vector(1.0f, 0.0f, 0.0f, o->Light);
			Vector(30.0f, 0.0f, Angle[2], o->Angle);

			CreateJoint(ThreadIndex, BITMAP_JOINT_FORCE, o->Position, o->Position, o->Angle, 1, o, 180.0f);
		}
		break;
	case MODEL_MAGIC1:
		o->LifeTime = 20;
		o->BlendMesh = 0;
		break;
	case MODEL_SKILL_WHEEL1:
		o->LifeTime = 6;
		break;
	case MODEL_SKILL_WHEEL2:
		o->LifeTime = 25;
		o->Weapon = 0;
		break;
	case MODEL_SKILL_FURY_STRIKE:
		{
			VectorCopy(o->Angle, o->HeadAngle);
			VectorCopy(o->Position, o->StartPosition);

			o->LifeTime = 20;
			o->SubType = GetLargeRand(100);
			o->Angle[2] += 330.0f;
			o->HeadAngle[0] += 80.0f;
			o->HeadAngle[2] += 180.0f;
			o->Gravity = 50.0f;

			o->Weapon = 0;
		}
		break;
	case MODEL_SKILL_FURY_STRIKE + 1:
		if (o->SubType == 0)
		{
			o->LifeTime = 35;
			o->Scale = PKKey / 100.0f;
			o->BlendMesh = 0;
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 60;
			o->Scale = PKKey / 100.0f;
			o->BlendMesh = 0;
		}
		break;
	case MODEL_SKILL_FURY_STRIKE + 2:
		if (o->SubType == 0 || o->SubType == 1)
		{
			o->LifeTime = 20;
			o->Scale = PKKey / 100.0f;
			o->BlendMesh = 0;
			if (SubType == 1)
				o->RenderType = RENDER_DARK;
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = 50;
			o->Scale = PKKey / 100.0f;
			o->BlendMesh = 0;
		}
		break;
	case MODEL_SKILL_FURY_STRIKE + 3:
		o->LifeTime = 35;
		o->Scale = PKKey / 100.0f;
		break;
	case MODEL_SKILL_FURY_STRIKE + 4:
		o->LifeTime = 35;
		o->Scale = PKKey / 100.0f;
		o->BlendMesh = 0;
		break;
	case MODEL_SKILL_FURY_STRIKE + 5:
		o->LifeTime = 40;
		o->Scale = PKKey / 100.0f;
		o->BlendMesh = 0;
		if (SubType == 1)
			o->RenderType = RENDER_DARK;
		break;
	case MODEL_SKILL_FURY_STRIKE + 6:
		o->LifeTime = 35;
		o->Scale = PKKey / 100.0f;
		break;
	case MODEL_SKILL_FURY_STRIKE + 7:
		o->LifeTime = 40;
		o->Scale = PKKey / 100.0f;
		o->BlendMesh = 0;
		break;
	case MODEL_SKILL_FURY_STRIKE + 8:
		o->LifeTime = 40;
		o->Scale = PKKey / 100.0f;
		o->BlendMesh = 0;
		if (SubType == 1)
			o->RenderType = RENDER_DARK;
		break;
	case MODEL_SKILL_FISSURE:
		o->LifeTime = 20;
		break;
	case MODEL_FISSURE:
	case MODEL_FISSURE_LIGHT:
		o->LifeTime = 120;
		o->Scale = 0.8f;
		break;
	case MODEL_BALGAS_SKILL:
		o->BlendMesh = 0;
		o->LifeTime = 20;
		o->Scale = 1.0f;
		break;
	case MODEL_CHANGE_UP_EFF:
		o->BlendMesh = -2;
		o->LifeTime = 100;
		o->Scale = 0.7f;
		o->Position[0] = Position[0];
		o->Position[1] = Position[1];
		o->Position[2] = Position[2] + 22.0f;
		Vector(0.0f, 0.0f, 0.0f, o->Direction);
		if (o->SubType == 1)
		{
			o->Scale = 0.4f;
			o->LifeTime = 10;
			o->BlendMeshLight = 0.7f;
		}
		break;
	case MODEL_CHANGE_UP_NASA:
		o->BlendMesh = -2;
		o->LifeTime = 100;
		o->Scale = 0.9f;
		if (o->SubType >= 1 && o->SubType <= 3)
			o->LifeTime = 80;
		
		o->Position[0] = Position[0];
		o->Position[1] = Position[1];
		o->Position[2] = Position[2] + 12.0f;
		Vector(0.0f, 0.0f, 0.0f, o->Direction);
		break;
	case MODEL_CHANGE_UP_CYLINDER:
		o->BlendMesh = -2;
		o->LifeTime = 100;
		o->Scale = 0.9f;
		Vector(0.0f, 0.0f, 1.0f, o->Direction);
		if (o->SubType == 1)
		{
			
			Vector(0.0f, 0.0f, 0.0f, o->Light);
			o->BlendMesh = -2;
			o->LifeTime = 10;
			o->Scale = 0.1f;
		}
		break;
	case MODEL_DARK_ELF_SKILL:
		o->BlendMesh = -2;
		o->LifeTime = 30;
		o->Scale = 0.0f;
		Vector(0.0f, -40.0f, 0.0f, o->Direction);
		break;
	case MODEL_MAGIC2:
		o->BlendMesh = 0;
		o->LifeTime = 20;
		Vector(0.0f, -60.0f, 0.0f, o->Direction);
		if (o->SubType == 2)
		{
			o->Weapon = 0;
		}
		break;
	case MODEL_STORM:
		switch (o->SubType)
		{
		case 0:
			o->LifeTime = 59;
			o->BlendMesh = 0;
			Vector(0.0f, -10.0f, 0.0f, o->Direction);
			
			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
			o->Weapon = 0;
			break;

		case 1:
			o->LifeTime = 30;
			o->HiddenMesh = -2;
			o->Scale = 1.0f;
			o->BlendMesh = 0;
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			break;

		case 2:
			o->LifeTime = 59;
			o->HiddenMesh = -2;
			o->Scale = 1.0f;
			o->BlendMesh = 0;
			Vector(0.0f, -10.0f, 0.0f, o->Direction);
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			break;
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			{
				o->LifeTime = 60;
				o->HiddenMesh = -2;
				o->Scale = 1.0f;

				if (o->SubType == 3) { Vector(0.0f, -12.0f, 0.0f, o->Direction); }
				else if (o->SubType == 4) { Vector(7.0f, -6.0f, 0.0f, o->Direction); }
				else if (o->SubType == 5) { Vector(-7.0f, -6.0f, 0.0f, o->Direction); }
				else if (o->SubType == 6) { Vector(4.0f, 5.0f, 0.0f, o->Direction); }
				else { Vector(-4.0f, 5.0f, 0.0f, o->Direction); }
			}
			break;
		case 8:
			{
				o->LifeTime = 100;
				o->BlendMesh = 0;
				o->Scale = 2.0f;
			}
			break;
		}
		break;
	case MODEL_SUMMON:
		{
			o->LifeTime = 60;
			o->BlendMesh = 0;
			if (o->SubType == 0)
				o->Scale = 0.7f;
			else
				o->Scale = 1.2f;
			o->Angle[2] += GetLargeRand(360);
		}
		break;
	case MODEL_STORM2:
		{
			if (o->SubType == 0)
			{
				o->Angle[0] = 90.0f;
				o->LifeTime = 35;
				o->BlendMesh = 0;
				o->Scale = 0.5f + GetLargeRand(10) / 100.0f;
			}
			else if (o->SubType == 1)
			{
				o->LifeTime = 60;
				o->BlendMesh = 0;
				o->Gravity = GetLargeRand(10) + 30.0f;
			}
			else if (o->SubType == 2)
			{
				o->LifeTime = 100;
				o->Angle[0] = 90.0f;
				o->BlendMesh = 0;
				o->Scale = 1.0f;
			}
		}
		break;
	case MODEL_STORM3:
		{
			o->LifeTime = 50;
			o->BlendMesh = 0;
			o->Angle[0] = 90.0f;
			o->Scale = 3.5f;
			VectorCopy(Hero->Object.Position, o->StartPosition);
		}
		break;
	case MODEL_MAYASTAR:
		{
			o->LifeTime = 50;
			o->Scale = 50.0f;
		}
		break;
	case MODEL_MAYASTONE1:	
	case MODEL_MAYASTONE2:
	case MODEL_MAYASTONE3:
		{
			o->LifeTime = 40;
			o->Scale = 6.0f + static_cast<mu_float>(GetLargeRand(8) + 15)*0.1f;
			Vector(0.0f, 0.0f, -60.0f, o->Direction);
			Vector(0.0f, 30.0f, 0.0f, o->HeadAngle);

			CreateJoint(ThreadIndex, BITMAP_SMOKE, o->Position, o->Position, o->HeadAngle, 2, o, 100.0f);
		}
		break;
	case MODEL_MAYASTONE4:
	case MODEL_MAYASTONE5:
		Vector(0.0f, static_cast<mu_float>(GetLargeRand(256) + 64)*0.1f, 0.0f, p1);
		o->LifeTime = GetLargeRand(16) + 32;
		o->Scale = static_cast<mu_float>(GetLargeRand(10)) / 3.0f + 1.0f;
		o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
		AngleMatrix(o->Angle, Matrix);
		VectorRotate(p1, Matrix, o->Direction);
		o->Gravity = static_cast<mu_float>(GetLargeRand(16) + 8);
		break;
	case MODEL_MAYASTONEFIRE:	
		{
			if (o->SubType == MODEL_MAYASTONE1)
				o->Scale = Scale*0.8f;
			else if (o->SubType == MODEL_MAYASTONE2)
				o->Scale = Scale*0.6f;
			else if (o->SubType == MODEL_MAYASTONE3)
				o->Scale = Scale*0.5f;
			o->LifeTime = 40;
			Vector(0.0f, 0.0f, -60.0f, o->Direction);
			Vector(0.0f, 30.0f, 0.0f, o->HeadAngle);
		}
		break;
	case MODEL_MAYAHANDSKILL:	
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 20;
				
				VectorCopy(o->Light, o->StartPosition);
			}
			else if (o->SubType == 1)
			{
				o->LifeTime = 20;
				
				VectorCopy(o->Light, o->StartPosition);
			}
		}
		break;
		
	case MODEL_CIRCLE:
		o->LifeTime = 45;
		o->BlendMesh = 0;
		if (o->SubType == 0)
		{

		}
		else if (o->SubType == 1 || o->SubType == 4)
		{
			o->LifeTime = 45;
			o->Scale = 1.0f;
			o->HiddenMesh = -2;
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = 250;
			o->Scale = 1.0f;
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = 30;
			o->Scale = 1.0f;
		}
		break;
		
	case MODEL_CIRCLE_LIGHT:
		o->LifeTime = 40;
		o->BlendMesh = 0;
		
		if (SubType == 1)
			o->RenderType = RENDER_DARK;
		else if (o->SubType == 2)
		{
			o->LifeTime = 40;
			o->Scale = 1.0f;
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = 250;
			o->Scale = 1.0f;
		}
		else if (o->SubType == 4)
		{
			o->LifeTime = 20;
			o->Scale = 1.0f;
		}
		break;
		
	case MODEL_BLOOD:
		o->LifeTime = 10;
		o->BlendMesh = 0;
		break;
	case MODEL_ICE:
		switch (o->SubType)
		{
		case 0:
			o->LifeTime = 50;
			o->Scale = 0.8f;
			o->Velocity = 1.0f;
			o->Angle[0] = 0.0f;
			o->BlendMesh = 0;
			break;

		case 1:
		case 2:
			o->LifeTime = 20;
			o->Scale = 0.8f;
			o->Angle[0] = -20.0f;
			o->BlendMesh = 0;
			o->BlendMeshLight = 0.5f;
			o->Gravity = 5.0f;
			Vector(0.0f, 0.0f, static_cast<mu_float>(GetLargeRand(360)), o->HeadAngle);

			
			for (mu_int32 i = 0; i<3; ++i)
			{
				EVector3 Position;
				Vector(o->Position[0] + static_cast<mu_float>(GetLargeRand(64) - 32),
					o->Position[1] + static_cast<mu_float>(GetLargeRand(64) - 32),
					o->Position[2] + static_cast<mu_float>(GetLargeRand(128) + 32), Position);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, o->Light);
			}
			Vector(0.4f, 0.3f, 0.2f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);

			VectorCopy(o->Position, o->StartPosition);
			break;
		}
		break;
	case MODEL_SNOW1:
		AngleMatrix(o->Angle, Matrix);
		Vector(0.0f, -40.0f, 150.0f, p1);
		VectorRotate(p1, Matrix, p2);
		VectorAdd(o->Position, p2, o->Position);
		o->Direction[1] = -40.0f;
		o->Direction[2] = 10.0f;
		o->LifeTime = 20;
		o->Scale = 1.2f;
		break;
	case MODEL_WOOSISTONE:
		if (o->SubType == 1)	
		{
			o->Direction[0] = 0;
			o->Direction[1] = 0;
			o->Direction[2] = 0;

			o->LifeTime = GetLargeRand(16) + 20;
			o->Scale = static_cast<mu_float>(GetLargeRand(13) + 3)*0.04f;
			o->Gravity = static_cast<mu_float>(GetLargeRand(3) + 3);

			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			Vector(0.0f, static_cast<mu_float>(GetLargeRand(128) + 64)*0.1f, 0.0f, p1);
			VectorRotate(p1, Matrix, o->HeadAngle);
			o->HeadAngle[2] += 15.0f;
			break;
		}
		else	
		{
			AngleMatrix(o->Angle, Matrix);
			Vector(0.0f, -60.0f, 150.0f, p1);
			VectorRotate(p1, Matrix, p2);
			VectorAdd(o->Position, p2, o->Position);
			o->Direction[1] = -40.0f;
			o->Direction[2] = 10.0f;
			o->LifeTime = 20;
			o->Scale = 1.0f;
			break;
		}
	case MODEL_SKULL:
		if (o->SubType == 0)
		{
			o->LifeTime = 1;
			o->Scale = 2.3f;
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 49;
			o->Scale = 2.0f;
			o->BlendMesh = -2;
			o->BlendMeshLight = 0.5f;
			Vector(0.0f, 0.0f, 0.0f, o->HeadAngle);
			Vector(0.0f, -45.0f, 0.0f, o->Direction);
			VectorCopy(o->Position, o->StartPosition);
			VectorCopy(o->Position, o->m_vDeadPosition);

			VectorCopy(o->Position, o->EyeLeft);
			VectorCopy(o->Position, o->EyeRight);

			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 10, o, 30.0f);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 11, o, 30.0f);
		}
		break;
	case MODEL_CUNDUN_PART1:
	case MODEL_CUNDUN_PART2:
	case MODEL_CUNDUN_PART3:
	case MODEL_CUNDUN_PART4:
	case MODEL_CUNDUN_PART5:
	case MODEL_CUNDUN_PART6:
	case MODEL_CUNDUN_PART7:
	case MODEL_CUNDUN_PART8:
	case MODEL_MONSTER01 + 64:	
		switch (o->SubType)
		{
		case 1:	
			o->LifeTime = 100;
			o->Scale = 2.0f;
			break;
		case 2:	
		case 3:	
			Vector(o->Position[0], o->Position[1],
				RequestTerrainHeight(o->Position[0], o->Position[1]) - static_cast<mu_float>(o->PKKey) - 80,
				o->StartPosition);
			Vector(0, 0, 0, o->Direction);
			o->LifeTime = 250 * 2;
			o->Scale = 2.0f;
			o->HeadAngle[0] = static_cast<mu_float>(GetLargeRand(2));
			if (o->SubType == 2) o->HeadAngle[0] = 0;
			break;
		case 4:	
			Vector(o->Position[0], o->Position[1],
				RequestTerrainHeight(o->Position[0], o->Position[1]) - static_cast<mu_float>(o->PKKey),
				o->StartPosition);
			Vector(0, 0, 0, o->Direction);
		case 5:	
			o->CurrentAction = 0;
			o->LifeTime = 170;
			o->Scale = 2.0f;
			break;
		}
		
		o->PKKey = -1;
		o->Owner = Owner;
		break;
	case MODEL_CURSEDTEMPLE_STATUE_PART1:
	case MODEL_CURSEDTEMPLE_STATUE_PART2:
		o->LifeTime = 180 + (GetLargeRand(40));
		o->Scale = 0.1f + (GetLargeRand(6))*0.1f;
		o->Velocity = 0.0f;
		o->PKKey = -1;
		o->Owner = Owner;
		o->Gravity = 2.5f;
		o->Direction[0] = static_cast<mu_float>((-15 + (GetLargeRand(30))));
		o->Direction[1] = static_cast<mu_float>((-15 + (GetLargeRand(30))));
		o->Direction[2] = 0;
		o->Angle[0] = 0;
		o->Angle[1] = 0;
		o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
		o->Light[0] = o->Light[1] = o->Light[2] = 0.5f + (GetLargeRand(6))*0.1f;
		break;
	case MODEL_XMAS2008_SNOWMAN_HEAD:
		{
			o->LifeTime = 50;
			o->Scale = 1.3f;

			o->Angle[2] = o->Owner->Angle[2];
			AngleMatrix(o->Angle, Matrix);
			o->Gravity = 5.0f;
			EVector3 p;
			Vector(static_cast<mu_float>(GetLargeRand(10) - 5)*0.1f, static_cast<mu_float>(GetLargeRand(60) - 40)*0.1f, 0.0f, p);
			VectorScale(p, 1.3f, p);
			VectorRotate(p, Matrix, o->Direction);
			o->m_iAnimation = GetLargeRand(3);
		}
		break;
	case MODEL_XMAS2008_SNOWMAN_BODY:
		{
			o->LifeTime = 50;
			o->Scale = 1.3f;
			o->Velocity = 1.2f;

			o->PKKey = -1;
			o->Owner = Owner;

			o->Direction[0] = o->Owner->Direction[0];
			o->Direction[1] = o->Owner->Direction[1];
			o->Direction[2] = o->Owner->Direction[2];
		}
		break;
	case MODEL_TOTEMGOLEM_PART1:
	case MODEL_TOTEMGOLEM_PART2:
	case MODEL_TOTEMGOLEM_PART3:
	case MODEL_TOTEMGOLEM_PART4:
	case MODEL_TOTEMGOLEM_PART5:
	case MODEL_TOTEMGOLEM_PART6:
		o->LifeTime = 30 + (GetLargeRand(30));
		o->Scale = 0.17f;
		o->Velocity = 0.0f;
		o->PKKey = -1;
		o->Owner = Owner;
		o->Gravity = 3.5f;

		o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
		AngleMatrix(o->Angle, Matrix);
		Vector(0.0f, static_cast<mu_float>(GetLargeRand(64) + 48)*0.1f, 0.0f, p1);
		VectorRotate(p1, Matrix, o->HeadAngle);
		o->HeadAngle[2] = 15;

		o->SubType = GetLargeRand(2);

		o->Light[0] = o->Light[1] = o->Light[2] = 0.5f + (GetLargeRand(6))*0.1f;
		o->Direction[0] = 0;
		o->Direction[1] = 0;
		o->Direction[2] = 0;
		break;
	case MODEL_DOPPELGANGER_SLIME_CHIP:
		o->LifeTime = 30 + (GetLargeRand(10));
		o->Scale = 0.1f + (GetLargeRand(7))*0.1f;
		o->Velocity = 0.0f;
		o->PKKey = -1;
		o->Owner = Owner;
		o->Gravity = 4.0f;

		o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
		AngleMatrix(o->Angle, Matrix);
		Vector(0.0f, static_cast<mu_float>(GetLargeRand(64) + 48)*0.1f, 0.0f, p1);
		VectorRotate(p1, Matrix, o->HeadAngle);
		o->HeadAngle[2] = 35 + GetLargeRand(5);

		o->SubType = GetLargeRand(2);

		VectorCopy(Light, o->Light);
		o->Direction[0] = 0;
		o->Direction[1] = 0;
		o->Direction[2] = 0;

		o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
		o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
		break;
	case MODEL_SHADOW_PAWN_ANKLE_LEFT:		case MODEL_SHADOW_PAWN_ANKLE_RIGHT:
	case MODEL_SHADOW_PAWN_BELT:			case MODEL_SHADOW_PAWN_CHEST:
	case MODEL_SHADOW_PAWN_HELMET:
	case MODEL_SHADOW_PAWN_KNEE_LEFT:		case MODEL_SHADOW_PAWN_KNEE_RIGHT:
	case MODEL_SHADOW_PAWN_WRIST_LEFT:		case MODEL_SHADOW_PAWN_WRIST_RIGHT:

	case MODEL_SHADOW_KNIGHT_ANKLE_LEFT:	case MODEL_SHADOW_KNIGHT_ANKLE_RIGHT:
	case MODEL_SHADOW_KNIGHT_BELT:			case MODEL_SHADOW_KNIGHT_CHEST:
	case MODEL_SHADOW_KNIGHT_HELMET:
	case MODEL_SHADOW_KNIGHT_KNEE_LEFT:		case MODEL_SHADOW_KNIGHT_KNEE_RIGHT:
	case MODEL_SHADOW_KNIGHT_WRIST_LEFT:	case MODEL_SHADOW_KNIGHT_WRIST_RIGHT:

	case MODEL_ICE_GIANT_PART1:				case MODEL_ICE_GIANT_PART2:
	case MODEL_ICE_GIANT_PART3:				case MODEL_ICE_GIANT_PART4:
	case MODEL_ICE_GIANT_PART5:				case MODEL_ICE_GIANT_PART6:

		o->LifeTime = 30 + (GetLargeRand(30));
		o->Scale = 1.1f;
		o->Velocity = 0.0f;
		o->PKKey = -1;
		o->Owner = Owner;
		o->Gravity = 3.5f;

		o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
		AngleMatrix(o->Angle, Matrix);
		Vector(0.0f, static_cast<mu_float>(GetLargeRand(64) + 48)*0.1f, 0.0f, p1);
		VectorRotate(p1, Matrix, o->HeadAngle);
		o->HeadAngle[2] = 15;

		o->SubType = GetLargeRand(2);

		o->Light[0] = o->Light[1] = o->Light[2] = 0.5f + (GetLargeRand(6))*0.1f;
		o->Direction[0] = 0;
		o->Direction[1] = 0;
		o->Direction[2] = 0;
		break;
	case MODEL_SHADOW_ROOK_ANKLE_LEFT:		case MODEL_SHADOW_ROOK_ANKLE_RIGHT:
	case MODEL_SHADOW_ROOK_BELT:			case MODEL_SHADOW_ROOK_CHEST:
	case MODEL_SHADOW_ROOK_HELMET:
	case MODEL_SHADOW_ROOK_KNEE_LEFT:		case MODEL_SHADOW_ROOK_KNEE_RIGHT:
	case MODEL_SHADOW_ROOK_WRIST_LEFT:		case MODEL_SHADOW_ROOK_WRIST_RIGHT:
		o->LifeTime = 30 + (GetLargeRand(30));
		o->Scale = 1.3f;
		o->Velocity = 0.0f;
		o->PKKey = -1;
		o->Owner = Owner;
		o->Gravity = 3.5f;

		o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
		AngleMatrix(o->Angle, Matrix);
		Vector(0.0f, static_cast<mu_float>(GetLargeRand(64) + 48)*0.1f, 0.0f, p1);
		VectorRotate(p1, Matrix, o->HeadAngle);
		o->HeadAngle[2] = 15;

		o->SubType = GetLargeRand(2);

		o->Light[0] = o->Light[1] = o->Light[2] = 0.5f + (GetLargeRand(6))*0.1f;
		o->Direction[0] = 0;
		o->Direction[1] = 0;
		o->Direction[2] = 0;
		break;

	case MODEL_WATER_WAVE:
		o->BlendMesh = -2;
		o->BlendMeshLight = 0.2f;
		o->Scale = 0.4f;
		o->LifeTime = 20;
		o->Gravity = 120.0f;
		o->Velocity = -40.0f;
		Vector(0.0f, o->Velocity, 0.0f, o->Direction);
		Vector(1.0f, 1.0f, 1.0f, o->Light);
		VectorCopy(o->Position, o->StartPosition);
		break;

	case MODEL_FIRE:
		o->BlendMesh = 1;
		if (o->SubType == 0)
		{
			o->LifeTime = 40;
			o->Scale = static_cast<mu_float>(GetLargeRand(8) + 10)*0.1f;
			o->Position[0] += 130.0f + GetLargeRand(32);
			o->Position[2] += 400.0f;
			Vector(0.0f, 0.0f, -50.0f, o->Direction);
			Vector(0.0f, 20.0f, 0.0f, o->Angle);
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = 40;
			o->Scale = static_cast<mu_float>(GetLargeRand(8) + 10)*0.1f;
			Vector(0.0f, 0.0f, -50.0f, o->Direction);
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = 80;
			o->Scale = 0.3f;
			Vector(0.0f, -12.0f, 0.0f, o->Direction);
		}
		else if (o->SubType == 4)   
		{
			o->LifeTime = 40;
			o->Scale = static_cast<mu_float>(GetLargeRand(10) + 15)*0.1f;
			o->Position[0] += 130.0f + GetLargeRand(32);
			o->Position[2] += 400.0f;
			Vector(0.0f, -(GetLargeRand(20) + 10.0f), -(GetLargeRand(10) + 20.0f), o->Direction);
			Vector(0.0f, 20.0f, 0.0f, o->Angle);
		}
		else if (o->SubType == 5)   
		{
			o->LifeTime = 40;
			o->Gravity = 5.0f;
			Vector(0.0f, -30.0f, 0.0f, o->Direction);
		}
		else if (o->SubType == 6)	
		{
			o->LifeTime = 40;
			o->HiddenMesh = -2;
			o->Scale = static_cast<mu_float>(GetLargeRand(8) + 15)*0.1f;
			o->PKKey = -1;
			o->Velocity = PKKey;
			o->Position[0] += GetLargeRand(100) + 200;
			o->Position[1] += GetLargeRand(100) - 50;
			o->Position[2] += GetLargeRand(300) + 500;
			Vector(0.0f, 0.0f, -50.0f - GetLargeRand(50), o->Direction);
			Vector(0.0f, 20.0f, 0.0f, o->HeadAngle);

			CreateJoint(ThreadIndex, BITMAP_SMOKE, o->Position, o->Position, o->HeadAngle, 0, o, 100.0f);
		}
		else if (o->SubType == 7)	
		{
			o->LifeTime = 40;
			o->HiddenMesh = -2;
			o->Scale = static_cast<mu_float>(GetLargeRand(8) + 15)*0.1f;
			o->PKKey = -1;
			o->Velocity = PKKey;
			o->Position[0] += GetLargeRand(100) + 200;
			o->Position[1] += GetLargeRand(100) - 50;
			o->Position[2] += GetLargeRand(300) + 500;
			Vector(0.0f, 0.0f, -50.0f - GetLargeRand(50), o->Direction);
			Vector(0.0f, 20.0f, 0.0f, o->HeadAngle);
		}
		else if (o->SubType == 8)	
		{
			o->LifeTime = 40;
			o->HiddenMesh = -2;
			o->Scale = static_cast<mu_float>(GetLargeRand(8) + 15)*0.1f;
			o->PKKey = -1;
			o->Velocity = PKKey;
			o->Position[2] += GetLargeRand(300) + 500;
			Vector(0.0f, 0.0f, -50.0f - GetLargeRand(50), o->Direction);
			Vector(0.0f, static_cast<mu_float>(5 + GetLargeRand(5)), 0.0f, o->HeadAngle);

			CreateJoint(ThreadIndex, BITMAP_SMOKE, o->Position, o->Position, o->HeadAngle, 0, o, 60.0f);
		}
		else if (o->SubType == 9)
		{
			o->BlendMeshLight = 0.0f;
			o->LifeTime = 600;
			o->Scale = static_cast<mu_float>(GetLargeRand(10) + 20)*0.13f;
			o->Position[0] += 130.0f + GetLargeRand(32);
			o->Position[2] += 400.0f + GetLargeRand(32);
			Vector(0.0f, -GetLargeRand(5) - 8.0f, -10.0f - GetLargeRand(10), o->Direction);
			Vector(0.0f, 0.0f, 0.0f, o->Angle);
		}
		else
		{
			o->LifeTime = 60;
			o->Scale = static_cast<mu_float>(GetLargeRand(4) + 8)*0.1f;
			o->Position[2] += 120.0f;
			Vector(0.0f, -50.0f, 0.0f, o->Direction);
		}
		break;
	case MODEL_BONE1:
		o->Position[2] += 50.0f;
	case MODEL_BONE2:
		o->Position[2] += 100.0f;
	case MODEL_BIG_STONE1:
	case MODEL_BIG_STONE2:
		if (o->SubType == 5)
		{
			o->LifeTime = 60;
			o->Scale = static_cast<mu_float>(GetLargeRand(4) + 8)*0.1f;
			VectorCopy(o->Position, o->StartPosition);
			VectorCopy(o->Owner->Position, o->Position);
			break;
		}
	case MODEL_SNOW2:
	case MODEL_SNOW3:
	case MODEL_STONE1:
	case MODEL_STONE2:
		if (o->SubType == 5)
		{
			o->LifeTime = 60;
			o->Scale = static_cast<mu_float>(GetLargeRand(4) + 8)*0.1f;
			break;
		}
		else if (o->SubType == 11)
		{
			o->LifeTime = 40;
			o->Scale = static_cast<mu_float>(GetLargeRand(4) + 8)*0.8f;
			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
			break;
		}
		else if (o->SubType == 10)	
		{
			Vector(0.0f, static_cast<mu_float>(GetLargeRand(256) + 64)*0.2f, 0.0f, p1);
			o->LifeTime = GetLargeRand(16) + 32;
			o->Scale = static_cast<mu_float>(GetLargeRand(4) + 15)*0.05f;
			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			VectorRotate(p1, Matrix, o->Direction);
			o->Gravity = static_cast<mu_float>(GetLargeRand(16) + 28);
			break;
		}
		else if (o->SubType == 12)	
		{
			Vector(0.0f, static_cast<mu_float>(GetLargeRand(256) + 64)*0.1f, 0.0f, p1);
			o->LifeTime = GetLargeRand(16) + 32;
			o->Scale = static_cast<mu_float>(GetLargeRand(4) + 15)*0.1f;
			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			VectorRotate(p1, Matrix, o->Direction);
			o->Gravity = static_cast<mu_float>(GetLargeRand(16) + 8);
			break;
		}
		else if (o->SubType == 13)
		{
			o->Direction[0] = 0;
			o->Direction[1] = 0;
			o->Direction[2] = 0;

			o->LifeTime = GetLargeRand(16) + 20;
			o->Scale = static_cast<mu_float>(GetLargeRand(13) + 3)*0.08f;
			o->Gravity = static_cast<mu_float>(GetLargeRand(3) + 3);

			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			Vector(0.0f, static_cast<mu_float>(GetLargeRand(128) + 64)*0.1f, 0.0f, p1);
			VectorRotate(p1, Matrix, o->HeadAngle);
			o->HeadAngle[2] += 15.0f;
			break;
		}

	case MODEL_ICE_SMALL:
	case MODEL_METEO1:
	case MODEL_METEO2:
	case MODEL_EFFECT_SAPITRES_ATTACK_2:		
		if (o->Type == MODEL_BIG_STONE1 || o->Type == MODEL_BIG_STONE2)
		{
			Vector(static_cast<mu_float>(GetLargeRand(128) - 64), static_cast<mu_float>(GetLargeRand(128) - 64), static_cast<mu_float>(GetLargeRand(180)), p1);
			VectorAdd(o->Position, p1, o->Position);
		}
		if (Type == MODEL_ICE_SMALL)
		{
			o->BlendMesh = 0;
			o->BlendMeshLight = 0.3f;
			Vector(0.0f, static_cast<mu_float>(GetLargeRand(256) + 64)*0.1f, 0.0f, p1);
			o->Position[2] += 50.0f;
			if (o->SubType == 13)
			{
				Vector(0.0f, static_cast<mu_float>(GetLargeRand(256) + 64)*0.2f, 0.0f, p1);
				o->LifeTime = GetLargeRand(16) + 32;
				o->Scale = static_cast<mu_float>(GetLargeRand(4) + 15)*0.05f;
				o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
				AngleMatrix(o->Angle, Matrix);
				VectorRotate(p1, Matrix, o->Direction);
				o->Gravity = static_cast<mu_float>(GetLargeRand(16) + 28);
			}
		}
		else if (Type == MODEL_EFFECT_SAPITRES_ATTACK_2)
		{
			o->BlendMesh = 0;
			o->BlendMeshLight = 1.0f;
			Vector(0.0f, static_cast<mu_float>(GetLargeRand(256) + 64)*0.1f, 0.0f, p1);
			o->Position[2] += 50.0f;
		}
		else
		{
			if (o->SubType == 1)
			{
				Vector(0.0f, 0.0f, 0.0f, p1);
			}
			else
			{
				Vector(0.0f, static_cast<mu_float>(GetLargeRand(256) + 64)*0.1f, 0.0f, p1);
			}
		}
		if (o->SubType == 13)
		{
			o->LifeTime = GetLargeRand(16) + 100;
			o->Scale = static_cast<mu_float>(GetLargeRand(4) + 8)*0.1f;
			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			VectorRotate(p1, Matrix, o->Direction);
			o->Gravity = static_cast<mu_float>(GetLargeRand(16) + 8);
		}
		else if (Type == MODEL_EFFECT_SAPITRES_ATTACK_2 && o->SubType == 14)
		{
			o->LifeTime = GetLargeRand(5) + 15;
			o->Angle[0] = static_cast<mu_float>(GetLargeRand(60) - 30);
			o->Angle[1] = static_cast<mu_float>(GetLargeRand(60) - 30);
			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			VectorRotate(p1, Matrix, o->Direction);
			VectorNormalize(o->Direction);
			o->Gravity = 0.0f;
		}
		else
		{
			o->LifeTime = GetLargeRand(16) + 32;
			o->Scale = static_cast<mu_float>(GetLargeRand(4) + 8)*0.1f;
			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			VectorRotate(p1, Matrix, o->Direction);
			o->Gravity = static_cast<mu_float>(GetLargeRand(16) + 8);
		}
		break;

	case MODEL_EFFECT_BROKEN_ICE0:
	case MODEL_EFFECT_BROKEN_ICE1:
	case MODEL_EFFECT_BROKEN_ICE2:
	case MODEL_EFFECT_BROKEN_ICE3:
		if (o->SubType == 0)	
		{
			o->Direction[0] = 0;
			o->Direction[1] = 0;
			o->Direction[2] = 0;

			o->LifeTime = GetLargeRand(16) + 35;
			o->Scale = static_cast<mu_float>(GetLargeRand(13) + 3)*0.2f;
			o->Gravity = static_cast<mu_float>(GetLargeRand(3) + 3);

			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			Vector(0.0f, static_cast<mu_float>(GetLargeRand(128) + 64)*0.1f, 0.0f, p1);
			VectorRotate(p1, Matrix, o->HeadAngle);
			o->HeadAngle[2] += 25.0f;
			break;
		}
		else if (o->SubType == 1)	
		{
			o->LifeTime = 40;
			o->Scale = Scale + static_cast<mu_float>(GetLargeRand(8) + 15) * 0.1f;
			Vector(0.0f, 0.0f, -60.0f, o->Direction);
			Vector(0.0f, 30.0f, 0.0f, o->HeadAngle);
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = 100;
			o->Scale = Scale;
			o->Gravity = 20.0f + static_cast<mu_float>(GetLargeRand(20)) * 0.5f;
		}
		break;
	case MODEL_PKFIELD_ASSASSIN_EFFECT_GREEN_HEAD:
	case MODEL_PKFIELD_ASSASSIN_EFFECT_RED_HEAD:
		{
			o->LifeTime = 50 + (GetLargeRand(30));
			o->Scale = 1.0f;
			o->Velocity = 1.0f;
			o->PKKey = -1;
			o->Owner = Owner;
			o->Gravity = 3.5f;

			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			Vector(0.0f, static_cast<mu_float>(GetLargeRand(64) + 25)*0.2f, 0.0f, p1);
			VectorRotate(p1, Matrix, o->HeadAngle);
			o->HeadAngle[2] = 25;

			o->SubType = GetLargeRand(2);

			o->Direction[0] = 0;
			o->Direction[1] = 0;
			o->Direction[2] = 0;
		}
		break;
	case MODEL_PKFIELD_ASSASSIN_EFFECT_GREEN_BODY:
	case MODEL_PKFIELD_ASSASSIN_EFFECT_RED_BODY:
		{
			o->Scale = 1.0f;
			o->LifeTime = 40 + (GetLargeRand(30));
			o->Velocity = 0.4f;						
			o->CurrentAction = MONSTER01_DIE;
		}
		break;
		

	case MODEL_DUNGEON_STONE01:
		o->LifeTime = GetLargeRand(16) + 24;
		o->Scale = static_cast<mu_float>(GetLargeRand(8) + 6)*0.1f;
		o->Gravity = -static_cast<mu_float>(GetLargeRand(4));
		Vector(static_cast<mu_float>(GetLargeRand(64) - 32), -static_cast<mu_float>(GetLargeRand(32) + 50), static_cast<mu_float>(GetLargeRand(128) + 200), p1);
		AngleMatrix(o->Angle, Matrix);
		VectorRotate(p1, Matrix, p2);
		VectorAdd(o->Position, p2, o->Position);
		break;

	case MODEL_WARCRAFT:
		o->LifeTime = 50;
		o->Scale = 0.7f;
		if (SubType == 0)
			o->BlendMesh = -3;
		else
			o->BlendMesh = -4;
		Vector(0.0f, 0.0f, 45.0f, o->Angle);
		break;

	case MODEL_POISON:
		o->BlendMesh = 1;
		o->LifeTime = 40;
		o->Scale = 1.0f;
		break;
	case BITMAP_FIRECRACKERRISE:	
		o->Angle = EVector3();
		o->Position[2] = 100.0f;
		o->LifeTime = 15 * 5;
		break;
	case BITMAP_FIRECRACKER:	
		Vector(1.0f, 1.0f, 1.0f, o->Light);
		Vector(static_cast<mu_float>(GetLargeRand(9) - 4), static_cast<mu_float>(GetLargeRand(9) - 4), 26.0f, o->Direction);

		if (o->SubType == 1)
			o->LifeTime = 4 + (GetLargeRand(3));
		else
			o->LifeTime = 12;

		PlayBackground(SOUND_FIRECRACKER1, o);
		break;
	case BITMAP_FIRECRACKER0001:	
		{
			o->LifeTime = 31;
			Vector(0, 0, 0.0f, o->Direction);
		}
		break;
	case BITMAP_FIRECRACKER0002:	
		{
			o->LifeTime = 30;

			CreateParticle(ThreadIndex, BITMAP_EXPLOTION_MONO, o->Position, o->Angle, o->Light, 0, 0.6f);

			EVector3 vLight;
			Vector(1.0f, 1.0f, 1.0f, vLight);
			for (mu_int32 i = 0; i<60; ++i)
			{
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, o->Position, o->Angle, vLight, 27);
			}
			for (mu_int32 i = 0; i<30; ++i)
			{
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, o->Position, o->Angle, vLight, 28);
			}

			Vector(o->Position[0] + (GetLargeRand(100) - 50), o->Position[1] + (GetLargeRand(100) - 50),
				o->Position[2], Position);
			CreateSprite(BITMAP_DS_SHOCK, Position, GetLargeRand(10) * 0.1f + 1.5f, o->Light, o);

			for (mu_int32 i = 0; i<60; ++i)
			{
				Vector(0.3f + (GetLargeRand(700))*0.001f, 0.3f + (GetLargeRand(700))*0.001f, 0.3f + (GetLargeRand(700))*0.001f, vLight);
				CreateParticle(ThreadIndex, BITMAP_SHINY, o->Position, o->Angle, vLight, 6);
			}

			if (o->SubType == 1)	
			{
				CreateEffect(ThreadIndex, MODEL_HALLOWEEN_CANDY_STAR, o->Position, o->Angle, vLight, 1);
				CreateEffect(ThreadIndex, GetLargeRand(4) + MODEL_XMAS_EVENT_BOX, o->Position, o->Angle, vLight, 0, o);
				CreateEffect(ThreadIndex, GetLargeRand(4) + MODEL_XMAS_EVENT_BOX, o->Position, o->Angle, vLight, 0, o);
			}

			PlayObject(SOUND_XMAS_FIRECRACKER, o);
		}
		break;
	case BITMAP_FIRECRACKER0003:	
		{
			o->LifeTime = 15;
			o->Angle[2] = GetLargeRand(360);
		}
		break;
	case BITMAP_SWORD_FORCE:
		o->LifeTime = 30;
		if (o->SubType == 0 || o->SubType == 1)
			Vector(0.8f, 0.8f, 0.8f, o->Light);
		Vector(0.0f, 0.0f, Angle[2] + 45.0f, o->HeadAngle);
		VectorCopy(o->Position, o->StartPosition);
		break;
	case MODEL_CLOUD:
		o->LifeTime = 2;
		o->BlendMesh = 0;
		o->Scale = 10.0f;
		o->Position[1] += 200.0f;
		o->Position[2] -= 190.0f;
		o->Flags.Clear(OBJECT::eOF_LIGHTENABLE);
		break;
	case BITMAP_BLIZZARD:
		{
			o->LifeTime = GetLargeRand(15) + 15;
			o->Gravity = -20.0f;
			o->Velocity = static_cast<mu_float>(GetLargeRand(360));
			Vector(0.0f, 0.0f, 0.0f, o->Light);

			mu_int32 rangeX, rangeY, rangeZ;
			if (o->SubType == 1)
			{
				rangeX = 300; rangeY = 150; rangeZ = 700;
				o->Scale = 0.5f;
				o->Gravity -= GetLargeRand(20) + 10;
			}
			else
			{
				rangeX = 200; rangeY = 100; rangeZ = 500;
				o->Scale = 0.0f;
			}

			o->Position[0] = o->Position[0] + GetLargeRand(rangeX) - rangeY;
			o->Position[1] = o->Position[1] + GetLargeRand(rangeX) - rangeY;
			o->Position[2] = o->Position[2] + 500.0f;
			o->Position[0] += 100.0f;

			VectorCopy(o->Position, o->StartPosition);
			PlayBackground(SOUND_METEORITE01);
		}
		break;

	case BITMAP_SHOTGUN:
		{
			o->LifeTime = 10;
			o->Velocity = 1.0f;
			Vector(0.0f, -30.0f, 0.0f, o->Direction);
			AngleMatrix(o->Angle, Matrix);
			Vector(0.0f, -20.0f, 50.0f, p1);
			VectorRotate(p1, Matrix, p2);
			VectorAdd(o->Position, p2, o->Position);

			
			EVector3  Angle, Pos, p3;
			Vector(-20.0f, -20.0f, 60.0f, p1);
			VectorRotate(p1, Matrix, p2);
			VectorAdd(Position, p2, Pos);
			VectorCopy(o->Angle, Angle);
			for (mu_int32 i = 0; i<20; ++i)
			{
				Angle[0] = o->Angle[0] + GetLargeRand(20) + 5;
				Angle[1] += i * 18;
				CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, Pos, Pos, Angle, 1);
			}

			Vector(30.0f, -20.0f, 60.0f, p3);
			VectorRotate(p3, Matrix, p2);
			VectorAdd(Position, p2, Pos);
			VectorCopy(o->Angle, Angle);
			for (mu_int32 i = 0; i<20; ++i)
			{
				Angle[0] = o->Angle[0] + GetLargeRand(20) + 5;
				Angle[1] += i * 18;
				CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, Pos, Pos, Angle, 1);
			}
		}
		break;
	case MODEL_GATE:    
	case MODEL_GATE + 1:
		Vector(0.0f, static_cast<mu_float>(GetLargeRand(128) + 64)*0.1f, 0.0f, p1);
		o->Position[2] += 50.0f;
		o->LifeTime = GetLargeRand(16) + 32;
		o->Scale = static_cast<mu_float>(GetLargeRand(4) + 8)*0.1f;
		o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
		AngleMatrix(o->Angle, Matrix);
		VectorRotate(p1, Matrix, o->Direction);
		o->Gravity = static_cast<mu_float>(GetLargeRand(5) + 2);
		if (o->Type == MODEL_GATE && o->SubType == 0)
		{
			o->SubType = 1;
			o->Gravity += static_cast<mu_float>(GetLargeRand(5));
		}
		else if (o->SubType == 2)
		{
			o->Scale *= 0.6f;
		}

		o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
		o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
		o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
		break;

	case MODEL_STONE_COFFIN:    
	case MODEL_STONE_COFFIN + 1:
		Vector(0.0f, static_cast<mu_float>(GetLargeRand(128) + 32)*0.1f, 0.0f, p1);
		o->Position[2] += 50.0f;
		o->LifeTime = GetLargeRand(16) + 32;
		o->Scale = static_cast<mu_float>(GetLargeRand(4) + 8)*0.1f;
		o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
		AngleMatrix(o->Angle, Matrix);
		VectorRotate(p1, Matrix, o->Direction);
		o->Gravity = static_cast<mu_float>(GetLargeRand(5) + 2);
		if (o->Type == MODEL_STONE_COFFIN + 1 && o->SubType == 0)
		{
			o->SubType = 1;
			o->Gravity += static_cast<mu_float>(GetLargeRand(5));
		}

		o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
		o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
		o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
		break;
	case MODEL_SHINE:
		if (o->SubType == 0)
		{
			EVector3 Pos;
			o->LifeTime = 50;
			for (mu_int32 j = 0; j<10; ++j)
			{
				Pos[0] = o->Position[0] + (j - 5)*12.0f - 30.0f;
				Pos[1] = o->Position[1] + (j - 5)*12.0f + 30.0f;
				Pos[2] = o->Position[2] - 300.0f;

				CreateJoint(ThreadIndex, BITMAP_FLARE, Pos, o->Position, o->Angle, 16, o->Owner, 120.0f);
			}
		}
		break;
	case MODEL_BLIZZARD:
		if (o->SubType == 0 || o->SubType == 2)
		{
			o->BlendMesh = -2;
			o->LifeTime = GetLargeRand(15) + 15;
			o->Gravity = -20.0f;
			o->Velocity = static_cast<mu_float>(GetLargeRand(360));
			Vector(0.0f, 0.0f, 0.0f, o->Light);

			mu_int32 rangeX, rangeY, rangeZ;

			rangeX = 300; rangeY = 150; rangeZ = 700;
			o->Scale = 0.5f;
			o->Gravity -= GetLargeRand(30) + 10;

			o->Position[0] = o->Position[0] + GetLargeRand(rangeX) - rangeY;
			o->Position[1] = o->Position[1] + GetLargeRand(rangeX) - rangeY;
			o->Position[2] = o->Position[2] + 600.0f;
			o->Position[0] += 100.0f;

			VectorCopy(o->Position, o->StartPosition);

			if (o->SubType == 2)
			{
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, o->Position, o->Position, o->Angle, 4, o, 60.0f + GetLargeRand(10));
				if (GetLargeRand(2) == 0)
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, o->Position, o->Position, o->Angle, 4, o, 60.0f + GetLargeRand(10));
			}
		}
		else if (o->SubType == 1)
		{
			o->BlendMesh = -2;
			o->LifeTime = 20;
			o->Velocity = 0.0f;
		}
		break;

	case MODEL_ARROW_DRILL:
		if (o->SubType == 0 || o->SubType == 2)
		{
			o->LifeTime = 30;
			o->BlendMesh = -2;
			o->Scale = 1.0f;
			o->Gravity = -10.0f;
			o->Position[2] += 130.0f;

			if (o->SubType != 0)
			{
				CreateEffect(ThreadIndex, MODEL_PIERCING, o->Position, o->Angle, o->Light, 0, o);
				o->AttackPoint[0] = 0;
				o->Kind = 1;
			}

			Vector(0.0f, -70.0f, 0.0f, o->Direction);
			VectorCopy(o->Position, o->EyeLeft);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 5, o, 100.0f);

			o->Weapon = 0;
		}
		break;

	case MODEL_COMBO:
		o->LifeTime = 20;
		o->Gravity = 0.1f;
		o->BlendMesh = -2;
		o->BlendMeshLight = 1.0f;
		Vector(0.0f, 0.0f, 0.0f, o->Angle);
		VectorCopy(o->Position, o->StartPosition);
		o->Position[2] += 50.0f;

		if (o->SubType == 0)
		{
			for (mu_int32 j = 0; j<60; ++j)
			{
				CreateJoint(ThreadIndex, BITMAP_LIGHT, o->Position, o->Position, o->Angle, 0, nullptr, static_cast<mu_float>(GetLargeRand(40) + 70));
			}
		}
		break;
	case MODEL_AIR_FORCE:	    
		if (o->SubType == 0)
		{
			o->LifeTime = 15;
			o->BlendMeshLight = 1.0f;
			o->Scale = 0.6f;
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 20;
			o->BlendMeshLight = 1.0f;
			o->Scale = 1.2f;
		}
		break;
	case MODEL_WAVES:
		o->LifeTime = 20;
		o->Gravity = 0.1f;
		o->BlendMesh = -2;
		o->BlendMeshLight = 1.0f;
		Vector(0.0f, 0.0f, 0.0f, o->Angle);
		VectorCopy(o->Position, o->StartPosition);
		o->Position[2] += 50.0f;

		if (o->SubType == 0)
		{
			for (mu_int32 j = 0; j<60; ++j)
			{
				CreateJoint(ThreadIndex, BITMAP_LIGHT, o->Position, o->Position, o->Angle, 0, nullptr, static_cast<mu_float>(GetLargeRand(40) + 70));
			}
		}
		else if (o->SubType == 1)   
		{
			o->LifeTime = 15;
			o->RenderType = RENDER_NODEPTH;
			o->Position[2] += 80.0f;
			o->Scale = 0.1f + GetLargeRand(50) / 100.0f;
			o->Gravity = 0.01f;

			o->Angle[0] = 90.0f;
			o->Angle[2] = Angle[2];

			for (mu_int32 j = 0; j<2; ++j)
			{
				CreateJoint(ThreadIndex, BITMAP_PIERCING, o->Position, o->Position, o->Angle, 0, nullptr, static_cast<mu_float>(GetLargeRand(40) + 70));
			}

			o->Position[0] += GetLargeRand(100) - 50.0f;
			o->Position[1] += GetLargeRand(100) - 50.0f;
		}
		else if (o->SubType == 2 || o->SubType == 3 || o->SubType == 4)
		{
			o->LifeTime = 15;
			o->PKKey = -1;
			o->Scale = PKKey*0.05f;
			o->Gravity = 0.01f;
			o->Position[2] -= 50.0f;

			o->Angle[0] = 90.0f;
			o->Angle[2] = Angle[2];
		}
		else if (o->SubType == 5 || o->SubType == 6)
		{
			o->LifeTime = 10;
			o->PKKey = -1;
			o->Scale = PKKey*0.05f;
			o->Gravity = 0.08f;
			o->Position[2] -= 50.0f;

			o->Angle[0] = -90.0f;
			o->Angle[2] = Angle[2];
		}
		break;
	case MODEL_PIERCING2:     
		o->Scale = 2.0f;
		if (o->SubType == 1)
			o->LifeTime = 6;
		else if (o->SubType == 2)
		{
			o->Scale = 3.0f;
			o->LifeTime = 10;
		}
		else
			o->LifeTime = 10;
		o->BlendMesh = -2;
		Vector(0.0f, -60.0f, 0.0f, o->Direction);
		VectorCopy(o->Position, o->StartPosition);
		o->Position[2] += 130.0f;
		break;
	case MODEL_DEASULER:
		if (o->SubType == 0)
		{
			const mu_int32	TOTAL_LIFETIME = 55;
			EVector3		v3PosProcess01, v3PosProcessFinal,
				v3DirModify, v3PosModify,
				v3PosTargetModify;

			o->ExtState = TOTAL_LIFETIME;			
			o->LifeTime = TOTAL_LIFETIME;
			o->Gravity = 0.0f;
			o->HiddenMesh = 1;
			o->Distance = 0.0f;
			o->Alpha = 1.0f;

			Vector(o->Angle[0], o->Angle[1], o->Angle[2], o->HeadAngle);

			VectorCopy(o->Position, o->StartPosition);	

			VectorSubtract(o->Light, o->Position, o->Direction);

			o->Distance = EMath::Sqrt(o->Direction[0] * o->Direction[0] +
				o->Direction[1] * o->Direction[1] +
				o->Direction[2] * o->Direction[2]);

			VectorDivFSelf(o->Direction, o->Distance);

			mu_float	fDistanceResult = 0;
			EVector3	v3DirResult;

			EVector3 v3PosModify02;
			mu_float	fTotalDist = 1700.0f, fFirstDist = 0.0f, fRateFirstDist = 0.0f;
			VectorCopy(o->Owner->Position, v3PosModify02);
			VectorCopy(o->Position, v3PosModify);

			v3PosModify02[2] = o->Light[2];
			v3PosModify[2] = o->Light[2];
			VectorCopy(o->Light, v3PosTargetModify);
			v3PosModify[2] = v3PosModify[2] + 100.0f;
			v3PosModify02[2] = v3PosModify02[2] + 100.0f;
			v3PosTargetModify[2] = v3PosTargetModify[2] + 100.0f;
			VectorDistNormalize(v3PosModify02, v3PosTargetModify, v3DirModify);

			fDistanceResult = o->Distance * 0.3f;
			fFirstDist = fDistanceResult;
			VectorMulF(v3DirModify, fDistanceResult, v3DirResult);
			VectorAdd(v3PosModify, v3DirResult, v3PosProcess01);

			fDistanceResult = fTotalDist;
			VectorMulF(v3DirModify, fDistanceResult, v3DirResult);
			VectorAdd(v3PosModify, v3DirResult, v3PosProcessFinal);

			fRateFirstDist = fFirstDist / fTotalDist;

			MUObjectExtra *ox = &g_EffectsExtra[i];
			ox->m_Interpolates.ClearContainer();

			fRateFirstDist = fFirstDist / fTotalDist;

			CInterpolateContainer::INTERPOLATE_FACTOR	InsertFactor;
			InsertFactor.fRateStart = 0.0f;			
			InsertFactor.fRateEnd = fRateFirstDist;			
			Vector(0.0f, 0.0f, 0.0f, InsertFactor.v3Start);
			Vector(0.0f, 90.0f, 0.0f, InsertFactor.v3End);
			ox->m_Interpolates.m_vecInterpolatesAngle.push_back(InsertFactor);		

			InsertFactor.fRateStart = fRateFirstDist;			
			InsertFactor.fRateEnd = 1.01f;
			Vector(0.0f, 90.0f, 0.0f, InsertFactor.v3Start);
			Vector(0.0f, 90.0f, 2560.0f, InsertFactor.v3End);

			ox->m_Interpolates.m_vecInterpolatesAngle.push_back(InsertFactor);		
			
			InsertFactor.fRateStart = 0.0f;
			InsertFactor.fRateEnd = fRateFirstDist;
			VectorCopy(o->Position, InsertFactor.v3Start);
			VectorCopy(v3PosProcess01, InsertFactor.v3End);
			ox->m_Interpolates.m_vecInterpolatesPos.push_back(InsertFactor);		

			InsertFactor.fRateStart = fRateFirstDist;
			InsertFactor.fRateEnd = 1.01f;
			VectorCopy(v3PosProcess01, InsertFactor.v3Start);
			VectorCopy(v3PosProcessFinal, InsertFactor.v3End);
			ox->m_Interpolates.m_vecInterpolatesPos.push_back(InsertFactor);		
																				
			ox->m_Interpolates.GetAngleCurrent(o->Angle, 0.0f);		
			ox->m_Interpolates.GetPosCurrent(o->Position, 0.0f);
		}
		break;
	case MODEL_DEATH_SPI_SKILL:
		if (o->SubType == 0)
		{
			o->LifeTime = 30;
			o->Gravity = 1.0f;
			o->Velocity = 10.0f;
			o->HiddenMesh = 1;
			o->Scale = 0.3f;
			o->BlendMesh = -1;
			o->Alpha = 0.8f;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			Vector(0.0f, -26.0f, 0.0f, o->Direction);
			VectorCopy(Light, o->StartPosition);
			VectorCopy(o->Angle, o->HeadAngle);
			Vector(0.0f, 0.0f, Angle[2], o->Angle);
		}
		else if (o->SubType == 1)
		{
			
			o->LifeTime = o->Owner->LifeTime;
			o->HiddenMesh = 1;
			o->Scale = 0.2f;
			o->BlendMesh = -1;
			o->Alpha = static_cast<mu_float>((20 - o->LifeTime) / 5.0f);
			Vector(0.3f, 0.4f, 1.0f, o->Light);
			Vector(0.0f, 0.0f, 0.0f, o->Direction);

			CreateParticle(ThreadIndex, BITMAP_FIRE + 2, o->Position, o->Angle, o->Light, 0);
			
		}
		break;
	case MODEL_PIER_PART:   
		if (o->SubType == 0)
		{
			o->LifeTime = 20;
			o->Gravity = 2.0f;
			o->Velocity = 10.0f;
			o->HiddenMesh = 1;
			o->Scale = 1.2f;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			Vector(0.0f, -26.0f, 0.0f, o->Direction);
			VectorCopy(Light, o->StartPosition);
			VectorCopy(o->Angle, o->HeadAngle);
			Vector(0.0f, 0.0f, Angle[2], o->Angle);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = o->Owner->LifeTime;
			o->HiddenMesh = 0;
			o->Scale = 0.5f;
			o->Alpha = static_cast<mu_float>((20 - o->LifeTime) / 5.0f);
			Vector(0.0f, 0.0f, 0.0f, o->Direction);

			CreateParticle(ThreadIndex, BITMAP_FIRE + 1, o->Position, o->Angle, o->Light, 0, 1.0f, o);
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = 20;
			o->Velocity = 50.0f;
			o->HiddenMesh = -2;
			o->Position[2] -= 20.0f;

			Vector(0.0f, -40.0f, 0.0f, o->Direction);
			VectorCopy(o->Angle, o->HeadAngle);
			Vector(0.0f, 0.0f, Angle[2], o->Angle);
		}
		break;

	case BITMAP_FLARE_FORCE:
		if (o->SubType == 0)    
		{
			o->LifeTime = 0;
			if (o->Owner != nullptr)
			{
				CreateJoint(ThreadIndex, BITMAP_FLARE_FORCE, Position, Position, Angle, 1, o->Owner, 100.0f);
				CreateJoint(ThreadIndex, BITMAP_FLARE_FORCE, Position, Position, Angle, 0, o->Owner, 250.0f);
				CreateJoint(ThreadIndex, BITMAP_FLARE_FORCE, Position, Position, Angle, 2, o->Owner, 100.0f);
				CreateJoint(ThreadIndex, BITMAP_FLARE_FORCE, Position, Position, Angle, 3, o->Owner, 100.0f);
				CreateJoint(ThreadIndex, BITMAP_FLARE_FORCE, Position, Position, Angle, 4, o->Owner, 100.0f);
			}
		}
		else if (o->SubType == 1)   
		{
			CreateJoint(ThreadIndex, BITMAP_FLARE_FORCE, o->Position, o->Position, o->Angle, 5, o->Owner, 20.0f, PKKey, SkillIndex);
			CreateJoint(ThreadIndex, BITMAP_FLARE_FORCE, o->Position, o->Position, o->Angle, 6, o->Owner, 20.0f, PKKey, SkillIndex);
			CreateJoint(ThreadIndex, BITMAP_FLARE_FORCE, o->Position, o->Position, o->Angle, 7, o->Owner, 20.0f, PKKey, SkillIndex);
		}
		else if (o->SubType >= 2 && o->SubType <= 4)	
		{	
			CreateJoint(ThreadIndex, BITMAP_FLARE_FORCE, Position, Position, Angle, 8 + (o->SubType - 2), o->Owner, 100.0f); 
			CreateJoint(ThreadIndex, BITMAP_FLARE_FORCE, Position, Position, Angle, 0, o->Owner, 150.0f);
		}
		else if (o->SubType >= 5 && o->SubType <= 7)
		{
			CreateJoint(ThreadIndex, BITMAP_FLARE_FORCE, Position, Position, Angle, 11 + (o->SubType - 5), o->Owner, 100.0f); 
			CreateJoint(ThreadIndex, BITMAP_FLARE_FORCE, Position, Position, Angle, 1, o->Owner, 100.0f);
		}
		break;

	case MODEL_DARKLORD_SKILL:
		{
			o->LifeTime = 10;
			o->Scale = 0.2f;
			o->Velocity = 0.1f;

			if (o->SubType <= 1)
			{
				mu_float angle = 45.0f + (-90.0f*o->SubType);

				if (GetLargeRand(2) == 0)
				{
					if (o->SubType)
					{
						angle = 45.0f - 90.0f;
					}
					else
					{
						angle = 45.0f;
					}
				}
				Vector(45.0f, angle, 0.0f, o->Angle);
			}
			else if (o->SubType == 2)
			{
				o->LifeTime = 12;
				o->Velocity = 0.4f;
			}
		}
		break;

	case MODEL_GROUND_STONE:
		{
			mu_int32 TargetX = (mu_int32)(o->Position[0] * g_TerrainScaleRev);
			mu_int32 TargetY = (mu_int32)(o->Position[1] * g_TerrainScaleRev);

			mu_uint16 wall = TerrainWall[TERRAIN_INDEX(TargetX, TargetY)];

			if ((wall&TW_NOMOVE) != TW_NOMOVE && (wall&TW_NOGROUND) != TW_NOGROUND && (wall&TW_WATER) != TW_WATER)
			{
				o->LifeTime = 40;
				o->Scale = 1.2f + GetLargeRand(30) / 100.0f;
				o->Velocity = 0.3f;
				o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
				SetAction(o, 0);
			}
			else
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);
			}
		}
		break;
	case MODEL_GROUND_STONE2:
		{
			mu_int32 TargetX = (mu_int32)(o->Position[0] * g_TerrainScaleRev);
			mu_int32 TargetY = (mu_int32)(o->Position[1] * g_TerrainScaleRev);

			mu_uint16 wall = TerrainWall[TERRAIN_INDEX(TargetX, TargetY)];

			if ((wall&TW_NOMOVE) != TW_NOMOVE && (wall&TW_NOGROUND) != TW_NOGROUND && (wall&TW_WATER) != TW_WATER)
			{
				o->LifeTime = 40;
				o->Scale = 1.0f + GetLargeRand(30) / 100.0f;
				o->Velocity = 0.3f;
				o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
				SetAction(o, 0);

			}
			else
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);
			}
		}
		break;
	case BITMAP_TWLIGHT:
		{
			if (o->SubType == 0 || o->SubType == 1 || o->SubType == 2)
			{
				o->LifeTime = 80;
				o->Scale = 8.0f;
			}
			else if (o->SubType == 3)	
			{
				o->LifeTime = 30;
				o->Alpha = 0.0f;
				o->PKKey = 0;		
									
									
				VectorCopy(o->Light, o->EyeRight);
			}
		}
		break;
	case BITMAP_SHOCK_WAVE:
		if (o->SubType == 0)
		{
			o->LifeTime = 30;
			o->Scale = 20.0f;
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 20;
			o->Scale = (GetLargeRand(10) + 10.0f) / 10.0f;
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = 20;
			o->Scale = (GetLargeRand(10) + 10.0f) / 10.0f;
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = 15;
			o->Scale = 1.0f;
		}
		else if (o->SubType == 4)
		{
			o->LifeTime = 10;
			o->Scale = (GetLargeRand(6) + 6.0f) / 10.0f;
		}
		else if (o->SubType == 5)
		{
			o->LifeTime = 20;
			o->Scale = 9.0f;
		}
		else if (o->SubType == 6)
		{
			o->LifeTime = 50;
			o->Scale = 9.0f;
		}
		else if (o->SubType == 7)
		{
			o->LifeTime = 10;
			o->Scale = 1.0f;
		}
		else if (o->SubType == 8)
		{
			o->LifeTime = 10;
			o->Scale = (GetLargeRand(10) + 10.0f) / 5.0f;
		}
		else if (o->SubType == 9)
		{
			o->LifeTime = 5;
			o->Scale = 5.0f;
		}
		else if (o->SubType == 10)
		{
			o->LifeTime = 1;
			o->Scale = 2.0f;

			o->Light[0] *= 2.3f;
			o->Light[1] *= 2.3f;
			o->Light[2] *= 2.3f;
		}
		else if (o->SubType == 11)
		{
			o->LifeTime = 20;
			o->Scale = (GetLargeRand(10) + 10.0f) / 10.0f;
		}
		else if (o->SubType == 12)
		{
			o->LifeTime = 10;
			o->Scale = Scale;
		}
		else if (o->SubType == 13)
		{
			o->LifeTime = 40;
			o->Scale = (GetLargeRand(10) + 20.0f) / 7.0f;
		}
		else if (o->SubType == 14)
		{
			o->LifeTime = 30;
			o->Alpha = 0.0f;
			o->PKKey = 0;		
								
			VectorCopy(o->Light, o->EyeRight);
		}
		break;

	case BITMAP_DAMAGE_01_MONO:
		if (SubType == 0)
		{
			o->LifeTime = 20;
			o->Scale = Scale;
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 10;
			o->Scale = 0.1f;
			o->Alpha = 1.0f;
		}
		break;

	case BITMAP_FLARE:
		if (SubType == 1 || SubType == 2)
		{
			o->LifeTime = 30;
		}
		else if (SubType == 3)
		{
			o->LifeTime = 60;
		}
		else
		{
			o->LifeTime = 30;
		}
		break;

	case BITMAP_JOINT_THUNDER + 1:
		o->LifeTime = 10;

		if (o->Owner != nullptr && o->SubType == 0)
		{
			EBone Matrix;
			EVector3  p, p2;

			Vector(-25.0f, -80.0f, 0.0f, p);
			AngleMatrix(o->Owner->Angle, Matrix);
			VectorRotate(p, Matrix, p2);
			VectorAdd(p2, o->Position, o->Position);
		}
		break;

	case MODEL_CUNDUN_DRAGON_HEAD:
		{
			o->LifeTime = 30;
			o->BlendMesh = -2;
			o->Scale = 1.0f;
			Vector(0.5f, 0.5f, 0.5f, o->Light);
			o->Alpha = 0;
			EVector3 Position;
			VectorCopy(o->Position, Position);
			o->Position[2] += 100;

			mu_float fAngle = mu_float(GetLargeRand(360));
			mu_float fDistance = mu_float(GetLargeRand(600) + 200);
			Position[0] = o->Position[0] + EMath::Sin(fAngle)*fDistance;
			Position[1] = o->Position[1] + EMath::Cos(fAngle)*fDistance;
			Position[2] = 0;
			CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT2, Position, Position, o->Angle, 18, o, 100.0f, 0, 0);
			o->Angle[0] = -30;
			o->Angle[1] = 0;
			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
		}
		break;
	case MODEL_CUNDUN_PHOENIX:
		{
			o->Velocity = 0.34f;
			o->LifeTime = 20;
			o->BlendMesh = -2;
			o->Scale = 0.7f;
			Vector(0.5f, 0.5f, 0.5f, o->Light);
			o->Alpha = 0;
			EVector3 Position, Angle;
			VectorCopy(o->Position, Position);
			
			Vector(0, 0, o->Angle[2] * Q_PI / 180.0f, Angle);
			CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, Position, Position, Angle, 15, o, 100.0f, 0, 0);
			
			o->AnimationFrame = mu_float(GetLargeRand(5));
		}
		break;
	case MODEL_CUNDUN_GHOST:
		o->Velocity = 0.08f;
		o->LifeTime = 200;
		o->BlendMesh = -2;
		o->Scale = 1.80f;
		
		Vector(0.5f, 0.5f, 0.5f, o->Light);
		break;
	case MODEL_CUNDUN_SKILL:
		switch (o->SubType)
		{
		case 0:	
			o->LifeTime = 30;
			break;
		case 1:	
			o->LifeTime = 30;
			break;
		case 2:	
			o->LifeTime = 40;
			o->PKKey = 0;
			CreateEffect(ThreadIndex, MODEL_CUNDUN_GHOST, o->Position, o->Angle, o->Light, 0, o);
			break;
		};
		break;

	case BATTLE_CASTLE_WALL1:   
	case BATTLE_CASTLE_WALL2:
	case BATTLE_CASTLE_WALL3:
	case BATTLE_CASTLE_WALL4:
		o->LifeTime = 2;
		break;

	case MODEL_HALLOWEEN_EX:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 0;
				mu_int32 iEffectType;
				for (mu_int32 i = 0; i<24; ++i)
				{
					
					iEffectType = GetLargeRand(8);
					switch (iEffectType)
					{
					case 0: iEffectType = MODEL_HALLOWEEN_CANDY_BLUE;	break;
					case 1: iEffectType = MODEL_HALLOWEEN_CANDY_ORANGE;	break;
					case 2: iEffectType = MODEL_HALLOWEEN_CANDY_YELLOW;	break;
					case 3: iEffectType = MODEL_HALLOWEEN_CANDY_RED;	break;
					case 4: iEffectType = MODEL_HALLOWEEN_CANDY_HOBAK;	break;
					case 5: iEffectType = MODEL_HALLOWEEN_CANDY_STAR;	break;
					case 6: iEffectType = MODEL_HALLOWEEN_CANDY_HOBAK;	break;
					case 7: iEffectType = MODEL_HALLOWEEN_CANDY_STAR;	break;
					}

					CreateEffect(ThreadIndex, iEffectType, o->Position, o->Angle, o->Light, 0);
				}
			}
		}
		break;
	case MODEL_HALLOWEEN_CANDY_BLUE:
	case MODEL_HALLOWEEN_CANDY_ORANGE:
	case MODEL_HALLOWEEN_CANDY_YELLOW:
	case MODEL_HALLOWEEN_CANDY_RED:
	case MODEL_HALLOWEEN_CANDY_HOBAK:
	case MODEL_HALLOWEEN_CANDY_STAR:
		{
			if (o->SubType == 0)
			{
				if (o->Type == MODEL_HALLOWEEN_CANDY_HOBAK)
					o->Scale = 2.0f + (GetLargeRand(10) - 5)*0.02f;
				else if (o->Type == MODEL_HALLOWEEN_CANDY_STAR)
					o->Scale = 2.0f + (GetLargeRand(10) - 5)*0.02f;
				else
					o->Scale = 0.6f + (GetLargeRand(10) - 5)*0.02f;
				o->LifeTime = GetLargeRand(10) + 50;
				o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
				o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
				o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
				AngleMatrix(o->Angle, Matrix);
				o->Gravity = static_cast<mu_float>(GetLargeRand(10) + 10);
				EVector3 p;
				Vector(static_cast<mu_float>(GetLargeRand(60) - 30)*0.1f, static_cast<mu_float>(GetLargeRand(60) - 30)*0.1f, 0.0f, p);
				VectorScale(p, 2.0f, p);
				VectorRotate(p, Matrix, o->Direction);
				o->m_iAnimation = GetLargeRand(3);
			}
			else if (o->SubType == 1)
			{
				o->Scale = 2.0f + (GetLargeRand(10) - 5)*0.02f;
				o->LifeTime = GetLargeRand(10) + 40;
				o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
				o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
				o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
				AngleMatrix(o->Angle, Matrix);
				o->Gravity = static_cast<mu_float>(GetLargeRand(10) + 10);
				EVector3 p;
				Vector(static_cast<mu_float>(GetLargeRand(60) - 30)*0.1f, static_cast<mu_float>(GetLargeRand(60) - 30)*0.1f, -1.0f, p);
				VectorScale(p, 1.5f, p);
				VectorRotate(p, Matrix, o->Direction);
				o->m_iAnimation = GetLargeRand(3);
			}
		}
		break;

	case MODEL_XMAS_EVENT_BOX:
	case MODEL_XMAS_EVENT_CANDY:
	case MODEL_XMAS_EVENT_TREE:
	case MODEL_XMAS_EVENT_SOCKS:
		{
			o->Scale = 0.7f + (GetLargeRand(10) - 5)*0.02f;
			if (o->Type == MODEL_XMAS_EVENT_BOX)
			{
				o->Scale += 0.3f;
			}
			o->LifeTime = GetLargeRand(10) + 50;
			o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
			o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			o->Gravity = static_cast<mu_float>(GetLargeRand(5) + 15);
			EVector3 vPos;
			Vector(static_cast<mu_float>(GetLargeRand(60) - 30)*0.1f, static_cast<mu_float>(GetLargeRand(60) - 30)*0.1f, -1.0f, vPos);
			VectorScale(vPos, 1.5f, vPos);
			VectorRotate(vPos, Matrix, o->Direction);
			o->m_iAnimation = GetLargeRand(3);
		}
		break;
	case MODEL_XMAS_EVENT_ICEHEART:
		{
			o->Scale = 4.0f + (GetLargeRand(10) - 5)*0.02f;
			o->LifeTime = 100;
		}
		break;

	case MODEL_NEWYEARSDAY_EVENT_BEKSULKI:
	case MODEL_NEWYEARSDAY_EVENT_CANDY:
	case MODEL_NEWYEARSDAY_EVENT_MONEY:
	case MODEL_NEWYEARSDAY_EVENT_HOTPEPPER_GREEN:
	case MODEL_NEWYEARSDAY_EVENT_HOTPEPPER_RED:
	case MODEL_NEWYEARSDAY_EVENT_YUT:
	case MODEL_NEWYEARSDAY_EVENT_PIG:
		{
			if (o->Type == MODEL_NEWYEARSDAY_EVENT_HOTPEPPER_GREEN)
			{
				if (GetLargeRand(2) == 0)
					o->Type = MODEL_NEWYEARSDAY_EVENT_HOTPEPPER_RED;
			}
			else if (o->Type == MODEL_NEWYEARSDAY_EVENT_HOTPEPPER_RED)
			{
				if (GetLargeRand(2) == 0)
					o->Type = MODEL_NEWYEARSDAY_EVENT_HOTPEPPER_GREEN;
			}

			o->LifeTime = GetLargeRand(10) + 50;
			o->Scale = 1.6f + (GetLargeRand(10) - 5)*0.02f;

			if (o->Type == MODEL_NEWYEARSDAY_EVENT_BEKSULKI)
			{
				o->Scale = 2.5f + (GetLargeRand(10) - 5)*0.02f;
			}
			if (o->Type == MODEL_NEWYEARSDAY_EVENT_CANDY)
			{
				o->Scale = 3.0f + (GetLargeRand(10) - 5)*0.02f;
			}
			else if (o->Type == MODEL_NEWYEARSDAY_EVENT_PIG)
			{
				o->Scale = 1.0f + (GetLargeRand(10) - 5)*0.02f;
			}

			o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
			o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			o->Gravity = static_cast<mu_float>(GetLargeRand(10) + 10);
			EVector3 p;
			Vector(static_cast<mu_float>(GetLargeRand(10) - 5)*0.1f, static_cast<mu_float>(GetLargeRand(60) - 40)*0.1f, 0.0f, p);
			VectorScale(p, 1.3f, p);
			VectorRotate(p, Matrix, o->Direction);
			o->m_iAnimation = GetLargeRand(3);
		}
		break;

	case MODEL_MOONHARVEST_MOON:
		if (o->SubType == 0)
		{
			o->Alpha = 0.6f;
			o->LifeTime = 70;
			o->m_iAnimation = 0;
			VectorCopy(Position, o->Position);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 50;
			VectorCopy(o->Angle, o->Direction);
			Vector(0.0f, 0.0f, 0.0f, o->Angle);
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = 1;
			VectorCopy(Position, o->Position);
		}
		break;

	case MODEL_MOONHARVEST_GAM:
	case MODEL_MOONHARVEST_SONGPUEN1:
	case MODEL_MOONHARVEST_SONGPUEN2:
		{
			o->LifeTime = GetLargeRand(10) + 50;
			if (o->Type == MODEL_MOONHARVEST_GAM)
			{
				o->Scale = 0.5f + (GetLargeRand(10) - 5)*0.02f;
			}
			else
			{
				o->Scale = 0.8f + (GetLargeRand(10) - 5)*0.02f;
			}
			o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
			o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			o->Gravity = static_cast<mu_float>(GetLargeRand(10) + 10);
			EVector3 p;
			Vector(static_cast<mu_float>(GetLargeRand(10) - 5)*0.1f, static_cast<mu_float>(GetLargeRand(60) - 30)*0.1f, 0.0f, p);
			VectorScale(p, 1.2f, p);
			VectorRotate(p, Matrix, o->Direction);
		}
		break;

	case MODEL_MONSTER01 + 77:
		if (o->SubType == 0)   
		{
			o->LifeTime = 20;
			o->HiddenMesh = 3;
			o->Velocity = 0.33f;
			SetAction(o, 2);

			Vector(0.0f, 0.0f, 0.0f, o->Angle);
		}
		break;
	case MODEL_ARROW_TANKER_HIT:
		{
			o->LifeTime = 100;
			o->Scale = 1.0f;

			Vector(1.0f, 1.0f, 1.0f, o->Light);

			VectorCopy(Angle, o->m_vDeadPosition);
			VectorCopy(Position, o->StartPosition);

			if (o->SubType == 0)
			{
				Vector(40.0f, 0.0f, 90.0f, o->Angle);
				Vector(0.0f, -80.0f, 0.0f, o->Direction);
			}
			else if (o->SubType == 1)
			{
				Vector(40.0f, 0.0f, -90.0f, o->Angle);
				Vector(0.0f, -80.0f, 0.0f, o->Direction);
			}
			else if (o->SubType == 2)
			{
				Vector(40.0f, 0.0f, 0.0f, o->Angle);
				Vector(0.0f, -80.0f, 0.0f, o->Direction);
			}
		}
		break;
	case MODEL_FLY_BIG_STONE1:
		if (o->SubType <= 1)
		{
			o->LifeTime = 300;
			o->Velocity = GetLargeRand(200) / 100.0f + 28.0f;
			o->Gravity = 2.0f;
			o->Scale = 2.0f;
			o->PKKey = PKKey;
			o->Kind = Skill;
			o->Skill = SkillIndex;

			switch (o->PKKey)
			{
			case 1:
				o->Velocity = 29.0f;
				VectorCopy(Angle, o->m_vDeadPosition);
				break;

			default:
				o->m_vDeadPosition[0] = Hero->Object.Position[0] + (GetLargeRand(16) - 8)*g_TerrainScale;
				if (GetLargeRand(5) == 0)
					o->m_vDeadPosition[1] = 114 * g_TerrainScale;
				else
					o->m_vDeadPosition[1] = (GetLargeRand(33) + 98)*g_TerrainScale;
				o->m_vDeadPosition[2] = 100.0f;
				break;
			}
			Vector(0.0f, 0.0f, 0.0f, o->Angle);
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			VectorCopy(Position, o->StartPosition);

			VectorSubtract(o->m_vDeadPosition, o->StartPosition, o->Direction);
			VectorScale(o->Direction, 0.001f, o->Direction);
			o->Direction[2] = 5.0f;
		}
		else if (o->SubType == 2)   
		{
			o->LifeTime = 50;
			o->Scale = GetLargeRand(8) / 20.0f + 0.7f;
			o->Gravity = 10.0f;
			o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));

			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			Vector(0.0f, 0.0f, static_cast<mu_float>(GetLargeRand(360)), o->HeadAngle);
		}
		break;

	case MODEL_FLY_BIG_STONE2:
		if (o->SubType <= 1)
		{
			o->LifeTime = 300;
			o->Velocity = GetLargeRand(200) / 100.0f + 28.0f;
			o->Gravity = 2.0f;
			o->Scale = 2.0f;
			o->PKKey = PKKey;
			o->Kind = Skill;
			o->Skill = SkillIndex;

			switch (o->PKKey)
			{
			case 1:
				o->Velocity = 29.0f;
				VectorCopy(Angle, o->m_vDeadPosition);
				break;

			default:
				o->m_vDeadPosition[0] = Hero->Object.Position[0] + (GetLargeRand(16) - 8)*g_TerrainScale;
				o->m_vDeadPosition[1] = (GetLargeRand(30) + 80)*g_TerrainScale;
				o->m_vDeadPosition[2] = 100.0f;
				break;
			}
			Vector(0.0f, 0.0f, 0.0f, o->Angle);
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			VectorCopy(Position, o->StartPosition);

			VectorSubtract(o->m_vDeadPosition, o->StartPosition, o->Direction);
			VectorScale(o->Direction, 0.001f, o->Direction);
			o->Direction[2] = 5.0f;
		}
		break;

	case MODEL_BIG_STONE_PART1:
	case MODEL_BIG_STONE_PART2:
	case MODEL_WALL_PART1:
	case MODEL_WALL_PART2:
	case MODEL_GOLEM_STONE:
		if (o->Type >= MODEL_WALL_PART1 && o->Type <= MODEL_WALL_PART2)
		{
			Vector(0.0f, static_cast<mu_float>(GetLargeRand(256) + 128)*0.1f, 0.0f, p1);
			o->Scale = static_cast<mu_float>(GetLargeRand(2) + 7)*0.1f;
			o->Gravity = static_cast<mu_float>(GetLargeRand(5) + 6);
		}
		else if (o->Type == MODEL_GOLEM_STONE)	
		{
			Vector(0.0f, static_cast<mu_float>(GetLargeRand(128) + 32)*0.1f, 0.0f, p1);
			o->Scale = static_cast<mu_float>(GetLargeRand(2) + 10)*0.25f;
			o->Gravity = static_cast<mu_float>(GetLargeRand(5) + 2);
		}
		else if ((o->Type == MODEL_BIG_STONE_PART1 || o->Type == MODEL_BIG_STONE_PART2) && o->SubType == 2)
		{	
			Vector(0.0f, static_cast<mu_float>(GetLargeRand(128) + 32)*0.1f, 0.0f, p1);
			o->Scale = 0.6F + static_cast<mu_float>(GetLargeRand(2) + 4)*0.12f;
			o->Gravity = static_cast<mu_float>(GetLargeRand(5) + 2);
		}
		else if (o->Type == MODEL_BIG_STONE_PART2 && o->SubType == 3)
		{	
			o->LifeTime = GetLargeRand(16) + 32;
			o->Scale = 1.2f + static_cast<mu_float>(GetLargeRand(3) + 2)*0.12f;
			o->Direction[2] = -(GetLargeRand(5) + 20);
			o->Velocity = 1.8f;
			break;
		}
		else
		{
			Vector(0.0f, static_cast<mu_float>(GetLargeRand(128) + 128)*0.1f, 0.0f, p1);
			o->Scale = static_cast<mu_float>(GetLargeRand(7) + 10)*0.1f;
			o->Gravity = static_cast<mu_float>(GetLargeRand(5) + 2);
		}
		o->LifeTime = GetLargeRand(16) + 32;
		o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
		AngleMatrix(o->Angle, Matrix);
		VectorRotate(p1, Matrix, o->Direction);

		o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
		o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
		o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));

		o->Position[2] += 50.0f;
		break;

	case MODEL_GATE_PART1:
	case MODEL_GATE_PART2:
	case MODEL_GATE_PART3:
		Vector(0.0f, static_cast<mu_float>(GetLargeRand(128) + 64)*0.1f, 0.0f, p1);
		o->Position[2] += 50.0f;
		o->LifeTime = GetLargeRand(16) + 32;
		o->Scale = static_cast<mu_float>(GetLargeRand(2) + 7)*0.1f;
		o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
		AngleMatrix(o->Angle, Matrix);
		VectorRotate(p1, Matrix, o->Direction);
		o->Gravity = static_cast<mu_float>(GetLargeRand(5) + 2);
		if (o->Type == MODEL_GATE_PART1 && o->SubType == 0)
		{
			o->SubType = 1;
			o->Gravity += static_cast<mu_float>(GetLargeRand(5));
		}
		else if (o->SubType == 2)
		{
			o->Scale *= 0.6f;
		}

		o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
		o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
		o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
		break;

	case MODEL_AURORA:
		o->LifeTime = 100;
		o->Scale = PKKey / 100.0f;
		o->BlendMesh = 0;
		o->BlendMeshLight = 0.0f;
		break;

	case MODEL_TOWER_GATE_PLANE:
		o->LifeTime = 100;
		o->BlendMeshLight = 0.3f;
		VectorCopy(o->Position, o->StartPosition);
		break;

	case BITMAP_CRATER:
		if (o->SubType == 0)
		{
			o->LifeTime = 60;
			o->StartPosition[0] = 4.5;
			o->StartPosition[1] = 4.5;
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 30;
			o->StartPosition[0] = 2.5;
			o->StartPosition[1] = 2.5;
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = 40;
			o->StartPosition[0] = 3.0;
			o->StartPosition[1] = 3.0;
		}

		Vector(1.0f, 1.0f, 1.0f, o->Light);
		break;

	case BITMAP_CHROME_ENERGY2:
		if (o->SubType == 0)
		{
			o->LifeTime = 30;
			o->StartPosition[0] = 1.5;
			o->StartPosition[1] = 1.5;
		}
		break;

	case MODEL_STUN_STONE:
		if (o->SubType == 0)
		{
			Vector(GetLargeRand(256) / 64.0f - 2.0f, -static_cast<mu_float>(GetLargeRand(200) + 64)*0.1f, 0.0f, p1);

			o->LifeTime = 40;
			o->Scale = 1.1f + GetLargeRand(100) / 100.0f;
			o->Gravity = -static_cast<mu_float>(GetLargeRand(16) + 10);

			o->ExtState = 0;
			o->Position[2] += 600.0f;

			AngleMatrix(Angle, Matrix);
			VectorRotate(p1, Matrix, o->StartPosition);

			Vector(1.0f, 1.0f, 1.0f, o->Light);
			Vector(-20.0f, 0.0f, GetLargeRand(360), o->Angle);
			Vector(0.0f, 0.0f, Angle[2], o->HeadAngle);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 22;
			o->HiddenMesh = -2;

			Vector(0.0f, -25.0f, 0.0f, o->Direction);
			Vector(-1.0f, -1.0f, -1.0f, o->Light);
		}
		break;

	case MODEL_SKIN_SHELL:      
		Vector(0.0f, static_cast<mu_float>(GetLargeRand(128) + 32)*0.1f, 0.0f, p1);
		o->Position[2] += 50.0f;
		o->LifeTime = GetLargeRand(16) + 32;
		o->Scale = static_cast<mu_float>(GetLargeRand(3) + 3)*0.1f;
		o->Alpha = 0.5f;
		o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
		AngleMatrix(o->Angle, Matrix);
		VectorRotate(p1, Matrix, o->Direction);
		o->Gravity = static_cast<mu_float>(GetLargeRand(5) + 2);
		o->Gravity += static_cast<mu_float>(GetLargeRand(5));

		o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
		o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
		o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));

		if (o->SubType == 0)
		{
			Vector(0.5f, 0.5f, 0.5f, o->Light);
		}
		else if (o->SubType == 1)
		{
			Vector(0.1f, 0.6f, 1.0f, o->Light);
		}
		break;

	case MODEL_MANA_RUNE:
		if (o->SubType == 0)
		{
			o->LifeTime = 50;
			o->Scale = 0.0f;
			o->Gravity = 0.1f;
			o->Alpha = 0.3f;
			o->HiddenMesh = 0;

			o->Position[2] += 300.0f;

			Vector(0.0f, 0.0f, 45.0f, o->Angle);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 10;
			o->Scale = 1.1f;
			o->Gravity = 0.1f;
			o->HiddenMesh = -1;
			o->BlendMesh = -2;
			o->BlendMeshLight = 0.4f;
		}
		break;
		
	case MODEL_SKILL_JAVELIN:
		{
			o->LifeTime = 35;
			o->Gravity = 2.0f;
			o->Velocity = 10.0f;
			o->Scale = 1.2f;

			Vector(0.0f, -5.0f, 0.0f, o->Direction);
			VectorCopy(o->Angle, o->HeadAngle);
			VectorCopy(o->Owner->Position, o->StartPosition);
			o->Position[2] += 150.0f;

			mu_float Ang = GetLargeRand(80) + 10;

			o->HeadAngle[2] += o->SubType*Ang - Ang;
		}
		break;
		
		
	case MODEL_FENRIR_THUNDER:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 100;
				o->Scale = 0.3f + static_cast<mu_float>(GetLargeRand(100))*0.002f;
				o->m_iAnimation = 0;	
				o->Alpha = 0.7f;

				o->Angle[0] = GetLargeRand(360);
				o->Angle[1] = GetLargeRand(360);
				o->Angle[2] = GetLargeRand(360);

				VectorCopy(Light, o->Light);

				EVector3 vPos;
				Vector(0.0f, 0.0f, 0.0f, vPos);
				MUModel* p_b = &MODELS::Data[o->Owner->Type];
				mu_int32 irandom = GetLargeRand(30);

				if (irandom >= 1 && irandom <= 2)	
				{
					p_b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[p_b->GetBoneIndex(10)], vPos, o->Position, false);
				}
				else if (irandom == 3)	
				{
					o->Scale -= 0.2f;
					p_b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[p_b->GetBoneIndex(14)], vPos, o->Position, false);
				}
				else if (irandom >= 4 && irandom <= 5)	
				{
					p_b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[p_b->GetBoneIndex(2)], vPos, o->Position, false);
				}
				else if (irandom >= 6 && irandom <= 7)	
				{
					o->Scale -= 0.2f;
					if (GetLargeRand(2) == 0)
						p_b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[p_b->GetBoneIndex(50)], vPos, o->Position, false);
					else
						p_b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[p_b->GetBoneIndex(51)], vPos, o->Position, false);
				}
				else if (irandom == 8)	
				{
					o->Scale -= 0.2f;
					p_b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[p_b->GetBoneIndex(53)], vPos, o->Position, false);
				}
				else if (irandom >= 9 && irandom <= 10)
				{
					o->Flags.Clear(OBJECT::eOF_LIVE);
					o->Alpha = 0.0f;
				}
				else	
				{
					o->Scale += 0.1f;
					o->Position[0] += GetLargeRand(240) - 120;
					o->Position[1] += GetLargeRand(10) - 5;
					o->Position[2] += 110;
				}

				EVector3 vLight;
				vLight[0] = o->Light[0] - 0.3f;
				vLight[1] = o->Light[1] - 0.3f;
				vLight[2] = o->Light[2] - 0.3f;
				CreateSprite(BITMAP_LIGHT, o->Position, 2.0f, vLight, o);
			}
			else if (o->SubType == 1)
			{
				o->LifeTime = 100;
				o->Scale = 0.3f + static_cast<mu_float>(GetLargeRand(100))*0.002f;
				o->Alpha = 1.0f;
				o->m_iAnimation = 0;

				o->Angle[0] = GetLargeRand(360);
				o->Angle[1] = GetLargeRand(360);
				o->Angle[2] = GetLargeRand(360);

				VectorCopy(Light, o->Light);

				o->Position[0] += GetLargeRand(40) - 20;
				o->Position[1] += GetLargeRand(40) - 20;
				o->Position[2] += GetLargeRand(40) - 20;

				EVector3 vLight;
				vLight[0] = o->Light[0] - 0.3f;
				vLight[1] = o->Light[1] - 0.3f;
				vLight[2] = o->Light[2] - 0.3f;
				CreateSprite(BITMAP_LIGHT, o->Position, 2.0f, vLight, o);
			}
			else if (o->SubType == 2)
			{
				o->LifeTime = 4;
				o->Scale = 0.1f + static_cast<mu_float>(GetLargeRand(100))*0.002f;
				o->Alpha = 1.0f;
				o->m_iAnimation = 0;

				o->Angle[0] = GetLargeRand(360);
				o->Angle[1] = GetLargeRand(360);
				o->Angle[2] = GetLargeRand(360);

				VectorCopy(Light, o->Light);

				o->Position[0] += GetLargeRand(40) - 20;
				o->Position[1] += GetLargeRand(40) - 20;
				o->Position[2] += GetLargeRand(40) - 20;

				EVector3 vLight;
				vLight[0] = o->Light[0] - 0.3f;
				vLight[1] = o->Light[1] - 0.3f;
				vLight[2] = o->Light[2] - 0.3f;
				CreateSprite(BITMAP_LIGHT, o->Position, 2.0f, vLight, o);
			}
			else if (o->SubType == 3)
			{
				o->LifeTime = 4;
				o->Scale = 0.5f + static_cast<mu_float>(GetLargeRand(100))*0.002f;
				o->Alpha = 1.0f;
				o->m_iAnimation = 0;

				o->Angle[0] = GetLargeRand(360);
				o->Angle[1] = GetLargeRand(360);
				o->Angle[2] = GetLargeRand(360);

				VectorCopy(Light, o->Light);

				o->Position[0] += GetLargeRand(160) - 80;
				o->Position[1] += GetLargeRand(160) - 80;
				o->Position[2] += GetLargeRand(160) - 80;

				EVector3 vLight;
				vLight[0] = o->Light[0] - 0.3f;
				vLight[1] = o->Light[1] - 0.3f;
				vLight[2] = o->Light[2] - 0.3f;
				CreateSprite(BITMAP_LIGHT, o->Position, 2.0f, vLight, o);
			}
		}
		break;
	case MODEL_FALL_STONE_EFFECT:
		{
			if (o->SubType == 0 || o->SubType == 1)
			{
				o->LifeTime = GetLargeRand(5) + 100;

				if (o->SubType == 0)
					o->Scale = static_cast<mu_float>(GetLargeRand(20) + 5) * 0.005f + o->Scale * 0.05f;
				else
					o->Scale = static_cast<mu_float>(GetLargeRand(10) + 5) * 0.02f + o->Scale * 0.05f;

				o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
				o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
				o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));

				o->Position[0] += static_cast<mu_float>(GetLargeRand(100) - 50);
				o->Position[1] += static_cast<mu_float>(GetLargeRand(100) - 50);

				o->Gravity = static_cast<mu_float>(GetLargeRand(10) + 10)*0.5f;
			}
			else if (o->SubType == 2)
			{
				o->LifeTime = 100;
				o->Scale = o->Scale + static_cast<mu_float>(GetLargeRand(20) + 5) * 0.05f;

				o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
				o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
				o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));

				o->Gravity = static_cast<mu_float>(GetLargeRand(20) + 40)*0.5f + o->Scale * 2.0f;
			}
			else if (o->SubType == 3)
			{
				o->LifeTime = GetLargeRand(5) + 15;
				o->Scale = Scale;
				o->Gravity = static_cast<mu_float>(GetLargeRand(3) + 3);

				o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
				AngleMatrix(o->Angle, Matrix);
				Vector(0.0f, static_cast<mu_float>(GetLargeRand(60) + 30)*0.1f, 0.0f, p1);
				VectorRotate(p1, Matrix, o->HeadAngle);
				o->HeadAngle[2] += 25.0f;

				Vector(0, 0, 0, o->Direction);
			}
		}
		break;
	case MODEL_FENRIR_FOOT_THUNDER:
		o->LifeTime = 200;
		o->m_iAnimation = 0;
		VectorCopy(Position, o->Position);
		VectorCopy(Light, o->Light);
		o->Alpha = 1.0f;
		o->m_dwTime = g_CurrentTime;
		o->Position[2] = 0.0f;
		break;
	case MODEL_TWINTAIL_EFFECT:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 200;
				VectorCopy(Position, o->Position);
				VectorCopy(Light, o->Light);
				o->Alpha = 1.0f;
				o->Position[2] = 0.0f;
				o->m_dwTime = g_CurrentTime;
				o->m_iAnimation = 0;
			}
			else if (o->SubType == 1 || o->SubType == 2)
			{
				o->LifeTime = 50;
				o->Scale = 3.5f;
				VectorCopy(Position, o->Position);
				VectorCopy(Light, o->Light);
				o->Alpha = 1.0f;
				o->Position[2] = 0.0f;
				o->Angle[0] = 0.0f;
			}
		}
		break;
		
	case MODEL_FENRIR_SKILL_DAMAGE:
		{
			switch (o->SubType)
			{
			case 1:	
				Vector(0.6f, 0.2f, 0.2f, o->Light);
				break;
			case 2:	
				Vector(0.2f, 0.2f, 0.4f, o->Light);
				break;
			case 3:	
				Vector(0.6f, 0.8f, 0.6f, o->Light);
				break;
			}

			

			EVector3 vPos;
			mu_int32 irandom;

			MUModel* p_b = &MODELS::Data[o->Owner->Type];
			const mu_uint32 boneCount = p_b->_BoneNonDummyEnd;
			for (mu_int32 i = 0; i<10; ++i)
			{
				irandom = GetLargeRand(boneCount);
				Vector(0.0f, 0.0f, 0.0f, vPos);
				p_b->TransformPosition(ThreadIndex, o->Owner->BoneTransform[irandom], vPos, o->Position, true);
				CreateParticle(ThreadIndex, BITMAP_ENERGY, o->Position, o->Angle, o->Light, 2);
			}
		}
		break;
		
	case MODEL_ARROW_IMPACT:
		o->Velocity = 1.0f;
		o->LifeTime = 20;
		o->Scale = 1.8f;
		o->BlendMesh = -2;
		o->Direction[1] = -30.0f;

		Vector(0.3f, 0.8f, 1.0f, o->Light);

		AngleMatrix(o->Angle, Matrix);
		Vector(-10.0f, -80.0f, 200.0f, p1);
		VectorRotate(p1, Matrix, p2);
		VectorAdd(o->Position, p2, o->Position);

		o->Angle[0] = -30.0f;
		CreateJoint(ThreadIndex, BITMAP_FLASH, o->Position, o->Position, o->Angle, 4, o, 40.0f, 50);

		o->Weapon = 0;
		break;
	case BITMAP_JOINT_FORCE:
		if (o->SubType == 0)
		{
			o->LifeTime = 20;
			Vector(0.0f, -550.0f, 0.0f, o->Direction);
			Vector(-90.0f, 0.0f, Angle[2], o->Angle);
			Vector(0.0f, 0.0f, GetLargeRand(360), o->HeadAngle);
			CreateJoint(ThreadIndex, BITMAP_JOINT_FORCE, o->Position, o->Position, o->Angle, 3, nullptr, 80.0f);
			VectorCopy(Position, o->StartPosition);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 20;
			Vector(0.0f, -250.0f, 0.0f, o->Direction);
			Vector(-90.0f, 0.0f, Angle[2], o->Angle);
			Vector(0.0f, 0.0f, GetLargeRand(360), o->HeadAngle);
			VectorCopy(Position, o->StartPosition);
		}
		break;
	case MODEL_SWORD_FORCE:
		o->Velocity = 0.25f;
		if (o->SubType == 0)
		{
			o->LifeTime = 15;
			o->Scale = 0.0f;

			o->Position[2] += 100.0f;
			Vector(0.0f, -10.0f, 0.0f, o->Direction);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 5;
			o->Scale = 3.5f;
		}
		o->BlendMesh = 0;
		o->BlendMeshLight = 1.0f;
		break;
	case MODEL_PROTECTGUILD:
		{
			o->Alpha = 0;
			o->Angle[2] = +45.0f;

			MUModel *b = &MODELS::Data[o->Owner->Type];
			EVector3 tempPosition, p;
			Vector(0.0f, 0.0f, 0.0f, p);
			b->TransformPosition(ThreadIndex, o->Owner->BoneTransform[b->GetBoneIndex(20)], p, tempPosition, true);
			o->Position[0] = tempPosition[0];
			o->Position[1] = tempPosition[1];
			o->Position[2] = o->Owner->Position[2] + tempPosition[2] - o->Owner->Position[2] + 60;

			o->LifeTime = 130;
			o->Scale = 3.2f;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			VectorCopy(o->Position, o->StartPosition);
			for (mu_int32 i = 0; i < 10; ++i)
			{
				mu_float fAngle = GetLargeRand(360);
				Vector(o->Position[0] + (GetLargeRand(20) + 15) * EMath::Sin(fAngle),
					o->Position[1] + (GetLargeRand(20) + 15) * EMath::Cos(fAngle),
					o->Position[2], p);
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, p, o->Angle, o->Light, 4, 0.6f, o);
			}
		}
		break;
	case MODEL_MOVE_TARGETPOSITION_EFFECT:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 30;
				o->BlendMesh = 0;

				EVector3 vLight, vPos;
				Vector(1.0f, 0.7f, 0.3f, vLight);
				VectorCopy(o->Position, vPos);
				vPos[2] += 85.0f;

				DeleteEffect(BITMAP_MAGIC, nullptr, 11);
				DeleteEffect(BITMAP_TARGET_POSITION_EFFECT2, nullptr, 0);
				DeleteEffect(BITMAP_TARGET_POSITION_EFFECT1, nullptr, 0);
				DeleteJoint(MODEL_SPEARSKILL, nullptr, 16);

				CreateEffect(ThreadIndex, BITMAP_MAGIC, o->Position, o->Angle, vLight, 11);
				CreateEffect(ThreadIndex, BITMAP_TARGET_POSITION_EFFECT2, o->Position, o->Angle, vLight, 0);
				CreateJoint(ThreadIndex, MODEL_SPEARSKILL, vPos, vPos, Angle, 16, o, 5.0f);
				CreateJoint(ThreadIndex, MODEL_SPEARSKILL, vPos, vPos, Angle, 16, o, 5.0f);
				CreateJoint(ThreadIndex, MODEL_SPEARSKILL, vPos, vPos, Angle, 16, o, 5.0f);
				CreateJoint(ThreadIndex, MODEL_SPEARSKILL, vPos, vPos, Angle, 16, o, 5.0f);
			}
		}
		break;

	case BITMAP_TARGET_POSITION_EFFECT1:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 20;
				o->Scale = 1.2f;
			}
		}
		break;
	case BITMAP_TARGET_POSITION_EFFECT2:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 30;
				o->Scale = 1.8f;
			}
		}
		break;
	case MODEL_EFFECT_SAPITRES_ATTACK:		
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 20;

				
				for (mu_int32 i = 0; i<10; ++i)
				{
					CreateEffect(ThreadIndex, MODEL_EFFECT_SAPITRES_ATTACK_2, o->Position, o->Angle, o->Light, 14);
				}
			}
		}
		break;
	case MODEL_EFFECT_SAPITRES_ATTACK_1:	
		{
			if (o->SubType == 0)
			{
				o->BlendMesh = 0;
				o->BlendMeshLight = 1.0f;
				o->Position[2] += 100.0f;
				o->LifeTime = 17;
				o->Scale = 1.1f;
				VectorSubtract(o->Owner->Position, o->Position, o->Direction);
				VectorNormalize(o->Direction);
				
				o->Angle[2] = CreateAngle(o->Position[0], o->Position[1], o->Owner->Position[0], o->Owner->Position[1]);
			}
		}
		break;
	case MODEL_EFFECT_THUNDER_NAPIN_ATTACK_1:	
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 30;
			}
			else if (o->SubType == 1)
			{
				o->LifeTime = 30;
			}
		}
		break;

		

	case MODEL_EFFECT_SKURA_ITEM:
		{
			if ((o->SubType == 0) || (o->SubType == 1))
			{
				o->LifeTime = 52;
			}
		}
		break;

		
	case MODEL_EFFECT_TRACE:			
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 50;
				VectorCopy(o->Position, o->EyeLeft2);
				CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 17, o, o->Scale, -1, 0, 0, -1, &o->Light);
			}
		}break;
	case MODEL_STAR_SHINE:
		{
			switch (o->SubType)
			{
			case 0:
				{
					o->LifeTime = 30;
					o->Alpha = 0.2f;
					o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
				}break;
			}
		}break;
	case MODEL_FEATHER:				
		{
			switch (o->SubType)
			{
			case 0:
			case 1:
			case 2:
			case 3:
				{
					EVector3 vOriginPos;
					VectorCopy(o->Position, vOriginPos);

					o->Position[0] += static_cast<mu_float>(GetLargeRand(20) - 10)*4.0f;
					o->Position[1] += static_cast<mu_float>(GetLargeRand(20) - 10)*4.0f;
					o->Position[2] += static_cast<mu_float>(GetLargeRand(20) - 10)*4.0f;

					VectorSubtract(vOriginPos, o->Position, o->Direction);
					VectorNormalize(o->Direction);
					mu_int32 iAddDirection = (static_cast<mu_float>(GetLargeRand(10) - 5)*0.08f);
					o->Direction[0] += iAddDirection;
					o->Direction[1] += iAddDirection;
					o->Direction[2] += iAddDirection;

					o->Scale = o->Scale + (static_cast<mu_float>(GetLargeRand(20) - 10)*(o->Scale*0.03f)); 
					o->LifeTime = 30 + (GetLargeRand(20) - 10);						
					if (o->SubType == 2 || o->SubType == 3)
						o->LifeTime = 100;						
					o->Alpha = 0.6f + (static_cast<mu_float>(GetLargeRand(10))*0.02f);				

					o->Gravity = 0.1f;
					if (o->SubType == 2 || o->SubType == 3)
						o->Gravity = 0.0f;
					o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
					o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
					o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));

					o->EyeRight[0] = static_cast<mu_float>(GetLargeRand(10) - 5);		
					o->EyeRight[1] = static_cast<mu_float>(GetLargeRand(10) - 5);
					o->EyeRight[2] = static_cast<mu_float>(GetLargeRand(10) - 5);

				}break;
			}

		}break;
	case MODEL_FEATHER_FOREIGN:				
		{
			switch (o->SubType)
			{
			case 4:
				{
					EVector3 vOriginPos;
					VectorCopy(o->Position, vOriginPos);

					o->Position[0] += static_cast<mu_float>(GetLargeRand(20) - 10)*4.0f;
					o->Position[1] += static_cast<mu_float>(GetLargeRand(20) - 10)*4.0f;
					o->Position[2] += static_cast<mu_float>(GetLargeRand(20) - 10)*4.0f;

					VectorSubtract(vOriginPos, o->Position, o->Direction);
					VectorNormalize(o->Direction);
					mu_int32 iAddDirection = (static_cast<mu_float>(GetLargeRand(10) - 5)*0.08f);
					o->Direction[0] += iAddDirection;
					o->Direction[1] += iAddDirection;
					o->Direction[2] += iAddDirection;

					o->Scale = o->Scale + (static_cast<mu_float>(GetLargeRand(20) - 10)*(o->Scale*0.03f)); 
					o->LifeTime = 25;
					o->Alpha = 0.3f; 

					o->Gravity = 0.1f;
					o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
					o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
					o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));

					o->EyeRight[0] = static_cast<mu_float>(GetLargeRand(10) - 5);		
					o->EyeRight[1] = static_cast<mu_float>(GetLargeRand(10) - 5);
					o->EyeRight[2] = static_cast<mu_float>(GetLargeRand(10) - 5);

				}break;
			}

		}break;

	case MODEL_BLOW_OF_DESTRUCTION:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 40;

				EVector3 vPos, vPos2;
				EBone Matrix;
				Vector(-20.0f, -100.0f, 0.0f, vPos);
				AngleMatrix(o->Owner->Angle, Matrix);
				VectorRotate(vPos, Matrix, vPos2);
				VectorAdd(vPos2, o->Position, o->Position);

				VectorCopy(o->Light, o->StartPosition);

				Vector(1.2f, 1.2f, 1.2f, o->Light);

				CreateEffect(ThreadIndex, MODEL_BLOW_OF_DESTRUCTION, o->StartPosition, o->Angle, o->Light, 1, o->Owner);
			}
			else if (o->SubType == 1)
			{
				o->LifeTime = 40;
				Vector(1.2f, 1.2f, 1.2f, o->Light);
				o->Position[2] = 150.0f;
				o->Scale = 5.0f;
			}
		}
		break;

	case MODEL_NIGHTWATER_01:
		{
			o->LifeTime = 25;
			o->Alpha = 1.0f;
			o->Angle[2] = GetLargeRand(360);
		}
		break;

	case MODEL_KNIGHT_PLANCRACK_A:
		if (o->SubType == 0)
		{
			o->LifeTime = 25;
			o->Alpha = 1.0f;
			o->Angle[2] = GetLargeRand(360);
			o->Scale = Scale + GetLargeRand(10) * 0.05f;
			o->Position[2] += 10.0f;
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 20;
			o->Alpha = 1.0f;
			o->Angle[2] = GetLargeRand(360);
			o->Scale = Scale + GetLargeRand(10) * 0.05f;
			o->Position[2] += 10.0f;
		}
		break;

	case MODEL_KNIGHT_PLANCRACK_B:
		{
			o->LifeTime = 25;
			o->Alpha = 1.0f;
			o->Position[2] += 15.0f;
			o->Angle[2] += 90.0f;
		}
		break;

		

	case MODEL_EFFECT_FLAME_STRIKE:
		if (o->SubType == 0)
		{
			o->Alpha = 0;
			o->LifeTime = 35;
			o->Velocity = MODELS::Data[o->Owner->Type]._AnimationHeader[PLAYER_SKILL_FLAMESTRIKE].Speed;
			o->AI = 0;
			o->m_iAnimation = GetLargeRand(1000000000);
		}
		break;

		

	case MODEL_STREAMOFICEBREATH:
		{
			const	mu_float LENSCALAR = 50.0f;
			const	mu_float RANDOMOFFSET_ANGLE = 5.0f;
			const	mu_float RANDOMOFFSET_SCALE = 0.01f;
			const	mu_float RANDOMOFFSET_POSITION = 20.0f;
			const	mu_float RANDNUM = (static_cast<mu_float>((GetLargeRand(2000)) - 1000) * 0.001f);

			
			o->LifeTime = 17;
			o->Light[0] = 0.6f;
			o->Light[1] = 0.6f;
			o->Light[2] = 0.6f;

			
			EVector3		v3Len_, v3LenBasis_, v3PosBasis_;
			EBone			matRotation;

			Vector(0.0f, -1.0f, 0.0f, v3LenBasis_);
			AngleMatrix(o->Angle, matRotation);
			VectorRotate(v3LenBasis_, matRotation, v3Len_);

			v3PosBasis_[0] = o->Position[0] + (v3Len_[0] * LENSCALAR);
			v3PosBasis_[1] = o->Position[1] + (v3Len_[1] * LENSCALAR);
			v3PosBasis_[2] = o->Position[2] + (v3Len_[2] * LENSCALAR);

			VectorCopy(v3PosBasis_, o->StartPosition);
			
			for (mu_int32 i_ = 0; i_ < 4; ++i_)
			{
				EVector3		v3ResultAngle, v3ResultPos;
				mu_float		fResultScale;
				mu_float		fCurrentOffsetScale, fCurrentOffsetPos, fCurrentOffsetAngle;
				fCurrentOffsetScale = RANDOMOFFSET_SCALE * RANDNUM;
				fCurrentOffsetPos = RANDOMOFFSET_POSITION * RANDNUM;
				fCurrentOffsetAngle = RANDOMOFFSET_ANGLE * RANDNUM;

				
				v3ResultPos[0] = v3PosBasis_[0] + fCurrentOffsetPos;
				v3ResultPos[1] = v3PosBasis_[1] + fCurrentOffsetPos;
				v3ResultPos[2] = v3PosBasis_[2] + fCurrentOffsetPos;

				
				v3ResultAngle[0] = o->Angle[0] + fCurrentOffsetAngle;
				v3ResultAngle[1] = o->Angle[1] + fCurrentOffsetAngle;
				v3ResultAngle[2] = o->Angle[2] + fCurrentOffsetAngle;

				
				fResultScale = o->Scale + fCurrentOffsetScale;

				CreateParticle(ThreadIndex, BITMAP_RAKLION_CLOUDS, v3ResultPos, v3ResultAngle, o->Light, 0, fResultScale);
			}

		}break;

		

		

	case MODEL_1_STREAMBREATHFIRE:
		{
			o->LifeTime = 30;
		}
		break;

		

	case MODEL_LAVAGIANT_FOOTPRINT_R:
	case MODEL_LAVAGIANT_FOOTPRINT_V:
		{
			o->LifeTime = 200;
			VectorCopy(Position, o->Position);
			VectorCopy(Light, o->Light);
			o->Alpha = 1.0f;
			o->Position[2] = 0.0f;
			o->m_dwTime = g_CurrentTime;
			o->Scale = o->Scale;
		}
		break;

	case MODEL_EFFECT_FIRE_HIK3_MONO:		
		{
			mu_int32 iRandNum = (GetLargeRand(100));

			if (iRandNum > 20)
			{
				CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3_MONO, o->Position, o->Angle, o->Light, 1, o->Scale);
			}
		}
		break;

	case MODEL_PROJECTILE:
		{
			
			o->LifeTime = 50;
			o->Gravity = 0.0f;
			o->Velocity = 70.0f;
			VectorCopy(Position, o->StartPosition); 
													
		}
		break;
	case MODEL_DOOR_CRUSH_EFFECT_PIECE01:	
	case MODEL_DOOR_CRUSH_EFFECT_PIECE02:	
	case MODEL_DOOR_CRUSH_EFFECT_PIECE03:	
	case MODEL_DOOR_CRUSH_EFFECT_PIECE04:	
	case MODEL_DOOR_CRUSH_EFFECT_PIECE05:	
	case MODEL_DOOR_CRUSH_EFFECT_PIECE06:	
	case MODEL_DOOR_CRUSH_EFFECT_PIECE07:	
	case MODEL_DOOR_CRUSH_EFFECT_PIECE08:	
	case MODEL_DOOR_CRUSH_EFFECT_PIECE09:	
	case MODEL_DOOR_CRUSH_EFFECT_PIECE11:	
	case MODEL_DOOR_CRUSH_EFFECT_PIECE12:	
	case MODEL_DOOR_CRUSH_EFFECT_PIECE13:	
	case MODEL_STATUE_CRUSH_EFFECT_PIECE01:	
	case MODEL_STATUE_CRUSH_EFFECT_PIECE02:	
	case MODEL_STATUE_CRUSH_EFFECT_PIECE03:	
		{
			o->LifeTime = 30 + (GetLargeRand(30));
			o->Velocity = 0.0f;
			o->PKKey = -1;
			o->Owner = Owner;
			o->Gravity = 2.3f;

			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			Vector(0.0f, static_cast<mu_float>(GetLargeRand(64) + 48)*0.1f, 0.0f, p1);
			VectorRotate(p1, Matrix, o->HeadAngle);
			o->HeadAngle[2] = 15;

			o->SubType = GetLargeRand(2);

			o->Light[0] = o->Light[1] = o->Light[2] = 0.5f + static_cast<mu_float>(GetLargeRand(6))*0.1f;
			o->Direction[0] = 0;
			o->Direction[1] = 0;
			o->Direction[2] = 0;
		}
		break;
	case MODEL_STATUE_CRUSH_EFFECT_PIECE04:	
	case MODEL_DOOR_CRUSH_EFFECT_PIECE10:	
		{
			o->LifeTime = 100;
			o->Scale = Scale;

			o->PKKey = -1;
			o->Owner = Owner;
		}
		break;
	case MODEL_DOOR_CRUSH_EFFECT:	
		{
			switch (o->SubType)

			{
			case 0:
				{
					EVector3 vPos;
					for (mu_int32 i = 0; i<9; ++i)
					{
						Vector(o->Position[0] + (GetLargeRand(200) - 100),
							o->Position[1] + (GetLargeRand(200) - 100),
							o->Position[2] + (GetLargeRand(200) - 100),
							vPos);
						CreateEffect(ThreadIndex, MODEL_DOOR_CRUSH_EFFECT_PIECE01 + i, vPos, o->Angle, o->Light, 0, o->Owner, 0, 0);
					}
				}break;
			case 1:	
				{
					EVector3 vPos;
					for (mu_int32 i = 0; i<9; ++i)
					{
						Vector(o->Position[0] + (GetLargeRand(200) - 100),
							o->Position[1] + (GetLargeRand(200) - 100),
							o->Position[2] + (GetLargeRand(200) - 100),
							vPos);
						CreateEffect(ThreadIndex, MODEL_DOOR_CRUSH_EFFECT_PIECE11 + (i % 3), vPos, o->Angle, o->Light, 0, o->Owner, 0, 0);
					}
				}break;
			}
		}
		break;
	case MODEL_STATUE_CRUSH_EFFECT:			
		{
			for (mu_int32 i = 0; i<6; ++i)
			{
				CreateEffect(ThreadIndex, MODEL_STATUE_CRUSH_EFFECT_PIECE01 + (i % 3), o->Position, o->Angle, o->Light, 0, o->Owner, 0, 0);
			}
		}
		break;

	case MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_:
	case MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_:
	case MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_:
	case MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_:
	case MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_:
	case MODEL_EMPIREGUARDIANBOSS_FRAMESTRIKE:
		{
			MUObjectExtra *ox = &g_EffectsExtra[i];
			OBJECT	*O = &(Hero->Object);
			for (mu_int32 i = 113; i <= 155; ++i)
			{
				RemoveObjectBlurs(O, i);
			}

			if (o->SubType == 0)		
			{
				o->LifeTime = 2;
				o->Flags.Set(OBJECT::eOF_CHROMEENABLE);
			}
			else if (o->SubType == 3)		
			{
				const mu_int32	TOTAL_LIFETIME = 60;		
				EVector3		v3PosStart, v3PosTarget;
				EVector3		arv3PosProcess[4];

				o->ExtState = TOTAL_LIFETIME;			
				o->LifeTime = TOTAL_LIFETIME;
				o->Gravity = 0.0f;
				o->HiddenMesh = 1;
				o->Distance = 0.0f;
				o->Alpha = 1.0f;

				Vector(o->Angle[0], o->Angle[1], o->Angle[2], o->HeadAngle);

				VectorCopy(o->Position, o->StartPosition);	
				VectorCopy(o->Position, v3PosStart);			
				VectorCopy(o->Light, v3PosTarget);			

															
				EVector3		v3DirDistAD;
				EVector3		v3PosStartModify, v3PosTargetModify;
				const mu_int32	iLimitArea1 = 200;
				mu_float		fDistAD, fDistAB;
				mu_float		fHeightTerrainTarget = RequestTerrainHeight(v3PosTarget[0], v3PosTarget[1]);
				mu_int32			iOffsetDist = 100;
				
				VectorCopy(v3PosStart, v3PosStartModify);
				VectorCopy(v3PosTarget, v3PosTargetModify);
				v3PosTargetModify[2] = v3PosStartModify[2];
				fDistAD = VectorDistance3D_DirDist(v3PosStartModify, v3PosTargetModify, v3DirDistAD);

				fDistAB = 900.0f + (GetLargeRand(iOffsetDist) - (iOffsetDist / 2));
				VectorCopy(v3PosStart, arv3PosProcess[0]);		
																
				arv3PosProcess[1][0] = arv3PosProcess[0][0] + static_cast<mu_float>(GetLargeRand(iLimitArea1) - (iLimitArea1 / 2));
				arv3PosProcess[1][1] = arv3PosProcess[0][1] + static_cast<mu_float>(GetLargeRand(iLimitArea1) - (iLimitArea1 / 2));
				arv3PosProcess[1][2] = v3PosStart[2] + fDistAB;
				VectorAdd(arv3PosProcess[1], v3DirDistAD, arv3PosProcess[2]);	
																
				VectorCopy(v3PosTargetModify, arv3PosProcess[3]);
				arv3PosProcess[3][0] = arv3PosProcess[2][0];
				arv3PosProcess[3][1] = arv3PosProcess[2][1];
				arv3PosProcess[3][2] = fHeightTerrainTarget + static_cast<mu_float>(iOffsetDist)*1.2f + (GetLargeRand(iOffsetDist) - (iOffsetDist / 2));
 					
				mu_float	arfRates[] = { 0.0f, 0.22f, 0.35f, 0.50f, 1.01f };
				mu_float	fOffsetRate = (0.03f * (o->Type - MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_));

				ox->m_Interpolates.ClearContainer();
				
				CInterpolateContainer::INTERPOLATE_FACTOR	InsertFactor;
				InsertFactor.fRateStart = arfRates[0] + fOffsetRate;
				InsertFactor.fRateEnd = arfRates[1] + fOffsetRate;
				VectorCopy(v3PosStart, InsertFactor.v3Start);
				VectorCopy(arv3PosProcess[1], InsertFactor.v3End);
				ox->m_Interpolates.m_vecInterpolatesPos.push_back(InsertFactor);		

				InsertFactor.fRateStart = arfRates[1] + fOffsetRate;
				InsertFactor.fRateEnd = arfRates[2] + fOffsetRate;
				VectorCopy(arv3PosProcess[1], InsertFactor.v3Start);
				VectorCopy(arv3PosProcess[2], InsertFactor.v3End);
				ox->m_Interpolates.m_vecInterpolatesPos.push_back(InsertFactor);		

				InsertFactor.fRateStart = arfRates[2] + fOffsetRate;
				InsertFactor.fRateEnd = arfRates[3] + fOffsetRate;
				VectorCopy(arv3PosProcess[2], InsertFactor.v3Start);
				VectorCopy(arv3PosProcess[3], InsertFactor.v3End);
				ox->m_Interpolates.m_vecInterpolatesPos.push_back(InsertFactor);		

				InsertFactor.fRateStart = arfRates[3] + fOffsetRate;
				InsertFactor.fRateEnd = arfRates[4];
				VectorCopy(arv3PosProcess[3], InsertFactor.v3Start);
				VectorCopy(arv3PosProcess[3], InsertFactor.v3End);
				ox->m_Interpolates.m_vecInterpolatesPos.push_back(InsertFactor);		
																					
				CInterpolateContainer::INTERPOLATE_FACTOR_F	InsertFactorF;
				InsertFactorF.fRateStart = arfRates[0];
				InsertFactorF.fRateEnd = arfRates[3];
				InsertFactorF.fStart = 1.0f;
				InsertFactorF.fEnd = 1.0f;

				InsertFactorF.fRateStart = arfRates[3];
				InsertFactorF.fRateEnd = arfRates[4];
				InsertFactorF.fStart = 1.0f;
				InsertFactorF.fEnd = 0.0f;
				ox->m_Interpolates.m_vecInterpolatesAlpha.push_back(InsertFactorF);		
																						
				ox->m_Interpolates.GetAngleCurrent(o->Angle, 0.0f);		
				ox->m_Interpolates.GetPosCurrent(o->Position, 0.0f);		
				ox->m_Interpolates.GetAlphaCurrent(o->Alpha, 0.0f);		

				CreateJoint(ThreadIndex, BITMAP_FLARE + 1, o->Position, o->Position, o->Angle, 20, o, 160.0f, 40);
			}
			else if (o->SubType == 11)	
			{
				const mu_int32	TOTALLIFETIME = 24;
				o->LifeTime = TOTALLIFETIME;
				o->ExtState = TOTALLIFETIME;
				o->Flags.Set(OBJECT::eOF_CHROMEENABLE);
				o->Scale = Scale;

				ox->m_Interpolates.ClearContainer();

				CInterpolateContainer::INTERPOLATE_FACTOR_F InsertFactorF;
				InsertFactorF.fRateStart = 0.0f;
				InsertFactorF.fRateEnd = 0.61f;
				InsertFactorF.fStart = 0.0f;
				InsertFactorF.fEnd = 1.0f;
				ox->m_Interpolates.m_vecInterpolatesAlpha.push_back(InsertFactorF);		

				InsertFactorF.fRateStart = 0.61f;
				InsertFactorF.fRateEnd = 1.01f;
				InsertFactorF.fStart = 1.0f;
				InsertFactorF.fEnd = 1.0f;
				ox->m_Interpolates.m_vecInterpolatesAlpha.push_back(InsertFactorF);		
			}
			else if (o->SubType == 12)	
			{
				const mu_int32	TOTALLIFETIME = 14;
				o->LifeTime = TOTALLIFETIME;
				o->ExtState = TOTALLIFETIME;
				o->Flags.Set(OBJECT::eOF_CHROMEENABLE);
				o->Scale = Scale;

				MUObjectExtra *ox = &g_EffectsExtra[i];
				ox->m_Interpolates.ClearContainer();

				CInterpolateContainer::INTERPOLATE_FACTOR_F InsertFactorF;
				InsertFactorF.fRateStart = 0.0f;
				InsertFactorF.fRateEnd = 1.01f;
				InsertFactorF.fStart = 0.0f;
				InsertFactorF.fEnd = 1.0f;
				ox->m_Interpolates.m_vecInterpolatesAlpha.push_back(InsertFactorF);		
			}
			else if (o->SubType == 20)	
			{
				const mu_int32	TOTALLIFETIME = 10;
				o->LifeTime = TOTALLIFETIME;
				o->ExtState = TOTALLIFETIME;

				switch (o->Type)
				{
				case MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_:
				case MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_:
					{
						o->LifeTime = 10;
					}
					break;
				case MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_:
				case MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_:
					{
						o->LifeTime = 5;
					}
					break;
				case MODEL_EMPIREGUARDIANBOSS_FRAMESTRIKE:
					{
						o->LifeTime = 5;
					}
				}

				ox->m_Interpolates.ClearContainer();

				CInterpolateContainer::INTERPOLATE_FACTOR	InsertFactor;
				InsertFactor.fRateStart = 0.0f;
				InsertFactor.fRateEnd = 1.0f;
				VectorCopy(o->Angle, InsertFactor.v3Start);
				VectorCopy(o->Angle, InsertFactor.v3End);
				ox->m_Interpolates.m_vecInterpolatesAngle.push_back(InsertFactor);

				InsertFactor.fRateStart = 0.0f;
				InsertFactor.fRateEnd = 1.0f;
				VectorCopy(o->Position, InsertFactor.v3Start);
				VectorCopy(o->Position, InsertFactor.v3End);
				ox->m_Interpolates.m_vecInterpolatesPos.push_back(InsertFactor);

				CInterpolateContainer::INTERPOLATE_FACTOR_F	InsertFactorF;
				InsertFactorF.fRateStart = 0.0f;
				InsertFactorF.fRateEnd = 1.0f;
				InsertFactorF.fStart = 0.7f;
				InsertFactorF.fEnd = 0.0f;
				ox->m_Interpolates.m_vecInterpolatesAlpha.push_back(InsertFactorF);

				InsertFactorF.fRateStart = 0.0f;
				InsertFactorF.fRateEnd = 1.0f;
				InsertFactorF.fStart = o->Scale;
				InsertFactorF.fEnd = o->Scale;
				ox->m_Interpolates.m_vecInterpolatesScale.push_back(InsertFactorF);
			}
			else	
			{
				switch (o->Type)
				{
				case MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_:		
					{
						if (o->SubType == 1)	
						{
							const mu_int32	TOTAL_LIFETIME = 35;
							EVector3		v3PosProcess01, v3PosProcessFinal,
								v3DirModify, v3PosModify,
								v3PosTargetModify;

							o->ExtState = TOTAL_LIFETIME;			
							o->LifeTime = TOTAL_LIFETIME;
							o->Gravity = 0.0f;
							o->HiddenMesh = 1;
							o->Distance = 0.0f;
							o->Alpha = 1.0f;
							Vector(o->Angle[0], o->Angle[1], o->Angle[2], o->HeadAngle);

							VectorCopy(o->Position, o->StartPosition);	

							VectorSubtract(o->Light, o->Position, o->Direction);

							o->Distance = EMath::Sqrt(o->Direction[0] * o->Direction[0] +
								o->Direction[1] * o->Direction[1] +
								o->Direction[2] * o->Direction[2]);

							VectorDivFSelf(o->Direction, o->Distance);

							mu_float	fDistanceResult = 0;
							EVector3	v3DirResult;

							EVector3 v3PosModify02;
							mu_float	fTotalDist = 1700.0f, fFirstDist = 0.0f, fRateFirstDist = 0.0f;
							VectorCopy(o->Owner->Position, v3PosModify02);
							VectorCopy(o->Position, v3PosModify);

							v3PosModify02[2] = o->Light[2];
							v3PosModify[2] = o->Light[2];
							VectorCopy(o->Light, v3PosTargetModify);
							v3PosModify[2] = v3PosModify[2] + 100.0f;
							v3PosModify02[2] = v3PosModify02[2] + 100.0f;
							v3PosTargetModify[2] = v3PosTargetModify[2] + 100.0f;
							VectorDistNormalize(v3PosModify02, v3PosTargetModify, v3DirModify);

							fDistanceResult = o->Distance * 0.3f;
							fFirstDist = fDistanceResult;
							VectorMulF(v3DirModify, fDistanceResult, v3DirResult);
							VectorAdd(v3PosModify, v3DirResult, v3PosProcess01);

							fDistanceResult = fTotalDist;
							VectorMulF(v3DirModify, fDistanceResult, v3DirResult);
							VectorAdd(v3PosModify, v3DirResult, v3PosProcessFinal);

							fRateFirstDist = fFirstDist / fTotalDist;

							ox->m_Interpolates.ClearContainer();

							fRateFirstDist = fFirstDist / fTotalDist;

							CInterpolateContainer::INTERPOLATE_FACTOR	InsertFactor;
							InsertFactor.fRateStart = 0.0f;			
							InsertFactor.fRateEnd = fRateFirstDist;			
							Vector(0.0f, 0.0f, 0.0f, InsertFactor.v3Start);
							Vector(90.0f, 0.0f, 0.0f, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesAngle.push_back(InsertFactor);

							InsertFactor.fRateStart = fRateFirstDist;			
							InsertFactor.fRateEnd = 1.01f;
							Vector(90.0f, 0.0f, 0.0f, InsertFactor.v3Start);
							Vector(90.0f, 0.0f, 1560.0f, InsertFactor.v3End);
							
							ox->m_Interpolates.m_vecInterpolatesAngle.push_back(InsertFactor);		
							InsertFactor.fRateStart = 0.0f;
							InsertFactor.fRateEnd = fRateFirstDist;
							VectorCopy(o->Position, InsertFactor.v3Start);
							VectorCopy(v3PosProcess01, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesPos.push_back(InsertFactor);		

							InsertFactor.fRateStart = fRateFirstDist;
							InsertFactor.fRateEnd = 1.01f;
							VectorCopy(v3PosProcess01, InsertFactor.v3Start);
							VectorCopy(v3PosProcessFinal, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesPos.push_back(InsertFactor);		
							ox->m_Interpolates.GetAngleCurrent(o->Angle, 0.0f);		
							ox->m_Interpolates.GetPosCurrent(o->Position, 0.0f);	
						}
					}	
					break;
				case MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_:		
					{
						if (o->SubType == 1)	
						{
							const mu_int32	TOTAL_LIFETIME = 35;
							EVector3		v3PosProcess01, v3PosProcessFinal,
								v3DirModify, v3PosModify,
								v3PosTargetModify;

							o->ExtState = TOTAL_LIFETIME;			
							o->LifeTime = TOTAL_LIFETIME;
							o->Gravity = 0.0f;
							o->HiddenMesh = 1;
							o->Distance = 0.0f;
							o->Alpha = 1.0f;

							Vector(o->Angle[0], o->Angle[1], o->Angle[2], o->HeadAngle);

							VectorCopy(o->Position, o->StartPosition);	

							VectorSubtract(o->Light, o->Position, o->Direction);

							o->Distance = EMath::Sqrt(o->Direction[0] * o->Direction[0] +
								o->Direction[1] * o->Direction[1] +
								o->Direction[2] * o->Direction[2]);

							VectorDivFSelf(o->Direction, o->Distance);

							mu_float	fDistanceResult = 0;
							EVector3	v3DirResult;

							EVector3 v3PosModify02;
							mu_float	fTotalDist = 1700.0f, fFirstDist = 0.0f, fRateFirstDist = 0.0f;
							VectorCopy(o->Owner->Position, v3PosModify02);
							VectorCopy(o->Position, v3PosModify);

							v3PosModify02[2] = o->Light[2];
							v3PosModify[2] = o->Light[2];
							VectorCopy(o->Light, v3PosTargetModify);
							v3PosModify[2] = v3PosModify[2] + 100.0f;
							v3PosModify02[2] = v3PosModify02[2] + 100.0f;
							v3PosTargetModify[2] = v3PosTargetModify[2] + 100.0f;
							VectorDistNormalize(v3PosModify02, v3PosTargetModify, v3DirModify);

							fDistanceResult = o->Distance * 0.3f;
							fFirstDist = fDistanceResult;
							VectorMulF(v3DirModify, fDistanceResult, v3DirResult);
							VectorAdd(v3PosModify, v3DirResult, v3PosProcess01);

							fDistanceResult = fTotalDist;
							VectorMulF(v3DirModify, fDistanceResult, v3DirResult);
							VectorAdd(v3PosModify, v3DirResult, v3PosProcessFinal);

							fRateFirstDist = fFirstDist / fTotalDist;

							ox->m_Interpolates.ClearContainer();

							CInterpolateContainer::INTERPOLATE_FACTOR	InsertFactor;
							InsertFactor.fRateStart = 0.0f;			
							InsertFactor.fRateEnd = fRateFirstDist;			
							Vector(0.0f, 0.0f, 0.0f, InsertFactor.v3Start);
							Vector(90.0f, 0.0f, 0.0f, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesAngle.push_back(InsertFactor);		

							InsertFactor.fRateStart = fRateFirstDist;			
							InsertFactor.fRateEnd = 1.01f;
							Vector(90.0f, 0.0f, 0.0f, InsertFactor.v3Start);
							Vector(90.0f, 0.0f, -1560.0f, InsertFactor.v3End);
							
							ox->m_Interpolates.m_vecInterpolatesAngle.push_back(InsertFactor);		

							InsertFactor.fRateStart = 0.0f;
							InsertFactor.fRateEnd = fRateFirstDist;
							VectorCopy(o->Position, InsertFactor.v3Start);
							VectorCopy(v3PosProcess01, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesPos.push_back(InsertFactor);		

							InsertFactor.fRateStart = fRateFirstDist;
							InsertFactor.fRateEnd = 1.01f;
							VectorCopy(v3PosProcess01, InsertFactor.v3Start);
							VectorCopy(v3PosProcessFinal, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesPos.push_back(InsertFactor);		
							
							ox->m_Interpolates.GetAngleCurrent(o->Angle, 0.0f);		
							ox->m_Interpolates.GetPosCurrent(o->Position, 0.0f);		
						}
					}	
					break;
				case MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_:		
					{
						if (o->SubType == 1)	
						{
							const mu_int32	TOTAL_LIFETIME = 30;
							EVector3		v3PosProcess01, v3PosProcessFinal,
								v3AngleOffset,
								v3Dir,
								v3DirPower,
								v3DirModify,
								v3DirModifyR;

							o->ExtState = TOTAL_LIFETIME;			
							o->LifeTime = TOTAL_LIFETIME;
							o->Gravity = 0.2f;
							o->HiddenMesh = 1;
							o->Distance = 0.0f;
							o->Alpha = 1.0f;

							Vector(o->Angle[0], o->Angle[1], o->Angle[2], o->HeadAngle);

							VectorCopy(o->Position, o->StartPosition);	

							EVector3	v3PosStartModify, v3PosEndModify;
							VectorCopy(o->Light, v3PosEndModify);
							VectorCopy(o->StartPosition, v3PosStartModify);
							v3PosStartModify[2] = v3PosEndModify[2];

							VectorSubtract(v3PosEndModify, v3PosStartModify, v3DirModify);
							VectorSubtract(o->Light, o->StartPosition, v3Dir);

							o->Distance = EMath::Sqrt(v3Dir[0] * v3Dir[0] +
								v3Dir[1] * v3Dir[1] +
								v3Dir[2] * v3Dir[2]);

							v3Dir[0] /= o->Distance;
							v3Dir[1] /= o->Distance;
							v3Dir[2] /= o->Distance;

							VectorNormalize(v3DirModify);
							VectorCopy(v3Dir, o->Direction);

							Vector(0.0f, 0.0f, -40.0f, v3AngleOffset);
							AngleMatrix(v3AngleOffset, Matrix);
							VectorRotate(v3DirModify, Matrix, v3DirModifyR);
							VectorMulF(v3DirModifyR, o->Distance * 0.4f, v3DirPower);
							VectorAdd(o->StartPosition, v3DirPower, v3PosProcess01);
							v3PosProcess01[2] -= 100.0f;

							Vector(0.0f, 0.0f, 24.0f, v3AngleOffset);
							AngleMatrix(v3AngleOffset, Matrix);
							VectorRotate(v3DirModify, Matrix, v3DirModifyR);
							VectorMulF(v3DirModifyR, o->Distance + 700.0f, v3DirPower);
							VectorAdd(o->StartPosition, v3DirPower, v3PosProcessFinal);
							
							v3PosProcessFinal[2] -= 290.0f;

							ox->m_Interpolates.ClearContainer();

							CInterpolateContainer::INTERPOLATE_FACTOR	InsertFactor;
							InsertFactor.fRateStart = 0.0f;			
							InsertFactor.fRateEnd = 0.32f;			
							Vector(0.0f, 0.0f, 0.0f, InsertFactor.v3Start);
							
							Vector(90.0f, 0.0f, 0.0f, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesAngle.push_back(InsertFactor);		

							InsertFactor.fRateStart = 0.32f;			
							InsertFactor.fRateEnd = 1.01f;			
																	
							Vector(90.0f, 0.0f, 0.0f, InsertFactor.v3Start);
							Vector(90.0f, 0.0f, 340.0f, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesAngle.push_back(InsertFactor);		
																									
							InsertFactor.fRateStart = 0.0f;
							InsertFactor.fRateEnd = 0.32f;
							VectorCopy(o->Position, InsertFactor.v3Start);
							VectorCopy(v3PosProcess01, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesPos.push_back(InsertFactor);		

							InsertFactor.fRateStart = 0.32f;
							InsertFactor.fRateEnd = 1.01f;
							VectorCopy(v3PosProcess01, InsertFactor.v3Start);
							VectorCopy(v3PosProcessFinal, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesPos.push_back(InsertFactor);		

							CInterpolateContainer::INTERPOLATE_FACTOR_F InsertFactorF;
							InsertFactorF.fRateStart = 0.0f;
							InsertFactorF.fRateEnd = 0.15f;
							InsertFactorF.fStart = 1.0f;
							InsertFactorF.fEnd = 1.0f;
							ox->m_Interpolates.m_vecInterpolatesAlpha.push_back(InsertFactorF);		

							InsertFactorF.fRateStart = 0.15f;
							InsertFactorF.fRateEnd = 0.75f;
							InsertFactorF.fStart = 1.0f;
							InsertFactorF.fEnd = 1.0f;
							ox->m_Interpolates.m_vecInterpolatesAlpha.push_back(InsertFactorF);		

							InsertFactorF.fRateStart = 0.75f;
							InsertFactorF.fRateEnd = 1.01f;
							InsertFactorF.fStart = 1.0f;
							InsertFactorF.fEnd = 0.0f;
							ox->m_Interpolates.m_vecInterpolatesAlpha.push_back(InsertFactorF);		
							
							ox->m_Interpolates.GetAngleCurrent(o->Angle, 0.0f);		
							ox->m_Interpolates.GetPosCurrent(o->Position, 0.0f);		
							ox->m_Interpolates.GetAlphaCurrent(o->Alpha, 0.0f);		
						}
					}	
					break;
				case MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_:		
					{
						if (o->SubType == 1)	
						{
							const mu_int32	TOTAL_LIFETIME = 30;
							EVector3		v3PosProcess01, v3PosProcessFinal,
								v3AngleOffset,
								v3Dir,
								v3DirPower,
								v3DirModify,
								v3DirModifyR;

							o->ExtState = TOTAL_LIFETIME;
							o->LifeTime = TOTAL_LIFETIME;
							o->Gravity = 0.2f;
							o->HiddenMesh = 1;
							o->Distance = 0.0f;
							o->Alpha = 1.0f;

							Vector(o->Angle[0], o->Angle[1], o->Angle[2], o->HeadAngle);

							VectorCopy(o->Position, o->StartPosition);

							EVector3	v3PosStartModify, v3PosEndModify;
							VectorCopy(o->Light, v3PosEndModify);
							VectorCopy(o->StartPosition, v3PosStartModify);
							v3PosStartModify[2] = v3PosEndModify[2];

							VectorSubtract(v3PosEndModify, v3PosStartModify, v3DirModify);
							VectorSubtract(o->Light, o->StartPosition, v3Dir);

							o->Distance = EMath::Sqrt(v3Dir[0] * v3Dir[0] +
								v3Dir[1] * v3Dir[1] +
								v3Dir[2] * v3Dir[2]);

							v3Dir[0] /= o->Distance;
							v3Dir[1] /= o->Distance;
							v3Dir[2] /= o->Distance;

							VectorNormalize(v3DirModify);
							VectorCopy(v3Dir, o->Direction);

							Vector(0.0f, 0.0f, 50.0f, v3AngleOffset);
							AngleMatrix(v3AngleOffset, Matrix);
							VectorRotate(v3DirModify, Matrix, v3DirModifyR);
							VectorMulF(v3DirModifyR, o->Distance * 0.4f, v3DirPower);
							VectorAdd(o->StartPosition, v3DirPower, v3PosProcess01);
							v3PosProcess01[2] -= 100.0f;

							Vector(0.0f, 0.0f, -26.0f, v3AngleOffset);
							AngleMatrix(v3AngleOffset, Matrix);
							VectorRotate(v3DirModify, Matrix, v3DirModifyR);
							VectorMulF(v3DirModifyR, o->Distance + 700.0f, v3DirPower);
							VectorAdd(o->StartPosition, v3DirPower, v3PosProcessFinal);
							v3PosProcessFinal[2] -= 280.0f;

							ox->m_Interpolates.ClearContainer();

							CInterpolateContainer::INTERPOLATE_FACTOR	InsertFactor;
							InsertFactor.fRateStart = 0.0f;
							InsertFactor.fRateEnd = 0.4f;
							Vector(0.0f, 0.0f, 0.0f, InsertFactor.v3Start);
							Vector(110.0f, 0.0f, 0.0f, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesAngle.push_back(InsertFactor);

							InsertFactor.fRateStart = 0.4f;
							InsertFactor.fRateEnd = 1.01f;
							Vector(110.0f, 0.0f, 0.0f, InsertFactor.v3Start);
							Vector(60.0f, 0.0f, -300.0f, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesAngle.push_back(InsertFactor);

							InsertFactor.fRateStart = 0.0f;
							InsertFactor.fRateEnd = 0.33f;
							VectorCopy(o->Position, InsertFactor.v3Start);
							VectorCopy(v3PosProcess01, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesPos.push_back(InsertFactor);

							InsertFactor.fRateStart = 0.33f;
							InsertFactor.fRateEnd = 1.01f;
							VectorCopy(v3PosProcess01, InsertFactor.v3Start);
							VectorCopy(v3PosProcessFinal, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesPos.push_back(InsertFactor);

							CInterpolateContainer::INTERPOLATE_FACTOR_F InsertFactorF;
							InsertFactorF.fRateStart = 0.0f;
							InsertFactorF.fRateEnd = 0.15f;
							InsertFactorF.fStart = 1.0f;
							InsertFactorF.fEnd = 1.0f;
							ox->m_Interpolates.m_vecInterpolatesAlpha.push_back(InsertFactorF);

							InsertFactorF.fRateStart = 0.15f;
							InsertFactorF.fRateEnd = 0.75f;
							InsertFactorF.fStart = 1.0f;
							InsertFactorF.fEnd = 1.0f;
							ox->m_Interpolates.m_vecInterpolatesAlpha.push_back(InsertFactorF);

							InsertFactorF.fRateStart = 0.75f;
							InsertFactorF.fRateEnd = 1.01f;
							InsertFactorF.fStart = 1.0f;
							InsertFactorF.fEnd = 0.0f;
							ox->m_Interpolates.m_vecInterpolatesAlpha.push_back(InsertFactorF);

							ox->m_Interpolates.GetAngleCurrent(o->Angle, 0.0f);
							ox->m_Interpolates.GetPosCurrent(o->Position, 0.0f);
							ox->m_Interpolates.GetAlphaCurrent(o->Alpha, 0.0f);
						}
					}	
					break;
				case MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_:		
					{
						if (o->SubType == 1)
						{
							const mu_int32	TOTAL_LIFETIME = 10;

							o->ExtState = TOTAL_LIFETIME;			
							o->LifeTime = TOTAL_LIFETIME;
							o->Flags.Set(OBJECT::eOF_CHROMEENABLE);

							ox->m_Interpolates.ClearContainer();

							CInterpolateContainer::INTERPOLATE_FACTOR_F InsertFactorF;
							InsertFactorF.fRateStart = 0.0f;
							InsertFactorF.fRateEnd = 1.01f;
							InsertFactorF.fStart = 1.0f;
							InsertFactorF.fEnd = 0.0f;
							ox->m_Interpolates.m_vecInterpolatesAlpha.push_back(InsertFactorF);
						}
					}	
					break;
				case MODEL_EMPIREGUARDIANBOSS_FRAMESTRIKE:
					{
						if (o->SubType == 1)
						{
							const mu_int32	TOTAL_LIFETIME = 36;
							EVector3		v3PosProcess01,
								v3DirModify,
								v3PosModify,
								v3PosTargetModify;

							o->ExtState = TOTAL_LIFETIME;			
							o->LifeTime = TOTAL_LIFETIME;
							o->Gravity = 0.0f;
							o->HiddenMesh = 1;
							o->Distance = 0.0f;
							o->Alpha = 1.0f;				
							o->Scale = 1.0f;
							o->Velocity = 0.4f;

							Vector(o->Angle[0], o->Angle[1], o->Angle[2], o->HeadAngle);

							VectorSubtract(o->Light, o->Position, o->Direction);

							o->Distance = EMath::Sqrt(o->Direction[0] * o->Direction[0] +
								o->Direction[1] * o->Direction[1] +
								o->Direction[2] * o->Direction[2]);

							VectorDivFSelf(o->Direction, o->Distance);

							VectorCopy(o->Owner->Position, v3PosModify);

							v3PosModify[2] = o->Light[2];
							VectorCopy(o->Light, v3PosTargetModify);

							v3PosModify[2] = v3PosModify[2] + 100.0f;
							v3PosTargetModify[2] = v3PosTargetModify[2] + 100.0f;
							VectorDistNormalize(v3PosTargetModify, v3PosModify, v3DirModify);

							VectorMA(v3PosTargetModify, 200.0f, v3DirModify, v3PosProcess01);

							VectorCopy(v3PosProcess01, o->StartPosition);	

							ox->m_Interpolates.ClearContainer();

							CInterpolateContainer::INTERPOLATE_FACTOR	InsertFactor;
							InsertFactor.fRateStart = 0.0f;
							InsertFactor.fRateEnd = 0.28f;
							Vector(0.0f, 0.0f, 0.0f, InsertFactor.v3Start);
							Vector(0.0f, 0.0f, 0.0f, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesAngle.push_back(InsertFactor);		

							InsertFactor.fRateStart = 0.28f;
							InsertFactor.fRateEnd = 0.36f;
							Vector(0.0f, 0.0f, 0.0f, InsertFactor.v3Start);
							Vector(-90.0f, 0.0f, 0.0f, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesAngle.push_back(InsertFactor);		

							InsertFactor.fRateStart = 0.36f;
							InsertFactor.fRateEnd = 1.01f;
							Vector(-90.0f, 0.0f, 0.0f, InsertFactor.v3Start);
							Vector(-90.0f, 0.0f, 880.0f, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesAngle.push_back(InsertFactor);		
																									
							InsertFactor.fRateStart = 0.0f;
							InsertFactor.fRateEnd = 1.01f;
							VectorCopy(v3PosProcess01, InsertFactor.v3Start);
							VectorCopy(v3PosProcess01, InsertFactor.v3End);
							ox->m_Interpolates.m_vecInterpolatesPos.push_back(InsertFactor);		

							CInterpolateContainer::INTERPOLATE_FACTOR_F	InsertFactorF;
							InsertFactorF.fRateStart = 0.0f;
							InsertFactorF.fRateEnd = 1.01f;
							InsertFactorF.fStart = o->Scale * 1.0f;
							InsertFactorF.fEnd = o->Scale * 1.0f;
							ox->m_Interpolates.m_vecInterpolatesScale.push_back(InsertFactorF);		

							InsertFactorF.fRateStart = 0.0f;
							InsertFactorF.fRateEnd = 0.15f;
							InsertFactorF.fStart = 0.0f;
							InsertFactorF.fEnd = 1.0f;
							ox->m_Interpolates.m_vecInterpolatesAlpha.push_back(InsertFactorF);		

							InsertFactorF.fRateStart = 0.15f;
							InsertFactorF.fRateEnd = 0.75f;
							InsertFactorF.fStart = 1.0f;
							InsertFactorF.fEnd = 1.0f;
							ox->m_Interpolates.m_vecInterpolatesAlpha.push_back(InsertFactorF);		

							InsertFactorF.fRateStart = 0.75f;
							InsertFactorF.fRateEnd = 1.01f;
							InsertFactorF.fStart = 1.0f;
							InsertFactorF.fEnd = 0.0f;
							ox->m_Interpolates.m_vecInterpolatesAlpha.push_back(InsertFactorF);		

							ox->m_Interpolates.GetAngleCurrent(o->Angle, 0.0f);		
							ox->m_Interpolates.GetPosCurrent(o->Position, 0.0f);		
							ox->m_Interpolates.GetAlphaCurrent(o->Alpha, 0.0f);		
							ox->m_Interpolates.GetScaleCurrent(o->Scale, 0.0f);		
						}
					}
				}
			}
		}
		break;

	case MODEL_EMPIREGUARDIAN_BLOW_OF_DESTRUCTION:	
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 40;

				EVector3 vPos, vPos2;
				EBone Matrix;
				Vector(-20.0f, -100.0f, 0.0f, vPos);
				AngleMatrix(o->Owner->Angle, Matrix);
				VectorRotate(vPos, Matrix, vPos2);
				VectorAdd(vPos2, o->Position, o->Position);

				VectorCopy(o->Light, o->StartPosition);

				Vector(1.2f, 1.2f, 1.2f, o->Light);

				CreateEffect(ThreadIndex, MODEL_EMPIREGUARDIAN_BLOW_OF_DESTRUCTION,
					o->StartPosition, o->Angle,
					o->Light, 1, o->Owner, -1, 0,
					0, 0, o->Scale);
			}
			else if (o->SubType == 1)
			{
				o->LifeTime = 40;
				Vector(1.2f, 1.2f, 1.2f, o->Light);
				o->Position[2] = 150.0f;
				o->Scale = o->Scale * 2.0f;
			}
		}
		break;

	case BITMAP_RING_OF_GRADATION:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 20;
			}
		}break;
	case MODEL_EFFECT_UMBRELLA_DIE:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 30;
			}
		}break;
	case MODEL_EFFECT_UMBRELLA_GOLD:
		{
			o->LifeTime = GetLargeRand(10) + 50;
			o->Scale = 2.0f + (GetLargeRand(10) - 5)*0.2f;

			o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));
			o->Angle[1] = static_cast<mu_float>(GetLargeRand(360));
			o->Angle[2] = static_cast<mu_float>(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			o->Gravity = static_cast<mu_float>(GetLargeRand(10) + 10);
			EVector3 p;
			Vector(static_cast<mu_float>(GetLargeRand(10) - 5)*0.1f, static_cast<mu_float>(GetLargeRand(60) - 30)*0.1f, 0.0f, p);
			VectorScale(p, 1.2f, p);
			VectorRotate(p, Matrix, o->Direction);

			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 100.0f;
		}break;

	case MODEL_EFFECT_EG_GUARDIANDEFENDER_ATTACK2:		
		{
			o->LifeTime = 20;
			o->Scale = 0.9f;
		}break;

	case MODEL_EFFECT_SD_AURA:
		{
			o->LifeTime = 1000;
			o->Scale = 1.0f;
		}
		break;

	case BITMAP_WATERFALL_4:
		{
			o->LifeTime = 80;
			o->Scale = (GetLargeRand(20) * 0.01f) + 0.1f;
			o->Angle[0] = static_cast<mu_float>(GetLargeRand(360));

			o->Distance = GetLargeRand(10) + 5.0f;

			o->Timer = static_cast<mu_float>(GetLargeRand(360)); 
		}
		break;
	}

	if (o->Flags.GetAny(OBJECT::eOF_LIVE) == false)
	{
		i = INVALID_PARTICLE;

		g_EffectsDeleteCount.fetch_add(1);
	}
	else
	{
		o->BoneTransform = nullptr;
	}
}

template void CreateEffect<false>(const mu_uint32 ThreadIndex, mu_int32 Type, EVector3 Position, EVector3 Angle,
	EVector3 Light, mu_int32 SubType, OBJECT *Owner, mu_int16 PKKey, mu_uint16 SkillIndex, mu_uint16 Skill, mu_uint16 SkillSerialNum,
	mu_float Scale, mu_int16 sTargetIndex);
template void CreateEffect<true>(const mu_uint32 ThreadIndex, mu_int32 Type, EVector3 Position, EVector3 Angle,
	EVector3 Light, mu_int32 SubType, OBJECT *Owner, mu_int16 PKKey, mu_uint16 SkillIndex, mu_uint16 Skill, mu_uint16 SkillSerialNum,
	mu_float Scale, mu_int16 sTargetIndex);

void DeleteEffect(mu_int32 Type, OBJECT *Owner, mu_int32 SubType)
{
	mu_uint32 i = (mu_uint32)-1;

	g_EffectsEraseLock.Lock();

	if (g_EffectsEraseCount < MAX_EFFECTS_WAIT)
	{
		i = g_EffectsEraseCount++;
	}

	g_EffectsEraseLock.Unlock();

	if (i != (mu_uint32)-1)
	{
		EFFECT_DELETE &effect = g_EffectsErase[i];
		effect.Type = Type;
		effect.SubType = SubType;
		effect.Owner = Owner;
	}
}

mu_boolean SearchEffect(mu_int32 iType, OBJECT* pOwner, mu_int32 iSubType)
{
	MUAutoCriticalSpinLock lock(g_EffectsLock);

	for (mu_int32 i = 0; i < g_EffectsCurCount; ++i)
	{
		OBJECT *o = &g_Effects[i];
		if (o->Flags.GetAny(OBJECT::eOF_LIVE) == true &&
			o->Owner == pOwner &&
			o->Type == iType)
		{
			if (iSubType == -1 || o->SubType == iSubType)
			{
				return true;
			}
		}
	}

	return false;
}