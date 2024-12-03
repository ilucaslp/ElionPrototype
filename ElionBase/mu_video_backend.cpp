#include "stdafx.h"
#include "mu_screengrab.h"
#include <SDL_image.h>

namespace EVideoBackend
{
	struct ETextureLimit
	{
		mu_uint32 MaxSize;
		mu_uint32 MaxArraySize;
	};
	ETextureLimit TextureLimits[ETextureTypeMax] = {};

	EVideoBackendType CurrentBackend = EVideoBackendType::eNone;

	mu_uint32 TextureMaxSize = 0;
	mu_uint32 TextureMaxDepth = 0;
	mu_uint32 TextureMaxArray = 0;

	mu_uint32 VideoStatusFlag = EVideoStatusFlag::VSF_Active | EVideoStatusFlag::VSF_Focused | EVideoStatusFlag::VSF_Visible;
	mu_uint32 VideoRequestFlag = 0;

	void SetBackend(const EVideoBackendType backendType)
	{
		CurrentBackend = backendType;
	}

	const mu_text *GetBackendString()
	{
		return BackendToString(GetBackend());
	}

	void SetTextureCapabilities(const ETextureType type, const mu_uint32 maxSize, const mu_uint32 maxArraySize)
	{
		ETextureLimit &limit = TextureLimits[ECastEnum(type)];
		limit.MaxSize = maxSize;
		limit.MaxArraySize = maxArraySize;
	}

	const mu_uint32 GetTextureMaxSize(const ETextureType type)
	{
		return TextureLimits[ECastEnum(type)].MaxSize;
	}

	const mu_uint32 GetTextureMaxArray(const ETextureType type)
	{
		return TextureLimits[ECastEnum(type)].MaxArraySize;
	}

	const mu_boolean PrepareShaders()
	{
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				if (EDirectXShader::Create(false) == false)
				{
					return false;
				}
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				if (EVulkanShader::Create(false) == false)
				{
					return false;
				}

				if (EVulkanPipeline::CreatePipelines(ECombinedShader::eBeginLate, ECombinedShader::eEndLate) == false)
				{
					return false;
				}
			}
			break;
#endif

			default: break;
		}

		return true;
	}

	void RequestReloadVerticalSync()
	{
		SetVideoRequest(EVideoRequestFlag::VRF_ToggleVerticalSync);
	}

	void RequestResizeWindow()
	{
		SetVideoRequest(EVideoRequestFlag::VRF_ResizeWindow);
	}

	void RequestChangeWindowMode()
	{
		SetVideoRequest(EVideoRequestFlag::VRF_ToggleWindowMode);
	}

	void RequestReloadMSAA()
	{
		SetVideoRequest(EVideoRequestFlag::VRF_ToggleMSAA);
	}

	void RequestReloadShaders()
	{
		SetVideoRequest(EVideoRequestFlag::VRF_ReloadShader);
		if (EVideoBackend::GetBackend() == EVideoBackendType::eVulkan)
		{
			EVulkanShader::ClearCache();
		}
	}

	void RequestReloadShadowMap()
	{
		SetVideoRequest(EVideoRequestFlag::VRF_ToggleShadowMap);
	}

	void ClearReloadRequests()
	{
		VideoRequestFlag = 0;
	}

	const mu_boolean PreReload()
	{
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				if (GetVideoRequest(~EVideoRequestFlag::VRF_ToggleVerticalSync) == true)
				{
					EDirectXInit::DeviceContext->Flush();
				}
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				if (GetVideoRequest(~EVideoRequestFlag::VRF_ToggleVerticalSync) == true)
				{
					vkDeviceWaitIdle(EVulkan::Device);
				}
			}
			break;
#endif

		default: break;
		}

		return VideoRequestFlag != 0;
	}

	const mu_boolean ReloadWindowMode()
	{
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				if (GetVideoRequest(EVideoRequestFlag::VRF_ToggleWindowMode) == true)
				{
					SDL_SetWindowFullscreen(MU_WINDOW->GetSDLWindow(), MU_RESOURCE->IsWindowMode() == false ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
				}
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				if (GetVideoRequest(EVideoRequestFlag::VRF_ToggleWindowMode) == true)
				{
					SDL_SetWindowFullscreen(MU_WINDOW->GetSDLWindow(), MU_RESOURCE->IsWindowMode() == false ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
				}
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				if (GetVideoRequest(EVideoRequestFlag::VRF_ToggleWindowMode) == true)
				{
					SDL_SetWindowFullscreen(MU_WINDOW->GetSDLWindow(), MU_RESOURCE->IsWindowMode() == false ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
				}
			}
			break;
#endif

		default: break;
		}

		return false;
	}

	const mu_boolean ReloadWindowSize()
	{
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				if (GetVideoRequest(EVideoRequestFlag::VRF_ResizeWindow) == true &&
					MU_RESOURCE->IsWindowMode() == true)
				{
					const mu_uint32 Width = MU_RESOURCE->GetWindowWidth();
					const mu_uint32 Height = MU_RESOURCE->GetWindowHeight();

					SDL_DisplayMode mode = {};
					RetrieveCompatibleResolution(mode, Width, Height, true);

					SDL_SetWindowSize(MU_WINDOW->GetSDLWindow(), mode.w, mode.h);

					return true;
				}
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				if (GetVideoRequest(EVideoRequestFlag::VRF_ResizeWindow) == true &&
					MU_RESOURCE->IsWindowMode() == true)
				{
					const mu_uint32 Width = MU_RESOURCE->GetWindowWidth();
					const mu_uint32 Height = MU_RESOURCE->GetWindowHeight();

					SDL_DisplayMode mode = {};
					RetrieveCompatibleResolution(mode, Width, Height, true);

					SDL_SetWindowSize(MU_WINDOW->GetSDLWindow(), mode.w, mode.h);

					return true;
				}
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				if (GetVideoRequest(EVideoRequestFlag::VRF_ResizeWindow) == true &&
					MU_RESOURCE->IsWindowMode() == true)
				{
					const mu_uint32 Width = MU_RESOURCE->GetWindowWidth();
					const mu_uint32 Height = MU_RESOURCE->GetWindowHeight();

					SDL_DisplayMode mode = {};
					RetrieveCompatibleResolution(mode, Width, Height, true);

					SDL_SetWindowSize(MU_WINDOW->GetSDLWindow(), mode.w, mode.h);

					return true;
				}
			}
			break;
#endif

		default: break;
		}

		return false;
	}

	void Reload()
	{
		mu_boolean verticalSync = (GetVideoRequest(EVideoRequestFlag::VRF_ToggleVerticalSync) == true);
		mu_boolean resizeWindow = (GetVideoRequest(EVideoRequestFlag::VRF_ResizeWindow) == true);
		mu_boolean changeWindowMode = (GetVideoRequest(EVideoRequestFlag::VRF_ToggleWindowMode) == true);

		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				mu_boolean reloadMultisample = (GetVideoRequest(EVideoRequestFlag::VRF_ToggleMSAA) == true);
				mu_boolean reloadShaders = (GetVideoRequest(EVideoRequestFlag::VRF_ReloadShader) == true);
				mu_boolean reloadShadowMap = (GetVideoRequest(EVideoRequestFlag::VRF_ToggleShadowMap) == true);

				if (resizeWindow == true ||
					reloadMultisample == true)
				{
					EDirectXInit::DestroyDepthBuffer();
				}

				if (reloadMultisample == true)
				{
					EDirectXBuffer::DestroyMultisample();
					EDirectXBuffer::CreateMultisample();
				}

				if (resizeWindow == true)
				{
					EDirectXInit::DestroyRenderTargetView();
					EDirectXInit::Swapchain->ResizeBuffers(DIRECTX_BACKEND_BUFFERCOUNT, MU_RESOURCE->GetWindowWidth(), MU_RESOURCE->GetWindowHeight(), DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
					EDirectXInit::SwapchainDesc.BufferDesc.Width = MU_RESOURCE->GetWindowWidth();
					EDirectXInit::SwapchainDesc.BufferDesc.Height = MU_RESOURCE->GetWindowHeight();
					EDirectXInit::CreateRenderTargetView();
				}

				if (resizeWindow == true ||
					reloadMultisample == true)
				{
					EDirectXInit::CreateDepthBuffer();
				}

				if (reloadShaders == true)
				{
					EDirectXShader::DestroyNonEarly();
					EDirectXShader::Create(false);
				}

				if (reloadShadowMap == true)
				{
					EDirectXBuffer::DestroyShadowMap();
					EDirectXBuffer::CreateShadowMap();
				}

				if ((changeWindowMode == true ||
					 resizeWindow == true) &&
					MU_RESOURCE->IsWindowMode() == true)
				{
					SDL_SetWindowPosition(MU_WINDOW->GetSDLWindow(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
				}
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				mu_boolean reloadMultisample = (GetVideoRequest(EVideoRequestFlag::VRF_ToggleMSAA) == true);
				mu_boolean reloadShaders = (GetVideoRequest(EVideoRequestFlag::VRF_ReloadShader) == true);
				mu_boolean reloadShadowMap = (GetVideoRequest(EVideoRequestFlag::VRF_ToggleShadowMap) == true);

				if (verticalSync == true)
				{
					SDL_GL_SetSwapInterval(MU_RESOURCE->IsVerticalSync() == true ? 1 : 0);
				}

				if (reloadShaders == true)
				{
					EOpenGLShader::DestroyNonEarly();
					EOpenGLShader::Create(false);
				}

				if (reloadShadowMap == true)
				{
					EOpenGLBuffer::DestroyShadowMap();
					EOpenGLBuffer::CreateShadowMap();
				}

				if ((changeWindowMode == true ||
					 resizeWindow == true) &&
					MU_RESOURCE->IsWindowMode() == true)
				{
					SDL_SetWindowPosition(MU_WINDOW->GetSDLWindow(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
				}

				if (reloadMultisample == true)
				{
					EOpenGLBuffer::DestroyMultisample();
					EOpenGLBuffer::CreateMultisample();
				}
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				mu_boolean recreateSwapchain = (GetVideoRequest(EVideoRequestFlag::VRF_ResizeWindow) == true || GetVideoRequest(EVideoRequestFlag::VRF_ToggleWindowMode) == true);
				mu_boolean recreateColor = (GetVideoRequest(EVideoRequestFlag::VRF_ResizeWindow) == true || GetVideoRequest(EVideoRequestFlag::VRF_ToggleWindowMode) == true || GetVideoRequest(EVideoRequestFlag::VRF_ToggleMSAA) == true);
				mu_boolean recreateDepthBuffer = (GetVideoRequest(EVideoRequestFlag::VRF_ResizeWindow) == true || GetVideoRequest(EVideoRequestFlag::VRF_ToggleWindowMode) == true || GetVideoRequest(EVideoRequestFlag::VRF_ToggleMSAA) == true);
				mu_boolean recreateFramebuffer = (GetVideoRequest(EVideoRequestFlag::VRF_ResizeWindow) == true || GetVideoRequest(EVideoRequestFlag::VRF_ToggleWindowMode) == true || GetVideoRequest(EVideoRequestFlag::VRF_ToggleMSAA) == true);
				mu_boolean reloadRenderpass = (GetVideoRequest(EVideoRequestFlag::VRF_ResizeWindow) == true || GetVideoRequest(EVideoRequestFlag::VRF_ToggleWindowMode) == true || GetVideoRequest(EVideoRequestFlag::VRF_ToggleMSAA) == true);
				mu_boolean reloadPipeline = (GetVideoRequest(EVideoRequestFlag::VRF_ReloadShader) == true || reloadRenderpass == true);
				mu_boolean reloadShaders = (GetVideoRequest(EVideoRequestFlag::VRF_ReloadShader) == true);
				mu_boolean reloadShadowMap = (GetVideoRequest(EVideoRequestFlag::VRF_ToggleShadowMap) == true);

				if (reloadPipeline == true)
				{
					EVulkanPipeline::DestroyPipelines();
				}

				if (reloadRenderpass == true)
				{
					EVulkanPipeline::DestroyRenderPasses();
				}

				if (recreateFramebuffer == true)
				{
					EVulkanInit::UpdateSamplesCount(MU_RESOURCE->IsAntialiasing() ? VK_SAMPLE_COUNT_16_BIT : VK_SAMPLE_COUNT_1_BIT);
					EVulkanInit::DestroyFramebuffers();
				}

				if (recreateColor == true)
				{
					EVulkanInit::DestroyColor();
				}

				if (recreateDepthBuffer == true)
				{
					EVulkanInit::DestroyDepthStencil();
				}

				if (recreateSwapchain == true)
				{
					EVulkanInit::UpdateSwapchainSize(MU_WINDOW->GetSDLWindow());
					EVulkanInit::DestroySwapchainView();
					// No need to destroy swapchain, it is replaced calling CreateSwapchain function
					EVulkanInit::CreateSwapchain();
					EVulkanInit::CreateSwapchainView();
				}

				if (recreateColor == true)
				{
					EVulkanInit::CreateColor();
				}

				if (recreateDepthBuffer == true)
				{
					EVulkanInit::CreateDepthStencil();
				}

				if (reloadShaders == true)
				{
					EVulkanShader::DestroyNonEarly();
					EVulkanShader::Create(false);
				}

				if (reloadRenderpass == true)
				{
					EVulkanPipeline::CreateRenderPasses();
				}

				if (recreateFramebuffer == true)
				{
					EVulkanInit::CreateFramebuffers();
				}

				if (reloadPipeline == true)
				{
					EVulkanPipeline::CreatePipelines(ECombinedShader::eBeginEarly, ECombinedShader::eEndLate);
				}

				if (reloadShadowMap == true)
				{
					EVulkanBuffer::DestroyShadowMap();
					EVulkanBuffer::CreateShadowMap();
				}

				if ((changeWindowMode == true ||
					 resizeWindow == true) &&
					MU_RESOURCE->IsWindowMode() == true)
				{
					SDL_SetWindowPosition(MU_WINDOW->GetSDLWindow(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
				}
			}
			break;
#endif

			default: break;
		}

		if (resizeWindow == true)
		{
			mu_int32 ViewportOffsetX = MU_TO_VIEWPORT(MU_VIDEO->_3DViewport.OffsetX, MU_RESOURCE->_640x480Rate[0]);
			mu_int32 ViewportOffsetY = MU_TO_VIEWPORT(MU_VIDEO->_3DViewport.OffsetY, MU_RESOURCE->_640x480Rate[1]);
			mu_int32 ViewportWidth = MU_TO_VIEWPORT(MU_VIDEO->_3DViewport.Width, MU_RESOURCE->_640x480Rate[0]);
			mu_int32 ViewportHeight = MU_TO_VIEWPORT(MU_VIDEO->_3DViewport.Height, MU_RESOURCE->_640x480Rate[1]);

			MU_RESOURCE->CalculateAspectRatios();

			MU_VIDEO->_3DViewport.OffsetX = MU_TO_WINDOW(ViewportOffsetX, MU_RESOURCE->_640x480Rate[0]);
			MU_VIDEO->_3DViewport.OffsetY = MU_TO_WINDOW(ViewportOffsetY, MU_RESOURCE->_640x480Rate[1]);
			MU_VIDEO->_3DViewport.Width = MU_TO_WINDOW(ViewportWidth, MU_RESOURCE->_640x480Rate[0]);
			MU_VIDEO->_3DViewport.Height = MU_TO_WINDOW(ViewportHeight, MU_RESOURCE->_640x480Rate[1]);

			MU_VIDEO->ConfigureViewport2D();

			MUVideoManager::Prepare2DCamera();

			MU_ROOT->_FontFactory->CreateFonts();

			if (FONT_NORMAL != nullptr)
			{
				FONT_NORMAL->SystemConfigure();
			}
			if (FONT_BOLD != nullptr)
			{
				FONT_BOLD->SystemConfigure();
			}
			if (FONT_BIG != nullptr)
			{
				FONT_BIG->SystemConfigure();
			}
			if (FONT_NUMBER != nullptr)
			{
				FONT_NUMBER->SystemConfigure();
			}

			MU_INPUT->SystemConfigure();

			if (g_pMessageRoot != nullptr)
			{
				g_pMessageRoot->SystemConfigure();
			}

			if (g_pOptionRoot != nullptr)
			{
				g_pOptionRoot->SystemConfigure();
			}

			if (g_pLoginRoot != nullptr)
			{
				g_pLoginRoot->SystemConfigure();
			}

			if (g_pCharacterRoot != nullptr)
			{
				g_pCharacterRoot->SystemConfigure();
			}

			if (g_pNewUISystem != nullptr)
			{
				g_pNewUISystem->SystemConfigure();
			}
		}
	}

	const mu_uint32 GetBufferingCount()
	{
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			return DIRECTX_BACKEND_BUFFERCOUNT;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			return OPENGL_BACKEND_BUFFERCOUNT;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			return EVulkan::SwapchainImagesCount;
#endif

		default: break;
		}

		return 1;
	}
	
	const mu_uint32 GetCurrentBufferingIndex()
	{
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			return EDirectX::CurrentCommandBufferIndex;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			return EOpenGL::CurrentCommandBufferIndex;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			return EVulkan::CurrentCommandBufferIndex;
#endif

		default: break;
		}

		return 0;
	}

	const mu_boolean TakeScreenshot(const mu_text *filename)
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				ID3D11Texture2D *backBuffer = nullptr;
				HRESULT result = EDirectXInit::Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
				if (FAILED(result))
				{
					return false;
				}

				result = MU::SaveWICTextureToFile(EDirectXInit::DeviceContext, backBuffer, GUID_ContainerFormatJpeg, filename, nullptr);
				backBuffer->Release();

				if (FAILED(result))
				{
					return false;
				}
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				const mu_uint32 Components = 3;
				const mu_uint32 Width = MU_RESOURCE->GetWindowWidth();
				const mu_uint32 Height = MU_RESOURCE->GetWindowHeight();

				std::vector<mu_uint8> pixels(Width * Height * Components);
				glReadPixels(0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
				if (EOpenGLError::ReportCheck() == false)
				{
					return false;
				}

				SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormatFrom(&pixels[0], Width, Height, 1, Width * Components * sizeof(mu_uint8), SDL_PIXELFORMAT_RGB888);
				if (surface == nullptr)
				{
					return false;
				}

				SDL_RWops *fp = nullptr;
				if (mu_rwfromfile(&fp, filename, "wb") == false)
				{
					SDL_FreeSurface(surface);
					return false;
				}

				mu_int32 result = IMG_SaveJPG_RW(surface, fp, 0, 100);

				SDL_RWclose(fp);
				SDL_FreeSurface(surface);

				if(result < 0)
				{
					return false;
				}
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				const mu_uint32 Width = MU_RESOURCE->GetWindowWidth();
				const mu_uint32 Height = MU_RESOURCE->GetWindowHeight();

				if (!(EVulkan::SurfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT))
				{
					return false;
				}

				mu_boolean supportsBlit = true;

				VkFormatProperties formatProps = {};
				vkGetPhysicalDeviceFormatProperties(EVulkan::PhysicalDevice, EVulkan::SurfaceFormat.format, &formatProps);
				if(!(formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT))
				{
					supportsBlit = false;
				}

				vkGetPhysicalDeviceFormatProperties(EVulkan::PhysicalDevice, VK_FORMAT_R8G8B8A8_UNORM, &formatProps);
				if (!(formatProps.linearTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT))
				{
					supportsBlit = false;
				}

				const mu_uint32 lastSwapchainIndex = EVulkan::LastSwapchainIndex;

				VkImage srcImage = EVulkan::SwapchainImages[lastSwapchainIndex];

				VkImage dstImage = VK_NULL_HANDLE;
				VmaAllocation dstMemory = nullptr;
				VmaAllocationInfo dstMemoryInfo = {};

				if(EVulkanMemory::CreateImage(dstImage,
					dstMemory,
					dstMemoryInfo,
					VK_FORMAT_R8G8B8A8_UNORM,
					1, 1,
					VK_IMAGE_TYPE_2D,
					{ Width, Height, 1 },
					VK_IMAGE_USAGE_TRANSFER_DST_BIT,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
					VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_TO_CPU) == false)
				{
					return false;
				}

				VkCommandBuffer command = EVulkan::CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, EVulkan::ShortCommandPool, true);
				if (command == nullptr)
				{
					EVulkanMemory::DestroyImage(dstImage, dstMemory);
					return false;
				}

				VkImageSubresourceRange subresourceRange = {};
				subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				subresourceRange.baseMipLevel = 0;
				subresourceRange.levelCount = 1;
				subresourceRange.baseArrayLayer = 0;
				subresourceRange.layerCount = 1;
				EVulkan::SetImageLayout(command, dstImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);
				EVulkan::SetImageLayout(command, srcImage, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, subresourceRange, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

				if (supportsBlit)
				{
					// Define the region to blit (we will blit the whole swapchain image)
					VkOffset3D blitSize;
					blitSize.x = Width;
					blitSize.y = Height;
					blitSize.z = 1;
					VkImageBlit imageBlitRegion{};
					imageBlitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					imageBlitRegion.srcSubresource.layerCount = 1;
					imageBlitRegion.srcOffsets[1] = blitSize;
					imageBlitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					imageBlitRegion.dstSubresource.layerCount = 1;
					imageBlitRegion.dstOffsets[1] = blitSize;

					// Issue the blit command
					vkCmdBlitImage(
						command,
						srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
						dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						1,
						&imageBlitRegion,
						VK_FILTER_NEAREST);
				}
				else
				{
					// Otherwise use image copy (requires us to manually flip components)
					VkImageCopy imageCopyRegion{};
					imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					imageCopyRegion.srcSubresource.layerCount = 1;
					imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					imageCopyRegion.dstSubresource.layerCount = 1;
					imageCopyRegion.extent.width = Width;
					imageCopyRegion.extent.height = Height;
					imageCopyRegion.extent.depth = 1;

					// Issue the copy command
					vkCmdCopyImage(
						command,
						srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
						dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						1,
						&imageCopyRegion);
				}

				EVulkan::SetImageLayout(command, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL, subresourceRange, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);
				EVulkan::SetImageLayout(command, srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, subresourceRange, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

				if (EVulkan::FlushCommandBuffer<true>(command, EVulkan::ShortCommandPool, EVulkan::GraphicsQueue) == false)
				{
					EVulkanMemory::DestroyImage(dstImage, dstMemory);
					return false;
				}

				// Get layout of the image (including row pitch)
				VkImageSubresource subResource{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 0 };
				VkSubresourceLayout subResourceLayout;
				vkGetImageSubresourceLayout(EVulkan::Device, dstImage, &subResource, &subResourceLayout);

				// Map image memory so we can start copying from it
				mu_uint8 *srcData = nullptr;
				VkResult result = EVulkanMemory::MapMemory(dstMemory, reinterpret_cast<void**>(&srcData));
				if (result != VK_SUCCESS)
				{
					EVulkanMemory::DestroyImage(dstImage, dstMemory);
					return false;
				}

				srcData += subResourceLayout.offset;

				// If source is BGR (destination is always RGB) and we can't use blit (which does automatic conversion), we'll have to manually swizzle color components
				mu_boolean colorSwizzle = false;
				// Check if source is BGR 
				// Note: Not complete, only contains most common and basic BGR surface formats for demonstation purposes
				if (!supportsBlit)
				{
					std::vector<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
					colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), EVulkan::SurfaceFormat.format) != formatsBGR.end());
				}

				SDL_Surface *surface = nullptr;
				if (colorSwizzle)
				{
					switch(EVulkan::SurfaceFormat.format)
					{
					case VK_FORMAT_R8G8B8A8_UNORM:
						surface = SDL_CreateRGBSurfaceWithFormatFrom(srcData, Width, Height, 1, subResourceLayout.rowPitch, SDL_PIXELFORMAT_RGBA8888);
						break;

					case VK_FORMAT_B8G8R8A8_UNORM:
						surface = SDL_CreateRGBSurfaceWithFormatFrom(srcData, Width, Height, 1, subResourceLayout.rowPitch, SDL_PIXELFORMAT_BGRA8888);
						break;
					}
				}
				else
				{
					surface = SDL_CreateRGBSurfaceWithFormatFrom(srcData, Width, Height, 1, subResourceLayout.rowPitch, SDL_PIXELFORMAT_RGBA8888);
				}

				EVulkanMemory::UnmapMemory(dstMemory);
				EVulkanMemory::DestroyImage(dstImage, dstMemory);

				if (surface == nullptr)
				{
					return false;
				}

				SDL_RWops *fp = nullptr;
				if (mu_rwfromfile(&fp, filename, "wb") == false)
				{
					SDL_FreeSurface(surface);
					return false;
				}

				mu_int32 img_result = IMG_SaveJPG_RW(surface, fp, 0, 100);

				SDL_RWclose(fp);
				SDL_FreeSurface(surface);

				if (img_result < 0)
				{
					return false;
				}
			}
			break;
#endif

		default: break;
		}

		return true;
	}

	void WaitForDevice()
	{
		switch (GetBackend())
		{
#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				vkDeviceWaitIdle(EVulkan::Device);
			}
			break;
#endif

		default: break;
		}
	}

	void BeginFrame()
	{
		switch(GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectX::CurrentCommandBufferIndex = (EDirectX::CurrentCommandBufferIndex + 1) % DIRECTX_BACKEND_BUFFERCOUNT;
				MU_FONT->Update();
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGL::CurrentCommandBufferIndex = (EOpenGL::CurrentCommandBufferIndex + 1) % OPENGL_BACKEND_BUFFERCOUNT;
				MU_FONT->Update();
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false) break;

				VkResult result;

				if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Visible) == false)
				{
					EVulkan::CurrentCommandBufferIndex = (EVulkan::CurrentCommandBufferIndex + 1) % EVulkan::SwapchainImagesCount;
				}
				else
				{
					EVulkan::LastSwapchainIndex = EVulkan::CurrentSwapchainIndex;

					result = vkAcquireNextImageKHR(EVulkan::Device, EVulkan::Swapchain, UINT64_MAX, EVulkan::ImageAvailableSemaphore, VK_NULL_HANDLE, &EVulkan::CurrentSwapchainIndex);
					
					switch (result)
					{
					case VK_SUCCESS:
						{
							EVulkan::CurrentCommandBufferIndex = EVulkan::CurrentSwapchainIndex;
						}
						break;

					default:
						{
							EVulkan::CurrentCommandBufferIndex = (EVulkan::CurrentCommandBufferIndex + 1) % EVulkan::SwapchainImagesCount;
						}
						break;
					}
				}

				const mu_uint32 currentIndex = EVulkan::CurrentCommandBufferIndex;

				result = EVulkan::WaitAndResetFence(EVulkan::CommandFences[currentIndex]);
				if(result != VK_SUCCESS)
				{
					DEBUG_BREAK();
				}

				EVulkanGarbage::MaskProcess(currentIndex);

				if(EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Visible) == false)
				{
					VkCommandBuffer transferBuffer = EVulkan::TransferBuffers[currentIndex];
					result = vkResetCommandBuffer(transferBuffer, 0);
					if (result != VK_SUCCESS)
					{
						DEBUG_BREAK();
					}

					VkCommandBufferBeginInfo beginInfo = EVulkanInitializer::CommandBufferBeginInfo();
					result = vkBeginCommandBuffer(transferBuffer, &beginInfo);
					if (result != VK_SUCCESS)
					{
						DEBUG_BREAK();
					}
				}
				else
				{
					VkCommandBuffer transferBuffer = EVulkan::TransferBuffers[currentIndex];
					VkCommandBuffer shadowmapBuffer = EVulkan::ShadowMapBuffers[currentIndex];
					VkCommandBuffer commandBuffer = EVulkan::CommandBuffers[currentIndex];

					result = vkResetCommandBuffer(transferBuffer, 0);
					if (result != VK_SUCCESS)
					{
						DEBUG_BREAK();
					}

					if (MU_RESOURCE->GetShadowType() >= EShadowType::eShadowMappingBegin)
					{
						result = vkResetCommandBuffer(shadowmapBuffer, 0);
						if (result != VK_SUCCESS)
						{
							DEBUG_BREAK();
						}
					}

					result = vkResetCommandBuffer(commandBuffer, 0);
					if (result != VK_SUCCESS)
					{
						DEBUG_BREAK();
					}

					VkCommandBufferBeginInfo beginInfo = EVulkanInitializer::CommandBufferBeginInfo();
					result = vkBeginCommandBuffer(transferBuffer, &beginInfo);
					if (result != VK_SUCCESS)
					{
						DEBUG_BREAK();
					}

					if (MU_RESOURCE->GetShadowType() >= EShadowType::eShadowMappingBegin)
					{
						result = vkBeginCommandBuffer(shadowmapBuffer, &beginInfo);
						if (result != VK_SUCCESS)
						{
							DEBUG_BREAK();
						}
					}

					result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
					if (result != VK_SUCCESS)
					{
						DEBUG_BREAK();
					}
				}

				MU_FONT->Update();
			}
			break;
#endif

		default: break;
		}
	}

	void EndFrame()
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{

			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false) break;

				const mu_uint32 currentIndex = EVulkan::CurrentCommandBufferIndex;

				if(EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Visible) == false)
				{
					VkCommandBuffer transferBuffer = EVulkan::TransferBuffers[currentIndex];

					VkResult result = vkEndCommandBuffer(transferBuffer);
					if (result != VK_SUCCESS)
					{
						DEBUG_BREAK();
					}
				}
				else
				{
					VkCommandBuffer transferBuffer = EVulkan::TransferBuffers[currentIndex];
					VkCommandBuffer shadowmapBuffer = EVulkan::ShadowMapBuffers[currentIndex];
					VkCommandBuffer commandBuffer = EVulkan::CommandBuffers[currentIndex];

					VkResult result = vkEndCommandBuffer(transferBuffer);
					if (result != VK_SUCCESS)
					{
						DEBUG_BREAK();
					}

					if (MU_RESOURCE->GetShadowType() >= EShadowType::eShadowMappingBegin)
					{
						result = vkEndCommandBuffer(shadowmapBuffer);
						if (result != VK_SUCCESS)
						{
							DEBUG_BREAK();
						}
					}

					result = vkEndCommandBuffer(commandBuffer);
					if (result != VK_SUCCESS)
					{
						DEBUG_BREAK();
					}
				}
			}
			break;
#endif

		default: break;
		}
	}

	void BeginShadowRenderPass()
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXPipeline::CleanBindBuffers();

				MUTexture *ShadowMap = EDirectXBuffer::GetShadowMapTexture(EDirectX::CurrentCommandBufferIndex);
				D3D11_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<mu_float>(SHADOWMAP_RESOLUTION), static_cast<mu_float>(SHADOWMAP_RESOLUTION), 0.0f, 1.0f };
				EDirectXInit::DeviceContext->OMSetRenderTargets(0, nullptr, ShadowMap->DirectXInternal.DepthStencilView);
				EDirectXInit::DeviceContext->RSSetViewports(1, &viewport);
				EDirectXInit::DeviceContext->ClearDepthStencilView(ShadowMap->DirectXInternal.DepthStencilView, D3D11_CLEAR_DEPTH, DIRECTX_DEPTH_CLEAR, DIRECTX_STENCIL_CLEAR);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				// FIRST CALL CleanBindBuffers (OpenGL ERROR ON PC IF DOESN'T CALL THIS FUNCTION FIRST!)
				EOpenGLPipeline::CleanBindBuffers();
#if !defined(GL_ES_VERSION_3_0)
				EOpenGLPipeline::SetMultiSample(false);
#endif

				EViewport viewport = { 0.0f, 0.0f, static_cast<mu_float>(SHADOWMAP_RESOLUTION), static_cast<mu_float>(SHADOWMAP_RESOLUTION), 0.0f, 1.0f };
				glBindFramebuffer(GL_FRAMEBUFFER, EOpenGLBuffer::GetShadowMapTexture(EOpenGL::CurrentCommandBufferIndex)->OpenGLInternal.FramebufferResource);
				glViewport(viewport.OffsetX, viewport.OffsetY, viewport.Width, viewport.Height);

				const mu_float DepthClear = OPENGL_DEPTH_CLEAR;
				glClearBufferfv(GL_DEPTH, 0, &DepthClear);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				const mu_uint32 currentIndex = EVulkan::CurrentCommandBufferIndex;
				VkCommandBuffer commandBuffer = EVulkan::ShadowMapBuffers[currentIndex];
				EVulkan::CurrentCommandBuffer = commandBuffer;

				MUTexture *ShadowTexture = EVulkanBuffer::GetShadowMapTexture(currentIndex);

				VkClearValue clearValues[1];
				clearValues[0].depthStencil = { VULKAN_DEPTH_CLEAR, VULKAN_STENCIL_CLEAR };

				VkRenderPassBeginInfo renderPassBeginInfo = EVulkanInitializer::RenderPassBeginInfo();
				renderPassBeginInfo.renderPass = EVulkanPipeline::GetRenderPass(ERenderPass::eShadow);
				renderPassBeginInfo.renderArea.offset.x = 0;
				renderPassBeginInfo.renderArea.offset.y = 0;
				renderPassBeginInfo.renderArea.extent.width = SHADOWMAP_RESOLUTION;
				renderPassBeginInfo.renderArea.extent.height = SHADOWMAP_RESOLUTION;
				renderPassBeginInfo.clearValueCount = mu_countof(clearValues);
				renderPassBeginInfo.pClearValues = clearValues;
				renderPassBeginInfo.framebuffer = ShadowTexture->VulkanInternal.Framebuffer;

				vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

				EVulkan::MarkBusyMask(ShadowTexture->TextureInternal.BusyMask, currentIndex);

				VkViewport viewport =
				{
					0,
					0,
					SHADOWMAP_RESOLUTION,
					SHADOWMAP_RESOLUTION,
					0.0f, 1.0f
				};
				vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

				VkRect2D scissor =
				{
					{ 0, 0 },
					{ SHADOWMAP_RESOLUTION, SHADOWMAP_RESOLUTION }
				};
				vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

				EVulkanPipeline::ClearPipeline();
				EVulkanPipeline::ClearBindBuffers();
			}
			break;
#endif

		default: break;
		}
	}

	void EndShadowRenderPass()
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				const mu_uint32 currentIndex = EVulkan::CurrentCommandBufferIndex;
				VkCommandBuffer commandBuffer = EVulkan::ShadowMapBuffers[currentIndex];
				vkCmdEndRenderPass(commandBuffer);
			}
			break;
#endif

		default: break;
		}
	}

	void BeginRenderPass(const std::array<mu_float, 4> ClearColor)
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				MUTexture *multisampleTexture = EDirectXBuffer::GetMultisampleTexture(EDirectX::CurrentCommandBufferIndex);
				if (MU_RESOURCE->IsAntialiasing() == true && multisampleTexture->IsLoaded() == true)
				{
					EDirectXInit::DeviceContext->OMSetRenderTargets(1, &multisampleTexture->DirectXInternal.RenderTargetView, EDirectXInit::DepthStencilView);
					EDirectXInit::DeviceContext->ClearDepthStencilView(EDirectXInit::DepthStencilView, D3D11_CLEAR_DEPTH, DIRECTX_DEPTH_CLEAR, DIRECTX_STENCIL_CLEAR);
					EDirectXInit::DeviceContext->ClearRenderTargetView(multisampleTexture->DirectXInternal.RenderTargetView, ClearColor.data());
				}
				else
				{
					EDirectXInit::DeviceContext->OMSetRenderTargets(1, &EDirectXInit::RenderTargetView, EDirectXInit::DepthStencilView);
					EDirectXInit::DeviceContext->ClearDepthStencilView(EDirectXInit::DepthStencilView, D3D11_CLEAR_DEPTH, DIRECTX_DEPTH_CLEAR, DIRECTX_STENCIL_CLEAR);
					EDirectXInit::DeviceContext->ClearRenderTargetView(EDirectXInit::RenderTargetView, ClearColor.data());
				}
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				// FIRST CALL CleanBindBuffers (OpenGL ERROR ON PC IF DOESN'T CALL THIS FUNCTION FIRST!)
				EOpenGLPipeline::CleanBindBuffers();

				if (MU_RESOURCE->IsAntialiasing() == true)
				{
					MUTexture *multisampleTexture = EOpenGLBuffer::GetMultisampleTexture(EOpenGL::CurrentCommandBufferIndex);
					if (multisampleTexture->IsLoaded() == true)
					{
#if !defined(GL_ES_VERSION_3_0)
						EOpenGLPipeline::SetMultiSample(true);
#endif
						glBindFramebuffer(GL_FRAMEBUFFER, multisampleTexture->OpenGLInternal.FramebufferResource);
					}
#if !defined(GL_ES_VERSION_3_0)
					else
					{
						EOpenGLPipeline::SetMultiSample(false);
					}
#endif
				}
#if !defined(GL_ES_VERSION_3_0)
				else
				{
					EOpenGLPipeline::SetMultiSample(false);
				}
#endif

				const mu_float DepthClear = OPENGL_DEPTH_CLEAR;
				glClearBufferfv(GL_DEPTH, 0, &DepthClear);
				glClearBufferfv(GL_COLOR, 0, ClearColor.data());
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				const mu_uint32 currentIndex = EVulkan::CurrentCommandBufferIndex;
				VkCommandBuffer commandBuffer = EVulkan::CommandBuffers[currentIndex];
				EVulkan::CurrentCommandBuffer = commandBuffer;
				
				VkClearValue clearValues[2];
				clearValues[0].color = { { ClearColor[0], ClearColor[1], ClearColor[2], ClearColor[3] } };
				clearValues[1].depthStencil = { VULKAN_DEPTH_CLEAR, VULKAN_STENCIL_CLEAR };

				VkRenderPassBeginInfo renderPassBeginInfo = EVulkanInitializer::RenderPassBeginInfo();
				renderPassBeginInfo.renderPass = EVulkanPipeline::GetRenderPass(ERenderPass::eNormal);
				renderPassBeginInfo.renderArea.offset.x = 0;
				renderPassBeginInfo.renderArea.offset.y = 0;
				renderPassBeginInfo.renderArea.extent.width = EVulkan::SwapchainSize.width;
				renderPassBeginInfo.renderArea.extent.height = EVulkan::SwapchainSize.height;
				renderPassBeginInfo.clearValueCount = mu_countof(clearValues);
				renderPassBeginInfo.pClearValues = clearValues;
				renderPassBeginInfo.framebuffer = EVulkan::Framebuffers[EVulkan::CurrentSwapchainIndex];

				vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

				EVulkanPipeline::ClearPipeline();
				EVulkanPipeline::ClearBindBuffers();
			}
			break;
#endif

		default: break;
		}
	}

	void EndRenderPass()
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{

			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				const mu_uint32 currentIndex = EVulkan::CurrentCommandBufferIndex;
				VkCommandBuffer commandBuffer = EVulkan::CommandBuffers[currentIndex];
				vkCmdEndRenderPass(commandBuffer);
			}
			break;
#endif

		default: break;
		}
	}

	void SubmitFrame()
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{

			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false) break;

				const mu_uint32 currentIndex = EVulkan::CurrentCommandBufferIndex;

				if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Visible) == false)
				{
					VkSubmitInfo submitInfo = {};
					submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
					submitInfo.commandBufferCount = 1;
					submitInfo.pCommandBuffers = &EVulkan::TransferBuffers[currentIndex];

					VkResult result = vkQueueSubmit(EVulkan::GraphicsQueue, 1, &submitInfo, EVulkan::CommandFences[currentIndex]);
					if (result != VK_SUCCESS)
					{
						DEBUG_BREAK();
					}
				}
				else
				{
					VkSubmitInfo submitInfo = {};
					submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
					submitInfo.commandBufferCount = 1;
					submitInfo.pCommandBuffers = &EVulkan::TransferBuffers[currentIndex];
					submitInfo.signalSemaphoreCount = 1;
					submitInfo.pSignalSemaphores = &EVulkan::TransferFinishedSemaphore;

					VkResult result = vkQueueSubmit(EVulkan::GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
					switch (result)
					{
					default: break;
					}

					if (MU_RESOURCE->GetShadowType() >= EShadowType::eShadowMappingBegin)
					{
						// Shadow Map Queue
						{
							VkSemaphore waitSemaphores[] = { EVulkan::ImageAvailableSemaphore, EVulkan::TransferFinishedSemaphore };
							VkPipelineStageFlags stageFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT };

							submitInfo.commandBufferCount = 1;
							submitInfo.pCommandBuffers = &EVulkan::ShadowMapBuffers[currentIndex];
							submitInfo.waitSemaphoreCount = mu_countof(waitSemaphores);
							submitInfo.pWaitSemaphores = waitSemaphores;
							submitInfo.pWaitDstStageMask = stageFlags;
							submitInfo.signalSemaphoreCount = 1;
							submitInfo.pSignalSemaphores = &EVulkan::ShadowMapFinishedSemaphore;

							result = vkQueueSubmit(EVulkan::GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
							if (result != VK_SUCCESS)
							{
								DEBUG_BREAK();
							}
						}

						// Normal Queue
						{
							VkSemaphore waitSemaphores[] = { EVulkan::ShadowMapFinishedSemaphore };
							VkPipelineStageFlags stageFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

							submitInfo.commandBufferCount = 1;
							submitInfo.pCommandBuffers = &EVulkan::CommandBuffers[currentIndex];
							submitInfo.waitSemaphoreCount = mu_countof(waitSemaphores);
							submitInfo.pWaitSemaphores = waitSemaphores;
							submitInfo.pWaitDstStageMask = stageFlags;
							submitInfo.signalSemaphoreCount = 1;
							submitInfo.pSignalSemaphores = &EVulkan::RenderingFinishedSemaphore;

							result = vkQueueSubmit(EVulkan::GraphicsQueue, 1, &submitInfo, EVulkan::CommandFences[currentIndex]);
							if (result != VK_SUCCESS)
							{
								DEBUG_BREAK();
							}
						}
					}
					else
					{
						VkSemaphore waitSemaphores[] = { EVulkan::ImageAvailableSemaphore, EVulkan::TransferFinishedSemaphore };
						VkPipelineStageFlags stageFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT };

						submitInfo.commandBufferCount = 1;
						submitInfo.pCommandBuffers = &EVulkan::CommandBuffers[currentIndex];
						submitInfo.waitSemaphoreCount = mu_countof(waitSemaphores);
						submitInfo.pWaitSemaphores = waitSemaphores;
						submitInfo.pWaitDstStageMask = stageFlags;
						submitInfo.signalSemaphoreCount = 1;
						submitInfo.pSignalSemaphores = &EVulkan::RenderingFinishedSemaphore;

						result = vkQueueSubmit(EVulkan::GraphicsQueue, 1, &submitInfo, EVulkan::CommandFences[currentIndex]);
						if (result != VK_SUCCESS)
						{
							DEBUG_BREAK();
						}
					}
				}
			}
			break;
#endif

		default: break;
		}
	}

	void PresentFrame()
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Visible | EVideoStatusFlag::VSF_Available) == true)
				{
					if (MU_RESOURCE->IsAntialiasing() == true)
					{
						MUTexture *multisampleTexture = EDirectXBuffer::GetMultisampleTexture(EDirectX::CurrentCommandBufferIndex);
						if (multisampleTexture->IsLoaded() == true)
						{
							ID3D11Texture2D *backBuffer = nullptr;
							HRESULT result = EDirectXInit::Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
							if (SUCCEEDED(result))
							{
								EDirectXInit::DeviceContext->ResolveSubresource(backBuffer, 0, multisampleTexture->DirectXInternal.TextureResource, 0, EDirectXInit::SwapchainDesc.BufferDesc.Format);
								backBuffer->Release();
							}
						}
					}

					HRESULT hr;
					if (GetVideoStatus(EVideoStatusFlag::VSF_VerticalSync) == true &&
						g_CurrentScene != STARTUP_SCENE)
					{
						hr = EDirectXInit::Swapchain->Present(1, 0);
					}
					else
					{
						hr = EDirectXInit::Swapchain->Present(0, 0);
					}
					if (hr == DXGI_ERROR_DEVICE_REMOVED)
					{
						hr = EDirectXInit::Device->GetDeviceRemovedReason();
					}
					mu_assert(SUCCEEDED(hr));
				}
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Visible | EVideoStatusFlag::VSF_Available) == true)
				{
					if (MU_RESOURCE->IsAntialiasing() == true)
					{
						MUTexture *multisampleTexture = EOpenGLBuffer::GetMultisampleTexture(EOpenGL::CurrentCommandBufferIndex);
						if (multisampleTexture->IsLoaded() == true)
						{
							glBindFramebuffer(GL_FRAMEBUFFER, 0);
							glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampleTexture->OpenGLInternal.FramebufferResource);

#if !defined(GL_ES_VERSION_3_0)
							if constexpr (OPENGL_BACKEND_BUFFERCOUNT < 2)
							{
								glDrawBuffer(GL_BACK);
							}
#endif

							const mu_uint32 width = MU_RESOURCE->GetWindowWidth();
							const mu_uint32 height = MU_RESOURCE->GetWindowHeight();
							glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

#if !defined(GL_ES_VERSION_3_0)
							if constexpr (OPENGL_BACKEND_BUFFERCOUNT < 2)
							{
								glDrawBuffer(GL_FRONT);
							}
#endif

							glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
						}
					}

					SDL_GL_SwapWindow(MU_WINDOW->GetSDLWindow());
				}
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Visible | EVideoStatusFlag::VSF_Available) == true)
				{
					const mu_uint32 currentIndex = EVulkan::CurrentCommandBufferIndex;
					VkPresentInfoKHR presentInfo = {};
					presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
					presentInfo.waitSemaphoreCount = 1;
					presentInfo.pWaitSemaphores = &EVulkan::RenderingFinishedSemaphore;
					presentInfo.swapchainCount = 1;
					presentInfo.pSwapchains = &EVulkan::Swapchain;
					presentInfo.pImageIndices = &currentIndex;

					VkResult result = vkQueuePresentKHR(EVulkan::GraphicsQueue, &presentInfo);
					if (result != VK_SUCCESS)
					{
						DEBUG_BREAK();
					}
				}
			}
			break;
#endif

		default: break;
		}
	}

	void FinishAndWaitDevice()
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				if (EDirectXInit::Swapchain != nullptr)
				{
					EDirectXInit::Swapchain->Present(0, 0);
				}
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				// Nothing to do
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				if (EVulkan::Device != VK_NULL_HANDLE)
				{
					vkDeviceWaitIdle(EVulkan::Device);
				}
			}
			break;
#endif

		default: break;
		}
	}

	void ClearDepthAndStencil()
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXInit::DeviceContext->ClearDepthStencilView(EDirectXInit::DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, DIRECTX_DEPTH_CLEAR, DIRECTX_STENCIL_CLEAR);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				const mu_float DepthClear = OPENGL_DEPTH_CLEAR;
				glClearBufferfv(GL_DEPTH, 0, &DepthClear);
				//glClearBufferfi(GL_DEPTH_STENCIL, 0, OPENGL_DEPTH_CLEAR, OPENGL_STENCIL_CLEAR);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				VkCommandBuffer commandBuffer = EVulkan::CurrentCommandBuffer;

				VkClearAttachment clearAttachment = {};
				clearAttachment.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
				clearAttachment.colorAttachment = VK_ATTACHMENT_UNUSED;
				clearAttachment.clearValue.depthStencil = { VULKAN_DEPTH_CLEAR, VULKAN_STENCIL_CLEAR };

				VkClearRect clearRect = {};
				clearRect.rect.offset = { 0, 0 };
				clearRect.rect.extent = { EVulkan::SwapchainSize.width, EVulkan::SwapchainSize.height };
				clearRect.baseArrayLayer = 0;
				clearRect.layerCount = 1;
				
				vkCmdClearAttachments(commandBuffer, 1, &clearAttachment, 1, &clearRect);
			}
			break;
#endif

		default: break;
		}
	}

	void SetViewport(const mu_float OffsetX, const mu_float OffsetY, const mu_float Width, const mu_float Height)
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				D3D11_VIEWPORT viewport =
				{
					OffsetX, OffsetY,
					Width, Height,
					0.0f, 1.0f
				};
				EDirectXInit::DeviceContext->RSSetViewports(1, &viewport);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				mu_float flipY = (mu_float)MU_RESOURCE->GetWindowHeight() - OffsetY - Height;
				glViewport(OffsetX, flipY, Width, Height);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				VkCommandBuffer commandBuffer = EVulkan::CurrentCommandBuffer;

				VkViewport viewport =
				{
					OffsetX,
					OffsetY,
					Width,
					Height,
					0.0f, 1.0f
				};
				vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
			}
			break;
#endif

		default: break;
		}
	}

	void SetScissor(const mu_int32 OffsetX, const mu_int32 OffsetY, const mu_int32 Width, const mu_int32 Height)
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				D3D11_RECT ScissorRect;

				ScissorRect.left = OffsetX;
				ScissorRect.top = OffsetY;
				ScissorRect.right = OffsetX + Width;
				ScissorRect.bottom = OffsetY + Height;

				EDirectXInit::DeviceContext->RSSetScissorRects(1, &ScissorRect);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLPipeline::SetScissorArea(OffsetX, OffsetY, Width, Height);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				VkCommandBuffer commandBuffer = EVulkan::CurrentCommandBuffer;

				VkRect2D scissor =
				{
					{ OffsetX, OffsetY },
					{ static_cast<mu_uint32>(Width), static_cast<mu_uint32>(Height) }
				};
				vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
				EVulkanPipeline::SetScissor(true);
			}
			break;
#endif

		default: break;
		}
	}

	void CleanScissor()
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				VkCommandBuffer commandBuffer = EVulkan::CurrentCommandBuffer;

				VkRect2D scissor =
				{
					{ 0, 0 },
					{ EVulkan::SwapchainSize.width, EVulkan::SwapchainSize.height }
				};
				vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
			}
			break;
#endif

		default: break;
		}
	}

	void RenderMouse(const mu_double ElapsedTime, const mu_uint64 CurrentTime)
	{
		MU_INPUT->RenderCursor(CurrentTime); // Used for render mouse cursor
		g_Tasking.RenderDebugInformation(ElapsedTime, CurrentTime);
	}

	void UpdateGlobalOnly()
	{
		if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false) return;

		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXBuffer::UpdateGlobalOnly();
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLBuffer::UpdateGlobalOnly();
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				EVulkanBuffer::UpdateGlobalOnly();
			}
			break;
#endif

		default: break;
		}
	}

	void UpdateBuffers(const mu_boolean IsLightUpload)
	{
		if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false) return;

		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXBuffer::Update(IsLightUpload);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLBuffer::Update(IsLightUpload);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				EVulkanBuffer::Update(IsLightUpload);
			}
			break;
#endif

		default: break;
		}
	}

	void UIPreUpdateBuffers()
	{
		if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false) return;

		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXBuffer::UIUpdateBuffers();
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLBuffer::UIUpdateBuffers();
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
			}
			break;
#endif

		default: break;
		}
	}

	void UIPostUpdateBuffers()
	{
		if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false) return;

		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				EVulkanBuffer::UIUpdateBuffers();
			}
			break;
#endif

		default: break;
		}
	}

	const mu_uint32 UIAllocateDataIndex(const mu_uint32 count)
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			return EDirectXBuffer::UIAllocateDataIndex(count);
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			return EOpenGLBuffer::UIAllocateDataIndex(count);
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			return EVulkanBuffer::UIAllocateDataIndex(count);
#endif

		default: break;
		}

		return EInvalidUInt32;
	}

	SHUIDefaultVertex *UIRetrieveDataBuffer(const mu_uint32 index)
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			return EDirectXBuffer::UIRetrieveDataBuffer(index);
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			return EOpenGLBuffer::UIRetrieveDataBuffer(index);
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			return EVulkanBuffer::UIRetrieveDataBuffer(index);
#endif

		default: break;
		}

		return nullptr;
	}

	void UIRequestUpdateData(const mu_uint32 index, const mu_uint32 count)
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXBuffer::UIRequestUpdateData(index, count);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLBuffer::UIRequestUpdateData(index, count);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				EVulkanBuffer::UIRequestUpdateData(index, count);
			}
			break;
#endif

		default: break;
		}
	}

	void FontPreUpdateBuffers()
	{
		if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false) return;

		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXBuffer::FontUpdateBuffers();
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLBuffer::FontUpdateBuffers();
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
			}
			break;
#endif

		default: break;
		}
	}

	void FontPostUpdateBuffers()
	{
		if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false) return;

		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				EVulkanBuffer::FontUpdateBuffers();
			}
			break;
#endif

		default: break;
		}
	}

	const mu_uint32 FontAllocateDataIndex(const mu_uint32 count)
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			return EDirectXBuffer::FontAllocateDataIndex(count);
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			return EOpenGLBuffer::FontAllocateDataIndex(count);
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			return EVulkanBuffer::FontAllocateDataIndex(count);
#endif

		default: break;
		}

		return EInvalidUInt32;
	}

	void FontDeallocateDataIndex(const mu_uint32 index)
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			EDirectXBuffer::FontDeallocateDataIndex(index);
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			EOpenGLBuffer::FontDeallocateDataIndex(index);
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			EVulkanBuffer::FontDeallocateDataIndex(index);
			break;
#endif

		default: break;
		}
	}

	SHFontInstance *FontRetrieveDataBuffer(const mu_uint32 index)
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			return EDirectXBuffer::FontRetrieveDataBuffer(index);
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			return EOpenGLBuffer::FontRetrieveDataBuffer(index);
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			return EVulkanBuffer::FontRetrieveDataBuffer(index);
#endif

		default: break;
		}

		return nullptr;
	}

	const mu_uint32 FontRetrieveBufferBaseIndex(const mu_uint32 index)
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			return EDirectXBuffer::FontRetrieveBufferBaseIndex(index);
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			return EOpenGLBuffer::FontRetrieveBufferBaseIndex(index);
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			return EVulkanBuffer::FontRetrieveBufferBaseIndex(index);
#endif

		default: break;
		}

		return 0;
	}

	void FontRequestUpdateData(const mu_uint32 index, const mu_uint32 count)
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXBuffer::FontRequestUpdateData(index, count);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLBuffer::FontRequestUpdateData(index, count);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				EVulkanBuffer::FontRequestUpdateData(index, count);
			}
			break;
#endif

		default: break;
		}
	}

	void EmptyTrash()
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				EVulkanBuffer::UIEmptyTrash();
				EVulkanBuffer::FontEmptyTrash();
			}
			break;
#endif

		default: break;
		}
	}

	void DefragmentBuffer()
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXBuffer::FontDefragmentBuffer();
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLBuffer::FontDefragmentBuffer();
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				EVulkanBuffer::FontDefragmentBuffer();
			}
			break;
#endif

		default: break;
		}
	}

	void CollectGarbage()
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				EVulkanGarbage::Empty();
			}
			break;
#endif

		default: break;
		}
	}

	void BindGlobalConstant(const EGlobalConstant globalConstant)
	{
		switch (GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXBuffer::BindGlobalConstant(globalConstant);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLBuffer::BindGlobalConstant(globalConstant);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				VkDescriptorSet globalDescriptorSets[] =
				{
					EVulkanDescriptor::GetGlobalDescriptorSet(globalConstant),
				};
				EVulkanPipeline::BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, EVulkanPipeline::GetPipelineLayout(EUniformLayout::eGlobalConstantOnly), 0, mu_countof(globalDescriptorSets), globalDescriptorSets);
			}
			break;
#endif

		default: break;
		}
	}

	static EMatrix4 IdentityMatrix = EMatrix4();
	void Unproject2D(const mu_float X, const mu_float Y, const mu_float AspectWidth, const mu_float AspectHeight, EVector3 &Source, EVector3 &Target)
	{
		const mu_float sx = MU_TO_WINDOW(X, AspectWidth);
		const mu_float sy = MU_TO_WINDOW(Y, AspectHeight);
		switch (EVideoBackend::GetBackend())
		{
		case EVideoBackendType::eDirectX:
		case EVideoBackendType::eOpenGL:
			{
				Source = EMath::unProject(EVector3(sx,
												   MU_RESOURCE->GetWindowHeight() - sy,
												   0.0f),
										  IdentityMatrix,
										  MU_VIDEO->_CurrentProjectionUI,
										  EVector4(MU_VIDEO->_2DViewport.OffsetX,
												   MU_VIDEO->_2DViewport.OffsetY,
												   MU_VIDEO->_2DViewport.OffsetX + MU_VIDEO->_2DViewport.Width,
												   MU_VIDEO->_2DViewport.OffsetY + MU_VIDEO->_2DViewport.Height));
				Target = EMath::unProject(EVector3(sx,
												   MU_RESOURCE->GetWindowHeight() - sy,
												   1.0f),
										  IdentityMatrix,
										  MU_VIDEO->_CurrentProjectionUI,
										  EVector4(MU_VIDEO->_2DViewport.OffsetX,
												   MU_VIDEO->_2DViewport.OffsetY,
												   MU_VIDEO->_2DViewport.OffsetX + MU_VIDEO->_2DViewport.Width,
												   MU_VIDEO->_2DViewport.OffsetY + MU_VIDEO->_2DViewport.Height));
			}
			break;

		case EVideoBackendType::eVulkan:
			{
				Source = EMath::unProject(EVector3(sx,
												   sy,
												   0.0f),
										  IdentityMatrix,
										  MU_VIDEO->_CurrentProjectionUI,
										  EVector4(MU_VIDEO->_2DViewport.OffsetX,
												   MU_VIDEO->_2DViewport.OffsetY,
												   MU_VIDEO->_2DViewport.OffsetX + MU_VIDEO->_2DViewport.Width,
												   MU_VIDEO->_2DViewport.OffsetY + MU_VIDEO->_2DViewport.Height));
				Target = EMath::unProject(EVector3(sx,
												   sy,
												   1.0f),
										  IdentityMatrix,
										  MU_VIDEO->_CurrentProjectionUI,
										  EVector4(MU_VIDEO->_2DViewport.OffsetX,
												   MU_VIDEO->_2DViewport.OffsetY,
												   MU_VIDEO->_2DViewport.OffsetX + MU_VIDEO->_2DViewport.Width,
												   MU_VIDEO->_2DViewport.OffsetY + MU_VIDEO->_2DViewport.Height));
			}
			break;

		default: break;
		}
	}
};