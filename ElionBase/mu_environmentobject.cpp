#include "stdafx.h"
#include "mu_environmentobject.h"

EParallelVariable<mu_int32, MAX_TASKING_THREADS> g_ActionObjectType(-1);
EParallelVariable<mu_int32, MAX_TASKING_THREADS> g_ActionWorld(-1);
EParallelVariable<mu_int32, MAX_TASKING_THREADS> g_ActionTime(-1);
EParallelVariable<mu_float, MAX_TASKING_THREADS> g_ActionObjectVelocity(-1.0f);

MUCriticalSectionSpinLock g_EnvironmentObjectsLock;
mu_uint32 g_EnvironmentObjectsCount = 0;
EObjectCullingData g_EnvironmentObjectsNodes[MAX_OBJECTS] = { 0 };
std::array<OBJECT, MAX_OBJECTS> g_EnvironmentObjects;
std::array<std::array<mu_uint16, MAX_OBJECTS/4>, CPUCulling::BLOCK_COUNT> g_EnvironmentBlocks;
std::array<mu_uint16, CPUCulling::BLOCK_COUNT> g_EnvironmentBlocksCount;

void CreateObject(const mu_uint32 ThreadIndex, mu_uint32 Type, const EVector3 &Position, const EVector3 &Angle, mu_float Scale)
{
	MUModel &model = MODELS::Data[Type];
	EMatrix4 matrix = EMath::MakeMatrix(Position,
			EQuat(EMath::ToRadians(Angle)),
			Scale);

	EMathSphere boundingSphere = model._BoundingSphere;
	boundingSphere.Transform(matrix);

	mu_float beginX = (boundingSphere._Center.x - boundingSphere._Radius) - EMath::Mod((boundingSphere._Center.x - boundingSphere._Radius), g_TerrainScale * 4.0f) - g_TerrainScale * 4.0f;
	mu_float beginY = (boundingSphere._Center.y - boundingSphere._Radius) - EMath::Mod((boundingSphere._Center.y - boundingSphere._Radius), g_TerrainScale * 4.0f) - g_TerrainScale * 4.0f;

	mu_float endX = (boundingSphere._Center.x + boundingSphere._Radius) + (g_TerrainScale * 4.0f - EMath::Mod((boundingSphere._Center.x + boundingSphere._Radius), g_TerrainScale * 4.0f)) + g_TerrainScale * 4.0f;
	mu_float endY = (boundingSphere._Center.y + boundingSphere._Radius) + (g_TerrainScale * 4.0f - EMath::Mod((boundingSphere._Center.y + boundingSphere._Radius), g_TerrainScale * 4.0f)) + g_TerrainScale * 4.0f;

	if (beginX < 0.0f)
		beginX = 0.0f;
	if (beginY < 0.0f)
		beginY = 0.0f;
	if (endX > TERRAIN_SIZE * g_TerrainScale)
		endX = TERRAIN_SIZE * g_TerrainScale;
	if (endY > TERRAIN_SIZE * g_TerrainScale)
		endY = TERRAIN_SIZE * g_TerrainScale;

	mu_float x, y;
	mu_uint32 bx, by;

	EObjectCullingData data = { 255, 255, 0, 0 };

	for (y = beginY, by = (mu_uint32)(beginY / (g_TerrainScale * 4.0f)); y < endY; y += g_TerrainScale * 4.0f, ++by)
	{
		x = beginX;
		bx = (mu_uint32)(beginX / (g_TerrainScale * 4.0f));

		mu_boolean isFound = false;
		mu_boolean isFoundEnd = false;

		for (; x < endX; x += g_TerrainScale * 4.0f, ++bx)
		{
			EVector3 point1(x, y, -10000.0f);
			EVector3 point2(x + g_TerrainScale * 4.0f, y + g_TerrainScale * 4.0f, 10000.0f);
			EMathAABB boundingBox(point1, point2);

			if (EMath::Contains(boundingBox, boundingSphere) == true)
			{
				isFound = true;
				if (data.SX > bx)
				{
					data.SX = bx;
				}
				break;
			}
		}

		for (; x < endX; x += g_TerrainScale * 4.0f, ++bx)
		{
			EVector3 point1(x, y, -10000.0f);
			EVector3 point2(x + g_TerrainScale * 4.0f, y + g_TerrainScale * 4.0f, 10000.0f);
			EMathAABB boundingBox(point1, point2);

			if (EMath::Contains(boundingBox, boundingSphere) == false)
			{
				isFoundEnd = true;
				if (data.EX < bx - 1)
				{
					data.EX = bx - 1;
				}
				break;
			}
		}

		if (isFound == true)
		{
			if (isFoundEnd == false &&
				data.EX < bx)
			{
				data.EX = bx;
			}

			if (data.SY > by)
			{
				data.SY = by;
			}

			if(data.EY < by)
			{
				data.EY = by;
			}
		}
	}

	g_EnvironmentObjectsLock.Lock();
	mu_uint32 objectIndex = g_EnvironmentObjectsCount++;
	CreateObject(ThreadIndex, objectIndex, Type, Position, Angle, Scale, data);
	g_EnvironmentObjectsLock.Unlock();
}