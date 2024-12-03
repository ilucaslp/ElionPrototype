#ifndef __MU_DIRECTX_INIT_H__
#define __MU_DIRECTX_INIT_H__

#pragma once

namespace EDirectXInit
{
	extern HMODULE Direct3DLibrary;
	extern HMODULE D3DCompilerLibrary;
	extern HMODULE DXGILibrary;

	extern D3D_DRIVER_TYPE DriverType;
	extern D3D_FEATURE_LEVEL FeatureLevel;
	extern ID3D11Device *Device;
	extern ID3D11DeviceContext *DeviceContext;
	extern DXGI_SWAP_CHAIN_DESC SwapchainDesc;
	extern IDXGISwapChain *Swapchain;
	extern ID3D11RenderTargetView *RenderTargetView;
	extern D3D11_TEXTURE2D_DESC DepthStencilDesc;
	extern ID3D11Texture2D *DepthStencilBuffer;
	extern ID3D11DepthStencilView *DepthStencilView;

	extern mu_uint32 TextureUVSize;
	extern mu_uint32 TextureDepthSize;

	const mu_boolean Initialize(SDL_Window *SDLWindow);
	void Destroy(const mu_boolean gpuOnly);

	void ConfigureFeatures();

	const mu_boolean LoadLibraries();
	void ReleaseLibraries();

	const mu_boolean CreateDevice();
	void DestroyDevice();

	const mu_boolean CreateSwapchain();
	void DestroySwapchain();

	const mu_boolean CreateRenderTargetView();
	void DestroyRenderTargetView();

	const mu_boolean CreateDepthBuffer();
	void DestroyDepthBuffer();
};

#endif