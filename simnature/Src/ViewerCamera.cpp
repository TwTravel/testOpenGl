
/*
   ģ������:
       VCamera.cpp
   Ŀ��: 
       �ṩ�۲�����Ӱ���Ĳ���,����,ƽ��,ǰ��,����,��б,��ת��;

   ����:
        ������ HeartBlue
		       XpertSoft HeartBlue 2002/9/26
			   Allright reserved
			   ��Ȩ���� 
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

//ʹ����Ӱ��
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
		       m_info.pos.x+dx,    m_info.pos.y+dy,     m_info.pos.z+dz,          //�۲��ߵ�λ��
		       m_info.target.x+dx, m_info.target.y+dy,  m_info.target.z+dz, //�ӵ������
			   m_info.up.x,     m_info.up.y,      m_info.up.z               //�Ϸ���λ��
			 );
*/

	gluLookAt(
		       m_info.pos.x,    m_info.pos.y,     m_info.pos.z,          //�۲��ߵ�λ��
		       m_info.target.x, m_info.target.y,  m_info.target.z, //�ӵ������
			   m_info.up.x,     m_info.up.y,      m_info.up.z               //�Ϸ���λ��
			 );

}
//�����Ƕ�
void CViewerCamera::Raise(float theta)
{
	MVECTOR vdir= {m_info.target.x - m_info.pos.x, m_info.target.y-m_info.pos.y,m_info.target.z-m_info.pos.z};
	MVECTOR vn;	
	float m=Math.LenVector(vdir);
	Math.Normalize(vdir);

	//�õ�����һ���۲�ƽ��� �����᷽��
	Math.CrossNormal(vdir,m_info.up,vn);
	
    //����λ������ת
	Math.Rotate(vn,m_info.up,theta);
    //��ת���߷���
    Math.Rotate(vn,vdir,theta);

    m_info.target.x = m_info.pos.x + vdir.x*m;
	m_info.target.y = m_info.pos.y + vdir.y*m; 
	m_info.target.z = m_info.pos.z + vdir.z*m; 
}
//��ת
void CViewerCamera::Turn(float theta)
{
   MVECTOR n = m_info.vup;
   //��ת��Ӱ�����Ϸ���
   Math.RotatePoint(n,m_info.up,theta);

   //��ת��Ӱ����ǰ����
   MVECTOR vTagDir = {m_info.target.x-m_info.pos.x,m_info.target.y-m_info.pos.y,m_info.target.z-m_info.pos.z};
   
   float m=Math.LenVector(vTagDir);
   Math.Normalize(vTagDir);
   Math.RotatePoint(n,vTagDir,theta);
   
   m_info.target.x=m_info.pos.x + vTagDir.x*m;  
   m_info.target.y=m_info.pos.y + vTagDir.y*m;  
   m_info.target.z=m_info.pos.z + vTagDir.z*m;  

   //��ת�˵�ǰ�淽��
   Math.Rotate(n,m_info.vdir,theta);
}

//����
void CViewerCamera::Circle(float theta)
{
	MVECTOR vdir={m_info.target.x-m_info.pos.x,m_info.target.y-m_info.pos.y,m_info.target.z-m_info.pos.z};
	float m=Math.LenVector(vdir);
 
	//�����ӵ��λ��
	MVECTOR oldCenter = {  m_info.pos.x + m * m_info.vdir.x,
		                    m_info.pos.y + m * m_info.vdir.y,
		                    m_info.pos.z + m * m_info.vdir.z	};

	//��ת �۲��� �� ��Ӱ���ķ���
	Turn(theta);

	//�õ��µ��ӵ�λ��
	MVECTOR newCenter= {   m_info.pos.x + m * m_info.vdir.x,
		                   m_info.pos.y + m * m_info.vdir.y,
		                   m_info.pos.z + m * m_info.vdir.z	};

	MVECTOR deta= {oldCenter.x - newCenter.x,
		           oldCenter.y - newCenter.y,
				   oldCenter.z - newCenter.z};

    //�ָ��ɵ��ӵ�λ�ã��������µĹ۲��ߵ�λ��
	m_info.pos.x += deta.x;
	m_info.pos.y += deta.y;
	m_info.pos.z += deta.z;

	m_info.target.x += deta.x; 
	m_info.target.y += deta.y; 
	m_info.target.z += deta.z; 
} 

//��б
void CViewerCamera::Lean(float theta)
{
	MVECTOR vTagDir = {m_info.target.x-m_info.pos.x,m_info.target.y-m_info.pos.y,m_info.target.z-m_info.pos.z};
	float m=Math.LenVector(vTagDir);
	Math.Normalize(vTagDir);

	Math.Rotate(vTagDir,m_info.up,theta); 
}
//��ǰ����˶�
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
//����ƽ��
void CViewerCamera::Shift(float dist)
{

//�õ�x�ķ���
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