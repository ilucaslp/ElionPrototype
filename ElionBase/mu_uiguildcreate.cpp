#include "stdafx.h"
#include "mu_uiguildcreate.h"
#include "mu_uiguildcreate_defines.h"

UIGuildCreate::UIGuildCreate(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(GUILDCREATE_POSX, GUILDCREATE_POSY);
	UIBase::SetDimensions(GUILDCREATE_WIDTH, GUILDCREATE_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_HIDDEN);
	SetEvent(GFX_EVENT::GFX_MOUSE_EVENT | GFX_EVENT::GFX_MOUSE_DBLCLICK_AS_CLICK);

	// Initialize
	{
		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_BACK], SCREEN_WIDTH, SCREEN_HEIGHT, BACK_POSX, BACK_POSY, BACK_WIDTH, BACK_HEIGHT, BACK_UVBEGIN, BACK_UVEND);
		_TopFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_TOP], SCREEN_WIDTH, SCREEN_HEIGHT, TOP_POSX, TOP_POSY, TOP_WIDTH, TOP_HEIGHT);
		_TopFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BottomFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_BOTTOM], SCREEN_WIDTH, SCREEN_HEIGHT, BOTTOM_POSX, BOTTOM_POSY, BOTTOM_WIDTH, BOTTOM_HEIGHT);
		_BottomFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_LeftFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, LEFT_POSX, LEFT_POSY, LEFT_WIDTH, LEFT_HEIGHT);
		_LeftFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_RightFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, RIGHT_POSX, RIGHT_POSY, RIGHT_WIDTH, RIGHT_HEIGHT);
		_RightFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_BigGoldTop = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TRADING_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, GOLDLINE_BIGTOP_POSX, GOLDLINE_BIGTOP_POSY, GOLDLINE_BIGHOR_WIDTH, GOLDLINE_BIGHOR_HEIGHT, GOLDLINE_HOR_UV, GOLDLINE_HOR_UV + GOLDLINE_HOR_UVSIZE);
		_BigGoldTop->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BigGoldBottom = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TRADING_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, GOLDLINE_BIGBOTTOM_POSX, GOLDLINE_BIGBOTTOM_POSY, GOLDLINE_BIGHOR_WIDTH, GOLDLINE_BIGHOR_HEIGHT, GOLDLINE_HOR_UV, GOLDLINE_HOR_UV + GOLDLINE_HOR_UVSIZE);
		_BigGoldBottom->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BigGoldLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::ITEM_BACK01], SCREEN_WIDTH, SCREEN_HEIGHT, GOLDLINE_BIGLEFT_POSX, GOLDLINE_BIGLEFT_POSY, GOLDLINE_BIGVER_WIDTH, GOLDLINE_BIGVER_HEIGHT, GOLDLINE_VER_UV, GOLDLINE_VER_UV + GOLDLINE_VER_UVSIZE);
		_BigGoldLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BigGoldRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::ITEM_BACK01], SCREEN_WIDTH, SCREEN_HEIGHT, GOLDLINE_BIGRIGHT_POSX, GOLDLINE_BIGRIGHT_POSY, GOLDLINE_BIGVER_WIDTH, GOLDLINE_BIGVER_HEIGHT, GOLDLINE_VER_UV, GOLDLINE_VER_UV + GOLDLINE_VER_UVSIZE);
		_BigGoldRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_SmallGoldTop = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TRADING_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, GOLDLINE_SMALLTOP_POSX, GOLDLINE_SMALLTOP_POSY, GOLDLINE_SMALLHOR_WIDTH, GOLDLINE_SMALLHOR_HEIGHT, GOLDLINE_HOR_UV, GOLDLINE_HOR_UV + GOLDLINE_HOR_UVSIZE);
		_SmallGoldTop->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_SmallGoldBottom = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TRADING_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, GOLDLINE_SMALLBOTTOM_POSX, GOLDLINE_SMALLBOTTOM_POSY, GOLDLINE_SMALLHOR_WIDTH, GOLDLINE_SMALLHOR_HEIGHT, GOLDLINE_HOR_UV, GOLDLINE_HOR_UV + GOLDLINE_HOR_UVSIZE);
		_SmallGoldBottom->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_SmallGoldLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::ITEM_BACK01], SCREEN_WIDTH, SCREEN_HEIGHT, GOLDLINE_SMALLLEFT_POSX, GOLDLINE_SMALLLEFT_POSY, GOLDLINE_SMALLVER_WIDTH, GOLDLINE_SMALLVER_HEIGHT, GOLDLINE_VER_UV, GOLDLINE_VER_UV + GOLDLINE_VER_UVSIZE);
		_SmallGoldLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_SmallGoldRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::ITEM_BACK01], SCREEN_WIDTH, SCREEN_HEIGHT, GOLDLINE_SMALLRIGHT_POSX, GOLDLINE_SMALLRIGHT_POSY, GOLDLINE_SMALLVER_WIDTH, GOLDLINE_SMALLVER_HEIGHT, GOLDLINE_VER_UV, GOLDLINE_VER_UV + GOLDLINE_VER_UVSIZE);
		_SmallGoldRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		for (mu_uint32 y = 0; y < GUILDMARK_HEIGHT; ++y)
		{
			for (mu_uint32 x = 0; x < GUILDMARK_WIDTH; ++x)
			{
				_MarksBack[y][x] = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::INVENTORY_BOX02], SCREEN_WIDTH, SCREEN_HEIGHT, MARKS_POSX(x) - 1.0f, MARKS_POSY(y) - 1.0f, MARKS_WIDTH + 2.0f, MARKS_HEIGHT + 2.0f);

				_MarksBtn[y][x] = new UIOverButton(GAMEUI::MESSAGE_GUILDCREATE_MARKS_BUTTON_BEGIN + (y * GUILDMARK_WIDTH) + x, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MARKS_POSX(x), MARKS_POSY(y), MARKS_WIDTH, MARKS_HEIGHT);
			}
		}

		for (mu_uint32 y = 0; y < 2; ++y)
		{
			for (mu_uint32 x = 0; x < MAX_GUILD_COLORS_HALF; ++x)
			{
				mu_uint32 colorIndex = (y * MAX_GUILD_COLORS_HALF) + x;
				_ColorsBack[y][x] = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::INVENTORY_BOX02], SCREEN_WIDTH, SCREEN_HEIGHT, COLORS_POSX(x) - 1.0f, COLORS_POSY(y) - 1.0f, COLORS_WIDTH + 2.0f, COLORS_HEIGHT + 2.0f);

				_ColorsBtn[y][x] = new UIOverButton(GAMEUI::MESSAGE_GUILDCREATE_COLORS_BUTTON_BEGIN + colorIndex, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, COLORS_POSX(x), COLORS_POSY(y), COLORS_WIDTH, COLORS_HEIGHT);
			}
		}

		_SelectedBack = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::INVENTORY_BOX02], SCREEN_WIDTH, SCREEN_HEIGHT, SELECTCOLOR_POSX - 1.0f, SELECTCOLOR_POSY - 1.0f, SELECTCOLOR_WIDTH + 2.0f, SELECTCOLOR_HEIGHT + 2.0f);
		_SelectedColor = new UISprite(manager, this, &_ColorsTexture[0], SCREEN_WIDTH, SCREEN_HEIGHT, SELECTCOLOR_POSX, SELECTCOLOR_POSY, SELECTCOLOR_WIDTH, SELECTCOLOR_HEIGHT);
		_SelectedColor->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_MarkSprite = new UISprite(manager, this, nullptr, SCREEN_WIDTH, SCREEN_HEIGHT, MARK_POSX, MARK_POSY, MARK_WIDTH, MARK_HEIGHT);
		_MarkSprite->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MarkSprite->SetSampler(ESampler::eUcVcWcMinNMagNMipN);

		_InputBack = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::GUILD_MAKE_EDITBOX], SCREEN_WIDTH, SCREEN_HEIGHT, INPUTBACK_POSX, INPUTBACK_POSY, INPUTBACK_WIDTH, INPUTBACK_HEIGHT);
		_InputBack->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_InputBox = new UIEditBox(GAMEUI::MESSAGE_GUILDCREATE_NAMEINPUT, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, INPUT_POSX, INPUT_POSY, INPUT_WIDTH, INPUT_HEIGHT);
		_InputBox->SetLimitText(GUILDNAME_MAXIMUM);
		_InputBox->SetTextColor(255, 230, 210, 255);
		_InputBox->SetBackColor(0, 0, 0, 0);
		_InputBox->SetHighlightTextColor(0, 0, 0, 255);
		_InputBox->SetHighlightColor(150, 150, 150, 255);

		_BackBtn = new UIButton(GAMEUI::MESSAGE_GUILDCREATE_BACK_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, BACKBTN_POSX, BACKBTN_POSY, SMALLBTN_WIDTH, SMALLBTN_HEIGHT);
		_BackBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BackBtn->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_BackBtn->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_BackBtn->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_BackBtn->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_BackBtn->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[1306], MU::MakeARGB(100, 100, 100, 255));
		_BackBtn->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[1306], MU::MakeARGB(200, 200, 200, 255));
		_BackBtn->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[1306], MU::MakeARGB(255, 255, 255, 255));
		_BackBtn->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[1306], MU::MakeARGB(150, 150, 150, 255));

		_NextBtn = new UIButton(GAMEUI::MESSAGE_GUILDCREATE_NEXT_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, NEXTBTN_POSX, NEXTBTN_POSY, SMALLBTN_WIDTH, SMALLBTN_HEIGHT);
		_NextBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_NextBtn->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_NextBtn->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_NextBtn->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_NextBtn->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_NextBtn->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[1305], MU::MakeARGB(100, 100, 100, 255));
		_NextBtn->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[1305], MU::MakeARGB(200, 200, 200, 255));
		_NextBtn->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[1305], MU::MakeARGB(255, 255, 255, 255));
		_NextBtn->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[1305], MU::MakeARGB(150, 150, 150, 255));

		_MakeBtn = new UIButton(GAMEUI::MESSAGE_GUILDCREATE_NEXT_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MAKEBTN_POSX, MAKEBTN_POSY, MEDIUMBTN_WIDTH, MEDIUMBTN_HEIGHT);
		_MakeBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MakeBtn->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_MakeBtn->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_MakeBtn->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_MakeBtn->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_MakeBtn->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[1303], MU::MakeARGB(100, 100, 100, 255));
		_MakeBtn->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[1303], MU::MakeARGB(200, 200, 200, 255));
		_MakeBtn->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[1303], MU::MakeARGB(255, 255, 255, 255));
		_MakeBtn->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[1303], MU::MakeARGB(150, 150, 150, 255));

		_XButton = new UIButton(GAMEUI::MESSAGE_GUILDCREATE_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, XBUTTON_POSX, XBUTTON_POSY, XBUTTON_WIDTH, XBUTTON_HEIGHT);

		_CloseButton = new UIButton(GAMEUI::MESSAGE_GUILDCREATE_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, GUILDCREATE_POSX + 13.0f, GUILDCREATE_POSY + 391.0f, 36.0f, 29.0f);
		_CloseButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CloseButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_CloseButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));

		InitializeWindow();
		InitializeGPU();
	}
}

UIGuildCreate::~UIGuildCreate()
{
	ReleaseGPU();
}

void UIGuildCreate::InitializeGPU()
{
	_MarkTexture.Create2D(GUILDMARK_WIDTH, GUILDMARK_HEIGHT, ETextureFormat::eRGBA8_Unorm, ETextureUsage::eDynamic, ESampler::eUcVcWcMinNMagNMipN, nullptr, 0);
	_MarkStagingBuffer.Allocate(sizeof(mu_uint8) * 4 * GUILDMARK_WIDTH * GUILDMARK_HEIGHT, true);
	_MarkSprite->SetTexture(&_MarkTexture);

	for (mu_uint32 n = 0; n < MAX_GUILD_COLORS; ++n)
	{
		mu_uint8 markBuffer[MAX_GUILDMARKBUFFER];

		if (n == 0)
		{
			mu_memset(markBuffer, n, MAX_GUILDMARKBUFFER);

			mu_uint8 *tmpBuffer = markBuffer;
			for (mu_uint32 i = 0; i < 8; ++i)
			{
				tmpBuffer[i * GUILDMARK_WIDTH + i] = 2;
				tmpBuffer[i * GUILDMARK_WIDTH + (GUILDMARK_WIDTH - 1) - i] = 2;
			}
		}
		else
		{
			mu_memset(markBuffer, n, MAX_GUILDMARKBUFFER);
		}

		_ColorsTexture[n].Create2D(GUILDMARK_WIDTH, GUILDMARK_HEIGHT, ETextureFormat::eRGBA8_Unorm, ETextureUsage::eDynamic, ESampler::eUcVcWcMinNMagNMipN, nullptr, 0);
		_ColorsStagingBuffer[n].Allocate(sizeof(mu_uint8) * 4 * GUILDMARK_WIDTH * GUILDMARK_HEIGHT, true);

		CreateGuildMark(markBuffer, false, &_ColorsTexture[n], &_ColorsStagingBuffer[n]);
	}

	for (mu_uint32 y = 0; y < GUILDMARK_HEIGHT; ++y)
	{
		for (mu_uint32 x = 0; x < GUILDMARK_WIDTH; ++x)
		{
			_MarksBtn[y][x]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_MarksBtn[y][x]->SetStateData(UIOverButton::UI_DISABLED, UI_STATE_DATA(&_ColorsTexture[0], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
			_MarksBtn[y][x]->SetStateData(UIOverButton::UI_NORMAL, UI_STATE_DATA(&_ColorsTexture[0], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		}
	}

	for (mu_uint32 y = 0; y < 2; ++y)
	{
		for (mu_uint32 x = 0; x < MAX_GUILD_COLORS_HALF; ++x)
		{
			mu_uint32 colorIndex = (y * MAX_GUILD_COLORS_HALF) + x;
			_ColorsBtn[y][x]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_ColorsBtn[y][x]->SetStateData(UIOverButton::UI_DISABLED, UI_STATE_DATA(&_ColorsTexture[colorIndex], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
			_ColorsBtn[y][x]->SetStateData(UIOverButton::UI_NORMAL, UI_STATE_DATA(&_ColorsTexture[colorIndex], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		}
	}

	UIBase::InitializeGPU();
}

void UIGuildCreate::ReleaseGPU()
{
	_MarkTexture.Release();
	_MarkStagingBuffer.Deallocate();

	for (mu_uint32 n = 0; n < MAX_GUILD_COLORS; ++n)
	{
		_ColorsTexture[n].Release();
		_ColorsStagingBuffer[n].Deallocate();
	}

	UIBase::ReleaseGPU();
}

void UIGuildCreate::InitializeWindow()
{
	_CurrentPage = PAGE_INFO;

	for (mu_uint32 y = 0; y < GUILDMARK_HEIGHT; ++y)
	{
		for (mu_uint32 x = 0; x < GUILDMARK_WIDTH; ++x)
		{
			_MarksBtn[y][x]->SetStateData(UIOverButton::UI_DISABLED, UI_STATE_DATA(&_ColorsTexture[0], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
			_MarksBtn[y][x]->SetStateData(UIOverButton::UI_NORMAL, UI_STATE_DATA(&_ColorsTexture[0], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		}
	}

	_SelectedColor->SetTexture(&_ColorsTexture[0]);
	_ColorIndex = 0;
	mu_zeromem(_ColorBuffer, sizeof(_ColorBuffer));

	_InputBox->ClearAllText();
}

void UIGuildCreate::Update(mu_uint64 CurrentTime)
{
	switch (_CurrentPage)
	{
	case PAGE_MARK:
		{
			_InputBox->Update(CurrentTime);
		}
		break;
	}
}

void UIGuildCreate::Draw()
{
	DrawFrames();
	DrawButtons();
	DrawTexts();
}

void UIGuildCreate::PostDraw()
{
	PostDrawButtons();
}

void UIGuildCreate::DrawFrames()
{
	_BackFrame->Draw();
	_TopFrame->Draw();
	_BottomFrame->Draw();
	_LeftFrame->Draw();
	_RightFrame->Draw();

	switch (_CurrentPage)
	{
	case PAGE_INFO:
		{
		}
		break;

	case PAGE_MARK:
		{
			_InputBack->Draw();

			_BigGoldLeft->Draw();
			_BigGoldRight->Draw();
			_BigGoldTop->Draw();
			_BigGoldBottom->Draw();
		}
		break;

	case PAGE_RESULT:
		{
			_SmallGoldLeft->Draw();
			_SmallGoldRight->Draw();
			_SmallGoldTop->Draw();
			_SmallGoldBottom->Draw();

			_MarkSprite->Draw();
		}
		break;
	}
}

void UIGuildCreate::DrawButtons()
{
	switch (_CurrentPage)
	{
	case PAGE_INFO:
		{
			_CloseButton->Draw();
			_MakeBtn->Draw();
		}
		break;

	case PAGE_MARK:
		{
			for (mu_uint32 y = 0; y < GUILDMARK_HEIGHT; ++y)
			{
				for (mu_uint32 x = 0; x < GUILDMARK_WIDTH; ++x)
				{
					_MarksBack[y][x]->Draw();
					_MarksBtn[y][x]->Draw();
				}
			}

			for (mu_uint32 y = 0; y < 2; ++y)
			{
				for (mu_uint32 x = 0; x < MAX_GUILD_COLORS_HALF; ++x)
				{
					_ColorsBack[y][x]->Draw();
					_ColorsBtn[y][x]->Draw();
				}
			}

			_SelectedBack->Draw();
			_SelectedColor->Draw();

			_BackBtn->Draw();
			_NextBtn->Draw();
		}
		break;

	case PAGE_RESULT:
		{
			_BackBtn->Draw();
			_NextBtn->Draw();
		}
		break;
	}
}

void UIGuildCreate::PostDrawButtons()
{
	switch (_CurrentPage)
	{
	case PAGE_INFO:
		{
			_CloseButton->PostDraw();
			_MakeBtn->PostDraw();
		}
		break;

	case PAGE_MARK:
		{
			for (mu_uint32 y = 0; y < GUILDMARK_HEIGHT; ++y)
			{
				for (mu_uint32 x = 0; x < GUILDMARK_WIDTH; ++x)
				{
					_MarksBtn[y][x]->PostDraw();
				}
			}

			for (mu_uint32 y = 0; y < 2; ++y)
			{
				for (mu_uint32 x = 0; x < MAX_GUILD_COLORS_HALF; ++x)
				{
					_ColorsBtn[y][x]->PostDraw();
				}
			}

			_BackBtn->PostDraw();
			_NextBtn->PostDraw();
		}
		break;

	case PAGE_RESULT:
		{
			_BackBtn->PostDraw();
			_NextBtn->PostDraw();
		}
		break;
	}
}

void UIGuildCreate::DrawTexts()
{
	mu_float screenX = MU_RESOURCE->_640x480Rate[0];
	mu_float screenY = MU_RESOURCE->_640x480Rate[1];
	mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255);
	mu_uint32 bgColor = MU::MakeARGB(0, 0, 0, 0);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::GUILDCREATE_TITLE,
		MU_TO_WINDOW(_ViewX + GUILDCREATE_WIDTH * 0.5f, screenX),
		MU_TO_WINDOW(_ViewY + 15.0f, screenY),
		1.0f,
		ETA_CENTER,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[180]);


	switch (_CurrentPage)
	{
	case PAGE_INFO:
		{
			FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDCREATE_MAKETITLE,
				MU_TO_WINDOW(_ViewX + _ViewWidth * 0.5f, screenX),
				MU_TO_WINDOW(_ViewY + 50.0f, screenY),
				1.0f,
				ETA_CENTER,
				ETA_CENTER,
				0.0f,
				textColor,
				bgColor,
				GlobalText[181]);
		}
		break;

	case PAGE_MARK:
		{
			_InputBox->Draw();

			static mu_text markBuffer[256] = { 0 };
			static mu_boolean markInit = false;

			if (markInit == false)
			{
				markInit = true;
				mu_sprintf_s(markBuffer, mu_countof(markBuffer), _T("") SPF_STRING _T("\n\n") SPF_STRING _T(""), GlobalText[183], GlobalText[184]);
			}

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDCREATE_MARKTITLE,
				MU_TO_WINDOW(_ViewX + 18.0f, screenX),
				MU_TO_WINDOW(_ViewY + 66.0f, screenY),
				1.0f,
				ETA_LEFT,
				ETA_BOTTOM,
				0.0f,
				MU::MakeARGB(255, 176, 73, 255),
				bgColor,
				GlobalText[182]);

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDCREATE_MARKINFO,
				MU_TO_WINDOW(_ViewX + 50.0f, screenX),
				MU_TO_WINDOW(_ViewY + 230.0f, screenY),
				1.0f,
				ETA_LEFT,
				ETA_BOTTOM,
				0.0f,
				MU::MakeARGB(230, 230, 230, 255),
				bgColor,
				markBuffer);
		}
		break;

	case PAGE_RESULT:
		{
			mu_text textBuffer[512];
			mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("") SPF_STRING _T(" : ") SPF_STRING _T(""), GlobalText[182], _GuildName.c_str());

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDCREATE_RESULTTITLE,
				MU_TO_WINDOW(_ViewX + _ViewWidth * 0.5f, screenX),
				MU_TO_WINDOW(_ViewY + 140.0f, screenY),
				1.0f,
				ETA_CENTER,
				ETA_CENTER,
				0.0f,
				MU::MakeARGB(230, 230, 230, 255),
				bgColor,
				textBuffer);
		}
		break;
	}
}

UIBase *UIGuildCreate::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _XButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		switch (_CurrentPage)
		{
		case PAGE_INFO:
			{
				object = _CloseButton->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}

				object = _MakeBtn->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}
			}
			break;

		case PAGE_MARK:
			{
				object = _InputBox->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}

				for (mu_uint32 y = 0; y < GUILDMARK_HEIGHT; ++y)
				{
					for (mu_uint32 x = 0; x < GUILDMARK_WIDTH; ++x)
					{
						object = _MarksBtn[y][x]->GetFocus(mouseX, mouseY);

						if (object != nullptr)
						{
							return object;
						}
					}
				}

				for (mu_uint32 y = 0; y < 2; ++y)
				{
					for (mu_uint32 x = 0; x < MAX_GUILD_COLORS_HALF; ++x)
					{
						object = _ColorsBtn[y][x]->GetFocus(mouseX, mouseY);

						if (object != nullptr)
						{
							return object;
						}
					}
				}

				object = _BackBtn->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}

				object = _NextBtn->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}
			}
			break;

		case PAGE_RESULT:
			{
				object = _BackBtn->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}

				object = _NextBtn->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}
			}
			break;
		}
	}

	return focus;
}

void UIGuildCreate::SetSelectedColor(mu_uint32 index)
{
	_ColorIndex = index;
	_SelectedColor->SetTexture(&_ColorsTexture[index]);
}

void UIGuildCreate::SetBufferColor(mu_uint32 index)
{
	_ColorBuffer[index] = _ColorIndex;

	UIOverButton *btn = _MarksBtn[index / GUILDMARK_WIDTH][index % GUILDMARK_WIDTH];
	
	btn->SetStateData(UIOverButton::UI_DISABLED, UI_STATE_DATA(&_ColorsTexture[_ColorIndex], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
	btn->SetStateData(UIOverButton::UI_NORMAL, UI_STATE_DATA(&_ColorsTexture[_ColorIndex], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
}

void UIGuildCreate::BackPage()
{
	if (_CurrentPage > PAGE_INFO)
	{
		--_CurrentPage;
	}
}

void UIGuildCreate::NextPage()
{
	if (_CurrentPage < PAGE_RESULT)
	{
		mu_text nameBuffer[GUILDNAME_MAXIMUM + 1] = { 0 };

		if (_CurrentPage == PAGE_MARK)
		{
			if (_InputBox->GetLength() < GUILDNAME_MINIMUM ||
				_InputBox->GetLength() > GUILDNAME_MAXIMUM)
			{
				return;
			}

			_InputBox->GetText(nameBuffer, mu_countof(nameBuffer));

			mu_regex guildRegex(GUILDNAME_REGEX);

			if (boost::regex_match(nameBuffer, guildRegex) == false)
			{
				g_pChatListBox->AddText(_T(""), GlobalText[940], TYPE_ERROR_MESSAGE);
				return;
			}
		}

		++_CurrentPage;

		if (_CurrentPage == PAGE_RESULT)
		{
			_GuildName = nameBuffer;
			CreateGuildMark(_ColorBuffer, true, &_MarkTexture, &_MarkStagingBuffer);
		}
	}
	else
	{
		// Create guild packet
		CL2GS_REQUEST_CREATEGUILD RequestCreateGuild;

		mu_memcpy(RequestCreateGuild.MarkBuffer, _ColorBuffer, sizeof(RequestCreateGuild.MarkBuffer));
		RequestCreateGuild.GuildName = _GuildName;

		g_GameServerObject->Send(&RequestCreateGuild);

		g_pNewUISystem->Hide(GAMEUI::INTERFACE_NPCGUILDMASTER);
	}
}