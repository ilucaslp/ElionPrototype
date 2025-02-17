#ifndef __MU_OPENGL_EXTENSIONS_H__
#define __MU_OPENGL_EXTENSIONS_H__

#pragma once

#if OPENGL_BACKEND_ANDROID == 1
#define GL_MAP_PERSISTENT_BIT GL_MAP_PERSISTENT_BIT_EXT
#define GL_MAP_COHERENT_BIT GL_MAP_COHERENT_BIT_EXT
#define GL_DYNAMIC_STORAGE_BIT GL_DYNAMIC_STORAGE_BIT_EXT
#define GL_CLIENT_STORAGE_BIT GL_CLIENT_STORAGE_BIT_EXT
#define GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT_EXT
#define GL_BUFFER_IMMUTABLE_STORAGE GL_BUFFER_IMMUTABLE_STORAGE_EXT
#define GL_BUFFER_STORAGE_FLAGS GL_BUFFER_STORAGE_FLAGS_EXT

#define GL_TEXTURE_BUFFER GL_TEXTURE_BUFFER_EXT
#define GL_TEXTURE_BUFFER_BINDING GL_TEXTURE_BUFFER_BINDING_EXT
#define GL_MAX_TEXTURE_BUFFER_SIZE GL_MAX_TEXTURE_BUFFER_SIZE_EXT
#define GL_TEXTURE_BINDING_BUFFER GL_TEXTURE_BINDING_BUFFER_EXT
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING GL_TEXTURE_BUFFER_DATA_STORE_BINDING_EXT
#define GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT_EXT
#define GL_SAMPLER_BUFFER GL_SAMPLER_BUFFER_EXT
#define GL_INT_SAMPLER_BUFFER GL_INT_SAMPLER_BUFFER_EXT
#define GL_UNSIGNED_INT_SAMPLER_BUFFER GL_UNSIGNED_INT_SAMPLER_BUFFER_EXT
#define GL_IMAGE_BUFFER GL_IMAGE_BUFFER_EXT
#define GL_INT_IMAGE_BUFFER GL_INT_IMAGE_BUFFER_EXT
#define GL_UNSIGNED_INT_IMAGE_BUFFER GL_UNSIGNED_INT_IMAGE_BUFFER_EXT
#define GL_TEXTURE_BUFFER_OFFSET GL_TEXTURE_BUFFER_OFFSET_EXT
#define GL_TEXTURE_BUFFER_SIZE GL_TEXTURE_BUFFER_SIZE_EXT

#define GL_MAX_TEXTURE_MAX_ANISOTROPY GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY GL_TEXTURE_MAX_ANISOTROPY_EXT

#define GL_CLAMP_TO_BORDER GL_CLAMP_TO_BORDER_EXT

// GL_EXT_buffer_storage
extern PFNGLBUFFERSTORAGEEXTPROC glBufferStorage;

// GL_EXT_base_instance
extern PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEEXTPROC glDrawArraysInstancedBaseInstance;
extern PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEEXTPROC glDrawElementsInstancedBaseInstance;
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEEXTPROC glDrawElementsInstancedBaseVertexBaseInstance;

// GL_EXT_texture_buffer_object
extern PFNGLTEXBUFFEREXTPROC glTexBuffer;
extern PFNGLTEXBUFFERRANGEEXTPROC glTexBufferRange;

// GL_KHR_debug
extern PFNGLDEBUGMESSAGECALLBACKKHRPROC glDebugMessageCallback;
extern PFNGLDEBUGMESSAGECONTROLKHRPROC glDebugMessageControl;
#elif OPENGL_BACKEND_MALIGLES == 1
// GL_EXT_buffer_storage
#define glBufferStorage

// GL_EXT_base_instance
#define glDrawArraysInstancedBaseInstance
#define glDrawElementsInstancedBaseInstance
#define glDrawElementsInstancedBaseVertexBaseInstance
#endif

namespace EOpenGLExtensions
{
	void Initialize();

	const mu_boolean IsExtensionSupported(const OpenGLExtensions extension);
	const mu_boolean IsFeatureSupported(const OpenGLFeatures feature);

	const mu_float GetMaxAnisotropy();
};

#endif