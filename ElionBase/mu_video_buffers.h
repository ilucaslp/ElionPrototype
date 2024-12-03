#ifndef __MU_VIDEO_BUFFERS_H__
#define __MU_VIDEO_BUFFERS_H__

#pragma once

namespace EVideoBuffer
{
	EINLINE SHGlobalConstantBuffer *GetGlobalConstantBuffer(const EGlobalConstant globalConstant)
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		switch(backendType)
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			return EDirectXBuffer::GetGlobalConstantBuffer(globalConstant);
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			return EOpenGLBuffer::GetGlobalConstantBuffer(globalConstant);
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			return EVulkanBuffer::GetGlobalConstantBuffer(globalConstant);
#endif

#if METAL_BACKEND_ENABLE == 1
		case EVideoBackendType::eMetal:
			break;
#endif

		default: break;
		}

		return nullptr;
	}

	EINLINE void UpdateGlobalOnly()
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		switch (backendType)
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			EDirectXBuffer::UpdateGlobalOnly();
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			EOpenGLBuffer::UpdateGlobalOnly();
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			EVideoBuffer::UpdateGlobalOnly();
			break;
#endif

#if METAL_BACKEND_ENABLE == 1
		case EVideoBackendType::eMetal:
			break;
#endif

		default: break;
		}
	}

#ifdef USE_DUAL_QUATERNION
	EINLINE EDualQuat *GetBonesBuffer()
#else
	EINLINE EMatrix4 *GetBonesBuffer()
#endif
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		switch (backendType)
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			return EDirectXBuffer::GetBonesBuffer();
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			return EOpenGLBuffer::GetBonesBuffer();
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			return EVulkanSkeleton::GetBonesBuffer();
#endif

#if METAL_BACKEND_ENABLE == 1
		case EVideoBackendType::eMetal:
			break;
#endif

		default: break;
		}

		return nullptr;
	}

	EINLINE SHModelInstance *GetModelInstancesBuffer()
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		switch (backendType)
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			return EDirectXBuffer::GetModelInstancesBuffer();
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			return EOpenGLBuffer::GetModelInstancesBuffer();
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			return EVulkanBuffer::GetModelInstancesBuffer();
#endif

#if METAL_BACKEND_ENABLE == 1
		case EVideoBackendType::eMetal:
			break;
#endif

		default: break;
		}

		return nullptr;
	}

	EINLINE SHEffectInstance *GetEffectInstancesBuffer()
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		switch (backendType)
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			return EDirectXBuffer::GetEffectInstancesBuffer();
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			return EOpenGLBuffer::GetEffectInstancesBuffer();
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			return EVulkanBuffer::GetEffectInstancesBuffer();
#endif

#if METAL_BACKEND_ENABLE == 1
		case EVideoBackendType::eMetal:
			break;
#endif

		default: break;
		}

		return nullptr;
	}

	EINLINE SHClothVertexData *GetClothVerticesBuffer()
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		switch (backendType)
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			return EDirectXBuffer::GetClothVerticesBuffer();
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			return EOpenGLBuffer::GetClothVerticesBuffer();
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			return EVulkanBuffer::GetClothVerticesBuffer();
#endif

#if METAL_BACKEND_ENABLE == 1
		case EVideoBackendType::eMetal:
			break;
#endif

		default: break;
		}

		return nullptr;
	}
};

#endif