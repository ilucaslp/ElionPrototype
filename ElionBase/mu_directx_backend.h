#ifndef __MU_DIRECTX_BACKEND_H__
#define __MU_DIRECTX_BACKEND_H__

#pragma once

#if DIRECTX_BACKEND_ENABLE == 1
#include <d3d11.h>
#include <d3dcompiler.h>

#include "mu_directx_loader.h"
#include "mu_directx_helpers.h"
#include "mu_directx_attributes.h"
#include "mu_directx_texture.h"
#include "mu_directx_buffer.h"
#include "mu_directx_buffers.h"
#include "mu_directx_shader.h"
#include "mu_directx_dynamicbuffer.h"
#include "mu_directx_global.h"
#include "mu_directx_pipeline.h"
#include "mu_directx_init.h"
#endif

#endif