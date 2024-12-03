#ifndef __MU_PATH_H__
#define __MU_PATH_H__

#pragma once

#define MAX_MAP_SIZE (256)
#define MAX_GRID_QUEUE ((MAX_MAP_SIZE * MAX_MAP_SIZE * 8) / 2)
#define MAX_PATH_COST (0x3FFFFF - 1)

namespace PATH
{
	mu_boolean FindPath(const mu_uint32 ThreadIndex, mu_int32 xStart, mu_int32 yStart, mu_int32 xEnd, mu_int32 yEnd, mu_boolean bErrorCheck, mu_int32 iWall, mu_boolean Value, mu_float fDistance = 0.0f);
};

#endif