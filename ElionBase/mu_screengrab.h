#ifndef __MU_SCREENGRAB_H__
#define __MU_SCREENGRAB_H__

#pragma once

#if DIRECTX_BACKEND_ENABLE == 1 && (defined(WIN32) || defined(WIN64))
#include <wincodec.h>

namespace MU
{
	HRESULT SaveWICTextureToFile(ID3D11DeviceContext *pContext,
		ID3D11Resource *pSource,
		REFGUID guidContainerFormat,
		const mu_text *filename,
		const GUID* targetFormat);
};
#endif

#endif