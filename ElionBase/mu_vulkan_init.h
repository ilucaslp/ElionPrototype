#ifndef __MU_VULKAN_INIT_H__
#define __MU_VULKAN_INIT_H__

#pragma once

namespace EVulkanInit
{
	const mu_boolean Initialize(SDL_Window *SDLWindow);
	void Destroy(const mu_boolean gpuOnly);

	const mu_boolean CreateInstance(SDL_Window *SDLWindow);
	void DestroyInstance();

	const mu_boolean CreateDebugCallback();
	void DestroyDebugCallback();

	const mu_boolean CreateSurface(SDL_Window *SDLWindow);
	void DestroySurface();

	const mu_boolean FindPhysicalDevice();

	const mu_boolean CreateLogicalDevice();
	void DestroyLogicalDevice();

	const mu_boolean CreateSync();
	void DestroySync();

	const mu_boolean GetSurfaceCapabilities();
	const mu_boolean GetSurfaceFormat();

	void UpdateSamplesCount(const VkSampleCountFlags DesiredSamples);
	const mu_boolean UpdateSwapchainSize(SDL_Window *SDLWindow);

	const mu_boolean CreateSwapchain();
	void DestroySwapchain();

	const mu_boolean CreateSwapchainView();
	void DestroySwapchainView();

	const mu_boolean CreateColor();
	void DestroyColor();

	const mu_boolean CreateDepthStencil();
	void DestroyDepthStencil();

	const mu_boolean CreateCommandPool();
	const mu_boolean CreateCommandBuffers();
	const mu_boolean CreateCommandFences();
	void DestroyCommandPool();
	void DestroyCommandBuffers();
	void DestroyCommandFences();

	const mu_boolean CreateFramebuffers();
	void DestroyFramebuffers();
};

#endif