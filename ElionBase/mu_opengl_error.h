#ifndef __MU_OPENGL_ERROR_H__
#define __MU_OPENGL_ERROR_H__

#pragma once

namespace EOpenGLError
{
	EINLINE const mu_boolean ReportCheck()
	{
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
#if defined(__ANDROID__)
			__android_log_print(ANDROID_LOG_INFO, "OpenGL", "[Error] %u (0x%04X)\n", error, error);
#else
			std::stringstream debugMessage;
			debugMessage << "[Error] %u (0x%04X)" << error << error << std::endl;
			OutputDebugStringA(debugMessage.str().c_str());
#endif
		}

		return error == GL_NO_ERROR;
	}
};

#endif