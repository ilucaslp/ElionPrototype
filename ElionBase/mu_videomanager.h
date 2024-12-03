#ifndef __MU_VIDEO_MANAGER_H__
#define __MU_VIDEO_MANAGER_H__

#pragma once

class MUCamera;

class MUVideoManager
{
public:
	MUVideoManager();
	~MUVideoManager();

	mu_boolean Initialize(SDL_Window *SDLWindow);
	void Shutdown(const mu_boolean gpuOnly = false);

	void ConfigureViewport2D();

	static void Prepare2DCamera();

	static void OnUpdateBuffer();

public:
	static EViewport _2DViewport;
	static EViewport _3DViewport;

	// View/Projection Matrices
	static MUCamera *_CurrentCamera;
	static EMatrix4 _CurrentShadowView;
	static EMatrix4 _CurrentShadowProjection;
	static EMatrix4 _CurrentView;
	static EMatrix4 _CurrentInverseView;
	static EMatrix4 _CurrentTextView;
	static EDualQuat _CurrentTextViewDQ;
	static EMatrix4 _CurrentProjection;
	static EMatrix4 _CurrentRayProjection;
	static EMatrix4 _CurrentView2D;
	static EMatrix4 _CurrentProjection2D;
	static EMatrix4 _CurrentProjectionUI;

	static mu_float _LastWorldTime;
	static mu_int32 _LastRandom;
};

#define MU_RANDOM (MUVideoManager::_LastRandom)
#define MU_CAMERA (MUVideoManager::_CurrentCamera)

#endif