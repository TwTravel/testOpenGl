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




  ģ������:
  ����������

  �汾��
  ���ߣ�

**********************************************************/
#ifndef _FREE_CAMERA_H_
#define _FREE_CAMERA_H_

#include "OpenGLCom.h"
#include "Camera.h"
class CFreeCamera : public CCamera 
{

public:
	CFreeCamera();
	void  SetZoomFactor(float factor){m_zoom_info.factor = factor;}
	float GetZoomFactor(){return m_zoom_info.factor;}
	void  SetZoomAngle(float angle){m_zoom_info.zoom_angle = angle;}
	float GetZoomAngle(){return m_zoom_info.zoom_angle;}
	void  SetViewerAngle(float angle){m_info.vangle = angle;}
	float GetViewerAngle(){return m_info.vangle;}

	void  ZoomedProject(float factor);
    void  Project(float factor);

	void  SetViewDist(float dist){m_info.far_dist = dist;}
	float GetViewDist(){return m_info.far_dist;}

	void SetUpDir(float x,float y,float z);
	void SetCameraPos(float x,float y,float z);
	void SetTarget(float x,float y,float z);
	void ReLocate(float x,float y,float z);
	virtual void SetViewerDir(float x,float y,float z){m_info.vdir = m_info.target - m_info.pos;};
	virtual void SetViewerUpDir(float x,float y,float z){m_info.vup  = m_info.up ;};

    CAMERA_INFO* GetCameraInfo(){return &m_info;}
	ZOOM_INFO  * GetZoomInfo(){return &m_zoom_info;}
//ʹ����Ӱ��
	void Look();
//�����Ƕ�
	void Raise(float theta);
//��ת
	void Turn(float theta);
//����
	void Circle(float theta);
//��б
	void Lean(float theta);
//��ǰ����˶�
	void Forward(float dist);
//����ƽ��
	void Shift(float dist);

	void Up(float dist);


private:
	CAMERA_INFO m_info;
	ZOOM_INFO   m_zoom_info;
};


#endif


