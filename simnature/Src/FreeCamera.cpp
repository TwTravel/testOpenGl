/**********************************************************
   模块名称:
       Camera.cpp
   目的: 
       提供摄影机的操作,俯仰,平移,前进,后退,倾斜,旋转等;

   作者:
        潘李亮 HeartBlue
		       XpertSoft HeartBlue 2002/9/26
			   Allright reserved
			   版权所有 

//全是数学运算…^_^

**********************************************************/
#include "CommGLHeader.h"
#include "FreeCamera.h"

void CFreeCamera::SetUpDir(float x,float y,float z)
{
   MVECTOR v={x,y,z};
   Math.Normalize(v);

   m_info.up.x  = v.x; 
   m_info.up.y  = v.y;
   m_info.up.z  = v.z;

}
void CFreeCamera::SetCameraPos(float x,float y,float z)
{
	m_info.pos.x =x;
	m_info.pos.y =y;
	m_info.pos.z =z;
}
void CFreeCamera::SetTarget(float x,float y,float z)
{
	m_info.target.x = x; 
	m_info.target.y = y;
	m_info.target.z = z;
}
//使用摄影机
void CFreeCamera::Look()
{
//	MVECTOR d = {   m_info.target.x - m_info.pos.x,  
//					m_info.target.y - m_info.pos.y,  
//					m_info.target.z - m_info.pos.z,  
//					m_info.target.w - m_info.pos.w
//	};
//	float len = Math.LenVector(d);
//	float dx = m_zoom_info.fdist * d.x /len;
//	float dy = m_zoom_info.fdist * d.y /len;
//	float dz = m_zoom_info.fdist * d.z /len;
	gluLookAt(
		       m_info.pos.x,    m_info.pos.y,     m_info.pos.z,          //观察者的位置
		       m_info.target.x, m_info.target.y,  m_info.target.z, //视点的中心
			   m_info.up.x,     m_info.up.y,      m_info.up.z               //上方的位置
			 );
}
//俯仰角度
void CFreeCamera::Raise(float theta)
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
void CFreeCamera::Turn(float theta)
{
   	MVECTOR vdir = {m_info.target.x-m_info.pos.x,m_info.target.y-m_info.pos.y,m_info.target.z-m_info.pos.z};
	float m=Math.LenVector(vdir);
	Math.Normalize(vdir);

	Math.Rotate(m_info.up,vdir,theta);
	m_info.target.x = m_info.pos.x+vdir.x*m;
	m_info.target.y = m_info.pos.y+vdir.y*m; 
	m_info.target.z = m_info.pos.z+vdir.z*m; 

	m_info.vdir = m_info.target - m_info.pos;
	m_info.vup  = m_info.up ;
}

//绕行
void CFreeCamera::Circle(float theta)
{
   	MVECTOR vdir = {m_info.pos.x-m_info.target.x,m_info.pos.y-m_info.target.y,m_info.pos.z-m_info.target.z};
	float m = Math.LenVector(vdir);
	Math.Normalize(vdir);

	Math.Rotate(m_info.up,vdir,theta);
	m_info.pos.x = m_info.target.x+vdir.x * m;
	m_info.pos.y = m_info.target.y+vdir.y * m; 
	m_info.pos.z = m_info.target.z+vdir.z * m; 

	m_info.vdir = m_info.target - m_info.pos;
	m_info.vup  = m_info.up ;
}
//倾斜
void CFreeCamera::Lean(float theta)
{
	MVECTOR  vdir={m_info.target.x-m_info.pos.x,m_info.target.y-m_info.pos.y,m_info.target.z-m_info.pos.z};
	float m=Math.LenVector(vdir);
	Math.Normalize(vdir);

	Math.Rotate(vdir,m_info.up,theta); 

	m_info.vdir = m_info.target - m_info.pos;
	m_info.vup  = m_info.up ;
}
//向前向后运动
void CFreeCamera::Forward(float dist)
{
    MVECTOR  vdir = {m_info.target.x-m_info.pos.x,m_info.target.y-m_info.pos.y,m_info.target.z-m_info.pos.z};
	float m  =  Math.LenVector(vdir);
    float dx = dist*vdir.x/m;
	float dy = dist*vdir.y/m;
	float dz = dist*vdir.z/m;

    m_info.target.x += dx;
	m_info.target.y += dy;
	m_info.target.z += dz;

	m_info.pos.x += dx;
	m_info.pos.y += dy;
	m_info.pos.z += dz;
	

}
//左右平移
void CFreeCamera::Shift(float dist)
{

	MVECTOR vdir={m_info.target.x-m_info.pos.x,m_info.target.y-m_info.pos.y,m_info.target.z-m_info.pos.z};
	MVECTOR vx;
	Math.Normalize(vdir);
	Math.CrossNormal(vdir,m_info.up,vx);
	
	float m=Math.LenVector(vx);
    float dx=dist*vx.x/m;
	float dy=dist*vx.y/m;
	float dz=dist*vx.z/m;

    m_info.target.x+=dx;
	m_info.target.y+=dy;
	m_info.target.z+=dz;

	m_info.pos.x+=dx;
	m_info.pos.y+=dy;
	m_info.pos.z+=dz;
}
void CFreeCamera::Up(float dist)
{
   float dx=dist*m_info.up.x; 
   float dy=dist*m_info.up.y; 
   float dz=dist*m_info.up.z; 

   m_info.target.x+=dx; 
   m_info.target.y+=dy; 
   m_info.target.z+=dz; 

   m_info.pos.x += dx; 
   m_info.pos.y += dy; 
   m_info.pos.z += dz;
}

void CFreeCamera::ReLocate(float x,float y,float z)
{
    MVECTOR vdir = {m_info.target.x-m_info.pos.x,m_info.target.y-m_info.pos.y,m_info.target.z-m_info.pos.z};
    m_info.pos.x = x;
	m_info.pos.y = y;
	m_info.pos.z = z;
	m_info.target.x=x+vdir.x; 
	m_info.target.y=y+vdir.y; 
	m_info.target.z=z+vdir.z; 
}

CFreeCamera::CFreeCamera()
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
void  CFreeCamera::ZoomedProject(float factor)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(this->m_zoom_info.zoom_angle,factor,0.1,m_info.far_dist *ZOOM_LOD_F*(m_info.vangle/m_zoom_info.zoom_angle));
	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
}
void  CFreeCamera::Project(float factor)
{
	if(IsZoomed())
	  ZoomedProject(factor);
	else
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(this->m_info.vangle,factor,0.1,m_info.far_dist );
		glMatrixMode(GL_MODELVIEW);
	}
}