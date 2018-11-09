#ifndef _GL_TEXTURE_H_
#define _GL_TEXTURE_H_

#include <windows.h>
#include <GL\gl.h>
#include <image.h>
typedef struct
{
 int width,height;
 unsigned char* pdata;
 int      bits;
 GLuint   format;

}DIBTEXDATA,*PDIBTEXDATA;

#ifdef _DEBUG
#pragma comment(lib,"gltextured.lib")
#else
#pragma comment(lib,"gltexture.lib")
#endif

typedef GLuint GLTEXTURE;


extern "C" DIBTEXDATA	LoadTexData(char* filename);
extern "C" DIBTEXDATA	CreateDibTexData(int w,int h);
extern "C" void			FreeDibTexData(DIBTEXDATA dib);
extern "C" void			SetTexAlpha(GLubyte color[],GLubyte alpha,DIBTEXDATA dib,int r);
extern "C" void			SetTexPixel(int x,int y,GLubyte color[],DIBTEXDATA dib);
extern "C" void			GetTexPixel(int x,int y,GLubyte color[],DIBTEXDATA dib);
extern "C" GLuint		BuildTexture(char* filename);
extern "C" GLuint		BuildTransparencyTexture(char* filename,GLubyte color[],GLubyte alpha,int r);
extern "C" GLuint		BuildTextureFromImage(unsigned char* image,IMAGESIZE size);
extern "C" GLuint		BuildTextureFromRGBImage(unsigned char* image,IMAGESIZE size);



#endif