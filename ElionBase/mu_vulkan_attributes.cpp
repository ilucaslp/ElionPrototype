#include "stdafx.h"

namespace EVulkanAttributes
{
	VkVertexInputBindingDescription UIDefaultBinding[] =
	{
		{ 0, sizeof(SHUIDefaultVertex), VK_VERTEX_INPUT_RATE_VERTEX },
	};

	VkVertexInputAttributeDescription UIDefaultAttributes[] =
	{
		{ 0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(SHUIDefaultVertex, Position) },
		{ 1, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(SHUIDefaultVertex, UV) },
		{ 2, 0, VK_FORMAT_R8G8B8A8_UNORM, offsetof(SHUIDefaultVertex, Color) },
	};

	VkVertexInputBindingDescription FontBinding[] =
	{
		{ 0, sizeof(SHFontInstance), VK_VERTEX_INPUT_RATE_INSTANCE },
	};

	VkVertexInputAttributeDescription FontAttributes[] =
	{
		{ 0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(SHFontInstance, Position) },
		{ 1, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(SHFontInstance, UV) },
		{ 2, 0, VK_FORMAT_R8G8B8A8_UNORM, offsetof(SHFontInstance, Color) },
		{ 3, 0, VK_FORMAT_R8_UINT, offsetof(SHFontInstance, TextureIndex) },
	};

	VkVertexInputBindingDescription TerrainBinding[] =
	{
		{ 0, sizeof(SHTerrainInstance), VK_VERTEX_INPUT_RATE_INSTANCE },
	};

	VkVertexInputAttributeDescription TerrainAttributes[] =
	{
		{ 0, 0, VK_FORMAT_R8G8B8A8_UINT, offsetof(SHTerrainInstance, Position) },
	#ifdef USE_COMPRESSED_VERTICES
		{ 1, 0, VK_FORMAT_R16G16_SFLOAT, offsetof(SHTerrainInstance, UV) },
	#else
		{ 1, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(SHTerrainInstance, UV) },
	#endif
	};

	VkVertexInputBindingDescription GrassBinding[] =
	{
		{ 0, sizeof(SHGrassInstance), VK_VERTEX_INPUT_RATE_INSTANCE },
	};

	VkVertexInputAttributeDescription GrassAttributes[] =
	{
	#ifdef USE_COMPRESSED_VERTICES
		{ 0, 0, VK_FORMAT_R16G16_SFLOAT, offsetof(SHGrassInstance, Position) },
	#else
		{ 0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(SHGrassInstance, Position) },
	#endif
		{ 1, 0, VK_FORMAT_R8G8B8A8_UINT, offsetof(SHGrassInstance, Data) },
	};

	VkVertexInputBindingDescription WaterBinding[] =
	{
		{ 0, sizeof(SHWaterVertex), VK_VERTEX_INPUT_RATE_VERTEX },
	};

	VkVertexInputAttributeDescription WaterAttributes[] =
	{
	#ifdef USE_COMPRESSED_VERTICES
		{ 0, 0, VK_FORMAT_R16G16B16A16_SFLOAT, offsetof(SHWaterVertex, Position) },
		{ 1, 0, VK_FORMAT_R16G16B16A16_SFLOAT, offsetof(SHWaterVertex, Normal) },
	#else
		{ 0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(SHWaterVertex, Position) },
		{ 1, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(SHWaterVertex, Normal) },
	#endif
	};

	VkVertexInputBindingDescription ModelBinding[] =
	{
		{ 0, sizeof(Model::VertexData1), VK_VERTEX_INPUT_RATE_VERTEX },
		{ 1, sizeof(SHModelInstance), VK_VERTEX_INPUT_RATE_INSTANCE },
	};

	VkVertexInputAttributeDescription ModelAttributes[] =
	{
	#ifdef USE_COMPRESSED_VERTICES
		{ 0, 0, VK_FORMAT_R16G16B16A16_SFLOAT, offsetof(Model::VertexData1, Position) },
		{ 1, 0, VK_FORMAT_R16G16_SFLOAT, offsetof(Model::VertexData1, UV) },
		{ 2, 0, VK_FORMAT_R16G16B16A16_SFLOAT, offsetof(Model::VertexData1, Normal) },
	#else
		{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Model::VertexData1, Position) },
		{ 1, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Model::VertexData1, UV) },
		{ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Model::VertexData1, Normal) },
	#endif
		{ 3, 0, VK_FORMAT_R8G8_UINT, offsetof(Model::VertexData1, Bone) },

		{ 6, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(SHModelInstance, BodyOrigin) },
	#ifdef USE_COMPRESSED_VERTICES
		{ 7, 1, VK_FORMAT_R16G16_SFLOAT, offsetof(SHModelInstance, BodyScale) },
		{ 8, 1, VK_FORMAT_R16G16_SFLOAT, offsetof(SHModelInstance, Data) },
		{ 9, 1, VK_FORMAT_R16G16B16A16_SFLOAT, offsetof(SHModelInstance, Data2) },
	#else
		{ 7, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(SHModelInstance, BodyScale) },
		{ 8, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(SHModelInstance, Data) },
		{ 9, 1, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(SHModelInstance, Data2) },
	#endif
		{ 10, 1, VK_FORMAT_R8G8B8A8_UNORM, offsetof(SHModelInstance, BodyLight) },
		{ 11, 1, VK_FORMAT_R8G8B8A8_UNORM, offsetof(SHModelInstance, ShadowLight) },
		{ 12, 1, VK_FORMAT_R32_UINT, offsetof(SHModelInstance, BoneIndex) },
	};

	VkVertexInputBindingDescription ClothBinding[] =
	{
		{ 0, sizeof(SHClothVertexData), VK_VERTEX_INPUT_RATE_VERTEX },
		{ 1, sizeof(SHModelInstance), VK_VERTEX_INPUT_RATE_INSTANCE },
	};

	VkVertexInputAttributeDescription ClothAttributes[] =
	{
	#ifdef USE_COMPRESSED_VERTICES
		{ 0, 0, VK_FORMAT_R16G16B16A16_SFLOAT, offsetof(SHClothVertexData, Position) },
		{ 1, 0, VK_FORMAT_R16G16_SFLOAT, offsetof(SHClothVertexData, UV) },
		{ 2, 0, VK_FORMAT_R16G16B16A16_SFLOAT, offsetof(SHClothVertexData, Normal) },
	#else
		{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(SHClothVertexData, Position) },
		{ 1, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(SHClothVertexData, UV) },
		{ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(SHClothVertexData, Normal) },
	#endif

		{ 6, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(SHModelInstance, BodyOrigin) },
	#ifdef USE_COMPRESSED_VERTICES
		{ 7, 1, VK_FORMAT_R16G16_SFLOAT, offsetof(SHModelInstance, BodyScale) },
		{ 8, 1, VK_FORMAT_R16G16_SFLOAT, offsetof(SHModelInstance, Data) },
		{ 9, 1, VK_FORMAT_R16G16B16A16_SFLOAT, offsetof(SHModelInstance, Data2) },
	#else
		{ 7, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(SHModelInstance, BodyScale) },
		{ 8, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(SHModelInstance, Data) },
		{ 9, 1, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(SHModelInstance, Data2) },
	#endif
		{ 10, 1, VK_FORMAT_R8G8B8A8_UNORM, offsetof(SHModelInstance, BodyLight) },
		{ 11, 1, VK_FORMAT_R8G8B8A8_UNORM, offsetof(SHModelInstance, ShadowLight) },
		{ 12, 1, VK_FORMAT_R32_UINT, offsetof(SHModelInstance, BoneIndex) },
	};

	VkVertexInputBindingDescription EffectBinding[] =
	{
		{ 0, sizeof(SHEffectInstance), VK_VERTEX_INPUT_RATE_INSTANCE },
	};

	VkVertexInputAttributeDescription EffectAttributes[] =
	{
		{ 0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, (offsetof(SHEffectInstance, Matrix) + sizeof(EVector4) * 0) },
		{ 1, 0, VK_FORMAT_R32G32B32A32_SFLOAT, (offsetof(SHEffectInstance, Matrix) + sizeof(EVector4) * 1) },
		{ 2, 0, VK_FORMAT_R32G32B32A32_SFLOAT, (offsetof(SHEffectInstance, Matrix) + sizeof(EVector4) * 2) },
		{ 3, 0, VK_FORMAT_R32G32B32A32_SFLOAT, (offsetof(SHEffectInstance, Matrix) + sizeof(EVector4) * 3) },
	#ifdef USE_COMPRESSED_VERTICES
		{ 4, 0, VK_FORMAT_R16G16B16A16_SFLOAT, offsetof(SHEffectInstance, UV) },
		{ 5, 0, VK_FORMAT_R16G16_SFLOAT, offsetof(SHEffectInstance, Size) },
	#else
		{ 4, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(SHEffectInstance, UV) },
		{ 5, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(SHEffectInstance, Size) },
	#endif
		{ 6, 0, VK_FORMAT_R8G8B8A8_UNORM, offsetof(SHEffectInstance, Color) },
		{ 7, 0, VK_FORMAT_R8G8B8A8_UINT, offsetof(SHEffectInstance, Texture) },
	};

	VkVertexInputBindingDescription BboxBinding[] =
	{
		{ 0, sizeof(SHContainersVertex), VK_VERTEX_INPUT_RATE_VERTEX },
	};

	VkVertexInputAttributeDescription BboxAttributes[] =
	{
		{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(SHContainersVertex, Pos) },
		{ 1, 0, VK_FORMAT_R32_SFLOAT, offsetof(SHContainersVertex, Color) },
	};

	void Retrieve(const EInputLayout layout,
		mu_uint32 &bindingCount,
		const VkVertexInputBindingDescription *&bindings,
		mu_uint32 &attributesCount,
		const VkVertexInputAttributeDescription *&attributes)
	{
		bindingCount = 0;
		bindings = nullptr;
		attributesCount = 0;
		attributes = nullptr;

		switch(layout)
		{
		case EInputLayout::eUIDefault:
			{
				bindingCount = mu_countof(UIDefaultBinding);
				bindings = UIDefaultBinding;
				attributesCount = mu_countof(UIDefaultAttributes);
				attributes = UIDefaultAttributes;
			}
			break;

		case EInputLayout::eFont:
			{
				bindingCount = mu_countof(FontBinding);
				bindings = FontBinding;
				attributesCount = mu_countof(FontAttributes);
				attributes = FontAttributes;
			}
			break;

		case EInputLayout::eTerrainCommon:
			{
				bindingCount = mu_countof(TerrainBinding);
				bindings = TerrainBinding;
				attributesCount = mu_countof(TerrainAttributes);
				attributes = TerrainAttributes;
			}
			break;

		case EInputLayout::eGrass:
			{
				bindingCount = mu_countof(GrassBinding);
				bindings = GrassBinding;
				attributesCount = mu_countof(GrassAttributes);
				attributes = GrassAttributes;
			}
			break;

		case EInputLayout::eWater:
			{
				bindingCount = mu_countof(WaterBinding);
				bindings = WaterBinding;
				attributesCount = mu_countof(WaterAttributes);
				attributes = WaterAttributes;
			}
			break;

		case EInputLayout::eModel:
			{
				bindingCount = mu_countof(ModelBinding);
				bindings = ModelBinding;
				attributesCount = mu_countof(ModelAttributes);
				attributes = ModelAttributes;
			}
			break;

		case EInputLayout::eCloth:
			{
				bindingCount = mu_countof(ClothBinding);
				bindings = ClothBinding;
				attributesCount = mu_countof(ClothAttributes);
				attributes = ClothAttributes;
			}
			break;

		case EInputLayout::eEffect:
			{
				bindingCount = mu_countof(EffectBinding);
				bindings = EffectBinding;
				attributesCount = mu_countof(EffectAttributes);
				attributes = EffectAttributes;
			}
			break;

		default: break;
		}
	}
};