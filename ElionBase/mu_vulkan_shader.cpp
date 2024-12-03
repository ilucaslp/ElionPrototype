#include "stdafx.h"

#define VULKAN_ENABLE_SHADER_CACHE (0)

#include <shaderc/shaderc.h>
#if defined(_MSC_VER)
#if 1
#pragma comment(lib, "shaderc_shared.lib")
#else
#ifndef NDEBUG
#pragma comment(lib, "shaderc_sharedd.lib")
#else
#pragma comment(lib, "shaderc_shared.lib")
#endif
#endif
#endif

namespace EVulkanShader
{
	const mu_string RootPath = _T("./data/shaders/vulkan/");
	const mu_string IncludePath = RootPath + _T("common/");

	struct EShaderCacheData
	{
		mu_uint32 Size;
		mu_uint8 *Data;
	};

	std::array<VkShaderModule, EVertexShaderMax> VertexShaderModule = { { VK_NULL_HANDLE } };
	std::array<VkShaderModule, EFragmentShaderMax> FragmentShaderModule = { { VK_NULL_HANDLE } };
#if GEOMETRY_SHADER_ENABLE == 1
	std::array<VkShaderModule, EGeometryShaderMax> GeometryShaderModule = { { VK_NULL_HANDLE } };
#endif

	std::map<mu_string, shaderc_include_result*> IncludesMap;
	std::map<mu_string, EShaderCacheData> CacheMap;

	shaderc_include_result *IncludeResolver(void *user_data,
		const mu_char *requested_source,
		mu_int32 type,
		const mu_char *requesting_source,
		mu_size include_depth)
	{
		shaderc_include_result *result = new shaderc_include_result();
		mu_zeromem(result, sizeof(shaderc_include_result));

		mu_size filenameSize = mu_asciilen(requested_source);
		mu_uint32 neededSize = 0;

		if (ConvertFromUTF8(requested_source, filenameSize, nullptr, 0, &neededSize) == false)
		{
			return result;
		}

		++neededSize;
		std::unique_ptr<mu_text[]> filename(new (std::nothrow) mu_text[neededSize]);
		mu_zeromem(filename.get(), sizeof(mu_text) * neededSize);
		mu_uint32 convertedSize = 0;

		if (ConvertFromUTF8(requested_source, filenameSize, filename.get(), neededSize, &convertedSize) == false)
		{
			return result;
		}

		std::transform(filename.get(), filename.get() + convertedSize, filename.get(), mu_tolower);

		auto it = IncludesMap.find(filename.get());
		if (it != IncludesMap.end())
		{
			delete result;
			return it->second;
		}

		mu_string relativePath(IncludePath);
		relativePath += filename.get();
		mu_uint8 *bufferData = nullptr;
		mu_int64 bufferSize = 0;
		if (LoadEncryptedShader(relativePath.c_str(), &bufferData, bufferSize) == false)
		{
			return result;
		}

		mu_uint32 mneededSize = 0;
		if (ConvertToUTF8(relativePath.c_str(), relativePath.size(), nullptr, 0, &mneededSize) == false)
		{
			return result;
		}

		++mneededSize;
		mu_char *mrelativePath = new (std::nothrow) mu_char[mneededSize];
		mu_zeromem(mrelativePath, sizeof(mu_char) * mneededSize);
		mu_uint32 mconvertedSize = 0;
		if (mrelativePath == nullptr)
		{
			return result;
		}

		if (ConvertToUTF8(relativePath.c_str(), relativePath.size(), mrelativePath, mneededSize, &mconvertedSize) == false)
		{
			delete[] mrelativePath;
			return result;
		}

		result->source_name = mrelativePath;
		result->source_name_length = mconvertedSize;
		result->content = (const char*)bufferData;
		result->content_length = bufferSize;

		IncludesMap.insert(std::make_pair(filename.get(), result));

		return result;
	}

	void IncludeReleaser(void *user_data, shaderc_include_result *include_result)
	{
		// Do Nothing
	}

	void ReleaseIncludes()
	{
		for (auto it : IncludesMap)
		{
			shaderc_include_result *result = it.second;

			if (result->source_name != nullptr)
			{
				delete[] result->source_name;
			}

			if (result->content != nullptr)
			{
				delete[] result->content;
			}

			delete result;
		}

		IncludesMap.clear();
	}

	constexpr shaderc_optimization_level OptimizationLevel = shaderc_optimization_level_performance;
	constexpr mu_text *ShaderCacheDirPath = _T("./cache/");
	constexpr mu_text *ShaderCacheFormat = _T("./cache/vulkan_shader_%1%_%2%_%3%_%4%_%5%_%6%");

	mu_boolean LoadShader(const mu_text *path, const shaderc_compiler_t compiler, const shaderc_compile_options_t options, const shaderc_shader_kind shaderkind, VkShaderModule *shader)
	{
		mu_uint8 *shaderBuffer = nullptr;
		mu_int64 shaderLength = 0;

		if (LoadEncryptedShader(path, &shaderBuffer, shaderLength) == false)
		{
			return false;
		}

		auto cacheIter = CacheMap.find(path);
		if (cacheIter != CacheMap.end())
		{
			EShaderCacheData &cacheData = cacheIter->second;

			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = cacheData.Size;
			createInfo.pCode = reinterpret_cast<const uint32_t*>(cacheData.Data);

			VkResult resultStatus = vkCreateShaderModule(EVulkan::Device, &createInfo, nullptr, shader);
			if (resultStatus == VK_SUCCESS)
			{
				return true;
			}

			CacheMap.erase(cacheIter);
		}

		std::stringstream definesStream;
		
		definesStream << "#version 460" << std::endl;
		definesStream << "#extension GL_ARB_separate_shader_objects : enable" << std::endl;
		definesStream << "#extension GL_ARB_shading_language_420pack : enable" << std::endl;

		switch (shaderkind)
		{
		case shaderc_glsl_vertex_shader:
			{
				definesStream << "#define SHADER_TYPE 0" << std::endl;
				definesStream << "precision highp float;" << std::endl;
				definesStream << "precision highp int;" << std::endl;
				definesStream << "precision highp sampler2D;" << std::endl;
				definesStream << "precision highp usampler2D;" << std::endl;
				definesStream << "precision highp samplerBuffer;" << std::endl;
			}
			break;

		case shaderc_glsl_fragment_shader:
			{
				definesStream << "#define SHADER_TYPE 1" << std::endl;
				definesStream << "#ifdef GL_ES" << std::endl;
				definesStream << "#ifdef GL_FRAGMENT_PRECISION_HIGH" << std::endl;
				definesStream << "precision highp float;" << std::endl;
				definesStream << "#else" << std::endl;
				definesStream << "precision mediump float;" << std::endl;
				definesStream << "#endif" << std::endl;
				definesStream << "#else" << std::endl;
				definesStream << "precision mediump float;" << std::endl;
				definesStream << "#endif" << std::endl;
				definesStream << "precision mediump int;" << std::endl;
				definesStream << "precision mediump sampler2D;" << std::endl;
				definesStream << "precision mediump sampler2DArray;" << std::endl;
				definesStream << "precision mediump sampler2DShadow;" << std::endl;
			}
			break;
			
		default: break;
		}

		definesStream << "#define SHADOW_TYPE (";
		definesStream << static_cast<mu_uint32>(MU_RESOURCE->GetShadowType());
		definesStream << ")" << std::endl;

#ifdef USE_DUAL_QUATERNION
		definesStream << "#define USE_DUAL_QUATERNION (1)" << std::endl;
#endif
#if VULKAN_REVERSED_DEPTH == 1
		definesStream << "#define USE_INVERSEMAPPING (1)" << std::endl;
#endif
		definesStream << "const float SHADOWMAP_RESOLUTION = (" << SHADOWMAP_RESOLUTION << ".0);" << std::endl;
		definesStream << "#define SHADOWMAP_TRANSPARENCY (" << SHADOWMAP_TRANSPARENCY << ")" << std::endl;
		definesStream << "const uint SKELETON_TEXTURE_WIDTH = (" << SKELETON_TEXTURE_WIDTH << "u);" << std::endl;
		definesStream << "const uint SKELETON_TEXTURE_HEIGHT = (" << SKELETON_TEXTURE_HEIGHT << "u);" << std::endl;
		if (EVulkan::SkeletonTexelBufferEnabled == true)
		{
			definesStream << "#define USE_TEXTURE_BUFFER (1)" << std::endl;
		}
		definesStream << "const uint SKELETON_TEXTURE_SIZE = (" << SKELETON_TEXTURE_WIDTH << "u * " << SKELETON_TEXTURE_HEIGHT << "u);" << std::endl;

		mu_boolean isSuccess = false;
		mu_asciistr defines = definesStream.str();

#if VULKAN_ENABLE_SHADER_CACHE == 1
		mu_uint32 shaderCrc32 = GenerateCRC32(shaderBuffer, shaderLength);
		mu_uint32 definesCrc32 = GenerateCRC32(defines.c_str(), defines.size() * sizeof(mu_char));

		mu_stringstream ShaderCRC32;
		ShaderCRC32 << std::hex << std::setfill(_T('0')) << std::setw(4) << std::nouppercase << shaderCrc32;

		mu_stringstream DefinesCRC32;
		DefinesCRC32 << std::hex << std::setfill(_T('0')) << std::setw(4) << std::nouppercase << definesCrc32;

		mu_stringstream PipelineCacheUUID;
		PipelineCacheUUID << std::hex << std::setfill(_T('0')) << std::setw(2) << std::nouppercase;
		for (mu_uint32 n = 0; n < VK_UUID_SIZE; ++n)
		{
			PipelineCacheUUID << static_cast<mu_int32>(EVulkan::DeviceProperties.pipelineCacheUUID[n]);
		}

		boost::basic_format<mu_text> ShaderFormatString = (boost::basic_format<mu_text>(ShaderCacheFormat) % ECastEnum(OptimizationLevel) % ShaderCRC32.str() % DefinesCRC32.str() % EVulkan::DeviceProperties.vendorID % EVulkan::DeviceProperties.deviceID % PipelineCacheUUID.str());
		mu_string ShaderCachePath = ShaderFormatString.str();

		if (mu_rwexists(ShaderCachePath.c_str()) == true)
		{
			SDL_RWops *file = nullptr;
			if (mu_rwfromfile(&file, ShaderCachePath.c_str(), "rb") == true)
			{
				mu_size bytecodeLength = SDL_RWsize(file);
				mu_uint8 *bytecode = new mu_uint8[bytecodeLength];

				if(bytecode != nullptr)
				{
					SDL_RWread(file, bytecode, bytecodeLength, 1);
					SDL_RWclose(file);

					VkShaderModuleCreateInfo createInfo = {};
					createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
					createInfo.codeSize = bytecodeLength;
					createInfo.pCode = reinterpret_cast<const uint32_t*>(bytecode);

					VkResult resultStatus = vkCreateShaderModule(EVulkan::Device, &createInfo, nullptr, shader);
					if (resultStatus != VK_SUCCESS)
					{
						mu_asciistr logError;
						logError.append("[Error][Vulkan][High] ");
						logError.append(EVulkanError::ResultToString(resultStatus));
						logError.append("\n");

#if defined(__ANDROID__)
						__android_log_print(ANDROID_LOG_ERROR, "Elion-Vulkan", "%s", logError.c_str());
#else
						OutputDebugStringA(logError.c_str());
#endif
					}
					else
					{
						isSuccess = true;

						// Insert to Temporal Cache
						EShaderCacheData cacheData = {};
						cacheData.Size = bytecodeLength;
						cacheData.Data = new mu_uint8[bytecodeLength];
						mu_memcpy(cacheData.Data, bytecode, bytecodeLength);
						CacheMap.insert(std::make_pair(path, cacheData));
					}

					MU_SAFE_DELETE_ARRAY(bytecode);
				}
			}
		}
#endif

		mu_asciistr shaderData;

		if(isSuccess == false)
		{
			shaderData.append(defines);
			shaderData.append((const mu_char*)shaderBuffer, (const size_t)shaderLength);
		}

		MU_SAFE_DELETE_ARRAY(shaderBuffer);

		if(isSuccess == false)
		{
			shaderc_compilation_result_t result = shaderc_compile_into_spv(compiler, shaderData.c_str(), shaderData.size(), shaderkind, "", "main", options);
			if (result == nullptr)
			{
				return false;
			}

			shaderc_compilation_status status = shaderc_result_get_compilation_status(result);
			isSuccess = status == shaderc_compilation_status_success;
			if (isSuccess == true)
			{
				const size_t bytecodeLength = shaderc_result_get_length(result);
				const mu_char *bytecode = shaderc_result_get_bytes(result);

#if VULKAN_ENABLE_SHADER_CACHE == 1
				MU::MakeDirectory(ShaderCacheDirPath);

				SDL_RWops *file = nullptr;
				if (mu_rwfromfile(&file, ShaderCachePath.c_str(), "wb") == true)
				{
					SDL_RWwrite(file, bytecode, bytecodeLength, 1);
					SDL_RWclose(file);
				}
#endif

				VkShaderModuleCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				createInfo.codeSize = bytecodeLength;
				createInfo.pCode = reinterpret_cast<const uint32_t*>(bytecode);

				VkResult resultStatus = vkCreateShaderModule(EVulkan::Device, &createInfo, nullptr, shader);
				if (resultStatus != VK_SUCCESS)
				{
					mu_asciistr logError;
					logError.append("[Error][Vulkan][High] ");
					logError.append(EVulkanError::ResultToString(resultStatus));
					logError.append("\n");

	#if defined(__ANDROID__)
					__android_log_print(ANDROID_LOG_ERROR, "Elion-Vulkan", "%s", logError.c_str());
	#else
					OutputDebugStringA(logError.c_str());
	#endif

					isSuccess = false;
				}
				else
				{
					// Insert to Temporal Cache
					EShaderCacheData cacheData = {};
					cacheData.Size = bytecodeLength;
					cacheData.Data = new mu_uint8[bytecodeLength];
					mu_memcpy(cacheData.Data, bytecode, bytecodeLength);
					CacheMap.insert(std::make_pair(path, cacheData));
				}
			}

			const mu_size numWarnings = shaderc_result_get_num_warnings(result);
			const mu_size numErrors = shaderc_result_get_num_errors(result);
			if (numWarnings > 0 || numErrors > 0)
			{
				mu_asciistr logError;
				if (numErrors == 0)
				{
					logError.append("[Error][Vulkan][High] ");
				}
				else
				{
					logError.append("[Warning][Vulkan][High] ");
				}
				logError.append(shaderc_result_get_error_message(result));
				logError.append("\n");

	#if defined(__ANDROID__)
				__android_log_print(ANDROID_LOG_ERROR, "Elion-Vulkan", "%s", logError.c_str());
	#else
				OutputDebugStringA(logError.c_str());
	#endif
			}

			shaderc_result_release(result);
		}

		return isSuccess;
	}

	void ClearCache()
	{
		CacheMap.clear();
	}

	const mu_boolean Create(const mu_boolean isEarly)
	{
		shaderc_compiler_t compiler = shaderc_compiler_initialize();
		shaderc_compile_options_t options = shaderc_compile_options_initialize();
		shaderc_compile_options_set_include_callbacks(options, IncludeResolver, IncludeReleaser, nullptr);
		shaderc_compile_options_set_optimization_level(options, OptimizationLevel);

		for (mu_uint32 index = 0; index < EVertexShaderMax; ++index)
		{
			if(VertexShaderModule[index] != VK_NULL_HANDLE) continue;

			EShaderData &vertexList = VertexShaderData[index];
			if (vertexList.Early != isEarly) continue;

			mu_string fullPath = RootPath + vertexList.Path;

#ifndef NDEBUG
#if defined(__ANDROID__)
			__android_log_print(ANDROID_LOG_INFO, "Elion-Vulkan", "Parsing Shader to SPIRV [%ls]\n", fullPath.c_str());
#else
			std::stringstream debugMessage;
			debugMessage << "Parsing Shader to SPIRV [" << fullPath.c_str() << "]" << std::endl;
			OutputDebugStringA(debugMessage.str().c_str());
#endif
#endif

			if (LoadShader(fullPath.c_str(), compiler, options, shaderc_glsl_vertex_shader, &VertexShaderModule[index]) == false)
			{
				shaderc_compile_options_release(options);
				shaderc_compiler_release(compiler);
				ReleaseIncludes();
				return false;
			}
		}

		for (mu_uint32 index = 0; index < EFragmentShaderMax; ++index)
		{
			if (FragmentShaderModule[index] != VK_NULL_HANDLE) continue;

			EShaderData &fragmentList = FragmentShaderData[index];
			if (fragmentList.Early != isEarly) continue;

			mu_string fullPath = RootPath + fragmentList.Path;

#ifndef NDEBUG
#if defined(__ANDROID__)
			__android_log_print(ANDROID_LOG_INFO, "Elion-Vulkan", "Parsing Shader to SPIRV [%ls]\n", fullPath.c_str());
#else
			std::stringstream debugMessage;
			debugMessage << "Parsing Shader to SPIRV [" << fullPath.c_str() << "]" << std::endl;
			OutputDebugStringA(debugMessage.str().c_str());
#endif
#endif

			if (LoadShader(fullPath.c_str(), compiler, options, shaderc_glsl_fragment_shader, &FragmentShaderModule[index]) == false)
			{
				shaderc_compile_options_release(options);
				shaderc_compiler_release(compiler);
				ReleaseIncludes();
				return false;
			}
		}

#if GEOMETRY_SHADER_ENABLE == 1
		for (mu_uint32 index = 0; index < EGeometryShaderMax; ++index)
		{
			if (GeometryShaderModule[index] != VK_NULL_HANDLE) continue;

			EShaderData &geometryList = GeometryShaderData[index];
			if (geometryList.Early != isEarly) continue;

			mu_string fullPath = RootPath + geometryList.Path;

#ifndef NDEBUG
#if defined(__ANDROID__)
			__android_log_print(ANDROID_LOG_INFO, "Elion-Vulkan", "Parsing Shader to SPIRV [%ls]\n", fullPath.c_str());
#else
			std::stringstream debugMessage;
			debugMessage << "Parsing Shader to SPIRV [" << fullPath << "]" << std::endl;
			OutputDebugStringA(debugMessage.str().c_str());
#endif
#endif

			if (LoadShader(fullPath.c_str(), compiler, options, shaderc_glsl_geometry_shader, &GeometryShaderModule[index]) == false)
			{
				shaderc_compile_options_release(options);
				shaderc_compiler_release(compiler);
				ReleaseIncludes();
				return false;
			}
		}
#endif

		shaderc_compile_options_release(options);
		shaderc_compiler_release(compiler);
		ReleaseIncludes();

		return true;
	}

	void Destroy()
	{
		for (mu_uint32 index = 0; index < mu_countof(VertexShaderModule); ++index)
		{
			if (VertexShaderModule[index] != VK_NULL_HANDLE)
			{
				vkDestroyShaderModule(EVulkan::Device, VertexShaderModule[index], nullptr);
				VertexShaderModule[index] = VK_NULL_HANDLE;
			}
		}

		for (mu_uint32 index = 0; index < mu_countof(FragmentShaderModule); ++index)
		{
			if (FragmentShaderModule[index] != VK_NULL_HANDLE)
			{
				vkDestroyShaderModule(EVulkan::Device, FragmentShaderModule[index], nullptr);
				FragmentShaderModule[index] = VK_NULL_HANDLE;
			}
		}

#if GEOMETRY_SHADER_ENABLE == 1
		for (mu_uint32 index = 0; index < mu_countof(GeometryShaderModule); ++index)
		{
			if (GeometryShaderModule[index] != VK_NULL_HANDLE)
			{
				vkDestroyShaderModule(EVulkan::Device, GeometryShaderModule[index], nullptr);
				GeometryShaderModule[index] = VK_NULL_HANDLE;
			}
		}
#endif
	}

	void DestroyNonEarly()
	{
		for (mu_uint32 index = 0; index < EVertexShaderMax; ++index)
		{
			EShaderData &vertexList = VertexShaderData[index];

			if (VertexShaderModule[index] == VK_NULL_HANDLE ||
				vertexList.Early == true) continue;

			if (VertexShaderModule[index] != VK_NULL_HANDLE)
			{
				vkDestroyShaderModule(EVulkan::Device, VertexShaderModule[index], nullptr);
				VertexShaderModule[index] = VK_NULL_HANDLE;
			}
		}

		for (mu_uint32 index = 0; index < EFragmentShaderMax; ++index)
		{
			EShaderData &fragmentList = FragmentShaderData[index];

			if (FragmentShaderModule[index] == VK_NULL_HANDLE ||
				fragmentList.Early == true) continue;

			if (FragmentShaderModule[index] != VK_NULL_HANDLE)
			{
				vkDestroyShaderModule(EVulkan::Device, FragmentShaderModule[index], nullptr);
				FragmentShaderModule[index] = VK_NULL_HANDLE;
			}
		}

#if GEOMETRY_SHADER_ENABLE == 1
		for (mu_uint32 index = 0; index < EGeometryShaderMax; ++index)
		{
			EShaderData &geometryList = GeometryShaderData[index];

			if (GeometryShaderModule[index] == VK_NULL_HANDLE ||
				geometryList.Early == true) continue;

			if (GeometryShaderModule[index] != VK_NULL_HANDLE)
			{
				vkDestroyShaderModule(EVulkan::Device, GeometryShaderModule[index], nullptr);
				GeometryShaderModule[index] = VK_NULL_HANDLE;
			}
		}
#endif
	}
};