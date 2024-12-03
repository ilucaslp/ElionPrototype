#include "stdafx.h"
#include "mu_path.h"

static const mu_int32 MovePaths[12][2] = {
	{ -1, -1 },
	{ 0, -1 },
	{ +1, -1 },
	{ -1, 0 },
	{ +1, 0 },
	{ -1, +1 },
	{ 0, +1 },
	{ +1, +1 },
	{ 0, +1 },
	{ +1, 0 },
	{ -1, 0 },
	{ 0, -1 },
};

static const mu_int32 NDMoveIndex[] = {
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	1,
	3,
	4,
	6,
};

static const mu_int32 RMovePaths[8][2] = {
	{ +1, +1 },
	{ 0, +1 },
	{ -1, +1 },
	{ +1, 0 },
	{ -1, 0 },
	{ +1, -1 },
	{ 0, -1 },
	{ -1, -1 },
};

#define NORMAL_PATH_COST (5)
#define DIAGONAL_PATH_COST (7)

static mu_uint32 MovePathCost[12] = {
	DIAGONAL_PATH_COST,
	NORMAL_PATH_COST,
	DIAGONAL_PATH_COST,
	NORMAL_PATH_COST,
	NORMAL_PATH_COST,
	DIAGONAL_PATH_COST,
	NORMAL_PATH_COST,
	DIAGONAL_PATH_COST,
	DIAGONAL_PATH_COST,
	DIAGONAL_PATH_COST,
	DIAGONAL_PATH_COST,
	DIAGONAL_PATH_COST,
};

static mu_uint8 PathDir[8] = {
	0,
	1,
	2,
	7,
	3,
	6,
	5,
	4,
};

mu_boolean PATH::FindPath(const mu_uint32 ThreadIndex, mu_int32 xStart, mu_int32 yStart, mu_int32 xEnd, mu_int32 yEnd, mu_boolean bErrorCheck, mu_int32 iWall, mu_boolean Value, mu_float fDistance)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (xStart < 0 ||
		xStart >= MAX_MAP_SIZE ||
		yStart < 0 ||
		yStart >= MAX_MAP_SIZE ||
		(xStart == xEnd &&
			yStart == yEnd))
	{
		return false;
	}

	mu_int32 beginNumber = yStart * MAX_MAP_SIZE + xStart;
	mu_int32 endNumber = yEnd * MAX_MAP_SIZE + xEnd;
	if (bErrorCheck && (iWall <= TerrainWall[endNumber] && (TerrainWall[endNumber] & TW_ACTION) != TW_ACTION))
	{
		return false;
	}

	NodeGrid &beginNode = ThreadInfo.NodesQueue[0];
	beginNode.PathX = xStart;
	beginNode.PathY = yStart;

	ThreadInfo.MapNodesVisited.Reset();

	MapGrid &curGrid = ThreadInfo.MapGrids[beginNumber];

	curGrid.PathCost = 0;
	curGrid.MoveCount = 0;
	curGrid.MoveIndex = 0;

	mu_int32 nodesCount = 1;
	mu_int32 nodeIndex = 0;
	mu_int32 nodeInsertIndex = 1;
	mu_uint32 shortPathCost = MAX_PATH_COST + 1;
	for (; nodesCount-- > 0; nodeIndex = (nodeIndex + 1) % MAX_GRID_QUEUE)
	{
		NodeGrid *node = &ThreadInfo.NodesQueue[nodeIndex];
		MapGrid *grid = &ThreadInfo.MapGrids[node->PathY * MAX_MAP_SIZE + node->PathX];

		mu_uint16 moveCount = grid->MoveCount + 1;

		mu_uint32 n = 0;
		mu_uint32 maxn = 8;
		if (grid->PathCost + DIAGONAL_PATH_COST >= shortPathCost)
		{

			n += 8;
			maxn += 4;
		}

		for (; n < maxn; n++)
		{
			mu_uint32 pathCost = MovePathCost[n];
			mu_uint32 nextCost = grid->PathCost + pathCost;
			if (nextCost >= shortPathCost)
			{
				continue;
			}

			const mu_int32 *movePath = MovePaths[n];
			mu_int32 nextX = (mu_int32)node->PathX + movePath[0];
			if (nextX < 0 || nextX >= MAX_MAP_SIZE) continue;
			mu_int32 nextY = (mu_int32)node->PathY + movePath[1];
			if (nextY < 0 || nextY >= MAX_MAP_SIZE) continue;
			mu_int32 nextIndex = nextY * MAX_MAP_SIZE + nextX;
			MapGrid *nextGrid = &ThreadInfo.MapGrids[nextIndex];

			mu_boolean isVisited = ThreadInfo.MapNodesVisited.Get(nextIndex);
			if (isVisited == true)
			{
				if (nextGrid->PathCost <= nextCost)
				{
					continue;
				}
			}
			else
			{
				ThreadInfo.MapNodesVisited.Set(nextIndex);

				mu_int32 byMapAttribute = TerrainWall[nextIndex] & ~(TW_ACTION | TW_HEIGHT | TW_CAMERA_UP);

				if (iWall <= byMapAttribute)
				{
					nextGrid->PathCost = 0;
					nextGrid->MoveCount = 0;
					continue;
				}
			}

			nextGrid->PathCost = nextCost;
			nextGrid->MoveCount = moveCount;
			nextGrid->MoveIndex = NDMoveIndex[n];

			if (nextIndex != endNumber)
			{
				if (nextCost + NORMAL_PATH_COST < shortPathCost)
				{
					nodesCount++;
					NodeGrid *queueNode = &ThreadInfo.NodesQueue[nodeInsertIndex];
					queueNode->PathX = nextX;
					queueNode->PathY = nextY;
					nodeInsertIndex = (nodeInsertIndex + 1) % MAX_GRID_QUEUE;
				}
			}
			else
			{
				shortPathCost = nextCost;
			}
		}
	}

	MapGrid &endGrid = ThreadInfo.MapGrids[endNumber];
	if (ThreadInfo.MapNodesVisited.Get(endNumber) == 1 &&
		endGrid.MoveCount > 0 &&
		endGrid.PathCost != MAX_PATH_COST + 1)
	{
		mu_int32 currentX = xEnd, currentY = yEnd;
		MapGrid *currentGrid = &endGrid;
		mu_uint32 gridIndex = endGrid.MoveCount;
		for (gridIndex = endGrid.MoveCount; gridIndex > MAX_COUNT_PATH - 1; --gridIndex)
		{
			const mu_int32 *movePath = RMovePaths[currentGrid->MoveIndex];
			currentX += movePath[0], currentY += movePath[1];
			currentGrid = &ThreadInfo.MapGrids[currentY * MAX_MAP_SIZE + currentX];
		}

		ThreadInfo.NumPath = gridIndex + 1;
		for (; gridIndex > 0; --gridIndex)
		{
			ThreadInfo.DirPath[gridIndex] = PathDir[currentGrid->MoveIndex];
			ThreadInfo.PathX[gridIndex] = currentX, ThreadInfo.PathY[gridIndex] = currentY;

			const mu_int32 *movePath = RMovePaths[currentGrid->MoveIndex];
			currentX += movePath[0], currentY += movePath[1];
			currentGrid = &ThreadInfo.MapGrids[currentY * MAX_MAP_SIZE + currentX];
		}

		ThreadInfo.PathX[0] = xStart, ThreadInfo.PathY[0] = yStart;

		return true;
	}

	return false;
}