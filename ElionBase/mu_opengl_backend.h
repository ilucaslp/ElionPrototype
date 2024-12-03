#ifndef __MU_OPENGL_BACKEND_H__
#define __MU_OPENGL_BACKEND_H__

#pragma once

#if OPENGL_BACKEND_ENABLE == 1
#if OPENGL_BACKEND_MALIGLES == 1 || OPENGL_BACKEND_ANDROID == 1
#define KTX_OPENGL_ES3 1

#if OPENGL_BACKEND_ANDROID == 0
#define OPENGL_BACKEND_NONKHR (1)
#define GL_GLEXT_PROTOTYPES
#define ELION_FORCE_KTX 1
#include <GLES3\gl32.h>
#else
#include <GLES3\gl31.h>
#endif

#include <GLES2\gl2ext.h>
#include <ktx.h>

#if defined(_MSC_VER)
#pragma comment(lib, "libGLESv2.lib")
#pragma comment(lib, "libMaliEmulator.lib")
#pragma comment(lib, "libktx.gl.lib")
#endif
#else
#define OPENGL_BACKEND_NONKHR (1)

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL\GLU.h>

#ifndef NDEBUG
#pragma comment(lib, "glew32sd.lib")
#else
#pragma comment(lib, "glew32s.lib")
#endif
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "Glu32.lib")
#endif

#include "mu_opengl_enum.h"
#include "mu_opengl_extensions.h"
#include "mu_opengl_helpers.h"
#include "mu_opengl_buffer.h"
#include "mu_opengl_texture.h"
#include "mu_opengl_error.h"
#include "mu_opengl_debug.h"
#include "mu_opengl_attributes.h"
#include "mu_opengl_shader.h"
#include "mu_opengl_pipeline.h"
#include "mu_opengl_buffers.h"
#include "mu_opengl_init.h"
#include "mu_opengl_global.h"
#endif

#endif