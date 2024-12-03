#ifndef __MU_MODEL_H__
#define __MU_MODEL_H__

#pragma once

#include "mu_threadinfo.h"

#define MAX_MODEL_SOUND 10
#define INVALID_SOUND 0xFFFF
#define BONE_INVALID -1

#define PREFETCH_DATA_SIZE 64
#define PREFETCH_BONE_COUNT 2 // (sizeof(EDualQuat) / PREFETCH_DATA_SIZE)

enum SHADOW_TYPE
{
	SHADOW_NONE,
	SHADOW_RENDER_COLOR,
	SHADOW_RENDER_TEXTURE,
};

namespace Model
{
	enum TEXTURE_SPECIAL
	{
		TEXSPCL_SKIN = -1,
		TEXSPCL_HAIR = -2,
		TEXSPCL_HIDDEN = -3,
	};

	/*
	Structure:
	Vertex File(emdl):
		[Header Signature] (4 bytes) (uint)
		[Header Version] (2 bytes) (ushort)
		[CRC32] (4 bytes) (uint)
		[Encrypted Data] (aligned 8 bytes)
			[Compressed Size] (4 bytes) (uint)
			[Decompressed Size] (4 bytes) (uint)
			[Compressed Data]
			(Begin)
				[Vertex Header] (based on version) (VertexHeaderX)
				[Vertex Data Count] (4 bytes) (uint)
				[Vertex Data Array] (based on version)[VertexCount] (VertexDataX)
				[Index Data Count] (4 bytes) (uint)
				[Index Data Array] (2/4 bytes based on vertex header flag)[IndexCount] (uint16/uint32)
				[Mesh Data Count] (4 bytes) (uint)
				[Mesh Data Array] (based on version)[MeshCount] (MeshDataX)
				[Render Data Count] (4 bytes) (uint)
				[Render Data Array] (based on version)[RenderCount] (RenderDataX)
				[Texture Data Count] (4 bytes) (uint)
				[Texture Data Array] (based on version)[TextureCount] (TextureDataX)
				[Bounding Sphere] (based on version) (BoundingSphereX)
			(End)

	Animation File(eani, only if non-static mesh):
		[Header Signature] (4 bytes) (uint)
		[Header Version] (2 bytes) (ushort)
		[CRC32] (4 bytes) (uint)
		[Encrypted Data] (aligned 8 bytes)
			[Compressed Size] (4 bytes) (uint)
			[Decompressed Size] (4 bytes) (uint)
			[Compressed Data]
			(Begin)
				[Animations Header] (based on version) (AnimationHeaderX)
				[Animations Data Count] (4 bytes) (uint)
				[Bones Data Count] (4 bytes) (uint)
				[Animations Data Array] (based on version)[AnimationsCount] (AnimationDataX)
				(Begin)
					[Animation Repeat] (1 byte) (boolean)
					[Animation Speed] (4 bytes) (mu_float)
					[Frames Data Count] (4 bytes) (uint)
					[Frames Data Array] (based on version)[FramesCount] (FrameDataX)
					(Begin)
						[Bones Data Array] (based on version)[BonesCount] (BoneDataX)
					(End)
				(End)
				[Bones Reference Array] (based on version)[BonesCount] (BoneRefX)
			(End)
	*/
	enum
	{
		INVALID_LIGHT = 0xFFFFFFFF,
		INVALID_MODEL = 0xFFFFFFFF,
	};

	struct VertexHeader1
	{
		VertexHeader1()
		{
			IndexType = 0;
			HasPhysics = false;
			StaticMesh = false;
		}

		mu_uint8 IndexType;
		mu_boolean HasPhysics;
		mu_boolean StaticMesh;
	};

	MU_ALIGN(4, struct) VertexData1
	{
		VertexData1()
		{
#ifdef USE_COMPRESSED_VERTICES
			mu_zeromem(Position, sizeof(Position));
			mu_zeromem(UV, sizeof(UV));
			mu_zeromem(&Normal, sizeof(Normal));
#else
			mu_zeromem(&Position, sizeof(Position));
			mu_zeromem(&UV, sizeof(UV));
			mu_zeromem(&Normal, sizeof(Normal));
#endif
			mu_zeromem(Bone, sizeof(Bone));
		}

#ifdef USE_COMPRESSED_VERTICES
		mu_uint16 Position[4];
		mu_uint16 UV[2];
		mu_uint16 Normal[4];
#else
		EVector3 Position;
		EVector2 UV;
		EVector3 Normal;
#endif
		mu_uint8 Bone[2];
	};

	struct MeshVertex1
	{
		mu_int16 Node;
		EVector3 Position;
	};

	struct MeshNormal1
	{
		mu_int16 Node;
		EVector3 Normal;
	};

	struct MeshTriangle1
	{
		mu_uint16 VertexIndex[3];
		mu_uint16 NormalIndex[3];
		mu_uint16 TexCoordIndex[3];
	};

	struct Mesh1
	{
		Mesh1()
		{
			Vertices = nullptr;
			Normals = nullptr;
			TexCoords = nullptr;
			Triangles = nullptr;
		}

		~Mesh1()
		{
			MU_SAFE_DELETE_ARRAY(Vertices);
			MU_SAFE_DELETE_ARRAY(Normals);
			MU_SAFE_DELETE_ARRAY(TexCoords);
			MU_SAFE_DELETE_ARRAY(Triangles);
		}

		mu_uint16 NumVertices;
		mu_uint16 NumNormals;
		mu_uint16 NumTexCoords;
		mu_uint16 NumTriangles;
		MeshVertex1 *Vertices;
		MeshNormal1 *Normals;
		EVector2 *TexCoords;
		MeshTriangle1 *Triangles;
	};

	struct MeshData1
	{
		MeshData1()
		{
			VertexStart = 0;
			VertexCount = 0;
			IndexStart = 0;
			IndexCount = 0;
		}

		mu_uint32 VertexStart;
		mu_uint32 VertexCount;
		mu_uint32 IndexStart;
		mu_uint32 IndexCount;
	};

	struct MeshExtra1
	{
		MeshExtra1()
		{
			IsUsed = false;
			IsBright = false;
			IsHidden = false;
			IsStream = false;
			IsNoneBlend = false;
			ShadowMapping = SHADOW_NONE;
		}

		mu_boolean IsUsed;
		mu_boolean IsBright;
		mu_boolean IsHidden;
		mu_boolean IsStream;
		mu_boolean IsNoneBlend;
		mu_uint8 ShadowMapping;
	};

	struct BoneRef1
	{
		BoneRef1()
		{
			Dummy = false;
			Parent = BONE_INVALID;
		}

		mu_boolean Dummy;
		mu_int16 Parent;
	};

	struct FrameData1
	{
		FrameData1()
		{
			BoneData = nullptr;
		}

		~FrameData1()
		{
			MU_ALIGNED_FREE(BoneData);
		}

		EDualQuat *BoneData;
	};

	struct AnimationHeader1
	{
		AnimationHeader1()
		{
			Repeat = true;
			LockPositions = false;
			Speed = 1.0f;
			FrameCount = 0;
		}

		mu_boolean Repeat;
		mu_boolean LockPositions;
		mu_float Speed;
		mu_uint32 FrameCount;
	};

	struct AnimationData1
	{
		AnimationData1()
		{
			FrameData = nullptr;
		}

		~AnimationData1()
		{
			MU_SAFE_DELETE_ARRAY(FrameData);
		}

		FrameData1 *FrameData;
	};

	struct BoundingSphere1
	{
		BoundingSphere1()
		{
			mu_zeromem(&Center, sizeof(Center));
			Radius = 0.0f;
		}

		EVector3 Center;
		mu_float Radius;
	};

	struct BoundingBox1
	{
		BoundingBox1()
		{
			mu_zeromem(&Min, sizeof(Min));
			mu_zeromem(&Max, sizeof(Max));
		}

		EVector3 Min;
		EVector3 Max;
	};
};

class MUModel : public EModelBackend
{
private:
	friend class MUTerrain;
	friend class MUCPUCulling;
	friend class MUTasking;

	static const mu_uint8 CryptKey[];
	static MUBlowfish Crypt;

public:
	enum VERSION : mu_uint16
	{
		VERSION_1 = 0x0001,

		CVERSION = VERSION_1,
	};

	enum CONFIG
	{
		INDEX_16BIT = 0,
		INDEX_32BIT = 1,

		NO_PHYSICS = 0,
		USE_PHYSICS = 1,
	};

	enum ERROR_CODE
	{
		ERROR_UNKNOWN_BONE = 0xFF000000,
	};

	static mu_boolean IsError(mu_uint32 errorCode)
	{
		return errorCode >= ERROR_UNKNOWN_BONE;
	}

public:
	MUModel();
	~MUModel();

	void Release();
	void ReleaseHeader();
	void ReleaseData();
	void ReleaseGPU(const mu_boolean dataOnly = false);

	void TryRealtimeLoad();
	void RequestLoad();
	const mu_boolean LoadHeaderFromFileEx(const mu_text *Path);
	const mu_boolean LoadDataFromFile(const mu_text *Path);
	const mu_boolean LoadGPUFromFile(const mu_text *Path);
	const mu_boolean LoadFromFileEx(const mu_text *Path);
	const mu_boolean TransferCPUtoGPU(const Model::VertexData1 *vertexData, const mu_uint32 vertexCount, const void *indexData, const mu_uint32 indexCount);

	void ConfigureCloth(mu_uint32 MeshIndex, mu_int32 NumHor, mu_int32 NumVer);

	mu_boolean PlayAnimation(const mu_uint32 ThreadIndex, mu_float *AnimationFrame, mu_float *PriorAnimationFrame, mu_uint16 *PriorAction, mu_float Speed);
	
#ifdef USE_DUAL_QUATERNION
	void Animation(const mu_uint32 ThreadIndex, EDualQuat *BoneMatrix, mu_float AnimationFrame, mu_float PriorFrame, mu_uint16 PriorAction, EVector3 Angle, EVector3 HeadAngle, mu_boolean Parent = false, mu_boolean Translate = true);
#else
	void Animation(const mu_uint32 ThreadIndex, EMatrix4 *BoneMatrix, mu_float AnimationFrame, mu_float PriorFrame, mu_uint16 PriorAction, EVector3 Angle, EVector3 HeadAngle, mu_boolean Parent = false, mu_boolean Translate = true);
#endif
	void Transform(const mu_uint32 ThreadIndex, EVector3 &BoundingBoxMin, EVector3 &BoundingBoxMax, OBB_t *OBB, mu_boolean Translate = false);

	void RenderMesh(const mu_uint32 ThreadIndex, mu_int32 i, mu_int32 RenderFlag, mu_float Alpha = 1.0f, mu_int32 BlendMesh = -1, mu_float BlendMeshLight = 1.0f, mu_float BlendMeshTexCoordU = 0.0f, mu_float BlendMeshTexCoordV = 0.0f, mu_int32 MeshTexture = -1);
	void RenderBody(const mu_uint32 ThreadIndex, mu_int32 RenderFlag, mu_float Alpha = 1.0f, mu_int32 BlendMesh = -1, mu_float BlendMeshLight = 1.0f, mu_float BlendMeshTexCoordU = 0.0f, mu_float BlendMeshTexCoordV = 0.0f, mu_int32 HiddenMesh = -1, mu_int32 Texture = -1);
#ifdef USE_DUAL_QUATERNION
	void RenderMeshEffect(const mu_uint32 ThreadIndex, const EDualQuat *BoneMatrix, mu_int32 i, mu_int32 iType, mu_int32 iSubType = 0, EVector3 Angle = EVector3(0.0f, 0.0f, 0.0f), void* obj = nullptr);
#else
	void RenderMeshEffect(const mu_uint32 ThreadIndex, const EMatrix4 *BoneMatrix, mu_int32 i, mu_int32 iType, mu_int32 iSubType = 0, EVector3 Angle = EVector3(0.0f, 0.0f, 0.0f), void* obj = nullptr);
#endif

#ifdef USE_DUAL_QUATERNION
	void TransformVertices(const mu_uint32 ThreadIndex, mu_int32 Mesh, const EDualQuat *BoneMatrix);
#else
	void TransformVertices(const mu_uint32 ThreadIndex, mu_int32 Mesh, const EMatrix4 *BoneMatrix);
#endif
	void TransformByObjectBone(const mu_uint32 ThreadIndex, EVector3 &vResultPosition, OBJECT * pObject, mu_int32 iBoneNumber, EVector3 *vRelativePosition = nullptr) const;
	void TransformByObjectBoneDirect(const mu_uint32 ThreadIndex, EVector3 &vResultPosition, OBJECT * pObject, mu_int32 iBoneNumber, EVector3 *vRelativePosition = nullptr) const;
#ifdef USE_DUAL_QUATERNION
	void TransformByBoneMatrix(const mu_uint32 ThreadIndex, EVector3 &vResultPosition, const EDualQuat &BoneMatrix, EVector3 *vWorldPosition = nullptr, EVector3 *vRelativePosition = nullptr);
	void TransformPosition(const mu_uint32 ThreadIndex, const EDualQuat &Matrix, EVector3 Position, EVector3 &WorldPosition, mu_boolean Translate = false);
	void RotationPosition(const mu_uint32 ThreadIndex, const EDualQuat &Matrix, EVector3 Position, EVector3 &WorldPosition);
#else
	void TransformByBoneMatrix(const mu_uint32 ThreadIndex, EVector3 &vResultPosition, const EMatrix4 &BoneMatrix, EVector3 *vWorldPosition = nullptr, EVector3 *vRelativePosition = nullptr);
	void TransformPosition(const mu_uint32 ThreadIndex, const EMatrix4 &Matrix, EVector3 Position, EVector3 &WorldPosition, mu_boolean Translate = false);
	void RotationPosition(const mu_uint32 ThreadIndex, const EMatrix4 &Matrix, EVector3 Position, EVector3 &WorldPosition);
#endif

	void AnimationTransformWithAttachHighModel_usingGlobalTM(
		mu_uint32 ThreadIndex,
		OBJECT* oHighHierarchyModel,
		MUModel* bmdHighHierarchyModel,
		mu_int32 iBoneNumberHighHierarchyModel,
		EVector3 *vOutPosHighHiearachyModelBone,
		EVector3 *arrOutSetfAllBonePositions,
		mu_boolean bApplyTMtoVertices);

	void AnimationTransformWithAttachHighModel(
		mu_uint32 ThreadIndex,
		OBJECT* oHighHierarchyModel,
		MUModel* bmdHighHierarchyModel,
		mu_int32 iBoneNumberHighHierarchyModel,
		EVector3 *vOutPosHighHiearachyModelBone,
		EVector3 *arrOutSetfAllBonePositions);

	void AnimationTransformOnlySelf(
		mu_uint32 ThreadIndex,
		EVector3 *arrOutSetfAllBonePositions,
		const OBJECT* oSelf);

	void AnimationTransformOnlySelf(
		mu_uint32 ThreadIndex,
		EVector3 *arrOutSetfAllBonePositions,
		const EVector3 &v3Angle,
		const EVector3 &v3Position,
		const mu_float &fScale,
		OBJECT* oRefAnimation = nullptr,
		const mu_float fFrameArea = -1.0f,
		const mu_float fWeight = -1.0f);

private:
	template<bool isParentMatrix, bool isLockPosition, bool isDirectHead = false>
	FORCEINLINE void FastAnimationWithPriorAction(
		const mu_uint32 threadIndex,
		const MUThreadInfo &threadInfo,
		const mu_uint32 i,
#ifdef USE_DUAL_QUATERNION
		EDualQuat *boneMatrix,
#else
		EMatrix4 *boneMatrix,
#endif
		const Model::AnimationHeader1 &currentHeader,
		const Model::AnimationHeader1 &priorHeader,
		const Model::AnimationData1 &currentAction,
		const Model::AnimationData1 &priorAction,
		const EDualQuat &currentBone1,
		const EDualQuat &currentBone2,
		const EDualQuat &priorBone1,
		const EDualQuat &priorBone2,
		const mu_float s1,
		const mu_float s2,
		const mu_float ps1,
		const mu_float ps2,
		const mu_int32 boneHead,
		const EQuat &headRotation,
		const mu_int32 parentBone = -1)
	{
		const EVector3 &cp1 = currentBone1.Position;
		const EVector3 &cp2 = currentBone2.Position;
		const EVector3 &pp1 = priorBone1.Position;
		const EVector3 &pp2 = priorBone2.Position;

		const EVector3 p1 = pp1 * ps2 + pp2 * ps1;
		const EVector3 p2 = cp1 * s2 + cp2 * s1;

		EQuat q1, q2;
		if constexpr(isDirectHead == true)
		{
			const EQuat &cq1 = currentBone1.Rotation;
			const EQuat &cq2 = currentBone2.Rotation;
			const EQuat &pq1 = priorBone1.Rotation;
			const EQuat &pq2 = priorBone2.Rotation;

			QuaternionSlerp(pq1, pq2, ps1, q1);
			QuaternionSlerp(cq1, cq2, s1, q2);

			q1 = q1 * headRotation;
			q2 = q2 * headRotation;
		}
		else
		{
			if (i == boneHead)
			{
				const EQuat &cq1 = currentBone1.Rotation;
				const EQuat &cq2 = currentBone2.Rotation;
				const EQuat &pq1 = priorBone1.Rotation;
				const EQuat &pq2 = priorBone2.Rotation;

				QuaternionSlerp(pq1, pq2, ps1, q1);
				QuaternionSlerp(cq1, cq2, s1, q2);

				q1 = q1 * headRotation;
				q2 = q2 * headRotation;
			}
			else
			{
				const EQuat &cq1 = currentBone1.Rotation;
				const EQuat &cq2 = currentBone2.Rotation;
				const EQuat &pq1 = priorBone1.Rotation;
				const EQuat &pq2 = priorBone2.Rotation;

				QuaternionSlerp(pq1, pq2, ps1, q1);
				QuaternionSlerp(cq1, cq2, s1, q2);
			}
		}

#ifdef USE_DUAL_QUATERNION
		EDualQuat &DualQuat = boneMatrix[i];
		QuaternionSlerp(q1, q2, s1, DualQuat.Rotation);
		if constexpr(isLockPosition == true)
		{
			if (priorHeader.LockPositions || currentHeader.LockPositions)
			{
				EDualQuat &boneData = currentAction.FrameData[0].BoneData[0];
				DualQuat.Position[0] = boneData.Position[0];
				DualQuat.Position[1] = boneData.Position[1];
				DualQuat.Position[2] = p1[2] * s2 + p2[2] * s1 + BodyHeight(threadIndex);
			}
			else
			{
				DualQuat.Position = p1 * s2 + p2 * s1;
			}
		}
		else
		{
			DualQuat.Position = p1 * s2 + p2 * s1;
		}

		if constexpr(isParentMatrix)
		{
			DualQuat.Mix(threadInfo.ParentMatrix);
		}
		else
		{
			DualQuat.Mix(boneMatrix[parentBone]);
		}
#else
		EMatrix4 &Matrix = BoneMatrix[i];
		EQuat q;
		QuaternionSlerp(q1, q2, s1, q);
		if constexpr(isLockPosition == true)
		{
			if (priorHeader.LockPositions || currentHeader.LockPositions)
			{
				EDualQuat &boneData = currentAction.FrameData[0].BoneData[0];
				FastMakeMatrix(Matrix, q, EVector3(boneData.Position[0], boneData.Position[1], p1[2] * s2 + p2[2] * s1 + BodyHeight(threadIndex)));
			}
			else
			{
				FastMakeMatrix(Matrix, q, p1 * s2 + p2 * s1);
			}
		}
		else
		{
			FastMakeMatrix(Matrix, q, p1 * s2 + p2 * s1);
		}

		if constexpr(isParentMatrix)
		{
			Matrix = ThreadInfo.ParentMatrix * Matrix;
		}
		else
		{
			Matrix = boneMatrix[parentBone] * Matrix;
		}
#endif
	}

	template<bool isParentMatrix, bool isLockPosition, bool isDirectHead = false>
	FORCEINLINE void FastAnimationWithPriorAction(
		const mu_uint32 threadIndex,
		const MUThreadInfo &threadInfo,
		const mu_uint32 i,
#ifdef USE_DUAL_QUATERNION
		EDualQuat *boneMatrix,
#else
		EMatrix4 *boneMatrix,
#endif
		const Model::AnimationHeader1 &currentHeader,
		const Model::AnimationHeader1 &priorHeader,
		const Model::AnimationData1 &currentAction,
		const Model::AnimationData1 &priorAction,
		const EDualQuat &currentBone1,
		const EDualQuat &currentBone2,
		const EDualQuat &priorBone1,
		const EDualQuat &priorBone2,
		const mu_float s1,
		const mu_float s2,
		const mu_float ps1,
		const mu_float ps2,
		const mu_int32 parentBone = -1)
	{
		const EVector3 &cp1 = currentBone1.Position;
		const EVector3 &cp2 = currentBone2.Position;
		const EVector3 &pp1 = priorBone1.Position;
		const EVector3 &pp2 = priorBone2.Position;

		const EVector3 p1 = pp1 * ps2 + pp2 * ps1;
		const EVector3 p2 = cp1 * s2 + cp2 * s1;

		EQuat q1, q2;
		const EQuat &cq1 = currentBone1.Rotation;
		const EQuat &cq2 = currentBone2.Rotation;
		const EQuat &pq1 = priorBone1.Rotation;
		const EQuat &pq2 = priorBone2.Rotation;

		QuaternionSlerp(pq1, pq2, ps1, q1);
		QuaternionSlerp(cq1, cq2, s1, q2);

#ifdef USE_DUAL_QUATERNION
		EDualQuat &DualQuat = boneMatrix[i];
		QuaternionSlerp(q1, q2, s1, DualQuat.Rotation);
		if constexpr(isLockPosition == true)
		{
			if (priorHeader.LockPositions || currentHeader.LockPositions)
			{
				EDualQuat &boneData = currentAction.FrameData[0].BoneData[0];
				DualQuat.Position[0] = boneData.Position[0];
				DualQuat.Position[1] = boneData.Position[1];
				DualQuat.Position[2] = p1[2] * s2 + p2[2] * s1 + BodyHeight(threadIndex);
			}
			else
			{
				DualQuat.Position = p1 * s2 + p2 * s1;
			}
		}
		else
		{
			DualQuat.Position = p1 * s2 + p2 * s1;
		}

		if constexpr(isParentMatrix)
		{
			DualQuat.Mix(threadInfo.ParentMatrix);
		}
		else
		{
			DualQuat.Mix(boneMatrix[parentBone]);
		}
#else
		EMatrix4 &Matrix = BoneMatrix[i];
		EQuat q;
		QuaternionSlerp(q1, q2, s1, q);
		if constexpr(isLockPosition == true)
		{
			if (priorHeader.LockPositions || currentHeader.LockPositions)
			{
				EDualQuat &boneData = currentAction.FrameData[0].BoneData[0];
				FastMakeMatrix(Matrix, q, EVector3(boneData.Position[0], boneData.Position[1], p1[2] * s2 + p2[2] * s1 + BodyHeight(threadIndex)));
			}
			else
			{
				FastMakeMatrix(Matrix, q, p1 * s2 + p2 * s1);
			}
		}
		else
		{
			FastMakeMatrix(Matrix, q, p1 * s2 + p2 * s1);
		}

		if constexpr(isParentMatrix)
		{
			Matrix = ThreadInfo.ParentMatrix * Matrix;
		}
		else
		{
			Matrix = boneMatrix[parentBone] * Matrix;
		}
#endif
	}

	template<bool isParentMatrix, bool isLockPosition, bool isDirectHead = false>
	FORCEINLINE void FastAnimation(
		const mu_uint32 threadIndex,
		const MUThreadInfo &threadInfo,
		const mu_uint32 i,
#ifdef USE_DUAL_QUATERNION
		EDualQuat *boneMatrix,
#else
		EMatrix4 *boneMatrix,
#endif
		const Model::AnimationHeader1 &header,
		const Model::AnimationData1 &action,
		const EDualQuat &bone1,
		const EDualQuat &bone2,
		const mu_float s1,
		const mu_float s2,
		const mu_int32 boneHead,
		const EQuat &headRotation,
		const mu_int32 parentBone = -1)
	{
		const EVector3 &p1 = bone1.Position;
		const EVector3 &p2 = bone2.Position;

		EQuat q1, q2;
		if constexpr(isDirectHead == true)
		{
			q1 = bone1.Rotation * headRotation;
			q2 = bone2.Rotation * headRotation;
		}
		else
		{
			if (i == boneHead)
			{
				q1 = bone1.Rotation * headRotation;
				q2 = bone2.Rotation * headRotation;
			}
			else
			{
				q1 = bone1.Rotation;
				q2 = bone2.Rotation;
			}
		}

#ifdef USE_DUAL_QUATERNION
		EDualQuat &DualQuat = boneMatrix[i];
		QuaternionSlerp(q1, q2, s1, DualQuat.Rotation);
		if constexpr(isLockPosition == true)
		{
			if (header.LockPositions)
			{
				EDualQuat &boneData = action.FrameData[0].BoneData[0];
				DualQuat.Position[0] = boneData.Position[0];
				DualQuat.Position[1] = boneData.Position[1];
				DualQuat.Position[2] = p1[2] * s2 + p2[2] * s1 + BodyHeight(threadIndex);
			}
			else
			{
				DualQuat.Position = p1 * s2 + p2 * s1;
			}
		}
		else
		{
			DualQuat.Position = p1 * s2 + p2 * s1;
		}

		if constexpr(isParentMatrix)
		{
			DualQuat.Mix(threadInfo.ParentMatrix);
		}
		else
		{
			DualQuat.Mix(boneMatrix[parentBone]);
		}
#else
		EMatrix4 &Matrix = BoneMatrix[i];
		EQuat q;
		QuaternionSlerp(q1, q2, s1, q);
		if constexpr(isLockPosition == true)
		{
			if (header.LockPositions)
			{
				EDualQuat &boneData = action.FrameData[0].BoneData[0];
				FastMakeMatrix(Matrix, q, EVector3(boneData.Position[0], boneData.Position[1], p1[2] * s2 + p2[2] * s1 + BodyHeight(threadIndex)));
			}
			else
			{
				FastMakeMatrix(Matrix, q, p1 * s2 + p2 * s1);
			}
		}
		else
		{
			FastMakeMatrix(Matrix, q, p1 * s2 + p2 * s1);
		}

		if constexpr(isParentMatrix)
		{
			Matrix = ThreadInfo.ParentMatrix * Matrix;
		}
		else
		{
			Matrix = boneMatrix[parentBone] * Matrix;
		}
#endif
	}

	template<bool isParentMatrix, bool isLockPosition, bool isDirectHead = false>
	FORCEINLINE void FastAnimation(
		const mu_uint32 threadIndex,
		const MUThreadInfo &threadInfo,
		const mu_uint32 i,
#ifdef USE_DUAL_QUATERNION
		EDualQuat *boneMatrix,
#else
		EMatrix4 *boneMatrix,
#endif
		const Model::AnimationHeader1 &header,
		const Model::AnimationData1 &action,
		const EDualQuat &bone1,
		const EDualQuat &bone2,
		const mu_float s1,
		const mu_float s2,
		const mu_int32 parentBone = -1)
	{
		const EVector3 &p1 = bone1.Position;
		const EVector3 &p2 = bone2.Position;

#ifdef USE_DUAL_QUATERNION
		EDualQuat &DualQuat = boneMatrix[i];
		QuaternionSlerp(bone1.Rotation, bone2.Rotation, s1, DualQuat.Rotation);
		if constexpr(isLockPosition == true)
		{
			if (header.LockPositions)
			{
				EDualQuat &boneData = action.FrameData[0].BoneData[0];
				DualQuat.Position[0] = boneData.Position[0];
				DualQuat.Position[1] = boneData.Position[1];
				DualQuat.Position[2] = p1[2] * s2 + p2[2] * s1 + BodyHeight(threadIndex);
			}
			else
			{
				DualQuat.Position = p1 * s2 + p2 * s1;
			}
		}
		else
		{
			DualQuat.Position = p1 * s2 + p2 * s1;
		}

		if constexpr(isParentMatrix)
		{
			DualQuat.Mix(threadInfo.ParentMatrix);
		}
		else
		{
			DualQuat.Mix(boneMatrix[parentBone]);
		}
#else
		EMatrix4 &Matrix = BoneMatrix[i];
		EQuat q;
		QuaternionSlerp(q1, q2, s1, q);
		if constexpr(isLockPosition == true)
		{
			if (header.LockPositions)
			{
				EDualQuat &boneData = action.FrameData[0].BoneData[0];
				FastMakeMatrix(Matrix, q, EVector3(boneData.Position[0], boneData.Position[1], p1[2] * s2 + p2[2] * s1 + BodyHeight(threadIndex)));
			}
			else
			{
				FastMakeMatrix(Matrix, q, p1 * s2 + p2 * s1);
			}
		}
		else
		{
			FastMakeMatrix(Matrix, q, p1 * s2 + p2 * s1);
		}

		if constexpr(isParentMatrix)
		{
			Matrix = ThreadInfo.ParentMatrix * Matrix;
		}
		else
		{
			Matrix = boneMatrix[parentBone] * Matrix;
		}
#endif
	}

public:
	FORCEINLINE const mu_boolean IsHeaderLoaded() const
	{
		return _LoadFlags.Get(ELoadFlags::ELoadFlag_Header);
	}

	FORCEINLINE const mu_boolean IsDataLoaded() const
	{
		return _LoadFlags.Get(ELoadFlags::ELoadFlag_Data);
	}

	FORCEINLINE const mu_boolean IsGPULoaded() const
	{
		return _LoadFlags.Get(ELoadFlags::ELoadFlag_GPU);
	}

	FORCEINLINE const mu_boolean IsGPUAvailable() const
	{
		return _LoadFlags.Get(ELoadFlags::ELoadFlag_AvailableGPU);
	}

	FORCEINLINE const mu_boolean IsPendingTransferGPU() const
	{
		return _LoadFlags.Get(ELoadFlags::ELoadFlag_PendingGPU);
	}

	FORCEINLINE const mu_boolean IsRequestedLoad() const
	{
		return _IsRequested.load();
	}

	FORCEINLINE const mu_int32 GetBoneIndex(const mu_uint32 index) const
	{
		if(index >= _BoneCount) return -1;

		return _BonesIndex[index];
	}

public:
	const mu_int32 _ModelIndex;

private:
	enum ELoadFlags
	{
		ELoadFlag_Header = 0,
		ELoadFlag_Data,
		ELoadFlag_GPU,
		ELoadFlag_PendingGPU,
		ELoadFlag_AvailableGPU,
		ELoadFlag_Max,
	};
	MUBitset<ELoadFlag_Max> _LoadFlags;

public:
	mu_atomic_bool _IsRequested;

public:
	// Headers Data
	Model::VertexHeader1 _VertexHeader;
	Model::AnimationHeader1 *_AnimationHeader;

	// Body Data
	mu_int32 _BoneHead;
	mu_uint32 _MeshCount;
	mu_uint32 _AnimationCount;
	mu_uint32 _BoneCount;
	mu_uint32 _VertexCount;
	mu_uint32 _IndexCount;
	mu_uint32 _BoneParentEnd;
	mu_uint32 _BoneNonDummyEnd;
	
	mu_uint32 *_MeshTexture;

	Model::Mesh1 *_Mesh;
	Model::MeshData1 *_MeshData;
	Model::MeshExtra1 *_MeshExtra;
	Model::AnimationData1 *_AnimationData;
	Model::BoneRef1 *_BoneRef;
	mu_int16 *_BonesIndex;
	mu_int16 *_ReverseBonesIndex;

	// Sounds Data
	mu_uint16 _Sounds[MAX_MODEL_SOUND];
	EMathSphere _BoundingSphere;
	EMathAABB _BoundingBox;

public:
#define THREAD_VARIABLE(type, name) type _##name [MAX_TASKING_THREADS]; \
									EINLINE type &name (mu_uint32 i) { return _##name[i]; }

	THREAD_VARIABLE(mu_boolean, LightEnable);
	THREAD_VARIABLE(mu_boolean, HideSkin);

	THREAD_VARIABLE(mu_int8, Skin);

	THREAD_VARIABLE(mu_int16, StreamMesh);
	THREAD_VARIABLE(mu_uint16, CurrentAction);
	THREAD_VARIABLE(mu_int16, CurrentAnimationFrame);

	THREAD_VARIABLE(EVector3, BodyLight);
	THREAD_VARIABLE(EVector3, BodyOrigin);
	THREAD_VARIABLE(EVector3, BodyAngle);
	THREAD_VARIABLE(mu_float, BodyScale);
	THREAD_VARIABLE(mu_float, BodyHeight);
	THREAD_VARIABLE(mu_float, CurrentAnimation);

	THREAD_VARIABLE(mu_int32, ClothMesh);
	THREAD_VARIABLE(mu_int32, ClothVerticesIndex);
	THREAD_VARIABLE(mu_int32, ClothVerticesCount);

#undef THREAD_VARIABLE
};

#endif