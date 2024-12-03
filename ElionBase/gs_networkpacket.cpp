#include "stdafx.h"
#include "gs_networkpacket.h"

using namespace ElionNetwork;

mu_boolean GSNetworkPacket::IsValid()
{
	if (IsMissing() == true || GetPacketSize() == 0)
		return false;

	return true;
}

mu_uint32 GSNetworkPacket::ParsePacket(mu_uint8 *buffer, mu_uint32 bufferSize)
{
	if (GetPacketSize() > 0 && IsMissing() == false)
		return PACKET_COMPLETED;

	if (bufferSize < PACKET_MINIMUM_SIZE)
		return PACKET_NOT_ENOUGH;

	if (GetPacketSize() == 0)
	{
		// Prevent unaligned access
		MU_PACKET_HEADER header;
		mu_memcpy(&header, buffer, PACKET_MINIMUM_SIZE);

		if (header.Size <= PACKET_MINIMUM_SIZE ||
			(header.Size - PACKET_MINIMUM_SIZE) % CTS_ENCRYPTION_BLOCKSIZE != 0 ||
			CreateBuffer(header.Size) == false)
		{
			return PACKET_CORRUPTED;
		}
	}

	mu_uint32 copiedSize = PACKET_EMPTY;

	if (IsMissing() == true)
	{
		mu_uint32 toCopy = GetMissingSize() > bufferSize ? bufferSize : GetMissingSize();

		if (AddToBuffer(buffer, toCopy) == true)
			copiedSize = toCopy;

		if (IsMissing() == false)
		{
			g_GameServerCrypt.Decode(GetBuffer() + PACKET_MINIMUM_SIZE, GetBuffer() + PACKET_MINIMUM_SIZE, GetPacketSize() - PACKET_MINIMUM_SIZE);
		}
	}

	return copiedSize;
}