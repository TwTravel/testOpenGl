/**********************************************************
   ģ������:
       InitGLExt.cpp
   Ŀ��: 
       ��ʼ��OpenGL����չ

   ����:
        ������ HeartBlue
		       XpertSoft HeartBlue 2003 - 4- 26
			   Allright reserved
			   ��Ȩ���� 
**********************************************************/
#include <windows.h>

#include <gl\gl.h>
#include <gl\glu.h>
#include "InitSence.h"
#include "Console.h"
//I copy a glext.h to the source code directory 
//please copy it to your opengl include directory
//or you can modify all #include <gl\glext.h>
// to #include "glext.h"
#include <gl\glext.h>

//==================================================
//��ʼ�� �������ӳ�����չ
//==================================================
PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB	= NULL;
PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB		= NULL;
BOOL InitMultiTex()
{
	glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)		wglGetProcAddress("glMultiTexCoord2fARB");
	if(glActiveTextureARB==NULL || glMultiTexCoord2fARB==NULL)
		return FALSE;
	return TRUE;
	
}