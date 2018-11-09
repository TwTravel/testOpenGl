/**********************************************************
  SimNature Ver1.0.0
  Copyright : Panliliang (Stanly Lee)
  2002 - 2003
  Alright reserved!
  All Noncommeric use is Free!

  ��������Ȩ���� 
             2003-2003
  ����������з���ҵ��ʹ��Ϊ��Ѵ��롣

  E- Mial: Panliliang@etang.com
		   Xheartblue@etang.com 




  ģ������: ViewFrutum Culler
  �����������ṩһ�������Ԥ�ü���

  �汾��Ver l.0
  ���ߣ������� 2003 - 3

**********************************************************/
#ifndef _VIEW_FRUSTUM_H_
#define _VIEW_FRUSTUM_H_

#include "Camera.h"
#include "MMath.h"	// Added by ClassView
#include "frustum.h"


class CTerrain;

class CViewFrustum
{
public:
	MVECTOR GetViewerPos();
	bool IsCameraInRect(int cx,int cy,int d);
	void SetViewAngle(int angle);
	void SetTerrain(CTerrain* ter);
	void SetCamera(CCamera* pcm){m_cinfo=pcm->GetCameraInfo();m_zoom_info = pcm->GetZoomInfo();}
    void SetViewerPos(float x,float y,float z){m_view_x = x; m_view_y = y;m_view_z = z;}
	void SetViewerDir(float x,float y,float z){m_dir_x = x;m_dir_y = y;m_dir_z = z;}
	void SetViewerDist(float d){m_view_dist = d*d;}

	bool IsRectCulled(float cx ,float cy,float size); 
	bool IsPtCulled(float cx ,float cy,float cz);
	void UpdateCamera();

	float m_dir_x,m_dir_y,m_dir_z;
	float m_view_x,m_view_y,m_view_z;
	float m_view_dist;
	float   m_view_angle_cos2;
	float   m_view_angle_cos;
	float   m_view_angle_sin2;
	float   m_view_angle_sin;
	CTerrain* m_mesh;
	CAMERA_INFO* m_cinfo;
	ZOOM_INFO  *m_zoom_info;
	char*  m_culled;
	CFrustum m_frustum;
};

#endif