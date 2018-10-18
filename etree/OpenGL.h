//****************************************************************************
//  OpenGL.h : main header file for the OPENGL application
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

#if !defined(AFX_OPENGL_H__EF3380C4_1F06_11D4_9D13_000021E6EBF5__INCLUDED_)
#define AFX_OPENGL_H__EF3380C4_1F06_11D4_9D13_000021E6EBF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// COpenGLApp:
// See OpenGL.cpp for the implementation of this class
//

#include "openglwnd.h"
#include "controlpane.h"


class COpenGLApp : public CWinApp
{
private:
	static const LPCTSTR ConfigFn;

public:

	typedef struct _tagConfiguration
	{
		int	MainWndWidth;
		int MainWndHeight;
		BOOL	IsZoomed;

		int CtrlPane_X,CtrlPane_Y;
		BOOL	bAutoClose;

		float OrgX,OrgY,OrgZ;
		float RotX,RotY;

		float	ModelParams[CP_ITEMCOUNTS];
	}Configuration,LPConfiguration;

	COpenGLApp();

	int Wnd_w,Wnd_h;
	BOOL	IsZoomed;
	int CtrlPane_X,CtrlPane_Y;
	BOOL	bAutoClose;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenGLApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(COpenGLApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	COpenGLWnd wgl;
	CControlPane wcp;

public:
	virtual int ExitInstance();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGL_H__EF3380C4_1F06_11D4_9D13_000021E6EBF5__INCLUDED_)
