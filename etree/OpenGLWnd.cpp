//****************************************************************************
//  OpenGLWnd.cpp : implementation file
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
#include "OpenGLWnd.h"
#include "ControlPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenGLWnd dialog
int COpenGLWnd::MoveSpeed = 10; //it is timer interval ^_^ not real speed

COpenGLWnd::COpenGLWnd(CWnd* pParent /*=NULL*/)
	: CDialog(COpenGLWnd::IDD, pParent),
	  Help(_T("help.BMP"),GL_Tex_BMP_RGA_RRGB,-1.0),
	  Toolbar(_T("button.bmp"),GL_Tex_BMP_RGA_RRGB,-1.0)
{
	//{{AFX_DATA_INIT(COpenGLWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDC=NULL;

	///////////////////////////////////////0.000000,-9.099350,-11.970904,0.000000,-1022.000000
	//Finial Control
	MouseEvent = MS_NONE;
	KeyEvent = MouseEvent =0;
	OrgX = 0.0f;
	OrgY = -11.7872f;
	OrgZ = -8.04298f;
    RotX = -10.00f;
	RotY = 19346.0f;
	ShowTitle = false;
	GLInitialized = false;
	wcp = NULL;
	ButtonHoverState = BUTTON_HOVER_NONE;
	AutoRotating = FALSE;
}


void COpenGLWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenGLWnd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenGLWnd, CDialog)
	//{{AFX_MSG_MAP(COpenGLWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
//	ON_WM_KILLFOCUS()
//ON_WM_SETCURSOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenGLWnd message handlers

BOOL COpenGLWnd::Create(int cx,int cy, CWnd* pParentWnd,bool FullScreen)
{
	bool re;
	re=CDialog::Create(IDD, pParentWnd);
	if(re)
	{
		SetWindowPos(NULL,0,0,cx,cy,SWP_NOZORDER|SWP_NOMOVE);
	}
	return re;
}

void COpenGLWnd::OnCancel()
{
	if(IsZoomed())
	{
		ShowWindow(SW_RESTORE);
		((COpenGLApp *)AfxGetApp())->IsZoomed = TRUE;
	}
	else
		((COpenGLApp *)AfxGetApp())->IsZoomed = FALSE;

	CRect rc;
	GetWindowRect(rc);

	((COpenGLApp *)AfxGetApp())->Wnd_h = rc.Height();
	((COpenGLApp *)AfxGetApp())->Wnd_w = rc.Width();

	wcp->GetWindowRect(rc);
	((COpenGLApp *)AfxGetApp())->CtrlPane_X = rc.left;
	((COpenGLApp *)AfxGetApp())->CtrlPane_Y = rc.top;

	wcp->UpdateData();
	((COpenGLApp *)AfxGetApp())->bAutoClose = wcp->bAutoClose;

	wcp->DestroyWindow();
	DestroyWindow();
	AfxGetApp()->m_pMainWnd = NULL;
	AfxGetApp()->PostThreadMessage(WM_QUIT,0,0);
};

BOOL COpenGLWnd::DestroyWindow()
{
	if(GLInitialized)
	{
		HGLRC	hrc;
		
		hrc = ::wglGetCurrentContext();

		::wglMakeCurrent(NULL,  NULL);
		
		if (hrc)
		{
			::wglDeleteContext(hrc);
			hrc = NULL;
		}
	}

    if (m_pDC)
	{
        delete m_pDC;
		m_pDC = NULL;
	}
	
	return CDialog::DestroyWindow();
}

BOOL COpenGLWnd::OnInitDialog() 
{
	hHand = ::LoadCursor(NULL,0);//IDC_HAND
	hArrow = ::LoadCursor(NULL,IDC_ARROW);

	CDialog::OnInitDialog();

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    GetClientRect(&m_oldRect);


	if(!InitGL())
	{
		CString str;
		str.Format(_T("Failed to initialize OpenGL subsystem. \15\12Error Code:  0x%08x\15\12\15\12A hardware 3D accelerator is required.\15\12Or you'd better update your video card driver.\15\12\15\12Please contact e_boris2002@hotmail.com with\15\12the error code. Thank you for the cooperation."),GetLastError());
		AfxMessageBox(str,MB_ICONSTOP);
		return TRUE;
	}

	GLInitialized = true;

	Tree.Initialize();

	ASSERT(wcp);

	//// Detect Renderer 
	{CString str = _T(" Natural Tree Simulation on ");
		LPCSTR pStr;

		pStr = (LPCSTR)glGetString(GL_RENDERER);

		if(pStr)
		{
			str += pStr;
			pStr = (LPCSTR)glGetString(GL_VERSION);
			if(pStr)
			{
				str+=_T(" (");
				str+=pStr;
				str+=_T(")");
			}
		}
		else
			str += "UnknownDevice";

		SetWindowText(str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COpenGLWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	if(GLInitialized)Render();
}

#define FovZoom 0.6f

void COpenGLWnd::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	Width = cx,Height = cy;
	
	if(cy > 0 && GLInitialized)
    { 
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
		glFrustum(-FovZoom,FovZoom,-1.0,1.0,1.0,3.0e10);
		glViewport(0, 0, cx, cy);
		Aspect = (double)cy/(double)cx;

		Render();
    }
}

//#define BONE_ONLY 

bool COpenGLWnd::InitGL()
{
	//Setup GL pixel format
    m_pDC = new CClientDC(this);
    ASSERT(m_pDC);

	if(SetPixel())
	{
	PIXELFORMATDESCRIPTOR pfd;
		int         n;
		HGLRC		hrc;

		n =::GetPixelFormat(m_pDC->GetSafeHdc());
		::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

		hrc = wglCreateContext(m_pDC->GetSafeHdc());
		if(hrc == NULL)return FALSE;

		if(!wglMakeCurrent(m_pDC->GetSafeHdc(), hrc))return FALSE;

		glClearDepth(5.0f);
		glEnable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//TODO: Initialize some optional GL options
		CglTexture::Enable();
		glEnable(GL_LIGHTING);
		static GLfloat LightAmb[]={1.0f,1.0f,1.0f,0.5f};
		static GLfloat LightDif[]={1.0f,1.0f,1.0f,0.8f};

//		glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,spotDir);
//		glLightfv(GL_LIGHT0,GL_POSITION,spotPos);
		glLightfv(GL_LIGHT0,GL_AMBIENT,LightAmb);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,LightDif);

		static float LightPos[4]={1.0f,0.0f,0.0f,0.0f};
		glLightfv(GL_LIGHT0,GL_POSITION,LightPos);

		glEnable(GL_LIGHT0);

		/////////////////////////////////////////////////////////////
		//Must disable this when rendering leaf, for, on ATI Riva128 I740 Video card,there will
		//be a box-edge on every leaf. I think this option is collision with
		//GL_ALPHA_TEST
		//
		//glEnable(GL_POLYGON_SMOOTH);  

		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

#ifdef BONE_ONLY
		glPolygonMode(GL_FRONT,GL_LINE);
		glPolygonMode(GL_BACK,GL_LINE);

		// Antialiasing
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
#endif
#ifdef POINT_ONLY
		glPolygonMode(GL_FRONT,GL_POINT);
		glPolygonMode(GL_BACK,GL_POINT);
#endif
#ifdef SINLE_SIDE
		glPolygonMode(GL_FRONT,GL_POINT);
		glPolygonMode(GL_BACK,GL_FILL);
#endif

	}
	else
	{
		return false;
	}

	return TRUE;
}

bool COpenGLWnd::SetPixel()
{
static PIXELFORMATDESCRIPTOR pfd = 
	{
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                              // version number
        PFD_DRAW_TO_WINDOW|             // support window
			PFD_SUPPORT_OPENGL|		    // support OpenGL
			PFD_DOUBLEBUFFER,           // double buffered,		
        PFD_TYPE_RGBA,                  // RGBA type
        16,                             // 24-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // accum bits ignored
        24,                             // 32-bit z-buffer
        0,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                // main layer
        0,                              // reserved
        0, 0, 0                         // layer masks ignored
    };

    int pixelformat;

    if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
    {
        return FALSE;
    }

    if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}

void COpenGLWnd::Render()
{
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	///////////////////////////////////////////////////////
	//Move the Camera,set view position
	glRotatef(RotX,1,0,0);
	glTranslatef(0,OrgY,OrgZ);
	glScalef(0.3,0.3,0.3);

	///////////////////////////////////////////////////////////
	//render the whole world
	CTree::TotalVertex = CTree::TotalQuad = CTree::TotalNode = CTree::TotalMemory = 0;

	//////////////////////////////////////////////
	//1. Render Sky and ground
#ifndef BONE_ONLY
	glScalef(Aspect,1.0,1.0);
	glPushMatrix();
		glRotatef(RotY,0,1,0); //Rotate the camera

		Env.RenderSky();
		Env.RenderGround();
	glPopMatrix();
	glScalef(1.0f,1.0f/FovZoom,1.0f);
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	//2. Render shadow of the tree
	Tree.DrawShadow(-RotY);
#endif
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	//3. Render the tree
#ifdef BONE_ONLY
	CglTexture::Enable(FALSE);
	glColor3f(1.0,1.0,1.0);
#endif
	glPushMatrix();
		glRotatef(RotY,0,1,0);//Rotate the camera

		Tree.Render();
	glPopMatrix();
	//////////////////////////////////////////////////

#ifndef BONE_ONLY
	///////////////////////////////////////////////
	//4 Render Toolbar and help
	glLoadIdentity();
	glScaled(FovZoom*2.0,2,1);	
#define BUTTON_ALPHA	0.6
#define HOVER_BUTTON_ALPHA	0.85
#define TITLE_ALPHA		0.65
	{
		CTitlePlane::ABS_POSITION_CTX ctx;
		ctx = CTitlePlane::BeginRenderAbsPosition(this);
		{
			const static RECT TexPart[] = 
			{
				{0,155,256,256},	//Help for buttons
				{0,0,255,155},		//Help for Title

				{0,0,37,128},		//By Boris
				{37,0,256,61},		//Title Tree
				{44,61,256,128}		//Bottons
			};

			//By Boris J. Wang
			glPushMatrix();
				glTranslatef(5,0,0);
				Toolbar.SetAbsPosition(TP_ALIGN_TOP,TP_ALIGN_TOP,30,200);
				Toolbar.RenderAbsPosition(ctx,TITLE_ALPHA,&TexPart[2]);
			glPopMatrix();

			//Fractal Tree
			glPushMatrix();
				glTranslatef(27.5,-4.3,0);
				Toolbar.SetAbsPosition(TP_ALIGN_TOP,TP_ALIGN_TOP,220,60);
				Toolbar.RenderAbsPosition(ctx,TITLE_ALPHA,&TexPart[3]);
			glPopMatrix();

			//Buttons
			glPushMatrix();
				glTranslatef(-7,7,0);
				Toolbar.SetAbsPosition(TP_ALIGN_BTM,TP_ALIGN_BTM,200,60);
				Toolbar.RenderAbsPosition(ctx,ButtonHoverState?HOVER_BUTTON_ALPHA:BUTTON_ALPHA,&TexPart[4]);
			glPopMatrix();

			if(ShowTitle)
			{
				//Help for buttons
				glPushMatrix();
					glTranslatef(0,47.5,0);
					Help.SetAbsPosition(TP_ALIGN_BTM,TP_ALIGN_BTM,256,256-155);
					Help.RenderAbsPosition(ctx,TITLE_ALPHA,&TexPart[0]);
				glPopMatrix();

				//Help for Title
				glPushMatrix();
					glTranslatef(29,-52,0);
					Help.SetAbsPosition(TP_ALIGN_TOP,TP_ALIGN_TOP,300,120);
					Help.RenderAbsPosition(ctx,TITLE_ALPHA,&TexPart[1]);
				glPopMatrix();
			}
		}
		CTitlePlane::EndRenderAbsPosition();
	}
#undef BUTTON_ALPHA
#undef HOVER_BUTTON_ALPHA
#undef TITLE_ALPHA

#endif
	////////////////////////////////////////////////

	////////////////////////////////////////////////
	//5. Flip scene
	glFinish();
	SwapBuffers(wglGetCurrentDC());
	////////////////////////////////////////////////

	////////////////////////////////////////////////
	//End of rendering

	////////////////////////////////////////////////
	//Output status information
	CString str;
	str.Format(_T("[Model]  Object: %d Nodes\15\12              Memory: %1.2f KB\15\12[Scene]  Elevation: %3.1f Deg.\15\12              Pos.(%2.1f,%2.1f,%2.1f)\15\12[Mesh.]  %7d Vertices\15\12              %7d Quadrangles"),
			   CTree::TotalNode,(float)CTree::TotalMemory/1024.0f,
			   -RotX,
		       sin(Deg2Rad(RotY))*OrgZ,-OrgY,cos(Deg2Rad(RotY))*OrgZ,
			   CTree::TotalVertex + 50,CTree::TotalQuad + 22
			   );
	//str.Format("[模型]  总结点个数: %d个\15\12             存储器分配: %1.2f KB\15\12[场景]  视点: (%2.1f,%2.1f,%2.1f)\15\12             观察仰角: %3.2f度\15\12[渲染]  总顶点个数: %d个\15\12             四边形个数: %d个",
	//		   CTree::TotalNode,(float)CTree::TotalMemory/1024.0f,
	//	       sin(Deg2Rad(RotY))*OrgZ,-OrgY,cos(Deg2Rad(RotY))*OrgZ,
	//		   RotX,
	//		   CTree::TotalVertex + 50,CTree::TotalQuad + 22
	//		   );
	//str.Format("%f,%f,%f,%f,%f",OrgX,OrgY,OrgZ,RotX,RotY);
	wcp->DisplayStatus(str);
}

void COpenGLWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(ButtonHoverState == BUTTON_HOVER_NONE)
	{
		MouseEvent = MS_LBUT;
		MouseX = (point.x - Width>>1)/(float)(Width>>1);
		MouseY = (Height>>1 - point.y)/(float)(Height>>1);
		SetTimer(MOUSE_TIMER,MoveSpeed,NULL);
		SetCapture();
	}

	if(ButtonHoverState)
		SetCursor(hHand);
	else
		SetCursor(hArrow);
}

void COpenGLWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(MouseEvent != MS_NONE)
	{
		MouseEvent = MS_NONE;
		KillTimer(MOUSE_TIMER);
		ReleaseCapture();
		return;
	}

	if(ButtonHoverState)
		SetCursor(hHand);
	else
		SetCursor(hArrow);

	MSG msg;
	msg.message = WM_KEYDOWN;
    switch(ButtonHoverState)
	{
	case BUTTON_HOVER_SPIN:
		{
			msg.wParam = 's';
			break;
		}
	case BUTTON_HOVER_HELP:
		{
			msg.wParam = 'h';
			break;
		}
	case BUTTON_HOVER_TREE:
		{
			msg.wParam = 't';
			break;
		}
	case BUTTON_HOVER_MAIL:
		{
			msg.wParam = 'm';
			break;
		}
	case BUTTON_HOVER_NONE:
		{
			return;
		}
	default:ASSERT(0);	
	}

	PreTranslateMessage(&msg);
}

void COpenGLWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(MouseEvent)
	{
		MouseX = (point.x - Width>>1)/(float)(Width>>1);
		MouseY = (Height>>1 - point.y)/(float)(Height>>1);
	}

	int v = Height - point.y;

	bool Org_State = ButtonHoverState;
	ButtonHoverState = BUTTON_HOVER_NONE;

	if(v>32 && v<62)
	{
		v = Width - point.x;
		if(v>18 && v<39)
		{
			ButtonHoverState = BUTTON_HOVER_HELP;
		}
		else
			if(v>59 && v<80)
			{
				ButtonHoverState = BUTTON_HOVER_TREE;
			}
			else
				if(v>102 && v<120)
				{
					ButtonHoverState = BUTTON_HOVER_MAIL;
				}
				else
					if(v>142 && v<161)ButtonHoverState = BUTTON_HOVER_SPIN;
	}

	if(ButtonHoverState)
		SetCursor(hHand);
	else
		SetCursor(hArrow);

	if(Org_State != (bool)ButtonHoverState && !AutoRotating)Render();
}

void COpenGLWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	MouseEvent = MS_RBUT;
	MouseX = (point.x - Width>>1)/(float)(Width>>1);
	MouseY = (Height>>1 - point.y)/(float)(Height>>1);
	SetTimer(MOUSE_TIMER,MoveSpeed,NULL);
	SetCapture();
}

void COpenGLWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	MouseEvent = MS_NONE;
	KillTimer(MOUSE_TIMER);
	ReleaseCapture();
}

BOOL COpenGLWnd::PreTranslateMessage(MSG* pMsg) 
{
	////Prevent MFC from handling F1 help hotkey
	if (pMsg->message == 0x4d)return TRUE;

	if(pMsg->message == WM_KEYDOWN)
	{
		if( pMsg->wParam == 's' || pMsg->wParam == 'S' )
		{
			if(!AutoRotating)
			{
				SetTimer(ROTATE_TIMER,50,NULL);
				AutoRotating = TRUE;
			}
			else
			{
				KillTimer(ROTATE_TIMER);
				AutoRotating = FALSE;
			}
			return true;
		}

		if( pMsg->wParam == 'h' || pMsg->wParam == 'H' )
		{
			KeyEvent = pMsg->wParam;
			ToggleHelpWnd();
			return true;
		}

		if( pMsg->wParam == 't' || pMsg->wParam == 'T' )
		{
			wcp->ShowWindow(SW_SHOW);
			wcp->SetFocus();
			return true;
		}

		if( pMsg->wParam == 'm' || pMsg->wParam == 'M' )
		{
			ShellExecute(NULL,_T("open"),_T("mailto:e_boris2002@hotmail.com?subject=Feedback_to_Fractal_Tree_(OpenGL_Version)"),NULL,NULL,SW_SHOW);
			return true;
		}

		if(KeyEvent == pMsg->wParam)return true;
		KeyEvent = pMsg->wParam;
		if( KeyEvent == VK_LEFT  ||  KeyEvent == VK_UP    ||  
			KeyEvent == VK_RIGHT ||  KeyEvent == VK_DOWN   )
		{
			OnTimer(KEY_TIMER);
			SetTimer(KEY_TIMER,MoveSpeed,NULL);
			return true;
		}
	}

	if(pMsg->message == WM_KEYUP)
	{
		KeyEvent = pMsg->wParam;
		if( KeyEvent == VK_LEFT  ||  KeyEvent == VK_UP    ||  
			KeyEvent == VK_RIGHT ||  KeyEvent == VK_DOWN   )
		{
			KeyEvent = 0;
			KillTimer(KEY_TIMER);
			return true;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

#define XY_MOVING_SPEED 0.4f
#define XY_ANGLE_SPEED   4.0f

void COpenGLWnd::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == MOUSE_TIMER)
	{
		ASSERT(MouseEvent);
		float oy,oz;
		oy = sin(Deg2Rad(RotX))*XY_MOVING_SPEED;
		oz = cos(Deg2Rad(RotX))*XY_MOVING_SPEED;
		if(MouseEvent == MS_LBUT)
		{// go forward
			OrgY += oy;
			if(OrgY >= -1.5 || OrgY <= -60)
			{
				OrgY -= oy;
			}

			OrgZ += oz;
			if(OrgZ >= -3.2 || OrgZ < -130)
			{
				OrgZ -= oz;
			}
		}
		else
		{// go backward
			ASSERT(MouseEvent == MS_RBUT);
			OrgY -= oy;
			if(OrgY >= -1.5 || OrgY <= -60)
			{
				OrgY += oy;
			}

			OrgZ -= oz;
			if(OrgZ >= -3.2 || OrgZ < -130)
			{
				OrgZ += oz;
			}
		}
		Render();
		return;
	}

	if(nIDEvent == KEY_TIMER)
	{
		ASSERT(KeyEvent);
		switch(KeyEvent)
		{
		case VK_UP:
			{
				RotX -= XY_ANGLE_SPEED/2;
				Render();
				break;
			}
		case VK_DOWN:
			{
				RotX += XY_ANGLE_SPEED/2;
				Render();
				break;
			}
		case VK_LEFT:
			{
				RotY -= XY_ANGLE_SPEED;
				Render();
				break;
			}
		case VK_RIGHT:
			{
				RotY += XY_ANGLE_SPEED;
				Render();
				break;
			}
		default:
			ASSERT(0);
		}
		return;
	}

	if(nIDEvent == ROTATE_TIMER)
	{
		RotY += XY_ANGLE_SPEED/8;
		Render();
		return;
	}
}

void COpenGLWnd::ToggleHelpWnd()
{
	if(ShowTitle)
	{
		ShowTitle = false;
		Render();
	}
	else
	{
		ShowTitle = true;
		Render();
	}
}

void COpenGLWnd::RebuildTreeModel()
{
	Tree.Rebuild(CTree::TreeDepth);
}
