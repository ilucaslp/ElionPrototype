#ifndef __MU_OPENGL_SHADER_H__
#define __MU_OPENGL_SHADER_H__

#pragma once

#if OPENGL_BACKEND_ENABLE == 1
namespace EOpenGLShader
{
	const mu_boolean Create(const mu_boolean earlyShaders);
	void DestroyNonEarly();
	void Destroy();

	void SetProgramShader(const ECombinedShader programShader);
};
#endif

#endif