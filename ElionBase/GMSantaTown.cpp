#include "stdafx.h"
#include "GMSantaTown.h"

CGMSantaTown g_SantaTown;

CGMSantaTown::CGMSantaTown()
{

}

CGMSantaTown::~CGMSantaTown()
{

}

mu_boolean CGMSantaTown::CreateObject(mu_uint32 objectIndex, OBJECT* o)
{
	switch(o->Type)
	{
	case 21: case 18:
	case 19: case 12: case 13:
	case 25:
		o->CollisionRange = -300;
		return true;
	}

	return false;
}

CHARACTER* CGMSantaTown::CreateMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key)
{
	CHARACTER* pCharacter = nullptr;

	switch (iType)
	{
	case 465:
		pCharacter = CreateCharacter(Key, MODEL_XMAS2008_SANTA_NPC, PosX, PosY);
		pCharacter->Object.Scale = 1.7f;
		break;
	case 467:
		pCharacter = CreateCharacter(Key, MODEL_XMAS2008_SNOWMAN_NPC, PosX, PosY);
		pCharacter->Object.Scale = 1.3f;
		break;
	}

	return pCharacter;
}

mu_boolean CGMSantaTown::MoveObject(OBJECT* o)
{
	switch(o->Type)
	{
	case 16:
		{
			o->Velocity = 0.06f;
			return true;
		}
		break;
	case 26:
	case 27:
	case 28:
		{
			o->HiddenMesh = -2;
			return true;
		}
		break;
	}

	return false;
}

mu_boolean CGMSantaTown::RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	switch (o->Type)
	{
	case 16:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(0.0f, 0.5f, 0.5f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 2, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			return true;
		}
	}

	return false;
}

mu_boolean CGMSantaTown::RenderObjectVisual( OBJECT* o, MUModel* b )
{
	switch(o->Type)
	{
	case 26:
		{
 			mu_float fLumi;
 			fLumi = (EMath::Sin(FWorldTime*0.005f) + 1.0f) * 0.1f + 0.9f;
			EVector3 vLightFire;
			Vector(fLumi*0.8f, fLumi*0.2f, fLumi*0.0f, vLightFire);
			CreateSprite(BITMAP_LIGHT, o->Position, 8.0f * o->Scale, vLightFire, o);
			return true;
		}
		break;
	case 27:
		{
 			mu_float fLumi;
 			fLumi = (EMath::Sin(FWorldTime*0.005f) + 1.0f) * 0.1f + 0.9f;
			EVector3 vLightFire;
			Vector(fLumi*0.0f, fLumi*0.2f, fLumi*0.8f, vLightFire);
			CreateSprite(BITMAP_LIGHT, o->Position, 8.0f * o->Scale, vLightFire, o);
			return true;
		}
		break;
	case 28:
		{
 			mu_float fLumi;
 			fLumi = (EMath::Sin(FWorldTime*0.005f) + 1.0f) * 0.1f + 0.9f;
			EVector3 vLightFire;
			Vector(fLumi*0.0f, fLumi*0.6f, fLumi*0.6f, vLightFire);
			CreateSprite(BITMAP_LIGHT, o->Position, 8.0f * o->Scale, vLightFire, o);
			return true;
		}
		break;
	}
	
	return false;
}

mu_boolean CGMSantaTown::CreateSnow( PARTICLE* o )
{
	if (IsSantaTown() == false)
		return false;

	o->Type = BITMAP_LEAF06;
	o->Scale = (mu_float)(GetLargeRand(10) + 5);
	if (GetLargeRand(10) == 0)
	{
		o->Type = BITMAP_LEAF08;
		o->Scale = 12.0f;
	}
	Vector(Hero->Object.Position[0]+(mu_float)(GetLargeRand(1600)-800),
		Hero->Object.Position[1]+(mu_float)(GetLargeRand(1400)-500),
		Hero->Object.Position[2]+(mu_float)(GetLargeRand(200)+200),
		o->Position);
	Vector(-20.0f,0.0f,0.0f,o->Angle);
	EVector3 Velocity;
	Vector(0.0f,0.0f,-(mu_float)(GetLargeRand(8)+4),Velocity);
	EBone Matrix;
	AngleMatrix(o->Angle,Matrix);
	VectorRotate(Velocity,Matrix,o->Velocity);

	return true;
}

void CGMSantaTown::PlayBGM()
{
	if (IsSantaTown())
	{
		EMusicBackend::Play(MUSICS::Data[MUSIC_SANTA_TOWN]);
	}
}