#include "stdafx.h"
#include "mu_text_handler.h"

namespace ETextHandler
{
	void ParseTextLine(MUFont *font,
		hb_font_t *hbfont,
		hb_buffer_t *hbbuffer,
		const std::vector<ETextPartition>::iterator iter,
		const std::vector<ETextPartition>::iterator iterEnd,
		const mu_text *text,
		const mu_uint32 textlength,
		mu_uint32 &glyphCount,
		hb_glyph_info_t *&glyphInfo,
		hb_glyph_position_t *&glyphPos)
	{
		glyphCount = 0;
		glyphInfo = nullptr;
		glyphPos = nullptr;

		hb_buffer_clear_contents(hbbuffer);
		hb_buffer_set_direction(hbbuffer, HB_DIRECTION_LTR);
		hb_buffer_set_script(hbbuffer, HB_SCRIPT_LATIN);
		hb_buffer_set_language(hbbuffer, hb_language_from_string("en", 2));

		for (auto it = iter; it != iterEnd; ++it)
		{
			ETextPartition &partition = *it;
			if (partition.Type == ETextType::eColor)
			{
				continue;
			}
			if (partition.Type != ETextType::eText)
			{
				break;
			}

#ifdef UNICODE
#if __ANDROID__
			hb_buffer_add_utf32(hbbuffer, reinterpret_cast<const mu_uint32*>(text), textlength, partition.Index, partition.Count);
#else
			hb_buffer_add_utf16(hbbuffer, reinterpret_cast<const mu_uint16*>(text), textlength, partition.Index, partition.Count);
#endif
#else
			hb_buffer_add_utf8(hbbuffer, text, textlength, partition.Index, partition.Count);
#endif
		}

		hb_shape(hbfont, hbbuffer, font->_Features.empty() ? nullptr : font->_Features.data(), font->_Features.size());

		glyphInfo = hb_buffer_get_glyph_infos(hbbuffer, &glyphCount);
		glyphPos = hb_buffer_get_glyph_positions(hbbuffer, &glyphCount);
	}

	const mu_uint32 CopyStringWithColor(mu_text *output, const mu_uint32 ouputLength, const mu_text *input, const mu_uint32 inputLength, const mu_uint32 textColor, const mu_uint32 backColor)
	{
		mu_sprintf_s(output, ouputLength, SPF_CHAR _T("%08X") SPF_CHAR SPF_CHAR _T("%08X") SPF_CHAR, TextColorHeader, textColor, TextColorHeader, BackColorHeader, backColor, BackColorHeader);
		mu_memcpy(output + DefaultColorFormatSize, input, inputLength * sizeof(mu_text));
		output[DefaultColorFormatSize + inputLength] = _T('\0');
		return DefaultColorFormatSize + inputLength;
	}

	const ETextType FindType(const mu_text *text, const mu_uint32 length, const mu_uint32 n)
	{
		switch (text[n])
		{
		case TextColorHeader:
			{
				if (length - n < ColorFormatSize)
				{
					return ETextType::eText;
				}

				if (text[n + ColorFormatSize - 1] != TextColorHeader)
				{
					return ETextType::eText;
				}

				const mu_uint32 endJ = n + ColorDataFormatSize + 1;
				for (mu_uint32 j = n + 1; j < endJ; ++j)
				{
					if (mu_isxdigit(text[j]) == 0)
					{
						return ETextType::eText;
					}
				}
			}
			return ETextType::eColor;

		case BackColorHeader:
			{
				if (length - n < ColorFormatSize)
				{
					return ETextType::eText;
				}

				if (text[n + ColorFormatSize - 1] != BackColorHeader)
				{
					return ETextType::eText;
				}

				const mu_uint32 endJ = n + ColorDataFormatSize + 1;
				for (mu_uint32 j = n + 1; j < endJ; ++j)
				{
					if (mu_isxdigit(text[j]) == 0)
					{
						return ETextType::eText;
					}
				}
			}
			return ETextType::eColor;

		case _T('\n'):
			return ETextType::eNewLine;

		case _T('\t'):
			return ETextType::eTab;
		}

		return ETextType::eText;
	}

	void PreparePartitions(const mu_text *text, const mu_uint32 length, std::vector<ETextPartition> &partitions)
	{
		// Default Back & Text Color
		ETextPartition tmp = {};
		tmp.Type = ETextType::eInvalid;
		tmp.Color.Back = DefaultBackColor;
		tmp.Color.Text = DefaultTextColor;

		mu_uint32 colors = 0;
		for (mu_uint32 n = 0; n < length;)
		{
			const ETextType type = FindType(text, length, n);

			if (tmp.Type != type)
			{
				if (tmp.Type != ETextType::eInvalid)
				{
					partitions.push_back(tmp);
				}

				tmp.Index = n;
				tmp.Count = 0;
				tmp.Type = type;
				tmp.Spaces = 0;
				tmp.Colors = colors;
			}

			switch (type)
			{
			case ETextType::eColor:
				{
					switch (text[n])
					{
					case TextColorHeader:
						{
							mu_text colorBuffer[ColorDataFormatSize + 1];
							mu_memcpy(colorBuffer, &text[n + 1], sizeof(colorBuffer));
							colorBuffer[ColorDataFormatSize] = _T('\0');

							tmp.Count += ColorFormatSize;
							tmp.Color.Text = mu_strtoul(colorBuffer, nullptr, 16);
						}
						break;

					case BackColorHeader:
						{
							mu_text colorBuffer[ColorDataFormatSize + 1];
							mu_memcpy(colorBuffer, &text[n + 1], sizeof(colorBuffer));
							colorBuffer[ColorDataFormatSize] = _T('\0');

							tmp.Count += ColorFormatSize;
							tmp.Color.Back = mu_strtoul(colorBuffer, nullptr, 16);
						}
						break;
					}

					n += ColorFormatSize;
					colors += ColorFormatSize;
				}
				break;

			case ETextType::eNewLine:
				{
					++tmp.Count;
					++n;
				}
				break;

			case ETextType::eTab:
				{
					++tmp.Count;
					++n;
				}
				break;

			default:
				{
					if (text[n] == _T(' '))
					{
						++tmp.Spaces;
					}

					++tmp.Count;
					++n;
				}
				break;
			}
		}

		if (tmp.Type != ETextType::eInvalid)
		{
			partitions.push_back(tmp);
		}
	}

	void PreparePartitionsNoColor(const mu_text *text, const mu_uint32 length, std::vector<ETextPartition> &partitions)
	{
		// Default Back & Text Color
		ETextPartition tmp = {};
		tmp.Type = ETextType::eInvalid;
		tmp.Color.Back = DefaultBackColor;
		tmp.Color.Text = DefaultTextColor;

		mu_uint32 colors = 0;
		for (mu_uint32 n = 0; n < length;)
		{
			const ETextType type = FindType(text, length, n);

			if (tmp.Type != type &&
				type != ETextType::eColor)
			{
				if (tmp.Type != ETextType::eInvalid)
				{
					partitions.push_back(tmp);
				}

				tmp.Index = n;
				tmp.Count = 0;
				tmp.Type = type;
				tmp.Spaces = 0;
				tmp.Colors = colors;
			}

			switch (type)
			{
			case ETextType::eColor:
				{
					n += ColorFormatSize;
					colors += ColorFormatSize;
				}
				break;

			case ETextType::eNewLine:
				{
					++tmp.Count;
					++n;
				}
				break;

			case ETextType::eTab:
				{
					++tmp.Count;
					++n;
				}
				break;

			default:
				{
					if (text[n] == _T(' '))
					{
						++tmp.Spaces;
					}

					++tmp.Count;
					++n;
				}
				break;
			}
		}

		if (tmp.Type != ETextType::eInvalid)
		{
			partitions.push_back(tmp);
		}
	}

	void CalculateInstancesCount(std::vector<ETextPartition> &partitions, mu_uint32 &linesCount, mu_uint32 &textCount, mu_uint32 &backCount)
	{
		linesCount = 0;
		textCount = 0;
		backCount = 0;

		mu_uint32 newLine = 1;
		mu_boolean hasBackground = false;
		//mu_boolean wasNewLine = false;
		for (auto iter = partitions.begin(); iter != partitions.end(); ++iter)
		{
			ETextPartition &partition = *iter;

			linesCount += newLine;
			newLine = 0;

			switch(partition.Type)
			{
			case ETextType::eColor:
				{
					if (MU::GetAlpha(partition.Color.Back) > 0)
					{
						hasBackground = true;
						/*
							Prevent to create multiples background when new line
							
							Example:
							eColor (Add Backcount)
							eNewLine (Add Backcount [next line])
							eColor  (Add Backcount [only if not new line])
							eText
						*/
						++backCount;
					}
					else
					{
						hasBackground = false;
					}
				}
				break;

			case ETextType::eNewLine:
				{
					mu_uint32 l = partition.Count - 1;

					for (auto niter = iter + 1; niter != partitions.end(); ++niter)
					{
						ETextPartition &tmp = *niter;
						if (tmp.Type != ETextType::eColor)
						{
							++l;
							break;
						}
					}

					newLine = l;

					if (hasBackground == true)
					{
						backCount += l;
					}
				}
				break;

			case ETextType::eText:
				{
					textCount += partition.Count - partition.Spaces;
				}
				break;
			}
		}
		
		linesCount += newLine;
		newLine = 0;
	}
};