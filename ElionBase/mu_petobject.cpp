#include "stdafx.h"
#include "mu_petobject.h"

MUPetObject::MUPetObject()
{
	Initialize();
}

MUPetObject::~MUPetObject()
{
	
}

void MUPetObject::Initialize()
{
	mu_zeromem(_Actions, sizeof(_Actions));
	mu_zeromem(_Speeds, sizeof(_Speeds));

	_Owner = nullptr;
	_TargetKey = -1;
	_ItemType = -1;

	_StartTick = 0;

	_MoveType = eAction_Stand;
	_OldMoveType = eAction_End;

	_Object.Initialize();
}

void MUPetObject::Release()
{
	if (_MoveType >= 0 && _MoveType < eAction_End)
	{
		MUPetAction *petAction = _Actions[_MoveType];

		if (petAction != nullptr)
		{
			petAction->Release(&_Object, _Owner);
		}
	}
}

void MUPetObject::Create(mu_int32 ItemType, mu_int32 ModelType, EVector3 &Position, CHARACTER *Owner, mu_int32 SubType, mu_int32 LinkBone)
{
	_StartTick = (mu_uint64)g_CurrentTime;

	_Owner = Owner;
	_ItemType = ItemType;

	_Object.Flags.Clear();
	_Object.Flags.Set(OBJECT::eOF_LIVE | OBJECT::eOF_LIGHTENABLE);

	_Object.Type = ModelType;
	_Object.Owner = &_Owner->Object;
	_Object.SubType = SubType;
	_Object.HiddenMesh = -1;
	_Object.BlendMesh = -1;
	_Object.BlendMeshLight = 1.0f;
	_Object.Scale = 0.7f;
	_Object.LifeTime = 30;
	_Object.Alpha = 0.0f;
	_Object.AlphaTarget = 1.0f;

	VectorCopy(Position, _Object.Position);
	VectorCopy(_Object.Owner->Angle, _Object.Angle);
	Vector(3.0f, 3.0f, 3.0f, _Object.Light);

	_Object.PriorAnimationFrame = 0.0f;
	_Object.AnimationFrame = 0.0f;
	_Object.Velocity = 0.5f;

	switch (_Object.Type)
	{
	case MODEL_HELPER + 123:
		_Object.Position[1] += (60.0f * Owner->Object.Scale);
		break;
	}
}

void MUPetObject::Update(const mu_uint32 ThreadIndex, mu_boolean bForceRender)
{
	if (_Object.Flags.Get(OBJECT::eOF_LIVE) == false || nullptr == _Object.Owner) return;

	if (g_CurrentScene == MAIN_SCENE)
	{
		if (_Object.Owner->Flags.Get(OBJECT::eOF_LIVE) == false || _Object.Owner->Kind != KIND_PLAYER)
		{
			_Object.Flags.Clear(OBJECT::eOF_LIVE);
			return;
		}
	}

	Alpha(&_Object);

	mu_uint64 tick = g_CurrentTime - _StartTick;

	if (_MoveType < 0 || _MoveType >= eAction_End)
	{
		_MoveType = eAction_Stand;
	}

	MUPetAction *petAction = _Actions[_MoveType];

	if (petAction == nullptr)
	{
		_MoveType = eAction_Stand;
		petAction = _Actions[_MoveType];
	}

	if (_OldMoveType != _MoveType)
	{
		_Object.Velocity = _Speeds[_MoveType];
	}

	MUModel* b = &MODELS::Data[_Object.Type];
	b->CurrentAction(ThreadIndex) = _Object.CurrentAction;
	b->PlayAnimation(ThreadIndex, &_Object.AnimationFrame, &_Object.PriorAnimationFrame, &_Object.PriorAction, _Object.Velocity);

	if (petAction != nullptr)
	{
		petAction->Move(ThreadIndex, &_Object, _Owner, _TargetKey, tick, bForceRender);
		petAction->Sound(ThreadIndex, &_Object, _Owner, _TargetKey, tick, bForceRender);
	}
}

void MUPetObject::Render(const mu_uint32 ThreadIndex, mu_boolean bForceRender)
{
	if (_Object.Flags.Get(OBJECT::eOF_LIVE))
	{
		const mu_boolean isVisible = bForceRender == true || TestFrustrum2D(_Object.Position, 100.0f) == true;
		if (isVisible)
		{
			_Object.Flags.Set(OBJECT::eOF_VISIBLE);
		}
		else
		{
			_Object.Flags.Clear(OBJECT::eOF_VISIBLE);
		}

		if (isVisible == true)
		{
			mu_int32 State = g_isCharacterBuff(_Object.Owner, eBuff_Cloaking) ? 10 : 0;

			RenderObject(ThreadIndex, &_Object, false, 0, State);

			if (MU_UPDATECOUNTER > 0)
			{
				mu_uint64 tick = g_CurrentTime - _StartTick;
				CreateEffect(ThreadIndex, tick, bForceRender);
			}
		}
	}
}

void MUPetObject::CreateEffect(const mu_uint32 ThreadIndex, mu_uint64 Tick, mu_boolean bForceRender)
{
	MUPetAction *petAction = _Actions[_MoveType];

	if (petAction != nullptr)
	{
		petAction->Effect(ThreadIndex, &_Object, _Owner, _TargetKey, Tick, bForceRender);
	}
}