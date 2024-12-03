#ifndef __MU_OPENGL_INIT_H__
#define __MU_OPENGL_INIT_H__

#pragma once

namespace EOpenGLInit
{
	const mu_boolean Initialize(SDL_Window *SDLWindow);
	void Destroy(const mu_boolean gpuOnly);

	void ConfigureFeatures();

	const mu_uint32 *GetVersion();
};

#endif