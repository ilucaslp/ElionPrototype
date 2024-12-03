#include "stdafx.h"
#include "mu_uihighvaluewarningbox.h"
#include "mu_uihighvaluewarningbox_defines.h"

UIHighValueWarningBox::UIHighValueWarningBox(UISceneManager *manager) : UIBase()
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
		
		_AcceptButton = new UIButton(MESSAGEBOX::MESSAGE_HIGHVALUE_ACCEPTBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ACCEPT_POSX, ACCEPT_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_AcceptButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AcceptButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_AcceptButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_AcceptButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_AcceptButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));

		_CancelButton = new UIButton(MESSAGEBOX::MESSAGE_HIGHVALUE_CANCELBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CANCEL_POSX, CANCEL_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_CancelButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CancelButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_CancelButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
	}
}

UIHighValueWarningBox::~UIHighValueWarningBox()
{

}

void UIHighValueWarningBox::Update(mu_uint64 CurrentTime)
{

}

void UIHighValueWarningBox::Draw3D()
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

void UIHighValueWarningBox::PostDraw()
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

void UIHighValueWarningBox::DrawFrame()
{
	TMSGBOX_RENDERBACK;
	TMSGBOX_RENDERFRONT;
}

void UIHighValueWarningBox::DrawText()
{
	mu_float lineHeight = FONT_BOLD->GetFont()->GetLineHeight() + 8.0f;

	FONT_BOLD->DrawStringC2D(TEXTINDEX::HIGHVALUE_LINE1,
		MU_TO_WINDOW(TEXT_POSX, _AspectWidth),
		MU_TO_WINDOW(TEXT_POSY, _AspectHeight),
		1.0f,
		ETA_LEFT,
		ETA_BOTTOM,
		5.0f,
		MU::MakeARGB(255, 0, 0, 255),
		MU::MakeARGB(0, 0, 0, 0),
		GlobalText[536]);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::HIGHVALUE_LINE1,
		MU_TO_WINDOW(TEXT_POSX, _AspectWidth),
		MU_TO_WINDOW(TEXT_POSY + lineHeight, _AspectHeight),
		1.0f,
		ETA_LEFT,
		ETA_BOTTOM,
		5.0f,
		MU::MakeARGB(255, 178, 0, 255),
		MU::MakeARGB(0, 0, 0, 0),
		GlobalText[537]);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::HIGHVALUE_LINE1,
		MU_TO_WINDOW(TEXT_POSX, _AspectWidth),
		MU_TO_WINDOW(TEXT_POSY + lineHeight * 2.0f, _AspectHeight),
		1.0f,
		ETA_LEFT,
		ETA_BOTTOM,
		5.0f,
		MU::MakeARGB(255, 178, 0, 255),
		MU::MakeARGB(0, 0, 0, 0),
		GlobalText[538]);
}

void UIHighValueWarningBox::DrawButton()
{
	_AcceptButton->Draw();
	_CancelButton->Draw();
	_AcceptButton->PostDraw();
	_CancelButton->PostDraw();
}

mu_uint32 UIHighValueWarningBox::OnPreKeyPressed(const SDL_Keysym &key)
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

UIBase *UIHighValueWarningBox::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
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