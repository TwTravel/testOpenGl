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

  ģ������: Motion Blur Ч����
  �����������ṩ�˶�ģ��Ч����
			ͬʱ��������һ����������������Ҫ�õ�֡�����������������Ч����
			������һ��Ч�����Ŀ�ʼ��

  �汾��
  ���ߣ�

**********************************************************/
#ifndef _MOTION_BLUR_H_
#define _MOTION_BLUR_H_

#include <windows.h>
#include <gl\gl.h>
#include <vector>
using namespace std;
#include "Effect.h"

typedef int (*PFNCRENDERBLUR)(long);
class CMotionBlur  
{
public:
	BOOL	RemoveEffect(CEffect* eff);
	int		AddEffect(CEffect* eff);
	void	OnlyRenderOtherEffect();

	void	Render();
	void	BeginRender();
	void    Render(PFNCRENDERBLUR pfnRender,long fps);

	void	AfterRender();
	void	PreRender();
	float	GetBlurValue();
	void	SetBlurValue(float f);
	BOOL	IsEffected();
	void	SetEffected(BOOL eflag);
	void	Resize();
	void	Create(int size_x,int size_y);

	CMotionBlur();
	virtual ~CMotionBlur();

private:
	BOOL   m_bIsCreated;
	int    m_view_port_x,m_view_port_y;
	GLuint m_tex;
    BOOL   m_bEffected;
	int    m_tex_size_x,m_tex_size_y;
	float  m_blur_val;

	vector<CEffect*> m_Effects;

};

#endif 