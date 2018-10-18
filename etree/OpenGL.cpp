//****************************************************************************
//  OpenGL.cpp : Defines the class behaviors for the application.
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
#include "OpenGL.h"
#include "ControlPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenGLApp

BEGIN_MESSAGE_MAP(COpenGLApp, CWinApp)
	//{{AFX_MSG_MAP(COpenGLApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenGLApp construction

COpenGLApp::COpenGLApp()
{
	Wnd_w = 540,Wnd_h = 350;
	IsZoomed = FALSE;
	CtrlPane_X = 100,CtrlPane_Y = 100;
	bAutoClose = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COpenGLApp object

COpenGLApp theApp;

/////////////////////////////////////////////////////////////////////////////
// COpenGLApp initialization
const LPCTSTR COpenGLApp::ConfigFn = _T("TreeModl.Cfg");

BOOL COpenGLApp::InitInstance()
{
	// Standard initialization

	//AfxMessageBox("On InitInstance()");   Pause for remote debug

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	///////////////////////////////////////////
	//Load Config
	{Configuration	conf;
		CFile f;
		if(f.Open(ConfigFn,CFile::modeRead,NULL))
		{
			try
			{
				f.Read(&conf,sizeof(Configuration));
				if(conf.MainWndWidth >100 )Wnd_w = conf.MainWndWidth;
				if(conf.MainWndHeight >50 )Wnd_h = conf.MainWndHeight;

				wgl.RotX = conf.RotX;
				wgl.RotY = conf.RotY;
				wgl.OrgX = conf.OrgX;
				wgl.OrgY = conf.OrgY;
				wgl.OrgZ = conf.OrgZ;
				IsZoomed = conf.IsZoomed;

				CtrlPane_X = conf.CtrlPane_X;
				CtrlPane_Y = conf.CtrlPane_Y;
				bAutoClose = conf.bAutoClose;

				int i;
				for(i=0;i<CP_ITEMCOUNTS;i++)
				{
					if(conf.ModelParams[i] <= CControlPane::AllCtrlParam[i].Max && conf.ModelParams[i] >= CControlPane::AllCtrlParam[i].Min)
					{
						*(CControlPane::AllCtrlParam[i].Target) = conf.ModelParams[i];
					}
				}
			}
			catch(CFileException * e)
			{
				e->Delete();
			}
		}
	}

	CLeaf::InitLeafSizeNoiseSeq();

	wgl.wcp = &wcp;
	wcp.wgl = &wgl;

	this->m_pMainWnd = &wgl;

	wgl.Create(Wnd_w,Wnd_h,NULL);
	wgl.ShowWindow(SW_SHOW);
	wgl.CenterWindow();
	if(IsZoomed)wgl.ShowWindow(SW_MAXIMIZE);

	wcp.bAutoClose = bAutoClose;
	wcp.Create(IDD_Control,&wgl);
	wcp.ShowWindow(SW_HIDE);
	wcp.SetWindowPos(NULL,CtrlPane_X,CtrlPane_Y,0,0,SWP_NOZORDER|SWP_NOSIZE);

	wgl.SetFocus();

	m_bHelpMode = FALSE;

	return TRUE;
}

int COpenGLApp::ExitInstance()
{
	//Save config
	{Configuration	conf;
		CFile f;
		if(f.Open(ConfigFn,CFile::modeReadWrite|CFile::modeCreate,NULL))
		{
			conf.MainWndWidth = Wnd_w;
			conf.MainWndHeight = Wnd_h;

			conf.RotX = wgl.RotX;
			conf.RotY = wgl.RotY;
			conf.OrgX = wgl.OrgX;
			conf.OrgY = wgl.OrgY;
			conf.OrgZ = wgl.OrgZ;
			conf.IsZoomed = IsZoomed;
			conf.CtrlPane_X = CtrlPane_X;
			conf.CtrlPane_Y = CtrlPane_Y;
			conf.bAutoClose = bAutoClose;

			int i;
			for(i=0;i<CP_ITEMCOUNTS;i++)
			{
				conf.ModelParams[i] = *(CControlPane::AllCtrlParam[i].Target);
			}

			try
			{
				f.Write(&conf,sizeof(Configuration));
			}
			catch(CFileException * e)
			{
				e->Delete();
			}
		}
	}

	CLeaf::InitLeafSizeNoiseSeq(TRUE);

	return CWinApp::ExitInstance();
}
