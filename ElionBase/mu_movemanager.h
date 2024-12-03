#ifndef __MU_MOVEMANAGER_H__
#define __MU_MOVEMANAGER_H__

#pragma once

class MoveRequirementData
{
public:
	MoveRequirementData()
	{
		Index = -1;
		mu_zeromem(MainMapName, sizeof(MainMapName));
		mu_zeromem(SubMapName, sizeof(SubMapName));
		MinimumLevel = 0;
		MaximumLevel = 0;
		MoveCost = 0;
		GateNumber = -1;
	}

public:
	mu_int32 Index;
	mu_text MainMapName[64];
	mu_text SubMapName[64];
	mu_int32 MinimumLevel;
	mu_int32 MaximumLevel;
	mu_int64 MoveCost;
	mu_int32 GateNumber;
};

class MoveRequirementDataEx : public MoveRequirementData
{
public:
	MoveRequirementDataEx() : MoveRequirementData()
	{
		CanMove = false;
		IsStrife = false;
	}

public:
	mu_boolean CanMove;
	mu_boolean IsStrife;
};

class MUMoveManager
{
public:
	MUMoveManager();
	~MUMoveManager();

	mu_boolean Open(const mu_text *filename);

	void Update();

public:
	EINLINE void SetCastleOwner(mu_boolean isOwner)
	{
		IsCastleOwner = isOwner;
	}

public:
	std::vector<MoveRequirementDataEx> MoveData;
	mu_boolean IsCastleOwner;
};

extern MUMoveManager g_MoveManager;

#endif