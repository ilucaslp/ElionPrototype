#ifndef __MU_CAMERA_H__
#define __MU_CAMERA_H__

#pragma once

#define WALK_FPS (1000.0f/21.0f)
//#define USE_WALK_OLDCODE

extern const mu_float MIN_CAMERA_RADIUS;
extern const mu_float MAX_CAMERA_RADIUS;

extern const mu_float GameFOV;
extern const mu_float GameRFOV;
extern const mu_float LoginFOV;
extern const mu_float LoginRFOV;

class MUTerrain;

class MUCamera
{
public:
	MUCamera()
	{
		_Dragging = false;
	}

	virtual void Update(mu_double ElapsedTime, const EVector3 &Position) {}

public:
	EVector3 &GetCameraPosition()
	{
		return _CameraPosition;
	}

	EVector3 &GetCameraInverseDirection()
	{
		return _CameraDirection;
	}

	EVector3 &GetCameraInverseUp()
	{
		return _CameraUp;
	}

	EVector3 &GetCameraInverseRight()
	{
		return _CameraRight;
	}

	mu_float GetNear()
	{
		return _CameraNear;
	}

	mu_float GetFar()
	{
		return _CameraFar;
	}

	mu_boolean IsDragging()
	{
		return _Dragging;
	}

protected:
	mu_boolean _Dragging;
	EVector3 _CameraPosition, _CameraDirection, _CameraUp, _CameraRight;
	mu_float _CameraNear, _CameraFar;
};

class MUOrbitCamera : public MUCamera
{
public:
	enum
	{
		CAMERA_WIDTH_LIMIT = 360,
		CAMERA_HEIGHT_LIMIT = 160,
	};

public:
	MUOrbitCamera();

	void Move(mu_double ElapsedTime, EVector3 &v);
	virtual void Update(mu_double ElapsedTime, const EVector3 &Position) override;

	void ForceUpdate()
	{
		_Update = true;
	}

	void SetAngles(mu_float X, mu_float Y)
	{
		_CameraAngle.x = EMath::ToRadians(X);
		_CameraAngle.y = EMath::ToRadians(Y);
		_DefaultCameraAngle = _CameraAngle;
	}

	void SetRadius(mu_float Radius)
	{
		_Radius = MU_MIN_MAX(Radius, MIN_CAMERA_RADIUS, MAX_CAMERA_RADIUS);
		_DefaultRadius = _Radius;
	}

	EVector3 &GetFocusPosition()
	{
		return _FocusPosition;
	}

	void SetFocusPosition(EVector3 &Position)
	{
		_FocusPosition = Position;
	}

protected:
	void UpdateMouseDelta();
	void OnMove(mu_double ElapsedTime);

private:
	mu_float _Radius, _DefaultRadius;
	EVector2 _CameraAngle, _DefaultCameraAngle;

	mu_boolean _Update;
#if defined(WIN32) || defined(WIN64)
	mu_boolean _RightClick;
	EIPoint _LastMousePosition;
	EIPoint _BackupMousePosition;
	EVector2 _MouseDelta, _RotVelocity;
	mu_float _MouseWheelDelta;
#endif

	EVector3 _FocusPosition;
};

class MUWalkCamera : public MUCamera
{
public:
	MUWalkCamera();
	~MUWalkCamera();

	mu_boolean Load(const mu_text *filename);
	void Unload();

	virtual void Update(mu_double ElapsedTime, const EVector3 &Position) override;

private:
#pragma pack(push,1)
	class ARM_PACKED WALKPOINT
	{
	public:
		mu_int32 Index;

		EVector2 CameraPosition;
		EFloat CameraHeight;

		mu_int32 Delay;
		mu_float CameraMove;
		mu_float CameraDistance;
	};
	class ARM_PACKED WALKPOINTv2 : public WALKPOINT
	{
	public:
		mu_double DelayTime;
	};
#pragma pack(pop)

	WALKPOINTv2 *_Walkpoints;
	mu_uint32 _WalkpointsCount;

#ifndef USE_WALK_OLDCODE
	EVector3 _BackupCameraPosition;
	EVector3 _BackupTourCameraPosition;
	EFloat _BackupTourCameraAngle;
#endif
	EVector3 _CurrentCameraPosition;
	EVector3 _TourCameraPosition;
	EFloat _TourCameraAngle;
	EFloat _TargetTourCameraAngle;
	EFloat _TourCameraDistance;

	mu_uint32 _CurrentIndex;
	mu_float _CurrentElapse;
#ifdef USE_WALK_OLDCODE
	mu_int32 _DelayCount;
	mu_float _CameraElapse;
#else
	mu_double _CameraElapse;
	mu_boolean _IsDelayDisabled;
#endif
};

class MUBasicCamera : public MUCamera
{
public:
	MUBasicCamera();
	~MUBasicCamera();

	virtual void Update(mu_double ElapsedTime, const EVector3 &Position) override;

public:
	EINLINE void SetPosition(const EVector3 &Position)
	{
		_Position = Position;
	}

	EINLINE void SetAngle(const EVector3 &Angle)
	{
		_Angle = Angle;
	}

private:
	EVector3 _Position;
	EVector3 _Angle;
};

#endif