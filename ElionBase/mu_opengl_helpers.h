#ifndef __MU_OPENGL_HELPERS_H__
#define __MU_OPENGL_HELPERS_H__

#pragma once

namespace EOpenGLHelpers
{
	EINLINE void GenerateBuffers(const mu_uint32 count, GLuint *buffers)
	{
		glGenBuffers(count, buffers);
	}

	EINLINE void DeleteBuffers(const mu_uint32 count, GLuint *buffers)
	{
		glDeleteBuffers(count, buffers);
		mu_zeromem(buffers, sizeof(GLuint) * count);
	}

	EINLINE void GenerateTextures(const mu_uint32 count, GLuint *buffers)
	{
		glGenTextures(count, buffers);
	}

	EINLINE void DeleteTextures(const mu_uint32 count, GLuint *buffers)
	{
		glDeleteTextures(count, buffers);
		mu_zeromem(buffers, sizeof(GLuint) * count);
	}

	EINLINE void GenerateRenderbuffers(const mu_uint32 count, GLuint *buffers)
	{
		glGenRenderbuffers(count, buffers);
	}

	EINLINE void DeleteRenderbuffers(const mu_uint32 count, GLuint *buffers)
	{
		glDeleteRenderbuffers(count, buffers);
		mu_zeromem(buffers, sizeof(GLuint) * count);
	}

	EINLINE void GenerateFramebuffers(const mu_uint32 count, GLuint *buffers)
	{
		glGenFramebuffers(count, buffers);
	}

	EINLINE void DeleteFramebuffers(const mu_uint32 count, GLuint *buffers)
	{
		glDeleteFramebuffers(count, buffers);
		mu_zeromem(buffers, sizeof(GLuint) * count);
	}

	EINLINE void GenerateVAOs(const mu_uint32 count, GLuint *buffers)
	{
		glGenVertexArrays(count, buffers);
	}

	EINLINE void DeleteVAOs(const mu_uint32 count, GLuint *buffers)
	{
		glDeleteVertexArrays(count, buffers);
		mu_zeromem(buffers, sizeof(GLuint) * count);
	}
};

#endif