#include "stdafx.h"
#include "GMUnitedMarketPlace.h"

GMUnitedMarketPlace g_UnitedMarketPlace;

GMUnitedMarketPlace::GMUnitedMarketPlace()
{
}

GMUnitedMarketPlace::~GMUnitedMarketPlace()
{

}

mu_boolean GMUnitedMarketPlace::CreateObject(mu_uint32 objectIndex, OBJECT* o)
{
	switch(o->Type) 
	{
	case 67:
		{
			CreateOperate(objectIndex, o);
			Vector( 100.0f, 100.0f,160.0f,o->BoundingBoxMax);
			o->HiddenMesh = -2;
		}
		break;
	}

	return false;
}

mu_boolean GMUnitedMarketPlace::MoveObject(const mu_uint32 ThreadIndex, OBJECT* o)
{
	Alpha(o);
	if (o->Alpha < 0.01f) return false;

	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];
	MUModel *b = &MODELS::Data[o->Type];

	switch (o->Type)
	{
	case 8:
		{
			o->Velocity = 0.2f;
		}
		return true;
	case 30:
		{
			VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));
			b->BodyScale(ThreadIndex) = o->Scale;
			b->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle, false, true);

			EVector3	vLightPosition, vRelativePos;
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(1)], vRelativePos, vLightPosition, false);

			mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.2f + 0.6f;
			EVector3 vLightFire;
			Vector(fLumi*0.7f, fLumi*0.5f, fLumi*0.2f, vLightFire);

			AddTerrainLight(vLightPosition[0], vLightPosition[1], vLightFire, 3, PrimaryTerrainLight);
		}
		return true;
	case 35:
		{
			VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));
			b->BodyScale(ThreadIndex) = o->Scale;
			b->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle, false, true);

			EVector3	vLightPosition, vRelativePos;
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(2)], vRelativePos, vLightPosition, false);

			mu_float fLumi = (EMath::Sin(FWorldTime*0.039f) + 1.0f) * 0.2f + 0.6f;
			EVector3 vLightFire;
			Vector(fLumi*0.7f, fLumi*0.5f, fLumi*0.2f, vLightFire);

			AddTerrainLight(vLightPosition[0], vLightPosition[1], vLightFire, 1, PrimaryTerrainLight);
		}
		return true;
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
		{
			o->HiddenMesh = -2;
		}
		return true;
	}

	return false;
}

mu_boolean GMUnitedMarketPlace::RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	switch (o->Type)
	{
	case 8:
		{
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight);
			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight);
			mu_float fFlow_u = EMath::Sin(FWorldTime*0.0007f)*0.05f;
			mu_float fFlow_v = EMath::Sin(FWorldTime*0.001f)*0.05f;
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, 0.3f, o->BlendMeshTexCoordU, -IWorldTime % 10000 * 0.001f);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, 0.3f, fFlow_u, fFlow_v);
		}
		break;
	case 30:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		break;
	}

	return false;
}

mu_boolean GMUnitedMarketPlace::RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 p, Position, Light;
	Vector(0.0f, 30.0f, 0.0f, Position);
	Vector(0.0f, 0.0f, 0.0f, p);
	Vector(0.0f, 0.0f, 0.0f, Light);

	switch (o->Type)
	{
	case 30:
		{
			for (mu_int32 i = 1; i <= 3; ++i)
			{
				EVector3	vLightPosition, vRelativePos;
				Vector(0.0f, 0.0f, 0.0f, vRelativePos);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(i)], vRelativePos, vLightPosition, false);

				mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.2f + 0.6f;
				EVector3 vLightFire;
				Vector(fLumi*0.5f, fLumi*0.5f, fLumi*0.5f, vLightFire);
				CreateSprite(BITMAP_FLARE, vLightPosition, 1.5f * o->Scale, vLightFire, o);
			}
		}
		return true;
	case 35:
		{
			EVector3	vLightPosition, vRelativePos;
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(2)], vRelativePos, vLightPosition, false);

			mu_float fLumi = (EMath::Sin(FWorldTime*0.039f) + 1.0f) * 0.2f + 0.6f;
			EVector3 vLightFire;
			Vector(fLumi*0.7f, fLumi*0.7f, fLumi*0.7f, vLightFire);
			CreateSprite(BITMAP_FLARE, vLightPosition, 1.5f * o->Scale, vLightFire, o);
		}
		return true;
	case 54:
		{
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, o->Position, o->Angle, Light, 0);
		}
		return true;
	case 55:
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, o->Position, o->Angle, Light, 8, o->Scale);
		}
		return true;
	case 56:
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			if (GetLargeRand(8) == 0)
			{
				CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, o->Position, o->Angle, Light, 4, o->Scale);
			}
		}
		return true;
	case 57:
		{
			EVector3 vLightFire;
			Vector(1.0f, 0.2f, 0.0f, vLightFire);
			CreateSprite(BITMAP_LIGHT, o->Position, 2.0f * o->Scale, vLightFire, o);

			EVector3 vLight;
			Vector(1.0f, 1.0f, 1.0f, vLight);

			switch (GetLargeRand(3))
			{
			case 0:
				CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1, o->Position, o->Angle, vLight, 0, o->Scale);
				break;
			case 1:
				CreateParticle(ThreadIndex, BITMAP_FIRE_CURSEDLICH, o->Position, o->Angle, vLight, 4, o->Scale);
				break;
			case 2:
				CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3, o->Position, o->Angle, vLight, 0, o->Scale);
				break;
			}
		}
		return true;
	case 58:
		{
			mu_float fLumi;
			fLumi = (EMath::Sin(FWorldTime*0.005f) + 1.0f) * 0.3f + 0.5f;
			EVector3 vLightFire;
			Vector(fLumi*0.4f, fLumi*0.4f, fLumi*0.2f, vLightFire);
			CreateSprite(BITMAP_LIGHT, o->Position, 20.0f * o->Scale, vLightFire, o);
		}
		return true;
	}

	return false;
}

mu_boolean GMUnitedMarketPlace::RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	switch (o->Type)
	{
	case MODEL_UNITEDMARKETPLACE_CHRISTIN:
		{

		}
		return true;
	case MODEL_UNITEDMARKETPLACE_RAUL:
		{
			EVector3 vRelativePos, vWorldPos, Light, vLightPosition;
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);

			Vector(0.8f, 0.8f, 0.8f, Light);
			EVector3 vAngle;
			Vector(10.0f, 0.0f, 0.0f, vAngle);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(43)], vRelativePos, vWorldPos, true);
			CreateParticle(ThreadIndex, BITMAP_SMOKELINE1 + GetLargeRand(3), vWorldPos, o->Angle, Light, 1, 0.6f, o);
			CreateParticle(ThreadIndex, BITMAP_CLUD64, vWorldPos, o->Angle, Light, 6, 0.6f, o);

			Vector(1.0f, 1.0f, 1.0f, Light);
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(43)], vRelativePos, vLightPosition, true);
			CreateSprite(BITMAP_FLARE_RED, vLightPosition, 0.24f, Light, o);

			EVector3	v3Angle;
			Vector(0.0f, 0.0f, 0.0f, v3Angle);
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);
			Vector(1.0f, 0.7f, 0.3f, Light);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(79)], vRelativePos, vWorldPos, true);
			CreateSprite(BITMAP_LIGHT, vWorldPos, 0.6f, Light, o);
			CreateSprite(BITMAP_LIGHT, vWorldPos, 0.6f, Light, o);

			Vector(0.0f, 0.0f, 0.0f, vRelativePos);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(78)], vRelativePos, vLightPosition, true);

			mu_float fLumi = (EMath::Sin(FWorldTime*0.030f) + 1.0f) * 0.4f + 0.6f;
			EVector3 vLightFire;
			Vector(fLumi*1.0f, fLumi*0.7f, fLumi*0.3f, vLightFire);
			CreateSprite(BITMAP_FLARE, vLightPosition, 0.5f * o->Scale, vLightFire, o);

			Vector(1.0f, 1.0f, 1.0f, Light);
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(77)], vRelativePos, vLightPosition, true);
			CreateSprite(BITMAP_FLARE_RED, vLightPosition, 0.5f, Light, o);

			Vector(1.0f, 0.7f, 0.3f, Light);
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(76)], vRelativePos, vLightPosition, true);
			CreateSprite(BITMAP_FLARE, vLightPosition, 0.4f, Light, o);
		}
		return true;
	}

	return false;
}

mu_boolean GMUnitedMarketPlace::CreateRain(PARTICLE* o)
{
	if (IsUnitedMarketPlace() == false)
	{
		return false;
	}

	o->Type = BITMAP_RAIN;
	Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(1600) - 800), Hero->Object.Position[1] + (mu_float)(GetLargeRand(1400) - 500),
		Hero->Object.Position[2] + (mu_float)(GetLargeRand(200) + 200),
		o->Position);
	Vector(-30.0f, 0.0f, 0.0f, o->Angle);
	EVector3 Velocity;
	Vector(0.0f, 0.0f, -(mu_float)(GetLargeRand(24) + 20), Velocity);
	EBone Matrix;
	AngleMatrix(o->Angle, Matrix);
	VectorRotate(Velocity, Matrix, o->Velocity);

	return true;
}

mu_boolean GMUnitedMarketPlace::MoveRain(const mu_uint32 ThreadIndex, PARTICLE_HEADER *oh, PARTICLE* o)
{
	if (IsUnitedMarketPlace() == false)
	{
		return false;
	}

	if (o->Type == BITMAP_RAIN)
	{
		VectorAdd(o->Position, o->Velocity, o->Position);
		mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
		if (o->Position[2] < Height)
		{
			oh->Flags.Clear(PARTICLE::ePF_LIVE);
			o->Position[2] = Height + 10.0f;
			if (GetLargeRand(4) == 0)
				CreateParticle(ThreadIndex, BITMAP_RAIN_CIRCLE, o->Position, o->Angle, o->Light, 2);
			else
				CreateParticle(ThreadIndex, BITMAP_RAIN_CIRCLE + 1, o->Position, o->Angle, o->Light, 2);
		}
	}
	else
	{
		o->Velocity[0] += (mu_float)(GetLargeRand(16) - 8)*0.1f;
		o->Velocity[1] += (mu_float)(GetLargeRand(16) - 8)*0.1f;
		o->Velocity[2] += (mu_float)(GetLargeRand(16) - 8)*0.1f;
		VectorAdd(o->Position, o->Velocity, o->Position);

		o->TurningForce[0] += (mu_float)(GetLargeRand(8) - 4)*0.02f;
		o->TurningForce[1] += (mu_float)(GetLargeRand(16) - 8)*0.02f;
		o->TurningForce[2] += (mu_float)(GetLargeRand(8) - 4)*0.02f;
		VectorAdd(o->Angle, o->TurningForce, o->Angle);

		EVector3 Range;
		VectorSubtract(o->StartPosition, o->Position, Range);
		mu_float Length = Range[0] * Range[0] + Range[1] * Range[1] + Range[2] * Range[2];
		if (Length >= 200000.0f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
	}

	return true;
}