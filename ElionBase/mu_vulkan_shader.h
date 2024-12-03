#ifndef __MU_VULKAN_SHADER_H__
#define __MU_VULKAN_SHADER_H__

#pragma once

namespace EVulkanShader
{
	extern std::array<VkShaderModule, EVertexShaderMax> VertexShaderModule;
	extern std::array<VkShaderModule, EFragmentShaderMax> FragmentShaderModule;
#if GEOMETRY_SHADER_ENABLE == 1
	extern std::array<VkShaderModule, EGeometryShaderMax> GeometryShaderModule;
#endif

	void ClearCache();
	const mu_boolean Create(const mu_boolean isEarly);
	void Destroy();
	void DestroyNonEarly();
};

#endif