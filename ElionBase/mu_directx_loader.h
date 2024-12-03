#ifndef __MU_DIRECTX_LOADER_H__
#define __MU_DIRECTX_LOADER_H__

#pragma once

namespace EDirectX
{
	typedef HRESULT(WINAPI* PFN_CREATE_DXGI_FACTORY)(REFIID riid, _COM_Outptr_ void **ppFactory);

	extern PFN_D3D11_CREATE_DEVICE D3D11CreateDevice;
	extern PFN_CREATE_DXGI_FACTORY CreateDXGIFactory;
	extern pD3DCompile D3DCompile;

	void LoadFunctions();
};

#endif