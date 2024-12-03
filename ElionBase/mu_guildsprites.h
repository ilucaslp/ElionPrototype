#ifndef __MU_GUILDSPRITES_H__
#define __MU_GUILDSPRITES_H__

#pragma once

#define GUILD_RENDERTYPE BLEND::TXB_ALPHATEST_BEGIN

class MUGuildSprite
{
public:
	OBJECT *Owner;
	mu_uint32 Type;
	mu_int32 ItemType;
#ifdef USE_DUAL_QUATERNION
	EDualQuat LatestMatrix;
#else
	EMatrix4 LatestMatrix;
#endif
};

extern MUGuildSprite g_GuildSprites[MAX_GUILDSPRITES];
extern mu_atomic_uint32_t g_GuildSpritesCount;
extern mu_uint32 g_GuildSpritesRenderCount;

extern const mu_uint32 MarkColorNonBlend[MAX_GUILD_COLORS];
extern const mu_uint32 MarkColorBlend[MAX_GUILD_COLORS];

void RenderGuild(OBJECT *o, mu_int32 Type, mu_uint32 TextureIndex);
void CreateGuildMark(const mu_uint8 *MarkBuffer, mu_boolean Blend, MUTexture *MarkTexture, EStagingBufferBackend *StagingBuffer);

#endif