//****************************************************************************
//  ControlPane.h : header file of CControlPane dialog
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

#if !defined(AFX_CONTROLPANE_H__3D152517_BA49_4181_9F76_824A0F3CB900__INCLUDED_)
#define AFX_CONTROLPANE_H__3D152517_BA49_4181_9F76_824A0F3CB900__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CControlPane dialog

#define CP_ITEMCOUNTS 13

class COpenGLWnd;

class CControlPane : public CDialog
{
private:
	static const LPCTSTR lpszNumFormat;

// Construction
public:
	void DisplayStatus(CString &str);
	CControlPane(CWnd* pParent = NULL);   // standard constructor
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	typedef struct _tagCCtrlParam
	{
		bool NeedRebuild;
		float Min;
		float Max;
		float * Target;
		LPCTSTR Name;
	}CCtrlParam;

// Dialog Data
	//{{AFX_DATA(CControlPane)
	enum { IDD = IDD_Control };
	CSliderCtrl	wSlider;
	CListBox	wItemList;
	CString	sMax;
	CString	sMin;
	int		iSlider;
	CString	sInfo;
	CString	sCurrent;
	//}}AFX_DATA

	BOOL bAutoClose;
	COpenGLWnd * wgl;
	static const CCtrlParam AllCtrlParam[CP_ITEMCOUNTS];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlPane)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateParam(bool IsGet = true);

	// Generated message map functions
	//{{AFX_MSG(CControlPane)
	afx_msg void OnSelchangeControlItem();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCustomdrawSlider(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	bool	Initialized;
	int		CurrentItem;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLPANE_H__3D152517_BA49_4181_9F76_824A0F3CB900__INCLUDED_)
