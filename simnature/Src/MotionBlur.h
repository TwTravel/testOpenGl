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

  模块名称: Motion Blur 效果器
  功能描述：提供运动模糊效果。
			同时，它还是一个容器。它包含了要用到帧缓存纹理的其它所有效果。
			它是这一类效果链的开始。

  版本：
  作者：

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