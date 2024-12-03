#include "stdafx.h"

const ESampler TextureEnumToSamplers[TSAMPLER_MAX] =
{
	ESampler::eUcVcWcMinNMagNMipN_Anisotropy,
	ESampler::eUrVrWrMinNMagNMipN_Anisotropy,
	ESampler::eUcVcWcMinLMagLMipL_Anisotropy,
	ESampler::eUrVrWrMinLMagLMipL_Anisotropy,
};

const mu_boolean RetrieveCompatibleResolution(SDL_DisplayMode &Resolution, const mu_uint32 Width, const mu_uint32 Height, const mu_boolean AnyCloseResolution)
{
	mu_int32 displayIndex = SDL_GetWindowDisplayIndex(MU_WINDOW->GetSDLWindow());
	if (displayIndex == -1)
	{
		return false;
	}

	mu_zeromem(&Resolution, sizeof(Resolution));
	SDL_DisplayMode mode;
	for (mu_uint32 n = 0; SDL_GetDisplayMode(displayIndex, n, &mode) == 0; ++n)
	{
		if (mode.w == Width &&
			mode.h == Height &&
			(Resolution.w == 0 ||
				mode.refresh_rate > Resolution.refresh_rate))
		{
			mu_memcpy(&Resolution, &mode, sizeof(Resolution));
		}
	}

	if (Resolution.w == 0)
	{
		if (AnyCloseResolution == false)
		{
			return false;
		}

		for (mu_uint32 n = 0; SDL_GetDisplayMode(displayIndex, n, &mode) == 0; ++n)
		{
			if (mode.w > Width ||
				mode.h > Height)
			{
				continue;
			}

			if (mode.w > Resolution.w ||
				(mode.w == Resolution.w &&
					mode.h > Resolution.h) ||
				mode.refresh_rate > Resolution.refresh_rate)
			{
				mu_memcpy(&Resolution, &mode, sizeof(Resolution));
			}
		}

		if (Resolution.w == 0)
		{
			return false;
		}
	}

	return true;
}