//****************************************************************************
//  Enviroment.cpp: implementation of the CEnviroment class.
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
#include "opengl.h"
#include "Enviroment.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLfloat CEnviroment::EnvFogFar = 1000;
GLfloat CEnviroment::EnvFogNear = 300;

CEnviroment::CEnviroment(LPCTSTR SkyTexture,LPCTSTR GroundTexture,GLfloat iSkyR,GLfloat iSkyG,GLfloat iSkyB)
:Sky(SkyTexture,GL_Tex_BMP,0),
 Ground(GroundTexture,GL_Tex_BMP,9)
{
	SkyDetail = 20;
	SkyR = iSkyR;
	SkyG = iSkyG;
	SkyB = iSkyB;
}

CEnviroment::CEnviroment()
:Sky(_T("sky.bmp"),GL_Tex_BMP,0),
 Ground(_T("ground.bmp"),GL_Tex_BMP,8)
{
	SkyDetail = 20;
	SkyR = 24.0/256.0;//0.09375;	
	SkyG = 60.0/256.0;//0.234375;
	SkyB = 107.0/256.0;//0.390625;
}

CEnviroment::~CEnviroment()
{
}

void CEnviroment::RenderAllWithFog()
{
	glEnable(GL_FOG);
	{
		GLfloat fogColor[4] = {0.1953125,0.3515625,0.546875, 1.0};  //color of top sky

		glFogi (GL_FOG_MODE, GL_LINEAR);
		glFogfv (GL_FOG_COLOR, fogColor);
		glFogf (GL_FOG_DENSITY, 1);
		glFogf (GL_FOG_START, EnvFogNear);
		glFogf (GL_FOG_END, EnvFogFar);
		glHint (GL_FOG_HINT, GL_DONT_CARE  );
	}

	RenderSky();
	RenderGround();

	glDisable(GL_FOG);
}

void CEnviroment::SetSkyDetail(int d)
{
	ASSERT(d>4);
	SkyDetail = d;
}


void CEnviroment::RenderSky()
{
	glDisable(GL_LIGHTING);
	glPushMatrix();
		glTranslatef(0,1,0);
		glScalef(80,120,80);
		double angle = 0.0,inc_angle = 360.0/SkyDetail;
		double tex = 0.0f,inc_tex = 3.0/SkyDetail;

		glColor3f(1,1,1);

		Sky.Apply();

		int i = 0;
		glBegin(GL_QUAD_STRIP);
		{
			for(;i<SkyDetail;i++,angle += inc_angle,tex += inc_tex)
			{float x,z;
				x = cos(Deg2Rad(angle))*ENV_DEFAULT_SIZE;
				z = sin(Deg2Rad(angle))*ENV_DEFAULT_SIZE;

				glTexCoord2f(tex,0.0f); 
				glVertex3f(x,0.0f,z); 

				glTexCoord2f(tex,1.0f); 
				glVertex3f(x,ENV_DEFAULT_SIZE,z);
			}

			glTexCoord2f(3.0f,0.0f); 
			glVertex3f(ENV_DEFAULT_SIZE,0.0f,0.0f); 
			glTexCoord2f(3.0f,1.0f); 
			glVertex3f(ENV_DEFAULT_SIZE,ENV_DEFAULT_SIZE,0.0f); 
		}
		glEnd();

		CglTexture::Enable(FALSE);
		glBegin(GL_POLYGON);
		{
			glColor3f(SkyR,SkyG,SkyB);

			for(i = 0,angle = 0.0f;i<SkyDetail;i++,angle += inc_angle)
			{float x,z;
				x = cos(Deg2Rad(angle))*ENV_DEFAULT_SIZE*2;
				z = sin(Deg2Rad(angle))*ENV_DEFAULT_SIZE*2;

				glVertex3f(x,ENV_DEFAULT_SIZE*0.9,z); 
			}

			glVertex3f(ENV_DEFAULT_SIZE*2,ENV_DEFAULT_SIZE*0.9,0); 

		}
		glEnd();
		CglTexture::Enable();

	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void CEnviroment::RenderGround()
{
	glDisable(GL_LIGHTING);
	glPushMatrix();
		glScalef(900,0,900);
		
		glColor3f(1,1,1);
		Ground.Enable(true);
		Ground.Apply();
		glBegin(GL_QUADS);
			glTexCoord2f(-8,-8); 
			glVertex3f(-1.0,0,-1); 

			glTexCoord2f(-8,8); 
			glVertex3f(-1.0,0,1); 

			glTexCoord2f(8,8); 
			glVertex3f(1.0,0,1); 

			glTexCoord2f(8,-8); 
			glVertex3f(1.0,0,-1);
		glEnd();

	glPopMatrix();
	glEnable(GL_LIGHTING);
}
