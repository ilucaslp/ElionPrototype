#ifndef __MU_METAL_CONFIG_H__
#define __MU_METAL_CONFIG_H__

#pragma once

#define METAL_BACKEND_ENABLE (0)
#define METAL_REVERSED_DEPTH (1)
#define METAL_INFINITE_PERSPECTIVE (0)
#if METAL_REVERSED_DEPTH == 1
#define METAL_DEPTH_CLEAR (0.0f)
#else
#define METAL_DEPTH_CLEAR (1.0f)
#endif
#define METAL_STENCIL_CLEAR (0)

#endif