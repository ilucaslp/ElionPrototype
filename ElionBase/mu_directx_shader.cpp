#include "stdafx.h"

#if DIRECTX_BACKEND_ENABLE == 1
#include "mu_directx_shaderinclude.h"

namespace EDirectXShader
{
	const mu_string RootPath = _T("./data/shaders/directx/");
	const mu_string CommonPath = _T("./data/shaders/directx/common/");
	EShaderInclude ShaderInclude(CommonPath);

	std::array<ID3D11VertexShader*, EVertexShaderMax> VertexShader = { { nullptr } };
	std::array<ID3D11PixelShader*, EFragmentShaderMax> FragmentShader = { { nullptr } };
	std::array<ID3D11GeometryShader*, EGeometryShaderMax> GeometryShader = { { nullptr } };
	std::array<ID3D11InputLayout*, EInputLayoutMax> InputLayouts = { { nullptr } };

	template<const EShaderType shaderType, typename T>
	const mu_boolean LoadShader(const mu_text *path, T **shader, const mu_uint32 layoutCount = 0, const D3D11_INPUT_ELEMENT_DESC *layoutElements = nullptr, ID3D11InputLayout **layout = nullptr)
	{
		mu_uint8 *shaderBuffer = nullptr;
		mu_int64 shaderLength = 0;

		if (LoadEncryptedShader(path, &shaderBuffer, shaderLength) == false)
		{
			return false;
		}

		const mu_char *shaderProfile = nullptr;
		if constexpr (shaderType == EShaderType::eVertex)
		{
			switch (EDirectXInit::FeatureLevel)
			{
			case D3D_FEATURE_LEVEL_10_0:
				{
					shaderProfile = "vs_4_0";
				}
				break;

			case D3D_FEATURE_LEVEL_10_1:
				{
					shaderProfile = "vs_4_1";
				}
				break;

			case D3D_FEATURE_LEVEL_11_0:
				{
					shaderProfile = "vs_5_0";
				}
				break;
			}
		}
		else if constexpr (shaderType == EShaderType::eFragment)
		{
			switch (EDirectXInit::FeatureLevel)
			{
			case D3D_FEATURE_LEVEL_10_0:
				{
					shaderProfile = "ps_4_0";
				}
				break;

			case D3D_FEATURE_LEVEL_10_1:
				{
					shaderProfile = "ps_4_1";
				}
				break;

			case D3D_FEATURE_LEVEL_11_0:
				{
					shaderProfile = "ps_5_0";
				}
				break;
			}
		}
		else if constexpr (shaderType == EShaderType::eGeometry)
		{
			switch (EDirectXInit::FeatureLevel)
			{
			case D3D_FEATURE_LEVEL_10_0:
				{
					shaderProfile = "gs_4_0";
				}
				break;

			case D3D_FEATURE_LEVEL_10_1:
				{
					shaderProfile = "gs_4_1";
				}
				break;

			case D3D_FEATURE_LEVEL_11_0:
				{
					shaderProfile = "gs_5_0";
				}
				break;
			}
		}

		std::vector<D3D_SHADER_MACRO> macros;

		mu_char directVersion[16];
		mu_sprintf8_s(directVersion, "(%d)", (mu_int32)EDirectXInit::FeatureLevel);
		macros.push_back(D3D_SHADER_MACRO{ "DIRECTX_VERSION", directVersion });

		mu_char shadowType[4];
		mu_sprintf8_s(shadowType, "%d", MU_RESOURCE->GetShadowType());
		macros.push_back(D3D_SHADER_MACRO{ "SHADOW_TYPE", shadowType });
#ifdef USE_DUAL_QUATERNION
		macros.push_back(D3D_SHADER_MACRO{ "USE_DUAL_QUATERNION", "1" });
#endif
#if DIRECTX_REVERSED_DEPTH == 1
		macros.push_back(D3D_SHADER_MACRO{ "USE_INVERSEMAPPING", "1" });
#endif
		macros.push_back(D3D_SHADER_MACRO{ "SHADOWMAP_RESOLUTION", "(" _GETTEXT(SHADOWMAP_RESOLUTION) ")" });
		macros.push_back(D3D_SHADER_MACRO{ "SHADOWMAP_TRANSPARENCY", "(" _GETTEXT(SHADOWMAP_TRANSPARENCY) ")" });

		macros.push_back(D3D_SHADER_MACRO{ "SKELETON_TEXTURE_WIDTH", "(" _GETTEXT(SKELETON_TEXTURE_WIDTH) ")" });
		macros.push_back(D3D_SHADER_MACRO{ "SKELETON_TEXTURE_HEIGHT", "(" _GETTEXT(SKELETON_TEXTURE_HEIGHT) ")" });

		macros.push_back(D3D_SHADER_MACRO{ nullptr, nullptr });

		mu_uint32 flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
		ID3DBlob *shaderCode = nullptr, *errorMsg = nullptr;
		HRESULT result = EDirectX::D3DCompile(shaderBuffer, shaderLength, nullptr, macros.data(), &ShaderInclude, "main", shaderProfile, flags, 0, &shaderCode, &errorMsg);

		if (SUCCEEDED(result))
		{
			if constexpr (shaderType == EShaderType::eVertex)
			{
				result = EDirectXInit::Device->CreateVertexShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), nullptr, shader);
				mu_assert(SUCCEEDED(result));
				if (SUCCEEDED(result))
				{
					if (layoutElements != nullptr &&
						layoutCount > 0 &&
						layout != nullptr &&
						*layout == nullptr)
					{
						result = EDirectXInit::Device->CreateInputLayout(layoutElements, layoutCount, shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), layout);
						mu_assert(SUCCEEDED(result));
					}
				}
			}
			else if constexpr (shaderType == EShaderType::eFragment)
			{
				result = EDirectXInit::Device->CreatePixelShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), nullptr, shader);
				mu_assert(SUCCEEDED(result));
			}
			else if constexpr (shaderType == EShaderType::eGeometry)
			{
				result = EDirectXInit::Device->CreateGeometryShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), nullptr, shader);
				mu_assert(SUCCEEDED(result));
			}
		}

		if (errorMsg != nullptr)
		{
#if !defined(NDEBUG) || BACKEND_FORCE_DEBUGREPORT == 1
			OutputDebugStringA((LPCSTR)errorMsg->GetBufferPointer());
#endif
		}

		MU_SAFE_DELETE_ARRAY(shaderBuffer);

		EDirectXHelpers::Release(shaderCode);
		EDirectXHelpers::Release(errorMsg);

		return SUCCEEDED(result);
	}

	const mu_boolean Create(const mu_boolean earlyShaders)
	{
		for (mu_uint32 n = 0; n < EVertexShaderMax; ++n)
		{
			EShaderData &data = VertexShaderData[n];

			if (data.Early != earlyShaders)
			{
				continue;
			}

			mu_uint32 layoutCount = 0;
			D3D11_INPUT_ELEMENT_DESC *layoutElements = nullptr;
			EDirectXAttributes::Retrieve(data.InputLayout, layoutCount, layoutElements);

			mu_string fullPath = RootPath + data.Path;
			if (LoadShader<EShaderType::eVertex>(fullPath.c_str(), &VertexShader[n], layoutCount, layoutElements, &InputLayouts[ECastEnum(data.InputLayout)]) == false)
			{
				return false;
			}
		}

		for (mu_uint32 n = 0; n < EFragmentShaderMax; ++n)
		{
			EShaderData &data = FragmentShaderData[n];

			if (data.Early != earlyShaders)
			{
				continue;
			}

			mu_string fullPath = RootPath + data.Path;
			if (LoadShader<EShaderType::eFragment>(fullPath.c_str(), &FragmentShader[n]) == false)
			{
				return false;
			}
		}

#if GEOMETRY_SHADER_ENABLE == 1
		for (mu_uint32 n = 0; n < EGeometryShaderMax; ++n)
		{
			EShaderData &data = GeometryShaderData[n];

			if (data.Early != earlyShaders)
			{
				continue;
			}

			mu_string fullPath = RootPath + data.Path;
			if (LoadShader<EShaderType::eGeometry>(fullPath.c_str(), &GeometryShader[n]) == false)
			{
				return false;
			}
		}
#endif

		return true;
	}

	void DestroyNonEarly()
	{
		for (mu_uint32 n = 0; n < EVertexShaderMax; ++n)
		{
			EShaderData &data = VertexShaderData[n];

			if (data.Early == true)
			{
				continue;
			}

			EDirectXHelpers::Release(VertexShader[n]);
		}

		for (mu_uint32 n = 0; n < EFragmentShaderMax; ++n)
		{
			EShaderData &data = FragmentShaderData[n];

			if (data.Early == true)
			{
				continue;
			}

			EDirectXHelpers::Release(FragmentShader[n]);
		}

#if GEOMETRY_SHADER_ENABLE == 1
		for (mu_uint32 n = 0; n < EGeometryShaderMax; ++n)
		{
			EShaderData &data = GeometryShaderData[n];

			if (data.Early == true)
			{
				continue;
			}

			EDirectXHelpers::Release(GeometryShader[n]);
		}
#endif
	}

	void Destroy()
	{
		for (mu_uint32 n = 0; n < EInputLayoutMax; ++n)
		{
			EDirectXHelpers::Release(InputLayouts[n]);
		}

		for (mu_uint32 n = 0; n < EVertexShaderMax; ++n)
		{
			EDirectXHelpers::Release(VertexShader[n]);
		}

		for (mu_uint32 n = 0; n < EFragmentShaderMax; ++n)
		{
			EDirectXHelpers::Release(FragmentShader[n]);
		}

#if GEOMETRY_SHADER_ENABLE == 1
		for (mu_uint32 n = 0; n < EGeometryShaderMax; ++n)
		{
			EDirectXHelpers::Release(GeometryShader[n]);
		}
#endif
	}

	ECombinedShader CurrentProgramShader = ECombinedShader::eInvalid;
	ETopologyType CurrentTopologyType = ETopologyType::eInvalid;
	EInputLayout CurrentInputLayout = EInputLayout::eInvalid;
	EVertexShader CurrentVertexShader = EVertexShader::eInvalid;
	EFragmentShader CurrentFragmentShader = EFragmentShader::eInvalid;
	EGeometryShader CurrentGeometryShader = EGeometryShader::eInvalid;

	FORCEINLINE void SetTopologyType(const ETopologyType type)
	{
		static D3D11_PRIMITIVE_TOPOLOGY topologyList[ETopologyTypeMax] =
		{
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		};

		if (CurrentTopologyType != type)
		{
			if (type == ETopologyType::eInvalid)
			{
				EDirectXInit::DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED);
			}
			else
			{
				EDirectXInit::DeviceContext->IASetPrimitiveTopology(topologyList[ECastEnum(type)]);
			}
			CurrentTopologyType = type;
		}
	}

	FORCEINLINE void SetInputLayout(const EInputLayout layout)
	{
		if (CurrentInputLayout != layout)
		{
			if (layout == EInputLayout::eInvalid)
			{
				EDirectXInit::DeviceContext->IASetInputLayout(nullptr);
			}
			else
			{
				EDirectXInit::DeviceContext->IASetInputLayout(InputLayouts[ECastEnum(layout)]);
			}
			CurrentInputLayout = layout;
		}
	}

	FORCEINLINE void SetVertexShader(const EVertexShader shader)
	{
		if (CurrentVertexShader != shader)
		{
			if (shader == EVertexShader::eInvalid)
			{
				EDirectXInit::DeviceContext->VSSetShader(nullptr, nullptr, 0);
			}
			else
			{
				EDirectXInit::DeviceContext->VSSetShader(VertexShader[ECastEnum(shader)], nullptr, 0);
			}
			CurrentVertexShader = shader;
		}
	}

	FORCEINLINE void SetFragmentShader(const EFragmentShader shader)
	{
		if (CurrentFragmentShader != shader)
		{
			if (shader == EFragmentShader::eInvalid)
			{
				EDirectXInit::DeviceContext->PSSetShader(nullptr, nullptr, 0);
			}
			else
			{
				EDirectXInit::DeviceContext->PSSetShader(FragmentShader[ECastEnum(shader)], nullptr, 0);
			}
			CurrentFragmentShader = shader;
		}
	}

	FORCEINLINE void SetGeometryShader(const EGeometryShader shader)
	{
		if (CurrentGeometryShader != shader)
		{
			if (shader == EGeometryShader::eInvalid)
			{
				EDirectXInit::DeviceContext->GSSetShader(nullptr, nullptr, 0);
			}
			else
			{
				EDirectXInit::DeviceContext->GSSetShader(GeometryShader[ECastEnum(shader)], nullptr, 0);
			}
			CurrentGeometryShader = shader;
		}
	}

	void SetProgramShader(const ECombinedShader programShader)
	{
		if (CurrentProgramShader != programShader)
		{
			if(programShader == ECombinedShader::eInvalid)
			{
				SetTopologyType(ETopologyType::eInvalid);
				SetInputLayout(EInputLayout::eInvalid);
				SetVertexShader(EVertexShader::eInvalid);
				SetFragmentShader(EFragmentShader::eInvalid);
				SetGeometryShader(EGeometryShader::eInvalid);
			}
			else
			{
				const ECombinedShaderData &data = CombinedShaderList[ECastEnum(programShader)];
				SetTopologyType(data.TopologyType);
				SetInputLayout(data.InputLayout);
				SetVertexShader(data.Vertex);
				SetFragmentShader(data.Fragment);
				SetGeometryShader(data.Geometry);
			}

			CurrentProgramShader = programShader;
		}
	}
};
#endif