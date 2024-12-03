#ifndef __MU_UISIMPLENOTICE_H__
#define __MU_UISIMPLENOTICE_H__

#pragma once

#define MAX_NOTICE 6

class UINoticeData
{
public:
	UINoticeData()
	{
		Color = 0;
	}

public:
	mu_string Text;
	mu_uint8 Color;
};

class UISimpleNotice : public UIBase
{
public:
	UISimpleNotice(UISceneManager *manager);
	~UISimpleNotice();

	virtual void Draw() override;

	void Clear();

	void Create(const mu_string &Text, mu_uint8 Color);
	void Move();
	void Scroll();

private:
	mu_int32 _NoticeTime;
	std::vector<UINoticeData> _Notices;
};

#endif