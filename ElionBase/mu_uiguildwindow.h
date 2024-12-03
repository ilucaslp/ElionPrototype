#ifndef __MU_UIGUILDWINDOW_H__
#define __MU_UIGUILDWINDOW_H__

#pragma once

#define GUILDWINDOW_POSX 450.0f
#define GUILDWINDOW_POSY 0.0f
#define GUILDWINDOW_WIDTH 190.0f
#define GUILDWINDOW_HEIGHT 429.0f

#define MAX_MEMBER_RENDER (17)
#define MAX_ALLIANCE_RENDER (6)

class UIGuildWindow : public UIBase
{
public:
	UIGuildWindow(UISceneManager *manager);
	~UIGuildWindow();

	virtual void InitializeGPU() override;
	virtual void ReleaseGPU() override;

	void ChangePosition(mu_float x, mu_float y);

	virtual mu_uint32 OnPreWindowMessage(const SDL_Event *event, mu_size *result) override;

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	void DrawFrames();
	void DrawButtons();
	void PostDrawButtons();
	void DrawTexts();

	void SetCurrentTab(mu_uint32 currentTab);

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	// Members Box
	void ClearMembers();
	void AddMember(GUILD_LIST_t &member);
	void DeleteMember(mu_int32 number);
	void SetMemberOffline(mu_int32 number);

	void ConfigureMemberScroll();
	void ConfigureMemberList();
	
	// Alliances Box
	void ClearAlliances();
	void AddAlliance(ALLIANCE_LIST_t &member);
	void DeleteAlliance(mu_uint32 number);

	void ConfigureAllianceScroll();
	void ConfigureAllianceList();

	virtual mu_boolean OnGFxMessage(mu_uint32 objectid, mu_uint32 message, mu_size message_data) override;

	void SetMarkTexture(MUTexture *texture);
	void SetGuildNotice(const mu_string &notice);

public:
	EINLINE GUILD_LIST_t *GetSelectedMember()
	{
		if (_MemberSelected == -1) return nullptr;

		for (auto mit = _Members.begin(); mit != _Members.end(); ++mit)
		{
			GUILD_LIST_t *member = &(*mit);

			if (member->CharacterIndex == _MemberSelected)
			{
				return member;
			}
		}

		return nullptr;
	}

public:
	enum
	{
		TAB_GUILD,
		TAB_MEMBERS,
		TAB_ALLIANCE,
		MAX_TAB,
	};

private:
	friend class UIGameRoot;
	UISprite *_BackFrame;
	UISprite *_TopFrame;
	UISprite *_BottomFrame;
	UISprite *_LeftFrame;
	UISprite *_RightFrame;

	UIButton *_XButton;
	UIButton *_CloseButton;

	UISprite *_TabBackground;
	UIRadioButton *_TabButtons[MAX_TAB];
	mu_uint32 _CurrentTab;

	// Guild Tab
	// Mark Box
	UISprite *_MarkTopLine;
	UISprite *_MarkBottomLine;
	UISprite *_MarkLeftLine;
	UISprite *_MarkRightLine;
	UISprite *_MarkTopLeft;
	UISprite *_MarkTopRight;
	UISprite *_MarkBottomLeft;
	UISprite *_MarkBottomRight;
	UISprite *_MarkBox;

	// Info Box
	UISprite *_InfoTopLine;
	UISprite *_InfoBottomLine;
	UISprite *_InfoLeftLine;
	UISprite *_InfoRightLine;
	UISprite *_InfoTopLeft;
	UISprite *_InfoTopRight;
	UISprite *_InfoBottomLeft;
	UISprite *_InfoBottomRight;

	// Announcement Box
	UISpriteColor *_AnnBack;
	UISprite *_AnnTopLine;
	UISprite *_AnnBottomLine;
	UISprite *_AnnLeftLine;
	UISprite *_AnnRightLine;
	UISprite *_AnnTab;
	UISprite *_AnnTopRight;
	UISprite *_AnnBottomLeft;
	UISprite *_AnnBottomRight;
	UIScrollBar *_AnnScrollBar;
	UIMultiLineEditBox *_AnnTextBox;

	UIButton *_LeaveGuildBtn;

	// Members Tab
	// Members Box
	UISpriteColor *_MemberColorABox;
	UISpriteColor *_MemberColorBBox;
	UISprite *_MemberTopLine;
	UISprite *_MemberBottomLine;
	UISprite *_MemberLeftLine;
	UISprite *_MemberRightLine;
	UISprite *_MemberTopLeft;
	UISprite *_MemberTopRight;
	UISprite *_MemberBottomLeft;
	UISprite *_MemberBottomRight;
	UIScrollBar *_MemberScrollBar;

	UIButton *_GivePositionBtn;
	UIButton *_FreePositionBtn;
	UIButton *_KickPlayerBtn;

	UIRadioButton *_MembersListBtn[MAX_MEMBER_RENDER];

	std::vector<GUILD_LIST_t> _Members;
	mu_int32 _MemberSelected;
	mu_uint32 _MemberListIndex;

	// Alliance Tab
	// Alliance Box
	UISpriteColor *_AllianceColorABox;
	UISpriteColor *_AllianceColorBBox;
	UISprite *_AllianceTopLine;
	UISprite *_AllianceBottomLine;
	UISprite *_AllianceLeftLine;
	UISprite *_AllianceRightLine;
	UISprite *_AllianceTopLeft;
	UISprite *_AllianceTopRight;
	UISprite *_AllianceBottomLeft;
	UISprite *_AllianceBottomRight;
	UIScrollBar *_AllianceScrollBar;

	UIButton *_AllianceCreateBtn;
	UIButton *_AllianceOutBtn;

	UIRadioButton *_AlliancesListBtn[MAX_ALLIANCE_RENDER];
	UISprite *_AlliancesMarksSprite[MAX_ALLIANCE_RENDER];

	MUTexture _AlliancesMarksTextures[MAX_ALLIANCE_RENDER];
	EStagingBufferBackend _AlliancesMarksStagingBuffer[MAX_ALLIANCE_RENDER];
	std::map<mu_uint32, ALLIANCE_LIST_t> _Alliances;
	mu_uint32 _AllianceSelected;
	mu_uint32 _AllianceListIndex;
};

#endif