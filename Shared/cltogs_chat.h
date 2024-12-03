#ifndef __CLTOGS_CHAT_H__
#define __CLTOGS_CHAT_H__

#pragma once

class CL2GS_REQUEST_CHAT : public MU_CLIENT_TO_SERVER_HEADEREX
{
public:
	CL2GS_REQUEST_CHAT()
	{
		Code = GS2CL_PROT_CHAT;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITESTRING(Message, MAX_CHAT_SIZE);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITESTRING(Message, MAX_CHAT_SIZE);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READSTRING(Message, MAX_CHAT_SIZE);

		return true;
	}

public:
	mu_string Message;
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_CHAT_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class GS2CL_RECEIVE_CHAT : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_CHAT_DATA
{
public:
	GS2CL_RECEIVE_CHAT()
	{
		Code = GS2CL_PROT_CHAT;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_CHAT_DATA*>(this));
		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);
		PARSER_WRITESTRING(Message, MAX_CHAT_SIZE);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_CHAT_DATA*>(this));
		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);
		PARSER_WRITESTRING(Message, MAX_CHAT_SIZE);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_CHAT_DATA*>(this));
		PARSER_READSTRING(Name, CHARACTER_MAXIMUM);
		PARSER_READSTRING(Message, MAX_CHAT_SIZE);

		return true;
	}

public:
	mu_string Name;
	mu_string Message;
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_CHATKEY_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class GS2CL_RECEIVE_CHATKEY : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_CHATKEY_DATA
{
public:
	GS2CL_RECEIVE_CHATKEY()
	{
		Code = GS2CL_PROT_CHATKEY;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_CHATKEY_DATA*>(this));
		PARSER_WRITESTRING(Message, MAX_CHAT_SIZE);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_CHATKEY_DATA*>(this));
		PARSER_WRITESTRING(Message, MAX_CHAT_SIZE);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_CHATKEY_DATA*>(this));
		PARSER_READSTRING(Message, MAX_CHAT_SIZE);

		return true;
	}

public:
	mu_string Message;
};

class CL2GS_REQUEST_WHISPER : public MU_CLIENT_TO_SERVER_HEADEREX
{
public:
	CL2GS_REQUEST_WHISPER()
	{
		Code = GS2CL_PROT_WHISPER;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);
		PARSER_WRITESTRING(Message, MAX_CHAT_SIZE);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);
		PARSER_WRITESTRING(Message, MAX_CHAT_SIZE);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READSTRING(Name, CHARACTER_MAXIMUM);
		PARSER_READSTRING(Message, MAX_CHAT_SIZE);

		return true;
	}

public:
	mu_string Name;
	mu_string Message;
};

class GS2CL_RECEIVE_WHISPER : public MU_SERVER_TO_CLIENT_HEADEREX
{
public:
	GS2CL_RECEIVE_WHISPER()
	{
		Code = GS2CL_PROT_WHISPER;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);
		PARSER_WRITESTRING(Message, MAX_CHAT_SIZE);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);
		PARSER_WRITESTRING(Message, MAX_CHAT_SIZE);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READSTRING(Name, CHARACTER_MAXIMUM);
		PARSER_READSTRING(Message, MAX_CHAT_SIZE);

		return true;
	}

public:
	mu_string Name;
	mu_string Message;
};

class GS2CL_RECEIVE_WHISPERFAIL : public MU_SERVER_TO_CLIENT_HEADEREX
{
public:
	GS2CL_RECEIVE_WHISPERFAIL()
	{
		Code = GS2CL_PROT_WHISPERFAIL;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READSTRING(Name, CHARACTER_MAXIMUM);

		return true;
	}

public:
	mu_string Name;
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_NOTICE_DATA
{
public:
	mu_uint8 Type;
	mu_uint8 Count;
	mu_uint16 Delay;
	mu_uint32 Color;
	mu_uint8 Speed;
};
#pragma pack(pop)

class GS2CL_RECEIVE_NOTICE : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_NOTICE_DATA
{
public:
	GS2CL_RECEIVE_NOTICE()
	{
		Code = GS2CL_PROT_NOTICE;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_NOTICE_DATA*>(this));
		PARSER_WRITESTRING(Message, MAX_CHAT_SIZE);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_NOTICE_DATA*>(this));
		PARSER_WRITESTRING(Message, MAX_CHAT_SIZE);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_NOTICE_DATA*>(this));
		PARSER_READSTRING(Message, MAX_CHAT_SIZE);

		return true;
	}

public:
	mu_string Message;
};

#endif