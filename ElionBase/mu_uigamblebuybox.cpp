#include "stdafx.h"
#include "mu_uigamblebuybox.h"

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 960.0f

#define BASE_WIDTH 230.0f
#define BASE_HEIGHT 132.0f

#define BOX_WIDTH 450.0f
#define BOX_HEIGHT (BOX_WIDTH / (BASE_WIDTH / BASE_HEIGHT))
#define BOX_POSX (SCREEN_WIDTH * 0.5f - BOX_WIDTH * 0.5f)
#define BOX_POSY (SCREEN_HEIGHT * 0.5f - BOX_HEIGHT * 0.5f)

#define BACKTEXTURE_WIDTH 225.0f
#define BACKTEXTURE_HEIGHT 512.0f

#define BACK_POSX (BOX_POSX + BOX_WIDTH * (2.5f / BASE_WIDTH))
#define BACK_POSY (BOX_POSY + BOX_HEIGHT * (4.0f / BASE_HEIGHT))
#define BACK_WIDTH (BOX_WIDTH - BOX_WIDTH * (10.0f / BASE_WIDTH))
#define BACK_HEIGHT (BOX_HEIGHT - BOX_HEIGHT * (12.0f / BASE_HEIGHT))
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND EVector2(1.0f, BACK_HEIGHT / BACKTEXTURE_HEIGHT)

#define TOP_WIDTH BOX_WIDTH
#define TOP_HEIGHT (BOX_WIDTH * (67.0f / BASE_WIDTH))
#define BOTTOM_WIDTH BOX_WIDTH
#define BOTTOM_HEIGHT (BOX_WIDTH * (50.0f / BASE_WIDTH))
#define MIDDLE_WIDTH BOX_WIDTH
#define MIDDLE_HEIGHT (BOX_HEIGHT - TOP_HEIGHT - BOTTOM_HEIGHT)

#define TOP_POSX BOX_POSX
#define TOP_POSY BOX_POSY
#define MIDDLE_POSX BOX_POSX
#define MIDDLE_POSY (TOP_POSY + TOP_HEIGHT)
#define BOTTOM_POSX BOX_POSX
#define BOTTOM_POSY (MIDDLE_POSY + MIDDLE_HEIGHT)

#define BUTTON_WIDTH 54.0f
#define BUTTON_HEIGHT 30.0f

#define ACCEPT_POSX (BOX_POSX + 70.0f)
#define ACCEPT_POSY (BOX_POSY + BOX_HEIGHT - BUTTON_HEIGHT * 2.25f)
#define CANCEL_POSX (BOX_POSX + BOX_WIDTH - 70.0f - BUTTON_WIDTH)
#define CANCEL_POSY (BOX_POSY + BOX_HEIGHT - BUTTON_HEIGHT * 2.25f)

#define ITEM_WIDTH 200.0f
#define ITEM_HEIGHT 80.0f
#define ITEM_POSX (BOX_POSX + ITEM_WIDTH * 0.5f)
#define ITEM_POSY (BOX_POSY + (BOX_HEIGHT * (MSGBOX_TEXT_TOP_BLANK / BASE_HEIGHT)) + ITEM_HEIGHT * 0.5f)
#define TEXT_POSX (BOX_POSX + ITEM_WIDTH - 20.0f)
#define TEXT_POSY (BOX_POSY + (BOX_HEIGHT * (MSGBOX_TEXT_TOP_BLANK / BASE_HEIGHT)))

UIGambleBuyBox::UIGambleBuyBox(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(BOX_POSX, BOX_POSY);
	UIBase::SetDimensions(BOX_WIDTH, BOX_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_NO_FOCUS | GFX_FLAG::GFX_HIDDEN);

	// Initialize
	{
		_Back = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_BACK], SCREEN_WIDTH, SCREEN_HEIGHT, BACK_POSX, BACK_POSY, BACK_WIDTH, BACK_HEIGHT, BACK_UVBEGIN, BACK_UVEND);
		_Top = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_TOP], SCREEN_WIDTH, SCREEN_HEIGHT, TOP_POSX, TOP_POSY, TOP_WIDTH, TOP_HEIGHT);
		_Middle = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_MIDDLE], SCREEN_WIDTH, SCREEN_HEIGHT, MIDDLE_POSX, MIDDLE_POSY, MIDDLE_WIDTH, MIDDLE_HEIGHT);
		_Bottom = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_BOTTOM], SCREEN_WIDTH, SCREEN_HEIGHT, BOTTOM_POSX, BOTTOM_POSY, BOTTOM_WIDTH, BOTTOM_HEIGHT);

		_Top->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_Middle->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_Bottom->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_AcceptButton = new UIButton(MESSAGEBOX::MESSAGE_GAMBLEBUY_ACCEPTBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ACCEPT_POSX, ACCEPT_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_AcceptButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AcceptButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_AcceptButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_AcceptButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_AcceptButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));

		_CancelButton = new UIButton(MESSAGEBOX::MESSAGE_GAMBLEBUY_CANCELBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CANCEL_POSX, CANCEL_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_CancelButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CancelButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_CancelButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
	}
}

UIGambleBuyBox::~UIGambleBuyBox()
{

}

void UIGambleBuyBox::Update(mu_uint64 CurrentTime)
{

}

void UIGambleBuyBox::Draw3D()
{
	_SlotCount = 0;

	ITEM *PickedItem = g_pPickedItem.GetItem();
	if (PickedItem != nullptr)
	{
		MUThreadInfo &ThreadInfo = g_ThreadInfo[0];
		ThreadInfo.GUIRenderIndex++;
		ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);

		_SlotIndex = g_GUIRenderSlotCount;

		mu_float aspectWidth = SCREEN_WIDTH / 640.0f;
		mu_float aspectHeight = SCREEN_HEIGHT / 480.0f;

		ITEM_ATTRIBUTE* itemAttr = &ItemAttribute[PickedItem->Type];
		mu_float width = itemAttr->Width * INVENTORY_SQUARE_WIDTH;
		mu_float height = itemAttr->Height * INVENTORY_SQUARE_HEIGHT;
		mu_float x = MU_TO_VIEWPORT(ITEM_POSX, aspectWidth) - width * 0.5f;
		mu_float y = MU_TO_VIEWPORT(ITEM_POSY, aspectHeight) - height * 0.5f;

		RenderItem3D(x, y, width, height, PickedItem->Type, PickedItem->Level, PickedItem->ExcOption, PickedItem->SetOption, true);

		_SlotCount = g_GUIRenderSlotCount - _SlotIndex;
	}
}

void UIGambleBuyBox::PostDraw()
{
	DrawFrame();
	DrawText();
	DrawButton();

	MUThreadInfo &ThreadInfo = g_ThreadInfo[0];

	if (_SlotCount > 0)
	{
		ThreadInfo.EnableScissor = false;
		g_Tasking.RenderGUIModelsAtRuntime(_SlotIndex, _SlotCount);
	}
}

void UIGambleBuyBox::DrawFrame()
{
	_Back->Draw();
	_Top->Draw();
	_Middle->Draw();
	_Bottom->Draw();
}

void UIGambleBuyBox::DrawText()
{
	FONT_BOLD->DrawStringC2D(TEXTINDEX::GAMBLEBUY_LINE1,
		MU_TO_WINDOW(TEXT_POSX, _AspectWidth),
		MU_TO_WINDOW(TEXT_POSY, _AspectHeight),
		1.0f,
		ETA_LEFT,
		ETA_BOTTOM,
		5.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(0, 0, 0, 0),
		GlobalText[1612]);
}

void UIGambleBuyBox::DrawButton()
{
	_AcceptButton->Draw();
	_CancelButton->Draw();
	_AcceptButton->PostDraw();
	_CancelButton->PostDraw();
}

mu_uint32 UIGambleBuyBox::OnPreKeyPressed(const SDL_Keysym &key)
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

UIBase *UIGambleBuyBox::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _AcceptButton->GetFocus(mouseX, mouseY);

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