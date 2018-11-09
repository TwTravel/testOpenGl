#include "MMath.h"
#include "Terrain.h"
#include "frustum.h"
#include "ViewFrustum.h"


bool CViewFrustum::IsRectCulled(float cx ,float cy,float size)
{
   
   // return false;
   //判断一个矩形是不是可见，就判断四个角点
	if(m_view_x>=(cx+size/2))		goto next_test;
	if(m_view_x<=(cx-size/2))		goto next_test;
	if(m_view_y<=(cy-size/2))		goto next_test;
	if(m_view_y>=(cy+size/2))		goto next_test;
	   return false;
next_test:
  return !m_frustum.SphereInFrustum(cx,cy,m_mesh->GetRawValue(cx,cy),1.8*size); 

}
bool CViewFrustum::IsPtCulled(float cx ,float cy,float cz)
{


	return false;
	float  x1 = cx - m_view_x;
	float  y1 = cy - m_view_y;
    float  z1 = cz - m_view_z;
    bool   rt=false;
	float dotp2 =   x1 * m_dir_x + y1 * m_dir_y;
	float dotp3 =  dotp2 + z1 * m_dir_z; 

	//背后的三角形，首先将被切除。
	if( dotp3 <= -0.00)
		return true;

	return !m_frustum.PointInFrustum(cx,cy,cz);

}

void CViewFrustum::SetTerrain(CTerrain *ter)
{
   m_mesh = ter;
}
void CViewFrustum::UpdateCamera()
{

	m_frustum.CalculateFrustum();
/*	MVECTOR d = {   m_cinfo->target.x - m_cinfo->pos.x,  
					m_cinfo->target.y - m_cinfo->pos.y,  
					m_cinfo->target.z - m_cinfo->pos.z,  
					m_cinfo->target.w - m_cinfo->pos.w
	};
	float len = Math.LenVector(d);
	float dx = m_zoom_info->fdist * d.x /len;
	float dy = m_zoom_info->fdist * d.y /len;
	float dz = m_zoom_info->fdist * d.z /len;
*/
	m_view_x = m_cinfo->pos.x ;//+ dx;
	m_view_y = m_cinfo->pos.y ;//+ dy;
	m_view_z = m_cinfo->pos.z ;//+ dz;

	m_dir_x = m_cinfo->target.x - m_cinfo->pos.x;  
	m_dir_y = m_cinfo->target.y - m_cinfo->pos.y; 
	m_dir_z = m_cinfo->target.z - m_cinfo->pos.z; 

	float l= sqrt(m_dir_x*m_dir_x + m_dir_y*m_dir_y + m_dir_z*m_dir_z);
	
	m_dir_x/=l;
	m_dir_y/=l;
	m_dir_z/=l;
}

void CViewFrustum::SetViewAngle(int angle)
{
	m_view_angle_cos = Math.Cos(angle);
    m_view_angle_cos2 = m_view_angle_cos * m_view_angle_cos;
	m_view_angle_sin = Math.Sin(angle);
    m_view_angle_sin2 = m_view_angle_sin * m_view_angle_sin;
}

bool CViewFrustum::IsCameraInRect(int cx, int cy, int d)
{
	if(m_view_x>(cx+d)) return false;
	if(m_view_x<(cx-d)) return false;
	if(m_view_y<(cy-d)) return false;
	if(m_view_y>(cy+d)) return false;
	   return true;

}

MVECTOR CViewFrustum::GetViewerPos()
{
	MVECTOR d = {   m_cinfo->target.x - m_cinfo->pos.x,  
					m_cinfo->target.y - m_cinfo->pos.y,  
					m_cinfo->target.z - m_cinfo->pos.z,  
					m_cinfo->target.w - m_cinfo->pos.w
	};
	float len = Math.LenVector(d);
	float dx = 0;// m_zoom_info->fdist * d.x /len;
	float dy = 0;// m_zoom_info->fdist * d.y /len;
	float dz = 0;// m_zoom_info->fdist * d.z /len;
	MVECTOR v = {m_view_x +dx,m_view_y+dy,m_view_z+dz};
	return  v;
}

