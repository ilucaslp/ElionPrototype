#ifndef __MU_BUFFSTATESYSTEM_H__
#define __MU_BUFFSTATESYSTEM_H__

#pragma once

#include "mu_buffscriptloader.h"
#include "mu_bufftimecontrol.h"
#include "mu_buffstatevaluecontrol.h"

class BuffStateSystem
{
public:
	BuffStateSystem();
	virtual ~BuffStateSystem();

public:
	BuffScriptLoader& GetBuffInfo()
	{
		return m_BuffInfo;
	}
	BuffTimeControl& GetBuffTimeControl()
	{
		return m_BuffTimeControl;
	}
	BuffStateValueControl& GetBuffStateValueControl()
	{
		return m_BuffStateValueControl;
	}

private:
	BuffScriptLoader m_BuffInfo;
	BuffTimeControl m_BuffTimeControl;
	BuffStateValueControl m_BuffStateValueControl;
};

extern BuffStateSystem g_BuffSystem;

#endif