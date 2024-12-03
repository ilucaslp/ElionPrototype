#include "stdafx.h"
#include "mu_camera.h"

const mu_float ItemViewFOV = EMath::ToRadians(1.0f);
constexpr mu_float ItemViewNear = 20.0f;
constexpr mu_float ItemViewFar = 2000.0f;

const mu_float MIN_CAMERA_RADIUS = 200.0f;
const mu_float MAX_CAMERA_RADIUS = 50000.0f;

const mu_float GameFOV = 35.0f;
const mu_float GameRFOV = EMath::ToRadians(GameFOV);
const mu_float LoginFOV = 65.0f;
const mu_float LoginRFOV = EMath::ToRadians(LoginFOV);

#if defined(__ANDROID__)
#define GAME_REVERSED_NEAR (1.0f)
#define GAME_REVERSED_FAR (5000.0f)
#define LOGIN_REVERSED_NEAR (1.0f)
#define LOGIN_REVERSED_FAR (5000.0f)
#define RAY_REVERSED_NEAR (10.0f)
#define RAY_REVERSED_FAR (5000.0f)
#define SHADOW_REVERSED_NEAR (5000.0f)
#define SHADOW_REVERSED_FAR (10.0f)
#else
#define GAME_REVERSED_NEAR (1.0f)
#define GAME_REVERSED_FAR (100000.0f)
#define LOGIN_REVERSED_NEAR (1.0f)
#define LOGIN_REVERSED_FAR (10000.0f)
#define RAY_REVERSED_NEAR (10.0f)
#define RAY_REVERSED_FAR (100000.0f)
#define SHADOW_REVERSED_NEAR (5000.0f)
#define SHADOW_REVERSED_FAR (10.0f)
#endif

#define GAME_NORMAL_NEAR (10.0f)
#define GAME_NORMAL_FAR (5000.0f)
#define LOGIN_NORMAL_NEAR (10.0f)
#define LOGIN_NORMAL_FAR (5000.0f)
#define RAY_NORMAL_NEAR (10.0f)
#define RAY_NORMAL_FAR (5000.0f)
#define SHADOW_NORMAL_NEAR (10.0f)
#define SHADOW_NORMAL_FAR (5000.0f)

const mu_float VkGameNearZ = (VULKAN_REVERSED_DEPTH ? GAME_REVERSED_NEAR : GAME_NORMAL_NEAR);
const mu_float VkGameFarZ = (VULKAN_REVERSED_DEPTH ? GAME_REVERSED_FAR : GAME_NORMAL_FAR);
const mu_float VkLoginNearZ = (VULKAN_REVERSED_DEPTH ? LOGIN_REVERSED_NEAR : LOGIN_NORMAL_NEAR);
const mu_float VkLoginFarZ = (VULKAN_REVERSED_DEPTH ? LOGIN_REVERSED_FAR : LOGIN_NORMAL_FAR);
const mu_float VkRayNearZ = (VULKAN_REVERSED_DEPTH ? RAY_REVERSED_NEAR : RAY_NORMAL_NEAR);
const mu_float VkRayFarZ = (VULKAN_REVERSED_DEPTH ? RAY_REVERSED_FAR : RAY_NORMAL_FAR);
const mu_float VkShadowNearZ = (VULKAN_REVERSED_DEPTH ? SHADOW_REVERSED_NEAR : SHADOW_NORMAL_NEAR);
const mu_float VkShadowFarZ = (VULKAN_REVERSED_DEPTH ? SHADOW_REVERSED_FAR : SHADOW_NORMAL_FAR);

const mu_float DXGameNearZ = (DIRECTX_REVERSED_DEPTH ? GAME_REVERSED_NEAR : GAME_NORMAL_NEAR);
const mu_float DXGameFarZ = (DIRECTX_REVERSED_DEPTH ? GAME_REVERSED_FAR : GAME_NORMAL_FAR);
const mu_float DXLoginNearZ = (DIRECTX_REVERSED_DEPTH ? LOGIN_REVERSED_NEAR : LOGIN_NORMAL_NEAR);
const mu_float DXLoginFarZ = (DIRECTX_REVERSED_DEPTH ? LOGIN_REVERSED_FAR : LOGIN_NORMAL_FAR);
const mu_float DXRayNearZ = (DIRECTX_REVERSED_DEPTH ? RAY_REVERSED_NEAR : RAY_NORMAL_NEAR);
const mu_float DXRayFarZ = (DIRECTX_REVERSED_DEPTH ? RAY_REVERSED_FAR : RAY_NORMAL_FAR);
const mu_float DXShadowNearZ = (DIRECTX_REVERSED_DEPTH ? SHADOW_REVERSED_NEAR : SHADOW_NORMAL_NEAR);
const mu_float DXShadowFarZ = (DIRECTX_REVERSED_DEPTH ? SHADOW_REVERSED_FAR : SHADOW_NORMAL_FAR);

const mu_float OGLGameNearZ = (OPENGL_REVERSED_DEPTH ? GAME_REVERSED_NEAR : GAME_NORMAL_NEAR);
const mu_float OGLGameFarZ = (OPENGL_REVERSED_DEPTH ? GAME_REVERSED_FAR : GAME_NORMAL_FAR);
const mu_float OGLLoginNearZ = (OPENGL_REVERSED_DEPTH ? LOGIN_REVERSED_NEAR : LOGIN_NORMAL_NEAR);
const mu_float OGLLoginFarZ = (OPENGL_REVERSED_DEPTH ? LOGIN_REVERSED_FAR : LOGIN_NORMAL_FAR);
const mu_float OGLRayNearZ = (OPENGL_REVERSED_DEPTH ? RAY_REVERSED_NEAR : RAY_NORMAL_NEAR);
const mu_float OGLRayFarZ = (OPENGL_REVERSED_DEPTH ? RAY_REVERSED_FAR : RAY_NORMAL_FAR);
const mu_float OGLShadowNearZ = (OPENGL_REVERSED_DEPTH ? SHADOW_REVERSED_NEAR : SHADOW_NORMAL_NEAR);
const mu_float OGLShadowFarZ = (OPENGL_REVERSED_DEPTH ? SHADOW_REVERSED_FAR : SHADOW_NORMAL_FAR);

const EVector3 UP(0.0f, 0.0f, 1.0f);
const EVector3 InverseUP = -UP;

MUOrbitCamera::MUOrbitCamera() : MUCamera(), _Radius(0.0f), _DefaultRadius(0.0f),
								 _CameraAngle(0.0f, 0.0f), _DefaultCameraAngle(0.0f, 0.0f),
								 _Update(true)
#if defined(WIN32) || defined(WIN64)
								, _RightClick(false),
								 _MouseDelta(0.0f, 0.0f), _MouseWheelDelta(0)
#endif
{
}

void MUOrbitCamera::Move(mu_double ElapsedTime, EVector3 &v)
{
	mu_double deltaTime = ElapsedTime / 1000.0;

	EQuat rotation(EVector3(0.0f, 0.0f, _CameraAngle.x));
	EMatrix4 direction = EMath::MakeMatrix(rotation);

	if (MU_INPUT->IsKeyHolding('A'))
	{
		v += EVector3(direction[0]);
	}
	else if (MU_INPUT->IsKeyHolding('D'))
	{
		v += -EVector3(direction[0]);
	}

	if (MU_INPUT->IsKeyHolding('W'))
	{
		v += -EVector3(direction[1]);
	}
	else if (MU_INPUT->IsKeyHolding('S'))
	{
		v += EVector3(direction[1]);
	}

	v *= 10.0f * deltaTime;
}

void MUOrbitCamera::Update(mu_double ElapsedTime, const EVector3 &Position)
{
#if defined(WIN32) || defined(WIN64)
	if(_Dragging == true &&
		(SDL_GetRelativeMouseMode() == SDL_FALSE ||
		EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Focused) == false))
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
		MU_INPUT->ShowCursor(true);
		_Dragging = false;
	}
#endif

	if(MU_INPUT->IsMouseOverInterface() == false)
	{
#if defined(WIN32) || defined(WIN64)
		mu_boolean shiftPressed = MU_INPUT->IsShiftPressing() == true;
		if (shiftPressed == true &&
			MU_INPUT->IsMousePressing(MOUSE_BUTTON_LEFT) == true)
		{
			if (_Dragging == false)
			{
				SDL_SetRelativeMouseMode(SDL_TRUE);
				SDL_GetRelativeMouseState(&_LastMousePosition.x, &_LastMousePosition.y);
				MU_INPUT->ShowCursor(false);
				_BackupMousePosition = _LastMousePosition;
				_Dragging = true;
			}
		}
		else
		{
			if (_Dragging == true)
			{
				SDL_SetRelativeMouseMode(SDL_FALSE);
				MU_INPUT->ShowCursor(true);
				_Dragging = false;
			}
		}

		if (shiftPressed == true &&
			MU_INPUT->IsMousePressing(MOUSE_BUTTON_RIGHT) == true)
		{
			if (_RightClick == false)
			{
				_RightClick = true;
				_CameraAngle = _DefaultCameraAngle;
				_Radius = _DefaultRadius;
			}
		}
		else
		{
			_RightClick = false;
		}

		if (shiftPressed == true &&
			MU_INPUT->GetMouseWheel() != 0.0f)
		{
			_MouseWheelDelta += (MU_INPUT->GetMouseWheel() * 100.0f);
		}

		if (_Dragging)
		{
			OnMove(ElapsedTime);
		}
#endif
	}

	// Update Matrices
	{
#if defined(WIN32) || defined(WIN64)
		if (_MouseWheelDelta != 0.0f)
		{
			_Radius -= _MouseWheelDelta * _Radius * 0.1f / 120.0f;

			_Radius = MU_MIN_MAX(_Radius, MIN_CAMERA_RADIUS, MAX_CAMERA_RADIUS);
			_MouseWheelDelta = 0.0f;
		}
#endif

		EVector2 CartAngle(_CameraAngle.x - EMath::PI * 0.5f, EMath::PI * 0.5f - (_CameraAngle.y - EMath::ToRadians(EarthQuake.Load())));
		mu_float xyRad = _Radius * EMath::Sin(CartAngle.y);
		EVector3 CameraRot = EVector3(xyRad * EMath::Cos(CartAngle.x), xyRad * EMath::Sin(CartAngle.x), _Radius * EMath::Cos(CartAngle.y));

		EVector3 PlayerPosition = Position;
		EVector3 EyePosition = PlayerPosition + CameraRot;

		EyePosition.x = MU_MIN_MAX(EyePosition.x, 0.0f, 255.0f * g_TerrainScale);
		EyePosition.y = MU_MIN_MAX(EyePosition.y, 0.0f, 255.0f * g_TerrainScale);

		mu_float Height = g_Terrain.GetTerrainHeight(EyePosition.x * g_TerrainScaleRev, EyePosition.y * g_TerrainScaleRev) + 100.0f;

		if (EyePosition.z < Height)
		{
			EyePosition.z = Height;
		}

		EVector3 shadowPosition;
		if (GET_WORLD == WD_30BATTLECASTLE)
		{
			shadowPosition = EVector3(1000.0f, 0.0f, 2000.0f);
		}
		else
		{
			shadowPosition = EVector3(1250.0f, 0.0f, 2000.0f);
		}

		EVector3 shadowBiasPosition = EVector3(0.0f, 500.0f, 0.0f);

		switch(EVideoBackend::GetBackend())
		{
		case EVideoBackendType::eDirectX:
			{
				MUVideoManager::_CurrentShadowProjection = EMathDX::ShadowOrthographic(SHADOWMAP_ORTHOWIDTH, SHADOWMAP_ORTHOHEIGHT, DXShadowNearZ, DXShadowFarZ);
				MUVideoManager::_CurrentProjection = EMathDX::PerspectiveFov(GameRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, DXGameNearZ, DXGameFarZ);
				MUVideoManager::_CurrentProjectionUI = EMathDX::PerspectiveFovUI((mu_float)ItemViewFOV, (mu_float)MU_RESOURCE->GetWindowWidth(), (mu_float)MU_RESOURCE->GetWindowHeight(), ItemViewNear, ItemViewFar);
				MUVideoManager::_CurrentRayProjection = EMathDX::RayPerspectiveFov(GameRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, DXRayNearZ, DXRayFarZ);
				_CameraNear = DXGameNearZ;
				_CameraFar = DXGameFarZ;
			}
			break;

		case EVideoBackendType::eOpenGL:
			{
				MUVideoManager::_CurrentShadowProjection = EMathOGL::ShadowOrthographic(SHADOWMAP_ORTHOWIDTH, SHADOWMAP_ORTHOHEIGHT, OGLShadowNearZ, OGLShadowFarZ);
				MUVideoManager::_CurrentProjection = EMathOGL::PerspectiveFov(GameRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, OGLGameNearZ, OGLGameFarZ);
				MUVideoManager::_CurrentProjectionUI = EMathOGL::PerspectiveFovUI((mu_float)ItemViewFOV, (mu_float)MU_RESOURCE->GetWindowWidth(), (mu_float)MU_RESOURCE->GetWindowHeight(), ItemViewNear, ItemViewFar);
				MUVideoManager::_CurrentRayProjection = EMathOGL::RayPerspectiveFov(GameRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, OGLRayNearZ, OGLRayFarZ);
				_CameraNear = OGLGameNearZ;
				_CameraFar = OGLGameFarZ;
			}
			break;

		case EVideoBackendType::eVulkan:
			{
				MUVideoManager::_CurrentShadowProjection = EMathVK::ShadowOrthographic(SHADOWMAP_ORTHOWIDTH, SHADOWMAP_ORTHOHEIGHT, VkShadowNearZ, VkShadowFarZ);
				MUVideoManager::_CurrentProjection = EMathVK::PerspectiveFov(GameRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, VkGameNearZ, VkGameFarZ);
				MUVideoManager::_CurrentProjectionUI = EMathVK::PerspectiveFovUI((mu_float)ItemViewFOV, (mu_float)MU_RESOURCE->GetWindowWidth(), (mu_float)MU_RESOURCE->GetWindowHeight(), ItemViewNear, ItemViewFar);
				MUVideoManager::_CurrentRayProjection = EMathVK::RayPerspectiveFov(GameRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, VkRayNearZ, VkRayFarZ);
				_CameraNear = VkGameNearZ;
				_CameraFar = VkGameFarZ;
			}
			break;

		default: break;
		}

		MUVideoManager::_CurrentShadowView = EMath::LookAt(PlayerPosition + shadowPosition + shadowBiasPosition, PlayerPosition + shadowBiasPosition, UP);

		MUVideoManager::_CurrentView = EMath::LookAt(EyePosition, PlayerPosition, UP);
		MUVideoManager::_CurrentInverseView = EMath::Inverse(MUVideoManager::_CurrentView);

		_CameraPosition = EyePosition;
		_FocusPosition = PlayerPosition;
		_CameraDirection = EVector3(MUVideoManager::_CurrentInverseView[2]);
		_CameraRight = EVector3(MUVideoManager::_CurrentInverseView[0]);
		_CameraUp = EVector3(MUVideoManager::_CurrentInverseView[1]);

		MUVideoManager::_CurrentTextView = EMath::CreateBillboard(-GetCameraInverseUp(), GetCameraInverseRight(), -GetCameraInverseDirection());
#ifdef USE_DUAL_QUATERNION
		MUVideoManager::_CurrentTextViewDQ = EMath::CreateBillboardDQ(GetCameraInverseUp(), GetCameraInverseRight(), GetCameraInverseDirection());
#endif

#if 0
		printf("{%.08f, %.08f, %.08f}\n{%.08f, %.08f, %.08f}\n", EyePosition.x, EyePosition.y, EyePosition.z,
			PlayerPosition.x, PlayerPosition.y, PlayerPosition.z);
#endif
#if 0
		printf("{%.6f\t%.6f\t%.6f\t%.6f}\n" \
			"{%.6f\t%.6f\t%.6f\t%.6f}\n" \
			"{%.6f\t%.6f\t%.6f\t%.6f}\n" \
			"{%.6f\t%.6f\t%.6f\t%.6f}\n" \
			"---------------------------------------------------\n",
			MUVideoManager::_CurrentInverseView[0][0], MUVideoManager::_CurrentInverseView[0][1], MUVideoManager::_CurrentInverseView[0][2], MUVideoManager::_CurrentInverseView[0][3],
			MUVideoManager::_CurrentInverseView[1][0], MUVideoManager::_CurrentInverseView[1][1], MUVideoManager::_CurrentInverseView[1][2], MUVideoManager::_CurrentInverseView[1][3],
			MUVideoManager::_CurrentInverseView[2][0], MUVideoManager::_CurrentInverseView[2][1], MUVideoManager::_CurrentInverseView[2][2], MUVideoManager::_CurrentInverseView[2][3],
			MUVideoManager::_CurrentInverseView[3][0], MUVideoManager::_CurrentInverseView[3][1], MUVideoManager::_CurrentInverseView[3][2], MUVideoManager::_CurrentInverseView[3][3]);
#endif
	}
}

void MUOrbitCamera::UpdateMouseDelta()
{
#if defined(WIN32) || defined(WIN64)
	// Get current position of mouse
	EIPoint MousePosition;
	SDL_GetRelativeMouseState(&MousePosition.x, &MousePosition.y);

	// Calc how far it's moved since last frame
	EIPoint MouseDelta;
	MouseDelta.x = MousePosition.x;
	MouseDelta.y = MousePosition.y;

	// Smooth the relative mouse data over a few frames so it isn't 
	// jerky when moving slowly at low frame rates.
	mu_float PercentOfNew = 0.5f;
	mu_float PercentOfOld = 1.0f - PercentOfNew;
	_MouseDelta.x = _MouseDelta.x * PercentOfOld + MouseDelta.x * PercentOfNew;
	_MouseDelta.y = _MouseDelta.y * PercentOfOld + MouseDelta.y * PercentOfNew;

	_RotVelocity.x = _MouseDelta.x * 0.01f;
	_RotVelocity.y = _MouseDelta.y * 0.01f;
#endif
}

void MUOrbitCamera::OnMove(mu_double ElapsedTime)
{
#if defined(WIN32) || defined(WIN64)
	UpdateMouseDelta();

	_CameraAngle.x += _RotVelocity.x;
	_CameraAngle.y += _RotVelocity.y;

	_CameraAngle.y = MU_MIN_MAX(_CameraAngle.y, EMath::ToRadians(100.0f), EMath::ToRadians(180.0f));
#endif
}

/*
	Walk Camera(Used for Login Animation)
*/
MUWalkCamera::MUWalkCamera() : MUCamera(), _Walkpoints(nullptr), _WalkpointsCount(0), _CurrentIndex(0), _CurrentElapse(0.0f)
{

}

MUWalkCamera::~MUWalkCamera()
{
	Unload();
}

mu_boolean MUWalkCamera::Load(const mu_text *filename)
{
	Unload();

	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, filename, "rb") == false)
	{
		return false;
	}

	static mu_int64 WalkScriptMinimumSize = sizeof(mu_uint32) + sizeof(mu_uint32) + sizeof(WALKPOINT);

	mu_int64 fileLength = SDL_RWsize(file);

	if (fileLength < WalkScriptMinimumSize)
	{
		return false;
	}

	mu_uint32 Signature, Count;
	SDL_RWread(file, (mu_char*)&Signature, sizeof(Signature), 1);
	SDL_RWread(file, (mu_char*)&Count, sizeof(Count), 1);

	if (Count <= 1 ||
		fileLength < WalkScriptMinimumSize + (Count - 1) * sizeof(WALKPOINT))
	{
		return false;
	}

	MU_SAFE_DELETE_ARRAY(_Walkpoints);
	_Walkpoints = new WALKPOINTv2[Count];
	_WalkpointsCount = Count;

	for (mu_uint32 i = 0; i < Count; ++i)
	{
		WALKPOINTv2 *walkpoint = &_Walkpoints[i];

		SDL_RWread(file, (char*)&(*static_cast<WALKPOINT*>(walkpoint)), sizeof(WALKPOINT), 1);

		walkpoint->DelayTime = (mu_double)walkpoint->Delay * WALK_FPS;
	}

	mu_uint32 targetIndex = _CurrentIndex < _WalkpointsCount ? _CurrentIndex : 0;
	WALKPOINTv2 &targetWalkpoint = _Walkpoints[targetIndex];
	mu_uint32 originIndex = _CurrentIndex > 0 ? _CurrentIndex - 1 : _WalkpointsCount - 1;
	WALKPOINTv2 &originWalkpoint = _Walkpoints[originIndex];

	_TourCameraPosition = _CurrentCameraPosition = EVector3(originWalkpoint.CameraPosition, -300.0f);
	_BackupTourCameraPosition = _BackupCameraPosition = _CurrentCameraPosition;

	EVector2 subVector = targetWalkpoint.CameraPosition - originWalkpoint.CameraPosition;
	EFloat subDistance = EMath::Length(subVector);
	EVector2 subDir = subVector / subDistance;

	_TargetTourCameraAngle = _TourCameraAngle = CreateAngle(0.0f, 0.0f, subDir.x, -subDir.y);
	_BackupTourCameraAngle = _TourCameraAngle;
	_TourCameraDistance = 0.0f;

	SDL_RWclose(file);

	Update(GAME_FPS, EVector3());

	return true;
}

void MUWalkCamera::Unload()
{
	MU_SAFE_DELETE_ARRAY(_Walkpoints);
	_WalkpointsCount = 0;

	_CurrentIndex = 1;
	_CurrentElapse = 0.0f;
#ifdef USE_WALK_OLDCODE
	_DelayCount = 0;
	_CameraElapse = 0.0f;
#else
	_CameraElapse = 0.0;
	_IsDelayDisabled = false;
#endif
}

void MUWalkCamera::Update(mu_double ElapsedTime, const EVector3 &Position)
{
#ifdef USE_WALK_OLDCODE
	_CameraElapse += (mu_float)ElapsedTime;
	mu_uint32 updateCount = (mu_uint32)(_CameraElapse / GAME_FPS);
	_CameraElapse -= GAME_FPS * updateCount;

	for(;updateCount > 0; --updateCount)
	{
		mu_uint32 targetIndex = _CurrentIndex < _WalkpointsCount ? _CurrentIndex : 0;
		WALKPOINTv2 &targetWalkpoint = _Walkpoints[targetIndex];
		mu_uint32 originIndex = _CurrentIndex > 0 ? _CurrentIndex - 1 : _WalkpointsCount - 1;
		WALKPOINTv2 &originWalkpoint = _Walkpoints[originIndex];

		if (_DelayCount >= targetWalkpoint.Delay)
		{
			EVector2 subVector = targetWalkpoint.CameraPosition - EVector2(_CurrentCameraPosition);
			EFloat subDistance = EMath::Length(subVector);
			EVector2 subDir = subVector / subDistance;

			EVector2 originVector = originWalkpoint.CameraPosition - EVector2(_CurrentCameraPosition);
			EFloat originDistance = EMath::Length(originVector);
			EVector2 originDir = originVector / originDistance;

			EFloat blendDistance = 300.0f;
			EVector2 tourDir = subDir;

			if (subDistance <= blendDistance)
			{
				mu_uint32 nextIndex = targetIndex + 1 < _WalkpointsCount ? targetIndex + 1 : 0;
				WALKPOINTv2 &nextWalkpoint = _Walkpoints[nextIndex];

				EVector2 nextSubVector = nextWalkpoint.CameraPosition - targetWalkpoint.CameraPosition;
				EFloat nextSubDistance = EMath::Length(nextSubVector);
				EVector2 nextDirVector = nextSubVector / nextSubDistance;

				EFloat blendRate = subDistance / blendDistance * 0.5f + 0.5f;
				tourDir = EMath::Lerp(nextDirVector, subDir, blendRate);
			}
			else if (originDistance <= blendDistance)
			{
				mu_uint32 previousIndex = originIndex > 0 ? originIndex - 1 : _WalkpointsCount - 1;
				WALKPOINTv2 &previousWalkpoint = _Walkpoints[previousIndex];

				EVector2 previousOriginVector = originWalkpoint.CameraPosition - previousWalkpoint.CameraPosition;
				EFloat previousOriginDistance = EMath::Length(previousOriginVector);
				EVector2 previousDirVector = previousOriginVector / previousOriginDistance;

				EFloat blendRate = originDistance / blendDistance * 0.5f + 0.5f;
				tourDir = EMath::Lerp(previousDirVector, subDir, blendRate);
			}

			if (subDistance <= targetWalkpoint.CameraMove)
			{
				_CurrentIndex = (_CurrentIndex + 1) % _WalkpointsCount;
				_DelayCount = 0;
				++updateCount;
				continue;
			}

			{
				if (subDistance > 5.0f && originDistance > 5.0f)
				{
					_TargetTourCameraAngle = CreateAngle(0.0f, 0.0f, tourDir.x, -tourDir.y);
				}

				EFloat rotDir = 0.0f;
				EFloat angleDistance = 0.0f;
				EFloat angleTest = EMath::Abs(_TargetTourCameraAngle - _TourCameraAngle);

				if (angleTest > 180.0f)
				{
					rotDir = (_TargetTourCameraAngle - _TourCameraAngle > 0.0f ? -1.0f : 1.0f);
					angleDistance = 360.0f - angleTest;
				}
				else
				{
					rotDir = (_TargetTourCameraAngle - _TourCameraAngle > 0.0f ? 1.0f : -1.0f);
					angleDistance = angleTest;
				}

				static const EFloat maxRotateSpeed = 1.0f;
				EFloat rotateSpeed = 0.0f;

				if (angleDistance > 0.0f)
				{
					rotateSpeed = angleDistance / 30.0f;
				}

				if (rotateSpeed > maxRotateSpeed)
				{
					rotateSpeed = maxRotateSpeed;
				}

				if (angleDistance <= rotateSpeed)
				{
					_TourCameraAngle = _TargetTourCameraAngle;
				}
				else
				{
					_TourCameraAngle += rotateSpeed * rotDir;
				}

				if (_TourCameraAngle < 0.0f)
				{
					_TourCameraAngle += 360.0f;
				}
				else if (_TourCameraAngle > 360.0f)
				{
					_TourCameraAngle -= 360.0f;
				}
			}

			_CurrentCameraPosition.x += subDir.x * targetWalkpoint.CameraMove;
			_CurrentCameraPosition.y += subDir.y * targetWalkpoint.CameraMove;

			_TourCameraPosition.x += tourDir.x * targetWalkpoint.CameraMove;
			_TourCameraPosition.y += tourDir.y * targetWalkpoint.CameraMove;

			_TourCameraDistance = originWalkpoint.CameraDistance * subDistance / (originDistance + subDistance) +
									targetWalkpoint.CameraDistance * originDistance / (originDistance + subDistance);
			_TourCameraDistance = 1100.0f * _TourCameraDistance * 0.1f;
		}

		++_DelayCount;
	}
#else
	_CameraElapse += (mu_float)ElapsedTime;

	do
	{
		mu_uint32 targetIndex = _CurrentIndex < _WalkpointsCount ? _CurrentIndex : 0;
		WALKPOINTv2 &targetWalkpoint = _Walkpoints[targetIndex];
		mu_uint32 originIndex = _CurrentIndex > 0 ? _CurrentIndex - 1 : _WalkpointsCount - 1;
		WALKPOINTv2 &originWalkpoint = _Walkpoints[originIndex];

		mu_uint32 fpsTimes = (mu_uint32)(_CameraElapse / GAME_FPS);

		if (_IsDelayDisabled == true ||
			_CameraElapse >= targetWalkpoint.Delay)
		{
			if (_IsDelayDisabled == false)
			{
				_CameraElapse -= targetWalkpoint.DelayTime;
				fpsTimes = (mu_uint32)(_CameraElapse / GAME_FPS);
				_IsDelayDisabled = true;
			}

			mu_float fpsPercent;
			if (fpsTimes >= 1)
			{
				fpsPercent = 1.0f;
			}
			else
			{
				fpsPercent = _CameraElapse / GAME_FPS;
			}

			EVector2 subVector = targetWalkpoint.CameraPosition - EVector2(_BackupCameraPosition);
			EFloat subDistance = EMath::Length(subVector);
			EVector2 subDir = (subVector / subDistance) * fpsPercent;

			EVector2 originVector = originWalkpoint.CameraPosition - EVector2(_BackupCameraPosition);
			EFloat originDistance = EMath::Length(originVector);
			//EVector2 originDir = (originVector / originDistance) * fpsPercent;

			EFloat blendDistance = 300.0f;
			EVector2 tourDir = subDir;

			if (subDistance <= blendDistance)
			{
				mu_uint32 nextIndex = targetIndex + 1 < _WalkpointsCount ? targetIndex + 1 : 0;
				WALKPOINTv2 &nextWalkpoint = _Walkpoints[nextIndex];

				EVector2 nextSubVector = nextWalkpoint.CameraPosition - targetWalkpoint.CameraPosition;
				EFloat nextSubDistance = EMath::Length(nextSubVector);
				EVector2 nextDirVector = (nextSubVector / nextSubDistance) * fpsPercent;

				EFloat blendRate = (subDistance / blendDistance * 0.5f + 0.5f);
				tourDir = EMath::Lerp(nextDirVector, subDir, blendRate);
			}
			else if (originDistance <= blendDistance)
			{
				mu_uint32 previousIndex = originIndex > 0 ? originIndex - 1 : _WalkpointsCount - 1;
				WALKPOINTv2 &previousWalkpoint = _Walkpoints[previousIndex];

				EVector2 previousOriginVector = originWalkpoint.CameraPosition - previousWalkpoint.CameraPosition;
				EFloat previousOriginDistance = EMath::Length(previousOriginVector);
				EVector2 previousDirVector = (previousOriginVector / previousOriginDistance) * fpsPercent;

				EFloat blendRate = (originDistance / blendDistance * 0.5f + 0.5f);
				tourDir = EMath::Lerp(previousDirVector, subDir, blendRate);
			}

			if (fpsTimes >= 1)
			{
				_CameraElapse -= GAME_FPS;
			}

			if (subDistance <= targetWalkpoint.CameraMove)
			{
				_CurrentIndex = (_CurrentIndex + 1) % _WalkpointsCount;
				_IsDelayDisabled = false;
				_CameraElapse += GAME_FPS;
				continue;
			}

			if (fpsTimes >= 1 &&
				subDistance > 5.0f &&
				originDistance > 5.0f)
			{
				_TargetTourCameraAngle = CreateAngle(0.0f, 0.0f, tourDir.x, -tourDir.y);
			}

			EFloat rotDir = 0.0f;
			EFloat angleDistance = 0.0f;
			EFloat angleTest = EMath::Abs(_TargetTourCameraAngle - _BackupTourCameraAngle);

			if (angleTest > 180.0f)
			{
				rotDir = ((_TargetTourCameraAngle - _BackupTourCameraAngle) > 0.0f ? -1.0f : 1.0f);
				angleDistance = 360.0f - angleTest;
			}
			else
			{
				rotDir = ((_TargetTourCameraAngle - _BackupTourCameraAngle) > 0.0f ? 1.0f : -1.0f);
				angleDistance = angleTest;
			}

			angleDistance *= fpsPercent;

			static const EFloat maxRotateSpeed = 1.0f;
			EFloat rotateSpeed = 0.0f;

			if (angleDistance > 0.0f)
			{
				rotateSpeed = angleDistance / 30.0f;
			}

			if (rotateSpeed > maxRotateSpeed)
			{
				rotateSpeed = maxRotateSpeed;
			}

			_TourCameraAngle = _BackupTourCameraAngle;

			if (fpsTimes >= 1 &&
				angleDistance <= rotateSpeed)
			{
				_TourCameraAngle = _TargetTourCameraAngle;
			}
			else
			{
				_TourCameraAngle += rotateSpeed * rotDir;
			}

			if (_TourCameraAngle < 0.0f)
			{
				_TourCameraAngle += 360.0f;
			}
			else if (_TourCameraAngle > 360.0f)
			{
				_TourCameraAngle -= 360.0f;
			}

			if (fpsTimes >= 1)
			{
				_BackupTourCameraAngle = _TourCameraAngle;
			}

			if (fpsTimes >= 1)
			{
				_BackupCameraPosition.x += subDir.x * targetWalkpoint.CameraMove;
				_BackupCameraPosition.y += subDir.y * targetWalkpoint.CameraMove;

				_BackupTourCameraPosition.x += tourDir.x * targetWalkpoint.CameraMove;
				_BackupTourCameraPosition.y += tourDir.y * targetWalkpoint.CameraMove;

				_CurrentCameraPosition = _BackupCameraPosition;
				_TourCameraPosition = _BackupTourCameraPosition;
			}
			else
			{
				_CurrentCameraPosition = _BackupCameraPosition;
				_TourCameraPosition = _BackupTourCameraPosition;

				_CurrentCameraPosition.x += subDir.x * targetWalkpoint.CameraMove;
				_CurrentCameraPosition.y += subDir.y * targetWalkpoint.CameraMove;

				_TourCameraPosition.x += tourDir.x * targetWalkpoint.CameraMove;
				_TourCameraPosition.y += tourDir.y * targetWalkpoint.CameraMove;
			}

			_TourCameraDistance = originWalkpoint.CameraDistance * subDistance / (originDistance + subDistance) +
									targetWalkpoint.CameraDistance * originDistance / (originDistance + subDistance);
			_TourCameraDistance = 1100.0f * _TourCameraDistance * 0.1f;
		}
	}
	while (_CameraElapse >= GAME_FPS);
#endif

	// Update Matrices
	{
		static const mu_float CameraAngleX = -EMath::ToRadians(-112.5f);

		EQuat cameraRotation = EQuat(EVector3(0.0f, 0.0f, EMath::ToRadians(_TourCameraAngle)));
		EVector3 transformPosition = EMath::InverseTransform(EMath::MakeMatrix(cameraRotation), EVector3(0.0f, -_TourCameraDistance, 0.0f));
		transformPosition += EVector3(0.0f, 0.0f, -100.0f);

		_CameraPosition = _TourCameraPosition;
		_CameraPosition += transformPosition;
		_CameraPosition.z += _TourCameraDistance - 150.0f;

		EVector3 focusPosition = _CameraPosition + EQuat(EVector3(CameraAngleX, 0.0f, EMath::ToRadians(-_TourCameraAngle))) * InverseUP;

		EVector3 shadowPosition;
		if (GET_WORLD == WD_30BATTLECASTLE)
		{
			shadowPosition = EVector3(2000.0f, 0.0f, 4000.0f);
		}
		else
		{
			shadowPosition = EVector3(1250.0f, 0.0f, 2000.0f);
		}

		EVector3 shadowBiasPosition = EVector3(0.0f, 500.0f, 0.0f);
		
		switch (EVideoBackend::GetBackend())
		{
		case EVideoBackendType::eDirectX:
			{
				MUVideoManager::_CurrentShadowProjection = EMathDX::ShadowOrthographic(SHADOWMAP_ORTHOWIDTH, SHADOWMAP_ORTHOHEIGHT, DXShadowNearZ, DXShadowFarZ);
				MUVideoManager::_CurrentProjection = EMathDX::PerspectiveFov(LoginRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, DXLoginNearZ, DXLoginFarZ);
				MUVideoManager::_CurrentProjectionUI = EMathDX::PerspectiveFovUI((mu_float)ItemViewFOV, (mu_float)MU_RESOURCE->GetWindowWidth(), (mu_float)MU_RESOURCE->GetWindowHeight(), ItemViewNear, ItemViewFar);
				MUVideoManager::_CurrentRayProjection = EMathDX::RayPerspectiveFov(LoginRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, DXRayNearZ, DXRayFarZ);
				_CameraNear = DXLoginNearZ;
				_CameraFar = DXLoginFarZ;
			}
			break;

		case EVideoBackendType::eOpenGL:
			{
				MUVideoManager::_CurrentShadowProjection = EMathOGL::ShadowOrthographic(SHADOWMAP_ORTHOWIDTH, SHADOWMAP_ORTHOHEIGHT, OGLShadowNearZ, OGLShadowFarZ);
				MUVideoManager::_CurrentProjection = EMathOGL::PerspectiveFov(LoginRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, OGLLoginNearZ, OGLLoginFarZ);
				MUVideoManager::_CurrentProjectionUI = EMathOGL::PerspectiveFovUI((mu_float)ItemViewFOV, (mu_float)MU_RESOURCE->GetWindowWidth(), (mu_float)MU_RESOURCE->GetWindowHeight(), ItemViewNear, ItemViewFar);
				MUVideoManager::_CurrentRayProjection = EMathOGL::RayPerspectiveFov(LoginRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, OGLRayNearZ, OGLRayFarZ);
				_CameraNear = OGLLoginNearZ;
				_CameraFar = OGLLoginFarZ;
			}
			break;

		case EVideoBackendType::eVulkan:
			{
				MUVideoManager::_CurrentShadowProjection = EMathVK::ShadowOrthographic(SHADOWMAP_ORTHOWIDTH, SHADOWMAP_ORTHOHEIGHT, VkShadowNearZ, VkShadowFarZ);
				MUVideoManager::_CurrentProjection = EMathVK::PerspectiveFov(LoginRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, VkLoginNearZ, VkLoginFarZ);
				MUVideoManager::_CurrentProjectionUI = EMathVK::PerspectiveFovUI((mu_float)ItemViewFOV, (mu_float)MU_RESOURCE->GetWindowWidth(), (mu_float)MU_RESOURCE->GetWindowHeight(), ItemViewNear, ItemViewFar);
				MUVideoManager::_CurrentRayProjection = EMathVK::RayPerspectiveFov(LoginRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, VkRayNearZ, VkRayFarZ);
				_CameraNear = VkLoginNearZ;
				_CameraFar = VkLoginFarZ;
			}
			break;

		default: break;
		}

		MUVideoManager::_CurrentShadowView = EMath::LookAt(focusPosition + shadowPosition + shadowBiasPosition, focusPosition + shadowBiasPosition, UP);

		MUVideoManager::_CurrentView = EMath::LookAt(_CameraPosition, focusPosition, UP);
		MUVideoManager::_CurrentInverseView = EMath::Inverse(MUVideoManager::_CurrentView);

		_CameraDirection = EVector3(MUVideoManager::_CurrentInverseView[2]);
		_CameraRight = EVector3(MUVideoManager::_CurrentInverseView[0]);
		_CameraUp = EVector3(MUVideoManager::_CurrentInverseView[1]);

		MUVideoManager::_CurrentTextView = EMath::CreateBillboard(-GetCameraInverseUp(), GetCameraInverseRight(), -GetCameraInverseDirection());
#ifdef USE_DUAL_QUATERNION
		MUVideoManager::_CurrentTextViewDQ = EMath::CreateBillboardDQ(GetCameraInverseUp(), GetCameraInverseRight(), GetCameraInverseDirection());
#endif
	}
}

/*
	Basic Camera(Used for Static Camera)
*/
MUBasicCamera::MUBasicCamera() : MUCamera()
{

}

MUBasicCamera::~MUBasicCamera()
{

}

void MUBasicCamera::Update(mu_double ElapsedTime, const EVector3 &Position)
{
	// Update Matrices
	{
		EVector3 focusPosition = _CameraPosition + EQuat(EMath::ToRadians(-_Angle)) * InverseUP;

		EVector3 shadowPosition;
		if (GET_WORLD == WD_30BATTLECASTLE)
		{
			shadowPosition = EVector3(2000.0f, 0.0f, 4000.0f);
		}
		else
		{
			shadowPosition = EVector3(1250.0f, 0.0f, 2000.0f);
		}

		EVector3 shadowBiasPosition;
		if (g_CurrentScene == CHARACTER_SCENE)
		{
			shadowBiasPosition = EVector3(-1250.0f, 500.0f, 0.0f);
		}
		else
		{
			shadowBiasPosition = EVector3(0.0f, 500.0f, 0.0f);
		}

		switch (EVideoBackend::GetBackend())
		{
		case EVideoBackendType::eDirectX:
			{
				MUVideoManager::_CurrentShadowProjection = EMathDX::ShadowOrthographic(SHADOWMAP_ORTHOWIDTH, SHADOWMAP_ORTHOHEIGHT, DXShadowNearZ, DXShadowFarZ);
				MUVideoManager::_CurrentProjection = EMathDX::PerspectiveFov(GameRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, DXGameNearZ, DXGameFarZ);
				MUVideoManager::_CurrentProjectionUI = EMathDX::PerspectiveFovUI((mu_float)ItemViewFOV, (mu_float)MU_RESOURCE->GetWindowWidth(), (mu_float)MU_RESOURCE->GetWindowHeight(), ItemViewNear, ItemViewFar);
				MUVideoManager::_CurrentRayProjection = EMathDX::RayPerspectiveFov(GameRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, DXRayNearZ, DXRayFarZ);
				_CameraNear = DXGameNearZ;
				_CameraFar = DXGameFarZ;
			}
			break;

		case EVideoBackendType::eOpenGL:
			{
				MUVideoManager::_CurrentShadowProjection = EMathOGL::ShadowOrthographic(SHADOWMAP_ORTHOWIDTH, SHADOWMAP_ORTHOHEIGHT, OGLShadowNearZ, OGLShadowFarZ);
				MUVideoManager::_CurrentProjection = EMathOGL::PerspectiveFov(GameRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, OGLGameNearZ, OGLGameFarZ);
				MUVideoManager::_CurrentProjectionUI = EMathOGL::PerspectiveFovUI((mu_float)ItemViewFOV, (mu_float)MU_RESOURCE->GetWindowWidth(), (mu_float)MU_RESOURCE->GetWindowHeight(), ItemViewNear, ItemViewFar);
				MUVideoManager::_CurrentRayProjection = EMathOGL::RayPerspectiveFov(GameRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, OGLRayNearZ, OGLRayFarZ);
				_CameraNear = OGLGameNearZ;
				_CameraFar = OGLGameFarZ;
			}
			break;

		case EVideoBackendType::eVulkan:
			{
				MUVideoManager::_CurrentShadowProjection = EMathVK::ShadowOrthographic(SHADOWMAP_ORTHOWIDTH, SHADOWMAP_ORTHOHEIGHT, VkShadowNearZ, VkShadowFarZ);
				MUVideoManager::_CurrentProjection = EMathVK::PerspectiveFov(GameRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, VkGameNearZ, VkGameFarZ);
				MUVideoManager::_CurrentProjectionUI = EMathVK::PerspectiveFovUI((mu_float)ItemViewFOV, (mu_float)MU_RESOURCE->GetWindowWidth(), (mu_float)MU_RESOURCE->GetWindowHeight(), ItemViewNear, ItemViewFar);
				MUVideoManager::_CurrentRayProjection = EMathVK::RayPerspectiveFov(GameRFOV, MUVideoManager::_3DViewport.Width, MUVideoManager::_3DViewport.Height, VkRayNearZ, VkRayFarZ);
				_CameraNear = VkGameNearZ;
				_CameraFar = VkGameFarZ;
			}
			break;

		default: break;
		}

		MUVideoManager::_CurrentShadowView = EMath::LookAt(focusPosition + shadowPosition + shadowBiasPosition, focusPosition + shadowBiasPosition, UP);

		MUVideoManager::_CurrentView = EMath::LookAt(_Position, focusPosition, UP);
		MUVideoManager::_CurrentInverseView = EMath::Inverse(MUVideoManager::_CurrentView);

		_CameraPosition = _Position;
		_CameraDirection = EVector3(MUVideoManager::_CurrentInverseView[2]);
		_CameraRight = EVector3(MUVideoManager::_CurrentInverseView[0]);
		_CameraUp = EVector3(MUVideoManager::_CurrentInverseView[1]);

		MUVideoManager::_CurrentTextView = EMath::CreateBillboard(-GetCameraInverseUp(), GetCameraInverseRight(), -GetCameraInverseDirection());
#ifdef USE_DUAL_QUATERNION
		MUVideoManager::_CurrentTextViewDQ = EMath::CreateBillboardDQ(GetCameraInverseUp(), GetCameraInverseRight(), GetCameraInverseDirection());
#endif
	}
}