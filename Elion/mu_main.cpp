// mu_main.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "mu_main.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

#ifdef USE_CRTLEAK
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif

#ifdef ENABLE_CRASHRPT
	CR_INSTALL_INFO crInfo;
	mu_zeromem(&crInfo, sizeof(crInfo));
	crInfo.cb = sizeof(CR_INSTALL_INFO);
	crInfo.pszAppName = ELION_APPNAME;
	crInfo.pszAppVersion = ELION_VERSION;
	crInfo.pszUrl = _T("http://crashreport.elion.mu/crashrpt.php");
	crInfo.uPriorities[CR_HTTP] = 1;
	crInfo.dwFlags |= CR_INST_ALL_POSSIBLE_HANDLERS;
	crInfo.dwFlags |= CR_INST_AUTO_THREAD_HANDLERS;
	//crInfo.pszPrivacyPolicyURL = _T("http://elion.mu/Home/Privacy");

	if (crInstall(&crInfo) != 0)
	{
		mu_text errorMessage[1024];
		crGetLastErrorMsg(errorMessage, mu_countof(errorMessage));
		mu_messagebox(nullptr, errorMessage, _T("Crash Report Failed"), MB_OK | MB_ICONERROR);
	}
#endif

	MURoot *root = new MURoot();

	if (root->Initialize() == true)
	{
		root->Run();
	}

	root->Destroy();
	delete root;

#ifdef ENABLE_CRASHRPT
	crUninstall();
#endif

	return 0;
}