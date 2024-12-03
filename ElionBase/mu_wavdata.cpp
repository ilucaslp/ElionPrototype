#include "stdafx.h"
#include "mu_wavdata.h"

//--------------------------------------------------------------------------------------
// .WAV files
//--------------------------------------------------------------------------------------
const mu_uint32 FOURCC_RIFF_TAG = 'FFIR';
const mu_uint32 FOURCC_FORMAT_TAG = ' tmf';
const mu_uint32 FOURCC_DATA_TAG = 'atad';
const mu_uint32 FOURCC_WAVE_FILE_TAG = 'EVAW';
const mu_uint32 FOURCC_XWMA_FILE_TAG = 'AMWX';
const mu_uint32 FOURCC_DLS_SAMPLE = 'pmsw';
const mu_uint32 FOURCC_MIDI_SAMPLE = 'lpms';
const mu_uint32 FOURCC_XWMA_DPDS = 'sdpd';
const mu_uint32 FOURCC_XMA_SEEK = 'kees';

MUWavData::MUWavData()
{
	_Loaded = false;
	_Failed = false;

	_VoiceIndex = (mu_uint32)-1;
	_WavBytes = 0;
	_AudioBytes = 0;

	_WavFormat = nullptr;
	_WavBuffer = nullptr;
	_AudioBuffer = nullptr;

	_ReferenceCount = 0;
}

MUWavData::~MUWavData()
{
	Unload();
}

void MUWavData::SetWavPath(const mu_string Path)
{
	_WavPath = Path;
}

mu_boolean MUWavData::LoadWav()
{
	if (_Loaded != false)
	{
		return true;
	}

	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, _WavPath.c_str(), "rb") == false)
	{
		_Failed = true;
		return false;
	}

	mu_int64 fileLength = SDL_RWsize(file);

	if (fileLength < (sizeof(RIFFChunk) * 2 + sizeof(mu_uint32) + sizeof(WAVEFORMAT)))
	{
		SDL_RWclose(file);
		Unload();
		_Failed = true;
		return false;
	}

	_WavBytes = fileLength;
	_WavBuffer = new mu_uint8[fileLength];

	SDL_RWread(file, _WavBuffer, fileLength, 1);
	SDL_RWclose(file);

	if (FindFormatAndData() == false)
	{
		Unload();
		_Failed = true;
		return false;
	}

	_Loaded = true;
	_Failed = false;

	return true;
}

void MUWavData::Unload()
{
	_Loaded = false;
	_Failed = false;

	_WavBytes = 0;
	_AudioBytes = 0;

	_WavFormat = nullptr;
	MU_SAFE_DELETE_ARRAY(_WavBuffer);
	_AudioBuffer = nullptr;
}

const RIFFChunk *MUWavData::FindChunk(const mu_uint8 *Data, mu_uint32 DataSize, mu_uint32 Tag)
{
	const mu_uint8* ptr = Data;
	const mu_uint8* end = Data + DataSize;

	while (end > (ptr + sizeof(RIFFChunk)))
	{
		auto header = reinterpret_cast<const RIFFChunk*>(ptr);
		if (header->tag == Tag)
			return header;

		ptrdiff_t offset = header->size + sizeof(RIFFChunk);
		ptr += offset;
	}

	return nullptr;
}

mu_boolean MUWavData::FindFormatAndData()
{
#if 0
	mu_boolean dpds = false, seek = false;
#endif

	if (_WavBytes < (sizeof(RIFFChunk) * 2 + sizeof(mu_uint32) + sizeof(WAVEFORMAT)))
	{
		return false;
	}

	const mu_uint8* wavEnd = _WavBuffer + _WavBytes;

	// Locate RIFF 'WAVE'
	auto riffChunk = FindChunk(_WavBuffer, _WavBytes, FOURCC_RIFF_TAG);
	if (!riffChunk || riffChunk->size < 4)
	{
		return false;
	}

	auto riffHeader = reinterpret_cast<const RIFFChunkHeader*>(riffChunk);
	if (riffHeader->riff != FOURCC_WAVE_FILE_TAG && riffHeader->riff != FOURCC_XWMA_FILE_TAG)
	{
		return false;
	}

	// Locate 'fmt '
	auto ptr = reinterpret_cast<const mu_uint8*>(riffHeader) + sizeof(RIFFChunkHeader);
	if ((ptr + sizeof(RIFFChunk)) > wavEnd)
	{
		return false;
	}

	auto fmtChunk = FindChunk(ptr, riffHeader->size, FOURCC_FORMAT_TAG);
	if (!fmtChunk || fmtChunk->size < sizeof(PCMWAVEFORMAT))
	{
		return false;
	}

	ptr = reinterpret_cast<const mu_uint8*>(fmtChunk) + sizeof(RIFFChunk);
	if (ptr + fmtChunk->size > wavEnd)
	{
		return false;
	}

	auto wf = reinterpret_cast<const WAVEFORMAT*>(ptr);

	// Validate WAVEFORMAT (focused on chunk size and format tag, not other data that XAUDIO2 will validate)
	switch (wf->wFormatTag)
	{
	case WAVE_FORMAT_PCM:
	case WAVE_FORMAT_IEEE_FLOAT:
		// Can be a PCMWAVEFORMAT (8 bytes) or WAVEFORMATEX (10 bytes)
		// We validiated chunk as at least sizeof(PCMWAVEFORMAT) above
		break;

	default:
		{
			if (fmtChunk->size < sizeof(WAVEFORMATEX))
			{
				return false;
			}

			auto wfx = reinterpret_cast<const WAVEFORMATEX*>(ptr);

			if (fmtChunk->size < (sizeof(WAVEFORMATEX) + wfx->cbSize))
			{
				return false;
			}

			switch (wfx->wFormatTag)
			{
			case WAVE_FORMAT_WMAUDIO2:
			case WAVE_FORMAT_WMAUDIO3:
#if 1
				return false;
#else
				dpds = true;
				break;
#endif

			case  0x166 /*WAVE_FORMAT_XMA2*/: // XMA2 is supported by Xbox One
#if 1
				return false;
#else
				if ((fmtChunk->size < 52 /*sizeof(XMA2WAVEFORMATEX)*/) || (wfx->cbSize < 34 /*( sizeof(XMA2WAVEFORMATEX) - sizeof(WAVEFORMATEX) )*/))
				{
					return false;
				}
				seek = true;
				break;
#endif

			case WAVE_FORMAT_ADPCM:
				if ((fmtChunk->size < (sizeof(WAVEFORMATEX) + 32)) || (wfx->cbSize < 32 /*MSADPCM_FORMAT_EXTRA_BYTES*/))
				{
					return false;
				}
				break;

			case WAVE_FORMAT_EXTENSIBLE:
				if ((fmtChunk->size < sizeof(WAVEFORMATEXTENSIBLE)) || (wfx->cbSize < (sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX))))
				{
					return false;
				}
				else
				{
					static const GUID s_wfexBase = { 0x00000000, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };

					auto wfex = reinterpret_cast<const WAVEFORMATEXTENSIBLE*>(ptr);

					if (memcmp(reinterpret_cast<const mu_uint8*>(&wfex->SubFormat) + sizeof(mu_uint32),
						reinterpret_cast<const mu_uint8*>(&s_wfexBase) + sizeof(mu_uint32), sizeof(GUID) - sizeof(mu_uint32)) != 0)
					{
						return false;
					}

					switch (wfex->SubFormat.Data1)
					{
					case WAVE_FORMAT_PCM:
					case WAVE_FORMAT_IEEE_FLOAT:
						break;

						// MS-ADPCM and XMA2 are not supported as WAVEFORMATEXTENSIBLE

					case WAVE_FORMAT_WMAUDIO2:
					case WAVE_FORMAT_WMAUDIO3:
#if 1
						return false;
#else
						dpds = true;
						break;
#endif

					default:
						return false;
					}

				}
				break;

			default:
				return false;
			}
		}
	}

	// Locate 'data'
	ptr = reinterpret_cast<const mu_uint8*>(riffHeader) + sizeof(RIFFChunkHeader);
	if ((ptr + sizeof(RIFFChunk)) > wavEnd)
	{
		return false;
	}

	auto dataChunk = FindChunk(ptr, riffChunk->size, FOURCC_DATA_TAG);
	if (!dataChunk || !dataChunk->size)
	{
		return false;
	}

	ptr = reinterpret_cast<const mu_uint8*>(dataChunk) + sizeof(RIFFChunk);
	if (ptr + dataChunk->size > wavEnd)
	{
		return false;
	}

	_WavFormat = reinterpret_cast<const WAVEFORMATEX*>(wf);
	_AudioBuffer = ptr;
	_AudioBytes = dataChunk->size;

	return true;
}