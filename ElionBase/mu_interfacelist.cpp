#include "stdafx.h"
#include "mu_interfacelist.h"

mu_atomic_uint32_t g_RandomTextIndex(0);

namespace INTERFACES
{
	const mu_uint8 Type = MULoadingType::LOADING_TEXTURE;
	const mu_uint32 Counter = LOADING_COUNTER::CT_INTERFACE;
	MUTexture Data[MAX];

	MULoadingData Load[] =
	{
		// General usage
		DEFINE_LOAD(CURSOR_NORMAL, _T("./data/interface/cursor.ttex"))
		DEFINE_LOAD(CURSOR_PUSH, _T("./data/interface/cursorpush.ttex"))
		DEFINE_LOAD(CURSOR_ATTACK, _T("./data/interface/cursorattack.ttex"))
		DEFINE_LOAD(CURSOR_ATTACK2, _T("./data/interface/cursorattack2.ttex"))
		DEFINE_LOAD(CURSOR_GET, _T("./data/interface/cursorget.ttex"))
		DEFINE_LOAD(CURSOR_TALK, _T("./data/interface/cursortalk.ttex"))
		DEFINE_LOAD(CURSOR_REPAIR, _T("./data/interface/cursorrepair.ttex"))
		DEFINE_LOAD(CURSOR_LEANAGAINST, _T("./data/interface/cursorleanagainst.ttex"))
		DEFINE_LOAD(CURSOR_SITDOWN, _T("./data/interface/cursorsitdown.ttex"))
		DEFINE_LOAD(CURSOR_DONTMOVE, _T("./data/interface/cursordontmove.ttex"))
		DEFINE_LOAD(CURSOR_IDSELECT, _T("./data/interface/cursorid.ttex"))

		DEFINE_LOAD(MENU_BACK, _T("./data/interface/op1_stone.jtex"))
		DEFINE_LOAD(MENU_TOP, _T("./data/interface/op1_back1.ttex"))
		DEFINE_LOAD(MENU_BOTTOM, _T("./data/interface/op1_back2.ttex"))
		DEFINE_LOAD(MENU_LEFT, _T("./data/interface/op1_back3.jtex"))
		DEFINE_LOAD(MENU_RIGHT, _T("./data/interface/op1_back4.jtex"))
		DEFINE_LOAD(MENU_BUTTON, _T("./data/interface/op1_b_all.ttex"))

		DEFINE_LOAD(MSGBOX_WINDOW, _T("./data/interface/message_back.ttex"))
		DEFINE_LOAD(MSGBOX_TOP, _T("./data/interface/newui_msgbox_top.ttex"))
		DEFINE_LOAD(MSGBOX_MIDDLE, _T("./data/interface/newui_msgbox_middle.ttex"))
		DEFINE_LOAD(MSGBOX_BOTTOM, _T("./data/interface/newui_msgbox_bottom.ttex"))
		DEFINE_LOAD(MSGBOX_BACK, _T("./data/interface/newui_msgbox_back.jtex"))
		DEFINE_LOAD(MSGBOX_LINE, _T("./data/interface/newui_message_line.ttex"))
		DEFINE_LOAD(MSGBOX_TITLEBAR, _T("./data/interface/newui_message_03.ttex"))
		DEFINE_LOAD(MSGBOX_SEPARATE_LINE, _T("./data/interface/newui_separate_line.jtex"))
		DEFINE_LOAD(MSGBOX_OK, _T("./data/interface/newui_button_ok.ttex"))
		DEFINE_LOAD(MSGBOX_CANCEL, _T("./data/interface/newui_button_cancel.ttex"))
		DEFINE_LOAD(MSGBOX_CLOSE, _T("./data/interface/newui_button_close.ttex"))
		DEFINE_LOAD(MSGBOX_PROGRESSBAR_BACK, _T("./data/interface/newui_bar_switch01.jtex"))
		DEFINE_LOAD(MSGBOX_PROGRESSBAR_FRONT, _T("./data/interface/newui_bar_switch02.jtex"))
		DEFINE_LOAD(MSGBOX_DUELWINDOW_BACK, _T("./data/interface/newui_duelwindow.ttex"))

		DEFINE_LOAD(WINDOWFRAME_TOP, _T("./data/interface/newui_item_back01.ttex"))
		DEFINE_LOAD(WINDOWFRAME_TOP_EXT, _T("./data/interface/newui_item_back04.ttex"))
		DEFINE_LOAD(WINDOWFRAME_LEFT, _T("./data/interface/newui_item_back02-l.ttex"))
		DEFINE_LOAD(WINDOWFRAME_RIGHT, _T("./data/interface/newui_item_back02-r.ttex"))
		DEFINE_LOAD(WINDOWFRAME_BOTTOM, _T("./data/interface/newui_item_back03.ttex"))
		
		DEFINE_LOAD(WINDOW_TOP, _T("./data/interface/newui_commamd01.ttex"))
		DEFINE_LOAD(WINDOW_TOP_EXT, _T("./data/interface/newui_commamd04.ttex"))
		DEFINE_LOAD(WINDOW_MIDDLE, _T("./data/interface/newui_commamd02.ttex"))
		DEFINE_LOAD(WINDOW_BOTTOM, _T("./data/interface/newui_commamd03.ttex"))
		DEFINE_LOAD(WINDOW_LINE, _T("./data/interface/newui_commamd_line.jtex"))
		DEFINE_LOAD(WINDOW_ARROW_LEFT, _T("./data/interface/newui_arrow(l).ttex"))
		DEFINE_LOAD(WINDOW_ARROW_RIGHT, _T("./data/interface/newui_arrow(r).ttex"))

		DEFINE_LOAD(TABLE_TOP_LEFT, _T("./data/interface/newui_item_table01(l).ttex"))
		DEFINE_LOAD(TABLE_TOP_RIGHT, _T("./data/interface/newui_item_table01(r).ttex"))
		DEFINE_LOAD(TABLE_BOTTOM_LEFT, _T("./data/interface/newui_item_table02(l).ttex"))
		DEFINE_LOAD(TABLE_BOTTOM_RIGHT, _T("./data/interface/newui_item_table02(r).ttex"))
		DEFINE_LOAD(TABLE_TOP_LINE, _T("./data/interface/newui_item_table03(up).ttex"))
		DEFINE_LOAD(TABLE_BOTTOM_LINE, _T("./data/interface/newui_item_table03(dw).ttex"))
		DEFINE_LOAD(TABLE_LEFT_LINE, _T("./data/interface/newui_item_table03(l).ttex"))
		DEFINE_LOAD(TABLE_RIGHT_LINE, _T("./data/interface/newui_item_table03(r).ttex"))

		// Game Shop UI
		DEFINE_LOAD(INGAMESHOP_PAGE_LEFT, _T("./data/interface/ingameshop/ingame_bt_page_l.ttex"))
		DEFINE_LOAD(INGAMESHOP_PAGE_RIGHT, _T("./data/interface/ingameshop/ingame_bt_page_r.ttex"))
		DEFINE_LOAD(INGAMESHOP_PAGE_LEFT2, _T("./data/interface/ingameshop/igs_storage_page_left.ttex"))
		DEFINE_LOAD(INGAMESHOP_PAGE_RIGHT2, _T("./data/interface/ingameshop/igs_storage_page_right.ttex"))

		// Lines
		DEFINE_LOAD(ITEM_BACK01, _T("./data/interface/item_back01.jtex"))
		DEFINE_LOAD(INVENTORY_BOX02, _T("./data/interface/inventorybox2.jtex"))
		DEFINE_LOAD(TRADING_LINE, _T("./data/interface/trading_line.jtex"))

		DEFINE_LOAD(EXIT_BUTTON, _T("./data/interface/newui_exit_00.ttex"))
		DEFINE_LOAD(EMPTY_BUTTON, _T("./data/interface/newui_btn_empty.ttex"))
		DEFINE_LOAD(EMPTY_BUTTON_BIG, _T("./data/interface/newui_btn_empty_big.ttex"))
		DEFINE_LOAD(EMPTY_BUTTON_SMALL, _T("./data/interface/newui_btn_empty_small.ttex"))
		DEFINE_LOAD(EMPTY_BUTTON_VERYSMALL, _T("./data/interface/newui_btn_empty_very_small.ttex"))

		DEFINE_LOAD(EMPTYNS_BUTTON, _T("./data/interface/newui_btn_empty.jtex"))
		DEFINE_LOAD(EMPTYNS_BUTTON_BIG, _T("./data/interface/newui_btn_empty_big.jtex"))

		DEFINE_LOAD(EMPTY_ROUND, _T("./data/interface/newui_btn_round.ttex"))

		DEFINE_LOAD(MACROUI_INPUTNUMBER, _T("./data/interface/macroui/macroui_inputnumber.ttex"))
		DEFINE_LOAD(MACROUI_INPUTSTRING, _T("./data/interface/macroui/macroui_inputstring.ttex"))
		DEFINE_LOAD(MACROUI_OPTIONBUTTON, _T("./data/interface/macroui/macroui_optionbutton.ttex"))
		DEFINE_LOAD(MACROUI_RANGEMINUS, _T("./data/interface/macroui/macroui_rangeminus.ttex"))
		DEFINE_LOAD(MACROUI_SETUP, _T("./data/interface/macroui/macroui_setup.ttex"))
		DEFINE_LOAD(MACROUI_START, _T("./data/interface/macroui/macroui_start.ttex"))
		DEFINE_LOAD(MACROUI_STOP, _T("./data/interface/macroui/macroui_stop.ttex"))

		DEFINE_LOAD(SCROLLBAR_TOP, _T("./data/interface/newui_scrollbar_up.ttex"))
		DEFINE_LOAD(SCROLLBAR_MIDDLE, _T("./data/interface/newui_scrollbar_m.ttex"))
		DEFINE_LOAD(SCROLLBAR_BOTTOM, _T("./data/interface/newui_scrollbar_down.ttex"))
		DEFINE_LOAD(SCROLLBAR_SLIDE_ON, _T("./data/interface/newui_scroll_on.ttex"))
		DEFINE_LOAD(SCROLLBAR_SLIDE_OFF, _T("./data/interface/newui_scroll_off.ttex"))
		DEFINE_LOAD(SCROLLBAR_STRETCH, _T("./data/interface/newui_scrollbar_stretch.jtex"))

		DEFINE_LOAD(SCROLL_BTN_UP, _T("./data/interface/newui_bt_scroll_up.jtex"))
		DEFINE_LOAD(SCROLL_BTN_DOWN, _T("./data/interface/newui_bt_scroll_dn.jtex"))

		DEFINE_LOAD(MYQUEST_LINE, _T("./data/interface/newui_myquest_line.ttex"))

		DEFINE_LOAD(GUILD_BTN_TAB1, _T("./data/interface/newui_guild_tab01.ttex"))
		DEFINE_LOAD(GUILD_BTN_TAB2, _T("./data/interface/newui_guild_tab02.ttex"))
		DEFINE_LOAD(GUILD_BTN_TAB3, _T("./data/interface/newui_guild_tab03.ttex"))
		DEFINE_LOAD(GUILD_BTN_TAB4, _T("./data/interface/newui_guild_tab04.ttex"))
		DEFINE_LOAD(GUILD_MAKE_EDITBOX, _T("./data/interface/newui_guildmakeeditbox.ttex"))
		
		DEFINE_LOAD(MIX_BUTTON, _T("./data/interface/newui_bt_mix.ttex"))
		DEFINE_LOAD(REPAIR_BUTTON, _T("./data/interface/newui_repair_00.ttex"))

		DEFINE_LOAD(PSHOP_OPEN_BUTTON, _T("./data/interface/newui_bt_openshop.ttex"))
		DEFINE_LOAD(PSHOP_CLOSE_BUTTON, _T("./data/interface/newui_bt_closeshop.ttex"))
		DEFINE_LOAD(PSHOP_EDITBOX_TITLE, _T("./data/interface/newui_box_opentitle.ttex"))

		DEFINE_LOAD(ICON_WEAPON_RIGHT, _T("./data/interface/newui_item_weapon(r).ttex"))
		DEFINE_LOAD(ICON_WEAPON_LEFT, _T("./data/interface/newui_item_weapon(l).ttex"))
		DEFINE_LOAD(ICON_HELM, _T("./data/interface/newui_item_cap.ttex"))
		DEFINE_LOAD(ICON_ARMOR, _T("./data/interface/newui_item_upper.ttex"))
		DEFINE_LOAD(ICON_PANTS, _T("./data/interface/newui_item_lower.ttex"))
		DEFINE_LOAD(ICON_GLOVES, _T("./data/interface/newui_item_gloves.ttex"))
		DEFINE_LOAD(ICON_BOOTS, _T("./data/interface/newui_item_boots.ttex"))
		DEFINE_LOAD(ICON_WING, _T("./data/interface/newui_item_wing.ttex"))
		DEFINE_LOAD(ICON_HELPER, _T("./data/interface/newui_item_fairy.ttex"))
		DEFINE_LOAD(ICON_NECKLACE, _T("./data/interface/newui_item_necklace.ttex"))
		DEFINE_LOAD(ICON_RING, _T("./data/interface/newui_item_ring.ttex"))

		DEFINE_LOAD(ICON_MONEY1, _T("./data/interface/newui_item_money.ttex"))
		DEFINE_LOAD(ICON_MONEY2, _T("./data/interface/newui_item_money2.ttex"))
		DEFINE_LOAD(ICON_MONEY3, _T("./data/interface/newui_item_money3.ttex"))
		
		DEFINE_LOAD(QUEST_BUTTON_LEFT, _T("./data/interface/quest_bt_l.ttex"))
		DEFINE_LOAD(QUEST_BUTTON_RIGHT, _T("./data/interface/quest_bt_r.ttex"))

		// Login interface
		DEFINE_LOAD(LOGIN_LOGO, _T("./data/logo/elion.ttex"))
		DEFINE_LOAD(LOGIN_SERVERLIST_BACKGROUND, _T("./data/interface/cha_bt.ttex"))
		DEFINE_LOAD(LOGIN_SERVER_BACKGROUND, _T("./data/interface/server_b2_all.ttex"))
		DEFINE_LOAD(LOGIN_SERVER_LOADBAR, _T("./data/interface/server_b2_loding.jtex"))
		DEFINE_LOAD(LOGIN_DECORATION_ALL, _T("./data/interface/server_deco_all.ttex"))
		DEFINE_LOAD(LOGIN_MENU_BUTTON, _T("./data/interface/server_menu_b_all.ttex"))
		DEFINE_LOAD(LOGIN_CREDIT_BUTTON, _T("./data/interface/server_credit_b_all.ttex"))
		DEFINE_LOAD(LOGIN_DECORATION, _T("./data/interface/deco.ttex"))
		DEFINE_LOAD(LOGIN_BACKGROUND, _T("./data/interface/login_back.ttex"))
		DEFINE_LOAD(LOGIN_TEXT_BACKGROUND, _T("./data/interface/login_me.ttex"))
		DEFINE_LOAD(LOGIN_SERVER_EX01, _T("./data/interface/server_ex01.ttex"))
		DEFINE_LOAD(LOGIN_SERVER_EX02, _T("./data/interface/server_ex02.jtex"))
		DEFINE_LOAD(LOGIN_SERVER_EX03, _T("./data/interface/server_ex03.ttex"))

		// Character interface
		DEFINE_LOAD(CHARACTER_CREATE_BUTTON, _T("./data/interface/b_create.ttex"))
		DEFINE_LOAD(CHARACTER_CONNECT_BUTTON, _T("./data/interface/b_connect.ttex"))
		DEFINE_LOAD(CHARACTER_DELETE_BUTTON, _T("./data/interface/b_delete.ttex"))
		DEFINE_LOAD(CHARACTER_CREATE_NAMEINPUT, _T("./data/interface/cha_id.ttex"))
		DEFINE_LOAD(CHARACTER_BALLOON, _T("./data/interface/character_ex.ttex"))

		// Mainframe interface
		DEFINE_LOAD(MAINFRAME_MENU_1, _T("./data/interface/newui_menu01.jtex"))
		DEFINE_LOAD(MAINFRAME_MENU_2, _T("./data/interface/newui_menu02.jtex"))
		DEFINE_LOAD(MAINFRAME_MENU_2_1, _T("./data/interface/newui_menu02-03.jtex"))
		DEFINE_LOAD(MAINFRAME_MENU_3, _T("./data/interface/partcharge1/newui_menu03.jtex"))
		DEFINE_LOAD(MAINFRAME_GAUGE_BLUE, _T("./data/interface/newui_menu_blue.jtex"))
		DEFINE_LOAD(MAINFRAME_GAUGE_GREEN, _T("./data/interface/newui_menu_green.jtex"))
		DEFINE_LOAD(MAINFRAME_GAUGE_RED, _T("./data/interface/newui_menu_red.jtex"))
		DEFINE_LOAD(MAINFRAME_GAUGE_AG, _T("./data/interface/newui_menu_ag.jtex"))
		DEFINE_LOAD(MAINFRAME_GAUGE_SD, _T("./data/interface/newui_menu_sd.jtex"))
		DEFINE_LOAD(MAINFRAME_GAUGE_EXBAR, _T("./data/interface/newui_exbar.jtex"))
		DEFINE_LOAD(MAINFRAME_MASTER_GAUGE_BAR, _T("./data/interface/exbar_master.jtex"))
		DEFINE_LOAD(MAINFRAME_MENU_BTN_CHARINFO, _T("./data/interface/partcharge1/newui_menu_bt01.jtex"))
		DEFINE_LOAD(MAINFRAME_MENU_BTN_INVENTORY, _T("./data/interface/partcharge1/newui_menu_bt02.jtex"))
		DEFINE_LOAD(MAINFRAME_MENU_BTN_FRIEND, _T("./data/interface/partcharge1/newui_menu_bt03.jtex"))
		DEFINE_LOAD(MAINFRAME_MENU_BTN_OPTION, _T("./data/interface/partcharge1/newui_menu_bt04.jtex"))
		DEFINE_LOAD(MAINFRAME_MENU_BTN_CASHSHOP, _T("./data/interface/partcharge1/newui_menu_bt05.jtex"))
		DEFINE_LOAD(MAINFRAME_SKILL1, _T("./data/interface/newui_skill.jtex"))
		DEFINE_LOAD(MAINFRAME_NON_SKILL1, _T("./data/interface/newui_non_skill.jtex"))
		DEFINE_LOAD(MAINFRAME_SKILL2, _T("./data/interface/newui_skill2.jtex"))
		DEFINE_LOAD(MAINFRAME_NON_SKILL2, _T("./data/interface/newui_non_skill2.jtex"))
		DEFINE_LOAD(MAINFRAME_COMMAND, _T("./data/interface/newui_command.jtex"))
		DEFINE_LOAD(MAINFRAME_NON_COMMAND, _T("./data/interface/newui_non_command.jtex"))
		DEFINE_LOAD(MAINFRAME_SKILLBOX1, _T("./data/interface/newui_skillbox.jtex"))
		DEFINE_LOAD(MAINFRAME_SKILLBOX2, _T("./data/interface/newui_skillbox2.jtex"))

		// Options interface
		DEFINE_LOAD(OPTION_TOP, _T("./data/interface/newui_option_top.ttex"))
		DEFINE_LOAD(OPTION_LEFT, _T("./data/interface/newui_option_back06(l).ttex"))
		DEFINE_LOAD(OPTION_RIGHT, _T("./data/interface/newui_option_back06(r).ttex"))
		DEFINE_LOAD(OPTION_LINE, _T("./data/interface/newui_option_line.jtex"))
		DEFINE_LOAD(OPTION_POINT, _T("./data/interface/newui_option_point.ttex"))
		DEFINE_LOAD(OPTION_CHECK, _T("./data/interface/newui_option_check.ttex"))
		DEFINE_LOAD(OPTION_EFFECT_BACK, _T("./data/interface/newui_option_effect03.ttex"))
		DEFINE_LOAD(OPTION_EFFECT_COLOR, _T("./data/interface/newui_option_effect04.ttex"))
		DEFINE_LOAD(OPTION_VOLUME_BACK, _T("./data/interface/newui_option_volume01.ttex"))
		DEFINE_LOAD(OPTION_VOLUME_COLOR, _T("./data/interface/newui_option_volume02.ttex"))

		// Chat interface
		DEFINE_LOAD(CHATFRAME_INPUT_BACK, _T("./data/interface/newui_chat_back.jtex"))
		DEFINE_LOAD(CHATFRAME_INPUT_NORMAL_ON, _T("./data/interface/newui_chat_normal_on.jtex"))
		DEFINE_LOAD(CHATFRAME_INPUT_PARTY_ON, _T("./data/interface/newui_chat_party_on.jtex"))
		DEFINE_LOAD(CHATFRAME_INPUT_GUILD_ON, _T("./data/interface/newui_chat_guild_on.jtex"))
		DEFINE_LOAD(CHATFRAME_INPUT_WHISPER_ON, _T("./data/interface/newui_chat_whisper_on.jtex"))
		DEFINE_LOAD(CHATFRAME_INPUT_SYSTEM_ON, _T("./data/interface/newui_chat_system_on.jtex"))
		DEFINE_LOAD(CHATFRAME_INPUT_LOG_ON, _T("./data/interface/newui_chat_chat_on.jtex"))
		DEFINE_LOAD(CHATFRAME_INPUT_FRAME_ON, _T("./data/interface/newui_chat_frame_on.jtex"))
		DEFINE_LOAD(CHATFRAME_INPUT_BTN_SIZE, _T("./data/interface/newui_chat_btn_size.jtex"))
		DEFINE_LOAD(CHATFRAME_INPUT_BTN_ALPHA, _T("./data/interface/newui_chat_btn_alpha.jtex"))

		// Character interface
		DEFINE_LOAD(CHARFRAME_TEXTBOX, _T("./data/interface/newui_cha_textbox02.ttex"))
		DEFINE_LOAD(CHARFRAME_BTN_STAT, _T("./data/interface/newui_chainfo_btn_level.ttex"))
		DEFINE_LOAD(CHARFRAME_BTN_QUEST, _T("./data/interface/newui_chainfo_btn_quest.ttex"))
		DEFINE_LOAD(CHARFRAME_BTN_PET, _T("./data/interface/newui_chainfo_btn_pet.ttex"))
		DEFINE_LOAD(CHARFRAME_BTN_MASTER, _T("./data/interface/newui_chainfo_btn_master.ttex"))

		// Inventory interface
		DEFINE_LOAD(INVENFRAME_ITEM_BOX, _T("./data/interface/newui_item_box.ttex"))
		DEFINE_LOAD(INVENFRAME_USEBOX01, _T("./data/interface/newui_inven_usebox_01.ttex"))
		DEFINE_LOAD(INVENFRAME_USEBOX02, _T("./data/interface/newui_inven_usebox_02.ttex"))
		DEFINE_LOAD(INVENFRAME_USEBOX03, _T("./data/interface/newui_inven_usebox_03.ttex"))

		// Storage interface
		DEFINE_LOAD(STORAGE_BTN_DEPOSIT_MONEY, _T("./data/interface/newui_bt_money01.ttex"))
		DEFINE_LOAD(STORAGE_BTN_WITHDRAW_MONEY, _T("./data/interface/newui_bt_money02.ttex"))
		DEFINE_LOAD(STORAGE_BTN_UNLOCK, _T("./data/interface/newui_bt_lock02.ttex"))
		DEFINE_LOAD(STORAGE_BTN_LOCK, _T("./data/interface/newui_bt_lock.ttex"))

		// Trade interface
		DEFINE_LOAD(TRADE_ACCOUNT_TITLE, _T("./data/interface/newui_account_title.ttex"))
		DEFINE_LOAD(TRADE_BTN_ACCEPT, _T("./data/interface/newui_bt_accept.ttex"))

		// Party interface
		DEFINE_LOAD(PARTY_LIST_BACK, _T("./data/interface/newui_party_back.ttex"))
		DEFINE_LOAD(PARTY_HPBAR_BACK, _T("./data/interface/newui_party_lifebar01.jtex"))
		DEFINE_LOAD(PARTY_HPBAR_COLOR, _T("./data/interface/newui_party_lifebar02.jtex"))
		DEFINE_LOAD(PARTYLIST_HPBAR, _T("./data/interface/newui_party_hpbar.jtex"))
		DEFINE_LOAD(PARTY_FLAG, _T("./data/interface/newui_party_flag.ttex"))
		DEFINE_LOAD(PARTY_EXIT, _T("./data/interface/newui_party_x.ttex"))

		// Command interface
		DEFINE_LOAD(COMMANDFRAME_CURSORID_WND, _T("./data/interface/newui_cursorid_wnd.jtex"))

		// Pet interface
		DEFINE_LOAD(PET_LIFEBAR_BACK, _T("./data/interface/newui_pet_lifebar01.jtex"))
		DEFINE_LOAD(PET_LIFEBAR_COLOR, _T("./data/interface/newui_pet_lifebar02.jtex"))

		// Buff interface
		DEFINE_LOAD(BUFFWINDOW_STATUSICON1, _T("./data/interface/newui_statusicon.jtex"))
		DEFINE_LOAD(BUFFWINDOW_STATUSICON2, _T("./data/interface/newui_statusicon2.jtex"))

		// Catapult interface
		DEFINE_LOAD(CATAPULTFRAME_BTN_SMALL, _T("./data/interface/newui_btn_gate.ttex"))
		DEFINE_LOAD(CATAPULTFRAME_BTN_BIG, _T("./data/interface/newui_btn_round.ttex"))

		// Master interface
		DEFINE_LOAD(MASTERFRAME_BACK, _T("./data/interface/master_back.jtex"))
		DEFINE_LOAD(MASTERFRAME_SKILLBOX, _T("./data/interface/master_skillbox.ttex"))
		DEFINE_LOAD(MASTERFRAME_ARROW_DOWN, _T("./data/interface/master_arrow(d).ttex"))
		DEFINE_LOAD(MASTERFRAME_ARROW_RIGHT, _T("./data/interface/master_arrow(r).ttex"))

		// Duel Watch interface
		DEFINE_LOAD(DUELWATCH_MAINFRAME_BACK1, _T("./data/interface/menu_pk_01.jtex"))
		DEFINE_LOAD(DUELWATCH_MAINFRAME_BACK2, _T("./data/interface/menu_pk_02.jtex"))
		DEFINE_LOAD(DUELWATCH_MAINFRAME_BACK3, _T("./data/interface/menu_pk_03.jtex"))
		DEFINE_LOAD(DUELWATCH_MAINFRAME_SCORE, _T("./data/interface/menu_pk_bt02.ttex"))
		DEFINE_LOAD(DUELWATCH_MAINFRAME_HP_GAUGE, _T("./data/interface/menu_pk_hp03(bar2).jtex"))
		DEFINE_LOAD(DUELWATCH_MAINFRAME_SD_GAUGE, _T("./data/interface/menu_pk_sd03(bar2).jtex"))
		DEFINE_LOAD(DUELWATCH_MAINFRAME_HP_GAUGE_FX, _T("./data/interface/menu_pk_hp06(bar).jtex"))
		DEFINE_LOAD(DUELWATCH_MAINFRAME_SD_GAUGE_FX, _T("./data/interface/menu_pk_sd05(bar).jtex"))
		DEFINE_LOAD(DUELWATCH_USERLIST_BOX, _T("./data/interface/pk_box.ttex"))

		// Gens interface
		DEFINE_LOAD(GENS_RANKBACK, _T("./data/interface/newui_gens_rankback.ttex"))
		DEFINE_LOAD(GENS_POINT, _T("./data/interface/gens_point.ttex"))

		// Hero position interface
		DEFINE_LOAD(HEROPOSITION_POSITIONA, _T("./data/interface/minimap_positiona.ttex"))
		DEFINE_LOAD(HEROPOSITION_POSITIONB, _T("./data/interface/minimap_positionb.ttex"))
		DEFINE_LOAD(HEROPOSITION_POSITIONC, _T("./data/interface/macroui/minimap_positionc.ttex"))

		// Endurance interface
		DEFINE_LOAD(ENDURANCE_PET_BACK, _T("./data/interface/newui_pet_back.ttex"))
		DEFINE_LOAD(ENDURANCE_PET_HPBAR, _T("./data/interface/newui_pet_hpbar.jtex"))
		DEFINE_LOAD(ENDURANCE_BOOTS, _T("./data/interface/newui_durable_boots.ttex"))
		DEFINE_LOAD(ENDURANCE_CAP, _T("./data/interface/newui_durable_cap.ttex"))
		DEFINE_LOAD(ENDURANCE_GLOVES, _T("./data/interface/newui_durable_gloves.ttex"))
		DEFINE_LOAD(ENDURANCE_PANTS, _T("./data/interface/newui_durable_lower.ttex"))
		DEFINE_LOAD(ENDURANCE_NECKLACE, _T("./data/interface/newui_durable_necklace.ttex"))
		DEFINE_LOAD(ENDURANCE_RING, _T("./data/interface/newui_durable_ring.ttex"))
		DEFINE_LOAD(ENDURANCE_SHIELD, _T("./data/interface/newui_durable_shield.ttex"))
		DEFINE_LOAD(ENDURANCE_UPPER, _T("./data/interface/newui_durable_upper.ttex"))
		DEFINE_LOAD(ENDURANCE_WEAPON, _T("./data/interface/newui_durable_weapon.ttex"))
		DEFINE_LOAD(ENDURANCE_WING, _T("./data/interface/newui_durable_wing.ttex"))
		DEFINE_LOAD(ENDURANCE_PCROOM, _T("./data/interface/newui_pcroom.ttex"))

		// Minimap interface
		DEFINE_LOAD(MINIMAP_CORNER, _T("./data/interface/mini_map_ui_corner.ttex"))
		DEFINE_LOAD(MINIMAP_LINE, _T("./data/interface/mini_map_ui_line.jtex"))
		DEFINE_LOAD(MINIMAP_CHARACTER, _T("./data/interface/mini_map_ui_cha.ttex"))
		DEFINE_LOAD(MINIMAP_PORTAL, _T("./data/interface/mini_map_ui_portal.ttex"))
		DEFINE_LOAD(MINIMAP_NPC, _T("./data/interface/mini_map_ui_npc.ttex"))
		DEFINE_LOAD(MINIMAP_CANCEL, _T("./data/interface/mini_map_ui_cancel.ttex"))

		// My Quest interface
		DEFINE_LOAD(MYQUEST_BTN_OPEN, _T("./data/interface/quest_bt_open.ttex"))
		DEFINE_LOAD(MYQUEST_BTN_GIVEUP, _T("./data/interface/quest_bt_cast.ttex"))
		DEFINE_LOAD(MYQUEST_TAB1, _T("./data/interface/quest_tab01.ttex"))
		DEFINE_LOAD(MYQUEST_TAB2, _T("./data/interface/quest_tab02.ttex"))
		DEFINE_LOAD(MYQUEST_TAB3, _T("./data/interface/quest_tab03.ttex"))

		// Credits interface
		DEFINE_LOAD(CREDITS_LEFT1, _T("./data/interface/im1_1.jtex"))
		DEFINE_LOAD(CREDITS_RIGHT1, _T("./data/interface/im1_2.jtex"))
		DEFINE_LOAD(CREDITS_LEFT2, _T("./data/interface/im2_1.jtex"))
		DEFINE_LOAD(CREDITS_RIGHT2, _T("./data/interface/im2_2.jtex"))
		DEFINE_LOAD(CREDITS_LEFT3, _T("./data/interface/im3_1.jtex"))
		DEFINE_LOAD(CREDITS_RIGHT3, _T("./data/interface/im3_2.jtex"))
		DEFINE_LOAD(CREDITS_LEFT4, _T("./data/interface/im4_1.jtex"))
		DEFINE_LOAD(CREDITS_RIGHT4, _T("./data/interface/im4_2.jtex"))
		DEFINE_LOAD(CREDITS_LEFT5, _T("./data/interface/im5_1.jtex"))
		DEFINE_LOAD(CREDITS_RIGHT5, _T("./data/interface/im5_2.jtex"))
		DEFINE_LOAD(CREDITS_LEFT6, _T("./data/interface/im6_1.jtex"))
		DEFINE_LOAD(CREDITS_RIGHT6, _T("./data/interface/im6_2.jtex"))
		DEFINE_LOAD(CREDITS_LEFT7, _T("./data/interface/im7_1.jtex"))
		DEFINE_LOAD(CREDITS_RIGHT7, _T("./data/interface/im7_2.jtex"))
		DEFINE_LOAD(CREDITS_LEFT8, _T("./data/interface/im8_1.jtex"))
		DEFINE_LOAD(CREDITS_RIGHT8, _T("./data/interface/im8_2.jtex"))

		// Blood Castle interface
		DEFINE_LOAD(BLOODCASTLE_TIMER, _T("./data/interface/newui_figure_blood.ttex"))

		// Battle Soccer interface
		DEFINE_LOAD(BATTLESOCCER_TIMER, _T("./data/interface/newui_figure_ground.ttex"))

		// Castle Siege interface
		DEFINE_LOAD(CASTLESIEGE_MINIMAP, _T("./data/world31/map1.jtex"))
		DEFINE_LOAD(CASTLESIEGE_MINIMAP_FRAME, _T("./data/interface/newui_sw_minimap_frame.ttex"))
		DEFINE_LOAD(CASTLESIEGE_TIMER_FRAME, _T("./data/interface/newui_sw_time_frame.ttex"))
		DEFINE_LOAD(CASTLESIEGE_COMMAND_ATTACK, _T("./data/interface/i_attack.ttex"))
		DEFINE_LOAD(CASTLESIEGE_COMMAND_DEFEND, _T("./data/interface/i_defense.ttex"))
		DEFINE_LOAD(CASTLEISEGE_COMMAND_WAIT, _T("./data/interface/i_wait.ttex"))
		DEFINE_LOAD(CASTLESIEGE_BATTLESKILL_FRAME, _T("./data/interface/newui_sw_battleskill_frame.ttex"))
		DEFINE_LOAD(CASTLESIEGE_SCROLL_BTN_UP, _T("./data/interface/newui_bt_skill_scroll_up.jtex"))
		DEFINE_LOAD(CASTLESIEGE_SCROLL_BTN_DOWN, _T("./data/interface/newui_bt_skill_scroll_dn.jtex"))
		DEFINE_LOAD(CASTLESIEGE_MINIMAP_BTN_ALPHA, _T("./data/interface/newui_sw_minimap_bt_clearness.jtex"))
		DEFINE_LOAD(CASTLESIEGE_MINIMAP_BTN_GROUP, _T("./data/interface/newui_sw_minimap_bt_group.ttex"))
		DEFINE_LOAD(CASTLESIEGE_MINIMAP_BTN_COMMAND, _T("./data/interface/newui_sw_minimap_bt_command.ttex"))

		// Crywolf interface
		DEFINE_LOAD(CRYWOLF_BAR, _T("./data/interface/in_bar.ttex"))
		DEFINE_LOAD(CRYWOLF_BAR2, _T("./data/interface/in_bar2.jtex"))
		DEFINE_LOAD(CRYWOLF_DECO, _T("./data/interface/in_deco.ttex"))
		DEFINE_LOAD(CRYWOLF_MAIN1, _T("./data/interface/in_main-new.ttex"))
		DEFINE_LOAD(CRYWOLF_MAIN2, _T("./data/interface/in_main2-new.ttex"))
		DEFINE_LOAD(CRYWOLF_TMAIN, _T("./data/interface/t_main-new.ttex"))
		DEFINE_LOAD(CRYWOLF_BAL1, _T("./data/interface/in_main_icon_bal1.ttex"))
		DEFINE_LOAD(CRYWOLF_DARKELF1, _T("./data/interface/in_main_icon_dl1.ttex"))
		DEFINE_LOAD(CRYWOLF_DARKELF2, _T("./data/interface/in_main_icon_dl2.ttex"))
		DEFINE_LOAD(CRYWOLF_NUMBER0, _T("./data/interface/in_main_number0_2.ttex"))
		DEFINE_LOAD(CRYWOLF_NUMBER1_OFF, _T("./data/interface/in_main_number1.ttex"))
		DEFINE_LOAD(CRYWOLF_NUMBER1_ON, _T("./data/interface/in_main_number1_1.ttex"))
		DEFINE_LOAD(CRYWOLF_NUMBER2_OFF, _T("./data/interface/in_main_number2.ttex"))
		DEFINE_LOAD(CRYWOLF_NUMBER2_ON, _T("./data/interface/in_main_number2_1.ttex"))
		DEFINE_LOAD(CRYWOLF_FAILURE, _T("./data/interface/icon_failure.ttex"))
		DEFINE_LOAD(CRYWOLF_SUCCESS, _T("./data/interface/icon_success.ttex"))
		DEFINE_LOAD(CRYWOLF_BTN_NO1, _T("./data/interface/m_b_no1.ttex"))
		DEFINE_LOAD(CRYWOLF_BTN_NO2, _T("./data/interface/m_b_no2.ttex"))
		DEFINE_LOAD(CRYWOLF_BTN_NO3, _T("./data/interface/m_b_no3.ttex"))
		DEFINE_LOAD(CRYWOLF_BTN_YES1, _T("./data/interface/m_b_yes1.ttex"))
		DEFINE_LOAD(CRYWOLF_BTN_YES2, _T("./data/interface/m_b_yes2.ttex"))
		DEFINE_LOAD(CRYWOLF_BTN_YES3, _T("./data/interface/m_b_yes3.ttex"))
		DEFINE_LOAD(CRYWOLF_BTN_OK1, _T("./data/interface/m_b_ok1.ttex"))
		DEFINE_LOAD(CRYWOLF_BTN_OK2, _T("./data/interface/m_b_ok2.ttex"))
		DEFINE_LOAD(CRYWOLF_BTN_OK3, _T("./data/interface/m_b_ok3.ttex"))
		DEFINE_LOAD(CRYWOLF_M_MAIN, _T("./data/interface/m_main.ttex"))
		DEFINE_LOAD(CRYWOLF_M_MAIN_RANK, _T("./data/interface/m_main_rank.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_TABLE, _T("./data/interface/icon_rank_table.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_RANK, _T("./data/interface/icon_rank_rank.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_EXP, _T("./data/interface/icon_rank_exp.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_S, _T("./data/interface/icon_rank_s.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_A, _T("./data/interface/icon_rank_a.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_B, _T("./data/interface/icon_rank_b.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_C, _T("./data/interface/icon_rank_c.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_D, _T("./data/interface/icon_rank_d.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_0, _T("./data/interface/icon_rank_0.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_1, _T("./data/interface/icon_rank_1.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_2, _T("./data/interface/icon_rank_2.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_3, _T("./data/interface/icon_rank_3.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_4, _T("./data/interface/icon_rank_4.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_5, _T("./data/interface/icon_rank_5.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_6, _T("./data/interface/icon_rank_6.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_7, _T("./data/interface/icon_rank_7.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_8, _T("./data/interface/icon_rank_8.ttex"))
		DEFINE_LOAD(CRYWOLF_RANK_9, _T("./data/interface/icon_rank_9.ttex"))

		// Kanturu interface
		DEFINE_LOAD(KANTURU_WINDOW, _T("./data/interface/newui_figure_kantru.ttex"))

		// Illusion Temple interface
		DEFINE_LOAD(ILLUSIONTEMPLE_FAILURE, _T("./data/interface/illusion_failure.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SUCCESS, _T("./data/interface/illusion_success.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_MINIMAP, _T("./data/interface/newui_ctminmap.jtex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_MINIMAP_FRAME, _T("./data/interface/newui_ctminmapframe.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_MINIMAP_BTN_ALPHA, _T("./data/interface/newui_bt_clearness_illusion.jtex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_MINIMAP_RELIC, _T("./data/interface/newui_ctminmap_relic.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_MINIMAP_TEAMA_BOX, _T("./data/interface/newui_ctminmap_teama_box.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_MINIMAP_TEAMA_MEMBER, _T("./data/interface/newui_ctminmap_teama_member.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_MINIMAP_TEAMA_NPC, _T("./data/interface/newui_ctminmap_teama_npc.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_MINIMAP_TEAMB_BOX, _T("./data/interface/newui_ctminmap_teamb_box.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_MINIMAP_TEAMB_MEMBER, _T("./data/interface/newui_ctminmap_teamb_member.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_MINIMAP_TEAMB_NPC, _T("./data/interface/newui_ctminmap_teamb_npc.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_MINIMAP_HERO, _T("./data/interface/newui_ctminmap_hero.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SKILLFRAME, _T("./data/interface/newui_ctskillframe.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SKILLUP, _T("./data/interface/newui_ctskillup.jtex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SKILLDOWN, _T("./data/interface/newui_ctskilldown.jtex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_TIMER, _T("./data/interface/newui_ctgametimeframe.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_VS0, _T("./data/interface/newui_ctscorevs0.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_VS1, _T("./data/interface/newui_ctscorevs1.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_LEFT, _T("./data/interface/newui_ctscoreleft.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_RIGHT, _T("./data/interface/newui_ctscoreright.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ALLIED_GAAIL, _T("./data/interface/newui_ctscorealliedgaail.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ALLIED_NUM0, _T("./data/interface/newui_ctscorealliednum0.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ALLIED_NUM1, _T("./data/interface/newui_ctscorealliednum1.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ALLIED_NUM2, _T("./data/interface/newui_ctscorealliednum2.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ALLIED_NUM3, _T("./data/interface/newui_ctscorealliednum3.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ALLIED_NUM4, _T("./data/interface/newui_ctscorealliednum4.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ALLIED_NUM5, _T("./data/interface/newui_ctscorealliednum5.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ALLIED_NUM6, _T("./data/interface/newui_ctscorealliednum6.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ALLIED_NUM7, _T("./data/interface/newui_ctscorealliednum7.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ALLIED_NUM8, _T("./data/interface/newui_ctscorealliednum8.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ALLIED_NUM9, _T("./data/interface/newui_ctscorealliednum9.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ILLUSION_GAAIL, _T("./data/interface/newui_ctscoreillsiongaail.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ILLUSION_NUM0, _T("./data/interface/newui_ctscoreillusionnum0.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ILLUSION_NUM1, _T("./data/interface/newui_ctscoreillusionnum1.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ILLUSION_NUM2, _T("./data/interface/newui_ctscoreillusionnum2.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ILLUSION_NUM3, _T("./data/interface/newui_ctscoreillusionnum3.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ILLUSION_NUM4, _T("./data/interface/newui_ctscoreillusionnum4.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ILLUSION_NUM5, _T("./data/interface/newui_ctscoreillusionnum5.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ILLUSION_NUM6, _T("./data/interface/newui_ctscoreillusionnum6.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ILLUSION_NUM7, _T("./data/interface/newui_ctscoreillusionnum7.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ILLUSION_NUM8, _T("./data/interface/newui_ctscoreillusionnum8.ttex"))
		DEFINE_LOAD(ILLUSIONTEMPLE_SCORE_ILLUSION_NUM9, _T("./data/interface/newui_ctscoreillusionnum9.ttex"))

		// Doppelganger interface
		DEFINE_LOAD(DOPPELGANGER_WINDOWFRAME, _T("./data/interface/double_back.ttex"))
		DEFINE_LOAD(DOPPELGANGER_GAUGE_RED, _T("./data/interface/double_bar(r).jtex"))
		DEFINE_LOAD(DOPPELGANGER_GAUGE_ORANGE, _T("./data/interface/double_bar(o).jtex"))
		DEFINE_LOAD(DOPPELGANGER_GAUGE_YELLOW, _T("./data/interface/double_bar(y).jtex"))
		DEFINE_LOAD(DOPPELGANGER_GAUGE_PLAYER, _T("./data/interface/double_baricon04.ttex"))
		DEFINE_LOAD(DOPPELGANGER_GAUGE_PARTY_MEMBER, _T("./data/interface/double_baricon01.ttex"))
		DEFINE_LOAD(DOPPELGANGER_GAUGE_ICEWALKER, _T("./data/interface/double_micon01.ttex"))

		// Map Welcome interface
		DEFINE_LOAD(WELCOME_STRIFE, _T("./data/interface/mapnameaddstrife.ttex"))
		DEFINE_LOAD(WELCOME_LORENCIA, _T("./data/interface/lorencia.ttex"))
		DEFINE_LOAD(WELCOME_DUNGEON, _T("./data/interface/dungeun.ttex"))
		DEFINE_LOAD(WELCOME_DEVIAS, _T("./data/interface/devias.ttex"))
		DEFINE_LOAD(WELCOME_NORIA, _T("./data/interface/noria.ttex"))
		DEFINE_LOAD(WELCOME_LOSTTOWER, _T("./data/interface/losttower.ttex"))
		DEFINE_LOAD(WELCOME_STADIUM, _T("./data/interface/stadium.ttex"))
		DEFINE_LOAD(WELCOME_ATLANS, _T("./data/interface/atlans.ttex"))
		DEFINE_LOAD(WELCOME_TARKAN, _T("./data/interface/tarcan.ttex"))
		DEFINE_LOAD(WELCOME_DEVILSQUARE, _T("./data/interface/devilsquare.ttex"))
		DEFINE_LOAD(WELCOME_ICARUS, _T("./data/interface/icarus.ttex"))
		DEFINE_LOAD(WELCOME_BLOODCASTLE, _T("./data/interface/bloodcastle.ttex"))
		DEFINE_LOAD(WELCOME_CHAOSCASTLE, _T("./data/interface/chaoscastle.ttex"))
		DEFINE_LOAD(WELCOME_KALIMA, _T("./data/interface/kalima.ttex"))
		DEFINE_LOAD(WELCOME_VALLEYOFLOREN, _T("./data/interface/loren.ttex"))
		DEFINE_LOAD(WELCOME_LANDOFTRIAL, _T("./data/interface/ordeal.ttex"))
		DEFINE_LOAD(WELCOME_AIDA, _T("./data/interface/aida.ttex"))
		DEFINE_LOAD(WELCOME_CRYWOLF, _T("./data/interface/crywolffortress.ttex"))
		DEFINE_LOAD(WELCOME_KANTURU, _T("./data/interface/kantru.ttex"))
		DEFINE_LOAD(WELCOME_BALGASBARRACK, _T("./data/interface/balgasbarrack.ttex"))
		DEFINE_LOAD(WELCOME_BALGASREFUGE, _T("./data/interface/balgasrefuge.ttex"))
		DEFINE_LOAD(WELCOME_ILLUSIONTEMPLE, _T("./data/interface/illusiontemple.ttex"))
		DEFINE_LOAD(WELCOME_ELBELAND, _T("./data/interface/elbeland.ttex"))
		DEFINE_LOAD(WELCOME_SWAMPOFCALMNESS, _T("./data/interface/swampofcalmness.ttex"))
		DEFINE_LOAD(WELCOME_RAKLION, _T("./data/interface/mapname_raklion.ttex"))
		DEFINE_LOAD(WELCOME_RAKLIONBOSS, _T("./data/interface/mapname_raklionboss.ttex"))
		DEFINE_LOAD(WELCOME_SANTATOWN, _T("./data/interface/santatown.ttex"))
		DEFINE_LOAD(WELCOME_PKFIELD, _T("./data/interface/pkfield.ttex"))
		DEFINE_LOAD(WELCOME_DUELARENA, _T("./data/interface/duelarena.ttex"))
		DEFINE_LOAD(WELCOME_DOPPELGANGER, _T("./data/interface/doppelganger.ttex"))
		DEFINE_LOAD(WELCOME_EMPIREGUARDIAN, _T("./data/interface/empireguardian.ttex"))
		DEFINE_LOAD(WELCOME_LORENMARKET, _T("./data/interface/mapname_marketrolen.ttex"))
	};
	const mu_uint32 LoadCount = mu_countof(Load);
};