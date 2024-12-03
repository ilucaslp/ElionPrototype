#ifndef __MU_VULKAN_BACKEND_H__
#define __MU_VULKAN_BACKEND_H__

#pragma once

#if VULKAN_BACKEND_ENABLE == 1
#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#include <SDL_vulkan.h>
#if OPENGL_BACKEND_MALIGLES == 1 || OPENGL_BACKEND_ANDROID == 1
#include <ktx.h>
#endif

#include <vk_mem_alloc.h>

#include "mu_vulkan_loader.h"

#include "mu_vulkan_enum.h"
#include "mu_vulkan_common.h"
#include "mu_vulkan_error.h"
#include "mu_vulkan_initializer.h"
#include "mu_vulkan_global.h"
#include "mu_vulkan_memory.h"
#include "mu_vulkan_garbagecollector.h"
#include "mu_vulkan_binding.h"
#include "mu_vulkan_attributes.h"
#include "mu_vulkan_sampler.h"
#include "mu_vulkan_pipeline.h"
#include "mu_vulkan_shader.h"
#include "mu_vulkan_descriptors.h"
#include "mu_vulkan_skeleton.h"
#include "mu_vulkan_debug.h"
#include "mu_vulkan_texture.h"
#include "mu_vulkan_buffer.h"
#include "mu_vulkan_buffers.h"
#include "mu_vulkan_init.h"
#include "mu_vulkan_misc.h"
#endif

#endif