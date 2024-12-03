#ifndef __MU_TEXTURE_ATLAS_H__
#define __MU_TEXTURE_ATLAS_H__

#pragma once

struct MUTextureAtlasSlot : public MUIntrusiveLinkedList<MUTextureAtlasSlot>
{
	mu_uint32 X;
	mu_uint32 Y;
	mu_uint32 Width;
	mu_uint32 Height;
	mu_uint8 Padding;

	MUTextureAtlasSlot(mu_uint32 InX, mu_uint32 InY, mu_uint32 InWidth, mu_uint32 InHeight, mu_uint8 InPadding);
};

class MUTextureAtlas
{
public:
	enum AtlasPaddingStyle
	{
		NoPadding,
		DilateBorder,
		PadWithZero,
	};

public:
	MUTextureAtlas(mu_uint32 InWidth, mu_uint32 InHeight, mu_uint32 InBytesPerPixel, AtlasPaddingStyle InPaddingStyle);
	~MUTextureAtlas();

	void Empty();

	const MUTextureAtlasSlot* AddTexture(mu_uint32 TextureWidth, mu_uint32 TextureHeight, const mu_uint8 *Data, mu_uint32 DataSize, mu_uint32 Pitch);

	mu_uint32 GetWidth() const { return AtlasWidth; }
	mu_uint32 GetHeight() const { return AtlasHeight; }

	void MarkTextureDirty();

protected:
	const MUTextureAtlasSlot* FindSlotForTexture(mu_uint32 InWidth, mu_uint32 InHeight);

	void InitAtlasData();

	struct MUCopyRowData
	{
		const mu_uint8* SrcData;
		mu_uint8* DestData;
		mu_uint32 SrcRow;
		mu_uint32 DestRow;
		mu_uint32 RowWidth;
		mu_uint32 SrcTextureWidth;
		mu_uint32 DestTextureWidth;
		mu_uint32 SrcTexturePitch;
	};

	void CopyRow(const MUCopyRowData& CopyRowData);

	void ZeroRow(const MUCopyRowData& CopyRowData);

	void CopyDataIntoSlot(const MUTextureAtlasSlot* SlotToCopyTo, const mu_uint8 *Data, mu_uint32 DataSize, mu_uint32 Pitch);

private:
	FORCEINLINE mu_int32 GetPaddingAmount() const
	{
		return (PaddingStyle == NoPadding) ? 0 : 1;
	}

protected:
	friend class MUFont;
	EStagingBufferBackend AtlasData;
	MUTextureAtlasSlot* AtlasUsedSlots;
	MUTextureAtlasSlot* AtlasEmptySlots;
	mu_uint32 AtlasWidth;
	mu_uint32 AtlasHeight;
	mu_uint32 BytesPerPixel;
	AtlasPaddingStyle PaddingStyle;

	mu_boolean bNeedsUpdate;
};

#endif