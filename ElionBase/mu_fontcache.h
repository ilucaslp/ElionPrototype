#ifndef __MU_FONT_CACHE_H__
#define __MU_FONT_CACHE_H__

#pragma once

class MUTextSimpleRender;

struct MUFontGlyph
{
	MUFontGlyph()
	{
		Valid = false;
	}

	mu_boolean Valid;
	mu_uint8 TextureIndex;

	mu_float StartU;
	mu_float StartV;
	mu_float SizeU;
	mu_float SizeV;
	mu_float BearingX;
	mu_float BearingY;
};

namespace MUFontFeatures
{
	const hb_tag_t KernTag = HB_TAG('k', 'e', 'r', 'n'); // kerning operations
	const hb_tag_t LigaTag = HB_TAG('l', 'i', 'g', 'a'); // standard ligature substitution
	const hb_tag_t CligTag = HB_TAG('c', 'l', 'i', 'g'); // contextual ligature substitution

	static hb_feature_t LigatureOff = { LigaTag, 0, 0, UINT_MAX };
	static hb_feature_t LigatureOn = { LigaTag, 1, 0, UINT_MAX };
	static hb_feature_t KerningOff = { KernTag, 0, 0, UINT_MAX };
	static hb_feature_t KerningOn = { KernTag, 1, 0, UINT_MAX };
	static hb_feature_t CligOff = { CligTag, 0, 0, UINT_MAX };
	static hb_feature_t CligOn = { CligTag, 1, 0, UINT_MAX };
};

#ifdef UNICODE
#define DEFAULT_CHARACTER_LIST L"abcdefghijklmn\u00F1opqrstuvwxyzABCDEFGHIJKLMN\u00D1OPQRSTUVWXYZ0123456789\u00A1!@#$%^&*(){}[]\\/|\u00BF?><,.;:`~-_=+\""
#else
#define DEFAULT_CHARACTER_LIST "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\u00A1!@#$%^&*(){}[]\\/|¿?><,.;:`~-_=+\""
#endif

class MUFont
{
public:
	MUFont();
	~MUFont();

	mu_boolean Create(FT_Library FTLibrary, const mu_text *FontPath, mu_float FontScale, mu_uint32 TextureSize, mu_uint32 TextureLayers, mu_float OutlineSize = 0.0f);
	void Destroy();

	void Update();

	MUFontGlyph *GetCharacter(mu_uint32 GlyphIndex);
	
public:
	EINLINE mu_float GetLineHeight()
	{
		return _MaxAdvanceY;
	}
	
public:
	FT_Library _FTLibrary;
	FT_Face _FTFace;
	FT_Stroker _FTStroker;
	mu_uint8 *_FontBuffer;
	mu_uint32 _FontBufferSize;
	hb_font_t *_HBFont;
	hb_buffer_t *_HBBuffer;
	std::vector<hb_feature_t> _Features;
	MUFontGlyph _Characters[65536];
	std::vector<MUTextureAtlas*> _Atlas;
	MUTexture _Texture;
	mu_uint32 _TextureSize;
	mu_uint32 _TextureLayers;
	mu_float _FontScale;
	mu_float _Ascender;
	mu_float _Descender;
	mu_float _MaxAdvanceX;
	mu_float _MaxAdvanceY;
	mu_float _InvTextureSize;
	mu_float _OutlineSize;
};

class MUFontFactory
{
public:
	enum
	{
		TEXTURE_SIZE = 2048, // Higher size would just make a huge memory usage
	};

public:
	MUFontFactory();
	~MUFontFactory();

	mu_boolean Initialize();
	mu_boolean CreateFonts();
	void DestroyFonts();

	void Update();

	MUFont *GetFont(mu_uint32 Index);

private:
	mu_uint32 _TextureSize;
	mu_uint32 _TextureArraySize;
	MUFont _Fonts[MAX_FONT];
	FT_Library _FTLibrary;
};

#endif