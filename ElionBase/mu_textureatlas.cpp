#include "stdafx.h"
#include "mu_textureatlas.h"

MUTextureAtlasSlot::MUTextureAtlasSlot(mu_uint32 InX, mu_uint32 InY, mu_uint32 InWidth, mu_uint32 InHeight, mu_uint8 InPadding)
	: MUIntrusiveLinkedList<MUTextureAtlasSlot>()
	, X(InX)
	, Y(InY)
	, Width(InWidth)
	, Height(InHeight)
	, Padding(InPadding)
{
}

MUTextureAtlas::MUTextureAtlas(mu_uint32 InWidth, mu_uint32 InHeight, mu_uint32 InBytesPerPixel, AtlasPaddingStyle InPaddingStyle)
	: AtlasUsedSlots(nullptr)
	, AtlasEmptySlots(nullptr)
	, AtlasWidth(InWidth)
	, AtlasHeight(InHeight)
	, BytesPerPixel(InBytesPerPixel)
	, PaddingStyle(InPaddingStyle)
	, bNeedsUpdate(false)
{
	InitAtlasData();
}

MUTextureAtlas::~MUTextureAtlas()
{
	Empty();
}

void MUTextureAtlas::Empty()
{
	std::vector<MUTextureAtlasSlot*> DeleteSlots;

	for (MUTextureAtlasSlot::Iterator SlotIt(AtlasUsedSlots); SlotIt.IsValid(); SlotIt.Next())
	{
		MUTextureAtlasSlot& CurSlot = *SlotIt;
		DeleteSlots.push_back(&CurSlot);
	}

	for (MUTextureAtlasSlot::Iterator SlotIt(AtlasEmptySlots); SlotIt.IsValid(); SlotIt.Next())
	{
		MUTextureAtlasSlot& CurSlot = *SlotIt;
		DeleteSlots.push_back(&CurSlot);
	}

	AtlasUsedSlots = nullptr;
	AtlasEmptySlots = nullptr;

	for (MUTextureAtlasSlot* CurSlot : DeleteSlots)
	{
		delete CurSlot;
	}

	DeleteSlots.clear();

	AtlasData.Deallocate();
}

const MUTextureAtlasSlot* MUTextureAtlas::AddTexture(mu_uint32 TextureWidth, mu_uint32 TextureHeight, const mu_uint8 *Data, mu_uint32 DataSize, mu_uint32 Pitch)
{
	// Find a spot for the character in the texture
	const MUTextureAtlasSlot* NewSlot = FindSlotForTexture(TextureWidth, TextureHeight);

	// handle cases like space, where the size of the glyph is zero. The copy data code doesn't handle zero sized source data with a padding
	// so make sure to skip this call.
	if (NewSlot && TextureWidth > 0 && TextureHeight > 0)
	{
		CopyDataIntoSlot(NewSlot, Data, DataSize, Pitch);
		MarkTextureDirty();
	}

	return NewSlot;
}

void MUTextureAtlas::MarkTextureDirty()
{
	bNeedsUpdate = true;
}

void MUTextureAtlas::InitAtlasData()
{
	MUTextureAtlasSlot* RootSlot = new MUTextureAtlasSlot(0, 0, AtlasWidth, AtlasHeight, GetPaddingAmount());

	RootSlot->LinkHead(AtlasEmptySlots);

	AtlasData.Deallocate();
	AtlasData.Allocate(sizeof(mu_uint8) * AtlasWidth * AtlasHeight * BytesPerPixel, true);
	mu_zeromem(AtlasData.BufferInternal.MappedMemory, AtlasWidth * AtlasHeight * BytesPerPixel);
}

void MUTextureAtlas::CopyRow( const MUCopyRowData& CopyRowData )
{
	const mu_uint8* Data = CopyRowData.SrcData;
	mu_uint8* Start = CopyRowData.DestData;
	const mu_uint32 SourceWidth = CopyRowData.SrcTextureWidth;
	const mu_uint32 DestWidth = CopyRowData.DestTextureWidth;
	const mu_uint32 SrcRow = CopyRowData.SrcRow;
	const mu_uint32 DestRow = CopyRowData.DestRow;
	const mu_uint32 SourcePitch = CopyRowData.SrcTexturePitch;
	// this can only be one or zero
	const mu_uint32 Padding = GetPaddingAmount();

	const mu_uint8* SourceDataAddr = &Data[(SrcRow * SourcePitch) * BytesPerPixel];
	mu_uint8* DestDataAddr = &Start[(DestRow * DestWidth + Padding) * BytesPerPixel];
	mu_memcpy(DestDataAddr, SourceDataAddr, SourceWidth * BytesPerPixel);

	if (Padding > 0)
	{
		mu_uint8* DestPaddingPixelLeft = &Start[(DestRow * DestWidth) * BytesPerPixel];
		mu_uint8* DestPaddingPixelRight = DestPaddingPixelLeft + ((CopyRowData.RowWidth - 1) * BytesPerPixel);
		if (PaddingStyle == AtlasPaddingStyle::DilateBorder)
		{
			const mu_uint8* FirstPixel = SourceDataAddr; 
			const mu_uint8* LastPixel = SourceDataAddr + ((SourceWidth - 1) * BytesPerPixel);
			mu_memcpy(DestPaddingPixelLeft, FirstPixel, BytesPerPixel);
			mu_memcpy(DestPaddingPixelRight, LastPixel, BytesPerPixel);
		}
		else
		{
			mu_zeromem(DestPaddingPixelLeft, BytesPerPixel);
			mu_zeromem(DestPaddingPixelRight, BytesPerPixel);
		}
	} 
}

void MUTextureAtlas::ZeroRow( const MUCopyRowData& CopyRowData )
{
	const mu_uint32 SourceWidth = CopyRowData.SrcTextureWidth;
	const mu_uint32 DestWidth = CopyRowData.DestTextureWidth;
	const mu_uint32 DestRow = CopyRowData.DestRow;

	mu_uint8* DestDataAddr = &CopyRowData.DestData[DestRow * DestWidth * BytesPerPixel];
	mu_zeromem(DestDataAddr, CopyRowData.RowWidth * BytesPerPixel);
}

void MUTextureAtlas::CopyDataIntoSlot(const MUTextureAtlasSlot* SlotToCopyTo, const mu_uint8 *Data, mu_uint32 DataSize, mu_uint32 Pitch)
{
	// Copy pixel data to the texture
	mu_uint8* Start = &reinterpret_cast<mu_uint8*>(AtlasData.BufferInternal.MappedMemory)[SlotToCopyTo->Y*AtlasWidth*BytesPerPixel + SlotToCopyTo->X*BytesPerPixel];

	// Account for same padding on each sides
	const mu_uint32 Padding = GetPaddingAmount();
	const mu_uint32 AllPadding = Padding * 2;

	// The width of the source texture without padding (actual width)
	const mu_uint32 SourceWidth = SlotToCopyTo->Width - AllPadding;
	const mu_uint32 SourceHeight = SlotToCopyTo->Height - AllPadding;

	MUCopyRowData CopyRowData;
	CopyRowData.DestData = Start;
	CopyRowData.SrcData = Data;
	CopyRowData.DestTextureWidth = AtlasWidth;
	CopyRowData.SrcTextureWidth = SourceWidth;
	CopyRowData.RowWidth = SlotToCopyTo->Width;
	CopyRowData.SrcTexturePitch = Pitch;

	// Apply the padding for bilinear filtering. 
	// Not used if no padding (assumes sampling outside boundaries of the sub texture is not possible)
	if (Padding > 0)
	{
		// Copy first color row into padding.  
		CopyRowData.SrcRow = 0;
		CopyRowData.DestRow = 0;

		if (PaddingStyle == AtlasPaddingStyle::DilateBorder)
		{
			CopyRow(CopyRowData);
		}
		else
		{
			ZeroRow(CopyRowData);
		}
	}

	// Copy each row of the texture
	for (mu_uint32 Row = Padding; Row < SlotToCopyTo->Height - Padding; ++Row)
	{
		CopyRowData.SrcRow = Row - Padding;
		CopyRowData.DestRow = Row;

		CopyRow(CopyRowData);
	}

	if (Padding > 0)
	{
		// Copy last color row into padding row for bilinear filtering
		CopyRowData.SrcRow = SourceHeight - 1;
		CopyRowData.DestRow = SlotToCopyTo->Height - Padding;

		if (PaddingStyle == AtlasPaddingStyle::DilateBorder)
		{
			CopyRow(CopyRowData);
		}
		else
		{
			ZeroRow(CopyRowData);
		}
	}
}

const MUTextureAtlasSlot* MUTextureAtlas::FindSlotForTexture(mu_uint32 InWidth, mu_uint32 InHeight)
{
	MUTextureAtlasSlot* ReturnVal = nullptr;

	// Account for padding on both sides
	const mu_uint32 Padding = GetPaddingAmount();
	const mu_uint32 TotalPadding = Padding * 2;
	const mu_uint32 PaddedWidth = InWidth + TotalPadding;
	const mu_uint32 PaddedHeight = InHeight + TotalPadding;

	// Previously, slots were stored as a binary tree - this has been replaced with a linked-list of slots on the edge of the tree
	// (slots on the edge of the tree represent empty slots); this iterates empty slots in same order as a binary depth-first-search,
	// except much faster.
	for (MUTextureAtlasSlot::Iterator SlotIt(AtlasEmptySlots); SlotIt.IsValid(); ++SlotIt)
	{
		MUTextureAtlasSlot& CurSlot = *SlotIt;

		if (PaddedWidth <= CurSlot.Width && PaddedHeight <= CurSlot.Height)
		{
			ReturnVal = &CurSlot;
			break;
		}
	}


	if (ReturnVal != nullptr)
	{
		// The width and height of the new child node
		const mu_uint32 RemainingWidth =  std::max<mu_uint32>(0, ReturnVal->Width - PaddedWidth);
		const mu_uint32 RemainingHeight = std::max<mu_uint32>(0, ReturnVal->Height - PaddedHeight);

		// New slots must have a minimum width/height, to avoid excessive slots i.e. excessive memory usage and iteration.
		// No glyphs seem to use slots this small, and cutting these slots out improves performance/memory-usage a fair bit
		const mu_uint32 MinSlotDim = 2;

		// Split the remaining area around this slot into two children.
		if (RemainingHeight >= MinSlotDim || RemainingWidth >= MinSlotDim)
		{
			MUTextureAtlasSlot* LeftSlot = nullptr;
			MUTextureAtlasSlot* RightSlot = nullptr;

			if (RemainingHeight <= RemainingWidth)
			{
				// Split vertically
				// - - - - - - - - -
				// |       |       |
				// |  Slot |       |
				// |       |       |
				// | - - - | Right |
				// |       |       |
				// |  Left |       |
				// |       |       |
				// - - - - - - - - -
				LeftSlot = new MUTextureAtlasSlot(ReturnVal->X, ReturnVal->Y + PaddedHeight, PaddedWidth, RemainingHeight, Padding);
				RightSlot = new MUTextureAtlasSlot(ReturnVal->X + PaddedWidth, ReturnVal->Y, RemainingWidth, ReturnVal->Height, Padding);
			}
			else
			{
				// Split horizontally
				// - - - - - - - - -
				// |       |       |
				// |  Slot | Left  |
				// |       |       |
				// | - - - - - - - |
				// |               |
				// |     Right     |
				// |               |
				// - - - - - - - - -
				LeftSlot = new MUTextureAtlasSlot(ReturnVal->X + PaddedWidth, ReturnVal->Y, RemainingWidth, PaddedHeight, Padding);
				RightSlot = new MUTextureAtlasSlot(ReturnVal->X, ReturnVal->Y + PaddedHeight, ReturnVal->Width, RemainingHeight, Padding);
			}

			// Replace the old slot within AtlasEmptySlots, with the new Left and Right slot, then add the old slot to AtlasUsedSlots
			LeftSlot->LinkReplace(ReturnVal);
			RightSlot->LinkAfter(LeftSlot);

			ReturnVal->LinkHead(AtlasUsedSlots);
		}
		else
		{
			// Remove the old slot from AtlasEmptySlots, into AtlasUsedSlots
			ReturnVal->Unlink();
			ReturnVal->LinkHead(AtlasUsedSlots);
		}


		// Shrink the slot to the remaining area.
		ReturnVal->Width = PaddedWidth;
		ReturnVal->Height = PaddedHeight;
	}

	return ReturnVal;
}