#ifndef __MU_PETOBJECT_H__
#define __MU_PETOBJECT_H__

#pragma once

#include "mu_petaction.h"
#include "mu_petactionstand.h"
#include "mu_petactionround.h"
#include "mu_petactiondemon.h"
#include "mu_petactioncollecter.h"
#include "mu_petactioncollecteradd.h"
#include "mu_petactionunicorn.h"
#include "mu_petactioncollecterskeleton.h"

class MUPetObject
{
public:
	enum ActionType
	{
		eAction_Stand = 0,
		eAction_Move,
		eAction_Attack,
		eAction_Skill,
		eAction_Birth,
		eAction_Dead,
		eAction_End,
	};

public:
	MUPetObject();
	~MUPetObject();

	void Initialize();
	void Release();

	void Create(mu_int32 ItemType, mu_int32 ModelType, EVector3 &Position, CHARACTER *Owner, mu_int32 SubType, mu_int32 LinkBone);

	void Update(const mu_uint32 ThreadIndex, mu_boolean bForceRender = false);
	void Render(const mu_uint32 ThreadIndex, mu_boolean bForceRender = false);
	void CreateEffect(const mu_uint32 ThreadIndex, mu_uint64 Tick, mu_boolean bForceRender);

public:
	EINLINE OBJECT *GetObject()
	{
		return &_Object;
	}

	EINLINE mu_boolean IsSameOwner(OBJECT *Owner)
	{
		return Owner == _Object.Owner;
	}

	EINLINE mu_boolean IsSameObject(OBJECT *Owner, mu_int32 ItemType)
	{
		return Owner == _Object.Owner && ItemType == _ItemType;
	}

	EINLINE void SetActions(ActionType type, MUPetAction *action, mu_float speed)
	{
		_Actions[type] = action;
		_Speeds[type] = speed;
	}

	EINLINE void SetCommand(mu_int32 targetKey, ActionType cmdType)
	{
		_TargetKey = targetKey;
		_MoveType = cmdType;
	}

	EINLINE void SetScale(mu_float scale = 0.0f)
	{
		_Object.Scale = scale;
	}

	EINLINE void SetBlendMesh(mu_int32 blendMesh = -1)
	{
		_Object.BlendMesh = blendMesh;
	}

public:
	mu_boolean Live;

private:
	MUPetAction *_Actions[eAction_End];
	mu_float _Speeds[eAction_End];

	CHARACTER *_Owner;
	OBJECT _Object;
	mu_int32 _TargetKey;
	mu_int32 _ItemType;

	mu_uint64 _StartTick;

	ActionType _MoveType;
	ActionType _OldMoveType;
};

#endif