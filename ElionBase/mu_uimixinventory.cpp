#include "stdafx.h"
#include "mu_uimixinventory.h"

mu_int32 g_ChaosTaxRate = 0;

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define BACKTEXTURE_WIDTH 225.0f
#define BACKTEXTURE_HEIGHT 512.0f

#define BACK_POSX MIXINVENTORY_POSX
#define BACK_POSY MIXINVENTORY_POSY
#define BACK_WIDTH MIXINVENTORY_WIDTH
#define BACK_HEIGHT MIXINVENTORY_HEIGHT
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND EVector2(BACK_WIDTH / BACKTEXTURE_WIDTH, BACK_HEIGHT / BACKTEXTURE_HEIGHT)

#define TOP_WIDTH MIXINVENTORY_WIDTH
#define TOP_HEIGHT 64.0f
#define TOP_POSX MIXINVENTORY_POSX
#define TOP_POSY MIXINVENTORY_POSY

#define BOTTOM_WIDTH MIXINVENTORY_WIDTH
#define BOTTOM_HEIGHT 45.0f
#define BOTTOM_POSX MIXINVENTORY_POSX
#define BOTTOM_POSY (MIXINVENTORY_POSY + MIXINVENTORY_HEIGHT - BOTTOM_HEIGHT)

#define LEFT_WIDTH 21.0f
#define LEFT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define LEFT_POSX MIXINVENTORY_POSX
#define LEFT_POSY (TOP_POSY + TOP_HEIGHT)

#define RIGHT_WIDTH 21.0f
#define RIGHT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define RIGHT_POSX (MIXINVENTORY_POSX + MIXINVENTORY_WIDTH - RIGHT_WIDTH)
#define RIGHT_POSY (TOP_POSY + TOP_HEIGHT)

#define XBUTTON_WIDTH 16.0f
#define XBUTTON_HEIGHT 16.0f
#define XBUTTON_POSX (TOP_POSX + TOP_WIDTH - 7.0f - XBUTTON_WIDTH)
#define XBUTTON_POSY (TOP_POSY + 7.0f)

UIMixInventory::UIMixInventory(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(MIXINVENTORY_POSX, MIXINVENTORY_POSY);
	UIBase::SetDimensions(MIXINVENTORY_WIDTH, MIXINVENTORY_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_HIDDEN);
	SetEvent(GFX_EVENT::GFX_MOUSE_EVENT | GFX_EVENT::GFX_MOUSE_DBLCLICK_AS_CLICK);

	// Initialize
	{
		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_BACK], SCREEN_WIDTH, SCREEN_HEIGHT, BACK_POSX, BACK_POSY, BACK_WIDTH, BACK_HEIGHT, BACK_UVBEGIN, BACK_UVEND);
		_TopFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_TOP_EXT], SCREEN_WIDTH, SCREEN_HEIGHT, TOP_POSX, TOP_POSY, TOP_WIDTH, TOP_HEIGHT);
		_TopFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BottomFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_BOTTOM], SCREEN_WIDTH, SCREEN_HEIGHT, BOTTOM_POSX, BOTTOM_POSY, BOTTOM_WIDTH, BOTTOM_HEIGHT);
		_BottomFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_LeftFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, LEFT_POSX, LEFT_POSY, LEFT_WIDTH, LEFT_HEIGHT);
		_LeftFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_RightFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, RIGHT_POSX, RIGHT_POSY, RIGHT_WIDTH, RIGHT_HEIGHT);
		_RightFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_InventoryControl = new UIInventoryControl(manager, this, TEXTINDEX::BEGIN_MIXINVENTORY_ITEMNUMBER, SCREEN_WIDTH, SCREEN_HEIGHT, MIXINVENTORY_POSX + 15.0f, MIXINVENTORY_POSY + 110.0f, COLUMN_MIX_INVENTORY, ROW_MIX_INVENTORY, INVENTORY_TYPE::IT_MIX);

		_XButton = new UIButton(GAMEUI::MESSAGE_MYINVENTORY_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, XBUTTON_POSX, XBUTTON_POSY, XBUTTON_WIDTH, XBUTTON_HEIGHT);

		_MixButton = new UIButton(GAMEUI::MESSAGE_MIXINVENTORY_MIX_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MIXINVENTORY_POSX + MIXINVENTORY_WIDTH * 0.5f - 22.0f, MIXINVENTORY_POSY + 380.0f, 44.0f, 35.0f);
		_MixButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MixButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MIX_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_MixButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MIX_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_MixButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MIX_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_MixButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MIX_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_MixButton->SetTooltip(FONT_NORMAL, GlobalText[591], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		for (mu_uint32 ty = 0; ty < ROW_MIX_INVENTORY; ++ty)
		{
			for (mu_uint32 tx = 0; tx < COLUMN_MIX_INVENTORY; ++tx)
			{
				UISprite **ShineEffect = _ShineEffect[ty * COLUMN_MIX_INVENTORY + tx];
				mu_float Scale = 5.0f;
				ShineEffect[0] = new UISprite(manager, this, &TEXTURES::Data[BITMAP_SHINY], SCREEN_WIDTH, SCREEN_HEIGHT, _InventoryControl->_ViewX + tx * INVENTORY_SQUARE_WIDTH, _InventoryControl->_ViewY + ty * INVENTORY_SQUARE_HEIGHT, Scale, Scale);
				ShineEffect[1] = new UISprite(manager, this, &TEXTURES::Data[BITMAP_SHINY], SCREEN_WIDTH, SCREEN_HEIGHT, _InventoryControl->_ViewX + tx * INVENTORY_SQUARE_WIDTH, _InventoryControl->_ViewY + ty * INVENTORY_SQUARE_HEIGHT, Scale, Scale);
				ShineEffect[2] = new UISprite(manager, this, &TEXTURES::Data[BITMAP_SHINY + 1], SCREEN_WIDTH, SCREEN_HEIGHT, _InventoryControl->_ViewX + tx * INVENTORY_SQUARE_WIDTH, _InventoryControl->_ViewY + ty * INVENTORY_SQUARE_HEIGHT, Scale * 3.0f, Scale * 3.0f);
				ShineEffect[3] = new UISprite(manager, this, &TEXTURES::Data[BITMAP_LIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, _InventoryControl->_ViewX + tx * INVENTORY_SQUARE_WIDTH, _InventoryControl->_ViewY + ty * INVENTORY_SQUARE_HEIGHT, Scale * 6.0f, Scale * 6.0f);

				ShineEffect[0]->SetBlendState(EBlendState::eOne_One);
				ShineEffect[1]->SetBlendState(EBlendState::eOne_One);
				ShineEffect[2]->SetBlendState(EBlendState::eOne_One);
				ShineEffect[3]->SetBlendState(EBlendState::eOne_One);
			}
		}

		SetPosition(MIXINVENTORY_POSX, MIXINVENTORY_POSY);

		_MixState = MIX_WAITING;
		_MixEffectTimer = 0;
	}
}

UIMixInventory::~UIMixInventory()
{

}

void UIMixInventory::Configure()
{
	switch (g_MixManager.GetMixType())
	{
	case MIXTYPE_TRAINER:
		{
			_MixButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[1212], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[1212], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[1212], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[1212], MU::MakeARGB(100, 100, 100, 255));
		}
		break;
	case MIXTYPE_OSBOURNE:
		{
			_MixButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[2061], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[2061], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[2061], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[2061], MU::MakeARGB(100, 100, 100, 255));
		}
		break;
	case MIXTYPE_JERRIDON:
		{
			_MixButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[2062], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[2062], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[2062], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[2062], MU::MakeARGB(100, 100, 100, 255));
		}
		break;
	case MIXTYPE_ELPIS:
		{
			_MixButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[2063], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[2063], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[2063], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[2063], MU::MakeARGB(100, 100, 100, 255));
		}
		break;
	case MIXTYPE_EXTRACT_SEED:
		{
			_MixButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[2660], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[2660], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[2660], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[2660], MU::MakeARGB(100, 100, 100, 255));
		}
		break;
	case MIXTYPE_SEED_SPHERE:
		{
			_MixButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[2661], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[2661], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[2661], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[2661], MU::MakeARGB(100, 100, 100, 255));
		}
		break;
	case MIXTYPE_ATTACH_SOCKET:
		{
			_MixButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[2662], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[2662], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[2662], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[2662], MU::MakeARGB(100, 100, 100, 255));
		}
		break;
	case MIXTYPE_DETACH_SOCKET:
		{
			_MixButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[2663], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[2663], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[2663], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[2663], MU::MakeARGB(100, 100, 100, 255));
		}
		break;
	default:
		{
			_MixButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[591], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[591], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[591], MU::MakeARGB(100, 100, 100, 255));
			_MixButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[591], MU::MakeARGB(100, 100, 100, 255));
		}
		break;
	}

	UpdateMix();
}

void UIMixInventory::UpdateMix()
{
	g_MixManager.ResetMixItemInventory();
	auto itemsMap = _InventoryControl->GetItems();
	for (auto it = itemsMap.begin(); it != itemsMap.end(); ++it)
	{
		g_MixManager.AddItemToMixItemInventory(it->second);
	}
	g_MixManager.CheckMixInventory();

	switch (g_MixManager.GetMixType())
	{
	case MIXTYPE_ATTACH_SOCKET:
	case MIXTYPE_DETACH_SOCKET:
		{
#if 0 // FIX
			if (m_SocketListBox.GetLineNum() == 0)
			{
				mu_text szText[64] = { 0, };
				mu_text szSocketText[64] = { 0, };
				for (int i = 0; i < g_MixManager.GetFirstItemSocketCount(); ++i)
				{
					if (g_MixManager.GetFirstItemSocketSeedID(i) == SOCKET_EMPTY)
					{
						mu_strcpy_s(szSocketText, mu_countof(szSocketText), GlobalText[2652]);
					}
					else
					{
						g_SocketItemMgr.CreateSocketOptionText(szSocketText, mu_countof(szSocketText), g_MixManager.GetFirstItemSocketSeedID(i), g_MixManager.GetFirstItemSocketShpereLv(i));
					}
					mu_sprintf_s(szText, mu_countof(szText), _T("%d: ") SPF_STRING _T(""), i + 1, szSocketText);
					m_SocketListBox.AddText(i, szText);
				}
				m_SocketListBox.SLSetSelectLine(0);
			}
			else
			{
				if (g_MixManager.GetFirstItemSocketCount() == 0)
				{
					m_SocketListBox.Clear();
				}
			}
			m_SocketListBox.DoAction();
#endif
		}
		break;
	}
}

void UIMixInventory::Update(mu_uint64 CurrentTime)
{

}

void UIMixInventory::Draw()
{
	DrawFrames();
	DrawInventoryDetails();
	DrawButtons();
	_InventoryControl->Draw();

	if (g_CurrentTime <= _MixEffectTimer)
	{
		mu_float vx = _InventoryControl->_ViewX;
		mu_float vy = _InventoryControl->_ViewY;

		auto &itemsMap = _InventoryControl->GetItems();
		for (auto it = itemsMap.begin(); it != itemsMap.end(); ++it)
		{
			ITEM *item = it->second;
			ITEM_ATTRIBUTE *itemAttr = &ItemAttribute[item->Type];
			mu_uint32 ix = item->x;
			mu_uint32 iy = item->y;
			mu_uint32 iw = itemAttr->Width;
			mu_uint32 ih = itemAttr->Height;

			for (mu_uint32 ey = 0; ey < ih; ++ey)
			{
				for (mu_uint32 ex = 0; ex < iw; ++ex)
				{
					mu_uint32 color = MU::Float4ToARGB((mu_float)GetLargeRand(6, 6) * 0.1f, (mu_float)GetLargeRand(4, 4) * 0.1f, 0.2f, 1.0f);
					mu_float rotate = (mu_float)(IWorldTime % 100) * 20.0f;
					mu_float scale = (mu_float)GetLargeRand(10, 5);

					mu_uint32 cx = ix + ex;
					mu_uint32 cy = iy + ey;
					mu_float efx = vx + cx * INVENTORY_SQUARE_WIDTH + (mu_float)GetLargeRand((mu_int32)INVENTORY_SQUARE_WIDTH);
					mu_float efy = vy + cy * INVENTORY_SQUARE_HEIGHT + (mu_float)GetLargeRand((mu_int32)INVENTORY_SQUARE_HEIGHT);

					UISprite **ShineEffect = _ShineEffect[cy * COLUMN_MIX_INVENTORY + cx];
					mu_float ns = scale;
					mu_float nfx = efx + INVENTORY_SQUARE_WIDTH * 0.5f - ns * 0.5f;
					mu_float nfy = efy + INVENTORY_SQUARE_WIDTH * 0.5f - ns * 0.5f;
					ShineEffect[0]->SetPosition(nfx, nfy);
					ShineEffect[0]->SetDimensions(scale, scale);
					ShineEffect[0]->SetColor(color);
					ShineEffect[1]->SetPosition(nfx, nfy);
					ShineEffect[1]->SetDimensions(scale, scale);
					ShineEffect[1]->SetRotation(rotate);
					ShineEffect[1]->SetColor(color);
					ns = (scale * 3.0f);
					nfx = efx + INVENTORY_SQUARE_WIDTH * 0.5f - ns * 0.5f;
					nfy = efy + INVENTORY_SQUARE_WIDTH * 0.5f - ns * 0.5f;
					ShineEffect[2]->SetPosition(nfx, nfy);
					ShineEffect[2]->SetDimensions(scale * 3.0f, scale * 3.0f);
					ShineEffect[2]->SetRotation(rotate);
					ShineEffect[2]->SetColor(color);
					ns = (scale * 6.0f);
					nfx = efx + INVENTORY_SQUARE_WIDTH * 0.5f - ns * 0.5f;
					nfy = efy + INVENTORY_SQUARE_WIDTH * 0.5f - ns * 0.5f;
					ShineEffect[3]->SetPosition(nfx, nfy);
					ShineEffect[3]->SetDimensions(scale * 6.0f, scale * 6.0f);
					ShineEffect[3]->SetColor(color);

					ShineEffect[0]->Draw();
					ShineEffect[1]->Draw();
					ShineEffect[2]->Draw();
					ShineEffect[3]->Draw();
				}
			}
		}
	}
}

void UIMixInventory::Draw3D()
{
	_InventoryControl->Draw3D();
}

void UIMixInventory::PostDraw()
{
	_InventoryControl->PostDraw();
	PostDrawButtons();
}

void UIMixInventory::DrawFrames()
{
	_BackFrame->Draw();
	_TopFrame->Draw();
	_BottomFrame->Draw();
	_LeftFrame->Draw();
	_RightFrame->Draw();
}

void UIMixInventory::DrawInventoryDetails()
{
	mu_text szText[256] = { 0, };
	mu_float fPos_x = this->_ViewX + 15.0f, fPos_y = this->_ViewY;
	mu_float fLine_y = 13.0f;

	MUTextSimpleRender *font = FONT_BOLD;
	mu_float fontSize = font->GetFont()->GetLineHeight();
	mu_uint32 textColor = MU::MakeARGB(220, 220, 220, 255);
	mu_uint32 backColor = MU::MakeARGB(0, 0, 0, 0);
	mu_uint32 textIndex = TEXTINDEX::BEGIN_MIXINVENTORY_ITEMNUMBER;

	switch (g_MixManager.GetMixType())
	{
	case MIXTYPE_GOBLIN_NORMAL:
		mu_sprintf_s(szText, mu_countof(szText), _T("") SPF_STRING _T(""), GlobalText[735]);
		break;
	case MIXTYPE_GOBLIN_CHAOSITEM:
		mu_sprintf_s(szText, mu_countof(szText), _T("") SPF_STRING _T(""), GlobalText[736]);
		break;
	case MIXTYPE_GOBLIN_ADD380:
		mu_sprintf_s(szText, mu_countof(szText), _T("") SPF_STRING _T(""), GlobalText[2193]);
		break;
	case MIXTYPE_CASTLE_SENIOR:
		fLine_y += 5.0f;
		mu_sprintf_s(szText, mu_countof(szText), _T("") SPF_STRING _T(""), GlobalText[1640]);
		break;
	case MIXTYPE_TRAINER:
		mu_sprintf_s(szText, mu_countof(szText), _T("") SPF_STRING _T(""), GlobalText[1205]);
		break;
	case MIXTYPE_OSBOURNE:
		fLine_y += 5.0f;
		mu_sprintf_s(szText, mu_countof(szText), _T("") SPF_STRING _T(""), GlobalText[2061]);
		break;
	case MIXTYPE_JERRIDON:
		fLine_y += 5.0f;
		mu_sprintf_s(szText, mu_countof(szText), _T("") SPF_STRING _T(""), GlobalText[2062]);
		break;
	case MIXTYPE_ELPIS:
		fLine_y += 5.0f;
		mu_sprintf_s(szText, mu_countof(szText), _T("") SPF_STRING _T(""), GlobalText[2063]);
		break;
	case MIXTYPE_CHAOS_CARD:
		fLine_y += 5.0f;
		mu_sprintf_s(szText, mu_countof(szText), _T("") SPF_STRING _T(""), GlobalText[2265]);
		break;
	case MIXTYPE_CHERRYBLOSSOM:
		fLine_y += 5.0f;
		mu_sprintf_s(szText, mu_countof(szText), _T("") SPF_STRING _T(""), GlobalText[2563]);
		break;
	case MIXTYPE_EXTRACT_SEED:
		fLine_y += 5.0f;
		mu_sprintf_s(szText, mu_countof(szText), _T("") SPF_STRING _T(""), GlobalText[2660]);
		break;
	case MIXTYPE_SEED_SPHERE:
		fLine_y += 5.0f;
		mu_sprintf_s(szText, mu_countof(szText), _T("") SPF_STRING _T(""), GlobalText[2661]);
		break;
	case MIXTYPE_ATTACH_SOCKET:
		fLine_y += 5.0f;
		mu_sprintf_s(szText, mu_countof(szText), _T("") SPF_STRING _T(""), GlobalText[2662]);
		break;
	case MIXTYPE_DETACH_SOCKET:
		fLine_y += 5.0f;
		mu_sprintf_s(szText, mu_countof(szText), _T("") SPF_STRING _T(""), GlobalText[2663]);
		break;
	default:
		fLine_y += 5.0f;
		mu_sprintf_s(szText, mu_countof(szText), _T("") SPF_STRING _T(""), GlobalText[583]);
		break;
	}
	font->DrawStringC2D(textIndex++, MU_TO_WINDOW(this->_ViewX + this->_ViewWidth * 0.5f, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
	
	fLine_y += 12;
	switch (g_MixManager.GetMixType())
	{
	case MIXTYPE_GOBLIN_NORMAL:
	case MIXTYPE_GOBLIN_CHAOSITEM:
	case MIXTYPE_GOBLIN_ADD380:
	case MIXTYPE_TRAINER:
		mu_sprintf_s(szText, mu_countof(szText), GlobalText[1623], g_ChaosTaxRate);
		font->DrawStringC2D(textIndex++, MU_TO_WINDOW(this->_ViewX + this->_ViewWidth * 0.5f, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
		break;
	default:
		fLine_y -= 5;
		break;
	}

	if (_MixState == MIX_FINISHED)
	{
		return;
	}

	fLine_y += 24;
	if (!g_MixManager.IsReadyToMix())
		textColor = MU::MakeARGB(255, 48, 48, 255);
	else
		textColor = MU::MakeARGB(255, 255, 48, 255);
	backColor = MU::MakeARGB(40, 40, 40, 128);

	g_MixManager.GetCurRecipeName(szText, mu_countof(szText), 1);
	font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
	fLine_y += 10;
	if (g_MixManager.GetCurRecipeName(szText, mu_countof(szText), 2))
		font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
	fLine_y += 10;

	switch (g_MixManager.GetMixType())
	{
	case MIXTYPE_GOBLIN_NORMAL:
	case MIXTYPE_GOBLIN_CHAOSITEM:
	case MIXTYPE_GOBLIN_ADD380:
	case MIXTYPE_OSBOURNE:
	case MIXTYPE_ELPIS:
	case MIXTYPE_TRAINER:
	case MIXTYPE_EXTRACT_SEED:
	case MIXTYPE_SEED_SPHERE:
		if (g_MixManager.IsReadyToMix() &&
			g_MixManager.GetPlusChaosRate() > 0 && g_MixManager.GetCurRecipe()->MixOption == 'F')
		{
			textColor = MU::MakeARGB(255, 255, 48, 255);
			backColor = MU::MakeARGB(40, 40, 40, 128);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[584], GlobalText[591], g_MixManager.GetSuccessRate());
			mu_sprintf_s(szText, mu_countof(szText), _T("") SPF_STRING _T(" + %d%%"), szText, g_MixManager.GetPlusChaosRate());
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
			textColor = MU::MakeARGB(210, 230, 255, 255);
		}
		else
		{
			textColor = MU::MakeARGB(210, 230, 255, 255);
			backColor = MU::MakeARGB(40, 40, 40, 128);
			switch (g_MixManager.GetMixType())
			{
			case MIXTYPE_GOBLIN_NORMAL:
			case MIXTYPE_GOBLIN_CHAOSITEM:
			case MIXTYPE_GOBLIN_ADD380:
			case MIXTYPE_EXTRACT_SEED:
			case MIXTYPE_SEED_SPHERE:
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[584], GlobalText[591], g_MixManager.GetSuccessRate());
				break;
			case MIXTYPE_TRAINER:
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[584], GlobalText[1212], g_MixManager.GetSuccessRate());
				break;
			case MIXTYPE_OSBOURNE:
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[584], GlobalText[2061], g_MixManager.GetSuccessRate());
				break;
			case MIXTYPE_ELPIS:
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[584], GlobalText[2063], g_MixManager.GetSuccessRate());
				break;
			}
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
		}
		fLine_y += 20;
		break;
	}

	switch (g_MixManager.GetMixType())
	{
	case MIXTYPE_GOBLIN_NORMAL:
	case MIXTYPE_GOBLIN_CHAOSITEM:
	case MIXTYPE_GOBLIN_ADD380:
	case MIXTYPE_CASTLE_SENIOR:
	case MIXTYPE_TRAINER:
	case MIXTYPE_JERRIDON:
	case MIXTYPE_EXTRACT_SEED:
	case MIXTYPE_SEED_SPHERE:
	case MIXTYPE_ATTACH_SOCKET:
	case MIXTYPE_DETACH_SOCKET:
		{
			mu_text szGoldText[32];
			mu_text szGoldText2[32];

			ConvertGold(g_MixManager.GetReqiredZen(), szGoldText, mu_countof(szGoldText));
			ConvertChaosTaxGold(g_MixManager.GetReqiredZen(), szGoldText2, mu_countof(szGoldText2));
			if (g_MixManager.IsReadyToMix() && g_MixManager.GetCurRecipe()->RequiredZenType == 'C')
			{
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[1636], szGoldText2, szGoldText);
			}
			else
			{
				mu_sprintf_s(szText, mu_countof(szText), GlobalText[1622], szGoldText2, szGoldText);
			}

			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
		}
		fLine_y += 20;
		break;
	}

	fLine_y = 203;
	mu_int32 iTextPos_y = 0;
	if (g_MixManager.GetMostSimilarRecipe() != nullptr)
	{
		textColor = MU::MakeARGB(220, 220, 220, 255);
		backColor = MU::MakeARGB(40, 40, 40, 128);

		mu_text szTempText[2][100];
		mu_int32 iTextLines = 0;
		if (!g_MixManager.IsReadyToMix() && g_MixManager.GetMostSimilarRecipeName(szTempText[0], mu_countof(szTempText[0]), 1) == true)
		{
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2334], szTempText[0]);

			iTextLines = CutStr(font, szText, mu_countof(szText), szTempText[0], mu_countof(szTempText[0]), 150, 2, 100);

			for (mu_int32 i = 0; i < iTextLines; ++i)
			{
				if (i >= 2)
					break;

				font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y + (iTextPos_y * fontSize), _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szTempText[i]);
				++iTextPos_y;
			}
		}

		mu_int32 iResult;
		for (mu_int32 iLine = 0; iLine < 8; ++iLine)
		{
			iResult = g_MixManager.GetSourceName(iLine, szText, mu_countof(szText));
			if (iResult == MIX_SOURCE_ERROR) break;
			else if (iResult == MIX_SOURCE_NO) textColor = MU::MakeARGB(255, 50, 20, 255);
			else if (iResult == MIX_SOURCE_PARTIALLY) textColor = MU::MakeARGB(210, 230, 255, 255);
			else if (iResult == MIX_SOURCE_YES) textColor = MU::MakeARGB(255, 255, 48, 255);

			iTextLines = CutStr(font, szText, mu_countof(szText), szTempText[0], mu_countof(szTempText[0]), 156, 2, 100);

			for (mu_int32 i = 0; i < iTextLines; ++i)
			{
				if (i >= 2)
					break;

				font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y + (iTextPos_y * fontSize), _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szTempText[i]);
				++iTextPos_y;
			}
		}
	}
	else if (g_MixManager.IsMixInit())
	{
		textColor = MU::MakeARGB(255, 50, 20, 255);
		backColor = MU::MakeARGB(40, 40, 40, 128);

		mu_sprintf_s(szText, mu_countof(szText), GlobalText[2346]);
		font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);

		++iTextPos_y;
	}
	else
	{
		textColor = MU::MakeARGB(255, 50, 20, 255);
		backColor = MU::MakeARGB(40, 40, 40, 128);

		mu_sprintf_s(szText, mu_countof(szText), GlobalText[2334], " ");
		font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);

		mu_sprintf_s(szText, mu_countof(szText), GlobalText[601]);
		font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y + (++iTextPos_y) * fontSize, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
	}

	++iTextPos_y;
	textColor = MU::MakeARGB(255, 50, 20, 255);
	backColor = MU::MakeARGB(40, 40, 40, 128);

	if (g_MixManager.IsReadyToMix())
	{
		if (g_MixManager.GetCurRecipeDesc(szText, mu_countof(szText), 1) == true)
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y + (++iTextPos_y) * fontSize, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
		if (g_MixManager.GetCurRecipeDesc(szText, mu_countof(szText), 2) == true)
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y + (++iTextPos_y) * fontSize, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
		if (g_MixManager.GetCurRecipeDesc(szText, mu_countof(szText), 3) == true)
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y + (++iTextPos_y) * fontSize, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
	}
	else if (g_MixManager.GetMostSimilarRecipe() != nullptr)
	{
		if (g_MixManager.GetRecipeAdvice(szText, mu_countof(szText), 1) == true)
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y + (++iTextPos_y) * fontSize, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
		if (g_MixManager.GetRecipeAdvice(szText, mu_countof(szText), 2) == true)
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y + (++iTextPos_y) * fontSize, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
		if (g_MixManager.GetRecipeAdvice(szText, mu_countof(szText), 3) == true)
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + fLine_y + (++iTextPos_y) * fontSize, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
	}

	switch (g_MixManager.GetMixType())
	{
	case MIXTYPE_GOBLIN_NORMAL:
	case MIXTYPE_GOBLIN_CHAOSITEM:
	case MIXTYPE_GOBLIN_ADD380:
		break;

	case MIXTYPE_CASTLE_SENIOR:
		{
			backColor = MU::MakeARGB(0, 0, 0, 0);
			textColor = MU::MakeARGB(200, 200, 200, 255);
			for (mu_int32 i = 0; i < 6; ++i)
				font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 270 + i * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, GlobalText[1644 + i]);
		}
		break;

	case MIXTYPE_TRAINER:
		break;

	case MIXTYPE_OSBOURNE:
		{
			backColor = MU::MakeARGB(0, 0, 0, 0);
			textColor = MU::MakeARGB(255, 255, 255, 255);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2220]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 0 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2221]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 1 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2084], GlobalText[2061], GlobalText[2082]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 2 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2098]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 3 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
			textColor = MU::MakeARGB(255, 0, 0, 255);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2222]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 4 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
		}
		break;

	case MIXTYPE_JERRIDON:
		{
			backColor = MU::MakeARGB(0, 0, 0, 0);
			textColor = MU::MakeARGB(255, 255, 255, 255);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2102]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 0 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2103]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 1 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2104]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 2 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2088]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 3 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2100]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 4 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2101]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 5 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
		}
		break;

	case MIXTYPE_ELPIS:
		backColor = MU::MakeARGB(0, 0, 0, 0);
		textColor = MU::MakeARGB(255, 255, 255, 255);
		mu_sprintf_s(szText, mu_countof(szText), GlobalText[2071]);
		font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 0 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
		mu_sprintf_s(szText, mu_countof(szText), GlobalText[2095]);
		font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 1 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
		mu_sprintf_s(szText, mu_countof(szText), GlobalText[2096]);
		font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 2 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
		break;

	case MIXTYPE_CHAOS_CARD:
		{
			backColor = MU::MakeARGB(0, 0, 0, 0);

			textColor = MU::MakeARGB(255, 40, 20, 255);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2223]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 4 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);

			textColor = MU::MakeARGB(255, 255, 255, 255);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2262]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 6 * fontSize, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);

			textColor = MU::MakeARGB(255, 255, 255, 255);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2306]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 7 * fontSize, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);

			textColor = MU::MakeARGB(255, 255, 255, 255);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2261]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 8 * fontSize, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
		}
		break;

	case MIXTYPE_CHERRYBLOSSOM:
		{
			backColor = MU::MakeARGB(0, 0, 0, 0);

			textColor = MU::MakeARGB(255, 40, 20, 255);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2223]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 0 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);

			textColor = MU::MakeARGB(255, 255, 255, 255);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2565]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 2 * fontSize, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);

			textColor = MU::MakeARGB(255, 255, 255, 255);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2540]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 3 * fontSize, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);

			textColor = MU::MakeARGB(255, 255, 255, 255);
			mu_sprintf_s(szText, mu_countof(szText), GlobalText[2306]);
			font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 250 + 4 * fontSize, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textColor, backColor, szText);
		}
		break;

	case MIXTYPE_ATTACH_SOCKET:
		backColor = MU::MakeARGB(0, 0, 0, 0);
		textColor = MU::MakeARGB(200, 200, 200, 255);
		mu_sprintf_s(szText, mu_countof(szText), GlobalText[2674]);
		font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 280 + 0 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);

#if 0 // FIX
		m_SocketListBox.Render();
#endif
		break;

	case MIXTYPE_DETACH_SOCKET:
		backColor = MU::MakeARGB(0, 0, 0, 0);
		textColor = MU::MakeARGB(200, 200, 200, 255);
		mu_sprintf_s(szText, mu_countof(szText), GlobalText[2675]);
		font->DrawStringC2D(textIndex++, MU_TO_WINDOW(fPos_x, _AspectWidth), MU_TO_WINDOW(fPos_y + 280 + 0 * fontSize, _AspectHeight), 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textColor, backColor, szText);

#if 0 // FIX
		m_SocketListBox.Render();
#endif
		break;
	}
}

void UIMixInventory::DrawButtons()
{
	_MixButton->Draw();
}

void UIMixInventory::PostDrawButtons()
{
	_MixButton->PostDraw();
}

UIBase *UIMixInventory::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _InventoryControl->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _XButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _MixButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return focus;
}

mu_boolean UIMixInventory::InsertItem(mu_int32 ColumnX, mu_int32 RowY, SHARED::ITEM_DATA &Item)
{
	ITEM *NewItem = GenerateItem();

	ParseItem(Item, NewItem);

	if (_InventoryControl->AddItem(ColumnX, RowY, NewItem) == true)
		return true;

	DestroyItem(NewItem);

	return false;
}

void UIMixInventory::DeleteItem(mu_int32 ColumnX, mu_int32 RowY)
{
	ITEM* pItem = _InventoryControl->FindItem(ColumnX, RowY);
	if (pItem != nullptr)
	{
		_InventoryControl->RemoveItem(pItem);
	}
}

void UIMixInventory::DeleteAllItems()
{
	_InventoryControl->RemoveAllItems();
}

ITEM *UIMixInventory::FindItem(mu_int32 iLinealPos)
{
	return _InventoryControl->FindItem(iLinealPos);
}

ITEM *UIMixInventory::FindItem(mu_int32 ColumnX, mu_int32 RowY)
{
	return _InventoryControl->FindItem(ColumnX, RowY);
}

ITEM *UIMixInventory::FindItemByKey(mu_uint32 dwKey)
{
	return _InventoryControl->FindItemByKey(dwKey);
}

mu_int32 UIMixInventory::FindEmptySlot(ITEM* pItem)
{
	ITEM_ATTRIBUTE *pItemAttr = &ItemAttribute[pItem->Type];

	return _InventoryControl->FindEmptySlot(pItemAttr->Width, pItemAttr->Height);
}

void UIMixInventory::SetMixState(mu_uint8 mixState)
{
	_MixState = mixState;

	if (g_pPickedItem.GetItem() != nullptr)
	{
		g_pPickedItem.BackupPickedItem();
	}

	if (mixState == MIX_PROCESSING)
	{
		_MixEffectTimer = g_CurrentTime + 2400;
	}
	else if (mixState == MIX_PROCESSING)
	{
		_MixEffectTimer = g_CurrentTime + 1000;
	}

	if (mixState == MIX_PROCESSING)
	{
		_InventoryControl->Lock();
		g_pMyInventory->GetInventoryControl()->Lock();
		_MixButton->ChangeState(UIButton::UI_DISABLED);
	}
	else
	{
		_InventoryControl->Unlock();
		g_pMyInventory->GetInventoryControl()->Unlock();
		_MixButton->ChangeState(UIButton::UI_NORMAL);
	}
}