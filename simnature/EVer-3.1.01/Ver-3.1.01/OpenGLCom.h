#ifndef _OPENGL_COM_H
#define _OPENGL_COM_H

#include <windows.h>

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
