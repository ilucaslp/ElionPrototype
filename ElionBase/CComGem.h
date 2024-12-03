#ifndef _COMGEM_H_
#define _COMGEM_H_

typedef std::pair<mu_int32, mu_uint8> INTBYTEPAIR;

namespace COMGEM
{
	enum _METHOD
	{
		ATTACH = false,
		DETACH = true,
	};

	enum _GEMTYPE
	{
		NOGEM = -1,
		CELE = 0,
		SOUL = 1,
		COMCELE = 2,
		COMSOUL = 3,
	};

	enum _COMTYPE
	{
		NOCOM = -1,
		FIRST = 10,
		SECOND = 20,
		THIRD = 30,
	};

	enum _STATE
	{
		STATE_READY,
		STATE_HOLD,
		STATE_END,
	};

	enum _ERRORTYPE
	{
		NOERR = 0,
		POPERROR_NOTALLOWED,
		COMERROR_NOTALLOWED,
		DEERROR_SOMANY,
		DEERROR_NOTALLOWED,
		ERROR_UNKNOWN,
		ERROR_ALL,
	};

};

namespace COMGEM
{
	extern mu_boolean m_bType;
	extern mu_int8 m_cGemType;
	extern mu_int8 m_cComType;
	extern mu_uint8 m_cCount;
	extern mu_int32 m_iValue;
	extern mu_uint8 m_cPercent;
	extern mu_int8 m_cState;
	extern mu_int8 m_cErr;
#if 0 // FIX
	extern CUIUnmixgemList m_UnmixTarList;
#endif

	void Init();
	void Exit();
	void GetBack();
	void RenderUnMixList();
	void MoveUnMixList();
	void CalcGen();
	mu_int32 CalcItemValue(const ITEM*);
	mu_int8 CalcCompiledCount(const ITEM*);
	mu_int32 CalcEmptyInv();
	mu_int8 CheckOneItem(const ITEM*);
	mu_boolean CheckMyInvValid();
	mu_boolean CheckInv();

	mu_boolean FindWantedList();
	void ResetWantedList();
	void SelectFromList(mu_int32 iIndex, mu_int32 iLevel);
	mu_int32 GetUnMixGemLevel();

	void SendReqMix();
	void SendReqUnMix();
	void ProcessCSAction();

	EINLINE void SetMode(mu_boolean mode) { m_bType = mode; }
	EINLINE void SetGem(mu_int8 gem) { m_cGemType = gem; }
	EINLINE void SetComType(mu_int8 type) { m_cComType = type; }
	EINLINE void SetState(mu_int8 state) { m_cState = state; }
	EINLINE void SetError(mu_int8 err) { m_cErr = err; }
	EINLINE mu_int8 GetError() { return m_cErr; }
	EINLINE mu_boolean isComMode() { return (m_bType == ATTACH); }

	EINLINE mu_boolean isCompiledGem(const ITEM* p) { return (CheckOneItem(p) >= COMCELE); }
	EINLINE mu_boolean isAble() { return (m_cState == STATE_READY); }
};

#endif