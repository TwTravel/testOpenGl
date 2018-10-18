//****************************************************************************
//  GL_Helper.cpp: implementation of the GL_Helper class.
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

#include "stdafx.h"
#include "GL_Helper.h"
#include "dib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

GLuint CglTexture::CurrentTexName = 0;
GLfloat CglTexture::MappingMode = GL_MODULATE;

CglTexture::~CglTexture()
{

}

CglTexture::CglTexture(LPCTSTR ImgFn,int iType,int iMipmapLevel)
{
	TexName = 0;
	VERIFY(Fn = ImgFn);
	Type = iType;
	MipmapLevel = iMipmapLevel;
	BadFile = FALSE;
}

void CglTexture::Apply()
{
	if(BadFile)return;

	if(TexName)
	{
		if(CurrentTexName != TexName)
		{
		    glBindTexture(GL_TEXTURE_2D, TexName);
			CurrentTexName = TexName;
		}
	}
	else
	{
		LoadTexture();
		CurrentTexName = TexName;
	}
}

void CglTexture::LoadTexture()
{LPBYTE Dib, RGBA = NULL;
 CDib bmp;
	try
	{
		switch(Type)
		{
		case GL_Tex_BMP:
		case GL_Tex_BMP_RGA_RRGB:
		case GL_Tex_BMP_RGA_RG0B:
			{
				if(!bmp.LoadBitmapFile(Fn))
				{
					BadFile = TRUE;
					AfxMessageBox(CString(_T("Failed to load Texture: "))+Fn,MB_ICONWARNING);
					return;
				}

				Dib = bmp.GetDibBitsPtr();
				w = bmp.GetDibWidth();
				h = bmp.GetDibHeight();
				break;
			}
		case GL_Tex_256RGBA:
			{
				w = 256;
				h = 256;
				CFile file;
				if(!file.Open(Fn,CFile::modeRead|CFile::shareDenyNone))
				{
					BadFile = TRUE;
					AfxMessageBox(CString("Failed to load Texture: ")+Fn,MB_ICONWARNING);
					return;
				}
				RGBA = new unsigned char [262144];
				if(RGBA)
				{
					file.Read(RGBA,262144);
				}

				file.Close();

				Dib = RGBA;

				break;
			}
		default:
			{
				ASSERT(0);
				return;
			}
		}
	}
	catch(CFileException * e)
	{
		e->ReportError();
		e->Delete();

		if(RGBA)delete RGBA;
		return ;
	}

	if(Type>GL_Tex_BMP)
	{
		RGBA = new BYTE[4*w*h];
		ASSERT(RGBA);
		LPBYTE p = RGBA;
		int i,l=w*h;

		switch(Type)
		{
		case GL_Tex_BMP_RGA_RRGB:
				{
					for(i=0;i<l;i++,p+=4,Dib+=3)
					{
						p[0] = Dib[1];  //G -> B  
						p[1] = Dib[2];  //R -> G
						p[2] = Dib[2];  //R -> R
						p[3] = Dib[0];  //B -> A
					}
					break;
				}
		case GL_Tex_BMP_RGA_RG0B:
				{
					for(i=0;i<l;i++,p+=4,Dib+=3)
					{
						p[0] = 0;		//0 -> B
						p[1] = Dib[1];	//G -> G
						p[2] = Dib[2];	//R -> R
						p[3] = Dib[0];  //B -> A
					}
					break;
				}
		default:ASSERT(0);
		}

		Dib = RGBA;
	}


    glGenTextures(1, &TexName);
	ASSERT(TexName);
    glBindTexture(GL_TEXTURE_2D, TexName);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	//Define Texture
	switch(Type)
	{
	case GL_Tex_BMP:
		{
			if(MipmapLevel)
			{
				glAutoBuildMipmap2D(3,w,h,GL_BGR_EXT ,Dib);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D,0,3,w,h,0,GL_BGR_EXT ,GL_UNSIGNED_BYTE,Dib);
			}
			break;
		}
	case GL_Tex_256RGBA:
	case GL_Tex_BMP_RGA_RRGB:
	case GL_Tex_BMP_RGA_RG0B:
		{
			if(MipmapLevel)
			{
				glAutoBuildMipmap2D(4,w,h,GL_BGRA_EXT ,Dib);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D,0,4,w,h,0,GL_BGRA_EXT ,GL_UNSIGNED_BYTE,Dib);
			}
			break;
		}
	default:
		{
			ASSERT(0);
			if(RGBA)delete RGBA;
			return;
		}
	}
	
	//	Control Texture
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	if(MipmapLevel)
	{
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	}

	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,MappingMode);
	if(RGBA)delete RGBA;

}

void CglTexture::Enable(bool yes)
{
	if(yes)
	{
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
}

void CglTexture::glAutoBuildMipmap2D(GLint internalformat, GLsizei Width, GLsizei Height, GLenum format, const GLvoid * DibPtr,GLenum Type)
{
	ASSERT(DibPtr);
	ASSERT(internalformat>=1 && internalformat<=4);
	ASSERT(Type == GL_UNSIGNED_BYTE||Type == GL_BYTE);
	
	///additional mipmap needed
	GLvoid * TmpDib;
	MipmapLevel --;
	GLint size = 1<<MipmapLevel;
	
	TmpDib = new BYTE[size*size*internalformat];
	if(TmpDib)
	{int i=0;
		while(size)
		{
			gluScaleImage(format,Width,Height,Type,DibPtr,size,size,Type,TmpDib); 
			glTexImage2D(GL_TEXTURE_2D,i++,internalformat,size,size,0,format ,Type,TmpDib);
			size >>=1;
		}

		delete TmpDib;
	}
}


///////////////////////////////////////////////////////////////////////
//Implement for class CglMaterial

int CglMaterial::LastApplied = 0;
int	CglMaterial::LastName = 0;

CglMaterial::~CglMaterial()
{

}

CglMaterial::CglMaterial()
{
	SetAmbient(0.2,0.2,0.2,1.0);
	SetDiffuse(0.8,0.8,0.8,1.0);
	SetSpecular(0.0,0.0,0.0,1.0);
	SetEmission(0.0,0.0,0.0,1.0);
	SetShiness(0.0);

	Name = ++ LastName;
}


void CglMaterial::SetAmbient(GLfloat r, GLfloat g, GLfloat b,GLfloat a)
{
	Ambient[0] = r;
	Ambient[1] = g;
	Ambient[2] = b;
	Ambient[3] = a;
}

void CglMaterial::SetDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	Diffuse[0] = r;
	Diffuse[1] = g;
	Diffuse[2] = b;
	Diffuse[3] = a;
}

void CglMaterial::SetSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	Specular[0] = r;
	Specular[1] = g;
	Specular[2] = b;
	Specular[3] = a;
}

void CglMaterial::SetEmission(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	Emission[0] = r;
	Emission[1] = g;
	Emission[2] = b;
	Emission[3] = a;
}

void CglMaterial::SetShiness(GLfloat shin)
{
	Shiness  = shin;
}

void CglMaterial::Apply(GLenum face)
{
	if(LastApplied != Name)
	{
		glMaterialfv(face,GL_AMBIENT,Ambient);
		glMaterialfv(face,GL_DIFFUSE ,Diffuse);
		glMaterialfv(face,GL_SPECULAR,Specular);
		glMaterialfv(face,GL_EMISSION ,Emission);

		glMaterialf(face,GL_SHININESS,Shiness);

		LastApplied = Name;
	}
}


