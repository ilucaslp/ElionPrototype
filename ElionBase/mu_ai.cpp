#include "stdafx.h"
#include "mu_ai.h"

mu_uint8 GetDestValue(mu_int32 xPos, mu_int32 yPos, mu_int32 xDst, mu_int32 yDst)
{
	mu_int32 DestX = xDst - xPos;
	mu_int32 DestY = yDst - yPos;
	if (DestX < -8) DestX = -8;
	if (DestX > 7) DestX = 7;
	if (DestY < -8) DestY = -8;
	if (DestY > 7) DestY = 7;

	mu_uint8 byValue1 = ((mu_uint8)(DestX + 8)) << 4;
	mu_uint8 byValue2 = ((mu_uint8)(DestY + 8)) & 0xf;
	return (byValue1 | byValue2);
}

mu_uint8 CalcTargetPos(mu_float x, mu_float y, mu_int32 Tx, mu_int32 Ty)
{
	mu_uint8 position;
	mu_int32 PositionX = (mu_int32)(x * g_TerrainScaleRev);
	mu_int32 PositionY = (mu_int32)(y * g_TerrainScaleRev);
	mu_int32 TargetX = (mu_int32)(Tx * g_TerrainScaleRev);
	mu_int32 TargetY = (mu_int32)(Ty * g_TerrainScaleRev);

	mu_uint8 dx = 8 + TargetX - PositionX;
	mu_uint8 dy = 8 + TargetY - PositionY;

	position = ((mu_uint8)((mu_uint8)dx | (mu_uint8)(dy << 4)));

	return position;
}

mu_boolean IsGMCharacter()
{
	if ((Hero->Object.Kind == KIND_PLAYER &&
		Hero->Object.Type == MODEL_PLAYER &&
		Hero->Object.SubType == MODEL_GM_CHARACTER)
		|| (g_isCharacterBuff((&Hero->Object), eBuff_GMEffect))
		|| (Hero->CtlCode & CTLCODE_EVENT_GM)
		|| (Hero->CtlCode & CTLCODE_ADMIN))
	{
		return true;
	}

	return false;
}

mu_boolean CheckMouseInX(mu_int32 x, mu_int32 width)
{
	mu_int32 MouseX = MU_INPUT->GetMouseX();

	if (MouseX >= x && MouseX < x + width)
		return true;
	return false;
}

mu_boolean CheckMouseInY(mu_int32 y, mu_int32 height)
{
	mu_int32 MouseY = MU_INPUT->GetMouseY();

	if (MouseY >= y && MouseY < y + height)
		return true;
	return false;
}

mu_boolean CheckMouseIn(mu_int32 x, mu_int32 y, mu_int32 width, mu_int32 height)
{
	mu_int32 MouseX = MU_INPUT->GetMouseX();
	mu_int32 MouseY = MU_INPUT->GetMouseY();

	if (MouseX >= x && MouseX < x + width && MouseY >= y && MouseY < y + height)
		return true;
	return false;
}

mu_boolean CheckMouseIn2(mu_int32 x, mu_int32 y, mu_int32 width, mu_int32 height, mu_float aspectWidth = MU_RESOURCE->_640x480Rate[0], mu_float aspectHeight = MU_RESOURCE->_640x480Rate[1])
{
	x = MU_TO_WINDOW(x, aspectWidth);
	y = MU_TO_WINDOW(y, aspectHeight);
	width = MU_TO_WINDOW(width, aspectWidth);
	height = MU_TO_WINDOW(height, aspectHeight);

	mu_int32 MouseX = MU_INPUT->GetMouseX();
	mu_int32 MouseY = MU_INPUT->GetMouseY();

	if (MouseX >= x && MouseX < x + width && MouseY >= y && MouseY < y + height)
		return true;
	return false;
}

mu_boolean TestFrustrum2D(mu_float posx, mu_float posy, mu_float Range)
{
	mu_float absRange = EMath::Abs(Range);

	const mu_float BLOCK_DIVISOR = (g_TerrainScale * 4.0f);

	mu_float beginX = (posx - absRange) - EMath::Mod((posx - absRange), BLOCK_DIVISOR) - BLOCK_DIVISOR;
	mu_float beginY = (posy - absRange) - EMath::Mod((posy - absRange), BLOCK_DIVISOR) - BLOCK_DIVISOR;
	mu_float endX = (posx + absRange) + (BLOCK_DIVISOR - EMath::Mod((posx + absRange), BLOCK_DIVISOR)) + BLOCK_DIVISOR;
	mu_float endY = (posy + absRange) + (BLOCK_DIVISOR - EMath::Mod((posy + absRange), BLOCK_DIVISOR)) + BLOCK_DIVISOR;

	if (beginX < 0.0f)
		beginX = 0.0f;
	if (beginY < 0.0f)
		beginY = 0.0f;
	if (endX > TERRAIN_SIZE * g_TerrainScale)
		endX = TERRAIN_SIZE * g_TerrainScale;
	if (endY > TERRAIN_SIZE * g_TerrainScale)
		endY = TERRAIN_SIZE * g_TerrainScale;

	mu_uint32 bx = beginX / BLOCK_DIVISOR, by = beginY / BLOCK_DIVISOR;
	mu_int32 ex = endX / BLOCK_DIVISOR, ey = endY / BLOCK_DIVISOR;
	mu_int32 xCount = ex - bx;
	for (mu_int32 y = by; y < ey; ++y)
	{
		mu_int32 index = y * CPUCulling::BLOCK_SIZE;
		if (mu_memcmp(g_FastCullingCheck.data(), &g_CulledBlockVisible[index + bx], xCount) != 0)
		{
			return true;
		}
	}

	return false;
}

mu_boolean TestFrustrum2D(EVector3 Position, mu_float Range)
{
#if 0
	EMathAABB aabb(Position, Range);
	mu_boolean isIncluded = EMath::Contains(g_CpuCulling._Frustum, aabb) != EMath::FR_EXCLUDED;

	return isIncluded;
#else
	mu_float absRange = EMath::Abs(Range);

	const mu_float BLOCK_DIVISOR = (g_TerrainScale * 4.0f);

	mu_float beginX = (Position.x - absRange) - EMath::Mod((Position.x - absRange), BLOCK_DIVISOR) - BLOCK_DIVISOR;
	mu_float beginY = (Position.y - absRange) - EMath::Mod((Position.y - absRange), BLOCK_DIVISOR) - BLOCK_DIVISOR;
	mu_float endX = (Position.x + absRange) + (BLOCK_DIVISOR - EMath::Mod((Position.x + absRange), BLOCK_DIVISOR)) + BLOCK_DIVISOR;
	mu_float endY = (Position.y + absRange) + (BLOCK_DIVISOR - EMath::Mod((Position.y + absRange), BLOCK_DIVISOR)) + BLOCK_DIVISOR;

	if (beginX < 0.0f)
		beginX = 0.0f;
	if (beginY < 0.0f)
		beginY = 0.0f;
	if (endX > TERRAIN_SIZE * g_TerrainScale)
		endX = TERRAIN_SIZE * g_TerrainScale;
	if (endY > TERRAIN_SIZE * g_TerrainScale)
		endY = TERRAIN_SIZE * g_TerrainScale;

	mu_uint32 bx = beginX / BLOCK_DIVISOR, by = beginY / BLOCK_DIVISOR;
	mu_int32 ex = endX / BLOCK_DIVISOR, ey = endY / BLOCK_DIVISOR;
	mu_int32 xCount = ex - bx;
	for (mu_int32 y = by; y < ey; ++y)
	{
		mu_int32 index = y * CPUCulling::BLOCK_SIZE;
		if (mu_memcmp(g_FastCullingCheck.data(), &g_CulledBlockVisible[index + bx], xCount) != 0)
		{
			return true;
		}
	}

	return false;
#endif
}

mu_float CreateAngle(mu_float x1, mu_float y1, mu_float x2, mu_float y2)
{
	mu_float nx2 = x2 - x1, ny2 = y2 - y1;
	mu_float r, angle;

	if (EMath::Abs(nx2) < 0.0001f)
	{
		if (ny2 < 0.0f)
		{
			return 0.0f;
		}
		else
		{
			return 180.0f;
		}
	}
	else if (EMath::Abs(ny2) < 0.0001f)
	{
		if (nx2 < 0.0f)
		{
			return 270.0f;
		}
		else
		{
			return 90.0f;
		}
	}
	else
	{
		angle = (mu_float)EMath::Atan(ny2 / nx2) / EMath::PI*180.0f + 90.0f;
		if (nx2 < 0.0f  && ny2 >= 0.0f)
		{
			r = angle + 180.0f;
		}
		else if (nx2 < 0.0f  && ny2 < 0.0f)
		{
			r = angle + 180.0f;
		}
		else if (nx2 >= 0.0f && ny2 < 0.0f)
		{
			r = angle;
		}
		else
		{
			r = angle;
		}
	}

	return r;
}

mu_int32 TurnAngle(mu_int32 iTheta, mu_int32 iHeading, mu_int32 maxTURN)
{
	mu_int32 iChange = 0;
	mu_int32 Delta = EMath::Abs(iTheta - iHeading);
	if (iTheta > iHeading)
	{
		if (Delta < EMath::Abs((iHeading + 360) - iTheta)) iChange = -EMath::Min(maxTURN, Delta);
		else iChange = EMath::Min(maxTURN, Delta);
	}
	if (iTheta < iHeading)
	{
		if (Delta < EMath::Abs((iTheta + 360) - iHeading)) iChange = EMath::Min(maxTURN, Delta);
		else iChange = -EMath::Min(maxTURN, Delta);
	}
	iTheta += iChange + 360;
	iTheta %= 360;
	return iTheta;
}

mu_float TurnAngle2(mu_float angle, mu_float a, mu_float d)
{
	if (angle < 0.0f) angle += 360.0f;
	if (a < 0.0f) a += 360.0f;
	mu_float aa;
	if (angle < 180.0f)
	{
		aa = angle - d;
		if (a >= angle + d && (a < angle + 180.0f)) angle += d;
		else if (aa >= 0 && (a >= angle + 180.0f || a < aa)) angle -= d;
		else if (aa < 0 && (a >= angle + 180.0f && a < aa + 360.0f)) { angle = angle - d + 360.0f; }
		else                                                  angle = a;
	}
	else
	{
		aa = angle + d;
		if (a < angle - d && (a >= angle - 180.0f)) angle -= d;
		else if (aa < 360.0f && (a < angle - 180.0f || a >= aa)) angle += d;
		else if (aa >= 360.0f && (a < angle - 180.0f && a >= aa - 360.0f)) { angle = angle + d - 360.0f; }
		else                                                  angle = a;
	}
	return angle;
}

mu_float FarAngle(mu_float angle1, mu_float angle2, mu_boolean abs)
{
	if (angle1 < 0.0f) angle1 += 360.0f;
	if (angle2 < 0.0f) angle2 += 360.0f;
	mu_float d = angle2 - angle1;
	if (angle1 < 180.0f)
	{
		if (angle2 >= angle1 + 180.0f) d = (360.0f - angle2 + angle1);
	}
	else
	{
		if (angle2 < angle1 - 180.0f) d = (360.0f - angle1 + angle2);
	}
	if (abs == true) if (d < 0.0f) d = -d;
	return d;
}

mu_int32 CalcAngle(mu_float PositionX, mu_float PositionY, mu_float TargetX, mu_float TargetY)
{
	mu_float Grad;
	if (TargetX - PositionX == 0.0f)
		Grad = 0.0f;
	else
		Grad = (PositionY - TargetY) / (TargetX - PositionX);
	mu_int32 TargetTheta = (mu_int32)(atanf(Grad)*Q_EULER);
	if (TargetX < PositionX) TargetTheta += 180;
	if (TargetTheta < 0) TargetTheta += 360;
	TargetTheta = 360 - TargetTheta;
	TargetTheta %= 360;
	return TargetTheta;
}

namespace BoneManager
{
	void GetBonePosition(OBJECT *pObject, mu_uint32 BoneIndex, EVector3 &Position)
	{
		MUModel *b = &MODELS::Data[pObject->Type];
		Position = EVector3(0.0f, 0.0f, 0.0f);
#ifdef USE_DUAL_QUATERNION
		pObject->BoneTransform[b->GetBoneIndex(BoneIndex)].Transform(Position);
#else
		VectorTransform(Position, pObject->BoneTransform[b->GetBoneIndex(BoneIndex)], Position);
#endif
		VectorScale(Position, pObject->Scale, Position);
		VectorAdd(Position, pObject->Position, Position);
	}

	void GetBonePosition(OBJECT *pObject, mu_uint32 BoneIndex, EVector3 &Relative, EVector3 &Position)
	{
		MUModel *b = &MODELS::Data[pObject->Type];
#ifdef USE_DUAL_QUATERNION
		pObject->BoneTransform[b->GetBoneIndex(BoneIndex)].TransformOut(Relative, Position);
#else
		VectorTransform(Relative, pObject->BoneTransform[b->GetBoneIndex(BoneIndex)], Position);
#endif
		VectorScale(Position, pObject->Scale, Position);
		VectorAdd(Position, pObject->Position, Position);
	}
};

mu_float MoveHumming(EVector3 Position, EVector3 &Angle, EVector3 TargetPosition, mu_float Turn)
{
	mu_float TargetAngle = CreateAngle(Position[0], Position[1], TargetPosition[0], TargetPosition[1]);
	Angle[2] = TurnAngle2(Angle[2], TargetAngle, Turn);

	EVector3 Range = Position - TargetPosition;
	mu_float Distance = EMath::Length(EVector2(Range));

	TargetAngle = 360.0f - CreateAngle(Position[2], Distance, TargetPosition[2], 0.0f);
	Angle[0] = TurnAngle2(Angle[0], TargetAngle, Turn);

	return EMath::Length(Range);
}

void SetAction_Fenrir_Skill(CHARACTER* c, OBJECT* o)
{
	if (c->Weapon[0].Type != -1 && c->Weapon[1].Type != -1)
		SetAction(o, PLAYER_FENRIR_SKILL_TWO_SWORD);
	else if (c->Weapon[0].Type != -1 && c->Weapon[1].Type == -1)
		SetAction(o, PLAYER_FENRIR_SKILL_ONE_RIGHT);
	else if (c->Weapon[0].Type == -1 && c->Weapon[1].Type != -1)
		SetAction(o, PLAYER_FENRIR_SKILL_ONE_LEFT);
	else
		SetAction(o, PLAYER_FENRIR_SKILL);
}

void SetAction_Fenrir_Damage(CHARACTER* c, OBJECT* o)
{
	if (c->Weapon[0].Type != -1 && c->Weapon[1].Type != -1)
		SetAction(o, PLAYER_FENRIR_DAMAGE_TWO_SWORD);
	else if (c->Weapon[0].Type != -1 && c->Weapon[1].Type == -1)
		SetAction(o, PLAYER_FENRIR_DAMAGE_ONE_RIGHT);
	else if (c->Weapon[0].Type != -1 && c->Weapon[1].Type != -1 && c->Weapon[1].Type == MODEL_BOW + 7)
		SetAction(o, PLAYER_FENRIR_DAMAGE_ONE_RIGHT);
	else if (c->Weapon[0].Type == -1 && c->Weapon[1].Type != -1)
		SetAction(o, PLAYER_FENRIR_DAMAGE_ONE_LEFT);
	else if (c->Weapon[0].Type != -1 && c->Weapon[1].Type != -1 && c->Weapon[0].Type == MODEL_BOW + 15)
		SetAction(o, PLAYER_FENRIR_DAMAGE_ONE_LEFT);
	else
		SetAction(o, PLAYER_FENRIR_DAMAGE);
}

void SetAction_Fenrir_Run(CHARACTER* c, OBJECT* o)
{
	if (c->Weapon[0].Type != -1 && c->Weapon[1].Type != -1 && c->Weapon[0].Type != MODEL_BOW + 15 && c->Weapon[1].Type != MODEL_BOW + 7)
	{
		if (GetBaseClass(c->Class) == GAME::CLASS_ELF)
			SetAction(o, PLAYER_FENRIR_RUN_TWO_SWORD_ELF);
		else if (GetBaseClass(c->Class) == GAME::CLASS_DARK)
			SetAction(o, PLAYER_FENRIR_RUN_TWO_SWORD_MAGOM);
		else
			SetAction(o, PLAYER_FENRIR_RUN_TWO_SWORD);
	}
	else if (c->Weapon[0].Type != -1 && c->Weapon[1].Type == -1)
	{
		if (GetBaseClass(c->Class) == GAME::CLASS_ELF)
			SetAction(o, PLAYER_FENRIR_RUN_ONE_RIGHT_ELF);
		else if (GetBaseClass(c->Class) == GAME::CLASS_DARK)
			SetAction(o, PLAYER_FENRIR_RUN_ONE_RIGHT_MAGOM);
		else
			SetAction(o, PLAYER_FENRIR_RUN_ONE_RIGHT);
	}
	else if (c->Weapon[0].Type != -1 && c->Weapon[1].Type != -1 && c->Weapon[1].Type == MODEL_BOW + 7)
	{
		SetAction(o, PLAYER_FENRIR_RUN_ONE_RIGHT_ELF);
	}
	else if (c->Weapon[0].Type == -1 && c->Weapon[1].Type != -1)
	{
		if (GetBaseClass(c->Class) == GAME::CLASS_ELF)
			SetAction(o, PLAYER_FENRIR_RUN_ONE_LEFT_ELF);
		else if (GetBaseClass(c->Class) == GAME::CLASS_DARK)
			SetAction(o, PLAYER_FENRIR_RUN_ONE_LEFT_MAGOM);
		else
			SetAction(o, PLAYER_FENRIR_RUN_ONE_LEFT);
	}
	else if (c->Weapon[0].Type != -1 && c->Weapon[1].Type != -1 && c->Weapon[0].Type == MODEL_BOW + 15)
	{
		SetAction(o, PLAYER_FENRIR_RUN_ONE_LEFT_ELF);
	}
	else
	{
		if (GetBaseClass(c->Class) == GAME::CLASS_ELF)
			SetAction(o, PLAYER_FENRIR_RUN_ELF);
		else if (GetBaseClass(c->Class) == GAME::CLASS_DARK)
			SetAction(o, PLAYER_FENRIR_RUN_MAGOM);
		else
			SetAction(o, PLAYER_FENRIR_RUN);
	}
}

void SetAction_Fenrir_Walk(CHARACTER* c, OBJECT* o)
{
	if (c->Weapon[0].Type != -1 && c->Weapon[1].Type != -1 && c->Weapon[0].Type != MODEL_BOW + 15 && c->Weapon[1].Type != MODEL_BOW + 7)
		SetAction(o, PLAYER_FENRIR_WALK_TWO_SWORD);
	else if (c->Weapon[0].Type != -1 && c->Weapon[1].Type == -1)
		SetAction(o, PLAYER_FENRIR_WALK_ONE_RIGHT);
	else if (c->Weapon[0].Type != -1 && c->Weapon[1].Type != -1 && c->Weapon[1].Type == MODEL_BOW + 7)
		SetAction(o, PLAYER_FENRIR_WALK_ONE_RIGHT);
	else if (c->Weapon[0].Type == -1 && c->Weapon[1].Type != -1)
		SetAction(o, PLAYER_FENRIR_WALK_ONE_LEFT);
	else if (c->Weapon[0].Type != -1 && c->Weapon[1].Type != -1 && c->Weapon[0].Type == MODEL_BOW + 15)
		SetAction(o, PLAYER_FENRIR_WALK_ONE_LEFT);
	else
		SetAction(o, PLAYER_FENRIR_WALK);
}

mu_boolean IsAliceRideAction_UniDino(mu_uint16 byAction)
{
	if (byAction == PLAYER_SKILL_CHAIN_LIGHTNING_UNI
		|| byAction == PLAYER_SKILL_CHAIN_LIGHTNING_DINO
		|| byAction == PLAYER_SKILL_LIGHTNING_ORB_UNI
		|| byAction == PLAYER_SKILL_LIGHTNING_ORB_DINO
		|| byAction == PLAYER_SKILL_DRAIN_LIFE_UNI
		|| byAction == PLAYER_SKILL_DRAIN_LIFE_DINO)
	{
		return true;
	}

	return false;
}

mu_boolean IsAliceRideAction_Fenrir(mu_uint16 byAction)
{
	if (
		byAction == PLAYER_SKILL_CHAIN_LIGHTNING_FENRIR
		|| byAction == PLAYER_SKILL_LIGHTNING_ORB_FENRIR
		|| byAction == PLAYER_SKILL_DRAIN_LIFE_FENRIR
		)
	{
		return true;
	}

	return false;
}

void SetAction(OBJECT *o, mu_int32 Action, mu_boolean bBlending)
{
	MUModel *b = &MODELS::Data[o->Type];

	if (Action >= b->_AnimationCount)
	{
		return;
	}

	o->Flags.Clear(OBJECT::eOF_SECONDANIMATION);

	if (o->CurrentAction != Action)
	{
		o->PriorAction = o->CurrentAction;
		o->PriorAnimationFrame = o->AnimationFrame;
		o->CurrentAction = Action;
		o->AnimationFrame = 0;

		if (bBlending == false)
		{
			o->PriorAnimationFrame = 0;
		}
	}
}

void SetActionClass(CHARACTER *c, OBJECT *o, mu_int32 Action, mu_int32 ActionType)
{
	if ((o->CurrentAction >= PLAYER_STOP_MALE && o->CurrentAction <= PLAYER_STOP_RIDE_WEAPON)
		|| o->CurrentAction == PLAYER_STOP_TWO_HAND_SWORD_TWO)
	{
		if (!IsFemale(c->Class) || (Action >= PLAYER_RESPECT1 && Action <= PLAYER_RUSH1))
			SetAction(o, Action);
		else
			SetAction(o, Action + 1);
	}
}

void SetAllAction(int Action)
{
	for (mu_int32 i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
	{
		CHARACTER *c = &CharactersClient[i];
		if (c->Object.Flags.Get(OBJECT::eOF_LIVE) == true &&
			c->Object.Type == MODEL_PLAYER)
		{
			Vector(0.0f, 0.0f, 180.0f, c->Object.Angle);
			SetAction(&c->Object, Action);
		}
	}
}

bool SkillKeyPush(mu_int32 Skill)
{
	if (Skill == AT_SKILL_BLAST_HELL && MU_INPUT->Is3DMousePressedAny(MOUSE_BUTTON_LEFT) == true)
	{
		return true;
	}
	return false;
}

void Attack(const mu_uint32 ThreadIndex, CHARACTER *c)
{
	if (MU_INPUT->IsMouseOverInterface() == true ||
		MU_CAMERA->IsDragging() == true)
	{
		if (ActionRequest == ACTION_REQUEST::AR_MAGIC)
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

		if (MU_INPUT->Is3DMousePressing(MOUSE_BUTTON_RIGHT) == true)
		{
			MU_INPUT->SetMouseUsed(MOUSE_BUTTON_RIGHT, MOUSE_USED_ALL);
		}
		MouseRButtonPress = 0;

		return;
	}

	OBJECT *o = &c->Object;
	mu_int32 ClassIndex = GetBaseClass(c->Class);

	if (o->Teleport == TELEPORT_BEGIN)
	{
		o->Alpha -= 0.1f;
		if (o->Alpha < 0.1f)
		{
			o->Teleport = TELEPORT;
		}
	}

	mu_boolean Success = false;
	mu_boolean CursedTempleSkillSuccess = false;

	mu_int32 Skill = CharacterAttribute->Skill[Hero->CurrentSkill];

// 	if (MouseRButtonPress != 0)
// 	{
// 		if (MU_INPUT->Is3DMousePressing(MOUSE_BUTTON_RIGHT) == false ||
// 			SkillKeyPush(Skill))
// 		{
// 			MouseRButtonPress = 0;
// 			Success = true;
// 		}
// 		else
// 		{
// 			//MouseRButtonPress++;
// 		}
// 	}
// 	else
	if (MU_INPUT->Is3DMousePressing(MOUSE_BUTTON_RIGHT) == true &&
		MU_INPUT->Is3DMousePressing(MOUSE_BUTTON_LEFT) == false)
	{
		if (Skill == AT_SKILL_BLAST_HELL)
		{
			if (o->Teleport != TELEPORT_END && o->Teleport != TELEPORT_NONE) return;

			mu_int32 iReqEng = 0;
			GetSkillInformation_Energy(Skill, &iReqEng);
			if (CharacterAttribute->Energy + CharacterAttribute->AddEnergy < iReqEng) return;

			MouseRButtonPress = 1;
			Skill = AT_SKILL_BLAST_HELL_BEGIN;
		}

		g_pPickedItem.BackupPickedItem();

		MU_INPUT->SetMouseUsed(MOUSE_BUTTON_RIGHT, MOUSE_USED_PRESSED);
		Success = true;
	}

	if (MU_RESOURCE->IsAutoAttack() == true
		&& GET_WORLD != WD_6STADIUM
		&& InChaosCastle() == false
		&& Attacking == 2
		&& SelectedCharacter != -1)
	{
		Success = true;
	}

	if (Success)
	{
		RButtonPressTime = ((FWorldTime - RButtonPopTime) / CLOCKS_PER_SEC);

		if (RButtonPressTime >= AutoMouseLimitTime)
		{
			if (MU_INPUT->Is3DMousePressing(MOUSE_BUTTON_RIGHT) == true)
			{
				MU_INPUT->SetMouseUsed(MOUSE_BUTTON_RIGHT, MOUSE_USED_ALL);
			}
			Success = false;
		}
	}
	else
	{
		RButtonPopTime = FWorldTime;
		RButtonPressTime = 0.0f;
	}

	if (Success)
	{
		g_iFollowCharacter = -1;

		if (!g_isCharacterBuff((&Hero->Object), eBuff_Cloaking))
		{
			if (giPetManager::SendPetCommand(c, Hero->CurrentSkill) == true)
			{
				return;
			}
		}

#if 0 // FIX
		if (g_CursedTemple.IsCursedTemple())
		{
#if 0 // FIX
			if (g_pCursedTempleWindow->IsCursedTempleSkillKey(SelectedCharacter))
			{
				CursedTempleSkillSuccess = true;
			}
			else
#endif
			{
				if (g_CursedTemple.IsPartyMember(SelectedCharacter) == true)
				{
					if (!IsCorrectSkillType_FrendlySkill(Skill) && !IsCorrectSkillType_Buff(Skill))
					{
						return;
					}
				}
				else
				{
					if (IsCorrectSkillType_FrendlySkill(Skill) || IsCorrectSkillType_Buff(Skill))
					{
						if (-1 != SelectedCharacter)
						{
							return;
						}
					}
				}

				if (CheckSkillUseCondition(o, Skill) == false) return;
			}
		}
		else
#endif
		{
			if (CheckSkillUseCondition(o, Skill) == false)
			{
				return;
			}
		}
	}

	if (Success && c->Dead == 0)
	{
#if 0 // FIX
		if (!c->SafeZone && CursedTempleSkillSuccess)
		{
#if 0 // FIX
			g_pCursedTempleWindow->SetCursedTempleSkill(c, o, SelectedCharacter);
#endif
		}
		else
#endif
		if (!c->SafeZone ||
			InBloodCastle() == true ||
			InChaosCastle() == true)
		{
			mu_boolean AttackSuccess = true;

			if (SelectedCharacter != -1 &&
				CharactersClient[SelectedCharacter].Object.Kind == KIND_PLAYER)
			{
				CHARACTER *tc = &CharactersClient[SelectedCharacter];
				OBJECT *to = &tc->Object;

				if (battleCastle::IsBattleCastleStart())
				{
					if ((c->EtcPart == PARTS_ATTACK_KING_TEAM_MARK || c->EtcPart == PARTS_ATTACK_TEAM_MARK
						|| c->EtcPart == PARTS_ATTACK_KING_TEAM_MARK2 || c->EtcPart == PARTS_ATTACK_KING_TEAM_MARK3
						|| c->EtcPart == PARTS_ATTACK_TEAM_MARK2 || c->EtcPart == PARTS_ATTACK_TEAM_MARK3)
						&& (tc->EtcPart == PARTS_DEFENSE_KING_TEAM_MARK || tc->EtcPart == PARTS_DEFENSE_TEAM_MARK))
					{
						if (!g_isCharacterBuff((&tc->Object), eBuff_Cloaking))
						{
							AttackSuccess = true;
						}
						else
						{
							AttackSuccess = false;
						}
					}
					else if ((c->EtcPart == PARTS_DEFENSE_KING_TEAM_MARK || c->EtcPart == PARTS_DEFENSE_TEAM_MARK)
						&& (tc->EtcPart == PARTS_ATTACK_KING_TEAM_MARK || tc->EtcPart == PARTS_ATTACK_TEAM_MARK
							|| tc->EtcPart == PARTS_ATTACK_KING_TEAM_MARK2 || tc->EtcPart == PARTS_ATTACK_KING_TEAM_MARK3
							|| tc->EtcPart == PARTS_ATTACK_TEAM_MARK2 || tc->EtcPart == PARTS_ATTACK_TEAM_MARK3))
					{
						if (!g_isCharacterBuff((&tc->Object), eBuff_Cloaking))
						{
							AttackSuccess = true;
						}
						else
						{
							AttackSuccess = false;
						}
					}
					else if (g_isCharacterBuff((&c->Object), eBuff_CastleRegimentAttack1)
						|| g_isCharacterBuff((&c->Object), eBuff_CastleRegimentAttack2)
						|| g_isCharacterBuff((&c->Object), eBuff_CastleRegimentAttack3))
					{
						if (!g_isCharacterBuff(to, eBuff_CastleRegimentAttack1)
							&& !g_isCharacterBuff(to, eBuff_CastleRegimentAttack2)
							&& !g_isCharacterBuff(to, eBuff_CastleRegimentAttack3))
						{
							if (!g_isCharacterBuff((&tc->Object), eBuff_Cloaking))
							{
								AttackSuccess = true;
							}
							else
							{
								AttackSuccess = false;
							}
						}
					}
					else if (g_isCharacterBuff((&c->Object), eBuff_CastleRegimentDefense))
					{
						if (!g_isCharacterBuff(to, eBuff_CastleRegimentDefense))
						{
							if (!g_isCharacterBuff((&tc->Object), eBuff_Cloaking))
							{
								AttackSuccess = true;
							}
							else
							{
								AttackSuccess = false;
							}
						}
					}
				}

				if (tc->GuildRelationShip == GAME::GR_RIVAL || tc->GuildRelationShip == GAME::GR_RIVALUNION)
				{
					AttackSuccess = true;
				}

				if (EnableGuildWar && tc->PK >= PVP_MURDERER2 && tc->GuildMarkIndex != -1
					&& GuildMark[c->GuildMarkIndex].GuildIndex == GuildMark[tc->GuildMarkIndex].GuildIndex)
				{
					AttackSuccess = false;
				}
				else if (g_DuelMgr.IsDuelEnabled())
				{
					if (g_DuelMgr.IsDuelPlayer(tc, DUEL_ENEMY))
					{
						AttackSuccess = true;
					}
					else
					{
						AttackSuccess = false;
					}
				}
				else if (g_isCharacterBuff((&c->Object), eBuff_DuelWatch))
				{
					AttackSuccess = false;
				}
				else if (EnableGuildWar)
				{
					if (tc->GuildTeam == 2 && tc != c)
					{
						AttackSuccess = true;
					}
					else
					{
						AttackSuccess = false;
					}
				}
				else if (tc->PK >= PVP_MURDERER2 ||
					(MU_INPUT->IsCtrlPressing() == true &&
						tc != c))
				{
					AttackSuccess = true;
				}
				else if (g_CursedTemple.IsCursedTemple() && !g_CursedTemple.IsPartyMember(SelectedCharacter))
				{
					AttackSuccess = true;
				}
				else
				{
					AttackSuccess = false;
				}
			}

			if(AttackSuccess == true)
			{
				mu_int16 CharacterTarget = SelectedCharacter >= 0 ? CharactersClient[SelectedCharacter].Key : -1;

				if(g_pNewUISystem->AllowHeroAction() == true)
				{
					mu_float SkillDirection = CreateAngle(o->Position[0], o->Position[1], (SelectXF + 0.5f) * g_TerrainScale, (SelectYF + 0.5f) * g_TerrainScale);

					if (ActionRequest != ACTION_REQUEST::AR_MAGIC ||
						ActionRequestMagic != Hero->CurrentSkill ||
						ActionRequestTarget != CharacterTarget ||
						(CharacterTarget == -1 &&
						(ActionRequestX != SelectXF ||
							ActionRequestY != SelectYF ||
							ActionRequestDirection != SkillDirection)))
					{
						CL2GS_REQUEST_MAGIC RequestMagic;

						RequestMagic.Key = CharacterTarget;
						RequestMagic.Magic = Hero->CurrentSkill;
						RequestMagic.X = SelectXF;
						RequestMagic.Y = SelectYF;
						RequestMagic.Direction = SkillDirection;

						g_GameServerObject->Send(&RequestMagic);

						ActionRequest = ACTION_REQUEST::AR_MAGIC;
						ActionRequestTarget = CharacterTarget;
						ActionRequestMagic = Hero->CurrentSkill;
						ActionRequestX = SelectXF;
						ActionRequestY = SelectYF;
						ActionRequestDirection = SkillDirection;
					}
				}
			}
			else if (ActionRequest == ACTION_REQUEST::AR_MAGIC)
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
		else if (ActionRequest == ACTION_REQUEST::AR_MAGIC)
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
	else if (ActionRequest == ACTION_REQUEST::AR_MAGIC)
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

void AppearMonster(const mu_uint32 ThreadIndex, CHARACTER *c)
{
	OBJECT *o = &c->Object;
	switch (c->MonsterIndex)
	{
	case 44:
		SetAction(o, MONSTER01_STOP2);
		o->PriorAction = MONSTER01_STOP2;
		c->Object.Alpha = 1.0f;
		PlayBackground(SOUND_BULL_ATTACK1);
		break;
	case 21:
		SetAction(o, MONSTER01_STOP2);
		o->PriorAction = MONSTER01_STOP2;
		c->Object.Alpha = 1.0f;
		PlayBackground(SOUND_ASSASSIN);
		break;
	case 53:
	case 54:
		c->Appear = 60;
		SetAction(o, MONSTER01_STOP2);
		o->PriorAction = MONSTER01_STOP2;
		c->Object.Alpha = 1.0f;
		break;
	case 85:
	case 91:
	case 97:
	case 114:
	case 120:
	case 126:
		PlayBackground(SOUND_ORC_CAP_ATTACK1);
		break;
	case 440:
	case 340:
	case 341:
	case 344:
	case 345:
		{
			if (g_Direction.m_CMVP.m_iCryWolfState == CRYWOLF_STATE_READY)
				c->Object.Alpha = 1.0f;
			else
			{
				c->Object.Alpha = 0.0f;

				EVector3 vPos;
				Vector(c->Object.Position[0] + 20.0f, c->Object.Position[1] + 20.0f, c->Object.Position[2], vPos);

				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, vPos, vPos, c->Object.Angle, 7, nullptr, 60.0f + GetLargeRand(10));
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, vPos, vPos, c->Object.Angle, 7, nullptr, 50.0f + GetLargeRand(10));
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, vPos, vPos, c->Object.Angle, 7, nullptr, 50.0f + GetLargeRand(10));
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, vPos, vPos, c->Object.Angle, 7, nullptr, 60.0f + GetLargeRand(10));

				CreateParticle(ThreadIndex, BITMAP_SMOKE + 4, c->Object.Position, c->Object.Angle, c->Object.Light, 1, 5.0f);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 4, c->Object.Position, c->Object.Angle, c->Object.Light, 1, 5.0f);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 4, c->Object.Position, c->Object.Angle, c->Object.Light, 1, 5.0f);

				Vector(c->Object.Position[0], c->Object.Position[1], c->Object.Position[2] + 120.0f, vPos);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, c->Object.Position, vPos, c->Object.Angle, 17);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, c->Object.Position, vPos, c->Object.Angle, 17);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, c->Object.Position, vPos, c->Object.Angle, 17);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, c->Object.Position, vPos, c->Object.Angle, 17);
			}
		}
		break;
	case 362:
	case 363:
		{
			SetAction(o, MONSTER01_APEAR);
			c->Object.Alpha = 0.0f;
		}
		break;
	default:
		if (o->Type == MODEL_PLAYER)
		{
			SetAction(o, PLAYER_COME_UP);
			o->PriorAction = PLAYER_COME_UP;
			c->Object.Alpha = 1.0f;
		}
		else
		{
			c->Object.Alpha = 0.0f;
		}
		break;
	}
}

void LookAtTarget(OBJECT *o, CHARACTER *tc)
{
	if (tc == nullptr) return;
	OBJECT *to = &tc->Object;
	mu_float Angle = CreateAngle(o->Position[0], o->Position[1], to->Position[0], to->Position[1]);
	if (FarAngle(o->Angle[2], Angle) < 90.0f)
	{
		o->HeadTargetAngle[0] = (o->Angle[2] - Angle);
		o->HeadTargetAngle[1] = (to->Position[2] - (o->Position[2] + 50.0f))*0.2f;
		for (mu_int32 i = 0; i < 2; ++i)
			if (o->HeadTargetAngle[i] < 0) o->HeadTargetAngle[i] += 360.0f;
	}
	else
	{
		o->HeadTargetAngle[0] = 0.0f;
		o->HeadTargetAngle[1] = 0.0f;
	}
}

void MoveHead(CHARACTER *c)
{
	OBJECT *o = &c->Object;
	if (o->CurrentAction != MONSTER01_DIE)
	{
		if (o->CurrentAction == MONSTER01_STOP1)
		{
			if (GetLargeRand(32) == 0)
			{
				o->HeadTargetAngle[0] = (mu_float)(GetLargeRand(128) - 64);
				o->HeadTargetAngle[1] = (mu_float)(GetLargeRand(48) - 16);
				for (mu_int32 i = 0; i < 2; ++i)
					if (o->HeadTargetAngle[i] < 0) o->HeadTargetAngle[i] += 360.0f;
			}
		}
		else if (o->CurrentAction == MONSTER01_WALK && c->TargetCharacter != -1)
		{
			LookAtTarget(o, &CharactersClient[c->TargetCharacter]);
		}
		else
		{
			o->HeadTargetAngle[0] = 0.0f;
			o->HeadTargetAngle[1] = 0.0f;
		}
	}
}

mu_boolean MovePath(CHARACTER *c, mu_boolean Turn)
{
	mu_boolean Success = false;
	PATH_t *p = &c->Path;
	if (p->CurrentPath < p->PathNum)
	{
		mu_float x[4], y[4];
		for (mu_int32 i = 0; i < 4; ++i)
		{
			mu_int32 Path = p->CurrentPath + i - 1;

			if (Path < 0)
				Path = 0;
			else if (Path > p->PathNum - 1)
				Path = p->PathNum - 1;

			x[i] = ((mu_float)p->PathX[Path] + 0.5f)*g_TerrainScale;
			y[i] = ((mu_float)p->PathY[Path] + 0.5f)*g_TerrainScale;
		}

		mu_float cx, cy;

		switch (p->CurrentPathFloat)
		{
		case 0:
			cx = x[0] * -0.0703125f + x[1] * 0.8671875f + x[2] * 0.2265625f + x[3] * -0.0234375f;
			cy = y[0] * -0.0703125f + y[1] * 0.8671875f + y[2] * 0.2265625f + y[3] * -0.0234375f;
			break;
		case 1:
			cx = x[0] * -0.0625f + x[1] * 0.5625f + x[2] * 0.5625f + x[3] * -0.0625f;
			cy = y[0] * -0.0625f + y[1] * 0.5625f + y[2] * 0.5625f + y[3] * -0.0625f;
			break;
		case 2:
			cx = x[0] * -0.0234375f + x[1] * 0.2265625f + x[2] * 0.8671875f + x[3] * -0.0703125f;
			cy = y[0] * -0.0234375f + y[1] * 0.2265625f + y[2] * 0.8671875f + y[3] * -0.0703125f;
			break;
		case 3:
			cx = x[2];
			cy = y[2];
			break;
		}

		OBJECT *o = &c->Object;
		mu_float dx = o->Position[0] - cx;
		mu_float dy = o->Position[1] - cy;
		mu_float Distance = EMath::Sqrt(dx*dx + dy*dy);
		mu_boolean isMovedNext = false;

		if (Distance <= 20.0f)
		{
			if (++p->CurrentPathFloat > 3)
			{
				p->CurrentPathFloat = 0;
				if (++p->CurrentPath >= p->PathNum - 1)
				{
					p->CurrentPath = p->PathNum - 1;
					c->PositionX = p->PathX[p->CurrentPath];
					c->PositionY = p->PathY[p->CurrentPath];
					o->Position[0] = ((mu_float)c->PositionX + 0.5f) * g_TerrainScale;
					o->Position[1] = ((mu_float)c->PositionY + 0.5f) * g_TerrainScale;
					p->PathNum = 0;
					Success = true;
				}
				else
				{
					c->PositionX = p->PathX[p->CurrentPath];
					c->PositionY = p->PathY[p->CurrentPath];
					isMovedNext = true;
				}
			}
		}

		if (!Success && Turn)
		{
			mu_float ncx, ncy;
			
// 			if(isMovedNext == true)
// 			{
// 				mu_float x2[4], y2[4];
// 				for (mu_int32 i = 0; i < 4; ++i)
// 				{
// 					mu_int32 Path = p->CurrentPath + i - 1;
// 
// 					if (Path < 0)
// 						Path = 0;
// 					else if (Path > p->PathNum - 1)
// 						Path = p->PathNum - 1;
// 
// 					x2[i] = ((mu_float)p->PathX[Path] + 0.5f)*TERRAIN_SCALE;
// 					y2[i] = ((mu_float)p->PathY[Path] + 0.5f)*TERRAIN_SCALE;
// 				}
// 
// 				switch (p->CurrentPathFloat)
// 				{
// 				case 0:
// 					ncx = x2[0] * -0.0703125f + x2[1] * 0.8671875f + x2[2] * 0.2265625f + x2[3] * -0.0234375f;
// 					ncy = y2[0] * -0.0703125f + y2[1] * 0.8671875f + y2[2] * 0.2265625f + y2[3] * -0.0234375f;
// 					break;
// 				case 1:
// 					ncx = x2[0] * -0.0625f + x2[1] * 0.5625f + x2[2] * 0.5625f + x2[3] * -0.0625f;
// 					ncy = y2[0] * -0.0625f + y2[1] * 0.5625f + y2[2] * 0.5625f + y2[3] * -0.0625f;
// 					break;
// 				case 2:
// 					ncx = x2[0] * -0.0234375f + x2[1] * 0.2265625f + x2[2] * 0.8671875f + x2[3] * -0.0703125f;
// 					ncy = y2[0] * -0.0234375f + y2[1] * 0.2265625f + y2[2] * 0.8671875f + y2[3] * -0.0703125f;
// 					break;
// 				case 3:
// 					ncx = x2[2];
// 					ncy = y2[2];
// 					break;
// 				}
// 			}
// 			else
			{
				ncx = cx;
				ncy = cy;
			}

// 			if(EMath::Abs(cx - o->Position[0]) > 0.0001f ||
// 				EMath::Abs(cy - o->Position[1]) > 0.0001f)
			{
				EVector2 originPos = EVector2(o->Position);
				EVector2 targetPos = EVector2(ncx, ncy);
				mu_float Angle = CreateAngle(originPos[0], originPos[1], targetPos[0], targetPos[1]);
				mu_float TargetAngle = FarAngle(o->Angle[2], Angle);
				if (TargetAngle >= 45.0f)
				{
					o->Angle[2] = Angle;
				}
				else
				{
					o->Angle[2] = TurnAngle2(o->Angle[2], Angle, TargetAngle*0.5f);
				}
			}
		}
	}

	return Success;
}

mu_boolean PathFinding2(const mu_uint32 ThreadIndex, mu_int32 sx, mu_int32 sy, mu_int32 tx, mu_int32 ty, PATH_t *a, mu_float fDistance, mu_int32 iDefaultWall, mu_uint8 extraAddr)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];
	mu_boolean Value = false;

	if (M34CryWolf1st::Get_State_Only_Elf() == true && M34CryWolf1st::IsCyrWolf1st() == true)
	{
		if ((CharactersClient[TargetNpc].Object.Type >= MODEL_CRYWOLF_ALTAR1 && CharactersClient[TargetNpc].Object.Type <= MODEL_CRYWOLF_ALTAR5))
		{
			Value = true;
		}
	}

	mu_int32 Wall = iDefaultWall;

	mu_boolean Success = PATH::FindPath(ThreadIndex, sx, sy, tx, ty, true, Wall, Value, fDistance);
	
	if (!Success)
	{
		if (((TerrainWall[TERRAIN_INDEX_REPEAT(sx, sy)] & TW_SAFEZONE) == TW_SAFEZONE || (TerrainWall[TERRAIN_INDEX_REPEAT(tx, ty)] & TW_SAFEZONE) == TW_SAFEZONE)
			&& (TerrainWall[TERRAIN_INDEX_REPEAT(tx, ty)] & TW_CHARACTER) != TW_CHARACTER)
		{
			Wall = TW_NOMOVE;
		}

		Success = PATH::FindPath(ThreadIndex, sx, sy, tx, ty, false, Wall, Value, fDistance);
	}

	if (Success)
	{
		mu_int32 PathNum = ThreadInfo.NumPath;
		if (PathNum > 1)
		{
			a->PathNum = PathNum;
			mu_memcpy(a->PathX + extraAddr, ThreadInfo.PathX, a->PathNum);
			mu_memcpy(a->PathY + extraAddr, ThreadInfo.PathY, a->PathNum);

			a->CurrentPath = 0;
			a->CurrentPathFloat = 0;

			return true;
		}
	}
	return false;
}

mu_boolean TestPath(const mu_uint32 ThreadIndex, mu_int32 sx, mu_int32 sy, mu_int32 tx, mu_int32 ty, mu_float fDistance, mu_int32 iDefaultWall)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];
	mu_boolean Value = false;

	if (M34CryWolf1st::Get_State_Only_Elf() == true && M34CryWolf1st::IsCyrWolf1st() == true)
	{
		if ((CharactersClient[TargetNpc].Object.Type >= MODEL_CRYWOLF_ALTAR1 && CharactersClient[TargetNpc].Object.Type <= MODEL_CRYWOLF_ALTAR5))
		{
			Value = true;
		}
	}

	mu_int32 Wall = iDefaultWall;

	mu_boolean Success = PATH::FindPath(ThreadIndex, sx, sy, tx, ty, true, Wall, Value, fDistance);

	if (!Success)
	{
		if (((TerrainWall[TERRAIN_INDEX_REPEAT(sx, sy)] & TW_SAFEZONE) == TW_SAFEZONE || (TerrainWall[TERRAIN_INDEX_REPEAT(tx, ty)] & TW_SAFEZONE) == TW_SAFEZONE)
			&& (TerrainWall[TERRAIN_INDEX_REPEAT(tx, ty)] & TW_CHARACTER) != TW_CHARACTER)
		{
			Wall = TW_NOMOVE;
		}

		Success = PATH::FindPath(ThreadIndex, sx, sy, tx, ty, false, Wall, Value, fDistance);
	}

	if (Success)
	{
		mu_int32 PathNum = ThreadInfo.NumPath;
		if (PathNum > 1)
		{
			return true;
		}
	}
	return false;
}

void BodyLight(const mu_uint32 ThreadIndex, OBJECT *o, MUModel *b)
{
	switch (o->Type)
	{
	case MODEL_MONSTER01 + 55:
		{
			b->BodyLight(ThreadIndex) = EVector3(0.6f, 0.6f, 0.6f);
		}
		break;

	case MODEL_PROTECT:
		{
			mu_float Luminosity = EMath::Sin(FWorldTime*0.003f)*0.5f + 0.5f;
			b->BodyLight(ThreadIndex) = EVector3(Luminosity, Luminosity, Luminosity);
		}
		break;

	default:
		{
			const mu_boolean lightEnable = o->Flags.Get(OBJECT::eOF_LIGHTENABLE);
			b->LightEnable(ThreadIndex) = lightEnable;

			if (lightEnable)
			{
				RequestTerrainLight(o->Position[0], o->Position[1], b->BodyLight(ThreadIndex));
				b->BodyLight(ThreadIndex) += o->Light;
			}
			else
			{
				RequestTerrainLight(o->Position[0], o->Position[1], b->BodyLight(ThreadIndex));
				b->BodyLight(ThreadIndex) *= 0.1f;
				b->BodyLight(ThreadIndex) += o->Light;
			}
		}
		break;
	}
}

void MoveEye(const mu_uint32 ThreadIndex, OBJECT *o, MUModel *b, mu_int32 Right, mu_int32 Left, mu_int32 Right2, mu_int32 Left2, mu_int32 Right3, mu_int32 Left3)
{
	EVector3 p;
	Vector(0.0f, 0.0f, 0.0f, p);
	b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(Right)], p, o->EyeRight, true);
	Vector(0.0f, 0.0f, 0.0f, p);
	b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(Left)], p, o->EyeLeft, true);
	if (Right2 != -1)
		b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(Right2)], p, o->EyeRight2, true);
	if (Left2 != -1)
		b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(Left2)], p, o->EyeLeft2, true);
	if (Right3 != -1)
		b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(Right3)], p, o->EyeRight3, true);
	if (Left3 != -1)
		b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(Left3)], p, o->EyeLeft3, true);
}

void MonsterDieSandSmoke(const mu_uint32 ThreadIndex, OBJECT *o)
{
	if (o->CurrentAction == MONSTER01_DIE && o->AnimationFrame >= 8.0f && o->AnimationFrame < 9.0f)
	{
		EVector3 Position;
		for (mu_int32 i = 0; i < 20; ++i)
		{
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			Vector(o->Position[0] + (mu_float)(GetLargeRand(64) - 32),
				o->Position[1] + (mu_float)(GetLargeRand(64) - 32),
				o->Position[2] + (mu_float)(GetLargeRand(32) - 16), Position);
			CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, Position, o->Angle, o->Light, 1);
		}
	}
}

void MonsterMoveSandSmoke(const mu_uint32 ThreadIndex, OBJECT *o)
{
	if (o->CurrentAction == MONSTER01_WALK)
	{
		EVector3 Position;
		Vector(o->Position[0] + GetLargeRand(200) - 100, o->Position[1] + GetLargeRand(200) - 100, o->Position[2], Position);
		CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, Position, o->Angle, o->Light);
	}
}

mu_int32 FindCharacterIndex(mu_int32 Key, mu_boolean checkAlive)
{
	if (Key < 0 || Key >= MAX_KEYS) return MAX_CHARACTERS_CLIENT;

	mu_int16 index = CharactersKeys[Key];

	if (index == -1 ||
		(checkAlive == true && CharactersClient[index].Object.Flags.Get(OBJECT::eOF_LIVE) == false))
		return MAX_CHARACTERS_CLIENT;

	return index;
}

mu_int32 HangerBloodCastleQuestItem(mu_int32 Key)
{
	for (mu_int32 i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
	{
		CharactersClient[i].EtcPart = 0;
	}
	return FindCharacterIndex(Key);
}

void FallingStartCharacter(CHARACTER* c, OBJECT* o)
{
	mu_uint8 positionX = (mu_uint8)(o->Position[0] * g_TerrainScaleRev);
	mu_uint8 positionY = (mu_uint8)(o->Position[1] * g_TerrainScaleRev);
	mu_int32 WallIndex = TERRAIN_INDEX_REPEAT(positionX, positionY);
	mu_int32 Wall = TerrainWall[WallIndex] & TW_ACTION;

	o->Flags.Clear(OBJECT::eOF_ACTIONSTART);

	if (InChaosCastle() == true && (TerrainWall[WallIndex] & TW_NOGROUND) == TW_NOGROUND)
	{
		c->StormTime = 0;
		o->Flags.Set(OBJECT::eOF_ACTIONSTART);
		o->Gravity = GetLargeRand(10) + 10.0f;
		o->Velocity -= GetLargeRand(3) + 3.0f;

		Vector(0.0f, 0.0f, 1.0f, o->Direction);
		VectorCopy(o->Position, o->m_vDeadPosition);

		PlayBackground(SOUND_CHAOS_FALLING);
	}
	else if (Wall == TW_ACTION)
	{
		c->StormTime = 0;
		o->Flags.Set(OBJECT::eOF_ACTIONSTART);
		o->Gravity = GetLargeRand(10) + 10.0f;
		o->Velocity = GetLargeRand(20) + 20.0f;
		mu_float angle = GetLargeRand(10) + 85.0f;

		if ((TerrainWall[WallIndex + 1] & TW_NOGROUND) == TW_NOGROUND)
		{
			Vector(0.0f, 0.0f, -angle, o->m_vDownAngle);
		}
		else if ((TerrainWall[WallIndex - 1] & TW_NOGROUND) == TW_NOGROUND)
		{
			Vector(0.0f, 0.0f, angle, o->m_vDownAngle);
		}

		o->Angle[2] = o->m_vDownAngle[2];
		Vector(GetLargeRand(6) + 8.0f, -GetLargeRand(2) + 13.0f, 5.0f, o->Direction);
		VectorCopy(o->Position, o->m_vDeadPosition);
	}

	if (c == Hero)
	{
#if 0 // FIX
		matchEvent::ClearMatchInfo();
#endif
	}
}

void CreateBonfire(const mu_uint32 ThreadIndex, EVector3 Position, EVector3 Angle)
{
	Position[0] += (mu_float)(GetLargeRand(16) - 8);
	Position[1] += (mu_float)(GetLargeRand(16) - 8);
	Position[2] += (mu_float)(GetLargeRand(16) - 8);
	EVector3 Light;
	Vector(1.0f, 1.0f, 1.0f, Light);
	CreateParticle(ThreadIndex, BITMAP_FIRE, Position, Angle, Light, GetLargeRand(4));
	if (GetLargeRand(4) == 0)
	{
		CreateParticle(ThreadIndex, BITMAP_SPARK, Position, Angle, Light);
		EVector3 a;
		Vector(-(mu_float)(GetLargeRand(60) + 30), 0.0f, (mu_float)(GetLargeRand(360)), a);
		CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, Position, Position, a);
	}
	mu_float Luminosity = (mu_float)(GetLargeRand(6) + 6)*0.1f;
	Vector(Luminosity, Luminosity*0.6f, Luminosity*0.4f, Light);
	AddTerrainLight(Position[0], Position[1], Light, 4, PrimaryTerrainLight);
}

void CreateFire(const mu_uint32 ThreadIndex, mu_int32 Type, OBJECT *o, mu_float x, mu_float y, mu_float z)
{
	EVector3 Light;
	EVector3 p, Position;
	mu_float Luminosity;
	EMatrix4 Matrix = EMath::EulerToMatrix(o->Angle);
	Vector(x, y, z, p);
	Position = EMath::TransformNormal(Matrix, p);
	VectorAdd(Position, o->Position, Position);
	Position[0] += (mu_float)(GetLargeRand(16) - 8);
	Position[1] += (mu_float)(GetLargeRand(16) - 8);
	Position[2] += (mu_float)(GetLargeRand(16) - 8);
	switch (Type)
	{
	case 0:
		Luminosity = (mu_float)(GetLargeRand(6) + 6)*0.1f;
		Vector(Luminosity, Luminosity*0.6f, Luminosity*0.4f, Light);
		if (GetLargeRand(2) == 0)
			CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, Light, GetLargeRand(4));
		AddTerrainLight(Position[0], Position[1], Light, 4, PrimaryTerrainLight);
		break;
	case 1:
		if (GetLargeRand(2) == 0)
			CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, o->Light);
		break;
	case 2:
		if (GetLargeRand(2) == 0)
			CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, o->Light, 2);
		break;
	}
}

void CheckSkull(OBJECT *o)
{
	EVector3 Position;
	VectorCopy(Hero->Object.Position, Position);
	if (Hero->Object.CurrentAction >= PLAYER_WALK_MALE && Hero->Object.CurrentAction <= PLAYER_RUN_RIDE_WEAPON)
	{
		if (o->Direction[0] < 0.1f)
		{
			mu_float dx = Position[0] - o->Position[0];
			mu_float dy = Position[1] - o->Position[1];
			mu_float Distance = EMath::Sqrt(dx*dx + dy*dy);
			if (Distance < 50.0f)
			{
				Vector(-dx*0.4f, -dy*0.4f, 0.0f, o->Direction);
				o->HeadAngle[1] = -dx*4.0f;
				o->HeadAngle[0] = -dy*4.0f;
				PlayObject(SOUND_BONE2, o);
			}
		}
	}
	VectorScale(o->Direction, 0.6f, o->Direction);
	VectorScale(o->HeadAngle, 0.6f, o->HeadAngle);
	VectorAdd(o->Position, o->Direction, o->Position);
	VectorAdd(o->Angle, o->HeadAngle, o->Angle);
}

void RenderAurora(mu_int32 Type, mu_int32 RenderType, mu_float x, mu_float y, mu_float sx, mu_float sy, EVector3 Light)
{
	if (MU_UPDATECOUNTER == 0) return;

	mu_float Luminosity = EMath::Sin(FWorldTime*0.0015f)*0.3f + 0.7f;

	mu_uint32 renderType = RenderType == RENDER_DARK ? BLEND::TXB_BLENDMINUS : BLEND::TXB_BLEND01;

	if (RenderType == RENDER_DARK)
	{
		Vector(Luminosity, Luminosity, Luminosity, Light);
	}
	else
	{
		Vector(Luminosity*Light[0], Luminosity*Light[1], Luminosity*Light[2], Light);
	}

	RenderTerrainAlphaBitmap(Type, renderType, x, y, sx, sy, Light, FWorldTime*0.01f, 1.0f, 5.0f);

	if (RenderType == RENDER_DARK)
	{
		Luminosity = 1.0f - Luminosity;
		Vector(Luminosity*Light[0], Luminosity*Light[1], Luminosity*Light[2], Light);
	}

	RenderTerrainAlphaBitmap(Type, renderType, x, y, sx, sy, Light, -FWorldTime*0.01f, 1.0f, 5.0f);
}

void RenderPartObjectEdge(const mu_uint32 ThreadIndex, MUModel *b, OBJECT *o, mu_int32 Flag, mu_boolean Translate, mu_float Scale)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (g_isCharacterBuff(o, eBuff_Cloaking))
	{
		return;
	}

	b->LightEnable(ThreadIndex) = false;

	ThreadInfo.Translate = Translate;
	ThreadInfo.BoneScale = Scale;
	b->Transform(ThreadIndex, o->BoundingBoxMin, o->BoundingBoxMax, &o->OBB, Translate);

	if (o->Type == MODEL_WARCRAFT)
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, o->Alpha);
		b->RenderMesh(ThreadIndex, 0, Flag, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (o->Type == MODEL_MONSTER01 + 114)
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
		b->RenderMesh(ThreadIndex, 0, Flag, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, Flag, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (o->Type == MODEL_MONSTER01 + 116)
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
		b->RenderMesh(ThreadIndex, 0, Flag, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, Flag, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (o->Type == MODEL_MONSTER01 + 121)
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
		b->RenderMesh(ThreadIndex, 0, Flag, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 2, Flag, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 3, Flag, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else
	{
		b->RenderBody(ThreadIndex, Flag, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}

	ThreadInfo.BoneScale = 1.0f;
}

void RenderPartObjectEdge2(const mu_uint32 ThreadIndex, MUModel *b, OBJECT* o, mu_int32 Flag, mu_boolean Translate, mu_float Scale, OBB_t* OBB)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 tmp(0.0f, 0.0f, 0.0f);

	b->LightEnable(ThreadIndex) = false;

	ThreadInfo.Translate = Translate;
	ThreadInfo.BoneScale = Scale;
	b->Transform(ThreadIndex, tmp, tmp, OBB, Translate);
	b->RenderBody(ThreadIndex, Flag, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

	ThreadInfo.BoneScale = 1.0f;
}

void RenderPartObjectEdgeLight(const mu_uint32 ThreadIndex, MUModel *b, OBJECT* o, mu_int32 Flag, mu_boolean Translate, mu_float Scale)
{
	mu_float Luminosity = EMath::Sin(FWorldTime*0.001f)*0.5f + 0.5f;
	Vector(Luminosity, Luminosity*0.8f, Luminosity*0.3f, b->BodyLight(ThreadIndex));
	RenderPartObjectEdge(ThreadIndex, b, o, Flag | RENDER_PREBLEND | RENDER_BACKCULL, Translate, Scale);
}

mu_boolean Calc_RenderObject(const mu_uint32 ThreadIndex, OBJECT *o, mu_boolean Translate, mu_int32 Select, mu_int32 ExtraMon)
{
	if (InChaosCastle() == true && Hero->Object.Flags.Get(OBJECT::eOF_ACTIONSTART) == true)
	{
		o->Alpha -= 0.15f;
	}

	if (o->Alpha < 0.01f)
	{
		return false;
	}

	MUModel *b = &MODELS::Data[o->Type];
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (b->IsDataLoaded() == false)
	{
		b->RequestLoad();
		return false;
	}

	b->BodyHeight(ThreadIndex) = 0.0f;
	BodyLight(ThreadIndex, o, b);
	b->BodyScale(ThreadIndex) = o->Scale;
	b->CurrentAction(ThreadIndex) = o->CurrentAction;
	VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));

	if (o->Type == MODEL_MONSTER01 + 61)
	{
		EVector3 Position;
		VectorCopy(o->Position, Position);

		Position[1] += 60.0f;
		VectorCopy(Position, b->BodyOrigin(ThreadIndex));
	}
	else if (o->Type == MODEL_MONSTER01 + 60)
	{
		EVector3 Position;
		VectorCopy(o->Position, Position);

		Position[1] += 120.0f;
		VectorCopy(Position, b->BodyOrigin(ThreadIndex));
	}

	if (o->Owner != nullptr)
	{
		if (g_isCharacterBuff(o->Owner, eDeBuff_Stun)
			|| g_isCharacterBuff(o->Owner, eDeBuff_Sleep))
		{
			o->AnimationFrame = 0.0f;
		}
	}

	b->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle, false, !Translate);
	ThreadInfo.SkeletalIndex = g_Tasking.RegisterSkeletal(ThreadInfo.BoneTransform, b->_BoneNonDummyEnd);
	o->BoneTransform = &g_Tasking._SkeletonBuffer[ThreadInfo.SkeletalIndex];

	if (ThreadInfo.SkeletalIndex == FAILED_SKELETAL)
	{
		return false;
	}

	ThreadInfo.BoneScale = 1.0f;
	if (3 == Select)
	{
		ThreadInfo.BoneScale = 1.4f;
	}
	else if (2 == Select)
	{
		ThreadInfo.BoneScale = 1.2f;
	}
	else if (1 == Select)
	{
		b->LightEnable(ThreadIndex) = false;

		if (InChaosCastle() == true || o->Kind != KIND_NPC)
		{
			Vector(0.1f, 0.01f, 0.0f, b->BodyLight(ThreadIndex));
			if (o->Type == MODEL_MONSTER01 + 32)
			{
				ThreadInfo.BoneScale = 1.2f;
			}
			else
			{
				ThreadInfo.BoneScale = 1.0f + (0.1f / o->Scale);
			}
			if (o->m_fEdgeScale != 1.2f)
			{
				ThreadInfo.BoneScale = o->m_fEdgeScale;
			}
		}
		else
		{
			Vector(0.02f, 0.1f, 0.0f, b->BodyLight(ThreadIndex));
			ThreadInfo.BoneScale = o->m_fEdgeScale;
		}

		mu_float Scale = ThreadInfo.BoneScale;
		RenderPartObjectEdge(ThreadIndex, b, o, RENDER_BRIGHT | RENDER_PREBLEND | RENDER_BACKCULL, Translate, Scale);
		
		if (InChaosCastle() == true || o->Kind != KIND_NPC)
		{
			Vector(0.7f, 0.07f, 0.0f, b->BodyLight(ThreadIndex));
			if (o->Type == MODEL_MONSTER01 + 32)
			{
				ThreadInfo.BoneScale = 1.08f;
			}
			else
			{
				ThreadInfo.BoneScale = 1.0f + (0.04f / o->Scale);
			}
			if (o->m_fEdgeScale != 1.2f)
			{
				ThreadInfo.BoneScale = EMath::Max(o->m_fEdgeScale - 0.04f, 1.01f);
			}
		}
		else
		{
			Vector(0.16f, 0.7f, 0.0f, b->BodyLight(ThreadIndex));
			ThreadInfo.BoneScale = EMath::Max(o->m_fEdgeScale - 0.12f, 1.01f);
		}

		Scale = ThreadInfo.BoneScale;
		RenderPartObjectEdge(ThreadIndex, b, o, RENDER_BRIGHT | RENDER_PREBLEND | RENDER_BACKCULL, Translate, Scale);
		BodyLight(ThreadIndex, o, b);
		ThreadInfo.BoneScale = 1.0f;
	}

	ThreadInfo.Translate = Translate;

	b->Transform(ThreadIndex, o->BoundingBoxMin, o->BoundingBoxMax, &o->OBB, Translate);

	return true;
}

mu_boolean Calc_ObjectAnimation(const mu_uint32 ThreadIndex, OBJECT* o, mu_boolean Translate, mu_int32 Select)
{
	if (InChaosCastle() == true && Hero->Object.Flags.Get(OBJECT::eOF_ACTIONSTART) == true)
	{
		o->Alpha -= 0.15f;
	}

	if (o->Alpha < 0.01f) return false;

	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	MUModel *b = &MODELS::Data[o->Type];
	if (b->IsDataLoaded() == false)
	{
		b->RequestLoad();
		return false;
	}

	b->BodyHeight(ThreadIndex) = 0.0f;
	BodyLight(ThreadIndex, o, b);
	b->BodyScale(ThreadIndex) = o->Scale;
	b->CurrentAction(ThreadIndex) = o->CurrentAction;
	VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));

	b->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle, false, !Translate);
	ThreadInfo.SkeletalIndex = g_Tasking.RegisterSkeletal(ThreadInfo.BoneTransform, b->_BoneNonDummyEnd);
	o->BoneTransform = &g_Tasking._SkeletonBuffer[ThreadInfo.SkeletalIndex];

	ThreadInfo.Translate = Translate;

	if (ThreadInfo.SkeletalIndex == FAILED_SKELETAL)
	{
		return false;
	}

	return true;
}

RENDER_STATUS Draw_RenderObject(const mu_uint32 ThreadIndex, OBJECT *o, mu_boolean Translate, mu_int32 Select, mu_int32 ExtraMon)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	MUModel *b = &MODELS::Data[o->Type];
	mu_boolean View = true;

	if (o->HiddenMesh == -2)
	{
		return RENDER_HIDDEN;
	}

	if (ExtraMon == 10)
	{
		mu_float Alpha = 0.5f;

		CHARACTER *c = (CHARACTER *)((mu_size)o->Owner - (mu_size)(&((CHARACTER*)0)->Object));

		if (c != Hero &&
			battleCastle::IsBattleCastleStart() == true &&
			g_isCharacterBuff(o->Owner, eBuff_Cloaking))
		{
			if (Hero->EtcPart == PARTS_ATTACK_KING_TEAM_MARK || Hero->EtcPart == PARTS_ATTACK_TEAM_MARK)
			{
				if (!(c->EtcPart == PARTS_ATTACK_KING_TEAM_MARK || c->EtcPart == PARTS_ATTACK_TEAM_MARK))
				{
					View = false;
				}
			}
			else if (Hero->EtcPart == PARTS_ATTACK_KING_TEAM_MARK2 || Hero->EtcPart == PARTS_ATTACK_TEAM_MARK2)
			{
				if (!(c->EtcPart == PARTS_ATTACK_KING_TEAM_MARK2 || c->EtcPart == PARTS_ATTACK_TEAM_MARK2))
				{
					View = false;
				}
			}
			else if (Hero->EtcPart == PARTS_ATTACK_KING_TEAM_MARK3 || Hero->EtcPart == PARTS_ATTACK_TEAM_MARK3)
			{
				if (!(c->EtcPart == PARTS_ATTACK_KING_TEAM_MARK3 || c->EtcPart == PARTS_ATTACK_TEAM_MARK3))
				{
					View = false;
				}
			}
			else if (Hero->EtcPart == PARTS_DEFENSE_KING_TEAM_MARK || Hero->EtcPart == PARTS_DEFENSE_TEAM_MARK)
			{
				if (!(c->EtcPart == PARTS_DEFENSE_KING_TEAM_MARK || c->EtcPart == PARTS_DEFENSE_TEAM_MARK))
				{
					View = false;
				}
			}
		}

		if (View == false)
			return RENDER_HIDDEN;

		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		for (mu_int32 i = 0; i < b->_MeshCount; ++i)
		{
			b->RenderMesh(ThreadIndex, i, RENDER_BRIGHT | RENDER_CHROME5, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU*8.0f, o->BlendMeshTexCoordV*2.0f, BITMAP_CHROME2);
		}

		return RENDER_SUCCEED;
	}

	if (GET_WORLD == WD_10HEAVEN)
	{
		if (o->Type == MODEL_MONSTER01 + 31)
		{
			Vector(0.02f, 0.05f, 0.15f, b->BodyLight(ThreadIndex));
		}
	}
	if (InDevilSquare())
	{
		if (o->Type == MODEL_MONSTER01 + 18)
		{
			Vector(0.0f, 0.3f, 1.0f, b->BodyLight(ThreadIndex));
		}
	}
	if (ExtraMon && o->Type == MODEL_MONSTER01 + 27)
	{
		Vector(0.0f, 0.0f, 1.0f, b->BodyLight(ThreadIndex));
	}

	if (ThreadInfo.CulledType == CULLED_BOIDS)
	{
		if (GET_WORLD != WD_10HEAVEN &&
			GET_WORLD != WD_51HOME_6TH_CHAR &&
			o->Type != MODEL_MAP_TORNADO)
		{
			ThreadInfo.EnableShadow = true;
			ThreadInfo.ShadowHeight = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (o->Type == MODEL_EAGLE)
			{
				if (o->ShadowScale == 0.0f)
					ThreadInfo.RenderLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 0.0f);
				else
					ThreadInfo.RenderLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 1.0f);
			}
			else
			{
				ThreadInfo.RenderLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 0.2f);
			}
		}
	}
	else if (ThreadInfo.CulledType == CULLED_FISHS)
	{
		if (GET_WORLD != WD_10HEAVEN &&
			o->Type != MODEL_FISH01 + 7 &&
			o->Type != MODEL_FISH01 + 8 &&
			!(InHellas() && o->Type == MODEL_MONSTER01 + 33))
		{
			ThreadInfo.EnableShadow = true;
			ThreadInfo.ShadowHeight = RequestTerrainHeight(o->Position[0], o->Position[1]);
			ThreadInfo.RenderLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 0.2f);
		}
	}
	else if (ThreadInfo.CulledType == CULLED_BUGS)
	{
		if (GET_WORLD != WD_10HEAVEN &&
			GET_WORLD != WD_51HOME_6TH_CHAR &&
			(o->Type == MODEL_UNICON ||
				o->Type == MODEL_PEGASUS))
		{
			ThreadInfo.EnableShadow = true;
			ThreadInfo.ShadowHeight = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (GET_WORLD == WD_7ATLANSE)
			{
				ThreadInfo.RenderLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 0.2f);
			}
			else
			{
				ThreadInfo.RenderLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 0.7f);
			}
		}
	}

	if (o->RenderType == RENDER_DARK)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_DARK, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (o->Type == MODEL_CHANGE_UP_EFF)
	{
		if (o->SubType == 0 || o->SubType == 1)
		{
			Vector(0.1f, 0.4f, 0.6f, b->BodyLight(ThreadIndex));
		}
		else
		{
			Vector(0.1f, 0.2f, 0.9f, b->BodyLight(ThreadIndex));
		}
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type == MODEL_CHANGE_UP_NASA)
	{
		Vector(0.5f, 0.5f, 0.9f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type == MODEL_CHANGE_UP_CYLINDER)
	{
		Vector(0.4f, 0.5f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type == MODEL_SUMMON)
	{
		if (!M39Kanturu3rd::IsInKanturu3rd())
		{
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		}
	}
	else if (o->Type == MODEL_DEASULER)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		Vector(0.3f, 0.4f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type == MODEL_MONSTER01 + 168)
	{
		MoveEye(ThreadIndex, o, b, 14, 15, 71, 72, 73, 74);

		if (!o->m_pCloth)
		{
			CPhysicsCloth *pCloth = new CPhysicsCloth[1];
			pCloth[0].Create(ThreadIndex, o, 10, 0.0f, -10.0f, 0.0f, 5, 12, 150.0f, 190.0f, BITMAP_DEASULER_CLOTH, BITMAP_DEASULER_CLOTH, PCT_MASK_ALPHA | PCT_HEAVY | PCT_STICKED | PCT_SHORT_SHOULDER);
			pCloth[0].AddCollisionSphere(50.0f, -140.0f, -20.0f, 30.0f, 2);
			o->m_pCloth = pCloth;
			o->m_byNumCloth = 1;
		}
		CPhysicsCloth *pCloth = o->m_pCloth;
		if (MU_UPDATECOUNTER > 0 && !pCloth[0].Move2(ThreadIndex, 0.005f, 5))
		{
			CHARACTER *c = &CharactersClient[o->PKKey];
			DeleteCloth(c, o);
		}
		else
		{
			pCloth[0].Render(ThreadIndex, b);
		}

		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		if (o->CurrentAction != MONSTER01_ATTACK2 || o->AnimationFrame < 2.5f)
		{
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(0.4f, 0.5f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
	}
	else if (o->Type == MODEL_MONSTER01 + 176)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.35f;

		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		Vector(fLumi, fLumi, fLumi, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_ASSASSIN_EFFECT1);
	}
	else if (o->Type == MODEL_RAKLION_BOSS_CRACKEFFECT)
	{
		mu_float fLumi = o->Alpha;
		Vector(o->Light[0] * fLumi, o->Light[1] * fLumi, o->Light[2] * fLumi, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type == MODEL_RAKLION_BOSS_MAGIC)
	{
		mu_float fLumi = o->Alpha;
		Vector(o->Light[0] * fLumi, o->Light[1] * fLumi, o->Light[2] * fLumi, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type == MODEL_NIGHTWATER_01)
	{
		mu_float fLumi = o->Alpha;
		Vector(o->Light[0] * fLumi, o->Light[1] * fLumi, o->Light[2] * fLumi, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type == MODEL_KNIGHT_PLANCRACK_A)
	{
		mu_float fLumi = o->Alpha;
		Vector(o->Light[0] * fLumi, o->Light[1] * fLumi, o->Light[2] * fLumi, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 2000 * 0.0001f);
	}
	else if (o->Type == MODEL_KNIGHT_PLANCRACK_B)
	{
		mu_float fLumi = o->Alpha;
		Vector(o->Light[0] * fLumi, o->Light[1] * fLumi, o->Light[2] * fLumi, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 2000 * 0.0001f);
	}
	else if (o->Type == MODEL_ALICE_BUFFSKILL_EFFECT || o->Type == MODEL_ALICE_BUFFSKILL_EFFECT2)
	{
		if (o->SubType == 1)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_DARK, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
	}
	else if (o->Type == MODEL_CURSEDTEMPLE_HOLYITEM)
	{
		if (MU_UPDATECOUNTER > 0)
		{
			EVector3 Light;
			mu_float fLuminosity = (mu_float)EMath::Sin((FWorldTime)*0.0002f)*0.002f;
			Vector(0.9f, 0.4f, 0.4f, Light);
			CreateSprite(BITMAP_LIGHT, o->Position, 3.0f, Light, o, 0.0f, 0);
			Vector(0.9f, 0.6f, 0.6f, Light);
			CreateSprite(BITMAP_POUNDING_BALL, o->Position, 0.9f + fLuminosity, Light, nullptr, (FWorldTime / 10.0f));
			CreateParticle(ThreadIndex, BITMAP_POUNDING_BALL, o->Position, o->Angle, Light, 2, 0.1f, o);
		}

		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 0);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, 0.7f, 0, o->BlendMeshLight*3.0f, o->BlendMeshTexCoordU, -FWorldTime*0.004f);
	}
	else if (o->Type == MODEL_CURSEDTEMPLE_PRODECTION_SKILL)
	{
		Vector(0.3f, 0.3f, 1.0f, b->BodyLight(ThreadIndex));
		VectorCopy(o->Angle, b->BodyAngle(ThreadIndex));
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, -FWorldTime*0.0004f);
	}
	else if (o->Type == MODEL_CURSEDTEMPLE_RESTRAINT_SKILL)
	{
		mu_float fLuminosity = (mu_float)EMath::Sin((FWorldTime)*0.0002f)*0.002f;
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, 0.7f, 0, 0.35f + fLuminosity, -FWorldTime*0.0004f, FWorldTime*0.0004f);
	}
	else if (g_CursedTemple.IsCursedTemple() == true
		&& (o->Type == 32 || o->Type == 39 || o->Type == 41 || o->Type == 46 || o->Type == 62
			|| o->Type == 67 || o->Type == 68 || o->Type == 64 || o->Type == 65 || o->Type == 66 || o->Type == 80)
		)
	{
		if (o->Type == 32)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight);
			b->StreamMesh(ThreadIndex) = 2;
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, -IWorldTime % 4000 * 0.00025f, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, -IWorldTime % 5000 * 0.0002f, o->BlendMeshTexCoordV);
			b->StreamMesh(ThreadIndex) = -1;
		}
		else if (o->Type == 39 || o->Type == 41)
		{
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight);
		}
		else if (o->Type == 46)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight);
		}
		else if (o->Type == 62)
		{
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight);
		}
		else if (o->Type == 67)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight);
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight);
		}
		else if (o->Type == 68)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight);
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight);
		}
		else if (o->Type == 64 || o->Type == 65 || o->Type == 66 || o->Type == 80)
		{
			g_CursedTemple.RenderObjectMesh(ThreadIndex, o, b);
		}
	}
	else if (o->Type == MODEL_EFFECT_SKURA_ITEM)
	{
		b->RenderBody(ThreadIndex, RENDER_COLOR, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (o->Type == MODEL_MONSTER01 + 26)
	{
		Vector(0.4f, 0.6f, 1.0f, b->BodyLight(ThreadIndex));
		b->StreamMesh(ThreadIndex) = 0;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		b->StreamMesh(ThreadIndex) = -1;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (o->Type == MODEL_PEGASUS)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (o->Type == MODEL_SKIN_SHELL)
	{
		if (o->SubType == 0)
		{
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
		}
		else if (o->SubType == 1)
		{
			Vector(0.1f, 0.5f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
		}
	}
	else if (o->Type == MODEL_STUN_STONE)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_CHROME, 0.5f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU*5.0f, o->BlendMeshTexCoordV*2.0f, -1, BITMAP_CHROME);
	}
	else if (o->Type == MODEL_DARK_HORSE && o->SubType == 1)
	{
		if (GET_WORLD == WD_65DOPPLEGANGER1)
		{
			//Vector(0.2f, 0.2f, 0.2f, b->BodyLight(ThreadIndex));
			o->Alpha = 0.7f;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else
		{
			Vector(0.2f, 0.2f, 0.2f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
	}
	else if (o->Type == MODEL_DARK_HORSE)
	{
		if (GET_WORLD != WD_10HEAVEN && InHellas() == false)
		{
			if (!g_Direction.m_CKanturu.IsMayaScene())
			{
				ThreadInfo.EnableShadow = true;
				ThreadInfo.ShadowHeight = b->BodyOrigin(ThreadIndex)[2];

				if (GET_WORLD == WD_7ATLANSE)
				{
					ThreadInfo.RenderLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 0.2f);
				}
				else
				{
					ThreadInfo.RenderLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 0.7f);
				}
			}
		}

		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		Vector(1.0f, 0.8f, 0.3f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 12, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 13, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 14, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 15, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		ThreadInfo.EnableShadow = false;
	}
	else if (o->Type == MODEL_FENRIR_BLACK || o->Type == MODEL_FENRIR_BLUE || o->Type == MODEL_FENRIR_RED
		|| o->Type == MODEL_FENRIR_GOLD
		)
	{
		b->BodyLight(ThreadIndex)[0] = 1.0f;
		b->BodyLight(ThreadIndex)[1] = 1.0f;
		b->BodyLight(ThreadIndex)[2] = 1.0f;

		if (GET_WORLD != WD_10HEAVEN && InHellas() == false)
		{
			if (!g_Direction.m_CKanturu.IsMayaScene())
			{
				ThreadInfo.EnableShadow = true;
				ThreadInfo.ShadowHeight = b->BodyOrigin(ThreadIndex)[2];

				if (GET_WORLD == WD_7ATLANSE)
				{
					ThreadInfo.RenderLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 0.2f);
				}
				else
				{
					ThreadInfo.RenderLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 0.7f);
				}
			}
		}

		if (o->Type == MODEL_FENRIR_GOLD)
		{
			b->StreamMesh(ThreadIndex) = 0;

			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			
			b->StreamMesh(ThreadIndex) = -1;
		}
		else
		{
			b->StreamMesh(ThreadIndex) = 0;

			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			if (o->CurrentAction == FENRIR_ATTACK_SKILL)
			{
				b->BodyLight(ThreadIndex)[0] = 1.0f;
				b->BodyLight(ThreadIndex)[1] = 1.0f;
				b->BodyLight(ThreadIndex)[2] = 1.0f;

				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}
			b->StreamMesh(ThreadIndex) = -1;
		}

		ThreadInfo.EnableShadow = false;
	}
	else if (o->Type >= MODEL_FACE &&
		o->Type <= MODEL_FACE + 5)
	{
		Vector(4.8f, 4.8f, 4.8f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
	}
	else if (o->Type == MODEL_DARKLORD_SKILL)
	{
		VectorCopy(o->Light, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
	}
	else if (o->Type == MODEL_DARK_SPIRIT)
	{
		if (GET_WORLD != WD_10HEAVEN && InHellas() == false)
		{
			if (!g_Direction.m_CKanturu.IsMayaScene())
			{
				ThreadInfo.EnableShadow = true;
				ThreadInfo.ShadowHeight = RequestTerrainHeight(o->Position[0], o->Position[1]);

				if (GET_WORLD == WD_7ATLANSE)
				{
					ThreadInfo.RenderLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 0.2f);
				}
				else
				{
					ThreadInfo.RenderLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 1.0f);
				}
			}
		}

		ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);
		b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		if (o->WeaponLevel >= 40)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			Vector(0.3f, 0.6f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 3, RENDER_BRIGHT | RENDER_TEXTURE, o->Alpha, 3, EMath::Sin(FWorldTime*0.001f), o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_MONSTER_SKIN + 2);
		}
		else if (o->WeaponLevel >= 20)
		{
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else
		{
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}

		ThreadInfo.EnableShadow = false;
	}
	else if (GET_WORLD == WD_0LORENCIA && o->Type == MODEL_WATERSPOUT)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight);
		b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, 3, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (GET_WORLD == WD_4LOSTTOWER && (o->Type == 23 || o->Type == 19 || o->Type == 20 || o->Type == 3 || o->Type == 4))
	{
		EVector3 Light, p;
		mu_float Luminosity;
		Luminosity = (mu_float)(GetLargeRand(2) + 6)*0.1f;
		Vector(Luminosity*0.4f, Luminosity*0.8f, Luminosity*1.0f, Light);
		Vector(0.0f, 0.0f, 0.0f, p);
		if (o->Type == 23)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == 19 || o->Type == 20)
		{
			VectorCopy(b->BodyLight(ThreadIndex), Light);
			Vector(1.0f, 0.2f, 0.1f, b->BodyLight(ThreadIndex));
			b->StreamMesh(ThreadIndex) = 2;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
			VectorCopy(Light, b->BodyLight(ThreadIndex));
			b->StreamMesh(ThreadIndex) = -1;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == 3 || o->Type == 4)
		{
			VectorCopy(b->BodyLight(ThreadIndex), Light);
			Vector(1.0f, 0.2f, 0.1f, b->BodyLight(ThreadIndex));
			b->StreamMesh(ThreadIndex) = 1;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
			VectorCopy(Light, b->BodyLight(ThreadIndex));
			b->StreamMesh(ThreadIndex) = -1;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
	}
	else if (GET_WORLD == WD_8TARKAN && (o->Type == 81))
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
	}
	else if (InBloodCastle() == true && (o->Type == 28 || o->Type == 29))
	{
		ThreadInfo.EnableShadow = true;
		ThreadInfo.ShadowHeight = RequestTerrainHeight(o->Position[0], o->Position[1]);
		ThreadInfo.RenderLight = MU::Float4ToARGB(0.0f, 0.0f, 0.0f, 1.0f);

		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		ThreadInfo.EnableShadow = false;
	}
	else if (GET_WORLD == WD_3NORIA && o->Type == MODEL_WARP3)
	{
		b->BodyLight(ThreadIndex)[0] = 0.8f;
		b->BodyLight(ThreadIndex)[1] = 0.8f;
		b->BodyLight(ThreadIndex)[2] = 0.8f;

		b->StreamMesh(ThreadIndex) = 0;
		b->RenderMesh(ThreadIndex, 0, RENDER_DARK, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->StreamMesh(ThreadIndex) = -1;
	}
	else
	{
		mu_boolean bIsRendered = true;
		if (g_isCharacterBuff(o, eDeBuff_Poison) && g_isCharacterBuff(o, eDeBuff_Freeze))
		{
			Vector(0.3f, 1.0f, 0.8f, b->BodyLight(ThreadIndex));
		}
		else if (g_isCharacterBuff(o, eDeBuff_Poison))
		{
			Vector(0.3f, 1.0f, 0.5f, b->BodyLight(ThreadIndex));
		}
		else if (g_isCharacterBuff(o, eDeBuff_Freeze))
		{
			Vector(0.3f, 0.5f, 1.0f, b->BodyLight(ThreadIndex));
		}
		else if (g_isCharacterBuff(o, eDeBuff_BlowOfDestruction))
		{
			Vector(0.3f, 0.5f, 1.0f, b->BodyLight(ThreadIndex));
		}

		if (o->Type == MODEL_MONSTER01 + 35 || o->Type == MODEL_MONSTER01 + 15 || o->Type == MODEL_MONSTER01 + 50 || o->Type == MODEL_MONSTER01 + 51)
		{
			if (o->Alpha == 1.0f && o->BlendMeshLight == 0.05f)
			{
				o->BlendMeshLight = 1.0f;
			}
		}

		if (TheMapProcess().RenderObjectMesh(ThreadIndex, o, b, ExtraMon)) {}
		else if (o->Type == MODEL_KALIMA_SHOP)
		{
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else if (o->Type == MODEL_NPC_QUARREL)
		{
			Vector(0.5f, 0.5f, 0.8f, b->BodyLight(ThreadIndex));
			for (mu_int32 i = 0; i < b->_MeshCount; ++i)
			{
				b->RenderMesh(ThreadIndex, i, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, i, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
			}
		}
		else if (o->Type == MODEL_SEED_MASTER)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			mu_float fLumi = (EMath::Sin(FWorldTime*0.001f) + 1.0f) * 0.2f + 0.0f;
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, fLumi, 1, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else if (o->Type == MODEL_MONSTER01 + 45)
		{
			if (ExtraMon)
			{
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));

				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
				b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, 2, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
			}
			else
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
		}
		else if (o->Type == MODEL_MONSTER01 + 42 && o->SubType == 1)
		{
			mu_float Light = EMath::Sin(FWorldTime*0.002f)*0.01f + 1.0f;
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, Light, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_MONSTER_SKIN + 1);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, Light, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_MONSTER_SKIN);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, Light, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, Light, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else if (o->Type == MODEL_MONSTER01 + 62)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, 2, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else if (o->Type == MODEL_MONSTER01 + 54)
		{
			if (ExtraMon)
			{
				Vector(0.1f, 0.1f, 0.1f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1);
				Vector(1.0f, 0.1f, 0.1f, b->BodyLight(ThreadIndex));
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}
			else
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
		}
		else if (o->Type == MODEL_MONSTER01 + 46)
		{
			if (ExtraMon)
			{
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_DEST_ORC_WAR0);
				b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_DEST_ORC_WAR2);
			}
			else
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
		}
		else if (o->Type == MODEL_MONSTER01 + 47)
		{
			if (ExtraMon)
			{
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_DEST_ORC_WAR1);
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_DEST_ORC_WAR0);
			}
			else
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
		}
		else if (o->Type == MODEL_MONSTER01 + 48)
		{
			if (ExtraMon)
			{
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1, BITMAP_WHITE_WIZARD);
			}
			else
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
		}
		else if (o->Type == MODEL_MONSTER01 + 52)
		{
			if (ExtraMon == 0)
			{
				mu_float Luminosity = EMath::Sin(FWorldTime*0.002f)*0.3f + 0.5f;
				Vector(Luminosity + 0.5f, 0.3f - Luminosity*0.5f, -Luminosity*0.5f + 0.5f, b->BodyLight(ThreadIndex));
				b->StreamMesh(ThreadIndex) = 0;
				Vector(.4f, .3f, .5f, b->BodyLight(ThreadIndex));
				++ThreadInfo.AlphaTestIndex;
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_JANUSEXT);
				--ThreadInfo.AlphaTestIndex;
				Vector(.5f, .5f, .5f, b->BodyLight(ThreadIndex));
				b->StreamMesh(ThreadIndex) = -1;
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
			else if (ExtraMon == 301)
			{
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
			}
			else
			{	
				Vector(0.1f, 1.0f, .8f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
		}
		else if (o->Type == MODEL_MONSTER01 + 53) 
		{
			Vector(.9f, .8f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME + 1);
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_MONSTER01 + 51)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1, -1);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_WAVE, 0.5f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_WAVE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_MONSTER01 + 127)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_MOONHARVEST_MOON)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_MOONHARVEST_GAM)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_MOONHARVEST_SONGPUEN1)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_MOONHARVEST_SONGPUEN2)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_NPC_CHERRYBLOSSOM)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, 2.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_CHROME7, 0.25f, o->BlendMesh, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, 0.8f, o->BlendMesh, 2.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 0);
		}
		else if (o->Type == MODEL_NPC_CHERRYBLOSSOMTREE)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_MONSTER01 + 56)
		{
			b->StreamMesh(ThreadIndex) = 0;
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_PREBLEND, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
			b->StreamMesh(ThreadIndex) = -1;
			Vector(.6f, .6f, .6f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_MONSTER01 + 60)    
		{
			if (o->CurrentAction == MONSTER01_DIE ||
				(MoveSceneFrame - o->InitialSceneFrame) < 25
				)
			{
				if (o->CurrentAction == MONSTER01_DIE)
				{
					o->Flags.Clear(OBJECT::eOF_LIVE);
				}

				PlayBackground(SOUND_HIT_GATE2);
				b->RenderMeshEffect(ThreadIndex, o->BoneTransform, 0, MODEL_STONE_COFFIN);
			}
			else
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
				Vector(0.3f, 0.3f, 1.0f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_METAL, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
			}
		}
		else if (o->Type == MODEL_MONSTER01 + 61 && o->CurrentAction == MONSTER01_DIE)
		{
			o->Flags.Clear(OBJECT::eOF_LIVE);

			PlayBackground(SOUND_HIT_GATE2);
			b->RenderMeshEffect(ThreadIndex, o->BoneTransform, 0, MODEL_GATE);
		}
		else if (o->Type == MODEL_STONE_COFFIN || o->Type == MODEL_STONE_COFFIN + 1)
		{
			if (o->SubType == 2 || o->SubType == 3)
			{
				if (o->SubType == 2)
				{
					Vector(0.1f, 0.3f, 0.6f, b->BodyLight(ThreadIndex));
				}
				else
				{
					Vector(0.1f, 0.6f, 0.3f, b->BodyLight(ThreadIndex));
				}
				b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
			}
			else
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
				Vector(0.3f, 0.3f, 1.0f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_METAL, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
			}
		}
		else if (o->Type == MODEL_FLY_BIG_STONE1)
		{
			if (o->HiddenMesh == 99)
			{
				o->HiddenMesh = -2;
				b->RenderMeshEffect(ThreadIndex, o->BoneTransform, 0, MODEL_BIG_STONE_PART1);
			}
			else
			{
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				if (o->SubType <= 1)
				{
					Vector(1.0f, 0.2f, 0.1f, b->BodyLight(ThreadIndex));
					b->RenderBody(ThreadIndex, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				}
			}
		}
		else if (o->Type == MODEL_FLY_BIG_STONE2)
		{
			if (o->HiddenMesh == 99)
			{
				o->HiddenMesh = -2;
				b->RenderMeshEffect(ThreadIndex, o->BoneTransform, 0, MODEL_BIG_STONE_PART2);
			}
			else
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
		}
		else if (o->Type == MODEL_BIG_STONE_PART1 || o->Type == MODEL_BIG_STONE_PART2)
		{
			if (o->SubType == 1)
			{
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				Vector(1.0f, 0.2f, 0.1f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			}
			else if (o->SubType == 2 || o->SubType == 3)
			{
				Vector(0.5f, 1.0f, 0.3f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			}
			else
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
		}
		else if (o->Type == MODEL_WALL_PART1 || o->Type == MODEL_WALL_PART2)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_MONSTER01 + 70 || o->Type == MODEL_MONSTER01 + 71)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_COMBO && o->SubType == 1)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | o->RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_CIRCLE_LIGHT && (o->SubType == 3 || o->SubType == 4))
		{
			Vector(0.1f, 0.1f, 10.0f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | o->RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		}
		else if (o->Type == MODEL_CIRCLE && (o->SubType == 2 || o->SubType == 3))
		{
			Vector(0.5f, 0.5f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_MAGIC_EMBLEM);
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		}
		else if (o->Type == MODEL_MULTI_SHOT1 || o->Type == MODEL_MULTI_SHOT2 || o->Type == MODEL_MULTI_SHOT3)
		{
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_DESAIR)
		{
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_DARK, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_DARK_SCREAM)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_DARK_SCREAM_FIRE)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_ARROW_SPARK)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_CHROME5 | RENDER_BRIGHT, o->Alpha, o->BlendMesh, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_SKULL)
		{
			Vector(1.0f, 0.6f, 0.3f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_WAVES)
		{
			if (o->SubType == 3)
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1, BITMAP_PINK_WAVE);
			else if (o->SubType == 4 || o->SubType == 5)
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1, BITMAP_PINK_WAVE);
			else
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_PROTECTGUILD)
		{
			Vector(0.4f, 0.6f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_MANA_RUNE && o->SubType == 0)
		{
			Vector(0.3f, 0.6f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_SKILL_JAVELIN)
		{
			Vector(1.0f, 0.6f, 0.3f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1);
		}
		else if (o->Type == MODEL_MAGIC_CAPSULE2 && o->SubType == 1)
		{
			b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME4, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1);
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_ARROW_AUTOLOAD)
		{
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_INFINITY_ARROW)
		{
		}
		else if (o->Type >= MODEL_INFINITY_ARROW && o->Type <= MODEL_INFINITY_ARROW4)
		{
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_SHIELD_CRASH || o->Type == MODEL_SHIELD_CRASH2)
		{
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_IRON_RIDER_ARROW)
		{
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_BLADE_SKILL)
		{
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			for (mu_int32 abc = 0; abc < 3; ++abc)
				b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_KENTAUROS_ARROW)
		{
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_XMAS_EVENT_EARRING)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, -FWorldTime*0.0002f, o->BlendMeshTexCoordV, BITMAP_CHROME);
		}
		else if (o->Type == MODEL_XMAS_EVENT_ICEHEART)
		{
			b->StreamMesh(ThreadIndex) = 0;
			Vector(1.0f, 0.4f, 0.4f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, -IWorldTime % 2000 * 0.0005f, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->StreamMesh(ThreadIndex) = -1;
		}
		else if (o->Type == MODEL_ARROW_BEST_CROSSBOW)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_NEWYEARSDAY_EVENT_PIG)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			Vector(1.0f, 0.4f, 0.2f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_CHROME3 | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		}
		else if (o->Type == MODEL_FENRIR_THUNDER)
		{
			if (o->SubType == 1)
			{
				VectorCopy(o->Light, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			}
			else
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
		}
		else if (o->Type == MODEL_MAP_TORNADO)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1);
		}
		else if (o->Type >= MODEL_SUMMONER_CASTING_EFFECT1 && o->Type <= MODEL_SUMMONER_CASTING_EFFECT4)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1);
		}
		else if (o->Type == MODEL_SUMMONER_SUMMON_SAHAMUTT)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1);
			b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME7, o->Alpha, o->BlendMesh, o->Alpha, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1);
		}
		else if (o->Type == MODEL_SUMMONER_SUMMON_NEIL)
		{
			if (o->Alpha < 0.7f)
			{
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->Alpha, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1);
				b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 2, o->Alpha, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1);
			}
			else
			{
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1);
				b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1);
			}
			Vector(1.0f, 0.0f, 0.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, 1, o->Alpha, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1);
		}
		else if (o->Type >= MODEL_SUMMONER_SUMMON_NEIL_NIFE1 && o->Type <= MODEL_SUMMONER_SUMMON_NEIL_NIFE3)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1);
			Vector(1.0f, 0.0f, 0.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, o->Alpha, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1);
		}
		else if (o->Type >= MODEL_SUMMONER_SUMMON_NEIL_GROUND1 && o->Type <= MODEL_SUMMONER_SUMMON_NEIL_GROUND3)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->Alpha, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, o->Alpha, -FWorldTime*0.001f, o->BlendMeshTexCoordV, -1);
		}
		else if (o->Type == MODEL_SUMMONER_SUMMON_LAGUL)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->Alpha, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, o->Alpha, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1);
		}
		else if (o->Type >= MODEL_EFFECT_BROKEN_ICE0 && o->Type <= MODEL_EFFECT_BROKEN_ICE3)
		{
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_MOVE_TARGETPOSITION_EFFECT)
		{
			Vector(1.0f, 0.7f, 0.3f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight);
		}
		else if (o->Type == MODEL_ARROW_DARKSTINGER)
		{
			b->BodyLight(ThreadIndex)[0] = 0.7f;
			b->BodyLight(ThreadIndex)[1] = 0.7f;
			b->BodyLight(ThreadIndex)[2] = 0.9f;

			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, 0, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			b->BodyLight(ThreadIndex)[0] = 0.3f;
			b->BodyLight(ThreadIndex)[1] = 0.4f;
			b->BodyLight(ThreadIndex)[2] = 0.9f;
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, 1, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			b->BodyLight(ThreadIndex)[0] = 1.0f;
			b->BodyLight(ThreadIndex)[1] = 1.0f;
			b->BodyLight(ThreadIndex)[2] = 1.0f;
		}
		else if (o->Type == MODEL_FEATHER)
		{
			if (o->SubType == 2 || o->SubType == 3)
				b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_DARK, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			else
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, 0, o->Alpha, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_ARROW_GAMBLE)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->Alpha, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_HELPER + 64)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_HELPER + 65)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else
		{
			bIsRendered = false;
		}

		if (bIsRendered == true)
		{

		}
		else if (o->Type == MODEL_XMAS2008_SNOWMAN)
		{
			if (o->CurrentAction == MONSTER01_DIE)
			{
			}
			else
			{
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, 1, 0.5f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
		}
		else if (o->Type == MODEL_XMAS2008_SNOWMAN_BODY)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, 1, 0.5f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_FEATHER_FOREIGN)
		{
			if (o->SubType == 2 || o->SubType == 3)
				b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_DARK, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			else
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, 0, o->Alpha, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_SWELL_OF_MAGICPOWER)
		{
			o->BlendMesh = 0;
			Vector(0.7f, 0.4f, 0.9f, b->BodyLight(ThreadIndex));

			if (o->LifeTime <= 20 &&
				MU_UPDATECOUNTER > 0)
			{
				o->BlendMeshLight *= 0.86f;
			}

			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_ARROWSRE06)
		{
			o->BlendMesh = 0;
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
			
			if (o->LifeTime <= 10 &&
				MU_UPDATECOUNTER > 0)
			{
				o->BlendMeshLight *= 0.8f;
			}

			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_XMAS2008_SNOWMAN_NPC)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, 1, 0.5f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_XMAS2008_SANTA_NPC)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_MONSTER01 + 155)
		{
			if (o->CurrentAction == MONSTER01_DIE)
			{
				EVector3 vLight;
				Vector(1.0 * o->Alpha, 0.8 * o->Alpha, 0.5 * o->Alpha, vLight);
				EVector3 vPosition;
				Vector(o->Position[0], o->Position[1], o->Position[2] + 200, vPosition);

				if (o->AnimationFrame >= 13)
				{
					o->Angle[2] = 45;

					b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_GOOD_SANTA);
					b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_GOOD_SANTA_BAGGAGE);
					b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				}
				else if (o->AnimationFrame >= 9)
				{
					o->AI = 3;
					o->Alpha = 1.0f;

					mu_float fFade = (13.0f - o->AnimationFrame) / (13.0f - 9.0f);
					b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_GOOD_SANTA);
					b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_GOOD_SANTA_BAGGAGE);
					b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, 1.0f - fFade, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

					b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, fFade, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
					b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, fFade, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				}
				else
				{
					b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
					b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				}
			}
			else
			{
				VectorCopy(o->Position, o->StartPosition);
				o->WeaponLevel = 0;
				o->AI = 4;
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
		}
		else if (o->Type >= MODEL_LITTLESANTA && o->Type <= MODEL_LITTLESANTA_END)
		{
			if (MU_UPDATECOUNTER > 0)
			{
				mu_float fLumi = (EMath::Sin(FWorldTime*0.004f) + 1.2f) * 0.5f + 0.1f;
				mu_float Rotation = (mu_float)((mu_int32)(FWorldTime*0.1f) % 360);
				EVector3 vWorldPos, vLight;

				switch (o->Type)
				{
				case MODEL_LITTLESANTA:
					Vector(0.5f, 0.5f, 0.0f, vLight);
					break;
				case MODEL_LITTLESANTA + 1:
					Vector(0.3f, 0.8f, 0.4f, vLight);
					break;
				case MODEL_LITTLESANTA + 2:
					Vector(0.8f, 0.1f, 0.1f, vLight);
					break;
				case MODEL_LITTLESANTA + 3:
					Vector(0.3f, 0.3f, 0.8f, vLight);
					break;
				case MODEL_LITTLESANTA + 4:
					Vector(0.9f, 0.9f, 0.9f, vLight);
					break;
				case MODEL_LITTLESANTA + 5:
					Vector(0.9f, 0.9f, 0.9f, vLight);
					break;
				case MODEL_LITTLESANTA + 6:
					Vector(0.8f, 0.4f, 0.0f, vLight);
					break;
				case MODEL_LITTLESANTA + 7:
					Vector(0.9f, 0.5f, 0.7f, vLight);
					break;
				default:
					break;
				}

				b->TransformByObjectBone(ThreadIndex, vWorldPos, o, 17);
				CreateSprite(BITMAP_LIGHTMARKS, vWorldPos, 0.7f, vLight, o, Rotation);
				CreateSprite(BITMAP_LIGHTMARKS, vWorldPos, 1.3f, vLight, o, Rotation);

				b->TransformByObjectBone(ThreadIndex, vWorldPos, o, 20);
				CreateSprite(BITMAP_LIGHTMARKS, vWorldPos, 0.7f, vLight, o, Rotation);
				CreateSprite(BITMAP_LIGHTMARKS, vWorldPos, 1.3f, vLight, o, Rotation);

				RenderAurora(BITMAP_LIGHTMARKS, RENDER_BRIGHT, o->Position[0], o->Position[1], 2.0f + fLumi, 2.0f + fLumi, vLight);
				RenderAurora(BITMAP_LIGHTMARKS, RENDER_BRIGHT, o->Position[0], o->Position[1], 0.5f + fLumi, 0.5 + fLumi, vLight);

				if (GetLargeRand(50) <= 5)
				{
					CreateParticle(ThreadIndex, BITMAP_LIGHT + 3, o->Position, o->Angle, vLight, 0, 1.0f);
				}
			}

			b->RenderBody(ThreadIndex, RENDER_TEXTURE, 0.9f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			
			Vector(b->BodyLight(ThreadIndex)[0] * 0.5f, b->BodyLight(ThreadIndex)[0] * 0.5f, b->BodyLight(ThreadIndex)[0] * 0.5f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_TEXTURE, 0.9f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else if (o->Type == MODEL_DUEL_NPC_TITUS)
		{
			if (MU_UPDATECOUNTER > 0)
			{
				const mu_int32 iBones[] = { 9, 45, 23, 11 };
				EVector3 vLight, vPos, vRelative;
				Vector(1.0f, 1.0f, 1.0f, vLight);
				EVector3 vLightFire;
				Vector(1.0f, 0.2f, 0.0f, vLightFire);
				for (mu_int32 i = 0; i < 4; ++i)
				{
					mu_float fScale = 1.0f;
					if (i == 0)
					{
						b->TransformByObjectBone(ThreadIndex, vPos, o, iBones[i]);
						CreateSprite(BITMAP_LIGHT, vPos, 3.0f, vLightFire, o);

						fScale = 0.8f;
						Vector((GetLargeRand(10) - 5)*1.0f, (GetLargeRand(10) - 5)*1.0f, (GetLargeRand(10) - 5)*1.0f, vRelative);
						b->TransformByObjectBone(ThreadIndex, vPos, o, iBones[i], &vRelative);

					}
					else
					{
						b->TransformByObjectBone(ThreadIndex, vPos, o, iBones[i]);
						CreateSprite(BITMAP_LIGHT, vPos, 2.0f, vLightFire, o);

						fScale = 0.6f;
						Vector((GetLargeRand(10) - 5)*1.0f, (GetLargeRand(10) - 5)*1.0f, (GetLargeRand(10) - 5)*1.0f, vRelative);
						b->TransformByObjectBone(ThreadIndex, vPos, o, iBones[i], &vRelative);
					}

					switch (GetLargeRand(3))
					{
					case 0:
						CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1, vPos, o->Angle, vLight, 3, fScale);
						break;
					case 1:
						CreateParticle(ThreadIndex, BITMAP_FIRE_CURSEDLICH, vPos, o->Angle, vLight, 7, fScale);
						break;
					case 2:
						CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3, vPos, o->Angle, vLight, 3, fScale);
						break;
					}
				}
			}
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, FWorldTime*0.0001f);
		}
		else if (o->Type == MODEL_GAMBLE_NPC_MOSS)
		{
			if (MU_UPDATECOUNTER > 0)
			{
				EVector3 vRelativePos, vWorldPos, Light;
				Vector(0.0f, 0.0f, 0.0f, vRelativePos);

				Vector(0.8f, 0.8f, 0.8f, Light);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(41)], vRelativePos, vWorldPos, true);
				CreateParticle(ThreadIndex, BITMAP_SMOKELINE1 + GetLargeRand(3), vWorldPos, o->Angle, Light, 1, 0.6f, o);
				CreateParticle(ThreadIndex, BITMAP_CLUD64, vWorldPos, o->Angle, Light, 6, 0.6f, o);

				Vector(0.5f, 0.5f, 0.5f, Light);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(55)], vRelativePos, vWorldPos, true);
				CreateSprite(BITMAP_LIGHT, vWorldPos, 2.0f, Light, o);
			}
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_DOPPELGANGER_NPC_LUGARD)	
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 5, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 5, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 5, 0.1f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else if (o->Type == MODEL_DOPPELGANGER_NPC_BOX || o->Type == MODEL_DOPPELGANGER_NPC_GOLDENBOX)
		{
			if (o->CurrentAction == MONSTER01_DIE)
			{
				o->Alpha = (10 - o->AnimationFrame) * 0.1f;

				if (o->AnimationFrame > 9)
					o->AnimationFrame = 9;
			}

			if (o->Type == MODEL_DOPPELGANGER_NPC_BOX)
			{
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}
			else if (o->Type == MODEL_DOPPELGANGER_NPC_GOLDENBOX)
			{
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_DOPPELGANGER_GOLDENBOX2);
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_DOPPELGANGER_GOLDENBOX1);
				b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME7, 0.5f, 1, 0.3f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}

			if (o->CurrentAction == MONSTER01_DIE && o->AnimationFrame > 1 && o->AnimationFrame < 9)
			{
				if (MU_UPDATECOUNTER > 0)
				{
					EVector3 vPos, vLight;

					for (mu_int32 i = 0; i < 1; ++i)
					{
						vPos[0] = o->Position[0] + GetLargeRand(120) - 60;
						vPos[1] = o->Position[1] + GetLargeRand(120) - 60;
						vPos[2] = o->Position[2];
						Vector(0.3f, 0.3f, 0.3f, vLight);
						CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 54, 2.0f);
						Vector(0.3f, 0.3f, 0.3f, vLight);
						CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 66, 2.0f, o);
					}

					if (GetLargeRand(2) == 0)
					{
						vPos[0] = o->Position[0] + GetLargeRand(80) - 40;
						vPos[1] = o->Position[1] + GetLargeRand(80) - 40;
						vPos[2] = o->Position[2] + GetLargeRand(50) + 10;
						Vector(0.2f, 0.5f, 1.0f, vLight);
						CreateParticle(ThreadIndex, BITMAP_PIN_LIGHT, vPos, o->Angle, vLight, 2, 1.0f, o);
						vPos[2] = o->Position[2] + GetLargeRand(50) + 10;
						Vector(1.0f, 0.8f, 0.5f, vLight);
						CreateParticle(ThreadIndex, BITMAP_PIN_LIGHT, vPos, o->Angle, vLight, 3, 0.2f, o);
					}

					Vector(0.9f, 0.7f, 0.0f, vLight);
					CreateParticle(ThreadIndex, BITMAP_SHINY, vPos, o->Angle, vLight, 3, 0.5f, o);
					CreateParticle(ThreadIndex, BITMAP_SHINY, vPos, o->Angle, vLight, 3, 0.5f, o);

					vPos[0] = o->Position[0];
					vPos[1] = o->Position[1];
					if (o->Type == MODEL_DOPPELGANGER_NPC_GOLDENBOX)
						vPos[2] = o->Position[2] + 80;
					else
						vPos[2] = o->Position[2] + 50;
					Vector(1.0f, 1.0f, 1.0f, vLight);
					mu_float fRot = (FWorldTime*0.0006f) * 360.0f;
					CreateSprite(BITMAP_DS_EFFECT, vPos, 2.5f, vLight, o, fRot);
					Vector(0.9f, 0.7f, 0.0f, vLight);
					CreateSprite(BITMAP_SHINY + 6, vPos, 0.8f, vLight, o, -fRot);
					mu_float fLight = (EMath::Sin(FWorldTime*0.01f) + 1.0f)*0.5f*0.9f + 0.1f;
					Vector(1.0f*fLight, 1.0f*fLight, 1.0f*fLight, vLight);
					CreateSprite(BITMAP_FLARE, vPos, 1.5f, vLight, o);
				}
			}
		}
		else if (o->Type == MODEL_DOPPELGANGER_SLIME_CHIP)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_MONSTER01 + 154)
		{
			if (o->CurrentAction == MONSTER01_DIE)
			{
				Vector(1.0f, 0.6f, 0.9f, b->BodyLight(ThreadIndex));
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, -2, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}
			else
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				EVector3 vOriBodyLight;
				VectorCopy(b->BodyLight(ThreadIndex), vOriBodyLight);
				Vector(1.0f, 0.6f, 0.5f, b->BodyLight(ThreadIndex));
				b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				VectorCopy(vOriBodyLight, b->BodyLight(ThreadIndex));
			}
		}
		else if (o->Type == MODEL_EFFECT_UMBRELLA_GOLD)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else if (o->Type == MODEL_EFFECT_SD_AURA)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, FWorldTime*0.0006f);
		}
		else if (o->Type == MODEL_UNITEDMARKETPLACE_CHRISTIN)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (o->Type == MODEL_UNITEDMARKETPLACE_RAUL)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 5, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 6, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 7, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 8, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 9, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 10, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 11, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 12, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 13, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 14, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else if (o->Type == MODEL_UNITEDMARKETPLACE_JULIA)
		{
			if (MU_UPDATECOUNTER > 0)
			{
				EVector3 vRelativePos, vWorldPos, Light;
				Vector(0.0f, 0.0f, 0.0f, vRelativePos);
				mu_float fScale = 0.0f;

				if (o->CurrentAction == 0 && GetLargeRand(5) == 0)
				{
					Vector(0.0f, 0.0f, 0.0f, vWorldPos);
					Vector((GetLargeRand(90) + 10)*0.01f, (GetLargeRand(90) + 10)*0.01f, (GetLargeRand(90) + 10)*0.01f, Light);
					fScale = (GetLargeRand(5) + 5)*0.1f;
					Vector(0.0f, 0.0f, 0.0f, vRelativePos);
					vRelativePos[0] = -5 + (GetLargeRand(1000) - 500)*0.01f;
					vRelativePos[1] = (GetLargeRand(300) - 150)*0.01f;
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(127)], vRelativePos, vWorldPos, true);
					CreateParticle(ThreadIndex, BITMAP_LIGHT, vWorldPos, o->Angle, Light, 13, fScale, o);
					CreateParticle(ThreadIndex, BITMAP_LIGHT, vWorldPos, o->Angle, Light, 12, fScale*0.1f, o);
				}

				Vector(0.0f, 0.0f, 0.0f, vRelativePos);
				Vector(0.0f, 0.0f, 0.0f, vWorldPos);
				Vector(1.0f, 1.0f, 1.0f, Light);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(71)], vRelativePos, vWorldPos, true);
				CreateSprite(BITMAP_LIGHT, vWorldPos, 3.0f, Light, o);

				EVector3	vLight;
				mu_float fLight = (EMath::Sin(FWorldTime*0.001f) + 1.0f)*0.5f*0.9f + 0.4f;
				Vector(1.0f*fLight, 1.0f*fLight, 1.0f*fLight, vLight);
				CreateSprite(BITMAP_FLARE, vWorldPos, 1.5f, vLight, o);
			}

			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		
		if (g_isCharacterBuff(o, eDeBuff_Freeze))
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, -2, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else if (g_isCharacterBuff(o, eDeBuff_BlowOfDestruction))
		{
			Vector(0.3f, 0.5f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, -2, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}

		if (o->Type == MODEL_MONSTER01 + 37)
		{
			mu_float Light = EMath::Sin(FWorldTime*0.002f)*0.3f + 0.5f;
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, 0, Light, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, b->_MeshTexture[6]);
			mu_float TexCoord = (mu_float)(IWorldTime % 100)*0.01f;
			b->RenderMesh(ThreadIndex, 3, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, 3, Light, o->BlendMeshTexCoordU, -TexCoord);
		}
		if (o->Type == MODEL_NPC_ARCHANGEL_MESSENGER || o->Type == MODEL_NPC_ARCHANGEL)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight);
		}
		if (o->Type == MODEL_ARROW_DRILL)
		{
			if (MU_UPDATECOUNTER > 0)
			{
				EVector3 Position, p, Light;

				Vector(1.0f, 0.6f, 0.4f, Light);
				Vector(0.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(1)], p, Position);
				CreateSprite(BITMAP_SHINY + 1, Position, (mu_float)(EMath::Sin(FWorldTime*0.002f)*0.3f + 1.3f), Light, o, (mu_float)(GetLargeRand(360)));
				CreateSprite(BITMAP_LIGHT, Position, 1.5f, Light, o, 90.0f);
				CreateSprite(BITMAP_SHINY + 1, Position, (mu_float)(EMath::Sin(FWorldTime*0.002f)*0.3f + 1.3f), Light, o, (mu_float)(GetLargeRand(360)));
				CreateParticle(ThreadIndex, BITMAP_SPARK, Position, o->Angle, Light);
			}
		}
		if (o->Type == MODEL_ARROW_TANKER_HIT || o->Type == MODEL_ARROW_TANKER)
		{
			b->BodyLight(ThreadIndex)[0] = 1.0f;
			b->BodyLight(ThreadIndex)[1] = 1.0f;
			b->BodyLight(ThreadIndex)[2] = 1.0f;

			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, -2, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

			if (MU_UPDATECOUNTER > 0)
			{
				EVector3 p1, p2;

				Vector(1.0f, 1.0f, 1.0f, o->Light);
				Vector(0.0f, 1.5f, 0.0f, p1);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(2)], p1, p2);
				CreateParticle(ThreadIndex, BITMAP_FIRE + 1, o->Position, o->Angle, o->Light, 8, o->Scale - 0.4f, o);
				CreateParticle(ThreadIndex, BITMAP_FIRE + 1, o->Position, o->Angle, o->Light, 8, o->Scale - 0.4f, o);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 38, o->Scale, o);
				Vector(1.0f, 0.4f, 0.0f, o->Light);
				CreateParticle(ThreadIndex, BITMAP_FIRE + 1, o->Position, o->Angle, o->Light, 9, o->Scale - 0.4f, o);

				for (mu_int32 i = 0; i < 10; ++i)
				{
					Vector(1.0f - (i*0.1f), 0.4f - (i*0.04f), 0.0f, o->Light);
					Vector(15.0f*i, 1.5f, 0.0f, p1);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(1)], p1, p2);
					CreateSprite(BITMAP_SPARK + 1, p2, 3.0f + (i*0.9f), o->Light, o);
				}
			}
		}
	}

	if (ThreadInfo.CulledType == CULLED_BOIDS)
	{
		if (GET_WORLD != WD_10HEAVEN &&
			GET_WORLD != WD_51HOME_6TH_CHAR &&
			o->Type != MODEL_MAP_TORNADO)
		{
			ThreadInfo.EnableShadow = false;
		}
	}
	else if (ThreadInfo.CulledType == CULLED_FISHS)
	{
		if (GET_WORLD != WD_10HEAVEN &&
			o->Type != MODEL_FISH01 + 7 &&
			o->Type != MODEL_FISH01 + 8 &&
			!(InHellas() && o->Type == MODEL_MONSTER01 + 33))
		{
			ThreadInfo.EnableShadow = false;
		}
	}
	else if (ThreadInfo.CulledType == CULLED_BUGS)
	{
		if (GET_WORLD != WD_10HEAVEN &&
			GET_WORLD != WD_51HOME_6TH_CHAR &&
			(o->Type == MODEL_UNICON ||
				o->Type == MODEL_PEGASUS))
		{
			ThreadInfo.EnableShadow = false;
		}
	}

	return RENDER_SUCCEED;
}

RENDER_STATUS RenderObject(mu_int32 ThreadIndex, OBJECT *o, mu_boolean Translate, mu_int32 Select, mu_int32 ExtraMon)
{
	if (Calc_RenderObject(ThreadIndex, o, Translate, Select, ExtraMon) == false)
	{
		return RENDER_HIDDEN;
	}

	if (InBloodCastle() == true &&
		o->Type == 5)
	{
		return RENDER_HIDDEN; // FIX // temporary bug fix for model Object06, because somehow it is bugged, wrong positions, angles and size...
	}

	return Draw_RenderObject(ThreadIndex, o, Translate, Select, ExtraMon);
}

void RenderObject_AfterImage(const mu_uint32 ThreadIndex, OBJECT* o, mu_boolean Translate, mu_int32 Select, mu_int32 ExtraMon)
{
	mu_float fAnimationFrame = o->AnimationFrame;
	mu_float fAni1, fAni2;

	fAni1 = fAnimationFrame - 1.4f;
	fAni2 = fAnimationFrame - 0.7f;
	if (fAni1 > 0.0f)
	{
		o->Alpha = 0.2f;
		o->AnimationFrame = fAni1;
		RenderObject(ThreadIndex, o, Translate, Select, 0);
	}
	if (fAni2 > 0.0f)
	{
		o->Alpha = 0.6f;
		o->AnimationFrame = fAni2;
		RenderObject(ThreadIndex, o, Translate, Select, 0);
	}

	o->Alpha = 1.0f;
	o->AnimationFrame = fAnimationFrame;
	RenderObject(ThreadIndex, o, Translate, Select, 0);
}

void PartObjectColor(mu_int32 Type, mu_float Alpha, mu_float Bright, EVector3 &Light, mu_boolean ExtraMon)
{
	mu_int32 Color = 0;

	if (ExtraMon && (Type == MODEL_MONSTER01 + 27 || Type == MODEL_SPEAR + 9))
	{
		Color = 8;
	}
	else if (Type == MODEL_MONSTER01 + 27 || Type == MODEL_SPEAR + 9)
	{
		Color = 1;
	}
	else if (Type == MODEL_MONSTER01 + 35 || Type == MODEL_BOW + 5 || Type == MODEL_BOW + 13)
	{
		Color = 5;
	}
	else if (Type == MODEL_SWORD + 14 || Type == MODEL_STAFF + 5 || (Type >= MODEL_POTION + 25 && Type < MODEL_POTION + 27))
	{
		Color = 2;
	}
	else if (Type == MODEL_BOW + 17 || Type == MODEL_BOW + 19)
	{
		Color = 9;
	}
	else if (Type == MODEL_WING + 14)
	{
		Color = 2;
	}
	else if (Type == MODEL_BOW + 18)
	{
		Color = 10;
	}
	else if (Type == MODEL_STAFF + 9)
	{
		Color = 5;
	}
	else if (Type == MODEL_SWORD + 31)
	{
		Color = 10;
	}
	else if (Type == MODEL_SHIELD + 16)
	{
		Color = 6;
	}
	else if (Type == MODEL_SPEAR + 10)
	{
		Color = 9;
	}
	else if (Type == MODEL_MACE + 8)
	{
		Color = 9;
	}
	else if (Type == MODEL_MACE + 9)
	{
		Color = 10;
	}
	else if (Type == MODEL_MACE + 10)
	{
		Color = 12;
	}
	else if (Type == MODEL_SWORD + 20)
	{
		Color = 10;
	}
	else if (Type == MODEL_SWORD + 21)
	{
		Color = 5;
	}
	else if (Type == MODEL_BOW + 20)
	{
		Color = 16;
	}
	else if (Type == MODEL_STAFF + 11)
	{
		Color = 17;
	}
	else if (Type == MODEL_MACE + 12)
	{
		Color = 16;
	}
	else if (Type == MODEL_SWORD + 22)
	{
		Color = 18;
	}
	else if (Type == MODEL_STAFF + 12)
	{
		Color = 19;
	}
	else if (Type == MODEL_BOW + 21)
	{
		Color = 20;
	}
	else if (Type == MODEL_SWORD + 23)
	{
		Color = 23;
	}
	else if (Type == MODEL_MACE + 14)
	{
		Color = 22;
	}
	else if (Type == MODEL_SWORD + 24)
	{
		Color = 24;
	}
	else if (Type == MODEL_STAFF + 13)
	{
		Color = 25;
	}
	else if (Type == MODEL_BOW + 22)
	{
		Color = 26;
	}
	else if (Type == MODEL_MACE + 15)
	{
		Color = 28;
	}
	else if (Type == MODEL_SWORD + 25)
	{
		Color = 27;
	}
	else if (Type == MODEL_STAFF + 14)
		Color = 24;
	else if (Type == MODEL_STAFF + 15)
		Color = 15;
	else if (Type == MODEL_STAFF + 16)
		Color = 1;
	else if (Type == MODEL_STAFF + 17)
		Color = 3;
	else if (Type == MODEL_STAFF + 18)
		Color = 30;
	else if (Type == MODEL_STAFF + 19)
		Color = 21;
	else if (Type == MODEL_STAFF + 20)
		Color = 5;
	else if (Type == MODEL_STAFF + 22)
		Color = 1;
	else if (Type == MODEL_SWORD + 28)
		Color = 8;
	else if (Type == MODEL_STAFF + 30)
		Color = 1;
	else if (Type == MODEL_STAFF + 31)
		Color = 19;
	else if (Type == MODEL_MACE + 17)
		Color = 40;
	else if (Type == MODEL_SHIELD + 19)
		Color = 29;
	else if (Type == MODEL_BOW + 23)
		Color = 35;
	else if (Type == MODEL_SHIELD + 20)
		Color = 36;
	else if (Type == MODEL_SHIELD + 21)
		Color = 30;
	else if (Type == MODEL_SPEAR + 11)
		Color = 20;
	else if (Type == MODEL_STAFF + 33)
		Color = 43;
	else if (Type == MODEL_STAFF + 34)
		Color = 5;
	else if (Type == MODEL_MACE + 18)
		Color = 5;
	else if (Type == MODEL_BOW + 24)
		Color = 36;
	else
	{
		mu_int32 ItemType = Type - MODEL_ITEM;
		if (ItemType / MAX_ITEM_INDEX >= 7 && ItemType / MAX_ITEM_INDEX <= 11)
		{
			switch (ItemType%MAX_ITEM_INDEX)
			{
			case 1:Color = 1; break;
			case 9:Color = 2; break;
			case 12:Color = 2; break;
			case 3:Color = 3; break;
			case 13:Color = 4; break;
			case 4:Color = 5; break;
			case 14:Color = 5; break;
			case 6:Color = 6; break;
			case 15:Color = 7; break;
			case 16:Color = 10; break;
			case 17:Color = 9; break;
			case 18:Color = 5; break;
			case 19:Color = 9; break;
			case 20:Color = 9; break;
			case 21:Color = 16; break;
			case 22:Color = 17; break;
			case 23:Color = 11; break;
			case 24:Color = 16; break;
			case 25:Color = 11; break;
			case 26:Color = 12; break;
			case 27:Color = 10; break;
			case 28:Color = 15; break;
			case 29:Color = 18; break;
			case 30:Color = 19; break;
			case 31:Color = 20; break;
			case 32:Color = 21; break;
			case 33:Color = 22; break;
			case 34:Color = 24; break;
			case 35:Color = 25; break;
			case 36:Color = 26; break;
			case 37:Color = 27; break;
			case 38:Color = 28; break;
			case 39:Color = 29; break;
			case 40:Color = 30; break;
			case 41:Color = 31; break;
			case 42:Color = 32; break;
			case 43:Color = 33; break;
			case 44:Color = 34; break;
			case 45:Color = 36; break;
			case 46:Color = 42; break;
			case 47:Color = 37; break;
			case 48:Color = 1; break;
			case 49:Color = 35; break;
			case 50:Color = 39; break;
			case 51:Color = 40; break;
			case 52:Color = 36; break;
			case 53:Color = 41; break;
			}
		}
	}
	Bright *= Alpha;
	switch (Color)
	{
	case 0:Vector(Bright*1.0f, Bright*0.5f, Bright*0.0f, Light); break;
	case 1:Vector(Bright*1.0f, Bright*0.2f, Bright*0.0f, Light); break;
	case 2:Vector(Bright*0.0f, Bright*0.5f, Bright*1.0f, Light); break;
	case 3:Vector(Bright*0.0f, Bright*0.5f, Bright*1.0f, Light); break;
	case 4:Vector(Bright*0.0f, Bright*0.8f, Bright*0.4f, Light); break;
	case 5:Vector(Bright*1.0f, Bright*1.0f, Bright*1.0f, Light); break;
	case 6:Vector(Bright*0.6f, Bright*0.8f, Bright*0.4f, Light); break;
	case 7:Vector(Bright*0.9f, Bright*0.8f, Bright*1.0f, Light); break;
	case 8:Vector(Bright*0.8f, Bright*0.8f, Bright*1.0f, Light); break;
	case 9:Vector(Bright*0.5f, Bright*0.5f, Bright*0.8f, Light); break;
	case 10:Vector(Bright*0.75f, Bright*0.65f, Bright*0.5f, Light); break;
	case 11:Vector(Bright*0.35f, Bright*0.35f, Bright*0.6f, Light); break;
	case 12:Vector(Bright*0.47f, Bright*0.67f, Bright*0.6f, Light); break;
	case 13:Vector(Bright*0.0f, Bright*0.3f, Bright*0.6f, Light); break;
	case 14:Vector(Bright*0.65f, Bright*0.65f, Bright*0.55f, Light); break;
	case 15:Vector(Bright*0.2f, Bright*0.3f, Bright*0.6f, Light); break;
	case 16:Vector(Bright*0.8f, Bright*0.46f, Bright*0.25f, Light); break;
	case 17:Vector(Bright*0.65f, Bright*0.45f, Bright*0.3f, Light); break;
	case 18:Vector(Bright*0.5f, Bright*0.4f, Bright*0.3f, Light); break;
	case 19:Vector(Bright*0.37f, Bright*0.37f, Bright*1.0f, Light); break;
	case 20:Vector(Bright*0.3f, Bright*0.7f, Bright*0.3f, Light); break;
	case 21:Vector(Bright*0.5f, Bright*0.4f, Bright*1.0f, Light); break;
	case 22:Vector(Bright*0.45f, Bright*0.45f, Bright*0.23f, Light); break;
	case 23:Vector(Bright*0.3f, Bright*0.3f, Bright*0.45f, Light); break;
	case 24:Vector(Bright*0.6f, Bright*0.5f, Bright*0.2f, Light); break;
	case 25:Vector(Bright*0.6f, Bright*0.6f, Bright*0.6f, Light); break;
	case 26:Vector(Bright*0.3f, Bright*0.7f, Bright*0.3f, Light); break;
	case 27:Vector(Bright*0.5f, Bright*0.6f, Bright*0.7f, Light); break;
	case 28:Vector(Bright*0.45f, Bright*0.45f, Bright*0.23f, Light); break;
	case 29:Vector(Bright*0.2f, Bright*0.7f, Bright*0.3f, Light); break;
	case 30:Vector(Bright*0.7f, Bright*0.3f, Bright*0.3f, Light); break;
	case 31:Vector(Bright*0.7f, Bright*0.5f, Bright*0.3f, Light); break;
	case 32:Vector(Bright*0.5f, Bright*0.2f, Bright*0.7f, Light); break;
	case 33:Vector(Bright*0.8f, Bright*0.4f, Bright*0.6f, Light); break;
	case 34:Vector(Bright*0.6f, Bright*0.4f, Bright*0.8f, Light); break;
	case 35:Vector(Bright*0.7f, Bright*0.4f, Bright*0.4f, Light); break;
	case 36:Vector(Bright*0.5f, Bright*0.5f, Bright*0.7f, Light); break;
	case 37:Vector(Bright*0.7f, Bright*0.5f, Bright*0.7f, Light); break;
	case 38:Vector(Bright*0.2f, Bright*0.4f, Bright*0.7f, Light); break;
	case 39:Vector(Bright*0.3f, Bright*0.6f, Bright*0.4f, Light); break;
	case 40:Vector(Bright*0.7f, Bright*0.2f, Bright*0.2f, Light); break;
	case 41:Vector(Bright*0.7f, Bright*0.2f, Bright*0.7f, Light); break;
	case 42:Vector(Bright*0.8f, Bright*0.4f, Bright*0.0f, Light); break;
	case 43:Vector(Bright*0.8f, Bright*0.6f, Bright*0.2f, Light); break;
	}
}

void PartObjectColor2(mu_int32 Type, mu_float Alpha, mu_float Bright, EVector3 &Light, mu_boolean ExtraMon)
{
	mu_int32 Color = 0;
	if (Type == MODEL_BOW + 5 || Type == MODEL_BOW + 13)
	{
		Color = 2;
	}
	else if (Type == MODEL_SWORD + 14 || Type == MODEL_STAFF + 5)
	{
		Color = 2;
	}
	else if (Type == MODEL_SWORD + 18)
	{
		Color = 0;
	}
	else if (Type == MODEL_BOW + 17)
	{
		Color = 0;
	}
	else if (Type == MODEL_STAFF + 9)
	{
		Color = 0;
	}
	else
	{
		mu_int32 ItemType = Type - MODEL_ITEM;
		if (ItemType / MAX_ITEM_INDEX >= 7 && ItemType / MAX_ITEM_INDEX <= 11)
		{
			switch (ItemType%MAX_ITEM_INDEX)
			{
			case 0:Color = 0; break;
			case 1:Color = 0; break;
			case 2:Color = 0; break;
			case 3:Color = 0; break;
			case 4:Color = 1; break;
			case 5:Color = 0; break;
			case 6:Color = 0; break;
			case 7:Color = 0; break;
			case 8:Color = 0; break;
			case 9:Color = 0; break;
			case 10:Color = 0; break;
			case 11:Color = 0; break;
			case 12:Color = 0; break;
			case 13:Color = 0; break;
			case 14:Color = 1; break;
			case 15:Color = 1; break;
			case 16:Color = 0; break;
			case 17:Color = 1; break;
			case 18:Color = 2; break;
			case 19:Color = 0; break;
			case 21:Color = 3; break;
			case 39:Color = 1; break;
			case 40:Color = 1; break;
			case 41:Color = 1; break;
			case 42:Color = 1; break;
			case 43:Color = 2; break;
			case 44:Color = 3; break;
			case 45:Color = 0; break;
			}
		}
	}
	Bright *= Alpha;
	switch (Color)
	{
	case 0: Vector(Bright*1.0f*Light[0], Bright*1.0f*Light[1], Bright*1.0f*Light[2], Light); break;
	case 1: Vector(Bright*1.0f*Light[0], Bright*0.5f*Light[1], Bright*0.0f*Light[2], Light); break;
	case 2: Vector(Bright*0.0f*Light[0], Bright*0.5f*Light[1], Bright*1.0f*Light[2], Light); break;
	case 3: Vector(1.0f, 1.0f, 1.0f, Light);
	}
}

void PartObjectColor3(mu_int32 Type, mu_float Alpha, mu_float Bright, EVector3 &Light, mu_boolean ExtraMon)
{
	mu_int32 Color = 0;
	mu_int32 ItemType = Type - MODEL_ITEM;

	if (ItemType / MAX_ITEM_INDEX >= 7 && ItemType / MAX_ITEM_INDEX <= 11)
	{
		switch (ItemType%MAX_ITEM_INDEX)
		{
		case 0:Color = 0; break;
		case 1:Color = 0; break;
		case 2:Color = 0; break;
		case 3:Color = 1; break;
		case 4:Color = 0; break;
		case 5:Color = 0; break;
		case 6:Color = 0; break;
		case 7:Color = 0; break;
		case 8:Color = 0; break; 
		case 9:Color = 1; break;
		case 10:Color = 0; break;
		case 11:Color = 0; break;
		case 12:Color = 0; break;
		case 13:Color = 0; break;
		case 14:Color = 0; break;
		case 15:Color = 0; break;
		case 16:Color = 0; break;
		case 17:Color = 1; break;
		case 18:Color = 0; break;
		case 19:Color = 0; break;
		}
	}
	switch (Color)
	{
	case 0: Vector(0.1f, 0.6f, 1.0f, Light); break;
	case 1: Vector(1.0f, 0.7f, 0.2f, Light); break;
	}
}

void RenderPartObjectBody(const mu_uint32 ThreadIndex, MUModel *b, OBJECT *o, mu_int32 Type, mu_float Alpha, mu_int32 RenderType)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_boolean bIsNotRendered = false;

	if ((Type == MODEL_ARMOR + 15 || Type == MODEL_GLOVES + 15 || Type == MODEL_PANTS + 15 || Type == MODEL_BOOTS + 15)
		&& !(RenderType & RENDER_DOPPELGANGER)
		)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME + 1);
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}

	else if (Type == MODEL_ARMOR + 20 || Type == MODEL_GLOVES + 20 || Type == MODEL_PANTS + 20 || Type == MODEL_BOOTS + 20)
	{
		EVector3 Light;
		VectorCopy(b->BodyLight(ThreadIndex), Light);
		Vector(0.85f*Light[0], 0.85f*Light[1], 1.2f*Light[2], b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME + 1);
		VectorCopy(Light, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (Type == MODEL_WING + 6)    
	{
		Vector(0.8f, 0.6f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, 0.5f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME + 1);
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (Type == MODEL_WING + 36)	
	{
		Vector(1.0f, 0.7f, 0.5f, b->BodyLight(ThreadIndex));
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		mu_float fU = 0.0f;
		static mu_int32 s_iTexAni[MAX_TASKING_THREADS] = { 0 };
		s_iTexAni[ThreadIndex] += MU_UPDATECOUNTER;
		if (s_iTexAni[ThreadIndex] > 15)
			s_iTexAni[ThreadIndex] = 0;
		fU = (s_iTexAni[ThreadIndex] / 4) * 0.25f;
		Vector(0.9f, 0.6f, 0.3f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, o->BlendMeshLight, fU, o->BlendMeshTexCoordV, o->HiddenMesh);
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
	}
	else if (Type == MODEL_WING + 39)	
	{
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		mu_float Luminosity = EMath::Abs(EMath::Sin(FWorldTime*0.001f))*0.3f;
		Vector(0.1f + Luminosity, 0.1f + Luminosity, 0.1f + Luminosity, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		Luminosity = EMath::Abs(EMath::Sin(FWorldTime*0.001f))*0.8f;
		Vector(0.0f + Luminosity, 0.0f + Luminosity, 0.0f + Luminosity, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_3RDWING_LAYER);
	}
	else if (Type == MODEL_WING + 40)
	{
		if (b->BodyLight(ThreadIndex)[0] == 1 && b->BodyLight(ThreadIndex)[1] == 1 && b->BodyLight(ThreadIndex)[2] == 1)
		{
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else
		{
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
	}
	else if (Type == MODEL_WING + 42)	
	{
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME6, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_WING + 43)	
	{
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_SWORD + 19)	
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_METAL, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_LIGHTMAP | RENDER_TEXTURE, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, FWorldTime*0.0001f, BITMAP_CHROME);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_STAFF + 10)	
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_METAL, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_LIGHTMAP | RENDER_TEXTURE, 1.0f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, -FWorldTime*0.0001f, BITMAP_CHROME);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_BOW + 18)	
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_METAL, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_LIGHTMAP | RENDER_TEXTURE, 1.0f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, -FWorldTime*0.0001f, BITMAP_CHROME);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_MACE + 13) 
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_LIGHTMAP | RENDER_TEXTURE, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, -FWorldTime*0.0001f, BITMAP_CHROME);
	}
	else if (Type == MODEL_BOW + 19)
	{
		EVector3 Light;
		VectorCopy(b->BodyLight(ThreadIndex), Light);
		Vector(0.8f*Light[0], 0.0f, 0.8f*Light[2], b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 0, RENDER_LIGHTMAP | RENDER_TEXTURE, 1.0f, 0, o->BlendMeshLight, -FWorldTime*0.0002f, o->BlendMeshTexCoordV, BITMAP_CHROME);
		VectorCopy(Light, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, 1.0f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME + 1);
		VectorCopy(Light, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (Type == MODEL_POTION + 45)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_DARK | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, -FWorldTime*0.0002f, o->BlendMeshTexCoordV, BITMAP_CHROME);
	
		if (MU_UPDATECOUNTER > 0)
		{
			EVector3 vPos, vRelativePos;
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(8)], vRelativePos, vPos, true);
			mu_float fLumi = (EMath::Sin(FWorldTime*0.004f) + 1.0f) * 0.05f;
			Vector(0.8f + fLumi, 0.8f + fLumi, 0.3f + fLumi, o->Light);
			CreateSprite(BITMAP_LIGHT, vPos, 1.5f, o->Light, o, 0.5f);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(10)], vRelativePos, vPos, true);
			CreateSprite(BITMAP_LIGHT, vPos, 0.5f, o->Light, o, 0.5f);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(11)], vRelativePos, vPos, true);
			CreateSprite(BITMAP_LIGHT, vPos, 0.5f, o->Light, o, 0.5f);
		}
	}
	else if (o->Type >= MODEL_ETC + 19 && o->Type <= MODEL_ETC + 27)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.5f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_ROOLOFPAPER_EFFECT_R);
	}
	else if (Type == MODEL_SWORD + 31
		&& !(RenderType & RENDER_DOPPELGANGER))
	{
		EVector3 Light;
		VectorCopy(b->BodyLight(ThreadIndex), Light);
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
		b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, EMath::Sin(FWorldTime*0.01f), 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_TEXTURE, 1.0f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, FWorldTime*0.001f, BITMAP_CHROME);

		mu_float Luminosity = EMath::Sin(FWorldTime*0.001f)*0.5f + 0.5f;
		Vector(Light[0] * Luminosity, Light[0] * Luminosity, Light[0] * Luminosity, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, 1.0f, 2, o->BlendMeshLight, FWorldTime*0.0001f, -FWorldTime*0.0005f);
	}
	else if (Type == MODEL_SPEAR + 10)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, 1.0f, 1, o->BlendMeshLight, FWorldTime*0.0001f, FWorldTime*0.0005f);
	}
	else if (Type == MODEL_MACE + 7)
	{
		EVector3 Light;
		VectorCopy(b->BodyLight(ThreadIndex), Light);
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		mu_float time = FWorldTime*0.001f;
		mu_float Luminosity = EMath::Sin(time)*0.5f + 0.3f;
		Vector(Light[0] * Luminosity, Light[0] * Luminosity, Light[0] * Luminosity, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, 1.0f, 2, o->BlendMeshLight, time, -FWorldTime*0.0005f);
	}
	else if (Type == MODEL_HELPER + 4)   
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		Vector(0.8f, 0.4f, 0.1f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_HELPER + 5)  
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		Vector(0.3f, 0.8f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_MACE + 8)		
	{
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.001f)*0.6f + 0.4f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (Type == MODEL_MACE + 9)		
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		Vector(0.1f, 0.3f, 1.0f, b->BodyLight(ThreadIndex));
		o->BlendMesh = 0;
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.001f)*0.6f + 0.4f;
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		Vector(0.6f, 0.8f, 1.0f, b->BodyLight(ThreadIndex));
		o->BlendMesh = 1;
		o->BlendMeshLight = 1.0f;
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, FWorldTime*0.0003f, BITMAP_CHROME);
	}
	else if (Type == MODEL_SWORD + 26)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		o->BlendMesh = 1;
		Vector(1.0f, 0.0f, 0.2f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		mu_float fV;
		fV = (((mu_int32)(FWorldTime*0.05) % 16) / 4)*0.25f;
		b->RenderMesh(ThreadIndex, 5, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 5, o->BlendMeshLight, o->BlendMeshTexCoordU, fV);
	}
	else if (Type == MODEL_SWORD + 27)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_SWORD + 28)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		o->BlendMesh = 1;
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.001f)*0.6f + 0.4f;
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

	}
	else if (Type == MODEL_MACE + 16)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_STAFF + 30)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		o->BlendMesh = 1;
		o->BlendMeshLight = 1.0f;
		Vector(1.0f, 0.5f, 0.5f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_STAFF + 31)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		o->BlendMesh = 0;
		o->BlendMeshLight = EMath::Abs(EMath::Sin(FWorldTime*0.001f)) + 0.1f;
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_SOCKETSTAFF);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_SOCKETSTAFF);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_SOCKETSTAFF);
	}
	else if (Type == MODEL_STAFF + 32)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_SHIELD + 17)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_SHIELD + 18)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_SHIELD + 19)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		o->BlendMesh = 2;
		o->BlendMeshLight = EMath::Abs((EMath::Sin(FWorldTime*0.001f)));
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_SHIELD + 20)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_SHIELD + 21)
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->StreamMesh(ThreadIndex) = 1;
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 2000 * 0.0005f);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type == MODEL_HELPER + 49)
	{
		mu_float sine = mu_float(EMath::Sin(FWorldTime*0.002f)*0.3f) + 0.7f;
		b->RenderBody(ThreadIndex, RenderType, 0.7f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1);
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, 1.0f, 4, sine, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 0);
	}
	else if (o->Type == MODEL_HELPER + 50)
	{
		mu_float sine = mu_float(EMath::Sin(FWorldTime*0.00004f)*0.15f) + 0.5f;
		b->RenderBody(ThreadIndex, RenderType, 1.0f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 0);
		b->RenderBody(ThreadIndex, RenderType, 0.5f, o->BlendMesh, sine, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1);
	}
	else if (o->Type == MODEL_HELPER + 51)
	{
		mu_float sine = mu_float(EMath::Sin(FWorldTime*0.002f)*0.3f) + 0.7f;
		b->RenderBody(ThreadIndex, RenderType, 0.7f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 0);
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, 1.0f, 0, sine, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1);
	}
	else if (Type == MODEL_POTION + 64)
	{
		mu_float PlaySpeed = b->_AnimationHeader[b->CurrentAction(ThreadIndex)].Speed;

		PlaySpeed *= MU_FRAMEELAPSED;

		b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, 2.0 / 7.0);
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight*1.5f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, Alpha, o->BlendMesh, o->BlendMeshLight / 4.0f, FWorldTime*0.005f, -FWorldTime*0.005f, 0);

		EVector3 Light;
		EVector3 vRelativePos, vWorldPos;
		Vector(0.0f, 0.0f, 0.0f, vRelativePos);
		Vector(1.0f, 0.0f, 0.0f, Light);

		b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(1)], vRelativePos, vWorldPos, true);
		CreateSprite(BITMAP_LIGHT, vWorldPos, 3.0f, Light, o, 0.0f);
	}
	else if (o->Type == MODEL_HELPER + 52)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, 0.9, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type == MODEL_HELPER + 53)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, 0.9, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type == MODEL_POTION + 65)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.5f) * 0.4f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight,
			o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi,
			o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_ITEM_EFFECT_DBSTONE_R);
	}
	else if (o->Type == MODEL_POTION + 66)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.5f) * 0.4f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight,
			o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi,
			o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_ITEM_EFFECT_HELLHORN_R);
	}
	else if (o->Type == MODEL_POTION + 67)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.5f) * 0.4f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight,
			o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi,
			o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_ITEM_EFFECT_PFEATHER_R);
	}
	else if (o->Type == MODEL_POTION + 68)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight,
			o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.5f) * 0.4f;
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, Alpha, 1, fLumi,
			o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_ITEM_EFFECT_DEYE_R);
		b->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, 0.2f, -1,
			o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type >= MODEL_HELPER + 46 && Type <= MODEL_HELPER + 48)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.2f) * 0.3f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_FREETICKET_R);
	}
	else if (Type == MODEL_POTION + 54)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.2f) * 0.4f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, 1.0f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHAOSCARD_R);
	}
	else if (Type == MODEL_POTION + 58)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.5f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM1_R);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_POTION + 59)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.5f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM2_R);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_POTION + 60)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.5f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM3_R);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_POTION + 61)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.5f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM4_R);
	}
	else if (Type == MODEL_POTION + 62)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.5f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM5_R);
	}
	else if (o->Type == MODEL_POTION + 53)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.5f) * 0.5f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_LUCKY_CHARM_EFFECT53);
	}
	else if (o->Type == MODEL_HELPER + 43
		|| o->Type == MODEL_HELPER + 93
		)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.001f) + 1.5f) * 0.25f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_LUCKY_SEAL_EFFECT43);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type == MODEL_HELPER + 44
		|| o->Type == MODEL_HELPER + 94
		|| o->Type == MODEL_HELPER + 116
		)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.001f) + 1.5f) * 0.25f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_LUCKY_SEAL_EFFECT44);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type == MODEL_HELPER + 45)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.001f) + 1.5f) * 0.25f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_LUCKY_SEAL_EFFECT45);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type >= MODEL_POTION + 70 && o->Type <= MODEL_POTION + 71)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_CHROME4, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type >= MODEL_POTION + 72 && o->Type <= MODEL_POTION + 77)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (o->Type == MODEL_HELPER + 59)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (o->Type >= MODEL_HELPER + 54 && o->Type <= MODEL_HELPER + 58)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (o->Type >= MODEL_POTION + 78 && o->Type <= MODEL_POTION + 82)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (o->Type == MODEL_HELPER + 60)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (o->Type == MODEL_HELPER + 61)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.2f) * 0.3f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_FREETICKET_R);
	}
	else if (o->Type == MODEL_POTION + 83)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.5f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM5_R);
	}
	else if (o->Type == MODEL_POTION + 145)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.5f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM7);
	}
	else if (o->Type == MODEL_POTION + 146)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.5f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM8);
	}
	else if (o->Type == MODEL_POTION + 147)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.5f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM9);
	}
	else if (o->Type == MODEL_POTION + 148)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.5f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM10);
	}
	else if (o->Type == MODEL_POTION + 149)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.5f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM11);
	}
	else if (o->Type == MODEL_POTION + 150)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.5f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAREITEM12);
	}
	else if (Type == MODEL_HELPER + 125)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.2f) * 0.3f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_DOPPLEGANGGER_FREETICKET);
	}
	else if (Type == MODEL_HELPER + 126)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.2f) * 0.3f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_BARCA_FREETICKET);
	}
	else if (Type == MODEL_HELPER + 127)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.2f) * 0.3f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_BARCA7TH_FREETICKET);
	}
	else if (Type == MODEL_POTION + 91) 
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.2f) * 0.4f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, 1.0f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHARACTERCARD_R);
	}
	else if (Type == MODEL_POTION + 92) 
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.2f) * 0.4f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, 1.0f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_NEWCHAOSCARD_GOLD_R);
	}
	else if (Type == MODEL_POTION + 93) 
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.2f) * 0.4f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, 1.0f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_NEWCHAOSCARD_RARE_R);
	}
	else if (Type == MODEL_POTION + 95) 
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.2f) * 0.4f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, 1.0f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_NEWCHAOSCARD_MINI_R);
	}
	else if (Type == MODEL_POTION + 94)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_POTION + 84)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_POTION + 85)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_POTION + 86)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_POTION + 87)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_POTION + 88)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_POTION + 89)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_POTION + 90)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type >= MODEL_HELPER + 62 && Type <= MODEL_HELPER + 63)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (Type >= MODEL_POTION + 97 && Type <= MODEL_POTION + 98)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (Type == MODEL_POTION + 96)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (Type == MODEL_HELPER + 64) 
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (Type == MODEL_HELPER + 65) 
	{
#if 0
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
#else
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
#endif
	}
	else if (Type == MODEL_MACE + 10
		&& !(RenderType & RENDER_DOPPELGANGER)
		)
	{
		mu_float Luminosity = FWorldTime*0.0005f;

		o->HiddenMesh = 2;
		o->BlendMesh = 2;
		o->BlendMeshLight = 1.0f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, Luminosity, Luminosity);
	}
	else if (Type == MODEL_MACE + 11)
	{
		o->BlendMeshLight = 1.0f;
		o->BlendMeshTexCoordU = FWorldTime*0.0008f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.001f)*0.3f + 0.7f;
		b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME, Alpha, 0, o->BlendMeshLight, 0.0f, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_SWORD + 20)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		mu_float Luminosity = EMath::Sin(FWorldTime*0.0008f)*0.7f + 0.5f;
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, Alpha, 2, Luminosity, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		ThreadInfo.RenderLight = MU::Float4ToARGB(0.43f, 0.14f, 0.6f, 1.0f);

		b->RenderMesh(ThreadIndex, 3, RENDER_BRIGHT | RENDER_CHROME, Alpha, 3, o->BlendMeshLight, FWorldTime*0.0001f, FWorldTime*0.0005f);

		ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (Type == MODEL_SWORD + 21)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, 1.0f, 0, o->BlendMeshLight, FWorldTime*0.0005f, FWorldTime*0.0005f);
		o->HiddenMesh = 0;
		b->StreamMesh(ThreadIndex) = 1;
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, FWorldTime*0.0005f);
	}
	else if (Type == MODEL_ARMOR + 23 || Type == MODEL_GLOVES + 23 || Type == MODEL_PANTS + 23 || Type == MODEL_BOOTS + 23)
	{
		EVector3 Light;
		VectorCopy(b->BodyLight(ThreadIndex), Light);
		Vector(Light[0] * 0.3f, Light[1] * 0.8f, Light[1] * 1.0f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME + 1);
		VectorCopy(Light, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (Type == MODEL_BOW + 21 || Type == MODEL_STAFF + 12 || Type == MODEL_MACE + 14 || Type == MODEL_SWORD + 22)
	{
		b->StreamMesh(ThreadIndex) = 0;
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		if (Type == MODEL_SWORD + 22)
		{
			b->BodyLight(ThreadIndex)[0] = 1.0f;
			b->BodyLight(ThreadIndex)[1] = 0.7f;
			b->BodyLight(ThreadIndex)[2] = 0.4f;
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, IWorldTime % 4000 * 0.0004f - 0.7f, o->BlendMeshTexCoordV, BITMAP_CHROME7);
			b->BodyLight(ThreadIndex)[0] = 0.7f;
			b->BodyLight(ThreadIndex)[1] = 0.7f;
			b->BodyLight(ThreadIndex)[2] = 0.7f;
		}
		else if (Type == MODEL_STAFF + 12)
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, IWorldTime % 2000 * 0.0002f - 0.3f, IWorldTime % 2000 * 0.0002f - 0.3f, BITMAP_CHROME_ENERGY);
		else
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, IWorldTime % 4000 * 0.0002f - 0.3f, IWorldTime % 4000 * 0.0002f - 0.3f, BITMAP_CHROME6);
		b->StreamMesh(ThreadIndex) = -1;
	}
	else if (Type == MODEL_SWORD + 23)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordU);
		mu_float Luminosity = EMath::Sin(FWorldTime*0.0015f)*0.03f + 0.3f;
		Vector(Luminosity, Luminosity, Luminosity + 0.1f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, -2, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordU);

		o->Alpha = 0.5f;
		Vector(0.4f, 0.4f, 0.8f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, -2, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordU);
	}
	else if (Type == MODEL_ARMOR + 31)
	{
		if (b->HideSkin(ThreadIndex) == true)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
	}
	else if (Type == MODEL_PANTS + 31)
	{
		if (b->HideSkin(ThreadIndex) == true)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
	}
	else if (Type == MODEL_SWORD + 25)
	{
		o->HiddenMesh = 1;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, 0.5f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_LAVA);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, 0.7f, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, FWorldTime*0.0009f);
	}
	else if (Type == MODEL_MACE + 15)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_BOW + 22)
	{
		o->HiddenMesh = 1;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, 1.0f, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME5, 0.5f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_STAFF + 13)
	{
		o->HiddenMesh = 1;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, 1.0f, 1, o->BlendMeshLight, FWorldTime*0.0009f, FWorldTime*0.0009f);
	}
	else if (Type == MODEL_ARMOR + 36)
	{
		if (b->HideSkin(ThreadIndex) == true)
		{
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
	}
	else if (Type == MODEL_PANTS + 36)
	{
		if (b->HideSkin(ThreadIndex) == true)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
	}
	else if (Type == MODEL_GLOVES + 36)
	{
		if (b->HideSkin(ThreadIndex) == true)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
	}
	else if (MODEL_HELM + 39 <= Type && MODEL_HELM + 44 >= Type
		&& !(RenderType & RENDER_DOPPELGANGER)
		)
	{
		if (b->HideSkin(ThreadIndex))
		{
			ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
			mu_int32 anMesh[6] = { 2, 1, 0, 2, 1, 2 };
			b->RenderMesh(ThreadIndex, anMesh[Type - (MODEL_HELM + 39)], RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}

	else if (MODEL_ARMOR + 39 <= Type && MODEL_ARMOR + 44 >= Type
		&& !(RenderType & RENDER_DOPPELGANGER)
		)
	{
		if (b->HideSkin(ThreadIndex))
		{
			ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
			mu_int32 nTexture = Type - (MODEL_ARMOR + 39);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_INVEN_ARMOR + nTexture);
			for (mu_int32 i = 1; i < b->_MeshCount; ++i)
				b->RenderMesh(ThreadIndex, i, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (MODEL_PANTS + 39 <= Type && MODEL_PANTS + 44 >= Type
		&& !(RenderType & RENDER_DOPPELGANGER)
		)
	{
		if (b->HideSkin(ThreadIndex))
		{
			ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
			mu_int32 nTexture = Type - (MODEL_PANTS + 39);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_INVEN_PANTS + nTexture);
			for (mu_int32 i = 1; i < b->_MeshCount; ++i)
				b->RenderMesh(ThreadIndex, i, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (MODEL_GLOVES + 44 == Type)	
	{
		if (b->HideSkin(ThreadIndex))
		{
			ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else
	{
		bIsNotRendered = true;
	}

	if (bIsNotRendered == false);
	else if (Type == MODEL_ARMOR + 50 || Type == MODEL_PANTS + 50 || Type == MODEL_ARMOR + 53 || Type == MODEL_PANTS + 53)
	{
		mu_int32 nTexture = 0;

		switch (Type)
		{
		case MODEL_ARMOR + 50:	nTexture = BITMAP_SKIN_ARMOR_DEVINE; break;
		case MODEL_PANTS + 50:	nTexture = BITMAP_SKIN_PANTS_DEVINE; break;
		case MODEL_ARMOR + 53:	nTexture = BITMAP_SKIN_ARMOR_SUCCUBUS; break;
		case MODEL_PANTS + 53:	nTexture = BITMAP_SKIN_PANTS_SUCCUBUS; break;
		}
		if (b->HideSkin(ThreadIndex))
		{
			ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, nTexture);
			for (mu_int32 i = 1; i < b->_MeshCount; ++i)
			{
				b->RenderMesh(ThreadIndex, i, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}
		}
		else
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
	}
	else if (Type == MODEL_HELM + 49)
	{
		if (b->HideSkin(ThreadIndex) == true)
		{
			ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
	}
	else if (Type == MODEL_ARMOR + 49)
	{
		if (b->HideSkin(ThreadIndex) == true)
		{
			ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
	}
	else if (Type == MODEL_HELM + 50)
	{
		if (b->HideSkin(ThreadIndex) == true)
		{
			ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
	}
	else if (Type == MODEL_HELM + 53)
	{
		if (b->HideSkin(ThreadIndex) == true)
		{
			ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
	}
	else if (Type == MODEL_SPEAR + 11)		
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 5, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, 0.4f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, 0.8f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_MACE + 18)	
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 5, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 6, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, 0.5f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_BOW + 20)
	{
		mu_float Luminosity = EMath::Sin(FWorldTime*0.002f)*0.3f + 0.5f;
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 6, RENDER_TEXTURE, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, 1.0f, 3, Luminosity, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE, 1.0f, 4, Luminosity, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);
		b->RenderMesh(ThreadIndex, 5, RENDER_TEXTURE, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_POTION + 28)
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);
		MODELS::Data[o->Type].StreamMesh(ThreadIndex) = 1;
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, FWorldTime*0.0005f);
		MODELS::Data[o->Type].StreamMesh(ThreadIndex) = -1;
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_POTION + 29)
	{
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		b->StreamMesh(ThreadIndex) = 1;
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, FWorldTime*0.0005f);
		b->StreamMesh(ThreadIndex) = -1;

		Vector(1.0f, 0.5f, 0.0f, b->BodyLight(ThreadIndex));

		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, 1.0f, 2, g_Hellas01MeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, 0.3f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_STAFF + 11)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, Alpha, 1, 0.2f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, Alpha, 0, EMath::Sin(FWorldTime*0.005f), o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_CHROME4 | RENDER_BRIGHT, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_STAFF + 18)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, Alpha, 1, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_STAFF + 19)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 0.5f) * 0.5f;
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, Alpha, 1, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_MACE + 12)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, Alpha, 1, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, Alpha, 3, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_SHIELD + 15
		&& !(RenderType & RENDER_DOPPELGANGER)
		)
	{
		EVector3 Light;
		VectorCopy(b->BodyLight(ThreadIndex), Light);
		Vector(Light[0] * 0.3f, Light[1] * 0.3f, Light[2] * 0.3f, b->BodyLight(ThreadIndex));
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
		b->RenderMesh(ThreadIndex, 2, RENDER_COLOR, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		VectorCopy(Light, b->BodyLight(ThreadIndex));
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
		b->RenderMesh(ThreadIndex, 2, RENDER_CHROME | RENDER_BRIGHT, 1.0f, 2, o->BlendMeshLight, o->BlendMeshTexCoordU, FWorldTime*0.01f, BITMAP_CHROME);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, 1.0f, 1, o->BlendMeshLight, (mu_float)(GetLargeRand(10))*0.1f, (mu_float)(GetLargeRand(10))*0.1f);

		mu_float Luminosity = EMath::Sin(FWorldTime*0.001f)*0.4f + 0.6f;
		Vector(Light[0] * Luminosity, Light[0] * Luminosity, Light[0] * Luminosity, b->BodyLight(ThreadIndex));
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, 1.0f, 2, o->BlendMeshLight, FWorldTime*0.0001f, FWorldTime*0.0005f);
	}
	else if (Type == MODEL_SHIELD + 16)
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 0.8f);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, 0.8f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 0.5f);
		b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, 0.5f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, 1.0f, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, 1.0f, 2, o->BlendMeshLight, FWorldTime*0.0005f, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, 1.0f, 3, o->BlendMeshLight, (mu_float)(GetLargeRand(10))*0.1f, (mu_float)(GetLargeRand(10))*0.1f);
	}
	else if (Type == MODEL_BOW + 3 && (RenderType&RENDER_EXTRA))
	{
		RenderType -= RENDER_EXTRA;
		Vector(0.1f, 0.1f, 0.1f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_ANGEL && b->_MeshCount)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		if (RenderType&RENDER_CHROME)
		{
			Vector(0.75f, 0.55f, 0.5f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_CHROME4 | RENDER_BRIGHT, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_TEXTURE | RENDER_BRIGHT, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
	}
	else if (Type == MODEL_POTION + 7)
	{
		if (o->HiddenMesh == 1)
		{
			ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, 1.0f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(0.1f, 0.5f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_METAL | RENDER_BRIGHT, 1.0f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, 1.0f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else if (o->HiddenMesh == 0)
		{
			ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, 1.0f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(0.1f, 0.5f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 1, RENDER_METAL | RENDER_BRIGHT, 1.0f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, 1.0f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
	}
	else if (Type == MODEL_HELPER + 7)   
	{
		if (o->HiddenMesh == 1)
		{
			ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, 1.0f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else if (o->HiddenMesh == 0)
		{
			ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, 1.0f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
	}
	else if (Type == MODEL_HELPER + 11)   
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, 1.0f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		Vector(0.0f, 0.5f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, 1.0f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_BOW + 7 || Type == MODEL_BOW + 15)
	{
		if (g_isCharacterBuff(o, eBuff_InfinityArrow))
		{
			Vector(1.0f, 0.8f, 0.2f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderBody(ThreadIndex, RENDER_CHROME | RENDER_BRIGHT, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else
		{
			b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
	}
	else if (o->Flags.Get(OBJECT::eOF_PCROOM) == true
		&& (Type == MODEL_HELM + 9 || Type == MODEL_ARMOR + 9 || Type == MODEL_PANTS + 9
			|| Type == MODEL_GLOVES + 9 || Type == MODEL_BOOTS + 9)
		)
	{
		if (Type == MODEL_ARMOR + 9 &&
			MU_UPDATECOUNTER > 0)
		{
			EVector3 EndRelative, EndPos;
			Vector(0.0f, 0.0f, 0.0f, EndRelative);

			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], EndRelative, EndPos, true);

			Vector(0.4f, 0.6f, 0.8f, o->Light);
			CreateSprite(BITMAP_LIGHT, EndPos, 6.0f, o->Light, o, 0.5f);

			mu_float Luminosity;
			Luminosity = EMath::Sin(FWorldTime*0.05f)*0.4f + 0.9f;
			Vector(Luminosity*0.3f, Luminosity*0.5f, Luminosity*0.8f, o->Light);
			CreateSprite(BITMAP_LIGHT, EndPos, 2.0f, o->Light, o);
		}

		EVector3 Light;
		VectorCopy(b->BodyLight(ThreadIndex), Light);
		Vector(0.9f, 0.7f, 1.0f, b->BodyLight(ThreadIndex));

		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_FENRIR_THUNDER);
		b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_METAL, o->Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_FENRIR_THUNDER);
		VectorCopy(Light, b->BodyLight(ThreadIndex));
	}
	else if (Type == MODEL_POTION + 42 || Type == MODEL_POTION + 43 || Type == MODEL_POTION + 44)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type >= MODEL_WING + 60 && Type <= MODEL_WING + 65)
	{
		mu_int32 iCategoryIndex = Type - (MODEL_WING + 60) + 1;
		switch (iCategoryIndex)
		{
		case 1:
			Vector(0.9f, 0.1f, 0.2f, b->BodyLight(ThreadIndex));
			break;
		case 2:
			Vector(0.4f, 0.5f, 1.0f, b->BodyLight(ThreadIndex));
			break;
		case 3:
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			break;
		case 4:
			Vector(0.4f, 1.0f, 0.6f, b->BodyLight(ThreadIndex));
			break;
		case 5:
			Vector(1.0f, 0.8f, 0.4f, b->BodyLight(ThreadIndex));
			break;
		case 6:
			Vector(1.0f, 0.4f, 1.0f, b->BodyLight(ThreadIndex));
			break;
		}
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (Type >= MODEL_WING + 100 && Type <= MODEL_WING + 129)
	{
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		mu_int32 iCategoryIndex = (Type - (MODEL_WING + 100)) % 6 + 1;
		switch (iCategoryIndex)
		{
		case 1:
			Vector(0.9f, 0.1f, 0.2f, b->BodyLight(ThreadIndex));
			break;
		case 2:
			Vector(0.4f, 0.5f, 1.0f, b->BodyLight(ThreadIndex));
			break;
		case 3:
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			break;
		case 4:
			Vector(0.4f, 1.0f, 0.6f, b->BodyLight(ThreadIndex));
			break;
		case 5:
			Vector(1.0f, 0.8f, 0.4f, b->BodyLight(ThreadIndex));
			break;
		case 6:
			Vector(1.0f, 0.4f, 1.0f, b->BodyLight(ThreadIndex));
			break;
		}
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1);
		b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1);
	}
	else if (Type == MODEL_HELPER + 71 || Type == MODEL_HELPER + 72 || Type == MODEL_HELPER + 73 || Type == MODEL_HELPER + 74 || Type == MODEL_HELPER + 75)
	{
		mu_int32 _angle = mu_int32(b->BodyAngle(ThreadIndex)[1]) % 360;
		mu_float _meshLight1;
		if (0 < _angle && _angle <= 180)
		{
			_meshLight1 = 0.2f - (EMath::Sin(Q_PI*(_angle) / 180.0f) * 0.2f);
		}
		else
		{
			_meshLight1 = 0.2f;
		}
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, 0.35f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, _meshLight1, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, 2, _meshLight1, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Kind == KIND_PLAYER && o->Type == MODEL_PLAYER && o->SubType == MODEL_SKELETON_PCBANG)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		EVector3 Light;
		VectorCopy(b->BodyLight(ThreadIndex), Light);
		Vector(0.9f, 0.8f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_FENRIR_THUNDER);
		b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_METAL, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_FENRIR_THUNDER);
		VectorCopy(Light, b->BodyLight(ThreadIndex));
	}
	else if (o->Kind == KIND_PLAYER && o->Type == MODEL_PLAYER && o->SubType == MODEL_CURSEDTEMPLE_ALLIED_PLAYER)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1);

		if (g_isCharacterBuff(o, eDeBuff_CursedTempleRestraint) || g_isCharacterBuff(o, eBuff_CursedTempleProdection))
		{
			EVector3 vRelativePos, vtaWorldPos, Light;
			Vector(0.4f, 0.4f, 0.8f, Light);
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);

			if (g_isCharacterBuff(o, eDeBuff_CursedTempleRestraint))
			{
				mu_float Luminosity = EMath::Sin(FWorldTime*0.002f)*0.3f + 0.5f;
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, 0.4f + Luminosity, 0, o->BlendMeshLight*Luminosity, -FWorldTime*0.0005f, FWorldTime*0.0005f);
			}

			if(MU_UPDATECOUNTER > 0)
			{
				static const mu_int32 boneindex[15] = { 1, 2, 17, 18, 19, 20, 44, 25, 27, 34, 37, 3, 5, 10, 12 };

				for (mu_int32 j = 0; j < 15; ++j)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(boneindex[j])], vRelativePos, vtaWorldPos, true);
					CreateSprite(BITMAP_LIGHT, vtaWorldPos, 1.3f, Light, o);
				}
			}
		}

		if (o->m_pCloth == nullptr)
		{
			mu_int32 iTex = BITMAP_CURSEDTEMPLE_ALLIED_PHYSICSCLOTH;
			CPhysicsCloth *pCloth = new CPhysicsCloth[1];
			pCloth[0].Create(ThreadIndex, o, 19, 0.0f, 8.0f, 0.0f, 10, 10, 140.0f, 140.0f, iTex, iTex, PCT_CURVED | PCT_SHORT_SHOULDER | PCT_MASK_ALPHA);
			o->m_pCloth = pCloth;
			o->m_byNumCloth = 1;
		}
		CPhysicsCloth *pCloth = o->m_pCloth;
		if (pCloth)
		{
			mu_float Flag = 0.005f;

			if (g_isCharacterBuff(o, eDeBuff_Stun) || g_isCharacterBuff(o, eDeBuff_Sleep))
			{
				Flag = 0.0f;
			}

			for (mu_int32 k = 0; k < o->m_byNumCloth; ++k)
			{
				if (MU_UPDATECOUNTER > 0 && pCloth[k].Move2(ThreadIndex, Flag, 5) == false)
				{
					DeleteCloth(nullptr, o);
					break;
				}
				else
				{
					pCloth[k].Render(ThreadIndex, b);
				}
			}
		}
	}
	else if (o->Kind == KIND_PLAYER && o->Type == MODEL_PLAYER && o->SubType == MODEL_CURSEDTEMPLE_ILLUSION_PLAYER)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 2);

		if (g_isCharacterBuff(o, eDeBuff_CursedTempleRestraint) || g_isCharacterBuff(o, eBuff_CursedTempleProdection))
		{
			EVector3 vRelativePos, vtaWorldPos, Light;
			Vector(0.4f, 0.4f, 0.8f, Light);
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);

			if (g_isCharacterBuff(o, eDeBuff_CursedTempleRestraint))
			{
				mu_float Luminosity = EMath::Sin(FWorldTime*0.002f)*0.3f + 0.5f;
				b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, 0.4f + Luminosity, 0, o->BlendMeshLight*Luminosity, -FWorldTime*0.0005f, FWorldTime*0.0005f);
			}

			if(MU_UPDATECOUNTER > 0)
			{
				static const mu_int32 boneindex[15] = { 1, 2, 17, 18, 19, 20, 44, 25, 27, 34, 37, 3, 5, 10, 12 };

				for (mu_int32 j = 0; j < 15; ++j)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(boneindex[j])], vRelativePos, vtaWorldPos, true);
					CreateSprite(BITMAP_LIGHT, vtaWorldPos, 1.3f, Light, o);
				}
			}
		}

		if (o->m_pCloth == nullptr)
		{
			mu_int32 iTex = BITMAP_CURSEDTEMPLE_ILLUSION_PHYSICSCLOTH;
			CPhysicsCloth* pCloth = new CPhysicsCloth[1];
			pCloth[0].Create(ThreadIndex, o, 20, -4.0f, 5.0f, 0.0f, 10, 20, 17.0f, 100.0f, iTex, iTex, PCT_SHAPE_HALLOWEEN | PCT_ELASTIC_HALLOWEEN | PCT_MASK_ALPHA);
			o->m_pCloth = pCloth;
			o->m_byNumCloth = 1;
		}
		CPhysicsCloth *pCloth = o->m_pCloth;
		if (pCloth)
		{
			mu_float Flag = 0.005f;

			if (g_isCharacterBuff(o, eDeBuff_Stun) || g_isCharacterBuff(o, eDeBuff_Sleep))
			{
				Flag = 0.0f;
			}

			for (mu_int32 k = 0; k < o->m_byNumCloth; ++k)
			{
				if (MU_UPDATECOUNTER > 0 && pCloth[k].Move2(ThreadIndex, Flag, 5) == false)
				{
					DeleteCloth(nullptr, o);
					break;
				}
				else
				{
					pCloth[k].Render(ThreadIndex, b);
				}
			}
		}
	}
	else if (o->Kind == KIND_PLAYER && o->Type == MODEL_PLAYER && o->SubType == MODEL_HALLOWEEN)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		EVector3 vLight;
		Vector(1.0f, 1.0f, 1.0f, vLight);
		EVector3 vPos, vRelativePos;
		Vector(6.0f, 6.0f, 0.0f, vRelativePos);
		VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));
		b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(20)], vRelativePos, vPos, true);
		vPos[2] += 36.0f;
		if(MU_UPDATECOUNTER > 0)
		{
			if (GetLargeRand(2) == 0)
			{
				CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, vPos, o->Angle, vLight, 8, 0.5f, o);
			}
			CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, vPos, o->Angle, vLight, 9, 0.5f, o);
		}

		vPos[2] -= 5.0f;
		mu_float fLumi;
		fLumi = (mu_float)(GetLargeRand(5) + 10) * 0.1f;
		Vector(fLumi + 0.5f, fLumi*0.3f + 0.1f, 0.0f, vLight);
		if(MU_UPDATECOUNTER > 0)
		{
			CreateSprite(BITMAP_LIGHT, vPos, 1.0f + fLumi*0.1f, vLight, o);

			if (GetLargeRand(8) == 0)
			{
				vPos[2] -= 10.0f;
				fLumi = (mu_float)(GetLargeRand(100)) * 0.01f;
				vLight[0] = fLumi;
				fLumi = (mu_float)(GetLargeRand(100)) * 0.01f;
				vLight[1] = fLumi;
				fLumi = (mu_float)(GetLargeRand(100)) * 0.01f;
				vLight[2] = fLumi;
				CreateParticle(ThreadIndex, BITMAP_SHINY, vPos, o->Angle, vLight, 4, 0.8f);
			}
		}

		if (o->CurrentAction == PLAYER_JACK_1)
		{
			mu_int32 iAnimationFrame = (mu_int32)o->AnimationFrame;

			if ((iAnimationFrame == 2 ||
				iAnimationFrame == 9) &&
				MU_UPDATECOUNTER > 0)
			{
				Vector(1.0f, 1.0f, 1.0f, vLight);
				vPos[2] -= 20.0f;
				CreateParticle(ThreadIndex, BITMAP_SPARK + 2, vPos, o->Angle, vLight, 0, 0.6f);
				CreateParticle(ThreadIndex, BITMAP_SPARK + 2, vPos, o->Angle, vLight, 0, 0.6f);
				for (mu_int32 i = 0; i<20; ++i)
				{
					fLumi = (mu_float)(GetLargeRand(100)) * 0.01f;
					vLight[0] = fLumi;
					fLumi = (mu_float)(GetLargeRand(100)) * 0.01f;
					vLight[1] = fLumi;
					fLumi = (mu_float)(GetLargeRand(100)) * 0.01f;
					vLight[2] = fLumi;
					CreateParticle(ThreadIndex, BITMAP_SHINY, vPos, o->Angle, vLight, 4, 0.8f);
				}
				++o->m_iAnimation;
			}

			if (o->m_iAnimation >= 40 &&
				iAnimationFrame == 9 &&
				MU_UPDATECOUNTER > 0)
			{
				Vector(1.0f, 1.0f, 1.0f, vLight);
				CreateEffect(ThreadIndex, MODEL_HALLOWEEN_EX, vPos, o->Angle, vLight, 0);
				vPos[2] -= 40.0f;
				CreateParticle(ThreadIndex, BITMAP_SPARK + 2, vPos, o->Angle, vLight, 1);
				SetAction(o, PLAYER_SHOCK);
#if 0 // FIX
				SendRequestAction(PLAYER_SHOCK, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
				o->m_iAnimation = 0;
			}

			mu_float fStageG, fStageB;
			fStageG = 0.8f - ((o->m_iAnimation / 40.0f) * 0.8f);
			if (fStageG < 0.2f)
			{
				fStageG = 0.2f;
			}
			fStageB = 0.8f - ((o->m_iAnimation / 40.0f) * 0.9f);
			if (fStageB < 0.1f)
			{
				fStageB = 0.1f;
			}
			VectorCopy(b->BodyLight(ThreadIndex), vLight);
			Vector(1.0f, fStageG, fStageB, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			VectorCopy(vLight, b->BodyLight(ThreadIndex));
		}
		else if (o->CurrentAction == PLAYER_JACK_2)
		{
			mu_int32 iAnimationFrame = (mu_int32)o->AnimationFrame;

			if ((iAnimationFrame == 10 ||
				iAnimationFrame == 19) &&
				MU_UPDATECOUNTER > 0)
			{
				Vector(1.0f, 1.0f, 1.0f, vLight);
				vPos[2] -= 20.0f;
				CreateParticle(ThreadIndex, BITMAP_SPARK + 2, vPos, o->Angle, vLight, 0, 0.6f);
				CreateParticle(ThreadIndex, BITMAP_SPARK + 2, vPos, o->Angle, vLight, 0, 0.6f);
				for (mu_int32 i = 0; i<20; ++i)
				{
					fLumi = (mu_float)(GetLargeRand(100)) * 0.01f;
					vLight[0] = fLumi;
					fLumi = (mu_float)(GetLargeRand(100)) * 0.01f;
					vLight[1] = fLumi;
					fLumi = (mu_float)(GetLargeRand(100)) * 0.01f;
					vLight[2] = fLumi;
					CreateParticle(ThreadIndex, BITMAP_SHINY, vPos, o->Angle, vLight, 4, 0.8f);
				}
				++o->m_iAnimation;
			}

			if (o->m_iAnimation >= 40 &&
				iAnimationFrame == 19 &&
				MU_UPDATECOUNTER > 0)
			{
				Vector(1.0f, 1.0f, 1.0f, vLight);
				CreateEffect(ThreadIndex, MODEL_HALLOWEEN_EX, vPos, o->Angle, vLight, 0);
				vPos[2] -= 40.0f;
				CreateParticle(ThreadIndex, BITMAP_SPARK + 2, vPos, o->Angle, vLight, 1);
				SetAction(o, PLAYER_SHOCK);
#if 0 // FIX
				SendRequestAction(PLAYER_SHOCK, ((mu_uint8)((o->Angle[2] + 22.5f) / 360.0f*8.0f + 1.0f) % 8));
#endif
				o->m_iAnimation = 0;
			}

			mu_float fStageG, fStageB;
			fStageG = 0.8f - ((o->m_iAnimation / 40.0f) * 0.8f);
			if (fStageG < 0.2f)
			{
				fStageG = 0.2f;
			}
			fStageB = 0.8f - ((o->m_iAnimation / 40.0f) * 0.9f);
			if (fStageB < 0.1f)
			{
				fStageB = 0.1f;
			}
			VectorCopy(b->BodyLight(ThreadIndex), vLight);
			Vector(1.0f, fStageG, fStageB, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			VectorCopy(vLight, b->BodyLight(ThreadIndex));
		}

		if (o->m_pCloth == nullptr)
		{
			mu_int32 iTex = BITMAP_ROBE + 3;
			CPhysicsCloth* pCloth = new CPhysicsCloth[1];
			pCloth[0].Create(ThreadIndex, o, 19, 0.0f, 10.0f, 0.0f, 10, 20, 30.0f, 200.0f, iTex, iTex, PCT_SHAPE_HALLOWEEN | PCT_ELASTIC_HALLOWEEN | PCT_MASK_ALPHA);
			o->m_pCloth = pCloth;
			o->m_byNumCloth = 1;
		}
		CPhysicsCloth *pCloth = o->m_pCloth;
		if (pCloth)
		{
			if (MU_UPDATECOUNTER > 0 && pCloth[0].Move2(ThreadIndex, 0.005f, 5) == false)
			{
				DeleteCloth(nullptr, o);
			}
			else
			{
				pCloth[0].Render(ThreadIndex, b);
			}
		}
	}
	else if (o->Kind == KIND_PLAYER && o->Type == MODEL_PLAYER && o->SubType == MODEL_PANDA)
	{
		EVector3 Light;
		EVector3 vPos;

		Vector(1.0f, 0.6f, 0.2f, Light);
		if (GetLargeRand(100) == 0)
		{
			CreateEffect(ThreadIndex, MODEL_ARROWSRE06, vPos, o->Angle, Light, 2, o, 0, 0, 0, 0, 1.0f);
			CreateEffect(ThreadIndex, MODEL_ARROWSRE06, vPos, o->Angle, Light, 2, o, 1, 0, 0, 0, 1.0f);
		}

		mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_PANDABODY_R);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (o->Kind == KIND_PLAYER && o->Type == MODEL_PLAYER && o->SubType == MODEL_XMAS_EVENT_CHANGE_GIRL)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		mu_int32 iAnimationFrame = (mu_int32)o->AnimationFrame;

		if (o->CurrentAction == PLAYER_SANTA_1)
		{
			if(MU_UPDATECOUNTER > 0)
			{
				if (iAnimationFrame == 4 || iAnimationFrame == 8 || iAnimationFrame == 12)
				{
					mu_int32 iEffectType = GetLargeRand(4) + MODEL_XMAS_EVENT_BOX;
					EVector3 vPos;
					EVector3 vLight;
					Vector(1.0f, 1.0f, 1.0f, vLight);
					VectorCopy(o->Position, vPos);
					vPos[2] += 230.0f;
					for (mu_int32 i = 0; i < 2; ++i)
					{
						CreateEffect(ThreadIndex, iEffectType, vPos, o->Angle, o->Light);
						CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, o->Light, 20, 1.0f);
					}

					for (mu_int32 i = 0; i < 5; ++i)
					{
						CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, vLight, 20, 1.0f);
					}

					if (iAnimationFrame >= 19)
					{
						++o->m_iAnimation;
					}
				}
			}
		}
		else if (o->CurrentAction == PLAYER_SANTA_2)
		{
			if(MU_UPDATECOUNTER > 0)
			{
				EVector3 vPos, vLight;
				VectorCopy(o->Position, vPos);
				vPos[2] += 230;
				Vector(1.0f, 1.0f, 1.0f, vLight);
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, vLight, 20, 1.0f);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 4, 2.0f);
				if (GetLargeRand(6) == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_SNOW_EFFECT_1, vPos, o->Angle, vLight, 0, 0.3f);
				}
				if (GetLargeRand(3) == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_SNOW_EFFECT_2, vPos, o->Angle, vLight, 0, 0.5f);
				}

				if (iAnimationFrame >= 14)
				{
					++o->m_iAnimation;
					DeleteEffect(MODEL_XMAS_EVENT_ICEHEART, nullptr);
					DeleteParticle(BITMAP_DS_EFFECT);
					DeleteParticle(BITMAP_LIGHT);
				}
			}
		}
	}
	else if (o->SubType == MODEL_XMAS_EVENT_CHA_DEER)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, 1, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_BLOOD + 1);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, 1, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_BLOOD + 1);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, 1, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_BLOOD + 1);
	}
	else if (o->Kind == KIND_PLAYER && o->Type == MODEL_PLAYER && o->SubType == MODEL_GM_CHARACTER)
	{
		o->HiddenMesh = 2;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_HELPER + 69)	
	{
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		mu_float Luminosity = (EMath::Sin(FWorldTime*0.003f) + 1)*0.3f + 0.3f;
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, Luminosity, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (Type == MODEL_HELPER + 70)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (Type == MODEL_HELPER + 81)	
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_HELPER + 82)	
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (Type == MODEL_HELPER + 66)	
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME4, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		mu_float Luminosity = (EMath::Sin(FWorldTime*0.003f) + 1)*0.3f + 0.6f;
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, Luminosity, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_POTION + 100)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type >= MODEL_TYPE_CHARM_MIXWING + EWS_BEGIN
		&& o->Type <= MODEL_TYPE_CHARM_MIXWING + EWS_END)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (Type == MODEL_HELPER + 107)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_HELPER + 104)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, 0.2f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_HELPER + 105)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, 0.2f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type >= MODEL_HELPER + 109 && o->Type <= MODEL_HELPER + 112)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type >= MODEL_HELPER + 113 && o->Type <= MODEL_HELPER + 115)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type >= MODEL_POTION + 112 && o->Type <= MODEL_POTION + 113)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type == MODEL_POTION + 120)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type == MODEL_POTION + 121 || o->Type == MODEL_POTION + 122)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type == MODEL_POTION + 123 || o->Type == MODEL_POTION + 124)
	{
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (o->Type == MODEL_WING + 130)
	{
		if (b->BodyLight(ThreadIndex)[0] == 1 && b->BodyLight(ThreadIndex)[1] == 1 && b->BodyLight(ThreadIndex)[2] == 1)
		{
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
	}
	else if (o->Type == MODEL_POTION + 134)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_PACKAGEBOX_RED);
	}
	else if (o->Type == MODEL_POTION + 135)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_PACKAGEBOX_BLUE);
	}
	else if (o->Type == MODEL_POTION + 136)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_PACKAGEBOX_GOLD);
	}
	else if (o->Type == MODEL_POTION + 137)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_PACKAGEBOX_GREEN);
	}
	else if (o->Type == MODEL_POTION + 138)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_PACKAGEBOX_PUPLE);
	}
	else if (o->Type == MODEL_POTION + 139)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_PACKAGEBOX_SKY);
	}
	else if (o->Type >= MODEL_POTION + 114 && o->Type <= MODEL_POTION + 119)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_INGAMESHOP_PRIMIUM6);
	}
	else if (o->Type >= MODEL_POTION + 126 && o->Type <= MODEL_POTION + 129)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_INGAMESHOP_COMMUTERTICKET4);
	}
	else if (o->Type >= MODEL_POTION + 130 && o->Type <= MODEL_POTION + 132)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_INGAMESHOP_SIZECOMMUTERTICKET3);
	}
	else if (o->Type == MODEL_HELPER + 121)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.2f) * 0.3f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_FREETICKET_R);
	}
	else if (Type >= MODEL_POTION + 141 && Type <= MODEL_POTION + 144)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_HELPER + 122)	
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, 0.5f, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
	}
	else if (o->Type == MODEL_15GRADE_ARMOR_OBJ_ARMLEFT ||
		o->Type == MODEL_15GRADE_ARMOR_OBJ_ARMRIGHT ||
		o->Type == MODEL_15GRADE_ARMOR_OBJ_BODYLEFT ||
		o->Type == MODEL_15GRADE_ARMOR_OBJ_BODYRIGHT ||
		o->Type == MODEL_15GRADE_ARMOR_OBJ_BOOTLEFT ||
		o->Type == MODEL_15GRADE_ARMOR_OBJ_BOOTRIGHT ||
		o->Type == MODEL_15GRADE_ARMOR_OBJ_HEAD ||
		o->Type == MODEL_15GRADE_ARMOR_OBJ_PANTLEFT ||
		o->Type == MODEL_15GRADE_ARMOR_OBJ_PANTRIGHT)
	{
		mu_float fLight, texCoordU;

		fLight = 0.8f - EMath::Abs(EMath::Sin(FWorldTime*0.0018f) * 0.5f);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLight - 0.1f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		texCoordU = EMath::Abs(EMath::Sin(FWorldTime*0.0005f));
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLight - 0.3f, texCoordU, o->BlendMeshTexCoordV, BITMAP_RGB_MIX);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_CHROME4, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_HELPER + 128 || Type == MODEL_HELPER + 129)
	{
		mu_float fEyeBlinkingTime = EMath::Sin(FWorldTime*0.005f) + 1;

		b->RenderBody(ThreadIndex, RENDER_TEXTURE, 1.0f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, 0.3f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, fEyeBlinkingTime, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type >= MODEL_HELPER + 130 && Type <= MODEL_HELPER + 133)
	{
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		ThreadInfo.RenderLight = MU::Float4ToARGB(b->BodyLight(ThreadIndex), 1.0f);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		mu_float Luminosity = EMath::Abs(EMath::Sin(FWorldTime*0.001f))*0.3f;
		Vector(0.1f + Luminosity, 0.1f + Luminosity, 0.1f + Luminosity, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		Luminosity = EMath::Abs(EMath::Sin(FWorldTime*0.001f))*0.8f;
		Vector(0.0f + Luminosity, 0.0f + Luminosity, 0.0f + Luminosity, b->BodyLight(ThreadIndex));

		switch (Type)
		{
		case MODEL_HELPER + 130:	
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_ORK_CHAM_LAYER_R);
			break;
		case MODEL_HELPER + 131:	
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_MAPLE_CHAM_LAYER_R);
			break;
		case MODEL_HELPER + 132:	
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_GOLDEN_ORK_CHAM_LAYER_R);
			break;
		case MODEL_HELPER + 133:	
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_GOLDEN_MAPLE_CHAM_LAYER_R);
			break;
		default:
			break;
		}
	}
	else if (Type == MODEL_SPEAR + 9)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		Vector(0.5f, 0.5f, 1.5f, b->BodyLight(ThreadIndex));
		++ThreadInfo.AlphaTestIndex;
		b->StreamMesh(ThreadIndex) = 0;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
		b->StreamMesh(ThreadIndex) = -1;
		--ThreadInfo.AlphaTestIndex;
	}
	else
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
}

void RenderPartObjectBodyColor(const mu_uint32 ThreadIndex, MUModel *b, OBJECT *o, mu_int32 Type, mu_float Alpha, mu_int32 RenderType, mu_float Bright, mu_int32 Texture, mu_int32 iMonsterIndex)
{
	if ((RenderType&RENDER_LIGHTMAP) == RENDER_LIGHTMAP)
	{
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
	}
	else if (Type == MODEL_MONSTER01 + 54)
	{
		if (RenderType&RENDER_EXTRA)
		{
			RenderType -= RENDER_EXTRA;
			Vector(1.0f, 0.1f, 0.1f, b->BodyLight(ThreadIndex));
		}
		else
		{
			Vector(0.2f, 0.2f, 0.8f, b->BodyLight(ThreadIndex));
		}
	}
	else if (iMonsterIndex >= 493 && iMonsterIndex <= 502)
	{
		if (iMonsterIndex == 495)
		{
			Vector(1.0f, 0.8f, 0.0f, b->BodyLight(ThreadIndex));
		}
		else if (iMonsterIndex == 496)
		{
			Vector(1.0f, 0.6f, 0.0f, b->BodyLight(ThreadIndex));
		}
		else if (iMonsterIndex == 499)
		{
			Vector(1.0f, 0.5f, 0.0f, b->BodyLight(ThreadIndex));
		}
		else if (iMonsterIndex == 501)
		{
			Vector(1.0f, 0.5f, 0.0f, b->BodyLight(ThreadIndex));
		}
		else
		{
			Vector(1.0f, 0.6f, 0.1f, b->BodyLight(ThreadIndex));
		}
	}
	else
	{
		PartObjectColor(Type, Alpha, Bright, b->BodyLight(ThreadIndex), (RenderType&RENDER_EXTRA) ? true : false);
	}

	if (Type == MODEL_MACE + 7)
	{
		o->HiddenMesh = 2;
	}

	if (Type == MODEL_STAFF + 5)
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1, Texture);
	else if (Type == MODEL_SWORD + 20)
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1, Texture);
	else if (Type == MODEL_SWORD + 21)
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 0, Texture);
	else if (Type == MODEL_SWORD + 31 || Type == MODEL_SPEAR + 10 || Type == MODEL_MACE + 7 || Type == MODEL_SHIELD + 16)
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, Texture);
	else if (Type >= MODEL_MACE + 8 && Type <= MODEL_MACE + 13)
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, Texture);
	else if (Type == MODEL_SWORD + 26)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 5);
	}
	else if (Type == MODEL_SWORD + 27)
	{
		b->RenderMesh(ThreadIndex, 0, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_SWORD + 28)
	{
		b->RenderMesh(ThreadIndex, 2, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_STAFF + 30)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1, Texture);

		o->BlendMesh = 1;
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 1, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_STAFF + 32)
	{
		b->RenderMesh(ThreadIndex, 1, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_SHIELD + 19)
	{
		b->RenderMesh(ThreadIndex, 0, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_SHIELD + 20)
	{
		b->RenderMesh(ThreadIndex, 0, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_HELM + 49)
	{
		b->RenderMesh(ThreadIndex, 0, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_HELM + 50)
	{
		b->RenderMesh(ThreadIndex, 1, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_HELM + 53)
	{
		b->RenderMesh(ThreadIndex, 2, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_SHIELD + 21)
	{
		b->RenderMesh(ThreadIndex, 0, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_SPEAR + 11)
	{
		b->RenderMesh(ThreadIndex, 0, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_STAFF + 33)
	{
		b->RenderMesh(ThreadIndex, 0, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_STAFF + 34)
	{
		b->RenderMesh(ThreadIndex, 0, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_MACE + 18)
	{
		b->RenderMesh(ThreadIndex, 0, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_BOW + 24)
	{
		b->RenderMesh(ThreadIndex, 0, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1, Texture);
	}

	if (Type == MODEL_MACE + 7)
	{
		o->HiddenMesh = -1;
	}
}

void RenderPartObjectBodyColor2(const mu_uint32 ThreadIndex, MUModel *b, OBJECT *o, mu_int32 Type, mu_float Alpha, mu_int32 RenderType, mu_float Bright, mu_int32 Texture)
{
	if ((RenderType&RENDER_LIGHTMAP) == RENDER_LIGHTMAP)
	{
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
	}
	else if ((RenderType&RENDER_CHROME3) == RENDER_CHROME3)
	{
		PartObjectColor3(Type, Alpha, Bright, b->BodyLight(ThreadIndex), (RenderType&RENDER_EXTRA) ? true : false);
	}
	else
	{
		PartObjectColor2(Type, Alpha, Bright, b->BodyLight(ThreadIndex), (RenderType&RENDER_EXTRA) ? true : false);
	}

	if (Type == MODEL_STAFF + 5)
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1, Texture);
	else if (Type == MODEL_SWORD + 20)
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1, Texture);
	else if (Type == MODEL_SWORD + 21)
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 0, Texture);
	else if (Type == MODEL_SHIELD + 15 || Type == MODEL_SHIELD + 16)
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1, Texture);
	else if (Type == MODEL_HELPER + 50)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 1, Texture);
	}
	else if (Type == MODEL_POTION + 64)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 0, Texture);
	}
	else if (Type == MODEL_SWORD + 26)
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 5, Texture);
	}
	else if (Type == MODEL_SHIELD + 19)
	{
		b->RenderMesh(ThreadIndex, 0, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_HELM + 49)
	{
		b->RenderMesh(ThreadIndex, 0, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_HELM + 50)
	{
		b->RenderMesh(ThreadIndex, 1, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else if (Type == MODEL_HELM + 53)
	{
		b->RenderMesh(ThreadIndex, 2, RenderType, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
	}
	else
	{
		b->RenderBody(ThreadIndex, RenderType, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1, Texture);
	}
}

void RenderPartObjectEffect(const mu_uint32 ThreadIndex, OBJECT *o, mu_int32 Type, EVector3 Light, mu_float Alpha, mu_int32 ItemLevel, mu_int32 ExcOption, mu_int32 SetOption, mu_int32 Select, mu_int32 RenderType)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_int32 Level = ItemLevel;
	if (RenderType & RENDER_WAVE)
	{
		Level = 0;
	}
	MUModel *b = &MODELS::Data[Type];

	switch (Type)
	{
	case MODEL_HELPER + 3:Level = 8; break;
	case MODEL_HELPER + 39:Level = 13; break;
	case MODEL_HELPER + 40:Level = 13; break;
	case MODEL_HELPER + 41:Level = 13; break;
	case MODEL_POTION + 51:Level = 13; break;
	case MODEL_HELPER + 42:Level = 13; break;
	case MODEL_HELPER + 10:Level = 8; break;
	case MODEL_HELPER + 30:
		Level = 0; break;
	case MODEL_EVENT + 16: Level = 0; break;
	case MODEL_POTION + 13:
	case MODEL_POTION + 14:
	case MODEL_POTION + 16:
	case MODEL_POTION + 31:
	case MODEL_COMPILED_CELE:
	case MODEL_COMPILED_SOUL:
	case MODEL_WING + 15:Level = 8; break;
	case MODEL_WING + 36:
	case MODEL_WING + 37:
	case MODEL_WING + 38:
	case MODEL_WING + 39:
	case MODEL_WING + 40:
	case MODEL_WING + 41:
	case MODEL_WING + 42:
	case MODEL_WING + 43:
	case MODEL_WING + 3:
	case MODEL_WING + 4:
	case MODEL_WING + 5:
	case MODEL_WING + 6:
	case MODEL_WING:
	case MODEL_WING + 1:
	case MODEL_WING + 2:Level = 0; break;
	case MODEL_WING + 7:Level = 9; break;
	case MODEL_WING + 11:Level = 0; break;
	case MODEL_WING + 12:
	case MODEL_WING + 13:
	case MODEL_WING + 16:
	case MODEL_WING + 17:
	case MODEL_WING + 18:
	case MODEL_WING + 19:
	case MODEL_WING + 44:
	case MODEL_WING + 45:
	case MODEL_WING + 46:
	case MODEL_WING + 47:
		{
			Level = 9;
			break;
		}
	case MODEL_WING + 14:
		Level = 9;
		break;
	case MODEL_POTION + 12:
		Level = 8;
		break;
	case MODEL_POTION + 17:
	case MODEL_POTION + 18:
	case MODEL_POTION + 19:
		{
			if (Level <= 6)
			{
				Level *= 2;
			}
			else
			{
				Level = 13;
			}
		}
		break;
	case MODEL_POTION + 20:Level = 9; break;
	case MODEL_POTION + 22:Level = 8; break;
	case MODEL_POTION + 25:Level = 8; break;
	case MODEL_POTION + 26:Level = 8; break;
	case MODEL_POTION + 41:
		Level = 0; break;
	case MODEL_POTION + 42:
		Level = 0; break;
	case MODEL_POTION + 43:
		Level = 0; break;
	case MODEL_POTION + 44:
		Level = 0; break;
	case MODEL_HELPER + 50:
	case MODEL_POTION + 64:
		Level = 0; break;
	case MODEL_HELPER + 52:
	case MODEL_HELPER + 53:
		Level = 0; break;
	case MODEL_EVENT + 4:Level = 0; break;
	case MODEL_EVENT + 6:
		if (Level == 13)
		{
			Level = 13;
		}
		else
		{
			Level = 9;
		}
		break;
	case MODEL_EVENT + 7:Level = 0; break;
	case MODEL_EVENT + 8:Level = 0; break;
	case MODEL_EVENT + 9:Level = 8; break;
	case MODEL_EVENT + 5:
		{
			Level = 9;
		}
		break;
	case MODEL_EVENT + 10:
		Level = (Level - 8) * 2 + 1;
		break;
	case MODEL_POTION + 11:
		if (Level >= 1 && Level <= 5) Level = 7;
		else Level = 9;
		break;
	case MODEL_EVENT + 11:
		--Level;
		break;
	case MODEL_EVENT + 12:
		Level = 0;
		break;
	case MODEL_EVENT + 13:
		Level = 0;
		break;
	case MODEL_EVENT + 14:
		Level += 7;
		break;
	case MODEL_EVENT + 15:
		Level = 8;
		break;
	case MODEL_EVENT + 17:
		Level = 0;
		break;
	case MODEL_EVENT:
	case MODEL_EVENT + 1:Level = 8; break;
	case MODEL_BOW + 7: Level >= 1 ? Level = Level * 2 + 1 : Level = 0; break;
	case MODEL_BOW + 15:Level >= 1 ? Level = Level * 2 + 1 : Level = 0; break;
	case MODEL_HELPER + 134:
		Level = 13;
		break;
	}

	if (g_pOption->GetRenderLevel() < 4)
	{
		Level = EMath::Min(Level, g_pOption->GetRenderLevel() * 2 + 5);
	}

	if (o->Type == MODEL_POTION + 27)
	{
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		b->StreamMesh(ThreadIndex) = 0;
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		if (Level == 1)
		{
		}
		else if (Level == 2)
		{
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(0.75f, 0.65f, 0.5f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
		}
		else if (Level == 3)
		{
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(0.75f, 0.65f, 0.5f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
			b->RenderMesh(ThreadIndex, 2, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
		}
		b->StreamMesh(ThreadIndex) = -1;
		return;
	}
	else if (o->Type == MODEL_POTION + 63)
	{
		b->StreamMesh(ThreadIndex) = 0;
		o->BlendMeshLight = 1.0f;
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		Vector(1.0f, 0.0f, 0.0f, b->BodyLight(ThreadIndex));
		b->LightEnable(ThreadIndex) = true;
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->StreamMesh(ThreadIndex) = -1;
		return;
	}
	else if (o->Type == MODEL_POTION + 52)
	{
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->LightEnable(ThreadIndex) = true;
		Vector(0.1f, 0.6f, 0.4f, b->BodyLight(ThreadIndex));
		o->Alpha = 0.5f;
		b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		return;
	}
	else if (o->Type == MODEL_EVENT + 11 && Level == 1)
	{
		Vector(0.3f, 0.8f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		Vector(1.0f, 0.8f, 0.3f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		return;
	}
	else if (o->Type == MODEL_EVENT + 14 && Level == 9)
	{
		Vector(0.3f, 0.8f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		Vector(1.0f, 0.8f, 0.3f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		return;
	}
	else if ((o->Type >= MODEL_WING + 21 && o->Type <= MODEL_WING + 24)
		|| o->Type == MODEL_WING + 35
		|| (o->Type == MODEL_WING + 48)
		)
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);
		o->BlendMeshLight = 1.0f;
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, Alpha, -1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.001f)*0.5f + 0.5f;
		b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_TEXTURE, Alpha, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 2, RENDER_BRIGHT | RENDER_TEXTURE, Alpha, 2, 1 - o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		return;
	}
	else if (o->Type == MODEL_HELPER + 31)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		switch (Level)
		{
		case 0:
			b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_TEXTURE, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			break;

		case 1:
			Vector(0.3f, 0.8f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_TEXTURE, Alpha, 0, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			break;
		}
		return;
	}
	else if (o->Type >= MODEL_POTION && o->Type <= MODEL_POTION + 6)
	{
		if (Level > 0)
			Level = 7;
	}
	else if ((o->Type >= MODEL_WING + 60 && o->Type <= MODEL_WING + 65)
		|| (o->Type >= MODEL_WING + 70 && o->Type <= MODEL_WING + 74)
		|| (o->Type >= MODEL_WING + 100 && o->Type <= MODEL_WING + 129))
	{
		Level = 0;
	}
	else if (o->Type == MODEL_HELPER + 15)
	{
		switch (Level)
		{
		case 0: Vector(0.0f, 0.5f, 1.0f, b->BodyLight(ThreadIndex)); break;
		case 1: Vector(1.0f, 0.2f, 0.0f, b->BodyLight(ThreadIndex)); break;
		case 2: Vector(1.0f, 0.8f, 0.0f, b->BodyLight(ThreadIndex)); break;
		case 3: Vector(0.6f, 0.8f, 0.4f, b->BodyLight(ThreadIndex)); break;
		}
		b->RenderBody(ThreadIndex, RENDER_METAL, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME + 1);
		b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME + 1);
		return;
	}
	else if (o->Type == MODEL_EVENT + 11)
	{
		Vector(0.9f, 0.9f, 0.9f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderBody(ThreadIndex, RENDER_CHROME | RENDER_BRIGHT, 0.5f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, -1, BITMAP_CHROME + 1);
		return;
	}
	else if (Type == MODEL_EVENT + 5 && ItemLevel == 14)
	{
		Vector(0.2f, 0.3f, 0.5f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		Vector(0.1f, 0.3f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderBody(ThreadIndex, RENDER_METAL | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		return;
	}
	else if (Type == MODEL_EVENT + 5 && ItemLevel == 15)
	{
		Vector(0.5f, 0.3f, 0.2f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		Vector(1.0f, 0.3f, 0.1f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderBody(ThreadIndex, RENDER_METAL | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		return;
	}
	else if (o->Type == MODEL_HELPER + 17)
	{
		o->BlendMeshTexCoordU = EMath::Sin(GET_WORLD*0.0001f);
		o->BlendMeshTexCoordV = -FWorldTime*0.0005f;
		
		Vector(.9f, .9f, .9f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		++ThreadInfo.AlphaTestIndex;
		Vector(.9f, .1f, .1f, b->BodyLight(ThreadIndex)); 
		MODELS::Data[o->Type].StreamMesh(ThreadIndex) = 0;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh, BITMAP_CHROME);
		MODELS::Data[o->Type].StreamMesh(ThreadIndex) = -1;
		--ThreadInfo.AlphaTestIndex;

		return;
	}
	else if (o->Type == MODEL_HELPER + 18)
	{
		Vector(0.8f, 0.8f, 0.8f, b->BodyLight(ThreadIndex));
		mu_float sine = mu_float(EMath::Sin(FWorldTime*0.002f)*0.3f) + 0.7f;

		b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, 1.0f, 0, sine, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		return;
	}
	else if (o->Type == MODEL_EVENT + 12)
	{
		if (MU_UPDATECOUNTER > 0)
		{
			mu_float Luminosity = (mu_float)EMath::Sin((FWorldTime)*0.002f)*0.35f + 0.65f;
			EVector3 p, Position, EffLight;
			Vector(0.0f, 0.0f, 15.0f, p);

			mu_float Scale = Luminosity*0.8f + 2.0f;
			Vector(Luminosity*0.32f, Luminosity*0.32f, Luminosity*2.0f, EffLight);

			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(0)], p, Position);
			VectorAdd(Position, o->Position, Position);

			CreateSprite(BITMAP_SPARK + 1, Position, Scale, EffLight, o);
		}
	}
	else if (o->Type == MODEL_EVENT + 6 && Level == 13)
	{
		Vector(0.4f, 0.6f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_COLOR, 1.0f, 0, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderBody(ThreadIndex, RENDER_CHROME | RENDER_BRIGHT, 1.0f, 0, 1.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		return;
	}
	else if (o->Type == MODEL_EVENT + 13)
	{
		mu_float Luminosity = (mu_float)EMath::Sin((FWorldTime)*0.002f)*0.35f + 0.65f;
		EVector3 p, Position, EffLight;
		Vector(0.0f, -5.0f, -15.0f, p);

		mu_float Scale = Luminosity*0.8f + 2.5f;
		Vector(Luminosity*2.0f, Luminosity*0.32f, Luminosity*0.32f, EffLight);

		b->StreamMesh(ThreadIndex) = 0;
		o->BlendMeshTexCoordV = (mu_int32)-FWorldTime % 4000 * 0.00025f;

		if (MU_UPDATECOUNTER > 0)
		{
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(0)], p, Position);
			VectorAdd(Position, o->Position, Position);

			CreateSprite(BITMAP_SPARK + 1, Position, Scale, EffLight, o);
		}
	}
	else if (o->Type == MODEL_POTION + 17)
	{
		mu_float   sine = (mu_float)EMath::Sin(FWorldTime*0.002f)*10.0f + 15.65f;

		o->BlendMesh = 1;
		o->BlendMeshLight = sine;
		o->BlendMeshTexCoordV = IWorldTime % 2000 * 0.0005f;
		o->Alpha = 2.0f;

		mu_float Luminosity = sine;
		Vector(Luminosity / 5.0f, Luminosity / 5.0f, Luminosity / 5.0f, o->Light);
	}
	else if (o->Type == MODEL_POTION + 18)
	{
		if (MU_UPDATECOUNTER > 0)
		{
			mu_float Luminosity = (mu_float)EMath::Sin((FWorldTime)*0.002f)*0.35f + 0.65f;
			EVector3 p, Position, EffLight;
			Vector(0.0f, 0.0f, 0.0f, p);

			mu_float Scale = Luminosity*0.8f;
			Vector(Luminosity * 2, Luminosity*0.32f, Luminosity*0.32f, EffLight);

			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(1)], p, Position);
			VectorAdd(Position, o->Position, Position);
			CreateSprite(BITMAP_SPARK + 1, Position, Scale, EffLight, o);

			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(2)], p, Position);
			VectorAdd(Position, o->Position, Position);
			CreateSprite(BITMAP_SPARK + 1, Position, Scale, EffLight, o);
		}
	}
	else if (o->Type == MODEL_POTION + 19)
	{
		if (MU_UPDATECOUNTER > 0)
		{
			mu_float Luminosity = (mu_float)EMath::Sin((FWorldTime)*0.002f)*0.35f + 0.65f;
			EVector3 p, Position, EffLight;
			Vector(0.0f, 0.0f, 0.0f, p);

			mu_float Scale = Luminosity*0.8f;
			Vector(Luminosity * 2, Luminosity*0.32f, Luminosity*0.32f, EffLight);

			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(9)], p, Position);
			VectorAdd(Position, o->Position, Position);
			CreateSprite(BITMAP_SPARK + 1, Position, Scale, EffLight, o);

			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(10)], p, Position);
			VectorAdd(Position, o->Position, Position);
			CreateSprite(BITMAP_SPARK + 1, Position, Scale, EffLight, o);
		}
	}
	else if (o->Type == MODEL_POTION + 21)
	{
		if (MU_UPDATECOUNTER > 0)
		{
			mu_float Luminosity = (mu_float)EMath::Sin((FWorldTime)*0.002f)*0.25f + 0.75f;
			EVector3 EffLight;

			Vector(Luminosity*1.0f, Luminosity*0.5f, Luminosity*0.0f, EffLight);
			CreateSprite(BITMAP_SPARK + 1, o->Position, 2.5f, EffLight, o);
		}
	}
	else if (o->Type == MODEL_WING + 5)
	{
		o->BlendMeshLight = (mu_float)(EMath::Sin(FWorldTime*0.001f) + 1.0f) / 4.0f;
	}
	else if (o->Type == MODEL_WING + 4)
	{
		o->BlendMeshLight = (mu_float)EMath::Sin(FWorldTime*0.001f) + 1.1f;
	}
	else if (Type == MODEL_PANTS + 24 || Type == MODEL_HELM + 24)
	{
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.001f)*0.4f + 0.6f;
	}
	else if (o->Type == MODEL_STAFF + 11)
	{
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.004f)*0.3f + 0.7f;
	}
	else if (Type == MODEL_HELM + 19 || Type == MODEL_GLOVES + 19 || Type == MODEL_BOOTS + 19)
	{
		o->BlendMeshLight = 1.0f;
	}
	else if (Type == MODEL_POTION + 7)
	{
		switch (Level)
		{
		case 0: o->HiddenMesh = 1; break;
		case 1: o->HiddenMesh = 0; break;
		}
	}
	else if (Type == MODEL_HELPER + 7)
	{
		switch (Level)
		{
		case 0: o->HiddenMesh = 1; break;
		case 1: o->HiddenMesh = 0; break;
		}
	}
	else if (Type == MODEL_HELPER + 11)
	{
		o->HiddenMesh = 1;
	}
	else if (Type == MODEL_EVENT + 18)
	{
		o->BlendMesh = 1;
	}
	else if (o->Type == MODEL_WING + 6)
	{
		if (MU_UPDATECOUNTER > 0)
		{
			EVector3  posCenter, p, Position, Light;
			mu_float   Scale = EMath::Sin(FWorldTime*0.004f)*0.3f + 0.3f;

			Scale = (Scale*10.0f) + 20.0f;

			Vector(0.6f, 0.3f, 0.8f, Light);

			Vector(0.0f, 0.0f, 0.0f, p);

			for (mu_int32 i = 0; i < 5; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(22 - i)], p, posCenter, true);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(30 - i)], p, Position, true);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, posCenter, o->Angle, 14, o, Scale);
				CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, posCenter, Position, o->Angle, 4, o, Scale + 5);
				CreateSprite(BITMAP_FLARE_BLUE, posCenter, Scale / 28.0f, Light, o);
			}

			for (mu_int32 i = 0; i < 5; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(7 - i)], p, posCenter, true);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(11 + i)], p, Position, true);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, posCenter, o->Angle, 14, o, Scale);
				CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, posCenter, Position, o->Angle, 4, o, Scale + 5);
				CreateSprite(BITMAP_FLARE_BLUE, posCenter, Scale / 28.0f, Light, o);
			}
		}
	}
	else if (Type == MODEL_WING + 36)
	{
		if (MU_UPDATECOUNTER > 0)
		{
			EVector3 vRelativePos, vPos, vLight;
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);
			Vector(0.0f, 0.0f, 0.0f, vPos);
			Vector(0.0f, 0.0f, 0.0f, vLight);

			mu_float fLuminosity = EMath::Abs(EMath::Sin(FWorldTime*0.0004f))*0.4f;
			Vector(0.5f + fLuminosity, 0.5f + fLuminosity, 0.5f + fLuminosity, vLight);
			mu_int32 iBone[] = { 9, 20, 19, 10, 18,
				28, 27, 36, 35, 38,
				37, 53, 48, 62, 70,
				72, 71, 78, 79, 80,
				87, 90, 91, 106, 102 };
			mu_float fScale = 0.0f;

			for (mu_int32 i = 0; i < 25; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iBone[i])], vRelativePos, vPos, true);
				fScale = 0.5f;
				CreateSprite(BITMAP_CLUD64, vPos, fScale, vLight, o, FWorldTime*0.01f, 1);
			}

			mu_int32 iBoneThunder[] = { 11, 21, 29, 63, 81, 89 };
			if (GetLargeRand(2) == 0)
			{
				for (mu_int32 i = 0; i < 6; ++i)
				{
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iBoneThunder[i])], vRelativePos, vPos, true);
					if (GetLargeRand(20) == 0)
					{
						Vector(0.6f, 0.6f, 0.9f, vLight);
						CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, vPos, o->Angle, vLight, 1, o);
					}
				}
			}

			mu_int32 iBoneLight[] = { 64, 61, 69, 77, 86,
				98, 97, 99, 104, 103,
				105, 12, 8, 17, 26,
				34, 52, 44, 51, 50,
				49, 45 };

			fScale = EMath::Abs(EMath::Sin(FWorldTime*0.003f))*0.2f;
			for (mu_int32 i = 0; i < 22; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iBoneLight[i])], vRelativePos, vPos, true);
				if (iBoneLight[i] == 12 || iBoneLight[i] == 64 || iBoneLight[i] == 98 || iBoneLight[i] == 52)
				{
					Vector(0.9f, 0.0f, 0.0f, vLight);
					CreateSprite(BITMAP_LIGHT, vPos, fScale + 1.4f, vLight, o);
				}
				else
				{
					Vector(0.8f, 0.5f, 0.2f, vLight);
					CreateSprite(BITMAP_LIGHT, vPos, fScale + 0.3f, vLight, o);
				}
			}
		}
	}
	else if (Type == MODEL_WING + 37)
	{
		if (MU_UPDATECOUNTER > 0)
		{
			EVector3  p, Position, Light;
			Vector(0.0f, 0.0f, 0.0f, p);
			mu_float Scale = EMath::Abs(EMath::Sin(FWorldTime*0.003f))*0.2f;
			mu_float Luminosity = EMath::Abs(EMath::Sin(FWorldTime*0.003f))*0.3f;

			Vector(0.5f + Luminosity, 0.5f + Luminosity, 0.6f + Luminosity, Light);
			mu_int32 iRedFlarePos[] = { 24, 31, 15, 8, 53, 35 };
			for (mu_int32 i = 0; i < 6; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iRedFlarePos[i])], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, Scale + 1.3f, Light, o);
			}

			Vector(0.1f, 0.1f, 0.9f, Light);
			mu_int32 iGreenFlarePos[] = { 22, 23, 25, 29, 30, 28, 32, 13, 16, 14, 12, 9, 7, 6, 57, 58, 40, 39 };
			for (mu_int32 i = 0; i < 18; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iGreenFlarePos[i])], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, Scale + 1.5f, Light, o);
			}
			mu_int32 iGreenFlarePos2[] = { 56, 38, 51, 45 };
			for (mu_int32 i = 0; i < 4; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iGreenFlarePos2[i])], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, Scale + 0.5f, Light, o);
			}
		}
	}
	else if (Type == MODEL_WING + 38)
	{
		if (MU_UPDATECOUNTER > 0)
		{
			EVector3  p, Position, Light;
			Vector(0.0f, 0.0f, 0.0f, p);
			mu_float Scale = EMath::Abs(EMath::Sin(FWorldTime*0.002f))*0.2f;
			mu_float Luminosity = EMath::Abs(EMath::Sin(FWorldTime*0.002f))*0.4f;

			Vector(0.5f + Luminosity, 0.0f + Luminosity, 0.0f + Luminosity, Light);
			mu_int32 iRedFlarePos[] = { 5, 6, 7, 8, 18, 19, 23, 24, 25, 27, 37, 38 };
			for (mu_int32 i = 0; i < 12; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iRedFlarePos[i])], p, Position, true);
				CreateSprite(BITMAP_FLARE, Position, Scale + 0.6f, Light, o);
			}

			Vector(0.0f + Luminosity, 0.5f + Luminosity, 0.0f + Luminosity, Light);
			mu_int32 iGreenFlarePos[] = { 4, 9, 13, 14, 26, 32, 31, 33 };
			for (mu_int32 i = 0; i < 8; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iGreenFlarePos[i])], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 1.3f, Light, o);
			}

			Vector(1.0f, 1.0f, 1.0f, Light);
			mu_float fLumi = (EMath::Sin(FWorldTime*0.004f) + 1.0f) * 0.05f;
			Vector(0.8f + fLumi, 0.8f + fLumi, 0.3f + fLumi, Light);
			CreateSprite(BITMAP_LIGHT, Position, 0.4f, Light, o, 0.5f);
			if (GetLargeRand(15) >= 8)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(13)], p, Position, true);
				CreateParticle(ThreadIndex, BITMAP_SHINY, Position, o->Angle, Light, 5, 0.5f);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(31)], p, Position, true);
				CreateParticle(ThreadIndex, BITMAP_SHINY, Position, o->Angle, Light, 5, 0.5f);
			}
		}
	}
	else if (Type == MODEL_WING + 39)
	{
		if (MU_UPDATECOUNTER > 0)
		{
			EVector3  p, Position, Light;
			Vector(0.0f, 0.0f, 0.0f, p);
			mu_float Scale = EMath::Abs(EMath::Sin(FWorldTime*0.003f))*0.2f;
			mu_float Luminosity = EMath::Abs(EMath::Sin(FWorldTime*0.003f))*0.3f;

			Vector(0.7f + Luminosity, 0.5f + Luminosity, 0.8f + Luminosity, Light);
			mu_int32 iRedFlarePos[] = { 6, 15, 24, 56, 47, 38 };
			for (mu_int32 i = 0; i < 6; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iRedFlarePos[i])], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, Scale + 1.5f, Light, o);
			}

			Vector(0.6f, 0.4f, 0.7f, Light);
			mu_int32 iSparkPos[] = { 7, 16, 25, 57, 48, 39,
				11, 22, 31, 63, 54, 40,
				10, 21, 30, 62, 53, 41,
				9, 20, 29, 61, 52, 42,
				8, 19, 28, 60, 51, 43,
				18, 27, 59, 50,
				17, 26, 58, 49 };
			mu_int32 iNumParticle = 1;
			for (mu_int32 i = 0; i < 6; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iSparkPos[i])], p, Position, true);
				for (mu_int32 j = 0; j < iNumParticle; ++j)
					CreateParticle(ThreadIndex, BITMAP_CHROME_ENERGY2, Position, o->Angle, Light, 0, 0.1f);
			}

			for (mu_int32 i = 6; i < 18; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iSparkPos[i])], p, Position, true);
				for (mu_int32 j = 0; j < iNumParticle; ++j)
					CreateParticle(ThreadIndex, BITMAP_CHROME_ENERGY2, Position, o->Angle, Light, 0, 0.3f);
			}

			for (mu_int32 i = 18; i < 30; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iSparkPos[i])], p, Position, true);
				for (mu_int32 j = 0; j < iNumParticle; ++j)
					CreateParticle(ThreadIndex, BITMAP_CHROME_ENERGY2, Position, o->Angle, Light, 0, 0.5f);
			}

			for (mu_int32 i = 30; i < 38; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iSparkPos[i])], p, Position, true);
				for (mu_int32 j = 0; j < iNumParticle; ++j)
					CreateParticle(ThreadIndex, BITMAP_CHROME_ENERGY2, Position, o->Angle, Light, 0, 0.7f);
			}
		}
	}
	else if (Type == MODEL_WING + 43)
	{
		if (MU_UPDATECOUNTER > 0)
		{
			EVector3  p, Position, Light;
			Vector(0.0f, 0.0f, 0.0f, p);
			mu_float Scale = EMath::Abs(EMath::Sin(FWorldTime*0.002f))*0.2f;
			mu_float Luminosity = EMath::Abs(EMath::Sin(FWorldTime*0.002f))*0.4f;

			Vector((1.0f + Luminosity) * 0.5f, (0.7f + Luminosity) * 0.5f, (0.2f + Luminosity) * 0.5f, Light);
			mu_int32 iFlarePos0[] = { 7, 30, 31, 43, 8, 20 };

			mu_int32 icnt;
			for (icnt = 0; icnt < 2; ++icnt)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iFlarePos0[icnt])], p, Position, true);
				CreateSprite(BITMAP_FLARE, Position, Scale + 2.0f, Light, o);
			}
			Vector((1.0f + Luminosity) / 4.0f, (0.7f + Luminosity) / 4.0f, (0.2f + Luminosity) / 4.0f, Light);
			for (; icnt < 6; ++icnt)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iFlarePos0[icnt])], p, Position, true);
				CreateSprite(BITMAP_FLARE, Position, Scale + 0.5f, Light, o);
			}

			Vector((0.5f + Luminosity) * 0.5f, (0.1f + Luminosity) * 0.5f, (0.4f + Luminosity) * 0.5f, Light);
			mu_int32 iGreenFlarePos[] = { 29, 38, 42, 19, 15, 6 };

			for (mu_int32 i = 0; i < 6; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iGreenFlarePos[i])], p, Position, true);
				CreateSprite(BITMAP_FLARE, Position, Scale + 2.0f, Light, o);
			}
		}
	}

	mu_float Luminosity = 1.0f;
	if (g_isCharacterBuff(o, eBuff_Cloaking))
	{
		Alpha = 0.5f;
		Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
		b->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME5, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU*8.0f, o->BlendMeshTexCoordV*2.0f, -1, BITMAP_CHROME2);
	}
	else if (g_isCharacterBuff(o, eDeBuff_Poison) && g_isCharacterBuff(o, eDeBuff_Freeze))
	{
		Vector(Luminosity*0.3f, Luminosity*1.0f, Luminosity*1.0f, b->BodyLight(ThreadIndex));
		RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
	}
	else if (g_isCharacterBuff(o, eDeBuff_BlowOfDestruction))
	{
		Vector(Luminosity*0.3f, Luminosity*1.0f, Luminosity*1.0f, b->BodyLight(ThreadIndex));
		RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
	}
	else if (g_isCharacterBuff(o, eDeBuff_Poison))
	{
		Vector(Luminosity*0.3f, Luminosity*1.0f, Luminosity*0.5f, b->BodyLight(ThreadIndex));
		RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
	}
	else if (g_isCharacterBuff(o, eDeBuff_Stun))
	{
		DeleteEffect(BITMAP_SKULL, o, 5);
		CreateEffect(ThreadIndex, BITMAP_SKULL, o->Position, o->Angle, Light, 5, o);
		Vector(Luminosity*0.0f, Luminosity*0.0f, Luminosity*1.0f, b->BodyLight(ThreadIndex));
		RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
	}
	else if (g_isCharacterBuff(o, eDeBuff_Freeze))
	{
		Vector(Luminosity*0.3f, Luminosity*0.5f, Luminosity*1.0f, b->BodyLight(ThreadIndex));
		RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
	}
	else if (g_isCharacterBuff(o, eDeBuff_BlowOfDestruction))
	{
		Vector(Luminosity*0.3f, Luminosity*0.5f, Luminosity*1.0f, b->BodyLight(ThreadIndex));
		RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
	}
	else if (g_isCharacterBuff(o, eDeBuff_Harden))
	{
		Vector(Luminosity*0.3f, Luminosity*0.5f, Luminosity*1.0f, b->BodyLight(ThreadIndex));
		RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
	}
	else if (Level < 3 || o->Type == MODEL_POTION + 15)
	{
		if (o->Type == MODEL_POTION + 64)
		{
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 0.5f, RENDER_TEXTURE | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 0.5f);
			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 1.0f, RENDER_CHROME4 | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
		}
		else if (o->Type == MODEL_HELPER + 50)
		{
			VectorCopy(Light, b->BodyLight(ThreadIndex));
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 1.5f, RENDER_CHROME2 | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.5f);
			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 1.0f, RENDER_CHROME4 | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
		}
		else if (o->Type == MODEL_POTION + 42 || o->Type == MODEL_HELPER + 38)
		{
			VectorCopy(Light, b->BodyLight(ThreadIndex));
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);

			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 1.5f, RENDER_CHROME2 | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.5f);
			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 1.0f, RENDER_CHROME4 | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
		}
		else if (o->Type == MODEL_HELPER + 43
			|| o->Type == MODEL_HELPER + 93
			)
		{
			Vector(Light[0] * 0.9f, Light[1] * 0.9f, Light[2] * 0.9f, b->BodyLight(ThreadIndex));
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 1.5f, RENDER_CHROME2 | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.5f);
			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 1.0f, RENDER_CHROME4 | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
		}
		else if (o->Type == MODEL_HELPER + 44
			|| o->Type == MODEL_HELPER + 94
			|| o->Type == MODEL_HELPER + 116
			)
		{
			Vector(Light[0] * 0.9f, Light[1] * 0.9f, Light[2] * 0.9f, b->BodyLight(ThreadIndex));
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 1.5f, RENDER_CHROME2 | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.5f);
			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 1.0f, RENDER_CHROME4 | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
		}
		else if (o->Type == MODEL_HELPER + 45)
		{
			Vector(Light[0] * 0.9f, Light[1] * 0.9f, Light[2] * 0.9f, b->BodyLight(ThreadIndex));
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 1.5f, RENDER_CHROME2 | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.5f);
			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 1.0f, RENDER_CHROME4 | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
		}
		else
		{
			VectorCopy(Light, b->BodyLight(ThreadIndex));
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
		}
	}
	else if (Level < 5)
	{
		EVector3 l;
		Vector(g_Luminosity, g_Luminosity*0.6f, g_Luminosity*0.6f, l);
		VectorMul(l, Light, b->BodyLight(ThreadIndex));
		RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
	}
	else if (Level < 7)
	{
		EVector3 l;
		Vector(g_Luminosity*0.5f, g_Luminosity*0.7f, g_Luminosity, l);
		VectorMul(l, Light, b->BodyLight(ThreadIndex));
		RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
	}
	else if (g_pOption->GetRenderLevel())
	{
		if (Level < 8 && g_pOption->GetRenderLevel() >= 1)  //  +7
		{
			Vector(Light[0] * 0.8f, Light[1] * 0.8f, Light[2] * 0.8f, b->BodyLight(ThreadIndex));
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
			RenderPartObjectBodyColor(ThreadIndex, b, o, Type, Alpha, RENDER_CHROME | RENDER_BRIGHT, 1.0f);
		}
		else if (Level < 9 && g_pOption->GetRenderLevel() >= 1)  //  +8
		{
			Vector(Light[0] * 0.8f, Light[1] * 0.8f, Light[2] * 0.8f, b->BodyLight(ThreadIndex));
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
			RenderPartObjectBodyColor(ThreadIndex, b, o, Type, Alpha, RENDER_CHROME | RENDER_BRIGHT, 1.0f);
		}
		else if (Level < 10 && g_pOption->GetRenderLevel() >= 2) //  +9
		{
			Vector(Light[0] * 0.9f, Light[1] * 0.9f, Light[2] * 0.9f, b->BodyLight(ThreadIndex));
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
			RenderPartObjectBodyColor(ThreadIndex, b, o, Type, Alpha, RENDER_CHROME | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
			RenderPartObjectBodyColor(ThreadIndex, b, o, Type, Alpha, RENDER_METAL | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
		}
		else if (Level < 11 && g_pOption->GetRenderLevel() >= 2) //  +10
		{
			Vector(Light[0] * 0.9f, Light[1] * 0.9f, Light[2] * 0.9f, b->BodyLight(ThreadIndex));
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
			RenderPartObjectBodyColor(ThreadIndex, b, o, Type, Alpha, RENDER_CHROME | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
			RenderPartObjectBodyColor(ThreadIndex, b, o, Type, Alpha, RENDER_METAL | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
		}
		else if (Level < 12 && g_pOption->GetRenderLevel() >= 3) //  +11
		{
			Vector(Light[0] * 0.9f, Light[1] * 0.9f, Light[2] * 0.9f, b->BodyLight(ThreadIndex));
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 1.0f, RENDER_CHROME2 | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
			RenderPartObjectBodyColor(ThreadIndex, b, o, Type, Alpha, RENDER_METAL | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
			RenderPartObjectBodyColor(ThreadIndex, b, o, Type, Alpha, RENDER_CHROME | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
		}
		else if (Level < 13 && g_pOption->GetRenderLevel() >= 3) //  +12
		{
			Vector(Light[0] * 0.9f, Light[1] * 0.9f, Light[2] * 0.9f, b->BodyLight(ThreadIndex));
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 1.0f, RENDER_CHROME2 | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
			RenderPartObjectBodyColor(ThreadIndex, b, o, Type, Alpha, RENDER_METAL | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
			RenderPartObjectBodyColor(ThreadIndex, b, o, Type, Alpha, RENDER_CHROME | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
		}
		else if (Level < 14 && g_pOption->GetRenderLevel() >= 4) //  +13
		{
			Vector(Light[0] * 0.9f, Light[1] * 0.9f, Light[2] * 0.9f, b->BodyLight(ThreadIndex));
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 1.0f, RENDER_CHROME4 | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
			RenderPartObjectBodyColor(ThreadIndex, b, o, Type, Alpha, RENDER_METAL | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
			RenderPartObjectBodyColor(ThreadIndex, b, o, Type, Alpha, RENDER_CHROME | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
		}
		else if (Level < 15 && g_pOption->GetRenderLevel() >= 4) //  +14
		{
			Vector(Light[0] * 0.9f, Light[1] * 0.9f, Light[2] * 0.9f, b->BodyLight(ThreadIndex));
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 1.0f, RENDER_CHROME4 | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
			RenderPartObjectBodyColor(ThreadIndex, b, o, Type, Alpha, RENDER_METAL | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
			RenderPartObjectBodyColor(ThreadIndex, b, o, Type, Alpha, RENDER_CHROME | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
		}
		else if (Level < 16 && g_pOption->GetRenderLevel() >= 4) //  +15
		{
			Vector(Light[0] * 0.9f, Light[1] * 0.9f, Light[2] * 0.9f, b->BodyLight(ThreadIndex));
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, 1.0f, RENDER_CHROME4 | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
			RenderPartObjectBodyColor(ThreadIndex, b, o, Type, Alpha, RENDER_METAL | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
			RenderPartObjectBodyColor(ThreadIndex, b, o, Type, Alpha, RENDER_CHROME | RENDER_BRIGHT | (RenderType&RENDER_EXTRA), 1.0f);
		}
		else
		{
			VectorCopy(Light, b->BodyLight(ThreadIndex));
			RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
		}
	}
	else
	{
		VectorCopy(Light, b->BodyLight(ThreadIndex));
		RenderPartObjectBody(ThreadIndex, b, o, Type, Alpha, RenderType);
	}

	if (g_pOption->GetRenderLevel() == 0)
	{
		return;
	}

	if (!g_isCharacterBuff(o, eDeBuff_Harden) && !g_isCharacterBuff(o, eBuff_Cloaking)
		&& !g_isCharacterBuff(o, eDeBuff_CursedTempleRestraint)
		)
	{
		if (ExcOption > 0 && (o->Type<MODEL_WING || o->Type>MODEL_WING + 6) && o->Type != MODEL_HELPER + 30
			&& (o->Type<MODEL_WING + 36 || o->Type>MODEL_WING + 43)
			&& (o->Type < MODEL_WING + 130 || MODEL_WING + 134 < o->Type)
			)
		{
			Luminosity = EMath::Sin(FWorldTime*0.002f)*0.5f + 0.5f;
			Vector(Luminosity, Luminosity*0.3f, 1.0f - Luminosity, b->BodyLight(ThreadIndex));
			Alpha = 1.0f;

			if (b->HideSkin(ThreadIndex) && MODEL_HELM + 39 <= o->Type && MODEL_HELM + 44 >= o->Type)
			{
				mu_int32 anMesh[6] = { 2, 1, 0, 2, 1, 2 };
				b->RenderMesh(ThreadIndex, anMesh[o->Type - (MODEL_HELM + 39)], RENDER_TEXTURE | RENDER_BRIGHT, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}
			else
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}
		}
		else if (((SetOption % 0x04) == EXT_A_SET_OPTION || (SetOption % 0x04) == EXT_B_SET_OPTION))
		{
			Alpha = EMath::Sin(FWorldTime*0.001f)*0.5f + 0.4f;

			RenderPartObjectBodyColor2(ThreadIndex, b, o, Type, Alpha, RENDER_CHROME3 | RENDER_BRIGHT, 1.0f);
		}
	}
}

void RenderPartObject(const mu_uint32 ThreadIndex, OBJECT *o, mu_int32 Type, void *p2, EVector3 Light, mu_float Alpha, mu_int32 ItemLevel, mu_int32 ExcOption, mu_int32 SetOption, mu_boolean GlobalTransform, mu_boolean HideSkin, mu_boolean Translate, mu_int32 Select, mu_int32 RenderType)
{
	if (Alpha <= 0.01f)
	{
		return;
	}

	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	PART_t *p = (PART_t*)p2;

	if (Type == MODEL_POTION + 12)
	{
		if (ItemLevel == 0)
		{
			Type = MODEL_EVENT;
		}
		else if (ItemLevel == 2)
		{
			Type = MODEL_EVENT + 1;
		}
	}

	MUModel *b = &MODELS::Data[Type];
	if (b->IsGPUAvailable() == false)
	{
		b->RequestLoad();
		return;
	}
	b->HideSkin(ThreadIndex) = HideSkin;
	b->BodyScale(ThreadIndex) = o->Scale;
	b->LightEnable(ThreadIndex) = o->Flags.Get(OBJECT::eOF_LIGHTENABLE);
	VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));
	b->ClothMesh(ThreadIndex) = -1;

	ThreadInfo.BoneScale = 1.0f;

	if (3 == Select)
	{
		ThreadInfo.BoneScale = 1.4f;
	}
	else if (2 == Select)
	{
		ThreadInfo.BoneScale = 1.2f;
	}
	else if (1 == Select)
	{
		mu_float Scale = o->m_fEdgeScale;

		if (o->Kind == KIND_NPC)
		{
			Vector(0.02f, 0.1f, 0.0f, b->BodyLight(ThreadIndex));
		}
		else
		{
			Vector(0.1f, 0.03f, 0.0f, b->BodyLight(ThreadIndex));
		}

		if (InChaosCastle())
		{
			Vector(0.1f, 0.01f, 0.0f, b->BodyLight(ThreadIndex));
			Scale = 1.0f + 0.1f / o->Scale;
		}

		RenderPartObjectEdge(ThreadIndex, b, o, RENDER_BRIGHT | RENDER_PREBLEND | RENDER_BACKCULL, Translate, Scale);

		if (o->Kind == KIND_NPC)
		{
			Vector(0.16f, 0.7f, 0.0f, b->BodyLight(ThreadIndex));
		}
		else
		{
			Vector(0.7f, 0.2f, 0.0f, b->BodyLight(ThreadIndex));
		}

		if (InChaosCastle())
		{
			Vector(0.7f, 0.07f, 0.0f, b->BodyLight(ThreadIndex));
			Scale = 1.0f + 0.04f / o->Scale + 0.02f;
		}

		RenderPartObjectEdge(ThreadIndex, b, o, RENDER_BRIGHT | RENDER_PREBLEND | RENDER_BACKCULL, Translate, Scale - 0.02f);
	}

	if (ThreadInfo.RenderGUI == true)
	{
		const EVector4 &TmpLight = MU::ARGBToFloat4(ThreadInfo.RenderLight);
		b->BodyLight(ThreadIndex) = TmpLight;
		Alpha = TmpLight[3];
	}
	else
	{
		BodyLight(ThreadIndex, o, b);
	}

	ThreadInfo.Translate = Translate;
	b->Transform(ThreadIndex, o->BoundingBoxMin, o->BoundingBoxMax, &o->OBB, Translate);

	if (p)
	{
		mu_int32 iCloth = 0;
		switch (Type)
		{
		case MODEL_PANTS + 18:
			iCloth = 1;
			break;

		case MODEL_PANTS + 19:
			iCloth = 2;
			break;

		case MODEL_PANTS + 22:
			iCloth = 3;
			break;
		}

		if (!p->m_pCloth[0] && iCloth > 0)
		{
			mu_int32 iCount = 1;
			CPhysicsClothMesh *pCloth[2] = { nullptr, nullptr };
			switch (iCloth)
			{
			case 1:
				pCloth[0] = new CPhysicsClothMesh;
				pCloth[0]->Create(ThreadIndex, o, 2, 17, 0.0f, 9.0f, 7.0f, 5, 8, 45.0f, 85.0f, BITMAP_PANTS_G_SOUL, BITMAP_PANTS_G_SOUL, PCT_MASK_ALPHA | PCT_HEAVY | PCT_STICKED | PCT_SHORT_SHOULDER, Type);
				pCloth[0]->AddCollisionSphere(0.0f, -15.0f, -20.0f, 30.0f, 2);
				break;
			case 2:
				iCount = 1;
				pCloth[0] = new CPhysicsClothMesh;
				pCloth[0]->Create(ThreadIndex, o, 3, 2, PCT_OPT_CORRECTEDFORCE | PCT_HEAVY, Type);
				pCloth[0]->AddCollisionSphere(0.0f, 0.0f, -15.0f, 22.0f, 2);
				pCloth[0]->AddCollisionSphere(0.0f, 0.0f, -27.0f, 23.0f, 2);
				pCloth[0]->AddCollisionSphere(0.0f, 0.0f, -40.0f, 24.0f, 2);
				pCloth[0]->AddCollisionSphere(0.0f, 0.0f, -54.0f, 25.0f, 2);
				pCloth[0]->AddCollisionSphere(0.0f, 0.0f, -69.0f, 26.0f, 2);
				break;
			case 3:
				pCloth[0] = new CPhysicsClothMesh;
				pCloth[0]->Create(ThreadIndex, o, 2, 17, 0.0f, 9.0f, 7.0f, 7, 5, 50.0f, 100.0f, b->_MeshTexture[2], b->_MeshTexture[2], PCT_MASK_ALPHA | PCT_HEAVY | PCT_STICKED | PCT_SHORT_SHOULDER, Type);
				pCloth[0]->AddCollisionSphere(0.0f, -15.0f, -20.0f, 30.0f, 2);
				break;
			}
			p->m_byNumCloth = iCount;
			p->m_pCloth[0] = pCloth[0];
			p->m_pCloth[1] = pCloth[1];
		}

		if (p->m_pCloth[0])
		{
			if (iCloth > 0)
			{
				for (mu_int32 i = 0; i < p->m_byNumCloth; ++i)
				{
					CPhysicsCloth *pCloth = p->m_pCloth[i];
					if (MU_UPDATECOUNTER > 0 && !pCloth->Move2(ThreadIndex, 0.005f, 5))
					{
						DeleteCloth(nullptr, o, p);
						break;
					}
					else
					{
						pCloth->Render(ThreadIndex, b);
					}
				}
			}
			else if (iCloth == 0)
			{
				DeleteCloth(nullptr, o, p);
			}
		}
	}

	RenderPartObjectEffect(ThreadIndex, o, Type, Light, Alpha, ItemLevel, ExcOption, SetOption, Select, RenderType);
}

void RenderWheelWeapon(const mu_uint32 ThreadIndex, OBJECT *o)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 TempPosition, TempAngle;
	VectorCopy(o->Position, TempPosition);
	VectorCopy(o->Angle, TempAngle);

	o->Direction[2] -= 30;
	o->Angle[2] += o->Direction[2];
	o->Angle[1] = 90;
	o->Position[2] += 100.0f;

	mu_float Alpha = o->Alpha;

	mu_int32 Type = o->Owner->Weapon + MODEL_SWORD;
	MUModel *b = &MODELS::Data[Type];
	b->Skin(ThreadIndex) = GetBaseClass(Hero->Class);
	b->CurrentAction(ThreadIndex) = o->CurrentAction;
	VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));

	mu_int32 TempType = o->Type;
	o->Type = Type;
	ItemObjectAttribute(o);
	b->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle, false, false);
	ThreadInfo.SkeletalIndex = g_Tasking.RegisterSkeletal(ThreadInfo.BoneTransform, b->_BoneNonDummyEnd);
	o->BoneTransform = &g_Tasking._SkeletonBuffer[ThreadInfo.SkeletalIndex];

	EVector3 Light;
	RequestTerrainLight(o->Position[0], o->Position[1], Light);
	VectorAdd(Light, o->Light, Light);
	RenderPartObject(ThreadIndex, o, Type, nullptr, Light, Alpha, o->Owner->WeaponLevel, o->Owner->WeaponExcOption, o->Owner->WeaponSetOption, true, true, true);
	o->Type = TempType;

	VectorCopy(TempPosition, o->Position);
	VectorCopy(TempAngle, o->Angle);
}

void RenderFuryStrike(const mu_uint32 ThreadIndex, OBJECT *o)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (o->LifeTime > 10 && o->Kind == 0)
	{
		EVector3 p, Light, Angle;
		mu_float Alpha = o->Alpha;

		Vector(0.0f, 100.0f, 0.0f, p);

		mu_int32 Type = o->Owner->Weapon + MODEL_SWORD;
		MUModel *b = &MODELS::Data[Type];
		b->Skin(ThreadIndex) = GetBaseClass(Hero->Class);
		b->CurrentAction(ThreadIndex) = o->CurrentAction;
		VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));

		mu_int32 TempType = o->Type;
		mu_float tempGravity = o->Gravity;
		o->Type = Type;
		ItemObjectAttribute(o);
		o->Gravity = tempGravity;

		RequestTerrainLight(o->Position[0], o->Position[1], Light);
		VectorAdd(Light, o->Light, Light);
		VectorCopy(o->Angle, Angle);

		b->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, Angle, o->HeadAngle, false, false);
		ThreadInfo.SkeletalIndex = g_Tasking.RegisterSkeletal(ThreadInfo.BoneTransform, b->_BoneNonDummyEnd);
		o->BoneTransform = &g_Tasking._SkeletonBuffer[ThreadInfo.SkeletalIndex];

		RenderPartObject(ThreadIndex, o, Type, nullptr, Light, Alpha, o->Owner->WeaponLevel, o->Owner->WeaponExcOption, o->Owner->WeaponSetOption, true, true, true);

		o->Type = TempType;
	}
}

void RenderSkillSpear(const mu_uint32 ThreadIndex, OBJECT *o)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	MUModel *b = &MODELS::Data[MODEL_SPEARSKILL];
	b->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle, false, false);
	o->BlendMeshLight = 1.0f;
	EVector3 Light = o->Light * (mu_float)o->LifeTime * 0.05f;
	RenderPartObject(ThreadIndex, o, MODEL_SPEARSKILL, nullptr, Light, 1.0f, 0, 0, 0, true, true, true, false, RENDER_BRIGHT | RENDER_TEXTURE);
}

void RenderBrightEffect(const mu_uint32 ThreadIndex, MUModel *b, mu_int32 Bitmap, mu_int32 Link, mu_float Scale, EVector3 Light, OBJECT *o)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];
	EVector3 p, Position;
	Vector(0.0f, 0.0f, 0.0f, p);
	b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(Link)], p, Position, true);
	CreateSprite(Bitmap, Position, Scale, Light, o);
}

void RenderLight(const mu_uint32 ThreadIndex, OBJECT *o, mu_int32 Texture, mu_float Scale, mu_int32 Bone, mu_float x, mu_float y, mu_float z)
{
	MUModel *b = &MODELS::Data[o->Type];
	EVector3 p, Position;
	Vector(x, y, z, p);
	b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(Bone)], p, Position, true);
	mu_float Luminosity = EMath::Sin(FWorldTime*0.002f)*0.3f + 0.7f;
	EVector3 Light;
	Vector(Luminosity*1.0f, Luminosity*0.6f, Luminosity*0.4f, Light);
	CreateSprite(Texture, Position, Scale, Light, o);
}

void RenderEye(const mu_uint32 ThreadIndex, OBJECT *o, mu_int32 Left, mu_int32 Right, mu_float fSize)
{
	MUModel *b = &MODELS::Data[o->Type];
	EVector3 p, Position;
	mu_float Luminosity = EMath::Sin(FWorldTime*0.002f)*0.3f + 0.8f;
	EVector3 Light;
	Vector(Luminosity, Luminosity, Luminosity, Light);
	Vector(5.0f, 0.0f, 0.0f, p);
	b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(Left)], p, Position, true);
	CreateSprite(BITMAP_SHINY + 3, Position, fSize, Light, nullptr);
	Vector(-5.0f, 0.0f, 0.0f, p);
	b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(Right)], p, Position, true);
	CreateSprite(BITMAP_SHINY + 3, Position, fSize, Light, nullptr);
	VectorCopy(Position, o->EyeLeft);
}

void RenderLinkObject(const mu_uint32 ThreadIndex, mu_float x, mu_float y, mu_float z, CHARACTER *c, PART_t *f, mu_int32 Type, mu_int32 Level, mu_int32 ExcOption, mu_boolean Link, mu_boolean Translate, mu_int32 RenderType, mu_boolean bRightHandItem)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	OBJECT *o = &c->Object;
	MUModel *b = &MODELS::Data[Type];
	if (b->IsGPUAvailable() == false)
	{
		b->RequestLoad();
		return;
	}
	MUModel *bOwner = &MODELS::Data[o->Type];

	if (o->SubType == MODEL_CURSEDTEMPLE_ALLIED_PLAYER || o->SubType == MODEL_CURSEDTEMPLE_ILLUSION_PLAYER)
	{
		if (Type >= MODEL_WING && Type <= MODEL_WING + 6) return;
		else if (Type >= MODEL_WING + 36 && Type <= MODEL_WING + 43)
			return;
		else if (ITEM_WING + 130 <= Type && Type <= ITEM_WING + 134) return;
	}

	if (Type == MODEL_HELPER + 30)
		return;

	if (ITEM_WING + 130 == Type)
		return;

	if (Type >= MODEL_STAFF + 21 && Type <= MODEL_STAFF + 29)
	{
		return;
	}

	if (g_isCharacterBuff(o, eBuff_Cloaking))
		return;

	b->BodyScale(ThreadIndex) = o->Scale;
	b->CurrentAction(ThreadIndex) = f->CurrentAction;
	b->BodyHeight(ThreadIndex) = 0.0f;

	static OBJECT ItemObjects[MAX_TASKING_THREADS];
	OBJECT* Object = &ItemObjects[ThreadIndex];

	Object->Type = Type;
	ItemObjectAttribute(Object);
	Object->Scale = o->Scale; // Fix effects
	b->LightEnable(ThreadIndex) = Object->Flags.Get(OBJECT::eOF_LIGHTENABLE);
	b->LightEnable(ThreadIndex) = false;

	g_CharacterCopyBuff(Object, o);

	switch (Type)
	{
	case MODEL_WING + 39:
		f->PlaySpeed = 0.15f;
		f->PlaySpeed *= MU_FRAMEELAPSED;
		break;
	}

	if (c->MonsterIndex >= 529 && c->MonsterIndex <= 539)
	{
		if (GET_WORLD == WD_65DOPPLEGANGER1)
			RenderType = RENDER_DOPPELGANGER | RENDER_TEXTURE;
		else
			RenderType = RENDER_DOPPELGANGER | RENDER_BRIGHT | RENDER_TEXTURE;
	}

	OBB_t OBB;
	EVector3 p, Position;

	if (Link)
	{
		EVector3 Angle(0.0f, 0.0f, 0.0f);
		EVector3 Pos(0.0f, 0.0f, 0.0f);
#ifdef USE_DUAL_QUATERNION
		EDualQuat Matrix;
#else
		EMatrix4 Matrix;
#endif

		if (c->MonsterIndex >= 132 && c->MonsterIndex <= 134)
		{
			if (Type == MODEL_STAFF + 10 || Type == MODEL_SWORD + 19)
			{
				Vector(90.0f, 0.0f, 90.0f, Angle);
				Vector(0.0f, 80.0f, 120.0f, Pos);
			}
			else if (Type == MODEL_BOW + 18)
			{
				Vector(10.0f, 0.0f, 0.0f, Angle);
				Vector(0.0f, 11.0f, 80.0f, Pos);
			}
		}
		else if (Type >= MODEL_WING + 40)
		{
			Vector(0.0f, 90.0f, 0.0f, Angle);
			Vector(-47.0f, -7.0f, 0.0f, Pos);
		}
		else if ((Type >= MODEL_BOW + 8 && Type<MODEL_BOW + 15) || (Type >= MODEL_BOW + 16 && Type<MODEL_BOW + 17) ||
			(Type >= MODEL_BOW + 18 && Type<MODEL_BOW + 20))
		{
			Vector(0.0f, 20.0f, 180.0f, Angle);
			Vector(-10.0f, 8.0f, 40.0f, Pos);
		}
		else if (Type == MODEL_15GRADE_ARMOR_OBJ_ARMLEFT || Type == MODEL_15GRADE_ARMOR_OBJ_ARMRIGHT ||
			Type == MODEL_15GRADE_ARMOR_OBJ_BODYLEFT || Type == MODEL_15GRADE_ARMOR_OBJ_BODYRIGHT ||
			Type == MODEL_15GRADE_ARMOR_OBJ_BOOTLEFT || Type == MODEL_15GRADE_ARMOR_OBJ_BOOTRIGHT ||
			Type == MODEL_15GRADE_ARMOR_OBJ_HEAD || Type == MODEL_15GRADE_ARMOR_OBJ_PANTLEFT ||
			Type == MODEL_15GRADE_ARMOR_OBJ_PANTRIGHT)
		{
			switch (Type)
			{
			case MODEL_15GRADE_ARMOR_OBJ_ARMLEFT:
				{
					Vector(0.0f, -90.0f, 0.0f, Angle);
					Vector(30.0f, 0.0f, 20.0f, Pos);
				}break;
			case MODEL_15GRADE_ARMOR_OBJ_ARMRIGHT:
				{
					Vector(0.0f, -90.0f, 0.0f, Angle);
					Vector(30.0f, 0.0f, -20.0f, Pos);
				}break;
			case MODEL_15GRADE_ARMOR_OBJ_BODYLEFT:
				{
					Vector(0.0f, -90.0f, 0.0f, Angle);
					Vector(5.0f, -20.0f, 0.0f, Pos);
				}break;
			case MODEL_15GRADE_ARMOR_OBJ_BODYRIGHT:
				{
					Vector(0.0f, -90.0f, 0.0f, Angle);
					Vector(5.0f, -20.0f, 0.0f, Pos);
				}break;
			case MODEL_15GRADE_ARMOR_OBJ_BOOTLEFT:
				{
					Vector(0.0f, 90.0f, 180.0f, Angle);
					Vector(20.0f, 15.0f, -10.0f, Pos);
				}break;
			case MODEL_15GRADE_ARMOR_OBJ_BOOTRIGHT:
				{
					Vector(0.0f, 90.0f, 180.0f, Angle);
					Vector(20.0f, 15.0f, 10.0f, Pos);
				}break;
			case MODEL_15GRADE_ARMOR_OBJ_HEAD:
				{
					Vector(180.0f, -90.0f, 0.0f, Angle);
					Vector(28.0f, 20.0f, 0.0f, Pos);
				}break;
			case MODEL_15GRADE_ARMOR_OBJ_PANTLEFT:
				{
					Vector(0.0f, 90.0f, 180.0f, Angle);
					Vector(25.0f, 5.0f, -5.0f, Pos);
				}break;
			case MODEL_15GRADE_ARMOR_OBJ_PANTRIGHT:
				{
					Vector(0.0f, 90.0f, 180.0f, Angle);
					Vector(25.0f, 5.0f, 5.0f, Pos);
				}break;
			}
		}
		else
		{
			if (Type == MODEL_STAFF + 9)
			{
				Vector(90.0f + 20.0f, 180.0f, 90.0f, Angle);
			}
			else
			{
				Vector(90.0f - 20.0f, 0.0f, 90.0f, Angle);
			}

			if (Type == MODEL_BOW + 20)
			{
				Vector(-60.0f, 0.0f, -80.0f, Angle);
				Vector(-5.0f, 20.0f, 0.0f, Pos);
			}
			else if (Type == MODEL_BOW + 23)
			{
				Vector(-60.0f, 0.0f, -80.0f, Angle);
				Vector(-5.0f, 20.0f, -5.0f, Pos);
			}
			else if (Type == MODEL_BOW + 24)
			{
				Vector(40.0f, 0.0f, -80.0f, Angle);
				Vector(-30.0f, 20.0f, -10.0f, Pos);
			}
			else if (Type >= MODEL_BOW && Type < MODEL_STAFF)
			{
				Vector(-10.0f, 5.0f, 10.0f, Pos);
			}
			else if (Type == MODEL_STAFF + 9)
			{
				Vector(-10.0f, 5.0f, -10.0f, Pos);
			}
			else if (Type == MODEL_STAFF + 8)
			{
				Vector(-10.0f, 5.0f, 10.0f, Pos);
			}
			else if (Type >= MODEL_SHIELD && Type < MODEL_HELM)
			{
				if (Type == MODEL_SHIELD + 16)
				{
					Vector(30.0f, 0.0f, 90.0f, Angle);
					Vector(-20.0f, 0.0f, -20.0f, Pos);
				}
				else if (Type == MODEL_SHIELD + 14 || Type == MODEL_SHIELD + 15)
				{
					Vector(50.0f, 0.0f, 90.0f, Angle);
					Vector(-28.0f, 0.0f, -25.0f, Pos);
				}
				else if (Type == MODEL_SHIELD + 6)
				{
					Vector(30.0f, 0.0f, 90.0f, Angle);
					Vector(-15.0f, 0.0f, -25.0f, Pos);
				}
				else
				{
					Vector(-10.0f, 0.0f, 0.0f, Pos);
				}
			}
			else
			{
				Vector(-20.0f, 5.0f, 40.0f, Pos);
			}
		}

		if (bRightHandItem == false
			&& !(Type >= MODEL_SHIELD && Type < MODEL_HELM)
			&& Type != MODEL_BOW + 20
			)
		{
			Angle[2] += 180.0f;
			Pos[0] += 30.0f;
			Pos[1] += 3.0f;
		}

#ifdef USE_DUAL_QUATERNION
		Matrix.Set(Angle, Pos);
		Matrix.MixOut(o->BoneTransform[f->LinkBone], ThreadInfo.ParentMatrix);
#else
		AngleMatrix(Angle, Matrix);
		Matrix[3][0] = Pos[0];
		Matrix[3][1] = Pos[1];
		Matrix[3][2] = Pos[2];

		R_ConcatTransforms(o->BoneTransform[f->LinkBone], Matrix, ThreadInfo.ParentMatrix);
#endif

		VectorCopy(c->Object.Position, b->BodyOrigin(ThreadIndex));

		Vector(0.0f, 0.0f, 0.0f, Object->Angle);
	}
	else
	{
		Vector(x, y, z, p);
		MUModel *Owner = &MODELS::Data[o->Type];
		Owner->RotationPosition(ThreadIndex, o->BoneTransform[f->LinkBone], p, Position);
		VectorAdd(c->Object.Position, Position, b->BodyOrigin(ThreadIndex));
		Vector(0.0f, 0.0f, 0.0f, Object->Angle);
	}

	if ((c->Skill) == AT_SKILL_PIERCING &&
		((o->Type == MODEL_PLAYER && o->CurrentAction >= PLAYER_ATTACK_FIST && o->CurrentAction <= PLAYER_RIDE_SKILL)))
	{
		if (o->AnimationFrame >= 5.0f && o->AnimationFrame <= 10.0f)
		{
			f->PriorAnimationFrame = 2.0f;
			f->AnimationFrame = 3.2f;
		}
	}

	if ((f->Type >= MODEL_WING + 6 && f->Type <= MODEL_WING + 6) && c->SafeZone)
	{
		b->CurrentAction(ThreadIndex) = 1;
	}

	if (!Link || (Type<MODEL_BOW || Type >= MODEL_STAFF) || Type == MODEL_BOW + 23)
	{
		if (!g_isCharacterBuff(o, eDeBuff_Stun)
			&& !g_isCharacterBuff(o, eDeBuff_Sleep)
			)
		{
			b->PlayAnimation(ThreadIndex, &f->AnimationFrame, &f->PriorAnimationFrame, &f->PriorAction, f->PlaySpeed);
		}
	}

	VectorCopy(b->BodyOrigin(ThreadIndex), Object->Position);

	EVector3 Temp;
	b->Animation(ThreadIndex, ThreadInfo.BoneTransform, f->AnimationFrame, f->PriorAnimationFrame, f->PriorAction, Object->Angle, Object->Angle, true);
	ThreadInfo.SkeletalIndex = g_Tasking.RegisterSkeletal(ThreadInfo.BoneTransform, b->_BoneNonDummyEnd);
	Object->BoneTransform = &g_Tasking._SkeletonBuffer[ThreadInfo.SkeletalIndex];
	ThreadInfo.Translate = Translate;
	b->Transform(ThreadIndex, Temp, Temp, &OBB, Translate);
	RenderPartObjectEffect(ThreadIndex, Object, Type, c->Light, o->Alpha, Level, ExcOption, 0, 0,
		RenderType | ((c->MonsterIndex == 67 || c->MonsterIndex == 137)
			? (RENDER_EXTRA | RENDER_TEXTURE) : RENDER_TEXTURE));

	if (MU_UPDATECOUNTER > 0)
	{
		mu_float Luminosity;
		EVector3 Light;
		Luminosity = (mu_float)(GetLargeRand(30) + 70)*0.005f;
		switch (Type)
		{
		case MODEL_BOW + 4:
		case MODEL_BOW + 5:
		case MODEL_BOW + 6:
		case MODEL_BOW + 17:
			if (Type == MODEL_BOW + 6)
			{
				Vector(Luminosity*0.6f, Luminosity*1.0f, Luminosity*0.8f, Light);
				for (mu_int32 i = 13; i <= 18; ++i)
					RenderBrightEffect(ThreadIndex, b, BITMAP_SHINY + 1, i, 1.0f, Light, o);
			}
			else if (Type == MODEL_BOW + 17)
			{
				Vector(Luminosity*0.5f, Luminosity*0.5f, Luminosity*0.8f, Light);
				for (mu_int32 i = 13; i <= 18; ++i)
					RenderBrightEffect(ThreadIndex, b, BITMAP_SHINY + 1, i, 1.0f, Light, o);

				for (mu_int32 i = 5; i <= 8; ++i)
					RenderBrightEffect(ThreadIndex, b, BITMAP_SHINY + 1, i, 1.0f, Light, o);
			}
			else
			{
				Vector(Luminosity*1.0f, Luminosity*0.6f, Luminosity*0.2f, Light);
				RenderBrightEffect(ThreadIndex, b, BITMAP_SHINY + 1, 2, 1.0f, Light, o);
				RenderBrightEffect(ThreadIndex, b, BITMAP_SHINY + 1, 6, 1.0f, Light, o);
			}
			break;
		case MODEL_STAFF + 10:
			Vector(Luminosity*1.0f, Luminosity*0.3f, Luminosity*0.1f, Light);

			for (mu_int32 i = 0; i < 10; ++i)
			{
				EVector3 Light2;
				Vector(0.4f, 0.4f, 0.4f, Light2);
				Vector(i*30.0f - 180.0f, -40.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(0)], p, Position, true);

				if ((GetLargeRand(3)) == 0)
				{
					CreateSprite(BITMAP_SHINY + 1, Position, 0.6f, Light2, o, (mu_float)(GetLargeRand(360)));
				}
				CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);
			}
			break;
		case MODEL_BOW + 21:
			{
				Vector(0.8f, 0.8f, 0.2f, Light);
				Vector(0.0f, 0.0f, 0.0f, p);
				if (GetLargeRand(2) == 1)
				{
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(4)], p, Position, true);
					CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, o->Angle, Light, 11, 0.8f);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(12)], p, Position, true);
					CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, o->Angle, Light, 11, 0.8f);
				}
				Vector(0.5f, 0.5f, 0.1f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(7)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(15)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);

				Vector(20.0f, 0.0f, 0.0f, p);
				Vector(1.0f, 1.0f, 0.4f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(0)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 0.8f, Light, o);
				CreateSprite(BITMAP_LIGHT, Position, 1.3f, Light, o);
				CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);
			}
			break;
		case MODEL_MACE + 14:
			{
				Vector(1.0f, 0.3f, 0.0f, Light);

				mu_float fRendomPos = (mu_float)(GetLargeRand(60)) / 20.0f - 1.5f;
				mu_float fRendomScale = (mu_float)(GetLargeRand(10)) / 20.0f + 1.4f;
				Vector(0.0f, -100.0f + fRendomPos, fRendomPos, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(0)], p, Position, true);
				CreateSprite(BITMAP_SHINY + 1, Position, fRendomScale, Light, o);
				CreateSprite(BITMAP_SHINY + 1, Position, fRendomScale - 0.3f, Light, o, 20.0f);

				fRendomPos = (mu_float)(GetLargeRand(60)) / 20.0f - 1.5f;
				fRendomScale = (mu_float)(GetLargeRand(10)) / 20.0f + 1.0f;
				Vector(0.0f, -100.0f + fRendomPos, fRendomPos, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(0)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, fRendomScale + 0.3f, Light, o);

				fRendomPos = (mu_float)(GetLargeRand(40)) / 20.0f - 1.0f;
				fRendomScale = (mu_float)(GetLargeRand(8)) / 20.0f + 0.4f;
				Vector(0.0f, 100.0f + fRendomPos, fRendomPos, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(0)], p, Position, true);
				CreateSprite(BITMAP_SHINY + 1, Position, fRendomScale, Light, o);
				CreateSprite(BITMAP_SHINY + 1, Position, fRendomScale - 0.2f, Light, o, 90.0f);
			}
			break;
		case MODEL_SWORD + 22:
			{
				mu_float fLight = (mu_float)EMath::Sin((FWorldTime)*0.4f)*0.25f + 0.7f;
				Vector(fLight, fLight - 0.5f, fLight - 0.5f, Light);

				Vector(5.0f, -22.0f, -10.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(0)], p, Position, true);
				CreateSprite(BITMAP_LIGHT + 1, Position, 0.75f, Light, o);

				Vector(-5.0f, -22.0f, -10.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(0)], p, Position, true);
				CreateSprite(BITMAP_LIGHT + 1, Position, 0.75f, Light, o);
			}
			break;
		case MODEL_SWORD + 23:
			{
				mu_float fRendomPos = (mu_float)(GetLargeRand(60)) / 20.0f - 1.5f;
				mu_float fRendomScale = (mu_float)(GetLargeRand(30)) / 20.0f + 1.5f;
				mu_float fLight = (mu_float)EMath::Sin((FWorldTime)*0.7f)*0.2f + 0.5f;

				mu_float fRotation = (FWorldTime*0.0006f)*360.0f;
				mu_float fRotation2 = (FWorldTime*0.0006f)*360.0f;

				Vector(0.2f, 0.2f, fLight, Light);

				Vector(0.0f, fRendomPos, fRendomPos, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(4)], p, Position, true);
				CreateSprite(BITMAP_SHINY + 1, Position, fRendomScale, Light, o, fRotation);
				CreateSprite(BITMAP_SHINY + 1, Position, fRendomScale - 0.4f, Light, o, 90.0f + fRotation2);
				Vector(0.0f, 0.0f, 0.0f, p);
				CreateSprite(BITMAP_LIGHT, Position, 2.3f, Light, o);

				Vector(30.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(4)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);

				Vector(0.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(6)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(7)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(8)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 1.0f, Light, o);
			}
			break;
		case MODEL_STAFF + 12:
			{
				Vector(0.4f, 0.4f, 0.4f, Light);
				mu_float fRendomPos = (mu_float)(GetLargeRand(60)) / 20.0f - 1.5f;
				mu_float fRendomScale = (mu_float)(GetLargeRand(15)) / 20.0f + 1.8f;
				Vector(0.0f, -170.0f + fRendomPos, 0.0f + fRendomPos, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(0)], p, Position, true);
				CreateSprite(BITMAP_SPARK + 1, Position, fRendomScale, Light, o);

				VectorCopy(Position, o->EyeLeft2);
				CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, Position, Position, o->Angle, 17, o, 30.0f);

				fRendomPos = (mu_float)(GetLargeRand(60)) / 20.0f - 1.5f;
				fRendomScale = (mu_float)(GetLargeRand(15)) / 20.0f + 1.0f;
				Vector(0.0f, -170.0f + fRendomPos, 0.0f + fRendomPos, p);
				Vector(1.0f, 0.4f, 1.0f, Light);
				CreateSprite(BITMAP_LIGHT, Position, fRendomScale, Light, o);
				CreateSprite(BITMAP_SHINY + 1, Position, fRendomScale, Light, o);
				CreateSprite(BITMAP_SHINY + 1, Position, fRendomScale - 0.3f, Light, o, 90.0f);
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, o->Angle, Light, 11, 2.0f);

				mu_float fLight = (mu_float)EMath::Sin((FWorldTime)*0.7f)*0.2f + 0.5f;
				mu_float fRotation = (FWorldTime*0.0006f)*360.0f;

				Vector(fLight - 0.1f, 0.1f, fLight - 0.1f, Light);
				Vector(0.0f, 10.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], p, Position, true);
				CreateSprite(BITMAP_SHINY + 1, Position, 1.5f, Light, o, fRotation);
				CreateSprite(BITMAP_SHINY + 1, Position, 1.2f, Light, o, 90.0f + fRotation);

				Vector(-40.0f, -10.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(1)], p, Position, true);
				CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, Light, o, fRotation);
				CreateSprite(BITMAP_SHINY + 1, Position, 0.7f, Light, o, 90.0f + fRotation);

				Vector(-160.0f, -5.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(1)], p, Position, true);
				CreateSprite(BITMAP_SHINY + 1, Position, 1.2f, Light, o, fRotation);
				CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, Light, o, 90.0f + fRotation);
			}
			break;
		case MODEL_SWORD + 24:
			{
				Vector(0.6f, 0.6f, 0.6f, Light);

				Vector(0.0f, 0.0f, 3.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], p, Position, true);
				CreateSprite(BITMAP_LIGHT + 1, Position, 0.45f, Light, o);

				Vector(0.0f, 0.0f, -3.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], p, Position, true);
				CreateSprite(BITMAP_LIGHT + 1, Position, 0.45f, Light, o);

				Vector(0.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], p, Position, true);
				CreateSprite(BITMAP_SHINY + 1, Position, 0.7f, Light, o);

				mu_float fScale = 0.5f + (mu_float)(GetLargeRand(100)) / 180;
				mu_float fRotation = (FWorldTime*0.0006f)*360.0f;
				Vector(0.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], p, Position, true);
				CreateSprite(BITMAP_SHINY + 1, Position, fScale, Light, o, fRotation);

				mu_float fLight = (mu_float)EMath::Sin((FWorldTime)*0.4f)*0.25f + 0.6f;
				Vector(fLight, fLight, fLight, Light);
				Vector(0.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 3.0f, Light, o, 0.0f);
			}
			break;
		case MODEL_SWORD + 25:
			{
				mu_float fLight, fScale, fRotation;
				static mu_float fPosition[MAX_TASKING_THREADS] = { 0 };
				static mu_int32 iRandom[MAX_TASKING_THREADS] = { 0 };

				Vector(1.0f, 0.1f, 0.0f, Light);
				Vector(0.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(1)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 1.0f, Light, o);

				fScale = (mu_float)(GetLargeRand(30)) / 60.0f + 0.2f;
				fLight = (mu_float)EMath::Sin((FWorldTime)*0.7f)*0.2f + 0.3f;
				fRotation = (FWorldTime*0.0006f)*360.0f;
				Vector(0.1f + fLight, 0.2f, 0.0f, Light);
				CreateSprite(BITMAP_LIGHT + 3, Position, fScale, Light, o, fRotation);

				Vector(1.0f - fLight, 0.0f, 0.0f, Light);
				if (fPosition[ThreadIndex] >= 20.0f)
				{
					iRandom[ThreadIndex] = GetLargeRand(5) + 2;
					fPosition[ThreadIndex] = 0.0f;
				}
				else
					fPosition[ThreadIndex] += 1.5f;
				Vector(0.0f, fPosition[ThreadIndex], 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(iRandom[ThreadIndex])], p, Position, true);
				CreateSprite(BITMAP_WATERFALL_4, Position, 0.7f, Light, o);
				Vector(0.1f, 0.1f, 0.1f, Light);
				CreateSprite(BITMAP_WATERFALL_2, Position, 0.3f, Light, o);

				fLight = (mu_float)EMath::Sin((FWorldTime)*0.4f)*0.25f + 0.2f;
				Vector(0.8f + fLight, 0.1f, 0.0f, Light);
				Vector(0.0f, 0.0f, 0.0f, p);
				for (mu_int32 i = 0; i < 5; ++i)
				{
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2 + i)], p, Position, true);
					CreateSprite(BITMAP_LIGHT, Position, 1.1f, Light, o);
				}

				if (o->CurrentAction == PLAYER_RUN_TWO_HAND_SWORD_TWO && GET_WORLD != WD_10HEAVEN && GetLargeRand(2) == 0)
				{
					if (!g_Direction.m_CKanturu.IsMayaScene())
					{
						Vector(1.0f, 1.0f, 1.0f, Light);
						Vector(0.0f, 0.0f, 0.0f, p);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(6)], p, Position, true);
						Position[0] += GetLargeRand(30) - 15.0f;
						Position[1] += GetLargeRand(30) - 15.0f;
						Position[2] += 20.0f;

						EVector3	Angle;
						for (mu_int32 i = 0; i < 4; ++i)
						{
							Vector((mu_float)(GetLargeRand(60) + 60 + 90), 0.0f, o->Angle[2], Angle);
							CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, Position, Position, Angle);
							CreateParticle(ThreadIndex, BITMAP_SPARK, Position, Angle, Light);
						}
					}
				}
			}
			break;
		case MODEL_MACE + 15:
			{
				mu_float fScale;
				Luminosity = (mu_float)EMath::Sin((FWorldTime)*0.002f)*0.3f + 0.1f;
				fScale = (mu_float)(GetLargeRand(10)) / 30.0f + 1.7f;

				Vector(0.0f, 0.0f, 0.0f, p);

				Vector(0.8f + Luminosity, 0.5f + Luminosity, 0.1f + Luminosity, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(1)], p, Position, true);
				CreateSprite(BITMAP_SHINY + 1, Position, fScale, Light, o);

				Vector(0.7f, 0.5f, 0.3f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(1)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 3.0f, Light, o);

				Vector(0.8f, 0.6f, 0.4f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 1.5f, Light, o);

				Vector(0.8f, 0.6f, 0.4f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(3)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 1.2f, Light, o);
			}
			break;
		case MODEL_BOW + 22:
			{
				mu_float fLight;
				fLight = (mu_float)EMath::Sin((FWorldTime)*0.4f)*0.25f;
				Vector(0.0f, 0.7f, 0.0f, Light);
				Vector(0.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(10)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 1.2f, Light, o);

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(28)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 1.2f, Light, o);

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(34)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 1.2f, Light, o);

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(16)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 1.2f, Light, o);

				Vector(0.0f, 0.0f, 0.0f, p);
				Vector(0.3f, 0.9f, 0.2f, Light);
				if (GetLargeRand(2) == 1)
				{
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(10)], p, Position, true);
					CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, o->Angle, Light, 14, 0.05f);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(28)], p, Position, true);
					CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, o->Angle, Light, 14, 0.05f);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(34)], p, Position, true);
					CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, o->Angle, Light, 14, 0.05f);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(16)], p, Position, true);
					CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, o->Angle, Light, 14, 0.05f);
				}
			}
			break;
		case MODEL_STAFF + 13:
			{
				mu_float fLight, fScale, fRotation;
				fLight = (mu_float)EMath::Sin((FWorldTime)*0.7f)*0.2f + 0.3f;
				Vector(0.0f, 0.0f, 0.0f, p);

				Vector(0.7f, 0.7f, 0.7f, Light);
				fScale = (mu_float)(GetLargeRand(10)) / 500.0f;
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(9)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 0.3f + fScale, Light, o);

				Vector(0.2f, 0.6f + fLight, 0.2f, Light);
				fScale = (mu_float)(GetLargeRand(10)) / 500.0f;
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(9)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 4.0f + fScale, Light, o);

				Vector(0.4f, 0.5f + fLight, 0.4f, Light);
				fScale = (mu_float)(GetLargeRand(30)) / 60.0f;
				fRotation = (FWorldTime*0.0004f)*360.0f;
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(9)], p, Position, true);
				CreateSprite(BITMAP_SHINY + 1, Position, 1.2f + fScale, Light, o, -fRotation);

				Vector(0.6f, 1.0f, 0.6f, Light);
				fScale = (mu_float)(GetLargeRand(10)) / 500.0f;
				fRotation = (FWorldTime*0.0006f)*360.0f;
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(9)], p, Position, true);
				CreateSprite(BITMAP_MAGIC, Position, 0.25f + fScale, Light, o, fRotation);

				Vector(0.0f, 0.0f, 0.0f, p);
				for (mu_int32 i = 0; i < 4; ++i)
				{
					Vector(0.1f, 0.8f, 0.1f, Light);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(5 + i)], p, Position, true);
					CreateSprite(BITMAP_LIGHT, Position, 0.2f, Light, o);
					Vector(0.5f, 0.5f, 0.5f, Light);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(10 + i)], p, Position, true);
					CreateSprite(BITMAP_LIGHT, Position, 0.3f, Light, o);
				}

				Vector(0.1f, 0.8f, 0.1f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(4)], p, Position, true);
				CreateSprite(BITMAP_SHINY + 1, Position, 0.6f + fScale, Light, o, fRotation);

				Vector(0.6f, 1.0f, 0.6f, Light);
				fScale = (mu_float)(GetLargeRand(10)) / 500.0f;
				fRotation = (FWorldTime*0.0006f)*360.0f;
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], p, Position, true);
				CreateSprite(BITMAP_MAGIC, Position, 0.15f + fScale, Light, o, fRotation);
				Vector(0.8f, 0.8f, 0.8f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 0.7f + fScale, Light, o, -fRotation);
				Vector(0.1f, 1.0f, 0.1f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], p, Position, true);
				CreateSprite(BITMAP_SHINY + 1, Position, 0.5f + fScale, Light, o, fRotation);

				Vector(0.2f, 0.6f + fLight, 0.2f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 2.0f + fScale, Light, o);
			}
			break;
		case MODEL_STAFF + 11:
			{
				Luminosity = (mu_float)EMath::Sin((FWorldTime)*0.002f)*0.3f + 0.7f;

				Vector(0.0f, 0.0f, 0.0f, p);
				Vector(Luminosity*0.0f, Luminosity*0.5f, Luminosity*1.0f, Light);

				mu_float Rotation = (mu_float)(GetLargeRand(360));
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(5)], p, Position, true);
				Vector(Luminosity*0.0f, Luminosity*0.5f, Luminosity*1.0f, Light);
				CreateSprite(BITMAP_LIGHT, Position, 1.0f, Light, o);
				Vector(0.5f, 0.5f, 0.5f, Light);
				CreateSprite(BITMAP_SHINY + 1, Position, 0.5f, Light, o, Rotation);
				CreateSprite(BITMAP_SHINY + 1, Position, 0.5f, Light, o, 360.0f - Rotation);

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(6)], p, Position, true);
				Vector(Luminosity*0.0f, Luminosity*0.5f, Luminosity*1.0f, Light);
				CreateSprite(BITMAP_LIGHT, Position, 1.0f, Light, o);
				Vector(0.5f, 0.5f, 0.5f, Light);
				CreateSprite(BITMAP_SHINY + 1, Position, 0.5f, Light, o, Rotation);
				CreateSprite(BITMAP_SHINY + 1, Position, 0.5f, Light, o, 360.0f - Rotation);

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(8)], p, Position, true);
				Vector(Luminosity*0.0f, Luminosity*0.5f, Luminosity*1.0f, Light);
				CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);
				Vector(0.5f, 0.5f, 0.5f, Light);
				CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, Light, o, Rotation);
				CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, Light, o, 360.0f - Rotation);
			}
			break;
		case MODEL_MACE + 12:
			{
				mu_float Rotation = (mu_float)(GetLargeRand(360));
				Luminosity = (mu_float)EMath::Sin((FWorldTime)*0.002f)*0.3f + 0.7f;

				Vector(0.0f, 0.0f, 0.0f, p);

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(1)], p, Position, true);
				Vector(Luminosity*0.0f, Luminosity*0.5f, Luminosity*1.0f, Light);
				CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);
				Vector(0.5f, 0.5f, 0.5f, Light);
				CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, Light, o, Rotation);
				CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, Light, o, 360.0f - Rotation);
			}
			break;
		case MODEL_SWORD + 19:
			Vector(Luminosity*1.0f, Luminosity*0.3f, Luminosity*0.1f, Light);

			Vector(0.0f, 0.0f, 0.0f, p);
			for (mu_int32 i = 0; i < 7; ++i)
			{
				EVector3 Light2;
				Vector(0.4f, 0.4f, 0.4f, Light2);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(i + 2)], p, Position, true);

				if ((GetLargeRand(3)) == 0)
				{
					CreateSprite(BITMAP_SHINY + 1, Position, 0.6f, Light2, o, (mu_float)(GetLargeRand(360)));
				}
				CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);
			}

			if (((o->CurrentAction<PLAYER_WALK_MALE || o->CurrentAction>PLAYER_RUN_RIDE_WEAPON) &&
				(o->CurrentAction<PLAYER_ATTACK_SKILL_SWORD1 || o->CurrentAction>PLAYER_ATTACK_SKILL_SWORD5)
				))
			{
				EVector3 pos, delta, angle;

				Vector(0.0f, 0.0f, 0.0f, p);
				Vector(-90.0f, (mu_float)(GetLargeRand(360)), o->Angle[2] - 45, angle);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(3)], p, pos, true);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], p, Position, true);

				VectorSubtract(pos, Position, delta);
			}
			break;
		case MODEL_BOW + 18:
			Vector(Luminosity*1.0f, Luminosity*0.3f, Luminosity*0.1f, Light);
			break;
		case MODEL_BOW + 19:
			Vector(0.0f, 0.0f, 10.0f, p);
			for (mu_int32 i = 1; i < 6; ++i)
			{
				Vector(Luminosity*0.5f, Luminosity*0.5f, Luminosity*0.8f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(i)], p, Position, true);

				CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, Light, o);

				if (i == 5)
				{
					Vector(1.0f, 1.0f, 1.0f, Light);
					CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);
				}
				else
					CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);
			}
			break;
		case MODEL_SHIELD + 15:
			Vector(Luminosity*0.6f, Luminosity*0.6f, Luminosity*2.0f, Light);

			Vector(15.0f, -15.0f, 0.0f, p);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(1)], p, Position, true);
			CreateSprite(BITMAP_SHINY + 1, Position, 1.5f, Light, o);
			CreateSprite(BITMAP_LIGHT, Position, Luminosity + 1.5f, Light, o);
			break;
		case MODEL_MACE + 7:
			Vector(Luminosity*1.0f, Luminosity*0.9f, Luminosity*0.0f, Light);

			Vector(0.0f, 0.0f, 0.0f, p);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(1)], p, Position, true);
			CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);

			Vector(0.5f, 0.5f, 0.5f, Light);
			CreateSprite(BITMAP_LIGHT, Position, EMath::Sin(FWorldTime*0.002f) + 0.5f, Light, o);
			break;
		case MODEL_MACE + 8:
			{
				mu_float Scale = EMath::Sin(FWorldTime*0.001f) + 1.0f;
				Vector(Luminosity*0.2f, Luminosity*0.1f, Luminosity*3.0f, Light);
				Vector(-15.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(1)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, Scale, Light, o);
				Vector(10.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(1)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, Scale, Light, o);

				Scale = EMath::Sin(FWorldTime*0.01f) * 360;
				Luminosity = EMath::Sin(FWorldTime*0.001f)*0.5f + 0.5f;
				Vector(Luminosity, Luminosity, Luminosity, Light);
				CreateSprite(BITMAP_SHINY + 1, Position, 0.6f, Light, o, 360 - Scale);
				CreateSprite(BITMAP_SHINY + 1, Position, 0.6f, Light, o, Scale);
			}
			break;
		case MODEL_MACE + 9:
			{
				for (mu_int32 i = 1; i < 5; ++i)
				{
					Luminosity = (mu_float)EMath::Sin((FWorldTime)*0.002f)*0.35f + 0.65f;
					Vector(Luminosity*0.6f, Luminosity*0.8f, Luminosity*1.0f, Light);
					Vector(-10.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(i + 1)], p, Position, true);
					CreateSprite(BITMAP_LIGHT, Position, /*Luminosity**/1.0f, Light, o);

					if (i == 3)
					{
						Vector(0.5f, 0.5f, 0.5f, Light);
						CreateSprite(BITMAP_SHINY + 1, Position, 0.6f, Light, o, GetLargeRand(360));
					}
				}
			}
			break;
		case MODEL_MACE + 10:
			{
				Luminosity = EMath::Sin(FWorldTime*0.001f)*0.5f + 0.7f;
				Vector(Luminosity*1.0f, Luminosity*0.8f, Luminosity*0.6f, Light);
				Vector(0.0f, 0.0f, 0.0f, p);

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 1.5f, Light, o);
				CreateSprite(BITMAP_SHINY + 1, Position, Luminosity*0.5f, Light, o);

				Vector(Luminosity*-10.0f, 0.0f, 0.0f, p);
				Vector(0.6f, 0.8f, 1.0f, Light);
				Luminosity = GetLargeRand(360);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(1)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 0.7f, Light, o);
				CreateSprite(BITMAP_SHINY + 1, Position, 0.5f, Light, o, Luminosity);
				CreateSprite(BITMAP_SHINY + 1, Position, 0.4f, Light, o, 360 - Luminosity);
			}
			break;
		case MODEL_MACE + 11:
			{
				Vector(1.0f, 0.6f, 0.3f, Light);
				Vector(0.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(1)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 1.3f, Light, o);

				for (mu_int32 i = 0; i < 3; ++i)
				{
					Vector(i*15.0f - 10.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], p, Position, true);
					CreateSprite(BITMAP_LIGHT, Position, 1.3f, Light, o);
					CreateSprite(BITMAP_SHINY + 1, Position, 0.4f, Light, o, GetLargeRand(360));
				}
			}
			break;
		case MODEL_MACE + 13:
			{
				Vector(Luminosity*1.0f, Luminosity*0.3f, Luminosity*0.1f, Light);
				Vector(0.0f, 0.0f, 0.0f, p);
				EVector3 Light2;
				Vector(0.4f, 0.4f, 0.4f, Light2);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(0)], p, Position, true);
				if ((GetLargeRand(3)) == 0)
				{
					CreateSprite(BITMAP_SHINY + 1, Position, 0.6f, Light2, o, (mu_float)(GetLargeRand(360)));
				}
				CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);
			}
			break;
		case MODEL_SWORD + 20:
			{
				for (mu_int32 i = 0; i < 2; ++i) {
					Vector(0.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(i + 1)], p, Position, true);
					Luminosity = (mu_float)EMath::Sin((FWorldTime)*0.002f)*0.35f + 0.65f;
					Vector(Luminosity*0.43f, Luminosity*0.14f, Luminosity*0.6f, Light);
					CreateSprite(BITMAP_LIGHT, Position, Luminosity*0.9f, Light, o);
					Vector(0.3f, 0.3f, 0.3f, Light);
					CreateSprite(BITMAP_LIGHT, Position, mu_float(EMath::Sin(FWorldTime*0.002f)*0.5f) + 0.4f, Light, o);
				}
			}
			break;
		case MODEL_BOW + 20:
			{
				mu_float Scale = EMath::Sin(FWorldTime*0.001f) + 1.0f;

				Vector(Luminosity*3.0f, Luminosity, Luminosity, Light);
				Vector(0.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(6)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, Scale*0.8f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, Scale*0.8f, Light, o);


				mu_float Rotation = EMath::Sin(FWorldTime*0.01f) * 360;
				Luminosity = EMath::Sin(FWorldTime*0.001f)*0.3 + 0.3f;
				Vector(Luminosity, Luminosity, Luminosity, Light);
				if (!c->SafeZone)
				{
					Vector(10.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(9)], p, Position, true);
					CreateSprite(BITMAP_LIGHT, Position, Scale*0.8f, Light, o);
					Vector(Luminosity*3.0f, Luminosity, Luminosity, Light);
					CreateSprite(BITMAP_SHINY + 1, Position, 0.8f, Light, o, 360 - Rotation);
					CreateSprite(BITMAP_SHINY + 1, Position, 0.8f, Light, o, Rotation);
				}
			}
			break;
		case MODEL_SWORD + 21:
		case MODEL_SWORD + 31:
			if (o->CurrentAction == PLAYER_RUN_TWO_HAND_SWORD_TWO && GET_WORLD != WD_10HEAVEN && GetLargeRand(2) == 0)
			{
				if (!g_Direction.m_CKanturu.IsMayaScene())
				{
					Vector(1.0f, 1.0f, 1.0f, Light);
					Vector(0.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(1)], p, Position, true);
					Position[0] += GetLargeRand(30) - 15.0f;
					Position[1] += GetLargeRand(30) - 15.0f;
					Position[2] += 20.0f;

					EVector3	Angle;
					for (mu_int32 i = 0; i < 4; ++i)
					{
						Vector((mu_float)(GetLargeRand(60) + 60 + 90), 0.0f, o->Angle[2], Angle);
						CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, Position, Position, Angle);
						CreateParticle(ThreadIndex, BITMAP_SPARK, Position, Angle, Light);
					}
				}
			}
			break;
		case MODEL_SPEAR + 10:
			Vector(Luminosity*0.2f, Luminosity*0.1f, Luminosity*0.8f, Light);
			Vector(0.0f, 0.0f, 0.0f, p);
			for (mu_int32 i = 1; i < 9; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(i)], p, Position, true);
				CreateSprite(BITMAP_LIGHT, Position, 1.3f, Light, o);
			}
			break;
		case MODEL_SWORD + 26:
			{
				Vector(0.8f, 0.6f, 0.2f, Light);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 11);
				CreateSprite(BITMAP_LIGHT, Position, 0.6f, Light, o);
				CreateSprite(BITMAP_LIGHT, Position, 0.6f, Light, o);

				Vector(0.3f, 0.8f, 0.7f, Light);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 12);
				CreateSprite(BITMAP_LIGHT, Position, 0.5f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 13);
				CreateSprite(BITMAP_LIGHT, Position, 0.5f, Light, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 12);
				CreateSprite(BITMAP_LIGHT, Position, 0.3f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 13);
				CreateSprite(BITMAP_LIGHT, Position, 0.3f, Light, o);

				Vector(0.9f, 0.1f, 0.1f, Light);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 1);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.8f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 2);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.7f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 3);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.7f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 4);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.7f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 5);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.6f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 6);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.5f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 7);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.5f, Light, o);

			}break;
		case MODEL_SWORD + 27:
			{
				Vector(0.1f, 0.9f, 0.1f, Light);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 1);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.6f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 2);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.5f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 3);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.5f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 4);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.5f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 5);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.4f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 6);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.3f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 7);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.3f, Light, o);

				for (mu_int32 i = 1; i <= 7; ++i)
				{
					if (GetLargeRand(4) != 0)
					{
						continue;
					}
					Vector(0.0f, 0.0f, 0.0f, Position);
					b->TransformByObjectBone(ThreadIndex, Position, Object, i);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_4, Position, Object->Angle, Light, 12, 0.5f, Object);
				}
			}break;
		case MODEL_SWORD + 28:
			{
				mu_float fRendomScale = (mu_float)((GetLargeRand(15)) / 30.0f) + 0.5f;
				Vector(0.0f, 0.0f, 0.0f, Position);
				Vector(0.1f, 0.4f, 0.9f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(8)], Position, p, true);
				CreateSprite(BITMAP_FLARE_BLUE, p, 0.4f, o->Light, o);
				CreateSprite(BITMAP_SHINY + 6, p, fRendomScale, Light, o);

				EVector3 vColor;
				VectorCopy(p, o->EyeLeft2);
				Vector(0.0f, 0.0f, 0.9f, vColor);
				CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, p, p, o->Angle, 17, o, 25.0f);

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(9)], Position, p, true);
				CreateSprite(BITMAP_FLARE_BLUE, p, 0.4f, o->Light, o);
				CreateSprite(BITMAP_SHINY + 6, p, fRendomScale, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(10)], Position, p, true);
				CreateSprite(BITMAP_FLARE_BLUE, p, 0.4f, o->Light, o);
				CreateSprite(BITMAP_SHINY + 6, p, 0.4f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(11)], Position, p, true);
				CreateSprite(BITMAP_FLARE_BLUE, p, 0.4f, o->Light, o);
				CreateSprite(BITMAP_SHINY + 6, p, 0.4f, Light, o);

				Vector(0.0f, 0.3f, 0.7f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], Position, p, true);
				CreateSprite(BITMAP_LIGHTMARKS, p, 1.0f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(3)], Position, p, true);
				CreateSprite(BITMAP_LIGHTMARKS, p, 0.8f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(4)], Position, p, true);
				CreateSprite(BITMAP_LIGHTMARKS, p, 0.6f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(5)], Position, p, true);
				CreateSprite(BITMAP_LIGHTMARKS, p, 0.4f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(6)], Position, p, true);
				CreateSprite(BITMAP_LIGHTMARKS, p, 0.2f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(7)], Position, p, true);
				CreateSprite(BITMAP_LIGHTMARKS, p, 0.1f, Light, o);
			}break;
		case MODEL_MACE + 16:
			{
				EVector3 vDPos;
				Vector(0.5f, 0.8f, 0.5f, Light);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 11);
				CreateSprite(BITMAP_SHINY + 6, Position, 0.2f, Light, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 10);
				CreateSprite(BITMAP_SHINY + 6, Position, 0.2f, Light, o);

				Vector(0.9f, 0.1f, 0.3f, Light);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 9);
				CreateSprite(BITMAP_SHINY + 6, Position, 0.2f, Light, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 8);
				CreateSprite(BITMAP_SHINY + 6, Position, 0.2f, Light, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 1);
				CreateSprite(BITMAP_SHINY + 6, Position, 0.2f, Light, o);
				CreateSprite(BITMAP_LIGHT, Position, 0.8f, Light, o);

				Vector(0.5f, 0.8f, 0.6f, Light);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 24);
				CreateSprite(BITMAP_SHINY + 6, Position, 0.3f, Light, o);
				CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);
				CreateSprite(BITMAP_PIN_LIGHT, Position, 0.5f, Light, o, ((mu_int32)(FWorldTime*0.04f) % 360));
				CreateSprite(BITMAP_PIN_LIGHT, Position, 0.7f, Light, o, -((mu_int32)(FWorldTime*0.03f) % 360));
				CreateSprite(BITMAP_PIN_LIGHT, Position, 0.9f, Light, o, ((mu_int32)(FWorldTime*0.02f) % 360));

				if (GetLargeRand(3) != 0)
				{
					mu_float fTemp = Position[2];
					Position[2] -= 15.0f;
					Vector(0.5f, 0.8f, 0.8f, Light);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, Position, o->Angle, Light, 9, 0.4f);
					Position[2] = fTemp;
				}


				Vector(0.5f, 0.8f, 0.6f, Light);
				vDPos[0] = Position[0] + ((mu_float)(GetLargeRand(20) - 10)*3.0f);
				vDPos[1] = Position[1] + ((mu_float)(GetLargeRand(20) - 10)*3.0f);
				vDPos[2] = Position[2] + ((mu_float)(GetLargeRand(20) - 10)*3.0f);

				if (GetLargeRand(10) == 0)
				{
					CreateEffect(ThreadIndex, MODEL_STAR_SHINE, vDPos, o->Angle, Light, 0, Object, -1, 0, 0, 0, 0.22f);
				}

				b->TransformByObjectBone(ThreadIndex, Position, Object, 12);
				CreateSprite(BITMAP_LIGHT, Position, 0.2f, Light, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 13);
				CreateSprite(BITMAP_LIGHT, Position, 0.2f, Light, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 14);
				CreateSprite(BITMAP_LIGHT, Position, 0.2f, Light, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 15);
				CreateSprite(BITMAP_LIGHT, Position, 0.2f, Light, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 16);
				CreateSprite(BITMAP_LIGHT, Position, 0.3f, Light, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 17);
				CreateSprite(BITMAP_LIGHT, Position, 0.3f, Light, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 18);
				CreateSprite(BITMAP_LIGHT, Position, 0.3f, Light, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 19);
				CreateSprite(BITMAP_LIGHT, Position, 0.3f, Light, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 20);
				CreateSprite(BITMAP_LIGHT, Position, 0.3f, Light, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 21);
				CreateSprite(BITMAP_LIGHT, Position, 0.4f, Light, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 22);
				CreateSprite(BITMAP_LIGHT, Position, 0.4f, Light, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 23);
				CreateSprite(BITMAP_LIGHT, Position, 0.4f, Light, o);
			}break;
		case MODEL_MACE + 17:
			{
				mu_float fRandomScale;
				EVector3 vPosZx01, vPosZx02, vLight1, vLight2, vDLight;

				mu_float fLumi = EMath::Abs((EMath::Sin(FWorldTime*0.0008f)))*0.8 + 0.2f;
				Vector(fLumi*0.6f, fLumi*0.5f, fLumi*0.8f, vDLight);

				Vector(0.6f, 0.5f, 0.8f, vLight1);
				Vector(0.8f, 0.8f, 0.8f, vLight2);
				b->TransformByObjectBone(ThreadIndex, vPosZx01, Object, 3);
				b->TransformByObjectBone(ThreadIndex, vPosZx02, Object, 4);

				if ((IWorldTime / 100) % 10 == 0)
				{
					Object->m_iAnimation = GetLargeRand(100);
					Object->EyeRight[0] = (GetLargeRand(10) - 5);
					Object->EyeRight[1] = (GetLargeRand(10) - 5);
					Object->EyeRight[2] = (GetLargeRand(10) - 5);
					Object->EyeRight2[0] = (GetLargeRand(10) - 5)*1.2f;
					Object->EyeRight2[1] = (GetLargeRand(10) - 5)*1.2f;
					Object->EyeRight2[2] = (GetLargeRand(10) - 5)*1.2f;
				}

				mu_int32 iRandomTexure1, iRandomTexure2;
				iRandomTexure1 = (Object->m_iAnimation / 10) % 3;
				iRandomTexure2 = (Object->m_iAnimation) % 3;

				fRandomScale = (mu_float)(GetLargeRand(10)) / 10.0f + 1.0f;
				CreateSprite(BITMAP_LIGHT, vPosZx01, fRandomScale, vLight1, o);
				CreateSprite(BITMAP_SHINY + 1, vPosZx01, 0.5f, vDLight, o);
				VectorAdd(vPosZx01, Object->EyeRight, vPosZx01);
				CreateSprite(BITMAP_LIGHTNING_MEGA1 + iRandomTexure1, vPosZx01, (((GetLargeRand(11)) - 20) / 100.0f) + 0.5f, vLight2, o, GetLargeRand(380));

				fRandomScale = (mu_float)((GetLargeRand(10)) / 5.0f) + 1.5f;
				CreateSprite(BITMAP_LIGHT, vPosZx02, fRandomScale, vLight1, o);
				CreateSprite(BITMAP_SHINY + 1, vPosZx02, 1.0f, vDLight, o);
				VectorAdd(vPosZx02, Object->EyeRight2, vPosZx02);
				CreateSprite(BITMAP_LIGHTNING_MEGA1 + iRandomTexure1, vPosZx02, (((GetLargeRand(11)) - 20) / 50.0f) + 0.8f, vLight2, o, GetLargeRand(380));
			}break;
		case MODEL_BOW + 23:
			{
				EVector3 vZX03(0.0f), vZx04(0.0f);
				mu_int32 iNumCreateFeather = GetLargeRand(3);

				Vector(0.2f, 0.25f, 0.3f, Light);

				for (mu_int32 i = 0; i <= 43; ++i)
				{
					if (i == 1)
					{
						continue;
					}
					b->TransformByObjectBone(ThreadIndex, Position, Object, i);
					CreateSprite(BITMAP_LIGHT, Position, 0.8f, Light, Object);
				}

				if (o->AnimationFrame >= 4.5f && o->AnimationFrame <= 5.0f)
				{
					for (mu_int32 i = 0; i < iNumCreateFeather; ++i)
					{
						CreateEffect(ThreadIndex, MODEL_FEATHER, vZX03, o->Angle, Light, 0, nullptr, -1, 0, 0, 0, 0.6f);
						CreateEffect(ThreadIndex, MODEL_FEATHER, vZX03, o->Angle, Light, 1, nullptr, -1, 0, 0, 0, 0.6f);
						CreateEffect(ThreadIndex, MODEL_FEATHER, vZx04, o->Angle, Light, 0, nullptr, -1, 0, 0, 0, 0.6f);
						CreateEffect(ThreadIndex, MODEL_FEATHER, vZx04, o->Angle, Light, 1, nullptr, -1, 0, 0, 0, 0.6f);
					}
				}


			}break;
		case MODEL_STAFF + 30:
			{
				Vector(0.0f, 0.0f, 0.0f, Position);
				Vector(0.8f, 0.3f, 0.1f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(4)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 0.8f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(5)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 0.4f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(6)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 0.8f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(7)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 0.8f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(8)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 2.0f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(9)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 0.8f, Light, o);

				mu_float fLumi = EMath::Abs((EMath::Sin(FWorldTime*0.001f)))*0.8f + 0.2f;
				EVector3 vDLight;
				Vector(fLumi*0.8f, fLumi*0.1f, fLumi*0.3f, vDLight);
				mu_float fRendomScale = (mu_float)(GetLargeRand(10)) / 20.0f + 0.8f;
				Vector(0.8f, 0.2f, 0.4f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 2.0f, vDLight, o);
				CreateSprite(BITMAP_SHINY + 6, p, fRendomScale, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(3)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 2.0f, vDLight, o);
				CreateSprite(BITMAP_SHINY + 6, p, fRendomScale, Light, o);

				EVector3 vColor;
				VectorCopy(p, o->EyeRight);
				Vector(0.9f, 0.0f, 0.0f, vColor);
				CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, p, p, o->Angle, 47, o, 25.0f);
			}break;
		case MODEL_STAFF + 31:
			{
				Vector(0.0f, 0.0f, 0.0f, Position);
				Vector(0.3f, 0.3f, 0.9f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 2.0f, Light, o);
				CreateSprite(BITMAP_LIGHT, p, 2.0f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(3)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 2.5f, Light, o);
				CreateSprite(BITMAP_LIGHT, p, 2.5f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(4)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 3.0f, Light, o);
				Vector(0.7f, 0.1f, 0.2f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(5)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 2.0f, Light, o);
				Vector(0.9f, 0.3f, 0.5f, Light);
				CreateSprite(BITMAP_SHINY + 6, p, 0.8f, Light, o);

				mu_float fRendomScale = (mu_float)(GetLargeRand(15)) / 20.0f + 1.0f;
				CreateSprite(BITMAP_SHINY + 1, p, fRendomScale, Light, o);
				CreateSprite(BITMAP_SHINY + 1, p, fRendomScale - 0.3f, Light, o, 90.0f);
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, p, o->Angle, Light, 11, 2.0f);

				EVector3 vColor;
				VectorCopy(p, o->EyeLeft2);
				Vector(0.0f, 0.0f, 0.9f, vColor);
				CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, p, p, o->Angle, 17, o, 30.0f);

				Vector(0.7f, 0.7f, 0.7f, Light);
				CreateSprite(BITMAP_SHINY + 2, p, 2.0f, Light, o);
			}break;
		case MODEL_STAFF + 32:
			{
				mu_float fRandomScale;
				EVector3 vLight1, vLight2;
				Vector(0.9f, 0.7f, 0.4f, vLight1);
				Vector(0.9f, 0.1f, 0.3f, vLight2);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 1);
				Vector(1.0f, 0.1f, 0.2f, Light);
				CreateSprite(BITMAP_SHINY + 6, Position, 0.7f, Light, o);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.6f, Light, o);
				CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);

				Vector(0.7f, 0.1f, 0.6f, Light);
				CreateSprite(BITMAP_SHOCK_WAVE, Position, 0.65f, Light, o, -((mu_int32)(FWorldTime*0.05f) % 360));

				Object->Timer += 0.01f;
				if (Object->Timer <= 0.1f || Object->Timer > 0.9f)
				{
					Object->Timer = 0.15f;
					Vector(0.7f, 0.1f, 0.6f, Object->EyeRight);
				}
				if (Object->Timer > 0.5f)
				{
					Object->EyeRight[0] *= 0.95f;
					Object->EyeRight[1] *= 0.95f;
					Object->EyeRight[2] *= 0.95f;
				}

				CreateSprite(BITMAP_SHOCK_WAVE, Position, Object->Timer, Object->EyeRight, o);

				Vector(0.9f, 0.5f, 0.2f, Light);
				fRandomScale = (mu_float)(GetLargeRand(5)) / 25.0f + 0.3f;
				CreateSprite(BITMAP_LIGHTMARKS, Position, fRandomScale, Light, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 2);
				CreateSprite(BITMAP_SHINY + 6, Position, 0.3f, vLight1, o);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.4f, vLight2, o);

				b->TransformByObjectBone(ThreadIndex, Position, Object, 3);
				CreateSprite(BITMAP_SHINY + 6, Position, 0.2f, vLight1, o);
				CreateSprite(BITMAP_LIGHTMARKS, Position, 0.3f, vLight2, o);
			}break;
		case MODEL_SHIELD + 17:
			{
				EVector3 vDLight;
				Vector(0.8f, 0.6f, 0.2f, Light);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 4);
				CreateSprite(BITMAP_LIGHT, Position, 1.5f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 5);
				CreateSprite(BITMAP_LIGHT, Position, 1.5f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 6);
				CreateSprite(BITMAP_LIGHT, Position, 1.5f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 7);
				CreateSprite(BITMAP_LIGHT, Position, 1.5f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 8);
				CreateSprite(BITMAP_LIGHT, Position, 1.5f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 9);
				CreateSprite(BITMAP_LIGHT, Position, 1.5f, Light, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 10);
				CreateSprite(BITMAP_LIGHT, Position, 1.5f, Light, o);

				mu_float fLumi = EMath::Abs((EMath::Sin(FWorldTime*0.0005f)));
				Vector(fLumi*1.0f, fLumi*1.0f, fLumi*1.0f, vDLight);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 1);
				CreateSprite(BITMAP_FLARE_RED, Position, 0.5f, vDLight, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 2);
				CreateSprite(BITMAP_FLARE_RED, Position, 0.3f, vDLight, o);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 3);
				CreateSprite(BITMAP_FLARE_RED, Position, 0.3f, vDLight, o);


			}break;
		case MODEL_SHIELD + 18:
			{
				Vector(0.9f, 0.0f, 0.2f, Light);
				b->TransformByObjectBone(ThreadIndex, Position, Object, 1);
				CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);

				Vector(1.0f, 1.0f, 1.0f, Light);

				switch (GetLargeRand(3))
				{
				case 0:
					CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1, Position, Object->Angle, Light, 0, 0.7f);
					break;
				case 1:
					CreateParticle(ThreadIndex, BITMAP_FIRE_CURSEDLICH, Position, Object->Angle, Light, 4, 0.7f);
					break;
				case 2:
					CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3, Position, Object->Angle, Light, 0, 0.7);
					break;
				}
			}break;
		case MODEL_SHIELD + 19:
			{
			}break;
		case MODEL_SHIELD + 20:
			{
				Vector(0.0f, 0.0f, 0.0f, Position);
				mu_float fLumi = EMath::Abs(EMath::Sin(FWorldTime*0.001f)) + 0.1f;
				Vector(0.8f*fLumi, 0.3f*fLumi, 0.8f*fLumi, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(1)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 1.5f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(2)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 1.5f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(3)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 1.5f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(4)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 1.5f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(5)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 1.5f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(6)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 1.5f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(7)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 1.5f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(8)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 1.5f, Light, o);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[bOwner->GetBoneIndex(9)], Position, p, true);
				CreateSprite(BITMAP_LIGHT, p, 1.5f, Light, o);
			}break;
		case MODEL_SHIELD + 21:
			{
				EVector3 vPos, vLight;
				Vector(0.0f, 0.0f, 0.0f, vPos);
				mu_float fLumi = EMath::Abs(EMath::Sin(FWorldTime*0.001f)) + 0.1f;
				Vector(0.2f*fLumi, 0.2f*fLumi, 0.8f*fLumi, vLight);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 4);
				CreateSprite(BITMAP_LIGHT, vPos, 1.2f, vLight, Object);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 5);
				CreateSprite(BITMAP_LIGHT, vPos, 1.2f, vLight, Object);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 6);
				CreateSprite(BITMAP_LIGHT, vPos, 1.2f, vLight, Object);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 7);
				CreateSprite(BITMAP_LIGHT, vPos, 1.2f, vLight, Object);

				Vector(0.2f*fLumi, 0.6f*fLumi, 0.6f*fLumi, vLight);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 8);
				CreateSprite(BITMAP_LIGHT, vPos, 1.4f, vLight, Object);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 9);
				CreateSprite(BITMAP_LIGHT, vPos, 1.4f, vLight, Object);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 10);
				CreateSprite(BITMAP_LIGHT, vPos, 1.4f, vLight, Object);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 11);
				CreateSprite(BITMAP_LIGHT, vPos, 1.4f, vLight, Object);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 12);
				CreateSprite(BITMAP_LIGHT, vPos, 1.4f, vLight, Object);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 13);
				CreateSprite(BITMAP_LIGHT, vPos, 1.4f, vLight, Object);
			}
			break;
		case MODEL_STAFF + 33:
			{
				EVector3 vPos, vLight;

				b->TransformByObjectBone(ThreadIndex, vPos, Object, 8);
				Vector(0.9f, 0.1f, 0.4f, vLight);
				CreateSprite(BITMAP_LIGHT, vPos, 3.5f, vLight, Object);
				vLight[0] = 0.1f + 0.8f * EMath::Abs(EMath::Sin(FWorldTime*0.0008f));
				vLight[1] = 0.1f * EMath::Abs(EMath::Sin(FWorldTime*0.0008f));
				vLight[2] = 0.1f + 0.4f * EMath::Abs(EMath::Sin(FWorldTime*0.0008f));
				CreateSprite(BITMAP_MAGIC, vPos, 0.3f, vLight, Object);
				CreateEffect(ThreadIndex, MODEL_MOONHARVEST_MOON, vPos, o->Angle, vLight, 2, nullptr, -1, 0, 0, 0, 0.12f);
				Vector(0.8f, 0.8f, 0.2f, vLight);
				CreateSprite(BITMAP_SHINY + 1, vPos, 1.0f, vLight, Object);

				for (mu_int32 i = 1; i < 8; ++i)
				{
					b->TransformByObjectBone(ThreadIndex, vPos, Object, i);
					Vector(0.8f, 0.1f, 0.4f, vLight);
					CreateSprite(BITMAP_LIGHT, vPos, 0.6f, vLight, Object);
					CreateSprite(BITMAP_MAGIC, vPos, 0.12f, vLight, Object);
					Vector(0.8f, 0.8f, 0.2f, vLight);
					CreateSprite(BITMAP_SHINY + 1, vPos, 0.3f, vLight, Object);
				}
			}
			break;
		case MODEL_STAFF + 34:
			{
				EVector3 vPos, vLight;

				b->TransformByObjectBone(ThreadIndex, vPos, Object, 1);
				Vector(0.4f, 0.2f, 1.0f, vLight);
				CreateSprite(BITMAP_SHINY + 6, vPos, 1.4f, vLight, Object);
				Vector(0.7f, 0.1f, 0.9f, vLight);
				CreateSprite(BITMAP_LIGHT, vPos, 1.5f, vLight, Object);
				Vector(0.0f, 0.0f, 1.0f, vLight);
				CreateSprite(BITMAP_SHINY + 2, vPos, 1.5f, vLight, Object);
				Vector(0.4f, 0.4f, 1.0f, vLight);
				CreateSprite(BITMAP_PIN_LIGHT, vPos, 0.7f, vLight, Object, -((mu_int32)(FWorldTime*0.04f) % 360));
				CreateSprite(BITMAP_PIN_LIGHT, vPos, 0.8f, vLight, Object, -((mu_int32)(FWorldTime*0.03f) % 360));

				mu_float quarterAngle, theta, fSize;
				quarterAngle = Q_PI / 180.0f*(mu_int32(FWorldTime*0.02f) % 90);
				theta = EMath::Abs(EMath::Sin(quarterAngle + Q_PI / 2));
				fSize = EMath::Abs(EMath::Sin(quarterAngle)) * 0.5f;
				Vector(0.7f * theta, 0.1f * theta, 0.9f * theta, vLight);
				CreateSprite(BITMAP_MAGIC, vPos, fSize, vLight, Object);
				quarterAngle = Q_PI / 180.0f*(mu_int32(FWorldTime*0.05f) % 60 + 30);
				theta = EMath::Abs(EMath::Sin(quarterAngle + Q_PI / 2));
				fSize = EMath::Abs(EMath::Sin(quarterAngle)) * 0.5f;
				Vector(0.1f + 0.7f * theta, 0.1f * theta, 0.1f + 0.3f * theta, vLight);
				CreateSprite(BITMAP_MAGIC, vPos, fSize, vLight, Object);

				Vector(0.9f, 0.0f, 0.1f, vLight);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 2);
				CreateSprite(BITMAP_SHINY + 6, vPos, 0.8f, vLight, Object);

				Vector(0.9f, 0.4f, 0.7f, vLight);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 3);
				CreateSprite(BITMAP_SHINY + 6, vPos, 0.6f, vLight, Object);
			}
			break;
		case MODEL_SPEAR + 11:
			{
				EVector3 vPos, vLight;

				Vector(0.9f, 0.2f, 0.7f, vLight);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 3);
				CreateSprite(BITMAP_SHINY + 6, vPos, 0.8f, vLight, Object);
				CreateSprite(BITMAP_SHINY + 6, vPos, 0.3f, vLight, Object);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 4);
				CreateSprite(BITMAP_SHINY + 6, vPos, 0.8f, vLight, Object);
				CreateSprite(BITMAP_SHINY + 6, vPos, 0.3f, vLight, Object);

				Vector(0.2f, 0.9f, 0.1f, vLight);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 5);
				CreateSprite(BITMAP_LIGHT, vPos, 1.5f, vLight, Object);
			}
			break;
		case MODEL_MACE + 18:
			{
				EVector3 vPos, vLight;
				mu_float fSize = 0.0f;

				b->TransformByObjectBone(ThreadIndex, vPos, Object, 5);
				Vector(0.5f, 0.8f, 0.9f, vLight);
				CreateSprite(BITMAP_SHINY + 1, vPos, 0.5f, vLight, Object);
				Vector(0.4f, 0.7f, 0.9f, vLight);
				CreateSprite(BITMAP_LIGHTMARKS, vPos, 1.0f, vLight, Object);

				Vector(0.4f, 0.6f, 0.9f, vLight);
				if (mu_int32(FWorldTime) % 14 == 0)
					CreateParticle(ThreadIndex, BITMAP_SHINY + 6, vPos, o->Angle, vLight, 1);

				Vector(0.5f, 0.6f, 0.9f, vLight);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 6);
				CreateSprite(BITMAP_SHINY + 1, vPos, 1.1f, vLight, Object);

				Vector(0.8f, 0.5f, 0.2f, vLight);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 2);
				CreateSprite(BITMAP_SHINY + 1, vPos, 0.5f, vLight, Object);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 3);
				CreateSprite(BITMAP_SHINY + 1, vPos, 0.5f, vLight, Object);

				Vector(1.0f, 1.0f, 1.0f, vLight);
				b->TransformByObjectBone(ThreadIndex, vPos, Object, 4);
				fSize = 0.5f * EMath::Abs((EMath::Sin(FWorldTime*0.0015f)));
				CreateSprite(BITMAP_FLARE, vPos, fSize, vLight, Object);
			}
			break;
		case MODEL_BOW + 24:
			{
				EVector3 vPos, vLight;
				mu_float quarterAngle, theta, fSize;

				Vector(0.2f, 0.8f, 0.5f, vLight);
				mu_int32 iBoneNum[2] = { 7,11 };

				for (mu_int32 i = 0; i < 2; ++i)
				{
					b->TransformByObjectBone(ThreadIndex, vPos, Object, iBoneNum[i]);

					CreateSprite(BITMAP_LIGHT, vPos, 0.8f, vLight, Object);
					CreateSprite(BITMAP_LIGHT, vPos, 0.8f, vLight, Object);

					quarterAngle = Q_PI / 180.0f*(mu_int32(FWorldTime*0.02f) % 90);
					theta = EMath::Abs(EMath::Sin(quarterAngle + Q_PI / 2));
					fSize = EMath::Abs(EMath::Sin(quarterAngle)) * 0.3f;
					Vector(0.2f * theta, 0.8f * theta, 0.5f * theta, vLight);
					CreateSprite(BITMAP_SHOCK_WAVE, vPos, fSize, vLight, Object);

					quarterAngle = Q_PI / 180.0f*(mu_int32(FWorldTime*0.05f) % 60 + 30);
					theta = EMath::Abs(EMath::Sin(quarterAngle + Q_PI / 2));
					fSize = EMath::Abs(EMath::Sin(quarterAngle)) * 0.3f;
					Vector(0.2f * theta, 0.8f * theta, 0.5f * theta, vLight);
					CreateSprite(BITMAP_SHOCK_WAVE, vPos, fSize, vLight, Object);
				}

				if (o->CurrentAction >= PLAYER_ATTACK_FIST && o->CurrentAction <= PLAYER_RIDE_SKILL)
				{
					Vector(0.2f, 0.8f, 0.5f, vLight);
					for (mu_int32 i = 0; i < 8; ++i)
					{
						b->TransformByObjectBone(ThreadIndex, vPos, Object, 22 + i);
						CreateSprite(BITMAP_FLARE, vPos, 0.4f, vLight, Object);
					}
				}
			}
			break;
		}
	}
}

#define MAX_GRADE_OBJ 2
void NextGradeObjectRender(const mu_uint32 ThreadIndex, CHARACTER *c)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_int32 weaponIndex;
	mu_int32 weaponIndex2;
	MUModel *b = &MODELS::Data[c->Object.Type];
	EVector3 vRelativePos, vPos, vLight;
	mu_float fLight2, fScale;
	mu_int32 Level;
	PART_t *w;

	for (mu_int32 i = 0; i < 2; ++i)
	{
		w = &c->Weapon[i];
		Level = w->Level;
		if (Level < 15 || w->Type == -1) continue;

		if (MODEL_BOW <= w->Type && w->Type < MODEL_STAFF)
		{
			weaponIndex = 27;
			weaponIndex2 = 28;
		}
		else
		{
			weaponIndex = (i == 0 ? 27 : 36);
			weaponIndex2 = (i == 0 ? 28 : 37);
		}

		switch (Level)
		{
		case 15:
			{
				Vector(0.0f, 0.0f, 0.0f, vRelativePos);
				b->TransformByObjectBone(ThreadIndex, vPos, &c->Object, weaponIndex, &vRelativePos);
				Vector(1.0f, 0.6f, 0.0f, vLight);
				CreateSprite(BITMAP_LIGHT, vPos, 0.6f, vLight, &c->Object);

				Vector(10.0f, 0.0f, 0.0f, vRelativePos);
				b->TransformByObjectBone(ThreadIndex, vPos, &c->Object, weaponIndex, &vRelativePos);
				Vector(1.0f, 0.6f, 0.0f, vLight);
				CreateSprite(BITMAP_LIGHT, vPos, 0.6f, vLight, &c->Object);

				Vector(20.0f, 0.0f, 0.0f, vRelativePos);
				b->TransformByObjectBone(ThreadIndex, vPos, &c->Object, weaponIndex, &vRelativePos);
				Vector(1.0f, 0.6f, 0.0f, vLight);
				CreateSprite(BITMAP_LIGHT, vPos, 0.6f, vLight, &c->Object);

				Vector(10.0f, 0.0f, 0.0f, vRelativePos);
				b->TransformByObjectBone(ThreadIndex, vPos, &c->Object, weaponIndex2, &vRelativePos);

				fLight2 = EMath::Abs(EMath::Sin(FWorldTime*0.002f));
				Vector(0.7f*fLight2 + 0.3f, 0.1f*fLight2 + 0.1f, 0.0f, vLight);
				CreateSprite(BITMAP_MAGIC, vPos, 0.35f, vLight, &c->Object);

				Vector(1.0f, 1.0f, 1.0f, vLight);
				fScale = (mu_float)(GetLargeRand(60))*0.01f;
				CreateSprite(BITMAP_FLARE_RED, vPos, 0.6f*fScale + 0.4f, vLight, &c->Object);

				Vector(1.0f, 0.2f, 0.0f, vLight);
				fScale = (mu_float)(GetLargeRand(80) + 10)*0.01f*1.0f;
				CreateParticle(ThreadIndex, BITMAP_LIGHTNING_MEGA1 + GetLargeRand(3), vPos, c->Object.Angle, vLight, 0, fScale);
			}
			break;
		}
	}

	static const mu_int32 bornIndex[MAX_BODYPART][MAX_GRADE_OBJ] =
	{
		{ -1, -1 },
		{ 20, -1 },
		{ 35, 26 },
		{ 3, 10 },
		{ 36, 27 },
		{ 4, 11 },
	};

	static const mu_int32 gradeType[MAX_BODYPART][MAX_GRADE_OBJ] =
	{
		{ -1, -1 },
		{ MODEL_15GRADE_ARMOR_OBJ_HEAD, -1 },
		{ MODEL_15GRADE_ARMOR_OBJ_BODYLEFT, MODEL_15GRADE_ARMOR_OBJ_BODYRIGHT },
		{ MODEL_15GRADE_ARMOR_OBJ_PANTLEFT, MODEL_15GRADE_ARMOR_OBJ_PANTRIGHT },
		{ MODEL_15GRADE_ARMOR_OBJ_ARMLEFT, MODEL_15GRADE_ARMOR_OBJ_ARMRIGHT },
		{ MODEL_15GRADE_ARMOR_OBJ_BOOTLEFT, MODEL_15GRADE_ARMOR_OBJ_BOOTRIGHT },
	};

	for (mu_int32 k = 0; k < MAX_BODYPART; ++k)
	{
		w = &c->BodyPart[k];
		Level = w->Level;

		if (k == 0) continue;
		if (Level < 15 || w->Type == -1) continue;

		OBJECT *o = &c->Object;

		for (mu_int32 m = 0; m < MAX_GRADE_OBJ; ++m)
		{
			if (gradeType[k][m] == -1) continue;

			switch (Level)
			{
			case 15: // +15
				{
					w->LinkBone = b->GetBoneIndex(bornIndex[k][m]);
					RenderLinkObject(ThreadIndex, 0.0f, 0.0f, 0.0f, c, w, gradeType[k][m], 0, 0, true, true);

					b->TransformByBoneMatrix(ThreadIndex, vPos, ThreadInfo.BoneTransform[b->GetBoneIndex(0)], &o->Position);

					fLight2 = EMath::Abs(EMath::Sin(FWorldTime*0.01f));
					Vector(0.2f*fLight2, 0.4f*fLight2, 1.0f*fLight2, vLight);
					CreateSprite(BITMAP_MAGIC, vPos, 0.12f, vLight, o);

					Vector(0.4f, 0.7f, 1.0f, vLight);
					CreateSprite(BITMAP_SHINY + 6, vPos, 0.4f, vLight, o);
					Vector(0.1f, 0.3f, 1.0f, vLight);
					CreateSprite(BITMAP_PIN_LIGHT, vPos, 0.6f, vLight, o, 90.0f);
				}
				break;
			}
		}
	}
}

mu_boolean IsBackItem(CHARACTER *c, mu_int32 iType)
{
	mu_int32 iBowType = GetEquipedBowType(c);
	if (iBowType != BOWTYPE_NONE)
	{
		return true;
	}

	if (iType >= MODEL_SWORD && iType < MODEL_HELM &&
		!(iType >= MODEL_STAFF + 21 && iType <= MODEL_STAFF + 29))
	{
		return true;
	}

	return false;
}

mu_boolean RenderCharacterBackItem(const mu_uint32 ThreadIndex, CHARACTER *c, OBJECT* o, mu_boolean bTranslate)
{
	mu_boolean bBindBack = false;

	if (c->SafeZone)
	{
		bBindBack = true;
	}
	if (o->CurrentAction >= PLAYER_GREETING1 && o->CurrentAction <= PLAYER_SALUTE1)
	{
		bBindBack = true;
	}
	if ((GET_WORLD == WD_7ATLANSE ||
		InHellas() ||
		GET_WORLD == WD_67DOPPLEGANGER3 ) &&
		(o->CurrentAction == PLAYER_WALK_SWIM ||
			o->CurrentAction == PLAYER_RUN_SWIM))
	{
		bBindBack = true;
	}

	if (InBloodCastle() == true)
	{
		bBindBack = false;

#if 0 // FIX
		if (IsGMCharacter() == true)
		{
			return bBindBack;
		}
#endif
	}
	if (InChaosCastle() == true)
	{
		bBindBack = false;
	}

	if (o->Type == MODEL_PLAYER)
	{
		const MUModel *b = &MODELS::Data[MODEL_PLAYER];

		mu_boolean bBack = false;
		mu_int32 iBackupType = -1;

		for (mu_int32 i = 0; i < 2; ++i)
		{
			mu_int32 iType = c->Weapon[i].Type;
			mu_int32 iLevel = c->Weapon[i].Level;
			mu_int32 iExcOption = c->Weapon[i].ExcOption;

			if (iType < 0)
				continue;

			if (o->Kind == KIND_NPC && GET_WORLD == WD_0LORENCIA && o->Type == MODEL_PLAYER && (o->SubType >= MODEL_SKELETON1 && o->SubType <= MODEL_SKELETON3))
			{
				if (i == 0)
				{
					bBack = true;
					iType = MODEL_BOW + 9;
					iLevel = 8;
				}
			}

			if (IsBackItem(c, iType) == true)
			{
				bBack = true;
			}

			if (iType == MODEL_BOW + 7 || iType == MODEL_BOW + 15)
			{
				bBack = true;
			}
			else
			{
				if (bBindBack == false)
					bBack = false;
			}

			if (bBack && iType != -1)
			{
				PART_t *w = &c->Wing;

				mu_float fAnimationFrameBackUp = w->AnimationFrame;

				w->LinkBone = b->GetBoneIndex(47);

				if (o->CurrentAction == PLAYER_FLY || o->CurrentAction == PLAYER_FLY_CROSSBOW)
				{
					w->PlaySpeed = 1.0f;
				}
				else
				{
					w->PlaySpeed = 0.25f;
				}

				w->PlaySpeed *= MU_FRAMEELAPSED;

				PART_t t_Part;

				if (iType >= MODEL_SWORD && iType < MODEL_HELM)
				{
					mu_memcpy(&t_Part, w, sizeof(PART_t));
					t_Part.CurrentAction = 0;
					t_Part.AnimationFrame = 0.0f;
					t_Part.PlaySpeed = 0.0f;
					t_Part.PriorAction = 0;
					t_Part.PriorAnimationFrame = 0.0f;
				}

				if (iType == MODEL_BOW + 23)
				{
					PART_t *pWeapon = &c->Weapon[1];
					mu_uint8 byTempLinkBone = pWeapon->LinkBone;
					pWeapon->CurrentAction = 2;
					pWeapon->PlaySpeed = 0.25f;
					w->PlaySpeed *= MU_FRAMEELAPSED;
					pWeapon->LinkBone = b->GetBoneIndex(47);
					RenderLinkObject(ThreadIndex, 0.0f, 0.0f, 15.0f, c, pWeapon, iType, iLevel, iExcOption, true, bTranslate);
					pWeapon->LinkBone = byTempLinkBone;
				}
				else
				{
					mu_boolean bRightHandItem = false;
					if (i == 0)
						bRightHandItem = true;
					RenderLinkObject(ThreadIndex, 0.0f, 0.0f, 15.0f, c, &t_Part, iType, iLevel, iExcOption, true, bTranslate, 0, bRightHandItem);
				}

				w->AnimationFrame = fAnimationFrameBackUp;
			}

			iBackupType = iType;
		}

		if (InBloodCastle() && c->EtcPart != (mu_uint8)-1)
		{
			PART_t *w = &c->Wing;

			mu_int32 iType = -1;
			mu_int32 iLevel = 0;
			mu_int32 iExcOption = 0;

			switch (c->EtcPart)
			{
			case PARTS_STAFF: iType = MODEL_STAFF + 10; break;
			case PARTS_SWORD: iType = MODEL_SWORD + 19; break;
			case PARTS_BOW: iType = MODEL_BOW + 18; break;
			}

			if (iType != -1)
			{
				w->LinkBone = b->GetBoneIndex(47);
				if (o->CurrentAction == PLAYER_FLY || o->CurrentAction == PLAYER_FLY_CROSSBOW)
					w->PlaySpeed = 1.0f;
				else
					w->PlaySpeed = 0.25f;

				w->PlaySpeed *= MU_FRAMEELAPSED;

				RenderLinkObject(ThreadIndex, 0.0f, 0.0f, 15.0f, c, w, iType, iLevel, iExcOption, true, bTranslate);
			}
		}

		CreatePartsFactory(c);
		RenderParts(ThreadIndex, c);

		if (InChaosCastle() == false)
		{
			PART_t *w = &c->Wing;
			if (w->Type != -1)
			{
				w->LinkBone = b->GetBoneIndex(47);
				if (o->CurrentAction == PLAYER_FLY || o->CurrentAction == PLAYER_FLY_CROSSBOW)
				{
					if (w->Type == MODEL_WING + 36)
					{
						w->PlaySpeed = 0.5f;
					}
					else
					{
						w->PlaySpeed = 1.0f;
					}
				}
				else
				{
					w->PlaySpeed = 0.25f;
				}

				w->PlaySpeed *= MU_FRAMEELAPSED;

				switch (w->Type)
				{
				case MODEL_WING + 40:
					w->LinkBone = b->GetBoneIndex(19);
					RenderLinkObject(ThreadIndex, 0.0f, 0.0f, 15.0f, c, w, w->Type, w->Level, w->ExcOption, true, bTranslate);
					break;
				default:
					RenderLinkObject(ThreadIndex, 0.0f, 0.0f, 15.0f, c, w, w->Type, w->Level, w->ExcOption, false, bTranslate);
					break;
				}
			}

			mu_int32 iType = c->Helper.Type;
			if (iType == MODEL_HELPER + 1)
			{
				PART_t *w = &c->Helper;
				w->LinkBone = b->GetBoneIndex(34);
				w->PlaySpeed = 0.5f;
				w->PlaySpeed *= MU_FRAMEELAPSED;
				RenderLinkObject(ThreadIndex, 20.0f, 0.0f, 0.0f, c, w, iType, w->Level, w->ExcOption, false, bTranslate);
				EVector3 vRelativePos, vPos, vLight;
				Vector(20.0f, 0.0f, 15.0f, vRelativePos);

				MUModel *b = &MODELS::Data[o->Type];
				b->TransformPosition(ThreadIndex, o->BoneTransform[w->LinkBone], vRelativePos, vPos, true);

				mu_float fLuminosity = (mu_float)(GetLargeRand(30) + 70)*0.01f;
				Vector(fLuminosity*0.5f, fLuminosity*0.0f, fLuminosity*0.0f, vLight);
				CreateSprite(BITMAP_LIGHT, vPos, 1.5f, vLight, o);
			}
		}
	}

	return bBindBack;
}

void ItemObjectAttribute(OBJECT *o)
{
	Vector(0.3f, 0.3f, 0.3f, o->Light);
	o->Flags.Set(OBJECT::eOF_LIGHTENABLE);
	o->Flags.Clear(OBJECT::eOF_ALPHAENABLE);
	o->Velocity = 0.0f;
	o->CollisionRange = 50.0f;
	o->PriorAnimationFrame = 0.0f;
	o->AnimationFrame = 0.0f;
	o->PriorAction = 0;
	o->CurrentAction = 0;
	o->HiddenMesh = -1;
	o->Gravity = 0.0f;
	o->Alpha = 1.0f;
	o->BlendMesh = -1;
	o->BlendMeshLight = 1.0f;
	o->BlendMeshTexCoordU = 0.0f;
	o->BlendMeshTexCoordV = 0.0f;
	o->HiddenMesh = -1;
	o->Scale = 0.8f;
	g_CharacterClearBuff(o);
	if (o->Type >= MODEL_SPEAR && o->Type <= MODEL_STAFF + 13)
		o->Scale = 0.7f;
	switch (o->Type)
	{
	case MODEL_BOW + 16:
		o->BlendMesh = -2;
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.004f)*0.2f + 0.9f;
		break;
	case MODEL_STAFF + 8:
		o->BlendMesh = -2;
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.004f)*0.2f + 0.9f;
		break;
	case MODEL_STAFF + 7:
		o->BlendMesh = 1;
		o->BlendMeshLight = (mu_float)(GetLargeRand(11))*0.1f;
		break;
	case MODEL_STAFF + 6:
		o->BlendMesh = -2;
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.004f)*0.3f + 0.7f;
		break;
	case MODEL_BOW + 6:
	case MODEL_BOW + 13:
	case MODEL_BOW + 14:
		o->BlendMesh = -2;
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.004f)*0.3f + 0.7f;
		break;
	case MODEL_PANTS + 24:
	case MODEL_HELM + 24:
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.001f)*0.4f + 0.6f;
		break;
	case MODEL_STAFF + 11:
		o->BlendMesh = 2;
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.004f)*0.3f + 0.7f;
		break;
	case MODEL_MACE + 4:
		o->BlendMesh = 1;
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.004f)*0.2f + 0.8f;
		break;
	case MODEL_MACE + 5:
		o->BlendMesh = 0;
		break;
	case MODEL_MACE + 6:
		o->BlendMesh = 1;
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.004f)*0.3f + 0.7f;
		break;
	case MODEL_SPEAR + 9:
		o->BlendMeshTexCoordV = -(mu_float)((mu_int32)(FWorldTime) % 2000)*0.0005f;
		break;
	case MODEL_SHIELD + 14:
		o->BlendMesh = 1;
		o->BlendMeshTexCoordU = (mu_float)(GetLargeRand(10))*0.1f;
		o->BlendMeshTexCoordV = (mu_float)(GetLargeRand(10))*0.1f;
		break;
	case MODEL_SHIELD + 16:
		o->HiddenMesh = 2;
		break;
	case MODEL_MACE + 7:
		o->HiddenMesh = 2;
		break;
	case MODEL_SWORD + 31:
		o->HiddenMesh = 2;
		break;
	case MODEL_SPEAR + 10:
		o->HiddenMesh = 1;
		break;
	case MODEL_STAFF + 9:
		o->BlendMesh = 1;
		break;
	case MODEL_STAFF + 5:
		o->BlendMesh = 2;
		o->BlendMeshTexCoordU = (mu_float)(GetLargeRand(10))*0.1f;
		o->BlendMeshTexCoordV = (mu_float)(GetLargeRand(10))*0.1f;
		break;
	case MODEL_ETC:
		o->Scale = 0.7f;
		break;
	case MODEL_POTION + 1:
		o->Scale = 1.0f;
		break;
	case MODEL_POTION + 21:
		o->Scale = 0.5f;
		break;
	case MODEL_SWORD + 14:
		o->BlendMesh = 1;
		o->BlendMeshTexCoordU = (mu_float)(GetLargeRand(10))*0.1f;
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.004f)*0.3f + 0.7f;
		break;
	case MODEL_SWORD + 10:
	case MODEL_SPEAR:
		o->BlendMesh = 1;
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.004f)*0.3f + 0.7f;
		break;
	case MODEL_SWORD + 5:
	case MODEL_SWORD + 13:
		o->BlendMesh = 1;
		break;
	case MODEL_STAFF:
		o->BlendMesh = 2;
		break;
	case MODEL_HELPER:
		o->BlendMesh = 1;
		break;
	case MODEL_WING + 3:
		o->Scale = 0.5f;
		o->BlendMesh = 0;
	case MODEL_WING + 4:
	case MODEL_WING + 5:
	case MODEL_WING + 6:
		break;
	case MODEL_WING:
	case MODEL_WING + 8:
	case MODEL_WING + 9:
	case MODEL_WING + 10:
	case MODEL_WING + 11:
	case MODEL_WING + 20:
	case MODEL_WING + 132:
		o->BlendMesh = 0;
		break;
	case MODEL_SHIELD + 11:
	case MODEL_SHIELD + 12:
	case MODEL_SHIELD + 13:
		o->BlendMesh = 1;
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.004f)*0.3f + 0.7f;
		break;
	}
}

mu_boolean CheckCharacterRange(OBJECT *so, mu_float Range, mu_int16 PKKey, mu_uint8 Kind)
{
	if (Kind != 0)
	{
		for (mu_uint32 i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
		{
			CHARACTER *c = &CharactersClient[i];
			OBJECT    *o = &c->Object;
			if (o->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == false) continue;

			if (Kind == o->Kind) continue;

			if (so->Owner != o)
			{
				mu_float dx = so->Position[0] - o->Position[0];
				mu_float dy = so->Position[1] - o->Position[1];
				mu_float Distance = EMath::Sqrt(dx*dx + dy * dy);

				if ((c != Hero || (Kind == KIND_MONSTER)) && !c->Dead && Distance <= Range)
				{
					return true;
				}
			}
		}
	}
	else
	{
		for (mu_uint32 i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
		{
			CHARACTER *c = &CharactersClient[i];
			OBJECT    *o = &c->Object;
			if (o->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == false) continue;

			if (so->Owner != o)
			{
				mu_float dx = so->Position[0] - o->Position[0];
				mu_float dy = so->Position[1] - o->Position[1];
				mu_float Distance = EMath::Sqrt(dx*dx + dy * dy);

				if ((c != Hero || (Kind == KIND_MONSTER)) && !c->Dead && Distance <= Range)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void CheckClientArrow(const mu_uint32 ThreadIndex, OBJECT *o)
{
	if (battleCastle::IsBattleCastleStart())
	{
		mu_uint32 att = TERRAIN_ATTRIBUTE(o->Position[0], o->Position[1]);
		if ((att&TW_NOATTACKZONE) == TW_NOATTACKZONE)
		{
			o->Velocity = 0.0f;
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			o->LifeTime /= 2.0f;
			return;
		}
	}

	if (!o->Kind || o->SubType == 99)
	{
		if (CheckCharacterRange(o, 100.0f, o->PKKey))
		{
			mu_uint32 Skill = CharacterAttribute->Skill[o->Skill];

			switch (Skill)
			{
			case AT_SKILL_PIERCING:
				if (o->SubType == 2)
				{
					CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 6, o, 30.0f);
				}
				else
				{
					o->Flags.Clear(OBJECT::eOF_LIVE);
					CreateBomb(ThreadIndex, o->Position, true);
				}
				break;
			case AT_SKILL_PARALYZE:
				o->Flags.Clear(OBJECT::eOF_LIVE);
				if (o->Type == MODEL_ARROW_BOMB || o->Type == MODEL_ARROW_HOLY)
					CreateBomb(ThreadIndex, o->Position, true);
				break;
			default:
				o->Flags.Clear(OBJECT::eOF_LIVE);

				if (o->Type == MODEL_ARROW_BOMB || o->Type == MODEL_ARROW_HOLY)
				{
					if (o->SubType == 1)
					{
					}
					else
					{
						CreateBomb(ThreadIndex, o->Position, true);
					}
				}
				break;
			}
		}
	}
}

void CheckGrass(OBJECT* o)
{
	EVector3 Position;
	VectorCopy(Hero->Object.Position, Position);
	if ((Hero->Object.CurrentAction >= PLAYER_WALK_MALE && Hero->Object.CurrentAction <= PLAYER_RUN_RIDE_WEAPON)
		|| (Hero->Object.CurrentAction >= PLAYER_FENRIR_RUN && Hero->Object.CurrentAction <= PLAYER_FENRIR_RUN_ONE_LEFT_ELF)
		|| (Hero->Object.CurrentAction >= PLAYER_FENRIR_WALK && Hero->Object.CurrentAction <= PLAYER_FENRIR_WALK_ONE_LEFT)
		)
	{
		if (o->Direction[0] < 0.1f)
		{
			mu_float dx = Position[0] - o->Position[0];
			mu_float dy = Position[1] - o->Position[1];
			mu_float Distance = EMath::Sqrt(dx*dx + dy*dy);
			if (Distance < 50.0f)
			{
				Vector(-dx*0.6f, -dy*0.6f, 0.0f, o->Direction);
			}
		}

		VectorScale(o->Direction, 0.6f, o->Direction);
		VectorScale(o->HeadAngle, 0.6f, o->HeadAngle);
		VectorAdd(o->Position, o->Direction, o->Position);
		VectorAdd(o->Angle, o->HeadAngle, o->Angle);
	}
}

mu_int32 GetEquipedBowType(CHARACTER *pChar)
{
	if ((pChar->Weapon[1].Type != MODEL_BOW + 7)
		&& ((pChar->Weapon[1].Type >= MODEL_BOW) && (pChar->Weapon[1].Type < MODEL_STAFF))
		)
	{
		return BOWTYPE_BOW;
	}
	else if ((pChar->Weapon[0].Type != MODEL_BOW + 15)
		&& ((pChar->Weapon[0].Type >= MODEL_BOW + 8) && (pChar->Weapon[0].Type < MODEL_STAFF))
		)
	{
		return BOWTYPE_CROSSBOW;
	}

	return BOWTYPE_NONE;
}

mu_int32 GetEquipedBowType(ITEM* pItem)
{
	if (
		((pItem->Type >= ITEM_BOW) && (pItem->Type <= ITEM_BOW + 6))
		|| (pItem->Type == ITEM_BOW + 17)
		|| ((pItem->Type >= ITEM_BOW + 20) && (pItem->Type <= ITEM_BOW + 23))
		|| (pItem->Type == ITEM_BOW + 24)
		)
	{
		return BOWTYPE_BOW;
	}
	else if (
		((pItem->Type >= ITEM_BOW + 8) && (pItem->Type <= ITEM_BOW + 14))
		|| (pItem->Type == ITEM_BOW + 16)
		|| ((pItem->Type >= ITEM_BOW + 18) && (pItem->Type <= ITEM_BOW + 19))
		)
	{
		return BOWTYPE_CROSSBOW;
	}

	return BOWTYPE_NONE;
}

mu_int32 GetEquipedBowType()
{
	if ((CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Type != ITEM_BOW + 7) &&
		((CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Type >= ITEM_BOW)
			&& (CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Type < ITEM_STAFF))
		)
	{
		return BOWTYPE_BOW;
	}
	else if ((CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT].Type != ITEM_BOW + 15) &&
		((CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT].Type >= ITEM_BOW + 8)
			&& (CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT].Type < ITEM_STAFF))
		)
	{
		return BOWTYPE_CROSSBOW;
	}

	return BOWTYPE_NONE;
}

mu_int32 GetEquipedBowType_Skill()
{
	if ((CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Type != ITEM_BOW + 7) &&
		((CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Type >= ITEM_BOW)
			&& (CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Type < ITEM_BOW + MAX_ITEM_INDEX))
		)
	{
		if (CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT].Type == ITEM_BOW + 15)
			return BOWTYPE_BOW;
	}
	else if ((CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT].Type != ITEM_BOW + 15) &&
		((CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT].Type >= ITEM_BOW + 8)
			&& (CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT].Type < ITEM_BOW + MAX_ITEM_INDEX))
		)
	{
		if (CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT].Type == ITEM_BOW + 7)
			return BOWTYPE_CROSSBOW;
	}

	return BOWTYPE_NONE;
}

mu_boolean IsEquipedWing()
{
	ITEM* pEquippedItem = &CharacterMachine->Equipment[GAME::EQUIPMENT_WING];

	if ((pEquippedItem->Type >= ITEM_WING && pEquippedItem->Type <= ITEM_WING + 6)
		|| (pEquippedItem->Type >= ITEM_WING + 36 && pEquippedItem->Type <= ITEM_WING + 43)
		|| pEquippedItem->Type == ITEM_HELPER + 30
		|| (ITEM_WING + 130 <= pEquippedItem->Type && pEquippedItem->Type <= ITEM_WING + 134))
	{
		return true;
	}

	return false;
}

void RenderSkillEarthQuake(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b, mu_int32 iMaxSkill)
{
	EBone Matrix;
	EVector3 Angle, p, Position;
	OBJECT& TargetO = CharactersClient[c->TargetCharacter].Object;

	++o->WeaponLevel;

	if (o->WeaponLevel == iMaxSkill - 2 ||
		o->WeaponLevel == iMaxSkill - 1 ||
		o->WeaponLevel == iMaxSkill - 0)
	{
		Vector(0.0f, 40.0f*(o->WeaponLevel / 2), 0.0f, p);
		Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
		for (mu_int32 i = 0; i < 6; ++i)
		{
			Angle[2] += 60.0f;
			AngleMatrix(Angle, Matrix);
			VectorRotate(p, Matrix, Position);
			VectorAdd(TargetO.Position, Position, Position);

			mu_int32 iCurrentStone = MODEL_GROUND_STONE;

			CreateEffect(ThreadIndex, iCurrentStone + 1, Position, TargetO.Angle, TargetO.Light);
		}
	}

	if (o->WeaponLevel == iMaxSkill - 1)
	{
		EarthQuake.Store((GetLargeRand(3) - 3)*0.7f);
		CreateEffect(ThreadIndex, MODEL_SKILL_FURY_STRIKE, TargetO.Position, TargetO.Angle, TargetO.Light, 0, o, -1, 0, 2);
	}

	if (o->WeaponLevel > iMaxSkill)
	{
		o->WeaponLevel = 0;
	}
}

OBJECT ObjectSelect;

void RenderObjectScreen(mu_int32 Type, mu_int32 ItemLevel, mu_int32 ExcOption, mu_int32 SetOption, EVector3 Target, mu_int32 Select, mu_boolean PickUp)
{
	mu_int32 Level = ItemLevel;
	EVector3 Direction, Position;

	VectorSubtract(Target, MousePosition, Direction);
	if (PickUp)
		VectorMA(MousePosition, 0.07f, Direction, Position);
	else
		VectorMA(MousePosition, 0.1f, Direction, Position);

	if (Type == MODEL_SWORD + 0)
	{
		Position[0] -= 0.02f;
		Position[1] += 0.03f;
		Vector(180.0f, 270.0f, 15.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_BOW + 7 || Type == MODEL_BOW + 15)
	{
		Vector(0.0f, 270.0f, 15.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_SPEAR + 0)
	{
		Position[1] += 0.05f;
		Vector(0.0f, 90.0f, 20.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_BOW + 17)
	{
		Vector(0.0f, 90.0f, 15.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELM + 31)
	{
		Position[1] -= 0.06f;
		Position[0] += 0.03f;
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELM + 30)
	{
		Position[1] += 0.07f;
		Position[0] -= 0.03f;
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_ARMOR + 30)
	{
		Position[1] += 0.1f;
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_ARMOR + 29)
	{
		Position[1] += 0.07f;
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_BOW + 21)
	{
		Position[1] += 0.12f;
		Vector(180.0f, -90.0f, 15.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_STAFF + 12)
	{
		Position[1] -= 0.1f;
		Position[0] += 0.025f;
		Vector(180.0f, 0.0f, 8.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_STAFF + 21 && Type <= MODEL_STAFF + 29)
	{
		Vector(0.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_MACE + 14)
	{
		Position[1] += 0.1f;
		Position[0] -= 0.01;
		Vector(180.0f, 90.0f, 13.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_ARMOR + 34)
	{
		Position[1] += 0.03f;
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELM + 35)
	{
		Position[0] -= 0.02f;
		Position[1] += 0.05f;
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_ARMOR + 35)
	{
		Position[1] += 0.05f;
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_ARMOR + 36)
	{
		Position[1] -= 0.05f;
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_ARMOR + 37)
	{
		Position[1] -= 0.05f;
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (MODEL_HELM + 39 <= Type && MODEL_HELM + 44 >= Type)
	{
		Position[1] -= 0.05f;
		Vector(-90.0f, 25.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (MODEL_ARMOR + 38 <= Type && MODEL_ARMOR + 44 >= Type)
	{
		Position[1] -= 0.08f;
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_SWORD + 24)
	{
		Position[0] -= 0.02f;
		Position[1] += 0.03f;
		Vector(180.0f, 90.0f, 15.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_MACE + 15)
	{
		Position[1] += 0.05f;
		Vector(180.0f, 90.0f, 13.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_BOW + 22 || Type == MODEL_BOW + 23)
	{
		Position[0] -= 0.10f;
		Position[1] += 0.08f;
		Vector(180.0f, -90.0f, 15.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_STAFF + 13)
	{
		Position[0] += 0.02f;
		Position[1] += 0.02f;
		Vector(180.0f, 90.0f, 8.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_BOW + 20)
	{
		Vector(180.0f, -90.0f, 15.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_BOW + 8 && Type < MODEL_BOW + MAX_ITEM_INDEX)
	{
		Vector(90.0f, 180.0f, 20.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_SPEAR + 10)
	{
		Vector(180.0f, 270.0f, 20.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_SWORD && Type < MODEL_STAFF + MAX_ITEM_INDEX)
	{
		switch (Type)
		{
		case MODEL_STAFF + 14:							Position[1] += 0.04f;	break;
		case MODEL_STAFF + 17:	Position[0] += 0.02f;	Position[1] += 0.03f;	break;
		case MODEL_STAFF + 18:	Position[0] += 0.02f;							break;
		case MODEL_STAFF + 19:	Position[0] -= 0.02f;	Position[1] -= 0.02f;	break;
		case MODEL_STAFF + 20:	Position[0] += 0.01f;	Position[1] -= 0.01f;	break;
		}

		if (!ItemAttribute[Type - MODEL_ITEM].TwoHand)
		{
			Vector(180.0f, 270.0f, 15.0f, ObjectSelect.Angle);
		}
		else
		{
			Vector(180.0f, 270.0f, 25.0f, ObjectSelect.Angle);
		}
	}
	else if (Type >= MODEL_SHIELD && Type < MODEL_SHIELD + MAX_ITEM_INDEX)
	{
		Vector(270.0f, 270.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 3)
	{
		Vector(-90.0f, -90.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 4)
	{
		Vector(-90.0f, -90.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 5)
	{
		Vector(-90.0f, -35.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 31)
	{
		Vector(-90.0f, -90.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 30)
	{
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_EVENT + 16)
	{
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 16 || Type == MODEL_HELPER + 17)
	{
		Vector(270.0f, -10.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 18)
	{
		Vector(290.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_EVENT + 11)
	{
		Vector(-90.0f, -20.0f, -20.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_EVENT + 12)
	{
		Vector(250.0f, 140.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_EVENT + 14)
	{
		Vector(255.0f, 160.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_EVENT + 15)
	{
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_HELPER + 21 && Type <= MODEL_HELPER + 24)
	{
		Vector(270.0f, 160.0f, 20.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 29)
	{
		Vector(290.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 32)
	{
		Position[0] += 0.01f;
		Position[1] -= 0.03f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 33)
	{
		Position[1] += 0.02f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 34)
	{
		Position[0] += 0.01f;
		Position[1] += 0.02f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 35)
	{
		Position[0] += 0.01f;
		Position[1] += 0.02f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 36)
	{
		Position[0] += 0.01f;
		Position[1] += 0.05f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 37)
	{
		Position[0] += 0.01f;
		Position[1] += 0.04f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 49)
	{
		Position[1] -= 0.04f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 50)
	{
		Position[1] -= 0.03f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 51)
	{
		Position[1] -= 0.02f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 64)
	{
		Position[1] += 0.02f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 52)
	{
		Position[1] += 0.045f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 53)
	{
		Position[1] += 0.04f;
		Vector(270.0f, 120.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_WING + 37)
	{
		Position[1] += 0.05f;
		Vector(270.0f, -10.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_WING + 38)
	{
		Position[1] += 0.05f;
		Vector(270.0f, -10.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_WING + 39)
	{
		Position[1] += 0.08f;
		Vector(270.0f, -10.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_WING + 40)
	{
		Position[1] += 0.05f;
		Vector(270.0f, -10.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_WING + 42)
	{
		Position[1] += 0.05f;
		Vector(270.0f, 0.0f, 2.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 46)
	{
		Position[1] -= 0.04f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 47)
	{
		Position[1] -= 0.04f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 48)
	{
		Position[1] -= 0.04f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 54)
	{
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 58)
	{
		Position[1] += 0.07f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 59 || Type == MODEL_POTION + 60)
	{
		Position[1] += 0.06f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 61 || Type == MODEL_POTION + 62)
	{
		Position[1] += 0.06f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 53)
	{
		Position[1] += 0.042f;
		Vector(180.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 43)
	{
		Position[1] += 0.082f;
		Vector(90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 44)
	{
		Position[1] += 0.08f;
		Vector(90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 45)
	{
		Position[1] += 0.07f;
		Vector(90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_POTION + 70 && Type <= MODEL_POTION + 71)
	{
		Position[0] += 0.01f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_POTION + 72 && Type <= MODEL_POTION + 77)
	{
		Position[1] += 0.08f;
		Vector(0.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 59)
	{
		Position[0] += 0.01f;
		Position[1] += 0.02f;
		Vector(90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_HELPER + 54 && Type <= MODEL_HELPER + 58)
	{
		Position[1] -= 0.02f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_POTION + 78 && Type <= MODEL_POTION + 82)
	{
		Position[1] += 0.01f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 60)
	{
		Position[1] -= 0.06f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 61)
	{
		Position[1] -= 0.04f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 83)
	{
		Position[1] += 0.06f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 91)
	{
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 92)
	{
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 93)
	{
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 95)
	{
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 94)
	{
		Position[0] += 0.01f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_POTION + 84 && Type <= MODEL_POTION + 90)
	{
		if (Type == MODEL_POTION + 84)
		{
			Position[1] += 0.01f;
			Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
		}
		else if (Type == MODEL_POTION + 85)
		{
			Position[1] -= 0.01f;
			Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
		}
		else if (Type == MODEL_POTION + 86)
		{
			Position[1] += 0.01f;
			Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
		}
		else if (Type == MODEL_POTION + 87)
		{
			Position[1] += 0.01f;
			Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
		}
		else if (Type == MODEL_POTION + 88)
		{
			Position[1] += 0.015f;
			Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
		}
		else if (Type == MODEL_POTION + 89)
		{
			Position[1] += 0.015f;
			Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
		}
		else if (Type == MODEL_POTION + 90)
		{
			Position[1] += 0.015f;
			Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
		}
	}
	else if (Type == MODEL_HELPER + 62)
	{
		Position[0] += 0.01f;
		Position[1] += 0.08f;
		Vector(90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 63)
	{
		Position[0] += 0.01f;
		Position[1] += 0.082f;
		Vector(90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_POTION + 97 && Type <= MODEL_POTION + 98)
	{
		Position[1] += 0.09f;
		Vector(0.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 96)
	{
		Position[1] += 0.13f;
		Vector(90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (MODEL_HELPER + 64 <= Type && Type <= MODEL_HELPER + 65)
	{
		switch (Type)
		{
		case MODEL_HELPER + 64:
			Position[1] -= 0.05f;
			break;
		case MODEL_HELPER + 65:
			Position[1] -= 0.02f;
			break;
		}
		Vector(270.0f, -10.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 65)
	{
		Position[1] += 0.05f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 66)
	{
		Position[1] += 0.11f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 67)
	{
		Position[1] += 0.11f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 39)
	{
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 43)
	{
		Position[1] -= 0.03f;
		Vector(90.0f, 0.0f, 180.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 44)
	{
		Position[1] += 0.08f;
		Vector(90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 45)
	{
		Position[1] += 0.07f;
		Vector(90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 40)
	{
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 41)
	{
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 51)
	{
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 42)
	{
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 38)
	{
		Position[0] += 0.00f;
		Position[1] += 0.02f;
		Vector(-48 - 150.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 41)
	{
		Position[1] += 0.02f;
		Vector(270.0f, 90.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 42)
	{
		Position[1] += 0.02f;
		Vector(270.0f, -10.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 43 || Type == MODEL_POTION + 44)
	{
		Position[0] -= 0.04f;
		Position[1] += 0.02f;
		Position[2] += 0.02f;
		Vector(270.0f, -10.0f, -45.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_HELPER + 12 && Type < MODEL_HELPER + MAX_ITEM_INDEX && Type != MODEL_HELPER + 14 && Type != MODEL_HELPER + 15)
	{
		Vector(270.0f + 90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 12)
	{
		switch (Level)
		{
		case 0:Vector(180.0f, 0.0f, 0.0f, ObjectSelect.Angle); break;
		case 1:Vector(270.0f, 90.0f, 0.0f, ObjectSelect.Angle); break;
		case 2:Vector(90.0f, 0.0f, 0.0f, ObjectSelect.Angle); break;
		}
	}
	else if (Type == MODEL_EVENT + 5)
	{
		Vector(270.0f, 180.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_EVENT + 6)
	{
		Vector(270.0f, 90.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_EVENT + 7)
	{
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 20)
	{
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 27)
	{
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 63)
	{
		Position[1] += 0.08f;
		Vector(-50.0f, -60.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 52)
	{
		Vector(270.0f, -25.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_ETC + 19 && Type <= MODEL_ETC + 27)
	{
		Position[0] += 0.03f;
		Position[1] += 0.03f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_WING + 7)
	{
		Position[0] += 0.005f;
		Position[1] -= 0.015f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_ARMOR + 10)
	{
		Position[1] -= 0.1f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_PANTS + 10)
	{
		Position[1] -= 0.08f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_ARMOR + 11)
	{
		Position[1] -= 0.1f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_PANTS + 11)
	{
		Position[1] -= 0.08f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_WING + 44)
	{
		Position[0] += 0.005f;
		Position[1] -= 0.015f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_WING + 46
		|| Type == MODEL_WING + 45)
	{
		Position[0] += 0.005f;
		Position[1] -= 0.015f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else
	{
		Vector(270.0f, -10.0f, 0.0f, ObjectSelect.Angle);
	}

	if (Type >= MODEL_WING + 60 && Type <= MODEL_WING + 65)
	{
		Vector(10.0f, -10.0f, 10.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_WING + 70 && Type <= MODEL_WING + 74)
	{
		Vector(0.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_WING + 100 && Type <= MODEL_WING + 129)
	{
		Vector(0.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 67)
	{
		Position[1] -= 0.05f;
		Vector(270.0f, 40.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 123)
	{
		Position[1] -= 0.05f;
		Vector(270.0f, 40.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 140)
	{
		Position[1] += 0.09f;
		Vector(0.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_POTION + 145 && Type <= MODEL_POTION + 150)
	{
		Position[0] += 0.010f;
		Position[1] += 0.040f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_HELPER + 125 && Type <= MODEL_HELPER + 127)	
	{
		Position[0] += 0.007f;
		Position[1] -= 0.035f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 124)
	{
		Position[1] -= 0.04f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 80)
	{
		Position[1] -= 0.05f;
		Vector(270.0f, 40.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 106)
	{
		Position[0] += 0.01f;
		Position[1] -= 0.05f;
		Vector(255.0f, 45.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 68)
	{
		Position[0] += 0.02f;
		Position[1] -= 0.02f;
		Vector(300.0f, 10.0f, 20.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 76)
	{
		Position[1] -= 0.02f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 122)
	{
		Position[0] += 0.01f;
		Position[1] -= 0.035f;
		Vector(290.0f, -20.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 128)
	{
		Position[0] += 0.017f;
		Position[1] -= 0.053f;
		Vector(270.0f, -20.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 129)
	{
		Position[0] += 0.012f;
		Position[1] -= 0.045f;
		Vector(270.0f, -20.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 134)
	{
		Position[0] += 0.005f;
		Position[1] -= 0.033f;
		Vector(270.0f, -20.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 130)
	{
		Position[0] += 0.007f;
		Position[1] += 0.005f;
		Vector(270.0f, -20.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 131)
	{
		Position[0] += 0.017f;
		Position[1] -= 0.053f;
		Vector(270.0f, -20.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 132)
	{
		Position[0] += 0.007f;
		Position[1] += 0.045f;
		Vector(270.0f, -20.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 133)
	{
		Position[0] += 0.017f;
		Position[1] -= 0.053f;
		Vector(270.0f, -20.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 69)
	{
		Position[0] += 0.005f;
		Position[1] -= 0.05f;
		Vector(270.0f, -30.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 70)
	{
		Position[0] += 0.040f;
		Position[1] -= 0.000f;
		Vector(270.0f, -0.0f, 70.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 81)
	{
		Position[0] += 0.005f;
		Position[1] += 0.035f;
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 82)
	{
		Position[0] += 0.005f;
		Position[1] += 0.035f;
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 93)
	{
		Position[0] += 0.005f;
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 94)
	{
		Position[0] += 0.005f;
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 66)
	{
		Position[0] += 0.01f;
		Position[1] -= 0.05f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 100)
	{
		Position[0] += 0.01f;
		Position[1] -= 0.05f;
		Vector(0.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 99)
	{
		Position[0] += 0.02f;
		Position[1] -= 0.03f;
		Vector(290.0f, -40.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_STAFF + 33)
	{
		Position[0] += 0.02f;
		Position[1] -= 0.06f;
		Vector(180.0f, 90.0f, 10.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_STAFF + 34)
	{
		Position[1] -= 0.05f;
		Vector(180.0f, 90.0f, 10.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_SPEAR + 11)
	{
		Position[1] += 0.02f;
		Vector(180.0f, 90.0f, 15.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_MACE + 18)
	{
		Position[0] -= 0.03f;
		Position[1] += 0.06f;
		Vector(180.0f, 90.0f, 2.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_BOW + 24)
	{
		Position[0] -= 0.07f;
		Position[1] += 0.07f;
		Vector(180.0f, -90.0f, 15.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_WING + 47)
	{
		Position[0] += 0.005f;
		Position[1] -= 0.015f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 71 || Type == MODEL_HELPER + 72 || Type == MODEL_HELPER + 73 || Type == MODEL_HELPER + 74 || Type == MODEL_HELPER + 75)
	{
		Position[1] += 0.07f;
		Vector(270.0f, 180.0f, 0.0f, ObjectSelect.Angle);
		if (Select != 1)
		{
			ObjectSelect.Angle[1] = FWorldTime*0.2f;
		}
	}
	else if (Type >= MODEL_TYPE_CHARM_MIXWING + EWS_BEGIN
		&& Type <= MODEL_TYPE_CHARM_MIXWING + EWS_END)
	{
		Position[1] -= 0.03f;
		Vector(-90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_HELPER + 117 && Type <= MODEL_HELPER + 120)
	{
		Position[0] += 0.01f;
		Position[1] -= 0.05f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 110)
	{
		Position[0] += 0.005f;
		Position[1] -= 0.02f;
	}
	else if (Type == MODEL_POTION + 111)
	{
		Position[0] += 0.01f;
		Position[1] -= 0.02f;
	}
	else if (Type == MODEL_HELPER + 107)
	{
		Position[0] -= 0.0f;
		Position[1] += 0.0f;
		Vector(90.0f, 225.0f, 45.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 104)
	{
		Position[0] += 0.01f;
		Position[1] -= 0.03f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 105)
	{
		Position[0] += 0.01f;
		Position[1] -= 0.03f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 103)
	{
		Position[0] += 0.01f;
		Position[1] += 0.01f;
		Vector(0.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 133)
	{
		Position[0] += 0.01f;
		Position[1] -= 0.0f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (MODEL_POTION + 101 <= Type && Type <= MODEL_POTION + 109)
	{
		switch (Type)
		{
		case MODEL_POTION + 101:
			{
				Position[0] += 0.005f;
			}break;
		case MODEL_POTION + 102:
			{
				Position[0] += 0.005f;
				Position[1] += 0.05f;
				Vector(0.0f, 0.0f, 30.0f, ObjectSelect.Angle);
			}break;
		case MODEL_POTION + 103:
		case MODEL_POTION + 104:
		case MODEL_POTION + 105:
		case MODEL_POTION + 106:
		case MODEL_POTION + 107:
		case MODEL_POTION + 108:
			{
				Position[0] += 0.005f;
				Position[1] += 0.05f;
				Vector(0.0f, 0.0f, 30.0f, ObjectSelect.Angle);
			}break;
		case MODEL_POTION + 109:
			{
				Position[0] += 0.005f;
				Position[1] += 0.05f;
				Vector(0.0f, 0.0f, 0.0f, ObjectSelect.Angle);
			}break;
		}
	}
	else if (Type >= MODEL_HELPER + 109 && Type <= MODEL_HELPER + 112)
	{
		Position[0] += 0.025f;
		Position[1] -= 0.035f;
		Vector(270.0f, 25.0f, 25.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_HELPER + 113 && Type <= MODEL_HELPER + 115)
	{
		Position[0] += 0.005f;
		Position[1] -= 0.00f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_POTION + 112 && Type <= MODEL_POTION + 113)
	{
		Position[0] += 0.05f;
		Position[1] += 0.009f;
		Vector(270.0f, 180.0f, 45.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_POTION + 120)
	{
		Position[0] += 0.01f;
		Position[1] += 0.05f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (MODEL_POTION + 134 <= Type && Type <= MODEL_POTION + 139)
	{
		Position[0] += 0.00f;
		Position[1] += 0.05f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 116)
	{
		Position[1] += 0.08f;
		Vector(90.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_POTION + 114 && Type <= MODEL_POTION + 119)
	{
		Position[0] += 0.00f;
		Position[1] += 0.06f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_POTION + 126 && Type <= MODEL_POTION + 129)
	{
		Position[0] += 0.00f;
		Position[1] += 0.06f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type >= MODEL_POTION + 130 && Type <= MODEL_POTION + 132)
	{
		Position[0] += 0.00f;
		Position[1] += 0.06f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}
	else if (Type == MODEL_HELPER + 121)
	{
		Position[1] -= 0.04f;
		Vector(270.0f, 0.0f, 0.0f, ObjectSelect.Angle);
	}

	switch (Type)
	{
	case MODEL_SWORD + 26:
		{
			Position[0] -= 0.02f;
			Position[1] += 0.04f;
			Vector(180.0f, 270.0f, 10.0f, ObjectSelect.Angle);
		}break;
	case MODEL_SWORD + 27:
		{
			Vector(180.0f, 270.0f, 15.0f, ObjectSelect.Angle);
		}break;
	case MODEL_SWORD + 28:
		{
			Position[1] += 0.02f;
			Vector(180.0f, 270.0f, 10.0f, ObjectSelect.Angle);
		}break;
	case MODEL_MACE + 16:
		{
			Position[0] -= 0.02f;
			Vector(180.0f, 270.0f, 15.0f, ObjectSelect.Angle);
		}
		break;
	case MODEL_MACE + 17:
		{
			Position[0] -= 0.02f;
			Position[1] += 0.04f;
			Vector(180.0f, 270.0f, 15.0f, ObjectSelect.Angle);
		}break;
	case MODEL_STAFF + 30:
		{
			Vector(180.0f, 90.0f, 10.0f, ObjectSelect.Angle);
		}break;
	case MODEL_STAFF + 31:
		{
			Vector(180.0f, 90.0f, 10.0f, ObjectSelect.Angle);
		}break;
	case MODEL_STAFF + 32:
		{
			Vector(180.0f, 90.0f, 10.0f, ObjectSelect.Angle);
		}break;
	}

	switch (Type)
	{
	case MODEL_STAFF + 7:
		{
			Vector(0.0f, 0.0f, 205.0f, ObjectSelect.Angle);
		}break;
	}

	switch (Type)
	{
	case MODEL_WING + 8:
	case MODEL_WING + 9:
	case MODEL_WING + 10:
	case MODEL_WING + 11:
		{
			Position[0] += 0.005f;
			Position[1] -= 0.02f;
		}break;
	case MODEL_POTION + 21:
		{
			Position[0] += 0.005f;
			Position[1] -= 0.005f;
		}break;
	case MODEL_POTION + 13:
	case MODEL_POTION + 14:
	case MODEL_POTION + 22:
		{
			Position[0] += 0.005f;
			Position[1] += 0.015f;
		}break;
	}

	if (1 == Select)
	{
		ObjectSelect.Angle[1] = EMath::Mod(FWorldTime*0.45f, 360.0f);
	}

	ObjectSelect.Type = Type;
	if (ObjectSelect.Type >= MODEL_HELM && ObjectSelect.Type < MODEL_BOOTS + MAX_ITEM_INDEX)
		ObjectSelect.Type = MODEL_PLAYER;
	else if (ObjectSelect.Type == MODEL_POTION + 12)
	{
		if (Level == 0)
		{
			ObjectSelect.Type = MODEL_EVENT;
			Type = MODEL_EVENT;
		}
		else if (Level == 2)
		{
			ObjectSelect.Type = MODEL_EVENT + 1;
			Type = MODEL_EVENT + 1;
		}
	}

	MUModel *b = &MODELS::Data[ObjectSelect.Type];
	b->CurrentAction(0) = 0;
	ObjectSelect.AnimationFrame = 0;
	ObjectSelect.PriorAnimationFrame = 0;
	ObjectSelect.PriorAction = 0;
	if (Type >= MODEL_HELM && Type < MODEL_HELM + MAX_ITEM_INDEX)
	{
		b->BodyHeight(0) = -160.0f;
	}
	else if (Type >= MODEL_ARMOR && Type < MODEL_ARMOR + MAX_ITEM_INDEX)
	{
		b->BodyHeight(0) = -100.0f;
	}
	else if (Type >= MODEL_GLOVES && Type < MODEL_GLOVES + MAX_ITEM_INDEX)
		b->BodyHeight(0) = -70.0f;
	else if (Type >= MODEL_PANTS && Type < MODEL_PANTS + MAX_ITEM_INDEX)
		b->BodyHeight(0) = -50.0f;
	else
		b->BodyHeight(0) = 0.0f;
	mu_float Scale = 0.0f;

	if (Type >= MODEL_HELM && Type < MODEL_BOOTS + MAX_ITEM_INDEX)
	{
		if (Type >= MODEL_HELM && Type < MODEL_HELM + MAX_ITEM_INDEX)
		{
			Scale = MODEL_HELM + 39 <= Type && MODEL_HELM + 44 >= Type ? 0.007f : 0.0039f;
			if (Type == MODEL_HELM + 31)
				Scale = 0.007f;
		}
		else if (Type >= MODEL_ARMOR && Type < MODEL_ARMOR + MAX_ITEM_INDEX)
			Scale = 0.0039f;
		else if (Type >= MODEL_GLOVES && Type < MODEL_GLOVES + MAX_ITEM_INDEX)
			Scale = 0.0038f;
		else if (Type >= MODEL_PANTS && Type < MODEL_PANTS + MAX_ITEM_INDEX)
			Scale = 0.0033f;
		else if (Type >= MODEL_BOOTS && Type < MODEL_BOOTS + MAX_ITEM_INDEX)
			Scale = 0.0032f;
		else if (Type == MODEL_ARMOR + 30)
			Scale = 0.0035f;
		else if (Type == MODEL_ARMOR + 32)
			Scale = 0.0035f;
		else if (Type == MODEL_ARMOR + 29)
			Scale = 0.0033f;

		if (Type == MODEL_ARMOR + 34)
			Scale = 0.0032f;
		else if (Type == MODEL_ARMOR + 35)
			Scale = 0.0032f;
		else if (Type == MODEL_GLOVES + 38)
			Scale = 0.0032f;
	}
	else
	{
		if (Type == MODEL_WING + 6)
			Scale = 0.0015f;
		else if (Type == MODEL_COMPILED_CELE || Type == MODEL_COMPILED_SOUL)
			Scale = 0.004f;
		else if (Type >= MODEL_WING + 32 && Type <= MODEL_WING + 34)
		{
			Scale = 0.001f;
			Position[1] -= 0.05f;
		}
		else if (Type >= MODEL_WING + 60 && Type <= MODEL_WING + 65)
			Scale = 0.0022f;
		else if (Type >= MODEL_WING + 70 && Type <= MODEL_WING + 74)
			Scale = 0.0017f;
		else if (Type >= MODEL_WING + 100 && Type <= MODEL_WING + 129)
			Scale = 0.0017f;
		else if (Type >= MODEL_WING && Type < MODEL_WING + MAX_ITEM_INDEX)
		{
			Scale = 0.002f;
		}
		else if (Type == MODEL_POTION + 45 || Type == MODEL_POTION + 49)
		{
			Scale = 0.003f;
		}
		else if (Type >= MODEL_POTION + 46 && Type <= MODEL_POTION + 48)
		{
			Scale = 0.0025f;
		}
		else if (Type == MODEL_POTION + 50)
		{
			Scale = 0.001f;
		}
		else if (Type >= MODEL_POTION + 32 && Type <= MODEL_POTION + 34)
		{
			Scale = 0.002f;
			Position[1] += 0.05f;
			Vector(0.0f, ObjectSelect.Angle[1], 0.0f, ObjectSelect.Angle);
		}
		else if (Type >= MODEL_EVENT + 21 && Type <= MODEL_EVENT + 23)
		{
			Scale = 0.002f;
			if (Type == MODEL_EVENT + 21)
				Position[1] += 0.08f;
			else
				Position[1] += 0.06f;
			Vector(0.0f, ObjectSelect.Angle[1], 0.0f, ObjectSelect.Angle);
		}
		else if (Type == MODEL_POTION + 21)
			Scale = 0.002f;
		else if (Type == MODEL_BOW + 19)
			Scale = 0.002f;
		else if (Type == MODEL_EVENT + 11)
			Scale = 0.0015f;
		else if (Type == MODEL_HELPER + 4)
			Scale = 0.0015f;
		else if (Type == MODEL_HELPER + 5)
			Scale = 0.005f;
		else if (Type == MODEL_HELPER + 30)
			Scale = 0.002f;
		else if (Type == MODEL_EVENT + 16)
			Scale = 0.002f;
		else if (Type == MODEL_HELPER + 16)
			Scale = 0.002f;
		else if (Type == MODEL_HELPER + 17)
			Scale = 0.0018f;
		else if (Type == MODEL_HELPER + 18)
			Scale = 0.0018f;
		else if (Type == MODEL_HELPER + 46)
		{
			Scale = 0.0018f;
		}
		else if (Type == MODEL_HELPER + 47)
		{
			Scale = 0.0018f;
		}
		else if (Type == MODEL_HELPER + 48)
		{
			Scale = 0.0018f;
		}
		else if (Type == MODEL_POTION + 54)
		{
			Scale = 0.0024f;
		}
		else if (Type == MODEL_POTION + 58)
		{
			Scale = 0.0012f;
		}
		else if (Type == MODEL_POTION + 59 || Type == MODEL_POTION + 60)
		{
			Scale = 0.0010f;
		}
		else if (Type == MODEL_POTION + 61 || Type == MODEL_POTION + 62)
		{
			Scale = 0.0009f;
		}
		else if (Type == MODEL_POTION + 53)
		{
			Scale = 0.00078f;
		}
		else if (Type == MODEL_HELPER + 43 || Type == MODEL_HELPER + 44 || Type == MODEL_HELPER + 45)
		{
			Scale = 0.0021f;
		}
		else if (Type >= MODEL_POTION + 70 && Type <= MODEL_POTION + 71)
		{
			Scale = 0.0028f;
		}
		else if (Type >= MODEL_POTION + 72 && Type <= MODEL_POTION + 77)
		{
			Scale = 0.0025f;
		}
		else if (Type == MODEL_HELPER + 59)
		{
			Scale = 0.0008f;
		}
		else if (Type >= MODEL_HELPER + 54 && Type <= MODEL_HELPER + 58)
		{
			Scale = 0.004f;
		}
		else if (Type >= MODEL_POTION + 78 && Type <= MODEL_POTION + 82)
		{
			Scale = 0.0025f;
		}
		else if (Type == MODEL_HELPER + 60)
		{
			Scale = 0.005f;
		}
		else if (Type == MODEL_HELPER + 61)
		{
			Scale = 0.0018f;
		}
		else if (Type == MODEL_POTION + 83)
		{
			Scale = 0.0009f;
		}
		else if (Type == MODEL_HELPER + 43 || Type == MODEL_HELPER + 44 || Type == MODEL_HELPER + 45)
		{
			Scale = 0.0021f;
		}
		else if (Type == MODEL_POTION + 91)
		{
			Scale = 0.0034f;
		}
		else if (Type == MODEL_POTION + 92)
		{
			Scale = 0.0024f;
		}
		else if (Type == MODEL_POTION + 93)
		{
			Scale = 0.0024f;
		}
		else if (Type == MODEL_POTION + 95)
		{
			Scale = 0.0024f;
		}
		else if (Type == MODEL_POTION + 94)
		{
			Scale = 0.0022f;
		}
		else if (Type == MODEL_POTION + 84)
		{
			Scale = 0.0031f;
		}
		else if (Type == MODEL_POTION + 85)
		{
			Scale = 0.0044f;
		}
		else if (Type == MODEL_POTION + 86)
		{
			Scale = 0.0031f;
		}
		else if (Type == MODEL_POTION + 87)
		{
			Scale = 0.0061f;
		}
		else if (Type == MODEL_POTION + 88)
		{
			Scale = 0.0035f;
		}
		else if (Type == MODEL_POTION + 89)
		{
			Scale = 0.0035f;
		}
		else if (Type == MODEL_POTION + 90)
		{
			Scale = 0.0035f;
		}
		else if (Type >= MODEL_HELPER + 62 && Type <= MODEL_HELPER + 63)
		{
			Scale = 0.002f;
		}
		else if (Type >= MODEL_POTION + 97 && Type <= MODEL_POTION + 98)
		{
			Scale = 0.003f;
		}
		else if (Type == MODEL_POTION + 96)
		{
			Scale = 0.0028f;
		}
		else if (MODEL_HELPER + 64 == Type || Type == MODEL_HELPER + 65)
		{
			switch (Type)
			{
			case MODEL_HELPER + 64: Scale = 0.0005f; break;
			case MODEL_HELPER + 65: Scale = 0.0016f; break;
			}
		}
		else if (Type == MODEL_HELPER + 67)
		{
			Scale = 0.0015f;
		}
		else if (Type == MODEL_HELPER + 80)
		{
			Scale = 0.0020f;
		}
		else if (Type == MODEL_HELPER + 68)
		{
			Scale = 0.0026f;
		}
		else if (Type == MODEL_HELPER + 76)
		{
			Scale = 0.0026f;
		}
		else if (Type == MODEL_HELPER + 69)
		{
			Scale = 0.0023f;
		}
		else if (Type == MODEL_HELPER + 70)
		{
			Scale = 0.0018f;
		}
		else if (Type == MODEL_HELPER + 81)
			Scale = 0.0012f;
		else if (Type == MODEL_HELPER + 82)
			Scale = 0.0012f;
		else if (Type == MODEL_HELPER + 93)
			Scale = 0.0021f;
		else if (Type == MODEL_HELPER + 94)
			Scale = 0.0021f;
		else if (Type == MODEL_SWORD + 19)
		{
			if (ItemLevel >= 0)
			{
				Scale = 0.0025f;
			}
			else
			{
				Scale = 0.001f;
			}
		}
		else if (Type == MODEL_STAFF + 10)
		{
			if (ItemLevel >= 0)
			{
				Scale = 0.0019f;
			}
			else
			{
				Scale = 0.001f;
			}
		}
		else if (Type == MODEL_BOW + 18)
		{
			if (ItemLevel >= 0)
			{
				Scale = 0.0025f;
			}
			else
			{
				Scale = 0.0015f;
			}
		}
		else if (Type >= MODEL_MACE + 8 && Type <= MODEL_MACE + 11)
		{
			Scale = 0.003f;
		}
		else if (Type == MODEL_MACE + 12)
		{
			Scale = 0.0025f;
		}
		else if (Type == MODEL_MACE + 18)
		{
			Scale = 0.0024f;
		}
		else if (Type == MODEL_EVENT + 12)
		{
			Scale = 0.0012f;
		}
		else if (Type == MODEL_EVENT + 13)
		{
			Scale = 0.0025f;
		}
		else if (Type == MODEL_EVENT + 14)
		{
			Scale = 0.0028f;
		}
		else if (Type == MODEL_EVENT + 15)
		{
			Scale = 0.0023f;
		}
		else if (Type >= MODEL_POTION + 22 && Type < MODEL_POTION + 25)
		{
			Scale = 0.0025f;
		}
		else if (Type >= MODEL_POTION + 25 && Type < MODEL_POTION + 27)
		{
			Scale = 0.0028f;
		}
		else if (Type == MODEL_POTION + 63)
		{
			Scale = 0.007f;
		}
		else if (Type == MODEL_POTION + 99)
		{
			Scale = 0.0025f;
		}
		else if (Type == MODEL_POTION + 52)
		{
			Scale = 0.0014f;
		}
		else if (Type == MODEL_HELPER + 38)
		{
			Scale = 0.0025f;
		}
		else if (Type == MODEL_POTION + 41)
		{
			Scale = 0.0035f;
		}
		else if (Type == MODEL_POTION + 42)
		{
			Scale = 0.005f;
		}
		else if (Type == MODEL_POTION + 43)
		{
			Position[1] += -0.005f;
			Scale = 0.0035f;
		}
		else if (Type == MODEL_POTION + 44)
		{
			Position[1] += -0.005f;
			Scale = 0.004f;
		}
		else if (Type == MODEL_POTION + 7)
		{
			Scale = 0.0025f;
		}
		else if (Type == MODEL_HELPER + 43 || Type == MODEL_HELPER + 44 || Type == MODEL_HELPER + 45)
		{
			Scale = 0.0021f;
		}
		else if (Type == MODEL_HELPER + 7)
		{
			Scale = 0.0025f;
		}
		else if (Type == MODEL_HELPER + 11)
		{
			Scale = 0.0025f;
		}
		else if (Type == MODEL_HELPER + 32)
		{
			Scale = 0.0019f;
		}
		else if (Type == MODEL_HELPER + 33)
		{
			Scale = 0.004f;
		}
		else if (Type == MODEL_HELPER + 34)
		{
			Scale = 0.004f;
		}
		else if (Type == MODEL_HELPER + 35)
		{
			Scale = 0.004f;
		}
		else if (Type == MODEL_HELPER + 36)
		{
			Scale = 0.007f;
		}
		else if (Type == MODEL_HELPER + 37)
		{
			Scale = 0.005f;
		}
		else if (Type == MODEL_BOW + 21)
		{
			Scale = 0.0022f;
		}
		else if (Type == MODEL_HELPER + 49)
		{
			Scale = 0.0013f;
		}
		else if (Type == MODEL_HELPER + 50)
		{
			Scale = 0.003f;
		}
		else if (Type == MODEL_HELPER + 51)
		{
			Scale = 0.003f;
		}
		else if (Type == MODEL_POTION + 64)
		{
			Scale = 0.003f;
		}
		else if (Type == MODEL_POTION + 65)
			Scale = 0.003f;
		else if (Type == MODEL_POTION + 66)
			Scale = 0.0035f;
		else if (Type == MODEL_POTION + 67)
			Scale = 0.0035f;
		else if (Type == MODEL_POTION + 68)
			Scale = 0.003f;
		else if (Type == MODEL_HELPER + 52)
			Scale = 0.005f;
		else if (Type == MODEL_HELPER + 53)
			Scale = 0.005f;
		else if (Type == MODEL_SWORD + 24)
		{
			Scale = 0.0028f;
		}
		else if (Type == MODEL_BOW + 22)
		{
			Scale = 0.0020f;
		}
		else if (Type == MODEL_BOW + 23)
		{
			Scale = 0.0032f;
		}
		else if (Type == MODEL_HELPER + 14 || Type == MODEL_HELPER + 15)
		{
			Scale = 0.003f;
		}
		else if (Type == MODEL_POTION + 100)
		{
			Scale = 0.0040f;
		}
		else if (Type >= MODEL_POTION && Type < MODEL_POTION + MAX_ITEM_INDEX)
		{
			Scale = 0.0035f;
		}
		else if (Type >= MODEL_SPEAR && Type < MODEL_SPEAR + MAX_ITEM_INDEX)
		{
			if (Type == MODEL_SPEAR + 10)
				Scale = 0.0018f;
			else if (Type == MODEL_SPEAR + 11)
				Scale = 0.0025f;
			else
				Scale = 0.0021f;
		}
		else if (Type >= MODEL_STAFF && Type < MODEL_STAFF + MAX_ITEM_INDEX)
		{
			if (Type >= MODEL_STAFF + 14 && Type <= MODEL_STAFF + 20)
				Scale = 0.0028f;
			else if (Type >= MODEL_STAFF + 21 && Type <= MODEL_STAFF + 29)
				Scale = 0.004f;
			else if (Type == MODEL_STAFF + 33)
				Scale = 0.0028f;
			else if (Type == MODEL_STAFF + 34)
				Scale = 0.0028f;
			else
				Scale = 0.0022f;
		}
		else if (Type == MODEL_BOW + 15)
			Scale = 0.0011f;
		else if (Type == MODEL_BOW + 7)
			Scale = 0.0012f;
		else if (Type == MODEL_EVENT + 6)
			Scale = 0.0039f;
		else if (Type == MODEL_EVENT + 8)
			Scale = 0.0015f;
		else if (Type == MODEL_EVENT + 9)
			Scale = 0.0019f;
		else
		{
			Scale = 0.0025f;
		}

		if (Type >= MODEL_TYPE_CHARM_MIXWING + EWS_BEGIN
			&& Type <= MODEL_TYPE_CHARM_MIXWING + EWS_END)
		{
			Scale = 0.0020f;
		}

		if (Type == MODEL_EVENT + 10)
		{
			Scale = 0.001f;
		}
		else if (Type >= MODEL_ETC + 19 && Type <= MODEL_ETC + 27)
		{
			Scale = 0.0023f;
		}
		else if (Type == MODEL_HELPER + 66)
		{
			Scale = 0.0020f;
		}
		else if (Type == MODEL_POTION + 140)
		{
			Scale = 0.0026f;
		}
		else if (Type == MODEL_HELPER + 122)
		{
			Scale = 0.0033f;
		}
		else if (Type == MODEL_HELPER + 123)
		{
			Scale = 0.0009f;
		}
		else if (Type >= MODEL_POTION + 145 && Type <= MODEL_POTION + 150)
		{
			Scale = 0.0018f;
		}
		else if (Type >= MODEL_HELPER + 125 && Type <= MODEL_HELPER + 127)
		{
			Scale = 0.0013f;
		}
		else if (Type == MODEL_HELPER + 128)
		{
			Scale = 0.0035f;
		}
		else if (Type == MODEL_HELPER + 129)
		{
			Scale = 0.0035f;
		}
		else if (Type == MODEL_HELPER + 134)
		{
			Scale = 0.0033f;
		}
		else if (Type == MODEL_HELPER + 130)
		{
			Scale = 0.0032f;
		}
		else if (Type == MODEL_HELPER + 131)
		{
			Scale = 0.0033f;
		}
		else if (Type == MODEL_HELPER + 132)
		{
			Scale = 0.0025f;
		}
		else if (Type == MODEL_HELPER + 133)
		{
			Scale = 0.0033f;
		}
		else if (Type == MODEL_HELPER + 71 || Type == MODEL_HELPER + 72 || Type == MODEL_HELPER + 73 || Type == MODEL_HELPER + 74 || Type == MODEL_HELPER + 75)
		{
			Scale = 0.0019f;
		}
		else if (Type == MODEL_BOW + 24)
		{
			Scale = 0.0023f;
		}
		else if (Type == MODEL_POTION + 110)
		{
			Scale = 0.004f;
		}
		else if (Type == MODEL_HELPER + 107)
		{
			Scale = 0.0034f;
		}
		else if (Type == MODEL_POTION + 133)
		{
			Scale = 0.0030f;
		}
		else if (Type == MODEL_HELPER + 105)
		{
			Scale = 0.002f;
		}
		else if (MODEL_POTION + 101 <= Type && Type <= MODEL_POTION + 109)
		{
			switch (Type)
			{
			case MODEL_POTION + 101:
				{
					Scale = 0.004f;
				}break;
			case MODEL_POTION + 102:
				{
					Scale = 0.005f;
				}break;
			case MODEL_POTION + 103:
			case MODEL_POTION + 104:
			case MODEL_POTION + 105:
			case MODEL_POTION + 106:
			case MODEL_POTION + 107:
			case MODEL_POTION + 108:
				{
					Scale = 0.004f;
				}break;
			case MODEL_POTION + 109:
				{
					Scale = 0.003f;
				}break;
			}
		}
		else if (Type == MODEL_HELPER + 106)
		{
			Scale = 0.0015f;
		}
		else if (Type == MODEL_WING + 130)
		{
			Scale = 0.0012f;
		}
		else if (Type >= MODEL_POTION + 134 && Type <= MODEL_POTION + 139)
		{
			Scale = 0.0050f;
		}
		else if (Type >= MODEL_HELPER + 109 && Type <= MODEL_HELPER + 112)
		{
			Scale = 0.0045f;
		}
		else if (Type >= MODEL_HELPER + 113 && Type <= MODEL_HELPER + 115)
		{
			Scale = 0.0018f;
		}
		else if (Type >= MODEL_POTION + 112 && Type <= MODEL_POTION + 113)
		{
			Scale = 0.0032f;
		}
		else if (Type == MODEL_HELPER + 116)
		{
			Scale = 0.0021f;
		}
		else if (Type >= MODEL_POTION + 114 && Type <= MODEL_POTION + 119)
		{
			Scale = 0.0038f;
		}
		else if (Type >= MODEL_POTION + 126 && Type <= MODEL_POTION + 129)
		{
			Scale = 0.0038f;
		}
		else if (Type >= MODEL_POTION + 130 && Type <= MODEL_POTION + 132)
		{
			Scale = 0.0038f;
		}
		else if (Type == MODEL_HELPER + 121)
		{
			Scale = 0.0018f;
		}
		else if (Type == MODEL_HELPER + 124)
			Scale = 0.0018f;
	}

	MUThreadInfo &ThreadInfo = g_ThreadInfo[0];
	b->Animation(0, ThreadInfo.BoneTransform, ObjectSelect.AnimationFrame, ObjectSelect.PriorAnimationFrame, ObjectSelect.PriorAction, ObjectSelect.Angle, ObjectSelect.HeadAngle, false, false);
	ThreadInfo.SkeletalIndex = g_Tasking.RegisterSkeletal(ThreadInfo.BoneTransform, b->_BoneNonDummyEnd);
	ObjectSelect.BoneTransform = &g_Tasking._SkeletonBuffer[ThreadInfo.SkeletalIndex];
	
	if (ThreadInfo.SkeletalIndex == FAILED_SKELETAL)
	{
		return;
	}

	CHARACTER Armor;
	OBJECT *o = &Armor.Object;
	o->Type = Type;
	ItemObjectAttribute(o);
	o->Flags.Clear(OBJECT::eOF_LIGHTENABLE);
	Armor.Class = 2;

	if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_PARTCHARGE_SHOP) == true)
	{
#if 0 // FIX
		mu_float ChangeScale = (640.0f / static_cast<mu_float>(TheShell().GetWindowSize().x))*3.7f;
		o->Scale = Scale - (Scale / ChangeScale);
#endif
	}
	else
	{
		o->Scale = Scale;
	}

	VectorCopy(Position, o->Position);

	EVector3 Light;
	mu_float alpha = o->Alpha;

	Vector(1.0f, 1.0f, 1.0f, Light);

	RenderPartObject(0, o, Type, nullptr, Light, alpha, ItemLevel, ExcOption, SetOption, true, true, true);
}

void RenderItem3D(mu_float sx, mu_float sy, mu_float Width, mu_float Height, mu_int32 Type, mu_int32 Level, mu_int32 ExcOption, mu_int32 SetOption, mu_boolean PickUp)
{
	mu_boolean Success = false;
	if ((g_pPickedItem.GetItem() == nullptr || PickUp)
		&& CheckMouseIn2(sx, sy, Width, Height))
	{
		if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_PARTCHARGE_SHOP))
		{
			Success = true;
		}
		else
		{
			Success = true;
		}
	}

	if (Type >= ITEM_SWORD && Type < ITEM_SWORD + MAX_ITEM_INDEX)
	{
		sx += Width*0.8f;
		sy += Height*0.85f;
	}
	else if (Type >= ITEM_AXE && Type < ITEM_MACE + MAX_ITEM_INDEX)
	{
		if (Type == ITEM_MACE + 13)
		{
			sx += Width*0.6f;
			sy += Height*0.5f;
		}
		else
		{
			sx += Width*0.8f;
			sy += Height*0.7f;
		}
	}
	else if (Type >= ITEM_SPEAR && Type < ITEM_SPEAR + MAX_ITEM_INDEX)
	{
		sx += Width*0.6f;
		sy += Height*0.65f;
	}
	else if (Type == ITEM_BOW + 17)
	{
		sx += Width*0.5f;
		sy += Height*0.5f;
	}
	else if (Type == ITEM_BOW + 19)
	{
		sx += Width*0.7f;
		sy += Height*0.75f;
	}
	else if (Type == ITEM_BOW + 20)
	{
		sx += Width*0.5f;
		sy += Height*0.55f;
	}
	else if (Type >= ITEM_BOW + 8 && Type < ITEM_BOW + MAX_ITEM_INDEX)
	{
		sx += Width*0.7f;
		sy += Height*0.7f;
	}
	else if (Type >= ITEM_STAFF && Type < ITEM_STAFF + MAX_ITEM_INDEX)
	{
		sx += Width*0.6f;
		sy += Height*0.55f;
	}
	else if (Type >= ITEM_SHIELD && Type < ITEM_SHIELD + MAX_ITEM_INDEX)
	{
		sx += Width*0.5f;
		if (Type == ITEM_SHIELD + 15)
			sy += Height*0.7f;
		else if (Type == ITEM_SHIELD + 16)
			sy += Height*0.9f;
		else if (Type == ITEM_SHIELD + 21)
		{
			sx += Width*0.05f;
			sy += Height*0.5f;
		}
		else
			sy += Height*0.6f;
	}
	else if (Type >= ITEM_HELM && Type < ITEM_HELM + MAX_ITEM_INDEX)
	{
		sx += Width*0.5f;
		sy += Height*0.8f;
	}
	else if (Type >= ITEM_ARMOR && Type < ITEM_ARMOR + MAX_ITEM_INDEX)
	{
		sx += Width*0.5f;
		if (Type == ITEM_ARMOR + 2 || Type == ITEM_ARMOR + 4 || Type == ITEM_ARMOR + 6)
		{
			sy += Height*1.05f;
		}
		else if (Type == ITEM_ARMOR + 3 || Type == ITEM_ARMOR + 8)
		{
			sy += Height*1.1f;
		}
		else if (Type == ITEM_ARMOR + 17 || Type == ITEM_ARMOR + 18 || Type == ITEM_ARMOR + 20)
		{
			sy += Height*0.8f;
		}
		else if (Type == ITEM_ARMOR + 15)
		{
			sy += Height * 1.0f;
		}
		else
		{
			sy += Height*0.8f;
		}
	}
	else if (Type >= ITEM_PANTS && Type < ITEM_BOOTS + MAX_ITEM_INDEX)
	{
		sx += Width*0.5f;
		sy += Height*0.9f;
	}
	else if (Type == ITEM_HELPER + 14 && Level == 1)
	{
		sx += Width*0.55f;
		sy += Height*0.85f;
	}
	else if (Type == ITEM_HELPER + 14 || Type == ITEM_HELPER + 15)
	{
		sx += Width*0.6f;
		sy += Height*1.0f;
	}
	else if (Type == ITEM_HELPER + 16 || Type == ITEM_HELPER + 17)
	{
		sx += Width*0.5f;
		sy += Height*0.9f;
	}
	else if (Type == ITEM_HELPER + 18)
	{
		sx += Width*0.5f;
		sy += Height*0.75f;
	}
	else if (Type == ITEM_HELPER + 19)
	{
		switch(Level)
		{
		case 0: sx += Width*0.5f; sy += Height*0.5f; break;
		case 1: sx += Width*0.7f; sy += Height*0.8f; break;
		case 2: sx += Width*0.7f; sy += Height*0.7f; break;
		}
	}
	else if (Type == ITEM_HELPER + 20)
	{
		switch(Level)
		{
		case 0: sx += Width*0.5f; sy += Height*0.65f; break;
		case 1:
		case 2:
		case 3:
			sx += Width*0.5f; sy += Height*0.8f; break;
		}
	}
	else if (Type == ITEM_HELPER + 29)
	{
		sx += Width*0.5f;
		sy += Height*0.5f;
	}
	else if (Type == ITEM_HELPER + 4)
	{
		sx += Width*0.5f;
		sy += Height*0.6f;
	}
	else if (Type == ITEM_HELPER + 30)
	{
		sx += Width*0.5f;
		sy += Height*0.5f;
	}
	else if (Type == ITEM_HELPER + 31)
	{
		sx += Width*0.5f;
		sy += Height*0.9f;
	}
	else if (Type == ITEM_POTION + 7)
	{
		sx += Width*0.5f;
		sy += Height*0.5f;
	}
	else if (Type == ITEM_HELPER + 7)
	{
		sx += Width*0.5f;
		sy += Height*0.9f;
	}
	else if (Type == ITEM_HELPER + 11)
	{
		switch(Level)
		{
		case 0: sx += Width*0.5f; sy += Height*0.8f; break;
		case 1: sx += Width*0.5f; sy += Height*0.5f; break;
		}
	}
	else if (Type == ITEM_HELPER + 32)
	{
		sx += Width*0.5f;
		sy += Height*0.5f;
	}
	else if (Type == MODEL_HELPER + 33)
	{
		sx += Width*0.5f;
		sy += Height*0.5f;
	}
	else if (Type == MODEL_HELPER + 34)
	{
		sx += Width*0.5f;
		sy += Height*0.5f;
	}
	else if (Type == MODEL_HELPER + 35)
	{
		sx += Width*0.5f;
		sy += Height*0.5f;
	}
	else if (Type == MODEL_HELPER + 36)
	{
		sx += Width*0.5f;
		sy += Height*0.5f;
	}
	else if (Type == MODEL_HELPER + 37)
	{
		sx += Width*0.5f;
		sy += Height*0.5f;
	}
	else if (Type >= ITEM_HELPER && Type < ITEM_HELPER + MAX_ITEM_INDEX)
	{
		sx += Width*0.5f;
		sy += Height*0.7f;
	}
	else if (Type == ITEM_POTION + 12)
	{
		sx += Width*0.5f;
		sy += Height*0.5f;
	}
	else if (Type == ITEM_POTION + 11 && (Level == 3 || Level == 13))
	{
		sx += Width*0.5f;
		sy += Height*0.5f;
	}
	else if (Type == ITEM_POTION + 11 && (Level == 14 || Level == 15))
	{
		sx += Width*0.5f;
		sy += Height*0.8f;
	}
	else if (Type == ITEM_POTION + 9 && Level == 1)
	{
		sx += Width*0.5f;
		sy += Height*0.8f;
	}
	else if (Type == ITEM_POTION + 17 || Type == ITEM_POTION + 18 || Type == ITEM_POTION + 19)
	{
		sx += Width*0.5f;
		sy += Height*0.5f;
	}
	else if (Type == ITEM_POTION + 21)
	{
		switch(Level)
		{
		case 0: sx += Width*0.5f; sy += Height*0.5f; break;
		case 1: sx += Width*0.4f; sy += Height*0.8f; break;
		case 3: sx += Width*0.5f; sy += Height*0.5f; break;
		}
	}
	else if (Type >= ITEM_POTION + 22 && Type < ITEM_POTION + 25)
	{
		if (Type == ITEM_POTION + 24 && Level == 1)
		{
			sx += Width*0.5f;
			sy += Height*0.8f;
		}
		else
		{
			sx += Width*0.5f;
			sy += Height*0.95f;
		}
	}
	else if (Type >= ITEM_POTION + 46 && Type <= ITEM_POTION + 48)
	{
		sx += Width*0.5f;
		sy += Height*0.5f;
	}
	else if (Type >= ITEM_POTION + 25 && Type < ITEM_POTION + 27)
	{
		sx += Width*0.5f;
		sy += Height*0.9f;
	}
	else if (Type == ITEM_POTION + 31)
	{
		sx += Width*0.5f;
		sy += Height*0.5f;
	}
	else if (Type == INDEX_COMPILED_CELE || Type == INDEX_COMPILED_SOUL)
	{
		sx += Width*0.55f;
		sy += Height*0.8f;
	}
	else if (Type == ITEM_WING + 3)
	{
		sx += Width*0.5f;
		sy += Height*0.45f;
	}
	else if (Type == ITEM_WING + 4)
	{
		sx += Width*0.5f;
		sy += Height*0.4f;
	}
	else if (Type == ITEM_WING + 5)
	{
		sx += Width*0.5f;
		sy += Height*0.75f;
	}
	else if (Type == ITEM_WING + 6)
	{
		sx += Width*0.5f;
		sy += Height*0.55f;
	}
	else if (Type == ITEM_POTION + 100)
	{
		sx += Width*0.49f;
		sy += Height*0.28f;
	}
	else if (Type >= ITEM_POTION && Type < ITEM_POTION + MAX_ITEM_INDEX)
	{
		sx += Width*0.5f;
		sy += Height*0.95f;
	}
	else if ((Type >= ITEM_WING + 12 && Type <= ITEM_WING + 14) || (Type >= ITEM_WING + 16 && Type <= ITEM_WING + 19))
	{
		sx += Width*0.5f;
		sy += Height*0.75f;
	}
	else if (Type == ITEM_HELPER + 66)
	{
		sx += Width*1.5f;
		sy += Height*1.5f;
	}
	else
	{
		sx += Width*0.5f;
		sy += Height*0.6f;
	}

	EVector3 Position;
	EVideoBackend::Unproject2D(sx, sy, MU_RESOURCE->_640x480Rate[0], MU_RESOURCE->_640x480Rate[1], MousePosition, Position);

	if (Type == ITEM_POTION + 11 && Level == 1)
	{
		RenderObjectScreen(MODEL_EVENT + 4, Level, ExcOption, SetOption, Position, Success, PickUp);
	}
	else if (Type == ITEM_POTION + 11 && Level == 2)
	{
		RenderObjectScreen(MODEL_EVENT + 5, Level, ExcOption, SetOption, Position, Success, PickUp);
	}
	else if (Type == ITEM_POTION + 11 && Level == 3)
	{
		RenderObjectScreen(MODEL_EVENT + 6, Level, ExcOption, SetOption, Position, Success, PickUp);
	}
	else if (Type == ITEM_POTION + 11 && Level == 5)
	{
		RenderObjectScreen(MODEL_EVENT + 8, Level, ExcOption, SetOption, Position, Success, PickUp);
	}
	else if (Type == ITEM_POTION + 11 && Level == 6)
	{
		RenderObjectScreen(MODEL_EVENT + 9, Level, ExcOption, SetOption, Position, Success, PickUp);
	}
	else if (Type == ITEM_POTION + 11 && 8 <= Level && Level <= 12)
	{
		RenderObjectScreen(MODEL_EVENT + 10, Level, ExcOption, SetOption, Position, Success, PickUp);
	}
	else if (Type == ITEM_POTION + 11 && Level == 13)
	{
		RenderObjectScreen(MODEL_EVENT + 6, Level, ExcOption, SetOption, Position, Success, PickUp);
	}
	else if (Type == ITEM_POTION + 11 && (Level == 14 || Level == 15))
	{
		RenderObjectScreen(MODEL_EVENT + 5, Level, ExcOption, SetOption, Position, Success, PickUp);
	}
	else if (Type == ITEM_HELPER + 14 && Level == 1)
	{
		RenderObjectScreen(MODEL_EVENT + 16, Level, ExcOption, SetOption, Position, Success, PickUp);
	}
	else if (Type == ITEM_POTION + 9 && Level == 1)
	{
		RenderObjectScreen(MODEL_EVENT + 7, Level, ExcOption, SetOption, Position, Success, PickUp);
	}
	else if (Type == ITEM_POTION + 21)
	{
		switch (Level)
		{
		case 1:
			RenderObjectScreen(MODEL_EVENT + 11, Level, ExcOption, SetOption, Position, Success, PickUp);
			break;
		case 3:
			RenderObjectScreen(Type + MODEL_ITEM, Level, ExcOption, SetOption, Position, Success, PickUp);
			break;
		default:
			RenderObjectScreen(Type + MODEL_ITEM, Level, ExcOption, SetOption, Position, Success, PickUp);
			break;
		}
	}
	else if (Type == ITEM_POTION + 45)
	{
		RenderObjectScreen(MODEL_POTION + 45, Level, ExcOption, SetOption, Position, Success, PickUp);
	}
	else if (Type >= ITEM_POTION + 46 && Type <= ITEM_POTION + 48)
	{
		RenderObjectScreen(MODEL_POTION + 46, Level, ExcOption, SetOption, Position, Success, PickUp);
	}
	else if (Type == ITEM_POTION + 49)
	{
		RenderObjectScreen(MODEL_POTION + 49, Level, ExcOption, SetOption, Position, Success, PickUp);
	}
	else if (Type == ITEM_POTION + 50)
	{
		RenderObjectScreen(MODEL_POTION + 50, Level, ExcOption, SetOption, Position, Success, PickUp);
	}
	else if (Type == ITEM_POTION + 32)
	{
		switch (Level)
		{
		case 0:
			RenderObjectScreen(MODEL_POTION + 32, Level, ExcOption, SetOption, Position, Success, PickUp);
			break;
		case 1:
			RenderObjectScreen(MODEL_EVENT + 21, Level, ExcOption, SetOption, Position, Success, PickUp);
			break;
		}
	}
	else if (Type == ITEM_POTION + 33)
	{
		switch (Level)
		{
		case 0:
			RenderObjectScreen(MODEL_POTION + 33, Level, ExcOption, SetOption, Position, Success, PickUp);
			break;
		case 1:
			RenderObjectScreen(MODEL_EVENT + 22, Level, ExcOption, SetOption, Position, Success, PickUp);
			break;
		}
	}
	else if (Type == ITEM_POTION + 34)
	{
		switch (Level)
		{
		case 0:
			RenderObjectScreen(MODEL_POTION + 34, Level, ExcOption, SetOption, Position, Success, PickUp);
			break;
		case 1:
			RenderObjectScreen(MODEL_EVENT + 23, Level, ExcOption, SetOption, Position, Success, PickUp);
			break;
		}
	}
	else if (Type == ITEM_HELPER + 19)
	{
		switch (Level)
		{
		case 0:
			RenderObjectScreen(MODEL_STAFF + 10, -1, ExcOption, SetOption, Position, Success, PickUp);
			break;
		case 1:
			RenderObjectScreen(MODEL_SWORD + 19, -1, ExcOption, SetOption, Position, Success, PickUp);
			break;
		case 2:
			RenderObjectScreen(MODEL_BOW + 18, -1, ExcOption, SetOption, Position, Success, PickUp);
			break;
		}
	}
	else if (Type == ITEM_POTION + 23)
	{
		switch (Level)
		{
		case 0:
			RenderObjectScreen(Type + MODEL_ITEM, Level, ExcOption, SetOption, Position, Success, PickUp);
			break;
		case 1:
			RenderObjectScreen(MODEL_EVENT + 12, -1, ExcOption, SetOption, Position, Success, PickUp);
			break;
		}
	}
	else if (Type == ITEM_POTION + 24)
	{
		switch (Level)
		{
		case 0:
			RenderObjectScreen(Type + MODEL_ITEM, Level, ExcOption, SetOption, Position, Success, PickUp);
			break;
		case 1:
			RenderObjectScreen(MODEL_EVENT + 13, -1, ExcOption, SetOption, Position, Success, PickUp);
			break;
		}
	}
	else if (Type == ITEM_HELPER + 20)
	{
		switch (Level)
		{
		case 0:
			RenderObjectScreen(MODEL_EVENT + 15, Level, ExcOption, SetOption, Position, Success, PickUp);
			break;
		case 1:
		case 2:
		case 3:
			RenderObjectScreen(MODEL_EVENT + 14, Level, ExcOption, SetOption, Position, Success, PickUp);
			break;
		}
	}
	else if (Type == ITEM_HELPER + 11 && Level == 1)
	{
		RenderObjectScreen(MODEL_EVENT + 18, Level, ExcOption, SetOption, Position, Success, PickUp);
	}
#if 0 // FIX
	else if (Type == ITEM_POTION + 100)
	{
		mu_boolean _Angle;
		if (g_pLuckyCoinRegistration->GetItemRotation())
		{
			_Angle = true;
		}
		else
		{
			_Angle = Success;
		}

		RenderObjectScreen(MODEL_POTION + 100, Level, ExcOption, SetOption, Position, _Angle, PickUp);
	}
#endif
	else
	{
		RenderObjectScreen(Type + MODEL_ITEM, Level, ExcOption, SetOption, Position, Success, PickUp);
	}
}