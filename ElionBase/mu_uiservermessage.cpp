#include "stdafx.h"
#include "mu_uiservermessage.h"

#define SCREEN_WIDTH 1024.0f
#define SCREEN_HEIGHT 768.0f

#define UISERVERMESSAGE_POSX (50.0f)
#define UISERVERMESSAGE_POSY (50.0f)
#define UISERVERMESSAGE_EXWIDTH (256.0f)
#define UISERVERMESSAGE_HEIGHT(exheight) (4.0f * 2.0f + exheight)
#define UISERVERMESSAGE_EXHEIGHT (15.0f)

#define MESSAGE_EXHEIGHT (3.0f)

#define BACK_POSX _ViewX
#define BACK_POSY _ViewY
#define BACK_WIDTH _ViewWidth
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND EVector2(1.0f, 1.0f)

#define TOP_WIDTH _ViewWidth
#define TOP_HEIGHT 4.0f
#define TOP_POSX _ViewX
#define TOP_POSY _ViewY

#define LEFT_WIDTH ((_ViewWidth / 512.0f) * 3.0f)
#define LEFT_POSX _ViewX
#define LEFT_POSY (TOP_POSY + TOP_HEIGHT)

#define RIGHT_WIDTH ((_ViewWidth / 512.0f) * 3.0f)
#define RIGHT_POSX (_ViewX + _ViewWidth - RIGHT_WIDTH)
#define RIGHT_POSY (TOP_POSY + TOP_HEIGHT)

#define BOTTOM_WIDTH _ViewWidth
#define BOTTOM_HEIGHT 4.0f
#define BOTTOM_POSX _ViewX
#define BOTTOM_POSY(exheight) (LEFT_POSY + exheight)

UIServerMessage::UIServerMessage(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(UISERVERMESSAGE_POSX, UISERVERMESSAGE_POSY);
	UIBase::SetDimensions(UISERVERMESSAGE_EXWIDTH, UISERVERMESSAGE_HEIGHT(UISERVERMESSAGE_EXHEIGHT));

	// Initialize
	{
		_Background = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX03], SCREEN_WIDTH, SCREEN_HEIGHT, BACK_POSX, BACK_POSY, BACK_WIDTH, UISERVERMESSAGE_HEIGHT(UISERVERMESSAGE_EXHEIGHT), BACK_UVBEGIN, BACK_UVEND);
		_Background->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_TopFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], SCREEN_WIDTH, SCREEN_HEIGHT, TOP_POSX, TOP_POSY, TOP_WIDTH, TOP_HEIGHT, EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f));
		_TopFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BottomFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], SCREEN_WIDTH, SCREEN_HEIGHT, BOTTOM_POSX, BOTTOM_POSY(UISERVERMESSAGE_EXHEIGHT), BOTTOM_WIDTH, BOTTOM_HEIGHT, EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f));
		_BottomFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_LeftFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX02], SCREEN_WIDTH, SCREEN_HEIGHT, LEFT_POSX, LEFT_POSY, LEFT_WIDTH, UISERVERMESSAGE_EXHEIGHT, EVector2(0.0f, 0.0f), EVector2(0.5f, 1.0f));
		_LeftFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_RightFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX02], SCREEN_WIDTH, SCREEN_HEIGHT, RIGHT_POSX, RIGHT_POSY, RIGHT_WIDTH, UISERVERMESSAGE_EXHEIGHT, EVector2(0.5f, 0.0f), EVector2(1.0f, 1.0f));
		_RightFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_UpdateFrame = true;
	}
}

UIServerMessage::~UIServerMessage()
{

}

void UIServerMessage::ChangePosition(mu_float x, mu_float y)
{
#define SETPOSITION_HELPER(var) var->SetPosition(var->_ViewX - oldX + x, var->_ViewY - oldY + y)

	mu_float oldX = _ViewX, oldY = _ViewY;

	SETPOSITION_HELPER(this);

	SETPOSITION_HELPER(_Background);
	SETPOSITION_HELPER(_TopFrame);
	SETPOSITION_HELPER(_BottomFrame);
	SETPOSITION_HELPER(_LeftFrame);
	SETPOSITION_HELPER(_RightFrame);

#undef SETPOSITION_HELPER
}

void UIServerMessage::Update(mu_uint64 CurrentTime)
{
	if (_UpdateFrame == true)
	{
		mu_float exWidth = UISERVERMESSAGE_EXWIDTH;
		mu_float exHeight = MESSAGE_EXHEIGHT;

		for (auto it = _Messages.begin(); it != _Messages.end(); ++it)
		{
			UIServerMessageData &data = *it;

			mu_float newWidth = data.Width;
			if (newWidth > exWidth)
			{
				exWidth = newWidth;
			}

			exHeight += data.Height;
		}

		UIBase::SetDimensions(exWidth + LEFT_WIDTH + RIGHT_WIDTH + 1.0f, UISERVERMESSAGE_HEIGHT(exHeight));

		_Background->SetDimensions(BACK_WIDTH, _ViewHeight);
		_TopFrame->SetDimensions(TOP_WIDTH, TOP_HEIGHT);
		_BottomFrame->SetDimensions(BOTTOM_WIDTH, BOTTOM_HEIGHT);
		_BottomFrame->SetPosition(_BottomFrame->_ViewX, _BottomFrame->_ViewY - _LeftFrame->_ViewHeight + exHeight);
		_LeftFrame->SetDimensions(LEFT_WIDTH, exHeight);
		_RightFrame->SetPosition(RIGHT_POSX, RIGHT_POSY);
		_RightFrame->SetDimensions(RIGHT_WIDTH, exHeight);
	}
}

void UIServerMessage::Draw()
{
	if (_Messages.empty() == false)
	{
		_Background->Draw();
		_LeftFrame->Draw();
		_RightFrame->Draw();
		_TopFrame->Draw();
		_BottomFrame->Draw();

		mu_uint32 backColor = MU::MakeARGB(0, 0, 0, 0);
		mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255);

		mu_float posx = MU_TO_WINDOW(_LeftFrame->_ViewX + _LeftFrame->_ViewWidth + 0.5f, _AspectWidth);
		mu_float posy = _LeftFrame->_ViewY + MESSAGE_EXHEIGHT * 0.5f;

		mu_uint32 n = 0;
		for (auto it = _Messages.begin(); it != _Messages.end(); ++it, ++n)
		{
			UIServerMessageData &data = *it;

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::BEGIN_SERVERMESSAGES + n,
				posx,
				MU_TO_WINDOW(posy, _AspectHeight),
				1.0f,
				ETA_LEFT,
				ETA_BOTTOM,
				0.0f,
				textColor,
				backColor,
				data.Message.c_str());

			posy += data.Height;
		}
	}
}

void UIServerMessage::Clear()
{
	_Messages.clear();
}

void UIServerMessage::AddText(const mu_string &message)
{
	UIServerMessageData messageData;

	messageData.Message = message;

	EVector2 messageSize(0.0f, 0.0f);
	FONT_NORMAL->GetTextExtentPoint(message.c_str(), message.size(), ETA_LEFT, 1.0f, &messageSize);
	messageData.Width = MU_TO_VIEWPORT(messageSize.x, _AspectWidth);
	messageData.Height = MU_TO_VIEWPORT(messageSize.y, _AspectHeight);

	_Messages.push_back(messageData);

	if (_Messages.size() > MAX_SERVERMESSAGES)
	{
		_Messages.erase(_Messages.begin());
	}

	_UpdateFrame = true;
}