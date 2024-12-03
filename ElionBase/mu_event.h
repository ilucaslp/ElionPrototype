#ifndef __MU_EVENT_H__
#define __MU_EVENT_H__

#pragma once

#define XMAS_EVENT_TIME		60000
#define XMAS_MINIMUM_INDEX	50000
#define XMAS_MAXIMUM_INDEX	50050

class CXmasEvent
{
public:
	CXmasEvent();
	~CXmasEvent();

	void CreateXmasEventEffect(const mu_uint32 ThreadIndex, CHARACTER* pCha, OBJECT *pObj, mu_int32 iType);

	void GenID();

public:
	mu_int32 m_iEffectID;
};

extern CXmasEvent g_XmasEvent;

class CNewYearsDayEvent
{
public:
	CNewYearsDayEvent();
	~CNewYearsDayEvent();

	CHARACTER* CreateMonster(mu_int32 iType, mu_int32 iPosX, mu_int32 iPosY, mu_int32 iKey);
	mu_boolean MoveMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
};

extern CNewYearsDayEvent g_NewYearsDayEvent;

class CBlueLuckyBagEvent
{
public:
	CBlueLuckyBagEvent();
	virtual ~CBlueLuckyBagEvent();

	void StartBlueLuckyBag();

	void CheckTime();

	mu_boolean IsEnableBlueLuckyBag();

private:
	mu_boolean m_bBlueLuckyBag;
	mu_uint64 m_dwBlueLuckyBagTime;
	static const mu_uint64 m_dwBlueLuckyBagCheckTime;
};

extern CBlueLuckyBagEvent g_pBlueLuckyBagEvent;

class C09SummerEvent
{
public:
	C09SummerEvent();
	~C09SummerEvent();

	CHARACTER* CreateMonster(mu_int32 iType, mu_int32 iPosX, mu_int32 iPosY, mu_int32 iKey);
	mu_boolean MoveMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
};

extern C09SummerEvent g_09SummerEvent;

#endif