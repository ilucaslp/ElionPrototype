#ifndef __CS_NETWORKPACKET_H__
#define __CS_NETWORKPACKET_H__

#pragma once

class CSNetworkPacket : public ElionNetwork::ELNetworkPacket
{
public:
	virtual mu_boolean IsValid() override;
	virtual mu_uint32 ParsePacket(mu_uint8 *buffer, mu_uint32 bufferSize) override;
};

#endif