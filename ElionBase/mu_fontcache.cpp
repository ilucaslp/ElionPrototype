#include "stdafx.h"
#include "mu_fontcache.h"

MUFont::MUFont() :
	_FTLibrary(nullptr),
	_FTFace(nullptr),
	_FTStroker(nullptr),
	_FontBuffer(nullptr),
	_FontBufferSize(0),
	_HBFont(nullptr),
	_HBBuffer(nullptr),
	_FontScale(0.0f)
{
	mu_zeromem(&_Characters, sizeof(_Characters));
}

MUFont::~MUFont()
{
	Destroy();
}

mu_boolean MUFont::Create(FT_Library FTLibrary, const mu_text *FontPath, mu_float FontScale, mu_uint32 TextureSize, mu_uint32 TextureLayers, mu_float OutlineSize)
{
	_FTLibrary = FTLibrary;

	if (_FTFace != nullptr)
	{
		return false;
	}

	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, FontPath, "rb") == false)
	{
		return false;
	}

	mu_int64 fileSize = SDL_RWsize(file);

	if (fileSize <= 0 || fileSize > 8*1024*1024) // Limit to 8MB
	{
		Destroy();
		return false;
	}

	_FontBuffer = new mu_uint8[fileSize];
	_FontBufferSize = fileSize;

	if (!SDL_RWread(file, (mu_char*)_FontBuffer, fileSize, 1))
	{
		Destroy();
		return false;
	}

	if (FT_New_Memory_Face(FTLibrary, _FontBuffer, _FontBufferSize, 0, &_FTFace) != 0)
	{
		Destroy();
		return false;
	}

	mu_float FontWidth;

	if (MU_RESOURCE->GetAspectRatio() < 1.0f)
	{
		FontWidth = EMath::Round(MU_TO_WINDOW(FontScale, MU_RESOURCE->_640x480Rate[0])); // Common Aspect Ratio
	}
	else
	{
		mu_float VirtualWidth = MU_RESOURCE->GetWindowHeight() * (4.0f / 3.0f);

		FontWidth = EMath::Round(MU_TO_WINDOW(FontScale, MU_VIEWPORT_RATE(VirtualWidth, 640.0f))); // Common Aspect Ratio
	}

	_FontScale = FontScale;

	FT_Set_Pixel_Sizes(_FTFace, FontWidth, 0);

	_Ascender = (mu_float)_FTFace->size->metrics.ascender / 64.0f;
	_Descender = (mu_float)_FTFace->size->metrics.descender / 64.0f;
	_MaxAdvanceX = (mu_float)_FTFace->size->metrics.max_advance / 64.0f;
	_MaxAdvanceY = (mu_float)_FTFace->size->metrics.height / 64.0f;

	if(OutlineSize > 0.0f)
	{
		if (FT_Stroker_New(FTLibrary, &_FTStroker) != 0)
		{
			Destroy();
			return false;
		}

		FT_Stroker_Set(_FTStroker, static_cast<FT_Fixed>(OutlineSize * 64.0f), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
	}
	_OutlineSize = OutlineSize;

	_HBFont = hb_ft_font_create(_FTFace, nullptr);

	if (_HBFont == nullptr)
	{
		Destroy();
		return false;
	}

	hb_ft_font_set_load_flags(_HBFont, FT_LOAD_RENDER);

	_HBBuffer = hb_buffer_create();

	if (_HBBuffer == nullptr || hb_buffer_allocation_successful(_HBBuffer) == false)
	{
		Destroy();
		return false;
	}

	if (_Texture.Create2DArray(TextureSize, TextureSize, TextureLayers, FONT_TEXTURE_FORMAT, ETextureUsage::eDynamic, FONT_TEXTURE_SAMPLER, nullptr, FONT_TEXTURE_PITCH(TextureSize)) == false)
	{
		Destroy();
		return false;
	}

	for (mu_uint32 index = 0; index < TextureLayers; ++index)
	{
		MUTextureAtlas *atlas = new MUTextureAtlas(TextureSize, TextureSize, 2, MUTextureAtlas::PadWithZero);

		_Atlas.push_back(atlas);
	}

	_TextureSize = TextureSize;
	_TextureLayers = TextureLayers;
	_InvTextureSize = 1.0f / (mu_float)_TextureSize;

	_Features.push_back(MUFontFeatures::LigatureOff);
	_Features.push_back(MUFontFeatures::CligOff);
	_Features.push_back(MUFontFeatures::KerningOff);

	// Pre-loading basic letters
	{
		hb_font_t *hbfont = _HBFont;
		hb_buffer_t *hbbuffer = _HBBuffer;
		hb_buffer_reset(hbbuffer);
		hb_buffer_set_direction(hbbuffer, HB_DIRECTION_LTR);
		hb_buffer_set_script(hbbuffer, HB_SCRIPT_LATIN);
		hb_buffer_set_language(hbbuffer, hb_language_from_string("en", 2));

		mu_string text = DEFAULT_CHARACTER_LIST;

#ifdef UNICODE
#if __ANDROID__
		hb_buffer_add_utf32(hbbuffer, (const mu_uint32*)text.c_str(), text.size(), 0, text.size());
#else
		hb_buffer_add_utf16(hbbuffer, (const mu_uint16*)text.c_str(), text.size(), 0, text.size());
#endif
#else
		hb_buffer_add_utf8(hbbuffer, text.c_str(), text.size(), 0, text.size());
#endif

		hb_shape(hbfont, hbbuffer, _Features.empty() ? nullptr : _Features.data(), _Features.size());

		mu_uint32 glyphCount;
		hb_glyph_info_t *glyphInfo = hb_buffer_get_glyph_infos(hbbuffer, &glyphCount);

		for (mu_uint32 index = 0;index < glyphCount; ++index)
		{
			mu_uint32 glyphIndex = glyphInfo[index].codepoint;
			GetCharacter(glyphIndex);
		}
	}

	return true;
}

void MUFont::Destroy()
{
	for (auto it = _Atlas.begin();it != _Atlas.end();it = _Atlas.erase(it))
	{
		delete (*it);
	}

	_Texture.Release();

	if (_HBBuffer)
	{
		hb_buffer_destroy(_HBBuffer);
		_HBBuffer = nullptr;
	}

	if (_HBFont)
	{
		hb_font_destroy(_HBFont);
		_HBFont = nullptr;
	}

	if (_FTStroker)
	{
		FT_Stroker_Done(_FTStroker);
		_FTStroker = nullptr;
	}

	if (_FTFace)
	{
		FT_Done_Face(_FTFace);
		_FTFace = nullptr;
	}

	MU_SAFE_DELETE_ARRAY(_FontBuffer);

	mu_zeromem(&_Characters, sizeof(_Characters));
}

void MUFont::Update()
{
	for (mu_uint32 index = 0;index < _Atlas.size(); ++index)
	{
		MUTextureAtlas *atlas = _Atlas[index];

		if(atlas->bNeedsUpdate)
		{
			_Texture.FlushAsTexture2DArray(&atlas->AtlasData, 0, 0, 0, index, atlas->AtlasWidth, atlas->AtlasHeight, 1, FONT_TEXTURE_FORMAT, atlas->AtlasWidth * atlas->BytesPerPixel);

			atlas->bNeedsUpdate = false;
		}
	}
}

MUFontGlyph *MUFont::GetCharacter(mu_uint32 GlyphIndex)
{
	if (_Characters[GlyphIndex].Valid == true)
	{
		return &_Characters[GlyphIndex];
	}

	MUFontGlyph *charGlyph = &_Characters[GlyphIndex];

	mu_int32 error = FT_Load_Glyph(_FTFace, GlyphIndex, FT_LOAD_NO_BITMAP | FT_LOAD_TARGET_NORMAL);
	if (error != 0)
	{
		return nullptr;
	}

	mu_uint32 width;
	mu_uint32 rows;
	mu_int32 bitmapLeft;
	mu_int32 bitmapTop;
	std::vector<mu_uint8> imageBuffer;

	if (_OutlineSize > 0.0f)
	{
		FT_Glyph glyph;
		error = FT_Get_Glyph(_FTFace->glyph, &glyph);
		if (error == 0)
			error = FT_Glyph_Stroke(&glyph, _FTStroker, true);
		if (error == 0)
			error = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
		if (error == 0)
		{
			FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
			FT_Bitmap *bitmap = &bitmapGlyph->bitmap;

			if(imageBuffer.size() == 0)
			{
				width = bitmap->width;
				rows = bitmap->rows;
				bitmapLeft = bitmapGlyph->left;
				bitmapTop = bitmapGlyph->top;
				imageBuffer.resize(width * rows * 2, 0);

				for (mu_uint32 y = 0; y < bitmap->rows; ++y)
				{
					const mu_uint32 srcy = y * bitmap->pitch;
					const mu_uint32 dsty = y * width;
					for (mu_uint32 x = 0; x < bitmap->width; ++x)
					{
						const mu_uint32 srcindex = srcy + x;
						const mu_uint32 dstindex = dsty + x;

						imageBuffer[dstindex * 2 + 1] = bitmap->buffer[srcindex];
					}
				}
			}

			FT_Done_Glyph(glyph);
		}
	}

	// Fill
	{
		FT_Glyph glyph;
		error = FT_Get_Glyph(_FTFace->glyph, &glyph);
		if (error == 0)
			error = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
		if (error == 0)
		{
			FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
			FT_Bitmap *bitmap = &bitmapGlyph->bitmap;

			if (imageBuffer.size() == 0)
			{
				width = bitmap->width;
				rows = bitmap->rows;
				bitmapLeft = bitmapGlyph->left;
				bitmapTop = bitmapGlyph->top;
				imageBuffer.resize(width * rows * 2, 0);

				for (mu_uint32 y = 0; y < bitmap->rows; ++y)
				{
					const mu_uint32 srcy = y * bitmap->pitch;
					const mu_uint32 dsty = y * width;
					for (mu_uint32 x = 0; x < bitmap->width; ++x)
					{
						const mu_uint32 srcindex = srcy + x;
						const mu_uint32 dstindex = dsty + x;

						imageBuffer[dstindex * 2 + 0] = bitmap->buffer[srcindex];
					}
				}
			}
			else
			{
				if (imageBuffer.size() < width * rows * 2)
				{
					FORCED_DEBUGBREAK();
				}

				const mu_uint32 offsetx = (width - bitmap->width) / 2;
				const mu_uint32 offsety = (rows - bitmap->rows) / 2;

				for (mu_uint32 y = 0; y < bitmap->rows; ++y)
				{
					const mu_uint32 srcy = y * bitmap->width;
					const mu_uint32 dsty = (y + offsety) * width;
					for (mu_uint32 x = 0; x < bitmap->width; ++x)
					{
						const mu_uint32 srcindex = srcy + x;
						const mu_uint32 dstindex = dsty + (x + offsetx);
						
						imageBuffer[dstindex * 2 + 0] = bitmap->buffer[srcindex];
					}
				}
			}

			FT_Done_Glyph(glyph);
		}
	}

	const MUTextureAtlasSlot *slot = nullptr;
	mu_uint32 textureIndex = 0;

	for (auto it = _Atlas.begin(); it != _Atlas.end(); ++it, ++textureIndex)
	{
		MUTextureAtlas *atlas = (*it);

		slot = atlas->AddTexture(width, rows, imageBuffer.data(), rows * width * 2, width);

		if (slot != nullptr)
		{
			break;
		}
	}

	if (slot == nullptr)
	{
		return nullptr;
	}

	charGlyph->Valid = true;
	charGlyph->TextureIndex = (mu_float)textureIndex;
	charGlyph->StartU = slot->X;
	charGlyph->StartV = slot->Y;
	charGlyph->SizeU = slot->Width;
	charGlyph->SizeV = slot->Height;
	charGlyph->BearingX = bitmapLeft;
	charGlyph->BearingY = bitmapTop;

	return charGlyph;
}

MUFontFactory::MUFontFactory() : _FTLibrary(nullptr)
{
}

MUFontFactory::~MUFontFactory()
{
	DestroyFonts();

	if (_FTLibrary)
	{
		FT_Done_FreeType(_FTLibrary);
		_FTLibrary = nullptr;
	}
}

mu_boolean MUFontFactory::Initialize()
{
	if (FT_Init_FreeType(&_FTLibrary) != 0)
	{
		return false;
	}

	return true;
}

mu_boolean MUFontFactory::CreateFonts()
{
	for (mu_uint32 index = 0; index < MAX_FONT; ++index)
	{
		_Fonts[index].Destroy();
	}

	const mu_uint32 texture2DMaxSize = EVideoBackend::GetTextureMaxSize(ETextureType::eTexture2D);
	_TextureArraySize = TEXTURE_SIZE / MU_MIN(TEXTURE_SIZE, texture2DMaxSize);
	_TextureSize = MU_MIN(TEXTURE_SIZE, texture2DMaxSize);

	const mu_float FontHeight[MAX_FONT] = { 7.0f, 7.0f, 14.0f, 10.0f };
	const mu_float FontOutline[MAX_FONT] = { 0.0f, 0.0f, 0.0f, 2.0f };
	for (mu_uint32 index = 0; index < MAX_FONT; ++index)
	{
		if (_Fonts[index].Create(_FTLibrary, g_FontPath[index], FontHeight[index], _TextureSize, _TextureArraySize, FontOutline[index]) == false)
		{
			return false;
		}
	}

	return true;
}

void MUFontFactory::DestroyFonts()
{
	for (mu_uint32 index = 0; index < MAX_FONT; ++index)
	{
		_Fonts[index].Destroy();
	}
}

void MUFontFactory::Update()
{
	for (mu_uint32 index = 0; index < MAX_FONT; ++index)
	{
		_Fonts[index].Update();
	}
}

MUFont *MUFontFactory::GetFont(mu_uint32 Index)
{
	return &_Fonts[Index];
}