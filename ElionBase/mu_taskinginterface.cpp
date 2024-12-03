#include "stdafx.h"
#include "mu_tasking.h"

void MUTasking::CreateInterface()
{
	g_pMessageRoot = new UIMessageRoot();
	g_pOptionRoot = new UIOptionRoot();
	g_pNewUISystem = new UIGameRoot();
	g_pCharacterRoot = new UICharacterRoot();
	g_pLoginRoot = new UILoginRoot();
}

void MUTasking::DestroyInterface()
{
	MU_SAFE_DELETE(g_pLoginRoot);
	MU_SAFE_DELETE(g_pCharacterRoot);
	MU_SAFE_DELETE(g_pNewUISystem);
	MU_SAFE_DELETE(g_pOptionRoot);
	MU_SAFE_DELETE(g_pMessageRoot);
}