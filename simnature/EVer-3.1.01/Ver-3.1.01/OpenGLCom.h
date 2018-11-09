#ifndef _OPENGL_COM_H
#define _OPENGL_COM_H

#include <windows.h>

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


#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

class COpenGL
{
public:	
	BOOL InitOpenGL(HWND hWnd);
	void ReleaseGL();
	GLuint CreateFont();
	void ReleaseFont();
	void RenderFont(char*,int,int);
	void BeginRenderFont();
	void EndRenderFont();
	void SetForceFps(int fps);
	void Resize();
	void SwapBuffer();

private:
	HGLRC		 m_hrc;
	GLuint		 m_font_list;
public:
	int          m_view_width;
	int          m_view_height;
    float        m_eye_angle;
	HWND         m_hWnd;
	float        m_view_factor;
};
extern COpenGL g_GL;
#endif
