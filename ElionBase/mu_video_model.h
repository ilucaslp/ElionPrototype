#ifndef __MU_VIDEO_MODEL_H__
#define __MU_VIDEO_MODEL_H__

#pragma once

class EModelBackend
{
public:
	EModelBackend()
	{
		mu_zeromem(this, sizeof(EModelBackend));
	}
	~EModelBackend()
	{
	}

protected:
	EINLINE void ReleaseModel()
	{
		ModelInternal.IndexBuffer.Deallocate();
		ModelInternal.VertexBuffer.Deallocate();

		switch(EVideoBackend::GetBackend())
		{
#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLHelpers::DeleteVAOs(1, &OpenGLInternal.VAO);
			}
			break;
#endif

		default: break;
		}
	}

public:
	struct 
	{
		EBufferBackend IndexBuffer;
		EBufferBackend VertexBuffer;
	} ModelInternal;
	union
	{
#if DIRECTX_BACKEND_ENABLE == 1
		struct
		{
		} DirectXInternal;
#endif
#if OPENGL_BACKEND_ENABLE == 1
		struct
		{
			GLuint VAO;
		} OpenGLInternal;
#endif
#if VULKAN_BACKEND_ENABLE == 1
		struct
		{
		} VulkanInternal;
#endif
	};
};

#endif