#include "stdafx.h"

#if OPENGL_BACKEND_ENABLE == 1
namespace EOpenGLAttributes
{
	EAttributeData UIDefaultAttributes[] =
	{
		EAttributeData(0, 2, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHUIDefaultVertex), offsetof(SHUIDefaultVertex, Position)),
		EAttributeData(1, 2, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHUIDefaultVertex), offsetof(SHUIDefaultVertex, UV)),
		EAttributeData(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, GL_TRUE, sizeof(SHUIDefaultVertex), offsetof(SHUIDefaultVertex, Color)),
	};

	EAttributeData FontAttributes[] =
	{
		EAttributeData(0, 4, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHFontInstance), offsetof(SHFontInstance, Position)),
		EAttributeData(1, 4, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHFontInstance), offsetof(SHFontInstance, UV)),
		EAttributeData(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, GL_TRUE, sizeof(SHFontInstance), offsetof(SHFontInstance, Color)),
		EAttributeData(3, 1, GL_UNSIGNED_BYTE, GL_TRUE, GL_FALSE, sizeof(SHFontInstance), offsetof(SHFontInstance, TextureIndex)),
	};

	EAttributeData TerrainAttributes[] =
	{
		EAttributeData(0, 4, GL_UNSIGNED_BYTE, GL_TRUE, GL_FALSE, sizeof(SHTerrainInstance), offsetof(SHTerrainInstance, Position)),
#ifdef USE_COMPRESSED_VERTICES
		EAttributeData(1, 2, GL_HALF_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHTerrainInstance), offsetof(SHTerrainInstance, UV)),
#else
		EAttributeData(1, 2, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHTerrainInstance), offsetof(SHTerrainInstance, UV)),
#endif
	};

	EAttributeData GrassAttributes[] =
	{
#ifdef USE_COMPRESSED_VERTICES
		EAttributeData(0, 2, GL_HALF_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHGrassInstance), offsetof(SHGrassInstance, Position)),
#else
		EAttributeData(0, 2, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHGrassInstance), offsetof(SHGrassInstance, Position)),
#endif
		EAttributeData(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, GL_FALSE, sizeof(SHGrassInstance), offsetof(SHGrassInstance, Data)),
	};

	EAttributeData WaterAttributes[] =
	{
#ifdef USE_COMPRESSED_VERTICES
		EAttributeData(0, 4, GL_HALF_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHWaterVertex), offsetof(SHWaterVertex, Position)),
		EAttributeData(1, 4, GL_HALF_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHWaterVertex), offsetof(SHWaterVertex, Normal)),
#else
		EAttributeData(0, 3, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHWaterVertex), offsetof(SHWaterVertex, Position)),
		EAttributeData(1, 3, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHWaterVertex), offsetof(SHWaterVertex, Normal)),
#endif
	};

	EAttributeData ModelAttributes[] =
	{
#ifdef USE_COMPRESSED_VERTICES
		EAttributeData(0, 4, GL_HALF_FLOAT, GL_FALSE, GL_FALSE, sizeof(Model::VertexData1), offsetof(Model::VertexData1, Position)),
		EAttributeData(1, 2, GL_HALF_FLOAT, GL_FALSE, GL_FALSE, sizeof(Model::VertexData1), offsetof(Model::VertexData1, UV)),
		EAttributeData(2, 4, GL_HALF_FLOAT, GL_FALSE, GL_FALSE, sizeof(Model::VertexData1), offsetof(Model::VertexData1, Normal)),
#else
		EAttributeData(0, 3, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(Model::VertexData1), offsetof(Model::VertexData1, Position)),
		EAttributeData(1, 2, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(Model::VertexData1), offsetof(Model::VertexData1, UV)),
		EAttributeData(2, 3, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(Model::VertexData1), offsetof(Model::VertexData1, Normal)),
#endif
		EAttributeData(3, 2, GL_UNSIGNED_BYTE, GL_TRUE, GL_FALSE, sizeof(Model::VertexData1), offsetof(Model::VertexData1, Bone)),
	};

	EAttributeData ClothAttributes[] =
	{
#ifdef USE_COMPRESSED_VERTICES
		EAttributeData(0, 4, GL_HALF_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHClothVertexData), offsetof(SHClothVertexData, Position)),
		EAttributeData(1, 2, GL_HALF_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHClothVertexData), offsetof(SHClothVertexData, UV)),
		EAttributeData(2, 4, GL_HALF_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHClothVertexData), offsetof(SHClothVertexData, Normal)),
#else
		EAttributeData(0, 3, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHClothVertexData), offsetof(SHClothVertexData, Position)),
		EAttributeData(1, 2, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHClothVertexData), offsetof(SHClothVertexData, UV)),
		EAttributeData(2, 3, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHClothVertexData), offsetof(SHClothVertexData, Normal)),
#endif
	};

	EAttributeData InstanceModelAttributes[] =
	{
		EAttributeData(6, 3, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHModelInstance), offsetof(SHModelInstance, BodyOrigin)),
#ifdef USE_COMPRESSED_INSTANCES
		EAttributeData(7, 2, GL_HALF_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHModelInstance), offsetof(SHModelInstance, BodyScale)),
		EAttributeData(8, 2, GL_HALF_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHModelInstance), offsetof(SHModelInstance, Data)),
		EAttributeData(9, 4, GL_HALF_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHModelInstance), offsetof(SHModelInstance, Data2)),
#else
		EAttributeData(7, 2, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHModelInstance), offsetof(SHModelInstance, BodyScale)),
		EAttributeData(8, 2, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHModelInstance), offsetof(SHModelInstance, Data)),
		EAttributeData(9, 4, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHModelInstance), offsetof(SHModelInstance, Data2)),
#endif
		EAttributeData(10, 4, GL_UNSIGNED_BYTE, GL_FALSE, GL_TRUE, sizeof(SHModelInstance), offsetof(SHModelInstance, BodyLight)),
		EAttributeData(11, 4, GL_UNSIGNED_BYTE, GL_FALSE, GL_TRUE, sizeof(SHModelInstance), offsetof(SHModelInstance, ShadowLight)),
		EAttributeData(12, 1, GL_UNSIGNED_INT, GL_TRUE, GL_FALSE, sizeof(SHModelInstance), offsetof(SHModelInstance, BoneIndex)),
	};

	EAttributeData InstanceEffectAttributes[] =
	{
		EAttributeData(0, 4, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHEffectInstance), (offsetof(SHEffectInstance, Matrix) + sizeof(EVector4) * 0)),
		EAttributeData(1, 4, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHEffectInstance), (offsetof(SHEffectInstance, Matrix) + sizeof(EVector4) * 1)),
		EAttributeData(2, 4, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHEffectInstance), (offsetof(SHEffectInstance, Matrix) + sizeof(EVector4) * 2)),
		EAttributeData(3, 4, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHEffectInstance), (offsetof(SHEffectInstance, Matrix) + sizeof(EVector4) * 3)),
#ifdef USE_COMPRESSED_INSTANCES
		EAttributeData(4, 4, GL_HALF_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHEffectInstance), offsetof(SHEffectInstance, UV)),
		EAttributeData(5, 2, GL_HALF_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHEffectInstance), offsetof(SHEffectInstance, Size)),
#else
		EAttributeData(4, 4, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHEffectInstance), offsetof(SHEffectInstance, UV)),,
		EAttributeData(5, 2, GL_FLOAT, GL_FALSE, GL_FALSE, sizeof(SHEffectInstance), offsetof(SHEffectInstance, Size)),
#endif
		EAttributeData(6, 4, GL_UNSIGNED_BYTE, GL_FALSE, GL_TRUE, sizeof(SHEffectInstance), offsetof(SHEffectInstance, Color)),
		EAttributeData(7, 4, GL_UNSIGNED_BYTE, GL_TRUE, GL_FALSE, sizeof(SHEffectInstance), offsetof(SHEffectInstance, Texture)),
	};

	void Retrieve(const EInputLayout layout,
		mu_uint32 &count,
		EAttributeData *&attributes,
		mu_uint32 &instanceCount,
		EAttributeData *&instanceAttributes)
	{
		count = 0;
		attributes = nullptr;
		instanceCount = 0;
		instanceAttributes = nullptr;

		switch(layout)
		{
		case EInputLayout::eUIDefault:
			{
				count = mu_countof(UIDefaultAttributes);
				attributes = UIDefaultAttributes;
			}
			break;

		case EInputLayout::eFont:
			{
				instanceCount = mu_countof(FontAttributes);
				instanceAttributes = FontAttributes;
			}
			break;

		case EInputLayout::eTerrainCommon:
			{
				instanceCount = mu_countof(TerrainAttributes);
				instanceAttributes = TerrainAttributes;
			}
			break;

		case EInputLayout::eGrass:
			{
				instanceCount = mu_countof(GrassAttributes);
				instanceAttributes = GrassAttributes;
			}
			break;

		case EInputLayout::eWater:
			{
				count = mu_countof(WaterAttributes);
				attributes = WaterAttributes;
			}
			break;

		case EInputLayout::eModel:
			{
				count = mu_countof(ModelAttributes);
				attributes = ModelAttributes;
				instanceCount = mu_countof(InstanceModelAttributes);
				instanceAttributes = InstanceModelAttributes;
			}
			break;

		case EInputLayout::eCloth:
			{
				count = mu_countof(ClothAttributes);
				attributes = ClothAttributes;
				instanceCount = mu_countof(InstanceModelAttributes);
				instanceAttributes = InstanceModelAttributes;
			}
			break;

		case EInputLayout::eEffect:
			{
				instanceCount = mu_countof(InstanceEffectAttributes);
				instanceAttributes = InstanceEffectAttributes;
			}
			break;

		default: break;
		}
	}

	const mu_uint32 RetrieveInstanceSize(const EInputLayout layout)
	{
		switch (layout)
		{
		case EInputLayout::eTerrainCommon: return sizeof(SHTerrainInstance);
		case EInputLayout::eGrass: return sizeof(SHGrassInstance);
		case EInputLayout::eModel: return sizeof(SHModelInstance);
		case EInputLayout::eCloth: return sizeof(SHModelInstance);
		case EInputLayout::eEffect: return sizeof(SHEffectInstance);
		case EInputLayout::eFont: return sizeof(SHFontInstance);

		default: break;
		}

		return 0;
	}
};
#endif