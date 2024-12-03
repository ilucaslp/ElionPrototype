#ifndef __MU_OPENGL_ATTRIBUTES_H__
#define __MU_OPENGL_ATTRIBUTES_H__

#pragma once

namespace EOpenGLAttributes
{
	struct EAttributeData
	{
		EAttributeData(const GLuint index,
			const GLint size,
			const GLenum type,
			const GLboolean useinteger,
			const GLboolean normalized,
			const GLsizei stride,
			const GLintptr pointer) :
			Index(index),
			Size(size),
			Type(type),
			UseInteger(useinteger),
			Normalized(normalized),
			Stride(stride),
			Pointer(pointer)
		{
		}

		const GLuint Index;
		const GLint Size;
		const GLenum Type;
		const GLboolean UseInteger;
		const GLboolean Normalized;
		const GLsizei Stride;
		const GLintptr Pointer;
	};

	void Retrieve(const EInputLayout layout,
		mu_uint32 &count,
		EAttributeData *&attributes,
		mu_uint32 &instanceCount,
		EAttributeData *&instanceAttributes);
	const mu_uint32 RetrieveInstanceSize(const EInputLayout layout);
};

#endif