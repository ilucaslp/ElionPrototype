#ifndef __MU_DIRECTX_HELPERS_H__
#define __MU_DIRECTX_HELPERS_H__

#pragma once

namespace EDirectXHelpers
{
	FORCEINLINE const mu_boolean RetrieveDefaultAdapter(IDXGIAdapter *&adapter)
	{
		IDXGIFactory *factory = nullptr;
		adapter = nullptr;

		HRESULT result = EDirectX::CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));
		if (FAILED(result))
		{
			return false;
		}

		result = factory->EnumAdapters(0, &adapter);
		factory->Release();

		if (FAILED(result))
		{
			return false;
		}

		return true;
	}

	FORCEINLINE const mu_boolean RetrieveCompatibleResolution(DXGI_MODE_DESC &Resolution, const mu_uint32 Width, const mu_uint32 Height, const mu_boolean AnyCloseResolution = false, const DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM)
	{
		IDXGIAdapter *adapter = nullptr;

		if (RetrieveDefaultAdapter(adapter) == false)
		{
			return false;
		}

		IDXGIOutput *output = nullptr;

		HRESULT result = adapter->EnumOutputs(0, &output);
		if (FAILED(result))
		{
			adapter->Release();
			return false;
		}

		mu_uint32 numModes = 0;

		result = output->GetDisplayModeList(Format, 0, &numModes, nullptr);
		if (FAILED(result))
		{
			output->Release();
			adapter->Release();
			return false;
		}

		std::vector<DXGI_MODE_DESC> displayModes(numModes);

		result = output->GetDisplayModeList(Format, 0, &numModes, &displayModes[0]);
		if (FAILED(result))
		{
			output->Release();
			adapter->Release();
			return false;
		}

		mu_zeromem(&Resolution, sizeof(Resolution));
		for (mu_uint32 n = 0; n < numModes; ++n)
		{
			const DXGI_MODE_DESC &desc = displayModes[n];

			if (desc.Width == Width &&
				desc.Height == Height &&
				(Resolution.Width == 0 ||
				desc.RefreshRate.Numerator > Resolution.RefreshRate.Numerator))
			{
				mu_memcpy(&Resolution, &desc, sizeof(DXGI_MODE_DESC));
			}
		}

		output->Release();
		adapter->Release();

		if (Resolution.Width == 0)
		{
			if (AnyCloseResolution == false)
			{
				return false;
			}

			for (mu_uint32 n = 0; n < numModes; ++n)
			{
				const DXGI_MODE_DESC &desc = displayModes[n];

				if (desc.Width > Width ||
					desc.Height > Height)
				{
					continue;
				}

				if (desc.Width > Resolution.Width ||
					(desc.Width == Resolution.Width &&
						desc.Height > Resolution.Height) ||
					desc.RefreshRate.Numerator > Resolution.RefreshRate.Numerator)
				{
					mu_memcpy(&Resolution, &desc, sizeof(DXGI_MODE_DESC));
				}
			}

			if (Resolution.Width == 0)
			{
				return false;
			}
		}

		return true;
	}

	template<typename T>
	void Release(T *&Data)
	{
		if(Data != nullptr)
		{
			Data->Release();
			Data = nullptr;
		}
	}
};

#endif