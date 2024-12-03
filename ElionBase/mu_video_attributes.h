#ifndef __MU_VIDEO_ATTRIBUTES_H__
#define __MU_VIDEO_ATTRIBUTES_H__

#pragma once

namespace EVideoAttributes
{
	EINLINE void ConfigureAtLoad(const EInputLayout inputLayout)
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		if (backendType == EVideoBackendType::eOpenGL)
		{
			mu_uint32 attributesCount = 0;
			EOpenGLAttributes::EAttributeData *attributes = nullptr;
			mu_uint32 instancesCount = 0;
			EOpenGLAttributes::EAttributeData *instances = nullptr;

			EOpenGLAttributes::Retrieve(inputLayout, attributesCount, attributes, instancesCount, instances);

			EOpenGLBuffer::BindVertexBuffer(inputLayout);

			for(mu_uint32 n = 0;n < attributesCount;++n)
			{
				EOpenGLAttributes::EAttributeData &data = attributes[n];

				glEnableVertexAttribArray(data.Index);
				if(data.UseInteger == GL_TRUE)
				{
					glVertexAttribIPointer(data.Index, data.Size, data.Type, data.Stride, (const void*)data.Pointer);
				}
				else
				{
					glVertexAttribPointer(data.Index, data.Size, data.Type, data.Normalized, data.Stride, (const void*)data.Pointer);
				}
			}

			EOpenGLBuffer::BindInstanceBuffer(inputLayout);

			for (mu_uint32 n = 0; n < instancesCount; ++n)
			{
				EOpenGLAttributes::EAttributeData &data = instances[n];

				glEnableVertexAttribArray(data.Index);
				if (data.UseInteger == GL_TRUE)
				{
					glVertexAttribIPointer(data.Index, data.Size, data.Type, data.Stride, (const void*)data.Pointer);
				}
				else
				{
					glVertexAttribPointer(data.Index, data.Size, data.Type, data.Normalized, data.Stride, (const void*)data.Pointer);
				}
				glVertexAttribDivisor(data.Index, 1);
			}
		}
	}

	template<const EVideoBackendType backendType>
	void ConfigureAtRuntime(const EInputLayout inputLayout)
	{
		if constexpr(backendType == EVideoBackendType::eDirectX)
		{
		}
		else if constexpr(backendType == EVideoBackendType::eOpenGL)
		{
			mu_uint32 attributesCount = 0;
			EOpenGLAttributes::EAttributeData *attributes = nullptr;
			mu_uint32 instancesCount = 0;
			EOpenGLAttributes::EAttributeData *instances = nullptr;

			EOpenGLAttributes::Retrieve(inputLayout, attributesCount, attributes, instancesCount, instances);

			EOpenGLBuffer::BindInstanceBuffer(inputLayout);

			for (mu_uint32 n = 0; n < instancesCount; ++n)
			{
				EOpenGLAttributes::EAttributeData &data = instances[n];

				glEnableVertexAttribArray(data.Index);
				if (data.UseInteger == GL_TRUE)
				{
					glVertexAttribIPointer(data.Index, data.Size, data.Type, data.Stride, (const void*)data.Pointer);
				}
				else
				{
					glVertexAttribPointer(data.Index, data.Size, data.Type, data.Normalized, data.Stride, (const void*)data.Pointer);
				}
				glVertexAttribDivisor(data.Index, 1);
			}
		}
	}

	template<const EVideoBackendType backendType>
	void ConfigureInstanceAtRuntime(const EInputLayout inputLayout, const mu_uint32 instanceBase)
	{
		if constexpr(backendType == EVideoBackendType::eOpenGL)
		{
			mu_uint32 attributesCount = 0;
			EOpenGLAttributes::EAttributeData *attributes = nullptr;
			mu_uint32 instancesCount = 0;
			EOpenGLAttributes::EAttributeData *instances = nullptr;

			EOpenGLAttributes::Retrieve(inputLayout, attributesCount, attributes, instancesCount, instances);
			const mu_uint32 baseInstanceOffset = instanceBase * EOpenGLAttributes::RetrieveInstanceSize(inputLayout);

			EOpenGLBuffer::BindInstanceBuffer(inputLayout);

			for (mu_uint32 n = 0; n < instancesCount; ++n)
			{
				EOpenGLAttributes::EAttributeData &data = instances[n];

				glEnableVertexAttribArray(data.Index);
				if (data.UseInteger == GL_TRUE)
				{
					glVertexAttribIPointer(data.Index, data.Size, data.Type, data.Stride, (const void*)(data.Pointer + baseInstanceOffset));
				}
				else
				{
					glVertexAttribPointer(data.Index, data.Size, data.Type, data.Normalized, data.Stride, (const void*)(data.Pointer + baseInstanceOffset));
				}
				glVertexAttribDivisor(data.Index, 1);
			}
		}
	}
};

#endif