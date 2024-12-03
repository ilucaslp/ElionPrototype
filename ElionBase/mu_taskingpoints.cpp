#include "stdafx.h"
#include "mu_tasking.h"

void MUTasking::MovePoints(const mu_uint32 ThreadIndex)
{
	for (mu_uint32 n = ThreadIndex; n < MAX_POINTS; n += _ThreadsCount)
	{
		MUPoint *o = &g_Points[n];

		if(o->Flags.Get(MUPoint::ePF_LIVE) == true)
		{
			if (o->Flags.Get(MUPoint::ePF_MOVE) == true)
			{
				o->Position[2] += o->Gravity;
			}

			o->Gravity -= 0.3f;

			if (o->Gravity <= 0.0f)
			{
				o->Flags.Clear(MUPoint::ePF_LIVE);
				g_PointsDeleteCount.fetch_add(1);
			}

			if (o->Type != -2)
			{
				o->Scale -= 5.0f;

				if (o->Scale < 15.0f)
				{
					o->Scale = 15.0f;
				}
			}
		}
	}
}

void MUTasking::RenderPoints()
{
	for (mu_uint32 n = 0; n < MAX_POINTS; ++n)
	{
		MUPoint *o = &g_Points[n];

		if (o->Flags.Get(MUPoint::ePF_LIVE) == false)
		{
			continue;
		}

		EVector4 Color = EVector4(o->Color, o->Gravity * 0.4f);

		if (Color.w > 0.0f)
		{
			if (Color.w > 1.0f)
			{
				Color.w = 1.0f;
			}

			FONT_NUMBER->DrawStringOutlineC3D(TEXTINDEX::BEGIN_POINTS + n,
				o->Position.x, o->Position.y, o->Position.z,
				o->Scale * 0.06f,
				ETA_CENTER, ETA_CENTER, 0.0f,
				MU::Float4ToARGB(Color), 0,
				MU::MakeARGB(0, 0, 0, 255),
				o->Text.c_str());
		}
	}
}