#ifndef __MU_SHADER_LOADER_H__
#define __MU_SHADER_LOADER_H__

#pragma once

#include <unordered_map>

#define USE_ENCRYPTED_SHADERS

FORCEINLINE mu_boolean LoadEncryptedShader(const mu_text *Path, mu_uint8 **OutputBuffer, mu_int64 &ShaderLength)
{
#ifndef USE_ENCRYPTED_SHADERS
	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, Path, "rb") == false)
	{
		return false;
	}

	mu_int64 fileLength = SDL_RWsize(file);

	*OutputBuffer = new mu_uint8[fileLength];
	SDL_RWread(file, OutputBuffer, fileLength, 1);

	SDL_RWclose(file);
	return true;
#else
	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, Path, "rb") == false)
	{
		return false;
	}

	static const mu_size HeaderSize = FileHeaders::HEADER_SIZE;
	static const mu_size CompressedHeaderSize = FileHeaders::COMPRESSED_HEADER_SIZE;

	mu_int64 fileLength = SDL_RWsize(file);
	mu_int64 encryptedLength = fileLength - HeaderSize;

	if (encryptedLength % MUBlowfish::BytesLength != 0)
	{
		SDL_RWclose(file);
		return false;
	}

	mu_uint8 *dataBuffer = new mu_uint8[fileLength];
	SDL_RWread(file, dataBuffer, fileLength, 1);
	SDL_RWclose(file);

	FileHeaders::FileHeader *header = (FileHeaders::FileHeader*)dataBuffer;

	if (header->Signature != FILE_HEADERS::DATA_HEADER)
	{
		delete[] dataBuffer;
		return false;
	}

	if (GenerateCRC32(dataBuffer + HeaderSize, encryptedLength) != header->CRC)
	{
		delete[] dataBuffer;
		return false;
	}

	g_DataCrypt.Decode(dataBuffer + HeaderSize, dataBuffer + HeaderSize, encryptedLength);

	FileHeaders::CompressedHeader *compressedHeader = (FileHeaders::CompressedHeader*)(dataBuffer + HeaderSize);

	mu_uint8 *decompressedBuffer = new mu_uint8[compressedHeader->DecompressedSize];

	mu_boolean decompressed = FileHeaders::Decompress(dataBuffer + HeaderSize + CompressedHeaderSize,
		decompressedBuffer,
		compressedHeader->CompressedSize,
		compressedHeader->DecompressedSize);

	ShaderLength = compressedHeader->DecompressedSize;
	delete[] dataBuffer;

	if (decompressed == false)
	{
		delete[] decompressedBuffer;
		return false;
	}

	*OutputBuffer = decompressedBuffer;

	return true;
#endif
}

#endif