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




  模块名称: Viewer Camera
  功能描述：受到束缚的摄影机体的实现，
            可以方便提供场景漫游功能

  版本：Ver l.0
  作者：潘李亮 2003 - 3

**********************************************************/
#ifndef _VIEWER_CAMERA_H_
#define _VIEWER_CAMERA_H_

#include "Camera.h"

class CViewerCamera : public CCamera 
{

public:
	CViewerCamera();
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

	void  SetUpDir(float x,float y,float z);
	void  SetCameraPos(float x,float y,float z);
	void  SetTarget(float x,float y,float z);
	void  ReLocate(float x,float y,float z);

	virtual void SetViewerDir(float x,float y,float z);
	virtual void SetViewerUpDir(float x,float y,float z);
    CAMERA_INFO* GetCameraInfo(){return &m_info;}
	ZOOM_INFO  * GetZoomInfo(){return &m_zoom_info;}
//使用摄影机
	void Look();
//俯仰角度
	void Raise(float theta);
//旋转
	void Turn(float theta);
//绕行
	void Circle(float theta);
//倾斜
	void Lean(float theta);
//向前向后运动
	void Forward(float dist);
//左右平移
	void Shift(float dist);

	void Up(float dist);



private:
	CAMERA_INFO m_info;
	ZOOM_INFO   m_zoom_info;

};
#endif 