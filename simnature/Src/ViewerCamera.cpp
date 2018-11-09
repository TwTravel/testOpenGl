
/*
   模块名称:
       VCamera.cpp
   目的: 
       提供观察者摄影机的操作,俯仰,平移,前进,后退,倾斜,旋转等;

   作者:
        潘李亮 HeartBlue
		       XpertSoft HeartBlue 2002/9/26
			   Allright reserved
			   版权所有 
*/
#include "MMath.h"
#include "OpenGLCom.h"
#include "ViewerCamera.h"



void CViewerCamera::SetUpDir(float x,float y,float z)
{
   MVECTOR v={x,y,z};
   Math.Normalize(v);
   m_info.up.x=v.x; 
   m_info.up.y=v.y;
   m_info.up.z=v.z;
}

void CViewerCamera::SetCameraPos(float x,float y,float z)
{
	m_info.pos.x = x;
	m_info.pos.y = y;
	m_info.pos.z = z; 
}
void CViewerCamera::SetTarget(float x,float y,float z)
{
	m_info.target.x=x; 
	m_info.target.y=y;
	m_info.target.z=z;
}

void CViewerCamera::SetViewerUpDir(float x,float y,float z)
{
	m_info.vup.x=x;
	m_info.vup.y=y;
	m_info.vup.z=z;
	Math.Normalize(m_info.vup); 
}
void CViewerCamera::SetViewerDir(float x,float y,float z)
{
	m_info.vdir.x= x;
	m_info.vdir.y= y;
	m_info.vdir.z= z;
	Math.Normalize(m_info.vdir); 
}

//使用摄影机
void CViewerCamera::Look()
{

/*
//This commented code is for the ZOOMED distanced display
	MVECTOR d = {   m_info.target.x - m_info.pos.x,  
					m_info.target.y - m_info.pos.y,  
					m_info.target.z - m_info.pos.z,  
					m_info.target.w - m_info.pos.w
	};
	float len = Math.LenVector(d);
	float dx = m_zoom_info.fdist * d.x /len;
	float dy = m_zoom_info.fdist * d.y /len;
	float dz = m_zoom_info.fdist * d.z /len;
	gluLookAt(
		       m_info.pos.x+dx,    m_info.pos.y+dy,     m_info.pos.z+dz,          //观察者的位置
		       m_info.target.x+dx, m_info.target.y+dy,  m_info.target.z+dz, //视点的中心
			   m_info.up.x,     m_info.up.y,      m_info.up.z               //上方的位置
			 );
*/

	gluLookAt(
		       m_info.pos.x,    m_info.pos.y,     m_info.pos.z,          //观察者的位置
		       m_info.target.x, m_info.target.y,  m_info.target.z, //视点的中心
			   m_info.up.x,     m_info.up.y,      m_info.up.z               //上方的位置
			 );

}
//俯仰角度
void CViewerCamera::Raise(float theta)
{
	MVECTOR vdir= {m_info.target.x - m_info.pos.x, m_info.target.y-m_info.pos.y,m_info.target.z-m_info.pos.z};
	MVECTOR vn;	
	float m=Math.LenVector(vdir);
	Math.Normalize(vdir);

	//得到另外一条观察平面的 坐标轴方向
	Math.CrossNormal(vdir,m_info.up,vn);
	
    //向上位置先旋转
	Math.Rotate(vn,m_info.up,theta);
    //旋转视线方向
    Math.Rotate(vn,vdir,theta);

    m_info.target.x = m_info.pos.x + vdir.x*m;
	m_info.target.y = m_info.pos.y + vdir.y*m; 
	m_info.target.z = m_info.pos.z + vdir.z*m; 
}
//旋转
void CViewerCamera::Turn(float theta)
{
   MVECTOR n = m_info.vup;
   //旋转摄影机的上方向
   Math.RotatePoint(n,m_info.up,theta);

   //旋转摄影机的前方向
   MVECTOR vTagDir = {m_info.target.x-m_info.pos.x,m_info.target.y-m_info.pos.y,m_info.target.z-m_info.pos.z};
   
   float m=Math.LenVector(vTagDir);
   Math.Normalize(vTagDir);
   Math.RotatePoint(n,vTagDir,theta);
   
   m_info.target.x=m_info.pos.x + vTagDir.x*m;  
   m_info.target.y=m_info.pos.y + vTagDir.y*m;  
   m_info.target.z=m_info.pos.z + vTagDir.z*m;  

   //旋转人的前面方向
   Math.Rotate(n,m_info.vdir,theta);
}

//绕行
void CViewerCamera::Circle(float theta)
{
	MVECTOR vdir={m_info.target.x-m_info.pos.x,m_info.target.y-m_info.pos.y,m_info.target.z-m_info.pos.z};
	float m=Math.LenVector(vdir);
 
	//保存视点的位置
	MVECTOR oldCenter = {  m_info.pos.x + m * m_info.vdir.x,
		                    m_info.pos.y + m * m_info.vdir.y,
		                    m_info.pos.z + m * m_info.vdir.z	};

	//旋转 观察者 和 摄影机的方向
	Turn(theta);

	//得到新的视点位置
	MVECTOR newCenter= {   m_info.pos.x + m * m_info.vdir.x,
		                   m_info.pos.y + m * m_info.vdir.y,
		                   m_info.pos.z + m * m_info.vdir.z	};

	MVECTOR deta= {oldCenter.x - newCenter.x,
		           oldCenter.y - newCenter.y,
				   oldCenter.z - newCenter.z};

    //恢复旧的视点位置，和设置新的观察者的位置
	m_info.pos.x += deta.x;
	m_info.pos.y += deta.y;
	m_info.pos.z += deta.z;

	m_info.target.x += deta.x; 
	m_info.target.y += deta.y; 
	m_info.target.z += deta.z; 
} 

//倾斜
void CViewerCamera::Lean(float theta)
{
	MVECTOR vTagDir = {m_info.target.x-m_info.pos.x,m_info.target.y-m_info.pos.y,m_info.target.z-m_info.pos.z};
	float m=Math.LenVector(vTagDir);
	Math.Normalize(vTagDir);

	Math.Rotate(vTagDir,m_info.up,theta); 
}
//向前向后运动
void CViewerCamera::Forward(float dist)
{

    float dx = dist * m_info.vdir.x;
	float dy = dist * m_info.vdir.y;
	float dz = dist * m_info.vdir.z;

	//Move the position
	m_info.pos.x += dx;
	m_info.pos.y += dy;
	m_info.pos.z += dz;
    //Reset tagart
	m_info.target.x += dx; 
	m_info.target.y += dy; 
	m_info.target.z += dz; 

}
//左右平移
void CViewerCamera::Shift(float dist)
{

//得到x的方向
	MVECTOR vx;
	Math.CrossNormal(m_info.vdir,m_info.vup,vx);

    float dx=dist*vx.x;
	float dy=dist*vx.y;
	float dz=dist*vx.z;

    m_info.target.x+=dx;
	m_info.target.y+=dy;
	m_info.target.z+=dz;

	m_info.pos.x+=dx;
	m_info.pos.y+=dy;
	m_info.pos.z+=dz;
}
void CViewerCamera::Up(float dist)
{
   float dx=dist*m_info.vup.x; 
   float dy=dist*m_info.vup.y; 
   float dz=dist*m_info.vup.z; 

   m_info.target.x+=dx; 
   m_info.target.y+=dy; 
   m_info.target.z+=dz; 

   m_info.pos.x+=dx; 
   m_info.pos.y+=dy; 
   m_info.pos.z+=dz;
}

void CViewerCamera::ReLocate(float x,float y,float z)
{
    MVECTOR vTagDir = {m_info.target.x-m_info.pos.x,m_info.target.y-m_info.pos.y,m_info.target.z-m_info.pos.z};
    m_info.pos.x=x;
	m_info.pos.y=y;
	m_info.pos.z=z;

	m_info.target.x=x + vTagDir.x; 
	m_info.target.y=y + vTagDir.y; 
	m_info.target.z=z + vTagDir.z; 
}

CViewerCamera::CViewerCamera()
{
	this->m_zoom_info.factor = 1;
	this->m_zoom_info.zoom_angle =0;
	this->m_info.vangle =0; 

	m_info.near_dist = 0.01;
	m_info.pos.z = m_info.pos.y =m_info.pos.x = 0;
	m_info.target.x = m_info.target.z =0;
	m_info.target.y = 10;

	m_info.up.x =m_info.up.y = 0;
	m_info.up.z = 1.0;
	
	m_info.vdir.x = m_info.vdir.z = 0;
	m_info.vdir.y = 1.0;
	
	m_info.vup.x = m_info.up.x ; 
	m_info.vup.y = m_info.up.y ;
	m_info.vup.z = m_info.up.z ;  
}
void  CViewerCamera::ZoomedProject(float factor)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(this->m_zoom_info.zoom_angle,factor,0.1,m_info.far_dist *ZOOM_LOD_F* (m_info.vangle/m_zoom_info.zoom_angle));
	glMatrixMode(GL_MODELVIEW);
}
void  CViewerCamera::Project(float factor)
{
	if(IsZoomed())
	  ZoomedProject(factor);
	else
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(this->m_info.vangle,factor,0.1,m_info.far_dist);
		glMatrixMode(GL_MODELVIEW);
	}
}