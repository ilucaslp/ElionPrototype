#ifndef __MU_BUFF_H__
#define __MU_BUFF_H__

#pragma once

#include "mu_buffstatesystem.h"

class MUBuff;
typedef std::shared_ptr<MUBuff> MUBuffPtr;

typedef std::map<eBuffState, mu_uint32> MUBuffStateMap;

class MUBuff
{
public:
	static MUBuffPtr Make();
	MUBuff();
	virtual ~MUBuff();

public:
	MUBuff& operator =(const MUBuff& buff);

public:
	void RegisterBuff(eBuffState buffstate);
	void RegisterBuff(std::list<eBuffState> buffstate);
	void UnRegisterBuff(eBuffState buffstate);
	void UnRegisterBuff(std::list<eBuffState> buffstate);

	mu_boolean isBuff();
	mu_boolean isBuff(eBuffState buffstate);
	const eBuffState isBuff(std::list<eBuffState> buffstatelist);

	void TokenBuff(eBuffState curbufftype);

public:
	const mu_uint32 GetBuffSize();
	const eBuffState GetBuff(mu_int32 iterindex);
	const mu_uint32 GetBuffCount(eBuffState buffstate);
	void ClearBuff();

public:
	MUBuffStateMap m_Buff;
};

inline MUBuff& MUBuff::operator =(const MUBuff& buff)
{
	m_Buff = buff.m_Buff;
	return *this;
}

mu_boolean CheckExceptionBuff(eBuffState buff, OBJECT* o, mu_boolean iserase);
void InsertBuffLogicalEffect(eBuffState buff, OBJECT* o, const mu_int32 bufftime);
void ClearBuffLogicalEffect(eBuffState buff, OBJECT* o);
void InsertBuffPhysicalEffect(const mu_uint32 ThreadIndex, eBuffState buff, OBJECT* o);
void ClearBuffPhysicalEffect(eBuffState buff, OBJECT* o);
void RegisterBuff(const mu_uint32 ThreadIndex, eBuffState buff, mu_boolean addEffect, OBJECT* o, const mu_int32 bufftime);
void UnRegisterBuff(eBuffState buff, OBJECT* o);

//Character Buff
#define g_isNotCharacterBuff( o ) \
	o->m_BuffMap.isBuff()

#define g_isCharacterBuff( o, bufftype ) \
	o->m_BuffMap.isBuff( bufftype )

#define g_isCharacterBufflist( o, bufftypelist ) \
	o->m_BuffMap.isBuff( bufftypelist )

#define g_TokenCharacterBuff( o, bufftype ) \
	o->m_BuffMap.TokenBuff( bufftype )

#define g_CharacterBuffCount( o, bufftype ) \
	o->m_BuffMap.GetBuffCount( bufftype )

#define g_CharacterBuffSize( o ) \
	o->m_BuffMap.GetBuffSize()

#define g_CharacterBuff( o, iterindex ) \
	o->m_BuffMap.GetBuff( iterindex )

#define g_CharacterRegisterBuff( o, bufftype ) \
	o->m_BuffMap.RegisterBuff( bufftype )

#define g_CharacterRegisterBufflist( o, bufftypelist ) \
	o->m_BuffMap.RegisterBuff( bufftypelist )

#define g_CharacterUnRegisterBuff( o, bufftype ) \
	o->m_BuffMap.UnRegisterBuff( bufftype )

#define g_CharacterUnRegisterBuffList( o, bufftypelist ) \
	o->m_BuffMap.UnRegisterBuff( bufftypelist )

#define g_CharacterCopyBuff( outObj, inObj ) \
	outObj->m_BuffMap.m_Buff = inObj->m_BuffMap.m_Buff

#define g_CharacterClearBuff( o ) \
	o->m_BuffMap.ClearBuff()

#define g_BuffInfo( buff ) \
	g_BuffSystem.GetBuffInfo().GetBuffinfo( buff )

#define g_IsBuffClass( buff ) \
	g_BuffSystem.GetBuffInfo().IsBuffClass( buff )

#define g_RegisterBuffTime( bufftype, curbufftime ) \
	g_BuffSystem.GetBuffTimeControl().RegisterBuffTime( bufftype, curbufftime )

#define g_UnRegisterBuffTime( bufftype ) \
	g_BuffSystem.GetBuffTimeControl().UnRegisterBuffTime( bufftype )

#define g_BuffStringTime( bufftype, timeText ) \
	g_BuffSystem.GetBuffTimeControl().GetBuffStringTime( bufftype, timeText )

#define g_StringTime( time, timeText, issecond ) \
	g_BuffSystem.GetBuffTimeControl().GetStringTime( time, timeText, issecond )

#define g_BuffStateValue( type ) \
	g_BuffSystem.GetBuffStateValueControl().GetValue( type )

#define g_BuffToolTipString( outstr, type ) \
	g_BuffSystem.GetBuffStateValueControl().GetBuffInfoString( outstr, type )

#define g_BuffStateValueString( outstr, type ) \
	g_BuffSystem.GetBuffStateValueControl().GetBuffValueString( outstr, type )

#endif