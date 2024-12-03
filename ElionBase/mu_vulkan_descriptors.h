#ifndef __MU_VULKAN_DESCRIPTORS_H__
#define __MU_VULKAN_DESCRIPTORS_H__

#pragma once

class EUniformBufferBackend;

namespace EVulkanDescriptor
{
	const mu_boolean Create();
	VkDescriptorSet CreateBonesDescriptorSet();
	VkDescriptorSet CreateShadowMapDescriptorSet();
	VkDescriptorSet CreateTextureDescriptorSet();
	void FreeTextureDescriptorSet(VkDescriptorSet descriptorSet);
	void UpdateTextureDescriptorSet(ETextureBackend *TextureBackend);
	const mu_boolean AllocateAllSamplerDescriptorSet();
	const mu_boolean AllocateAllGlobalDescriptorSet();
	VkDescriptorSet CreateFontDescriptorSet(EBufferBackend *UniformBuffer);
	void Destroy();

	VkDescriptorSet GetSamplerDescriptorSet(const ESampler index);
	VkDescriptorSet GetGlobalDescriptorSet(const EGlobalConstant index);

	void UpdateTerrainTextureDescriptorSet(const MUTexture *TilesTexture);
	VkDescriptorSet GetTerrainTextureDescriptorSet(const mu_uint32 WaterIndex);

	void UpdateTerrainDataDescriptorSet(const MUTexture *AlphaTexture, const MUTexture *HeightTexture, const MUTexture *AttributeTexture);
	VkDescriptorSet GetTerrainDataDescriptorSet();

	void UpdateGrassTextureDescriptorSet(const MUTexture *GrassTexture);
	VkDescriptorSet GetGrassTextureDescriptorSet();

	VkDescriptorSet GetEffectSamplersDescriptorSet();
};

#endif