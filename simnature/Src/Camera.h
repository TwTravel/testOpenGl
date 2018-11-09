/**********************************************************
  SimNature Ver1.0.0
  Copyright : Panliliang (Stanly Lee)
  2002 - 2003
  Alright reserved!
  All Noncommeric use is Free!

  潘李亮版权所有 
             2003-2003
  本程序的所有非商业化使用为免费代码。

  E- Mial: Panliliang@etang.com
		   Xheartblue@etang.com 




  模块名称:
  功能描述：

  版本：
  作者：

**********************************************************/
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "OpenGLCom.h"
#include "MMath.h"
typedef struct
{
	int     id;//摄影机的唯一编号
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
	//使用摄影机
	virtual void Look() = 0;
	//俯仰角度
	virtual void Raise(float theta) = 0;
	//旋转
	virtual void Turn(float theta) = 0;
	//绕行
	virtual void Circle(float theta) = 0;
	//倾斜
	virtual void Lean(float theta) = 0;
	//向前向后运动
	virtual void Forward(float dist) = 0;
	//左右平移
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


