#include "stdafx.h"
#include "mu_fileheader.h"

#include <zlib.h>

const mu_uint8 g_DataCryptKey[] = { 0xdc, 0x35, 0xea, 0xe5, 0x2d, 0x33, 0xa8, 0x6a, 0xa0, 0x31, 0xae, 0xbf, 0xc3, 0xda, 0xb0, 0x54, 0xea, 0xca, 0xcc, 0xa2, 0x42, 0xe1, 0x85, 0x88, 0x4b, 0xa3, 0xc9, 0x82, 0x16, 0x87, 0xa3, 0xa4, 0xd0, 0xaf, 0xfb, 0x7f, 0x0b, 0x84, 0xea, 0x37, 0xfc, 0x93, 0x80, 0x30, 0x94, 0x05, 0x41, 0x42, 0x16, 0x9f, 0xba, 0xed, 0x48, 0x89, 0x94, 0xc8 };
MUBlowfish g_DataCrypt(g_DataCryptKey, mu_countof(g_DataCryptKey));

namespace FileHeaders
{
	mu_boolean Decompress(mu_uint8 *Input, mu_uint8 *Output, mu_uint32 InputLength, mu_uint32 OutputLength)
	{
		z_stream strm;

		/* allocate inflate state */
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		strm.avail_in = 0;
		strm.next_in = Z_NULL;

		if (inflateInit(&strm) != Z_OK)
		{
			return false;
		}

		strm.avail_in = InputLength;
		strm.next_in = Input;
		strm.avail_out = OutputLength;
		strm.next_out = Output;

		mu_int32 ret = inflate(&strm, Z_FINISH);
		mu_assert(ret == Z_STREAM_END);

		inflateEnd(&strm);

		if (ret != Z_STREAM_END)
		{
			return false;
		}

		return true;
	}

	void ReadBuffer(mu_uint8 *&buffer, void *data, const mu_size length)
	{
		mu_memcpy(data, buffer, length);
		buffer += length;
	}
};