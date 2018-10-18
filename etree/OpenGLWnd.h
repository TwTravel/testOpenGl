//****************************************************************************
//  OpenGLWnd.h : header file of COpenGLWnd dialog
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

#if !defined(AFX_OPENGLWND_H__EF3380CF_1F06_11D4_9D13_000021E6EBF5__INCLUDED_)
#define AFX_OPENGLWND_H__EF3380CF_1F06_11D4_9D13_000021E6EBF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COpenGLWnd dialog
#include "Tree.h"
#include "Branch.h"
#include "Enviroment.h"
#include "TitlePlane.h"

#define MS_NONE 0
#define MS_LBUT 1
#define MS_RBUT 2

#define KEY_TIMER		WM_USER + 1001
#define MOUSE_TIMER		WM_USER + 1002
#define ROTATE_TIMER	WM_USER + 1003

#define BUTTON_HOVER_NONE	0
#define BUTTON_HOVER_SPIN	1
#define BUTTON_HOVER_HELP	2
#define BUTTON_HOVER_TREE	3
#define BUTTON_HOVER_MAIL	4

class CControlPane;

class COpenGLWnd : public CDialog
{
// Construction
public:
	void RebuildTreeModel();
	void Render();
	COpenGLWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COpenGLWnd)
	enum { IDD = IDD_GLWnd };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CRect m_oldRect;
	CDC * m_pDC;	

	GLfloat OrgX,OrgY,OrgZ;
	GLfloat RotX,RotY;
	int		ButtonHoverState;
	BOOL	AutoRotating;

	double Aspect;
	int Width,Height;


	CControlPane * wcp;
	CEnviroment Env;
	CTree Tree;
	CTitlePlane Help,Toolbar;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenGLWnd)
	public:
	virtual BOOL Create(int cx,int cy, CWnd* pParentWnd = NULL,bool FullScreen = false);
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
	void ToggleHelpWnd();
	bool InitGL();

	static int MoveSpeed;

	// Generated message map functions
	//{{AFX_MSG(COpenGLWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnOK(){};
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool	SetPixel();
	bool	GLInitialized;
	HICON	m_hIcon;
	HCURSOR		hHand;
	HCURSOR		hArrow;

	////////////////////////////////////////////////////
	//Final Control
	WPARAM  KeyEvent;
	int		MouseEvent;
	float	MouseX,MouseY;

	//////////////////////////////////////
	//Title 
	bool ShowTitle;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGLWND_H__EF3380CF_1F06_11D4_9D13_000021E6EBF5__INCLUDED_)
