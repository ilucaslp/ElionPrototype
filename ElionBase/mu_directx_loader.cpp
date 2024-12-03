#include "stdafx.h"

#if DIRECTX_BACKEND_ENABLE == 1
namespace EDirectX
{
	PFN_D3D11_CREATE_DEVICE D3D11CreateDevice = nullptr;
	PFN_CREATE_DXGI_FACTORY CreateDXGIFactory = nullptr;
	pD3DCompile D3DCompile = nullptr;

	void LoadFunctions()
	{
		D3D11CreateDevice = (PFN_D3D11_CREATE_DEVICE)GetProcAddress(EDirectXInit::Direct3DLibrary, "D3D11CreateDevice");
		CreateDXGIFactory = (PFN_CREATE_DXGI_FACTORY)GetProcAddress(EDirectXInit::DXGILibrary, "CreateDXGIFactory");
		D3DCompile = (pD3DCompile)GetProcAddress(EDirectXInit::D3DCompilerLibrary, "D3DCompile");
	}
};
#endif