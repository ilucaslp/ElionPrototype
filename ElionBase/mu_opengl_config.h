#ifndef __MU_OPENGL_CONFIG_H__
#define __MU_OPENGL_CONFIG_H__

#pragma once

#define OPENGL_BACKEND_ENABLE (1)

#define OPENGL_MAX_TEXTURES (8)
#define OPENGL_BACKEND_BUFFERCOUNT (3) // 1 : Normal, 2 : Double buffering, 3 : Triple buffering

#define OPENGL_REVERSED_DEPTH (0)
#define OPENGL_INFINITE_PERSPECTIVE (0)
#if OPENGL_REVERSED_DEPTH == 1
#define OPENGL_DEPTH_CLEAR (0.0f)
#else
#define OPENGL_DEPTH_CLEAR (1.0f)
#endif
#define OPENGL_STENCIL_CLEAR (0)

#if !defined(NDEBUG)
#define OPENGL_BACKEND_ERRORREPORT (1)
#define OPENGL_BACKEND_DEBUG (1)
#else
#define OPENGL_BACKEND_ERRORREPORT (0)
#define OPENGL_BACKEND_DEBUG (0)
#endif

#if defined(__ANDROID__)
#define OPENGL_BACKEND_MALIGLES (0)
#define OPENGL_BACKEND_ANDROID (1)
#else
#define OPENGL_BACKEND_MALIGLES (0)
#define OPENGL_BACKEND_ANDROID (0)
#endif

#define OPENGL_INVALID_ENUM (0xFFFFFFFF)

#endif