#include "stdafx.h"
#include "mu_dialogmanager.h"

MUDialogManager g_DialogManager;

MUDialogManager::MUDialogManager()
{

}

mu_boolean MUDialogManager::Load(const mu_text *filename)
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

	mu_asciistr textData;

	mu_uint8 *readBuffer = decompressedBuffer;
	{
		mu_uint32 dialogCount = 0;
		FileHeaders::ReadBuffer(readBuffer, dialogCount);

		_Dialogs.resize(dialogCount);

		for (auto it = _Dialogs.begin(); it != _Dialogs.end(); ++it)
		{
			MUDialogData &dialog = *it;

			mu_int32 index;
			FileHeaders::ReadBuffer(readBuffer, index);

			mu_uint32 textLength = 0;
			FileHeaders::ReadBuffer(readBuffer, textLength);
			textData.resize(textLength);
			FileHeaders::ReadBuffer(readBuffer, (mu_char*)textData.data(), textLength);
#ifdef UNICODE
			dialog.Text = boost::locale::conv::utf_to_utf<mu_text>(textData);
#else
			dialog.Text = textData;
#endif

			mu_uint32 answersCount = 0;
			FileHeaders::ReadBuffer(readBuffer, answersCount);
			dialog.Answers.resize(answersCount);

			for (mu_uint32 k = 0; k < answersCount; ++k)
			{
				MUDialogAnswerData &answer = dialog.Answers[k];

				textLength = 0;
				FileHeaders::ReadBuffer(readBuffer, textLength);
				textData.resize(textLength);
				FileHeaders::ReadBuffer(readBuffer, (mu_char*)textData.data(), textLength);
#ifdef UNICODE
				answer.Answer = boost::locale::conv::utf_to_utf<mu_text>(textData);
#else
				answer.Answer = textData;
#endif

				FileHeaders::ReadBuffer(readBuffer, answer.LinkForAnswer);
				FileHeaders::ReadBuffer(readBuffer, answer.ReturnForAnswer);
			}

			_DialogsMap.insert(std::make_pair(index, &dialog));
		}
	}

	delete[] decompressedBuffer;

	return true;
}

const MUDialogData *MUDialogManager::GetDialog(mu_int32 dialogIndex)
{
	auto it = _DialogsMap.find(dialogIndex);
	if (it == _DialogsMap.end())
		return nullptr;

	return it->second;
}