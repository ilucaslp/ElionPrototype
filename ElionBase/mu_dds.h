#ifndef __MU_DDS_H__
#define __MU_DDS_H__

#pragma once

#if DIRECTX_BACKEND_ENABLE == 1
#pragma pack(push,1)

const mu_uint32 DDS_MAGIC = 0x20534444; // "DDS "

struct DDS_PIXELFORMAT
{
	mu_uint32    size;
	mu_uint32    flags;
	mu_uint32    fourCC;
	mu_uint32    RGBBitCount;
	mu_uint32    RBitMask;
	mu_uint32    GBitMask;
	mu_uint32    BBitMask;
	mu_uint32    ABitMask;
};

#define DDS_FOURCC      0x00000004  // DDPF_FOURCC
#define DDS_RGB         0x00000040  // DDPF_RGB
#define DDS_RGBA        0x00000041  // DDPF_RGB | DDPF_ALPHAPIXELS
#define DDS_LUMINANCE   0x00020000  // DDPF_LUMINANCE
#define DDS_LUMINANCEA  0x00020001  // DDPF_LUMINANCE | DDPF_ALPHAPIXELS
#define DDS_ALPHA       0x00000002  // DDPF_ALPHA
#define DDS_PAL8        0x00000020  // DDPF_PALETTEINDEXED8
#define DDS_BUMPDUDV    0x00080000  // DDPF_BUMPDUDV

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((mu_uint32)(mu_uint8)(ch0) | ((mu_uint32)(mu_uint8)(ch1) << 8) |       \
                ((mu_uint32)(mu_uint8)(ch2) << 16) | ((mu_uint32)(mu_uint8)(ch3) << 24 ))
#endif /* defined(MAKEFOURCC) */

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_DXT1 =
{ sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','1'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_DXT2 =
{ sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','2'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_DXT3 =
{ sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','3'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_DXT4 =
{ sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','4'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_DXT5 =
{ sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','5'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_BC4_UNORM =
{ sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('B','C','4','U'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_BC4_SNORM =
{ sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('B','C','4','S'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_BC5_UNORM =
{ sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('B','C','5','U'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_BC5_SNORM =
{ sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('B','C','5','S'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_R8G8_B8G8 =
{ sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('R','G','B','G'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_G8R8_G8B8 =
{ sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('G','R','G','B'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_YUY2 =
{ sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('Y','U','Y','2'), 0, 0, 0, 0, 0 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_A8R8G8B8 =
{ sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_X8R8G8B8 =
{ sizeof(DDS_PIXELFORMAT), DDS_RGB,  0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_A8B8G8R8 =
{ sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_X8B8G8R8 =
{ sizeof(DDS_PIXELFORMAT), DDS_RGB,  0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_G16R16 =
{ sizeof(DDS_PIXELFORMAT), DDS_RGB,  0, 32, 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_R5G6B5 =
{ sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 16, 0x0000f800, 0x000007e0, 0x0000001f, 0x00000000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_A1R5G5B5 =
{ sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 16, 0x00007c00, 0x000003e0, 0x0000001f, 0x00008000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_A4R4G4B4 =
{ sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 16, 0x00000f00, 0x000000f0, 0x0000000f, 0x0000f000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_R8G8B8 =
{ sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 24, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_L8 =
{ sizeof(DDS_PIXELFORMAT), DDS_LUMINANCE, 0,  8, 0xff, 0x00, 0x00, 0x00 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_L16 =
{ sizeof(DDS_PIXELFORMAT), DDS_LUMINANCE, 0, 16, 0xffff, 0x0000, 0x0000, 0x0000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_A8L8 =
{ sizeof(DDS_PIXELFORMAT), DDS_LUMINANCEA, 0, 16, 0x00ff, 0x0000, 0x0000, 0xff00 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_A8 =
{ sizeof(DDS_PIXELFORMAT), DDS_ALPHA, 0, 8, 0x00, 0x00, 0x00, 0xff };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_V8U8 =
{ sizeof(DDS_PIXELFORMAT), DDS_BUMPDUDV, 0, 16, 0x00ff, 0xff00, 0x0000, 0x0000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_Q8W8V8U8 =
{ sizeof(DDS_PIXELFORMAT), DDS_BUMPDUDV, 0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 };

extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_V16U16 =
{ sizeof(DDS_PIXELFORMAT), DDS_BUMPDUDV, 0, 32, 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000 };

// D3DFMT_A2R10G10B10/D3DFMT_A2B10G10R10 should be written using DX10 extension to avoid D3DX 10:10:10:2 reversal issue

// This indicates the DDS_HEADER_DXT10 extension is present (the format is in dxgiFormat)
extern __declspec(selectany) const DDS_PIXELFORMAT DDSPF_DX10 =
{ sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','1','0'), 0, 0, 0, 0, 0 };

#define DDS_HEADER_FLAGS_TEXTURE        0x00001007  // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT 
#define DDS_HEADER_FLAGS_MIPMAP         0x00020000  // DDSD_MIPMAPCOUNT
#define DDS_HEADER_FLAGS_VOLUME         0x00800000  // DDSD_DEPTH
#define DDS_HEADER_FLAGS_PITCH          0x00000008  // DDSD_PITCH
#define DDS_HEADER_FLAGS_LINEARSIZE     0x00080000  // DDSD_LINEARSIZE

#define DDS_HEIGHT 0x00000002 // DDSD_HEIGHT
#define DDS_WIDTH  0x00000004 // DDSD_WIDTH

#define DDS_SURFACE_FLAGS_TEXTURE 0x00001000 // DDSCAPS_TEXTURE
#define DDS_SURFACE_FLAGS_MIPMAP  0x00400008 // DDSCAPS_COMPLEX | DDSCAPS_MIPMAP
#define DDS_SURFACE_FLAGS_CUBEMAP 0x00000008 // DDSCAPS_COMPLEX

#define DDS_CUBEMAP_POSITIVEX 0x00000600 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
#define DDS_CUBEMAP_NEGATIVEX 0x00000a00 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
#define DDS_CUBEMAP_POSITIVEY 0x00001200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
#define DDS_CUBEMAP_NEGATIVEY 0x00002200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
#define DDS_CUBEMAP_POSITIVEZ 0x00004200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
#define DDS_CUBEMAP_NEGATIVEZ 0x00008200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ

#define DDS_CUBEMAP_ALLFACES ( DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |\
                               DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |\
                               DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ )

#define DDS_CUBEMAP 0x00000200 // DDSCAPS2_CUBEMAP

#define DDS_FLAGS_VOLUME 0x00200000 // DDSCAPS2_VOLUME

// Subset here matches D3D10_RESOURCE_DIMENSION and D3D11_RESOURCE_DIMENSION
enum DDS_RESOURCE_DIMENSION
{
	DDS_DIMENSION_TEXTURE1D = 2,
	DDS_DIMENSION_TEXTURE2D = 3,
	DDS_DIMENSION_TEXTURE3D = 4,
};

// Subset here matches D3D10_RESOURCE_MISC_FLAG and D3D11_RESOURCE_MISC_FLAG
enum DDS_RESOURCE_MISC_FLAG
{
	DDS_RESOURCE_MISC_TEXTURECUBE = 0x4L,
};

enum DDS_MISC_FLAGS2
{
	DDS_MISC_FLAGS2_ALPHA_MODE_MASK = 0x7L,
};

struct DDS_HEADER
{
	mu_uint32        size;
	mu_uint32        flags;
	mu_uint32        height;
	mu_uint32        width;
	mu_uint32        pitchOrLinearSize;
	mu_uint32        depth; // only if DDS_HEADER_FLAGS_VOLUME is set in flags
	mu_uint32        mipMapCount;
	mu_uint32        reserved1[11];
	DDS_PIXELFORMAT ddspf;
	mu_uint32        caps;
	mu_uint32        caps2;
	mu_uint32        caps3;
	mu_uint32        caps4;
	mu_uint32        reserved2;
};

struct DDS_HEADER_DXT10
{
	DXGI_FORMAT     dxgiFormat;
	mu_uint32        resourceDimension;
	mu_uint32        miscFlag; // see D3D11_RESOURCE_MISC_FLAG
	mu_uint32        arraySize;
	mu_uint32        miscFlags2; // see DDS_MISC_FLAGS2
};

#pragma pack(pop)

static_assert(sizeof(DDS_HEADER) == 124, "DDS Header size mismatch");
static_assert(sizeof(DDS_HEADER_DXT10) == 20, "DDS DX10 Extended Header size mismatch");

HRESULT FillInitData(mu_size width,
	mu_size height,
	mu_size depth,
	mu_size mipCount,
	mu_size arraySize,
	DXGI_FORMAT format,
	mu_size maxsize,
	mu_size bitSize,
	const mu_uint8* bitData,
	mu_size& twidth,
	mu_size& theight,
	mu_size& tdepth,
	mu_size& skipMip,
	D3D11_SUBRESOURCE_DATA* initData);
HRESULT CreateD3DResources(ID3D11Device* d3dDevice,
	mu_uint32 resDim,
	mu_size width,
	mu_size height,
	mu_size depth,
	mu_size mipCount,
	mu_size arraySize,
	DXGI_FORMAT format,
	D3D11_USAGE usage,
	mu_uint32 bindFlags,
	mu_uint32 cpuAccessFlags,
	mu_uint32 miscFlags,
	mu_boolean forceSRGB,
	mu_boolean isCubeMap,
	D3D11_SUBRESOURCE_DATA* initData,
	ID3D11Resource** texture,
	ID3D11ShaderResourceView** textureView);
HRESULT CreateTextureFromDDS(ID3D11Device* d3dDevice,
	ID3D11DeviceContext* d3dContext,
	const DDS_HEADER* header,
	_In_reads_bytes_(bitSize) const mu_uint8* bitData,
	mu_size bitSize,
	mu_size maxsize,
	D3D11_USAGE usage,
	mu_uint32 bindFlags,
	mu_uint32 cpuAccessFlags,
	mu_uint32 miscFlags,
	mu_boolean forceSRGB,
	ID3D11Resource** texture,
	ID3D11ShaderResourceView** textureView);
HRESULT CreateDDSTextureFromMemoryEx(ID3D11Device* d3dDevice,
	const mu_uint8* ddsData,
	mu_size ddsDataSize,
	mu_size maxsize,
	D3D11_USAGE usage,
	mu_uint32 bindFlags,
	mu_uint32 cpuAccessFlags,
	mu_uint32 miscFlags,
	mu_boolean forceSRGB,
	ID3D11Resource** texture,
	ID3D11ShaderResourceView** textureView);
#endif

#endif