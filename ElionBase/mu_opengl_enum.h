#ifndef __MU_OPENGL_ENUM_H__
#define __MU_OPENGL_ENUM_H__

#pragma once

#define OPENGL_FRAGMENT_TEXTUREBASE (0)
#define OPENGL_FRAGMENT_TEXTURE (GL_TEXTURE0 + OPENGL_FRAGMENT_TEXTUREBASE)

#define OPENGL_VERTEX_TEXTUREBASE (8)
#define OPENGL_VERTEX_TEXTURE (GL_TEXTURE0 + OPENGL_VERTEX_TEXTUREBASE)

enum class OpenGLExtensions : mu_uint32
{
	Debug,
	BufferStorageEXT,
	BufferStorageARB,
	TextureStorageEXT,
	TextureStorageARB,
	BaseInstanceEXT,
	BaseInstanceARB,
	TextureBufferObjectEXT,
	TextureBufferObjectARB,
	TextureBufferEXT,
	TextureBufferARB,
	AnisotropyFilterEXT,
	AnisotropyFilterARB,

	Max,
};

enum class OpenGLFeatures : mu_uint32
{
	Debug,
	BufferStorage,
	TextureStorage,
	BaseInstance,
	TextureBuffer,
	BonesTextureBuffer,
	AnisotropyFilter,

	Max,
};

constexpr mu_uint32 OpenGLExtensionsMax = static_cast<mu_uint32>(OpenGLExtensions::Max);
constexpr mu_uint32 OpenGLFeaturesMax = static_cast<mu_uint32>(OpenGLFeatures::Max);

#endif