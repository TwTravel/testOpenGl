//****************************************************************************
//  TitlePlane.h: interface for the CTitlePlane class.
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

#if !defined(AFX_TITLEPLANE_H__9939EF99_9A2A_4181_B48A_4FEE69138F6B__INCLUDED_)
#define AFX_TITLEPLANE_H__9939EF99_9A2A_4181_B48A_4FEE69138F6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GL_helper.h"

#define TP_ALIGN_TOP	(-1)
#define TP_ALIGN_MID	(0)
#define TP_ALIGN_BTM	(1)
#define TP_ALIGN_LFT	TP_ALIGN_TOP
#define TP_ALIGN_RIT	TP_ALIGN_BTM

class CTitlePlane
{
public:
	typedef DWORD	ABS_POSITION_CTX;

	CTitlePlane(LPCTSTR bmp,int Type,GLfloat PosZ);
	void Popup(GLfloat Zoom,GLfloat Aspect,GLfloat Alpha);
	void Render(GLfloat Zoom,GLfloat Aspect = 1.0,GLfloat Alpha = 1.0); //Make sure no any GL Translate perfromed and the last one to rendered

protected:
	CglTexture Bitmap;
	GLfloat RenderPos;
	int		h_Align;
	int		v_Align; 
	int		h_Len;
	int		v_Len;

public:
	void SetAbsPosition(int  h_align, int v_align , int h_len , int v_len);
	void RenderAbsPosition(CTitlePlane::ABS_POSITION_CTX ctx,GLfloat Alpha,const RECT * TexPxRect = NULL,GLfloat Zoom = 1.0,GLfloat Aspect = 1.0);

	static ABS_POSITION_CTX BeginRenderAbsPosition(HWND);
	static ABS_POSITION_CTX BeginRenderAbsPosition(CWnd *);
	static void EndRenderAbsPosition(void);
};

#endif // !defined(AFX_TITLEPLANE_H__9939EF99_9A2A_4181_B48A_4FEE69138F6B__INCLUDED_)
