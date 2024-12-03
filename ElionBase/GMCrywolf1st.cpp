#include "stdafx.h"
#include "GMCrywolf1st.h"

mu_uint8 m_AltarState[5] = {2,2,2,2,2};

mu_boolean View_Bal = false;
mu_int8	Suc_Or_Fail = -1;
mu_int8	View_Suc_Or_Fail = -1;
mu_float Deco_Insert  = 0.0f;
mu_int8 Message_Box = 0;
mu_text Box_String[2][200] = { {0}, {0} };
mu_int32 Dark_elf_Num	= 0;
mu_int32 Button_Down = 0;
mu_int32 BackUp_Key = 0;
mu_int32 Val_Hp = 100;
mu_int32 m_iHour = 0,m_iMinute = 0;
mu_uint64 m_dwSyncTime;
mu_int32 Delay = 1;
mu_int32 Add_Num = 10;
mu_boolean Dark_Elf_Check = false;
mu_int32 iNextNotice = -1;

mu_uint8 Rank = 0;
mu_int32 Exp = 0;
mu_uint8 Ranking[5];
mu_uint8 HeroClass[5];
mu_int32 HeroScore[5] = {-1,-1,-1,-1,-1};
mu_string HeroName[5];

mu_int32 BackUpMin = 0;
mu_boolean TimeStart = false;
mu_int32 Delay_Add_inter = 390;
mu_boolean View_End_Result = false;
mu_int32 nPastTick = 0;
mu_int32 BackUpTick = 0;

mu_uint8 m_OccupationState = 0;
mu_uint8 m_CrywolfState = 0;
mu_int32 m_StatueHP = 0;

void M34CryWolf1st::CryWolfMVPInit()
{
	Deco_Insert  = 0.0f;
	memset(Box_String, 0, sizeof(Box_String));
	Button_Down = 0;
	BackUp_Key = 0;
	m_iHour = 0,m_iMinute = 0;
	m_dwSyncTime = -1;
	Add_Num = 10;
	TargetNpc = -1;
	Delay = 1;
	Dark_Elf_Check = false;

	iNextNotice = -1;
	View_Bal = false;
	Message_Box = 0;
	Dark_elf_Num	= 0;
	Val_Hp = 100;

	m_OccupationState = 0;
	m_CrywolfState = 0;
	m_StatueHP = 0;
	SelectedNpc = -1;
}

mu_int32 M34CryWolf1st::IsCryWolf1stMVPStart()
{
	return m_OccupationState; 
}

mu_boolean M34CryWolf1st::IsCryWolf1stMVPStatePeace()
{
	if( GET_WORLD == WD_34CRYWOLF_1ST && m_OccupationState == CRYWOLF_OCCUPATION_STATE_PEACE )
		return true;

	return false;
}

void M34CryWolf1st::CheckCryWolf1stMVP(mu_uint8 btOccupationState, mu_uint8 btCrywolfState)
{
	if (m_OccupationState == btOccupationState && m_CrywolfState == btCrywolfState)
		return;

	m_CrywolfState = btCrywolfState;

	if (m_CrywolfState >= CRYWOLF_STATE_READY && m_CrywolfState < CRYWOLF_STATE_ENDCYCLE)
	{
#if 0 // FIX
		g_pNewUISystem->Show(SEASON3B::INTERFACE_CRYWOLF);
#endif
	}

	if (m_CrywolfState == CRYWOLF_STATE_START)
	{
		Dark_Elf_Check = true;

#if 0 // FIX
		g_pCryWolfInterface->InitTime();
#endif
	}

	if (m_CrywolfState != CRYWOLF_STATE_START)
		View_Bal = false;

	if (m_CrywolfState == CRYWOLF_STATE_END)
	{
		for (mu_int32 i = 0; i < 5; ++i)
			HeroScore[i] = -1;

		if (btOccupationState != CRYWOLF_OCCUPATION_STATE_WAR)
		{
			Suc_Or_Fail = 1;

			if (btOccupationState == CRYWOLF_OCCUPATION_STATE_PEACE)
			{
				Add_Num = 11;
				View_Suc_Or_Fail = 1;
			}
			else
				View_Suc_Or_Fail = -1;
		}
	}

	if (m_OccupationState == btOccupationState)
		return;

	m_OccupationState = btOccupationState;

	g_Terrain.Load(GET_WORLD, m_OccupationState, false);
}

void M34CryWolf1st::CheckCryWolf1stMVPAltarfInfo(mu_int32 StatueHP, mu_uint8 AltarState1, mu_uint8 AltarState2, mu_uint8 AltarState3, mu_uint8 AltarState4, mu_uint8 AltarState5)
{
	m_StatueHP = StatueHP;
	m_AltarState[0] = AltarState1;
	m_AltarState[1] = AltarState2;
	m_AltarState[2] = AltarState3;
	m_AltarState[3] = AltarState4;
	m_AltarState[4] = AltarState5;
}

void M34CryWolf1st::DoTankerFireFixStartPosition(const mu_uint32 ThreadIndex, mu_int32 SourceX, mu_int32 SourceY, mu_int32 PositionX, mu_int32 PositionY)
{
	EVector3 Position, TargetPosition;

    Vector ( PositionX*g_TerrainScale, PositionY*g_TerrainScale, 100.0f, TargetPosition );

	mu_int32 Type = 0;
	if(SourceX == 122 || SourceX == 116) Type = 2;
	else if(SourceX == 62)  Type = 0;
	else if(SourceX == 183)  Type = 1;

    switch ( Type )
    {
    case 0:
        Vector(TargetPosition[0]-800.0f, TargetPosition[1], 800.0f, Position);
        CreateEffect(ThreadIndex, MODEL_ARROW_TANKER_HIT, Position, TargetPosition, Hero->Object.Light, 0, &Hero->Object);
        break;
    case 1:
		Vector(TargetPosition[0]+800.0f, TargetPosition[1], 800.0f, Position);
        CreateEffect(ThreadIndex, MODEL_ARROW_TANKER_HIT, Position, TargetPosition, Hero->Object.Light, 1, &Hero->Object);
        break;
    case 2:
		Vector(TargetPosition[0], TargetPosition[1]+800.0f, 800.0f, Position);
        CreateEffect(ThreadIndex, MODEL_ARROW_TANKER_HIT, Position, TargetPosition, Hero->Object.Light, 2, &Hero->Object);
        break;
   }
}

void M34CryWolf1st::RenderNoticesCryWolf()
{
	if(m_CrywolfState != CRYWOLF_STATE_READY) 
	{
		return;
	}
	
	mu_int32 iTemp = 0;

#if 0 // FIX
	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetBgColor(0, 0, 0, 170);
#endif

	if(GetTimeCheck(10000))
	{
		if(++iNextNotice >= 4)
			iNextNotice = 0;
	}
	iTemp = 4 * iNextNotice;

	mu_text szText[256];
	mu_int32 nText = 0;

	for(mu_int32 i = 0; i < 4; ++i)
	{
#if 0 // FIX
		if(i == 0)
		{
			g_pRenderText->SetTextColor(100, 200, 255, 255);
		}
		else
		{
			g_pRenderText->SetTextColor(100, 150, 255, 255);
		}
#ifdef ASG_FIX_TEXT_SCRIPT_PERCENT_TREATMENT
		nText = 1957+i+iTemp;
		if (1966 == nText || 1967 == nText)
		{
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[nText]);
			g_pRenderText->RenderText(190, 63+i*13, szText);
		}
		else
			g_pRenderText->RenderText(190, 63+i*13, GlobalText[nText]);
#else	// ASG_FIX_TEXT_SCRIPT_PERCENT_TREATMENT
		g_pRenderText->RenderText(190,63+i*13,GlobalText[1957+i+iTemp]);
#endif	// ASG_FIX_TEXT_SCRIPT_PERCENT_TREATMENT
#endif
	}
}

void M34CryWolf1st::MoveCryWolf1stObject(OBJECT* o)
{
	mu_float Luminosity;
	EVector3 Light;

	switch (o->Type)
	{
	case 36:
		{
		}
		break;
	case 70:
		{
		}
		break;
	case 41:
		{
			Vector(0.2f, 0.7f, 0.5f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
		}
		break;
	case 71:
	case 57:
		{
			Luminosity = (mu_float)(GetLargeRand(4) + 3)*0.1f;
			Vector(Luminosity, Luminosity*0.6f, Luminosity*0.2f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
		}
		break;
	case 81:
		o->Alpha = 0.2f;
		break;
	case 82:
	case 83:
	case 84:
		o->HiddenMesh = -2;
		break;
	}
}

void M34CryWolf1st::RenderCryWolf1stObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	EVector3 Light, p;
	mu_float Scale;
	Vector(0.0f, 0.0f, 0.0f, p);

	switch (o->Type)
	{
	case 41:
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			VectorCopy(o->Position, p);
			static mu_atomic_int32_t Ro(0);

			mu_int32 exro = 360, ro = 0;
			if (Ro.compare_exchange_weak(exro, 0) == false)
			{
				ro = Ro.fetch_add(1) % 360;
			}

			mu_float aaa;
			if (Time_Effect > 20)
				aaa = 40 - Time_Effect;
			else
				aaa = Time_Effect;

			Scale = 1.3f;
			p[2] += (350.0f + aaa);

			Vector(1.0f, 1.0f, 1.0f, Light);
			CreateSprite(BITMAP_FLARE, p, Scale, Light, o);
			CreateSprite(BITMAP_FLARE, p, Scale, Light, o, ro);
		}
		break;

	case 84:
		if (GetLargeRand(3) == 0 && m_OccupationState == CRYWOLF_OCCUPATION_STATE_OCCUPIED)
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 21, o->Scale);
		}
		break;
	}
}

mu_boolean M34CryWolf1st::RenderCryWolf1stObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_int32 ExtraMon)
{
	mu_float Luminosity;
	EVector3 Light;

	switch (o->Type)
	{
	case 36:
		{
			if (m_OccupationState == CRYWOLF_OCCUPATION_STATE_PEACE)
			{
				Vector(0.4f, 0.4f, 0.4f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
			else
			{
				Vector(0.4f, 0.4f, 0.4f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
		}
		return true;
	case 56:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			if (MU_UPDATECOUNTER > 0 &&
				m_OccupationState == CRYWOLF_OCCUPATION_STATE_PEACE)
			{
				mu_float Color = (mu_int32)GetLargeRand(10000) * 0.0001f;

				Vector(Color, Color, Color, Light);

				if (GetLargeRand(3) == 0)
					CreateEffect(ThreadIndex, BITMAP_MAGIC, o->Position, o->Angle, Light, 3, o, 4.0f);
			}
		}
		return true;
	case 41:
		Vector(0.2f, 0.7f, 0.0f, Light);
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		return true;
	case 57:
		{
			if (MU_UPDATECOUNTER > 0 &&
				GetLargeRand(3) == 0)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, o->Position, o->Angle, Light, 5, o->Scale);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 21, o->Scale);
			}
		}
		return true;
	case 71:
		{
			if (MU_UPDATECOUNTER > 0 &&
				GetLargeRand(3) == 0)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, o->Position, o->Angle, Light, 5, o->Scale);
			}
		}
		return true;
	case 72:
		{
			if (MU_UPDATECOUNTER > 0 &&
				GetLargeRand(10) == 0)
			{
				EVector3 Position;
				VectorCopy(o->Position, Position);
				Position[2] += 50.0f;
				Luminosity = (mu_float)(GetLargeRand(4) + 3)*0.1f;
				Vector(Luminosity, Luminosity*0.6f, Luminosity*0.2f, Light);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 33, o->Scale);
			}
		}
		return true;
	case 73:
		{
			if (weather == 0)
				g_Ashies = true;
		}
		return true;
	case 74:
		{
			if (MU_UPDATECOUNTER > 0 &&
				GetLargeRand(3) == 0 &&
				m_OccupationState == CRYWOLF_OCCUPATION_STATE_OCCUPIED)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 21, o->Scale);
			}
		}
		return true;
	case 77:
		{
			if (MU_UPDATECOUNTER > 0 &&
				GetLargeRand(6) == 0)
			{
				Vector(0.02f, 0.03f, 0.02f, Light);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 9, o->Scale, o);
			}
		}
		return true;
	case 78:
		{
			if (MU_UPDATECOUNTER > 0 && 
				GetLargeRand(6) == 0)
			{
				Vector(0.03f, 0.06f, 0.05f, Light);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 9, o->Scale, o);
			}
		}
		return true;
	case 81:
		{
			if (m_OccupationState == CRYWOLF_OCCUPATION_STATE_WAR)
			{
				o->Scale = 1.03f;

				mu_float fTemp = 0.1f;

				if (m_StatueHP <= 10) fTemp = 1.0f;
				b->BodyLight(ThreadIndex)[0] = (EMath::Sin(FWorldTime*0.004f)*3.0f)*fTemp + 5.0f - m_StatueHP / 20.0f;
				b->BodyLight(ThreadIndex)[1] = m_StatueHP / 25.0f - 1.0f;
				b->BodyLight(ThreadIndex)[2] = m_StatueHP / 20.0f - 0.5f;

				b->StreamMesh(ThreadIndex) = 0;
				b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->StreamMesh(ThreadIndex) = -1;

				b->StreamMesh(ThreadIndex) = 1;
				b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME2, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->StreamMesh(ThreadIndex) = -1;
			}
		}
		return true;
	case MODEL_SKILL_FURY_STRIKE:
		{
			Vector(0.0f, 0.0f, 0.9f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		return true;
	}

	return RenderCryWolf1stMonsterObjectMesh(ThreadIndex, o, b, ExtraMon);
}

CHARACTER* M34CryWolf1st::CreateCryWolf1stMonster(const mu_uint32 ThreadIndex, mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key)
{
	if (!IsCyrWolf1st() && !(InDevilSquare()))
		return nullptr;

	CHARACTER* c = nullptr;

	switch (iType)
	{
	case 310:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 98, PosX, PosY);
			c->Object.Scale = 1.2f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
		}
		break;
	case 311:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 97, PosX, PosY);
			c->Object.Scale = 1.2f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
		}
		break;
	case 312:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 96, PosX, PosY);
			c->Object.Scale = 1.2f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
		}
		break;
	case 313:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 95, PosX, PosY);
			c->Object.Scale = 1.25f;
			c->Object.SubType = 0;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
		}
		break;
	case 314:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 98, PosX, PosY);
			c->Object.Scale = 1.6f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
		}
		break;
	case 315:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 95, PosX, PosY);
			c->Object.Scale = 1.65f;
			c->Object.SubType = 1;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
		}
		break;
	case 316:
	case 344:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 91, PosX, PosY);
			c->Object.Scale = 1.25f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
		}
		break;
	case 317:
	case 341:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 94, PosX, PosY);
			c->Object.Scale = 1.3f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
		}
		break;
	case 349:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 89, PosX, PosY);
			c->Object.Scale = 2.0f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, c->Object.Position, c->Object.Position, c->Object.Angle, 2, &c->Object, 30.0f);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, c->Object.Position, c->Object.Position, c->Object.Angle, 3, &c->Object, 30.0f);
		}
		break;
	case 345:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 93, PosX, PosY);
			c->Object.Scale = 1.25f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
		}
		break;
	case 440:
	case 340:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 92, PosX, PosY);
			c->Object.Scale = 1.5f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
		}
		break;
	case 348:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 99, PosX, PosY, 180);
			c->Object.Scale = 1.0f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;

			if (PosY == 90)
			{
			}
			if (PosX == 62)
				c->Object.Angle[2] = 90.0f;
			if (PosX == 183)
				c->Object.Angle[2] = 90.0f;
		}
		break;
	}

	return c;
}

mu_boolean M34CryWolf1st::MoveCryWolf1stMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 98:
		{
			mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
			mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
			b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);
			EVector3 Light;
			if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3 EndPos, EndPos1, EndRelative;
				if (o->AnimationFrame >= 5.5f && o->AnimationFrame < (5.5f + fActionSpeed))
				{
					Vector(0.0f, 0.0f, 0.0f, EndRelative);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(24)], EndRelative, EndPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(16)], EndRelative, EndPos1, false);
					Vector(1.0f, 1.0f, 1.0f, Light);

					for (mu_int32 iu = 0; iu < 6; ++iu)
					{
						CreateEffect(ThreadIndex, MODEL_STONE2, EndPos, o->Angle, o->Light);
						CreateEffect(ThreadIndex, MODEL_STONE2, EndPos1, o->Angle, o->Light);
					}
				}
			}
			else
				if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
				{
					if (GetLargeRand(10) == 0)
					{
						CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, o->Position, o->Angle, Light);
					}
				}
		}
		break;
	case MODEL_MONSTER01 + 94:
		{
			mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
			EVector3 Light;
			EVector3 EndPos, EndRelative;
			Vector(1.0f, 1.0f, 1.0f, Light);


			if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->AnimationFrame >= 6.5f && o->AnimationFrame < (6.5f + fActionSpeed))
				{
					Vector(0.0f, 0.0f, 0.0f, EndRelative);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(27)], EndRelative, EndPos, true);
					CreateEffect(ThreadIndex, BITMAP_CRATER, EndPos, o->Angle, o->Light, 2);
					CreateParticle(ThreadIndex, BITMAP_BLUE_BLUR, EndPos, o->Angle, Light, 1);
					CreateParticle(ThreadIndex, BITMAP_BLUE_BLUR, EndPos, o->Angle, Light, 1);
					Vector(1.0f, 0.6f, 0.4f, Light);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);

					for (mu_int32 iu = 0; iu < 4; ++iu)
					{
						CreateEffect(ThreadIndex, MODEL_BIG_STONE1, EndPos, o->Angle, o->Light, 10);
						CreateEffect(ThreadIndex, MODEL_STONE2, EndPos, o->Angle, o->Light);
					}
				}
			}
			else
				if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
				{
					if (GetLargeRand(10) == 0)
					{
						CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, o->Position, o->Angle, Light);
					}
				}
		}
		break;
	case MODEL_MONSTER01 + 89:
		{
			MoveEye(ThreadIndex, o, b, 9, 10);
			mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
			mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
			b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);
			EVector3 Light;

			if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3 EndPos, EndRelative;
				if (o->AnimationFrame >= 7.5f && o->AnimationFrame < (7.5f + fActionSpeed))
				{
					Vector(0.0f, 0.0f, 0.0f, EndRelative);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(33)], EndRelative, EndPos, false);
					Vector(1.0f, 1.0f, 1.0f, Light);
					CreateEffect(ThreadIndex, BITMAP_CRATER, EndPos, o->Angle, o->Light, 2);
					CreateParticle(ThreadIndex, BITMAP_EXPLOTION, EndPos, o->Angle, Light, 2);
					for (mu_int32 iu = 0; iu < 6; ++iu)
						CreateEffect(ThreadIndex, MODEL_BIG_STONE1, EndPos, o->Angle, o->Light, 10);
				}
			}
			else
				if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
				{
					if (GetLargeRand(10) == 0) {
						CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, o->Position, o->Angle, Light);
					}
				}
		}
		break;
	case MODEL_MONSTER01 + 91:
		{
			EVector3 Light;
			Vector(0.9f, 0.2f, 0.1f, Light);
			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(10) == 0) {
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, o->Position, o->Angle, Light);
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 92:
		{
			if (m_CrywolfState == CRYWOLF_STATE_START
				&& Dark_Elf_Check)
			{
				Dark_Elf_Check = false;

				EVector3 Light;
				EVector3 EndPos, EndRelative;

				Vector(1.0f, 0.2f, 0.2f, Light);
				Vector(0.0f, 0.0f, 0.0f, EndRelative);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(27)], EndRelative, EndPos, true);
				CreateEffect(ThreadIndex, BITMAP_CRATER, EndPos, o->Angle, o->Light, 2);
				CreateParticle(ThreadIndex, BITMAP_BLUE_BLUR, EndPos, o->Angle, Light, 1);
				CreateParticle(ThreadIndex, BITMAP_BLUE_BLUR, EndPos, o->Angle, Light, 1);
				Vector(1.0f, 0.2f, 0.2f, Light);
				CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
				CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
				CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
				CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
				CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
			}
		}
		break;
	case MODEL_MONSTER01 + 93:
		{
			EVector3 Light;
			Vector(0.9f, 0.2f, 0.1f, Light);
			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(10) == 0) {
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, o->Position, o->Angle, Light);
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 95:
		{
			EVector3 Position, Light;

			if (o->CurrentAction != MONSTER01_DIE)
			{
				Vector(0.9f, 0.2f, 0.1f, Light);
				BoneManager::GetBonePosition(o, 6, Position);
				CreateSprite(BITMAP_LIGHT, Position, 3.5f, Light, o);
			}

			mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
			EVector3 EndPos, EndRelative;
			Vector(1.0f, 1.0f, 1.0f, Light);

			if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->AnimationFrame >= 2.5f && o->AnimationFrame < (2.5f + fActionSpeed))
				{
					Vector(0.0f, 0.0f, 100.0f, EndRelative);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(20)], EndRelative, EndPos, true);
					if (o->Scale > 1.25f)
					{
						CreateEffect(ThreadIndex, MODEL_SKILL_FURY_STRIKE, EndPos, o->Angle, Light, 1, o, -1, 0, 1);
					}
				}
			}
			else if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(10) == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, o->Position, o->Angle, Light);
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 97:
		{
			EVector3 Position, Light;

			if (o->CurrentAction == MONSTER01_ATTACK2 && o->AnimationFrame >= 4.0f && o->AnimationFrame <= 6.0f)
			{
				EBone Matrix;
				AngleMatrix(o->Angle, Matrix);
				VectorRotate(o->Direction, Matrix, Position);

				Vector(1.0f, 0.0f, 0.5f, Light);
				CreateEffect(ThreadIndex, MODEL_PIERCING2, o->Position, o->Angle, Light, 1);
			}
		}
		break;
	case MODEL_MONSTER01 + 96:
		{
			EVector3 Position, Light;

			mu_float Rotation = (mu_float)(GetLargeRand(360));
			mu_float Luminosity = EMath::Sin(FWorldTime*0.0012f)*0.8f + 1.3f;

			mu_float fScalePercent = 1.0f;
			if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
				fScalePercent = .5f;

			BoneManager::GetBonePosition(o, 28, Position);
			Vector(Luminosity*0.0f, Luminosity*0.5f, Luminosity*0.1f, Light);
			CreateSprite(BITMAP_LIGHT, Position, fScalePercent, Light, o);

			Vector(0.5f, 0.5f, 0.5f, Light);

			BoneManager::GetBonePosition(o, 27, Position);
			CreateSprite(BITMAP_SHINY + 1, Position, 0.5f*fScalePercent, Light, o, Rotation);
			CreateSprite(BITMAP_SHINY + 1, Position, 0.5f*fScalePercent, Light, o, 360.0f - Rotation);

			BoneManager::GetBonePosition(o, 29, Position);
			CreateSprite(BITMAP_SHINY + 1, Position, 0.5f*fScalePercent, Light, o, Rotation);
			CreateSprite(BITMAP_SHINY + 1, Position, 0.5f*fScalePercent, Light, o, 360.0f - Rotation);
		}
		break;
	case MODEL_MONSTER01 + 99:
		{
			if (m_CrywolfState == CRYWOLF_STATE_START)
			{
				if (GetLargeRand(100) == 0)
				{
					SetAction(o, MONSTER01_ATTACK1);
					o->Timer = 1;
				}

				if (o->CurrentAction == MONSTER01_ATTACK1 && o->Timer == 1 && o->AnimationFrame >= 5.0f)
				{
					CreateEffect(ThreadIndex, MODEL_ARROW_TANKER, o->Position, o->Angle, o->Light, 1, o, o->PKKey);
					o->Timer = 0;
					return true;
				}
			}
		}
		break;
	}
	return false;
}

mu_boolean M34CryWolf1st::AttackEffectCryWolf1stMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	OBJECT *to = nullptr;

	if (!IsCyrWolf1st() && !(InDevilSquare()))
		return false;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 96:
		{
			if (c->AttackTime == 14)
			{
				CreateEffect(ThreadIndex, MODEL_ARROW_NATURE, o->Position, o->Angle, o->Light, 1, o, o->PKKey);
				return true;
			}
		}
		break;
	case MODEL_MONSTER01 + 91:
		{
			if (c->AttackTime == 14)
			{
				CreateEffect(ThreadIndex, MODEL_ARROW_HOLY, o->Position, o->Angle, o->Light, 1, o, o->PKKey);
				return true;
			}
		}
		break;
	case MODEL_MONSTER01 + 42:
		{
			EVector3 Angle;
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
						for (mu_int32 i = 0; i < 18; ++i)
						{
							VectorCopy(o->Angle, Angle);
							Angle[2] += i*20.0f;
							CreateEffect(ThreadIndex, MODEL_STAFF + 8, o->Position, Angle, o->Light);
						}
					}
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 44:
		{
			EVector3 p(0.0f), Position, Angle;
			for (mu_int32 i = 0; i < 6; ++i)
			{
				mu_int32 Hand = 0;
				if (i >= 3) Hand = 1;
				b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[Hand].LinkBone], p, Position, true);
				Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);

				if (to != nullptr)
				{
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, to->Position, Angle, 2, to, 50.0f);
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, to->Position, Angle, 2, to, 10.0f);
				}
			}

			if (c->AttackTime == 1)
				PlayBackground(SOUND_EVIL);

			for (mu_int32 i = 0; i < 4; ++i)
			{
				mu_int32 Hand = 0;
				if (i >= 2) Hand = 1;
				b->TransformPosition(ThreadIndex, o->BoneTransform[c->Weapon[Hand].LinkBone], p, Position, true);
				Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
				if (to != nullptr)
					CreateJoint(ThreadIndex, BITMAP_JOINT_LASER + 1, Position, to->Position, Angle, 0, to, 50.0f);
				CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, o->Light);
			}
		}
		break;
	case MODEL_MONSTER01 + 99:
		{
			if (c->AttackTime == 15)
			{
				CreateEffect(ThreadIndex, MODEL_ARROW_TANKER, o->Position, o->Angle, o->Light, 1, o, o->PKKey);
				return true;
			}
		}
		break;
	}
	return false;
}

void M34CryWolf1st::MoveCryWolf1stBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 Angle, Position;
	EBone Matrix;
	EVector3  p, p2, EndPos;
	EVector3  TempAngle;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 94:
		{
			EVector3  Light;
			Vector(1.0f, 1.0f, 1.0f, Light);

			if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3 EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				VectorCopy(o->Angle, TempAngle);
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					Vector(0.0f, 100.0f, -150.0f, EndRelative);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(16)], EndRelative, EndPos, false);

					if (o->AnimationFrame > 5.0f && o->AnimationFrame < 7.0f)
					{
						CreateParticle(ThreadIndex, BITMAP_FIRE, EndPos, o->Angle, Light);
					}


					Vector(0.0f, -150.0f, 0.0f, p);
					AngleMatrix(o->Angle, Matrix);
					VectorRotate(p, Matrix, Position);
					VectorAdd(Position, o->Position, p2);

					o->Angle[2] -= 18;

					Vector((mu_float)(GetLargeRand(60) + 60 - 90), 0.0f, (mu_float)(GetLargeRand(30) + 90), Angle);
					VectorAdd(Angle, o->Angle, Angle);
					VectorCopy(p2, Position);

					fAnimationFrame += fSpeedPerFrame;
				}
				VectorCopy(TempAngle, o->Angle);
			}
		}
		break;
	case MODEL_MONSTER01 + 98:
		{
			if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				EVector3  Light;
				Vector(1.0f, 1.0f, 1.0f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					Vector(20.0f, 0.0f, 0.0f, StartRelative);
					Vector(60.0f, 0.0f, 0.0f, EndRelative);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(12)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(16)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 3, true, 23);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(20)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(24)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 3, true, 24);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 89:
		{
			if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				EVector3  Light;
				Vector(1.0f, 1.0f, 1.0f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;

				VectorCopy(o->Angle, TempAngle);
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					Vector(0.0f, 0.0f, -60.0f, StartRelative);
					Vector(0.0f, 0.0f, -150.0f, EndRelative);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[c->Weapon[0].LinkBone], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[c->Weapon[0].LinkBone], EndRelative, EndPos, false);

					if (o->AnimationFrame > 3.5f && o->AnimationFrame < 4.5f)
					{
						CreateParticle(ThreadIndex, BITMAP_BLUE_BLUR, EndPos, o->Angle, Light, 0);
					}
					Vector(0.0f, 0.0f, 5.0f, Light);
					CreateBlur(c, StartPos, EndPos, Light, 3, true, 80);

					fAnimationFrame += fSpeedPerFrame;
				}
				VectorCopy(TempAngle, o->Angle);
			}
		}
		break;

	case MODEL_MONSTER01 + 92:
		{
			if ((o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(1.0f, 1.0f, 1.0f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;

				if (o->CurrentAction == MONSTER01_ATTACK2 && (o->AnimationFrame > 4.5f && o->AnimationFrame < 5.0f))
					CreateEffect(ThreadIndex, MODEL_DARK_ELF_SKILL, o->Position, o->Angle, o->Light, 2, o);

				VectorCopy(o->Angle, TempAngle);
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					Vector(0.0f, 0.0f, -60.0f, StartRelative);
					Vector(0.0f, 0.0f, -150.0f, EndRelative);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[c->Weapon[0].LinkBone], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[c->Weapon[0].LinkBone], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 3, true, 80);


					fAnimationFrame += fSpeedPerFrame;
				}
				VectorCopy(TempAngle, o->Angle);
			}
			else if (o->CurrentAction == MONSTER01_ATTACK3)
			{

				EVector3 Position, Light;
				BoneManager::GetBonePosition(o, 17, Position);

				Vector(0.2f, 0.2f, 0.7f, Light);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 27, 1.0f);
				Vector(0.3f, 0.3f, 0.4f, Light);
				CreateParticle(ThreadIndex, BITMAP_LIGHT + 1, Position, o->Angle, Light, 2, 0.8f);
				CreateParticle(ThreadIndex, BITMAP_LIGHT + 1, Position, o->Angle, Light, 2, 0.6f);
			}
		}
		break;
	case MODEL_MONSTER01 + 93:
		{
			if ((o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(0.2f, 1.0f, 0.4f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;

				VectorCopy(o->Angle, TempAngle);
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					Vector(0.0f, 250.0f, 0.0f, StartRelative);
					Vector(0.0f, 0.0f, 0.0f, EndRelative);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(27)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(27)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 3, true, 80);


					fAnimationFrame += fSpeedPerFrame;
				}
				VectorCopy(TempAngle, o->Angle);

				if (o->AnimationFrame > 4.5f && o->AnimationFrame < 5.0f)
				{
					CHARACTER *tc = &CharactersClient[c->TargetCharacter];
					OBJECT *to = &tc->Object;
					EVector3 Angle = { 0.0f, 0.0f, o->Angle[2] };
					EVector3 Pos = { 0.0f, 0.0f, (to->BoundingBoxMax[2] / 1.0f) };

					Vector(80.0f, 0.0f, 20.0f, p);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
					Position[2] += 50.0f;
					Angle[2] = o->Angle[2] + 90;
					CreateEffect(ThreadIndex, MODEL_DEATH_SPI_SKILL, Position, Angle, Pos, 0, to);
					CreateEffect(ThreadIndex, MODEL_DEATH_SPI_SKILL, Position, Angle, Pos, 1, to);
					Angle[2] = o->Angle[2] - 90;
					CreateEffect(ThreadIndex, MODEL_DEATH_SPI_SKILL, Position, Angle, Pos, 0, to);
					CreateEffect(ThreadIndex, MODEL_DEATH_SPI_SKILL, Position, Angle, Pos, 1, to);
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 95:
		{
			if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3  Light;
				Vector(1.0f, 1.0f, 1.0f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					Vector(0.0f, 0.0f, -90.0f, StartRelative);
					Vector(0.0f, 0.0f, 0.0f, EndRelative);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(80)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(80)], EndRelative, EndPos, false);

					CreateBlur(c, StartPos, EndPos, Light, 3, true, 80);

					Vector(0.0f, 0.0f, 90.0f, StartRelative);
					Vector(0.0f, 0.0f, 0.0f, EndRelative);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(82)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(82)], EndRelative, EndPos, false);

					CreateBlur(c, StartPos, EndPos, Light, 3, true, 84);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 97:
		{
			if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				EVector3  Light;
				Vector(1.0f, 1.0f, 1.0f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					Vector(0.0f, 0.0f, 120.0f, StartRelative);
					Vector(0.0f, 0.0f, 0.0f, EndRelative);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(25)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(25)], EndRelative, EndPos, false);

					CreateBlur(c, StartPos, EndPos, Light, 3, true, 25);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	}
}

mu_boolean M34CryWolf1st::RenderCryWolf1stMonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_int32 ExtraMon)
{
	switch (o->Type)
	{
	case MODEL_MONSTER01 + 94:
		{
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 5);
		}
		return true;
	case MODEL_MONSTER01 + 89:
		{
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			for (mu_int32 i = 0; i < b->_MeshCount; ++i)
			{
				b->RenderMesh(ThreadIndex, i, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, i, RENDER_CHROME2 | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME3);
			}
		}
		return true;
	case MODEL_MONSTER01 + 91:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 5);
		}
		return true;
	case MODEL_MONSTER01 + 93:
		{
			b->StreamMesh(ThreadIndex) = 2;

			for (mu_int32 i = 0; i < b->_MeshCount; ++i)
			{
				if (i == 2)
				{
					if (o->Alpha > 0.3f)
						o->Alpha = 0.3f;

					o->BlendMesh = -2;
				}
				else
					o->BlendMesh = -1;

				o->BlendMeshTexCoordU = IWorldTime % 10000 * 0.0005f;
				b->RenderMesh(ThreadIndex, i, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}

			o->Alpha = 1.0f;
		}
		return true;
	case MODEL_MONSTER01 + 98:
		{
			if (ExtraMon)
			{
				Vector(0.5f, 0.5f, 0.6f, b->BodyLight(ThreadIndex));

				for (mu_int32 i = 0; i < b->_MeshCount; ++i)
				{
					b->RenderMesh(ThreadIndex, i, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
					b->RenderMesh(ThreadIndex, i, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
				}
			}
			else
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 5);
		}
		return true;
	case MODEL_MONSTER01 + 95:
		{
			if (ExtraMon)
			{
				Vector(0.5f, 0.5f, 0.8f, b->BodyLight(ThreadIndex));
				for (mu_int32 i = 0; i < b->_MeshCount; ++i)
				{
					b->RenderMesh(ThreadIndex, i, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
					b->RenderMesh(ThreadIndex, i, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
				}
			}
			else
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 5);
		}
		return true;
	case MODEL_MONSTER01 + 42:
		{
			Vector(0.6f, 0.8f, 0.6f, b->BodyLight(ThreadIndex));
			for (mu_int32 i = 0; i < b->_MeshCount; ++i)
			{
				b->RenderMesh(ThreadIndex, i, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, i, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
			}
		}
		return true;
	case MODEL_MONSTER01 + 43:
		{
			Vector(0.6f, 0.8f, 0.6f, b->BodyLight(ThreadIndex));
			for (mu_int32 i = 0; i < b->_MeshCount; ++i)
			{
				b->RenderMesh(ThreadIndex, i, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, i, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
			}
		}
		return true;
	case MODEL_MONSTER01 + 44:
		{
			Vector(0.6f, 0.8f, 0.6f, b->BodyLight(ThreadIndex));
			for (mu_int32 i = 0; i < b->_MeshCount; ++i)
			{
				b->RenderMesh(ThreadIndex, i, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, i, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
			}
		}
		return true;
	case MODEL_MONSTER01 + 31:
		{
			Vector(0.6f, 0.8f, 0.6f, b->BodyLight(ThreadIndex));
			for (mu_int32 i = 0; i < b->_MeshCount; ++i)
			{
				b->RenderMesh(ThreadIndex, i, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, i, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
			}
		}
		return true;
	}
	return false;
}

mu_boolean M34CryWolf1st::RenderCryWolf1stMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (!IsCyrWolf1st() && !(InDevilSquare()))
		return false;

	switch (o->Type)
	{
	case MODEL_CRYWOLF_ALTAR1:
	case MODEL_CRYWOLF_ALTAR2:
	case MODEL_CRYWOLF_ALTAR3:
	case MODEL_CRYWOLF_ALTAR4:
	case MODEL_CRYWOLF_ALTAR5:
		{
			EVector3 Light;
			mu_float fRotation1 = 0.0f, fRotation2 = 0.0f;
			mu_float Luminosity = EMath::Sin(FWorldTime*0.002f)*0.1f + 0.28f;
			mu_float Luminosity2 = EMath::Sin(FWorldTime*0.002f)*0.04f + 0.2f;

			if (g_isCharacterBuff(o, eBuff_CrywolfAltarContracted))
			{
				Vector(Luminosity2, Luminosity2, 0.05f, Light);
				fRotation1 = FWorldTime*0.01f;
				fRotation2 = -FWorldTime*0.01f;
				Vector(0.09f, 0.09f, 0.04f, o->Light);

				CreateParticle(ThreadIndex, BITMAP_EFFECT, o->Position, o->Angle, o->Light);
				CreateParticle(ThreadIndex, BITMAP_EFFECT, o->Position, o->Angle, o->Light, 1);
			}
			if (g_isCharacterBuff(o, eBuff_CrywolfAltarOccufied))
			{
				Vector(Luminosity, 0.1f, 0.1f, Light);
			}
			if (g_isCharacterBuff(o, eBuff_CrywolfAltarDisable))
			{
				Vector(Luminosity, 0.1f, 0.1f, Light);
			}
			if (g_isCharacterBuff(o, eBuff_CrywolfAltarEnable) || g_isCharacterBuff(o, eBuff_CrywolfAltarAttempt))
			{
				fRotation1 = FWorldTime*0.01f;
				Vector(0.15f, 0.15f, Luminosity, Light);
			}

			Vector(Light[0] * 2.0f, Light[1] * 2.0f, Light[2] * 2.0f, Light);

			RenderTerrainAlphaBitmap(BITMAP_MAGIC_CIRCLE, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 2.8f, 2.8f, Light, fRotation1, 1.0f, 5.0f);
			RenderTerrainAlphaBitmap(BITMAP_MAGIC_CIRCLE, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 2.8f, 2.8f, Light, fRotation2, 1.0f, 5.0f);
		}
		break;
	case MODEL_MONSTER01 + 89:
		{
			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_CRY1ST_BALGAS_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_BALGAS_ATTACK1);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_BALGAS_ATTACK2);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK3)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_BALGAS_SKILL1);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK4)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_BALGAS_SKILL2);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_BALGAS_DIE);
				}
			}

			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;
		}
		break;
	case MODEL_MONSTER01 + 92:
		{
			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_CRY1ST_DARKELF_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_DARKELF_ATTACK1);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_DARKELF_ATTACK2);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK3)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_DARKELF_SKILL1);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK4)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_DARKELF_SKILL2);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_DARKELF_DIE);
				}
			}

			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;
		}
		break;
	case MODEL_MONSTER01 + 99:
		{
			if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_TANKER_ATTACK1);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_TANKER_DIE);
				}
			}

			if (!(o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_DIE))
				o->SubType = 0;
		}
		break;
	case MODEL_MONSTER01 + 93:
		{
			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_CRY1ST_DEATHSPIRIT_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_DEATHSPIRIT_ATTACK1);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_DEATHSPIRIT_ATTACK2);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_DEATHSPIRIT_DIE);
				}
			}

			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;

			EVector3 Position, Light;
			mu_int32 dummy = GetLargeRand(14);
			mu_float Data = (mu_float)((mu_float)dummy / (mu_float)100);
			mu_float Rot = (mu_float)(GetLargeRand(360));
			Vector(1.0f, 1.0f, 1.0f, Light);
			BoneManager::GetBonePosition(o, 27, Position);
			CreateSprite(BITMAP_DS_EFFECT, Position, 1.5f, Light, o);
			Vector(0.3f, 0.3f, 0.7f, Light);
			CreateSprite(BITMAP_LIGHT, Position, 3.5f + (Data * 5), Light, o);
			CreateSprite(BITMAP_LIGHT, Position, 3.5f + (Data * 5), Light, o);
			CreateSprite(BITMAP_LIGHT, Position, 3.5f + (Data * 5), Light, o);
			CreateSprite(BITMAP_DS_SHOCK, Position, 0.8f + Data, Light, o, Rot);

			BoneManager::GetBonePosition(o, 28, Position);
			Vector(0.1f, 0.0f, 0.6f, Light);
			CreateSprite(BITMAP_SHINY + 1, Position, 0.8f, Light, o, Rot);
			if (GetLargeRand(2) == 0)
			{
				Vector(0.7f, 0.7f, 1.0f, Light);
				CreateSprite(BITMAP_SHINY + 1, Position, 0.8f, Light, o, 360.0f - Rot);
			}
			Vector(0.3f, 0.3f, 0.7f, Light);
			CreateSprite(BITMAP_LIGHT, Position, 1.5f, Light, o);
		}
		return true;
	case MODEL_MONSTER01 + 91:
		{
			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_CRY1ST_BALRAM_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_BALRAM_ATTACK1);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_BALRAM_ATTACK2);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_BALRAM_DIE);
				}
			}
			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;
		}
		return true;
	case MODEL_MONSTER01 + 94:
		{
			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_CRY1ST_SORAM_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_SORAM_ATTACK1);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_SORAM_ATTACK2);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_SORAM_DIE);
				}
			}
			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;
		}
		return true;
	case MODEL_MONSTER01 + 95:
		{
			EVector3 Position, Light;

			if (o->CurrentAction != MONSTER01_DIE)
			{
				Vector(0.9f, 0.2f, 0.1f, Light);
				BoneManager::GetBonePosition(o, 6, Position);
				CreateSprite(BITMAP_LIGHT, Position, 3.5f, Light, o);
			}

			Vector(0.9f, 0.2f, 0.1f, Light);

			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(10) == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, o->Position, o->Angle, Light);
				}
			}

			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_CRY1ST_WWOLF_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_WWOLF_ATTACK1);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_WWOLF_ATTACK2);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_WWOLF_DIE);
				}
			}
			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;
		}
		return true;
	case MODEL_MONSTER01 + 97:
		{
			EVector3 Position, Light;

			if (o->CurrentAction == MONSTER01_ATTACK2 && o->AnimationFrame >= 4.0f && o->AnimationFrame <= 6.0f)
			{
				EBone Matrix;
				AngleMatrix(o->Angle, Matrix);
				VectorRotate(o->Direction, Matrix, Position);

				Vector(1.0f, 0.0f, 0.5f, Light);
				CreateEffect(ThreadIndex, MODEL_PIERCING2, o->Position, o->Angle, Light, 1);

			}

			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_CRY1ST_SCOUT2_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_SCOUT2_ATTACK1);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_SCOUT2_ATTACK2);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_SCOUT2_DIE);
				}
			}
			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;
		}
		return true;
	case MODEL_MONSTER01 + 96:
		{
			EVector3 Position, Light;

			mu_float Rotation = (mu_float)(GetLargeRand(360));
			mu_float Luminosity = EMath::Sin(FWorldTime*0.0012f)*0.8f + 1.3f;

			mu_float fScalePercent = 1.0f;
			if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
				fScalePercent = .5f;

			BoneManager::GetBonePosition(o, 28, Position);
			Vector(Luminosity*0.0f, Luminosity*0.5f, Luminosity*0.1f, Light);
			CreateSprite(BITMAP_LIGHT, Position, fScalePercent, Light, o);

			Vector(0.5f, 0.5f, 0.5f, Light);

			BoneManager::GetBonePosition(o, 27, Position);
			CreateSprite(BITMAP_SHINY + 1, Position, 0.5f*fScalePercent, Light, o, Rotation);
			CreateSprite(BITMAP_SHINY + 1, Position, 0.5f*fScalePercent, Light, o, 360.0f - Rotation);

			BoneManager::GetBonePosition(o, 29, Position);
			CreateSprite(BITMAP_SHINY + 1, Position, 0.5f*fScalePercent, Light, o, Rotation);
			CreateSprite(BITMAP_SHINY + 1, Position, 0.5f*fScalePercent, Light, o, 360.0f - Rotation);

			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_CRY1ST_SCOUT3_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_SCOUT3_ATTACK1);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_SCOUT3_ATTACK2);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_SCOUT3_DIE);
				}
			}

			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;
		}
		return true;
	case MODEL_MONSTER01 + 98:
		{
			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_CRY1ST_SCOUT1_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_SCOUT1_ATTACK1);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_SCOUT1_ATTACK2);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_SCOUT1_DIE);
				}
			}
			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;
		}
		return true;
	}
	return false;
}

mu_boolean M34CryWolf1st::CreateMist(const mu_uint32 ThreadIndex, PARTICLE_HEADER *pParticleHeader, PARTICLE *pParticleObj)
{
	if (!IsCyrWolf1st())
		return false;

	if (GetLargeRand(30) == 0)
	{
		EVector3 Light;
		Vector(0.07f, 0.07f, 0.07f, Light);
		mu_int32 ff = 200.0f;

		EVector3 TargetPosition = { 0.0f, 0.0f, 400.0f }, TargetAngle = { 0.0f, 0.0f, 0.0f };
		switch (GetLargeRand(8))
		{
		case 0:
			TargetPosition[0] = Hero->Object.Position[0] + (300 - ff + GetLargeRand(250));
			TargetPosition[1] = Hero->Object.Position[1] + (300 - ff + GetLargeRand(250));
			break;
		case 1:
			TargetPosition[0] = Hero->Object.Position[0] + (250 - ff + GetLargeRand(250));
			TargetPosition[1] = Hero->Object.Position[1] - (250 - ff + GetLargeRand(250));
			break;
		case 2:
			TargetPosition[0] = Hero->Object.Position[0] - (200 - ff + GetLargeRand(250));
			TargetPosition[1] = Hero->Object.Position[1] + (200 - ff + GetLargeRand(250));
			break;
		case 3:
			TargetPosition[0] = Hero->Object.Position[0] - (300 - ff + GetLargeRand(250));
			TargetPosition[1] = Hero->Object.Position[1] - (300 - ff + GetLargeRand(250));
			break;
		case 4:
			TargetPosition[0] = Hero->Object.Position[0] + (400 - ff + GetLargeRand(250));
			TargetPosition[1] = Hero->Object.Position[1];
			break;
		case 5:
			TargetPosition[0] = Hero->Object.Position[0] - (400 - ff + GetLargeRand(250));
			TargetPosition[1] = Hero->Object.Position[1];
			break;
		case 6:
			TargetPosition[0] = Hero->Object.Position[0];
			TargetPosition[1] = Hero->Object.Position[1] + (400 - ff + GetLargeRand(250));
			break;
		case 7:
			TargetPosition[0] = Hero->Object.Position[0];
			TargetPosition[1] = Hero->Object.Position[1] - (400 - ff + GetLargeRand(250));
			break;
		}

		if (Hero->Movement)
		{
			EBone Matrix;
			AngleMatrix(Hero->Object.Angle, Matrix);
			EVector3 Velocity, Direction;
			Vector(0.0f, -45.0f*CharacterMoveSpeed(Hero), 0.0f, Velocity);
			VectorRotate(Velocity, Matrix, Direction);
			VectorAdd(TargetPosition, Direction, TargetPosition);
		}
		if (Hero->Movement || (GetLargeRand(2) == 0))
		{
			CreateParticle(ThreadIndex, BITMAP_CLOUD, TargetPosition, TargetAngle, Light, 8, 0.4f);
		}
		pParticleHeader->Flags.Clear(PARTICLE::ePF_LIVE);

		return true;
	}

	return false;
}

mu_boolean M34CryWolf1st::Get_State()
{
	if(m_CrywolfState == CRYWOLF_STATE_START)
		return true;

	return false;
}

mu_boolean M34CryWolf1st::Get_State_Only_Elf()
{
	if(m_CrywolfState == CRYWOLF_STATE_START || m_CrywolfState == CRYWOLF_STATE_READY)
		return true;
	return false;
}

void M34CryWolf1st::SetTime ( mu_uint8 byHour, mu_uint8 byMinute )
{
	if(M34CryWolf1st::Get_State() == false || M34CryWolf1st::IsCyrWolf1st() == false)
		return;
    m_iHour     = byHour;
    m_iMinute   = byMinute;
	m_dwSyncTime= g_CurrentTime;

	if(TimeStart == false)
		TimeStart = true;

}

void M34CryWolf1st::Set_BossMonster(mu_int32 Val_Hp,mu_int32 Dl_Num)
{
	if(M34CryWolf1st::Get_State_Only_Elf() == false || M34CryWolf1st::IsCyrWolf1st() == false)
		return;
	Dark_elf_Num = Dl_Num;

	if(Val_Hp > 0 && View_Bal == false)
		View_Bal = true;
	else
	if(Val_Hp <= 0 && View_Bal == true)
		View_Bal = false;

	Set_Val_Hp(Val_Hp);
}

void M34CryWolf1st::Check_AltarState(mu_int32 Num,mu_int32 State)
{
	if(M34CryWolf1st::Get_State_Only_Elf() == false || M34CryWolf1st::IsCyrWolf1st() == false)
		return;
	m_AltarState[Num - 1] = State;
}

void M34CryWolf1st::Set_Message_Box(mu_int32 Str,mu_int32 Num,mu_int32 Key,mu_int32 ObjNum)
{
	if(M34CryWolf1st::Get_State_Only_Elf() == false || M34CryWolf1st::IsCyrWolf1st() == false || LogOut)
		return;

	if(Str== 56)
	{
		mu_uint8 State = (m_AltarState[ObjNum] & 0x0f);
		mu_sprintf_s(Box_String[Num], mu_countof(Box_String[0]), GlobalText[1950+Str],State);
	}
	else
		mu_strcpy(Box_String[Num], mu_countof(Box_String[0]), GlobalText[1950+Str]);
	if(Str == 56 || Str == 57)
		Message_Box = 1;
	else
		Message_Box = 2;
	if(Num == 0)
		Box_String[Num+1][0] = _T('\0');

	BackUp_Key = Key;
}

void M34CryWolf1st::Set_Val_Hp(mu_int32 State)
{
	Val_Hp = State;
}

mu_boolean M34CryWolf1st::Get_AltarState_State(mu_int32 Num)
{
	mu_uint8 Use = (m_AltarState[Num] & 0xf0)>>4;

	if(Use == CRYWOLF_ALTAR_STATE_CONTRACTED)
		return true;
	else
		return false;
}

mu_boolean M34CryWolf1st::SetCurrentActionCrywolfMonster(CHARACTER* c, OBJECT* o)
{
	if(!IsCyrWolf1st() && !(InDevilSquare()))
		return false;

	switch(c->MonsterIndex)
	{
	case 440:
	case 340:
	case 344:
	case 345:
	case 341:
	case 349:
		return CheckMonsterSkill(c, o);
	}
	return false;
}

void M34CryWolf1st::Set_MyRank(mu_uint8 MyRank,mu_int32 GettingExp)
{
	Rank = MyRank;
	Exp = GettingExp;
}

void M34CryWolf1st::Set_WorldRank(mu_uint8 Rank,mu_uint8 Class,mu_int32 Score,mu_text *szHeroName)
{
	HeroScore[Rank] = Score;
	HeroClass[Rank] = Class;
	HeroName[Rank] = szHeroName;
}

void M34CryWolf1st::PlayBGM()
{
	if (GET_WORLD == WD_34CRYWOLF_1ST)
	{
		if (m_CrywolfState == CRYWOLF_STATE_NOTIFY_2)
		{
			EMusicBackend::Stop(MUSICS::Data[MUSIC_BC_CRYWOLF_1ST]);
			EMusicBackend::Play(MUSICS::Data[MUSIC_CRYWOLF_BEFORE]);
		}
		else if (m_CrywolfState == CRYWOLF_STATE_READY)
		{
			EMusicBackend::Stop(MUSICS::Data[MUSIC_CRYWOLF_BEFORE]);
			EMusicBackend::Play(MUSICS::Data[MUSIC_CRYWOLF_READY]);
		}
		else if (m_CrywolfState == CRYWOLF_STATE_START)
		{
			EMusicBackend::Stop(MUSICS::Data[MUSIC_CRYWOLF_READY]);
			EMusicBackend::Play(MUSICS::Data[MUSIC_CRYWOLF_BACK]);
		}
		else if (m_CrywolfState == CRYWOLF_STATE_END)
		{
			EMusicBackend::Stop(MUSICS::Data[MUSIC_CRYWOLF_READY]);
			EMusicBackend::Stop(MUSICS::Data[MUSIC_CRYWOLF_BACK]);

			if (Add_Num == 10)
				PlayBackground(SOUND_CRY1ST_FAILED);
			else
				PlayBackground(SOUND_CRY1ST_SUCCESS);
		}
		else
		{
			EMusicBackend::Play(MUSICS::Data[MUSIC_BC_CRYWOLF_1ST]);
		}
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_BC_CRYWOLF_1ST]);
	}
}