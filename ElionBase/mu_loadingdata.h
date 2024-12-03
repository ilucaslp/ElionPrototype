#ifndef __MU_LOADING_DATA_H__
#define __MU_LOADING_DATA_H__

#pragma once

namespace MULoadingType
{
	enum : mu_uint8
	{
		LOADING_SHADER,
		LOADING_TEXTURE,
		LOADING_TEXTURE_RUNTIME,
		LOADING_MODEL,
		LOADING_MODEL_RUNTIME,
		LOADING_SOUND,
		LOADING_DATA,
		LOADING_SKIP, // Used for load on runtime and not at begin
	};
};

namespace MULoadingSubType
{
	enum : mu_uint8
	{
		ONCE,
		RUNTIME,
	};
};

namespace LOADING_STATUS
{
	enum : mu_uint8
	{
		NOT_LOADED = 0,
		SUCCESS,
		FAILED_OPEN,
		FAILED_READ,
		FAILED_DECRYPT,
		FAILED_LOAD,
	};
};

namespace LOADING_COUNTER
{
	enum
	{
		CT_INTERFACE = 0,
		CT_TEXTURES = 1,
		CT_MODELS = 2,
		CT_SOUNDS = 3,
		CT_DATA = 4,

		MAX,
	};
};

struct MULoadingData
{
	MULoadingData(const mu_uint8 type,
		const mu_uint8 subType,
		const mu_uint16 index,
		void *data,
		const mu_text *path,
		const mu_uint32 counter,
		const mu_uint8 status,
		const mu_uint8 channels,
		const mu_boolean enable3D) :
		Enable3D(enable3D),
		Type(type),
		SubType(subType),
		Channels(channels),
		Status(status),
		Index(index),
		Counter(counter),
		Data(data),
		Path(path)
	{
	}
	
	const mu_boolean Enable3D;

	const mu_uint8 Type;
	const mu_uint8 SubType;
	const mu_uint8 Channels;

	mu_uint8 Status;

	const mu_uint16 Index;
	
	const mu_uint32 Counter;

	void *Data;
	const mu_text *Path;
};

#define DEFINE_LOAD(Index, Path) \
		MULoadingData(Type, MULoadingSubType::ONCE, Index, Data, Path, Counter, LOADING_STATUS::NOT_LOADED, 0, false),

#define MODEL_LOAD(Index, Path) \
		MULoadingData(MULoadingType::LOADING_MODEL, MULoadingSubType::ONCE, Index, Data, Path, Counter, LOADING_STATUS::NOT_LOADED, 0, false),

#define TEXTURE_LOAD(Index, Path) \
		MULoadingData(MULoadingType::LOADING_TEXTURE, MULoadingSubType::ONCE, Index, Data, Path, Counter, LOADING_STATUS::NOT_LOADED, 0, false),

#define DEFINE_RUNTIME(Index, Path) \
		MULoadingData(Type, MULoadingSubType::RUNTIME, Index, Data, Path, Counter, LOADING_STATUS::NOT_LOADED, 0, false),

#define SOUND_LOAD(Index, Path, Channel, Enable3D) \
		MULoadingData(Type, MULoadingSubType::ONCE, Index, Data, Path, Counter, LOADING_STATUS::NOT_LOADED, Channel, Enable3D),

#define SOUND_RUNTIME(Index, Path, Channel, Enable3D) \
		MULoadingData(Type, MULoadingSubType::RUNTIME, Index, Data, Path, Counter, LOADING_STATUS::NOT_LOADED, Channel, Enable3D),

#define DEFINE_GAMEDATA(Index) \
		MULoadingData(MULoadingType::LOADING_DATA, MULoadingSubType::ONCE, Index, nullptr, nullptr, LOADING_COUNTER::CT_DATA, LOADING_STATUS::NOT_LOADED, 0, false),

#define DEFINE_SHADERDATA() \
		MULoadingData(MULoadingType::LOADING_SHADER, MULoadingSubType::ONCE, 0, nullptr, nullptr, LOADING_COUNTER::CT_DATA, LOADING_STATUS::NOT_LOADED, 0, false),

// Game Data List
#include "mu_interfacelist.h"
#include "mu_texturelist.h"
#include "mu_modellist.h"
#include "mu_soundlist.h"
#include "mu_datalist.h"

namespace LoadingData
{
	extern mu_atomic_uint32_t Counter[LOADING_COUNTER::MAX];
};

#endif