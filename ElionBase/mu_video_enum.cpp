#include "stdafx.h"

const mu_uint32 ETextureComponents[ETextureFormatMax] =
{
	1,
	1,
	1,
	1,
	
	2,
	2,
	2,
	2,

	4,
	4,
	4,
	4,

	1,
	1,
	1,
	1,
	1,

	2,
	2,
	2,
	2,
	2,

	4,
	4,
	4,
	4,
	4,

	1,
	1,
	1,

	2,
	2,
	2,

	3,
	3,
	3,

	4,
	4,
	4,

	1,
	1,
};

const mu_uint8 EffectSamplerMap[ESamplerMax] =
{
	// Linears
	static_cast<mu_uint32>(EEffectSampler::eUrVrWrMinLMagLMipL_Anisotropy),
	static_cast<mu_uint32>(EEffectSampler::eUrVrWrMinLMagLMipL_Anisotropy),
	static_cast<mu_uint32>(EEffectSampler::eUrVrWrMinLMagLMipL_Anisotropy),

	// Nearests
	static_cast<mu_uint32>(EEffectSampler::eUcVcWcMinNMagNMipN),
	static_cast<mu_uint32>(EEffectSampler::eUrVrWrMinLMagLMipL_Anisotropy),
	static_cast<mu_uint32>(EEffectSampler::eUrVrWrMinLMagLMipL_Anisotropy),

	// Anisotropy Linears
	static_cast<mu_uint32>(EEffectSampler::eUrVrWrMinLMagLMipL_Anisotropy),
	static_cast<mu_uint32>(EEffectSampler::eUcbVcbWcbMinLMagLMipL_Anisotropy),
	static_cast<mu_uint32>(EEffectSampler::eUrVrWrMinLMagLMipL_Anisotropy),
	
	// Anisotropy Nearests
	static_cast<mu_uint32>(EEffectSampler::eUrVrWrMinLMagLMipL_Anisotropy),
	static_cast<mu_uint32>(EEffectSampler::eUrVrWrMinLMagLMipL_Anisotropy),
	static_cast<mu_uint32>(EEffectSampler::eUrVrWrMinLMagLMipL_Anisotropy),

	// Shadow Comparison (Dummy)
	static_cast<mu_uint32>(EEffectSampler::eUrVrWrMinLMagLMipL_Anisotropy),
	static_cast<mu_uint32>(EEffectSampler::eUrVrWrMinLMagLMipL_Anisotropy),
};

const ESampler EffectSamplers[EEffectSamplerMax] =
{
	ESampler::eUrVrWrMinLMagLMipL_Anisotropy,
	ESampler::eUcbVcbWcbMinLMagLMipL_Anisotropy,
	ESampler::eUcbVcbWcbMinNMagNMipN,
};

const mu_boolean EffectSamplersMipmap[EEffectSamplerMax] = { true };