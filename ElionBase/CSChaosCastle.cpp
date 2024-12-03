#include "stdafx.h"
#include "CSChaosCastle.h"

const mu_int32 g_iChaosCastleLimitArea1[16] = { 23, 75, 44, 76, 43, 77, 44, 108, 23, 107, 42, 108, 23, 77, 24, 106 };
const mu_int32 g_iChaosCastleLimitArea2[16] = { 25, 77, 42, 78, 41, 79, 42, 106, 25, 105, 40, 106, 25, 79, 26, 104 };
const mu_int32 g_iChaosCastleLimitArea3[16] = { 27, 79, 40, 80, 39, 81, 40, 104, 27, 103, 38, 104, 27, 81, 28, 102 };
mu_uint8 g_byCurrCastleLevel = 255;
mu_boolean g_bActionMatch = true;

EParallelVariable<mu_boolean, MAX_TASKING_THREADS> g_ClearActionObject(false);

mu_boolean g_ActionSmoke = true;

void ClearActionObject(const mu_uint32 ThreadIndex)
{
	g_ActionObjectType[ThreadIndex] = -1;
	g_ActionWorld[ThreadIndex] = -1;
	g_ActionTime[ThreadIndex] = -1;
}

void SetActionObject(mu_int32 iWorld, mu_int32 iType, mu_int32 iLifeTime, mu_int32 iVel)
{
	g_ActionWorld.SetAll(iWorld);
	g_ActionObjectType.SetAll(iType);
	g_ActionTime.SetAll(iLifeTime);
	g_ActionObjectVelocity.SetAll((mu_float)iVel);

	g_ActionSmoke = false;
}

void PostProcessActionObject(const mu_uint32 ThreadIndex)
{
	if (g_ClearActionObject[ThreadIndex] == true)
	{
		g_ClearActionObject[ThreadIndex] = false;
		ClearActionObject(ThreadIndex);
	}

	if(GET_WORLD == g_ActionWorld[ThreadIndex] &&
		InChaosCastle() == true)
	{
		g_ActionObjectVelocity[ThreadIndex] += 0.4f + (g_ActionObjectVelocity[ThreadIndex] * 0.01f);
	}

	if (g_ActionTime[ThreadIndex] > 0)
	{
		--g_ActionTime[ThreadIndex];
	}
}

void ActionObject(const mu_uint32 ThreadIndex, OBJECT* o)
{
	if (g_ActionWorld[ThreadIndex] < 0)     return;
	if (g_ActionObjectType[ThreadIndex] < 0)return;
	if (g_ActionTime[ThreadIndex] < 0)      return;

	if (GET_WORLD == g_ActionWorld[ThreadIndex])
	{
		if (InChaosCastle() == true)
		{
			EVector3 Position;

			switch (o->Type)
			{
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
				if (g_byCurrCastleLevel == 7)
				{
					if (g_ActionTime[ThreadIndex] >= 30)
					{
						EVector3 Light = { 1.0f, 1.0f, 1.0f };

						Position[0] = o->Position[0] + (GetLargeRand(300) - 150.0f);
						Position[1] = o->Position[1];
						Position[2] = Hero->Object.Position[2];
						CreateParticle(ThreadIndex, BITMAP_SMOKE + 4, Position, o->Angle, Light, 0, 1.5f);

						EarthQuake.Store((mu_float)(GetLargeRand(3) - 3)*0.1f);
					}
					else if (g_ActionTime[ThreadIndex] <= 0)
					{
						o->HiddenMesh = -2;
						g_byCurrCastleLevel = 8;

						g_ClearActionObject[ThreadIndex] = true;
					}
					else
					{
						o->Position[2] = o->StartPosition[2] - ((30 - g_ActionTime[ThreadIndex]) * g_ActionObjectVelocity[ThreadIndex]);
					}
				}
				break;

			case 30:
			case 31:
			case 32:
			case 33:
			case 34:
			case 35:
				if (g_byCurrCastleLevel == 4)
				{
					if (g_ActionTime[ThreadIndex] >= 30)
					{
						EVector3 Light = { 1.0f, 1.0f, 1.0f };

						Position[0] = o->Position[0] + (GetLargeRand(300) - 150.0f);
						Position[1] = o->Position[1];
						Position[2] = Hero->Object.Position[2];
						CreateParticle(ThreadIndex, BITMAP_SMOKE + 4, Position, o->Angle, Light, 0, 1.5f);

						EarthQuake.Store((mu_float)(GetLargeRand(3) - 3)*0.1f);
					}
					else if (g_ActionTime[ThreadIndex] <= 0)
					{
						o->HiddenMesh = -2;
						g_byCurrCastleLevel = 5;

						g_ClearActionObject[ThreadIndex] = true;
					}
					else
					{
						o->Position[2] = o->StartPosition[2] - ((30 - g_ActionTime[ThreadIndex]) * g_ActionObjectVelocity[ThreadIndex]);
					}
				}
				break;

			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
				if (g_byCurrCastleLevel == 1)
				{
					if (g_ActionTime[ThreadIndex] >= 30)
					{
						EVector3 Light = { 1.0f, 1.0f, 1.0f };

						Position[0] = o->Position[0] + (GetLargeRand(300) - 150.0f);
						Position[1] = o->Position[1];
						Position[2] = Hero->Object.Position[2];
						CreateParticle(ThreadIndex, BITMAP_SMOKE + 4, Position, o->Angle, Light, 0, 1.5f);

						EarthQuake.Store((mu_float)(GetLargeRand(3) - 3)*0.1f);
					}
					else if (g_ActionTime[ThreadIndex] <= 0)
					{
						o->HiddenMesh = -2;
						g_byCurrCastleLevel = 2;

						g_ClearActionObject[ThreadIndex] = true;
					}
					else
					{
						o->Position[2] = o->StartPosition[2] - ((30 - g_ActionTime[ThreadIndex]) * g_ActionObjectVelocity[ThreadIndex]);
					}
				}
				break;
			}
		}
		else
		{
			EVector3 Position;

			if (o->Type == 9)
			{
				if (g_ActionTime[ThreadIndex] == 0)
				{
					o->HiddenMesh = -1;
					o->PKKey = 4;
				}
			}
			else if (o->Type == 10)
			{
				if (g_ActionTime[ThreadIndex] == 0)
				{
					o->HiddenMesh = -1;
					o->PKKey = 4;
				}
			}
			else if (o->Type == g_ActionObjectType[ThreadIndex]) // Threadsafe since it is used only for blood castle and there is only ONE gate
			{
				if (g_ActionTime[ThreadIndex] == 20)
				{
					o->Angle[0] = 35.0f;
					o->HiddenMesh = -1;

					PlayBackground(SOUND_DOWN_GATE);
				}

				if (g_ActionTime[ThreadIndex] >= 0)
				{
					const mu_float actionVelocity = g_ActionObjectVelocity[ThreadIndex];
					o->Angle[0] += actionVelocity;

					if (o->Angle[0] >= 90.0f)
					{
						o->Angle[0] -= g_ActionTime[ThreadIndex];
						g_ActionObjectVelocity.SetAll(2.0f);

						VectorCopy(o->Position, Position);

						if (o->Angle[0] <= 82.0f && g_ActionSmoke == false)
						{
							g_ActionSmoke = true;
							for (mu_int32 i = 0; i < 10; ++i)
							{
								Position[0] = o->Position[0] + (GetLargeRand(300) - 150.0f);
								Position[1] = o->Position[1] - (GetLargeRand(20) + 600.0f);
								CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, Position, o->Angle, o->Light);
							}
						}
					}
					else
					{
						g_ActionObjectVelocity.SetAll(actionVelocity + 1.5f);
					}

					if (g_ActionTime[ThreadIndex] == 0)
					{
						o->HiddenMesh = -2;
						o->Angle[0] = 90.0f;
						g_ClearActionObject.SetAll(true);

						if (InBloodCastle() == true)
						{
							g_Terrain.Load(GET_WORLD, 1, false);

							static mu_int32 g_btCastleEntranceMapXY[4] =
							{
								13, 15, 15, 23,
							};
							mu_int32 dx = g_btCastleEntranceMapXY[2] - g_btCastleEntranceMapXY[0] + 1;
							mu_int32 dy = g_btCastleEntranceMapXY[3] - g_btCastleEntranceMapXY[1] + 1;

							AddTerrainAttributeRange(g_btCastleEntranceMapXY[0], g_btCastleEntranceMapXY[1], dx, dy, TW_NOMOVE, false);
						}

						AddTerrainAttributeRange(13, 70, 3, 6, TW_NOGROUND, false);
					}
				}
			}
		}
	}
}

void ClearChaosCastleHelper(CHARACTER* c)
{
	c->Wing.Type = -1;
	c->Wing.Level = 0;
	c->Helper.Type = -1;
	c->Helper.Level = 0;
	c->Weapon[0].Type = -1;
	c->Weapon[0].Level = 0;
	c->Weapon[0].ExcOption = 0;
	c->Weapon[0].SetOption = 0;
	c->Weapon[1].Type = -1;
	c->Weapon[1].Level = 0;
	c->Weapon[1].ExcOption = 0;
	c->Weapon[1].SetOption = 0;
}

void ChangeChaosCastleUnit(CHARACTER* c)
{
	if (InChaosCastle() == false) return;

	ClearChaosCastleHelper(c);

#if 0 // FIX
	mu_uint32 t_dwUIID = g_pWindowMgr->GetAddFriendWindow();
	if (t_dwUIID != 0)
	{
		g_pWindowMgr->SendUIMessage(UI_MESSAGE_CLOSE, t_dwUIID, 0);
	}
	if (g_pUIManager->IsOpen(INTERFACE_FRIEND))
	{
		CUIFriendWindow* t_pFW = g_pWindowMgr->GetFriendMainWindow();
		t_pFW->Close();
	}
#endif

	mu_int32 Class = GetBaseClass(c->Class);

	if (Class == GAME::CLASS_KNIGHT || Class == GAME::CLASS_DARK || Class == GAME::CLASS_DARK_LORD)
	{
		c->Weapon[0].Type = MODEL_SWORD + 16;
		c->Weapon[0].Level = 0;
		c->Weapon[1].Type = MODEL_SWORD + 16;
		c->Weapon[1].Level = 0;
	}
	else if (Class == GAME::CLASS_ELF)
	{
		c->Weapon[0].Type = MODEL_BOW + 19;
		c->Weapon[0].Level = 0;
	}
	else if (Class == GAME::CLASS_WIZARD || Class == GAME::CLASS_SUMMONER)
	{
		c->Weapon[0].Type = MODEL_STAFF + 5;
		c->Weapon[0].Level = 0;
	}
}

void MoveChaosCastleObject(OBJECT* o)
{
	if (o->Type == 3)
	{
		if (g_bActionMatch)
		{
			o->LifeTime = 10;
			o->PKKey = 1;
			g_bActionMatch = false;
		}
		else
		{
			o->LifeTime = 10;
			o->PKKey = 0;
		}
	}
}

void CreateChaosCastleObject(mu_uint32 objectIndex, OBJECT* o)
{
	switch (o->Type)
	{
	case 18:
	case 19:
	case 20:
	case 21:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
		o->HiddenMesh = -2;
		VectorCopy(o->Position, o->StartPosition);
		break;
	}
}

void RenderChaosCastleVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 p, Position;

	switch (o->Type)
	{
	case 6:
		if (o->HiddenMesh != -2)
		{
			EVector3  Light;
			Vector(0.05f, 0.05f, 0.1f, Light);
			for (mu_int32 i = 0; i < 10; ++i)
			{
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 0, o->Scale, o);
			}
		}
		o->HiddenMesh = -2;
		break;

	case 7:
		if (o->HiddenMesh != -2)
		{
			EVector3  Light;
			Vector(0.05f, 0.05f, 0.1f, Light);
			for (mu_int32 i = 0; i < 10; ++i)
			{
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 1, o->Scale, o);
			}
		}
		o->HiddenMesh = -2;
		break;

	case 8:
		if (o->HiddenMesh != -2)
		{
			EVector3  Light;
			Vector(0.05f, 0.05f, 0.1f, Light);
			for (mu_int32 i = 0; i < 10; ++i)
			{
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 2, o->Scale, o);
			}
		}
		o->HiddenMesh = -2;
		break;

	case 9:
		if (o->HiddenMesh != -2)
		{
			EVector3  Light;
			Vector(0.05f, 0.05f, 0.1f, Light);
			for (mu_int32 i = 0; i < 5; ++i)
			{
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 3, o->Scale, o);
			}
		}
		o->HiddenMesh = -2;
		break;

	case 10:
		if (o->HiddenMesh != -2)
		{
			EVector3  Light;
			Vector(0.05f, 0.05f, 0.1f, Light);
			for (mu_int32 i = 0; i < 5; ++i)
			{
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 4, o->Scale, o);
			}
		}
		o->HiddenMesh = -2;
		break;

	case 11:
		if (o->HiddenMesh != -2)
		{
			EVector3  Light;
			Vector(0.05f, 0.05f, 0.1f, Light);
			for (mu_int32 i = 0; i < 5; ++i)
			{
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 5, o->Scale, o);
			}
		}
		o->HiddenMesh = -2;
		break;

	case 12:
		if (o->HiddenMesh != -2)
		{
			EVector3  Light;
			Vector(0.3f, 0.3f, 0.3f, Light);
			for (mu_int32 i = 0; i < 7; ++i)
			{
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 7, o->Scale, o);
			}
		}
		o->HiddenMesh = -2;
		break;

	case 18:
	case 19:
	case 20:
	case 21:
		if (g_byCurrCastleLevel == 7 || g_byCurrCastleLevel == 8)
		{
			o->HiddenMesh = -1;
		}
		else
		{
			o->HiddenMesh = -2;
		}
		break;

	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
		if (g_byCurrCastleLevel == 4 || g_byCurrCastleLevel == 5)
		{
			o->HiddenMesh = -1;
		}
		else if (g_byCurrCastleLevel >= 8)
		{
			o->HiddenMesh = -2;
		}
		break;

	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
		if (g_byCurrCastleLevel == 1 || g_byCurrCastleLevel == 2)
		{
			o->HiddenMesh = -1;
		}
		else if (g_byCurrCastleLevel >= 5)
		{
			o->HiddenMesh = -2;
		}
		break;

	case 0:
	case 1:
	case 2:
	case 3:
		if (o->PKKey && o->HiddenMesh != -2)
		{
			Vector(0.0f, 0.0f, 0.0f, p);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(1)], p, Position);
			if (o->LifeTime == 10)
			{
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, Position, Position, o->Angle, 2, nullptr, 60.0f + GetLargeRand(10));

				mu_int32 randValue = GetLargeRand(2);
				PlayBackground(SOUND_CHAOS_THUNDER01 + randValue);
			}

			if (o->LifeTime < 5)
			{
				o->PKKey = 0;
			}
			else
			{
				--o->LifeTime;
			}
		}
	case 4:
	case 5:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
		if (g_byCurrCastleLevel >= 2 && g_byCurrCastleLevel != 255)
		{
			o->HiddenMesh = -2;
		}
		break;
	}
}

void RenderChaosCastleVisual(const mu_uint32 ThreadIndex)
{
	if (InChaosCastle() == false || GetLargeRand(8) != 0) return;

	const mu_int32* Area = nullptr;

	if (g_byCurrCastleLevel == 0)
	{
		Area = &g_iChaosCastleLimitArea1[0];
	}
	else if (g_byCurrCastleLevel == 3)
	{
		Area = &g_iChaosCastleLimitArea2[0];
	}
	else if (g_byCurrCastleLevel == 6)
	{
		Area = &g_iChaosCastleLimitArea3[0];
	}
	else
	{
		return;
	}

	EVector3 Light = { 1.0f, 1.0f, 1.0f };
	EVector3 Angle = { 0.0f, 0.0f, 0.0f };

	for (mu_int32 i = 0; i < 4; ++i)
	{
		for (mu_int32 yi = Area[1]; yi <= Area[3]; ++yi)
		{
			for (mu_int32 xi = Area[0]; xi <= Area[2]; ++xi)
			{
				EVector3 Position;

				Position[0] = (xi*g_TerrainScale) + (GetLargeRand(30) - 15.0f);
				Position[1] = (yi*g_TerrainScale) + (GetLargeRand(30) - 15.0f);
				Position[2] = Hero->Object.Position[2];
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 4, Position, Angle, Light, 0, 1.5f);

				if (GetLargeRand(5) == 0)
				{
					EarthQuake.Store((mu_float)(GetLargeRand(3) - 3)*0.1f);
				}
			}
		}

		Area += 4;
	}
}

void SetCurrCastleLevel(mu_uint8 byCurrCastleLevel)
{
	g_byCurrCastleLevel = byCurrCastleLevel;
}

void SetActionMatch(mu_boolean bActionMatch)
{
	g_bActionMatch = bActionMatch;
}