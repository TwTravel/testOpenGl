//****************************************************************************
//  Enviroment.h: interface for the CEnviroment class.
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

#if !defined(AFX_ENVIROMENT_H__5A4958EB_F3F5_4A96_855E_6F0981A4FBE1__INCLUDED_)
#define AFX_ENVIROMENT_H__5A4958EB_F3F5_4A96_855E_6F0981A4FBE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gl_helper.h"

#define ENV_DEFAULT_SIZE 10.0f
#define ENV_DEFAULT_WAVE_POWER 1.0f

class CEnviroment  
{
public:
	CEnviroment();
	CEnviroment(LPCTSTR SkyTexture,LPCTSTR GrdTexture,GLfloat SkyR,GLfloat SkyG,GLfloat SkyB);
	~CEnviroment();

	void RenderGround();
	void RenderSky();
	void SetSkyDetail(int );
	void RenderAllWithFog();

	static GLfloat EnvFogFar,EnvFogNear;

protected:
	CglTexture Sky,Ground;
	int		SkyDetail;
	GLfloat SkyR,SkyG,SkyB;
};

#endif // !defined(AFX_ENVIROMENT_H__5A4958EB_F3F5_4A96_855E_6F0981A4FBE1__INCLUDED_)
