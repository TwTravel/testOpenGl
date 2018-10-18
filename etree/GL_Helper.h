//****************************************************************************
//  GL_Helper.h: interface for the GL_Helper class.
//
//  Copyright (c) Boris J. Wang (e_boris2002@hotmail.com) 2002 - 2003
//  From Institute of Computing Technology, Chinese Academy of Sciences
//                                                Beijing 100871, China
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
//  PARTICULAR PURPOSE.
//****************************************************************************

#if !defined(AFX_GL_HELPER_H__949D7E8A_43E7_4408_A07B_D08D0B349154__INCLUDED_)
#define AFX_GL_HELPER_H__949D7E8A_43E7_4408_A07B_D08D0B349154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

#define Deg2Rad(x) (x)*0.0174532925f

#define GL_Tex_256RGBA	1
#define GL_Tex_BMP		1000
#define GL_Tex_BMP_RGA_RRGB	GL_Tex_BMP+1
#define GL_Tex_BMP_RGA_RG0B	GL_Tex_BMP+2

class CglTexture
{
public:
	void glAutoBuildMipmap2D(GLint internalformat,GLsizei Width,GLsizei Height,GLenum format,const GLvoid * DibPtr,GLenum Type = GL_UNSIGNED_BYTE);
	static void Enable(bool yes = true);
	void Apply();
	CglTexture(LPCTSTR ImgFn,int Type,int MipmapLevel = 0); //eg: A 256X256 image MipmapLevel should be 9
	~CglTexture();

	int w,h;

protected:
	void LoadTexture();
	LPCTSTR Fn;
	int Type;
	int MipmapLevel;
	GLuint TexName;
	static GLuint CurrentTexName;
	static GLfloat MappingMode;
	BOOL	BadFile;
};

class CglMaterial
{
public:
	CglMaterial();
	~CglMaterial();

	void Apply(GLenum face = GL_FRONT_AND_BACK);
	void SetShiness(GLfloat shin);
	void SetEmission(GLfloat r,GLfloat g,GLfloat b,GLfloat a=1.0f);
	void SetSpecular(GLfloat r,GLfloat g,GLfloat b,GLfloat a=1.0f);
	void SetDiffuse(GLfloat r,GLfloat g,GLfloat b,GLfloat a=1.0f);
	void SetAmbient(GLfloat r,GLfloat g,GLfloat b,GLfloat a=1.0f);

protected:
	GLfloat Ambient[4];  //环境光
	GLfloat Diffuse[4];  //漫反射光
	GLfloat Specular[4]; //镜面高光
	GLfloat Emission [4];//辐射光
	GLfloat Shiness;	 //光亮度

	int		Name;
	static  int LastApplied;

private:
	static  int	LastName;
};

#endif // !defined(AFX_GL_HELPER_H__949D7E8A_43E7_4408_A07B_D08D0B349154__INCLUDED_)
