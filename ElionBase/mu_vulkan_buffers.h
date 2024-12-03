#ifndef __MU_VULKAN_BUFFERS_H__
#define __MU_VULKAN_BUFFERS_H__

#pragma once

namespace EVulkanBuffer
{
	extern VkDeviceSize GlobalConstantStagingSize;
	extern VkDeviceSize GlobalConstantUniformSize;
	extern VkDeviceSize GlobalConstantCopySize;
	extern std::vector<EBufferBackend> GlobalConstantBuffer;
	extern std::vector<EBufferBackend> ModelInstanceBuffer;
	extern std::vector<EBufferBackend> EffectInstanceBuffer;
	extern std::vector<EBufferBackend> ClothVertexBuffer;

	extern std::vector<EBufferBackend> WaterVertexBuffer;
	extern EBufferBackend WaterIndexBuffer;

	extern EBufferBackend DummyVertexBuffer;
	extern EBufferBackend DummyIndexBuffer;

	const mu_boolean Create();
	void Destroy(const mu_boolean gpuOnly);

	const mu_boolean CreateShadowMap();
	void DestroyShadowMap();

	void UpdateGlobalOnly();
	void Update(const mu_boolean IsLightUpload);

	MUTexture *GetShadowMapTexture(const mu_uint32 index);
	VkDescriptorSet GetShadowMapDescriptorSet(const mu_uint32 index);

	const MUTexture *GetTerrainLightTexture(const mu_uint32 index);
	SHGlobalConstantBuffer *GetGlobalConstantBuffer(const EGlobalConstant globalConstant);

	void UIUpdateBuffers();
	const mu_uint32 UIAllocateDataIndex(const mu_uint32 count);
	SHUIDefaultVertex *UIRetrieveDataBuffer(const mu_uint32 index);
	void UIRequestUpdateData(const mu_uint32 index, const mu_uint32 count);
	void UIEmptyTrash();
	VkBuffer UIRetrieveCurrentBuffer();

	void FontUpdateBuffers();
	const mu_uint32 FontAllocateDataIndex(const mu_uint32 count);
	void FontDeallocateDataIndex(const mu_uint32 index);
	SHFontInstance *FontRetrieveDataBuffer(const mu_uint32 index);
	const mu_uint32 FontRetrieveBufferBaseIndex(const mu_uint32 index);
	void FontRequestUpdateData(const mu_uint32 index, const mu_uint32 count);
	void FontEmptyTrash();
	void FontDefragmentBuffer();
	VkBuffer FontRetrieveCurrentBuffer();

	SHModelInstance *GetModelInstancesBuffer();
	SHEffectInstance *GetEffectInstancesBuffer();
	SHClothVertexData *GetClothVerticesBuffer();
};

#endif