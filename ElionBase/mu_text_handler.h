#ifndef __MU_TEXT_HELPER_H__
#define __MU_TEXT_HELPER_H__

#pragma once

namespace ETextHandler
{
	// Color Format is '\uE000'XXXXXXXX'\uE000'
	constexpr mu_text TextColorHeader = _T('\uE000');
	constexpr mu_text BackColorHeader = _T('\uE001');
	constexpr mu_uint32 ColorHeaderFormatSize = 1;
	constexpr mu_uint32 ColorDataFormatSize = 8;
	constexpr mu_uint32 ColorFormatSize = ColorHeaderFormatSize * 2 + ColorDataFormatSize;
	// Text + Back Color
	constexpr mu_uint32 DefaultColorFormatSize = ColorFormatSize * 2;
	constexpr mu_uint32 DefaultTextColor = MU::MakeARGB(0, 0, 0, 255);
	constexpr mu_uint32 DefaultBackColor = MU::MakeARGB(0, 0, 0, 0);

	enum class ETextType : mu_uint8
	{
		eColor,
		eText,
		eNewLine,
		eTab,
		eMax,
		eInvalid = 0xFF,
	};

	struct ETextBack
	{
		struct 
		{
			mu_boolean FirstRow : 1;
			mu_boolean LastRow : 1;
			mu_boolean FirstColumn : 1;
			mu_boolean LastColumn : 1;
		};

		mu_uint32 Color;
		mu_uint32 Line;
		mu_float X;
		mu_float Width;
	};

	struct ETextColor
	{
		ETextColor() {}

		ETextColor(const mu_uint32 back, const mu_uint32 text) : Back(back), Text(text)
		{
		}

		ETextColor(const ETextColor &color) : Back(color.Back), Text(color.Text)
		{
		}

		mu_uint32 Back;
		mu_uint32 Text;
	};

	struct ETextPartition
	{
		mu_uint32 Index;
		mu_uint32 Count;
		ETextType Type;
		ETextColor Color;
		mu_uint32 Spaces;
		mu_uint32 Colors; // used for know real index
	};

	void ParseTextLine(MUFont *font,
		hb_font_t *hbfont,
		hb_buffer_t *hbbuffer,
		const std::vector<ETextPartition>::iterator iter,
		const std::vector<ETextPartition>::iterator iterEnd,
		const mu_text *text,
		const mu_uint32 textlength,
		mu_uint32 &glyphCount,
		hb_glyph_info_t *&glyphInfo,
		hb_glyph_position_t *&glyphPos);
	
	const mu_uint32 CopyStringWithColor(mu_text *output, const mu_uint32 ouputLength, const mu_text *input, const mu_uint32 inputLength, const mu_uint32 textColor, const mu_uint32 backColor);

	const ETextType FindType(const mu_text *text, const mu_uint32 length, const mu_uint32 n);

	void PreparePartitions(const mu_text *text, const mu_uint32 length, std::vector<ETextPartition> &partitions);
	void PreparePartitionsNoColor(const mu_text *text, const mu_uint32 length, std::vector<ETextPartition> &partitions);

	void CalculateInstancesCount(std::vector<ETextPartition> &partitions, mu_uint32 &linesCount, mu_uint32 &textCount, mu_uint32 &backCount);
};

#endif