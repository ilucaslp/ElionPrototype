#ifndef __MU_BACKEND_GLOBAL_H__
#define __MU_BACKEND_GLOBAL_H__

#pragma once

#include "mu_video_backend_types.h"

namespace EVideoBackend
{
	extern EVideoBackendType CurrentBackend;
	
	extern mu_uint32 VideoStatusFlag;
	extern mu_uint32 VideoRequestFlag;

	void SetBackend(const EVideoBackendType backendType);
	FORCEINLINE const EVideoBackendType GetBackend()
	{
		return CurrentBackend;
	}

	const mu_text *GetBackendString();

	void SetTextureCapabilities(const ETextureType type, const mu_uint32 maxSize, const mu_uint32 maxArraySize);
	const mu_uint32 GetTextureMaxSize(const ETextureType type);
	const mu_uint32 GetTextureMaxArray(const ETextureType type);

	const mu_boolean PrepareShaders();

	void RequestReloadVerticalSync();
	void RequestResizeWindow();
	void RequestChangeWindowMode();
	void RequestReloadMSAA();
	void RequestReloadShaders();
	void RequestReloadShadowMap();

	void ClearReloadRequests();

	const mu_boolean PreReload();
	const mu_boolean ReloadWindowMode();
	const mu_boolean ReloadWindowSize();
	void Reload();

	const mu_uint32 GetBufferingCount();
	const mu_uint32 GetCurrentBufferingIndex();

	const mu_boolean TakeScreenshot(const mu_text *filename);

	void WaitForDevice();
	void BeginFrame();
	void EndFrame();
	void BeginShadowRenderPass();
	void EndShadowRenderPass();
	void BeginRenderPass(const std::array<mu_float, 4> ClearColor);
	void EndRenderPass();
	void SubmitFrame();
	void PresentFrame();
	void FinishAndWaitDevice();

	void ClearDepthAndStencil();

	void SetViewport(const mu_float OffsetX, const mu_float OffsetY, const mu_float Width, const mu_float Height);
	void SetScissor(const mu_int32 OffsetX, const mu_int32 OffsetY, const mu_int32 Width, const mu_int32 Height);
	void CleanScissor();

	void RenderMouse(const mu_double ElapsedTime, const mu_uint64 CurrentTime);

	void UpdateGlobalOnly();
	void UpdateBuffers(const mu_boolean IsLightUpload);

	void UIPreUpdateBuffers();
	void UIPostUpdateBuffers();
	const mu_uint32 UIAllocateDataIndex(const mu_uint32 count);
	SHUIDefaultVertex *UIRetrieveDataBuffer(const mu_uint32 index);
	void UIRequestUpdateData(const mu_uint32 index, const mu_uint32 count);

	void FontPreUpdateBuffers();
	void FontPostUpdateBuffers();
	const mu_uint32 FontAllocateDataIndex(const mu_uint32 count);
	void FontDeallocateDataIndex(const mu_uint32 index);
	SHFontInstance *FontRetrieveDataBuffer(const mu_uint32 index);
	const mu_uint32 FontRetrieveBufferBaseIndex(const mu_uint32 index);
	void FontRequestUpdateData(const mu_uint32 index, const mu_uint32 count);

	void EmptyTrash();
	void DefragmentBuffer();
	void CollectGarbage();

	void BindGlobalConstant(const EGlobalConstant globalConstant);

	void Unproject2D(const mu_float X, const mu_float Y, const mu_float AspectWidth, const mu_float AspectHeight, EVector3 &Source, EVector3 &Target);

	EINLINE void SetVideoStatus(const mu_uint32 flag)
	{
		VideoStatusFlag |= flag;
	}

	EINLINE void UnsetVideoStatus(const mu_uint32 flag)
	{
		VideoStatusFlag &= ~flag;
	}

	EINLINE const mu_boolean GetVideoStatus(const mu_uint32 flag)
	{
		return (VideoStatusFlag & flag) == flag;
	}

	EINLINE void SetVideoRequest(const mu_uint32 flag)
	{
		VideoRequestFlag |= flag;
	}

	EINLINE void UnsetVideoRequest(const mu_uint32 flag)
	{
		VideoRequestFlag &= ~flag;
	}

	EINLINE const mu_boolean GetVideoRequest(const mu_uint32 flag)
	{
		return (VideoRequestFlag & flag) == flag;
	}
};

#endif