#include "stdafx.h"
#include "mu_uisimplenotice.h"

UISimpleNotice::UISimpleNotice(UISceneManager *manager)
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	Clear();
}

UISimpleNotice::~UISimpleNotice()
{

}

void UISimpleNotice::Draw()
{
	mu_float fontHeight = FONT_BOLD->GetFont()->GetLineHeight();
	mu_float screenWidth = MU_RESOURCE->_640x480Rate[0];
	mu_float screenHeight = MU_RESOURCE->_640x480Rate[1];

	mu_int32 n = 0;
	for (auto it = _Notices.begin();it != _Notices.end(); ++it, ++n)
	{
		UINoticeData &notice = *it;

		mu_uint32 backColor;
		mu_uint32 textColor;

		if (notice.Color == 0)
		{
			backColor = MU::MakeARGB(0, 0, 0, 128);
			if (g_CurrentTime % 1000 < 500)
			{
				textColor = MU::MakeARGB(255, 200, 80, 128);
			}
			else
			{
				textColor = MU::MakeARGB(255, 200, 80, 255);
			}
		}
		else
		{
			backColor = MU::MakeARGB(0, 0, 0, 128);
			textColor = MU::MakeARGB(100, 255, 200, 255);
		}

		FONT_BOLD->DrawStringC2D(TEXTINDEX::BEGIN_NOTICES + n,
			MU_TO_WINDOW(320.0f, screenWidth),
			MU_TO_WINDOW(300 + n * fontHeight, screenHeight),
			1.0f,
			ETA_CENTER,
			ETA_CENTER,
			0.0f,
			textColor,
			backColor,
			notice.Text.c_str());
	}
}

void UISimpleNotice::Clear()
{
	_NoticeTime = 300;
	_Notices.clear();
}

void UISimpleNotice::Create(const mu_string &Text, mu_uint8 Color)
{
	EVector2 Size;
	FONT_BOLD->GetTextExtentPoint(Text.c_str(), Text.size(), ETA_LEFT, 1.0f, &Size);

	Scroll();

	UINoticeData noticeData;

	noticeData.Color = Color;

	if (Size.x < 256.0f)
	{
		noticeData.Text = Text;
		_Notices.push_back(noticeData);
	}
	else
	{
		mu_string Temp1, Temp2;

		CutText(Text.c_str(), Temp1, Temp2, Text.size());
		noticeData.Text = Temp2;
		_Notices.push_back(noticeData);
		Scroll();
		noticeData.Color = Color;
		noticeData.Text = Temp1;
		_Notices.push_back(noticeData);
	}

	_NoticeTime = 300;
}

void UISimpleNotice::Move()
{
	if (_NoticeTime-- <= 0)
	{
		_NoticeTime = 300;
		Create(_T(""), 0);
	}
}

void UISimpleNotice::Scroll()
{
	if (_Notices.size() == MAX_NOTICE)
	{
		_Notices.erase(_Notices.begin());
	}
}