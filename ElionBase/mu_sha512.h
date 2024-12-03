#ifndef __MU_SHA512_H__
#define __MU_SHA512_H__

#pragma once

class MUSha512
{
public:
	MUSha512();
	~MUSha512();

	void Init();
	void Process(const void *src, mu_uint32 inlen);
	void Output(void *output);

protected:
	void Compress(const mu_uint8 *buf);

private:
	mu_uint64 _Length;
	mu_uint64 _State[8];
	mu_uint32 _CurrentLength;
	mu_uint8 _Buffer[128];
};

#endif