#include "stdafx.h"

#if OPENGL_BACKEND_ENABLE == 1
namespace EOpenGLDebug
{
	void DebugCallback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar *message,
		const void *userParam)
	{
#if defined(__ANDROID__)
#if OPENGL_BACKEND_NONKHR == 1
		const mu_char *sourceMsg = "";
		const mu_char *severityMsg = "";

		switch (source)
		{
		case GL_DEBUG_SOURCE_API: sourceMsg = "Source API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceMsg = "Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceMsg = "Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY: sourceMsg = "Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION: sourceMsg = "Application"; break;
		case GL_DEBUG_SOURCE_OTHER: sourceMsg = "Other"; break;
		case GL_DONT_CARE: sourceMsg = "NA"; break;
		}

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_LOW: severityMsg = "Low"; break;
		case GL_DEBUG_SEVERITY_MEDIUM: severityMsg = "Medium"; break;
		case GL_DEBUG_SEVERITY_HIGH: severityMsg = "High"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: severityMsg = "Notification"; break;
		case GL_DONT_CARE: sourceMsg = "NA"; break;
		}

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			{
				__android_log_print(ANDROID_LOG_ERROR, "Elion-OpenGLES", "[Error][%s][%s] %s\n", sourceMsg, severityMsg, message);
			}
			break;

		case GL_DEBUG_TYPE_PORTABILITY:
		case GL_DEBUG_TYPE_PERFORMANCE:
		case GL_DEBUG_TYPE_MARKER:
			{
				__android_log_print(ANDROID_LOG_WARN, "Elion-OpenGLES", "[Warning][%s][%s] %s\n", sourceMsg, severityMsg, message);
			}
			break;

		case GL_DEBUG_TYPE_OTHER:
		default:
			{
				__android_log_print(ANDROID_LOG_INFO, "Elion-OpenGLES", "[Info][%s][%s] %s\n", sourceMsg, severityMsg, message);
			}
			break;
		}
#else
		const mu_char *sourceMsg = "";
		const mu_char *severityMsg = "";

		switch (source)
		{
		case GL_DEBUG_SOURCE_API_KHR: sourceMsg = "Source API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM_KHR: sourceMsg = "Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER_KHR: sourceMsg = "Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY_KHR: sourceMsg = "Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION_KHR: sourceMsg = "Application"; break;
		case GL_DEBUG_SOURCE_OTHER_KHR: sourceMsg = "Other"; break;
		case GL_DONT_CARE: sourceMsg = "NA"; break;
		}

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_LOW_KHR: severityMsg = "Low"; break;
		case GL_DEBUG_SEVERITY_MEDIUM_KHR: severityMsg = "Medium"; break;
		case GL_DEBUG_SEVERITY_HIGH_KHR: severityMsg = "High"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION_KHR: severityMsg = "Notification"; break;
		case GL_DONT_CARE: sourceMsg = "NA"; break;
		}

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR_KHR:
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_KHR:
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_KHR:
			{
				__android_log_print(ANDROID_LOG_ERROR, "Elion-OpenGLES", "[Error][%s][%s] %s\n", sourceMsg, severityMsg, message);
			}
			break;

		case GL_DEBUG_TYPE_PORTABILITY_KHR:
		case GL_DEBUG_TYPE_PERFORMANCE_KHR:
		case GL_DEBUG_TYPE_MARKER_KHR:
			{
				__android_log_print(ANDROID_LOG_WARN, "Elion-OpenGLES", "[Warning][%s][%s] %s\n", sourceMsg, severityMsg, message);
			}
			break;

		case GL_DEBUG_TYPE_OTHER_KHR:
		default:
			{
				__android_log_print(ANDROID_LOG_INFO, "Elion-OpenGLES", "[Info][%s][%s] %s\n", sourceMsg, severityMsg, message);
			}
			break;
		}
#endif
#else
		const mu_char *sourceMsg = "NA";
		const mu_char *severityMsg = "NA";
		const mu_char *typeMsg = "";

#if OPENGL_BACKEND_MALIGLES == 1
		switch (source)
		{
		case GL_DEBUG_SOURCE_API_KHR: sourceMsg = "Source API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM_KHR: sourceMsg = "Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER_KHR: sourceMsg = "Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY_KHR: sourceMsg = "Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION_KHR: sourceMsg = "Application"; break;
		case GL_DEBUG_SOURCE_OTHER_KHR: sourceMsg = "Other"; break;
		}

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_LOW_KHR: severityMsg = "Low"; break;
		case GL_DEBUG_SEVERITY_MEDIUM_KHR: severityMsg = "Medium"; break;
		case GL_DEBUG_SEVERITY_HIGH_KHR: severityMsg = "High"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION_KHR: severityMsg = "Notification"; break;
		}

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR_KHR:
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_KHR:
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_KHR:
			{
				typeMsg = "Error";
			}
			break;

		case GL_DEBUG_TYPE_PORTABILITY_KHR:
		case GL_DEBUG_TYPE_PERFORMANCE_KHR:
		case GL_DEBUG_TYPE_MARKER_KHR:
			{
				typeMsg = "Warning";
			}
			break;

		case GL_DEBUG_TYPE_OTHER_KHR:
		default:
			{
				typeMsg = "Info";
			}
			break;
		}
#else
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: sourceMsg = "Source API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceMsg = "Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceMsg = "Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY: sourceMsg = "Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION: sourceMsg = "Application"; break;
		case GL_DEBUG_SOURCE_OTHER: sourceMsg = "Other"; break;
		}

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_LOW: severityMsg = "Low"; break;
		case GL_DEBUG_SEVERITY_MEDIUM: severityMsg = "Medium"; break;
		case GL_DEBUG_SEVERITY_HIGH: severityMsg = "High"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: severityMsg = "Notification"; break;
		}

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			{
				typeMsg = "Error";
			}
			break;

		case GL_DEBUG_TYPE_PORTABILITY:
		case GL_DEBUG_TYPE_PERFORMANCE:
		case GL_DEBUG_TYPE_MARKER:
			{
				typeMsg = "Warning";
			}
			break;

		case GL_DEBUG_TYPE_OTHER:
		default:
			{
				typeMsg = "Info";
			}
			break;
		}
#endif

		mu_asciistr output;
		output += "[";
		output += typeMsg;
		output += "]";
		output += "[";
		output += sourceMsg;
		output += "]";
		output += "[";
		output += severityMsg;
		output += "] ";
		output.append(message, length);
		output += '\n';

		OutputDebugStringA(output.c_str());
#endif
	}
};
#endif