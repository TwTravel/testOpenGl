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
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "OpenGLCom.h"
#include "MMath.h"
typedef struct
{
	int     id;//��Ӱ����Ψһ���
	MVECTOR up;
	MPOINT  pos;
	MPOINT  target;
	
	MVECTOR vdir;
	MVECTOR vup;

	float vangle;
	float far_dist;
	float near_dist;	
}CAMERA_INFO;

typedef struct
{
	float zoom_angle;
	float factor;
}ZOOM_INFO;

class CCamera
{

public:
	virtual void  SetZoomFactor(float factor) =0;
	virtual float GetZoomFactor()=0;
	virtual void  SetZoomAngle(float angle)=0;
	virtual float GetZoomAngle()=0;
	virtual void  SetViewerAngle(float angle)=0;
	virtual float GetViewerAngle()=0;
	virtual void  ZoomedProject(float)=0;
    virtual void  Project(float)=0;

	virtual void  SetViewDist(float dist)=0;
	virtual float GetViewDist()=0;
	virtual void  SetUpDir(float x,float y,float z) = 0;
	virtual void  SetCameraPos(float x,float y,float z) = 0;
	virtual void  SetTarget(float x,float y,float z) = 0;
	virtual void  ReLocate(float x,float y,float z) = 0;
	
	virtual void SetViewerDir(float x,float y,float z) = 0;
	virtual void SetViewerUpDir(float x,float y,float z) =0;
	//ʹ����Ӱ��
	virtual void Look() = 0;
	//�����Ƕ�
	virtual void Raise(float theta) = 0;
	//��ת
	virtual void Turn(float theta) = 0;
	//����
	virtual void Circle(float theta) = 0;
	//��б
	virtual void Lean(float theta) = 0;
	//��ǰ����˶�
	virtual void Forward(float dist) = 0;
	//����ƽ��
	virtual void Shift(float dist) = 0;

	virtual void Up(float dist) = 0;
    virtual CAMERA_INFO* GetCameraInfo()=0;	
	virtual ZOOM_INFO  * GetZoomInfo()=0;

	 void   Zoomed(bool flag){m_is_zoom = flag;}
	 bool   IsZoomed(){return m_is_zoom;}
private:
	 bool m_is_zoom;
};
#define ZOOM_LOD_F 0.4
#endif


