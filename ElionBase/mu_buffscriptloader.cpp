#include "stdafx.h"
#include "mu_buffscriptloader.h"

void CutTokenString(const mu_text* pcCuttoken, std::list<mu_string>& out)
{
	if (mu_strlen(pcCuttoken) == 0) return;

	mu_int32 cutpos = 0;

	for (mu_int32 i = 0; i < MAX_DESCRIPT_LENGTH; ++i)
	{
		if (pcCuttoken[i] == _T('/') || pcCuttoken[i] == _T('\0'))
		{
			mu_text Temp[MAX_DESCRIPT_LENGTH] = { 0, };
			mu_strncpy_s(Temp, MAX_DESCRIPT_LENGTH, pcCuttoken + cutpos, i - cutpos);
			out.push_back(Temp);
			cutpos = i + 1;

			if (pcCuttoken[i] == 0) return;
		}
	}
}

BuffScriptLoader::BuffScriptLoader()
{
}

BuffScriptLoader::~BuffScriptLoader()
{

}

mu_boolean BuffScriptLoader::Load(const mu_text *filename)
{
	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, filename, "rb") == false)
	{
		return false;
	}

	static const mu_size HeaderSize = FileHeaders::HEADER_SIZE;
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
	delete[] dataBuffer;

	if (decompressed == false)
	{
		delete[] decompressedBuffer;
		return false;
	}

	mu_uint8 *readBuffer = decompressedBuffer;

	mu_uint32 listSize = 0;
	FileHeaders::ReadBuffer(readBuffer, listSize);

	std::string textData;

	for (mu_uint32 n = 0; n < listSize; ++n)
	{
		BuffInfo buffinfo;

		FileHeaders::ReadBuffer(readBuffer, buffinfo.s_BuffIndex);
		FileHeaders::ReadBuffer(readBuffer, buffinfo.s_BuffEffectType);
		FileHeaders::ReadBuffer(readBuffer, buffinfo.s_ItemType);
		FileHeaders::ReadBuffer(readBuffer, buffinfo.s_ItemIndex);
		FileHeaders::ReadBuffer(readBuffer, buffinfo.s_BuffClassType);
		FileHeaders::ReadBuffer(readBuffer, buffinfo.s_NoticeType);
		FileHeaders::ReadBuffer(readBuffer, buffinfo.s_ClearType);

		mu_uint32 textLength = 0;
		FileHeaders::ReadBuffer(readBuffer, textLength);
		textData.resize(textLength);
		FileHeaders::ReadBuffer(readBuffer, (mu_char*)textData.data(), textLength);
#ifdef UNICODE
		buffinfo.s_BuffName = boost::locale::conv::utf_to_utf<mu_text>(textData);
#else
		buffinfo.s_BuffName = textData;
#endif

		textLength = 0;
		FileHeaders::ReadBuffer(readBuffer, textLength);
		textData.resize(textLength);
		FileHeaders::ReadBuffer(readBuffer, (mu_char*)textData.data(), textLength);
#ifdef UNICODE
		buffinfo.s_BuffDescript = boost::locale::conv::utf_to_utf<mu_text>(textData);
#else
		buffinfo.s_BuffDescript = textData;
#endif

		CutTokenString(buffinfo.s_BuffDescript.c_str(), buffinfo.s_BuffDescriptlist);

		m_Info.insert(std::make_pair(static_cast<eBuffState>(buffinfo.s_BuffIndex), buffinfo));
	}

	delete[] decompressedBuffer;

	return true;
}

const BuffInfo BuffScriptLoader::GetBuffinfo(eBuffState type) const
{
	if (type >= eBuff_Count) return BuffInfo();

	BuffInfoMap::const_iterator iter = m_Info.find(type);

	if (iter != m_Info.end())
	{
		return (*iter).second;
	}

	return BuffInfo();
}

eBuffClass BuffScriptLoader::IsBuffClass(eBuffState type) const
{
	if (type >= eBuff_Count) return eBuffClass_Count;

	BuffInfoMap::const_iterator iter = m_Info.find(type);

	if (iter != m_Info.end())
	{
		return static_cast<eBuffClass>((*iter).second.s_BuffClassType);
	}
	else
	{
		return eBuffClass_Count;
	}
}