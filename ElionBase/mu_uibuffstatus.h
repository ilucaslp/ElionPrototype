#ifndef __MU_UIBUFFSTATUS_H__
#define __MU_UIBUFFSTATUS_H__

#pragma once

#define MAX_BUFFSTATUS 32

class UIBuffStatus : public UIBase
{
public:
	UIBuffStatus(UISceneManager *manager);
	~UIBuffStatus();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void UpdateBuffList();
	void UpdateWindow(mu_int32 screenWidth);

private:
	mu_uint32 _BuffCount;
	UIButton *_BuffList[MAX_BUFFSTATUS];
	eBuffState _BuffType[MAX_BUFFSTATUS];
};

#endif