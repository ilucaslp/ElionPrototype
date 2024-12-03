#include "stdafx.h"
#include "mu_effects.h"

void CreateMagicShiny(const mu_uint32 ThreadIndex, CHARACTER *c, mu_int32 Hand)
{
	OBJECT *o = &c->Object;
	MUModel *b = &MODELS::Data[o->Type];
	EVector3 p, Position;
	Vector(0.0f, 0.0f, 0.0f, p);

	b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[Hand].LinkBone], p, Position, true);
	EVector3 Light;
	Vector(1.0f, 0.5f, 0.2f, Light);
	CreateParticle(ThreadIndex, BITMAP_SHINY + 1, Position, o->Angle, Light, Hand + 0, 0.0f, o);
	CreateParticle(ThreadIndex, BITMAP_SHINY + 1, Position, o->Angle, Light, Hand + 2, 0.0f, o);
}

void CreateTeleportBegin(const mu_uint32 ThreadIndex, OBJECT *o)
{
	SetAction(o, PLAYER_SKILL_TELEPORT);
	o->AlphaTarget = 0.0f;
	o->Teleport = TELEPORT_BEGIN;
	CreateEffect(ThreadIndex, BITMAP_SPARK + 1, o->Position, o->Angle, o->Light);
	PlayBackground(SOUND_MAGIC);
}

void CreateTeleportEnd(const mu_uint32 ThreadIndex, OBJECT *o)
{
	SetAction(o, PLAYER_SKILL_TELEPORT);
	o->AnimationFrame = 5.0f;
	o->Teleport = TELEPORT_END;
	o->AlphaTarget = 1.0f;
	CreateEffect(ThreadIndex, BITMAP_SPARK + 1, o->Position, o->Angle, o->Light);
	PlayBackground(SOUND_MAGIC);
}

void CreateArrow(const mu_uint32 ThreadIndex, CHARACTER *c, OBJECT *o, OBJECT *to,
	mu_uint16 SkillIndex, mu_uint16 Skill, mu_uint16 SKKey)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_int32 SubType = 0;
	mu_int32 Right = c->Weapon[0].Type;
	mu_int32 Left = c->Weapon[1].Type;

	if (SKKey == AT_SKILL_PIERCING)
	{
		SubType = 2;
		PlayObject(SOUND_BOW01, o);
	}
	ThreadInfo.CurrentSkill = SKKey;
	if (SKKey == AT_SKILL_PARALYZE)
	{
		PlayObject(SOUND_ICEARROW, o);
	}

	if (Skill == 2)
	{
		SubType = 99;
	}

	if (SKKey == AT_SKILL_DEEPIMPACT)
	{
		CreateEffect(ThreadIndex, MODEL_ARROW_IMPACT, o->Position, o->Angle, o->Light, 0, to, o->PKKey, SkillIndex, Skill);
	}
	else
	{
		switch (Right)
		{
		case MODEL_BOW + 8:CreateEffect(ThreadIndex, MODEL_ARROW_STEEL, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 9:CreateEffect(ThreadIndex, MODEL_ARROW_STEEL, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 10:CreateEffect(ThreadIndex, MODEL_ARROW_SAW, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 11:CreateEffect(ThreadIndex, MODEL_ARROW_LASER, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 12:CreateEffect(ThreadIndex, MODEL_ARROW_THUNDER, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 13:CreateEffect(ThreadIndex, MODEL_ARROW_WING, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 14:CreateEffect(ThreadIndex, MODEL_ARROW_BOMB, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 16:CreateEffect(ThreadIndex, MODEL_ARROW_DOUBLE, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 18:
			CreateEffect(ThreadIndex, MODEL_ARROW_BEST_CROSSBOW, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill);
			break;
		case MODEL_BOW + 19:CreateEffect(ThreadIndex, MODEL_ARROW_DRILL, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		}
		switch (Left)
		{
		case MODEL_BOW:CreateEffect(ThreadIndex, MODEL_ARROW, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 1:CreateEffect(ThreadIndex, MODEL_ARROW, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 2:CreateEffect(ThreadIndex, MODEL_ARROW_V, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 3:CreateEffect(ThreadIndex, MODEL_ARROW, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 4:CreateEffect(ThreadIndex, MODEL_ARROW, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 5:CreateEffect(ThreadIndex, MODEL_ARROW, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 6:CreateEffect(ThreadIndex, MODEL_ARROW_NATURE, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 17:CreateEffect(ThreadIndex, MODEL_ARROW_HOLY, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;    
		case MODEL_BOW + 20:CreateEffect(ThreadIndex, MODEL_LACEARROW, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 21:CreateEffect(ThreadIndex, MODEL_ARROW_SPARK, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 22:CreateEffect(ThreadIndex, MODEL_ARROW_RING, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 23:CreateEffect(ThreadIndex, MODEL_ARROW_DARKSTINGER, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		case MODEL_BOW + 24:CreateEffect(ThreadIndex, MODEL_ARROW_GAMBLE, o->Position, o->Angle, o->Light, SubType, o, o->PKKey, SkillIndex, Skill); break;
		}
	}
}

void CreateArrows(const mu_uint32 ThreadIndex, CHARACTER *c, OBJECT *o, OBJECT *to, mu_uint16 SkillIndex, mu_uint16 Skill, mu_uint16 SKKey)
{
	if (SKKey == AT_SKILL_PIERCING || SKKey == AT_SKILL_PARALYZE
		|| SKKey == AT_SKILL_DEEPIMPACT
		)
	{
		CreateArrow(ThreadIndex, c, o, to, SkillIndex, Skill, SKKey);
	}
	else
	{
		if (Skill == 1)
		{
			if (c->Weapon[0].Type == MODEL_BOW + 18
				|| c->Weapon[0].Type == MODEL_BOW + 19
				|| c->Weapon[1].Type == MODEL_BOW + 22
				|| c->Weapon[1].Type == MODEL_BOW + 23
				|| c->Weapon[1].Type == MODEL_BOW + 24
				)
			{
				o->Angle[2] += 5.0f;
				CreateArrow(ThreadIndex, c, o, to, SkillIndex, Skill, SKKey);
				o->Angle[2] += 10.0f;
				CreateArrow(ThreadIndex, c, o, to, SkillIndex, Skill, SKKey);
				o->Angle[2] -= 20.0f;
				CreateArrow(ThreadIndex, c, o, to, SkillIndex, Skill, SKKey);
				o->Angle[2] -= 10.0f;
				CreateArrow(ThreadIndex, c, o, to, SkillIndex, Skill, SKKey);
				o->Angle[2] += 30.0f;
			}
			else
			{
				CreateArrow(ThreadIndex, c, o, to, SkillIndex, Skill, SKKey);
				o->Angle[2] += 15.0f;
				CreateArrow(ThreadIndex, c, o, to, SkillIndex, Skill, SKKey);
				o->Angle[2] -= 30.0f;
				CreateArrow(ThreadIndex, c, o, to, SkillIndex, Skill, SKKey);
				o->Angle[2] += 15.0f;
			}
		}
		else
		{
			CreateArrow(ThreadIndex, c, o, to, SkillIndex, Skill, SKKey);
		}

		if (c->Weapon[0].Type == MODEL_BOW + 19)
		{
			EVector3 p, Position, Light;
			MUModel *b = &MODELS::Data[o->Type];

			Vector(1.0f, 1.0f, 1.0f, Light);

			Vector(0.0f, 10.0f, -130.0f, p);
			b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[0].LinkBone], p, Position, true);

			EVector3 a;
			for (mu_int32 i = 0; i<15; ++i)
			{
				Vector((mu_float)(GetLargeRand(360)), 0.0f, 0.0f, a);
				if ((GetLargeRand(2)) == 0)
				{
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, Position, Position, a, 3);
				}
				CreateParticle(ThreadIndex, BITMAP_SPARK, Position, a, Light, 0);
			}
		}
	}
}

void CreateSpark(const mu_uint32 ThreadIndex, mu_int32 Type, CHARACTER *tc, EVector3 Position, EVector3 Angle)
{
	OBJECT *to = &tc->Object;
	EVector3 Light;
	Vector(1.0f, 1.0f, 1.0f, Light);
	CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, to->Angle, Light);
	EVector3 p, p2;
	EBone Matrix;
	Vector(0.0f, 50.0f, 0.0f, p);
	AngleMatrix(Angle, Matrix);
	VectorRotate(p, Matrix, p2);
	VectorAdd(p2, Position, p2);
	for (mu_int32 i = 0; i < 20; ++i)
	{
		EVector3 a;
		Vector((mu_float)(GetLargeRand(360)), 0.0f, (mu_float)(GetLargeRand(360)), a);
		VectorAdd(a, Angle, a);
		CreateParticle(ThreadIndex, BITMAP_SPARK, Position, to->Angle, Light);
	}
}

void CreateBlood(const mu_uint32 ThreadIndex, OBJECT *o)
{
	mu_int32 BoneHead = MODELS::Data[o->Type]._BoneHead;
	if (BoneHead != -1)
	{
		if (o->Type == MODEL_MONSTER01 + 15)
		{
			o->Flags.Clear(OBJECT::eOF_LIVE);
			for (mu_int32 i = 0; i < 10; ++i)
				CreateEffect(ThreadIndex, MODEL_ICE_SMALL, o->Position, o->Angle, o->Light);
		}
		else if (o->Type != MODEL_MONSTER01 + 7 && o->Type != MODEL_MONSTER01 + 14 && o->Type != MODEL_MONSTER01 + 18)
		{
			EVector3 p, Position;
			for (mu_int32 i = 0; i < 2; ++i)
			{
				Vector((mu_float)(GetLargeRand(100) - 50), (mu_float)(GetLargeRand(100) - 50), 0.0f, p);
				MUModel *b = &MODELS::Data[o->Type];
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(BoneHead)], p, Position, true);
				CreatePointer(BITMAP_BLOOD, Position, (mu_float)(GetLargeRand(360)), o->Light, (mu_float)(GetLargeRand(4) + 8)*0.1f);
			}
		}
	}
}

void CreateForce(const mu_uint32 ThreadIndex, OBJECT* o, EVector3 &Pos)
{
	EVector3 Angle, p, Position;
	EBone Matrix;
	Vector(0.0f, -500.0f, 0.0f, p);
	for (mu_int32 j = 0; j < 3; ++j)
	{
		Vector((mu_float)(GetLargeRand(90)), 0.0f, (mu_float)(GetLargeRand(360)), Angle);
		AngleMatrix(Angle, Matrix);
		VectorRotate(p, Matrix, Position);
		VectorSubtract(Pos, Position, Position);
		Position[2] += 120.0f;
		CreateJoint(ThreadIndex, BITMAP_JOINT_HEALING, Position, Pos, Angle, 8, o, 10.0f);
	}
}

void CreateHealing(const mu_uint32 ThreadIndex, OBJECT *o)
{
	EVector3 Angle, p, Position;
	EBone Matrix;
	Vector(0.0f, -200.0f, 0.0f, p);
	for (mu_int32 j = 0; j < 3; ++j)
	{
		Vector((mu_float)(GetLargeRand(90)), 0.0f, (mu_float)(GetLargeRand(360)), Angle);
		AngleMatrix(Angle, Matrix);
		VectorRotate(p, Matrix, Position);
		VectorSubtract(o->Position, Position, Position);
		Position[2] += 120.0f;
		if (o->SubType == 5)
		{
			CreateJoint(ThreadIndex, BITMAP_JOINT_HEALING, Position, o->Position, Angle, 11, o->Owner, 5.0f);

		}
		else
		{
			CreateJoint(ThreadIndex, BITMAP_JOINT_HEALING, Position, o->Position, Angle, o->SubType, o->Owner, 5.0f);
		}
	}
}