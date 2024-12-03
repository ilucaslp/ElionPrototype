#ifndef __MU_VIDEO_BACKEND_TYPES_H__
#define __MU_VIDEO_BACKEND_TYPES_H__

#pragma once

namespace EVideoBackend
{
	EINLINE const mu_text *BackendToString(const EVideoBackendType backendType)
	{
		switch(backendType)
		{
		case EVideoBackendType::eNone: return _T("None");
		case EVideoBackendType::eDirectX: return _T("DirectX");
		case EVideoBackendType::eOpenGL: return _T("OpenGL");
		case EVideoBackendType::eVulkan: return _T("Vulkan");
		case EVideoBackendType::eMetal: return _T("Metal");
		default: return _T("Unknown");
		};
	}
};

#endif