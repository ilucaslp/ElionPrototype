#ifndef __MU_RENDERPROXY_EFFECTS_H__
#define __MU_RENDERPROXY_EFFECTS_H__

#pragma once

enum class EEffectProxyEnum
{
	eLeaves,
	eJoints,
	eParticles,
	eBlurs,
	eObjectBlurs,
	ePointers,
	eSprites,
	eTileSprites,
	eGuildSprites,
};

namespace MURenderProxyEffects
{
	extern std::array<mu_uint32, MAX_JOINTS> JointsInstanceIndex;
	extern std::array<mu_uint32[2], MAX_PARTICLES> ParticlesInstanceIndex;
	extern std::array<mu_uint32, MAX_BLURS> BlursInstanceIndex;
	extern std::array<mu_uint32, MAX_OBJECTBLURS> ObjectBlursInstanceIndex;
	extern std::array<mu_uint32, MAX_LEAVES_DOUBLE> LeavesInstanceIndex;
	extern std::array<mu_uint32, MAX_POINTERS> PointersInstanceIndex;
	extern std::array<mu_uint32, MAX_SPRITES> SpritesInstanceIndex;
	extern std::array<mu_uint32, MAX_TILESPRITES> TileSpritesInstanceIndex;
	extern std::array<mu_uint32, MAX_GUILDSPRITES> GuildSpritesInstanceIndex;

	template<const EEffectProxyEnum type, const mu_uint32 slot>
	void SetInstanceIndex(const mu_uint32 index, const mu_uint32 instanceindex)
	{
		if constexpr(type == EEffectProxyEnum::eJoints)
		{
			JointsInstanceIndex[index] = instanceindex;
		}
		else if constexpr (type == EEffectProxyEnum::eParticles)
		{
			ParticlesInstanceIndex[index][slot] = instanceindex;
		}
		else if constexpr (type == EEffectProxyEnum::eBlurs)
		{
			BlursInstanceIndex[index] = instanceindex;
		}
		else if constexpr (type == EEffectProxyEnum::eObjectBlurs)
		{
			ObjectBlursInstanceIndex[index] = instanceindex;
		}
		else if constexpr (type == EEffectProxyEnum::eLeaves)
		{
			LeavesInstanceIndex[index] = instanceindex;
		}
		else if constexpr (type == EEffectProxyEnum::ePointers)
		{
			PointersInstanceIndex[index] = instanceindex;
		}
		else if constexpr (type == EEffectProxyEnum::eSprites)
		{
			SpritesInstanceIndex[index] = instanceindex;
		}
		else if constexpr (type == EEffectProxyEnum::eTileSprites)
		{
			TileSpritesInstanceIndex[index] = instanceindex;
		}
		else if constexpr (type == EEffectProxyEnum::eGuildSprites)
		{
			GuildSpritesInstanceIndex[index] = instanceindex;
		}
	}

	template<const EEffectProxyEnum type, const mu_uint32 slot>
	const mu_uint32 GetInstanceIndex(const mu_uint32 index)
	{
		if constexpr (type == EEffectProxyEnum::eJoints)
		{
			return JointsInstanceIndex[index];
		}
		else if constexpr (type == EEffectProxyEnum::eParticles)
		{
			return ParticlesInstanceIndex[index][slot];
		}
		else if constexpr (type == EEffectProxyEnum::eBlurs)
		{
			return BlursInstanceIndex[index];
		}
		else if constexpr (type == EEffectProxyEnum::eObjectBlurs)
		{
			return ObjectBlursInstanceIndex[index];
		}
		else if constexpr (type == EEffectProxyEnum::eLeaves)
		{
			return LeavesInstanceIndex[index];
		}
		else if constexpr (type == EEffectProxyEnum::ePointers)
		{
			return PointersInstanceIndex[index];
		}
		else if constexpr (type == EEffectProxyEnum::eSprites)
		{
			return SpritesInstanceIndex[index];
		}
		else if constexpr (type == EEffectProxyEnum::eTileSprites)
		{
			return TileSpritesInstanceIndex[index];
		}
		else if constexpr (type == EEffectProxyEnum::eGuildSprites)
		{
			return GuildSpritesInstanceIndex[index];
		}

		return 0;
	}
};

#endif