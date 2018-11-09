#include "MotionCtrl.h"

CMotionCtrl::CMotionCtrl()
{
	m_bTraceHeight = TRUE;
	m_bIsAutoPilot = FALSE;
	m_pCamera = NULL;
	m_pTerrain = NULL;
	m_pInput = NULL;
	m_v = 7;
	m_angle_v = 25;
	m_Region.bottom = m_Region.top = m_Region.left = m_Region.right = 0;
	m_camera_h = 0;
}

CMotionCtrl::~CMotionCtrl()
{

}

void CMotionCtrl::Move()
{

     //Test if the Key Shift is Press
     if(m_pInput->IsKeyDown(VK_SHIFT))
	 {
		 if( m_v < 100)
			m_v += 2;
	 }
	 //Test if the Key Ctrl is Press
	 if(m_pInput->IsKeyDown(VK_CONTROL))
	 {
		 if(m_v>=2)
			 m_v -= 2;
	 }
	 
	 if(m_pInput->IsKeyToggled(VK_CAPITAL))
	 {
		 m_bTraceHeight = TRUE;

		 if(m_bTraceHeight)
		 {
			 m_camera_h = m_pCmInfo->pos.z - m_pTerrain->GetHeight(m_pCmInfo->pos.x,m_pCmInfo->pos.y);
		 }
	 }
	 else
	 {
		 m_bTraceHeight= FALSE;
	 }

	 if(m_bIsAutoPilot)
		 this->AutoPilot();
	 else
		 this->CtrledMove();

	 m_Timer.Reset();
	 SetCameraByMouse();
}

void CMotionCtrl::SetCamera(CCamera *pCamera)
{
	m_pCamera = pCamera;
	m_pCmInfo = pCamera->GetCameraInfo(); 
}

BOOL CMotionCtrl::IsAutoPilot()
{
	return m_bIsAutoPilot;
}

void CMotionCtrl::EnableAutoPilot(BOOL flag)
{
	m_bIsAutoPilot = flag;
}

void CMotionCtrl::SetInput(CInput *pInput)
{
	m_pInput = pInput;
}

void CMotionCtrl::SetTerrain(CTerrain *pTerrain)
{
	m_pTerrain = pTerrain;
}

void CMotionCtrl::SetVelocity(float v)
{
	m_v = v;
}

void CMotionCtrl::SetAngleVelocity(float av)
{
	m_angle_v = av;
}

float CMotionCtrl::GetVelocity()
{
	return m_v;
}

float CMotionCtrl::GetAngleVelocity()
{
	return m_angle_v;
}

void CMotionCtrl::SetRegion(RECT rect)
{
	m_Region = rect;
}

RECT CMotionCtrl::GetRegion()
{
	return m_Region;
}

float CMotionCtrl::GetCameraHeight()
{
	return m_camera_h; 
}

void CMotionCtrl::SetCameraHeight(float h)
{
	m_camera_h = h;
}

void CMotionCtrl::CtrledMove()
{

	float terrian_h = m_pTerrain->GetHeight(m_pCmInfo->pos.x,m_pCmInfo->pos.y);
	float t = m_Timer.GetPassedTime()/1000.;

	if(m_pInput->IsKeyDown(VK_UP))
	{
		if( m_pCmInfo->pos.z < m_SkyHeight - 20)
		{
		      m_camera_h += 25*t;
			  m_pCmInfo->pos.z += 25*t;
			  m_pCmInfo->target.z += 25*t;
		}
	}

	if(m_pInput->IsKeyDown(VK_DOWN))
	{
		if( m_pCmInfo->pos.z > terrian_h + 2.0)
		{
		      m_camera_h -= 25*t;
			  m_pCmInfo->pos.z -= 25*t;
			  m_pCmInfo->target.z -= 25*t;
		}
	}
    
	if(m_pInput->IsKeyDown('A'))
	{
		m_pCamera->Shift(-m_v * t);

		if( !CanMove())
		{
			m_pCamera->Shift(m_v * t);
		}
		TestCameraH();
	}

	if(m_pInput->IsKeyDown('D'))
	{
		m_pCamera->Shift(m_v * t);

		if( !CanMove())
		{
			m_pCamera->Shift(-m_v * t);
		}
		TestCameraH();
	}
	
	if(m_pInput->IsKeyDown('S'))
	{
		m_pCamera->Forward( -m_v*t);

		if( !CanMove())
		{
			m_pCamera->Forward(m_v * t);
		}
		TestCameraH();
	}

	if(m_pInput->IsKeyDown('W'))
	{
		m_pCamera->Forward(m_v * t);

		if( !CanMove())
		{
			m_pCamera->Forward(-m_v * t);
		}
		TestCameraH();
	}

	if(m_pInput->IsKeyDown('Z'))
	{
		m_pCamera->Circle(m_v * t);

		if( !CanMove())
		{
			m_pCamera->Circle( - m_v * t);
		}
		TestCameraH();
	}

	if(m_pInput->IsKeyDown('X'))
	{
		m_pCamera->Circle(  - m_v * t);

		if( !CanMove())
		{
			m_pCamera->Circle( m_v * t);
		}
		TestCameraH();
	}

	if(m_pInput->IsKeyDown(VK_LEFT))
	{
		m_pCamera->Turn( m_angle_v * t);
	}

	if(m_pInput->IsKeyDown(VK_RIGHT))
	{
		m_pCamera->Turn( -m_angle_v * t);
	}

	if(m_pInput->IsKeyDown('Q'))
		m_pCamera->Lean(m_angle_v * t);
	if(m_pInput->IsKeyDown('E'))
		m_pCamera->Lean(-m_angle_v * t);


}

void CMotionCtrl::SetCameraByMouse()
{

	POINT cp={400,400};
	POINT pt;
	GetCursorPos(&pt);
    SetCursorPos(cp.x,cp.y); 
	int ax= pt.x - cp.x ;
	int ay= pt.y - cp.y ;
        
	m_pCamera->Raise(-ay/6.0);
	m_pCamera->Turn(-ax/6.0);
}

void CMotionCtrl::AutoPilot()
{
	float t = m_Timer.GetPassedTime()/1000.;
	m_pCamera->Forward(m_v*t);
	if( !CanMove())
	{
		m_pCamera->Forward(-m_v*t);
	}
	m_pCamera->Turn(m_angle_v*t);
	TestCameraH();

	
}

void CMotionCtrl::SetSkyHeight(float h)
{
    m_SkyHeight = h;
}

BOOL CMotionCtrl::CanMove()
{
	float x;
	float y;
	x = m_pCmInfo->pos.x;
	y = m_pCmInfo->pos.y;
	if(m_Region.bottom == m_Region.top)
		return TRUE;

	if(x<m_Region.left)
		return FALSE;
	if(x>m_Region.right)
		return FALSE;
	if(y<m_Region.top)
		return FALSE;
	if(y>m_Region.bottom)
		return FALSE;

	return TRUE;
}



void CMotionCtrl::TestCameraH()
{
	float th = m_pTerrain->GetHeight(m_pCmInfo->pos.x,m_pCmInfo->pos.y);

	if(m_bTraceHeight)
	{
		float dh = th+m_camera_h -m_pCmInfo->pos.z;
		m_pCmInfo->pos.z = th+m_camera_h;
		m_pCmInfo->target.z += dh;
		return;
	}
	if(th > m_pCmInfo->pos.z - 2.0)
	{
		float dh = th + 2.0 - m_pCmInfo->pos.z;
		m_pCmInfo->pos.z = th + 2.0;
		m_pCmInfo->target.z += dh;
	}
}

void CMotionCtrl::TraceHeight(BOOL flag)
{
	m_bTraceHeight = flag; 
}
