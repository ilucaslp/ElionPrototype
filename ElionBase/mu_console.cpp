#include "stdafx.h"
#include "mu_console.h"

#if defined(WIN32) || defined(WIN64)
#include <fcntl.h>
#include <io.h>

FILE *StreamFile = nullptr;
void RedirectIOToConsole()
{
	AllocConsole();

	freopen_s(&StreamFile, "CONOUT$", "w", stdout);
}
#endif