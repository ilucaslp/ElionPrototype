#include "stdafx.h"
#include "mu_guildsprites.h"

MUGuildSprite g_GuildSprites[MAX_GUILDSPRITES];
mu_atomic_uint32_t g_GuildSpritesCount(0);
mu_uint32 g_GuildSpritesRenderCount;

const mu_uint32 MarkColorNonBlend[MAX_GUILD_COLORS] =
{
	MU::MakeARGB(0, 0, 0, 128),
	MU::MakeARGB(0, 0, 0, 255),
	MU::MakeARGB(128, 128, 128, 255),
	MU::MakeARGB(255, 255, 255, 255),
	MU::MakeARGB(255, 0, 0, 255),
	MU::MakeARGB(255, 128, 0, 255),
	MU::MakeARGB(255, 255, 0, 255),
	MU::MakeARGB(128, 255, 0, 255),
	MU::MakeARGB(0, 255, 0, 255),
	MU::MakeARGB(0, 255, 128, 255),
	MU::MakeARGB(0, 255, 255, 255),
	MU::MakeARGB(0, 128, 255, 255),
	MU::MakeARGB(0, 0, 255, 255),
	MU::MakeARGB(128, 0, 255, 255),
	MU::MakeARGB(255, 0, 255, 255),
	MU::MakeARGB(255, 0, 128, 255),
};

const mu_uint32 MarkColorBlend[MAX_GUILD_COLORS] =
{
	MU::MakeARGB(0, 0, 0, 0),
	MU::MakeARGB(0, 0, 0, 255),
	MU::MakeARGB(128, 128, 128, 255),
	MU::MakeARGB(255, 255, 255, 255),
	MU::MakeARGB(255, 0, 0, 255),
	MU::MakeARGB(255, 128, 0, 255),
	MU::MakeARGB(255, 255, 0, 255),
	MU::MakeARGB(128, 255, 0, 255),
	MU::MakeARGB(0, 255, 0, 255),
	MU::MakeARGB(0, 255, 128, 255),
	MU::MakeARGB(0, 255, 255, 255),
	MU::MakeARGB(0, 128, 255, 255),
	MU::MakeARGB(0, 0, 255, 255),
	MU::MakeARGB(128, 0, 255, 255),
	MU::MakeARGB(255, 0, 255, 255),
	MU::MakeARGB(255, 0, 128, 255),
};

void RenderGuild(OBJECT *o, mu_int32 Type, mu_uint32 TextureIndex)
{
	mu_uint32 i = g_GuildSpritesCount.fetch_add(1);

	if (i >= MAX_GUILDSPRITES)
	{
		return;
	}

	MUGuildSprite *sprite = &g_GuildSprites[i];

	sprite->Owner = o;
	sprite->Type = TextureIndex;
	sprite->ItemType = Type;
}

void CreateGuildMark(const mu_uint8 *MarkBuffer, mu_boolean Blend, MUTexture *MarkTexture, EStagingBufferBackend *StagingBuffer)
{
#if 0 // FIX
	const mu_uint32 *colorBuffer = Blend == true ? MarkColorBlend : MarkColorNonBlend;

	mu_uint32 guildBuffer[GUILDMARK_HEIGHT][GUILDMARK_WIDTH];

	for (mu_uint32 h = 0; h < GUILDMARK_HEIGHT; ++h)
	{
		for (mu_uint32 w = 0; w < GUILDMARK_WIDTH; ++w)
		{
			mu_uint32 colorIndex = MarkBuffer[h * GUILDMARK_WIDTH + w];
			if (colorIndex > MAX_GUILD_COLORS - 1)
			{
				colorIndex = MAX_GUILD_COLORS - 1;
			}

			guildBuffer[h][w] = colorBuffer[colorIndex];
		}
	}

	mu_memcpy(StagingBuffer->BufferInternal.MappedMemory, guildBuffer, sizeof(guildBuffer));
	StagingBuffer->Flush(0, sizeof(guildBuffer));
	MarkTexture->Flush(StagingBuffer, 0, 0, sizeof(guildBuffer), ETextureFormat::eRGBA8_Unorm);
#endif
}