#include "stdafx.h"
#include "mu_uicreatecharacter.h"

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 960.0f

#define BASE_WIDTH 454.0f
#define BASE_HEIGHT 406.0f
#define BASE_POSX ((SCREEN_WIDTH - BASE_WIDTH) * 0.5f)
#define BASE_POSY ((SCREEN_HEIGHT - BASE_HEIGHT) * 0.5f)

#define INPUTBACK_POSX BASE_POSX
#define INPUTBACK_POSY (BASE_POSY + 317.0f)
#define INPUTBACK_WIDTH 346.0f
#define INPUTBACK_HEIGHT 38.0f

#define INPUT_POSX (INPUTBACK_POSX + 72.0f)
#define INPUT_POSY (INPUTBACK_POSY + 19.0f)
#define INPUT_WIDTH 188.0f
#define INPUT_HEIGHT 20.0f

#define STAT_POSX (BASE_POSX + 346.0f)
#define STAT_POSY (BASE_POSY + 24.0f)
#define STAT_WIDTH 108.0f
#define STAT_HEIGHT 80.0f

#define DESC_POSX BASE_POSX
#define DESC_POSY (BASE_POSY + 355.0f)
#define DESC_WIDTH BASE_WIDTH
#define DESC_HEIGHT 51.0f

#define OK_POSX (BASE_POSX + 346.0f)
#define OK_POSY (BASE_POSY + 325.0f)
#define OK_WIDTH 54.0f
#define OK_HEIGHT 30.0f

#define CANCEL_POSX (BASE_POSX + 400.0f)
#define CANCEL_POSY (BASE_POSY + 325.0f)
#define CANCEL_WIDTH 54.0f
#define CANCEL_HEIGHT 30.0f

#define CLASS_POSX (BASE_POSX + 346.0f)
#define CLASS_POSY (BASE_POSY + 131.0f)
#define CLASSEX_POSY (BASE_POSY + 246.0f)
#define CLASS_WIDTH 108.0f
#define CLASS_HEIGHT 26.0f

#define MODEL_POSX BASE_POSX
#define MODEL_POSY BASE_POSY
#define MODEL_WIDTH 410.0f
#define MODEL_HEIGHT 335.0f

#define STATTEXT_POSX (STAT_POSX + 1.0f)
#define STATTEXT_POSX2 (STAT_POSX + STAT_WIDTH - 1.0f)
#define STATTEXT_POSY (STAT_POSY + 10.0f)

UICreateCharacter::UICreateCharacter(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(0.0f, 0.0f);
	UIBase::SetDimensions(0.0f, 0.0f);

	SetGfxFlag(GFX_FLAG::GFX_HIDDEN);

	// Initialize
	{
		_Background = new UISpriteColor(manager, this, MU::MakeARGB(0, 0, 0, 128), 640.0f, 480.0f, 0.0f, 0.0f, 640.0f, 480.0f);
		_Background->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_InputFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::CHARACTER_CREATE_NAMEINPUT], SCREEN_WIDTH, SCREEN_HEIGHT, INPUTBACK_POSX, INPUTBACK_POSY, INPUTBACK_WIDTH, INPUTBACK_HEIGHT);
		_InputFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_StatBack = new UISpriteColor(manager, this, MU::MakeARGB(0, 0, 0, 143), SCREEN_WIDTH, SCREEN_HEIGHT, STAT_POSX, STAT_POSY, STAT_WIDTH, STAT_HEIGHT);
		_StatBack->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_DescBack = new UISpriteColor(manager, this, MU::MakeARGB(0, 0, 0, 143), SCREEN_WIDTH, SCREEN_HEIGHT, DESC_POSX, DESC_POSY, DESC_WIDTH, DESC_HEIGHT);
		_DescBack->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_InputBox = new UIEditBox(CHARACTERUI::MESSAGE_CREATECHARACTER_NAMEINPUT, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, INPUT_POSX, INPUT_POSY, INPUT_WIDTH, INPUT_HEIGHT);
		_InputBox->SetLimitText(CHARACTER_MAXIMUM);
		_InputBox->SetTextColor(255, 255, 255, 255);
		_InputBox->SetBackColor(0, 0, 0, 0);
		_InputBox->SetHighlightTextColor(0, 0, 0, 255);
		_InputBox->SetHighlightColor(150, 150, 150, 255);

		_OkButton = new UIButton(CHARACTERUI::MESSAGE_CREATECHARACTER_OKBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, OK_POSX, OK_POSY, OK_WIDTH, OK_HEIGHT);
		_OkButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_OkButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_OkButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));

		_CancelButton = new UIButton(CHARACTERUI::MESSAGE_CREATECHARACTER_CANCELBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CANCEL_POSX, CANCEL_POSY, CANCEL_WIDTH, CANCEL_HEIGHT);
		_CancelButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CancelButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_CancelButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));

		mu_uint32 classCodes[GAME::MAX_CLASS] =
		{
			20, // Dark Wizard
			21, // Dark Knight
			22, // Elf
			23, // Magic Gladiator
			24, // Dark Lord
			1687, // Summoner
		};

		for (mu_uint32 n = GAME::CLASS_WIZARD; n < GAME::MAX_CLASS; ++n)
		{
			_ClassesButton[n] = new UIRadioButton(CHARACTERUI::MESSAGE_CREATECHARACTER_CLASS + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CLASS_POSX, CLASS_POSY + ((mu_float)n * CLASS_HEIGHT), CLASS_WIDTH, CLASS_HEIGHT);
			_ClassesButton[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_ClassesButton[n]->SetStateData(UIRadioButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVERLIST_BACKGROUND], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(100, 100, 100, 255)));
			_ClassesButton[n]->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVERLIST_BACKGROUND], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(255, 255, 255, 255)));
			_ClassesButton[n]->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVERLIST_BACKGROUND], EVector2(0.0f, 0.25f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
			_ClassesButton[n]->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVERLIST_BACKGROUND], EVector2(0.0f, 0.75f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
			_ClassesButton[n]->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVERLIST_BACKGROUND], EVector2(0.0f, 0.75f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
			_ClassesButton[n]->SetStateText(UIRadioButton::UI_DISABLED, FONT_NORMAL, GlobalText[classCodes[n]], MU::MakeARGB(150, 150, 150, 200));
			_ClassesButton[n]->SetStateText(UIRadioButton::UI_NORMAL, FONT_NORMAL, GlobalText[classCodes[n]], MU::MakeARGB(226, 226, 226, 255));
			_ClassesButton[n]->SetStateText(UIRadioButton::UI_OVER, FONT_NORMAL, GlobalText[classCodes[n]], MU::MakeARGB(255, 255, 255, 255));
			_ClassesButton[n]->SetStateText(UIRadioButton::UI_PRESSED, FONT_NORMAL, GlobalText[classCodes[n]], MU::MakeARGB(226, 226, 226, 255));
			_ClassesButton[n]->SetStateText(UIRadioButton::UI_PRESSED_OVER, FONT_NORMAL, GlobalText[classCodes[n]], MU::MakeARGB(226, 226, 226, 255));
		}

		_ClassesButton[GAME::CLASS_SUMMONER]->SetPosition(CLASS_POSX, CLASS_POSY + ((mu_float)3 * CLASS_HEIGHT));
		_ClassesButton[GAME::CLASS_DARK]->SetPosition(CLASS_POSX, CLASSEX_POSY);
		_ClassesButton[GAME::CLASS_DARK_LORD]->SetPosition(CLASS_POSX, CLASSEX_POSY + CLASS_HEIGHT);

		_SelectedClass = -1;
		SetSelectedClass(GAME::CLASS_KNIGHT);
	}
}

UICreateCharacter::~UICreateCharacter()
{

}

void UICreateCharacter::Update(mu_uint64 CurrentTime)
{
	_InputBox->Update(CurrentTime);
}

void UICreateCharacter::Draw()
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[0];

	_Background->Draw();

	if (_SlotCount > 0)
	{
		ThreadInfo.EnableScissor = true;
		ThreadInfo.ScissorRect.left = MU_TO_WINDOW(MODEL_POSX, _AspectWidth);
		ThreadInfo.ScissorRect.top = MU_TO_WINDOW(MODEL_POSY, _AspectHeight);
		ThreadInfo.ScissorRect.right = ThreadInfo.ScissorRect.left + MU_TO_WINDOW(MODEL_WIDTH, _AspectWidth);
		ThreadInfo.ScissorRect.bottom = ThreadInfo.ScissorRect.top + MU_TO_WINDOW(MODEL_HEIGHT, _AspectHeight);

		g_Tasking.RenderGUIModelsAtRuntime(_SlotIndex, _SlotCount);
	}

	_StatBack->Draw();
	_DescBack->Draw();
	_InputFrame->Draw();
	_InputBox->Draw();
	_OkButton->Draw();
	_CancelButton->Draw();

	for (mu_uint32 n = GAME::CLASS_WIZARD; n < GAME::MAX_CLASS; ++n)
	{
		_ClassesButton[n]->Draw();
	}

	const mu_text *Stats[GAME::MAX_CLASS][5] =
	{
		{ _T("18"), _T("18"), _T("15"), _T("30"), _T("0") },		// Dark Wizard
		{ _T("28"), _T("20"), _T("25"), _T("10"), _T("0") },		// Dark Knight
		{ _T("22"), _T("25"), _T("20"), _T("15"), _T("0") },		// Elf
		{ _T("26"), _T("26"), _T("26"), _T("26"), _T("0") },		// Magic Gladiator
		{ _T("26"), _T("20"), _T("20"), _T("15"), _T("25") },		// Dark Lord
		{ _T("21"), _T("21"), _T("18"), _T("23"), _T("0") },		// Summoner
	};

	mu_uint32 LabelCode[5] =
	{
		1701, // Strength
		1702, // Dexterity
		1703, // Vitality
		1704, // Energy
		1738, // Leadership
	};

	mu_uint32 maxStat = _SelectedClass == GAME::CLASS_DARK_LORD ? 5 : 4;

	mu_uint32 labelColor = MU::MakeARGB(255, 255, 255, 255);
	mu_uint32 valueColor = MU::MakeARGB(255, 180, 0, 255);

	mu_float posX = MU_TO_WINDOW(STATTEXT_POSX, _AspectWidth);
	mu_float posX2 = MU_TO_WINDOW(STATTEXT_POSX2, _AspectWidth);
	mu_float posY = MU_TO_WINDOW(STATTEXT_POSY, _AspectHeight);
	mu_float incY = MU_TO_WINDOW(17.0f, _AspectHeight);

	for (mu_uint32 n = 0; n < maxStat; ++n)
	{
		FONT_NORMAL->DrawStringC2D(TEXTINDEX::CREATECHARACTER_STAT + n,
			posX,
			posY,
			1.0f,
			ETA_LEFT, ETA_BOTTOM,
			0.0f, labelColor, 0,
			GlobalText[LabelCode[n]]);

		FONT_NORMAL->DrawStringC2D(TEXTINDEX::CREATECHARACTER_STAT + n,
			posX2,
			posY,
			1.0f,
			ETA_RIGHT, ETA_BOTTOM,
			0.0f, valueColor, 0,
			Stats[_SelectedClass][n]);

		posY += incY;
	}
}

void UICreateCharacter::Draw3D()
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[0];
	++ThreadInfo.GUIRenderIndex;

	_SlotIndex = g_GUIRenderSlotCount;

	OBJECT *o = &CharacterView.Object;
	o->Light = EVector3(1.0f, 1.0f, 1.0f);

	mu_float X = BASE_POSX + 173.0f;
	mu_float Y = BASE_POSY + 346.0f;

	switch (CharacterView.Class)
	{
	case GAME::CLASS_WIZARD:
		{
			Vector(-90.0f, -40.0f, 0.0f, o->Angle);
			o->Scale = 0.059f;
		}
		break;
	case GAME::CLASS_KNIGHT:
		{
			Vector(-90.0f, -12.0f, 0.0f, o->Angle);
			o->Scale = 0.0605f;
		}
		break;
	case GAME::CLASS_ELF:
		{
			Vector(-82.0f, 5.0f, 0.0f, o->Angle);
			o->Scale = 0.091f;
			X += 4.8f;
		}
		break;
	case GAME::CLASS_DARK:
		{
			Vector(-82.0f, -13.0f, 0.0f, o->Angle);
			o->Scale = 0.06f;
			Y -= 1.8f;
		}
		break;
	case GAME::CLASS_DARK_LORD:
		{
			Vector(-82.0f, -18.0f, 0.0f, o->Angle);
			o->Scale = 0.06f;
		}
		break;
	case GAME::CLASS_SUMMONER:
		{
			Vector(-88.0f, 2.0f, 0.0f, o->Angle);
			o->Scale = 0.091f;
			X += 4.8f;
			Y -= 4.0f;
		}
		break;
	}

	EVector3 Source, Target;
	EVideoBackend::Unproject2D(X, Y, _AspectWidth, _AspectHeight, Source, Target);

	EVector3 Direction;
	VectorSubtract(Target, Source, Direction);
	VectorMA(Source, 0.07f, Direction, o->Position);

	g_Tasking.RenderCharacter(0, &CharacterView, o);

	_SlotCount = g_GUIRenderSlotCount - _SlotIndex;
}

void UICreateCharacter::PostDraw()
{
	_OkButton->PostDraw();
	_CancelButton->PostDraw();

	for (mu_uint32 n = GAME::CLASS_WIZARD; n < GAME::MAX_CLASS; ++n)
	{
		_ClassesButton[n]->PostDraw();
	}
}

mu_uint32 UICreateCharacter::OnPreWindowMessage(const SDL_Event *event, mu_size *result)
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
				case SDL_SCANCODE_RETURN:
				case SDL_SCANCODE_KP_ENTER:
					{
						_SceneManager->OnGFxMessagePush(_OkButton->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
					}
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;

				case SDL_SCANCODE_ESCAPE:
					{
						_SceneManager->OnGFxMessagePush(_CancelButton->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
					}
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
				}
			}
		}
		break;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

UIBase *UICreateCharacter::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = _OkButton->GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		return focus;
	}

	focus = _CancelButton->GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		return focus;
	}

	focus = _InputBox->GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		return focus;
	}

	for (mu_uint32 n = GAME::CLASS_WIZARD; n < GAME::MAX_CLASS; ++n)
	{
		focus = _ClassesButton[n]->GetFocus(mouseX, mouseY);

		if (focus != nullptr)
		{
			return focus;
		}
	}

	return GFX_BYPASS_FOCUS;
}

void UICreateCharacter::SetSelectedClass(mu_uint32 Class)
{
	if (_SelectedClass != -1)
	{
		UIBase *Focused = _SceneManager->GetFocused();

		_ClassesButton[_SelectedClass]->ChangeState(_ClassesButton[_SelectedClass] == Focused ? UIRadioButton::UI_OVER : UIRadioButton::UI_NORMAL);	
		_ClassesButton[Class]->ChangeState(_ClassesButton[Class] == Focused ? UIRadioButton::UI_PRESSED_OVER : UIRadioButton::UI_PRESSED);
	}
	else
	{
		_ClassesButton[Class]->ChangeState(UIRadioButton::UI_PRESSED);
	}

	_SelectedClass = Class;
	CharacterView.Class = Class;
	CharacterViewLive = true;
	CharacterViewVisible = false;
	CreateCharacterPointer(&CharacterView, MODEL_FACE + CharacterView.Class, 0, 0);
	CharacterView.Object.Flags.Set(OBJECT::eOF_LIVE);
	CharacterView.Object.Flags.Clear(OBJECT::eOF_LIGHTENABLE);
	CharacterView.Object.Kind = 0;
	SetAction(&CharacterView.Object, 1);
}