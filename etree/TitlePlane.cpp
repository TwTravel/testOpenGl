//****************************************************************************
//  TitlePlane.cpp: implementation of the CTitlePlane class.
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
#include "TitlePlane.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTitlePlane::CTitlePlane(LPCTSTR bmp,int Type,GLfloat PosZ = -0.2):Bitmap(bmp,Type,0)
{
	RenderPos = PosZ;
}

void CTitlePlane::Render(GLfloat Zoom,GLfloat Aspect,GLfloat Alpha)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	Bitmap.Apply();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glScalef(Zoom*Aspect,Zoom,1.0);
	glColor4f(1.0,1.0,1.0,Alpha);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f); 
		glVertex3f(-1.0,-1.0,RenderPos); 

		glTexCoord2f(0.0f,1.0f); 
		glVertex3f(-1.0,1.0,RenderPos); 

		glTexCoord2f(1.0f,1.0f); 
		glVertex3f(1.0,1.0,RenderPos); 

		glTexCoord2f(1.0f,0.0f); 
		glVertex3f(1.0,-1.0,RenderPos);
	glEnd();


	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void CTitlePlane::Popup(GLfloat Zoom, GLfloat Aspect, GLfloat Alpha)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	Bitmap.Apply();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat Sa,A_inc;
	Sa = 0.2;
	A_inc = (Alpha - Sa)/20.0;
	glScalef(Zoom*Aspect,Zoom,1.0);
	for(int i=20;i>=1;i--)
	{
		glPushMatrix();
			Sleep(30);
			Sa += A_inc;
			glColor4f(1.0,1.0,1.0,Sa);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f,0.0f); 
				glVertex3f(-1.0,-1.0,RenderPos); 

				glTexCoord2f(0.0f,1.0f); 
				glVertex3f(-1.0,1.0,RenderPos); 

				glTexCoord2f(1.0f,1.0f); 
				glVertex3f(1.0,1.0,RenderPos); 

				glTexCoord2f(1.0f,0.0f); 
				glVertex3f(1.0,-1.0,RenderPos);
			glEnd();
		glPopMatrix();

		glFinish();
		SwapBuffers(wglGetCurrentDC());

	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void CTitlePlane::SetAbsPosition(int  h_align, int v_align , int h_len , int v_len)
{
	h_Align=  h_align;
	v_Align=  v_align;
	h_Len=	   h_len;
	v_Len=	   v_len;
}

CTitlePlane::ABS_POSITION_CTX CTitlePlane::BeginRenderAbsPosition(CWnd * wnd)
{
	ASSERT(wnd);
	return BeginRenderAbsPosition(wnd->GetSafeHwnd());
}

CTitlePlane::ABS_POSITION_CTX CTitlePlane::BeginRenderAbsPosition(HWND hwnd)
{
	ASSERT(hwnd);
	RECT rc;
	::GetClientRect(hwnd,&rc);

	ASSERT(rc.left == 0);
	ASSERT(rc.top == 0);

    glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glScaled(1.0/rc.right,1.0/rc.bottom,1);

	return (((DWORD)rc.right)<<16)|((DWORD)rc.bottom);
}

void CTitlePlane::EndRenderAbsPosition(void)
{
	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();

}

void CTitlePlane::RenderAbsPosition(CTitlePlane::ABS_POSITION_CTX ctx,GLfloat Alpha,const RECT * TexPxRect,GLfloat Zoom,GLfloat Aspect)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	Bitmap.Apply();

	glEnable(GL_POLYGON_SMOOTH);  
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int left,top,right,bottom,w,h;
	GLfloat	TexLt = 0.0,TexRt = 1.0,TexUp = 1.0,TexDn = 0.0;

	if(TexPxRect)
	{
		TexLt = ((GLfloat)TexPxRect->left)/((GLfloat)Bitmap.w);
		TexRt = ((GLfloat)TexPxRect->right)/((GLfloat)Bitmap.w);
		TexUp = 1.0 - ((GLfloat)TexPxRect->top)/((GLfloat)Bitmap.h);
		TexDn = 1.0 - ((GLfloat)TexPxRect->bottom)/((GLfloat)Bitmap.h);
	}

	w = ctx>>17;
	h = (ctx&0xffff)>>1;

	////// Set V
	{
		if(v_Align == TP_ALIGN_MID)
		{
			bottom = (v_Len>>1);
			top = -(v_Len>>1);
		}
		else
		{
			if(v_Align == TP_ALIGN_BTM)
			{
				top = -h;
				bottom = v_Len-h;
			}
			else
			{
				ASSERT(v_Align == TP_ALIGN_TOP);
				bottom = h;
				top = h - v_Len;
			}
		}

	}

	////////////// Set H
	{
		if(h_Align == TP_ALIGN_MID)
		{
			right = (h_Len>>1);
			left = -(h_Len>>1);
		}
		else
		{
			if(h_Align == TP_ALIGN_TOP)
			{
				left = -w;
				right = h_Len-w;
			}
			else
			{
				ASSERT(h_Align == TP_ALIGN_BTM);
				right = w;
				left = w - h_Len;
			}
		}
	}

	glPushMatrix();

		glScalef(Zoom*Aspect,Zoom,1.0);
		glColor4f(1.0,1.0,1.0,Alpha);

		glBegin(GL_QUADS);
			glTexCoord2f(TexLt,TexDn); 
			glVertex3i(left,top,RenderPos); 

			glTexCoord2f(TexLt,TexUp); 
			glVertex3i(left,bottom,RenderPos); 

			glTexCoord2f(TexRt,TexUp); 
			glVertex3i(right,bottom,RenderPos); 

			glTexCoord2f(TexRt,TexDn); 
			glVertex3i(right,top,RenderPos);
		glEnd();

	glPopMatrix();

	glDisable(GL_POLYGON_SMOOTH);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}
