#ifndef __MU_DIRECTX_SHADERINCLUDE_H__
#define __MU_DIRECTX_SHADERINCLUDE_H__

#pragma once

class EShaderIncludeData
{
public:
	mu_uint8 *_Buffer;
	mu_uint32 _BufferSize;
};

class EShaderInclude : public ID3DInclude
{
public:
	EShaderInclude(const mu_string basePath) : _BasePath(basePath)
	{
	}

	~EShaderInclude()
	{
		for (auto it = _Includes.begin(); it != _Includes.end(); ++it)
		{
			MU_SAFE_DELETE_ARRAY(it->second._Buffer);
		}

		_Includes.clear();
	}

	HRESULT __stdcall Open(
		D3D_INCLUDE_TYPE IncludeType,
		LPCSTR pFileName,
		LPCVOID pParentData,
		LPCVOID *ppData,
		UINT *pBytes)
	{
		mu_size filenameSize = mu_asciilen(pFileName);
		mu_uint32 neededSize = 0;
		if (ConvertFromUTF8(pFileName, filenameSize, nullptr, 0, &neededSize) == false)
		{
			return E_FAIL;
		}

		if (neededSize == 0)
		{
			return E_FAIL;
		}

		mu_string filename(neededSize, '\0');
		mu_uint32 convertedSize = 0;
		if (ConvertFromUTF8(pFileName, filenameSize, (mu_text*)filename.data(), neededSize + 1, &convertedSize) == false)
		{
			return E_FAIL;
		}

		std::transform(filename.begin(), filename.end(), filename.begin(), mu_tolower);

		auto it = _Includes.find(filename);

		if (it != _Includes.end())
		{
			if (ppData != nullptr)
			{
				*ppData = it->second._Buffer;
			}
			if (pBytes != nullptr)
			{
				*pBytes = it->second._BufferSize;
			}

			return S_OK;
		}

		EShaderIncludeData includeData = { 0 };
		mu_int64 bufferSize = 0;

		mu_string path = _BasePath;
		path += filename;
		if (LoadEncryptedShader(path.c_str(), &includeData._Buffer, bufferSize) == false)
		{
			return E_FAIL;
		}

		includeData._BufferSize = (mu_uint32)bufferSize;

		_Includes.insert(std::make_pair(filename, includeData));

		if (ppData != nullptr)
		{
			*ppData = includeData._Buffer;
		}
		if (pBytes != nullptr)
		{
			*pBytes = includeData._BufferSize;
		}

		return S_OK;
	}

	HRESULT __stdcall Close(LPCVOID pData)
	{
		return S_OK;
	}

public:
	mu_string _BasePath;
	std::map<mu_string, EShaderIncludeData, map_icmp> _Includes;
};

#endif