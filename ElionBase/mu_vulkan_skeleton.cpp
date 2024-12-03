#include "stdafx.h"

namespace EVulkanSkeleton
{
	MUTexture *SkeletonTexture;
	std::vector<EStagingBufferBackend> SkeletonStagingBuffer;
	std::vector<void*> SkeletonMappedMemory;

	mu_boolean CreateSkeleton()
	{
		SkeletonTexture = new MUTexture[EVulkan::SwapchainImagesCount];
		SkeletonStagingBuffer.resize(EVulkan::SwapchainImagesCount);
		SkeletonMappedMemory.resize(EVulkan::SwapchainImagesCount);

		for (mu_uint32 n = 0; n < EVulkan::SwapchainImagesCount; ++n)
		{
			if (EVulkanTexture::CreateBones(&SkeletonTexture[n]) == false)
			{
				return false;
			}

			if (EVulkan::SkeletonTexelBufferEnabled == false)
			{
				if (SkeletonStagingBuffer[n].Allocate(SKELETON_TEXTURE_WIDTH * SKELETON_TEXTURE_HEIGHT * SKELETON_TEXTURE_TEXELSIZE) == false)
				{
					return false;
				}

				SkeletonMappedMemory[n] = SkeletonStagingBuffer[n].BufferInternal.MappedMemory;
			}
			else
			{
				SkeletonMappedMemory[n] = SkeletonTexture[n].VulkanBufferInternal.MappedMemory;
			}
		}

		return true;
	}

	void DestroySkeleton()
	{
		for (auto it = SkeletonStagingBuffer.begin(); it != SkeletonStagingBuffer.end(); ++it)
		{
			(*it).Deallocate();
		}

		MU_SAFE_DELETE_ARRAY(SkeletonTexture);
		SkeletonStagingBuffer.clear();
		SkeletonMappedMemory.clear();
	}

	VkDescriptorSet GetBonesDescriptorSet()
	{
		return EVulkan::SkeletonTexelBufferEnabled == true ?
			SkeletonTexture[EVulkan::CurrentCommandBufferIndex].VulkanBufferInternal.DescriptorSet :
			SkeletonTexture[EVulkan::CurrentCommandBufferIndex].VulkanInternal.DescriptorSet;
	}

#ifdef USE_DUAL_QUATERNION
	EDualQuat *GetBonesBuffer()
	{
		return reinterpret_cast<EDualQuat*>(SkeletonMappedMemory[EVulkan::CurrentCommandBufferIndex]);
	}
#else
	EMatrix4 *GetBonesBuffer()
	{
		return reinterpret_cast<EMatrix4*>(SkeletonMappedMemory[EVulkan::CurrentCommandBufferIndex]);
	}
#endif

	void FlushBuffer()
	{
		const mu_uint32 index = EVulkan::CurrentCommandBufferIndex;
		if (EVulkan::SkeletonTexelBufferEnabled == false)
		{
			const mu_uint32 skeletonCount = g_Tasking._SkeletonCount.load(std::memory_order_relaxed);
			SkeletonStagingBuffer[index].Flush(0, sizeof(EDualQuat) * skeletonCount);
#ifdef USE_DUAL_QUATERNION
			const mu_uint32 skeletonWidth = skeletonCount * 2 >= SKELETON_TEXTURE_WIDTH ? SKELETON_TEXTURE_WIDTH : skeletonCount * 2;
			const mu_uint32 skeletonHeight = (skeletonCount * 2 + (SKELETON_TEXTURE_WIDTH - 1)) / SKELETON_TEXTURE_WIDTH;
#else
			const mu_uint32 skeletonWidth = skeletonCount * 4 >= SKELETON_TEXTURE_WIDTH ? SKELETON_TEXTURE_WIDTH : skeletonCount * 4;
			const mu_uint32 skeletonHeight = (skeletonCount * 4 + (SKELETON_TEXTURE_WIDTH - 1)) / SKELETON_TEXTURE_WIDTH;
#endif
			SkeletonTexture[index].FlushAsTexture2D(&SkeletonStagingBuffer[index], 0, 0, 0, skeletonWidth, skeletonHeight, SKELETON_TEXTURE_FORMAT, SKELETON_TEXTURE_WIDTH * SKELETON_TEXTURE_TEXELSIZE);
		}
		else
		{
			SkeletonTexture[index].Flush(nullptr, 0, 0, sizeof(EDualQuat) * g_Tasking._SkeletonCount.load(std::memory_order_relaxed));
		}
	}
};