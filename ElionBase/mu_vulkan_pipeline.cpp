#include "stdafx.h"

namespace EVulkanPipeline
{
	using namespace EVulkan;

	constexpr mu_text *PipelineCacheDirPath = _T("./cache/");
#if CPUINFO_ARCH_X86 == 1
	constexpr mu_text *PipelineCachePathFormat = _T("./cache/vulkan_pipeline_%1%_%2%_%3%.x86");
#elif CPUINFO_ARCH_X86_64 == 1
	constexpr mu_text *PipelineCachePathFormat = _T("./cache/vulkan_pipeline_%1%_%2%_%3%.x64");
#elif CPUINFO_ARCH_ARM == 1
	constexpr mu_text *PipelineCachePathFormat = _T("./cache/vulkan_pipeline_%1%_%2%_%3%.arm");
#elif CPUINFO_ARCH_ARM64 == 1
	constexpr mu_text *PipelineCachePathFormat = _T("./cache/vulkan_pipeline_%1%_%2%_%3%.arm64");
#endif
	VkPipelineCache PipelineCache = VK_NULL_HANDLE;
	mu_size PipelineCacheSize = 0;
	mu_uint8 *PipelineCacheBuffer = nullptr;

	std::array<VkRenderPass, ERenderPassMax> RenderPasses = { { VK_NULL_HANDLE } };
	std::array<VkDescriptorSetLayout, VULKAN_SETLAYOUT_MAX> DescriptorSetLayout = { { VK_NULL_HANDLE } };
	std::array<VkPipelineLayout, EUniformLayoutMax> PipelinesLayout = { { VK_NULL_HANDLE } };
	std::array<VkPipeline, MAX_GRAPHICS_PIPELINE> Pipelines = { { VK_NULL_HANDLE } };

	mu_boolean CreateRenderPasses()
	{
		using namespace EVulkan;
		VkResult result;

		// Create Normal Rendering Pass
		{
			if(EVulkan::DesiredSamplesCount > VK_SAMPLE_COUNT_1_BIT)
			{
				std::array<VkAttachmentDescription, 3> attachments = {};
				// MSAA Color attachment
				attachments[0].format = EVulkan::SurfaceFormat.format;
				attachments[0].samples = static_cast<VkSampleCountFlagBits>(EVulkan::DesiredSamplesCount);
				attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				// MSAA Depth attachment
				attachments[1].format = EVulkan::DepthStencilFormat;
				attachments[1].samples = static_cast<VkSampleCountFlagBits>(EVulkan::DesiredSamplesCount);
				attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				// Color attachment
				attachments[2].format = EVulkan::SurfaceFormat.format;
				attachments[2].samples = VK_SAMPLE_COUNT_1_BIT;
				attachments[2].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachments[2].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachments[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachments[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachments[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachments[2].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

				VkAttachmentReference colorReference = {};
				colorReference.attachment = 0;
				colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				VkAttachmentReference depthReference = {};
				depthReference.attachment = 1;
				depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				VkAttachmentReference colorResolveReference = {};
				colorResolveReference.attachment = 2;
				colorResolveReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				VkSubpassDescription subpassDescription = {};
				subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				subpassDescription.colorAttachmentCount = 1;
				subpassDescription.pColorAttachments = &colorReference;
				subpassDescription.pDepthStencilAttachment = &depthReference;
				subpassDescription.inputAttachmentCount = 0;
				subpassDescription.pInputAttachments = nullptr;
				subpassDescription.preserveAttachmentCount = 0;
				subpassDescription.pPreserveAttachments = nullptr;
				subpassDescription.pResolveAttachments = &colorResolveReference;

				// Subpass dependencies for layout transitions
				std::array<VkSubpassDependency, 2> dependencies;

				dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
				dependencies[0].dstSubpass = 0;
				dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

				dependencies[1].srcSubpass = 0;
				dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
				dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

				VkRenderPassCreateInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
				renderPassInfo.pAttachments = attachments.data();
				renderPassInfo.subpassCount = 1;
				renderPassInfo.pSubpasses = &subpassDescription;
				renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
				renderPassInfo.pDependencies = dependencies.data();

				result = vkCreateRenderPass(EVulkan::Device, &renderPassInfo, nullptr, &RenderPasses[ECastEnum(ERenderPass::eNormal)]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}
			else
			{
				std::array<VkAttachmentDescription, 2> attachments = {};
				// Color attachment
				attachments[0].format = EVulkan::SurfaceFormat.format;
				attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
				attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				// Depth attachment
				attachments[1].format = EVulkan::DepthStencilFormat;
				attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
				attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				VkAttachmentReference colorReference = {};
				colorReference.attachment = 0;
				colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				VkAttachmentReference depthReference = {};
				depthReference.attachment = 1;
				depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				VkSubpassDescription subpassDescription = {};
				subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				subpassDescription.colorAttachmentCount = 1;
				subpassDescription.pColorAttachments = &colorReference;
				subpassDescription.pDepthStencilAttachment = &depthReference;
				subpassDescription.inputAttachmentCount = 0;
				subpassDescription.pInputAttachments = nullptr;
				subpassDescription.preserveAttachmentCount = 0;
				subpassDescription.pPreserveAttachments = nullptr;
				subpassDescription.pResolveAttachments = nullptr;

				// Subpass dependencies for layout transitions
				std::array<VkSubpassDependency, 2> dependencies;

				dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
				dependencies[0].dstSubpass = 0;
				dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

				dependencies[1].srcSubpass = 0;
				dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
				dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

				VkRenderPassCreateInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
				renderPassInfo.pAttachments = attachments.data();
				renderPassInfo.subpassCount = 1;
				renderPassInfo.pSubpasses = &subpassDescription;
				renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
				renderPassInfo.pDependencies = dependencies.data();

				result = vkCreateRenderPass(EVulkan::Device, &renderPassInfo, nullptr, &RenderPasses[ECastEnum(ERenderPass::eNormal)]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}
		}

		// Create Shadow Rendering Pass
		{
			std::array<VkAttachmentDescription, 1> attachments = {};
			// Depth attachment
			attachments[0].format = EVulkanTexture::RetrieveFormat(SHADOWMAP_FORMAT);
			attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
			attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachments[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			VkAttachmentReference depthReference = {};
			depthReference.attachment = 0;
			depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subpassDescription = {};
			subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpassDescription.colorAttachmentCount = 0;
			subpassDescription.pColorAttachments = nullptr;
			subpassDescription.pDepthStencilAttachment = &depthReference;
			subpassDescription.inputAttachmentCount = 0;
			subpassDescription.pInputAttachments = nullptr;
			subpassDescription.preserveAttachmentCount = 0;
			subpassDescription.pPreserveAttachments = nullptr;
			subpassDescription.pResolveAttachments = nullptr;

			// Subpass dependencies for layout transitions
			std::array<VkSubpassDependency, 2> dependencies;

			dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
			dependencies[0].dstSubpass = 0;
			dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			dependencies[0].dstStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			dependencies[0].srcAccessMask = 0;
			dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			dependencies[1].srcSubpass = 0;
			dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
			dependencies[1].srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			dependencies[1].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			VkRenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			renderPassInfo.pAttachments = attachments.data();
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpassDescription;
#if 0
			renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
			renderPassInfo.pDependencies = dependencies.data();
#endif

			result = vkCreateRenderPass(EVulkan::Device, &renderPassInfo, nullptr, &RenderPasses[ECastEnum(ERenderPass::eShadow)]);
			if (result != VK_SUCCESS)
			{
				return false;
			}
		}

		return true;
	}

	void DestroyRenderPasses()
	{
		for (mu_uint32 n = 0; n < RenderPasses.size(); ++n)
		{
			VkRenderPass renderPass = RenderPasses[n];
			if (renderPass != VK_NULL_HANDLE)
			{
				vkDestroyRenderPass(EVulkan::Device, renderPass, nullptr);
				RenderPasses[n] = VK_NULL_HANDLE;
			}
		}
	}

	VkRenderPass GetRenderPass(const ERenderPass index)
	{
		return RenderPasses[ECastEnum(index)];
	}

	void SavePipelineCache()
	{
		if (PipelineCache != VK_NULL_HANDLE)
		{
			mu_size pipelineCacheSize = 0;
			VkResult result = vkGetPipelineCacheData(EVulkan::Device, PipelineCache, &pipelineCacheSize, nullptr);
			if (result == VK_SUCCESS)
			{
				mu_uint8 *pipelineCacheBuffer = new mu_uint8[pipelineCacheSize];

				if (pipelineCacheBuffer != nullptr)
				{
					result = vkGetPipelineCacheData(EVulkan::Device, PipelineCache, &pipelineCacheSize, pipelineCacheBuffer);
					if (result == VK_SUCCESS)
					{
						mu_stringstream PipelineCacheUUID;
						PipelineCacheUUID << std::hex << std::setfill(_T('0')) << std::setw(2) << std::nouppercase;
						for (mu_uint32 n = 0; n < VK_UUID_SIZE; ++n)
						{
							PipelineCacheUUID << static_cast<mu_int32>(EVulkan::DeviceProperties.pipelineCacheUUID[n]);
						}

						boost::basic_format<mu_text> PipelineFormatString = (boost::basic_format<mu_text>(PipelineCachePathFormat) % EVulkan::DeviceProperties.vendorID % EVulkan::DeviceProperties.deviceID % PipelineCacheUUID.str());
						mu_string PipelineCachePath = PipelineFormatString.str();

						MU::MakeDirectory(PipelineCacheDirPath);

						SDL_RWops *file = nullptr;

						if (mu_rwfromfile(&file, PipelineCachePath.c_str(), "wb") == true)
						{
							SDL_RWwrite(file, pipelineCacheBuffer, pipelineCacheSize, 1);
							SDL_RWclose(file);
						}
					}

					MU_SAFE_DELETE_ARRAY(pipelineCacheBuffer);
				}
			}
		}
	}

	mu_boolean CreatePipelineCache()
	{
		VkPipelineCacheCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

		mu_stringstream PipelineCacheUUID;
		PipelineCacheUUID << std::hex << std::setfill(_T('0')) << std::setw(2) << std::nouppercase;
		for(mu_uint32 n = 0; n < VK_UUID_SIZE; ++n)
		{
			PipelineCacheUUID << static_cast<mu_int32>(EVulkan::DeviceProperties.pipelineCacheUUID[n]);
		}

		boost::basic_format<mu_text> PipelineFormatString = (boost::basic_format<mu_text>(PipelineCachePathFormat) % EVulkan::DeviceProperties.vendorID % EVulkan::DeviceProperties.deviceID % PipelineCacheUUID.str());
		mu_string PipelineCachePath = PipelineFormatString.str();

		if (mu_rwexists(PipelineCachePath.c_str()) == true)
		{
			SDL_RWops *file = nullptr;

			if (mu_rwfromfile(&file, PipelineCachePath.c_str(), "rb") == true)
			{
				constexpr mu_size MinimumCacheSize = sizeof(mu_uint32) // Header Size
					+ sizeof(mu_uint32) // Cache Header Version
					+ sizeof(mu_uint32) // Vendor ID
					+ sizeof(mu_uint32) // Device ID
					+ VK_UUID_SIZE; // Pipeline Cache ID

				PipelineCacheSize = SDL_RWsize(file);
				if (PipelineCacheSize > MinimumCacheSize)
				{
					PipelineCacheBuffer = new mu_uint8[PipelineCacheSize];
					SDL_RWread(file, PipelineCacheBuffer, PipelineCacheSize, 1);

					mu_boolean isSafeCache = true;

					MU_ALIGN(4, struct) EPipelineCacheHeader
					{
						mu_uint32 HeaderSize;
						mu_uint32 HeaderVersion;
						mu_uint32 VendorID;
						mu_uint32 DeviceID;
						mu_uint8 pipelineCacheUUID[VK_UUID_SIZE];
					};

					EPipelineCacheHeader *pipelineHeader = reinterpret_cast<EPipelineCacheHeader*>(PipelineCacheBuffer);

					if (pipelineHeader->HeaderSize < MinimumCacheSize)
					{
						isSafeCache = false;
					}
					else if (pipelineHeader->HeaderVersion < VK_PIPELINE_CACHE_HEADER_VERSION_BEGIN_RANGE ||
						pipelineHeader->HeaderVersion > VK_PIPELINE_CACHE_HEADER_VERSION_END_RANGE)
					{
						isSafeCache = false;
					}
					else if (pipelineHeader->VendorID != EVulkan::DeviceProperties.vendorID ||
						pipelineHeader->DeviceID != EVulkan::DeviceProperties.deviceID ||
						mu_memcmp(pipelineHeader->pipelineCacheUUID, EVulkan::DeviceProperties.pipelineCacheUUID, VK_UUID_SIZE) != 0)
					{
						isSafeCache = false;
					}

					if(isSafeCache == true)
					{
						createInfo.initialDataSize = PipelineCacheSize;
						createInfo.pInitialData = PipelineCacheBuffer;
					}
					else
					{
						PipelineCacheSize = 0;
						MU_SAFE_DELETE_ARRAY(PipelineCacheBuffer);
					}
				}

				SDL_RWclose(file);
			}
		}

		VkResult result = vkCreatePipelineCache(EVulkan::Device, &createInfo, nullptr, &PipelineCache);
		if (result != VK_SUCCESS)
		{
			return false;
		}

		return true;
	}

	void DestroyPipelineCache()
	{
		SavePipelineCache();

		if (PipelineCache != VK_NULL_HANDLE)
		{
			vkDestroyPipelineCache(EVulkan::Device, PipelineCache, nullptr);
			PipelineCache = VK_NULL_HANDLE;
		}

		MU_SAFE_DELETE_ARRAY(PipelineCacheBuffer);
	}

	std::vector<VkSampler> vFontSamplers;
	std::vector<VkSampler> vEffectSamplers;
	std::vector<VkSampler> vTerrainSamplers;
	std::vector<VkSampler> vGrassSamplers;
	std::vector<VkSampler> vWaterSamplers;
	std::vector<VkSampler> vShadowCompareSamplers;
	std::vector<VkSampler> vBonesSamplers;

#define BINDING_INSET(x) for(mu_uint32 bi = 0;bi < mu_countof(x);++bi) bindings.push_back(x[bi]);
	mu_boolean CreatePipelineLayouts()
	{
		VkResult result;

		// Configure Immutable Samplers
		{
			vFontSamplers =
			{
				EVulkanSampler::Get(ESampler::eUcVcWcMinNMagNMipN),
			};

			EVulkanBinding::FontTextureBinding[0].pImmutableSamplers = vFontSamplers.data();

			vEffectSamplers.resize(mu_countof(EffectSamplers));
			for(mu_uint32 n = 0;n < mu_countof(EffectSamplers);++n)
			{
				vEffectSamplers[n] = EVulkanSampler::Get(EffectSamplers[n]);
			}

			EVulkanBinding::EffectSamplerBinding[0].pImmutableSamplers = vEffectSamplers.data();

			vTerrainSamplers =
			{
				EVulkanSampler::Get(ESampler::eUrVrWrMinLMagLMipL_Anisotropy),
			};

			EVulkanBinding::TerrainTextureBinding[0].pImmutableSamplers = vTerrainSamplers.data();
			EVulkanBinding::TerrainTextureBinding[1].pImmutableSamplers = vTerrainSamplers.data();

			vGrassSamplers =
			{
				EVulkanSampler::Get(ESampler::eUrVrWrMinLMagLMipL_Anisotropy),
			};

			EVulkanBinding::GrassTextureBinding[0].pImmutableSamplers = vGrassSamplers.data();

			vWaterSamplers =
			{
				EVulkanSampler::Get(ESampler::eUcVcWcMinLMagLMipL_Anisotropy),
			};

			EVulkanBinding::WaterTextureBinding[0].pImmutableSamplers = vWaterSamplers.data();
			EVulkanBinding::WaterTextureBinding[1].pImmutableSamplers = vWaterSamplers.data();

			vShadowCompareSamplers =
			{
				EVulkanSampler::Get(SHADOWMAP_SAMPLER),
			};

			EVulkanBinding::ShadowBinding[0].pImmutableSamplers = vShadowCompareSamplers.data();

			if (EVulkan::SkeletonTexelBufferEnabled == false)
			{
				vBonesSamplers =
				{
					EVulkanSampler::Get(ESampler::eUcVcWcMinNMagNMipN),
				};

				EVulkanBinding::BonesBinding[0].pImmutableSamplers = vBonesSamplers.data();
			}
		}

		// Create Descriptors Set Layout
		{
			// Global Constant Layout
			{
				const mu_uint32 enumIndex = VULKAN_SETLAYOUT_GLOBAL_CONSTANT;
				std::vector<VkDescriptorSetLayoutBinding> bindings;

				BINDING_INSET(EVulkanBinding::GlobalConstantBinding);

				VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
				setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				setLayoutInfo.bindingCount = bindings.size();
				setLayoutInfo.pBindings = bindings.data();

				result = vkCreateDescriptorSetLayout(EVulkan::Device, &setLayoutInfo, nullptr, &DescriptorSetLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Shadow Layout
			{
				const mu_uint32 enumIndex = VULKAN_SETLAYOUT_SHADOW;
				std::vector<VkDescriptorSetLayoutBinding> bindings;

				BINDING_INSET(EVulkanBinding::ShadowBinding);

				VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
				setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				setLayoutInfo.bindingCount = bindings.size();
				setLayoutInfo.pBindings = bindings.data();

				result = vkCreateDescriptorSetLayout(EVulkan::Device, &setLayoutInfo, nullptr, &DescriptorSetLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Bones Layout
			{
				const mu_uint32 enumIndex = VULKAN_SETLAYOUT_BONES;
				std::vector<VkDescriptorSetLayoutBinding> bindings;

				BINDING_INSET(EVulkanBinding::BonesBinding);

				VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
				setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				setLayoutInfo.bindingCount = bindings.size();
				setLayoutInfo.pBindings = bindings.data();

				result = vkCreateDescriptorSetLayout(EVulkan::Device, &setLayoutInfo, nullptr, &DescriptorSetLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Terrain Layout
			{
				const mu_uint32 enumIndex = VULKAN_SETLAYOUT_TERRAIN_DATA;
				std::vector<VkDescriptorSetLayoutBinding> bindings;

				BINDING_INSET(EVulkanBinding::TerrainBinding);

				VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
				setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				setLayoutInfo.bindingCount = bindings.size();
				setLayoutInfo.pBindings = bindings.data();

				result = vkCreateDescriptorSetLayout(EVulkan::Device, &setLayoutInfo, nullptr, &DescriptorSetLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Terrain Texture Layout
			{
				const mu_uint32 enumIndex = VULKAN_SETLAYOUT_TERRAIN_TEXTURE;
				std::vector<VkDescriptorSetLayoutBinding> bindings;

				BINDING_INSET(EVulkanBinding::TerrainTextureBinding);

				VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
				setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				setLayoutInfo.bindingCount = bindings.size();
				setLayoutInfo.pBindings = bindings.data();

				result = vkCreateDescriptorSetLayout(EVulkan::Device, &setLayoutInfo, nullptr, &DescriptorSetLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Grass Texture Layout
			{
				const mu_uint32 enumIndex = VULKAN_SETLAYOUT_GRASS_TEXTURE;
				std::vector<VkDescriptorSetLayoutBinding> bindings;

				BINDING_INSET(EVulkanBinding::GrassTextureBinding);

				VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
				setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				setLayoutInfo.bindingCount = bindings.size();
				setLayoutInfo.pBindings = bindings.data();

				result = vkCreateDescriptorSetLayout(EVulkan::Device, &setLayoutInfo, nullptr, &DescriptorSetLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Water Texture Layout
			{
				const mu_uint32 enumIndex = VULKAN_SETLAYOUT_WATER_TEXTURE;
				std::vector<VkDescriptorSetLayoutBinding> bindings;

				BINDING_INSET(EVulkanBinding::WaterTextureBinding);

				VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
				setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				setLayoutInfo.bindingCount = bindings.size();
				setLayoutInfo.pBindings = bindings.data();

				result = vkCreateDescriptorSetLayout(EVulkan::Device, &setLayoutInfo, nullptr, &DescriptorSetLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Font Constant Layout
			{
				const mu_uint32 enumIndex = VULKAN_SETLAYOUT_FONT_CONSTANT;
				std::vector<VkDescriptorSetLayoutBinding> bindings;

				BINDING_INSET(EVulkanBinding::FontConstantBinding);

				VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
				setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				setLayoutInfo.bindingCount = bindings.size();
				setLayoutInfo.pBindings = bindings.data();

				result = vkCreateDescriptorSetLayout(EVulkan::Device, &setLayoutInfo, nullptr, &DescriptorSetLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Font Texture Layout
			{
				const mu_uint32 enumIndex = VULKAN_SETLAYOUT_FONT_TEXTURE;
				std::vector<VkDescriptorSetLayoutBinding> bindings;

				BINDING_INSET(EVulkanBinding::FontTextureBinding);

				VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
				setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				setLayoutInfo.bindingCount = bindings.size();
				setLayoutInfo.pBindings = bindings.data();

				result = vkCreateDescriptorSetLayout(EVulkan::Device, &setLayoutInfo, nullptr, &DescriptorSetLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Texture Layout
			{
				const mu_uint32 enumIndex = VULKAN_SETLAYOUT_TEXTURE;
				std::vector<VkDescriptorSetLayoutBinding> bindings;

				BINDING_INSET(EVulkanBinding::TextureBinding);

				VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
				setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				setLayoutInfo.bindingCount = bindings.size();
				setLayoutInfo.pBindings = bindings.data();

				result = vkCreateDescriptorSetLayout(EVulkan::Device, &setLayoutInfo, nullptr, &DescriptorSetLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Sampler Layout
			{
				const mu_uint32 enumIndex = VULKAN_SETLAYOUT_SAMPLER;
				std::vector<VkDescriptorSetLayoutBinding> bindings;

				BINDING_INSET(EVulkanBinding::SamplerBinding);

				VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
				setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				setLayoutInfo.bindingCount = bindings.size();
				setLayoutInfo.pBindings = bindings.data();

				result = vkCreateDescriptorSetLayout(EVulkan::Device, &setLayoutInfo, nullptr, &DescriptorSetLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Effect Sampler Layout
			{
				const mu_uint32 enumIndex = VULKAN_SETLAYOUT_EFFECT_SAMPLER;
				std::vector<VkDescriptorSetLayoutBinding> bindings;

				BINDING_INSET(EVulkanBinding::EffectSamplerBinding);

				VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
				setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				setLayoutInfo.bindingCount = bindings.size();
				setLayoutInfo.pBindings = bindings.data();

				result = vkCreateDescriptorSetLayout(EVulkan::Device, &setLayoutInfo, nullptr, &DescriptorSetLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}
		}

		// Create Pipeline Layouts
		{
			// Global Constant Only
			{
				const mu_uint32 enumIndex = static_cast<mu_uint32>(EUniformLayout::eGlobalConstantOnly);
				std::vector<VkDescriptorSetLayout> setLayouts;

				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_GLOBAL_CONSTANT]);

				VkPipelineLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				layoutInfo.setLayoutCount = setLayouts.size();
				layoutInfo.pSetLayouts = setLayouts.data();

				result = vkCreatePipelineLayout(EVulkan::Device, &layoutInfo, nullptr, &PipelinesLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// UI Pipeline
			{
				const mu_uint32 enumIndex = static_cast<mu_uint32>(EUniformLayout::eUIDefault);
				std::vector<VkDescriptorSetLayout> setLayouts;

				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_GLOBAL_CONSTANT]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_TEXTURE]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_SAMPLER]);

				VkPipelineLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				layoutInfo.setLayoutCount = setLayouts.size();
				layoutInfo.pSetLayouts = setLayouts.data();

				result = vkCreatePipelineLayout(EVulkan::Device, &layoutInfo, nullptr, &PipelinesLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Font Pipeline
			{
				const mu_uint32 enumIndex = static_cast<mu_uint32>(EUniformLayout::eFont);
				std::vector<VkDescriptorSetLayout> setLayouts;

				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_GLOBAL_CONSTANT]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_FONT_CONSTANT]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_FONT_TEXTURE]);

				VkPipelineLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				layoutInfo.setLayoutCount = setLayouts.size();
				layoutInfo.pSetLayouts = setLayouts.data();

				result = vkCreatePipelineLayout(EVulkan::Device, &layoutInfo, nullptr, &PipelinesLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Terrain Pipeline
			{
				const mu_uint32 enumIndex = static_cast<mu_uint32>(EUniformLayout::eTerrainCommon);
				std::vector<VkDescriptorSetLayout> setLayouts;

				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_GLOBAL_CONSTANT]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_TERRAIN_TEXTURE]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_SHADOW]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_TERRAIN_DATA]);

				VkPipelineLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				layoutInfo.setLayoutCount = setLayouts.size();
				layoutInfo.pSetLayouts = setLayouts.data();

				result = vkCreatePipelineLayout(EVulkan::Device, &layoutInfo, nullptr, &PipelinesLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Grass Pipeline
			{
				const mu_uint32 enumIndex = static_cast<mu_uint32>(EUniformLayout::eGrass);
				std::vector<VkDescriptorSetLayout> setLayouts;

				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_GLOBAL_CONSTANT]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_GRASS_TEXTURE]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_SHADOW]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_TERRAIN_DATA]);

				VkPipelineLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				layoutInfo.setLayoutCount = setLayouts.size();
				layoutInfo.pSetLayouts = setLayouts.data();

				result = vkCreatePipelineLayout(EVulkan::Device, &layoutInfo, nullptr, &PipelinesLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Water Pipeline
			{
				const mu_uint32 enumIndex = static_cast<mu_uint32>(EUniformLayout::eWater);
				std::vector<VkDescriptorSetLayout> setLayouts;

				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_GLOBAL_CONSTANT]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_WATER_TEXTURE]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_SHADOW]);

				VkPipelineLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				layoutInfo.setLayoutCount = setLayouts.size();
				layoutInfo.pSetLayouts = setLayouts.data();

				result = vkCreatePipelineLayout(EVulkan::Device, &layoutInfo, nullptr, &PipelinesLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Model Pipeline
			{
				const mu_uint32 enumIndex = static_cast<mu_uint32>(EUniformLayout::eModel);
				std::vector<VkDescriptorSetLayout> setLayouts;

				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_GLOBAL_CONSTANT]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_TEXTURE]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_BONES]);

				VkPipelineLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				layoutInfo.setLayoutCount = setLayouts.size();
				layoutInfo.pSetLayouts = setLayouts.data();

				result = vkCreatePipelineLayout(EVulkan::Device, &layoutInfo, nullptr, &PipelinesLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Shadow Pipeline
			{
				const mu_uint32 enumIndex = static_cast<mu_uint32>(EUniformLayout::eShadow);
				std::vector<VkDescriptorSetLayout> setLayouts;

				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_GLOBAL_CONSTANT]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_TEXTURE]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_BONES]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_TERRAIN_DATA]);

				VkPipelineLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				layoutInfo.setLayoutCount = setLayouts.size();
				layoutInfo.pSetLayouts = setLayouts.data();

				result = vkCreatePipelineLayout(EVulkan::Device, &layoutInfo, nullptr, &PipelinesLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}

			// Effect Pipeline
			{
				const mu_uint32 enumIndex = static_cast<mu_uint32>(EUniformLayout::eEffect);
				std::vector<VkDescriptorSetLayout> setLayouts;

				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_GLOBAL_CONSTANT]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_TEXTURE]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_EFFECT_SAMPLER]);
				setLayouts.push_back(DescriptorSetLayout[VULKAN_SETLAYOUT_TERRAIN_DATA]);

				VkPipelineLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
				layoutInfo.setLayoutCount = setLayouts.size();
				layoutInfo.pSetLayouts = setLayouts.data();

				result = vkCreatePipelineLayout(EVulkan::Device, &layoutInfo, nullptr, &PipelinesLayout[enumIndex]);
				if (result != VK_SUCCESS)
				{
					return false;
				}
			}
		}

		return true;
	}
#undef BINDING_INSERT

	void DestroyPipelineLayouts()
	{
		for(mu_size n = 0;n < PipelinesLayout.size();++n)
		{
			if(PipelinesLayout[n] != VK_NULL_HANDLE)
			{
				vkDestroyPipelineLayout(EVulkan::Device, PipelinesLayout[n], nullptr);
			}
		}

		for (mu_uint32 n = 0; n < DescriptorSetLayout.size(); ++n)
		{
			VkDescriptorSetLayout layout = DescriptorSetLayout[n];
			if (layout != VK_NULL_HANDLE)
			{
				vkDestroyDescriptorSetLayout(EVulkan::Device, layout, nullptr);
				DescriptorSetLayout[n] = VK_NULL_HANDLE;
			}
		}
	}

	VkPipelineLayout GetPipelineLayout(const EUniformLayout index)
	{
		return PipelinesLayout[static_cast<mu_uint32>(index)];
	}

	VkDescriptorSetLayout GetSetLayout(const mu_uint32 index)
	{
		return DescriptorSetLayout[index];
	}

	mu_boolean CreatePipelines(const ECombinedShader begin, const ECombinedShader end)
	{
		using namespace EVulkan;
		using namespace EVulkanShader;

		VkPipelineMultisampleStateCreateInfo multisampleInfo = {};
		multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleInfo.rasterizationSamples = static_cast<VkSampleCountFlagBits>(EVulkan::DesiredSamplesCount);
		if(EVulkan::DesiredSamplesCount > VK_SAMPLE_COUNT_1_BIT &&
			EVulkan::DeviceFeatures.sampleRateShading == VK_TRUE)
		{
			multisampleInfo.sampleShadingEnable = VK_TRUE;
			multisampleInfo.minSampleShading = 0.25f;
		}

		VkPipelineMultisampleStateCreateInfo multisampleShadowInfo = {};
		multisampleShadowInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleShadowInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		const VkDynamicState dynamicStates[] =
		{
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR,
		};

		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
		dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateCreateInfo.dynamicStateCount = mu_countof(dynamicStates);
		dynamicStateCreateInfo.pDynamicStates = dynamicStates;

		VkPipelineViewportStateCreateInfo vpStateInfo = {};
		vpStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		vpStateInfo.viewportCount = 1;
		vpStateInfo.scissorCount = 1;

		VkPipelineTessellationStateCreateInfo tessellationInfo = {};
		tessellationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.pDynamicState = &dynamicStateCreateInfo;
		pipelineInfo.pViewportState = &vpStateInfo;
		pipelineInfo.pTessellationState = &tessellationInfo;

		std::array<VkGraphicsPipelineCreateInfo, GRAPHICS_PIPELINE_VARIATIONS> pipelinesInfo;

		for (mu_uint32 n = ECastEnum(begin); n < ECastEnum(end); ++n)
		{
			mu_uint32 shaderStagesCount = 0;
			VkPipelineShaderStageCreateInfo shaderStages[16];
			mu_zeromem(shaderStages, sizeof(shaderStages));

			const ECombinedShaderData &data = CombinedShaderList[n];

			if(data.Vertex != EVertexShader::eInvalid)
			{
				VkPipelineShaderStageCreateInfo &vertexShaderStageInfo = shaderStages[shaderStagesCount++];
				vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
				vertexShaderStageInfo.module = VertexShaderModule[static_cast<mu_uint32>(data.Vertex)];
				vertexShaderStageInfo.pName = "main";
			}

#if GEOMETRY_SHADER_ENABLE == 1
			if (data.Geometry != EGeometryShader::eInvalid)
			{
				VkPipelineShaderStageCreateInfo &geometryShaderStageInfo = shaderStages[shaderStagesCount++];
				geometryShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				geometryShaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
				geometryShaderStageInfo.module = GeometryShaderModule[static_cast<mu_uint32>(data.Geometry)];
				geometryShaderStageInfo.pName = "main";
			}
#endif

			if (data.Fragment != EFragmentShader::eInvalid)
			{
				VkPipelineShaderStageCreateInfo &pixelShaderStageInfo = shaderStages[shaderStagesCount++];
				pixelShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				pixelShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				pixelShaderStageInfo.module = FragmentShaderModule[static_cast<mu_uint32>(data.Fragment)];
				pixelShaderStageInfo.pName = "main";
			}

			VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
			vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

			EVulkanAttributes::Retrieve(data.InputLayout,
				vertexInputInfo.vertexBindingDescriptionCount,
				vertexInputInfo.pVertexBindingDescriptions,
				vertexInputInfo.vertexAttributeDescriptionCount,
				vertexInputInfo.pVertexAttributeDescriptions);

			mu_uint32 renderPass = ECastEnum(data.RenderPass);
			if(data.RenderPass == ERenderPass::eShadow &&
				MU_RESOURCE->GetShadowType() >= EShadowType::eNonShadowMappingBegin &&
				MU_RESOURCE->GetShadowType() <= EShadowType::eNonShadowMappingEnd)
			{
				renderPass = ECastEnum(ERenderPass::eNormal);
			}

			pipelineInfo.stageCount = shaderStagesCount;
			pipelineInfo.pStages = shaderStages;
			pipelineInfo.pVertexInputState = &vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &EVulkan::TopologyList[static_cast<mu_uint32>(data.TopologyType)];
			pipelineInfo.pMultisampleState = data.RenderPass == ERenderPass::eNormal ? &multisampleInfo : &multisampleShadowInfo;
			pipelineInfo.layout = GetPipelineLayout(data.UniformLayout);
			pipelineInfo.renderPass = RenderPasses[renderPass];
			pipelineInfo.subpass = 0;
			pipelineInfo.flags = VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
			pipelineInfo.basePipelineIndex = -1;

			for (mu_uint32 j = 0, rIndex = 0; rIndex < ERasterizerMax; ++rIndex)
			{
				pipelineInfo.pRasterizationState = &RasterizerList[rIndex];

				for (mu_uint32 dIndex = 0; dIndex < EDepthStencilMax; ++dIndex)
				{
					pipelineInfo.pDepthStencilState = &DepthStencilList[dIndex];

					for (mu_uint32 bIndex = 0; bIndex < EBlendStateMax; ++bIndex)
					{
						pipelineInfo.pColorBlendState = &BlendStateList[bIndex];

						const mu_uint32 pipelineIndex = j++;
						mu_memcpy(&pipelinesInfo[pipelineIndex], &pipelineInfo, sizeof(pipelineInfo));

						pipelineInfo.flags = VK_PIPELINE_CREATE_DERIVATIVE_BIT;
						pipelineInfo.basePipelineIndex = 0;
					}
				}
			}

			const mu_uint32 pipelinesIndex = CreatePipelineIndex(static_cast<ECombinedShader>(n), static_cast<ERasterizer>(0), static_cast<EDepthStencil>(0), static_cast<EBlendState>(0));

			VkResult result = vkCreateGraphicsPipelines(EVulkan::Device, PipelineCache, GRAPHICS_PIPELINE_VARIATIONS, pipelinesInfo.data(), nullptr, &Pipelines[pipelinesIndex]);
			if (result != VK_SUCCESS)
			{
				return false;
			}
		}

		SavePipelineCache();

		return true;
	}

	void DestroyPipelines()
	{
		for (mu_uint32 n = 0; n < Pipelines.size(); ++n)
		{
			VkPipeline pipeline = Pipelines[n];
			if (pipeline != VK_NULL_HANDLE)
			{
				vkDestroyPipeline(EVulkan::Device, pipeline, nullptr);
				Pipelines[n] = VK_NULL_HANDLE;
			}
		}
	}

	mu_pipelinetype CurrentPipelineIndex[VK_PIPELINE_BIND_POINT_RANGE_SIZE];

	void ClearPipeline()
	{
		mu_memset(CurrentPipelineIndex, 0xFF, sizeof(CurrentPipelineIndex));
	}

	void SetPipeline(const VkPipelineBindPoint Bindpoint,
		const ECombinedShader Program,
		const ERasterizer Rasterizer,
		const EDepthStencil DepthStencil,
		const EBlendState BlendState)
	{
		const mu_pipelinetype pipelineIndex = CreatePipelineIndex(Program, Rasterizer, DepthStencil, BlendState);

		if(CurrentPipelineIndex[Bindpoint] != pipelineIndex)
		{
			CurrentPipelineIndex[Bindpoint] = pipelineIndex;
			vkCmdBindPipeline(EVulkan::CurrentCommandBuffer, Bindpoint, Pipelines[pipelineIndex]);
		}
	}

	mu_boolean CurrentScissorState = false;
	void SetScissor(const mu_boolean Enable)
	{
		if(CurrentScissorState != Enable)
		{
			if(Enable == false)
			{
				EVideoBackend::CleanScissor();
			}
			CurrentScissorState = Enable;
		}
	}

	VkBuffer IndexBuffer = VK_NULL_HANDLE;
	std::array<VkBuffer, 16> VertexBuffers = {};
	std::array<std::array<VkDescriptorSet, 4>, VK_PIPELINE_BIND_POINT_RANGE_SIZE> Descriptors = { {} };

	void ClearBindBuffers()
	{
		IndexBuffer = VK_NULL_HANDLE;
		ZeroArray(VertexBuffers);
		ZeroArray(Descriptors);
	}

	void BindIndexBuffer(const VkBuffer buffer, VkIndexType indexType)
	{
		if(IndexBuffer != buffer)
		{
			vkCmdBindIndexBuffer(EVulkan::CurrentCommandBuffer, buffer, 0, indexType);
			IndexBuffer = buffer;
		}
	}

	void BindVertexBuffers(const mu_uint32 index, const mu_uint32 count, const VkBuffer *buffers)
	{
		mu_uint32 offset = 0;
		for(;offset < count;++offset)
		{
			if(VertexBuffers[index + offset] != buffers[offset])
			{
				break;
			}
		}

		if(offset < count)
		{
			static const VkDeviceSize Offsets[16] = {};
			vkCmdBindVertexBuffers(EVulkan::CurrentCommandBuffer, index + offset, count - offset, &buffers[offset], Offsets);
			mu_memcpy(&VertexBuffers[index + offset], &buffers[offset], (count - offset) * sizeof(VkBuffer));
		}
	}

	void BindDescriptorSets(const VkPipelineBindPoint pipelineBindpoint, const VkPipelineLayout layout, const mu_uint32 index, const mu_uint32 count, const VkDescriptorSet *descriptors)
	{
#if 1
		vkCmdBindDescriptorSets(EVulkan::CurrentCommandBuffer, pipelineBindpoint, layout, index, count, descriptors, 0, nullptr);
#else
		mu_uint32 offset = 0;
		for (; offset < count; ++offset)
		{
			if (Descriptors[pipelineBindpoint][index + offset] != descriptors[offset])
			{
				break;
			}
		}

		if (offset < count)
		{
			vkCmdBindDescriptorSets(EVulkan::CurrentCommandBuffer, pipelineBindpoint, layout, index + offset, count - offset, &descriptors[offset], 0, nullptr);
			mu_memcpy(&Descriptors[pipelineBindpoint][index + offset], &descriptors[offset], (count - offset) * sizeof(VkDescriptorSet));
		}
#endif
	}
};