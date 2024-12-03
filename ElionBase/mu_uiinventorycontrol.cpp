#include "stdafx.h"
#include "mu_uiinventorycontrol.h"

UIPickedItem g_pPickedItem;

UIInventoryControl::UIInventoryControl(UISceneManager *manager, UIBase *owner, mu_uint32 beginNumberIndex, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_uint32 column, mu_uint32 row, mu_uint8 inventoryType) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(aspectWidth, aspectHeight);
	UIBase::SetPosition(x, y);
	UIBase::SetDimensions(column * INVENTORY_SQUARE_WIDTH, row * INVENTORY_SQUARE_HEIGHT);
	UIBase::SetParent(owner);
	SetEvent(GFX_EVENT::GFX_MOUSE_EVENT | GFX_EVENT::GFX_MOUSE_DBLCLICK_AS_CLICK);

	// Initialize
	{
		mu_float width = column * INVENTORY_SQUARE_WIDTH;
		mu_float height = row * INVENTORY_SQUARE_HEIGHT;

		_InventoryType = inventoryType;

		_CanPush = false;
		_RepairMode = false;
		_Locked = false;
		_PointedSquareIndex = -1;
		_BeginNumberIndex = beginNumberIndex;
		_ToolTipType = UNKNOWN_TOOLTIP_TYPE;

		SetSquareColorNormal(25, 102, 204);
		SetSquareColorWarning(255, 51, 51);

		_Column = column;
		_Row = row;
		_ItemCheckBox = new mu_uint32[column * row];
		mu_zeromem(_ItemCheckBox, sizeof(mu_uint32) * column * row);

		_UseboxColor.resize(column * row);
		_UseboxImage.resize(column * row);
		_PickedColor.resize(column * row);
		_PickedWarningColor = new UISpriteColor(manager, this, MU::MakeARGB(255, 51, 51, 102), 640.0f, 480.0f, x, y, INVENTORY_SQUARE_WIDTH + 1, INVENTORY_SQUARE_HEIGHT + 1);
		_PickedWarningColor->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		for (mu_uint32 ty = 0; ty < row; ++ty)
		{
			for (mu_uint32 tx = 0; tx < column; ++tx)
			{
				UISpriteColor* &color = _UseboxColor[ty * column + tx];
				UISprite* &image = _UseboxImage[ty * column + tx];
				UISpriteColor* &picked = _PickedColor[ty * column + tx];

				color = new UISpriteColor(manager, this, MU::MakeARGB(255, 255, 255, 255), 640.0f, 480.0f, x + tx * INVENTORY_SQUARE_WIDTH, y + ty * INVENTORY_SQUARE_HEIGHT, INVENTORY_SQUARE_WIDTH + 1, INVENTORY_SQUARE_HEIGHT + 1);
				color->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
				image = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::INVENFRAME_ITEM_BOX], 640.0f, 480.0f, x + tx * INVENTORY_SQUARE_WIDTH, y + ty * INVENTORY_SQUARE_HEIGHT, INVENTORY_SQUARE_WIDTH + 1, INVENTORY_SQUARE_HEIGHT + 1);
				image->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
				picked = new UISpriteColor(manager, this, MU::MakeARGB(255, 255, 255, 255), 640.0f, 480.0f, x + tx * INVENTORY_SQUARE_WIDTH, y + ty * INVENTORY_SQUARE_HEIGHT, INVENTORY_SQUARE_WIDTH, INVENTORY_SQUARE_HEIGHT);
				picked->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			}
		}

		_TopLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_LEFT], 640.0f, 480.0f, x - WND_LEFT_EDGE, y - WND_TOP_EDGE, 14.0f, 14.0f);
		_TopLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_TopRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_RIGHT], 640.0f, 480.0f, x + width - WND_RIGHT_EDGE, y - WND_TOP_EDGE, 14.0f, 14.0f);
		_TopRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BottomLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LEFT], 640.0f, 480.0f, x - WND_LEFT_EDGE, y + height - WND_BOTTOM_EDGE, 14.0f, 14.0f);
		_BottomLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BottomRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_RIGHT], 640.0f, 480.0f, x + width - WND_RIGHT_EDGE, y + height - WND_BOTTOM_EDGE, 14.0f, 14.0f);
		_BottomRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		mu_float lineWidth = width - 14.0f;
		mu_float lineHeight = height - 14.0f;
		_TopLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_LINE], 640.0f, 480.0f, x - WND_RIGHT_EDGE + 14.0f, y - WND_TOP_EDGE, lineWidth, 14.0f);
		_TopLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BottomLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LINE], 640.0f, 480.0f, x - WND_RIGHT_EDGE + 14.0f, y + height - WND_BOTTOM_EDGE, lineWidth, 14.0f);
		_BottomLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_LeftLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_LEFT_LINE], 640.0f, 480.0f, x - WND_LEFT_EDGE, y - WND_TOP_EDGE + 14.0f, 14.0f, lineHeight);
		_LeftLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_RightLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_RIGHT_LINE], 640.0f, 480.0f, x + width - WND_RIGHT_EDGE, y - WND_TOP_EDGE + 14.0f, 14.0f, lineHeight);
		_RightLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		SetPosition(x, y);

		SetToolTipType(TOOLTIP_TYPE_INVENTORY);
	}
}

UIInventoryControl::~UIInventoryControl()
{
	RemoveAllItems();
	MU_SAFE_DELETE_ARRAY(_ItemCheckBox);
}

void UIInventoryControl::SetPosition(mu_float x, mu_float y)
{
	UIBase::SetPosition(x, y);

	for (mu_uint32 ty = 0; ty < _Row; ++ty)
	{
		for (mu_uint32 tx = 0; tx < _Column; ++tx)
		{
			UISpriteColor* &color = _UseboxColor[ty * _Column + tx];
			UISprite* &image = _UseboxImage[ty * _Column + tx];
			UISpriteColor* &picked = _PickedColor[ty * _Column + tx];

			color->SetPosition(x + tx * INVENTORY_SQUARE_WIDTH, y + ty * INVENTORY_SQUARE_HEIGHT);
			image->SetPosition(x + tx * INVENTORY_SQUARE_WIDTH, y + ty * INVENTORY_SQUARE_HEIGHT);
			picked->SetPosition(x + tx * INVENTORY_SQUARE_WIDTH, y + ty * INVENTORY_SQUARE_HEIGHT);
		}
	}

	_TopLeft->SetPosition(x - WND_LEFT_EDGE, y - WND_TOP_EDGE);
	_TopRight->SetPosition(x + _ViewWidth - WND_RIGHT_EDGE, y - WND_TOP_EDGE);
	_BottomLeft->SetPosition(x - WND_LEFT_EDGE, y + _ViewHeight - WND_BOTTOM_EDGE);
	_BottomRight->SetPosition(x + _ViewWidth - WND_RIGHT_EDGE, y + _ViewHeight - WND_BOTTOM_EDGE);

	_TopLine->SetPosition(x - WND_RIGHT_EDGE + 14.0f, y - WND_TOP_EDGE);
	_BottomLine->SetPosition(x - WND_RIGHT_EDGE + 14.0f, y + _ViewHeight - WND_BOTTOM_EDGE);
	_LeftLine->SetPosition(x - WND_LEFT_EDGE, y - WND_TOP_EDGE + 14.0f);
	_RightLine->SetPosition(x + _ViewWidth - WND_RIGHT_EDGE, y - WND_TOP_EDGE + 14.0f);
}

void UIInventoryControl::Update(mu_uint64 CurrentTime)
{

}

void UIInventoryControl::Draw()
{
	ITEM *PickedItem = g_pPickedItem.GetItem();

	mu_int32 x, y;
	for (y = 0; y < _Row; ++y)
	{
		for (x = 0; x < _Column; ++x)
		{
			mu_int32 iCurSquareIndex = y*_Column + x;
			if (_ItemCheckBox[iCurSquareIndex] > 1)
			{
				UISpriteColor* &color = _UseboxColor[y * _Column + x];
				ITEM* pItem = FindItemByKey(_ItemCheckBox[iCurSquareIndex]);

				if (pItem != nullptr && pItem != PickedItem)
				{
					if (CanChangeItemColorState(pItem) == true)
					{
						SetItemColorState(pItem);
					}

					if (pItem->byColorState == ITEM_COLOR_NORMAL)
					{
						color->SetColor(MU::MakeARGB(76, 127, 127, 153));
					}
					else if (pItem->byColorState == ITEM_COLOR_DURABILITY_50)
					{
						color->SetColor(MU::MakeARGB(255, 255, 0, 102));
					}
					else if (pItem->byColorState == ITEM_COLOR_DURABILITY_70)
					{
						color->SetColor(MU::MakeARGB(255, 168, 0, 102));
					}
					else if (pItem->byColorState == ITEM_COLOR_DURABILITY_80)
					{
						color->SetColor(MU::MakeARGB(255, 84, 0, 102));
					}
					else if (pItem->byColorState == ITEM_COLOR_DURABILITY_100)
					{
						color->SetColor(MU::MakeARGB(255, 0, 0, 102));
					}
					else if (pItem->byColorState == ITEM_COLOR_TRADE_WARNING)
					{
						color->SetColor(MU::MakeARGB(255, 51, 25, 102));
					}
				}
				else
				{
					color->SetColor(MU::MakeARGB(76, 127, 127, 153));
				}

				color->Draw();
			}

			UISprite* &image = _UseboxImage[y * _Column + x];
			ITEM* pItem = nullptr;
			mu_int32 iItemHeight = 0;

			if (_ItemCheckBox[iCurSquareIndex] > 1)
			{
				pItem = FindItemByKey(_ItemCheckBox[iCurSquareIndex]);
				iItemHeight = ItemAttribute[pItem->Type].Height;
			}

			if (pItem == nullptr || !(pItem->Durability == 254 && IsInvenItem(pItem->Type)))
			{
				image->SetTexture(&INTERFACES::Data[INTERFACES::INVENFRAME_ITEM_BOX]);
			}
			else
			{
				if (iItemHeight == 1)
				{
					image->SetTexture(&INTERFACES::Data[INTERFACES::INVENFRAME_USEBOX01]);
				}
				else if (y == _Row - 1 || pItem == FindItem(x, y - 1))
				{
					image->SetTexture(&INTERFACES::Data[INTERFACES::INVENFRAME_USEBOX03]);
				}
				else
				{
					image->SetTexture(&INTERFACES::Data[INTERFACES::INVENFRAME_USEBOX02]);
				}
			}

			image->Draw();
		}
	}

	_TopLeft->Draw();
	_TopRight->Draw();
	_BottomLeft->Draw();
	_BottomRight->Draw();
	_TopLine->Draw();
	_BottomLine->Draw();
	_LeftLine->Draw();
	_RightLine->Draw();

	if (g_pPickedItem.GetItem() != nullptr)
	{
		mu_boolean pickitemvisible = g_pPickedItem.IsVisible();

		if (pickitemvisible)
		{
			ERect rcPickedItem, rcInventory, rcIntersect;
			g_pPickedItem.GetRect(rcPickedItem);
			GetRect(rcInventory);

			if (EMath::IntersectRect(&rcIntersect, &rcPickedItem, &rcInventory))
			{
				ITEM* pPickItem = g_pPickedItem.GetItem();
				ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[pPickItem->Type];
				mu_int32 iPickedItemX = MouseX - ((pItemAttr->Width - 1) * INVENTORY_SQUARE_WIDTH / 2);
				mu_int32 iPickedItemY = MouseY - ((pItemAttr->Height - 1) * INVENTORY_SQUARE_HEIGHT / 2);

				mu_int32 iColumnX = 0, iRowY = 0;
				mu_int32 nItemColumn = pItemAttr->Width, nItemRow = pItemAttr->Height;
				if (false == GetSquarePosAtPoint(iPickedItemX, iPickedItemY, iColumnX, iRowY))
				{
					iColumnX = ((iPickedItemX - rcInventory.left) / INVENTORY_SQUARE_WIDTH);

					if (iPickedItemX - rcInventory.left < 0)
						iColumnX = ((iPickedItemX - rcInventory.left) / INVENTORY_SQUARE_WIDTH) - 1;
					else
						iColumnX = (iPickedItemX - rcInventory.left) / INVENTORY_SQUARE_WIDTH;

					if (iPickedItemY - rcInventory.top < 0)
						iRowY = ((iPickedItemY - rcInventory.top) / INVENTORY_SQUARE_HEIGHT) - 1;
					else
						iRowY = (iPickedItemY - rcInventory.top) / INVENTORY_SQUARE_HEIGHT;
				}

				mu_boolean bWarning = false;

				if (iColumnX < 0 && iColumnX >= -nItemColumn)
				{
					nItemColumn = nItemColumn + iColumnX;
					iColumnX = 0;
					bWarning = true;
				}
				if (iColumnX + nItemColumn > _Column && iColumnX < _Column)
				{
					nItemColumn = _Column - iColumnX;
					bWarning = true;
				}
				if (iRowY < 0 && iRowY >= -nItemRow)
				{
					nItemRow = nItemRow + iRowY;
					iRowY = 0;
					bWarning = true;
				}
				if (iRowY + nItemRow > _Row && iRowY < _Row)
				{
					nItemRow = _Row - iRowY;
					bWarning = true;
				}

				mu_int32 iDestPosX = _ViewX + iColumnX * INVENTORY_SQUARE_WIDTH;
				mu_int32 iDestPosY = _ViewY + iRowY * INVENTORY_SQUARE_HEIGHT;
				mu_int32 iDestWidth = nItemColumn * INVENTORY_SQUARE_WIDTH;
				mu_int32 iDestHeight = nItemRow * INVENTORY_SQUARE_HEIGHT;

				_CanPush = bWarning;

				if (bWarning)
				{
					_PickedWarningColor->SetPosition(iDestPosX, iDestPosY);
					_PickedWarningColor->SetDimensions(iDestWidth, iDestHeight);
					_PickedWarningColor->Draw();
				}
				else if (iColumnX >= 0 && iColumnX < _Column && iRowY >= 0 && iRowY < _Row)
				{
					for (mu_int32 y = 0; y < nItemRow; ++y)
					{
						for (mu_int32 x = 0; x < nItemColumn; ++x)
						{
							mu_int32 iSquarePosX = iColumnX + x;
							mu_int32 iSquarePosY = iRowY + y;
							mu_int32 iCurSquareIndex = iSquarePosY*_Column + iSquarePosX;
							UISpriteColor* &color = _PickedColor[iCurSquareIndex];

							if (_ItemCheckBox[iCurSquareIndex] > 1)
							{
								mu_boolean bSuccess = false;

								ITEM* pTargetItem = FindItemByKey(_ItemCheckBox[iCurSquareIndex]);

								if (pTargetItem == PickedItem)
								{
									color->SetColor(_NormalColor);
								}
								else
								{
									if (pTargetItem)
									{
										mu_int32 iType = pTargetItem->Type;
										mu_int32 iDurability = pTargetItem->Durability;

										if (pPickItem->Type == ITEM_POTION + 13 ||
											pPickItem->Type == ITEM_POTION + 14 ||
											pPickItem->Type == ITEM_POTION + 16)
										{
											bSuccess = CanUpgradeItem(pPickItem, pTargetItem);
										}
										else if (pPickItem->Type == ITEM_POTION + 42)
										{
											if (pTargetItem->HarmonyOption == 0)
											{
												StrengthenItem strengthitem = g_pUIJewelHarmonyinfo.GetItemType(static_cast<mu_int32>(pTargetItem->Type));

												if ((strengthitem != SI_None) && (!g_SocketItemMgr.IsSocketItem(pTargetItem))
													&& ((pTargetItem->SetOption % 0x04) != EXT_A_SET_OPTION && (pTargetItem->SetOption % 0x04) != EXT_B_SET_OPTION))
												{
													bSuccess = true;
												}
											}
										}
										else if (pPickItem->Type == ITEM_POTION + 43
											|| pPickItem->Type == ITEM_POTION + 44)
										{
											if (pTargetItem->HarmonyOption != 0)
											{
												bSuccess = true;
											}
										}

										if (pPickItem->Type == ITEM_POTION + 13
											&& iType == ITEM_HELPER + 37 && iDurability != 255)
										{
											bSuccess = true;
										}

										if (bSuccess == false && _Parent == g_pMyInventory)
										{
											bSuccess = IsOverlayItem(pPickItem, pTargetItem);
										}
									}

									if (bSuccess)
									{
										SetSquareColorWarning(51, 102, 51);
									}
									else
									{
										SetSquareColorWarning(255, 51, 51);
									}

									color->SetColor(_WarningColor);
								}
							}
							else
							{
								color->SetColor(_NormalColor);
							}

							color->Draw();
						}
					}
				}
			}
		}
	}

	MUThreadInfo &ThreadInfo = g_ThreadInfo[0];

	if (_SlotCount > 0)
	{
		ThreadInfo.EnableScissor = false;
		g_Tasking.RenderGUIModelsAtRuntime(_SlotIndex, _SlotCount);
	}
}

void UIInventoryControl::Draw3D()
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[0];
	++ThreadInfo.GUIRenderIndex;

	_SlotIndex = g_GUIRenderSlotCount;

	mu_float sx = _ViewX;
	mu_float sy = _ViewY;

	ITEM *PickedItem = g_pPickedItem.GetItem();

	if (PickedItem == nullptr ||
		g_pPickedItem.IsVisible() == false)
	{
		for (auto it = _Items.begin(); it != _Items.end(); ++it)
		{
			ITEM* pItem = it->second;
			ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[pItem->Type];

			mu_float x = sx + (pItem->x*INVENTORY_SQUARE_WIDTH);
			mu_float y = sy + (pItem->y*INVENTORY_SQUARE_HEIGHT);
			mu_float width = pItemAttr->Width*INVENTORY_SQUARE_WIDTH;
			mu_float height = pItemAttr->Height*INVENTORY_SQUARE_HEIGHT;
			ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);

			RenderItem3D(x, y, width, height, pItem->Type, pItem->Level, pItem->ExcOption, pItem->SetOption, false);
		}
	}
	else
	{
		for (auto it = _Items.begin(); it != _Items.end(); ++it)
		{
			ITEM* pItem = it->second;
			ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[pItem->Type];

			mu_float x = sx + (pItem->x*INVENTORY_SQUARE_WIDTH);
			mu_float y = sy + (pItem->y*INVENTORY_SQUARE_HEIGHT);
			mu_float width = pItemAttr->Width*INVENTORY_SQUARE_WIDTH;
			mu_float height = pItemAttr->Height*INVENTORY_SQUARE_HEIGHT;

			if (PickedItem == pItem)
			{
				ThreadInfo.RenderLight = MU::Float4ToARGB(0.2f, 0.2f, 0.2f, 0.3f);
			}
			else
			{
				ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);
			}

			RenderItem3D(x, y, width, height, pItem->Type, pItem->Level, pItem->ExcOption, pItem->SetOption, false);
		}
	}

	_SlotCount = g_GUIRenderSlotCount - _SlotIndex;
}

void UIInventoryControl::PostDraw()
{
	if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_PARTCHARGE_SHOP))
	{
		return;
	}

	RenderNumberOfItem();

	if (g_pPickedItem.GetItem() == nullptr)
	{
		RenderItemToolTip();
	}
}

void UIInventoryControl::RenderNumberOfItem()
{
	mu_float sx = _ViewX;
	mu_float sy = _ViewY;

	mu_uint32 beginIndex = _BeginNumberIndex;

	for (auto li = _Items.begin(); li != _Items.end(); ++li)
	{
		ITEM* pItem = li->second;
		ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[pItem->Type];
		mu_float x = sx + (pItem->x*INVENTORY_SQUARE_WIDTH);
		mu_float y = sy + (pItem->y*INVENTORY_SQUARE_HEIGHT);
		mu_float width = pItemAttr->Width*INVENTORY_SQUARE_WIDTH;

		mu_boolean render = true;
		mu_int64 number;
		mu_uint32 textColor;

		if (pItem->Type >= ITEM_POTION && pItem->Type <= ITEM_POTION + 8 && pItem->Durability > 1)
		{
			number = pItem->Durability;
			textColor = MU::MakeARGB(255, 229, 178, 255);
			x = x + width - 6.0f;
			y = y + 1.0f;
		}
		else if (pItem->Type >= ITEM_POTION + 46 && pItem->Type <= ITEM_POTION + 50 && pItem->Durability > 1)
		{
			number = pItem->Durability;
			textColor = MU::MakeARGB(255, 229, 178, 255);
			x = x + width - 6.0f;
			y = y + 1.0f;
		}
		else if (pItem->Type >= ITEM_POTION + 35 && pItem->Type <= ITEM_POTION + 40 && pItem->Durability > 1)
		{
			number = pItem->Durability;
			textColor = MU::MakeARGB(255, 229, 178, 255);
			x = x + width - 6.0f;
			y = y + 1.0f;
		}
		else if (pItem->Type >= ITEM_POTION + 70 && pItem->Type <= ITEM_POTION + 71 && pItem->Durability > 1)
		{
			number = pItem->Durability;
			textColor = MU::MakeARGB(255, 229, 178, 255);
			x = x + width - 6.0f;
			y = y + 1.0f;
		}
		else if (pItem->Type == ITEM_POTION + 94 && pItem->Durability > 1)
		{
			number = pItem->Durability;
			textColor = MU::MakeARGB(255, 229, 178, 255);
			x = x + width - 6.0f;
			y = y + 1.0f;
		}
		else if (pItem->Type >= ITEM_POTION + 78 && pItem->Type <= ITEM_POTION + 82 && pItem->Durability > 1)
		{
			number = pItem->Durability;
			textColor = MU::MakeARGB(255, 229, 178, 255);
			x = x + width - 6.0f;
			y = y + 1.0f;
		}
		else if (pItem->Type >= ITEM_POTION + 85 && pItem->Type <= ITEM_POTION + 90 && pItem->Durability > 1)
		{
			number = pItem->Durability;
			textColor = MU::MakeARGB(255, 229, 178, 255);
			x = x + width - 6.0f;
			y = y + 1.0f;
		}
		else if (pItem->Type == ITEM_POTION + 133 && pItem->Durability > 1)
		{
			number = pItem->Durability;
			textColor = MU::MakeARGB(255, 229, 178, 255);
			x = x + width - 6.0f;
			y = y + 1.0f;
		}
		else if (pItem->Type >= ITEM_POTION + 153 && pItem->Type <= ITEM_POTION + 156 && pItem->Durability > 1)
		{
			number = pItem->Durability;
			textColor = MU::MakeARGB(255, 229, 178, 255);
			x = x + width - 6.0f;
			y = y + 1.0f;
		}
		else if (COMGEM::isCompiledGem(pItem))
		{
			mu_int32 Level = pItem->Level;
			number = (Level + 1) * COMGEM::FIRST;
			textColor = MU::MakeARGB(255, 229, 178, 255);
			x = x + width - 6.0f;
			y = y + 1.0f;
		}
		else
		{
			render = false;
		}

		if (render)
		{
			mu_unicode textBuffer[256];
			mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("\uE000%08X\uE000\uE001%08X\uE001%lld"), textColor, 0, number);
			FONT_NUMBER->DrawStringOutline2D(beginIndex++,
				MU_TO_WINDOW(x, MU_RESOURCE->_640x480Rate[0]),
				MU_TO_WINDOW(y, MU_RESOURCE->_640x480Rate[1]),
				0.8f, ETA_CENTER, ETA_BOTTOM, 0.0,
				MU::MakeARGB(0, 0, 0, 255),
				textBuffer);
		}
	}
}

void UIInventoryControl::RenderItemToolTip()
{
	if (_PointedSquareIndex != -1 &&
		_ItemCheckBox[_PointedSquareIndex] > 0)
	{
		ITEM *Item = FindItemByKey(_ItemCheckBox[_PointedSquareIndex]);

		if(Item != nullptr)
		{
			ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[Item->Type];
			mu_int32 iTargetX = _ViewX + Item->x*INVENTORY_SQUARE_WIDTH + pItemAttr->Width*INVENTORY_SQUARE_WIDTH / 2;
			mu_int32 iTargetY = _ViewY + Item->y*INVENTORY_SQUARE_HEIGHT;
			mu_boolean alternativePos = false;

			if (iTargetY + pItemAttr->Height * INVENTORY_SQUARE_HEIGHT < 480.0f * 0.5f)
			{
				alternativePos = true;
				if (pItemAttr->Height <= 1)
				{
					iTargetY += ((mu_float)pItemAttr->Height + 0.5f) * (mu_float)INVENTORY_SQUARE_HEIGHT;
				}
				else
				{
					iTargetY += pItemAttr->Height * INVENTORY_SQUARE_HEIGHT;
				}
			}

			if (_ToolTipType == TOOLTIP_TYPE_INVENTORY)
			{
				RenderItemInfo(iTargetX, iTargetY, Item, false, alternativePos);
			}
			else if (_ToolTipType == TOOLTIP_TYPE_REPAIR)
			{
				RenderRepairInfo(iTargetX, iTargetY, Item, false, alternativePos);
			}
			else if (_ToolTipType == TOOLTIP_TYPE_NPC_SHOP)
			{
				RenderItemInfo(iTargetX, iTargetY, Item, true, alternativePos);
			}
			else if (_ToolTipType == TOOLTIP_TYPE_MY_SHOP)
			{
				RenderItemInfo(iTargetX, iTargetY, Item, false, alternativePos, _ToolTipType);
			}
			else if (_ToolTipType == TOOLTIP_TYPE_PURCHASE_SHOP)
			{
				RenderItemInfo(iTargetX, iTargetY, Item, false, alternativePos, _ToolTipType);
			}
		}
	}
}

void UIInventoryControl::OnMouseLeave()
{
	_PointedSquareIndex = -1;
}

void UIInventoryControl::OnMouseMove(mu_int32 mouse_x, mu_int32 mouse_y)
{
	mu_int32 mx = MU_TO_VIEWPORT(mouse_x, _AspectWidth);
	mu_int32 my = MU_TO_VIEWPORT(mouse_y, _AspectHeight);
	mu_int32 CurSquareIndex = GetSquareIndexAtPoint(mx, my);

	if (CurSquareIndex != _PointedSquareIndex)
	{
#if 0 // FIX
		if (g_pPickedItem.GetItem() == nullptr &&
			g_pMyShopInventory->IsEnableInputValueTextBox() == false)
			giPetManager::InitItemBackup();
#endif

		_PointedSquareIndex = CurSquareIndex;
	}
}

void UIInventoryControl::OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys)
{
	if (IsLocked() == true)
	{
		if (g_pPickedItem.GetItem() != nullptr)
		{
			g_pPickedItem.BackupPickedItem();
		}
		return;
	}

	if (_InventoryType != INVENTORY_TYPE::IT_INVENTORY ||
		_RepairMode == false)
	{
		if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_LEFT) == true)
		{
			if (_ToolTipType == TOOLTIP_TYPE_NPC_SHOP)
			{
				// Shop Type
				if (g_pPickedItem.GetItem() != nullptr)
				{
					ITEM *Item = g_pPickedItem.GetItem();

#if 0 // FIX
					if (CPCRoomPtSys::Instance().IsPCRoomPointShopMode())
					{
						g_pChatListBox->AddText(_T(""), GlobalText[2329], TYPE_ERROR_MESSAGE);
						g_pPickedItem.BackupPickedItem();
						return;
					}
#endif
					if (Item->HarmonyOption != 0)
					{
						g_pChatListBox->AddText(_T(""), GlobalText[2211], TYPE_ERROR_MESSAGE);
						g_pPickedItem.BackupPickedItem();
						return;
					}

					if (IsSellingBan(Item) == true)
					{
						g_pChatListBox->AddText(_T(""), GlobalText[668], TYPE_ERROR_MESSAGE);
						g_pPickedItem.BackupPickedItem();
						return;
					}

					if (IsHighValueItem(Item) == true)
					{
						g_pPickedItem.Hide();
						g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_HIGHVALUEWARNINGBOX);
						return;
					}

					if (g_pPickedItem.GetOwnerInventory() == g_pMyInventory->GetInventoryControl())
					{
						g_GameServerObject->SendRequestSell(INVENTORY_TYPE::IT_INVENTORY, Item->x, Item->y);
					}
					else if (g_pPickedItem.GetOwnerInventory() == nullptr)
					{
						g_GameServerObject->SendRequestSell(INVENTORY_TYPE::IT_EQUIPMENT, g_pPickedItem.GetSourceLinealPos(), 0);
					}
				}
				else
				{
					if (_PointedSquareIndex != -1 &&
						_ItemCheckBox[_PointedSquareIndex] > 1)
					{
						mu_uint32 ItemKey = _ItemCheckBox[_PointedSquareIndex];
						ITEM *Item = FindItemByKey(ItemKey);

						if (Item != nullptr)
						{
							if (g_pNpcShopInventory->GetShopType() == UIShopInventory::SHOPTYPE_PCPOINT)
							{
								g_pPickedItem.CreatePickedItem(this, Item);
								g_pPickedItem.Hide();
								g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_PCPOINTBUY);
							}
							else if (g_pNpcShopInventory->GetShopType() == UIShopInventory::SHOPTYPE_GAMBLE)
							{
								g_pPickedItem.CreatePickedItem(this, Item);
								g_pPickedItem.Hide();
								g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_GAMBLEBUY);
							}
							else
							{
								g_GameServerObject->SendRequestBuy(Item->x, Item->y);
							}
						}
					}
				}
			}
			else if (g_pPickedItem.GetItem() != nullptr)
			{
				if (_InventoryType == INVENTORY_TYPE::IT_MIX &&
					g_pMixInventory->GetMixState() != UIMixInventory::MIX_WAITING)
					return;

				ITEM *Item = g_pPickedItem.GetItem();

				if (g_pPickedItem.GetOwnerInventory() != nullptr)
				{
					mu_boolean Process = true;
					mu_uint8 SrcType = g_pPickedItem.GetOwnerInventory()->GetInventoryType();
					mu_int32 iSourceIndex = g_pPickedItem.GetSourceLinealPos();
					mu_int32 iTargetIndex = g_pPickedItem.GetTargetLinealPos(this);

					if (g_pPickedItem.GetOwnerInventory() == this)
					{
						if (iSourceIndex == iTargetIndex)
						{
							Process = false;
							g_pPickedItem.BackupPickedItem();
						}
					}

					if (Process)
					{
						if (iTargetIndex != -1)
						{
							ITEM *TargetItem = this->FindItem(iTargetIndex);

							if (TargetItem != nullptr &&
								TargetItem != Item)
							{
								iTargetIndex = TargetItem->y * this->GetNumberOfColumn() + TargetItem->x;
							}

							if (TargetItem != nullptr &&
								this->GetInventoryType() == INVENTORY_TYPE::IT_INVENTORY &&
								g_pPickedItem.GetOwnerInventory() ==  this &&
								(Item->Type == ITEM_POTION + 13
								|| Item->Type == ITEM_POTION + 14
								|| Item->Type == ITEM_POTION + 16
								|| Item->Type == ITEM_POTION + 42
								|| Item->Type == ITEM_POTION + 43
								|| Item->Type == ITEM_POTION + 44))
							{
								mu_int32 iType = TargetItem->Type;
								mu_int32 iLevel = TargetItem->Level;
								mu_int32 iDurability = TargetItem->Durability;

								mu_boolean bSuccess = true;

								if (iType > ITEM_WING + 6
									&& iType != ITEM_HELPER + 30
									&& !(iType >= ITEM_WING + 36 && iType <= ITEM_WING + 43)
									&& !(ITEM_WING + 130 <= iType && iType <= ITEM_WING + 134))
								{
									bSuccess = false;
								}

								if (iType == ITEM_BOW + 7
									|| iType == ITEM_BOW + 15)
								{
									bSuccess = false;
								}

								if ((Item->Type == ITEM_POTION + 13 && iLevel >= 6)
									|| (Item->Type == ITEM_POTION + 14 && iLevel >= 9))
								{
									bSuccess = false;
								}

								if (Item->Type == ITEM_POTION + 13 &&
									iType == ITEM_HELPER + 37 &&
									iDurability != 255)
								{
#if 0 // FIX
									SEASON3B::CFenrirRepairMsgBox* pMsgBox = nullptr;
									SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CFenrirRepairMsgBoxLayout), &pMsgBox);
									pMsgBox->SetSourceIndex(iSourceIndex);

									mu_int32 iIndex = TargetItem->x + TargetItem->y * m_pNewInventoryCtrl->GetNumberOfColumn();
									pMsgBox->SetTargetIndex(MAX_EQUIPMENT_INDEX + iIndex);

									pPickedItem->HidePickedItem();
#endif
									return;
								}

								if (Item->Type == ITEM_POTION + 42)
								{
									if (g_SocketItemMgr.IsSocketItem(TargetItem))
									{
										bSuccess = false;
									}
									else if (TargetItem->HarmonyOption != 0)
									{
										bSuccess = false;
									}
									else
									{
										StrengthenItem strengthitem = g_pUIJewelHarmonyinfo.GetItemType(TargetItem->Type);

										if (strengthitem == SI_None)
										{
											bSuccess = false;
										}
									}
								}

								if (Item->Type == ITEM_POTION + 43
									|| Item->Type == ITEM_POTION + 44)
								{
									if (g_SocketItemMgr.IsSocketItem(TargetItem))
									{
										bSuccess = false;
									}
									else if (TargetItem->HarmonyOption == 0)
									{
										bSuccess = false;
									}
								}

								if (bSuccess)
								{
									g_GameServerObject->SendUseItem(iSourceIndex, iTargetIndex);
									PlayBackground(SOUND_GET_ITEM01);
									return;
								}
							}

							if ((TargetItem != nullptr &&
								this->IsOverlayItem(Item, TargetItem) == true) ||
								CanMove(iTargetIndex, Item, Item->Key))
							{
								if (SrcType != INVENTORY_TYPE::IT_STORAGE ||
									g_pStorageInventory->GetCurrentAction() == UIStorageInventory::WHAUTO_NONE)
								{
									if (SrcType == INVENTORY_TYPE::IT_STORAGE &&
										this->GetInventoryType() != INVENTORY_TYPE::IT_STORAGE &&
										g_pStorageInventory->IsStorageLocked() == true &&
										g_pStorageInventory->IsStorageAllowed() == false)
									{
										g_pStorageInventory->SetAutoMoveItem(SrcType, Item->x, Item->y, this->GetInventoryType(), iTargetIndex % this->GetNumberOfColumn(), iTargetIndex / this->GetNumberOfColumn());
										g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_STORAGELOCKBOX);
										g_pStorageLockBox->SetRequestState(CL2GS_REQUEST_STORAGELOCK::REQUEST_ALLOW);
										g_pPickedItem.BackupPickedItem();
									}
									else
									{
										CL2GS_REQUEST_MOVEITEM RequestMoveItem;

										RequestMoveItem.SrcType = SrcType;
										RequestMoveItem.SrcX = Item->x;
										RequestMoveItem.SrcY = Item->y;

										RequestMoveItem.DestType = this->GetInventoryType();
										RequestMoveItem.DestX = iTargetIndex % this->GetNumberOfColumn();
										RequestMoveItem.DestY = iTargetIndex / this->GetNumberOfColumn();

										g_GameServerObject->Send(&RequestMoveItem);
									}
								}
							}
						}
					}
				}
				else if (Item->ex_src_type == ITEM_EX_SRC_EQUIPMENT)
				{
					mu_int32 iSourceIndex = g_pPickedItem.GetSourceLinealPos();
					mu_int32 iTargetIndex = g_pPickedItem.GetTargetLinealPos(this);
					if (iTargetIndex != -1 && CanMove(iTargetIndex, Item))
					{
						CL2GS_REQUEST_MOVEITEM RequestMoveItem;

						RequestMoveItem.SrcType = INVENTORY_TYPE::IT_EQUIPMENT;
						RequestMoveItem.SrcX = iSourceIndex;
						RequestMoveItem.SrcY = 0;

						RequestMoveItem.DestType = this->GetInventoryType();
						RequestMoveItem.DestX = iTargetIndex % this->GetNumberOfColumn();
						RequestMoveItem.DestY = iTargetIndex / this->GetNumberOfColumn();

						g_GameServerObject->Send(&RequestMoveItem);
					}
				}
			}
			else
			{
				if (this->GetInventoryType() != INVENTORY_TYPE::IT_INVENTORY ||
					this->IsRepairMode() == false)
				{
					if (_PointedSquareIndex != -1 &&
						_ItemCheckBox[_PointedSquareIndex] > 1)
					{
						mu_uint32 ItemKey = _ItemCheckBox[_PointedSquareIndex];
						ITEM *Item = FindItemByKey(ItemKey);

						if (Item != nullptr)
						{
							g_pPickedItem.CreatePickedItem(this, Item);
						}
					}
				}
				else
				{
					if (_PointedSquareIndex != -1 &&
						_ItemCheckBox[_PointedSquareIndex] > 1)
					{
						mu_uint32 ItemKey = _ItemCheckBox[_PointedSquareIndex];
						ITEM *Item = FindItemByKey(ItemKey);

						if (Item != nullptr)
						{
							if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_NPCSHOP) &&
								g_pNpcShopInventory->GetShopState() == UIShopInventory::SHOP_REPAIR)
							{
								g_GameServerObject->SendRequestRepair(INVENTORY_TYPE::IT_INVENTORY, Item->x, Item->y, true);
							}
							else if (CharacterAttribute->Level >= REPAIR_MINLEVEL)
							{
								g_GameServerObject->SendRequestRepair(INVENTORY_TYPE::IT_INVENTORY, Item->x, Item->y, false);
							}
						}
					}
				}				
			}
		}
		else if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_RIGHT) == true)
		{
			if (_ToolTipType == TOOLTIP_TYPE_NPC_SHOP)
			{
			}
			else if(g_pPickedItem.GetItem() == nullptr &&
				_PointedSquareIndex != -1 &&
				_ItemCheckBox[_PointedSquareIndex] > 1)
			{
				mu_uint32 ItemKey = _ItemCheckBox[_PointedSquareIndex];
				ITEM *Item = FindItemByKey(ItemKey);

				if (Item != nullptr)
				{
					if (_InventoryType == INVENTORY_TYPE::IT_STORAGE)
					{
						// Move item to inventory
						if (g_pMyInventory->FindEmptySlot(Item) != -1)
						{
							CL2GS_REQUEST_AUTOMOVEITEM RequestAutoMoveItem;

							RequestAutoMoveItem.SrcType = _InventoryType;
							RequestAutoMoveItem.SrcX = Item->x;
							RequestAutoMoveItem.SrcY = Item->y;
							RequestAutoMoveItem.DestType = INVENTORY_TYPE::IT_INVENTORY;

							g_GameServerObject->Send(&RequestAutoMoveItem);
						}
					}
					else if(_InventoryType == INVENTORY_TYPE::IT_INVENTORY)
					{
						if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_STORAGE) == true)
						{
							// Move item to Storage
							if (g_pStorageInventory->FindEmptySlot(Item) != -1)
							{
								CL2GS_REQUEST_AUTOMOVEITEM RequestAutoMoveItem;

								RequestAutoMoveItem.SrcType = _InventoryType;
								RequestAutoMoveItem.SrcX = Item->x;
								RequestAutoMoveItem.SrcY = Item->y;
								RequestAutoMoveItem.DestType = INVENTORY_TYPE::IT_STORAGE;

								g_GameServerObject->Send(&RequestAutoMoveItem);
							}
						}
						else if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_NPCSHOP) == false &&
							g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_TRADE) == false &&
							g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_DEVILSQUARE) == false &&
							g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_BLOODCASTLE) == false &&
							g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_MIXINVENTORY) == false)
						{
							if (Item->Type == ITEM_POTION + 10)
							{
								if (Teleport == false)
								{
									g_GameServerObject->SendUseItem(Item->y * _Column + Item->x);
								}
							}
							else if ((Item->Type >= ITEM_POTION + 0 && Item->Type <= ITEM_POTION + 9)
								|| (Item->Type == ITEM_POTION + 20 && Item->Level == 0)
								|| (Item->Type >= ITEM_POTION + 35 && Item->Type <= ITEM_POTION + 40)
								|| (Item->Type >= ITEM_POTION + 46 && Item->Type <= ITEM_POTION + 50)
								|| (Item->Type == ITEM_POTION + 11 && Item->Level == 14)
								|| (Item->Type >= ITEM_POTION + 70 && Item->Type <= ITEM_POTION + 71)
								|| (Item->Type >= ITEM_POTION + 72 && Item->Type <= ITEM_POTION + 77)
								|| Item->Type == ITEM_HELPER + 60
								|| Item->Type == ITEM_POTION + 94
								|| (Item->Type >= ITEM_POTION + 85 && Item->Type <= ITEM_POTION + 87)
								|| (Item->Type >= ITEM_POTION + 97 && Item->Type <= ITEM_POTION + 98)
								|| Item->Type == ITEM_HELPER + 81
								|| Item->Type == ITEM_HELPER + 82
								|| Item->Type == ITEM_POTION + 133
								)
							{
								g_GameServerObject->SendUseItem(Item->y * _Column + Item->x);
							}
							else if ((Item->Type >= ITEM_POTION + 78 && Item->Type <= ITEM_POTION + 82))
							{
								std::list<eBuffState> secretPotionbufflist;
								secretPotionbufflist.push_back(eBuff_SecretPotion1);
								secretPotionbufflist.push_back(eBuff_SecretPotion2);
								secretPotionbufflist.push_back(eBuff_SecretPotion3);
								secretPotionbufflist.push_back(eBuff_SecretPotion4);
								secretPotionbufflist.push_back(eBuff_SecretPotion5);

								if (g_isCharacterBufflist((&Hero->Object), secretPotionbufflist) == eBuffNone)
								{
									g_GameServerObject->SendUseItem(Item->y * _Column + Item->x);
								}
								else
								{
									g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
									g_pOkBox->ClearMessages();
									g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2530], MU::MakeARGB(255, 30, 0, 255));
									g_pOkBox->Configure();
								}
							}
							else if ((Item->Type >= ITEM_HELPER + 54 && Item->Type <= ITEM_HELPER + 57)
								|| (Item->Type == ITEM_HELPER + 58 && GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD))
							{
								mu_boolean result = true;
								mu_uint16 point[5] = { 0, };

								point[0] = CharacterAttribute->Strength + CharacterAttribute->AddStrength;
								point[1] = CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;
								point[2] = CharacterAttribute->Vitality + CharacterAttribute->AddVitality;
								point[3] = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;
								point[4] = CharacterAttribute->Charisma + CharacterAttribute->AddCharisma;

								mu_uint8 nStat[GAME::MAX_CLASS][5] =
								{
								{ 18, 18, 15, 30, 0 },
								{ 28, 20, 25, 10, 0 },
								{ 22, 25, 20, 15, 0 },
								{ 26, 26, 26, 26, 0 },
								{ 26, 20, 20, 15, 25 },
								{ 21, 21, 18, 23, 0 }
								};

								point[Item->Type - (ITEM_HELPER + 54)] -= nStat[GetBaseClass(Hero->Class)][Item->Type - (ITEM_HELPER + 54)];

								if (point[Item->Type - (ITEM_HELPER + 54)] < (Item->Durability * 10))
								{
									result = false;
								}

								if (result == false)
								{
#if 0 // FIX
									SetStandbyItemKey(Item->Key);
									CreateMessageBox(MSGBOX_LAYOUT_CLASS(CUsePartChargeFruitMsgBoxLayout));
#endif
								}
								else
								{
									g_GameServerObject->SendUseItem(Item->y * _Column + Item->x);
								}
							}
							else if (Item->Type == ITEM_HELPER + 58 && GetBaseClass(Hero->Class) != GAME::CLASS_DARK_LORD)
							{
								g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
								g_pOkBox->ClearMessages();
								g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[1905]);
								g_pOkBox->Configure();
							}
							else if (Item->Type == ITEM_HELPER + 29)
							{
								if (true == IsUnitedMarketPlace())
								{
									mu_text szOutputText[512];
									mu_sprintf_s(szOutputText, mu_countof(szOutputText), _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[3014], GlobalText[3015]);

									g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
									g_pOkBox->ClearMessages();
									g_pOkBox->AddMessage(FONT_NORMAL, szOutputText);
									g_pOkBox->Configure();
									return;
								}

								if (Hero->SafeZone == false)
								{
									g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
									g_pOkBox->ClearMessages();
									g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2330]);
									g_pOkBox->Configure();
								}
								else
								{
#if 0 // FIX
									SendRequestEventZoneOpenTime(4, Item->Level);
									SetStandbyItemKey(Item->Key);
#endif
								}
							}
							else if (Item->Type == ITEM_HELPER + 46)
							{
#if 0 // FIX
								mu_uint8 byPossibleLevel = CaculateFreeTicketLevel(FREETICKET_TYPE_DEVILSQUARE);
								SendRequestEventZoneOpenTime(1, byPossibleLevel);
#endif
							}
							else if (Item->Type == ITEM_HELPER + 47)
							{
#if 0 // FIX
								mu_uint8 byPossibleLevel = CaculateFreeTicketLevel(FREETICKET_TYPE_BLOODCASTLE);
								SendRequestEventZoneOpenTime(2, byPossibleLevel);
#endif
							}
							else if (Item->Type == ITEM_HELPER + 48)
							{
								if (Hero->SafeZone || InHellas())
								{
									g_pChatListBox->AddText(_T(""), GlobalText[1238], TYPE_ERROR_MESSAGE);
								}
								else
								{
									g_GameServerObject->SendUseItem(Item->y * _Column + Item->x);
								}
							}
							else if (Item->Type == ITEM_HELPER + 61)
							{
#if 0 // FIX
								mu_uint8 byPossibleLevel = CaculateFreeTicketLevel(FREETICKET_TYPE_CURSEDTEMPLE);
								SendRequestEventZoneOpenTime(5, byPossibleLevel);
#endif
							}
							else if (Item->Type == ITEM_HELPER + 121)
							{
								if (Hero->SafeZone == false)
								{
									g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
									g_pOkBox->ClearMessages();
									g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2330]);
									g_pOkBox->Configure();
								}
								else
								{
#if 0 // FIX
									SendRequestEventZoneOpenTime(4, Item->Level);
									SetStandbyItemKey(Item->Key);
#endif
								}
							}
							else if (Item->Type == ITEM_HELPER + 51)
							{
#if 0 // FIX
								SendRequestEventZoneOpenTime(5, Item->Level);
#endif
							}
							else if (Item->Type == ITEM_POTION + 19)
							{
#if 0 // FIX
								SendRequestEventZoneOpenTime(1, Item->Level);
#endif
							}
							else if (Item->Type == ITEM_HELPER + 18)
							{
								if (Item->Level == 0)
								{
									g_pChatListBox->AddText(_T(""), GlobalText[2089], TYPE_ERROR_MESSAGE);
								}
								else
								{
#if 0 // FIX
									SendRequestEventZoneOpenTime(2, Item->Level - 1);
#endif
								}
							}
							else if ((Item->Type >= ITEM_ETC + 0 && Item->Type < ITEM_ETC + MAX_ITEM_INDEX)
								|| (Item->Type >= ITEM_WING + 7 && Item->Type <= ITEM_WING + 14)
								|| (Item->Type >= ITEM_WING + 16 && Item->Type <= ITEM_WING + 19)
								|| (Item->Type == ITEM_WING + 20)
								|| (Item->Type >= ITEM_WING + 21 && Item->Type <= ITEM_WING + 24)
								|| (Item->Type == ITEM_WING + 35)
								|| (Item->Type == ITEM_WING + 44)
								|| (Item->Type == ITEM_WING + 47)
								|| (Item->Type == ITEM_WING + 46)
								|| (Item->Type == ITEM_WING + 45)
								|| (Item->Type == ITEM_WING + 48)
								|| (Item->Type == ITEM_ETC + 29)
								|| (Item->Type == ITEM_ETC + 28))
							{
								mu_boolean bReadBookGem = true;

								if (Item->Type == ITEM_ETC + 18
									|| Item->Type == ITEM_ETC + 28
									|| Item->Type == ITEM_WING + 45
									|| Item->Type == ITEM_WING + 46
									|| Item->Type == ITEM_WING + 44)
								{
#if 0 // FIX
									if (g_csQuest.getQuestState2(QUEST_CHANGE_UP_3) != QUEST_END)
										bReadBookGem = false;
#endif
								}

								if (Item->Type == ITEM_WING + 48)
								{
									mu_int32 Level = CharacterAttribute->Level;
									if (Level < 220)
										bReadBookGem = false;
								}

								if (bReadBookGem)
								{

									mu_uint16 Strength, Energy;
									Strength = CharacterAttribute->Strength + CharacterAttribute->AddStrength;
									Energy = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;

									if (CharacterAttribute->Level >= ItemAttribute[Item->Type].RequireLevel &&
										Energy >= Item->RequireEnergy &&
										Strength >= Item->RequireStrength)
									{
										g_GameServerObject->SendUseItem(Item->y * _Column + Item->x);
									}
								}
							}
							else if (Item->Type == ITEM_HELPER + 15)
							{
								mu_int32 Level = CharacterAttribute->Level;

								if (Level >= 10)
								{
									mu_boolean bEquipmentEmpty = true;
									for (mu_int32 i = 0; i < GAME::MAX_EQUIPMENT; ++i)
									{
										if (CharacterMachine->Equipment[i].Type != -1)
										{
											bEquipmentEmpty = false;
										}
									}
									mu_int32 Class = CharacterAttribute->Class;

									if (bEquipmentEmpty == true)
									{
										if (Item->Level == 4)
										{
											if (GetBaseClass(Class) != GAME::CLASS_DARK_LORD)
											{
												g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
												g_pOkBox->ClearMessages();
												g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[1905]);
												g_pOkBox->Configure();
												return;
											}
										}

#if 0 // FIX
										SetStandbyItemKey(Item->Key);
										CreateMessageBox(MSGBOX_LAYOUT_CLASS(CUseFruitMsgBoxLayout));
#endif
									}
									else
									{
										g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
										g_pOkBox->ClearMessages();
										g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[1909]);
										g_pOkBox->Configure();
									}
								}
								else
								{
									g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
									g_pOkBox->ClearMessages();
									g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[749]);
									g_pOkBox->Configure();
								}
							}
							else if (Item->Type == ITEM_HELPER + 11)
							{
								mu_boolean bUse = false;
								mu_int32  Level = Item->Level;

								switch (Level)
								{
								case 0:
									bUse = true;
									break;
								case 1:
									if (Hero->GuildStatus != GAME::G_MASTER)
										bUse = true;
									break;
								}

								if (bUse)
								{
									g_GameServerObject->SendUseItem(Item->y * _Column + Item->x);
								}
							}
							else if (Item->Type == ITEM_HELPER + 69)
							{
#if 0 // FIX
								if (g_PortalMgr.IsRevivePositionSaved())
								{
									if (g_PortalMgr.IsPortalUsable())
									{
										SetStandbyItemKey(Item->Key);
										CreateMessageBox(MSGBOX_LAYOUT_CLASS(CUseReviveCharmMsgBoxLayout));
									}
									else
									{
										g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
										g_pOkBox->ClearMessages();
										g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2608]);
										g_pOkBox->Configure();
									}
								}
#endif
							}
							else if (Item->Type == ITEM_HELPER + 70)
							{
#if 0 // FIX
								if (g_PortalMgr.IsPortalUsable())
								{
									if (Item->Durability == 2)
									{
										if (g_PortalMgr.IsPortalPositionSaved())
										{
											g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
											g_pOkBox->ClearMessages();
											g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2610]);
											g_pOkBox->Configure();
										}
										else
										{
											SetStandbyItemKey(Item->Key);
											CreateMessageBox(MSGBOX_LAYOUT_CLASS(CUsePortalCharmMsgBoxLayout));
										}
									}
									else if (Item->Durability == 1)
									{
										SetStandbyItemKey(Item->Key);
										CreateMessageBox(MSGBOX_LAYOUT_CLASS(CReturnPortalCharmMsgBoxLayout));
									}
								}
								else
								{
									g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
									g_pOkBox->ClearMessages();
									g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2608]);
									g_pOkBox->Configure();
								}
#endif
							}
							else if (Item->Type == ITEM_HELPER + 66)
							{
#if 0 // FIX
								SetStandbyItemKey(Item->Key);
								CreateMessageBox(MSGBOX_LAYOUT_CLASS(CUseSantaInvitationMsgBoxLayout));
#endif
							}
							else if (IsInvenItem(Item->Type) == true)
							{
								if (Item->Durability > 0)
								{
#if 0 // FIX
									mu_int32 iChangeInvenItemStatus = 0;
									(Item->Durability == 255) ? iChangeInvenItemStatus = 254 : iChangeInvenItemStatus = 255;

									SendRequestEquippingInventoryItem(MAX_EQUIPMENT_INDEX + iIndex, iChangeInvenItemStatus);
#endif
								}
							}
						}
					}
				}
			}
		}
	}
}

void UIInventoryControl::GetRect(ERect &box)
{
	box.left = _ViewX;
	box.right = _ViewX + _ViewWidth;
	box.top = _ViewY;
	box.bottom = _ViewY + _ViewHeight;
}

mu_boolean UIInventoryControl::GetSquarePosAtPoint(mu_int32 x, mu_int32 y, mu_int32 &ColumnX, mu_int32 &RowY)
{
	if (EMath::IsInside((mu_float)x, (mu_float)y, _ViewX, _ViewY, _ViewWidth, _ViewHeight) == false)
		return false;

	ColumnX = (x - (mu_int32)_ViewX) / INVENTORY_SQUARE_WIDTH;
	RowY = (y - (mu_int32)_ViewY) / INVENTORY_SQUARE_HEIGHT;

	return true;
}

mu_int32 UIInventoryControl::GetSquareIndexAtPoint(mu_int32 x, mu_int32 y)
{
	mu_int32 ColumnX, RowY;

	if (GetSquarePosAtPoint(x, y, ColumnX, RowY) == true)
		return RowY * _Column + ColumnX;

	return -1;
}

mu_boolean UIInventoryControl::CheckSlot(mu_int32 iLinealPos, mu_int32 cx, mu_int32 cy)
{
	if (iLinealPos % _Column + cx > _Column) return false;
	if (iLinealPos / _Column >= _Row) return false;
	if (iLinealPos / _Column + cy > _Row) return false;

	for (mu_int32 y = 0; y < cy; ++y)
	{
		mu_int32 yy = (y * _Column);

		for (mu_int32 x = 0; x < cx; ++x)
		{
			mu_int32 iIndex = iLinealPos + yy + x;

			if (_ItemCheckBox[iIndex] != 0)
			{
				return false;
			}
		}
	}

	return true;
}

mu_boolean UIInventoryControl::CheckSlot(mu_int32 iColumnX, mu_int32 iRowY, mu_int32 cx, mu_int32 cy)
{
	mu_int32 iIndex = iRowY*_Column + iColumnX;
	return CheckSlot(iIndex, cx, cy);
}

mu_boolean UIInventoryControl::CheckSlotIgnore(mu_int32 iLinealPos, mu_int32 cx, mu_int32 cy, mu_uint32 IgnoreKey)
{
	if (iLinealPos % _Column + cx > _Column) return false;
	if (iLinealPos / _Column >= _Row) return false;
	if (iLinealPos / _Column + cy > _Row) return false;

	for (mu_int32 y = 0; y < cy; ++y)
	{
		mu_int32 yy = (y * _Column);

		for (mu_int32 x = 0; x < cx; ++x)
		{
			mu_int32 iIndex = iLinealPos + yy + x;
			mu_uint32 Key = _ItemCheckBox[iIndex];

			if (Key != 0 &&
				Key != IgnoreKey)
			{
				return false;
			}
		}
	}

	return true;
}

mu_boolean UIInventoryControl::CheckPointInRect(mu_int32 x, mu_int32 y)
{
	ERect rcSquare;
	GetRect(rcSquare);

	if (x < rcSquare.left || x >= rcSquare.right || y < rcSquare.top || y >= rcSquare.bottom)
		return false;
	return true;
}

mu_boolean UIInventoryControl::CheckRectInRect(const ERect& rcBox)
{
	ERect rcSquare;
	GetRect(rcSquare);

	if (rcBox.left >= rcSquare.left && rcBox.right <= rcSquare.right
		&& rcBox.top >= rcSquare.top && rcBox.bottom <= rcSquare.bottom)
		return true;
	return false;
}

mu_boolean UIInventoryControl::CanMove(mu_int32 iLinealPos, ITEM* pItem)
{
	if (iLinealPos < 0 || iLinealPos >= _Column*_Row)
		return false;

	ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[pItem->Type];
	return CheckSlot(iLinealPos, pItemAttr->Width, pItemAttr->Height);
}

mu_boolean UIInventoryControl::CanMove(mu_int32 iColumnX, mu_int32 iRowY, ITEM* pItem)
{
	ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[pItem->Type];
	return CheckSlot(iColumnX, iRowY, pItemAttr->Width, pItemAttr->Height);
}

mu_boolean UIInventoryControl::CanMove(mu_int32 iLinealPos, ITEM* pItem, mu_uint32 IgnoreKey)
{
	if (iLinealPos < 0 || iLinealPos >= _Column*_Row)
		return false;

	ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[pItem->Type];
	return CheckSlotIgnore(iLinealPos, pItemAttr->Width, pItemAttr->Height, IgnoreKey);
}

mu_boolean UIInventoryControl::CanMoveToPt(mu_int32 x, mu_int32 y, ITEM* pItem)
{
	mu_int32 iColumnX, iRowY;
	if (GetSquarePosAtPoint(x, y, iColumnX, iRowY))
		return CanMove(iColumnX, iRowY, pItem);
	return false;
}

mu_boolean UIInventoryControl::AddItem(mu_int32 ColumnX, mu_int32 RowY, ITEM *Item)
{
	if (ColumnX < 0 ||
		RowY < 0 ||
		ColumnX >= _Column ||
		RowY >= _Row)
	{
		return false;
	}

	if (!CanMove(ColumnX, RowY, Item))
	{
		return false;
	}

	if (g_PlayItemSoundEnable == true)
	{
		PlayBackground(SOUND_GET_ITEM01);
	}

	ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[Item->Type];
	Item->x = ColumnX;
	Item->y = RowY;

	for (mu_int32 y = 0; y < pItemAttr->Height; ++y)
	{
		for (mu_int32 x = 0; x < pItemAttr->Width; ++x)
		{
			mu_int32 iCurIndex = (Item->y + y)*_Column + (Item->x + x);
			_ItemCheckBox[iCurIndex] = Item->Key;
		}
	}

	_Items.insert(std::make_pair(Item->Key, Item));
	return true;
}

ITEM *UIInventoryControl::FindItemByKey(mu_uint32 Key)
{
	auto it = _Items.find(Key);

	if (it == _Items.end())
		return nullptr;

	return it->second;
}

ITEM *UIInventoryControl::FindItem(mu_int32 LinealPos)
{
	if (LinealPos < 0 || LinealPos >= _Column * _Row)
		return nullptr;

	return FindItemByKey(_ItemCheckBox[LinealPos]);
}

ITEM *UIInventoryControl::FindItem(mu_int32 Column, mu_int32 Row)
{
	return FindItem(Row * _Column + Column);
}

mu_int32 UIInventoryControl::FindItemIndex(mu_int32 Type, mu_int32 Level)
{
	if (_Items.empty() == false)
	{
		for (mu_int32 y = 0; y < _Row; ++y)
		{
			for (mu_int32 x = 0; x < _Column; ++x)
			{
				mu_int32 itemKey = _ItemCheckBox[y * _Column + x];

				if (itemKey > 1)
				{
					ITEM *Item = FindItemByKey(itemKey);

					if (Item != nullptr &&
						Item->Type == Type &&
						(Level == -1 || Item->Level == Level))
					{
						return y * _Column + x;
					}
				}
			}
		}
	}

	return -1;
}

mu_int32 UIInventoryControl::FindItemReverseIndex(mu_int32 Type, mu_int32 Level)
{
	if (_Items.empty() == false)
	{
		for (mu_int32 y = _Row - 1; y >= 0; --y)
		{
			for (mu_int32 x = _Column - 1; x >= 0; --x)
			{
				mu_int32 itemKey = _ItemCheckBox[y * _Column + x];

				if (itemKey > 1)
				{
					ITEM *Item = FindItemByKey(itemKey);

					if (Item != nullptr &&
						Item->Type == Type &&
						(Level == -1 || Item->Level == Level))
					{
						return y * _Column + x;
					}
				}
			}
		}
	}

	return -1;
}

mu_int32 UIInventoryControl::FindEmptySlot(mu_int32 width, mu_int32 height)
{
	if (width < 1 ||
		height < 1 ||
		width > _Column ||
		height > _Row)
	{
		return -1;
	}

	mu_int32 maxx = _Column - width;
	mu_int32 maxy = _Row - height;

	for (mu_int32 y = 0; y <= maxy; ++y)
	{
		for (mu_int32 x = 0; x <= maxx; ++x)
		{
			if (CheckSlot(x, y, width, height) == true)
			{
				return y * _Column + _Row;
			}
		}
	}

	return -1;
}

void UIInventoryControl::RemoveItemByKey(mu_uint32 Key)
{
	auto it = _Items.find(Key);

	if (it != _Items.end())
	{
		ITEM *pItem = it->second;
		ITEM_ATTRIBUTE *pItemAttr = &ItemAttribute[pItem->Type];

		if (g_pPickedItem.GetItem() == pItem)
		{
			g_pPickedItem.BackupPickedItem();
		}
		else if (g_PlayItemSoundEnable == true)
		{
			PlayBackground(SOUND_GET_ITEM01);
		}

		for (mu_int32 y = 0; y < pItemAttr->Height; ++y)
		{
			for (mu_int32 x = 0; x < pItemAttr->Width; ++x)
			{
				mu_int32 iCurIndex = (pItem->y + y)*_Column + (pItem->x + x);
				_ItemCheckBox[iCurIndex] = 0;
			}
		}

		_Items.erase(it);

		DestroyItem(pItem);
	}
}

void UIInventoryControl::RemoveItem(ITEM *pItem)
{
	if (g_pPickedItem.GetItem() == pItem)
	{
		g_pPickedItem.BackupPickedItem();
	}
	else if (g_PlayItemSoundEnable == true)
	{
		PlayBackground(SOUND_GET_ITEM01);
	}

	for (auto it = _Items.begin(); it != _Items.end(); ++it)
	{
		if (it->second == pItem)
		{
			ITEM_ATTRIBUTE *pItemAttr = &ItemAttribute[pItem->Type];

			for (mu_int32 y = 0; y < pItemAttr->Height; ++y)
			{
				for (mu_int32 x = 0; x < pItemAttr->Width; ++x)
				{
					mu_int32 iCurIndex = (pItem->y + y)*_Column + (pItem->x + x);
					_ItemCheckBox[iCurIndex] = 0;
				}
			}

			_Items.erase(it);

			DestroyItem(pItem);

			break;
		}
	}
}

void UIInventoryControl::RemoveAllItems()
{
	mu_zeromem(_ItemCheckBox, sizeof(mu_uint32) * _Column * _Row);

	if (g_pPickedItem.GetOwnerInventory() == this)
	{
		g_pPickedItem.BackupPickedItem();
	}
	else if (g_PlayItemSoundEnable == true)
	{
		PlayBackground(SOUND_GET_ITEM01);
	}

	for (auto it = _Items.begin(); it != _Items.end(); it = _Items.erase(it))
	{
		DestroyItem(it->second);
	}
}

void UIInventoryControl::SetItemColorState(ITEM* pItem)
{
	if (pItem == nullptr)
	{
		return;
	}

	if (pItem->byColorState == ITEM_COLOR_TRADE_WARNING)
	{
		return;
	}

	ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[pItem->Type];
	mu_int32 iLevel = pItem->Level;
	mu_int32 iMaxDurability = calcMaxDurability(pItem, pItemAttr, iLevel);

	if (pItem->Durability <= 0)
	{
		pItem->byColorState = ITEM_COLOR_DURABILITY_100;
	}
	else if (pItem->Durability <= (iMaxDurability*0.2f))
	{
		pItem->byColorState = ITEM_COLOR_DURABILITY_80;
	}
	else if (pItem->Durability <= (iMaxDurability*0.3f))
	{
		pItem->byColorState = ITEM_COLOR_DURABILITY_70;
	}
	else if (pItem->Durability <= (iMaxDurability*0.5f))
	{
		pItem->byColorState = ITEM_COLOR_DURABILITY_50;
	}
	else
	{
		pItem->byColorState = ITEM_COLOR_NORMAL;
	}
}

mu_boolean UIInventoryControl::CanChangeItemColorState(ITEM* pItem)
{
	if (pItem == nullptr)
	{
		return false;
	}

	if (pItem->Type < ITEM_WING)
	{
		return true;
	}

	if (pItem->Type == ITEM_BOW + 7 || pItem->Type == ITEM_BOW + 15)
	{
		return false;
	}

	if (pItem->Type == ITEM_HELPER + 20 && (pItem->Level == 1 || pItem->Level == 2))
	{
		return false;
	}

	if ((pItem->Type >= ITEM_HELPER + 8 && pItem->Type <= ITEM_HELPER + 9)
		|| pItem->Type == ITEM_HELPER + 10
		|| (pItem->Type >= ITEM_HELPER + 12 && pItem->Type <= ITEM_HELPER + 13)
		|| pItem->Type == ITEM_HELPER + 20
		|| (pItem->Type >= ITEM_HELPER + 21 && pItem->Type <= ITEM_HELPER + 28)
		|| (pItem->Type >= ITEM_HELPER + 38 && pItem->Type <= ITEM_HELPER + 42)
		|| pItem->Type == ITEM_HELPER + 76
		|| pItem->Type == ITEM_HELPER + 122
		|| pItem->Type == ITEM_HELPER + 80
		|| pItem->Type == ITEM_HELPER + 64
		|| pItem->Type == ITEM_HELPER + 65
		|| pItem->Type == ITEM_HELPER + 123
		|| pItem->Type == ITEM_HELPER + 107
		|| pItem->Type == ITEM_HELPER + 109
		|| pItem->Type == ITEM_HELPER + 110
		|| pItem->Type == ITEM_HELPER + 111
		|| pItem->Type == ITEM_HELPER + 112
		|| pItem->Type == ITEM_HELPER + 113
		|| pItem->Type == ITEM_HELPER + 114
		|| pItem->Type == ITEM_HELPER + 115
		|| IsInvenItem(pItem->Type)
		)
	{
		return true;
	}

	if ((pItem->Type >= ITEM_HELPER && pItem->Type <= ITEM_HELPER + 5)
		|| pItem->Type == ITEM_HELPER + 37
		|| pItem->Type == ITEM_HELPER + 106
		)
	{
		return true;
	}

	if (IsWingItem(pItem) == true)
	{
		return true;
	}

	return false;
}

void UIInventoryControl::SetRepairMode(mu_boolean bRepair)
{
	_RepairMode = bRepair;

	if (_RepairMode == true)
	{
		SetToolTipType(TOOLTIP_TYPE_REPAIR);
	}
	else
	{
		SetToolTipType(TOOLTIP_TYPE_INVENTORY);
	}
}

mu_boolean UIInventoryControl::IsRepairMode()
{
	return _RepairMode;
}

void UIInventoryControl::SetToolTipType(TOOLTIP_TYPE ToolTipType)
{
	_ToolTipType = ToolTipType;
}

mu_boolean UIInventoryControl::IsOverlayItem(ITEM *pSourceItem, ITEM *pTargetItem)
{
	if (pSourceItem == nullptr || pTargetItem == nullptr)
	{
		return false;
	}

	mu_int32 iSrcType = pSourceItem->Type;
	mu_int32 iTarType = pTargetItem->Type;
	mu_int32 iSrcLevel = pSourceItem->Level;
	mu_int32 iTarLevel = pTargetItem->Level;
	mu_int32 iSrcDurability = pSourceItem->Durability;
	mu_int32 iTarDurability = pTargetItem->Durability;

	if (iSrcType == iTarType)
	{
		if (iSrcType == ITEM_POTION + 7 && iTarType == ITEM_POTION + 7
			&& (iSrcDurability < 250 && iTarDurability < 250))
		{
			return true;
		}
		else if ((iSrcType >= ITEM_POTION && iSrcType <= ITEM_POTION + 8 && iSrcType != ITEM_POTION + 7)
			&& (iTarType >= ITEM_POTION && iTarType <= ITEM_POTION + 8 && iTarType != ITEM_POTION + 7)
			&& (iSrcDurability < 3 && iTarDurability < 3))
		{
			return true;
		}

		if ((iSrcType >= ITEM_POTION + 38 && iSrcType <= ITEM_POTION + 40)
			&& (iTarType >= ITEM_POTION + 38 && iTarType <= ITEM_POTION + 40)
			&& (iSrcDurability < 3 && iTarDurability < 3))
		{
			return true;
		}

		if ((iSrcType == ITEM_BOW + 7 && iTarType == ITEM_BOW + 7)
			&& (iSrcLevel == iTarLevel))
		{
			return true;
		}

		if ((iSrcType == ITEM_BOW + 15 && iTarType == ITEM_BOW + 15)
			&& (iSrcLevel == iTarLevel))
		{
			return true;
		}

		if (iSrcType == ITEM_POTION + 29 && iTarType == ITEM_POTION + 29)
		{
			return true;
		}

		if ((iSrcType >= ITEM_HELPER + 32 && iSrcType <= ITEM_HELPER + 34)
			&& (iTarType >= ITEM_HELPER + 32 && iTarType <= ITEM_HELPER + 34))
		{
			return true;
		}

		if ((iSrcType >= ITEM_POTION + 46 && iSrcType <= ITEM_POTION + 50)
			&& (iTarType >= ITEM_POTION + 46 && iTarType <= ITEM_POTION + 50)
			&& (iSrcDurability < 3 && iTarDurability < 3))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 70 && iTarType == ITEM_POTION + 70
			&& (iSrcDurability < 50 && iTarDurability < 50))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 71 && iTarType == ITEM_POTION + 71
			&& (iSrcDurability < 50 && iTarDurability < 50))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 78 && iTarType == ITEM_POTION + 78
			&& (iSrcDurability < 3 && iTarDurability < 3))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 79 && iTarType == ITEM_POTION + 79
			&& (iSrcDurability < 3 && iTarDurability < 3))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 80 && iTarType == ITEM_POTION + 80
			&& (iSrcDurability < 3 && iTarDurability < 3))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 81 && iTarType == ITEM_POTION + 81
			&& (iSrcDurability < 3 && iTarDurability < 3))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 82 && iTarType == ITEM_POTION + 82
			&& (iSrcDurability < 3 && iTarDurability < 3))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 94 && iTarType == ITEM_POTION + 94
			&& (iSrcDurability < 50 && iTarDurability < 50))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 85 && iTarType == ITEM_POTION + 85
			&& (iSrcDurability < 3 && iTarDurability < 3))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 86 && iTarType == ITEM_POTION + 86
			&& (iSrcDurability < 3 && iTarDurability < 3))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 87 && iTarType == ITEM_POTION + 87
			&& (iSrcDurability < 3 && iTarDurability < 3))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 88 && iTarType == ITEM_POTION + 88
			&& (iSrcDurability < 10 && iTarDurability < 10))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 89 && iTarType == ITEM_POTION + 89
			&& (iSrcDurability < 30 && iTarDurability < 30))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 90 && iTarType == ITEM_POTION + 90
			&& (iSrcDurability < 50 && iTarDurability < 50))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 100 && (iSrcDurability < 255 && iTarDurability < 255))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 110 && iTarType == ITEM_POTION + 110)
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 101 && iTarType == ITEM_POTION + 101
			&& (iSrcDurability < 5 && iTarDurability < 5))
		{
			return true;
		}
		else if (iSrcType == ITEM_POTION + 133
			&& (iSrcDurability < 50 && iTarDurability < 50))
		{
			return true;
		}
	}

	return false;
}

mu_boolean UIInventoryControl::CanUpgradeItem(ITEM *pSourceItem, ITEM *pTargetItem)
{
	mu_int32 iTargetLevel = pTargetItem->Level;

	if (((pTargetItem->Type >= ITEM_SWORD && pTargetItem->Type < ITEM_WING)
		&& (pTargetItem->Type != ITEM_BOW + 7)
		&& (pTargetItem->Type != ITEM_BOW + 15))
		|| (pTargetItem->Type >= ITEM_WING && pTargetItem->Type <= ITEM_WING + 6)
		|| (pTargetItem->Type >= ITEM_WING + 36 && pTargetItem->Type <= ITEM_WING + 43))
	{
		if ((pSourceItem->Type == ITEM_POTION + 13)
			&& (iTargetLevel >= 0 && iTargetLevel <= 5))
		{
			return true;
		}
		else if ((pSourceItem->Type == ITEM_POTION + 14)
			&& (iTargetLevel >= 0 && iTargetLevel <= 8))
		{
			return true;
		}
		else if (pSourceItem->Type == ITEM_POTION + 16 &&
			pTargetItem->Option < 7)
		{
			return true;
		}
	}

	return false;
}