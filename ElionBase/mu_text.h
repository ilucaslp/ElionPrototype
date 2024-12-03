#ifndef __MU_TEXT_H__
#define __MU_TEXT_H__

#pragma once

class MUFont;

enum TEXT_ALIGN
{
	ETA_LEFT = 0,
	ETA_BOTTOM = 0,
	ETA_CENTER = 1,
	ETA_RIGHT = 2,
	ETA_TOP = 2,

	ETF_DISABLE_AUTOPOS = (1 << 7),
};

class MUText
{
public:
	MUText(MUFont *font);
	~MUText();

public:
	mu_boolean Create(mu_string Text, EVector2 Position, mu_float BorderWidth, mu_float BoxWidth, mu_float BoxHeight);
	mu_boolean Create(mu_string Text, EVector3 Position, mu_float BorderWidth, mu_float BoxWidth, mu_float BoxHeight);
	void Destroy();

	void Update();
	void Render(const ECombinedShader programShader, const mu_boolean isFocus);

	mu_boolean Is3D()
	{
		return _Is3D;
	}

	const EVector2 &GetSize()
	{
		return _Size;
	}

private:
	friend class MUTextSimpleRender;
	friend class UISimpleChat;

	MUFont *_Font;
	mu_string _Text;

	mu_uint32 _TextInstanceCount;
	mu_uint32 _BackInstanceCount;

	mu_float _Scale;
	mu_float _BoxWidth, _BoxHeight;

	EVector3 _Position;
	EVector2 _Border;
	EVector2 _BackupSize;
	EVector2 _Size;
	EVector4 _OutlineColor;

	EMatrix4 _Model, _View, _Projection;

	std::vector<mu_boolean> _ConstantUpdate;
	mu_boolean _InstanceUpdate, _PositionUpdate;
	mu_boolean _Is3D, _IsReady, _EnableHighlightCorrection;
	mu_uint8 _HorizontalAlign, _VerticalAlign;

private:
	mu_uint32 _InstanceCount;
	mu_uint32 _InstanceIndex;
	std::vector<EBufferBackend> _ConstantBuffer;
};

class MUTextSimpleRender
{
public:
	MUTextSimpleRender(MUFont *Font, const ECombinedShader ShaderType);
	~MUTextSimpleRender();

	void ClearCache();
	void SystemConfigure();

	mu_boolean DrawString(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Z, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, const mu_text *Text, mu_float BoxWidth, mu_float BoxHeight, MUText **Buffer, mu_boolean is3D, mu_boolean HighlightCorrection, const EVector4 &OutlineColor);
	mu_boolean DrawStringC2D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, mu_uint32 TextColor, mu_uint32 BackColor, const mu_text *Text, mu_float BoxWidth = 0.0f, mu_float BoxHeight = 0.0f, MUText **Buffer = nullptr, mu_boolean HighlightCorrection = true);
	mu_boolean DrawStringOutlineC2D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, mu_uint32 TextColor, mu_uint32 BackColor, mu_uint32 OutlineColor, const mu_text *Text, mu_float BoxWidth = 0.0f, mu_float BoxHeight = 0.0f, MUText **Buffer = nullptr, mu_boolean HighlightCorrection = true);
	mu_boolean DrawStringC3D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Z, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, mu_uint32 TextColor, mu_uint32 BackColor, const mu_text *Text, mu_float BoxWidth = 0.0f, mu_float BoxHeight = 0.0f, MUText **Buffer = nullptr, mu_boolean HighlightCorrection = true);
	mu_boolean DrawStringOutlineC3D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Z, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, mu_uint32 TextColor, mu_uint32 BackColor, mu_uint32 OutlineColor, const mu_text *Text, mu_float BoxWidth = 0.0f, mu_float BoxHeight = 0.0f, MUText **Buffer = nullptr, mu_boolean HighlightCorrection = true);
	mu_boolean DrawString2D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, const mu_text *Text, mu_float BoxWidth = 0.0f, mu_float BoxHeight = 0.0f, MUText **Buffer = nullptr, mu_boolean HighlightCorrection = true);
	mu_boolean DrawStringOutline2D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, mu_uint32 OutlineColor, const mu_text *Text, mu_float BoxWidth = 0.0f, mu_float BoxHeight = 0.0f, MUText **Buffer = nullptr, mu_boolean HighlightCorrection = true);
	mu_boolean DrawString3D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Z, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, const mu_text *Text, mu_float BoxWidth = 0.0f, mu_float BoxHeight = 0.0f, MUText **Buffer = nullptr, mu_boolean HighlightCorrection = true);
	mu_boolean DrawStringOutline3D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Z, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, mu_uint32 OutlineColor, const mu_text *Text, mu_float BoxWidth = 0.0f, mu_float BoxHeight = 0.0f, MUText **Buffer = nullptr, mu_boolean HighlightCorrection = true);

	void GetTextExtentPoint(const mu_text *Text, mu_size Length, mu_uint8 HorizontalAlign, mu_float Scale, EVector2 *Size, mu_uint32 CaretIndex = 0, mu_float *CaretX = nullptr, mu_float *CaretY = nullptr);
	void UpdateCoord(mu_int32 UniqueId, mu_float &CoordX, mu_float &CoordY);
	mu_uint32 GetCharacterIndexByMouse(mu_int32 UniqueId, mu_float MouseX, mu_float MouseY, const mu_text *Text, mu_uint8 HorizontalAlign = ETA_LEFT);

	MUFont *GetFont()
	{
		return _Font;
	}

private:
	std::map<mu_int32, MUText*> _Cache;
	MUFont *_Font;
	ECombinedShader _ShaderType;
};

#endif