#ifndef __MU_DIRECTX_CONFIG_H__
#define __MU_DIRECTX_CONFIG_H__

#pragma once

#define DIRECTX_MAX_TEXTURES (8)

#define DIRECTX_BACKEND_BUFFERCOUNT (3) // 1 : Normal, 2 : Double buffering, 3 : Triple buffering

#if defined(_MSC_VER)
#define DIRECTX_BACKEND_ENABLE (1)
#else
#define DIRECTX_BACKEND_ENABLE (0)
#endif
#define DIRECTX_REVERSED_DEPTH (1)
#define DIRECTX_INFINITE_PERSPECTIVE (0)
#if DIRECTX_REVERSED_DEPTH == 1
#define DIRECTX_DEPTH_CLEAR (0.0f)
#else
#define DIRECTX_DEPTH_CLEAR (1.0f)
#endif
#define DIRECTX_STENCIL_CLEAR (0)
#if !defined(NDEBUG) || BACKEND_FORCE_DEBUGREPORT == 1
#define DIRECTX_BACKEND_DEBUG (1)
#else
#define DIRECTX_BACKEND_DEBUG (0)
#endif

#endif