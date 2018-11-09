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
#ifndef _SKYBOX_H_
#define _SKYBOX_H_
#include "Camera.h"
class CSkyBox
{

    CCamera* m_pcm;
public:
	void SetCamera(CCamera* c){m_pcm = c;}
	void InitSkyBox(float wx,float wy,float h,int front,int back,int left,int right,int top,int bottom);
	void Adjust(int start_x,int start_y,int start_h)
	{
		m_start_x = start_x;
		m_start_y = start_y;
		m_start_h = start_h;
	}
	void Draw(int start_x,int star_y,int start_h);
	void Draw(){Draw(m_start_x,m_start_y,m_start_h);};
private:
	int m_wx,m_wy,m_h;
	int m_start_x,m_start_y,m_start_h;
	int m_front,m_back,m_left,m_right,m_top,m_bottom;	
};

#endif
