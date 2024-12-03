#ifndef __MU_OPENGL_DEBUG_H__
#define __MU_OPENGL_DEBUG_H__

#pragma once

namespace EOpenGLDebug
{
	void DebugCallback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar *message,
		const void *userParam);

	EINLINE void Initialize()
	{
		if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::Debug) == true)
		{
#if OPENGL_BACKEND_NONKHR == 1
			glEnable(GL_DEBUG_OUTPUT);
#if OPENGL_BACKEND_MALIGLES == 0
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
#else
			glEnable(GL_DEBUG_OUTPUT_KHR);
#if OPENGL_BACKEND_MALIGLES == 0
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR);
#endif
#endif

			glDebugMessageCallback(DebugCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
	}

	EINLINE void ReportCompileShader(GLuint shader)
	{
		GLint logSize = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
		if (logSize > 1)
		{
			mu_char *logInfo = new mu_char[logSize];
			if (logInfo != nullptr)
			{
				glGetShaderInfoLog(shader, logSize, &logSize, logInfo);

#if defined(__ANDROID__)
				__android_log_print(ANDROID_LOG_INFO, "Elion-OpenGL", "[Compile-Error][%u] %s\n", shader, logInfo);
#else
				std::stringstream debugMessage;
				debugMessage << "[Compile-Error][" << shader << "] %s" << logInfo << std::endl;
				OutputDebugStringA(debugMessage.str().c_str());
#endif

				delete[] logInfo;
			}
		}
	}

	EINLINE void ReportLinkProgram(GLuint program)
	{
		GLint logSize = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		if (logSize > 1)
		{
			mu_char *logInfo = new mu_char[logSize];
			if (logInfo != nullptr)
			{
				glGetProgramInfoLog(program, logSize, &logSize, logInfo);

#if defined(__ANDROID__)
				__android_log_print(ANDROID_LOG_INFO, "Elion-OpenGL", "[Link-Error][%u] %s\n", program, logInfo);
#else
				std::stringstream debugMessage;
				debugMessage << "[Link-Error][" << program << "] %s" << logInfo << std::endl;
				OutputDebugStringA(debugMessage.str().c_str());
#endif

				delete[] logInfo;
			}
		}
	}

	EINLINE const mu_boolean ReportValidateProgram(GLuint program)
	{
		glValidateProgram(program);

		GLint isValidated = 0;
		glGetProgramiv(program, GL_VALIDATE_STATUS, &isValidated);
		if (isValidated == GL_FALSE)
		{
#ifndef NDEBUG
			GLint logSize = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
			if (logSize > 1)
			{
				mu_char *logInfo = new mu_char[logSize];
				if (logInfo != nullptr)
				{
					glGetProgramInfoLog(program, logSize, &logSize, logInfo);

#if defined(__ANDROID__)
					__android_log_print(ANDROID_LOG_INFO, "Elion-OpenGL", "[Validate-Error][%u] %s\n", program, logInfo);
#else
					std::stringstream debugMessage;
					debugMessage << "[Validate-Error][" << program << "] %s" << logInfo << std::endl;
					OutputDebugStringA(debugMessage.str().c_str());
#endif

					delete[] logInfo;
				}
			}
#endif

			glDeleteProgram(program);
			program = 0;
			return false;
		}

		return true;
	}
};

#endif