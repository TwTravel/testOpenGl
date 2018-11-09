/******************************************************************   
  ģ������:
       Effect.h
   Ŀ��: 
      Ч������ʵ���ļ���

   ����:
        ������ 2003 - 5

  ���еĺ�������Ч��������룬
  �ڱ���������е�Ч����MotionBlur��ʼ���γ�һ��Ч������
  ���е�Ч������������һ��CEffect������
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

//��ѯһ��Ч�����ǲ�����Ҫ����ͼ����������Ϊ�����á�
	virtual BOOL  IsNeedFrameTex(){return m_isEffected;}
//��ѯЧ�����ǲ��Ǵ򿪣�Ҳ�����ǲ��Ǵ�����Ч��״̬
	void          SetEffect(BOOL bFlag){m_isEffected = bFlag;}
	virtual BOOL  Render(GLuint texID, float tcx, float tcy,int view_x,int view_y) = 0;
protected:
	BOOL m_isEffected;
};

//���ȵļ������γɹ����ع��Ч��
class COverBurtEff :public CEffect
{
	    float m_val;
public:
    	void SetValue(float v);
    	float GetValue();
        COverBurtEff();
		virtual BOOL Render(GLuint texID, float tcx, float tcy,int view_x,int view_y);
};

//��ͼ����
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

//Gama������Ч��
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
//��ƬЧ��
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