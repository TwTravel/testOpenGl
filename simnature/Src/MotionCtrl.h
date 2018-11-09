#ifndef _MOTION_CTRL_H_
#define _MOTION_CTRL_H_

#include "Camera.h"
#include "Terrain.h"
#include "Input.h"
#include "Timer.h"
class CMotionCtrl  
{
public:
	void TraceHeight(BOOL flag);
	void SetCameraByMouse();
	void TestCameraH();
	void SetSkyHeight(float h);
	void AutoPilot();
	void CtrledMove();
	void SetCameraHeight(float h);
	float GetCameraHeight();
	RECT GetRegion();
	void SetRegion(RECT rect);
	float GetAngleVelocity();
	float GetVelocity();
	void SetAngleVelocity(float av);
	void SetVelocity(float v);
	void SetTerrain(CTerrain* pTerrain);
	void SetInput(CInput* pInput);
	void EnableAutoPilot(BOOL flag);
	BOOL IsAutoPilot();
	void SetCamera(CCamera* pCamera);
	void Move();
	CMotionCtrl();
	virtual ~CMotionCtrl();

private:
	BOOL CanMove();
	BOOL      m_bIsAutoPilot;
	BOOL      m_bTraceHeight;
	CCamera * m_pCamera;

	CTerrain* m_pTerrain;
	CInput  * m_pInput;
	float     m_v;
	float     m_angle_v;
	RECT      m_Region;
	float     m_SkyHeight;
	float     m_camera_h;
    CTimer    m_Timer;
	CAMERA_INFO* m_pCmInfo;
};

#endif 