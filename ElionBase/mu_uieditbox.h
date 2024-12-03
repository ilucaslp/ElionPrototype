#ifndef __MU_UITEXTINPUT_H__
#define __MU_UITEXTINPUT_H__

#pragma once 

// WARNING : This is a single line edit box, for multiline box use UIMultilineEditBox

#define VK_CTRLC	0x03
#define VK_CTRLV	0x16
#define VK_CTRLX	0x18
#define VK_CTRLZ	0x1A

#define MAXLINELENGTH 1024u

#define ISDELIMETER(ch) ((ch == _T(' ')) || (ch == _T('\t')))

mu_boolean UserIsFullWidth(mu_unicode Char);

class UIEditBox : public UIBase
{
public:
	static mu_unicode *s_TextBuffer;
	static mu_uint32 s_TextBufferLength;

	enum
	{
		UES_LEFT			= (0),
		UES_CENTER			= (1 << 0),
		UES_RIGHT			= (1 << 1),
		UES_UPPERCASE		= (1 << 2),
		UES_LOWERCASE		= (1 << 3),
		UES_PASSWORD		= (1 << 4),
		UES_AUTOVSCROLL		= (1 << 5),
		UES_AUTOHSCROLL		= (1 << 6),
		UES_NOHIDESEL		= (1 << 7),
		UES_OEMCONVERT		= (1 << 8),
		UES_READONLY		= (1 << 9),
		UES_WANTRETURN		= (1 << 10),
		UES_NUMBER			= (1 << 11),
	};

	enum
	{
		/*
		* Key modifiers which have been pressed.  Code in KeyDownHandler and
		* CharHandler depend on these exact values.
		*/
		NONEDOWN = 0, /* Neither shift nor control down */
		CTRLDOWN = 1, /* Control key only down */
		SHFTDOWN = 2, /* Shift key only down */
		SHCTDOWN = 3, /* Shift and control keys down = CTRLDOWN + SHFTDOWN */
		NOMODIFY = 4, /* Neither shift nor control down */

		DEFAULT_LIMIT = 30000,
		EXTRA_ALLOC = 32, // Power of 2 extra allocation for avoid any kind of problems in the text buffer
	};

	enum
	{
		UNDO_NONE,		/* We can't undo the last operation. */
		UNDO_INSERT,	/* We can undo the user's insertion of characters */
		UNDO_DELETE,	/* We can undo the user's deletion of characters */
	};

	struct UIUndoBuffer
	{
		mu_uint32 UndoType;				/* Current type of undo we support */
		mu_unicode *DeletedText;		/* Pointer to text which has been deleted (for
										undo) -- note, the memory is allocated as fixed
										*/
		mu_uint32 DeletedIndex;			/* Starting index from which text was deleted */
		mu_uint32 DeletedCount;			/* Count of deleted characters in buffer */
		mu_uint32 InsertionStart;		/* Starting index from which text was inserted */
		mu_uint32 InsertionEnd;			/* Ending index of inserted text */
	};

public:
	UIEditBox(mu_uint32 objectId, UISceneManager *manager, UIBase *owner, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_float width, mu_float height);
	~UIEditBox();

	virtual void SystemConfigure() override;

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;

	virtual void OnFocus() override;
	virtual void OnFocusCancelled() override;

	virtual mu_uint32 OnPreWindowMessage(const SDL_Event *event, mu_size *result) override;
	virtual mu_uint32 OnWindowMessage(const SDL_Event *event, mu_size *result) override;
	void OnChar(mu_unicode keyValue, mu_unicode mod = 0);
	mu_uint32 OnKeyDown(const SDL_Keysym &key);

	void EmptyUndo();

	void Word(mu_uint32 ichStart, mu_boolean fLeft, mu_uint32 *pichMin, mu_uint32 *pichMax);
	mu_uint32 PreviousIndex(mu_uint32 currentIndex);
	mu_uint32 NextIndex(mu_uint32 currentIndex);

	void MergeUndoInsertInfo(mu_uint32 insertIndex, mu_uint32 insertCount);

	mu_boolean SetText(const mu_unicode *Text);
	void GetText(mu_unicode *Buffer, mu_uint32 BufferLength);
	mu_uint32 GetLength();
	mu_uint32 DeleteText();
	mu_boolean InsertTextBuffer(const mu_unicode *lpText, mu_uint32 &cchInsert);
	mu_uint32 InsertText(const mu_unicode *lpText, mu_uint32 cchInsert);
	mu_uint32 PasteText();
	void ClearText();
	void ClearAllText();
	void ResetTextInfo();

	mu_uint32 CharactersInWidth(const mu_unicode *text, mu_uint32 charCount, mu_float width, mu_boolean isForward);
	mu_boolean ScrollText();
	void SelectAll();
	void ChangeSelection(mu_uint32 ichNewMinSel, mu_uint32 ichNewMaxSel);
	void SetSelection(mu_uint32 ichSelStart, mu_uint32 ichSelEnd);
	void SetCaretPosition();

	void DrawText();

	void SetFont(MUTextSimpleRender *font);

	// Mouse Processing
	virtual void OnMouseMove(mu_int32 mouse_x, mu_int32 mouse_y) override;
	virtual void OnMouseOver(mu_int32 mouse_x, mu_int32 mouse_y) override;
	virtual void OnMouseLeave() override;
	virtual void OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys) override;
	virtual void OnMouseReleased(mu_uint32 mouse_flags, mu_uint32 virtual_keys) override;
	virtual void OnMouseCancelled(mu_uint32 mouse_flags) override;
	mu_uint32 MouseToIch(mu_int32 mouseX, mu_int32 mouseY);

	// Copy & Paste functions
	mu_boolean Undo();
	void Cut();
	mu_uint32 Copy();
	void Paste();

public:
	virtual mu_boolean BypassMessage() override
	{
		return true;
	}

public: // Inline functions
	EINLINE void SetLimitText(mu_uint32 limit)
	{
		if (!limit)
		{
			limit = 0xFFFFFFFFu;
		}

		limit = EMath::Min(limit, 0x7FFFFFFEu);

		_MaxLength = limit;
	}

	EINLINE mu_boolean IsStyle(mu_uint32 style)
	{
		return (_Style & style) == style;
	}

	EINLINE mu_boolean ContainsStyle(mu_uint32 style)
	{
		return (_Style & style) != 0;
	}

	EINLINE void MarkStyle(mu_uint32 style)
	{
		_Style |= style;
	}

	EINLINE void UnmarkStyle(mu_uint32 style)
	{
		_Style &= ~style;
	}

	EINLINE void SetTextColor(mu_uint8 R, mu_uint8 G, mu_uint8 B, mu_uint8 A)
	{
		_TextColor = MU::MakeARGB(R, G, B, A);
		_IsDirty = true;
	}

	EINLINE void SetBackColor(mu_uint8 R, mu_uint8 G, mu_uint8 B, mu_uint8 A)
	{
		_BackColor = MU::MakeARGB(R, G, B, A);
		_IsDirty = true;
	}

	EINLINE void SetHighlightColor(mu_uint8 R, mu_uint8 G, mu_uint8 B, mu_uint8 A)
	{
		_HighlightColor = MU::MakeARGB(R, G, B, A);
		_IsDirty = true;
	}

	EINLINE void SetHighlightTextColor(mu_uint8 R, mu_uint8 G, mu_uint8 B, mu_uint8 A)
	{
		_HighlightTextColor = MU::MakeARGB(R, G, B, A);
		_IsDirty = true;
	}

	EINLINE void SetCaretColor(mu_uint8 R, mu_uint8 G, mu_uint8 B, mu_uint8 A)
	{
		_Caret->SetColor(MU::MakeARGB(R, G, B, A));
	}

	EINLINE void SetPasswordChar(mu_unicode Char)
	{
		_PasswordChar = Char;

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

	EINLINE mu_boolean IsReadOnly()
	{
		return _ReadOnly;
	}

	EINLINE void SetReadOnly(mu_boolean ReadOnly)
	{
		_ReadOnly = ReadOnly;
	}

private:
	UISpriteColor *_Caret;
	mu_float _CaretX, _CaretY;
	mu_boolean _UpdateCaret;

	mu_uint32 _TextColor;
	mu_uint32 _BackColor;
	mu_uint32 _HighlightColor;
	mu_uint32 _HighlightTextColor;

	MUTextSimpleRender *_Font;

	mu_uint32 _Style;
	mu_unicode *_Buffer;					/* the actual contents of the control */
	mu_uint32 _BufferLength;				/* allocated characters count */
	mu_uint32 _MaxLength;					/* max buffer length allowed in this control */
	mu_uint32 _Length;						/* current buffer length */
	mu_uint32 _MinimumSelection;			/* first selected character index */
	mu_uint32 _MaximumSelection;			/* last selected character index */
	mu_uint32 _StartMinimumSelection;
	mu_uint32 _StartMaximumSelection;
	mu_uint32 _CaretIndex;					/* Caret location, Caret is on left side of a character */
	mu_uint32 _DisplayIndex;				/* first displayed character on screen(most left and top character) */
	mu_unicode _PasswordChar;				/* If not null, display this character instead
											* of the real text. So that we can implement
											* hidden text fields.
											*/
	mu_float _PasswordCharWidth;			/* width of password characer */

	mu_boolean _IsMouseDown;				/* Is mouse button down? when moving mouse */
	mu_boolean _IsFocus;					/* Does ec have the focus ? */
	mu_boolean _IsDirty;					/* Modify flag for the edit control */
	mu_boolean _AutoHScroll;				/* Automatically scroll horizontally */
	mu_boolean _NoHideSel;					/* Hide sel when we lose focus? */
	mu_boolean _EatNextChar;				/* Hack for ALT-NUMPAD stuff with combo boxes.
											* If numlock is up, we want to eat the next
											* character generated by the keyboard driver
											* if user enter num pad ascii value...
											*/
	mu_boolean _ReadOnly;					/* Is this a read only edit control? Only
											* allow scrolling, selecting and copying.
											*/
	mu_boolean _CaretHidden;				/* This indicates whether the caret is
											* currently hidden because the width or height
											* of the edit control is too small to show it.
											*/

	UIUndoBuffer _Undo;

	mu_uint32 _RenderIndex;

	mu_string _DrawString;
};

#endif