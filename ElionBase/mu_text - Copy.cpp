#include "stdafx.h"
#include "mu_text.h"

mu_text *g_TextBuffer = nullptr;
mu_uint32 g_TextBufferLength = 0;
const mu_uint32 g_ExtraTextBufferLength = 10 * 2; // Text and Back Color
const mu_float TextTrickValue = FLT_MAX;
struct COLOR_SECTION
{
	mu_uint32 Index;
	mu_uint32 Color;
};
const mu_uint32 ColorMinLength = 10;
enum
{
	COLOR_LENGTH = sizeof(mu_uint32) * 2 + 1,
	COLOR_ZERO = COLOR_LENGTH - 1,
};

SHNormalFontVertex *g_FontVertexBuffer = nullptr;
mu_uint32 g_FontVertexBufferLength = 0;

using namespace DirectX;

MUText::MUText(MUFont *font) : _Font(font),
							   _VertexBuffer(nullptr), _VSConstantBuffer(nullptr),
							   _VertexUpdate(false), _VSConstantUpdate(false), _VSPositionUpdate(false),
							   _VertexCount(0)
{

}

MUText::~MUText()
{
	MU_SAFE_RELEASE(_VertexBuffer);
	MU_SAFE_RELEASE(_VSConstantBuffer);
}

mu_boolean MUText::CreateBuffer()
{
	D3D11_USAGE bufferUsage = D3D11_USAGE_DEFAULT;
	UINT cpuFlag = 0;

	if(_VertexBuffer == nullptr && _MaxLength > 0)
	{
		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.Usage = bufferUsage;
		vertexBufferDesc.ByteWidth = sizeof(SHNormalFontVertex) * _MaxLength * SHNormalFontVertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = cpuFlag;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		if (FAILED(MUVideoManager::_Device->CreateBuffer(&vertexBufferDesc, nullptr, &_VertexBuffer)))
		{
			return false;
		}

		_VertexUpdate = true;
	}

	if(_VSConstantBuffer == nullptr)
	{
		D3D11_BUFFER_DESC constantBufferDesc;
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.ByteWidth = sizeof(SHNormalFontConstantBufferVS);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.StructureByteStride = 0;

		if (FAILED(MUVideoManager::_Device->CreateBuffer(&constantBufferDesc, nullptr, &_VSConstantBuffer)))
		{
			MU_SAFE_RELEASE(_VertexBuffer);
			return false;
		}

		_VSConstantUpdate = true;
	}

	return true;
}

mu_boolean MUText::Create(mu_string Text, EVector2 Position, mu_float BorderWidth, mu_float BoxWidth, mu_float BoxHeight)
{
	EVector2 Border;
	Position.x = Position.x;
	Position.y = Position.y;
	Border.x = BorderWidth;
	Border.y = BorderWidth;

	_Text = Text;
	_Is3D = false;
	_VertexUpdate = true;
	_MaxLength = 0;
	_Scale = 1.0f;
	_Position = EVector3(Position.x, Position.y, 1.0f);
	_Border = Border;
	_BoxWidth = BoxWidth;
	_BoxHeight = BoxHeight;
	_Size = EVector2(0.0f, 0.0f);

	return true;
}

mu_boolean MUText::Create(mu_string Text, EVector3 Position, mu_float BorderWidth, mu_float BoxWidth, mu_float BoxHeight)
{
	EVector2 Border;
	Border.x = BorderWidth;
	Border.y = BorderWidth;

	_Text = Text;
	_Is3D = true;
	_VertexUpdate = true;
	_MaxLength = 0;
	_Scale = 1.0f;
	_Position = Position;
	_Border = Border;
	_BoxWidth = BoxWidth;
	_BoxHeight = BoxHeight;
	_Size = EVector2(0.0f, 0.0f);

	return true;
}

void MUText::Update()
{
	if (CreateBuffer() == false)
	{
		return;
	}

	mu_uint32 TextVertexLength = 0;

	SHNormalFontVertex *TextVertex = nullptr;
	SHNormalFontConstantBufferVS *VSTextConstant = nullptr;

	hb_font_t *hbfont;
	hb_buffer_t *hbbuffer;

	const mu_text *srctext = _Text.c_str();
	mu_size srctextsize = _Text.size();
	mu_text *text = nullptr, *hbtext = nullptr;
	mu_size textsize = 0, hbtextsize = 0;
	std::vector<COLOR_SECTION> colorSections;
	std::vector<COLOR_SECTION> backSections;
	mu_uint32 removeLength = 0;
	mu_uint32 extraVertex = 0;

	// Preprocess all colors and sections so we know how many vertex we would need and recreate the buffer if the vertex amount isn't enough
	if (_VertexUpdate == true)
	{
		hbfont = _Font->_HBFont;
		hbbuffer = _Font->_HBBuffer;
		hb_buffer_reset(hbbuffer);
		hb_buffer_set_direction(hbbuffer, HB_DIRECTION_LTR);
		hb_buffer_set_script(hbbuffer, HB_SCRIPT_LATIN);
		hb_buffer_set_language(hbbuffer, hb_language_from_string("en", 2));

		text = new mu_text[srctextsize + 1];
		hbtext = new mu_text[srctextsize + 1];

		mu_boolean createBack = true;

		for (mu_uint32 index = 0; index < srctextsize; index++)
		{
			switch (srctext[index])
			{
			case _T('\uE000'):
				{
					if (srctextsize - index < ColorMinLength)
						break;

					if (srctext[index + ColorMinLength - 1] != _T('\uE000'))
						break;

					mu_boolean isValid = true;

					mu_text color[COLOR_LENGTH];

					for (mu_uint32 index2 = index + 1, colorIndex = 0; index2 < index + ColorMinLength - 1; index2++, colorIndex++)
					{
						if (isxdigit(srctext[index2]) == 0)
						{
							isValid = false;
							break;
						}

						color[colorIndex] = srctext[index2];
					}

					color[COLOR_ZERO] = _T('\0');

					if (isValid == false)
						break;

					removeLength += ColorMinLength;

					COLOR_SECTION colorSection;
					colorSection.Index = index + ColorMinLength - removeLength;
					colorSection.Color = mu_strtoul(color, nullptr, 16);

					colorSections.push_back(colorSection);

					index += ColorMinLength - 1;
					continue;
				}
				break;
			case _T('\uE001'):
				{
					if (srctextsize - index < ColorMinLength)
						break;

					if (srctext[index + ColorMinLength - 1] != _T('\uE001'))
						break;

					mu_boolean isValid = true;

					mu_text color[COLOR_LENGTH];

					for (mu_uint32 index2 = index + 1, colorIndex = 0; index2 < index + ColorMinLength - 1; index2++, colorIndex++)
					{
						if (isxdigit(srctext[index2]) == 0)
						{
							isValid = false;
							break;
						}

						color[colorIndex] = srctext[index2];
					}

					color[COLOR_ZERO] = _T('\0');

					if (isValid == false)
						break;

					removeLength += ColorMinLength;

					COLOR_SECTION backSection;
					backSection.Index = index + ColorMinLength - removeLength;
					backSection.Color = mu_strtoul(color, nullptr, 16);

					backSections.push_back(backSection);

					index += ColorMinLength - 1;
					createBack = true;
					continue;
				}
				break;
			}

			if (createBack == true)
			{
				createBack = false;
				extraVertex++;
			}

			if (srctext[index] == _T('\n'))
			{
				createBack = true;
			}

			text[textsize++] = srctext[index];

			switch (srctext[index])
			{
			case _T('\n'):
			case _T('\t'):
			case _T('\r'):
				break;

			default:
				hbtext[hbtextsize++] = srctext[index];
				break;
			}
		}

		text[textsize++] = _T('\0');
		hbtext[hbtextsize++] = _T('\0');
	}

	if (hbtextsize + extraVertex > _MaxLength)
	{
		_MaxLength = hbtextsize + extraVertex;
		MU_SAFE_RELEASE(_VertexBuffer);
		if (CreateBuffer() == false)
		{
			return;
		}
	}

	if (_VertexUpdate)
	{
		if (_MaxLength > g_FontVertexBufferLength)
		{
			g_FontVertexBufferLength = _MaxLength;
			g_FontVertexBuffer = (SHNormalFontVertex *)mu_realloc(g_FontVertexBuffer, sizeof(SHNormalFontVertex) * g_FontVertexBufferLength * SHNormalFontVertexCount);
		}

		TextVertex = g_FontVertexBuffer;
	}

	if (_VSConstantUpdate)
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource = { 0 };
		mu_assert(SUCCEEDED(MUVideoManager::_DeviceContext->Map(_VSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource)));
		VSTextConstant = reinterpret_cast<SHNormalFontConstantBufferVS*>(mappedSubresource.pData);
	}

	//... all update process
	if (_VertexUpdate == true)
	{
#ifdef UNICODE
		hb_buffer_add_utf16(hbbuffer, (const mu_uint16*)hbtext, hbtextsize - 1, 0, hbtextsize - 1);
#else
		hb_buffer_add_utf8(hbbuffer, hbtext, hbtextsize - 1, 0, hbtextsize - 1);
#endif

		hb_shape(hbfont, hbbuffer, _Font->_Features.empty() ? nullptr : _Font->_Features.data(), _Font->_Features.size());

		mu_uint32 glyphCount;
		hb_glyph_info_t *glyphInfo = hb_buffer_get_glyph_infos(hbbuffer, &glyphCount);
		hb_glyph_position_t *glyphPos = hb_buffer_get_glyph_positions(hbbuffer, &glyphCount);

		mu_float defx = 0.0f;
		mu_float defy = -_Font->_Ascender;

		defx += _Border.x * 0.5f;
		defy -= _Border.y * 0.5f;

		mu_float x = _Border.x * 0.5f, y = defy, nextLine = defy, maxX = 0.0f, maxY = 0.0f;
		mu_float backX = 0.0f, backY0 = 0.0f, backY1 = y + _Font->_Descender;

		mu_uint32 currentColor = MU::MakeARGB(255, 255, 255, 255);
		mu_uint32 currentBack = MU::MakeARGB(0, 0, 0, 100);
		mu_uint32 nextColorIndex, nextColor = currentColor;
		mu_uint32 nextBackIndex, nextBack = currentBack;

		mu_float invTextureSize = _Font->_InvTextureSize;

		mu_int32 backVertex = -1;
		mu_uint32 backVertexCount = extraVertex;
		SHNormalFontVertex *newBack = nullptr;
		mu_uint32 vertexCount = extraVertex;

		mu_boolean createBack = true;

		if (colorSections.size() == 0)
		{
			nextColorIndex = (mu_uint32)-1;
		}
		else
		{
			auto it = colorSections.begin();
			COLOR_SECTION &section = *it;

			nextColorIndex = section.Index;
			nextColor = section.Color;

			colorSections.erase(it);
		}

		if (backSections.size() == 0)
		{
			nextBackIndex = (mu_uint32)-1;
		}
		else
		{
			auto it = backSections.begin();
			COLOR_SECTION &section = *it;

			nextBackIndex = section.Index;
			nextBack = section.Color;

			backSections.erase(it);
		}

		for (mu_uint32 index = 0, hbindex = 0;index < textsize - 1;index++)
		{
			if (index >= nextColorIndex)
			{
				currentColor = nextColor;

				auto it = colorSections.begin();

				if (it == colorSections.end())
				{
					nextColorIndex = (mu_uint32)-1;
				}
				else
				{
					COLOR_SECTION &section = *it;

					nextColorIndex = section.Index;
					nextColor = section.Color;

					colorSections.erase(it);
				}
			}

			if (index >= nextBackIndex)
			{
				if (backVertex >= 0 && newBack[0].Position.y == backY0)
				{
					newBack[2].Position.x = x;
					newBack[4].Position.x = x;
					newBack[5].Position.x = x;
				}

				currentBack = nextBack;

				auto it = backSections.begin();

				if (it == backSections.end())
				{
					nextBackIndex = (mu_uint32)-1;
				}
				else
				{
					COLOR_SECTION &section = *it;

					nextBackIndex = section.Index;
					nextBack = section.Color;

					backSections.erase(it);
				} 

				createBack = true;
			}

			if (createBack == true)
			{
				createBack = false;

				newBack = &TextVertex[++backVertex * SHNormalFontVertexCount];

				newBack[0].Position = EVector2(backX, backY0);
				newBack[1].Position = EVector2(backX, backY1);
				newBack[2].Position = EVector2(TextTrickValue, backY1);

				newBack[3].Position = EVector2(backX, backY0);
				newBack[4].Position = EVector2(TextTrickValue, backY1);
				newBack[5].Position = EVector2(TextTrickValue, backY0);

				newBack[0].UV = EVector3(0.0f, 0.0f, -1.0f);
				newBack[1].UV = EVector3(0.0f, 0.0f, -1.0f);
				newBack[2].UV = EVector3(0.0f, 0.0f, -1.0f);
				newBack[3].UV = EVector3(0.0f, 0.0f, -1.0f);
				newBack[4].UV = EVector3(0.0f, 0.0f, -1.0f);
				newBack[5].UV = EVector3(0.0f, 0.0f, -1.0f);

				newBack[0].Color = currentBack;
				newBack[1].Color = currentBack;
				newBack[2].Color = currentBack;
				newBack[3].Color = currentBack;
				newBack[4].Color = currentBack;
				newBack[5].Color = currentBack;
			}

			if (text[index] == _T('\n'))
			{
				backX = 0.0f;
				backY0 = backY1;
				nextLine -= _Font->_MaxAdvanceY;
				backY1 = nextLine + _Font->_Descender;
				x = defx, y = nextLine;
				createBack = true;

				if (maxX < x)
					maxX = x;
				if (maxY > y)
					maxY = y;

				continue;
			}
			else if (text[index] == _T('\t'))
			{
				/* (Characters Count * Font Height) / 2 */
				x += _Font->_MaxAdvanceX * 0.5f;

				if (maxX < x)
					maxX = x;
				if (maxY > y)
					maxY = y;

				continue;
			}

			mu_uint32 glyphIndex = glyphInfo[hbindex].codepoint;
			MUFontGlyph *glyph = _Font->GetCharacter(glyphIndex);
			mu_assert(glyph);

			mu_float U = glyph->StartU * invTextureSize;
			mu_float V = glyph->StartV * invTextureSize;
			mu_float SizeU = glyph->SizeU * invTextureSize;
			mu_float SizeV = glyph->SizeV * invTextureSize;

			mu_float xadvance = (mu_float)glyphPos[hbindex].x_advance / 64.0f;
			mu_float yadvance = (mu_float)glyphPos[hbindex].y_advance / 64.0f;
			mu_float xoffset = (mu_float)glyphPos[hbindex].x_offset / 64.0f;
			mu_float yoffset = (mu_float)glyphPos[hbindex].y_offset / 64.0f;
			mu_float x0 = x + xoffset + glyph->BearingX;
			mu_float y0 = EMath::Floor(y + yoffset + glyph->BearingY);
			mu_float x1 = x0 + glyph->SizeU;
			mu_float y1 = EMath::Floor(y0 - glyph->SizeV);

			SHNormalFontVertex *vertex = &TextVertex[(vertexCount++) * SHNormalFontVertexCount];

			vertex[0].Position = EVector2(x0, y0);
			vertex[1].Position = EVector2(x0, y1);
			vertex[2].Position = EVector2(x1, y1);
			vertex[0].UV = EVector3(U, V, glyph->TextureIndex);
			vertex[1].UV = EVector3(U, V + SizeV, glyph->TextureIndex);
			vertex[2].UV = EVector3(U + SizeU, V + SizeV, glyph->TextureIndex);

			vertex[3].Position = EVector2(x0, y0);
			vertex[4].Position = EVector2(x1, y1);
			vertex[5].Position = EVector2(x1, y0);
			vertex[3].UV = EVector3(U, V, glyph->TextureIndex);
			vertex[4].UV = EVector3(U + SizeU, V + SizeV, glyph->TextureIndex);
			vertex[5].UV = EVector3(U + SizeU, V, glyph->TextureIndex);

			vertex[0].Color = currentColor;
			vertex[1].Color = currentColor;
			vertex[2].Color = currentColor;
			vertex[3].Color = currentColor;
			vertex[4].Color = currentColor;
			vertex[5].Color = currentColor;

			x += xadvance;
			y += yadvance;

			backX = x;

			if (maxX < x)
				maxX = x;
			if (maxY > y)
				maxY = y;

			hbindex++;
		}

		_VertexCount = vertexCount * SHNormalFontVertexCount;

		mu_float backupX = maxX, backupY = maxY + _Font->_Descender;
		mu_float minX = 0.0f, minY = 0.0f, fixX = 0.0f, fixY = 0.0f;
		maxX += _Border.x * 0.5f;
		maxY += _Font->_Descender;
		maxY -= _Border.y * 0.5f;

		_BackupSize = EVector2(maxX, maxY);
		_Size = _BackupSize * _Scale;

		if (_BoxWidth != 0 ||
			_BoxHeight != 0)
		{
			if (maxX < _BoxWidth)
				fixX = (_BoxWidth - maxX);
			if (EMath::Abs(maxY) < _BoxHeight)
				fixY = (_BoxHeight - EMath::Abs(maxY));

			mu_boolean isHorizontalAutopos = (_HorizontalAlign & ETF_DISABLE_AUTOPOS) == 0;
			mu_boolean isVerticalAutopos = (_VerticalAlign & ETF_DISABLE_AUTOPOS) == 0;
			mu_uint8 Horizontal = (_HorizontalAlign & ((~0) ^ ETF_DISABLE_AUTOPOS));
			mu_uint8 Vertical = (_VerticalAlign & ((~0) ^ ETF_DISABLE_AUTOPOS));

			switch (Horizontal)
			{
			case ETA_LEFT:
				maxX += fixX;
				break;

			case ETA_CENTER:
				minX -= fixX * 0.5f;
				maxX += fixX * 0.5f;
				break;

			case ETA_RIGHT:
				minX -= fixX;
				break;
			}

			switch (Vertical)
			{
			case ETA_BOTTOM:
				maxY -= fixY;
				break;

			case ETA_CENTER:
				minY += fixY * 0.5f;
				maxY -= fixY * 0.5f;
				break;

			case ETA_TOP:
				minY += fixY;
				break;
			}

			for (mu_uint32 n = 0; n < backVertexCount; n++)
			{
				newBack = &TextVertex[n * SHNormalFontVertexCount];

				if (newBack[0].Position.x == 0.0f)
				{
					newBack[0].Position.x = minX;
					newBack[1].Position.x = minX;
					newBack[3].Position.x = minX;
				}

				if (newBack[2].Position.x == TextTrickValue)
				{
					newBack[2].Position.x = maxX;
					newBack[4].Position.x = maxX;
					newBack[5].Position.x = maxX;
				}

				if (newBack[0].Position.y == 0.0f)
				{
					newBack[0].Position.y = minY;
					newBack[3].Position.y = minY;
					newBack[5].Position.y = minY;
				}

				if (newBack[1].Position.y == backupY)
				{
					newBack[1].Position.y = maxY;
					newBack[2].Position.y = maxY;
					newBack[4].Position.y = maxY;
				}
			}
		}
		else
		{
			for (mu_uint32 n = 0; n < backVertexCount; n++)
			{
				newBack = &TextVertex[n * SHNormalFontVertexCount];

				if (newBack[2].Position.x == TextTrickValue)
				{
					newBack[2].Position.x = maxX;
					newBack[4].Position.x = maxX;
					newBack[5].Position.x = maxX;
				}

				if (newBack[1].Position.y == backupY)
				{
					newBack[1].Position.y = maxY;
					newBack[2].Position.y = maxY;
					newBack[4].Position.y = maxY;
				}
			}
		}

		TextVertexLength = _VertexCount * sizeof(SHNormalFontVertex);
	}

	MU_SAFE_DELETE_ARRAY(text);
	MU_SAFE_DELETE_ARRAY(hbtext);

	if (_VSConstantUpdate == true)
	{
		EVector3 position = _Position;

		mu_boolean isHorizontalAutopos = (_HorizontalAlign & ETF_DISABLE_AUTOPOS) == 0;
		mu_boolean isVerticalAutopos = (_VerticalAlign & ETF_DISABLE_AUTOPOS) == 0;
		mu_uint8 Horizontal = (_HorizontalAlign & ((~0) ^ ETF_DISABLE_AUTOPOS));
		mu_uint8 Vertical = (_VerticalAlign & ((~0) ^ ETF_DISABLE_AUTOPOS));

		_Size = _BackupSize * _Scale;

		if (_Is3D)
		{
			EFloat billboardSize = 1.0f;

			mu_boolean updateData = false;

			if (_VSPositionUpdate == true ||
				mu_memcmp(&_View, &MUVideoManager::_CurrentView, sizeof(EMatrix4)) != 0 ||
				mu_memcmp(&_Projection, &MUVideoManager::_CurrentProjection, sizeof(EMatrix4)) != 0)
			{
				updateData = true;

				billboardSize = EMath::RetrieveBillboardSize(MUVideoManager::_CurrentTextView,
					position,
					MUVideoManager::_CurrentCamera->GetCameraPosition(),
					MUVideoManager::_CurrentCamera->GetCameraInverseDirection(),
					MUCamera::RFOV,
					MUVideoManager::_3DViewport.Width,
					MUVideoManager::_3DViewport.Height);
			}

			EVector3 size = EVector3(_Size * billboardSize, 0.0f);

			switch (Horizontal)
			{
			case ETA_LEFT:
				size.x = 0.0f;
				break;

			case ETA_CENTER:
				size.x = -(size.x * 0.5f);
				break;

			case ETA_RIGHT:
				size.x = -size.x;
				break;
			}

			switch (Vertical)
			{
			case ETA_BOTTOM:
				size.y = 0.0f;
				break;

			case ETA_CENTER:
				size.y = -(size.y * 0.5f);
				break;

			case ETA_TOP:
				size.y = -size.y;
				break;
			}

			position += EMath::Transform(MUVideoManager::_CurrentTextView, size);

			mu_assert(MUVideoManager::_CurrentCamera != nullptr);
			if (updateData)
			{
				_Model = EMath::RetrieveBillboardAlternative(MUVideoManager::_CurrentTextView,
					position,
					billboardSize) * EMath::Scale(_Scale);
				_View = MUVideoManager::_CurrentView;
				_Projection = MUVideoManager::_CurrentProjection;
				_TextMatrix = _Projection *
							  _View  *
							  _Model;
			}
		}
		else
		{
			switch (Horizontal)
			{
			case ETA_LEFT:
				break;

			case ETA_CENTER:
				position.x -= _Size.x * 0.5f;
				break;

			case ETA_RIGHT:
				position.x -= _Size.x;
				break;
			}

			switch (Vertical)
			{
			case ETA_BOTTOM:
				break;

			case ETA_CENTER:
				position.y -= _Size.y * 0.5f;
				break;

			case ETA_TOP:
				position.y -= _Size.y;
				break;
			}

			if (isHorizontalAutopos)
			{
				if (position.x + _Size.x >= MU_RESOURCE->GetWindowHalfWidth())
				{
					position.x = MU_RESOURCE->GetWindowHalfWidth() - _Size.x;
				}
				else if (position.x < -MU_RESOURCE->GetWindowHalfWidth())
				{
					position.x = -MU_RESOURCE->GetWindowHalfWidth();
				}
			}

			if (isVerticalAutopos)
			{
				if (position.y + _Size.y >= MU_RESOURCE->GetWindowHalfHeight())
				{
					position.y = MU_RESOURCE->GetWindowHalfHeight() - _Size.y;
				}
				else if (position.y < -MU_RESOURCE->GetWindowHalfHeight())
				{
					position.y = -MU_RESOURCE->GetWindowHalfHeight();
				}
			}

			if (_VSPositionUpdate == true ||
				mu_memcmp(&_View, &MUVideoManager::_CurrentView2D, sizeof(EMatrix4)) != 0 ||
				mu_memcmp(&_Projection, &MUVideoManager::_CurrentProjection2D, sizeof(EMatrix4)) != 0)
			{
				_Model = EMath::Translate(position) * EMath::Scale(_Scale);
				_View = MUVideoManager::_CurrentView2D;
				_Projection = MUVideoManager::_CurrentProjection2D;
				_TextMatrix = _Projection *
							  _View *
							  _Model;
			}
		}

		VSTextConstant->PositionMatrix = _TextMatrix;
		VSTextConstant->Is3D = _Is3D == true ? 1.0f : 0.0f;
	}

	{
		D3D11_BOX box = { 0 };
		box.left = 0;
		box.top = 0;
		box.bottom = 1;
		box.front = 0;
		box.back = 1;

		if (_VertexUpdate && TextVertexLength > 0)
		{
			box.right = TextVertexLength;
			MUVideoManager::_DeviceContext->UpdateSubresource(_VertexBuffer, 0, &box, TextVertex, 0, 0);
		}
	}

	if (_VSConstantUpdate == true)
	{
		MUVideoManager::_DeviceContext->Unmap(_VSConstantBuffer, 0);
	}

	_VertexUpdate = false;
	_VSConstantUpdate = false;
	_VSPositionUpdate = false;
}

void MUText::Render(mu_boolean isFocus)
{
	MUVideoManager::PSSetShaderResources(0, 1, &_Font->_TextureView);

	mu_uint32 stride = sizeof(SHNormalFontVertex), offset = 0;
	MUVideoManager::_DeviceContext->IASetVertexBuffers(0, 1, &_VertexBuffer, &stride, &offset);
	MUVideoManager::_DeviceContext->VSSetConstantBuffers(1, 1, &_VSConstantBuffer);

	MUVideoManager::_DeviceContext->Draw(_VertexCount, 0);
}

MUTextSimpleRender::MUTextSimpleRender(MUFont *font,
											 mu_uint32 VSShader,
											 mu_uint32 PSShader) : _Font(font), _VSShader(VSShader), _PSShader(PSShader)
{

}

MUTextSimpleRender::~MUTextSimpleRender()
{
	for (auto it = _Cache.begin();it != _Cache.end();it = _Cache.erase(it))
	{
		delete it->second;
	}
}

mu_boolean MUTextSimpleRender::DrawString(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Z, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, const mu_text *Text, mu_float BoxWidth, mu_float BoxHeight, MUText **Buffer, mu_boolean is3D)
{
	auto it = _Cache.find(UniqueId);

	if (it == _Cache.end())
	{
		MUText *textData = new MUText(_Font);

		if (textData == nullptr)
		{
			return false;
		}

		if (textData->Create(Text, EVector3(X, Y, Z), BorderWidth, BoxWidth, BoxHeight) == false)
		{
			delete textData;
			return false;
		}

		auto iit = _Cache.insert(std::make_pair(UniqueId, textData));
		it = iit.first;

		if (iit.second == false)
		{
			return false;
		}
	}

	MUText *textData = it->second;

	if (Buffer != nullptr)
	{
		*Buffer = textData;
	}

	if (textData->_Position.x != X ||
		textData->_Position.y != Y ||
		textData->_Position.z != Z ||
		textData->_Scale != Scale ||
		textData->_HorizontalAlign != HorizontalAlign ||
		textData->_VerticalAlign != VerticalAlign)
	{
		textData->_Position = EVector3(X, Y, Z);
		textData->_Scale = Scale;
		textData->_HorizontalAlign = HorizontalAlign;
		textData->_VerticalAlign = VerticalAlign;
		textData->_VSConstantUpdate = true;
		textData->_VSPositionUpdate = true;

		if (BoxWidth != 0 ||
			BoxHeight != 0)
		{
			textData->_VertexUpdate = true;
		}
	}

	if (textData->_Text.compare(Text) != 0)
	{
		textData->_Text = Text;
		textData->_VertexUpdate = true;
		textData->_VSConstantUpdate = true;
		textData->_VSPositionUpdate = true;

		if (textData->_Text.size() > textData->_MaxLength)
		{
			textData->_MaxLength = textData->_Text.size();
			MU_SAFE_RELEASE(textData->_VertexBuffer);
		}
	}

	if (BoxWidth != textData->_BoxWidth ||
		BoxHeight != textData->_BoxHeight ||
		BorderWidth != textData->_Border.x)
	{
		textData->_BoxWidth = BoxWidth;
		textData->_BoxHeight = BoxHeight;
		textData->_Border = EVector2(BorderWidth, BorderWidth);
		textData->_VertexUpdate = true;
		textData->_VSConstantUpdate = true;
		textData->_VSPositionUpdate = true;
	}

	if (textData->_Is3D != is3D)
	{
		textData->_Is3D = is3D;
		textData->_VSConstantUpdate = true;
	}

	if(is3D)
	{
		if (mu_memcmp(&textData->_View, &MUVideoManager::_CurrentView, sizeof(EMatrix4)) != 0 ||
			mu_memcmp(&textData->_Projection, &MUVideoManager::_CurrentProjection, sizeof(EMatrix4)) != 0)
		{
			textData->_VSConstantUpdate = true;
		}
	}
	else
	{
		if (mu_memcmp(&textData->_View, &MUVideoManager::_CurrentView2D, sizeof(EMatrix4)) != 0 ||
			mu_memcmp(&textData->_Projection, &MUVideoManager::_CurrentProjection2D, sizeof(EMatrix4)) != 0)
		{
			textData->_VSConstantUpdate = true;
		}
	}

	textData->Update();

	const mu_float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	MUVideoManager::OMSetBlendState(BlendState::State[BlendState::ALPHASRC_INVALPHASRC], blendFactor, 0xFFFFFFFF);
	MUVideoManager::VSSetShader(_VSShader);
	MUVideoManager::PSSetShader(_PSShader);
	MUVideoManager::_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	MUVideoManager::PSSetSamplers(0, 1, &Sampler::State[Sampler::LINEAR_CLAMP]);
	MUVideoManager::RSSetState(Rasterizer::State[Rasterizer::DEFAULT]);
	MUVideoManager::OMSetDepthStencilState(DepthStencil::State[DepthStencil::NO_ZBUFFER], 0);

	textData->Render(false);

	return true;
}

mu_boolean MUTextSimpleRender::DrawStringC2D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, mu_uint32 TextColor, mu_uint32 BackColor, const mu_text *Text, mu_float BoxWidth, mu_float BoxHeight, MUText **Buffer)
{
	mu_size TextLength = mu_strlen(Text);
	if (g_TextBufferLength < TextLength + g_ExtraTextBufferLength + 1)
	{
		g_TextBufferLength = TextLength + g_ExtraTextBufferLength + 1;
		g_TextBuffer = (mu_text*)mu_realloc(g_TextBuffer, g_TextBufferLength * sizeof(mu_text));
	}

	mu_sprintf_s(g_TextBuffer, g_TextBufferLength, _T("\uE000%08X\uE000\uE001%08X\uE001"), TextColor, BackColor);
	mu_strcat_s(g_TextBuffer, g_TextBufferLength, Text);

	// Fix text alignment to nearest pixel
	X = EMath::Round(X);
	Y = EMath::Round(Y);

	MU_RESOURCE->Convert2D(X, Y);
	BorderWidth = MU_TO_WINDOW(BorderWidth, MU_RESOURCE->_640x480Rate[0]);
	return DrawString(UniqueId, X, Y, 0.0f, Scale, HorizontalAlign, VerticalAlign, BorderWidth, g_TextBuffer, BoxWidth, BoxHeight, Buffer, false);
}

mu_boolean MUTextSimpleRender::DrawStringC3D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Z, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, mu_uint32 TextColor, mu_uint32 BackColor, const mu_text *Text, mu_float BoxWidth, mu_float BoxHeight, MUText **Buffer)
{
	mu_size TextLength = mu_strlen(Text);
	if (g_TextBufferLength < TextLength + g_ExtraTextBufferLength + 1)
	{
		g_TextBufferLength = TextLength + g_ExtraTextBufferLength + 1;
		g_TextBuffer = (mu_text*)mu_realloc(g_TextBuffer, g_TextBufferLength * sizeof(mu_text));
	}

	mu_sprintf_s(g_TextBuffer, g_TextBufferLength, _T("\uE000%08X\uE000\uE001%08X\uE001"), TextColor, BackColor);
	mu_strcat_s(g_TextBuffer, g_TextBufferLength, Text);

	// Fix text alignment to nearest pixel
	X = EMath::Round(X);
	Y = EMath::Round(Y);
	Z = EMath::Round(Z);

	BorderWidth = MU_TO_WINDOW(BorderWidth, MU_RESOURCE->_640x480Rate[0]);
	return DrawString(UniqueId, X, Y, Z, Scale, HorizontalAlign, VerticalAlign, BorderWidth, g_TextBuffer, BoxWidth, BoxHeight, Buffer, true);
}

mu_boolean MUTextSimpleRender::DrawString2D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, const mu_text *Text, mu_float BoxWidth, mu_float BoxHeight, MUText **Buffer)
{
	// Fix text alignment to nearest pixel
	X = EMath::Round(X);
	Y = EMath::Round(Y);

	MU_RESOURCE->Convert2D(X, Y);
	BorderWidth = MU_TO_WINDOW(BorderWidth, MU_RESOURCE->_640x480Rate[0]);
	return DrawString(UniqueId, X, Y, 0.0f, Scale, HorizontalAlign, VerticalAlign, BorderWidth, Text, BoxWidth, BoxHeight, Buffer, false);
}

mu_boolean MUTextSimpleRender::DrawString3D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Z, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, const mu_text *Text, mu_float BoxWidth, mu_float BoxHeight, MUText **Buffer)
{
	// Fix text alignment to nearest pixel
	X = EMath::Round(X);
	Y = EMath::Round(Y);
	Z = EMath::Round(Z);

	BorderWidth = MU_TO_WINDOW(BorderWidth, MU_RESOURCE->_640x480Rate[0]);
	return DrawString(UniqueId, X, Y, Z, Scale, HorizontalAlign, VerticalAlign, BorderWidth, Text, BoxWidth, BoxHeight, Buffer, true);
}

void MUTextSimpleRender::GetTextExtentPoint(const mu_text *Text, mu_size Length, mu_float Scale, EVector2 *Size, mu_uint32 CaretIndex, mu_float *CaretX, mu_float *CaretY)
{
	hb_font_t *hbfont = _Font->_HBFont;
	hb_buffer_t *hbbuffer = _Font->_HBBuffer;
	hb_buffer_reset(hbbuffer);
	hb_buffer_set_direction(hbbuffer, HB_DIRECTION_LTR);
	hb_buffer_set_script(hbbuffer, HB_SCRIPT_LATIN);
	hb_buffer_set_language(hbbuffer, hb_language_from_string("en", 2));

	const mu_text *srctext = Text;
	mu_size srctextsize = Length;
	mu_text *text = new mu_text[srctextsize + 1];
	mu_text *hbtext = new mu_text[srctextsize + 1];
	mu_size textsize = 0;
	mu_size hbtextsize = 0;
	mu_uint32 removeLength = 0;

	for (mu_uint32 index = 0; index < srctextsize; index++)
	{
		switch (srctext[index])
		{
		case _T('\uE000'):
			{
				if (srctextsize - index < ColorMinLength)
					break;

				if (srctext[index + ColorMinLength - 1] != _T('\uE000'))
					break;

				mu_boolean isValid = true;

				mu_text color[COLOR_LENGTH];

				for (mu_uint32 index2 = index + 1, colorIndex = 0; index2 < index + ColorMinLength - 1; index2++, colorIndex++)
				{
					if (isxdigit(srctext[index2]) == 0)
					{
						isValid = false;
						break;
					}

					color[colorIndex] = srctext[index2];
				}

				color[COLOR_ZERO] = _T('\0');

				if (isValid == false)
					break;

				removeLength += ColorMinLength;

				index += ColorMinLength - 1;
				continue;
			}
			break;
		case _T('\uE001'):
			{
				if (srctextsize - index < ColorMinLength)
					break;

				if (srctext[index + ColorMinLength - 1] != _T('\uE001'))
					break;

				mu_boolean isValid = true;

				mu_text color[COLOR_LENGTH];

				for (mu_uint32 index2 = index + 1, colorIndex = 0; index2 < index + ColorMinLength - 1; index2++, colorIndex++)
				{
					if (isxdigit(srctext[index2]) == 0)
					{
						isValid = false;
						break;
					}

					color[colorIndex] = srctext[index2];
				}

				color[COLOR_ZERO] = _T('\0');

				if (isValid == false)
					break;

				removeLength += ColorMinLength;

				index += ColorMinLength - 1;
				continue;
			}
			break;
		}

		text[textsize++] = srctext[index];

		switch (srctext[index])
		{
		case _T('\n'):
		case _T('\t'):
		case _T('\r'):
			break;

		default:
			hbtext[hbtextsize++] = srctext[index];
			break;
		}
	}

	text[textsize++] = _T('\0');
	hbtext[hbtextsize++] = _T('\0');

#ifdef UNICODE
	hb_buffer_add_utf16(hbbuffer, (const mu_uint16*)hbtext, hbtextsize - 1, 0, hbtextsize - 1);
#else
	hb_buffer_add_utf8(hbbuffer, hbtext, hbtextsize - 1, 0, hbtextsize - 1);
#endif

	hb_shape(hbfont, hbbuffer, _Font->_Features.empty() ? nullptr : _Font->_Features.data(), _Font->_Features.size());

	mu_uint32 glyphCount;
	hb_glyph_info_t *glyphInfo = hb_buffer_get_glyph_infos(hbbuffer, &glyphCount);
	hb_glyph_position_t *glyphPos = hb_buffer_get_glyph_positions(hbbuffer, &glyphCount);

	mu_float defx = 0.0f;
	mu_float defy = -_Font->_Ascender;

	mu_float x = 0.0f, y = defy, nextLine = defy, maxX = 0.0f, maxY = 0.0f;

	mu_float invTextureSize = _Font->_InvTextureSize;

	if (CaretX != nullptr ||
		CaretY != nullptr)
	{
		for (mu_uint32 index = 0, hbindex = 0; index < textsize - 1; index++)
		{
			if (CaretIndex == index)
			{
				if (CaretX != nullptr)
				{
					*CaretX = x;
				}

				if (CaretY != nullptr)
				{
					*CaretY = y + _Font->_Ascender;
				}
			}

			if (text[index] == _T('\n'))
			{
				nextLine -= _Font->_MaxAdvanceY;
				x = defx, y = nextLine;

				if (maxX < x)
					maxX = x;
				if (maxY > y)
					maxY = y;
				continue;
			}
			else if (text[index] == _T('\t'))
			{
				/* (Characters Count * Font Height) / 2 */
				x += _Font->_MaxAdvanceX * 0.5f;

				if (maxX < x)
					maxX = x;
				if (maxY > y)
					maxY = y;
				continue;
			}

			mu_uint32 glyphIndex = glyphInfo[hbindex].codepoint;
			MUFontGlyph *glyph = _Font->GetCharacter(glyphIndex);
			mu_assert(glyph);

			mu_float U = glyph->StartU * invTextureSize;
			mu_float V = glyph->StartV * invTextureSize;
			mu_float SizeU = glyph->SizeU * invTextureSize;
			mu_float SizeV = glyph->SizeV * invTextureSize;

			mu_float xadvance = (mu_float)glyphPos[hbindex].x_advance / 64.0f;
			mu_float yadvance = (mu_float)glyphPos[hbindex].y_advance / 64.0f;
			mu_float xoffset = (mu_float)glyphPos[hbindex].x_offset / 64.0f;
			mu_float yoffset = (mu_float)glyphPos[hbindex].y_offset / 64.0f;
			mu_float x0 = x + xoffset + glyph->BearingX;
			mu_float y0 = EMath::Floor(y + yoffset + glyph->BearingY);
			mu_float x1 = x0 + glyph->SizeU;
			mu_float y1 = EMath::Floor(y0 - glyph->SizeV);

			x += xadvance;
			y += yadvance;

			if (maxX < x)
				maxX = x;
			if (maxY > y)
				maxY = y;

			hbindex++;
		}

		if (CaretIndex == glyphCount)
		{
			if (CaretX != nullptr)
			{
				*CaretX = x;
			}

			if (CaretY != nullptr)
			{
				*CaretY = y + _Font->_Ascender;
			}
		}
	}
	else
	{
		for (mu_uint32 index = 0, hbindex = 0; index < textsize - 1; index++)
		{
			if (text[index] == _T('\n'))
			{
				nextLine -= _Font->_MaxAdvanceY;
				x = defx, y = nextLine;

				if (maxX < x)
					maxX = x;
				if (maxY > y)
					maxY = y;
				continue;
			}
			else if (text[index] == _T('\t'))
			{
				/* (Characters Count * Font Height) / 2 */
				x += _Font->_MaxAdvanceX * 0.5f;

				if (maxX < x)
					maxX = x;
				if (maxY > y)
					maxY = y;
				continue;
			}

			mu_uint32 glyphIndex = glyphInfo[hbindex].codepoint;
			MUFontGlyph *glyph = _Font->GetCharacter(glyphIndex);
			mu_assert(glyph);

			mu_float U = glyph->StartU * invTextureSize;
			mu_float V = glyph->StartV * invTextureSize;
			mu_float SizeU = glyph->SizeU * invTextureSize;
			mu_float SizeV = glyph->SizeV * invTextureSize;

			mu_float xadvance = (mu_float)glyphPos[hbindex].x_advance / 64.0f;
			mu_float yadvance = (mu_float)glyphPos[hbindex].y_advance / 64.0f;
			mu_float xoffset = (mu_float)glyphPos[hbindex].x_offset / 64.0f;
			mu_float yoffset = (mu_float)glyphPos[hbindex].y_offset / 64.0f;
			mu_float x0 = x + xoffset + glyph->BearingX;
			mu_float y0 = EMath::Floor(y + yoffset + glyph->BearingY);
			mu_float x1 = x0 + glyph->SizeU;
			mu_float y1 = EMath::Floor(y0 - glyph->SizeV);

			x += xadvance;
			y += yadvance;

			if (maxX < x)
				maxX = x;
			if (maxY > y)
				maxY = y;

			hbindex++;
		}
	}

	maxY += _Font->_Descender;

	delete[] text;
	delete[] hbtext;

	Size->x = maxX * Scale;
	Size->y = -maxY * Scale;
}

void MUTextSimpleRender::UpdateCoord(mu_int32 UniqueId, mu_float &CoordX, mu_float &CoordY)
{
	auto it = _Cache.find(UniqueId);

	if (it == _Cache.end())
		return;

	MUText *textData = it->second;

	EVector3 out = EMath::Transform(textData->_Model, EVector3(CoordX, CoordY, 0.0f));

	CoordX = out.x;
	CoordY = out.y;
}

mu_uint32 MUTextSimpleRender::GetCharacterIndexByMouse(mu_int32 UniqueId, mu_float MouseX, mu_float MouseY, const mu_text *Text)
{
	auto it = _Cache.find(UniqueId);

	if (it == _Cache.end())
		return 0;

	MUText *textData = it->second;

	EVector2 mouse(MouseX, MouseY);

	MU_RESOURCE->Convert2D(mouse.x, mouse.y);

	hb_font_t *hbfont = _Font->_HBFont;
	hb_buffer_t *hbbuffer = _Font->_HBBuffer;
	hb_buffer_reset(hbbuffer);
	hb_buffer_set_direction(hbbuffer, HB_DIRECTION_LTR);
	hb_buffer_set_script(hbbuffer, HB_SCRIPT_LATIN);
	hb_buffer_set_language(hbbuffer, hb_language_from_string("en", 2));

	const mu_text *srctext = Text;
	mu_size srctextsize = mu_strlen(Text);
	mu_text *text = new mu_text[srctextsize + 1];
	mu_text *hbtext = new mu_text[srctextsize + 1];
	mu_size textsize = 0;
	mu_size hbtextsize = 0;
	mu_uint32 removeLength = 0;

	for (mu_uint32 index = 0; index < srctextsize; index++)
	{
		switch (srctext[index])
		{
		case _T('\uE000'):
			{
				if (srctextsize - index < ColorMinLength)
					break;

				if (srctext[index + ColorMinLength - 1] != _T('\uE000'))
					break;

				mu_boolean isValid = true;

				mu_text color[COLOR_LENGTH];

				for (mu_uint32 index2 = index + 1, colorIndex = 0; index2 < index + ColorMinLength - 1; index2++, colorIndex++)
				{
					if (isxdigit(srctext[index2]) == 0)
					{
						isValid = false;
						break;
					}

					color[colorIndex] = srctext[index2];
				}

				color[COLOR_ZERO] = _T('\0');

				if (isValid == false)
					break;

				removeLength += ColorMinLength;

				index += ColorMinLength - 1;
				continue;
			}
			break;
		case _T('\uE001'):
			{
				if (srctextsize - index < ColorMinLength)
					break;

				if (srctext[index + ColorMinLength - 1] != _T('\uE001'))
					break;

				mu_boolean isValid = true;

				mu_text color[COLOR_LENGTH];

				for (mu_uint32 index2 = index + 1, colorIndex = 0; index2 < index + ColorMinLength - 1; index2++, colorIndex++)
				{
					if (isxdigit(srctext[index2]) == 0)
					{
						isValid = false;
						break;
					}

					color[colorIndex] = srctext[index2];
				}

				color[COLOR_ZERO] = _T('\0');

				if (isValid == false)
					break;

				removeLength += ColorMinLength;

				index += ColorMinLength - 1;
				continue;
			}
			break;
		}

		text[textsize++] = srctext[index];

		switch (srctext[index])
		{
		case _T('\n'):
		case _T('\t'):
		case _T('\r'):
			break;

		default:
			hbtext[hbtextsize++] = srctext[index];
			break;
		}
	}

	text[textsize++] = _T('\0');
	hbtext[hbtextsize++] = _T('\0');

#ifdef UNICODE
	hb_buffer_add_utf16(hbbuffer, (const mu_uint16*)hbtext, hbtextsize - 1, 0, hbtextsize - 1);
#else
	hb_buffer_add_utf8(hbbuffer, hbtext, hbtextsize - 1, 0, hbtextsize - 1);
#endif

	hb_shape(hbfont, hbbuffer, _Font->_Features.empty() ? nullptr : _Font->_Features.data(), _Font->_Features.size());

	mu_uint32 glyphCount;
	hb_glyph_info_t *glyphInfo = hb_buffer_get_glyph_infos(hbbuffer, &glyphCount);
	hb_glyph_position_t *glyphPos = hb_buffer_get_glyph_positions(hbbuffer, &glyphCount);

	mu_float defx = 0.0f;
	mu_float defy = -_Font->_Ascender;

	mu_float x = 0.0f, y = defy, nextLine = defy;

	mu_float dis = FLT_MAX;
	mu_uint32 cch = 0;

	for (mu_uint32 index = 0, hbindex = 0; index < textsize - 1; index++)
	{
		EVector2 pos = EVector2(EMath::Transform(textData->_Model, EVector3(x, y, 0.0f)));

		mu_float d = EMath::Distance(pos, mouse);

		if (d < dis)
		{
			dis = d;
			cch = index;
		}
		else if (d > dis)
		{
			break;
		}

		if (text[index] == _T('\n'))
		{
			nextLine -= _Font->_MaxAdvanceY;
			x = defx, y = nextLine;
			continue;
		}
		else if (text[index] == _T('\t'))
		{
			/* (Characters Count * Font Height) / 2 */
			x += _Font->_MaxAdvanceX * 0.5f;
			continue;
		}

		mu_uint32 glyphIndex = glyphInfo[hbindex].codepoint;
		MUFontGlyph *glyph = _Font->GetCharacter(glyphIndex);
		mu_assert(glyph);

		mu_float xadvance = (mu_float)glyphPos[hbindex].x_advance / 64.0f;
		mu_float yadvance = (mu_float)glyphPos[hbindex].y_advance / 64.0f;

		x += xadvance;
		y += yadvance;

		hbindex++;
	}

	EVector2 pos = EVector2(EMath::Transform(textData->_Model, EVector3(x, y, 0.0f)));

	mu_float d = EMath::Distance(pos, mouse);

	if (d < dis)
	{
		dis = d;
		cch = glyphCount;
	}

	delete[] text;

	return cch;
}