/******************************************************************   
  模块名称:
       Effect.h
   目的: 
      效果器的实现文件。

   作者:
        潘李亮 2003 - 5

  所有的后期特殊效果处理代码，
  在本程序里。所有的效果以MotionBlur开始，形成一条效果链。
  所有的效果处理器都是一个CEffect的子类
******************************************************************/

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <windows.h>
#include <GL\gl.h>

#include "Camera.h"
class CEffect  
{
public:
	CEffect();
	virtual       ~CEffect();
	BOOL          IsEffect(){return m_isEffected;}

//查询一个效果器是不是需要拷贝图象到纹理里作为处理用。
	virtual BOOL  IsNeedFrameTex(){return m_isEffected;}
//查询效果器是不是打开，也就是是不是处于有效的状态
	void          SetEffect(BOOL bFlag){m_isEffected = bFlag;}
	virtual BOOL  Render(GLuint texID, float tcx, float tcy,int view_x,int view_y) = 0;
protected:
	BOOL m_isEffected;
};

//过度的加亮，形成过度曝光的效果
class COverBurtEff :public CEffect
{
	    float m_val;
public:
    	void SetValue(float v);
    	float GetValue();
        COverBurtEff();
		virtual BOOL Render(GLuint texID, float tcx, float tcy,int view_x,int view_y);
};

//地图导航
class CMapPilotEff:public CEffect
{
	GLuint  m_TerrTex;
	CAMERA_INFO * m_pCmInfo;
	int m_w,m_h;
public:
	CMapPilotEff(){};
	void    SetTerrainSize(int x,int y){m_w = x;m_h = y;}
	void    SetCamraInfo(CCamera* pCm){m_pCmInfo = pCm->GetCameraInfo();}
	void    SetTerrainTex(GLuint tex){m_TerrTex = tex;}
	virtual BOOL  IsNeedFrameTex(){return FALSE;}
	virtual BOOL  Render(GLuint texID, float tcx, float tcy,int view_x,int view_y);
};

//Gama控制器效果
class CGamaEff: public CEffect 
{
public:
	float   GetValue(){return m_gama;}
	virtual BOOL  Render(GLuint texID, float tcx, float tcy,int view_x,int view_y);
	void    SetValue(float f);

	virtual BOOL  IsNeedFrameTex(){return FALSE;}
	CGamaEff(){m_gama = 1.0;}
	virtual ~CGamaEff(){};

private:
	float m_gama;

};
//碎片效果
class CScrapEff: public CEffect 
{
	struct _PATCH
	{
		float x,float y;
		float angle_x,float angle_y;float angle_z;
	};

	_PATCH        _patch[20][20];
	int           m_life;
public:
	void Scrap();
	virtual BOOL  Render(GLuint texID, float tcx, float tcy,int view_x,int view_y);
	virtual BOOL  IsNeedFrameTex(){return FALSE;}
	CScrapEff();
	virtual ~CScrapEff(){};
};
class CSnipEff: public CEffect 
{
	GLuint m_texID;
public:
	virtual BOOL Render(GLuint texID, float tx, float ty,int view_x,int view_y);
	virtual BOOL  IsNeedFrameTex(){return FALSE;}
	CSnipEff(){};
	void Create(GLuint texID){m_texID = texID;};
	virtual ~CSnipEff(){};
};

class CCrossEff: public CEffect 
{
	GLuint m_texID;
public:
	virtual BOOL Render(GLuint texID, float tx, float ty,int view_x,int view_y);
	virtual BOOL  IsNeedFrameTex(){return FALSE;}
	CCrossEff(){};
	void Create(GLuint texID){m_texID = texID;};
	virtual ~CCrossEff(){};
};
#endif