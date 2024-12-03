#include "stdafx.h"

namespace EVulkanInit
{
	const mu_boolean Initialize(SDL_Window *SDLWindow)
	{
		if (EVulkanInit::CreateInstance(SDLWindow) == false)
		{
			return false;
		}

		if (EVulkanInit::CreateDebugCallback() == false)
		{
			return false;
		}

		if (EVulkanInit::CreateSurface(SDLWindow) == false)
		{
			return false;
		}

		if (EVulkanInit::FindPhysicalDevice() == false)
		{
			return false;
		}

		if (EVulkanInit::CreateLogicalDevice() == false)
		{
			return false;
		}

		if (EVulkanInit::CreateSync() == false)
		{
			return false;
		}

		if (EVulkanInit::GetSurfaceCapabilities() == false)
		{
			return false;
		}

		if (EVulkanInit::GetSurfaceFormat() == false)
		{
			return false;
		}

		if(EVulkanMemory::Initialize() == false)
		{
			return false;
		}

		EVulkanInit::UpdateSamplesCount(MU_RESOURCE->IsAntialiasing() ? VK_SAMPLE_COUNT_16_BIT : VK_SAMPLE_COUNT_1_BIT);

		if (EVulkanInit::UpdateSwapchainSize(SDLWindow) == false)
		{
			return false;
		}

		if (EVulkanInit::CreateSwapchain() == false)
		{
			return false;
		}

		if (EVulkanInit::CreateSwapchainView() == false)
		{
			return false;
		}

		if (EVulkan::DesiredSamplesCount > VK_SAMPLE_COUNT_1_BIT &&
			EVulkanInit::CreateColor() == false)
		{
			return false;
		}

		if (EVulkanInit::CreateDepthStencil() == false)
		{
			return false;
		}

		if (EVulkanInit::CreateCommandPool() == false ||
			EVulkanInit::CreateCommandBuffers() == false ||
			EVulkanInit::CreateCommandFences() == false)
		{
			return false;
		}

		if (EVulkanSampler::Create() == false)
		{
			return false;
		}

		if (EVulkanPipeline::CreateRenderPasses() == false)
		{
			return false;
		}

		if (EVulkanInit::CreateFramebuffers() == false)
		{
			return false;
		}

		if (EVulkanPipeline::CreatePipelineCache() == false)
		{
			return false;
		}

		if (EVulkanPipeline::CreatePipelineLayouts() == false)
		{
			return false;
		}

		if (EVulkanShader::Create(true) == false)
		{
			return false;
		}

		if (EVulkanPipeline::CreatePipelines(ECombinedShader::eBeginEarly, ECombinedShader::eEndEarly) == false)
		{
			return false;
		}

		if (EVulkanDescriptor::Create() == false)
		{
			return false;
		}

		if (EVulkanSkeleton::CreateSkeleton() == false)
		{
			return false;
		}

		if (EVulkanBuffer::Create() == false)
		{
			return false;
		}

		if (EVulkanDescriptor::AllocateAllSamplerDescriptorSet() == false ||
			EVulkanDescriptor::AllocateAllGlobalDescriptorSet() == false)
		{
			return false;
		}

		if(EVulkanBuffer::CreateShadowMap() == false)
		{
			return false;
		}

		return true;
	}

	void Destroy(const mu_boolean gpuOnly)
	{
		if (EVulkan::Device != VK_NULL_HANDLE)
		{
			vkDeviceWaitIdle(EVulkan::Device);
		}

		EVulkanBuffer::Destroy(gpuOnly);
		EVulkanBuffer::DestroyShadowMap();
		EVulkanSkeleton::DestroySkeleton();
		EVulkanPipeline::DestroyPipelines();
		EVulkanPipeline::ClearPipeline();
		EVulkanPipeline::ClearBindBuffers();
		EVulkanShader::Destroy();
		EVulkanPipeline::DestroyPipelineLayouts();
		EVulkanPipeline::DestroyPipelineCache();

		EVulkanInit::DestroyFramebuffers();

		EVulkanPipeline::DestroyRenderPasses();
		EVulkanSampler::Destroy();

		EVulkanGarbage::ForceEmpty();
		EVulkanDescriptor::Destroy();

		EVulkanInit::DestroyCommandFences();
		EVulkanInit::DestroyCommandBuffers();
		EVulkanInit::DestroyCommandPool();
		EVulkanInit::DestroyDepthStencil();
		EVulkanInit::DestroyColor();
		EVulkanInit::DestroySwapchainView();
		EVulkanInit::DestroySwapchain();
		EVulkanMemory::Destroy();
		EVulkanInit::DestroySync();
		EVulkanInit::DestroyLogicalDevice();
		EVulkanInit::DestroySurface();
		EVulkanInit::DestroyDebugCallback();
		EVulkanInit::DestroyInstance();

		EVulkan::SurfaceFormats.clear();
	}

	const mu_boolean CreateInstance(SDL_Window *SDLWindow)
	{
		VkResult result;

		vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)SDL_Vulkan_GetVkGetInstanceProcAddr();
		if (!vkGetInstanceProcAddr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"SDL_Vulkan_GetVkGetInstanceProcAddr(): %s\n",
				SDL_GetError());
			return false;
		}

#define VULKAN_GLOBAL_FUNCTION(name)                                             \
    name = EVulkan::GetGlobalProcessAddress<PFN_##name>(#name);                  \
    if(!name)                                                                    \
    {                                                                            \
        return false;                                                            \
    }
		VULKAN_GLOBAL_FUNCTIONS();
#undef VULKAN_GLOBAL_FUNCTION

		mu_uint32 extensionsCount = 0;
		if (SDL_Vulkan_GetInstanceExtensions(SDLWindow, &extensionsCount, nullptr) == SDL_FALSE)
		{
			return false;
		}

		std::vector<const mu_char*> extensionsNames(extensionsCount);
		if (SDL_Vulkan_GetInstanceExtensions(SDLWindow, &extensionsCount, extensionsNames.data()) == SDL_FALSE)
		{
			return false;
		}

#if !defined(NDEBUG) || defined(BACKEND_FORCE_DEBUGREPORT)
		extensionsNames.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Elion Online";
		appInfo.applicationVersion = VK_MAKE_VERSION(ELION_VERSION_MAJOR, ELION_VERSION_MINOR, ELION_VERSION_REVISION);
		appInfo.pEngineName = "Elion Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 11);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		instanceCreateInfo.enabledExtensionCount = extensionsNames.size();
		instanceCreateInfo.ppEnabledExtensionNames = extensionsNames.data();
#ifndef NDEBUG
#if !defined(__ANDROID__)
		// On desktop the LunarG loaders exposes a meta layer that contains all layers
		mu_int32 validationLayerCount = 1;
		const mu_char *validationLayerNames[] = {
			"VK_LAYER_LUNARG_standard_validation"
		};
#else
		// On Android we need to explicitly select all layers
		const mu_char *validationLayerNames[] = {
			"VK_LAYER_GOOGLE_threading",
			"VK_LAYER_LUNARG_parameter_validation",
			"VK_LAYER_LUNARG_object_tracker",
			"VK_LAYER_LUNARG_core_validation",
			"VK_LAYER_GOOGLE_unique_objects"
		};
		const mu_int32 validationLayerCount = mu_countof(validationLayerNames);
#endif
		instanceCreateInfo.enabledLayerCount = validationLayerCount;
		instanceCreateInfo.ppEnabledLayerNames = validationLayerNames;
#endif

		result = vkCreateInstance(&instanceCreateInfo, nullptr, &EVulkan::Instance);
		if (result == VK_ERROR_LAYER_NOT_PRESENT)
		{
			instanceCreateInfo.enabledLayerCount = 0;
			instanceCreateInfo.ppEnabledLayerNames = nullptr;

			result = vkCreateInstance(&instanceCreateInfo, nullptr, &EVulkan::Instance);
		}

		if (result != VK_SUCCESS)
		{
			return false;
		}

		// Load Instance Functions
		{
#define VULKAN_INSTANCE_FUNCTION(name)                                           \
    name = EVulkan::GetInstanceProcessAddress<PFN_##name>(#name);                \
    if(!name)                                                                    \
    {                                                                            \
        return false;                                                            \
    }
#if !defined(NDEBUG) || defined(BACKEND_FORCE_DEBUGREPORT)
			VULKAN_DEBUG_INSTANCE_FUNCTIONS();
#endif
			VULKAN_INSTANCE_FUNCTIONS();
#undef VULKAN_INSTANCE_FUNCTION
		}

		return true;
	}

	void DestroyInstance()
	{
		if(EVulkan::Instance != nullptr)
		{
			vkDestroyInstance(EVulkan::Instance, nullptr);
			EVulkan::Instance = nullptr;
		}
	}

	const mu_boolean CreateDebugCallback()
	{
#if !defined(NDEBUG) || defined(BACKEND_FORCE_DEBUGREPORT)
		VkDebugReportCallbackCreateInfoEXT debugCreateInfo = {};
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		debugCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)EVulkanDebug::DebugCallback;
		debugCreateInfo.flags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT;
		VkResult result = vkCreateDebugReportCallbackEXT(EVulkan::Instance, &debugCreateInfo, nullptr, &EVulkan::DebugCallback);
		if (result != VK_SUCCESS)
		{
			return false;
		}
#endif

		return true;
	}

	void DestroyDebugCallback()
	{
#if !defined(NDEBUG) || defined(BACKEND_FORCE_DEBUGREPORT)
		if (EVulkan::DebugCallback != VK_NULL_HANDLE)
		{
			vkDestroyDebugReportCallbackEXT(EVulkan::Instance, EVulkan::DebugCallback, nullptr);
			EVulkan::DebugCallback = VK_NULL_HANDLE;
		}
#endif
	}

	const mu_boolean CreateSurface(SDL_Window *SDLWindow)
	{
		if (SDL_Vulkan_CreateSurface(SDLWindow, EVulkan::Instance, &EVulkan::Surface) == SDL_FALSE)
		{
			return false;
		}

		return true;
	}

	void DestroySurface()
	{
		if (EVulkan::Surface != VK_NULL_HANDLE)
		{
			vkDestroySurfaceKHR(EVulkan::Instance, EVulkan::Surface, nullptr);
			EVulkan::Surface = VK_NULL_HANDLE;
		}
	}

	const mu_boolean FindPhysicalDevice()
	{
		mu_uint32 physicalDeviceCount = 0;
		VkResult result = vkEnumeratePhysicalDevices(EVulkan::Instance, &physicalDeviceCount, nullptr);
		if (result != VK_SUCCESS)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkEnumeratePhysicalDevices(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}

		if (physicalDeviceCount == 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkEnumeratePhysicalDevices(): no physical devices\n");
			return false;
		}

		std::unique_ptr<VkPhysicalDevice[]> physicalDevices(new (std::nothrow) VkPhysicalDevice[physicalDeviceCount]);
		if (!physicalDevices)
		{
			return false;
		}

		result = vkEnumeratePhysicalDevices(EVulkan::Instance, &physicalDeviceCount, physicalDevices.get());
		if (result != VK_SUCCESS)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkEnumeratePhysicalDevices(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}

		std::vector<VkQueueFamilyProperties> queueFamiliesProperties;
		std::vector<VkExtensionProperties> deviceExtensions;
		for (mu_uint32 physicalDeviceIndex = 0; physicalDeviceIndex < physicalDeviceCount; ++physicalDeviceIndex)
		{
			mu_uint32 queueFamiliesCount = 0;
			mu_uint32 deviceExtensionCount = 0;
			SDL_bool hasSwapchainExtension = SDL_FALSE;

			VkPhysicalDevice physicalDevice = physicalDevices[physicalDeviceIndex];
			vkGetPhysicalDeviceProperties(physicalDevice, &EVulkan::DeviceProperties);
			if (VK_VERSION_MAJOR(EVulkan::DeviceProperties.apiVersion) < 1)
				continue;

			vkGetPhysicalDeviceFeatures(physicalDevice, &EVulkan::DeviceFeatures);
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamiliesCount, nullptr);
			if (queueFamiliesCount == 0)
				continue;

			if (queueFamiliesProperties.size() < queueFamiliesCount)
			{
				queueFamiliesProperties.resize(queueFamiliesCount);
			}

			vkGetPhysicalDeviceQueueFamilyProperties(
				physicalDevice, &queueFamiliesCount, queueFamiliesProperties.data());
			EVulkan::GraphicsQueueFamilyIndex = queueFamiliesCount;
			EVulkan::PresentQueueFamilyIndex = queueFamiliesCount;

			for (mu_uint32 queueFamilyIndex = 0; queueFamilyIndex < queueFamiliesCount; ++queueFamilyIndex)
			{
				VkBool32 supported = 0;

				if (queueFamiliesProperties[queueFamilyIndex].queueCount == 0)
					continue;

				if (queueFamiliesProperties[queueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT)
					EVulkan::GraphicsQueueFamilyIndex = queueFamilyIndex;

				result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, EVulkan::Surface, &supported);
				if (result != VK_SUCCESS)
				{
					SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
						"vkGetPhysicalDeviceSurfaceSupportKHR(): %s\n",
						EVulkanError::ResultToString(result));
					return false;
				}

				if (supported)
				{
					EVulkan::PresentQueueFamilyIndex = queueFamilyIndex;
					if (queueFamiliesProperties[queueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT)
						break; // use this queue because it can present and do graphics
				}
			}

			if (EVulkan::GraphicsQueueFamilyIndex == queueFamiliesCount) // no good queues found
				continue;
			if (EVulkan::PresentQueueFamilyIndex == queueFamiliesCount) // no good queues found
				continue;

			result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &deviceExtensionCount, nullptr);
			if (result != VK_SUCCESS)
			{
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
					"vkEnumerateDeviceExtensionProperties(): %s\n",
					EVulkanError::ResultToString(result));
				return false;
			}

			if (deviceExtensionCount == 0)
				continue;

			if (deviceExtensions.size() < deviceExtensionCount)
			{
				deviceExtensions.resize(deviceExtensionCount);
			}

			result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &deviceExtensionCount, deviceExtensions.data());
			if (result != VK_SUCCESS)
			{
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
					"vkEnumerateDeviceExtensionProperties(): %s\n",
					EVulkanError::ResultToString(result));
				return false;
			}

			for (mu_uint32 i = 0; i < deviceExtensionCount; i++)
			{
				if (0 == SDL_strcmp(deviceExtensions[i].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME))
				{
					hasSwapchainExtension = SDL_TRUE;
					break;
				}
			}

			if (!hasSwapchainExtension)
				continue;

			EVulkan::PhysicalDevice = physicalDevice;
			break;
		}

		if (!EVulkan::PhysicalDevice)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Vulkan: no viable physical devices found");
			return false;
		}

		EVideoBackend::SetTextureCapabilities(ETextureType::eTexture1D, EVulkan::DeviceProperties.limits.maxImageDimension1D, EVulkan::DeviceProperties.limits.maxImageArrayLayers);
		EVideoBackend::SetTextureCapabilities(ETextureType::eTexture2D, EVulkan::DeviceProperties.limits.maxImageDimension2D, EVulkan::DeviceProperties.limits.maxImageArrayLayers);
		EVideoBackend::SetTextureCapabilities(ETextureType::eTexture3D, EVulkan::DeviceProperties.limits.maxImageDimension3D, 1);
		EVideoBackend::SetTextureCapabilities(ETextureType::eTextureCube, EVulkan::DeviceProperties.limits.maxImageDimensionCube, 1);

		vkGetPhysicalDeviceMemoryProperties(EVulkan::PhysicalDevice, &EVulkan::DeviceMemoryProperties);

		if (EVulkan::DeviceProperties.limits.maxTexelBufferElements >= SKELETON_TEXTURE_WIDTH * SKELETON_TEXTURE_HEIGHT)
		{
			EVulkan::SkeletonTexelBufferEnabled = true;
			EVulkanBinding::BonesBinding[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
		}
		else
		{
			EVulkan::SkeletonTexelBufferEnabled = false;
			EVulkanBinding::BonesBinding[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		}
		
		return true;
	}

	const mu_boolean CreateLogicalDevice()
	{
		static const mu_float queuePriority[] = { 1.0f };
		static const mu_char *const deviceExtensionNames[] = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		};

		VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
		deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo.queueFamilyIndex = EVulkan::GraphicsQueueFamilyIndex;
		deviceQueueCreateInfo.queueCount = 1;
		deviceQueueCreateInfo.pQueuePriorities = &queuePriority[0];

		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.samplerAnisotropy = EVulkan::DeviceFeatures.samplerAnisotropy;
		deviceFeatures.textureCompressionASTC_LDR = EVulkan::DeviceFeatures.textureCompressionASTC_LDR;
		deviceFeatures.textureCompressionETC2 = EVulkan::DeviceFeatures.textureCompressionETC2;
		deviceFeatures.textureCompressionBC = EVulkan::DeviceFeatures.textureCompressionBC;
		deviceFeatures.shaderInt16 = EVulkan::DeviceFeatures.shaderInt16;
		deviceFeatures.sampleRateShading = EVulkan::DeviceFeatures.sampleRateShading;

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
		deviceCreateInfo.enabledExtensionCount = SDL_arraysize(deviceExtensionNames);
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensionNames;
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

		VkResult result = vkCreateDevice(EVulkan::PhysicalDevice, &deviceCreateInfo, nullptr, &EVulkan::Device);
		if (result != VK_SUCCESS)
		{
			EVulkan::Device = VK_NULL_HANDLE;
			SDL_LogError(
				SDL_LOG_CATEGORY_APPLICATION, "vkCreateDevice(): %s\n", EVulkanError::ResultToString(result));
			return false;
		}

		// Load Device Functions
		{
#define VULKAN_DEVICE_FUNCTION(name)                                             \
    name = EVulkan::GetDeviceProcessAddress<PFN_##name>(#name);                  \
    if(!name)                                                                    \
    {                                                                            \
        return false;                                                            \
    }
			VULKAN_DEVICE_FUNCTIONS();
#undef VULKAN_DEVICE_FUNCTION
		}

		vkGetDeviceQueue(EVulkan::Device,
			EVulkan::GraphicsQueueFamilyIndex,
			0,
			&EVulkan::GraphicsQueue);
		if (EVulkan::GraphicsQueueFamilyIndex != EVulkan::PresentQueueFamilyIndex)
			vkGetDeviceQueue(EVulkan::Device,
				EVulkan::PresentQueueFamilyIndex,
				0,
				&EVulkan::PresentQueue);
		else
			EVulkan::PresentQueue = EVulkan::GraphicsQueue;

		return true;
	}

	void DestroyLogicalDevice()
	{
		if (EVulkan::Device != VK_NULL_HANDLE)
		{
			vkDestroyDevice(EVulkan::Device, nullptr);
			EVulkan::Device = VK_NULL_HANDLE;
		}
	}

	const mu_boolean CreateSync()
	{
		VkSemaphoreCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VkResult result = vkCreateSemaphore(EVulkan::Device, &createInfo, NULL, &EVulkan::ImageAvailableSemaphore);
		if (result != VK_SUCCESS)
		{
			EVulkan::ImageAvailableSemaphore = VK_NULL_HANDLE;
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkCreateSemaphore(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}

		result = vkCreateSemaphore(EVulkan::Device, &createInfo, NULL, &EVulkan::RenderingFinishedSemaphore);
		if (result != VK_SUCCESS)
		{
			EVulkan::ImageAvailableSemaphore = VK_NULL_HANDLE;
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkCreateSemaphore(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}

		result = vkCreateSemaphore(EVulkan::Device, &createInfo, NULL, &EVulkan::TransferFinishedSemaphore);
		if (result != VK_SUCCESS)
		{
			EVulkan::ImageAvailableSemaphore = VK_NULL_HANDLE;
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkCreateSemaphore(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}

		result = vkCreateSemaphore(EVulkan::Device, &createInfo, NULL, &EVulkan::ShadowMapFinishedSemaphore);
		if (result != VK_SUCCESS)
		{
			EVulkan::ImageAvailableSemaphore = VK_NULL_HANDLE;
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkCreateSemaphore(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}

		return true;
	}

	void DestroySync()
	{
		if (EVulkan::ImageAvailableSemaphore != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(EVulkan::Device, EVulkan::ImageAvailableSemaphore, nullptr);
			EVulkan::ImageAvailableSemaphore = VK_NULL_HANDLE;
		}

		if (EVulkan::RenderingFinishedSemaphore != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(EVulkan::Device, EVulkan::RenderingFinishedSemaphore, nullptr);
			EVulkan::RenderingFinishedSemaphore = VK_NULL_HANDLE;
		}

		if (EVulkan::TransferFinishedSemaphore != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(EVulkan::Device, EVulkan::TransferFinishedSemaphore, nullptr);
			EVulkan::TransferFinishedSemaphore = VK_NULL_HANDLE;
		}

		if (EVulkan::ShadowMapFinishedSemaphore != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(EVulkan::Device, EVulkan::ShadowMapFinishedSemaphore, nullptr);
			EVulkan::ShadowMapFinishedSemaphore = VK_NULL_HANDLE;
		}
	}

	const mu_boolean GetSurfaceCapabilities()
	{
		VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(EVulkan::PhysicalDevice, EVulkan::Surface, &EVulkan::SurfaceCapabilities);
		if (result != VK_SUCCESS)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkGetPhysicalDeviceSurfaceCapabilitiesKHR(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}

		// check surface usage
		if (!(EVulkan::SurfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT))
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"Vulkan surface doesn't support VK_IMAGE_USAGE_TRANSFER_DST_BIT\n");
			return false;
		}

		return true;
	}

	const mu_boolean GetSurfaceFormat()
	{
		VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(EVulkan::PhysicalDevice,
			EVulkan::Surface,
			&EVulkan::SurfaceFormatsCount,
			nullptr);
		if (result != VK_SUCCESS)
		{
			EVulkan::SurfaceFormatsCount = 0;
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkGetPhysicalDeviceSurfaceFormatsKHR(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}
		if (EVulkan::SurfaceFormatsCount > EVulkan::SurfaceFormats.size())
		{
			EVulkan::SurfaceFormats.resize(EVulkan::SurfaceFormatsCount);
		}
		result = vkGetPhysicalDeviceSurfaceFormatsKHR(EVulkan::PhysicalDevice,
			EVulkan::Surface,
			&EVulkan::SurfaceFormatsCount,
			EVulkan::SurfaceFormats.data());
		if (result != VK_SUCCESS)
		{
			EVulkan::SurfaceFormatsCount = 0;
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkGetPhysicalDeviceSurfaceFormatsKHR(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}

		return true;
	}

	void UpdateSamplesCount(const VkSampleCountFlags DesiredSamples)
	{
		const VkSampleCountFlags maxSamples = EMath::Min(EVulkan::DeviceProperties.limits.framebufferColorSampleCounts,
			EVulkan::DeviceProperties.limits.framebufferDepthSampleCounts);

		mu_uint32 n = DesiredSamples;
		for(;n > VK_SAMPLE_COUNT_1_BIT;n >>= 1)
		{
			if((maxSamples & n) != 0)
			{
				break;
			}
		}
		EVulkan::DesiredSamplesCount = n;
	}

	const mu_boolean UpdateSwapchainSize(SDL_Window *SDLWindow)
	{
		mu_int32 w = 0, h = 0;
		SDL_Vulkan_GetDrawableSize(SDLWindow, &w, &h);
		EVulkan::SwapchainSize.width = w;
		EVulkan::SwapchainSize.height = h;
		if (w == 0 || h == 0)
			return false;

		return true;
	}

	const mu_boolean CreateSwapchain()
	{
		mu_uint32 i;
		VkSwapchainCreateInfoKHR createInfo = {};

		mu_uint32 presentModeCount = 0;
		VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(EVulkan::PhysicalDevice, EVulkan::Surface, &presentModeCount, nullptr);
		if (result != VK_SUCCESS)
		{
			return false;
		}

		VkPresentModeKHR *presentModes = new VkPresentModeKHR[presentModeCount];
		if (presentModes == nullptr)
		{
			return false;
		}

		result = vkGetPhysicalDeviceSurfacePresentModesKHR(EVulkan::PhysicalDevice, EVulkan::Surface, &presentModeCount, presentModes);
		if (result != VK_SUCCESS)
		{
			delete[] presentModes;
			return false;
		}

#if 0
		VkPresentModeKHR swapchainPresentMode = presentModes[0];
		for (i = 1; i < presentModeCount; ++i)
		{
			if (presentModes[i] == VK_PRESENT_MODE_FIFO_KHR)
			{
				swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
				break;
			}
		}
#else
		constexpr VkPresentModeKHR desiredMode = VK_PRESENT_MODE_MAILBOX_KHR;
		constexpr VkPresentModeKHR alternativeMode = VK_PRESENT_MODE_FIFO_KHR;
		VkPresentModeKHR swapchainPresentMode = presentModes[0];
		for (i = 0; i < presentModeCount; ++i)
		{
			if (presentModes[i] == desiredMode)
			{
				swapchainPresentMode = desiredMode;
				break;
			}
			else if (presentModes[i] == alternativeMode)
			{
				swapchainPresentMode = alternativeMode;
			}
		}
#endif

		delete[] presentModes;

		// pick an image count
		EVulkan::SwapchainDesiredImageCount = EVulkan::SurfaceCapabilities.minImageCount + 1;
		if (EVulkan::SwapchainDesiredImageCount > EVulkan::SurfaceCapabilities.maxImageCount
			&& EVulkan::SurfaceCapabilities.maxImageCount > 0)
			EVulkan::SwapchainDesiredImageCount = EVulkan::SurfaceCapabilities.maxImageCount;

		// pick a format
		if (EVulkan::SurfaceFormatsCount == 1
			&& EVulkan::SurfaceFormats[0].format == VK_FORMAT_UNDEFINED)
		{
			// aren't any preferred formats, so we pick
			EVulkan::SurfaceFormat.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
			EVulkan::SurfaceFormat.format = VK_FORMAT_R8G8B8A8_UNORM;
		}
		else
		{
			EVulkan::SurfaceFormat = EVulkan::SurfaceFormats[0];
			for (i = 0; i < EVulkan::SurfaceFormatsCount; i++)
			{
				if (EVulkan::SurfaceFormats[i].format == VK_FORMAT_R8G8B8A8_UNORM)
				{
					EVulkan::SurfaceFormat = EVulkan::SurfaceFormats[i];
					break;
				}
			}
		}

		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = EVulkan::Surface;
		createInfo.minImageCount = EVulkan::SwapchainDesiredImageCount;
		createInfo.imageFormat = EVulkan::SurfaceFormat.format;
		createInfo.imageColorSpace = EVulkan::SurfaceFormat.colorSpace;
		createInfo.imageExtent = EVulkan::SwapchainSize;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		if (EVulkan::SurfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
		{
			createInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.preTransform = EVulkan::SurfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR != 0 ? VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR :
			EVulkan::SurfaceCapabilities.currentTransform;
		createInfo.compositeAlpha = EVulkan::SurfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR != 0 ? VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR : VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
		createInfo.presentMode = swapchainPresentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = EVulkan::Swapchain;

		result = vkCreateSwapchainKHR(EVulkan::Device, &createInfo, nullptr, &EVulkan::Swapchain);
		if (createInfo.oldSwapchain)
			vkDestroySwapchainKHR(EVulkan::Device, createInfo.oldSwapchain, nullptr);

		if (result != VK_SUCCESS)
		{
			EVulkan::Swapchain = VK_NULL_HANDLE;
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkCreateSwapchainKHR(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}

		result = vkGetSwapchainImagesKHR(EVulkan::Device, EVulkan::Swapchain, &EVulkan::SwapchainImagesCount, nullptr);
		if (result != VK_SUCCESS)
		{
			EVulkan::SwapchainImagesCount = 0;
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkGetSwapchainImagesKHR(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}

		EVulkan::SwapchainImages.resize(EVulkan::SwapchainImagesCount, VK_NULL_HANDLE);
		result = vkGetSwapchainImagesKHR(EVulkan::Device,
			EVulkan::Swapchain,
			&EVulkan::SwapchainImagesCount,
			EVulkan::SwapchainImages.data());
		if (result != VK_SUCCESS)
		{
			EVulkan::SwapchainImagesCount = 0;
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkGetSwapchainImagesKHR(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}

		return true;
	}

	void DestroySwapchain()
	{
		EVulkan::SwapchainImagesCount = 0;
		EVulkan::SwapchainImages.clear();

		if (EVulkan::Swapchain != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(EVulkan::Device, EVulkan::Swapchain, nullptr);
			EVulkan::Swapchain = VK_NULL_HANDLE;
		}
	}

	const mu_boolean CreateSwapchainView()
	{
		EVulkan::SwapchainImagesView.resize(EVulkan::SwapchainImagesCount, VK_NULL_HANDLE);

		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = EVulkan::SurfaceFormat.format;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		for (mu_uint32 n = 0; n < EVulkan::SwapchainImagesCount; ++n)
		{
			createInfo.image = EVulkan::SwapchainImages[n];

			VkResult result = vkCreateImageView(EVulkan::Device, &createInfo, nullptr, &EVulkan::SwapchainImagesView[n]);
			if (result != VK_SUCCESS)
			{
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
					"vkCreateImageView(): %s\n",
					EVulkanError::ResultToString(result));
				return false;
			}
		}

		return true;
	}

	void DestroySwapchainView()
	{
		for (auto it = EVulkan::SwapchainImagesView.begin(); it != EVulkan::SwapchainImagesView.end(); ++it)
		{
			VkImageView tmp = *it;
			if (tmp != VK_NULL_HANDLE)
			{
				vkDestroyImageView(EVulkan::Device, tmp, nullptr);
			}
		}
		EVulkan::SwapchainImagesView.clear();
	}

	const mu_boolean CreateColor()
	{
		VkImageCreateInfo image = {};
		image.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image.pNext = NULL;
		image.imageType = VK_IMAGE_TYPE_2D;
		image.format = EVulkan::SurfaceFormat.format;
		image.extent = { EVulkan::SwapchainSize.width, EVulkan::SwapchainSize.height, 1 };
		image.mipLevels = 1;
		image.arrayLayers = 1;
		image.samples = static_cast<VkSampleCountFlagBits>(EVulkan::DesiredSamplesCount);
		image.tiling = VK_IMAGE_TILING_OPTIMAL;
		image.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		image.flags = 0;

		VmaAllocationCreateInfo allocationInfo = {};
		allocationInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
		allocationInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		allocationInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		allocationInfo.preferredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		VkResult result = vmaCreateImage(EVulkanMemory::MemoryAllocator, &image, &allocationInfo, &EVulkan::ColorImage, &EVulkan::ColorMemory, nullptr);
		if (result != VK_SUCCESS)
		{
			return false;
		}

		VkImageViewCreateInfo imageViewInfo = {};
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.pNext = NULL;
		imageViewInfo.flags = 0;
		imageViewInfo.image = EVulkan::ColorImage;
		imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format = EVulkan::SurfaceFormat.format;
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		imageViewInfo.subresourceRange = {};
		imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = 1;
		result = vkCreateImageView(EVulkan::Device, &imageViewInfo, nullptr, &EVulkan::ColorImageView);
		if (result != VK_SUCCESS)
		{
			return false;
		}

		return true;
	}

	void DestroyColor()
	{
		EVulkanMemory::DestroyImageView(EVulkan::ColorImageView);
		EVulkanMemory::DestroyImage(EVulkan::ColorImage, EVulkan::ColorMemory);
	}

	const mu_boolean CreateDepthStencil()
	{
		std::array<VkFormat, 5> depthFormats =
		{ {
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM
		} };

		EVulkan::DepthStencilFormat = VK_FORMAT_MAX_ENUM;
		for (auto& format : depthFormats)
		{
			VkFormatProperties formatProps;
			vkGetPhysicalDeviceFormatProperties(EVulkan::PhysicalDevice, format, &formatProps);
			if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
			{
				EVulkan::DepthStencilFormat = format;
				break;
			}
		}

		if (EVulkan::DepthStencilFormat == VK_FORMAT_MAX_ENUM)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"No depth format available\n");
			return false;
		}

		VkImageCreateInfo image = {};
		image.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image.pNext = NULL;
		image.imageType = VK_IMAGE_TYPE_2D;
		image.format = EVulkan::DepthStencilFormat;
		image.extent = { EVulkan::SwapchainSize.width, EVulkan::SwapchainSize.height, 1 };
		image.mipLevels = 1;
		image.arrayLayers = 1;
		image.samples = static_cast<VkSampleCountFlagBits>(EVulkan::DesiredSamplesCount);
		image.tiling = VK_IMAGE_TILING_OPTIMAL;
		image.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		image.flags = 0;

		VmaAllocationCreateInfo allocationInfo = {};
		allocationInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
		allocationInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		allocationInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		allocationInfo.preferredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		VkResult result = vmaCreateImage(EVulkanMemory::MemoryAllocator, &image, &allocationInfo, &EVulkan::DepthStencilImage, &EVulkan::DepthStencilMemory, nullptr);
		if (result != VK_SUCCESS)
		{
			return false;
		}

		VkImageViewCreateInfo imageViewInfo = {};
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.pNext = NULL;
		imageViewInfo.flags = 0;
		imageViewInfo.image = EVulkan::DepthStencilImage;
		imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format = EVulkan::DepthStencilFormat;
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		imageViewInfo.subresourceRange = {};
		imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = 1;
		result = vkCreateImageView(EVulkan::Device, &imageViewInfo, nullptr, &EVulkan::DepthStencilImageView);
		if (result != VK_SUCCESS)
		{
			return false;
		}

		return true;
	}

	void DestroyDepthStencil()
	{
		EVulkanMemory::DestroyImageView(EVulkan::DepthStencilImageView);
		EVulkanMemory::DestroyImage(EVulkan::DepthStencilImage, EVulkan::DepthStencilMemory);
	}

	const mu_boolean CreateCommandPool()
	{
		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		createInfo.queueFamilyIndex = EVulkan::GraphicsQueueFamilyIndex;

		VkResult result = vkCreateCommandPool(EVulkan::Device, &createInfo, nullptr, &EVulkan::MainCommandPool);
		if (result != VK_SUCCESS)
		{
			EVulkan::MainCommandPool = VK_NULL_HANDLE;
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkCreateCommandPool(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}

		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		result = vkCreateCommandPool(EVulkan::Device, &createInfo, nullptr, &EVulkan::SecondaryCommandPool);
		if (result != VK_SUCCESS)
		{
			EVulkan::SecondaryCommandPool = VK_NULL_HANDLE;
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkCreateCommandPool(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}
		
		createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
		result = vkCreateCommandPool(EVulkan::Device, &createInfo, nullptr, &EVulkan::ShortCommandPool);
		if (result != VK_SUCCESS)
		{
			EVulkan::ShortCommandPool = VK_NULL_HANDLE;
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkCreateCommandPool(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}

		return true;
	}

	const mu_boolean CreateCommandBuffers()
	{
		VkCommandBufferAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandPool = EVulkan::MainCommandPool;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandBufferCount = EVulkan::SwapchainImagesCount;

		EVulkan::CommandBuffers.resize(EVulkan::SwapchainImagesCount, nullptr);
		VkResult result = vkAllocateCommandBuffers(EVulkan::Device, &allocateInfo, EVulkan::CommandBuffers.data());
		if (result != VK_SUCCESS)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkAllocateCommandBuffers(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}

		EVulkan::ShadowMapBuffers.resize(EVulkan::SwapchainImagesCount, nullptr);
		result = vkAllocateCommandBuffers(EVulkan::Device, &allocateInfo, EVulkan::ShadowMapBuffers.data());
		if (result != VK_SUCCESS)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkAllocateCommandBuffers(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}

		EVulkan::TransferBuffers.resize(EVulkan::SwapchainImagesCount, nullptr);
		result = vkAllocateCommandBuffers(EVulkan::Device, &allocateInfo, EVulkan::TransferBuffers.data());
		if (result != VK_SUCCESS)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"vkAllocateCommandBuffers(): %s\n",
				EVulkanError::ResultToString(result));
			return false;
		}

		return true;
	}

	const mu_boolean CreateCommandFences()
	{
		EVulkan::CommandFences.resize(EVulkan::SwapchainImagesCount, VK_NULL_HANDLE);

		for (mu_uint32 i = 0; i < EVulkan::SwapchainImagesCount; i++)
		{
			VkFenceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
			VkResult result = vkCreateFence(EVulkan::Device, &createInfo, nullptr, &EVulkan::CommandFences[i]);
			if (result != VK_SUCCESS)
			{
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
					"vkCreateFence(): %s\n",
					EVulkanError::ResultToString(result));
				return false;
			}
		}

		return true;
	}

	void DestroyCommandPool()
	{
		if (EVulkan::MainCommandPool != VK_NULL_HANDLE)
		{
			vkDestroyCommandPool(EVulkan::Device, EVulkan::MainCommandPool, nullptr);
			EVulkan::MainCommandPool = VK_NULL_HANDLE;
		}
		if (EVulkan::SecondaryCommandPool != VK_NULL_HANDLE)
		{
			vkDestroyCommandPool(EVulkan::Device, EVulkan::SecondaryCommandPool, nullptr);
			EVulkan::SecondaryCommandPool = VK_NULL_HANDLE;
		}
		if (EVulkan::ShortCommandPool != VK_NULL_HANDLE)
		{
			vkDestroyCommandPool(EVulkan::Device, EVulkan::ShortCommandPool, nullptr);
			EVulkan::ShortCommandPool = VK_NULL_HANDLE;
		}
	}

	void DestroyCommandBuffers()
	{
		if (EVulkan::TransferBuffers.size() > 0)
		{
			if (EVulkan::TransferBuffers[0] != nullptr)
			{
				vkFreeCommandBuffers(EVulkan::Device, EVulkan::MainCommandPool, EVulkan::SwapchainImagesCount, EVulkan::TransferBuffers.data());
			}
			EVulkan::TransferBuffers.clear();
		}

		if (EVulkan::ShadowMapBuffers.size() > 0)
		{
			if (EVulkan::ShadowMapBuffers[0] != nullptr)
			{
				vkFreeCommandBuffers(EVulkan::Device, EVulkan::MainCommandPool, EVulkan::SwapchainImagesCount, EVulkan::ShadowMapBuffers.data());
			}
			EVulkan::ShadowMapBuffers.clear();
		}

		if (EVulkan::CommandBuffers.size() > 0)
		{
			if (EVulkan::CommandBuffers[0] != nullptr)
			{
				vkFreeCommandBuffers(EVulkan::Device, EVulkan::MainCommandPool, EVulkan::SwapchainImagesCount, EVulkan::CommandBuffers.data());
			}
			EVulkan::CommandBuffers.clear();
		}
	}

	void DestroyCommandFences()
	{
		for (auto it = EVulkan::CommandFences.begin(); it != EVulkan::CommandFences.end(); ++it)
		{
			VkFence tmp = *it;
			if (tmp != VK_NULL_HANDLE)
			{
				vkDestroyFence(EVulkan::Device, tmp, nullptr);
			}
		}
		EVulkan::CommandFences.clear();
	}

	const mu_boolean CreateFramebuffers()
	{
		if(EVulkan::DesiredSamplesCount > VK_SAMPLE_COUNT_1_BIT)
		{
			VkImageView attachments[3] =
			{
				EVulkan::ColorImageView,
				EVulkan::DepthStencilImageView,
				VK_NULL_HANDLE,
			};

			VkFramebufferCreateInfo frameBufferCreateInfo = {};
			frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			frameBufferCreateInfo.pNext = NULL;
			frameBufferCreateInfo.renderPass = EVulkanPipeline::GetRenderPass(ERenderPass::eNormal);
			frameBufferCreateInfo.attachmentCount = mu_countof(attachments);
			frameBufferCreateInfo.pAttachments = attachments;
			frameBufferCreateInfo.width = EVulkan::SwapchainSize.width;
			frameBufferCreateInfo.height = EVulkan::SwapchainSize.height;
			frameBufferCreateInfo.layers = 1;

			EVulkan::Framebuffers.resize(EVulkan::SwapchainImagesCount, VK_NULL_HANDLE);
			for (mu_uint32 n = 0; n < EVulkan::SwapchainImagesCount; ++n)
			{
				attachments[2] = EVulkan::SwapchainImagesView[n];
				VkResult result = vkCreateFramebuffer(EVulkan::Device, &frameBufferCreateInfo, nullptr, &EVulkan::Framebuffers[n]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}
		}
		else
		{
			VkImageView attachments[2] =
			{
				VK_NULL_HANDLE,
				EVulkan::DepthStencilImageView,
			};

			VkFramebufferCreateInfo frameBufferCreateInfo = {};
			frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			frameBufferCreateInfo.pNext = NULL;
			frameBufferCreateInfo.renderPass = EVulkanPipeline::GetRenderPass(ERenderPass::eNormal);
			frameBufferCreateInfo.attachmentCount = mu_countof(attachments);
			frameBufferCreateInfo.pAttachments = attachments;
			frameBufferCreateInfo.width = EVulkan::SwapchainSize.width;
			frameBufferCreateInfo.height = EVulkan::SwapchainSize.height;
			frameBufferCreateInfo.layers = 1;

			EVulkan::Framebuffers.resize(EVulkan::SwapchainImagesCount, VK_NULL_HANDLE);
			for (mu_uint32 n = 0; n < EVulkan::SwapchainImagesCount; ++n)
			{
				attachments[0] = EVulkan::SwapchainImagesView[n];
				VkResult result = vkCreateFramebuffer(EVulkan::Device, &frameBufferCreateInfo, nullptr, &EVulkan::Framebuffers[n]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}
		}

		return true;
	}

	void DestroyFramebuffers()
	{
		for (auto it = EVulkan::Framebuffers.begin(); it != EVulkan::Framebuffers.end(); ++it)
		{
			VkFramebuffer tmp = *it;
			if (tmp != VK_NULL_HANDLE)
			{
				vkDestroyFramebuffer(EVulkan::Device, tmp, nullptr);
			}
		}
		EVulkan::Framebuffers.clear();
	}
};