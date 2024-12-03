#include "stdafx.h"
#include "GMHellas.h"

MUWaterTerrain* g_pCSWaterTerrain = nullptr;

typedef struct
{
	mu_text m_strName[64];
	EVector3 m_vPos;
}ObjectDescript;
std::queue<ObjectDescript> g_qObjDes;
MUCriticalSectionSpinLock g_qObjDesLock;

const mu_uint8 ACTION_DESTROY_PHY_DEF = 33;
const mu_uint8 ACTION_DESTROY_DEF = 34;

const mu_int32 g_iKalimaLevel[14][2] = { { 40, 999 }, { 131, 999 }, { 181, 999 }, { 231, 999 }, { 281, 999 }, { 331, 999 }, { 350, 999 },
                                         { 20, 999 }, { 111, 999 }, { 161, 999 }, { 211, 999 }, { 261, 999 }, { 311, 999 }, { 350, 999 } };

mu_boolean IsWaterTerrain()
{
	if (g_pCSWaterTerrain != nullptr)
	{
		return true;
	}
	return false;
}

void AddWaterWave(mu_int32 x, mu_int32 y, mu_int32 range, mu_int32 height)
{
	if (g_pCSWaterTerrain != nullptr)
	{
		mu_int32 WaveX = (x * 2);
		mu_int32 WaveY = (y * 2);
		g_pCSWaterTerrain->addSineWave(WaveX, WaveY, range, range, height);
	}
}

void DeleteWaterTerrain()
{
	if (g_pCSWaterTerrain != nullptr)
	{
		delete g_pCSWaterTerrain;
		g_pCSWaterTerrain = nullptr;
	}
}

mu_float GetWaterTerrain(mu_float x, mu_float y)
{
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (g_pCSWaterTerrain != nullptr)
	{
		return g_pCSWaterTerrain->GetWaterTerrain(x, y);
	}
	return 0.0f;
}

void  RenderWaterTerrain(mu_int32 Texture, mu_float xf, mu_float yf, mu_float SizeX, mu_float SizeY, EVector3 Light, mu_float Rotation, mu_float Alpha, mu_float Height)
{
#if 0 // FIX
	if (g_pCSWaterTerrain != nullptr)
	{
		g_pCSWaterTerrain->RenderWaterAlphaBitmap(Texture, xf, yf, SizeX, SizeY, Light, Rotation, Alpha, Height);
	}
#endif
}

mu_uint8 GetHellasLevel(mu_int32 Class, mu_int32 Level)
{
	mu_int32 startIndex = 0;
	if (GetBaseClass(Class) == GAME::CLASS_DARK || GetBaseClass(Class) == GAME::CLASS_DARK_LORD)
	{
		startIndex = NUM_HELLAS;
	}

	mu_int32 byLevel = 0;
	for (mu_int32 i = 0; i < NUM_HELLAS; ++i)
	{
		++byLevel;
		if (Level >= g_iKalimaLevel[startIndex + i][0] && Level <= g_iKalimaLevel[startIndex + i][1])
		{
			break;
		}
	}
	return byLevel;
}

mu_boolean EnableKalima(mu_int32 Class, mu_int32 Level, mu_int32 ItemLevel)
{
    mu_int32 startIndex = 0;

    if(GetBaseClass( Class ) == GAME::CLASS_DARK || GetBaseClass( Class ) == GAME::CLASS_DARK_LORD)
    {
        startIndex = NUM_HELLAS;
    }
	
	if ( Level < g_iKalimaLevel[startIndex+ItemLevel-1][0] )
	{
		return false;
	}

	return true;
}

mu_boolean GetUseLostMap(mu_boolean bDrawAlert)
{
	mu_int32 Level = CharacterAttribute->Level;

	mu_int32 startIndex = 0;
	if (GetBaseClass(Hero->Class) == GAME::CLASS_DARK || GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD)
	{
		startIndex = NUM_HELLAS;
	}

	if (bDrawAlert && Hero->SafeZone)
	{
		g_pChatListBox->AddText(_T(""), GlobalText[1238], TYPE_ERROR_MESSAGE);
		return false;
	}

	if (Level >= g_iKalimaLevel[startIndex][0])
	{
		return true;
	}

	if (bDrawAlert)
	{
		mu_text Text[100];
		mu_sprintf_s(Text, mu_countof(Text), GlobalText[1123], g_iKalimaLevel[startIndex][0]);
		g_pChatListBox->AddText(_T(""), Text, TYPE_ERROR_MESSAGE);
	}

	return false;
}

mu_int32 RenderHellasItemInfo(ITEM* ip, mu_int32 textNum)
{
	mu_int32 TextNum = textNum;

	switch (ip->Type)
	{
	case ITEM_POTION + 28:
		{
			mu_int32 startIndex = 0;
			if (GetBaseClass(Hero->Class) == GAME::CLASS_DARK || GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD)
			{
				startIndex = NUM_HELLAS;
			}

			mu_int32 HeroLevel = CharacterAttribute->Level;
			mu_int32 ItemLevel = ip->Level;

			TextListColor[TextNum] = TEXT_COLOR_WHITE;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T("       ") SPF_STRING _T("    "), GlobalText[58], GlobalText[368], GlobalText[935]); TextListColor[TextNum] = TEXT_COLOR_WHITE; TextBold[TextNum] = false; ++TextNum;
			for (mu_int32 i = 0; i < NUM_HELLAS; ++i)
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("        %d             %3d~%3d     "), i + 1, g_iKalimaLevel[startIndex + i][0], EMath::Min(400, g_iKalimaLevel[startIndex + i][1]));

				if (ItemLevel == i + 1)
				{
					TextListColor[TextNum] = TEXT_COLOR_DARKYELLOW;
				}
				else
				{
					TextListColor[TextNum] = TEXT_COLOR_WHITE;
				}
				TextBold[TextNum] = false; ++TextNum;
			}

			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum;
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1184]);  TextListColor[TextNum] = TEXT_COLOR_DARKBLUE; ++TextNum;

			if (HeroLevel < g_iKalimaLevel[startIndex][0])
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); ++TextNum;
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1123], g_iKalimaLevel[startIndex][0]);  TextListColor[TextNum] = TEXT_COLOR_DARKRED; ++TextNum;
			}
		}
		break;

	case ITEM_POTION + 29:
		{
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1181], ip->Durability, 5); ++TextNum;
			if (ip->Durability >= 5)
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1182]); TextListColor[TextNum] = TEXT_COLOR_YELLOW; ++TextNum;
			}
			else
			{
				mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[1183], (5 - ip->Durability)); TextListColor[TextNum] = TEXT_COLOR_YELLOW; ++TextNum;
			}
		}
		break;
	}

	return TextNum;
}

void    AddObjectDescription(mu_text* Text, EVector3 position)
{
	MUAutoCriticalSpinLock lock(g_qObjDesLock);
	ObjectDescript QD;

	mu_strcpy(QD.m_strName, mu_countof(QD.m_strName), Text);
	VectorCopy(position, QD.m_vPos);

	g_qObjDes.push(QD);
}

void RenderObjectDescription()
{
	MUAutoCriticalSpinLock lock(g_qObjDesLock);
#if 0 // FIX
	glColor3f(1.0f, 1.0f, 1.0f);
	while (!g_qObjDes.empty())
	{
		ObjectDescript QD = g_qObjDes.front();

		mu_int32    x, y;
		EVector3 Position;
		Vector(QD.m_vPos[0], QD.m_vPos[1], QD.m_vPos[2] + 370.0f, Position);
		Projection(Position, &x, &y);

		if (x >= 0 && y >= 0)
		{
			g_pRenderText->SetFont(g_hFontBold);
			g_pRenderText->SetTextColor(255, 230, 200, 255);
			g_pRenderText->SetBgColor(100, 0, 0, 255);
			g_pRenderText->RenderText(x, y, QD.m_strName, 0, 0, RT3_WRITE_CENTER);
		}

		g_qObjDes.pop();
	}
#endif
}

mu_boolean MoveHellasVisual(OBJECT* o)
{
	if (InHellas() == false) return false;

	switch (o->Type)
	{
	case 37:
	case 38:
	case 39:
	case 40:
		o->HiddenMesh = -2;
		break;
	}

	return true;
}

mu_boolean RenderHellasVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	if (InHellas() == false) return false;

	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 p, Position;
	EVector3 Light;
	mu_float  Luminosity = 0.0f;

	switch (o->Type)
	{
	case 12:
		Luminosity = EMath::Sin(FWorldTime*0.001f)*0.3f + 0.7f;

		Vector(0.6f, 0.6f, 1.0f, Light);
		Vector(0.0f, 0.0f, 0.0f, p);
		b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(5)], p, Position);
		CreateSprite(BITMAP_LIGHT, Position, Luminosity + 0.2f, Light, o);
		break;
	case 15:
	case 29:
		CheckGrass(o);
		o->Position[2] = GetWaterTerrain(o->Position[0], o->Position[1]) + 180;
		break;
	case 32:
		CheckGrass(o);
		Luminosity = EMath::Sin(FWorldTime*0.001f)*0.3f + 0.7f;

		Vector(0.6f, 0.6f, 1.0f, Light);
		Vector(0.0f, 0.0f, 0.0f, p);
		b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(5)], p, Position);
		CreateSprite(BITMAP_LIGHT, Position, Luminosity + 0.2f, Light, o);
		o->Position[2] = GetWaterTerrain(o->Position[0], o->Position[1]) + 180;
		break;
	case 35:
		Vector(0.3f, 0.6f, 1.0f, Light);
		CreateParticle(ThreadIndex, BITMAP_LIGHT, o->Position, o->Angle, Light, 6, 1.0f, o);
		o->HiddenMesh = -2;
		break;
	case 36:
		Vector(1.0f, 1.0f, 1.0f, Light);
		CreateParticle(ThreadIndex, BITMAP_TRUE_BLUE, o->Position, o->Angle, Light, 0);

		o->Scale = 0.5f;
		o->HiddenMesh = -2;
		break;

	case 37:
		Vector(1.0f, 1.0f, 1.0f, Light);
		CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, o->Position, o->Angle, Light, 0);
		o->Scale = 0.5f;
		PlayBackground(SOUND_KALIMA_WATER_FALL);
		break;
	case 38:
		Vector(1.0f, 1.0f, 1.0f, Light);
		if (GetLargeRand(2) == 0)
		{
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_1, o->Position, o->Angle, Light, 0);
		}
		PlayBackground(SOUND_KALIMA_WATER_FALL);
		o->Scale = 0.5f;
		break;
	case 39:
		Vector(1.0f, 1.0f, 1.0f, Light);
		CreateParticle(ThreadIndex, BITMAP_WATERFALL_3 + (GetLargeRand(2)), o->Position, o->Angle, Light, 0);
		o->Scale = 0.5f;
		break;
	case 40:
		Vector(1.0f, 1.0f, 1.0f, Light);
		if (GetLargeRand(4) == 0)
		{
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, o->Position, o->Angle, Light, 0);
		}
		o->Scale = 0.5f;
		break;
	}

	return true;
}

mu_boolean RenderHellasObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (o->Type == MODEL_MONSTER01 + 33 && InHellas())
	{
		Vector(0.0f, 0.0f, 0.0f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		return true;
	}
	else if (o->Type == MODEL_WARCRAFT)
	{
		if (o->SubType == 1)
		{
			Vector(1.0f, 0.1f, 0.1f, b->BodyLight(ThreadIndex));
		}
		else
		{
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		}
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		return true;
	}
	else if (o->Type == MODEL_CUNDUN_DRAGON_HEAD)
	{
		Vector(0.3f, 0.3f, 0.3f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		if(MU_UPDATECOUNTER > 0)
		{
			mu_float Luminosity = (mu_float)EMath::Sin(FWorldTime*0.003f)*0.2f + 0.8f;
			EVector3 p, Light, Position;

			Vector(0, 0, 0, p);
			Vector(Luminosity*1.0f, Luminosity*0.2f, Luminosity*0.1f, Light);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(3)], p, Position, false);
			CreateSprite(BITMAP_ENERGY, Position, 0.2f, Light, o, 0.0f);
			Vector(Luminosity*1.0f, Luminosity*0.2f, Luminosity*0.1f, Light);
			CreateSprite(BITMAP_SHINY + 1, Position, 0.5f, Light, o, 0.0f);

			Vector(Luminosity*1.0f, Luminosity*0.2f, Luminosity*0.1f, Light);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(4)], p, Position, false);
			CreateSprite(BITMAP_ENERGY, Position, 0.2f, Light, o, 0.0f);
			Vector(Luminosity*1.0f, Luminosity*0.2f, Luminosity*0.1f, Light);
			CreateSprite(BITMAP_SHINY + 1, Position, 0.5f, Light, o, 0.0f);
		}

		return true;
	}
	else if (o->Type == MODEL_CUNDUN_GHOST)
	{
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		if (o->AnimationFrame > 3 && o->Alpha > 0.2f)
			o->Alpha -= 0.02f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_DARK, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_DARK, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		Vector(0.0f, 0.0f, 0.0f, b->BodyLight(ThreadIndex));
		RenderPartObjectEdge(ThreadIndex, b, o, RENDER_COLOR | RENDER_PREBLEND, true, 0.7f);

		if(MU_UPDATECOUNTER > 0)
		{
			mu_float Luminosity = (mu_float)EMath::Sin(FWorldTime*0.003f)*0.2f + 0.8f;
			EVector3 p, Light, Position;

			Vector(0, 0, 0, p);
			Vector(Luminosity*1.0f, Luminosity*0.2f, Luminosity*0.1f, Light);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(8)], p, Position, false);
			CreateSprite(BITMAP_ENERGY, Position, 0.2f, Light, o, 0.0f);
			Vector(Luminosity*1.0f, Luminosity*0.2f, Luminosity*0.1f, Light);
			CreateSprite(BITMAP_SHINY + 1, Position, 0.5f, Light, o, 0.0f);

			Vector(Luminosity*1.0f, Luminosity*0.2f, Luminosity*0.1f, Light);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(9)], p, Position, false);
			CreateSprite(BITMAP_ENERGY, Position, 0.2f, Light, o, 0.0f);
			Vector(Luminosity*1.0f, Luminosity*0.2f, Luminosity*0.1f, Light);
			CreateSprite(BITMAP_SHINY + 1, Position, 0.5f, Light, o, 0.0f);

			Vector(2.0f, 10.0f, 0.0f, p);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(6)], p, Position, false);
			Vector(1.0f, 0.2f, 0.0f, Light);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 17, 3.0f);
		}
		return true;
	}
	else if (InHellas() == true && (o->Type >= MODEL_WORLD_OBJECT && o->Type < MAX_WORLD_OBJECTS))
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		if (o->Type >= 0 && o->Type <= 66)
		{
			if (o->Type != 2 && o->Type != 4 && o->Type != 12 && o->Type != 14 && o->Type != 15 && o->Type != 18 && o->Type != 20 && o->Type != 21 && o->Type != 27 && o->Type != 29 && o->Type != 30 &&
				o->Type != 31 && o->Type != 32 && o->Type != 41 && o->Type != 43 && o->Type != 52 && o->Type != 54 && o->Type != 55)
			{
				mu_float Luminosity = EMath::Sin(FWorldTime*0.002f)*0.1f + 0.3f;
				Vector(Luminosity, Luminosity, Luminosity, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, 0.3f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_WATER + WaterTextureNumber);
			}

			if (o->Type == 34)
			{
				if (b->_MeshCount > 1)
				{
					b->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				}
			}

			if (o->Type == 15 || o->Type == 29 || o->Type == 32)
			{
				Vector(0.1f, 0.1f, 0.1f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_SHADOWMAP, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			}
		}
		return true;
	}
	else
	{
		return RenderHellasMonsterObjectMesh(ThreadIndex, o, b);
	}

	return false;
}

mu_int32 CreateBigMon(OBJECT* o)
{
	if (InHellas() == false) return 0;

	if ((MoveSceneFrame % 100) == 0)
	{
		o->Flags.Clear();
		o->Flags.Set(OBJECT::eOF_LIVE | OBJECT::eOF_LIGHTENABLE);

		o->Type = MODEL_MONSTER01 + 33;
		o->Scale = 2.5f + (mu_float)(GetLargeRand(3) + 6)*0.05f;
		o->Alpha = 1.0f;
		o->AlphaTarget = o->Alpha;
		o->Velocity = (mu_float)(GetLargeRand(10) + 10)*0.04f;
		o->Gravity = GetLargeRand(3) - 1.5f;
		o->SubType = 0;
		o->HiddenMesh = 5;
		o->BlendMesh = -1;
		o->LifeTime = 200;
		o->CurrentAction = MONSTER01_WALK;
		SetAction(o, o->CurrentAction);
		Vector(0.0f, 0.0f, 90.0f - GetLargeRand(30) - 15, o->Angle);
		Vector(Hero->Object.Position[0] - 1000 - GetLargeRand(200),
			Hero->Object.Position[1] - 500 + GetLargeRand(200),
			Hero->Object.Position[2] - 800.0f, o->Position);
	}
	else
	{
		o->Flags.Clear(OBJECT::eOF_LIVE);
	}
	return 1;
}

void MoveBigMon(OBJECT* o)
{
	o->Angle[2] += o->Gravity;

	if (GetLargeRand(5) == 0)
	{
		o->Gravity *= -1;
	}

	if (o->LifeTime < 20)
	{
		o->Alpha /= 1.2f;
		o->Velocity += 0.5f;
		o->Angle[0] += 2.0f;
	}

	if (o->LifeTime < 0)
	{
		o->Flags.Clear(OBJECT::eOF_LIVE);
	}
}

void CreateMonsterSkill_ReduceDef(const mu_uint32 ThreadIndex, OBJECT* o, mu_int32 AttackTime, mu_uint8 time, mu_float Height)
{
	if (AttackTime >= time)
	{
		EVector3 Angle, Light, Position, p;

		Vector(0.0f, 0.0f, 0.0f, Light);
		Vector(0.0f, 0.0f, 0.0f, p);
		VectorCopy(o->Position, Position);

		Position[2] += Height;
		for (mu_int32 i = 0; i < 3; ++i)
		{
			Vector(0.0f, 0.0f, i*120.0f, Angle);
			CreateEffect(ThreadIndex, MODEL_SKULL, Position, Angle, Light, 1, o);
		}

		PlayBackground(SOUND_SKILL_SKULL);
	}
}

void CreateMonsterSkill_Poison(const mu_uint32 ThreadIndex, OBJECT* o, mu_int32 AttackTime, mu_uint8 time)
{
	if (AttackTime >= time)
	{
		EBone Matrix;
		EVector3 Angle, Light, Position, p;

		Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
		Vector(0.0f, 300.0f, 0.0f, p);
		Vector(0.8f, 0.5f, 0.1f, Light);
		for (mu_int32 i = 0; i < 5; ++i)
		{
			Angle[2] += 72.0f;
			AngleMatrix(Angle, Matrix);
			VectorRotate(p, Matrix, Position);
			VectorAdd(o->Position, Position, Position);

			CreateEffect(ThreadIndex, MODEL_FIRE, Position, o->Angle, Light, 8, nullptr, 0);
		}

		PlayBackground(SOUND_GREAT_POISON);
	}
}

void CreateMonsterSkill_Summon(const mu_uint32 ThreadIndex, OBJECT* o, mu_int32 AttackTime, mu_uint8 time)
{
	if (AttackTime >= time)
	{
		CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, o->Light, 3, o);
		CreateEffect(ThreadIndex, MODEL_CIRCLE_LIGHT, o->Position, o->Angle, o->Light, 4);
	}
}

void SetActionDestroy_Def(OBJECT* o)
{
	if (o->Type != MODEL_PLAYER)
	{
		if (g_isCharacterBuff(o, eBuff_PhysDefense))
		{
			o->AI = ACTION_DESTROY_PHY_DEF;
			g_CharacterUnRegisterBuff(o, eBuff_PhysDefense);
		}
		else if (g_isCharacterBuff(o, eBuff_Defense))
		{
			o->AI = ACTION_DESTROY_DEF;
			g_CharacterUnRegisterBuff(o, eBuff_Defense);
		}
	}
}

void RenderDestroy_Def(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (o->Type != MODEL_PLAYER)
	{
		if (o->AI == ACTION_DESTROY_PHY_DEF)
		{
			b->RenderMeshEffect(ThreadIndex, o->BoneTransform, 6, MODEL_STONE_COFFIN, 1);
			o->AI = 0;

			PlayBackground(SOUND_HIT_CRISTAL);
		}
		else if (o->AI == ACTION_DESTROY_DEF)
		{
			b->RenderMeshEffect(ThreadIndex, o->BoneTransform, 6, MODEL_STONE_COFFIN, 2);
			o->AI = 0;

			PlayBackground(SOUND_HIT_CRISTAL);
		}
	}
}

CHARACTER* CreateHellasMonster(mu_int32 Type, mu_int32 PositionX, mu_int32 PositionY, mu_int32 Key)
{
	CHARACTER* c = nullptr;
	OBJECT* o = nullptr;

	switch (Type)
	{
	case 144:
	case 174:
	case 182:
	case 190:
	case 260:
	case 268:
	case 334:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 63, PositionX, PositionY);
		c->Weapon[0].Type = -1;
		c->Weapon[0].Level = 0;
		c->Object.Scale = 1.2f;
		o = &c->Object;
		o->BlendMesh = 1;
		break;
	case 145:
	case 175:
	case 183:
	case 191:
	case 261:
	case 269:
	case 336:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 67, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_SPEAR + 10;
		c->Weapon[0].Level = 7;
		c->Object.Scale = 1.5f;
		o = &c->Object;
		o->SubType = 9;
		o->BlendMesh = 0;
		break;
	case 146:
	case 176:
	case 184:
	case 192:
	case 262:
	case 270:
	case 333:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 65, PositionX, PositionY);
		c->Weapon[0].Type = -1;
		c->Weapon[0].Level = 0;
		c->Object.Scale = 0.8f;
		o = &c->Object;
		break;
	case 147:
	case 177:
	case 185:
	case 193:
	case 263:
	case 271:
	case 331:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 66, PositionX, PositionY);
		c->Weapon[0].Type = -1;
		c->Weapon[0].Level = 0;
		c->Object.Scale = 1.4f;
		o = &c->Object;
		o->BlendMesh = 1;
		break;
	case 148:
	case 178:
	case 186:
	case 194:
	case 264:
	case 272:
	case 332:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 67, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_SPEAR + 10;
		c->Weapon[0].Level = 7;
		c->Object.Scale = 1.0f;
		o = &c->Object;
		o->BlendMesh = 0;
		break;
	case 149:
	case 179:
	case 187:
	case 195:
	case 265:
	case 273:
	case 335:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 68, PositionX, PositionY);
		c->Weapon[0].Type = -1;
		c->Weapon[0].Level = 7;
		c->Object.Scale = 1.2f;
		o = &c->Object;
		o->BlendMesh = 3;
		break;
	case 160:
	case 180:
	case 188:
	case 196:
	case 266:
	case 274:
	case 337:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 69, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_SWORD + 13;
		c->Weapon[0].Level = 0;
		c->Weapon[1].Type = MODEL_SWORD + 13;
		c->Weapon[1].Level = 0;
		c->Object.Scale = 1.2f;
		o = &c->Object;
		break;
	case 161:
	case 181:
	case 189:
	case 197:
	case 267:
	case 338:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 69, PositionX, PositionY);
		c->Weapon[0].Type = MODEL_SWORD + 13;
		c->Weapon[0].Level = 0;
		c->Weapon[1].Type = MODEL_SWORD + 13;
		c->Weapon[1].Level = 0;
		o = &c->Object;
		o->SubType = 9;
		o->Scale = 1.5f;
		break;

	case 275:
		c = CreateCharacter(Key, MODEL_MONSTER01 + 64, PositionX, PositionY);
		c->Weapon[1].Type = MODEL_STAFF + 11;
		c->Weapon[1].Level = 0;
		c->Object.Scale = 2.0f;
		o = &c->Object;
		o->LifeTime = 100;
		break;
	}

	return c;
}

mu_boolean SettingHellasMonsterLinkBone(CHARACTER* c, mu_int32 Type)
{
	const MUModel *b = &MODELS::Data[Type];
	switch (Type)
	{
	case MODEL_MONSTER01 + 64:
		c->Weapon[0].LinkBone = b->GetBoneIndex(29);
		c->Weapon[1].LinkBone = b->GetBoneIndex(49);
		return true;
	case MODEL_MONSTER01 + 66:
		c->Weapon[0].LinkBone = b->GetBoneIndex(13);
		c->Weapon[1].LinkBone = b->GetBoneIndex(14);
		return true;
	case MODEL_MONSTER01 + 67:
		c->Weapon[0].LinkBone = b->GetBoneIndex(56);
		c->Weapon[1].LinkBone = b->GetBoneIndex(42);
		return true;
	case MODEL_MONSTER01 + 68:
		c->Weapon[0].LinkBone = b->GetBoneIndex(60);
		c->Weapon[1].LinkBone = b->GetBoneIndex(60);
		return true;
	case MODEL_MONSTER01 + 69:
		c->Weapon[0].LinkBone = b->GetBoneIndex(41);
		c->Weapon[1].LinkBone = b->GetBoneIndex(51);
		return true;
	}

	return false;
}

mu_boolean SetCurrentAction_HellasMonster(CHARACTER* c, OBJECT* o)
{
	switch (c->MonsterIndex)
	{
	case 145:
	case 175:
	case 183:
	case 191:
	case 261:
	case 269:
		switch ((c->Skill))
		{
		case AT_SKILL_ENERGYBALL:
		case AT_SKILL_BLOOD_ATT_UP:
		case AT_SKILL_BLOOD_ATT_UP + 1:
		case AT_SKILL_BLOOD_ATT_UP + 2:
		case AT_SKILL_BLOOD_ATT_UP + 3:
		case AT_SKILL_BLOOD_ATT_UP + 4:
		case AT_SKILL_REDUCEDEFENSE:
		case AT_SKILL_POISON:
		case AT_SKILL_MONSTER_SUMMON:
		case AT_SKILL_MONSTER_MAGIC_DEF:
		case AT_SKILL_MONSTER_PHY_DEF:
			SetAction(o, MONSTER01_ATTACK2);
			break;

		default:
			SetAction(o, MONSTER01_ATTACK1);
			break;
		}
		return true;

	case 147:
	case 177:
	case 185:
	case 193:
	case 263:
	case 271:
		switch ((c->Skill))
		{
		case AT_SKILL_ENERGYBALL:
			SetAction(o, MONSTER01_ATTACK2);
			break;

		default:
			SetAction(o, MONSTER01_ATTACK1);
			break;
		}
		return true;

	case 148:
	case 178:
	case 186:
	case 194:
	case 264:
	case 272:
		switch ((c->Skill))
		{
		case AT_SKILL_ENERGYBALL:
			SetAction(o, MONSTER01_ATTACK2);
			break;

		default:
			SetAction(o, MONSTER01_ATTACK1);
			break;
		}
		return true;

	case 149:
	case 179:
	case 187:
	case 195:
	case 265:
	case 273:
		switch ((c->Skill))
		{
		case AT_SKILL_POISON:
			SetAction(o, MONSTER01_ATTACK2);
			break;

		case AT_SKILL_ENERGYBALL:
			SetAction(o, MONSTER01_ATTACK1);
			break;
		}
		return true;

	case 160:
	case 180:
	case 188:
	case 196:
	case 266:
	case 274:
	case 161:
	case 181:
	case 189:
	case 197:
	case 267:
		SetAction(o, MONSTER01_ATTACK1 + GetLargeRand(2));
		return true;

	case 275:
		SetAction(o, MONSTER01_ATTACK1 + GetLargeRand(2));
		return true;
	}
	return false;
}

mu_boolean AttackEffect_HellasMonster(const mu_uint32 ThreadIndex, CHARACTER* c, CHARACTER* tc, OBJECT* o, OBJECT* to, MUModel* b)
{
	EVector3 Light;
	EVector3 p, Position;
	Vector(0.0f, 0.0f, 0.0f, p);
	Vector(1.0f, 1.0f, 1.0f, Light);
	switch (c->MonsterIndex)
	{
	case 144:
	case 174:
	case 182:
	case 190:
	case 260:
	case 268:
	case 276:
		if (c->AttackTime == 14)
		{
			Vector(1.0f, 1.0f, 1.0f, Light);

			if (to != nullptr)
			{
				VectorCopy(to->Position, Position);
			}
			else
			{
				VectorCopy(Hero->Object.Position, Position);
			}
			Position[2] += 150.0f;
			CreateParticle(ThreadIndex, BITMAP_SHINY + 4, Position, o->Angle, Light, 1, 1.0f);
		}
		return true;

	case 145:
	case 175:
	case 183:
	case 191:
	case 261:
	case 269:
	case 277:
		switch ((c->Skill))
		{
		case AT_SKILL_BLOOD_ATT_UP:
		case AT_SKILL_BLOOD_ATT_UP + 1:
		case AT_SKILL_BLOOD_ATT_UP + 2:
		case AT_SKILL_BLOOD_ATT_UP + 3:
		case AT_SKILL_BLOOD_ATT_UP + 4:
		case AT_SKILL_REDUCEDEFENSE:
			CreateMonsterSkill_ReduceDef(ThreadIndex, o, c->AttackTime, 13, 200.0f);
			break;

		case AT_SKILL_POISON:
			CreateMonsterSkill_Poison(ThreadIndex, o, c->AttackTime, 13);
			break;

		case AT_SKILL_MONSTER_SUMMON:
			CreateMonsterSkill_Summon(ThreadIndex, o, c->AttackTime, 13);
			break;

		case AT_SKILL_MONSTER_MAGIC_DEF:
			if (c->AttackTime >= 13)
			{
				g_CharacterRegisterBuff(o, eBuff_PhysDefense);
				c->AttackTime = 15;
				PlayBackground(SOUND_GREAT_SHIELD);
			}
			break;

		case AT_SKILL_MONSTER_PHY_DEF:
			if (c->AttackTime >= 13)
			{
				g_CharacterRegisterBuff(o, eBuff_Defense);
				c->AttackTime = 15;
				PlayBackground(SOUND_GREAT_SHIELD);
			}
			break;

		default:
			break;
		}
		if (o->CurrentAction == MONSTER01_ATTACK2 && c->AttackTime == 14)
		{
			CreateEffect(ThreadIndex, MODEL_SKILL_FURY_STRIKE, o->Position, o->Angle, o->Light, 0, o, -1, 0, 1);
			CreateEffect(ThreadIndex, BITMAP_JOINT_THUNDER + 1, o->Position, o->Angle, o->Light, 0, o, -1, 0, 1);
			c->AttackTime = 15;
		}
		return true;

	case 146:
	case 176:
	case 184:
	case 192:
	case 262:
	case 270:
	case 278:
		if (c->AttackTime == 14)
		{
			Vector(1.0f, 1.0f, 1.0f, Light);

			if (to != nullptr)
			{
				VectorCopy(to->Position, Position);
			}
			else
			{
				VectorCopy(Hero->Object.Position, Position);
			}
			Position[2] += 150.0f;
			CreateParticle(ThreadIndex, BITMAP_SHINY + 4, Position, o->Angle, Light, 1, 1.0f);
		}
		return true;

	case 147:
	case 177:
	case 185:
	case 193:
	case 263:
	case 271:
	case 279:
		if (o->CurrentAction == MONSTER01_ATTACK2 && c->AttackTime == 14)
		{
			CreateEffect(ThreadIndex, MODEL_WATER_WAVE, o->Position, o->Angle, o->Light);
		}
		return true;

	case 148:
	case 178:
	case 186:
	case 194:
	case 264:
	case 272:
	case 280:
		switch ((c->Skill))
		{
		case AT_SKILL_ENERGYBALL:
			if (c->AttackTime == 14)
			{
				CreateEffect(ThreadIndex, MODEL_SKILL_FURY_STRIKE, o->Position, o->Angle, o->Light, 1, o, -1, 0, 1);
			}
			break;

		default:
			break;
		}
		return true;

	case 149:
	case 179:
	case 187:
	case 195:
	case 265:
	case 273:
	case 281:
		switch ((c->Skill))
		{
		case AT_SKILL_POISON:
			if (c->AttackTime == 14)
			{
				EVector3 Light, Position;

				Vector(0.8f, 0.5f, 0.1f, Light);

				for (mu_int32 i = 0; i < 3; ++i)
				{
					Position[0] = Hero->Object.Position[0] + (GetLargeRand(200) - 100);
					Position[1] = Hero->Object.Position[1] + (GetLargeRand(200) - 100);
					Position[2] = Hero->Object.Position[2];

					CreateEffect(ThreadIndex, MODEL_FIRE, Position, o->Angle, Light, 7, nullptr, 0);
				}
			}
			break;

		case AT_SKILL_ENERGYBALL:
			if (c->AttackTime == 14)
			{
				if (c->TargetCharacter >= 0 && c->TargetCharacter < MAX_CHARACTERS_CLIENT)
				{
					CHARACTER *tc = &CharactersClient[c->TargetCharacter];
					OBJECT *to = &tc->Object;

					EVector3 Angle;
					Vector(0.0f, 0.0f, 0.0f, p);
					VectorCopy(o->Angle, Angle);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(60)], p, Position, true);

					CreateJoint(ThreadIndex, BITMAP_FLARE + 1, Position, to->Position, Angle, 6, to, 30.0f, 50);
					Angle[2] -= 30.0f;
					CreateJoint(ThreadIndex, BITMAP_FLARE + 1, Position, to->Position, Angle, 6, to, 30.0f, 50);
					Angle[2] += 15.0f;
					CreateJoint(ThreadIndex, BITMAP_FLARE + 1, Position, to->Position, Angle, 6, to, 30.0f, 50);
				}
			}
			break;
		}
		return true;

	case 160:
	case 180:
	case 188:
	case 196:
	case 266:
	case 274:
		if (o->CurrentAction == MONSTER01_ATTACK1 && c->AttackTime >= 13)
		{
			CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, o->Position, o->Angle, o->Light, 5, o);
			CreateEffect(ThreadIndex, BITMAP_FLAME, o->Position, o->Angle, o->Light, 2, o);
			c->AttackTime = 15;
		}
		return true;

	case 161:
	case 181:
	case 189:
	case 197:
	case 267:
		switch ((c->Skill))
		{
		case AT_SKILL_BLOOD_ATT_UP:
		case AT_SKILL_BLOOD_ATT_UP + 1:
		case AT_SKILL_BLOOD_ATT_UP + 2:
		case AT_SKILL_BLOOD_ATT_UP + 3:
		case AT_SKILL_BLOOD_ATT_UP + 4:
		case AT_SKILL_REDUCEDEFENSE:
			CreateMonsterSkill_ReduceDef(ThreadIndex, o, c->AttackTime, 13, 200.0f);
			break;

		case AT_SKILL_POISON:
			CreateMonsterSkill_Poison(ThreadIndex, o, c->AttackTime, 13);
			break;

		case AT_SKILL_MONSTER_SUMMON:
			CreateMonsterSkill_Summon(ThreadIndex, o, c->AttackTime, 13);
			break;

		case AT_SKILL_MONSTER_MAGIC_DEF:
			if (c->AttackTime >= 13)
			{
				g_CharacterRegisterBuff(o, eBuff_PhysDefense);
				c->AttackTime = 15;
				PlayBackground(SOUND_GREAT_SHIELD);
			}
			break;

		case AT_SKILL_MONSTER_PHY_DEF:
			if (c->AttackTime >= 13)
			{
				g_CharacterRegisterBuff(o, eBuff_Defense);
				c->AttackTime = 15;
				PlayBackground(SOUND_GREAT_SHIELD);
			}
			break;
		}

		if (o->CurrentAction == MONSTER01_ATTACK1)
		{
			if (c->AttackTime == 7)
			{
				CreateEffect(ThreadIndex, MODEL_SKILL_FURY_STRIKE, o->Position, o->Angle, o->Light, 0, o, -1, 0, 0);
			}
			else if (c->AttackTime >= 13)
			{
				CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, o->Position, o->Angle, o->Light, 0, o);
				CreateEffect(ThreadIndex, BITMAP_FLAME, o->Position, o->Angle, o->Light, 1, o);
				c->AttackTime = 15;
			}
		}
		return true;
	case 275:
		switch ((c->Skill))
		{
		case AT_SKILL_BLOOD_ATT_UP:
		case AT_SKILL_BLOOD_ATT_UP + 1:
		case AT_SKILL_BLOOD_ATT_UP + 2:
		case AT_SKILL_BLOOD_ATT_UP + 3:
		case AT_SKILL_BLOOD_ATT_UP + 4:
		case AT_SKILL_REDUCEDEFENSE:
			CreateMonsterSkill_ReduceDef(ThreadIndex, o, c->AttackTime, 13, 200.0f);
			break;

		case AT_SKILL_POISON:
			CreateMonsterSkill_Poison(ThreadIndex, o, c->AttackTime, 13);
			break;

		case AT_SKILL_MONSTER_SUMMON:
			CreateMonsterSkill_Summon(ThreadIndex, o, c->AttackTime, 13);
			break;

		case AT_SKILL_MONSTER_MAGIC_DEF:
			if (c->AttackTime >= 13)
			{
				g_CharacterRegisterBuff(o, eBuff_PhysDefense);
				c->AttackTime = 15;

				PlayBackground(SOUND_GREAT_SHIELD);
			}
			break;

		case AT_SKILL_MONSTER_PHY_DEF:
			if (c->AttackTime >= 13)
			{
				g_CharacterRegisterBuff(o, eBuff_Defense);
				c->AttackTime = 15;

				PlayBackground(SOUND_GREAT_SHIELD);
			}
			break;
		}

		return true;
	}

	return false;
}

mu_boolean MoveHellasMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	EVector3 Position, p;
	EVector3 Light;

	switch (o->Type)
	{
	case MODEL_WARCRAFT:
		if (o->CurrentAction == 0)
		{
			Position[0] = o->Position[0] + GetLargeRand(200) - 100;
			Position[1] = o->Position[1] + GetLargeRand(100) - 50;
			Position[2] = o->Position[2];

			CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, Position, o->Angle, o->Light);
			CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, Position, o->Angle, o->Light);
			CreateEffect(ThreadIndex, MODEL_STONE1, o->Position, o->Angle, o->Light);
			CreateEffect(ThreadIndex, MODEL_STONE2, o->Position, o->Angle, o->Light);
		}
		o->BlendMesh = 1;
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.001f)*0.5f + 0.5f;
		return true;

	case MODEL_MONSTER01 + 63:
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.001f)*0.7f + 0.3f;
		return true;

	case MODEL_MONSTER01 + 64:
		if (GetLargeRand(2) == 0)
		{
			Vector(2.0f, 30.0f, 0.0f, p);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(6)], p, Position, true);
			Vector(1.0f, 0.2f, 0.0f, Light);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 17);
		}
		return true;

	case MODEL_MONSTER01 + 65:
		return true;

	case MODEL_MONSTER01 + 66:
		return true;

	case MODEL_MONSTER01 + 67:
		return true;

	case MODEL_MONSTER01 + 68:
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.001f)*0.7f + 0.3f;
		return true;

	case MODEL_MONSTER01 + 69:
		if (o->CurrentAction != MONSTER01_DIE)
		{
			if (GetLargeRand(2) == 0)
			{
				Vector(2.0f, 30.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(31)], p, Position, true);
				if (o->SubType == 9)
				{
					Vector(1.0f, 0.0f, 0.0f, Light);
				}
				else
				{
					Vector(0.0f, 0.3f, 1.0f, Light);
				}
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 17);
			}
		}
		return true;
	}

	return false;
}

mu_boolean RenderHellasMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	EVector3 Position, Light, p;
	mu_float  Luminosity;
	mu_int32    i;

	switch (o->Type)
	{
	case MODEL_WARCRAFT:
		if (o->CurrentAction == 1)
		{
			VectorCopy(o->Position, Position);
			Position[2] += 20.0f;

			if (o->LifeTime == 0)
			{
				Vector(0.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(8)], p, Position, true);
				Vector(1.0f, 0.1f, 0.1f, Light);
				CreateParticle(ThreadIndex, BITMAP_HOLE, Position, o->Angle, Light, 0, 3.0f);
			}

			if (++o->LifeTime >= 5)
			{
				o->LifeTime = 0;
			}
		}
		return true;

	case MODEL_MONSTER01 + 63:
		Vector(0.0f, 0.0f, 0.0f, p);

		if (o->CurrentAction != MONSTER01_DIE)
		{
			Luminosity = 0.0f;

			for (i = 0; i < 5; ++i)
			{
				Luminosity += 1 / 4.0f;
				Vector(Luminosity*0.1f, Luminosity*0.4f, Luminosity, Light);

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(i + 74)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 1.5f - (0.2f*i), Light, o, FWorldTime);
				CreateSprite(BITMAP_LIGHT, Position, 1.5f - (0.2f*i), Light, o, FWorldTime);
				if ((GetLargeRand(2)) == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_BUBBLE, Position, o->Angle, Light, 1);
				}

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(i + 62)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 1.5f - (0.2f*i), Light, o, FWorldTime);
				CreateSprite(BITMAP_LIGHT, Position, 1.5f - (0.2f*i), Light, o, FWorldTime);
				if ((GetLargeRand(2)) == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_BUBBLE, Position, o->Angle, Light, 1);
				}
			}

			Vector(2.0f, 2.0f, 0.0f, p);
			Vector(o->BlendMeshLight, o->BlendMeshLight, o->BlendMeshLight, Light);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(8)], p, Position, true);
			CreateSprite(BITMAP_ENERGY, Position, 0.2f, Light, o, 0.0f);
			Vector(o->BlendMeshLight*0.1f, o->BlendMeshLight*0.1f, o->BlendMeshLight, Light);
			CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, Light, o, 0.0f);

			Vector(0.0f, 0.0f, 0.0f, p);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(22)], p, Position, true);
			CreateSprite(BITMAP_ENERGY, Position, 0.2f, Light, o, 0.0f);
			Vector(o->BlendMeshLight*0.1f, o->BlendMeshLight*0.1f, o->BlendMeshLight, Light);
			CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, Light, o, 0.0f);

			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(15)], p, Position, true);
			CreateSprite(BITMAP_ENERGY, Position, 0.2f, Light, o, 0.0f);
			Vector(o->BlendMeshLight*0.1f, o->BlendMeshLight*0.1f, o->BlendMeshLight, Light);
			CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, Light, o, 0.0f);
		}

		Luminosity = EMath::Sin(FWorldTime*0.001f)*0.2f + 0.4f;
		for (i = 0; i < 5; ++i)
		{
			Vector(Luminosity*0.1f, Luminosity*0.4f, Luminosity, Light);

			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(i + 51)], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o, FWorldTime);

			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(i + 29)], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, 1.5f, Light, o, FWorldTime);

		}
		return true;

	case MODEL_MONSTER01 + 64:

		if (o->CurrentAction == MONSTER01_DIE)
		{
			c->Weapon[1].Type = -1;
			c->Weapon[1].Level = -1;
		}
		else
		{
			Luminosity = (mu_float)EMath::Sin(FWorldTime*0.003f)*0.2f + 0.8f;
			Vector(0, 0, 0, p);
			Vector(Luminosity*1.0f, Luminosity*0.0f, Luminosity*0.0f, Light);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(8)], p, Position, true);
			CreateSprite(BITMAP_ENERGY, Position, 0.4f, Light, o, 0.0f);

			Vector(0, 0, 0, p);
			Vector(Luminosity*1.0f, Luminosity*0.0f, Luminosity*0.0f, Light);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(9)], p, Position, true);
			CreateSprite(BITMAP_ENERGY, Position, 0.4f, Light, o, 0.0f);

			Vector(0, 0, 0, p);
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(100)], p, Position, true);
				mu_float fRoar = 1.0f;
				if (o->CurrentAction == MONSTER01_SHOCK)
				{
					if (o->AnimationFrame > 10.0f && o->PKKey > 10)
						o->PKKey -= 1;
					else if (o->AnimationFrame > 4.0f)
						o->PKKey += 1;
					else o->PKKey = 10;

					fRoar = o->PKKey * 0.15f;
					if (o->AnimationFrame > 4.0f && o->AnimationFrame < 6.0f)
					{
						if (o->AnimationFrame < 5.0f) o->Skill += 3;
						else if (o->Skill > 3) o->Skill -= 3;
						fRoar += o->Skill * 0.15f;
					}
					else if (o->AnimationFrame > 8.0f && o->AnimationFrame < 10.0f)
					{
						if (o->AnimationFrame < 9.0f) o->Skill += 3;
						else if (o->Skill > 3) o->Skill -= 3;
						fRoar += o->Skill * 0.15f;
					}
					else
					{
						o->Skill = 0;
					}
					CreateSprite(BITMAP_FLARE_BLUE, Position, (1.2f + (EMath::Sin(FWorldTime*0.001f)*0.3f)), Light, o, 0.0f);
					CreateSprite(BITMAP_FLARE_RED, Position, (1.2f + (EMath::Sin(FWorldTime*0.001f)*0.3f))*fRoar, Light, o, 0.0f);
				}
				else
				{
					o->PKKey = 0;
					fRoar = 1.0f;
					CreateSprite(BITMAP_FLARE_BLUE, Position, (1.2f + (EMath::Sin(FWorldTime*0.001f)*0.3f)), Light, o, 0.0f);
				}
				if ((GetLargeRand(2)) == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 13, fRoar);
				}
			}

			if (o->CurrentAction == MONSTER01_SHOCK)
			{
				if (o->AnimationFrame < 2.0f)
				{
					if (o->LifeTime != 100)
					{
						o->LifeTime = 100;

						PlayBackground(SOUND_KUNDUN_ROAR);
					}
				}
				if (o->LifeTime == 100 && o->AnimationFrame > 4.0f)
				{
					o->LifeTime = 101;
					if (InHellas())
					{
						AddWaterWave((c->PositionX), (c->PositionY), 2, 2000);
					}
				}
				if (o->AnimationFrame > 3.0f)
				{
					EarthQuake.Store((mu_float)(GetLargeRand(8) - 8)*0.2f);
				}
				if (InHellas() && o->AnimationFrame > 3.0f && o->AnimationFrame < 14.0f)
				{
					EVector3 Position, Light;
					Vector(0.3f, 0.8f, 1.0f, Light);
					EVector3 Angle = { 0.0f, 0.0f, 0.0f };
					for (mu_int32 i = 0; i < 2; ++i)
					{
						mu_float fAngle = mu_float(GetLargeRand(360));
						mu_float fDistance = mu_float(GetLargeRand(600) + 200);
						Position[0] = o->Position[0] + EMath::Sin(fAngle)*fDistance;
						Position[1] = o->Position[1] + EMath::Cos(fAngle)*fDistance;
						Position[2] = o->Position[2] + 800.0f;
						CreateEffect(ThreadIndex, 9, Position, Angle, Light);
					}
				}
			}
			if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->AnimationFrame < 2.0f)
				{
					o->LifeTime = 100;
				}
				if (o->LifeTime == 100 && o->AnimationFrame > 3.0f)
				{
					o->LifeTime = 101;
					EVector3 Position;
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(49)], p, Position, true);
					mu_float fHeight = Position[2];
					Vector(o->Position[0], o->Position[1], fHeight, Position);
					CreateEffect(ThreadIndex, MODEL_CUNDUN_SKILL, Position, o->Angle, o->Light, 0);
				}
			}
			if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->AnimationFrame < 2.0f)
				{
					o->LifeTime = 100;
				}
				if (o->LifeTime == 100 && o->AnimationFrame > 5.0f)
				{
					o->LifeTime = 101;

					EVector3 Position;
					Vector(0.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(49)], p, Position, true);
					Position[2] = 400;
					CreateEffect(ThreadIndex, MODEL_CUNDUN_SKILL, Position, o->Angle, o->Light, 1);
				}
				if (o->LifeTime == 101 && o->AnimationFrame > 6.0f)
				{
					o->LifeTime = 102;

					EVector3 Position, Angle;
					Vector(0.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(49)], p, Position, true);
					Position[2] = 400;
					Angle[0] = 0;
					Angle[1] = 0;

					for (i = 0; i < 24; ++i)
					{
						Angle[2] = (mu_float)(i * 30);
						CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT2, Position, Position, Angle, 14, nullptr, 100.0f, 0, 0);
					}
					if (InHellas())
					{
						AddWaterWave((c->PositionX), (c->PositionY), 2, 2000);
					}
				}
				if (o->LifeTime == 102 && o->AnimationFrame > 9.0f)
				{
					o->LifeTime = 103;
					if (InHellas())
					{
						AddWaterWave((c->PositionX), (c->PositionY), 2, 2000);
					}
				}
				if (o->AnimationFrame > 6.0f)
				{
					EarthQuake.Store((mu_float)(GetLargeRand(16) - 16)*0.1f);
				}
			}
		}
		return true;

	case MODEL_MONSTER01 + 65:
		if (o->CurrentAction != MONSTER01_DIE)
		{
			Luminosity = (mu_float)EMath::Sin(FWorldTime*0.003f)*0.2f + 0.8f;
			Vector(20.0f, 30.0f, -7.0f, p);
			Vector(Luminosity*1.0f, Luminosity*0.9f, Luminosity*0.9f, Light);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(6)], p, Position, true);
			CreateSprite(BITMAP_ENERGY, Position, 0.2f, Light, o, 0.0f);
			Vector(Luminosity*1.0f, Luminosity*0.3f, Luminosity*0.1f, Light);
			CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, Light, o, 0.0f);

			Vector(20.0f, 30.0f, 7.0f, p);
			Vector(Luminosity*1.0f, Luminosity*0.9f, Luminosity*0.9f, Light);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(6)], p, Position, true);
			CreateSprite(BITMAP_ENERGY, Position, 0.2f, Light, o, 0.0f);
			Vector(Luminosity*1.0f, Luminosity*0.3f, Luminosity*0.1f, Light);
			CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, Light, o, 0.0f);

			Vector(0.0f, 0.0f, 7.0f, p);
			Vector(Luminosity*1.0f, Luminosity*0.9f, Luminosity*0.9f, Light);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(13)], p, Position, true);
			CreateSprite(BITMAP_ENERGY, Position, 0.1f, Light, o, 0.0f);
			Vector(Luminosity*1.0f, Luminosity*0.5f, Luminosity*0.1f, Light);
			CreateSprite(BITMAP_SHINY + 1, Position, 0.7f, Light, o, 0.0f);

			Vector(0.0f, 0.0f, -7.0f, p);
			Vector(Luminosity*1.0f, Luminosity*0.9f, Luminosity*0.9f, Light);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(19)], p, Position, true);
			CreateSprite(BITMAP_ENERGY, Position, 0.1f, Light, o, 0.0f);
			Vector(Luminosity*1.0f, Luminosity*0.5f, Luminosity*0.1f, Light);
			CreateSprite(BITMAP_SHINY + 1, Position, 0.7f, Light, o, 0.0f);

			Vector(Luminosity*1.0f, Luminosity*0.8f, Luminosity*0.8f, Light);
			Vector(0, 0, 0, p);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(23)], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, 3, Light, o, 0.0f);
			Vector(-30, -25, 0, p);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(25)], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, 2.5f, Light, o, 0.0f);
			Vector(0, 0, 0, p);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(31)], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, 3, Light, o, 0.0f);
			Vector(30, 25, 0, p);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(33)], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, 2.5f, Light, o, 0.0f);
		}
		return true;

	case MODEL_MONSTER01 + 66:
		if (o->CurrentAction != MONSTER01_DIE)
		{
			Luminosity = (mu_float)EMath::Sin(FWorldTime*0.005f)*0.15f + 0.85f;
			Vector(4.0f, 0.0f, 5.0f, p);
			Vector(Luminosity*1.0f, Luminosity*1.0f, Luminosity*1.0f, Light);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(9)], p, Position, true);
			CreateSprite(BITMAP_ENERGY, Position, 0.1f, Light, o, 0.0f);
			Vector(Luminosity*0.3f, Luminosity*0.6f, Luminosity*1.0f, Light);
			CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, Light, o, 0.0f);

			Vector(4.0f, 0.0f, 5.0f, p);
			Vector(Luminosity*1.0f, Luminosity*1.0f, Luminosity*1.0f, Light);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(10)], p, Position, true);
			CreateSprite(BITMAP_ENERGY, Position, 0.1f, Light, o, 0.0f);
			Vector(Luminosity*0.3f, Luminosity*0.6f, Luminosity*1.0f, Light);
			CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, Light, o, 0.0f);

			Vector(0, 0, 0, p);
			Vector(0.7f, 0.6f, 1, Light);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(3)], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, 1.7f, Light, o, 0.0f);
			CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, Light, o, (mu_float)(GetLargeRand(360)));
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(4)], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, 1.7f, Light, o, 0.0f);
			CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, Light, o, (mu_float)(GetLargeRand(360)));

			EVector3 pos1, pos2;
			Vector(0, 0, 0, p);
			if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				for (i = 10; i < 16; ++i)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(i)], p, pos1, true);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(i + 1)], p, pos2, true);
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, pos1, pos2, o->Angle, 7, nullptr, 30.0f);
				}
				for (i = 31; i < 37; ++i)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(i)], p, pos1, true);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(i + 1)], p, pos2, true);
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, pos1, pos2, o->Angle, 7, nullptr, 30.0f);
				}

			}
		}
		return true;

	case MODEL_MONSTER01 + 67:
		if (o->CurrentAction != MONSTER01_DIE)
		{
			Vector(0.0f, 0.0f, 30.0f, p);

			if (c->MonsterIndex == 145)
			{
				Vector(1.0f, 0.0f, 0.0f, Light);

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
				CreateSprite(BITMAP_FLARE_BLUE, Position, 2.0f + (EMath::Sin(FWorldTime*0.001f)*0.3f), Light, o, 0.0f);

				if ((GetLargeRand(2)) == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 13);
				}
			}
			else
			{
				Vector(1.0f, 1.0f, 1.0f, Light);

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
				CreateSprite(BITMAP_FLARE_BLUE, Position, 1.2f + (EMath::Sin(FWorldTime*0.001f)*0.3f), Light, o, 0.0f);
			}

			Vector(5.0f, 0.0f, 0.0f, p);
			Vector(1.0f, 1.0f, 1.0f, Light);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(28)], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, 0.3f + (EMath::Sin(FWorldTime*0.001f)*0.2f), Light, o, 0.0f);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(29)], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, 0.3f + (EMath::Sin(FWorldTime*0.001f)*0.2f), Light, o, 0.0f);
		}
		else
		{
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
		}
		return true;

	case MODEL_MONSTER01 + 68:
		if (o->CurrentAction != MONSTER01_DIE)
		{
			EVector3 pos;
			EVector3 Angle;

			Vector(5.0f, 0.0f, 0.0f, p);
			Vector(1.0f, 1.0f, 1.0f, Light);
			VectorCopy(o->Angle, Angle);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(9)], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, 0.3f + (EMath::Sin(FWorldTime*0.001f)*0.2f), Light, o, 0.0f);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(10)], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, 0.3f + (EMath::Sin(FWorldTime*0.001f)*0.2f), Light, o, 0.0f);

			Vector(0.0f, 0.0f, 0.0f, p);
			Luminosity = (mu_float)EMath::Sin(FWorldTime*0.002f)*0.3f + 0.6f;

			if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				Vector(Luminosity*0.1f, Luminosity, Luminosity*0.1f, Light);
			}
			else
			{
				Vector(Luminosity, Luminosity, Luminosity, Light);
			}
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(60)], p, pos, true);
			CreateSprite(BITMAP_ENERGY, pos, 0.5f + (Luminosity*0.2f), Light, o, FWorldTime*0.1f);
			CreateSprite(BITMAP_ENERGY, pos, 0.5f + (Luminosity*0.2f), Light, o, -FWorldTime*0.1f);
			CreateParticle(ThreadIndex, BITMAP_LIGHT, pos, o->Angle, Light, 0, 1.1f);

			Vector(0.1f, 0.4f, 1.0f, Light);
			CreateSprite(BITMAP_LIGHT, Position, 1.0f, Light, o, 0.0f);

			Vector(0.1f, 0.3f, 1.0f, Light);
			for (i = 0; i < 5; ++i)
			{
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(63 + i)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 0.5f + (EMath::Sin(FWorldTime*0.001f)*0.2f), Light, o, 0.0f);

				if ((o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2) && (GetLargeRand(50)) == 0)
				{
					Angle[0] = (mu_float)(GetLargeRand(360));
					Angle[2] = (mu_float)(GetLargeRand(360));
					CreateJoint(ThreadIndex, BITMAP_FLARE + 1, Position, pos, Angle, 5, nullptr, 20.0f);
				}
			}
		}
		return true;

	case MODEL_MONSTER01 + 69:
		if (o->CurrentAction != MONSTER01_DIE)
		{
			EVector3 Pos1, Pos2;

			Luminosity = (mu_float)EMath::Sin(FWorldTime*0.003f)*0.2f + 0.8f;
			Vector(0, 0, 0, p);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(33)], p, Pos1, true);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(34)], p, Pos2, true);
			if (o->SubType == 9)
			{
				Vector(Luminosity*1.0f, Luminosity*0.9f, Luminosity*0.9f, Light);
			}
			else
			{
				Vector(Luminosity*0.0f, Luminosity*0.9f, Luminosity*1.0f, Light);
			}
			CreateSprite(BITMAP_ENERGY, Pos1, 0.1f, Light, o, 0.0f);
			CreateSprite(BITMAP_ENERGY, Pos2, 0.1f, Light, o, 0.0f);
			if (o->SubType == 9)
			{
				Vector(Luminosity*1.0f, Luminosity*0.3f, Luminosity*0.1f, Light);
			}
			else
			{
				Vector(Luminosity*0.1f, Luminosity*0.3f, Luminosity*1.0f, Light);
			}
			CreateSprite(BITMAP_SHINY + 1, Pos1, 0.7f, Light, o, 0.0f);
			CreateSprite(BITMAP_SHINY + 1, Pos2, 0.7f, Light, o, 0.0f);

			if (o->SubType == 9)
			{
				Vector(Luminosity*1.0f, Luminosity*0.9f, Luminosity*0.9f, Light);
			}
			else
			{
				Vector(Luminosity*0.9f, Luminosity*0.9f, Luminosity*1.0f, Light);
			}
			for (i = 0; i < 6; ++i)
			{
				Vector(0.0f, -40.0f - i * 24.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(41)], p, Pos1, true);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(51)], p, Pos2, true);
				if (o->SubType == 9)
				{
					CreateParticle(ThreadIndex, BITMAP_FIRE + 1, Pos1, o->Angle, Light, 1, 7.2f / (i / 2 + 6));
					CreateParticle(ThreadIndex, BITMAP_FIRE + 1, Pos2, o->Angle, Light, 1, 7.2f / (i / 2 + 6));
				}
				else
				{
					CreateParticle(ThreadIndex, BITMAP_FIRE + 3, Pos1, o->Angle, Light, 12, 7.2f / (i / 2 + 6)*0.5f);
					CreateParticle(ThreadIndex, BITMAP_FIRE + 3, Pos2, o->Angle, Light, 12, 7.2f / (i / 2 + 6)*0.5f);
				}
			}
		}
		else
		{
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
		}
		return true;
	}
	return false;
}

mu_boolean RenderHellasMonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_boolean success = false;
	if (o->Type == MODEL_MONSTER01 + 63)
	{
		if (o->CurrentAction == MONSTER01_DIE)
		{
			if (o->Alpha < 0.5f)
			{
				b->RenderMeshEffect(ThreadIndex, o->BoneTransform, 0, BITMAP_BUBBLE);
			}
		}
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_METAL | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
		success = true;
	}
	else if (o->Type == MODEL_MONSTER01 + 64)
	{
		if (o->CurrentAction == MONSTER01_DIE && o->AnimationFrame > 14.8f && o->LifeTime == 90)
		{
			if (o->LifeTime != 10)
			{
				PlayBackground(SOUND_KUNDUN_SHUDDER);
			}
			o->LifeTime = 10;
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			EarthQuake.Store((mu_float)(GetLargeRand(8) - 8)*0.1f);

			if(MU_UPDATECOUNTER > 0)
			{
				EVector3 p, Position;
				Vector(39.0f, -7.5f, -0.5, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(4)], p, Position, true);
				CreateEffect(ThreadIndex, MODEL_CUNDUN_PART8, Position, o->Angle, o->Light, 3, o, -130, 3);
				Vector(24.0f, -7.5f, 32.5f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(4)], p, Position, true);
				CreateEffect(ThreadIndex, MODEL_CUNDUN_PART1, Position, o->Angle, o->Light, 3, o, -130, 4);
				Vector(24.0f, -8.5f, -32.5f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(4)], p, Position, true);
				CreateEffect(ThreadIndex, MODEL_CUNDUN_PART2, Position, o->Angle, o->Light, 3, o, -130, 5);
				Vector(-0.5f, 4.0f, 0.5f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(4)], p, Position, true);
				CreateEffect(ThreadIndex, MODEL_CUNDUN_PART3, Position, o->Angle, o->Light, 2, o, -130, 6);
				Vector(-2.5f, -22.0f, 54.0f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(4)], p, Position, true);
				CreateEffect(ThreadIndex, MODEL_CUNDUN_PART4, Position, o->Angle, o->Light, 3, o, -130, 1);
				Vector(-4.5f, -24.5f, -53, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(4)], p, Position, true);
				CreateEffect(ThreadIndex, MODEL_CUNDUN_PART5, Position, o->Angle, o->Light, 3, o, -130, 2);
				Vector(-136.0f, -153.5f, 0, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(4)], p, Position, true);
				CreateEffect(ThreadIndex, MODEL_CUNDUN_PART6, Position, o->Angle, o->Light, 4, o, -10, 2);
				Vector(-135.0f, -153.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(4)], p, Position, true);
				CreateEffect(ThreadIndex, MODEL_CUNDUN_PART7, Position, o->Angle, o->Light, 5, o, -130, 2);

				CreateEffect(ThreadIndex, MODEL_CUNDUN_SKILL, o->Position, o->Angle, o->Light, 2);
			}
		}
		else
		{
			if (o->CurrentAction == MONSTER01_DIE && o->AnimationFrame >= 8 && o->LifeTime >= 100)
			{
				if (o->LifeTime != 90)
				{
					PlayBackground(SOUND_KUNDUN_DESTROY);
				}
				o->LifeTime = 90;

				if(MU_UPDATECOUNTER > 0)
				{
					EVector3 Angle = { 0.0f, 0.0f, 0.0f };
					mu_int32 iCount = 86;
					for (mu_int32 i = 0; i < iCount; ++i)
					{
						Angle[0] = -10.0f;
						Angle[1] = 0.0f;
						Angle[2] = i*(10.0f + GetLargeRand(10));

						EVector3 Position;
						VectorCopy(o->Position, Position);
						Position[2] += 200.0f;
						CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, Position, Position, Angle, 3, nullptr, 50.0f, 0, 0);
						CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT2, Position, Position, Angle, 3, nullptr, 50.0f, 0, 0);
					}
				}
				EarthQuake.Store((mu_float)(GetLargeRand(8) - 8)*0.1f);
			}

			if (o->LifeTime >= 90)
			{
				o->Alpha = 1.0f;
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				Vector(0.4f, 0.4f, 0.3f, b->BodyLight(ThreadIndex));
				b->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				b->RenderMesh(ThreadIndex, 2, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				b->RenderMesh(ThreadIndex, 3, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				b->RenderMesh(ThreadIndex, 4, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				b->RenderMesh(ThreadIndex, 5, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				b->RenderMesh(ThreadIndex, 6, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

				if (o->CurrentAction == MONSTER01_SHOCK)
				{
					if ((o->AnimationFrame > 3.5f && o->AnimationFrame < 6.0f) ||
						(o->AnimationFrame > 8.0f && o->AnimationFrame < 11.0f))
					{
						b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
						b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
						b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
						b->RenderMesh(ThreadIndex, 5, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
					}
				}
			}
		}
		success = true;
	}
	else if (o->Type == MODEL_CUNDUN_PART1 || o->Type == MODEL_CUNDUN_PART2 || o->Type == MODEL_CUNDUN_PART3 ||
		o->Type == MODEL_CUNDUN_PART4 || o->Type == MODEL_CUNDUN_PART5 || o->Type == MODEL_CUNDUN_PART8)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		Vector(0.4f, 0.4f, 0.3f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		success = true;
	}
	else if (o->Type == MODEL_MONSTER01 + 65)
	{
		if (o->CurrentAction == MONSTER01_DIE)
		{
			if (o->Alpha < 0.5f)
			{
				b->RenderMeshEffect(ThreadIndex, o->BoneTransform, 0, BITMAP_BUBBLE);
			}
		}
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		Vector(0.5f, 0.1f, 0.0f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		success = true;
	}
	else if (o->Type == MODEL_MONSTER01 + 66)
	{
		if (o->CurrentAction == MONSTER01_DIE)
		{
			if (o->Alpha < 0.5f)
			{
				b->RenderMeshEffect(ThreadIndex, o->BoneTransform, 0, BITMAP_BUBBLE);
			}
		}
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		Vector(0.25f, 0.2f, 0.1f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 2, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		success = true;
	}
	else if (o->Type == MODEL_MONSTER01 + 67)
	{
		if (o->CurrentAction == MONSTER01_DIE)
		{
			if (o->Alpha < 0.5f)
			{
				b->RenderMeshEffect(ThreadIndex, o->BoneTransform, 0, BITMAP_BUBBLE);
			}
		}
		if (o->SubType == 9)
		{
			Vector(0.3f, 0.1f, 0.1f, b->BodyLight(ThreadIndex));
		}
		else
		{
		}
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		if (o->SubType == 9)
		{
			Vector(1.0f, 0.6f, 0.3f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, 0.5f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 1, RENDER_CHROME2 | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			Vector(1.0f, 0.6f, 0.1f, b->BodyLight(ThreadIndex));
		}
		else
		{
			Vector(0.1f, 0.6f, 1.0f, b->BodyLight(ThreadIndex));
		}
		b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 2, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		if (o->SubType == 9)
		{
			RenderDestroy_Def(ThreadIndex, o, b);
		}
		success = true;
	}
	else if (o->Type == MODEL_MONSTER01 + 68)
	{
		if (o->CurrentAction == MONSTER01_DIE)
		{
			if (o->Alpha < 0.5f)
			{
				b->RenderMeshEffect(ThreadIndex, o->BoneTransform, 0, BITMAP_BUBBLE);
			}
		}
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		mu_float Luminosity = EMath::Sin(FWorldTime*0.002f)*0.3f + 0.7f;
		Vector(0.5f, Luminosity, 0.5f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, 0.8f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		success = true;
	}
	else if (o->Type == MODEL_MONSTER01 + 69)
	{
		if (o->CurrentAction == MONSTER01_DIE)
		{
			if (o->Alpha < 0.5f)
			{
				b->RenderMeshEffect(ThreadIndex, o->BoneTransform, 0, BITMAP_BUBBLE);
			}
		}

		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 5, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		mu_float Luminosity = EMath::Sin(FWorldTime*0.002f)*0.3f + 0.7f;
		if (o->SubType == 9)
		{
			o->BlendMeshLight = 0.5f;

			Vector(Luminosity, Luminosity*0.4f, Luminosity*0.4f, b->BodyLight(ThreadIndex));
		}
		else
		{
			o->BlendMeshLight = 1.0f;
			Vector(Luminosity*0.3f, Luminosity*0.6f, Luminosity*1.0f, b->BodyLight(ThreadIndex));
		}
		b->RenderMesh(ThreadIndex, 2, RENDER_WAVE | RENDER_BRIGHT, o->Alpha, 2, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 4, RENDER_WAVE | RENDER_BRIGHT, o->Alpha, 4, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		if (o->SubType == 9)
		{
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_METAL | RENDER_BRIGHT, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_METAL | RENDER_BRIGHT, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 3, RENDER_METAL | RENDER_BRIGHT, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else
		{
			mu_float Light = EMath::Sin(FWorldTime*0.002f)*0.3f + 0.7f;
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, 2, Light, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE, o->Alpha, 4, Light, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}

		o->BlendMeshLight = EMath::Sin(FWorldTime*0.002f)*0.1f + 0.05f;
		b->RenderMesh(ThreadIndex, 6, RENDER_TEXTURE, o->Alpha, 6, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		RenderDestroy_Def(ThreadIndex, o, b);
		success = true;
	}
	if (success)
	{
		if (g_isCharacterBuff(o, eBuff_PhysDefense) || g_isCharacterBuff(o, eBuff_Defense))
		{
			mu_float Luminosity = EMath::Sin(FWorldTime*0.001f)*0.2f + 0.5f;

			if (g_isCharacterBuff(o, eBuff_PhysDefense))
			{
				Vector(Luminosity*0.1f, Luminosity*0.3f, Luminosity*0.6f, b->BodyLight(ThreadIndex));
			}
			else if (g_isCharacterBuff(o, eBuff_Defense))
			{
				Vector(Luminosity*0.1f, Luminosity*0.6f, Luminosity*0.3f, b->BodyLight(ThreadIndex));
			}

			RenderPartObjectEdge(ThreadIndex, b, o, RENDER_CHROME | RENDER_BRIGHT, true, 1.3f);
		}
	}

	return success;
}