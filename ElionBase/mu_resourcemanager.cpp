#include "stdafx.h"
#include <shared_json.h>

#if defined(WIN32) || defined(WIN64)
#include <Shlobj.h>
#pragma comment(lib, "Shell32.lib")
#endif

#ifdef __ANDROID__
#include <jni.h>
#endif

#define JSON_BUFFER (256*1024)

mu_float FWorldTime = 0.0f;
mu_int64 IWorldTime = 0;
mu_int32 ChatTime = 0;
mu_int32 MacroTime = 0;
mu_uint32 WaterTextureNumber = 0;
mu_uint32 MoveSceneFrame = 0;
mu_uint32 EventMask = 0;
mu_int32 MaxBoids = MAX_BOIDS;
mu_int32 MaxFishs = MAX_FISHS;

MUAtomicFloat EarthQuake;

#define ELION_GAMECONFIGDIR _T("./config/")
#define ELION_GAMECONFIG (ELION_GAMECONFIGDIR _T("gameconfig.cfg"))

MUResourceManager::MUResourceManager()
{
	// Texture system settings
	{
		_Textures._Data.resize(TEXTURE_GROW_SIZE, nullptr);
		_Textures._IndexCounter = 0;
	}

	// Options Default Settings
	{
		_IsAutoAttack = false;

		_WindowWidth = 640;
		_WindowHeight = 480;

#if __ANDROID__
		_IsWindowMode = false;
#else
		_IsWindowMode = true;
#endif
		_IsVerticalSync = true;

#if defined(__ANDROID__)
		_IsAntialiasing = false;
		_ShadowType = EShadowType::eOriginalPlus;
#else // Windows
		_IsAntialiasing = true;
		_ShadowType = EShadowType::ePCF;
#endif

		_Threads = 0;

		_MusicVolume = 1.0f;
		_SoundVolume = 1.0f;
	}
}

MUResourceManager::~MUResourceManager()
{
}

mu_boolean MUResourceManager::LoadSettings()
{
	using namespace rapidjson;

	SDL_RWops *fp = nullptr;
	if (mu_rwfromfile(&fp, ELION_GAMECONFIG, "rb") == true)
	{
		mu_int64 fileLength = SDL_RWsize(fp);
		mu_int8 *jsonBuffer = new mu_int8[fileLength];
		SDL_RWread(fp, jsonBuffer, fileLength, 1);
		SDL_RWclose(fp);
		MemoryStream memoryStream((const char*)jsonBuffer, fileLength);

		JsonDocument document;

#ifdef UNICODE
		EncodedInputStream<UTF16LE<>, MemoryStream> encodedStream(memoryStream);
		document.ParseStream<kParseCommentsFlag, UTF16LE<>>(encodedStream);
#else
		document.ParseStream(memoryStream);
#endif

		if (document.HasParseError() == false)
		{
			const JsonDocument::Object &base = document.GetObject();

			if (base.HasMember(_T("AutoAttack")) == true)
			{
				_IsAutoAttack = base[_T("AutoAttack")].GetBool();
			}

#if !defined(__ANDROID__)
			if (base.HasMember(_T("WindowWidth")) == true)
			{
				_WindowWidth = base[_T("WindowWidth")].GetInt();
			}

			if (base.HasMember(_T("WindowHeight")) == true)
			{
				_WindowHeight = base[_T("WindowHeight")].GetInt();
			}
#endif

			if (_WindowWidth < 640 ||
				_WindowHeight < 480)
			{
				_WindowWidth = 640;
				_WindowHeight = 480;
			}

#if !defined(__ANDROID__)
			if (base.HasMember(_T("WindowMode")) == true)
			{
				_IsWindowMode = base[_T("WindowMode")].GetBool();
			}

			if (_IsWindowMode == false)
			{
				SDL_DisplayMode dm = {};
				if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
				{
					_IsWindowMode = true;
				}
				else
				{
					_WindowWidth = dm.w;
					_WindowHeight = dm.h;
				}
			}
#endif

			if (base.HasMember(_T("Antialiasing")) == true)
			{
				_IsAntialiasing = base[_T("Antialiasing")].GetBool();
			}

#if !defined(__ANDROID__)
			if (base.HasMember(_T("VerticalSync")) == true)
			{
				_IsVerticalSync = base[_T("VerticalSync")].GetBool();
			}
#endif

			if (base.HasMember(_T("ShadowType")) == true)
			{
				const mu_uint8 shadowType = static_cast<mu_uint8>(base[_T("ShadowType")].GetUint());
				_ShadowType = shadowType < static_cast<mu_uint8>(EShadowType::eMax) ? static_cast<EShadowType>(shadowType) : EShadowType::eNone;
			}

#if !defined(__ANDROID__)
			if (base.HasMember(_T("Threads")) == true)
			{
				_Threads = base[_T("Threads")].GetInt();
			}
#endif

			if (base.HasMember(_T("MusicVolume")) == true)
			{
				SetMusicVolume(base[_T("MusicVolume")].GetFloat());
			}

			if (base.HasMember(_T("SoundVolume")) == true)
			{
				SetSoundVolume(base[_T("SoundVolume")].GetFloat());
			}
		}

		if (jsonBuffer != nullptr)
		{
			delete[] jsonBuffer;
		}
	}
	else
	{
		SaveSettings();
	}

#if __ANDROID__
	JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
	jobject activity = (jobject)SDL_AndroidGetActivity();

	jclass activity_class = env->GetObjectClass(activity);
	jmethodID getWindowManager = env->GetMethodID(activity_class, "getWindowManager", "()Landroid/view/WindowManager;");
	jobject wm = env->CallObjectMethod(activity, getWindowManager);

	jclass windowManagerClass = env->FindClass("android/view/WindowManager");
	jmethodID getDefaultDisplay = env->GetMethodID(windowManagerClass, "getDefaultDisplay", "()Landroid/view/Display;");
	jobject display = env->CallObjectMethod(wm, getDefaultDisplay);
	jclass displayClass = env->FindClass("android/view/Display");
	jclass pointClass = env->FindClass("android/graphics/Point");
	jmethodID pointConstructor = env->GetMethodID(pointClass, "<init>", "()V");
	jobject pointDisplaySize = env->NewObject(pointClass, pointConstructor);
	jmethodID getRealSize = env->GetMethodID(displayClass, "getRealSize", "(Landroid/graphics/Point;)V");
	env->CallVoidMethod(display, getRealSize, pointDisplaySize);

	jfieldID width_field = env->GetFieldID(pointClass, "x", "I");
	jfieldID height_field = env->GetFieldID(pointClass, "y", "I");
	_WindowWidth = env->GetIntField(pointDisplaySize, width_field);
	_WindowHeight = env->GetIntField(pointDisplaySize, height_field);
#endif

	CalculateAspectRatios();

	return true;
}

void MUResourceManager::SaveSettings()
{
	using namespace rapidjson;

	std::vector<mu_int8> jsonBuffer(JSON_BUFFER);
	MemoryPoolAllocator<> allocator(jsonBuffer.data(), JSON_BUFFER);
#ifdef UNICODE
	GenericDocument<UTF16LE<>> meta(&allocator);
#else
	Document meta(&allocator);
#endif

	meta.SetObject();
	meta.AddMember(_T("AutoAttack"), _IsAutoAttack, allocator);
#if !defined(__ANDROID__)
	meta.AddMember(_T("WindowWidth"), _WindowWidth, allocator);
	meta.AddMember(_T("WindowHeight"), _WindowHeight, allocator);
	meta.AddMember(_T("WindowMode"), _IsWindowMode, allocator);
#endif
	meta.AddMember(_T("Antialiasing"), _IsAntialiasing, allocator);
#if !defined(__ANDROID__)
	meta.AddMember(_T("VerticalSync"), _IsVerticalSync, allocator);
#endif
	meta.AddMember(_T("ShadowType"), static_cast<mu_uint8>(_ShadowType), allocator);
#if !defined(__ANDROID__)
	meta.AddMember(_T("Threads"), _Threads, allocator);
#endif
	meta.AddMember(_T("MusicVolume"), _MusicVolume, allocator);
	meta.AddMember(_T("SoundVolume"), _SoundVolume, allocator);

	typedef GenericStringBuffer<UTF16LE<>, MemoryPoolAllocator<>> StringBuffer;
	StringBuffer stringBuffer(&allocator);
	Writer<StringBuffer, UTF16LE<>, UTF16LE<>, MemoryPoolAllocator<>> writer(stringBuffer, &allocator);
	meta.Accept(writer);

	mu_string gameConfigDir(ELION_GAMECONFIGDIR);
	MU::MakeDirectory(gameConfigDir);

	SDL_RWops *fp = nullptr;
	if (mu_rwfromfile(&fp, ELION_GAMECONFIG, "wb") == true)
	{
		SDL_RWwrite(fp, stringBuffer.GetString(), stringBuffer.GetSize(), 1);
		SDL_RWclose(fp);
	}
}

void MUResourceManager::CalculateAspectRatios()
{
	_640x480Rate[0] = MU_VIEWPORT_RATE(_WindowWidth, 640.0f);
	_640x480Rate[1] = MU_VIEWPORT_RATE(_WindowHeight, 480.0f);
	_1024x768Rate[0] = MU_VIEWPORT_RATE(_WindowWidth, 1024.0f);
	_1024x768Rate[1] = MU_VIEWPORT_RATE(_WindowHeight, 768.0f);
	_1024x1024Rate[0] = MU_VIEWPORT_RATE(_WindowWidth, 1024.0f);
	_1024x1024Rate[1] = MU_VIEWPORT_RATE(_WindowHeight, 1024.0f);
	_1280x720Rate[0] = MU_VIEWPORT_RATE(_WindowWidth, 1280.0f);
	_1280x720Rate[1] = MU_VIEWPORT_RATE(_WindowHeight, 720.0f);
}

void MUResourceManager::UpdateWorldTime(mu_uint64 CurrentTime)
{
	FWorldTime = (mu_float)CurrentTime;
	IWorldTime = (mu_int64)CurrentTime;
}

void MUResourceManager::SetWindowWidth(mu_int32 Width)
{
	_WindowWidth = Width;
}

void MUResourceManager::SetWindowHeight(mu_int32 Height)
{
	_WindowHeight = Height;
}

mu_int32 MUResourceManager::GetWindowWidth()
{
	return _WindowWidth;
}

mu_int32 MUResourceManager::GetWindowHeight()
{
	return _WindowHeight;
}

mu_float MUResourceManager::GetWindowHalfWidth()
{
	return (mu_float)GetWindowWidth() * 0.5f;
}

mu_float MUResourceManager::GetWindowHalfHeight()
{
	return (mu_float)GetWindowHeight() * 0.5f;
}

void MUResourceManager::SetWindowMode(mu_boolean isWindowMode)
{
	_IsWindowMode = isWindowMode;
}

mu_boolean MUResourceManager::IsWindowMode()
{
	return _IsWindowMode;
}

void MUResourceManager::SetAntialiasing(mu_boolean isAntialiasing)
{
	_IsAntialiasing = isAntialiasing;
}

mu_boolean MUResourceManager::IsAntialiasing()
{
	return _IsAntialiasing;
}

void MUResourceManager::SetVerticalSync(mu_boolean isVerticalSync)
{
	_IsVerticalSync = isVerticalSync;
}

mu_boolean MUResourceManager::IsVerticalSync()
{
	return _IsVerticalSync;
}

void MUResourceManager::SetShadowType(const EShadowType shadowType)
{
	_ShadowType = shadowType;
}

const EShadowType MUResourceManager::GetShadowType()
{
	return _ShadowType;
}

void MUResourceManager::SetMusicVolume(mu_float musicVolume)
{
	_MusicVolume = EMath::Clamp(musicVolume, 0.0f, 1.0f);
	EMusicBackend::SetMusicVolume(_MusicVolume);
}

mu_float MUResourceManager::GetMusicVolume()
{
	return _MusicVolume;
}

void MUResourceManager::SetSoundVolume(mu_float soundVolume)
{
	_SoundVolume = EMath::Clamp(soundVolume, 0.0f, 1.0f);
#ifndef DISABLE_AUDIO
	MU_AUDIO->SetMasterVolume(_SoundVolume);
#endif
}

mu_float MUResourceManager::GetSoundVolume()
{
	return _SoundVolume;
}

void MUResourceManager::DestroyTextures()
{
	for (auto it = _Textures._Data.begin(); it != _Textures._Data.end(); ++it)
	{
		MUTexture *texture = (*it);

		MU_SAFE_DELETE(texture);
	}
}

mu_uint32 MUResourceManager::LoadTexture2D(mu_string path)
{
	mu_uint32 textureIndex;

	std::transform(path.begin(), path.end(), path.begin(), mu_tolower);

	_RandomTexturesLock.Lock();

	auto it = TEXTURES::RandomTextureMap.find(path);

	if (it != TEXTURES::RandomTextureMap.end())
	{
		mu_uint32 textureIndex = it->second;
		TEXTURES::Data[textureIndex].IncreaseReference();
		_RandomTexturesLock.Unlock();

		return textureIndex;
	}

	if (TEXTURES::FreeRandomTexture.empty() == false)
	{
		textureIndex = TEXTURES::FreeRandomTexture.top();
	}
	else
	{
		textureIndex = TEXTURES::RandomTextureIndex++;
	}

	TEXTURES::RandomTextureMap.insert(std::make_pair(path, textureIndex));
	TEXTURES::RandomTexturePathMap.insert(std::make_pair(textureIndex, path));
	TEXTURES::Data[textureIndex].IncreaseReference();

	_RandomTexturesLock.Unlock();

	if (TEXTURES::Data[textureIndex].LoadHeaderFromFile(path.c_str()) == false ||
		TEXTURES::Data[textureIndex].LoadFromFileEx(path.c_str()) == false)
	{
		DEBUG_BREAK();
	}

	return textureIndex;
}

void MUResourceManager::UnloadTexture2D(mu_uint32 textureIndex)
{
	_RandomTexturesLock.Lock();

	auto it = TEXTURES::RandomTexturePathMap.find(textureIndex);

	if (it != TEXTURES::RandomTexturePathMap.end())
	{
		MUTexture *texture = &TEXTURES::Data[textureIndex];

		if (texture->DecreaseReference() == true)
		{
			TEXTURES::RandomTextureMap.erase(it->second);
			TEXTURES::RandomTexturePathMap.erase(it);
			texture->Release(false);
		}
	}

	_RandomTexturesLock.Unlock();
}

MUTexture *MUResourceManager::CreateTexture2D(mu_string path, const EImageViewType imageViewType)
{
	mu_uint32 index = -1;

	_Textures._IndexLock.Lock();
	if (_Textures._IndexFree.empty() == false)
	{
		index = _Textures._IndexFree.top();
		_Textures._IndexFree.pop();
	}
	else
	{
		index = _Textures._IndexCounter.fetch_add(1);

		if (index >= _Textures._Data.size())
		{
			_Textures._Data.resize(_Textures._Data.size() + TEXTURE_GROW_SIZE, nullptr);
		}
	}
	_Textures._IndexLock.Unlock();

	MUTexture *texture = _Textures._Data[index];

	if (texture != nullptr)
	{
		return texture;
	}

	texture = new MUTexture();

	if (texture->LoadFromFile(path.c_str(), imageViewType) == false)
	{
		delete texture;
		return nullptr;
	}

	texture->_Index = index;
	_Textures._Data[index] = texture;

	return texture;
}

void MUResourceManager::DeleteTexture(mu_uint32 index)
{
	MUTexture *texture = _Textures._Data[index];

	if (texture != nullptr)
	{
		_Textures._IndexLock.Lock();
		_Textures._IndexFree.push(index);
		_Textures._Data[index] = nullptr;
		_Textures._IndexLock.Unlock();

		texture->Release(false);
		delete texture;
	}
}