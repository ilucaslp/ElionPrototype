#include "stdafx.h"
#include "mu_tasking.h"

void CreateShiny(const mu_uint32 ThreadIndex, OBJECT *o)
{
	EVector3 p, Position;
	if (o->SubType++ % 48 == 0)
	{
		EBone Matrix;
		AngleMatrix(o->Angle, Matrix);
		Vector((mu_float)(GetLargeRand(32) + 16), 0.0f, (mu_float)(GetLargeRand(32) + 16), p);
		VectorRotate(p, Matrix, Position);
		VectorAdd(o->Position, Position, Position);
		EVector3 Light;
		Vector(1.0f, 1.0f, 1.0f, Light);

		CreateParticle(ThreadIndex, BITMAP_SHINY, Position, o->Angle, Light);
		CreateParticle(ThreadIndex, BITMAP_SHINY, Position, o->Angle, Light, 1);
	}
}

void MUTasking::MoveItems(const mu_uint32 ThreadIndex)
{
	mu_uint32 q = ThreadIndex;
	for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
	{
		const mu_uint32 queueCount = MUThreadQueues::ItemsQueueCount[t];
		const mu_uint16 *queue = MUThreadQueues::ItemsQueue.data() + MUThreadQueues::ItemsQueueIndex[t];

		for (; q < queueCount; q += _ThreadsCount)
		{
			const mu_uint32 itemIndex = queue[q];
			ITEM_t &item = Items[itemIndex];
			OBJECT *o = &item.Object;

			if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
			{
				o->Position[2] += o->Gravity;
				o->Gravity -= 6.0f;
				mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]) + 30.0f;
				if (o->Type >= MODEL_SWORD && o->Type < MODEL_STAFF + MAX_ITEM_INDEX)
					Height += 40.0f;
				if (o->Position[2] <= Height)
				{
					o->Position[2] = Height;
					ItemAngle(o);
				}
				else
				{
					if (o->Type >= MODEL_SHIELD && o->Type < MODEL_SHIELD + MAX_ITEM_INDEX)
						o->Angle[1] = -o->Gravity*10.0f;
					else
						o->Angle[0] = -o->Gravity*10.0f;
				}
				CreateShiny(ThreadIndex, o);
			}
		}

		q -= queueCount;
	}
}

void ItemHeight(const mu_uint32 ThreadIndex, mu_int32 Type, MUModel *b)
{
	if (Type >= MODEL_HELM && Type < MODEL_HELM + MAX_ITEM_INDEX)
		b->BodyHeight(ThreadIndex) = -160.0f;
	else if (Type >= MODEL_ARMOR && Type < MODEL_ARMOR + MAX_ITEM_INDEX)
		b->BodyHeight(ThreadIndex) = -100.0f;
	else if (Type >= MODEL_GLOVES && Type < MODEL_GLOVES + MAX_ITEM_INDEX)
		b->BodyHeight(ThreadIndex) = -70.0f;
	else if (Type >= MODEL_PANTS && Type < MODEL_PANTS + MAX_ITEM_INDEX)
		b->BodyHeight(ThreadIndex) = -50.0f;
	else if (Type >= MODEL_BOOTS && Type < MODEL_BOOTS + MAX_ITEM_INDEX)
		b->BodyHeight(ThreadIndex) = 0.0f;
	else
		b->BodyHeight(ThreadIndex) = 0.0f;
}

void MUTasking::CheckItems(const mu_uint32 ThreadIndex)
{
	mu_uint32 queueCount = 0;
	const mu_uint32 queueIndex = MUThreadQueues::ItemsQueueIndex[ThreadIndex];

	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MAX_ITEMS, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
	{
		OBJECT *o = &Items[i].Object;
		if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
		{
			const mu_boolean isVisible = TestFrustrum2D(o->Position, 100.0f);
			if (isVisible)
			{
				o->Flags.Set(OBJECT::eOF_VISIBLE);
			}
			else
			{
				o->Flags.Clear(OBJECT::eOF_VISIBLE);
			}

			if (isVisible)
			{
				MUThreadQueues::ItemsQueue[queueIndex + queueCount++] = i;
			}
		}
	}

	MUThreadQueues::ItemsQueueCount[ThreadIndex] = queueCount;
}

void MUTasking::PrepareItems(const mu_uint32 ThreadIndex)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_uint32 q = ThreadIndex;
	for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
	{
		const mu_uint32 queueCount = MUThreadQueues::ItemsQueueCount[t];
		const mu_uint16 *queue = MUThreadQueues::ItemsQueue.data() + MUThreadQueues::ItemsQueueIndex[t];

		for (; q < queueCount; q += _ThreadsCount)
		{
			const mu_uint32 itemIndex = queue[q];
			ITEM_t &item = Items[itemIndex];
			OBJECT *o = &item.Object;

			ThreadInfo.RenderIndex = 0;
			ThreadInfo.CulledType = CULLED_ITEMS;
			ThreadInfo.ObjectIndex = itemIndex;

			mu_int32 Type = o->Type;

			if (o->Type >= MODEL_HELM && o->Type < MODEL_BOOTS + MAX_ITEM_INDEX)
			{
				Type = MODEL_PLAYER;
			}
			else if (o->Type == MODEL_POTION + 12)
			{
				mu_int32 Level = item.Item.Level;

				if (Level == 0)
					Type = MODEL_EVENT;
				else if (Level == 2)
					Type = MODEL_EVENT + 1;
			}

			MUModel *b = &MODELS::Data[Type];

			b->Skin(ThreadIndex) = GetBaseClass(Hero->Class);
			b->CurrentAction(ThreadIndex) = o->CurrentAction;
			VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));

			ItemHeight(ThreadIndex, o->Type, b);
			b->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle, false, false);
			ThreadInfo.SkeletalIndex = g_Tasking.RegisterSkeletal(ThreadInfo.BoneTransform, b->_BoneNonDummyEnd);
			o->BoneTransform = &g_Tasking._SkeletonBuffer[ThreadInfo.SkeletalIndex];

			if (o->Type >= MODEL_HELM && o->Type < MODEL_BOOTS + MAX_ITEM_INDEX)
			{
				Type = o->Type;
			}

			b = &MODELS::Data[Type];
			EVector3 Light;
			RequestTerrainLight(o->Position[0], o->Position[1], Light);
			VectorAdd(Light, o->Light, Light);

			if (o->Type == MODEL_POTION + 15)
			{
				EVector3 Temp;
				VectorCopy(o->Position, Temp);
				mu_int32 Count = (mu_int32)(EMath::Sqrt((mu_double)item.Item.Money) * 0.5);
				if (Count < 3) Count = 3;
				if (Count > 80) Count = 80;
				mu_float minx = Temp[0] + o->BoundingBoxMin[0];
				mu_float maxx = Temp[0] + o->BoundingBoxMax[0];
				mu_float miny = Temp[1] + o->BoundingBoxMin[1];
				mu_float maxy = Temp[1] + o->BoundingBoxMax[1];
				for (mu_int32 j = 1; j < Count; ++j)
				{
					EVector3 Angle;
					EBone Matrix;
					EVector3 p, Position;
					Vector(0.0f, 0.0f, (mu_float)(RandomTable[(itemIndex * 20 + j) % 100] % 360), Angle);
					Vector((mu_float)(RandomTable[(itemIndex + j) % 100] % (Count + 20)), 0.0f, 0.0f, p);
					AngleMatrix(Angle, Matrix);
					VectorRotate(p, Matrix, Position);
					VectorAdd(Temp, Position, o->Position);

					if (o->Position[0] < minx) o->Position[0] = minx;
					else if (o->Position[0] > maxx) o->Position[0] = maxx;
					if (o->Position[1] < miny) o->Position[1] = miny;
					else if (o->Position[1] > maxy) o->Position[1] = maxy;

					RenderPartObject(ThreadIndex, o, o->Type, nullptr, Light, o->Alpha, 0, 0, 0, true, true, true);
				}
				VectorCopy(Temp, o->Position);
			}
			else
			{
				EVector3 vBackup;
				VectorCopy(o->Position, vBackup);
				if (GET_WORLD == WD_10HEAVEN)
				{
					o->Position[2] += 10.0f * (mu_float)EMath::Sin((mu_float)(itemIndex * 1237 + FWorldTime)*0.002f);
				}
				else if (GET_WORLD == WD_39KANTURU_3RD && g_Direction.m_CKanturu.IsMayaScene())
				{
					o->Position[2] += 10.0f * (mu_float)EMath::Sin((mu_float)(itemIndex * 1237 + FWorldTime)*0.002f);
				}
				else if (InHellas() == true)
				{
					o->Position[2] = GetWaterTerrain(o->Position[0], o->Position[1]) + 180.0f;
				}

				RenderPartObject(ThreadIndex, o, o->Type, nullptr, Light, o->Alpha, item.Item.Level, item.Item.ExcOption, item.Item.SetOption, true, true, true);
				VectorCopy(vBackup, o->Position);
			}
		}

		q -= queueCount;
	}
}