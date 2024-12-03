#ifndef __GS_NETWORKPACKET_H__
#define __GS_NETWORKPACKET_H__

#pragma once

class GSNetworkPacket : public ElionNetwork::ELNetworkPacket
{
public:
	virtual mu_boolean IsValid() override;
	virtual mu_uint32 ParsePacket(mu_uint8 *buffer, mu_uint32 bufferSize) override;
};

#endif