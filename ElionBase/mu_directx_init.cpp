#include "stdafx.h"

#if DIRECTX_BACKEND_ENABLE == 1
namespace EDirectXInit
{
	HMODULE Direct3DLibrary = nullptr;
	HMODULE D3DCompilerLibrary = nullptr;
	HMODULE DXGILibrary = nullptr;

	D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_UNKNOWN;
	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_10_0;
	ID3D11Device *Device = nullptr;
	ID3D11DeviceContext *DeviceContext = nullptr;
	DXGI_SWAP_CHAIN_DESC SwapchainDesc = {};
	IDXGISwapChain *Swapchain = nullptr;
	ID3D11RenderTargetView *RenderTargetView = nullptr;
	D3D11_TEXTURE2D_DESC DepthStencilDesc = {};
	ID3D11Texture2D *DepthStencilBuffer = nullptr;
	ID3D11DepthStencilView *DepthStencilView = nullptr;

	mu_uint32 TextureUVSize = 0;
	mu_uint32 TextureDepthSize = 0;

	const mu_boolean Initialize(SDL_Window *SDLWindow)
	{
		if (LoadLibraries() == false)
		{
			return false;
		}

		EDirectX::LoadFunctions();

		if (CreateDevice() == false)
		{
			return false;
		}

		if (CreateSwapchain() == false)
		{
			return false;
		}

		if (CreateRenderTargetView() == false)
		{
			return false;
		}

		if (EDirectXBuffer::CreateMultisample() == false)
		{
			return false;
		}

		if (CreateDepthBuffer() == false)
		{
			return false;
		}

		EDirectXInit::DeviceContext->OMSetRenderTargets(1, &EDirectXInit::RenderTargetView, EDirectXInit::DepthStencilView);

		if (EDirectXPipeline::Create() == false)
		{
			return false;
		}

		if (EDirectXShader::Create(true) == false)
		{
			return false;
		}

		if (EDirectXBuffer::Create() == false)
		{
			return false;
		}

		if (EDirectXBuffer::CreateShadowMap() == false)
		{
			return false;
		}

		return true;
	}

	void Destroy(const mu_boolean gpuOnly)
	{
		Swapchain->SetFullscreenState(FALSE, nullptr);

		EDirectXBuffer::DestroyMultisample();
		EDirectXBuffer::DestroyShadowMap();
		EDirectXBuffer::Destroy(gpuOnly);
		EDirectXShader::Destroy();
		EDirectXPipeline::Destroy();
		DestroyDepthBuffer();
		DestroyRenderTargetView();
		DestroySwapchain();
		DestroyDevice();
		ReleaseLibraries();
	}

	void ConfigureFeatures()
	{
		switch (FeatureLevel)
		{
		case D3D_FEATURE_LEVEL_10_0:
		case D3D_FEATURE_LEVEL_10_1:
			{
				EVideoBackend::SetTextureCapabilities(ETextureType::eTexture1D, D3D10_REQ_TEXTURE1D_U_DIMENSION, D3D10_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION);
				EVideoBackend::SetTextureCapabilities(ETextureType::eTexture2D, D3D10_REQ_TEXTURE2D_U_OR_V_DIMENSION, D3D10_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION);
				EVideoBackend::SetTextureCapabilities(ETextureType::eTexture3D, D3D10_REQ_TEXTURE3D_U_V_OR_W_DIMENSION, 1);
				EVideoBackend::SetTextureCapabilities(ETextureType::eTextureCube, D3D10_REQ_TEXTURECUBE_DIMENSION, 1);
			}
			break;

		case D3D_FEATURE_LEVEL_11_0:
			{
				EVideoBackend::SetTextureCapabilities(ETextureType::eTexture1D, D3D11_REQ_TEXTURE1D_U_DIMENSION, D3D11_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION);
				EVideoBackend::SetTextureCapabilities(ETextureType::eTexture2D, D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION, D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION);
				EVideoBackend::SetTextureCapabilities(ETextureType::eTexture3D, D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION, 1);
				EVideoBackend::SetTextureCapabilities(ETextureType::eTextureCube, D3D11_REQ_TEXTURECUBE_DIMENSION, 1);
			}
			break;
		}
	}

	const mu_boolean LoadLibraries()
	{
		if (Direct3DLibrary == nullptr)
		{
			Direct3DLibrary = LoadLibrary(_T("d3d11.dll"));
		}

		if (D3DCompilerLibrary == nullptr)
		{
			const mu_text *libraries[] =
			{
				_T("d3dcompiler_47.dll"),
				_T("d3dcompiler_46.dll"),
				_T("d3dcompiler_43.dll"),
			};
			for(mu_uint32 n = 0;n < mu_countof(libraries);++n)
			{
				D3DCompilerLibrary = LoadLibrary(libraries[n]);
				if (D3DCompilerLibrary != nullptr)
				{
					break;
				}
			}
		}

		if (DXGILibrary == nullptr)
		{
			DXGILibrary = LoadLibrary(_T("dxgi.dll"));
		}

		if (Direct3DLibrary == nullptr ||
			D3DCompilerLibrary == nullptr ||
			DXGILibrary == nullptr)
		{
			ReleaseLibraries();
			return false;
		}

		return true;
	}

	void ReleaseLibraries()
	{
		if (DXGILibrary != nullptr)
		{
			FreeLibrary(DXGILibrary);
			DXGILibrary = nullptr;
		}

		if (D3DCompilerLibrary != nullptr)
		{
			FreeLibrary(D3DCompilerLibrary);
			D3DCompilerLibrary = nullptr;
		}

		if (Direct3DLibrary != nullptr)
		{
			FreeLibrary(Direct3DLibrary);
			Direct3DLibrary = nullptr;
		}
	}

	const mu_boolean CreateDevice()
	{
		const D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
		};
		const mu_uint32 numDriverTypes = mu_countof(driverTypes);

		const D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		const mu_uint32 numFeatureLevels = mu_countof(featureLevels);

		HRESULT result;

		for (mu_uint32 driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
		{
			DriverType = driverTypes[driverTypeIndex];
#if DIRECTX_BACKEND_DEBUG == 1
			result = EDirectX::D3D11CreateDevice(nullptr, DriverType, nullptr, D3D11_CREATE_DEVICE_DEBUG, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &Device, &FeatureLevel, &DeviceContext);
#else
			result = EDirectX::D3D11CreateDevice(nullptr, DriverType, nullptr, 0, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &Device, &FeatureLevel, &DeviceContext);
#endif

			if (SUCCEEDED(result))
			{
				break;
			}
		}

		if (FAILED(result))
		{
			mu_error("Failed to create directx context.");
			return false;
		}

		ConfigureFeatures();

		IDXGIDevice *dxgiDevice = nullptr;
		result = Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (FAILED(result))
		{
			return false;
		}

		IDXGIAdapter *dxgiAdapter = nullptr;
		result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));
		if (FAILED(result))
		{
			dxgiDevice->Release();
			return false;
		}

		IDXGIFactory *dxgiFactory = nullptr;
		result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));
		if (FAILED(result))
		{
			dxgiAdapter->Release();
			dxgiDevice->Release();
			return false;
		}

		dxgiFactory->MakeWindowAssociation(MU_WINDOW->GetHWND(), DXGI_MWA_NO_ALT_ENTER);

		dxgiFactory->Release();
		dxgiAdapter->Release();
		dxgiDevice->Release();

		return true;
	}

	void DestroyDevice()
	{
		EDirectXHelpers::Release(DeviceContext);
		EDirectXHelpers::Release(Device);
	}

	const mu_boolean CreateSwapchain()
	{
		HRESULT result;
		DXGI_SWAP_CHAIN_DESC swapchainDesc = {};
		
		if (EDirectXHelpers::RetrieveCompatibleResolution(swapchainDesc.BufferDesc, MU_RESOURCE->GetWindowWidth(), MU_RESOURCE->GetWindowHeight(), true, DXGI_FORMAT_R8G8B8A8_UNORM) == false)
		{
			swapchainDesc.BufferDesc.Width = MU_RESOURCE->GetWindowWidth();
			swapchainDesc.BufferDesc.Height = MU_RESOURCE->GetWindowHeight();
			swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapchainDesc.BufferDesc.RefreshRate.Numerator = 60;
			swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
			swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		}

		swapchainDesc.BufferCount = DIRECTX_BACKEND_BUFFERCOUNT;
		swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchainDesc.OutputWindow = MU_WINDOW->GetHWND();
		swapchainDesc.Windowed = TRUE;
		swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		swapchainDesc.SampleDesc.Count = 1;
		swapchainDesc.SampleDesc.Quality = 0;

		IDXGIDevice *dxgiDevice = nullptr;
		result = Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (FAILED(result))
		{
			return false;
		}

		IDXGIAdapter *dxgiAdapter = nullptr;
		result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));
		if (FAILED(result))
		{
			dxgiDevice->Release();
			return false;
		}

		IDXGIFactory *dxgiFactory = nullptr;
		result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));
		if (FAILED(result))
		{
			dxgiAdapter->Release();
			dxgiDevice->Release();
			return false;
		}

		result = dxgiFactory->CreateSwapChain(Device, &swapchainDesc, &Swapchain);

		dxgiFactory->Release();
		dxgiAdapter->Release();
		dxgiDevice->Release();

		if (FAILED(result))
		{
			return false;
		}

		mu_memcpy(&SwapchainDesc, &swapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		return true;
	}

	void DestroySwapchain()
	{
		EDirectXHelpers::Release(Swapchain);
	}

	const mu_boolean CreateRenderTargetView()
	{
		ID3D11Texture2D *backBuffer = nullptr;
		HRESULT result = Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
		if (FAILED(result))
		{
			return false;
		}

		D3D11_TEXTURE2D_DESC backDesc = {};
		backBuffer->GetDesc(&backDesc);
		result = Device->CreateRenderTargetView(backBuffer, nullptr, &RenderTargetView);
		backBuffer->Release();

		if (FAILED(result))
		{
			return false;
		}

		return true;
	}

	void DestroyRenderTargetView()
	{
		EDirectXHelpers::Release(RenderTargetView);
	}

	const mu_boolean CreateDepthBuffer()
	{
		mu_zeromem(&DepthStencilDesc, sizeof(DepthStencilDesc));

		const DXGI_FORMAT msaaFormat = EDirectXInit::SwapchainDesc.BufferDesc.Format;
		mu_uint32 msaaSampleCount = 16;
		mu_uint32 msaaQuality = 0;

		MUTexture *multisampleTexture = EDirectXBuffer::GetMultisampleTexture(0);
		if (MU_RESOURCE->IsAntialiasing() == true &&
			multisampleTexture->IsLoaded() == true)
		{
			for (; msaaSampleCount >= 2; msaaSampleCount /= 2)
			{
				HRESULT result = EDirectXInit::Device->CheckMultisampleQualityLevels(msaaFormat, msaaSampleCount, &msaaQuality);
				if (SUCCEEDED(result) && msaaQuality > 0)
				{
					result = EDirectXInit::Device->CheckMultisampleQualityLevels(DXGI_FORMAT_D32_FLOAT, msaaSampleCount, &msaaQuality);
					if (SUCCEEDED(result) && msaaQuality > 0)
					{
						break;
					}
				}
			}

			if (msaaQuality > 0)
			{
				--msaaQuality;
			}
		}
		else
		{
			msaaSampleCount = 1;
			msaaQuality = 0;
		}

		DepthStencilDesc.Width = SwapchainDesc.BufferDesc.Width;
		DepthStencilDesc.Height = SwapchainDesc.BufferDesc.Height;
		DepthStencilDesc.MipLevels = 1;
		DepthStencilDesc.ArraySize = 1;
		DepthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
		DepthStencilDesc.SampleDesc.Count = msaaSampleCount;
		DepthStencilDesc.SampleDesc.Quality = msaaQuality;
		DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		DepthStencilDesc.CPUAccessFlags = 0;
		DepthStencilDesc.MiscFlags = 0;

		HRESULT result = Device->CreateTexture2D(&DepthStencilDesc, nullptr, &DepthStencilBuffer);
		if (FAILED(result))
		{
			return false;
		}

		result = Device->CreateDepthStencilView(DepthStencilBuffer, nullptr, &DepthStencilView);
		if (FAILED(result))
		{
			return false;
		}

		return true;
	}

	void DestroyDepthBuffer()
	{
		EDirectXHelpers::Release(DepthStencilView);
		EDirectXHelpers::Release(DepthStencilBuffer);
	}
};
#endif