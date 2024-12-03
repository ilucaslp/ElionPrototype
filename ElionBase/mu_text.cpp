#include "stdafx.h"
#include "mu_text.h"
#include "mu_text_handler.h"

mu_text *g_TextBuffer = nullptr;
mu_uint32 g_TextBufferLength = 0;

#define FONTBUFFER_GROWLENGTH (256)
#define FONTBUFFER_GROW (FONTBUFFER_GROWLENGTH - 1)
#define FONTINDEX_GROWLENGTH (1024)
#define FONTINDEX_GROW (FONTINDEX_GROWLENGTH - 1)

MUText::MUText(MUFont *font) : _Font(font),
							   _InstanceCount(0),
							   _InstanceUpdate(false),
							   _InstanceIndex(EInvalidUInt32),
							   _PositionUpdate(false)
{

}

MUText::~MUText()
{
	for (auto it = _ConstantBuffer.begin();it != _ConstantBuffer.end();++it)
	{
		(*it).Deallocate();
	}
}

mu_boolean MUText::Create(mu_string Text, EVector2 Position, mu_float BorderWidth, mu_float BoxWidth, mu_float BoxHeight)
{
	EVector2 Border;
	Position.x = Position.x;
	Position.y = Position.y;
	Border.x = BorderWidth;
	Border.y = BorderWidth;

	_Is3D = false;
	_EnableHighlightCorrection = true;
	_Scale = 1.0f;
	_Position = EVector3(Position.x, Position.y, 1.0f);
	_Border = Border;
	_BoxWidth = BoxWidth;
	_BoxHeight = BoxHeight;
	_Size = EVector2(0.0f, 0.0f);

	_InstanceUpdate = true;
	_PositionUpdate = true;

	const mu_uint32 bufferCount = EVideoBackend::GetBufferingCount();
	_ConstantBuffer.resize(bufferCount);
	for(mu_uint32 n = 0;n < bufferCount;++n)
	{
		if (_ConstantBuffer[n].Allocate(sizeof(SHFontConstant), EBufferUsage::eDynamic, EBufferType::eUniform, nullptr) == false)
		{
			return false;
		}

		_ConstantBuffer[n].ConfigureUniform(EUniformLayout::eFont);
	}

	_ConstantUpdate.resize(bufferCount, true);

	return true;
}

mu_boolean MUText::Create(mu_string Text, EVector3 Position, mu_float BorderWidth, mu_float BoxWidth, mu_float BoxHeight)
{
	EVector2 Border;
	Border.x = BorderWidth;
	Border.y = BorderWidth;

	_Is3D = true;
	_EnableHighlightCorrection = true;
	_Scale = 1.0f;
	_Position = Position;
	_Border = Border;
	_BoxWidth = BoxWidth;
	_BoxHeight = BoxHeight;
	_Size = EVector2(0.0f, 0.0f);

	_InstanceUpdate = true;
	_PositionUpdate = true;

	const mu_uint32 bufferCount = EVideoBackend::GetBufferingCount();
	_ConstantBuffer.resize(bufferCount);
	for (mu_uint32 n = 0; n < bufferCount; ++n)
	{
		if (_ConstantBuffer[n].Allocate(sizeof(SHFontConstant), EBufferUsage::eDynamic, EBufferType::eUniform, nullptr) == false)
		{
			return false;
		}

		_ConstantBuffer[n].ConfigureUniform(EUniformLayout::eFont);
	}

	_ConstantUpdate.resize(bufferCount, true);

	return true;
}

void MUText::Destroy()
{
	const mu_uint32 bufferCount = EVideoBackend::GetBufferingCount();
	for (mu_uint32 n = 0; n < bufferCount; ++n)
	{
		_ConstantBuffer[n].Deallocate();
	}
}

void MUText::Update()
{
	const mu_text *srctext = _Text.c_str();
	const mu_size srctextsize = _Text.size();

	if (_InstanceUpdate == true)
	{
		std::vector<ETextHandler::ETextPartition> partitions;
		mu_uint32 linesCount;

		ETextHandler::PreparePartitions(srctext, srctextsize, partitions);
		ETextHandler::CalculateInstancesCount(partitions, linesCount, _TextInstanceCount, _BackInstanceCount);

		const mu_uint32 totalInstanceCount = _TextInstanceCount + _BackInstanceCount;
		if (_InstanceCount < totalInstanceCount)
		{
			if (_InstanceIndex != EInvalidUInt32)
			{
				EVideoBackend::FontDeallocateDataIndex(_InstanceIndex);
			}
			const mu_uint32 roundInstanceCount = totalInstanceCount + (FONT_TEXT_GROW - (totalInstanceCount % FONT_TEXT_GROW)) % FONT_TEXT_GROW;
			_InstanceIndex = EVideoBackend::FontAllocateDataIndex(roundInstanceCount);
			_InstanceCount = roundInstanceCount;
		}

		if (_InstanceIndex != EInvalidUInt32)
		{
			std::vector<mu_float> linesWidth(linesCount + 1, 0.0f);
			std::vector<ETextHandler::ETextBack> backData(_BackInstanceCount);
			mu_uint32 backindex = 0;
			ETextHandler::ETextBack *back = nullptr;

			SHFontInstance *baseInstances = EVideoBackend::FontRetrieveDataBuffer(_InstanceIndex);
			SHFontInstance *backInstance = baseInstances;
			SHFontInstance *textInstance = baseInstances + _BackInstanceCount;

			ETextHandler::ETextColor color(ETextHandler::DefaultBackColor, ETextHandler::DefaultTextColor);

			hb_font_t *hbfont = _Font->_HBFont;
			hb_buffer_t *hbbuffer = _Font->_HBBuffer;
			const mu_float invTextureSize = _Font->_InvTextureSize;

			const mu_float ascender = _Font->_Ascender;
			const mu_float borderwidth = _Border.x * 0.5f, borderheight = _Border.y * 0.5f;
			mu_float maxwidth = 0.0f, maxheight = 0.0f;
			mu_float x = 0.0f, y = 0.0f;
			mu_float tx = 0.0f;
			mu_uint32 line = 0;
			mu_boolean newline = false;

			mu_uint32 glyphCount = 0;
			mu_uint32 glyphIndex = 0;
			hb_glyph_info_t *glyphInfo;
			hb_glyph_position_t *glyphPos;
			mu_boolean parsedText = false;

			for (auto iter = partitions.begin(); iter != partitions.end(); ++iter)
			{
				ETextHandler::ETextPartition &partition = *iter;

				if (partition.Type == ETextHandler::ETextType::eText &&
					parsedText == false)
				{
					ETextHandler::ParseTextLine(_Font, hbfont, hbbuffer, iter, partitions.end(), srctext, srctextsize, glyphCount, glyphInfo, glyphPos);
					glyphIndex = 0;
					parsedText = true;
				}

				switch(partition.Type)
				{
				case ETextHandler::ETextType::eColor:
					{
						color = partition.Color;

						if (MU::GetAlpha(color.Back) > 0)
						{
							/*
								Prevent to create multiples background when new line
							
								Example:
								eColor (Add Backcount)
								eNewLine (Add Backcount [next line])
								eColor  (Add Backcount [only if not new line])
								eText
							*/
							if (newline == false)
							{
								back = &backData[backindex++];
							}

							if (line == 0)
							{
								back->FirstRow = true;
							}
							else
							{
								back->FirstRow = false;
							}
							
							if (line == linesCount - 1)
							{
								back->LastRow = true;
							}
							else
							{
								back->LastRow = false;
							}

							if (x == 0.0f)
							{
								back->FirstColumn = true;
							}
							else
							{
								back->FirstColumn = false;
							}

							back->LastColumn = false;

							back->Color = color.Back;
							back->Line = line;
							back->X = tx - 0.001f;
							if (back->X < 0.0f) back->X = 0.0f;
							back->Width = 0.0f;
						}
					}
					break;

				case ETextHandler::ETextType::eNewLine:
					{
						x = 0.0f;

						if (MU::GetAlpha(color.Back) > 0)
						{
							for (mu_uint32 j = 0;j < partition.Count; ++j)
							{
								back->LastColumn = true;

								back = &backData[backindex++];
								if (line == linesCount - 1)
								{
									back->LastRow = true;
								}
								else
								{
									back->LastRow = false;
								}

								back->FirstColumn = true;
								back->LastColumn = false;

								back->Color = color.Back;
								back->Line = line + j + 1;
								back->X = tx - 0.001f;
								if (back->X < 0.0f) back->X = 0.0f;
								back->Width = 0.0f;
							}
						}

						y += _Font->_MaxAdvanceY * partition.Count;
						line += partition.Count;
						parsedText = false;
					}
					break;

				case ETextHandler::ETextType::eTab:
					{
						const mu_float xadvance = _Font->_MaxAdvanceX * partition.Count;
						x += xadvance;
						parsedText = false;

						if (back != nullptr)
						{
							back->Width += xadvance;
						}
					}
					break;

				case ETextHandler::ETextType::eText:
					{
						mu_float tmpx[2] = { x, x };
						const mu_uint32 maxJ = partition.Index + partition.Count;
						for (mu_uint32 j = partition.Index; j < maxJ; ++j, ++glyphIndex)
						{
							hb_glyph_info_t &ginfo = glyphInfo[glyphIndex];
							hb_glyph_position_t &gpos = glyphPos[glyphIndex];

							const mu_float xadvance = (mu_float)gpos.x_advance / 64.0f;
							const mu_float yadvance = (mu_float)gpos.y_advance / 64.0f;
							const mu_float xoffset = (mu_float)gpos.x_offset / 64.0f;
							const mu_float yoffset = (mu_float)gpos.y_offset / 64.0f;

							if (srctext[j] != _T(' '))
							{
								const mu_uint32 glyphIndex = ginfo.codepoint;
								const MUFontGlyph *glyph = _Font->GetCharacter(glyphIndex);
								mu_assert(glyph && "Retrieve glyph character failed!");

								const mu_float U = glyph->StartU * invTextureSize;
								const mu_float V = glyph->StartV * invTextureSize;
								const mu_float SizeU = glyph->SizeU * invTextureSize;
								const mu_float SizeV = glyph->SizeV * invTextureSize;

								const mu_float x0 = (tmpx[0] + xoffset + glyph->BearingX + borderwidth);
								const mu_float y0 = (y + yoffset - glyph->BearingY + ascender + borderheight);
								const mu_float x1 = (x0 + glyph->SizeU);
								const mu_float y1 = (y0 + glyph->SizeV);

								SetFontInstance(textInstance, EVector4(x0, x1, y0, y1), EVector4(U, U + SizeU, V, V + SizeV), color.Text, glyph->TextureIndex);
								++textInstance;

								tmpx[0] += xadvance;
								tmpx[1] = x1;
							}
							else
							{
								tmpx[0] += xadvance;
								tmpx[1] += xadvance;
							}

							y += yadvance;
						}

						if (MU::GetAlpha(color.Back) > 0)
						{
							back->Width = tmpx[0] - x - borderwidth;
						}

						x = tmpx[0] - borderwidth;
						tx = tmpx[1] - borderwidth;
					}
					break;
				}

				if (partition.Type != ETextHandler::ETextType::eNewLine)
				{
					newline = false;
				}

				if (tx > linesWidth[line])
				{
					linesWidth[line] = tx;
				}

				if (tx > maxwidth)
				{
					maxwidth = tx;
				}

				if (y + _Font->_MaxAdvanceY > maxheight)
				{
					maxheight = y + _Font->_MaxAdvanceY;
				}
			}

			if (maxwidth > 0.0f)
			{
				maxwidth += _Border.x;
			}

			if (maxheight > 0.0f)
			{
				maxheight += _Border.y;
			}

			if (maxwidth < _BoxWidth)
			{
				maxwidth = _BoxWidth;
			}

			if (maxheight < _BoxHeight)
			{
				maxheight = _BoxHeight;
			}

			if (MU::GetAlpha(color.Back) > 0)
			{
				back->LastRow = true;
				back->LastColumn = true; 
			}

			for (mu_uint32 n = 0;n < _BackInstanceCount; ++n)
			{
				ETextHandler::ETextBack &data = backData[n];
				SHFontInstance *instance = &baseInstances[n];

				const mu_float x0 = EMath::Floor(data.FirstColumn == true ? 0.0f : data.X + borderwidth);
				const mu_float x1 = EMath::Ceil(data.LastColumn == true ? maxwidth : data.X + data.Width - 0.001f);
				const mu_float y0 = data.FirstRow == true ? 0.0f : _Font->_MaxAdvanceY * data.Line;
				const mu_float y1 = data.LastRow == true ? maxheight : y0 + _Font->_MaxAdvanceY;

				SetFontInstance(instance, EVector4(x0, x1, y0, y1), EVector4(0.0f, 0.0f, 0.0f, 0.0f), data.Color, FONT_INVALIDTEXTURE);
			}

			_BackupSize = EVector2(maxwidth, maxheight);

			mu_uint8 Horizontal = (_HorizontalAlign & ((~0) ^ ETF_DISABLE_AUTOPOS));
			switch (Horizontal)
			{
			case ETA_LEFT:
				{
					// Nothing To Do
				}
				break;

			case ETA_CENTER:
				{
					line = 0;
					textInstance = baseInstances + _BackInstanceCount;
					for (auto iter = partitions.begin(); iter != partitions.end(); ++iter)
					{
						ETextHandler::ETextPartition &partition = *iter;

						switch(partition.Type)
						{
						case ETextHandler::ETextType::eNewLine:
							{
								line += partition.Count;
							}
							break;

						case ETextHandler::ETextType::eText:
							{
								const mu_float xoffset = (maxwidth - linesWidth[line]) * 0.5f;
								const mu_uint32 maxJ = partition.Index + partition.Count;
								for (mu_uint32 j = partition.Index, m = 0; j < maxJ; ++j, ++m)
								{
									if (srctext[j] != _T(' '))
									{
										textInstance->Position.x += xoffset;
										textInstance->Position.y += xoffset;
										++textInstance;
									}
								}
							}
							break;
						}
					}
				}
				break;

			case ETA_RIGHT:
				{
					line = 0;
					textInstance = baseInstances + _BackInstanceCount;
					for (auto iter = partitions.begin(); iter != partitions.end(); ++iter)
					{
						ETextHandler::ETextPartition &partition = *iter;

						switch (partition.Type)
						{
						case ETextHandler::ETextType::eNewLine:
							{
								line += partition.Count;
							}
							break;

						case ETextHandler::ETextType::eText:
							{
								const mu_float xoffset = maxwidth - linesWidth[line];
								const mu_uint32 maxJ = partition.Index + partition.Count;
								for (mu_uint32 j = partition.Index, m = 0; j < maxJ; ++j, ++m)
								{
									if (srctext[j] != _T(' '))
									{
										textInstance->Position.x += xoffset;
										textInstance->Position.y += xoffset;
										++textInstance;
									}
								}
							}
							break;
						}
					}
				}
				break;
			}

			EVideoBackend::FontRequestUpdateData(_InstanceIndex, _BackInstanceCount + _TextInstanceCount);
		}
	}

	const mu_uint32 currentIndex = EVideoBackend::GetCurrentBufferingIndex();
	if (_ConstantUpdate[currentIndex] == true)
	{
		_ConstantUpdate[currentIndex] = false;
		SHFontConstant *constantBuffer = nullptr;

		switch(EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			constantBuffer = reinterpret_cast<SHFontConstant*>(_ConstantBuffer[currentIndex].DirectXInternal.MappedMemory);
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			constantBuffer = reinterpret_cast<SHFontConstant*>(_ConstantBuffer[currentIndex].OpenGLInternal.MappedMemory);
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			constantBuffer = reinterpret_cast<SHFontConstant*>(_ConstantBuffer[currentIndex].VulkanInternal.MappedMemory);
			break;
#endif
		}

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

			if (_PositionUpdate == true ||
				mu_memcmp(&_View, &MUVideoManager::_CurrentView, sizeof(EMatrix4)) != 0 ||
				mu_memcmp(&_Projection, &MUVideoManager::_CurrentProjection, sizeof(EMatrix4)) != 0)
			{
				updateData = true;

				billboardSize = EMath::RetrieveBillboardSize(MUVideoManager::_CurrentTextView,
					position,
					MU_CAMERA->GetCameraPosition(),
					MU_CAMERA->GetCameraInverseDirection(),
					GameRFOV,
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

			mu_assert(MU_CAMERA != nullptr);
			if (updateData)
			{
				_Model = EMath::RetrieveBillboardAlternative(MUVideoManager::_CurrentTextView,
															 position,
															 billboardSize) * EMath::Scale(_Scale);
				_View = MUVideoManager::_CurrentView;
				_Projection = MUVideoManager::_CurrentProjection;
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
				if (position.x + _Size.x >= MU_RESOURCE->GetWindowWidth())
				{
					position.x = MU_RESOURCE->GetWindowWidth() - _Size.x;
				}
				else if (position.x < 0.0f)
				{
					position.x = 0.0f;
				}
			}

			if (isVerticalAutopos)
			{
				if (position.y + _Size.y >= MU_RESOURCE->GetWindowHeight())
				{
					position.y = MU_RESOURCE->GetWindowHeight() - _Size.y;
				}
				else if (position.y < 0.0f)
				{
					position.y = 0.0f;
				}
			}

			if (_PositionUpdate == true ||
				mu_memcmp(&_View, &MUVideoManager::_CurrentView2D, sizeof(EMatrix4)) != 0 ||
				mu_memcmp(&_Projection, &MUVideoManager::_CurrentProjection2D, sizeof(EMatrix4)) != 0)
			{
				EMath::MakeMatrix(_Model, position, _Scale);
				_View = MUVideoManager::_CurrentView2D;
				_Projection = MUVideoManager::_CurrentProjection2D;
			}
		}

		constantBuffer->PositionMatrix = _Projection *
			_View *
			_Model;
		constantBuffer->OutlineColor = _OutlineColor;

		_ConstantBuffer[currentIndex].Flush(0, 0, sizeof(SHFontConstant));
	}

	_InstanceUpdate = false;
	_PositionUpdate = false;
}

void MUText::Render(const ECombinedShader programShader, const mu_boolean isFocus)
{
	if (_TextInstanceCount == 0 ||
		_InstanceIndex == EInvalidUInt32) return;

	if (this->Is3D() == true &&
		this->_BoxWidth != 0.0f &&
		this->_BoxHeight != 0.0f)
	{
		EVideoPipeline::SetGraphicsPipelineAtRuntime(programShader, ERasterizer::eBackOnly, EDepthStencil::eNoZBuffer, EBlendState::eAlphaSrc_InvAlphaSrc, true);
		EVideoBackend::SetScissor(this->_Position.x, this->_Position.y, this->_BoxWidth, this->_BoxHeight);
	}
	else
	{
		EVideoPipeline::SetGraphicsPipelineAtRuntime(programShader, ERasterizer::eBackOnly, EDepthStencil::eNoZBuffer, EBlendState::eAlphaSrc_InvAlphaSrc, false);
	}

	EVideoPipeline::BindGraphicsFontBuffers(&_ConstantBuffer[EVideoBackend::GetCurrentBufferingIndex()]);
	EVideoPipeline::BindGraphicsFontTexture(&_Font->_Texture);
	EVideoPipeline::DrawInstancedAtRuntime(0, 6, EVideoBackend::FontRetrieveBufferBaseIndex(_InstanceIndex), (_BackInstanceCount + _TextInstanceCount), ETopologyType::eTriangleList, EInputLayout::eFont);
	EVideoPipeline::UnbindVAO();
}

MUTextSimpleRender::MUTextSimpleRender(MUFont *Font, const ECombinedShader ShaderType) : _Font(Font), _ShaderType(ShaderType)
{

}

MUTextSimpleRender::~MUTextSimpleRender()
{
	MU_SAFE_FREE(g_TextBuffer);

	ClearCache();
}

void MUTextSimpleRender::ClearCache()
{
	for (auto it = _Cache.begin(); it != _Cache.end(); it = _Cache.erase(it))
	{
		delete it->second;
	}
}

void MUTextSimpleRender::SystemConfigure()
{
	for (auto it = _Cache.begin(); it != _Cache.end(); it = _Cache.erase(it))
	{
		it->second->_InstanceUpdate = true;
		it->second->_PositionUpdate = true;

		std::fill(it->second->_ConstantUpdate.begin(), it->second->_ConstantUpdate.end(), true);
	}
}

mu_boolean MUTextSimpleRender::DrawString(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Z, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, const mu_text *Text, mu_float BoxWidth, mu_float BoxHeight, MUText **Buffer, mu_boolean is3D, mu_boolean HighlightCorrection, const EVector4 &OutlineColor)
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

	mu_boolean updateConstant = false;
	if (textData->_Position.x != X ||
		textData->_Position.y != Y ||
		textData->_Position.z != Z ||
		textData->_Scale != Scale ||
		textData->_HorizontalAlign != HorizontalAlign ||
		textData->_VerticalAlign != VerticalAlign)
	{
		textData->_Position = EVector3(X, Y, Z);
		textData->_Scale = Scale;
		updateConstant = true;
		textData->_PositionUpdate = true;

		if (BoxWidth != 0 ||
			BoxHeight != 0 ||
			textData->_HorizontalAlign != HorizontalAlign ||
			textData->_VerticalAlign != VerticalAlign)
		{
			textData->_InstanceUpdate = true;
		}

		textData->_HorizontalAlign = HorizontalAlign;
		textData->_VerticalAlign = VerticalAlign;
	}

	if (textData->_Text.compare(Text) != 0)
	{
		textData->_Text = Text;
		textData->_InstanceUpdate = true;
		updateConstant = true;
		textData->_PositionUpdate = true;
	}

	if (BoxWidth != textData->_BoxWidth ||
		BoxHeight != textData->_BoxHeight ||
		BorderWidth != textData->_Border.x)
	{
		textData->_BoxWidth = BoxWidth;
		textData->_BoxHeight = BoxHeight;
		textData->_Border = EVector2(BorderWidth, BorderWidth);
		textData->_InstanceUpdate = true;
		updateConstant = true;
		textData->_PositionUpdate = true;
	}

	if (textData->_Is3D != is3D)
	{
		textData->_Is3D = is3D;
		updateConstant = true;
	}

	if (textData->_EnableHighlightCorrection != HighlightCorrection)
	{
		textData->_EnableHighlightCorrection = HighlightCorrection;
		textData->_InstanceUpdate = true;
	}

	if (mu_memcmp(&textData->_OutlineColor, &OutlineColor, sizeof(OutlineColor)) != 0)
	{
		textData->_OutlineColor = OutlineColor;
		updateConstant = true;
	}

	if(is3D)
	{
		if (mu_memcmp(&textData->_View, &MUVideoManager::_CurrentView, sizeof(EMatrix4)) != 0 ||
			mu_memcmp(&textData->_Projection, &MUVideoManager::_CurrentProjection, sizeof(EMatrix4)) != 0)
		{
			updateConstant = true;
		}
	}
	else
	{
		if (mu_memcmp(&textData->_View, &MUVideoManager::_CurrentView2D, sizeof(EMatrix4)) != 0 ||
			mu_memcmp(&textData->_Projection, &MUVideoManager::_CurrentProjection2D, sizeof(EMatrix4)) != 0)
		{
			updateConstant = true;
		}
	}

	if(updateConstant == true)
	{
		std::fill(textData->_ConstantUpdate.begin(), textData->_ConstantUpdate.end(), true);
	}

	textData->Update();

	if(textData->_Text.empty() == false)
	{
		textData->Render(_ShaderType, false);
	}

	return true;
}

EVector4 DefaultOutlineColor(0.0f, 0.0f, 0.0f, 0.0f);

mu_boolean MUTextSimpleRender::DrawStringC2D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, mu_uint32 TextColor, mu_uint32 BackColor, const mu_text *Text, mu_float BoxWidth, mu_float BoxHeight, MUText **Buffer, mu_boolean HighlightCorrection)
{
	const mu_size TextLength = mu_strlen(Text);
	if (g_TextBufferLength < TextLength + ETextHandler::DefaultColorFormatSize + 1)
	{
		g_TextBufferLength = TextLength + ETextHandler::DefaultColorFormatSize + 1;
		g_TextBuffer = (mu_text*)mu_realloc(g_TextBuffer, g_TextBufferLength * sizeof(mu_text));
	}

	ETextHandler::CopyStringWithColor(g_TextBuffer, g_TextBufferLength, Text, TextLength, TextColor, BackColor);

	// Fix text alignment to nearest pixel
	X = EMath::Round(X);
	Y = EMath::Round(Y);

	BorderWidth = MU_TO_WINDOW(BorderWidth, MU_RESOURCE->_640x480Rate[0]);
	return DrawString(UniqueId, X, Y, 0.0f, Scale, HorizontalAlign, VerticalAlign, BorderWidth, g_TextBuffer, BoxWidth, BoxHeight, Buffer, false, HighlightCorrection, DefaultOutlineColor);
}

mu_boolean MUTextSimpleRender::DrawStringOutlineC2D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, mu_uint32 TextColor, mu_uint32 BackColor, mu_uint32 OutlineColor, const mu_text *Text, mu_float BoxWidth, mu_float BoxHeight, MUText **Buffer, mu_boolean HighlightCorrection)
{
	const mu_size TextLength = mu_strlen(Text);
	if (g_TextBufferLength < TextLength + ETextHandler::DefaultColorFormatSize + 1)
	{
		g_TextBufferLength = TextLength + ETextHandler::DefaultColorFormatSize + 1;
		g_TextBuffer = (mu_text*)mu_realloc(g_TextBuffer, g_TextBufferLength * sizeof(mu_text));
	}

	ETextHandler::CopyStringWithColor(g_TextBuffer, g_TextBufferLength, Text, TextLength, TextColor, BackColor);

	// Fix text alignment to nearest pixel
	X = EMath::Round(X);
	Y = EMath::Round(Y);

	BorderWidth = MU_TO_WINDOW(BorderWidth, MU_RESOURCE->_640x480Rate[0]);
	return DrawString(UniqueId, X, Y, 0.0f, Scale, HorizontalAlign, VerticalAlign, BorderWidth, g_TextBuffer, BoxWidth, BoxHeight, Buffer, false, HighlightCorrection, MU::ARGBToFloat4(OutlineColor));
}

mu_boolean MUTextSimpleRender::DrawStringC3D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Z, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, mu_uint32 TextColor, mu_uint32 BackColor, const mu_text *Text, mu_float BoxWidth, mu_float BoxHeight, MUText **Buffer, mu_boolean HighlightCorrection)
{
	const mu_size TextLength = mu_strlen(Text);
	if (g_TextBufferLength < TextLength + ETextHandler::DefaultColorFormatSize + 1)
	{
		g_TextBufferLength = TextLength + ETextHandler::DefaultColorFormatSize + 1;
		g_TextBuffer = (mu_text*)mu_realloc(g_TextBuffer, g_TextBufferLength * sizeof(mu_text));
	}

	ETextHandler::CopyStringWithColor(g_TextBuffer, g_TextBufferLength, Text, TextLength, TextColor, BackColor);

	// Fix text alignment to nearest pixel
	X = EMath::Round(X);
	Y = EMath::Round(Y);
	Z = EMath::Round(Z);

	BorderWidth = MU_TO_WINDOW(BorderWidth, MU_RESOURCE->_640x480Rate[0]);
	return DrawString(UniqueId, X, Y, Z, Scale, HorizontalAlign, VerticalAlign, BorderWidth, g_TextBuffer, BoxWidth, BoxHeight, Buffer, true, HighlightCorrection, DefaultOutlineColor);
}

mu_boolean MUTextSimpleRender::DrawStringOutlineC3D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Z, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, mu_uint32 TextColor, mu_uint32 BackColor, mu_uint32 OutlineColor, const mu_text *Text, mu_float BoxWidth, mu_float BoxHeight, MUText **Buffer, mu_boolean HighlightCorrection)
{
	const mu_size TextLength = mu_strlen(Text);
	if (g_TextBufferLength < TextLength + ETextHandler::DefaultColorFormatSize + 1)
	{
		g_TextBufferLength = TextLength + ETextHandler::DefaultColorFormatSize + 1;
		g_TextBuffer = (mu_text*)mu_realloc(g_TextBuffer, g_TextBufferLength * sizeof(mu_text));
	}

	ETextHandler::CopyStringWithColor(g_TextBuffer, g_TextBufferLength, Text, TextLength, TextColor, BackColor);

	// Fix text alignment to nearest pixel
	X = EMath::Round(X);
	Y = EMath::Round(Y);
	Z = EMath::Round(Z);

	BorderWidth = MU_TO_WINDOW(BorderWidth, MU_RESOURCE->_640x480Rate[0]);
	return DrawString(UniqueId, X, Y, Z, Scale, HorizontalAlign, VerticalAlign, BorderWidth, g_TextBuffer, BoxWidth, BoxHeight, Buffer, true, HighlightCorrection, MU::ARGBToFloat4(OutlineColor));
}

mu_boolean MUTextSimpleRender::DrawString2D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, const mu_text *Text, mu_float BoxWidth, mu_float BoxHeight, MUText **Buffer, mu_boolean HighlightCorrection)
{
	// Fix text alignment to nearest pixel
	X = EMath::Round(X);
	Y = EMath::Round(Y);

	BorderWidth = MU_TO_WINDOW(BorderWidth, MU_RESOURCE->_640x480Rate[0]);
	return DrawString(UniqueId, X, Y, 0.0f, Scale, HorizontalAlign, VerticalAlign, BorderWidth, Text, BoxWidth, BoxHeight, Buffer, false, HighlightCorrection, DefaultOutlineColor);
}

mu_boolean MUTextSimpleRender::DrawStringOutline2D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, mu_uint32 OutlineColor, const mu_text *Text, mu_float BoxWidth, mu_float BoxHeight, MUText **Buffer, mu_boolean HighlightCorrection)
{
	// Fix text alignment to nearest pixel
	X = EMath::Round(X);
	Y = EMath::Round(Y);

	BorderWidth = MU_TO_WINDOW(BorderWidth, MU_RESOURCE->_640x480Rate[0]);
	return DrawString(UniqueId, X, Y, 0.0f, Scale, HorizontalAlign, VerticalAlign, BorderWidth, Text, BoxWidth, BoxHeight, Buffer, false, HighlightCorrection, MU::ARGBToFloat4(OutlineColor));
}

mu_boolean MUTextSimpleRender::DrawString3D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Z, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, const mu_text *Text, mu_float BoxWidth, mu_float BoxHeight, MUText **Buffer, mu_boolean HighlightCorrection)
{
	// Fix text alignment to nearest pixel
	X = EMath::Round(X);
	Y = EMath::Round(Y);
	Z = EMath::Round(Z);

	BorderWidth = MU_TO_WINDOW(BorderWidth, MU_RESOURCE->_640x480Rate[0]);
	return DrawString(UniqueId, X, Y, Z, Scale, HorizontalAlign, VerticalAlign, BorderWidth, Text, BoxWidth, BoxHeight, Buffer, true, HighlightCorrection, DefaultOutlineColor);
}

mu_boolean MUTextSimpleRender::DrawStringOutline3D(mu_int32 UniqueId, mu_float X, mu_float Y, mu_float Z, mu_float Scale, mu_uint8 HorizontalAlign, mu_uint8 VerticalAlign, mu_float BorderWidth, mu_uint32 OutlineColor, const mu_text *Text, mu_float BoxWidth, mu_float BoxHeight, MUText **Buffer, mu_boolean HighlightCorrection)
{
	// Fix text alignment to nearest pixel
	X = EMath::Round(X);
	Y = EMath::Round(Y);
	Z = EMath::Round(Z);

	BorderWidth = MU_TO_WINDOW(BorderWidth, MU_RESOURCE->_640x480Rate[0]);
	return DrawString(UniqueId, X, Y, Z, Scale, HorizontalAlign, VerticalAlign, BorderWidth, Text, BoxWidth, BoxHeight, Buffer, true, HighlightCorrection, MU::ARGBToFloat4(OutlineColor));
}

void MUTextSimpleRender::GetTextExtentPoint(const mu_text *Text, mu_size Length, mu_uint8 HorizontalAlign, mu_float Scale, EVector2 *Size, mu_uint32 CaretIndex, mu_float *CaretX, mu_float *CaretY)
{
	const mu_text *srctext = Text;
	mu_size srctextsize = Length;

	std::vector<ETextHandler::ETextPartition> partitions;
	mu_uint32 linesCount;
	mu_uint32 textInstanceCount, backInstanceCount;

	ETextHandler::PreparePartitionsNoColor(srctext, srctextsize, partitions);
	ETextHandler::CalculateInstancesCount(partitions, linesCount, textInstanceCount, backInstanceCount);

	std::vector<mu_float> linesWidth(linesCount + 1, 0.0f);

	hb_font_t *hbfont = _Font->_HBFont;
	hb_buffer_t *hbbuffer = _Font->_HBBuffer;
	const mu_float invTextureSize = _Font->_InvTextureSize;

	const mu_float ascender = _Font->_Ascender;
	mu_float maxwidth = 0.0f, maxheight = 0.0f;
	mu_float x = 0.0f, y = 0.0f;
	mu_float tx = 0.0f;
	mu_uint32 line = 0;
	mu_uint32 caretline = 0;
	mu_boolean newline = false;

	if (CaretX != nullptr)
	{
		*CaretX = 0.0f;
	}

	if (CaretY != nullptr)
	{
		*CaretY = 0.0f;
	}

	mu_uint32 glyphCount = 0;
	mu_uint32 glyphIndex = 0;
	hb_glyph_info_t *glyphInfo;
	hb_glyph_position_t *glyphPos;
	mu_boolean parsedText = false;

	for (auto iter = partitions.begin(); iter != partitions.end(); ++iter)
	{
		ETextHandler::ETextPartition &partition = *iter;

		if (partition.Type == ETextHandler::ETextType::eText &&
			parsedText == false)
		{
			glyphIndex = 0;
			ETextHandler::ParseTextLine(_Font, hbfont, hbbuffer, iter, partitions.end(), srctext, srctextsize, glyphCount, glyphInfo, glyphPos);
			parsedText = true;
		}

		switch(partition.Type)
		{
		case ETextHandler::ETextType::eColor:
			break;

		case ETextHandler::ETextType::eNewLine:
			{
				const mu_uint32 caretIndex = CaretIndex + partition.Colors;
				if ((CaretX != nullptr ||
					CaretY != nullptr) &&
					caretIndex >= partition.Index &&
					caretIndex < partition.Index + partition.Count)
				{
					mu_uint32 count = caretIndex - partition.Index;

					caretline = line + count;

					if (CaretX != nullptr)
					{
						*CaretX = caretIndex == partition.Index ? x : 0.0f;
					}

					if (CaretY != nullptr)
					{
						*CaretY = y + _Font->_MaxAdvanceY * count;
					}
				}

				x = 0.0f;
				y += _Font->_MaxAdvanceY * partition.Count;
				line += partition.Count;
				parsedText = false;
			}
			break;

		case ETextHandler::ETextType::eTab:
			{
				const mu_float xadvance = _Font->_MaxAdvanceX * partition.Count;
				x += xadvance;
				parsedText = false;
			}
			break;

		case ETextHandler::ETextType::eText:
			{
				mu_float tmpx[2] = { x, x };

				const mu_uint32 caretIndex = CaretIndex + partition.Colors;
				if ((CaretX != nullptr ||
					CaretY != nullptr) &&
					caretIndex >= partition.Index &&
					caretIndex < partition.Index + partition.Count)
				{
					const mu_uint32 maxJ = partition.Index + partition.Count;
					for (mu_uint32 j = partition.Index; j < maxJ; ++j, ++glyphIndex)
					{
						hb_glyph_info_t &ginfo = glyphInfo[glyphIndex];
						hb_glyph_position_t &gpos = glyphPos[glyphIndex];

						if (j == caretIndex)
						{
							caretline = line;

							if (CaretX != nullptr)
							{
								*CaretX = tmpx[0];
							}

							if (CaretY != nullptr)
							{
								*CaretY = y;
							}
						}

						const mu_float xadvance = (mu_float)gpos.x_advance / 64.0f;
						const mu_float yadvance = (mu_float)gpos.y_advance / 64.0f;
						const mu_float xoffset = (mu_float)gpos.x_offset / 64.0f;
						const mu_float yoffset = (mu_float)gpos.y_offset / 64.0f;

						if (srctext[j] != _T(' '))
						{
							const mu_uint32 glyphIndex = ginfo.codepoint;
							const MUFontGlyph *glyph = _Font->GetCharacter(glyphIndex);
							mu_assert(glyph && "Retrieve glyph character failed!");

							const mu_float U = glyph->StartU * invTextureSize;
							const mu_float V = glyph->StartV * invTextureSize;
							const mu_float SizeU = glyph->SizeU * invTextureSize;
							const mu_float SizeV = glyph->SizeV * invTextureSize;

							const mu_float x0 = (tmpx[0] + xoffset + glyph->BearingX);
							const mu_float y0 = (y + yoffset - glyph->BearingY + ascender);
							const mu_float x1 = (x0 + glyph->SizeU);
							const mu_float y1 = (y0 + glyph->SizeV);

							tmpx[0] += xadvance;
							tmpx[1] = x1;
						}
						else
						{
							tmpx[0] += xadvance;
							tmpx[1] += xadvance;
						}

						y += yadvance;
					}
				}
				else
				{
					const mu_uint32 maxJ = partition.Index + partition.Count;
					for (mu_uint32 j = partition.Index; j < maxJ; ++j, ++glyphIndex)
					{
						hb_glyph_info_t &ginfo = glyphInfo[glyphIndex];
						hb_glyph_position_t &gpos = glyphPos[glyphIndex];

						const mu_float xadvance = (mu_float)gpos.x_advance / 64.0f;
						const mu_float yadvance = (mu_float)gpos.y_advance / 64.0f;
						const mu_float xoffset = (mu_float)gpos.x_offset / 64.0f;
						const mu_float yoffset = (mu_float)gpos.y_offset / 64.0f;

						if (srctext[j] != _T(' '))
						{
							const mu_uint32 glyphIndex = ginfo.codepoint;
							const MUFontGlyph *glyph = _Font->GetCharacter(glyphIndex);
							mu_assert(glyph && "Retrieve glyph character failed!");

							const mu_float U = glyph->StartU * invTextureSize;
							const mu_float V = glyph->StartV * invTextureSize;
							const mu_float SizeU = glyph->SizeU * invTextureSize;
							const mu_float SizeV = glyph->SizeV * invTextureSize;

							const mu_float x0 = (tmpx[0] + xoffset + glyph->BearingX);
							const mu_float y0 = (y + yoffset - glyph->BearingY + ascender);
							const mu_float x1 = (x0 + glyph->SizeU);
							const mu_float y1 = (y0 + glyph->SizeV);

							tmpx[0] += xadvance;
							tmpx[1] = x1;
						}
						else
						{
							tmpx[0] += xadvance;
							tmpx[1] += xadvance;
						}

						y += yadvance;
					}
				}

				x = tmpx[0];
				tx = tmpx[1];

				if(caretIndex >= partition.Index + partition.Count)
				{
					if (CaretX != nullptr)
					{
						*CaretX = x;
					}

					if (CaretY != nullptr)
					{
						*CaretY = y;
					}
				}
			}
			break;
		}

		if (partition.Type != ETextHandler::ETextType::eNewLine)
		{
			newline = false;
		}

		if (tx > linesWidth[line])
		{
			linesWidth[line] = tx;
		}

		if (tx > maxwidth)
		{
			maxwidth = tx;
		}

		if (y + _Font->_MaxAdvanceY > maxheight)
		{
			maxheight = y + _Font->_MaxAdvanceY;
		}
	}

	if (CaretX != nullptr)
	{
		mu_uint8 Horizontal = (Horizontal & ((~0) ^ ETF_DISABLE_AUTOPOS));
		switch (Horizontal)
		{
		case ETA_LEFT:
			{
				// Nothing To Do
			}
			break;

		case ETA_CENTER:
			{
				*CaretX += (maxwidth - linesWidth[caretline]) * 0.5f;
			}
			break;

		case ETA_RIGHT:
			{
				*CaretX += (maxwidth - linesWidth[caretline]);
			}
			break;
		}
	}

	Size->x = maxwidth;
	Size->y = maxheight;
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

mu_uint32 MUTextSimpleRender::GetCharacterIndexByMouse(mu_int32 UniqueId, mu_float MouseX, mu_float MouseY, const mu_text *Text, mu_uint8 HorizontalAlign)
{
	auto it = _Cache.find(UniqueId);

	if (it == _Cache.end())
		return 0;

	const MUText *textData = it->second;

	EVector2 mouseX(MouseX, 0.0f);
	EVector2 mouseY(0.0f, MouseY);

	const mu_text *srctext = Text;
	mu_size srctextsize = mu_strlen(Text);

	if (srctextsize == 0) return 0;

	std::vector<ETextHandler::ETextPartition> partitions;
	mu_uint32 linesCount;
	mu_uint32 backInstanceCount;
	mu_uint32 textInstanceCount;

	ETextHandler::PreparePartitionsNoColor(srctext, srctextsize, partitions);
	ETextHandler::CalculateInstancesCount(partitions, linesCount, textInstanceCount, backInstanceCount);

	std::vector<mu_float> linesWidth(linesCount + 1, 0.0f);

	hb_font_t *hbfont = _Font->_HBFont;
	hb_buffer_t *hbbuffer = _Font->_HBBuffer;
	const mu_float invTextureSize = _Font->_InvTextureSize;

	const mu_float ascender = _Font->_Ascender;
	const mu_float borderwidth = textData->_Border.x * 0.5f, borderheight = textData->_Border.y * 0.5f;
	mu_float maxwidth = 0.0f, maxheight = 0.0f;
	mu_float x = 0.0f, y = 0.0f;
	mu_float tx = 0.0f;
	mu_uint32 line = 0;
	mu_boolean newline = false;

	mu_uint32 glyphCount = 0;
	mu_uint32 glyphIndex = 0;
	hb_glyph_info_t *glyphInfo;
	hb_glyph_position_t *glyphPos;
	mu_boolean parsedText = false;

	for (auto iter = partitions.begin(); iter != partitions.end(); ++iter)
	{
		ETextHandler::ETextPartition &partition = *iter;

		if (partition.Type == ETextHandler::ETextType::eText &&
			parsedText == false)
		{
			ETextHandler::ParseTextLine(_Font, hbfont, hbbuffer, iter, partitions.end(), srctext, srctextsize, glyphCount, glyphInfo, glyphPos);
			glyphIndex = 0;
			parsedText = true;
		}

		switch(partition.Type)
		{
		case ETextHandler::ETextType::eColor:
			break;

		case ETextHandler::ETextType::eNewLine:
			{
				x = 0.0f;
				y += _Font->_MaxAdvanceY * partition.Count;
				line += partition.Count;
			}
			break;

		case ETextHandler::ETextType::eTab:
			{
				const mu_float xadvance = _Font->_MaxAdvanceX * partition.Count;
				x += xadvance;
			}
			break;

		case ETextHandler::ETextType::eText:
			{
				mu_float tmpx[2] = { x, x };
				const mu_uint32 maxJ = partition.Index + partition.Count;
				for (mu_uint32 j = partition.Index; j < maxJ; ++j, ++glyphIndex)
				{
					hb_glyph_info_t &ginfo = glyphInfo[glyphIndex];
					hb_glyph_position_t &gpos = glyphPos[glyphIndex];

					const mu_float xadvance = (mu_float)gpos.x_advance / 64.0f;
					const mu_float yadvance = (mu_float)gpos.y_advance / 64.0f;
					const mu_float xoffset = (mu_float)gpos.x_offset / 64.0f;
					const mu_float yoffset = (mu_float)gpos.y_offset / 64.0f;

					if (srctext[j] != _T(' '))
					{
						const mu_uint32 glyphIndex = ginfo.codepoint;
						const MUFontGlyph *glyph = _Font->GetCharacter(glyphIndex);
						mu_assert(glyph && "Retrieve glyph character failed!");

						const mu_float x0 = (tmpx[0] + xoffset + glyph->BearingX + borderwidth);
						const mu_float y0 = (y + yoffset - glyph->BearingY + ascender + borderheight);
						const mu_float x1 = (x0 + glyph->SizeU);
						const mu_float y1 = (y0 + glyph->SizeV);

						tmpx[0] += xadvance;
						tmpx[1] = x1;
					}
					else
					{
						tmpx[0] += xadvance;
						tmpx[1] += xadvance;
					}

					y += yadvance;
				}

				x = tmpx[0] - borderwidth;
				tx = tmpx[1] - borderwidth;
			}
			break;
		}

		if (partition.Type != ETextHandler::ETextType::eNewLine)
		{
			newline = false;
		}

		if (tx > linesWidth[line])
		{
			linesWidth[line] = tx;
		}

		if (tx > maxwidth)
		{
			maxwidth = tx;
		}

		if (y + _Font->_MaxAdvanceY > maxheight)
		{
			maxheight = y + _Font->_MaxAdvanceY;
		}
	}

	if (maxwidth > 0.0f)
	{
		maxwidth += textData->_Border.x;
	}

	if (maxheight > 0.0f)
	{
		maxheight += textData->_Border.y;
	}

	if (maxwidth < textData->_BoxWidth)
	{
		maxwidth = textData->_BoxWidth;
	}

	if (maxheight < textData->_BoxHeight)
	{
		maxheight = textData->_BoxHeight;
	}

	mu_float distanceX = FLT_MAX;
	mu_float distanceY = FLT_MAX;
	mu_uint32 curLine = 0;
	mu_uint32 curIndex = 0;

	// Calculate which line will be checked
	// Upper Y
	{
		EVector2 pos = EVector2(EMath::Transform(textData->_Model, EVector3(0.0f, _Font->_MaxAdvanceY, 0.0f)));
		pos.x = 0.0f;
		const mu_float d = EMath::Distance(pos, mouseY);
		if (d < distanceY)
		{
			distanceY = d;
			curLine = 0;
		}
	}

	// Per Line Y
	{
		for (mu_uint32 line = 1; line < linesCount - 1; ++line)
		{
			EVector2 pos = EVector2(EMath::Transform(textData->_Model, EVector3(0.0f, _Font->_MaxAdvanceY * (line + 1), 0.0f)));
			pos.x = 0.0f;
			const mu_float d = EMath::Distance(pos, mouseY);
			if (d >= distanceY)
			{
				break;
			}

			distanceY = d;
			curLine = line;
		}
	}
	
	// Lower Y
	{
		EVector2 pos = EVector2(EMath::Transform(textData->_Model, EVector3(0.0f, _Font->_MaxAdvanceY * linesCount, 0.0f)));
		pos.x = 0.0f;
		const mu_float d = EMath::Distance(pos, mouseY);
		if (d < distanceY)
		{
			distanceY = d;
			curLine = linesCount - 1;
		}
	}

	mu_float offsetx;
	x = 0.0f, y = 0.0f;
	tx = 0.0f;
	line = 0;

	mu_uint8 Horizontal = (textData->_HorizontalAlign & ((~0) ^ ETF_DISABLE_AUTOPOS));
	switch (Horizontal)
	{
	case ETA_LEFT:
		{
			offsetx = 0.0f;
		}
		break;

	case ETA_CENTER:
		{
			offsetx = (maxwidth - linesWidth[curLine]) * 0.5f;
		}
		break;

	case ETA_RIGHT:
		{
			offsetx = (maxwidth - linesWidth[curLine]);
		}
		break;
	}

	glyphCount = 0;
	glyphIndex = 0;
	parsedText = false;

	for (auto iter = partitions.begin(); iter != partitions.end() && line <= curLine; ++iter)
	{
		ETextHandler::ETextPartition &partition = *iter;

		if (partition.Type == ETextHandler::ETextType::eText &&
			parsedText == false)
		{
			ETextHandler::ParseTextLine(_Font, hbfont, hbbuffer, iter, partitions.end(), srctext, srctextsize, glyphCount, glyphInfo, glyphPos);
			glyphIndex = 0;
			parsedText = true;
		}

		switch (partition.Type)
		{
		case ETextHandler::ETextType::eNewLine:
			{
				if (curLine >= line &&
					curLine < line + partition.Count)
				{
					EVector2 pos = EVector2(EMath::Transform(textData->_Model, EVector3(x, 0.0f, 0.0f)));
					pos.y = 0.0f;
					if (pos.x > mouseX.x)
					{
						return curIndex;
					}

					const mu_float d = EMath::Distance(pos, mouseX);
					if (d >= distanceX)
					{
						return curIndex;
					}

					// Found the mouse position index
					return partition.Index + (curLine - line) - partition.Colors;
				}

				x = 0.0f;
				y += _Font->_MaxAdvanceY * partition.Count;
				line += partition.Count;
			}
			break;

		case ETextHandler::ETextType::eTab:
			{
				if (curLine == line)
				{
					for (mu_uint32 n = 0; n < partition.Count; ++n)
					{
						EVector2 pos = EVector2(EMath::Transform(textData->_Model, EVector3(x + offsetx + _Font->_MaxAdvanceX * n + _Font->_MaxAdvanceX * 0.5f, 0.0f, 0.0f)));
						pos.y = 0.0f;
						const mu_float d = EMath::Distance(pos, mouseX);
						if (d >= distanceX)
						{
							return curIndex;
						}

						distanceX = d;
						curIndex = partition.Index + n - partition.Colors;
					}
				}

				const mu_float xadvance = _Font->_MaxAdvanceX * partition.Count;
				x += xadvance;
			}
			break;

		case ETextHandler::ETextType::eText:
			{
				if (line == curLine)
				{
					mu_float tmpx[2] = { x, x };
					const mu_uint32 maxJ = partition.Index + partition.Count;
					for (mu_uint32 j = partition.Index; j < maxJ; ++j, ++glyphIndex)
					{
						hb_glyph_info_t &ginfo = glyphInfo[glyphIndex];
						hb_glyph_position_t &gpos = glyphPos[glyphIndex];

						const mu_float xadvance = (mu_float)gpos.x_advance / 64.0f;
						const mu_float yadvance = (mu_float)gpos.y_advance / 64.0f;
						const mu_float xoffset = (mu_float)gpos.x_offset / 64.0f;
						const mu_float yoffset = (mu_float)gpos.y_offset / 64.0f;

						if (srctext[j] != _T(' '))
						{
							const mu_uint32 glyphIndex = ginfo.codepoint;
							const MUFontGlyph *glyph = _Font->GetCharacter(glyphIndex);
							mu_assert(glyph && "Retrieve glyph character failed!");

							const mu_float x0 = (tmpx[0] + xoffset + glyph->BearingX + borderwidth);
							const mu_float y0 = (y + yoffset - glyph->BearingY + ascender + borderheight);
							const mu_float x1 = (x0 + glyph->SizeU);
							const mu_float y1 = (y0 + glyph->SizeV);

							EVector2 pos = EVector2(EMath::Transform(textData->_Model, EVector3(x0, 0.0f, 0.0f)));
							pos.y = 0.0f;
							if (pos.x > mouseX.x)
							{
								return curIndex;
							}

							const mu_float d = EMath::Distance(pos, mouseX);
							if (d >= distanceX)
							{
								return curIndex;
							}

							distanceX = d;
							curIndex = j - partition.Colors;

							tmpx[0] += xadvance;
							tmpx[1] = x1;
						}
						else
						{
							EVector2 pos = EVector2(EMath::Transform(textData->_Model, EVector3(tmpx[0], 0.0f, 0.0f)));
							pos.y = 0.0f;
							if (pos.x > mouseX.x)
							{
								return curIndex;
							}

							const mu_float d = EMath::Distance(pos, mouseX);
							if (d >= distanceX)
							{
								return curIndex;
							}

							distanceX = d;
							curIndex = j - partition.Colors;

							tmpx[0] += xadvance;
							tmpx[1] += xadvance;
						}

						y += yadvance;
					}

					x = tmpx[0] - borderwidth;
					tx = tmpx[1] - borderwidth;
				}
			}
			break;
		}
	}

	if (line == curLine)
	{
		EVector2 pos = EVector2(EMath::Transform(textData->_Model, EVector3(x, 0.0f, 0.0f)));
		pos.y = 0.0f;
		if (pos.x > mouseX.x)
		{
			return curIndex;
		}

		const mu_float d = EMath::Distance(pos, mouseX);
		if (d < distanceX)
		{
			return curIndex + 1;
		}
	}

	return curIndex;
}