#ifndef __MU_DIRECTX_SHADER_H__
#define __MU_DIRECTX_SHADER_H__

#pragma once

#if DIRECTX_BACKEND_ENABLE == 1
namespace EDirectXShader
{
	const mu_boolean Create(const mu_boolean earlyShaders);
	void DestroyNonEarly();
	void Destroy();

	void SetProgramShader(const ECombinedShader programShader);
};
#endif

#endif