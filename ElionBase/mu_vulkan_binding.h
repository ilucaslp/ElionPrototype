#ifndef __MU_VULKAN_BINDING_H__
#define __MU_VULKAN_BINDING_H__

#pragma once

namespace EVulkanBinding
{
	// Uniform Bindings
	extern VkDescriptorSetLayoutBinding GlobalConstantBinding[1];
	extern VkDescriptorSetLayoutBinding FontConstantBinding[1];
	extern VkDescriptorSetLayoutBinding TerrainBinding[4];
	extern VkDescriptorSetLayoutBinding ShadowBinding[1];
	extern VkDescriptorSetLayoutBinding TerrainTextureBinding[2];
	extern VkDescriptorSetLayoutBinding GrassTextureBinding[1];
	extern VkDescriptorSetLayoutBinding WaterTextureBinding[2];
	extern VkDescriptorSetLayoutBinding FontTextureBinding[1];
	extern VkDescriptorSetLayoutBinding TextureBinding[1];
	extern VkDescriptorSetLayoutBinding SamplerBinding[1];
	extern VkDescriptorSetLayoutBinding EffectSamplerBinding[1];
	extern VkDescriptorSetLayoutBinding BonesBinding[1];
};

#endif