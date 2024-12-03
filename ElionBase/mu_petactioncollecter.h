#ifndef __MU_PETACTIONCOLLECTER_H__
#define __MU_PETACTIONCOLLECTER_H__

#pragma once

#define SEARCH_LENGTH 300.0f
#define CIRCLE_STAND_RADIAN 50.0f
#define CIRCLE_GETITEM_RADIAN 50.0f

struct RootingItem
{
	mu_int32 itemIndex;
	EVector3 position;
};

class MUPetActionCollecter : public MUPetAction
{
public:
	MUPetActionCollecter();
	~MUPetActionCollecter();

	virtual mu_boolean Release(OBJECT* obj, CHARACTER *Owner) override;

	virtual mu_boolean Move(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender) override;
	virtual mu_boolean Effect(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender) override;
	virtual mu_boolean Sound(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender) override;

	void FindZen(OBJECT* obj);
	mu_boolean CompTimeControl(const mu_uint64& dwCompTime, mu_uint64& dwTime);

public:
	enum ActionState
	{
		eAction_Stand = 0,
		eAction_Move = 1,
		eAction_Get = 2,
		eAction_Return = 3,

		eAction_End_NotUse,
	};

private:
	RootingItem m_RootItem;
	mu_boolean m_isRooting;

	mu_uint64 m_dwSendDelayTime;
	mu_uint64 m_dwRootingTime;
	ActionState m_state;

	mu_float m_fRadWidthStand;
	mu_float m_fRadWidthGet;
};

extern MUPetActionCollecter g_PetActionCollecter;

#endif