#include "stdafx.h"
#include "mu_uibuffstatus.h"

#define SCREEN_WIDTH 800.0f
#define SCREEN_HEIGHT 600.0f

#define ICON_WIDTH 20.0f
#define ICON_HEIGHT 28.0f
#define ICON_SPACE 5.0f
#define ICON_UWIDTH (ICON_WIDTH / 200.0f)
#define ICON_VHEIGHT (ICON_HEIGHT / 224.0f)
#define ICON_LINE 8

UIBuffStatus::UIBuffStatus(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	SetGfxFlag(GFX_FLAG::GFX_NO_FOCUS);

	// Initialize
	{
		_BuffCount = 0;

		for (mu_uint32 n = 0; n < MAX_BUFFSTATUS; ++n)
		{
			_BuffList[n] = new UIButton(GAMEUI::MESSAGE_BUFFSTATUS_BEGIN, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, ICON_WIDTH, ICON_HEIGHT);
			_BuffList[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			
			_BuffType[n] = eBuffState::eBuffNone;
		}
	}
}

UIBuffStatus::~UIBuffStatus()
{

}

void UIBuffStatus::Update(mu_uint64 CurrentTime)
{

}

void UIBuffStatus::Draw()
{
	for (mu_uint32 n = 0; n < _BuffCount; ++n)
	{
		_BuffList[n]->Draw();
	}
}

void UIBuffStatus::PostDraw()
{
	UIBase *focused = _SceneManager->GetFocused();

	mu_float aspectWidth = (640.0f / SCREEN_WIDTH);
	mu_float aspectHeight = (480.0f / SCREEN_HEIGHT);

	if(focused != nullptr)
	{
		for (mu_uint32 n = 0; n < _BuffCount; ++n)
		{
			if (_BuffList[n] == focused)
			{
				eBuffState eBuffType = _BuffType[n];
				mu_float x = MU_TO_WINDOW(focused->_ViewX + ICON_WIDTH * 0.5f, aspectWidth);
				mu_float y = MU_TO_WINDOW(focused->_ViewY + ICON_HEIGHT + ICON_SPACE, aspectHeight);

				mu_int32 TextNum = 0;

				std::list<mu_string> tooltipinfo;
				g_BuffToolTipString(tooltipinfo, eBuffType);

				for (auto iter = tooltipinfo.begin(); iter != tooltipinfo.end(); ++iter)
				{
					mu_string& temp = *iter;

					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, temp.c_str());

					if (TextNum == 0)
					{
						TextListColor[TextNum] = TEXT_COLOR_BLUE;
						TextBold[TextNum] = true;
					}
					else
					{
						TextListColor[TextNum] = TEXT_COLOR_WHITE;
						TextBold[TextNum] = false;
					}

					++TextNum;
				}

				mu_string bufftime;
				g_BuffStringTime(eBuffType, bufftime);

				if (bufftime.size() != 0)
				{
					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2533], bufftime.c_str());

					TextListColor[TextNum] = TEXT_COLOR_PURPLE;
					TextBold[TextNum] = false;
					++TextNum;
				}

				RenderTipTextList(x, y, TextNum, 0, ETA_CENTER);
				break;
			}
		}
	}
}

UIBase *UIBuffStatus::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	for (mu_uint32 n = 0; n < _BuffCount; ++n)
	{
		UIBase *focus = _BuffList[n]->GetFocus(mouseX, mouseY);

		if (focus != nullptr)
		{
			return focus;
		}
	}

	return nullptr;
}

void UIBuffStatus::UpdateBuffList()
{
	OBJECT *o = &Hero->Object;

	std::vector<eBuffState> buffList, debuffList;
	_BuffCount = 0;

	for (auto it = o->m_BuffMap.m_Buff.begin(); it != o->m_BuffMap.m_Buff.end(); ++it)
	{
		//if (SetDisableRenderBuff(it->first) == true) continue;

		if (it->first != eBuffNone)
		{
			eBuffClass classType = g_IsBuffClass(it->first);

			if (classType == eBuffClass_Buff)
			{
				buffList.push_back(it->first);
			}
			else if (classType == eBuffClass_DeBuff)
			{
				debuffList.push_back(it->first);
			}
		}
	}

	for (auto it = buffList.begin(); it != buffList.end(); ++it)
	{
		eBuffState buffType = *it;

		mu_int32 index = _BuffCount++;
		mu_int32 interfaceIndex, widthIndex, heightIndex;

		if (buffType < eBuff_Berserker)
		{
			interfaceIndex = INTERFACES::BUFFWINDOW_STATUSICON1;
			widthIndex = (buffType - 1) % 10;
			heightIndex = (buffType - 1) / 10;
		}
		else
		{
			interfaceIndex = INTERFACES::BUFFWINDOW_STATUSICON2;
			widthIndex = (buffType - eBuff_Berserker) % 10;
			heightIndex = (buffType - eBuff_Berserker) / 10;
		}

		mu_float u = widthIndex * ICON_WIDTH / 200.0f;
		mu_float v = heightIndex * ICON_HEIGHT / 224.0f;

		_BuffList[index]->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[interfaceIndex], EVector2(u, v), EVector2(u + ICON_UWIDTH, v + ICON_VHEIGHT), MU::MakeARGB(100, 100, 100, 255)));
		_BuffList[index]->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[interfaceIndex], EVector2(u, v), EVector2(u + ICON_UWIDTH, v + ICON_VHEIGHT), MU::MakeARGB(255, 255, 255, 255)));
		_BuffList[index]->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[interfaceIndex], EVector2(u, v), EVector2(u + ICON_UWIDTH, v + ICON_VHEIGHT), MU::MakeARGB(255, 255, 255, 255)));
		_BuffList[index]->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[interfaceIndex], EVector2(u, v), EVector2(u + ICON_UWIDTH, v + ICON_VHEIGHT), MU::MakeARGB(255, 255, 255, 255)));
	
		_BuffType[index] = buffType;
	}

	for (auto it = debuffList.begin(); it != debuffList.end(); ++it)
	{
		eBuffState buffType = *it;

		mu_int32 index = _BuffCount++;
		mu_int32 interfaceIndex, widthIndex, heightIndex;

		if (buffType < eBuff_Berserker)
		{
			interfaceIndex = INTERFACES::BUFFWINDOW_STATUSICON1;
			widthIndex = (buffType - 1) % 10;
			heightIndex = (buffType - 1) / 10;
		}
		else
		{
			interfaceIndex = INTERFACES::BUFFWINDOW_STATUSICON2;
			widthIndex = (buffType - eBuff_Berserker) % 10;
			heightIndex = (buffType - eBuff_Berserker) / 10;
		}

		mu_float u = widthIndex * ICON_WIDTH / 200.0f;
		mu_float v = heightIndex * ICON_HEIGHT / 224.0f;

		_BuffList[index]->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[interfaceIndex], EVector2(u, v), EVector2(u + ICON_UWIDTH, v + ICON_VHEIGHT), MU::MakeARGB(100, 100, 100, 255)));
		_BuffList[index]->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[interfaceIndex], EVector2(u, v), EVector2(u + ICON_UWIDTH, v + ICON_VHEIGHT), MU::MakeARGB(255, 255, 255, 255)));
		_BuffList[index]->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[interfaceIndex], EVector2(u, v), EVector2(u + ICON_UWIDTH, v + ICON_VHEIGHT), MU::MakeARGB(255, 255, 255, 255)));
		_BuffList[index]->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[interfaceIndex], EVector2(u, v), EVector2(u + ICON_UWIDTH, v + ICON_VHEIGHT), MU::MakeARGB(255, 255, 255, 255)));

		_BuffType[index] = buffType;
	}

	this->UpdateWindow(GetScreenWidth());
}

void UIBuffStatus::UpdateWindow(mu_int32 screenWidth)
{
	mu_float x, y;
	switch (screenWidth)
	{
	case 640: x = 220.0f; y = 25.0f; break;
	case 450: x = 125.0f; y = 25.0f; break;
	case 373: x = 86.0f; y = 25.0f; break;
	case 260: x = 30.0f; y = 25.0f; break;
	default: x = 220.0f; y = 25.0f; break;
	}

	mu_float iconx = x;
	mu_float icony = y;
	mu_int32 iconcount = 0;

	for (mu_uint32 n = 0; n < _BuffCount; ++n)
	{
		_BuffList[n]->SetPosition(iconx, icony);
		
		if (++iconcount >= ICON_LINE)
		{
			iconx = x;
			icony += ICON_HEIGHT + ICON_SPACE;
			iconcount = 0;
		}
		else
		{
			iconx += ICON_WIDTH + ICON_SPACE;
		}
	}
}