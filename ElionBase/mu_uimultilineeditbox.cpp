#include "stdafx.h"
#include "mu_uimultilineeditbox.h"
#include "mu_uieditbox.h"

mu_unicode *UIMultiLineEditBox::s_TextBuffer = nullptr;
mu_uint32 UIMultiLineEditBox::s_TextBufferLength = 0;

UIMultiLineEditBox::UIMultiLineEditBox(mu_uint32 objectId, UISceneManager *manager, UIBase *owner, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_float width, mu_float height, UIScrollBar *verticalScroll, UIScrollBar *horizontalScroll) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_ObjectID = objectId;
	_SceneManager = manager;

	UIBase::SetAspectRatio(aspectWidth, aspectHeight);
	UIBase::SetPosition(x, y);
	UIBase::SetDimensions(width, height);
	UIBase::SetParent(owner);

	SetGfxFlag(GFX_FLAG::GFX_NONE);
	SetEvent(GFX_EVENT::GFX_MOUSE_EVENT |
		GFX_EVENT::GFX_KEYBOARD_EVENT |
		GFX_EVENT::GFX_PREMESSAGE_FIRST |
		GFX_EVENT::GFX_PREMESSAGE_FOCUS);

	_TextColor = MU::MakeARGB(255, 255, 255, 255);
	_BackColor = MU::MakeARGB(0, 0, 0, 100);
	_HighlightTextColor = MU::MakeARGB(255, 255, 255, 255);
	_HighlightColor = MU::MakeARGB(0, 0, 255, 255);

	_Caret = new UISpriteColor(manager, this, _TextColor, aspectWidth, aspectHeight, 0.0f, 0.0f, 1.0f, FONT_NORMAL->GetFont()->_MaxAdvanceY);
	_Caret->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
	_CaretX = 0.0f;
	_CaretY = 0.0f;

	_Style = 0;

	_Buffer = nullptr;
	_BufferLength = 0;
	_MaxLength = DEFAULT_MLLIMIT;
	_Length = 0;
	_Lines = (mu_uint32*)mu_realloc(nullptr, sizeof(mu_uint32) * 2);
	mu_zeromem(_Lines, sizeof(mu_uint32) * 2);
	_LinesLength = 2;
	_LinesCount = 1;
	_MinimumSelection = 0;
	_MaximumSelection = 0;
	_StartMinimumSelection = 0;
	_StartMaximumSelection = 0;
	_CaretIndex = 0;
	_CaretLine = 0;
	_DisplayIndexX = 0;
	_DisplayIndexY = 0.0f;
	_DisplayLinesCount = 0;

	_IsHighlightCorrection = false;
	_IsMouseDown = false;
	_IsFocus = false;
	_IsDirty = false;
	_AutoHScroll = true;
	_AutoVScroll = true;
	_NoHideSel = false;
	_EatNextChar = false;

	_ReadOnly = false;
	_CaretHidden = false;

	_WrapMode = false;
	_CalcLines = false;

	mu_zeromem(&_Undo, sizeof(_Undo));
	_Undo.InsertionStart = 0xFFFFFFFF;
	_Undo.InsertionEnd = 0xFFFFFFFF;
	_Undo.DeletedIndex = 0xFFFFFFFF;

	_RenderIndex = g_RandomTextIndex.fetch_add(1);

	_VerticalScroll = verticalScroll;
	_HorizontalScroll = horizontalScroll;

	if (_VerticalScroll != nullptr)
	{
		_VerticalScroll->SetMessageInheritor(this);
	}

	if (_HorizontalScroll != nullptr)
	{
		_HorizontalScroll->SetMessageInheritor(this);
	}

	SetFont(FONT_NORMAL);
	ScrollText();
}

UIMultiLineEditBox::~UIMultiLineEditBox()
{
	MU_SAFE_FREE(_Buffer);
	MU_SAFE_FREE(_Undo.DeletedText);
	MU_SAFE_FREE(_Lines);
}

void UIMultiLineEditBox::SystemConfigure()
{
	UIBase::SystemConfigure();

	SetFont(_Font);
}

void UIMultiLineEditBox::Update(mu_uint64 CurrentTime)
{
	if (_IsDirty == true)
	{
		_IsDirty = false;
		DrawText();
	}

	if (_UpdateGPU == true)
	{
		_UpdateCaret = true;
		_UpdateGPU = false;
	}
}

void UIMultiLineEditBox::Draw()
{
	if (IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == true)
	{
		return;
	}

	if (_Buffer != nullptr &&
		_Length > 0)
	{
		if (ContainsStyle(UES_RIGHT))
		{
			_Font->DrawString2D(_RenderIndex, _NativeX + _Width, _NativeY, 1.0f, ETA_RIGHT, ETA_BOTTOM, 0.0f, _DrawString.c_str(), 0.0f, 0.0f, nullptr, _IsHighlightCorrection);
		}
		else if (ContainsStyle(UES_CENTER))
		{
			_Font->DrawString2D(_RenderIndex, _NativeX + _Width * 0.5f, _NativeY, 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, _DrawString.c_str(), 0.0f, 0.0f, nullptr, _IsHighlightCorrection);
		}
		else
		{
			_Font->DrawString2D(_RenderIndex, _NativeX, _NativeY, 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, _DrawString.c_str(), 0.0f, 0.0f, nullptr, _IsHighlightCorrection);
		}
	}
	else
	{
		static const mu_unicode *textFixer = _T("");
		if (ContainsStyle(UES_RIGHT))
		{
			_Font->DrawString2D(_RenderIndex, _NativeX + _Width, _NativeY, 1.0f, ETA_RIGHT, ETA_BOTTOM, 0.0f, textFixer, 0.0f, 0.0f, nullptr, _IsHighlightCorrection);
		}
		else if (ContainsStyle(UES_CENTER))
		{
			_Font->DrawString2D(_RenderIndex, _NativeX + _Width * 0.5f, _NativeY, 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textFixer, 0.0f, 0.0f, nullptr, _IsHighlightCorrection);
		}
		else
		{
			_Font->DrawString2D(_RenderIndex, _NativeX, _NativeY, 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textFixer, 0.0f, 0.0f, nullptr, _IsHighlightCorrection);
		}
	}

	if (_UpdateCaret == true)
	{
		_UpdateCaret = false;

		mu_uint8 horizontalAlign = ETA_LEFT;

		if (ContainsStyle(UES_RIGHT) == true)
		{
			horizontalAlign = ETA_RIGHT;
		}
		else if (ContainsStyle(UES_CENTER) == true)
		{
			horizontalAlign = ETA_CENTER;
		}

		EVector2 Size;
		_Font->GetTextExtentPoint(_DrawString.c_str(), _DrawString.size(), horizontalAlign, 1.0f, &Size, _CaretIndex - _Lines[(mu_uint32)_DisplayIndexY] - _DisplayIndexX, &_CaretX, &_CaretY);
		_Font->UpdateCoord(_RenderIndex, _CaretX, _CaretY);

		_Caret->SetPosition(EMath::Round(_CaretX), _CaretY, false);
	}

	if (g_CaretEnabled == true &&
		_CaretHidden == false &&
		_IsFocus == true &&
		_ReadOnly == false &&
		_CaretLine >= (mu_uint32)_DisplayIndexY &&
		_CaretLine < (mu_uint32)_DisplayIndexY + _DisplayLinesCount)
	{
		_Caret->Draw();
	}
}

void UIMultiLineEditBox::OnFocus()
{
	_IsFocus = true;
}

void UIMultiLineEditBox::OnFocusCancelled()
{
	_IsFocus = false;
	_MinimumSelection = _MaximumSelection = _CaretIndex;

	/*
	* Set dirty bit
	*/
	_IsDirty = true;
}

mu_uint32 UIMultiLineEditBox::OnPreWindowMessage(const SDL_Event *event, mu_size *result)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		{
			if (_SceneManager->GetSelected() == this)
			{
				switch (event->key.keysym.scancode)
				{
				default:
					{
						mu_uint32 isAccept = OnKeyDown(event->key.keysym);
						if (isAccept != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
							return isAccept;
					}
					break;

				case SDL_SCANCODE_KP_ENTER:
				case SDL_SCANCODE_ESCAPE:
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;
				}
			}
		}
		break;

	case SDL_MOUSEWHEEL:
		{
			if (_SceneManager->GetFocused() == this)
			{
				/*
				* Don't handle zoom and datazoom.
				*/
				if (MU_INPUT->IsShiftPressing() == true ||
					MU_INPUT->IsCtrlPressing() == true)
				{
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;
				}

				if (_DisplayLinesCount >= _LinesCount)
				{
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
				}

				mu_float mouseWheel = event->wheel.y;

				mu_int32 tmpDisplayY = (mu_int32)_DisplayIndexY;
				_DisplayIndexY += -mouseWheel;
				mu_int32 newDisplayY = (mu_int32)_DisplayIndexY;

				if (newDisplayY < 0)
				{
					_DisplayIndexY = newDisplayY = 0;
				}
				else if (newDisplayY >= _LinesCount - _DisplayLinesCount)
				{
					_DisplayIndexY = newDisplayY = _LinesCount - _DisplayLinesCount;
				}

				if (newDisplayY != tmpDisplayY)
				{
					ScrollText(false);
					_IsDirty = true;
				}

				return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
			}
		}
		break;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

mu_uint32 UIMultiLineEditBox::OnWindowMessage(const SDL_Event *event, mu_size *result)
{
	switch (event->type)
	{
	case SDL_TEXTINPUT:
		{
			if (_EatNextChar == true)
				_EatNextChar = false;
			else
			{
				OnChar(boost::locale::conv::utf_to_utf<mu_text>(event->text.text).c_str()[0]);
			}
		}
		return GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

void UIMultiLineEditBox::OnChar(mu_unicode keyValue, mu_unicode mod)
{
	mu_boolean updateText = false;

	if (IsMouseFlag(MOUSE_FLAG_LEFT) == true ||
		(_ReadOnly == true && (keyValue != _T('c') || (mod & KMOD_CTRL) == 0)))
	{
		/*
		* Don't do anything if we are in the middle of a mousedown deal or if
		* this is a read only edit control, with exception of allowing
		* ctrl-C in order to copy to the clipboard.
		*/
		return;
	}

	switch (keyValue)
	{
	case _T('z'):
		{
			if ((mod & KMOD_CTRL) != 0)
			{
				/*
				* CTRL-Z Undo
				*/
				Undo();
			}
			else
			{
				goto processKey1;
			}
		}
		break;

	case _T('x'):
		{
			if ((mod & KMOD_CTRL) != 0)
			{
				/*
				* CTRL-X Cut
				*/
				if (_MinimumSelection != _MaximumSelection)
				{
					Cut();
				}
			}
			else
			{
				goto processKey1;
			}
		}
		break;

	case _T('c'):
		{
			if ((mod & KMOD_CTRL) != 0)
			{
				/*
				* CTRL-C Copy
				*/
				Copy();
			}
			else
			{
				goto processKey1;
			}
		}
		break;

	case _T('v'):
		{
			if ((mod & KMOD_CTRL) != 0)
			{
				/*
				* CTRL-V Paste
				*/
				if (_ReadOnly == false)
				{
					Paste();
				}
			}
			else
			{
				goto processKey1;
			}
		}
		break;

	case _T('\r'):
		keyValue = _T('\n');

	case _T('\n'):
	case _T('\t'):
	case _T('\b'):
DeleteSelection:
		if (DeleteText())
			updateText = true;
		break;

	default:
processKey1:
		if (keyValue >= _T(' '))
		{
			if (ContainsStyle(UES_NUMBER) == true &&
				mu_isdigit(keyValue) == 0)
			{
				return;
			}

			goto DeleteSelection;
		}
		break;
	}

	switch (keyValue)
	{
	case _T('z'):
		{
			if ((mod & KMOD_CTRL) == 0)
			{
				goto processKey;
			}
		}
		break;

	case _T('x'):
		{
			if ((mod & KMOD_CTRL) == 0)
			{
				goto processKey;
			}
		}
		break;

	case _T('c'):
		{
			if ((mod & KMOD_CTRL) == 0)
			{
				goto processKey;
			}
		}
		break;

	case _T('v'):
		{
			if ((mod & KMOD_CTRL) == 0)
			{
				goto processKey;
			}
		}
		break;

	case _T('\b'):
		{
			/*
			* Delete any selected text or delete character left if no sel
			*/
			if (!updateText && _MinimumSelection != 0)
			{

				/*
				* There was no selection to delete so we just delete character
				left if available
				*/
				//
				// Calling PreviousIndex rather than just doing a decrement for SDLK_BACK
				//
				_MinimumSelection = PreviousIndex(_MinimumSelection);
				DeleteText();
				updateText = true;
			}
		}
		break;

	default:
		{
processKey:
			if (keyValue >= 0x1E ||
				keyValue == _T('\n') ||
				keyValue == SDLK_TAB)
			{
				if (InsertText(&keyValue, 1, true) != 0)
				{
					updateText = true;
				}
				else
				{
					/*
					* Beep. Since we couldn't add the text
					*/
					// If want to beep can add some code here
				}
			}
			else
			{
				/*
				* User hit an illegal control key
				*/
				// If want to beep can add some code here
			}
		}
		break;
	}

	if (updateText)
	{
		if (ScrollText() == false)
		{
			_IsDirty = true;
		}
	}
}

mu_uint32 UIMultiLineEditBox::OnKeyDown(const SDL_Keysym &key)
{
	/*
	* Variables we will use for redrawing the updated text
	*/
	mu_uint32 newMaxSel = _MaximumSelection;
	mu_uint32 newMinSel = _MinimumSelection;

	/*
	* Flags for drawing the updated text
	*/
	mu_boolean updateText = false;
	mu_boolean changeSelection = false; /* new selection is specified by
										newMinSel, newMaxSel */

										/*
										* Comparisons we do often
										*/
	mu_boolean MinEqMax = (newMaxSel == newMinSel);
	mu_boolean MinEqCar = (_CaretIndex == newMinSel);
	mu_boolean MaxEqCar = (_CaretIndex == newMaxSel);

	/*
	* State of shift and control keys.
	*/
	mu_int32 scState;

	/*
	* Combo box support
	*/
	mu_boolean fIsListVisible;
	mu_boolean fIsExtendedUI;

	if (_IsMouseDown)
	{
		/*
		* If we are in the middle of a mouse down handler, then don't do
		* anything. ie. ignore keyboard input.
		*/
		return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
	}

	scState = 0;

	if (key.mod & KMOD_CTRL)
		scState |= CTRLDOWN;
	if (key.mod & KMOD_SHIFT)
		scState |= SHFTDOWN;

	switch (key.sym)
	{
	case SDL_SCANCODE_RETURN:
		{
			OnChar(key.sym);
		}
		break;

	case SDL_SCANCODE_Z:
	case SDL_SCANCODE_X:
	case SDL_SCANCODE_C:
	case SDL_SCANCODE_V:
		{
			if (scState == CTRLDOWN)
			{
				OnChar(SDL_GetKeyFromScancode(key.scancode), KMOD_CTRL);
			}
		}
		break;

	case SDL_SCANCODE_BACKSPACE:
		{
			OnChar(_T('\b'));
		}
		break;

	case SDL_SCANCODE_LEFT:
		//
		// If the caret isn't at the beginning, we can move left
		//
		if (_CaretIndex)
		{
			//
			// Get new caret pos.
			//
			if (scState & CTRLDOWN)
			{
				// Move caret word left
				Word(_CaretIndex, true, &_CaretIndex, nullptr);
			}
			else
			{
				// Move caret char left
				_CaretIndex = MoveSelection(_CaretIndex, true);
			}

			//
			// Get new selection
			//
			if (scState & SHFTDOWN)
			{
				if (MaxEqCar && !MinEqMax)
				{
					// Reduce selection
					newMaxSel = _CaretIndex;
				}
				else
				{
					// Extend selection
					newMinSel = _CaretIndex;
				}
			}
			else
			{
				//
				// Clear selection
				//
				newMaxSel = newMinSel = _CaretIndex;
			}

			changeSelection = true;
		}
		else
		{
			//
			// If the user tries to move left and we are at the 0th
			// character and there is a selection, then cancel the
			// selection.
			//
			if ((_MaximumSelection != _MinimumSelection) &&
				!(scState & SHFTDOWN))
			{
				changeSelection = true;
				newMaxSel = newMinSel = _CaretIndex;
			}
		}
		break;

	case SDL_SCANCODE_RIGHT:
		//
		// If the caret isn't at the end, we can move right.
		//
		if (_CaretIndex < _Length)
		{
			//
			// Get new caret pos.
			//
			if (scState & CTRLDOWN)
			{
				// Move caret word right
				Word(_CaretIndex, false, nullptr, &_CaretIndex);
			}
			else
			{
				// Move caret char right
				_CaretIndex = MoveSelection(_CaretIndex, false);
			}

			//
			// Get new selection.
			//
			if (scState & SHFTDOWN)
			{
				if (MinEqCar && !MinEqMax)
				{
					// Reduce selection
					newMinSel = _CaretIndex;
				}
				else
				{
					// Extend selection
					newMaxSel = _CaretIndex;
				}
			}
			else
			{
				// Clear selection
				newMaxSel = newMinSel = _CaretIndex;
			}

			changeSelection = true;
		}
		else
		{
			//
			// If the user tries to move right and we are at the last
			// character and there is a selection, then cancel the
			// selection.
			//
			if ((_MaximumSelection != _MinimumSelection) &&
				!(scState & SHFTDOWN))
			{
				newMaxSel = newMinSel = _CaretIndex;
				changeSelection = true;
			}
		}
		break;

	case SDL_SCANCODE_UP:
	case SDL_SCANCODE_DOWN:
		{
			mu_boolean prevLine;
			if (_LinesCount - 1 != _CaretLine &&
				_CaretIndex == _Lines[_CaretLine + 1])
				prevLine = true;
			else
				prevLine = false;

			EPoint mousePt;
			IchToXYPos(_CaretIndex, prevLine, mousePt);
			mousePt.y += 1 + (key.sym == SDL_SCANCODE_UP ? -_LineHeight : _LineHeight);

			if (!(scState & CTRLDOWN))
			{
				//
				// Send fake mouse messages to handle this
				// If SDL_SCANCODE_SHIFT is down, extend selection & move caret up/down
				// 1 line.  Otherwise, clear selection & move caret.
				//
				MousePressed(MOUSE_FLAG_LEFT, 0, !(scState & SHFTDOWN) ? 0 : VFLAG_SHIFT, mousePt);
				MouseReleased(MOUSE_FLAG_LEFT, !(scState & SHFTDOWN) ? 0 : VFLAG_SHIFT, mousePt);
			}
		}
		break;

	case SDL_SCANCODE_HOME:
		//
		// Move caret to top.
		//
		_CaretIndex = 0;

		//
		// Update selection.
		//
		if (scState & SHFTDOWN)
		{
			if (MaxEqCar && !MinEqMax)
			{
				// Reduce selection
				newMinSel = _CaretIndex;
				newMaxSel = _MinimumSelection;
			}
			else
			{
				// Extend selection
				newMinSel = _CaretIndex;
			}
		}
		else
		{
			// Clear selection
			newMaxSel = newMinSel = _CaretIndex;
		}

		UpdateCaretLine();

		changeSelection = true;
		break;

	case SDL_SCANCODE_END:
		//
		// Move caret to end.
		//
		_CaretIndex = _Length;

		//
		// Update selection.
		//
		newMaxSel = _CaretIndex;
		if (scState & SHFTDOWN)
		{
			if (MinEqCar && !MinEqMax)
			{
				// Reduce selection
				newMinSel = _MaximumSelection;
			}
		}
		else
		{
			// Clear selection
			newMinSel = _CaretIndex;
		}

		UpdateCaretLine();

		changeSelection = true;
		break;

	case SDL_SCANCODE_DELETE:
		if (_ReadOnly)
			break;

		switch (scState)
		{
		case NONEDOWN:
			/*
			* Clear selection. If no selection, delete (clear) character
			* right.
			*/
			if ((_MaximumSelection < _Length) && (_MinimumSelection == _MaximumSelection))
			{
				/*
				* Move cursor forwards and simulate a backspace.
				*/
				_CaretIndex = NextIndex(_CaretIndex);
				_MaximumSelection = _MinimumSelection = _CaretIndex;
				UpdateCaretLine();
				OnChar(_T('\b'));
			}
			if (_MinimumSelection != _MaximumSelection)
			{
				OnChar(_T('\b'));
			}
			break;

		case SHFTDOWN:

			//
			// Send ourself a WM_CUT message if a selection exists.
			// Otherwise, delete the left character.
			//
			if (_MinimumSelection == _MaximumSelection)
			{
				OnChar(_T('\b'));
			}
			else
			{
				//SendMessage(ped->hwnd, WM_CUT, 0, 0L); // FIX
			}
			break;

		case CTRLDOWN:

			/*
			* Delete to end of line if no selection else delete (clear)
			* selection.
			*/
			if ((_MaximumSelection < _Length) && (_MinimumSelection == _MaximumSelection))
			{
				/*
				* Move cursor to end of line and simulate a backspace.
				*/
				_MaximumSelection = _CaretIndex = _Length;
				UpdateCaretLine();
			}
			if (_MinimumSelection != _MaximumSelection)
			{
				OnChar(_T('\b'));
			}
			break;

		}

		/*
		* No need to update text or selection since BACKSPACE message does it
		* for us.
		*/
		break;

	case SDL_SCANCODE_INSERT:
		switch (scState)
		{
		case CTRLDOWN:

			/*
			* Copy current selection to clipboard
			*/
			//SendMessage(ped->hwnd, WM_COPY, 0, 0); // FIX
			break;

		case SHFTDOWN:
			//SendMessage(ped->hwnd, WM_PASTE, 0, 0L); // FIX
			break;
		}
		break;
	}

	if (changeSelection || updateText)
	{
		/*
		* Set the caret's line
		*/
		UpdateCaretLine();

		/*
		* Scroll if needed
		*/
		ScrollText();

		if (changeSelection)
			ChangeSelection(newMinSel, newMaxSel);

		if (updateText)
			_IsDirty = true;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;
}

void UIMultiLineEditBox::SaveUndo(UIUndoBuffer *FromUndo, UIUndoBuffer *ToUndo, mu_boolean clear)
{
	/*
	*  Save undo data
	*/
	mu_memcpy(ToUndo, FromUndo, sizeof(UIUndoBuffer));

	/*
	*  Clear passed in undo buffer
	*/
	if (clear)
	{
		mu_zeromem(FromUndo, sizeof(UIUndoBuffer));
	}
}

void UIMultiLineEditBox::EmptyUndo(UIUndoBuffer *Undo)
{
	MU_SAFE_FREE(Undo->DeletedText);

	mu_zeromem(Undo, sizeof(UIUndoBuffer));
}

void UIMultiLineEditBox::Word(mu_uint32 ichStart, mu_boolean fLeft, mu_uint32 *pichMin, mu_uint32 *pichMax)
{
	mu_boolean charLocated = false;
	mu_boolean spaceLocated = false;

	if ((!ichStart && fLeft) || (ichStart == _Length && !fLeft))
	{
		/*
		* We are at the beginning of the text (looking left) or we are at end
		* of text (looking right), no word here
		*/
		if (pichMin) *pichMin = 0;
		if (pichMax) *pichMax = 0;
		return;
	}

	mu_unicode *pwText;
	mu_unicode *pwWordMinSel;
	mu_unicode *pwWordMaxSel;
	mu_unicode *pwPrevChar;

	pwText = _Buffer;
	pwWordMinSel = pwWordMaxSel = pwText + ichStart;

	/*
	* if fLeft: Move pWordMinSel to the left looking for the start of a word.
	* If we start at a space, we will include spaces in the selection as we
	* move left untill we find a nonspace character. At that point, we continue
	* looking left until we find a space. Thus, the selection will consist of
	* a word with its trailing spaces or, it will consist of any leading at the
	* beginning of a line of text.
	*/

	/*
	* if !fLeft: (ie. right word) Move pWordMinSel looking for the start of a
	* word. If the pWordMinSel points to a character, then we move left
	* looking for a space which will signify the start of the word. If
	* pWordMinSel points to a space, we look right till we come upon a
	* character. pMaxWord will look right starting at pMinWord looking for the
	* end of the word and its trailing spaces.
	*/


	if (fLeft || (!ISDELIMETER(*pwWordMinSel) && *pwWordMinSel != _T('\r')))
		/* If we are moving left or if we are moving right and we are not on a
		* space or a CR (the start of a word), then we was look left for the
		* start of a word which is either a CR or a character. We do this by
		* looking left till we find a character (or if CR we stop), then we
		* continue looking left till we find a space or LF.
		*/
	{
		while (pwWordMinSel > pwText && ((!ISDELIMETER(*(pwWordMinSel - 1)) && *(pwWordMinSel - 1) != _T('\n')) || !charLocated))
		{
			/*
			* Treat double byte character as a word  ( in unicode pwWordMinSel loop )
			*/
			pwPrevChar = pwWordMinSel - 1;
			/*
			** we are looking right ( !fLeft ).
			**
			** if current character is a double width chararacter
			** or previous character is a double width character,
			** we are on the beggining of a word.
			*/
			if (!fLeft && (ISDELIMETER(*pwPrevChar) ||
				*pwPrevChar == _T('\n') ||
				UserIsFullWidth(*pwWordMinSel) ||
				UserIsFullWidth(*pwPrevChar)))
			{
				/*
				* If we are looking for the start of the word right, then we
				* stop when we have found it. (needed in case charLocated is
				* still false)
				*/
				break;
			}

			if (UserIsFullWidth(*pwPrevChar))
			{
				/*
				** Previous character is a double width character.
				**
				** if we are in a word ( charLocated == true )
				** current position is the beginning of the word
				** if we are not in a word ( charLocated == false )
				** the previous character is what we looking for.
				*/
				if (!charLocated) {
					pwWordMinSel = pwPrevChar;
				}
				break;
			}
			pwWordMinSel = pwPrevChar;

			if (!ISDELIMETER(*pwWordMinSel) && *pwWordMinSel != _T('\n'))
				/*
				* We have found the last char in the word. Continue looking
				* backwards till we find the first char of the word
				*/
			{
				charLocated = true;

				/*
				* We will consider a CR the start of a word
				*/
				if (*pwWordMinSel == _T('\r'))
					break;
			}
		}
	}
	else
	{

		/*
		* We are moving right and we are in between words so we need to move
		* right till we find the start of a word (either a CR or a character.
		*/
		while ((ISDELIMETER(*pwWordMinSel) || *pwWordMinSel == _T('\n')) && pwWordMinSel < pwText + _Length)
			++pwWordMinSel;
	}

	pwWordMaxSel = (pwWordMinSel + 1) <= (pwText + _Length) ? pwWordMinSel + 1 : pwText + _Length;

	/*
	** If pwWordMinSel points a double width character AND
	**    pwWordMaxSel points non space
	** then
	**    pwWordMaxSel points the beggining of next word.
	*/
	if (UserIsFullWidth(*pwWordMinSel) &&
		!ISDELIMETER(*pwWordMaxSel))
		goto FastReturnW;
	if (*pwWordMinSel == _T('\r'))
	{
		if (pwWordMinSel > pwText && *(pwWordMinSel - 1) == _T('\r'))
			/* So that we can treat CRCRLF as one word also. */
			--pwWordMinSel;
		else if (*(pwWordMinSel + 1) == _T('\r'))
			/* Move MaxSel on to the LF */
			++pwWordMaxSel;
	}



	/*
	* Check if we have a one character word
	*/
	if (ISDELIMETER(*pwWordMaxSel))
		spaceLocated = true;

	/*
	* Move pwWordMaxSel to the right looking for the end of a word and its
	* trailing spaces. WordMaxSel stops on the first character of the next
	* word. Thus, we break either at a CR or at the first nonspace char after
	* a run of spaces or LFs.
	*/
	while ((pwWordMaxSel < pwText + _Length) && (!spaceLocated || (ISDELIMETER(*pwWordMaxSel))))
	{
		if (*pwWordMaxSel == _T('\r'))
			break;

		/*
		* treat double byte character as a word ( in unicode pwWordMaxSel loop )
		*/
		/*
		** if it's a double width character
		** then we are at the beginning of
		** the next word which is a double
		** width character.
		*/
		if (UserIsFullWidth(*pwWordMaxSel))
			break;

		++pwWordMaxSel;

		if (ISDELIMETER(*pwWordMaxSel))
			spaceLocated = true;


		if (*(pwWordMaxSel - 1) == _T('\n'))
			break;
	}

	/*
	* label for fast return ( for Unicode )
	*/
FastReturnW:
	if (pichMin)   *pichMin = (mu_uint32)(pwWordMinSel - pwText);
	if (pichMax)   *pichMax = (mu_uint32)(pwWordMaxSel - pwText);
}

mu_uint32 UIMultiLineEditBox::PreviousIndex(mu_uint32 currentIndex)
{
	return currentIndex > 0 ? currentIndex - 1 : 0;
}

mu_uint32 UIMultiLineEditBox::NextIndex(mu_uint32 currentIndex)
{
	return currentIndex + 1;
}

void UIMultiLineEditBox::MergeUndoInsertInfo(mu_uint32 insertIndex, mu_uint32 insertCount)
{
	//
	// If undo buffer is empty, just insert the new info as UNDO_INSERT
	//
	if (_Undo.UndoType == UNDO_NONE)
	{
		_Undo.UndoType = UNDO_INSERT;
		_Undo.InsertionStart = insertIndex;
		_Undo.InsertionEnd = insertIndex + insertCount;
	}
	else if ((_Undo.UndoType & UNDO_INSERT) != 0)
	{
		//
		// If there's already some undo insert info,
		// try to merge the two.
		//
		if (_Undo.InsertionEnd == insertIndex) // Check they are adjacent.
			_Undo.InsertionEnd += insertCount; // if so, just concatenate.
		else
		{
			// The new insert is not contiguous with the old one.
UNDOINSERT:
			//
			// If there is some UNDO_DELETE info already here, check to see
			// if the new insert takes place at a point different from where
			// that deletion occurred.
			//
			if ((_Undo.UndoType & UNDO_DELETE) && (_Undo.DeletedIndex != insertIndex))
			{
				//
				// User is inserting into a different point; So, let us
				// forget any UNDO_DELETE info;
				//
				MU_SAFE_DELETE_ARRAY(_Undo.DeletedText);
				_Undo.DeletedIndex = 0xFFFFFFFF;
				_Undo.UndoType &= ~UNDO_DELETE;
			}

			// Since the old insert and new insert are not adjacent, let us
			// forget everything about the old insert and keep just the new
			// insert info as the UNDO_INSERT.
			_Undo.InsertionStart = insertIndex;
			_Undo.InsertionEnd = insertIndex + insertCount;
			_Undo.UndoType |= UNDO_INSERT;
		}
	}
	else if (_Undo.UndoType == UNDO_DELETE)
	{
		// If there is some Delete Info already present go and handle it.
		goto UNDOINSERT;
	}
}

mu_boolean UIMultiLineEditBox::SetText(const mu_unicode *Text)
{
	mu_uint32 cchLength = 0;
	mu_uint32 cchSave = _Length;
	mu_uint32 ichCaretSave = _CaretIndex;
	mu_unicode *hText;

	_Length = _CaretIndex = 0;
	UpdateCaretLine();

	if (Text == nullptr || mu_stricmp(Text, _T("")) == 0)
	{
		hText = (mu_unicode*)mu_realloc(_Buffer, EXTRA_ALLOC * sizeof(mu_unicode));
		if (hText != nullptr)
		{
			_Buffer = hText;
			_BufferLength = EXTRA_ALLOC;
			_LinesCount = 1;
			_DisplayIndexX = 0;
			_DisplayIndexY = 0.0f;
		}
		else
		{
			return false;
		}
	}
	else
	{
		cchLength = mu_strlen(Text);

		/*
		* Add the text
		*/
		if (cchLength > 0 && !InsertText(Text, cchLength, false))
		{
			/*
			* Restore original state and notify parent we ran out of memory.
			*/
			_Length = cchSave;
			_CaretIndex = ichCaretSave;
			UpdateCaretLine();
			return false;
		}
		else if (cchLength > 0)
		{
			_CaretIndex = _Length;
			UpdateCaretLine();
		}
	}

	ResetTextInfo();
	ScrollText();

	if (cchLength > 0)
	{
		_MinimumSelection = _Length;
		_MaximumSelection = _Length;
	}

	return true;
}

void UIMultiLineEditBox::GetText(mu_unicode *Buffer, mu_uint32 BufferLength)
{
	mu_uint32 length = EMath::Min(_Length, BufferLength - 1);

	if (length > 0)
	{
		mu_memcpy(Buffer, _Buffer, length * sizeof(mu_unicode));
	}
	Buffer[length] = _T('\0');
}

mu_uint32 UIMultiLineEditBox::GetLength()
{
	return _Length;
}

mu_uint32 UIMultiLineEditBox::DeleteText()
{
	mu_unicode *pedText;
	mu_uint32 cchDelete;
	mu_unicode *lpDeleteSaveBuffer;
	mu_unicode *hDeletedText;
	mu_uint32 bufferOffset;

	cchDelete = _MaximumSelection - _MinimumSelection;

	if (cchDelete == 0)
		return 0;

	/*
	* Ok, now lets delete the text.
	*/
	pedText = _Buffer;

	mu_int32 minSelLine = IchToLine(_MinimumSelection);
	mu_int32 maxSelLine = IchToLine(_MaximumSelection);

	mu_boolean fastDelete = false;

	if (_AutoVScroll == true &&
		cchDelete == 1 &&
		minSelLine == maxSelLine &&
		_MinimumSelection > _Lines[minSelLine])
	{
		fastDelete = true;
	}

	/*
	* Adjust UNDO fields so that we can undo this delete...
	*/
	if (_Undo.UndoType == UNDO_NONE)
	{
UNDODELETEFROMSCRATCH:
		_Undo.DeletedText = (mu_unicode*)mu_malloc((cchDelete + 1) * sizeof(mu_unicode));
		if (_Undo.DeletedText != nullptr)
		{
			mu_zeromem(_Undo.DeletedText, (cchDelete + 1) * sizeof(mu_unicode));
			_Undo.UndoType = UNDO_DELETE;
			_Undo.DeletedIndex = _MinimumSelection;
			_Undo.DeletedCount = cchDelete;
			lpDeleteSaveBuffer = _Undo.DeletedText;
			mu_memcpy(lpDeleteSaveBuffer, pedText + _MinimumSelection, cchDelete * sizeof(mu_unicode));
			lpDeleteSaveBuffer[cchDelete] = 0;
		}
	}
	else if (_Undo.UndoType & UNDO_INSERT)
	{
UNDODELETE:
		EmptyUndo(&_Undo);

		_Undo.InsertionStart = 0xFFFFFFFF;
		_Undo.InsertionEnd = 0xFFFFFFFF;
		_Undo.DeletedIndex = 0xFFFFFFFF;
		_Undo.DeletedCount = 0;
		goto UNDODELETEFROMSCRATCH;
	}
	else if (_Undo.UndoType == UNDO_DELETE)
	{
		if (_Undo.DeletedIndex == _MaximumSelection)
		{

			/*
			* Copy deleted text to front of undo buffer
			*/
			hDeletedText = (mu_unicode*)mu_realloc(_Undo.DeletedText, (cchDelete + _Undo.DeletedCount + 1) * sizeof(mu_unicode));
			if (!hDeletedText)
				goto UNDODELETE;
			bufferOffset = 0;
			_Undo.DeletedIndex = _MinimumSelection;
		}
		else if (_Undo.DeletedIndex == _MinimumSelection)
		{

			/*
			* Copy deleted text to end of undo buffer
			*/
			hDeletedText = (mu_unicode*)mu_realloc(_Undo.DeletedText, (mu_size)(cchDelete + _Undo.DeletedCount + 1) * sizeof(mu_unicode));
			if (!hDeletedText)
				goto UNDODELETE;
			bufferOffset = _Undo.DeletedCount;
		}
		else
		{

			/*
			* Clear the current UNDO delete and add the new one since
			the deletes aren't contiguous.
			*/
			goto UNDODELETE;
		}

		_Undo.DeletedText = hDeletedText;
		lpDeleteSaveBuffer = hDeletedText;
		if (!bufferOffset)
		{

			/*
			* Move text in delete buffer up so that we can insert the next
			* text at the head of the buffer.
			*/
			mu_memmove(lpDeleteSaveBuffer + cchDelete, lpDeleteSaveBuffer, _Undo.DeletedCount * sizeof(mu_unicode));
		}
		mu_memcpy(lpDeleteSaveBuffer + bufferOffset, pedText + _MinimumSelection, cchDelete * sizeof(mu_unicode));

		lpDeleteSaveBuffer[_Undo.DeletedCount + cchDelete] = 0;
		_Undo.DeletedCount += cchDelete;
	}

	if (_MaximumSelection != _Length)
	{

		/*
		* We are deleting text from the middle of the buffer so we have to
		shift text to the left.
		*/
		mu_memmove(pedText + _MinimumSelection, pedText + _MaximumSelection, (_Length - _MaximumSelection) * sizeof(mu_unicode));
	}

	if (_BufferLength - _Length > EXTRA_ALLOC)
	{

		/*
		* Free some memory since we deleted a lot
		*/
		_Buffer = (mu_unicode*)mu_realloc(_Buffer, (_Length + (EXTRA_ALLOC / 2)) * sizeof(mu_unicode));
		_BufferLength = (_Length + (EXTRA_ALLOC / 2));
	}

	_Length -= cchDelete;

	_CaretIndex = _MaximumSelection = _MinimumSelection;

	/*
	* Set dirty bit
	*/
	_IsDirty = true;

	mu_int32 hl = 0;
	if (fastDelete == true)
	{
		ShiftchLines(minSelLine + 1, -1);
		BuildLines(minSelLine, 1, true, nullptr, &hl);
	}
	else
	{
		BuildLines(EMath::Max(minSelLine - 1, 0), -(mu_int32)cchDelete, false, nullptr, nullptr);
	}

	UpdateCaretLine();

	return cchDelete;
}

mu_boolean UIMultiLineEditBox::InsertTextBuffer(const mu_unicode *lpText, mu_uint32 &cchInsert)
{
	if (cchInsert >= (_BufferLength - _Length))
	{
		mu_uint32 allocateLength = _Length + cchInsert + EXTRA_ALLOC;

		mu_unicode *tmpBuffer = (mu_unicode*)mu_realloc(_Buffer, allocateLength * sizeof(mu_unicode));

		if (tmpBuffer == nullptr)
			return false;

		mu_zeromem(&tmpBuffer[_Length + cchInsert], EXTRA_ALLOC * sizeof(mu_unicode));

		_Buffer = tmpBuffer;
		_BufferLength = allocateLength;
	}

	/*
	* Get a pointer to the place where text is to be inserted
	*/
	mu_unicode *textBuffer = _Buffer + _CaretIndex;

	if (_CaretIndex != _Length)
	{
		/*
		* We are inserting text into the middle. We have to shift text to the
		* right before inserting new text.
		*/
		mu_memmove(textBuffer + cchInsert, textBuffer, (_Length - _CaretIndex) * sizeof(mu_unicode));
	}

	/*
	* Make a copy of the text being inserted in the edit buffer.
	* Use this copy for doing UPPERCASE/LOWERCASE ANSI/OEM conversions
	*/
	mu_memmove(textBuffer, lpText, cchInsert * sizeof(mu_unicode));
	_Length += cchInsert;

	if (ContainsStyle(UES_LOWERCASE) == true)
	{
		mu_unicode *tmp = textBuffer;
		for (mu_uint32 left = cchInsert; left > 0; --left)
		{
			*tmp = mu_tolower(*tmp);
		}
	}
	else if (ContainsStyle(UES_UPPERCASE) == true)
	{
		mu_unicode *tmp = textBuffer;
		for (mu_uint32 left = cchInsert; left > 0; --left)
		{
			*tmp = mu_toupper(*tmp);
		}
	}

	/* Adjust UNDO fields so that we can undo this insert... */
	MergeUndoInsertInfo(_CaretIndex, cchInsert);

	_CaretIndex += cchInsert;

	_MinimumSelection = _MaximumSelection = _CaretIndex;

	/* Processing lines */

	/*
	* Set dirty bit
	*/
	_IsDirty = true;

	return true;
}

mu_uint32 UIMultiLineEditBox::InsertText(const mu_unicode *lpText, mu_uint32 cchInsert, mu_boolean userTyping)
{
	mu_unicode *pText;
	mu_uint32 cchInsertCopy = cchInsert;
	mu_uint32 oldCaret = _CaretIndex;
	mu_uint32 oldCaretLine = _CaretLine;
	mu_uint32 cchT;
	EVector2 size;
	EPoint xyPosInitial(0, 0);
	EPoint xyPosFinal(0, 0);

	if (cchInsertCopy == 0)
	{
		return 0;
	}

	/*
	* Limit the amount of text we add
	*/
	cchInsertCopy = EMath::Min(cchInsertCopy, _MaxLength - _Length);

	if (cchInsertCopy == 0)
	{
		return 0;
	}

	UIUndoBuffer undo = { 0 };
	SaveUndo(&_Undo, &undo, !_AutoVScroll);

	if (_Length > 0)
	{
		IchToXYPos(_Length - 1, false, xyPosInitial);
	}

	/*
	* Insert the text
	*/
	mu_uint32 validCchTemp = cchInsertCopy;
	if (InsertTextBuffer(lpText, validCchTemp) == false)
	{
		if (_AutoVScroll == false)
		{
			SaveUndo(&undo, &_Undo, false);
		}

		return 0;
	}

	mu_int32 ll = 0;
	mu_int32 hl = 0;
	BuildLines(oldCaretLine, (mu_int32)cchInsertCopy, userTyping, &ll, &hl);

	if (_Length > 0)
	{
		IchToXYPos(_Length - 1, false, xyPosFinal);
	}

	if (_AutoVScroll == false)
	{
		if (_DisplayLinesCount < _LinesCount)
		{
			Undo();
			EmptyUndo(&_Undo);
			SaveUndo(&undo, &_Undo, false);
			return 0;
		}
		else
		{
			EmptyUndo(&undo);
		}
	}

	if (userTyping == true &&
		_WrapMode == true)
	{
		//
		// To avoid oldCaret points intermediate of DBCS character,
		// adjust oldCaret position if necessary.
		//
		// !!!CR If MLBuildchLines() returns reasonable value ( and I think
		//       it does), we don't probably need this. Check this out later.
		//
		oldCaret = EMath::Min((mu_uint32)ll, oldCaret);
	}

	UpdateCaretLine();
	EnsureCaretVisible();

	_IsDirty = true;

	return cchInsertCopy;
}

mu_uint32 UIMultiLineEditBox::PasteText()
{
	if (SDL_HasClipboardText() == SDL_FALSE)
		return 0;

	mu_char *clipboardData = SDL_GetClipboardText();

	if (clipboardData == nullptr)
		return 0;
	
	mu_asciistr clipboardString(clipboardData);
	SDL_free(clipboardData);

	mu_uint32 cchAdded = 0;
	mu_size carrierIndex = clipboardString.find('\r');

	if (carrierIndex != mu_asciistr::npos)
	{
		mu_string string = boost::locale::conv::utf_to_utf<mu_text>(clipboardString.substr(0, carrierIndex));
		cchAdded = InsertText(string.c_str(), string.size(), false);
	}
	else
	{
		mu_string string = boost::locale::conv::utf_to_utf<mu_text>(clipboardString.substr(0, carrierIndex));
		cchAdded = InsertText(string.c_str(), string.size(), false);
	}

	return cchAdded;
}

void UIMultiLineEditBox::ClearText()
{
	if (_ReadOnly == false &&
		_MinimumSelection < _MaximumSelection)
	{
		DeleteText();
	}
}

void UIMultiLineEditBox::ClearAllText()
{
	mu_boolean isReadOnly = IsReadOnly();
	SetReadOnly(false);
	SelectAll();
	ClearText();
	SetReadOnly(isReadOnly);
}

void UIMultiLineEditBox::ResetTextInfo()
{
	//
	// Reset caret, selections, scrolling, and dirty information.
	//
	_MinimumSelection = _MaximumSelection = 0;
	_DisplayIndexX = 0;
	_DisplayIndexY = 0.0f;
	_IsDirty = true;

	EmptyUndo(&_Undo);
}

mu_uint32 UIMultiLineEditBox::CharactersInWidth(const mu_unicode *text, mu_uint32 charCount, mu_float width, mu_boolean isForward)
{
	mu_float stringExtent;
	mu_int32 cchhigh;
	mu_int32 cchnew = 0;
	mu_int32 cchlow = 0;
	EVector2 size;
	const mu_unicode *lpStart;

	if (width <= 0 || !charCount)
		return 0;

	/*
	* ALWAYS RESTRICT TO AT MOST MAXLINELENGTH to avoid overflow...
	*/
	charCount = EMath::Min(MAXLINELENGTH, charCount);

	cchhigh = charCount + 1;
	while (cchlow < cchhigh - 1)
	{
		cchnew = EMath::Max((cchhigh - cchlow) / 2, 1) + cchlow;

		lpStart = text;

		/*
		* If we want to figure out how many fit starting at the end and moving
		* backwards, make sure we move to the appropriate position in the
		* string before calculating the text extent.
		*/
		if (!isForward)
			lpStart += (charCount - cchnew);

		_Font->GetTextExtentPoint(lpStart, cchnew, ETA_LEFT, 1.0f, &size);
		stringExtent = size.x;

		if (stringExtent > width)
		{
			cchhigh = cchnew;
		}
		else
		{
			cchlow = cchnew;
		}
	}

	return (cchlow);
}

mu_boolean UIMultiLineEditBox::ScrollText(mu_boolean forceCaretLine)
{
	mu_unicode *pTextScreenStart;
	mu_uint32 scrollAmount;
	mu_uint32 newScreenStartX = _DisplayIndexX;
	mu_uint32 cch;
	mu_boolean fAdjustNext = false;

	if (_AutoVScroll)
	{
		mu_boolean isScrollEnabled = _VerticalScroll == nullptr ? false : _VerticalScroll->GetState() != UIScrollBar::UI_DISABLED;
		mu_float oldScrollValue = _VerticalScroll == nullptr ? 0.0f : _VerticalScroll->GetCurrentValue();
		mu_float oldScrollMax = _VerticalScroll == nullptr ? 0.0f : _VerticalScroll->GetMaxValue();

		if(forceCaretLine == true)
		{
			if (_CaretLine < (mu_uint32)_DisplayIndexY)
			{
				_DisplayIndexY = _CaretLine;
				_IsDirty = true;
			}
			else if (_CaretLine >= (mu_uint32)_DisplayIndexY + _DisplayLinesCount)
			{
				_DisplayIndexY = _CaretLine - _DisplayLinesCount + 1;
				_IsDirty = true;
			}
		}

		if (_LinesCount < _DisplayLinesCount)
		{
			if ((mu_uint32)_DisplayIndexY > 0)
			{
				_DisplayIndexY = 0;
				_IsDirty = true;
			}
		}
		else
		{
			if ((mu_uint32)_DisplayIndexY > _LinesCount - _DisplayLinesCount)
			{
				_DisplayIndexY = _LinesCount - _DisplayLinesCount;
			}
		}

		if(_VerticalScroll != nullptr)
		{
			if (isScrollEnabled == false &&
				_DisplayLinesCount < _LinesCount)
			{
				_VerticalScroll->ChangeState(UIScrollBar::UI_NORMAL);
				_VerticalScroll->ConfigureScrollInfo(0.0f, _LinesCount - _DisplayLinesCount, 0.0f);
			}
			else if (isScrollEnabled == true &&
				_DisplayLinesCount >= _LinesCount)
			{
				_VerticalScroll->ChangeState(UIScrollBar::UI_DISABLED);
				_VerticalScroll->ConfigureScrollInfo(0.0f, 1.0f, 1.0f);
			}
			else if (isScrollEnabled == true)
			{
				if (oldScrollValue != _DisplayIndexY ||
					(mu_uint32)oldScrollMax != _LinesCount - _DisplayLinesCount)
				{
					_VerticalScroll->ConfigureScrollInfo(0.0f, _LinesCount - _DisplayLinesCount, _DisplayIndexY);
				}
			}
		}
	}

	if (_AutoHScroll)
	{
		mu_boolean isScrollEnabled = _HorizontalScroll == nullptr ? false : _HorizontalScroll->GetState() != UIScrollBar::UI_DISABLED;
		mu_float oldScrollValue = _DisplayIndexY;
		mu_float oldScrollMax = _HorizontalScroll == nullptr ? 0.0f : _HorizontalScroll->GetMaxValue();

		mu_uint32 caretBase = _Lines[_CaretLine];
		mu_uint32 caretIndexX = _CaretIndex < caretBase ? caretBase : _CaretIndex - caretBase;
		mu_uint32 caretIndexY = _CaretLine;

		/*
		* Calculate the new starting screen position
		*/
		if (caretIndexX <= _DisplayIndexX)
		{
			/*
			* Caret is to the left of the starting text on the screen we must
			* scroll the text backwards to bring it into view. Watch out when
			* subtracting unsigned numbers when we have the possibility of going
			* negative.
			*/
			pTextScreenStart = _Buffer + caretBase;

			scrollAmount = CharactersInWidth(pTextScreenStart, caretIndexX, _Width / 4.0f, false);

			newScreenStartX = caretIndexX - scrollAmount;
		}
		else if (caretIndexX != _DisplayIndexX)
		{
			mu_uint32 lineLength = GetLineLength(caretIndexY);

			if (_DisplayIndexX > lineLength)
				pTextScreenStart = _Buffer + _Lines[(mu_uint32)caretIndexY] + lineLength;
			else
				pTextScreenStart = _Buffer + _Lines[(mu_uint32)caretIndexY] + _DisplayIndexX;

			cch = CharactersInWidth(pTextScreenStart, caretIndexX - _DisplayIndexX, _Width, false);

			if (cch < caretIndexX - _DisplayIndexX)
			{
				fAdjustNext = true;

				/*
				* Scroll Forward 1/4 -- if that leaves some empty space
				* at the end, scroll back enough to fill the space
				*/
				newScreenStartX = caretIndexX - (3 * cch / 4);

				cch = CharactersInWidth(pTextScreenStart, lineLength - _DisplayIndexX, _Width, false);

				if (newScreenStartX > (lineLength - cch))
					newScreenStartX = lineLength - cch;
			}
			else if (ContainsStyle(UES_CENTER | UES_RIGHT) == true)
			{
				cch = CharactersInWidth(pTextScreenStart, lineLength - _DisplayIndexX, _Width, false);

				/*
				* Scroll the text hidden behind the left border back
				* into view.
				*/
				if (_DisplayIndexX == lineLength - cch)
				{
					pTextScreenStart -= _DisplayIndexX;
					cch = CharactersInWidth(pTextScreenStart, lineLength, _Width, false);

					newScreenStartX = lineLength - cch;
				}
			}
		}

#if 0 // FIX // Add support for horizontal scrolling, try to emulate floating display index at horizontal, will be a lot better
		if (_HorizontalScroll != nullptr)
		{
			if (isScrollEnabled == false &&
				_DisplayLinesCount < _LinesCount)
			{
				_VerticalScroll->ChangeState(UIScrollBar::UI_NORMAL);
				_VerticalScroll->ConfigureScrollInfo(0.0f, _LinesCount - _DisplayLinesCount, 0.0f);
			}
			else if (isScrollEnabled == true &&
				_DisplayLinesCount >= _LinesCount)
			{
				_VerticalScroll->ChangeState(UIScrollBar::UI_DISABLED);
				_VerticalScroll->ConfigureScrollInfo(0.0f, 0.0f, 0.0f);
			}
			else if (isScrollEnabled == true)
			{
				if (oldScrollValue != _DisplayIndexY ||
					(mu_uint32)oldScrollMax != _LinesCount - _DisplayLinesCount)
				{
					_VerticalScroll->ConfigureScrollInfo(0.0f, _LinesCount - _DisplayLinesCount, _DisplayIndexY);
				}
			}
		}
#endif

		if (_DisplayIndexX != newScreenStartX)
		{
			_DisplayIndexX = newScreenStartX;
			_IsDirty = true;

			// Caret pos is set by DrawText().
			return true;
		}
	}

	return false;
}

void UIMultiLineEditBox::SelectAll()
{
	SetSelection(0, GetLength());
}

void UIMultiLineEditBox::ChangeSelection(mu_uint32 ichNewMinSel, mu_uint32 ichNewMaxSel)
{
	mu_uint32 temp;
	mu_uint32 ichOldMinSel;
	mu_uint32 ichOldMaxSel;

	if (ichNewMinSel > ichNewMaxSel)
	{
		temp = ichNewMinSel;
		ichNewMinSel = ichNewMaxSel;
		ichNewMaxSel = temp;
	}
	ichNewMinSel = EMath::Min(ichNewMinSel, _Length);
	ichNewMaxSel = EMath::Min(ichNewMaxSel, _Length);

	/*
	* Preserve the Old selection
	*/
	ichOldMinSel = _MinimumSelection;
	ichOldMaxSel = _MaximumSelection;

	/*
	* Set new selection
	*/
	_MinimumSelection = ichNewMinSel;
	_MaximumSelection = ichNewMaxSel;

	/*
	* We will find the intersection of current selection rectangle with the new
	* selection rectangle. We will then invert the parts of the two rectangles
	* not in the intersection.
	*/
	if (IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false && (_IsFocus || _NoHideSel))
	{
		//
		// Update text and caret.
		//
		_IsDirty = true;
	}
}

void UIMultiLineEditBox::SetSelection(mu_uint32 ichSelStart, mu_uint32 ichSelEnd)
{
	if (ichSelStart == (mu_uint32)-1)
	{
		/*
		* Set no selection if we specify -1
		*/
		ichSelStart = ichSelEnd = _CaretIndex;
	}

	/*
	* Bounds ichSelStart, ichSelEnd are checked in SLChangeSelection...
	*/
	ChangeSelection(ichSelStart, ichSelEnd);

	/*
	* Put the caret at the end of the selected text
	*/
	_CaretIndex = _MaximumSelection;
	UpdateCaretLine();

	SetCaretPosition();

	/*
	* We may need to scroll the text to bring the caret into view...
	*/
	ScrollText();
}

void UIMultiLineEditBox::SetCaretPosition()
{
	mu_int32 xPosition;

	/*
	* We will only position the caret if we have the focus since we don't want
	* to move the caret while another window could own it.
	*/
	_UpdateCaret = true;

	g_CaretTime = g_CurrentTime;
	g_CaretEnabled = true;
}

void UIMultiLineEditBox::UpdateCaretLine()
{
	mu_unicode *pText;

	_CaretLine = IchToLine(_CaretIndex);

	/*
	* If caret gets to beginning of next line, pop it up to end of current line
	* when inserting text.
	*/
	pText = _Buffer + (_CaretIndex - 1);
	if (_CaretLine &&
		_Lines[_CaretLine] == _CaretIndex &&
		pText[0] != '\n')
		--_CaretLine;
}

void UIMultiLineEditBox::EnsureCaretVisible()
{
	ScrollText();
}

void UIMultiLineEditBox::DrawText()
{
	mu_unicode *pText = _Buffer;
	mu_uint32 displayLineIndex = (mu_uint32)_DisplayIndexY;
	mu_uint32 displayLines = EMath::Min(_LinesCount - displayLineIndex, _DisplayLinesCount);

	/*
	* There is no selection if,
	* 1. MinSel and MaxSel are equal OR
	* 2. (This has lost the focus AND Selection is to be hidden)
	*/
	mu_boolean noSelection = ((_MinimumSelection == _MaximumSelection) ||
		(!_IsFocus && !_NoHideSel) ||
		(_MaximumSelection < GetLineIndex(displayLineIndex)) ||
		(_MinimumSelection >= GetLineIndex(displayLineIndex + displayLines)));

	mu_unicode colorBuffer[32];
	mu_unicode backBuffer[32];
	mu_unicode highlightBuffer[32];
	mu_unicode highlightTextBuffer[32];
	mu_sprintf_s(colorBuffer, mu_countof(colorBuffer), _T("\uE000%08X\uE000"), _TextColor);
	mu_sprintf_s(backBuffer, mu_countof(backBuffer), _T("\uE001%08X\uE001"), _BackColor);
	mu_sprintf_s(highlightBuffer, mu_countof(highlightBuffer), _T("\uE001%08X\uE001"), _HighlightColor);
	mu_sprintf_s(highlightTextBuffer, mu_countof(highlightTextBuffer), _T("\uE000%08X\uE000"), _HighlightTextColor);

	if (noSelection)
	{
		_DrawString = colorBuffer;
		_DrawString += backBuffer;

		for (mu_uint32 n = 0; n < displayLines; ++n)
		{
			mu_uint32 startIndex = GetLineIndex(n + displayLineIndex) + _DisplayIndexX;
			mu_uint32 endIndex = GetLineIndex(n + displayLineIndex + 1);

			if (startIndex < endIndex)
			{
				/*
				* Find out how many characters will fit on the screen so that we don't do
				* any needless drawing.
				*/
				mu_uint32 cchToDraw = CharactersInWidth(pText + startIndex, GetLineLength(n + displayLineIndex) - _DisplayIndexX, _Width, true);

				if (_Buffer[endIndex - 1] == _T('\n')) --endIndex;

				mu_uint32 drawCount = endIndex - startIndex;
				if (drawCount > cchToDraw) drawCount = cchToDraw;
				_DrawString += mu_string(_Buffer + startIndex, drawCount);
			}

			if (n < displayLines - 1)
			{
				_DrawString += _T('\n');
			}
		}
	}
	else
	{
		mu_uint32 minSelLine = IchToLine(_MinimumSelection);
		mu_uint32 maxSelLine = IchToLine(_MaximumSelection);

		if (minSelLine < displayLineIndex)
		{
			_DrawString = highlightTextBuffer;
			_DrawString += highlightBuffer;

			if (maxSelLine >= displayLineIndex + displayLines)
			{
				for (mu_uint32 n = 0; n < displayLines; ++n)
				{
					mu_uint32 startIndex = GetLineIndex(n + displayLineIndex) + _DisplayIndexX;
					mu_uint32 endIndex = GetLineIndex(n + displayLineIndex + 1);

					if (startIndex < endIndex)
					{
						/*
						* Find out how many characters will fit on the screen so that we don't do
						* any needless drawing.
						*/
						mu_uint32 cchToDraw = CharactersInWidth(pText + startIndex, GetLineLength(n + displayLineIndex) - _DisplayIndexX, _Width, true);

						if (_Buffer[endIndex - 1] == _T('\n')) --endIndex;

						mu_uint32 drawCount = endIndex - startIndex;
						if (drawCount > cchToDraw) drawCount = cchToDraw;
						_DrawString += mu_string(_Buffer + startIndex, drawCount);
					}

					if (n < displayLines - 1)
					{
						_DrawString += _T('\n');
					}
				}
			}
			else
			{
				mu_boolean isInsideHighlight = true;
				for (mu_uint32 n = 0; n < displayLines; ++n)
				{
					mu_uint32 startIndex = GetLineIndex(n + displayLineIndex) + _DisplayIndexX;
					mu_uint32 endIndex = GetLineIndex(n + displayLineIndex + 1);

					if (startIndex < endIndex)
					{
						/*
						* Find out how many characters will fit on the screen so that we don't do
						* any needless drawing.
						*/
						mu_uint32 cchToDraw = CharactersInWidth(pText + startIndex, GetLineLength(n + displayLineIndex) - _DisplayIndexX, _Width, true);

						if (_Buffer[endIndex - 1] == _T('\n')) --endIndex;

						mu_uint32 drawCount = endIndex - startIndex;
						if (drawCount > cchToDraw) drawCount = cchToDraw;

						if (isInsideHighlight == false)
						{
							_DrawString += mu_string(_Buffer + startIndex, drawCount);
						}
						else
						{
							if (endIndex + 1 <= _MaximumSelection)
							{
								_DrawString += mu_string(_Buffer + startIndex, drawCount);
							}
							else
							{
								isInsideHighlight = false;
								mu_uint32 selectCount = _MaximumSelection - startIndex;

								if (selectCount >= drawCount)
								{
									_DrawString += mu_string(_Buffer + startIndex, drawCount);
									_DrawString += colorBuffer;
									_DrawString += backBuffer;
								}
								else
								{
									_DrawString += mu_string(_Buffer + startIndex, selectCount);
									_DrawString += colorBuffer;
									_DrawString += backBuffer;
									_DrawString += mu_string(_Buffer + startIndex + selectCount, drawCount - selectCount);
								}
							}
						}
					}

					if (n < displayLines - 1)
					{
						_DrawString += _T('\n');
					}
				}
			}
		}
		else
		{
			_DrawString = colorBuffer;
			_DrawString += backBuffer;

			mu_boolean isInsideHighlight = false;
			mu_boolean isCompletedHighlight = false;
			for (mu_uint32 n = 0; n < displayLines; ++n)
			{
				mu_uint32 startIndex = GetLineIndex(n + displayLineIndex) + _DisplayIndexX;
				mu_uint32 endIndex = GetLineIndex(n + displayLineIndex + 1);

				if (startIndex < endIndex)
				{
					/*
					* Find out how many characters will fit on the screen so that we don't do
					* any needless drawing.
					*/
					mu_uint32 cchToDraw = CharactersInWidth(pText + startIndex, GetLineLength(n + displayLineIndex) - _DisplayIndexX, _Width, true);

					if (_Buffer[endIndex - 1] == _T('\n')) --endIndex;

					mu_uint32 drawCount = endIndex - startIndex;
					if (drawCount > cchToDraw) drawCount = cchToDraw;

					if (isCompletedHighlight == true)
					{
						_DrawString += mu_string(_Buffer + startIndex, drawCount);
					}
					else
					{
						if ((isInsideHighlight == false &&
							endIndex + 1 <= _MinimumSelection) ||
							(isInsideHighlight == true &&
								endIndex + 1 <= _MaximumSelection))
						{
							_DrawString += mu_string(_Buffer + startIndex, drawCount);
						}
						else if (isInsideHighlight == false)
						{
							isInsideHighlight = true;
							mu_uint32 unselectCount = _MinimumSelection >= startIndex ? _MinimumSelection - startIndex : 0;

							if (unselectCount >= drawCount)
							{
								_DrawString += mu_string(_Buffer + startIndex, drawCount);
								_DrawString += highlightTextBuffer;
								_DrawString += highlightBuffer;
							}
							else
							{
								_DrawString += mu_string(_Buffer + startIndex, unselectCount);
								_DrawString += highlightTextBuffer;
								_DrawString += highlightBuffer;

								if (endIndex + 1 >= _MaximumSelection)
								{
									mu_uint32 selectCount = _MaximumSelection - startIndex - unselectCount;

									if (selectCount >= drawCount - unselectCount)
									{
										isCompletedHighlight = true;
										_DrawString += mu_string(_Buffer + startIndex + unselectCount, drawCount - unselectCount);
										_DrawString += colorBuffer;
										_DrawString += backBuffer;
									}
									else
									{
										_DrawString += mu_string(_Buffer + startIndex + unselectCount, selectCount);
										_DrawString += colorBuffer;
										_DrawString += backBuffer;
										_DrawString += mu_string(_Buffer + startIndex + unselectCount + selectCount, drawCount - unselectCount - selectCount);
									}
								}
								else
								{
									_DrawString += mu_string(_Buffer + startIndex + unselectCount, drawCount - unselectCount);
								}
							}
						}
						else
						{
							isCompletedHighlight = true;
							mu_uint32 selectCount = _MaximumSelection < startIndex ? 0 : _MaximumSelection - startIndex;

							if (selectCount >= drawCount)
							{
								_DrawString += mu_string(_Buffer + startIndex, drawCount);
								_DrawString += colorBuffer;
								_DrawString += backBuffer;
							}
							else
							{
								_DrawString += mu_string(_Buffer + startIndex, selectCount);
								_DrawString += colorBuffer;
								_DrawString += backBuffer;
								_DrawString += mu_string(_Buffer + startIndex + selectCount, drawCount - selectCount);
							}
						}
					}
				}

				if (n < displayLines - 1)
				{
					_DrawString += _T('\n');
				}
			}
		}
	}

	SetCaretPosition();
}

void UIMultiLineEditBox::SetFont(MUTextSimpleRender *font)
{
	if (font == nullptr) return;

	_Font = font;

	_LineHeight = _Font->GetFont()->_MaxAdvanceY;
	_DisplayLinesCount = (mu_uint32)EMath::Abs(_Height / _LineHeight);

	_Caret->SetDimensions(1.0f, _Font->GetFont()->_MaxAdvanceY, false);
	_UpdateCaret = true;
	_IsDirty = true;
}

mu_boolean UIMultiLineEditBox::OnGFxMessage(mu_uint32 objectid, mu_uint32 message, mu_size message_data)
{
	if (_VerticalScroll != nullptr &&
		_VerticalScroll->GetObjectID() == objectid)
	{
		switch (message)
		{
		case GFX_MESSAGE_TYPE::GFX_SLIDE:
			{
				_DisplayIndexY = _VerticalScroll->GetCurrentValue();
				_IsDirty = true;
			}
			return true;
		}
	}
	else if (_HorizontalScroll != nullptr &&
		_HorizontalScroll->GetObjectID() == objectid)
	{
		switch (message)
		{
		case GFX_MESSAGE_TYPE::GFX_SLIDE:
			{
				_DisplayIndexX = _HorizontalScroll->GetCurrentValue();
				_IsDirty = true;
			}
			return true;
		}
	}

	return false;
}

void UIMultiLineEditBox::OnMouseMove(mu_int32 mouse_x, mu_int32 mouse_y)
{
	if (_IsMouseDown == false) return;

	mu_uint32   selectionl;
	mu_uint32   selectionh;
	mu_uint32   newMaxSel;
	mu_uint32   newMinSel;
	mu_uint32   mouseIch;
	mu_unicode  *pText;

	newMinSel = _MinimumSelection;
	newMaxSel = _MaximumSelection;

	mouseIch = MouseToIch(mouse_x, mouse_y);

	// Extend selection, move caret word right
	if (_StartMinimumSelection || _StartMaximumSelection)
	{
		// We're in WORD SELECT mode
		mu_boolean fReverse = (mouseIch <= _StartMinimumSelection);

		Word(mouseIch, !fReverse, &selectionl, &selectionh);

		mu_int32 wordLineLow = IchToLine(selectionl);
		mu_int32 worldLineHigh = IchToLine(selectionh);

		if (wordLineLow != worldLineHigh)
		{
			--selectionh;
		}

		if (fReverse)
		{
			newMinSel = _CaretIndex = selectionl;
			newMaxSel = _StartMaximumSelection;
		}
		else
		{
			newMinSel = _StartMinimumSelection;
			newMaxSel = _CaretIndex = selectionh;
		}
	}
	else if ((_MinimumSelection == _CaretIndex) &&
		(_MinimumSelection != _MaximumSelection))
		// Reduce selection extent
		newMinSel = _CaretIndex = mouseIch;
	else
		// Extend selection extent
		newMaxSel = _CaretIndex = mouseIch;

	UpdateCaretLine();

	ScrollText();
	ChangeSelection(newMinSel, newMaxSel);
}

void UIMultiLineEditBox::OnMouseOver(mu_int32 mouse_x, mu_int32 mouse_y)
{

}

void UIMultiLineEditBox::OnMouseLeave()
{

}

void UIMultiLineEditBox::MousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys, EPoint mousePt)
{
	mu_int32 mouseX = mousePt.x, mouseY = mousePt.y;

	mu_uint32   selectionl;
	mu_uint32   selectionh;
	mu_boolean  changeSelection;
	mu_uint32   newMaxSel;
	mu_uint32   newMinSel;
	mu_uint32   mouseIch;
	mu_unicode  *pText;

	changeSelection = false;

	newMinSel = _MinimumSelection;
	newMaxSel = _MaximumSelection;

	mouseIch = MouseToIch(mouseX, mouseY);

	if (MUMisc::IsFlag(double_flags, MOUSE_FLAG_LEFT))
	{
		// if shift key is down, extend selection to word we double clicked on
		// else clear current selection and select word.

		Word(mouseIch, (mouseIch) ? true : false, &selectionl, &selectionh);

		mu_int32 wordLineLow = IchToLine(selectionl);
		mu_int32 worldLineHigh = IchToLine(selectionh);

		if (wordLineLow != worldLineHigh)
		{
			--selectionh;
		}

		if (MUMisc::IsFlag(virtual_keys, VFLAG_SHIFT) == false)
		{
			// If shift key isn't down, move caret to mouse point and clear
			// old selection
			newMinSel = selectionl;
			newMaxSel = _CaretIndex = selectionh;
		}
		else
		{
			// Shiftkey is down so we want to maintain the current selection
			// (if any) and just extend or reduce it
			if (_MinimumSelection == _CaretIndex)
			{
				newMinSel = _CaretIndex = selectionl;
				Word(newMaxSel, true, &selectionl, &selectionh);
			}
			else
			{
				newMaxSel = _CaretIndex = selectionh;
				Word(newMinSel, false, &selectionl, &selectionh);
			}
			/*
			* v-ronaar: fix bug 24627 - edit selection is weird.
			*/
			_MaximumSelection = _CaretIndex;
		}

		_StartMinimumSelection = selectionl;
		_StartMaximumSelection = selectionh;

		UpdateCaretLine();

		goto InitDragSelect;
	}
	else if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_LEFT))
	{
		// If we currently don't have the focus yet, try to get it.
		if (!_IsFocus)
		{
			if (!_NoHideSel)
			{
				// Clear the selection before setting the focus so that we
				// don't get refresh problems and flicker. Doesn't matter
				// since the mouse down will end up changing it anyway.
				_MinimumSelection = _MaximumSelection = _CaretIndex;
			}

			newMaxSel = _MaximumSelection;
			newMinSel = _MinimumSelection;
			mouseIch = EMath::Min(mouseIch, _Length);
		}

		if (_IsFocus)
		{
			// Only do this if we have the focus since a clever app may not
			// want to give us the focus at the SetFocus call above.
			if (!MUMisc::IsFlag(virtual_keys, VFLAG_SHIFT))
			{
				// If shift key isn't down, move caret to mouse point and
				// clear old selection
				newMinSel = newMaxSel = _CaretIndex = mouseIch;
			}
			else
			{
				// Shiftkey is down so we want to maintain the current
				// selection (if any) and just extend or reduce it
				if (_MinimumSelection == _CaretIndex)
					newMinSel = _CaretIndex = mouseIch;
				else
					newMaxSel = _CaretIndex = mouseIch;
			}

			_StartMinimumSelection = _StartMaximumSelection = 0;
			_IsMouseDown = true;

			UpdateCaretLine();

InitDragSelect:
			changeSelection = true;
		}
	}

	if (changeSelection)
	{
		ScrollText();
		ChangeSelection(newMinSel, newMaxSel);
	}
}

void UIMultiLineEditBox::OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys)
{
	MousePressed(mouse_flags, double_flags, virtual_keys, EPoint(MU_INPUT->GetMouseX(), MU_INPUT->GetMouseY()));
}

void UIMultiLineEditBox::MouseReleased(mu_uint32 mouse_flags, mu_uint32 virtual_keys, EPoint mousePt)
{
	if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_LEFT))
	{
		_IsMouseDown = false;
	}
}

void UIMultiLineEditBox::OnMouseReleased(mu_uint32 mouse_flags, mu_uint32 virtual_keys)
{
	MouseReleased(mouse_flags, virtual_keys, EPoint(MU_INPUT->GetMouseX(), MU_INPUT->GetMouseY()));
}

void UIMultiLineEditBox::OnMouseCancelled(mu_uint32 mouse_flags)
{
	if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_LEFT))
	{
		_IsMouseDown = false;
	}
}

void UIMultiLineEditBox::OnMouseWheel(mu_float mouse_wheel)
{
	
}

mu_uint32 UIMultiLineEditBox::MouseToIch(mu_int32 mouseX, mu_int32 mouseY)
{
	mu_unicode *pText;
	mu_uint32 cch;

	if (_Length == 0)
		return 0;

	mu_uint32 line = (mu_uint32)_DisplayIndexY;
	mu_float mouseYF = (mu_float)mouseY;
	if (mouseYF <= _NativeY)
	{
		if (line == 0)
		{
			line = 0;
		}
		else
		{
			line = EMath::Max(0u, line - 1);
		}
	}
	else if (mouseYF > _NativeY + _Height)
	{
		line = EMath::Min(line + _DisplayLinesCount, _LinesCount - 1);
	}
	else
	{
		line = EMath::Min(line + (mu_uint32)(EMath::Floor(mouseYF - _NativeY) / _LineHeight), _LinesCount - 1);
	}

	if (line >= _LinesCount) line = _LinesCount - 1;

	pText = _Buffer + _Lines[line];
	mu_uint32 cLineLength = GetLineLength(line);

	if (_DisplayIndexX >= cLineLength)
	{
		return _Lines[line] + cLineLength;
	}

	if (mouseX <= _NativeX)
	{
		/*
		* Return either the first non visible character or return 0 if at
		* beginning of text
		*/
		if (_DisplayIndexX)
			return (_Lines[line] + _DisplayIndexX - 1);
		else
			return _Lines[line];
	}

	if (mouseX > _NativeX + _Width)
	{
		mu_uint32 cchIndex = _Lines[line] + cLineLength;
		if (_Buffer[cchIndex - 1] == _T('\n'))
			return cchIndex - 1;
		return cchIndex;
	}

	mu_uint8 horizontalAlign = ETA_LEFT;

	if (ContainsStyle(UES_RIGHT) == true)
	{
		horizontalAlign = ETA_RIGHT;
	}
	else if (ContainsStyle(UES_CENTER) == true)
	{
		horizontalAlign = ETA_CENTER;
	}

	mu_string lineText(pText + _DisplayIndexX, cLineLength - _DisplayIndexX);
	cch = _Font->GetCharacterIndexByMouse(_RenderIndex, (mu_float)mouseX, mouseY + _LineHeight * 0.5f, _DrawString.c_str(), horizontalAlign) + _DisplayIndexX;

	return _Lines[(mu_uint32)_DisplayIndexY] + cch;
}

// Copy & Paste functions
mu_boolean UIMultiLineEditBox::Undo()
{
	mu_unicode *hDeletedText = _Undo.DeletedText;
	mu_boolean fDelete = (_Undo.UndoType & UNDO_DELETE) != 0;
	mu_uint32 cchDeleted = _Undo.DeletedCount;
	mu_uint32 ichDeleted = _Undo.DeletedIndex;

	if (_Undo.UndoType == UNDO_NONE)
	{
		/*
		* No undo...
		*/
		return false;
	}

	_Undo.DeletedText = nullptr;
	_Undo.DeletedCount = 0;
	_Undo.DeletedIndex = (mu_uint32)-1;
	_Undo.UndoType &= ~UNDO_DELETE;

	if (_Undo.UndoType == UNDO_INSERT)
	{
		_Undo.UndoType = UNDO_NONE;

		/*
		* Set the selection to the inserted text
		*/
		SetSelection(_Undo.InsertionStart, _Undo.InsertionEnd);
		_Undo.InsertionStart = _Undo.InsertionEnd = (mu_uint32)-1;

		/*
		* Delete the selected text and save it in undo buff.
		* Call ECDeleteText() instead of sending a VK_BACK message
		* which results in an EN_UPDATE notification send even before
		* we insert the deleted chars. This results in Bug #6610.
		* Fix for Bug #6610 -- SANKAR -- 04/19/91 --
		*/
		if (DeleteText())
		{
			/*
			* Text was deleted -- flag for update and clear selection
			*/
			SetSelection(ichDeleted, ichDeleted);
		}
	}

	if (fDelete)
	{
		/*
		* Insert deleted chars. Set the selection to the inserted text.
		*/
		SetSelection(ichDeleted, ichDeleted);
		InsertText(hDeletedText, cchDeleted, false);
		mu_free(hDeletedText);

		SetSelection(ichDeleted, ichDeleted + cchDeleted);
	}

	return true;
}

void UIMultiLineEditBox::Cut()
{
	if (_ReadOnly == false &&
		_MinimumSelection < _MaximumSelection &&
		Copy() > 0)
	{
		ClearText();
	}
}

mu_uint32 UIMultiLineEditBox::Copy()
{
	mu_uint32 cbData;

	cbData = (_MaximumSelection - _MinimumSelection);

	if (!cbData)
		return 0;

	mu_uint32 minBufferLength = cbData + _LinesCount * 2 + 128;

	if (s_TextBufferLength < minBufferLength)
	{
		s_TextBufferLength = minBufferLength;
		s_TextBuffer = (mu_unicode *)mu_realloc(s_TextBuffer, sizeof(mu_unicode) * s_TextBufferLength);
	}

	mu_int32 minLine = IchToLine(_MinimumSelection);
	mu_int32 maxLine = IchToLine(_MaximumSelection);

	if (minLine == maxLine)
	{
		/*
		* +1 for the terminating nullptr
		*/
		mu_unicode *pchSel = _Buffer + _MinimumSelection;
		if (SDL_SetClipboardText(boost::locale::conv::utf_to_utf<mu_char>(mu_string(pchSel, cbData)).c_str()) != 0)
			return 0;
	}
	else
	{
		mu_uint32 bufferIndex = 0;
		mu_uint32 linesCount = maxLine - minLine + 1;
		for (mu_uint32 n = 0; n < linesCount; ++n)
		{
			mu_uint32 startIndex = GetLineIndex(n + minLine) + _DisplayIndexX;
			mu_uint32 endIndex = GetLineIndex(n + minLine + 1);
			if (startIndex >= endIndex) continue;
			if (_Buffer[endIndex - 1] == _T('\n')) --endIndex;

			if (n == linesCount - 1)
			{
				if (endIndex > _MaximumSelection)
					endIndex = _MaximumSelection;

				if (startIndex < endIndex)
				{
					mu_memcpy(&s_TextBuffer[bufferIndex], &_Buffer[startIndex], (endIndex - startIndex) * sizeof(mu_unicode));
					bufferIndex += endIndex - startIndex;
				}
			}
			else
			{
				if (n == 0 && startIndex < _MinimumSelection)
				{
					startIndex = _MinimumSelection;
				}

				mu_memcpy(&s_TextBuffer[bufferIndex], &_Buffer[startIndex], (endIndex - startIndex) * sizeof(mu_unicode));
				bufferIndex += endIndex - startIndex;
				mu_memcpy(&s_TextBuffer[bufferIndex], (mu_unicode*)_T("\r\n"), 2 * sizeof(mu_unicode));
				bufferIndex += 2;
			}
		}

		cbData = bufferIndex;

		/*
		* +1 for the terminating nullptr
		*/
		if (SDL_SetClipboardText(boost::locale::conv::utf_to_utf<mu_char>(mu_string(s_TextBuffer, cbData)).c_str()) != 0)
			return 0;
	}

	return cbData;
}

void UIMultiLineEditBox::Paste()
{
	//
	// Insert contents of clipboard, after unhilighting current selection
	// and deleting it.
	//
	DeleteText();
	PasteText();

	//
	// Update display
	//
	ScrollText();
	_IsDirty = true;
}

mu_uint32 UIMultiLineEditBox::GetLineLength(mu_uint32 lineNumber)
{
	mu_uint32 result;

	if (lineNumber >= _LinesCount)
		return 0;

	if (lineNumber == _LinesCount - 1)
	{
		/*
		* Since we can't have a CRLF on the last line
		*/
		return (_Length - _Lines[_LinesCount - 1]);
	}
	else
	{
		result = _Lines[lineNumber + 1] - _Lines[lineNumber];
	}

	return result;
}

mu_uint32 UIMultiLineEditBox::GetLineWidth(mu_unicode *textBuffer, mu_uint32 textLength)
{
	EVector2 Size;
	_Font->GetTextExtentPoint(textBuffer, textLength, ETA_LEFT, 1.0f, &Size);
	return Size.x;
}

mu_int32 UIMultiLineEditBox::IchToLine(mu_uint32 ich)
{
	mu_int32 iLo, iHi, iLine;

	iLo = 0;
	iHi = _LinesCount;

	if (ich == (mu_uint32)-1)
		ich = _MinimumSelection;

	while (iLo < iHi - 1)
	{
		iLine = EMath::Max((iHi - iLo) / 2, 1) + iLo;

		if (_Lines[iLine] > ich)
		{
			iHi = iLine;
		}
		else
		{
			iLo = iLine;
		}
	}

	return iLo;
}

void UIMultiLineEditBox::IchToXYPos(mu_uint32 ich, mu_boolean prevLine, EPoint &ppt)
{
	mu_int32 iline;
	mu_uint32 cch;
	mu_int32 xPosition, yPosition;
	mu_int32 xOffset;

	/*
	* For horizontal scroll displacement on left justified text and
	* for indent on centered or right justified text
	*/
	mu_unicode *pText, *pTextStart, *pLineStart;

	/*
	* Determine what line the character is on
	*/
	iline = IchToLine(ich);

	/*
	* Calc. the yPosition now. Note that this may change by the height of one
	* char if the prevLine flag is set and the mu_uint32 is at the beginning of a
	* line.
	*/
	yPosition = (iline - (mu_uint32)_DisplayIndexY) * _LineHeight + _NativeY;

	/*
	* Now determine the xPosition of the character
	*/
	pTextStart = _Buffer;

	if (prevLine && iline && (ich == _Lines[iline]) &&
		pTextStart[ich - 1] != '\n')
	{
		/*
		* First char in the line. We want text extent upto end of the previous
		* line if we aren't at the 0th line.
		*/
		--iline;

		yPosition = yPosition - _LineHeight;
		pLineStart = pTextStart + _Lines[iline];

		/*
		* Note that we are taking the position in front of any CRLFs in the
		* text.
		*/
		cch = GetLineLength(iline);
	}
	else
	{
		pLineStart = pTextStart + _Lines[iline];
		pText = pTextStart + ich;

		/*
		* Strip off CRLF or CRCRLF. Note that we may be pointing to a CR but in
		* which case we just want to strip off a single CR or 2 CRs.
		*/

		/*
		* We want pText to point to the first CR at the end of the line if
		* there is one. Thus, we will get an xPosition to the right of the last
		* visible char on the line otherwise we will be to the left of
		* character ich.
		*/

		/*
		* Check if we at the end of text
		*/
		if (ich < _Length)
		{
			mu_unicode *pwText = pText;

			if (ich && *(pwText - 1) == '\n')
			{
				--pwText;
			}

			pText = pwText;
		}

		if (pText < pLineStart)
			pText = pLineStart;

		cch = (mu_uint32)((mu_size)pText - (mu_size)pLineStart) / sizeof(mu_unicode);
	}

	/*
	* Find out how many pixels we indent the line for funny formats
	*/
	if (ContainsStyle(UES_CENTER | UES_RIGHT) == true)
	{
		xOffset = CalcXOffset(iline);
	}
	else
	{
		xOffset = -(mu_int32)_DisplayIndexX;
	}

	xPosition = _NativeX + xOffset +
		GetLineWidth(pLineStart, cch);

	ppt.x = xPosition;
	ppt.y = yPosition;
}

mu_int32 UIMultiLineEditBox::CalcXOffset(mu_int32 lineNumber)
{
	mu_unicode *pText;
	mu_uint32 lineLength;
	mu_uint32 lineWidth;

	if (ContainsStyle(UES_CENTER | UES_RIGHT) == false)
		return 0;

	lineLength = GetLineLength(lineNumber);

	if (lineLength)
	{
		pText = _Buffer + _Lines[lineNumber];
		lineWidth = GetLineWidth(pText, lineLength);
	}
	else
	{
		lineWidth = 0;
	}

	/*
	* If a SPACE or a TAB was eaten at the end of a line by MLBuildchLines
	* to prevent a delimiter appearing at the begining of a line, the
	* the following calculation will become negative causing this bug.
	* So, now, we take zero in such cases.
	* Fix for Bug #3566 --01/31/91-- SANKAR --
	*/
	lineWidth = EMath::Max(0, (mu_int32)(_Width - lineWidth));

	if (ContainsStyle(UES_CENTER) == true)
		return (lineWidth / 2);

	if (ContainsStyle(UES_RIGHT) == true)
	{
		/*
		* Subtract 1 so that the 1 pixel wide cursor will be in the visible
		* region on the very right side of the screen.
		*/
		return EMath::Max(0, (mu_int32)(lineWidth - 1));
	}

	return 0;
}

void UIMultiLineEditBox::ShiftchLines(mu_uint32 line, mu_int32 delta)
{
	if (line >= _LinesCount)
		return;

	/*
	* Just add delta to the starting point of each line after iLine
	*/
	for (; line < _LinesCount; ++line)
		_Lines[line] += delta;
}

void UIMultiLineEditBox::BuildLines(mu_uint32 line, mu_int32 cchDelta, mu_boolean userTyping, mu_int32 *pll, mu_int32 *phl)
{
	mu_unicode *ptext; /* Starting address of the text */

	/*
	* We keep these mu_uint32's so that we can Unlock _Buffer when we have to grow
	* the _Lines array. With large text handles, it becomes a problem if we
	* have a locked block in the way.
	*/
	mu_uint32 ichLineStart;
	mu_uint32 ichLineEnd;
	mu_uint32 ichLineEndBeforeCRLF;
	mu_uint32 ichCRLF;

	mu_uint32 cch;

	mu_boolean fLineBroken = false; /* Initially, no new line breaks are made */
	mu_uint32 minCchBreak;
	mu_uint32 maxCchBreak;
	mu_boolean fOnDelimiter;

	if (_Length == 0)
	{
		_DisplayIndexX = 0;
		_LinesCount = 1;

		if (pll)
			*pll = 0;
		if (phl)
			*phl = 0;

		goto UpdateScroll;
	}

	if (userTyping && cchDelta)
		ShiftchLines(line + 1, cchDelta);

	if (!line && !cchDelta && !userTyping)
	{
		/*
		* Reset number of lines in text since we will be running through all
		* the text anyway...
		*/
		_LinesCount = 1;
	}

	/*
	* Set min and max line built to be the starting line
	*/
	minCchBreak = maxCchBreak = (cchDelta ? _Lines[line] : 0);

	ptext = _Buffer;

	ichCRLF = ichLineStart = _Lines[line];

	while (ichLineStart < _Length)
	{
		if (ichLineStart >= ichCRLF)
		{
			ichCRLF = ichLineStart;

			/*
			* Move ichCRLF ahead to either the first CR or to the end of text.
			*/
			mu_unicode *pwtext = ptext;

			while (ichCRLF < _Length)
			{
				if (*(pwtext + ichCRLF) == '\n')
				{
					break;
				}
				++ichCRLF;
			}
		}

		if (!_WrapMode)
		{
			mu_uint32  LineWidth;
			/*
			* If we are not word wrapping, line breaks are signified by CRLF.
			*/

			//
			// If we cut off the line at MAXLINELENGTH, we should
			// adjust ichLineEnd.
			//
			if ((ichCRLF - ichLineStart) <= MAXLINELENGTH)
			{
				ichLineEnd = ichCRLF;
			}
			else
			{
				ichLineEnd = ichLineStart + MAXLINELENGTH;
			}

			/*
			* We will keep track of what the longest line is for the horizontal
			* scroll bar thumb positioning.
			*/
			LineWidth = GetLineWidth(ptext + ichLineStart, ichLineEnd - ichLineStart);
		}
		else
		{
			/*
			* Check if the width of the edit control is non-zero;
			* a part of the fix for Bug #7402 -- SANKAR -- 01/21/91 --
			*/
			if (_Width > 0.0f)
			{
				/*
				* Find the end of the line based solely on text extents
				*/
				ichLineEnd = ichLineStart +
					CharactersInWidth(ptext + ichLineStart,
						ichCRLF - ichLineStart,
						_Width,
						true);
			}
			else
			{
				ichLineEnd = ichLineStart;
			}

			if (ichLineEnd == ichLineStart && ichCRLF - ichLineStart)
			{
				/*
				* Maintain a minimum of one char per line
				*/
				//
				// Since it might be a double byte char, so calling ECNextIch.
				//
				ichLineEnd = NextIndex(ichLineEnd);
			}

			/*
			* Now starting from ichLineEnd, if we are not at a hard line break,
			* then if we are not at a space AND the char before us is
			* not a space,(OR if we are at a CR) we will look word left for the
			* start of the word to break at.
			* This change was done for TWO reasons:
			* 1. If we are on a delimiter, no need to look word left to break at.
			* 2. If the previous char is a delimter, we can break at current char.
			* Change done by -- SANKAR --01/31/91--
			*/
			if (ichLineEnd != ichCRLF)
			{
				fOnDelimiter = (ISDELIMETER(*(ptext + ichLineEnd)) ||
					UserIsFullWidth(*(ptext + ichLineEnd)) ||
					ISDELIMETER(*(ptext + ichLineEnd - 1)) ||
					UserIsFullWidth(*(ptext + ichLineEnd - 1)));

				if (!fOnDelimiter ||
					*(ptext + ichLineEnd) == '\n')
				{
					_CalcLines = true;
					Word(ichLineEnd, true, &cch, nullptr);
					_CalcLines = false;

					if (cch > ichLineStart)
					{
						ichLineEnd = cch;
					}

					/*
					* Now, if the above test fails, it means the word left goes
					* back before the start of the line ie. a word is longer
					* than a line on the screen. So, we just fit as much of
					* the word on the line as possible. Thus, we use the
					* pLineEnd we calculated solely on width at the beginning
					* of this else block...
					*/
				}
			}
		}

		if (ptext[ichLineEnd] == ' ' ||
			ptext[ichLineEnd] == '\t')
		{
			/*
			* Swallow the space at the end of a line.
			*/
			if (ichLineEnd < _Length)
			{
				++ichLineEnd;
			}
		}

		/*
		* Skip over crlf or crcrlf if it exists. Thus, ichLineEnd is the first
		* character in the next line.
		*/
		ichLineEndBeforeCRLF = ichLineEnd;

		if (ichLineEnd < _Length && *(ptext + ichLineEnd) == '\n')
			++ichLineEnd;

		/*
		* Now, increment line, allocate space for the next line, and set its
		* starting point
		*/
		++line;

		if (!userTyping || (line > _LinesCount - 1) || (_Lines[line] != ichLineEnd))
		{
			/*
			* The line break occured in a different place than before.
			*/
			if (!fLineBroken)
			{
				/*
				* Since we haven't broken a line before, just set the min
				* break line.
				*/
				fLineBroken = true;
				if (ichLineEndBeforeCRLF == ichLineEnd)
					minCchBreak = maxCchBreak = (ichLineEnd ? ichLineEnd - 1 : 0);
				else
					minCchBreak = maxCchBreak = ichLineEndBeforeCRLF;
			}
			maxCchBreak = EMath::Max(maxCchBreak, ichLineEnd);

			/*
			* Now insert the new line into the array
			*/
			if (!InsertchLine(line, ichLineEnd, (mu_boolean)(cchDelta != 0)))
				goto EndUp;

			ptext = _Buffer;
		}
		else
		{
			maxCchBreak = _Lines[line];

			/*
			* Quick escape
			*/
			goto EndUp;
		}

		ichLineStart = ichLineEnd;
	} /* end while (ichLineStart < _Length) */


	if (line != _LinesCount)
	{
		_LinesCount = line;
		_Lines[line] = 0;
	}

	/*
	* Note that we incremented line towards the end of the while loop so, the
	* index, line, is actually equal to the line count
	*/
	if (_Length &&
		*(ptext + (_Length - 1)) == '\n' &&
		_Lines[_LinesCount - 1] < _Length)
	{
		/*
		* Make sure last line has no crlf in it
		*/
		if (!fLineBroken)
		{
			/*
			* Since we haven't broken a line before, just set the min break
			* line.
			*/
			fLineBroken = true;
			minCchBreak = _Length - 1;
		}
		maxCchBreak = EMath::Max(maxCchBreak, ichLineEnd);

		InsertchLine(line, _Length, false);
	}

EndUp:
	if (pll)
		*pll = minCchBreak;
	if (phl)
		*phl = maxCchBreak;

UpdateScroll:
	ScrollText();
}

mu_boolean UIMultiLineEditBox::InsertchLine(mu_uint32 line, mu_uint32 ich, mu_boolean userTyping)
{
	mu_uint32 dwSize;

	if (userTyping && line < _LinesCount)
	{
		_Lines[line] = ich;
		return (true);
	}

	dwSize = (_LinesCount + 2);

	if (dwSize > _LinesLength)
	{
		mu_uint32 *hResult;
		/*
		* Grow the line index buffer
		*/
		dwSize += DEFAULT_LINEBUMP;
		hResult = (mu_uint32*)mu_realloc(_Lines, dwSize * sizeof(mu_uint32));

		if (!hResult)
		{
			return false;
		}

		_Lines = hResult;
		_LinesLength = dwSize;
	}

	/*
	* Move indices starting at line up
	*/
	if (_LinesCount != line)
		mu_memmove(&_Lines[line + 1], &_Lines[line],
		(_LinesCount - line) * sizeof(mu_uint32));
	++_LinesCount;

	_Lines[line] = ich;
	return true;
}

mu_uint32 UIMultiLineEditBox::MoveSelection(mu_uint32 ich, mu_boolean left)
{
	if (left == true &&
		ich > 0)
	{
		/*
		* Move left
		*/
		ich = PreviousIndex(ich);
	}
	else if(left == false &&
		ich < _Length)
	{
		/*
		* Move right.
		*/
		ich = NextIndex(ich);
	}

	return ich;
}