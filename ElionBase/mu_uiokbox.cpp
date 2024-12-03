#include "stdafx.h"
#include "mu_uiokbox.h"

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 960.0f

#define BASE_WIDTH 230.0f
#define BASE_HEIGHT 132.0f

#define BOX_WIDTH 360.0f
#define BOX_HEIGHT (BOX_WIDTH / (BASE_WIDTH / BASE_HEIGHT))
#define BOX_POSX ((SCREEN_WIDTH - BOX_WIDTH) * 0.5f)
#define BOX_POSY ((SCREEN_HEIGHT - BOX_HEIGHT) * 0.5f)

#define BACKTEXTURE_WIDTH 225.0f
#define BACKTEXTURE_HEIGHT 512.0f

#define BACK_POSX(x) (x + BOX_WIDTH * (2.5f / BASE_WIDTH))
#define BACK_POSY(y, h) (y + h * (4.0f / BASE_HEIGHT))
#define BACK_WIDTH (BOX_WIDTH - BOX_WIDTH * (10.0f / BASE_WIDTH))
#define BACK_HEIGHT(h) (h - h * (12.0f / BASE_HEIGHT))
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND(h) EVector2(1.0f, BACK_HEIGHT(h) / BACKTEXTURE_HEIGHT)

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

#define OK_POSX (BOX_POSX + (BOX_WIDTH - BUTTON_WIDTH) * 0.5f)
#define OK_POSY(y, h) (y + h - BUTTON_HEIGHT * 2.25f)

#define MAX_TEXTWIDTH (BOX_WIDTH * (180.0f / BASE_WIDTH))

#define TOP_TEXT 0.65f

UIOkBox::UIOkBox(UISceneManager *manager) : UIBase()
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
		_Back = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_BACK], SCREEN_WIDTH, SCREEN_HEIGHT, BACK_POSX(BOX_POSX), BACK_POSY(BOX_POSY, BOX_HEIGHT), BACK_WIDTH, BACK_HEIGHT(BOX_HEIGHT), BACK_UVBEGIN, BACK_UVEND(BOX_HEIGHT));
		_Top = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_TOP], SCREEN_WIDTH, SCREEN_HEIGHT, TOP_POSX, TOP_POSY, TOP_WIDTH, TOP_HEIGHT);
		_Middle = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_MIDDLE], SCREEN_WIDTH, SCREEN_HEIGHT, MIDDLE_POSX, MIDDLE_POSY, MIDDLE_WIDTH, MIDDLE_HEIGHT);
		_Bottom = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_BOTTOM], SCREEN_WIDTH, SCREEN_HEIGHT, BOTTOM_POSX, BOTTOM_POSY, BOTTOM_WIDTH, BOTTOM_HEIGHT);

		_Top->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_Middle->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_Bottom->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_OkButton = new UIButton(MESSAGEBOX::MESSAGE_OKBOX_OKBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, OK_POSX, OK_POSY(BOX_POSY, BOX_HEIGHT), BUTTON_WIDTH, BUTTON_HEIGHT);
		_OkButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_OkButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_OkButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));

		_TextHeight = MIDDLE_HEIGHT;
		Configure();
	}
}

UIOkBox::~UIOkBox()
{
}

void UIOkBox::Configure()
{
	mu_float middleHeight = _TextHeight - 10.0f;

	if (middleHeight < 0.0f)
	{
		middleHeight = 0.0f;
	}

	mu_float Height = middleHeight + TOP_HEIGHT + BOTTOM_HEIGHT;
	mu_float posx = BOX_POSX, posy = ((SCREEN_HEIGHT - Height) * 0.5f);

	_Middle->SetDimensions(BOX_WIDTH, middleHeight);

	_Back->SetDimensions(BACK_WIDTH, BACK_HEIGHT(Height));
	_Back->SetUV(BACK_UVBEGIN, BACK_UVEND(Height));

	_Back->SetPosition(BACK_POSX(posx), BACK_POSY(posy, Height));
	_Top->SetPosition(posx, posy);
	_Middle->SetPosition(posx, posy + TOP_HEIGHT);
	_Bottom->SetPosition(posx, posy + middleHeight + TOP_HEIGHT);

	_OkButton->SetPosition(OK_POSX, OK_POSY(posy, Height));
}

void UIOkBox::Update(mu_uint64 CurrentTime)
{

}

void UIOkBox::PostDraw()
{
	DrawFrame();
	DrawText();
	DrawButton();
}

void UIOkBox::DrawFrame()
{
	_Back->Draw();
	_Top->Draw();
	_Middle->Draw();
	_Bottom->Draw();
}

void UIOkBox::DrawText()
{
	mu_float messageX = MU_TO_WINDOW(_ViewX + (BOX_WIDTH * 0.5f) - BOX_WIDTH * 0.02f, _AspectWidth);
	mu_float messageY = _Middle->_NativeY - MU_TO_WINDOW(TOP_HEIGHT * TOP_TEXT, _AspectHeight);
	mu_int32 index = 0;
	for (auto it = _Messages.begin(); it != _Messages.end(); ++it, ++index)
	{
		MESSAGE_DATA &data = *it;

		MUText *buffer = nullptr;
		data.font->DrawStringC2D(TEXTINDEX::OKBOX_BEGIN + index,
			messageX,
			messageY,
			1.0f,
			ETA_CENTER,
			ETA_BOTTOM,
			5.0f,
			data.color,
			MU::MakeARGB(0, 0, 0, 0),
			data.message.c_str(), 0.0f, 0.0f, &buffer);

		if (buffer != nullptr)
		{
			messageY += buffer->GetSize().y;
		}
	}
}

void UIOkBox::DrawButton()
{
	_OkButton->Draw();
	_OkButton->PostDraw();
}

mu_uint32 UIOkBox::OnPreKeyPressed(const SDL_Keysym &key)
{
	switch (key.scancode)
	{
	case SDL_SCANCODE_RETURN:
	case SDL_SCANCODE_KP_ENTER:
		{
			_SceneManager->OnGFxMessagePush(_OkButton->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
		}
		return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

UIBase *UIOkBox::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _OkButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return focus;
}

void UIOkBox::ClearMessages()
{
	_TextHeight = 0.0f;
	_Messages.clear();
}

void UIOkBox::AddMessage(MUTextSimpleRender *font, const mu_text *message, mu_uint32 color)
{
	mu_float totalHeight = SeparateText(font, message, color);

	_TextHeight += MU_TO_VIEWPORT(totalHeight, _AspectHeight);
}

mu_float UIOkBox::SeparateText(MUTextSimpleRender *font, const mu_text *message, mu_uint32 color)
{
	mu_size messageLength = mu_strlen(message);

	EVector2 TextSize;
	font->GetTextExtentPoint(message, messageLength, ETA_LEFT, 1.0f, &TextSize);
	mu_size TextExtentWidth = MU_TO_VIEWPORT(TextSize.x, _AspectWidth);

	if (TextExtentWidth <= MAX_TEXTWIDTH)
	{
		MESSAGE_DATA MessageData;

		MessageData.message = message;
		MessageData.color = color;
		MessageData.font = font;

		_Messages.push_back(MessageData);
		return TextSize.y;
	}

	mu_string cutText;
	mu_string remainText = message;

	mu_float totalHeight = 0.0f;

	for (mu_boolean loop = true; loop == true;)
	{
		mu_size prev_offset = 0;
		for (mu_size cur_offset = 0; cur_offset < remainText.size();)
		{
			prev_offset = cur_offset++;

			mu_string tmpString(remainText, 0, cur_offset);

			font->GetTextExtentPoint(tmpString.c_str(), tmpString.size(), ETA_LEFT, 1.0f, &TextSize);
			TextExtentWidth = MU_TO_VIEWPORT(TextSize.x, _AspectWidth);

			if (TextExtentWidth > MAX_TEXTWIDTH && cur_offset != 0)
			{
				cutText.assign(remainText, 0, prev_offset);
				remainText.assign(remainText, prev_offset, remainText.size() - prev_offset);

				MESSAGE_DATA MessageData;

				MessageData.message = cutText;
				MessageData.color = color;
				MessageData.font = font;

				_Messages.push_back(MessageData);
				totalHeight += TextSize.y;

				font->GetTextExtentPoint(remainText.c_str(), remainText.size(), ETA_LEFT, 1.0f, &TextSize);
				TextExtentWidth = MU_TO_VIEWPORT(TextSize.x, _AspectWidth);

				if (TextExtentWidth <= MAX_TEXTWIDTH)
				{
					MESSAGE_DATA MessageData;

					MessageData.message = remainText;
					MessageData.color = color;
					MessageData.font = font;

					_Messages.push_back(MessageData);
					totalHeight += TextSize.y;

					loop = false;
					break;
				}

				break;
			}
		}
	}

	return totalHeight;
}