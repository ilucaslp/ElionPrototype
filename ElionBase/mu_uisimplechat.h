#ifndef __MU_UISIMPLECHAT_H__
#define __MU_UISIMPLECHAT_H__

#pragma once

#define MAX_CHAT 500

class UIChatData
{
public:
	UIChatData()
	{
		Owner = nullptr;

		IDLifeTime = 0;
		LifeTime[0] = 0;
		LifeTime[1] = 0;

		X = 0.0f;
		Y = 0.0f;
		Width = 0.0f;
		Height = 0.0f;

		Position = EVector3(0.0f, 0.0f, 0.0f);

		PkColor = 0;
		GuildColor = 0;

		for (mu_uint32 i = 0; i < mu_countof(TextIndex); ++i)
		{
			TextIndex[i] = g_RandomTextIndex.fetch_add(1);
		}

		MouseOver = false;
	}

public:
	CHARACTER *Owner;

	mu_string ID;
	mu_string Union;
	mu_string Guild;
	mu_string ShopTitle;

	mu_string Text[2];

	mu_int32 IDLifeTime;
	mu_int32 LifeTime[2];

	mu_float X;
	mu_float Y;
	mu_float Width;
	mu_float Height;

	EVector3 Position;

	mu_uint8 PkColor;
	mu_uint8 GuildColor;
	
	// Internal usage only
	mu_uint32 TextIndex[7];

	mu_boolean Visible;
	mu_boolean MouseOver;
	mu_uint64 MouseTime;
};

class UISimpleChat : public UIBase
{
public:
	UISimpleChat(UISceneManager *manager);
	~UISimpleChat();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;

	void RenderBoolean(mu_float x, mu_float y, UIChatData *c);

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void MultithreadUpdate(const mu_uint32 ThreadIndex);

	void Clear();
	void Create(const mu_text *ID, const mu_text *Text, CHARACTER *Owner, mu_int32 Flag = 0, mu_int32 PkColor = -1);
	mu_int32 Create(const mu_text *ID, const mu_text *Text, OBJECT *Owner, mu_int32 LifeTime, mu_int32 PkColor = -1);
	void Assign(mu_int32 Key, const mu_text *ID, const mu_text *Text, mu_int32 Flag = 0);
	void Delete(CHARACTER *Owner);
	
	void AddChat(UIChatData *c, const mu_text *Text, mu_size TextLength, mu_int32 Flag);
	void AddGuildName(UIChatData *c, CHARACTER *Owner);

	void SetBooleanPosition(UIChatData *c);

private:
	UIChatData _Chat[MAX_CHAT];
};

EINLINE mu_size GetClosestBlankFromCenter(const mu_string wstrTarget)
{
	mu_size iLength = wstrTarget.length();

	mu_string wstrText1 = wstrTarget.substr(iLength / 2, mu_string::npos);
	mu_string wstrText2 = wstrTarget.substr(0, iLength / 2);

	mu_size iPosLastBlankFromFirstHalf = wstrText2.find_last_of(L" ");
	mu_size iPosFirstBlankFromSecondHalf = wstrText1.find_first_of(L" ", 1);
	mu_size iClosestBlankFromCenter = 0;

	if (iPosLastBlankFromFirstHalf == mu_string::npos && iPosFirstBlankFromSecondHalf == mu_string::npos)
	{
		iClosestBlankFromCenter = iLength / 2;
	}
	else if (iPosLastBlankFromFirstHalf == mu_string::npos)
	{
		iClosestBlankFromCenter = iPosFirstBlankFromSecondHalf + iLength / 2;
	}
	else if (iPosFirstBlankFromSecondHalf == mu_string::npos)
	{
		iClosestBlankFromCenter = iPosLastBlankFromFirstHalf;
	}
	else if (iPosFirstBlankFromSecondHalf <= iLength / 2 - iPosLastBlankFromFirstHalf)
	{
		iClosestBlankFromCenter = iPosFirstBlankFromSecondHalf + iLength / 2;
	}
	else
	{
		iClosestBlankFromCenter = iPosLastBlankFromFirstHalf;
	}

	return iClosestBlankFromCenter;
}

#endif