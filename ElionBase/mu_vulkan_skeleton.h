#ifndef __MU_VULKAN_SKELETON_H__
#define __MU_VULKAN_SKELETON_H__

#pragma once

namespace EVulkanSkeleton
{
	mu_boolean CreateSkeleton();
	void DestroySkeleton();
	VkDescriptorSet GetBonesDescriptorSet();
#ifdef USE_DUAL_QUATERNION
	EDualQuat *GetBonesBuffer();
#else
	EMatrix4 *GetBonesBuffer();
#endif

	void FlushBuffer();
};

#endif
