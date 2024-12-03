#ifndef __MU_FILEHEADER_H__
#define __MU_FILEHEADER_H__

#pragma once

typedef std::vector<mu_uint8> mu_filebuffer;

namespace FILE_HEADERS
{
	enum : mu_uint32
	{
		DATA_HEADER = MAKEFOURCC('D', 'A', 'T', 'X'),
		TERRAIN_HEADER = MAKEFOURCC('E', 'W', 'L', 'D'),
		TEXTURE_HEADER = MAKEFOURCC('E', 'T', 'E', 'X'),
		MODELHEADER_HEADER = MAKEFOURCC('E', 'M', 'H', 'D'),
		MODEL_HEADER = MAKEFOURCC('E', 'M', 'D', 'L'),
		ANIMATION_HEADER = MAKEFOURCC('E', 'A', 'N', 'I'),
		MODELSOUND_HEADER = MAKEFOURCC('E', 'S', 'N', 'D'),
	};
}

namespace FileHeaders
{
	class ARM_PACKED FileHeader
	{
	public:
		mu_uint32 Signature;
		mu_uint32 CRC;
	};

	class ARM_PACKED FileHeaderEx
	{
	public:
		mu_uint32 Signature;
		mu_uint32 Version;
		mu_uint32 CRC;
	};

	class ARM_PACKED CompressedHeader
	{
	public:
		mu_uint32 CompressedSize;
		mu_uint32 DecompressedSize;
	};

	enum
	{
		HEADER_SIZE = sizeof(FileHeader),
		HEADEREX_SIZE = sizeof(FileHeaderEx),
		COMPRESSED_HEADER_SIZE = sizeof(CompressedHeader),
	};

	mu_boolean Decompress(mu_uint8 *Input, mu_uint8 *Output, mu_uint32 InputLength, mu_uint32 OutputLength);

	template<typename T>
	void ReadBuffer(mu_uint8 *&buffer, T &data)
	{
		mu_memcpy(&data, buffer, sizeof(data));
		buffer += sizeof(data);
	}

	void ReadBuffer(mu_uint8 *&buffer, void *data, const mu_size length);
}

extern MUBlowfish g_DataCrypt;

#endif