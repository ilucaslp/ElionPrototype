#include "stdafx.h"
#include "mu_renderproxy_effects.h"

namespace MURenderProxyEffects
{
	std::array<mu_uint32, MAX_JOINTS> JointsInstanceIndex;
	std::array<mu_uint32[2], MAX_PARTICLES> ParticlesInstanceIndex;
	std::array<mu_uint32, MAX_BLURS> BlursInstanceIndex;
	std::array<mu_uint32, MAX_OBJECTBLURS> ObjectBlursInstanceIndex;
	std::array<mu_uint32, MAX_LEAVES_DOUBLE> LeavesInstanceIndex;
	std::array<mu_uint32, MAX_POINTERS> PointersInstanceIndex;
	std::array<mu_uint32, MAX_SPRITES> SpritesInstanceIndex;
	std::array<mu_uint32, MAX_TILESPRITES> TileSpritesInstanceIndex;
	std::array<mu_uint32, MAX_GUILDSPRITES> GuildSpritesInstanceIndex;
};