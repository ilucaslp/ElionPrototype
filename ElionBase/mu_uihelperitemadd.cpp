#include "stdafx.h"
#include "mu_uihelperitemadd.h"
#include "mu_uihelperitemadd_defines.h"

UIHelperItemAdd::UIHelperItemAdd(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(WINBOX_POSX, WINBOX_POSY);
	UIBase::SetDimensions(WINBOX_WIDTH, WINBOX_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_NO_FOCUS | GFX_FLAG::GFX_HIDDEN);

	// Initialize
	{
		TMSGBOX_PREPARE(manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, WINBOX_POSX, WINBOX_POSY, WINBOX_WIDTH, WINBOX_HEIGHT);
		TGLAYOUT_PREPARE(MainBack, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MAIN_BACK_POSX, MAIN_BACK_POSY, MAIN_BACK_WIDTH, MAIN_BACK_HEIGHT);

		_MainDiv = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], SCREEN_WIDTH, SCREEN_HEIGHT, MAIN_DIV_POSX, MAIN_DIV_POSY, MAIN_DIV_WIDTH, MAIN_DIV_HEIGHT, EVector2(0.0f, 0.0f), EVector2(1.0f, 4.0f / 12.0f));
		_MainDiv->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_RenderDiv = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX02], SCREEN_WIDTH, SCREEN_HEIGHT, RENDER_DIV_POSX, RENDER_DIV_POSY, RENDER_DIV_WIDTH, RENDER_DIV_HEIGHT, EVector2(0.0f, 0.0f), EVector2(0.5f, 1.0f));
		_RenderDiv->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		TGLAYOUT_PREPARE(TypeBack, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, TYPE_BACK_POSX, TYPE_BACK_POSY, TYPE_BACK_WIDTH, TYPE_BACK_HEIGHT);
		TGLAYOUT_PREPARE(ItemBack, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ITEM_BACK_POSX, ITEM_BACK_POSY, ITEM_BACK_WIDTH, ITEM_BACK_HEIGHT);

		_TypeScrollBar = new UIScrollBar(MESSAGEBOX::MESSAGE_HELPERITEMADD_TYPE_SCROLL, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, TYPE_SCROLL_POSX, TYPE_SCROLL_POSY, UIScrollBar::ORI_VERTICAL);
		_TypeScrollBar->ConfigureTop(&INTERFACES::Data[INTERFACES::SCROLLBAR_TOP], EBlendState::eAlphaSrc_InvAlphaSrc, TYPE_SCROLL_BACKWIDTH, TYPE_SCROLL_TOP_BOTTOM_PART_HEIGHT);
		_TypeScrollBar->ConfigureMiddle(&INTERFACES::Data[INTERFACES::SCROLLBAR_MIDDLE], EBlendState::eAlphaSrc_InvAlphaSrc, TYPE_SCROLL_BACKWIDTH, TYPE_SCROLL_LINESHEIGHT);
		_TypeScrollBar->ConfigureBottom(&INTERFACES::Data[INTERFACES::SCROLLBAR_BOTTOM], EBlendState::eAlphaSrc_InvAlphaSrc, TYPE_SCROLL_BACKWIDTH, TYPE_SCROLL_TOP_BOTTOM_PART_HEIGHT);
		_TypeScrollBar->ConfigureScroll(EBlendState::eAlphaSrc_InvAlphaSrc, TYPE_SCROLL_BTN_WIDTH, TYPE_SCROLL_BTN_HEIGHT, true);

		_TypeScrollBar->SetStateData(UIScrollBar::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_OFF], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_TypeScrollBar->SetStateData(UIScrollBar::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_TypeScrollBar->SetStateData(UIScrollBar::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_TypeScrollBar->SetStateData(UIScrollBar::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(178, 178, 178, 255)));

		_TypeScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
		_TypeScrollBar->ConfigureScrollDimension(TYPE_SCROLL_LINESHEIGHT);
		_TypeScrollBar->ChangeState(UIScrollBar::UI_DISABLED);
		_TypeScrollBar->SetMessageInheritor(this);

		_TypeScrollBar->ApplyChanges();

		_ItemScrollBar = new UIScrollBar(MESSAGEBOX::MESSAGE_HELPERITEMADD_ITEM_SCROLL, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ITEM_SCROLL_POSX, ITEM_SCROLL_POSY, UIScrollBar::ORI_VERTICAL);
		_ItemScrollBar->ConfigureTop(&INTERFACES::Data[INTERFACES::SCROLLBAR_TOP], EBlendState::eAlphaSrc_InvAlphaSrc, ITEM_SCROLL_BACKWIDTH, ITEM_SCROLL_TOP_BOTTOM_PART_HEIGHT);
		_ItemScrollBar->ConfigureMiddle(&INTERFACES::Data[INTERFACES::SCROLLBAR_MIDDLE], EBlendState::eAlphaSrc_InvAlphaSrc, ITEM_SCROLL_BACKWIDTH, ITEM_SCROLL_LINESHEIGHT);
		_ItemScrollBar->ConfigureBottom(&INTERFACES::Data[INTERFACES::SCROLLBAR_BOTTOM], EBlendState::eAlphaSrc_InvAlphaSrc, ITEM_SCROLL_BACKWIDTH, ITEM_SCROLL_TOP_BOTTOM_PART_HEIGHT);
		_ItemScrollBar->ConfigureScroll(EBlendState::eAlphaSrc_InvAlphaSrc, ITEM_SCROLL_BTN_WIDTH, ITEM_SCROLL_BTN_HEIGHT, true);

		_ItemScrollBar->SetStateData(UIScrollBar::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_OFF], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ItemScrollBar->SetStateData(UIScrollBar::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ItemScrollBar->SetStateData(UIScrollBar::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ItemScrollBar->SetStateData(UIScrollBar::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(178, 178, 178, 255)));

		_ItemScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
		_ItemScrollBar->ConfigureScrollDimension(ITEM_SCROLL_LINESHEIGHT);
		_ItemScrollBar->ChangeState(UIScrollBar::UI_DISABLED);
		_ItemScrollBar->SetMessageInheritor(this);

		_ItemScrollBar->ApplyChanges();

		for (mu_uint32 n = 0; n < MAX_ITEMADD_SHOWTYPE; ++n)
		{
			_TypesListBtn[n] = new UIRadioButton(MESSAGEBOX::MESSAGE_HELPERITEMADD_TYPE_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, TYPE_LIST_POSX, TYPE_LIST_POSY(n), TYPE_LIST_WIDTH, TYPE_LIST_HEIGHT);
			_TypesListBtn[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_TypesListBtn[n]->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 0, 0, 0)));
			_TypesListBtn[n]->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 220)));
			_TypesListBtn[n]->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 180)));
			_TypesListBtn[n]->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 220)));
			_TypesListBtn[n]->SetRouteToParent(true);
		}

		for (mu_uint32 n = 0; n < MAX_ITEMADD_SHOWITEM; ++n)
		{
			_ItemsListBtn[n] = new UIRadioButton(MESSAGEBOX::MESSAGE_HELPERITEMADD_ITEM_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ITEM_LIST_POSX, ITEM_LIST_POSY(n), ITEM_LIST_WIDTH, ITEM_LIST_HEIGHT);
			_ItemsListBtn[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_ItemsListBtn[n]->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 0, 0, 0)));
			_ItemsListBtn[n]->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 220)));
			_ItemsListBtn[n]->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 180)));
			_ItemsListBtn[n]->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 220)));
			_ItemsListBtn[n]->SetRouteToParent(true);
		}

		_DecLevelBtn = new UIButton(MESSAGEBOX::MESSAGE_HELPERITEMADD_DECLEVEL_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, DECLEVEL_POSX, DECLEVEL_POSY, DECLEVEL_WIDTH, DECLEVEL_HEIGHT);
		_DecLevelBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_DecLevelBtn->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::INGAMESHOP_PAGE_LEFT2], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_DecLevelBtn->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::INGAMESHOP_PAGE_LEFT2], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_DecLevelBtn->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::INGAMESHOP_PAGE_LEFT2], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_DecLevelBtn->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::INGAMESHOP_PAGE_LEFT2], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));

		_IncLevelBtn = new UIButton(MESSAGEBOX::MESSAGE_HELPERITEMADD_INCLEVEL_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, INCLEVEL_POSX, INCLEVEL_POSY, INCLEVEL_WIDTH, INCLEVEL_HEIGHT);
		_IncLevelBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_IncLevelBtn->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::INGAMESHOP_PAGE_RIGHT2], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_IncLevelBtn->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::INGAMESHOP_PAGE_RIGHT2], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_IncLevelBtn->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::INGAMESHOP_PAGE_RIGHT2], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_IncLevelBtn->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::INGAMESHOP_PAGE_RIGHT2], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));

		_LuckCheckBox = new UIToggleButton(MESSAGEBOX::MESSAGE_HELPERITEMADD_LUCK_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, LUCK_POSX, LUCK_POSY, LUCK_WIDTH, LUCK_HEIGHT);
		_LuckCheckBox->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_LuckCheckBox->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_LuckCheckBox->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_LuckCheckBox->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_LuckCheckBox->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));

		_SkillCheckBox = new UIToggleButton(MESSAGEBOX::MESSAGE_HELPERITEMADD_SKILL_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, SKILL_POSX, SKILL_POSY, SKILL_WIDTH, SKILL_HEIGHT);
		_SkillCheckBox->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_SkillCheckBox->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_SkillCheckBox->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_SkillCheckBox->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_SkillCheckBox->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));

		_OptionCheckBox = new UIToggleButton(MESSAGEBOX::MESSAGE_HELPERITEMADD_OPTION_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, OPTION_POSX, OPTION_POSY, OPTION_WIDTH, OPTION_HEIGHT);
		_OptionCheckBox->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_OptionCheckBox->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_OptionCheckBox->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_OptionCheckBox->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_OptionCheckBox->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));

		_ExcellentCheckBox = new UIToggleButton(MESSAGEBOX::MESSAGE_HELPERITEMADD_EXC_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, EXC_POSX, EXC_POSY, EXC_WIDTH, EXC_HEIGHT);
		_ExcellentCheckBox->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_ExcellentCheckBox->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_ExcellentCheckBox->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ExcellentCheckBox->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_ExcellentCheckBox->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));

		_SetItemCheckBox = new UIToggleButton(MESSAGEBOX::MESSAGE_HELPERITEMADD_SETITEM_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, SETITEM_POSX, SETITEM_POSY, SETITEM_WIDTH, SETITEM_HEIGHT);
		_SetItemCheckBox->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_SetItemCheckBox->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_SetItemCheckBox->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_SetItemCheckBox->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_SetItemCheckBox->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));

		_SocketCheckBox = new UIToggleButton(MESSAGEBOX::MESSAGE_HELPERITEMADD_SOCKET_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, SOCKET_POSX, SOCKET_POSY, SOCKET_WIDTH, SOCKET_HEIGHT);
		_SocketCheckBox->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_SocketCheckBox->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_SocketCheckBox->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_SocketCheckBox->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_SocketCheckBox->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));

		_AcceptButton = new UIButton(MESSAGEBOX::MESSAGE_HELPERITEMADD_ACCEPT_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ACCEPT_POSX, ACCEPT_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_AcceptButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AcceptButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_AcceptButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_AcceptButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_AcceptButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));

		_CancelButton = new UIButton(MESSAGEBOX::MESSAGE_HELPERITEMADD_CANCEL_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CANCEL_POSX, CANCEL_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_CancelButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CancelButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_CancelButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
	
		_MinItemLevel = 0;
		_TypeListIndex = 0;
		_ItemListIndex = 0;
		_TypeSelected = -1;
		_ItemSelected = -1;

		_TypeIndex = -1;
		_ItemIndex = -1;
		_EnableItemInfo = false;

		InitializeItemsData();
	}
}

UIHelperItemAdd::~UIHelperItemAdd()
{

}

void UIHelperItemAdd::InitializeItemsData()
{
	ConfigureTypeScroll();
	ConfigureItemScroll();
	ConfigureTypeList();
	ConfigureItemList();
}

void UIHelperItemAdd::ConfigureTypeScroll()
{
	mu_boolean isEnabledScroll = _TypeScrollBar->GetState() != UIScrollBar::UI_DISABLED;

	if (g_ItemCategoriesDatabase._Categories.size() > MAX_ITEMADD_SHOWTYPE &&
		isEnabledScroll == false)
	{
		_TypeListIndex = 0;
		_TypeScrollBar->ChangeState(UIScrollBar::UI_NORMAL);
		_TypeScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
	}
	else if (g_ItemCategoriesDatabase._Categories.size() <= MAX_ITEMADD_SHOWTYPE &&
		isEnabledScroll == true)
	{
		_TypeListIndex = 0;
		_TypeScrollBar->ChangeState(UIScrollBar::UI_DISABLED);
		_TypeScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
	}

	if (_TypeScrollBar->GetState() != UIScrollBar::UI_DISABLED)
	{
		mu_uint32 maxIndex = g_ItemCategoriesDatabase._Categories.size() - MAX_ITEMADD_SHOWTYPE;
		if (_TypeListIndex > maxIndex)
		{
			_TypeListIndex = maxIndex;
			_TypeScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
		}
	}
}

void UIHelperItemAdd::ConfigureItemScroll()
{
	if (_TypeSelected < 0) return;

	MUItemCategoriesData &typeData = g_ItemCategoriesDatabase._Categories[_TypeSelected];
	mu_boolean isEnabledScroll = _ItemScrollBar->GetState() != UIScrollBar::UI_DISABLED;

	if (typeData.Items.size() > MAX_ITEMADD_SHOWITEM &&
		isEnabledScroll == false)
	{
		_ItemScrollBar->ChangeState(UIScrollBar::UI_NORMAL);
	}
	else if (typeData.Items.size() <= MAX_ITEMADD_SHOWITEM &&
		isEnabledScroll == true)
	{
		_ItemListIndex = 0;
		_ItemScrollBar->ChangeState(UIScrollBar::UI_DISABLED);
		_ItemScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
	}

	if (_ItemScrollBar->GetState() != UIScrollBar::UI_DISABLED)
	{
		if (_ItemSelected >= MAX_ITEMADD_SHOWITEM)
		{
			MUItemCategoriesData &typeData = g_ItemCategoriesDatabase._Categories[_TypeSelected];

			_ItemListIndex = _ItemSelected - (MAX_ITEMADD_SHOWTYPE - 1);
			mu_uint32 maxIndex = typeData.Items.size() - MAX_ITEMADD_SHOWITEM;
			_ItemScrollBar->ConfigureScrollInfo(0.0f, 1.0f, (mu_float)_ItemListIndex / (mu_float)maxIndex);
		}
		else
		{
			_ItemListIndex = 0;
			_ItemScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
		}
	}
}

void UIHelperItemAdd::CheckTypeList()
{
	if (_TypeSelected >= 0 &&
		_TypeSelected == _TypeIndex)
	{
		_ItemSelected = _ItemIndex;
	}
}

void UIHelperItemAdd::ConfigureTypeList()
{
	UIBase *focused = _SceneManager->GetFocused();

	mu_uint32 maxIndex = g_ItemCategoriesDatabase._Categories.size() > MAX_ITEMADD_SHOWTYPE ? MAX_ITEMADD_SHOWTYPE : g_ItemCategoriesDatabase._Categories.size();
	for (mu_uint32 n = 0; n < maxIndex; ++n)
	{
		UIRadioButton *btn = _TypesListBtn[n];

		if (_TypeSelected == _TypeListIndex + n)
		{
			btn->ChangeState(focused == btn ? UIRadioButton::UI_PRESSED_OVER : UIRadioButton::UI_PRESSED);
		}
		else
		{
			btn->ChangeState(focused == btn ? UIRadioButton::UI_OVER : UIRadioButton::UI_NORMAL);
		}

		btn->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 0, 0, 0)));
		btn->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 51, 204, 150)));
		btn->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 51, 204, 110)));
		btn->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 51, 204, 150)));
	}
}

void UIHelperItemAdd::ConfigureItemList()
{
	if(_TypeSelected >= 0)
	{
		MUItemCategoriesData &typeData = g_ItemCategoriesDatabase._Categories[_TypeSelected];
		UIBase *focused = _SceneManager->GetFocused();

		mu_uint32 maxIndex = typeData.Items.size() > MAX_ITEMADD_SHOWITEM ? MAX_ITEMADD_SHOWITEM : typeData.Items.size();
		for (mu_uint32 n = 0; n < maxIndex; ++n)
		{
			UIRadioButton *btn = _ItemsListBtn[n];

			if (_ItemSelected == _ItemListIndex + n)
			{
				btn->ChangeState(focused == btn ? UIRadioButton::UI_PRESSED_OVER : UIRadioButton::UI_PRESSED);
			}
			else
			{
				btn->ChangeState(focused == btn ? UIRadioButton::UI_OVER : UIRadioButton::UI_NORMAL);
			}

			btn->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 0, 0, 0)));
			btn->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 51, 204, 150)));
			btn->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 51, 204, 110)));
			btn->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 51, 204, 150)));
		}
	}
}

void UIHelperItemAdd::ConfigureItemInfo()
{
	if (_TypeSelected >= 0)
	{
		_TypeIndex = _TypeSelected;
		_ItemIndex = _ItemSelected;

		MUItemCategoriesData &typeData = g_ItemCategoriesDatabase._Categories[_TypeSelected];
		MUItemCategoryItemData &itemInfo = typeData.Items[_ItemSelected];

		SHARED::ITEM_DATA itemData;
		mu_zeromem(&itemData, sizeof(itemData));
		itemData.ItemIndex = itemInfo.ItemIndex;
		itemData.Level = itemInfo.ItemLevel == -1 ? 0 : itemInfo.ItemLevel;
		itemData.Skill = true;
		itemData.Luck = false;
		itemData.ItemOption = false;
		mu_memset(itemData.SocketOption, 0xFF, sizeof(itemData.SocketOption));
		itemData.BonusSocketOption = 0xFF;

		_EnableItemInfo = true;
		ParseItem(itemData, &_ItemInfo);
	}
}

mu_uint32 UIHelperItemAdd::OnPreWindowMessage(const SDL_Event *event, mu_size *result)
{
	UIBase *focusObject = _SceneManager->GetFocused();
	if (focusObject == this ||
		(focusObject != nullptr &&
			focusObject->_Parent == this))
	{
		mu_int32 mouseX = MU_INPUT->GetMouseX();
		mu_int32 mouseY = MU_INPUT->GetMouseY();

		mu_float typePosX = MU_TO_WINDOW(TYPE_BACK_POSX, _AspectWidth);
		mu_float typeWidth = MU_TO_WINDOW(TYPE_BACK_WIDTH, _AspectWidth);
		mu_float itemPosX = MU_TO_WINDOW(ITEM_BACK_POSX, _AspectWidth);
		mu_float itemWidth = MU_TO_WINDOW(ITEM_BACK_WIDTH, _AspectWidth);

		if (mouseY >= _TypeScrollBar->_NativeY &&
			mouseY <= _TypeScrollBar->_NativeY + _TypeScrollBar->_Height &&
			mouseX >= typePosX &&
			mouseX <= typePosX + typeWidth)
		{
			switch (event->type)
			{
			case SDL_MOUSEWHEEL:
				{
					if (_TypeScrollBar->GetState() != UIScrollBar::UI_DISABLED)
					{
						mu_uint32 maxIndex = g_ItemCategoriesDatabase._Categories.size() <= MAX_ITEMADD_SHOWTYPE ? 0 : g_ItemCategoriesDatabase._Categories.size() - MAX_ITEMADD_SHOWTYPE;
						mu_float mouseWheel = event->wheel.y;
						mu_float diffValue = mouseWheel * _TypeScrollBar->_DiffValue * -0.03f;
						_TypeScrollBar->ConfigureScrollInfo(_TypeScrollBar->_MinValue, _TypeScrollBar->_MaxValue, _TypeScrollBar->_CurrentValue + diffValue);

						mu_uint32 newIndex = (mu_uint32)(_TypeScrollBar->GetCurrentValue() * (mu_float)maxIndex);
						if (newIndex != _TypeListIndex)
						{
							_TypeListIndex = newIndex;
							ConfigureTypeList();
						}
					}
				}
				return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
			}
		}
		else if (mouseY >= _ItemScrollBar->_NativeY &&
			mouseY <= _ItemScrollBar->_NativeY + _ItemScrollBar->_Height &&
			mouseX >= itemPosX &&
			mouseX <= itemPosX + itemWidth)
		{
			switch (event->type)
			{
			case SDL_MOUSEWHEEL:
				{
					if (_TypeSelected >= 0 &&
						_ItemScrollBar->GetState() != UIScrollBar::UI_DISABLED)
					{
						MUItemCategoriesData &typeData = g_ItemCategoriesDatabase._Categories[_TypeSelected];
						mu_uint32 maxIndex = typeData.Items.size() <= MAX_ITEMADD_SHOWITEM ? 0 : typeData.Items.size() - MAX_ITEMADD_SHOWITEM;
						mu_float mouseWheel = event->wheel.y;
						mu_float diffValue = mouseWheel * _ItemScrollBar->_DiffValue * -0.03f;
						_ItemScrollBar->ConfigureScrollInfo(_ItemScrollBar->_MinValue, _ItemScrollBar->_MaxValue, _ItemScrollBar->_CurrentValue + diffValue);

						mu_uint32 newIndex = (mu_uint32)(_ItemScrollBar->GetCurrentValue() * (mu_float)maxIndex);
						if (newIndex != _ItemListIndex)
						{
							_ItemListIndex = newIndex;
							ConfigureItemList();
						}
					}
				}
				return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
			}
		}
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

mu_uint32 UIHelperItemAdd::OnWindowMessage(const SDL_Event *event, mu_size *result)
{
	UIBase *selectedUI = _SceneManager->GetSelected();

	if (selectedUI != nullptr)
	{
		if (selectedUI->GetObjectID() >= MESSAGEBOX::MESSAGE_HELPERITEMADD_TYPE_BEGIN &&
			selectedUI->GetObjectID() <= MESSAGEBOX::MESSAGE_HELPERITEMADD_TYPE_END)
		{
			switch (event->type)
			{
			case SDL_KEYDOWN:
				{
					if (MU_INPUT->IsShiftPressing() == false &&
						MU_INPUT->IsCtrlPressing() == false)
					{
						switch (event->key.keysym.scancode)
						{
						case SDL_SCANCODE_UP:
							{
								if (_TypeSelected > 0)
								{
									--_TypeSelected;
									_ItemSelected = -1;

									if (_TypeSelected < _TypeListIndex)
									{
										_TypeListIndex = _TypeSelected;
									}

									if (g_ItemCategoriesDatabase._Categories.size() > MAX_ITEMADD_SHOWTYPE)
									{
										mu_uint32 maxIndex = g_ItemCategoriesDatabase._Categories.size() - MAX_ITEMADD_SHOWTYPE;
										_TypeScrollBar->ConfigureScrollInfo(0.0f, 1.0f, (mu_float)_TypeListIndex / (mu_float)maxIndex);
									}

									CheckTypeList();
									ConfigureTypeList();
									ConfigureItemScroll();
									ConfigureItemList();
								}
							}
							return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;

						case SDL_SCANCODE_DOWN:
							{
								if (_TypeSelected < g_ItemCategoriesDatabase._Categories.size() - 1)
								{
									++_TypeSelected;
									_ItemSelected = -1;

									if (_TypeSelected >= _TypeListIndex + MAX_ITEMADD_SHOWTYPE)
									{
										_TypeListIndex += _TypeSelected - (_TypeListIndex + MAX_ITEMADD_SHOWTYPE - 1);
									}

									if (g_ItemCategoriesDatabase._Categories.size() > MAX_ITEMADD_SHOWTYPE)
									{
										mu_uint32 maxIndex = g_ItemCategoriesDatabase._Categories.size() - MAX_ITEMADD_SHOWTYPE;
										_TypeScrollBar->ConfigureScrollInfo(0.0f, 1.0f, (mu_float)_TypeListIndex / (mu_float)maxIndex);
									}

									CheckTypeList();
									ConfigureTypeList();
									ConfigureItemScroll();
									ConfigureItemList();
								}
							}
							return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
						}
					}
				}
				break;
			}
		}
		else if (selectedUI->GetObjectID() >= MESSAGEBOX::MESSAGE_HELPERITEMADD_ITEM_BEGIN &&
			selectedUI->GetObjectID() <= MESSAGEBOX::MESSAGE_HELPERITEMADD_ITEM_END)
		{
			if (_TypeSelected >= 0)
			{
				MUItemCategoriesData &typeData = g_ItemCategoriesDatabase._Categories[_TypeSelected];
				switch (event->type)
				{
				case SDL_KEYDOWN:
					{
						if (MU_INPUT->IsShiftPressing() == false &&
							MU_INPUT->IsCtrlPressing() == false)
						{
							switch (event->key.keysym.scancode)
							{
							case SDL_SCANCODE_UP:
								{
									if (_ItemSelected > 0)
									{
										--_ItemSelected;

										if (_ItemSelected < _ItemListIndex)
										{
											_ItemListIndex = _ItemSelected;
										}

										if (typeData.Items.size() > MAX_ITEMADD_SHOWITEM)
										{
											mu_uint32 maxIndex = typeData.Items.size() - MAX_ITEMADD_SHOWITEM;
											_ItemScrollBar->ConfigureScrollInfo(0.0f, 1.0f, (mu_float)_ItemListIndex / (mu_float)maxIndex);
										}

										ConfigureItemList();
										ConfigureItemInfo();
									}
								}
								return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;

							case SDL_SCANCODE_DOWN:
								{
									if (_ItemSelected < typeData.Items.size() - 1)
									{
										++_ItemSelected;

										if (_ItemSelected >= _ItemListIndex + MAX_ITEMADD_SHOWITEM)
										{
											_ItemListIndex += _ItemSelected - (_ItemListIndex + MAX_ITEMADD_SHOWITEM - 1);
										}

										if (typeData.Items.size() > MAX_ITEMADD_SHOWITEM)
										{
											mu_uint32 maxIndex = typeData.Items.size() - MAX_ITEMADD_SHOWITEM;
											_ItemScrollBar->ConfigureScrollInfo(0.0f, 1.0f, (mu_float)_ItemListIndex / (mu_float)maxIndex);
										}

										ConfigureItemList();
										ConfigureItemInfo();
									}
								}
								return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
							}
						}
					}
					break;
				}
			}
		}
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

mu_boolean UIHelperItemAdd::OnGFxMessage(mu_uint32 objectid, mu_uint32 message, mu_size message_data)
{
	if (objectid == MESSAGEBOX::MESSAGE_HELPERITEMADD_TYPE_SCROLL)
	{
		switch (message)
		{
		case GFX_MESSAGE_TYPE::GFX_SLIDE:
			{
				mu_uint32 maxIndex = g_ItemCategoriesDatabase._Categories.size() <= MAX_ITEMADD_SHOWTYPE ? 0 : g_ItemCategoriesDatabase._Categories.size() - MAX_ITEMADD_SHOWTYPE;
				mu_uint32 newIndex = (mu_uint32)(_TypeScrollBar->GetCurrentValue() * (mu_float)maxIndex);

				if (newIndex != _TypeListIndex)
				{
					_TypeListIndex = newIndex;
					ConfigureTypeList();
				}
			}
			return true;
		}
	}
	else if (objectid == MESSAGEBOX::MESSAGE_HELPERITEMADD_ITEM_SCROLL)
	{
		switch (message)
		{
		case GFX_MESSAGE_TYPE::GFX_SLIDE:
			{
				MUItemCategoriesData &typeData = g_ItemCategoriesDatabase._Categories[_TypeSelected];
				mu_uint32 maxIndex = typeData.Items.size() <= MAX_ITEMADD_SHOWITEM ? 0 : typeData.Items.size() - MAX_ITEMADD_SHOWITEM;
				mu_uint32 newIndex = (mu_uint32)(_ItemScrollBar->GetCurrentValue() * (mu_float)maxIndex);

				if (newIndex != _ItemListIndex)
				{
					_ItemListIndex = newIndex;
					ConfigureItemList();
				}
			}
			return true;
		}
	}

	return false;
}

void UIHelperItemAdd::Update(mu_uint64 CurrentTime)
{

}

void UIHelperItemAdd::Draw3D()
{
	_SlotCount = 0;

	if (_EnableItemInfo == true)
	{
		MUThreadInfo &ThreadInfo = g_ThreadInfo[0];
		++ThreadInfo.GUIRenderIndex;
		ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);

		_SlotIndex = g_GUIRenderSlotCount;

		mu_float aspectWidth = SCREEN_WIDTH / 640.0f;
		mu_float aspectHeight = SCREEN_HEIGHT / 480.0f;

		ITEM_ATTRIBUTE* itemAttr = &ItemAttribute[_ItemInfo.Type];
		mu_float width = itemAttr->Width * INVENTORY_SQUARE_WIDTH;
		mu_float height = itemAttr->Height * INVENTORY_SQUARE_HEIGHT;
		mu_float x = MU_TO_VIEWPORT(RENDER_POSX, aspectWidth) - width * 0.5f;
		mu_float y = MU_TO_VIEWPORT(RENDER_POSY, aspectHeight) - height * 0.5f;

		RenderItem3D(x, y, width, height, _ItemInfo.Type, _ItemInfo.Level, 0, 0, false);

		_SlotCount = g_GUIRenderSlotCount - _SlotIndex;
	}
}

void UIHelperItemAdd::PostDraw()
{
	DrawFrame();
	DrawText();
	DrawButton();
}

void UIHelperItemAdd::DrawFrame()
{
	TMSGBOX_RENDERBACK;
	TGLAYOUT_RENDERBACK(MainBack);
	TGLAYOUT_RENDERBACK(TypeBack);
	TGLAYOUT_RENDERBACK(ItemBack);

	_RenderDiv->Draw();
	_MainDiv->Draw();

	TGLAYOUT_RENDERFRONT(MainBack);
	TGLAYOUT_RENDERFRONT(TypeBack);
	TGLAYOUT_RENDERFRONT(ItemBack);
	TMSGBOX_RENDERFRONT;

	if(_EnableItemInfo == true)
	{
		MUThreadInfo &ThreadInfo = g_ThreadInfo[0];

		if (_SlotCount > 0)
		{
			ThreadInfo.EnableScissor = false;
			g_Tasking.RenderGUIModelsAtRuntime(_SlotIndex, _SlotCount);
		}

		mu_float aspectWidth = SCREEN_WIDTH / 640.0f;
		mu_float aspectHeight = SCREEN_HEIGHT / 480.0f;
		mu_float x = MU_TO_VIEWPORT(RENDER_NAME_POSX, aspectWidth);
		mu_float y = MU_TO_VIEWPORT(RENDER_NAME_POSY, aspectHeight);

		RenderItemInfo(x, y, &_ItemInfo, false, true, 0, false, false, false);
	}
}

void UIHelperItemAdd::DrawText()
{
	mu_float screenX = _AspectWidth;
	mu_float screenY = _AspectHeight;
	mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255);
	mu_uint32 bgColor = MU::MakeARGB(0, 0, 0, 0);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::HELPERITEMADD_MAIN_TITLE,
		MU_TO_WINDOW(MAIN_TITLE_POSX, screenX),
		MU_TO_WINDOW(MAIN_TITLE_POSY, screenY),
		1.0f,
		ETA_CENTER,
		ETA_TOP,
		0.0f,
		textColor,
		bgColor,
		GlobalText[15100]);

	UIBase *focused = _SceneManager->GetFocused();
	mu_uint32 maxIndex = g_ItemCategoriesDatabase._Categories.size();
	if (maxIndex > MAX_ITEMADD_SHOWTYPE) maxIndex = MAX_ITEMADD_SHOWTYPE;
	auto it = g_ItemCategoriesDatabase._Categories.begin();
	for (mu_uint32 n = _TypeListIndex; n--; ++it);
	for (mu_uint32 n = 0; n < maxIndex; ++n, ++it)
	{
		MUItemCategoriesData &data = *it;

		mu_boolean isSelected = (n + _TypeListIndex) == _TypeSelected || _TypesListBtn[n] == focused;
		mu_uint32 resColor = isSelected == true ? MU::MakeARGB(255, 255, 255, 255) : MU::MakeARGB(200, 200, 200, 255);

		FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERITEMADD_TYPENAME_BEGIN + n,
			MU_TO_WINDOW(TYPE_LIST_POSX + 11.0f, screenX),
			MU_TO_WINDOW(TYPE_LIST_POSY(n) + TYPE_LIST_HEIGHT * 0.5f - 2.0f, screenY),
			1.0f,
			ETA_LEFT,
			ETA_CENTER,
			0.0f,
			resColor,
			bgColor,
			GlobalText[data.CategoryName]);
	}

	if (_TypeSelected >= 0)
	{
		MUItemCategoriesData &typeData = g_ItemCategoriesDatabase._Categories[_TypeSelected];
		maxIndex = typeData.Items.size();
		if (maxIndex > MAX_ITEMADD_SHOWITEM) maxIndex = MAX_ITEMADD_SHOWITEM;
		auto it = typeData.Items.begin();
		for (mu_uint32 n = _ItemListIndex; n--; ++it);
		for (mu_uint32 n = 0; n < maxIndex; ++n, ++it)
		{
			MUItemCategoryItemData &data = *it;

			mu_boolean isSelected = (n + _ItemListIndex) == _ItemSelected || _ItemsListBtn[n] == focused;
			mu_uint32 resColor = isSelected == true ? MU::MakeARGB(255, 255, 255, 255) : MU::MakeARGB(200, 200, 200, 255);

			mu_text nameBuffer[512];
			GetItemName(data.ItemIndex, data.ItemLevel == -1 ? 0 : data.ItemLevel, nameBuffer, mu_countof(nameBuffer));

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERITEMADD_ITEMNAME_BEGIN + n,
				MU_TO_WINDOW(ITEM_LIST_POSX + 11.0f, screenX),
				MU_TO_WINDOW(ITEM_LIST_POSY(n) + ITEM_LIST_HEIGHT * 0.5f - 2.0f, screenY),
				1.0f,
				ETA_LEFT,
				ETA_CENTER,
				0.0f,
				resColor,
				bgColor,
				nameBuffer);
		}
	}

	mu_text textBuffer[32];
	mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("%u"), _MinItemLevel);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::HELPERITEMADD_MINLEVEL,
		MU_TO_WINDOW(LEVEL_POSX, screenX),
		MU_TO_WINDOW(LEVEL_POSY, screenY),
		1.0f,
		ETA_CENTER,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		textBuffer);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERITEMADD_LEVEL_LABEL,
		MU_TO_WINDOW(LEVEL_LABEL_POSX, screenX),
		MU_TO_WINDOW(LEVEL_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[15102]);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERITEMADD_LUCK_LABEL,
		MU_TO_WINDOW(LUCK_LABEL_POSX, screenX),
		MU_TO_WINDOW(LUCK_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[15103]);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERITEMADD_SKILL_LABEL,
		MU_TO_WINDOW(SKILL_LABEL_POSX, screenX),
		MU_TO_WINDOW(SKILL_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[15104]);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERITEMADD_OPTION_LABEL,
		MU_TO_WINDOW(OPTION_LABEL_POSX, screenX),
		MU_TO_WINDOW(OPTION_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[15105]);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERITEMADD_EXC_LABEL,
		MU_TO_WINDOW(EXC_LABEL_POSX, screenX),
		MU_TO_WINDOW(EXC_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[15106]);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERITEMADD_SETITEM_LABEL,
		MU_TO_WINDOW(SETITEM_LABEL_POSX, screenX),
		MU_TO_WINDOW(SETITEM_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[15107]);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERITEMADD_SOCKET_LABEL,
		MU_TO_WINDOW(SOCKET_LABEL_POSX, screenX),
		MU_TO_WINDOW(SOCKET_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[15108]);
}

void UIHelperItemAdd::DrawButton()
{
	mu_uint32 maxIndex = g_ItemCategoriesDatabase._Categories.size();
	if (maxIndex > MAX_ITEMADD_SHOWTYPE) maxIndex = MAX_ITEMADD_SHOWTYPE;
	for (mu_uint32 n = 0; n < maxIndex; ++n)
	{
		_TypesListBtn[n]->Draw();
	}

	if (_TypeSelected >= 0)
	{
		MUItemCategoriesData &typeData = g_ItemCategoriesDatabase._Categories[_TypeSelected];
		maxIndex = typeData.Items.size();
		if (maxIndex > MAX_ITEMADD_SHOWITEM) maxIndex = MAX_ITEMADD_SHOWITEM;
		for (mu_uint32 n = 0; n < maxIndex; ++n)
		{
			_ItemsListBtn[n]->Draw();
		}
	}

	_TypeScrollBar->Draw();
	_ItemScrollBar->Draw();

	_DecLevelBtn->Draw();
	_IncLevelBtn->Draw();

	_LuckCheckBox->Draw();
	_SkillCheckBox->Draw();
	_OptionCheckBox->Draw();
	_ExcellentCheckBox->Draw();
	_SetItemCheckBox->Draw();
	_SocketCheckBox->Draw();

	_AcceptButton->Draw();
	_AcceptButton->PostDraw();
	_CancelButton->Draw();
	_CancelButton->PostDraw();
}

mu_uint32 UIHelperItemAdd::OnPreKeyPressed(const SDL_Keysym &key)
{
	switch (key.scancode)
	{
	case SDL_SCANCODE_RETURN:
	case SDL_SCANCODE_KP_ENTER:
		{
			_SceneManager->OnGFxMessagePush(_AcceptButton->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
		}
		return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;

	case SDL_SCANCODE_ESCAPE:
		{
			_SceneManager->OnGFxMessagePush(_CancelButton->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
		}
		return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

UIBase *UIHelperItemAdd::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _TypeScrollBar->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _ItemScrollBar->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		mu_uint32 maxIndex = g_ItemCategoriesDatabase._Categories.size();
		if (maxIndex > MAX_ITEMADD_SHOWTYPE) maxIndex = MAX_ITEMADD_SHOWTYPE;
		for (mu_uint32 n = 0; n < maxIndex; ++n)
		{
			object = _TypesListBtn[n]->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}
		}

		if (_TypeSelected >= 0)
		{
			MUItemCategoriesData &typeData = g_ItemCategoriesDatabase._Categories[_TypeSelected];
			maxIndex = typeData.Items.size();
			if (maxIndex > MAX_ITEMADD_SHOWITEM) maxIndex = MAX_ITEMADD_SHOWITEM;
			for (mu_uint32 n = 0; n < maxIndex; ++n)
			{
				object = _ItemsListBtn[n]->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}
			}
		}

		object = _DecLevelBtn->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _IncLevelBtn->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _LuckCheckBox->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _SkillCheckBox->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _OptionCheckBox->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _ExcellentCheckBox->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _SetItemCheckBox->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _SocketCheckBox->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _AcceptButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _CancelButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return focus;
}

void UIHelperItemAdd::Configure(mu_int32 helperIndex, HELPER_ITEMSETTING *itemSetting)
{
	_HelperIndex = helperIndex;
	_TypeIndex = -1;
	_ItemIndex = -1;

	if(_HelperIndex >= 0)
	{
		mu_boolean isFound = false;
		mu_uint32 categoryIndex = 0, itemIndex;
		mu_uint32 categoriesCount = g_ItemCategoriesDatabase._Categories.size();
		for (auto cit = g_ItemCategoriesDatabase._Categories.begin(); categoryIndex < categoriesCount; ++categoryIndex, ++cit)
		{
			MUItemCategoriesData &typeData = *cit;

			itemIndex = 0;
			mu_uint32 itemsCount = typeData.Items.size();
			for (auto iit = typeData.Items.begin(); itemIndex < itemsCount; ++itemIndex, ++iit)
			{
				MUItemCategoryItemData &itemData = *iit;

				if (itemData.ItemIndex == itemSetting->ItemIndex &&
					itemData.ItemLevel == itemSetting->ItemLevel)
				{
					isFound = true;
					break;
				}
			}

			if (isFound == true)
			{
				break;
			}
		}

		if (isFound == true)
		{
			_TypeSelected = categoryIndex;
			_ItemSelected = itemIndex;

			_MinItemLevel = itemSetting->MinLevel;
			_LuckCheckBox->ChangeState(itemSetting->IsLuck == true ? UIToggleButton::UI_PRESSED : UIToggleButton::UI_NORMAL);
			_SkillCheckBox->ChangeState(itemSetting->IsSkill == true ? UIToggleButton::UI_PRESSED : UIToggleButton::UI_NORMAL);
			_OptionCheckBox->ChangeState(itemSetting->IsOption == true ? UIToggleButton::UI_PRESSED : UIToggleButton::UI_NORMAL);
			_ExcellentCheckBox->ChangeState(itemSetting->IsExcellent == true ? UIToggleButton::UI_PRESSED : UIToggleButton::UI_NORMAL);
			_SetItemCheckBox->ChangeState(itemSetting->IsSetItem == true ? UIToggleButton::UI_PRESSED : UIToggleButton::UI_NORMAL);
			_SocketCheckBox->ChangeState(itemSetting->IsSocket == true ? UIToggleButton::UI_PRESSED : UIToggleButton::UI_NORMAL);

			ConfigureTypeList();
			ConfigureItemScroll();
			ConfigureItemList();
			ConfigureItemInfo();
		}
		else
		{
			goto cleanAll;
		}
	}
	else
	{
cleanAll:
		_TypeListIndex = 0;
		_ItemListIndex = 0;
		_TypeSelected = -1;
		_ItemSelected = -1;

		_EnableItemInfo = false;

		_MinItemLevel = 0;
		_LuckCheckBox->ChangeState(UIToggleButton::UI_NORMAL);
		_SkillCheckBox->ChangeState(UIToggleButton::UI_NORMAL);
		_OptionCheckBox->ChangeState(UIToggleButton::UI_NORMAL);
		_ExcellentCheckBox->ChangeState(UIToggleButton::UI_NORMAL);
		_SetItemCheckBox->ChangeState(UIToggleButton::UI_NORMAL);
		_SocketCheckBox->ChangeState(UIToggleButton::UI_NORMAL);

		ConfigureTypeList();
		ConfigureItemScroll();
		ConfigureItemList();
		ConfigureItemInfo();
	}
}