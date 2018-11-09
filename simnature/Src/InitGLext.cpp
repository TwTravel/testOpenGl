/**********************************************************
   模块名称:
       InitGLExt.cpp
   目的: 
       初始化OpenGL的扩展

   作者:
        潘李亮 HeartBlue
		       XpertSoft HeartBlue 2003 - 4- 26
			   Allright reserved
			   版权所有 
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
//初始化 多遍纹理映射的扩展
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