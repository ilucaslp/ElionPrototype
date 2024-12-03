#include "stdafx.h"
#include "mu_uiserverlist.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define GROUP_BUTTON_WIDTH 108.0f
#define GROUP_BUTTON_HEIGHT 26.0f
#define SERVER_BUTTON_WIDTH 193.0f
#define SERVER_BUTTON_HEIGHT 26.0f

#define LEFT_GROUP_POSX 140.0f
#define LEFT_GROUP_POSY 260.0f
#define RIGHT_GROUP_POSX 776.0f
#define RIGHT_GROUP_POSY 260.0f
#define CENTER_GROUP_POSX 458.0f
#define CENTER_GROUP_POSY 683.0f

#define EXTRA_BUTTON_SPACE 28.0f
#define SERVER_BUTTON_POSX (LEFT_GROUP_POSX + GROUP_BUTTON_WIDTH + EXTRA_BUTTON_SPACEX)

#define SERVER_GAUGE_POSX 16.0f
#define SERVER_GAUGE_POSY 19.0f
#define SERVER_GAUGE_WIDTH 160.0f
#define SERVER_GAUGE_HEIGHT 4.0f

UIServerList::UIServerList(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(0.0f, 0.0f);
	UIBase::SetDimensions(0.0f, 0.0f);

	// Initialize
	{
		for (mu_uint32 n = 0; n < SERVERLIST::MAX_SERVER_GROUP; ++n)
		{
			_GroupButton[n] = new UIToggleButton(LOGINUI::MESSAGE_SERVERGROUP_BUTTON + n, manager, this, 1024.0f, 768.0f, 0.0f, 0.0f, GROUP_BUTTON_WIDTH, GROUP_BUTTON_HEIGHT);
			_GroupButton[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_GroupButton[n]->SetStateData(UIToggleButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVERLIST_BACKGROUND], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(100, 100, 100, 255)));
			_GroupButton[n]->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVERLIST_BACKGROUND], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(255, 255, 255, 255)));
			_GroupButton[n]->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVERLIST_BACKGROUND], EVector2(0.0f, 0.25f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
			_GroupButton[n]->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVERLIST_BACKGROUND], EVector2(0.0f, 0.75f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
			_GroupButton[n]->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVERLIST_BACKGROUND], EVector2(0.0f, 0.75f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		}

		for (mu_uint32 n = 0; n < SERVERLIST::MAX_SERVER; ++n)
		{
			_ServerButton[n] = new UIButton(LOGINUI::MESSAGE_SERVER_BUTTON + n, manager, this, 1024.0f, 768.0f, 0.0f, 0.0f, SERVER_BUTTON_WIDTH, SERVER_BUTTON_HEIGHT);
			_ServerButton[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_ServerButton[n]->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVER_BACKGROUND], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
			_ServerButton[n]->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVER_BACKGROUND], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
			_ServerButton[n]->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVER_BACKGROUND], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
			_ServerButton[n]->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVER_BACKGROUND], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));

			_ServerGauge[n] = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_LOADBAR], 1024.0f, 768.0f, 0.0f, 0.0f, SERVER_GAUGE_WIDTH, SERVER_GAUGE_HEIGHT);
		}

		Clear();
	}
}

UIServerList::~UIServerList()
{

}

void UIServerList::Update(mu_uint64 CurrentTime)
{

}

void UIServerList::Draw()
{
	for (mu_uint32 n = 0; n < SERVERLIST::MAX_SERVER_GROUP; ++n)
	{
		_GroupButton[n]->Draw();
	}

	for (mu_uint32 n = 0; n < SERVERLIST::MAX_SERVER; ++n)
	{
		_ServerButton[n]->Draw();
		_ServerGauge[n]->Draw();
	}
}

void UIServerList::PostDraw()
{
	for (mu_uint32 n = 0; n < SERVERLIST::MAX_SERVER_GROUP; ++n)
	{
		_GroupButton[n]->PostDraw();
	}

	for (mu_uint32 n = 0; n < SERVERLIST::MAX_SERVER; ++n)
	{
		_ServerButton[n]->PostDraw();
	}
}

UIBase *UIServerList::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	for (mu_uint32 n = 0; n < SERVERLIST::MAX_SERVER_GROUP; ++n)
	{
		UIBase *focus = _GroupButton[n]->GetFocus(mouseX, mouseY);

		if (focus != nullptr)
		{
			return focus;
		}
	}

	for (mu_uint32 n = 0; n < SERVERLIST::MAX_SERVER; ++n)
	{
		UIBase *focus = _ServerButton[n]->GetFocus(mouseX, mouseY);

		if (focus != nullptr)
		{
			return focus;
		}
	}

	return nullptr;
}

void UIServerList::Clear()
{
	_GroupSelected = -1;
	_ServerSelected = -1;

	for (mu_uint32 g = 0; g < SERVERLIST::MAX_SERVER_GROUP; ++g)
	{
		GROUP_INFO &group = _ServerGroups[g];

		group.Active = false;
		group.GroupName.clear();

		for (mu_uint32 s = 0; s < SERVERLIST::MAX_SERVER; ++s)
		{
			SERVER_INFO &server = group.Servers[s];

			server.Active = false;
			server.ServerName.clear();
			server.ServerType = 0;
			server.ServerFlag = 0;
			server.Percent = 0;
		}
	}

	for (mu_uint32 n = 0; n < SERVERLIST::MAX_SERVER_GROUP; ++n)
	{
		_GroupButton[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}

	for (mu_uint32 n = 0; n < SERVERLIST::MAX_SERVER; ++n)
	{
		_ServerButton[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		_ServerGauge[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}
}

void UIServerList::ClearServer(mu_int32 groupIndex)
{
	if (groupIndex == -1 || groupIndex >= MAX_GROUP_COUNT) return;

	GROUP_INFO &group = _ServerGroups[groupIndex];
	 
	for (mu_uint32 n = 0; n < SERVERLIST::MAX_SERVER; ++n)
	{
		SERVER_INFO &server = group.Servers[n];

		server.Active = false;
		server.ServerName.clear();
		server.ServerType = 0;
		server.ServerFlag = 0;
		server.Percent = 0;

		_ServerButton[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		_ServerGauge[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}
}

void UIServerList::Setup()
{
	if (_GroupSelected != -1 &&
		_ServerGroups[_GroupSelected].Active == false)
	{
		_GroupSelected = -1;
	}

	for (mu_uint32 n = 0; n < SERVERLIST::MAX_SERVER_GROUP; ++n)
	{
		_GroupButton[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}

	for (mu_uint32 n = 0; n < SERVERLIST::MAX_SERVER; ++n)
	{
		_ServerButton[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		_ServerGauge[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}

	mu_uint32 groupPos = 0;

	for (mu_uint32 n = SERVERLIST::LEFT_GROUP_INDEX; n < SERVERLIST::RIGHT_GROUP_INDEX; ++n)
	{
		GROUP_INFO &group = _ServerGroups[n];
		UIToggleButton* &button = _GroupButton[n];

		if (group.Active == true)
		{
			button->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			button->SetPosition(LEFT_GROUP_POSX, LEFT_GROUP_POSY + groupPos * GROUP_BUTTON_HEIGHT);
			button->SetStateText(UIToggleButton::UI_DISABLED, FONT_NORMAL, group.GroupName.c_str(), MU::MakeARGB(150, 150, 150, 200));
			button->SetStateText(UIToggleButton::UI_NORMAL, FONT_NORMAL, group.GroupName.c_str(), MU::MakeARGB(226, 226, 226, 255));
			button->SetStateText(UIToggleButton::UI_OVER, FONT_NORMAL, group.GroupName.c_str(), MU::MakeARGB(255, 255, 255, 255));
			button->SetStateText(UIToggleButton::UI_PRESSED, FONT_NORMAL, group.GroupName.c_str(), MU::MakeARGB(226, 226, 226, 255));
			button->SetStateText(UIToggleButton::UI_PRESSED_OVER, FONT_NORMAL, group.GroupName.c_str(), MU::MakeARGB(226, 226, 226, 255));
			++groupPos;
		}
	}

	groupPos = 0;

	for (mu_uint32 n = SERVERLIST::RIGHT_GROUP_INDEX; n < SERVERLIST::CENTER_GROUP_INDEX; ++n)
	{
		GROUP_INFO &group = _ServerGroups[n];
		UIToggleButton* &button = _GroupButton[n];

		if (group.Active == true)
		{
			button->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			button->SetPosition(RIGHT_GROUP_POSX, RIGHT_GROUP_POSY + groupPos * GROUP_BUTTON_HEIGHT);
			button->SetStateText(UIToggleButton::UI_DISABLED, FONT_NORMAL, group.GroupName.c_str(), MU::MakeARGB(150, 150, 150, 200));
			button->SetStateText(UIToggleButton::UI_NORMAL, FONT_NORMAL, group.GroupName.c_str(), MU::MakeARGB(226, 226, 226, 255));
			button->SetStateText(UIToggleButton::UI_OVER, FONT_NORMAL, group.GroupName.c_str(), MU::MakeARGB(255, 255, 255, 255));
			button->SetStateText(UIToggleButton::UI_PRESSED, FONT_NORMAL, group.GroupName.c_str(), MU::MakeARGB(226, 226, 226, 255));
			button->SetStateText(UIToggleButton::UI_PRESSED_OVER, FONT_NORMAL, group.GroupName.c_str(), MU::MakeARGB(226, 226, 226, 255));
			++groupPos;
		}
	}

	groupPos = 0;

	for (mu_uint32 n = SERVERLIST::CENTER_GROUP_INDEX; n < SERVERLIST::MAX_SERVER_GROUP; ++n)
	{
		GROUP_INFO &group = _ServerGroups[n];
		UIToggleButton* &button = _GroupButton[n];

		if (group.Active == true)
		{
			button->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			button->SetPosition(CENTER_GROUP_POSX, CENTER_GROUP_POSY + groupPos * GROUP_BUTTON_HEIGHT);
			button->SetStateText(UIToggleButton::UI_DISABLED, FONT_NORMAL, group.GroupName.c_str(), MU::MakeARGB(150, 150, 150, 200));
			button->SetStateText(UIToggleButton::UI_NORMAL, FONT_NORMAL, group.GroupName.c_str(), MU::MakeARGB(226, 226, 226, 255));
			button->SetStateText(UIToggleButton::UI_OVER, FONT_NORMAL, group.GroupName.c_str(), MU::MakeARGB(255, 255, 255, 255));
			button->SetStateText(UIToggleButton::UI_PRESSED, FONT_NORMAL, group.GroupName.c_str(), MU::MakeARGB(226, 226, 226, 255));
			button->SetStateText(UIToggleButton::UI_PRESSED_OVER, FONT_NORMAL, group.GroupName.c_str(), MU::MakeARGB(226, 226, 226, 255));
			++groupPos;
		}
	}

	if (_GroupSelected != -1)
	{
		GROUP_INFO &group = _ServerGroups[_GroupSelected];
		UIToggleButton* &groupButton = _GroupButton[_GroupSelected];
		mu_uint32 serverCount = 0;

		for (mu_uint32 n = 0; n < SERVERLIST::MAX_SERVER; ++n)
		{
			if (group.Servers[n].Active == true)
			{
				++serverCount;
			}
		}

		mu_float serverBaseX, serverBaseY;

		if (_GroupSelected >= SERVERLIST::LEFT_GROUP_INDEX &&
			_GroupSelected < SERVERLIST::RIGHT_GROUP_INDEX)
		{
			serverBaseX = groupButton->_ViewX + GROUP_BUTTON_WIDTH + EXTRA_BUTTON_SPACE;
			serverBaseY = groupButton->_ViewY;

			if (serverBaseY + SERVER_BUTTON_HEIGHT * serverCount > CENTER_GROUP_POSY - EXTRA_BUTTON_SPACE)
			{
				serverBaseY = CENTER_GROUP_POSY - EXTRA_BUTTON_SPACE - SERVER_BUTTON_HEIGHT * serverCount;
			}
		}
		else if (_GroupSelected >= SERVERLIST::RIGHT_GROUP_INDEX &&
			_GroupSelected < SERVERLIST::CENTER_GROUP_INDEX)
		{
			serverBaseX = groupButton->_ViewX - EXTRA_BUTTON_SPACE - SERVER_BUTTON_WIDTH;
			serverBaseY = groupButton->_ViewY;

			if (serverBaseY + SERVER_BUTTON_HEIGHT * serverCount > CENTER_GROUP_POSY - EXTRA_BUTTON_SPACE)
			{
				serverBaseY = CENTER_GROUP_POSY - EXTRA_BUTTON_SPACE - SERVER_BUTTON_HEIGHT * serverCount;
			}
		}
		else
		{
			serverBaseX = groupButton->_ViewX + groupButton->_ViewWidth * 0.5f - SERVER_BUTTON_WIDTH * 0.5f;
			serverBaseY = groupButton->_ViewY - EXTRA_BUTTON_SPACE - SERVER_BUTTON_HEIGHT * serverCount;
		}

		mu_uint32 serverPos = 0;

		for (mu_uint32 n = 0; n < SERVERLIST::MAX_SERVER; ++n)
		{
			SERVER_INFO &server = group.Servers[n];
			UIButton* &button = _ServerButton[n];
			UISprite* &gauge = _ServerGauge[n];

			if (server.Active == true)
			{
				mu_float serverPercent = (mu_float)server.Percent / 100.0f;
				
				if (serverPercent < 0.0f)
					serverPercent = 0.0f;
				else if (serverPercent > 1.0f)
					serverPercent = 1.0f;

				button->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
				button->SetPosition(serverBaseX, serverBaseY + serverPos * SERVER_BUTTON_HEIGHT);

				switch (server.Percent)
				{
				case CS2CL::SERVER_OFFLINE:
					{
						button->ChangeState(UIButton::UI_DISABLED);
						gauge->SetColor(MU::MakeARGB(100, 100, 100, 255));
					}
					break;

				default:
					{
						button->ChangeState(UIButton::UI_NORMAL);
						gauge->SetColor(MU::MakeARGB(255, 255, 255, 255));
					}
					break;
				}

				switch (server.ServerType)
				{
				default:
				case CS2CL::SERVER_TYPE::ST_NORMAL:
					{
						if ((server.ServerFlag & CS2CL::SERVER_FLAG::SF_NONPVP) != 0)
						{
							button->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, server.ServerName.c_str(), MU::MakeARGB(150, 150, 71, 200), MU::MakeARGB(0, 0, 0, 0), 0.0f, -2.0f);
							button->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, server.ServerName.c_str(), MU::MakeARGB(255, 255, 121, 255), MU::MakeARGB(0, 0, 0, 0), 0.0f, -2.0f);
							button->SetStateText(UIButton::UI_OVER, FONT_NORMAL, server.ServerName.c_str(), MU::MakeARGB(255, 255, 121, 255), MU::MakeARGB(0, 0, 0, 0), 0.0f, -2.0f);
							button->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, server.ServerName.c_str(), MU::MakeARGB(255, 255, 121, 255), MU::MakeARGB(0, 0, 0, 0), 0.0f, -2.0f);
						}
						else
						{
							button->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, server.ServerName.c_str(), MU::MakeARGB(150, 150, 150, 200), MU::MakeARGB(0, 0, 0, 0), 0.0f, -2.0f);
							button->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, server.ServerName.c_str(), MU::MakeARGB(226, 226, 226, 255), MU::MakeARGB(0, 0, 0, 0), 0.0f, -2.0f);
							button->SetStateText(UIButton::UI_OVER, FONT_NORMAL, server.ServerName.c_str(), MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 0), 0.0f, -2.0f);
							button->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, server.ServerName.c_str(), MU::MakeARGB(226, 226, 226, 255), MU::MakeARGB(0, 0, 0, 0), 0.0f, -2.0f);
						}
					}
					break;

				case CS2CL::SERVER_TYPE::ST_GOLD:
					{
						button->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, server.ServerName.c_str(), MU::MakeARGB(150, 106, 0, 200), MU::MakeARGB(0, 0, 0, 0), 0.0f, -2.0f);
						button->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, server.ServerName.c_str(), MU::MakeARGB(255, 180, 0, 255), MU::MakeARGB(0, 0, 0, 0), 0.0f, -2.0f);
						button->SetStateText(UIButton::UI_OVER, FONT_NORMAL, server.ServerName.c_str(), MU::MakeARGB(255, 180, 0, 255), MU::MakeARGB(0, 0, 0, 0), 0.0f, -2.0f);
						button->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, server.ServerName.c_str(), MU::MakeARGB(255, 180, 0, 255), MU::MakeARGB(0, 0, 0, 0), 0.0f, -2.0f);
					}
					break;
				}

				gauge->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
				gauge->SetPosition(serverBaseX + SERVER_GAUGE_POSX, serverBaseY + serverPos * SERVER_BUTTON_HEIGHT + SERVER_GAUGE_POSY);
				gauge->SetDimensions(SERVER_GAUGE_WIDTH * serverPercent, SERVER_GAUGE_HEIGHT);
				gauge->SetUV(EVector2(0.0f, 0.0f), EVector2(serverPercent, 1.0f));
				++serverPos;
			}
		}
	}
}

const mu_string &UIServerList::GetSelectedGroupName()
{
	static mu_string emptyString = _T("");
	if (_GroupSelected < 0 || _GroupSelected >= SERVERLIST::MAX_SERVER_GROUP) return emptyString;
	return _ServerGroups[_GroupSelected].GroupName;
}

mu_int32 UIServerList::GetSelectedServerIndex()
{
	if (_GroupSelected < 0 || _GroupSelected >= SERVERLIST::MAX_SERVER_GROUP) return 0;
	if (_ServerSelected < 0 || _ServerSelected >= SERVERLIST::MAX_SERVER) return 0;
	return _ServerSelected;
}

mu_uint8 UIServerList::GetSelectedServerType()
{
	if (_GroupSelected < 0 || _GroupSelected >= SERVERLIST::MAX_SERVER_GROUP) return 0;
	if (_ServerSelected < 0 || _ServerSelected >= SERVERLIST::MAX_SERVER) return 0;
	return _ServerGroups[_GroupSelected].Servers[_ServerSelected].ServerType;
}

mu_uint8 UIServerList::GetSelectedServerFlag()
{
	if (_GroupSelected < 0 || _GroupSelected >= SERVERLIST::MAX_SERVER_GROUP) return 0;
	if (_ServerSelected < 0 || _ServerSelected >= SERVERLIST::MAX_SERVER) return 0;
	return _ServerGroups[_GroupSelected].Servers[_ServerSelected].ServerFlag;
}

const mu_string &UIServerList::GetGroupName(mu_int32 GroupIndex)
{
	static mu_string emptyString = _T("");
	if (GroupIndex < 0 || GroupIndex >= SERVERLIST::MAX_SERVER_GROUP) return emptyString;
	return _ServerGroups[GroupIndex].GroupName;
}