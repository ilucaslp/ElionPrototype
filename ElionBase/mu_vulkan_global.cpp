#include "stdafx.h"

namespace EVulkan
{
	// Global Variables
	SDL_vulkanInstance Instance = nullptr;
	VkDebugReportCallbackEXT DebugCallback = VK_NULL_HANDLE;
	SDL_vulkanSurface Surface = VK_NULL_HANDLE;
	std::vector<VkSurfaceFormatKHR> SurfaceFormats;
	VkSurfaceFormatKHR SurfaceFormat = {};
	VkFormat DepthStencilFormat = VK_FORMAT_MAX_ENUM;
	VkSurfaceCapabilitiesKHR SurfaceCapabilities = {};
	mu_uint32 SurfaceFormatsCount = 0;
	VkPhysicalDeviceMemoryProperties DeviceMemoryProperties = {};
	VkPhysicalDeviceProperties DeviceProperties = {};
	VkPhysicalDeviceFeatures DeviceFeatures = {};
	VkPhysicalDevice PhysicalDevice = nullptr;
	VkDevice Device = nullptr;
	VkQueue GraphicsQueue = nullptr;
	VkQueue PresentQueue = nullptr;
	mu_uint32 GraphicsQueueFamilyIndex = 0;
	mu_uint32 PresentQueueFamilyIndex = 0;
	mu_uint32 SwapchainDesiredImageCount = 0;
	VkSampleCountFlags DesiredSamplesCount = VK_SAMPLE_COUNT_1_BIT;
	VkExtent2D SwapchainSize = { 0, 0 };
	VkSwapchainKHR Swapchain = VK_NULL_HANDLE;
	mu_uint32 SwapchainImagesCount = 0;
	std::vector<VkImage> SwapchainImages;
	std::vector<VkImageView> SwapchainImagesView;
	VkImage ColorImage = VK_NULL_HANDLE;
	VkImageView ColorImageView = VK_NULL_HANDLE;
	VmaAllocation ColorMemory = nullptr;
	VkImage DepthStencilImage = VK_NULL_HANDLE;
	VkImageView DepthStencilImageView = VK_NULL_HANDLE;
	VmaAllocation DepthStencilMemory = nullptr;
	std::vector<VkFramebuffer> Framebuffers;
	VkCommandPool MainCommandPool = VK_NULL_HANDLE;
	VkCommandPool SecondaryCommandPool = VK_NULL_HANDLE;
	VkCommandPool ShortCommandPool = VK_NULL_HANDLE;
	std::vector<VkCommandBuffer> CommandBuffers;
	std::vector<VkCommandBuffer> TransferBuffers;
	std::vector<VkCommandBuffer> ShadowMapBuffers;
	VkCommandBuffer CurrentCommandBuffer;
	std::vector<VkFence> CommandFences;
	VkSemaphore ImageAvailableSemaphore = VK_NULL_HANDLE;
	VkSemaphore RenderingFinishedSemaphore = VK_NULL_HANDLE;
	VkSemaphore TransferFinishedSemaphore = VK_NULL_HANDLE;
	VkSemaphore ShadowMapFinishedSemaphore = VK_NULL_HANDLE;
	mu_boolean SkeletonTexelBufferEnabled = false;

	mu_uint32 CurrentCommandBufferIndex = 0;
	mu_uint32 CurrentTransferCommandBufferIndex = 0;
	mu_uint32 CurrentSwapchainIndex = 0;
	mu_uint32 LastSwapchainIndex = 0;
};