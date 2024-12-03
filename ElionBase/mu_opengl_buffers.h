#ifndef __MU_OPENGL_BUFFERS_H__
#define __MU_OPENGL_BUFFERS_H__

#pragma once

namespace EOpenGLBuffer
{
	const mu_boolean Create();
	void Destroy(const mu_boolean gpuOnly);

	const mu_boolean CreateShadowMap();
	void DestroyShadowMap();

	const mu_boolean CreateMultisample();
	void DestroyMultisample();

	void UpdateGlobalOnly();
	void Update(const mu_boolean IsLightUpload);

	void BindVertexBuffer(const EInputLayout inputLayout);
	void BindInstanceBuffer(const EInputLayout inputLayout);
	void BindGlobalConstant(const EGlobalConstant globalConstant);

	MUTexture *GetShadowMapTexture(const mu_uint32 index);
	MUTexture *GetMultisampleTexture(const mu_uint32 index);
	
	const MUTexture *GetTerrainLightTexture(const mu_uint32 index);

	SHGlobalConstantBuffer *GetGlobalConstantBuffer(const EGlobalConstant globalConstant);
	SHModelInstance *GetModelInstancesBuffer();
	SHEffectInstance *GetEffectInstancesBuffer();
	SHClothVertexData *GetClothVerticesBuffer();

	const MUTexture *GetCurrentBonesTexture();
#ifdef USE_DUAL_QUATERNION
	EDualQuat *GetBonesBuffer();
#else
	EMatrix4 *GetBonesBuffer();
#endif

	void UIUpdateBuffers();
	const mu_uint32 UIAllocateDataIndex(const mu_uint32 count);
	SHUIDefaultVertex *UIRetrieveDataBuffer(const mu_uint32 index);
	void UIRequestUpdateData(const mu_uint32 index, const mu_uint32 count);
	GLuint UIRetrieveCurrentBuffer();
	GLuint UIRetrieveVAO();

	void FontUpdateBuffers();
	void FontDefragmentBuffer();
	const mu_uint32 FontAllocateDataIndex(const mu_uint32 count);
	void FontDeallocateDataIndex(const mu_uint32 index);
	SHFontInstance *FontRetrieveDataBuffer(const mu_uint32 index);
	const mu_uint32 FontRetrieveBufferBaseIndex(const mu_uint32 index);
	void FontRequestUpdateData(const mu_uint32 index, const mu_uint32 count);
	GLuint FontRetrieveCurrentBuffer();
	GLuint FontRetrieveVAO();

	void UpdateTerrainVAO();
	void UpdateGrassVAO();
	GLuint RetrieveTerrainVAO();
	GLuint RetrieveGrassVAO();

	GLuint RetrieveClothVAO();
	GLuint RetrieveEffectVAO();
	GLuint WaterRetrieveVAO();
};

#endif