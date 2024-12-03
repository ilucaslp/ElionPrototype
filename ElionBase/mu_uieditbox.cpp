#include "stdafx.h"
#include "mu_uieditbox.h"

mu_unicode *UIEditBox::s_TextBuffer = nullptr;
mu_uint32 UIEditBox::s_TextBufferLength = 0;

mu_boolean UserIsFullWidth(mu_unicode Char)
{
	//
	// if this Unicode character is mapped to Double-Byte character,
	// this is also FullWidth character..
	//
	const mu_asciistr &Chars = boost::locale::conv::utf_to_utf<mu_char>(mu_unicodestr(1, Char));

	return(Chars.size() > 1 ? true : false);
}

UIEditBox::UIEditBox(mu_uint32 objectId, UISceneManager *manager, UIBase *owner, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_float width, mu_float height) : UIBase()
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
	_MaxLength = DEFAULT_LIMIT;
	_Length = 0;
	_MinimumSelection = 0;
	_MaximumSelection = 0;
	_StartMinimumSelection = 0;
	_StartMaximumSelection = 0;
	_CaretIndex = 0;
	_DisplayIndex = 0;

	_PasswordChar = _T('\0');
	_PasswordCharWidth = 0;

	_IsMouseDown = false;
	_IsFocus = false;
	_IsDirty = false;
	_AutoHScroll = true;
	_NoHideSel = false;
	_EatNextChar = false;

	_ReadOnly = false;
	_CaretHidden = false;

	mu_zeromem(&_Undo, sizeof(_Undo));
	_Undo.InsertionStart = 0xFFFFFFFF;
	_Undo.InsertionEnd = 0xFFFFFFFF;
	_Undo.DeletedIndex = 0xFFFFFFFF;

	_RenderIndex = g_RandomTextIndex.fetch_add(1);

	SetFont(FONT_NORMAL);
}

UIEditBox::~UIEditBox()
{
	MU_SAFE_FREE(_Buffer);
	MU_SAFE_FREE(_Undo.DeletedText);
	MU_SAFE_FREE(s_TextBuffer);
}

void UIEditBox::SystemConfigure()
{
	UIBase::SystemConfigure();

	SetFont(_Font);
}

void UIEditBox::Update(mu_uint64 CurrentTime)
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

void UIEditBox::Draw()
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
			_Font->DrawString2D(_RenderIndex, _NativeX + _Width, _NativeY, 1.0f, ETA_RIGHT, ETA_BOTTOM, 0.0f, _DrawString.c_str());
		}
		else if (ContainsStyle(UES_CENTER))
		{
			_Font->DrawString2D(_RenderIndex, _NativeX + _Width * 0.5f, _NativeY, 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, _DrawString.c_str());
		}
		else
		{
			_Font->DrawString2D(_RenderIndex, _NativeX, _NativeY, 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, _DrawString.c_str());
		}
	}
	else
	{
		static const mu_unicode *textFixer = _T("");
		if (ContainsStyle(UES_RIGHT))
		{
			_Font->DrawString2D(_RenderIndex, _NativeX + _Width, _NativeY, 1.0f, ETA_RIGHT, ETA_BOTTOM, 0.0f, textFixer);
		}
		else if (ContainsStyle(UES_CENTER))
		{
			_Font->DrawString2D(_RenderIndex, _NativeX + _Width * 0.5f, _NativeY, 1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f, textFixer);
		}
		else
		{
			_Font->DrawString2D(_RenderIndex, _NativeX, _NativeY, 1.0f, ETA_LEFT, ETA_BOTTOM, 0.0f, textFixer);
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
		_Font->GetTextExtentPoint(_DrawString.c_str(), _DrawString.size(), horizontalAlign, 1.0f, &Size, _CaretIndex - _DisplayIndex, &_CaretX, &_CaretY);
		_Font->UpdateCoord(_RenderIndex, _CaretX, _CaretY);

		_Caret->SetPosition(EMath::Round(_CaretX), _CaretY, false);
	}

	if (g_CaretEnabled == true &&
		_CaretHidden == false &&
		_IsFocus == true &&
		_ReadOnly == false)
	{
		_Caret->Draw();
	}
}

void UIEditBox::OnFocus()
{
	_IsFocus = true;
}

void UIEditBox::OnFocusCancelled()
{
	_IsFocus = false;
	_MinimumSelection = _MaximumSelection = _CaretIndex;

	/*
	* Set dirty bit
	*/
	_IsDirty = true;
}

mu_uint32 UIEditBox::OnPreWindowMessage(const SDL_Event *event, mu_size *result)
{
	switch (event->type)
	{
	case SDL_TEXTINPUT:
		{
		}
		break;

	case SDL_TEXTEDITING:
		{
		}
		break;

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

				case SDL_SCANCODE_RETURN:
				case SDL_SCANCODE_KP_ENTER:
				case SDL_SCANCODE_ESCAPE:
				case SDL_SCANCODE_TAB:
					break;
				}
			}
		}
		break;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

mu_uint32 UIEditBox::OnWindowMessage(const SDL_Event *event, mu_size *result)
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

void UIEditBox::OnChar(mu_unicode keyValue, mu_unicode mod)
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
				// Calling PreviousIndex rather than just doing a decrement for VK_BACK
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
			if (keyValue >= 0x1E)
			{
				if (InsertText(&keyValue, 1) != 0)
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

mu_uint32 UIEditBox::OnKeyDown(const SDL_Keysym &key)
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

	switch (key.scancode)
	{
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

	case SDL_SCANCODE_UP:
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
				_CaretIndex = PreviousIndex(_CaretIndex);
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

	case SDL_SCANCODE_DOWN:
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
				_CaretIndex = NextIndex(_CaretIndex);
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

void UIEditBox::EmptyUndo()
{
	MU_SAFE_FREE(_Undo.DeletedText);

	mu_zeromem(&_Undo, sizeof(_Undo));
}

void UIEditBox::Word(mu_uint32 ichStart, mu_boolean fLeft, mu_uint32 *pichMin, mu_uint32 *pichMax)
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

	/*
	* Don't give out hints about word breaks if password chars are being used,
	*/
	if (_PasswordChar != _T('\0'))
	{
		if (pichMin) *pichMin = 0;
		if (pichMax) *pichMax = _Length;
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

mu_uint32 UIEditBox::PreviousIndex(mu_uint32 currentIndex)
{
	return currentIndex > 0 ? currentIndex - 1 : 0;
}

mu_uint32 UIEditBox::NextIndex(mu_uint32 currentIndex)
{
	return currentIndex + 1;
}

void UIEditBox::MergeUndoInsertInfo(mu_uint32 insertIndex, mu_uint32 insertCount)
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

mu_boolean UIEditBox::SetText(const mu_unicode *Text)
{
	mu_uint32 cchLength = 0;
	mu_uint32 cchSave = _Length;
	mu_uint32 ichCaretSave = _CaretIndex;
	mu_unicode *hText;

	_Length = _CaretIndex = 0;

	if (Text == nullptr || mu_stricmp(Text, _T("")) == 0)
	{
		hText = (mu_unicode*)mu_realloc(_Buffer, EXTRA_ALLOC * sizeof(mu_unicode));
		if (hText != nullptr)
		{
			_Buffer = hText;
			_BufferLength = EXTRA_ALLOC;
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
		if (cchLength > 0 && !InsertTextBuffer(Text, cchLength))
		{
			/*
			* Restore original state and notify parent we ran out of memory.
			*/
			_Length = cchSave;
			_CaretIndex = ichCaretSave;
			return false;
		}
		else if (cchLength > 0)
		{
			_CaretIndex = _Length;
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

void UIEditBox::GetText(mu_unicode *Buffer, mu_uint32 BufferLength)
{
	mu_uint32 length = EMath::Min(_Length, BufferLength - 1);

	if (length > 0)
	{
		mu_memcpy(Buffer, _Buffer, length * sizeof(mu_unicode));
	}
	Buffer[length] = _T('\0');
}

mu_uint32 UIEditBox::GetLength()
{
	return _Length;
}

mu_uint32 UIEditBox::DeleteText()
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
		EmptyUndo();

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

	return cchDelete;
}

mu_boolean UIEditBox::InsertTextBuffer(const mu_unicode *lpText, mu_uint32 &cchInsert)
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

	/*
	* Set dirty bit
	*/
	_IsDirty = true;

	return true;
}

mu_uint32 UIEditBox::InsertText(const mu_unicode *lpText, mu_uint32 cchInsert)
{
	mu_unicode *pText;
	mu_uint32 cchInsertCopy = cchInsert;
	mu_uint32 cchT;
	mu_float textWidth;
	EVector2 size;

	if (_Length >= _MaxLength)
	{
		cchInsert = 0;
	}
	else
	{
		if (_AutoHScroll == false)
		{
			pText = _Buffer;

			cchInsert = EMath::Min(cchInsert, (unsigned)(_MaxLength - _Length));
			if (_PasswordChar)
			{
				textWidth = _Length * _PasswordCharWidth;
			}
			else
			{
				_Font->GetTextExtentPoint(pText, _Length, ETA_LEFT, 1.0f, &size);
				textWidth = size.x;
			}

			cchT = CharactersInWidth(lpText, cchInsert, _Width - textWidth, true);
			cchInsert = EMath::Min(cchInsert, cchT);
		}
		else
		{
			cchInsert = EMath::Min(cchInsert, _MaxLength - _Length);
		}
	}

	/*
	* Now try actually adding the text to the ped
	*/
	if (cchInsert != 0 && InsertTextBuffer(lpText, cchInsert) == false)
	{
		// Notify not enough space to insert
		return 0;
	}

	if (cchInsert != 0)
	{
		_IsDirty = true;
	}

	if (cchInsert < cchInsertCopy)
	{
		/*
		* Notify parent that we couldn't insert all the text requested
		*/
		// Notify the length of the copied text
	}

	/*
	* Update selection extents and the caret position. Note that ECInsertText
	* updates _CaretIndex, _MinimumSelection, and _MaximumSelection to all be after
	* the inserted text.
	*/
	return (cchInsert);
}

mu_uint32 UIEditBox::PasteText()
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
		cchAdded = InsertText(string.c_str(), string.size());
	}
	else
	{
		mu_string string = boost::locale::conv::utf_to_utf<mu_text>(clipboardString.substr(0, carrierIndex));
		cchAdded = InsertText(string.c_str(), string.size());
	}

	return cchAdded;
}

void UIEditBox::ClearText()
{
	if (_ReadOnly == false &&
		_MinimumSelection < _MaximumSelection)
	{
		DeleteText();
	}
}

void UIEditBox::ClearAllText()
{
	mu_boolean isReadOnly = IsReadOnly();
	SetReadOnly(false);
	SelectAll();
	ClearText();
	SetReadOnly(isReadOnly);
}

void UIEditBox::ResetTextInfo()
{
	//
	// Reset caret, selections, scrolling, and dirty information.
	//
	_MinimumSelection = _MaximumSelection = 0;
	_DisplayIndex = 0;
	_IsDirty = true;

	EmptyUndo();
}

mu_uint32 UIEditBox::CharactersInWidth(const mu_unicode *text, mu_uint32 charCount, mu_float width, mu_boolean isForward)
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
	* Check if password hidden chars are being used.
	*/
	if (_PasswordChar)
	{
		return (EMath::Min((mu_uint32)(width / _PasswordCharWidth), charCount));
	}

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

mu_boolean UIEditBox::ScrollText()
{
	mu_unicode *pTextScreenStart;
	mu_uint32 scrollAmount;
	mu_uint32 newScreenStartX = _DisplayIndex;
	mu_uint32 cch;
	mu_boolean fAdjustNext = false;

	if (!_AutoHScroll)
		return false;

	/*
	* Calculate the new starting screen position
	*/
	if (_CaretIndex <= _DisplayIndex)
	{
		/*
		* Caret is to the left of the starting text on the screen we must
		* scroll the text backwards to bring it into view. Watch out when
		* subtracting unsigned numbers when we have the possibility of going
		* negative.
		*/
		pTextScreenStart = _Buffer;

		scrollAmount = CharactersInWidth(pTextScreenStart, _CaretIndex, _Width / 4.0f, false);

		newScreenStartX = _CaretIndex - scrollAmount;
	}
	else if (_CaretIndex != _DisplayIndex)
	{
		pTextScreenStart = _Buffer;
		pTextScreenStart += _DisplayIndex;

		cch = CharactersInWidth(pTextScreenStart, _CaretIndex - _DisplayIndex, _Width, false);

		if (cch < _CaretIndex - _DisplayIndex)
		{
			fAdjustNext = true;

			/*
			* Scroll Forward 1/4 -- if that leaves some empty space
			* at the end, scroll back enough to fill the space
			*/
			newScreenStartX = _CaretIndex - (3 * cch / 4);

			cch = CharactersInWidth(pTextScreenStart, _Length - _DisplayIndex, _Width, false);

			if (newScreenStartX > (_Length - cch))
				newScreenStartX = _Length - cch;
		}
		else if (ContainsStyle(UES_CENTER | UES_RIGHT) == true)
		{
			cch = CharactersInWidth(pTextScreenStart, _Length - _DisplayIndex, _Width, false);

			/*
			* Scroll the text hidden behind the left border back
			* into view.
			*/
			if (_DisplayIndex == _Length - cch)
			{
				pTextScreenStart -= _DisplayIndex;
				cch = CharactersInWidth(pTextScreenStart, _Length, _Width, false);

				newScreenStartX = _Length - cch;
			}
		}
	}

	if (_DisplayIndex != newScreenStartX)
	{
		_DisplayIndex = newScreenStartX;
		_IsDirty = true;

		// Caret pos is set by DrawText().
		return true;
	}

	return false;
}

void UIEditBox::SelectAll()
{
	SetSelection(0, GetLength());
}

void UIEditBox::ChangeSelection(mu_uint32 ichNewMinSel, mu_uint32 ichNewMaxSel)
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
	//if (IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false && (_IsFocus || _NoHideSel))
	{
		//
		// Update text and caret.
		//
		_IsDirty = true;
	}
}

void UIEditBox::SetSelection(mu_uint32 ichSelStart, mu_uint32 ichSelEnd)
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

	SetCaretPosition();

	/*
	* We may need to scroll the text to bring the caret into view...
	*/
	ScrollText();
}

void UIEditBox::SetCaretPosition()
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

void UIEditBox::DrawText()
{
	mu_unicode *pText = _Buffer;

	/*
	* Find out how many characters will fit on the screen so that we don't do
	* any needless drawing.
	*/
	mu_uint32 cchToDraw = CharactersInWidth(pText + _DisplayIndex, _Length - _DisplayIndex, _Width, true);
	mu_uint32 ichEnd = _DisplayIndex + cchToDraw;

	/*
	* There is no selection if,
	* 1. MinSel and MaxSel are equal OR
	* 2. (This has lost the focus AND Selection is to be hidden)
	*/
	mu_boolean noSelection = ((_MinimumSelection == _MaximumSelection) ||
							  (!_IsFocus && !_NoHideSel) ||
							  (_MaximumSelection <= _DisplayIndex) ||
							  (_MinimumSelection >= ichEnd));

	mu_unicode colorBuffer[32];
	mu_unicode backBuffer[32];
	mu_unicode highlightBuffer[32];
	mu_unicode highlightTextBuffer[32];
	mu_sprintf_s(colorBuffer, mu_countof(colorBuffer), _T("\uE000%08X\uE000"), _TextColor);
	mu_sprintf_s(backBuffer, mu_countof(backBuffer), _T("\uE001%08X\uE001"), _BackColor);
	mu_sprintf_s(highlightBuffer, mu_countof(highlightBuffer), _T("\uE001%08X\uE001"), _HighlightColor);
	mu_sprintf_s(highlightTextBuffer, mu_countof(highlightTextBuffer), _T("\uE000%08X\uE000"), _HighlightTextColor);
	mu_uint32 minBufferLength = cchToDraw + mu_countof(colorBuffer) + mu_countof(backBuffer) + 128;

	if (s_TextBufferLength < minBufferLength)
	{
		s_TextBufferLength = minBufferLength;
		s_TextBuffer = (mu_unicode *)mu_realloc(s_TextBuffer, sizeof(mu_unicode) * s_TextBufferLength);
	}

	if (noSelection)
	{
		_DrawString = colorBuffer;
		_DrawString += backBuffer;

		if (_PasswordChar != _T('\0'))
		{
			_DrawString += mu_string(cchToDraw, _PasswordChar);
		}
		else
		{
			_DrawString += mu_string(pText + _DisplayIndex, cchToDraw);
		}
	}
	else
	{
		mu_sprintf_s(s_TextBuffer, s_TextBufferLength, _T("") SPF_STRING _T("") SPF_STRING _T(""), colorBuffer, backBuffer);

		if (_PasswordChar != _T('\0'))
		{
			if (_MinimumSelection <= _DisplayIndex)
			{
				_DrawString = highlightTextBuffer;
				_DrawString += highlightBuffer;

				if (_MaximumSelection >= ichEnd)
				{
					_DrawString += mu_string(cchToDraw, _PasswordChar);
				}
				else
				{
					_DrawString += mu_string(_MaximumSelection - _DisplayIndex, _PasswordChar);
					_DrawString += colorBuffer;
					_DrawString += backBuffer;
					_DrawString += mu_string(ichEnd - _MaximumSelection, _PasswordChar);
				}
			}
			else
			{
				_DrawString = colorBuffer;
				_DrawString += backBuffer;

				_DrawString += mu_string(_MinimumSelection - _DisplayIndex, _PasswordChar);
				_DrawString += highlightTextBuffer;
				_DrawString += highlightBuffer;

				if (_MaximumSelection >= ichEnd)
				{
					_DrawString += mu_string(ichEnd - _MinimumSelection, _PasswordChar);
				}
				else
				{
					_DrawString += mu_string(_MaximumSelection - _MinimumSelection, _PasswordChar);
					_DrawString += colorBuffer;
					_DrawString += backBuffer;
					_DrawString += mu_string(ichEnd - _MaximumSelection, _PasswordChar);
				}
			}
		}
		else
		{
			if (_MinimumSelection <= _DisplayIndex)
			{
				_DrawString = highlightTextBuffer;
				_DrawString += highlightBuffer;

				if (_MaximumSelection >= ichEnd)
				{
					_DrawString += mu_string(pText + _DisplayIndex, cchToDraw);
				}
				else
				{
					_DrawString += mu_string(pText + _DisplayIndex, _MaximumSelection - _DisplayIndex);
					_DrawString += colorBuffer;
					_DrawString += backBuffer;
					_DrawString += mu_string(pText + _MaximumSelection, ichEnd - _MaximumSelection);
				}
			}
			else
			{
				_DrawString = colorBuffer;
				_DrawString += backBuffer;

				_DrawString += mu_string(pText + _DisplayIndex, _MinimumSelection - _DisplayIndex);
				_DrawString += highlightTextBuffer;
				_DrawString += highlightBuffer;

				if (_MaximumSelection >= ichEnd)
				{
					_DrawString += mu_string(pText + _MinimumSelection, ichEnd - _MinimumSelection);
				}
				else
				{
					_DrawString += mu_string(pText + _MinimumSelection, _MaximumSelection - _MinimumSelection);
					_DrawString += colorBuffer;
					_DrawString += backBuffer;
					_DrawString += mu_string(pText + _MaximumSelection, ichEnd - _MaximumSelection);
				}
			}
		}
	}

	SetCaretPosition();
}

void UIEditBox::SetFont(MUTextSimpleRender *font)
{
	if (font == nullptr) return;

	_Font = font;

	_Caret->SetDimensions(1.0f, _Font->GetFont()->_MaxAdvanceY, false);
	_UpdateCaret = true;
	_IsDirty = true;

	if (_PasswordChar != _T('\0'))
	{
		EVector2 size;
		_Font->GetTextExtentPoint(&_PasswordChar, 1, ETA_LEFT, 1.0f, &size);
		_PasswordCharWidth = size.x;
	}
	else
	{
		_PasswordCharWidth = 0.0f;
	}
}

void UIEditBox::OnMouseMove(mu_int32 mouse_x, mu_int32 mouse_y)
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

	ScrollText();
	ChangeSelection(newMinSel, newMaxSel);
}

void UIEditBox::OnMouseOver(mu_int32 mouse_x, mu_int32 mouse_y)
{

}

void UIEditBox::OnMouseLeave()
{

}

void UIEditBox::OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys)
{
	mu_int32 mouseX = MU_INPUT->GetMouseX(), mouseY = MU_INPUT->GetMouseY();

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

void UIEditBox::OnMouseReleased(mu_uint32 mouse_flags, mu_uint32 virtual_keys)
{
	if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_LEFT))
	{
		_IsMouseDown = false;
	}
}

void UIEditBox::OnMouseCancelled(mu_uint32 mouse_flags)
{
	if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_LEFT))
	{
		_IsMouseDown = false;
	}
}

mu_uint32 UIEditBox::MouseToIch(mu_int32 mouseX, mu_int32 mouseY)
{
	mu_unicode *pText;
	mu_uint32 cch;

	if (_Length == 0)
		return 0;

	if (mouseX <= _NativeX)
	{
		/*
		* Return either the first non visible character or return 0 if at
		* beginning of text
		*/
		if (_DisplayIndex)
			return (_DisplayIndex - 1);
		else
			return (0);
	}

	if (mouseX > _NativeX + _Width)
	{
		pText = _Buffer;

		cch = CharactersInWidth(pText + _DisplayIndex, _Length - _DisplayIndex, _Width, true) + _DisplayIndex;

		if (cch >= _Length)
			return _Length;
		else
			return _Length + 1;
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

	cch = _Font->GetCharacterIndexByMouse(_RenderIndex, mouseX, mouseY, _DrawString.c_str(), horizontalAlign) + _DisplayIndex;

	return cch;
}

// Copy & Paste functions
mu_boolean UIEditBox::Undo()
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
		InsertText(hDeletedText, cchDeleted);
		mu_free(hDeletedText);

		SetSelection(ichDeleted, ichDeleted + cchDeleted);
	}

	return true;
}

void UIEditBox::Cut()
{
	if (_ReadOnly == false &&
		_MinimumSelection < _MaximumSelection &&
		Copy() > 0)
	{
		ClearText();
	}
}

mu_uint32 UIEditBox::Copy()
{
	if (_PasswordChar != _T('\0'))
	{
		return 0;
	}

	mu_uint32 cbData = (_MaximumSelection - _MinimumSelection);

	if (!cbData)
		return 0;

	mu_unicode *pchSel = _Buffer + _MinimumSelection;
	if (SDL_SetClipboardText(boost::locale::conv::utf_to_utf<mu_char>(mu_string(pchSel, cbData)).c_str()) != 0)
		return 0;

	return cbData;
}

void UIEditBox::Paste()
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