#ifndef __SHARED_PROTOCOL_H__
#define __SHARED_PROTOCOL_H__

#pragma once

#ifndef FORCEINLINE
#if __ANDROID__
#define FORCEINLINE inline
#else
#define FORCEINLINE __forceinline
#endif
#endif

#include <shared_define.h>
#include <shared_encryption.h>

#define EBITS(b) ~((~0U)<<b)
#define EROUND_UP(N, S) ((((N) + (S) - 1) / (S)) * (S))
#define ECU8(S) ((mu_uint8*)Buffer)
#define ECU16(S) ((mu_uint16*)Buffer)
#define ECU32(S) ((mu_uint32*)Buffer)

#define SOCKETSLOT_NONE		0xFF
#define SOCKETSLOT_EMPTY	0xFE

namespace SHARED
{
	enum PETDB_REQUEST
	{
		PETDB_NONE = -1,
		PETDB_CREATE = -2,
	};

	namespace BITS_COUNT
	{
		enum
		{
			ITEM_ENABLED = 1, // 1 bits enabled flag
			ITEM_TYPE = 4, // 16 categories(0 to 15)
			ITEM_INDEX = 9, // 512 indexes(0 to 511)
			ITEM_LEVEL = 4, // 0 to 15 Level
			ITEM_SKILL = 1, // 0 or 1 skill
			ITEM_LUCK = 1, // 0 or 1 luck
			ITEM_DURABILITY = 8, // 0 to 255 durability
			ITEM_OPTION = 3, // 0 to 7 option
			ITEM_EXCELLENT = 8, // 8 bits excellent
			ITEM_SETOPTION = 8, // 8 bits setoption
			ITEM_ITEMOPTION = 1, // 1 bit 380 item option
			ITEM_HARMONYOPTION = 8, // 8 bits harmony option
			ITEM_BONUSSOCKET = 8, // 8 bits bonus socket option
			ITEM_SOCKETOPTION = 8, // 8 bits per socket option
		};

		static const mu_uint32 ITEM_ENABLED_F = EBITS(ITEM_ENABLED);
		static const mu_uint32 ITEM_TYPE_F = EBITS(ITEM_TYPE);
		static const mu_uint32 ITEM_INDEX_F = EBITS(ITEM_INDEX);
		static const mu_uint32 ITEM_LEVEL_F = EBITS(ITEM_LEVEL);
		static const mu_uint32 ITEM_SKILL_F = EBITS(ITEM_SKILL);
		static const mu_uint32 ITEM_LUCK_F = EBITS(ITEM_LUCK);
		static const mu_uint32 ITEM_DURABILITY_F = EBITS(ITEM_DURABILITY);
		static const mu_uint32 ITEM_OPTION_F = EBITS(ITEM_OPTION);
		static const mu_uint32 ITEM_EXCELLENT_F = EBITS(ITEM_EXCELLENT);
		static const mu_uint32 ITEM_SETOPTION_F = EBITS(ITEM_SETOPTION);
		static const mu_uint32 ITEM_ITEMOPTION_F = EBITS(ITEM_ITEMOPTION);
		static const mu_uint32 ITEM_HARMONYOPTION_F = EBITS(ITEM_HARMONYOPTION);
		static const mu_uint32 ITEM_BONUSSOCKET_F = EBITS(ITEM_BONUSSOCKET);
		static const mu_uint32 ITEM_SOCKETOPTION_F = EBITS(ITEM_SOCKETOPTION);
	};

	static const mu_uint32 CharsetBytes = EROUND_UP((BITS_COUNT::ITEM_ENABLED + \
		BITS_COUNT::ITEM_TYPE + \
		BITS_COUNT::ITEM_INDEX + \
		BITS_COUNT::ITEM_LEVEL + \
		BITS_COUNT::ITEM_EXCELLENT + \
		BITS_COUNT::ITEM_SETOPTION), 8) / 8;

#pragma pack(push, 1)
	// Charset (Character List or Player Viewport)
	struct ARM_PACKED CHARSET_PACKET
	{
		CHARSET_PACKET()
		{
			Enabled = false;
		}

		mu_boolean Enabled;

		mu_uint8 Type;
		mu_uint8 Level;
		mu_uint8 ExcOption;
		mu_uint8 SetOption;

		mu_uint16 Index;
	};

	class ARM_PACKED ITEM_DATA
	{
	public:
		ITEM_DATA()
		{
			Skill = false;
			Luck = false;
			ItemOption = false;

			InventoryType = 0;
			InventoryPosX = 0;
			InventoryPosY = 0;

			ItemType = 0;
			ItemIndex = 0;

			Level = 0;
			Durability = 0;
			Option = 0;
			ExcOption = 0;
			SetOption = 0;
			HarmonyOption = 0;
			BonusSocketOption = SOCKETSLOT_NONE;
			mu_memset(SocketOption, SOCKETSLOT_NONE, sizeof(SocketOption));

			PetLevel = 0;
			PetExperience = 0;
		}

	public:
		mu_boolean Skill;
		mu_boolean Luck;
		mu_boolean ItemOption;

		mu_uint8 InventoryType;
		mu_uint8 InventoryPosX;
		mu_uint8 InventoryPosY;
		mu_uint8 ItemType;
		mu_uint8 Option;
		mu_uint8 ExcOption;
		mu_uint8 SetOption;
		mu_uint8 HarmonyOption;
		mu_uint8 BonusSocketOption;
		mu_uint8 SocketOption[5];

		mu_uint16 ItemIndex;
		mu_uint16 Level;
		mu_uint16 Durability;

		mu_int32 PetLevel;
		mu_int64 PetExperience;
	};

	class ARM_PACKED ITEM_DATAEX : public ITEM_DATA
	{
	public:
		ITEM_DATAEX() : ITEM_DATA()
		{
			DbIndex = -1;
			ServerIndex = 0;
			ServerKey = 0;
			ItemFlags = 0;
			PetIndex = SHARED::PETDB_NONE;
		}

	public:
		mu_uint16 ServerIndex;

		mu_int32 PetIndex;

		mu_int64 DbIndex;
		mu_int64 ServerKey;
		mu_int64 ItemFlags;
	};

	struct ARM_PACKED SKILL_DATA
	{
		mu_uint16 SkillIndex;
		mu_uint8 SkillLevel;
	};

	struct ARM_PACKED QUESTMONSTERKILL_DATA
	{
		mu_int16 QuestIndex;

		mu_int16 MonsterIndex[MAX_QUEST_MONSTER_KILL_INFO];
		mu_int16 KillCount[MAX_QUEST_MONSTER_KILL_INFO];
	};
#pragma pack(pop)
};

// Helper
class MUParseHelper
{
public:
	MUParseHelper(mu_uint8 *buffer, mu_uint32 bufferLength, mu_boolean isLengthOnly)
	{
		BufferBase = buffer;
		Buffer = buffer;
		BufferLength = bufferLength;
		ProcessedLength = 0;
		IsLengthOnly = isLengthOnly;
	}

	// Prevent unaligned access
#pragma pack(push, 1)
	struct ARM_PACKED CHARSET_BUFFER
	{
		mu_uint16 d16[2];
		mu_uint8 d8[1];
	};
#pragma pack(pop)

	// Charset
	FORCEINLINE mu_boolean WriteCharset(const SHARED::CHARSET_PACKET &Data)
	{
		using namespace SHARED;
		using namespace BITS_COUNT;

		if(IsLengthOnly == false)
		{
			if (BufferLength - ProcessedLength < CharsetBytes)
			{
				return false;
			}

			CHARSET_BUFFER tmpBuffer;

			tmpBuffer.d16[0] = (mu_uint8)Data.Enabled |
				((Data.Type & ITEM_TYPE_F) << ITEM_ENABLED) |
				((Data.Index & ITEM_INDEX_F) << (ITEM_ENABLED + ITEM_TYPE));
			tmpBuffer.d16[1] = (Data.Level & ITEM_LEVEL_F) |
				(Data.ExcOption << ITEM_LEVEL);
			tmpBuffer.d8[0] = Data.SetOption;

			mu_memcpy(Buffer, &tmpBuffer, sizeof(tmpBuffer));
		}

		Buffer += CharsetBytes;
		ProcessedLength += CharsetBytes;
		
		return true;
	}

	FORCEINLINE mu_boolean ReadCharset(SHARED::CHARSET_PACKET &Data)
	{
		using namespace SHARED;
		using namespace BITS_COUNT;

		if (BufferLength - ProcessedLength < CharsetBytes)
		{
			return false;
		}

		CHARSET_BUFFER tmpBuffer;
		mu_memcpy(&tmpBuffer, Buffer, sizeof(tmpBuffer));

		Data.Enabled = (tmpBuffer.d16[0] & ITEM_ENABLED_F) != 0 ? true : false;
		Data.Type = (tmpBuffer.d16[0] >> ITEM_ENABLED) & ITEM_TYPE_F;
		Data.Index = (tmpBuffer.d16[0] >> (ITEM_ENABLED + ITEM_TYPE)) & ITEM_INDEX_F;
		Data.Level = tmpBuffer.d16[1] & ITEM_LEVEL_F;
		Data.ExcOption = (tmpBuffer.d16[1] >> ITEM_LEVEL) & ITEM_EXCELLENT_F;
		Data.SetOption = tmpBuffer.d8[0];

		Buffer += CharsetBytes;
		ProcessedLength += CharsetBytes;

		return true;
	}

	// General
	template<typename T>
	mu_boolean Write(T &Data)
	{
		if(IsLengthOnly == false)
		{
			if (BufferLength - ProcessedLength < sizeof(T))
			{
				return false;
			}

			memcpy(Buffer, &Data, sizeof(T));
		}

		Buffer += sizeof(T);
		ProcessedLength += sizeof(T);

		return true;
	}

	template<typename T>
	mu_boolean WriteEx(T Data)
	{
		if (IsLengthOnly == false)
		{
			if (BufferLength - ProcessedLength < sizeof(T))
			{
				return false;
			}

			memcpy(Buffer, &Data, sizeof(T));
		}

		Buffer += sizeof(T);
		ProcessedLength += sizeof(T);

		return true;
	}

	FORCEINLINE mu_boolean Write(void *Data, mu_uint32 ByteSize)
	{
		if (IsLengthOnly == false)
		{
			if (BufferLength - ProcessedLength < ByteSize)
			{
				return false;
			}

			memcpy(Buffer, Data, ByteSize);
		}

		Buffer += ByteSize;
		ProcessedLength += ByteSize;

		return true;
	}

	template<typename T>
	mu_boolean WriteArray(T *Data, mu_uint32 Count)
	{
		if (IsLengthOnly == false)
		{
			if (BufferLength - ProcessedLength < sizeof(T) * Count)
			{
				return false;
			}

			memcpy(Buffer, Data, sizeof(T) * Count);
		}

		Buffer += sizeof(T) * Count;
		ProcessedLength += sizeof(T) * Count;

		return true;
	}

	template<typename T>
	mu_boolean WriteVector(std::vector<T> &Data)
	{
		if (WriteEx((mu_uint32)Data.size()) == false)
		{
			return false;
		}

		if (Write(Data.data(), sizeof(T) * (mu_uint32)Data.size()) == false)
		{
			return false;
		}

		return true;
	}

	template<mu_uint32 Limit>
	mu_boolean WriteString(const mu_string &String)
	{
		mu_uint32 textSize = 0;
		if (ConvertToUTF8(String.c_str(), (mu_uint32)String.size(), nullptr, 0, &textSize) == false)
		{
			textSize = 0;
		}
		else if (textSize > WIDESIZE(Limit))
		{
			textSize = 0;
		}

		if (WriteEx((mu_uint32)textSize) == false)
		{
			return false;
		}

		if (textSize > 0)
		{
			mu_char textBuffer[WIDESIZE(Limit) + 1] = { 0 };
			textSize = 0;

			ConvertToUTF8(String.c_str(), (mu_uint32)String.size(), textBuffer, WIDESIZE(Limit), &textSize);

			if (Write(textBuffer, (mu_uint32)textSize) == false)
			{
				return false;
			}
		}

		return true;
	}

	template<typename T>
	mu_boolean Read(T &Data)
	{
		if (BufferLength - ProcessedLength < sizeof(T))
		{
			return false;
		}

		memcpy(&Data, Buffer, sizeof(T));

		Buffer += sizeof(T);
		ProcessedLength += sizeof(T);

		return true;
	}

	FORCEINLINE mu_boolean Read(void *Data, mu_uint32 ByteSize)
	{
		if (BufferLength - ProcessedLength < ByteSize)
		{
			return false;
		}

		memcpy(Data, Buffer, ByteSize);

		Buffer += ByteSize;
		ProcessedLength += ByteSize;

		return true;
	}

	template<typename T>
	mu_boolean ReadArray(T *Data, mu_uint32 Count)
	{
		if (BufferLength - ProcessedLength < sizeof(T) * Count)
		{
			return false;
		}

		memcpy(Data, Buffer, sizeof(T) * Count);

		Buffer += sizeof(T) * Count;
		ProcessedLength += sizeof(T) * Count;

		return true;
	}

	template<typename T>
	mu_boolean ReadVector(std::vector<T> &Data)
	{
		mu_uint32 dataCount = 0;
		if (Read(dataCount) == false)
		{
			return false;
		}

		Data.resize(dataCount);

		if (Read(Data.data(), sizeof(T) * dataCount) == false)
		{
			return false;
		}

		return true;
	}

	template<mu_uint32 Limit>
	mu_boolean ReadString(mu_string &String)
	{
		String.clear();

		mu_uint32 textSize = 0;
		if (Read(textSize) == false)
		{
			return false;
		}

		if (textSize > WIDESIZE(Limit))
		{
			return false;
		}

		if (textSize > 0)
		{
			mu_char textBuffer[WIDESIZE(Limit) + 1] = { 0 };
			mu_unicode unicodeBuffer[Limit + 1] = { 0 };

			if (Read(textBuffer, textSize) == false)
			{
				return false;
			}

			mu_uint32 convertedSize = 0;
			if (ConvertFromUTF8(textBuffer, textSize, unicodeBuffer, Limit, &convertedSize) == false)
			{
				return false;
			}

			String.assign(unicodeBuffer, convertedSize);
		}

		return true;
	}

public:
	mu_boolean IsLengthOnly;
	mu_uint8 *BufferBase;
	mu_uint8 *Buffer;
	mu_uint32 BufferLength;
	mu_uint32 ProcessedLength;
};

#define DECLARE_PARSERHELPER(buffer, length, lengthOnly) MUParseHelper parser(buffer, length, lengthOnly)

#define PARSER_WRITE(data) if(parser.Write(data) == false) return false;
#define PARSER_WRITEEX(data) if(parser.WriteEx(data) == false) return false;
#define PARSER_WRITE32(data) if(parser.WriteEx((mu_uint32)data) == false) return false;
#define PARSER_WRITEDATA(data, length) if(parser.Write(data, length) == false) return false;
#define PARSER_WRITEARRAY(data, count) if(parser.WriteArray(data, (mu_uint32)count) == false) return false;
#define PARSER_WRITEVECTOR(data) if(parser.WriteVector(data) == false) return false;
#define PARSER_WRITESTRING(string, limit) if(parser.WriteString<limit>(string) == false) return false;
#define PARSER_WRITECHARSET(data) if(parser.WriteCharset(data) == false) return false;

#define PARSER_READ(data) if(parser.Read(data) == false) return false;
#define PARSER_READDATA(data, length) if(parser.Read(data, length) == false) return false;
#define PARSER_READARRAY(data, count) if(parser.ReadArray(data, (mu_uint32)count) == false) return false;
#define PARSER_READVECTOR(data) if(parser.ReadVector(data) == false) return false;
#define PARSER_READSTRING(string, limit) if(parser.ReadString<limit>(string) == false) return false;
#define PARSER_READCHARSET(data) if(parser.ReadCharset(data) == false) return false;

namespace ParseHelper
{
	FORCEINLINE mu_uint32 RoundByBlockSize(mu_uint32 Size, mu_uint32 BlockSize)
	{
		mu_uint32 remainder = Size % BlockSize;
		return remainder == 0 ? Size : Size + (BlockSize - remainder);
	}
};

#pragma pack(push, 1)
// Shared
class MU_PACKET_HEADER
{
public:
	mu_uint32 Size;
	mu_uint8 Buffer[1];
};

#define PACKET_MINIMUM_SIZE offsetof(MU_PACKET_HEADER, Buffer)

// Client<->Server
class ARM_PACKED MU_CLIENT_TO_SERVER_HEADER
{
public:
	mu_uint16 Code;
};

class ARM_PACKED MU_SERVER_TO_CLIENT_HEADER
{
public:
	mu_uint16 Code;
};

class ARM_PACKED MU_VIEWPORT_HEADER
{
public:
	mu_uint32 Size;
	mu_uint16 Code;
};

// Server<->Server
class ARM_PACKED MU_SERVER_TO_SERVER_HEADER
{
public:
	mu_boolean RequireReceived;
	mu_uint16 Code;
};
#pragma pack(pop)

class ARM_PACKED MU_CLIENT_TO_SERVER_HEADEREX : public MU_CLIENT_TO_SERVER_HEADER
{
public:
	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));

		return true;
	}
};

class ARM_PACKED MU_SERVER_TO_CLIENT_HEADEREX : public MU_SERVER_TO_CLIENT_HEADER
{
public:
	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));

		return true;
	}
};

class ARM_PACKED MU_VIEWPORT_HEADEREX : public MU_VIEWPORT_HEADER
{
public:
	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));

		return true;
	}
};

class ARM_PACKED MU_SERVER_TO_SERVER_HEADEREX : public MU_SERVER_TO_SERVER_HEADER
{
public:
	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, STS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));

		return true;
	}
};

#endif