#ifndef __CLTOCS_PROTOCOL_H__
#define __CLTOCS_PROTOCOL_H__

#pragma once

// Client<->ConnectServer
enum CL2CS_PROTOCOL : mu_uint16
{
	CL2CS_PROT_GROUPLIST = 0x0010,
	CL2CS_PROT_SERVERLIST = 0x0011,
	CL2CS_PROT_CONNECT = 0x0012,
};

class CS2CL_RECEIVE_GROUPLIST : public MU_SERVER_TO_CLIENT_HEADEREX
{
public:
	CS2CL_RECEIVE_GROUPLIST()
	{
		Code = CL2CS_PROT_GROUPLIST;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE32(GroupList.size());

		for (auto it = GroupList.begin(); it != GroupList.end(); ++it)
		{
			GROUP_LIST &group = *it;

			PARSER_WRITE(group.GroupIndex);
			PARSER_WRITESTRING(group.GroupName, MAX_GROUP_NAME);
		}

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE32(GroupList.size());

		for (auto it = GroupList.begin(); it != GroupList.end(); ++it)
		{
			GROUP_LIST &group = *it;

			PARSER_WRITE(group.GroupIndex);
			PARSER_WRITESTRING(group.GroupName, MAX_GROUP_NAME);
		}

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		mu_uint32 groupCount = 0;
		PARSER_READ(groupCount);
		GroupList.resize(groupCount);

		for (auto it = GroupList.begin(); it != GroupList.end(); ++it)
		{
			GROUP_LIST &group = *it;

			PARSER_READ(group.GroupIndex);
			PARSER_READSTRING(group.GroupName, MAX_GROUP_NAME);
		}

		return true;
	}

public:
	struct GROUP_LIST
	{
		mu_uint32 GroupIndex;
		mu_string GroupName;
	};
	std::vector<GROUP_LIST> GroupList;
};

class CL2CS_REQUEST_SERVERLIST : public MU_CLIENT_TO_SERVER_HEADEREX
{
public:
	CL2CS_REQUEST_SERVERLIST()
	{
		Code = CL2CS_PROT_SERVERLIST;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(GroupIndex);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(GroupIndex);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(GroupIndex);

		return true;
	}

public:
	mu_uint8 GroupIndex;
};

class CS2CL_RECEIVE_SERVERLIST : public MU_SERVER_TO_CLIENT_HEADEREX
{
public:
	CS2CL_RECEIVE_SERVERLIST()
	{
		Code = CL2CS_PROT_SERVERLIST;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(GroupIndex);
		PARSER_WRITEVECTOR(ServerList);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(GroupIndex);
		PARSER_WRITEVECTOR(ServerList);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(GroupIndex);
		PARSER_READVECTOR(ServerList);

		return true;
	}

public:
	struct SERVER_LIST
	{
		mu_uint8 ServerIndex;
		mu_uint8 ServerType;
		mu_uint8 ServerFlag;
		mu_uint8 Percent;
	};

public:
	mu_uint8 GroupIndex;
	std::vector<SERVER_LIST> ServerList;
};

#pragma pack(push, 1)
class ARM_PACKED CL2CS_REQUEST_CONNECT_DATA
{
public:
	mu_uint8 GroupIndex;
	mu_uint8 ServerIndex;
};
#pragma pack(pop)

class CL2CS_REQUEST_CONNECT : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2CS_REQUEST_CONNECT_DATA
{
public:
	CL2CS_REQUEST_CONNECT()
	{
		Code = CL2CS_PROT_CONNECT;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2CS_REQUEST_CONNECT_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2CS_REQUEST_CONNECT_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2CS_REQUEST_CONNECT_DATA*>(this));

		return true;
	}
};

class CS2CL_RECEIVE_CONNECT : public MU_SERVER_TO_CLIENT_HEADEREX
{
public:
	enum
	{
		MAX_IP_LENGTH = 64,
	};

public:
	CS2CL_RECEIVE_CONNECT()
	{
		Code = CL2CS_PROT_CONNECT;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(Response);
		PARSER_WRITESTRING(ServerIP, MAX_IP_LENGTH);
		PARSER_WRITE(ServerPort);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(Response);
		PARSER_WRITESTRING(ServerIP, MAX_IP_LENGTH);
		PARSER_WRITE(ServerPort);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(Response);
		PARSER_READSTRING(ServerIP, MAX_IP_LENGTH);
		PARSER_READ(ServerPort);

		return true;
	}

public:
	mu_uint8 Response;
	mu_string ServerIP;
	mu_uint16 ServerPort;
};

#endif