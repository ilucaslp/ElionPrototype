#include "stdafx.h"

#if DIRECTX_BACKEND_ENABLE == 1
namespace EDirectXDebug
{
	ID3D11Debug *DebugContext = nullptr;
};
#endif