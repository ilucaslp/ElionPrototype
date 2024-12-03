#include "stdafx.h"
#include "mu_model.h"

const mu_uint8 MUModel::CryptKey[] = { 0x2b, 0x57, 0x42, 0xa8, 0xeb, 0x88, 0xaa, 0xc2, 0x09, 0xef, 0x77, 0xb7, 0x77, 0x39, 0x93, 0xf4, 0xa9, 0xcf, 0x00, 0x24, 0x11, 0x25, 0x01, 0x95, 0x18, 0x96, 0x8b, 0x1d, 0xa1, 0x0d, 0xbc, 0x3c, 0x8d, 0x59, 0x99, 0x36, 0x7c, 0x68, 0x0a, 0x3b, 0x0f, 0xad, 0x02, 0x2e, 0x88, 0x54, 0x5f, 0xa5, 0x0a, 0x62, 0x97, 0xd3, 0x06, 0xcb, 0x86, 0x67 };
MUBlowfish MUModel::Crypt(CryptKey, mu_countof(CryptKey));

#include <glm/gtx/matrix_decompose.hpp>

MUModel::MUModel() : EModelBackend(), _ModelIndex(((mu_size)this - (mu_size)MODELS::Data) / sizeof(MUModel))
{
	_LoadFlags.Reset();

	mu_memset(_Sounds, 0xFF, sizeof(_Sounds));

	_MeshCount = 0;
	_AnimationCount = 0;
	_BoneCount = 0;
	_VertexCount = 0;
	_IndexCount = 0;

	_Mesh = nullptr;
	_MeshData = nullptr;
	_MeshExtra = nullptr;
	_MeshTexture = nullptr;
	_AnimationHeader = nullptr;
	_BoneRef = nullptr;
	_BonesIndex = nullptr;

	for (mu_uint32 i = 0; i < MAX_TASKING_THREADS; ++i)
	{
		_LightEnable[i] = false;
		_HideSkin[i] = false;

		_StreamMesh[i] = -1;

		_ClothMesh[i] = -1;
	}
}

MUModel::~MUModel()
{
	Release();
}

void MUModel::Release()
{
	ReleaseHeader();
	ReleaseData();
	ReleaseGPU();
}

void MUModel::ReleaseHeader()
{
	_LoadFlags.Unset(ELoadFlags::ELoadFlag_Header);

	_AnimationCount = 0;
	_BoneCount = 0;
	_BoneHead = -1;

	_BoneParentEnd = 0;
	_BoneNonDummyEnd = 0;
	MU_SAFE_DELETE_ARRAY(_AnimationHeader);
	MU_SAFE_DELETE_ARRAY(_BoneRef);
	MU_SAFE_DELETE_ARRAY(_BonesIndex);
	MU_SAFE_DELETE_ARRAY(_ReverseBonesIndex);
}

void MUModel::ReleaseData()
{
	_LoadFlags.Unset(ELoadFlags::ELoadFlag_Data);

	MU_SAFE_DELETE_ARRAY(_Mesh);
	MU_SAFE_DELETE_ARRAY(_MeshData);
	MU_SAFE_DELETE_ARRAY(_MeshExtra);
	MU_SAFE_DELETE_ARRAY(_MeshTexture);
	MU_SAFE_DELETE_ARRAY(_AnimationData);

	_MeshCount = 0;
	_VertexCount = 0;
	_IndexCount = 0;

	for (mu_uint32 i = 0; i < MAX_TASKING_THREADS; ++i)
	{
		_LightEnable[i] = false;
		_HideSkin[i] = false;

		_StreamMesh[i] = -1;
	}
}

void MUModel::ReleaseGPU(const mu_boolean dataOnly)
{
	_LoadFlags.Unset(ELoadFlags::ELoadFlag_GPU);
	_LoadFlags.Unset(ELoadFlags::ELoadFlag_PendingGPU);
	_LoadFlags.Unset(ELoadFlags::ELoadFlag_AvailableGPU);

	if (dataOnly == true)
	{
		for (mu_uint32 n = 0; n < _MeshCount; ++n)
		{
			mu_uint32 textureIndex = _MeshTexture[n];

			if (textureIndex >= BITMAP_NONAMED_TEXTURES_BEGIN &&
				textureIndex <= BITMAP_NONAMED_TEXTURES_END)
			{
				TEXTURES::Data[textureIndex].Release();
			}
		}
	}
	else
	{
		for (mu_uint32 n = 0; n < _MeshCount; ++n)
		{
			mu_uint32 textureIndex = _MeshTexture[n];

			if (textureIndex >= BITMAP_NONAMED_TEXTURES_BEGIN &&
				textureIndex <= BITMAP_NONAMED_TEXTURES_END)
			{
				MU_RESOURCE->UnloadTexture2D(textureIndex);
			}
		}

		mu_zeromem(_MeshTexture, sizeof(mu_uint32) * _MeshCount);
	}

	ReleaseModel();
}

void MUModel::TryRealtimeLoad()
{
	if (_ModelIndex < MAX_WORLD_OBJECTS)
	{
		return;
	}

	if (_IsRequested.exchange(true) == true)
	{
		return;
	}

	MULoadingData *data = MODELS::LoadData[_ModelIndex];
	if(data == nullptr)
	{
		return;
	}

	LoadHeaderFromFileEx(data->Path);
	LoadFromFileEx(data->Path);
}

void MUModel::RequestLoad()
{
	if(_ModelIndex < MAX_WORLD_OBJECTS)
	{
		return;
	}

	if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false)
	{
		return;
	}

	if(_LoadFlags.Get(ELoadFlags::ELoadFlag_AvailableGPU) == true ||
		_IsRequested.exchange(true) == true)
	{
		return;
	}

	MULoadingData *data = MODELS::LoadData[_ModelIndex];
	if (data == nullptr)
	{
		return;
	}

	g_LoadingManager.AddRuntime(data);
}

const mu_boolean MUModel::LoadHeaderFromFileEx(const mu_text *Path)
{
	if (_LoadFlags.Get(ELoadFlags::ELoadFlag_Header) == true)
	{
		return true;
	}

	mu_string ModelHeaderPath = mu_string(Path) + _T(".emhd");

	if (mu_rwexists(ModelHeaderPath.c_str()) == false)
	{
		return false;
	}

	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, ModelHeaderPath.c_str(), "rb") == false)
	{
		return false;
	}

	static const mu_size HeaderSize = FileHeaders::HEADEREX_SIZE;
	static const mu_size CompressedHeaderSize = FileHeaders::COMPRESSED_HEADER_SIZE;

	mu_int64 fileLength = SDL_RWsize(file);
	mu_int64 encryptedLength = fileLength - HeaderSize;

	if (encryptedLength % MUBlowfish::BytesLength != 0)
	{
		return false;
	}

	mu_uint8 *dataBuffer = new mu_uint8[fileLength];
	SDL_RWread(file, dataBuffer, fileLength, 1);
	SDL_RWclose(file);

	FileHeaders::FileHeaderEx *header = (FileHeaders::FileHeaderEx*)dataBuffer;

	if (header->Signature != FILE_HEADERS::MODELHEADER_HEADER)
	{
		delete[] dataBuffer;
		return false;
	}

	if (GenerateCRC32(dataBuffer + HeaderSize, encryptedLength) != header->CRC)
	{
		delete[] dataBuffer;
		return false;
	}

	Crypt.Decode(dataBuffer + HeaderSize, dataBuffer + HeaderSize, encryptedLength);

	FileHeaders::CompressedHeader *compressedHeader = (FileHeaders::CompressedHeader*)(dataBuffer + HeaderSize);

	mu_uint8 *decompressedBuffer = new mu_uint8[compressedHeader->DecompressedSize];

	mu_boolean decompressed = FileHeaders::Decompress(dataBuffer + HeaderSize + CompressedHeaderSize,
		decompressedBuffer,
		compressedHeader->CompressedSize,
		compressedHeader->DecompressedSize);
	delete[] dataBuffer;

	if (decompressed == false)
	{
		delete[] decompressedBuffer;
		return false;
	}

	mu_uint8 *readBuffer = decompressedBuffer;
	{
		FileHeaders::ReadBuffer(readBuffer, _VertexHeader.IndexType);
		FileHeaders::ReadBuffer(readBuffer, _VertexHeader.HasPhysics);
		FileHeaders::ReadBuffer(readBuffer, _VertexHeader.StaticMesh);

		FileHeaders::ReadBuffer(readBuffer, _AnimationCount);
		FileHeaders::ReadBuffer(readBuffer, _BoneCount);
		FileHeaders::ReadBuffer(readBuffer, _BoneHead);
		if (_AnimationCount > 0)
		{
			_AnimationHeader = new Model::AnimationHeader1[_AnimationCount];

			for (mu_uint32 animIndex = 0; animIndex < _AnimationCount; ++animIndex)
			{
				Model::AnimationHeader1 &animationHeader = _AnimationHeader[animIndex];

				FileHeaders::ReadBuffer(readBuffer, animationHeader.Repeat);
				FileHeaders::ReadBuffer(readBuffer, animationHeader.LockPositions);
				FileHeaders::ReadBuffer(readBuffer, animationHeader.Speed);

				FileHeaders::ReadBuffer(readBuffer, animationHeader.FrameCount);
			}
		}

		if (_BoneCount > 0)
		{
			FileHeaders::ReadBuffer(readBuffer, _BoneParentEnd);
			FileHeaders::ReadBuffer(readBuffer, _BoneNonDummyEnd);
			_BoneRef = new Model::BoneRef1[_BoneCount];
			FileHeaders::ReadBuffer(readBuffer, _BoneRef, sizeof(Model::BoneRef1) * _BoneCount);
			_BonesIndex = new mu_int16[_BoneCount];
			FileHeaders::ReadBuffer(readBuffer, _BonesIndex, sizeof(mu_int16) * _BoneCount);
			_ReverseBonesIndex = new mu_int16[_BoneCount];
			FileHeaders::ReadBuffer(readBuffer, _ReverseBonesIndex, sizeof(mu_int16) * _BoneCount);
		}
	}

	delete[] decompressedBuffer;

	_LoadFlags.Set(ELoadFlags::ELoadFlag_Header);

	return true;
}

const mu_boolean MUModel::LoadDataFromFile(const mu_text *Path)
{
	if (_LoadFlags.Get(ELoadFlags::ELoadFlag_Data) == true)
	{
		return true;
	}

	mu_string ModelPath = mu_string(Path) + _T(".emdl");
	mu_string AnimationPath = mu_string(Path) + _T(".eani");

	const mu_boolean hasModel = mu_rwexists(ModelPath.c_str());
	const mu_boolean hasAnimation = mu_rwexists(AnimationPath.c_str());

	if (hasModel == false &&
		hasAnimation == false)
	{
		return false;
	}

	if (hasModel == true)
	{
		SDL_RWops *file = nullptr;

		if (mu_rwfromfile(&file, ModelPath.c_str(), "rb") == false)
		{
			return false;
		}

		static const mu_size HeaderSize = FileHeaders::HEADEREX_SIZE;
		static const mu_size CompressedHeaderSize = FileHeaders::COMPRESSED_HEADER_SIZE;

		mu_int64 fileLength = SDL_RWsize(file);
		mu_int64 encryptedLength = fileLength - HeaderSize;

		if (encryptedLength % MUBlowfish::BytesLength != 0)
		{
			return false;
		}

		mu_uint8 *dataBuffer = new mu_uint8[fileLength];
		SDL_RWread(file, dataBuffer, fileLength, 1);
		SDL_RWclose(file);

		FileHeaders::FileHeaderEx *header = (FileHeaders::FileHeaderEx*)dataBuffer;

		if (header->Signature != FILE_HEADERS::MODEL_HEADER)
		{
			delete[] dataBuffer;
			return false;
		}

		if (GenerateCRC32(dataBuffer + HeaderSize, encryptedLength) != header->CRC)
		{
			delete[] dataBuffer;
			return false;
		}

		Crypt.Decode(dataBuffer + HeaderSize, dataBuffer + HeaderSize, encryptedLength);

		FileHeaders::CompressedHeader *compressedHeader = (FileHeaders::CompressedHeader*)(dataBuffer + HeaderSize);

		mu_uint8 *decompressedBuffer = new mu_uint8[compressedHeader->DecompressedSize];

		mu_boolean decompressed = FileHeaders::Decompress(dataBuffer + HeaderSize + CompressedHeaderSize,
														  decompressedBuffer,
														  compressedHeader->CompressedSize,
														  compressedHeader->DecompressedSize);
		delete[] dataBuffer;

		if (decompressed == false)
		{
			delete[] decompressedBuffer;
			return false;
		}

		mu_uint8 *readBuffer = decompressedBuffer;

		FileHeaders::ReadBuffer(readBuffer, _MeshCount);
		if (_MeshCount > 0)
		{
			_MeshData = new Model::MeshData1[_MeshCount];
			FileHeaders::ReadBuffer(readBuffer, _MeshData, sizeof(Model::MeshData1) * _MeshCount);
			_MeshExtra = new Model::MeshExtra1[_MeshCount];
			FileHeaders::ReadBuffer(readBuffer, _MeshExtra, sizeof(Model::MeshExtra1) * _MeshCount);
			_Mesh = new Model::Mesh1[_MeshCount];

			for (mu_uint32 n = 0; n < _MeshCount; ++n)
			{
				Model::Mesh1 &mesh = _Mesh[n];

				FileHeaders::ReadBuffer(readBuffer, mesh.NumVertices);
				FileHeaders::ReadBuffer(readBuffer, mesh.NumNormals);
				FileHeaders::ReadBuffer(readBuffer, mesh.NumTexCoords);
				FileHeaders::ReadBuffer(readBuffer, mesh.NumTriangles);

				mesh.Vertices = new Model::MeshVertex1[mesh.NumVertices];
				mesh.Normals = new Model::MeshNormal1[mesh.NumNormals];
				mesh.TexCoords = new EVector2[mesh.NumTexCoords];
				mesh.Triangles = new Model::MeshTriangle1[mesh.NumTriangles];

				FileHeaders::ReadBuffer(readBuffer, mesh.Vertices, sizeof(Model::MeshVertex1) * mesh.NumVertices);
				FileHeaders::ReadBuffer(readBuffer, mesh.Normals, sizeof(Model::MeshNormal1) * mesh.NumNormals);
				FileHeaders::ReadBuffer(readBuffer, mesh.TexCoords, sizeof(EVector2) * mesh.NumTexCoords);
				FileHeaders::ReadBuffer(readBuffer, mesh.Triangles, sizeof(Model::MeshTriangle1) * mesh.NumTriangles);
			}

			_MeshTexture = new mu_uint32[_MeshCount];
			mu_memset(_MeshTexture, EInvalidUInt32, sizeof(mu_uint32) * _MeshCount);
		}

		Model::BoundingSphere1 boundingSphere;
		FileHeaders::ReadBuffer(readBuffer, boundingSphere);
		_BoundingSphere._Center = boundingSphere.Center;
		_BoundingSphere._Radius = boundingSphere.Radius;

		Model::BoundingBox1 boundingBox;
		FileHeaders::ReadBuffer(readBuffer, boundingBox);
		_BoundingBox.CreateFromPoints(boundingBox.Min, boundingBox.Max);

		delete[] decompressedBuffer;

		switch (_ModelIndex)
		{
		case MODEL_PANTS + 18:
			ConfigureCloth(2, 5, 8);
			break;
		case MODEL_PANTS + 22:
			ConfigureCloth(2, 7, 5);
			break;
		}
	}

	if (hasAnimation == true)
	{
		SDL_RWops *file = nullptr;

		if (mu_rwfromfile(&file, AnimationPath.c_str(), "rb") == false)
		{
			return false;
		}

		static const mu_size HeaderSize = FileHeaders::HEADEREX_SIZE;
		static const mu_size CompressedHeaderSize = FileHeaders::COMPRESSED_HEADER_SIZE;

		mu_int64 fileLength = SDL_RWsize(file);
		mu_int64 encryptedLength = fileLength - HeaderSize;

		if (encryptedLength % MUBlowfish::BytesLength != 0)
		{
			return false;
		}

		mu_uint8 *dataBuffer = new mu_uint8[fileLength];
		SDL_RWread(file, dataBuffer, fileLength, 1);
		SDL_RWclose(file);

		FileHeaders::FileHeaderEx *header = (FileHeaders::FileHeaderEx*)dataBuffer;

		if (header->Signature != FILE_HEADERS::ANIMATION_HEADER)
		{
			delete[] dataBuffer;
			return false;
		}

		if (GenerateCRC32(dataBuffer + HeaderSize, encryptedLength) != header->CRC)
		{
			delete[] dataBuffer;
			return false;
		}

		Crypt.Decode(dataBuffer + HeaderSize, dataBuffer + HeaderSize, encryptedLength);

		FileHeaders::CompressedHeader *compressedHeader = (FileHeaders::CompressedHeader*)(dataBuffer + HeaderSize);

		mu_uint8 *decompressedBuffer = new mu_uint8[compressedHeader->DecompressedSize];

		mu_boolean decompressed = FileHeaders::Decompress(dataBuffer + HeaderSize + CompressedHeaderSize,
														  decompressedBuffer,
														  compressedHeader->CompressedSize,
														  compressedHeader->DecompressedSize);
		delete[] dataBuffer;

		if (decompressed == false)
		{
			delete[] decompressedBuffer;
			return false;
		}

		mu_uint8 *readBuffer = decompressedBuffer;

		if (_AnimationCount > 0)
		{
			_AnimationData = new Model::AnimationData1[_AnimationCount];

			for (mu_uint32 animIndex = 0; animIndex < _AnimationCount; ++animIndex)
			{
				const Model::AnimationHeader1 &animationHeader = _AnimationHeader[animIndex];
				Model::AnimationData1 &animationData = _AnimationData[animIndex];

				animationData.FrameData = new Model::FrameData1[animationHeader.FrameCount];

				for (mu_uint32 frameIndex = 0; frameIndex < animationHeader.FrameCount; ++frameIndex)
				{
					Model::FrameData1 &frameData = animationData.FrameData[frameIndex];
#ifdef USE_DUAL_QUATERNION
					MU_ALIGNED_ALLOC(frameData.BoneData, EDualQuat, _BoneCount, 16);
					FileHeaders::ReadBuffer(readBuffer, frameData.BoneData, sizeof(EDualQuat) * _BoneCount);
#else
					MU_ALIGNED_ALLOC(frameData.BoneData, EMatrix4, _BoneCount, 16);
					FileHeaders::ReadBuffer(readBuffer, frameData.BoneData, sizeof(EMatrix4) * _BoneCount);
#endif
				}
			}
		}

		delete[] decompressedBuffer;
	}

	_LoadFlags.Set(ELoadFlags::ELoadFlag_Data);

	switch (_ModelIndex)
	{
	case MODEL_MONSTER01 + 27:
		{
			for (mu_uint32 i = 0; i < MAX_TASKING_THREADS; ++i)
			{
				this->_StreamMesh[i] = 1;
			}
		}
		break;

	case MODEL_MONSTER01 + 54:
		{
			if (this->_MeshCount > 0)
			{
				this->_MeshExtra[0].IsUsed = true;
				this->_MeshExtra[0].IsNoneBlend = true;
				this->_MeshExtra[1].IsNoneBlend = false;
				this->_MeshExtra[2].IsNoneBlend = false;
				this->_MeshExtra[3].IsUsed = true;
				this->_MeshExtra[3].IsNoneBlend = true;
				this->_MeshExtra[4].IsUsed = true;
				this->_MeshExtra[4].IsNoneBlend = true;
			}
		}
		break;

	case MODEL_MONSTER01 + 55:
		{
			for (mu_uint32 i = 0; i < MAX_TASKING_THREADS; ++i)
			{
				this->_StreamMesh[i] = 0;
			}
		}
		break;

	case MODEL_SPEAR:
		{
			this->_MeshExtra[1].IsUsed = true;
			this->_MeshExtra[1].IsNoneBlend = true;
		}
		break;

	case MODEL_SWORD + 10:
		{
			this->_MeshExtra[1].IsUsed = true;
			this->_MeshExtra[1].IsNoneBlend = true;
		}
		break;

	case MODEL_STAFF + 6:
		{
			this->_MeshExtra[2].IsUsed = true;
			this->_MeshExtra[2].IsNoneBlend = true;
		}
		break;

	case MODEL_MACE + 6:
		{
			this->_MeshExtra[1].IsUsed = true;
			this->_MeshExtra[1].IsNoneBlend = true;
		}
		break;

	case MODEL_EVENT + 9:
		{
			this->_MeshExtra[1].IsUsed = true;
			this->_MeshExtra[1].IsNoneBlend = true;
		}
		break;
	}

	return true;
}

const mu_boolean MUModel::LoadGPUFromFile(const mu_text *Path)
{
	if (_LoadFlags.Get(ELoadFlags::ELoadFlag_GPU) == true)
	{
		return true;
	}

	if (_LoadFlags.Get(ELoadFlags::ELoadFlag_Data) == false)
	{
		return false;
	}

	mu_string DataPath = mu_string(Path) + _T(".egpu");
	if (mu_rwexists(DataPath.c_str()) == false)
	{
		return true;
	}

	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, DataPath.c_str(), "rb") == false)
	{
		return false;
	}

	static const mu_size HeaderSize = FileHeaders::HEADEREX_SIZE;
	static const mu_size CompressedHeaderSize = FileHeaders::COMPRESSED_HEADER_SIZE;

	mu_int64 fileLength = SDL_RWsize(file);
	mu_int64 encryptedLength = fileLength - HeaderSize;

	if (encryptedLength % MUBlowfish::BytesLength != 0)
	{
		return false;
	}

	mu_uint8 *dataBuffer = new mu_uint8[fileLength];
	SDL_RWread(file, dataBuffer, fileLength, 1);
	SDL_RWclose(file);

	FileHeaders::FileHeaderEx *header = (FileHeaders::FileHeaderEx*)dataBuffer;

	if (header->Signature != FILE_HEADERS::MODEL_HEADER)
	{
		delete[] dataBuffer;
		return false;
	}

	if (GenerateCRC32(dataBuffer + HeaderSize, encryptedLength) != header->CRC)
	{
		delete[] dataBuffer;
		return false;
	}

	Crypt.Decode(dataBuffer + HeaderSize, dataBuffer + HeaderSize, encryptedLength);

	FileHeaders::CompressedHeader *compressedHeader = (FileHeaders::CompressedHeader*)(dataBuffer + HeaderSize);

	mu_uint8 *decompressedBuffer = new mu_uint8[compressedHeader->DecompressedSize];

	mu_boolean decompressed = FileHeaders::Decompress(dataBuffer + HeaderSize + CompressedHeaderSize,
													  decompressedBuffer,
													  compressedHeader->CompressedSize,
													  compressedHeader->DecompressedSize);
	delete[] dataBuffer;

	if (decompressed == false)
	{
		delete[] decompressedBuffer;
		return false;
	}

	mu_uint8 *readBuffer = decompressedBuffer;

	MULoadingGPUData gpuData = {};
	mu_uint32 dataCount = 0;
	FileHeaders::ReadBuffer(readBuffer, dataCount);
	Model::VertexData1 *VertexData = (Model::VertexData1*)mu_malloc(sizeof(Model::VertexData1) * dataCount);
	FileHeaders::ReadBuffer(readBuffer, VertexData, sizeof(Model::VertexData1) * dataCount);

	_VertexCount = dataCount;

	if (_VertexHeader.IndexType == INDEX_16BIT)
	{
		dataCount = 0;
		FileHeaders::ReadBuffer(readBuffer, dataCount);
		mu_uint16 *IndexData16 = (mu_uint16*)mu_malloc(sizeof(mu_uint16) * dataCount);
		FileHeaders::ReadBuffer(readBuffer, IndexData16, sizeof(mu_uint16) * dataCount);

		_IndexCount = dataCount;

		gpuData.DataType = GPU_DATATYPE::GPUDT_MODEL;
		gpuData.Object = this;
		gpuData.Data[0] = reinterpret_cast<mu_uint8*>(VertexData);
		gpuData.DataSize[0] = _VertexCount;
		gpuData.Data[1] = reinterpret_cast<mu_uint8*>(IndexData16);
		gpuData.DataSize[1] = _IndexCount;
	}
	else
	{
		dataCount = 0;
		FileHeaders::ReadBuffer(readBuffer, dataCount);
		mu_uint32 *IndexData32 = (mu_uint32*)mu_malloc(sizeof(mu_uint32) * dataCount);
		FileHeaders::ReadBuffer(readBuffer, IndexData32, sizeof(mu_uint32) * dataCount);

		_IndexCount = dataCount;

		gpuData.DataType = GPU_DATATYPE::GPUDT_MODEL;
		gpuData.Object = this;
		gpuData.Data[0] = reinterpret_cast<mu_uint8*>(VertexData);
		gpuData.DataSize[0] = _VertexCount;
		gpuData.Data[1] = reinterpret_cast<mu_uint8*>(IndexData32);
		gpuData.DataSize[1] = _IndexCount;
	}

	mu_uint32 pathSize = 0, filenameSize = 0;
	mu_asciistr path;
	mu_char filename[512];
	FileHeaders::ReadBuffer(readBuffer, pathSize);
	if (pathSize > 0)
	{
		path.resize(pathSize);
		FileHeaders::ReadBuffer(readBuffer, (mu_char*)path.c_str(), pathSize);
	}

	for (mu_uint32 n = 0; n < _MeshCount; ++n)
	{
		FileHeaders::ReadBuffer(readBuffer, filenameSize);
		if (filenameSize > 0)
		{
			FileHeaders::ReadBuffer(readBuffer, filename, filenameSize);
			filename[filenameSize] = _T('\0');
		}

#ifdef UNICODE
		mu_string filepath = mu_string(L"./data/") + boost::locale::conv::utf_to_utf<mu_text>(path + filename);
#else
		mu_string filepath = mu_string(L"./data/") + path + filename;
#endif

		if (mu_utf8ncmp(filename, "SKI", 3) == 0)
			_MeshTexture[n] = BITMAP_SKIN;
		else if (mu_utf8ncmp(filename, "LEVEL", 5) == 0)
			_MeshTexture[n] = BITMAP_SKIN;
		else if (mu_utf8ncmp(filename, "HID", 3) == 0)
			_MeshTexture[n] = BITMAP_HIDE;
		else if (mu_utf8ncmp(filename, "HAIR", 4) == 0)
			_MeshTexture[n] = BITMAP_HAIR;
		else
		{
			const mu_uint32 textureIndex = _MeshTexture[n];
			if (textureIndex != EInvalidUInt32)
			{
				std::transform(filepath.begin(), filepath.end(), filepath.begin(), mu_tolower);
				TEXTURES::Data[textureIndex].LoadHeaderFromFile(filepath.c_str());
				TEXTURES::Data[textureIndex].LoadFromFileEx(filepath.c_str());
			}
			else
			{
				_MeshTexture[n] = MU_RESOURCE->LoadTexture2D(filepath);
			}
		}
	}

	delete[] decompressedBuffer;

	_LoadFlags.Set(ELoadFlags::ELoadFlag_GPU);
	_LoadFlags.Set(ELoadFlags::ELoadFlag_PendingGPU);

	// Critical Area
	{
		MURWLockShared lock(g_GPUDataLock);
		mu_uint32 gpuQueueIndex = g_GPUQueueIndex;
		mu_uint32 gpuDataIndex = g_GPUDataCount[gpuQueueIndex]++;
		if (gpuDataIndex >= GPUDATA_SIZE)
		{
			DEBUG_BREAK();
		}

		mu_memcpy(&g_GPUData[gpuQueueIndex][gpuDataIndex], &gpuData, sizeof(gpuData));
	}

	return true;
}

const mu_boolean MUModel::LoadFromFileEx(const mu_text *Path)
{
	if (LoadDataFromFile(Path) == false)
	{
		ReleaseData();
		return false;
	}

	if (LoadGPUFromFile(Path) == false)
	{
		ReleaseData();
		ReleaseGPU();
		return false;
	}

	return true;
}

const mu_boolean MUModel::TransferCPUtoGPU(const Model::VertexData1 *vertexData, const mu_uint32 vertexCount, const void *indexData, const mu_uint32 indexCount)
{
	const mu_size vertexSize = vertexCount * sizeof(Model::VertexData1);
	const mu_size indexSize = _VertexHeader.IndexType == INDEX_16BIT ? indexCount * sizeof(mu_uint16) : indexCount * sizeof(mu_uint32);

	if(ModelInternal.VertexBuffer.Allocate(vertexSize, EBufferUsage::eStatic, EBufferType::eVertex, vertexData) == false)
	{
		return false;
	}

	if (ModelInternal.IndexBuffer.Allocate(indexSize, EBufferUsage::eStatic, EBufferType::eIndex, indexData) == false)
	{
		return false;
	}

	switch(EVideoBackend::GetBackend())
	{
#if OPENGL_BACKEND_ENABLE == 1
	case EVideoBackendType::eOpenGL:
		{
			EOpenGLHelpers::GenerateVAOs(1, &OpenGLInternal.VAO);
			EOpenGLPipeline::BindVAO(OpenGLInternal.VAO);
			glBindBuffer(GL_ARRAY_BUFFER, ModelInternal.VertexBuffer.OpenGLInternal.Resource);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ModelInternal.IndexBuffer.OpenGLInternal.Resource);
			EVideoAttributes::ConfigureAtLoad(EInputLayout::eModel);
			EOpenGLPipeline::BindVAO(EInvalidUInt32);
		}
		break;
#endif

	default: break;
	}

	_LoadFlags.Unset(ELoadFlags::ELoadFlag_PendingGPU);
	_LoadFlags.Set(ELoadFlags::ELoadFlag_AvailableGPU);

	return true;
}

void MUModel::ConfigureCloth(mu_uint32 MeshIndex, mu_int32 NumHor, mu_int32 NumVer)
{
	Model::Mesh1 *pMesh = &_Mesh[MeshIndex];

	for (mu_int32 j = 0; j < pMesh->NumTriangles; ++j)
	{
		mu_int32 iRight = j % 2;
		mu_int32 xQuad = (j / 2) % (NumHor - 1);
		mu_int32 yQuad = (j / 2) / (NumHor - 1);

		Model::MeshTriangle1 *tp = &pMesh->Triangles[j];
		mu_int32 x;
		mu_int32 y;
		mu_int32 xDim[6] = { 0, 1, 0, 1, 1, 0 };
		mu_int32 yDim[6] = { 0, 0, 1, 0, 1, 1 };
		for (mu_int32 i = 0; i < 3; ++i)
		{
			x = xDim[iRight * 3 + i] + xQuad;
			y = yDim[iRight * 3 + i] + yQuad;

			tp->VertexIndex[i] = y * NumHor + x;
			tp->NormalIndex[i] = (x * pMesh->NumNormals / NumHor + y * pMesh->NumNormals / NumVer) / 3;
			tp->TexCoordIndex[i] = tp->VertexIndex[i];

			pMesh->TexCoords[tp->TexCoordIndex[i]] = EVector2((mu_float)x / (mu_float)(NumHor - 1), (mu_float)y / (mu_float)(NumVer - 1));
		}
	}
}

mu_boolean MUModel::PlayAnimation(const mu_uint32 ThreadIndex, mu_float *AnimationFrame, mu_float *PriorAnimationFrame, mu_uint16 *PriorAction, mu_float Speed)
{
	if(IsDataLoaded() == false)
	{
		RequestLoad();
	}

	mu_boolean Loop = true;

	if (AnimationFrame == nullptr || PriorAnimationFrame == nullptr || PriorAction == nullptr || _AnimationCount == 0 || CurrentAction(ThreadIndex) >= _AnimationCount)
	{
		return Loop;
	}

	const Model::AnimationHeader1 &currentHeader = _AnimationHeader[CurrentAction(ThreadIndex)];

	if (currentHeader.FrameCount <= 1)
	{
		return Loop;
	}

	mu_int32 Temp = (mu_int32)*AnimationFrame;
	*AnimationFrame += Speed;
	if (Temp != (mu_int32)*AnimationFrame)
	{
		*PriorAction = CurrentAction(ThreadIndex);
		*PriorAnimationFrame = static_cast<mu_float>(Temp);
	}

	if (*AnimationFrame <= 0.0f)
	{
		*AnimationFrame += static_cast<mu_float>(currentHeader.FrameCount) - 1.0f;
	}

	if (currentHeader.Repeat)
	{
		if (*AnimationFrame >= static_cast<mu_float>(currentHeader.FrameCount))
		{
			*AnimationFrame = static_cast<mu_float>(currentHeader.FrameCount) - 0.01f;
			Loop = false;
		}
	}
	else
	{
		mu_uint32 Key;
		if (currentHeader.LockPositions)
			Key = currentHeader.FrameCount - 1;
		else
			Key = currentHeader.FrameCount;

		mu_float fTemp;

		if (g_CurrentScene == CHARACTER_SCENE)
		{
			fTemp = *AnimationFrame + 2;
		}
		else if (GET_WORLD == WD_39KANTURU_3RD && CurrentAction(ThreadIndex) == MONSTER01_APEAR)
		{
			fTemp = *AnimationFrame + 1;
		}
		else
		{
			fTemp = *AnimationFrame;
		}

		if (fTemp >= static_cast<mu_float>(Key))
		{
			mu_uint32 Frame = (mu_int32)*AnimationFrame;
			*AnimationFrame = static_cast<mu_float>(Frame % (Key)) + (*AnimationFrame - static_cast<mu_float>(Frame));
			Loop = false;
		}
	}

	CurrentAnimation(ThreadIndex) = *AnimationFrame;
	CurrentAnimationFrame(ThreadIndex) = (mu_int32)CurrentAnimation(ThreadIndex);

	return Loop;
}

void MUModel::Animation(const mu_uint32 ThreadIndex, EDualQuat *BoneMatrix, mu_float AnimationFrame, mu_float PriorFrame, mu_uint16 PriorAction, EVector3 Angle, EVector3 HeadAngle, mu_boolean Parent, mu_boolean Translate)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if(IsDataLoaded() == false)
	{
		RequestLoad();
		return;
	}

	if (_AnimationCount <= 0 || _BoneCount <= 0) return;

	if (PriorAction >= _AnimationCount) PriorAction = 0;
	if (CurrentAction(ThreadIndex) >= _AnimationCount) CurrentAction(ThreadIndex) = 0;
	BodyAngle(ThreadIndex) = Angle;

	if (AnimationFrame < 0.0f)
		AnimationFrame = 0.0f;
	if (PriorFrame < 0.0f)
		PriorFrame = 0.0f;

	if (Parent == false)
	{
#ifdef USE_DUAL_QUATERNION
		if (Translate)
		{
			ThreadInfo.ParentMatrix.Set(BodyAngle(ThreadIndex), BodyOrigin(ThreadIndex), BodyScale(ThreadIndex));
		}
		else
		{
			ThreadInfo.ParentMatrix.Set(BodyAngle(ThreadIndex));
		}
#else
		if (Translate)
		{
			EMath::MakeMatrix(ThreadInfo.ParentMatrix, BodyOrigin(ThreadIndex), BodyAngle(ThreadIndex), BodyScale(ThreadIndex));
		}
		else
		{
			ThreadInfo.ParentMatrix = EMath::EulerToMatrix(BodyAngle(ThreadIndex));
		}
#endif
	}

	mu_int32 current = EMath::Floor(AnimationFrame);
	mu_int32 currentNext = EMath::Ceil(AnimationFrame);
	mu_int32 prior = EMath::Floor(PriorFrame);
	mu_int32 priorNext = EMath::Ceil(PriorFrame);

	const Model::AnimationHeader1 &currentHeader = _AnimationHeader[CurrentAction(ThreadIndex)];
	const Model::AnimationData1 &currentAction = _AnimationData[CurrentAction(ThreadIndex)];
	const Model::AnimationHeader1 &priorHeader = _AnimationHeader[PriorAction];
	const Model::AnimationData1 &priorAction = _AnimationData[PriorAction];

	if (currentHeader.Repeat)
	{
		if (current >= currentHeader.FrameCount)
			current = currentHeader.FrameCount - 1;
		if (currentNext >= currentHeader.FrameCount)
			currentNext = currentHeader.FrameCount - 1;
	}
	else
	{
		if (current >= currentHeader.FrameCount)
			current = 0;
		if (currentNext >= currentHeader.FrameCount)
			currentNext = 0;
	}

	if (priorHeader.Repeat)
	{
		if (prior >= priorHeader.FrameCount)
			prior = priorHeader.FrameCount - 1;
		if (priorNext >= priorHeader.FrameCount)
			priorNext = priorHeader.FrameCount - 1;
	}
	else
	{
		if (prior >= priorHeader.FrameCount)
			prior = 0;
		if (priorNext >= priorHeader.FrameCount)
			priorNext = 0;
	}


	CurrentAnimation(ThreadIndex) = AnimationFrame;
	CurrentAnimationFrame(ThreadIndex) = current;

	const mu_float s1 = (CurrentAnimation(ThreadIndex) - CurrentAnimationFrame(ThreadIndex));
	const mu_float s2 = 1.0f - s1;
	const mu_float ps1 = (PriorFrame - prior);
	const mu_float ps2 = 1.0f - ps1;

	const Model::FrameData1 &currentFrame1 = currentAction.FrameData[current];
	const Model::FrameData1 &currentFrame2 = currentAction.FrameData[currentNext];
	const Model::FrameData1 &priorFrame1 = priorAction.FrameData[prior];
	const Model::FrameData1 &priorFrame2 = priorAction.FrameData[priorNext];

	if (_BoneHead >= 0 && _BoneHead < _BoneCount)
	{
		const EQuat headRotation = EMath::QuaternionRotation(EMath::ToRadians(-HeadAngle[0]), 0.0f, EMath::ToRadians(-HeadAngle[1]));

		if (_BoneHead == 0)
		{
			if (PriorAction != CurrentAction(ThreadIndex))
			{
#if __ANDROID__
				__builtin_prefetch(&currentFrame1.BoneData[0], 0, 1);
				__builtin_prefetch(&currentFrame2.BoneData[0], 0, 1);
				__builtin_prefetch(&priorFrame1.BoneData[0], 0, 1);
				__builtin_prefetch(&priorFrame2.BoneData[0], 0, 1);
#else
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[0]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[0]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[0]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[0]);
#endif

				// First Bone Calculation for decrease branching
				if (_BoneRef[0].Dummy == false)
				{
					FastAnimationWithPriorAction<true, true, true>(ThreadIndex, ThreadInfo, 0, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[0],
						currentFrame2.BoneData[0],
						priorFrame1.BoneData[0],
						priorFrame2.BoneData[0],
						s1, s2, ps1, ps2, 0, headRotation);
				}

				mu_int32 i = 1;
				mu_uint32 leftCount = _BoneParentEnd;
				mu_uint32 mulCount = i + ((leftCount - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

				for (; i < mulCount; i += PREFETCH_BONE_COUNT)
				{
#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

					FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 0],
						currentFrame2.BoneData[i + 0],
						priorFrame1.BoneData[i + 0],
						priorFrame2.BoneData[i + 0],
						s1, s2, ps1, ps2);
					FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 1],
						currentFrame2.BoneData[i + 1],
						priorFrame1.BoneData[i + 1],
						priorFrame2.BoneData[i + 1],
						s1, s2, ps1, ps2);
#if (PREFETCH_BONE_COUNT >= 3)
					FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 2],
						currentFrame2.BoneData[i + 2],
						priorFrame1.BoneData[i + 2],
						priorFrame2.BoneData[i + 2],
						s1, s2, ps1, ps2);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
					FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 3],
						currentFrame2.BoneData[i + 3],
						priorFrame1.BoneData[i + 3],
						priorFrame2.BoneData[i + 3],
						s1, s2, ps1, ps2);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
				}

#if __ANDROID__
				__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
				__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
				__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
				__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

				for (; i < leftCount; ++i)
				{
					FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i],
						currentFrame2.BoneData[i],
						priorFrame1.BoneData[i],
						priorFrame2.BoneData[i],
						s1, s2, ps1, ps2);
				}

				leftCount = _BoneNonDummyEnd;
				mulCount = i + ((leftCount - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

				Model::BoneRef1 *ref = &_BoneRef[i];

				for (; i < mulCount; i += PREFETCH_BONE_COUNT, ref += PREFETCH_BONE_COUNT)
				{
#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

					FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 0],
						currentFrame2.BoneData[i + 0],
						priorFrame1.BoneData[i + 0],
						priorFrame2.BoneData[i + 0],
						s1, s2, ps1, ps2, ref[0].Parent);
					FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 1],
						currentFrame2.BoneData[i + 1],
						priorFrame1.BoneData[i + 1],
						priorFrame2.BoneData[i + 1],
						s1, s2, ps1, ps2, ref[1].Parent);
#if (PREFETCH_BONE_COUNT >= 3)
					FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 2],
						currentFrame2.BoneData[i + 2],
						priorFrame1.BoneData[i + 2],
						priorFrame2.BoneData[i + 2],
						s1, s2, ps1, ps2, ref[2].Parent);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
					FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 3],
						currentFrame2.BoneData[i + 3],
						priorFrame1.BoneData[i + 3],
						priorFrame2.BoneData[i + 3],
						s1, s2, ps1, ps2, ref[3].Parent);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
				}

#if __ANDROID__
				__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
				__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
				__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
				__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

				for (; i < leftCount; ++i, ++ref)
				{
					FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i],
						currentFrame2.BoneData[i],
						priorFrame1.BoneData[i],
						priorFrame2.BoneData[i],
						s1, s2, ps1, ps2, ref->Parent);
				}
			}
			else
			{
#if __ANDROID__
				__builtin_prefetch(&currentFrame1.BoneData[0], 0, 1);
				__builtin_prefetch(&currentFrame2.BoneData[0], 0, 1);
#else
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[0]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[0]);
#endif

				// First Bone Calculation for decrease branching
				if (_BoneRef[0].Dummy == false)
				{
					FastAnimation<true, true, true>(ThreadIndex, ThreadInfo, 0, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[0],
						currentFrame2.BoneData[0],
						s1, s2, 0, headRotation);
				}

				mu_int32 i = 1;
				mu_uint32 leftCount = _BoneParentEnd;
				mu_uint32 mulCount = i + ((leftCount - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

				for (; i < mulCount; i += PREFETCH_BONE_COUNT)
				{
#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

					FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 0],
						currentFrame2.BoneData[i + 0],
						s1, s2);
					FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 1],
						currentFrame2.BoneData[i + 1],
						s1, s2);
#if (PREFETCH_BONE_COUNT >= 3)
					FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 2],
						currentFrame2.BoneData[i + 2],
						s1, s2);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
					FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 3],
						currentFrame2.BoneData[i + 3],
						s1, s2);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
				}

#if __ANDROID__
				__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
				__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

				for (; i < leftCount; ++i)
				{
					FastAnimation<true, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i],
						currentFrame2.BoneData[i],
						s1, s2);
				}

				leftCount = _BoneNonDummyEnd;
				mulCount = i + ((leftCount - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

				Model::BoneRef1 *ref = &_BoneRef[i];

				for (; i < mulCount; i += PREFETCH_BONE_COUNT, ref += PREFETCH_BONE_COUNT)
				{
#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

					FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 0],
						currentFrame2.BoneData[i + 0],
						s1, s2, ref[0].Parent);
					FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 1],
						currentFrame2.BoneData[i + 1],
						s1, s2, ref[1].Parent);
#if (PREFETCH_BONE_COUNT >= 3)
					FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 2],
						currentFrame2.BoneData[i + 2],
						s1, s2, ref[2].Parent);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
					FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 3],
						currentFrame2.BoneData[i + 3],
						s1, s2, ref[3].Parent);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
				}

#if __ANDROID__
				__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
				__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

				for (; i < leftCount; ++i, ++ref)
				{
					FastAnimation<false, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i],
						currentFrame2.BoneData[i],
						s1, s2, ref->Parent);
				}
			}
		}
		else if (_BoneHead < _BoneParentEnd)
		{
			if (PriorAction != CurrentAction(ThreadIndex))
			{
#if __ANDROID__
				__builtin_prefetch(&currentFrame1.BoneData[0], 0, 1);
				__builtin_prefetch(&currentFrame2.BoneData[0], 0, 1);
				__builtin_prefetch(&priorFrame1.BoneData[0], 0, 1);
				__builtin_prefetch(&priorFrame2.BoneData[0], 0, 1);
#else
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[0]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[0]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[0]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[0]);
#endif

				// First Bone Calculation for decrease branching
				if (_BoneRef[0].Dummy == false)
				{
					FastAnimationWithPriorAction<true, true>(ThreadIndex, ThreadInfo, 0, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[0],
						currentFrame2.BoneData[0],
						priorFrame1.BoneData[0],
						priorFrame2.BoneData[0],
						s1, s2, ps1, ps2);
				}

				mu_int32 i = 1;

				mu_int32 blockHead = i + ((_BoneHead - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

				mu_uint32 leftCount = _BoneParentEnd;
				mu_uint32 mulCount = i + ((leftCount - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

				for (; i < blockHead; i += PREFETCH_BONE_COUNT)
				{
#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

					FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 0],
						currentFrame2.BoneData[i + 0],
						priorFrame1.BoneData[i + 0],
						priorFrame2.BoneData[i + 0],
						s1, s2, ps1, ps2);
					FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 1],
						currentFrame2.BoneData[i + 1],
						priorFrame1.BoneData[i + 1],
						priorFrame2.BoneData[i + 1],
						s1, s2, ps1, ps2);
#if (PREFETCH_BONE_COUNT >= 3)
					FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 2],
						currentFrame2.BoneData[i + 2],
						priorFrame1.BoneData[i + 2],
						priorFrame2.BoneData[i + 2],
						s1, s2, ps1, ps2);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
					FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 3],
						currentFrame2.BoneData[i + 3],
						priorFrame1.BoneData[i + 3],
						priorFrame2.BoneData[i + 3],
						s1, s2, ps1, ps2);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
				}

				if(leftCount - blockHead >= PREFETCH_BONE_COUNT)
				{
					// Bone Head Block
					{
#if __ANDROID__
						__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
						__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
						__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
						__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

						FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 0],
							currentFrame2.BoneData[i + 0],
							priorFrame1.BoneData[i + 0],
							priorFrame2.BoneData[i + 0],
							s1, s2, ps1, ps2, _BoneHead, headRotation);
						FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 1],
							currentFrame2.BoneData[i + 1],
							priorFrame1.BoneData[i + 1],
							priorFrame2.BoneData[i + 1],
							s1, s2, ps1, ps2, _BoneHead, headRotation);
#if (PREFETCH_BONE_COUNT >= 3)
						FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 2],
							currentFrame2.BoneData[i + 2],
							priorFrame1.BoneData[i + 2],
							priorFrame2.BoneData[i + 2],
							s1, s2, ps1, ps2, _BoneHead, headRotation);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
						FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 3],
							currentFrame2.BoneData[i + 3],
							priorFrame1.BoneData[i + 3],
							priorFrame2.BoneData[i + 3],
							s1, s2, ps1, ps2, _BoneHead, headRotation);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif

						i += PREFETCH_BONE_COUNT;
					}

					for (; i < mulCount; i += PREFETCH_BONE_COUNT)
					{
#if __ANDROID__
						__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
						__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
						__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
						__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

						FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 0],
							currentFrame2.BoneData[i + 0],
							priorFrame1.BoneData[i + 0],
							priorFrame2.BoneData[i + 0],
							s1, s2, ps1, ps2);
						FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 1],
							currentFrame2.BoneData[i + 1],
							priorFrame1.BoneData[i + 1],
							priorFrame2.BoneData[i + 1],
							s1, s2, ps1, ps2);
#if (PREFETCH_BONE_COUNT >= 3)
						FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 2],
							currentFrame2.BoneData[i + 2],
							priorFrame1.BoneData[i + 2],
							priorFrame2.BoneData[i + 2],
							s1, s2, ps1, ps2);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
						FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 3],
							currentFrame2.BoneData[i + 3],
							priorFrame1.BoneData[i + 3],
							priorFrame2.BoneData[i + 3],
							s1, s2, ps1, ps2);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
					}

#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

					for (; i < leftCount; ++i)
					{
						FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i],
							currentFrame2.BoneData[i],
							priorFrame1.BoneData[i],
							priorFrame2.BoneData[i],
							s1, s2, ps1, ps2);
					}

					leftCount = _BoneNonDummyEnd;
					mulCount = i + ((leftCount - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

					Model::BoneRef1 *ref = &_BoneRef[i];

					for (; i < mulCount; i += PREFETCH_BONE_COUNT, ref += PREFETCH_BONE_COUNT)
					{
#if __ANDROID__
						__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
						__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
						__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
						__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

						FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 0],
							currentFrame2.BoneData[i + 0],
							priorFrame1.BoneData[i + 0],
							priorFrame2.BoneData[i + 0],
							s1, s2, ps1, ps2, ref[0].Parent);
						FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 1],
							currentFrame2.BoneData[i + 1],
							priorFrame1.BoneData[i + 1],
							priorFrame2.BoneData[i + 1],
							s1, s2, ps1, ps2, ref[1].Parent);
#if (PREFETCH_BONE_COUNT >= 3)
						FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 2],
							currentFrame2.BoneData[i + 2],
							priorFrame1.BoneData[i + 2],
							priorFrame2.BoneData[i + 2],
							s1, s2, ps1, ps2, ref[2].Parent);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
						FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 3],
							currentFrame2.BoneData[i + 3],
							priorFrame1.BoneData[i + 3],
							priorFrame2.BoneData[i + 3],
							s1, s2, ps1, ps2, ref[3].Parent);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
					}

#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

					for (; i < leftCount; ++i, ++ref)
					{
						FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i],
							currentFrame2.BoneData[i],
							priorFrame1.BoneData[i],
							priorFrame2.BoneData[i],
							s1, s2, ps1, ps2, ref->Parent);
					}
				}
				else
				{
#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

					for (; i < leftCount; ++i)
					{
						FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i],
							currentFrame2.BoneData[i],
							priorFrame1.BoneData[i],
							priorFrame2.BoneData[i],
							s1, s2, ps1, ps2, _BoneHead, headRotation);
					}
				}
			}
			else
			{
#if __ANDROID__
				__builtin_prefetch(&currentFrame1.BoneData[0], 0, 1);
				__builtin_prefetch(&currentFrame2.BoneData[0], 0, 1);
#else
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[0]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[0]);
#endif

				// First Bone Calculation for decrease branching
				if (_BoneRef[0].Dummy == false)
				{
					FastAnimation<true, true>(ThreadIndex, ThreadInfo, 0, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[0],
						currentFrame2.BoneData[0],
						s1, s2);
				}

				mu_int32 i = 1;

				mu_int32 blockHead = i + ((_BoneHead - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

				mu_uint32 leftCount = _BoneParentEnd;
				mu_uint32 mulCount = i + ((leftCount - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

				for (; i < blockHead; i += PREFETCH_BONE_COUNT)
				{
#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

					FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 0],
						currentFrame2.BoneData[i + 0],
						s1, s2);
					FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 1],
						currentFrame2.BoneData[i + 1],
						s1, s2);
#if (PREFETCH_BONE_COUNT >= 3)
					FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 2],
						currentFrame2.BoneData[i + 2],
						s1, s2);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
					FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 3],
						currentFrame2.BoneData[i + 3],
						s1, s2);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
				}

				if(leftCount - blockHead >= PREFETCH_BONE_COUNT)
				{
					// Bone Head Block
					{
#if __ANDROID__
						__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
						__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

						FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 0],
							currentFrame2.BoneData[i + 0],
							s1, s2, _BoneHead, headRotation);
						FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 1],
							currentFrame2.BoneData[i + 1],
							s1, s2, _BoneHead, headRotation);
#if (PREFETCH_BONE_COUNT >= 3)
						FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 2],
							currentFrame2.BoneData[i + 2],
							s1, s2, _BoneHead, headRotation);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
						FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 3],
							currentFrame2.BoneData[i + 3],
							s1, s2, _BoneHead, headRotation);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
						i += PREFETCH_BONE_COUNT;
					}

					for (; i < mulCount; i += PREFETCH_BONE_COUNT)
					{
#if __ANDROID__
						__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
						__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

						FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 0],
							currentFrame2.BoneData[i + 0],
							s1, s2);
						FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 1],
							currentFrame2.BoneData[i + 1],
							s1, s2);
#if (PREFETCH_BONE_COUNT >= 3)
						FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 2],
							currentFrame2.BoneData[i + 2],
							s1, s2);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
						FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 3],
							currentFrame2.BoneData[i + 3],
							s1, s2);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
					}

#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

					for (; i < leftCount; ++i)
					{
						FastAnimation<true, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i],
							currentFrame2.BoneData[i],
							s1, s2);
					}

					leftCount = _BoneNonDummyEnd;
					mulCount = i + ((leftCount - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

					Model::BoneRef1 *ref = &_BoneRef[i];

					for (; i < mulCount; i += PREFETCH_BONE_COUNT, ref += PREFETCH_BONE_COUNT)
					{
#if __ANDROID__
						__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
						__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

						FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 0],
							currentFrame2.BoneData[i + 0],
							s1, s2, ref[0].Parent);
						FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 1],
							currentFrame2.BoneData[i + 1],
							s1, s2, ref[1].Parent);
#if (PREFETCH_BONE_COUNT >= 3)
						FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 2],
							currentFrame2.BoneData[i + 2],
							s1, s2, ref[2].Parent);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
						FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 3],
							currentFrame2.BoneData[i + 3],
							s1, s2, ref[3].Parent);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
					}

#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

					for (; i < leftCount; ++i, ++ref)
					{
						FastAnimation<false, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i],
							currentFrame2.BoneData[i],
							s1, s2, ref->Parent);
					}
				}
				else
				{
#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

					for (; i < leftCount; ++i)
					{
						FastAnimation<true, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i],
							currentFrame2.BoneData[i],
							s1, s2, _BoneHead, headRotation);
					}
				}
			}
		}
		else if (_BoneHead < _BoneNonDummyEnd)
		{
			if (PriorAction != CurrentAction(ThreadIndex))
			{
#if __ANDROID__
				__builtin_prefetch(&currentFrame1.BoneData[0], 0, 1);
				__builtin_prefetch(&currentFrame2.BoneData[0], 0, 1);
				__builtin_prefetch(&priorFrame1.BoneData[0], 0, 1);
				__builtin_prefetch(&priorFrame2.BoneData[0], 0, 1);
#else
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[0]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[0]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[0]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[0]);
#endif

				// First Bone Calculation for decrease branching
				if (_BoneRef[0].Dummy == false)
				{
					FastAnimationWithPriorAction<true, true>(ThreadIndex, ThreadInfo, 0, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[0],
						currentFrame2.BoneData[0],
						priorFrame1.BoneData[0],
						priorFrame2.BoneData[0],
						s1, s2, ps1, ps2);
				}

				mu_int32 i = 1;
				mu_uint32 leftCount = _BoneParentEnd;
				mu_uint32 mulCount = i + ((leftCount - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

				for (; i < mulCount; i += PREFETCH_BONE_COUNT)
				{
#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

					FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 0],
						currentFrame2.BoneData[i + 0],
						priorFrame1.BoneData[i + 0],
						priorFrame2.BoneData[i + 0],
						s1, s2, ps1, ps2);
					FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 1],
						currentFrame2.BoneData[i + 1],
						priorFrame1.BoneData[i + 1],
						priorFrame2.BoneData[i + 1],
						s1, s2, ps1, ps2);
#if (PREFETCH_BONE_COUNT >= 3)
					FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 2],
						currentFrame2.BoneData[i + 2],
						priorFrame1.BoneData[i + 2],
						priorFrame2.BoneData[i + 2],
						s1, s2, ps1, ps2);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
					FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 3],
						currentFrame2.BoneData[i + 3],
						priorFrame1.BoneData[i + 3],
						priorFrame2.BoneData[i + 3],
						s1, s2, ps1, ps2);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
				}

#if __ANDROID__
				__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
				__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
				__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
				__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

				for (; i < leftCount; ++i)
				{
					FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i],
						currentFrame2.BoneData[i],
						priorFrame1.BoneData[i],
						priorFrame2.BoneData[i],
						s1, s2, ps1, ps2);
				}

				mu_int32 blockHead = i + ((_BoneHead - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;
				leftCount = _BoneNonDummyEnd;
				mulCount = i + ((leftCount - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

				Model::BoneRef1 *ref = &_BoneRef[i];

				for (; i < blockHead; i += PREFETCH_BONE_COUNT, ref += PREFETCH_BONE_COUNT)
				{
#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

					FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 0],
						currentFrame2.BoneData[i + 0],
						priorFrame1.BoneData[i + 0],
						priorFrame2.BoneData[i + 0],
						s1, s2, ps1, ps2, ref[0].Parent);
					FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 1],
						currentFrame2.BoneData[i + 1],
						priorFrame1.BoneData[i + 1],
						priorFrame2.BoneData[i + 1],
						s1, s2, ps1, ps2, ref[1].Parent);
#if (PREFETCH_BONE_COUNT >= 3)
					FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 2],
						currentFrame2.BoneData[i + 2],
						priorFrame1.BoneData[i + 2],
						priorFrame2.BoneData[i + 2],
						s1, s2, ps1, ps2, ref[2].Parent);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
					FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
						currentHeader, priorHeader,
						currentAction, priorAction,
						currentFrame1.BoneData[i + 3],
						currentFrame2.BoneData[i + 3],
						priorFrame1.BoneData[i + 3],
						priorFrame2.BoneData[i + 3],
						s1, s2, ps1, ps2, ref[3].Parent);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
				}

				if(leftCount - blockHead >= PREFETCH_BONE_COUNT)
				{
					// Bone Head Block
					{
#if __ANDROID__
						__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
						__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
						__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
						__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

						FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 0],
							currentFrame2.BoneData[i + 0],
							priorFrame1.BoneData[i + 0],
							priorFrame2.BoneData[i + 0],
							s1, s2, ps1, ps2, _BoneHead, headRotation, ref[0].Parent);
						FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 1],
							currentFrame2.BoneData[i + 1],
							priorFrame1.BoneData[i + 1],
							priorFrame2.BoneData[i + 1],
							s1, s2, ps1, ps2, _BoneHead, headRotation, ref[1].Parent);
#if (PREFETCH_BONE_COUNT >= 3)
						FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 2],
							currentFrame2.BoneData[i + 2],
							priorFrame1.BoneData[i + 2],
							priorFrame2.BoneData[i + 2],
							s1, s2, ps1, ps2, _BoneHead, headRotation, ref[2].Parent);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
						FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 3],
							currentFrame2.BoneData[i + 3],
							priorFrame1.BoneData[i + 3],
							priorFrame2.BoneData[i + 3],
							s1, s2, ps1, ps2, _BoneHead, headRotation, ref[3].Parent);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
						i += PREFETCH_BONE_COUNT, ref += PREFETCH_BONE_COUNT;
					}

					for (; i < mulCount; i += PREFETCH_BONE_COUNT, ref += PREFETCH_BONE_COUNT)
					{
#if __ANDROID__
						__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
						__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
						__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
						__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

						FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 0],
							currentFrame2.BoneData[i + 0],
							priorFrame1.BoneData[i + 0],
							priorFrame2.BoneData[i + 0],
							s1, s2, ps1, ps2, ref[0].Parent);
						FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 1],
							currentFrame2.BoneData[i + 1],
							priorFrame1.BoneData[i + 1],
							priorFrame2.BoneData[i + 1],
							s1, s2, ps1, ps2, ref[1].Parent);
#if (PREFETCH_BONE_COUNT >= 3)
						FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 2],
							currentFrame2.BoneData[i + 2],
							priorFrame1.BoneData[i + 2],
							priorFrame2.BoneData[i + 2],
							s1, s2, ps1, ps2, ref[2].Parent);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
						FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i + 3],
							currentFrame2.BoneData[i + 3],
							priorFrame1.BoneData[i + 3],
							priorFrame2.BoneData[i + 3],
							s1, s2, ps1, ps2, ref[3].Parent);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
					}

#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

					for (; i < leftCount; ++i, ++ref)
					{
						FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i],
							currentFrame2.BoneData[i],
							priorFrame1.BoneData[i],
							priorFrame2.BoneData[i],
							s1, s2, ps1, ps2, ref->Parent);
					}
				}
				else
				{
#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

					for (; i < leftCount; ++i, ++ref)
					{
						FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
							currentHeader, priorHeader,
							currentAction, priorAction,
							currentFrame1.BoneData[i],
							currentFrame2.BoneData[i],
							priorFrame1.BoneData[i],
							priorFrame2.BoneData[i],
							s1, s2, ps1, ps2, _BoneHead, headRotation, ref->Parent);
					}
				}
			}
			else
			{
#if __ANDROID__
				__builtin_prefetch(&currentFrame1.BoneData[0], 0, 1);
				__builtin_prefetch(&currentFrame2.BoneData[0], 0, 1);
#else
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[0]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[0]);
#endif

				// First Bone Calculation for decrease branching
				if (_BoneRef[0].Dummy == false)
				{
					FastAnimation<true, true>(ThreadIndex, ThreadInfo, 0, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[0],
						currentFrame2.BoneData[0],
						s1, s2);
				}

				mu_int32 i = 1;
				mu_uint32 leftCount = _BoneParentEnd;
				mu_uint32 mulCount = i + ((leftCount - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

				for (; i < mulCount; i += PREFETCH_BONE_COUNT)
				{
#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

					FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 0],
						currentFrame2.BoneData[i + 0],
						s1, s2);
					FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 1],
						currentFrame2.BoneData[i + 1],
						s1, s2);
#if (PREFETCH_BONE_COUNT >= 3)
					FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 2],
						currentFrame2.BoneData[i + 2],
						s1, s2);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
					FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 3],
						currentFrame2.BoneData[i + 3],
						s1, s2);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
				}

#if __ANDROID__
				__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
				__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

				for (; i < leftCount; ++i)
				{
					FastAnimation<true, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i],
						currentFrame2.BoneData[i],
						s1, s2);
				}

				mu_int32 blockHead = i + ((_BoneHead - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;
				leftCount = _BoneNonDummyEnd;
				mulCount = i + ((leftCount - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

				Model::BoneRef1 *ref = &_BoneRef[i];

				for (; i < blockHead; i += PREFETCH_BONE_COUNT, ref += PREFETCH_BONE_COUNT)
				{
#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

					FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 0],
						currentFrame2.BoneData[i + 0],
						s1, s2, ref[0].Parent);
					FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 1],
						currentFrame2.BoneData[i + 1],
						s1, s2, ref[1].Parent);
#if (PREFETCH_BONE_COUNT >= 3)
					FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 2],
						currentFrame2.BoneData[i + 2],
						s1, s2, ref[2].Parent);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
					FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
						currentHeader,
						currentAction,
						currentFrame1.BoneData[i + 3],
						currentFrame2.BoneData[i + 3],
						s1, s2, ref[3].Parent);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
				}

				if (leftCount - blockHead >= PREFETCH_BONE_COUNT)
				{
					// Bone Head Block
					{
#if __ANDROID__
						__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
						__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

						FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 0],
							currentFrame2.BoneData[i + 0],
							s1, s2, _BoneHead, headRotation, ref[0].Parent);
						FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 1],
							currentFrame2.BoneData[i + 1],
							s1, s2, _BoneHead, headRotation, ref[1].Parent);
#if (PREFETCH_BONE_COUNT >= 3)
						FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 2],
							currentFrame2.BoneData[i + 2],
							s1, s2, _BoneHead, headRotation, ref[2].Parent);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
						FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 3],
							currentFrame2.BoneData[i + 3],
							s1, s2, _BoneHead, headRotation, ref[3].Parent);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
						i += PREFETCH_BONE_COUNT, ref += PREFETCH_BONE_COUNT;
					}

					for (; i < mulCount; i += PREFETCH_BONE_COUNT, ref += PREFETCH_BONE_COUNT)
					{
#if __ANDROID__
						__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
						__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
						PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

						FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 0],
							currentFrame2.BoneData[i + 0],
							s1, s2, ref[0].Parent);
						FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 1],
							currentFrame2.BoneData[i + 1],
							s1, s2, ref[1].Parent);
#if (PREFETCH_BONE_COUNT >= 3)
						FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 2],
							currentFrame2.BoneData[i + 2],
							s1, s2, ref[2].Parent);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
						FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i + 3],
							currentFrame2.BoneData[i + 3],
							s1, s2, ref[3].Parent);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
					}

#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

					for (; i < leftCount; ++i, ++ref)
					{
						FastAnimation<false, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i],
							currentFrame2.BoneData[i],
							s1, s2, ref->Parent);
					}
				}
				else
				{
#if __ANDROID__
					__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
					__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
					PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

					for (; i < leftCount; ++i, ++ref)
					{
						FastAnimation<false, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
							currentHeader,
							currentAction,
							currentFrame1.BoneData[i],
							currentFrame2.BoneData[i],
							s1, s2, _BoneHead, headRotation, ref->Parent);
					}
				}
			}
		}
	}
	else
	{
		if (PriorAction != CurrentAction(ThreadIndex))
		{
#if __ANDROID__
			__builtin_prefetch(&currentFrame1.BoneData[0], 0, 1);
			__builtin_prefetch(&currentFrame2.BoneData[0], 0, 1);
			__builtin_prefetch(&priorFrame1.BoneData[0], 0, 1);
			__builtin_prefetch(&priorFrame2.BoneData[0], 0, 1);
#else
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[0]);
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[0]);
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[0]);
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[0]);
#endif

			// First Bone Calculation for decrease branching
			if (_BoneRef[0].Dummy == false)
			{
				FastAnimationWithPriorAction<true, true>(ThreadIndex, ThreadInfo, 0, BoneMatrix,
					currentHeader, priorHeader,
					currentAction, priorAction,
					currentFrame1.BoneData[0],
					currentFrame2.BoneData[0],
					priorFrame1.BoneData[0],
					priorFrame2.BoneData[0],
					s1, s2, ps1, ps2);
			}

			mu_int32 i = 1;
			mu_uint32 leftCount = _BoneParentEnd;
			mu_uint32 mulCount = i + ((leftCount - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

			for (; i < mulCount; i += PREFETCH_BONE_COUNT)
			{
#if __ANDROID__
				__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
				__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
				__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
				__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

				FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
					currentHeader, priorHeader,
					currentAction, priorAction,
					currentFrame1.BoneData[i + 0],
					currentFrame2.BoneData[i + 0],
					priorFrame1.BoneData[i + 0],
					priorFrame2.BoneData[i + 0],
					s1, s2, ps1, ps2);
				FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
					currentHeader, priorHeader,
					currentAction, priorAction,
					currentFrame1.BoneData[i + 1],
					currentFrame2.BoneData[i + 1],
					priorFrame1.BoneData[i + 1],
					priorFrame2.BoneData[i + 1],
					s1, s2, ps1, ps2);
#if (PREFETCH_BONE_COUNT >= 3)
				FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
					currentHeader, priorHeader,
					currentAction, priorAction,
					currentFrame1.BoneData[i + 2],
					currentFrame2.BoneData[i + 2],
					priorFrame1.BoneData[i + 2],
					priorFrame2.BoneData[i + 2],
					s1, s2, ps1, ps2);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
				FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
					currentHeader, priorHeader,
					currentAction, priorAction,
					currentFrame1.BoneData[i + 3],
					currentFrame2.BoneData[i + 3],
					priorFrame1.BoneData[i + 3],
					priorFrame2.BoneData[i + 3],
					s1, s2, ps1, ps2);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
			}

#if __ANDROID__
			__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
			__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
			__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
			__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

			for (; i < leftCount; ++i)
			{
				FastAnimationWithPriorAction<true, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
					currentHeader, priorHeader,
					currentAction, priorAction,
					currentFrame1.BoneData[i],
					currentFrame2.BoneData[i],
					priorFrame1.BoneData[i],
					priorFrame2.BoneData[i],
					s1, s2, ps1, ps2);
			}

			leftCount = _BoneNonDummyEnd;
			mulCount = i + ((leftCount - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

			Model::BoneRef1 *ref = &_BoneRef[i];

			for (; i < mulCount; i += PREFETCH_BONE_COUNT, ref += PREFETCH_BONE_COUNT)
			{
#if __ANDROID__
				__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
				__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
				__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
				__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

				FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
					currentHeader, priorHeader,
					currentAction, priorAction,
					currentFrame1.BoneData[i + 0],
					currentFrame2.BoneData[i + 0],
					priorFrame1.BoneData[i + 0],
					priorFrame2.BoneData[i + 0],
					s1, s2, ps1, ps2, ref[0].Parent);
				FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
					currentHeader, priorHeader,
					currentAction, priorAction,
					currentFrame1.BoneData[i + 1],
					currentFrame2.BoneData[i + 1],
					priorFrame1.BoneData[i + 1],
					priorFrame2.BoneData[i + 1],
					s1, s2, ps1, ps2, ref[1].Parent);
#if (PREFETCH_BONE_COUNT >= 3)
				FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
					currentHeader, priorHeader,
					currentAction, priorAction,
					currentFrame1.BoneData[i + 2],
					currentFrame2.BoneData[i + 2],
					priorFrame1.BoneData[i + 2],
					priorFrame2.BoneData[i + 2],
					s1, s2, ps1, ps2, ref[2].Parent);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
				FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
					currentHeader, priorHeader,
					currentAction, priorAction,
					currentFrame1.BoneData[i + 3],
					currentFrame2.BoneData[i + 3],
					priorFrame1.BoneData[i + 3],
					priorFrame2.BoneData[i + 3],
					s1, s2, ps1, ps2, ref[3].Parent);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
			}

#if __ANDROID__
			__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
			__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
			__builtin_prefetch(&priorFrame1.BoneData[i], 0, 1);
			__builtin_prefetch(&priorFrame2.BoneData[i], 0, 1);
#else
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame1.BoneData[i]);
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &priorFrame2.BoneData[i]);
#endif

			for (; i < leftCount; ++i, ++ref)
			{
				FastAnimationWithPriorAction<false, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
					currentHeader, priorHeader,
					currentAction, priorAction,
					currentFrame1.BoneData[i],
					currentFrame2.BoneData[i],
					priorFrame1.BoneData[i],
					priorFrame2.BoneData[i],
					s1, s2, ps1, ps2, ref->Parent);
			}
		}
		else
		{
#if __ANDROID__
			__builtin_prefetch(&currentFrame1.BoneData[0], 0, 1);
			__builtin_prefetch(&currentFrame2.BoneData[0], 0, 1);
#else
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[0]);
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[0]);
#endif

			// First Bone Calculation for decrease branching
			if (_BoneRef[0].Dummy == false)
			{
				FastAnimation<true, true>(ThreadIndex, ThreadInfo, 0, BoneMatrix,
					currentHeader,
					currentAction,
					currentFrame1.BoneData[0],
					currentFrame2.BoneData[0],
					s1, s2);
			}

			mu_int32 i = 1;
			mu_uint32 leftCount = _BoneParentEnd;
			mu_uint32 mulCount = i + ((leftCount - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

			for (; i < mulCount; i += PREFETCH_BONE_COUNT)
			{
#if __ANDROID__
				__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
				__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

				FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
					currentHeader,
					currentAction,
					currentFrame1.BoneData[i + 0],
					currentFrame2.BoneData[i + 0],
					s1, s2);
				FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
					currentHeader,
					currentAction,
					currentFrame1.BoneData[i + 1],
					currentFrame2.BoneData[i + 1],
					s1, s2);
#if (PREFETCH_BONE_COUNT >= 3)
				FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
					currentHeader,
					currentAction,
					currentFrame1.BoneData[i + 2],
					currentFrame2.BoneData[i + 2],
					s1, s2);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
				FastAnimation<true, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
					currentHeader,
					currentAction,
					currentFrame1.BoneData[i + 3],
					currentFrame2.BoneData[i + 3],
					s1, s2);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
			}

#if __ANDROID__
			__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
			__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

			for (; i < leftCount; ++i)
			{
				FastAnimation<true, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
					currentHeader,
					currentAction,
					currentFrame1.BoneData[i],
					currentFrame2.BoneData[i],
					s1, s2);
			}

			leftCount = _BoneNonDummyEnd;
			mulCount = i + ((leftCount - i) / PREFETCH_BONE_COUNT) * PREFETCH_BONE_COUNT;

			Model::BoneRef1 *ref = &_BoneRef[i];

			for (; i < mulCount; i += PREFETCH_BONE_COUNT, ref += PREFETCH_BONE_COUNT)
			{
#if __ANDROID__
				__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
				__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
				PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

				FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 0, BoneMatrix,
					currentHeader,
					currentAction,
					currentFrame1.BoneData[i + 0],
					currentFrame2.BoneData[i + 0],
					s1, s2, ref[0].Parent);
				FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 1, BoneMatrix,
					currentHeader,
					currentAction,
					currentFrame1.BoneData[i + 1],
					currentFrame2.BoneData[i + 1],
					s1, s2, ref[1].Parent);
#if (PREFETCH_BONE_COUNT >= 3)
				FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 2, BoneMatrix,
					currentHeader,
					currentAction,
					currentFrame1.BoneData[i + 2],
					currentFrame2.BoneData[i + 2],
					s1, s2, ref[2].Parent);
#endif
#if (PREFETCH_BONE_COUNT >= 4)
				FastAnimation<false, false>(ThreadIndex, ThreadInfo, i + 3, BoneMatrix,
					currentHeader,
					currentAction,
					currentFrame1.BoneData[i + 3],
					currentFrame2.BoneData[i + 3],
					s1, s2, ref[3].Parent);
#endif
#if (PREFETCH_BONE_COUNT > 4)
#pragma error("Add more processing data")
#endif
			}

#if __ANDROID__
			__builtin_prefetch(&currentFrame1.BoneData[i], 0, 1);
			__builtin_prefetch(&currentFrame2.BoneData[i], 0, 1);
#else
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame1.BoneData[i]);
			PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, &currentFrame2.BoneData[i]);
#endif

			for (; i < leftCount; ++i, ++ref)
			{
				FastAnimation<false, false>(ThreadIndex, ThreadInfo, i, BoneMatrix,
					currentHeader,
					currentAction,
					currentFrame1.BoneData[i],
					currentFrame2.BoneData[i],
					s1, s2, ref->Parent);
			}
		}
	}
}

void MUModel::Transform(const mu_uint32 ThreadIndex, EVector3 &BoundingBoxMin, EVector3 &BoundingBoxMax, OBB_t *OBB, mu_boolean Translate)
{
	VectorCopy(BoundingBoxMin, OBB->StartPos);
	OBB->XAxis[0] = (BoundingBoxMax[0] - BoundingBoxMin[0]);
	OBB->YAxis[1] = (BoundingBoxMax[1] - BoundingBoxMin[1]);
	OBB->ZAxis[2] = (BoundingBoxMax[2] - BoundingBoxMin[2]);
	VectorAdd(OBB->StartPos, BodyOrigin(ThreadIndex), OBB->StartPos);
	OBB->XAxis[1] = 0.0f;
	OBB->XAxis[2] = 0.0f;
	OBB->YAxis[0] = 0.0f;
	OBB->YAxis[2] = 0.0f;
	OBB->ZAxis[0] = 0.0f;
	OBB->ZAxis[1] = 0.0f;
}

void MUModel::RenderMesh(const mu_uint32 ThreadIndex, mu_int32 i, mu_int32 RenderFlag, mu_float Alpha, mu_int32 BlendMesh, mu_float BlendMeshLight, mu_float BlendMeshTexCoordU, mu_float BlendMeshTexCoordV, mu_int32 MeshTexture)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (i >= _MeshCount || i < 0) return;

	Model::MeshData1 *m = &_MeshData[i];
	Model::MeshExtra1 *me = &_MeshExtra[i];
	if (m->IndexCount == 0) return;

	mu_int32 Texture = _MeshTexture[i];
	if (Texture == BITMAP_HIDE)
		return;
	else if (Texture == BITMAP_SKIN)
	{
		if (HideSkin(ThreadIndex)) return;
		Texture = BITMAP_SKIN + Skin(ThreadIndex);
	}
	else if (Texture == BITMAP_WATER)
	{
		Texture = BITMAP_WATER + WaterTextureNumber;
	}
	else  if (Texture == BITMAP_HAIR)
	{
		if (HideSkin(ThreadIndex)) return;
		Texture = BITMAP_HAIR + (Skin(ThreadIndex) - 8);
	}

	if (MeshTexture != -1)
		Texture = MeshTexture;

	MUTexture* pBitmap = &TEXTURES::Data[Texture];

	mu_boolean EnableWave = false;
	mu_int32 streamMesh = StreamMesh(ThreadIndex);
	if (me->IsUsed != false)
	{
		if (me->IsStream == true)
		{
			streamMesh = i;
		}
	}

	if ((i == BlendMesh || i == streamMesh) && (BlendMeshTexCoordU != 0.0f || BlendMeshTexCoordV != 0.0f))
		EnableWave = true;

	mu_boolean EnableLight = LightEnable(ThreadIndex);
	if (i == StreamMesh(ThreadIndex))
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(BodyLight(ThreadIndex), 1.0f);
		EnableLight = false;
	}

	mu_uint8 RenderType;
	mu_uint8 BlendType;
	mu_boolean EnableDepth = true;

	EVector3 bodyLight = BodyLight(ThreadIndex);

	if ((RenderFlag&RENDER_COLOR) == RENDER_COLOR)
	{
		RenderType = RT_COLOR;

#if 1 // FIX seems like it should always be AlphaTest and done
		if ((RenderFlag&RENDER_BRIGHT) == RENDER_BRIGHT)
		{
			BlendType = BLEND::TXB_BLEND01;
		}
		else if ((RenderFlag&RENDER_DARK) == RENDER_DARK)
		{
			BlendType = BLEND::TXB_BLENDMINUS;
		}
		else
		{
			BlendType = BLEND::TXB_BLEND01;
		}

		if ((RenderFlag&RENDER_NODEPTH) == RENDER_NODEPTH)
		{
			EnableDepth = false;
		}
#else
		BlendType = BLEND::TXB_ALPHATEST_BEGIN + ThreadInfo.AlphaTestIndex;
#endif

		ThreadInfo.RenderLight = MU::Float4ToARGB(BodyLight(ThreadIndex), Alpha);
	}
	else if ((RenderFlag&RENDER_CHROME) == RENDER_CHROME ||
		(RenderFlag&RENDER_CHROME2) == RENDER_CHROME2 ||
		(RenderFlag&RENDER_CHROME3) == RENDER_CHROME3 ||
		(RenderFlag&RENDER_CHROME4) == RENDER_CHROME4 ||
		(RenderFlag&RENDER_CHROME5) == RENDER_CHROME5 ||
		(RenderFlag&RENDER_CHROME6) == RENDER_CHROME6 ||
		(RenderFlag&RENDER_CHROME7) == RENDER_CHROME7 ||
#ifdef PJH_NEW_CHROME
		(RenderFlag&RENDER_CHROME8) == RENDER_CHROME8 ||
#endif //#ifdef PJH_NEW_CHROME
		(RenderFlag&RENDER_METAL) == RENDER_METAL ||
		(RenderFlag&RENDER_OIL) == RENDER_OIL
		)
	{
		if (me->IsUsed)
		{
			if (me->IsNoneBlend) return;
		}

		if ((RenderFlag &RENDER_CHROME) == RENDER_CHROME)
		{
			RenderType = RT_CHROME;
		}
		else if ((RenderFlag &RENDER_CHROME2) == RENDER_CHROME2)
		{
			RenderType = RT_CHROME2;
		}
		else if ((RenderFlag &RENDER_CHROME3) == RENDER_CHROME3)
		{
			RenderType = RT_CHROME3;
		}
		else if ((RenderFlag &RENDER_CHROME4) == RENDER_CHROME4)
		{
			RenderType = RT_CHROME4;
		}
		else if ((RenderFlag &RENDER_CHROME5) == RENDER_CHROME5)
		{
			RenderType = RT_CHROME5;
		}
		else if ((RenderFlag &RENDER_CHROME6) == RENDER_CHROME6)
		{
			RenderType = RT_CHROME6;
		}
		else if ((RenderFlag &RENDER_CHROME7) == RENDER_CHROME7)
		{
			RenderType = RT_CHROME7;
		}
#ifdef PJH_NEW_CHROME
		else if ((RenderFlag &RENDER_CHROME8) == RENDER_CHROME8)
		{
			RenderType = RT_CHROME8;
		}
#endif
		else if ((RenderFlag &RENDER_METAL) == RENDER_METAL)
		{
			RenderType = RT_METAL;
		}
		else if ((RenderFlag &RENDER_OIL) == RENDER_OIL)
		{
			RenderType = RT_OIL;
		}
		else
		{
			RenderType = RT_METAL;
		}

		if ((RenderFlag&RENDER_CHROME3) == RENDER_CHROME3
			|| (RenderFlag&RENDER_CHROME4) == RENDER_CHROME4
			|| (RenderFlag&RENDER_CHROME5) == RENDER_CHROME5
			|| (RenderFlag&RENDER_CHROME7) == RENDER_CHROME7
#ifdef PJH_NEW_CHROME
			|| (RenderFlag&RENDER_CHROME8) == RENDER_CHROME8
#endif //PJH_NEW_CHROME
			)
		{
			if (Alpha < 0.99f)
			{
				bodyLight *= Alpha;
			}
			BlendType = BLEND::TXB_BLEND01;
		}
		else if ((RenderFlag&RENDER_BRIGHT) == RENDER_BRIGHT)
		{
			if (Alpha < 0.99f)
			{
				bodyLight *= Alpha;
			}
			BlendType = BLEND::TXB_BLEND01;
		}
		else if ((RenderFlag&RENDER_DARK) == RENDER_DARK)
		{
			BlendType = BLEND::TXB_BLENDMINUS;
		}
		else if ((RenderFlag&RENDER_LIGHTMAP) == RENDER_LIGHTMAP)
		{
			BlendType = BLEND::TXB_LIGHTMAP;
		}
		else
		{
			BlendType = BLEND::TXB_ALPHATEST_NOMASK;
		}

		if ((RenderFlag&RENDER_NODEPTH) == RENDER_NODEPTH)
		{
			EnableDepth = false;
		}

		if ((RenderFlag&RENDER_CHROME2) == RENDER_CHROME2 && MeshTexture == -1)
		{
			Texture = BITMAP_CHROME2;
		}
		else if ((RenderFlag&RENDER_CHROME3) == RENDER_CHROME3 && MeshTexture == -1)
		{
			Texture = BITMAP_CHROME2;
		}
		else if ((RenderFlag&RENDER_CHROME4) == RENDER_CHROME4 && MeshTexture == -1)
		{
			Texture = BITMAP_CHROME2;
		}
#ifdef PJH_NEW_CHROME
		else if ((RenderFlag&RENDER_CHROME8) == RENDER_CHROME8 && MeshTexture == -1)
		{
			Texture = BITMAP_CHROME9;
		}
#endif //PJH_NEW_CHROME
		else if ((RenderFlag&RENDER_CHROME6) == RENDER_CHROME6 && MeshTexture == -1)
		{
			Texture = BITMAP_CHROME6;
		}
		else if ((RenderFlag&RENDER_CHROME) == RENDER_CHROME && MeshTexture == -1)
		{
			Texture = BITMAP_CHROME;
		}
		else if ((RenderFlag&RENDER_METAL) == RENDER_METAL && MeshTexture == -1)
		{
			Texture = BITMAP_SHINY;
		}
	}
	else if (BlendMesh <= -2 || i == BlendMesh)
	{
		RenderType = RT_TEXTURE;

		if ((RenderFlag&RENDER_DARK) == RENDER_DARK)
		{
			BlendType = BLEND::TXB_BLENDMINUS;
		}
		else
		{
			BlendType = BLEND::TXB_BLEND01;
		}

		if ((RenderFlag&RENDER_NODEPTH) == RENDER_NODEPTH)
		{
			EnableDepth = false;
		}

		ThreadInfo.RenderLight = MU::Float4ToARGB(BodyLight(ThreadIndex) * BlendMeshLight, 1.0f);
		EnableLight = false;
	}
	else if ((RenderFlag&RENDER_TEXTURE) == RENDER_TEXTURE)
	{
		RenderType = RT_TEXTURE;

		if ((RenderFlag&RENDER_BRIGHT) == RENDER_BRIGHT)
		{
			BlendType = BLEND::TXB_BLEND01;
		}
		else if ((RenderFlag&RENDER_DARK) == RENDER_DARK)
		{
			BlendType = BLEND::TXB_BLENDMINUS;
		}
		else
		{
			BlendType = BLEND::TXB_ALPHATEST_BEGIN + ThreadInfo.AlphaTestIndex;
		}

		if ((RenderFlag&RENDER_NODEPTH) == RENDER_NODEPTH)
		{
			EnableDepth = false;
		}
	}
	else if ((RenderFlag&RENDER_BRIGHT) == RENDER_BRIGHT)
	{
// 		if (pBitmap->_Components == 4 || i == BlendMesh)
// 		{
// 			return;
// 		}

		RenderType = RT_COLOR;
		BlendType = BLEND::TXB_BLEND01;

		if ((RenderFlag&RENDER_NODEPTH) == RENDER_NODEPTH)
		{
			EnableDepth = false;
		}
	}
	else
	{
		RenderType = RT_TEXTURE;
		BlendType = BLEND::TXB_ALPHATEST_BEGIN + ThreadInfo.AlphaTestIndex;
	}

	if (RenderType == RT_TEXTURE &&
		EnableLight == true)
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(BodyLight(ThreadIndex), Alpha);
	}
	else if (RenderType >= RT_CHROME &&
		RenderType <= RT_OIL)
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(bodyLight, Alpha);
	}

	mu_uint16 ExtraFlag = ThreadInfo.ExtraFlag | (EnableDepth == true ? REXF_ENABLED_DEPTH : 0);

	if (RenderFlag & RENDER_DOPPELGANGER)
	{
		if (pBitmap->TextureInternal.Components != 4)
		{
			ExtraFlag |= REXF_DOPPELGANGER;
		}
	}

	if (RenderFlag & RENDER_BACKCULL)
	{
		ExtraFlag |= REXF_CULL_BACK;
	}

	if (RenderFlag & RENDER_FRONTCULL)
	{
		ExtraFlag |= REXF_CULL_FRONT;
	}

	if ((RenderFlag & RENDER_SHADOWMAP) == RENDER_SHADOWMAP)
	{
		if (RenderType == RT_TEXTURE)
		{
			RenderType = RT_TEXTURE_SHADOWMAP;
		}
		else if (RenderType == RT_COLOR)
		{
			RenderType = RT_COLOR_SHADOWMAP;
		}
	}
	else if ((RenderFlag & RENDER_WAVE) == RENDER_WAVE)
	{
		if (RenderType == RT_TEXTURE)
		{
			RenderType = RT_TEXTURE_WAVE;
		}
		else if (RenderType == RT_COLOR)
		{
			RenderType = RT_COLOR_WAVE;
		}
	}

	if (BlendType >= BLEND::TXB_ALPHATEST_BEGIN &&
		BlendType <= BLEND::TXB_ALPHATEST_END &&
		Alpha < 0.99f)
	{
		BlendType = BLEND::TXB_ALPHATEST_NONDEPTH;
	}

	if (BlendType >= BLEND::TXB_NON_DEPTH_BEGIN &&
		BlendType < BLEND::TXB_NON_DEPTH_END)
	{
		if ((RenderFlag & RENDER_PREBLEND) != 0)
		{
			switch (BlendType)
			{
			case BLEND::TXB_BLEND01:
				BlendType = BLEND::TXB_PRE_BLEND01;
				break;
			case BLEND::TXB_BLENDMINUS:
				BlendType = BLEND::TXB_PRE_BLENDMINUS;
				break;
			case BLEND::TXB_BLEND02:
				BlendType = BLEND::TXB_PRE_BLEND02;
				break;
			case BLEND::TXB_BLEND03:
				BlendType = BLEND::TXB_PRE_BLEND03;
				break;
			case BLEND::TXB_BLEND04:
				BlendType = BLEND::TXB_PRE_BLEND04;
				break;
			case BLEND::TXB_LIGHTMAP:
				BlendType = BLEND::TXB_PRE_LIGHTMAP;
				break;
			}
		}
	}

	//if(MU::GetAlpha(ThreadInfo.RenderLight) >= MU::To255(ThreadInfo.MinAlpha))
	{
		if (ThreadInfo.RenderGUI == true)
		{
			if (ClothMesh(ThreadIndex) != i)
			{
				mu_uint32 instanceIndex = g_Tasking._ModelInstanceCount.fetch_add(1);
				SHModelInstance *instanceData = g_Tasking.GetModelInstanceBuffer(instanceIndex);

				mu_float boneScale = (ThreadInfo.BoneScale - 1.0f);

				if (RenderType == RT_COLOR)
				{
					if (boneScale < 0.0f) boneScale -= 1.0f;
					else if (boneScale > 0.0f) boneScale += 1.0f;
				}

				const mu_float isTranslate = static_cast<mu_float>(ThreadInfo.Translate);
				SetModelInstance(instanceData,
					BodyOrigin(ThreadIndex) * isTranslate,
					EVector2((1.0f - isTranslate) + BodyScale(ThreadIndex) * isTranslate, boneScale),
					EVector2(LightEnable(ThreadIndex), ThreadInfo.MinAlpha),
					EVector4(EnableWave ? BlendMeshTexCoordU : 0.0f, EnableWave ? BlendMeshTexCoordV : 0.0f, ThreadInfo.EnableShadow, ThreadInfo.ShadowHeight),
					ThreadInfo.RenderLight,
					ThreadInfo.ShadowLight,
					ThreadInfo.SkeletalIndex);

				mu_uint32 renderSlotIndex = g_GUIRenderSlotCount++;
				MUGUIRenderSlot *renderSlot = &g_GUIRenderSlot[renderSlotIndex];

				renderSlot->Hash = RetrieveGUIHash(ThreadInfo.GUIRenderIndex, _ModelIndex, BlendType, i);

				renderSlot->MeshIndex = i;
				renderSlot->RenderType = RenderType;
				renderSlot->BlendType = BlendType;
				renderSlot->ExtraFlag = ExtraFlag;

				renderSlot->ModelIndex = _ModelIndex;
				renderSlot->Texture = Texture;

				renderSlot->InstanceIndex = instanceIndex;
			}
			else
			{
				// FIX Add support for cloths in GUI (Don't know if will be used)
			}
		}
		else
		{
			if (ClothMesh(ThreadIndex) != i)
			{
				mu_uint32 renderSlotIndex;

				if (BlendType >= BLEND::TXB_NON_DEPTH_BEGIN &&
					BlendType < BLEND::TXB_NON_DEPTH_END)
				{
					renderSlotIndex = g_PostRenderSlotCount.fetch_add(1);
				}
				else
				{
					renderSlotIndex = g_PreRenderSlotCount.fetch_add(1);
				}

				if (renderSlotIndex >= MAX_RENDERSLOT)
				{
					return;
				}

				mu_uint32 renderIndex;
				MURenderSlot *renderSlot;

				if (BlendType >= BLEND::TXB_NON_DEPTH_BEGIN &&
					BlendType < BLEND::TXB_NON_DEPTH_END)
				{
					renderIndex = BlendType == BLEND::TXB_ALPHATEST_NONDEPTH ? 0 : 1;
					renderSlot = &g_PostRenderSlot[renderSlotIndex];
				}
				else
				{
					renderIndex = 0;// ThreadInfo.RenderIndex++;
					renderSlot = &g_PreRenderSlot[renderSlotIndex];
					g_PreRenderSlotUsed[renderSlotIndex] = renderSlotIndex;
				}

				renderSlot->Hash = RetrieveRenderSlotHash(ThreadInfo.CulledType, ThreadInfo.ObjectIndex, renderIndex);

				renderSlot->Flags.SetDirect(
					MURenderSlot::eRSF_SHADOW * (mu_int32)ThreadInfo.EnableShadow |
					MURenderSlot::eRSF_WAVE * (mu_int32)EnableWave |
					MURenderSlot::eRSF_LIGHT * (mu_int32)EnableLight |
					MURenderSlot::eRSF_TRANSLATE * (mu_int32)ThreadInfo.Translate
				);

				// 			renderSlot->EnableShadow = ThreadInfo.EnableShadow;
				// 			renderSlot->EnableWave = EnableWave;
				// 			renderSlot->EnableLight = EnableLight;
				// 			renderSlot->Translate = ThreadInfo.Translate;

				renderSlot->CulledType = ThreadInfo.CulledType;
				renderSlot->RenderIndex = renderIndex;
				renderSlot->MeshIndex = i;
				renderSlot->RenderType = RenderType;
				renderSlot->BlendType = BlendType;
				renderSlot->ExtraFlag = ExtraFlag;

				renderSlot->ModelIndex = _ModelIndex;
				renderSlot->Texture = Texture;

				renderSlot->SkeletalIndex = ThreadInfo.SkeletalIndex;

				renderSlot->MinAlpha = ThreadInfo.MinAlpha;
				renderSlot->BlendMeshLight = BlendMeshLight;
				renderSlot->BlendMeshTexCoordU = EnableWave ? BlendMeshTexCoordU : 0.0f;
				renderSlot->BlendMeshTexCoordV = EnableWave ? BlendMeshTexCoordV : 0.0f;
				renderSlot->ShadowHeight = ThreadInfo.ShadowHeight;

				renderSlot->BoneScale = ThreadInfo.BoneScale;
				renderSlot->BodyOrigin = BodyOrigin(ThreadIndex);
				renderSlot->BodyAngle = BodyAngle(ThreadIndex);
				renderSlot->BodyScale = BodyScale(ThreadIndex);
				renderSlot->BodyLight = ThreadInfo.RenderLight;
				renderSlot->ShadowLight = ThreadInfo.ShadowLight;

#if 0
				renderSlot->EnableShadow = true;
				renderSlot->ShadowLight = EVector4(0.5f, 0.5f, 0.2f, 1.0f);
				renderSlot->ShadowHeight = renderSlot->BodyOrigin[2];
#endif
			}
			else
			{
				mu_uint32 instanceIndex = g_Tasking._ModelInstanceCount.fetch_add(1);
				SHModelInstance *instanceData = g_Tasking.GetModelInstanceBuffer(instanceIndex);

				mu_float boneScale = (ThreadInfo.BoneScale - 1.0f);

				if (RenderType == RT_COLOR)
				{
					if (boneScale < 0.0f) boneScale -= 1.0f;
					else if (boneScale > 0.0f) boneScale += 1.0f;
				}

				SetModelInstance(instanceData,
					EVector3(0.0f, 0.0f, 0.0f),
					EVector2(1.0f, boneScale),
					EVector2(LightEnable(ThreadIndex), ThreadInfo.MinAlpha),
					EVector4(EnableWave ? BlendMeshTexCoordU : 0.0f, EnableWave ? BlendMeshTexCoordV : 0.0f, ThreadInfo.EnableShadow, ThreadInfo.ShadowHeight),
					ThreadInfo.RenderLight,
					ThreadInfo.ShadowLight,
					0);

				mu_uint32 renderSlotIndex = g_ClothRenderSlotCount.fetch_add(1);
				MUClothRenderSlot *renderSlot = &g_ClothRenderSlot[renderSlotIndex];
				g_ClothRenderSlotUsed[renderSlotIndex] = renderSlotIndex;

				mu_uint32 renderIndex;

				if (BlendType >= BLEND::TXB_NON_DEPTH_BEGIN &&
					BlendType < BLEND::TXB_NON_DEPTH_END)
				{
					renderIndex = BlendType == BLEND::TXB_ALPHATEST_NONDEPTH ? 0 : 1;
				}
				else
				{
					renderIndex = 0;
				}

				renderSlot->HighHash = RetrieveClothHighHash(renderIndex, _ModelIndex, _MeshTexture[i]);
				renderSlot->LowHash = RetrieveClothLowHash(RenderType, BlendType, ExtraFlag, i);

				renderSlot->EnableShadow = ThreadInfo.EnableShadow;

				renderSlot->CulledType = ThreadInfo.CulledType;
				renderSlot->RenderIndex = renderIndex;
				renderSlot->MeshIndex = i;
				renderSlot->RenderType = RenderType;
				renderSlot->BlendType = BlendType;
				renderSlot->ExtraFlag = ExtraFlag;

				renderSlot->ModelIndex = _ModelIndex;
				renderSlot->Texture = _MeshTexture[i];

				renderSlot->BaseIndex = ClothVerticesIndex(ThreadIndex);
				renderSlot->IndexCount = ClothVerticesCount(ThreadIndex);
				renderSlot->InstanceIndex = instanceIndex;
			}
		}
	}
}

void MUModel::RenderBody(const mu_uint32 ThreadIndex, mu_int32 RenderFlag, mu_float Alpha, mu_int32 BlendMesh, mu_float BlendMeshLight, mu_float BlendMeshTexCoordU, mu_float BlendMeshTexCoordV, mu_int32 HiddenMesh, mu_int32 Texture)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (_MeshCount == 0) return;

	mu_int32 iBlendMesh = BlendMesh;
	if (!LightEnable(ThreadIndex))
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(BodyLight(ThreadIndex), Alpha);
	}

	for (mu_int32 i = 0; i < _MeshCount; ++i)
	{
		iBlendMesh = BlendMesh;

		Model::MeshExtra1 *me = &_MeshExtra[i];

		if (me->IsUsed == true)
		{
			if (me->IsHidden == false && i != HiddenMesh)
			{
				if (me->IsBright == true)
				{
					iBlendMesh = i;
				}
				RenderMesh(ThreadIndex, i, RenderFlag, Alpha, iBlendMesh, BlendMeshLight, BlendMeshTexCoordU, BlendMeshTexCoordV, Texture);

				if (me->ShadowMapping == SHADOW_RENDER_COLOR)
				{
					mu_uint32 BackupLight = ThreadInfo.RenderLight;
					ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, Alpha);
					RenderMesh(ThreadIndex, i, RENDER_COLOR | RENDER_SHADOWMAP, Alpha, iBlendMesh, BlendMeshLight, BlendMeshTexCoordU, BlendMeshTexCoordV);
					ThreadInfo.RenderLight = BackupLight;
				}
				else if (me->ShadowMapping == SHADOW_RENDER_TEXTURE)
				{
					mu_uint32 BackupLight = ThreadInfo.RenderLight;
					ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, Alpha);
					RenderMesh(ThreadIndex, i, RENDER_TEXTURE | RENDER_SHADOWMAP, Alpha, iBlendMesh, BlendMeshLight, BlendMeshTexCoordU, BlendMeshTexCoordV);
					ThreadInfo.RenderLight = BackupLight;
				}
			}
		}
		else
		{
			if (i != HiddenMesh)
			{
				RenderMesh(ThreadIndex, i, RenderFlag, Alpha, iBlendMesh, BlendMeshLight, BlendMeshTexCoordU, BlendMeshTexCoordV, Texture);
			}
		}
	}
}

#ifdef USE_DUAL_QUATERNION
void MUModel::RenderMeshEffect(const mu_uint32 ThreadIndex, const EDualQuat *BoneMatrix, mu_int32 i, mu_int32 iType, mu_int32 iSubType, EVector3 Angle, void* obj)
#else
void MUModel::RenderMeshEffect(const mu_uint32 ThreadIndex, const EMatrix4 *BoneMatrix, mu_int32 i, mu_int32 iType, mu_int32 iSubType, EVector3 Angle, void* obj)
#endif
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (MU_UPDATECOUNTER == 0 || i >= _MeshCount || i < 0)
	{
		return;
	}

	Model::Mesh1 *m = &_Mesh[i];
	Model::MeshData1 *md = &_MeshData[i];
	if (m->NumTriangles == 0)
	{
		return;
	}

	EVector3 angle = EVector3(0.0f, 0.0f, 0.0f), Light = EVector3(1.0f, 1.0f, 1.0f);
	mu_int32 iEffectCount = 0;

	TransformVertices(ThreadIndex, i, BoneMatrix);

	EVector3 *vertexTransform = &ThreadInfo.VertexTransform[md->VertexStart];

	for (mu_uint32 j = 0; j < m->NumTriangles; ++j)
	{
		Model::MeshTriangle1 *tp = &m->Triangles[j];

		for (mu_uint32 k = 0; k < 3; ++k)
		{
			mu_uint32 vi = tp->VertexIndex[k];

			switch (iType)
			{
			case MODEL_STONE_COFFIN:
				if (iSubType == 0)
				{
					if ((GetLargeRand(2)) == 0)
					{
						CreateEffect(ThreadIndex, MODEL_STONE_COFFIN + 1, vertexTransform[vi], angle, Light);
					}
					if ((GetLargeRand(10)) == 0)
					{
						CreateEffect(ThreadIndex, MODEL_STONE_COFFIN, vertexTransform[vi], angle, Light);
					}
				}
				else if (iSubType == 1)
				{
					CreateEffect(ThreadIndex, MODEL_STONE_COFFIN + 1, vertexTransform[vi], angle, Light, 2);
				}
				else if (iSubType == 2)
				{
					CreateEffect(ThreadIndex, MODEL_STONE_COFFIN + 1, vertexTransform[vi], angle, Light, 3);
				}
				else if (iSubType == 3)
				{
					CreateEffect(ThreadIndex, MODEL_STONE_COFFIN + GetLargeRand(2), vertexTransform[vi], angle, Light, 4);
				}
				break;
			case MODEL_GATE:
				if (iSubType == 1)
				{
					Vector(0.2f, 0.2f, 0.2f, Light);
					if ((GetLargeRand(5)) == 0)
					{
						CreateEffect(ThreadIndex, MODEL_GATE + 1, vertexTransform[vi], angle, Light, 2);
					}
					if ((GetLargeRand(10)) == 0)
					{
						CreateEffect(ThreadIndex, MODEL_GATE, vertexTransform[vi], angle, Light, 2);
					}
				}
				else if (iSubType == 0)
				{
					Vector(0.2f, 0.2f, 0.2f, Light);
					if ((GetLargeRand(12)) == 0)
					{
						CreateEffect(ThreadIndex, MODEL_GATE + 1, vertexTransform[vi], angle, Light);
					}
					if ((GetLargeRand(50)) == 0)
					{
						CreateEffect(ThreadIndex, MODEL_GATE, vertexTransform[vi], angle, Light);
					}
				}
				break;
			case MODEL_BIG_STONE_PART1:
				if ((GetLargeRand(3)) == 0)
				{
					CreateEffect(ThreadIndex, MODEL_BIG_STONE_PART1 + GetLargeRand(2), vertexTransform[vi], angle, Light, 1);
				}
				break;

			case MODEL_BIG_STONE_PART2:
				if ((GetLargeRand(3)) == 0)
				{
					CreateEffect(ThreadIndex, MODEL_BIG_STONE_PART1 + GetLargeRand(2), vertexTransform[vi], angle, Light);
				}
				break;

			case MODEL_WALL_PART1:
				if ((GetLargeRand(3)) == 0)
				{
					CreateEffect(ThreadIndex, MODEL_WALL_PART1 + GetLargeRand(2), vertexTransform[vi], angle, Light);
				}
				break;

			case MODEL_GATE_PART1:
				Vector(0.2f, 0.2f, 0.2f, Light);
				if ((GetLargeRand(12)) == 0)
				{
					CreateEffect(ThreadIndex, MODEL_GATE_PART1 + 1, vertexTransform[vi], angle, Light);
				}
				if ((GetLargeRand(40)) == 0)
				{
					CreateEffect(ThreadIndex, MODEL_GATE_PART1, vertexTransform[vi], angle, Light);
				}
				if ((GetLargeRand(40)) == 0)
				{
					CreateEffect(ThreadIndex, MODEL_GATE_PART1 + 2, vertexTransform[vi], angle, Light);
				}
				break;
			case MODEL_GOLEM_STONE:
				if ((GetLargeRand(45)) == 0 && iEffectCount < 20)
				{
					if (iSubType == 0)
					{
						CreateEffect(ThreadIndex, MODEL_GOLEM_STONE, vertexTransform[vi], angle, Light);
					}
					else if (iSubType == 1)
					{
						CreateEffect(ThreadIndex, MODEL_BIG_STONE_PART1, vertexTransform[vi], angle, Light, 2);
						CreateEffect(ThreadIndex, MODEL_BIG_STONE_PART2, vertexTransform[vi], angle, Light, 2);
					}
					++iEffectCount;
				}
				break;
			case MODEL_SKIN_SHELL:
				if ((GetLargeRand(8)) == 0)
				{
					CreateEffect(ThreadIndex, MODEL_SKIN_SHELL, vertexTransform[vi], angle, Light, iSubType);
				}
				break;
			case BITMAP_LIGHT:
				Vector(0.08f, 0.08f, 0.08f, Light);
				if (iSubType == 0)
				{
					CreateSprite(BITMAP_LIGHT, vertexTransform[vi], BodyScale(ThreadIndex), Light, nullptr);
				}
				else if (iSubType == 1)
				{
					Vector(1.0f, 0.8f, 0.2f, Light);
					if ((j % 22) == 0)
					{
						OBJECT* o = (OBJECT*)obj;

						angle[0] = -(mu_float)(GetLargeRand(90));
						angle[1] = 0.0f;
						angle[2] = Angle[2] + (mu_float)(GetLargeRand(120) - 60);
						CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, vertexTransform[vi], o->Position, angle, 13, o, 20.0f, 0, 0);
					}
				}
				break;
			case BITMAP_BUBBLE:
				Vector(1.0f, 1.0f, 1.0f, Light);
				if ((GetLargeRand(30)) == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_BUBBLE, vertexTransform[vi], angle, Light, 2);
				}
				break;
			}
		}
	}
}

#ifdef USE_DUAL_QUATERNION
void MUModel::TransformVertices(const mu_uint32 ThreadIndex, mu_int32 Mesh, const EDualQuat *BoneMatrix)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	for (mu_int32 i = 0; i < _MeshCount; ++i)
	{
		if (Mesh != -1 && i != Mesh) continue;

		Model::Mesh1 *m = &_Mesh[i];
		Model::MeshData1 *md = &_MeshData[i];

		mu_int32 beginVertex = md->VertexStart;
		EVector3 *vertexTransform = &ThreadInfo.VertexTransform[beginVertex];

		if (ThreadInfo.Translate)
		{
			for (mu_int32 j = 0; j < m->NumVertices; ++j)
			{
				Model::MeshVertex1 &v = m->Vertices[j];
				EVector3 &vp = vertexTransform[j];
				const EDualQuat &bone = BoneMatrix[v.Node];

				vp = ((bone.Rotation * vp) * bone.Scale) * ThreadInfo.BoneScale;
				vp += bone.Position;
				VectorScale(vp, BodyScale(ThreadIndex), vp);
				VectorAdd(vp, BodyOrigin(ThreadIndex), vp);
			}
		}
		else
		{
			for (mu_int32 j = 0; j < m->NumVertices; ++j)
			{
				Model::MeshVertex1 &v = m->Vertices[j];
				EVector3 &vp = vertexTransform[j];
				const EDualQuat &bone = BoneMatrix[v.Node];

				vp = ((bone.Rotation * vp) * bone.Scale) * ThreadInfo.BoneScale;
				vp += bone.Position;
			}
		}
	}
}
#else
void MUModel::TransformVertices(const mu_uint32 ThreadIndex, mu_int32 Mesh, const EMatrix4 *BoneMatrix)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	for (mu_int32 i = 0; i < _MeshCount; ++i)
	{
		if (Mesh != -1 && i != Mesh) continue;

		Model::Mesh1 *m = &_Mesh[i];
		Model::MeshData1 *md = &_MeshData[i];

		mu_int32 beginVertex = md->VertexStart;
		EVector3 *vertexTransform = &ThreadInfo.VertexTransform[beginVertex];

		if (ThreadInfo.Translate)
		{
			for (mu_int32 j = 0; j < m->NumVertices; ++j)
			{
				Model::MeshVertex1 &v = m->Vertices[j];
				EVector3 &vp = vertexTransform[j];

				VectorRotate(v.Position, BoneMatrix[v.Node], vp);
				vp[0] = vp[0] * ThreadInfo.BoneScale + BoneMatrix[v.Node][3][0];
				vp[1] = vp[1] * ThreadInfo.BoneScale + BoneMatrix[v.Node][3][1];
				vp[2] = vp[2] * ThreadInfo.BoneScale + BoneMatrix[v.Node][3][2];
				VectorScale(vp, BodyScale(ThreadIndex), vp);
				VectorAdd(vp, BodyOrigin(ThreadIndex), vp);
			}
		}
		else
		{
			for (mu_int32 j = 0; j < m->NumVertices; ++j)
			{
				Model::MeshVertex1 &v = m->Vertices[j];
				EVector3 &vp = vertexTransform[j];

				VectorRotate(v.Position, BoneMatrix[v.Node], vp);
				vp[0] = vp[0] * ThreadInfo.BoneScale + BoneMatrix[v.Node][3][0];
				vp[1] = vp[1] * ThreadInfo.BoneScale + BoneMatrix[v.Node][3][1];
				vp[2] = vp[2] * ThreadInfo.BoneScale + BoneMatrix[v.Node][3][2];
			}
		}
	}
}
#endif

void MUModel::TransformByObjectBone(const mu_uint32 ThreadIndex, EVector3 &vResultPosition, OBJECT * pObject, mu_int32 iBoneNumber, EVector3 *vRelativePosition) const
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

#ifdef USE_DUAL_QUATERNION
	EDualQuat TransformMatrix;
#else
	EMatrix4 TransformMatrix;
#endif
	if (pObject->BoneTransform != nullptr)
	{
		TransformMatrix = pObject->BoneTransform[MODELS::Data[pObject->Type].GetBoneIndex(iBoneNumber)];
	}
	else
	{
		TransformMatrix = ThreadInfo.BoneTransform[MODELS::Data[pObject->Type].GetBoneIndex(iBoneNumber)];
	}

	EVector3 vTemp;
	if (vRelativePosition == nullptr)
	{
#ifdef USE_DUAL_QUATERNION
		vTemp = TransformMatrix.Position;
#else
		vTemp = EVector3(TransformMatrix[3]);
#endif
	}
	else
	{
#ifdef USE_DUAL_QUATERNION
		TransformMatrix.TransformOut(*vRelativePosition, vTemp);
#else
		VectorTransform(*vRelativePosition, TransformMatrix, vTemp);
#endif
	}
	VectorScale(vTemp, pObject->Scale, vTemp);
	VectorAdd(vTemp, pObject->Position, vResultPosition);
}

void MUModel::TransformByObjectBoneDirect(const mu_uint32 ThreadIndex, EVector3 &vResultPosition, OBJECT * pObject, mu_int32 iBoneNumber, EVector3 *vRelativePosition) const
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

#ifdef USE_DUAL_QUATERNION
	EDualQuat TransformMatrix;
#else
	EMatrix4 TransformMatrix;
#endif
	if (pObject->BoneTransform != nullptr)
	{
		TransformMatrix = pObject->BoneTransform[iBoneNumber];
	}
	else
	{
		TransformMatrix = ThreadInfo.BoneTransform[iBoneNumber];
	}

	EVector3 vTemp;
	if (vRelativePosition == nullptr)
	{
#ifdef USE_DUAL_QUATERNION
		vTemp = TransformMatrix.Position;
#else
		vTemp = EVector3(TransformMatrix[3]);
#endif
	}
	else
	{
#ifdef USE_DUAL_QUATERNION
		TransformMatrix.TransformOut(*vRelativePosition, vTemp);
#else
		VectorTransform(*vRelativePosition, TransformMatrix, vTemp);
#endif
	}
	VectorScale(vTemp, pObject->Scale, vTemp);
	VectorAdd(vTemp, pObject->Position, vResultPosition);
}

#ifdef USE_DUAL_QUATERNION
void MUModel::TransformByBoneMatrix(const mu_uint32 ThreadIndex, EVector3 &vResultPosition, const EDualQuat &BoneMatrix, EVector3 *vWorldPosition, EVector3 *vRelativePosition)
{
	EVector3 vTemp;
	if (vRelativePosition == nullptr)
	{
		vTemp = BoneMatrix.Position;
	}
	else
	{
		BoneMatrix.TransformOut(*vRelativePosition, vTemp);
	}

	if (vWorldPosition != nullptr)
	{
		VectorScale(vTemp, BodyScale(ThreadIndex), vTemp);
		VectorAdd(vTemp, *vWorldPosition, vResultPosition);
	}
	else
	{
		VectorScale(vTemp, BodyScale(ThreadIndex), vResultPosition);
	}
}

void MUModel::TransformPosition(const mu_uint32 ThreadIndex, const EDualQuat &Matrix, EVector3 Position, EVector3 &WorldPosition, mu_boolean Translate)
{
	if (Translate)
	{
		EVector3 p;
		Matrix.TransformOut(Position, p);
		VectorScale(p, BodyScale(ThreadIndex), p);
		VectorAdd(p, BodyOrigin(ThreadIndex), WorldPosition);
	}
	else
	{
		Matrix.TransformOut(Position, WorldPosition);
	}
}

void MUModel::RotationPosition(const mu_uint32 ThreadIndex, const EDualQuat &Matrix, EVector3 Position, EVector3 &WorldPosition)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 p;
	Matrix.TransformNormalOut(Position, p);
	VectorScale(p, BodyScale(ThreadIndex), WorldPosition);
	ThreadInfo.ParentMatrix = Matrix;
}
#else
void MUModel::TransformByBoneMatrix(const mu_uint32 ThreadIndex, EVector3 &vResultPosition, const EMatrix4 &BoneMatrix, EVector3 *vWorldPosition, EVector3 *vRelativePosition)
{
	EVector3 vTemp;
	if (vRelativePosition == nullptr)
	{
		vTemp = EVector3(BoneMatrix[3]);
	}
	else
	{
		VectorTransform(*vRelativePosition, BoneMatrix, vTemp);
	}

	if (vWorldPosition != nullptr)
	{
		VectorScale(vTemp, BodyScale(ThreadIndex), vTemp);
		VectorAdd(vTemp, *vWorldPosition, vResultPosition);
	}
	else
	{
		VectorScale(vTemp, BodyScale(ThreadIndex), vResultPosition);
	}
}

void MUModel::TransformPosition(const mu_uint32 ThreadIndex, const EMatrix4 &Matrix, EVector3 Position, EVector3 &WorldPosition, mu_boolean Translate)
{
	if (Translate)
	{
		EVector3 p;
		VectorTransform(Position, Matrix, p);
		VectorScale(p, BodyScale(ThreadIndex), p);
		VectorAdd(p, BodyOrigin(ThreadIndex), WorldPosition);
	}
	else
	{
		VectorTransform(Position, Matrix, WorldPosition);
	}
}

void MUModel::RotationPosition(const mu_uint32 ThreadIndex, const EMatrix4 &Matrix, EVector3 Position, EVector3 &WorldPosition)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 p;
	VectorRotate(Position, Matrix, p);
	VectorScale(p, BodyScale(ThreadIndex), WorldPosition);
	ThreadInfo.ParentMatrix = Matrix;
}
#endif

void MUModel::AnimationTransformWithAttachHighModel_usingGlobalTM(
	mu_uint32 ThreadIndex,
	OBJECT* oHighHierarchyModel,
	MUModel* bmdHighHierarchyModel,
	mu_int32 iBoneNumberHighHierarchyModel,
	EVector3 *vOutPosHighHiearachyModelBone,
	EVector3 *arrOutSetfAllBonePositions,
	mu_boolean bApplyTMtoVertices)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3	Temp, v3Position;
	OBB_t		OBB;

	Vector(0.0f, 0.0f, 0.0f, Temp);

#ifdef USE_DUAL_QUATERNION
	EDualQuat tmBoneHierarchicalObject = oHighHierarchyModel->BoneTransform[bmdHighHierarchyModel->GetBoneIndex(iBoneNumberHighHierarchyModel)];
#else
	EMatrix4 tmBoneHierarchicalObject = oHighHierarchyModel->BoneTransform[bmdHighHierarchyModel->GetBoneIndex(iBoneNumberHighHierarchyModel)];
#endif
	BodyScale(ThreadIndex) = oHighHierarchyModel->Scale;

#ifdef USE_DUAL_QUATERNION
	tmBoneHierarchicalObject.Position *= BodyScale(ThreadIndex);

	if (nullptr != vOutPosHighHiearachyModelBone)
	{
		*vOutPosHighHiearachyModelBone = tmBoneHierarchicalObject.Position;
	}
#else
	tmBoneHierarchicalObject[3][0] *= BodyScale(ThreadIndex);
	tmBoneHierarchicalObject[3][1] *= BodyScale(ThreadIndex);
	tmBoneHierarchicalObject[3][2] *= BodyScale(ThreadIndex);

	if (nullptr != vOutPosHighHiearachyModelBone)
	{
		*vOutPosHighHiearachyModelBone = EVector3(tmBoneHierarchicalObject[3]);
	}
#endif

	VectorCopy(oHighHierarchyModel->Position, v3Position);

	Animation(ThreadIndex, ThreadInfo.BoneTransform2, 0, 0, 0, Temp, Temp, false, false);

	for (mu_int32 i_ = 0; i_ < _BoneNonDummyEnd; ++i_)
	{
#ifdef USE_DUAL_QUATERNION
		const EDualQuat &In = ThreadInfo.BoneTransform2[i_];
		EDualQuat &Out = ThreadInfo.BoneTransform[i_];
		tmBoneHierarchicalObject.MixOut(In, Out);
		Out.Position += v3Position;
		arrOutSetfAllBonePositions[i_] = Out.Position;
#else
		R_ConcatTransforms(tmBoneHierarchicalObject, ThreadInfo.BoneTransform2[i_], ThreadInfo.BoneTransform[i_]);

		ThreadInfo.BoneTransform[i_][3] += EVector4(v3Position, 0.0f);
		arrOutSetfAllBonePositions[i_] = EVector3(ThreadInfo.BoneTransform[i_][3]);
#endif
	}

	if (true == bApplyTMtoVertices)
	{
		Transform(ThreadIndex, Temp, Temp, &OBB, false);
	}
}

void MUModel::AnimationTransformWithAttachHighModel(
	mu_uint32 ThreadIndex,
	OBJECT* oHighHierarchyModel,
	MUModel* bmdHighHierarchyModel,
	mu_int32 iBoneNumberHighHierarchyModel,
	EVector3 *vOutPosHighHiearachyModelBone,
	EVector3 *arrOutSetfAllBonePositions)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3	Temp, v3Position;

	Vector(0.0f, 0.0f, 0.0f, Temp);

#ifdef USE_DUAL_QUATERNION
	EDualQuat tmBoneHierarchicalObject = oHighHierarchyModel->BoneTransform[bmdHighHierarchyModel->GetBoneIndex(iBoneNumberHighHierarchyModel)];
#else
	EMatrix4 tmBoneHierarchicalObject = oHighHierarchyModel->BoneTransform[bmdHighHierarchyModel->GetBoneIndex(iBoneNumberHighHierarchyModel)];
#endif

	BodyScale(ThreadIndex) = oHighHierarchyModel->Scale;

#ifdef USE_DUAL_QUATERNION
	tmBoneHierarchicalObject.Position *= BodyScale(ThreadIndex);

	if (nullptr != vOutPosHighHiearachyModelBone)
	{
		*vOutPosHighHiearachyModelBone = tmBoneHierarchicalObject.Position;
	}
#else
	tmBoneHierarchicalObject[3][0] *= BodyScale(ThreadIndex);
	tmBoneHierarchicalObject[3][1] *= BodyScale(ThreadIndex);
	tmBoneHierarchicalObject[3][2] *= BodyScale(ThreadIndex);

	if (nullptr != vOutPosHighHiearachyModelBone)
	{
		*vOutPosHighHiearachyModelBone = EVector3(tmBoneHierarchicalObject[3]);
	}
#endif

	VectorCopy(oHighHierarchyModel->Position, v3Position);

	Animation(ThreadIndex, ThreadInfo.BoneTransform2, 0, 0, 0, Temp, Temp, false, false);

#ifdef USE_DUAL_QUATERNION
	EDualQuat resultBone;
#else
	EMatrix4 resultBone;
#endif

	for (mu_int32 i_ = 0; i_ < _BoneNonDummyEnd; ++i_)
	{
#ifdef USE_DUAL_QUATERNION
		const EDualQuat &In = ThreadInfo.BoneTransform2[i_];
		tmBoneHierarchicalObject.MixOut(In, resultBone);
		arrOutSetfAllBonePositions[i_] = resultBone.Position + v3Position;
#else
		R_ConcatTransforms(tmBoneHierarchicalObject, ThreadInfo.BoneTransform2[i_], resultBone);

		resultBone[3] += EVector4(v3Position, 0.0f);

		arrOutSetfAllBonePositions[i_] = EVector3(resultBone[3]);
#endif
	}
}

void MUModel::AnimationTransformOnlySelf(
	mu_uint32 ThreadIndex,
	EVector3 *arrOutSetfAllBonePositions,
	const OBJECT* oSelf)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3		Temp;

	Vector(0.0f, 0.0f, 0.0f, Temp);

	Animation(ThreadIndex, ThreadInfo.BoneTransform2, oSelf->AnimationFrame, oSelf->PriorAnimationFrame, oSelf->PriorAction, (const_cast<OBJECT*>(oSelf))->Angle, Temp, false, true);

	for (mu_int32 i_ = 0; i_ < _BoneNonDummyEnd; ++i_)
	{
#ifdef USE_DUAL_QUATERNION
		arrOutSetfAllBonePositions[i_] = ThreadInfo.BoneTransform2[i_].Position;
#else
		arrOutSetfAllBonePositions[i_] = EVector3(ThreadInfo.BoneTransform2[i_][3]);
#endif
	}
}

void MUModel::AnimationTransformOnlySelf(
	mu_uint32 ThreadIndex,
	EVector3 *arrOutSetfAllBonePositions,
	const EVector3 &v3Angle,
	const EVector3 &v3Position,
	const mu_float &fScale,
	OBJECT* oRefAnimation,
	const mu_float fFrameArea,
	const mu_float fWeight)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3	v3RootAngle = v3Angle;
	EVector3	Temp;

	Vector(0.0f, 0.0f, 0.0f, Temp);

	if (nullptr == oRefAnimation)
	{
		Animation(ThreadIndex, ThreadInfo.BoneTransform2, 0, 0, 0, v3RootAngle, Temp, false, true);
	}
	else
	{
		mu_float fAnimationFrame = oRefAnimation->AnimationFrame,
			fPiriorAnimationFrame = oRefAnimation->PriorAnimationFrame;
		mu_uint16 iPiriorAction = oRefAnimation->PriorAction;

		if (fWeight >= 0.0f && fFrameArea > 0.0f)
		{
			mu_float fAnimationFrameStart = fAnimationFrame - fFrameArea;
			mu_float fAnimationFrameEnd = fAnimationFrame;
			LInterpolationF(fAnimationFrame, fAnimationFrameStart, fAnimationFrameEnd, fWeight);
		}

		Animation(ThreadIndex,
			ThreadInfo.BoneTransform2,
			fAnimationFrame,
			fPiriorAnimationFrame,
			iPiriorAction,
			v3RootAngle, Temp, false, true);
	}

	for (mu_int32 i_ = 0; i_ < _BoneNonDummyEnd; ++i_)
	{
#ifdef USE_DUAL_QUATERNION
		arrOutSetfAllBonePositions[i_] = ThreadInfo.BoneTransform2[i_].Position;
#else
		arrOutSetfAllBonePositions[i_] = EVector3(ThreadInfo.BoneTransform2[i_][3]);
#endif
	}
}