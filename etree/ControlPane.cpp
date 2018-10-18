//****************************************************************************
//  ControlPane.cpp : implementation file of the CControlPane class.
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
#include "branch.h"
#include "Enviroment.h"
#include "Leaf.h"
#include "Tree.h"
#include "OpenGLWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlPane dialog
const CControlPane::CCtrlParam CControlPane::AllCtrlParam[CP_ITEMCOUNTS] = 
{
	{true,4,30.0,&CTree::TreeDepth,_T("Tree Total Depth")},
	{true,0.0,90.0,&CSegment::SegExpand,_T("SEG. Expand Angle")},
	{true,0.1,0.99,&CSegment::SegScaleLen,_T("SEG. Len. Attenuate")},
	{true,0.1,0.99,&CSegment::SegScaleRad,_T("SEG. Rad. Attenuate")},
	{true,0.0,180.0,&CSegment::SegTwistInc,_T("SEG. Twist Angle")},

	{true,0.0,180.0,&CSegment::SegSideExpand,_T("SSE. Bifurcate Angle")},
	{true,0.1,1.0,&CSegment::SegSideScaleDepth,_T("SSE. Dep. Attenuate")},
	{true,0.1,1.0,&CSegment::SegSideScale,_T("SSE. Scale Attenuate")},

	{true,0.1,4.0,&CSegment::SegLeafSize,_T("Leaf Size")},
	{true,0.0,90.0,&CSegment::LeafExpand,_T("Leaf Expand Angle")},

	{false,0.5,5.0,&CTree::TreeShadowLength,_T("Tree Shadow Length")},
	{false,0.0,1.0,&CTree::BranchShadowDensity,_T("Brush Shad. Density")},
	{false,0.0,1.0,&CTree::LeafShadowDensity,_T("Leaf Shadow Density")}
};

CControlPane::CControlPane(CWnd* pParent /*=NULL*/)
	: CDialog(CControlPane::IDD, pParent)
	, bAutoClose(FALSE)
{
	//{{AFX_DATA_INIT(CControlPane)
	sCurrent = sMax = sMin = _T("n/a");
	iSlider = 0;
	sInfo = _T("");
	//}}AFX_DATA_INIT
	Initialized = false;
	CurrentItem = LB_ERR;
	wgl = NULL;
}


void CControlPane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CControlPane)
	DDX_Control(pDX, IDC_Slider, wSlider);
	DDX_Control(pDX, IDC_ControlItem, wItemList);
	DDX_Text(pDX, IDC_Max, sMax);
	DDX_Text(pDX, IDC_Min, sMin);
	DDX_Slider(pDX, IDC_Slider, iSlider);
	DDX_Text(pDX, IDC_Status, sInfo);
	DDX_Text(pDX, IDC_Current, sCurrent);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_AUTO, bAutoClose);
}


BEGIN_MESSAGE_MAP(CControlPane, CDialog)
	//{{AFX_MSG_MAP(CControlPane)
	ON_LBN_SELCHANGE(IDC_ControlItem, OnSelchangeControlItem)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_Slider, OnCustomdrawSlider)
	//}}AFX_MSG_MAP
//	ON_WM_KILLFOCUS()
//	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlPane message handlers
const LPCTSTR CControlPane::lpszNumFormat = _T("%3.2f");

void CControlPane::OnSelchangeControlItem() 
{
	CurrentItem = wItemList.GetCurSel();
	if(CurrentItem != LB_ERR)
	{
		ASSERT(CurrentItem >=0 && CurrentItem < CP_ITEMCOUNTS);
		
		Initialized = false;
		UpdateParam(false);
		Initialized = true;
	}

	sMax.Format(lpszNumFormat,AllCtrlParam[CurrentItem].Max);
	sMin.Format(lpszNumFormat,AllCtrlParam[CurrentItem].Min);
	sCurrent.Format(lpszNumFormat,*AllCtrlParam[CurrentItem].Target);

	UpdateData(false);
}

BOOL CControlPane::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	wSlider.SetRange(0,1000);

	Initialized = true;

	for(int i = 0;i<CP_ITEMCOUNTS;i++)
	{
		wItemList.InsertString(i,AllCtrlParam[i].Name);
	}

	ASSERT(wgl);

	return TRUE;  
}

void CControlPane::OnOK() 
{
}

void CControlPane::OnCancel() 
{
	ShowWindow(SW_HIDE);
}

void CControlPane::UpdateParam(bool IsGet)
{
	ASSERT(CurrentItem >=0 );
	if(IsGet)
	{
		if(!Initialized)return;

		UpdateData();
		(* AllCtrlParam[CurrentItem].Target) = (AllCtrlParam[CurrentItem].Max - AllCtrlParam[CurrentItem].Min)*(1000 - iSlider)/1000 + AllCtrlParam[CurrentItem].Min;
		if(AllCtrlParam[CurrentItem].NeedRebuild)
		{
			wgl->RebuildTreeModel();
		}
		
		wgl->Render();
	}
	else
	{
		iSlider = 1000*(1.0 - ((* AllCtrlParam[CurrentItem].Target - AllCtrlParam[CurrentItem].Min)/(AllCtrlParam[CurrentItem].Max - AllCtrlParam[CurrentItem].Min)));
		UpdateData(false);
	}
}

void CControlPane::OnLButtonDown(UINT nFlags, CPoint point) 
{
	 PostMessage(WM_NCLBUTTONDOWN , HTCAPTION , MAKELPARAM(point.x,point.y));
}

void CControlPane::OnCustomdrawSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(!Initialized || CurrentItem == LB_ERR)return;

	UpdateParam();
	sCurrent.Format(lpszNumFormat,*AllCtrlParam[CurrentItem].Target);

	UpdateData(false);
	
	*pResult = 0;
}

void CControlPane::DisplayStatus(CString &str)
{
	if(!Initialized)return;

	UpdateData();

	sInfo = str;
	UpdateData(false);
}

void CControlPane::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	UpdateData();
	if(WA_INACTIVE == nState && bAutoClose)ShowWindow(SW_HIDE);
}